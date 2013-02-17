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

#pragma ident	"@(#)mlib_ImageChannelMerge_D64.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_D64
 *      mlib_ImageChannelMerge3_D64
 *      mlib_ImageChannelMerge4_D64
 *
 * DESCRIPTION
 *      Internal functions.
 */

#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge2_D64(
    mlib_d64 *dst_d64_0,
    const mlib_d64 *src_d64_0,
    const mlib_d64 *src_d64_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	mlib_s32 i, j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			dst_d64_0[2 * i + 0] = src_d64_0[i];
			dst_d64_0[2 * i + 1] = src_d64_1[i];
		}

		dst_d64_0 += dst_stride;
		src_d64_0 += src0_stride;
		src_d64_1 += src1_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge3_D64(
    mlib_d64 *dst_d64_0,
    const mlib_d64 *src_d64_0,
    const mlib_d64 *src_d64_1,
    const mlib_d64 *src_d64_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride)
{
	mlib_s32 i, j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			dst_d64_0[3 * i + 0] = src_d64_0[i];
			dst_d64_0[3 * i + 1] = src_d64_1[i];
			dst_d64_0[3 * i + 2] = src_d64_2[i];
		}

		dst_d64_0 += dst_stride;
		src_d64_0 += src0_stride;
		src_d64_1 += src1_stride;
		src_d64_2 += src2_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_D64(
    mlib_d64 *dst_d64_0,
    const mlib_d64 *src_d64_0,
    const mlib_d64 *src_d64_1,
    const mlib_d64 *src_d64_2,
    const mlib_d64 *src_d64_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride)
{
	mlib_s32 i, j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			dst_d64_0[4 * i + 0] = src_d64_0[i];
			dst_d64_0[4 * i + 1] = src_d64_1[i];
			dst_d64_0[4 * i + 2] = src_d64_2[i];
			dst_d64_0[4 * i + 3] = src_d64_3[i];
		}

		dst_d64_0 += dst_stride;
		src_d64_0 += src0_stride;
		src_d64_1 += src1_stride;
		src_d64_2 += src2_stride;
		src_d64_3 += src3_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
