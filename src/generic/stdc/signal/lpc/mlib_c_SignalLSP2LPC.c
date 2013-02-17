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

#pragma ident	"@(#)mlib_c_SignalLSP2LPC.c	9.3	07/10/09 SMI"

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

#define	D64_TO_S16(x)	(((mlib_s32)(x*scale+32768)) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	D64_TO_S16(x)                                                      \
	(((mlib_s32)(((x * scale + 32768) >=                               \
	    MLIB_S32_MAX) ? MLIB_S32_MAX : (((x * scale + 32768) <=        \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (x * scale + 32768)))) >> 16)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + (shift)) << 20

#else /* _LITTLE_ENDIAN */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + (shift)) << 20

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_SignalLSP2LPC_S16(
    mlib_s16 *lpc,
    mlib_s32 lscale,
    const mlib_s16 *lsp,
    mlib_s32 order)
{
	mlib_d64 tmp_dlpc[ORDER + 1], tmp_dlsp[ORDER + 1], scale;
	mlib_d64 *dlpc, *dlsp, *buf = NULL;
	mlib_s32 tmp_slsp[ORDER + 1];
	mlib_s32 *slsp;
	mlib_s32 i;
	mlib_status res;

	if (order < 1)
		return (MLIB_FAILURE);

	if (lpc == NULL || lsp == NULL)
		return (MLIB_FAILURE);

	if (order > ORDER) {
		buf =
		    (mlib_d64 *)__mlib_malloc(2 * (order +
		    1) * sizeof (mlib_d64) + (order + 1) * sizeof (mlib_s32));

		if (buf == NULL)
			return (MLIB_FAILURE);
		dlpc = buf;
		dlsp = dlpc + (order + 1);
		slsp = (mlib_s32 *)(dlsp + (order + 1));
	} else {
		dlpc = tmp_dlpc;
		dlsp = tmp_dlsp;
		slsp = tmp_slsp;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < order; i++) {
		slsp[i] = lsp[i];
		dlsp[i] = slsp[i];
	}

	res = mlib_SignalLSP2LPC_double(dlpc, dlsp, order);

	if (res == MLIB_SUCCESS) {
		SCALBN(scale, 1 + lscale);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 1; i <= order; i++) {
			lpc[i] = D64_TO_S16(dlpc[i]);
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
	mlib_d64 tmp_dlpc[ORDER + 1], tmp_dlsp[ORDER + 1], scale, tmp;
	mlib_d64 *dlpc, *dlsp, *buf = NULL;
	mlib_s32 tmp_slsp[ORDER + 1], iscale;
	mlib_s32 *slsp;
	mlib_s32 i;
	mlib_status res;

	if (order < 1)
		return (MLIB_FAILURE);

	if (lpc == NULL || lsp == NULL)
		return (MLIB_FAILURE);

	if (order > ORDER) {
		buf =
		    (mlib_d64 *)__mlib_malloc(2 * (order +
		    1) * sizeof (mlib_d64) + (order + 1) * sizeof (mlib_s32));

		if (buf == NULL)
			return (MLIB_FAILURE);
		dlpc = buf;
		dlsp = dlpc + (order + 1);
		slsp = (mlib_s32 *)(dlsp + (order + 1));
	} else {
		dlpc = tmp_dlpc;
		dlsp = tmp_dlsp;
		slsp = tmp_slsp;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < order; i++) {
		slsp[i] = lsp[i];
		dlsp[i] = slsp[i];
	}

	res = mlib_SignalLSP2LPC_double(dlpc, dlsp, order);

	if (res == MLIB_SUCCESS) {
		tmp = mlib_fabs(dlpc[1]);
		for (i = 2; i <= order; i++)
			tmp =
			    (tmp >
			    mlib_fabs(dlpc[i])) ? tmp : mlib_fabs(dlpc[i]);

		if (tmp != 0.0) {
			iscale = 29 - mlib_ilogb(tmp);
		} else {
			iscale = 0;
		}
		SCALBN(scale, 1 + iscale);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 1; i <= order; i++) {
			lpc[i] = D64_TO_S16(dlpc[i]);
		}

		*lscale = iscale;
	}

	if (buf != NULL)
		__mlib_free(buf);
	return (res);
}

/* *********************************************************** */
