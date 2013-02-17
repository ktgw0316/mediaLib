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

#pragma ident	"@(#)mlib_c_VideoColorAVDYUV444.c	9.2	07/10/09 SMI"

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
	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, yuv += slb, abgr += dlb, a_array += alb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	} else {
		for (i = 0; i < h; i++, yuv += slb, abgr += dlb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, yuv += slb, argb += dlb, a_array += alb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	} else {
		for (i = 0; i < h; i++, yuv += slb, argb += dlb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, abgr +=
			dlb, a_array += slb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	} else {
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, abgr +=
			dlb, a_array += slb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, argb +=
			dlb, a_array += slb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	} else {
		for (i = 0; i < h;
			i++, y += slb, u += slb, v += slb, argb +=
			dlb, a_array += slb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, uyv += slb, abgr += dlb, a_array += alb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	} else {
		for (i = 0; i < h; i++, uyv += slb, abgr += dlb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	mlib_s32 i, j;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h; i++, uyv += slb, argb += dlb, a_array += alb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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
	} else {
		for (i = 0; i < h; i++, uyv += slb, argb += dlb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
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

/* *********************************************************** */
