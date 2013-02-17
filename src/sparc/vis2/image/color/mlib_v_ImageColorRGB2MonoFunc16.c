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

#pragma ident	"@(#)mlib_v_ImageColorRGB2MonoFunc16.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageColorRGB2Mono_S16_A8D1X4
 *      mlib_v_ImageColorRGB2Mono_S16_A8D2X4
 *      mlib_v_ImageColorRGB2Mono_S16_D1
 *      mlib_v_ImageColorRGB2Mono_S16
 *      mlib_v_ImageColorRGB2Mono_GEN_S16_A8D1X4
 *      mlib_v_ImageColorRGB2Mono_GEN_S16_A8D2X4
 *      mlib_v_ImageColorRGB2Mono_GEN_S16_D1
 *      mlib_v_ImageColorRGB2Mono_GEN_S16
 *      mlib_v_ImageColorRGB2Mono_U16_A8D1X4
 *      mlib_v_ImageColorRGB2Mono_U16_A8D2X4
 *      mlib_v_ImageColorRGB2Mono_U16_D1
 *      mlib_v_ImageColorRGB2Mono_U16
 *      mlib_v_ImageColorRGB2Mono_GEN_U16_A8D1X4
 *      mlib_v_ImageColorRGB2Mono_GEN_U16_A8D2X4
 *      mlib_v_ImageColorRGB2Mono_GEN_U16_D1
 *      mlib_v_ImageColorRGB2Mono_GEN_U16
 *
 * ARGUMENT
 *      src      pointer to source image data
 *      dst      pointer to destination image data
 *      slb      source image line stride in bytes
 *      dlb      destination image line stride in bytes
 *      dsize    image data size in pixels
 *      xsize    image width in pixels
 *      ysize    image height in lines
 *      weight   array of 3 blending coefficients
 *
 * DESCRIPTION
 *  Separate the three channels of the source image into three
 *  1-channel destination images -- VIS2 version low level functions.
 *
 * NOTE
 *  These functions are separated from mlib_v_ImageChannelColorRGB2Mono.c
 *  for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageColorRGB2MonoFunc.h>

/* *********************************************************** */

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

#define	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd)        \
	sda = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdb = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdd = vis_faligndata(sdb, sdc);                          \
	rdd = vis_bshuffle(sdb, sda);                            \
	gdd = vis_bshuffle(sdd, sdd);                            \
	bdd = vis_bshuffle(sda, sdc)

/* *********************************************************** */

#define	CHANNELWEIGHT_S16(rdd, gdd, bdd, dd)                    \
	ddt = vis_fmul8sux16(rdd, alpha);                       \
	dd = vis_fmul8ulx16(rdd, alpha);                        \
	sda = vis_fpadd16(ddt, dd);                             \
	ddt = vis_fmul8sux16(gdd, beta);                        \
	dd = vis_fmul8ulx16(gdd, beta);                         \
	sdb = vis_fpadd16(ddt, dd);                             \
	sda = vis_fpadd16(sda, sdb);                            \
	ddt = vis_fmul8sux16(bdd, gamma);                       \
	dd = vis_fmul8ulx16(bdd, gamma);                        \
	sdb = vis_fpadd16(ddt, dd);                             \
	dd = vis_fpadd16(sda, sdb);                             \
	dd = vis_fpadd16(dd, dd)

/* *********************************************************** */

#define	CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd)                \
	ddt_hi = vis_fmuld8sux16(vis_read_hi(rdd), alpha);      \
	dd_hi = vis_fmuld8ulx16(vis_read_hi(rdd), alpha);       \
	sda_hi = vis_fpadd32(ddt_hi, dd_hi);                    \
	ddt_lo = vis_fmuld8sux16(vis_read_lo(rdd), alpha);      \
	dd_lo = vis_fmuld8ulx16(vis_read_lo(rdd), alpha);       \
	sda_lo = vis_fpadd32(ddt_lo, dd_lo);                    \
	ddt_hi = vis_fmuld8sux16(vis_read_hi(gdd), beta);       \
	dd_hi = vis_fmuld8ulx16(vis_read_hi(gdd), beta);        \
	sdb_hi = vis_fpadd32(ddt_hi, dd_hi);                    \
	ddt_lo = vis_fmuld8sux16(vis_read_lo(gdd), beta);       \
	dd_lo = vis_fmuld8ulx16(vis_read_lo(gdd), beta);        \
	sdb_lo = vis_fpadd32(ddt_lo, dd_lo);                    \
	sda_hi = vis_fpadd32(sda_hi, sdb_hi);                   \
	sda_lo = vis_fpadd32(sda_lo, sdb_lo);                   \
	ddt_hi = vis_fmuld8sux16(vis_read_hi(bdd), gamma);      \
	dd_hi = vis_fmuld8ulx16(vis_read_hi(bdd), gamma);       \
	sdb_hi = vis_fpadd32(ddt_hi, dd_hi);                    \
	ddt_lo = vis_fmuld8sux16(vis_read_lo(bdd), gamma);      \
	dd_lo = vis_fmuld8ulx16(vis_read_lo(bdd), gamma);       \
	sdb_lo = vis_fpadd32(ddt_lo, dd_lo);                    \
	dd_hi = vis_fpadd32(sda_hi, sdb_hi);                    \
	dd_lo = vis_fpadd32(sda_lo, sdb_lo);                    \
	dd = vis_fpackfix_pair(dd_hi, dd_lo)

/* *********************************************************** */

#define	CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back)          \
	ddt_hi = vis_fmuld8sux16(vis_read_hi(rdd), alpha);      \
	dd_hi = vis_fmuld8ulx16(vis_read_hi(rdd), alpha);       \
	sda_hi = vis_fpadd32(ddt_hi, dd_hi);                    \
	ddt_lo = vis_fmuld8sux16(vis_read_lo(rdd), alpha);      \
	dd_lo = vis_fmuld8ulx16(vis_read_lo(rdd), alpha);       \
	sda_lo = vis_fpadd32(ddt_lo, dd_lo);                    \
	ddt_hi = vis_fmuld8sux16(vis_read_hi(gdd), beta);       \
	dd_hi = vis_fmuld8ulx16(vis_read_hi(gdd), beta);        \
	sdb_hi = vis_fpadd32(ddt_hi, dd_hi);                    \
	ddt_lo = vis_fmuld8sux16(vis_read_lo(gdd), beta);       \
	dd_lo = vis_fmuld8ulx16(vis_read_lo(gdd), beta);        \
	sdb_lo = vis_fpadd32(ddt_lo, dd_lo);                    \
	sda_hi = vis_fpadd32(sda_hi, sdb_hi);                   \
	sda_lo = vis_fpadd32(sda_lo, sdb_lo);                   \
	ddt_hi = vis_fmuld8sux16(vis_read_hi(bdd), gamma);      \
	dd_hi = vis_fmuld8ulx16(vis_read_hi(bdd), gamma);       \
	sdb_hi = vis_fpadd32(ddt_hi, dd_hi);                    \
	ddt_lo = vis_fmuld8sux16(vis_read_lo(bdd), gamma);      \
	dd_lo = vis_fmuld8ulx16(vis_read_lo(bdd), gamma);       \
	sdb_lo = vis_fpadd32(ddt_lo, dd_lo);                    \
	dd_hi = vis_fpadd32(sda_hi, sdb_hi);                    \
	dd_lo = vis_fpadd32(sda_lo, sdb_lo);                    \
	dd_hi = vis_fpadd32(dd_hi, back);                       \
	dd_lo = vis_fpadd32(dd_lo, back);                       \
	dd_hi = vis_fpsub32(dd_hi, dip);                        \
	dd_lo = vis_fpsub32(dd_lo, dip);                        \
	dd = vis_fpackfix_pair(dd_hi, dd_lo)

/* *********************************************************** */

static const mlib_s32 bmask_table[8] = {
	0x01234567,
	0x12345678,
	0x23456789,
	0x3456789A,
	0x456789AB,
	0x56789ABC,
	0x6789ABCD,
	0x789ABCDE,
};

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_S16_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd, ddt, alpha, gamma, beta;
	mlib_f32 zalpha, zgamma, zbeta;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
/* prepare the weight */

	vis_write_gsr(4);

	zalpha = vis_to_float(weight[0] * 32767);
	zbeta = vis_to_float(weight[1] * 32767);
	zgamma = vis_to_float(weight[2] * 32767);

	ddt = vis_fpmerge(zalpha, zalpha);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	alpha = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zbeta, zbeta);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	beta = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zgamma, zgamma);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	gamma = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	vis_write_bmask(0x02CE13DF, 0);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_S16_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd, ddt, alpha, gamma, beta;
	mlib_f32 zalpha, zgamma, zbeta;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(4);

/* prepare the weight  */
	zalpha = vis_to_float(weight[0] * 32767);
	zbeta = vis_to_float(weight[1] * 32767);
	zgamma = vis_to_float(weight[2] * 32767);

	ddt = vis_fpmerge(zalpha, zalpha);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	alpha = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zbeta, zbeta);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	beta = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zgamma, zgamma);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	gamma = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	vis_write_bmask(0x02CE13DF, 0);

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
			CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
			(*dp++) = dd;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_S16_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_s16 *dst_end;
	mlib_d64 *src_all, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 d0, d1, d2, d3;
	mlib_d64 sd01, sd11, sd21;
	mlib_d64 dd1, rdd1, gdd1, bdd1;
	mlib_d64 d4, d5, d6;

/* offset of address in src */
	mlib_s32 off;
	mlib_s32 i, emask;
	mlib_d64 dd, rdd, gdd, bdd, ddt, alpha, gamma, beta;
	mlib_f32 zalpha, zgamma, zbeta;
	mlib_s32 bmask1, bmask2 = 0x02CE13DF;

/* prepare the weight */
	zalpha = vis_to_float(weight[0] * 32767);
	zbeta = vis_to_float(weight[1] * 32767);
	zgamma = vis_to_float(weight[2] * 32767);

	ddt = vis_fpmerge(zalpha, zalpha);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	alpha = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zbeta, zbeta);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	beta = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zgamma, zgamma);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	gamma = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;

	dst_end = dst + (dsize - 1);
	emask = vis_edge16(dst, dst_end);
	src_all = VIS_ALIGNADDR(src, (3 * off));
	vis_write_gsr(4);

	d0 = (*src_all++);
	d1 = (*src_all++);
	d2 = (*src_all++);
	d3 = (*src_all++);

	bmask1 = bmask_table[(((mlib_addr)src) + (3 * off)) & 7];
	vis_write_bmask(bmask1, 0);

	sd0 = vis_bshuffle(d0, d1);
	sd1 = vis_bshuffle(d1, d2);
	sd2 = vis_bshuffle(d2, d3);

	vis_write_bmask(bmask2, 0);

	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
	CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
	vis_pst_16(dd, dp, emask);
	dp++;

	d6 = d3;
#pragma pipeloop(0)
	for (i = 4 + (off >> 1); i <= (dsize - 8); i += 8) {
		d0 = d6;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		d4 = (*src_all++);
		d5 = (*src_all++);
		d6 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		sd01 = vis_bshuffle(d3, d4);
		sd11 = vis_bshuffle(d4, d5);
		sd21 = vis_bshuffle(d5, d6);

		vis_write_bmask(bmask2, 0);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELSEPARATE_S16(sd01, sd11, sd21, rdd1, gdd1, bdd1);

		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		CHANNELWEIGHT_S16(rdd1, gdd1, bdd1, dd1);

		(*dp++) = dd;
		(*dp++) = dd1;
	}

	d3 = d6;
	for (; i <= (dsize - 4); i += 4) {
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);

		vis_write_bmask(bmask2, 0);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dst_end) {

/* VIS_ALIGNADDR(src, (3*off)); */
		emask = vis_edge16(dp, dst_end);
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);

		vis_write_bmask(bmask2, 0);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		vis_pst_16(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageColorRGB2Mono_S16_D1(sa, da, xsize, weight);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_S16_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd;
	mlib_s32 zalpha, zgamma, zbeta;
	mlib_f32 alpha, gamma, beta;
	mlib_s32 i;
	mlib_d64 dd_hi, dd_lo, ddt_hi, ddt_lo, sda_hi, sda_lo, sdb_hi, sdb_lo;

	vis_write_gsr(4 + (2 << 3));

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* prepare the weight */
	zalpha = weight[0] * 16384;
	zbeta = weight[1] * 16384;
	zgamma = weight[2] * 16384;

	alpha = vis_to_float((zalpha << 16) + (zalpha & 0xFFFF));
	beta = vis_to_float((zbeta << 16) + (zbeta & 0xFFFF));
	gamma = vis_to_float((zgamma << 16) + (zgamma & 0xFFFF));

	vis_write_bmask(0x02CE13DF, 0);

	sd0 = (*sp++);
	sd1 = (*sp++);
	sd2 = (*sp++);
	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd);
		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_S16_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd;
	mlib_s32 zalpha, zgamma, zbeta;
	mlib_f32 alpha, gamma, beta;
	mlib_s32 i, j;
	mlib_d64 dd_hi, dd_lo, ddt_hi, ddt_lo, sda_hi, sda_lo, sdb_hi, sdb_lo;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(4 + (2 << 3));

/* prepare the weight  */
	zalpha = weight[0] * 16384;
	zbeta = weight[1] * 16384;
	zgamma = weight[2] * 16384;

	alpha = vis_to_float((zalpha << 16) + (zalpha & 0xFFFF));
	beta = vis_to_float((zbeta << 16) + (zbeta & 0xFFFF));
	gamma = vis_to_float((zgamma << 16) + (zgamma & 0xFFFF));

	vis_write_bmask(0x02CE13DF, 0);

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		for (i = 0; i < xsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd);
			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
			(*dp++) = dd;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_S16_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_s16 *dst_end;
	mlib_d64 *src_all, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 d0, d1, d2, d3;

/* offset of address in src */
	mlib_s32 off;
	mlib_s32 i, emask;
	mlib_d64 dd, rdd, gdd, bdd;
	mlib_d64 sd01, sd11, sd21;
	mlib_d64 dd1, rdd1, gdd1, bdd1;
	mlib_d64 d4, d5, d6;
	mlib_f32 alpha, gamma, beta;
	mlib_s32 zalpha, zgamma, zbeta;
	mlib_d64 dd_hi, dd_lo, ddt_hi, ddt_lo, sda_hi, sda_lo, sdb_hi, sdb_lo;
	mlib_s32 bmask1, bmask2 = 0x02CE13DF;

/* prepare the weight */

	zalpha = weight[0] * 16384;
	zbeta = weight[1] * 16384;
	zgamma = weight[2] * 16384;

	alpha = vis_to_float((zalpha << 16) + (zalpha & 0xFFFF));
	beta = vis_to_float((zbeta << 16) + (zbeta & 0xFFFF));
	gamma = vis_to_float((zgamma << 16) + (zgamma & 0xFFFF));

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;

	dst_end = dst + (dsize - 1);
	emask = vis_edge16(dst, dst_end);
	src_all = VIS_ALIGNADDR(src, (3 * off));
	vis_write_gsr((2 << 3) + 4);

	d0 = (*src_all++);
	d1 = (*src_all++);
	d2 = (*src_all++);
	d3 = (*src_all++);

	bmask1 = bmask_table[(((mlib_addr)src) + (3 * off)) & 7];
	vis_write_bmask(bmask1, 0);

	sd0 = vis_bshuffle(d0, d1);
	sd1 = vis_bshuffle(d1, d2);
	sd2 = vis_bshuffle(d2, d3);

	vis_write_bmask(bmask2, 0);

	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
	CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd);
	vis_pst_16(dd, dp, emask);
	dp++;

	{
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		d4 = (*src_all++);
		d5 = (*src_all++);
		d6 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		sd01 = vis_bshuffle(d3, d4);
		sd11 = vis_bshuffle(d4, d5);
		sd21 = vis_bshuffle(d5, d6);

		vis_write_bmask(bmask2, 0);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELSEPARATE_S16(sd01, sd11, sd21, rdd1, gdd1, bdd1);
	}

#pragma pipeloop(0)
	for (i = 4 + (off >> 1); i <= (dsize - 8); i += 8) {
		d0 = d6;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		d4 = (*src_all++);
		d5 = (*src_all++);
		d6 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		sd01 = vis_bshuffle(d3, d4);
		sd11 = vis_bshuffle(d4, d5);
		sd21 = vis_bshuffle(d5, d6);

		vis_write_bmask(bmask2, 0);

		CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd);
		CHANNELWEIGHT_GEN_S16(rdd1, gdd1, bdd1, dd1);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELSEPARATE_S16(sd01, sd11, sd21, rdd1, gdd1, bdd1);

		(*dp++) = dd;
		(*dp++) = dd1;
	}

	if (i <= (dsize - 4)) {
		CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd);
		(*dp++) = dd;

		if ((mlib_addr)dp <= (mlib_addr)dst_end) {
			CHANNELWEIGHT_GEN_S16(rdd1, gdd1, bdd1, dd);
			emask = vis_edge16(dp, dst_end);
			vis_pst_16(dd, dp, emask);
		}
	} else if ((mlib_addr)dp <= (mlib_addr)dst_end) {
		CHANNELWEIGHT_GEN_S16(rdd, gdd, bdd, dd);
		emask = vis_edge16(dp, dst_end);
		vis_pst_16(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageColorRGB2Mono_GEN_S16_D1(sa, da, xsize, weight);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U16_A8D1X4(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd, ddt, alpha, gamma, beta;
	mlib_f32 zalpha, zgamma, zbeta;
	mlib_s32 i;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 back, dsum;
	mlib_s32 ssum;

	dsum = (weight[0] + weight[1] + weight[2]) * 32768;
	ssum = (mlib_s32)dsum;
	back = vis_to_double_dup((ssum << 16) | ssum);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(4);

/* prepare the weight */
	zalpha = vis_to_float(weight[0] * 32767);
	zbeta = vis_to_float(weight[1] * 32767);
	zgamma = vis_to_float(weight[2] * 32767);

	ddt = vis_fpmerge(zalpha, zalpha);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	alpha = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zbeta, zbeta);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	beta = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zgamma, zgamma);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	gamma = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	vis_write_bmask(0x02CE13DF, 0);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		dd = vis_fpadd16(dd, back);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U16_A8D2X4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd, ddt, alpha, gamma, beta;
	mlib_f32 zalpha, zgamma, zbeta;
	mlib_s32 i, j;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 back, dsum;
	mlib_s32 ssum;

	dsum = (weight[0] + weight[1] + weight[2]) * 32768;
	ssum = (mlib_s32)dsum;
	back = vis_to_double_dup((ssum << 16) | ssum);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(4);

/* prepare the weight  */
	zalpha = vis_to_float(weight[0] * 32767);
	zbeta = vis_to_float(weight[1] * 32767);
	zgamma = vis_to_float(weight[2] * 32767);

	ddt = vis_fpmerge(zalpha, zalpha);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	alpha = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zbeta, zbeta);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	beta = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zgamma, zgamma);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	gamma = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	vis_write_bmask(0x02CE13DF, 0);

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd0 = vis_fxor(sd0, xor_mask);
			sd1 = vis_fxor(sd1, xor_mask);
			sd2 = vis_fxor(sd2, xor_mask);
			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
			CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
			dd = vis_fpadd16(dd, back);
			(*dp++) = dd;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U16_D1(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_u16 *dst_end;
	mlib_d64 *src_all, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 d0, d1, d2, d3;
	mlib_d64 sd01, sd11, sd21;
	mlib_d64 dd1, rdd1, gdd1, bdd1;
	mlib_d64 d4, d5, d6;

/* offset of address in src */
	mlib_s32 off;
	mlib_s32 i, emask;
	mlib_d64 dd, rdd, gdd, bdd, ddt, alpha, gamma, beta;
	mlib_f32 zalpha, zgamma, zbeta;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 back, dsum;
	mlib_s32 ssum;
	mlib_s32 bmask1, bmask2 = 0x02CE13DF;

	dsum = (weight[0] + weight[1] + weight[2]) * 32768;
	ssum = (mlib_s32)dsum;
	back = vis_to_double_dup((ssum << 16) | ssum);

/* prepare the weight */
	zalpha = vis_to_float(weight[0] * 32767);
	zbeta = vis_to_float(weight[1] * 32767);
	zgamma = vis_to_float(weight[2] * 32767);

	ddt = vis_fpmerge(zalpha, zalpha);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	alpha = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zbeta, zbeta);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	beta = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	ddt = vis_fpmerge(zgamma, zgamma);
	ddt = vis_fpmerge(vis_read_lo(ddt), vis_read_lo(ddt));
	gamma = vis_fpmerge(vis_read_hi(ddt), vis_read_lo(ddt));

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;

	dst_end = dst + (dsize - 1);
	emask = vis_edge16(dst, dst_end);
	src_all = VIS_ALIGNADDR(src, (3 * off));
	vis_write_gsr(4);

	d0 = (*src_all++);
	d1 = (*src_all++);
	d2 = (*src_all++);
	d3 = (*src_all++);

	bmask1 = bmask_table[(((mlib_addr)src) + (3 * off)) & 7];
	vis_write_bmask(bmask1, 0);

	sd0 = vis_bshuffle(d0, d1);
	sd1 = vis_bshuffle(d1, d2);
	sd2 = vis_bshuffle(d2, d3);

	sd0 = vis_fxor(sd0, xor_mask);
	sd1 = vis_fxor(sd1, xor_mask);
	sd2 = vis_fxor(sd2, xor_mask);

	vis_write_bmask(bmask2, 0);
	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
	CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
	dd = vis_fpadd16(dd, back);
	vis_pst_16(dd, dp, emask);
	dp++;

	d6 = d3;
#pragma pipeloop(0)
	for (i = 4 + (off >> 1); i <= (dsize - 8); i += 8) {

		d0 = d6;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		d4 = (*src_all++);
		d5 = (*src_all++);
		d6 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		sd01 = vis_bshuffle(d3, d4);
		sd11 = vis_bshuffle(d4, d5);
		sd21 = vis_bshuffle(d5, d6);

		vis_write_bmask(bmask2, 0);

		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		sd01 = vis_fxor(sd01, xor_mask);
		sd11 = vis_fxor(sd11, xor_mask);
		sd21 = vis_fxor(sd21, xor_mask);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELSEPARATE_S16(sd01, sd11, sd21, rdd1, gdd1, bdd1);

		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		CHANNELWEIGHT_S16(rdd1, gdd1, bdd1, dd1);

		dd = vis_fpadd16(dd, back);
		dd1 = vis_fpadd16(dd1, back);
		(*dp++) = dd;
		(*dp++) = dd1;
	}

	d3 = d6;

	for (; i <= (dsize - 4); i += 4) {
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);

		vis_write_bmask(bmask1, 0);
		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		vis_write_bmask(bmask2, 0);
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);

		dd = vis_fpadd16(dd, back);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dst_end) {

		emask = vis_edge16(dp, dst_end);
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);

		vis_write_bmask(bmask1, 0);
		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		vis_write_bmask(bmask2, 0);
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELWEIGHT_S16(rdd, gdd, bdd, dd);
		dd = vis_fpadd16(dd, back);
		vis_pst_16(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_u16 *sa, *da;
	mlib_u16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageColorRGB2Mono_U16_D1(sa, da, xsize, weight);
		sa = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_U16_A8D1X4(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd;
	mlib_s32 zalpha, zgamma, zbeta;
	mlib_f32 alpha, gamma, beta;
	mlib_s32 i;
	mlib_d64 dd_hi, dd_lo, ddt_hi, ddt_lo, sda_hi, sda_lo, sdb_hi, sdb_lo;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 dip = vis_to_double_dup(0x20000000);
	mlib_d64 back, dsum;
	mlib_s32 ssum;

	dsum = (weight[0] + weight[1] + weight[2]) * 32768 * 16384;
	ssum = (mlib_s32)dsum;
	back = vis_to_double_dup(ssum);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* prepare the weight */
	zalpha = weight[0] * 16384;
	zbeta = weight[1] * 16384;
	zgamma = weight[2] * 16384;

	alpha = vis_to_float((zalpha << 16) + (zalpha & 0xFFFF));
	beta = vis_to_float((zbeta << 16) + (zbeta & 0xFFFF));
	gamma = vis_to_float((zgamma << 16) + (zgamma & 0xFFFF));

	vis_write_gsr((2 << 3) + 4);

	sd0 = (*sp++);
	sd1 = (*sp++);
	sd2 = (*sp++);

	vis_write_bmask(0x02CE13DF, 0);

	sd0 = vis_fxor(sd0, xor_mask);
	sd1 = vis_fxor(sd1, xor_mask);
	sd2 = vis_fxor(sd2, xor_mask);
	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back);
		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		dd = vis_fxor(dd, xor_mask);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_U16_A8D2X4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 dd;
	mlib_d64 rdd, gdd, bdd;
	mlib_s32 zalpha, zgamma, zbeta;
	mlib_f32 alpha, gamma, beta;
	mlib_s32 i, j;
	mlib_d64 dd_hi, dd_lo, ddt_hi, ddt_lo, sda_hi, sda_lo, sdb_hi, sdb_lo;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 dip = vis_to_double_dup(0x20000000);
	mlib_d64 back, dsum;
	mlib_s32 ssum;

	dsum = (weight[0] + weight[1] + weight[2]) * 32768 * 16384;
	ssum = (mlib_s32)dsum;
	back = vis_to_double_dup(ssum);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr((2 << 3) + 4);

/* prepare the weight  */
	zalpha = weight[0] * 16384;
	zbeta = weight[1] * 16384;
	zgamma = weight[2] * 16384;

	alpha = vis_to_float((zalpha << 16) + (zalpha & 0xFFFF));
	beta = vis_to_float((zbeta << 16) + (zbeta & 0xFFFF));
	gamma = vis_to_float((zgamma << 16) + (zgamma & 0xFFFF));

	vis_write_bmask(0x02CE13DF, 0);

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		for (i = 0; i < xsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd0 = vis_fxor(sd0, xor_mask);
			sd1 = vis_fxor(sd1, xor_mask);
			sd2 = vis_fxor(sd2, xor_mask);
			CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back);
			CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
			dd = vis_fxor(dd, xor_mask);
			(*dp++) = dd;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_U16_D1(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_u16 *dst_end;
	mlib_d64 *src_all, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 d0, d1, d2, d3;

/* offset of address in src */
	mlib_s32 off;
	mlib_s32 i, emask;
	mlib_d64 dd, rdd, gdd, bdd;
	mlib_d64 sd01, sd11, sd21;
	mlib_d64 dd1, rdd1, gdd1, bdd1;
	mlib_d64 d4, d5, d6;
	mlib_f32 alpha, gamma, beta;
	mlib_s32 zalpha, zgamma, zbeta;
	mlib_d64 dd_hi, dd_lo, ddt_hi, ddt_lo, sda_hi, sda_lo, sdb_hi, sdb_lo;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 dip = vis_to_double_dup(0x20000000);
	mlib_d64 back, dsum;
	mlib_s32 ssum;
	mlib_s32 bmask1, bmask2 = 0x02CE13DF;

	dsum = (weight[0] + weight[1] + weight[2]) * 32768 * 16384;
	ssum = (mlib_s32)dsum;
	back = vis_to_double_dup(ssum);

/* prepare the weight */
	zalpha = weight[0] * 16384;
	zbeta = weight[1] * 16384;
	zgamma = weight[2] * 16384;

	alpha = vis_to_float((zalpha << 16) + (zalpha & 0xFFFF));
	beta = vis_to_float((zbeta << 16) + (zbeta & 0xFFFF));
	gamma = vis_to_float((zgamma << 16) + (zgamma & 0xFFFF));

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;

	dst_end = dst + (dsize - 1);
	emask = vis_edge16(dst, dst_end);
	src_all = VIS_ALIGNADDR(src, (3 * off));
	vis_write_gsr((2 << 3) + 4);

	d0 = (*src_all++);
	d1 = (*src_all++);
	d2 = (*src_all++);
	d3 = (*src_all++);

	bmask1 = bmask_table[(((mlib_addr)src) + (3 * off)) & 7];
	vis_write_bmask(bmask1, 0);

	sd0 = vis_bshuffle(d0, d1);
	sd1 = vis_bshuffle(d1, d2);
	sd2 = vis_bshuffle(d2, d3);

	vis_write_bmask(bmask2, 0);

	sd0 = vis_fxor(sd0, xor_mask);
	sd1 = vis_fxor(sd1, xor_mask);
	sd2 = vis_fxor(sd2, xor_mask);

	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
	CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back);
	dd = vis_fxor(dd, xor_mask);
	vis_pst_16(dd, dp, emask);
	dp++;

	{
/* loop preheader */
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		d4 = (*src_all++);
		d5 = (*src_all++);
		d6 = (*src_all++);

		vis_write_bmask(bmask1, 0);

		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		sd01 = vis_bshuffle(d3, d4);
		sd11 = vis_bshuffle(d4, d5);
		sd21 = vis_bshuffle(d5, d6);

		vis_write_bmask(bmask2, 0);

		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		sd01 = vis_fxor(sd01, xor_mask);
		sd11 = vis_fxor(sd11, xor_mask);
		sd21 = vis_fxor(sd21, xor_mask);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELSEPARATE_S16(sd01, sd11, sd21, rdd1, gdd1, bdd1);
	}

#pragma pipeloop(0)
	for (i = 4 + (off >> 1); i <= (dsize - 8); i += 8) {
		d0 = d6;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		d4 = (*src_all++);
		d5 = (*src_all++);
		d6 = (*src_all++);

		vis_write_bmask(bmask1, 0);
		sd0 = vis_bshuffle(d0, d1);
		sd1 = vis_bshuffle(d1, d2);
		sd2 = vis_bshuffle(d2, d3);
		sd01 = vis_bshuffle(d3, d4);
		sd11 = vis_bshuffle(d4, d5);
		sd21 = vis_bshuffle(d5, d6);

		vis_write_bmask(bmask2, 0);
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		sd01 = vis_fxor(sd01, xor_mask);
		sd11 = vis_fxor(sd11, xor_mask);
		sd21 = vis_fxor(sd21, xor_mask);

		CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back);
		CHANNELWEIGHT_GEN_U16(rdd1, gdd1, bdd1, dd1, back);

		CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd);
		CHANNELSEPARATE_S16(sd01, sd11, sd21, rdd1, gdd1, bdd1);

		dd = vis_fxor(dd, xor_mask);
		dd1 = vis_fxor(dd1, xor_mask);

		(*dp++) = dd;
		(*dp++) = dd1;
	}

	if (i <= (dsize - 4)) {
		CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back);
		dd = vis_fxor(dd, xor_mask);
		(*dp++) = dd;

		if ((mlib_addr)dp <= (mlib_addr)dst_end) {
			CHANNELWEIGHT_GEN_U16(rdd1, gdd1, bdd1, dd, back);
			emask = vis_edge16(dp, dst_end);
			dd = vis_fxor(dd, xor_mask);
			vis_pst_16(dd, dp, emask);
		}
	} else if ((mlib_addr)dp <= (mlib_addr)dst_end) {
		CHANNELWEIGHT_GEN_U16(rdd, gdd, bdd, dd, back);
		emask = vis_edge16(dp, dst_end);
		dd = vis_fxor(dd, xor_mask);
		vis_pst_16(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_GEN_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_u16 *sa, *da;
	mlib_u16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageColorRGB2Mono_GEN_U16_D1(sa, da, xsize, weight);
		sa = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
