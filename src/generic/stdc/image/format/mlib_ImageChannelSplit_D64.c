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

#pragma ident	"@(#)mlib_ImageChannelSplit_D64.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_D64
 *      mlib_ImageChannelSplit3_D64
 *      mlib_ImageChannelSplit4_D64
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_D64(
    mlib_d64 *dst_d64_0,
    mlib_d64 *dst_d64_1,
    const mlib_d64 *src_d64_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_stride,
    mlib_s32 dst1_stride,
    mlib_s32 src0_stride)
{
	mlib_s32 j, i;

	dst0_stride >>= 3;
	dst1_stride >>= 3;
	src0_stride >>= 3;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_d64_0[i] = src_d64_0[2 * i + 0];
			dst_d64_1[i] = src_d64_0[2 * i + 1];
		}

		dst_d64_0 += dst0_stride;
		dst_d64_1 += dst1_stride;
		src_d64_0 += src0_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_D64(
    mlib_d64 *dst_d64_0,
    mlib_d64 *dst_d64_1,
    mlib_d64 *dst_d64_2,
    const mlib_d64 *src_d64_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_stride,
    mlib_s32 dst1_stride,
    mlib_s32 dst2_stride,
    mlib_s32 src0_stride)
{
	mlib_s32 j, i;

	dst0_stride >>= 3;
	dst1_stride >>= 3;
	dst2_stride >>= 3;
	src0_stride >>= 3;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_d64_0[i] = src_d64_0[3 * i + 0];
			dst_d64_1[i] = src_d64_0[3 * i + 1];
			dst_d64_2[i] = src_d64_0[3 * i + 2];
		}

		dst_d64_0 += dst0_stride;
		dst_d64_1 += dst1_stride;
		dst_d64_2 += dst2_stride;
		src_d64_0 += src0_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_D64(
    mlib_d64 *dst_d64_0,
    mlib_d64 *dst_d64_1,
    mlib_d64 *dst_d64_2,
    mlib_d64 *dst_d64_3,
    const mlib_d64 *src_d64_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_stride,
    mlib_s32 dst1_stride,
    mlib_s32 dst2_stride,
    mlib_s32 dst3_stride,
    mlib_s32 src0_stride)
{
	mlib_s32 j, i;

	dst0_stride >>= 3;
	dst1_stride >>= 3;
	dst2_stride >>= 3;
	dst3_stride >>= 3;
	src0_stride >>= 3;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_d64_0[i] = src_d64_0[4 * i + 0];
			dst_d64_1[i] = src_d64_0[4 * i + 1];
			dst_d64_2[i] = src_d64_0[4 * i + 2];
			dst_d64_3[i] = src_d64_0[4 * i + 3];
		}

		dst_d64_0 += dst0_stride;
		dst_d64_1 += dst1_stride;
		dst_d64_2 += dst2_stride;
		dst_d64_3 += dst3_stride;
		src_d64_0 += src0_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
