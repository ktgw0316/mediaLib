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

#pragma ident	"@(#)mlib_ImageZoomIn2X_NN_S32.c	9.2	07/11/05"

/*
 * FUNCTION
 *  mlib_ImageZoomIn2X_S32_NN - image 2X scaling,
 *                              image type is mlib_s32,
 *                              filter is MLIB_NEAREST.
 */

#include <mlib_image.h>
#include <mlib_ImageZoomIn2X.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                              \
	MLIB_TYPE *pdst,                                              \
	const MLIB_TYPE *psrc,                                        \
	mlib_s32 src_width,                                           \
	mlib_s32 src_height,                                          \
	mlib_s32 dst_stride,                                          \
	mlib_s32 src_stride,                                          \
	mlib_s32 src_w_beg,                                           \
	mlib_s32 src_h_beg

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE)                             \
	(MLIB_TYPE*)pdst, (MLIB_TYPE*)psrc,                           \
	src_width, src_height, dst_stride, src_stride,                \
	src_w_beg, src_h_beg

/* *********************************************************** */

#define	CP1(i0, i1, k)	pdst[i0 + k] = psrc[i1 + k]

#define	CPX(i0, i1, k)	pdst[i0 + k] = pdst[i0 + nchan + k] = psrc[i1 + k]

#define	CPY(i0, i1, k)	pdst[i0 + k] = pdst[i0 + dst_stride + k] = psrc[i1 + k]

/* *********************************************************** */

#define	CP4(i0, i1, k)                                             \
	pdst[i0 + k] = pdst[i0 + nchan + k] =                      \
		pdst[i0 + dst_stride + k] =                        \
		pdst[i0 + dst_stride + nchan + k] = psrc[i1 + k]

/* *********************************************************** */

#define	CP1_1(i0, i1)	CP1(i0, i1, 0)
#define	CPX_1(i0, i1)	CPX(i0, i1, 0)

#define	CPY_1(i0, i1)	CPY(i0, i1, 0)

#define	CP4_1(i0, i1)	CP4(i0, i1, 0)

/* *********************************************************** */

#define	CP1_2(i0, i1)	CP1(i0, i1, 0); CP1(i0, i1, 1);
#define	CPX_2(i0, i1)	CPX(i0, i1, 0); CPX(i0, i1, 1);

#define	CPY_2(i0, i1)	CPY(i0, i1, 0); CPY(i0, i1, 1);

#define	CP4_2(i0, i1)	CP4(i0, i1, 0); CP4(i0, i1, 1)

/* *********************************************************** */

#define	CP1_3(i0, i1)	CP1(i0, i1, 0); CP1(i0, i1, 1); CP1(i0, i1, 2);
#define	CPX_3(i0, i1)	CPX(i0, i1, 0); CPX(i0, i1, 1); CPX(i0, i1, 2);

#define	CPY_3(i0, i1)	CPY(i0, i1, 0); CPY(i0, i1, 1); CPY(i0, i1, 2);

#define	CP4_3(i0, i1)	CP4(i0, i1, 0); CP4(i0, i1, 1); CP4(i0, i1, 2)

/* *********************************************************** */

#define	CP1_4(i0, i1)	CP1(i0, i1, 0); CP1(i0, i1, 1); \
			CP1(i0, i1, 2); CP1(i0, i1, 3)
#define	CPX_4(i0, i1)	CPX(i0, i1, 0); CPX(i0, i1, 1); \
			CPX(i0, i1, 2); CPX(i0, i1, 3)

#define	CPY_4(i0, i1)	CPY(i0, i1, 0); CPY(i0, i1, 1); \
			CPY(i0, i1, 2); CPY(i0, i1, 3)

#define	CP4_4(i0, i1)	CP4(i0, i1, 0); CP4(i0, i1, 1); \
			CP4(i0, i1, 2); CP4(i0, i1, 3)

/* *********************************************************** */

#define	FUNC_ZOOM_NN(NAME, STYPE, N, N2)                        \
	static void                                             \
	NAME(MLIB_ZOOMIN2XDEFPARAM(STYPE))                      \
	{                                                       \
	    mlib_s32 i, j, nchan = N;                           \
	                                                        \
	    if (src_w_beg & 1) {                                \
		psrc += N;                                      \
		pdst += N;                                      \
		src_width--;                                    \
	    }                                                   \
	    if (src_h_beg & 1) {                                \
	                                                        \
/*                                                              \
 * upper bound of image                                         \
 */                                                             \
		if (src_w_beg & 1) {                            \
		    CP1_##N(-N, -N);                            \
		}                                               \
		for (j = 0; j < (src_width / 2); j++) {         \
		    CPX_##N(j * N2, j * N);                     \
		}                                               \
		if (src_width & 1) {                            \
		    CP1_##N(j * N2, j * N);                     \
		}                                               \
		psrc += src_stride;                             \
		pdst += dst_stride;                             \
		src_height--;                                   \
	    }                                                   \
	    for (i = 0; i < (src_height / 2); i++) {            \
		if (src_w_beg & 1) {                            \
		    CPY_##N(-N, -N);                            \
		}                                               \
		for (j = 0; j < (src_width / 2); j++) {         \
		    CP4_##N(j * N2, j * N);                     \
		}                                               \
		if (src_width & 1) {                            \
		    CPY_##N(j * N2, j * N);                     \
		}                                               \
		psrc += src_stride;                             \
		pdst += 2 * dst_stride;                         \
	    }                                                   \
	    if (src_height & 1) {                               \
	                                                        \
/*                                                              \
 * lower bound of image                                         \
 */                                                             \
		if (src_w_beg & 1) {                            \
		    CP1_##N(-N, -N);                            \
		}                                               \
		for (j = 0; j < (src_width / 2); j++) {         \
		    CPX_##N(j * N2, j * N);                     \
		}                                               \
		if (src_width & 1) {                            \
		    CP1_##N(j * N2, j * N);                     \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

FUNC_ZOOM_NN(mlib_ImageZoomIn2X_S32_NN_1, mlib_s32,
	1,
	2)
FUNC_ZOOM_NN(
	mlib_ImageZoomIn2X_S32_NN_2,
	mlib_s32,
	2,
	4)
FUNC_ZOOM_NN(
	mlib_ImageZoomIn2X_S32_NN_3,
	mlib_s32,
	3,
	6)
FUNC_ZOOM_NN(
	mlib_ImageZoomIn2X_S32_NN_4,
	mlib_s32,
	4,
	8)

/* *********************************************************** */
    void mlib_ImageZoomIn2X_S32_NN(
	MLIB_ZOOMIN2XDEFPARAM(mlib_s32),
	mlib_s32 nchan)
{
#ifndef _NO_LONGLONG

	if ((((((mlib_s32)psrc | (mlib_s32)pdst) >> 2) | src_stride | dst_stride
		| nchan) & 1) == 0) {
		mlib_ImageZoomIn2X_D64_NN((void *)pdst, (void *)psrc, src_width,
			src_height, dst_stride / 2, src_stride / 2, src_w_beg,
			src_h_beg, nchan / 2);
		return;
	}
#endif /* _NO_LONGLONG */

	switch (nchan) {
	    case 1:
		    mlib_ImageZoomIn2X_S32_NN_1(MLIB_ZOOMIN2XCALLPARAM(void));
		    break;
	    case 2:
		    mlib_ImageZoomIn2X_S32_NN_2(MLIB_ZOOMIN2XCALLPARAM(void));
		    break;
	    case 3:
		    mlib_ImageZoomIn2X_S32_NN_3(MLIB_ZOOMIN2XCALLPARAM(void));
		    break;
	    case 4:
		    mlib_ImageZoomIn2X_S32_NN_4(MLIB_ZOOMIN2XCALLPARAM(void));
	}
}

/* *********************************************************** */
