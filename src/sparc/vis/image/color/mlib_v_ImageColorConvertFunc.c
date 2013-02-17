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

#pragma ident	"@(#)mlib_v_ImageColorConvertFunc.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     void mlib_v_ImageColorConvert1_S16_D1(const mlib_u8 *src,
 *                                           mlib_u8 *dst,
 *                                           mlib_s32 dsize,
 *                                           const mlib_d64 *fmat,
 *                                           mlib_s32 scale);
 *
 *     void mlib_v_ImageColorConvert1_S16(const mlib_s16 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s16 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 dsize,
 *                                        mlib_s32 ysize,
 *                                        const mlib_d64 *fmat,
 *                                        mlib_s32 scale);
 *
 *     void mlib_v_ImageColorConvert2_S16_D1(const mlib_s16 *src,
 *                                           mlib_s16 *dst,
 *                                           mlib_s32 dsize,
 *                                           const mlib_d64 *fmat,
 *                                           const mlib_d64 *offset,
 *                                           mlib_s32 scale);
 *
 *     void mlib_v_ImageColorConvert1_U16(const mlib_s16 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s16 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 dsize,
 *                                        mlib_s32 ysize,
 *                                        const mlib_d64 *fmat,
 *                                        mlib_s32 scale);
 *
 *     void mlib_v_ImageColorConvert2_U16(const mlib_u8 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_u8 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 dsize,
 *                                        mlib_s32 ysize,
 *                                        const mlib_d64 *fmat,
 *                                        const mlib_d64 *offsets,
 *                                        mlib_s32 scale);
 *
 *     void mlib_v_ImageColorConvert2_S16(const mlib_u8 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_u8 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 dsize,
 *                                        mlib_s32 ysize,
 *                                        const mlib_d64 *fmat,
 *                                        const mlib_d64 *offset,
 *                                        mlib_s32 scale);
 *
 *     void mlib_v_ImageColorConvert1_S32(const mlib_s32 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s32 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        const mlib_d64 *fmat);
 *
 *     void mlib_v_ImageColorConvert2_S32(const mlib_s32 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s32 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        const mlib_d64 *fmat,
 *                                        const mlib_d64 *offset);
 *
 *
 *
 * DESCRIPTION
 *         Internal functions.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageColorConvertFunc.h>

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_S16(DST, SRC)	(*DST++) = (((mlib_s32)(SRC)) >> 16)
#define	SAT_U16(DST, SRC)                                            \
		(*DST++) = (((mlib_s32) (SRC - sat)) >> 16) ^ 0x8000

#define	FLOAT2INT_CLAMP(DST, SRC)	(*DST++) = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT_S16(DST, SRC)                                       \
	{                                                       \
	    mlib_d64 sat_s16_tmp = (SRC);                       \
	                                                        \
	    if (sat_s16_tmp >= MLIB_S16_MAX)                    \
		sat_s16_tmp = MLIB_S16_MAX;                     \
	    if (sat_s16_tmp <= MLIB_S16_MIN)                    \
		sat_s16_tmp = MLIB_S16_MIN;                     \
	    (*DST++) = (mlib_s32)sat_s16_tmp;                   \
	}

#define	SAT_U16(DST, SRC)                                       \
	{                                                       \
	    mlib_d64 sat_u16_tmp = (SRC);                       \
	                                                        \
	    if (sat_u16_tmp >= MLIB_U16_MAX)                    \
		sat_u16_tmp = MLIB_U16_MAX;                     \
	    if (sat_u16_tmp <= MLIB_U16_MIN)                    \
		sat_u16_tmp = MLIB_U16_MIN;                     \
	    (*DST++) = (mlib_s32)sat_u16_tmp;                   \
	}

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    (*DST++) = (mlib_s32)dsrc;                          \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd)        \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	rdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc));   \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdc = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdb));   \
	gdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc));   \
	sda = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	bdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNEL_XOR_U16()                                       \
	d3 = vis_fxor(d3, offset);                              \
	d4 = vis_fxor(d4, offset);                              \
	d5 = vis_fxor(d5, offset)

/* *********************************************************** */

#define	CHANNELSEPARATE_U16(sd0, sd1, sd2, rdd, gdd, bdd)        \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	rdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc));   \
	rdd = vis_fxor(rdd, offset);                             \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdc = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdb));   \
	gdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc));   \
	gdd = vis_fxor(gdd, offset);                             \
	sda = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	bdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc));   \
	bdd = vis_fxor(bdd, offset)

/* *********************************************************** */

#define	STORE_PIXEL_S16(ind1, ind2, ind3)                       \
	vis_st_u16_i(d2, dp, ind3);                             \
	d2 = vis_faligndata(d2, d2);                            \
	vis_st_u16_i(d1, dp, ind2);                             \
	d1 = vis_faligndata(d1, d1);                            \
	vis_st_u16_i(d0, dp, ind1);                             \
	d0 = vis_faligndata(d0, d0)

/* *********************************************************** */

#define	CHANNELMUL_S16(ind, dd1, k0, k1, k2)                    \
	dd##ind##0 =                                            \
	vis_fpadd32(vis_fmuld8sux16(vis_read_hi(dd1), k0),      \
	    vis_fmuld8ulx16(vis_read_hi(dd1), k0));             \
	dd##ind##1 =                                            \
	vis_fpadd32(vis_fmuld8sux16(vis_read_lo(dd1), k0),      \
	    vis_fmuld8ulx16(vis_read_lo(dd1), k0));             \
	dd##ind##2 =                                            \
	vis_fpadd32(vis_fmuld8sux16(vis_read_hi(dd1), k1),      \
	    vis_fmuld8ulx16(vis_read_hi(dd1), k1));             \
	dd##ind##3 =                                            \
	vis_fpadd32(vis_fmuld8sux16(vis_read_lo(dd1), k1),      \
	    vis_fmuld8ulx16(vis_read_lo(dd1), k1));             \
	dd##ind##4 =                                            \
	vis_fpadd32(vis_fmuld8sux16(vis_read_hi(dd1), k2),      \
	    vis_fmuld8ulx16(vis_read_hi(dd1), k2));             \
	dd##ind##5 =                                            \
	vis_fpadd32(vis_fmuld8sux16(vis_read_lo(dd1), k2),      \
	    vis_fmuld8ulx16(vis_read_lo(dd1), k2))

/* *********************************************************** */

#define	CHANNELADD_S16(ind)                                     \
	dd00 = vis_fpadd32(dd00, dd##ind##0);                   \
	dd01 = vis_fpadd32(dd01, dd##ind##1);                   \
	dd02 = vis_fpadd32(dd02, dd##ind##2);                   \
	dd03 = vis_fpadd32(dd03, dd##ind##3);                   \
	dd04 = vis_fpadd32(dd04, dd##ind##4);                   \
	dd05 = vis_fpadd32(dd05, dd##ind##5)

/* *********************************************************** */

#define	CHANNELADD_OFFSET_S16()                                 \
	dd00 = vis_fpadd32(dd00, doffset0);                     \
	dd01 = vis_fpadd32(dd01, doffset0);                     \
	dd02 = vis_fpadd32(dd02, doffset1);                     \
	dd03 = vis_fpadd32(dd03, doffset1);                     \
	dd04 = vis_fpadd32(dd04, doffset2);                     \
	dd05 = vis_fpadd32(dd05, doffset2)

/* *********************************************************** */

void
mlib_v_ImageColorConvert1_S16_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_d64 *fmat,
    mlib_s32 scale)
{
/* pointer to src address */
	mlib_s16 *sp;

/* pointer to dst address */
	mlib_s16 *dp;

/* destination data */
	mlib_d64 d0, d1, d2, d3, d4, d5;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* source data after merge */
	mlib_d64 rdd, gdd, bdd;

/* coeff's for VIS version */
	mlib_f32 k0, k1, k2, k3, k4, k5, k6, k7, k8;

/* pointer to src address */
	mlib_d64 *psrc;

/* temporaries used in macro */
	mlib_d64 sda, sdb, sdc;

/* temporaries used in macro */
	mlib_d64 dd00, dd01, dd02, dd03, dd04, dd05;

/* temporaries used in macro */
	mlib_d64 dd10, dd11, dd12, dd13, dd14, dd15;

/* temporaries used in macro */
	mlib_d64 dd20, dd21, dd22, dd23, dd24, dd25;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's for VIS version */
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8;

/* mlib_d64 source data */
	mlib_d64 p0, p1, p2;

/* loop variables */
	mlib_s32 i, off;
	mlib_s32 smat0, smat1, smat2, smat3, smat4;
	mlib_s32 smat5, smat6, smat7, smat8;
	mlib_s32 scalef;
	mlib_s32 shift_dst;

	scalef = (0x10000 >> scale);

	if (fmat[0] >= 0)
		smat0 = (mlib_s32)(fmat[0] * scalef + 0.5) & 0xFFFF;
	else
		smat0 = (mlib_s32)(fmat[0] * scalef - 0.5) & 0xFFFF;

	if (fmat[1] >= 0)
		smat1 = (mlib_s32)(fmat[1] * scalef + 0.5) & 0xFFFF;
	else
		smat1 = (mlib_s32)(fmat[1] * scalef - 0.5) & 0xFFFF;

	if (fmat[2] >= 0)
		smat2 = (mlib_s32)(fmat[2] * scalef + 0.5) & 0xFFFF;
	else
		smat2 = (mlib_s32)(fmat[2] * scalef - 0.5) & 0xFFFF;

	if (fmat[3] >= 0)
		smat3 = (mlib_s32)(fmat[3] * scalef + 0.5) & 0xFFFF;
	else
		smat3 = (mlib_s32)(fmat[3] * scalef - 0.5) & 0xFFFF;

	if (fmat[4] >= 0)
		smat4 = (mlib_s32)(fmat[4] * scalef + 0.5) & 0xFFFF;
	else
		smat4 = (mlib_s32)(fmat[4] * scalef - 0.5) & 0xFFFF;

	if (fmat[5] >= 0)
		smat5 = (mlib_s32)(fmat[5] * scalef + 0.5) & 0xFFFF;
	else
		smat5 = (mlib_s32)(fmat[5] * scalef - 0.5) & 0xFFFF;

	if (fmat[6] >= 0)
		smat6 = (mlib_s32)(fmat[6] * scalef + 0.5) & 0xFFFF;
	else
		smat6 = (mlib_s32)(fmat[6] * scalef - 0.5) & 0xFFFF;

	if (fmat[7] >= 0)
		smat7 = (mlib_s32)(fmat[7] * scalef + 0.5) & 0xFFFF;
	else
		smat7 = (mlib_s32)(fmat[7] * scalef - 0.5) & 0xFFFF;

	if (fmat[8] >= 0)
		smat8 = (mlib_s32)(fmat[8] * scalef + 0.5) & 0xFFFF;
	else
		smat8 = (mlib_s32)(fmat[8] * scalef - 0.5) & 0xFFFF;

/* prepare the weight */
	k0 = vis_to_float((smat0 << 16) + smat0);
	k1 = vis_to_float((smat1 << 16) + smat1);
	k2 = vis_to_float((smat2 << 16) + smat2);
	k3 = vis_to_float((smat3 << 16) + smat3);
	k4 = vis_to_float((smat4 << 16) + smat4);
	k5 = vis_to_float((smat5 << 16) + smat5);
	k6 = vis_to_float((smat6 << 16) + smat6);
	k7 = vis_to_float((smat7 << 16) + smat7);
	k8 = vis_to_float((smat8 << 16) + smat8);

#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = 65536.0;
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

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;

	off = ((mlib_addr)sp & 7) >> 1;
	off = (off > dsize) ? dsize : off;

	for (i = 0; i < 3 * off; i += 3) {
		p0 = sp[i];
		p1 = sp[i + 1];
		p2 = sp[i + 2];
		sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2;
		sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5;
		sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8;

		SAT_S16(dp, sum1);
		SAT_S16(dp, sum2);
		SAT_S16(dp, sum3);
	}

	i = i / 3;

	if ((dsize - off - 4) > 0) {

		psrc = (mlib_d64 *)(sp + 3 * off);
		sd0 = psrc[0];
		sd1 = psrc[1];
		sd2 = psrc[2];

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);

		shift_dst = 0;

#pragma pipeloop(0)
		for (; i < (dsize - 8); i += 4) {
			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);

			CHANNELMUL_S16(0, rdd, k0, k3, k6);
			CHANNELMUL_S16(1, gdd, k1, k4, k7);
			CHANNELADD_S16(1);
			CHANNELMUL_S16(2, bdd, k2, k5, k8);
			CHANNELADD_S16(2);

			d3 = vis_fpackfix_pair(dd00, dd01);
			d4 = vis_fpackfix_pair(dd02, dd03);
			d5 = vis_fpackfix_pair(dd04, dd05);

			dp += shift_dst;
			shift_dst = 12;
			psrc += 3;
			d0 = d3;
			d1 = d4;
			d2 = d5;

			sd0 = psrc[0];
			sd1 = psrc[1];
			sd2 = psrc[2];

			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		}

		STORE_PIXEL_S16(18, 20, 22);
		STORE_PIXEL_S16(12, 14, 16);
		STORE_PIXEL_S16(6, 8, 10);

		vis_st_u16_i(d2, dp, 4);
		vis_st_u16_i(d1, dp, 2);
		vis_st_u16_i(d0, dp, 0);

		CHANNELMUL_S16(0, rdd, k0, k3, k6);
		CHANNELMUL_S16(1, gdd, k1, k4, k7);
		CHANNELADD_S16(1);
		CHANNELMUL_S16(2, bdd, k2, k5, k8);
		CHANNELADD_S16(2);

		d3 = vis_fpackfix_pair(dd00, dd01);
		d4 = vis_fpackfix_pair(dd02, dd03);
		d5 = vis_fpackfix_pair(dd04, dd05);

		dp += shift_dst;
		shift_dst = 12;
		psrc += 3;
		d0 = d3;
		d1 = d4;
		d2 = d5;

		STORE_PIXEL_S16(18, 20, 22);
		STORE_PIXEL_S16(12, 14, 16);
		STORE_PIXEL_S16(6, 8, 10);

		vis_st_u16_i(d2, dp, 4);
		vis_st_u16_i(d1, dp, 2);
		vis_st_u16_i(d0, dp, 0);
		dp += shift_dst;
		i += 4;
	}

	i = i * 3;

	for (; i < dsize * 3; i += 3) {
		p0 = sp[i];
		p1 = sp[i + 1];
		p2 = sp[i + 2];
		sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2;
		sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5;
		sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8;

		SAT_S16(dp, sum1);
		SAT_S16(dp, sum2);
		SAT_S16(dp, sum3);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert1_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    mlib_s32 scale)
{
/* pointer to src address */
	mlib_s16 *sp;

/* pointer to dst address */
	mlib_s16 *dp, *dl;

/* destination data */
	mlib_d64 d0, d1, d2, d3, d4, d5;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* source data after merge */
	mlib_d64 rdd, gdd, bdd;

/* coeff's for VIS version */
	mlib_f32 k0, k1, k2, k3, k4, k5, k6, k7, k8;

/* pointer to src address */
	mlib_d64 *psrc;

/* temporaries used in macro */
	mlib_d64 sda, sdb, sdc;

/* temporaries used in macro */
	mlib_d64 dd00, dd01, dd02, dd03, dd04, dd05;

/* temporaries used in macro */
	mlib_d64 dd10, dd11, dd12, dd13, dd14, dd15;

/* temporaries used in macro */
	mlib_d64 dd20, dd21, dd22, dd23, dd24, dd25;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's for VIS version */
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8;

/* mlib_d64 source data */
	mlib_d64 p0, p1, p2;

/* loop variables */
	mlib_s32 i, j, off;
	mlib_s32 smat0, smat1, smat2, smat3, smat4;
	mlib_s32 smat5, smat6, smat7, smat8;
	mlib_s32 scalef;
	mlib_s32 shift_dst;

	scalef = (0x10000 >> scale);

	if (fmat[0] >= 0)
		smat0 = (mlib_s32)(fmat[0] * scalef + 0.5) & 0xFFFF;
	else
		smat0 = (mlib_s32)(fmat[0] * scalef - 0.5) & 0xFFFF;

	if (fmat[1] >= 0)
		smat1 = (mlib_s32)(fmat[1] * scalef + 0.5) & 0xFFFF;
	else
		smat1 = (mlib_s32)(fmat[1] * scalef - 0.5) & 0xFFFF;

	if (fmat[2] >= 0)
		smat2 = (mlib_s32)(fmat[2] * scalef + 0.5) & 0xFFFF;
	else
		smat2 = (mlib_s32)(fmat[2] * scalef - 0.5) & 0xFFFF;

	if (fmat[3] >= 0)
		smat3 = (mlib_s32)(fmat[3] * scalef + 0.5) & 0xFFFF;
	else
		smat3 = (mlib_s32)(fmat[3] * scalef - 0.5) & 0xFFFF;

	if (fmat[4] >= 0)
		smat4 = (mlib_s32)(fmat[4] * scalef + 0.5) & 0xFFFF;
	else
		smat4 = (mlib_s32)(fmat[4] * scalef - 0.5) & 0xFFFF;

	if (fmat[5] >= 0)
		smat5 = (mlib_s32)(fmat[5] * scalef + 0.5) & 0xFFFF;
	else
		smat5 = (mlib_s32)(fmat[5] * scalef - 0.5) & 0xFFFF;

	if (fmat[6] >= 0)
		smat6 = (mlib_s32)(fmat[6] * scalef + 0.5) & 0xFFFF;
	else
		smat6 = (mlib_s32)(fmat[6] * scalef - 0.5) & 0xFFFF;

	if (fmat[7] >= 0)
		smat7 = (mlib_s32)(fmat[7] * scalef + 0.5) & 0xFFFF;
	else
		smat7 = (mlib_s32)(fmat[7] * scalef - 0.5) & 0xFFFF;

	if (fmat[8] >= 0)
		smat8 = (mlib_s32)(fmat[8] * scalef + 0.5) & 0xFFFF;
	else
		smat8 = (mlib_s32)(fmat[8] * scalef - 0.5) & 0xFFFF;

/* prepare the weight */
	k0 = vis_to_float((smat0 << 16) + smat0);
	k1 = vis_to_float((smat1 << 16) + smat1);
	k2 = vis_to_float((smat2 << 16) + smat2);
	k3 = vis_to_float((smat3 << 16) + smat3);
	k4 = vis_to_float((smat4 << 16) + smat4);
	k5 = vis_to_float((smat5 << 16) + smat5);
	k6 = vis_to_float((smat6 << 16) + smat6);
	k7 = vis_to_float((smat7 << 16) + smat7);
	k8 = vis_to_float((smat8 << 16) + smat8);

#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = 65536.0;
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

	sp = (mlib_s16 *)src;
	dp = dl = (mlib_s16 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		off = ((mlib_addr)sp & 7) >> 1;
		off = (off > dsize) ? dsize : off;

		for (i = 0; i < 3 * off; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8;

			SAT_S16(dp, sum1);
			SAT_S16(dp, sum2);
			SAT_S16(dp, sum3);
		}

		i = i / 3;

		if ((dsize - off - 4) > 0) {

			psrc = (mlib_d64 *)(sp + 3 * off);
			sd0 = psrc[0];
			sd1 = psrc[1];
			sd2 = psrc[2];

			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);

			shift_dst = 0;

#pragma pipeloop(0)
			for (; i < (dsize - 8); i += 4) {
				STORE_PIXEL_S16(18, 20, 22);
				STORE_PIXEL_S16(12, 14, 16);
				STORE_PIXEL_S16(6, 8, 10);

				vis_st_u16_i(d2, dp, 4);
				vis_st_u16_i(d1, dp, 2);
				vis_st_u16_i(d0, dp, 0);

				CHANNELMUL_S16(0, rdd, k0, k3, k6);
				CHANNELMUL_S16(1, gdd, k1, k4, k7);
				CHANNELADD_S16(1);
				CHANNELMUL_S16(2, bdd, k2, k5, k8);
				CHANNELADD_S16(2);

				d3 = vis_fpackfix_pair(dd00, dd01);
				d4 = vis_fpackfix_pair(dd02, dd03);
				d5 = vis_fpackfix_pair(dd04, dd05);

				dp += shift_dst;
				shift_dst = 12;
				psrc += 3;
				d0 = d3;
				d1 = d4;
				d2 = d5;

				sd0 = psrc[0];
				sd1 = psrc[1];
				sd2 = psrc[2];

				CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd,
				    bdd);
			}

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);

			CHANNELMUL_S16(0, rdd, k0, k3, k6);
			CHANNELMUL_S16(1, gdd, k1, k4, k7);
			CHANNELADD_S16(1);
			CHANNELMUL_S16(2, bdd, k2, k5, k8);
			CHANNELADD_S16(2);

			d3 = vis_fpackfix_pair(dd00, dd01);
			d4 = vis_fpackfix_pair(dd02, dd03);
			d5 = vis_fpackfix_pair(dd04, dd05);

			dp += shift_dst;
			shift_dst = 12;
			psrc += 3;
			d0 = d3;
			d1 = d4;
			d2 = d5;

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);
			dp += shift_dst;
			i += 4;
		}

		i = i * 3;

		for (; i < dsize * 3; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8;

			SAT_S16(dp, sum1);
			SAT_S16(dp, sum2);
			SAT_S16(dp, sum3);
		}

		sp = sp + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert2_S16_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale)
{
/* pointer to src address */
	mlib_s16 *sp;

/* pointer to dst address */
	mlib_s16 *dp;

/* destination data */
	mlib_d64 d0, d1, d2, d3, d4, d5;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* source data after merge */
	mlib_d64 rdd, gdd, bdd;

/* coeff's for VIS version */
	mlib_f32 k0, k1, k2, k3, k4, k5, k6, k7, k8;

/* offsets for VIS version */
	mlib_d64 doffset0, doffset1, doffset2;

/* pointer to src address */
	mlib_d64 *psrc;

/* temporaries used in macro */
	mlib_d64 sda, sdb, sdc;

/* temporaries used in macro */
	mlib_d64 dd00, dd01, dd02, dd03, dd04, dd05;

/* temporaries used in macro */
	mlib_d64 dd10, dd11, dd12, dd13, dd14, dd15;

/* temporaries used in macro */
	mlib_d64 dd20, dd21, dd22, dd23, dd24, dd25;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's for VIS version */
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8;

/* offsets for VIS version */
	mlib_d64 offc0, offc1, offc2;

/* mlib_d64 source data */
	mlib_d64 p0, p1, p2;

/* loop variables */
	mlib_s32 i, off;
	mlib_s32 smat0, smat1, smat2, smat3, smat4;
	mlib_s32 smat5, smat6, smat7, smat8;
	mlib_s32 scalef;
	mlib_s32 shift_dst;

	scalef = (0x10000 >> scale);

	if (fmat[0] >= 0)
		smat0 = (mlib_s32)(fmat[0] * scalef + 0.5) & 0xFFFF;
	else
		smat0 = (mlib_s32)(fmat[0] * scalef - 0.5) & 0xFFFF;

	if (fmat[1] >= 0)
		smat1 = (mlib_s32)(fmat[1] * scalef + 0.5) & 0xFFFF;
	else
		smat1 = (mlib_s32)(fmat[1] * scalef - 0.5) & 0xFFFF;

	if (fmat[2] >= 0)
		smat2 = (mlib_s32)(fmat[2] * scalef + 0.5) & 0xFFFF;
	else
		smat2 = (mlib_s32)(fmat[2] * scalef - 0.5) & 0xFFFF;

	if (fmat[3] >= 0)
		smat3 = (mlib_s32)(fmat[3] * scalef + 0.5) & 0xFFFF;
	else
		smat3 = (mlib_s32)(fmat[3] * scalef - 0.5) & 0xFFFF;

	if (fmat[4] >= 0)
		smat4 = (mlib_s32)(fmat[4] * scalef + 0.5) & 0xFFFF;
	else
		smat4 = (mlib_s32)(fmat[4] * scalef - 0.5) & 0xFFFF;

	if (fmat[5] >= 0)
		smat5 = (mlib_s32)(fmat[5] * scalef + 0.5) & 0xFFFF;
	else
		smat5 = (mlib_s32)(fmat[5] * scalef - 0.5) & 0xFFFF;

	if (fmat[6] >= 0)
		smat6 = (mlib_s32)(fmat[6] * scalef + 0.5) & 0xFFFF;
	else
		smat6 = (mlib_s32)(fmat[6] * scalef - 0.5) & 0xFFFF;

	if (fmat[7] >= 0)
		smat7 = (mlib_s32)(fmat[7] * scalef + 0.5) & 0xFFFF;
	else
		smat7 = (mlib_s32)(fmat[7] * scalef - 0.5) & 0xFFFF;

	if (fmat[8] >= 0)
		smat8 = (mlib_s32)(fmat[8] * scalef + 0.5) & 0xFFFF;
	else
		smat8 = (mlib_s32)(fmat[8] * scalef - 0.5) & 0xFFFF;

/* prepare the weight */
	k0 = vis_to_float((smat0 << 16) + smat0);
	k1 = vis_to_float((smat1 << 16) + smat1);
	k2 = vis_to_float((smat2 << 16) + smat2);
	k3 = vis_to_float((smat3 << 16) + smat3);
	k4 = vis_to_float((smat4 << 16) + smat4);
	k5 = vis_to_float((smat5 << 16) + smat5);
	k6 = vis_to_float((smat6 << 16) + smat6);
	k7 = vis_to_float((smat7 << 16) + smat7);
	k8 = vis_to_float((smat8 << 16) + smat8);

	if (offset[0] >= 0)
		smat0 = (mlib_s32)(offset[0] * scalef + 0.5);
	else
		smat0 = (mlib_s32)(offset[0] * scalef - 0.5);

	if (offset[1] >= 0)
		smat1 = (mlib_s32)(offset[1] * scalef + 0.5);
	else
		smat1 = (mlib_s32)(offset[1] * scalef - 0.5);

	if (offset[2] >= 0)
		smat2 = (mlib_s32)(offset[2] * scalef + 0.5);
	else
		smat2 = (mlib_s32)(offset[2] * scalef - 0.5);

	doffset0 = vis_to_double_dup(smat0);
	doffset1 = vis_to_double_dup(smat1);
	doffset2 = vis_to_double_dup(smat2);

#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = 65536.0;
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

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;

	off = ((mlib_addr)sp & 7) >> 1;
	off = (off > dsize) ? dsize : off;

	for (i = 0; i < 3 * off; i += 3) {
		p0 = sp[i];
		p1 = sp[i + 1];
		p2 = sp[i + 2];
		sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
		sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
		sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

		SAT_S16(dp, sum1);
		SAT_S16(dp, sum2);
		SAT_S16(dp, sum3);
	}

	i = i / 3;

	if ((dsize - off - 4) > 0) {

		psrc = (mlib_d64 *)(sp + 3 * off);
		sd0 = psrc[0];
		sd1 = psrc[1];
		sd2 = psrc[2];

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);

		shift_dst = 0;

#pragma pipeloop(0)
		for (; i < (dsize - 8); i += 4) {
			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);

			CHANNELMUL_S16(0, rdd, k0, k3, k6);
			CHANNELMUL_S16(1, gdd, k1, k4, k7);
			CHANNELADD_S16(1);
			CHANNELMUL_S16(2, bdd, k2, k5, k8);
			CHANNELADD_S16(2);
			CHANNELADD_OFFSET_S16();

			d3 = vis_fpackfix_pair(dd00, dd01);
			d4 = vis_fpackfix_pair(dd02, dd03);
			d5 = vis_fpackfix_pair(dd04, dd05);

			dp += shift_dst;
			shift_dst = 12;
			psrc += 3;
			d0 = d3;
			d1 = d4;
			d2 = d5;

			sd0 = psrc[0];
			sd1 = psrc[1];
			sd2 = psrc[2];

			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		}

		STORE_PIXEL_S16(18, 20, 22);
		STORE_PIXEL_S16(12, 14, 16);
		STORE_PIXEL_S16(6, 8, 10);

		vis_st_u16_i(d2, dp, 4);
		vis_st_u16_i(d1, dp, 2);
		vis_st_u16_i(d0, dp, 0);

		CHANNELMUL_S16(0, rdd, k0, k3, k6);
		CHANNELMUL_S16(1, gdd, k1, k4, k7);
		CHANNELADD_S16(1);
		CHANNELMUL_S16(2, bdd, k2, k5, k8);
		CHANNELADD_S16(2);
		CHANNELADD_OFFSET_S16();

		d3 = vis_fpackfix_pair(dd00, dd01);
		d4 = vis_fpackfix_pair(dd02, dd03);
		d5 = vis_fpackfix_pair(dd04, dd05);

		dp += shift_dst;
		shift_dst = 12;
		psrc += 3;
		d0 = d3;
		d1 = d4;
		d2 = d5;

		STORE_PIXEL_S16(18, 20, 22);
		STORE_PIXEL_S16(12, 14, 16);
		STORE_PIXEL_S16(6, 8, 10);

		vis_st_u16_i(d2, dp, 4);
		vis_st_u16_i(d1, dp, 2);
		vis_st_u16_i(d0, dp, 0);
		dp += shift_dst;
		i += 4;
	}

	i = i * 3;

	for (; i < dsize * 3; i += 3) {
		p0 = sp[i];
		p1 = sp[i + 1];
		p2 = sp[i + 2];
		sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
		sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
		sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

		SAT_S16(dp, sum1);
		SAT_S16(dp, sum2);
		SAT_S16(dp, sum3);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert1_U16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    mlib_s32 scale)
{
/* pointer to src address */
	mlib_u16 *sp;

/* pointer to dst address */
	mlib_u16 *dp, *dl;

/* destination data */
	mlib_d64 d0, d1, d2, d3, d4, d5;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* offsets for mlib_u32 version */
	mlib_d64 doffset0, doffset1, doffset2;

/* source data after merge */
	mlib_d64 rdd, gdd, bdd;

/* coeff's for VIS version */
	mlib_f32 k0, k1, k2, k3, k4, k5, k6, k7, k8;

/* pointer to src address */
	mlib_d64 *psrc;

/* temporaries used in macro */
	mlib_d64 sda, sdb, sdc;

/* temporaries used in macro */
	mlib_d64 dd00, dd01, dd02, dd03, dd04, dd05;

/* temporaries used in macro */
	mlib_d64 dd10, dd11, dd12, dd13, dd14, dd15;

/* temporaries used in macro */
	mlib_d64 dd20, dd21, dd22, dd23, dd24, dd25;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's for VIS version */
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8;

/* mlib_d64 source data */
	mlib_d64 p0, p1, p2;

/* loop variables */
	mlib_s32 i, j, off;
	mlib_s32 smat0, smat1, smat2, smat3, smat4;
	mlib_s32 smat5, smat6, smat7, smat8;
	mlib_s32 scalef;
	mlib_s32 shift_dst;
	mlib_d64 offset = vis_to_double_dup(0x80008000);

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_d64 sat = (mlib_d64)0x80000000;
#endif /* MLIB_USE_FTOI_CLAMPING */

	scalef = (0x10000 >> scale);

	if (fmat[0] >= 0)
		smat0 = (mlib_s32)(fmat[0] * scalef + 0.5) & 0xFFFF;
	else
		smat0 = (mlib_s32)(fmat[0] * scalef - 0.5) & 0xFFFF;

	if (fmat[1] >= 0)
		smat1 = (mlib_s32)(fmat[1] * scalef + 0.5) & 0xFFFF;
	else
		smat1 = (mlib_s32)(fmat[1] * scalef - 0.5) & 0xFFFF;

	if (fmat[2] >= 0)
		smat2 = (mlib_s32)(fmat[2] * scalef + 0.5) & 0xFFFF;
	else
		smat2 = (mlib_s32)(fmat[2] * scalef - 0.5) & 0xFFFF;

	if (fmat[3] >= 0)
		smat3 = (mlib_s32)(fmat[3] * scalef + 0.5) & 0xFFFF;
	else
		smat3 = (mlib_s32)(fmat[3] * scalef - 0.5) & 0xFFFF;

	if (fmat[4] >= 0)
		smat4 = (mlib_s32)(fmat[4] * scalef + 0.5) & 0xFFFF;
	else
		smat4 = (mlib_s32)(fmat[4] * scalef - 0.5) & 0xFFFF;

	if (fmat[5] >= 0)
		smat5 = (mlib_s32)(fmat[5] * scalef + 0.5) & 0xFFFF;
	else
		smat5 = (mlib_s32)(fmat[5] * scalef - 0.5) & 0xFFFF;

	if (fmat[6] >= 0)
		smat6 = (mlib_s32)(fmat[6] * scalef + 0.5) & 0xFFFF;
	else
		smat6 = (mlib_s32)(fmat[6] * scalef - 0.5) & 0xFFFF;

	if (fmat[7] >= 0)
		smat7 = (mlib_s32)(fmat[7] * scalef + 0.5) & 0xFFFF;
	else
		smat7 = (mlib_s32)(fmat[7] * scalef - 0.5) & 0xFFFF;

	if (fmat[8] >= 0)
		smat8 = (mlib_s32)(fmat[8] * scalef + 0.5) & 0xFFFF;
	else
		smat8 = (mlib_s32)(fmat[8] * scalef - 0.5) & 0xFFFF;

/* prepare the weight */
	k0 = vis_to_float((smat0 << 16) + smat0);
	k1 = vis_to_float((smat1 << 16) + smat1);
	k2 = vis_to_float((smat2 << 16) + smat2);
	k3 = vis_to_float((smat3 << 16) + smat3);
	k4 = vis_to_float((smat4 << 16) + smat4);
	k5 = vis_to_float((smat5 << 16) + smat5);
	k6 = vis_to_float((smat6 << 16) + smat6);
	k7 = vis_to_float((smat7 << 16) + smat7);
	k8 = vis_to_float((smat8 << 16) + smat8);

/* prepare offsets */

	d0 = (fmat[0] + fmat[1] + fmat[2] - 1) * 32768.0;

	if (d0 >= 0)
		smat0 = (mlib_s32)(d0 * scalef + 0.5);
	else
		smat0 = (mlib_s32)(d0 * scalef - 0.5);

	d0 = (fmat[3] + fmat[4] + fmat[5] - 1) * 32768.0;

	if (d0 >= 0)
		smat3 = (mlib_s32)(d0 * scalef + 0.5);
	else
		smat3 = (mlib_s32)(d0 * scalef - 0.5);

	d0 = (fmat[6] + fmat[7] + fmat[8] - 1) * 32768.0;

	if (d0 >= 0)
		smat6 = (mlib_s32)(d0 * scalef + 0.5);
	else
		smat6 = (mlib_s32)(d0 * scalef - 0.5);

	doffset0 = vis_to_double_dup(smat0);
	doffset1 = vis_to_double_dup(smat3);
	doffset2 = vis_to_double_dup(smat6);

/* prepare mlib_f32 kernel */
#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = 65536.0;
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

	sp = (mlib_u16 *)src;
	dp = dl = (mlib_u16 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		off = ((mlib_addr)sp & 7) >> 1;
		off = (off > dsize) ? dsize : off;

		for (i = 0; i < 3 * off; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8;

			SAT_U16(dp, sum1);
			SAT_U16(dp, sum2);
			SAT_U16(dp, sum3);
		}

		i = i / 3;

		if ((dsize - off - 4) > 0) {

			psrc = (mlib_d64 *)(sp + 3 * off);
			sd0 = psrc[0];
			sd1 = psrc[1];
			sd2 = psrc[2];

			CHANNELSEPARATE_U16(sd0, sd1, sd2, rdd, gdd, bdd);

			shift_dst = 0;

#pragma pipeloop(0)
			for (; i < (dsize - 8); i += 4) {

				STORE_PIXEL_S16(18, 20, 22);
				STORE_PIXEL_S16(12, 14, 16);
				STORE_PIXEL_S16(6, 8, 10);

				vis_st_u16_i(d2, dp, 4);
				vis_st_u16_i(d1, dp, 2);
				vis_st_u16_i(d0, dp, 0);

				CHANNELMUL_S16(0, rdd, k0, k3, k6);
				CHANNELMUL_S16(1, gdd, k1, k4, k7);
				CHANNELADD_S16(1);
				CHANNELMUL_S16(2, bdd, k2, k5, k8);
				CHANNELADD_S16(2);
				CHANNELADD_OFFSET_S16();

				d3 = vis_fpackfix_pair(dd00, dd01);
				d4 = vis_fpackfix_pair(dd02, dd03);
				d5 = vis_fpackfix_pair(dd04, dd05);
				CHANNEL_XOR_U16();

				dp += shift_dst;
				shift_dst = 12;
				psrc += 3;
				d0 = d3;
				d1 = d4;
				d2 = d5;

				sd0 = psrc[0];
				sd1 = psrc[1];
				sd2 = psrc[2];

				CHANNELSEPARATE_U16(sd0, sd1, sd2, rdd, gdd,
				    bdd);
			}

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);

			CHANNELMUL_S16(0, rdd, k0, k3, k6);
			CHANNELMUL_S16(1, gdd, k1, k4, k7);
			CHANNELADD_S16(1);
			CHANNELMUL_S16(2, bdd, k2, k5, k8);
			CHANNELADD_S16(2);
			CHANNELADD_OFFSET_S16();

			d3 = vis_fpackfix_pair(dd00, dd01);
			d4 = vis_fpackfix_pair(dd02, dd03);
			d5 = vis_fpackfix_pair(dd04, dd05);
			CHANNEL_XOR_U16();

			dp += shift_dst;
			shift_dst = 12;
			psrc += 3;
			d0 = d3;
			d1 = d4;
			d2 = d5;

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);
			dp += shift_dst;
			i += 4;
		}

		i = i * 3;

		for (; i < dsize * 3; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8;

			SAT_U16(dp, sum1);
			SAT_U16(dp, sum2);
			SAT_U16(dp, sum3);
		}

		sp = sp + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert2_U16(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offsets,
    mlib_s32 scale)
{
/* pointer to src address */
	mlib_u16 *sp;

/* pointer to dst address */
	mlib_u16 *dp, *dl;

/* destination data */
	mlib_d64 d0, d1, d2, d3, d4, d5;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* offsets for mlib_u32 version */
	mlib_d64 doffset0, doffset1, doffset2;
	mlib_d64 tmp_doffset0, tmp_doffset1, tmp_doffset2;

/* source data after merge */
	mlib_d64 rdd, gdd, bdd;

/* coeff's for C-part */
	mlib_f32 k0, k1, k2, k3, k4, k5, k6, k7, k8;

/* pointer to src address */
	mlib_d64 *psrc;

/* temporaries used in macro */
	mlib_d64 sda, sdb, sdc;

/* temporaries used in macro */
	mlib_d64 dd00, dd01, dd02, dd03, dd04, dd05;

/* temporaries used in macro */
	mlib_d64 dd10, dd11, dd12, dd13, dd14, dd15;

/* temporaries used in macro */
	mlib_d64 dd20, dd21, dd22, dd23, dd24, dd25;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's for C-part */
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8;

/* offsets for VIS version */
	mlib_d64 offc0, offc1, offc2;

/* mlib_d64 source data */
	mlib_d64 p0, p1, p2;

/* loop variables */
	mlib_s32 i, j, off;
	mlib_s32 smat0, smat1, smat2, smat3, smat4;
	mlib_s32 smat5, smat6, smat7, smat8;
	mlib_s32 scalef;
	mlib_s32 shift_dst;
	mlib_d64 offset = vis_to_double_dup(0x80008000);

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_d64 sat = (mlib_d64)0x80000000;
#endif /* MLIB_USE_FTOI_CLAMPING */

	scalef = (0x10000 >> scale);

	if (fmat[0] >= 0)
		smat0 = (mlib_s32)(fmat[0] * scalef + 0.5) & 0xFFFF;
	else
		smat0 = (mlib_s32)(fmat[0] * scalef - 0.5) & 0xFFFF;

	if (fmat[1] >= 0)
		smat1 = (mlib_s32)(fmat[1] * scalef + 0.5) & 0xFFFF;
	else
		smat1 = (mlib_s32)(fmat[1] * scalef - 0.5) & 0xFFFF;

	if (fmat[2] >= 0)
		smat2 = (mlib_s32)(fmat[2] * scalef + 0.5) & 0xFFFF;
	else
		smat2 = (mlib_s32)(fmat[2] * scalef - 0.5) & 0xFFFF;

	if (fmat[3] >= 0)
		smat3 = (mlib_s32)(fmat[3] * scalef + 0.5) & 0xFFFF;
	else
		smat3 = (mlib_s32)(fmat[3] * scalef - 0.5) & 0xFFFF;

	if (fmat[4] >= 0)
		smat4 = (mlib_s32)(fmat[4] * scalef + 0.5) & 0xFFFF;
	else
		smat4 = (mlib_s32)(fmat[4] * scalef - 0.5) & 0xFFFF;

	if (fmat[5] >= 0)
		smat5 = (mlib_s32)(fmat[5] * scalef + 0.5) & 0xFFFF;
	else
		smat5 = (mlib_s32)(fmat[5] * scalef - 0.5) & 0xFFFF;

	if (fmat[6] >= 0)
		smat6 = (mlib_s32)(fmat[6] * scalef + 0.5) & 0xFFFF;
	else
		smat6 = (mlib_s32)(fmat[6] * scalef - 0.5) & 0xFFFF;

	if (fmat[7] >= 0)
		smat7 = (mlib_s32)(fmat[7] * scalef + 0.5) & 0xFFFF;
	else
		smat7 = (mlib_s32)(fmat[7] * scalef - 0.5) & 0xFFFF;

	if (fmat[8] >= 0)
		smat8 = (mlib_s32)(fmat[8] * scalef + 0.5) & 0xFFFF;
	else
		smat8 = (mlib_s32)(fmat[8] * scalef - 0.5) & 0xFFFF;

/* prepare the weight */
	k0 = vis_to_float((smat0 << 16) + smat0);
	k1 = vis_to_float((smat1 << 16) + smat1);
	k2 = vis_to_float((smat2 << 16) + smat2);
	k3 = vis_to_float((smat3 << 16) + smat3);
	k4 = vis_to_float((smat4 << 16) + smat4);
	k5 = vis_to_float((smat5 << 16) + smat5);
	k6 = vis_to_float((smat6 << 16) + smat6);
	k7 = vis_to_float((smat7 << 16) + smat7);
	k8 = vis_to_float((smat8 << 16) + smat8);

/* prepare offsets */
	d0 = (fmat[0] + fmat[1] + fmat[2]) * 32768.0;

	if (d0 >= 0)
		smat0 = (mlib_s32)(d0 * scalef + 0.5);
	else
		smat0 = (mlib_s32)(d0 * scalef - 0.5);

	d0 = (fmat[3] + fmat[4] + fmat[5]) * 32768.0;

	if (d0 >= 0)
		smat3 = (mlib_s32)(d0 * scalef + 0.5);
	else
		smat3 = (mlib_s32)(d0 * scalef - 0.5);

	d0 = (fmat[6] + fmat[7] + fmat[8]) * 32768.0;

	if (d0 >= 0)
		smat6 = (mlib_s32)(d0 * scalef + 0.5);
	else
		smat6 = (mlib_s32)(d0 * scalef - 0.5);

	tmp_doffset0 = vis_to_double_dup(smat0);
	tmp_doffset1 = vis_to_double_dup(smat3);
	tmp_doffset2 = vis_to_double_dup(smat6);

	smat0 = (mlib_s32)(32768.0 * scalef);

	d0 = vis_to_double_dup(smat0);

	tmp_doffset0 = vis_fpsub32(tmp_doffset0, d0);
	tmp_doffset1 = vis_fpsub32(tmp_doffset1, d0);
	tmp_doffset2 = vis_fpsub32(tmp_doffset2, d0);

	if (offsets[0] >= 0)
		smat0 = (mlib_s32)(offsets[0] * scalef + 0.5);
	else
		smat0 = (mlib_s32)(offsets[0] * scalef - 0.5);

	if (offsets[1] >= 0)
		smat1 = (mlib_s32)(offsets[1] * scalef + 0.5);
	else
		smat1 = (mlib_s32)(offsets[1] * scalef - 0.5);

	if (offsets[2] >= 0)
		smat2 = (mlib_s32)(offsets[2] * scalef + 0.5);
	else
		smat2 = (mlib_s32)(offsets[2] * scalef - 0.5);

	doffset0 = vis_to_double_dup(smat0);
	doffset1 = vis_to_double_dup(smat1);
	doffset2 = vis_to_double_dup(smat2);

	doffset0 = vis_fpadd32(doffset0, tmp_doffset0);
	doffset1 = vis_fpadd32(doffset1, tmp_doffset1);
	doffset2 = vis_fpadd32(doffset2, tmp_doffset2);

/* prepare mlib_f32 kernel&offsets */
#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = 65536.0;
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

	offc0 = scalef * offsets[0];
	offc1 = scalef * offsets[1];
	offc2 = scalef * offsets[2];

	sp = (mlib_u16 *)src;
	dp = dl = (mlib_u16 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		off = ((mlib_addr)sp & 7) >> 1;
		off = (off > dsize) ? dsize : off;

		for (i = 0; i < 3 * off; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_U16(dp, sum1);
			SAT_U16(dp, sum2);
			SAT_U16(dp, sum3);
		}

		i = i / 3;

		if ((dsize - off - 4) > 0) {

			psrc = (mlib_d64 *)(sp + 3 * off);
			sd0 = psrc[0];
			sd1 = psrc[1];
			sd2 = psrc[2];

			CHANNELSEPARATE_U16(sd0, sd1, sd2, rdd, gdd, bdd);

			shift_dst = 0;

#pragma pipeloop(0)
			for (; i < (dsize - 8); i += 4) {

				STORE_PIXEL_S16(18, 20, 22);
				STORE_PIXEL_S16(12, 14, 16);
				STORE_PIXEL_S16(6, 8, 10);

				vis_st_u16_i(d2, dp, 4);
				vis_st_u16_i(d1, dp, 2);
				vis_st_u16_i(d0, dp, 0);

				CHANNELMUL_S16(0, rdd, k0, k3, k6);
				CHANNELMUL_S16(1, gdd, k1, k4, k7);
				CHANNELADD_S16(1);
				CHANNELMUL_S16(2, bdd, k2, k5, k8);
				CHANNELADD_S16(2);
				CHANNELADD_OFFSET_S16();

				d3 = vis_fpackfix_pair(dd00, dd01);
				d4 = vis_fpackfix_pair(dd02, dd03);
				d5 = vis_fpackfix_pair(dd04, dd05);
				CHANNEL_XOR_U16();

				dp += shift_dst;
				shift_dst = 12;
				psrc += 3;
				d0 = d3;
				d1 = d4;
				d2 = d5;

				sd0 = psrc[0];
				sd1 = psrc[1];
				sd2 = psrc[2];

				CHANNELSEPARATE_U16(sd0, sd1, sd2, rdd, gdd,
				    bdd);
			}

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);

			CHANNELMUL_S16(0, rdd, k0, k3, k6);
			CHANNELMUL_S16(1, gdd, k1, k4, k7);
			CHANNELADD_S16(1);
			CHANNELMUL_S16(2, bdd, k2, k5, k8);
			CHANNELADD_S16(2);
			CHANNELADD_OFFSET_S16();

			d3 = vis_fpackfix_pair(dd00, dd01);
			d4 = vis_fpackfix_pair(dd02, dd03);
			d5 = vis_fpackfix_pair(dd04, dd05);
			CHANNEL_XOR_U16();

			dp += shift_dst;
			shift_dst = 12;
			psrc += 3;
			d0 = d3;
			d1 = d4;
			d2 = d5;

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);
			dp += shift_dst;
			i += 4;
		}

		i = i * 3;

		for (; i < dsize * 3; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_U16(dp, sum1);
			SAT_U16(dp, sum2);
			SAT_U16(dp, sum3);
		}

		sp = sp + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert2_S16(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale)
{
/* pointer to src address */
	mlib_s16 *sp;

/* pointer to dst address */
	mlib_s16 *dp, *dl;

/* destination data */
	mlib_d64 d0, d1, d2, d3, d4, d5;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* source data after merge */
	mlib_d64 rdd, gdd, bdd;

/* coeff's for VIS version */
	mlib_f32 k0, k1, k2, k3, k4, k5, k6, k7, k8;

/* offsets for VIS version */
	mlib_d64 doffset0, doffset1, doffset2;

/* pointer to src address */
	mlib_d64 *psrc;

/* temporaries used in macro */
	mlib_d64 sda, sdb, sdc;

/* temporaries used in macro */
	mlib_d64 dd00, dd01, dd02, dd03, dd04, dd05;

/* temporaries used in macro */
	mlib_d64 dd10, dd11, dd12, dd13, dd14, dd15;

/* temporaries used in macro */
	mlib_d64 dd20, dd21, dd22, dd23, dd24, dd25;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's for VIS version */
	mlib_d64 kk0, kk1, kk2, kk3, kk4, kk5, kk6, kk7, kk8;

/* offsets for VIS version */
	mlib_d64 offc0, offc1, offc2;

/* mlib_d64 source data */
	mlib_d64 p0, p1, p2;

/* loop variables */
	mlib_s32 i, j, off;
	mlib_s32 smat0, smat1, smat2, smat3, smat4;
	mlib_s32 smat5, smat6, smat7, smat8;
	mlib_s32 scalef;
	mlib_s32 shift_dst;

	scalef = (0x10000 >> scale);

	if (fmat[0] >= 0)
		smat0 = (mlib_s32)(fmat[0] * scalef + 0.5) & 0xFFFF;
	else
		smat0 = (mlib_s32)(fmat[0] * scalef - 0.5) & 0xFFFF;

	if (fmat[1] >= 0)
		smat1 = (mlib_s32)(fmat[1] * scalef + 0.5) & 0xFFFF;
	else
		smat1 = (mlib_s32)(fmat[1] * scalef - 0.5) & 0xFFFF;

	if (fmat[2] >= 0)
		smat2 = (mlib_s32)(fmat[2] * scalef + 0.5) & 0xFFFF;
	else
		smat2 = (mlib_s32)(fmat[2] * scalef - 0.5) & 0xFFFF;

	if (fmat[3] >= 0)
		smat3 = (mlib_s32)(fmat[3] * scalef + 0.5) & 0xFFFF;
	else
		smat3 = (mlib_s32)(fmat[3] * scalef - 0.5) & 0xFFFF;

	if (fmat[4] >= 0)
		smat4 = (mlib_s32)(fmat[4] * scalef + 0.5) & 0xFFFF;
	else
		smat4 = (mlib_s32)(fmat[4] * scalef - 0.5) & 0xFFFF;

	if (fmat[5] >= 0)
		smat5 = (mlib_s32)(fmat[5] * scalef + 0.5) & 0xFFFF;
	else
		smat5 = (mlib_s32)(fmat[5] * scalef - 0.5) & 0xFFFF;

	if (fmat[6] >= 0)
		smat6 = (mlib_s32)(fmat[6] * scalef + 0.5) & 0xFFFF;
	else
		smat6 = (mlib_s32)(fmat[6] * scalef - 0.5) & 0xFFFF;

	if (fmat[7] >= 0)
		smat7 = (mlib_s32)(fmat[7] * scalef + 0.5) & 0xFFFF;
	else
		smat7 = (mlib_s32)(fmat[7] * scalef - 0.5) & 0xFFFF;

	if (fmat[8] >= 0)
		smat8 = (mlib_s32)(fmat[8] * scalef + 0.5) & 0xFFFF;
	else
		smat8 = (mlib_s32)(fmat[8] * scalef - 0.5) & 0xFFFF;

/* prepare the weight */
	k0 = vis_to_float((smat0 << 16) + smat0);
	k1 = vis_to_float((smat1 << 16) + smat1);
	k2 = vis_to_float((smat2 << 16) + smat2);
	k3 = vis_to_float((smat3 << 16) + smat3);
	k4 = vis_to_float((smat4 << 16) + smat4);
	k5 = vis_to_float((smat5 << 16) + smat5);
	k6 = vis_to_float((smat6 << 16) + smat6);
	k7 = vis_to_float((smat7 << 16) + smat7);
	k8 = vis_to_float((smat8 << 16) + smat8);

	if (offset[0] >= 0)
		smat0 = (mlib_s32)(offset[0] * scalef + 0.5);
	else
		smat0 = (mlib_s32)(offset[0] * scalef - 0.5);

	if (offset[1] >= 0)
		smat1 = (mlib_s32)(offset[1] * scalef + 0.5);
	else
		smat1 = (mlib_s32)(offset[1] * scalef - 0.5);

	if (offset[2] >= 0)
		smat2 = (mlib_s32)(offset[2] * scalef + 0.5);
	else
		smat2 = (mlib_s32)(offset[2] * scalef - 0.5);

	doffset0 = vis_to_double_dup(smat0);
	doffset1 = vis_to_double_dup(smat1);
	doffset2 = vis_to_double_dup(smat2);

#ifdef MLIB_USE_FTOI_CLAMPING
	scalef = 65536.0;
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

	sp = (mlib_s16 *)src;
	dp = dl = (mlib_s16 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		off = ((mlib_addr)sp & 7) >> 1;
		off = (off > dsize) ? dsize : off;

		for (i = 0; i < 3 * off; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_S16(dp, sum1);
			SAT_S16(dp, sum2);
			SAT_S16(dp, sum3);
		}

		i = i / 3;

		if ((dsize - off - 4) > 0) {

			psrc = (mlib_d64 *)(sp + 3 * off);
			sd0 = psrc[0];
			sd1 = psrc[1];
			sd2 = psrc[2];

			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);

			shift_dst = 0;

#pragma pipeloop(0)
			for (; i < (dsize - 8); i += 4) {
				STORE_PIXEL_S16(18, 20, 22);
				STORE_PIXEL_S16(12, 14, 16);
				STORE_PIXEL_S16(6, 8, 10);

				vis_st_u16_i(d2, dp, 4);
				vis_st_u16_i(d1, dp, 2);
				vis_st_u16_i(d0, dp, 0);

				CHANNELMUL_S16(0, rdd, k0, k3, k6);
				CHANNELMUL_S16(1, gdd, k1, k4, k7);
				CHANNELADD_S16(1);
				CHANNELMUL_S16(2, bdd, k2, k5, k8);
				CHANNELADD_S16(2);
				CHANNELADD_OFFSET_S16();

				d3 = vis_fpackfix_pair(dd00, dd01);
				d4 = vis_fpackfix_pair(dd02, dd03);
				d5 = vis_fpackfix_pair(dd04, dd05);

				dp += shift_dst;
				shift_dst = 12;
				psrc += 3;
				d0 = d3;
				d1 = d4;
				d2 = d5;

				sd0 = psrc[0];
				sd1 = psrc[1];
				sd2 = psrc[2];

				CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd,
				    bdd);
			}

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);

			CHANNELMUL_S16(0, rdd, k0, k3, k6);
			CHANNELMUL_S16(1, gdd, k1, k4, k7);
			CHANNELADD_S16(1);
			CHANNELMUL_S16(2, bdd, k2, k5, k8);
			CHANNELADD_S16(2);
			CHANNELADD_OFFSET_S16();

			d3 = vis_fpackfix_pair(dd00, dd01);
			d4 = vis_fpackfix_pair(dd02, dd03);
			d5 = vis_fpackfix_pair(dd04, dd05);

			dp += shift_dst;
			shift_dst = 12;
			psrc += 3;
			d0 = d3;
			d1 = d4;
			d2 = d5;

			STORE_PIXEL_S16(18, 20, 22);
			STORE_PIXEL_S16(12, 14, 16);
			STORE_PIXEL_S16(6, 8, 10);

			vis_st_u16_i(d2, dp, 4);
			vis_st_u16_i(d1, dp, 2);
			vis_st_u16_i(d0, dp, 0);
			dp += shift_dst;
			i += 4;
		}

		i = i * 3;

		for (; i < dsize * 3; i += 3) {
			p0 = sp[i];
			p1 = sp[i + 1];
			p2 = sp[i + 2];
			sum1 = p0 * kk0 + p1 * kk1 + p2 * kk2 + offc0;
			sum2 = p0 * kk3 + p1 * kk4 + p2 * kk5 + offc1;
			sum3 = p0 * kk6 + p1 * kk7 + p2 * kk8 + offc2;

			SAT_S16(dp, sum1);
			SAT_S16(dp, sum2);
			SAT_S16(dp, sum3);
		}

		sp = sp + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert1_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat)
{
/* pointers for pixel and line of source */
	mlib_s32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_s32 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = sa[i];
			p1 = sa[i + 1];
			p2 = sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8;

			FLOAT2INT_CLAMP(da, sum1);
			FLOAT2INT_CLAMP(da, sum2);
			FLOAT2INT_CLAMP(da, sum3);
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorConvert2_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
/* pointers for pixel and line of source */
	mlib_s32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_s32 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;
	mlib_d64 off0, off1, off2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	off0 = offset[0];
	off1 = offset[1];
	off2 = offset[2];

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = sa[i];
			p1 = sa[i + 1];
			p2 = sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2 + off0;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5 + off1;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8 + off2;

			FLOAT2INT_CLAMP(da, sum1);
			FLOAT2INT_CLAMP(da, sum2);
			FLOAT2INT_CLAMP(da, sum3);
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}
}

/* *********************************************************** */
