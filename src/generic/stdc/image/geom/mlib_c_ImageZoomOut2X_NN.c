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

#pragma ident	"@(#)mlib_c_ImageZoomOut2X_NN.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X -  image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge    Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_ImageZoomOut2X.h>

/* *********************************************************** */

#define	PSN	p_src[2 * i * src_stride + 2 * j]
#define	PDN	p_dst[i * dst_stride + j]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, i2;

	for (i = 0, i2 = 0; i < dst_height; i++, i2 += 2) {
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + i2 * src_stride;
		mlib_s32 j, j2, w = dst_width;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < w; j++, j2 += 2) {
			pd[j] = ps[j2];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

#define	PSN(A)	p_src[2 * i * src_stride + 4 * j + A]
#define	PDN(A)	p_dst[i * dst_stride + 2 * j + A]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;

	if ((((mlib_addr)p_src | (mlib_addr)p_dst | src_stride | dst_stride) &
	    1) == 0) {
		mlib_ImageZoomOut2X_S16_1_Nearest((mlib_s16 *)p_src,
		    (mlib_s16 *)p_dst, src_stride / 2, dst_stride / 2,
		    dst_height, dst_width);
		return;
	}

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j2;
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 2; j += 2, j2 += 4) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

#define	PSN(A)	p_src[2 * i * src_stride + 6 * j + A]
#define	PDN(A)	p_dst[i * dst_stride + 3 * j + A]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j2;
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 3; j += 3, j2 += 6) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

#define	PSN(A)	((mlib_u32)p_src[2 * i * src_stride + 8 * j + A])
#define	PDN(A)	p_dst[i * dst_stride + 4 * j + A]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;
	mlib_u8 *pz_row;
	mlib_u32 *dx, dz;

	if ((((mlib_addr)p_src | (mlib_addr)p_dst | src_stride | dst_stride) &
	    1) == 0) {
		mlib_ImageZoomOut2X_S16_2_Nearest((mlib_s16 *)p_src,
		    (mlib_s16 *)p_dst, src_stride / 2, dst_stride / 2,
		    dst_height, dst_width);
		return;
	}

	if ((((mlib_addr)p_dst | dst_stride) & 3) == 0) {
		for (i = 0; i < dst_height; i++) {
			pz_row = p_dst + i * dst_stride;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
#ifdef _LITTLE_ENDIAN
				dz = (PSN(0) | (PSN(1) << 8) | (PSN(2) << 16) |
				    (PSN(3) << 24));
#else /* _LITTLE_ENDIAN */
				dz = ((PSN(0) << 24) | (PSN(1) << 16) | (PSN(2)
				    << 8) | PSN(3));
#endif /* _LITTLE_ENDIAN */
				((mlib_u32 *)pz_row)[0] = dz;
				pz_row += 4;
			}
		}

		return;
	}

	if ((((mlib_addr)p_src | src_stride) & 3) == 0) {
		for (i = 0; i < dst_height; i++) {
			dx = (mlib_u32 *)(p_src + 2 * i * src_stride);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				mlib_s32 a0 = *dx;

#ifdef _LITTLE_ENDIAN
				PDN(3) = (a0 >> 24);
				PDN(2) = (a0 >> 16);
				PDN(1) = (a0 >> 8);
				PDN(0) = a0;
#else /* _LITTLE_ENDIAN */
				PDN(3) = a0;
				PDN(2) = (a0 >> 8);
				PDN(1) = (a0 >> 16);
				PDN(0) = (a0 >> 24);
#endif /* _LITTLE_ENDIAN */
				dx += 2;
			}
		}

		return;
	}

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j2;
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 4; j += 4, j2 += 8) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
			pd[j + 3] = ps[j2 + 3];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_1_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width; j++, j2 += 2) {
			pd[j] = ps[j2];
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	if ((((mlib_addr)p_src | (mlib_addr)p_dst | src_stride | dst_stride) &
	    3) == 0) {
		for (i = 0; i < dst_height; i++) {
			mlib_s32 j, j2;
			mlib_s32 *ps = (mlib_s32 *)(p_src + 2 * i * src_stride);
			mlib_s32 *pd = (mlib_s32 *)(p_dst + i * dst_stride);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0, j2 = 0; j < dst_width; j++, j2 += 2) {
				pd[j] = ps[j2];
			}
		}

		return;
	}

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 2; j += 2, j2 += 4) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_3_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 3; j += 3, j2 += 6) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

#ifndef	_NO_LONGLONG

	if ((((mlib_addr)p_src | (mlib_addr)p_dst | src_stride | dst_stride) &
	    7) == 0) {
		for (i = 0; i < dst_height; i++) {
			mlib_s32 j, j2;
			mlib_s64 *ps = (mlib_s64 *)(p_src + 2 * i * src_stride);
			mlib_s64 *pd = (mlib_s64 *)(p_dst + i * dst_stride);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0, j2 = 0; j < dst_width; j++, j2 += 2)
				pd[j] = ps[j2];
		}

		return;
	}
#endif /* _NO_LONGLONG */

	if ((((mlib_addr)p_src | (mlib_addr)p_dst | src_stride | dst_stride) &
	    3) == 0) {
		for (i = 0; i < dst_height; i++) {
			mlib_s32 j, j2;
			mlib_s32 *ps = (mlib_s32 *)(p_src + 2 * i * src_stride);
			mlib_s32 *pd = (mlib_s32 *)(p_dst + i * dst_stride);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = j2 = 0; j < dst_width * 2; j += 2, j2 += 4) {
				pd[j] = ps[j2];
				pd[j + 1] = ps[j2 + 1];
			}
		}

		return;
	}

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 4; j += 4, j2 += 8) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
			pd[j + 3] = ps[j2 + 3];
		}
	}
}

/* *********************************************************** */
