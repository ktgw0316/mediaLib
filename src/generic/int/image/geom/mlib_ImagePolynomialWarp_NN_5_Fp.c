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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN_5_Fp.c	9.2	07/11/05 SMI"

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	INT_TYPE	mlib_s64
#define	SHIFT	24
#define	DTOLL(d)	((INT_TYPE) ((d) * ((mlib_s64)1<<SHIFT)))
#define	SHIFT_DIV(a)                                             \
	((a) > 0 ? ((a)>>SHIFT) : (-((-(a))>>SHIFT)))

/* *********************************************************** */

#define	INIT_COEFFS                                                     \
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
	wx += 1;                                                        \
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
	wy += 1;                                                        \
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

#define	DELTA                                                   \
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

#define	DELTA_INT                                               \
	wx_int += dx_int;                                       \
	dx_int += ddx_int;                                      \
	ddx_int += dddx_int;                                    \
	dddx_int += ddddx_int;                                  \
	ddddx_int += dddddx_int;                                \
	wy_int += dy_int;                                       \
	dy_int += ddy_int;                                      \
	ddy_int += dddy_int;                                    \
	dddy_int += ddddy_int;                                  \
	ddddy_int += dddddy_int

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_1)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s64 pix0;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;
	INT_TYPE dddx_int, ddddx_int, dddddx_int;
	INT_TYPE dddy_int, ddddy_int, dddddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		wx_int = DTOLL(wx);
		dx_int = DTOLL(dx);
		ddx_int = DTOLL(ddx);
		dddx_int = DTOLL(dddx);
		ddddx_int = DTOLL(ddddx);
		dddddx_int = DTOLL(dddddx);
		wy_int = DTOLL(wy);
		dy_int = DTOLL(dy);
		ddy_int = DTOLL(ddy);
		dddy_int = DTOLL(dddy);
		ddddy_int = DTOLL(ddddy);
		dddddy_int = DTOLL(dddddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)(SHIFT_DIV(wx_int));
			iwy = (mlib_s32)(SHIFT_DIV(wy_int));

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 8) & mask));

			pix0 = ((mlib_s64 *)sp)[0];

			DELTA_INT;

			dp = (void *)(((mlib_addr)(dstData +
			    8 * i) & mask) | ((mlib_addr)buf & ~mask));
			((mlib_s64 *)dp)[0] = pix0;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_2)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s64 pix0, pix1;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;
	INT_TYPE dddx_int, ddddx_int, dddddx_int;
	INT_TYPE dddy_int, ddddy_int, dddddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		wx_int = DTOLL(wx);
		dx_int = DTOLL(dx);
		ddx_int = DTOLL(ddx);
		dddx_int = DTOLL(dddx);
		ddddx_int = DTOLL(ddddx);
		dddddx_int = DTOLL(dddddx);
		wy_int = DTOLL(wy);
		dy_int = DTOLL(dy);
		ddy_int = DTOLL(ddy);
		dddy_int = DTOLL(dddy);
		ddddy_int = DTOLL(ddddy);
		dddddy_int = DTOLL(dddddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)(SHIFT_DIV(wx_int));
			iwy = (mlib_s32)(SHIFT_DIV(wy_int));

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 16) & mask));

			pix0 = ((mlib_s64 *)sp)[0];
			pix1 = ((mlib_s64 *)sp)[1];

			DELTA_INT;

			dp = (void *)(((mlib_addr)(dstData +
			    16 * i) & mask) | ((mlib_addr)buf & ~mask));
			((mlib_s64 *)dp)[0] = pix0;
			((mlib_s64 *)dp)[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_3)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;
	INT_TYPE dddx_int, ddddx_int, dddddx_int;
	INT_TYPE dddy_int, ddddy_int, dddddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		wx_int = DTOLL(wx);
		dx_int = DTOLL(dx);
		ddx_int = DTOLL(ddx);
		dddx_int = DTOLL(dddx);
		ddddx_int = DTOLL(ddddx);
		dddddx_int = DTOLL(dddddx);
		wy_int = DTOLL(wy);
		dy_int = DTOLL(dy);
		ddy_int = DTOLL(ddy);
		dddy_int = DTOLL(dddy);
		ddddy_int = DTOLL(ddddy);
		dddddy_int = DTOLL(dddddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)(SHIFT_DIV(wx_int));
			iwy = (mlib_s32)(SHIFT_DIV(wy_int));

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			DELTA_INT;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 24) & mask));
			dp = (void *)(((mlib_addr)(dstData +
			    24 * i) & mask) | ((mlib_addr)buf & ~mask));
			((mlib_s64 *)dp)[0] = ((mlib_s64 *)sp)[0];
			((mlib_s64 *)dp)[1] = ((mlib_s64 *)sp)[1];
			((mlib_s64 *)dp)[2] = ((mlib_s64 *)sp)[2];
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_4)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s64 pix0, pix1, pix2, pix3;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;
	INT_TYPE dddx_int, ddddx_int, dddddx_int;
	INT_TYPE dddy_int, ddddy_int, dddddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		wx_int = DTOLL(wx);
		dx_int = DTOLL(dx);
		ddx_int = DTOLL(ddx);
		dddx_int = DTOLL(dddx);
		ddddx_int = DTOLL(ddddx);
		dddddx_int = DTOLL(dddddx);
		wy_int = DTOLL(wy);
		dy_int = DTOLL(dy);
		ddy_int = DTOLL(ddy);
		dddy_int = DTOLL(dddy);
		ddddy_int = DTOLL(ddddy);
		dddddy_int = DTOLL(dddddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)(SHIFT_DIV(wx_int));
			iwy = (mlib_s32)(SHIFT_DIV(wy_int));

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 32) & mask));

			pix0 = ((mlib_s64 *)sp)[0];
			pix1 = ((mlib_s64 *)sp)[1];
			pix2 = ((mlib_s64 *)sp)[2];
			pix3 = ((mlib_s64 *)sp)[3];

			DELTA_INT;

			dp = (void *)(((mlib_addr)(dstData +
			    32 * i) & mask) | ((mlib_addr)buf & ~mask));
			((mlib_s64 *)dp)[0] = pix0;
			((mlib_s64 *)dp)[1] = pix1;
			((mlib_s64 *)dp)[2] = pix2;
			((mlib_s64 *)dp)[3] = pix3;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	INIT_COEFFS                                                     \
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
	wx += 1;                                                        \
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
	wy += 1;                                                        \
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

#define	DELTA                                                   \
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

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_1)
{
	mlib_d64 buf[8], pix0, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 8) & mask));

			pix0 = sp[0];

			DELTA;

			dp = (void *)(((mlib_addr)(dstData +
			    8 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_2)
{
	mlib_d64 buf[8], pix0, pix1, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 16) & mask));

			pix0 = sp[0];
			pix1 = sp[1];

			DELTA;

			dp = (void *)(((mlib_addr)(dstData +
			    16 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_3)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			DELTA;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 24) & mask));
			dp = (void *)(((mlib_addr)(dstData +
			    24 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_4)
{
	mlib_d64 buf[8], pix0, pix1, pix2, pix3, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 32) & mask));

			pix0 = sp[0];
			pix1 = sp[1];
			pix2 = sp[2];
			pix3 = sp[3];

			DELTA;

			dp = (void *)(((mlib_addr)(dstData +
			    32 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
			dp[1] = pix1;
			dp[2] = pix2;
			dp[3] = pix3;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

#endif /* _NO_LONGLONG */
/* *********************************************************** */
