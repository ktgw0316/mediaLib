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

#pragma ident	"@(#)mlib_v_ImageColorConvertFuncU8.c	9.2	07/11/05 SMI"

/*
 *
 * FUNCTIONS
 *     void mlib_v_ImageColorConvert2_U8(const mlib_u8  *src,
 *                                       mlib_s32       slb,
 *                                       mlib_u8        *dst,
 *                                       mlib_s32       dlb,
 *                                       mlib_s32       xsize,
 *                                       mlib_s32       ysize,
 *                                       const mlib_d64 *fmat,
 *                                       const mlib_d64 *offset,
 *                                       mlib_s32       scale);
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageDivTables.h>
#include <mlib_v_ImageColorConvertFunc.h>

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_U8(DST, SRC)	DST = ((mlib_s32)((SRC) - sat) >> 24) ^ 0x80

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT_U8(DST, SRC)                                        \
	{                                                       \
	    mlib_d64 sat_u8_tmp = (SRC);                        \
	                                                        \
	    if (sat_u8_tmp >= MLIB_U8_MAX)                      \
		sat_u8_tmp = MLIB_U8_MAX;                       \
	    if (sat_u8_tmp <= MLIB_U8_MIN)                      \
		sat_u8_tmp = MLIB_U8_MIN;                       \
	    DST = (mlib_s32)sat_u8_tmp;                         \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_RINT_S16(res, x, scalef)                           \
	if (x >= 0)                                             \
	    res = (mlib_s32)(x * scalef + 0.5) & 0xffff;        \
	else                                                    \
	    res = (mlib_s32)(x * scalef - 0.5) & 0xffff

/* *********************************************************** */

void
mlib_v_ImageColorConvert2_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale)
{
	mlib_d64 buffColorConvert1[BUFF_SIZE + 4];
	mlib_d64 *buff1, *pbuff_arr1 = buffColorConvert1;
	mlib_d64 buffColorConvert2[BUFF_SIZE + 4];
	mlib_d64 *buff2, *pbuff_arr2 = buffColorConvert2;
	mlib_u8 *sp, *dp, *sl, *dl;
	mlib_s32 i, j, n, n3, dp_aligned;
	mlib_s32 k[9];
	mlib_s32 off[3];
	mlib_d64 scalef;
	mlib_d64 sum1, sum2, sum3;
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8, offc0, offc1,
	    offc2;
	mlib_d64 p0, p1, p2;
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_d64 sat = (mlib_d64)0x7F800000;
#endif /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 k00, k01, k02, k10, k11, k12, k20, k21, k22, c_0, c_1, c_2;
	mlib_d64 s00, s01, s02, s10, s11, s12, s20, s21, s22;
	mlib_d64 dt0, dt1, dt2, dt3, dt4, dt5;
	mlib_f32 x0, x1, x2, df0, df1, df2;
	mlib_s32 size4, l;
	mlib_f32 *psrc, *fb;
	mlib_d64 d0, d1, d2;

	vis_write_gsr(((scale & 0xF) << 3) + 4);

/* 1-D vectors */
	if ((3 * xsize == slb) && (slb == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}
#pragma pipeloop(0)
	for (i = 1; i < BUFF_SIZE; i += 2) {
		buffColorConvert2[i] = 0;
	}

#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = (mlib_d64)0x1000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	scalef = 1.0;
#endif /* MLIB_USE_FTOI_CLAMPING */

	kk0 = scalef * fmat[0];
	kk1 = scalef * fmat[1];
	kk2 = scalef * fmat[2];
	kk3 = scalef * fmat[3];
	kk4 = scalef * fmat[4];
	kk5 = scalef * fmat[5];
	kk6 = scalef * fmat[6];
	kk7 = scalef * fmat[7];
	kk8 = scalef * fmat[8];

	offc0 = scalef * offset[0];
	offc1 = scalef * offset[1];
	offc2 = scalef * offset[2];

	scalef = (0x10000 >> (scale + 1));
	for (i = 0; i < 9; i++) {
		MLIB_RINT_S16(k[i], fmat[i], scalef);
	}

	scalef = (0x100 >> (scale + 1));
	for (i = 0; i < 3; i++) {
		MLIB_RINT_S16(off[i], offset[i], scalef);
	}

	k00 = vis_to_double_dup((k[0] << 16) | k[3]);
	k01 = vis_to_double_dup((k[1] << 16) | k[4]);
	k02 = vis_to_double_dup((k[2] << 16) | k[5]);
	k10 = vis_to_double_dup((k[3] << 16) | k[6]);
	k11 = vis_to_double_dup((k[4] << 16) | k[7]);
	k12 = vis_to_double_dup((k[5] << 16) | k[8]);
	k20 = vis_to_double_dup((k[6] << 16) | k[0]);
	k21 = vis_to_double_dup((k[7] << 16) | k[1]);
	k22 = vis_to_double_dup((k[8] << 16) | k[2]);
	c_0 = vis_to_double_dup((off[0] << 16) | off[1]);
	c_1 = vis_to_double_dup((off[1] << 16) | off[2]);
	c_2 = vis_to_double_dup((off[2] << 16) | off[0]);

	sp = sl = (void *)src;
	dp = dl = dst;

/* line loop */
	for (j = 0; j < ysize; j++) {

/* first iterations for aligning of source pointer */
		n = ((mlib_addr)sp & 3);
		n = (n > xsize) ? xsize : n;

		n3 = (n << 1) + n;
		for (i = 0; i < n3; i += 3) {
			p0 = mlib_U82D64[sp[0]];
			p1 = mlib_U82D64[sp[1]];
			p2 = mlib_U82D64[sp[2]];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_U8(dp[0], sum1);
			SAT_U8(dp[1], sum2);
			SAT_U8(dp[2], sum3);
			sp += 3;
			dp += 3;
		}

		i = i / 3;

		size4 = ((xsize - n) >> 2) & ~1;

		psrc = (mlib_f32 *)sp;

/* loop on buffer size */
		while (size4 > 0) {

			if (size4 > BUFF_SIZE / 2) {
				n = BUFF_SIZE / 2;
			} else {
				n = size4;
			}

			size4 -= n;
			i += (n << 2);
			fb = (mlib_f32 *)pbuff_arr2;

			for (l = 0; l < n; l++) {
				((mlib_f32 *)&dt0)[0] = ((mlib_f32 *)psrc)[0];
				((mlib_f32 *)&dt0)[1] = ((mlib_f32 *)psrc)[1];
				((mlib_f32 *)&dt1)[0] = ((mlib_f32 *)psrc)[2];
				((mlib_f32 *)&dt1)[1] = vis_fzeros();

				vis_write_bmask(0x0369147a, 0);
				dt2 = vis_bshuffle(dt0, dt1);
				x0 = vis_read_hi(dt2);
				x1 = vis_read_lo(dt2);

				vis_write_bmask(0x258b147a, 0);
				dt3 = vis_bshuffle(dt0, dt1);
				x2 = vis_read_hi(dt3);

				s00 = vis_fmul8x16(x0, k00);
				s10 = vis_fmul8x16(x0, k10);
				s20 = vis_fmul8x16(x0, k20);
				s01 = vis_fmul8x16(x1, k01);
				s11 = vis_fmul8x16(x1, k11);
				s21 = vis_fmul8x16(x1, k21);
				s02 = vis_fmul8x16(x2, k02);
				s12 = vis_fmul8x16(x2, k12);
				s22 = vis_fmul8x16(x2, k22);

				s00 = vis_fpadd16(s00, s01);
				s10 = vis_fpadd16(s10, s11);
				s20 = vis_fpadd16(s20, s21);

				s02 = vis_fpadd16(s02, c_0);
				s12 = vis_fpadd16(s12, c_1);
				s22 = vis_fpadd16(s22, c_2);

				s00 = vis_fpadd16(s00, s02);
				s10 = vis_fpadd16(s10, s12);
				s20 = vis_fpadd16(s20, s22);

				df0 = vis_fpack16(s00);
				df1 = vis_fpack16(s10);
				df2 = vis_fpack16(s20);

				((mlib_f32 *)fb)[0] = df0;
				((mlib_f32 *)fb)[1] = df1;
				((mlib_f32 *)fb)[2] = df2;

				psrc += 3;
				fb += 3;
			}

			dp_aligned = (((mlib_addr)dp & 7) == 0);
			buff2 = pbuff_arr2;

			if (dp_aligned)
				buff1 = (mlib_d64 *)dp;
			else
				buff1 = (mlib_d64 *)pbuff_arr1;

#pragma pipeloop(0)
			for (l = 0; l < n >> 1; l++) {
				dt0 = buff2[0];
				dt1 = buff2[1];
				dt2 = buff2[2];

				vis_write_bmask(0x04891526, 0);
				dt3 = vis_bshuffle(dt0, dt1);

				d0 = vis_fpmerge(vis_read_hi(dt0),
				    vis_read_lo(dt0));
				d1 = vis_fpmerge(vis_read_hi(dt2),
				    vis_read_lo(dt2));
				d2 = vis_fpmerge(vis_read_lo(d0),
				    vis_read_hi(d1));

				vis_write_bmask(0xab46c137, 0);
				dt4 = vis_bshuffle(d2, dt1);

				vis_write_bmask(0x596aef7b, 0);
				dt5 = vis_bshuffle(dt1, dt2);

				buff1[0] = dt3;
				buff1[1] = dt4;
				buff1[2] = dt5;

				buff2 += 3;
				buff1 += 3;
			}

			if (!dp_aligned) {
				buff1 = pbuff_arr1;
				mlib_ImageCopy_na((mlib_u8 *)buff1,
				    (mlib_u8 *)dp, (n << 3) + (n << 2));
			}

			dp += (n << 3) + (n << 2);
		}

		i = (i << 1) + i;
		sp = (mlib_u8 *)psrc;
		for (; i < (xsize << 1) + xsize; i += 3) {
			p0 = mlib_U82D64[sp[0]];
			p1 = mlib_U82D64[sp[1]];
			p2 = mlib_U82D64[sp[2]];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_U8(dp[0], sum1);
			SAT_U8(dp[1], sum2);
			SAT_U8(dp[2], sum3);
			sp += 3;
			dp += 3;
		}

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */
