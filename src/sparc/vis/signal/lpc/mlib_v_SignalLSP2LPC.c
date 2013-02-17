/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_SignalLSP2LPC.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLSP2LPC_S16   - convert line spectral pair coefficients
 *                                to linear prediction coefficients
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLSP2LPC_S16(mlib_s16       *lpc,
 *                                         mlib_s32       lscale,
 *                                         const mlib_s16 *lsp,
 *                                         mlib_s32       order);
 *      mlib_status mlib_SignalLSP2LPC_S16_Adp(mlib_s16       *lpc,
 *                                             mlib_s32       *lscale,
 *                                             const mlib_s16 *lsp,
 *                                             mlib_s32       order);
 * ARGUMENTS
 *      lpc     the linear prediction coefficients.
 *      lscale  the scaling factor of the linear prediction coefficients,
 *              where actual_data = output_data * 2^(-scaling_factor).
 *      lsp     the line spectral pair coefficients,
 *              the coefficients are in Q15 format.
 *      order   the order of the linear prediction filter.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if function completed successfully.
 *      MLIB_FAILURE is returned if error is encountered.
 *
 * DESCRIPTION
 *      The line spectral pair (LPS) coefficients are defined as the
 *      roots of the following two polynomials
 *
 *                             -(M+1)     -1
 *              P(z) = A(z) + z      * A(z  )
 *
 *                             -(M+1)     -1
 *              Q(z) = A(z) - z      * A(z  )
 *
 *      where A(z) is the inverse filter
 *
 *                         M          -i
 *              A(z) = 1- SUM a(i) * z
 *                        i = 1
 *
 *      Note that since P(z) is symmetric and Q(z) is antisymmetric
 *      all roots of these polynomials are on the unit circle and
 *      they alternate each other. P(z) has a root at z = -1 (w = PI)
 *      and Q(z) has a root at z = 1 (w = 0).
 *
 *      The line spectral frequency (LPF) are the angular frequency
 *      of the line spectral pair (LPS) coefficients.
 *
 *              q = cos(w)
 *
 *      where q is the LPS and w is the LPF.
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 *
 *      Note for functions with adaptive scaling (with _Adp postfix),
 *      the scaling factor of the output data will be calculated based
 *      on the actual data; for functions with non-adaptive scaling
 *      (without _Adp postfix), the user supplied scaling factor will
 *      be used and the output will be saturated if necessary.
 *
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while lpc[0] element is not initialized and
 *      not used.
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalLPC.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLSP2LPC_S16 = __mlib_SignalLSP2LPC_S16
#pragma weak mlib_SignalLSP2LPC_S16_Adp = __mlib_SignalLSP2LPC_S16_Adp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLSP2LPC_S16) mlib_SignalLSP2LPC_S16
    __attribute__((weak, alias("__mlib_SignalLSP2LPC_S16")));
__typeof__(__mlib_SignalLSP2LPC_S16_Adp) mlib_SignalLSP2LPC_S16_Adp
    __attribute__((weak, alias("__mlib_SignalLSP2LPC_S16_Adp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ONE_HALF	(mlib_d64)16384
#define	ONE	(mlib_d64)32768
#define	TWO	(mlib_d64)0.00006103515625

#define	ORDER	100

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + (shift)) << 20

/* *********************************************************** */

mlib_status
__mlib_SignalLSP2LPC_S16(
    mlib_s16 *lpc,
    mlib_s32 lscale,
    const mlib_s16 *lsp,
    mlib_s32 order)
{
	mlib_d64 tmp_dlpc[ORDER + 10], tmp_dlsp[ORDER + 10], scale;
	mlib_d64 str1, str2, str3, str4, *dsp, src, dst;
	mlib_d64 *dlpc, *dlsp, *pdlsp, *buf = NULL;
	mlib_f32 fone = vis_to_float(0x00010001);
	mlib_s32 i, order1, offset, mask;
	mlib_status res;

	if (order < 1)
		return (MLIB_FAILURE);

	if (lpc == NULL || lsp == NULL)
		return (MLIB_FAILURE);

	if (order > ORDER) {
		buf =
		    (mlib_d64 *)__mlib_malloc(2 * (order +
		    10) * sizeof (mlib_d64));

		if (buf == NULL)
			return (MLIB_FAILURE);
		dlpc = buf;
		dlsp = dlpc + (order + 10);
	} else {
		dlpc = tmp_dlpc;
		dlsp = tmp_dlsp;
	}

	vis_write_gsr(0);

	order1 = order;
	pdlsp = dlsp;

	offset = (((mlib_addr)lsp & 7) >> 1);
	dlsp += offset;
	order1 += offset;
	dsp = (mlib_d64 *)(lsp - offset);

#pragma pipeloop(0)
	for (i = 0; i <= order1 - 8; i += 8) {
		src = dsp[0];
		str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
		pdlsp[0] = *((mlib_s32 *)&str1);
		pdlsp[1] = *((mlib_s32 *)&str1 + 1);
		str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
		pdlsp[2] = *((mlib_s32 *)&str2);
		pdlsp[3] = *((mlib_s32 *)&str2 + 1);

		src = dsp[1];
		str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
		pdlsp[4] = *((mlib_s32 *)&str1);
		pdlsp[5] = *((mlib_s32 *)&str1 + 1);
		str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
		pdlsp[6] = *((mlib_s32 *)&str2);
		pdlsp[7] = *((mlib_s32 *)&str2 + 1);

		pdlsp += 8;
		dsp += 2;
	}

	if (i <= order1 - 4) {
		src = dsp[0];
		str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
		pdlsp[0] = *((mlib_s32 *)&str1);
		pdlsp[1] = *((mlib_s32 *)&str1 + 1);
		str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
		pdlsp[2] = *((mlib_s32 *)&str2);
		pdlsp[3] = *((mlib_s32 *)&str2 + 1);

		pdlsp += 4;
		dsp += 1;
		i += 4;
	}

	if (i < order1) {
		if (i == order1 - 3) {
			src = dsp[0];
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			pdlsp[0] = *((mlib_s32 *)&str1);
			pdlsp[1] = *((mlib_s32 *)&str1 + 1);
			str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
			pdlsp[2] = *((mlib_s32 *)&str2);
		} else if (i == order1 - 2) {
			src = dsp[0];
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			pdlsp[0] = *((mlib_s32 *)&str1);
			pdlsp[1] = *((mlib_s32 *)&str1 + 1);
		} else {
			src = dsp[0];
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			pdlsp[0] = *((mlib_s32 *)&str1);
		}
	}

	lpc++;
	offset = (((mlib_addr)lpc & 7) >> 1);

/* Fix <rtc> Read from uninitialized (rui) */
	dlpc[1] = 0;
	dlpc[2] = 0;
	dlpc[3] = 0;

	res = mlib_SignalLSP2LPC_double(dlpc + offset, dlsp, order);

	if (res == MLIB_SUCCESS) {
		SCALBN(scale, 1 + lscale);
		dlpc++;

		order1 = order + offset;
		mask = 0xF >> offset;
		dsp = (mlib_d64 *)(lpc - offset);

		dlpc[order1] = 0;
		dlpc[order1 + 1] = 0;
		dlpc[order1 + 2] = 0;
		dlpc[order1 + 3] = 0;

		FLOAT2INT_CLAMP(*((mlib_s32 *)&str3), dlpc[0] * scale + 32768);
		FLOAT2INT_CLAMP(*((mlib_s32 *)&str3 + 1),
		    dlpc[1] * scale + 32768);
		FLOAT2INT_CLAMP(*((mlib_s32 *)&str4), dlpc[2] * scale + 32768);
		FLOAT2INT_CLAMP(*((mlib_s32 *)&str4 + 1),
		    dlpc[3] * scale + 32768);
		dst = vis_fpackfix_pair(str3, str4);
		dlpc += 4;

#pragma pipeloop(0)
		for (i = 0; i <= order1 - 4; i += 4) {
			vis_pst_16(dst, dsp, mask);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str3),
			    dlpc[0] * scale + 32768);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str3 + 1),
			    dlpc[1] * scale + 32768);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str4),
			    dlpc[2] * scale + 32768);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str4 + 1),
			    dlpc[3] * scale + 32768);
			dst = vis_fpackfix_pair(str3, str4);
			dlpc += 4;
			dsp++;
			mask = 0xF;
		}

		if (i < order1) {
			mask &= (0xF << (4 - order1 + i));
			vis_pst_16(dst, dsp, mask);
		}
	}

	if (buf != NULL)
		__mlib_free(buf);
	return (res);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLSP2LPC_S16_Adp(
    mlib_s16 *lpc,
    mlib_s32 *lscale,
    const mlib_s16 *lsp,
    mlib_s32 order)
{
	mlib_d64 tmp_dlpc[ORDER + 10], tmp_dlsp[ORDER + 10], scale;
	mlib_d64 str1, str2, str3, str4, *dsp, src, dst, tmp;
	mlib_d64 *dlpc, *dlsp, *pdlsp, *buf = NULL;
	mlib_f32 fone = vis_to_float(0x00010001);
	mlib_s32 i, order1, offset, mask, iscale;
	mlib_status res;

	if (order < 1)
		return (MLIB_FAILURE);

	if (lpc == NULL || lsp == NULL)
		return (MLIB_FAILURE);

	if (order > ORDER) {
		buf =
		    (mlib_d64 *)__mlib_malloc(2 * (order +
		    10) * sizeof (mlib_d64));

		if (buf == NULL)
			return (MLIB_FAILURE);
		dlpc = buf;
		dlsp = dlpc + (order + 10);
	} else {
		dlpc = tmp_dlpc;
		dlsp = tmp_dlsp;
	}

	vis_write_gsr(0);

	order1 = order;
	pdlsp = dlsp;

	offset = (((mlib_addr)lsp & 7) >> 1);
	dlsp += offset;
	order1 += offset;
	dsp = (mlib_d64 *)(lsp - offset);

#pragma pipeloop(0)
	for (i = 0; i <= order1 - 8; i += 8) {
		src = dsp[0];
		str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
		pdlsp[0] = *((mlib_s32 *)&str1);
		pdlsp[1] = *((mlib_s32 *)&str1 + 1);
		str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
		pdlsp[2] = *((mlib_s32 *)&str2);
		pdlsp[3] = *((mlib_s32 *)&str2 + 1);

		src = dsp[1];
		str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
		pdlsp[4] = *((mlib_s32 *)&str1);
		pdlsp[5] = *((mlib_s32 *)&str1 + 1);
		str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
		pdlsp[6] = *((mlib_s32 *)&str2);
		pdlsp[7] = *((mlib_s32 *)&str2 + 1);

		pdlsp += 8;
		dsp += 2;
	}

	if (i <= order1 - 4) {
		src = dsp[0];
		str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
		pdlsp[0] = *((mlib_s32 *)&str1);
		pdlsp[1] = *((mlib_s32 *)&str1 + 1);
		str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
		pdlsp[2] = *((mlib_s32 *)&str2);
		pdlsp[3] = *((mlib_s32 *)&str2 + 1);

		pdlsp += 4;
		dsp += 1;
		i += 4;
	}

	if (i < order1) {
		if (i == order1 - 3) {
			src = dsp[0];
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			pdlsp[0] = *((mlib_s32 *)&str1);
			pdlsp[1] = *((mlib_s32 *)&str1 + 1);
			str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
			pdlsp[2] = *((mlib_s32 *)&str2);
		} else if (i == order1 - 2) {
			src = dsp[0];
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			pdlsp[0] = *((mlib_s32 *)&str1);
			pdlsp[1] = *((mlib_s32 *)&str1 + 1);
		} else {
			src = dsp[0];
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			pdlsp[0] = *((mlib_s32 *)&str1);
		}
	}

	lpc++;
	offset = (((mlib_addr)lpc & 7) >> 1);

/* Fix <rtc> Read from uninitialized (rui) */
	dlpc[1] = 0;
	dlpc[2] = 0;
	dlpc[3] = 0;

	res = mlib_SignalLSP2LPC_double(dlpc + offset, dlsp, order);

	if (res == MLIB_SUCCESS) {
		dlpc++;

		tmp = mlib_fabs(dlpc[offset]);
		for (i = offset + 1; i < order + offset; i++)
			tmp =
			    (tmp >
			    mlib_fabs(dlpc[i])) ? tmp : mlib_fabs(dlpc[i]);

		if (tmp != 0.0) {
			iscale = 29 - mlib_ilogb(tmp);
		} else {
			iscale = 0;
		}
		SCALBN(scale, 1 + iscale);

		order1 = order + offset;
		mask = 0xF >> offset;
		dsp = (mlib_d64 *)(lpc - offset);

		dlpc[order1] = 0;
		dlpc[order1 + 1] = 0;
		dlpc[order1 + 2] = 0;
		dlpc[order1 + 3] = 0;

		FLOAT2INT_CLAMP(*((mlib_s32 *)&str3), dlpc[0] * scale + 32768);
		FLOAT2INT_CLAMP(*((mlib_s32 *)&str3 + 1),
		    dlpc[1] * scale + 32768);
		FLOAT2INT_CLAMP(*((mlib_s32 *)&str4), dlpc[2] * scale + 32768);
		FLOAT2INT_CLAMP(*((mlib_s32 *)&str4 + 1),
		    dlpc[3] * scale + 32768);
		dst = vis_fpackfix_pair(str3, str4);
		dlpc += 4;

#pragma pipeloop(0)
		for (i = 0; i <= order1 - 4; i += 4) {
			vis_pst_16(dst, dsp, mask);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str3),
			    dlpc[0] * scale + 32768);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str3 + 1),
			    dlpc[1] * scale + 32768);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str4),
			    dlpc[2] * scale + 32768);
			FLOAT2INT_CLAMP(*((mlib_s32 *)&str4 + 1),
			    dlpc[3] * scale + 32768);
			dst = vis_fpackfix_pair(str3, str4);
			dlpc += 4;
			dsp++;
			mask = 0xF;
		}

		if (i < order1) {
			mask &= (0xF << (4 - order1 + i));
			vis_pst_16(dst, dsp, mask);
		}

		*lscale = iscale;
	}

	if (buf != NULL)
		__mlib_free(buf);
	return (res);
}

/* *********************************************************** */
