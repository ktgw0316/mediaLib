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

#pragma ident	"@(#)mlib_c_VideoColorAVD422int.c	9.3	07/10/09 SMI"

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
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, yuyv += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			val_y0 = y[j];
			val_y1 = y[j1];
			val_u0 = (u[j] + u[j1]) >> 1;
			val_v0 = (v[j] + v[j1]) >> 1;
#ifdef _LITTLE_ENDIAN
			yuyv[j2] =
				val_y0 | (val_u0 << 8) | (val_y1 << 16) |
				(val_v0 << 24);
#else /* _LITTLE_ENDIAN */
			yuyv[j2] =
				(val_y0 << 24) | (val_u0 << 16) | (val_y1 << 8)
				| val_v0;
#endif /* _LITTLE_ENDIAN */
		}
	}
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
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, yuv += slb, yuyv += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j3 = 3 * j;

			val_y0 = yuv[j3];
			val_y1 = yuv[j3 + 3];
			val_u0 = (yuv[j3 + 1] + yuv[j3 + 4]) >> 1;
			val_v0 = (yuv[j3 + 2] + yuv[j3 + 5]) >> 1;
#ifdef _LITTLE_ENDIAN
			yuyv[j2] =
				val_y0 | (val_u0 << 8) | (val_y1 << 16) |
				(val_v0 << 24);
#else /* _LITTLE_ENDIAN */
			yuyv[j2] =
				(val_y0 << 24) | (val_u0 << 16) | (val_y1 << 8)
				| val_v0;
#endif /* _LITTLE_ENDIAN */
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
	mlib_s32 i, j2;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < h; i++) {
#ifdef _LITTLE_ENDIAN
		mlib_s32 val0 =
			(u[i * uvlb + 0] << 8) | (v[i * uvlb + 0] << 24);
		mlib_s32 val1 = (y[i * ylb + 0]) | (y[i * ylb + 1] << 16);
#else /* _LITTLE_ENDIAN */
		mlib_s32 val0 = (u[i * uvlb + 0] << 16) | (v[i * uvlb + 0]);
		mlib_s32 val1 = (y[i * ylb + 0] << 24) | (y[i * ylb + 1] << 8);
#endif /* _LITTLE_ENDIAN */

		for (j2 = 1; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			yuyv[i * dlb + j2 - 1] = val0 | val1;
#ifdef _LITTLE_ENDIAN
			val0 = (u[i * uvlb + j2] << 8) | (v[i * uvlb +
				j2] << 24);
			val1 = (y[i * ylb + j]) | (y[i * ylb + j1] << 16);
#else /* _LITTLE_ENDIAN */
			val0 = (u[i * uvlb + j2] << 16) | (v[i * uvlb + j2]);
			val1 = (y[i * ylb + j] << 24) | (y[i * ylb + j1] << 8);
#endif /* _LITTLE_ENDIAN */
		}

		yuyv[i * dlb + j2 - 1] = val0 | val1;
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
	mlib_s32 i, j4;

	dlb >>= 2;
	w >>= 2;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < h; i++) {
#ifdef _LITTLE_ENDIAN
		mlib_s32 val0 =
			(u[i * uvlb + 0] << 8) | (v[i * uvlb + 0] << 24);
		mlib_s32 val1 = (y[i * ylb + 0]) | (y[i * ylb + 1] << 16);
		mlib_s32 val2 = (y[i * ylb + 2]) | (y[i * ylb + 3] << 16);
#else /* _LITTLE_ENDIAN */
		mlib_s32 val0 = (u[i * uvlb + 0] << 16) | (v[i * uvlb + 0]);
		mlib_s32 val1 = (y[i * ylb + 0] << 24) | (y[i * ylb + 1] << 8);
		mlib_s32 val2 = (y[i * ylb + 2] << 24) | (y[i * ylb + 3] << 8);
#endif /* _LITTLE_ENDIAN */

		for (j4 = 1; j4 < w; j4++) {
			mlib_s32 j = 4 * j4;
			mlib_s32 j1 = j + 1;
			mlib_s32 j2 = j + 2;
			mlib_s32 j3 = j + 3;

			yuyv[i * dlb + 2 * j4 - 2] = val0 | val1;
			yuyv[i * dlb + 2 * j4 - 1] = val0 | val2;

#ifdef _LITTLE_ENDIAN
			val0 = (u[i * uvlb + j4] << 8) | (v[i * uvlb +
				j4] << 24);
			val1 = (y[i * ylb + j]) | (y[i * ylb + j1] << 16);
			val2 = (y[i * ylb + j2]) | (y[i * ylb + j3] << 16);
#else /* _LITTLE_ENDIAN */
			val0 = (u[i * uvlb + j4] << 16) | (v[i * uvlb + j4]);
			val1 = (y[i * ylb + j] << 24) | (y[i * ylb + j1] << 8);
			val2 = (y[i * ylb + j2] << 24) | (y[i * ylb + j3] << 8);
#endif /* _LITTLE_ENDIAN */
		}

		yuyv[i * dlb + 2 * j4 - 2] = val0 | val1;
		yuyv[i * dlb + 2 * j4 - 1] = val0 | val2;
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
	mlib_s32 i2, j2;

	h >>= 1;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i2 = 0; i2 < h; i2++) {
		mlib_s32 i = 2 * i2;
		mlib_s32 i1 = i + 1;
		mlib_s32 ld0 = u[i2 * uvlb + 0];
		mlib_s32 ld1 = v[i2 * uvlb + 0];
		mlib_s32 ld2 = y[i * ylb + 0];
		mlib_s32 ld3 = y[i * ylb + 1];
		mlib_s32 ld4 = y[i1 * ylb + 0];
		mlib_s32 ld5 = y[i1 * ylb + 1];
		mlib_s32 val0, val1, val2;

#ifdef _LITTLE_ENDIAN
		val0 = (ld0 << 8) | (ld1 << 24);
		val1 = (ld2) | (ld3 << 16);
		val2 = (ld4) | (ld5 << 16);
#else /* _LITTLE_ENDIAN */
		val0 = (ld0 << 16) | (ld1);
		val1 = (ld2 << 24) | (ld3 << 8);
		val2 = (ld4 << 24) | (ld5 << 8);
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 1; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			*(mlib_u32 *)((mlib_u8 *)yuyv + i * dlb + 4 * (j2 -
				1)) = val0 | val1;
			*(mlib_u32 *)((mlib_u8 *)yuyv + i1 * dlb + 4 * (j2 -
				1)) = val0 | val2;

			ld0 = u[i2 * uvlb + j2];
			ld1 = v[i2 * uvlb + j2];
			ld2 = y[i * ylb + j];
			ld3 = y[i * ylb + j1];
			ld4 = y[i1 * ylb + j];
			ld5 = y[i1 * ylb + j1];

#ifdef _LITTLE_ENDIAN
			val0 = (ld0 << 8) | (ld1 << 24);
			val1 = (ld2) | (ld3 << 16);
			val2 = (ld4) | (ld5 << 16);
#else /* _LITTLE_ENDIAN */
			val0 = (ld0 << 16) | (ld1);
			val1 = (ld2 << 24) | (ld3 << 8);
			val2 = (ld4 << 24) | (ld5 << 8);
#endif /* _LITTLE_ENDIAN */
		}

		*(mlib_u32 *)((mlib_u8 *)yuyv + i * dlb + 4 * (j2 - 1)) =
			val0 | val1;
		*(mlib_u32 *)((mlib_u8 *)yuyv + i1 * dlb + 4 * (j2 - 1)) =
			val0 | val2;
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
	mlib_s32 i2, j2;

	h >>= 1;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i2 = 0; i2 < h; i2++) {
		mlib_s32 i = 2 * i2;
		mlib_s32 i1 = i + 1;
		mlib_s32 ld0 = u[i2 * uvlb + 0];
		mlib_s32 ld1 = v[i2 * uvlb + 0];
		mlib_s32 ld2 = y[i * ylb + 0];
		mlib_s32 ld3 = y[i * ylb + 1];
		mlib_s32 ld4 = y[i1 * ylb + 0];
		mlib_s32 ld5 = y[i1 * ylb + 1];
		mlib_s32 val0, val1, val2;

#ifdef _LITTLE_ENDIAN
		val0 = (ld0) | (ld1 << 16);
		val1 = (ld2 << 8) | (ld3 << 24);
		val2 = (ld4 << 8) | (ld5 << 24);
#else /* _LITTLE_ENDIAN */
		val0 = (ld0 << 24) | (ld1 << 8);
		val1 = (ld2 << 16) | (ld3);
		val2 = (ld4 << 16) | (ld5);
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 1; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j1 = j + 1;

			*(mlib_u32 *)((mlib_u8 *)uyvy + i * dlb + 4 * (j2 -
				1)) = val0 | val1;
			*(mlib_u32 *)((mlib_u8 *)uyvy + i1 * dlb + 4 * (j2 -
				1)) = val0 | val2;

			ld0 = u[i2 * uvlb + j2];
			ld1 = v[i2 * uvlb + j2];
			ld2 = y[i * ylb + j];
			ld3 = y[i * ylb + j1];
			ld4 = y[i1 * ylb + j];
			ld5 = y[i1 * ylb + j1];

#ifdef _LITTLE_ENDIAN
			val0 = (ld0) | (ld1 << 16);
			val1 = (ld2 << 8) | (ld3 << 24);
			val2 = (ld4 << 8) | (ld5 << 24);
#else /* _LITTLE_ENDIAN */
			val0 = (ld0 << 24) | (ld1 << 8);
			val1 = (ld2 << 16) | (ld3);
			val2 = (ld4 << 16) | (ld5);
#endif /* _LITTLE_ENDIAN */
		}

		*(mlib_u32 *)((mlib_u8 *)uyvy + i * dlb + 4 * (j2 - 1)) =
			val0 | val1;
		*(mlib_u32 *)((mlib_u8 *)uyvy + i1 * dlb + 4 * (j2 - 1)) =
			val0 | val2;
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
	mlib_s32 i, j4;

	dlb >>= 2;
	w >>= 2;

	if (w == 0 || h == 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < h; i++) {
#ifdef _LITTLE_ENDIAN
		mlib_s32 val0 = (u[i * uvlb + 0]) | (v[i * uvlb + 0] << 16);
		mlib_s32 val1 = (y[i * ylb + 0] << 8) | (y[i * ylb + 1] << 24);
		mlib_s32 val2 = (y[i * ylb + 2] << 8) | (y[i * ylb + 3] << 24);
#else /* _LITTLE_ENDIAN */
		mlib_s32 val0 =
			(u[i * uvlb + 0] << 24) | (v[i * uvlb + 0] << 8);
		mlib_s32 val1 = (y[i * ylb + 0] << 16) | (y[i * ylb + 1]);
		mlib_s32 val2 = (y[i * ylb + 2] << 16) | (y[i * ylb + 3]);
#endif /* _LITTLE_ENDIAN */

		for (j4 = 1; j4 < w; j4++) {
			mlib_s32 j = 4 * j4;
			mlib_s32 j1 = j + 1;
			mlib_s32 j2 = j + 2;
			mlib_s32 j3 = j + 3;

			uyvy[i * dlb + 2 * j4 - 2] = val0 | val1;
			uyvy[i * dlb + 2 * j4 - 1] = val0 | val2;

#ifdef _LITTLE_ENDIAN
			val0 = (u[i * uvlb + j4]) | (v[i * uvlb + j4] << 16);
			val1 = (y[i * ylb + j] << 8) | (y[i * ylb + j1] << 24);
			val2 = (y[i * ylb + j2] << 8) | (y[i * ylb + j3] << 24);
#else /* _LITTLE_ENDIAN */
			val0 = (u[i * uvlb + j4] << 24) | (v[i * uvlb +
				j4] << 8);
			val1 = (y[i * ylb + j] << 16) | (y[i * ylb + j1]);
			val2 = (y[i * ylb + j2] << 16) | (y[i * ylb + j3]);
#endif /* _LITTLE_ENDIAN */
		}

		uyvy[i * dlb + 2 * j4 - 2] = val0 | val1;
		uyvy[i * dlb + 2 * j4 - 1] = val0 | val2;
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
	mlib_s32 i, j2, val0, val1;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++) {

		if (((mlib_addr)y & 1) == 0) {
			mlib_u16 ld;

#ifdef _LITTLE_ENDIAN
			val0 = (u[i * uvlb + 0]) | (v[i * uvlb + 0] << 16);
			val1 = (y[i * ylb + 0] << 8) | (y[i * ylb + 1] << 24);
#else /* _LITTLE_ENDIAN */
			val0 = (u[i * uvlb + 0] << 24) | (v[i * uvlb + 0] << 8);
			ld = *((mlib_u16 *)(y + i * ylb));
			val1 = ((ld << 8) & ~0xffff) | (ld & 0xff);
#endif /* _LITTLE_ENDIAN */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j2 = 1; j2 < w; j2++) {
				mlib_s32 j = 2 * j2;

				uyvy[i * dlb + j2 - 1] = val0 | val1;
#ifdef _LITTLE_ENDIAN
				{
					mlib_s32 j1 = j + 1;

					val0 = (u[i * uvlb +
						j2]) | (v[i * uvlb + j2] << 16);
					ld = *((mlib_u16 *)(y + i * ylb + j));
					val1 = (ld & ~0xff) | (ld << 24);
					val1 = (y[i * ylb +
						j] << 8) | (y[i * ylb +
						j1] << 24);
				}

#else /* _LITTLE_ENDIAN */
				val0 = (u[i * uvlb + j2] << 24) | (v[i * uvlb +
					j2] << 8);
				ld = *((mlib_u16 *)(y + i * ylb + j));
				val1 = ((ld << 8) & ~0xffff) | (ld & 0xff);
#endif /* _LITTLE_ENDIAN */
			}

			uyvy[i * dlb + j2 - 1] = val0 | val1;
		} else {
#ifdef _LITTLE_ENDIAN
			mlib_s32 val0 =
				(u[i * uvlb + 0]) | (v[i * uvlb + 0] << 16);
			mlib_s32 val1 =
				(y[i * ylb + 0] << 8) | (y[i * ylb + 1] << 24);
#else /* _LITTLE_ENDIAN */
			val0 = (u[i * uvlb + 0] << 24) | (v[i * uvlb + 0] << 8);
			val1 = (y[i * ylb + 0] << 16) | (y[i * ylb + 1]);
#endif /* _LITTLE_ENDIAN */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j2 = 1; j2 < w; j2++) {
				mlib_s32 j = 2 * j2;
				mlib_s32 j1 = j + 1;

				uyvy[i * dlb + j2 - 1] = val0 | val1;
#ifdef _LITTLE_ENDIAN
				val0 = (u[i * uvlb + j2]) | (v[i * uvlb +
					j2] << 16);
				val1 = (y[i * ylb + j] << 8) | (y[i * ylb +
					j1] << 24);
#else /* _LITTLE_ENDIAN */
				val0 = (u[i * uvlb + j2] << 24) | (v[i * uvlb +
					j2] << 8);
				val1 = (y[i * ylb + j] << 16) | (y[i * ylb +
					j1]);
#endif /* _LITTLE_ENDIAN */
			}

			uyvy[i * dlb + j2 - 1] = val0 | val1;
		}
	}
}

/* *********************************************************** */
