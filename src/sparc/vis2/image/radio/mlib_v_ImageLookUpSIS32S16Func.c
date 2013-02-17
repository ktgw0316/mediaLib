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

#pragma ident	"@(#)mlib_v_ImageLookUpSIS32S16Func.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLookUpFunc.h>

/* *********************************************************** */

static void mlib_v_ImageLookUpSI_S32_S16_2_DstA8D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S32_S16_2_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S32_S16_3_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S32_S16_4_DstOff0_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S32_S16_4_DstOff1_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S32_S16_4_DstOff2_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S32_S16_4_DstOff3_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table);

/* *********************************************************** */

#define	HALF_U64	(MLIB_U64_CONST(2147483648) * sizeof (table[0][0]))
#define	VIS_LD_U16_I(X, Y)	vis_ld_u16_i((void *)(X), (Y))

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_2_DstA8D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* source data */
	mlib_s32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, acc0;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);

/* destination data */
	mlib_d64 acc1;

	sp = (void *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	if (xsize >= 2) {

		s0 = sp[0];
		s1 = sp[1];
		sp += 2;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 4; i += 2, sp += 2) {
			t3 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
			t2 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
			t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
			t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s0));
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = sp[0];
			s1 = sp[1];
			(*dp++) = vis_bshuffle(acc0, acc1);
		}

		t3 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
		t2 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
		t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
		t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s0));
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		(*dp++) = vis_bshuffle(acc0, acc1);
	}

	if ((xsize & 1) != 0) {
		s0 = sp[0];
		t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
		t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s0));
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		*(mlib_f32 *)dp = vis_read_hi(acc0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_2_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* source data */
	mlib_s32 s0, s1, s2;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2;

/* destination data */
	mlib_d64 t3, acc0;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);

/* destination data */
	mlib_d64 acc1;

	sp = (void *)src;
	dl = dst;

	vis_alignaddr((void *)0, 6);

	s0 = (*sp++);
	(*dl++) = tab0[s0];
	dp = (mlib_d64 *)dl;
	xsize--;

	if (xsize >= 2) {

		s1 = sp[0];
		s2 = sp[1];
		sp += 2;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 4; i += 2, sp += 2) {
			t3 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s2));
			t2 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
			t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
			t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s2;
			s1 = sp[0];
			s2 = sp[1];
			(*dp++) = vis_bshuffle(acc0, acc1);
		}

		t3 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s2));
		t2 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
		t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
		t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s2;
		(*dp++) = vis_bshuffle(acc0, acc1);
	}

	dl = (mlib_s16 *)dp;

	if ((xsize & 1) != 0) {
		s1 = sp[0];
		t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
		t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		*(mlib_f32 *)dp = vis_read_hi(acc0);
		s0 = s1;
		dl += 2;
	}

	*dl = tab1[s0];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table)
{
	mlib_s32 *sl;
	mlib_s16 *dl;
	mlib_s32 j;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s16 *dp = dl;
		mlib_s32 off, s0, size = xsize;

		off = (mlib_s32)(((8 - ((mlib_addr)dp & 7)) & 7));

		if ((off >= 4) && (size > 0)) {
			s0 = (*sp++);
			(*dp++) = tab0[s0];
			(*dp++) = tab1[s0];
			size--;
		}

		if (size > 0) {

			if (((mlib_addr)dp & 7) == 0) {
				mlib_v_ImageLookUpSI_S32_S16_2_DstA8D1(sp, dp,
				    size, table);
			} else {
				mlib_v_ImageLookUpSI_S32_S16_2_D1(sp, dp, size,
				    table);
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_3_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc00, acc01, acc02;

/* destination data */
	mlib_d64 acc10, acc11, acc12;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);
	mlib_s32 s00, s01, s02, s03;

	sp = (void *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	i = 0;

	if (xsize >= 4) {

		s00 = sp[0];
		s01 = sp[1];
		s02 = sp[2];
		s03 = sp[3];
		sp += 4;

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 8; i += 4, sp += 4) {
			t3 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s01));
			t2 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s00));
			t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s00));
			t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s00));
			acc10 = vis_faligndata(t3, acc10);
			acc10 = vis_faligndata(t2, acc10);
			acc00 = vis_faligndata(t1, acc00);
			acc00 = vis_faligndata(t0, acc00);
			t3 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s02));
			t2 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s02));
			t1 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s01));
			t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s01));
			acc11 = vis_faligndata(t3, acc11);
			acc11 = vis_faligndata(t2, acc11);
			acc01 = vis_faligndata(t1, acc01);
			acc01 = vis_faligndata(t0, acc01);
			t3 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s03));
			t2 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s03));
			t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s03));
			t0 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s02));
			acc12 = vis_faligndata(t3, acc12);
			acc12 = vis_faligndata(t2, acc12);
			acc02 = vis_faligndata(t1, acc02);
			acc02 = vis_faligndata(t0, acc02);
			s00 = sp[0];
			s01 = sp[1];
			s02 = sp[2];
			s03 = sp[3];
			(*dp++) = vis_bshuffle(acc00, acc10);
			(*dp++) = vis_bshuffle(acc01, acc11);
			(*dp++) = vis_bshuffle(acc02, acc12);
		}

		t3 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s01));
		t2 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s00));
		t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s00));
		t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s00));
		acc10 = vis_faligndata(t3, acc10);
		acc10 = vis_faligndata(t2, acc10);
		acc00 = vis_faligndata(t1, acc00);
		acc00 = vis_faligndata(t0, acc00);
		t3 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s02));
		t2 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s02));
		t1 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s01));
		t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s01));
		acc11 = vis_faligndata(t3, acc11);
		acc11 = vis_faligndata(t2, acc11);
		acc01 = vis_faligndata(t1, acc01);
		acc01 = vis_faligndata(t0, acc01);
		t3 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s03));
		t2 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s03));
		t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s03));
		t0 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s02));
		acc12 = vis_faligndata(t3, acc12);
		acc12 = vis_faligndata(t2, acc12);
		acc02 = vis_faligndata(t1, acc02);
		acc02 = vis_faligndata(t0, acc02);
		(*dp++) = vis_bshuffle(acc00, acc10);
		(*dp++) = vis_bshuffle(acc01, acc11);
		(*dp++) = vis_bshuffle(acc02, acc12);
		i += 4;
	}

	dl = (mlib_s16 *)dp;

#pragma pipeloop(0)
	for (; i < xsize; i++) {
		s00 = sp[0];
		dl[0] = tab0[s00];
		dl[1] = tab1[s00];
		dl[2] = tab2[s00];
		dl += 3;
		sp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table)
{
	mlib_s32 *sl;
	mlib_s16 *dl;
	mlib_s32 i, j;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s16 *dp = dl;
		mlib_s32 off, s0, size = xsize;

		off = (mlib_s32)(((mlib_addr)dp & 7) >> 1);
		off = (off < size) ? off : size;

		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			(*dp++) = tab0[s0];
			(*dp++) = tab1[s0];
			(*dp++) = tab2[s0];
			size--;
		}

		if (size > 0) {
			mlib_v_ImageLookUpSI_S32_S16_3_D1(sp, dp, size, table);
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_4_DstOff0_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* source data */
	mlib_s32 s0;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);
	const mlib_s16 *tab3 = (void *)&(((mlib_u8 **)table)[3][HALF_U64]);

/* destination data */
	mlib_d64 acc1;

	sp = (void *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	if (xsize >= 1) {

		s0 = (*sp++);

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 2; i++) {
			t3 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
			t2 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s0));
			t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
			t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s0));
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = (*sp++);
			(*dp++) = vis_bshuffle(acc0, acc1);
		}

		t3 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
		t2 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s0));
		t1 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
		t0 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s0));
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		(*dp++) = vis_bshuffle(acc0, acc1);
	}
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_4_DstOff1_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* source data */
	mlib_s32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);
	const mlib_s16 *tab3 = (void *)&(((mlib_u8 **)table)[3][HALF_U64]);

/* destination data */
	mlib_d64 acc1;

	sp = (void *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	s0 = (*sp++);

	if (xsize >= 1) {

		s1 = (*sp++);

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 2; i++) {
			t3 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
			t2 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
			t1 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s0));
			t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = (*sp++);
			(*dp++) = vis_bshuffle(acc0, acc1);
		}

		t3 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
		t2 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
		t1 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s0));
		t0 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s0));
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s1;
		(*dp++) = vis_bshuffle(acc0, acc1);
	}

	dl = (mlib_s16 *)dp;

	dl[0] = tab1[s0];
	dl[1] = tab2[s0];
	dl[2] = tab3[s0];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_4_DstOff2_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* source data */
	mlib_s32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);
	const mlib_s16 *tab3 = (void *)&(((mlib_u8 **)table)[3][HALF_U64]);

/* destination data */
	mlib_d64 acc1;

	sp = (void *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	s0 = (*sp++);

	if (xsize >= 1) {

		s1 = (*sp++);

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 2; i++) {
			t3 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
			t2 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
			t1 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
			t0 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s0));
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = (*sp++);
			(*dp++) = vis_bshuffle(acc0, acc1);
		}

		t3 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
		t2 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
		t1 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
		t0 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s0));
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s1;
		(*dp++) = vis_bshuffle(acc0, acc1);
	}

	dl = (mlib_s16 *)dp;

	dl[0] = tab2[s0];
	dl[1] = tab3[s0];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_4_DstOff3_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 xsize,
    const mlib_s16 **table)
{
/* pointer to source data */
	mlib_s32 *sp;

/* source data */
	mlib_s32 s0, s1;

/* pointer to start of destination */
	mlib_s16 *dl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* destination data */
	mlib_d64 t0, t1, t2, t3;

/* destination data */
	mlib_d64 acc0;

/* loop variable */
	mlib_s32 i;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);
	const mlib_s16 *tab3 = (void *)&(((mlib_u8 **)table)[3][HALF_U64]);

/* destination data */
	mlib_d64 acc1;

	sp = (void *)src;
	dl = dst;
	dp = (mlib_d64 *)dl;

	vis_alignaddr((void *)0, 6);

	s0 = (*sp++);

	if (xsize >= 1) {

		s1 = (*sp++);

		vis_write_bmask(0x012389ab, 0);

#pragma pipeloop(0)
		for (i = 0; i <= xsize - 2; i++) {
			t3 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s1));
			t2 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
			t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
			t0 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
			acc1 = vis_faligndata(t3, acc1);
			acc1 = vis_faligndata(t2, acc1);
			acc0 = vis_faligndata(t1, acc0);
			acc0 = vis_faligndata(t0, acc0);
			s0 = s1;
			s1 = (*sp++);
			(*dp++) = vis_bshuffle(acc0, acc1);
		}

		t3 = VIS_LD_U16_I(tab2, ((mlib_addr)2 * s1));
		t2 = VIS_LD_U16_I(tab1, ((mlib_addr)2 * s1));
		t1 = VIS_LD_U16_I(tab0, ((mlib_addr)2 * s1));
		t0 = VIS_LD_U16_I(tab3, ((mlib_addr)2 * s0));
		acc1 = vis_faligndata(t3, acc1);
		acc1 = vis_faligndata(t2, acc1);
		acc0 = vis_faligndata(t1, acc0);
		acc0 = vis_faligndata(t0, acc0);
		s0 = s1;
		(*dp++) = vis_bshuffle(acc0, acc1);
	}

	dl = (mlib_s16 *)dp;

	dl[0] = tab3[s0];
}

/* *********************************************************** */

void
mlib_v_ImageLookUpSI_S32_S16_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table)
{
	mlib_s32 *sl;
	mlib_s16 *dl;
	mlib_s32 j;
	const mlib_s16 *tab0 = (void *)&(((mlib_u8 **)table)[0][HALF_U64]);
	const mlib_s16 *tab1 = (void *)&(((mlib_u8 **)table)[1][HALF_U64]);
	const mlib_s16 *tab2 = (void *)&(((mlib_u8 **)table)[2][HALF_U64]);

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s16 *dp = dl;
		mlib_s32 off, s0, size = xsize;

		if (size > 0) {
			off = (mlib_s32)(((8 - ((mlib_addr)dp & 7)) & 7) >> 1);

			if (off == 0) {
				mlib_v_ImageLookUpSI_S32_S16_4_DstOff0_D1(sp,
				    dp, size, table);
			} else if (off == 1) {
				s0 = *sp;
				(*dp++) = tab0[s0];
				size--;
				mlib_v_ImageLookUpSI_S32_S16_4_DstOff1_D1(sp,
				    dp, size, table);
			} else if (off == 2) {
				s0 = *sp;
				(*dp++) = tab0[s0];
				(*dp++) = tab1[s0];
				size--;
				mlib_v_ImageLookUpSI_S32_S16_4_DstOff2_D1(sp,
				    dp, size, table);
			} else if (off == 3) {
				s0 = *sp;
				(*dp++) = tab0[s0];
				(*dp++) = tab1[s0];
				(*dp++) = tab2[s0];
				size--;
				mlib_v_ImageLookUpSI_S32_S16_4_DstOff3_D1(sp,
				    dp, size, table);
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
