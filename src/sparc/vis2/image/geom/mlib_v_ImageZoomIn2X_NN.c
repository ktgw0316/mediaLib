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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageZoomIn2X_NN.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2X -  image 2X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2X(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_filter      filter,
 *                                     mlib_edge        edge)
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

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageZoomIn2X_proto.h>

/* *********************************************************** */

typedef union
{
	mlib_d64 db;
	struct
	{
		mlib_f32 fl0;
		mlib_f32 fl1;
	} two_float;
} type_union_mlib_d64;

#define	SIZE_OF_CACH	16384

/* *********************************************************** */

#define	SHAG_STR(type)                                          \
	shag_row = SIZE_OF_CACH / sizeof (type) / src_stride;   \
	if (shag_row < 4)                                       \
	    shag_row = 4

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                        \
	MLIB_TYPE  *pdst,                                       \
	MLIB_TYPE  *psrc,                                       \
	mlib_s32 src_width,                                     \
	mlib_s32 src_height,                                    \
	mlib_s32 dst_stride,                                    \
	mlib_s32 src_stride,                                    \
	mlib_s32 src_w_beg,                                     \
	mlib_s32 src_h_beg,                                     \
	mlib_s32 channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE)                                \
	(MLIB_TYPE *) pdst, (MLIB_TYPE *) psrc, src_width, src_height,   \
	dst_stride, src_stride, src_w_beg, src_h_beg, channels

/* *********************************************************** */

static void mlib_v_ImageZoomIn2X_U8_1NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));
static void mlib_v_ImageZoomIn2X_U8_2NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));
static void mlib_v_ImageZoomIn2X_U8_3NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));
static void mlib_v_ImageZoomIn2X_U8_4NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));
static void mlib_v_ImageZoomIn2X_S16_1NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));
static void mlib_v_ImageZoomIn2X_S16_2NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));
static void mlib_v_ImageZoomIn2X_S16_3NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));
static void mlib_v_ImageZoomIn2X_S16_4NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));

/* *********************************************************** */

#define	ZOOM_IN2X_U8_1_NEAREST                                   \
	dr_hi = vis_fpmerge(vis_read_hi(dx), vis_read_hi(dx));   \
	dr_lo = vis_fpmerge(vis_read_lo(dx), vis_read_lo(dx))

/* *********************************************************** */

#define	ZOOM_IN2X_U8_2_NEAREST                                  \
	dr = vis_fpmerge(vis_read_hi(dx), vis_read_hi(dx));     \
	dr1 = vis_fpmerge(vis_read_lo(dx), vis_read_lo(dx));    \
	dr_hi = vis_bshuffle(dr, dr);                           \
	dr_lo = vis_bshuffle(dr1, dr1)

/* *********************************************************** */

#define	ZOOM_IN2X_U8_3_NEAREST                                  \
	vis_write_bmask(0x01201234, 0);                         \
	dr = vis_bshuffle(dx, dx);                              \
	vis_write_bmask(0x53456786, 0);                         \
	dr1 = vis_bshuffle(dx, dx1);                            \
	vis_write_bmask(0x789AB9AB, 0);                         \
	dr2 = vis_bshuffle(dx, dx1);                            \
	vis_write_bmask(0x45645678, 0);                         \
	dr3 = vis_bshuffle(dx1, dx2);                           \
	vis_write_bmask(0x9789ABCA, 0);                         \
	dr4 = vis_bshuffle(dx1, dx2);                           \
	vis_write_bmask(0xBCDEFDEF, 0);                         \
	dr5 = vis_bshuffle(dx1, dx2)

/* *********************************************************** */

#define	ZOOM_IN2X_U8_4_NEAREST                                     \
	dr_hi = vis_freg_pair(vis_read_hi(dx), vis_read_hi(dx));   \
	dr_lo = vis_freg_pair(vis_read_lo(dx), vis_read_lo(dx))

/* *********************************************************** */

#define	ZOOM_IN2X_S16_1_NEAREST	ZOOM_IN2X_U8_2_NEAREST

/* *********************************************************** */

#define	ZOOM_IN2X_S16_2_NEAREST	ZOOM_IN2X_U8_4_NEAREST

/* *********************************************************** */

#define	ZOOM_IN2X_S16_3_NEAREST                                 \
	(void *)vis_alignaddr(dpx, 4);                          \
	dr6 = vis_faligndata(dx, dx1);                          \
	dr7 = vis_faligndata(dx1, dx2);                         \
	(void *)vis_alignaddr(dpx, 2);                          \
	dr1 = vis_faligndata(dx, dx1);                          \
	dr3 = vis_faligndata(dx1, dx2);                         \
	dr3 = vis_faligndata(dr3, dr7);                         \
	dr5 = vis_faligndata(dx2, dx2);                         \
	(void *)vis_alignaddr(dpx, 6);                          \
	dr4 = vis_faligndata(dx1, dx2);                         \
	dr5 = vis_faligndata(dx2, dr5);                         \
	dr2 = vis_faligndata(dx, dx1);                          \
	dr2 = vis_faligndata(dr6, dr2);                         \
	dr = vis_faligndata(dx, dx);                            \
	(void *)vis_alignaddr(dpx, 2);                          \
	dr = vis_faligndata(dr, dx)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k;

	if ((((mlib_addr)psrc | (mlib_addr)pdst | src_stride | dst_stride |
	    channels) & 1) == 0) {
		dst_stride /= 2;
		src_stride /= 2;
		channels /= 2;
		mlib_v_ImageZoomIn2X_S16_NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s16));
		return;
	}

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			if (j = src_w_beg & 1)
				pdst[k] = psrc[k];
			for (j++; j < src_width; j += 2)
				pdst[j * channels - channels + k] =
				    pdst[j * channels + k] =
				    psrc[(j >> 1) * channels + k];

			if (j == src_width)
				pdst[j * channels - channels + k] =
				    psrc[(j >> 1) * channels + k];
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride + k] =
				    pdst[(2 * i + 1) * dst_stride + k] =
				    psrc[i * src_stride + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride + k] =
				    psrc[i * src_stride + k];
		psrc += channels;
		pdst += channels;
		src_width--;
	}

	if (channels <= 2)
		if (channels == 1)
			mlib_v_ImageZoomIn2X_U8_1NN(MLIB_ZOOMIN2XCALLPARAM
			    (mlib_u8));
		else
			mlib_v_ImageZoomIn2X_U8_2NN(MLIB_ZOOMIN2XCALLPARAM
			    (mlib_u8));
	else if (channels == 3)
		mlib_v_ImageZoomIn2X_U8_3NN(MLIB_ZOOMIN2XCALLPARAM(mlib_u8));

	else
		mlib_v_ImageZoomIn2X_U8_4NN(MLIB_ZOOMIN2XCALLPARAM(mlib_u8));

	i = src_height / 2;
	j = src_width / 2;

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++)
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] =
				    pdst[2 * i * dst_stride + 2 * j * channels +
				    channels + k] =
				    psrc[i * src_stride + j * channels + k];
			}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_1NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
	mlib_s32 i, j; /* indices */

	mlib_u8 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx0, dx1, dr_hi, dr_lo, dr, dr1;

	if ((src_width / 2) < 16) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j] =
				    pdst[2 * i * dst_stride + 2 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 2 * j] =
				    pdst[(2 * i + 1) * dst_stride + 2 * j + 1] =
				    psrc[i * src_stride + j];
			}
		}
	} else {
		if (((dst_stride & 7) | ((mlib_u32)pdst & 1)) == 0) {
			mlib_s32 beg = ((8 - (mlib_u32)pdst) & 7) >> 1;

			for (i = 0; i < (src_height / 2); i++) {
				for (j = 0; j < beg; j++) {
					pdst[2 * i * dst_stride + 2 * j] =
					    pdst[2 * i * dst_stride + 2 * j +
					    1] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j + 1] =
					    psrc[i * src_stride + j];
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride + beg;
				pz_row = pdst + 2 * i * dst_stride + 2 * beg;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				dx = vis_faligndata(dx0, dx1);
				ZOOM_IN2X_U8_1_NEAREST;
#pragma pipeloop(0)
				for (j = beg; j <= src_width / 2 - 8; j += 8) {
					dx0 = dx1;
					dx1 = dpx[2];
					((mlib_d64 *)pz_row)[0] = dr_hi;
					((mlib_d64 *)pz_row)[1] = dr_lo;
					((mlib_d64 *)pz1_row)[0] = dr_hi;
					((mlib_d64 *)pz1_row)[1] = dr_lo;
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_U8_1_NEAREST;
					dpx++;
					pz_row += 16;
					pz1_row += 16;
				}
			}

			beg = j;
			for (i = 0; i < (src_height / 2); i++) {
				for (j = beg; j < (src_width / 2); j++) {
					pdst[2 * i * dst_stride + 2 * j] =
					    pdst[2 * i * dst_stride + 2 * j +
					    1] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j + 1] =
					    psrc[i * src_stride + j];
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				emask_hi =
				    0xff >> ((mlib_u32)pz_row - (mlib_u32)dpz);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xff >> ((mlib_u32)pz1_row -
				    (mlib_u32)dpz1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dx0 = dpx[0];
				dx1 = dpx[1];
#pragma pipeloop(3)
				for (j = 0; j < (src_width / 16); j++) {
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_U8_1_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_8(dr, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz += 2;
					vis_pst_8(dr1, dpz, emask_lo);
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_8(dr, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz1 += 2;
					vis_pst_8(dr1, dpz1, emask1_lo);
					dpx++;
					(void *)vis_alignaddr(px_row, 0);
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				for (j = src_width / 16 * 8;
				    j < (src_width / 2); j++) {
					pdst[2 * i * dst_stride + 2 * j] =
					    pdst[2 * i * dst_stride + 2 * j +
					    1] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j + 1] =
					    psrc[i * src_stride + j];
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_2NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
	mlib_s32 i, j; /* indices */

	mlib_u8 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx0, dx1, dr_hi, dr_lo, dr, dr1;

	vis_write_bmask(0x02134657, 0);

	if ((src_width / 2) < 8) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				mlib_u8 a0 = psrc[i * src_stride + 2 * j],
				    a1 = psrc[i * src_stride + 2 * j + 1];
				pdst[2 * i * dst_stride + 4 * j] =
				    pdst[2 * i * dst_stride + 4 * j + 2] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j + 2] =
				    a0;
				pdst[2 * i * dst_stride + 4 * j + 1] =
				    pdst[2 * i * dst_stride + 4 * j + 3] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j + 3] =
				    a1;
			}
		}
	} else {
		if (((dst_stride & 7) | ((mlib_u32)pdst & 3)) == 0) {
			mlib_s32 beg = ((mlib_u32)pdst & 7) >> 2;

			if (beg) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_s32 arg =
					    (psrc[i *
					    src_stride] << 8) | psrc[i *
					    src_stride + 1];
					arg |= arg << 16;
					((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 2)] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 2)] = arg;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride + 2 * beg;
				pz_row = pdst + 2 * i * dst_stride + 4 * beg;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				for (j = beg; j <= src_width / 2 - 4; j += 4) {
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_U8_2_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					((mlib_d64 *)pz_row)[0] = dr_hi;
					((mlib_d64 *)pz_row)[1] = dr_lo;
					((mlib_d64 *)pz1_row)[0] = dr_hi;
					((mlib_d64 *)pz1_row)[1] = dr_lo;
					dpx++;
					pz_row += 16;
					pz1_row += 16;
				}
			}

			beg = j;
			for (i = 0; i < (src_height / 2); i++) {
				for (j = beg; j < (src_width / 2); j++) {
					mlib_s32 arg =
					    (psrc[i * src_stride +
					    2 * j] << 8) | psrc[i * src_stride +
					    2 * j + 1];
					arg |= arg << 16;
					((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 2) + j] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 2) + j] = arg;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				emask_hi =
				    0xff >> ((mlib_u32)pz_row - (mlib_u32)dpz);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xff >> ((mlib_u32)pz1_row -
				    (mlib_u32)dpz1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dx0 = dpx[0];
				dx1 = dpx[1];
				dx = vis_faligndata(dx0, dx1);
				for (j = 0; j < (src_width / 8); j++) {
					ZOOM_IN2X_U8_2_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					(void *)vis_alignaddr(px_row, 0);
					dx = vis_faligndata(dx0, dx1);
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_8(dr, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz += 2;
					vis_pst_8(dr1, dpz, emask_lo);
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_8(dr, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz1 += 2;
					vis_pst_8(dr1, dpz1, emask1_lo);
					dpx++;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				for (j = src_width / 8 * 4; j < (src_width / 2);
				    j++) {
					mlib_u8 a0 =
					    psrc[i * src_stride + 2 * j], a1 =
					    psrc[i * src_stride + 2 * j + 1];
					pdst[2 * i * dst_stride + 4 * j] =
					    pdst[2 * i * dst_stride + 4 * j +
					    2] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j + 2] = a0;
					pdst[2 * i * dst_stride + 4 * j + 1] =
					    pdst[2 * i * dst_stride + 4 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j + 3] = a1;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_3NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
	mlib_s32 i, j; /* indices */

	mlib_u8 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx1, dx2, dxx0, dxx1, dxx2, dxx3;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6;

	if ((src_width / 2) < 16) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				mlib_u8 a0 = psrc[i * src_stride + 3 * j],
				    a1 = psrc[i * src_stride + 3 * j + 1],
				    a2 = psrc[i * src_stride + 3 * j + 2];
				pdst[2 * i * dst_stride + 6 * j] =
				    pdst[2 * i * dst_stride + 6 * j + 3] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 3] =
				    a0;
				pdst[2 * i * dst_stride + 6 * j + 1] =
				    pdst[2 * i * dst_stride + 6 * j + 4] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 4] =
				    a1;
				pdst[2 * i * dst_stride + 6 * j + 2] =
				    pdst[2 * i * dst_stride + 6 * j + 5] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 2] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 5] =
				    a2;
			}
		}

	} else {
		if (((dst_stride & 7) | ((mlib_u32)pdst & 1)) == 0) {
			mlib_s32 beg = ((mlib_u32)pdst & 7) >> 1;

			for (i = 0; i < (src_height / 2); i++) {
				for (j = 0; j < beg; j++) {
					mlib_u8 a0 =
					    psrc[i * src_stride + 3 * j], a1 =
					    psrc[i * src_stride + 3 * j + 1],
					    a2 =
					    psrc[i * src_stride + 3 * j + 2];
					pdst[2 * i * dst_stride + 6 * j] =
					    pdst[2 * i * dst_stride + 6 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 3] = a0;
					pdst[2 * i * dst_stride + 6 * j + 1] =
					    pdst[2 * i * dst_stride + 6 * j +
					    4] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 4] = a1;
					pdst[2 * i * dst_stride + 6 * j + 2] =
					    pdst[2 * i * dst_stride + 6 * j +
					    5] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 2] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 5] = a2;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride + 3 * beg;
				pz_row = pdst + 2 * i * dst_stride + 6 * beg;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dxx3 = dpx[0];
				for (j = beg; j <= src_width / 2 - 8; j += 8) {
					dxx0 = dxx3;
					dxx1 = dpx[1];
					dxx2 = dpx[2];
					dxx3 = dpx[3];
					dx = vis_faligndata(dxx0, dxx1);
					dx1 = vis_faligndata(dxx1, dxx2);
					dx2 = vis_faligndata(dxx2, dxx3);
					ZOOM_IN2X_U8_3_NEAREST;
					((mlib_d64 *)pz_row)[0] =
					    ((mlib_d64 *)pz1_row)[0] = dr;
					((mlib_d64 *)pz_row)[1] =
					    ((mlib_d64 *)pz1_row)[1] = dr1;
					((mlib_d64 *)pz_row)[2] =
					    ((mlib_d64 *)pz1_row)[2] = dr2;
					((mlib_d64 *)pz_row)[3] =
					    ((mlib_d64 *)pz1_row)[3] = dr3;
					((mlib_d64 *)pz_row)[4] =
					    ((mlib_d64 *)pz1_row)[4] = dr4;
					((mlib_d64 *)pz_row)[5] =
					    ((mlib_d64 *)pz1_row)[5] = dr5;
					dpx += 3;
					pz_row += 48;
					pz1_row += 48;
				}
			}

			beg = (src_width / 2 - beg) / 8 * 8 + beg;
			for (i = 0; i < (src_height / 2); i++) {
				for (j = beg; j < (src_width / 2); j++) {
					mlib_u8 a0 =
					    psrc[i * src_stride + 3 * j], a1 =
					    psrc[i * src_stride + 3 * j + 1],
					    a2 =
					    psrc[i * src_stride + 3 * j + 2];
					pdst[2 * i * dst_stride + 6 * j] =
					    pdst[2 * i * dst_stride + 6 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 3] = a0;
					pdst[2 * i * dst_stride + 6 * j + 1] =
					    pdst[2 * i * dst_stride + 6 * j +
					    4] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 4] = a1;
					pdst[2 * i * dst_stride + 6 * j + 2] =
					    pdst[2 * i * dst_stride + 6 * j +
					    5] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 2] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 5] = a2;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				mlib_d64 *dpx_end;

				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dpx_end = dpx + (src_width / 16) * 3;
				emask_hi =
				    0xff >> ((mlib_u32)pz_row - (mlib_u32)dpz);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xff >> ((mlib_u32)pz1_row -
				    (mlib_u32)dpz1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dxx0 = dpx[0];
				dxx1 = dpx[1];
				dxx2 = dpx[2];
				dxx3 = dpx[3];
				dx = vis_faligndata(dxx0, dxx1);
				while ((mlib_u32)dpx < (mlib_u32)dpx_end) {
/* for (j = 0; j < (src_width/16); j++) { */
/* loop with unroll is worse */
					dxx0 = dxx3;
					dx1 = vis_faligndata(dxx1, dxx2);
					dxx1 = dpx[4];
					dx2 = vis_faligndata(dxx2, dxx3);
					dxx2 = dpx[5];
					dxx3 = dpx[6];
					ZOOM_IN2X_U8_3_NEAREST;
					(void *)vis_alignaddr((void *)all_z, 0);
					dr6 = vis_faligndata(dr, dr);
					vis_pst_8(dr6, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr, dr1);
					dpz[2] = vis_faligndata(dr1, dr2);
					dpz[3] = vis_faligndata(dr2, dr3);
					dpz[4] = vis_faligndata(dr3, dr4);
					dpz[5] = vis_faligndata(dr4, dr5);
					dr6 = vis_faligndata(dr5, dr5);
					dpz += 6;
					vis_pst_8(dr6, dpz, emask_lo);
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					dr6 = vis_faligndata(dr, dr);
					vis_pst_8(dr6, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr, dr1);
					dpz1[2] = vis_faligndata(dr1, dr2);
					dpz1[3] = vis_faligndata(dr2, dr3);
					dpz1[4] = vis_faligndata(dr3, dr4);
					dpz1[5] = vis_faligndata(dr4, dr5);
					dr6 = vis_faligndata(dr5, dr5);
					dpz1 += 6;
					(void *)vis_alignaddr(px_row, 0);
					dx = vis_faligndata(dxx0, dxx1);
					vis_pst_8(dr6, dpz1, emask1_lo);
					dpx += 3;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				for (j = src_width / 16 * 8;
				    j < (src_width / 2); j++) {
					mlib_u8 a0 =
					    psrc[i * src_stride + 3 * j], a1 =
					    psrc[i * src_stride + 3 * j + 1],
					    a2 =
					    psrc[i * src_stride + 3 * j + 2];
					pdst[2 * i * dst_stride + 6 * j] =
					    pdst[2 * i * dst_stride + 6 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 3] = a0;
					pdst[2 * i * dst_stride + 6 * j + 1] =
					    pdst[2 * i * dst_stride + 6 * j +
					    4] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 4] = a1;
					pdst[2 * i * dst_stride + 6 * j + 2] =
					    pdst[2 * i * dst_stride + 6 * j +
					    5] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 2] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 5] = a2;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_4NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
	mlib_s32 i, j; /* indices */

	mlib_u8 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx0, dx1, dr_hi, dr_lo, dr, dr1;

	if ((src_width / 2) < 4) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				mlib_u8 a0 = psrc[i * src_stride + 4 * j],
				    a1 = psrc[i * src_stride + 4 * j + 1],
				    a2 = psrc[i * src_stride + 4 * j + 2],
				    a3 = psrc[i * src_stride + 4 * j + 3];
				pdst[2 * i * dst_stride + 8 * j] =
				    pdst[2 * i * dst_stride + 8 * j + 4] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 4] =
				    a0;
				pdst[2 * i * dst_stride + 8 * j + 1] =
				    pdst[2 * i * dst_stride + 8 * j + 5] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 5] =
				    a1;
				pdst[2 * i * dst_stride + 8 * j + 2] =
				    pdst[2 * i * dst_stride + 8 * j + 6] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 2] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 6] =
				    a2;
				pdst[2 * i * dst_stride + 8 * j + 3] =
				    pdst[2 * i * dst_stride + 8 * j + 7] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 3] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 7] =
				    a3;
			}
		}
	} else {
		if ((((mlib_s32)pdst | dst_stride) & 7) == 0) {
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				for (j = 0; j <= (src_width / 2 - 2); j += 2) {
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_U8_4_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					((mlib_d64 *)pz_row)[0] =
					    ((mlib_d64 *)pz1_row)[0] = dr_hi;
					((mlib_d64 *)pz_row)[1] =
					    ((mlib_d64 *)pz1_row)[1] = dr_lo;
					dpx++;
					pz_row += 16;
					pz1_row += 16;
				}
			}

			if (j < (src_width / 2)) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_s32 a0 =
					    (psrc[i * src_stride +
					    4 * j] << 24) | (psrc[i *
					    src_stride + 4 * j + 1] << 16)
					    | (psrc[i * src_stride + 4 * j +
					    2] << 8) | psrc[i * src_stride +
					    4 * j + 3];
					((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 2) + 2 * j] =
					    ((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 2) + 2 * j + 1] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 2) + 2 * j] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 2) + 2 * j +
					    1] = a0;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				emask_hi =
				    0xff >> ((mlib_u32)pz_row - (mlib_u32)dpz);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xff >> ((mlib_u32)pz1_row -
				    (mlib_u32)dpz1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dx0 = dpx[0];
				dx1 = dpx[1];
				dx = vis_faligndata(dx0, dx1);
				ZOOM_IN2X_U8_4_NEAREST;
				for (j = 0; j <= (src_width / 2 - 2); j += 2) {
					dx0 = dx1;
					dx1 = dpx[2];
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_8(dr, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz += 2;
					vis_pst_8(dr1, dpz, emask_lo);
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_8(dr, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz1 += 2;
					vis_pst_8(dr1, dpz1, emask1_lo);
					dpx++;
					(void *)vis_alignaddr(px_row, 0);
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_U8_4_NEAREST;
				}
			}

			if (j < (src_width / 2)) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_u8 a0 =
					    psrc[i * src_stride + 4 * j], a1 =
					    psrc[i * src_stride + 4 * j + 1],
					    a2 =
					    psrc[i * src_stride + 4 * j + 2],
					    a3 =
					    psrc[i * src_stride + 4 * j + 3];
					pdst[2 * i * dst_stride + 8 * j] =
					    pdst[2 * i * dst_stride + 8 * j +
					    4] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 4] = a0;
					pdst[2 * i * dst_stride + 8 * j + 1] =
					    pdst[2 * i * dst_stride + 8 * j +
					    5] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 5] = a1;
					pdst[2 * i * dst_stride + 8 * j + 2] =
					    pdst[2 * i * dst_stride + 8 * j +
					    6] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 2] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 6] = a2;
					pdst[2 * i * dst_stride + 8 * j + 3] =
					    pdst[2 * i * dst_stride + 8 * j +
					    7] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 3] =
					    pdst[(2 * i + 1) * dst_stride +
					    8 * j + 7] = a3;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_S16_NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
	mlib_s32 i, j, k; /* indices */

	if (src_h_beg & 1) { /* upper bound of image */
		for (k = 0; k < channels; k++) {
			if (j = src_w_beg & 1)
				pdst[k] = psrc[k];
			for (j++; j < src_width; j += 2)
				pdst[j * channels - channels + k] =
				    pdst[j * channels + k] =
				    psrc[(j >> 1) * channels + k];

			if (j == src_width)
				pdst[j * channels - channels + k] =
				    psrc[(j >> 1) * channels + k];
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

	if (src_w_beg & 1) { /* left bound of image */
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride + k] =
				    pdst[(2 * i + 1) * dst_stride + k] =
				    psrc[i * src_stride + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride + k] =
				    psrc[i * src_stride + k];
		psrc += channels;
		pdst += channels;
		src_width--;
	}

	if (channels <= 2)
		if (channels == 1)
			mlib_v_ImageZoomIn2X_S16_1NN(MLIB_ZOOMIN2XCALLPARAM
			    (mlib_s16));
		else
			mlib_v_ImageZoomIn2X_S16_2NN(MLIB_ZOOMIN2XCALLPARAM
			    (mlib_s16));
	else if (channels == 3)
		mlib_v_ImageZoomIn2X_S16_3NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s16));

	else
		mlib_v_ImageZoomIn2X_S16_4NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s16));

	i = src_height / 2;
	j = src_width / 2;

	if (src_height & 1) { /* lower bound of image */
		for (k = 0; k < channels; k++)
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] =
				    pdst[2 * i * dst_stride + 2 * j * channels +
				    channels + k] =
				    psrc[i * src_stride + j * channels + k];
			}
	}

	if (src_width & 1) { /* right bound of image */
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_S16_1NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
	mlib_s32 i, j;

	mlib_s16 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx0, dx1, dr_hi, dr_lo, dr, dr1;

	vis_write_bmask(0x02134657, 0);

	if ((src_width / 2) < 8) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j] =
				    pdst[2 * i * dst_stride + 2 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 2 * j] =
				    pdst[(2 * i + 1) * dst_stride + 2 * j + 1] =
				    psrc[i * src_stride + j];
			}
		}
	} else {
		if (((dst_stride | (mlib_u32)pdst) & 3) == 0) {
			mlib_s32 beg = ((mlib_u32)pdst & 7) >> 2;

			if (beg) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_s32 a0 =
					    (((mlib_u16 *)psrc)[i *
					    src_stride] << 16) | ((mlib_u16 *)
					    psrc)[i * src_stride];
					((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 1)] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 1)] = a0;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride + beg;
				pz_row = pdst + 2 * i * dst_stride + 2 * beg;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				for (j = beg; j <= src_width / 2 - 4; j += 4) {
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_S16_1_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					((mlib_d64 *)pz_row)[0] = dr_hi;
					((mlib_d64 *)pz_row)[1] = dr_lo;
					((mlib_d64 *)pz1_row)[0] = dr_hi;
					((mlib_d64 *)pz1_row)[1] = dr_lo;
					dpx++;
					pz_row += 8;
					pz1_row += 8;
				}
			}

			beg = j;
			for (i = 0; i < (src_height / 2); i++) {
				for (j = beg; j < (src_width / 2); j++) {
					mlib_s32 a0 =
					    (((mlib_u16 *)psrc)[i * src_stride +
					    j] << 16) | ((mlib_u16 *)psrc)[i *
					    src_stride + j];
					((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 1) + j] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 1) + j] = a0;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				emask_hi =
				    0xf >> (((mlib_u32)pz_row -
				    (mlib_u32)dpz) >> 1);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xf >> (((mlib_u32)pz1_row -
				    (mlib_u32)dpz1) >> 1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dx0 = dpx[0];
				dx1 = dpx[1];
				dx = vis_faligndata(dx0, dx1);
				for (j = 0; j < (src_width / 8); j++) {
					ZOOM_IN2X_S16_1_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					(void *)vis_alignaddr(px_row, 0);
					dx = vis_faligndata(dx0, dx1);
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_16(dr, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz += 2;
					vis_pst_16(dr1, dpz, emask_lo);
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_16(dr, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz1 += 2;
					vis_pst_16(dr1, dpz1, emask1_lo);
					dpx++;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				for (j = src_width / 8 * 4; j < (src_width / 2);
				    j++) {
					pdst[2 * i * dst_stride + 2 * j] =
					    pdst[2 * i * dst_stride + 2 * j +
					    1] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * j + 1] =
					    psrc[i * src_stride + j];
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_S16_2NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
	mlib_s32 i, j;

	mlib_s16 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx0, dx1, dr_hi, dr_lo, dr, dr1;

	if ((src_width / 2) < 4) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				mlib_s32 a0 = psrc[i * src_stride + 2 * j],
				    a1 = psrc[i * src_stride + 2 * j + 1];
				pdst[2 * i * dst_stride + 4 * j] =
				    pdst[2 * i * dst_stride + 4 * j + 2] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j + 2] =
				    a0;
				pdst[2 * i * dst_stride + 4 * j + 1] =
				    pdst[2 * i * dst_stride + 4 * j + 3] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 4 * j + 3] =
				    a1;
			}
		}
	} else {
		if ((((mlib_s32)pdst & 7) | (dst_stride & 3)) == 0) {
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				for (j = 0; j <= (src_width / 2 - 2); j += 2) {
					dx = vis_faligndata(dx0, dx1);
					ZOOM_IN2X_S16_2_NEAREST;
					dx0 = dx1;
					dx1 = dpx[2];
					((mlib_d64 *)pz_row)[0] =
					    ((mlib_d64 *)pz1_row)[0] = dr_hi;
					((mlib_d64 *)pz_row)[1] =
					    ((mlib_d64 *)pz1_row)[1] = dr_lo;
					dpx++;
					pz_row += 8;
					pz1_row += 8;
				}
			}

			if (j < (src_width / 2)) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_s32 a0 =
					    (((mlib_u16 *)psrc)[i * src_stride +
					    2 *
					    j] << 16) | ((mlib_u16 *)psrc)[i *
					    src_stride + 2 * j + 1];
					((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 1) + 2 * j] =
					    ((mlib_s32 *)pdst)[2 * i *
					    (dst_stride >> 1) + 2 * j + 1] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 1) + 2 * j] =
					    ((mlib_s32 *)pdst)[(2 * i +
					    1) * (dst_stride >> 1) + 2 * j +
					    1] = a0;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				emask_hi =
				    0xf >> (((mlib_u32)pz_row -
				    (mlib_u32)dpz) >> 1);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xf >> (((mlib_u32)pz1_row -
				    (mlib_u32)dpz1) >> 1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dx0 = dpx[0];
				dx1 = dpx[1];
				dx = vis_faligndata(dx0, dx1);
				ZOOM_IN2X_S16_2_NEAREST;
				for (j = 0; j <= (src_width / 2 - 2); j += 2) {
					dx0 = dx1;
					dx1 = dpx[2];
					(void *)vis_alignaddr((void *)all_z, 0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_16(dr, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz += 2;
					(void *)vis_alignaddr(px_row, 0);
					dx = vis_faligndata(dx0, dx1);
					vis_pst_16(dr1, dpz, emask_lo);
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					dr = vis_faligndata(dr_hi, dr_hi);
					vis_pst_16(dr, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr_hi, dr_lo);
					dr1 = vis_faligndata(dr_lo, dr_lo);
					dpz1 += 2;
					vis_pst_16(dr1, dpz1, emask1_lo);
					dpx++;
					ZOOM_IN2X_S16_2_NEAREST;
				}
			}

			if (j < (src_width / 2)) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_s32 a0 =
					    psrc[i * src_stride + 2 * j], a1 =
					    psrc[i * src_stride + 2 * j + 1];
					pdst[2 * i * dst_stride + 4 * j] =
					    pdst[2 * i * dst_stride + 4 * j +
					    2] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j + 2] = a0;
					pdst[2 * i * dst_stride + 4 * j + 1] =
					    pdst[2 * i * dst_stride + 4 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    4 * j + 3] = a1;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_S16_3NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
	mlib_s32 i, j;

	mlib_s16 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx1, dx2, dxx0, dxx1, dxx2, dxx3;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;

	if ((src_width / 2) < 8) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				mlib_s32 a0 = psrc[i * src_stride + 3 * j],
				    a1 = psrc[i * src_stride + 3 * j + 1],
				    a2 = psrc[i * src_stride + 3 * j + 2];
				pdst[2 * i * dst_stride + 6 * j] =
				    pdst[2 * i * dst_stride + 6 * j + 3] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 3] =
				    a0;
				pdst[2 * i * dst_stride + 6 * j + 1] =
				    pdst[2 * i * dst_stride + 6 * j + 4] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 4] =
				    a1;
				pdst[2 * i * dst_stride + 6 * j + 2] =
				    pdst[2 * i * dst_stride + 6 * j + 5] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 2] =
				    pdst[(2 * i + 1) * dst_stride + 6 * j + 5] =
				    a2;
			}
		}
	} else {
		if (((dst_stride | (mlib_u32)pdst) & 3) == 0) {
			mlib_s32 beg = ((mlib_u32)pdst & 7) >> 2;

			if (beg) {
				for (i = 0; i < (src_height / 2); i++) {
					mlib_s32 a0 = psrc[i * src_stride],
					    a1 = psrc[i * src_stride + 1],
					    a2 = psrc[i * src_stride + 2];
					pdst[2 * i * dst_stride] =
					    pdst[2 * i * dst_stride + 3] =
					    pdst[(2 * i + 1) * dst_stride] =
					    pdst[(2 * i + 1) * dst_stride + 3] =
					    a0;
					pdst[2 * i * dst_stride + 1] =
					    pdst[2 * i * dst_stride + 4] =
					    pdst[(2 * i + 1) * dst_stride + 1] =
					    pdst[(2 * i + 1) * dst_stride + 4] =
					    a1;
					pdst[2 * i * dst_stride + 2] =
					    pdst[2 * i * dst_stride + 5] =
					    pdst[(2 * i + 1) * dst_stride + 2] =
					    pdst[(2 * i + 1) * dst_stride + 5] =
					    a2;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride + 3 * beg;
				pz_row = pdst + 2 * i * dst_stride + 6 * beg;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dxx3 = dpx[0];
#pragma pipeloop(0)
				for (j = beg; j <= src_width / 2 - 4; j += 4) {
					dxx0 = dxx3;
					dxx1 = dpx[1];
					dxx2 = dpx[2];
					dxx3 = dpx[3];
					dx = vis_faligndata(dxx0, dxx1);
					dx1 = vis_faligndata(dxx1, dxx2);
					dx2 = vis_faligndata(dxx2, dxx3);
					vis_write_bmask(0x01234501, 0);
					dr = vis_bshuffle(dx, dx);
					((mlib_d64 *)pz_row)[0] =
					    ((mlib_d64 *)pz1_row)[0] = dr;
					vis_write_bmask(0x23456789, 0);
					dr1 = vis_bshuffle(dx, dx1);
					((mlib_d64 *)pz_row)[1] =
					    ((mlib_d64 *)pz1_row)[1] = dr1;
					vis_write_bmask(0xAB6789AB, 0);
					dr2 = vis_bshuffle(dx, dx1);
					((mlib_d64 *)pz_row)[2] =
					    ((mlib_d64 *)pz1_row)[2] = dr2;
					vis_write_bmask(0x45678945, 0);
					dr3 = vis_bshuffle(dx1, dx2);
					((mlib_d64 *)pz_row)[3] =
					    ((mlib_d64 *)pz1_row)[3] = dr3;
					vis_write_bmask(0x6789ABCD, 0);
					dr4 = vis_bshuffle(dx1, dx2);
					((mlib_d64 *)pz_row)[4] =
					    ((mlib_d64 *)pz1_row)[4] = dr4;
					vis_write_bmask(0xEFABCDEF, 0);
					dr5 = vis_bshuffle(dx1, dx2);
					((mlib_d64 *)pz_row)[5] =
					    ((mlib_d64 *)pz1_row)[5] = dr5;
					dpx += 3;
					pz_row += 24;
					pz1_row += 24;
				}
			}

			beg = j;
			for (i = 0; i < (src_height / 2); i++) {
				for (j = beg; j < (src_width / 2); j++) {
					mlib_s32 a0 =
					    psrc[i * src_stride + 3 * j], a1 =
					    psrc[i * src_stride + 3 * j + 1],
					    a2 =
					    psrc[i * src_stride + 3 * j + 2];
					pdst[2 * i * dst_stride + 6 * j] =
					    pdst[2 * i * dst_stride + 6 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 3] = a0;
					pdst[2 * i * dst_stride + 6 * j + 1] =
					    pdst[2 * i * dst_stride + 6 * j +
					    4] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 4] = a1;
					pdst[2 * i * dst_stride + 6 * j + 2] =
					    pdst[2 * i * dst_stride + 6 * j +
					    5] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 2] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 5] = a2;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				mlib_d64 *dpx_end;

				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dpx_end = dpx + (src_width / 8) * 3;
				emask_hi =
				    0xf >> (((mlib_u32)pz_row -
				    (mlib_u32)dpz) >> 1);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xf >> (((mlib_u32)pz1_row -
				    (mlib_u32)dpz1) >> 1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dxx0 = dpx[0];
				dxx1 = dpx[1];
				dxx2 = dpx[2];
				dxx3 = dpx[3];
				dx = vis_faligndata(dxx0, dxx1);
/* for (j = 0; j < (src_width/8); j++) { */
/* loop with unroll is worse */
				while ((mlib_u32)dpx < (mlib_u32)dpx_end) {
					dxx0 = dxx3;
					dx1 = vis_faligndata(dxx1, dxx2);
					dxx1 = dpx[4];
					dx2 = vis_faligndata(dxx2, dxx3);
					dxx2 = dpx[5];
					dxx3 = dpx[6];
					ZOOM_IN2X_S16_3_NEAREST;
					(void *)vis_alignaddr((void *)all_z, 0);
					dr6 = vis_faligndata(dr, dr);
					vis_pst_16(dr6, dpz, emask_hi);
					dpz[1] = vis_faligndata(dr, dr1);
					dpz[2] = vis_faligndata(dr1, dr2);
					dpz[3] = vis_faligndata(dr2, dr3);
					dpz[4] = vis_faligndata(dr3, dr4);
					dpz[5] = vis_faligndata(dr4, dr5);
					dr6 = vis_faligndata(dr5, dr5);
					dpz += 6;
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					vis_pst_16(dr6, dpz, emask_lo);
					dr6 = vis_faligndata(dr, dr);
					vis_pst_16(dr6, dpz1, emask1_hi);
					dpz1[1] = vis_faligndata(dr, dr1);
					dpz1[2] = vis_faligndata(dr1, dr2);
					dpz1[3] = vis_faligndata(dr2, dr3);
					dpz1[4] = vis_faligndata(dr3, dr4);
					dpz1[5] = vis_faligndata(dr4, dr5);
					dr6 = vis_faligndata(dr5, dr5);
					dpz1 += 6;
					(void *)vis_alignaddr(px_row, 0);
					dx = vis_faligndata(dxx0, dxx1);
					vis_pst_16(dr6, dpz1, emask1_lo);
					dpx += 3;
				}
			}

			for (i = 0; i < (src_height / 2); i++) {
				for (j = src_width / 8 * 4; j < (src_width / 2);
				    j++) {
					mlib_s32 a0 =
					    psrc[i * src_stride + 3 * j], a1 =
					    psrc[i * src_stride + 3 * j + 1],
					    a2 =
					    psrc[i * src_stride + 3 * j + 2];
					pdst[2 * i * dst_stride + 6 * j] =
					    pdst[2 * i * dst_stride + 6 * j +
					    3] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 3] = a0;
					pdst[2 * i * dst_stride + 6 * j + 1] =
					    pdst[2 * i * dst_stride + 6 * j +
					    4] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 1] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 4] = a1;
					pdst[2 * i * dst_stride + 6 * j + 2] =
					    pdst[2 * i * dst_stride + 6 * j +
					    5] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 2] =
					    pdst[(2 * i + 1) * dst_stride +
					    6 * j + 5] = a2;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_S16_4NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
	mlib_s32 i, j;

	mlib_s16 *px_row, *pz_row, *pz1_row;
	mlib_d64 *dpx, *dpz, *dpz1;
	mlib_s32 emask_hi, emask_lo, emask1_hi, emask1_lo, all_z, all_z1;
	mlib_d64 dx, dx0, dx1, dr, dr1;

	if ((src_width / 2) < 4) { /* small width case */
		for (i = 0; i < (src_height / 2); i++) {
			for (j = 0; j < (src_width / 2); j++) {
				mlib_s32 a0 = psrc[i * src_stride + 4 * j],
				    a1 = psrc[i * src_stride + 4 * j + 1],
				    a2 = psrc[i * src_stride + 4 * j + 2],
				    a3 = psrc[i * src_stride + 4 * j + 3];
				pdst[2 * i * dst_stride + 8 * j] =
				    pdst[2 * i * dst_stride + 8 * j + 4] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 4] =
				    a0;
				pdst[2 * i * dst_stride + 8 * j + 1] =
				    pdst[2 * i * dst_stride + 8 * j + 5] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 1] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 5] =
				    a1;
				pdst[2 * i * dst_stride + 8 * j + 2] =
				    pdst[2 * i * dst_stride + 8 * j + 6] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 2] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 6] =
				    a2;
				pdst[2 * i * dst_stride + 8 * j + 3] =
				    pdst[2 * i * dst_stride + 8 * j + 7] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 3] =
				    pdst[(2 * i + 1) * dst_stride + 8 * j + 7] =
				    a3;
			}
		}
	} else {
		if ((((mlib_s32)pdst & 7) | (dst_stride & 3)) == 0) {
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				for (j = 0; j < (src_width / 2); j++) {
					dx = vis_faligndata(dx0, dx1);
					((mlib_d64 *)pz_row)[0] = dx;
					dx0 = dx1;
					dx1 = dpx[2];
					((mlib_d64 *)pz1_row)[0] =
					    ((mlib_d64 *)pz_row)[1] =
					    ((mlib_d64 *)pz1_row)[1] = dx;
					dpx++;
					pz_row += 8;
					pz1_row += 8;
				}
			}
		} else { /* common case */
			for (i = 0; i < (src_height / 2); i++) {
				px_row = psrc + i * src_stride;
				pz_row = pdst + 2 * i * dst_stride;
				pz1_row = pz_row + dst_stride;
				dpz = (mlib_d64 *)vis_alignaddr(pz_row, -1);
				dpz1 = (mlib_d64 *)vis_alignaddr(pz1_row, -1);
				dpx = (mlib_d64 *)vis_alignaddr(px_row, 0);
				emask_hi =
				    0xf >> (((mlib_u32)pz_row -
				    (mlib_u32)dpz) >> 1);
				emask_lo = ~emask_hi;
				emask1_hi =
				    0xf >> (((mlib_u32)pz1_row -
				    (mlib_u32)dpz1) >> 1);
				emask1_lo = ~emask1_hi;
				all_z = 8 - (mlib_u32)pz_row;
				all_z1 = 8 - (mlib_u32)pz1_row;
				dx0 = dpx[0];
				dx1 = dpx[1];
				dx = vis_faligndata(dx0, dx1);
				(void *)vis_alignaddr((void *)all_z, 0);
				for (j = 0; j < (src_width / 2); j++) {
					dx0 = dx1;
					dx1 = dpx[2];
					dr = vis_faligndata(dx, dx);
					vis_pst_16(dr, dpz, emask_hi);
					dpz[1] = dr;
					dpz += 2;
					(void *)vis_alignaddr((void *)all_z1,
					    0);
					vis_pst_16(dr, dpz, emask_lo);
					dr1 = vis_faligndata(dx, dx);
					vis_pst_16(dr1, dpz1, emask1_hi);
					(void *)vis_alignaddr(px_row, 0);
					dpz1[1] = dr1;
					dx = vis_faligndata(dx0, dx1);
					dpz1 += 2;
					vis_pst_16(dr1, dpz1, emask1_lo);
					(void *)vis_alignaddr((void *)all_z, 0);
					dpx++;
				}
			}
		}
	}
}

/* *********************************************************** */
