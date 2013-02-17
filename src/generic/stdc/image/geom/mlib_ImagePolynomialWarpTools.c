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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_ImagePolynomialWarpTools.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	MAX_POWS	128

/* *********************************************************** */

static void *
mlib_div_kernel(
    mlib_s32 n,
    mlib_s32 *rr)
{
	mlib_s32 i;
	mlib_s32 n4, n3, n2, n1;
	mlib_s32 *p, *pp;

	pp = p = __mlib_malloc((n + 1) * sizeof (mlib_s32));

	if (!pp)
		return (pp);

	n4 = n / 4;
	n -= n4 * 4;

	if (n == 2 || n == 1) {
		n4--;
		n += 4 & ~(n4 >> 31);
	}
	n3 = n / 3;
	n -= n3 * 3;
	n2 = n / 2;
	n -= n2 * 2;
	n1 = n;

	for (i = 0; i < n4; i++) {
		(*pp++) = 4;
	}
	for (i = 0; i < n3; i++) {
		(*pp++) = 3;
	}
	for (i = 0; i < n2; i++) {
		(*pp++) = 2;
	}
	for (i = 0; i < n1; i++) {
		(*pp++) = 1;
	}

	*pp = 0;
	*rr = pp - p;
	return (p);
}

/* *********************************************************** */

void *
mlib_IPWT_get_call_func(
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 type,
    mlib_s32 *count)
{
	mlib_s32 offH = 0, offV = 0;
	mlib_s32 ch, cw, itype = type * 3;
	mlib_s32 *ph, *pw;
	mlib_s32 i, j, n;
	mlib_IPWFSat array_sat[] = {
		mlib_IPWT_U8_Sat,
		mlib_IPWT_S16_Sat,
		mlib_IPWT_U16_Sat,
		mlib_IPWT_S32_Sat,
		mlib_IPWT_F32_Sat,
		mlib_IPWT_D64_Sat
	};
	mlib_IPWT_CF *cf, *cfp;
	mlib_IPWFTCall array_kernel_func[][4] = {
		{mlib_IPWT_U8_2x1, mlib_IPWT_U8_2x2, mlib_IPWT_U8_2x3,
		    mlib_IPWT_U8_2x4},
		{mlib_IPWT_U8_3x1, mlib_IPWT_U8_3x2, mlib_IPWT_U8_3x3,
		    mlib_IPWT_U8_3x4},
		{mlib_IPWT_U8_4x1, mlib_IPWT_U8_4x2, mlib_IPWT_U8_4x3,
		    mlib_IPWT_U8_4x4},

		{mlib_IPWT_S16_2x1, mlib_IPWT_S16_2x2, mlib_IPWT_S16_2x3,
		    mlib_IPWT_S16_2x4},
		{mlib_IPWT_S16_3x1, mlib_IPWT_S16_3x2, mlib_IPWT_S16_3x3,
		    mlib_IPWT_S16_3x4},
		{mlib_IPWT_S16_4x1, mlib_IPWT_S16_4x2, mlib_IPWT_S16_4x3,
		    mlib_IPWT_S16_4x4},

		{mlib_IPWT_U16_2x1, mlib_IPWT_U16_2x2, mlib_IPWT_U16_2x3,
		    mlib_IPWT_U16_2x4},
		{mlib_IPWT_U16_3x1, mlib_IPWT_U16_3x2, mlib_IPWT_U16_3x3,
		    mlib_IPWT_U16_3x4},
		{mlib_IPWT_U16_4x1, mlib_IPWT_U16_4x2, mlib_IPWT_U16_4x3,
		    mlib_IPWT_U16_4x4},

		{mlib_IPWT_S32_2x1, mlib_IPWT_S32_2x2, mlib_IPWT_S32_2x3,
		    mlib_IPWT_S32_2x4},
		{mlib_IPWT_S32_3x1, mlib_IPWT_S32_3x2, mlib_IPWT_S32_3x3,
		    mlib_IPWT_S32_3x4},
		{mlib_IPWT_S32_4x1, mlib_IPWT_S32_4x2, mlib_IPWT_S32_4x3,
		    mlib_IPWT_S32_4x4},

		{mlib_IPWT_F32_2x1, mlib_IPWT_F32_2x2, mlib_IPWT_F32_2x3,
		    mlib_IPWT_F32_2x4},
		{mlib_IPWT_F32_3x1, mlib_IPWT_F32_3x2, mlib_IPWT_F32_3x3,
		    mlib_IPWT_F32_3x4},
		{mlib_IPWT_F32_4x1, mlib_IPWT_F32_4x2, mlib_IPWT_F32_4x3,
		    mlib_IPWT_F32_4x4},

		{mlib_IPWT_D64_2x1, mlib_IPWT_D64_2x2, mlib_IPWT_D64_2x3,
		    mlib_IPWT_D64_2x4},
		{mlib_IPWT_D64_3x1, mlib_IPWT_D64_3x2, mlib_IPWT_D64_3x3,
		    mlib_IPWT_D64_3x4},
		{mlib_IPWT_D64_4x1, mlib_IPWT_D64_4x2, mlib_IPWT_D64_4x3,
		    mlib_IPWT_D64_4x4},
	};

	ph = mlib_div_kernel(h, &ch);

	if (!ph)
		return (ph);
	pw = mlib_div_kernel(w, &cw);

	if (!pw) {
		__mlib_free(ph);
		return (pw);
	}

	n = cw * ch;

	cf = cfp = __mlib_malloc(sizeof (mlib_IPWT_CF) * (n + 1));

	if (!cf) {
		__mlib_free(ph);
		__mlib_free(pw);
		return (cf);
	}

	for (i = 0; ph[i] != 0; i++) {
		offH = 0;
		for (j = 0; pw[j] != 0; j++) {

			cf->func =
			    array_kernel_func[pw[j] - 2 + itype][ph[i] - 1];
			cf->offH = offH;
			cf->offV = offV;

			offH += pw[j];
			cf++;
		}

		offV += ph[i];
	}

	cf->func = (mlib_IPWFTCall) array_sat[type];

	__mlib_free(ph);
	__mlib_free(pw);

	*count = n;
	return (cfp);
}

/* *********************************************************** */

void *
mlib_ImagePolynimialWarpCoeffsPrepare(
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_d64 preScaleX,
    mlib_d64 preScaleY,
    mlib_d64 postScaleX,
    mlib_d64 postScaleY,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_s32 degree)
{
	mlib_s32 i, j, k;
	mlib_d64 *mem, *x_new, *y_new;
	mlib_d64 psX, psY, pssX, pssY;

	k = (degree + 1) * (degree + 2) / 2;
	mem = __mlib_malloc(sizeof (mlib_d64) * k * 2);

	if (!mem)
		return (mem);

	x_new = mem;
	y_new = x_new + k;

	psX = postScaleX;
	psY = postScaleY;

	for (i = 0; i <= degree; i++) {
		k = (i + 1) * (i + 2) / 2 - 1;

		pssX = psX;
		pssY = psY;

		for (j = 0; j <= degree - i; j++) {
			((*x_new++)) = xCoeffs[k] * pssX;
			((*y_new++)) = yCoeffs[k] * pssY;

			k += i + 1 + j;
			pssX *= preScaleX;
			pssY *= preScaleX;
		}

		psX *= preScaleY;
		psY *= preScaleY;
	}

	k = (degree + 1) * (degree + 2) / 2;
	x_new = mem;
	y_new = x_new + k;

	x_new[0] -= postShiftX;
	y_new[0] -= postShiftY;

	return (mem);
}

/* *********************************************************** */

/*			Clipping				*/

/* *********************************************************** */

/*			degree == 1				*/

/* *********************************************************** */

#define	INIT_COEFF_1                                            \
	a0 = xCoeffs[0] + xCoeffs[2] * y;                       \
	a1 = xCoeffs[1];                                        \
	wx = a0 + a1 * x;                                       \
	dx = a1;                                                \
	b0 = yCoeffs[0] + yCoeffs[2] * y;                       \
	b1 = yCoeffs[1];                                        \
	wy = b0 + b1 * x;                                       \
	dy = b1

/* *********************************************************** */

#define	DELTA_1	wx += dx; wy += dy

/* *********************************************************** */

#define	DEGREE	1

/* *********************************************************** */

#define	MODE	10
#include <mlib_ImagePolynomialWarpTools.h>

#undef DEGREE

/* *********************************************************** */

/*			degree == 2				*/

/* *********************************************************** */

#define	INIT_COEFF_2                                            \
	a0 = xCoeffs[0] + (xCoeffs[3] + xCoeffs[5] * y) * y;    \
	a1 = xCoeffs[1] + xCoeffs[4] * y;                       \
	a2 = xCoeffs[2];                                        \
	wx = a0 + (a1 + a2 * x) * x;                            \
	dx = 2 * a2 * x + a2 + a1;                              \
	ddx = 2 * a2;                                           \
	b0 = yCoeffs[0] + (yCoeffs[3] + yCoeffs[5] * y) * y;    \
	b1 = yCoeffs[1] + yCoeffs[4] * y;                       \
	b2 = yCoeffs[2];                                        \
	wy = b0 + (b1 + b2 * x) * x;                            \
	dy = 2 * b2 * x + b2 + b1;                              \
	ddy = 2 * b2

/* *********************************************************** */

#define	DELTA_2	wx += dx; dx += ddx; wy += dy; dy += ddy

/* *********************************************************** */

#define	DEGREE	2

/* *********************************************************** */

#define	MODE	0
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	1
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	2
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	10
#include <mlib_ImagePolynomialWarpTools.h>

#undef DEGREE

/* *********************************************************** */

/*			degree == 3				*/

/* *********************************************************** */

#define	INIT_COEFF_3                                                  \
	a0 = xCoeffs[0] + (xCoeffs[4] + (xCoeffs[7] +                 \
	    xCoeffs[9] * y) * y) * y;                                 \
	a1 = xCoeffs[1] + (xCoeffs[5] + xCoeffs[8] * y) * y;          \
	a2 = xCoeffs[2] + xCoeffs[6] * y;                             \
	a3 = xCoeffs[3];                                              \
	wx = a0 + (a1 + (a2 + a3 * x) * x) * x;                       \
	dx = (3 * a3 * x + (3 * a3 + 2 * a2)) * x + (a3 + a2 + a1);   \
	ddx = 6 * a3 * x + 6 * a3 + 2 * a2;                           \
	dddx = 6 * a3;                                                \
	b0 = yCoeffs[0] + (yCoeffs[4] + (yCoeffs[7] +                 \
	    yCoeffs[9] * y) * y) * y;                                 \
	b1 = yCoeffs[1] + (yCoeffs[5] + yCoeffs[8] * y) * y;          \
	b2 = yCoeffs[2] + yCoeffs[6] * y;                             \
	b3 = yCoeffs[3];                                              \
	wy = b0 + (b1 + (b2 + b3 * x) * x) * x;                       \
	dy = (3 * b3 * x + (3 * b3 + 2 * b2)) * x + (b3 + b2 + b1);   \
	ddy = 6 * b3 * x + 6 * b3 + 2 * b2;                           \
	dddy = 6 * b3

/* *********************************************************** */

#define	DELTA_3                                                 \
	wx += dx;                                               \
	dx += ddx;                                              \
	ddx += dddx;                                            \
	wy += dy;                                               \
	dy += ddy;                                              \
	ddy += dddy

/* *********************************************************** */

#define	DEGREE	3

/* *********************************************************** */

#define	MODE	0
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	1
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	2
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	10
#include <mlib_ImagePolynomialWarpTools.h>

#undef DEGREE

/* *********************************************************** */

/*			degree == 4				*/

/* *********************************************************** */

#define	INIT_COEFF_4                                                   \
	a0 = xCoeffs[0] + (xCoeffs[5] + (xCoeffs[9] + (xCoeffs[12] +   \
	    xCoeffs[14] * y) * y) * y) * y;                            \
	a1 = xCoeffs[1] + (xCoeffs[6] + (xCoeffs[10] +                 \
	    xCoeffs[13] * y) * y) * y;                                 \
	a2 = xCoeffs[2] + (xCoeffs[7] + xCoeffs[11] * y) * y;          \
	a3 = xCoeffs[3] + xCoeffs[8] * y;                              \
	a4 = xCoeffs[4];                                               \
	wx = a0 + (a1 + (a2 + (a3 + a4 * x) * x) * x) * x;             \
	a0 = 1 * a4 + 1 * a3 + 1 * a2 + 1 * a1;                        \
	a1 = 4 * a4 + 3 * a3 + 2 * a2;                                 \
	a2 = 6 * a4 + 3 * a3;                                          \
	a3 = 4 * a4;                                                   \
	dx = a0 + (a1 + (a2 + a3 * x) * x) * x;                        \
	a0 = 1 * a3 + 1 * a2 + 1 * a1;                                 \
	a1 = 3 * a3 + 2 * a2;                                          \
	a2 = 3 * a3;                                                   \
	ddx = a0 + (a1 + a2 * x) * x;                                  \
	a0 = 1 * a2 + 1 * a1;                                          \
	a1 = 2 * a2;                                                   \
	dddx = a0 + a1 * x;                                            \
	ddddx = a1;                                                    \
	b0 = yCoeffs[0] + (yCoeffs[5] + (yCoeffs[9] + (yCoeffs[12] +   \
	    yCoeffs[14] * y) * y) * y) * y;                            \
	b1 = yCoeffs[1] + (yCoeffs[6] + (yCoeffs[10] +                 \
	    yCoeffs[13] * y) * y) * y;                                 \
	b2 = yCoeffs[2] + (yCoeffs[7] + yCoeffs[11] * y) * y;          \
	b3 = yCoeffs[3] + yCoeffs[8] * y;                              \
	b4 = yCoeffs[4];                                               \
	wy = b0 + (b1 + (b2 + (b3 + b4 * x) * x) * x) * x;             \
	b0 = 1 * b4 + 1 * b3 + 1 * b2 + 1 * b1;                        \
	b1 = 4 * b4 + 3 * b3 + 2 * b2;                                 \
	b2 = 6 * b4 + 3 * b3;                                          \
	b3 = 4 * b4;                                                   \
	dy = b0 + (b1 + (b2 + b3 * x) * x) * x;                        \
	b0 = 1 * b3 + 1 * b2 + 1 * b1;                                 \
	b1 = 3 * b3 + 2 * b2;                                          \
	b2 = 3 * b3;                                                   \
	ddy = b0 + (b1 + b2 * x) * x;                                  \
	b0 = 1 * b2 + 1 * b1;                                          \
	b1 = 2 * b2;                                                   \
	dddy = b0 + b1 * x;                                            \
	ddddy = b1

/* *********************************************************** */

#define	DELTA_4                                                 \
	wx += dx;                                               \
	dx += ddx;                                              \
	ddx += dddx;                                            \
	dddx += ddddx;                                          \
	wy += dy;                                               \
	dy += ddy;                                              \
	ddy += dddy;                                            \
	dddy += ddddy

/* *********************************************************** */

#define	DEGREE	4

/* *********************************************************** */

#define	MODE	0
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	1
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	2
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	10
#include <mlib_ImagePolynomialWarpTools.h>

#undef DEGREE

/* *********************************************************** */

/*			degree == 5				*/

/* *********************************************************** */

#define	INIT_COEFF_5                                                    \
	a0 = xCoeffs[0] + (xCoeffs[6] + (xCoeffs[11] + (xCoeffs[15] +   \
	    (xCoeffs[18] + xCoeffs[20] * y) * y) * y) * y) * y;         \
	a1 = xCoeffs[1] + (xCoeffs[7] + (xCoeffs[12] + (xCoeffs[16] +   \
	    xCoeffs[19] * y) * y) * y) * y;                             \
	a2 = xCoeffs[2] + (xCoeffs[8] + (xCoeffs[13] +                  \
	    xCoeffs[17] * y) * y) * y;                                  \
	a3 = xCoeffs[3] + (xCoeffs[9] + xCoeffs[14] * y) * y;           \
	a4 = xCoeffs[4] + xCoeffs[10] * y;                              \
	a5 = xCoeffs[5];                                                \
	wx = a0 + (a1 + (a2 + (a3 + (a4 + a5 * x) * x) * x) * x) * x;   \
	a0 = 1 * a5 + 1 * a4 + 1 * a3 + 1 * a2 + 1 * a1;                \
	a1 = 5 * a5 + 4 * a4 + 3 * a3 + 2 * a2;                         \
	a2 = 10 * a5 + 6 * a4 + 3 * a3;                                 \
	a3 = 10 * a5 + 4 * a4;                                          \
	a4 = 5 * a5;                                                    \
	dx = a0 + (a1 + (a2 + (a3 + a4 * x) * x) * x) * x;              \
	a0 = 1 * a4 + 1 * a3 + 1 * a2 + 1 * a1;                         \
	a1 = 4 * a4 + 3 * a3 + 2 * a2;                                  \
	a2 = 6 * a4 + 3 * a3;                                           \
	a3 = 4 * a4;                                                    \
	ddx = a0 + (a1 + (a2 + a3 * x) * x) * x;                        \
	a0 = 1 * a3 + 1 * a2 + 1 * a1;                                  \
	a1 = 3 * a3 + 2 * a2;                                           \
	a2 = 3 * a3;                                                    \
	dddx = a0 + (a1 + a2 * x) * x;                                  \
	a0 = 1 * a2 + 1 * a1;                                           \
	a1 = 2 * a2;                                                    \
	ddddx = a0 + a1 * x;                                            \
	dddddx = a1;                                                    \
	b0 = yCoeffs[0] + (yCoeffs[6] + (yCoeffs[11] + (yCoeffs[15] +   \
	    (yCoeffs[18] + yCoeffs[20] * y) * y) * y) * y) * y;         \
	b1 = yCoeffs[1] + (yCoeffs[7] + (yCoeffs[12] + (yCoeffs[16] +   \
	    yCoeffs[19] * y) * y) * y) * y;                             \
	b2 = yCoeffs[2] + (yCoeffs[8] + (yCoeffs[13] +                  \
	    yCoeffs[17] * y) * y) * y;                                  \
	b3 = yCoeffs[3] + (yCoeffs[9] + yCoeffs[14] * y) * y;           \
	b4 = yCoeffs[4] + yCoeffs[10] * y;                              \
	b5 = yCoeffs[5];                                                \
	wy = b0 + (b1 + (b2 + (b3 + (b4 + b5 * x) * x) * x) * x) * x;   \
	b0 = 1 * b5 + 1 * b4 + 1 * b3 + 1 * b2 + 1 * b1;                \
	b1 = 5 * b5 + 4 * b4 + 3 * b3 + 2 * b2;                         \
	b2 = 10 * b5 + 6 * b4 + 3 * b3;                                 \
	b3 = 10 * b5 + 4 * b4;                                          \
	b4 = 5 * b5;                                                    \
	dy = b0 + (b1 + (b2 + (b3 + b4 * x) * x) * x) * x;              \
	b0 = 1 * b4 + 1 * b3 + 1 * b2 + 1 * b1;                         \
	b1 = 4 * b4 + 3 * b3 + 2 * b2;                                  \
	b2 = 6 * b4 + 3 * b3;                                           \
	b3 = 4 * b4;                                                    \
	ddy = b0 + (b1 + (b2 + b3 * x) * x) * x;                        \
	b0 = 1 * b3 + 1 * b2 + 1 * b1;                                  \
	b1 = 3 * b3 + 2 * b2;                                           \
	b2 = 3 * b3;                                                    \
	dddy = b0 + (b1 + b2 * x) * x;                                  \
	b0 = 1 * b2 + 1 * b1;                                           \
	b1 = 2 * b2;                                                    \
	ddddy = b0 + b1 * x;                                            \
	dddddy = b1

/* *********************************************************** */

#define	DELTA_5                                                 \
	wx += dx;                                               \
	dx += ddx;                                              \
	ddx += dddx;                                            \
	dddx += ddddx;                                          \
	ddddx += dddddx;                                        \
	wy += dy;                                               \
	dy += ddy;                                              \
	ddy += dddy;                                            \
	dddy += ddddy;                                          \
	ddddy += dddddy

/* *********************************************************** */

#define	DEGREE	5

/* *********************************************************** */

#define	MODE	0
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	1
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	2
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	10
#include <mlib_ImagePolynomialWarpTools.h>

#undef DEGREE

/* *********************************************************** */

#define	INIT_CLIP                                                        \
	if (degree + 1 > MAX_POWS) {                                     \
	    mem = __mlib_malloc((degree + 1) * sizeof (mlib_d64) * 2);   \
	    if (!mem)                                                    \
		return (-1);                                           \
	    xPows = mem;                                                 \
	    yPows = xPows + degree + 1;                                  \
	}                                                                \
	for (i = 0; i <= degree; i++)                                    \
	    xPows[i] = yPows[i] = 0;                                     \
	tmp_y = 1;                                                       \
	k = 0;                                                           \
	for (j = 0; j <= degree; j++) {                                  \
	    for (i = 0; i <= degree - j; i++) {                          \
		xPows[i] += xCoeffs[k] * tmp_y;                          \
		yPows[i] += yCoeffs[k] * tmp_y;                          \
		k++;                                                     \
	    }                                                            \
	    tmp_y *= y;                                                  \
	}
/* *********************************************************** */

#define	DEGREE	6

/* *********************************************************** */

#define	MODE	0
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	1
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	2
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	3
#include <mlib_ImagePolynomialWarpTools.h>

/* *********************************************************** */

#define	MODE	10
#include <mlib_ImagePolynomialWarpTools.h>

#undef DEGREE

/* *********************************************************** */
