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

#pragma ident	"@(#)mlib_s_VideoColorAVDUYV422int.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV444seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved with subsampling
 *
 * SYNOPSIS
 *      void mlib_VideoColorYUV444seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      src_stride)
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y, U, and V buffers
 *      have dimensions width and height. Dimension width is assumed to be a
 *      multiple of 2.
 *      Adjacent U and V values are averaged to get the output U and V values.
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *        UYVY[r][c/2] =  | (((U[r][c] + U[r][c+1]) / 2) << 24) |
 *                          (Y[r][c] << 16)
 *                        | (((V[r][c] + V[r][c+1]) / 2) << 8)) |
 *                          (Y[r][c+1])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444int_to_UYVY422int - convert YUV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *      void mlib_VideoColorYUV444int_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *yuv,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      src_stride)
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      yuv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The YUV pixel stream is broken apart and recombined into a UYVY
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The YUV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is Y[r][c], U[r][c],
 *      V[r][c], Y[r][c+1], U[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] =  | (((U[r][c] + U[r][c+1]) / 2) << 24) |
 *                           (Y[r][c] << 16)
 *                         | (((V[r][c] + V[r][c+1]) / 2) << 8)) |
 *                           (Y[r][c+1])
 */

/*
 * FUNCTION
 *      mlib_VideoColorUYV444int_to_YUYV422int - convert UYV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *      void mlib_VideoColorUYV444int_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *uyv,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      src_stride)
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      uyv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The UYV pixel stream is broken apart and recombined into a YUYV
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The UYV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is U[r][c], Y[r][c],
 *      V[r][c], U[r][c+1], Y[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (((U[r][c] + U[r][c+1]) / 2) << 16)
 *                      | (Y[r][c+1] << 8) | (((V[r][c] + V[r][c+1]) / 2))
 */

/*
 * FUNCTION
 *      mlib_VideoColorUYV444int_to_UYVY422int - convert UYV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *      void mlib_VideoColorUYV444int_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *uyv,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      src_stride)
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      uyv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The UYV pixel stream is broken apart and recombined into a UYVY
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The UYV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is U[r][c], Y[r][c],
 *      V[r][c], U[r][c+1], Y[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] =  (((U[r][c] + U[r][c+1]) / 2) << 24) | (Y[r][c] << 16)
 *                       | (((V[r][c] + V[r][c+1]) / 2) << 8)) | (Y[r][c+1])
 */

#include <mlib_video.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorUYV444int_to_UYVY422int = \
	__mlib_VideoColorUYV444int_to_UYVY422int
#pragma weak mlib_VideoColorUYV444int_to_YUYV422int = \
	__mlib_VideoColorUYV444int_to_YUYV422int
#pragma weak mlib_VideoColorYUV444int_to_UYVY422int = \
	__mlib_VideoColorYUV444int_to_UYVY422int
#pragma weak mlib_VideoColorYUV444seq_to_UYVY422int = \
	__mlib_VideoColorYUV444seq_to_UYVY422int

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorUYV444int_to_UYVY422int)
	mlib_VideoColorUYV444int_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorUYV444int_to_UYVY422int")));
__typeof__(__mlib_VideoColorUYV444int_to_YUYV422int)
	mlib_VideoColorUYV444int_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorUYV444int_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV444int_to_UYVY422int)
	mlib_VideoColorYUV444int_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444int_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV444seq_to_UYVY422int)
	mlib_VideoColorYUV444seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444seq_to_UYVY422int")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	Separate48(x_s0, x_s1, x_s2, x_d0, x_d1, x_d2)			\
{									\
	x_t1 = _mm_unpacklo_epi8(x_s0, _mm_srli_si128(x_s1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_s0, 8), x_s2);	\
	x_t3 = _mm_unpacklo_epi8(x_s1, _mm_srli_si128(x_s2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
									\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
									\
	x_d0 = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_d1 = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_d2 = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

#define	Separate24(x_s0, x_s1, x_d0, x_d1, x_d2)			\
{									\
	x_t1 = _mm_unpacklo_epi8(x_s0, _mm_srli_si128(x_s1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_s0, 8), x_zero);	\
	x_t3 = _mm_unpacklo_epi8(x_s1, x_zero);				\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
									\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
									\
	x_d0 = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_d1 = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_d2 = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

#define	U8_Split(s1, s2, d1, d2)					\
{									\
	d1 = _mm_and_si128(s1, x_mask);					\
	d2 = _mm_and_si128(s2, x_mask);					\
	d1 = _mm_packus_epi16(d1, d2);					\
	d2 = _mm_packus_epi16(_mm_srli_epi16(s1, 8),			\
			_mm_srli_epi16(s2, 8));				\
}

#define	U8_Split2(s1, d1, d2)						\
{									\
	d1 = _mm_and_si128(s1, x_mask);					\
	d1 = _mm_packus_epi16(d1, x_zero);				\
	d2 = _mm_packus_epi16(_mm_srli_epi16(s1, 8), x_zero);		\
}

/* *********************************************************** */

static void
mlib_VideoColorYUV444seq_to_UYVY422int_aligned(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_y0, x_y1, x_u0, x_u1, x_v0, x_v1, x_t0, x_t1, x_temp;
	__m128i *px_y, *px_u, *px_v, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, uyvy += dlb) {
		px_y = (__m128i *)y;
		px_u = (__m128i *)u;
		px_v = (__m128i *)v;
		px_d = (__m128i *)uyvy;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_y0 = _mm_load_si128(px_y);
			px_y++;
			x_y1 = _mm_load_si128(px_y);
			px_y++;
			x_u0 = _mm_load_si128(px_u);
			px_u++;
			x_u1 = _mm_load_si128(px_u);
			px_u++;
			x_v0 = _mm_load_si128(px_v);
			px_v++;
			x_v1 = _mm_load_si128(px_v);
			px_v++;

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_t1, x_y1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t1, x_y1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_y0 = _mm_load_si128(px_y);
			px_y++;
			x_u0 = _mm_load_si128(px_u);
			px_u++;
			x_v0 = _mm_load_si128(px_v);
			px_v++;

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_y0 = _mm_loadl_epi64(px_y);
			px_y = (__m128i *)((__m64 *)px_y + 1);
			x_u0 = _mm_loadl_epi64(px_u);
			px_u = (__m128i *)((__m64 *)px_u + 1);
			x_v0 = _mm_loadl_epi64(px_v);
			px_v = (__m128i *)((__m64 *)px_v + 1);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j1 = j + 1;

				val_y0 = y[j];
				val_y1 = y[j1];
				val_u0 = (u[j] + u[j1]) >> 1;
				val_v0 = (v[j] + v[j1]) >> 1;
				uyvy[j2] = val_u0 | (val_y0 << 8) |
					(val_v0 << 16) | (val_y1 << 24);
			}
		}
	}
}

static void
mlib_VideoColorYUV444seq_to_UYVY422int_naligned(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_y0, x_y1, x_u0, x_u1, x_v0, x_v1, x_t0, x_t1, x_temp;
	__m128i *px_y, *px_u, *px_v, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, uyvy += dlb) {
		px_y = (__m128i *)y;
		px_u = (__m128i *)u;
		px_v = (__m128i *)v;
		px_d = (__m128i *)uyvy;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_y0 = _mm_loadu_si128(px_y);
			px_y++;
			x_y1 = _mm_loadu_si128(px_y);
			px_y++;
			x_u0 = _mm_loadu_si128(px_u);
			px_u++;
			x_u1 = _mm_loadu_si128(px_u);
			px_u++;
			x_v0 = _mm_loadu_si128(px_v);
			px_v++;
			x_v1 = _mm_loadu_si128(px_v);
			px_v++;

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_t1, x_y1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t1, x_y1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_y0 = _mm_loadu_si128(px_y);
			px_y++;
			x_u0 = _mm_loadu_si128(px_u);
			px_u++;
			x_v0 = _mm_loadu_si128(px_v);
			px_v++;

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_y0 = _mm_loadl_epi64(px_y);
			px_y = (__m128i *)((__m64 *)px_y + 1);
			x_u0 = _mm_loadl_epi64(px_u);
			px_u = (__m128i *)((__m64 *)px_u + 1);
			x_v0 = _mm_loadl_epi64(px_v);
			px_v = (__m128i *)((__m64 *)px_v + 1);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j1 = j + 1;

				val_y0 = y[j];
				val_y1 = y[j1];
				val_u0 = (u[j] + u[j1]) >> 1;
				val_v0 = (v[j] + v[j1]) >> 1;
				uyvy[j2] = val_u0 | (val_y0 << 8) |
					(val_v0 << 16) | (val_y1 << 24);
			}
		}
	}
}

void
__mlib_VideoColorYUV444seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	if (NULL == uyvy || NULL == y || NULL == u || NULL == v)
		return;

	if (w == 0 || h == 0)
		return;

	if ((0 == (0xf & ((mlib_addr)y | (mlib_addr)u |
		(mlib_addr)v | (mlib_addr)uyvy))) &&
		((0 == (0xf & (dlb | slb))) || (1 == h))) {
		mlib_VideoColorYUV444seq_to_UYVY422int_aligned(
				uyvy, y, u, v, w, h, dlb, slb);
	} else {
		mlib_VideoColorYUV444seq_to_UYVY422int_naligned(
				uyvy, y, u, v, w, h, dlb, slb);
	}
}

/* *********************************************************** */

static void
mlib_VideoColorYUV444int_to_UYVY422int_aligned(
	mlib_u32 *uyvy,
	const mlib_u8 *yuv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_t6, x_t7, x_t8, x_t9, x_temp;
	__m128i x_yuv0, x_yuv1, x_yuv2, x_y0, x_y1, x_u0, x_u1, x_v0, x_v1;
	__m128i *px_yuv, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, yuv += slb, uyvy += dlb) {
		px_yuv = (__m128i *)yuv;
		px_d = (__m128i *)uyvy;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_yuv0 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv2 = _mm_load_si128(px_yuv);
			px_yuv++;
			Separate48(x_yuv0, x_yuv1, x_yuv2, x_y0, x_u0, x_v0);
			x_yuv0 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv2 = _mm_load_si128(px_yuv);
			px_yuv++;
			Separate48(x_yuv0, x_yuv1, x_yuv2, x_y1, x_u1, x_v1);

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_t1, x_y1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t1, x_y1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_yuv0 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv2 = _mm_load_si128(px_yuv);
			px_yuv++;
			Separate48(x_yuv0, x_yuv1, x_yuv2, x_y0, x_u0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_yuv0 = _mm_load_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_loadl_epi64(px_yuv);
			px_yuv = (__m128i *)((__m64 *)px_yuv + 1);
			Separate24(x_yuv0, x_yuv1, x_y0, x_u0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j3 = 3 * j;

				val_y0 = yuv[j3];
				val_y1 = yuv[j3 + 3];
				val_u0 = (yuv[j3 + 1] + yuv[j3 + 4]) >> 1;
				val_v0 = (yuv[j3 + 2] + yuv[j3 + 5]) >> 1;
				uyvy[j2] = val_u0 | (val_y0 << 8) |
					(val_v0 << 16) | (val_y1 << 24);
			}
		}
	}
}

static void
mlib_VideoColorYUV444int_to_UYVY422int_naligned(
	mlib_u32 *uyvy,
	const mlib_u8 *yuv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_t6, x_t7, x_t8, x_t9, x_temp;
	__m128i x_yuv0, x_yuv1, x_yuv2, x_y0, x_y1, x_u0, x_u1, x_v0, x_v1;
	__m128i *px_yuv, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, yuv += slb, uyvy += dlb) {
		px_yuv = (__m128i *)yuv;
		px_d = (__m128i *)uyvy;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_yuv0 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv2 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			Separate48(x_yuv0, x_yuv1, x_yuv2, x_y0, x_u0, x_v0);
			x_yuv0 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv2 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			Separate48(x_yuv0, x_yuv1, x_yuv2, x_y1, x_u1, x_v1);

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_t1, x_y1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t1, x_y1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_yuv0 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv2 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			Separate48(x_yuv0, x_yuv1, x_yuv2, x_y0, x_u0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_yuv0 = _mm_loadu_si128(px_yuv);
			px_yuv++;
			x_yuv1 = _mm_loadl_epi64(px_yuv);
			px_yuv = (__m128i *)((__m64 *)px_yuv + 1);
			Separate24(x_yuv0, x_yuv1, x_y0, x_u0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j3 = 3 * j;

				val_y0 = yuv[j3];
				val_y1 = yuv[j3 + 3];
				val_u0 = (yuv[j3 + 1] + yuv[j3 + 4]) >> 1;
				val_v0 = (yuv[j3 + 2] + yuv[j3 + 5]) >> 1;
				uyvy[j2] = val_u0 | (val_y0 << 8) |
					(val_v0 << 16) | (val_y1 << 24);
			}
		}
	}
}

void
__mlib_VideoColorYUV444int_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *yuv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	if (NULL == uyvy || NULL == yuv)
		return;

	if (w == 0 || h == 0)
		return;

	if ((0 == (0xf & ((mlib_addr)yuv | (mlib_addr)uyvy))) &&
		((0 == (0xf & (dlb | slb))) || (1 == h))) {
		mlib_VideoColorYUV444int_to_UYVY422int_aligned(
				uyvy, yuv, w, h, dlb, slb);
	} else {
		mlib_VideoColorYUV444int_to_UYVY422int_naligned(
				uyvy, yuv, w, h, dlb, slb);
	}
}

/* *********************************************************** */

static void
mlib_VideoColorUYV444int_to_YUYV422int_aligned(
	mlib_u32 *yuyv,
	const mlib_u8 *uyv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_t6, x_t7, x_t8, x_t9, x_temp;
	__m128i x_uyv0, x_uyv1, x_uyv2, x_y0, x_y1, x_u0, x_u1, x_v0, x_v1;
	__m128i *px_uyv, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, uyv += slb, yuyv += dlb) {
		px_uyv = (__m128i *)uyv;
		px_d = (__m128i *)yuyv;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_load_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_load_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u1, x_y1, x_v1);

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_y0, x_t0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_y0, x_t0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_y1, x_t1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_y1, x_t1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_load_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_y0, x_t0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_y0, x_t0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadl_epi64(px_uyv);
			px_uyv = (__m128i *)((__m64 *)px_uyv + 1);
			Separate24(x_uyv0, x_uyv1, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_y0, x_t0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j3 = 3 * j;

				val_y0 = uyv[j3 + 1];
				val_y1 = uyv[j3 + 4];
				val_u0 = (uyv[j3 + 0] + uyv[j3 + 3]) >> 1;
				val_v0 = (uyv[j3 + 2] + uyv[j3 + 5]) >> 1;
				yuyv[j2] = val_y0 | (val_u0 << 8) |
					(val_y1 << 16) | (val_v0 << 24);
			}
		}
	}
}

static void
mlib_VideoColorUYV444int_to_YUYV422int_naligned(
	mlib_u32 *yuyv,
	const mlib_u8 *uyv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_t6, x_t7, x_t8, x_t9, x_temp;
	__m128i x_uyv0, x_uyv1, x_uyv2, x_y0, x_y1, x_u0, x_u1, x_v0, x_v1;
	__m128i *px_uyv, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, uyv += slb, yuyv += dlb) {
		px_uyv = (__m128i *)uyv;
		px_d = (__m128i *)yuyv;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u1, x_y1, x_v1);

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_y0, x_t0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_y0, x_t0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_y1, x_t1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_y1, x_t1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_y0, x_t0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_y0, x_t0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadl_epi64(px_uyv);
			px_uyv = (__m128i *)((__m64 *)px_uyv + 1);
			Separate24(x_uyv0, x_uyv1, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_y0, x_t0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j3 = 3 * j;

				val_y0 = uyv[j3 + 1];
				val_y1 = uyv[j3 + 4];
				val_u0 = (uyv[j3 + 0] + uyv[j3 + 3]) >> 1;
				val_v0 = (uyv[j3 + 2] + uyv[j3 + 5]) >> 1;
				yuyv[j2] = val_y0 | (val_u0 << 8) |
					(val_y1 << 16) | (val_v0 << 24);
			}
		}
	}
}

void
__mlib_VideoColorUYV444int_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *uyv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	if (NULL == yuyv || NULL == uyv)
		return;

	if (w == 0 || h == 0)
		return;

	if ((0 == (0xf & ((mlib_addr)uyv | (mlib_addr)yuyv))) &&
		((0 == (0xf & (dlb | slb))) || (1 == h))) {
		mlib_VideoColorUYV444int_to_YUYV422int_aligned(
				yuyv, uyv, w, h, dlb, slb);
	} else {
		mlib_VideoColorUYV444int_to_YUYV422int_naligned(
				yuyv, uyv, w, h, dlb, slb);
	}
}

/* *********************************************************** */

static void
mlib_VideoColorUYV444int_to_UYVY422int_aligned(
	mlib_u32 *uyvy,
	const mlib_u8 *uyv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_t6, x_t7, x_t8, x_t9, x_temp;
	__m128i x_uyv0, x_uyv1, x_uyv2, x_y0, x_y1, x_u0, x_u1, x_v0, x_v1;
	__m128i *px_uyv, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, uyv += slb, uyvy += dlb) {
		px_uyv = (__m128i *)uyv;
		px_d = (__m128i *)uyvy;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_load_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_load_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u1, x_y1, x_v1);

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_t1, x_y1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t1, x_y1);
			_mm_store_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_load_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_uyv0 = _mm_load_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadl_epi64(px_uyv);
			px_uyv = (__m128i *)((__m64 *)px_uyv + 1);
			Separate24(x_uyv0, x_uyv1, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_store_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j3 = 3 * j;

				val_y0 = uyv[j3 + 1];
				val_y1 = uyv[j3 + 4];
				val_u0 = (uyv[j3 + 0] + uyv[j3 + 3]) >> 1;
				val_v0 = (uyv[j3 + 2] + uyv[j3 + 5]) >> 1;
				uyvy[j2] = val_u0 | (val_y0 << 8) |
					(val_v0 << 16) | (val_y1 << 24);
			}
		}
	}
}

static void
mlib_VideoColorUYV444int_to_UYVY422int_naligned(
	mlib_u32 *uyvy,
	const mlib_u8 *uyv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_t6, x_t7, x_t8, x_t9, x_temp;
	__m128i x_uyv0, x_uyv1, x_uyv2, x_y0, x_y1, x_u0, x_u1, x_v0, x_v1;
	__m128i *px_uyv, *px_d;
	const __m128i x_mask = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	for (i = 0; i < h; i++, uyv += slb, uyvy += dlb) {
		px_uyv = (__m128i *)uyv;
		px_d = (__m128i *)uyvy;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= (w - 16); j2 += 16) {
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u1, x_y1, x_v1);

			U8_Split(x_u0, x_u1, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split(x_v0, x_v1, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);
			x_t1 = _mm_unpackhi_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpacklo_epi8(x_t1, x_y1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t1, x_y1);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
		}

		if (j2 <= (w - 8)) {
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv2 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			Separate48(x_uyv0, x_uyv1, x_uyv2, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;
			x_temp = _mm_unpackhi_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 8;
		}

		if (j2 <= (w - 4)) {
			x_uyv0 = _mm_loadu_si128(px_uyv);
			px_uyv++;
			x_uyv1 = _mm_loadl_epi64(px_uyv);
			px_uyv = (__m128i *)((__m64 *)px_uyv + 1);
			Separate24(x_uyv0, x_uyv1, x_u0, x_y0, x_v0);

			U8_Split2(x_u0, x_t0, x_t1);
			x_u0 = _mm_avg_epu8(x_t0, x_t1);
			U8_Split2(x_v0, x_t0, x_t1);
			x_v0 = _mm_avg_epu8(x_t0, x_t1);
			x_t0 = _mm_unpacklo_epi8(x_u0, x_v0);

			x_temp = _mm_unpacklo_epi8(x_t0, x_y0);
			_mm_storeu_si128(px_d, x_temp);
			px_d++;

			j2 += 4;
		}

		if (j2 <= (w - 1)) {
			for (; j2 <= (w - 1); j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j3 = 3 * j;

				val_y0 = uyv[j3 + 1];
				val_y1 = uyv[j3 + 4];
				val_u0 = (uyv[j3 + 0] + uyv[j3 + 3]) >> 1;
				val_v0 = (uyv[j3 + 2] + uyv[j3 + 5]) >> 1;
				uyvy[j2] = val_u0 | (val_y0 << 8) |
					(val_v0 << 16) | (val_y1 << 24);
			}
		}
	}
}

void
__mlib_VideoColorUYV444int_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *uyv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	if (NULL == uyvy || NULL == uyv)
		return;

	if (w == 0 || h == 0)
		return;

	if ((0 == (0xf & ((mlib_addr)uyv | (mlib_addr)uyvy))) &&
		((0 == (0xf & (dlb | slb))) || (1 == h))) {
		mlib_VideoColorUYV444int_to_UYVY422int_aligned(
				uyvy, uyv, w, h, dlb, slb);
	} else {
		mlib_VideoColorUYV444int_to_UYVY422int_naligned(
				uyvy, uyv, w, h, dlb, slb);
	}
}

/* *********************************************************** */
