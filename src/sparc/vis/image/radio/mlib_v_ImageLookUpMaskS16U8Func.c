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

#pragma ident	"@(#)mlib_v_ImageLookUpMaskS16U8Func.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpMaskFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUpMask_S16_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMask_S16_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMask_S16_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMask_S16_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1);

static void mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask);

static void mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask);

static void mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask);

static void mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask);

/* *********************************************************** */

#define	VIS_LD_U16_I(X, Y)	vis_ld_u16_i((void *)(X), (Y))

/* *********************************************************** */

void
mlib_v_ImageLookUpMask_S16_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;

	if (xsize >= 12) {

		s0 = sa[0];
		s1 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 2) {
			t7 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s0 >> 15) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 >> 23) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s1 >> 15) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s1 >> 23) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = sa[0];
			s1 = sa[1];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s0 >> 15) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 >> 23) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s1 >> 15) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s1 >> 23) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 2) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMask_S16_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 12) {

		s1 = sa[0];
		s2 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 2) {
			t7 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s0 >> 7) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 >> 15) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s1 >> 7) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s1 >> 15) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s2 >> 23) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s1 << 1) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = s2;
			s1 = sa[0];
			s2 = sa[1];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s0 >> 7) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 >> 15) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s1 >> 7) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s1 >> 15) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s2 >> 23) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s1 << 1) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 2) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMask_S16_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 12) {

		s1 = sa[0];
		s2 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 2) {
			t7 = VIS_LD_U16_I(table0, (s1 >> 23) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 >> 15) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s2 >> 15) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s2 >> 23) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = s2;
			s1 = sa[0];
			s2 = sa[1];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s1 >> 23) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 >> 15) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s2 >> 15) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s2 >> 23) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 2) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMask_S16_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 12) {

		s1 = sa[0];
		s2 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 2) {
			t7 = VIS_LD_U16_I(table0, (s1 >> 15) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s2 >> 23) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s1 << 1) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 >> 7) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s2 >> 7) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s2 >> 15) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = s2;
			s1 = sa[0];
			s2 = sa[1];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s1 >> 15) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s2 >> 23) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s1 << 1) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 >> 7) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s2 >> 7) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s2 >> 15) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	sp -= 1;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 2) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;

	if (xsize >= 12) {

		s0 = sa[0];
		s1 = sa[1];
		s2 = sa[2];
		sa += 3;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 3) {
			t7 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s0 >> 15) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 >> 23) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s2 >> 7) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s2 >> 15) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = sa[0];
			s1 = sa[1];
			s2 = sa[2];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s0 >> 15) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 >> 23) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s2 >> 7) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s2 >> 15) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 3) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 12) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		sa += 3;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 3) {
			t7 = VIS_LD_U16_I(table0, (s1 >> 23) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s0 >> 7) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 >> 15) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s2 >> 23) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s1 << 1) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 >> 15) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s2 << 1) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s2 >> 7) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = s3;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s1 >> 23) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s0 >> 7) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 >> 15) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s2 >> 23) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s1 << 1) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 >> 15) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s2 << 1) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s2 >> 7) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 3) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 12) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		sa += 3;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 3) {
			t7 = VIS_LD_U16_I(table0, (s1 >> 15) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s2 >> 15) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s2 >> 23) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 >> 7) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s3 >> 23) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s2 << 1) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = s3;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s1 >> 15) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s2 >> 15) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s2 >> 23) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 >> 7) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s3 >> 23) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s2 << 1) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 3) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xD;
	emask1 = 0xB;
	emask2 = 0x6;

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;
	s0 = (*sa++);

	if (xsize >= 12) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		sa += 3;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 24; i += 12, sa += 3) {
			t7 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
			t6 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
			t5 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
			t4 = VIS_LD_U16_I(table1, (s2 >> 7) & 0x1FE);
			t3 = VIS_LD_U16_I(table0, (s2 >> 15) & 0x1FE);
			t2 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s3 >> 15) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s3 >> 23) & 0x1FE);
			acc0 = vis_faligndata(t7, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t6, acc0);
			acc0 = vis_faligndata(t5, acc0);
			acc1 = vis_faligndata(t4, acc1);
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(t0, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			s0 = s3;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			vis_pst_16(acc0, dp++, emask0);
			vis_pst_16(acc1, dp++, emask1);
			vis_pst_16(acc2, dp++, emask2);
		}

		t7 = VIS_LD_U16_I(table0, (s1 >> 7) & 0x1FE);
		t6 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
		t5 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
		t4 = VIS_LD_U16_I(table1, (s2 >> 7) & 0x1FE);
		t3 = VIS_LD_U16_I(table0, (s2 >> 15) & 0x1FE);
		t2 = VIS_LD_U16_I(table1, (s1 << 1) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s3 >> 15) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s3 >> 23) & 0x1FE);
		acc0 = vis_faligndata(t7, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t6, acc0);
		acc0 = vis_faligndata(t5, acc0);
		acc1 = vis_faligndata(t4, acc1);
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(t0, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		vis_pst_16(acc0, dp++, emask0);
		vis_pst_16(acc1, dp++, emask1);
		vis_pst_16(acc2, dp++, emask2);
		i += 12;
	}

	sp = (mlib_u8 *)sa;
	sp -= 1;
	dl = (mlib_s16 *)dp;

	for (; i <= xsize - 3; i += 3, dl += 3, sp += 3) {
		dl[0] = table0[sp[0]];
		dl[1] = table1[sp[1]];
	}

	if (i < xsize) {
		dl[0] = table0[sp[0]];
		i++;
	}

	if (i < xsize)
		dl[1] = table1[sp[1]];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
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

/* pointer to end of destination */
	mlib_s16 *dend;

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

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 6);

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t3 = VIS_LD_U16_I(table3, (s0 << 1) & 0x1FE);
			t2 = VIS_LD_U16_I(table2, (s0 >> 7) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s0 >> 15) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s0 >> 23) & 0x1FE);
			acc0 = vis_faligndata(t3, acc0);
			acc0 = vis_faligndata(t2, acc0);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = (*sa++);
			vis_pst_16(acc0, dp++, mask);
		}

		t3 = VIS_LD_U16_I(table3, (s0 << 1) & 0x1FE);
		t2 = VIS_LD_U16_I(table2, (s0 >> 7) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s0 >> 15) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s0 >> 23) & 0x1FE);
		acc0 = vis_faligndata(t3, acc0);
		acc0 = vis_faligndata(t2, acc0);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		vis_pst_16(acc0, dp++, mask);
	}

	sp = (mlib_u8 *)sa;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_s16 *)dend - (mlib_s16 *)dp;
		sp += num;
		num++;

		if (num == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table2, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge16(dp, dend);
		vis_pst_16(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* pointer to end of destination */
	mlib_s16 *dend;

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

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 6);

	s0 = (*sa++);

	if (xsize >= 4) {

		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t3 = VIS_LD_U16_I(table3, (s1 >> 23) & 0x1FE);
			t2 = VIS_LD_U16_I(table2, (s0 << 1) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s0 >> 7) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s0 >> 15) & 0x1FE);
			acc0 = vis_faligndata(t3, acc0);
			acc0 = vis_faligndata(t2, acc0);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = (*sa++);
			vis_pst_16(acc0, dp++, mask);
		}

		t3 = VIS_LD_U16_I(table3, (s1 >> 23) & 0x1FE);
		t2 = VIS_LD_U16_I(table2, (s0 << 1) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s0 >> 7) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s0 >> 15) & 0x1FE);
		acc0 = vis_faligndata(t3, acc0);
		acc0 = vis_faligndata(t2, acc0);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s1;
		vis_pst_16(acc0, dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_s16 *)dend - (mlib_s16 *)dp;
		sp += num;
		num++;

		if (num == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table2, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge16(dp, dend);
		vis_pst_16(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* pointer to end of destination */
	mlib_s16 *dend;

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

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 6);

	s0 = (*sa++);

	if (xsize >= 4) {

		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t3 = VIS_LD_U16_I(table3, (s1 >> 15) & 0x1FE);
			t2 = VIS_LD_U16_I(table2, (s1 >> 23) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
			acc0 = vis_faligndata(t3, acc0);
			acc0 = vis_faligndata(t2, acc0);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = (*sa++);
			vis_pst_16(acc0, dp++, mask);
		}

		t3 = VIS_LD_U16_I(table3, (s1 >> 15) & 0x1FE);
		t2 = VIS_LD_U16_I(table2, (s1 >> 23) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s0 << 1) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s0 >> 7) & 0x1FE);
		acc0 = vis_faligndata(t3, acc0);
		acc0 = vis_faligndata(t2, acc0);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s1;
		vis_pst_16(acc0, dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_s16 *)dend - (mlib_s16 *)dp;
		sp += num;
		num++;

		if (num == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table2, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge16(dp, dend);
		vis_pst_16(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_S16_U8_4_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 *table0,
    const mlib_s16 *table1,
    const mlib_s16 *table2,
    const mlib_s16 *table3,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* pointer to end of destination */
	mlib_s16 *dend;

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

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 6);

	s0 = (*sa++);

	if (xsize >= 4) {

		s1 = (*sa++);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			t3 = VIS_LD_U16_I(table3, (s1 >> 7) & 0x1FE);
			t2 = VIS_LD_U16_I(table2, (s1 >> 15) & 0x1FE);
			t1 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
			t0 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
			acc0 = vis_faligndata(t3, acc0);
			acc0 = vis_faligndata(t2, acc0);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = (*sa++);
			vis_pst_16(acc0, dp++, mask);
		}

		t3 = VIS_LD_U16_I(table3, (s1 >> 7) & 0x1FE);
		t2 = VIS_LD_U16_I(table2, (s1 >> 15) & 0x1FE);
		t1 = VIS_LD_U16_I(table1, (s1 >> 23) & 0x1FE);
		t0 = VIS_LD_U16_I(table0, (s0 << 1) & 0x1FE);
		acc0 = vis_faligndata(t3, acc0);
		acc0 = vis_faligndata(t2, acc0);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s1;
		vis_pst_16(acc0, dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 1;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_s16 *)dend - (mlib_s16 *)dp;
		sp += num;
		num++;

		if (num == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		} else if (num == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table2, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table1, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U16_I(table0, 2 * s0);
			acc0 = vis_faligndata(t0, acc0);
		}

		emask = vis_edge16(dp, dend);
		vis_pst_16(acc0, dp, emask & mask);
	}
}

/* *********************************************************** */

#define	FUNC_NAME1(F)	mlib_v_ImageLookUpMask_S16_U8_##F
#define	FUNC_NAME2(F)	mlib_v_ImageLookUpMaskNN_S16_U8_##F

void
mlib_v_ImageLookUpMaskMN_S16_U8(
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
    mlib_s32 schan,
    mlib_s32 chan)
{
	if ((dchan == 3) && (schan == 2)) {
		mlib_u8 *sl;
		mlib_s16 *dl;
		const mlib_s16 *tab0;
		const mlib_s16 *tab1;
		mlib_s32 j, i;
		mlib_s32 cmask = 0;
		mlib_s32 dst_off;

		cmask |= (1 << (2 - dst_i[0]));
		cmask |= (1 << (2 - dst_i[1]));

		if (cmask == 6)
			dst_off = 0;
		else if (cmask == 3)
			dst_off = 1;
		else
			dst_off = 2;

		if (cmask == 5) {
			tab0 = table[table_i[0]];
			tab1 = table[table_i[1]];
		} else {
			tab0 = table[table_i[1]];
			tab1 = table[table_i[0]];
		}

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s16 *dp = dl;
			mlib_u8 *sp1;
			mlib_s16 *dp1;
			const mlib_s16 *tab2;
			mlib_s32 off, size = xsize;

			off = ((mlib_addr)(dp + dst_off) & 7) >> 1;
			off = (off < size) ? off : size;

			dp1 = dp + dst_i[0];
			tab2 = table[table_i[0]];
			sp1 = sp + 1;
			for (i = 0; i < off; i++, dp1 += 3, sp1 += 2)
				dp1[0] = tab2[sp1[0]];

			dp1 = dp + dst_i[1];
			tab2 = table[table_i[1]];
			sp1 = sp;
			for (i = 0; i < off; i++, dp1 += 3, sp1 += 2)
				dp1[0] = tab2[sp1[0]];

			size = 3 * (size - off);
			sp += 2 * off;
			dp += 3 * off;

			if (size > 0) {
				if (dst_off == 2) {
					dp[0] = table[table_i[1]][sp[0]];
					sp++;
				}

				size -= dst_off;
				dp += dst_off;

				off = (mlib_addr)sp & 3;

				if (off == 0) {
					FUNC_NAME1(3_SrcOff0_D1)
					    (sp, dp, size, tab0, tab1);
				} else if (off == 1) {
					FUNC_NAME1(3_SrcOff1_D1)
					    (sp, dp, size, tab0, tab1);
				} else if (off == 2) {
					FUNC_NAME1(3_SrcOff2_D1)
					    (sp, dp, size, tab0, tab1);
				} else {
					FUNC_NAME1(3_SrcOff3_D1)
					    (sp, dp, size, tab0, tab1);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	} else if (dchan == 3) {
		mlib_u8 *sl;
		mlib_s16 *dl;
		const mlib_s16 *tab0;
		const mlib_s16 *tab1;
		mlib_s32 j, i;
		mlib_s32 cmask = 0;
		mlib_s32 dst_off;

		cmask |= (1 << (2 - dst_i[0]));
		cmask |= (1 << (2 - dst_i[1]));

		if (cmask == 6)
			dst_off = 0;
		else if (cmask == 3)
			dst_off = 1;
		else
			dst_off = 2;

		if (cmask == 5) {
			tab0 = table[table_i[0]];
			tab1 = table[table_i[1]];
		} else {
			tab0 = table[table_i[1]];
			tab1 = table[table_i[0]];
		}

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s16 *dp = dl;
			mlib_u8 *sp1;
			mlib_s16 *dp1;
			const mlib_s16 *tab2;
			mlib_s32 off, size = xsize;

			off = ((mlib_addr)(dp + dst_off) & 7) >> 1;
			off = (off < size) ? off : size;

			dp1 = dp + dst_i[0];
			tab2 = table[table_i[0]];
			sp1 = sp + dst_i[0];
			for (i = 0; i < off; i++, dp1 += 3, sp1 += 3)
				dp1[0] = tab2[sp1[0]];

			dp1 = dp + dst_i[1];
			tab2 = table[table_i[1]];
			sp1 = sp + dst_i[1];
			for (i = 0; i < off; i++, dp1 += 3, sp1 += 3)
				dp1[0] = tab2[sp1[0]];

			size = 3 * (size - off);
			sp += 3 * off;
			dp += 3 * off;

			if (size > 0) {
				dp[dst_i[0]] = table[table_i[0]][sp[dst_i[0]]];
				dp[dst_i[1]] = table[table_i[1]][sp[dst_i[1]]];
				size -= dst_off;
				dp += dst_off;
				sp += dst_off;

				off = (mlib_addr)sp & 3;

				if (off == 0) {
					FUNC_NAME2(3_SrcOff0_D1)
					    (sp, dp, size, tab0, tab1);
				} else if (off == 1) {
					FUNC_NAME2(3_SrcOff1_D1)
					    (sp, dp, size, tab0, tab1);
				} else if (off == 2) {
					FUNC_NAME2(3_SrcOff2_D1)
					    (sp, dp, size, tab0, tab1);
				} else {
					FUNC_NAME2(3_SrcOff3_D1)
					    (sp, dp, size, tab0, tab1);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		mlib_u8 *sl;
		mlib_s16 *dl;
		const mlib_s16 *t0 = table[table_i[0]];
		const mlib_s16 *t1 = table[table_i[1]];
		const mlib_s16 *t2 = table[table_i[2]];
		mlib_s16 *tabl[4];
		mlib_s32 j, i;
		mlib_s32 cmask = 0;
		mlib_s32 d0 = dst_i[0];
		mlib_s32 d1 = dst_i[1];
		mlib_s32 d2 = dst_i[2];

		for (i = 0; i < 4; i++)
			tabl[i] = (void *)t0;

		tabl[d1] = (void *)t1;
		tabl[d2] = (void *)t2;

		for (i = 0; i < chan; i++)
			cmask |= (1 << (3 - dst_i[i]));

		cmask |= (cmask << 4);
		cmask |= (cmask << 8);

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s16 *dp = dl;
			mlib_s16 *tab0 = tabl[0];
			mlib_s16 *tab1 = tabl[1];
			mlib_s16 *tab2 = tabl[2];
			mlib_s16 *tab3 = tabl[3];
			mlib_s16 *tab;
			mlib_s32 mask = cmask;
			mlib_s32 off, size = xsize * 4;

			off = ((8 - ((mlib_addr)dp & 7)) & 7) >> 1;

			off = (off < size) ? off : size;

			if (off > 0) {
				dp[d0] = t0[sp[d0]];
				dp[d1] = t1[sp[d1]];
				dp[d2] = t2[sp[d2]];
			}

			if (off == 1) {
				tab = tab0;
				tab0 = tab1;
				tab1 = tab2;
				tab2 = tab3;
				tab3 = tab;
			} else if (off == 2) {
				tab = tab0;
				tab0 = tab2;
				tab2 = tab;
				tab = tab1;
				tab1 = tab3;
				tab3 = tab;
			} else if (off == 3) {
				tab = tab3;
				tab3 = tab2;
				tab2 = tab1;
				tab1 = tab0;
				tab0 = tab;
			}

			size -= off;
			dp += off;
			sp += off;
			mask = mask >> (4 - off);

			if (size > 0) {

				off = (mlib_addr)sp & 3;

				if (off == 0) {
					FUNC_NAME2(4_SrcOff0_D1)
					    (sp, dp, size, tab0, tab1, tab2,
					    tab3, mask);
				} else if (off == 1) {
					FUNC_NAME2(4_SrcOff1_D1)
					    (sp, dp, size, tab0, tab1, tab2,
					    tab3, mask);
				} else if (off == 2) {
					FUNC_NAME2(4_SrcOff2_D1)
					    (sp, dp, size, tab0, tab1, tab2,
					    tab3, mask);
				} else {
					FUNC_NAME2(4_SrcOff3_D1)
					    (sp, dp, size, tab0, tab1, tab2,
					    tab3, mask);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
