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

#pragma ident	"@(#)mlib_v_VideoP64Loop_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoP64Loop_U8_U8 - Applies a 2-dimensional 3x3 spatial
 *                                filter on the reference block
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoP64Loop_U8_U8(mlib_u8       *curr_block,
 *                                          const mlib_u8 *ref_block,
 *                                          mlib_s32      stride);
 *
 * ARGUMENT
 *      curr_block Pointer to current block, must be 8-byte aligned
 *      ref_block  Pointer to reference block
 *      stride     Stride in bytes between adjacent rows in both
 *                 current and reference block, must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoP64Loop_U8_U8 = __mlib_VideoP64Loop_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoP64Loop_U8_U8) mlib_VideoP64Loop_U8_U8
	__attribute__((weak, alias("__mlib_VideoP64Loop_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	XFILTERL1(rc, dst1, dst2)                                          \
	rl = vis_bshuffle(rc, vis_fzero());                                \
	tmp1 = vis_fmul8x16(fcl, vis_fmul8x16au(vis_read_hi(rc), fmul));   \
	tmp2 = vis_fmul8x16(fcr, vis_fmul8x16au(vis_read_lo(rc), fmul));   \
	rc = vis_fand(rc, dbmask2);                                        \
	dst1 = vis_fmul8x16al(vis_read_hi(rl), fmul);                      \
	dst2 = vis_fmul8x16al(vis_read_lo(rl), fmul);                      \
	dst1 = vis_fpadd16(dst1, rnd2);                                    \
	dst2 = vis_fpadd16(dst2, rnd2);                                    \
	dst1 = vis_fpadd16(tmp1, dst1);                                    \
	dst2 = vis_fpadd16(tmp2, dst2)

/* *********************************************************** */

#define	XFILTERL2(rc, dst1, dst2)                                          \
	rl = vis_bshuffle(rc, vis_fzero());                                \
	tmp1 = vis_fmul8x16(fcl, vis_fmul8x16au(vis_read_hi(rc), fmul));   \
	tmp2 = vis_fmul8x16(fcr, vis_fmul8x16au(vis_read_lo(rc), fmul));   \
	dst1 = vis_fmul8x16al(vis_read_hi(rl), fmul);                      \
	dst2 = vis_fmul8x16al(vis_read_lo(rl), fmul);                      \
	dst1 = vis_fpadd16(dst1, rnd2);                                    \
	dst2 = vis_fpadd16(dst2, rnd2);                                    \
	dst1 = vis_fpadd16(tmp1, dst1);                                    \
	dst2 = vis_fpadd16(tmp2, dst2)

/* *********************************************************** */

#define	XFILTERR1(rc, dst1, dst2)                                          \
	rc = vis_faligndata(rc, rc);                                       \
	dst1 = vis_fpadd16(dst1, vis_fmul8x16al(vis_read_hi(rc), fmul));   \
	dst2 = vis_fpadd16(dst2, vis_fmul8x16al(vis_read_lo(rc), fmul))

/* *********************************************************** */

#define	XFILTERR2(rc, dst1, dst2)                                          \
	rc = vis_bshuffle(rc, vis_fzero());                                \
	dst1 = vis_fpadd16(dst1, vis_fmul8x16al(vis_read_hi(rc), fmul));   \
	dst2 = vis_fpadd16(dst2, vis_fmul8x16al(vis_read_lo(rc), fmul))

/* *********************************************************** */

#define	LDNA(dst)                                               \
	tmp1 = sp[0];                                           \
	tmp2 = sp[1];                                           \
	dst = vis_faligndata(tmp1, tmp2)

/* *********************************************************** */

#define	LDNA_NF(dst)                                            \
	tmp1 = sp[0];                                           \
	tmp2 = vis_ld_d64_nf(sp + 1);                           \
	dst = vis_faligndata(tmp1, tmp2)

/* *********************************************************** */

#pragma align 8(mlib_looptab)

static const mlib_u32 mlib_looptab[] = {
	0x00ffffff, 0xffffff00,
	0x0000ffff, 0xffffffff,
	0x80404040, 0x40404080,
	0x08000100, 0,
	0x00020002, 0x00020002,
};

/* *********************************************************** */

mlib_status
__mlib_VideoP64Loop_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 a0, a1, a2, a3, a4, a5, a6, a7;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;
	mlib_d64 r8, r9, r10, r11, r12, r13, r14, r15;
	mlib_d64 *sp;
	mlib_d64 dbmask2, rnd2;
	mlib_f32 fcl, fcr, fmul;
	mlib_d64 rl, tmp1, tmp2;

	dbmask2 = ((mlib_d64 *)mlib_looptab)[1];
	fcl = ((mlib_f32 *)mlib_looptab)[4];
	fcr = ((mlib_f32 *)mlib_looptab)[5];
	fmul = ((mlib_f32 *)mlib_looptab)[6];
	rnd2 = ((mlib_d64 *)mlib_looptab)[4];

	sp = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);

	LDNA(a0);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA(a1);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA(a2);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA(a3);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA(a4);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA(a5);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA(a6);
	sp = (mlib_d64 *)((mlib_u8 *)sp + stride);
	LDNA_NF(a7);

	vis_write_gsr64(((mlib_u64)0x80123458 << 32) | (5 << 3) | 1);

	XFILTERL1(a0, r0, r1);
	XFILTERL2(a1, r2, r3);
	XFILTERL2(a2, r4, r5);
	XFILTERL2(a3, r6, r7);
	XFILTERL2(a4, r8, r9);
	XFILTERL2(a5, r10, r11);
	XFILTERL2(a6, r12, r13);
	XFILTERL1(a7, r14, r15);

	XFILTERR1(a0, r0, r1);
	XFILTERR1(a7, r14, r15);

	*(mlib_d64 *)(curr_block) = vis_fpack16_pair(r0, r1);
	*(mlib_d64 *)(curr_block + 7 * stride) = vis_fpack16_pair(r14, r15);

	vis_write_gsr64(((mlib_u64)0x82345678 << 32) | (3 << 3));

	XFILTERR2(a1, r2, r3);
	XFILTERR2(a2, r4, r5);
	XFILTERR2(a3, r6, r7);
	XFILTERR2(a4, r8, r9);
	XFILTERR2(a5, r10, r11);
	XFILTERR2(a6, r12, r13);

	r0 = vis_fpadd16(r0, r2);
	r1 = vis_fpadd16(r1, r3);
	r2 = vis_fpadd16(r2, r4);
	r3 = vis_fpadd16(r3, r5);
	r4 = vis_fpadd16(r4, r6);
	r5 = vis_fpadd16(r5, r7);
	r6 = vis_fpadd16(r6, r8);
	r7 = vis_fpadd16(r7, r9);
	r8 = vis_fpadd16(r8, r10);
	r9 = vis_fpadd16(r9, r11);
	r10 = vis_fpadd16(r10, r12);
	r11 = vis_fpadd16(r11, r13);
	r12 = vis_fpadd16(r12, r14);
	r13 = vis_fpadd16(r13, r15);

	*(mlib_d64 *)(curr_block + stride) =
		vis_fpack16_pair(vis_fpadd16(r0, r2), vis_fpadd16(r1, r3));
	*(mlib_d64 *)(curr_block + 2 * stride) =
		vis_fpack16_pair(vis_fpadd16(r2, r4), vis_fpadd16(r3, r5));
	*(mlib_d64 *)(curr_block + 3 * stride) =
		vis_fpack16_pair(vis_fpadd16(r4, r6), vis_fpadd16(r5, r7));
	*(mlib_d64 *)(curr_block + 4 * stride) =
		vis_fpack16_pair(vis_fpadd16(r6, r8), vis_fpadd16(r7, r9));
	*(mlib_d64 *)(curr_block + 5 * stride) =
		vis_fpack16_pair(vis_fpadd16(r8, r10), vis_fpadd16(r9, r11));
	*(mlib_d64 *)(curr_block + 6 * stride) =
		vis_fpack16_pair(vis_fpadd16(r10, r12), vis_fpadd16(r11, r13));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
