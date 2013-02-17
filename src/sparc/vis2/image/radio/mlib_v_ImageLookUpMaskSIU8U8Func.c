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

#pragma ident	"@(#)mlib_v_ImageLookUpMaskSIU8U8Func.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpMaskFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask);

static void mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask);

static void mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask);

static void mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask);

static void mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_U8_U8_4_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_f32 *table,
    mlib_s32 mask);

/* *********************************************************** */

#define	VIS_LD_U16_I(X, Y)	vis_ld_u16_i((void *)(X), (Y))

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, acc0;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

/* destination data */
	mlib_d64 acc1;

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + 2 * xsize - 1;

	vis_alignaddr((void *)0, 6);

	if (xsize >= 4) {

		s0 = sa[0];
		sa++;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4, sa++) {
			t3 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
			t2 = VIS_LD_U16_I(table, (s0 >> 7) & 0x1FE);
			t1 = VIS_LD_U16_I(table, (s0 >> 15) & 0x1FE);
			t0 = VIS_LD_U16_I(table, (s0 >> 23) & 0x1FE);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = sa[0];
			vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
		}

		t3 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
		t2 = VIS_LD_U16_I(table, (s0 >> 7) & 0x1FE);
		t1 = VIS_LD_U16_I(table, (s0 >> 15) & 0x1FE);
		t0 = VIS_LD_U16_I(table, (s0 >> 23) & 0x1FE);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
	}

	sp = (mlib_u8 *)sa;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = ((mlib_u8 *)dend - (mlib_u8 *)dp) >> 1;
		sp += num;
		num++;
#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			s0 = (int)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, acc0;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

/* destination data */
	mlib_d64 acc1;

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + 2 * xsize - 1;

	vis_alignaddr((void *)0, 6);

	s0 = (*sa++);

	if (xsize >= 4) {

		s1 = sa[0];
		sa++;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4, sa++) {
			t3 = VIS_LD_U16_I(table, (s1 >> 23) & 0x1FE);
			t2 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
			t1 = VIS_LD_U16_I(table, (s0 >> 7) & 0x1FE);
			t0 = VIS_LD_U16_I(table, (s0 >> 15) & 0x1FE);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = sa[0];
			vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
		}

		t3 = VIS_LD_U16_I(table, (s1 >> 23) & 0x1FE);
		t2 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
		t1 = VIS_LD_U16_I(table, (s0 >> 7) & 0x1FE);
		t0 = VIS_LD_U16_I(table, (s0 >> 15) & 0x1FE);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = ((mlib_u8 *)dend - (mlib_u8 *)dp) >> 1;
		sp += num;
		num++;
#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			s0 = (int)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask)
{
/* pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, acc0;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

/* destination data */
	mlib_d64 acc1;

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + 2 * xsize - 1;

	vis_alignaddr((void *)0, 6);

	s0 = (*sa++);

	if (xsize >= 4) {

		s1 = sa[0];
		sa++;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4, sa++) {
			t3 = VIS_LD_U16_I(table, (s1 >> 15) & 0x1FE);
			t2 = VIS_LD_U16_I(table, (s1 >> 23) & 0x1FE);
			t1 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
			t0 = VIS_LD_U16_I(table, (s0 >> 7) & 0x1FE);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = sa[0];
			vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
		}

		t3 = VIS_LD_U16_I(table, (s1 >> 15) & 0x1FE);
		t2 = VIS_LD_U16_I(table, (s1 >> 23) & 0x1FE);
		t1 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
		t0 = VIS_LD_U16_I(table, (s0 >> 7) & 0x1FE);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = ((mlib_u8 *)dend - (mlib_u8 *)dp) >> 1;
		sp += num;
		num++;
#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			s0 = (int)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_2_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u16 *table,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, acc0;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

/* destination data */
	mlib_d64 acc1;

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + 2 * xsize - 1;

	vis_alignaddr((void *)0, 6);

	s0 = (*sa++);

	if (xsize >= 4) {

		s1 = sa[0];
		sa++;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4, sa++) {
			t3 = VIS_LD_U16_I(table, (s1 >> 7) & 0x1FE);
			t2 = VIS_LD_U16_I(table, (s1 >> 15) & 0x1FE);
			t1 = VIS_LD_U16_I(table, (s1 >> 23) & 0x1FE);
			t0 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = sa[0];
			vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
		}

		t3 = VIS_LD_U16_I(table, (s1 >> 7) & 0x1FE);
		t2 = VIS_LD_U16_I(table, (s1 >> 15) & 0x1FE);
		t1 = VIS_LD_U16_I(table, (s1 >> 23) & 0x1FE);
		t0 = VIS_LD_U16_I(table, (s0 << 1) & 0x1FE);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		vis_pst_8(vis_bshuffle(acc0, acc1), dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 1;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = ((mlib_u8 *)dend - (mlib_u8 *)dp) >> 1;
		sp += num;
		num++;
#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			s0 = (int)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* destination data */
	mlib_d64 acc00, acc01;

/* destination data */
	mlib_d64 acc02, acc03;

/* destination data */
	mlib_d64 acc04, acc05;

/* loop variable */
	mlib_s32 i;
	mlib_u8 *ptr;

	dl = dst;
	dp = (mlib_d64 *)dl;
	sp = (void *)src;
	sa = (mlib_u32 *)sp;

	vis_alignaddr((void *)0, 3);

	i = 0;

	if (xsize >= 8) {

		s0 = (*sa++);
		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 21) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 13) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 5) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t4 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			t5 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 13) & 0x7F8));
			t6 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 5) & 0x7F8));
			t7 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 << 3) & 0x7F8));
			acc00 = vis_faligndata(t0, t0);
			acc00 = vis_faligndata(acc00, t1);
			acc01 = vis_faligndata(acc00, acc00);
			acc00 = vis_faligndata(acc00, t2);
			acc01 = vis_faligndata(acc01, acc00);
			acc00 = vis_faligndata(acc00, t3);
			acc02 = vis_faligndata(t4, t4);
			acc02 = vis_faligndata(acc02, t5);
			acc03 = vis_faligndata(acc02, acc02);
			acc02 = vis_faligndata(acc02, t6);
			acc03 = vis_faligndata(acc03, acc02);
			acc02 = vis_faligndata(acc02, t7);
			s0 = (*sa++);
			s1 = (*sa++);
			acc04 =
			    vis_freg_pair(vis_read_lo(acc01),
			    vis_read_hi(acc00));
			acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
			vis_pst_8(acc04, dp++, emask0);
			vis_pst_8(acc05, dp++, emask1);
			vis_pst_8(acc02, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 21) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 13) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 5) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t4 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		t5 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 13) & 0x7F8));
		t6 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 5) & 0x7F8));
		t7 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 << 3) & 0x7F8));
		acc00 = vis_faligndata(t0, t0);
		acc00 = vis_faligndata(acc00, t1);
		acc01 = vis_faligndata(acc00, acc00);
		acc00 = vis_faligndata(acc00, t2);
		acc01 = vis_faligndata(acc01, acc00);
		acc00 = vis_faligndata(acc00, t3);
		acc02 = vis_faligndata(t4, t4);
		acc02 = vis_faligndata(acc02, t5);
		acc03 = vis_faligndata(acc02, acc02);
		acc02 = vis_faligndata(acc02, t6);
		acc03 = vis_faligndata(acc03, acc02);
		acc02 = vis_faligndata(acc02, t7);
		acc04 = vis_freg_pair(vis_read_lo(acc01), vis_read_hi(acc00));
		acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
		vis_pst_8(acc04, dp++, emask0);
		vis_pst_8(acc05, dp++, emask1);
		vis_pst_8(acc02, dp++, emask2);
		i += 8;
	}

	sp = (mlib_u8 *)sa;
	dl = (mlib_u8 *)dp;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* destination data */
	mlib_d64 acc00, acc01;

/* destination data */
	mlib_d64 acc02, acc03;

/* destination data */
	mlib_d64 acc04, acc05;

/* loop variable */
	mlib_s32 i;
	mlib_u8 *ptr;

	dl = dst;
	dp = (mlib_d64 *)dl;
	sp = (void *)src;
	sa = (mlib_u32 *)(sp - 1);

	vis_alignaddr((void *)0, 3);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 8) {

		s1 = (*sa++);
		s2 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 13) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 5) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			t4 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 13) & 0x7F8));
			t5 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 5) & 0x7F8));
			t6 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 << 3) & 0x7F8));
			t7 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s2 >> 21) & 0x7F8));
			acc00 = vis_faligndata(t0, t0);
			acc00 = vis_faligndata(acc00, t1);
			acc01 = vis_faligndata(acc00, acc00);
			acc00 = vis_faligndata(acc00, t2);
			acc01 = vis_faligndata(acc01, acc00);
			acc00 = vis_faligndata(acc00, t3);
			acc02 = vis_faligndata(t4, t4);
			acc02 = vis_faligndata(acc02, t5);
			acc03 = vis_faligndata(acc02, acc02);
			acc02 = vis_faligndata(acc02, t6);
			acc03 = vis_faligndata(acc03, acc02);
			acc02 = vis_faligndata(acc02, t7);
			s0 = s2;
			s1 = (*sa++);
			s2 = (*sa++);
			acc04 =
			    vis_freg_pair(vis_read_lo(acc01),
			    vis_read_hi(acc00));
			acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
			vis_pst_8(acc04, dp++, emask0);
			vis_pst_8(acc05, dp++, emask1);
			vis_pst_8(acc02, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 13) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 5) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		t4 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 13) & 0x7F8));
		t5 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 5) & 0x7F8));
		t6 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 << 3) & 0x7F8));
		t7 = *(mlib_d64 *)((mlib_u8 *)table + ((s2 >> 21) & 0x7F8));
		acc00 = vis_faligndata(t0, t0);
		acc00 = vis_faligndata(acc00, t1);
		acc01 = vis_faligndata(acc00, acc00);
		acc00 = vis_faligndata(acc00, t2);
		acc01 = vis_faligndata(acc01, acc00);
		acc00 = vis_faligndata(acc00, t3);
		acc02 = vis_faligndata(t4, t4);
		acc02 = vis_faligndata(acc02, t5);
		acc03 = vis_faligndata(acc02, acc02);
		acc02 = vis_faligndata(acc02, t6);
		acc03 = vis_faligndata(acc03, acc02);
		acc02 = vis_faligndata(acc02, t7);
		acc04 = vis_freg_pair(vis_read_lo(acc01), vis_read_hi(acc00));
		acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
		vis_pst_8(acc04, dp++, emask0);
		vis_pst_8(acc05, dp++, emask1);
		vis_pst_8(acc02, dp++, emask2);
		i += 8;
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;
	dl = (mlib_u8 *)dp;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* destination data */
	mlib_d64 acc00, acc01;

/* destination data */
	mlib_d64 acc02, acc03;

/* destination data */
	mlib_d64 acc04, acc05;

/* loop variable */
	mlib_s32 i;
	mlib_u8 *ptr;

	dl = dst;
	dp = (mlib_d64 *)dl;
	sp = (void *)src;
	sa = (mlib_u32 *)(sp - 2);

	vis_alignaddr((void *)0, 3);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 8) {

		s1 = (*sa++);
		s2 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 5) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 13) & 0x7F8));
			t4 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 5) & 0x7F8));
			t5 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 << 3) & 0x7F8));
			t6 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s2 >> 21) & 0x7F8));
			t7 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s2 >> 13) & 0x7F8));
			acc00 = vis_faligndata(t0, t0);
			acc00 = vis_faligndata(acc00, t1);
			acc01 = vis_faligndata(acc00, acc00);
			acc00 = vis_faligndata(acc00, t2);
			acc01 = vis_faligndata(acc01, acc00);
			acc00 = vis_faligndata(acc00, t3);
			acc02 = vis_faligndata(t4, t4);
			acc02 = vis_faligndata(acc02, t5);
			acc03 = vis_faligndata(acc02, acc02);
			acc02 = vis_faligndata(acc02, t6);
			acc03 = vis_faligndata(acc03, acc02);
			acc02 = vis_faligndata(acc02, t7);
			s0 = s2;
			s1 = (*sa++);
			s2 = (*sa++);
			acc04 =
			    vis_freg_pair(vis_read_lo(acc01),
			    vis_read_hi(acc00));
			acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
			vis_pst_8(acc04, dp++, emask0);
			vis_pst_8(acc05, dp++, emask1);
			vis_pst_8(acc02, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 5) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 13) & 0x7F8));
		t4 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 5) & 0x7F8));
		t5 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 << 3) & 0x7F8));
		t6 = *(mlib_d64 *)((mlib_u8 *)table + ((s2 >> 21) & 0x7F8));
		t7 = *(mlib_d64 *)((mlib_u8 *)table + ((s2 >> 13) & 0x7F8));
		acc00 = vis_faligndata(t0, t0);
		acc00 = vis_faligndata(acc00, t1);
		acc01 = vis_faligndata(acc00, acc00);
		acc00 = vis_faligndata(acc00, t2);
		acc01 = vis_faligndata(acc01, acc00);
		acc00 = vis_faligndata(acc00, t3);
		acc02 = vis_faligndata(t4, t4);
		acc02 = vis_faligndata(acc02, t5);
		acc03 = vis_faligndata(acc02, acc02);
		acc02 = vis_faligndata(acc02, t6);
		acc03 = vis_faligndata(acc03, acc02);
		acc02 = vis_faligndata(acc02, t7);
		acc04 = vis_freg_pair(vis_read_lo(acc01), vis_read_hi(acc00));
		acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
		vis_pst_8(acc04, dp++, emask0);
		vis_pst_8(acc05, dp++, emask1);
		vis_pst_8(acc02, dp++, emask2);
		i += 8;
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;
	dl = (mlib_u8 *)dp;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* destination data */
	mlib_d64 acc00, acc01;

/* destination data */
	mlib_d64 acc02, acc03;

/* destination data */
	mlib_d64 acc04, acc05;

/* loop variable */
	mlib_s32 i;
	mlib_u8 *ptr;

	dl = dst;
	dp = (mlib_d64 *)dl;
	sp = (void *)src;
	sa = (mlib_u32 *)(sp - 3);

	vis_alignaddr((void *)0, 3);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 8) {

		s1 = (*sa++);
		s2 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 13) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 5) & 0x7F8));
			t4 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 << 3) & 0x7F8));
			t5 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s2 >> 21) & 0x7F8));
			t6 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s2 >> 13) & 0x7F8));
			t7 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s2 >> 5) & 0x7F8));
			acc00 = vis_faligndata(t0, t0);
			acc00 = vis_faligndata(acc00, t1);
			acc01 = vis_faligndata(acc00, acc00);
			acc00 = vis_faligndata(acc00, t2);
			acc01 = vis_faligndata(acc01, acc00);
			acc00 = vis_faligndata(acc00, t3);
			acc02 = vis_faligndata(t4, t4);
			acc02 = vis_faligndata(acc02, t5);
			acc03 = vis_faligndata(acc02, acc02);
			acc02 = vis_faligndata(acc02, t6);
			acc03 = vis_faligndata(acc03, acc02);
			acc02 = vis_faligndata(acc02, t7);
			s0 = s2;
			s1 = (*sa++);
			s2 = (*sa++);
			acc04 =
			    vis_freg_pair(vis_read_lo(acc01),
			    vis_read_hi(acc00));
			acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
			vis_pst_8(acc04, dp++, emask0);
			vis_pst_8(acc05, dp++, emask1);
			vis_pst_8(acc02, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 13) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 5) & 0x7F8));
		t4 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 << 3) & 0x7F8));
		t5 = *(mlib_d64 *)((mlib_u8 *)table + ((s2 >> 21) & 0x7F8));
		t6 = *(mlib_d64 *)((mlib_u8 *)table + ((s2 >> 13) & 0x7F8));
		t7 = *(mlib_d64 *)((mlib_u8 *)table + ((s2 >> 5) & 0x7F8));
		acc00 = vis_faligndata(t0, t0);
		acc00 = vis_faligndata(acc00, t1);
		acc01 = vis_faligndata(acc00, acc00);
		acc00 = vis_faligndata(acc00, t2);
		acc01 = vis_faligndata(acc01, acc00);
		acc00 = vis_faligndata(acc00, t3);
		acc02 = vis_faligndata(t4, t4);
		acc02 = vis_faligndata(acc02, t5);
		acc03 = vis_faligndata(acc02, acc02);
		acc02 = vis_faligndata(acc02, t6);
		acc03 = vis_faligndata(acc03, acc02);
		acc02 = vis_faligndata(acc02, t7);
		acc04 = vis_freg_pair(vis_read_lo(acc01), vis_read_hi(acc00));
		acc05 = vis_write_hi(acc03, vis_read_lo(acc00));
		vis_pst_8(acc04, dp++, emask0);
		vis_pst_8(acc05, dp++, emask1);
		vis_pst_8(acc02, dp++, emask2);
		i += 8;
	}

	sp = (mlib_u8 *)sa;
	sp -= 1;
	dl = (mlib_u8 *)dp;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_u8 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_U8_U8_4_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_f32 *table,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 acc00, acc01, acc02;

/* loop variable */
	mlib_s32 i;

/* pointer to end of destination */
	mlib_u8 *dend;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 off;
	mlib_u32 s00, s01, s02, s03;

	sa = (mlib_u32 *)src;
	sp = (void *)src;
	dl = dst;
	dend = dl + (xsize << 2) - 1;
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	mask >>= (-off);
	vis_alignaddr(dp, off);

	emask = vis_edge8(dl, dend);
	acc00 = vis_freg_pair(table[sp[0]], table[sp[1]]);
	vis_pst_8(vis_faligndata(acc00, acc00), dp++, emask & mask);
	sp += 2;

	if (off == 0)
		dp--;

	xsize -= 2;

	if (xsize >= 2) {
		acc01 = vis_freg_pair(table[sp[0]], table[sp[1]]);
		vis_pst_8(vis_faligndata(acc00, acc01), dp++, mask);
		acc00 = acc01;
		sp += 2;
		xsize -= 2;
	}

	sa++;

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 22) & 0x3FC;
		s01 = (s0 >> 14) & 0x3FC;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 >> 6) & 0x3FC;
			s03 = (s0 << 2) & 0x3FC;
			acc01 =
			    vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s00),
			    *(mlib_f32 *)((mlib_u8 *)table + s01));
			acc02 =
			    vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s02),
			    *(mlib_f32 *)((mlib_u8 *)table + s03));
			s0 = (*sa++);
			s00 = (s0 >> 22) & 0x3FC;
			s01 = (s0 >> 14) & 0x3FC;
			vis_pst_8(vis_faligndata(acc00, acc01), dp++, mask);
			vis_pst_8(vis_faligndata(acc01, acc02), dp++, mask);
			acc00 = acc02;
		}

		s02 = (s0 >> 6) & 0x3FC;
		s03 = (s0 << 2) & 0x3FC;
		acc01 = vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s00),
		    *(mlib_f32 *)((mlib_u8 *)table + s01));
		acc02 = vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s02),
		    *(mlib_f32 *)((mlib_u8 *)table + s03));
		vis_pst_8(vis_faligndata(acc00, acc01), dp++, mask);
		vis_pst_8(vis_faligndata(acc01, acc02), dp++, mask);
		acc00 = acc02;
		sp = (mlib_u8 *)sa;
		i += 4;
	}

	if (i <= xsize - 2) {
		acc01 = vis_freg_pair(table[sp[0]], table[sp[1]]);
		vis_pst_8(vis_faligndata(acc00, acc01), dp++, mask);
		acc00 = acc01;
		i += 2;
		sp += 2;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		acc01 = vis_freg_pair(table[sp[0]], table[sp[1]]);
		vis_pst_8(vis_faligndata(acc00, acc01), dp++, mask & emask);
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		vis_pst_8(vis_faligndata(acc01, acc01), dp++, mask & emask);
	}
}

/* *********************************************************** */

#define	FUNC_NAME(F)	mlib_v_ImageLookUpSIMask_U8_U8_##F

void
mlib_v_ImageLookUpMaskSI_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 chan)
{
	if (dchan == 2) {
		mlib_u8 *sl;
		mlib_u8 *dl;
		mlib_u16 tab[256];
		const mlib_u8 *tab0 = table[table_i[0]];
		mlib_s32 i, j, s0, s1;

		s0 = tab0[0];
		for (i = 1; i < 256; i++) {
			s1 = (s0 << 8) + s0;
			s0 = tab0[i];
			tab[i - 1] = (mlib_u16)s1;
		}

		s1 = (s0 << 8) + s0;
		tab[255] = (mlib_u16)s1;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_u8 *dp = dl + dst_i[0];
			mlib_s32 off, size = xsize;
			mlib_s32 mask;

			if (((mlib_addr)dp & 1) == 0) {
				off = ((8 - ((mlib_addr)dp & 7)) & 7) >> 1;
				mask = 0xAA;
			} else {
				off =
				    ((8 - ((mlib_addr)(dp - 1) & 7)) & 7) >> 1;
				mask = 0x55;
			}

			off = (off < size) ? off : size;

			for (i = 0; i < off; i++) {
				*dp = (mlib_u8)tab[(*sp)];
				dp += 2;
				size--;
				sp++;
			}

			if (((mlib_addr)dp & 1) != 0)
				dp -= 1;

			if (size > 0) {

				off = (mlib_addr)sp & 3;

				if (off == 0) {
					FUNC_NAME(2_SrcOff0_D1)
					    (sp, dp, size, tab, mask);
				} else if (off == 1) {
					FUNC_NAME(2_SrcOff1_D1)
					    (sp, dp, size, tab, mask);
				} else if (off == 2) {
					FUNC_NAME(2_SrcOff2_D1)
					    (sp, dp, size, tab, mask);
				} else {
					FUNC_NAME(2_SrcOff3_D1)
					    (sp, dp, size, tab, mask);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else if (dchan == 3) {
		mlib_u8 *sl;
		mlib_u8 *dl;
		mlib_d64 tab[256];
		mlib_s32 i, j;
		mlib_s32 cmask = 0;
		mlib_s32 emask0, emask1, emask2;

		for (i = 0; i < chan; i++) {
			mlib_u8 *btab = (mlib_u8 *)tab + dst_i[i];
			const mlib_u8 *tab0 = table[table_i[i]];

			cmask |= (1 << (2 - dst_i[i]));

			for (j = 0; j < 256; j++)
				btab[8 * j] = tab0[j];
		}

		emask0 = (cmask << 5) | (cmask << 2) | (cmask >> 1);
		emask1 =
		    (cmask << 7) | (cmask << 4) | (cmask << 1) | (cmask >> 2);
		emask2 = (cmask << 6) | (cmask << 3) | cmask;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_u8 *dp = dl;
			mlib_s32 off, s0, s1, size = xsize;
			const mlib_u8 *ptr1, *ptr2;

			off = ((mlib_addr)dp & 7);
			off = (5 * off) & 7;
			off = (off < size) ? off : size;

			if (chan == 1) {
				s0 = dst_i[0];
				ptr1 = table[table_i[0]];
				for (i = 0; i < off; i++) {
					dp[s0] = ptr1[sp[0]];
					dp += 3;
					sp++;
					size--;
				}
			} else {
				s0 = dst_i[0];
				s1 = dst_i[1];
				ptr1 = table[table_i[0]];
				ptr2 = table[table_i[1]];
				for (i = 0; i < off; i++) {
					dp[s0] = ptr1[sp[0]];
					dp[s1] = ptr2[sp[0]];
					dp += 3;
					sp++;
					size--;
				}
			}

			if (size > 0) {

				off = (mlib_addr)sp & 3;

				if (off == 0) {
					FUNC_NAME(3_SrcOff0_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2, chan);
				} else if (off == 1) {
					FUNC_NAME(3_SrcOff1_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2, chan);
				} else if (off == 2) {
					FUNC_NAME(3_SrcOff2_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2, chan);
				} else {
					FUNC_NAME(3_SrcOff3_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2, chan);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else if (dchan == 4) {
		mlib_u8 *sl;
		mlib_u8 *dl;
		mlib_f32 tab[256];
		mlib_s32 i, j;
		mlib_s32 cmask = 0;

		for (i = 0; i < chan; i++) {
			mlib_u8 *btab = (mlib_u8 *)tab + dst_i[i];
			const mlib_u8 *tab0 = table[table_i[i]];

			cmask |= (1 << (3 - dst_i[i]));
			for (j = 0; j < 256; j++)
				btab[4 * j] = tab0[j];
		}

		cmask |= (cmask << 4);
		cmask |= (cmask << 8);

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_u8 *dp = dl;
			mlib_s32 off, size = xsize;
			mlib_s32 s0, s1, s2;
			mlib_u8 *ptr;

			off = ((4 - ((mlib_addr)sp & 3)) & 3);
			off = (off < size) ? off : size;

			if (chan == 1) {
				s0 = dst_i[0];

#pragma pipeloop(0)
				for (i = 0; i < off; i++) {
					ptr = (mlib_u8 *)(tab + sp[0]);
					dp[s0] = ptr[s0];
					dp += 4;
					sp++;
				}
			} else if (chan == 2) {
				s0 = dst_i[0];
				s1 = dst_i[1];

#pragma pipeloop(0)
				for (i = 0; i < off; i++) {
					ptr = (mlib_u8 *)(tab + sp[0]);
					dp[s0] = ptr[s0];
					dp[s1] = ptr[s1];
					dp += 4;
					sp++;
				}
			} else {
				s0 = dst_i[0];
				s1 = dst_i[1];
				s2 = dst_i[2];

#pragma pipeloop(0)
				for (i = 0; i < off; i++) {
					ptr = (mlib_u8 *)(tab + sp[0]);
					dp[s0] = ptr[s0];
					dp[s1] = ptr[s1];
					dp[s2] = ptr[s2];
					dp += 4;
					sp++;
				}
			}

			size -= off;

			if (size > 0) {
				FUNC_NAME(4_DstNonAl_D1)
						(sp, dp, size, tab, cmask);
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
