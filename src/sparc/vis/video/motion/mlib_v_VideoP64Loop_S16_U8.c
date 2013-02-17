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

#pragma ident	"@(#)mlib_v_VideoP64Loop_S16_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoP64Loop_S16_U8 - Applies a 2-dimensional 3x3 spatial
 *                                 filter on the reference block
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoP64Loop_S16_U8(mlib_s16      *mc_block,
 *                                           const mlib_u8 *ref_block,
 *                                           mlib_s32      stride);
 *
 * ARGUMENT
 *      mc_block   Pointer to motion-compensated reference block,
 *                 must be 8-byte aligned
 *      ref_block  Pointer to reference block
 *      stride     Stride in bytes between adjacent rows in reference
 *                 block, must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoP64Loop_S16_U8 = __mlib_VideoP64Loop_S16_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoP64Loop_S16_U8) mlib_VideoP64Loop_S16_U8
	__attribute__((weak, alias("__mlib_VideoP64Loop_S16_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	XFILTERL(rc, dst1, dst2)                                           \
	rl = vis_faligndata(rc, rc);                                       \
	rl = vis_fand(rl, dbmask1);                                        \
	tmp1 = vis_fmul8x16(fcl, vis_fmul8x16au(vis_read_hi(rc), fmul));   \
	tmp2 = vis_fmul8x16(fcr, vis_fmul8x16au(vis_read_lo(rc), fmul));   \
	rc = vis_fand(rc, dbmask2);                                        \
	dst1 = vis_fpadd16(tmp1, vis_fmul8x16al(vis_read_hi(rl), fmul));   \
	dst2 = vis_fpadd16(tmp2, vis_fmul8x16al(vis_read_lo(rl), fmul))

/* *********************************************************** */

#define	XFILTERR(rc, dst1, dst2)                                           \
	rc = vis_faligndata(rc, rc);                                       \
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
	0x40404040, 0x10101010,
	0x08000100, 0,
};

/* *********************************************************** */

mlib_status
__mlib_VideoP64Loop_S16_U8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 a0, a1, a2, a3, a4, a5, a6, a7;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;
	mlib_d64 r8, r9, r10, r11, r12, r13, r14, r15;
	mlib_d64 *dd, *sp;
	mlib_d64 dbmask1, dbmask2;
	mlib_f32 fcl, fcr, fmul, fybrd, fycnt;
	mlib_d64 rl, tmp1, tmp2;

	dbmask1 = ((mlib_d64 *)mlib_looptab)[0];
	dbmask2 = ((mlib_d64 *)mlib_looptab)[1];
	fcl = ((mlib_f32 *)mlib_looptab)[4];
	fcr = ((mlib_f32 *)mlib_looptab)[5];
	fybrd = ((mlib_f32 *)mlib_looptab)[6];
	fycnt = ((mlib_f32 *)mlib_looptab)[7];
	fmul = ((mlib_f32 *)mlib_looptab)[8];

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

	vis_alignaddr((void *)0, 7);
	XFILTERL(a0, r0, r1);
	XFILTERL(a1, r2, r3);
	XFILTERL(a2, r4, r5);
	XFILTERL(a3, r6, r7);
	XFILTERL(a4, r8, r9);
	XFILTERL(a5, r10, r11);
	XFILTERL(a6, r12, r13);
	XFILTERL(a7, r14, r15);

	vis_alignaddr((void *)0, 1);
	XFILTERR(a0, r0, r1);
	XFILTERR(a1, r2, r3);
	XFILTERR(a2, r4, r5);
	XFILTERR(a3, r6, r7);
	XFILTERR(a4, r8, r9);
	XFILTERR(a5, r10, r11);
	XFILTERR(a6, r12, r13);
	XFILTERR(a7, r14, r15);

	dd = (mlib_d64 *)mc_block;
	dd[0] = vis_fmul8x16(fybrd, r0);
	dd[1] = vis_fmul8x16(fybrd, r1);

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

	dd[2] = vis_fmul8x16(fycnt, vis_fpadd16(r0, r2));
	dd[3] = vis_fmul8x16(fycnt, vis_fpadd16(r1, r3));
	dd[4] = vis_fmul8x16(fycnt, vis_fpadd16(r2, r4));
	dd[5] = vis_fmul8x16(fycnt, vis_fpadd16(r3, r5));
	dd[6] = vis_fmul8x16(fycnt, vis_fpadd16(r4, r6));
	dd[7] = vis_fmul8x16(fycnt, vis_fpadd16(r5, r7));
	dd[8] = vis_fmul8x16(fycnt, vis_fpadd16(r6, r8));
	dd[9] = vis_fmul8x16(fycnt, vis_fpadd16(r7, r9));
	dd[10] = vis_fmul8x16(fycnt, vis_fpadd16(r8, r10));
	dd[11] = vis_fmul8x16(fycnt, vis_fpadd16(r9, r11));
	dd[12] = vis_fmul8x16(fycnt, vis_fpadd16(r10, r12));
	dd[13] = vis_fmul8x16(fycnt, vis_fpadd16(r11, r13));

	dd[14] = vis_fmul8x16(fybrd, r14);
	dd[15] = vis_fmul8x16(fybrd, r15);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
