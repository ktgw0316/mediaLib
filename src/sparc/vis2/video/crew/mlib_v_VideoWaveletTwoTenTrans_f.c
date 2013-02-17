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

static void mlib_TwoTen_1D_InverseTransform_U8_S16(
	void *x,
	mlib_s32 x_w,
	void *s,
	mlib_s32 s_w,
	mlib_s32 s_h,
	void *d,
	mlib_s32 d_w,
	mlib_s32 d_h,
	void *l_buf,
	void *c_22,
	void *c_3);
static void mlib_TwoTen_1D_InverseTransform_S16_S16(
	void *x,
	mlib_s32 x_w,
	void *s,
	mlib_s32 s_w,
	mlib_s32 s_h,
	void *d,
	mlib_s32 d_w,
	mlib_s32 d_h,
	void *l_buf,
	void *c_22,
	void *c_3);
static void mlib_TwoTen_1D_InverseTransform_S16_S32(
	void *x,
	mlib_s32 x_w,
	void *s,
	mlib_s32 s_w,
	mlib_s32 s_h,
	void *d,
	mlib_s32 d_w,
	mlib_s32 d_h,
	void *l_buf,
	void *c_22,
	void *c_3);
static void mlib_TwoTen_1D_InverseTransform_S32_S32(
	void *x,
	mlib_s32 x_w,
	void *s,
	mlib_s32 s_w,
	mlib_s32 s_h,
	void *d,
	mlib_s32 d_w,
	mlib_s32 d_h,
	void *l_buf,
	void *c_22,
	void *c_3);

#pragma no_inline(mlib_TwoTen_1D_InverseTransform_U8_S16, \
mlib_TwoTen_1D_InverseTransform_S16_S16)
#pragma no_inline(mlib_TwoTen_1D_InverseTransform_S16_S32, \
mlib_TwoTen_1D_InverseTransform_S32_S32)

/* *********************************************************** */

/* Forward transform */

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_U8(
	void *ss_ptr,
	mlib_s32 ss_stride,
	void *dd_ptr,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height)
{
	mlib_u8 *x, *pxx;
	mlib_s32 i, j, k, len, off, bmask0;
	mlib_s16 *s, *pss, *d, *pdd;
	mlib_d64 tmp0, tmp1, tmp2, src0;
	mlib_d64 prom, add, res, *ds, *dd;
	mlib_d64 ld0, ld1, *d_pxx;
	mlib_d64 buf_s0, buf_s1, buf_s2, buf_tmp;
	mlib_d64 mask = vis_to_double_dup(0x00ff00ff);
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_f32 fone = vis_to_float(0x100);
	mlib_d64 pix_odd, pix_evn;
	mlib_f32 scale_6 = vis_to_float(0x80808080 >> 5);
	mlib_f32 scale_3 = vis_to_float(0x0300);
	mlib_f32 scale_22 = vis_to_float(0x1600), fres1;
	mlib_d64 ds_2, ds_1, ds1, ds2, dsub0, dsub1;

	pss = ss_ptr;
	pdd = dd_ptr;
	pxx = xx_ptr;

	len = xx_width >> 1;

	for (j = 0; j < xx_height; j++) {
		if (len > 3) {
			x = pxx + (xx_width & ~1);
			vis_write_gsr((6 << 3) | 2);

			off = (mlib_addr)pxx & 7;
			d_pxx = (mlib_d64 *)((mlib_u8 *)pxx - off);
			bmask0 = off * 0x11111111 + 0x01234567;

			ds = (mlib_d64 *)pss;
			ld0 = (*d_pxx++);

			vis_write_bmask(bmask0, 0);
			ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
			src0 = vis_bshuffle(ld0, ld1);
			ld0 = ld1;

			tmp2 = vis_fmul8sux16(src0, done);
			pix_odd = vis_fand(tmp2, mask);
			pix_evn = vis_fand(src0, mask);
			add = vis_fpadd16(pix_odd, pix_evn);
			dsub0 = vis_fpsub16(pix_odd, pix_evn);
			fres1 = vis_fpack16(add);
			(*ds++) = vis_fmul8x16al(fres1, fone);

			buf_s2 = vis_freg_pair(fres1, vis_fzero());
			vis_write_bmask(0xffffff10, 0);
			buf_s1 = vis_bshuffle(buf_s2, vis_fzero());

			dd = (mlib_d64 *)pdd;

			for (i = 1; i < (len >> 2); i++) {
				vis_write_bmask(bmask0, 0);
				ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
				src0 = vis_bshuffle(ld0, ld1);
				ld0 = ld1;

				tmp2 = vis_fmul8sux16(src0, done);
				pix_odd = vis_fand(tmp2, mask);
				pix_evn = vis_fand(src0, mask);
				add = vis_fpadd16(pix_odd, pix_evn);
				dsub1 = vis_fpsub16(pix_odd, pix_evn);
				fres1 = vis_fpack16(add);
				(*ds++) = vis_fmul8x16al(fres1, fone);

				buf_s1 = vis_freg_pair(vis_read_lo(buf_s1),
					vis_read_hi(buf_s2));
				buf_s2 = vis_freg_pair(fres1, vis_fzero());

				buf_s0 = vis_faligndata(buf_s1, buf_s2);

				vis_write_bmask(0x12343456, 0);
				src0 = vis_bshuffle(buf_s0, buf_s0);

				ds_2 = vis_fmul8x16al(vis_read_hi(buf_s0),
					scale_3);
				ds2 = vis_fmul8x16al(vis_read_lo(buf_s0),
					scale_3);
				ds_1 = vis_fmul8x16al(vis_read_hi(src0),
					scale_22);
				ds1 = vis_fmul8x16al(vis_read_lo(src0),
					scale_22);

				tmp0 = vis_fpsub16(ds_2, ds2);
				tmp1 = vis_fpsub16(ds1, ds_1);
				prom = vis_fpadd16(tmp0, tmp1);
				prom = vis_fmul8x16(scale_6, prom);
				res = vis_fpadd16(prom, dsub0);
				(*dd++) = res;
				dsub0 = dsub1;
			}

			s = (mlib_s16 *)ds;

			if (len & 3) {
				vis_write_bmask(bmask0, 0);
				ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
				src0 = vis_bshuffle(ld0, ld1);

				tmp2 = vis_fmul8sux16(src0, done);
				pix_odd = vis_fand(tmp2, mask);
				pix_evn = vis_fand(src0, mask);
				add = vis_fpadd16(pix_odd, pix_evn);
				dsub1 = vis_fpsub16(pix_odd, pix_evn);
				fres1 = vis_fpack16(add);
				res = vis_fmul8x16al(fres1, fone);

				for (k = 0; k < (len & 3); k++) {
					res = vis_faligndata(res, res);
					vis_st_u16(res, s);
					s++;
				}

				buf_s1 = vis_freg_pair(vis_read_lo(buf_s1),
					vis_read_hi(buf_s2));
				buf_s2 = vis_freg_pair(fres1, vis_fzero());

				if (xx_width & 1) {
					*s = *x;
					buf_tmp = vis_ld_u8(x);

					if ((len & 3) == 1) {
						vis_write_bmask(0x0ff08888, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
					} else
					if ((len & 3) == 2) {
						vis_write_bmask(0x01ff1888, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
					} else
					if ((len & 3) == 3) {
						vis_write_bmask(0x012ff288, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
					}

				} else {

					if ((len & 3) == 1) {
						vis_write_bmask(0x887fffff, 0);
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
					} else
					if ((len & 3) == 2) {
						vis_write_bmask(0x89980000, 0);
						buf_s2 = vis_bshuffle(vis_fzero
							(), buf_s2);
					} else
					if ((len & 3) == 3) {
						vis_write_bmask(0x89aa9000, 0);
						buf_s2 = vis_bshuffle(vis_fzero
							(), buf_s2);
					}
				}

			} else {
				buf_s1 = vis_freg_pair(vis_read_lo(buf_s1),
					vis_read_hi(buf_s2));

				if (xx_width & 1) {
					*s = *x;
					buf_tmp = vis_ld_u8(x);
					vis_write_bmask(0xff788888, 0);
					buf_s2 = vis_bshuffle(buf_s1, buf_tmp);

				} else {
					vis_write_bmask(0x32ffffff, 0);
					buf_s2 = vis_bshuffle(buf_s2,
						vis_fzero());
				}
			}

			buf_s0 = vis_faligndata(buf_s1, buf_s2);

			vis_write_bmask(0x12343456, 0);
			src0 = vis_bshuffle(buf_s0, buf_s0);

			ds_2 = vis_fmul8x16al(vis_read_hi(buf_s0), scale_3);
			ds2 = vis_fmul8x16al(vis_read_lo(buf_s0), scale_3);
			ds_1 = vis_fmul8x16al(vis_read_hi(src0), scale_22);
			ds1 = vis_fmul8x16al(vis_read_lo(src0), scale_22);

			tmp0 = vis_fpsub16(ds_2, ds2);
			tmp1 = vis_fpsub16(ds1, ds_1);
			prom = vis_fpadd16(tmp0, tmp1);
			prom = vis_fmul8x16(scale_6, prom);
			res = vis_fpadd16(prom, dsub0);
			(*dd++) = res;
			dsub0 = dsub1;
			d = (mlib_s16 *)dd;

			if (len & 3) {
				buf_s1 = vis_freg_pair(vis_read_lo(buf_s1),
					vis_read_hi(buf_s2));
				buf_s2 = vis_freg_pair(vis_read_lo(buf_s2),
					vis_fzero());
				buf_s0 = vis_faligndata(buf_s1, buf_s2);

				vis_write_bmask(0x12343456, 0);
				src0 = vis_bshuffle(buf_s0, buf_s0);

				ds_2 = vis_fmul8x16al(vis_read_hi(buf_s0),
					scale_3);
				ds2 = vis_fmul8x16al(vis_read_lo(buf_s0),
					scale_3);
				ds_1 = vis_fmul8x16al(vis_read_hi(src0),
					scale_22);
				ds1 = vis_fmul8x16al(vis_read_lo(src0),
					scale_22);

				tmp0 = vis_fpsub16(ds_2, ds2);
				tmp1 = vis_fpsub16(ds1, ds_1);
				prom = vis_fpadd16(tmp0, tmp1);
				prom = vis_fmul8x16(scale_6, prom);
				res = vis_fpadd16(prom, dsub0);

				for (k = 0; k < (len & 3); k++) {
					res = vis_faligndata(res, res);
					vis_st_u16(res, d);
					d++;
				}
			}

			pss += ss_stride;
			pdd += ss_stride;
			pxx += xx_width;

		} else {
			x = pxx + (xx_width & ~1);
			vis_write_gsr((6 << 3) | 2);

			off = (mlib_addr)pxx & 7;
			d_pxx = (mlib_d64 *)((mlib_u8 *)pxx - off);
			bmask0 = off * 0x11111111 + 0x01234567;

			s = pss;
			ld0 = vis_ld_d64_nf(d_pxx); d_pxx++;

			vis_write_bmask(bmask0, 0);
			ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
			src0 = vis_bshuffle(ld0, ld1);

			tmp2 = vis_fmul8sux16(src0, done);
			pix_odd = vis_fand(tmp2, mask);
			pix_evn = vis_fand(src0, mask);
			add = vis_fpadd16(pix_odd, pix_evn);
			dsub0 = vis_fpsub16(pix_odd, pix_evn);
			fres1 = vis_fpack16(add);
			res = vis_fmul8x16al(fres1, fone);

			buf_s1 = res;

			for (i = 0; i < len; i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, s);
				s++;
			}

			if (xx_width & 1) {
				*s = *x;
				buf_tmp = vis_ld_u8(x);

				if (len == 1) {
					vis_write_bmask(0xf11ff100, 0);
					buf_s0 = vis_bshuffle(buf_s1, buf_tmp);
				} else
				if (len == 2) {
					vis_write_bmask(0x3113ff30, 0);
					buf_s0 = vis_bshuffle(buf_s1, buf_tmp);
				} else
				if (len == 3) {
					vis_write_bmask(0x31135ff5, 0);
					buf_s0 = vis_bshuffle(buf_s1, buf_tmp);
				}

			} else {

				if (len == 1) {
					vis_write_bmask(0x11111fff, 0);
					buf_s0 = vis_bshuffle(buf_s1,
						vis_fzero());
				} else
				if (len == 2) {
					vis_write_bmask(0x311331ff, 0);
					buf_s0 = vis_bshuffle(buf_s1,
						vis_fzero());
				} else
				if (len == 3) {
					vis_write_bmask(0x3113553f, 0);
					buf_s0 = vis_bshuffle(buf_s1,
						vis_fzero());
				}
			}

			d = pdd;
			vis_write_bmask(0x12343456, 0);
			src0 = vis_bshuffle(buf_s0, buf_s0);

			ds_2 = vis_fmul8x16al(vis_read_hi(buf_s0), scale_3);
			ds2 = vis_fmul8x16al(vis_read_lo(buf_s0), scale_3);
			ds_1 = vis_fmul8x16al(vis_read_hi(src0), scale_22);
			ds1 = vis_fmul8x16al(vis_read_lo(src0), scale_22);

			tmp0 = vis_fpsub16(ds_2, ds2);
			tmp1 = vis_fpsub16(ds1, ds_1);
			prom = vis_fpadd16(tmp0, tmp1);
			prom = vis_fmul8x16(scale_6, prom);

			res = vis_fpadd16(prom, dsub0);

			for (i = 0; i < len; i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, d);
				d++;
			}

			pss += ss_stride;
			pdd += ss_stride;
			pxx += xx_width;
		}
	}
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_S16_src_na(
	void *ss_ptr,
	mlib_s32 ss_stride,
	void *dd_ptr,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height)
{
	mlib_s32 i, j, k, len;
	mlib_s16 *s, *pss, *d, *pdd, *x, *pxx;
	mlib_d64 src0, src1, src2, src3;
	mlib_d64 tmp0, tmp1, tmp2, tmp3;
	mlib_d64 prom, add, add0, res, sub;
	mlib_d64 round_mask = vis_to_double_dup(0x10001);
	mlib_d64 scale_32 = vis_to_double_dup(32);
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_f32 scale_3 = vis_to_float(0x00030003);
	mlib_f32 scale_22 = vis_to_float(0x00160016);
	mlib_d64 ld0, ld1, ld2, sr0, sr1, *d_pxx;
	mlib_d64 buf_s0, buf_s1, buf_s2, buf_s3 = 0, buf_tmp;
	mlib_s32 off, bmask0;
	mlib_d64 *ds, *dd;

	pss = ss_ptr;
	pdd = dd_ptr;
	pxx = xx_ptr;

	len = xx_width >> 1;

	for (j = 0; j < xx_height; j++) {
		if (len > 3) {
			x = pxx + (xx_width & ~1);
			vis_write_gsr((10 << 3) | 2);

			off = (mlib_addr)pxx & 7;
			d_pxx = (mlib_d64 *)((mlib_u8 *)pxx - off);
			bmask0 = off * 0x11111111 + 0x01234567;

			ds = (mlib_d64 *)pss;
			ld0 = (*d_pxx++);

			vis_write_bmask(bmask0, 0);
			ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
			sr0 = vis_bshuffle(ld0, ld1);
			ld2 = vis_ld_d64_nf(d_pxx); d_pxx++;
			sr1 = vis_bshuffle(ld1, ld2);
			ld0 = ld2;
			vis_write_bmask(0x014589cd, 0);
			src0 = vis_bshuffle(sr0, sr1);
			src1 = vis_bshuffle(vis_faligndata(sr0, sr0),
				vis_faligndata(sr1, sr1));

			add0 = vis_fpsub16(src0, src1);
			add = vis_fpadd16(src0, src1);
			add = vis_fandnot(round_mask, add);
			buf_s1 = vis_fmul8x16(scale, add);

			(*ds++) = buf_s1;

			vis_write_bmask(0x88882301, 0);
			buf_s0 = vis_bshuffle(buf_s1, vis_fzero());

			src3 = vis_freg_pair(vis_read_lo(buf_s0),
				vis_read_hi(buf_s1));
			dd = (mlib_d64 *)pdd;

			for (i = 1; i < (len >> 2); i++) {
				vis_write_bmask(bmask0, 0);
				ld1 = (*d_pxx++);
				sr0 = vis_bshuffle(ld0, ld1);
				ld2 = vis_ld_d64_nf(d_pxx); d_pxx++;
				sr1 = vis_bshuffle(ld1, ld2);

				vis_write_bmask(0x014589cd, 0);
				src0 = vis_bshuffle(sr0, sr1);
				src1 = vis_bshuffle(vis_faligndata(sr0, sr0),
					vis_faligndata(sr1, sr1));

				sub = vis_fpsub16(src0, src1);
				add = vis_fpadd16(src0, src1);
				add = vis_fandnot(round_mask, add);
				buf_s2 = vis_fmul8x16(scale, add);

				(*ds++) = buf_s2;

				src0 = src3;
				src3 = vis_freg_pair(vis_read_lo(buf_s1),
					vis_read_hi(buf_s2));
				src1 = vis_faligndata(src0, src3);
				src2 = vis_faligndata(buf_s1, buf_s2);
				buf_s1 = buf_s2;

				tmp0 = vis_fpsub16(src0, src3);
				tmp1 = vis_fpsub16(src2, src1);

				tmp2 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_hi(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_hi(tmp1)));

				tmp3 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_lo(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_lo(tmp1)));

				tmp2 = vis_fpadd32(tmp2, scale_32);
				tmp3 = vis_fpadd32(tmp3, scale_32);

				prom = vis_fpackfix_pair(tmp2, tmp3);

				ld0 = ld2;
				(*dd++) = vis_fpadd16(prom, add0);
				add0 = sub;
			}

			s = (mlib_s16 *)ds;

			if (len & 3) {
				vis_write_bmask(bmask0, 0);
				ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
				sr0 = vis_bshuffle(ld0, ld1);
				ld2 = vis_ld_d64_nf(d_pxx); d_pxx++;
				sr1 = vis_bshuffle(ld1, ld2);

				vis_write_bmask(0x014589cd, 0);
				src0 = vis_bshuffle(sr0, sr1);
				src1 = vis_bshuffle(vis_faligndata(sr0, sr0),
					vis_faligndata(sr1, sr1));

				sub = vis_fpsub16(src0, src1);
				add = vis_fpadd16(src0, src1);
				add = vis_fandnot(round_mask, add);
				res = vis_fmul8x16(scale, add);

				buf_s2 = res;

				for (k = 0; k < (len & 3); k++) {
					res = vis_faligndata(res, res);
					vis_st_u16(res, s);
					s++;
				}

				if (xx_width & 1) {
					*s = *x;
					buf_tmp = vis_ld_u16(x);

					if ((len & 3) == 1) {
						vis_write_bmask(0x01efef01, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
					} else
/* besides needs add one pixel */
					if ((len & 3) == 2) {
						vis_write_bmask(0x0123efef, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
						vis_write_bmask(0x23abcdef, 0);
						buf_s3 = vis_bshuffle(buf_s2,
							buf_s3);
					} else
					if ((len & 3) == 3) {
/* besides needs add two pixels */
						vis_write_bmask(0x012345ef, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
						vis_write_bmask(0x6745cdef, 0);
						buf_s3 = vis_bshuffle(buf_s2,
							buf_s3);
					}

				} else {

					if ((len & 3) == 1) {
						vis_write_bmask(0x898967ef, 0);
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
					} else
					if ((len & 3) == 2) {
						vis_write_bmask(0x89abab89, 0);
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
					} else
					if ((len & 3) == 3) {
						vis_write_bmask(0x89abcdcd, 0);
/* besides needs add one pixel */
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
						vis_write_bmask(0x23abcdef, 0);
						buf_s3 = vis_bshuffle(buf_s2,
							buf_s3);
					}
				}

			} else {

				if (xx_width & 1) {
					*s = *x;
					buf_s2 = vis_ld_u16(x);
					vis_write_bmask(0xefef67cd, 0);
					buf_s2 = vis_bshuffle(buf_s1, buf_s2);

				} else {

					vis_write_bmask(0x67458888, 0);
					buf_s2 = vis_bshuffle(buf_s1,
						vis_fzero());
				}
			}

			src0 = src3;
			src3 = vis_freg_pair(vis_read_lo(buf_s1),
				vis_read_hi(buf_s2));
			src1 = vis_faligndata(src0, src3);
			src2 = vis_faligndata(buf_s1, buf_s2);
			buf_s1 = buf_s2;

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
			res = vis_fpadd16(prom, add0);

			(*dd++) = res;
			add0 = sub;
			d = (mlib_s16 *)dd;

			if (len & 3) {
				buf_s2 = buf_s3;
				src0 = src3;
				src3 = vis_freg_pair(vis_read_lo(buf_s1),
					vis_read_hi(buf_s2));
				src1 = vis_faligndata(src0, src3);
				src2 = vis_faligndata(buf_s1, buf_s2);

				tmp0 = vis_fpsub16(src0, src3);
				tmp1 = vis_fpsub16(src2, src1);

				tmp2 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_hi(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_hi(tmp1)));

				tmp3 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_lo(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_lo(tmp1)));

				tmp2 = vis_fpadd32(tmp2, scale_32);
				tmp3 = vis_fpadd32(tmp3, scale_32);

				prom = vis_fpackfix_pair(tmp2, tmp3);
				res = vis_fpadd16(prom, add0);

				for (k = 0; k < (len & 3); k++) {
					res = vis_faligndata(res, res);
					vis_st_u16(res, d);
					d++;
				}
			}

			pss += ss_stride;
			pdd += ss_stride;
			pxx += xx_width;

		} else {
			x = pxx + (xx_width & ~1);
			vis_write_gsr((10 << 3) | 2);

			off = (mlib_addr)pxx & 7;
			d_pxx = (mlib_d64 *)((mlib_u8 *)pxx - off);
			bmask0 = off * 0x11111111 + 0x01234567;

			s = pss;
			ld0 = (*d_pxx++);

			vis_write_bmask(bmask0, 0);
			ld1 = vis_ld_d64_nf(d_pxx); d_pxx++;
			sr0 = vis_bshuffle(ld0, ld1);
			ld2 = vis_ld_d64_nf(d_pxx); d_pxx++;
			sr1 = vis_bshuffle(ld1, ld2);

			vis_write_bmask(0x014589cd, 0);
			src0 = vis_bshuffle(sr0, sr1);
			src1 = vis_bshuffle(vis_faligndata(sr0, sr0),
				vis_faligndata(sr1, sr1));

			sub = vis_fpsub16(src0, src1);
			add = vis_fpadd16(src0, src1);
			add = vis_fandnot(round_mask, add);
			res = vis_fmul8x16(scale, add);

			buf_s1 = res;

			for (i = 0; i < len; i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, s);
				s++;
			}

			if (xx_width & 1) {
				*s = *x;
				buf_tmp = vis_ld_u16(x);

				if (len == 1) {
					vis_write_bmask(0xef0101ef, 0);
					src0 = vis_bshuffle(buf_s1, buf_tmp);
					src3 = src0;
				} else
				if (len == 2) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1, buf_tmp);
					vis_write_bmask(0xefef2323, 0);
					src3 = vis_bshuffle(buf_s1, buf_tmp);
				} else
				if (len == 3) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1, buf_tmp);
					vis_write_bmask(0x45efef45, 0);
					src3 = vis_bshuffle(buf_s1, buf_tmp);
				}

			} else {

				if (len == 1) {
					vis_write_bmask(0x01010101, 0);
					src0 = vis_bshuffle(buf_s1,
						vis_fzero());
					src3 = src0;
				} else
				if (len == 2) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1,
						vis_fzero());
					src3 = src0;
				} else
				if (len == 3) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1,
						vis_fzero());
					vis_write_bmask(0x454523ef, 0);
					src3 = vis_bshuffle(buf_s1,
						vis_fzero());
				}
			}

			d = pdd;

			add = sub;
			src1 = vis_faligndata(src0, src3);
			vis_write_bmask(0x6789abcd, 0);
			src2 = vis_bshuffle(src0, src3);

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

			for (i = 0; i < len; i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, d);
				d++;
			}

			pss += ss_stride;
			pdd += ss_stride;
			pxx += xx_width;
		}
	}
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_S16_src_all(
	void *ss_ptr,
	mlib_s32 ss_stride,
	void *dd_ptr,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	mlib_s32 xx_stride)
{
	mlib_s32 i, j, k, len;
	mlib_s16 *s, *pss, *d, *pdd, *x, *pxx;
	mlib_d64 src0, src1, src2, src3;
	mlib_d64 tmp0, tmp1, tmp2, tmp3;
	mlib_d64 prom, add, add0, add1, res, sub;
	mlib_d64 round_mask = vis_to_double_dup(0x10001);
	mlib_d64 scale_32 = vis_to_double_dup(32);
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_f32 scale_3 = vis_to_float(0x00030003);
	mlib_f32 scale_22 = vis_to_float(0x00160016);
	mlib_d64 sr0, sr1, *d_pxx;
	mlib_d64 buf_s0, buf_s1, buf_s2, buf_s3 = 0, buf_tmp;
	mlib_s32 bmask1;
	mlib_d64 *ds, *dd;

	pss = ss_ptr;
	pdd = dd_ptr;
	pxx = xx_ptr;

	len = xx_width >> 1;

	for (j = 0; j < xx_height; j++) {
		if (len > 3) {
			x = pxx + (xx_width & ~1);
			vis_write_gsr((10 << 3) | 2);

			d_pxx = (mlib_d64 *)pxx;

			s = pss;
			ds = (mlib_d64 *)s;

			sr0 = (*d_pxx++);
			sr1 = (*d_pxx++);

			tmp0 = vis_fpmerge(vis_read_hi(sr0), vis_read_hi(sr1));
			tmp1 = vis_fpmerge(vis_read_lo(sr0), vis_read_lo(sr1));
			tmp2 = vis_fpmerge(vis_read_hi(tmp0),
				vis_read_hi(tmp1));
			tmp3 = vis_fpmerge(vis_read_lo(tmp0),
				vis_read_lo(tmp1));
			src0 = vis_fpmerge(vis_read_hi(tmp2),
				vis_read_lo(tmp2));
			src1 = vis_fpmerge(vis_read_hi(tmp3),
				vis_read_lo(tmp3));

			sub = vis_fpsub16(src0, src1);
			add = vis_fpadd16(src0, src1);
			add = vis_fandnot(round_mask, add);
			res = vis_fmul8x16(scale, add);

			add0 = sub;
			buf_s1 = res;

			(*ds++) = res;

			vis_write_bmask(0x88882301, 0);
			buf_s0 = vis_bshuffle(buf_s1, vis_fzero());

			bmask1 = 0x456789ab;
			vis_write_bmask(bmask1, 0);
			src3 = vis_bshuffle(buf_s0, buf_s1);
			d = pdd;
			dd = (mlib_d64 *)d;

			for (i = 1; i < (len >> 2); i++) {
				sr0 = (*d_pxx++);
				sr1 = (*d_pxx++);

				tmp0 = vis_fpmerge(vis_read_hi(sr0),
					vis_read_hi(sr1));
				tmp1 = vis_fpmerge(vis_read_lo(sr0),
					vis_read_lo(sr1));
				tmp2 = vis_fpmerge(vis_read_hi(tmp0),
					vis_read_hi(tmp1));
				tmp3 = vis_fpmerge(vis_read_lo(tmp0),
					vis_read_lo(tmp1));
				src0 = vis_fpmerge(vis_read_hi(tmp2),
					vis_read_lo(tmp2));
				src1 = vis_fpmerge(vis_read_hi(tmp3),
					vis_read_lo(tmp3));

				sub = vis_fpsub16(src0, src1);
				add = vis_fpadd16(src0, src1);
				add = vis_fandnot(round_mask, add);
				res = vis_fmul8x16(scale, add);

				add1 = sub;
				buf_s2 = res;

				(*ds++) = res;

				vis_write_bmask(bmask1, 0);
				src0 = src3;
				src3 = vis_bshuffle(buf_s1, buf_s2);
				src1 = vis_faligndata(src0, src3);
				src2 = vis_faligndata(buf_s1, buf_s2);
				buf_s1 = buf_s2;

				tmp0 = vis_fpsub16(src0, src3);
				tmp1 = vis_fpsub16(src2, src1);

				tmp2 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_hi(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_hi(tmp1)));

				tmp3 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_lo(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_lo(tmp1)));

				tmp2 = vis_fpadd32(tmp2, scale_32);
				tmp3 = vis_fpadd32(tmp3, scale_32);

				prom = vis_fpackfix_pair(tmp2, tmp3);
				res = vis_fpadd16(prom, add0);

				(*dd++) = res;
				add0 = add1;
			}

			s = (mlib_s16 *)ds;

			if (len & 3) {
				sr0 = (*d_pxx++);
				sr1 = (*d_pxx++);

				tmp0 = vis_fpmerge(vis_read_hi(sr0),
					vis_read_hi(sr1));
				tmp1 = vis_fpmerge(vis_read_lo(sr0),
					vis_read_lo(sr1));
				tmp2 = vis_fpmerge(vis_read_hi(tmp0),
					vis_read_hi(tmp1));
				tmp3 = vis_fpmerge(vis_read_lo(tmp0),
					vis_read_lo(tmp1));
				src0 = vis_fpmerge(vis_read_hi(tmp2),
					vis_read_lo(tmp2));
				src1 = vis_fpmerge(vis_read_hi(tmp3),
					vis_read_lo(tmp3));

				sub = vis_fpsub16(src0, src1);
				add = vis_fpadd16(src0, src1);
				add = vis_fandnot(round_mask, add);
				res = vis_fmul8x16(scale, add);

				add1 = sub;
				buf_s2 = res;

				for (k = 0; k < (len & 3); k++) {
					res = vis_faligndata(res, res);
					vis_st_u16(res, s);
					s++;
				}

				if (xx_width & 1) {
					*s = *x;
					buf_tmp = vis_ld_u16(x);

					if ((len & 3) == 1) {
						vis_write_bmask(0x01efef01, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
					} else
/* besides needs add one pixel */
					if ((len & 3) == 2) {
						vis_write_bmask(0x0123efef, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
						vis_write_bmask(0x23abcdef, 0);
						buf_s3 = vis_bshuffle(buf_s2,
							buf_s3);
					} else
					if ((len & 3) == 3) {
/* besides needs add two pixels */
						vis_write_bmask(0x012345ef, 0);
						buf_s2 = vis_bshuffle(buf_s2,
							buf_tmp);
						vis_write_bmask(0x6745cdef, 0);
						buf_s3 = vis_bshuffle(buf_s2,
							buf_s3);
					}

				} else {

					if ((len & 3) == 1) {
						vis_write_bmask(0x898967ef, 0);
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
					} else
					if ((len & 3) == 2) {
						vis_write_bmask(0x89abab89, 0);
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
					} else
					if ((len & 3) == 3) {
						vis_write_bmask(0x89abcdcd, 0);
/* besides needs add one pixel */
						buf_s2 = vis_bshuffle(buf_s1,
							buf_s2);
						vis_write_bmask(0x23abcdef, 0);
						buf_s3 = vis_bshuffle(buf_s2,
							buf_s3);
					}
				}

			} else {

				if (xx_width & 1) {
					*s = *x;
					buf_s2 = vis_ld_u16(x);
					vis_write_bmask(0xefef67cd, 0);
					buf_s2 = vis_bshuffle(buf_s1, buf_s2);

				} else {

					vis_write_bmask(0x67458888, 0);
					buf_s2 = vis_bshuffle(buf_s1,
						vis_fzero());
				}
			}

			vis_write_bmask(bmask1, 0);
			src0 = src3;
			src3 = vis_bshuffle(buf_s1, buf_s2);
			src1 = vis_faligndata(src0, src3);
			src2 = vis_faligndata(buf_s1, buf_s2);
			buf_s1 = buf_s2;

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
			res = vis_fpadd16(prom, add0);

			(*dd++) = res;
			add0 = add1;
			d = (mlib_s16 *)dd;

			if (len & 3) {
				buf_s2 = buf_s3;
				src0 = src3;
				vis_write_bmask(bmask1, 0);
				src3 = vis_bshuffle(buf_s1, buf_s2);
				src1 = vis_faligndata(src0, src3);
				src2 = vis_faligndata(buf_s1, buf_s2);

				tmp0 = vis_fpsub16(src0, src3);
				tmp1 = vis_fpsub16(src2, src1);

				tmp2 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_hi(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_hi(tmp1)));

				tmp3 = vis_fpadd32(vis_fmuld8ulx16(scale_3,
					vis_read_lo(tmp0)),
					vis_fmuld8ulx16(scale_22,
					vis_read_lo(tmp1)));

				tmp2 = vis_fpadd32(tmp2, scale_32);
				tmp3 = vis_fpadd32(tmp3, scale_32);

				prom = vis_fpackfix_pair(tmp2, tmp3);
				res = vis_fpadd16(prom, add0);

				for (k = 0; k < (len & 3); k++) {
					res = vis_faligndata(res, res);
					vis_st_u16(res, d);
					d++;
				}
			}

			pss += ss_stride;
			pdd += ss_stride;
			pxx += xx_stride;
		} else {
			x = pxx + (xx_width & ~1);
			vis_write_gsr((10 << 3) | 2);

			d_pxx = (mlib_d64 *)pxx;
			s = pss;

			sr0 = (*d_pxx++);
			sr1 = (*d_pxx++);

			tmp0 = vis_fpmerge(vis_read_hi(sr0), vis_read_hi(sr1));
			tmp1 = vis_fpmerge(vis_read_lo(sr0), vis_read_lo(sr1));
			tmp2 = vis_fpmerge(vis_read_hi(tmp0),
				vis_read_hi(tmp1));
			tmp3 = vis_fpmerge(vis_read_lo(tmp0),
				vis_read_lo(tmp1));
			src0 = vis_fpmerge(vis_read_hi(tmp2),
				vis_read_lo(tmp2));
			src1 = vis_fpmerge(vis_read_hi(tmp3),
				vis_read_lo(tmp3));

			sub = vis_fpsub16(src0, src1);
			add = vis_fpadd16(src0, src1);
			add = vis_fandnot(round_mask, add);
			res = vis_fmul8x16(scale, add);

			buf_s1 = res;

			for (i = 0; i < len; i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, s);
				s++;
			}

			if (xx_width & 1) {
				*s = *x;
				buf_tmp = vis_ld_u16(x);

				if (len == 1) {
					vis_write_bmask(0xef0101ef, 0);
					src0 = vis_bshuffle(buf_s1, buf_tmp);
					src3 = src0;
				} else
				if (len == 2) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1, buf_tmp);
					vis_write_bmask(0xefef2323, 0);
					src3 = vis_bshuffle(buf_s1, buf_tmp);
				} else
				if (len == 3) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1, buf_tmp);
					vis_write_bmask(0x45efef45, 0);
					src3 = vis_bshuffle(buf_s1, buf_tmp);
				}

			} else {

				if (len == 1) {
					vis_write_bmask(0x01010101, 0);
					src0 = vis_bshuffle(buf_s1,
						vis_fzero());
					src3 = src0;
				} else
				if (len == 2) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1,
						vis_fzero());
					src3 = src0;
				} else
				if (len == 3) {
					vis_write_bmask(0x23010123, 0);
					src0 = vis_bshuffle(buf_s1,
						vis_fzero());
					vis_write_bmask(0x454523ef, 0);
					src3 = vis_bshuffle(buf_s1,
						vis_fzero());
				}
			}

			d = pdd;

			add = sub;
			src1 = vis_faligndata(src0, src3);
			vis_write_bmask(0x6789abcd, 0);
			src2 = vis_bshuffle(src0, src3);

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

			for (i = 0; i < len; i++) {
				res = vis_faligndata(res, res);
				vis_st_u16(res, d);
				d++;
			}

			pss += ss_stride;
			pdd += ss_stride;
			pxx += xx_stride;
		}
	}
}

/* *********************************************************** */

static void
mlib_MatrixTranspose_S16_S16_dst_na(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 x_stride)
{
	mlib_s32 i, j;
	mlib_d64 p0, p1, p2, p3, *dp0, *dp1, *dp2, *dp3;
	mlib_d64 q0, q1, q2, q3;
	mlib_s32 dx_stride = x_stride >> 2;
	mlib_d64 aa0, bb0, aa1;
	mlib_s16 *dst0, *dst1, *dst2, *dst3;
	mlib_s16 *ld = (mlib_s16 *)x, *st = z;

/*  ld and st by doubles  */
	if (!((m | n) & 3) && !((mlib_addr)st & 7)) {
/*  for UIII it works very bad because of store problem  */
		for (i = 0; i <= m - 4; i += 4) {
			dst0 = st + i;
			dst1 = dst0 + m;
			dst2 = dst1 + m;
			dst3 = dst2 + m;

			dp0 = (mlib_d64 *)ld + i * dx_stride;
			dp1 = dp0 + dx_stride;
			dp2 = dp1 + dx_stride;
			dp3 = dp2 + dx_stride;
			for (j = 0; j < n >> 2; j++) {
				p0 = dp0[j];
				p1 = dp1[j];
				p2 = dp2[j];
				p3 = dp3[j];

				aa1 = vis_fpmerge(vis_read_hi(p0),
					vis_read_hi(p2));
				bb0 = vis_fpmerge(vis_read_hi(p1),
					vis_read_hi(p3));
				aa0 = vis_fpmerge(vis_read_hi(aa1),
					vis_read_hi(bb0));
				q0 = vis_fpmerge(vis_read_hi(aa0),
					vis_read_lo(aa0));
				aa0 = vis_fpmerge(vis_read_lo(aa1),
					vis_read_lo(bb0));
				q1 = vis_fpmerge(vis_read_hi(aa0),
					vis_read_lo(aa0));
				aa1 = vis_fpmerge(vis_read_lo(p0),
					vis_read_lo(p2));
				bb0 = vis_fpmerge(vis_read_lo(p1),
					vis_read_lo(p3));
				aa0 = vis_fpmerge(vis_read_hi(aa1),
					vis_read_hi(bb0));
				q2 = vis_fpmerge(vis_read_hi(aa0),
					vis_read_lo(aa0));
				aa0 = vis_fpmerge(vis_read_lo(aa1),
					vis_read_lo(bb0));
				q3 = vis_fpmerge(vis_read_hi(aa0),
					vis_read_lo(aa0));

				*(mlib_d64 *)dst0 = q0;
				*(mlib_d64 *)dst1 = q1;
				*(mlib_d64 *)dst2 = q2;
				*(mlib_d64 *)dst3 = q3;

				dst0 = dst3 + m;
				dst1 = dst0 + m;
				dst2 = dst1 + m;
				dst3 = dst2 + m;
			}
		}

	} else {
		mlib_s32 k, l;
		const mlib_s16 *ldo = x;
		mlib_s16 *sto = z;

		for (l = 0; l < n; l++) {
#pragma pipeloop(0)
			for (k = 0; k < m; k++) {
				sto[k] = ldo[k * x_stride];
			}

			ldo++;
			sto += m;
		}
	}

/* *********************************************************** */

}

/* *********************************************************** */

#define	FORWARD_TRANSFORM_S32_S32                                   \
	vis_write_gsr((9 << 3) | 4);                                \
	len = xx_width >> 1;                                        \
	pxx = xx_ptr;                                               \
	pss = ss_ptr;                                               \
	pdd = dd_ptr;                                               \
	for (j = 0; j < xx_height; j++) {                           \
	    if (len > 3) {                                          \
		x = pxx;                                            \
		s = pss;                                            \
		ds = (mlib_d64 *)s;                                 \
		p0 = (x[0] + x[1]) >> 1;                            \
		p1 = (x[2] + x[3]) >> 1;                            \
		sub0 = x[0] - x[1];                                 \
		sub1 = x[2] - x[3];                                 \
		dbuf0 = vis_to_double(sub0, sub1);                  \
		d3_1 = vis_to_double(p0 * 3, p1 * 3);               \
		d3_0 = vis_to_double(p1 * 3, p0 * 3);               \
		d22_1 = vis_to_double(p0 * 22, p1 * 22);            \
		d22_0 = vis_to_double(p1 * 22, p0 * 22);            \
		(*ds++) = vis_to_double(p0, p1);                    \
		x += 4;                                             \
		d = pdd;                                            \
		dd = (mlib_d64 *)d;                                 \
		for (i = 1; i < (len >> 1); i++) {                  \
		    p0 = (x[0] + x[1]) >> 1;                        \
		    p1 = (x[2] + x[3]) >> 1;                        \
		    sub0 = x[0] - x[1];                             \
		    sub1 = x[2] - x[3];                             \
		    dbuf1 = vis_to_double(sub0, sub1);              \
		    d3_2 = vis_to_double(p0 * 3, p1 * 3);           \
		    d22_2 = vis_to_double(p0 * 22, p1 * 22);        \
		    (*ds++) = vis_to_double(p0, p1);                \
		    x += 4;                                         \
		    add = dbuf0;                                    \
		    dbuf0 = dbuf1;                                  \
		    dsc_3_0 = d3_0;                                 \
		    dsc_3_1 = d3_2;                                 \
		    dsc_22_0 = vis_faligndata(d22_1, d22_2);        \
		    dsc_22_1 = vis_faligndata(d22_0, d22_1);        \
		    d3_0 = d3_1;                                    \
		    d3_1 = d3_2;                                    \
		    d22_0 = d22_1;                                  \
		    d22_1 = d22_2;                                  \
		    dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);            \
		    dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);          \
		    dp = vis_fpadd32(dp1, dp2);                     \
		    src = vis_fpadd32(dp, add_32);                  \
		    prom0 = vis_fand(mask, src);                    \
		    prom0 = vis_fmul8x16(mask_mul, prom0);          \
		    prom1 = vis_fandnot(mask, src);                 \
		    prom1 = vis_fpack32(dzero, prom1);              \
		    prom1 = vis_fpack32(prom1, dzero);              \
		    dst = vis_for(prom0, prom1);                    \
		    dst = vis_fpadd32(dst, add);                    \
		    (*dd++) = dst;                                  \
		}                                                   \
		s = (mlib_s32 *)ds;                                 \
		if (len & 1) {                                      \
		    if (xx_width & 1) {                             \
			p0 = (x[0] + x[1]) >> 1;                    \
			sub0 = x[0] - x[1];                         \
			dbuf1 = vis_to_double(sub0, 0);             \
			*s = p0;                                    \
			s++;                                        \
			x += 2;                                     \
			*s = *x;                                    \
			d3_2 = vis_to_double(p0 * 3, *x * 3);       \
			d3_3 = vis_freg_pair(vis_read_lo(d3_2),     \
				vis_read_hi(d3_2));                 \
			d22_2 = vis_to_double(p0 * 22, *x * 22);    \
			d22_3 = vis_freg_pair(vis_read_lo(d22_2),   \
				vis_read_hi(d22_2));                \
		    } else {                                        \
			p0 = (x[0] + x[1]) >> 1;                    \
			sub0 = x[0] - x[1];                         \
			dbuf1 = vis_to_double(sub0, 0);             \
			d3_2 = vis_to_double(p0 * 3, p0 * 3);       \
			d22_2 = vis_to_double(p0 * 22, p0 * 22);    \
			*s = p0;                                    \
			s++;                                        \
			x += 2;                                     \
			d3_3 = vis_freg_pair(vis_read_lo(d3_1),     \
				vis_fzeros());                      \
			d22_3 = vis_freg_pair(vis_read_lo(d22_1),   \
				vis_fzeros());                      \
		    }                                               \
		} else {                                            \
		    if (xx_width & 1) {                             \
			*s = *x;                                    \
			d3_2 = vis_to_double(*x * 3, *x * 3);       \
			d3_3 = vis_freg_pair(vis_read_lo(d3_1),     \
				vis_fzeros());                      \
			d22_2 = vis_to_double(*x * 22, *x * 22);    \
			d22_3 = vis_freg_pair(vis_read_lo(d22_1),   \
				vis_fzeros());                      \
		    } else {                                        \
			d3_2 = vis_freg_pair(vis_read_lo(d3_1),     \
				vis_read_hi(d3_1));                 \
			d22_2 = vis_freg_pair(vis_read_lo(d22_1),   \
				vis_read_hi(d22_1));                \
		    }                                               \
		}                                                   \
		add = dbuf0;                                        \
		dbuf0 = dbuf1;                                      \
		dsc_3_0 = d3_0;                                     \
		dsc_3_1 = d3_2;                                     \
		dsc_22_0 = vis_faligndata(d22_1, d22_2);            \
		dsc_22_1 = vis_faligndata(d22_0, d22_1);            \
		d3_0 = d3_1;                                        \
		d3_1 = d3_2;                                        \
		d3_2 = d3_3;                                        \
		d22_0 = d22_1;                                      \
		d22_1 = d22_2;                                      \
		d22_2 = d22_3;                                      \
		dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);                \
		dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);              \
		dp = vis_fpadd32(dp1, dp2);                         \
		src = vis_fpadd32(dp, add_32);                      \
		prom0 = vis_fand(mask, src);                        \
		prom0 = vis_fmul8x16(mask_mul, prom0);              \
		prom1 = vis_fandnot(mask, src);                     \
		prom1 = vis_fpack32(dzero, prom1);                  \
		prom1 = vis_fpack32(prom1, dzero);                  \
		dst = vis_for(prom0, prom1);                        \
		dst = vis_fpadd32(dst, add);                        \
		(*dd++) = dst;                                      \
		d = (mlib_s32 *)dd;                                 \
		if (len & 1) {                                      \
		    add = dbuf0;                                    \
		    dsc_3_0 = d3_0;                                 \
		    dsc_3_1 = d3_2;                                 \
		    dsc_22_0 = vis_faligndata(d22_1, d22_2);        \
		    dsc_22_1 = vis_faligndata(d22_0, d22_1);        \
		    dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);            \
		    dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);          \
		    dp = vis_fpadd32(dp1, dp2);                     \
		    src = vis_fpadd32(dp, add_32);                  \
		    prom0 = vis_fand(mask, src);                    \
		    prom0 = vis_fmul8x16(mask_mul, prom0);          \
		    prom1 = vis_fandnot(mask, src);                 \
		    prom1 = vis_fpack32(dzero, prom1);              \
		    prom1 = vis_fpack32(prom1, dzero);              \
		    dst = vis_for(prom0, prom1);                    \
		    dst = vis_fpadd32(dst, add);                    \
		    *(mlib_f32 *)d = vis_read_hi(dst);              \
		}                                                   \
		pxx += xx_width;                                    \
		pss += ss_stride;                                   \
		pdd += ss_stride;                                   \
	    } else {                                                \
		x = pxx;                                            \
		s = pss;                                            \
		sc_22 = bscale_22;                                  \
		sc_3 = bscale_3;                                    \
		for (i = 0; i < len; i++) {                         \
		    p = (x[0] + x[1]) >> 1;                         \
		    buf[i] = x[0] - x[1];                           \
		    (*sc_3++) = p * 3;                              \
		    (*sc_22++) = p * 22;                            \
		    *s = p;                                         \
		    x += 2;                                         \
		    s++;                                            \
		}                                                   \
		if (xx_width & 1) {                                 \
		    *s = *x;                                        \
		    (*sc_3++) = *x * 3;                             \
		    (*sc_22++) = *x * 22;                           \
		}                                                   \
		bscale_22[-1] = bscale_22[0];                       \
		bscale_3[-1] = bscale_3[0];                         \
		d = pdd;                                            \
		sc_22[0] = sc_22[-1];                               \
		sc_22[1] = sc_22[-2];                               \
		sc_3[0] = sc_3[-1];                                 \
		sc_3[1] = sc_3[-2];                                 \
		sc_22 = bscale_22;                                  \
		sc_3 = bscale_3;                                    \
		sc_22[-1] = sc_22[0];                               \
		sc_22[-2] = sc_22[1];                               \
		sc_3[-1] = sc_3[0];                                 \
		sc_3[-2] = sc_3[1];                                 \
		i = 0;                                              \
		if (len >> 1) {                                     \
		    add = *(mlib_d64 *)buf;                         \
		    dsc_3_0 = *(mlib_d64 *)(sc_3 - 2);              \
		    dsc_3_1 = *(mlib_d64 *)(sc_3 + 2);              \
		    dsc_22_0 = *(mlib_d64 *)(sc_22 + 1);            \
		    dsc_22_1 = *(mlib_d64 *)(sc_22 - 1);            \
		    dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);            \
		    dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);          \
		    dp = vis_fpadd32(dp1, dp2);                     \
		    src = vis_fpadd32(dp, add_32);                  \
		    prom0 = vis_fand(mask, src);                    \
		    prom0 = vis_fmul8x16(mask_mul, prom0);          \
		    prom1 = vis_fandnot(mask, src);                 \
		    prom1 = vis_fpack32(dzero, prom1);              \
		    prom1 = vis_fpack32(prom1, dzero);              \
		    dst = vis_for(prom0, prom1);                    \
		    dst = vis_fpadd32(dst, add);                    \
		    *(mlib_f32 *)d = vis_read_hi(dst);              \
		    d++;                                            \
		    *(mlib_f32 *)d = vis_read_lo(dst);              \
		    d++;                                            \
		    sc_3 += 2;                                      \
		    sc_22 += 2;                                     \
		    i++;                                            \
		}                                                   \
		if (len & 1) {                                      \
		    add = *(mlib_d64 *)(buf + 2 * i);               \
		    dsc_3_0 = *(mlib_d64 *)(sc_3 - 2);              \
		    dsc_3_1 = *(mlib_d64 *)(sc_3 + 2);              \
		    dsc_22_0 = *(mlib_d64 *)(sc_22 + 1);            \
		    dsc_22_1 = *(mlib_d64 *)(sc_22 - 1);            \
		    dp1 = vis_fpsub32(dsc_3_0, dsc_3_1);            \
		    dp2 = vis_fpsub32(dsc_22_0, dsc_22_1);          \
		    dp = vis_fpadd32(dp1, dp2);                     \
		    src = vis_fpadd32(dp, add_32);                  \
		    prom0 = vis_fand(mask, src);                    \
		    prom0 = vis_fmul8x16(mask_mul, prom0);          \
		    prom1 = vis_fandnot(mask, src);                 \
		    prom1 = vis_fpack32(dzero, prom1);              \
		    prom1 = vis_fpack32(prom1, dzero);              \
		    dst = vis_for(prom0, prom1);                    \
		    dst = vis_fpadd32(dst, add);                    \
		    *(mlib_f32 *)d = vis_read_hi(dst);              \
		}                                                   \
		pxx += xx_width;                                    \
		pss += ss_stride;                                   \
		pdd += ss_stride;                                   \
	    }                                                       \
	}

/* *********************************************************** */

static void
mlib_MatrixTranspose_S32_S32_dst_na(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 x_stride)
{
	mlib_s32 k, l;
	const mlib_s32 *ldo = x;
	mlib_s32 *sto = z;

	for (l = 0; l < n; l++) {
#pragma pipeloop(0)
		for (k = 0; k < m; k++) {
			sto[k] = ldo[k * x_stride];
		}

		ldo++;
		sto += m;
	}
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S16(
	void *ss_ptr,
	mlib_s32 ss_stride,
	void *dd_ptr,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height)
{
	mlib_s32 i, j, p, p0, p1, len;
	mlib_s16 *pxx, *x;
	mlib_s32 *pdd, *pss, *d, *s;
	mlib_d64 buf_sc_22[5], buf_sc_3[5], d_buf[5];
	mlib_s32 *buf = (mlib_s32 *)d_buf;
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
	mlib_s32 sub0, sub1;
	mlib_d64 dbuf0, dbuf1, d3_0, d3_1, d3_2, d3_3;
	mlib_d64 d22_0, d22_1, d22_2, d22_3;
	mlib_d64 *ds, *dd;

FORWARD_TRANSFORM_S32_S32}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S32_src_na(
	void *ss_ptr,
	mlib_s32 ss_stride,
	void *dd_ptr,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height)
{
	mlib_s32 i, j, p, p0, p1, len;
	mlib_s32 *pdd, *pss, *pxx, *d, *s, *x;
	mlib_d64 buf_sc_22[5], buf_sc_3[5], d_buf[5];
	mlib_s32 *buf = (mlib_s32 *)d_buf;
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
	mlib_s32 sub0, sub1;
	mlib_d64 dbuf0, dbuf1, d3_0, d3_1, d3_2, d3_3;
	mlib_d64 d22_0, d22_1, d22_2, d22_3;
	mlib_d64 *ds, *dd;

FORWARD_TRANSFORM_S32_S32}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S32_src_all(
	void *ss_ptr,
	mlib_s32 ss_stride,
	void *dd_ptr,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	mlib_s32 xx_stride)
{
	mlib_s32 i, j, p, p0, p1, len;
	mlib_s32 *pdd, *pss, *pxx, *d, *s, *x;
	mlib_d64 buf_sc_22[5], buf_sc_3[5], d_buf[5];
	mlib_s32 *buf = (mlib_s32 *)d_buf;
	mlib_s32 *bscale_22 = (mlib_s32 *)buf_sc_22 + 3;
	mlib_s32 *bscale_3 = (mlib_s32 *)buf_sc_3 + 2;
	mlib_s32 *sc_22, *sc_3, sub0;
	mlib_d64 dsc_3_0, dsc_3_1, dsc_22_0, dsc_22_1;
	mlib_d64 dp1, dp2, add, dp, src, dst;
	mlib_d64 prom0, prom1;
	mlib_d64 mask = vis_to_double_dup(0xfffe01fe << 5);
	mlib_d64 add_32 = vis_to_double_dup(32);
	mlib_d64 dzero = vis_fzero();
	mlib_f32 mask_mul = vis_to_float(0x80808080 >> 5);
	mlib_d64 dbuf0, dbuf1, d3_0, d3_1, d3_2, d3_3;
	mlib_d64 d22_0, d22_1, d22_2, d22_3;
	mlib_d64 *ds, *dd;

	vis_write_gsr((9 << 3) | 4);
	len = xx_width >> 1;
	pxx = xx_ptr;
	pss = ss_ptr;
	pdd = dd_ptr;

	for (j = 0; j < xx_height; j++) {
		mlib_d64 *dx, ld00, ld11, ld0, ld1, addp;

		if (len > 3) {
			x = pxx;
			dx = (mlib_d64 *)x;
			s = pss;
			ds = (mlib_d64 *)s;
			ld00 = dx[0];
			ld11 = dx[1];
			dx += 2;
			ld0 = vis_freg_pair(vis_read_hi(ld00),
				vis_read_hi(ld11));
			ld1 = vis_freg_pair(vis_read_lo(ld00),
				vis_read_lo(ld11));
			addp = vis_fpadd32(ld0, ld1);
			dbuf0 = vis_fpsub32(ld0, ld1);
			p0 = *((mlib_s32 *)&addp) >> 1;
			p1 = *((mlib_s32 *)&addp + 1) >> 1;
			d3_1 = vis_to_double(p0 * 3, p1 * 3);
			d3_0 = vis_to_double(p1 * 3, p0 * 3);
			d22_1 = vis_to_double(p0 * 22, p1 * 22);
			d22_0 = vis_to_double(p1 * 22, p0 * 22);
			(*ds++) = vis_to_double(p0, p1);

			d = pdd;
			dd = (mlib_d64 *)d;
#pragma pipeloop(0)
			for (i = 1; i < (len >> 1); i++) {
				ld00 = dx[0];
				ld11 = dx[1];
				dx += 2;
				ld0 = vis_freg_pair(vis_read_hi(ld00),
					vis_read_hi(ld11));
				ld1 = vis_freg_pair(vis_read_lo(ld00),
					vis_read_lo(ld11));
				addp = vis_fpadd32(ld0, ld1);
				dbuf1 = vis_fpsub32(ld0, ld1);
				p0 = *((mlib_s32 *)&addp) >> 1;
				p1 = *((mlib_s32 *)&addp + 1) >> 1;
				d3_2 = vis_to_double(p0 * 3, p1 * 3);
				d22_2 = vis_to_double(p0 * 22, p1 * 22);
				(*ds++) = vis_to_double(p0, p1);

				add = dbuf0;
				dbuf0 = dbuf1;
				dsc_3_0 = d3_0;
				dsc_3_1 = d3_2;
				dsc_22_0 = vis_faligndata(d22_1, d22_2);
				dsc_22_1 = vis_faligndata(d22_0, d22_1);

				d3_0 = d3_1;
				d3_1 = d3_2;
				d22_0 = d22_1;
				d22_1 = d22_2;

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

				(*dd++) = dst;
			}

			s = (mlib_s32 *)ds;
			x = (mlib_s32 *)dx;

			if (len & 1) {
				if (xx_width & 1) {
					p0 = (x[0] + x[1]) >> 1;
					sub0 = x[0] - x[1];
					dbuf1 = vis_to_double(sub0, 0);
					*s = p0;
					s++;
					x += 2;
					*s = *x;
					d3_2 = vis_to_double(p0 * 3, *x * 3);
					d3_3 = vis_freg_pair(vis_read_lo(d3_2),
						vis_read_hi(d3_2));
					d22_2 = vis_to_double(p0 * 22, *x * 22);
					d22_3 = vis_freg_pair(vis_read_lo
						(d22_2), vis_read_hi(d22_2));
				} else {
					p0 = (x[0] + x[1]) >> 1;
					sub0 = x[0] - x[1];
					dbuf1 = vis_to_double(sub0, 0);
					d3_2 = vis_to_double(p0 * 3, p0 * 3);
					d22_2 = vis_to_double(p0 * 22, p0 * 22);
					*s = p0;
					s++;
					x += 2;
					d3_3 = vis_freg_pair(vis_read_lo(d3_1),
						vis_fzeros());
					d22_3 = vis_freg_pair(vis_read_lo
						(d22_1), vis_fzeros());
				}

			} else {

				if (xx_width & 1) {
					*s = *x;
					d3_2 = vis_to_double(*x * 3, *x * 3);
					d3_3 = vis_freg_pair(vis_read_lo(d3_1),
						vis_fzeros());
					d22_2 = vis_to_double(*x * 22, *x * 22);
					d22_3 = vis_freg_pair(vis_read_lo
						(d22_1), vis_fzeros());
				} else {
					d3_2 = vis_freg_pair(vis_read_lo(d3_1),
						vis_read_hi(d3_1));
					d22_2 = vis_freg_pair(vis_read_lo
						(d22_1), vis_read_hi(d22_1));
				}
			}

			add = dbuf0;
			dbuf0 = dbuf1;
			dsc_3_0 = d3_0;
			dsc_3_1 = d3_2;
			dsc_22_0 = vis_faligndata(d22_1, d22_2);
			dsc_22_1 = vis_faligndata(d22_0, d22_1);

			d3_0 = d3_1;
			d3_1 = d3_2;
			d3_2 = d3_3;
			d22_0 = d22_1;
			d22_1 = d22_2;
			d22_2 = d22_3;

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

			(*dd++) = dst;
			d = (mlib_s32 *)dd;

			if (len & 1) {
				add = dbuf0;
				dsc_3_0 = d3_0;
				dsc_3_1 = d3_2;
				dsc_22_0 = vis_faligndata(d22_1, d22_2);
				dsc_22_1 = vis_faligndata(d22_0, d22_1);

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

			pxx += xx_stride;
			pss += ss_stride;
			pdd += ss_stride;

		} else {
			x = pxx;
			s = pss;
			sc_22 = bscale_22;
			sc_3 = bscale_3;
			for (i = 0; i < len; i++) {
				p = (x[0] + x[1]) >> 1;
				buf[i] = x[0] - x[1];
				(*sc_3++) = p * 3;
				(*sc_22++) = p * 22;
				*s = p;
				x += 2;
				s++;
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

			i = 0;

			if (len >> 1) {
				add = *(mlib_d64 *)buf;
				dsc_3_0 = *(mlib_d64 *)(sc_3 - 2);
				dsc_3_1 = *(mlib_d64 *)(sc_3 + 2);
				dsc_22_0 = *(mlib_d64 *)(sc_22 + 1);
				dsc_22_1 = *(mlib_d64 *)(sc_22 - 1);

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
				d++;
				*(mlib_f32 *)d = vis_read_lo(dst);
				d++;
				sc_3 += 2;
				sc_22 += 2;
				i++;
			}

			if (len & 1) {
				add = *(mlib_d64 *)(buf + 2 * i);
				dsc_3_0 = *(mlib_d64 *)(sc_3 - 2);
				dsc_3_1 = *(mlib_d64 *)(sc_3 + 2);
				dsc_22_0 = *(mlib_d64 *)(sc_22 + 1);
				dsc_22_1 = *(mlib_d64 *)(sc_22 - 1);

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

			pxx += xx_stride;
			pss += ss_stride;
			pdd += ss_stride;
		}
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
	mlib_s16 *s_buffer, *d_buffer, *s_buf_trans, *d_buf_trans;
	mlib_s16 *ss_buf_trans, *sd_buf_trans, *ds_buf_trans, *dd_buf_trans;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 size1, size2, len0, len1, height1;

/*  in shorts  */
	len0 = ((ss_x + 3) & ~3);
/*  to make matrix be processed by fast case (4x4)  */
	height1 = ((height + 3) & ~3);
/*  in shorts  */
	len1 = ((ss_y + 3) & ~3);
/*  in shorts  */
	size1 = len0 * height1 * 4;
/*  in shorts  */
	size2 = len1 * len0 * 4 + len1 * width * 2;

	s_buffer =
		(mlib_s16 *)__mlib_malloc((size1 + size2) * sizeof (mlib_s16));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height1 * len0;
	s_buf_trans = d_buffer + height1 * len0;
	d_buf_trans = s_buf_trans + height1 * len0;

	ss_buf_trans = d_buf_trans + height1 * len0;
	ds_buf_trans = ss_buf_trans + len1 * ss_x;
	sd_buf_trans = ds_buf_trans + len1 * dd_x;
	dd_buf_trans = sd_buf_trans + len1 * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S16_U8(s_buffer, len0, d_buffer,
		sp, width, height);

	mlib_MatrixTranspose_S16_S16_dst_na(s_buf_trans, s_buffer, height1,
		len0, len0);
	mlib_MatrixTranspose_S16_S16_dst_na(d_buf_trans, d_buffer, height1,
		len0, len0);

	mlib_TwoTen_1D_ForwardTransform_S16_S16_src_all(ss_buf_trans, len1,
		sd_buf_trans, s_buf_trans, height, ss_x, height1);

	mlib_TwoTen_1D_ForwardTransform_S16_S16_src_all(ds_buf_trans, len1,
		dd_buf_trans, d_buf_trans, height, dd_x, height1);

	mlib_MatrixTranspose_S16_S16_dst_na(ss_buffer, ss_buf_trans, ss_x, ss_y,
		len1);
	mlib_MatrixTranspose_S16_S16_dst_na(ds_buffer, ds_buf_trans, dd_x, ss_y,
		len1);
	mlib_MatrixTranspose_S16_S16_dst_na(sd_buffer, sd_buf_trans, ss_x,
		(height - ss_y), len1);
	mlib_MatrixTranspose_S16_S16_dst_na(dd_buffer, dd_buf_trans, dd_x,
		(height - ss_y), len1);

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
	mlib_s16 *s_buffer, *d_buffer, *s_buf_trans, *d_buf_trans;
	mlib_s16 *ss_buf_trans, *sd_buf_trans, *ds_buf_trans, *dd_buf_trans;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 size1, size2, len0, len1, height1;

/*  in shorts  */
	len0 = ((ss_x + 3) & ~3);
/*  to make matrix be processed by fast case (4x4)  */
	height1 = ((height + 3) & ~3);
/*  in shorts  */
	len1 = ((ss_y + 3) & ~3);
/*  in shorts  */
	size1 = len0 * height1 * 4;
/*  in shorts  */
	size2 = len1 * len0 * 4 + len1 * ss_x * 2 + len1 * dd_x * 2;

	s_buffer = (void *)__mlib_malloc((size1 + size2) * sizeof (mlib_s16));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height1 * len0;
	s_buf_trans = d_buffer + height1 * len0;
	d_buf_trans = s_buf_trans + height1 * len0;

	ss_buf_trans = d_buf_trans + height1 * len0;
	ds_buf_trans = ss_buf_trans + len1 * ss_x;
	sd_buf_trans = ds_buf_trans + len1 * dd_x;
	dd_buf_trans = sd_buf_trans + len1 * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S16_S16_src_na(s_buffer, len0, d_buffer,
		sp, width, height);

	mlib_MatrixTranspose_S16_S16_dst_na(s_buf_trans, s_buffer, height1,
		len0, len0);
	mlib_MatrixTranspose_S16_S16_dst_na(d_buf_trans, d_buffer, height1,
		len0, len0);

	mlib_TwoTen_1D_ForwardTransform_S16_S16_src_all(ss_buf_trans, len1,
		sd_buf_trans, s_buf_trans, height, ss_x, height1);

	mlib_TwoTen_1D_ForwardTransform_S16_S16_src_all(ds_buf_trans, len1,
		dd_buf_trans, d_buf_trans, height, dd_x, height1);

	mlib_MatrixTranspose_S16_S16_dst_na(ss_buffer, ss_buf_trans, ss_x, ss_y,
		len1);
	mlib_MatrixTranspose_S16_S16_dst_na(ds_buffer, ds_buf_trans, dd_x, ss_y,
		len1);
	mlib_MatrixTranspose_S16_S16_dst_na(sd_buffer, sd_buf_trans, ss_x,
		(height - ss_y), len1);
	mlib_MatrixTranspose_S16_S16_dst_na(dd_buffer, dd_buf_trans, dd_x,
		(height - ss_y), len1);

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
	mlib_s32 *s_buffer, *d_buffer, *s_buf_trans, *d_buf_trans;
	mlib_s32 *ss_buf_trans, *sd_buf_trans, *ds_buf_trans, *dd_buf_trans;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 size1, size2, len0, len1, height1;

/*  in ints  */
	len0 = ((ss_x + 1) & ~1);
	height1 = ((height + 1) & ~1);
/*  in ints  */
	len1 = ((ss_y + 1) & ~1);
/*  in ints  */
	size1 = len0 * height1 * 4;
/*  in ints  */
	size2 = len1 * len0 * 4 + len1 * ss_x * 2 + len1 * dd_x * 2;

	s_buffer =
		(mlib_s32 *)__mlib_malloc((size1 + size2) * sizeof (mlib_s32));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height1 * len0;
	s_buf_trans = d_buffer + height1 * len0;
	d_buf_trans = s_buf_trans + height1 * len0;

	ss_buf_trans = d_buf_trans + height1 * len0;
	ds_buf_trans = ss_buf_trans + len1 * ss_x;
	sd_buf_trans = ds_buf_trans + len1 * dd_x;
	dd_buf_trans = sd_buf_trans + len1 * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S32_S16(s_buffer, len0, d_buffer,
		sp, width, height);

	mlib_MatrixTranspose_S32_S32_dst_na(s_buf_trans, s_buffer, height1,
		len0, len0);
	mlib_MatrixTranspose_S32_S32_dst_na(d_buf_trans, d_buffer, height1,
		len0, len0);

	mlib_TwoTen_1D_ForwardTransform_S32_S32_src_all(ss_buf_trans, len1,
		sd_buf_trans, s_buf_trans, height, ss_x, height1);

	mlib_TwoTen_1D_ForwardTransform_S32_S32_src_all(ds_buf_trans, len1,
		dd_buf_trans, d_buf_trans, height, dd_x, height1);

	mlib_MatrixTranspose_S32_S32_dst_na(ss_buffer, ss_buf_trans, ss_x, ss_y,
		len1);
	mlib_MatrixTranspose_S32_S32_dst_na(ds_buffer, ds_buf_trans, dd_x, ss_y,
		len1);
	mlib_MatrixTranspose_S32_S32_dst_na(sd_buffer, sd_buf_trans, ss_x,
		(height - ss_y), len1);
	mlib_MatrixTranspose_S32_S32_dst_na(dd_buffer, dd_buf_trans, dd_x,
		(height - ss_y), len1);

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
	mlib_s32 *s_buffer, *d_buffer, *s_buf_trans, *d_buf_trans;
	mlib_s32 *ss_buf_trans, *sd_buf_trans, *ds_buf_trans, *dd_buf_trans;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1;
	mlib_s32 ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x;
	mlib_s32 dd_y = height - ss_y;
	mlib_s32 size1, size2, len0, len1, height1;

/*  in ints  */
	len0 = ((ss_x + 1) & ~1);
	height1 = ((height + 1) & ~1);
/*  in ints  */
	len1 = ((ss_y + 1) & ~1);
/*  in ints  */
	size1 = len0 * height1 * 4;
/*  in ints  */
	size2 = len1 * len0 * 4 + len1 * ss_x * 2 + len1 * dd_x * 2;

	s_buffer =
		(mlib_s32 *)__mlib_malloc((size1 + size2) * sizeof (mlib_s32));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);
	d_buffer = s_buffer + height1 * len0;
	s_buf_trans = d_buffer + height1 * len0;
	d_buf_trans = s_buf_trans + height1 * len0;

	ss_buf_trans = d_buf_trans + height1 * len0;
	ds_buf_trans = ss_buf_trans + len1 * ss_x;
	sd_buf_trans = ds_buf_trans + len1 * dd_x;
	dd_buf_trans = sd_buf_trans + len1 * ss_x;

	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S32_S32_src_na(s_buffer, len0, d_buffer,
		sp, width, height);

	mlib_MatrixTranspose_S32_S32_dst_na(s_buf_trans, s_buffer, height1,
		len0, len0);
	mlib_MatrixTranspose_S32_S32_dst_na(d_buf_trans, d_buffer, height1,
		len0, len0);

	mlib_TwoTen_1D_ForwardTransform_S32_S32_src_all(ss_buf_trans, len1,
		sd_buf_trans, s_buf_trans, height, ss_x, height1);

	mlib_TwoTen_1D_ForwardTransform_S32_S32_src_all(ds_buf_trans, len1,
		dd_buf_trans, d_buf_trans, height, dd_x, height1);

	mlib_MatrixTranspose_S32_S32_dst_na(ss_buffer, ss_buf_trans, ss_x, ss_y,
		len1);
	mlib_MatrixTranspose_S32_S32_dst_na(ds_buffer, ds_buf_trans, dd_x, ss_y,
		len1);
	mlib_MatrixTranspose_S32_S32_dst_na(sd_buffer, sd_buf_trans, ss_x,
		(height - ss_y), len1);
	mlib_MatrixTranspose_S32_S32_dst_na(dd_buffer, dd_buf_trans, dd_x,
		(height - ss_y), len1);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* Inverse transform */

/* *********************************************************** */

#define	TT_1D_INVERSE_1                                         \
	{                                                       \
	    mlib_s32 *c22, *c3, *c22_b =                        \
		    (mlib_s32 *)coeff_22 + 2, *c3_b =           \
		    (mlib_s32 *)coeff_3 + 2;                    \
	    mlib_s32 i, j, p, p1, p2;                           \
	                                                        \
	    pxx = xx_ptr;                                       \
	    pss = ss_ptr;                                       \
	    pdd = dd_ptr;                                       \
	    for (j = 0; j < ss_width; j++) {                    \
		s = pss;                                        \
		buf = buf_line;                                 \
		buf[-1] = *s;                                   \
		c22 = c22_b;                                    \
		c3 = c3_b

/* *********************************************************** */

#define	TT_1D_INVERSE_2                                         \
	for (i = 0; i < ss_height; i++) {                       \
	    (*buf++) = *s;                                      \
	    (*c3++) = *s * 3;                                   \
	    (*c22++) = *s * 22;                                 \
	    s += ss_width;                                      \
	}                                                       \
	                                                        \
	buf[0] = buf[-1];                                       \
	buf[1] = buf[-2];                                       \
	c3[0] = buf[-1] * 3;                                    \
	c3[1] = buf[-2] * 3;                                    \
	c22[0] = buf[-1] * 22;                                  \
	c22[1] = buf[-2] * 22;                                  \
	buf = buf_line;                                         \
	c3 = c3_b;                                              \
	c22 = c22_b;                                            \
	buf[-1] = buf[0];                                       \
	buf[-2] = buf[1];                                       \
	c3[-1] = buf[0] * 3;                                    \
	c3[-2] = buf[1] * 3;                                    \
	c22[-1] = buf[0] * 22;                                  \
	c22[-2] = buf[1] * 22;                                  \
	d = pdd;                                                \
	x = pxx;                                                \
	s = buf

/* *********************************************************** */

#define	TT_1D_INVERSE_3                                         \
	    for (i = 0; i < dd_height; i++) {                   \
		p1 = c3[i - 2] - c3[i + 2];                     \
		p2 = c22[i + 1] - c22[i - 1];                   \
		p = (p1 + p2 + 32) >> 6;                        \
		x[0] = *s + ((*d - p + 1) >> 1);                \
		x[1] = *s - ((*d - p) >> 1);                    \
		x += 2;                                         \
		s++;                                            \
		d += dd_width;                                  \
	    }                                                   \
	    if (dd_height != ss_height)                         \
		*x = *s;                                        \
	    pss++;                                              \
	    pdd++;                                              \
	    pxx += xx_width;                                    \
	}                                                       \
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
	mlib_s16 *buf, *buf_line = (mlib_s16 *)l_buffer + 2, *s, *d, *pss, *pdd;
	mlib_u8 *x, *pxx;

	TT_1D_INVERSE_1;
#pragma pipeloop(0)
	TT_1D_INVERSE_2;
#pragma pipeloop(0)
	TT_1D_INVERSE_3;
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
	mlib_s16 *buf, *buf_line = (mlib_s16 *)l_buffer + 2, *s, *d, *pss, *pdd;
	mlib_s16 *x, *pxx;

	TT_1D_INVERSE_1;
#pragma pipeloop(0)
	TT_1D_INVERSE_2;
#pragma pipeloop(0)
	TT_1D_INVERSE_3;
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
	mlib_s32 *buf, *buf_line = (mlib_s32 *)l_buffer + 2, *s, *d, *pss, *pdd;
	mlib_s16 *x, *pxx;

	TT_1D_INVERSE_1;
#pragma pipeloop(0)
	TT_1D_INVERSE_2;
#pragma pipeloop(0)
	TT_1D_INVERSE_3;
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
	mlib_s32 *buf, *buf_line = (mlib_s32 *)l_buffer + 2, *s, *d, *pss, *pdd;
	mlib_s32 *x, *pxx;

	TT_1D_INVERSE_1;
#pragma pipeloop(0)
	TT_1D_INVERSE_2;
#pragma pipeloop(0)
	TT_1D_INVERSE_3;
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
