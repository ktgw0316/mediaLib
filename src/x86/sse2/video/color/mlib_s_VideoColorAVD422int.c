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

#pragma ident	"@(#)mlib_s_VideoColorAVD422int.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV420seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *u,
 *                                                 const mlib_u8 *v,
 *                                                 mlib_s32      width,
 *                                                 mlib_s32      height,
 *                                                 mlib_s32      dst_stride,
 *                                                 mlib_s32      y_stride,
 *                                                 mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for YUYV buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive row
 *      of the output buffer YUYV use successive rows of Y and the same rows
 *      of U and V.
 *
 *      For r = 0, 2, 4, ..., height - 2 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (U[r/2][c/2] << 16) |
 *                        (Y[r][c+1] << 8) | (V[r/2][c/2])
 *
 *         YUYV[r+1][c/2] = (Y[r+1][c]  << 24) | (U[r/2][c/2] << 16) |
 *                          (Y[r+1][c+1] << 8) | (V[r/2][c/2])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV411seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *u,
 *                                                 const mlib_u8 *v,
 *                                                 mlib_s32      width,
 *                                                 mlib_s32      height,
 *                                                 mlib_s32      dst_stride,
 *                                                 mlib_s32      y_stride,
 *                                                 mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for YUYV buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row, every
 *      4 Y values use the same U and V values.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 4, 8, ..., width - 4:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (U[r][c/4] << 16) |
 *                        (Y[r][c+1] << 8) | (V[r][c/4])
 *
 *         YUYV[r][c/2+1] = (Y[r][c+2] << 24) | (U[r][c/4] << 16) |
 *                          (Y[r][c+3] <<  8) | (V[r][c/4])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV422seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV422seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *u,
 *                                                 const mlib_u8 *v,
 *                                                 mlib_s32      width,
 *                                                 mlib_s32      height,
 *                                                 mlib_s32      dst_stride,
 *                                                 mlib_s32      y_stride,
 *                                                 mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for YUYV buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width / 2 and height.
 *      Dimension width is assumed to be even. Similar to
 *      mlib_VideoColorYUV420seq_to_YUYV422int except U and V are not sampled
 *      in the height direction.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (U[r][c/2] << 16) |
 *                        (Y[r][c+1] << 8) | (V[r][c/2])
 *
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved with subsampling
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV444seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *u,
 *                                                 const mlib_u8 *v,
 *                                                 mlib_s32      width,
 *                                                 mlib_s32      height,
 *                                                 mlib_s32      dst_stride,
 *                                                 mlib_s32      src_stride)
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y, U, and V buffers
 *      have dimensions width and height. Dimension width is assumed to be a
 *      multiple of 2.
 *      Adjacent U and V values are averaged to get the output U and V values.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (((U[r][c] + U[r][c+1]) / 2) << 16)
 *                      | (Y[r][c+1] << 8) | (((V[r][c] + V[r][c+1]) / 2))
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444int_to_YUYV422int - convert YUV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV444int_to_YUYV422int(mlib_u32      *yuyv,
 *                                                 const mlib_u8 *yuv,
 *                                                 mlib_s32      width,
 *                                                 mlib_s32      height,
 *                                                 mlib_s32      dst_stride,
 *                                                 mlib_s32      src_stride)
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      yuv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The YUV pixel stream is broken apart and recombined into a YUYV
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The YUV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is Y[r][c], U[r][c],
 *      V[r][c], Y[r][c+1], U[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (((U[r][c] + U[r][c+1]) / 2) << 16)
 *                      | (Y[r][c+1] << 8) | (((V[r][c] + V[r][c+1]) / 2))
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *      void mlib_VideoColorYUV420seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      y_stride,
 *                                                  mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream.
 *      All pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive rows
 *      of the output buffer UYVY use successive rows of Y and the same
 *      rows of U and V.
 *
 *      For r = 0, 2, 4, ..., height - 2 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] = (U[r/2][c/2] << 24) | (Y[r][c] << 16) |
 *                        (V[r/2][c/2] <<  8) | (Y[r][c+1])
 *
 *         UYVY[r+1][c/2] = (U[r/2][c/2] << 24) | (Y[r+1][c] << 16) |
 *                          (V[r/2][c/2] <<  8) | (Y[r+1][c+1])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *      void mlib_VideoColorYUV411seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      y_stride,
 *                                                  mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream.
 *      All pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row,
 *      every 4 Y values use the same U and V values.
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 4, 8, ..., width - 4:
 *         UYVY[r][c/2] = (U[r][c/4] << 24) | (Y[r][c] << 16) |
 *                        (V[r][c/4] <<  8) | (Y[r][c+1])
 *
 *         UYVY[r][c/2+1] = (U[r][c/4] << 24) | (Y[r][c+2] << 16) |
 *                          (V[r][c/4] <<  8) | (Y[r][c+3])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV422seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *      void mlib_VideoColorYUV422seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      width,
 *                                                  mlib_s32      height,
 *                                                  mlib_s32      dst_stride,
 *                                                  mlib_s32      y_stride,
 *                                                  mlib_s32      uv_stride)
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream.
 *      All pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height.
 *      Dimensions width is assumed to be even. Similar to
 *      mlib_VideoColorYUV420seq_to_UYVY422int except U and V are not sampled
 *      in the height direction.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] = (U[r][c/2] << 24) | (Y[r][c] << 16) |
 *                        (V[r][c/2] << 8)  | (Y[r][c+1])
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

#pragma weak mlib_VideoColorYUV411seq_to_UYVY422int = \
	__mlib_VideoColorYUV411seq_to_UYVY422int
#pragma weak mlib_VideoColorYUV411seq_to_YUYV422int = \
	__mlib_VideoColorYUV411seq_to_YUYV422int
#pragma weak mlib_VideoColorYUV420seq_to_UYVY422int = \
	__mlib_VideoColorYUV420seq_to_UYVY422int
#pragma weak mlib_VideoColorYUV420seq_to_YUYV422int = \
	__mlib_VideoColorYUV420seq_to_YUYV422int
#pragma weak mlib_VideoColorYUV422seq_to_UYVY422int = \
	__mlib_VideoColorYUV422seq_to_UYVY422int
#pragma weak mlib_VideoColorYUV422seq_to_YUYV422int = \
	__mlib_VideoColorYUV422seq_to_YUYV422int
#pragma weak mlib_VideoColorYUV444int_to_YUYV422int = \
	__mlib_VideoColorYUV444int_to_YUYV422int
#pragma weak mlib_VideoColorYUV444seq_to_YUYV422int = \
	__mlib_VideoColorYUV444seq_to_YUYV422int

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV411seq_to_UYVY422int)
	mlib_VideoColorYUV411seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV411seq_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV411seq_to_YUYV422int)
	mlib_VideoColorYUV411seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV411seq_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV420seq_to_UYVY422int)
	mlib_VideoColorYUV420seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV420seq_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV420seq_to_YUYV422int)
	mlib_VideoColorYUV420seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV420seq_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV422seq_to_UYVY422int)
	mlib_VideoColorYUV422seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV422seq_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV422seq_to_YUYV422int)
	mlib_VideoColorYUV422seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV422seq_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV444int_to_YUYV422int)
	mlib_VideoColorYUV444int_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444int_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV444seq_to_YUYV422int)
	mlib_VideoColorYUV444seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444seq_to_YUYV422int")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

void
__mlib_VideoColorYUV444seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_y, x_u, x_v;
	__m128i x_u1, x_u2, x_v1, x_v2;
	__m128i *px_y, *px_u, *px_v, *px_d;
	__m128i x_t1, x_t2, x_t3;
	__m128i x_zero = _mm_setzero_si128();
	__m128i mask = _mm_set1_epi16(0xFF);

	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, yuyv += dlb) {
		px_y = (__m128i *)y;
		px_u = (__m128i *)u;
		px_v = (__m128i *)v;
		px_d = (__m128i *)yuyv;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= w - 8; j2 += 8) {
			x_y = _mm_loadu_si128(px_y++);
			x_u = _mm_loadu_si128(px_u++);
			x_u1 = _mm_srli_si128(x_u, 1);
			x_u2 = _mm_avg_epu8(x_u, x_u1);
			x_u = _mm_and_si128(x_u2, mask);
			x_v = _mm_loadu_si128(px_v++);
			x_v1 = _mm_slli_si128(x_v, 1);
			x_v2 = _mm_avg_epu8(x_v, x_v1);
			x_v = _mm_andnot_si128(mask, x_v2);

			x_t1 = _mm_or_si128(x_v, x_u);
			x_t2 = _mm_unpacklo_epi8(x_y, x_t1);
			_mm_storeu_si128(px_d++, x_t2);
			x_t3 = _mm_unpackhi_epi8(x_y, x_t1);
			_mm_storeu_si128(px_d++, x_t3);
		}

		for (; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			val_y0 = y[j];
			val_y1 = y[j1];
			val_u0 = (u[j] + u[j1]) >> 1;
			val_v0 = (v[j] + v[j1]) >> 1;
			yuyv[j2] =
				val_y0 | (val_u0 << 8) | (val_y1 << 16) |
				(val_v0 << 24);
		}
	}
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
__mlib_VideoColorYUV444int_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *yuv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	__m128i x_y, x_u, x_v;
	__m128i x_yuv1, x_yuv2, x_yuv3;
	__m128i x_u1, x_u2, x_v1, x_v2;
	__m128i *px_y, *px_u, *px_v, *px_d;
	__m128i x_t1, x_t2, x_t3;
	__m128i x_zero = _mm_setzero_si128();
	__m128i mask = _mm_set1_epi16(0xFF);

	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, yuv += slb, yuyv += dlb) {
		px_y = (__m128i *)yuv;
		px_d = (__m128i *)yuyv;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 <= w - 8; j2 += 8) {
			x_yuv1 = _mm_loadu_si128(px_y++);
			x_yuv2 = _mm_loadu_si128(px_y++);
			x_yuv3 = _mm_loadu_si128(px_y++);
			SeparateRGB48(x_yuv1, x_yuv2, x_yuv3, x_y, \
					x_u, x_v);
			x_u1 = _mm_srli_si128(x_u, 1);
			x_u2 = _mm_avg_epu8(x_u, x_u1);
			x_u = _mm_and_si128(x_u2, mask);
			x_v1 = _mm_slli_si128(x_v, 1);
			x_v2 = _mm_avg_epu8(x_v, x_v1);
			x_v = _mm_andnot_si128(mask, x_v2);

			x_t1 = _mm_or_si128(x_v, x_u);
			x_t2 = _mm_unpacklo_epi8(x_y, x_t1);
			_mm_storeu_si128(px_d++, x_t2);
			x_t3 = _mm_unpackhi_epi8(x_y, x_t1);
			_mm_storeu_si128(px_d++, x_t3);
		}

		for (; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j3 = 3 * j;

			val_y0 = yuv[j3];
			val_y1 = yuv[j3 + 3];
			val_u0 = (yuv[j3 + 1] + yuv[j3 + 4]) >> 1;
			val_v0 = (yuv[j3 + 2] + yuv[j3 + 5]) >> 1;
			yuyv[j2] =
				val_y0 | (val_u0 << 8) | (val_y1 << 16) |
				(val_v0 << 24);
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV422seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	__m128i x_y, x_u, x_v, x_y1;
	__m128i *px_u, *px_v, *px_y, *pD;
	__m128i x_yuv1, x_yuv2, x_yuv3, x_yuv4;
	__m128i x_u1, x_u2, x_v1, x_v2;
	mlib_s32 val0, val1;
	mlib_s32 i, j2;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < h; i++) {
		px_u = (__m128i *)&u[i * uvlb];
		px_v = (__m128i *)&v[i * uvlb];
		px_y = (__m128i *)&y[i * ylb];
		pD = (__m128i *)&yuyv[i * dlb];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w - 16; j2 += 16) {
			x_u = _mm_loadu_si128(px_u++);
			x_v = _mm_loadu_si128(px_v++);
			x_y = _mm_loadu_si128(px_y++);
			x_y1 = _mm_loadu_si128(px_y++);
			x_u1 = _mm_unpacklo_epi8(x_u, x_v);
			x_u2 = _mm_unpackhi_epi8(x_u, x_v);
			x_yuv1 = _mm_unpacklo_epi8(x_y, x_u1);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_y, x_u1);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_y1, x_u2);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_y1, x_u2);
			_mm_storeu_si128(pD++, x_yuv4);
		}

		for (; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			val0 = (u[i * uvlb + j2] << 8) | (v[i * uvlb +
				j2] << 24);
			val1 = (y[i * ylb + j]) | (y[i * ylb + j1] << 16);
			yuyv[i * dlb + j2 ] = val0 | val1;
		}

	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	__m128i x_y, x_u, x_v, x_y1, x_y2, x_y3;
	__m128i *px_u, *px_v, *px_y, *pD;
	__m128i x_yuv1, x_yuv2, x_yuv3, x_yuv4;
	__m128i x_u1, x_u2, x_v1, x_v2;
	mlib_s32 val0, val1, val2;
	mlib_s32 i, j4;

	dlb >>= 2;
	w >>= 2;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < h; i++) {
		px_u = (__m128i *)&u[i * uvlb];
		px_v = (__m128i *)&v[i * uvlb];
		px_y = (__m128i *)&y[i * ylb];
		pD = (__m128i *)&yuyv[i * dlb];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j4 = 0; j4 < w - 16; j4 += 16) {
			x_u = _mm_loadu_si128(px_u++);
			x_v = _mm_loadu_si128(px_v++);
			x_y = _mm_loadu_si128(px_y++);
			x_y1 = _mm_loadu_si128(px_y++);
			x_y2 = _mm_loadu_si128(px_y++);
			x_y3 = _mm_loadu_si128(px_y++);
			x_u1 = _mm_unpacklo_epi8(x_u, x_v);
			x_u2 = _mm_unpackhi_epi8(x_u, x_v);
			x_v1 = _mm_unpacklo_epi16(x_u1, x_u1);
			x_u1 = _mm_unpackhi_epi16(x_u1, x_u1);
			x_v2 = _mm_unpacklo_epi16(x_u2, x_u2);
			x_u2 = _mm_unpackhi_epi16(x_u2, x_u2);

			x_yuv1 = _mm_unpacklo_epi8(x_y, x_v1);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_y, x_v1);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_y1, x_u1);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_y1, x_u1);
			_mm_storeu_si128(pD++, x_yuv4);

			x_yuv1 = _mm_unpacklo_epi8(x_y2, x_v2);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_y2, x_v2);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_y3, x_u2);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_y3, x_u2);
			_mm_storeu_si128(pD++, x_yuv4);
		}

		for (; j4 < w; j4++) {
			mlib_s32 j = 4 * j4;
			mlib_s32 j1 = j + 1;
			mlib_s32 j2 = j + 2;
			mlib_s32 j3 = j + 3;

			val0 = (u[i * uvlb + j4] << 8) | (v[i * uvlb +
				j4] << 24);
			val1 = (y[i * ylb + j]) | (y[i * ylb + j1] << 16);
			val2 = (y[i * ylb + j2]) | (y[i * ylb + j3] << 16);
			yuyv[i * dlb + 2 * j4 ] = val0 | val1;
			yuyv[i * dlb + 2 * j4 + 1] = val0 | val2;
		}

	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	__m128i x_y, x_u, x_v, x_y1, x_y2, x_y3;
	__m128i *px_u, *px_v, *px_y, *pD, *px_y1, *pD1;
	__m128i x_yuv1, x_yuv2, x_yuv3, x_yuv4;
	__m128i x_u1, x_u2, x_v1, x_v2;
	mlib_s32 i2, j2;
	mlib_s32 i;
	mlib_s32 i1;
	mlib_s32 ld0;
	mlib_s32 ld1;
	mlib_s32 ld2;
	mlib_s32 ld3;
	mlib_s32 ld4;
	mlib_s32 ld5;
	mlib_s32 val0, val1, val2;

	h >>= 1;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i2 = 0; i2 < h; i2++) {
		i = 2 * i2;
		i1 = i + 1;

		px_u = (__m128i *)&u[i2 * uvlb];
		px_v = (__m128i *)&v[i2 * uvlb];
		px_y = (__m128i *)&y[i * ylb];
		px_y1 = (__m128i *)&y[i1 * ylb];
		pD = (__m128i *)((mlib_u8 *)yuyv + i * dlb);
		pD1 = (__m128i *)((mlib_u8 *)yuyv + i1 * dlb);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w - 16; j2 += 16) {
			x_u = _mm_loadu_si128(px_u++);
			x_v = _mm_loadu_si128(px_v++);
			x_y = _mm_loadu_si128(px_y++);
			x_y1 = _mm_loadu_si128(px_y++);
			x_y2 = _mm_loadu_si128(px_y1++);
			x_y3 = _mm_loadu_si128(px_y1++);
			x_u1 = _mm_unpacklo_epi8(x_u, x_v);
			x_u2 = _mm_unpackhi_epi8(x_u, x_v);

			x_yuv1 = _mm_unpacklo_epi8(x_y, x_u1);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_y, x_u1);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_y1, x_u2);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_y1, x_u2);
			_mm_storeu_si128(pD++, x_yuv4);

			x_yuv1 = _mm_unpacklo_epi8(x_y2, x_u1);
			_mm_storeu_si128(pD1++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_y2, x_u1);
			_mm_storeu_si128(pD1++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_y3, x_u2);
			_mm_storeu_si128(pD1++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_y3, x_u2);
			_mm_storeu_si128(pD1++, x_yuv4);
		}
		for (; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			ld0 = u[i2 * uvlb + j2];
			ld1 = v[i2 * uvlb + j2];
			ld2 = y[i * ylb + j];
			ld3 = y[i * ylb + j1];
			ld4 = y[i1 * ylb + j];
			ld5 = y[i1 * ylb + j1];

			val0 = (ld0 << 8) | (ld1 << 24);
			val1 = (ld2) | (ld3 << 16);
			val2 = (ld4) | (ld5 << 16);
			*(mlib_u32 *)((mlib_u8 *)yuyv + i * dlb +
				4 * (j2)) = val0 | val1;
			*(mlib_u32 *)((mlib_u8 *)yuyv + i1 * dlb +
					4 * (j2)) = val0 | val2;
		}

	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	__m128i x_y, x_u, x_v, x_y1, x_y2, x_y3;
	__m128i *px_u, *px_v, *px_y, *pD, *px_y1, *pD1;
	__m128i x_yuv1, x_yuv2, x_yuv3, x_yuv4;
	__m128i x_u1, x_u2, x_v1, x_v2;
	mlib_s32 i2, j2;
	mlib_s32 i, i1;
	mlib_s32 ld0;
	mlib_s32 ld1;
	mlib_s32 ld2;
	mlib_s32 ld3;
	mlib_s32 ld4;
	mlib_s32 ld5;
	mlib_s32 val0, val1, val2;

	h >>= 1;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i2 = 0; i2 < h; i2++) {
		i = 2 * i2;
		i1 = i + 1;
		px_u = (__m128i *)&u[i2 * uvlb];
		px_v = (__m128i *)&v[i2 * uvlb];
		px_y = (__m128i *)&y[i * ylb];
		px_y1 = (__m128i *)&y[i1 * ylb];
		pD = (__m128i *)((mlib_u8 *)uyvy + i * dlb);
		pD1 = (__m128i *)((mlib_u8 *)uyvy + i1 * dlb);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w - 16; j2 += 16) {
			x_u = _mm_loadu_si128(px_u++);
			x_v = _mm_loadu_si128(px_v++);
			x_y = _mm_loadu_si128(px_y++);
			x_y1 = _mm_loadu_si128(px_y++);
			x_y2 = _mm_loadu_si128(px_y1++);
			x_y3 = _mm_loadu_si128(px_y1++);
			x_u1 = _mm_unpacklo_epi8(x_u, x_v);
			x_u2 = _mm_unpackhi_epi8(x_u, x_v);

			x_yuv1 = _mm_unpacklo_epi8(x_u1, x_y);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_u1, x_y);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_u2, x_y1);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_u2, x_y1);
			_mm_storeu_si128(pD++, x_yuv4);

			x_yuv1 = _mm_unpacklo_epi8(x_u1, x_y2);
			_mm_storeu_si128(pD1++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_u1, x_y2);
			_mm_storeu_si128(pD1++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_u2, x_y3);
			_mm_storeu_si128(pD1++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_u2, x_y3);
			_mm_storeu_si128(pD1++, x_yuv4);
		}

		for (; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;


			ld0 = u[i2 * uvlb + j2];
			ld1 = v[i2 * uvlb + j2];
			ld2 = y[i * ylb + j];
			ld3 = y[i * ylb + j1];
			ld4 = y[i1 * ylb + j];
			ld5 = y[i1 * ylb + j1];

			val0 = (ld0) | (ld1 << 16);
			val1 = (ld2 << 8) | (ld3 << 24);
			val2 = (ld4 << 8) | (ld5 << 24);
			*(mlib_u32 *)((mlib_u8 *)uyvy + i * dlb +
					4 * (j2)) = val0 | val1;
			*(mlib_u32 *)((mlib_u8 *)uyvy + i1 * dlb +
					4 * (j2)) = val0 | val2;
		}

	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	__m128i x_y, x_u, x_v, x_y1, x_y2, x_y3;
	__m128i *px_u, *px_v, *px_y, *pD;
	__m128i x_yuv1, x_yuv2, x_yuv3, x_yuv4;
	__m128i x_u1, x_u2, x_v1, x_v2;
	mlib_s32 val0, val1, val2;
	mlib_s32 i, j4;

	dlb >>= 2;
	w >>= 2;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < h; i++) {
		px_u = (__m128i *)&u[i * uvlb];
		px_v = (__m128i *)&v[i * uvlb];
		px_y = (__m128i *)&y[i * ylb];
		pD = (__m128i *)&uyvy[i * dlb];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j4 = 0; j4 < w - 16; j4 += 16) {
			x_u = _mm_loadu_si128(px_u++);
			x_v = _mm_loadu_si128(px_v++);
			x_y = _mm_loadu_si128(px_y++);
			x_y1 = _mm_loadu_si128(px_y++);
			x_y2 = _mm_loadu_si128(px_y++);
			x_y3 = _mm_loadu_si128(px_y++);
			x_u1 = _mm_unpacklo_epi8(x_u, x_v);
			x_u2 = _mm_unpackhi_epi8(x_u, x_v);
			x_v1 = _mm_unpacklo_epi16(x_u1, x_u1);
			x_u1 = _mm_unpackhi_epi16(x_u1, x_u1);
			x_v2 = _mm_unpacklo_epi16(x_u2, x_u2);
			x_u2 = _mm_unpackhi_epi16(x_u2, x_u2);

			x_yuv1 = _mm_unpacklo_epi8(x_v1, x_y);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_v1, x_y);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_u1, x_y1);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_u1, x_y1);
			_mm_storeu_si128(pD++, x_yuv4);

			x_yuv1 = _mm_unpacklo_epi8(x_v2, x_y2);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_v2, x_y2);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_u2, x_y3);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_u2, x_y3);
			_mm_storeu_si128(pD++, x_yuv4);
		}

		for (; j4 < w; j4++) {
			mlib_s32 j = 4 * j4;
			mlib_s32 j1 = j + 1;
			mlib_s32 j2 = j + 2;
			mlib_s32 j3 = j + 3;


			val0 = (u[i * uvlb + j4]) | (v[i * uvlb + j4] << 16);
			val1 = (y[i * ylb + j] << 8) | (y[i * ylb + j1] << 24);
			val2 = (y[i * ylb + j2] << 8) | (y[i * ylb + j3] << 24);
			uyvy[i * dlb + 2 * j4 ] = val0 | val1;
			uyvy[i * dlb + 2 * j4 + 1] = val0 | val2;
		}

	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV422seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	__m128i x_y, x_u, x_v, x_y1;
	__m128i *px_u, *px_v, *px_y, *pD;
	__m128i x_yuv1, x_yuv2, x_yuv3, x_yuv4;
	__m128i x_u1, x_u2, x_v1, x_v2;
	mlib_s32 i, j2, val0, val1;

	mlib_u16 ld;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++) {
		px_u = (__m128i *)&u[i * uvlb];
		px_v = (__m128i *)&v[i * uvlb];
		px_y = (__m128i *)&y[i * ylb];
		pD = (__m128i *)&uyvy[i * dlb];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j2 = 0; j2 < w - 16; j2 += 16) {
			x_u = _mm_loadu_si128(px_u++);
			x_v = _mm_loadu_si128(px_v++);
			x_y = _mm_loadu_si128(px_y++);
			x_y1 = _mm_loadu_si128(px_y++);
			x_u1 = _mm_unpacklo_epi8(x_u, x_v);
			x_u2 = _mm_unpackhi_epi8(x_u, x_v);
			x_yuv1 = _mm_unpacklo_epi8(x_u1, x_y);
			_mm_storeu_si128(pD++, x_yuv1);
			x_yuv2 = _mm_unpackhi_epi8(x_u1, x_y);
			_mm_storeu_si128(pD++, x_yuv2);
			x_yuv3 = _mm_unpacklo_epi8(x_u2, x_y1);
			_mm_storeu_si128(pD++, x_yuv3);
			x_yuv4 = _mm_unpackhi_epi8(x_u2, x_y1);
			_mm_storeu_si128(pD++, x_yuv4);
		}

			for (; j2 < w; j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j1 = j + 1;

				val0 = (u[i * uvlb + j2]) |
					(v[i * uvlb + j2] << 16);
				ld = *((mlib_u16 *)(y + i * ylb + j));
				val1 = (ld & ~0xff) | (ld << 24);
				val1 = (y[i * ylb + j] << 8) |
					(y[i * ylb + j1] << 24);

				uyvy[i * dlb + j2 ] = val0 | val1;
			}

	}
}

/* *********************************************************** */
