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

#pragma ident	"@(#)mlib_c_VideoColorYUV2ABGR420_WX3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_WX3 - color conversion-YUV420 to ABGR
 *                                       inside clipping window with zoom x3
 *
 * SYNOPSIS
 *
 * mlib_status mlib_VideoColorYUV2ABGR420_WX3(mlib_u8       *abgr,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      abgr_stride,
 *                                            mlib_s32      y_stride,
 *                                            mlib_s32      uv_stride,
 *                                            mlib_s32      left,
 *                                            mlib_s32      top,
 *                                            mlib_s32      right,
 *                                            mlib_s32      bottom)
 *
 * ARGUMENT
 *      abgr        Pointer to output ABGR image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the not zoomed image
 *      height      Height of the not zoomed image
 *      abgr_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input image
 *      left        left border of the clipping window
 *      top         top border of the clipping window
 *      right       right opened border of the clipping window
 *      bottom      bottom opened border of the clipping window
 *
 * DESCRIPTION
 *        |A| = 0
 *        |R|   |1.1644   0.0000   1.5966|   [Y   16.0000 ]
 *        |G| = |1.1644  -0.3920  -0.8132| * [U - 128.0000]
 *        |B|   |1.1644   2.0184   0.0000|   [V   128.0000]
 *
 * NOTES
 *  abgr and y must be 8-byte aligned
 *  u and v must be 4-byte aligned
 *  width must be a multiple of 8
 *  height must be a multiple of 2
 *  abgr_stride and y_stride must be a multiple of 8
 *  uv_stride must be a multiple of 4
 *  window borders must be: 0 <= left < right <= width*3
 *  and 0 <= top < bottom <= height*3
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR420_WX3 = \
	__mlib_VideoColorYUV2ABGR420_WX3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_WX3) mlib_VideoColorYUV2ABGR420_WX3
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_WX3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(val, dst)	dst = pClip[val]

/* *********************************************************** */

#define	SCLAMP_U8(s, dst)                                       \
	{                                                       \
	    mlib_s32 v = s, mask = (v - 0x1fe) >> 31;           \
	                                                        \
	    dst = (((mlib_u32)v >> 1) | ~mask) & ~(v >> 31);    \
	}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	CALC4(pY, pZ, pU, pV)                                   \
	sf1 = pU[0];                                            \
	sf2 = pV[0];                                            \
	sf01 = *(mlib_u16 *)pY;                                 \
	sf02 = *(mlib_u16 *)pZ;                                 \
	sf03 = (mlib_u32)sf01 >> 8;                             \
	sf01 = sf01 & 0xff;                                     \
	sf04 = (mlib_u32)sf02 >> 8;                             \
	sf02 = sf02 & 0xff;                                     \
	sfb = mlib_tBU[sf1] - BBi;                              \
	sfg = mlib_tGU[sf1] - mlib_tGV[sf2];                    \
	sfr = mlib_tRV[sf2] - RRi;                              \
	sf01 = mlib_tYY[sf01];                                  \
	sf03 = mlib_tYY[sf03];                                  \
	sf02 = mlib_tYY[sf02];                                  \
	sf04 = mlib_tYY[sf04];                                  \
	TCLAMP_U8(sf01 + sfb, r0);                              \
	r0 <<= 8;                                               \
	TCLAMP_U8(sf01 + sfg, rr);                              \
	r0 |= rr << 16;                                         \
	TCLAMP_U8(sf01 + sfr, rr);                              \
	r0 |= rr << 24;                                         \
	TCLAMP_U8(sf03 + sfb, r2);                              \
	r2 <<= 8;                                               \
	TCLAMP_U8(sf03 + sfg, rr);                              \
	r2 |= rr << 16;                                         \
	TCLAMP_U8(sf03 + sfr, rr);                              \
	r2 |= rr << 24;                                         \
	TCLAMP_U8(sf02 + sfb, r1);                              \
	r1 <<= 8;                                               \
	TCLAMP_U8(sf02 + sfg, rr);                              \
	r1 |= rr << 16;                                         \
	TCLAMP_U8(sf02 + sfr, rr);                              \
	r1 |= rr << 24;                                         \
	TCLAMP_U8(sf04 + sfb, r3);                              \
	r3 <<= 8;                                               \
	TCLAMP_U8(sf04 + sfg, rr);                              \
	r3 |= rr << 16;                                         \
	TCLAMP_U8(sf04 + sfr, rr);                              \
	r3 |= rr << 24

#else /* _LITTLE_ENDIAN */

#define	CALC4(pY, pZ, pU, pV)                                   \
	sf1 = pU[0];                                            \
	sf2 = pV[0];                                            \
	sf01 = *(mlib_u16 *)pY;                                 \
	sf02 = *(mlib_u16 *)pZ;                                 \
	sf03 = sf01 & 0xff;                                     \
	sf01 = (mlib_u32)sf01 >> 8;                             \
	sf04 = sf02 & 0xff;                                     \
	sf02 = (mlib_u32)sf02 >> 8;                             \
	sfb = mlib_tBU[sf1] - BBi;                              \
	sfg = mlib_tGU[sf1] - mlib_tGV[sf2];                    \
	sfr = mlib_tRV[sf2] - RRi;                              \
	sf01 = mlib_tYY[sf01];                                  \
	sf03 = mlib_tYY[sf03];                                  \
	sf02 = mlib_tYY[sf02];                                  \
	sf04 = mlib_tYY[sf04];                                  \
	TCLAMP_U8(sf01 + sfb, r0);                              \
	r0 <<= 16;                                              \
	TCLAMP_U8(sf01 + sfg, rr);                              \
	r0 |= rr << 8;                                          \
	TCLAMP_U8(sf01 + sfr, rr);                              \
	r0 |= rr;                                               \
	TCLAMP_U8(sf03 + sfb, r2);                              \
	r2 <<= 16;                                              \
	TCLAMP_U8(sf03 + sfg, rr);                              \
	r2 |= rr << 8;                                          \
	TCLAMP_U8(sf03 + sfr, rr);                              \
	r2 |= rr;                                               \
	TCLAMP_U8(sf02 + sfb, r1);                              \
	r1 <<= 16;                                              \
	TCLAMP_U8(sf02 + sfg, rr);                              \
	r1 |= rr << 8;                                          \
	TCLAMP_U8(sf02 + sfr, rr);                              \
	r1 |= rr;                                               \
	TCLAMP_U8(sf04 + sfb, r3);                              \
	r3 <<= 16;                                              \
	TCLAMP_U8(sf04 + sfg, rr);                              \
	r3 |= rr << 8;                                          \
	TCLAMP_U8(sf04 + sfr, rr);                              \
	r3 |= rr

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	CALC4I(Y0, Y1, Z0, Z1, U0, V0)                          \
	sf1 = U0;                                               \
	sf2 = V0;                                               \
	sf01 = Y0;                                              \
	sf02 = Z0;                                              \
	sf03 = Y1;                                              \
	sf04 = Z1;                                              \
	sfb = mlib_tBU[sf1] - BBi;                              \
	sfg = mlib_tGU[sf1] - mlib_tGV[sf2];                    \
	sfr = mlib_tRV[sf2] - RRi;                              \
	sf01 = mlib_tYY[sf01];                                  \
	sf03 = mlib_tYY[sf03];                                  \
	sf02 = mlib_tYY[sf02];                                  \
	sf04 = mlib_tYY[sf04];                                  \
	TCLAMP_U8(sf01 + sfb, r0);                              \
	r0 <<= 8;                                               \
	TCLAMP_U8(sf01 + sfg, rr);                              \
	r0 |= rr << 16;                                         \
	TCLAMP_U8(sf01 + sfr, rr);                              \
	r0 |= rr << 24;                                         \
	TCLAMP_U8(sf03 + sfb, r2);                              \
	r2 <<= 8;                                               \
	TCLAMP_U8(sf03 + sfg, rr);                              \
	r2 |= rr << 16;                                         \
	TCLAMP_U8(sf03 + sfr, rr);                              \
	r2 |= rr << 24;                                         \
	TCLAMP_U8(sf02 + sfb, r1);                              \
	r1 <<= 8;                                               \
	TCLAMP_U8(sf02 + sfg, rr);                              \
	r1 |= rr << 16;                                         \
	TCLAMP_U8(sf02 + sfr, rr);                              \
	r1 |= rr << 24;                                         \
	TCLAMP_U8(sf04 + sfb, r3);                              \
	r3 <<= 8;                                               \
	TCLAMP_U8(sf04 + sfg, rr);                              \
	r3 |= rr << 16;                                         \
	TCLAMP_U8(sf04 + sfr, rr);                              \
	r3 |= rr << 24

#else /* _LITTLE_ENDIAN */

#define	CALC4I(Y0, Y1, Z0, Z1, U0, V0)                          \
	sf1 = U0;                                               \
	sf2 = V0;                                               \
	sf01 = Y0;                                              \
	sf02 = Z0;                                              \
	sf03 = Y1;                                              \
	sf04 = Z1;                                              \
	sfb = mlib_tBU[sf1] - BBi;                              \
	sfg = mlib_tGU[sf1] - mlib_tGV[sf2];                    \
	sfr = mlib_tRV[sf2] - RRi;                              \
	sf01 = mlib_tYY[sf01];                                  \
	sf03 = mlib_tYY[sf03];                                  \
	sf02 = mlib_tYY[sf02];                                  \
	sf04 = mlib_tYY[sf04];                                  \
	TCLAMP_U8(sf01 + sfb, r0);                              \
	r0 <<= 16;                                              \
	TCLAMP_U8(sf01 + sfg, rr);                              \
	r0 |= rr << 8;                                          \
	TCLAMP_U8(sf01 + sfr, rr);                              \
	r0 |= rr;                                               \
	TCLAMP_U8(sf03 + sfb, r2);                              \
	r2 <<= 16;                                              \
	TCLAMP_U8(sf03 + sfg, rr);                              \
	r2 |= rr << 8;                                          \
	TCLAMP_U8(sf03 + sfr, rr);                              \
	r2 |= rr;                                               \
	TCLAMP_U8(sf02 + sfb, r1);                              \
	r1 <<= 16;                                              \
	TCLAMP_U8(sf02 + sfg, rr);                              \
	r1 |= rr << 8;                                          \
	TCLAMP_U8(sf02 + sfr, rr);                              \
	r1 |= rr;                                               \
	TCLAMP_U8(sf04 + sfb, r3);                              \
	r3 <<= 16;                                              \
	TCLAMP_U8(sf04 + sfg, rr);                              \
	r3 |= rr << 8;                                          \
	TCLAMP_U8(sf04 + sfr, rr);                              \
	r3 |= rr

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR420_WX3(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride,
	mlib_s32 left,
	mlib_s32 top,
	mlib_s32 right,
	mlib_s32 bottom)
{
	mlib_s32 i, j;
	mlib_u8 *rgb0, *rgb1, *rgb2, *rgb3, *rgb4, *rgb5;
	mlib_s32 i1, i2, i3, i4, i5, iy0, iy1;
	mlib_s32 jy1, jy2, jy3, jy4;
	const mlib_u8 *Y;
	const mlib_u8 *U;
	const mlib_u8 *V;
	const mlib_u8 *Z;

	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_s32 sf01, sf02, sf03, sf04, sf1, sf2, sfr, sfg, sfb;
	mlib_s32 r0, r1, r2, r3, rr;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;
	mlib_s32 loff = left / 6;
	mlib_s32 toff = top / 6;
	mlib_s32 loff2 = (left + 5) / 6;
	mlib_s32 toff2 = (top + 5) / 6;

	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if (wd & 7 || ht & 1 || rgb_stride & 7 || uv_stride & 3 || y_stride & 7)
		return (MLIB_FAILURE);

	if (left < 0 || top < 0 || right <= left || bottom <= top ||
		3 * wd < right || 3 * ht < bottom)
		return (MLIB_FAILURE);

	abgr += (loff2 * 6) * 4 + (toff2 * 6) * rgb_stride;
	y_inp += (loff2 * 2) + (toff2 * 2) * y_stride;
	u_inp += (loff2) + (toff2) * uv_stride;
	v_inp += (loff2) + (toff2) * uv_stride;

	Y = y_inp;
	U = u_inp;
	V = v_inp;
	Z = y_inp + y_stride;

	wd = (right - loff2 * 6) / 6;
	ht = (bottom - toff2 * 6) / 6;

	right -= loff * 6;
	left -= loff * 6;
	bottom -= toff * 6;
	top -= toff * 6;

	rgb0 = abgr;
	rgb1 = rgb0 + rgb_stride;
	rgb2 = rgb1 + rgb_stride;
	rgb3 = rgb2 + rgb_stride;
	rgb4 = rgb3 + rgb_stride;
	rgb5 = rgb4 + rgb_stride;

	i1 = -5;
	i2 = -4;
	i3 = -3;
	i4 = -2;
	i5 = -1;

	if (left) {
		if (right == 2) {
			iy0 = -1;
			iy1 = -2;
			i2 = i3 = i4 = i5 = i1;
		} else if (right == 3) {
			iy0 = -1;
			iy1 = -2;
			i3 = i4 = i5 = i2;

			if (left == 2)
				i1 = i2;
			else
				i3 = i1;
		} else {
			iy0 = -2;
			iy1 = -1;

			if (left == 5)
				i1 = i2 = i3 = i4 = i5;
			else if (left == 4)
				i1 = i2 = i3 = i4;
			else if (left == 3)
				i1 = i2 = i3;
			else if (left == 2)
				i1 = i2;

			if (right == 5)
				i5 = i4;
			else if (right == 4)
				i5 = i4 = i3;
		}
	}

	if (right < 6 && left)
/* tail in head already */
		right = 0;
	else {
		right -= wd * 6;

		if (left)
			right -= 6;

		if (right == 1)
			jy1 = jy2 = jy3 = jy4 = 0;
		else if (right == 2)
			jy1 = jy2 = jy3 = jy4 = 1;
		else if (right == 3) {
			jy2 = jy3 = jy4 = 2;
			jy1 = 1;
		} else if (right == 4) {
			jy3 = jy4 = 3;
			jy1 = 1;
			jy2 = 2;
		} else {
			jy1 = 1;
			jy2 = 2;
			jy3 = 3;
			jy4 = 4;
		}
	}

	if (top) {
		const mlib_u8 *Y_, *Z_;

		rgb1 -= 6 * rgb_stride;
		rgb2 -= 6 * rgb_stride;
		rgb3 -= 6 * rgb_stride;
		rgb4 -= 6 * rgb_stride;
		rgb5 -= 6 * rgb_stride;

		if (bottom == 2) {
			Y_ = Y - y_stride;
			Z_ = Y - 2 * y_stride;
			rgb2 = rgb3 = rgb4 = rgb5 = rgb1;
		} else if (bottom == 3) {
			Y_ = Y - y_stride;
			Z_ = Y - 2 * y_stride;
			rgb3 = rgb4 = rgb5 = rgb2;

			if (top == 2)
				rgb1 = rgb2;
			else
				rgb3 = rgb1;
		} else {
			Y_ = Y - 2 * y_stride;
			Z_ = Y - y_stride;

			if (top == 5)
				rgb1 = rgb2 = rgb3 = rgb4 = rgb5;
			else if (top == 4)
				rgb1 = rgb2 = rgb3 = rgb4;
			else if (top == 3)
				rgb1 = rgb2 = rgb3;
			else if (top == 2)
				rgb1 = rgb2;

			if (bottom == 5)
				rgb5 = rgb4;
			else if (bottom == 4)
				rgb5 = rgb4 = rgb3;
		}

		if (left) {
			CALC4I(Y_[iy0], Y_[iy1], Z_[iy0], Z_[iy1],
				*(U - uv_stride - 1), *(V - uv_stride - 1));
			((mlib_s32 *)rgb1)[i1] = r0;
			((mlib_s32 *)rgb1)[i2] = r0;
			((mlib_s32 *)rgb1)[i3] = r2;
			((mlib_s32 *)rgb1)[i4] = r2;
			((mlib_s32 *)rgb1)[i5] = r2;

			((mlib_s32 *)rgb2)[i1] = r0;
			((mlib_s32 *)rgb2)[i2] = r0;
			((mlib_s32 *)rgb2)[i3] = r2;
			((mlib_s32 *)rgb2)[i4] = r2;
			((mlib_s32 *)rgb2)[i5] = r2;

			((mlib_s32 *)rgb3)[i1] = r1;
			((mlib_s32 *)rgb3)[i2] = r1;
			((mlib_s32 *)rgb3)[i3] = r3;
			((mlib_s32 *)rgb3)[i4] = r3;
			((mlib_s32 *)rgb3)[i5] = r3;

			((mlib_s32 *)rgb4)[i1] = r1;
			((mlib_s32 *)rgb4)[i2] = r1;
			((mlib_s32 *)rgb4)[i3] = r3;
			((mlib_s32 *)rgb4)[i4] = r3;
			((mlib_s32 *)rgb4)[i5] = r3;

			((mlib_s32 *)rgb5)[i1] = r1;
			((mlib_s32 *)rgb5)[i2] = r1;
			((mlib_s32 *)rgb5)[i3] = r3;
			((mlib_s32 *)rgb5)[i4] = r3;
			((mlib_s32 *)rgb5)[i5] = r3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(1)
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 0; i < wd; i++) {
			CALC4(Y_, Z_, (U - uv_stride + i), (V - uv_stride + i));
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb1)[1] = r0;
			((mlib_s32 *)rgb1)[2] = r0;
			((mlib_s32 *)rgb1)[3] = r2;
			((mlib_s32 *)rgb1)[4] = r2;
			((mlib_s32 *)rgb1)[5] = r2;

			((mlib_s32 *)rgb2)[0] = r0;
			((mlib_s32 *)rgb2)[1] = r0;
			((mlib_s32 *)rgb2)[2] = r0;
			((mlib_s32 *)rgb2)[3] = r2;
			((mlib_s32 *)rgb2)[4] = r2;
			((mlib_s32 *)rgb2)[5] = r2;

			((mlib_s32 *)rgb3)[0] = r1;
			((mlib_s32 *)rgb3)[1] = r1;
			((mlib_s32 *)rgb3)[2] = r1;
			((mlib_s32 *)rgb3)[3] = r3;
			((mlib_s32 *)rgb3)[4] = r3;
			((mlib_s32 *)rgb3)[5] = r3;

			((mlib_s32 *)rgb4)[0] = r1;
			((mlib_s32 *)rgb4)[1] = r1;
			((mlib_s32 *)rgb4)[2] = r1;
			((mlib_s32 *)rgb4)[3] = r3;
			((mlib_s32 *)rgb4)[4] = r3;
			((mlib_s32 *)rgb4)[5] = r3;

			((mlib_s32 *)rgb5)[0] = r1;
			((mlib_s32 *)rgb5)[1] = r1;
			((mlib_s32 *)rgb5)[2] = r1;
			((mlib_s32 *)rgb5)[3] = r3;
			((mlib_s32 *)rgb5)[4] = r3;
			((mlib_s32 *)rgb5)[5] = r3;

			Y_ += 2;
			Z_ += 2;
			rgb1 += 24;
			rgb2 += 24;
			rgb3 += 24;
			rgb4 += 24;
			rgb5 += 24;
		}

		if (right) {
			CALC4(Y_, Z_, (U - uv_stride + wd),
				(V - uv_stride + wd));
			((mlib_s32 *)rgb1)[jy4] = r2;
			((mlib_s32 *)rgb1)[jy3] = r2;
			((mlib_s32 *)rgb1)[jy2] = r0;
			((mlib_s32 *)rgb1)[jy1] = r0;
			((mlib_s32 *)rgb1)[0] = r0;

			((mlib_s32 *)rgb2)[jy4] = r2;
			((mlib_s32 *)rgb2)[jy3] = r2;
			((mlib_s32 *)rgb2)[jy2] = r0;
			((mlib_s32 *)rgb2)[jy1] = r0;
			((mlib_s32 *)rgb2)[0] = r0;

			((mlib_s32 *)rgb3)[jy4] = r3;
			((mlib_s32 *)rgb3)[jy3] = r3;
			((mlib_s32 *)rgb3)[jy2] = r1;
			((mlib_s32 *)rgb3)[jy1] = r1;
			((mlib_s32 *)rgb3)[0] = r1;

			((mlib_s32 *)rgb4)[jy4] = r3;
			((mlib_s32 *)rgb4)[jy3] = r3;
			((mlib_s32 *)rgb4)[jy2] = r1;
			((mlib_s32 *)rgb4)[jy1] = r1;
			((mlib_s32 *)rgb4)[0] = r1;

			((mlib_s32 *)rgb5)[jy4] = r3;
			((mlib_s32 *)rgb5)[jy3] = r3;
			((mlib_s32 *)rgb5)[jy2] = r1;
			((mlib_s32 *)rgb5)[jy1] = r1;
			((mlib_s32 *)rgb5)[0] = r1;
		}

		rgb1 = rgb0 + rgb_stride;
		rgb2 = rgb1 + rgb_stride;
		rgb3 = rgb2 + rgb_stride;
		rgb4 = rgb3 + rgb_stride;
		rgb5 = rgb4 + rgb_stride;
	}

	for (j = 0; j < ht; j++) {

		if ((left)) {
			CALC4I(Y[iy0], Y[iy1], Z[iy0], Z[iy1], *(U - 1),
				*(V - 1));
			((mlib_s32 *)rgb0)[i1] = r0;
			((mlib_s32 *)rgb0)[i2] = r0;
			((mlib_s32 *)rgb1)[i1] = r0;
			((mlib_s32 *)rgb1)[i2] = r0;
			((mlib_s32 *)rgb2)[i1] = r0;
			((mlib_s32 *)rgb2)[i2] = r0;

			((mlib_s32 *)rgb0)[i3] = r2;
			((mlib_s32 *)rgb0)[i4] = r2;
			((mlib_s32 *)rgb0)[i5] = r2;
			((mlib_s32 *)rgb1)[i3] = r2;
			((mlib_s32 *)rgb1)[i4] = r2;
			((mlib_s32 *)rgb1)[i5] = r2;
			((mlib_s32 *)rgb2)[i3] = r2;
			((mlib_s32 *)rgb2)[i4] = r2;
			((mlib_s32 *)rgb2)[i5] = r2;

			((mlib_s32 *)rgb3)[i1] = r1;
			((mlib_s32 *)rgb3)[i2] = r1;
			((mlib_s32 *)rgb4)[i1] = r1;
			((mlib_s32 *)rgb4)[i2] = r1;
			((mlib_s32 *)rgb5)[i1] = r1;
			((mlib_s32 *)rgb5)[i2] = r1;

			((mlib_s32 *)rgb3)[i3] = r3;
			((mlib_s32 *)rgb3)[i4] = r3;
			((mlib_s32 *)rgb3)[i5] = r3;
			((mlib_s32 *)rgb4)[i3] = r3;
			((mlib_s32 *)rgb4)[i4] = r3;
			((mlib_s32 *)rgb4)[i5] = r3;
			((mlib_s32 *)rgb5)[i3] = r3;
			((mlib_s32 *)rgb5)[i4] = r3;
			((mlib_s32 *)rgb5)[i5] = r3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)

#ifdef _LP64
/* to prevent compiler Bug Id  4193330 */
#pragma unroll(1)
#endif /* _LP64 */
#endif /* __SUNPRO_C */
		for (i = 0; i < wd; i++) {

			sf1 = U[0];
			sf2 = V[0];

			sf01 = *(mlib_u16 *)Y;
			sf02 = *(mlib_u16 *)Z;
#ifdef _LITTLE_ENDIAN
			sf03 = (mlib_u32)sf01 >> 8;
			sf01 = sf01 & 0xff;
			sf04 = (mlib_u32)sf02 >> 8;
			sf02 = sf02 & 0xff;
#else /* _LITTLE_ENDIAN */
			sf03 = sf01 & 0xff;
			sf01 = (mlib_u32)sf01 >> 8;
			sf04 = sf02 & 0xff;
			sf02 = (mlib_u32)sf02 >> 8;
#endif /* _LITTLE_ENDIAN */

			sfb = mlib_tBU[sf1] - BBi;
			sfg = mlib_tGU[sf1] - mlib_tGV[sf2];
			sfr = mlib_tRV[sf2] - RRi;

			sf01 = mlib_tYY[sf01];
			sf03 = mlib_tYY[sf03];
			sf02 = mlib_tYY[sf02];
			sf04 = mlib_tYY[sf04];

#ifdef _LITTLE_ENDIAN
			TCLAMP_U8(sf01 + sfb, r0);
			r0 <<= 8;
			TCLAMP_U8(sf01 + sfg, rr);
			r0 |= rr << 16;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr << 24;

			TCLAMP_U8(sf03 + sfb, r2);
			r2 <<= 8;
			TCLAMP_U8(sf03 + sfg, rr);
			r2 |= rr << 16;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr << 24;

			TCLAMP_U8(sf02 + sfb, r1);
			r1 <<= 8;
			TCLAMP_U8(sf02 + sfg, rr);
			r1 |= rr << 16;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr << 24;

			TCLAMP_U8(sf04 + sfb, r3);
			r3 <<= 8;
			TCLAMP_U8(sf04 + sfg, rr);
			r3 |= rr << 16;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr << 24;
#else /* _LITTLE_ENDIAN */
			TCLAMP_U8(sf01 + sfb, r0);
			r0 <<= 16;
			TCLAMP_U8(sf01 + sfg, rr);
			r0 |= rr << 8;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr;

			TCLAMP_U8(sf03 + sfb, r2);
			r2 <<= 16;
			TCLAMP_U8(sf03 + sfg, rr);
			r2 |= rr << 8;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr;

			TCLAMP_U8(sf02 + sfb, r1);
			r1 <<= 16;
			TCLAMP_U8(sf02 + sfg, rr);
			r1 |= rr << 8;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr;

			TCLAMP_U8(sf04 + sfb, r3);
			r3 <<= 16;
			TCLAMP_U8(sf04 + sfg, rr);
			r3 |= rr << 8;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr;
#endif /* _LITTLE_ENDIAN */

			((mlib_s32 *)rgb0)[0] = r0;
			((mlib_s32 *)rgb0)[1] = r0;
			((mlib_s32 *)rgb0)[2] = r0;
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb1)[1] = r0;
			((mlib_s32 *)rgb1)[2] = r0;
			((mlib_s32 *)rgb2)[0] = r0;
			((mlib_s32 *)rgb2)[1] = r0;
			((mlib_s32 *)rgb2)[2] = r0;

			((mlib_s32 *)rgb0)[3] = r2;
			((mlib_s32 *)rgb0)[4] = r2;
			((mlib_s32 *)rgb0)[5] = r2;
			((mlib_s32 *)rgb1)[3] = r2;
			((mlib_s32 *)rgb1)[4] = r2;
			((mlib_s32 *)rgb1)[5] = r2;
			((mlib_s32 *)rgb2)[3] = r2;
			((mlib_s32 *)rgb2)[4] = r2;
			((mlib_s32 *)rgb2)[5] = r2;

			((mlib_s32 *)rgb3)[0] = r1;
			((mlib_s32 *)rgb3)[1] = r1;
			((mlib_s32 *)rgb3)[2] = r1;
			((mlib_s32 *)rgb4)[0] = r1;
			((mlib_s32 *)rgb4)[1] = r1;
			((mlib_s32 *)rgb4)[2] = r1;
			((mlib_s32 *)rgb5)[0] = r1;
			((mlib_s32 *)rgb5)[1] = r1;
			((mlib_s32 *)rgb5)[2] = r1;

			((mlib_s32 *)rgb3)[3] = r3;
			((mlib_s32 *)rgb3)[4] = r3;
			((mlib_s32 *)rgb3)[5] = r3;
			((mlib_s32 *)rgb4)[3] = r3;
			((mlib_s32 *)rgb4)[4] = r3;
			((mlib_s32 *)rgb4)[5] = r3;
			((mlib_s32 *)rgb5)[3] = r3;
			((mlib_s32 *)rgb5)[4] = r3;
			((mlib_s32 *)rgb5)[5] = r3;

			Y += 2;
			Z += 2;
			U++;
			V++;
			rgb0 += 24;
			rgb1 += 24;
			rgb2 += 24;
			rgb3 += 24;
			rgb4 += 24;
			rgb5 += 24;
		}

		if (right) {
			CALC4(Y, Z, U, V);
			((mlib_s32 *)rgb0)[jy4] = r2;
			((mlib_s32 *)rgb0)[jy3] = r2;
			((mlib_s32 *)rgb0)[jy2] = r0;
			((mlib_s32 *)rgb0)[jy1] = r0;
			((mlib_s32 *)rgb0)[0] = r0;

			((mlib_s32 *)rgb1)[jy4] = r2;
			((mlib_s32 *)rgb1)[jy3] = r2;
			((mlib_s32 *)rgb1)[jy2] = r0;
			((mlib_s32 *)rgb1)[jy1] = r0;
			((mlib_s32 *)rgb1)[0] = r0;

			((mlib_s32 *)rgb2)[jy4] = r2;
			((mlib_s32 *)rgb2)[jy3] = r2;
			((mlib_s32 *)rgb2)[jy2] = r0;
			((mlib_s32 *)rgb2)[jy1] = r0;
			((mlib_s32 *)rgb2)[0] = r0;

			((mlib_s32 *)rgb3)[jy4] = r3;
			((mlib_s32 *)rgb3)[jy3] = r3;
			((mlib_s32 *)rgb3)[jy2] = r1;
			((mlib_s32 *)rgb3)[jy1] = r1;
			((mlib_s32 *)rgb3)[0] = r1;

			((mlib_s32 *)rgb4)[jy4] = r3;
			((mlib_s32 *)rgb4)[jy3] = r3;
			((mlib_s32 *)rgb4)[jy2] = r1;
			((mlib_s32 *)rgb4)[jy1] = r1;
			((mlib_s32 *)rgb4)[0] = r1;

			((mlib_s32 *)rgb5)[jy4] = r3;
			((mlib_s32 *)rgb5)[jy3] = r3;
			((mlib_s32 *)rgb5)[jy2] = r1;
			((mlib_s32 *)rgb5)[jy1] = r1;
			((mlib_s32 *)rgb5)[0] = r1;
		}

		Y += 2 * (y_stride - i);
		Z += 2 * (y_stride - i);
		rgb0 += 6 * rgb_stride - i * 24;
		rgb1 += 6 * rgb_stride - i * 24;
		rgb2 += 6 * rgb_stride - i * 24;
		rgb3 += 6 * rgb_stride - i * 24;
		rgb4 += 6 * rgb_stride - i * 24;
		rgb5 += 6 * rgb_stride - i * 24;
		U += uv_stride - i;
		V += uv_stride - i;
	}

	if (bottom < 6 && top)
/* tail in head already */
		bottom = 0;
	else {
		bottom -= ht * 6;

		if (top)
			bottom -= 6;
	}

	if (bottom > 0) {
		if (bottom == 1)
			rgb1 = rgb2 = rgb3 = rgb4 = rgb0;
		else if (bottom == 2)
			rgb2 = rgb3 = rgb4 = rgb1;
		else if (bottom == 3)
			rgb3 = rgb4 = rgb2;
		else if (bottom == 4)
			rgb4 = rgb3;

		if ((left)) {
			CALC4I(Y[iy0], Y[iy1], Z[iy0], Z[iy1], *(U - 1),
				*(V - 1));
			((mlib_s32 *)rgb4)[i1] = r1;
			((mlib_s32 *)rgb4)[i2] = r1;
			((mlib_s32 *)rgb4)[i3] = r3;
			((mlib_s32 *)rgb4)[i4] = r3;
			((mlib_s32 *)rgb4)[i5] = r3;

			((mlib_s32 *)rgb3)[i1] = r1;
			((mlib_s32 *)rgb3)[i2] = r1;
			((mlib_s32 *)rgb3)[i3] = r3;
			((mlib_s32 *)rgb3)[i4] = r3;
			((mlib_s32 *)rgb3)[i5] = r3;

			((mlib_s32 *)rgb2)[i1] = r0;
			((mlib_s32 *)rgb2)[i2] = r0;
			((mlib_s32 *)rgb2)[i3] = r2;
			((mlib_s32 *)rgb2)[i4] = r2;
			((mlib_s32 *)rgb2)[i5] = r2;

			((mlib_s32 *)rgb1)[i1] = r0;
			((mlib_s32 *)rgb1)[i2] = r0;
			((mlib_s32 *)rgb1)[i3] = r2;
			((mlib_s32 *)rgb1)[i4] = r2;
			((mlib_s32 *)rgb1)[i5] = r2;

			((mlib_s32 *)rgb0)[i1] = r0;
			((mlib_s32 *)rgb0)[i2] = r0;
			((mlib_s32 *)rgb0)[i3] = r2;
			((mlib_s32 *)rgb0)[i4] = r2;
			((mlib_s32 *)rgb0)[i5] = r2;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(1)
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 0; i < wd; i++) {
			sf1 = U[0];
			sf2 = V[0];
			sf01 = *(mlib_u16 *)Y;
			sf02 = *(mlib_u16 *)Z;

#ifdef _LITTLE_ENDIAN

			sf03 = (mlib_u32)sf01 >> 8;
			sf01 = sf01 & 0xff;
			sf04 = (mlib_u32)sf02 >> 8;
			sf02 = sf02 & 0xff;

#else /* _LITTLE_ENDIAN */

			sf03 = sf01 & 0xff;
			sf01 = (mlib_u32)sf01 >> 8;
			sf04 = sf02 & 0xff;
			sf02 = (mlib_u32)sf02 >> 8;

#endif /* _LITTLE_ENDIAN */

			sfb = mlib_tBU[sf1] - BBi;
			sfg = mlib_tGU[sf1] - mlib_tGV[sf2];
			sfr = mlib_tRV[sf2] - RRi;

			sf01 = mlib_tYY[sf01];
			sf03 = mlib_tYY[sf03];
			sf02 = mlib_tYY[sf02];
			sf04 = mlib_tYY[sf04];

#ifdef _LITTLE_ENDIAN

			TCLAMP_U8(sf01 + sfb, r0);
			r0 <<= 8;
			TCLAMP_U8(sf01 + sfg, rr);
			r0 |= rr << 16;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr << 24;

			TCLAMP_U8(sf03 + sfb, r2);
			r2 <<= 8;
			TCLAMP_U8(sf03 + sfg, rr);
			r2 |= rr << 16;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr << 24;

			TCLAMP_U8(sf02 + sfb, r1);
			r1 <<= 8;
			TCLAMP_U8(sf02 + sfg, rr);
			r1 |= rr << 16;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr << 24;

			TCLAMP_U8(sf04 + sfb, r3);
			r3 <<= 8;
			TCLAMP_U8(sf04 + sfg, rr);
			r3 |= rr << 16;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr << 24;

#else /* _LITTLE_ENDIAN */

			TCLAMP_U8(sf01 + sfb, r0);
			r0 <<= 16;
			TCLAMP_U8(sf01 + sfg, rr);
			r0 |= rr << 8;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr;

			TCLAMP_U8(sf03 + sfb, r2);
			r2 <<= 16;
			TCLAMP_U8(sf03 + sfg, rr);
			r2 |= rr << 8;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr;

			TCLAMP_U8(sf02 + sfb, r1);
			r1 <<= 16;
			TCLAMP_U8(sf02 + sfg, rr);
			r1 |= rr << 8;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr;

			TCLAMP_U8(sf04 + sfb, r3);
			r3 <<= 16;
			TCLAMP_U8(sf04 + sfg, rr);
			r3 |= rr << 8;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr;

#endif /* _LITTLE_ENDIAN */

			((mlib_s32 *)rgb4)[0] = r1;
			((mlib_s32 *)rgb4)[1] = r1;
			((mlib_s32 *)rgb4)[2] = r1;
			((mlib_s32 *)rgb4)[3] = r3;
			((mlib_s32 *)rgb4)[4] = r3;
			((mlib_s32 *)rgb4)[5] = r3;

			((mlib_s32 *)rgb3)[0] = r1;
			((mlib_s32 *)rgb3)[1] = r1;
			((mlib_s32 *)rgb3)[2] = r1;
			((mlib_s32 *)rgb3)[3] = r3;
			((mlib_s32 *)rgb3)[4] = r3;
			((mlib_s32 *)rgb3)[5] = r3;

			((mlib_s32 *)rgb2)[0] = r0;
			((mlib_s32 *)rgb2)[1] = r0;
			((mlib_s32 *)rgb2)[2] = r0;
			((mlib_s32 *)rgb2)[3] = r2;
			((mlib_s32 *)rgb2)[4] = r2;
			((mlib_s32 *)rgb2)[5] = r2;

			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb1)[1] = r0;
			((mlib_s32 *)rgb1)[2] = r0;
			((mlib_s32 *)rgb1)[3] = r2;
			((mlib_s32 *)rgb1)[4] = r2;
			((mlib_s32 *)rgb1)[5] = r2;

			((mlib_s32 *)rgb0)[0] = r0;
			((mlib_s32 *)rgb0)[1] = r0;
			((mlib_s32 *)rgb0)[2] = r0;
			((mlib_s32 *)rgb0)[3] = r2;
			((mlib_s32 *)rgb0)[4] = r2;
			((mlib_s32 *)rgb0)[5] = r2;

			Y += 2;
			Z += 2;
			U++;
			V++;
			rgb0 += 24;
			rgb1 += 24;
			rgb2 += 24;
			rgb3 += 24;
			rgb4 += 24;
		}

		if (right) {
			CALC4(Y, Z, U, V);

			((mlib_s32 *)rgb4)[jy4] = r3;
			((mlib_s32 *)rgb4)[jy3] = r3;
			((mlib_s32 *)rgb4)[jy2] = r1;
			((mlib_s32 *)rgb4)[jy1] = r1;
			((mlib_s32 *)rgb4)[0] = r1;

			((mlib_s32 *)rgb3)[jy4] = r3;
			((mlib_s32 *)rgb3)[jy3] = r3;
			((mlib_s32 *)rgb3)[jy2] = r1;
			((mlib_s32 *)rgb3)[jy1] = r1;
			((mlib_s32 *)rgb3)[0] = r1;

			((mlib_s32 *)rgb2)[jy4] = r2;
			((mlib_s32 *)rgb2)[jy3] = r2;
			((mlib_s32 *)rgb2)[jy2] = r0;
			((mlib_s32 *)rgb2)[jy1] = r0;
			((mlib_s32 *)rgb2)[0] = r0;

			((mlib_s32 *)rgb1)[jy4] = r2;
			((mlib_s32 *)rgb1)[jy3] = r2;
			((mlib_s32 *)rgb1)[jy2] = r0;
			((mlib_s32 *)rgb1)[jy1] = r0;
			((mlib_s32 *)rgb1)[0] = r0;

			((mlib_s32 *)rgb0)[jy4] = r2;
			((mlib_s32 *)rgb0)[jy3] = r2;
			((mlib_s32 *)rgb0)[jy2] = r0;
			((mlib_s32 *)rgb0)[jy1] = r0;
			((mlib_s32 *)rgb0)[0] = r0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
