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

#pragma ident	"@(#)mlib_v_VideoWaveletTwoTenTrans_f.c	9.2	07/11/05 SMI"

#include <mlib_video.h>
#include <mlib_algebra.h>
#include <vis_proto.h>
#include <mlib_VideoWaveletTwoTenTrans_f.h>

/* *********************************************************** */

/* Forward transform */

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_U8(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *int_s_buffer,
	void *int_sub_buffer,
	void *odd_buf)
{
	mlib_s32 i, j, len;
	mlib_s16 *s, *pss, *d, *pdd;
	mlib_u8 *x, *pxx, *buf_s;
	mlib_d64 *dps, *dbuf_sub;
	mlib_d64 src0, src1, tmp0, tmp1, tmp2;
	mlib_d64 res, ds_2, ds_1, ds1, ds2, src;
	mlib_d64 prom, pix_odd, pix_evn, add;
	mlib_d64 mask = vis_to_double_dup(0x00ff00ff);
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_f32 *fbuf_s, *fbuf_sub, fres;
	mlib_f32 scale_6 = vis_to_float(0x80808080 >> 5);
	mlib_f32 scale_3 = vis_to_float(0x0300);
	mlib_f32 scale_22 = vis_to_float(0x1600);
	mlib_f32 fone = vis_to_float(0x100);

	vis_write_gsr(6 << 3);

	pss = ss_ptr;
	pdd = dd_ptr;
	pxx = xx_ptr;

	for (j = 0; j < xx_height; j++) {

		__mlib_VectorCopy_U8((mlib_u8 *)odd_buf + 4, pxx, xx_width);

		dps = (mlib_d64 *)odd_buf;
		dbuf_sub = int_sub_buffer;
		fbuf_s = int_s_buffer;
		len = xx_width >> 1;
		x = pxx + (xx_width & ~1);
		buf_s = (mlib_u8 *)int_s_buffer + 2 + len;
		s = pss;

		if (len > 0) {

			len -= 2;
			src0 = dps[0];
			tmp2 = vis_fmul8sux16(src0, done);
			pix_odd = vis_fand(tmp2, mask);
			pix_evn = vis_fand(src0, mask);
			add = vis_fpadd16(pix_odd, pix_evn);
			fres = vis_fpack16(add);
			fbuf_s[0] = fres;
			dbuf_sub[0] = vis_fpsub16(pix_odd, pix_evn);
			res = vis_fmul8x16al(fres, fone);
			vis_alignaddr(dps, 6);
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			vis_alignaddr(dps, 2);

			if (xx_width > 3) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, s);
				s += ss_width;
			}
		}
#pragma pipeloop(0)
		for (i = 0; i < (len >> 2); i++) {

			src0 = dps[i + 1];
			tmp2 = vis_fmul8sux16(src0, done);
			pix_odd = vis_fand(tmp2, mask);
			pix_evn = vis_fand(src0, mask);
			add = vis_fpadd16(pix_odd, pix_evn);
			fres = vis_fpack16(add);
			fbuf_s[i + 1] = fres;
			res = vis_fmul8x16al(fres, fone);
			dbuf_sub[i + 1] = vis_fpsub16(pix_odd, pix_evn);
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
		}

		if (len & 3 && len > 0) {

			src0 = dps[i + 1];
			tmp2 = vis_fmul8sux16(src0, done);
			pix_odd = vis_fand(tmp2, mask);
			pix_evn = vis_fand(src0, mask);
			add = vis_fpadd16(pix_odd, pix_evn);
			fres = vis_fpack16(add);
			fbuf_s[i + 1] = fres;
			res = vis_fmul8x16al(fres, fone);
			dbuf_sub[i + 1] = vis_fpsub16(pix_odd, pix_evn);

#pragma pipeloop(0)
			for (i = 0; i < (len & 3); i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, s);
				s += ss_width;
			}
		}

		if (xx_width & 1)
			(*buf_s++) = *s = *x;

		*((mlib_u8 *)int_s_buffer + 1) = *((mlib_u8 *)int_s_buffer + 2);

		buf_s[0] = buf_s[-1];
		buf_s[1] = buf_s[-2];
		buf_s = (mlib_u8 *)int_s_buffer + 2;
		buf_s[-1] = buf_s[0];
		buf_s[-2] = buf_s[1];

		fbuf_s = (mlib_f32 *)int_s_buffer;
		fbuf_sub = (mlib_f32 *)int_sub_buffer;

		len = xx_width >> 1;
		d = pdd;

#pragma pipeloop(0)
		for (i = 0; i < (len >> 2); i++) {

			add = vis_freg_pair(fbuf_sub[2 * i + 1],
				fbuf_sub[2 * i + 2]);
			src = vis_freg_pair(fbuf_s[i], fbuf_s[i + 1]);
			vis_alignaddr(dps, 1);
			src0 = vis_faligndata(src, src);
			vis_alignaddr(dps, 3);
			src1 = vis_faligndata(src, src);

			ds_2 = vis_fmul8x16al(vis_read_hi(src), scale_3);
			ds_1 = vis_fmul8x16al(vis_read_hi(src0), scale_22);
			ds1 = vis_fmul8x16al(vis_read_hi(src1), scale_22);
			ds2 = vis_fmul8x16al(vis_read_lo(src), scale_3);

			tmp0 = vis_fpsub16(ds_2, ds2);
			tmp1 = vis_fpsub16(ds1, ds_1);
			prom = vis_fpadd16(tmp0, tmp1);
			prom = vis_fmul8x16(scale_6, prom);

			res = vis_fpadd16(prom, add);
			vis_alignaddr(dps, 2);

			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
		}

		if (len & 3) {

			add = vis_freg_pair(fbuf_sub[2 * i + 1],
				fbuf_sub[2 * i + 2]);
			src = vis_freg_pair(fbuf_s[i], fbuf_s[i + 1]);
			vis_alignaddr(dps, 1);
			src0 = vis_faligndata(src, src);
			vis_alignaddr(dps, 3);
			src1 = vis_faligndata(src, src);

			ds_2 = vis_fmul8x16al(vis_read_hi(src), scale_3);
			ds_1 = vis_fmul8x16al(vis_read_hi(src0), scale_22);
			ds1 = vis_fmul8x16al(vis_read_hi(src1), scale_22);
			ds2 = vis_fmul8x16al(vis_read_lo(src), scale_3);

			tmp0 = vis_fpsub16(ds_2, ds2);
			tmp1 = vis_fpsub16(ds1, ds_1);
			prom = vis_fpadd16(tmp0, tmp1);
			prom = vis_fmul8x16(scale_6, prom);

			res = vis_fpadd16(prom, add);
			vis_alignaddr(dps, 2);

#pragma pipeloop(0)
			for (i = 0; i < (len & 3); i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, d);
				d += dd_width;
			}
		}

		pss++;
		pdd++;
		pxx += xx_width;
	}
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_S16(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *int_s_buffer,
	void *int_sub_buffer,
	void *evn_buf,
	void *odd_buf)
{
	mlib_s32 i, j, len;
	mlib_s16 *s, *pss, *d, *pdd, *buf_s, *x, *pxx, *buf;
	mlib_d64 *ddd0 = evn_buf;
	mlib_d64 *ddd1 = odd_buf, *dbuf_s, *dbuf_sub;
	mlib_d64 src0, src1, src2, src3;
	mlib_d64 tmp0, tmp1, tmp2, tmp3;
	mlib_d64 prom, add, res, sub;
	mlib_d64 round_mask = vis_to_double_dup(0x10001);
	mlib_d64 scale_32 = vis_to_double_dup(32);
	mlib_f32 *fbuf_s, scale = vis_to_float(0x80808080);
	mlib_f32 scale_3 = vis_to_float(0x00030003);
	mlib_f32 scale_22 = vis_to_float(0x00160016);

	pss = ss_ptr;
	pdd = dd_ptr;
	pxx = xx_ptr;

	for (j = 0; j < xx_height; j++) {

		len = xx_width >> 1;

		__mlib_VectorSplit_S16_S16C((void *)ddd0, (void *)ddd1, pxx,
			len);

		x = pxx + (xx_width & ~1);
		vis_write_gsr((10 << 3) | 2);

		dbuf_s = (mlib_d64 *)int_s_buffer + 1;
		dbuf_sub = int_sub_buffer;
		buf_s = (mlib_s16 *)dbuf_s + (xx_width >> 1);

		s = pss;

#pragma pipeloop(0)
		for (i = 0; i < (len >> 2); i++) {
			src0 = ddd0[i];
			src1 = ddd1[i];

			sub = vis_fpsub16(src0, src1);
			add = vis_fpadd16(src0, src1);
			add = vis_fandnot(round_mask, add);
			res = vis_fmul8x16(scale, add);

			dbuf_sub[i] = sub;
			dbuf_s[i] = res;

			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, s);
			s += ss_width;
		}

		if (len & 3) {
			src0 = ddd0[i];
			src1 = ddd1[i];

			sub = vis_fpsub16(src0, src1);
			add = vis_fpadd16(src0, src1);
			add = vis_fandnot(round_mask, add);
			res = vis_fmul8x16(scale, add);

			dbuf_sub[i] = sub;
			dbuf_s[i] = res;

			for (i = 0; i < (len & 3); i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, s);
				s += ss_width;
			}
		}

		if (xx_width & 1)
			(*buf_s++) = *s = *x;

		buf = (mlib_s16 *)dbuf_s;
		buf[-1] = buf[0];

		buf_s[0] = buf_s[-1];
		buf_s[1] = buf_s[-2];
		buf_s = (mlib_s16 *)dbuf_s;
		buf_s[-2] = buf_s[1];

		dbuf_sub = int_sub_buffer;
		fbuf_s = (mlib_f32 *)int_s_buffer + 1;
		d = pdd;

#pragma pipeloop(0)
		for (i = 0; i < (len >> 2); i++) {
			add = dbuf_sub[i];
			src0 = vis_freg_pair(fbuf_s[2 * i], fbuf_s[2 * i + 1]);
			src3 = vis_freg_pair(fbuf_s[2 * i + 2],
				fbuf_s[2 * i + 3]);
			tmp2 = *(mlib_d64 *)(fbuf_s + 2 * i + 1);
			tmp3 = *(mlib_d64 *)(fbuf_s + 2 * i + 3);

			src1 = vis_faligndata(src0, src3);
			src2 = vis_faligndata(tmp2, tmp3);

			tmp0 = vis_fpsub16(src0, src3);
			tmp1 = vis_fpsub16(src2, src1);

			tmp2 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
				vis_read_hi(tmp0)), vis_fmuld8ulx16(scale_22,
				vis_read_hi(tmp1)));

			tmp3 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
				vis_read_lo(tmp0)), vis_fmuld8ulx16(scale_22,
				vis_read_lo(tmp1)));

			tmp2 = vis_fpadd32(tmp2, scale_32);
			tmp3 = vis_fpadd32(tmp3, scale_32);

			prom = vis_fpackfix_pair(tmp2, tmp3);
			res = vis_fpadd16(prom, add);

			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
			res = vis_faligndata(res, res);
			vis_st_u16(res, d);
			d += dd_width;
		}

		if (len & 3) {
			add = dbuf_sub[i];
			src0 = vis_freg_pair(fbuf_s[2 * i], fbuf_s[2 * i + 1]);
			src3 = vis_freg_pair(fbuf_s[2 * i + 2],
				fbuf_s[2 * i + 3]);
			tmp2 = *(mlib_d64 *)(fbuf_s + 2 * i + 1);
			tmp3 = *(mlib_d64 *)(fbuf_s + 2 * i + 3);

			src1 = vis_faligndata(src0, src3);
			src2 = vis_faligndata(tmp2, tmp3);

			tmp0 = vis_fpsub16(src0, src3);
			tmp1 = vis_fpsub16(src2, src1);

			tmp2 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
				vis_read_hi(tmp0)), vis_fmuld8ulx16(scale_22,
				vis_read_hi(tmp1)));

			tmp3 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
				vis_read_lo(tmp0)), vis_fmuld8ulx16(scale_22,
				vis_read_lo(tmp1)));

			tmp2 = vis_fpadd32(tmp2, scale_32);
			tmp3 = vis_fpadd32(tmp3, scale_32);

			prom = vis_fpackfix_pair(tmp2, tmp3);
			res = vis_fpadd16(prom, add);

#pragma pipeloop(0)
			for (i = 0; i < (len & 3); i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, d);
				d += dd_width;
			}
		}

		pss++;
		pdd++;
		pxx += xx_width;
	}
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S16(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *l_buffer,
	void *buf_sc_22,
	void *buf_sc_3)
{
	mlib_s32 i, j, p, len;
	mlib_s16 *pxx, *x;
	mlib_s32 *pdd, *pss, *d, *s, *buf;
	mlib_s32 *bscale_22 = (mlib_s32 *)buf_sc_22 + 3;
	mlib_s32 *bscale_3 = (mlib_s32 *)buf_sc_3 + 2;
	mlib_s32 *sc_22, *sc_3;
	mlib_d64 dsc_3_0, dsc_3_1, dsc_22_0, dsc_22_1;
	mlib_d64 dp1, dp2, add, dp, src, dst, prom0, prom1;
	mlib_d64 mask = vis_to_double_dup(0xfffe01fe << 5);
	mlib_d64 add_32 = vis_to_double_dup(32);
	mlib_d64 dzero = vis_fzero();
	mlib_f32 mask_mul = vis_to_float(0x80808080 >> 5);

	len = xx_width >> 1;
	pxx = xx_ptr;
	pss = ss_ptr;
	pdd = dd_ptr;

	for (j = 0; j < xx_height; j++) {
		x = pxx;
		s = pss;
		sc_22 = bscale_22;
		sc_3 = bscale_3;
		buf = l_buffer;

#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			p = (x[0] + x[1]) >> 1;
			buf[i] = x[0] - x[1];
			(*sc_3++) = p * 3;
			(*sc_22++) = p * 22;
			*s = p;
			x += 2;
			s += ss_width;
		}

		if (xx_width & 1) {
			*s = *x;
			(*sc_3++) = *x * 3;
			(*sc_22++) = *x * 22;
		}

		bscale_22[-1] = bscale_22[0];
		bscale_3[-1] = bscale_3[0];

		d = pdd;

		sc_22[0] = sc_22[-1];
		sc_22[1] = sc_22[-2];
		sc_3[0] = sc_3[-1];
		sc_3[1] = sc_3[-2];
		sc_22 = bscale_22;
		sc_3 = bscale_3;
		sc_22[-1] = sc_22[0];
		sc_22[-2] = sc_22[1];
		sc_3[-1] = sc_3[0];
		sc_3[-2] = sc_3[1];

		vis_write_gsr(9 << 3);

#pragma pipeloop(0)
		for (i = 0; i < (len >> 1); i++) {

			add = *(mlib_d64 *)(buf + 2 * i);
			dsc_3_0 = *(mlib_d64 *)(sc_3 + 2 * i - 2);
			dsc_3_1 = *(mlib_d64 *)(sc_3 + 2 * i + 2);
			dsc_22_0 = *(mlib_d64 *)(sc_22 + 2 * i + 1);
			dsc_22_1 = *(mlib_d64 *)(sc_22 + 2 * i - 1);

			dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);
			dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);
			dp = vis_fpadd32(dp1, dp2);
			src = vis_fpadd32(dp, add_32);

			prom0 = vis_fand(mask, src);
			prom0 = vis_fmul8x16(mask_mul, prom0);
			prom1 = vis_fandnot(mask, src);
			prom1 = vis_fpack32(dzero, prom1);
			prom1 = vis_fpack32(prom1, dzero);
			dst = vis_for(prom0, prom1);
			dst = vis_fpadd32(dst, add);

			*(mlib_f32 *)d = vis_read_hi(dst);
			d += dd_width;
			*(mlib_f32 *)d = vis_read_lo(dst);
			d += dd_width;
		}

		if (len & 1) {

			add = *(mlib_d64 *)(buf + 2 * i);
			dsc_3_0 = *(mlib_d64 *)(sc_3 + 2 * i - 2);
			dsc_3_1 = *(mlib_d64 *)(sc_3 + 2 * i + 2);
			dsc_22_0 = *(mlib_d64 *)(sc_22 + 2 * i + 1);
			dsc_22_1 = *(mlib_d64 *)(sc_22 + 2 * i - 1);

			dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);
			dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);
			dp = vis_fpadd32(dp1, dp2);
			src = vis_fpadd32(dp, add_32);

			prom0 = vis_fand(mask, src);
			prom0 = vis_fmul8x16(mask_mul, prom0);
			prom1 = vis_fandnot(mask, src);
			prom1 = vis_fpack32(dzero, prom1);
			prom1 = vis_fpack32(prom1, dzero);
			dst = vis_for(prom0, prom1);

			dst = vis_fpadd32(dst, add);
			*(mlib_f32 *)d = vis_read_hi(dst);
		}

		pxx += xx_width;
		pss++;
		pdd++;
	}
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S32(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *l_buffer,
	void *buf_sc_22,
	void *buf_sc_3)
{
	mlib_s32 i, j, p, len;
	mlib_s32 *pdd, *pss, *pxx, *d, *s, *x, *buf;
	mlib_s32 *bscale_22 = (mlib_s32 *)buf_sc_22 + 3;
	mlib_s32 *bscale_3 = (mlib_s32 *)buf_sc_3 + 2;
	mlib_s32 *sc_22, *sc_3;
	mlib_d64 dsc_3_0, dsc_3_1, dsc_22_0, dsc_22_1;
	mlib_d64 dp1, dp2, add, dp, src, dst;
	mlib_d64 prom0, prom1;
	mlib_d64 mask = vis_to_double_dup(0xfffe01fe << 5);
	mlib_d64 add_32 = vis_to_double_dup(32);
	mlib_d64 dzero = vis_fzero();
	mlib_f32 mask_mul = vis_to_float(0x80808080 >> 5);

	vis_write_gsr(9 << 3);
	len = xx_width >> 1;
	pxx = xx_ptr;
	pss = ss_ptr;
	pdd = dd_ptr;

	for (j = 0; j < xx_height; j++) {
		x = pxx;
		s = pss;
		sc_22 = bscale_22;
		sc_3 = bscale_3;
		buf = l_buffer;

#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			p = (x[0] + x[1]) >> 1;
			buf[i] = x[0] - x[1];
			(*sc_3++) = p * 3;
			(*sc_22++) = p * 22;
			*s = p;
			x += 2;
			s += ss_width;
		}

		if (xx_width & 1) {
			*s = *x;
			(*sc_3++) = *x * 3;
			(*sc_22++) = *x * 22;
		}

		bscale_22[-1] = bscale_22[0];
		bscale_3[-1] = bscale_3[0];

		d = pdd;

		sc_22[0] = sc_22[-1];
		sc_22[1] = sc_22[-2];
		sc_3[0] = sc_3[-1];
		sc_3[1] = sc_3[-2];
		sc_22 = bscale_22;
		sc_3 = bscale_3;
		sc_22[-1] = sc_22[0];
		sc_22[-2] = sc_22[1];
		sc_3[-1] = sc_3[0];
		sc_3[-2] = sc_3[1];

#pragma pipeloop(0)
		for (i = 0; i < (len >> 1); i++) {

			add = *(mlib_d64 *)(buf + 2 * i);
			dsc_3_0 = *(mlib_d64 *)(sc_3 + 2 * i - 2);
			dsc_3_1 = *(mlib_d64 *)(sc_3 + 2 * i + 2);
			dsc_22_0 = *(mlib_d64 *)(sc_22 + 2 * i + 1);
			dsc_22_1 = *(mlib_d64 *)(sc_22 + 2 * i - 1);

			dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);
			dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);
			dp = vis_fpadd32(dp1, dp2);
			src = vis_fpadd32(dp, add_32);

			prom0 = vis_fand(mask, src);
			prom0 = vis_fmul8x16(mask_mul, prom0);
			prom1 = vis_fandnot(mask, src);
			prom1 = vis_fpack32(dzero, prom1);
			prom1 = vis_fpack32(prom1, dzero);
			dst = vis_for(prom0, prom1);
			dst = vis_fpadd32(dst, add);

			*(mlib_f32 *)d = vis_read_hi(dst);
			d += dd_width;
			*(mlib_f32 *)d = vis_read_lo(dst);
			d += dd_width;
		}

		if (len & 1) {

			add = *(mlib_d64 *)(buf + 2 * i);
			dsc_3_0 = *(mlib_d64 *)(sc_3 + 2 * i - 2);
			dsc_3_1 = *(mlib_d64 *)(sc_3 + 2 * i + 2);
			dsc_22_0 = *(mlib_d64 *)(sc_22 + 2 * i + 1);
			dsc_22_1 = *(mlib_d64 *)(sc_22 + 2 * i - 1);

			dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);
			dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);
			dp = vis_fpadd32(dp1, dp2);
			src = vis_fpadd32(dp, add_32);

			prom0 = vis_fand(mask, src);
			prom0 = vis_fmul8x16(mask_mul, prom0);
			prom1 = vis_fandnot(mask, src);
			prom1 = vis_fpack32(dzero, prom1);
			prom1 = vis_fpack32(prom1, dzero);
			dst = vis_for(prom0, prom1);

			dst = vis_fpadd32(dst, add);
			*(mlib_f32 *)d = vis_read_hi(dst);
		}

		pxx += xx_width;
		pss++;
		pdd++;
	}
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S16_U8(
	mlib_s16 *dst,
	const mlib_u8 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer;
	mlib_s16 *int_s_buffer, *int_sub_buffer, *evn_buf, *odd_buf;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max = sizeof (mlib_d64) * (max + 7);

	size = width * height * sizeof (mlib_s16);
	size = ((size >> 3) + 1) << 3;

	s_buffer = (void *)__mlib_malloc(size + 8 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	int_s_buffer = (void *)((mlib_u8 *)s_buffer + size);
	int_sub_buffer = (void *)((mlib_u8 *)int_s_buffer + max * 2);

	odd_buf = (void *)((mlib_u8 *)int_sub_buffer + max * 2);
	evn_buf = (void *)((mlib_u8 *)int_sub_buffer + max * 4);

	mlib_TwoTen_1D_ForwardTransform_S16_U8(s_buffer, height, d_buffer,
		height, sp, width, height, int_s_buffer, int_sub_buffer,
		odd_buf);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, int_s_buffer, int_sub_buffer,
		odd_buf, evn_buf);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, int_s_buffer, int_sub_buffer,
		odd_buf, evn_buf);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer;
	mlib_s16 *int_s_buffer, *int_sub_buffer, *evn_buf, *odd_buf;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max = sizeof (mlib_d64) * (max + 7);

	size = width * height * sizeof (mlib_s16);
	size = ((size >> 3) + 1) << 3;

	s_buffer = (void *)__mlib_malloc(size + 8 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	int_s_buffer = (void *)((mlib_u8 *)s_buffer + size);
	int_sub_buffer = (void *)((mlib_u8 *)int_s_buffer + max * 2);

	odd_buf = (void *)((mlib_u8 *)int_sub_buffer + max * 2);
	evn_buf = (void *)((mlib_u8 *)int_sub_buffer + max * 4);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(s_buffer, height, d_buffer,
		height, sp, width, height, int_s_buffer, int_sub_buffer,
		odd_buf, evn_buf);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, int_s_buffer, int_sub_buffer,
		odd_buf, evn_buf);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, int_s_buffer, int_sub_buffer,
		odd_buf, evn_buf);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S32_S16(
	mlib_s32 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	void *l_buffer, *sc_22, *sc_3;
	mlib_s32 *s_buffer, *d_buffer, *ss_buffer;
	mlib_s32 *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max = sizeof (mlib_d64) * (max + 7);

	size = width * height * sizeof (mlib_s32);
	size = ((size >> 3) + 1) << 3;

	s_buffer = (void *)__mlib_malloc(size + 8 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	l_buffer = (mlib_u8 *)s_buffer + size;
	sc_22 = (mlib_u8 *)l_buffer + 3 * max;
	sc_3 = (mlib_u8 *)l_buffer + 6 * max;

	mlib_TwoTen_1D_ForwardTransform_S32_S16(s_buffer, height, d_buffer,
		height, sp, width, height, l_buffer, sc_22, sc_3);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, l_buffer, sc_22, sc_3);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, l_buffer, sc_22, sc_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	void *l_buffer, *sc_22, *sc_3;
	mlib_s32 *s_buffer, *d_buffer, *ss_buffer;
	mlib_s32 *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max = sizeof (mlib_d64) * (max + 7);

	size = width * height * sizeof (mlib_s32);
	size = ((size >> 3) + 1) << 3;

	s_buffer = (void *)__mlib_malloc(size + 8 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	l_buffer = (mlib_u8 *)s_buffer + size;
	sc_22 = (mlib_u8 *)l_buffer + 3 * max;
	sc_3 = (mlib_u8 *)l_buffer + 6 * max;

	mlib_TwoTen_1D_ForwardTransform_S32_S32(s_buffer, height, d_buffer,
		height, sp, width, height, l_buffer, sc_22, sc_3);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, l_buffer, sc_22, sc_3);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, l_buffer, sc_22, sc_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* Inverse transform */

/* *********************************************************** */

#define	TT_1D_INVERSE(FRM_IN, FRM_OUT, FRM_BUF)                    \
	{                                                          \
	    FRM_BUF *c22, *c3, *c22_b =                            \
		    (FRM_BUF *) coeff_22 + 2, *c3_b =              \
		    (FRM_BUF *) coeff_3 + 2;                       \
	    mlib_s32 i, j, p, p1, p2;                              \
	    FRM_IN *buf, *buf_line =                               \
		    (FRM_IN *) l_buffer + 2, *s, *d, *pss, *pdd;   \
	    FRM_OUT *x, *pxx;                                      \
	                                                           \
	    pxx = xx_ptr;                                          \
	    pss = ss_ptr;                                          \
	    pdd = dd_ptr;                                          \
	    for (j = 0; j < ss_width; j++) {                       \
		s = pss;                                           \
		buf = buf_line;                                    \
		buf[-1] = *s;                                      \
		c22 = c22_b;                                       \
		c3 = c3_b;                                         \
		for (i = 0; i < ss_height; i++) {                  \
		    (*buf++) = *s;                                 \
		    (*c3++) = *s * 3;                              \
		    (*c22++) = *s * 22;                            \
		    s += ss_width;                                 \
		}                                                  \
		buf[0] = buf[-1];                                  \
		buf[1] = buf[-2];                                  \
		c3[0] = buf[-1] * 3;                               \
		c3[1] = buf[-2] * 3;                               \
		c22[0] = buf[-1] * 22;                             \
		c22[1] = buf[-2] * 22;                             \
		buf = buf_line;                                    \
		c3 = c3_b;                                         \
		c22 = c22_b;                                       \
		buf[-1] = buf[0];                                  \
		buf[-2] = buf[1];                                  \
		c3[-1] = buf[0] * 3;                               \
		c3[-2] = buf[1] * 3;                               \
		c22[-1] = buf[0] * 22;                             \
		c22[-2] = buf[1] * 22;                             \
		d = pdd;                                           \
		x = pxx;                                           \
		s = buf;                                           \
		for (i = 0; i < dd_height; i++) {                  \
		    p1 = c3[i - 2] - c3[i + 2];                    \
		    p2 = c22[i + 1] - c22[i - 1];                  \
		    p = (p1 + p2 + 32) >> 6;                       \
		    x[0] = *s + ((*d - p + 1) >> 1);               \
		    x[1] = *s - ((*d - p) >> 1);                   \
		    x += 2;                                        \
		    s++;                                           \
		    d += dd_width;                                 \
		}                                                  \
		if (dd_height != ss_height)                        \
		    *x = *s;                                       \
		pss++;                                             \
		pdd++;                                             \
		pxx += xx_width;                                   \
	    }                                                      \
	}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_U8_S16(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s16,
		mlib_u8,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_S16_S16(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s16,
		mlib_s16,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_S16_S32(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s32,
		mlib_s16,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_S32_S32(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s32,
		mlib_s32,
		mlib_s32)
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_U8_S16(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer, *l_buffer;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s16 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, max1, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max1 = max * sizeof (mlib_s32);
	max *= sizeof (mlib_s16);

	size = (width * height + 4) * sizeof (mlib_s16);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 2 * (max + max1));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max1);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max1 * 2);

	mlib_TwoTen_1D_InverseTransform_S16_S16(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S16(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_U8_S16(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer, *l_buffer;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s16 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, max1, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max1 = max * sizeof (mlib_s32);
	max *= sizeof (mlib_s16);

	size = (width * height + 4) * sizeof (mlib_s16);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 2 * (max + max1));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max1);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max1 * 2);

	mlib_TwoTen_1D_InverseTransform_S16_S16(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S16(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S16(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_S16_S32(
	mlib_s16 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s32 *s_buffer, *d_buffer, *l_buffer;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max *= sizeof (mlib_s32);

	size = (width * height + 4) * sizeof (mlib_s32);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 4 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max * 2);

	mlib_TwoTen_1D_InverseTransform_S32_S32(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S32_S32(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S32(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s32 *s_buffer, *d_buffer, *l_buffer;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max *= sizeof (mlib_s32);

	size = (width * height + 4) * sizeof (mlib_s32);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 4 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max * 2);

	mlib_TwoTen_1D_InverseTransform_S32_S32(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S32_S32(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S32_S32(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
