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

#pragma ident	"@(#)mlib_v_VideoSumAbsDiff.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoSumAbsDiff - Computes the sum of absolute differences
 *                             between the pixels in the current block and
 *                             the corresponding pixels in the reference
 *                             block
 *
 * SYNOPSIS
 *
 *      mlib_s32 mlib_v_VideoSumAbsDiff(mlib_u8       *curr_block,
 *                                      const mlib_u8 *ref_block,
 *                                      mlib_s32      width,
 *                                      mlib_s32      height,
 *                                      mlib_s32      stride);
 * ARGUMENT
 *      curr_block  Pointer to current block, must be 8-byte aligned
 *      ref_block   Pointer to reference block
 *      width       Width of the blocks, must be a multiply of 8
 *      height      Height of the blocks, must be a multiply of 4
 *      stride      Stride in bytes between adjacent rows in a block,
 *                  must be a multiply of 8
 */

#include <vis_proto.h>
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

mlib_s32
__mlib_VideoSumAbsDiff(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_d64 *sp1, *sp2, s10, s21, s20;
	mlib_d64 accum = vis_fzero(), accum1 = accum;
	mlib_d64 accum2 = accum, accum3 = accum;
	mlib_s32 acc;
	mlib_u8 *sa1, *sa2;
	mlib_s32 i, j, nx8 = width >> 3;
	union
	{
		mlib_d64 d64;
		mlib_u64 ull;
	} result;

	sa1 = curr_block;
	sa2 = (void *)ref_block;

	if ((((mlib_addr)sa2 | stride) & 7) == 0) {

/*
 * aligned case
 */
		for (j = 0; j < height; j += 4) {
			sp1 = (mlib_d64 *)sa1;
			sp2 = (mlib_d64 *)sa2;
#pragma unroll(1)
			for (i = 0; i < nx8; i++) {
				s20 = sp2[0];
				s10 = sp1[0];
				accum = vis_pdist(s10, s20, accum);
				s20 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					stride))[0];
				s10 = ((mlib_d64 *)((mlib_u8 *)sp1 +
					stride))[0];
				accum1 = vis_pdist(s10, s20, accum1);
				s20 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					2 * stride))[0];
				s10 = ((mlib_d64 *)((mlib_u8 *)sp1 +
					2 * stride))[0];
				accum2 = vis_pdist(s10, s20, accum2);
				s20 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					3 * stride))[0];
				s10 = ((mlib_d64 *)((mlib_u8 *)sp1 +
					3 * stride))[0];
				accum3 = vis_pdist(s10, s20, accum3);
				sp1++;
				sp2++;
			}

			sa1 += 4 * stride;
			sa2 += 4 * stride;
		}
	} else {
/* common case */
		sa2 = (mlib_u8 *)vis_alignaddr(sa2, 0);
		for (j = 0; j < height; j += 4) {
			sp1 = (mlib_d64 *)sa1;
			sp2 = (mlib_d64 *)sa2;
#pragma unroll(1)
			for (i = 0; i < nx8; i++) {
				s20 = sp2[0];
				s21 = sp2[1];
				s20 = vis_faligndata(s20, s21);
				s10 = sp1[0];
				accum = vis_pdist(s10, s20, accum);
				s20 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					stride))[0];
				s21 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					stride))[1];
				s20 = vis_faligndata(s20, s21);
				s10 = ((mlib_d64 *)((mlib_u8 *)sp1 +
					stride))[0];
				accum1 = vis_pdist(s10, s20, accum1);
				s20 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					2 * stride))[0];
				s21 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					2 * stride))[1];
				s20 = vis_faligndata(s20, s21);
				s10 = ((mlib_d64 *)((mlib_u8 *)sp1 +
					2 * stride))[0];
				accum2 = vis_pdist(s10, s20, accum2);
				s20 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					3 * stride))[0];
				s21 = ((mlib_d64 *)((mlib_u8 *)sp2 +
					3 * stride))[1];
				s20 = vis_faligndata(s20, s21);
				s10 = ((mlib_d64 *)((mlib_u8 *)sp1 +
					3 * stride))[0];
				accum3 = vis_pdist(s10, s20, accum3);
				sp1++;
				sp2++;
			}

			sa1 += 4 * stride;
			sa2 += 4 * stride;
		}
	}

	accum = vis_fpadd32(accum, accum1);
	accum2 = vis_fpadd32(accum2, accum3);
	accum = vis_fpadd32(accum, accum2);
	result.d64 = accum;
	acc = (mlib_s32)result.ull;

	return (acc);
}

/* *********************************************************** */
