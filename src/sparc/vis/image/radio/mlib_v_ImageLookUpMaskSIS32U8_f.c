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

#pragma ident	"@(#)mlib_v_ImageLookUpMaskSIS32U8_f.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpMaskFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUpSIMask_S32_U8_2_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 xsize,
    const mlib_f32 *table);

static void mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2);

static void mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2);

static void mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2);

static void mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2);

static void mlib_v_ImageLookUpMaskSI_S32_U8_4_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 emask0,
    mlib_s32 emask1);

static void mlib_v_ImageLookUpMaskSI_U8_S32_4_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 emask0,
    mlib_s32 emask1);

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S32_U8_2_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 xsize,
    const mlib_f32 *table)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* pointer to start of destination */
	mlib_f32 *dp;

/* destination data */
	mlib_f32 acc0, acc1;

/* destination data */
	mlib_f32 acc2, acc3;

/* loop variable */
	mlib_s32 i;
	mlib_u32 s00, s01, s02, s03;

	sa = (mlib_u32 *)src;
	dp = dst;

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 22) & 0x3FC;
		s01 = (s0 >> 14) & 0x3FC;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4, dp += 8) {
			s02 = (s0 >> 6) & 0x3FC;
			s03 = (s0 << 2) & 0x3FC;
			acc0 = *(mlib_f32 *)((mlib_u8 *)table + s00);
			acc1 = *(mlib_f32 *)((mlib_u8 *)table + s01);
			acc2 = *(mlib_f32 *)((mlib_u8 *)table + s02);
			acc3 = *(mlib_f32 *)((mlib_u8 *)table + s03);
			s0 = (*sa++);
			s00 = (s0 >> 22) & 0x3FC;
			s01 = (s0 >> 14) & 0x3FC;
			dp[0] = acc0;
			dp[2] = acc1;
			dp[4] = acc2;
			dp[6] = acc3;
		}

		s02 = (s0 >> 6) & 0x3FC;
		s03 = (s0 << 2) & 0x3FC;
		acc0 = *(mlib_f32 *)((mlib_u8 *)table + s00);
		acc1 = *(mlib_f32 *)((mlib_u8 *)table + s01);
		acc2 = *(mlib_f32 *)((mlib_u8 *)table + s02);
		acc3 = *(mlib_f32 *)((mlib_u8 *)table + s03);
		dp[0] = acc0;
		dp[2] = acc1;
		dp[4] = acc2;
		dp[6] = acc3;
		i += 4;
		dp += 8;
	}

	sp = (mlib_u8 *)sa;

	for (; i < xsize; i++) {
		dp[0] = table[sp[0]];
		dp += 2;
		sp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s32 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* loop variable */
	mlib_s32 i;
	mlib_s32 *ptr;
	mlib_u32 s00, s01, s02, s03;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)sp;

	vis_alignaddr((void *)0, 4);

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 20) & 0xFF0;
		s01 = (s0 >> 12) & 0xFF0;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 >> 4) & 0xFF0;
			s03 = (s0 << 4) & 0xFF0;
			t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
			t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
			t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
			t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
			t1 = vis_faligndata(t1, t1);
			t1 = vis_faligndata(t1, t2);
			t2 = vis_faligndata(t2, t3);
			t5 = vis_faligndata(t5, t5);
			t5 = vis_faligndata(t5, t6);
			t6 = vis_faligndata(t6, t7);
			s0 = (*sa++);
			s00 = (s0 >> 20) & 0xFF0;
			s01 = (s0 >> 12) & 0xFF0;
			vis_pst_16(t0, dp++, emask0);
			vis_pst_16(t1, dp++, emask1);
			vis_pst_16(t2, dp++, emask2);
			vis_pst_16(t4, dp++, emask0);
			vis_pst_16(t5, dp++, emask1);
			vis_pst_16(t6, dp++, emask2);
		}

		s02 = (s0 >> 4) & 0xFF0;
		s03 = (s0 << 4) & 0xFF0;
		t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		t1 = vis_faligndata(t1, t1);
		t1 = vis_faligndata(t1, t2);
		t2 = vis_faligndata(t2, t3);
		t5 = vis_faligndata(t5, t5);
		t5 = vis_faligndata(t5, t6);
		t6 = vis_faligndata(t6, t7);
		vis_pst_16(t0, dp++, emask0);
		vis_pst_16(t1, dp++, emask1);
		vis_pst_16(t2, dp++, emask2);
		vis_pst_16(t4, dp++, emask0);
		vis_pst_16(t5, dp++, emask1);
		vis_pst_16(t6, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s32 *)dp;
	sp = (mlib_u8 *)sa;

	s0 = dst_i[0];
	s1 = dst_i[1];

#pragma pipeloop(0)
	for (; i < xsize; i++) {
		ptr = (mlib_s32 *)(table + (sp[0] << 1));
		dl[s0] = ptr[s0];
		dl[s1] = ptr[s1];
		dl += 3;
		sp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s32 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* loop variable */
	mlib_s32 i;
	mlib_s32 *ptr;
	mlib_u32 s00, s01, s02, s03;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)(sp - 1);

	i = 0;
	s0 = (*sa++);

	vis_alignaddr((void *)0, 4);

	if (xsize >= 4) {

		s1 = (*sa++);
		s00 = (s0 >> 12) & 0xFF0;
		s01 = (s0 >> 4) & 0xFF0;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 << 4) & 0xFF0;
			s03 = (s1 >> 20) & 0xFF0;
			t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
			t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
			t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
			t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
			t1 = vis_faligndata(t1, t1);
			t1 = vis_faligndata(t1, t2);
			t2 = vis_faligndata(t2, t3);
			t5 = vis_faligndata(t5, t5);
			t5 = vis_faligndata(t5, t6);
			t6 = vis_faligndata(t6, t7);
			s0 = s1;
			s1 = (*sa++);
			s00 = (s0 >> 12) & 0xFF0;
			s01 = (s0 >> 4) & 0xFF0;
			vis_pst_16(t0, dp++, emask0);
			vis_pst_16(t1, dp++, emask1);
			vis_pst_16(t2, dp++, emask2);
			vis_pst_16(t4, dp++, emask0);
			vis_pst_16(t5, dp++, emask1);
			vis_pst_16(t6, dp++, emask2);
		}

		s02 = (s0 << 4) & 0xFF0;
		s03 = (s1 >> 20) & 0xFF0;
		t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		t1 = vis_faligndata(t1, t1);
		t1 = vis_faligndata(t1, t2);
		t2 = vis_faligndata(t2, t3);
		t5 = vis_faligndata(t5, t5);
		t5 = vis_faligndata(t5, t6);
		t6 = vis_faligndata(t6, t7);
		vis_pst_16(t0, dp++, emask0);
		vis_pst_16(t1, dp++, emask1);
		vis_pst_16(t2, dp++, emask2);
		vis_pst_16(t4, dp++, emask0);
		vis_pst_16(t5, dp++, emask1);
		vis_pst_16(t6, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s32 *)dp;
	sp = (mlib_u8 *)sa;
	sp -= 3;

	s0 = dst_i[0];
	s1 = dst_i[1];

#pragma pipeloop(0)
	for (; i < xsize; i++) {
		ptr = (mlib_s32 *)(table + (sp[0] << 1));
		dl[s0] = ptr[s0];
		dl[s1] = ptr[s1];
		dl += 3;
		sp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s32 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* loop variable */
	mlib_s32 i;
	mlib_s32 *ptr;
	mlib_u32 s00, s01, s02, s03;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)(sp - 2);

	i = 0;
	s0 = (*sa++);

	vis_alignaddr((void *)0, 4);

	if (xsize >= 4) {

		s1 = (*sa++);
		s00 = (s0 >> 4) & 0xFF0;
		s01 = (s0 << 4) & 0xFF0;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s1 >> 20) & 0xFF0;
			s03 = (s1 >> 12) & 0xFF0;
			t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
			t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
			t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
			t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
			t1 = vis_faligndata(t1, t1);
			t1 = vis_faligndata(t1, t2);
			t2 = vis_faligndata(t2, t3);
			t5 = vis_faligndata(t5, t5);
			t5 = vis_faligndata(t5, t6);
			t6 = vis_faligndata(t6, t7);
			s0 = s1;
			s1 = (*sa++);
			s00 = (s0 >> 4) & 0xFF0;
			s01 = (s0 << 4) & 0xFF0;
			vis_pst_16(t0, dp++, emask0);
			vis_pst_16(t1, dp++, emask1);
			vis_pst_16(t2, dp++, emask2);
			vis_pst_16(t4, dp++, emask0);
			vis_pst_16(t5, dp++, emask1);
			vis_pst_16(t6, dp++, emask2);
		}

		s02 = (s1 >> 20) & 0xFF0;
		s03 = (s1 >> 12) & 0xFF0;
		t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		t1 = vis_faligndata(t1, t1);
		t1 = vis_faligndata(t1, t2);
		t2 = vis_faligndata(t2, t3);
		t5 = vis_faligndata(t5, t5);
		t5 = vis_faligndata(t5, t6);
		t6 = vis_faligndata(t6, t7);
		vis_pst_16(t0, dp++, emask0);
		vis_pst_16(t1, dp++, emask1);
		vis_pst_16(t2, dp++, emask2);
		vis_pst_16(t4, dp++, emask0);
		vis_pst_16(t5, dp++, emask1);
		vis_pst_16(t6, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s32 *)dp;
	sp = (mlib_u8 *)sa;
	sp -= 2;

	s0 = dst_i[0];
	s1 = dst_i[1];

#pragma pipeloop(0)
	for (; i < xsize; i++) {
		ptr = (mlib_s32 *)(table + (sp[0] << 1));
		dl[s0] = ptr[s0];
		dl[s1] = ptr[s1];
		dl += 3;
		sp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSIMask_S32_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    const mlib_s32 *dst_i,
    mlib_s32 emask0,
    mlib_s32 emask1,
    mlib_s32 emask2)
{
/* pointer to source data */
	mlib_u8 *sp;

/* aligned pointer to source data */
	mlib_u32 *sa;

/* source data */
	mlib_u32 s0, s1;

/* pointer to start of destination */
	mlib_s32 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* loop variable */
	mlib_s32 i;
	mlib_s32 *ptr;
	mlib_u32 s00, s01, s02, s03;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	sa = (mlib_u32 *)(sp - 3);

	i = 0;
	s0 = (*sa++);

	vis_alignaddr((void *)0, 4);

	if (xsize >= 4) {

		s1 = (*sa++);
		s00 = (s0 << 4) & 0xFF0;
		s01 = (s1 >> 20) & 0xFF0;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s1 >> 12) & 0xFF0;
			s03 = (s1 >> 4) & 0xFF0;
			t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
			t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
			t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
			t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
			t1 = vis_faligndata(t1, t1);
			t1 = vis_faligndata(t1, t2);
			t2 = vis_faligndata(t2, t3);
			t5 = vis_faligndata(t5, t5);
			t5 = vis_faligndata(t5, t6);
			t6 = vis_faligndata(t6, t7);
			s0 = s1;
			s1 = (*sa++);
			s00 = (s0 << 4) & 0xFF0;
			s01 = (s1 >> 20) & 0xFF0;
			vis_pst_16(t0, dp++, emask0);
			vis_pst_16(t1, dp++, emask1);
			vis_pst_16(t2, dp++, emask2);
			vis_pst_16(t4, dp++, emask0);
			vis_pst_16(t5, dp++, emask1);
			vis_pst_16(t6, dp++, emask2);
		}

		s02 = (s1 >> 12) & 0xFF0;
		s03 = (s1 >> 4) & 0xFF0;
		t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		t1 = vis_faligndata(t1, t1);
		t1 = vis_faligndata(t1, t2);
		t2 = vis_faligndata(t2, t3);
		t5 = vis_faligndata(t5, t5);
		t5 = vis_faligndata(t5, t6);
		t6 = vis_faligndata(t6, t7);
		vis_pst_16(t0, dp++, emask0);
		vis_pst_16(t1, dp++, emask1);
		vis_pst_16(t2, dp++, emask2);
		vis_pst_16(t4, dp++, emask0);
		vis_pst_16(t5, dp++, emask1);
		vis_pst_16(t6, dp++, emask2);
		i += 4;
	}

	dl = (mlib_s32 *)dp;
	sp = (mlib_u8 *)sa;
	sp -= 1;

	s0 = dst_i[0];
	s1 = dst_i[1];

#pragma pipeloop(0)
	for (; i < xsize; i++) {
		ptr = (mlib_s32 *)(table + (sp[0] << 1));
		dl[s0] = ptr[s0];
		dl[s1] = ptr[s1];
		dl += 3;
		sp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskSI_S32_U8_4_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 emask0,
    mlib_s32 emask1)
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
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7;

/* loop variable */
	mlib_s32 i;
	mlib_u32 s00, s01, s02, s03;

	sa = (mlib_u32 *)src;
	dp = (mlib_d64 *)dst;

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 20) & 0xFF0;
		s01 = (s0 >> 12) & 0xFF0;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 >> 4) & 0xFF0;
			s03 = (s0 << 4) & 0xFF0;
			t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
			t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
			t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
			t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
			s0 = (*sa++);
			s00 = (s0 >> 20) & 0xFF0;
			s01 = (s0 >> 12) & 0xFF0;
			vis_pst_16(t0, dp++, emask0);
			vis_pst_16(t1, dp++, emask1);
			vis_pst_16(t2, dp++, emask0);
			vis_pst_16(t3, dp++, emask1);
			vis_pst_16(t4, dp++, emask0);
			vis_pst_16(t5, dp++, emask1);
			vis_pst_16(t6, dp++, emask0);
			vis_pst_16(t7, dp++, emask1);
		}

		s02 = (s0 >> 4) & 0xFF0;
		s03 = (s0 << 4) & 0xFF0;
		t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t7 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		vis_pst_16(t0, dp++, emask0);
		vis_pst_16(t1, dp++, emask1);
		vis_pst_16(t2, dp++, emask0);
		vis_pst_16(t3, dp++, emask1);
		vis_pst_16(t4, dp++, emask0);
		vis_pst_16(t5, dp++, emask1);
		vis_pst_16(t6, dp++, emask0);
		vis_pst_16(t7, dp++, emask1);
		i += 4;
	}

	sp = (mlib_u8 *)sa;

	if (i < xsize) {
		vis_pst_16(table[2 * sp[0]], dp++, emask0);
		vis_pst_16(table[2 * sp[0] + 1], dp++, emask1);
		i++;
		sp++;
	}

	if (i < xsize) {
		vis_pst_16(table[2 * sp[0]], dp++, emask0);
		vis_pst_16(table[2 * sp[0] + 1], dp++, emask1);
		i++;
		sp++;
	}

	if (i < xsize) {
		vis_pst_16(table[2 * sp[0]], dp++, emask0);
		vis_pst_16(table[2 * sp[0] + 1], dp, emask1);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskSI_U8_S32_4_DstNonAl_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 xsize,
    const mlib_d64 *table,
    mlib_s32 emask0,
    mlib_s32 emask1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* pointer to start of destination */
	mlib_s32 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 t4, t5, t6, t7, t8;

/* loop variable */
	mlib_s32 i;
	mlib_u32 s00, s01, s02, s03;
	mlib_s32 mask0, mask1;

	mask0 = (((emask0 >> 2) & 3) | ((emask1 << 2) & 0xC));
	mask1 = (((emask1 >> 2) & 3) | ((emask0 << 2) & 0xC));

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	vis_alignaddr(dp, 4);
	s0 = (*sa++);
	s00 = (s0 >> 20) & 0xFF0;
	t0 = *(mlib_d64 *)((mlib_u8 *)table + s00);
	t1 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
	vis_pst_16(vis_faligndata(t0, t0), dp++, (mask0 & 3));
	vis_pst_16(vis_faligndata(t0, t1), dp++, mask1);
	t0 = t1;
	xsize--;
	sp = (mlib_u8 *)sa - 3;

	if (xsize >= 3) {
		s01 = (s0 >> 12) & 0xFF0;
		s02 = (s0 >> 4) & 0xFF0;
		s03 = (s0 << 4) & 0xFF0;
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		vis_pst_16(vis_faligndata(t0, t1), dp++, mask0);
		vis_pst_16(vis_faligndata(t1, t2), dp++, mask1);
		vis_pst_16(vis_faligndata(t2, t3), dp++, mask0);
		vis_pst_16(vis_faligndata(t3, t4), dp++, mask1);
		vis_pst_16(vis_faligndata(t4, t5), dp++, mask0);
		vis_pst_16(vis_faligndata(t5, t6), dp++, mask1);
		t0 = t6;
		xsize -= 3;
		sp = (mlib_u8 *)sa;
	}

	i = 0;

	if (xsize >= 4) {

		s0 = (*sa++);
		s00 = (s0 >> 20) & 0xFF0;
		s01 = (s0 >> 12) & 0xFF0;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4) {
			s02 = (s0 >> 4) & 0xFF0;
			s03 = (s0 << 4) & 0xFF0;
			t1 = *(mlib_d64 *)((mlib_u8 *)table + s00);
			t2 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
			t3 = *(mlib_d64 *)((mlib_u8 *)table + s01);
			t4 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
			t5 = *(mlib_d64 *)((mlib_u8 *)table + s02);
			t6 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
			t7 = *(mlib_d64 *)((mlib_u8 *)table + s03);
			t8 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
			s0 = (*sa++);
			s00 = (s0 >> 20) & 0xFF0;
			s01 = (s0 >> 12) & 0xFF0;
			vis_pst_16(vis_faligndata(t0, t1), dp++, mask0);
			vis_pst_16(vis_faligndata(t1, t2), dp++, mask1);
			vis_pst_16(vis_faligndata(t2, t3), dp++, mask0);
			vis_pst_16(vis_faligndata(t3, t4), dp++, mask1);
			vis_pst_16(vis_faligndata(t4, t5), dp++, mask0);
			vis_pst_16(vis_faligndata(t5, t6), dp++, mask1);
			vis_pst_16(vis_faligndata(t6, t7), dp++, mask0);
			vis_pst_16(vis_faligndata(t7, t8), dp++, mask1);
			t0 = t8;
		}

		s02 = (s0 >> 4) & 0xFF0;
		s03 = (s0 << 4) & 0xFF0;
		t1 = *(mlib_d64 *)((mlib_u8 *)table + s00);
		t2 = *(mlib_d64 *)((mlib_u8 *)table + s00 + 8);
		t3 = *(mlib_d64 *)((mlib_u8 *)table + s01);
		t4 = *(mlib_d64 *)((mlib_u8 *)table + s01 + 8);
		t5 = *(mlib_d64 *)((mlib_u8 *)table + s02);
		t6 = *(mlib_d64 *)((mlib_u8 *)table + s02 + 8);
		t7 = *(mlib_d64 *)((mlib_u8 *)table + s03);
		t8 = *(mlib_d64 *)((mlib_u8 *)table + s03 + 8);
		vis_pst_16(vis_faligndata(t0, t1), dp++, mask0);
		vis_pst_16(vis_faligndata(t1, t2), dp++, mask1);
		vis_pst_16(vis_faligndata(t2, t3), dp++, mask0);
		vis_pst_16(vis_faligndata(t3, t4), dp++, mask1);
		vis_pst_16(vis_faligndata(t4, t5), dp++, mask0);
		vis_pst_16(vis_faligndata(t5, t6), dp++, mask1);
		vis_pst_16(vis_faligndata(t6, t7), dp++, mask0);
		vis_pst_16(vis_faligndata(t7, t8), dp++, mask1);
		t0 = t8;
		i += 4;
		sp = (mlib_u8 *)sa;
	}

	if (i < xsize) {
		t1 = table[2 * sp[0]];
		t2 = table[2 * sp[0] + 1];
		vis_pst_16(vis_faligndata(t0, t1), dp++, mask0);
		vis_pst_16(vis_faligndata(t1, t2), dp++, mask1);
		i++;
		sp++;
		t0 = t2;
	}

	if (i < xsize) {
		t1 = table[2 * sp[0]];
		t2 = table[2 * sp[0] + 1];
		vis_pst_16(vis_faligndata(t0, t1), dp++, mask0);
		vis_pst_16(vis_faligndata(t1, t2), dp++, mask1);
		i++;
		sp++;
		t0 = t2;
	}

	if (i < xsize) {
		t1 = table[2 * sp[0]];
		t2 = table[2 * sp[0] + 1];
		vis_pst_16(vis_faligndata(t0, t1), dp++, mask0);
		vis_pst_16(vis_faligndata(t1, t2), dp++, mask1);
		t0 = t2;
	}

	vis_pst_16(vis_faligndata(t0, t0), dp, (mask0 & 0xC));
}

/* *********************************************************** */

#define	FUNC_NAME1(F)	mlib_v_ImageLookUpSIMask_S32_U8_##F
#define	FUNC_NAME2(F)	mlib_v_ImageLookUpMaskSI_S32_U8_##F
#define	FUNC_NAME3(F)	mlib_v_ImageLookUpMaskSI_U8_S32_##F

void
mlib_v_ImageLookUpMaskSI_S32_U8(
    mlib_s32 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 chan)
{
	if (dchan == 2) {
		mlib_u8 *sl;
		mlib_s32 *dl;
		mlib_f32 *tab = (mlib_f32 *)table[table_i[0]];
		mlib_s32 i, j;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_f32 *dp1;
			mlib_s32 off, size = xsize;

			dp1 = (mlib_f32 *)dl + dst_i[0];

			off = (mlib_s32)(((4 - ((mlib_addr)sp & 3)) & 3));
			off = (off < size) ? off : size;

			for (i = 0; i < off; i++) {
				*dp1 = tab[(*sp)];
				dp1 += 2;
				size--;
				sp++;
			}

			if (size > 0) {
				FUNC_NAME1(2_D1)
					(sp, dp1, size, tab);
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}

	} else if (dchan == 3) {
		mlib_u8 *sl;
		mlib_s32 *dl;
		mlib_d64 tab[512];
		mlib_s32 i, j;
		mlib_s32 cmask = 0;
		mlib_s32 emask0, emask1, emask2;

		for (i = 0; i < chan; i++) {
			mlib_s32 *btab = (mlib_s32 *)tab + dst_i[i];
			const mlib_s32 *tab0 = table[table_i[i]];

			cmask |=
			    ((1 << (2 * (2 - dst_i[i]))) | (1 << (2 * (2 -
			    dst_i[i]) + 1)));

			for (j = 0; j < 256; j++)
				btab[4 * j] = tab0[j];
		}

		emask0 = (cmask >> 2) & 0xF;
		emask1 = ((cmask << 2) | (cmask >> 4)) & 0xF;
		emask2 = cmask & 0xF;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s32 *dp = dl;
			mlib_s32 off, s0, s1, size = xsize;
			const mlib_s32 *ptr1, *ptr2;

			off = (mlib_s32)(((mlib_addr)dp & 7) >> 2);
			off = (off < size) ? off : size;

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

			if (size > 0) {

				off = (mlib_s32)((mlib_addr)sp & 3);

				if (off == 0) {
					FUNC_NAME1(3_SrcOff0_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2);
				} else if (off == 1) {
					FUNC_NAME1(3_SrcOff1_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2);
				} else if (off == 2) {
					FUNC_NAME1(3_SrcOff2_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2);
				} else {
					FUNC_NAME1(3_SrcOff3_D1)
					    (sp, dp, size, tab, dst_i, emask0,
					    emask1, emask2);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}

	} else if (dchan == 4) {
		mlib_u8 *sl;
		mlib_s32 *dl;
		mlib_d64 tab[512];
		mlib_s32 i, j;
		mlib_s32 cmask = 0;
		mlib_s32 emask0, emask1;

		for (i = 0; i < chan; i++) {
			mlib_s32 *btab = (mlib_s32 *)tab + dst_i[i];
			const mlib_s32 *tab0 = table[table_i[i]];

			cmask |=
			    ((1 << (2 * (3 - dst_i[i]))) | (1 << (2 * (3 -
			    dst_i[i]) + 1)));
			for (j = 0; j < 256; j++)
				btab[4 * j] = tab0[j];
		}

		emask0 = (cmask >> 4) & 0xF;
		emask1 = cmask & 0xF;

		sl = (void *)src;
		dl = dst;

/* row loop */
		for (j = 0; j < ysize; j++) {
			mlib_u8 *sp = sl;
			mlib_s32 *dp = dl;
			mlib_s32 off, size = xsize;
			mlib_s32 s0, s1, s2;
			mlib_s32 *ptr;

			off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
			off = (off < size) ? off : size;

			if (chan == 2) {
				s0 = dst_i[0];
				s1 = dst_i[1];

#pragma pipeloop(0)
				for (i = 0; i < off; i++) {
					ptr = (mlib_s32 *)(tab + (sp[0] << 1));
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
					ptr = (mlib_s32 *)(tab + (sp[0] << 1));
					dp[s0] = ptr[s0];
					dp[s1] = ptr[s1];
					dp[s2] = ptr[s2];
					dp += 4;
					sp++;
				}
			}

			size -= off;

			if (size > 0) {
				if (((mlib_addr)dp & 7) == 0) {
					FUNC_NAME2(4_SrcOff0_D1)
					    (sp, dp, size, tab, emask0, emask1);
				} else {
					FUNC_NAME3(4_DstNonAl_D1)
					    (sp, dp, size, tab, emask0, emask1);
				}
			}

			sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
