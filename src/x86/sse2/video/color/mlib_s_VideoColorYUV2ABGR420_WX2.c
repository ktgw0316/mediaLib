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

#pragma ident	"@(#)mlib_s_VideoColorYUV2ABGR420_WX2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_WX2 - color conversion-YUV420 to ABGR
 *                                       inside clipping window with zoom x2
 *
 * SYNOPSIS
 *
 * mlib_status mlib_VideoColorYUV2ABGR420_WX2(mlib_u8       *abgr,
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
 *  window borders must be: 0 <= left < right <= width*2
 *  and 0 <= top < bottom <= height*2
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR420_WX2 = \
	__mlib_VideoColorYUV2ABGR420_WX2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_WX2) mlib_VideoColorYUV2ABGR420_WX2
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_WX2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	SAVE_ABGR2(x_bgr, pd)				\
{							\
	_mm_storeu_si128((__m128i *)pd, x_bgr);		\
	pd += 16;					\
}

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
	r3 |= rr << 24;

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
	r3 |= rr;

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
	r3 |= rr << 24;

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
	r3 |= rr;

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR420_WX2(
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
	/* variables for C implementation */
	mlib_s32 i, j;
	mlib_u8 *rgb0, *rgb1, *rgb2, *rgb3;
	mlib_s32 i1, i2, i3, iy0, iy1;
	mlib_s32 jy1, jy2;
	const mlib_u8 *Y;
	const mlib_u8 *U;
	const mlib_u8 *V;
	const mlib_u8 *Z;

	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_s32 sf01, sf02, sf03, sf04, sf1, sf2, sfr, sfg, sfb;
	mlib_s32 r0, r1, r2, r3, rr;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	/* variables for SSE2 implementation */
/* 1.1644  * 8192 */
	const __m128i x_c0 = _mm_set1_epi16(0x2543);
	const mlib_s32 ic0 = 0x2543;

/* 2.0184  * 8192 */
	const __m128i x_c1 = _mm_set1_epi16(0x4097);
	const mlib_s32 ic1 = 0x4097;

/* abs( -0.3920 * 8192 ) */
	const __m128i x_c4 = _mm_set1_epi16(0xc8b);
	const mlib_s32 ic4 = 0xc8b;

/* abs( -0.8132 * 8192 ) */
	const __m128i x_c5 = _mm_set1_epi16(0x1a06);
	const mlib_s32 ic5 = 0x1a06;

/* 1.5966  * 8192 */
	const __m128i x_c8 = _mm_set1_epi16(0x3317);
	const mlib_s32 ic8 = 0x3317;

/* -276.9856 * 32 */
	const __m128i x_coff0 = _mm_set1_epi16(0xdd60);
	const mlib_s32 icoff0 = (mlib_s32)0xffffdd60;

/* 135.6352  * 32 */
	const __m128i x_coff1 = _mm_set1_epi16(0x10f4);
	const mlib_s32 icoff1 = 0x10f4;

/* -222.9952 * 32 */
	const __m128i x_coff2 = _mm_set1_epi16(0xe420);
	const mlib_s32 icoff2 = (mlib_s32)0xffffe420;

	const __m128i x_zero = _mm_setzero_si128();

	mlib_s32 iW;
	mlib_s32 iu, iv, ig, ir, ib, iTemp;
	mlib_s32 iu0, iu1, iv1, iv2;
	__m128i x_u0, x_u1, x_v1, x_v2, x_temp, x_out, x_bak, x_temp1;
	__m128i x_u, x_v, x_y1, x_y2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_r3, x_r4, x_g1, x_g2, x_g3, x_g4;
	__m128i x_b1, x_b2, x_b3, x_b4, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_bgrh, x_bgrl;
	__m128i *px_y1, *px_y2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pd1, *pd2, *pd3, *pd4;

	/* pure C implementation */
	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if (wd & 7 || ht & 1 || rgb_stride & 7 || uv_stride & 3 || y_stride & 7)
		return (MLIB_FAILURE);

	if (left < 0 || top < 0 || right <= left || bottom <= top ||
		2 * wd < right || 2 * ht < bottom)
		return (MLIB_FAILURE);

	abgr += ((left + 3) & ~3) * 4 + ((top + 3) & ~3) * rgb_stride;
	y_inp += (((left + 3) & ~3) >> 1) + (((top + 3) & ~3) >> 1) * y_stride;
	u_inp += ((left + 3) >> 2) + ((top + 3) >> 2) * uv_stride;
	v_inp += ((left + 3) >> 2) + ((top + 3) >> 2) * uv_stride;

	Y = y_inp;
	U = u_inp;
	V = v_inp;
	Z = y_inp + y_stride;

	right -= left & ~3;
	left &= 3;
	bottom -= top & ~3;
	top &= 3;

	wd = (right) >> 2;
	ht = (bottom) >> 2;
	rgb0 = abgr;
	rgb1 = rgb0 + rgb_stride;
	rgb2 = rgb1 + rgb_stride;
	rgb3 = rgb2 + rgb_stride;

	i1 = -3;
	i2 = -2;
	i3 = -1;

	if (left) {
		wd--;

		if (right == 2) {
			iy0 = -1;
			iy1 = -2;
			i2 = i3 = i1;
		} else {
			iy0 = -2;
			iy1 = -1;

			if (right == 3)
				i3 = i2;

			if (left == 2)
				i1 = i2;
			else if (left == 3)
				i1 = i2 = i3;
		}
	}

	if (right < 4 && left)
/* tail in head already */
		right = 0;
	else {
		if ((right & 3) == 1)
			jy1 = jy2 = 0;
		else if ((right & 3) == 2)
			jy1 = jy2 = 1;
		else {
			jy1 = 1;
			jy2 = 2;
		}
	}

	if (top) {
		const mlib_u8 *Y_, *Z_;

		ht--;
		rgb1 -= 4 * rgb_stride;
		rgb2 -= 4 * rgb_stride;
		rgb3 -= 4 * rgb_stride;

		if (bottom == 2) {
			Y_ = Y - y_stride;
			Z_ = Y - 2 * y_stride;
			rgb2 = rgb3 = rgb1;
		} else {
			Y_ = Y - 2 * y_stride;
			Z_ = Y - y_stride;

			if (bottom == 3)
				rgb3 = rgb2;

			if (top == 2)
				rgb1 = rgb2;
			else if (top == 3)
				rgb1 = rgb2 = rgb3;
		}

		if (left) {
			CALC4I(Y_[iy0], Y_[iy1], Z_[iy0], Z_[iy1],
				*(U - uv_stride - 1), *(V - uv_stride - 1));
			((mlib_s32 *)rgb1)[i1] = r0;
			((mlib_s32 *)rgb1)[i2] = r2;
			((mlib_s32 *)rgb1)[i3] = r2;
			((mlib_s32 *)rgb2)[i1] = r1;
			((mlib_s32 *)rgb2)[i2] = r3;
			((mlib_s32 *)rgb2)[i3] = r3;
			((mlib_s32 *)rgb3)[i1] = r1;
			((mlib_s32 *)rgb3)[i2] = r3;
			((mlib_s32 *)rgb3)[i3] = r3;
		}

		for (i = 0; i < wd; i++) {
			CALC4(Y_, Z_, (U - uv_stride + i), (V - uv_stride + i));
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb1)[1] = r0;
			((mlib_s32 *)rgb1)[2] = r2;
			((mlib_s32 *)rgb1)[3] = r2;
			((mlib_s32 *)rgb2)[0] = r1;
			((mlib_s32 *)rgb2)[1] = r1;
			((mlib_s32 *)rgb2)[2] = r3;
			((mlib_s32 *)rgb2)[3] = r3;
			((mlib_s32 *)rgb3)[0] = r1;
			((mlib_s32 *)rgb3)[1] = r1;
			((mlib_s32 *)rgb3)[2] = r3;
			((mlib_s32 *)rgb3)[3] = r3;
			Y_ += 2;
			Z_ += 2;
			rgb1 += 16;
			rgb2 += 16;
			rgb3 += 16;
		}

		if ((right & 3)) {
			CALC4(Y_, Z_, (U - uv_stride + wd),
				(V - uv_stride + wd));
			((mlib_s32 *)rgb1)[jy2] = r2;
			((mlib_s32 *)rgb1)[jy1] = r0;
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb2)[jy2] = r3;
			((mlib_s32 *)rgb2)[jy1] = r1;
			((mlib_s32 *)rgb2)[0] = r1;
			((mlib_s32 *)rgb3)[jy2] = r3;
			((mlib_s32 *)rgb3)[jy1] = r1;
			((mlib_s32 *)rgb3)[0] = r1;
		}

		rgb1 = rgb0 + rgb_stride;
		rgb2 = rgb1 + rgb_stride;
		rgb3 = rgb2 + rgb_stride;
	}

	for (j = 0; j < ht; j++) {

		if ((left)) {
			CALC4I(Y[iy0], Y[iy1], Z[iy0], Z[iy1], *(U - 1),
				*(V - 1));
			((mlib_s32 *)rgb0)[i1] = r0;
			((mlib_s32 *)rgb0)[i2] = r2;
			((mlib_s32 *)rgb0)[i3] = r2;
			((mlib_s32 *)rgb1)[i1] = r0;
			((mlib_s32 *)rgb1)[i2] = r2;
			((mlib_s32 *)rgb1)[i3] = r2;
			((mlib_s32 *)rgb2)[i1] = r1;
			((mlib_s32 *)rgb2)[i2] = r3;
			((mlib_s32 *)rgb2)[i3] = r3;
			((mlib_s32 *)rgb3)[i1] = r1;
			((mlib_s32 *)rgb3)[i2] = r3;
			((mlib_s32 *)rgb3)[i3] = r3;
		}

		/* SSE2 implementation */
		px_y1 = (__m128i*)Y;
		px_y2 = (__m128i*)Z;
		pm_u = (__m64*)U;
		pm_v = (__m64*)V;
		pd1 = rgb0;
		pd2 = rgb1;
		pd3 = rgb2;
		pd4 = rgb3;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; iW <= wd - 8; iW += 8) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_loadu_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_loadu_si128(px_y2);
			px_y2++;
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_out);
			x_out = _mm_unpackhi_epi16(x_temp, x_temp1);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_b[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b4 = _mm_add_epi16(x_y4, x_temp);

			/* x_g[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g4 = _mm_add_epi16(x_y4, x_temp);

			/* x_r[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r4 = _mm_add_epi16(x_y4, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_b2 = _mm_srai_epi16(x_b2, 5);
			x_b3 = _mm_srai_epi16(x_b3, 5);
			x_b4 = _mm_srai_epi16(x_b4, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_g2 = _mm_srai_epi16(x_g2, 5);
			x_g3 = _mm_srai_epi16(x_g3, 5);
			x_g4 = _mm_srai_epi16(x_g4, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);
			x_r2 = _mm_srai_epi16(x_r2, 5);
			x_r3 = _mm_srai_epi16(x_r3, 5);
			x_r4 = _mm_srai_epi16(x_r4, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			/* create bgr sequences : x_[b1/r1/g1] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b1);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b1);
			x_grl = _mm_unpacklo_epi8(x_g1, x_r1);
			x_grh = _mm_unpackhi_epi8(x_g1, x_r1);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
		}

		if (iW <= wd - 4) {
			iTemp = *((mlib_s32 *)pm_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u = (__m64 *) (((mlib_s32 *)pm_u) + 1);
			iTemp = *((mlib_s32 *)pm_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v = (__m64 *) (((mlib_s32 *)pm_v) + 1);
			x_temp = _mm_loadl_epi64(px_y1);
			px_y1 = (__m128i *) (((__m64 *)px_y1) + 1);
			x_temp1 = _mm_loadl_epi64(px_y2);
			px_y2 = (__m128i *) (((__m64 *)px_y2) + 1);
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);

			/* x_b[1/2] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b2 = _mm_add_epi16(x_y2, x_temp);

			/* x_g[1/2] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g2 = _mm_add_epi16(x_y2, x_temp);

			/* x_r[1/2] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r2 = _mm_add_epi16(x_y2, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_b2 = _mm_srai_epi16(x_b2, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_g2 = _mm_srai_epi16(x_g2, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);
			x_r2 = _mm_srai_epi16(x_r2, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_g = _mm_packus_epi16(x_g1, x_g2);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			iW += 4;
		}

		if (iW <= wd - 2) {
			iTemp = *((mlib_s16 *)pm_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u = (__m64 *) (((mlib_s16 *)pm_u) + 1);
			iTemp = *((mlib_s16 *)pm_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v = (__m64 *) (((mlib_s16 *)pm_v) + 1);

			iTemp = *((mlib_s32 *)px_y1);
			px_y1 = (__m128i *) (((mlib_s32 *)px_y1) + 1);
			x_temp = _mm_cvtsi32_si128(iTemp);
			iTemp = *((mlib_s32 *)px_y2);
			px_y2 = (__m128i *) (((mlib_s32 *)px_y2) + 1);
			x_temp1 = _mm_cvtsi32_si128(iTemp);
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][1] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);

			/* x_b[1] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);

			/* x_g[1] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* x_r[1] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_zero);
			x_r = _mm_packus_epi16(x_r1, x_zero);
			x_g = _mm_packus_epi16(x_g1, x_zero);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			iW += 2;
		}

		if (iW <= wd - 1) {
			/* load y u v, and expand */
			iu = *((mlib_u8 *)pm_u);
			pm_u = (__m64 *) (((mlib_u8 *)pm_u) + 1);
			iv = *((mlib_u8 *)pm_v);
			pm_v = (__m64 *) (((mlib_u8 *)pm_v) + 1);
			iTemp = (*((mlib_s16 *)px_y1)  & 0xffff) |
				(*((mlib_s16 *)px_y2)<<16);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y1 = (__m128i *) (((mlib_s16 *)px_y1) + 1);
			px_y2 = (__m128i *) (((mlib_s16 *)px_y2) + 1);

			/* pre-calc d[r/g/b][1] */
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0;

			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp;

			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2;

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);

			/* db1 */
			x_temp = _mm_set1_epi16(ib);
			x_b1 = _mm_add_epi16(x_y1, x_temp);

			/* dg1 */
			x_temp = _mm_set1_epi16(ig);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* dr1 */
			x_temp = _mm_set1_epi16(ir);
			x_r1 = _mm_add_epi16(x_y1, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_zero);
			x_r = _mm_packus_epi16(x_r1, x_zero);
			x_g = _mm_packus_epi16(x_g1, x_zero);

			/* create bgr sequences : db/dr/dg */
			/* bgr0 bgr0 bgr0 bgr0 */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			/* lower half of zbl & grl */
			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_zero);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_zero);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			iW += 1;
		}

		/* preparing for pure C implementation */
		if (wd > 0) {
			Y += wd << 1;
			Z += wd << 1;
			U += wd;
			V += wd;
			rgb0 += wd << 4;
			rgb1 += wd << 4;
			rgb2 += wd << 4;
			rgb3 += wd << 4;
			i = wd;
		} else {
			i = 0;
		}

		if ((right & 3)) {
			CALC4(Y, Z, U, V)
				((mlib_s32 *)rgb0)[jy2] = r2;
			((mlib_s32 *)rgb0)[jy1] = r0;
			((mlib_s32 *)rgb0)[0] = r0;
			((mlib_s32 *)rgb1)[jy2] = r2;
			((mlib_s32 *)rgb1)[jy1] = r0;
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb2)[jy2] = r3;
			((mlib_s32 *)rgb2)[jy1] = r1;
			((mlib_s32 *)rgb2)[0] = r1;
			((mlib_s32 *)rgb3)[jy2] = r3;
			((mlib_s32 *)rgb3)[jy1] = r1;
			((mlib_s32 *)rgb3)[0] = r1;
		}

		Y += 2 * (y_stride - i);
		Z += 2 * (y_stride - i);
		rgb0 += 4 * rgb_stride - i * 16;
		rgb1 += 4 * rgb_stride - i * 16;
		rgb2 += 4 * rgb_stride - i * 16;
		rgb3 += 4 * rgb_stride - i * 16;
		U += uv_stride - i;
		V += uv_stride - i;
	}

	if ((bottom & 3) && (bottom > 4 || top == 0)) {
		if ((bottom & 3) == 1)
			rgb1 = rgb2 = rgb0;
		else if ((bottom & 3) == 2)
			rgb2 = rgb1;

		if ((left)) {
			CALC4I(Y[iy0], Y[iy1], Z[iy0], Z[iy1], *(U - 1),
				*(V - 1));
			((mlib_s32 *)rgb2)[i1] = r1;
			((mlib_s32 *)rgb2)[i2] = r3;
			((mlib_s32 *)rgb2)[i3] = r3;
			((mlib_s32 *)rgb1)[i1] = r0;
			((mlib_s32 *)rgb1)[i2] = r2;
			((mlib_s32 *)rgb1)[i3] = r2;
			((mlib_s32 *)rgb0)[i1] = r0;
			((mlib_s32 *)rgb0)[i2] = r2;
			((mlib_s32 *)rgb0)[i3] = r2;
		}

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

			((mlib_s32 *)rgb2)[0] = r1;
			((mlib_s32 *)rgb2)[1] = r1;
			((mlib_s32 *)rgb2)[2] = r3;
			((mlib_s32 *)rgb2)[3] = r3;
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb1)[1] = r0;
			((mlib_s32 *)rgb1)[2] = r2;
			((mlib_s32 *)rgb1)[3] = r2;
			((mlib_s32 *)rgb0)[0] = r0;
			((mlib_s32 *)rgb0)[1] = r0;
			((mlib_s32 *)rgb0)[2] = r2;
			((mlib_s32 *)rgb0)[3] = r2;

			Y += 2;
			Z += 2;
			U++;
			V++;
			rgb0 += 16;
			rgb1 += 16;
			rgb2 += 16;
		}

		if ((right & 3)) {
			CALC4(Y, Z, U, V);
			((mlib_s32 *)rgb2)[jy2] = r3;
			((mlib_s32 *)rgb2)[jy1] = r1;
			((mlib_s32 *)rgb2)[0] = r1;
			((mlib_s32 *)rgb1)[jy2] = r2;
			((mlib_s32 *)rgb1)[jy1] = r0;
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb0)[jy2] = r2;
			((mlib_s32 *)rgb0)[jy1] = r0;
			((mlib_s32 *)rgb0)[0] = r0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
