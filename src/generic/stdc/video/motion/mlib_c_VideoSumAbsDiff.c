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

#pragma ident	"@(#)mlib_c_VideoSumAbsDiff.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoSumAbsDiff - Computes the sum of absolute differences
 *                             between the pixels in the current block and
 *                             the corresponding pixels in the reference
 *                             block
 *
 * SYNOPSIS
 *    mlib_s32 mlib_VideoSumAbsDiff(mlib_u8       *curr_block,
 *                                  const mlib_u8 *ref_block,
 *                                  mlib_s32      width,
 *                                  mlib_s32      height,
 *                                  mlib_s32      stride)
 *
 * ARGUMENT
 *      curr_block  Pointer to current block, must be 8-byte aligned
 *      ref_block   Pointer to reference block
 *      width       Width of the blocks, must be a multiply of 8
 *      height      Height of the blocks, must be a multiply of 4
 *      stride      Stride in bytes between adjacent rows in a block,
 *                  must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoSumAbsDiff = __mlib_VideoSumAbsDiff

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoSumAbsDiff) mlib_VideoSumAbsDiff
	__attribute__((weak, alias("__mlib_VideoSumAbsDiff")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* *********************************************************** */

mlib_s32
__mlib_VideoSumAbsDiff(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_u8 *sa1 = curr_block;
	const mlib_u8 *sa2 = ref_block;
	mlib_s32 sd1;
	mlib_s32 accum = 0;
	mlib_s32 i, j;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j += 8) {
			sd1 = sa1[j] - sa2[j];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 1] - sa2[j + 1];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 2] - sa2[j + 2];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 3] - sa2[j + 3];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 4] - sa2[j + 4];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 5] - sa2[j + 5];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 6] - sa2[j + 6];
			accum += ABS_VALUE(sd1);
			sd1 = sa1[j + 7] - sa2[j + 7];
			accum += ABS_VALUE(sd1);
		}

		sa1 = sa1 + stride;
		sa2 = sa2 + stride;
	}

	return (accum);
}

/* *********************************************************** */
