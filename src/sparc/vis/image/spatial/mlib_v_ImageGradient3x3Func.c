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

#pragma ident	"@(#)mlib_v_ImageGradient3x3Func.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageGradientFunc.h>
#include <mlib_ImageSqrtTable.h>
#include <mlib_ImageChannelSplit.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

static void mlib_v_ImageGradient3x3U82D64(
    mlib_d64 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_v_ImageGradient3x3MulAdd_U8(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_d64 *sp0,
    const mlib_d64 *sp1,
    const mlib_d64 *sp2,
    const mlib_f32 *hvfilter,
    mlib_s32 dw);

static void mlib_v_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
    const mlib_d64 *src1,
    const mlib_d64 *src2,
    const mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch);

/* *********************************************************** */

#define	MLIB_D2_15	32768.0f
#define	MLIB_D2_16	65536.0f

/* *********************************************************** */

void
mlib_v_ImageGradient3x3U82D64(
    mlib_d64 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_u8 *dst8 = (mlib_u8 *)dst;

	switch (nch) {
	case 1:
		mlib_ImageCopy_na(src, dst8, n);
		break;
	case 2:
		mlib_v_ImageChannelExtract_U8_21_D1(src, dst8, n, 2);
		break;
	case 3:
		mlib_v_ImageChannelExtract_U8_31_D1(src, dst8, n, 4);
		break;
	case 4:
		mlib_v_ImageChannelExtract_U8_41_D1(src, dst8, n, 8);
		break;
	}
}

/* *********************************************************** */

void
mlib_v_ImageGradient3x3U82D64_ext(
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
	mlib_v_ImageGradient3x3U82D64((mlib_d64 *)(dst8 + dx_l), src,
	    n - dx_l - dx_r, nch);
	val = dst8[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst8[i] = val;
}

/* *********************************************************** */

#define	HV_MUL_INI(s0, k0)                                      \
	h0 = vis_fmul8x16au(vis_read_hi(s0), k0);               \
	h1 = vis_fmul8x16au(vis_read_lo(s0), k0);               \
	v0 = vis_fmul8x16al(vis_read_hi(s0), k0);               \
	v1 = vis_fmul8x16al(vis_read_lo(s0), k0)

/* *********************************************************** */

#define	HV_MUL_ADD(s0, k0)                                           \
	h0 = vis_fpadd16(vis_fmul8x16au(vis_read_hi(s0), k0), h0);   \
	h1 = vis_fpadd16(vis_fmul8x16au(vis_read_lo(s0), k0), h1);   \
	v0 = vis_fpadd16(vis_fmul8x16al(vis_read_hi(s0), k0), v0);   \
	v1 = vis_fpadd16(vis_fmul8x16al(vis_read_lo(s0), k0), v1)

/* *********************************************************** */

void
mlib_v_ImageGradient3x3MulAdd_U8(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_d64 *sp0,
    const mlib_d64 *sp1,
    const mlib_d64 *sp2,
    const mlib_f32 *hvfilter,
    mlib_s32 dw)
{
	mlib_s32 i = 0;
	mlib_f32 k0 = hvfilter[0];
	mlib_f32 k1 = hvfilter[1];
	mlib_f32 k2 = hvfilter[2];
	mlib_f32 k3 = hvfilter[3];
	mlib_f32 k4 = hvfilter[4];
	mlib_f32 k5 = hvfilter[5];
	mlib_f32 k6 = hvfilter[6];
	mlib_f32 k7 = hvfilter[7];
	mlib_f32 k8 = hvfilter[8];
	mlib_d64 h0, h1, v0, v1;
	mlib_d64 a0, a1, a2, aa;
	mlib_d64 b0, b1, b2, bb;
	mlib_d64 c0, c1, c2, cc;

	vis_write_gsr(1);

#if MLIB_VIS >= 0x200
	vis_write_bmask(0x23456789, 0);
#endif /* MLIB_VIS >= 0x200 */

	aa = sp0[0];
	bb = sp1[0];
	cc = sp2[0];

#pragma pipeloop(0)
	for (i = 0; i < (dw + 7) / 8; i++) {
		a0 = aa;
		b0 = bb;
		c0 = cc;
		aa = sp0[i + 1];
		bb = sp1[i + 1];
		cc = vis_ld_d64_nf((mlib_d64 *)sp2 + i + 1);
		a1 = vis_faligndata(a0, aa);
		b1 = vis_faligndata(b0, bb);
		c1 = vis_faligndata(c0, cc);

#if MLIB_VIS >= 0x200
		a2 = vis_bshuffle(a0, aa);
		b2 = vis_bshuffle(b0, bb);
		c2 = vis_bshuffle(c0, cc);
#else /* MLIB_VIS >= 0x200 */
		a2 = vis_faligndata(a1, vis_faligndata(aa, aa));
		b2 = vis_faligndata(b1, vis_faligndata(bb, bb));
		c2 = vis_faligndata(c1, vis_faligndata(cc, cc));
#endif /* MLIB_VIS >= 0x200 */

		HV_MUL_INI(a0, k0);
		HV_MUL_ADD(b0, k3);
		HV_MUL_ADD(c0, k6);

		HV_MUL_ADD(a1, k1);
		HV_MUL_ADD(b1, k4);
		HV_MUL_ADD(c1, k7);

		HV_MUL_ADD(a2, k2);
		HV_MUL_ADD(b2, k5);
		HV_MUL_ADD(c2, k8);

		hdst[2 * i] = h0;
		hdst[2 * i + 1] = h1;
		vdst[2 * i] = v0;
		vdst[2 * i + 1] = v1;
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
mlib_v_ImageGradient3x3Median_U8(
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
		res1 = vis_fpadd32(res1,
		    VIS_FMUL16X32(vis_read_hi(val2), vis_read_hi(val2)));
		res2 = vis_fpadd32(res2,
		    VIS_FMUL16X32(vis_read_lo(val2), vis_read_lo(val2)));
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
mlib_ImageGradient3x3_U8_ext_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 *scale,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_d64 *buff[20];
	mlib_u8 *sp[20];
	mlib_s32 b_ind = 0;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_s32 n = 3, m = 3;
	mlib_s32 sw = dw + (m - 1) - (dx_l + dx_r);
	mlib_s32 i, j, k, dw8 = (dw + 7) / 8, sw8 = (dw + (m - 1) + 7) / 8;
	mlib_f32 hvfilter[9];

	dsh = dsa;
	dsv = dsa + 2 * dw8;
	dsa += 4 * dw8;

	for (i = 0; i < 9; i++) {
		((mlib_s16 *)hvfilter)[2 * i] = *(mlib_s16 *)(hmask + i);
		((mlib_s16 *)hvfilter)[2 * i + 1] = *(mlib_s16 *)(vmask + i);
	}

	for (i = 0; i < 20; i += 5) {
		buff[i] = buff[i + 3] = dsa;
		dsa += sw8;
		buff[i + 1] = buff[i + 4] = dsa;
		dsa += sw8;
		buff[i + 2] = dsa;
		dsa += sw8;
	}

	for (i = 0; i < 20; i++) {
		sp[i] = (void *)buff[i];
	}

	if (scale[0]) {
		mlib_s32 z0 = 1 << (8 - scale[0]);

		z0 = (z0 << 8) | z0;
		scale[0] = (z0 << 16) | z0;
	} else {
		scale[0] = 255;
	}

	if (scale[1]) {
		mlib_s32 z1 = 1 << (8 - scale[1]);

		z1 = (z1 << 8) | z1;
		scale[1] = (z1 << 16) | z1;
	} else {
		scale[1] = 255;
	}

	for (j = -2; j < dh; j++) {
		mlib_u8 *sl = sa;
		mlib_u8 *sp0 = sp[b_ind];
		mlib_u8 *sp1 = sp[b_ind + 5];
		mlib_u8 *sp2 = sp[b_ind + 10];
		mlib_u8 *sp3 = sp[b_ind + 15];

		switch (nchan) {
		case 1:

			if ((((mlib_s32)sl & 7) | dx_l | dx_r) == 0) {
				sp[b_ind] = sl;
			} else {
				sp0 = sp[b_ind] = (void *)buff[b_ind];
				for (i = 0; i < dx_l; i++)
					(*sp0++) = sl[0];
				mlib_ImageCopy_na(sl, sp0, sw);
				for (i = 0; i < dx_r; i++)
					sp0[sw + i] = sp0[sw - 1];
			}

			if (b_ind > 2)
				sp[b_ind - 3] = sp[b_ind];
			if (b_ind < 2)
				sp[b_ind + 3] = sp[b_ind];
			break;

		case 2:
			for (i = 0; i < dx_l; i++) {
				(*sp0++) = sl[0];
				(*sp1++) = sl[1];
			}

			mlib_ImageChannelSplit2_U8(sp0, sp1, sl, 1, sw, 0, 0,
			    0);
			for (i = 0; i < dx_r; i++) {
				sp0[sw + i] = sp0[sw - 1];
				sp1[sw + i] = sp1[sw - 1];
			}

			break;

		case 3:
			for (i = 0; i < dx_l; i++) {
				(*sp0++) = sl[0];
				(*sp1++) = sl[1];
				(*sp2++) = sl[2];
			}

			mlib_ImageChannelSplit3_U8(sp0, sp1, sp2, sl, 1, sw, 0,
			    0, 0, 0);
			for (i = 0; i < dx_r; i++) {
				sp0[sw + i] = sp0[sw - 1];
				sp1[sw + i] = sp1[sw - 1];
				sp2[sw + i] = sp2[sw - 1];
			}

			break;

		case 4:
			for (i = 0; i < dx_l; i++) {
				(*sp0++) = sl[0];
				(*sp1++) = sl[1];
				(*sp2++) = sl[2];
				(*sp3++) = sl[3];
			}

			mlib_ImageChannelSplit4_U8(sp0, sp1, sp2, sp3, sl, 1,
			    sw, 0, 0, 0, 0, 0);
			for (i = 0; i < dx_r; i++) {
				sp0[sw + i] = sp0[sw - 1];
				sp1[sw + i] = sp1[sw - 1];
				sp2[sw + i] = sp2[sw - 1];
				sp3[sw + i] = sp3[sw - 1];
			}

			break;
		}

		if ((j + 2 >= dy_t) && (j + 2 < dh + n - dy_b - 2))
			sa += slb;

		if (j >= 0) {
			for (k = 0; k < nchan; k++)
				if (acmask[k]) {
					mlib_s32 ii = b_ind + 5 * k;

					mlib_v_ImageGradient3x3MulAdd_U8(dsh,
					    dsv, (void *)sp[ii - 2],
					    (void *)sp[ii - 1], (void *)sp[ii],
					    hvfilter, dw);
					mlib_v_ImageGradient3x3Median_U8(da + k,
					    dsh, dsv, scale, dw, nchan);
				}

			da += dlb;
		}

		b_ind++;

		if (b_ind >= 5)
			b_ind -= 3;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U8_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 *scale,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_ImageGradient3x3_U8_ext_fast(dst, src, hmask, vmask, scale, 0, 0,
	    0, 0, acmask, dsa);
}

/* *********************************************************** */
