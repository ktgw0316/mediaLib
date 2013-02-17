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

#pragma ident	"@(#)mlib_s_VideoColorAVDYUV444.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV444seq_to_ARGBint - color convert YUV sequential
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV444seq_to_ARGBint(mlib_u32      *argb,
 *                                              const mlib_u8 *y,
 *                                              const mlib_u8 *u,
 *                                              const mlib_u8 *v,
 *                                              const mlib_u8 *a,
 *                                              mlib_u8       alpha,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      yuva_stride)
 *
 * ARGUMENTS
 *      argb         pointer to output buffer
 *      y            pointer to Y input buffer
 *      u            pointer to U input buffer
 *      v            pointer to V input buffer
 *      a            pointer to array of alpha values
 *      alpha        constant alpha value
 *      width        image width in pixels
 *      height       image height in lines
 *      dst_stride   linebytes for output buffer
 *      yuva_stride  linebytes for Y, U, V, and alpha buffers
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The Y, U, V pixel streams are converted into a ARGB pixel stream. All
 *      pixel components are 8-bit unsigned integers. All buffers have
 *      dimensions width and height.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444seq_to_ABGRint - color convert YUV sequential
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV444seq_to_ABGRint(mlib_u32      *abgr,
 *                                              const mlib_u8 *y,
 *                                              const mlib_u8 *u,
 *                                              const mlib_u8 *v,
 *                                              const mlib_u8 *a,
 *                                              mlib_u8       alpha,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      yuva_stride)
 *
 * ARGUMENTS
 *      abgr         pointer to output buffer
 *      y            pointer to Y input buffer
 *      u            pointer to U input buffer
 *      v            pointer to V input buffer
 *      a            pointer to array of alpha values
 *      alpha        constant alpha value
 *      width        image width in pixels
 *      height       image height in lines
 *      dst_stride   linebytes for output buffer
 *      yuva_stride  linebytes for Y, U, V and alpha buffers
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The Y, U, V pixel streams are converted into a ABGR pixel stream. All
 *      pixel components are 8-bit unsigned integers. All buffers have
 *      dimensions width and height.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444int_to_ARGBint - color convert YUV interleaved
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV444int_to_ARGBint(mlib_u32      *argb,
 *                                              const mlib_u8 *yuv,
 *                                              const mlib_u8 *a,
 *                                              mlib_u8       alpha,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      src_stride,
 *                                              mlib_s32      a_stride)
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      yuv         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The YUV pixel stream is converted into a ARGB pixel stream. All
 *      pixel components are 8-bit unsigned integers. All buffers have
 *      dimensions width and height.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the ARGB buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444int_to_ABGRint - color convert YUV interleaved
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV444int_to_ABGRint(mlib_u32      *abgr,
 *                                              const mlib_u8 *yuv,
 *                                              const mlib_u8 *a,
 *                                              mlib_u8       alpha,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      src_stride,
 *                                              mlib_s32      a_stride)
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      yuv         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The YUV pixel stream is converted into a ABGR pixel stream. All
 *      pixel components are 8-bit unsigned integers. All buffers have
 *      dimensions width and height.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the ABGR buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorUYV444int_to_ARGBint - color convert UYV interleaved
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *      void mlib_VideoColorUYV444int_to_ARGBint(mlib_u32      *argb,
 *                                               const mlib_u8 *uyv,
 *                                               const mlib_u8 *a,
 *                                               mlib_u8       alpha,
 *                                               mlib_s32      width,
 *                                               mlib_s32      height,
 *                                               mlib_s32      dst_stride,
 *                                               mlib_s32      src_stride,
 *                                               mlib_s32      a_stride)
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      uyv         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The UYV pixel stream is converted into a ARGB pixel stream. All
 *      pixel components are 8-bit unsigned integers. All buffers have
 *      dimensions width and width.
 *      The alpha values for this function work in the following fashion:
 *      - if 'a' pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the ARGB buffer.
 *      - if 'a' pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorUYV444int_to_ABGRint - color convert UYV interleaved
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *      void mlib_VideoColorUYV444int_to_ABGRint(mlib_u32      *abgr,
 *                                               const mlib_u8 *uyv,
 *                                               const mlib_u8 *a,
 *                                               const mlib_u8 alpha,
 *                                               mlib_s32      width,
 *                                               mlib_s32      height,
 *                                               mlib_s32      dst_stride,
 *                                               mlib_s32      src_stride,
 *                                               mlib_s32      a_stride)
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      uyv         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The UYV pixel stream is converted into a ABGR pixel stream. All
 *      pixel components are 8-bit unsigned integers. All buffers have
 *      dimensions width and width.
 *      The alpha values for this function work in the following fashion:
 *      - if 'a' pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the ABGR buffer.
 *      - if 'a' pointer is NULL, the alpha values for every pixel are
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

#pragma weak mlib_VideoColorUYV444int_to_ABGRint = \
	__mlib_VideoColorUYV444int_to_ABGRint
#pragma weak mlib_VideoColorUYV444int_to_ARGBint = \
	__mlib_VideoColorUYV444int_to_ARGBint
#pragma weak mlib_VideoColorYUV444int_to_ABGRint = \
	__mlib_VideoColorYUV444int_to_ABGRint
#pragma weak mlib_VideoColorYUV444int_to_ARGBint = \
	__mlib_VideoColorYUV444int_to_ARGBint
#pragma weak mlib_VideoColorYUV444seq_to_ABGRint = \
	__mlib_VideoColorYUV444seq_to_ABGRint
#pragma weak mlib_VideoColorYUV444seq_to_ARGBint = \
	__mlib_VideoColorYUV444seq_to_ARGBint

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorUYV444int_to_ABGRint)
	mlib_VideoColorUYV444int_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorUYV444int_to_ABGRint")));
__typeof__(__mlib_VideoColorUYV444int_to_ARGBint)
	mlib_VideoColorUYV444int_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorUYV444int_to_ARGBint")));
__typeof__(__mlib_VideoColorYUV444int_to_ABGRint)
	mlib_VideoColorYUV444int_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorYUV444int_to_ABGRint")));
__typeof__(__mlib_VideoColorYUV444int_to_ARGBint)
	mlib_VideoColorYUV444int_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorYUV444int_to_ARGBint")));
__typeof__(__mlib_VideoColorYUV444seq_to_ABGRint)
	mlib_VideoColorYUV444seq_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorYUV444seq_to_ABGRint")));
__typeof__(__mlib_VideoColorYUV444seq_to_ARGBint)
	mlib_VideoColorYUV444seq_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorYUV444seq_to_ARGBint")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(dst, s)	dst = mlib_video_pClipOff256[s]

/* *********************************************************** */

#define	SCLAMP_U8(dst, s)                                                  \
	{                                                                  \
	    mlib_s32 v = s, mask = (v - 0xff00000) >> 31;                  \
	                                                                   \
	    dst = (mlib_u8)((((mlib_u32)v >> 20) | ~mask) & ~(v >> 31));   \
	}

/* *********************************************************** */
/* rgb: 48 bytes */
#define	SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g,  x_b)	\
{	\
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9; \
	x_t1 = _mm_unpacklo_epi8(x_rgb0, _mm_srli_si128(x_rgb1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_rgb0, 8), x_rgb2);	\
	x_t3 = _mm_unpacklo_epi8(x_rgb1, _mm_srli_si128(x_rgb2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_r = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_b = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444int_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *yuv,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
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

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_a, *px_d;
	__m128i x_y, x_u, x_v, x_rgb1, x_rgb2, x_rgb3;
	mlib_u8 *pY, *pU, *pV, *pD;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, yuv += slb, abgr += dlb, \
				a_array += alb) {
			px_y = (__m128i*)yuv;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_y, \
					x_u, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			/* create arar sequences  */
			__m128i x_a = _mm_loadu_si128(px_a++);
			x_zbl = _mm_unpacklo_epi8(x_a, x_b);
			x_zbh = _mm_unpackhi_epi8(x_a, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(abgr + j);

				val_y = yuv[si];
				val_u = yuv[si + 1];
				val_v = yuv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_array[j];
				SCLAMP_U8(pd[1], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[3], s0 & 0x3ff);
			}
		}
	} else {
		__m128i x_a = _mm_set1_epi8((mlib_u8)a_const);
		for (i = 0; i < h; i++, yuv += slb, abgr += dlb) {
			px_y = (__m128i*)yuv;
			px_d = (__m128i*)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_y, \
					x_u, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			/* create arar sequences  */
			x_zbl = _mm_unpacklo_epi8(x_a, x_b);
			x_zbh = _mm_unpackhi_epi8(x_a, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

			for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(abgr + j);

				val_y = yuv[si];
				val_u = yuv[si + 1];
				val_v = yuv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_const;
				SCLAMP_U8(pd[1], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[3], s0 & 0x3ff);
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444int_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u8 *yuv,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
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

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_a, *px_d;
	__m128i x_y, x_u, x_v, x_rgb1, x_rgb2, x_rgb3;
	mlib_u8 *pY, *pU, *pV, *pD;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, yuv += slb, argb += dlb, \
				a_array += alb)	{
			px_y = (__m128i*)yuv;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_y, \
					x_u, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_b);
			x_grh = _mm_unpackhi_epi8(x_g, x_b);

			/* create arar sequences  */
			__m128i x_a = _mm_loadu_si128(px_a++);
			x_zbl = _mm_unpacklo_epi8(x_a, x_r);
			x_zbh = _mm_unpackhi_epi8(x_a, x_r);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

			for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(argb + j);

				val_y = yuv[si];
				val_u = yuv[si + 1];
				val_v = yuv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_array[j];
				SCLAMP_U8(pd[3], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[1], s0 & 0x3ff);
			}
		}
	} else {
		__m128i x_a = _mm_set1_epi8((mlib_u8)a_const);
		for (i = 0; i < h; i++, yuv += slb, argb += dlb) {
			px_y = (__m128i*)yuv;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_y, \
					x_u, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_b);
			x_grh = _mm_unpackhi_epi8(x_g, x_b);

			/* create arar sequences  */
			x_zbl = _mm_unpacklo_epi8(x_a, x_r);
			x_zbh = _mm_unpackhi_epi8(x_a, x_r);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

			for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(argb + j);

				val_y = yuv[si];
				val_u = yuv[si + 1];
				val_v = yuv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_const;
				SCLAMP_U8(pd[3], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[1], s0 & 0x3ff);
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444seq_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
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
	const __m128i x_mask = _mm_set1_epi32(0xff);

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_u, *px_v, *px_d;
	mlib_u8 *pY, *pU, *pV, *pD;
	__m128i *px_a;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, abgr +=
			dlb, a_array += slb) {
			px_y = (__m128i*)y;
			px_u = (__m128i*)u;
			px_v = (__m128i*)v;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)abgr;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_temp = _mm_loadu_si128(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			x_uh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_u++;
			x_temp = _mm_loadu_si128(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_vh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_v++;
			x_temp = _mm_loadu_si128(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_yh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_y++;

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create gr sequences */
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			/* create arar sequences  */
			__m128i x_a = _mm_loadu_si128(px_a++);
			x_zbl = _mm_unpacklo_epi8(x_a, x_b);
			x_zbh = _mm_unpackhi_epi8(x_a, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

			for (; j < w; j++) {
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(abgr + j);

				val_y = y[j];
				val_u = u[j];
				val_v = v[j];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_array[j];
				SCLAMP_U8(pd[1], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[3], s0 & 0x3ff);
			}
		}
	} else {
		__m128i x_a = _mm_set1_epi8((mlib_u8)a_const);
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, abgr +=
			dlb, a_array += slb) {
			px_y = (__m128i*)y;
			px_u = (__m128i*)u;
			px_v = (__m128i*)v;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)abgr;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_temp = _mm_loadu_si128(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			x_uh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_u++;
			x_temp = _mm_loadu_si128(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_vh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_v++;
			x_temp = _mm_loadu_si128(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_yh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_y++;

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create gr sequences */
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			/* create arar sequences  */
			x_zbl = _mm_unpacklo_epi8(x_a, x_b);
			x_zbh = _mm_unpackhi_epi8(x_a, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(abgr + j);

				val_y = y[j];
				val_u = u[j];
				val_v = v[j];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_const;
				SCLAMP_U8(pd[1], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[3], s0 & 0x3ff);
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444seq_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
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
	const __m128i x_mask = _mm_set1_epi32(0xff);

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_u, *px_v, *px_d;
	mlib_u8 *pY, *pU, *pV, *pD;
	__m128i *px_a;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, argb +=
			dlb, a_array += slb) {
			px_y = (__m128i*)y;
			px_u = (__m128i*)u;
			px_v = (__m128i*)v;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)argb;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_temp = _mm_loadu_si128(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			x_uh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_u++;
			x_temp = _mm_loadu_si128(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_vh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_v++;
			x_temp = _mm_loadu_si128(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_yh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_y++;

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create gb sequences */
			x_grl = _mm_unpacklo_epi8(x_g, x_b);
			x_grh = _mm_unpackhi_epi8(x_g, x_b);

			/* create arar sequences  */
			__m128i x_a = _mm_loadu_si128(px_a++);
			x_zbl = _mm_unpacklo_epi8(x_a, x_r);
			x_zbh = _mm_unpackhi_epi8(x_a, x_r);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(argb + j);

				val_y = y[j];
				val_u = u[j];
				val_v = v[j];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_array[j];
				SCLAMP_U8(pd[3], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[1], s0 & 0x3ff);
			}
		}
	} else {
		__m128i x_a = _mm_set1_epi8((mlib_u8)a_const);
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, argb +=
			dlb, a_array += slb) {
			px_y = (__m128i*)y;
			px_u = (__m128i*)u;
			px_v = (__m128i*)v;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_temp = _mm_loadu_si128(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			x_uh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_u++;
			x_temp = _mm_loadu_si128(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_vh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_v++;
			x_temp = _mm_loadu_si128(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_yh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_y++;

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create gb sequences */
			x_grl = _mm_unpacklo_epi8(x_g, x_b);
			x_grh = _mm_unpackhi_epi8(x_g, x_b);

			/* create arar sequences  */
			x_zbl = _mm_unpacklo_epi8(x_a, x_r);
			x_zbh = _mm_unpackhi_epi8(x_a, x_r);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(argb + j);

				val_y = y[j];
				val_u = u[j];
				val_v = v[j];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_const;
				SCLAMP_U8(pd[3], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[1], s0 & 0x3ff);
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorUYV444int_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *uyv,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
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

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_a, *px_d;
	__m128i x_y, x_u, x_v, x_rgb1, x_rgb2, x_rgb3;
	mlib_u8 *pY, *pU, *pV, *pD;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, uyv += slb, abgr += dlb, \
				a_array += alb)	{
			px_y = (__m128i*)uyv;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_u, \
					x_y, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			/* create arar sequences  */
			__m128i x_a = _mm_loadu_si128(px_a++);
			x_zbl = _mm_unpacklo_epi8(x_a, x_b);
			x_zbh = _mm_unpackhi_epi8(x_a, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(abgr + j);

				val_u = uyv[si];
				val_y = uyv[si + 1];
				val_v = uyv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_array[j];
				SCLAMP_U8(pd[1], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[3], s0 & 0x3ff);
			}
		}
	} else {
		__m128i x_a = _mm_set1_epi8((mlib_u8)a_const);
		for (i = 0; i < h; i++, uyv += slb, abgr += dlb) {
			px_y = (__m128i*)uyv;
			px_d = (__m128i*)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_u, \
					x_y, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			/* create arar sequences  */
			x_zbl = _mm_unpacklo_epi8(x_a, x_b);
			x_zbh = _mm_unpackhi_epi8(x_a, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(abgr + j);

				val_u = uyv[si];
				val_y = uyv[si + 1];
				val_v = uyv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_const;
				SCLAMP_U8(pd[1], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[3], s0 & 0x3ff);
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorUYV444int_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u8 *uyv,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
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

	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_a, *px_d;
	__m128i x_y, x_u, x_v, x_rgb1, x_rgb2, x_rgb3;
	mlib_u8 *pY, *pU, *pV, *pD;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, uyv += slb, argb += dlb, \
				a_array += alb) {
			px_y = (__m128i*)uyv;
			px_a = (__m128i *)a_array;
			px_d = (__m128i*)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_u, \
					x_y, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_b);
			x_grh = _mm_unpackhi_epi8(x_g, x_b);

			/* create arar sequences  */
			__m128i x_a = _mm_loadu_si128(px_a++);
			x_zbl = _mm_unpacklo_epi8(x_a, x_r);
			x_zbh = _mm_unpackhi_epi8(x_a, x_r);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(argb + j);

				val_u = uyv[si];
				val_y = uyv[si + 1];
				val_v = uyv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_array[j];
				SCLAMP_U8(pd[3], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[1], s0 & 0x3ff);
			}
		}
	} else {
		__m128i x_a = _mm_set1_epi8((mlib_u8)a_const);
		for (i = 0; i < h; i++, uyv += slb, argb += dlb) {
			px_y = (__m128i*)uyv;
			px_d = (__m128i*)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= w - 16; j += 16) {
			x_rgb1 = _mm_loadu_si128(px_y++);
			x_rgb2 = _mm_loadu_si128(px_y++);
			x_rgb3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_rgb1, x_rgb2, x_rgb3, x_u, \
					x_y, x_v);
			x_ul = _mm_unpacklo_epi8(x_zero, x_u);
			x_uh = _mm_unpackhi_epi8(x_zero, x_u);
			x_vl = _mm_unpacklo_epi8(x_zero, x_v);
			x_vh = _mm_unpackhi_epi8(x_zero, x_v);
			x_yl = _mm_unpacklo_epi8(x_zero, x_y);
			x_yh = _mm_unpackhi_epi8(x_zero, x_y);

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create grgr sequence */
			x_grl = _mm_unpacklo_epi8(x_g, x_b);
			x_grh = _mm_unpackhi_epi8(x_g, x_b);

			/* create arar sequences  */
			x_zbl = _mm_unpacklo_epi8(x_a, x_r);
			x_zbh = _mm_unpackhi_epi8(x_a, x_r);

			x_rgbl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbl, x_grl);
			_mm_storeu_si128(px_d++, x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zbh, x_grh);
			_mm_storeu_si128(px_d++, x_rgbh);
			}

	for (; j < w; j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 val_y, val_u, val_v, s0;
				mlib_u8 *pd = (mlib_u8 *)(argb + j);

				val_u = uyv[si];
				val_y = uyv[si + 1];
				val_v = uyv[si + 2];

				s0 = mlib_t_Yuv2rgb[val_y] +
					mlib_t_yUv2rgb[val_u] +
					mlib_t_yuV2rgb[val_v];

				pd[0] = a_const;
				SCLAMP_U8(pd[3], s0);
				TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);
				TCLAMP_U8(pd[1], s0 & 0x3ff);
			}
		}
	}
}

/* *********************************************************** */
