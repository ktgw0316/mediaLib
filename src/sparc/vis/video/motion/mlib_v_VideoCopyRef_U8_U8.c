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

#pragma ident	"@(#)mlib_v_VideoCopyRef_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRef_U8_U8_wxh,
 *      mlib_VideoCopyRef_U8_U8      - Copies a wxh block from reference block
 *                                     to current block
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoCopyRef_U8_U8(mlib_u8       *curr_block,
 *                                          const mlib_u8 *ref_block,
 *                                          mlib_s32      width,
 *                                          mlib_s32      height,
 *                                          mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_U8_U8_16x16(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_U8_U8_16x8(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_U8_U8_8x16(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_U8_U8_8x4(mlib_u8       *curr_block,
 *                                              const mlib_u8 *ref_block,
 *                                              mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRef_U8_U8_8x8(mlib_u8       *curr_block,
 *                                              const mlib_u8 *ref_block,
 *                                              mlib_s32      stride);
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      stride          Stride in bytes between adjacent rows in both current
 *                      and reference block, must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRef_U8_U8 = __mlib_VideoCopyRef_U8_U8
#pragma weak mlib_VideoCopyRef_U8_U8_16x16 = __mlib_VideoCopyRef_U8_U8_16x16
#pragma weak mlib_VideoCopyRef_U8_U8_16x8 = __mlib_VideoCopyRef_U8_U8_16x8
#pragma weak mlib_VideoCopyRef_U8_U8_8x16 = __mlib_VideoCopyRef_U8_U8_8x16
#pragma weak mlib_VideoCopyRef_U8_U8_8x4 = __mlib_VideoCopyRef_U8_U8_8x4
#pragma weak mlib_VideoCopyRef_U8_U8_8x8 = __mlib_VideoCopyRef_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRef_U8_U8) mlib_VideoCopyRef_U8_U8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8")));
__typeof__(__mlib_VideoCopyRef_U8_U8_16x16) mlib_VideoCopyRef_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_16x16")));
__typeof__(__mlib_VideoCopyRef_U8_U8_16x8) mlib_VideoCopyRef_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_16x8")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x16) mlib_VideoCopyRef_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x16")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x4) mlib_VideoCopyRef_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x4")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x8) mlib_VideoCopyRef_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, *sd, *dd;
	mlib_s32 height;

	sd = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	dd = (mlib_d64 *)curr_block;

	height = 8;

	if (ref_block == (mlib_u8 *)sd) {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			dd[0] = s0;
			dd[1] = s1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			dd[0] = s2;
			dd[1] = s3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		} while (--height);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			s2 = sd[2];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s3 = sd[0];
			s4 = sd[1];
			s5 = vis_ld_d64_nf(sd + 2);
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			dd[0] = vis_faligndata(s0, s1);
			dd[1] = vis_faligndata(s1, s2);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			dd[0] = vis_faligndata(s3, s4);
			dd[1] = vis_faligndata(s4, s5);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		} while (--height);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, *sd, *dd;
	mlib_s32 height;

	sd = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	dd = (mlib_d64 *)curr_block;

	height = 4;

	if (ref_block == (mlib_u8 *)sd) {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			dd[0] = s0;
			dd[1] = s1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			dd[0] = s2;
			dd[1] = s3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		} while (--height);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			s2 = sd[2];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s3 = sd[0];
			s4 = sd[1];
			s5 = vis_ld_d64_nf(sd + 2);
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			dd[0] = vis_faligndata(s0, s1);
			dd[1] = vis_faligndata(s1, s2);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			dd[0] = vis_faligndata(s3, s4);
			dd[1] = vis_faligndata(s4, s5);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		} while (--height);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, *sd, *dd;
	mlib_s32 height;

	sd = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	dd = (mlib_d64 *)curr_block;

	height = 4;

	if (ref_block == (mlib_u8 *)sd) {
		do {
			s0 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s1 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s2 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s3 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			*dd = s0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = s1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = s2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = s3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		} while (--height);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s6 = sd[0];
			s7 = vis_ld_d64_nf(sd + 1);
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			*dd = vis_faligndata(s0, s1);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = vis_faligndata(s2, s3);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = vis_faligndata(s4, s5);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = vis_faligndata(s6, s7);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		} while (--height);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, *sd, *dd;
	mlib_s32 height;

	sd = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	dd = (mlib_d64 *)curr_block;

	if (!((mlib_s32)ref_block & 7)) {
#pragma pipeloop(0)
		for (height = 0; height < 2; ++height) {
			s0 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s1 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s2 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s3 = *sd;
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			*dd = s0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = s1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = s2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = s3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		}
	} else {
#pragma pipeloop(0)
		for (height = 0; height < 2; ++height) {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			s6 = sd[0];
			s7 = vis_ld_d64_nf(sd + 1);
			sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
			*dd = vis_faligndata(s0, s1);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = vis_faligndata(s2, s3);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = vis_faligndata(s4, s5);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			*dd = vis_faligndata(s6, s7);
			dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, *sd, *dd;

	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	vis_alignaddr((void *)ref_block, 0);

	if (!((mlib_s32)ref_block & 7)) {
		s0 = *sd;
		sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
		s1 = *sd;
		sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
		s2 = *sd;
		sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
		s3 = *sd;

		*dd = s0;
		dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		*dd = s1;
		dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		*dd = s2;
		dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		*dd = s3;
	} else {
		s0 = sd[0];
		s1 = sd[1];
		sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
		s2 = sd[0];
		s3 = sd[1];
		sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
		s4 = sd[0];
		s5 = sd[1];
		sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
		s6 = sd[0];
		s7 = vis_ld_d64_nf(sd + 1);

		*dd = vis_faligndata(s0, s1);
		dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		*dd = vis_faligndata(s2, s3);
		dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		*dd = vis_faligndata(s4, s5);
		dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
		*dd = vis_faligndata(s6, s7);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, *sd, *dd;

	sd = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	dd = (mlib_d64 *)curr_block;

	if (width == 8) {
		height >>= 2;

		if (ref_block == (mlib_u8 *)sd) {
			do {
				s0 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s1 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s3 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				*dd = s0;
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				*dd = s1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				*dd = s2;
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				*dd = s3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			} while (--height);
		} else {
			do {
				s0 = sd[0];
				s1 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = sd[0];
				s3 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s4 = sd[0];
				s5 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s6 = sd[0];
				s7 = vis_ld_d64_nf(sd + 1);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				*dd = vis_faligndata(s0, s1);
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				*dd = vis_faligndata(s2, s3);
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				*dd = vis_faligndata(s4, s5);
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				*dd = vis_faligndata(s6, s7);
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			} while (--height);
		}
	} else {
/* if(width==16) */

		height >>= 1;

		if (ref_block == (mlib_u8 *)sd) {
			do {
				s0 = sd[0];
				s1 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = sd[0];
				s3 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				dd[0] = s0;
				dd[1] = s1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				dd[0] = s2;
				dd[1] = s3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			} while (--height);
		} else {
			do {
				s0 = sd[0];
				s1 = sd[1];
				s2 = sd[2];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s3 = sd[0];
				s4 = sd[1];
				s5 = vis_ld_d64_nf(sd + 2);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				dd[0] = vis_faligndata(s0, s1);
				dd[1] = vis_faligndata(s1, s2);
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
				dd[0] = vis_faligndata(s3, s4);
				dd[1] = vis_faligndata(s4, s5);
				dd = (mlib_d64 *)((mlib_u8 *)dd + stride);
			} while (--height);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
