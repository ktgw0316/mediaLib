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

#pragma ident	"@(#)mlib_c_VideoColorAVDUYV422int.c	9.3	07/10/09 SMI"

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
	mlib_s32 i, j2, val_y0, val_y1, val_u0, val_v0;

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, uyvy += dlb) {
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
			uyvy[j2] =
				val_u0 | (val_y0 << 8) | (val_v0 << 16) |
				(val_y1 << 24);
#else /* _LITTLE_ENDIAN */
			uyvy[j2] =
				(val_u0 << 24) | (val_y0 << 16) | (val_v0 << 8)
				| val_y1;
#endif /* _LITTLE_ENDIAN */
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444int_to_UYVY422int(
	mlib_u32 *uyvy,
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

	for (i = 0; i < h; i++, yuv += slb, uyvy += dlb) {
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
			uyvy[j2] =
				val_u0 | (val_y0 << 8) | (val_v0 << 16) |
				(val_y1 << 24);
#else /* _LITTLE_ENDIAN */
			uyvy[j2] =
				(val_u0 << 24) | (val_y0 << 16) | (val_v0 << 8)
				| val_y1;
#endif /* _LITTLE_ENDIAN */
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorUYV444int_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *uyv,
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

	for (i = 0; i < h; i++, uyv += slb, yuyv += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j3 = 3 * j;

			val_y0 = uyv[j3 + 1];
			val_y1 = uyv[j3 + 4];
			val_u0 = (uyv[j3 + 0] + uyv[j3 + 3]) >> 1;
			val_v0 = (uyv[j3 + 2] + uyv[j3 + 5]) >> 1;
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
__mlib_VideoColorUYV444int_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *uyv,
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

	for (i = 0; i < h; i++, uyv += slb, uyvy += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < w; j2++) {
			mlib_s32 j = 2 * j2;
			mlib_s32 j3 = 3 * j;

			val_y0 = uyv[j3 + 1];
			val_y1 = uyv[j3 + 4];
			val_u0 = (uyv[j3 + 0] + uyv[j3 + 3]) >> 1;
			val_v0 = (uyv[j3 + 2] + uyv[j3 + 5]) >> 1;
#ifdef _LITTLE_ENDIAN
			uyvy[j2] =
				val_u0 | (val_y0 << 8) | (val_v0 << 16) |
				(val_y1 << 24);
#else /* _LITTLE_ENDIAN */
			uyvy[j2] =
				(val_u0 << 24) | (val_y0 << 16) | (val_v0 << 8)
				| val_y1;
#endif /* _LITTLE_ENDIAN */
		}
	}
}

/* *********************************************************** */
