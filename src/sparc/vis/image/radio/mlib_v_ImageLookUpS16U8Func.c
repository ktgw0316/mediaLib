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

#pragma ident	"@(#)mlib_v_ImageLookUpS16U8Func.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUp_S16_U8_124_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3);

static void mlib_v_ImageLookUp_S16_U8_3_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2);

/* *********************************************************** */

#define	VIS_LD_U8_I(X, Y)	vis_ld_u8_i((void *)(X), (Y))

/* *********************************************************** */

void
mlib_v_ImageLookUp_S16_U8_124_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2,
    const mlib_u8 *table3)
{
/* pointer to source data */
	mlib_s16 *sp;

/* source data */
	mlib_s32 s0, s1, s2, s3;

/* source data */
	mlib_s32 s4, s5, s6, s7;

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

	dl = dst;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;
	sp = (void *)src;

	vis_alignaddr((void *)0, 7);

	if (xsize >= 8) {

		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];
		s4 = sp[4];
		s5 = sp[5];
		s6 = sp[6];
		s7 = sp[7];
		sp += 8;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8, sp += 8) {
			t7 = VIS_LD_U8_I(table3, s7);
			t6 = VIS_LD_U8_I(table2, s6);
			t5 = VIS_LD_U8_I(table1, s5);
			t4 = VIS_LD_U8_I(table0, s4);
			t3 = VIS_LD_U8_I(table3, s3);
			t2 = VIS_LD_U8_I(table2, s2);
			t1 = VIS_LD_U8_I(table1, s1);
			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t7, acc);
			acc = vis_faligndata(t6, acc);
			acc = vis_faligndata(t5, acc);
			acc = vis_faligndata(t4, acc);
			acc = vis_faligndata(t3, acc);
			acc = vis_faligndata(t2, acc);
			acc = vis_faligndata(t1, acc);
			acc = vis_faligndata(t0, acc);
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s3 = sp[3];
			s4 = sp[4];
			s5 = sp[5];
			s6 = sp[6];
			s7 = sp[7];
			(*dp++) = acc;
		}

		t7 = VIS_LD_U8_I(table3, s7);
		t6 = VIS_LD_U8_I(table2, s6);
		t5 = VIS_LD_U8_I(table1, s5);
		t4 = VIS_LD_U8_I(table0, s4);
		t3 = VIS_LD_U8_I(table3, s3);
		t2 = VIS_LD_U8_I(table2, s2);
		t1 = VIS_LD_U8_I(table1, s1);
		t0 = VIS_LD_U8_I(table0, s0);
		acc = vis_faligndata(t7, acc);
		acc = vis_faligndata(t6, acc);
		acc = vis_faligndata(t5, acc);
		acc = vis_faligndata(t4, acc);
		acc = vis_faligndata(t3, acc);
		acc = vis_faligndata(t2, acc);
		acc = vis_faligndata(t1, acc);
		acc = vis_faligndata(t0, acc);
		(*dp++) = acc;
	}

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
		vis_pst_8(acc, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUp_S16_U8_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table)
{
	mlib_s16 *sl;
	mlib_u8 *dl;
	const mlib_u8 *tab = &table[0][32768];
	mlib_s32 j, i;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_u8 *dp = dl;
		mlib_s32 off, size = xsize;

		off = (8 - ((mlib_addr)dp & 7)) & 7;

		off = (off < size) ? off : size;

		for (i = 0; i < off; i++, sp++) {
			(*dp++) = tab[sp[0]];
			size--;
		}

		if (size > 0) {
			mlib_v_ImageLookUp_S16_U8_124_D1(sp, dp, size, tab, tab,
			    tab, tab);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUp_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table)
{
	mlib_s16 *sl;
	mlib_u8 *dl;
	const mlib_u8 *tab;
	mlib_s32 j, i;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_u8 *dp = dl;
		mlib_s32 off, size = xsize * 2;
		const mlib_u8 *tab0 = &table[0][32768];
		const mlib_u8 *tab1 = &table[1][32768];

		off = (8 - ((mlib_addr)dp & 7)) & 7;

		off = (off < size) ? off : size;

		for (i = 0; i < off - 1; i += 2, sp += 2) {
			(*dp++) = tab0[sp[0]];
			(*dp++) = tab1[sp[1]];
			size -= 2;
		}

		if ((off & 1) != 0) {
			(*dp++) = tab0[sp[0]];
			size--;
			sp++;
			tab = tab0;
			tab0 = tab1;
			tab1 = tab;
		}

		if (size > 0) {
			mlib_v_ImageLookUp_S16_U8_124_D1(sp, dp, size, tab0,
			    tab1, tab0, tab1);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUp_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table)
{
	mlib_s16 *sl;
	mlib_u8 *dl;
	const mlib_u8 *tab;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_u8 *dp = dl;
		const mlib_u8 *tab0 = &table[0][32768];
		const mlib_u8 *tab1 = &table[1][32768];
		const mlib_u8 *tab2 = &table[2][32768];
		const mlib_u8 *tab3 = &table[3][32768];
		mlib_s32 off, size = xsize * 4;

		off = (8 - ((mlib_addr)dp & 7)) & 7;

		off = (off < size) ? off : size;

		if (off >= 4) {
			(*dp++) = tab0[sp[0]];
			(*dp++) = tab1[sp[1]];
			(*dp++) = tab2[sp[2]];
			(*dp++) = tab3[sp[3]];
			size -= 4;
			off -= 4;
			sp += 4;
		}

		if (off == 1) {
			(*dp++) = tab0[sp[0]];
			tab = tab0;
			tab0 = tab1;
			tab1 = tab2;
			tab2 = tab3;
			tab3 = tab;
			size--;
			sp++;
		} else if (off == 2) {
			(*dp++) = tab0[sp[0]];
			(*dp++) = tab1[sp[1]];
			tab = tab0;
			tab0 = tab2;
			tab2 = tab;
			tab = tab1;
			tab1 = tab3;
			tab3 = tab;
			size -= 2;
			sp += 2;
		} else if (off == 3) {
			(*dp++) = tab0[sp[0]];
			(*dp++) = tab1[sp[1]];
			(*dp++) = tab2[sp[2]];
			tab = tab3;
			tab3 = tab2;
			tab2 = tab1;
			tab1 = tab0;
			tab0 = tab;
			size -= 3;
			sp += 3;
		}

		if (size > 0) {
			mlib_v_ImageLookUp_S16_U8_124_D1(sp, dp, size, tab0,
			    tab1, tab2, tab3);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUp_S16_U8_3_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 xsize,
    const mlib_u8 *table0,
    const mlib_u8 *table1,
    const mlib_u8 *table2)
{
/* pointer to source data */
	mlib_s16 *sp;

/* source data */
	mlib_s32 s0, s1, s2, s3;

/* source data */
	mlib_s32 s4, s5, s6, s7;

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
	const mlib_u8 *table;

	dl = dst;
	sp = (void *)src;
	dp = (mlib_d64 *)dl;
	dend = dl + xsize - 1;

	vis_alignaddr((void *)0, 7);

	if (xsize >= 8) {

		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];
		s4 = sp[4];
		s5 = sp[5];
		s6 = sp[6];
		s7 = sp[7];
		sp += 8;

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 16; i += 8, sp += 8) {
			t7 = VIS_LD_U8_I(table1, s7);
			t6 = VIS_LD_U8_I(table0, s6);
			t5 = VIS_LD_U8_I(table2, s5);
			t4 = VIS_LD_U8_I(table1, s4);
			t3 = VIS_LD_U8_I(table0, s3);
			t2 = VIS_LD_U8_I(table2, s2);
			t1 = VIS_LD_U8_I(table1, s1);
			t0 = VIS_LD_U8_I(table0, s0);
			acc = vis_faligndata(t7, acc);
			acc = vis_faligndata(t6, acc);
			acc = vis_faligndata(t5, acc);
			acc = vis_faligndata(t4, acc);
			acc = vis_faligndata(t3, acc);
			acc = vis_faligndata(t2, acc);
			acc = vis_faligndata(t1, acc);
			acc = vis_faligndata(t0, acc);
			table = table0;
			table0 = table2;
			table2 = table1;
			table1 = table;
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s3 = sp[3];
			s4 = sp[4];
			s5 = sp[5];
			s6 = sp[6];
			s7 = sp[7];
			(*dp++) = acc;
		}

		t7 = VIS_LD_U8_I(table1, s7);
		t6 = VIS_LD_U8_I(table0, s6);
		t5 = VIS_LD_U8_I(table2, s5);
		t4 = VIS_LD_U8_I(table1, s4);
		t3 = VIS_LD_U8_I(table0, s3);
		t2 = VIS_LD_U8_I(table2, s2);
		t1 = VIS_LD_U8_I(table1, s1);
		t0 = VIS_LD_U8_I(table0, s0);
		acc = vis_faligndata(t7, acc);
		acc = vis_faligndata(t6, acc);
		acc = vis_faligndata(t5, acc);
		acc = vis_faligndata(t4, acc);
		acc = vis_faligndata(t3, acc);
		acc = vis_faligndata(t2, acc);
		acc = vis_faligndata(t1, acc);
		acc = vis_faligndata(t0, acc);
		table = table0;
		table0 = table2;
		table2 = table1;
		table1 = table;
		(*dp++) = acc;
	}

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
		vis_pst_8(acc, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUp_S16_U8_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table)
{
	mlib_s16 *sl;
	mlib_u8 *dl;
	const mlib_u8 *tab;
	mlib_s32 j, i;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_u8 *dp = dl;
		const mlib_u8 *tab0 = &table[0][32768];
		const mlib_u8 *tab1 = &table[1][32768];
		const mlib_u8 *tab2 = &table[2][32768];
		mlib_s32 off, size = xsize * 3;

		off = (8 - ((mlib_addr)dp & 7)) & 7;

		off = (off < size) ? off : size;

		for (i = 0; i < off - 2; i += 3, sp += 3) {
			(*dp++) = tab0[sp[0]];
			(*dp++) = tab1[sp[1]];
			(*dp++) = tab2[sp[2]];
			size -= 3;
		}

		off -= i;

		if (off == 1) {
			(*dp++) = tab0[sp[0]];
			tab = tab0;
			tab0 = tab1;
			tab1 = tab2;
			tab2 = tab;
			size--;
			sp++;
		} else if (off == 2) {
			(*dp++) = tab0[sp[0]];
			(*dp++) = tab1[sp[1]];
			tab = tab2;
			tab2 = tab1;
			tab1 = tab0;
			tab0 = tab;
			size -= 2;
			sp += 2;
		}

		if (size > 0) {
			mlib_v_ImageLookUp_S16_U8_3_D1(sp, dp, size, tab0, tab1,
			    tab2);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
