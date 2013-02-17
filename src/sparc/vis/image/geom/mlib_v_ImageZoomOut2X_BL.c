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

#pragma ident	"@(#)mlib_v_ImageZoomOut2X_BL.c	9.2	07/11/05 SMI"

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
 *      edge    Type of edge condition.
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

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageZoomOut2X.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#define	BUFF_SIZE	1024

/* *********************************************************** */

#define	BILINEAR_U8_1_XY                                        \
	dx1h = vis_fmul8sux16(dx1, done);                       \
	dx1h = vis_fand(dmask, dx1h);                           \
	dx2h = vis_fmul8sux16(dx2, done);                       \
	dx2h = vis_fand(dmask, dx2h);                           \
	dx1 = vis_fand(dmask, dx1);                             \
	dx2 = vis_fand(dmask, dx2);                             \
	dx1h = vis_fpadd16(dx1h, dx1);                          \
	dx2h = vis_fpadd16(dx2h, dx2);                          \
	dr = vis_fpadd16(dx1h, dx2h);                           \
	fr = vis_fpack16(dr)

/* *********************************************************** */

#define	BILINEAR_U8_1_X                                         \
	dx1l = vis_fand(dmask, dx1);                            \
	dx2l = vis_fand(dmask, dx2);                            \
	dx1 = vis_fmul8sux16(dx1, done);                        \
	dx1 = vis_fand(dmask, dx1);                             \
	dx2 = vis_fmul8sux16(dx2, done);                        \
	dx2 = vis_fand(dmask, dx2);                             \
	dx1 = vis_fpadd16(dx1, dx1l);                           \
	dx2 = vis_fpadd16(dx2, dx2l);                           \
	dr = vis_fpack16_pair(dx1, dx2)

/* *********************************************************** */

#define	BILINEAR_U8_1_Y                                         \
	dx1 = vis_fand(dmask, dx1);                             \
	dx2 = vis_fand(dmask, dx2);                             \
	dr = vis_fpadd16(dx1, dx2);                             \
	fr = vis_fpack16(dr)

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U8_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;
	mlib_u8 *sp0, *dp, *pdend;
	mlib_d64 dx1, dx2, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0, *dpd;
	mlib_d64 done = vis_to_double_dup(0x1000100);
	mlib_d64 dmask = vis_to_double_dup(0xff00ff);
	mlib_f32 fr;
	mlib_s32 beg, off, emask;

	vis_write_gsr(6 << 3);

	if (dh) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			pdend = (mlib_u8 *)dp + dst_width - 1;
			dpd = (mlib_d64 *)((mlib_addr)dp & (~7));
			off = (mlib_s32)dpd - (mlib_s32)dp;

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, off + off);
			dx1h = vis_ld_d64_nf(dsp0);
			dx1l = vis_ld_d64_nf(dsp0 + 1);
			dx2h = vis_ld_d64_nf(dsp0 + 2);
			dx1 = vis_faligndata(dx1h, dx1l);
			dx2 = vis_faligndata(dx1l, dx2h);
			BILINEAR_U8_1_X;
			emask = vis_edge8(dp, pdend);
			vis_pst_8(dr, dpd, emask);
			dsp0 += 2;
			dpd++;
#pragma pipeloop(0)
			for (j = 8 + off; j <= (dst_width - 8); j += 8) {
				dx1h = dx2h;
				dx1l = dsp0[1];
				dx2h = dsp0[2];
				dx1 = vis_faligndata(dx1h, dx1l);
				dx2 = vis_faligndata(dx1l, dx2h);
				BILINEAR_U8_1_X;
				(*dpd++) = dr;
				dsp0 += 2;
			}

			if (j < dst_width) {
				dx1h = dx2h;
				dx1l = vis_ld_d64_nf(dsp0 + 1);
				dx2h = vis_ld_d64_nf(dsp0 + 2);
				dx1 = vis_faligndata(dx1h, dx1l);
				dx2 = vis_faligndata(dx1l, dx2h);
				BILINEAR_U8_1_X;
				emask = vis_edge8(dpd, pdend);
				vis_pst_8(dr, dpd, emask);
			}
		}

		return (MLIB_SUCCESS);
	}

	if (dw) {
		if (((((mlib_s32)p_dst | dst_stride) & 3) | (((mlib_s32)p_src |
		    src_stride) & 7)) == 0) {

			if (((dst_stride & 3) == 0) || (dst_width <= 4)) {
				beg =
				    (dst_width <=
				    4) ? dst_width : (4 - (mlib_addr)p_dst) & 3;
				for (j = 0; j < beg; j++) {
					for (i = 0; i < dst_height; i++) {
						p_dst[i * dst_stride + j] =
						    (p_src[2 * i * src_stride +
						    2 * j] + p_src[(2 * i +
						    1) * src_stride +
						    2 * j]) >> 1;
					}
				}

				dst_width -= beg;
				p_src += 2 * beg;
				p_dst += beg;

				if (dst_width <= 0)
					return (MLIB_SUCCESS);
			}

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j <= (dst_width - 4); j += 4) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 =
					    ((mlib_d64 *)(sp0 + src_stride))[0];
					dx1 = vis_fmul8sux16(dx1, done);
					dx2 = vis_fmul8sux16(dx2, done);
					BILINEAR_U8_1_Y;
					((mlib_f32 *)dp)[0] = fr;
					sp0 += 8;
					dp += 4;
				}

				for (; j < dst_width; j++) {
					(*dp++) =
					    (sp0[0] + sp0[src_stride]) >> 1;
					sp0 += 2;
				}
			}

			return (MLIB_SUCCESS);
		} else {
			mlib_d64 buff_lcl[BUFF_SIZE / 8], *buffd = buff_lcl;
			mlib_u8 *sl = p_src, *sp0, *sp1;
			mlib_u8 *dl = p_dst, *dp0;
			mlib_d64 da0, da1, *ap0, *ap1, *dp;
			mlib_s32 n;

			if (dst_width > BUFF_SIZE) {
				buffd = __mlib_malloc(dst_width);

				if (buffd == NULL)
					return (MLIB_FAILURE);
			}

			for (i = 0; i < dst_height; i++) {
				n = dst_width;
				sp0 = sl;
				sp1 = sl += src_stride;
				dp0 = dl;

				while (((mlib_s32)sp0 & 6) && (n > 0)) {
					(*dp0++) = (sp0[0] + sp1[0]) >> 1;
					sp0 += 2;
					sp1 += 2;
					n--;
				}

				if ((mlib_s32)dp0 & 3) {
					dp = buffd;
				} else {
					dp = (mlib_d64 *)dp0;
				}

				if ((mlib_s32)sp0 & 1) {
					ap0 = (mlib_d64 *)(sp0 - 1);
					ap1 = vis_alignaddr(sp1 - 1, 0);

					da1 = (*ap1++);
#pragma pipeloop(0)
					for (j = 0; j < n / 4; j++) {
						dx1 = ap0[j];
						da0 = da1;
						da1 = ap1[j];
						dx2 = vis_faligndata(da0, da1);
						BILINEAR_U8_1_Y;
						((mlib_f32 *)dp)[j] = fr;
					}
				} else {
					ap0 = (mlib_d64 *)(sp0);
					ap1 = vis_alignaddr(sp1, 0);

					da1 = vis_ld_d64_nf(ap1); ap1++;
#pragma pipeloop(0)
					for (j = 0; j < n / 4; j++) {
						dx1 = ap0[j];
						da0 = da1;
						da1 = vis_ld_d64_nf(ap1 + j);
						dx2 = vis_faligndata(da0, da1);
						dx1 = vis_fmul8sux16(dx1, done);
						dx2 = vis_fmul8sux16(dx2, done);
						BILINEAR_U8_1_Y;
						((mlib_f32 *)dp)[j] = fr;
					}
				}

				if ((mlib_s32)dp0 & 3) {
					mlib_ImageCopy_na((void *)buffd,
					    (void *)dp0, n & ~3);
				}

				for (j = 4 * j; j < n; j++) {
					dp0[j] = (sp0[2 * j] + sp1[2 * j]) >> 1;
				}

				sl += src_stride;
				dl += dst_stride;
			}

			if (buffd != buff_lcl)
				__mlib_free(buffd);
			return (MLIB_SUCCESS);
		}
	}

/* averaging along both axes */
	vis_write_gsr(5 << 3);

	if (((((mlib_addr)p_dst | dst_stride) & 3) |
	    (((mlib_addr)p_src | src_stride) & 7)) == 0) {

		if (((dst_stride & 3) == 0) || (dst_width <= 4)) {
			beg =
			    (dst_width <=
			    4) ? dst_width : (4 - (mlib_addr)p_dst) & 3;
			for (j = 0; j < beg; j++) {
				for (i = 0; i < dst_height; i++) {
					p_dst[i * dst_stride + j] =
					    (p_src[2 * i * src_stride + 2 * j] +
					    p_src[2 * i * src_stride + 2 * j +
					    1] + p_src[(2 * i +
					    1) * src_stride + 2 * j] +
					    p_src[(2 * i + 1) * src_stride +
					    2 * j + 1]) >> 2;
				}
			}

			dst_width -= beg;
			p_src += 2 * beg;
			p_dst += beg;

			if (dst_width <= 0)
				return (MLIB_SUCCESS);
		}

		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
			for (j = 0; j <= (dst_width - 4); j += 4) {
				dx1 = ((mlib_d64 *)sp0)[0];
				dx2 = ((mlib_d64 *)(sp0 + src_stride))[0];
				BILINEAR_U8_1_XY;
				((mlib_f32 *)dp)[0] = fr;
				sp0 += 8;
				dp += 4;
			}

			for (; j < dst_width; j++) {
				(*dp++) = (sp0[0] + sp0[1]
				    + sp0[src_stride] + sp0[src_stride +
				    1]) >> 2;
				sp0 += 2;
			}
		}

		return (MLIB_SUCCESS);
	} else {
		mlib_d64 buff_lcl[BUFF_SIZE / 8], *buffd = buff_lcl, *buff0;
		mlib_u8 *sl = p_src, *sp0, *sp1;
		mlib_u8 *dl = p_dst, *dp0;
		mlib_d64 da0, da1, *ap0, *ap1, *dp;
		mlib_s32 n, bsize;

		bsize = ((dst_width + 7) & ~7) + 2 * dst_width;

		if (bsize > BUFF_SIZE) {
			buffd = __mlib_malloc(bsize);

			if (buffd == NULL)
				return (MLIB_FAILURE);
		}

		buff0 = buffd + ((dst_width + 7) / 8);

		for (i = 0; i < dst_height; i++) {
			n = dst_width;
			sp0 = sl;
			sp1 = sl += src_stride;
			dp0 = dl;

			if (((mlib_s32)sp1 & 1) == 0) {
				mlib_u8 *spT = sp0;

				sp0 = sp1;
				sp1 = spT;
			}

			if (((mlib_s32)sp0 & 1) == 0) {
				while (((mlib_s32)sp0 & 7) && (n > 0)) {
					(*dp0++) =
					    (sp0[0] + sp1[0] + sp0[1] +
					    sp1[1]) >> 2;
					sp0 += 2;
					sp1 += 2;
					n--;
				}
			}

			if ((mlib_s32)sp0 & 7) {
				mlib_ImageCopy_na((void *)sp0, (void *)buff0,
				    2 * n);
				sp0 = (void *)buff0;
			}

			if ((mlib_s32)dp0 & 3) {
				dp = buffd;
			} else {
				dp = (mlib_d64 *)dp0;
			}

			ap0 = (mlib_d64 *)sp0;
			ap1 = vis_alignaddr(sp1, 0);

			da1 = (*ap1++);
#pragma pipeloop(0)
			for (j = 0; j < n / 4; j++) {
				dx1 = ap0[j];
				da0 = da1;
				da1 = ap1[j];
				dx2 = vis_faligndata(da0, da1);
				BILINEAR_U8_1_XY;
				((mlib_f32 *)dp)[j] = fr;
			}

			if ((mlib_s32)dp0 & 3) {
				mlib_ImageCopy_na((void *)buffd, (void *)dp0,
				    n & ~3);
			}

			for (j = 4 * j; j < n; j++) {
				dp0[j] =
				    (sp0[2 * j] + sp1[2 * j] + sp0[2 * j + 1] +
				    sp1[2 * j + 1]) >> 2;
			}

			sl += src_stride;
			dl += dst_stride;
		}

		if (buffd != buff_lcl)
			__mlib_free(buffd);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

#define	BILINEAR_U8_2_XY                                                   \
	{                                                                  \
	    mlib_d64 dr2h, dr2l;                                           \
	                                                                   \
	    dx1h = vis_fmul8x16al(vis_read_hi(dx1), fone);                 \
	    dx1l = vis_fmul8x16al(vis_read_lo(dx1), fone);                 \
	    dr2h = vis_fmul8x16al(vis_read_hi(dx2), fone);                 \
	    dr2l = vis_fmul8x16al(vis_read_lo(dx2), fone);                 \
	    dx1 = vis_fmul8x16al(vis_read_hi(dx3), fone);                  \
	    dx3 = vis_fmul8x16al(vis_read_lo(dx3), fone);                  \
	    dx2 = vis_fmul8x16al(vis_read_hi(dx4), fone);                  \
	    dx4 = vis_fmul8x16al(vis_read_lo(dx4), fone);                  \
	    dx1 = vis_fpadd16(dx1h, dx1);                                  \
	    dx3 = vis_fpadd16(dx1l, dx3);                                  \
	    dx2 = vis_fpadd16(dr2h, dx2);                                  \
	    dx4 = vis_fpadd16(dr2l, dx4);                                  \
	    dr = vis_fpack16_pair(vis_freg_pair(vis_fpadd16s(vis_read_hi   \
		(dx1), vis_read_lo(dx1)), vis_fpadd16s(vis_read_hi(dx3),   \
		vis_read_lo(dx3))),                                        \
		vis_freg_pair(vis_fpadd16s(vis_read_hi(dx2),               \
		vis_read_lo(dx2)), vis_fpadd16s(vis_read_hi(dx4),          \
		vis_read_lo(dx4))));                                       \
	}

/* *********************************************************** */

#define	BILINEAR_U8_2_X                                                  \
	dx1h = vis_fmul8x16al(vis_read_hi(dx1), fone);                   \
	dx1l = vis_fmul8x16al(vis_read_lo(dx1), fone);                   \
	dx2l = vis_fmul8x16al(vis_read_hi(dx2), fone);                   \
	dx2 = vis_fmul8x16al(vis_read_lo(dx2), fone);                    \
	dr =                                                             \
	vis_fpack16_pair(vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1h),   \
	    vis_read_lo(dx1h)), vis_fpadd16s(vis_read_hi(dx1l),          \
	    vis_read_lo(dx1l))),                                         \
	    vis_freg_pair(vis_fpadd16s(vis_read_hi(dx2l),                \
	    vis_read_lo(dx2l)), vis_fpadd16s(vis_read_hi(dx2),           \
	    vis_read_lo(dx2))))

/* *********************************************************** */

#define	BILINEAR_U8_2_Y                                                    \
	{                                                                  \
	    mlib_d64 dr3h, dr4h;                                           \
	                                                                   \
	    dx1h = vis_fmul8x16al(vis_read_hi(dx1), fone);                 \
	    dx1 = vis_fmul8x16al(vis_read_lo(dx1), fone);                  \
	    dx1l = vis_fmul8x16al(vis_read_hi(dx2), fone);                 \
	    dx2 = vis_fmul8x16al(vis_read_lo(dx2), fone);                  \
	    dr3h = vis_fmul8x16al(vis_read_hi(dx3), fone);                 \
	    dx3 = vis_fmul8x16al(vis_read_lo(dx3), fone);                  \
	    dr4h = vis_fmul8x16al(vis_read_hi(dx4), fone);                 \
	    dx4 = vis_fmul8x16al(vis_read_lo(dx4), fone);                  \
	    dr = vis_fpack16_pair(vis_freg_pair(vis_fpadd16s(vis_read_hi   \
		(dx1h), vis_read_hi(dr3h)),                                \
		vis_fpadd16s(vis_read_hi(dx1), vis_read_hi(dx3))),         \
		vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1l),              \
		vis_read_hi(dr4h)), vis_fpadd16s(vis_read_hi(dx2),         \
		vis_read_hi(dx4))));                                       \
	}

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U8_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8))
{
	mlib_d64 buff_lcl[BUFF_SIZE / 8], *buffd = buff_lcl;
	mlib_s32 bsize = (2 * dst_width + 7) & ~7;
	mlib_u8 *sp0, *dp;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s32 beg, emask;
	mlib_s32 i, j;

	vis_write_gsr(6 << 3);

	if (dh) {
		if (bsize > BUFF_SIZE) {
			buffd = __mlib_malloc(bsize);

			if (buffd == NULL)
				return (MLIB_FAILURE);
		}

		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;

			if (((mlib_s32)dp & 1) == 0) {
				beg = ((8 - (mlib_addr)dp) & 7) >> 1;

				if (beg > dst_width)
					beg = dst_width;
				for (j = 0; j < beg; j++) {
					(*dp++) = (sp0[0] + sp0[2]) >> 1;
					(*dp++) = (sp0[1] + sp0[3]) >> 1;
					sp0 += 4;
				}

				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = dsp0[0];
				dx1l = vis_ld_d64_nf(dsp0 + 1);
				dx2h = vis_ld_d64_nf(dsp0 + 2);
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2 = vis_faligndata(dx1l, dx2h);
					BILINEAR_U8_2_X;
					dx1h = dx2h;
					dx1l = vis_ld_d64_nf(dsp0 + 3);
					dx2h = vis_ld_d64_nf(dsp0 + 4);
					((mlib_d64 *)dp)[0] = dr;
					dp += 8;
					sp0 += 16;
					dsp0 += 2;
				}

				if (j < dst_width) {
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2 = vis_faligndata(dx1l, dx2h);
					BILINEAR_U8_2_X;
					emask =
					    vis_edge8((void *)0,
					    (void *)(2 * (dst_width - j) - 1));
					vis_pst_8(dr, dp, emask);
				}
			} else {
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = dsp0[0];
				dx1l = vis_ld_d64_nf(dsp0 + 1);
				dx2h = vis_ld_d64_nf(dsp0 + 2);
#pragma pipeloop(0)
				for (j = 0; j < (dst_width + 3) / 4; j++) {
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2 = vis_faligndata(dx1l, dx2h);
					BILINEAR_U8_2_X;
					dx1h = dx2h;
					dx1l = vis_ld_d64_nf(dsp0 + 3);
					dx2h = vis_ld_d64_nf(dsp0 + 4);
					buffd[j] = dr;
					dsp0 += 2;
				}

				mlib_ImageCopy_na((void *)buffd, (void *)dp,
				    2 * dst_width);
			}
		}

		if (buffd != buff_lcl)
			__mlib_free(buffd);
		return (MLIB_SUCCESS);
	}

	if (dw) {
		if ((((mlib_s32)p_dst | dst_stride | (mlib_s32)p_src |
		    src_stride) & 7) == 0) {
			emask =
			    vis_edge8((void *)0,
			    (void *)((dst_width & 3) * 2 - 1));
			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j <= (dst_width - 4); j += 4) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 = ((mlib_d64 *)sp0)[1];
					dx3 =
					    ((mlib_d64 *)(sp0 + src_stride))[0];
					dx4 =
					    ((mlib_d64 *)(sp0 + src_stride))[1];
					BILINEAR_U8_2_Y;
					((mlib_d64 *)dp)[0] = dr;
					dp += 8;
					sp0 += 16;
				}

				if (j < dst_width) {
					dx1 = vis_ld_d64_nf((mlib_d64 *)sp0);
					dx2 = vis_ld_d64_nf((mlib_d64 *)sp0 +
						1);
					dx3 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride));
					dx4 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 1);
					BILINEAR_U8_2_Y;
					vis_pst_8(dr, dp, emask);
				}
			}

			return (MLIB_SUCCESS);
		} else {
			mlib_d64 *buff0;
			mlib_u8 *sl = p_src, *sp0, *sp1;
			mlib_u8 *dl = p_dst, *dp0;
			mlib_d64 da0, da1, da2, *ap0, *ap1, *dp;
			mlib_s32 n;

/* for source line */
			bsize += 4 * dst_width;

			if (bsize > BUFF_SIZE) {
				buffd = __mlib_malloc(bsize);

				if (buffd == NULL)
					return (MLIB_FAILURE);
			}

			buff0 = buffd + ((2 * dst_width + 7) / 8);

			for (i = 0; i < dst_height; i++) {
				n = dst_width;
				sp0 = sl;
				sp1 = sl += src_stride;
				dp0 = dl;

				if (((mlib_s32)sp1 & 3) == 0) {
					mlib_u8 *spT = sp0;

					sp0 = sp1;
					sp1 = spT;
				}

				if (((mlib_s32)sp0 & 7) == 4) {
					(*dp0++) = (sp0[0] + sp1[0]) >> 1;
					(*dp0++) = (sp0[1] + sp1[1]) >> 1;
					sp0 += 4;
					sp1 += 4;
					n--;
				}

				if ((mlib_s32)sp0 & 7) {
					mlib_ImageCopy_na((void *)sp0,
					    (void *)buff0, 4 * n);
					sp0 = (void *)buff0;
				}

				if ((mlib_s32)dp0 & 7) {
					dp = buffd;
				} else {
					dp = (mlib_d64 *)dp0;
				}

				ap0 = (mlib_d64 *)sp0;
				ap1 = vis_alignaddr(sp1, 0);

				da2 = (*ap1++);
#pragma pipeloop(0)
				for (j = 0; j < n / 4; j++) {
					da0 = da2;
					dx1 = ap0[2 * j];
					dx2 = ap0[2 * j + 1];
					da1 = ap1[2 * j];
					da2 = ap1[2 * j + 1];
					dx3 = vis_faligndata(da0, da1);
					dx4 = vis_faligndata(da1, da2);
					BILINEAR_U8_2_Y;
					((mlib_d64 *)dp)[j] = dr;
				}

				if ((mlib_s32)dp0 & 7) {
					mlib_ImageCopy_na((void *)buffd,
					    (void *)dp0, 2 * (n & ~3));
				}

				for (j = 4 * j; j < n; j++) {
					dp0[2 * j] =
					    (sp0[4 * j] + sp1[4 * j]) >> 1;
					dp0[2 * j + 1] =
					    (sp0[4 * j + 1] + sp1[4 * j +
					    1]) >> 1;
				}

				sl += src_stride;
				dl += dst_stride;
			}

			if (buffd != buff_lcl)
				__mlib_free(buffd);
			return (MLIB_SUCCESS);
		}
	}

/* averaging along both axes */
	vis_write_gsr(5 << 3);

	if ((((mlib_s32)p_dst | dst_stride | (mlib_s32)p_src | src_stride) & 7)
	    == 0) {
		emask = vis_edge8((void *)0, (void *)((dst_width & 3) * 2 - 1));
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
			for (j = 0; j <= (dst_width - 4); j += 4) {
				dx1 = ((mlib_d64 *)sp0)[0];
				dx2 = ((mlib_d64 *)sp0)[1];
				dx3 = ((mlib_d64 *)(sp0 + src_stride))[0];
				dx4 = ((mlib_d64 *)(sp0 + src_stride))[1];
				BILINEAR_U8_2_XY;
				((mlib_d64 *)dp)[0] = dr;
				dp += 8;
				sp0 += 16;
			}

			if (j < dst_width) {
				dx1 = ((mlib_d64 *)sp0)[0];
				dx2 = ((mlib_d64 *)sp0)[1];
				dx3 = ((mlib_d64 *)(sp0 + src_stride))[0];
				dx4 = ((mlib_d64 *)(sp0 + src_stride))[1];
				BILINEAR_U8_2_XY;
				vis_pst_8(dr, dp, emask);
			}
		}

		return (MLIB_SUCCESS);
	} else {
		mlib_d64 *buff0;
		mlib_u8 *sl = p_src, *sp0, *sp1;
		mlib_u8 *dl = p_dst, *dp0;
		mlib_d64 da0, da1, da2, *ap0, *ap1, *dp;
		mlib_s32 n;

/* for source line */
		bsize += 4 * dst_width;

		if (bsize > BUFF_SIZE) {
			buffd = __mlib_malloc(bsize);

			if (buffd == NULL)
				return (MLIB_FAILURE);
		}

		buff0 = buffd + ((2 * dst_width + 7) / 8);

		for (i = 0; i < dst_height; i++) {
			n = dst_width;
			sp0 = sl;
			sp1 = sl += src_stride;
			dp0 = dl;

			if (((mlib_s32)sp1 & 3) == 0) {
				mlib_u8 *spT = sp0;

				sp0 = sp1;
				sp1 = spT;
			}

			if (((mlib_s32)sp0 & 7) == 4) {
				(*dp0++) =
				    (sp0[0] + sp1[0] + sp0[2] + sp1[2]) >> 2;
				(*dp0++) =
				    (sp0[1] + sp1[1] + sp0[3] + sp1[3]) >> 2;
				sp0 += 4;
				sp1 += 4;
				n--;
			}

			if ((mlib_s32)sp0 & 7) {
				mlib_ImageCopy_na((void *)sp0, (void *)buff0,
				    4 * n);
				sp0 = (void *)buff0;
			}

			if ((mlib_s32)dp0 & 7) {
				dp = buffd;
			} else {
				dp = (mlib_d64 *)dp0;
			}

			ap0 = (mlib_d64 *)sp0;
			ap1 = vis_alignaddr(sp1, 0);

			da2 = (*ap1++);
#pragma pipeloop(0)
			for (j = 0; j < n / 4; j++) {
				da0 = da2;
				dx1 = ap0[2 * j];
				dx2 = ap0[2 * j + 1];
				da1 = ap1[2 * j];
				da2 = ap1[2 * j + 1];
				dx3 = vis_faligndata(da0, da1);
				dx4 = vis_faligndata(da1, da2);
				BILINEAR_U8_2_XY;
				((mlib_d64 *)dp)[j] = dr;
			}

			if ((mlib_s32)dp0 & 7) {
				mlib_ImageCopy_na((void *)buffd, (void *)dp0,
				    2 * (n & ~3));
			}

			for (j = 4 * j; j < n; j++) {
				dp0[2 * j] =
				    (sp0[4 * j] + sp1[4 * j] + sp0[4 * j + 2] +
				    sp1[4 * j + 2]) >> 2;
				dp0[2 * j + 1] =
				    (sp0[4 * j + 1] + sp1[4 * j + 1] +
				    sp0[4 * j + 3] + sp1[4 * j + 3]) >> 2;
			}

			sl += src_stride;
			dl += dst_stride;
		}

		if (buffd != buff_lcl)
			__mlib_free(buffd);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

#define	PACK_U8_3                                               \
	dr2 = vis_faligndata(dr1, dr3);                         \
	dr4 = vis_faligndata(dr3, dr3);                         \
	(void *)vis_alignaddr(dsp0, 3);                         \
	dr5 = vis_faligndata(dr3, dr3);                         \
	dr1 = vis_faligndata(dr3, dr1);                         \
	(void *)vis_alignaddr(dsp0, 5);                         \
	dr = vis_faligndata(dr1, dr2);                          \
	(void *)vis_alignaddr(dsp0, 7);                         \
	dr1 = vis_faligndata(dr5, dr4)

/* *********************************************************** */

#define	BILINEAR_U8_3_X                                         \
	(void *)vis_alignaddr(dsp0, 3);                         \
	dr1 = vis_fmul8x16al(vis_read_hi(dx0), fone);           \
	dr5 = vis_faligndata(dx0, dx0);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr1 = vis_fpadd16(dr1, dr5);                            \
	dr3 = vis_fmul8x16al(vis_read_lo(dx1), fone);           \
	dr5 = vis_faligndata(dx1, dx2);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr3 = vis_fpadd16(dr3, dr5);                            \
	(void *)vis_alignaddr(dsp0, 5);                         \
	dr2 = vis_fmul8x16al(vis_read_hi(dx1), fone);           \
	dr5 = vis_faligndata(dx0, dx1);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr2 = vis_fpadd16(dr2, dr5);                            \
	dr4 = vis_fmul8x16al(vis_read_lo(dx2), fone);           \
	dr5 = vis_faligndata(dx1, dx2);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr4 = vis_fpadd16(dr4, dr5);                            \
	dr1 = vis_fpack16_pair(dr1, dr2);                       \
	dr3 = vis_fpack16_pair(dr3, dr4);                       \
	PACK_U8_3

/* *********************************************************** */

#define	BILINEAR_U8_3_Y                                         \
	dr1 = vis_fmul8x16al(vis_read_hi(dx0), fone);           \
	dr5 = vis_fmul8x16al(vis_read_hi(dx3), fone);           \
	dr1 = vis_fpadd16(dr1, dr5);                            \
	dr3 = vis_fmul8x16al(vis_read_lo(dx1), fone);           \
	dr5 = vis_fmul8x16al(vis_read_lo(dx4), fone);           \
	dr3 = vis_fpadd16(dr3, dr5);                            \
	(void *)vis_alignaddr(dsp0, 5);                         \
	dr2 = vis_faligndata(dx0, dx1);                         \
	dr2 = vis_fmul8x16al(vis_read_hi(dr2), fone);           \
	dr5 = vis_faligndata(dx3, dx4);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr2 = vis_fpadd16(dr2, dr5);                            \
	dr4 = vis_faligndata(dx1, dx2);                         \
	dr4 = vis_fmul8x16al(vis_read_lo(dr4), fone);           \
	dr5 = vis_faligndata(dx4, dx5);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr4 = vis_fpadd16(dr4, dr5);                            \
	dr1 = vis_fpack16_pair(dr1, dr2);                       \
	dr3 = vis_fpack16_pair(dr3, dr4);                       \
	PACK_U8_3

/* *********************************************************** */

#define	BILINEAR_U8_3_XY                                        \
	dr1 = vis_fmul8x16al(vis_read_hi(dx0), fone);           \
	dr5 = vis_fmul8x16al(vis_read_hi(dx3), fone);           \
	dr1 = vis_fpadd16(dr1, dr5);                            \
	(void *)vis_alignaddr(dsp0, 3);                         \
	dr5 = vis_faligndata(dx0, dx0);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr1 = vis_fpadd16(dr1, dr5);                            \
	dr5 = vis_faligndata(dx3, dx3);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr1 = vis_fpadd16(dr1, dr5);                            \
	dr3 = vis_fmul8x16al(vis_read_lo(dx1), fone);           \
	dr5 = vis_fmul8x16al(vis_read_lo(dx4), fone);           \
	dr3 = vis_fpadd16(dr3, dr5);                            \
	dr5 = vis_faligndata(dx1, dx2);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr3 = vis_fpadd16(dr3, dr5);                            \
	dr5 = vis_faligndata(dx4, dx5);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr3 = vis_fpadd16(dr3, dr5);                            \
	(void *)vis_alignaddr(dsp0, 5);                         \
	dr2 = vis_fmul8x16al(vis_read_hi(dx1), fone);           \
	dr5 = vis_fmul8x16al(vis_read_hi(dx4), fone);           \
	dr2 = vis_fpadd16(dr2, dr5);                            \
	dr5 = vis_faligndata(dx0, dx1);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr2 = vis_fpadd16(dr2, dr5);                            \
	dr5 = vis_faligndata(dx3, dx4);                         \
	dr5 = vis_fmul8x16al(vis_read_hi(dr5), fone);           \
	dr2 = vis_fpadd16(dr2, dr5);                            \
	dr4 = vis_fmul8x16al(vis_read_lo(dx2), fone);           \
	dr5 = vis_fmul8x16al(vis_read_lo(dx5), fone);           \
	dr4 = vis_fpadd16(dr4, dr5);                            \
	dr5 = vis_faligndata(dx1, dx2);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr4 = vis_fpadd16(dr4, dr5);                            \
	dr5 = vis_faligndata(dx4, dx5);                         \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fone);           \
	dr4 = vis_fpadd16(dr4, dr5);                            \
	dr1 = vis_fpack16_pair(dr1, dr2);                       \
	dr3 = vis_fpack16_pair(dr3, dr4);                       \
	PACK_U8_3

/* *********************************************************** */

mlib_status
mlib_U8_3_XY_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;
	mlib_u8 *sp0, *dp;
	mlib_d64 dx0, dx1, dx2, dx33, dx3, dx4, dx5, dx66;
	mlib_d64 *dsp0, *dpstr2, dr, dr1, dr2, dr3, dr4, dr5;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s32 beg;

	vis_write_gsr(5 << 3);
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		beg = (mlib_s32)dp & 3;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			dp[0] =
			    (sp0[0] + sp0[3] + sp0[src_stride] +
			    sp0[src_stride + 3]) >> 2;
			dp[1] =
			    (sp0[1] + sp0[4] + sp0[src_stride + 1] +
			    sp0[src_stride + 4]) >> 2;
			dp[2] =
			    (sp0[2] + sp0[5] + sp0[src_stride + 2] +
			    sp0[src_stride + 5]) >> 2;
			sp0 += 6;
			dp += 3;
		}

		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
		dx0 = dsp0[0];
		dx1 = vis_ld_d64_nf(dsp0 + 1);
		dx2 = vis_ld_d64_nf(dsp0 + 2);
		dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, src_stride);
		dx3 = dpstr2[0];
		dx4 = vis_ld_d64_nf(dpstr2 + 1);
		dx5 = vis_ld_d64_nf(dpstr2 + 2);

		if ((((mlib_s32)sp0 | src_stride) & 7) == 0) {
			for (; j <= (dst_width - 4); j += 4) {
				BILINEAR_U8_3_XY;
				dx0 = vis_ld_d64_nf((mlib_d64 *)sp0 + 3);
				dx1 = vis_ld_d64_nf((mlib_d64 *)sp0 + 4);
				dx2 = vis_ld_d64_nf((mlib_d64 *)sp0 + 5);
				dx3 = vis_ld_d64_nf(
					(mlib_d64 *)(sp0 + src_stride) + 3);
				dx4 = vis_ld_d64_nf(
					(mlib_d64 *)(sp0 + src_stride) + 4);
				dx5 = vis_ld_d64_nf(
					(mlib_d64 *)(sp0 + src_stride) + 5);
				((mlib_f32 *)dp)[0] = vis_read_hi(dr);
				((mlib_f32 *)dp)[1] = vis_read_lo(dr);
				((mlib_f32 *)dp)[2] = vis_read_hi(dr1);
				dp += 12;
				sp0 += 24;
			}
		} else {
			dx66 = vis_ld_d64_nf(dpstr2 + 3);
			dx33 = vis_ld_d64_nf(dsp0 + 3);
			for (; j <= (dst_width - 4); j += 4) {
				dx3 = vis_faligndata(dx3, dx4);
				dx4 = vis_faligndata(dx4, dx5);
				dx5 = vis_faligndata(dx5, dx66);
				vis_alignaddr(sp0, 0);
				dx0 = vis_faligndata(dx0, dx1);
				dx1 = vis_faligndata(dx1, dx2);
				dx2 = vis_faligndata(dx2, dx33);
				BILINEAR_U8_3_XY;
				dx0 = dx33;
				dx1 = vis_ld_d64_nf(dsp0 + 4);
				dx2 = vis_ld_d64_nf(dsp0 + 5);
				dx33 = vis_ld_d64_nf(dsp0 + 6);
				dx3 = dx66;
				dx4 = vis_ld_d64_nf(dpstr2 + 4);
				dx5 = vis_ld_d64_nf(dpstr2 + 5);
				dx66 = vis_ld_d64_nf(dpstr2 + 6);
				((mlib_f32 *)dp)[0] = vis_read_hi(dr);
				((mlib_f32 *)dp)[1] = vis_read_lo(dr);
				((mlib_f32 *)dp)[2] = vis_read_hi(dr1);
				vis_alignaddr(sp0, src_stride);
				dp += 12;
				sp0 += 24;
				dsp0 += 3;
				dpstr2 += 3;
			}
		}

		for (; j < dst_width; j++) {
			dp[0] =
			    (sp0[0] + sp0[3] + sp0[src_stride] +
			    sp0[src_stride + 3]) >> 2;
			dp[1] =
			    (sp0[1] + sp0[4] + sp0[src_stride + 1] +
			    sp0[src_stride + 4]) >> 2;
			dp[2] =
			    (sp0[2] + sp0[5] + sp0[src_stride + 2] +
			    sp0[src_stride + 5]) >> 2;
			sp0 += 6;
			dp += 3;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U8_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;
	mlib_u8 *sp0, *dp;
	mlib_d64 dx0, dx1, dx2, dx33, dx3, dx4, dx5, dx66;
	mlib_d64 *dsp0, *dpstr2, dr, dr1, dr2, dr3, dr4, dr5;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s32 beg;

	vis_write_gsr(6 << 3);

/* averaging along X axis */

	if (dh) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			beg = (mlib_s32)dp & 3;

			if (beg > dst_width)
				beg = dst_width;
			for (j = 0; j < beg; j++) {
				dp[0] = (sp0[0] + sp0[3]) >> 1;
				dp[1] = (sp0[1] + sp0[4]) >> 1;
				dp[2] = (sp0[2] + sp0[5]) >> 1;
				sp0 += 6;
				dp += 3;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx0 = vis_ld_d64_nf(dsp0);
			dx1 = vis_ld_d64_nf(dsp0 + 1);
			dx2 = vis_ld_d64_nf(dsp0 + 2);

			if (((mlib_s32)sp0 & 7) == 0) {
				for (; j <= (dst_width - 4); j += 4) {
					BILINEAR_U8_3_X;
					dx0 = ((mlib_d64 *)sp0)[3];
					dx1 = ((mlib_d64 *)sp0)[4];
					dx2 = ((mlib_d64 *)sp0)[5];
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_hi(dr1);
					dp += 12;
					sp0 += 24;
				}
			} else {
				dx3 = vis_ld_d64_nf(dsp0 + 3);
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx0 = vis_faligndata(dx0, dx1);
					dx1 = vis_faligndata(dx1, dx2);
					dx2 = vis_faligndata(dx2, dx3);
					BILINEAR_U8_3_X;
					dx0 = dx3;
					dx1 = vis_ld_d64_nf(dsp0 + 4);
					dx2 = vis_ld_d64_nf(dsp0 + 5);
					dx3 = vis_ld_d64_nf(dsp0 + 6);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_hi(dr1);
					vis_alignaddr(sp0, 0);
					dp += 12;
					sp0 += 24;
					dsp0 += 3;
				}
			}

			for (; j < dst_width; j++) {
				dp[0] = (sp0[0] + sp0[3]) >> 1;
				dp[1] = (sp0[1] + sp0[4]) >> 1;
				dp[2] = (sp0[2] + sp0[5]) >> 1;
				sp0 += 6;
				dp += 3;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */

	if (dw) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			beg = (mlib_s32)dp & 3;

			if (beg > dst_width)
				beg = dst_width;
			for (j = 0; j < beg; j++) {
				dp[0] = (sp0[0] + sp0[src_stride]) >> 1;
				dp[1] = (sp0[1] + sp0[src_stride + 1]) >> 1;
				dp[2] = (sp0[2] + sp0[src_stride + 2]) >> 1;
				sp0 += 6;
				dp += 3;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx0 = vis_ld_d64_nf(dsp0);
			dx1 = vis_ld_d64_nf(dsp0 + 1);
			dx2 = vis_ld_d64_nf(dsp0 + 2);
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, src_stride);
			dx3 = vis_ld_d64_nf(dpstr2 + 0);
			dx4 = vis_ld_d64_nf(dpstr2 + 1);
			dx5 = vis_ld_d64_nf(dpstr2 + 2);

			if ((((mlib_s32)sp0 | src_stride) & 7) == 0) {
				for (; j <= (dst_width - 4); j += 4) {
					BILINEAR_U8_3_Y;
					dx0 = ((mlib_d64 *)sp0)[3];
					dx1 = ((mlib_d64 *)sp0)[4];
					dx2 = ((mlib_d64 *)sp0)[5];
					dx3 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 3);
					dx4 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 4);
					dx5 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 5);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_hi(dr1);
					dp += 12;
					sp0 += 24;
				}
			} else {
				dx66 = vis_ld_d64_nf(dpstr2 + 3);
				dx33 = vis_ld_d64_nf(dsp0 + 3);
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx3 = vis_faligndata(dx3, dx4);
					dx4 = vis_faligndata(dx4, dx5);
					dx5 = vis_faligndata(dx5, dx66);
					vis_alignaddr(sp0, 0);
					dx0 = vis_faligndata(dx0, dx1);
					dx1 = vis_faligndata(dx1, dx2);
					dx2 = vis_faligndata(dx2, dx33);
					BILINEAR_U8_3_Y;
					dx0 = dx33;
					dx1 = dsp0[4];
					dx2 = dsp0[5];
					dx33 = dsp0[6];
					dx3 = dx66;
					dx4 = vis_ld_d64_nf(dpstr2 + 4);
					dx5 = vis_ld_d64_nf(dpstr2 + 5);
					dx66 = vis_ld_d64_nf(dpstr2 + 6);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_hi(dr1);
					vis_alignaddr(sp0, src_stride);
					dp += 12;
					sp0 += 24;
					dsp0 += 3;
					dpstr2 += 3;
				}
			}

			for (; j < dst_width; j++) {
				dp[0] = (sp0[0] + sp0[src_stride]) >> 1;
				dp[1] = (sp0[1] + sp0[src_stride + 1]) >> 1;
				dp[2] = (sp0[2] + sp0[src_stride + 2]) >> 1;
				sp0 += 6;
				dp += 3;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	return (mlib_U8_3_XY_Bilinear(BILINEAR_P));
}

/* *********************************************************** */

#define	BILINEAR_U8_4_X                                         \
	dr = vis_fmul8x16al(vis_read_lo(dx1), fone);            \
	dx1 = vis_fpmerge(fzero, vis_read_hi(dx1));             \
	dx1 = vis_fpadd16(dx1, dr);                             \
	fr = vis_fpack16(dx1)

/* *********************************************************** */

#define	BILINEAR_U8_4_Y                                         \
	dx1 = vis_fmul8x16al(vis_read_hi(dx1), fone);           \
	dx2 = vis_fmul8x16al(vis_read_hi(dx2), fone);           \
	dr = vis_fpadd16(dx1, dx2);                             \
	fr = vis_fpack16(dr)

/* *********************************************************** */

#define	BILINEAR_U8_4_XY                                        \
	dx3 = vis_fmul8x16al(vis_read_lo(dx1), fone);           \
	dx1 = vis_fmul8x16al(vis_read_hi(dx1), fone);           \
	dx4 = vis_fmul8x16al(vis_read_lo(dx2), fone);           \
	dx2 = vis_fpmerge(fzero, vis_read_hi(dx2));             \
	dx1 = vis_fpadd16(dx1, dx3);                            \
	dx2 = vis_fpadd16(dx2, dx4);                            \
	dr = vis_fpadd16(dx1, dx2);                             \
	fr = vis_fpack16(dr)

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U8_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8))
{
	mlib_d64 buff_lcl[BUFF_SIZE / 8], *buffd = buff_lcl;
	mlib_s32 bsize = (4 * dst_width + 7) & ~7;
	mlib_u8 *sp0, *dp;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0;
	mlib_f32 fr, fzero = vis_to_float(0), fone = vis_to_float(0x100);
	mlib_s32 i, j = 0;

	vis_write_gsr(6 << 3);

/* averaging along X axis */

	if (dh) {
		if (((((mlib_s32)p_dst | dst_stride) & 3) |
		    (((mlib_s32)p_src | src_stride) & 7)) == 0) {
			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = ((mlib_d64 *)sp0)[0];
					BILINEAR_U8_4_X;
					((mlib_f32 *)dp)[0] = fr;
					sp0 += 8;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}

		if (bsize > BUFF_SIZE) {
			buffd = __mlib_malloc(bsize);

			if (buffd == NULL)
				return (MLIB_FAILURE);
		}

		for (i = 0; i < dst_height; i++) {
			mlib_u8 *dl;
			mlib_f32 *dp;

			sp0 = p_src + 2 * i * src_stride;
			dl = p_dst + i * dst_stride;

			if ((mlib_s32)dl & 3) {
				dp = (void *)buffd;
			} else {
				dp = (void *)dl;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx3 = (*dsp0++);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx2 = dx3;
				dx3 = dsp0[j];
				dx1 = vis_faligndata(dx2, dx3);
				BILINEAR_U8_4_X;
				dp[j] = fr;
			}

			if ((mlib_s32)dl & 3) {
				mlib_ImageCopy_na((void *)buffd, (void *)dl,
				    4 * dst_width);
			}
		}

		if (buffd != buff_lcl)
			__mlib_free(buffd);
		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */

	if (dw) {
		if (((((mlib_s32)p_dst | dst_stride) & 3) | (src_stride & 7)) ==
		    0) {
			mlib_s32 src_stride8 = src_stride >> 3;

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1l = dsp0[0];
				dx2l = dsp0[src_stride8];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1h = dx1l;
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2h = dx2l;
					dx2l = dsp0[src_stride8 + 1];
					dx2 = vis_faligndata(dx2h, dx2l);
					BILINEAR_U8_4_Y;
					((mlib_f32 *)dp)[0] = fr;
					dsp0++;
					dp += 4;
				}
			}
		} else {
			mlib_d64 *buff0;
			mlib_u8 *sl = p_src, *sp0, *sp1;
			mlib_u8 *dl = p_dst, *dp0;
			mlib_d64 da0, da1, *ap0, *ap1;
			mlib_f32 *dp;

/* for source line */
			bsize += 8 * dst_width;

			if (bsize > BUFF_SIZE) {
				buffd = __mlib_malloc(bsize);

				if (buffd == NULL)
					return (MLIB_FAILURE);
			}

			buff0 = buffd + ((4 * dst_width + 7) / 8);

			for (i = 0; i < dst_height; i++) {
				sp0 = sl;
				sp1 = sl += src_stride;
				dp0 = dl;

				if (((mlib_s32)sp1 & 7) == 0) {
					mlib_u8 *spT = sp0;

					sp0 = sp1;
					sp1 = spT;
				}

				if ((mlib_s32)sp0 & 7) {
					mlib_ImageCopy_na((void *)sp0,
					    (void *)buff0, 8 * dst_width);
					sp0 = (void *)buff0;
				}

				if ((mlib_s32)dp0 & 3) {
					dp = (void *)buffd;
				} else {
					dp = (void *)dp0;
				}

				ap0 = (mlib_d64 *)sp0;
				ap1 = vis_alignaddr(sp1, 0);

				da1 = (*ap1++);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					da0 = da1;
					dx1 = ap0[j];
					da1 = ap1[j];
					dx2 = vis_faligndata(da0, da1);
					BILINEAR_U8_4_Y;
					dp[j] = fr;
				}

				if ((mlib_s32)dp0 & 3) {
					mlib_ImageCopy_na((void *)buffd,
					    (void *)dp0, 4 * dst_width);
				}

				for (; j < dst_width; j++) {
					dp0[4 * j] =
					    (sp0[8 * j] + sp1[8 * j]) >> 1;
					dp0[4 * j + 1] =
					    (sp0[8 * j + 1] + sp1[8 * j +
					    1]) >> 1;
					dp0[4 * j + 2] =
					    (sp0[8 * j + 2] + sp1[8 * j +
					    2]) >> 1;
					dp0[4 * j + 3] =
					    (sp0[8 * j + 3] + sp1[8 * j +
					    3]) >> 1;
				}

				sl += src_stride;
				dl += dst_stride;
			}

			if (buffd != buff_lcl)
				__mlib_free(buffd);
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	vis_write_gsr(5 << 3);

	if (((((mlib_s32)p_dst | dst_stride) & 3) |
	    (((mlib_s32)p_src | src_stride) & 7)) == 0) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx1 = ((mlib_d64 *)sp0)[0];
				dx2 = ((mlib_d64 *)(sp0 + src_stride))[0];
				BILINEAR_U8_4_XY;
				((mlib_f32 *)dp)[0] = fr;
				sp0 += 8;
				dp += 4;
			}
		}

		return (MLIB_SUCCESS);
	} else {
		mlib_d64 *buff0;
		mlib_u8 *sl = p_src, *sp0, *sp1;
		mlib_u8 *dl = p_dst, *dp0;
		mlib_d64 da0, da1, *ap0, *ap1;
		mlib_f32 *dp;

/* for source line */
		bsize += 8 * dst_width;

		if (bsize > BUFF_SIZE) {
			buffd = __mlib_malloc(bsize);

			if (buffd == NULL)
				return (MLIB_FAILURE);
		}

		buff0 = buffd + ((4 * dst_width + 7) / 8);

		for (i = 0; i < dst_height; i++) {
			sp0 = sl;
			sp1 = sl += src_stride;
			dp0 = dl;

			if (((mlib_s32)sp1 & 7) == 0) {
				mlib_u8 *spT = sp0;

				sp0 = sp1;
				sp1 = spT;
			}

			if ((mlib_s32)sp0 & 7) {
				mlib_ImageCopy_na((void *)sp0, (void *)buff0,
				    8 * dst_width);
				sp0 = (void *)buff0;
			}

			if ((mlib_s32)dp0 & 3) {
				dp = (void *)buffd;
			} else {
				dp = (void *)dp0;
			}

			ap0 = (mlib_d64 *)sp0;
			ap1 = vis_alignaddr(sp1, 0);

			da1 = (*ap1++);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				da0 = da1;
				dx1 = ap0[j];
				da1 = ap1[j];
				dx2 = vis_faligndata(da0, da1);
				BILINEAR_U8_4_XY;
				dp[j] = fr;
			}

			if ((mlib_s32)dp0 & 3) {
				mlib_ImageCopy_na((void *)buffd, (void *)dp0,
				    4 * dst_width);
			}

			for (; j < dst_width; j++) {
				dp0[4 * j] =
				    (sp0[8 * j] + sp1[8 * j] + sp0[8 * j + 4] +
				    sp1[8 * j + 4]) >> 2;
				dp0[4 * j + 1] =
				    (sp0[8 * j + 1] + sp1[8 * j + 1] +
				    sp0[8 * j + 5] + sp1[8 * j + 5]) >> 2;
				dp0[4 * j + 2] =
				    (sp0[8 * j + 2] + sp1[8 * j + 2] +
				    sp0[8 * j + 6] + sp1[8 * j + 6]) >> 2;
				dp0[4 * j + 3] =
				    (sp0[8 * j + 3] + sp1[8 * j + 3] +
				    sp0[8 * j + 7] + sp1[8 * j + 7]) >> 2;
			}

			sl += src_stride;
			dl += dst_stride;
		}

		if (buffd != buff_lcl)
			__mlib_free(buffd);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

#define	BILINEAR_S16_1_X                                                \
	dr = vis_fmuld8ulx16(fone, vis_read_hi(dx1));                   \
	dr1 = vis_fmuld8ulx16(fone, vis_read_lo(dx1));                  \
	dx1 =                                                           \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr), vis_read_lo(dr)),   \
	    vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1)));          \
	dr = vis_fmuld8ulx16(fone, vis_read_hi(dx2));                   \
	dr1 = vis_fmuld8ulx16(fone, vis_read_lo(dx2));                  \
	dx2 =                                                           \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr), vis_read_lo(dr)),   \
	    vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1)));          \
	dr = vis_fpackfix_pair(dx1, dx2)

/* *********************************************************** */

#define	BILINEAR_S16_ADD_1_Y                                    \
	dx1 = vis_fand(dx1, dmask_even);                        \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	dx3 = vis_fmul8x16(scale05, dx3);                       \
	dx1 = vis_fpadd16(dx1, dx3);                            \
	dx2 = vis_fand(dx2, dmask_even);                        \
	dx2 = vis_fmul8x16(scale05, dx2);                       \
	dx4 = vis_fmul8x16(scale05, dx4);                       \
	dx2 = vis_fpadd16(dx2, dx4)

/* *********************************************************** */

#define	BILINEAR_S16_1_Y                                         \
	BILINEAR_S16_ADD_1_Y;                                    \
	dr = vis_fpmerge(vis_read_hi(dx1), vis_read_hi(dx2));    \
	dr1 = vis_fpmerge(vis_read_lo(dx1), vis_read_lo(dx2));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));     \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	BILINEAR_S16_1_XY	BILINEAR_S16_ADD_1_Y; BILINEAR_S16_1_X

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_S16_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;
	mlib_s16 *sp0, *dp, *pdend;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr, dr1;
	mlib_d64 *dsp0, *dpstr2, *dpd;
	mlib_d64 dmask_even = vis_to_double_dup(0xfffefffe);
	mlib_f32 scale05 = vis_to_float(0x80808080);
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_s32 beg, off, emask;

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* averaging along X axis */
	if (dh) {
		if (dst_width <= 0)
			return (MLIB_SUCCESS);
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			pdend = dp + dst_width - 1;
			dpd = (mlib_d64 *)((mlib_addr)dp & (~7));
			off = 2 * ((mlib_s32)dpd - (mlib_s32)dp);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, off);
			dx1h = vis_ld_d64_nf(dsp0);
			dx1l = vis_ld_d64_nf(dsp0 + 1);
			dx2h = vis_ld_d64_nf(dsp0 + 2);
			dx1 = vis_faligndata(dx1h, dx1l);
			dx2 = vis_faligndata(dx1l, dx2h);
			BILINEAR_S16_1_X;
			emask = vis_edge16(dp, pdend);
/* store first bytes of result */
			vis_pst_16(dr, dpd, emask);
			dsp0 += 2;
			dpd++;
			j = 4 + (off >> 2);

			if ((((mlib_s32)sp0 + off) & 7) == 0) {
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1 = dsp0[0];
					dx2 = dsp0[1];
					BILINEAR_S16_1_X;
					(*dpd++) = dr;
					dsp0 += 2;
				}

				dx2h = vis_ld_d64_nf(dsp0);
			} else {
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1h = dx2h;
					dx1l = dsp0[1];
					dx2h = dsp0[2];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2 = vis_faligndata(dx1l, dx2h);
					BILINEAR_S16_1_X;
					(*dpd++) = dr;
					dsp0 += 2;
				}
			}

			if (j < dst_width) {
				dx1h = dx2h;
				dx1l = vis_ld_d64_nf(dsp0 + 1);
				dx2h = vis_ld_d64_nf(dsp0 + 2);
				dx1 = vis_faligndata(dx1h, dx1l);
				dx2 = vis_faligndata(dx1l, dx2h);
				BILINEAR_S16_1_X;
				emask = vis_edge16(dpd, pdend);
/* store last bytes of result */
				vis_pst_16(dr, dpd, emask);
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			beg = ((8 - (mlib_addr)dp) & 7) >> 1;

			if (beg > dst_width)
				beg = dst_width;
			for (j = 0; j < beg; j++) {
				(*dp++) = (sp0[0] + sp0[src_stride]) >> 1;
				sp0 += 2;
			}

			if ((src_stride & 7) == 0) {
/* aligned case */
				if (((mlib_addr)sp0 & 7) == 0) {
#pragma pipeloop(0)
					for (; j <= (dst_width - 4); j += 4) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = ((mlib_d64 *)sp0)[1];
						dx3 = vis_ld_d64_nf(
							(mlib_d64 *)(sp0 +
							src_stride));
						dx4 = vis_ld_d64_nf(
							(mlib_d64 *)(sp0 +
							src_stride) + 1);
						BILINEAR_S16_1_Y;
						((mlib_d64 *)dp)[0] = dr;
						sp0 += 8;
						dp += 4;
					}
				} else {
					mlib_s32 src_stride8 = src_stride >> 2;

					dsp0 =
					    (mlib_d64 *)vis_alignaddr(sp0, 0);
					dx1h = dsp0[0];
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8);
#pragma pipeloop(0)
					for (; j <= (dst_width - 4); j += 4) {
						dx1l = dsp0[1];
						dx1 =
						    vis_faligndata(dx1h, dx1l);
						dx1h = dsp0[2];
						dx2 =
						    vis_faligndata(dx1l, dx1h);
						dx2l = vis_ld_d64_nf(dsp0 +
							src_stride8 + 1);
						dx3 =
						    vis_faligndata(dx2h, dx2l);
						dx2h = vis_ld_d64_nf(dsp0 +
							src_stride8 + 2);
						dx4 =
						    vis_faligndata(dx2l, dx2h);
						BILINEAR_S16_1_Y;
						dx4 =
						    vis_faligndata(dx2l, dx2h);
						((mlib_d64 *)dp)[0] = dr;
						dsp0 += 2;
						sp0 += 8;
						dp += 4;
					}
				}
			} else {
				mlib_s32 src_stride2 = src_stride * 2;

				dpstr2 =
				    (mlib_d64 *)vis_alignaddr(sp0, src_stride2);
				dx2h = vis_ld_d64_nf(dpstr2);
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = dsp0[0];
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx1h = dsp0[2];
					dx2 = vis_faligndata(dx1l, dx1h);
					vis_alignaddr(sp0, src_stride2);
					dx2l = vis_ld_d64_nf(dpstr2 + 1);
					dx3 = vis_faligndata(dx2h, dx2l);
					dx2h = vis_ld_d64_nf(dpstr2 + 2);
					dx4 = vis_faligndata(dx2l, dx2h);
					vis_alignaddr(sp0, 0);
					BILINEAR_S16_1_Y;
					((mlib_d64 *)dp)[0] = dr;
					vis_alignaddr(sp0, 0);
					dsp0 += 2;
					dpstr2 += 2;
					sp0 += 8;
					dp += 4;
				}
			}

			for (; j < dst_width; j++) {
				(*dp++) = (sp0[0] + sp0[src_stride]) >> 1;
				sp0 += 2;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		beg = ((8 - (mlib_addr)dp) & 7) >> 1;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			(*dp++) = (sp0[0] + sp0[src_stride]
			    + sp0[1] + sp0[src_stride + 1]) >> 2;
			sp0 += 2;
		}

		if ((src_stride & 7) == 0) {
			if (((mlib_addr)sp0 & 7) == 0) { /* aligned case */
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 = ((mlib_d64 *)sp0)[1];
					dx3 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride));
					dx4 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 1);
					BILINEAR_S16_1_XY;
					((mlib_d64 *)dp)[0] = dr;
					sp0 += 8;
					dp += 4;
				}
			} else {
				mlib_s32 src_stride8 = src_stride >> 2;

				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = dsp0[0];
				dx2h = vis_ld_d64_nf(dsp0 + src_stride8);
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx1h = dsp0[2];
					dx2 = vis_faligndata(dx1l, dx1h);
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8 + 1);
					dx3 = vis_faligndata(dx2h, dx2l);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8 + 2);
					dx4 = vis_faligndata(dx2l, dx2h);
					BILINEAR_S16_1_XY;
					dx4 = vis_faligndata(dx2l, dx2h);
					((mlib_d64 *)dp)[0] = dr;
					dsp0 += 2;
					sp0 += 8;
					dp += 4;
				}
			}
		} else {
			mlib_s32 src_stride2 = src_stride * 2;

			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, src_stride2);
			dx2h = vis_ld_d64_nf(dpstr2);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx1h = dsp0[0];
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				dx1l = dsp0[1];
				dx1 = vis_faligndata(dx1h, dx1l);
				dx1h = dsp0[2];
				dx2 = vis_faligndata(dx1l, dx1h);
				vis_alignaddr(sp0, src_stride2);
				dx2l = vis_ld_d64_nf(dpstr2 + 1);
				dx3 = vis_faligndata(dx2h, dx2l);
				dx2h = vis_ld_d64_nf(dpstr2 + 2);
				dx4 = vis_faligndata(dx2l, dx2h);
				vis_alignaddr(sp0, 0);
				BILINEAR_S16_1_XY;
				((mlib_d64 *)dp)[0] = dr;
				vis_alignaddr(sp0, 0);
				dsp0 += 2;
				dpstr2 += 2;
				sp0 += 8;
				dp += 4;
			}
		}

		for (; j < dst_width; j++) {
			(*dp++) = (sp0[0] + sp0[src_stride]
			    + sp0[1] + sp0[src_stride + 1]) >> 2;
			sp0 += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BILINEAR_S16_2_X                                        \
	dx1 = vis_fand(dx1, dmask_05);                          \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	fr = vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1))

/* *********************************************************** */

#define	BILINEAR_S16_2_X_2                                                \
	dx1 = vis_fand(dx1, dmask_05);                                    \
	dx2 = vis_fand(dx2, dmask_05);                                    \
	dx1 = vis_fmul8x16(scale05, dx1);                                 \
	dx2 = vis_fmul8x16(scale05, dx2);                                 \
	dr =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1)),   \
	    vis_fpadd16s(vis_read_hi(dx2), vis_read_lo(dx2)))

/* *********************************************************** */

#define	BILINEAR_S16_2_XY                                       \
	dx1 = vis_fand(dx1, dmask_025);                         \
	dx1 = vis_fmul8x16(scale025, dx1);                      \
	dx2 = vis_fmul8x16(scale025, dx2);                      \
	dx1 = vis_fpadd16(dx1, dx2);                            \
	fr = vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1))

/* *********************************************************** */

#define	BILINEAR_S16_2_XY_2                                               \
	dx1 = vis_fand(dx1, dmask_025);                                   \
	dx3 = vis_fand(dx3, dmask_025);                                   \
	dx1 = vis_fmul8x16(scale025, dx1);                                \
	dx2 = vis_fmul8x16(scale025, dx2);                                \
	dx3 = vis_fmul8x16(scale025, dx3);                                \
	dx4 = vis_fmul8x16(scale025, dx4);                                \
	dx1 = vis_fpadd16(dx1, dx3);                                      \
	dx2 = vis_fpadd16(dx2, dx4);                                      \
	dr =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1)),   \
	    vis_fpadd16s(vis_read_hi(dx2), vis_read_lo(dx2)))

/* *********************************************************** */

#define	BILINEAR_S16_2_Y                                        \
	dx1 = vis_fand(dx1, dmask_05);                          \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	dx2 = vis_fmul8x16(scale05, dx2);                       \
	fr = vis_fpadd16s(vis_read_hi(dx1), vis_read_hi(dx2))

/* *********************************************************** */

#define	BILINEAR_S16_2_Y_2                                                \
	dx1 = vis_fand(dx1, dmask_05);                                    \
	dx1 = vis_fmul8x16(scale05, dx1);                                 \
	dx3 = vis_fmul8x16(scale05, dx3);                                 \
	dx2 = vis_fand(dx2, dmask_05);                                    \
	dx2 = vis_fmul8x16(scale05, dx2);                                 \
	dx4 = vis_fmul8x16(scale05, dx4);                                 \
	dr =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1), vis_read_hi(dx3)),   \
	    vis_fpadd16s(vis_read_hi(dx2), vis_read_hi(dx4)))

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_S16_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;

	mlib_s16 *sp0, *dp;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0, *dpstr2, *dpd;
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe);
	mlib_d64 dmask_025 = vis_to_double_dup(0xfffcfffc);
	mlib_f32 scale025 = vis_to_float(0x40404040), scale05 =
	    vis_to_float(0x80808080), fr;
	mlib_s32 emask_hi, emask_lo, all_z;

/* averaging along X axis */
	if (dh) {
		if ((((mlib_s32)p_dst | (2 * dst_stride)) & 3) == 0) {
			if ((((mlib_s32)p_src | (2 * src_stride)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						BILINEAR_S16_2_X;
						((mlib_f32 *)dp)[0] = fr;
						sp0 += 4;
						dp += 2;
					}
				}

				return (MLIB_SUCCESS);
			}
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;

			if (((mlib_s32)dp & 3) == 0) {
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1l = dsp0[0];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1h = dx1l;
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					BILINEAR_S16_2_X;
					((mlib_f32 *)dp)[0] = fr;
					dsp0++;
					dp += 2;
				}
			} else {
				dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				emask_hi =
				    0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
				emask_lo = ~emask_hi;
				all_z = 8 - (mlib_addr)dp;
				dx2l = dsp0[0];
				dx2 = dsp0[1];
				dx1 = vis_faligndata(dx2l, dx2);
				dx2l = dsp0[2];
				dx2 = vis_faligndata(dx2, dx2l);
#pragma pipeloop(0)
				for (j = 0; j <= (dst_width - 2); j += 2) {
					BILINEAR_S16_2_X_2;
					dx2 = dsp0[3];
					dx1 = vis_faligndata(dx2l, dx2);
					dx2l = dsp0[4];
					dx2 = vis_faligndata(dx2, dx2l);
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr, dr);
					vis_pst_8(dr, dpd, emask_hi);
					dpd++;
					vis_pst_8(dr, dpd, emask_lo);
					dsp0 += 2;
					vis_alignaddr(sp0, 0);
				}

				if (j < dst_width) {
					dp[2 * j] =
					    (sp0[4 * j] + sp0[4 * j + 2]) >> 1;
					dp[2 * j + 1] =
					    (sp0[4 * j + 1] + sp0[4 * j +
					    3]) >> 1;
				}
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		if ((((mlib_s32)p_dst | (2 * dst_stride)) & 3) == 0) {
			if ((((mlib_s32)p_src | (2 * src_stride)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 =
						    ((mlib_d64 *)(sp0 +
						    src_stride))[0];
						BILINEAR_S16_2_Y;
						((mlib_f32 *)dp)[0] = fr;
						sp0 += 4;
						dp += 2;
					}
				}

				return (MLIB_SUCCESS);
			}

			if ((src_stride & 3) == 0) {
				mlib_s32 src_stride8 = src_stride >> 2;

				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
					dsp0 =
					    (mlib_d64 *)vis_alignaddr(sp0, 0);
					dx1l = dsp0[0];
					dx2l = dsp0[src_stride8];
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1h = dx1l;
						dx1l = dsp0[1];
						dx1 =
						    vis_faligndata(dx1h, dx1l);
						dx2h = dx2l;
						dx2l = dsp0[src_stride8 + 1];
						dx2 =
						    vis_faligndata(dx2h, dx2l);
						BILINEAR_S16_2_Y;
						((mlib_f32 *)dp)[0] = fr;
						dsp0++;
						dp += 2;
					}
				}

				return (MLIB_SUCCESS);
			}
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
			emask_lo = ~emask_hi;
			all_z = 8 - (mlib_addr)dp;
			dx2l = dsp0[0];
			dx2 = dsp0[1];
			dx1 = vis_faligndata(dx2l, dx2);
			dx2l = dsp0[2];
			dx2 = vis_faligndata(dx2, dx2l);
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx1l = vis_ld_d64_nf(dpstr2);
			dx4 = vis_ld_d64_nf(dpstr2 + 1);
			dx3 = vis_faligndata(dx1l, dx4);
			dx1l = vis_ld_d64_nf(dpstr2 + 2);
			dx4 = vis_faligndata(dx4, dx1l);

			j = 0;
			if (dst_width >= 2) {
				BILINEAR_S16_2_Y_2;
				dx4 = vis_ld_d64_nf(dpstr2 + 3);
				dx3 = vis_faligndata(dx1l, dx4);
				dx1l = vis_ld_d64_nf(dpstr2 + 4);
				dx4 = vis_faligndata(dx4, dx1l);
				vis_alignaddr(sp0, 0);
				dx2 = dsp0[3];
				dx1 = vis_faligndata(dx2l, dx2);
				dx2l = dsp0[4];
				dx2 = vis_faligndata(dx2, dx2l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				if (emask_hi)
					vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
				j = 2;
			}
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				BILINEAR_S16_2_Y_2;
				dx4 = vis_ld_d64_nf(dpstr2 + 3);
				dx3 = vis_faligndata(dx1l, dx4);
				dx1l = vis_ld_d64_nf(dpstr2 + 4);
				dx4 = vis_faligndata(dx4, dx1l);
				vis_alignaddr(sp0, 0);
				dx2 = dsp0[3];
				dx1 = vis_faligndata(dx2l, dx2);
				dx2l = dsp0[4];
				dx2 = vis_faligndata(dx2, dx2l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
			}
		}

		if (j < dst_width) {
			for (i = 0; i < dst_height; i++) {
#pragma pipeloop(0)
				p_dst[i * dst_stride + 2 * j] =
				    (p_src[2 * i * src_stride + 4 * j] +
				    p_src[(2 * i + 1) * src_stride +
				    4 * j]) >> 1;
				p_dst[i * dst_stride + 2 * j + 1] =
				    (p_src[2 * i * src_stride + 4 * j + 1] +
				    p_src[(2 * i + 1) * src_stride + 4 * j +
				    1]) >> 1;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */

	if ((((mlib_s32)p_dst | (2 * dst_stride)) & 3) == 0) {
		if ((((mlib_s32)p_src | (2 * src_stride)) & 7) == 0) {
			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 =
					    ((mlib_d64 *)(sp0 + src_stride))[0];
					BILINEAR_S16_2_XY;
					((mlib_f32 *)dp)[0] = fr;
					sp0 += 4;
					dp += 2;
				}
			}

			return (MLIB_SUCCESS);
		}

		if ((src_stride & 3) == 0) {
			mlib_s32 src_stride8 = src_stride >> 2;

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1l = dsp0[0];
				dx2l = dsp0[src_stride8];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1h = dx1l;
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2h = dx2l;
					dx2l = dsp0[src_stride8 + 1];
					dx2 = vis_faligndata(dx2h, dx2l);
					BILINEAR_S16_2_XY;
					((mlib_f32 *)dp)[0] = fr;
					dsp0++;
					dp += 2;
				}
			}

			return (MLIB_SUCCESS);
		}
	}

/* common case */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
		emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
		emask_lo = ~emask_hi;
		all_z = 8 - (mlib_addr)dp;
		dx2l = dsp0[0];
		dx2 = dsp0[1];
		dx1 = vis_faligndata(dx2l, dx2);
		dx2l = dsp0[2];
		dx2 = vis_faligndata(dx2, dx2l);
		dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
		dx1l = dpstr2[0];
		dx4 = vis_ld_d64_nf(dpstr2 + 1);
		dx3 = vis_faligndata(dx1l, dx4);
		dx1l = vis_ld_d64_nf(dpstr2 + 2);
		dx4 = vis_faligndata(dx4, dx1l);
		j = 0;

		if (dst_width >= 2) {
			BILINEAR_S16_2_XY_2;
			dx4 = vis_ld_d64_nf(dpstr2 + 3);
			dx3 = vis_faligndata(dx1l, dx4);
			dx1l = vis_ld_d64_nf(dpstr2 + 4);
			dx4 = vis_faligndata(dx4, dx1l);
			vis_alignaddr(sp0, 0);
			dx2 = dsp0[3];
			dx1 = vis_faligndata(dx2l, dx2);
			dx2l = dsp0[4];
			dx2 = vis_faligndata(dx2, dx2l);
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			if (emask_hi)
				vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
			j = 2;
		}
#pragma pipeloop(0)
		for (; j <= (dst_width - 2); j += 2) {
			BILINEAR_S16_2_XY_2;
			dx4 = vis_ld_d64_nf(dpstr2 + 3);
			dx3 = vis_faligndata(dx1l, dx4);
			dx1l = vis_ld_d64_nf(dpstr2 + 4);
			dx4 = vis_faligndata(dx4, dx1l);
			vis_alignaddr(sp0, 0);
			dx2 = dsp0[3];
			dx1 = vis_faligndata(dx2l, dx2);
			dx2l = dsp0[4];
			dx2 = vis_faligndata(dx2, dx2l);
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
		}
	}

	if (j < dst_width) {
		for (i = 0; i < dst_height; i++) {
			p_dst[i * dst_stride + 2 * j] =
			    (p_src[2 * i * src_stride + 4 * j] +
			    p_src[2 * i * src_stride + 4 * j + 2] +
			    p_src[(2 * i + 1) * src_stride + 4 * j] +
			    p_src[(2 * i + 1) * src_stride + 4 * j + 2]) >> 2;
			p_dst[i * dst_stride + 2 * j + 1] =
			    (p_src[2 * i * src_stride + 4 * j + 1] +
			    p_src[2 * i * src_stride + 4 * j + 3] +
			    p_src[(2 * i + 1) * src_stride + 4 * j + 1] +
			    p_src[(2 * i + 1) * src_stride + 4 * j + 3]) >> 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S16_3                                              \
	dr = vis_faligndata(dr, dr);                            \
	(void *)vis_alignaddr(dsp0, 2);                         \
	dr2 = vis_faligndata(dr1, dr1);                         \
	dr = vis_faligndata(dr, dr2)

/* *********************************************************** */

#define	BILINEAR_S16_3_X                                        \
	dr1 = vis_fand(dx2, dmask_05);                          \
	dr1 = vis_fmul8x16(scale05, dr1);                       \
	dr2 = vis_faligndata(dx1, dx2);                         \
	dr2 = vis_fmul8x16(scale05, dr2);                       \
	dr1 = vis_fpadd16(dr1, dr2);                            \
	(void *)vis_alignaddr(dsp0, 6);                         \
	dr = vis_fand(dx0, dmask_05);                           \
	dr = vis_fmul8x16(scale05, dr);                         \
	dr2 = vis_faligndata(dx0, dx1);                         \
	dr2 = vis_fmul8x16(scale05, dr2);                       \
	dr = vis_fpadd16(dr, dr2);                              \
	PACK_S16_3

/* *********************************************************** */

#define	BILINEAR_S16_3_Y                                        \
	dr1 = vis_faligndata(dx1, dx2);                         \
	dr1 = vis_fand(dr1, dmask_05);                          \
	dr1 = vis_fmul8x16(scale05, dr1);                       \
	dr2 = vis_faligndata(dx4, dx5);                         \
	dr2 = vis_fmul8x16(scale05, dr2);                       \
	dr1 = vis_fpadd16(dr1, dr2);                            \
	(void *)vis_alignaddr(dsp0, 6);                         \
	dr = vis_fand(dx0, dmask_05);                           \
	dr = vis_fmul8x16(scale05, dr);                         \
	dr2 = vis_fmul8x16(scale05, dx3);                       \
	dr = vis_fpadd16(dr, dr2);                              \
	PACK_S16_3

/* *********************************************************** */

#define	BILINEAR_S16_3_XY                                       \
	dr1 = vis_fand(dx2, dmask_025);                         \
	dr1 = vis_fmul8x16(scale025, dr1);                      \
	dr2 = vis_faligndata(dx1, dx2);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr1 = vis_fpadd16(dr1, dr2);                            \
	dr4 = vis_fmul8x16(scale025, dx5);                      \
	dr2 = vis_faligndata(dx4, dx5);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr4 = vis_fpadd16(dr4, dr2);                            \
	dr1 = vis_fpadd16(dr1, dr4);                            \
	(void *)vis_alignaddr(dsp0, 6);                         \
	dr = vis_fand(dx0, dmask_025);                          \
	dr = vis_fmul8x16(scale025, dr);                        \
	dr2 = vis_faligndata(dx0, dx1);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr = vis_fpadd16(dr, dr2);                              \
	dr3 = vis_fmul8x16(scale025, dx3);                      \
	dr2 = vis_faligndata(dx3, dx4);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr3 = vis_fpadd16(dr3, dr2);                            \
	dr = vis_fpadd16(dr, dr3);                              \
	PACK_S16_3

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_S16_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;
	mlib_s16 *sp0, *dp;
	mlib_d64 dx0, dx1, dx2, dx33, dx3, dx4, dx5, dx66;
	mlib_d64 *dsp0, *dpstr2, dr, dr1, dr2, dr3, dr4;
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe);
	mlib_d64 dmask_025 = vis_to_double_dup(0xfffcfffc);
	mlib_f32 scale025 = vis_to_float(0x40404040), scale05 =
	    vis_to_float(0x80808080);

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			j = ((mlib_s32)dp >> 1) & 1;

			if (j > 0 && dst_width > 0) {
				dp[0] = (sp0[0] + sp0[3]) >> 1;
				dp[1] = (sp0[1] + sp0[4]) >> 1;
				dp[2] = (sp0[2] + sp0[5]) >> 1;
				sp0 += 6;
				dp += 3;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx0 = vis_ld_d64_nf(dsp0);
			dx1 = vis_ld_d64_nf(dsp0 + 1);
			dx2 = vis_ld_d64_nf(dsp0 + 2);

			if (((mlib_s32)sp0 & 7) == 0) {
				(void *)vis_alignaddr(dsp0, 2);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					BILINEAR_S16_3_X;
					dx0 = ((mlib_d64 *)sp0)[3];
					dx1 = ((mlib_d64 *)sp0)[4];
					dx2 = ((mlib_d64 *)sp0)[5];
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					dp += 6;
					sp0 += 12;
				}
			} else {
				dx3 = vis_ld_d64_nf(dsp0 + 3);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					dx0 = vis_faligndata(dx0, dx1);
					dx1 = vis_faligndata(dx1, dx2);
					dx2 = vis_faligndata(dx2, dx3);
					(void *)vis_alignaddr(dsp0, 2);
					BILINEAR_S16_3_X;
					dx0 = dx3;
					dx1 = dsp0[4];
					dx2 = dsp0[5];
					dx3 = dsp0[6];
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					vis_alignaddr(sp0, 0);
					dp += 6;
					sp0 += 12;
					dsp0 += 3;
				}
			}

			if (j < dst_width) {
				dp[0] = (sp0[0] + sp0[3]) >> 1;
				dp[1] = (sp0[1] + sp0[4]) >> 1;
				dp[2] = (sp0[2] + sp0[5]) >> 1;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			j = ((mlib_s32)dp >> 1) & 1;

			if (j > 0 && dst_width > 0) {
				dp[0] = (sp0[0] + sp0[src_stride]) >> 1;
				dp[1] = (sp0[1] + sp0[src_stride + 1]) >> 1;
				dp[2] = (sp0[2] + sp0[src_stride + 2]) >> 1;
				sp0 += 6;
				dp += 3;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx0 = vis_ld_d64_nf(dsp0);
			dx1 = vis_ld_d64_nf(dsp0 + 1);
			dx2 = vis_ld_d64_nf(dsp0 + 2);
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx3 = vis_ld_d64_nf(dpstr2);
			dx4 = vis_ld_d64_nf(dpstr2 + 1);
			dx5 = vis_ld_d64_nf(dpstr2 + 2);

			if ((((mlib_s32)sp0 | (2 * src_stride)) & 7) == 0) {
				(void *)vis_alignaddr(dsp0, 2);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					BILINEAR_S16_3_Y;
					dx0 = ((mlib_d64 *)sp0)[3];
					dx1 = ((mlib_d64 *)sp0)[4];
					dx2 = ((mlib_d64 *)sp0)[5];
					dx3 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 3);
					dx4 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 4);
					dx5 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 5);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					dp += 6;
					sp0 += 12;
				}
			} else {
				dx33 = vis_ld_d64_nf(dsp0 + 3);
				dx66 = vis_ld_d64_nf(dpstr2 + 3);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					dx3 = vis_faligndata(dx3, dx4);
					dx4 = vis_faligndata(dx4, dx5);
					dx5 = vis_faligndata(dx5, dx66);
					vis_alignaddr(sp0, 0);
					dx0 = vis_faligndata(dx0, dx1);
					dx1 = vis_faligndata(dx1, dx2);
					dx2 = vis_faligndata(dx2, dx33);
					(void *)vis_alignaddr(dsp0, 2);
					BILINEAR_S16_3_Y;
					dx0 = dx33;
					dx1 = dsp0[4];
					dx2 = dsp0[5];
					dx33 = dsp0[6];
					dx3 = dx66;
					dx4 = vis_ld_d64_nf(dpstr2 + 4);
					dx5 = vis_ld_d64_nf(dpstr2 + 5);
					dx66 = vis_ld_d64_nf(dpstr2 + 6);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					vis_alignaddr(sp0, 2 * src_stride);
					dp += 6;
					sp0 += 12;
					dsp0 += 3;
					dpstr2 += 3;
				}
			}

			if (j < dst_width) {
				dp[0] = (sp0[0] + sp0[src_stride]) >> 1;
				dp[1] = (sp0[1] + sp0[src_stride + 1]) >> 1;
				dp[2] = (sp0[2] + sp0[src_stride + 2]) >> 1;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		j = ((mlib_s32)dp >> 1) & 1;

		if (j > 0 && dst_width > 0) {
			dp[0] =
			    (sp0[0] + sp0[3] + sp0[src_stride] +
			    sp0[src_stride + 3]) >> 2;
			dp[1] =
			    (sp0[1] + sp0[4] + sp0[src_stride + 1] +
			    sp0[src_stride + 4]) >> 2;
			dp[2] =
			    (sp0[2] + sp0[5] + sp0[src_stride + 2] +
			    sp0[src_stride + 5]) >> 2;
			sp0 += 6;
			dp += 3;
		}

		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);

		if ((((mlib_s32)sp0 | (2 * src_stride)) & 7) == 0) {
			(void *)vis_alignaddr(dsp0, 2);
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				dx0 = ((mlib_d64 *)sp0)[0];
				dx1 = ((mlib_d64 *)sp0)[1];
				dx2 = ((mlib_d64 *)sp0)[2];
				dx3 = ((mlib_d64 *)(sp0 + src_stride))[0];
				dx4 = ((mlib_d64 *)(sp0 + src_stride))[1];
				dx5 = ((mlib_d64 *)(sp0 + src_stride))[2];
				BILINEAR_S16_3_XY;
				((mlib_f32 *)dp)[0] = vis_read_hi(dr);
				((mlib_f32 *)dp)[1] = vis_read_lo(dr);
				((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
				dp += 6;
				sp0 += 12;
			}
		} else {
			dx33 = dsp0[0];
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx66 = vis_ld_d64_nf(dpstr2);
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				dx0 = dx33;
				dx1 = dsp0[1];
				dx2 = dsp0[2];
				dx33 = dsp0[3];
				dx3 = dx66;
				dx4 = vis_ld_d64_nf(dpstr2 + 1);
				dx5 = vis_ld_d64_nf(dpstr2 + 2);
				dx66 = vis_ld_d64_nf(dpstr2 + 3);
				dx3 = vis_faligndata(dx3, dx4);
				dx4 = vis_faligndata(dx4, dx5);
				dx5 = vis_faligndata(dx5, dx66);
				vis_alignaddr(sp0, 0);
				dx0 = vis_faligndata(dx0, dx1);
				dx1 = vis_faligndata(dx1, dx2);
				dx2 = vis_faligndata(dx2, dx33);
				(void *)vis_alignaddr(dsp0, 2);
				BILINEAR_S16_3_XY;
				((mlib_f32 *)dp)[0] = vis_read_hi(dr);
				((mlib_f32 *)dp)[1] = vis_read_lo(dr);
				((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
				vis_alignaddr(sp0, 2 * src_stride);
				dp += 6;
				sp0 += 12;
				dsp0 += 3;
				dpstr2 += 3;
			}
		}

		if (j < dst_width) {
			dp[0] =
			    (sp0[0] + sp0[3] + sp0[src_stride] +
			    sp0[src_stride + 3]) >> 2;
			dp[1] =
			    (sp0[1] + sp0[4] + sp0[src_stride + 1] +
			    sp0[src_stride + 4]) >> 2;
			dp[2] =
			    (sp0[2] + sp0[5] + sp0[src_stride + 2] +
			    sp0[src_stride + 5]) >> 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BILINEAR_S16_4                                          \
	dx1 = vis_fand(dx1, dmask_05);                          \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	dx2 = vis_fmul8x16(scale05, dx2);                       \
	dr = vis_fpadd16(dx1, dx2)

/* *********************************************************** */

#define	BILINEAR_S16_4_XY                                       \
	dx1 = vis_fand(dx1, dmask_025);                         \
	dx1 = vis_fmul8x16(scale025, dx1);                      \
	dx2 = vis_fmul8x16(scale025, dx2);                      \
	dx1 = vis_fpadd16(dx1, dx2);                            \
	dx3 = vis_fmul8x16(scale025, dx3);                      \
	dx4 = vis_fmul8x16(scale025, dx4);                      \
	dx3 = vis_fpadd16(dx3, dx4);                            \
	dr = vis_fpadd16(dx1, dx3)

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_S16_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16))
{
	mlib_s32 i, j;

	mlib_s16 *sp0, *dp;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0, *dpstr2, *dpd;
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe);
	mlib_d64 dmask_025 = vis_to_double_dup(0xfffcfffc);
	mlib_f32 scale025 = vis_to_float(0x40404040),
		scale05 = vis_to_float(0x80808080);
	mlib_s32 emask_hi, emask_lo, all_z;

/* averaging along X axis */
	if (dh) {
		if ((((mlib_s32)p_dst | (dst_stride * 2)) & 7) == 0) {
			if ((((mlib_s32)p_src | (src_stride * 2)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = ((mlib_d64 *)sp0)[1];
						BILINEAR_S16_4;
						((mlib_d64 *)dp)[0] = dr;
						sp0 += 8;
						dp += 4;
					}
				}

				return (MLIB_SUCCESS);
			}

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx3 = vis_ld_d64_nf(dsp0);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx2 = dsp0[1];
					dx1 = vis_faligndata(dx3, dx2);
					dx3 = dsp0[2];
					dx2 = vis_faligndata(dx2, dx3);
					BILINEAR_S16_4;
					((mlib_d64 *)dp)[0] = dr;
					dsp0 += 2;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
			emask_lo = ~emask_hi;
			all_z = 8 - (mlib_addr)dp;
			dx1 = vis_ld_d64_nf(dsp0);
			dx2 = vis_ld_d64_nf(dsp0 + 1);
			dx3 = vis_ld_d64_nf(dsp0 + 2);
			dx1 = vis_faligndata(dx1, dx2);
			dx2 = vis_faligndata(dx2, dx3);

			if (dst_width > 0) {
				BILINEAR_S16_4;
				dx2 = vis_ld_d64_nf(dsp0 + 3);
				dx1 = vis_faligndata(dx3, dx2);
				dx3 = vis_ld_d64_nf(dsp0 + 4);
				dx2 = vis_faligndata(dx2, dx3);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				if (emask_hi)
					vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				(void *)vis_alignaddr(sp0, 0);
			}
#pragma pipeloop(0)
			for (j = 1; j < dst_width; j++) {
				BILINEAR_S16_4;
				dx2 = vis_ld_d64_nf(dsp0 + 3);
				dx1 = vis_faligndata(dx3, dx2);
				dx3 = vis_ld_d64_nf(dsp0 + 4);
				dx2 = vis_faligndata(dx2, dx3);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				(void *)vis_alignaddr(sp0, 0);
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		if ((((mlib_s32)p_dst | (dst_stride * 2)) & 7) == 0) {
			if ((((mlib_s32)p_src | (src_stride * 2)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = vis_ld_d64_nf(
							(mlib_d64 *)(sp0 +
							src_stride));
						BILINEAR_S16_4;
						((mlib_d64 *)dp)[0] = dr;
						sp0 += 8;
						dp += 4;
					}
				}

				return (MLIB_SUCCESS);
			}

			if ((src_stride & 3) == 0) {
				mlib_s32 src_stride8 = src_stride >> 2;

				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
					dsp0 =
					    (mlib_d64 *)vis_alignaddr(sp0, 0);
					dx1h = dsp0[0];
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8);
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8 + 1);
					dx2 = vis_faligndata(dx2h, dx2l);
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						BILINEAR_S16_4;
						dx1h = dsp0[2];
						dx1l = dsp0[3];
						dx1 =
						    vis_faligndata(dx1h, dx1l);
						dx2h = vis_ld_d64_nf(dsp0 +
							src_stride8 + 2);
						dx2l = vis_ld_d64_nf(dsp0 +
							src_stride8 + 3);
						dx2 =
						    vis_faligndata(dx2h, dx2l);
						((mlib_d64 *)dp)[0] = dr;
						dsp0 += 2;
						dp += 4;
					}
				}

				return (MLIB_SUCCESS);
			}
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
			emask_lo = ~emask_hi;
			all_z = 8 - (mlib_addr)dp;
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx2h = vis_ld_d64_nf(dpstr2);
			dx2l = vis_ld_d64_nf(dpstr2 + 1);
			dx2 = vis_faligndata(dx2h, dx2l);
			vis_alignaddr(sp0, 0);
			dx1h = dsp0[0];
			dx1l = dsp0[1];
			dx1 = vis_faligndata(dx1h, dx1l);
			vis_alignaddr(sp0, 2 * src_stride);

			if (dst_width > 0) {
				BILINEAR_S16_4;
				dx2h = vis_ld_d64_nf(dpstr2 + 2);
				dx2l = vis_ld_d64_nf(dpstr2 + 3);
				dx2 = vis_faligndata(dx2h, dx2l);
				vis_alignaddr(sp0, 0);
				dx1h = dsp0[2];
				dx1l = dsp0[3];
				dx1 = vis_faligndata(dx1h, dx1l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				if (emask_hi)
					vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
			}
#pragma pipeloop(0)
			for (j = 1; j < dst_width; j++) {
				BILINEAR_S16_4;
				dx2h = vis_ld_d64_nf(dpstr2 + 2);
				dx2l = vis_ld_d64_nf(dpstr2 + 3);
				dx2 = vis_faligndata(dx2h, dx2l);
				vis_alignaddr(sp0, 0);
				dx1h = dsp0[2];
				dx1l = dsp0[3];
				dx1 = vis_faligndata(dx1h, dx1l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */

	if ((((mlib_s32)p_dst | (dst_stride * 2)) & 7) == 0) {
		if ((((mlib_s32)p_src | (src_stride * 2)) & 7) == 0) {
			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 = ((mlib_d64 *)sp0)[1];
					dx3 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride));
					dx4 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 1);
					BILINEAR_S16_4_XY;
					((mlib_d64 *)dp)[0] = dr;
					sp0 += 8;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}

		if ((src_stride & 3) == 0) {
			mlib_s32 src_stride8 = src_stride >> 2;

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = dsp0[0];
				dx2h = vis_ld_d64_nf(dsp0 + src_stride8);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx1h = dsp0[2];
					dx2 = vis_faligndata(dx1l, dx1h);
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8 + 1);
					dx3 = vis_faligndata(dx2h, dx2l);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8 + 2);
					dx4 = vis_faligndata(dx2l, dx2h);
					BILINEAR_S16_4_XY;
					((mlib_d64 *)dp)[0] = dr;
					dsp0 += 2;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}
	}

/* common case */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
		emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
		emask_lo = ~emask_hi;
		all_z = 8 - (mlib_addr)dp;
		dx1h = dsp0[0];
		dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
		dx2h = vis_ld_d64_nf(dpstr2);

		if (dst_width > 0) {
			dx2l = vis_ld_d64_nf(dpstr2 + 1);
			dx3 = vis_faligndata(dx2h, dx2l);
			dx2h = vis_ld_d64_nf(dpstr2 + 2);
			dx4 = vis_faligndata(dx2l, dx2h);
			vis_alignaddr(sp0, 0);
			dx1l = dsp0[1];
			dx1 = vis_faligndata(dx1h, dx1l);
			dx1h = dsp0[2];
			dx2 = vis_faligndata(dx1l, dx1h);
			BILINEAR_S16_4_XY;
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			if (emask_hi)
				vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
		}
#pragma pipeloop(0)
		for (j = 1; j < dst_width; j++) {
			dx2l = vis_ld_d64_nf(dpstr2 + 1);
			dx3 = vis_faligndata(dx2h, dx2l);
			dx2h = vis_ld_d64_nf(dpstr2 + 2);
			dx4 = vis_faligndata(dx2l, dx2h);
			vis_alignaddr(sp0, 0);
			dx1l = dsp0[1];
			dx1 = vis_faligndata(dx1h, dx1l);
			dx1h = dsp0[2];
			dx2 = vis_faligndata(dx1l, dx1h);
			BILINEAR_S16_4_XY;
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BILINEAR_U16_1_X__                                              \
	dr = vis_fmuld8ulx16(fone, vis_read_hi(dx1));                   \
	dr1 = vis_fmuld8ulx16(fone, vis_read_lo(dx1));                  \
	dx1 =                                                           \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr), vis_read_lo(dr)),   \
	    vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1)));          \
	dr = vis_fmuld8ulx16(fone, vis_read_hi(dx2));                   \
	dr1 = vis_fmuld8ulx16(fone, vis_read_lo(dx2));                  \
	dx2 =                                                           \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr), vis_read_lo(dr)),   \
	    vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1)));          \
	dr = vis_fpackfix_pair(dx1, dx2);                               \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

#define	BILINEAR_U16_1_X                                        \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	BILINEAR_U16_1_X__

/* *********************************************************** */

#define	BILINEAR_U16_ADD_1_Y                                    \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	dx3 = vis_fxor(dx3, mask8000);                          \
	dx4 = vis_fxor(dx4, mask8000);                          \
	dx1 = vis_fand(dx1, dmask_even);                        \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	dx3 = vis_fmul8x16(scale05, dx3);                       \
	dx1 = vis_fpadd16(dx1, dx3);                            \
	dx2 = vis_fand(dx2, dmask_even);                        \
	dx2 = vis_fmul8x16(scale05, dx2);                       \
	dx4 = vis_fmul8x16(scale05, dx4);                       \
	dx2 = vis_fpadd16(dx2, dx4)

/* *********************************************************** */

#define	BILINEAR_U16_1_Y                                         \
	BILINEAR_U16_ADD_1_Y;                                    \
	dr = vis_fpmerge(vis_read_hi(dx1), vis_read_hi(dx2));    \
	dr1 = vis_fpmerge(vis_read_lo(dx1), vis_read_lo(dx2));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));     \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

#define	BILINEAR_U16_1_XY	BILINEAR_U16_ADD_1_Y; BILINEAR_U16_1_X__

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U16_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16))
{
	mlib_s32 i, j;
	mlib_u16 *sp0, *dp, *pdend;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr, dr1;
	mlib_d64 *dsp0, *dpstr2, *dpd;
	mlib_d64 dmask_even = vis_to_double_dup(0xfffefffe);
	mlib_f32 scale05 = vis_to_float(0x80808080);
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_s32 beg, off, emask;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* averaging along X axis */
	if (dh) {
		if (dst_width <= 0)
			return (MLIB_SUCCESS);
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			pdend = dp + dst_width - 1;
			dpd = (mlib_d64 *)((mlib_addr)dp & (~7));
			off = 2 * ((mlib_s32)dpd - (mlib_s32)dp);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, off);
			dx1h = vis_ld_d64_nf(dsp0);
			dx1l = vis_ld_d64_nf(dsp0 + 1);
			dx2h = vis_ld_d64_nf(dsp0 + 2);
			dx1 = vis_faligndata(dx1h, dx1l);
			dx2 = vis_faligndata(dx1l, dx2h);
			BILINEAR_U16_1_X;
			emask = vis_edge16(dp, pdend);
/* store first bytes of result */
			vis_pst_16(dr, dpd, emask);
			dsp0 += 2;
			dpd++;
			j = 4 + (off >> 2);

			if ((((mlib_s32)sp0 + off) & 7) == 0) {
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1 = dsp0[0];
					dx2 = dsp0[1];
					BILINEAR_U16_1_X;
					(*dpd++) = dr;
					dsp0 += 2;
				}

				dx2h = vis_ld_d64_nf(dsp0);
			} else {
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1h = dx2h;
					dx1l = dsp0[1];
					dx2h = dsp0[2];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2 = vis_faligndata(dx1l, dx2h);
					BILINEAR_U16_1_X;
					(*dpd++) = dr;
					dsp0 += 2;
				}
			}

			if (j < dst_width) {
				dx1h = dx2h;
				dx1l = dsp0[1];
				dx2h = dsp0[2];
				dx1 = vis_faligndata(dx1h, dx1l);
				dx2 = vis_faligndata(dx1l, dx2h);
				BILINEAR_U16_1_X;
				emask = vis_edge16(dpd, pdend);
/* store last bytes of result */
				vis_pst_16(dr, dpd, emask);
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			beg = ((8 - (mlib_addr)dp) & 7) >> 1;

			if (beg > dst_width)
				beg = dst_width;
			for (j = 0; j < beg; j++) {
				(*dp++) = (sp0[0] + sp0[src_stride]) >> 1;
				sp0 += 2;
			}

			if ((src_stride & 7) == 0) {
/* aligned case */
				if (((mlib_addr)sp0 & 7) == 0) {
#pragma pipeloop(0)
					for (; j <= (dst_width - 4); j += 4) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = ((mlib_d64 *)sp0)[1];
						dx3 = vis_ld_d64_nf(
							(mlib_d64 *)(sp0 +
							src_stride));
						dx4 = vis_ld_d64_nf(
							(mlib_d64 *)(sp0 +
							src_stride) + 1);
						BILINEAR_U16_1_Y;
						((mlib_d64 *)dp)[0] = dr;
						sp0 += 8;
						dp += 4;
					}
				} else {
					mlib_s32 src_stride8 = src_stride >> 2;

					dsp0 =
					    (mlib_d64 *)vis_alignaddr(sp0, 0);
					dx1h = vis_ld_d64_nf(dsp0);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8);
#pragma pipeloop(0)
					for (; j <= (dst_width - 4); j += 4) {
						dx1l = dsp0[1];
						dx1 =
						    vis_faligndata(dx1h, dx1l);
						dx1h = dsp0[2];
						dx2 =
						    vis_faligndata(dx1l, dx1h);
						dx2l = vis_ld_d64_nf(dsp0 +
							src_stride8 + 1);
						dx3 =
						    vis_faligndata(dx2h, dx2l);
						dx2h = vis_ld_d64_nf(dsp0 +
							src_stride8 + 2);
						dx4 =
						    vis_faligndata(dx2l, dx2h);
						BILINEAR_U16_1_Y;
						dx4 =
						    vis_faligndata(dx2l, dx2h);
						((mlib_d64 *)dp)[0] = dr;
						dsp0 += 2;
						sp0 += 8;
						dp += 4;
					}
				}
			} else {
				mlib_s32 src_stride2 = src_stride * 2;

				dpstr2 =
				    (mlib_d64 *)vis_alignaddr(sp0, src_stride2);
				dx2h = vis_ld_d64_nf(dpstr2);
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = vis_ld_d64_nf(dsp0);
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx1h = dsp0[2];
					dx2 = vis_faligndata(dx1l, dx1h);
					vis_alignaddr(sp0, src_stride2);
					dx2l = vis_ld_d64_nf(dpstr2 + 1);
					dx3 = vis_faligndata(dx2h, dx2l);
					dx2h = vis_ld_d64_nf(dpstr2 + 2);
					dx4 = vis_faligndata(dx2l, dx2h);
					vis_alignaddr(sp0, 0);
					BILINEAR_U16_1_Y;
					((mlib_d64 *)dp)[0] = dr;
					vis_alignaddr(sp0, 0);
					dsp0 += 2;
					dpstr2 += 2;
					sp0 += 8;
					dp += 4;
				}
			}

			for (; j < dst_width; j++) {
				(*dp++) = (sp0[0] + sp0[src_stride]) >> 1;
				sp0 += 2;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		beg = ((8 - (mlib_addr)dp) & 7) >> 1;

		if (beg > dst_width)
			beg = dst_width;
		for (j = 0; j < beg; j++) {
			(*dp++) = (sp0[0] + sp0[src_stride] +
			    sp0[1] + sp0[src_stride + 1]) >> 2;
			sp0 += 2;
		}

		if ((src_stride & 7) == 0) {

			if (((mlib_addr)sp0 & 7) == 0) { /* aligned case */
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 = ((mlib_d64 *)sp0)[1];
					dx3 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride));
					dx4 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 1);
					BILINEAR_U16_1_XY;
					((mlib_d64 *)dp)[0] = dr;
					sp0 += 8;
					dp += 4;
				}
			} else {
				mlib_s32 src_stride8 = src_stride >> 2;

				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = vis_ld_d64_nf(dsp0);
				dx2h = vis_ld_d64_nf(dsp0 + src_stride8);
#pragma pipeloop(0)
				for (; j <= (dst_width - 4); j += 4) {
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx1h = dsp0[2];
					dx2 = vis_faligndata(dx1l, dx1h);
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8 + 1);
					dx3 = vis_faligndata(dx2h, dx2l);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8 + 2);
					dx4 = vis_faligndata(dx2l, dx2h);
					BILINEAR_U16_1_XY;
					dx4 = vis_faligndata(dx2l, dx2h);
					((mlib_d64 *)dp)[0] = dr;
					dsp0 += 2;
					sp0 += 8;
					dp += 4;
				}
			}
		} else {
			mlib_s32 src_stride2 = src_stride * 2;

			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, src_stride2);
			dx2h = vis_ld_d64_nf(dpstr2);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx1h = vis_ld_d64_nf(dsp0);
#pragma pipeloop(0)
			for (; j <= (dst_width - 4); j += 4) {
				dx1l = dsp0[1];
				dx1 = vis_faligndata(dx1h, dx1l);
				dx1h = dsp0[2];
				dx2 = vis_faligndata(dx1l, dx1h);
				vis_alignaddr(sp0, src_stride2);
				dx2l = vis_ld_d64_nf(dpstr2 + 1);
				dx3 = vis_faligndata(dx2h, dx2l);
				dx2h = vis_ld_d64_nf(dpstr2 + 2);
				dx4 = vis_faligndata(dx2l, dx2h);
				vis_alignaddr(sp0, 0);
				BILINEAR_U16_1_XY;
				((mlib_d64 *)dp)[0] = dr;
				vis_alignaddr(sp0, 0);
				dsp0 += 2;
				dpstr2 += 2;
				sp0 += 8;
				dp += 4;
			}
		}

		for (; j < dst_width; j++) {
			(*dp++) = (sp0[0] + sp0[src_stride]
			    + sp0[1] + sp0[src_stride + 1]) >> 2;
			sp0 += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BILINEAR_U16_2_X                                         \
	dx1 = vis_fxor(dx1, mask8000);                           \
	dx1 = vis_fand(dx1, dmask_05);                           \
	dx1 = vis_fmul8x16(scale05, dx1);                        \
	fr = vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1));   \
	fr = vis_fxors(fr, vis_read_hi(mask8000))

/* *********************************************************** */

#define	BILINEAR_U16_2_X_2                                                \
	dx1 = vis_fxor(dx1, mask8000);                                    \
	dx2 = vis_fxor(dx2, mask8000);                                    \
	dx1 = vis_fand(dx1, dmask_05);                                    \
	dx2 = vis_fand(dx2, dmask_05);                                    \
	dx1 = vis_fmul8x16(scale05, dx1);                                 \
	dx2 = vis_fmul8x16(scale05, dx2);                                 \
	dr =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1)),   \
	    vis_fpadd16s(vis_read_hi(dx2), vis_read_lo(dx2)));            \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

#define	BILINEAR_U16_2_XY                                        \
	dx1 = vis_fxor(dx1, mask8000);                           \
	dx2 = vis_fxor(dx2, mask8000);                           \
	dx1 = vis_fand(dx1, dmask_025);                          \
	dx1 = vis_fmul8x16(scale025, dx1);                       \
	dx2 = vis_fmul8x16(scale025, dx2);                       \
	dx1 = vis_fpadd16(dx1, dx2);                             \
	fr = vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1));   \
	fr = vis_fxors(fr, vis_read_hi(mask8000))

/* *********************************************************** */

#define	BILINEAR_U16_2_XY_2                                               \
	dx1 = vis_fxor(dx1, mask8000);                                    \
	dx2 = vis_fxor(dx2, mask8000);                                    \
	dx3 = vis_fxor(dx3, mask8000);                                    \
	dx4 = vis_fxor(dx4, mask8000);                                    \
	dx1 = vis_fand(dx1, dmask_025);                                   \
	dx3 = vis_fand(dx3, dmask_025);                                   \
	dx1 = vis_fmul8x16(scale025, dx1);                                \
	dx2 = vis_fmul8x16(scale025, dx2);                                \
	dx3 = vis_fmul8x16(scale025, dx3);                                \
	dx4 = vis_fmul8x16(scale025, dx4);                                \
	dx1 = vis_fpadd16(dx1, dx3);                                      \
	dx2 = vis_fpadd16(dx2, dx4);                                      \
	dr =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1), vis_read_lo(dx1)),   \
	    vis_fpadd16s(vis_read_hi(dx2), vis_read_lo(dx2)));            \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

#define	BILINEAR_U16_2_Y                                         \
	dx1 = vis_fxor(dx1, mask8000);                           \
	dx2 = vis_fxor(dx2, mask8000);                           \
	dx1 = vis_fand(dx1, dmask_05);                           \
	dx1 = vis_fmul8x16(scale05, dx1);                        \
	dx2 = vis_fmul8x16(scale05, dx2);                        \
	fr = vis_fpadd16s(vis_read_hi(dx1), vis_read_hi(dx2));   \
	fr = vis_fxors(fr, vis_read_hi(mask8000))

/* *********************************************************** */

#define	BILINEAR_U16_2_Y_2                                                \
	dx1 = vis_fxor(dx1, mask8000);                                    \
	dx2 = vis_fxor(dx2, mask8000);                                    \
	dx3 = vis_fxor(dx3, mask8000);                                    \
	dx4 = vis_fxor(dx4, mask8000);                                    \
	dx1 = vis_fand(dx1, dmask_05);                                    \
	dx1 = vis_fmul8x16(scale05, dx1);                                 \
	dx3 = vis_fmul8x16(scale05, dx3);                                 \
	dx2 = vis_fand(dx2, dmask_05);                                    \
	dx2 = vis_fmul8x16(scale05, dx2);                                 \
	dx4 = vis_fmul8x16(scale05, dx4);                                 \
	dr =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(dx1), vis_read_hi(dx3)),   \
	    vis_fpadd16s(vis_read_hi(dx2), vis_read_hi(dx4)));            \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U16_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16))
{
	mlib_s32 i, j;
	mlib_u16 *sp0, *dp;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0, *dpstr2, *dpd;
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe);
	mlib_d64 dmask_025 = vis_to_double_dup(0xfffcfffc);
	mlib_f32 scale025 = vis_to_float(0x40404040), scale05 =
	    vis_to_float(0x80808080), fr;
	mlib_s32 emask_hi, emask_lo, all_z;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* averaging along X axis */
	if (dh) {
		if ((((mlib_s32)p_dst | (2 * dst_stride)) & 3) == 0) {
			if ((((mlib_s32)p_src | (2 * src_stride)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						BILINEAR_U16_2_X;
						((mlib_f32 *)dp)[0] = fr;
						sp0 += 4;
						dp += 2;
					}
				}

				return (MLIB_SUCCESS);
			}
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;

			if (((mlib_s32)dp & 3) == 0) {
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1l = dsp0[0];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1h = dx1l;
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					BILINEAR_U16_2_X;
					((mlib_f32 *)dp)[0] = fr;
					dsp0++;
					dp += 2;
				}
			} else {
				dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				emask_hi =
				    0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
				emask_lo = ~emask_hi;
				all_z = 8 - (mlib_addr)dp;
				dx2l = vis_ld_d64_nf(dsp0);
				dx2 = vis_ld_d64_nf(dsp0 + 1);
				dx1 = vis_faligndata(dx2l, dx2);
				dx2l = vis_ld_d64_nf(dsp0 + 2);
				dx2 = vis_faligndata(dx2, dx2l);
#pragma pipeloop(0)
				for (j = 0; j <= (dst_width - 2); j += 2) {
					BILINEAR_U16_2_X_2;
					dx2 = dsp0[3];
					dx1 = vis_faligndata(dx2l, dx2);
					dx2l = dsp0[4];
					dx2 = vis_faligndata(dx2, dx2l);
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr, dr);
					vis_pst_8(dr, dpd, emask_hi);
					dpd++;
					vis_pst_8(dr, dpd, emask_lo);
					dsp0 += 2;
					vis_alignaddr(sp0, 0);
				}

				if (j < dst_width) {
					dp[2 * j] =
					    (sp0[4 * j] + sp0[4 * j + 2]) >> 1;
					dp[2 * j + 1] =
					    (sp0[4 * j + 1] + sp0[4 * j +
					    3]) >> 1;
				}
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		if ((((mlib_s32)p_dst | (2 * dst_stride)) & 3) == 0) {
			if ((((mlib_s32)p_src | (2 * src_stride)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = vis_ld_d64_nf(
							(mlib_d64 *)(sp0 +
						    src_stride));
						BILINEAR_U16_2_Y;
						((mlib_f32 *)dp)[0] = fr;
						sp0 += 4;
						dp += 2;
					}
				}

				return (MLIB_SUCCESS);
			}

			if ((src_stride & 3) == 0) {
				mlib_s32 src_stride8 = src_stride >> 2;

				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
					dsp0 =
					    (mlib_d64 *)vis_alignaddr(sp0, 0);
					dx1l = dsp0[0];
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8);
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1h = dx1l;
						dx1l = dsp0[1];
						dx1 =
						    vis_faligndata(dx1h, dx1l);
						dx2h = dx2l;
						dx2l = vis_ld_d64_nf(dsp0 +
							src_stride8 + 1);
						dx2 =
						    vis_faligndata(dx2h, dx2l);
						BILINEAR_U16_2_Y;
						((mlib_f32 *)dp)[0] = fr;
						dsp0++;
						dp += 2;
					}
				}

				return (MLIB_SUCCESS);
			}
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
			emask_lo = ~emask_hi;
			all_z = 8 - (mlib_addr)dp;
			dx2l = dsp0[0];
			dx2 = dsp0[1];
			dx1 = vis_faligndata(dx2l, dx2);
			dx2l = dsp0[2];
			dx2 = vis_faligndata(dx2, dx2l);
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx1l = vis_ld_d64_nf(dpstr2);
			dx4 = vis_ld_d64_nf(dpstr2 + 1);
			dx3 = vis_faligndata(dx1l, dx4);
			dx1l = vis_ld_d64_nf(dpstr2 + 2);
			dx4 = vis_faligndata(dx4, dx1l);
			j = 0;

			if (dst_width >= 2) {
				BILINEAR_U16_2_Y_2;
				dx4 = vis_ld_d64_nf(dpstr2 + 3);
				dx3 = vis_faligndata(dx1l, dx4);
				dx1l = vis_ld_d64_nf(dpstr2 + 4);
				dx4 = vis_faligndata(dx4, dx1l);
				vis_alignaddr(sp0, 0);
				dx2 = dsp0[3];
				dx1 = vis_faligndata(dx2l, dx2);
				dx2l = dsp0[4];
				dx2 = vis_faligndata(dx2, dx2l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				if (emask_hi)
					vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
				j = 2;
			}
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				BILINEAR_U16_2_Y_2;
				dx4 = vis_ld_d64_nf(dpstr2 + 3);
				dx3 = vis_faligndata(dx1l, dx4);
				dx1l = vis_ld_d64_nf(dpstr2 + 4);
				dx4 = vis_faligndata(dx4, dx1l);
				vis_alignaddr(sp0, 0);
				dx2 = dsp0[3];
				dx1 = vis_faligndata(dx2l, dx2);
				dx2l = dsp0[4];
				dx2 = vis_faligndata(dx2, dx2l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
			}
		}

		if (j < dst_width) {
			for (i = 0; i < dst_height; i++) {
#pragma pipeloop(0)
				p_dst[i * dst_stride + 2 * j] =
				    (p_src[2 * i * src_stride + 4 * j] +
				    p_src[(2 * i + 1) * src_stride +
				    4 * j]) >> 1;
				p_dst[i * dst_stride + 2 * j + 1] =
				    (p_src[2 * i * src_stride + 4 * j + 1] +
				    p_src[(2 * i + 1) * src_stride + 4 * j +
				    1]) >> 1;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */

	if ((((mlib_s32)p_dst | (2 * dst_stride)) & 3) == 0) {
		if ((((mlib_s32)p_src | (2 * src_stride)) & 7) == 0) {
			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 = vis_ld_d64_nf(
						(mlib_d64 *)(sp0 + src_stride));
					BILINEAR_U16_2_XY;
					((mlib_f32 *)dp)[0] = fr;
					sp0 += 4;
					dp += 2;
				}
			}

			return (MLIB_SUCCESS);
		}

		if ((src_stride & 3) == 0) {
			mlib_s32 src_stride8 = src_stride >> 2;

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1l = dsp0[0];
				dx2l = vis_ld_d64_nf(dsp0 + src_stride8);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1h = dx1l;
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2h = dx2l;
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8 + 1);
					dx2 = vis_faligndata(dx2h, dx2l);
					BILINEAR_U16_2_XY;
					((mlib_f32 *)dp)[0] = fr;
					dsp0++;
					dp += 2;
				}
			}

			return (MLIB_SUCCESS);
		}
	}

/* common case */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
		emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
		emask_lo = ~emask_hi;
		all_z = 8 - (mlib_addr)dp;
		dx2l = dsp0[0];
		dx2 = dsp0[1];
		dx1 = vis_faligndata(dx2l, dx2);
		dx2l = dsp0[2];
		dx2 = vis_faligndata(dx2, dx2l);
		dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
		dx1l = vis_ld_d64_nf(dpstr2);
		dx4 = vis_ld_d64_nf(dpstr2 + 1);
		dx3 = vis_faligndata(dx1l, dx4);
		dx1l = vis_ld_d64_nf(dpstr2 + 2);
		dx4 = vis_faligndata(dx4, dx1l);
		j = 0;

		if (dst_width >= 2) {
			BILINEAR_U16_2_XY_2;
			dx4 = vis_ld_d64_nf(dpstr2 + 3);
			dx3 = vis_faligndata(dx1l, dx4);
			dx1l = vis_ld_d64_nf(dpstr2 + 4);
			dx4 = vis_faligndata(dx4, dx1l);
			vis_alignaddr(sp0, 0);
			dx2 = dsp0[3];
			dx1 = vis_faligndata(dx2l, dx2);
			dx2l = dsp0[4];
			dx2 = vis_faligndata(dx2, dx2l);
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			if (emask_hi)
				vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
			j = 2;
		}
#pragma pipeloop(0)
		for (; j <= (dst_width - 2); j += 2) {
			BILINEAR_U16_2_XY_2;
			dx4 = vis_ld_d64_nf(dpstr2 + 3);
			dx3 = vis_faligndata(dx1l, dx4);
			dx1l = vis_ld_d64_nf(dpstr2 + 4);
			dx4 = vis_faligndata(dx4, dx1l);
			vis_alignaddr(sp0, 0);
			dx2 = dsp0[3];
			dx1 = vis_faligndata(dx2l, dx2);
			dx2l = dsp0[4];
			dx2 = vis_faligndata(dx2, dx2l);
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
		}
	}

	if (j < dst_width) {
		for (i = 0; i < dst_height; i++) {
			p_dst[i * dst_stride + 2 * j] =
			    (p_src[2 * i * src_stride + 4 * j] +
			    p_src[2 * i * src_stride + 4 * j + 2] +
			    p_src[(2 * i + 1) * src_stride + 4 * j] +
			    p_src[(2 * i + 1) * src_stride + 4 * j + 2]) >> 2;
			p_dst[i * dst_stride + 2 * j + 1] =
			    (p_src[2 * i * src_stride + 4 * j + 1] +
			    p_src[2 * i * src_stride + 4 * j + 3] +
			    p_src[(2 * i + 1) * src_stride + 4 * j + 1] +
			    p_src[(2 * i + 1) * src_stride + 4 * j + 3]) >> 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_U16_3                                              \
	dr = vis_faligndata(dr, dr);                            \
	(void *)vis_alignaddr(dsp0, 2);                         \
	dr2 = vis_faligndata(dr1, dr1);                         \
	dr = vis_faligndata(dr, dr2);                           \
	dr = vis_fxor(dr, mask8000);                            \
	dr1 = vis_fxor(dr1, mask8000)

/* *********************************************************** */

#define	BILINEAR_U16_3_X                                        \
	dx0 = vis_fxor(dx0, mask8000);                          \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	dr1 = vis_fand(dx2, dmask_05);                          \
	dr1 = vis_fmul8x16(scale05, dr1);                       \
	dr2 = vis_faligndata(dx1, dx2);                         \
	dr2 = vis_fmul8x16(scale05, dr2);                       \
	dr1 = vis_fpadd16(dr1, dr2);                            \
	(void *)vis_alignaddr(dsp0, 6);                         \
	dr = vis_fand(dx0, dmask_05);                           \
	dr = vis_fmul8x16(scale05, dr);                         \
	dr2 = vis_faligndata(dx0, dx1);                         \
	dr2 = vis_fmul8x16(scale05, dr2);                       \
	dr = vis_fpadd16(dr, dr2);                              \
	PACK_U16_3

/* *********************************************************** */

#define	BILINEAR_U16_3_Y                                        \
	dx0 = vis_fxor(dx0, mask8000);                          \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	dx3 = vis_fxor(dx3, mask8000);                          \
	dx4 = vis_fxor(dx4, mask8000);                          \
	dx5 = vis_fxor(dx5, mask8000);                          \
	dr1 = vis_faligndata(dx1, dx2);                         \
	dr1 = vis_fand(dr1, dmask_05);                          \
	dr1 = vis_fmul8x16(scale05, dr1);                       \
	dr2 = vis_faligndata(dx4, dx5);                         \
	dr2 = vis_fmul8x16(scale05, dr2);                       \
	dr1 = vis_fpadd16(dr1, dr2);                            \
	(void *)vis_alignaddr(dsp0, 6);                         \
	dr = vis_fand(dx0, dmask_05);                           \
	dr = vis_fmul8x16(scale05, dr);                         \
	dr2 = vis_fmul8x16(scale05, dx3);                       \
	dr = vis_fpadd16(dr, dr2);                              \
	PACK_U16_3

/* *********************************************************** */

#define	BILINEAR_U16_3_XY                                       \
	dx0 = vis_fxor(dx0, mask8000);                          \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	dx3 = vis_fxor(dx3, mask8000);                          \
	dx4 = vis_fxor(dx4, mask8000);                          \
	dx5 = vis_fxor(dx5, mask8000);                          \
	dr1 = vis_fand(dx2, dmask_025);                         \
	dr1 = vis_fmul8x16(scale025, dr1);                      \
	dr2 = vis_faligndata(dx1, dx2);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr1 = vis_fpadd16(dr1, dr2);                            \
	dr4 = vis_fmul8x16(scale025, dx5);                      \
	dr2 = vis_faligndata(dx4, dx5);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr4 = vis_fpadd16(dr4, dr2);                            \
	dr1 = vis_fpadd16(dr1, dr4);                            \
	(void *)vis_alignaddr(dsp0, 6);                         \
	dr = vis_fand(dx0, dmask_025);                          \
	dr = vis_fmul8x16(scale025, dr);                        \
	dr2 = vis_faligndata(dx0, dx1);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr = vis_fpadd16(dr, dr2);                              \
	dr3 = vis_fmul8x16(scale025, dx3);                      \
	dr2 = vis_faligndata(dx3, dx4);                         \
	dr2 = vis_fmul8x16(scale025, dr2);                      \
	dr3 = vis_fpadd16(dr3, dr2);                            \
	dr = vis_fpadd16(dr, dr3);                              \
	PACK_U16_3

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U16_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16))
{
	mlib_s32 i, j;

	mlib_u16 *sp0, *dp;
	mlib_d64 dx0, dx1, dx2, dx33, dx3, dx4, dx5, dx66;
	mlib_d64 *dsp0, *dpstr2, dr, dr1, dr2, dr3, dr4;
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe);
	mlib_d64 dmask_025 = vis_to_double_dup(0xfffcfffc);
	mlib_f32 scale025 = vis_to_float(0x40404040),
		scale05 = vis_to_float(0x80808080);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			j = ((mlib_s32)dp >> 1) & 1;

			if (j > 0 && dst_width > 0) {
				dp[0] = (sp0[0] + sp0[3]) >> 1;
				dp[1] = (sp0[1] + sp0[4]) >> 1;
				dp[2] = (sp0[2] + sp0[5]) >> 1;
				sp0 += 6;
				dp += 3;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx0 = vis_ld_d64_nf(dsp0);
			dx1 = vis_ld_d64_nf(dsp0 + 1);
			dx2 = vis_ld_d64_nf(dsp0 + 2);

			if (((mlib_s32)sp0 & 7) == 0) {
				(void *)vis_alignaddr(dsp0, 2);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					BILINEAR_U16_3_X;
					dx0 = ((mlib_d64 *)sp0)[3];
					dx1 = ((mlib_d64 *)sp0)[4];
					dx2 = ((mlib_d64 *)sp0)[5];
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					dp += 6;
					sp0 += 12;
				}
			} else {
				dx3 = vis_ld_d64_nf(dsp0 + 3);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					dx0 = vis_faligndata(dx0, dx1);
					dx1 = vis_faligndata(dx1, dx2);
					dx2 = vis_faligndata(dx2, dx3);
					(void *)vis_alignaddr(dsp0, 2);
					BILINEAR_U16_3_X;
					dx0 = dx3;
					dx1 = dsp0[4];
					dx2 = dsp0[5];
					dx3 = dsp0[6];
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					vis_alignaddr(sp0, 0);
					dp += 6;
					sp0 += 12;
					dsp0 += 3;
				}
			}

			if (j < dst_width) {
				dp[0] = (sp0[0] + sp0[3]) >> 1;
				dp[1] = (sp0[1] + sp0[4]) >> 1;
				dp[2] = (sp0[2] + sp0[5]) >> 1;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			j = ((mlib_s32)dp >> 1) & 1;

			if (j > 0 && dst_width > 0) {
				dp[0] = (sp0[0] + sp0[src_stride]) >> 1;
				dp[1] = (sp0[1] + sp0[src_stride + 1]) >> 1;
				dp[2] = (sp0[2] + sp0[src_stride + 2]) >> 1;
				sp0 += 6;
				dp += 3;
			}

			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			dx0 = dsp0[0];
			dx1 = dsp0[1];
			dx2 = dsp0[2];
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx3 = vis_ld_d64_nf(dpstr2);
			dx4 = vis_ld_d64_nf(dpstr2 + 1);
			dx5 = vis_ld_d64_nf(dpstr2 + 2);

			if ((((mlib_s32)sp0 | (2 * src_stride)) & 7) == 0) {
				(void *)vis_alignaddr(dsp0, 2);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					BILINEAR_U16_3_Y;
					dx0 = ((mlib_d64 *)sp0)[3];
					dx1 = ((mlib_d64 *)sp0)[4];
					dx2 = ((mlib_d64 *)sp0)[5];
					dx3 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 3);
					dx4 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 4);
					dx5 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
						src_stride) + 5);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					dp += 6;
					sp0 += 12;
				}
			} else {
				dx33 = dsp0[3];
				dx66 = vis_ld_d64_nf(dpstr2 + 3);
#pragma pipeloop(0)
				for (; j <= (dst_width - 2); j += 2) {
					dx3 = vis_faligndata(dx3, dx4);
					dx4 = vis_faligndata(dx4, dx5);
					dx5 = vis_faligndata(dx5, dx66);
					vis_alignaddr(sp0, 0);
					dx0 = vis_faligndata(dx0, dx1);
					dx1 = vis_faligndata(dx1, dx2);
					dx2 = vis_faligndata(dx2, dx33);
					(void *)vis_alignaddr(dsp0, 2);
					BILINEAR_U16_3_Y;
					dx0 = dx33;
					dx1 = dsp0[4];
					dx2 = dsp0[5];
					dx33 = dsp0[6];
					dx3 = dx66;
					dx4 = vis_ld_d64_nf(dpstr2 + 4);
					dx5 = vis_ld_d64_nf(dpstr2 + 5);
					dx66 = vis_ld_d64_nf(dpstr2 + 6);
					((mlib_f32 *)dp)[0] = vis_read_hi(dr);
					((mlib_f32 *)dp)[1] = vis_read_lo(dr);
					((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
					vis_alignaddr(sp0, 2 * src_stride);
					dp += 6;
					sp0 += 12;
					dsp0 += 3;
					dpstr2 += 3;
				}
			}

			if (j < dst_width) {
				dp[0] = (sp0[0] + sp0[src_stride]) >> 1;
				dp[1] = (sp0[1] + sp0[src_stride + 1]) >> 1;
				dp[2] = (sp0[2] + sp0[src_stride + 2]) >> 1;
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		j = ((mlib_s32)dp >> 1) & 1;

		if (j > 0 && dst_width > 0) {
			dp[0] =
			    (sp0[0] + sp0[3] + sp0[src_stride] +
			    sp0[src_stride + 3]) >> 2;
			dp[1] =
			    (sp0[1] + sp0[4] + sp0[src_stride + 1] +
			    sp0[src_stride + 4]) >> 2;
			dp[2] =
			    (sp0[2] + sp0[5] + sp0[src_stride + 2] +
			    sp0[src_stride + 5]) >> 2;
			sp0 += 6;
			dp += 3;
		}

		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);

		if ((((mlib_s32)sp0 | (2 * src_stride)) & 7) == 0) {
			(void *)vis_alignaddr(dsp0, 2);
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				dx0 = ((mlib_d64 *)sp0)[0];
				dx1 = ((mlib_d64 *)sp0)[1];
				dx2 = ((mlib_d64 *)sp0)[2];
				dx3 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
					src_stride));
				dx4 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
					src_stride) + 1);
				dx5 = vis_ld_d64_nf((mlib_d64 *)(sp0 +
					src_stride) + 2);
				BILINEAR_U16_3_XY;
				((mlib_f32 *)dp)[0] = vis_read_hi(dr);
				((mlib_f32 *)dp)[1] = vis_read_lo(dr);
				((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
				dp += 6;
				sp0 += 12;
			}
		} else {
			dx33 = dsp0[0];
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx66 = vis_ld_d64_nf(dpstr2);
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				dx0 = dx33;
				dx1 = dsp0[1];
				dx2 = dsp0[2];
				dx33 = dsp0[3];
				dx3 = dx66;
				dx4 = vis_ld_d64_nf(dpstr2 + 1);
				dx5 = vis_ld_d64_nf(dpstr2 + 2);
				dx66 = vis_ld_d64_nf(dpstr2 + 3);
				dx3 = vis_faligndata(dx3, dx4);
				dx4 = vis_faligndata(dx4, dx5);
				dx5 = vis_faligndata(dx5, dx66);
				vis_alignaddr(sp0, 0);
				dx0 = vis_faligndata(dx0, dx1);
				dx1 = vis_faligndata(dx1, dx2);
				dx2 = vis_faligndata(dx2, dx33);
				(void *)vis_alignaddr(dsp0, 2);
				BILINEAR_U16_3_XY;
				((mlib_f32 *)dp)[0] = vis_read_hi(dr);
				((mlib_f32 *)dp)[1] = vis_read_lo(dr);
				((mlib_f32 *)dp)[2] = vis_read_lo(dr1);
				vis_alignaddr(sp0, 2 * src_stride);
				dp += 6;
				sp0 += 12;
				dsp0 += 3;
				dpstr2 += 3;
			}
		}

		if (j < dst_width) {
			dp[0] =
			    (sp0[0] + sp0[3] + sp0[src_stride] +
			    sp0[src_stride + 3]) >> 2;
			dp[1] =
			    (sp0[1] + sp0[4] + sp0[src_stride + 1] +
			    sp0[src_stride + 4]) >> 2;
			dp[2] =
			    (sp0[2] + sp0[5] + sp0[src_stride + 2] +
			    sp0[src_stride + 5]) >> 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BILINEAR_U16_4                                          \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	dx1 = vis_fand(dx1, dmask_05);                          \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	dx2 = vis_fmul8x16(scale05, dx2);                       \
	dr = vis_fpadd16(dx1, dx2);                             \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

#define	BILINEAR_U16_4_XY                                       \
	dx1 = vis_fxor(dx1, mask8000);                          \
	dx2 = vis_fxor(dx2, mask8000);                          \
	dx3 = vis_fxor(dx3, mask8000);                          \
	dx4 = vis_fxor(dx4, mask8000);                          \
	dx1 = vis_fand(dx1, dmask_025);                         \
	dx1 = vis_fmul8x16(scale025, dx1);                      \
	dx2 = vis_fmul8x16(scale025, dx2);                      \
	dx1 = vis_fpadd16(dx1, dx2);                            \
	dx3 = vis_fmul8x16(scale025, dx3);                      \
	dx4 = vis_fmul8x16(scale025, dx4);                      \
	dx3 = vis_fpadd16(dx3, dx4);                            \
	dr = vis_fpadd16(dx1, dx3);                             \
	dr = vis_fxor(dr, mask8000)

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_U16_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16))
{
	mlib_s32 i, j;
	mlib_u16 *sp0, *dp;
	mlib_d64 dx1, dx2, dx3, dx4, dx1h, dx1l, dx2h, dx2l, dr;
	mlib_d64 *dsp0, *dpstr2, *dpd;
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe);
	mlib_d64 dmask_025 = vis_to_double_dup(0xfffcfffc);
	mlib_f32 scale025 = vis_to_float(0x40404040),
		scale05 = vis_to_float(0x80808080);
	mlib_s32 emask_hi, emask_lo, all_z;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* averaging along X axis */
	if (dh) {
		if ((((mlib_s32)p_dst | (dst_stride * 2)) & 7) == 0) {
			if ((((mlib_s32)p_src | (src_stride * 2)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = ((mlib_d64 *)sp0)[1];
						BILINEAR_U16_4;
						((mlib_d64 *)dp)[0] = dr;
						sp0 += 8;
						dp += 4;
					}
				}

				return (MLIB_SUCCESS);
			}

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx3 = dsp0[0];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx2 = dsp0[1];
					dx1 = vis_faligndata(dx3, dx2);
					dx3 = dsp0[2];
					dx2 = vis_faligndata(dx2, dx3);
					BILINEAR_U16_4;
					((mlib_d64 *)dp)[0] = dr;
					dsp0 += 2;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
			emask_lo = ~emask_hi;
			all_z = 8 - (mlib_addr)dp;
			dx1 = dsp0[0];
			dx2 = dsp0[1];
			dx3 = dsp0[2];
			dx1 = vis_faligndata(dx1, dx2);
			dx2 = vis_faligndata(dx2, dx3);

			if (dst_width > 0) {
				BILINEAR_U16_4;
				dx2 = dsp0[3];
				dx1 = vis_faligndata(dx3, dx2);
				dx3 = dsp0[4];
				dx2 = vis_faligndata(dx2, dx3);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				if (emask_hi)
					vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				(void *)vis_alignaddr(sp0, 0);
			}
#pragma pipeloop(0)
			for (j = 1; j < dst_width; j++) {
				BILINEAR_U16_4;
				dx2 = dsp0[3];
				dx1 = vis_faligndata(dx3, dx2);
				dx3 = dsp0[4];
				dx2 = vis_faligndata(dx2, dx3);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				(void *)vis_alignaddr(sp0, 0);
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		if ((((mlib_s32)p_dst | (dst_stride * 2)) & 7) == 0) {
			if ((((mlib_s32)p_src | (src_stride * 2)) & 7) == 0) {
				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						dx1 = ((mlib_d64 *)sp0)[0];
						dx2 = vis_ld_d64_nf(
						    (mlib_d64 *)(sp0 +
						    src_stride));
						BILINEAR_U16_4;
						((mlib_d64 *)dp)[0] = dr;
						sp0 += 8;
						dp += 4;
					}
				}

				return (MLIB_SUCCESS);
			}

			if ((src_stride & 3) == 0) {
				mlib_s32 src_stride8 = src_stride >> 2;

				for (i = 0; i < dst_height; i++) {
					sp0 = p_src + 2 * i * src_stride;
					dp = p_dst + i * dst_stride;
					dsp0 =
					    (mlib_d64 *)vis_alignaddr(sp0, 0);
					dx1h = dsp0[0];
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8);
					dx2l = vis_ld_d64_nf(dsp0 +
							src_stride8 + 1);
					dx2 = vis_faligndata(dx2h, dx2l);
#pragma pipeloop(0)
					for (j = 0; j < dst_width; j++) {
						BILINEAR_U16_4;
						dx1h = dsp0[2];
						dx1l = dsp0[3];
						dx1 =
						    vis_faligndata(dx1h, dx1l);
						dx2h = vis_ld_d64_nf(dsp0 +
							src_stride8 + 2);
						dx2l = vis_ld_d64_nf(dsp0 +
							src_stride8 + 3);
						dx2 =
						    vis_faligndata(dx2h, dx2l);
						((mlib_d64 *)dp)[0] = dr;
						dsp0 += 2;
						dp += 4;
					}
				}

				return (MLIB_SUCCESS);
			}
		}

/* common case */
		for (i = 0; i < dst_height; i++) {
			sp0 = p_src + 2 * i * src_stride;
			dp = p_dst + i * dst_stride;
			dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
			dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
			emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
			emask_lo = ~emask_hi;
			all_z = 8 - (mlib_addr)dp;
			dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
			dx2h = vis_ld_d64_nf(dpstr2);
			dx2l = vis_ld_d64_nf(dpstr2 + 1);
			dx2 = vis_faligndata(dx2h, dx2l);
			vis_alignaddr(sp0, 0);
			dx1h = dsp0[0];
			dx1l = dsp0[1];
			dx1 = vis_faligndata(dx1h, dx1l);
			vis_alignaddr(sp0, 2 * src_stride);

			if (dst_width > 0) {
				BILINEAR_U16_4;
				dx2h = vis_ld_d64_nf(dpstr2 + 2);
				dx2l = vis_ld_d64_nf(dpstr2 + 3);
				dx2 = vis_faligndata(dx2h, dx2l);
				vis_alignaddr(sp0, 0);
				dx1h = dsp0[2];
				dx1l = dsp0[3];
				dx1 = vis_faligndata(dx1h, dx1l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				if (emask_hi)
					vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
			}
#pragma pipeloop(0)
			for (j = 1; j < dst_width; j++) {
				BILINEAR_U16_4;
				dx2h = vis_ld_d64_nf(dpstr2 + 2);
				dx2l = vis_ld_d64_nf(dpstr2 + 3);
				dx2 = vis_faligndata(dx2h, dx2l);
				vis_alignaddr(sp0, 0);
				dx1h = dsp0[2];
				dx1l = dsp0[3];
				dx1 = vis_faligndata(dx1h, dx1l);
				(void *)vis_alignaddr((void *)all_z, 0);
				dr = vis_faligndata(dr, dr);
				vis_pst_8(dr, dpd, emask_hi);
				dpd++;
				vis_pst_8(dr, dpd, emask_lo);
				dsp0 += 2;
				dpstr2 += 2;
				vis_alignaddr(sp0, 2 * src_stride);
			}
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */

	if ((((mlib_s32)p_dst | (dst_stride * 2)) & 7) == 0) {
		if ((((mlib_s32)p_src | (src_stride * 2)) & 7) == 0) {
			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = ((mlib_d64 *)sp0)[0];
					dx2 = ((mlib_d64 *)sp0)[1];
					dx3 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride));
					dx4 = vis_ld_d64_nf(
					    (mlib_d64 *)(sp0 + src_stride) + 1);
					BILINEAR_U16_4_XY;
					((mlib_d64 *)dp)[0] = dr;
					sp0 += 8;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}

		if ((src_stride & 3) == 0) {
			mlib_s32 src_stride8 = src_stride >> 2;

			for (i = 0; i < dst_height; i++) {
				sp0 = p_src + 2 * i * src_stride;
				dp = p_dst + i * dst_stride;
				dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
				dx1h = dsp0[0];
				dx2h = vis_ld_d64_nf(dsp0 + src_stride8);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1l = dsp0[1];
					dx1 = vis_faligndata(dx1h, dx1l);
					dx1h = dsp0[2];
					dx2 = vis_faligndata(dx1l, dx1h);
					dx2l = vis_ld_d64_nf(dsp0 +
						src_stride8 + 1);
					dx3 = vis_faligndata(dx2h, dx2l);
					dx2h = vis_ld_d64_nf(dsp0 +
						src_stride8 + 2);
					dx4 = vis_faligndata(dx2l, dx2h);
					BILINEAR_U16_4_XY;
					((mlib_d64 *)dp)[0] = dr;
					dsp0 += 2;
					dp += 4;
				}
			}

			return (MLIB_SUCCESS);
		}
	}

/* common case */
	for (i = 0; i < dst_height; i++) {
		sp0 = p_src + 2 * i * src_stride;
		dp = p_dst + i * dst_stride;
		dpd = (mlib_d64 *)vis_alignaddr(dp, -1);
		dsp0 = (mlib_d64 *)vis_alignaddr(sp0, 0);
		emask_hi = 0xff >> ((mlib_u8 *)dp - (mlib_u8 *)dpd);
		emask_lo = ~emask_hi;
		all_z = 8 - (mlib_addr)dp;
		dx1h = dsp0[0];
		dpstr2 = (mlib_d64 *)vis_alignaddr(sp0, 2 * src_stride);
		dx2h = vis_ld_d64_nf(dpstr2);

		if (dst_width > 0) {
			dx2l = vis_ld_d64_nf(dpstr2 + 1);
			dx3 = vis_faligndata(dx2h, dx2l);
			dx2h = vis_ld_d64_nf(dpstr2 + 2);
			dx4 = vis_faligndata(dx2l, dx2h);
			vis_alignaddr(sp0, 0);
			dx1l = dsp0[1];
			dx1 = vis_faligndata(dx1h, dx1l);
			dx1h = dsp0[2];
			dx2 = vis_faligndata(dx1l, dx1h);
			BILINEAR_U16_4_XY;
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			if (emask_hi)
				vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
		}
#pragma pipeloop(0)
		for (j = 1; j < dst_width; j++) {
			dx2l = vis_ld_d64_nf(dpstr2 + 1);
			dx3 = vis_faligndata(dx2h, dx2l);
			dx2h = vis_ld_d64_nf(dpstr2 + 2);
			dx4 = vis_faligndata(dx2l, dx2h);
			vis_alignaddr(sp0, 0);
			dx1l = dsp0[1];
			dx1 = vis_faligndata(dx1h, dx1l);
			dx1h = dsp0[2];
			dx2 = vis_faligndata(dx1l, dx1h);
			BILINEAR_U16_4_XY;
			(void *)vis_alignaddr((void *)all_z, 0);
			dr = vis_faligndata(dr, dr);
			vis_pst_8(dr, dpd, emask_hi);
			dpd++;
			vis_pst_8(dr, dpd, emask_lo);
			dsp0 += 2;
			dpstr2 += 2;
			vis_alignaddr(sp0, 2 * src_stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
