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
#include <mlib_ImageDivTables.h>
#include <mlib_v_ImageColorConvertFunc.h>

/* *********************************************************** */

static void mlib_v_ImageColorSeparate(
    mlib_f32 *dst,
    const mlib_d64 *src,
    mlib_s32 n);

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

#define	CHANNELSEPARATE_U8(sd0, sd1, sd2, rdd, gdd, bdd)         \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	rdd = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));   \
	sde = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));   \
	gdd = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sde));   \
	sdd = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	bdd = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde))

/* *********************************************************** */

void
mlib_v_ImageColorSeparate(
    mlib_f32 *dst,
    const mlib_d64 *src,
    mlib_s32 n)
{
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < n - 1; i++) {
		mlib_d64 sd0, sd1, sd2, sda, sdb, sdc, sdd, sde, d0, d1, d2;

		sd0 = src[0];
		sd1 = src[1];
		sd2 = src[2];
		CHANNELSEPARATE_U8(sd0, sd1, sd2, d0, d1, d2);
		dst[0] = vis_read_hi(d0);
		dst[1] = vis_read_hi(d1);
		dst[2] = vis_read_hi(d2);
		dst[3] = vis_read_lo(d0);
		dst[4] = vis_read_lo(d1);
		dst[5] = vis_read_lo(d2);

		src += 3;
		dst += 6;
	}

	if (i < n) {
		mlib_d64 sd0, sd1, sd2, sda, sdb, sdc, sdd, sde, d0, d1, d2;

		sd0 = src[0];
		sd1 = vis_ld_d64_nf((mlib_d64 *)src + 1);
		sd2 = vis_ld_d64_nf((mlib_d64 *)src + 2);
		CHANNELSEPARATE_U8(sd0, sd1, sd2, d0, d1, d2);
		dst[0] = vis_read_hi(d0);
		dst[1] = vis_read_hi(d1);
		dst[2] = vis_read_hi(d2);
		dst[3] = vis_read_lo(d0);
		dst[4] = vis_read_lo(d1);
		dst[5] = vis_read_lo(d2);
	}
}

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
	mlib_d64 buffColorConvert2[BUFF_SIZE + 4];
	mlib_f32 *pfd;
	mlib_d64 *buff1, *pbuff_arr1 = buffColorConvert1;
	mlib_d64 *buff2, *pbuff_arr2 = buffColorConvert2;
	mlib_f32 *buff1f;
	mlib_u8 *sp, *dp, *sl, *dl, *dend;
	mlib_s32 i, j, size, n, m, emask, dp_aligned;
	mlib_s32 k[9];
	mlib_s32 off[3];
	mlib_d64 scalef, tmp_arr64[2];
	mlib_d64 sum1, sum2, sum3;
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8, offc0, offc1,
	    offc2;
	mlib_d64 p0, p1, p2;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_d64 sat = (mlib_d64)0x7F800000;
#endif /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 k00, k01, k02, k10, k11, k12, k20, k21, k22, c_0, c_1, c_2;
	mlib_d64 dmask1001 = vis_to_double(0xffff0000, 0x0000ffff);

	vis_write_gsr(((scale & 0xF) << 3) + 2);

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
		n = ((mlib_addr)sp & 7) ^ (((mlib_addr)sp & 1) << 2);
		n = (n > xsize) ? xsize : n;

		for (i = 0; i < 3 * n; i += 3) {
			p0 = mlib_U82D64[sp[i]];
			p1 = mlib_U82D64[sp[i + 1]];
			p2 = mlib_U82D64[sp[i + 2]];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_U8(dp[i], sum1);
			SAT_U8(dp[i + 1], sum2);
			SAT_U8(dp[i + 2], sum3);
		}

		sp += 3 * n;
		dp += 3 * n;
		size = xsize - n;

		do {
/* loop on buffer size */

			if (size > 2 * BUFF_SIZE) {
				n = 2 * BUFF_SIZE;
			} else {
				n = size;
			}

			m = (n - 1) >> 3;
			mlib_v_ImageColorSeparate((mlib_f32 *)pbuff_arr1,
			    (mlib_d64 *)sp, m + 1);

			m = (n - 1) >> 2;
			buff1f = (mlib_f32 *)pbuff_arr1;
			buff2 = pbuff_arr2;

#pragma pipeloop(0)
			for (i = 0; i < (m + 1); i++) {
				mlib_d64 s00, s01, s02, s10, s11, s12, s20, s21,
				    s22;
				mlib_f32 x0, x1, x2, df0, df1, df2;

				x0 = buff1f[0];
				x1 = buff1f[1];
				x2 = buff1f[2];

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

				((mlib_f32 *)buff2)[0] = df0;
				((mlib_f32 *)buff2)[1] = df1;
/* ((mlib_f32*)buff2)[2] = vis_fzeros(); */
				((mlib_f32 *)buff2)[3] = df2;

				buff1f += 3;
				buff2 += 2;
			}

/* merge buff values to 3-channel array */
			buff2 = pbuff_arr2;
			dp_aligned = (((mlib_addr)dp & 3) == 0);

			if (dp_aligned) {
				pfd = (mlib_f32 *)dp;
				m = n >> 2;
			} else {
				pfd = (mlib_f32 *)pbuff_arr1;
				m = ((n - 1) >> 2) + 1;
			}

#pragma pipeloop(0)
			for (i = 0; i < m; i++) {
				mlib_d64 d_0235, d_xx14, d_0xx5, d_x14x, d_23xx,
				    d_0145;

				d_0235 = buff2[0];
				d_xx14 = buff2[1];

				d_0235 =
				    vis_fpmerge(vis_read_hi(d_0235),
				    vis_read_lo(d_0235));
				d_23xx = vis_faligndata(d_0235, d_0235);
				d_x14x = vis_faligndata(d_xx14, d_xx14);
				d_0xx5 = vis_fand(dmask1001, d_0235);
				d_0145 = vis_for(d_0xx5, d_x14x);

				pfd[0] = vis_read_hi(d_0145);
				pfd[1] = vis_read_hi(d_23xx);
				pfd[2] = vis_read_lo(d_0145);

				buff2 += 2;
				pfd += 3;
			}

			dend = dp + n + n + n - 1;

/* last pixels in case of aligned destination */

			if (dp_aligned && (mlib_u8 *)pfd <= dend) {
				mlib_d64 d_0235, d_xx14, d_0xx5, d_x14x, d_23xx,
				    d_0145;
				mlib_f32 *tmp_arr32 = (mlib_f32 *)tmp_arr64;

				d_0235 = buff2[0];
				d_xx14 = buff2[1];

				d_0235 =
				    vis_fpmerge(vis_read_hi(d_0235),
				    vis_read_lo(d_0235));
				d_23xx = vis_faligndata(d_0235, d_0235);
				d_x14x = vis_faligndata(d_xx14, d_xx14);
				d_0xx5 = vis_fand(dmask1001, d_0235);
				d_0145 = vis_for(d_0xx5, d_x14x);

				emask = vis_edge8(pfd, dend);

				if ((mlib_addr)pfd & 7) {
					pfd--;
					tmp_arr32++;
				}

				tmp_arr32[0] = vis_read_hi(d_0145);
				tmp_arr32[1] = vis_read_hi(d_23xx);
				tmp_arr32[2] = vis_read_lo(d_0145);

				vis_pst_8(tmp_arr64[0], pfd, emask);

				pfd += 2;
				emask = vis_edge8(pfd, dend);

				if ((mlib_u8 *)pfd <= dend)
					vis_pst_8(tmp_arr64[1], pfd, emask);
			}

/* align destination data */

			if (!dp_aligned) {
				mlib_d64 dd0, dd1, dd, *ddp;
				mlib_s32 offset;

				buff1 = pbuff_arr1;
				offset = (mlib_addr)dp & 7;
				ddp = (mlib_d64 *)(dp - offset);

				if (offset)
					offset = 8 - offset;
				vis_alignaddr((void *)0, offset);
				emask = vis_edge8(dp, dend);
				dd0 = (*buff1++);
				dd = vis_faligndata(dd0, dd0);
				vis_pst_8(dd, ddp, emask);
				ddp++;

#pragma pipeloop(0)
				for (; (mlib_u8 *)ddp <= (dend - 8); ddp++) {
					dd1 = (*buff1++);
					dd = vis_faligndata(dd0, dd1);
					ddp[0] = dd;
					dd0 = dd1;
				}

				if ((mlib_u8 *)ddp <= dend) {
					dd1 = (*buff1++);
					dd = vis_faligndata(dd0, dd1);
					emask = vis_edge8(ddp, dend);
					vis_pst_8(dd, ddp, emask);
				}

				vis_alignaddr((void *)0, 2);
			}

			sp += 3 * n;
			dp += 3 * n;
			size -= n;

		} while (size);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */
