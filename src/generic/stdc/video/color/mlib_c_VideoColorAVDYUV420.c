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

#pragma ident	"@(#)mlib_c_VideoColorAVDYUV420.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_ARGBint - color convert YUV sequential
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV420seq_to_ARGBint(mlib_u32      *argb,
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
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive rows
 *      of the output buffer ARGB use successive rows of Y and the same rows
 *      of U and V.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_ABGRint - color convert YUV sequential
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorYUV420seq_to_ABGRint(mlib_u32      *abgr,
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
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive rows
 *      of the output buffer ABGR use successive rows of Y and the same rows
 *      of U and V.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV420seq_to_ABGRint = \
	__mlib_VideoColorYUV420seq_to_ABGRint
#pragma weak mlib_VideoColorYUV420seq_to_ARGBint = \
	__mlib_VideoColorYUV420seq_to_ARGBint

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV420seq_to_ABGRint)
	mlib_VideoColorYUV420seq_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorYUV420seq_to_ABGRint")));
__typeof__(__mlib_VideoColorYUV420seq_to_ARGBint)
	mlib_VideoColorYUV420seq_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorYUV420seq_to_ARGBint")));

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
__mlib_VideoColorYUV420seq_to_ABGRint(
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
	mlib_s32 i, j;
	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	dlb >>= 2;
	w >>= 1;
	h >>= 1;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, y += 2 * aylb, u += uvlb, v += uvlb, abgr +=
			2 * dlb, a_array += 2 * aylb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd0 = (mlib_u8 *)(abgr + 2 * j);
				mlib_u8 *pd1 = (mlib_u8 *)(abgr + 2 * j + dlb);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[2 * j];
				val_y1 = y[2 * j + 1];
				val_y2 = y[2 * j + aylb];
				val_y3 = y[2 * j + aylb + 1];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd0[0] = a_array[2 * j];
				TCLAMP_U8(pd0[1], val_y0 + val_b);
				TCLAMP_U8(pd0[2], val_y0 + val_g);
				SCLAMP_U8(pd0[3], val_y0 + val_r);
				pd0[4] = a_array[2 * j + 1];
				TCLAMP_U8(pd0[5], val_y1 + val_b);
				TCLAMP_U8(pd0[6], val_y1 + val_g);
				SCLAMP_U8(pd0[7], val_y1 + val_r);

				pd1[0] = a_array[2 * j + aylb];
				TCLAMP_U8(pd1[1], val_y2 + val_b);
				TCLAMP_U8(pd1[2], val_y2 + val_g);
				SCLAMP_U8(pd1[3], val_y2 + val_r);
				pd1[4] = a_array[2 * j + aylb + 1];
				TCLAMP_U8(pd1[5], val_y3 + val_b);
				TCLAMP_U8(pd1[6], val_y3 + val_g);
				SCLAMP_U8(pd1[7], val_y3 + val_r);
			}
	} else {
		for (i = 0; i < h;
			i++, y += 2 * aylb, u += uvlb, v += uvlb, abgr +=
			2 * dlb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd0 = (mlib_u8 *)(abgr + 2 * j);
				mlib_u8 *pd1 = (mlib_u8 *)(abgr + 2 * j + dlb);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[2 * j];
				val_y1 = y[2 * j + 1];
				val_y2 = y[2 * j + aylb];
				val_y3 = y[2 * j + aylb + 1];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd0[0] = a_const;
				TCLAMP_U8(pd0[1], val_y0 + val_b);
				TCLAMP_U8(pd0[2], val_y0 + val_g);
				SCLAMP_U8(pd0[3], val_y0 + val_r);
				pd0[4] = a_const;
				TCLAMP_U8(pd0[5], val_y1 + val_b);
				TCLAMP_U8(pd0[6], val_y1 + val_g);
				SCLAMP_U8(pd0[7], val_y1 + val_r);

				pd1[0] = a_const;
				TCLAMP_U8(pd1[1], val_y2 + val_b);
				TCLAMP_U8(pd1[2], val_y2 + val_g);
				SCLAMP_U8(pd1[3], val_y2 + val_r);
				pd1[4] = a_const;
				TCLAMP_U8(pd1[5], val_y3 + val_b);
				TCLAMP_U8(pd1[6], val_y3 + val_g);
				SCLAMP_U8(pd1[7], val_y3 + val_r);
			}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_ARGBint(
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
	mlib_s32 i, j;
	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	dlb >>= 2;
	w >>= 1;
	h >>= 1;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, y += 2 * aylb, u += uvlb, v += uvlb,
			argb += 2 * dlb, a_array += 2 * aylb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd0 = (mlib_u8 *)(argb + 2 * j);
				mlib_u8 *pd1 = (mlib_u8 *)(argb + 2 * j + dlb);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[2 * j];
				val_y1 = y[2 * j + 1];
				val_y2 = y[2 * j + aylb];
				val_y3 = y[2 * j + aylb + 1];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd0[0] = a_array[2 * j];
				TCLAMP_U8(pd0[3], val_y0 + val_b);
				TCLAMP_U8(pd0[2], val_y0 + val_g);
				SCLAMP_U8(pd0[1], val_y0 + val_r);
				pd0[4] = a_array[2 * j + 1];
				TCLAMP_U8(pd0[7], val_y1 + val_b);
				TCLAMP_U8(pd0[6], val_y1 + val_g);
				SCLAMP_U8(pd0[5], val_y1 + val_r);

				pd1[0] = a_array[2 * j + aylb];
				TCLAMP_U8(pd1[3], val_y2 + val_b);
				TCLAMP_U8(pd1[2], val_y2 + val_g);
				SCLAMP_U8(pd1[1], val_y2 + val_r);
				pd1[4] = a_array[2 * j + aylb + 1];
				TCLAMP_U8(pd1[7], val_y3 + val_b);
				TCLAMP_U8(pd1[6], val_y3 + val_g);
				SCLAMP_U8(pd1[5], val_y3 + val_r);
			}
	} else {
		for (i = 0; i < h;
			i++, y += 2 * aylb, u += uvlb, v += uvlb, argb +=
			2 * dlb)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 val_y0, val_y1, val_u, val_v, val_b,
					val_g, val_r;
				mlib_s32 val_y2, val_y3;
				mlib_u8 *pd0 = (mlib_u8 *)(argb + 2 * j);
				mlib_u8 *pd1 = (mlib_u8 *)(argb + 2 * j + dlb);

				val_u = u[j];
				val_v = v[j];
				val_y0 = y[2 * j];
				val_y1 = y[2 * j + 1];
				val_y2 = y[2 * j + aylb];
				val_y3 = y[2 * j + aylb + 1];

				val_b = mlib_tBU[val_u] - BBi;
				val_g = mlib_tGU[val_u] - mlib_tGV[val_v];
				val_r = mlib_tRV[val_v] - RRi;

				val_y0 = mlib_tYY[val_y0];
				val_y1 = mlib_tYY[val_y1];
				val_y2 = mlib_tYY[val_y2];
				val_y3 = mlib_tYY[val_y3];

				pd0[0] = a_const;
				TCLAMP_U8(pd0[3], val_y0 + val_b);
				TCLAMP_U8(pd0[2], val_y0 + val_g);
				SCLAMP_U8(pd0[1], val_y0 + val_r);
				pd0[4] = a_const;
				TCLAMP_U8(pd0[7], val_y1 + val_b);
				TCLAMP_U8(pd0[6], val_y1 + val_g);
				SCLAMP_U8(pd0[5], val_y1 + val_r);

				pd1[0] = a_const;
				TCLAMP_U8(pd1[3], val_y2 + val_b);
				TCLAMP_U8(pd1[2], val_y2 + val_g);
				SCLAMP_U8(pd1[1], val_y2 + val_r);
				pd1[4] = a_const;
				TCLAMP_U8(pd1[7], val_y3 + val_b);
				TCLAMP_U8(pd1[6], val_y3 + val_g);
				SCLAMP_U8(pd1[5], val_y3 + val_r);
			}
	}
}

/* *********************************************************** */
