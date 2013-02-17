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

#pragma ident	"@(#)mlib_v_ImageMaximum.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMaximum - calculate maximum for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMaximum(mlib_s32         *max,
 *                                    const mlib_image *img)
 *
 * ARGUMENTS
 *      max     pointer to max vector
 *      img     pointer to an image
 *
 * DESCRIPTION
 *
 *      max = MAX (img[i][j]);
 *            i, j
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageExtrema.h>
#include <mlib_v_ImageStat.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMaximum = __mlib_ImageMaximum

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMaximum) mlib_ImageMaximum
    __attribute__((weak, alias("__mlib_ImageMaximum")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PXL_OPER(res, x)	res = (x > res) ? x : res

/* *********************************************************** */

#define	PXL_OPER_U16(res, x)                                    \
	{                                                       \
	    mlib_s32 delta = res - x;                           \
	    mlib_s32 mask = delta >> 31;                        \
	                                                        \
	    res -= (mask & delta);                              \
	}

/* *********************************************************** */

static void mlib_v_ImageMaximum_U8_124_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n);

static void mlib_v_ImageMaximum_U8_3_D1(
    mlib_d64 *pres,
    const mlib_f32 *sp,
    mlib_s32 n);

static void mlib_v_ImageMaximum_U8_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_v_ImageMaximum_U8_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_v_ImageMaximum_S16_124_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n);

static void mlib_v_ImageMaximum_S16_3_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n);

static void mlib_v_ImageMaximum_S16_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_v_ImageMaximum_S16_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_v_ImageMaximum_U16_124_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n);

static void mlib_v_ImageMaximum_U16_3_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n);

static void mlib_v_ImageMaximum_U16_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_v_ImageMaximum_U16_3(
    mlib_s32 *res,
    const mlib_image *img);

/* *********************************************************** */

mlib_status
__mlib_ImageMaximum(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s32 ires[24];
	mlib_s32 nchan;
	mlib_s32 i, k, num_res = 24;

	MLIB_IMAGE_CHECK(img);

	if (res == NULL)
		return (MLIB_NULLPOINTER);

	nchan = mlib_ImageGetChannels(img);

	if (nchan < 0 || nchan > 4)
		return (MLIB_FAILURE);

	switch (mlib_ImageGetType(img)) {
	case MLIB_BYTE:
		for (i = 0; i < 24; i++)
			ires[i] = MLIB_S32_MIN;

		if (nchan == 3) {
			mlib_v_ImageMaximum_U8_3(ires, img);
		} else {
			mlib_v_ImageMaximum_U8_124(ires, img);
		}

		break;
	case MLIB_SHORT:
		for (i = 0; i < 24; i++)
			ires[i] = MLIB_S32_MIN;

		if (nchan == 3) {
			mlib_v_ImageMaximum_S16_3(ires, img);
		} else {
			mlib_v_ImageMaximum_S16_124(ires, img);
		}

		break;
	case MLIB_USHORT:
		for (i = 0; i < 24; i++)
			ires[i] = 0;

		if (nchan == 3) {
			mlib_v_ImageMaximum_U16_3(ires, img);
		} else {
			mlib_v_ImageMaximum_U16_124(ires, img);
		}

		for (k = 0; k < nchan; k++) {
			res[k] = ires[k];
			for (i = k + nchan; i < num_res; i += nchan)
				PXL_OPER(res[k], ires[i]);
		}

		return (MLIB_SUCCESS);
	case MLIB_INT:
		for (i = 0; i < 24; i++)
			ires[i] = MLIB_S32_MIN;
		switch (nchan) {
		case 1:
		case 2:
			mlib_ImageMaximum_S32_124(ires, img);
			num_res = 4;
			break;
		case 3:
			mlib_ImageMaximum_S32_3(res, img);
			return (MLIB_SUCCESS);
		case 4:
			mlib_ImageMaximum_S32_124(res, img);
			return (MLIB_SUCCESS);
		}

		break;
	default:
		return (MLIB_FAILURE);
	}

	for (k = 0; k < nchan; k++) {
		res[k] = ires[k];
		for (i = k + nchan; i < num_res; i += nchan)
			PXL_OPER(res[k], ires[i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U8_124_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n)
{
	mlib_d64 *tbl_mask = (mlib_d64 *)mlib_tbl_mask_s16;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 i;

	dd0 = dd1 = dd2 = dd3 = vis_to_double_dup(0x80008000);

#pragma pipeloop(0)
	for (i = 0; i < (n / 16); i++) {
		mlib_d64 s01, s23;
		mlib_d64 sd0, sd1, sd2, sd3;
		mlib_d64 md0, md1, md2, md3;
		mlib_s32 m_0, m_1, m_2, m_3;

		s01 = sp[2 * i];
		s23 = sp[2 * i + 1];

		sd0 = vis_fmul8x16al(vis_read_hi(s01), fone);
		sd1 = vis_fmul8x16al(vis_read_lo(s01), fone);
		sd2 = vis_fmul8x16al(vis_read_hi(s23), fone);
		sd3 = vis_fmul8x16al(vis_read_lo(s23), fone);

		m_0 = vis_fcmplt16(dd0, sd0);
		m_1 = vis_fcmplt16(dd1, sd1);
		m_2 = vis_fcmplt16(dd2, sd2);
		m_3 = vis_fcmplt16(dd3, sd3);

		md0 = tbl_mask[m_0];
		md1 = tbl_mask[m_1];
		md2 = tbl_mask[m_2];
		md3 = tbl_mask[m_3];

		sd0 = vis_fand(md0, sd0);
		sd1 = vis_fand(md1, sd1);
		sd2 = vis_fand(md2, sd2);
		sd3 = vis_fand(md3, sd3);
		dd0 = vis_fandnot(md0, dd0);
		dd1 = vis_fandnot(md1, dd1);
		dd2 = vis_fandnot(md2, dd2);
		dd3 = vis_fandnot(md3, dd3);

		dd0 = vis_for(sd0, dd0);
		dd1 = vis_for(sd1, dd1);
		dd2 = vis_for(sd2, dd2);
		dd3 = vis_for(sd3, dd3);
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U8_3_D1(
    mlib_d64 *pres,
    const mlib_f32 *sp,
    mlib_s32 n)
{
	mlib_d64 *tbl_mask = (mlib_d64 *)mlib_tbl_mask_s16;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 i;

	dd0 = dd1 = dd2 = vis_to_double_dup(0x80008000);

#pragma pipeloop(0)
	for (i = 0; i < (n / 12); i++) {
		mlib_f32 sf0, sf1, sf2;
		mlib_d64 sd0, sd1, sd2;
		mlib_d64 md0, md1, md2;
		mlib_s32 m_0, m_1, m_2;

		sf0 = sp[3 * i];
		sf1 = sp[3 * i + 1];
		sf2 = sp[3 * i + 2];

		sd0 = vis_fmul8x16al(sf0, fone);
		sd1 = vis_fmul8x16al(sf1, fone);
		sd2 = vis_fmul8x16al(sf2, fone);

		m_0 = vis_fcmplt16(dd0, sd0);
		m_1 = vis_fcmplt16(dd1, sd1);
		m_2 = vis_fcmplt16(dd2, sd2);

		md0 = tbl_mask[m_0];
		md1 = tbl_mask[m_1];
		md2 = tbl_mask[m_2];

		sd0 = vis_fand(md0, sd0);
		sd1 = vis_fand(md1, sd1);
		sd2 = vis_fand(md2, sd2);
		dd0 = vis_fandnot(md0, dd0);
		dd1 = vis_fandnot(md1, dd1);
		dd2 = vis_fandnot(md2, dd2);

		dd0 = vis_for(sd0, dd0);
		dd1 = vis_for(sd1, dd1);
		dd2 = vis_for(sd2, dd2);
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U8_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u8 *sl, *sp;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_d64 dres[4];
	mlib_s16 *sres = (mlib_s16 *)dres;
	mlib_s32 *res_k;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		mlib_s32 i, k, n = xsize;

		k = (8 - (mlib_addr)sl & 7) & 7;

		if (k > n)
			k = n;

		n -= k;
		mlib_v_ImageMaximum_U8_124_D1(dres, (mlib_d64 *)(sl + k), n);

#pragma pipeloop(0)
		for (i = 0; i < k; i++)
			PXL_OPER(res[i], sl[i]);

		sp = sl + k + 16 * (n / 16);
#pragma pipeloop(0)
		for (i = 0; i < (n & 15); i++)
			PXL_OPER(sres[i], sp[i]);

		res_k = res + k;
#pragma pipeloop(0)
		for (i = 0; i < 16; i++)
			PXL_OPER(res_k[i], sres[i]);

		sl += slb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U8_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u8 *sl, *sp;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_d64 dres[4];
	mlib_s16 *sres = (mlib_s16 *)dres;
	mlib_s32 *res_k;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		mlib_s32 i, k, n = xsize;
		mlib_u8 *send = sl + xsize;

		k = (8 - (mlib_addr)sl & 7) & 7;

		if (k > n)
			k = n;

		n -= k;
		mlib_v_ImageMaximum_U8_3_D1(dres, (mlib_f32 *)(sl + k), n);

#pragma pipeloop(0)
		for (i = 0; i < k; i++)
			PXL_OPER(res[i], sl[i]);

		sp = sl + k + 12 * (n / 12);
#pragma pipeloop(0)
		for (i = 0; sp < send; sp++, i++)
			PXL_OPER(sres[i], sp[0]);

		res_k = res + k;
#pragma pipeloop(0)
		for (i = 0; i < 12; i++)
			PXL_OPER(res_k[i], sres[i]);

		sl += slb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S16_124_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n)
{
	mlib_d64 *tbl_mask = (mlib_d64 *)mlib_tbl_mask_s16;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 i;

	dd0 = dd1 = dd2 = dd3 = vis_to_double_dup(0x80008000);

#pragma pipeloop(0)
	for (i = 0; i < (n / 16); i++) {
		mlib_d64 sd0, sd1, sd2, sd3;
		mlib_d64 md0, md1, md2, md3;
		mlib_s32 m_0, m_1, m_2, m_3;

		sd0 = sp[4 * i];
		sd1 = sp[4 * i + 1];
		sd2 = sp[4 * i + 2];
		sd3 = sp[4 * i + 3];

		m_0 = vis_fcmplt16(dd0, sd0);
		m_1 = vis_fcmplt16(dd1, sd1);
		m_2 = vis_fcmplt16(dd2, sd2);
		m_3 = vis_fcmplt16(dd3, sd3);

		md0 = tbl_mask[m_0];
		md1 = tbl_mask[m_1];
		md2 = tbl_mask[m_2];
		md3 = tbl_mask[m_3];

		sd0 = vis_fand(md0, sd0);
		sd1 = vis_fand(md1, sd1);
		sd2 = vis_fand(md2, sd2);
		sd3 = vis_fand(md3, sd3);
		dd0 = vis_fandnot(md0, dd0);
		dd1 = vis_fandnot(md1, dd1);
		dd2 = vis_fandnot(md2, dd2);
		dd3 = vis_fandnot(md3, dd3);

		dd0 = vis_for(sd0, dd0);
		dd1 = vis_for(sd1, dd1);
		dd2 = vis_for(sd2, dd2);
		dd3 = vis_for(sd3, dd3);
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S16_3_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n)
{
	mlib_d64 *tbl_mask = (mlib_d64 *)mlib_tbl_mask_s16;
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 i;

	dd0 = dd1 = dd2 = vis_to_double_dup(0x80008000);

#pragma pipeloop(0)
	for (i = 0; i < (n / 12); i++) {
		mlib_d64 sd0, sd1, sd2;
		mlib_d64 md0, md1, md2;
		mlib_s32 m_0, m_1, m_2;

		sd0 = sp[3 * i];
		sd1 = sp[3 * i + 1];
		sd2 = sp[3 * i + 2];

		m_0 = vis_fcmplt16(dd0, sd0);
		m_1 = vis_fcmplt16(dd1, sd1);
		m_2 = vis_fcmplt16(dd2, sd2);

		md0 = tbl_mask[m_0];
		md1 = tbl_mask[m_1];
		md2 = tbl_mask[m_2];

		sd0 = vis_fand(md0, sd0);
		sd1 = vis_fand(md1, sd1);
		sd2 = vis_fand(md2, sd2);
		dd0 = vis_fandnot(md0, dd0);
		dd1 = vis_fandnot(md1, dd1);
		dd2 = vis_fandnot(md2, dd2);

		dd0 = vis_for(sd0, dd0);
		dd1 = vis_for(sd1, dd1);
		dd2 = vis_for(sd2, dd2);
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S16_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s16 *sl, *sp;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_d64 dres[4];
	mlib_s16 *sres = (mlib_s16 *)dres;
	mlib_s32 *res_k;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);
	slb /= 2;
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		mlib_s32 i, k, n = xsize;

		k = ((8 - (mlib_addr)sl & 7) & 7) >> 1;

		if (k > n)
			k = n;

		n -= k;
		mlib_v_ImageMaximum_S16_124_D1(dres, (mlib_d64 *)(sl + k), n);

#pragma pipeloop(0)
		for (i = 0; i < k; i++)
			PXL_OPER(res[i], sl[i]);

		sp = sl + k + 16 * (n / 16);
#pragma pipeloop(0)
		for (i = 0; i < (n & 15); i++)
			PXL_OPER(sres[i], sp[i]);

		res_k = res + k;
#pragma pipeloop(0)
		for (i = 0; i < 16; i++)
			PXL_OPER(res_k[i], sres[i]);

		sl += slb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S16_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s16 *sl, *sp;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_d64 dres[4];
	mlib_s16 *sres = (mlib_s16 *)dres;
	mlib_s32 *res_k;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);
	slb /= 2;
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		mlib_s32 i, k, n = xsize;
		mlib_s16 *send = sl + xsize;

		k = ((8 - (mlib_addr)sl & 7) & 7) >> 1;

		if (k > n)
			k = n;

		n -= k;
		mlib_v_ImageMaximum_S16_3_D1(dres, (mlib_d64 *)(sl + k), n);

#pragma pipeloop(0)
		for (i = 0; i < k; i++)
			PXL_OPER(res[i], sl[i]);

		sp = sl + k + 12 * (n / 12);
#pragma pipeloop(0)
		for (i = 0; sp < send; sp++, i++)
			PXL_OPER(sres[i], sp[0]);

		res_k = res + k;
#pragma pipeloop(0)
		for (i = 0; i < 12; i++)
			PXL_OPER(res_k[i], sres[i]);

		sl += slb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U16_124_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n)
{
	mlib_d64 *tbl_mask = (mlib_d64 *)mlib_tbl_mask_s16;
	mlib_d64 dd0, dd1, dd2, dd3, xor_mask;
	mlib_s32 i;

	xor_mask = vis_to_double_dup(0x80008000);
	dd0 = dd1 = dd2 = dd3 = xor_mask;

#pragma pipeloop(0)
	for (i = 0; i < (n / 16); i++) {
		mlib_d64 sd0, sd1, sd2, sd3;
		mlib_d64 md0, md1, md2, md3;
		mlib_s32 m_0, m_1, m_2, m_3;

		sd0 = sp[4 * i];
		sd1 = sp[4 * i + 1];
		sd2 = sp[4 * i + 2];
		sd3 = sp[4 * i + 3];

		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);
		sd3 = vis_fxor(sd3, xor_mask);

		m_0 = vis_fcmplt16(dd0, sd0);
		m_1 = vis_fcmplt16(dd1, sd1);
		m_2 = vis_fcmplt16(dd2, sd2);
		m_3 = vis_fcmplt16(dd3, sd3);

		md0 = tbl_mask[m_0];
		md1 = tbl_mask[m_1];
		md2 = tbl_mask[m_2];
		md3 = tbl_mask[m_3];

		sd0 = vis_fand(md0, sd0);
		sd1 = vis_fand(md1, sd1);
		sd2 = vis_fand(md2, sd2);
		sd3 = vis_fand(md3, sd3);
		dd0 = vis_fandnot(md0, dd0);
		dd1 = vis_fandnot(md1, dd1);
		dd2 = vis_fandnot(md2, dd2);
		dd3 = vis_fandnot(md3, dd3);

		dd0 = vis_for(sd0, dd0);
		dd1 = vis_for(sd1, dd1);
		dd2 = vis_for(sd2, dd2);
		dd3 = vis_for(sd3, dd3);
	}

	{
		dd0 = vis_fxor(dd0, xor_mask);
		dd1 = vis_fxor(dd1, xor_mask);
		dd2 = vis_fxor(dd2, xor_mask);
		dd3 = vis_fxor(dd3, xor_mask);
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U16_3_D1(
    mlib_d64 *pres,
    const mlib_d64 *sp,
    mlib_s32 n)
{
	mlib_d64 *tbl_mask = (mlib_d64 *)mlib_tbl_mask_s16;
	mlib_d64 dd0, dd1, dd2, xor_mask;
	mlib_s32 i;

	xor_mask = vis_to_double_dup(0x80008000);
	dd0 = dd1 = dd2 = xor_mask;

#pragma pipeloop(0)
	for (i = 0; i < (n / 12); i++) {
		mlib_d64 sd0, sd1, sd2;
		mlib_d64 md0, md1, md2;
		mlib_s32 m_0, m_1, m_2;

		sd0 = sp[3 * i];
		sd1 = sp[3 * i + 1];
		sd2 = sp[3 * i + 2];
		sd0 = vis_fxor(sd0, xor_mask);
		sd1 = vis_fxor(sd1, xor_mask);
		sd2 = vis_fxor(sd2, xor_mask);

		m_0 = vis_fcmplt16(dd0, sd0);
		m_1 = vis_fcmplt16(dd1, sd1);
		m_2 = vis_fcmplt16(dd2, sd2);

		md0 = tbl_mask[m_0];
		md1 = tbl_mask[m_1];
		md2 = tbl_mask[m_2];

		sd0 = vis_fand(md0, sd0);
		sd1 = vis_fand(md1, sd1);
		sd2 = vis_fand(md2, sd2);
		dd0 = vis_fandnot(md0, dd0);
		dd1 = vis_fandnot(md1, dd1);
		dd2 = vis_fandnot(md2, dd2);

		dd0 = vis_for(sd0, dd0);
		dd1 = vis_for(sd1, dd1);
		dd2 = vis_for(sd2, dd2);
	}

	{
		dd0 = vis_fxor(dd0, xor_mask);
		dd1 = vis_fxor(dd1, xor_mask);
		dd2 = vis_fxor(dd2, xor_mask);
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U16_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u16 *sl, *sp;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_d64 dres[4];
	mlib_u16 *sres = (mlib_u16 *)dres;
	mlib_s32 *res_k;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);
	slb /= 2;
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		mlib_s32 i, k, n = xsize;

		k = ((8 - (mlib_addr)sl & 7) & 7) >> 1;

		if (k > n)
			k = n;

		n -= k;
		mlib_v_ImageMaximum_U16_124_D1(dres, (mlib_d64 *)(sl + k), n);

#pragma pipeloop(0)
		for (i = 0; i < k; i++)
			PXL_OPER_U16(res[i], sl[i]);

		sp = sl + k + 16 * (n / 16);
#pragma pipeloop(0)
		for (i = 0; i < (n & 15); i++)
			PXL_OPER_U16(sres[i], sp[i]);

		res_k = res + k;
#pragma pipeloop(0)
		for (i = 0; i < 16; i++)
			PXL_OPER_U16(res_k[i], sres[i]);

		sl += slb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_U16_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u16 *sl, *sp;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_d64 dres[4];
	mlib_u16 *sres = (mlib_u16 *)dres;
	mlib_s32 *res_k;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);
	slb /= 2;
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		mlib_s32 i, k, n = xsize;
		mlib_u16 *send = sl + xsize;

		k = ((8 - (mlib_addr)sl & 7) & 7) >> 1;

		if (k > n)
			k = n;

		n -= k;
		mlib_v_ImageMaximum_U16_3_D1(dres, (mlib_d64 *)(sl + k), n);

#pragma pipeloop(0)
		for (i = 0; i < k; i++)
			PXL_OPER_U16(res[i], sl[i]);

		sp = sl + k + 12 * (n / 12);
#pragma pipeloop(0)
		for (i = 0; sp < send; sp++, i++)
			PXL_OPER_U16(sres[i], sp[0]);

		res_k = res + k;
#pragma pipeloop(0)
		for (i = 0; i < 12; i++)
			PXL_OPER_U16(res_k[i], sres[i]);

		sl += slb;
	}
}

/* *********************************************************** */
