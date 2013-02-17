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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN_2_Fp.c	9.2	07/11/05 SMI"

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#define	INT_TYPE	mlib_s64
#define	SHIFT	24
#define	DTOLL(l, d)	BASIC_DTOLL(l, d, SHIFT)
#define	SHIFT_DIV(a)                                             \
	((a) > 0 ? ((a)>>SHIFT) : (-((-(a))>>SHIFT)))

/* *********************************************************** */

#define	INIT_COEFFS                                              \
	a0 = xCoeffs[0] + xCoeffs[3] * y + xCoeffs[5] * y * y;   \
	a1 = xCoeffs[1] + xCoeffs[4] * y;                        \
	a2 = xCoeffs[2];                                         \
	wx = a0 + a1 * x + a2 * x * x + 1;                       \
	dx = 2 * a2 * x + a2 + a1;                               \
	ddx = 2 * a2;                                            \
	b0 = yCoeffs[0] + yCoeffs[3] * y + yCoeffs[5] * y * y;   \
	b1 = yCoeffs[1] + yCoeffs[4] * y;                        \
	b2 = yCoeffs[2];                                         \
	wy = b0 + b1 * x + b2 * x * x + 1;                       \
	dy = 2 * b2 * x + b2 + b1;                               \
	ddy = 2 * b2

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_1)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s64 pix0;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		DTOLL(wx_int, wx);
		DTOLL(dx_int, dx);
		DTOLL(ddx_int, ddx);
		DTOLL(wy_int, wy);
		DTOLL(dy_int, dy);
		DTOLL(ddy_int, ddy);

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

			wx_int += dx_int;
			dx_int += ddx_int;
			wy_int += dy_int;
			dy_int += ddy_int;

			dp = (void *)(((mlib_addr)(dstData +
			    8 * i) & mask) | ((mlib_addr)buf & ~mask));
			((mlib_s64 *)dp)[0] = pix0;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_2)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s64 pix0, pix1;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		DTOLL(wx_int, wx);
		DTOLL(dx_int, dx);
		DTOLL(ddx_int, ddx);
		DTOLL(wy_int, wy);
		DTOLL(dy_int, dy);
		DTOLL(ddy_int, ddy);

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

			wx_int += dx_int;
			dx_int += ddx_int;
			wy_int += dy_int;
			dy_int += ddy_int;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_3)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		DTOLL(wx_int, wx);
		DTOLL(dx_int, dx);
		DTOLL(ddx_int, ddx);
		DTOLL(wy_int, wy);
		DTOLL(dy_int, dy);
		DTOLL(ddy_int, ddy);

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

			wx_int += dx_int;
			dx_int += ddx_int;
			wy_int += dy_int;
			dy_int += ddy_int;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_4)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s64 pix0, pix1, pix2, pix3;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;
	INT_TYPE wx_int, dx_int, ddx_int, wy_int, dy_int, ddy_int;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

		DTOLL(wx_int, wx);
		DTOLL(dx_int, dx);
		DTOLL(ddx_int, ddx);
		DTOLL(wy_int, wy);
		DTOLL(dy_int, dy);
		DTOLL(ddy_int, ddy);

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

			wx_int += dx_int;
			dx_int += ddx_int;
			wy_int += dy_int;
			dy_int += ddy_int;

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

#define	INIT_COEFFS                                              \
	a0 = xCoeffs[0] + xCoeffs[3] * y + xCoeffs[5] * y * y;   \
	a1 = xCoeffs[1] + xCoeffs[4] * y;                        \
	a2 = xCoeffs[2];                                         \
	wx = a0 + a1 * x + a2 * x * x + 1;                       \
	dx = 2 * a2 * x + a2 + a1;                               \
	ddx = 2 * a2;                                            \
	b0 = yCoeffs[0] + yCoeffs[3] * y + yCoeffs[5] * y * y;   \
	b1 = yCoeffs[1] + yCoeffs[4] * y;                        \
	b2 = yCoeffs[2];                                         \
	wy = b0 + b1 * x + b2 * x * x + 1;                       \
	dy = 2 * b2 * x + b2 + b1;                               \
	ddy = 2 * b2

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_1)
{
	mlib_d64 buf[8], pix0, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

			dp = (void *)(((mlib_addr)(dstData +
			    8 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_2)
{
	mlib_d64 buf[8], pix0, pix1, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_3)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_4)
{
	mlib_d64 buf[8], pix0, pix1, pix2, pix3, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

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
