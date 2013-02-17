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

#pragma ident	"@(#)mlib_v_ImageSobel_U8.c	9.2	07/11/05 SMI"

/*
 * VIS functions for mlib_ImageSobel
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageSobel.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageSqrtTable.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

static mlib_status mlib_ImageSobel_U8_2_Sep(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask);

static mlib_status mlib_ImageSobel_U8_4_Sep(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask);

/* *********************************************************** */

#define	PROCESS_CHANNEL(k)	(cmask & (1 << (nchan - (k) - 1)))

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s16 s3;
		mlib_s16 s2;
		mlib_s16 s1;
		mlib_s16 s0;
	} s16;
	struct
	{
		mlib_s16 u16_0;
		mlib_s16 u16_1;
		mlib_s16 u16_2;
		mlib_s16 u16_3;
	} u16;
	struct
	{
		mlib_s32 s32_0;
		mlib_s32 s32_1;
	} s32;
} type_d64_4s16;

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_1(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_s32 i, j, d0, d1, d2, incr, d, diap, v16, n, nchan = 1;
	const mlib_u8 *sp0, *sp1, *sp2;
	mlib_u8 *dp;
	mlib_d64 v, h, vp, vn, v0, v1, vps, hp, hn, h0, h1, hps;
	mlib_d64 r0, r1, r2, r3, r01, r23, s0, s1, s2, res;
	type_d64_4s16 tmp_data;
	mlib_f32 fone = vis_to_float(0x100), ftwo =
	    vis_to_float(0x200), a0, a1, a2;
	mlib_s32 bsize1 = ((dw + 16) * 3) & ~7;
	mlib_s32 bsize2 = (dw + 16) * 4;
	mlib_f32 *line = __mlib_malloc(bsize1 + bsize2);

	if (line == NULL)
		return (MLIB_FAILURE);

/* initialize two line buffers */
	incr = ((dw >> 3) << 1) + 2;
	d0 = 0;
	d1 = incr;
	d2 = incr * 2;

	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;

	mlib_ImageCopy_na(sp0 + 1, (void *)(line + d0), dw);
	mlib_ImageCopy_na(sp1 + 1, (void *)(line + d1), dw);

	for (i = 0; i < dh; i++) {

/* initialize line buffer */
		mlib_ImageCopy_na(sp2 + 1, (void *)(line + d2), dw);

		vis_write_gsr((15 << 3) | 6);

/* initialize column subsums for two first pixels */
/* t0 = sp0[0] + sp1[0] + sp1[0] + sp2[0]; */
/* t1 = sp0[-nchan] + sp1[-nchan] + sp1[-nchan] + sp2[-nchan]; */
/* vp = vis_to_double(0, (t1 << 16) | t0); */

		tmp_data.s16.s0 = sp0[0] + sp1[0] + sp1[0] + sp2[0];
		tmp_data.s16.s1 =
		    sp0[-nchan] + sp1[-nchan] + sp1[-nchan] + sp2[-nchan];
		vp = tmp_data.d64;

/* t0 = sp2[0] - sp0[0]; */
/* t1 = sp2[-nchan] - sp0[-nchan]; */
/* hp = vis_to_double(0, (t1 << 16) | t0); */

		tmp_data.s16.s0 = sp2[0] - sp0[0];
		tmp_data.s16.s1 = sp2[-nchan] - sp0[-nchan];
		hp = tmp_data.d64;

/* prepare shifted hp and vp */
		hps = vis_faligndata(hp, hp);
		vps = vis_faligndata(vp, vp);

#pragma pipeloop(0)
#pragma nomemorydepend
		for (j = 0; j < (dw >> 2); j++) {

			a0 = line[d0 + j];
			a1 = line[d1 + j];
			a2 = line[d2 + j];

			s0 = vis_fmul8x16al(a0, fone);
			s1 = vis_fmul8x16al(a1, ftwo);
			s2 = vis_fmul8x16al(a2, fone);

			hn = vis_fpsub16(s2, s0);
			h1 = vis_faligndata(hp, hn);
			h0 = vis_faligndata(hps, h1);
			h = vis_fpadd16(vis_fpadd16(h0, hn), vis_fpadd16(h1,
			    h1));
			r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
			    vis_read_hi(h)), vis_fmuld8ulx16(vis_read_hi(h),
			    vis_read_hi(h)));
			r3 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(h),
			    vis_read_lo(h)), vis_fmuld8ulx16(vis_read_lo(h),
			    vis_read_lo(h)));

			vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
			v1 = vis_faligndata(vp, vn);
			v0 = vis_faligndata(vps, v1);
			v = vis_fpsub16(vn, v0);
			r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
			    vis_read_hi(v)), vis_fmuld8ulx16(vis_read_hi(v),
			    vis_read_hi(v)));
			r2 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(v),
			    vis_read_lo(v)), vis_fmuld8ulx16(vis_read_lo(v),
			    vis_read_lo(v)));

			r01 = vis_fpadd32(r0, r1);
			r23 = vis_fpadd32(r2, r3);

			res = vis_fpackfix_pair(r01, r23);
			res = vis_fpadd16(res, res);

			hps = h1;
			vps = v1;
			hp = hn;
			vp = vn;

			v16 = ((mlib_u16 *)&res)[0];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&res)[1];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 1] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&res)[2];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 2] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&res)[3];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 3] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];
		}

		a0 = line[d0 + j];
		a1 = line[d1 + j];
		a2 = line[d2 + j];

		s0 = vis_fmul8x16al(a0, fone);
		s1 = vis_fmul8x16al(a1, ftwo);
		s2 = vis_fmul8x16al(a2, fone);

		hn = vis_fpsub16(s2, s0);
		h1 = vis_faligndata(hp, hn);
		h0 = vis_faligndata(hps, h1);
		h = vis_fpadd16(vis_fpadd16(h0, hn), vis_fpadd16(h1, h1));
		r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
		    vis_read_hi(h)), vis_fmuld8ulx16(vis_read_hi(h),
		    vis_read_hi(h)));
		r3 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(h),
		    vis_read_lo(h)), vis_fmuld8ulx16(vis_read_lo(h),
		    vis_read_lo(h)));

		vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
		v1 = vis_faligndata(vp, vn);
		v0 = vis_faligndata(vps, v1);
		v = vis_fpsub16(vn, v0);
		r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
		    vis_read_hi(v)), vis_fmuld8ulx16(vis_read_hi(v),
		    vis_read_hi(v)));
		r2 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(v),
		    vis_read_lo(v)), vis_fmuld8ulx16(vis_read_lo(v),
		    vis_read_lo(v)));

		r01 = vis_fpadd32(r0, r1);
		r23 = vis_fpadd32(r2, r3);

		res = vis_fpackfix_pair(r01, r23);
		res = vis_fpadd16(res, res);

		hps = h1;
		vps = v1;
		hp = hn;
		vp = vn;

		n = dw & 3;

		if (n) {
			v16 = ((mlib_u16 *)&res)[0];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			if (n > 1) {
				v16 = ((mlib_u16 *)&res)[1];
				diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
				dp[4 * j + 1] =
				    mlib_SqrtTable_U8[((v16) >> (diap)) +
				    (diap)];

				if (n > 2) {
					v16 = ((mlib_u16 *)&res)[2];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[4 * j + 2] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];
				}
			}
		}

/* swap line buffers */
		d = d0;
		d0 = d1;
		d1 = d2;
		d2 = d;

/* increment row pointers */
		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}

	__mlib_free(line);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_2(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_d64 hp, hn, h1, r0, r1, r2, r3, r01, r23, s0, s1, s2, res, v, h,
	    vp, vn;
	mlib_f32 fone = vis_to_float(0x100), ftwo = vis_to_float(0x200), fres;
	mlib_s32 d0, d1, d2, incr, d, diap, v16, i, j, nchan = 2;
	const mlib_u8 *sp0, *sp1, *sp2;
	mlib_u8 *dp;
	type_d64_4s16 tmp_data;
	mlib_s32 bsize1, bsize2;
	mlib_f32 *line;

	if ((cmask & 3) != 3) {
		return mlib_ImageSobel_U8_2_Sep(da, sa, dlb, slb, dw, dh,
		    cmask);
	}

	bsize1 = ((dw + 16) * 3 * 2) & ~7;
	bsize2 = (dw + 16) * 4 * 2;
	line = __mlib_malloc(bsize1 + bsize2);

	if (line == NULL)
		return (MLIB_FAILURE);

/* initialize two line buffers */
	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;

	incr = ((dw >> 2) << 1) + 2;
	d0 = 0;
	d1 = incr;
	d2 = incr * 2;

	mlib_ImageCopy_na(sp0 + 2, (void *)(line + d0), dw * 2);
	mlib_ImageCopy_na(sp1 + 2, (void *)(line + d1), dw * 2);

/* row loop */
	for (i = 0; i < dh; i++) {

/* initialize line buffer */
		mlib_ImageCopy_na(sp2 + 2, (void *)(line + d2), dw * 2);
		vis_write_gsr((15 << 3) | 4);

/* initialize column subsums for two first pixels */
		tmp_data.s16.s1 = sp0[0] + sp1[0] + sp1[0] + sp2[0];
		tmp_data.s16.s3 =
		    sp0[-nchan] + sp1[-nchan] + sp1[-nchan] + sp2[-nchan];
		tmp_data.s16.s0 = sp0[1] + sp1[1] + sp1[1] + sp2[1];
		tmp_data.s16.s2 =
		    sp0[-nchan + 1] + sp1[-nchan + 1] + sp1[-nchan + 1] +
		    sp2[-nchan + 1];
		vp = tmp_data.d64;
		tmp_data.s16.s1 = sp2[0] - sp0[0];
		tmp_data.s16.s3 = sp2[-nchan] - sp0[-nchan];
		tmp_data.s16.s0 = sp2[+1] - sp0[+1];
		tmp_data.s16.s2 = sp2[-nchan + 1] - sp0[-nchan + 1];
		hp = tmp_data.d64;

#pragma pipeloop(0)
#pragma nomemorydepend
		for (j = 0; j < (dw >> 1); j++) {

			s0 = vis_fmul8x16al(line[d0 + j], fone);
			s1 = vis_fmul8x16al(line[d1 + j], ftwo);
			s2 = vis_fmul8x16al(line[d2 + j], fone);

/* calculate column subsums for vert & horiz. matrixes */
			vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
			hn = vis_fpsub16(s2, s0);

/* form column subsums for two previous pixels */
			h1 = vis_faligndata(hp, hn);

/* calculate results for vert. & horiz. matrixes */
			v = vis_fpsub16(vn, vp);
			h = vis_fpadd16(vis_fpadd16(hp, hn), vis_fpadd16(h1,
			    h1));

/* calculate v**2+h**2 for pixels 0 & 1 */
			r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
			    vis_read_hi(v)), vis_fmuld8ulx16(vis_read_hi(v),
			    vis_read_hi(v)));
			r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
			    vis_read_hi(h)), vis_fmuld8ulx16(vis_read_hi(h),
			    vis_read_hi(h)));
			r01 = vis_fpadd32(r0, r1);

/* calculate v**2+h**2 for pixels 2 & 3 */
			r2 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(v),
			    vis_read_lo(v)), vis_fmuld8ulx16(vis_read_lo(v),
			    vis_read_lo(v)));
			r3 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(h),
			    vis_read_lo(h)), vis_fmuld8ulx16(vis_read_lo(h),
			    vis_read_lo(h)));
			r23 = vis_fpadd32(r2, r3);

			res = vis_fpackfix_pair(r01, r23);
			res = vis_fpadd16(res, res);

			v16 = ((mlib_u16 *)&res)[0];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&res)[1];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 1] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&res)[2];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 2] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&res)[3];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 3] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			hp = hn;
			vp = vn;
		}

		s0 = vis_fmul8x16al(line[d0 + j], fone);
		s1 = vis_fmul8x16al(line[d1 + j], ftwo);
		s2 = vis_fmul8x16al(line[d2 + j], fone);

/* calculate column subsums for vert & horiz. matrixes */
		vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
		hn = vis_fpsub16(s2, s0);

/* form column subsums for two previous pixels */
		h1 = vis_faligndata(hp, hn);

/* calculate results for vert. & horiz. matrixes */
		v = vis_fpsub16(vn, vp);
		h = vis_fpadd16(vis_fpadd16(hp, hn), vis_fpadd16(h1, h1));

/* calculate v**2+h**2 for pixels 0 & 1 */
		r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
		    vis_read_hi(v)), vis_fmuld8ulx16(vis_read_hi(v),
		    vis_read_hi(v)));
		r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
		    vis_read_hi(h)), vis_fmuld8ulx16(vis_read_hi(h),
		    vis_read_hi(h)));
		r01 = vis_fpadd32(r0, r1);
		fres = vis_fpackfix(r01);
		fres = vis_fpadd16s(fres, fres);

		if (dw & 1) {

			v16 = ((mlib_u16 *)&fres)[0];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];

			v16 = ((mlib_u16 *)&fres)[1];
			diap = mlib_SqrtTable_U8_Diap[(v16) >> 8];
			dp[4 * j + 1] =
			    mlib_SqrtTable_U8[((v16) >> (diap)) + (diap)];
		}

/* swap line buffers */
		d = d0;
		d0 = d1;
		d1 = d2;
		d2 = d;

/* increment row pointers */
		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}

	__mlib_free(line);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_2_Sep(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_s32 i, j, k, d0, d1, d2, incr, d, diap, v16, n, nchan = 2;
	const mlib_u8 *sp0, *sp1, *sp2;
	mlib_u8 *dp;
	mlib_d64 v, h, vp, vn, v0, v1, vps, hp, hn, h0, h1, hps;
	mlib_d64 r0, r1, r2, r3, r01, r23, s0, s1, s2, res;
	type_d64_4s16 tmp_data;
	mlib_f32 fone = vis_to_float(0x100), ftwo =
	    vis_to_float(0x200), a0, a1, a2;
	mlib_s32 bsize1 = ((dw + 16) * 3 * 2) & ~7;
	mlib_s32 bsize2 = (dw + 16) * 4;
	mlib_f32 *line = __mlib_malloc(bsize1 + bsize2);

	if (line == NULL)
		return (MLIB_FAILURE);

/* initialize two line buffers */
	incr = ((dw >> 3) << 1) + 2;
	d0 = 0;
	d1 = incr;
	d2 = incr * 2;

	for (k = 0; k < nchan; k++) {
		if (PROCESS_CHANNEL(k)) {

			sp0 = sa - slb + k;
			sp1 = sa + k;
			sp2 = sa + slb + k;
			dp = da + k;

			mlib_v_ImageChannelExtract_U8_21_D1(sp0 + nchan,
			    (void *)(line + d0), dw, 2);
			mlib_v_ImageChannelExtract_U8_21_D1(sp1 + nchan,
			    (void *)(line + d1), dw, 2);

/* row loop */
			for (i = 0; i < dh; i++) {

/* initialize line buffer */
				mlib_v_ImageChannelExtract_U8_21_D1(sp2 + nchan,
				    (void *)(line + d2), dw, 2);

				vis_write_gsr((15 << 3) | 6);

/* initialize column subsums for two first pixels */
				tmp_data.s16.s0 =
				    sp0[0] + sp1[0] + sp1[0] + sp2[0];
				tmp_data.s16.s1 =
				    sp0[-nchan] + sp1[-nchan] + sp1[-nchan] +
				    sp2[-nchan];
				vp = tmp_data.d64;

				tmp_data.s16.s0 = sp2[0] - sp0[0];
				tmp_data.s16.s1 = sp2[-nchan] - sp0[-nchan];
				hp = tmp_data.d64;

/* prepare shifted hp and vp */
				hps = vis_faligndata(hp, hp);
				vps = vis_faligndata(vp, vp);

#pragma pipeloop(0)
#pragma nomemorydepend
				for (j = 0; j < (dw >> 2); j++) {

					a0 = line[d0 + j];
					a1 = line[d1 + j];
					a2 = line[d2 + j];

					s0 = vis_fmul8x16al(a0, fone);
					s1 = vis_fmul8x16al(a1, ftwo);
					s2 = vis_fmul8x16al(a2, fone);

					hn = vis_fpsub16(s2, s0);
					h1 = vis_faligndata(hp, hn);
					h0 = vis_faligndata(hps, h1);
					h = vis_fpadd16(vis_fpadd16(h0, hn),
					    vis_fpadd16(h1, h1));
					r1 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_hi(h), vis_read_hi(h)),
					    vis_fmuld8ulx16(vis_read_hi(h),
					    vis_read_hi(h)));
					r3 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_lo(h), vis_read_lo(h)),
					    vis_fmuld8ulx16(vis_read_lo(h),
					    vis_read_lo(h)));

					vn = vis_fpadd16(vis_fpadd16(s0, s2),
					    s1);
					v1 = vis_faligndata(vp, vn);
					v0 = vis_faligndata(vps, v1);
					v = vis_fpsub16(vn, v0);
					r0 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_hi(v), vis_read_hi(v)),
					    vis_fmuld8ulx16(vis_read_hi(v),
					    vis_read_hi(v)));
					r2 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_lo(v), vis_read_lo(v)),
					    vis_fmuld8ulx16(vis_read_lo(v),
					    vis_read_lo(v)));

					r01 = vis_fpadd32(r0, r1);
					r23 = vis_fpadd32(r2, r3);

					res = vis_fpackfix_pair(r01, r23);
					res = vis_fpadd16(res, res);

					hps = h1;
					vps = v1;
					hp = hn;
					vp = vn;

					v16 = ((mlib_u16 *)&res)[0];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[8 * j] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[1];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[8 * j + 2] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[2];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[8 * j + 4] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[3];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[8 * j + 6] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];
				}

				s0 = vis_fmul8x16al(line[d0 + j], fone);
				s1 = vis_fmul8x16al(line[d1 + j], ftwo);
				s2 = vis_fmul8x16al(line[d2 + j], fone);

				hn = vis_fpsub16(s2, s0);
				h1 = vis_faligndata(hp, hn);
				h0 = vis_faligndata(hps, h1);
				h = vis_fpadd16(vis_fpadd16(h0, hn),
				    vis_fpadd16(h1, h1));
				r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
				    vis_read_hi(h)),
				    vis_fmuld8ulx16(vis_read_hi(h),
				    vis_read_hi(h)));
				r3 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(h),
				    vis_read_lo(h)),
				    vis_fmuld8ulx16(vis_read_lo(h),
				    vis_read_lo(h)));

				vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
				v1 = vis_faligndata(vp, vn);
				v0 = vis_faligndata(vps, v1);
				v = vis_fpsub16(vn, v0);
				r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
				    vis_read_hi(v)),
				    vis_fmuld8ulx16(vis_read_hi(v),
				    vis_read_hi(v)));
				r2 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(v),
				    vis_read_lo(v)),
				    vis_fmuld8ulx16(vis_read_lo(v),
				    vis_read_lo(v)));

				r01 = vis_fpadd32(r0, r1);
				r23 = vis_fpadd32(r2, r3);

				res = vis_fpackfix_pair(r01, r23);
				res = vis_fpadd16(res, res);

				n = dw & 3;

				if (n) {
					v16 = ((mlib_u16 *)&res)[0];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[8 * j] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					if (n > 1) {
						v16 = ((mlib_u16 *)&res)[1];
						diap =
						    mlib_SqrtTable_U8_Diap[(v16)
						    >> 8];
						dp[8 * j + 2] =
						    mlib_SqrtTable_U8[((v16) >>
						    (diap)) + (diap)];

						if (n > 2) {
						    v16 = ((mlib_u16 *)&res)[2];
						    diap =
							mlib_SqrtTable_U8_Diap
							[(v16) >> 8];
						    dp[8 * j + 4] =
							mlib_SqrtTable_U8[(
							(v16) >> (diap)) +
							(diap)];
						}
					}
				}

/* swap line buffers */
				d = d0;
				d0 = d1;
				d1 = d2;
				d2 = d;

/* increment row pointers */
				sp0 += slb;
				sp1 += slb;
				sp2 += slb;
				dp += dlb;
			}
		}
	}

	__mlib_free(line);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_4_Sep(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_d64 hp, hn, h0, h1, hps, r0, r1, r2, r3, r01, r23, s0, s1, s2;
	mlib_d64 v, h, res, vp, vn, v0, v1, vps;
	mlib_f32 fone = vis_to_float(0x100), ftwo =
	    vis_to_float(0x200), a0, a1, a2;
	mlib_s32 d0, d1, d2, incr, d, diap, v16, n, i, j, k, nchan = 4;
	const mlib_u8 *sp0, *sp1, *sp2;
	mlib_u8 *dp;
	type_d64_4s16 tmp_data;
	mlib_s32 bsize1 = ((dw + 16) * 3) & ~7;
	mlib_s32 bsize2 = (dw + 16) * 4;
	mlib_f32 *line = __mlib_malloc(bsize1 + bsize2);

	if (line == NULL)
		return (MLIB_FAILURE);

/* initialize two line buffers */
	incr = ((dw >> 3) << 1) + 2;
	d0 = 0;
	d1 = incr;
	d2 = incr * 2;

	for (k = 0; k < nchan; k++) {
		if (PROCESS_CHANNEL(k)) {

			sp0 = sa - slb + k;
			sp1 = sa + k;
			sp2 = sa + slb + k;
			dp = da + k;

			mlib_v_ImageChannelExtract_U8_41_D1(sp0 + nchan,
			    (void *)(line + d0), dw, 8);
			mlib_v_ImageChannelExtract_U8_41_D1(sp1 + nchan,
			    (void *)(line + d1), dw, 8);

/* row loop */
			for (i = 0; i < dh; i++) {
/* initialize line buffer */
				mlib_v_ImageChannelExtract_U8_41_D1(sp2 + nchan,
				    (void *)(line + d2), dw, 8);

/* initialize column subsums for two first pixels */
				tmp_data.s16.s0 = *sp0 + *sp1 + *sp1 + *sp2;
				tmp_data.s16.s1 =
				    *(sp0 - nchan) + *(sp1 - nchan) + *(sp1 -
				    nchan) + *(sp2 - nchan);
				vp = tmp_data.d64;
				tmp_data.s16.s0 = *sp2 - *sp0;
				tmp_data.s16.s1 =
				    *(sp2 - nchan) - *(sp0 - nchan);
				hp = tmp_data.d64;

/* prepare gsr value for obtaining h0, h1, v0 */
				vis_write_gsr((15 << 3) | 6);

/* prepare shifted hp and vp */
				hps = vis_faligndata(hp, hp);
				vps = vis_faligndata(vp, vp);

#pragma pipeloop(0)
#pragma nomemorydepend
				for (j = 0; j < (dw >> 2); j++) {

					a0 = line[d0 + j];
					a1 = line[d1 + j];
					a2 = line[d2 + j];

					s0 = vis_fmul8x16al(a0, fone);
					s1 = vis_fmul8x16al(a1, ftwo);
					s2 = vis_fmul8x16al(a2, fone);

/* calculate column subsums for vert & horiz. matrixes */
					vn = vis_fpadd16(vis_fpadd16(s0, s2),
					    s1);
					hn = vis_fpsub16(s2, s0);

/* form column subsums for two previous pixels */
					h1 = vis_faligndata(hp, hn);
					h0 = vis_faligndata(hps, h1);
					v1 = vis_faligndata(vp, vn);
					v0 = vis_faligndata(vps, v1);

/* calculate results for vert. & horiz. matrixes */
					v = vis_fpsub16(vn, v0);
					h = vis_fpadd16(vis_fpadd16(h0, hn),
					    vis_fpadd16(h1, h1));

/* calculate v**2+h**2 for pixels 0 & 1 */
					r0 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_hi(v), vis_read_hi(v)),
					    vis_fmuld8ulx16(vis_read_hi(v),
					    vis_read_hi(v)));
					r1 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_hi(h), vis_read_hi(h)),
					    vis_fmuld8ulx16(vis_read_hi(h),
					    vis_read_hi(h)));
					r01 = vis_fpadd32(r0, r1);

/* calculate v**2+h**2 for pixels 2 & 3 */
					r2 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_lo(v), vis_read_lo(v)),
					    vis_fmuld8ulx16(vis_read_lo(v),
					    vis_read_lo(v)));
					r3 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_lo(h), vis_read_lo(h)),
					    vis_fmuld8ulx16(vis_read_lo(h),
					    vis_read_lo(h)));
					r23 = vis_fpadd32(r2, r3);

					res = vis_fpackfix_pair(r01, r23);
					res = vis_fpadd16(res, res);

					v16 = ((mlib_u16 *)&res)[0];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[16 * j] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[1];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[16 * j + 4] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[2];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[16 * j + 8] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[3];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[16 * j + 12] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					hps = h1;
					vps = v1;
					hp = hn;
					vp = vn;
				}

				a0 = line[d0 + j];
				a1 = line[d1 + j];
				a2 = line[d2 + j];

				s0 = vis_fmul8x16al(a0, fone);
				s1 = vis_fmul8x16al(a1, ftwo);
				s2 = vis_fmul8x16al(a2, fone);

/* calculate column subsums for vert & horiz. matrixes */
				vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
				hn = vis_fpsub16(s2, s0);

/* form column subsums for two previous pixels */
				h1 = vis_faligndata(hp, hn);
				h0 = vis_faligndata(hps, h1);
				v1 = vis_faligndata(vp, vn);
				v0 = vis_faligndata(vps, v1);

/* calculate results for vert. & horiz. matrixes */
				v = vis_fpsub16(vn, v0);
				h = vis_fpadd16(vis_fpadd16(h0, hn),
				    vis_fpadd16(h1, h1));

/* calculate v**2+h**2 for pixels 0 & 1 */
				r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
				    vis_read_hi(v)),
				    vis_fmuld8ulx16(vis_read_hi(v),
				    vis_read_hi(v)));
				r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
				    vis_read_hi(h)),
				    vis_fmuld8ulx16(vis_read_hi(h),
				    vis_read_hi(h)));
				r01 = vis_fpadd32(r0, r1);

/* calculate v**2+h**2 for pixels 2 & 3 */
				r2 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(v),
				    vis_read_lo(v)),
				    vis_fmuld8ulx16(vis_read_lo(v),
				    vis_read_lo(v)));
				r3 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(h),
				    vis_read_lo(h)),
				    vis_fmuld8ulx16(vis_read_lo(h),
				    vis_read_lo(h)));
				r23 = vis_fpadd32(r2, r3);

				res = vis_fpackfix_pair(r01, r23);
				res = vis_fpadd16(res, res);

				n = dw & 3;

				if (n) {

					v16 = ((mlib_u16 *)&res)[0];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[16 * j] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					if (n > 1) {

						v16 = ((mlib_u16 *)&res)[1];
						diap =
						    mlib_SqrtTable_U8_Diap[(v16)
						    >> 8];
						dp[16 * j + 4] =
						    mlib_SqrtTable_U8[((v16) >>
						    (diap)) + (diap)];

						if (n > 2) {
						    v16 = ((mlib_u16 *)&res)[2];
						    diap =
							mlib_SqrtTable_U8_Diap
							[(v16) >> 8];
						    dp[16 * j + 8] =
							mlib_SqrtTable_U8[(
							(v16) >> (diap)) +
							(diap)];
						}
					}
				}

/* swap line buffers */
				d = d0;
				d0 = d1;
				d1 = d2;
				d2 = d;

/* increment row pointers */
				sp0 += slb;
				sp1 += slb;
				sp2 += slb;
				dp += dlb;
			}
		}
	}

	__mlib_free(line);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_3(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_d64 hp, hn, h0, h1, hps, r0, r1, r2, r3, r01, r23, s0, s1, s2;
	mlib_d64 v, h, res, vp, vn, v0, v1, vps;
	mlib_f32 fone = vis_to_float(0x100), ftwo =
	    vis_to_float(0x200), a0, a1, a2;
	mlib_s32 d0, d1, d2, incr, d, diap, v16, n, i, j, k, nchan = 3;
	const mlib_u8 *sp0, *sp1, *sp2;
	mlib_u8 *dp;
	type_d64_4s16 tmp_data;
	mlib_s32 bsize1 = ((dw + 16) * 3) & ~7;
	mlib_s32 bsize2 = (dw + 16) * 4;
	mlib_f32 *line = __mlib_malloc(bsize1 + bsize2);

	if (line == NULL)
		return (MLIB_FAILURE);

/* initialize two line buffers */
	incr = ((dw >> 3) << 1) + 2;
	d0 = 0;
	d1 = incr;
	d2 = incr * 2;

	for (k = 0; k < nchan; k++) {
		if (PROCESS_CHANNEL(k)) {

			sp0 = sa - slb + k;
			sp1 = sa + k;
			sp2 = sa + slb + k;
			dp = da + k;

			mlib_v_ImageChannelExtract_U8_31_D1(sp0 + nchan,
			    (void *)(line + d0), dw, 4);
			mlib_v_ImageChannelExtract_U8_31_D1(sp1 + nchan,
			    (void *)(line + d1), dw, 4);

/* row loop */
			for (i = 0; i < dh; i++) {
/* initialize line buffer */
				mlib_v_ImageChannelExtract_U8_31_D1(sp2 + nchan,
				    (void *)(line + d2), dw, 4);

/* initialize column subsums for two first pixels */
				tmp_data.s16.s0 = *sp0 + *sp1 + *sp1 + *sp2;
				tmp_data.s16.s1 =
				    *(sp0 - nchan) + *(sp1 - nchan) + *(sp1 -
				    nchan) + *(sp2 - nchan);
				vp = tmp_data.d64;
				tmp_data.s16.s0 = *sp2 - *sp0;
				tmp_data.s16.s1 =
				    *(sp2 - nchan) - *(sp0 - nchan);
				hp = tmp_data.d64;

/* prepare gsr value for obtaining h0, h1, v0 */
				vis_write_gsr((15 << 3) | 6);

/* prepare shifted hp and vp */
				hps = vis_faligndata(hp, hp);
				vps = vis_faligndata(vp, vp);

#pragma pipeloop(0)
#pragma nomemorydepend
				for (j = 0; j < (dw >> 2); j++) {

					a0 = line[d0 + j];
					a1 = line[d1 + j];
					a2 = line[d2 + j];

					s0 = vis_fmul8x16al(a0, fone);
					s1 = vis_fmul8x16al(a1, ftwo);
					s2 = vis_fmul8x16al(a2, fone);

/* calculate column subsums for vert & horiz. matrixes */
					vn = vis_fpadd16(vis_fpadd16(s0, s2),
					    s1);
					hn = vis_fpsub16(s2, s0);

/* form column subsums for two previous pixels */
					h1 = vis_faligndata(hp, hn);
					h0 = vis_faligndata(hps, h1);
					v1 = vis_faligndata(vp, vn);
					v0 = vis_faligndata(vps, v1);

/* calculate results for vert. & horiz. matrixes */
					v = vis_fpsub16(vn, v0);
					h = vis_fpadd16(vis_fpadd16(h0, hn),
					    vis_fpadd16(h1, h1));

/* calculate v**2+h**2 for pixels 0 & 1 */
					r0 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_hi(v), vis_read_hi(v)),
					    vis_fmuld8ulx16(vis_read_hi(v),
					    vis_read_hi(v)));
					r1 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_hi(h), vis_read_hi(h)),
					    vis_fmuld8ulx16(vis_read_hi(h),
					    vis_read_hi(h)));
					r01 = vis_fpadd32(r0, r1);

/* calculate v**2+h**2 for pixels 2 & 3 */
					r2 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_lo(v), vis_read_lo(v)),
					    vis_fmuld8ulx16(vis_read_lo(v),
					    vis_read_lo(v)));
					r3 = vis_fpadd32(vis_fmuld8sux16
					    (vis_read_lo(h), vis_read_lo(h)),
					    vis_fmuld8ulx16(vis_read_lo(h),
					    vis_read_lo(h)));
					r23 = vis_fpadd32(r2, r3);

					res = vis_fpackfix_pair(r01, r23);
					res = vis_fpadd16(res, res);

					v16 = ((mlib_u16 *)&res)[0];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[12 * j] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[1];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[12 * j + 3] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[2];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[12 * j + 6] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					v16 = ((mlib_u16 *)&res)[3];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[12 * j + 9] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					hps = h1;
					vps = v1;
					hp = hn;
					vp = vn;
				}

				a0 = line[d0 + j];
				a1 = line[d1 + j];
				a2 = line[d2 + j];

				s0 = vis_fmul8x16al(a0, fone);
				s1 = vis_fmul8x16al(a1, ftwo);
				s2 = vis_fmul8x16al(a2, fone);

/* calculate column subsums for vert & horiz. matrixes */
				vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
				hn = vis_fpsub16(s2, s0);

/* form column subsums for two previous pixels */
				h1 = vis_faligndata(hp, hn);
				h0 = vis_faligndata(hps, h1);
				v1 = vis_faligndata(vp, vn);
				v0 = vis_faligndata(vps, v1);

/* calculate results for vert. & horiz. matrixes */
				v = vis_fpsub16(vn, v0);
				h = vis_fpadd16(vis_fpadd16(h0, hn),
				    vis_fpadd16(h1, h1));

/* calculate v**2+h**2 for pixels 0 & 1 */
				r0 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(v),
				    vis_read_hi(v)),
				    vis_fmuld8ulx16(vis_read_hi(v),
				    vis_read_hi(v)));
				r1 = vis_fpadd32(vis_fmuld8sux16(vis_read_hi(h),
				    vis_read_hi(h)),
				    vis_fmuld8ulx16(vis_read_hi(h),
				    vis_read_hi(h)));
				r01 = vis_fpadd32(r0, r1);

/* calculate v**2+h**2 for pixels 2 & 3 */
				r2 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(v),
				    vis_read_lo(v)),
				    vis_fmuld8ulx16(vis_read_lo(v),
				    vis_read_lo(v)));
				r3 = vis_fpadd32(vis_fmuld8sux16(vis_read_lo(h),
				    vis_read_lo(h)),
				    vis_fmuld8ulx16(vis_read_lo(h),
				    vis_read_lo(h)));
				r23 = vis_fpadd32(r2, r3);

				res = vis_fpackfix_pair(r01, r23);
				res = vis_fpadd16(res, res);

				n = dw & 3;

				if (n) {

					v16 = ((mlib_u16 *)&res)[0];
					diap =
					    mlib_SqrtTable_U8_Diap[(v16) >> 8];
					dp[12 * j] =
					    mlib_SqrtTable_U8[((v16) >> (diap))
					    + (diap)];

					if (n > 1) {

						v16 = ((mlib_u16 *)&res)[1];
						diap =
						    mlib_SqrtTable_U8_Diap[(v16)
						    >> 8];
						dp[12 * j + 3] =
						    mlib_SqrtTable_U8[((v16) >>
						    (diap)) + (diap)];

						if (n > 2) {
						    v16 = ((mlib_u16 *)&res)[2];
						    diap =
							mlib_SqrtTable_U8_Diap
							[(v16) >> 8];
						    dp[12 * j + 6] =
							mlib_SqrtTable_U8[(
							(v16) >> (diap)) +
							(diap)];
						}
					}
				}

/* swap line buffers */
				d = d0;
				d0 = d1;
				d1 = d2;
				d2 = d;

/* increment row pointers */
				sp0 += slb;
				sp1 += slb;
				sp2 += slb;
				dp += dlb;
			}
		}
	}

	__mlib_free(line);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SQRT_U16(res, src)                                      \
	diap = mlib_SqrtTable_U8_Diap[(src) >> 8];              \
	res = mlib_SqrtTable_U8[((src) >> (diap)) + (diap)]

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_4(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_s32 i, j;
	mlib_s32 d0, d1, d2, d;
	mlib_d64 v, h;
	const mlib_u8 *sp0, *sp1, *sp2;
	mlib_u8 *dp;
	mlib_d64 v0, vn, v1;
	mlib_d64 h0, h1, hn;
	mlib_d64 r0, r1, r, r01, r23;
	mlib_d64 s0, s1, s2, res;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_f32 ftwo = vis_to_float(0x200);
	mlib_s32 diap;
	mlib_s32 bsize1, bsize2;
	mlib_f32 *line;
	mlib_d64 *buf;

	if ((cmask & 15) != 15) {
		return mlib_ImageSobel_U8_4_Sep(da, sa, dlb, slb, dw, dh,
		    cmask);
	}

	dw *= 4;
	bsize1 = (dw + 8 + 7) & ~7;
	bsize2 = dw * sizeof (mlib_u16);
	line = __mlib_malloc(3 * bsize1 + bsize2);

	if (line == NULL)
		return (MLIB_FAILURE);
	buf = (mlib_d64 *)((mlib_u8 *)line + 3 * bsize1);

	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;

	d0 = 2;
	d1 = (bsize1 / sizeof (mlib_f32)) + 2;
	d2 = 2 * (bsize1 / sizeof (mlib_f32)) + 2;

	mlib_ImageCopy_na(sp0 - 4, (void *)(line + d0 - 2), dw + 8);
	mlib_ImageCopy_na(sp1 - 4, (void *)(line + d1 - 2), dw + 8);

/* row loop */
	for (i = 0; i < dh; i++) {
/* initialize line buffer */
		mlib_ImageCopy_na(sp2 - 4, (void *)(line + d2 - 2), dw + 8);
		vis_write_gsr((15 << 3) | 4);

/* initialize column subsums for two first pixels */
		s0 = vis_fmul8x16al(line[d0 - 2], fone);
		s1 = vis_fmul8x16al(line[d1 - 2], ftwo);
		s2 = vis_fmul8x16al(line[d2 - 2], fone);
		v0 = vis_fpadd16(vis_fpadd16(s0, s2), s1);
		h0 = vis_fpsub16(s2, s0);
		s0 = vis_fmul8x16al(line[d0 - 1], fone);
		s1 = vis_fmul8x16al(line[d1 - 1], ftwo);
		s2 = vis_fmul8x16al(line[d2 - 1], fone);
		v1 = vis_fpadd16(vis_fpadd16(s0, s2), s1);
		h1 = vis_fpsub16(s2, s0);

#pragma pipeloop(0)
		for (j = 0; j < (dw >> 2); j++) {
			SQRT_U16(dp[4 * j], ((mlib_u16 *)(buf + j))[0]);
			SQRT_U16(dp[4 * j + 1], ((mlib_u16 *)(buf + j))[1]);
			SQRT_U16(dp[4 * j + 2], ((mlib_u16 *)(buf + j))[2]);
			SQRT_U16(dp[4 * j + 3], ((mlib_u16 *)(buf + j))[3]);

			s0 = vis_fmul8x16al(line[d0 + j], fone);
			s1 = vis_fmul8x16al(line[d1 + j], ftwo);
			s2 = vis_fmul8x16al(line[d2 + j], fone);

/* calculate column subsums for vert & horiz. matrixes */
			vn = vis_fpadd16(vis_fpadd16(s0, s2), s1);
			hn = vis_fpsub16(s2, s0);

/* calculate results for vert. & horiz. matrixes */
			v = vis_fpsub16(vn, v0);

/* calculate v**2+h**2 for pixels 0 & 1 */
			r0 = vis_fmuld8sux16(vis_read_hi(v), vis_read_hi(v));
			r1 = vis_fmuld8ulx16(vis_read_hi(v), vis_read_hi(v));
			r = vis_fpadd32(r0, r1);

			h = vis_fpadd16(vis_fpadd16(h0, hn), vis_fpadd16(h1,
			    h1));
			h0 = h1;
			h1 = hn;
			v0 = v1;
			v1 = vn;

			r0 = vis_fmuld8sux16(vis_read_hi(h), vis_read_hi(h));
			r1 = vis_fmuld8ulx16(vis_read_hi(h), vis_read_hi(h));
			r = vis_fpadd32(r, r0);
			r01 = vis_fpadd32(r, r1);

/* calculate v**2+h**2 for pixels 2 & 3 */
			r0 = vis_fmuld8sux16(vis_read_lo(h), vis_read_lo(h));
			r1 = vis_fmuld8ulx16(vis_read_lo(h), vis_read_lo(h));
			r = vis_fpadd32(r0, r1);
			r0 = vis_fmuld8sux16(vis_read_lo(v), vis_read_lo(v));
			r1 = vis_fmuld8ulx16(vis_read_lo(v), vis_read_lo(v));
			r = vis_fpadd32(r, r0);
			r23 = vis_fpadd32(r, r1);

			res = vis_fpackfix_pair(r01, r23);
			res = vis_fpadd16(res, res);

			buf[j] = res;
		}

/* swap line buffers */
		d = d0;
		d0 = d1;
		d1 = d2;
		d2 = d;

/* increment row pointers */
		sp0 += slb;
		sp1 += slb;
		sp2 += slb;

		if (i)
			dp += dlb;
	}

/* last dst line */
#pragma pipeloop(0)
	for (j = 0; j < dw; j++) {
		SQRT_U16(dp[j], ((mlib_u16 *)buf)[j]);
	}

	__mlib_free(line);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
