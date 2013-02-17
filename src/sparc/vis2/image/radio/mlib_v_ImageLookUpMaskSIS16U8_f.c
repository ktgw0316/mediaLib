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

#pragma ident	"@(#)mlib_v_ImageLookUpMaskSIS16U8_f.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpMaskFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUpSIMask_S16_U8_2_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_f32 *table,
    mlib_s32 mask);

static void mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2,
    mlib_s32 chan);

static void mlib_v_ImageLookUpSIMask_S16_U8_4_DstA8D1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 mask);

static void mlib_v_ImageLookUpSIMask_S16_U8_4_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 mask);

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_2_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
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
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;

/* pointer to end of destination */
	mlib_s16 *dend;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 off;
	mlib_u32 s00, s01, s02, s03;

	sa = (mlib_u32 *)src;
	sp = (void *)src;
	dl = dst;
	dend = dl + 2 * xsize - 1;
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	vis_alignaddr(dp, off);

	emask = vis_edge16(dl, dend);
	acc0 = vis_freg_pair(table[sp[0]], table[sp[1]]);
	vis_pst_16(vis_faligndata(acc0, acc0), dp++, emask & mask);
	sp += 2;

	if (off == 0)
		dp--;

	xsize -= 2;

	if (xsize >= 2) {
		acc1 = vis_freg_pair(table[sp[0]], table[sp[1]]);
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
		acc0 = acc1;
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
			acc1 =
			    vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s00),
			    *(mlib_f32 *)((mlib_u8 *)table + s01));
			acc2 =
			    vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s02),
			    *(mlib_f32 *)((mlib_u8 *)table + s03));
			s0 = (*sa++);
			s00 = (s0 >> 22) & 0x3FC;
			s01 = (s0 >> 14) & 0x3FC;
			vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
			vis_pst_16(vis_faligndata(acc1, acc2), dp++, mask);
			acc0 = acc2;
		}

		s02 = (s0 >> 6) & 0x3FC;
		s03 = (s0 << 2) & 0x3FC;
		acc1 = vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s00),
		    *(mlib_f32 *)((mlib_u8 *)table + s01));
		acc2 = vis_freg_pair(*(mlib_f32 *)((mlib_u8 *)table + s02),
		    *(mlib_f32 *)((mlib_u8 *)table + s03));
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
		vis_pst_16(vis_faligndata(acc1, acc2), dp++, mask);
		acc0 = acc2;
		sp = (mlib_u8 *)sa;
		i += 4;
	}

	if (i <= xsize - 2) {
		acc1 = vis_freg_pair(table[sp[0]], table[sp[1]]);
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
		acc0 = acc1;
		i += 2;
		sp += 2;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		acc1 = vis_freg_pair(table[sp[0]], table[sp[1]]);
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask & emask);
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		vis_pst_16(vis_faligndata(acc1, acc1), dp++, mask & emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
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
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s16 *ptr;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)sp;

	vis_alignaddr((void *)0, 6);

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 21) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 13) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 5) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			acc0 = vis_faligndata(t0, t0);
			acc1 = vis_faligndata(acc0, acc0);
			acc2 = vis_faligndata(acc0, t1);
			acc0 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t2);
			acc0 = vis_faligndata(acc0, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t3);
			s0 = (*sa++);
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 21) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 13) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 5) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		acc0 = vis_faligndata(t0, t0);
		acc1 = vis_faligndata(acc0, acc0);
		acc2 = vis_faligndata(acc0, t1);
		acc0 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t2);
		acc0 = vis_faligndata(acc0, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t3);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s16 *)dp;
	sp = (mlib_u8 *)sa;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
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
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s16 *ptr;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)(sp - 1);

	i = 0;
	s0 = (*sa++);

	vis_alignaddr((void *)0, 6);

	if (xsize >= 4) {

		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 13) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 5) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			acc0 = vis_faligndata(t0, t0);
			acc1 = vis_faligndata(acc0, acc0);
			acc2 = vis_faligndata(acc0, t1);
			acc0 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t2);
			acc0 = vis_faligndata(acc0, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t3);
			s0 = s1;
			s1 = (*sa++);
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 13) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 5) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		acc0 = vis_faligndata(t0, t0);
		acc1 = vis_faligndata(acc0, acc0);
		acc2 = vis_faligndata(acc0, t1);
		acc0 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t2);
		acc0 = vis_faligndata(acc0, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t3);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s16 *)dp;
	sp = (mlib_u8 *)sa;
	sp -= 3;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
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
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s16 *ptr;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)(sp - 2);

	i = 0;
	s0 = (*sa++);

	vis_alignaddr((void *)0, 6);

	if (xsize >= 4) {

		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 >> 5) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 13) & 0x7F8));
			acc0 = vis_faligndata(t0, t0);
			acc1 = vis_faligndata(acc0, acc0);
			acc2 = vis_faligndata(acc0, t1);
			acc0 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t2);
			acc0 = vis_faligndata(acc0, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t3);
			s0 = s1;
			s1 = (*sa++);
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 >> 5) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 13) & 0x7F8));
		acc0 = vis_faligndata(t0, t0);
		acc1 = vis_faligndata(acc0, acc0);
		acc2 = vis_faligndata(acc0, t1);
		acc0 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t2);
		acc0 = vis_faligndata(acc0, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t3);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s16 *)dp;
	sp = (mlib_u8 *)sa;
	sp -= 2;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
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
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s16 *ptr;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)(sp - 3);

	i = 0;
	s0 = (*sa++);

	vis_alignaddr((void *)0, 6);

	if (xsize >= 4) {

		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t0 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s0 << 3) & 0x7F8));
			t1 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 21) & 0x7F8));
			t2 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 13) & 0x7F8));
			t3 = *(mlib_d64 *)((mlib_u8 *)table +
			    ((s1 >> 5) & 0x7F8));
			acc0 = vis_faligndata(t0, t0);
			acc1 = vis_faligndata(acc0, acc0);
			acc2 = vis_faligndata(acc0, t1);
			acc0 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t2);
			acc0 = vis_faligndata(acc0, acc1);
			acc1 = vis_faligndata(acc1, acc2);
			acc2 = vis_faligndata(acc2, t3);
			s0 = s1;
			s1 = (*sa++);
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t0 = *(mlib_d64 *)((mlib_u8 *)table + ((s0 << 3) & 0x7F8));
		t1 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 21) & 0x7F8));
		t2 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 13) & 0x7F8));
		t3 = *(mlib_d64 *)((mlib_u8 *)table + ((s1 >> 5) & 0x7F8));
		acc0 = vis_faligndata(t0, t0);
		acc1 = vis_faligndata(acc0, acc0);
		acc2 = vis_faligndata(acc0, t1);
		acc0 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t2);
		acc0 = vis_faligndata(acc0, acc1);
		acc1 = vis_faligndata(acc1, acc2);
		acc2 = vis_faligndata(acc2, t3);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s16 *)dp;
	sp = (mlib_u8 *)sa;
	sp -= 1;

	if (chan == 1) {
		s0 = dst_i[0];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl += 3;
			sp++;
		}
	} else {
		s0 = dst_i[0];
		s1 = dst_i[1];

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			ptr = (mlib_s16 *)(table + sp[0]);
			dl[s0] = ptr[s0];
			dl[s1] = ptr[s1];
			dl += 3;
			sp++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_4_DstA8D1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 acc0, acc1;

/* destination data */
	mlib_d64 acc2, acc3;

/* loop variable */
	mlib_s32 i;
	mlib_u32 s00, s01, s02, s03;

	sa = (mlib_u32 *)src;
	dp = (mlib_d64 *)dst;

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 21) & 0x7F8;
		s01 = (s0 >> 13) & 0x7F8;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 >> 5) & 0x7F8;
			s03 = (s0 << 3) & 0x7F8;
			acc0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			acc1 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			acc2 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			acc3 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			s0 = (*sa++);
			s00 = (s0 >> 21) & 0x7F8;
			s01 = (s0 >> 13) & 0x7F8;
			vis_pst_16(acc0, dp++, mask);
			vis_pst_16(acc1, dp++, mask);
			vis_pst_16(acc2, dp++, mask);
			vis_pst_16(acc3, dp++, mask);
		}

		s02 = (s0 >> 5) & 0x7F8;
		s03 = (s0 << 3) & 0x7F8;
		acc0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		acc1 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		acc2 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		acc3 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		vis_pst_16(acc0, dp++, mask);
		vis_pst_16(acc1, dp++, mask);
		vis_pst_16(acc2, dp++, mask);
		vis_pst_16(acc3, dp++, mask);
		i += 4;
	}

	sp = (mlib_u8 *)sa;

	if (i <= xsize - 2) {
		vis_pst_16(table[sp[0]], dp++, mask);
		vis_pst_16(table[sp[1]], dp++, mask);
		i += 2;
		sp += 2;
	}

	if (i < xsize)
		vis_pst_16(table[sp[0]], dp++, mask);
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S16_U8_4_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 acc0, acc1;

/* destination data */
	mlib_d64 acc2, acc3, acc4;

/* loop variable */
	mlib_s32 i;

/* pointer to end of destination */
	mlib_s16 *dend;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 off;
	mlib_u32 s00, s01, s02, s03;

	sp = (void *)src;
	dl = dst;
	dend = dl + (xsize << 2) - 1;
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	vis_alignaddr(dp, off);
	mask >>= ((-off) >> 1);

	emask = vis_edge16(dl, dend);
	acc0 = table[sp[0]];
	vis_pst_16(vis_faligndata(acc0, acc0), dp++, emask & mask);
	sp++;

	sa = (mlib_u32 *)sp;

	xsize--;

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 21) & 0x7F8;
		s01 = (s0 >> 13) & 0x7F8;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 >> 5) & 0x7F8;
			s03 = (s0 << 3) & 0x7F8;
			acc1 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			acc2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			acc3 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			acc4 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			s0 = (*sa++);
			s00 = (s0 >> 21) & 0x7F8;
			s01 = (s0 >> 13) & 0x7F8;
			vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
			vis_pst_16(vis_faligndata(acc1, acc2), dp++, mask);
			vis_pst_16(vis_faligndata(acc2, acc3), dp++, mask);
			vis_pst_16(vis_faligndata(acc3, acc4), dp++, mask);
			acc0 = acc4;
		}

		s02 = (s0 >> 5) & 0x7F8;
		s03 = (s0 << 3) & 0x7F8;
		acc1 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		acc2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		acc3 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		acc4 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
		vis_pst_16(vis_faligndata(acc1, acc2), dp++, mask);
		vis_pst_16(vis_faligndata(acc2, acc3), dp++, mask);
		vis_pst_16(vis_faligndata(acc3, acc4), dp++, mask);
		acc0 = acc4;
		i += 4;
	}

	sp = (mlib_u8 *)sa;

	if (i <= xsize - 2) {
		acc1 = table[sp[0]];
		acc2 = table[sp[1]];
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
		vis_pst_16(vis_faligndata(acc1, acc2), dp++, mask);
		i += 2;
		sp += 2;
		acc0 = acc2;
	}

	if (i < xsize) {
		acc1 = table[sp[0]];
		vis_pst_16(vis_faligndata(acc0, acc1), dp++, mask);
		acc0 = acc1;
	}

	emask = vis_edge16(dp, dend);
	vis_pst_16(vis_faligndata(acc0, acc0), dp++, emask & mask);
}

/* *********************************************************** */

#define	FUNC_NAME(F)	mlib_v_ImageLookUpSIMask_S16_U8_##F

void
mlib_v_ImageLookUpMaskSI_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 chan)
{
	if (dchan == 2) {
		mlib_u8 *sl;
		mlib_s16 *dl;
		mlib_u32 tab[256];
		mlib_u16 *tab0 = (mlib_u16 *)table[table_i[0]];
		mlib_s32 i, j;
		mlib_u32 s0, s1;

		s0 = tab0[0];
		for (i = 1; i < 256; i++) {
			s1 = (s0 << 16) + s0;
			s0 = tab0[i];
			tab[i - 1] = s1;
		}

		s1 = (s0 << 16) + s0;
		tab[255] = s1;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s16 *dp = dl;
			mlib_s16 *dp1;
			mlib_s32 off, size = xsize;
			mlib_s32 mask;

			dp1 = dl + dst_i[0];

			if (((mlib_addr)dp1 & 3) == 0)
				mask = 0xA;
			else
				mask = 5;

			off = ((4 - ((mlib_addr)sp & 3)) & 3);
			off = (off < size) ? off : size;

			for (i = 0; i < off; i++) {
				*dp1 = (mlib_u16)tab[(*sp)];
				dp += 2;
				dp1 += 2;
				size--;
				sp++;
			}

			if (size > 0) {
				FUNC_NAME(2_DstNonAl_D1)
				    (sp, dp, size, (mlib_f32 *)tab, mask);
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	} else if (dchan == 3) {
		mlib_u8 *sl;
		mlib_s16 *dl;
		mlib_d64 tab[256];
		mlib_s32 i, j;
		mlib_s32 cmask = 0;
		mlib_s32 emask0, emask1, emask2;

		for (i = 0; i < chan; i++) {
			mlib_s16 *btab = (mlib_s16 *)tab + dst_i[i];
			const mlib_s16 *tab0 = table[table_i[i]];

			cmask |= (1 << (2 - dst_i[i]));

			for (j = 0; j < 256; j++)
				btab[4 * j] = tab0[j];
		}

		emask0 = (cmask << 1) | (cmask >> 2);
		emask1 = (cmask << 2) | (cmask >> 1);
		emask2 = (cmask << 3) | cmask;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s16 *dp = dl;
			mlib_s32 off, s0, s1, size = xsize;
			const mlib_s16 *ptr1, *ptr2;

			off = ((mlib_addr)dp & 7) >> 1;
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
			dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	} else if (dchan == 4) {
		mlib_u8 *sl;
		mlib_s16 *dl;
		mlib_d64 tab[256];
		mlib_s32 i, j;
		mlib_s32 cmask = 0;

		for (i = 0; i < chan; i++) {
			mlib_s16 *btab = (mlib_s16 *)tab + dst_i[i];
			const mlib_s16 *tab0 = table[table_i[i]];

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
			mlib_s16 *dp = dl;
			mlib_s32 off, size = xsize;
			mlib_s32 s0, s1, s2;
			mlib_s16 *ptr;

			if (((mlib_addr)dp & 7) == 0) {

				off = ((4 - (mlib_addr)sp & 3) & 3);
				off = (off < size) ? off : size;

				if (chan == 1) {
					s0 = dst_i[0];

#pragma pipeloop(0)
					for (i = 0; i < off; i++) {
						ptr = (mlib_s16 *)(tab + sp[0]);
						dp[s0] = ptr[s0];
						dp += 4;
						sp++;
					}
				} else if (chan == 2) {
					s0 = dst_i[0];
					s1 = dst_i[1];

#pragma pipeloop(0)
					for (i = 0; i < off; i++) {
						ptr = (mlib_s16 *)(tab + sp[0]);
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
						ptr = (mlib_s16 *)(tab + sp[0]);
						dp[s0] = ptr[s0];
						dp[s1] = ptr[s1];
						dp[s2] = ptr[s2];
						dp += 4;
						sp++;
					}
				}

				size -= off;

				if (size > 0) {
					FUNC_NAME(4_DstA8D1_D1)
					    (sp, dp, size, tab, cmask);
				}
			} else {

				off = (3 - ((mlib_addr)sp & 3));
				off = (off < size) ? off : size;

				if (chan == 1) {
					s0 = dst_i[0];

#pragma pipeloop(0)
					for (i = 0; i < off; i++) {
						ptr = (mlib_s16 *)(tab + sp[0]);
						dp[s0] = ptr[s0];
						dp += 4;
						sp++;
					}
				} else if (chan == 2) {
					s0 = dst_i[0];
					s1 = dst_i[1];

#pragma pipeloop(0)
					for (i = 0; i < off; i++) {
						ptr = (mlib_s16 *)(tab + sp[0]);
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
						ptr = (mlib_s16 *)(tab + sp[0]);
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
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
