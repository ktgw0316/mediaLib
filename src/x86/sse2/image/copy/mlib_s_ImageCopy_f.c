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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageCopy_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageCopy_a64        - 1-D, Aligned8, size 8x
 *      mlib_s_ImageCopy_a128       - 1-D, Aligned16, size 16x
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      sp       pointer to source image data
 *      dp       pointer to destination image data
 *      size     size in 8-bytes, bytes, or SHORTs
 *      width    image width in 8-bytes
 *      height   image height in lines
 *      stride   source image line stride in 8-bytes
 *      dstride  destination image line stride in 8-bytes
 *      s_offset source image line bit offset
 *      d_offset destination image line bit offset
 *
 * DESCRIPTION
 *      Direct copy from one image to another -- VIS version low level
 *      functions.
 *
 * NOTE
 *      These functions are separated from mlib_s_ImageCopy.c for loop
 *      unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_s_ImageCopy_f.h>

#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else	/* __SUNPRO_C */
#include <emmintrin.h>
#endif	/* __SUNPRO_C */

/* *********************************************************** */
/*
 * Either source or destination image data are not 1-d vectors, but
 * they are 8-byte aligned. And stride and width are in 8-bytes.
 */

void
mlib_s_ImageCopy_a64(
    mlib_u8 *sp,
    mlib_u8 *dp,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 dstride)
{
/* 8-byte aligned pointer for line */
	mlib_d64 *spl, *sps;

/* 8-byte aligned pointer for line */
	mlib_d64 *dpl, *dps;

/* indices for x, y */
	mlib_s32 i, j, width64, stride64, dstride64;
	width64 = width >> 3;
	stride64 = stride >> 3;
	dstride64 = dstride >> 3;


	sps = spl = (mlib_d64 *)sp;
	dps = dpl = (mlib_d64 *)dp;
/* row loop */
	for (j = 0; j < height; j++) {
/* 8-byte column loop */
#pragma pipeloop(0)
		for (i = 0; i < width64; i++) {
			(*dpl++) = (*spl++);
		}

		spl = sps += stride64;
		dpl = dps += dstride64;
	}
}

/* *********************************************************** */

/*
 * Either source or destination image data are not 1-d vectors, but
 * they are 16-byte aligned. And stride and width are in 8-bytes.
 */

void
mlib_s_ImageCopy_a128(
    mlib_u8 *sp,
    mlib_u8 *dp,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 dstride)
{
/* 8-byte aligned pointer for line */
	__m128i *spl, *sps;

/* 8-byte aligned pointer for line */
	__m128i *dpl, *dps;

/* indices for x, y */
	mlib_s32 i, j, width128, stride128, dstride128;

	width128 = width >> 4;
	stride128 = stride >> 4;
	dstride128 = dstride >> 4;


	sps = spl = (__m128i *)sp;
	dps = dpl = (__m128i *)dp;

/* row loop */
	for (j = 0; j < height; j++) {
/* 8-byte column loop */
#pragma pipeloop(0)
		for (i = 0; i < width128; i++) {
			(*dpl++) = (*spl++);
		}

		spl = sps += stride128;
		dpl = dps += dstride128;
	}
}
/* *********************************************************** */
