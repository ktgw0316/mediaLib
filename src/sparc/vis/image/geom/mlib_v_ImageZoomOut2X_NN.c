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

#pragma ident	"@(#)mlib_v_ImageZoomOut2X_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X -  image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_ImageZoomOut2X.h>
#include <vis_proto.h>

/* *********************************************************** */

#define	ZOOM_OUT2X_U8_1                                         \
	dx_first = vis_fand(dx_first, mask);                    \
	dx_second = vis_fand(dx_second, mask);                  \
	dz = vis_fpack16_pair(dx_first, dx_second)

/* *********************************************************** */

#define	ZOOM_OUT2X_U8_2                                                    \
	dr = vis_fpmerge(vis_read_hi(dx_first), vis_read_hi(dx_second));   \
	dr1 = vis_fpmerge(vis_read_lo(dx_first), vis_read_lo(dx_second));  \
	dz = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));               \
	dz = vis_fpmerge(vis_read_hi(dz), vis_read_lo(dz))

/* *********************************************************** */

#define	ZOOM_OUT2X_U8_3                                         \
	{                                                       \
	    mlib_d64 dr3, dr4, dr5;                             \
	                                                        \
	    (void *)vis_alignaddr(dpx, 6);                      \
	    dr1 = vis_faligndata(dx0, dx1);                     \
	    dr3 = vis_faligndata(dx3, dx4);                     \
	    (void *)vis_alignaddr(dpx, 3);                      \
	    dr = vis_faligndata(dx0, dx0);                      \
	    dr = vis_faligndata(dr, dr1);                       \
	    (void *)vis_alignaddr(dpx, 4);                      \
	    dr1 = vis_faligndata(dx1, dx1);                     \
	    dr4 = vis_faligndata(dx4, dx4);                     \
	    (void *)vis_alignaddr(dpx, 2);                      \
	    dr = vis_faligndata(dr, dr1);                       \
	    dr2 = vis_faligndata(dx2, dx2);                     \
	    dr5 = vis_faligndata(dx5, dx5);                     \
	    (void *)vis_alignaddr(dpx, 7);                      \
	    dr1 = vis_faligndata(dx1, dx1);                     \
	    (void *)vis_alignaddr(dpx, 3);                      \
	    dr1 = vis_faligndata(dr1, dr2);                     \
	    dr1 = vis_faligndata(dr1, dx3);                     \
	    (void *)vis_alignaddr(dpx, 1);                      \
	    dr1 = vis_faligndata(dr1, dr3);                     \
	    dr2 = vis_faligndata(dx3, dx4);                     \
	    (void *)vis_alignaddr(dpx, 3);                      \
	    dr2 = vis_faligndata(dr2, dr4);                     \
	    dr2 = vis_faligndata(dr2, dr5);                     \
	}

/* *********************************************************** */

#define	ZOOM_OUT2X_U8_4                                                \
	dz = vis_freg_pair(vis_read_hi(dx_first), vis_read_hi(dx_second))

/* *********************************************************** */

#define	ZOOM_OUT2X_S16_1	ZOOM_OUT2X_U8_2

/* *********************************************************** */

#define	ZOOM_OUT2X_S16_2	ZOOM_OUT2X_U8_4

/* *********************************************************** */

#define	ZOOM_OUT2X_S16_3                                        \
	(void *)vis_alignaddr(dpx, 6);                          \
	dr = vis_faligndata(dx0, dx0);                          \
	dr2 = vis_faligndata(dx3, dx3);                         \
	(void *)vis_alignaddr(dpx, 4);                          \
	dr1 = vis_faligndata(dx1, dx2);                         \
	dr3 = vis_faligndata(dx4, dx5);                         \
	(void *)vis_alignaddr(dpx, 2);                          \
	dr = vis_faligndata(dr, dr1);                           \
	(void *)vis_alignaddr(dpx, 6);                          \
	dr1 = vis_faligndata(dr1, dr1);                         \
	dr2 = vis_faligndata(dr2, dr3);                         \
	(void *)vis_alignaddr(dpx, 4);                          \
	dr1 = vis_faligndata(dr1, dx3)

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;
	mlib_u8 *px_row, *pz_row;
	mlib_d64 *dpx, dx0, dx1, dx2, dx_first, dx_second, dz,
	    mask = vis_to_double_dup(0xFF00FF);
	mlib_s32 beg, emask;

	vis_write_gsr(7 << 3);
	for (i = 0; i < dst_height; i++) {
		px_row = p_src + 2 * i * src_stride;
		pz_row = p_dst + i * dst_stride;
		beg = (8 - (mlib_u32)pz_row) & 7;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			(*pz_row++) = *px_row;
			px_row += 2;
		}

		dpx = (mlib_d64 *)vis_alignaddr(px_row, -1);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx2 = vis_ld_d64_nf(dpx + 2);
#pragma pipeloop(0)
		for (; j <= (dst_width - 8); j += 8) {
			dx_first = vis_faligndata(dx0, dx1);
			dx_second = vis_faligndata(dx1, dx2);
			ZOOM_OUT2X_U8_1;
			dx0 = dx2;
			dx1 = vis_ld_d64_nf(dpx + 3);
			dx2 = vis_ld_d64_nf(dpx + 4);
			((mlib_d64 *)pz_row)[0] = dz;
			pz_row += 8;
			px_row += 16;
			dpx += 2;
		}

		if (j < dst_width) {
			dx_first = vis_faligndata(dx0, dx1);
			dx_second = vis_faligndata(dx1, dx2);
			ZOOM_OUT2X_U8_1;
			emask =
			    vis_edge8((void *)0, (void *)(dst_width - 1 - j));
			vis_pst_8(dz, pz_row, emask);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;

	mlib_u8 *px_row, *pz_row;
	mlib_d64 *dpx, dx0, dx1, dx2, dx_first, dx_second, dz, dz1, dr, dr1;
	mlib_s32 beg, emask;

	if ((((mlib_s32)p_src | (mlib_s32)p_dst | src_stride | dst_stride) & 1)
	    == 0) {
		mlib_ImageZoomOut2X_S16_1_Nearest;
		((mlib_s16 *)p_src, (mlib_s16 *)p_dst, src_stride / 2,
		    dst_stride / 2, dst_height, dst_width);
		return;
	}

	for (i = 0; i < dst_height; i++) {
		px_row = p_src + 2 * i * src_stride;
		pz_row = p_dst + i * dst_stride;
		emask = (mlib_s32)pz_row & 1;
		beg = (((8 - emask - (mlib_u32)pz_row) & 7) >> 1) + emask;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			(*pz_row++) = *px_row;
			(*pz_row++) = *(px_row + 1);
			px_row += 4;
		}

		dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx2 = vis_ld_d64_nf(dpx + 2);

		if (emask == 0) {
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				dx_first = vis_faligndata(dx0, dx1);
				dx_second = vis_faligndata(dx1, dx2);
				ZOOM_OUT2X_U8_2;
				dx0 = dx2;
				dx1 = vis_ld_d64_nf(dpx + 3);
				dx2 = vis_ld_d64_nf(dpx + 4);
				((mlib_d64 *)pz_row)[0] = dz;
				pz_row += 8;
				px_row += 16;
				dpx += 2;
			}

			if (j < dst_width) {
				dx_first = vis_faligndata(dx0, dx1);
				dx_second = vis_faligndata(dx1, dx2);
				ZOOM_OUT2X_U8_2;
				emask =
				    vis_edge8((void *)0,
				    (void *)(2 * (dst_width - j) - 1));
				vis_pst_8(dz, pz_row, emask);
			}
		} else {
			pz_row--;
			dz1 = vis_ld_u8(pz_row);
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				dx_first = vis_faligndata(dx0, dx1);
				dx_second = vis_faligndata(dx1, dx2);
				ZOOM_OUT2X_U8_2;
				vis_alignaddr(dpx, 7);
				dz1 = vis_faligndata(dz1, dz);
				dx0 = dx2;
				dx1 = vis_ld_d64_nf(dpx + 3);
				dx2 = vis_ld_d64_nf(dpx + 4);
				((mlib_d64 *)pz_row)[0] = dz1;
				dz1 = dz;
				vis_alignaddr(px_row, 0);
				pz_row += 8;
				px_row += 16;
				dpx += 2;
			}

			vis_st_u8(dz1, pz_row);

			if (j < dst_width) {
				dx_first = vis_faligndata(dx0, dx1);
				dx_second = vis_faligndata(dx1, dx2);
				ZOOM_OUT2X_U8_2;
				vis_alignaddr(dpx, 7);
				dz1 = vis_faligndata(dz1, dz);
				emask =
				    vis_edge8((void *)0,
				    (void *)(2 * (dst_width - j)));
				vis_pst_8(dz1, pz_row, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;

	mlib_u8 *px_row, *pz_row;
	mlib_d64 *dpx, dx0, dx1, dx2, dx3, dx4, dx5, dx6;
	mlib_d64 dr, dr1, dr2;
	mlib_s32 beg;

	for (i = 0; i < dst_height; i++) {
		px_row = p_src + 2 * i * src_stride;
		pz_row = p_dst + i * dst_stride;
		beg = ((mlib_s32)pz_row & 7) ^ (((mlib_s32)pz_row & 1) << 2);

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			pz_row[0] = px_row[0];
			pz_row[1] = px_row[1];
			pz_row[2] = px_row[2];
			px_row += 6;
			pz_row += 3;
		}

		dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx2 = vis_ld_d64_nf(dpx + 2);
		dx3 = vis_ld_d64_nf(dpx + 3);
		dx4 = vis_ld_d64_nf(dpx + 4);
		dx5 = vis_ld_d64_nf(dpx + 5);

		if (((mlib_s32)px_row & 7) == 0) {
#pragma pipeloop(0)
			for (; j <= (dst_width - 8); j += 8) {
				ZOOM_OUT2X_U8_3;
				dx0 = vis_ld_d64_nf((mlib_d64 *)px_row + 6);
				dx1 = vis_ld_d64_nf((mlib_d64 *)px_row + 7);
				dx2 = vis_ld_d64_nf((mlib_d64 *)px_row + 8);
				dx3 = vis_ld_d64_nf((mlib_d64 *)px_row + 9);
				dx4 = vis_ld_d64_nf((mlib_d64 *)px_row + 10);
				dx5 = vis_ld_d64_nf((mlib_d64 *)px_row + 11);
				((mlib_d64 *)pz_row)[0] = dr;
				((mlib_d64 *)pz_row)[1] = dr1;
				((mlib_d64 *)pz_row)[2] = dr2;
				pz_row += 24;
				px_row += 48;
			}
		} else {
			dx6 = vis_ld_d64_nf(dpx + 6);
#pragma pipeloop(0)
			for (; j <= (dst_width - 8); j += 8) {
				dx0 = vis_faligndata(dx0, dx1);
				dx1 = vis_faligndata(dx1, dx2);
				dx2 = vis_faligndata(dx2, dx3);
				dx3 = vis_faligndata(dx3, dx4);
				dx4 = vis_faligndata(dx4, dx5);
				dx5 = vis_faligndata(dx5, dx6);
				ZOOM_OUT2X_U8_3;
				dx0 = dx6;
				dx1 = vis_ld_d64_nf(dpx + 7);
				dx2 = vis_ld_d64_nf(dpx + 8);
				dx3 = vis_ld_d64_nf(dpx + 9);
				dx4 = vis_ld_d64_nf(dpx + 10);
				dx5 = vis_ld_d64_nf(dpx + 11);
				dx6 = vis_ld_d64_nf(dpx + 12);
				((mlib_d64 *)pz_row)[0] = dr;
				((mlib_d64 *)pz_row)[1] = dr1;
				((mlib_d64 *)pz_row)[2] = dr2;
				vis_alignaddr(px_row, 0);
				pz_row += 24;
				dpx += 6;
				px_row += 48;
			}
		}

		for (; j < dst_width; j++) {
			pz_row[0] = px_row[0];
			pz_row[1] = px_row[1];
			pz_row[2] = px_row[2];
			px_row += 6;
			pz_row += 3;
		}
	}
}

/* *********************************************************** */

#define	NN_U8_4_LOAD_SRC(J)                                     \
	r01 = r09;                                              \
	r02 = vis_ld_d64_nf(psd + J);                           \
	r03 = vis_ld_d64_nf(psd + J + 1);                       \
	r04 = vis_ld_d64_nf(psd + J + 2);                       \
	r05 = vis_ld_d64_nf(psd + J + 3);                       \
	r06 = vis_ld_d64_nf(psd + J + 4);                       \
	r07 = vis_ld_d64_nf(psd + J + 5);                       \
	r08 = vis_ld_d64_nf(psd + J + 6);                       \
	r09 = vis_ld_d64_nf(psd + J + 7)

/* *********************************************************** */

#define	NN_U8_4_ALIGNDATA                                       \
	r10 = vis_faligndata(r01, r02);                         \
	r11 = vis_faligndata(r02, r03);                         \
	r12 = vis_faligndata(r03, r04);                         \
	r13 = vis_faligndata(r04, r05);                         \
	r14 = vis_faligndata(r05, r06);                         \
	r15 = vis_faligndata(r06, r07);                         \
	r16 = vis_faligndata(r07, r08);                         \
	r17 = vis_faligndata(r08, r09)

/* *********************************************************** */

#define	NN_U8_4_CALC                                               \
	r18 = vis_freg_pair(vis_read_hi(r10), vis_read_hi(r11));   \
	r19 = vis_freg_pair(vis_read_hi(r12), vis_read_hi(r13));   \
	r20 = vis_freg_pair(vis_read_hi(r14), vis_read_hi(r15));   \
	r21 = vis_freg_pair(vis_read_hi(r16), vis_read_hi(r17))

/* *********************************************************** */

#define	NN_U8_4_CALC_D                                             \
	r23 = vis_freg_pair(vis_read_hi(r10), vis_read_hi(r11));   \
	r24 = vis_freg_pair(vis_read_hi(r12), vis_read_hi(r13));   \
	r25 = vis_freg_pair(vis_read_hi(r14), vis_read_hi(r15));   \
	r26 = vis_freg_pair(vis_read_hi(r16), vis_read_hi(r17));   \
	vis_alignaddr((mlib_s32 *)dsn, 0);                         \
	r18 = vis_faligndata(r22, r23);                            \
	r19 = vis_faligndata(r23, r24);                            \
	r20 = vis_faligndata(r24, r25);                            \
	r21 = vis_faligndata(r25, r26);                            \
	r22 = r26;                                                 \
	vis_alignaddr(ps, 0)

/* *********************************************************** */

#define	NN_U8_4_SAVE(J)                                         \
	pdd[J] = r18;                                           \
	pdd[J + 1] = r19;                                       \
	pdd[J + 2] = r20;                                       \
	pdd[J + 3] = r21

/* *********************************************************** */

void
mlib_Common_u8_4_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_u16 *ps = (mlib_u16 *)(p_src + 2 * i * src_stride);
		mlib_u16 *pd = (mlib_u16 *)(p_dst + i * dst_stride);
		mlib_s32 j, j2, dw = ((dst_width - 8) & ~7) >> 1;
		mlib_d64 *pdd, *psd;
		mlib_d64 r00, r01, r02, r03, r04, r05, r06, r07,
		    r08, r09, r10, r11, r12, r13, r14, r15,
		    r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26;

		if (!((mlib_addr)pd & 3)) {
		    mlib_s32 pr = ((mlib_addr)pd & 4) >> 2, jsd = 1;

		    pdd = (mlib_d64 *)(((mlib_addr)pd + 4) & ~7);
		    psd = (mlib_d64 *)vis_alignaddr(ps, 0);

		    if (pr) {
			r00 = *psd;
			r09 = *(psd + 1);
			jsd++;
			r02 = vis_faligndata(r00, r09);
			*((mlib_f32 *)pd) = vis_read_hi(r02);
			dw = ((dst_width - 9) & ~7) >> 1;
		    } else
			r09 = *(psd);

		    NN_U8_4_LOAD_SRC(jsd);
		    jsd += 8;
		    NN_U8_4_ALIGNDATA;
		    NN_U8_4_CALC;
		    NN_U8_4_LOAD_SRC(jsd);
		    jsd += 8;
#pragma pipeloop(0)
		    for (j = 0; j < dw; j += 4, jsd += 8) {
			NN_U8_4_SAVE(j);
			NN_U8_4_ALIGNDATA;
			NN_U8_4_CALC;
			NN_U8_4_LOAD_SRC(jsd);
		    }

		    NN_U8_4_SAVE(j);

		    jsd -= 8;
		    for (j2 = (j + 4) * 2 + pr; j2 < dst_width; j2++, jsd++) {
			r00 = r01;
			r01 = vis_ld_d64_nf(psd + jsd);
			r02 = vis_faligndata(r00, r01);
			*((mlib_f32 *)pd + j2) = vis_read_hi(r02);
		    }
		} else {
			mlib_s32 jsd = 3, dst_al = ((mlib_addr)pd & 7), dsn =
			    8 - dst_al;
			mlib_s32 emask_hi, emask_lo, pr, ds =
			    (dsn + 3) >> 2, de =
			    8 - (((dst_width << 2) - dsn) & 7);
			pdd = (mlib_d64 *)(((mlib_addr)pd) & ~7);
			psd = (mlib_d64 *)vis_alignaddr(ps, 0);
			pr = ((dst_width - ds) & 7);
			dw = (((dst_width - ds) & ~7) >> 1) - 4;
			emask_hi = 0xff >> dst_al;
			emask_lo = 0xff << de;

			r01 = vis_ld_d64_nf(psd);
			r02 = vis_ld_d64_nf(psd + 1);
			r09 = vis_ld_d64_nf(psd + 2);
			r10 = vis_faligndata(r01, r02);
			r11 = vis_faligndata(r02, r09);
			r22 = vis_freg_pair(vis_read_hi(r10), vis_read_hi(r11));
			vis_alignaddr((mlib_s32 *)dsn, 0);
			r23 = vis_faligndata(r22, r22);
			vis_pst_8(r23, pdd, emask_hi);
			vis_alignaddr(ps, 0);

			NN_U8_4_LOAD_SRC(jsd);
			jsd += 8;
			NN_U8_4_ALIGNDATA;
			NN_U8_4_CALC_D;
			NN_U8_4_LOAD_SRC(jsd);
			jsd += 8;
#pragma pipeloop(0)
			for (j = 1; j < dw; j += 4, jsd += 8) {
				NN_U8_4_SAVE(j);
				NN_U8_4_ALIGNDATA;
				NN_U8_4_CALC_D;
				NN_U8_4_LOAD_SRC(jsd);
			}

			NN_U8_4_SAVE(j);

			NN_U8_4_ALIGNDATA;
			NN_U8_4_CALC_D;
			pdd += (j + 4);

			if (pr < 2)
				vis_pst_8(r18, pdd, emask_lo);
			else {
				pdd[0] = r18;

				if (pr < 4)
					vis_pst_8(r19, pdd + 1, emask_lo);
				else {
					pdd[1] = r19;

					if (pr < 6)
						vis_pst_8(r20, pdd + 2,
						    emask_lo);
					else {
						pdd[2] = r20;
						vis_pst_8(r21, pdd + 3,
						    emask_lo);
					}
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i;

	if ((((mlib_s32)p_src | (mlib_s32)p_dst | src_stride | dst_stride) & 1)
	    == 0) {
		mlib_ImageZoomOut2X_S16_2_Nearest;
		((mlib_s16 *)p_src, (mlib_s16 *)p_dst, src_stride / 2,
		    dst_stride / 2, dst_height, dst_width);
		return;
	}

	if (dst_width > 15)
		mlib_Common_u8_4_Nearest(NEAREST_P);
	else {
		for (i = 0; i < dst_height; i++) {
		    mlib_s32 j, j2;
		    mlib_u8 *pd = p_dst + i * dst_stride;
		    mlib_u8 *ps = p_src + 2 * i * src_stride;

#pragma pipeloop(0)
		    for (j = 0, j2 = 0; j < dst_width * 4; j += 4, j2 += 8) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
			pd[j + 3] = ps[j2 + 3];
		    }
		}
	}

#if 0
	mlib_s32 i, j = 0;

	mlib_u8 *px_row, *pz_row;
	mlib_d64 *dpx, *dpz;
	mlib_d64 dx0, dx1, dx2, dx_first, dx_second, dz;
	mlib_s32 all_z, emask_hi, emask_lo;

	if ((((mlib_s32)p_src | (mlib_s32)p_dst | src_stride | dst_stride) & 1)
	    == 0) {
		mlib_ImageZoomOut2X_S16_2_Nearest;
		((mlib_s16 *)p_src, (mlib_s16 *)p_dst, src_stride / 2,
		    dst_stride / 2, dst_height, dst_width);
		return;
	}

	if ((((mlib_s32)p_dst | dst_stride) & 3) == 0) {
		for (i = 0; i < dst_height; i++) {
			px_row = p_src + 2 * i * src_stride;
			pz_row = p_dst + i * dst_stride;
			dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
			dx0 = dpx[0];
			dx1 = dpx[1];
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx_first = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dx1 = dpx[2];
				((mlib_f32 *)pz_row)[0] = vis_read_hi(dx_first);
				dpx++;
				pz_row += 4;
			}
		}

		return;
	}

	for (i = 0; i < dst_height; i++) {
		px_row = p_src + 2 * i * src_stride;
		pz_row = p_dst + i * dst_stride;
		dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
		dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
		emask_hi = 0xff >> ((mlib_u8 *)pz_row - (mlib_u8 *)dpz);
		emask_lo = ~emask_hi;
		all_z = 8 - (mlib_addr)pz_row;
		dx0 = dpx[0];
		dx1 = dpx[1];
		dx2 = dpx[2];
		dx_first = vis_faligndata(dx0, dx1);
		dx_second = vis_faligndata(dx1, dx2);
		ZOOM_OUT2X_U8_4;
#pragma pipeloop(0)
		for (j = 0; j <= (dst_width - 2); j += 2) {
			dx0 = dx2;
			dx1 = dpx[3];
			dx2 = dpx[4];
			(void *)vis_alignaddr((void *)all_z, 0);
			dz = vis_faligndata(dz, dz);
			vis_pst_8(dz, dpz, emask_hi);
			dpz++;
			vis_pst_8(dz, dpz, emask_lo);
			dpx += 2;
			(void *)vis_alignaddr(px_row, 0);
			dx_first = vis_faligndata(dx0, dx1);
			dx_second = vis_faligndata(dx1, dx2);
			ZOOM_OUT2X_U8_4;
		}
	}

	if (j < dst_width) {
		for (i = 0; i < dst_height; i++) {
			p_dst[i * dst_stride + 4 * j] =
			    p_src[2 * i * src_stride + 8 * j];
			p_dst[i * dst_stride + 4 * j + 1] =
			    p_src[2 * i * src_stride + 8 * j + 1];
			p_dst[i * dst_stride + 4 * j + 2] =
			    p_src[2 * i * src_stride + 8 * j + 2];
			p_dst[i * dst_stride + 4 * j + 3] =
			    p_src[2 * i * src_stride + 8 * j + 3];
		}
	}
#endif /* 0 */
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_1_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;

	mlib_u16 *px_row, *pz_row;
	mlib_d64 *dpx, dx2, dx_first, dx_second, dz, dr, dr1;
	mlib_s32 beg, emask;

	for (i = 0; i < dst_height; i++) {
		px_row = (mlib_u16 *)p_src + 2 * i * src_stride;
		pz_row = (mlib_u16 *)p_dst + i * dst_stride;
		beg = ((8 - (mlib_u32)pz_row) & 7) >> 1;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			(*pz_row++) = *px_row;
			px_row += 2;
		}

		dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
		dx_first = vis_ld_d64_nf(dpx);
		dx_second = vis_ld_d64_nf(dpx + 1);

		if (((mlib_u32)px_row & 7) == 0) {
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				ZOOM_OUT2X_S16_1;
				dx_first = vis_ld_d64_nf(dpx + 2);
				dx_second = vis_ld_d64_nf(dpx + 3);
				((mlib_d64 *)pz_row)[0] = dz;
				pz_row += 4;
				dpx += 2;
			}
		} else {
			dx2 = vis_ld_d64_nf(dpx + 2);
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				dx_first = vis_faligndata(dx_first, dx_second);
				dx_second = vis_faligndata(dx_second, dx2);
				ZOOM_OUT2X_S16_1;
				dx_first = dx2;
				dx_second = vis_ld_d64_nf(dpx + 3);
				dx2 = vis_ld_d64_nf(dpx + 4);
				((mlib_d64 *)pz_row)[0] = dz;
				pz_row += 4;
				dpx += 2;
			}
		}

		if (j < dst_width) {
			dx_first = vis_faligndata(dx_first, dx_second);
			dx_second = vis_faligndata(dx_second, dx2);
			ZOOM_OUT2X_S16_1;
			emask =
			    vis_edge8((void *)0,
			    (void *)(2 * (dst_width - j) - 1));
			vis_pst_8(dz, pz_row, emask);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	if ((((mlib_addr)p_src | (mlib_addr)p_dst | dst_stride | src_stride) &
	    2) && dst_width > 15) {
		for (i = 0; i < dst_height; i++) {
			mlib_u16 *ps = (mlib_u16 *)(p_src + 2 * i * src_stride);
			mlib_u16 *pd = (mlib_u16 *)(p_dst + i * dst_stride);
			mlib_s32 j, j2, dw = ((dst_width - 8) & ~7) >> 1;
			mlib_d64 *pdd, *psd;
			mlib_d64 r00, r01, r02, r03, r04, r05, r06, r07,
			    r08, r09, r10, r11, r12, r13, r14, r15,
			    r16, r17, r18, r19, r20, r21, r22, r23, r24, r25,
			    r26;

			psd = (mlib_d64 *)vis_alignaddr(ps, 0);

			if (!((mlib_addr)pd & 2)) {
				mlib_s32 pr = ((mlib_addr)pd & 4) >> 2, jsd = 1;

				pdd = (mlib_d64 *)(((mlib_addr)pd + 6) & ~7);

				if (pr) {
					r00 = *psd;
					r09 = *(psd + 1);
					jsd++;
					r02 = vis_faligndata(r00, r09);
					*((mlib_f32 *)pd) = vis_read_hi(r02);
					dw = ((dst_width - 9) & ~7) >> 1;
				} else
					r09 = *(psd);

				NN_U8_4_LOAD_SRC(jsd);
				jsd += 8;
				NN_U8_4_ALIGNDATA;
				NN_U8_4_CALC;
				NN_U8_4_LOAD_SRC(jsd);
				jsd += 8;
#pragma pipeloop(0)
				for (j = 0; j < dw; j += 4, jsd += 8) {
					NN_U8_4_SAVE(j);
					NN_U8_4_ALIGNDATA;
					NN_U8_4_CALC;
					NN_U8_4_LOAD_SRC(jsd);
				}

				NN_U8_4_SAVE(j);

				jsd -= 8;
				for (j2 = (j + 4) * 2 + pr; j2 < dst_width;
				    j2++, jsd++) {
					r00 = r01;
					r01 = vis_ld_d64_nf(psd + jsd);
					r02 = vis_faligndata(r00, r01);
					*((mlib_f32 *)pd + j2) =
					    vis_read_hi(r02);
				}
			} else {
				mlib_s32 jsd = 3, dst_al =
				    ((mlib_addr)pd & 6), dsn = 8 - dst_al;
				mlib_s32 emask_hi, emask_lo, pr, ds =
				    (dsn + 2) >> 2;
				pdd = (mlib_d64 *)(((mlib_addr)pd) & ~7);
				pr = ((dst_width - ds) & 7);
				dw = (((dst_width - ds) & ~7) / 2) - 4;
				emask_hi = (dsn == 2) ? 0x03 : 0x3f;
				emask_lo = ((dst_width - ds) & 1) ? 0xfc : 0xc0;

				r01 = vis_ld_d64_nf(psd);
				r02 = vis_ld_d64_nf(psd + 1);
				r09 = vis_ld_d64_nf(psd + 2);
				r10 = vis_faligndata(r01, r02);
				r11 = vis_faligndata(r02, r09);
				r22 =
				    vis_freg_pair(vis_read_hi(r10),
				    vis_read_hi(r11));
				vis_alignaddr((mlib_s32 *)dsn, 0);
				r23 = vis_faligndata(r22, r22);
				vis_pst_8(r23, pdd, emask_hi);
				vis_alignaddr(ps, 0);

				NN_U8_4_LOAD_SRC(jsd);
				jsd += 8;
				NN_U8_4_ALIGNDATA;
				NN_U8_4_CALC_D;
				NN_U8_4_LOAD_SRC(jsd);
				jsd += 8;
#pragma pipeloop(0)
				for (j = 1; j < dw; j += 4, jsd += 8) {
					NN_U8_4_SAVE(j);
					NN_U8_4_ALIGNDATA;
					NN_U8_4_CALC_D;
					NN_U8_4_LOAD_SRC(jsd);
				}

				NN_U8_4_SAVE(j);

				NN_U8_4_ALIGNDATA;
				NN_U8_4_CALC_D;
				pdd += (j + 4);

				if (pr < 2)
					vis_pst_8(r18, pdd, emask_lo);
				else {
					pdd[0] = r18;

					if (pr < 4)
						vis_pst_8(r19, pdd + 1,
						    emask_lo);
					else {
						pdd[1] = r19;

						if (pr < 6)
							vis_pst_8(r20, pdd + 2,
							    emask_lo);
						else {
							pdd[2] = r20;
							vis_pst_8(r21, pdd + 3,
							    emask_lo);
						}
					}
				}
			}
		}
	} else {
		for (i = 0; i < dst_height; i++) {
			mlib_s32 j, j2;
			mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
			mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

			if ((((mlib_addr)ps | (mlib_addr)pd) & 3) == 0) {
				mlib_f32 *psf = (mlib_f32 *)ps;
				mlib_f32 *pdf = (mlib_f32 *)pd;

#pragma pipeloop(0)
				for (j = 0, j2 = 0; j < dst_width; j++, j2 += 2)
					pdf[j] = psf[j2];
			} else {
#pragma pipeloop(0)
				for (j = 0, j2 = 0; j < dst_width * 2;
				    j += 2, j2 += 4) {
					pd[j] = ps[j2];
					pd[j + 1] = ps[j2 + 1];
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_3_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;

	mlib_u16 *px_row, *pz_row;
	mlib_d64 *dpx, dx0, dx1, dx2, dx3, dx4, dx5, dx6;
	mlib_d64 dr, dr1, dr2, dr3;
	mlib_s32 beg;

	for (i = 0; i < dst_height; i++) {
		px_row = (mlib_u16 *)p_src + 2 * i * src_stride;
		pz_row = (mlib_u16 *)p_dst + i * dst_stride;
		beg = ((mlib_s32)pz_row & 7) >> 1;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			pz_row[0] = px_row[0];
			pz_row[1] = px_row[1];
			pz_row[2] = px_row[2];
			px_row += 6;
			pz_row += 3;
		}

		dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx2 = vis_ld_d64_nf(dpx + 2);
		dx3 = vis_ld_d64_nf(dpx + 3);
		dx4 = vis_ld_d64_nf(dpx + 4);
		dx5 = vis_ld_d64_nf(dpx + 5);

		if (((mlib_s32)px_row & 7) == 0) {
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				ZOOM_OUT2X_S16_3;
				dx0 = vis_ld_d64_nf((mlib_d64 *)px_row + 6);
				dx1 = vis_ld_d64_nf((mlib_d64 *)px_row + 7);
				dx2 = vis_ld_d64_nf((mlib_d64 *)px_row + 8);
				dx3 = vis_ld_d64_nf((mlib_d64 *)px_row + 9);
				dx4 = vis_ld_d64_nf((mlib_d64 *)px_row + 10);
				dx5 = vis_ld_d64_nf((mlib_d64 *)px_row + 11);
				((mlib_d64 *)pz_row)[0] = dr;
				((mlib_d64 *)pz_row)[1] = dr1;
				((mlib_d64 *)pz_row)[2] = dr2;
				pz_row += 12;
				px_row += 24;
			}
		} else {
			dx6 = vis_ld_d64_nf(dpx + 6);
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				dx0 = vis_faligndata(dx0, dx1);
				dx1 = vis_faligndata(dx1, dx2);
				dx2 = vis_faligndata(dx2, dx3);
				dx3 = vis_faligndata(dx3, dx4);
				dx4 = vis_faligndata(dx4, dx5);
				dx5 = vis_faligndata(dx5, dx6);
				ZOOM_OUT2X_S16_3;
				dx0 = dx6;
				dx1 = vis_ld_d64_nf(dpx + 7);
				dx2 = vis_ld_d64_nf(dpx + 8);
				dx3 = vis_ld_d64_nf(dpx + 9);
				dx4 = vis_ld_d64_nf(dpx + 10);
				dx5 = vis_ld_d64_nf(dpx + 11);
				dx6 = vis_ld_d64_nf(dpx + 12);
				((mlib_d64 *)pz_row)[0] = dr;
				((mlib_d64 *)pz_row)[1] = dr1;
				((mlib_d64 *)pz_row)[2] = dr2;
				vis_alignaddr(px_row, 0);
				pz_row += 12;
				dpx += 6;
				px_row += 24;
			}
		}

		for (; j < dst_width; j++) {
			pz_row[0] = px_row[0];
			pz_row[1] = px_row[1];
			pz_row[2] = px_row[2];
			px_row += 6;
			pz_row += 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;

	mlib_u16 *px_row, *pz_row;
	mlib_d64 *dpx, *dpz, dx0, dx1, dz;
	mlib_s32 all_z, emask_hi, emask_lo;

	if ((((mlib_s32)p_dst | (2 * dst_stride)) & 7) == 0) {
		for (i = 0; i < dst_height; i++) {
			px_row = (mlib_u16 *)p_src + 2 * i * src_stride;
			pz_row = (mlib_u16 *)p_dst + i * dst_stride;

			if (((mlib_s32)px_row & 7) == 0) {
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dz = ((mlib_d64 *)px_row)[0];
					((mlib_d64 *)pz_row)[0] = dz;
					px_row += 8;
					pz_row += 4;
				}
			} else {
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = dpx[0];
					dx1 = dpx[1];
					dz = vis_faligndata(dx0, dx1);
					((mlib_d64 *)pz_row)[0] = dz;
					dpx += 2;
					pz_row += 4;
				}
			}
		}

		return;
	}

	for (i = 0; i < dst_height; i++) {
		px_row = (mlib_u16 *)p_src + 2 * i * src_stride;
		pz_row = (mlib_u16 *)p_dst + i * dst_stride;
		dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
		dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
		emask_hi = 0xff >> ((mlib_u8 *)pz_row - (mlib_u8 *)dpz);
		emask_lo = ~emask_hi;
		all_z = 8 - (mlib_addr)pz_row;

		if (dst_width > 0) {
			dx0 = dpx[0];
			dx1 = vis_ld_d64_nf(dpx + 1);
			dz = vis_faligndata(dx0, dx1);
			(void *)vis_alignaddr((void *)all_z, 0);
			dz = vis_faligndata(dz, dz);
			if (emask_hi)
				vis_pst_8(dz, dpz, emask_hi);
			dpz++;
			vis_pst_8(dz, dpz, emask_lo);
			dpx += 2;
			(void *)vis_alignaddr(px_row, 0);
		}
#pragma pipeloop(0)
		for (j = 1; j < dst_width; j++) {
			dx0 = dpx[0];
			dx1 = vis_ld_d64_nf(dpx + 1);
			dz = vis_faligndata(dx0, dx1);
			(void *)vis_alignaddr((void *)all_z, 0);
			dz = vis_faligndata(dz, dz);
			vis_pst_8(dz, dpz, emask_hi);
			dpz++;
			vis_pst_8(dz, dpz, emask_lo);
			dpx += 2;
			(void *)vis_alignaddr(px_row, 0);
		}
	}
}

/* *********************************************************** */
