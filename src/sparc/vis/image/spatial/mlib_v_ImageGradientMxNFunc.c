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

#pragma ident	"@(#)mlib_v_ImageGradientMxNFunc.c	9.2	07/11/05 SMI"

#include <stdlib.h>
#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageGradientFunc.h>
#include <mlib_ImageSqrtTable.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

static mlib_d64 mlib_v_ImageMaskOffset_U16(
    const mlib_d64 *mask,
    mlib_s32 m,
    mlib_s32 n);

static mlib_status mlib_ImageKernelConvertOffset_U16(
    mlib_s32 *ikernel,
    mlib_s32 *iscale,
    const mlib_d64 *fkernel,
    mlib_d64 offset,
    mlib_s32 m,
    mlib_s32 n);

static mlib_status mlib_ImageConvKernelConvert_U8S16(
    mlib_s32 *ikernel,
    mlib_s32 *iscale,
    const mlib_d64 *fkernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_type type);

static void mlib_v_ImageGradientMxNU82D64(
    mlib_d64 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_v_ImageGradientMxNMulAdd_U8(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m);

static void mlib_v_ImageGradientMxNMedian_U8(
    mlib_u8 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_v_ImageGradientMxNU82D64_ext(
    mlib_d64 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_v_ImageGradientMxNS162D64(
    mlib_d64 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_v_ImageGradientMxNMulAdd_S16(
    mlib_d64 *hdst,
    const mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m);

static void mlib_v_ImageGradientMxNMulAdd_U16(
    mlib_d64 *hdst,
    const mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m);

static void mlib_v_ImageGradientMxNMedian_S16(
    mlib_s16 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_v_ImageGradientMxNMedian_U16(
    mlib_u16 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch,
    const mlib_d64 *offset);

static void mlib_v_ImageGradientMxNS162D64_ext(
    mlib_d64 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	SAT_S16(DST, SRC)	DST = (mlib_s32)(SRC) >> 16

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)(SRC - sat)) >> 16) ^ 0x8000

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT_S16(DST, SRC)                                       \
	if (SRC >= MLIB_S16_MAX)                                \
	    SRC = MLIB_S16_MAX;                                 \
	DST = (mlib_s32)SRC

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	if (SRC >= MLIB_U16_MAX)                                \
	    SRC = MLIB_U16_MAX;                                 \
	DST = (mlib_s32)SRC

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_d64
mlib_v_ImageMaskOffset_U16(
    const mlib_d64 *mask,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 sum = 0;

#pragma pipeloop(0)
	for (i = 0; i < m * n; i++) {
		sum += mask[i] * 32768.;
	}

	return (sum);
}

/* *********************************************************** */

mlib_status
mlib_ImageKernelConvertOffset_U16(
    mlib_s32 *ikernel,
    mlib_s32 *iscale,
    const mlib_d64 *fkernel,
    mlib_d64 offset,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 sum, norm, max, f;
	mlib_s32 isum_pos, isum_neg, isum, test;
	mlib_s32 i, scale, scale1, *tmp_ikernel, tmp_offset;
	mlib_s32 iskernel[256], *idkernel;
	mlib_s32 ismalloc;

	if (ikernel == NULL || iscale == NULL || fkernel == NULL || (m < 1) ||
	    (n < 1))
		return (MLIB_FAILURE);

	tmp_ikernel = iskernel;
	ismalloc = 0;

	if (m * n > 256) {
		idkernel = (mlib_s32 *)__mlib_malloc(m * n * sizeof (mlib_s32));

		if (idkernel == NULL)
			return (MLIB_FAILURE);
		tmp_ikernel = idkernel;
		ismalloc = 1;
	}

	offset /= 32767.;

	sum = mlib_fabs(offset);
	max = mlib_fabs(offset);

	for (i = 0; i < m * n; i++) {
		f = mlib_fabs(fkernel[i]);
		sum += f;
		max = (max > f) ? max : f;
	}

	scale1 = mlib_ilogb(max) + 1;
	scale = mlib_ilogb(sum);
	scale = (scale > scale1) ? scale : scale1;

	if (scale > 14)
		return (MLIB_FAILURE);
	scale = 15 - scale;

	if (scale > 15)
		scale = 15;

	norm = (32768 >> (15 - scale));

	for (i = 0; i < m * n; i++) {
		if (fkernel[i] > 0)
			tmp_ikernel[i] = (mlib_s32)(fkernel[i] * norm + 0.5);
		else
			tmp_ikernel[i] = (mlib_s32)(fkernel[i] * norm - 0.5);
	}

	if (offset > 0)
		tmp_offset = (mlib_s32)(offset * norm + 0.5);
	else
		tmp_offset = (mlib_s32)(offset * norm - 0.5);

	isum_pos = 0;
	isum_neg = 0;
	test = 0;

	if (tmp_offset > 0)
		isum_pos += tmp_offset;
	else
		isum_neg += tmp_offset;

	for (i = 0; i < m * n; i++) {
		if (tmp_ikernel[i] > 0)
			isum_pos += tmp_ikernel[i];
		else
			isum_neg += tmp_ikernel[i];
	}

	isum_neg = abs(isum_neg);
	isum = isum_pos + isum_neg;

	if (isum > 65535) {
		test = 1;
	}

	if (test == 0) {
		isum = abs(tmp_offset);
		for (i = 0; i < m * n; i++) {
			isum =
			    (isum >
			    abs(tmp_ikernel[i])) ? isum : abs(tmp_ikernel[i]);
		}

		if (isum > 32767)
			test = 1;
	}

	if (test == 1) {
		for (i = 0; i < m * n; i++)
			ikernel[i] = (mlib_s32)(fkernel[i] * norm) << 16;
	} else {
		for (i = 0; i < m * n; i++)
			ikernel[i] = tmp_ikernel[i] << 16;
	}

	*iscale = scale + 16;

	if (ismalloc != 0)
		__mlib_free(idkernel);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageConvKernelConvert_U8S16(
    mlib_s32 *ikernel,
    mlib_s32 *iscale,
    const mlib_d64 *fkernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_type type)
{
	mlib_d64 sum_pos, sum_neg, sum, norm, max, f;
	mlib_s32 isum_pos, isum_neg, isum, test;
	mlib_s32 i, scale, scale1;

	if (ikernel == NULL || iscale == NULL || fkernel == NULL || m < 1 ||
	    n < 1) {
		return (MLIB_FAILURE);
	}

	if (type != MLIB_SHORT) {
		sum_pos = 0;
		sum_neg = 0;

		for (i = 0; i < m * n; i++) {
			if (fkernel[i] > 0)
				sum_pos += fkernel[i];
			else
				sum_neg -= fkernel[i];
		}

		sum = (sum_pos > sum_neg) ? sum_pos : sum_neg;
		scale = mlib_ilogb(sum);
		scale++;

		scale = 31 - scale;
	} else {
/* MLIB_SHORT */
		sum = 0;
		max = 0;

		for (i = 0; i < m * n; i++) {
			f = mlib_fabs(fkernel[i]);
			sum += f;
			max = (max > f) ? max : f;
		}

		scale1 = mlib_ilogb(max) + 1;
		scale = mlib_ilogb(sum);
		scale = (scale > scale1) ? scale : scale1;
		scale++;

		scale = 32 - scale;
	}

	if (scale <= 16)
		return (MLIB_FAILURE);

	if (scale > 31)
		scale = 31;

	*iscale = scale;

/* try to round coefficients */
/* shift of coefficients is 16 */
	scale1 = 16;
	norm = (1u << (scale - scale1));

	for (i = 0; i < m * n; i++) {
		if (fkernel[i] > 0)
			ikernel[i] = (mlib_s32)(fkernel[i] * norm + 0.5);
		else
			ikernel[i] = (mlib_s32)(fkernel[i] * norm - 0.5);
	}

	isum_pos = 0;
	isum_neg = 0;
	test = 0;

	for (i = 0; i < m * n; i++) {
		if (ikernel[i] > 0)
			isum_pos += ikernel[i];
		else
			isum_neg -= ikernel[i];
	}

	if (type == MLIB_BYTE) {
		isum = (isum_pos > isum_neg) ? isum_pos : isum_neg;

		if (isum >= (1 << (31 - scale1)))
			test = 1;
	} else {
		isum = isum_pos + isum_neg;

		if (isum >= (1 << (32 - scale1)))
			test = 1;
		for (i = 0; i < m * n; i++) {
			if (abs(ikernel[i]) >= (1 << (31 - scale1)))
				test = 1;
		}
	}

/* rounding according scale1 cause overflow, truncate instead of round */
	if (test == 1) {
		for (i = 0; i < m * n; i++)
			ikernel[i] = (mlib_s32)(fkernel[i] * norm) << scale1;
	} else {
/* rounding is Ok */
		for (i = 0; i < m * n; i++)
			ikernel[i] = ikernel[i] << scale1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageGradientKernVIS(
    mlib_d64 *dhmask,
    mlib_d64 *dvmask,
    mlib_s32 *iscale,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_d64 *offset,
    mlib_type type,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 akernel[512], *ihmask = akernel, *ivmask;
	mlib_status reth, retv;
	mlib_s32 i, j;

	if (m * n > 256) {
		ihmask =
		    __mlib_malloc(2 * m * n * (sizeof (mlib_s32) +
		    sizeof (mlib_d64)));

		if (ihmask == NULL)
			return (MLIB_FAILURE);
	}

	ivmask = ihmask + m * n;

	if (type == MLIB_USHORT) {
		offset[0] = mlib_v_ImageMaskOffset_U16(hmask, m, n);
		offset[1] = mlib_v_ImageMaskOffset_U16(vmask, m, n);
		reth =
		    mlib_ImageKernelConvertOffset_U16(ihmask, iscale, hmask,
		    offset[0], m, n);
		retv =
		    mlib_ImageKernelConvertOffset_U16(ivmask, iscale + 1, vmask,
		    offset[1], m, n);
	} else {
		reth =
		    mlib_ImageConvKernelConvert_U8S16(ihmask, iscale, hmask, m,
		    n, type);
		retv =
		    mlib_ImageConvKernelConvert_U8S16(ivmask, iscale + 1, vmask,
		    m, n, type);
	}

	if (!((reth == MLIB_SUCCESS) && (retv == MLIB_SUCCESS) &&
	    (iscale[0] < 32) && (iscale[0] >= 24) && (iscale[1] < 32) &&
	    (iscale[1] >= 24))) {

		if (ihmask != akernel)
			__mlib_free(ihmask);
		return (MLIB_FAILURE);
	}

	iscale[0] -= 24;
	iscale[1] -= 24;

	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
			mlib_s32 ihval = ihmask[j * m + i] >> 16;
			mlib_s32 ivval = ivmask[j * m + i] >> 16;
			mlib_d64 hval, vval;

			ihval = (ihval & 0xFFFF) | (ihval << 16);
			hval = vis_to_double_dup(ihval);
			ivval = (ivval & 0xFFFF) | (ivval << 16);
			vval = vis_to_double_dup(ivval);
			dhmask[j * m + i] = hval;
			dvmask[j * m + i] = vval;
		}
	}

	if (ihmask != akernel)
		__mlib_free(ihmask);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNU82D64(
    mlib_d64 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch)
{
	switch (nch) {
	case 1:
		mlib_ImageCopy_na(src, (void *)dst, n);
		break;
	case 2:
		mlib_v_ImageChannelExtract_U8_21_D1(src, (void *)dst, n, 2);
		break;
	case 3:
		mlib_v_ImageChannelExtract_U8_31_D1(src, (void *)dst, n, 4);
		break;
	case 4:
		mlib_v_ImageChannelExtract_U8_41_D1(src, (void *)dst, n, 8);
		break;
	}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNMulAdd_U8(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m)
{
	mlib_s32 j;
	mlib_d64 w_zero = vis_fzero();

	for (j = 0; j < m; j += 3, hfilter += 3, vfilter += 3) {
		mlib_s32 i = 0;
		mlib_d64 hval0 = hfilter[0];
		mlib_d64 vval0 = vfilter[0];
		mlib_d64 hval1 = hfilter[1];
		mlib_d64 vval1 = vfilter[1];
		mlib_d64 hval2 = hfilter[2];
		mlib_d64 vval2 = vfilter[2];
		mlib_d64 w_src;

		vis_write_gsr(1);

		if (j == m - 2) {
			hval2 = w_zero;
			vval2 = w_zero;
		} else if (j == m - 1) {
			hval1 = w_zero;
			vval1 = w_zero;
			hval2 = w_zero;
			vval2 = w_zero;
		}
#pragma pipeloop(0)
		for (i = 0; i < n - 4; i += 8) {
			mlib_d64 w_src0 = *(mlib_d64 *)((mlib_u8 *)src + i);
			mlib_d64 w_src1 = *(mlib_d64 *)((mlib_u8 *)src + i + 8);
			mlib_d64 w_hdst0 = *(mlib_d64 *)((mlib_s16 *)hdst + i);
			mlib_d64 w_hdst1 =
			    *(mlib_d64 *)((mlib_s16 *)hdst + i + 4);
			mlib_d64 w_vdst0 = *(mlib_d64 *)((mlib_s16 *)vdst + i);
			mlib_d64 w_vdst1 =
			    *(mlib_d64 *)((mlib_s16 *)vdst + i + 4);

			w_src = vis_faligndata(w_src0, w_src1);
			w_src1 = vis_faligndata(w_src1, w_src1);
			w_hdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    hval0), w_hdst0);
			w_hdst1 =
			    vis_fpadd16(vis_fmul8x16(vis_read_lo(w_src0),
			    hval0), w_hdst1);
			w_vdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    vval0), w_vdst0);
			w_vdst1 =
			    vis_fpadd16(vis_fmul8x16(vis_read_lo(w_src0),
			    vval0), w_vdst1);

			w_src0 = vis_faligndata(w_src, w_src1);
			w_hdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src), hval1),
			    w_hdst0);
			w_hdst1 =
			    vis_fpadd16(vis_fmul8x16(vis_read_lo(w_src), hval1),
			    w_hdst1);
			w_vdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src), vval1),
			    w_vdst0);
			w_vdst1 =
			    vis_fpadd16(vis_fmul8x16(vis_read_lo(w_src), vval1),
			    w_vdst1);

			*(mlib_d64 *)((mlib_s16 *)hdst + i) =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    hval2), w_hdst0);
			*(mlib_d64 *)((mlib_s16 *)hdst + i + 4) =
			    vis_fpadd16(vis_fmul8x16(vis_read_lo(w_src0),
			    hval2), w_hdst1);
			*(mlib_d64 *)((mlib_s16 *)vdst + i) =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    vval2), w_vdst0);
			*(mlib_d64 *)((mlib_s16 *)vdst + i + 4) =
			    vis_fpadd16(vis_fmul8x16(vis_read_lo(w_src0),
			    vval2), w_vdst1);
		}

		if (i < n) {
			mlib_d64 w_src0 = *(mlib_d64 *)((mlib_u8 *)src + i);
			mlib_d64 w_src1 = *(mlib_d64 *)((mlib_u8 *)src + i + 8);
			mlib_d64 w_hdst0 = *(mlib_d64 *)((mlib_s16 *)hdst + i);
			mlib_d64 w_vdst0 = *(mlib_d64 *)((mlib_s16 *)vdst + i);

			w_src = vis_faligndata(w_src0, w_src1);
			w_src1 = vis_faligndata(w_src1, w_src1);
			w_hdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    hval0), w_hdst0);
			w_vdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    vval0), w_vdst0);

			w_src0 = vis_faligndata(w_src, w_src1);
			w_hdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src), hval1),
			    w_hdst0);
			w_vdst0 =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src), vval1),
			    w_vdst0);

			*(mlib_d64 *)((mlib_s16 *)hdst + i) =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    hval2), w_hdst0);
			*(mlib_d64 *)((mlib_s16 *)vdst + i) =
			    vis_fpadd16(vis_fmul8x16(vis_read_hi(w_src0),
			    vval2), w_vdst0);
		}

		if (m - j > 3)
			mlib_ImageCopy_na((mlib_u8 *)src + 3, (mlib_u8 *)src,
			    n + m - j);
	}
}

/* *********************************************************** */

typedef union
{
	mlib_d64 dv;
	mlib_u16 sv[4];
} mlib_union_type;

/* *********************************************************** */

#define	FAST_SQRT_U8(dp, v)                                     \
	{                                                       \
	    mlib_s32 diap;                                      \
	                                                        \
	    diap = mlib_SqrtTable_U8_Diap[(v) >> 8];            \
	    *(dp) = mlib_SqrtTable_U8[((v) >> diap) + diap];    \
	}

/* *********************************************************** */

#define	VIS_FMUL16X32(X, Y)                                         \
	vis_fpadd32(vis_fmuld8sux16(X, Y), vis_fmuld8ulx16(X, Y))

/* *********************************************************** */

void
mlib_v_ImageGradientMxNMedian_U8(
    mlib_u8 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i, count = n >> 2, left = n & 3;
	mlib_f32 v_scale0 = ((mlib_f32 *)scale)[0];
	mlib_f32 v_scale1 = ((mlib_f32 *)scale)[1];
	mlib_d64 val1 = src1[0];
	mlib_d64 val2 = src2[0];

	vis_write_gsr(15 << 3);
	src1++;
	src2++;

#pragma pipeloop(0)
	for (i = 0; i < count; i++, dst += 4 * nch, src1++, src2++) {
		mlib_union_type res;
		mlib_d64 res1, res2;
		mlib_s32 detl0, detl1, detl2, detl3;

		val1 = vis_fmul8x16(v_scale0, val1);
		val2 = vis_fmul8x16(v_scale1, val2);
		res1 = VIS_FMUL16X32(vis_read_hi(val1), vis_read_hi(val1));
		res2 = VIS_FMUL16X32(vis_read_lo(val1), vis_read_lo(val1));
		res1 =
		    vis_fpadd32(res1, VIS_FMUL16X32(vis_read_hi(val2),
		    vis_read_hi(val2)));
		res2 =
		    vis_fpadd32(res2, VIS_FMUL16X32(vis_read_lo(val2),
		    vis_read_lo(val2)));
		res1 = vis_fpackfix_pair(res1, res2);
		res.dv = vis_fpadd16(res1, res1);
		val1 = src1[0];
		val2 = src2[0];
		detl0 = res.sv[0];
		detl1 = res.sv[1];
		detl2 = res.sv[2];
		detl3 = res.sv[3];
		FAST_SQRT_U8(dst, detl0);
		FAST_SQRT_U8(dst + nch, detl1);
		FAST_SQRT_U8(dst + 2 * nch, detl2);
		FAST_SQRT_U8(dst + 3 * nch, detl3);
	}

	if (left) {
		mlib_union_type res;
		mlib_d64 res1, res2;

		val1 = vis_fmul8x16(v_scale0, val1);
		val2 = vis_fmul8x16(v_scale1, val2);
		res1 = VIS_FMUL16X32(vis_read_hi(val1), vis_read_hi(val1));
		res2 = VIS_FMUL16X32(vis_read_lo(val1), vis_read_lo(val1));
		res1 =
		    vis_fpadd32(res1, VIS_FMUL16X32(vis_read_hi(val2),
		    vis_read_hi(val2)));
		res2 =
		    vis_fpadd32(res2, VIS_FMUL16X32(vis_read_lo(val2),
		    vis_read_lo(val2)));
		res1 = vis_fpackfix_pair(res1, res2);
		res.dv = vis_fpadd16(res1, res1);
		for (i = 0; i < left; i++, dst += nch) {
			mlib_s32 detl = res.sv[i];

			FAST_SQRT_U8(dst, detl);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_U8_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 *scale,
    mlib_s32 m,
    mlib_s32 n,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k, dw2 = (dw + 3) >> 2;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	{
		mlib_s32 z0 = 1 << (8 - scale[0]);
		mlib_s32 z1 = 1 << (8 - scale[1]);

		if (!scale[0])
			scale[0] = 255;

		if (!scale[1])
			scale[1] = 255;

		z0 = (z0 << 8) | z0;
		z1 = (z1 << 8) | z1;
		scale[0] = (z0 << 16) | z0;
		scale[1] = (z1 << 16) | z1;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb)
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_u8 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (i = 0; i < dw2; i++) {
					dsh[i] = 0;
					dsv[i] = 0;
				}

				for (j1 = 0; j1 < n;
				    j1++, sa1 += slb, hmask1 += m, vmask1 +=
				    m) {
					mlib_v_ImageGradientMxNU82D64(dsa, sa1,
					    dw + m - 1, nch);
					mlib_v_ImageGradientMxNMulAdd_U8(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m);
				}

				mlib_v_ImageGradientMxNMedian_U8(da + k, dsh,
				    dsv, scale, dw, nch);
			}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNU82D64_ext(
    mlib_d64 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_u8 *dst8 = (mlib_u8 *)dst;
	mlib_s32 i, val = src[0];

	for (i = 0; i < dx_l; i++)
		dst8[i] = val;
	mlib_v_ImageGradientMxNU82D64((mlib_d64 *)(dst8 + dx_l), src,
	    n - dx_l - dx_r, nch);
	val = dst8[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst8[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_U8_ext_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 *scale,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k, dw2 = (dw + 3) >> 2;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	{
		mlib_s32 z0 = 1 << (8 - scale[0]);
		mlib_s32 z1 = 1 << (8 - scale[1]);

		if (!scale[0])
			scale[0] = 255;

		if (!scale[1])
			scale[1] = 255;

		z0 = (z0 << 8) | z0;
		z1 = (z1 << 8) | z1;
		scale[0] = (z0 << 16) | z0;
		scale[1] = (z1 << 16) | z1;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_u8 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (i = 0; i < dw2; i++) {
					dsh[i] = 0;
					dsv[i] = 0;
				}

				for (j1 = 0; j1 < n;
				    j1++, hmask1 += m, vmask1 += m) {
					mlib_v_ImageGradientMxNU82D64_ext(dsa,
					    sa1, dw + m - 1, nch, dx_l, dx_r);
					mlib_v_ImageGradientMxNMulAdd_U8(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + n - dy_b - 2))
						sa1 += slb;
				}

				mlib_v_ImageGradientMxNMedian_U8(da + k, dsh,
				    dsv, scale, dw, nch);
			}

		if ((j >= dy_t) && (j < dh + n - dy_b - 2))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNS162D64(
    mlib_d64 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s16 *dst16 = (mlib_s16 *)dst;

	switch (nch) {
	case 1:
		mlib_ImageCopy_na((mlib_u8 *)src, (mlib_u8 *)dst16, 2 * n);
		break;
	case 2:
		mlib_v_ImageChannelExtract_S16_21_D1(src, dst16, n, 2);
		break;
	case 3:
		mlib_v_ImageChannelExtract_S16_31_D1(src, dst16, n, 4);
		break;
	case 4:
		mlib_v_ImageChannelExtract_S16_41_D1(src, dst16, n, 8);
		break;
	}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNMulAdd_S16(
    mlib_d64 *hdst,
    const mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m)
{
	mlib_s32 i, j;

	for (j = 0; j < m; j++, src += !(j & 3)) {
		mlib_f32 hval = vis_read_lo(hfilter[j]);
		mlib_f32 vval = vis_read_lo(vfilter[j]);
		mlib_d64 w_src;

		vis_write_gsr(2 * (j & 3));

#pragma pipeloop(0)
		for (i = 0; i < n - 2; i += 4) {
			mlib_d64 w_src0 = *(mlib_d64 *)((mlib_s16 *)src + i);
			mlib_d64 w_src1 =
			    *(mlib_d64 *)((mlib_s16 *)src + i + 4);
			mlib_d64 w_hdst0 = *(mlib_d64 *)((mlib_s32 *)hdst + i);
			mlib_d64 w_hdst1 =
			    *(mlib_d64 *)((mlib_s32 *)hdst + i + 2);
			mlib_d64 w_vdst0 = *(mlib_d64 *)((mlib_s32 *)vdst + i);
			mlib_d64 w_vdst1 =
			    *(mlib_d64 *)((mlib_s32 *)vdst + i + 2);

			w_src = vis_faligndata(w_src0, w_src1);
			*(mlib_d64 *)((mlib_s32 *)hdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), hval),
			    w_hdst0);
			*(mlib_d64 *)((mlib_s32 *)hdst + i + 2) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_lo(w_src), hval),
			    w_hdst1);
			*(mlib_d64 *)((mlib_s32 *)vdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), vval),
			    w_vdst0);
			*(mlib_d64 *)((mlib_s32 *)vdst + i + 2) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_lo(w_src), vval),
			    w_vdst1);
		}

		if (i < n) {
			mlib_d64 w_src0 = *(mlib_d64 *)((mlib_s16 *)src + i);
			mlib_d64 w_src1 =
			    *(mlib_d64 *)((mlib_s16 *)src + i + 4);

			w_src = vis_faligndata(w_src0, w_src1);
			*(mlib_d64 *)((mlib_s32 *)hdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), hval),
			    *(mlib_d64 *)((mlib_s32 *)hdst + i));
			*(mlib_d64 *)((mlib_s32 *)vdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), vval),
			    *(mlib_d64 *)((mlib_s32 *)vdst + i));
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNMulAdd_U16(
    mlib_d64 *hdst,
    const mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m)
{
	mlib_s32 i, j;
	mlib_d64 offset = vis_to_double_dup(0x80008000);

	for (j = 0; j < m; j++, src += !(j & 3)) {
		mlib_f32 hval = vis_read_lo(hfilter[j]);
		mlib_f32 vval = vis_read_lo(vfilter[j]);
		mlib_d64 w_src, w_src_tmp;

		vis_write_gsr(2 * (j & 3));

#pragma pipeloop(0)
		for (i = 0; i < n - 2; i += 4) {
			mlib_d64 w_src0 = *(mlib_d64 *)((mlib_s16 *)src + i);
			mlib_d64 w_src1 =
			    *(mlib_d64 *)((mlib_s16 *)src + i + 4);
			mlib_d64 w_hdst0 = *(mlib_d64 *)((mlib_s32 *)hdst + i);
			mlib_d64 w_hdst1 =
			    *(mlib_d64 *)((mlib_s32 *)hdst + i + 2);
			mlib_d64 w_vdst0 = *(mlib_d64 *)((mlib_s32 *)vdst + i);
			mlib_d64 w_vdst1 =
			    *(mlib_d64 *)((mlib_s32 *)vdst + i + 2);

			w_src_tmp = vis_faligndata(w_src0, w_src1);
			w_src = vis_fxor(w_src_tmp, offset);

			*(mlib_d64 *)((mlib_s32 *)hdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), hval),
			    w_hdst0);
			*(mlib_d64 *)((mlib_s32 *)hdst + i + 2) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_lo(w_src), hval),
			    w_hdst1);
			*(mlib_d64 *)((mlib_s32 *)vdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), vval),
			    w_vdst0);
			*(mlib_d64 *)((mlib_s32 *)vdst + i + 2) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_lo(w_src), vval),
			    w_vdst1);
		}

		if (i < n) {
			mlib_d64 w_src0 = *(mlib_d64 *)((mlib_s16 *)src + i);
			mlib_d64 w_src1 =
			    *(mlib_d64 *)((mlib_s16 *)src + i + 4);

			w_src_tmp = vis_faligndata(w_src0, w_src1);
			w_src = vis_fxor(w_src_tmp, offset);

			*(mlib_d64 *)((mlib_s32 *)hdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), hval),
			    *(mlib_d64 *)((mlib_s32 *)hdst + i));
			*(mlib_d64 *)((mlib_s32 *)vdst + i) =
			    vis_fpadd32(VIS_FMUL16X32(vis_read_hi(w_src), vval),
			    *(mlib_d64 *)((mlib_s32 *)vdst + i));
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNMedian_S16(
    mlib_s16 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
	mlib_s32 *src32_1 = (mlib_s32 *)src1;
	mlib_s32 *src32_2 = (mlib_s32 *)src2;
	mlib_f32 scalef0 = (0x10000 >> (scale[0] + 8));
	mlib_f32 scalef1 = (0x10000 >> (scale[1] + 8));
	mlib_f32 dsrc1, dsrc2, detl;

#ifndef MLIB_USE_FTOI_CLAMPING
	scalef0 *= 1.f / (1 << 16);
	scalef1 *= 1.f / (1 << 16);
#endif /* MLIB_USE_FTOI_CLAMPING */

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		dsrc1 = (mlib_f32)src32_1[i] * scalef0;
		dsrc2 = (mlib_f32)src32_2[i] * scalef1;

		detl = mlib_sqrtf(dsrc1 * dsrc1 + dsrc2 * dsrc2);

		SAT_S16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_S16_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_s32 *scale,
    mlib_s32 m,
    mlib_s32 n,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_s16 *da = mlib_ImageGetData(dst);
	mlib_s16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k, dw2 = (dw + 1) >> 1;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	for (j = 0; j < dh; j++, da += dlb, sa += slb)
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_s16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (i = 0; i < dw2; i++) {
					dsh[i] = 0;
					dsv[i] = 0;
				}

				for (j1 = 0; j1 < n;
				    j1++, sa1 += slb, hmask1 += m, vmask1 +=
				    m) {
					mlib_v_ImageGradientMxNS162D64(dsa, sa1,
					    dw + m - 1, nch);
					mlib_v_ImageGradientMxNMulAdd_S16(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m);
				}

				mlib_v_ImageGradientMxNMedian_S16(da + k, dsh,
				    dsv, scale, dw, nch);
			}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNMedian_U16(
    mlib_u16 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch,
    const mlib_d64 *offset)
{
	mlib_s32 i;
	mlib_s32 *src32_1 = (mlib_s32 *)src1;
	mlib_s32 *src32_2 = (mlib_s32 *)src2;
	mlib_f32 scalef0 = (0x10000 >> (scale[0] + 8));
	mlib_f32 scalef1 = (0x10000 >> (scale[1] + 8));
	mlib_f32 offset0 = offset[0];
	mlib_f32 offset1 = offset[1];
	mlib_f32 dsrc1, dsrc2, detl;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 sat = (mlib_f32)(0x80000000);

	offset0 *= (1 << 16);
	offset1 *= (1 << 16);
#else /* MLIB_USE_FTOI_CLAMPING */
	scalef0 *= 1.f / (1 << 16);
	scalef1 *= 1.f / (1 << 16);
#endif /* MLIB_USE_FTOI_CLAMPING */

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		dsrc1 = (mlib_f32)src32_1[i] * scalef0 + offset0;
		dsrc2 = (mlib_f32)src32_2[i] * scalef1 + offset1;

		detl = mlib_sqrtf(dsrc1 * dsrc1 + dsrc2 * dsrc2);

		SAT_U16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_U16_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_s32 *scale,
    mlib_s32 m,
    mlib_s32 n,
    const mlib_u8 *acmask,
    mlib_d64 *dsa,
    const mlib_d64 *offset)
{
	mlib_d64 *dsh, *dsv;
	mlib_u16 *da = mlib_ImageGetData(dst);
	mlib_u16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k, dw2 = (dw + 1) >> 1;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	for (j = 0; j < dh; j++, da += dlb, sa += slb)
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_u16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (i = 0; i < dw2; i++) {
					dsh[i] = 0;
					dsv[i] = 0;
				}

				for (j1 = 0; j1 < n;
				    j1++, sa1 += slb, hmask1 += m, vmask1 +=
				    m) {
					mlib_v_ImageGradientMxNS162D64(dsa,
					    (mlib_s16 *)sa1, dw + m - 1, nch);
					mlib_v_ImageGradientMxNMulAdd_U16(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m);
				}

				mlib_v_ImageGradientMxNMedian_U16(da + k, dsh,
				    dsv, scale, dw, nch, offset);
			}
}

/* *********************************************************** */

void
mlib_v_ImageGradientMxNS162D64_ext(
    mlib_d64 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s16 *dst16 = (mlib_s16 *)dst;
	mlib_s32 i, val = src[0];

	for (i = 0; i < dx_l; i++)
		dst16[i] = val;
	mlib_v_ImageGradientMxNS162D64((mlib_d64 *)(dst16 + dx_l), src,
	    n - dx_l - dx_r, nch);
	val = dst16[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst16[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_S16_ext_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_s32 *scale,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_s16 *da = mlib_ImageGetData(dst);
	mlib_s16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k, dw2 = (dw + 1) >> 1;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_s16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (i = 0; i < dw2; i++) {
					dsh[i] = 0;
					dsv[i] = 0;
				}

				for (j1 = 0; j1 < n;
				    j1++, hmask1 += m, vmask1 += m) {
					mlib_v_ImageGradientMxNS162D64_ext(dsa,
					    sa1, dw + m - 1, nch, dx_l, dx_r);
					mlib_v_ImageGradientMxNMulAdd_S16(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + n - dy_b - 2))
						sa1 += slb;
				}

				mlib_v_ImageGradientMxNMedian_S16(da + k, dsh,
				    dsv, scale, dw, nch);
			}

		if ((j >= dy_t) && (j < dh + n - dy_b - 2))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_U16_ext_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_s32 *scale,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa,
    const mlib_d64 *offset)
{
	mlib_d64 *dsh, *dsv;
	mlib_u16 *da = mlib_ImageGetData(dst);
	mlib_u16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k, dw2 = (dw + 1) >> 1;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_u16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

#pragma pipeloop(0)
				for (i = 0; i < dw2; i++) {
					dsh[i] = 0;
					dsv[i] = 0;
				}

				for (j1 = 0; j1 < n;
				    j1++, hmask1 += m, vmask1 += m) {
					mlib_v_ImageGradientMxNS162D64_ext(dsa,
					    (mlib_s16 *)sa1, dw + m - 1, nch,
					    dx_l, dx_r);
					mlib_v_ImageGradientMxNMulAdd_U16(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + n - dy_b - 2))
						sa1 += slb;
				}

				mlib_v_ImageGradientMxNMedian_U16(da + k, dsh,
				    dsv, scale, dw, nch, offset);
			}

		if ((j >= dy_t) && (j < dh + n - dy_b - 2))
			sa += slb;
	}
}

/* *********************************************************** */
