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

#pragma ident	"@(#)mlib_v_ImageLookUpMaskU8U8Func.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpMaskFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUpMask_U8_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMask_U8_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMask_U8_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMask_U8_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1);

static void mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask);

static void mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask);

static void mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask);

static void mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask);

/* *********************************************************** */

#define	VIS_LD_U8_I(X, Y)	vis_ld_u8_i((void *)(X), (Y))

/* *********************************************************** */

void
mlib_v_ImageLookUpMask_U8_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
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
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 7);

	i = 0;

	if (xsize >= 24) {

		s0 = sa[0];
		s1 = sa[1];
		s2 = sa[2];
		s3 = sa[3];
		sa += 4;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 4) {
			t15 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
			t14 = VIS_LD_U8_I(table0, s1 >> 24);
			t13 = VIS_LD_U8_I(table1, s0 & 0xFF);
			t12 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
			t11 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
			t10 = VIS_LD_U8_I(table0, s0 >> 24);
			t9 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
			t8 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
			t7 = VIS_LD_U8_I(table0, s2 >> 24);
			t6 = VIS_LD_U8_I(table1, s1 & 0xFF);
			t5 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
			t4 = VIS_LD_U8_I(table1, s3 & 0xFF);
			t3 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
			t2 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
			t1 = VIS_LD_U8_I(table0, s3 >> 24);
			t0 = VIS_LD_U8_I(table1, s2 & 0xFF);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = sa[0];
			s1 = sa[1];
			s2 = sa[2];
			s3 = sa[3];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
		t14 = VIS_LD_U8_I(table0, s1 >> 24);
		t13 = VIS_LD_U8_I(table1, s0 & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
		t11 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
		t10 = VIS_LD_U8_I(table0, s0 >> 24);
		t9 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
		t8 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
		t7 = VIS_LD_U8_I(table0, s2 >> 24);
		t6 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t5 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t4 = VIS_LD_U8_I(table1, s3 & 0xFF);
		t3 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
		t2 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
		t1 = VIS_LD_U8_I(table0, s3 >> 24);
		t0 = VIS_LD_U8_I(table1, s2 & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	sp = (mlib_u8 *)sa;
	dl = (mlib_u8 *)dp;

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
mlib_v_ImageLookUpMask_U8_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3, s4;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 7);

	i = 0;

	s0 = (*sa++);

	if (xsize >= 24) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		sa += 4;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 4) {
			t15 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
			t14 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
			t13 = VIS_LD_U8_I(table1, (s1 >> 24));
			t12 = VIS_LD_U8_I(table0, s0 & 0xFF);
			t11 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
			t10 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
			t9 = VIS_LD_U8_I(table0, s2 & 0xFF);
			t8 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
			t7 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
			t6 = VIS_LD_U8_I(table1, s2 >> 24);
			t5 = VIS_LD_U8_I(table0, s1 & 0xFF);
			t4 = VIS_LD_U8_I(table1, s4 >> 24);
			t3 = VIS_LD_U8_I(table0, s3 & 0xFF);
			t2 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
			t1 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
			t0 = VIS_LD_U8_I(table1, s3 >> 24);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = s4;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			s4 = sa[3];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
		t13 = VIS_LD_U8_I(table1, (s1 >> 24));
		t12 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t11 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
		t9 = VIS_LD_U8_I(table0, s2 & 0xFF);
		t8 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t7 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		t6 = VIS_LD_U8_I(table1, s2 >> 24);
		t5 = VIS_LD_U8_I(table0, s1 & 0xFF);
		t4 = VIS_LD_U8_I(table1, s4 >> 24);
		t3 = VIS_LD_U8_I(table0, s3 & 0xFF);
		t2 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
		t1 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
		t0 = VIS_LD_U8_I(table1, s3 >> 24);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;
	dl = (mlib_u8 *)dp;

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
mlib_v_ImageLookUpMask_U8_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3, s4;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 7);

	i = 0;

	s0 = (*sa++);

	if (xsize >= 24) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		sa += 4;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 4) {
			t15 = VIS_LD_U8_I(table1, s1 & 0xFF);
			t14 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
			t13 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
			t12 = VIS_LD_U8_I(table0, (s1 >> 24));
			t11 = VIS_LD_U8_I(table1, s0 & 0xFF);
			t10 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
			t9 = VIS_LD_U8_I(table0, s3 >> 24);
			t8 = VIS_LD_U8_I(table1, s2 & 0xFF);
			t7 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
			t6 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
			t5 = VIS_LD_U8_I(table0, s2 >> 24);
			t4 = VIS_LD_U8_I(table1, (s4 >> 16) & 0xFF);
			t3 = VIS_LD_U8_I(table0, s4 >> 24);
			t2 = VIS_LD_U8_I(table1, s3 & 0xFF);
			t1 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
			t0 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = s4;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			s4 = sa[3];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t13 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 24));
		t11 = VIS_LD_U8_I(table1, s0 & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
		t9 = VIS_LD_U8_I(table0, s3 >> 24);
		t8 = VIS_LD_U8_I(table1, s2 & 0xFF);
		t7 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
		t6 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
		t5 = VIS_LD_U8_I(table0, s2 >> 24);
		t4 = VIS_LD_U8_I(table1, (s4 >> 16) & 0xFF);
		t3 = VIS_LD_U8_I(table0, s4 >> 24);
		t2 = VIS_LD_U8_I(table1, s3 & 0xFF);
		t1 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
		t0 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;
	dl = (mlib_u8 *)dp;

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
mlib_v_ImageLookUpMask_U8_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3, s4;

/* pointer to start of destination */
	mlib_u8 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* loop variable */
	mlib_s32 i;
	mlib_s32 emask0, emask1, emask2;

	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 7);

	i = 0;

	s0 = (*sa++);

	if (xsize >= 24) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		sa += 4;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 4) {
			t15 = VIS_LD_U8_I(table1, s2 >> 24);
			t14 = VIS_LD_U8_I(table0, s1 & 0xFF);
			t13 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
			t12 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
			t11 = VIS_LD_U8_I(table1, (s1 >> 24));
			t10 = VIS_LD_U8_I(table0, s0 & 0xFF);
			t9 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
			t8 = VIS_LD_U8_I(table1, s3 >> 24);
			t7 = VIS_LD_U8_I(table0, s2 & 0xFF);
			t6 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
			t5 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
			t4 = VIS_LD_U8_I(table1, (s4 >> 8) & 0xFF);
			t3 = VIS_LD_U8_I(table0, (s4 >> 16) & 0xFF);
			t2 = VIS_LD_U8_I(table1, s4 >> 24);
			t1 = VIS_LD_U8_I(table0, s3 & 0xFF);
			t0 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = s4;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			s4 = sa[3];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, s2 >> 24);
		t14 = VIS_LD_U8_I(table0, s1 & 0xFF);
		t13 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
		t11 = VIS_LD_U8_I(table1, (s1 >> 24));
		t10 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t9 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
		t8 = VIS_LD_U8_I(table1, s3 >> 24);
		t7 = VIS_LD_U8_I(table0, s2 & 0xFF);
		t6 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t5 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		t4 = VIS_LD_U8_I(table1, (s4 >> 8) & 0xFF);
		t3 = VIS_LD_U8_I(table0, (s4 >> 16) & 0xFF);
		t2 = VIS_LD_U8_I(table1, s4 >> 24);
		t1 = VIS_LD_U8_I(table0, s3 & 0xFF);
		t0 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	sp = (mlib_u8 *)sa;
	sp -= 1;
	dl = (mlib_u8 *)dp;

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
mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* source data */
	mlib_u32 s4, s5;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;
	const mlib_u8 *table2;
	mlib_s32 emask0, emask1, emask2;

	table2 = table0;
	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	i = 0;

	if (xsize >= 24) {

		s0 = sa[0];
		s1 = sa[1];
		s2 = sa[2];
		s3 = sa[3];
		s4 = sa[4];
		s5 = sa[5];
		sa += 6;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 6) {
			t15 = VIS_LD_U8_I(table1, s1 & 0xFF);
			t14 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
			t13 = VIS_LD_U8_I(table1, s1 >> 24);
			t12 = VIS_LD_U8_I(table0, s0 & 0xFF);
			t11 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
			t10 = VIS_LD_U8_I(table0, s0 >> 24);
			t9 = VIS_LD_U8_I(table0, s3 & 0xFF);
			t8 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
			t7 = VIS_LD_U8_I(table0, s3 >> 24);
			t6 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
			t5 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
			t4 = VIS_LD_U8_I(table1, (s5 >> 8) & 0xFF);
			t3 = VIS_LD_U8_I(table0, (s5 >> 16) & 0xFF);
			t2 = VIS_LD_U8_I(table1, s4 & 0xFF);
			t1 = VIS_LD_U8_I(table0, (s4 >> 8) & 0xFF);
			t0 = VIS_LD_U8_I(table1, s4 >> 24);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = sa[0];
			s1 = sa[1];
			s2 = sa[2];
			s3 = sa[3];
			s4 = sa[4];
			s5 = sa[5];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t13 = VIS_LD_U8_I(table1, s1 >> 24);
		t12 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t11 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
		t10 = VIS_LD_U8_I(table0, s0 >> 24);
		t9 = VIS_LD_U8_I(table0, s3 & 0xFF);
		t8 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
		t7 = VIS_LD_U8_I(table0, s3 >> 24);
		t6 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t5 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		t4 = VIS_LD_U8_I(table1, (s5 >> 8) & 0xFF);
		t3 = VIS_LD_U8_I(table0, (s5 >> 16) & 0xFF);
		t2 = VIS_LD_U8_I(table1, s4 & 0xFF);
		t1 = VIS_LD_U8_I(table0, (s4 >> 8) & 0xFF);
		t0 = VIS_LD_U8_I(table1, s4 >> 24);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	if (i <= xsize - 16) {
		s0 = sa[0];
		s1 = sa[1];
		s2 = sa[2];
		s3 = sa[3];
		sa += 4;
		t15 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t13 = VIS_LD_U8_I(table1, s1 >> 24);
		t12 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t11 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
		t10 = VIS_LD_U8_I(table0, s0 >> 24);
		t9 = VIS_LD_U8_I(table0, s3 & 0xFF);
		t8 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
		t7 = VIS_LD_U8_I(table0, s3 >> 24);
		t6 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t5 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		table2 = table0;
		table0 = table1;
		emask0 = emask2;
	} else if (i <= xsize - 8) {
		s0 = sa[0];
		s1 = sa[1];
		sa += 2;
		t15 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t13 = VIS_LD_U8_I(table1, s1 >> 24);
		t12 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t11 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
		t10 = VIS_LD_U8_I(table0, s0 >> 24);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		vis_pst_8(acc0, dp++, emask0);
		table2 = table1;
		table1 = table0;
		emask0 = emask1;
	}

	sp = (mlib_u8 *)sa;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;
		i = num - 3 * (num / 3);

		if (i == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if (i == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		}
#pragma pipeloop(0)
		for (i = 0; i < num; i += 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & emask0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* source data */
	mlib_u32 s4, s5, s6;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;
	const mlib_u8 *table2;
	mlib_s32 emask0, emask1, emask2;

	table2 = table0;
	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	s0 = (*sa++);

	i = 0;

	if (xsize >= 24) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		s5 = sa[4];
		s6 = sa[5];
		sa += 6;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 6) {
			t15 = VIS_LD_U8_I(table1, s2 >> 24);
			t14 = VIS_LD_U8_I(table0, s1 & 0xFF);
			t13 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
			t12 = VIS_LD_U8_I(table0, s1 >> 24);
			t11 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
			t10 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
			t9 = VIS_LD_U8_I(table0, s4 >> 24);
			t8 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
			t7 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
			t6 = VIS_LD_U8_I(table1, s2 & 0xFF);
			t5 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
			t4 = VIS_LD_U8_I(table1, s5 & 0xFF);
			t3 = VIS_LD_U8_I(table0, (s5 >> 8) & 0xFF);
			t2 = VIS_LD_U8_I(table1, s5 >> 24);
			t1 = VIS_LD_U8_I(table0, s4 & 0xFF);
			t0 = VIS_LD_U8_I(table1, (s4 >> 16) & 0xFF);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = s6;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			s4 = sa[3];
			s5 = sa[4];
			s6 = sa[5];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, s2 >> 24);
		t14 = VIS_LD_U8_I(table0, s1 & 0xFF);
		t13 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
		t12 = VIS_LD_U8_I(table0, s1 >> 24);
		t11 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
		t9 = VIS_LD_U8_I(table0, s4 >> 24);
		t8 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
		t7 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
		t6 = VIS_LD_U8_I(table1, s2 & 0xFF);
		t5 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
		t4 = VIS_LD_U8_I(table1, s5 & 0xFF);
		t3 = VIS_LD_U8_I(table0, (s5 >> 8) & 0xFF);
		t2 = VIS_LD_U8_I(table1, s5 >> 24);
		t1 = VIS_LD_U8_I(table0, s4 & 0xFF);
		t0 = VIS_LD_U8_I(table1, (s4 >> 16) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		s0 = s6;
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	if (i <= xsize - 16) {
		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		sa += 4;
		t15 = VIS_LD_U8_I(table1, s2 >> 24);
		t14 = VIS_LD_U8_I(table0, s1 & 0xFF);
		t13 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
		t12 = VIS_LD_U8_I(table0, s1 >> 24);
		t11 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
		t9 = VIS_LD_U8_I(table0, s4 >> 24);
		t8 = VIS_LD_U8_I(table1, (s3 >> 8) & 0xFF);
		t7 = VIS_LD_U8_I(table0, (s3 >> 16) & 0xFF);
		t6 = VIS_LD_U8_I(table1, s2 & 0xFF);
		t5 = VIS_LD_U8_I(table0, (s2 >> 8) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		table2 = table0;
		table0 = table1;
		emask0 = emask2;
	} else if (i <= xsize - 8) {
		s1 = sa[0];
		s2 = sa[1];
		sa += 2;
		t15 = VIS_LD_U8_I(table1, s2 >> 24);
		t14 = VIS_LD_U8_I(table0, s1 & 0xFF);
		t13 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
		t12 = VIS_LD_U8_I(table0, s1 >> 24);
		t11 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		vis_pst_8(acc0, dp++, emask0);
		table2 = table1;
		table1 = table0;
		emask0 = emask1;
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;
		i = num - 3 * (num / 3);

		if (i == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if (i == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		}
#pragma pipeloop(0)
		for (i = 0; i < num; i += 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & emask0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* source data */
	mlib_u32 s4, s5, s6;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;
	const mlib_u8 *table2;
	mlib_s32 emask0, emask1, emask2;

	table2 = table0;
	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	i = 0;

	s0 = (*sa++);

	if (xsize >= 24) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		s5 = sa[4];
		s6 = sa[5];
		sa += 6;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 6) {
			t15 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
			t14 = VIS_LD_U8_I(table0, s2 >> 24);
			t13 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
			t12 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
			t11 = VIS_LD_U8_I(table1, s0 & 0xFF);
			t10 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
			t9 = VIS_LD_U8_I(table0, (s4 >> 16) & 0xFF);
			t8 = VIS_LD_U8_I(table1, s3 & 0xFF);
			t7 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
			t6 = VIS_LD_U8_I(table1, s3 >> 24);
			t5 = VIS_LD_U8_I(table0, s2 & 0xFF);
			t4 = VIS_LD_U8_I(table1, s6 >> 24);
			t3 = VIS_LD_U8_I(table0, s5 & 0xFF);
			t2 = VIS_LD_U8_I(table1, (s5 >> 16) & 0xFF);
			t1 = VIS_LD_U8_I(table0, s5 >> 24);
			t0 = VIS_LD_U8_I(table1, (s4 >> 8) & 0xFF);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = s6;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			s4 = sa[3];
			s5 = sa[4];
			s6 = sa[5];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
		t14 = VIS_LD_U8_I(table0, s2 >> 24);
		t13 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
		t11 = VIS_LD_U8_I(table1, s0 & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
		t9 = VIS_LD_U8_I(table0, (s4 >> 16) & 0xFF);
		t8 = VIS_LD_U8_I(table1, s3 & 0xFF);
		t7 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
		t6 = VIS_LD_U8_I(table1, s3 >> 24);
		t5 = VIS_LD_U8_I(table0, s2 & 0xFF);
		t4 = VIS_LD_U8_I(table1, s6 >> 24);
		t3 = VIS_LD_U8_I(table0, s5 & 0xFF);
		t2 = VIS_LD_U8_I(table1, (s5 >> 16) & 0xFF);
		t1 = VIS_LD_U8_I(table0, s5 >> 24);
		t0 = VIS_LD_U8_I(table1, (s4 >> 8) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		s0 = s6;
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	if (i <= xsize - 16) {
		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		sa += 4;
		t15 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
		t14 = VIS_LD_U8_I(table0, s2 >> 24);
		t13 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
		t11 = VIS_LD_U8_I(table1, s0 & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
		t9 = VIS_LD_U8_I(table0, (s4 >> 16) & 0xFF);
		t8 = VIS_LD_U8_I(table1, s3 & 0xFF);
		t7 = VIS_LD_U8_I(table0, (s3 >> 8) & 0xFF);
		t6 = VIS_LD_U8_I(table1, s3 >> 24);
		t5 = VIS_LD_U8_I(table0, s2 & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		table2 = table0;
		table0 = table1;
		emask0 = emask2;
	} else if (i <= xsize - 8) {
		s1 = sa[0];
		s2 = sa[1];
		sa += 2;
		t15 = VIS_LD_U8_I(table1, (s2 >> 16) & 0xFF);
		t14 = VIS_LD_U8_I(table0, s2 >> 24);
		t13 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
		t11 = VIS_LD_U8_I(table1, s0 & 0xFF);
		t10 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		vis_pst_8(acc0, dp++, emask0);
		table2 = table1;
		table1 = table0;
		emask0 = emask1;
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;
		i = num - 3 * (num / 3);

		if (i == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if (i == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		}
#pragma pipeloop(0)
		for (i = 0; i < num; i += 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & emask0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_3_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* source data */
	mlib_u32 s2, s3;

/* source data */
	mlib_u32 s4, s5, s6;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* destination data */
	mlib_d64 t8, t9, t10;

/* destination data */
	mlib_d64 t11, t12, t13;

/* destination data */
	mlib_d64 t14, t15;
	mlib_d64 acc0, acc1, acc2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;
	const mlib_u8 *table2;
	mlib_s32 emask0, emask1, emask2;

	table2 = table0;
	emask0 = 0xDB;
	emask1 = 0x6D;
	emask2 = 0xB6;

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	i = 0;

	s0 = (*sa++);

	if (xsize >= 24) {

		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		s5 = sa[4];
		s6 = sa[5];
		sa += 6;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 48; i += 24, sa += 6) {
			t15 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
			t14 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
			t13 = VIS_LD_U8_I(table1, s1 & 0xFF);
			t12 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
			t11 = VIS_LD_U8_I(table1, s1 >> 24);
			t10 = VIS_LD_U8_I(table0, s0 & 0xFF);
			t9 = VIS_LD_U8_I(table0, (s4 >> 8) & 0xFF);
			t8 = VIS_LD_U8_I(table1, s4 >> 24);
			t7 = VIS_LD_U8_I(table0, s3 & 0xFF);
			t6 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
			t5 = VIS_LD_U8_I(table0, s3 >> 24);
			t4 = VIS_LD_U8_I(table1, (s6 >> 16) & 0xFF);
			t3 = VIS_LD_U8_I(table0, s6 >> 24);
			t2 = VIS_LD_U8_I(table1, (s5 >> 8) & 0xFF);
			t1 = VIS_LD_U8_I(table0, (s5 >> 16) & 0xFF);
			t0 = VIS_LD_U8_I(table1, s4 & 0xFF);
			acc0 = vis_faligndata(t15, acc0);
			acc0 = vis_faligndata(t14, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t13, acc0);
			acc0 = vis_faligndata(t12, acc0);
			acc0 = vis_faligndata(acc0, acc0);
			acc0 = vis_faligndata(t11, acc0);
			acc0 = vis_faligndata(t10, acc0);
			acc1 = vis_faligndata(t9, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t8, acc1);
			acc1 = vis_faligndata(t7, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc1 = vis_faligndata(t6, acc1);
			acc1 = vis_faligndata(t5, acc1);
			acc1 = vis_faligndata(acc1, acc1);
			acc2 = vis_faligndata(t4, acc2);
			acc2 = vis_faligndata(t3, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t2, acc2);
			acc2 = vis_faligndata(t1, acc2);
			acc2 = vis_faligndata(acc2, acc2);
			acc2 = vis_faligndata(t0, acc2);
			s0 = s6;
			s1 = sa[0];
			s2 = sa[1];
			s3 = sa[2];
			s4 = sa[3];
			s5 = sa[4];
			s6 = sa[5];
			vis_pst_8(acc0, dp++, emask0);
			vis_pst_8(acc1, dp++, emask1);
			vis_pst_8(acc2, dp++, emask2);
		}

		t15 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		t13 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t11 = VIS_LD_U8_I(table1, s1 >> 24);
		t10 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t9 = VIS_LD_U8_I(table0, (s4 >> 8) & 0xFF);
		t8 = VIS_LD_U8_I(table1, s4 >> 24);
		t7 = VIS_LD_U8_I(table0, s3 & 0xFF);
		t6 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
		t5 = VIS_LD_U8_I(table0, s3 >> 24);
		t4 = VIS_LD_U8_I(table1, (s6 >> 16) & 0xFF);
		t3 = VIS_LD_U8_I(table0, s6 >> 24);
		t2 = VIS_LD_U8_I(table1, (s5 >> 8) & 0xFF);
		t1 = VIS_LD_U8_I(table0, (s5 >> 16) & 0xFF);
		t0 = VIS_LD_U8_I(table1, s4 & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc2 = vis_faligndata(t4, acc2);
		acc2 = vis_faligndata(t3, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t2, acc2);
		acc2 = vis_faligndata(t1, acc2);
		acc2 = vis_faligndata(acc2, acc2);
		acc2 = vis_faligndata(t0, acc2);
		s0 = s6;
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		vis_pst_8(acc2, dp++, emask2);
		i += 24;
	}

	if (i <= xsize - 16) {
		s1 = sa[0];
		s2 = sa[1];
		s3 = sa[2];
		s4 = sa[3];
		sa += 4;
		t15 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		t13 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t11 = VIS_LD_U8_I(table1, s1 >> 24);
		t10 = VIS_LD_U8_I(table0, s0 & 0xFF);
		t9 = VIS_LD_U8_I(table0, (s4 >> 8) & 0xFF);
		t8 = VIS_LD_U8_I(table1, s4 >> 24);
		t7 = VIS_LD_U8_I(table0, s3 & 0xFF);
		t6 = VIS_LD_U8_I(table1, (s3 >> 16) & 0xFF);
		t5 = VIS_LD_U8_I(table0, s3 >> 24);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		acc1 = vis_faligndata(t9, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t8, acc1);
		acc1 = vis_faligndata(t7, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		acc1 = vis_faligndata(t6, acc1);
		acc1 = vis_faligndata(t5, acc1);
		acc1 = vis_faligndata(acc1, acc1);
		vis_pst_8(acc0, dp++, emask0);
		vis_pst_8(acc1, dp++, emask1);
		table2 = table0;
		table0 = table1;
		emask0 = emask2;
	} else if (i <= xsize - 8) {
		s1 = sa[0];
		s2 = sa[1];
		sa += 2;
		t15 = VIS_LD_U8_I(table1, (s2 >> 8) & 0xFF);
		t14 = VIS_LD_U8_I(table0, (s2 >> 16) & 0xFF);
		t13 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t12 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t11 = VIS_LD_U8_I(table1, s1 >> 24);
		t10 = VIS_LD_U8_I(table0, s0 & 0xFF);
		acc0 = vis_faligndata(t15, acc0);
		acc0 = vis_faligndata(t14, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t13, acc0);
		acc0 = vis_faligndata(t12, acc0);
		acc0 = vis_faligndata(acc0, acc0);
		acc0 = vis_faligndata(t11, acc0);
		acc0 = vis_faligndata(t10, acc0);
		vis_pst_8(acc0, dp++, emask0);
		table2 = table1;
		table1 = table0;
		emask0 = emask1;
	}

	sp = (mlib_u8 *)sa;
	sp--;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;
		i = num - 3 * (num / 3);

		if (i == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if (i == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		}
#pragma pipeloop(0)
		for (i = 0; i < num; i += 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & emask0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff0_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
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
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

	sa = (mlib_u32 *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	if (xsize >= 8) {

		s0 = sa[0];
		s1 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8, sa += 2) {
			t7 = VIS_LD_U8_I(table3, s1 & 0xFF);
			t6 = VIS_LD_U8_I(table2, (s1 >> 8) & 0xFF);
			t5 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
			t4 = VIS_LD_U8_I(table0, s1 >> 24);
			t3 = VIS_LD_U8_I(table3, s0 & 0xFF);
			t2 = VIS_LD_U8_I(table2, (s0 >> 8) & 0xFF);
			t1 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
			t0 = VIS_LD_U8_I(table0, s0 >> 24);
			acc = vis_faligndata(t7, acc);
			acc = vis_faligndata(t6, acc);
			acc = vis_faligndata(t5, acc);
			acc = vis_faligndata(t4, acc);
			acc = vis_faligndata(t3, acc);
			acc = vis_faligndata(t2, acc);
			acc = vis_faligndata(t1, acc);
			acc = vis_faligndata(t0, acc);
			s0 = sa[0];
			s1 = sa[1];
			vis_pst_8(acc, dp++, mask);
		}

		t7 = VIS_LD_U8_I(table3, s1 & 0xFF);
		t6 = VIS_LD_U8_I(table2, (s1 >> 8) & 0xFF);
		t5 = VIS_LD_U8_I(table1, (s1 >> 16) & 0xFF);
		t4 = VIS_LD_U8_I(table0, s1 >> 24);
		t3 = VIS_LD_U8_I(table3, s0 & 0xFF);
		t2 = VIS_LD_U8_I(table2, (s0 >> 8) & 0xFF);
		t1 = VIS_LD_U8_I(table1, (s0 >> 16) & 0xFF);
		t0 = VIS_LD_U8_I(table0, s0 >> 24);
		acc = vis_faligndata(t7, acc);
		acc = vis_faligndata(t6, acc);
		acc = vis_faligndata(t5, acc);
		acc = vis_faligndata(t4, acc);
		acc = vis_faligndata(t3, acc);
		acc = vis_faligndata(t2, acc);
		acc = vis_faligndata(t1, acc);
		acc = vis_faligndata(t0, acc);
		vis_pst_8(acc, dp++, mask);
	}

	sp = (mlib_u8 *)sa;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;

		if ((num & 3) == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		} else if ((num & 3) == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if ((num & 3) == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 3;
		}

		if (num != 0) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table3, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff1_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

	sa = (mlib_u32 *)(src - 1);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	s0 = (*sa++);

	if (xsize >= 8) {

		s1 = sa[0];
		s2 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8, sa += 2) {
			t7 = VIS_LD_U8_I(table3, s2 >> 24);
			t6 = VIS_LD_U8_I(table2, s1 & 0xFF);
			t5 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
			t4 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
			t3 = VIS_LD_U8_I(table3, s1 >> 24);
			t2 = VIS_LD_U8_I(table2, s0 & 0xFF);
			t1 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
			t0 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
			acc = vis_faligndata(t7, acc);
			acc = vis_faligndata(t6, acc);
			acc = vis_faligndata(t5, acc);
			acc = vis_faligndata(t4, acc);
			acc = vis_faligndata(t3, acc);
			acc = vis_faligndata(t2, acc);
			acc = vis_faligndata(t1, acc);
			acc = vis_faligndata(t0, acc);
			s0 = s2;
			s1 = sa[0];
			s2 = sa[1];
			vis_pst_8(acc, dp++, mask);
		}

		t7 = VIS_LD_U8_I(table3, s2 >> 24);
		t6 = VIS_LD_U8_I(table2, s1 & 0xFF);
		t5 = VIS_LD_U8_I(table1, (s1 >> 8) & 0xFF);
		t4 = VIS_LD_U8_I(table0, (s1 >> 16) & 0xFF);
		t3 = VIS_LD_U8_I(table3, s1 >> 24);
		t2 = VIS_LD_U8_I(table2, s0 & 0xFF);
		t1 = VIS_LD_U8_I(table1, (s0 >> 8) & 0xFF);
		t0 = VIS_LD_U8_I(table0, (s0 >> 16) & 0xFF);
		acc = vis_faligndata(t7, acc);
		acc = vis_faligndata(t6, acc);
		acc = vis_faligndata(t5, acc);
		acc = vis_faligndata(t4, acc);
		acc = vis_faligndata(t3, acc);
		acc = vis_faligndata(t2, acc);
		acc = vis_faligndata(t1, acc);
		acc = vis_faligndata(t0, acc);
		vis_pst_8(acc, dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 3;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;

		if ((num & 3) == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		} else if ((num & 3) == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if ((num & 3) == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 3;
		}

		if (num != 0) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table3, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff2_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

	sa = (mlib_u32 *)(src - 2);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	s0 = (*sa++);

	if (xsize >= 8) {

		s1 = sa[0];
		s2 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8, sa += 2) {
			t7 = VIS_LD_U8_I(table3, (s2 >> 16) & 0xFF);
			t6 = VIS_LD_U8_I(table2, s2 >> 24);
			t5 = VIS_LD_U8_I(table1, s1 & 0xFF);
			t4 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
			t3 = VIS_LD_U8_I(table3, (s1 >> 16) & 0xFF);
			t2 = VIS_LD_U8_I(table2, s1 >> 24);
			t1 = VIS_LD_U8_I(table1, s0 & 0xFF);
			t0 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
			acc = vis_faligndata(t7, acc);
			acc = vis_faligndata(t6, acc);
			acc = vis_faligndata(t5, acc);
			acc = vis_faligndata(t4, acc);
			acc = vis_faligndata(t3, acc);
			acc = vis_faligndata(t2, acc);
			acc = vis_faligndata(t1, acc);
			acc = vis_faligndata(t0, acc);
			s0 = s2;
			s1 = sa[0];
			s2 = sa[1];
			vis_pst_8(acc, dp++, mask);
		}

		t7 = VIS_LD_U8_I(table3, (s2 >> 16) & 0xFF);
		t6 = VIS_LD_U8_I(table2, s2 >> 24);
		t5 = VIS_LD_U8_I(table1, s1 & 0xFF);
		t4 = VIS_LD_U8_I(table0, (s1 >> 8) & 0xFF);
		t3 = VIS_LD_U8_I(table3, (s1 >> 16) & 0xFF);
		t2 = VIS_LD_U8_I(table2, s1 >> 24);
		t1 = VIS_LD_U8_I(table1, s0 & 0xFF);
		t0 = VIS_LD_U8_I(table0, (s0 >> 8) & 0xFF);
		acc = vis_faligndata(t7, acc);
		acc = vis_faligndata(t6, acc);
		acc = vis_faligndata(t5, acc);
		acc = vis_faligndata(t4, acc);
		acc = vis_faligndata(t3, acc);
		acc = vis_faligndata(t2, acc);
		acc = vis_faligndata(t1, acc);
		acc = vis_faligndata(t0, acc);
		vis_pst_8(acc, dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp -= 2;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;

		if ((num & 3) == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		} else if ((num & 3) == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if ((num & 3) == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 3;
		}

		if (num != 0) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table3, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & mask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpMaskNN_U8_U8_4_SrcOff3_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3,
    mlib_s32 mask)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1, s2;

/* pointer to start of destination */
	mlib_u8 *dl;

/* pointer to end of destination */
	mlib_u8 *dend;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, t4, t5;

/* destination data */
	mlib_d64 t6, t7, acc;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 i, num;

	sa = (mlib_u32 *)(src - 3);
	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	s0 = (*sa++);

	if (xsize >= 8) {

		s1 = sa[0];
		s2 = sa[1];
		sa += 2;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8, sa += 2) {
			t7 = VIS_LD_U8_I(table3, (s2 >> 8) & 0xFF);
			t6 = VIS_LD_U8_I(table2, (s2 >> 16) & 0xFF);
			t5 = VIS_LD_U8_I(table1, s2 >> 24);
			t4 = VIS_LD_U8_I(table0, s1 & 0xFF);
			t3 = VIS_LD_U8_I(table3, (s1 >> 8) & 0xFF);
			t2 = VIS_LD_U8_I(table2, (s1 >> 16) & 0xFF);
			t1 = VIS_LD_U8_I(table1, s1 >> 24);
			t0 = VIS_LD_U8_I(table0, s0 & 0xFF);
			acc = vis_faligndata(t7, acc);
			acc = vis_faligndata(t6, acc);
			acc = vis_faligndata(t5, acc);
			acc = vis_faligndata(t4, acc);
			acc = vis_faligndata(t3, acc);
			acc = vis_faligndata(t2, acc);
			acc = vis_faligndata(t1, acc);
			acc = vis_faligndata(t0, acc);
			s0 = s2;
			s1 = sa[0];
			s2 = sa[1];
			vis_pst_8(acc, dp++, mask);
		}

		t7 = VIS_LD_U8_I(table3, (s2 >> 8) & 0xFF);
		t6 = VIS_LD_U8_I(table2, (s2 >> 16) & 0xFF);
		t5 = VIS_LD_U8_I(table1, s2 >> 24);
		t4 = VIS_LD_U8_I(table0, s1 & 0xFF);
		t3 = VIS_LD_U8_I(table3, (s1 >> 8) & 0xFF);
		t2 = VIS_LD_U8_I(table2, (s1 >> 16) & 0xFF);
		t1 = VIS_LD_U8_I(table1, s1 >> 24);
		t0 = VIS_LD_U8_I(table0, s0 & 0xFF);
		acc = vis_faligndata(t7, acc);
		acc = vis_faligndata(t6, acc);
		acc = vis_faligndata(t5, acc);
		acc = vis_faligndata(t4, acc);
		acc = vis_faligndata(t3, acc);
		acc = vis_faligndata(t2, acc);
		acc = vis_faligndata(t1, acc);
		acc = vis_faligndata(t0, acc);
		vis_pst_8(acc, dp++, mask);
	}

	sp = (mlib_u8 *)sa;
	sp--;

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		num = (mlib_addr)dend - (mlib_addr)dp;
		sp += num;
		num++;

		if ((num & 3) == 1) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num--;
		} else if ((num & 3) == 2) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 2;
		} else if ((num & 3) == 3) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
			num -= 3;
		}

		if (num != 0) {
			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table3, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table2, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table1, s0);
			acc = vis_faligndata(t0, acc);

			s0 = (mlib_s32)*sp;
			sp--;

			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t0, acc);
		}

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc, dp, emask & mask);
	}
}

/* *********************************************************** */

#define	FUNC_NAME1(F)	mlib_v_ImageLookUpMask_U8_U8_##F
#define	FUNC_NAME2(F)	mlib_v_ImageLookUpMaskNN_U8_U8_##F


void
mlib_v_ImageLookUpMaskMN_U8_U8(
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
    mlib_s32 schan,
    mlib_s32 chan)
{
	if ((dchan == 3) && (schan == 2)) {
		mlib_u8 *sl;
		mlib_u8 *dl;
		const mlib_u8 *tab0;
		const mlib_u8 *tab1;
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
			mlib_u8 *dp = dl;
			mlib_u8 *sp1, *dp1;
			const mlib_u8 *tab2;
			mlib_s32 off, size = xsize;

			off = ((mlib_addr)(dp + dst_off) & 7);
			off = (5 * off) & 7;
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
			dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else if (dchan == 3) {
		mlib_u8 *sl;
		mlib_u8 *dl;
		const mlib_u8 *tab0;
		const mlib_u8 *tab1;
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
			mlib_u8 *dp = dl;
			mlib_u8 *sp1, *dp1;
			const mlib_u8 *tab2;
			mlib_s32 off, size = xsize;

			off = ((mlib_addr)(dp + dst_off) & 7);
			off = (5 * off) & 7;
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
			dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		mlib_u8 *sl;
		mlib_u8 *dl;
		const mlib_u8 *t0 = table[table_i[0]];
		const mlib_u8 *t1 = table[table_i[1]];
		const mlib_u8 *t2 = table[table_i[2]];
		mlib_u8 *tabl[4];
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
			mlib_u8 *dp = dl;
			mlib_u8 *tab0 = tabl[0];
			mlib_u8 *tab1 = tabl[1];
			mlib_u8 *tab2 = tabl[2];
			mlib_u8 *tab3 = tabl[3];
			mlib_u8 *tab;
			mlib_s32 mask = cmask;
			mlib_s32 off, size = xsize * 4;

			off = (8 - ((mlib_addr)dp & 7)) & 7;

			off = (off < size) ? off : size;

			if (off >= 4) {
				dp[d0] = t0[sp[d0]];
				dp[d1] = t1[sp[d1]];
				dp[d2] = t2[sp[d2]];
				size -= 4;
				off -= 4;
				dp += 4;
				sp += 4;
			}

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
			dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
