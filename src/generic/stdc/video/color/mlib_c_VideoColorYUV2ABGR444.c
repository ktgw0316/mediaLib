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

#pragma ident	"@(#)mlib_c_VideoColorYUV2ABGR444.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR444 - color conversion from YUV444 to ABGR
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2ABGR444(mlib_u8       *abgr,
 *                                             const mlib_u8 *y,
 *                                             const mlib_u8 *u,
 *                                             const mlib_u8 *v,
 *                                             mlib_s32      width,
 *                                             mlib_s32      height,
 *                                             mlib_s32      abgr_stride,
 *                                             mlib_s32      yuv_stride)
 *
 * ARGUMENT
 *      abgr        Pointer to the output ABGR image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image
 *      height      Height of the image
 *      abgr_stride Stride in bytes between adjacent rows in output image
 *      yuv_stride  Stride in bytes between adjacent rows input image
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR444 = __mlib_VideoColorYUV2ABGR444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR444) mlib_VideoColorYUV2ABGR444
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(dst, s)	dst = mlib_video_pClipOff256[s]

/* *********************************************************** */

#define	SCLAMP_U8(dst, s)                                       \
	{                                                       \
	    mlib_s32 v = s, mask = (v - 0xff00000) >> 31;       \
	                                                        \
	    dst = (((mlib_u32)v >> 20) | ~mask) & ~(v >> 31);   \
	}

/* *********************************************************** */

#define	MLIB_CONVERT_U8_1(pd, ps0, ps1, ps2)                    \
	{                                                       \
	    mlib_u32 x0, y0, z0;                                \
	    mlib_u32 s0;                                        \
	                                                        \
	    x0 = (*ps0++);                                      \
	    y0 = (*ps1++);                                      \
	    z0 = (*ps2++);                                      \
	    s0 = mlib_t_Yuv2rgb[x0] + mlib_t_yUv2rgb[y0] +      \
		    mlib_t_yuV2rgb[z0];                         \
	    SCLAMP_U8(pd[1], s0);                               \
	    TCLAMP_U8(pd[2], (s0 >> 10) & 0x3ff);               \
	    TCLAMP_U8(pd[3], s0 & 0x3ff);                       \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR444(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride)
{
	const mlib_u8 *sa0, *sa1, *sa2, *sl0, *sl1, *sl2;
	mlib_u8 *da, *dl;
	mlib_s32 j;

	if (rgb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (xsize <= 0 || ysize <= 0)
		return (MLIB_FAILURE);

/* 1-D vectors */
	if ((4 * xsize == rgb_stride) && (xsize == yuv_stride)) {
		xsize *= ysize;
		ysize = 1;
	}

	sl0 = sa0 = y;
	sl1 = sa1 = u;
	sl2 = sa2 = v;
	dl = da = rgb;

	for (j = 0; j < ysize; j++) {
		mlib_u8 *pend = da + 4 * xsize;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; da < pend; da += 4) {
			MLIB_CONVERT_U8_1(da, sa0, sa1, sa2);
		}

		sl0 = sa0 = sl0 + yuv_stride;
		sl1 = sa1 = sl1 + yuv_stride;
		sl2 = sa2 = sl2 + yuv_stride;
		dl = da = dl + rgb_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
