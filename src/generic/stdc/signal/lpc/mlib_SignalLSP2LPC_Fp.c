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

#pragma ident	"@(#)mlib_SignalLSP2LPC_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLSP2LPC_F32   - convert line spectral coefficients
 *                                to linear prediction pair coefficients
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLSP2LPC_F32(mlib_f32       *lpc,
 *                                         const mlib_f32 *lsp,
 *                                         mlib_s32       order);
 * ARGUMENTS
 *      lpc     the linear prediction coefficients.
 *      lsp     the line spectral pair coefficients.
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

#pragma weak mlib_SignalLSP2LPC_F32 = __mlib_SignalLSP2LPC_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLSP2LPC_F32) mlib_SignalLSP2LPC_F32
    __attribute__((weak, alias("__mlib_SignalLSP2LPC_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ONE_HALF	(mlib_d64)0.5
#define	ORDER	100

/* *********************************************************** */

static void mlib_SignalLSP2LPC_F32_COM_even(
    mlib_d64 *f,
    const mlib_f32 *lsp,
    mlib_s32 order);

static void mlib_SignalLSP2LPC_F32_COM_odd(
    mlib_d64 *f,
    const mlib_f32 *lsp,
    mlib_s32 order);

static void mlib_SignalLSP2LPC_double_COM_even(
    mlib_d64 *f,
    const mlib_d64 *lsp,
    mlib_s32 order);

static void mlib_SignalLSP2LPC_double_COM_odd(
    mlib_d64 *f,
    const mlib_d64 *lsp,
    mlib_s32 order);

/* *********************************************************** */

mlib_status
__mlib_SignalLSP2LPC_F32(
    mlib_f32 *lpc,
    const mlib_f32 *lsp,
    mlib_s32 order)
{
	mlib_d64 f1_0, f1_1, f1_2, f1_3, f1_4;
	mlib_d64 f1_5, f1_6;
	mlib_d64 f2_0, f2_1, f2_2, f2_3, f2_4;
	mlib_d64 f2_5, f2_6;
	mlib_d64 a, a1, tmp1, tmp2;
	mlib_d64 tmp_f[ORDER + 4];
	mlib_d64 *buf = NULL, *f;
	mlib_d64 x0, x1, y0, y1, y2;
	mlib_s32 i;

	if (order < 1)
		return (MLIB_FAILURE);

	if (lpc == NULL || lsp == NULL)
		return (MLIB_FAILURE);

	if ((order & 1) == 0) {

	    f1_0 = lsp[0];
	    f2_0 = lsp[1];

	    if (order > 2) {
		a1 = lsp[2];
		a = 2 * a1;
		f1_1 = -a * f1_0 - 1;
		f1_0 = f1_0 + a1;

		a1 = lsp[3];
		a = 2 * a1;
		f2_1 = -a * f2_0 - 1;
		f2_0 = f2_0 + a1;

		if (order > 4) {
		    a1 = lsp[4];
		    a = 2 * a1;
		    f1_2 = 2 * f1_0 - a * f1_1;
		    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
		    f1_0 = f1_0 + a1;

		    a1 = lsp[5];
		    a = 2 * a1;
		    f2_2 = 2 * f2_0 - a * f2_1;
		    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
		    f2_0 = f2_0 + a1;

		    if (order > 6) {
			a1 = lsp[6];
			a = 2 * a1;
			f1_3 = 2 * f1_1 - a * f1_2;
			f1_2 = (f1_2 + f1_0) - a * f1_1;
			f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			f1_0 = f1_0 + a1;

			a1 = lsp[7];
			a = 2 * a1;
			f2_3 = 2 * f2_1 - a * f2_2;
			f2_2 = (f2_2 + f2_0) - a * f2_1;
			f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			f2_0 = f2_0 + a1;

			if (order > 8) {
			    a1 = lsp[8];
			    a = 2 * a1;
			    f1_4 = 2 * f1_2 - a * f1_3;
			    f1_3 = (f1_3 + f1_1) - a * f1_2;
			    f1_2 = (f1_2 + f1_0) - a * f1_1;
			    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			    f1_0 = f1_0 + a1;

			    a1 = lsp[9];
			    a = 2 * a1;
			    f2_4 = 2 * f2_2 - a * f2_3;
			    f2_3 = (f2_3 + f2_1) - a * f2_2;
			    f2_2 = (f2_2 + f2_0) - a * f2_1;
			    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			    f2_0 = f2_0 + a1;

			    if (order > 10) {
				a1 = lsp[10];
				a = 2 * a1;
				f1_5 = 2 * f1_3 - a * f1_4;
				f1_4 = (f1_4 + f1_2) - a * f1_3;
				f1_3 = (f1_3 + f1_1) - a * f1_2;
				f1_2 = (f1_2 + f1_0) - a * f1_1;
				f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				f1_0 = f1_0 + a1;

				a1 = lsp[11];
				a = 2 * a1;
				f2_5 = 2 * f2_3 - a * f2_4;
				f2_4 = (f2_4 + f2_2) - a * f2_3;
				f2_3 = (f2_3 + f2_1) - a * f2_2;
				f2_2 = (f2_2 + f2_0) - a * f2_1;
				f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				f2_0 = f2_0 + a1;

				if (order > 12) {
				    a1 = lsp[12];
				    a = 2 * a1;
				    f1_6 = 2 * f1_4 - a * f1_5;
				    f1_5 = (f1_5 + f1_3) - a * f1_4;
				    f1_4 = (f1_4 + f1_2) - a * f1_3;
				    f1_3 = (f1_3 + f1_1) - a * f1_2;
				    f1_2 = (f1_2 + f1_0) - a * f1_1;
				    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				    f1_0 = f1_0 + a1;

				    a1 = lsp[13];
				    a = 2 * a1;
				    f2_6 = 2 * f2_4 - a * f2_5;
				    f2_5 = (f2_5 + f2_3) - a * f2_4;
				    f2_4 = (f2_4 + f2_2) - a * f2_3;
				    f2_3 = (f2_3 + f2_1) - a * f2_2;
				    f2_2 = (f2_2 + f2_0) - a * f2_1;
				    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				    f2_0 = f2_0 + a1;

				    if (order > 14) {
					f = tmp_f;

					if (order > ORDER) {
					    buf = (mlib_d64 *)__mlib_malloc
						((order + 4) *
						sizeof (mlib_d64));

					    if (buf == NULL)
						return (MLIB_FAILURE);
					    f = buf;
					}

					f[2] = f1_0;
					f[3] = f2_0;
					f[4] = f1_1;
					f[5] = f2_1;
					f[6] = f1_2;
					f[7] = f2_2;
					f[8] = f1_3;
					f[9] = f2_3;
					f[10] = f1_4;
					f[11] = f2_4;
					f[12] = f1_5;
					f[13] = f2_5;
					f[14] = f1_6;
					f[15] = f2_6;

					mlib_SignalLSP2LPC_F32_COM_even
					    (f, lsp, order);

					f1_0 = f[2];
					f2_0 = f[3];
					f1_1 = f[4];
					f2_1 = f[5];
					f1_2 = f[6];
					f2_2 = f[7];
					f1_3 = f[8];
					f2_3 = f[9];
					f1_4 = f[10];
					f2_4 = f[11];
					f1_5 = f[12];
					f2_5 = f[13];
					f1_6 = f[14];
					f2_6 = f[15];

					x0 = f1_6;
					y0 = f2_6;
					x1 = f[16];
					y1 = f[17];
					for (i = 8; i <= (order >> 1); i++) {
					    tmp1 = x1 + x0;
					    tmp2 = y1 - y0;
					    lpc[i] = (tmp1 + tmp2);
					    lpc[order + 1 - i] = (tmp1 - tmp2);
					    x0 = x1;
					    y0 = y1;
					    x1 = f[2 * i + 2];
					    y1 = f[2 * i + 3];
					}
				    }

					tmp1 = f1_6 + f1_5;
					tmp2 = f2_6 - f2_5;
					lpc[7] = (tmp1 + tmp2);
					lpc[order - 6] = (tmp1 - tmp2);

					if (buf != NULL)
						__mlib_free(buf);
				}

				tmp1 = f1_5 + f1_4;
				tmp2 = f2_5 - f2_4;
				lpc[6] = (tmp1 + tmp2);
				lpc[order - 5] = (tmp1 - tmp2);
			    }

			    tmp1 = f1_4 + f1_3;
			    tmp2 = f2_4 - f2_3;
			    lpc[5] = (tmp1 + tmp2);
			    lpc[order - 4] = (tmp1 - tmp2);
			}

			tmp1 = f1_3 + f1_2;
			tmp2 = f2_3 - f2_2;
			lpc[4] = (tmp1 + tmp2);
			lpc[order - 3] = (tmp1 - tmp2);
		    }

		    tmp1 = f1_2 + f1_1;
		    tmp2 = f2_2 - f2_1;
		    lpc[3] = (tmp1 + tmp2);
		    lpc[order - 2] = (tmp1 - tmp2);
		}

		tmp1 = f1_1 + f1_0;
		tmp2 = f2_1 - f2_0;
		lpc[2] = (tmp1 + tmp2);
		lpc[order - 1] = (tmp1 - tmp2);
	    }

	    lpc[1] = (f1_0 + f2_0);
	    lpc[order] = (f1_0 - f2_0 - 1);
	} else {

	    f1_0 = lsp[0];

	    if (order > 1) {
		f2_0 = lsp[1];

		a1 = lsp[2];
		a = 2 * a1;
		f1_1 = -a * f1_0 - 1;
		f1_0 = f1_0 + a1;

		if (order > 3) {
		    a1 = lsp[3];
		    a = 2 * a1;
		    f2_1 = -a * f2_0 - 1;
		    f2_0 = f2_0 + a1;

		    a1 = lsp[4];
		    a = 2 * a1;
		    f1_2 = 2 * f1_0 - a * f1_1;
		    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
		    f1_0 = f1_0 + a1;

		    if (order > 5) {
			a1 = lsp[5];
			a = 2 * a1;
			f2_2 = 2 * f2_0 - a * f2_1;
			f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			f2_0 = f2_0 + a1;

			a1 = lsp[6];
			a = 2 * a1;
			f1_3 = 2 * f1_1 - a * f1_2;
			f1_2 = (f1_2 + f1_0) - a * f1_1;
			f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			f1_0 = f1_0 + a1;

			if (order > 7) {
			    a1 = lsp[7];
			    a = 2 * a1;
			    f2_3 = 2 * f2_1 - a * f2_2;
			    f2_2 = (f2_2 + f2_0) - a * f2_1;
			    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			    f2_0 = f2_0 + a1;

			    a1 = lsp[8];
			    a = 2 * a1;
			    f1_4 = 2 * f1_2 - a * f1_3;
			    f1_3 = (f1_3 + f1_1) - a * f1_2;
			    f1_2 = (f1_2 + f1_0) - a * f1_1;
			    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			    f1_0 = f1_0 + a1;

			    if (order > 9) {

				a1 = lsp[9];
				a = 2 * a1;
				f2_4 = 2 * f2_2 - a * f2_3;
				f2_3 = (f2_3 + f2_1) - a * f2_2;
				f2_2 = (f2_2 + f2_0) - a * f2_1;
				f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				f2_0 = f2_0 + a1;

				a1 = lsp[10];
				a = 2 * a1;
				f1_5 = 2 * f1_3 - a * f1_4;
				f1_4 = (f1_4 + f1_2) - a * f1_3;
				f1_3 = (f1_3 + f1_1) - a * f1_2;
				f1_2 = (f1_2 + f1_0) - a * f1_1;
				f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				f1_0 = f1_0 + a1;

				if (order > 11) {

				    a1 = lsp[11];
				    a = 2 * a1;
				    f2_5 = 2 * f2_3 - a * f2_4;
				    f2_4 = (f2_4 + f2_2) - a * f2_3;
				    f2_3 = (f2_3 + f2_1) - a * f2_2;
				    f2_2 = (f2_2 + f2_0) - a * f2_1;
				    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				    f2_0 = f2_0 + a1;

				    a1 = lsp[12];
				    a = 2 * a1;
				    f1_6 = 2 * f1_4 - a * f1_5;
				    f1_5 = (f1_5 + f1_3) - a * f1_4;
				    f1_4 = (f1_4 + f1_2) - a * f1_3;
				    f1_3 = (f1_3 + f1_1) - a * f1_2;
				    f1_2 = (f1_2 + f1_0) - a * f1_1;
				    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				    f1_0 = f1_0 + a1;

				    if (order > 13) {
					f = tmp_f;

					if (order > ORDER) {
					    buf = (mlib_d64 *)__mlib_malloc
						((order + 4) *
						sizeof (mlib_d64));

					    if (buf == NULL)
						return (MLIB_FAILURE);
					    f = buf;
					}

					f[2] = f1_0;
					f[3] = f2_0;
					f[4] = f1_1;
					f[5] = f2_1;
					f[6] = f1_2;
					f[7] = f2_2;
					f[8] = f1_3;
					f[9] = f2_3;
					f[10] = f1_4;
					f[11] = f2_4;
					f[12] = f1_5;
					f[13] = f2_5;
					f[14] = f1_6;

					mlib_SignalLSP2LPC_F32_COM_odd
						(f, lsp, order);

					f1_0 = f[2];
					f2_0 = f[3];
					f1_1 = f[4];
					f2_1 = f[5];
					f1_2 = f[6];
					f2_2 = f[7];
					f1_3 = f[8];
					f2_3 = f[9];
					f1_4 = f[10];
					f2_4 = f[11];
					f1_5 = f[12];
					f2_5 = f[13];

					x1 = f[14];
					y0 = f2_4;
					y1 = f2_5;
					y2 = f[15];
					for (i = 7; i <= (order >> 1); i++) {
					    tmp2 = y2 - y0;
					    lpc[i] = (x1 + tmp2);
					    lpc[order + 1 - i] = (x1 - tmp2);
					    y0 = y1;
					    y1 = y2;
					    x1 = f[2 * i + 2];
					    y2 = f[2 * i + 3];
					}

					lpc[i] = x1;

					if (buf != NULL)
						__mlib_free(buf);
				    } else
					lpc[7] = f1_6;
				    tmp2 = f2_5 - f2_3;
				    lpc[6] = (f1_5 + tmp2);
				    lpc[order - 5] = (f1_5 - tmp2);
				} else
					lpc[6] = f1_5;
				    tmp2 = f2_4 - f2_2;
				    lpc[5] = (f1_4 + tmp2);
				    lpc[order - 4] = (f1_4 - tmp2);
			    } else
				lpc[5] = f1_4;
			    tmp2 = f2_3 - f2_1;
			    lpc[4] = (f1_3 + tmp2);
			    lpc[order - 3] = (f1_3 - tmp2);
			} else
				lpc[4] = f1_3;
			    tmp2 = f2_2 - f2_0;
			    lpc[3] = (f1_2 + tmp2);
			    lpc[order - 2] = (f1_2 - tmp2);
		    } else
			lpc[3] = f1_2;
		    tmp2 = f2_1 + ONE_HALF;
		    lpc[2] = (f1_1 + tmp2);
		    lpc[order - 1] = (f1_1 - tmp2);
		} else
			lpc[2] = f1_1;
		lpc[1] = (f1_0 + f2_0);
		lpc[order] = (f1_0 - f2_0);
	    } else
		lpc[1] = f1_0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_SignalLSP2LPC_F32_COM_even(
    mlib_d64 *f,
    const mlib_f32 *lsp,
    mlib_s32 order)
{
	mlib_d64 x0, x1, x2, y0, y1, y2, b, b1, a, a1;
	mlib_s32 i, j;

	for (i = 14; i < order; i += 2) {
		a1 = lsp[i];
		a = 2 * a1;
		b1 = lsp[i + 1];
		b = 2 * b1;
		x0 = f[i];
		y0 = f[i + 1];
		x1 = f[i - 2];
		y1 = f[i - 1];

		f[i + 2] = 2 * x1 - a * x0;
		f[i + 3] = 2 * y1 - b * y0;

		x2 = f[i - 4];
		y2 = f[i - 3];

		for (j = i - 2; j > 2; j -= 2) {
			f[j + 2] = (x0 + x2) - a * x1;
			f[j + 3] = (y0 + y2) - b * y1;
			x0 = x1;
			x1 = x2;
			y0 = y1;
			y1 = y2;
			x2 = f[j - 4];
			y2 = f[j - 3];
		}

		f[2] = x1 + a1;
		f[3] = y1 + b1;
		f[4] = (x0 - ONE_HALF) - a * x1;
		f[5] = (y0 - ONE_HALF) - b * y1;
	}
}

/* *********************************************************** */

void
mlib_SignalLSP2LPC_F32_COM_odd(
    mlib_d64 *f,
    const mlib_f32 *lsp,
    mlib_s32 order)
{
	mlib_d64 x0, x1, x2, y0, y1, y2, b, b1, a, a1;
	mlib_s32 i, j;

	for (i = 13; i < order; i += 2) {
		a1 = lsp[i];
		a = 2 * a1;
		b1 = lsp[i + 1];
		b = 2 * b1;
		x0 = f[i];
		y0 = f[i + 1];
		x1 = f[i - 2];
		y1 = f[i - 1];

		f[i + 2] = 2 * x1 - a * x0;
		f[i + 3] = 2 * y1 - b * y0;

		x2 = f[i - 4];
		y2 = f[i - 3];

		for (j = i - 2; j > 3; j -= 2) {
			f[j + 2] = (x0 + x2) - a * x1;
			f[j + 3] = (y0 + y2) - b * y1;
			x0 = x1;
			x1 = x2;
			y0 = y1;
			y1 = y2;
			x2 = f[j - 4];
			y2 = f[j - 3];
		}

		f[2] = y2 + b1;
		f[3] = x1 + a1;
		f[4] = (y1 - ONE_HALF) - b * y2;
		f[5] = (x0 - ONE_HALF) - a * x1;
		f[6] = (y0 + y2) - b * y1;
	}
}

/* *********************************************************** */

#undef ONE_HALF
#define	ONE_HALF	(mlib_d64)16384
#define	ONE	(mlib_d64)32768
#define	TWO	(mlib_d64)0.00006103515625

/* *********************************************************** */

/*
 * FUNCTIONS
 *     mlib_SignalLSP2LPC_double   - convert line spectral coefficients
 *                                   to linear prediction pair coefficients
 *
 * SYNOPSIS
 *     mlib_status mlib_SignalLSP2LPC_double(mlib_d64       *lpc,
 *                                           const mlib_d64 *lsp,
 *                                           mlib_s32       order);
 *
 * ARGUMENTS
 *      lpc     the linear prediction coefficients (*32768).
 *      lsp     the line spectral pair coefficients (*32768).
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
 */

/* *********************************************************** */

mlib_status
mlib_SignalLSP2LPC_double(
    mlib_d64 *lpc,
    const mlib_d64 *lsp,
    mlib_s32 order)
{
	mlib_d64 f1_0, f1_1, f1_2, f1_3, f1_4;
	mlib_d64 f1_5, f1_6;
	mlib_d64 f2_0, f2_1, f2_2, f2_3, f2_4;
	mlib_d64 f2_5, f2_6;
	mlib_d64 a, a1, tmp1, tmp2;
	mlib_d64 tmp_f[ORDER + 4];
	mlib_d64 *buf = NULL, *f;
	mlib_d64 x0, x1, y0, y1, y2;
	mlib_s32 i;

	if ((order & 1) == 0) {

	    f1_0 = lsp[0];
	    f2_0 = lsp[1];

	    if (order > 2) {
		a1 = lsp[2];
		a = TWO * a1;
		f1_1 = -a * f1_0 - ONE;
		f1_0 = f1_0 + a1;

		a1 = lsp[3];
		a = TWO * a1;
		f2_1 = -a * f2_0 - ONE;
		f2_0 = f2_0 + a1;

		if (order > 4) {
		    a1 = lsp[4];
		    a = TWO * a1;
		    f1_2 = 2 * f1_0 - a * f1_1;
		    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
		    f1_0 = f1_0 + a1;

		    a1 = lsp[5];
		    a = TWO * a1;
		    f2_2 = 2 * f2_0 - a * f2_1;
		    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
		    f2_0 = f2_0 + a1;

		    if (order > 6) {
			a1 = lsp[6];
			a = TWO * a1;
			f1_3 = 2 * f1_1 - a * f1_2;
			f1_2 = (f1_2 + f1_0) - a * f1_1;
			f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			f1_0 = f1_0 + a1;

			a1 = lsp[7];
			a = TWO * a1;
			f2_3 = 2 * f2_1 - a * f2_2;
			f2_2 = (f2_2 + f2_0) - a * f2_1;
			f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			f2_0 = f2_0 + a1;

			if (order > 8) {
			    a1 = lsp[8];
			    a = TWO * a1;
			    f1_4 = 2 * f1_2 - a * f1_3;
			    f1_3 = (f1_3 + f1_1) - a * f1_2;
			    f1_2 = (f1_2 + f1_0) - a * f1_1;
			    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			    f1_0 = f1_0 + a1;

			    a1 = lsp[9];
			    a = TWO * a1;
			    f2_4 = 2 * f2_2 - a * f2_3;
			    f2_3 = (f2_3 + f2_1) - a * f2_2;
			    f2_2 = (f2_2 + f2_0) - a * f2_1;
			    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			    f2_0 = f2_0 + a1;

			    if (order > 10) {
				a1 = lsp[10];
				a = TWO * a1;
				f1_5 = 2 * f1_3 - a * f1_4;
				f1_4 = (f1_4 + f1_2) - a * f1_3;
				f1_3 = (f1_3 + f1_1) - a * f1_2;
				f1_2 = (f1_2 + f1_0) - a * f1_1;
				f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				f1_0 = f1_0 + a1;

				a1 = lsp[11];
				a = TWO * a1;
				f2_5 = 2 * f2_3 - a * f2_4;
				f2_4 = (f2_4 + f2_2) - a * f2_3;
				f2_3 = (f2_3 + f2_1) - a * f2_2;
				f2_2 = (f2_2 + f2_0) - a * f2_1;
				f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				f2_0 = f2_0 + a1;

				if (order > 12) {
				    a1 = lsp[12];
				    a = TWO * a1;
				    f1_6 = 2 * f1_4 - a * f1_5;
				    f1_5 = (f1_5 + f1_3) - a * f1_4;
				    f1_4 = (f1_4 + f1_2) - a * f1_3;
				    f1_3 = (f1_3 + f1_1) - a * f1_2;
				    f1_2 = (f1_2 + f1_0) - a * f1_1;
				    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				    f1_0 = f1_0 + a1;

				    a1 = lsp[13];
				    a = TWO * a1;
				    f2_6 = 2 * f2_4 - a * f2_5;
				    f2_5 = (f2_5 + f2_3) - a * f2_4;
				    f2_4 = (f2_4 + f2_2) - a * f2_3;
				    f2_3 = (f2_3 + f2_1) - a * f2_2;
				    f2_2 = (f2_2 + f2_0) - a * f2_1;
				    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				    f2_0 = f2_0 + a1;

				    if (order > 14) {
					f = tmp_f;

					if (order > ORDER) {
					    buf = (mlib_d64 *)__mlib_malloc
						((order + 4) *
						sizeof (mlib_d64));

					    if (buf == NULL)
						return (MLIB_FAILURE);
					    f = buf;
					}

					f[2] = f1_0;
					f[3] = f2_0;
					f[4] = f1_1;
					f[5] = f2_1;
					f[6] = f1_2;
					f[7] = f2_2;
					f[8] = f1_3;
					f[9] = f2_3;
					f[10] = f1_4;
					f[11] = f2_4;
					f[12] = f1_5;
					f[13] = f2_5;
					f[14] = f1_6;
					f[15] = f2_6;

					mlib_SignalLSP2LPC_double_COM_even
						(f, lsp, order);

					f1_0 = f[2];
					f2_0 = f[3];
					f1_1 = f[4];
					f2_1 = f[5];
					f1_2 = f[6];
					f2_2 = f[7];
					f1_3 = f[8];
					f2_3 = f[9];
					f1_4 = f[10];
					f2_4 = f[11];
					f1_5 = f[12];
					f2_5 = f[13];
					f1_6 = f[14];
					f2_6 = f[15];

					x0 = f1_6;
					y0 = f2_6;
					x1 = f[16];
					y1 = f[17];
					for (i = 8; i <= (order >> 1); i++) {
					    tmp1 = x1 + x0;
					    tmp2 = y1 - y0;
					    lpc[i] = (tmp1 + tmp2);
					    lpc[order + 1 - i] = (tmp1 - tmp2);
					    x0 = x1;
					    y0 = y1;
					    x1 = f[2 * i + 2];
					    y1 = f[2 * i + 3];
					}
				    }

				    tmp1 = f1_6 + f1_5;
				    tmp2 = f2_6 - f2_5;
				    lpc[7] = (tmp1 + tmp2);
				    lpc[order - 6] = (tmp1 - tmp2);

				    if (buf != NULL)
					__mlib_free(buf);
				}

						tmp1 = f1_5 + f1_4;
						tmp2 = f2_5 - f2_4;
						lpc[6] = (tmp1 + tmp2);
						lpc[order - 5] =
						    (tmp1 - tmp2);
					}

					tmp1 = f1_4 + f1_3;
					tmp2 = f2_4 - f2_3;
					lpc[5] = (tmp1 + tmp2);
					lpc[order - 4] = (tmp1 - tmp2);
				}

				tmp1 = f1_3 + f1_2;
				tmp2 = f2_3 - f2_2;
				lpc[4] = (tmp1 + tmp2);
				lpc[order - 3] = (tmp1 - tmp2);
			}

			tmp1 = f1_2 + f1_1;
			tmp2 = f2_2 - f2_1;
			lpc[3] = (tmp1 + tmp2);
			lpc[order - 2] = (tmp1 - tmp2);
		}

		tmp1 = f1_1 + f1_0;
		tmp2 = f2_1 - f2_0;
		lpc[2] = (tmp1 + tmp2);
		lpc[order - 1] = (tmp1 - tmp2);
	    }

	    lpc[1] = (f1_0 + f2_0);
	    lpc[order] = (f1_0 - f2_0 - ONE);
	} else {

	    f1_0 = lsp[0];

	    if (order > 1) {
		f2_0 = lsp[1];

		a1 = lsp[2];
		a = TWO * a1;
		f1_1 = -a * f1_0 - ONE;
		f1_0 = f1_0 + a1;

		if (order > 3) {
		    a1 = lsp[3];
		    a = TWO * a1;
		    f2_1 = -a * f2_0 - ONE;
		    f2_0 = f2_0 + a1;

		    a1 = lsp[4];
		    a = TWO * a1;
		    f1_2 = 2 * f1_0 - a * f1_1;
		    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
		    f1_0 = f1_0 + a1;

		    if (order > 5) {
			a1 = lsp[5];
			a = TWO * a1;
			f2_2 = 2 * f2_0 - a * f2_1;
			f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			f2_0 = f2_0 + a1;

			a1 = lsp[6];
			a = TWO * a1;
			f1_3 = 2 * f1_1 - a * f1_2;
			f1_2 = (f1_2 + f1_0) - a * f1_1;
			f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			f1_0 = f1_0 + a1;

			if (order > 7) {
			    a1 = lsp[7];
			    a = TWO * a1;
			    f2_3 = 2 * f2_1 - a * f2_2;
			    f2_2 = (f2_2 + f2_0) - a * f2_1;
			    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
			    f2_0 = f2_0 + a1;

			    a1 = lsp[8];
			    a = TWO * a1;
			    f1_4 = 2 * f1_2 - a * f1_3;
			    f1_3 = (f1_3 + f1_1) - a * f1_2;
			    f1_2 = (f1_2 + f1_0) - a * f1_1;
			    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
			    f1_0 = f1_0 + a1;

			    if (order > 9) {

				a1 = lsp[9];
				a = TWO * a1;
				f2_4 = 2 * f2_2 - a * f2_3;
				f2_3 = (f2_3 + f2_1) - a * f2_2;
				f2_2 = (f2_2 + f2_0) - a * f2_1;
				f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				f2_0 = f2_0 + a1;

				a1 = lsp[10];
				a = TWO * a1;
				f1_5 = 2 * f1_3 - a * f1_4;
				f1_4 = (f1_4 + f1_2) - a * f1_3;
				f1_3 = (f1_3 + f1_1) - a * f1_2;
				f1_2 = (f1_2 + f1_0) - a * f1_1;
				f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				f1_0 = f1_0 + a1;

				if (order > 11) {

				    a1 = lsp[11];
				    a = TWO * a1;
				    f2_5 = 2 * f2_3 - a * f2_4;
				    f2_4 = (f2_4 + f2_2) - a * f2_3;
				    f2_3 = (f2_3 + f2_1) - a * f2_2;
				    f2_2 = (f2_2 + f2_0) - a * f2_1;
				    f2_1 = (f2_1 - ONE_HALF) - a * f2_0;
				    f2_0 = f2_0 + a1;

				    a1 = lsp[12];
				    a = TWO * a1;
				    f1_6 = 2 * f1_4 - a * f1_5;
				    f1_5 = (f1_5 + f1_3) - a * f1_4;
				    f1_4 = (f1_4 + f1_2) - a * f1_3;
				    f1_3 = (f1_3 + f1_1) - a * f1_2;
				    f1_2 = (f1_2 + f1_0) - a * f1_1;
				    f1_1 = (f1_1 - ONE_HALF) - a * f1_0;
				    f1_0 = f1_0 + a1;

				    if (order > 13) {
					f = tmp_f;

					if (order > ORDER) {
					    buf = (mlib_d64 *)__mlib_malloc
						((order + 4) *
						sizeof (mlib_d64));

					    if (buf == NULL)
						return (MLIB_FAILURE);
					    f = buf;
					}

					f[2] = f1_0;
					f[3] = f2_0;
					f[4] = f1_1;
					f[5] = f2_1;
					f[6] = f1_2;
					f[7] = f2_2;
					f[8] = f1_3;
					f[9] = f2_3;
					f[10] = f1_4;
					f[11] = f2_4;
					f[12] = f1_5;
					f[13] = f2_5;
					f[14] = f1_6;

					mlib_SignalLSP2LPC_double_COM_odd
						(f, lsp, order);

					f1_0 = f[2];
					f2_0 = f[3];
					f1_1 = f[4];
					f2_1 = f[5];
					f1_2 = f[6];
					f2_2 = f[7];
					f1_3 = f[8];
					f2_3 = f[9];
					f1_4 = f[10];
					f2_4 = f[11];
					f1_5 = f[12];
					f2_5 = f[13];

					x1 = f[14];
					y0 = f2_4;
					y1 = f2_5;
					y2 = f[15];
					for (i = 7; i <= (order >> 1); i++) {
					    tmp2 = y2 - y0;
					    lpc[i] = (x1 + tmp2);
					    lpc[order + 1 - i] = (x1 - tmp2);
					    y0 = y1;
					    y1 = y2;
					    x1 = f[2 * i + 2];
					    y2 = f[2 * i + 3];
					}

					lpc[i] = x1;

					if (buf != NULL)
						__mlib_free(buf);
				    } else
					lpc[7] = f1_6;
				    tmp2 = f2_5 - f2_3;
				    lpc[6] = (f1_5 + tmp2);
				    lpc[order - 5] = (f1_5 - tmp2);
				} else
					lpc[6] = f1_5;
				tmp2 = f2_4 - f2_2;
				lpc[5] = (f1_4 + tmp2);
				lpc[order - 4] = (f1_4 - tmp2);
			    } else
				lpc[5] = f1_4;
			    tmp2 = f2_3 - f2_1;
			    lpc[4] = (f1_3 + tmp2);
			    lpc[order - 3] = (f1_3 - tmp2);
			} else
			    lpc[4] = f1_3;
			tmp2 = f2_2 - f2_0;
			lpc[3] = (f1_2 + tmp2);
			lpc[order - 2] = (f1_2 - tmp2);
		    } else
			lpc[3] = f1_2;
		    tmp2 = f2_1 + ONE_HALF;
		    lpc[2] = (f1_1 + tmp2);
		    lpc[order - 1] = (f1_1 - tmp2);
		} else
		    lpc[2] = f1_1;
		lpc[1] = (f1_0 + f2_0);
		lpc[order] = (f1_0 - f2_0);
	    } else
		lpc[1] = f1_0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_SignalLSP2LPC_double_COM_even(
    mlib_d64 *f,
    const mlib_d64 *lsp,
    mlib_s32 order)
{
	mlib_d64 x0, x1, x2, y0, y1, y2, b, b1, a, a1;
	mlib_s32 i, j;

	for (i = 14; i < order; i += 2) {
		a1 = lsp[i];
		a = TWO * a1;
		b1 = lsp[i + 1];
		b = TWO * b1;
		x0 = f[i];
		y0 = f[i + 1];
		x1 = f[i - 2];
		y1 = f[i - 1];

		f[i + 2] = 2 * x1 - a * x0;
		f[i + 3] = 2 * y1 - b * y0;

		x2 = f[i - 4];
		y2 = f[i - 3];

		for (j = i - 2; j > 2; j -= 2) {
			f[j + 2] = (x0 + x2) - a * x1;
			f[j + 3] = (y0 + y2) - b * y1;
			x0 = x1;
			x1 = x2;
			y0 = y1;
			y1 = y2;
			x2 = f[j - 4];
			y2 = f[j - 3];
		}

		f[2] = x1 + a1;
		f[3] = y1 + b1;
		f[4] = (x0 - ONE_HALF) - a * x1;
		f[5] = (y0 - ONE_HALF) - b * y1;
	}
}

/* *********************************************************** */

void
mlib_SignalLSP2LPC_double_COM_odd(
    mlib_d64 *f,
    const mlib_d64 *lsp,
    mlib_s32 order)
{
	mlib_d64 x0, x1, x2, y0, y1, y2, b, b1, a, a1;
	mlib_s32 i, j;

	for (i = 13; i < order; i += 2) {
		a1 = lsp[i];
		a = TWO * a1;
		b1 = lsp[i + 1];
		b = TWO * b1;
		x0 = f[i];
		y0 = f[i + 1];
		x1 = f[i - 2];
		y1 = f[i - 1];

		f[i + 2] = 2 * x1 - a * x0;
		f[i + 3] = 2 * y1 - b * y0;

		x2 = f[i - 4];
		y2 = f[i - 3];

		for (j = i - 2; j > 3; j -= 2) {
			f[j + 2] = (x0 + x2) - a * x1;
			f[j + 3] = (y0 + y2) - b * y1;
			x0 = x1;
			x1 = x2;
			y0 = y1;
			y1 = y2;
			x2 = f[j - 4];
			y2 = f[j - 3];
		}

		f[2] = y2 + b1;
		f[3] = x1 + a1;
		f[4] = (y1 - ONE_HALF) - b * y2;
		f[5] = (x0 - ONE_HALF) - a * x1;
		f[6] = (y0 + y2) - b * y1;
	}
}

/* *********************************************************** */
