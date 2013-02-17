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

#pragma ident	"@(#)mlib_v_VideoCopyRef_S16_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRef_S16_U8_wxh,
 *      mlib_VideoCopyRef_S16_U8    - Copies a wxh block from reference
 *                                    block to current block
 *                                    non-replenishment mode
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoCopyRef_S16_U8(mlib_s16      *mc_block,
 *                                           const mlib_u8 *ref_block,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_S16_U8_16x16(mlib_s16      *mc_block,
 *                                                 const mlib_u8 *ref_block,
 *                                                 mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_S16_U8_16x8(mlib_s16      *mc_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_S16_U8_8x16(mlib_s16      *mc_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_S16_U8_8x4(mlib_s16      *mc_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_S16_U8_8x8(mlib_s16      *mc_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      stride);
 *
 * ARGUMENT
 *      mc_block        Pointer to motion-compensated reference block,
 *                      must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      stride          Stride in bytes between adjacent rows in reference
 *                      block, must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRef_S16_U8 = __mlib_VideoCopyRef_S16_U8
#pragma weak mlib_VideoCopyRef_S16_U8_16x16 = \
	__mlib_VideoCopyRef_S16_U8_16x16
#pragma weak mlib_VideoCopyRef_S16_U8_16x8 = __mlib_VideoCopyRef_S16_U8_16x8
#pragma weak mlib_VideoCopyRef_S16_U8_8x16 = __mlib_VideoCopyRef_S16_U8_8x16
#pragma weak mlib_VideoCopyRef_S16_U8_8x4 = __mlib_VideoCopyRef_S16_U8_8x4
#pragma weak mlib_VideoCopyRef_S16_U8_8x8 = __mlib_VideoCopyRef_S16_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRef_S16_U8) mlib_VideoCopyRef_S16_U8
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8")));
__typeof__(__mlib_VideoCopyRef_S16_U8_16x16) mlib_VideoCopyRef_S16_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_16x16")));
__typeof__(__mlib_VideoCopyRef_S16_U8_16x8) mlib_VideoCopyRef_S16_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_16x8")));
__typeof__(__mlib_VideoCopyRef_S16_U8_8x16) mlib_VideoCopyRef_S16_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_8x16")));
__typeof__(__mlib_VideoCopyRef_S16_U8_8x4) mlib_VideoCopyRef_S16_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_8x4")));
__typeof__(__mlib_VideoCopyRef_S16_U8_8x8) mlib_VideoCopyRef_S16_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPY16(height)                              \
	for (y = 0; y < height; y++) {                          \
	    s1 = sp1[0];                                        \
	    s2 = sp1[1];                                        \
	    s3 = vis_ld_d64_nf(sp1 + 2);                        \
	    sd = vis_faligndata(s1, s2);                        \
	    *(dd++) = vis_fpmerge(fzero, vis_read_hi(sd));      \
	    *(dd++) = vis_fpmerge(fzero, vis_read_lo(sd));      \
	    sd = vis_faligndata(s2, s3);                        \
	    *(dd++) = vis_fpmerge(fzero, vis_read_hi(sd));      \
	    *(dd++) = vis_fpmerge(fzero, vis_read_lo(sd));      \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);        \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPY8(height)                               \
	for (y = 0; y < height; y++) {                          \
	    s1 = sp1[0];                                        \
	    s2 = vis_ld_d64_nf(sp1 + 1);                        \
	    sd = vis_faligndata(s1, s2);                        \
	    *(dd++) = vis_fpmerge(fzero, vis_read_hi(sd));      \
	    *(dd++) = vis_fpmerge(fzero, vis_read_lo(sd));      \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);        \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_16x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, sd, s1, s2, s3;
	mlib_f32 fzero = vis_fzeros();

	dd = (mlib_d64 *)mc_block;

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_16x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, sd, s1, s2, s3;
	mlib_f32 fzero = vis_fzeros();

	dd = (mlib_d64 *)mc_block;

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_8x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, sd, s1, s2;
	mlib_f32 fzero = vis_fzeros();

	dd = (mlib_d64 *)mc_block;

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_8x4(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, sd, s1, s2;
	mlib_f32 fzero = vis_fzeros();

	dd = (mlib_d64 *)mc_block;

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_8x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, sd, s1, s2;
	mlib_f32 fzero = vis_fzeros();

	dd = (mlib_d64 *)mc_block;

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, s1, s2, s3, sd;
	mlib_f32 fzero = vis_fzeros();

	dd = (mlib_d64 *)mc_block;
	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

	if (width == 8) {
#pragma pipeloop(0)
		MLIB_V_VIDEOCOPY8(height);
	} else {
/* ( width == 16) */

#pragma pipeloop(0)
		MLIB_V_VIDEOCOPY16(height);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
