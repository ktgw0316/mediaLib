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

#pragma ident	"@(#)mlib_s_VideoColorAVDYUV411.c	9.3	%E SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_ARGBint - color convert YUV sequential
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV411seq_to_ARGBint(mlib_u32      *argb,
 *                                              const mlib_u8 *y,
 *                                              const mlib_u8 *u,
 *                                              const mlib_u8 *v,
 *                                              const mlib_u8 *a,
 *                                              mlib_u8       alpha,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      ya_stride,
 *                                              mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      ya_stride   linebytes for Y and alpha buffers
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The Y, U, V pixel streams are converted into a ARGB pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row, every
 *      4 Y values use the same U and V values.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_ABGRint - color convert YUV sequential
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV411seq_to_ABGRint(mlib_u32      *abgr,
 *                                              const mlib_u8 *y,
 *                                              const mlib_u8 *u,
 *                                              const mlib_u8 *v,
 *                                              const mlib_u8 *a,
 *                                              mlib_u8       alpha,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      ya_stride,
 *                                              mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      ya_stride   linebytes for Y and alpha buffers
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The Y, U, V pixel streams are converted into a ABGR pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row, every
 *      4 Y values use the same U and V values.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
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

#pragma weak mlib_VideoColorYUV411seq_to_ABGRint = \
	__mlib_VideoColorYUV411seq_to_ABGRint
#pragma weak mlib_VideoColorYUV411seq_to_ARGBint = \
	__mlib_VideoColorYUV411seq_to_ARGBint

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV411seq_to_ABGRint)
	mlib_VideoColorYUV411seq_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorYUV411seq_to_ABGRint")));
__typeof__(__mlib_VideoColorYUV411seq_to_ARGBint)
	mlib_VideoColorYUV411seq_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorYUV411seq_to_ARGBint")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(dst, val)	dst = pClip[val]

/* *********************************************************** */

#define	SCLAMP_U8(dst, s)                                                 \
	{                                                                 \
	    mlib_s32 v = s, mask = (v - 0x1fe) >> 31;                     \
	                                                                  \
	    dst = (mlib_u8)((((mlib_u32)v >> 1) | ~mask) & ~(v >> 31));   \
	}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 aylb,
	mlib_s32 uvlb)
{
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

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp, x_out, x_bak, x_temp1;
	__m128i x_temp2, x_temp3;
	__m128i x_u, x_v, x_y1, x_y2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_r3, x_r4, x_g1, x_g2, x_g3, x_g4;
	__m128i x_b1, x_b2, x_b3, x_b4, x_r, x_g, x_b;
	__m128i *px_y1, *px_y2;
	__m128i *p_a1, *p_a2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pY1, *pY2, *pU, *pV, *pD1, *pD2, *pd1, *pd2;
	mlib_u8 *p_A1, *p_A2;
	pY1 = (mlib_u8*)y;

	dlb >>= 2;
	w >>= 2;

	pY2 = (mlib_u8*)y + 16;
	pU = (mlib_u8*)u;
	pV = (mlib_u8*)v;
	pD1 = (mlib_u8*)abgr;
	p_A1 = (mlib_u8 *)a_array;

	mlib_s32 i, j;
	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;


	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += aylb, u += uvlb, v += uvlb, abgr +=
			dlb, a_array += aylb) {
		px_y1 = (__m128i*)pY1;
		p_a1 = (__m128i *)p_A1;
		p_a2 = (__m128i *)p_A2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (j = 0; j <= w - 8; j += 8) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_loadu_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_loadu_si128(px_y1);
			px_y1++;
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_temp);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_temp1);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_temp1);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);
			x_b4 = _mm_unpackhi_epi16(x_b, x_b);
			x_b = _mm_unpacklo_epi16(x_b, x_b);
			x_b1 = _mm_unpacklo_epi32(x_b, x_b);
			x_b2 = _mm_unpackhi_epi32(x_b, x_b);
			x_b3 = _mm_unpacklo_epi32(x_b4, x_b4);
			x_b4 = _mm_unpackhi_epi32(x_b4, x_b4);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);
			x_g4 = _mm_unpackhi_epi16(x_g, x_g);
			x_g = _mm_unpacklo_epi16(x_g, x_g);
			x_g1 = _mm_unpacklo_epi32(x_g, x_g);
			x_g2 = _mm_unpackhi_epi32(x_g, x_g);
			x_g3 = _mm_unpacklo_epi32(x_g4, x_g4);
			x_g4 = _mm_unpackhi_epi32(x_g4, x_g4);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);
			x_r4 = _mm_unpackhi_epi16(x_r, x_r);
			x_r = _mm_unpacklo_epi16(x_r, x_r);
			x_r1 = _mm_unpacklo_epi32(x_r, x_r);
			x_r2 = _mm_unpackhi_epi32(x_r, x_r);
			x_r3 = _mm_unpacklo_epi32(x_r4, x_r4);
			x_r4 = _mm_unpackhi_epi32(x_r4, x_r4);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_g */
			x_g1 = _mm_add_epi16(x_g1, x_y1);
			x_g2 = _mm_add_epi16(x_g2, x_y2);
			x_g3 = _mm_add_epi16(x_g3, x_y3);
			x_g4 = _mm_add_epi16(x_g4, x_y4);

			/* x_b */
			x_b1 = _mm_add_epi16(x_b1, x_y1);
			x_b2 = _mm_add_epi16(x_b2, x_y2);
			x_b3 = _mm_add_epi16(x_b3, x_y3);
			x_b4 = _mm_add_epi16(x_b4, x_y4);
			/* x_r */
			x_r1 = _mm_add_epi16(x_r1, x_y1);
			x_r2 = _mm_add_epi16(x_r2, x_y2);
			x_r3 = _mm_add_epi16(x_r3, x_y3);
			x_r4 = _mm_add_epi16(x_r4, x_y4);

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

			/* sequential b */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);

			/* sequential g */
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);

			/* sequential r */
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);

			/* sequence gb */
			x_g2 = _mm_unpacklo_epi8(x_g, x_r);
			x_g3 = _mm_unpackhi_epi8(x_g, x_r);
			x_r2 = _mm_unpacklo_epi8(x_g1, x_r1);
			x_r3 = _mm_unpackhi_epi8(x_g1, x_r1);

			/* sequence ar */
			__m128i x_a1 = _mm_loadu_si128(p_a1++);
			__m128i x_a2 = _mm_loadu_si128(p_a1++);
			x_temp = _mm_unpacklo_epi8(x_a1, x_b);
			x_temp1 = _mm_unpackhi_epi8(x_a1, x_b);
			x_temp2 = _mm_unpacklo_epi8(x_a2, x_b1);
			x_temp3 = _mm_unpackhi_epi8(x_a2, x_b1);

			/* sequence argbargb */
			x_b = _mm_unpacklo_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;

			x_b = _mm_unpacklo_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;
		}
			for (; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd = (mlib_u8 *)(abgr + 4 * j);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[4 * j];
				val_y1 = y[4 * j + 1];
				val_y2 = y[4 * j + 2];
				val_y3 = y[4 * j + 3];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd[0] = a_array[4 * j];
				TCLAMP_U8(pd[1], val_y0 + val_b);
				TCLAMP_U8(pd[2], val_y0 + val_g);
				SCLAMP_U8(pd[3], val_y0 + val_r);
				pd[4] = a_array[4 * j + 1];
				TCLAMP_U8(pd[5], val_y1 + val_b);
				TCLAMP_U8(pd[6], val_y1 + val_g);
				SCLAMP_U8(pd[7], val_y1 + val_r);
				pd[8] = a_array[4 * j + 2];
				TCLAMP_U8(pd[9], val_y2 + val_b);
				TCLAMP_U8(pd[10], val_y2 + val_g);
				SCLAMP_U8(pd[11], val_y2 + val_r);
				pd[12] = a_array[4 * j + 3];
				TCLAMP_U8(pd[13], val_y3 + val_b);
				TCLAMP_U8(pd[14], val_y3 + val_g);
				SCLAMP_U8(pd[15], val_y3 + val_r);
			}
		pY1 += (aylb);
		p_A1 += (aylb);
		pU += uvlb;
		pV += uvlb;
		pD1 += (4 * dlb);
		}
	} else {
		__m128i x_a = _mm_set1_epi8((char)a_const);
		for (i = 0; i < h;
			i++, y += aylb, u += uvlb, v += uvlb,
			abgr += dlb) {
		px_y1 = (__m128i*)pY1;
		p_a1 = (__m128i *)p_A1;
		p_a2 = (__m128i *)p_A2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (j = 0; j <= w - 8; j += 8) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_loadu_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_loadu_si128(px_y1);
			px_y1++;
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_temp);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_temp1);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_temp1);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);
			x_b4 = _mm_unpackhi_epi16(x_b, x_b);
			x_b = _mm_unpacklo_epi16(x_b, x_b);
			x_b1 = _mm_unpacklo_epi32(x_b, x_b);
			x_b2 = _mm_unpackhi_epi32(x_b, x_b);
			x_b3 = _mm_unpacklo_epi32(x_b4, x_b4);
			x_b4 = _mm_unpackhi_epi32(x_b4, x_b4);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);
			x_g4 = _mm_unpackhi_epi16(x_g, x_g);
			x_g = _mm_unpacklo_epi16(x_g, x_g);
			x_g1 = _mm_unpacklo_epi32(x_g, x_g);
			x_g2 = _mm_unpackhi_epi32(x_g, x_g);
			x_g3 = _mm_unpacklo_epi32(x_g4, x_g4);
			x_g4 = _mm_unpackhi_epi32(x_g4, x_g4);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);
			x_r4 = _mm_unpackhi_epi16(x_r, x_r);
			x_r = _mm_unpacklo_epi16(x_r, x_r);
			x_r1 = _mm_unpacklo_epi32(x_r, x_r);
			x_r2 = _mm_unpackhi_epi32(x_r, x_r);
			x_r3 = _mm_unpacklo_epi32(x_r4, x_r4);
			x_r4 = _mm_unpackhi_epi32(x_r4, x_r4);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_g */
			x_g1 = _mm_add_epi16(x_g1, x_y1);
			x_g2 = _mm_add_epi16(x_g2, x_y2);
			x_g3 = _mm_add_epi16(x_g3, x_y3);
			x_g4 = _mm_add_epi16(x_g4, x_y4);

			/* x_b */
			x_b1 = _mm_add_epi16(x_b1, x_y1);
			x_b2 = _mm_add_epi16(x_b2, x_y2);
			x_b3 = _mm_add_epi16(x_b3, x_y3);
			x_b4 = _mm_add_epi16(x_b4, x_y4);
			/* x_r */
			x_r1 = _mm_add_epi16(x_r1, x_y1);
			x_r2 = _mm_add_epi16(x_r2, x_y2);
			x_r3 = _mm_add_epi16(x_r3, x_y3);
			x_r4 = _mm_add_epi16(x_r4, x_y4);

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

			/* sequential b */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);

			/* sequential g */
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);

			/* sequential r */
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);

			/* sequence gb */
			x_g2 = _mm_unpacklo_epi8(x_g, x_r);
			x_g3 = _mm_unpackhi_epi8(x_g, x_r);
			x_r2 = _mm_unpacklo_epi8(x_g1, x_r1);
			x_r3 = _mm_unpackhi_epi8(x_g1, x_r1);

			/* sequence ar */
			x_temp = _mm_unpacklo_epi8(x_a, x_b);
			x_temp1 = _mm_unpackhi_epi8(x_a, x_b);
			x_temp2 = _mm_unpacklo_epi8(x_a, x_b1);
			x_temp3 = _mm_unpackhi_epi8(x_a, x_b1);

			/* sequence argbargb */
			x_b = _mm_unpacklo_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;

			x_b = _mm_unpacklo_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;
		}
			for (; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd = (mlib_u8 *)(abgr + 4 * j);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[4 * j];
				val_y1 = y[4 * j + 1];
				val_y2 = y[4 * j + 2];
				val_y3 = y[4 * j + 3];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd[0] = a_const;
				TCLAMP_U8(pd[1], val_y0 + val_b);
				TCLAMP_U8(pd[2], val_y0 + val_g);
				SCLAMP_U8(pd[3], val_y0 + val_r);
				pd[4] = a_const;
				TCLAMP_U8(pd[5], val_y1 + val_b);
				TCLAMP_U8(pd[6], val_y1 + val_g);
				SCLAMP_U8(pd[7], val_y1 + val_r);
				pd[8] = a_const;
				TCLAMP_U8(pd[9], val_y2 + val_b);
				TCLAMP_U8(pd[10], val_y2 + val_g);
				SCLAMP_U8(pd[11], val_y2 + val_r);
				pd[12] = a_const;
				TCLAMP_U8(pd[13], val_y3 + val_b);
				TCLAMP_U8(pd[14], val_y3 + val_g);
				SCLAMP_U8(pd[15], val_y3 + val_r);
			}
		pY1 += (aylb);
		p_A1 += (aylb);
		pU += uvlb;
		pV += uvlb;
		pD1 += (4 * dlb);
	}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 aylb,
	mlib_s32 uvlb)
{
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

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp, x_out, x_bak, x_temp1;
	__m128i x_temp2, x_temp3;
	__m128i x_u, x_v, x_y1, x_y2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_r3, x_r4, x_g1, x_g2, x_g3, x_g4;
	__m128i x_b1, x_b2, x_b3, x_b4, x_r, x_g, x_b;
	__m128i *px_y1, *px_y2;
	__m128i *p_a1, *p_a2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pY1, *pY2, *pU, *pV, *pD1, *pD2, *pd1, *pd2;
	mlib_u8 *p_A1, *p_A2;
	pY1 = (mlib_u8*)y;

	dlb >>= 2;
	w >>= 2;

	pY2 = (mlib_u8*)y + 16;
	pU = (mlib_u8*)u;
	pV = (mlib_u8*)v;
	pD1 = (mlib_u8*)argb;
	p_A1 = (mlib_u8 *)a_array;

	mlib_s32 i, j;
	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;


	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += aylb, u += uvlb, v += uvlb, argb +=
			dlb, a_array += aylb) {
		px_y1 = (__m128i*)pY1;
		p_a1 = (__m128i *)p_A1;
		p_a2 = (__m128i *)p_A2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (j = 0; j <= w - 8; j += 8) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_loadu_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_loadu_si128(px_y1);
			px_y1++;
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_temp);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_temp1);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_temp1);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);
			x_b4 = _mm_unpackhi_epi16(x_b, x_b);
			x_b = _mm_unpacklo_epi16(x_b, x_b);
			x_b1 = _mm_unpacklo_epi32(x_b, x_b);
			x_b2 = _mm_unpackhi_epi32(x_b, x_b);
			x_b3 = _mm_unpacklo_epi32(x_b4, x_b4);
			x_b4 = _mm_unpackhi_epi32(x_b4, x_b4);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);
			x_g4 = _mm_unpackhi_epi16(x_g, x_g);
			x_g = _mm_unpacklo_epi16(x_g, x_g);
			x_g1 = _mm_unpacklo_epi32(x_g, x_g);
			x_g2 = _mm_unpackhi_epi32(x_g, x_g);
			x_g3 = _mm_unpacklo_epi32(x_g4, x_g4);
			x_g4 = _mm_unpackhi_epi32(x_g4, x_g4);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);
			x_r4 = _mm_unpackhi_epi16(x_r, x_r);
			x_r = _mm_unpacklo_epi16(x_r, x_r);
			x_r1 = _mm_unpacklo_epi32(x_r, x_r);
			x_r2 = _mm_unpackhi_epi32(x_r, x_r);
			x_r3 = _mm_unpacklo_epi32(x_r4, x_r4);
			x_r4 = _mm_unpackhi_epi32(x_r4, x_r4);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_g */
			x_g1 = _mm_add_epi16(x_g1, x_y1);
			x_g2 = _mm_add_epi16(x_g2, x_y2);
			x_g3 = _mm_add_epi16(x_g3, x_y3);
			x_g4 = _mm_add_epi16(x_g4, x_y4);

			/* x_b */
			x_b1 = _mm_add_epi16(x_b1, x_y1);
			x_b2 = _mm_add_epi16(x_b2, x_y2);
			x_b3 = _mm_add_epi16(x_b3, x_y3);
			x_b4 = _mm_add_epi16(x_b4, x_y4);
			/* x_r */
			x_r1 = _mm_add_epi16(x_r1, x_y1);
			x_r2 = _mm_add_epi16(x_r2, x_y2);
			x_r3 = _mm_add_epi16(x_r3, x_y3);
			x_r4 = _mm_add_epi16(x_r4, x_y4);

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

			/* sequential b */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);

			/* sequential g */
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);

			/* sequential r */
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);

			/* sequence gb */
			x_g2 = _mm_unpacklo_epi8(x_g, x_b);
			x_g3 = _mm_unpackhi_epi8(x_g, x_b);
			x_r2 = _mm_unpacklo_epi8(x_g1, x_b1);
			x_r3 = _mm_unpackhi_epi8(x_g1, x_b1);

			/* sequence ar */
			__m128i x_a1 = _mm_loadu_si128(p_a1++);
			__m128i x_a2 = _mm_loadu_si128(p_a1++);
			x_temp = _mm_unpacklo_epi8(x_a1, x_r);
			x_temp1 = _mm_unpackhi_epi8(x_a1, x_r);
			x_temp2 = _mm_unpacklo_epi8(x_a2, x_r1);
			x_temp3 = _mm_unpackhi_epi8(x_a2, x_r1);

			/* sequence argbargb */
			x_b = _mm_unpacklo_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;

			x_b = _mm_unpacklo_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;
		}
			for (; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd = (mlib_u8 *)(argb + 4 * j);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[4 * j];
				val_y1 = y[4 * j + 1];
				val_y2 = y[4 * j + 2];
				val_y3 = y[4 * j + 3];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd[0] = a_array[4 * j];
				TCLAMP_U8(pd[3], val_y0 + val_b);
				TCLAMP_U8(pd[2], val_y0 + val_g);
				SCLAMP_U8(pd[1], val_y0 + val_r);
				pd[4] = a_array[4 * j + 1];
				TCLAMP_U8(pd[7], val_y1 + val_b);
				TCLAMP_U8(pd[6], val_y1 + val_g);
				SCLAMP_U8(pd[5], val_y1 + val_r);
				pd[8] = a_array[4 * j + 2];
				TCLAMP_U8(pd[11], val_y2 + val_b);
				TCLAMP_U8(pd[10], val_y2 + val_g);
				SCLAMP_U8(pd[9], val_y2 + val_r);
				pd[12] = a_array[4 * j + 3];
				TCLAMP_U8(pd[15], val_y3 + val_b);
				TCLAMP_U8(pd[14], val_y3 + val_g);
				SCLAMP_U8(pd[13], val_y3 + val_r);
			}
		pY1 += (aylb);
		p_A1 += (aylb);
		pU += uvlb;
		pV += uvlb;
		pD1 += (4 * dlb);
		}
	} else {
		__m128i x_a = _mm_set1_epi8((char)a_const);
		for (i = 0; i < h;
			i++, y += aylb, u += uvlb, v += uvlb,
			argb += dlb) {
		px_y1 = (__m128i*)pY1;
		p_a1 = (__m128i *)p_A1;
		p_a2 = (__m128i *)p_A2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (j = 0; j <= w - 8; j += 8) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_loadu_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_loadu_si128(px_y1);
			px_y1++;
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_temp);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_temp1);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_temp1);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);
			x_b4 = _mm_unpackhi_epi16(x_b, x_b);
			x_b = _mm_unpacklo_epi16(x_b, x_b);
			x_b1 = _mm_unpacklo_epi32(x_b, x_b);
			x_b2 = _mm_unpackhi_epi32(x_b, x_b);
			x_b3 = _mm_unpacklo_epi32(x_b4, x_b4);
			x_b4 = _mm_unpackhi_epi32(x_b4, x_b4);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);
			x_g4 = _mm_unpackhi_epi16(x_g, x_g);
			x_g = _mm_unpacklo_epi16(x_g, x_g);
			x_g1 = _mm_unpacklo_epi32(x_g, x_g);
			x_g2 = _mm_unpackhi_epi32(x_g, x_g);
			x_g3 = _mm_unpacklo_epi32(x_g4, x_g4);
			x_g4 = _mm_unpackhi_epi32(x_g4, x_g4);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);
			x_r4 = _mm_unpackhi_epi16(x_r, x_r);
			x_r = _mm_unpacklo_epi16(x_r, x_r);
			x_r1 = _mm_unpacklo_epi32(x_r, x_r);
			x_r2 = _mm_unpackhi_epi32(x_r, x_r);
			x_r3 = _mm_unpacklo_epi32(x_r4, x_r4);
			x_r4 = _mm_unpackhi_epi32(x_r4, x_r4);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_g */
			x_g1 = _mm_add_epi16(x_g1, x_y1);
			x_g2 = _mm_add_epi16(x_g2, x_y2);
			x_g3 = _mm_add_epi16(x_g3, x_y3);
			x_g4 = _mm_add_epi16(x_g4, x_y4);

			/* x_b */
			x_b1 = _mm_add_epi16(x_b1, x_y1);
			x_b2 = _mm_add_epi16(x_b2, x_y2);
			x_b3 = _mm_add_epi16(x_b3, x_y3);
			x_b4 = _mm_add_epi16(x_b4, x_y4);
			/* x_r */
			x_r1 = _mm_add_epi16(x_r1, x_y1);
			x_r2 = _mm_add_epi16(x_r2, x_y2);
			x_r3 = _mm_add_epi16(x_r3, x_y3);
			x_r4 = _mm_add_epi16(x_r4, x_y4);

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
			/* sequential b */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);

			/* sequential g */
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);
			/* sequential r */
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);
			/* sequence gb */
			x_g2 = _mm_unpacklo_epi8(x_g, x_r);
			x_g3 = _mm_unpackhi_epi8(x_g, x_r);
			x_r2 = _mm_unpacklo_epi8(x_g1, x_r1);
			x_r3 = _mm_unpackhi_epi8(x_g1, x_r1);

			/* sequence ar */
			x_temp = _mm_unpacklo_epi8(x_a, x_b);
			x_temp1 = _mm_unpackhi_epi8(x_a, x_b);
			x_temp2 = _mm_unpacklo_epi8(x_a, x_b1);
			x_temp3 = _mm_unpackhi_epi8(x_a, x_b1);

			/* sequence argbargb */
			x_b = _mm_unpacklo_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp, x_g2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp1, x_g3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;

			x_b = _mm_unpacklo_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b);
			pd1 += 16;
			x_b1 = _mm_unpackhi_epi16(x_temp2, x_r2);
			_mm_storeu_si128((__m128i *)pd1, x_b1);
			pd1 += 16;
			x_b2 = _mm_unpacklo_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b2);
			pd1 += 16;
			x_b3 = _mm_unpackhi_epi16(x_temp3, x_r3);
			_mm_storeu_si128((__m128i *)pd1, x_b3);
			pd1 += 16;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd = (mlib_u8 *)(argb + 4 * j);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[4 * j];
				val_y1 = y[4 * j + 1];
				val_y2 = y[4 * j + 2];
				val_y3 = y[4 * j + 3];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd[0] = a_const;
				TCLAMP_U8(pd[3], val_y0 + val_b);
				TCLAMP_U8(pd[2], val_y0 + val_g);
				SCLAMP_U8(pd[1], val_y0 + val_r);
				pd[4] = a_const;
				TCLAMP_U8(pd[7], val_y1 + val_b);
				TCLAMP_U8(pd[6], val_y1 + val_g);
				SCLAMP_U8(pd[5], val_y1 + val_r);
				pd[8] = a_const;
				TCLAMP_U8(pd[11], val_y2 + val_b);
				TCLAMP_U8(pd[10], val_y2 + val_g);
				SCLAMP_U8(pd[9], val_y2 + val_r);
				pd[12] = a_const;
				TCLAMP_U8(pd[15], val_y3 + val_b);
				TCLAMP_U8(pd[14], val_y3 + val_g);
				SCLAMP_U8(pd[13], val_y3 + val_r);
			}
		pY1 += (aylb);
		p_A1 += (aylb);
		pU += uvlb;
		pV += uvlb;
		pD1 += (4 * dlb);
	}
	}
}

/* *********************************************************** */
