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

#pragma ident	"@(#)mlib_v_VideoH263OvrlapMC_U8_U8.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *   mlib_VideoH263OverlappedMC_U8_U8 - Generates the 8x8 luminance prediction
 *                                      block (motion-compensated reference
 *                                      block) in the Advanced Prediction Mode
 *                                      for a H.263 decoder
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoH263OverlappedMC_U8_U8(mlib_u8 *curr_block,
 *                                                   const mlib_u8 *ref_frame,
 *                                                   mlib_s32 mch,
 *                                                   mlib_s32 mcv,
 *                                                   mlib_s32 mah,
 *                                                   mlib_s32 mav,
 *                                                   mlib_s32 mbh,
 *                                                   mlib_s32 mbv,
 *                                                   mlib_s32 mlh,
 *                                                   mlib_s32 mlv,
 *                                                   mlib_s32 mrh,
 *                                                   mlib_s32 mrv,
 *                                                   mlib_s32 curr_stride,
 *                                                   mlib_s32 ref_stride)
 *
 * ARGUMENTS
 *      curr_block  Pointer to current block, must be 8-byte aligned
 *      ref_block   Pointer to reference block in
 *                  the reference frame at the same
 *                  location as the current block in the current frame
 *      mch         Horizontal coordinate of the motion vector for
 *                  the current block
 *      mcv         Vertical coordinate of the motion vector for
 *                  the current block
 *      mah         Horizontal coordinate of the motion vector for
 *                  the block above the current block
 *      mav         Vertical coordinate of the motion vector for
 *                  the block above the current block
 *      mbh         Horizontal coordinate of the motion vector for
 *                  the block below the current block
 *      mbv         Vertical coordinate of the motion vector for
 *                  the block below the current block
 *      mlh         Horizontal coordinate of the motion vector for
 *                  the block to the left of the current block
 *      mlv         Vertical coordinate of the motion vector for
 *                  the block to the left of the current block
 *      mrh         Horizontal coordinate of the motion vector for
 *                  the block to the right of the current block
 *      mrv         Vertical coordinate of the motion vector for
 *                  the block to the right of the current block
 *      curr_stride Stride in bytes between adjacent rows in
 *                  the current block, must be a multiply of 8
 *      ref_stride  Stride in bytes between adjacent rows in
 *                  the reference block, must be a multiply of 8
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoH263OverlappedMC_U8_U8 = \
	__mlib_VideoH263OverlappedMC_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoH263OverlappedMC_U8_U8) mlib_VideoH263OverlappedMC_U8_U8
	__attribute__((weak, alias("__mlib_VideoH263OverlappedMC_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ACCADD(acc, val, cf)                                    \
	val = vis_fmul8sux16(val, denom);                       \
	val = vis_fand(val, dmask);                             \
	val = vis_fmul8x16(cf, val);                            \
	acc = vis_fpadd16(acc, val)

/* *********************************************************** */

#define	ACCSET(acc, val, cf)                                    \
	val = vis_fmul8sux16(val, denom);                       \
	val = vis_fand(val, dmask);                             \
	acc = vis_fmul8x16(cf, val)

/* *********************************************************** */

#define	ACCPUT(dst, acc, val, cf)                               \
	val = vis_fmul8sux16(val, denom);                       \
	val = vis_fand(val, dmask);                             \
	val = vis_fmul8x16(cf, val);                            \
	acc = vis_fpsub16(acc, roundreg);                       \
	dst = vis_fpadd16(acc, val)

/* *********************************************************** */

mlib_status
__mlib_VideoH263OverlappedMC_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_frame,
	mlib_s32 mch,
	mlib_s32 mcv,
	mlib_s32 mah,
	mlib_s32 mav,
	mlib_s32 mbh,
	mlib_s32 mbv,
	mlib_s32 mlh,
	mlib_s32 mlv,
	mlib_s32 mrh,
	mlib_s32 mrv,
	mlib_s32 curr_stride,
	mlib_s32 ref_stride)
{
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	mlib_d64 d10, d11, d12, d13, d14, d15;
	mlib_d64 tmp1, tmp2, tmp3, res1, res2;
	mlib_d64 roundreg;
	mlib_d64 dmask;
	mlib_d64 denom;
	mlib_f32 reg_H0_00, reg_H0_01, reg_H0_10, reg_H0_20, reg_H0_21;
	mlib_f32 reg_H1_00, reg_H1_10, reg_H1_11, reg_H1_20;
	mlib_f32 reg_H2_00, reg_H2_01, reg_H2_10, reg_H2_11;
	mlib_d64 *sd;
	const mlib_u8 *sp1, *sp2, *sp3, *sp4, *sp5;
	mlib_s32 ref_stride2 = ref_stride << 1, off;

	vis_write_gsr((4 << 3));

	dmask = vis_fexpand(vis_fones());
	denom = vis_fandnot(dmask, vis_fpadd16(dmask, dmask));

	sp1 = (ref_frame + mch + mcv * ref_stride);
	sp2 = (ref_frame + mah + mav * ref_stride);
	sp3 = (ref_frame + mlh + mlv * ref_stride);
	sp4 = (ref_frame + mrh + 8 + mrv * ref_stride);
	sp5 = (ref_frame + mbh + (mbv + 8) * ref_stride);

	reg_H0_00 = vis_to_float(0x40505050);
	reg_H0_01 = vis_to_float(0x50505040);
	reg_H0_10 = vis_to_float(0x50505050);
	reg_H0_20 = vis_to_float(0x50506060);
	reg_H0_21 = vis_to_float(0x60605050);

/*
 * central
 */
	sd = (mlib_d64 *)vis_alignaddr((void *)sp1, 0);
	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d0, tmp1, reg_H0_00);
	ACCSET(d1, tmp2, reg_H0_01);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d2, tmp1, reg_H0_10);
	ACCSET(d3, tmp2, reg_H0_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d4, tmp1, reg_H0_20);
	ACCSET(d5, tmp2, reg_H0_21);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d6, tmp1, reg_H0_20);
	ACCSET(d7, tmp2, reg_H0_21);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d8, tmp1, reg_H0_20);
	ACCSET(d9, tmp2, reg_H0_21);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d10, tmp1, reg_H0_20);
	ACCSET(d11, tmp2, reg_H0_21);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d12, tmp1, reg_H0_10);
	ACCSET(d13, tmp2, reg_H0_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = vis_ld_d64_nf(sd + 2);
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCSET(d14, tmp1, reg_H0_00);
	ACCSET(d15, tmp2, reg_H0_01);

	reg_H2_00 = vis_to_float(0x20101010);
	reg_H2_01 = vis_to_float(0x10101020);
	reg_H2_10 = vis_to_float(0x20201010);
	reg_H2_11 = vis_to_float(0x10102020);

/*
 * left
 */
	off = (mlib_addr)sp3 & 7;
	sd = (mlib_d64 *)((mlib_u8 *)sp3 - off);
	vis_write_bmask(0x11111111 * off + 0x01234567, 0);
	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d0, tmp1, reg_H2_00);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d2, tmp1, reg_H2_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d4, tmp1, reg_H2_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d6, tmp1, reg_H2_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d8, tmp1, reg_H2_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d10, tmp1, reg_H2_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d12, tmp1, reg_H2_10);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = vis_ld_d64_nf(sd + 1);
	tmp1 = vis_bshuffle(tmp1, tmp2);
	ACCADD(d14, tmp1, reg_H2_00);

/*
 * right
 */
	sd = (mlib_d64 *)vis_alignaddr((void *)sp4, 0);
	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d1, tmp1, reg_H2_01);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d3, tmp1, reg_H2_11);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d5, tmp1, reg_H2_11);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d7, tmp1, reg_H2_11);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d9, tmp1, reg_H2_11);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d11, tmp1, reg_H2_11);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d13, tmp1, reg_H2_11);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = vis_ld_d64_nf(sd + 1);
	tmp1 = vis_faligndata(tmp1, tmp2);
	ACCADD(d15, tmp1, reg_H2_01);

	reg_H1_00 = vis_to_float(0x20202020);
	reg_H1_10 = vis_to_float(0x10102020);
	reg_H1_11 = vis_to_float(0x20201010);
	reg_H1_20 = vis_to_float(0x10101010);

/*
 * above
 */

	roundreg = vis_fone();
	roundreg = vis_fpadd16(roundreg, roundreg);
	roundreg = vis_fpadd16(roundreg, roundreg);

	off = (mlib_addr)sp2 & 7;
	sd = (mlib_d64 *)((mlib_u8 *)sp2 - off);
	vis_write_bmask(0x11111111 * off + 0x01234567, 0);
	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	tmp2 = vis_bshuffle(tmp2, tmp3);
	ACCPUT(res1, d0, tmp1, reg_H1_00);
	ACCPUT(res2, d1, tmp2, reg_H1_00);
	*(mlib_d64 *)(curr_block) = vis_fpack16_pair(res1, res2);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	tmp2 = vis_bshuffle(tmp2, tmp3);
	ACCPUT(res1, d2, tmp1, reg_H1_10);
	ACCPUT(res2, d3, tmp2, reg_H1_11);
	*(mlib_d64 *)(curr_block + curr_stride) = vis_fpack16_pair(res1, res2);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_bshuffle(tmp1, tmp2);
	tmp2 = vis_bshuffle(tmp2, tmp3);
	ACCPUT(res1, d4, tmp1, reg_H1_20);
	ACCPUT(res2, d5, tmp2, reg_H1_20);
	*(mlib_d64 *)(curr_block + 2 * curr_stride) =
		vis_fpack16_pair(res1, res2);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = vis_ld_d64_nf(sd + 2);
	tmp1 = vis_bshuffle(tmp1, tmp2);
	tmp2 = vis_bshuffle(tmp2, tmp3);
	ACCPUT(res1, d6, tmp1, reg_H1_20);
	ACCPUT(res2, d7, tmp2, reg_H1_20);
	*(mlib_d64 *)(curr_block + 3 * curr_stride) =
		vis_fpack16_pair(res1, res2);

/*
 * below
 */
	sd = (mlib_d64 *)vis_alignaddr((void *)sp5, 0);
	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCPUT(res1, d8, tmp1, reg_H1_20);
	ACCPUT(res2, d9, tmp2, reg_H1_20);
	*(mlib_d64 *)(curr_block + 4 * curr_stride) =
		vis_fpack16_pair(res1, res2);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCPUT(res1, d10, tmp1, reg_H1_20);
	ACCPUT(res2, d11, tmp2, reg_H1_20);
	*(mlib_d64 *)(curr_block + 5 * curr_stride) =
		vis_fpack16_pair(res1, res2);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = sd[2];
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCPUT(res1, d12, tmp1, reg_H1_10);
	ACCPUT(res2, d13, tmp2, reg_H1_11);
	*(mlib_d64 *)(curr_block + 6 * curr_stride) =
		vis_fpack16_pair(res1, res2);
	sd = (mlib_d64 *)((mlib_u8 *)sd + ref_stride2);

	tmp1 = sd[0];
	tmp2 = sd[1];
	tmp3 = vis_ld_d64_nf(sd + 2);
	tmp1 = vis_faligndata(tmp1, tmp2);
	tmp2 = vis_faligndata(tmp2, tmp3);
	ACCPUT(res1, d14, tmp1, reg_H1_00);
	ACCPUT(res2, d15, tmp2, reg_H1_00);
	*(mlib_d64 *)(curr_block + 7 * curr_stride) =
		vis_fpack16_pair(res1, res2);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
