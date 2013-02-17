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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_SignalLPC2LSP.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLPC2LSP_S16   - convert linear prediction coefficients
 *                                 to line spectral pair coefficients
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPC2LSP_S16(mlib_s16       *lsp,
 *                                         const mlib_s16 *lpc,
 *                                         mlib_s32       lscale,
 *                                         mlib_s32       order);
 * ARGUMENTS
 *      lsp     the line spectral pair coefficients,
 *              the coefficients are in Q15 format.
 *      lpc     the linear prediction coefficients.
 *      lscale  the scaling factor of the linear prediction coefficients,
 *              where actual_data = input_data * 2^(-scaling_factor).
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
 *      they alternate each other. In the case of even order
 *      P(z) has a root at z = -1 (w = PI) and Q(z) has a root at
 *      z = 1 (w = 0). In the case of odd order Q(z) has both
 *      (z = 1, z = -1) roots.
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
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while lpc[0] element is not initialized and
 *      not used.
 */

#include <stdlib.h>
#include <mlib_signal.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPC2LSP_S16 = __mlib_SignalLPC2LSP_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPC2LSP_S16) mlib_SignalLPC2LSP_S16
    __attribute__((weak, alias("__mlib_SignalLPC2LSP_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                      \
	((X) >= MLIB_S32_MAX ? MLIB_S32_MAX : ((X) <            \
	    MLIB_S32_MIN ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	ORDER	100

/* *********************************************************** */

mlib_status
__mlib_SignalLPC2LSP_S16(
    mlib_s16 *lsp,
    const mlib_s16 *lpc,
    mlib_s32 lscale,
    mlib_s32 order)
{
	mlib_f32 loc_flsp[ORDER + 1];
	mlib_f32 loc_flpc[ORDER + 1];
	mlib_s32 loc_tmp_buf[ORDER + 1];
	mlib_f32 *flsp, *flpc;
	mlib_f32 *buf = NULL, ftmp;
	mlib_s32 *tmp_buf;
	mlib_f32 scale;
	mlib_s32 i;
	mlib_status res;

	if (order < 1)
		return (MLIB_FAILURE);

	if (order > ORDER) {
		buf =
		    (mlib_f32 *)__mlib_malloc(2 * (order +
		    1) * sizeof (mlib_f32) + (order + 1) * sizeof (mlib_s32));

		if (buf == NULL)
			return (MLIB_FAILURE);

		flsp = buf;
		flpc = flsp + (order + 1);
		tmp_buf = (mlib_s32 *)(flpc + (order + 1));
	} else {
		tmp_buf = loc_tmp_buf;
		flsp = loc_flsp;
		flpc = loc_flpc;
	}

	scale = 1;

	for (i = abs(lscale); i > 15; i -= 16)
		scale *= 65536;

	scale *= (1 << i);

	if (lscale > 0)
		scale = 1 / scale;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < order + 1; i++) {
		tmp_buf[i] = lpc[i];
		flpc[i] = tmp_buf[i] * scale;
	}

	res = __mlib_SignalLPC2LSP_F32(flsp, flpc, order);

	if (res == MLIB_SUCCESS) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < order; i++) {
			ftmp = flsp[i] * 2147483648U + 32768;
			lsp[i] = FLOAT2INT_CLAMP(ftmp) >> 16;
		}
	}

	if (buf != NULL)
		__mlib_free(buf);
	return (res);
}

/* *********************************************************** */
