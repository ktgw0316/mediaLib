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

#pragma ident	"@(#)mlib_v_ImageColorHSV2RGB.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorHSV2RGB - converts image from HSV to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorHSV2RGB(mlib_image       *dst,
 *                                         const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number
 *      of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      data values are mapped to next ranges
 *      0 <= H < 1
 *      0 <= S, V,P, Q,T, R,G, B <= 1
 *
 *      P  = V * (1 - S)
 *      Q  = V * (1 - S * fraction(H*6))
 *      T  = V * (1 - S * (1-fraction(H*6)))
 *
 *              | V, T,P if 0   <= H < 1/6
 *              | Q, V,P if 1/6 <= H < 2/6
 *      R, G,B = | P, V,T if 2/6 <= H < 3/6
 *              | P, Q,V if 3/6 <= H < 3/6
 *              | T, P,V if 4/6 <= H < 5/6
 *              | V, P,Q if 5/6 <= H < 1
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorHSV2RGB = __mlib_ImageColorHSV2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorHSV2RGB) mlib_ImageColorHSV2RGB
    __attribute__((weak, alias("__mlib_ImageColorHSV2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageColorHSV2RGB_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageColorHSV2RGB_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageColorHSV2RGB_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageColorHSV2RGB_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorHSV2RGB(
    mlib_image *dst,
    const mlib_image *src)
{
/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	switch (mlib_ImageGetType(dst)) {
/*  handle MLIB_BYTE data type of image  */
	case MLIB_BYTE:
		mlib_v_ImageColorHSV2RGB_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_v_ImageColorHSV2RGB_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_v_ImageColorHSV2RGB_U16(dst, src);
		break;
	case MLIB_INT:
		mlib_v_ImageColorHSV2RGB_S32(dst, src);
		break;
/*  discard any other data types  */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	dw	sw
#define	dh	sh

#define	PREPAREVARS(data_type)                                     \
	mlib_s32 sstride =                                         \
	mlib_ImageGetStride(src) / sizeof (data_type);             \
	    mlib_s32 dstride =                                     \
		mlib_ImageGetStride(dst) / sizeof (data_type);     \
	    mlib_s32 ws_ = mlib_ImageGetWidth(src);                \
	    mlib_s32 hs_ = mlib_ImageGetHeight(src);               \
	    mlib_s32 wd_ = mlib_ImageGetWidth(dst);                \
	    mlib_s32 hd_ = mlib_ImageGetHeight(dst);               \
	    mlib_s32 sw = (ws_ < wd_) ? ws_ : wd_;                 \
	    mlib_s32 sh = (hs_ < hd_) ? hs_ : hd_;                 \
	    mlib_s32 dx = ((mlib_s32)ws_ - (mlib_s32)wd_) >> 1;    \
	    mlib_s32 dy = ((mlib_s32)hs_ - (mlib_s32)hd_) >> 1;    \
	    mlib_s32 dxs = ((dx > 0) ? dx : 0);                    \
	    mlib_s32 dxd = ((dx > 0) ? 0 : -dx);                   \
	    mlib_s32 dys = ((dy > 0) ? dy : 0);                    \
	    mlib_s32 dyd = ((dy > 0) ? 0 : -dy);                   \
	    data_type *sdata =                                     \
		(data_type *) mlib_ImageGetData(src) + dxs * 3 +   \
	    sstride * dys;                                         \
	    data_type *ddata =                                     \
		(data_type *) mlib_ImageGetData(dst) + dxd * 3 +   \
	    dstride * dyd;                                         \
	    data_type *ps = sdata;                                 \
	    data_type *pd = ddata;                                 \
	    mlib_s32 i, j

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

void
mlib_v_ImageColorHSV2RGB_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294;
	mlib_u32 mid_ind = 0x861;
	mlib_u32 min_ind = 0x50a;

	PREPAREVARS(mlib_u8);

	for (j = 0; j < dh; j++) {
#pragma pipeloop(0)
		for (i = 0; i < dw; i++) {
			mlib_s32 h = ps[0];
			mlib_s32 v = ps[2];
			mlib_s32 s = ps[1];
			mlib_d64 dv = vis_ld_u8_i(ps, 2);
			mlib_d64 dp, dq;
			mlib_f32 sv, fsv;
			mlib_u32 in;

			in = (((h << 2) - h) >> 6) & 0xe;

			sv = mlib_U82F32[s] * mlib_U82F32[v] * (.5f / 255.f);
			fsv = sv - sv * mlib_HSL2RGB_F[h];
			((mlib_s64 *)&dq)[0] = fsv;
			((mlib_s64 *)&dp)[0] = sv + sv;
			dq = vis_fpsub32(dv, dq);
			dp = vis_fpsub32(dv, dp);

			pd[(max_ind >> in) & 3] = v;
			vis_st_u8_i(dp, pd, ((min_ind >> in) & 3));
			vis_st_u8_i(dq, pd, ((mid_ind >> in) & 3));

			ps += 3;
			pd += 3;
		}

		ps += sstride - 3 * dw;
		pd += dstride - 3 * dw;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorHSV2RGB_S16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 1;
	mlib_u32 mid_ind = 0x861 << 1;
	mlib_u32 min_ind = 0x50a << 1;
	mlib_f32 mask_0006 = vis_to_float(0x00060006);
	mlib_d64 mask_0300 = vis_to_double_dup(0x00030000);
	mlib_d64 unsigner = vis_to_double_dup(0x80008000);
	mlib_d64 mask_4000 = vis_to_double_dup(0x40004000);
	mlib_d64 d_twol = vis_to_double_dup(0x00020002);
	mlib_s16 *pd0;
	mlib_s16 *ps0;
	mlib_d64 *psd;

	PREPAREVARS(mlib_s16);

	ps0 = ps;
	pd0 = pd;
	vis_write_gsr(6);
	for (j = 0; j < dh; ++j) {

#pragma unroll(1)
		for (i = 0; (i < dw) && ((mlib_addr)ps & 7); i++) {
			mlib_s32 hi = ((mlib_u16 *)ps)[0] ^ 0x8000;
			mlib_s32 vi = ps[2];
			mlib_d64 s = (((mlib_u16 *)ps)[1] ^ 0x8000);
			mlib_d64 h = hi;
			mlib_d64 v = (vi + 0x8000);
			mlib_d64 sv;
			mlib_s32 p, q;
			mlib_u32 in;

			sv = s * v * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 2.0 - 1.0;

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)(sv * mlib_fabs(h));

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_s16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_s16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_s16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		psd = (mlib_d64 *)ps;

#pragma pipeloop(0)
		for (; i <= (dw - 4); i += 4) {
			mlib_d64 h, hl, maskh, h6;
			mlib_d64 s, sl, v, vl;
			mlib_d64 f, fhi_tmp, flo_tmp, sv, svf, tmp_sum;
			mlib_d64 p, q;
			mlib_d64 fhi, flo;
			mlib_d64 sda, sdb, sdc;
			mlib_u32 in0, in1, in2, in3;
			mlib_u64 sflo, sfhi;

			hl = psd[0];
			sl = psd[1];
			vl = psd[2];

			CHANNELSEPARATE_S16(hl, sl, vl, h, s, v);

			flo_tmp = vis_fmuld8ulx16(mask_0006, vis_read_lo(h));
			fhi_tmp = vis_fmuld8ulx16(mask_0006, vis_read_hi(h));
			flo = vis_fpadd32(mask_0300, flo_tmp);
			fhi = vis_fpadd32(mask_0300, fhi_tmp);

			sv = vis_fpadd16(vis_fmul8sux16(unsigner, v),
			    vis_fmul8sux16(unsigner, s));
			tmp_sum =
			    vis_fpadd16(vis_fmul8sux16(s, v), vis_fmul8ulx16(s,
			    v));
			tmp_sum = vis_fpadd16(mask_4000, tmp_sum);
			sv = vis_fpsub16(tmp_sum, sv);

			h6 = vis_fpadd16(h, vis_fpadd16(h, h));
			maskh = vis_fand(unsigner, h6);
			maskh = vis_fmul8ulx16(d_twol, maskh);
			f = vis_fpadd16(h6, h6);
			f = vis_fxor(f, maskh);

			p = vis_fpsub16(v, sv);
			sv = vis_fxor(sv, unsigner);
			f = vis_fxor(f, unsigner);

			svf =
			    vis_fpadd16(vis_fmul8sux16(unsigner, sv),
			    vis_fmul8sux16(unsigner, f));
			tmp_sum =
			    vis_fpadd16(vis_fmul8sux16(f, sv), vis_fmul8ulx16(f,
			    sv));
			tmp_sum = vis_fpadd16(mask_4000, tmp_sum);
			svf = vis_fpsub16(tmp_sum, svf);
			q = vis_fpsub16(v, svf);

			sflo = ((mlib_u64 *)&flo)[0];
			sfhi = ((mlib_u64 *)&fhi)[0];

			in0 = (sflo >> 15) & 0x1fffe;
			in1 = (sflo >> 47) & 0x1fffe;
			in2 = (sfhi >> 15) & 0x1fffe;
			in3 = (sfhi >> 47) & 0x1fffe;

			vis_st_u16_i(p, pd, ((min_ind >> in0) & 6) + 18);
			vis_st_u16_i(v, pd, ((max_ind >> in0) & 6) + 18);
			vis_st_u16_i(q, pd, ((mid_ind >> in0) & 6) + 18);
			p = vis_faligndata(p, p);
			v = vis_faligndata(v, v);
			q = vis_faligndata(q, q);
			vis_st_u16_i(p, pd, ((min_ind >> in1) & 6) + 12);
			vis_st_u16_i(v, pd, ((max_ind >> in1) & 6) + 12);
			vis_st_u16_i(q, pd, ((mid_ind >> in1) & 6) + 12);
			p = vis_faligndata(p, p);
			v = vis_faligndata(v, v);
			q = vis_faligndata(q, q);
			vis_st_u16_i(p, pd, ((min_ind >> in2) & 6) + 6);
			vis_st_u16_i(v, pd, ((max_ind >> in2) & 6) + 6);
			vis_st_u16_i(q, pd, ((mid_ind >> in2) & 6) + 6);
			p = vis_faligndata(p, p);
			v = vis_faligndata(v, v);
			q = vis_faligndata(q, q);
			vis_st_u16_i(p, pd, (min_ind >> in3) & 6);
			vis_st_u16_i(v, pd, (max_ind >> in3) & 6);
			vis_st_u16_i(q, pd, (mid_ind >> in3) & 6);

			pd += 12;
			psd += 3;
		}

		ps = (mlib_s16 *)(psd);
#pragma unroll(1)
		for (; i < dw; i++) {
			mlib_s32 hi = ((mlib_u16 *)ps)[0] ^ 0x8000;
			mlib_s32 vi = ps[2];
			mlib_d64 s = (((mlib_u16 *)ps)[1] ^ 0x8000);
			mlib_d64 h = hi;
			mlib_d64 v = (vi + 0x8000);
			mlib_d64 sv;
			mlib_s32 p, q;
			mlib_u32 in;

			sv = s * v * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 2.0 - 1.0;

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)(sv * mlib_fabs(h));

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_s16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_s16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_s16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		ps0 += sstride;
		pd0 += dstride;
		ps = ps0;
		pd = pd0;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorHSV2RGB_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 1;
	mlib_u32 mid_ind = 0x861 << 1;
	mlib_u32 min_ind = 0x50a << 1;
	mlib_f32 mask_0006 = vis_to_float(0x00060006);
	mlib_d64 mask_0300 = vis_to_double_dup(0x00030000);
	mlib_d64 unsigner = vis_to_double_dup(0x80008000);
	mlib_d64 mask_4000 = vis_to_double_dup(0x40004000);
	mlib_d64 d_twol = vis_to_double_dup(0x00020002);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_u16 *pd0;
	mlib_u16 *ps0;
	mlib_d64 *psd;

	PREPAREVARS(mlib_u16);

	ps0 = ps;
	pd0 = pd;
	vis_write_gsr(6);
	for (j = 0; j < dh; ++j) {

#pragma unroll(1)
		for (i = 0; (i < dw) && ((mlib_addr)ps & 7); i++) {
			mlib_s32 hi = ps[0];
			mlib_s32 vi = ps[2];
			mlib_d64 s = ps[1];
			mlib_d64 h = hi;
			mlib_d64 v = vi;
			mlib_d64 sv;
			mlib_s32 p, q;
			mlib_u32 in;

			sv = s * v * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 2.0 - 1.0;

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)(sv * mlib_fabs(h));

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_u16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_u16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_u16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		psd = (mlib_d64 *)ps;

#pragma pipeloop(0)
		for (; i <= (dw - 4); i += 4) {
			mlib_d64 h, hl, maskh, h6;
			mlib_d64 s, sl, v, vl;
			mlib_d64 f, fhi_tmp, flo_tmp, sv, svf, tmp_sum;
			mlib_d64 p, q;
			mlib_d64 fhi, flo;
			mlib_d64 sda, sdb, sdc;
			mlib_u32 in0, in1, in2, in3;
			mlib_u64 sflo, sfhi;

			hl = psd[0];
			sl = psd[1];
			vl = psd[2];

			CHANNELSEPARATE_S16(hl, sl, vl, h, s, v);
			h = vis_fxor(h, xor_mask);
			s = vis_fxor(s, xor_mask);
			v = vis_fxor(v, xor_mask);

			flo_tmp = vis_fmuld8ulx16(mask_0006, vis_read_lo(h));
			fhi_tmp = vis_fmuld8ulx16(mask_0006, vis_read_hi(h));
			flo = vis_fpadd32(mask_0300, flo_tmp);
			fhi = vis_fpadd32(mask_0300, fhi_tmp);

			sv = vis_fpadd16(vis_fmul8sux16(unsigner, v),
			    vis_fmul8sux16(unsigner, s));
			tmp_sum =
			    vis_fpadd16(vis_fmul8sux16(s, v), vis_fmul8ulx16(s,
			    v));
			tmp_sum = vis_fpadd16(mask_4000, tmp_sum);
			sv = vis_fpsub16(tmp_sum, sv);

			h6 = vis_fpadd16(h, vis_fpadd16(h, h));
			maskh = vis_fand(unsigner, h6);
			maskh = vis_fmul8ulx16(d_twol, maskh);
			f = vis_fpadd16(h6, h6);
			f = vis_fxor(f, maskh);

			p = vis_fpsub16(v, sv);
			sv = vis_fxor(sv, unsigner);
			f = vis_fxor(f, unsigner);

			svf =
			    vis_fpadd16(vis_fmul8sux16(unsigner, sv),
			    vis_fmul8sux16(unsigner, f));
			tmp_sum =
			    vis_fpadd16(vis_fmul8sux16(f, sv), vis_fmul8ulx16(f,
			    sv));
			tmp_sum = vis_fpadd16(mask_4000, tmp_sum);
			svf = vis_fpsub16(tmp_sum, svf);
			q = vis_fpsub16(v, svf);

			sflo = ((mlib_u64 *)&flo)[0];
			sfhi = ((mlib_u64 *)&fhi)[0];

			in0 = (sflo >> 15) & 0x1fffe;
			in1 = (sflo >> 47) & 0x1fffe;
			in2 = (sfhi >> 15) & 0x1fffe;
			in3 = (sfhi >> 47) & 0x1fffe;

			p = vis_fxor(p, xor_mask);
			v = vis_fxor(v, xor_mask);
			q = vis_fxor(q, xor_mask);

			vis_st_u16_i(p, pd, ((min_ind >> in0) & 6) + 18);
			vis_st_u16_i(v, pd, ((max_ind >> in0) & 6) + 18);
			vis_st_u16_i(q, pd, ((mid_ind >> in0) & 6) + 18);
			p = vis_faligndata(p, p);
			v = vis_faligndata(v, v);
			q = vis_faligndata(q, q);
			vis_st_u16_i(p, pd, ((min_ind >> in1) & 6) + 12);
			vis_st_u16_i(v, pd, ((max_ind >> in1) & 6) + 12);
			vis_st_u16_i(q, pd, ((mid_ind >> in1) & 6) + 12);
			p = vis_faligndata(p, p);
			v = vis_faligndata(v, v);
			q = vis_faligndata(q, q);
			vis_st_u16_i(p, pd, ((min_ind >> in2) & 6) + 6);
			vis_st_u16_i(v, pd, ((max_ind >> in2) & 6) + 6);
			vis_st_u16_i(q, pd, ((mid_ind >> in2) & 6) + 6);
			p = vis_faligndata(p, p);
			v = vis_faligndata(v, v);
			q = vis_faligndata(q, q);
			vis_st_u16_i(p, pd, (min_ind >> in3) & 6);
			vis_st_u16_i(v, pd, (max_ind >> in3) & 6);
			vis_st_u16_i(q, pd, (mid_ind >> in3) & 6);

			pd += 12;
			psd += 3;
		}

		ps = (mlib_u16 *)(psd);
#pragma unroll(1)
		for (; i < dw; i++) {
			mlib_s32 hi = ps[0];
			mlib_s32 vi = ps[2];
			mlib_d64 s = ps[1];
			mlib_d64 h = hi;
			mlib_d64 v = vi;
			mlib_d64 sv;
			mlib_s32 p, q;
			mlib_u32 in;

			sv = s * v * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 2.0 - 1.0;

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)(sv * mlib_fabs(h));

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_u16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_u16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_u16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		ps0 += sstride;
		pd0 += dstride;
		ps = ps0;
		pd = pd0;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorHSV2RGB_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 2;
	mlib_u32 mid_ind = 0x861 << 2;
	mlib_u32 min_ind = 0x50a << 2;

	PREPAREVARS(mlib_s32);

	for (j = 0; j < dh; ++j) {
#pragma pipeloop(0)

		for (i = 0; i < dw; ++i) {
			mlib_d64 h = (mlib_d64)ps[0] - (mlib_d64)MLIB_S32_MIN;
			mlib_d64 s = (mlib_d64)ps[1] - (mlib_d64)MLIB_S32_MIN;
			mlib_d64 v0 = (mlib_d64)ps[2];
			mlib_d64 v = v0 - (mlib_d64)MLIB_S32_MIN;
			mlib_d64 p, q;
			mlib_d64 sv, fh2 = h * (3. / (MLIB_U32_MAX + 1.));
			mlib_d64 f, fh = fh2 * 2.0;
			mlib_s32 vi = ps[2];
			mlib_u32 in;

			fh2 = fh2 - (mlib_s32)fh2;
			f = fh2 * 2.0 - 1.0;
			sv = s * v * (1. / (mlib_d64)MLIB_U32_MAX);

			p = v0 - sv;
			q = v0 - sv * mlib_fabs(f);

			in = ((mlib_s32)fh) << 1;

			*(mlib_s32 *)((mlib_u8 *)pd + ((max_ind >> in) & 0xc)) =
			    vi;
			*(mlib_s32 *)((mlib_u8 *)pd + ((min_ind >> in) & 0xc)) =
			    p;
			*(mlib_s32 *)((mlib_u8 *)pd + ((mid_ind >> in) & 0xc)) =
			    q;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}

/* *********************************************************** */
