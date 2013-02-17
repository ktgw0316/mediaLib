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

#pragma ident	"@(#)mlib_v_ImageCrossCorrel.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageCrossCorrel - calculates image cross correlation
 *                              for the input images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCrossCorrel(mlib_d64         *correl,
 *                                        const mlib_image *img1,
 *                                        const mlib_image *img2);
 *
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      img1     pointer to the first input image
 *      img2     pointer to the second input image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      c = --- * SUM SUM(x[i][j] * y[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageStat.h>
#include <mlib_v_ImageCorrel.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCrossCorrel = __mlib_ImageCrossCorrel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCrossCorrel) mlib_ImageCrossCorrel
    __attribute__((weak, alias("__mlib_ImageCrossCorrel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCrossCorrel(
    mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2)
{
	mlib_s32 num_pixel;
	mlib_d64 rez[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img1);
	MLIB_IMAGE_CHECK(img2);
	MLIB_IMAGE_SIZE_EQUAL(img1, img2);
	MLIB_IMAGE_TYPE_EQUAL(img1, img2);
	MLIB_IMAGE_CHAN_EQUAL(img1, img2);

	if (correl == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img1) * mlib_ImageGetHeight(img1);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img1)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_v_ImageCrossCorrel_U8_3(img1, img2, rez);
		else
			mlib_v_ImageCrossCorrel_U8_124(img1, img2, rez);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_v_ImageCrossCorrel_U16_3(img1, img2, rez);
		else
			mlib_v_ImageCrossCorrel_U16_124(img1, img2, rez);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_v_ImageCrossCorrel_S16_3(img1, img2, rez);
		else
			mlib_v_ImageCrossCorrel_S16_124(img1, img2, rez);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_v_ImageCrossCorrel_S32_3(img1, img2, rez);
		else
			mlib_v_ImageCrossCorrel_S32_124(img1, img2, rez);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img1)) {
	case 1:
		correl[0] = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		break;

	case 2:
		correl[0] = (rez[0] + rez[2]) * divider;
		correl[1] = (rez[1] + rez[3]) * divider;
		break;

	case 4:
		correl[3] = rez[3] * divider;
	case 3:
		correl[0] = rez[0] * divider;
		correl[1] = rez[1] * divider;
		correl[2] = rez[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_U8_U32                                                   \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);                 \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);                 \
	dr2 = vis_fpmerge(vis_read_hi(dzero), vis_read_hi(dy));      \
	dr4 = vis_fpmerge(vis_read_hi(dzero), vis_read_lo(dy));      \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dxh), vis_read_hi(dr2));   \
	dr2 = vis_fmuld8ulx16(vis_read_lo(dxh), vis_read_lo(dr2));   \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dxl), vis_read_hi(dr4));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dxl), vis_read_lo(dr4))

/* *********************************************************** */

#define	SUM_U32                                                 \
	dc1 = vis_fpadd32(dr1, dc1);                            \
	dc2 = vis_fpadd32(dr2, dc2);                            \
	dc1 = vis_fpadd32(dr3, dc1);                            \
	dc2 = vis_fpadd32(dr4, dc2)

#define	MAX_ELEM	0x20000

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_U8_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* linestride = elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* linestride = elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;

/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 size, num_row;
	mlib_u8 *psrc1, *psrc2;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dxh, dxl, dy0, dy1, dr1, dr2, dr3, dr4, dc1, dc2;
	const mlib_f32 fone = vis_to_float(0x100);
	const mlib_d64 dzero = vis_fzero();
	type_union_mlib_d64 dd1, dd2;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = rez[3] = dzero;
	for (i = 0; i < height; i++) {
		psrc1 = point_src1;
		psrc2 = point_src2;

/* prepare the source address */
		off = (mlib_addr)psrc1 & 7;
		emask = vis_edge8(psrc1, psrc1 + size_row - 1);
		num_row = size_row + off;
		psrc1 -= off;
		psrc2 -= off;
		dpsrc1 = (mlib_d64 *)psrc1;
		dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, 0);

		midd[0] = midd[1] = dc1 = dc2 = dzero;

		vis_pst_8(dpsrc1[0], (void *)midd, emask);
		dx = midd[0];
		dy0 = vis_ld_d64_nf(dpsrc2);
		dy1 = vis_ld_d64_nf(dpsrc2 + 1);
		dy = vis_faligndata(dy0, dy1);

		dpsrc1++;
		dpsrc2++;

		while (num_row > 7) {
			size = num_row;

			if (size > MAX_ELEM)
				size = MAX_ELEM;

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
				for (j = 0; j <= (size - 8); j += 8) {
					MUL_U8_U32;
					SUM_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j);
					dy = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j);
				}
			} else {
				for (j = 0; j <= (size - 8); j += 8) {
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U32;
					SUM_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j);
					dy0 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j);
					dy1 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 8);
				}
			}

			num_row -= j;
			dpsrc1 = (mlib_d64 *)((mlib_u8 *)dpsrc1 + j);
			dpsrc2 = (mlib_d64 *)((mlib_u8 *)dpsrc2 + j);
			dy0 = vis_ld_d64_nf(dpsrc2 - 1);
			dy1 = vis_ld_d64_nf(dpsrc2);

			dd1.db = dc1;
			dd2.db = dc2;
			rez[3 & (0 - off)] += (mlib_d64)dd1.two_int.int0;
			rez[3 & (1 - off)] += (mlib_d64)dd1.two_int.int1;
			rez[3 & (2 - off)] += (mlib_d64)dd2.two_int.int0;
			rez[3 & (3 - off)] += (mlib_d64)dd2.two_int.int1;
			dc1 = dc2 = dzero;
		}

		if (num_row > 0) {
			dy = vis_faligndata(dy0, dy1);
			emask =
			    vis_edge8(dpsrc1, (mlib_u8 *)dpsrc1 + num_row - 1);
			vis_pst_8(dx, (void *)(midd + 1), emask);
			dx = midd[1];
			MUL_U8_U32;
			SUM_U32;
			dd1.db = dc1;
			dd2.db = dc2;
			rez[3 & (0 - off)] += (mlib_d64)dd1.two_int.int0;
			rez[3 & (1 - off)] += (mlib_d64)dd1.two_int.int1;
			rez[3 & (2 - off)] += (mlib_d64)dd2.two_int.int0;
			rez[3 & (3 - off)] += (mlib_d64)dd2.two_int.int1;
		}

		point_src1 += src1_stride;
		point_src2 += src2_stride;
	}
}

/* *********************************************************** */

#define	EMUL(freg)	vis_fmul8x16al(freg, fone)
#define	EMER(freg)	vis_fpmerge(vis_read_hi(dzero), freg)

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_U8_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 size, num_row;
	mlib_u8 *psrc1, *psrc2;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dxh, dxl, dy0, dy1;
	mlib_d64 dr1, dr2, dr3, dr4, dc1, dc2, dc3;
	mlib_d64 acc1, acc2, acc3;
	const mlib_f32 fone = vis_to_float(0x100);

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dd1, dd2, dd3;
	const mlib_d64 dzero = vis_fzero();

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = 0;

/* calculate image cross correlation for 3 channels */

	for (i = 0; i < height; i++) {
		num_row = size_row;
		psrc1 = point_src1;
		psrc2 = point_src2;

/* prepare the source address */
		off = (mlib_addr)psrc1 & 7;
		emask = vis_edge8(psrc1, psrc1 + size_row - 1);
		num_row = size_row + off;
		psrc1 -= off;
		psrc2 -= off;
		dpsrc1 = (mlib_d64 *)psrc1;
		dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, 0);

		midd[0] = midd[1] = acc1 = acc2 = acc3 = dc1 = dc2 = dc3 =
		    dzero;

		vis_pst_8(dpsrc1[0], (void *)midd, emask);
		dx = midd[0];
		dy0 = vis_ld_d64_nf(dpsrc2);
		dy1 = vis_ld_d64_nf(dpsrc2 + 1);

		while (num_row >= 8) {
			size = num_row & ~7;

			if (size > MAX_ELEM / 4 * 3)
				size = MAX_ELEM / 4 * 3;

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
				dy = dy0;
#pragma pipeloop(0)
				for (j = 0; j <= (size - 8); j += 24) {
					MUL_U8_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j + 24);
					dy = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 24);
					dc1 = vis_fpadd32(dc1, dr1);
					dc2 = vis_fpadd32(dc2, dr2);
					dc3 = vis_fpadd32(dc3, dr3);
					dc1 = vis_fpadd32(dc1, dr4);
				}

				dx = vis_ld_d64_nf(dpsrc1 + 1);
				dy = vis_ld_d64_nf(dpsrc2 + 1);
				for (j = 8; j <= (size - 8); j += 24) {
					MUL_U8_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j + 24);
					dy = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 24);
					dc2 = vis_fpadd32(dc2, dr1);
					dc3 = vis_fpadd32(dc3, dr2);
					dc1 = vis_fpadd32(dc1, dr3);
					dc2 = vis_fpadd32(dc2, dr4);
				}

				dx = vis_ld_d64_nf(dpsrc1 + 2);
				dy = vis_ld_d64_nf(dpsrc2 + 2);
				for (j = 16; j <= (size - 8); j += 24) {
					MUL_U8_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j + 24);
					dy = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 24);
					dc3 = vis_fpadd32(dc3, dr1);
					dc1 = vis_fpadd32(dc1, dr2);
					dc2 = vis_fpadd32(dc2, dr3);
					dc3 = vis_fpadd32(dc3, dr4);
				}
			} else {
#pragma pipeloop(0)
				for (j = 0; j <= (size - 8); j += 24) {
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j + 24);
					dy0 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 24);
					dy1 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 32);
					dc1 = vis_fpadd32(dc1, dr1);
					dc2 = vis_fpadd32(dc2, dr2);
					dc3 = vis_fpadd32(dc3, dr3);
					dc1 = vis_fpadd32(dc1, dr4);
				}

				dx = vis_ld_d64_nf(dpsrc1 + 1);
				dy0 = vis_ld_d64_nf(dpsrc2 + 1);
				dy1 = vis_ld_d64_nf(dpsrc2 + 2);
				for (j = 8; j <= (size - 8); j += 24) {
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j + 24);
					dy0 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 24);
					dy1 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 32);
					dc2 = vis_fpadd32(dc2, dr1);
					dc3 = vis_fpadd32(dc3, dr2);
					dc1 = vis_fpadd32(dc1, dr3);
					dc2 = vis_fpadd32(dc2, dr4);
				}

				dx = vis_ld_d64_nf(dpsrc1 + 2);
				dy0 = vis_ld_d64_nf(dpsrc2 + 2);
				dy1 = vis_ld_d64_nf(dpsrc2 + 3);
				for (j = 16; j <= (size - 8); j += 24) {
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U32;
					dx = vis_ld_d64_nf((mlib_u8 *)dpsrc1 +
						j + 24);
					dy0 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 24);
					dy1 = vis_ld_d64_nf((mlib_u8 *)dpsrc2 +
						j + 32);
					dc3 = vis_fpadd32(dc3, dr1);
					dc1 = vis_fpadd32(dc1, dr2);
					dc2 = vis_fpadd32(dc2, dr3);
					dc3 = vis_fpadd32(dc3, dr4);
				}
			}

			num_row -= size;
			dpsrc1 = (mlib_d64 *)((mlib_u8 *)dpsrc1 + size);
			dpsrc2 = (mlib_d64 *)((mlib_u8 *)dpsrc2 + size);
			dx = vis_ld_d64_nf(dpsrc1);
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);

			dd1.db = dc1;
			dd2.db = dc2;
			dd3.db = dc3;
			dd1.two_float.fl0 =
			    vis_fpadd32s(dd1.two_float.fl0, dd2.two_float.fl1);
			dd1.two_float.fl1 =
			    vis_fpadd32s(dd1.two_float.fl1, dd3.two_float.fl0);
			dd2.two_float.fl0 =
			    vis_fpadd32s(dd2.two_float.fl0, dd3.two_float.fl1);
			acc1 += dd1.two_int.int0;
			acc2 += dd1.two_int.int1;
			acc3 += dd2.two_int.int0;
			dc1 = dc2 = dc3 = dzero;
		}

		if (num_row > 0) {
			dy = vis_faligndata(dy0, dy1);
			emask = (mlib_s32)0xff00 >> num_row;
			vis_pst_8(dx, (void *)(midd + 1), emask);
			dx = midd[1];
			MUL_U8_U32;
			dc1 = vis_fpadd32(dc1, dr1);
			dc2 = vis_fpadd32(dc2, dr2);
			dc3 = vis_fpadd32(dc3, dr3);
			dc1 = vis_fpadd32(dc1, dr4);
			dd1.db = dc1;
			dd2.db = dc2;
			dd3.db = dc3;
			dd1.two_float.fl0 =
			    vis_fpadd32s(dd1.two_float.fl0, dd2.two_float.fl1);
			dd1.two_float.fl1 =
			    vis_fpadd32s(dd1.two_float.fl1, dd3.two_float.fl0);
			dd2.two_float.fl0 =
			    vis_fpadd32s(dd2.two_float.fl0, dd3.two_float.fl1);
			rez[al2off[num_row + 0]] += dd1.two_int.int0;
			rez[al2off[num_row + 2]] += dd1.two_int.int1;
			rez[al2off[num_row + 1]] += dd2.two_int.int0;
		}

		rez[al2off[off + 0]] += acc1;
		rez[al2off[off + 2]] += acc2;
		rez[al2off[off + 1]] += acc3;
		point_src1 += src1_stride;
		point_src2 += src2_stride;
	}
}

/* *********************************************************** */

#define	MUL_U16_D64                                                       \
	sd1 = vis_fxor((dx), offset);                                     \
	sd2 = vis_fxor((dy), offset);                                     \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdh = vis_fpadd32(rdhh, rdhl);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd2));   \
	rdh = vis_fpadd32(rdh, offset3);                                  \
	rdh_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdh = vis_fpsub32(rdh, rdh_1);                                    \
	rdh_0 = vis_fxor(rdh, offset2);                                   \
	c1 += (mlib_d64)(((mlib_s32 *)(&rdh_0))[0]);                      \
	c2 += (mlib_d64)(((mlib_s32 *)(&rdh_0))[1]);                      \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdl = vis_fpadd32(rdlh, rdll);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd2));   \
	rdl = vis_fpadd32(rdl, offset3);                                  \
	rdl_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdl = vis_fpsub32(rdl, rdl_1);                                    \
	rdl_0 = vis_fxor(rdl, offset2);                                   \
	c3 += (mlib_d64)(((mlib_s32 *)(&rdl_0))[0]);                      \
	c4 += (mlib_d64)(((mlib_s32 *)(&rdl_0))[1])

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_U16_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);
	mlib_s32 num_row;

/* indices */
	mlib_s32 i, j;

/* edge masks */
	mlib_s32 emask;

/* offset of address alignment src1 */
	mlib_s32 off;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 compens;

	mlib_d64 rdhh, rdhl;
	mlib_d64 rdlh, rdll;
	mlib_d64 rdh, rdl;
	mlib_d64 sd1, sd2, sd1ad, sd2ad, rdh_0, rdh_1, rdl_0, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 offset2 = vis_to_double_dup(0x80000000);
	mlib_d64 offset3 = vis_to_double_dup(0x40000000);

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = rez[3] = vis_fzero();

	for (i = 0; i < height; i++) {
		midd[0] = midd[1] = c1 = c2 = c3 = c4 = vis_fzero();

/* prepare the source address */
		off = (mlib_addr)psrc1 & 7;
		off >>= 1;
		emask = vis_edge16(psrc1, psrc1 + size_row - 1);
		num_row = size_row + off;
		dpsrc1 = (mlib_d64 *)(psrc1 - off);
		dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

		vis_pst_16(dpsrc1[0], (void *)midd, emask);
		dx = midd[0];
		dy0 = vis_ld_d64_nf(dpsrc2);
		dy1 = vis_ld_d64_nf(dpsrc2 + 1);
		dy = vis_faligndata(dy0, dy1);
		compens = ((num_row + 3) >> 2) * (mlib_d64)0x80000000;

		if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
#pragma pipeloop(0)
			for (j = 0; j <= (num_row - 8); j += 8) {
				MUL_U16_D64;
				dx = dpsrc1[1];
				dy = dpsrc2[1];
				MUL_U16_D64;
				dx = vis_ld_d64_nf(dpsrc1 + 2);
				dy = vis_ld_d64_nf(dpsrc2 + 2);
				dpsrc1 += 2;
				dpsrc2 += 2;
			}
		} else {
#pragma pipeloop(0)
			for (j = 0; j <= (num_row - 8); j += 8) {
				MUL_U16_D64;
				dx = vis_ld_d64_nf(dpsrc1 + 1);
				dy0 = vis_ld_d64_nf(dpsrc2 + 2);
				dy = vis_faligndata(dy1, dy0);
				MUL_U16_D64;
				dx = vis_ld_d64_nf(dpsrc1 + 2);
				dy1 = vis_ld_d64_nf(dpsrc2 + 3);
				dy = vis_faligndata(dy0, dy1);
				dpsrc1 += 2;
				dpsrc2 += 2;
			}
		}

		if (num_row & 4) {
			MUL_U16_D64;
			dpsrc1++;
			dpsrc2++;
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			dx = vis_ld_d64_nf(dpsrc1);
		}

		if (num_row & 3) {
			num_row &= 3;
/* prepare edge mask for the last bytes */
			emask = 0xf0 >> num_row;
			vis_pst_16(dx, (void *)(midd + 1), emask);
			dx = midd[1];
			MUL_U16_D64;
		}

		rez[3 & (0 - off)] += c1 + compens;
		rez[3 & (1 - off)] += c2 + compens;
		rez[3 & (2 - off)] += c3 + compens;
		rez[3 & (3 - off)] += c4 + compens;
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}
}

/* *********************************************************** */

#define	MUL_U16_D64_3                                                     \
	sd1 = vis_fxor((dx), offset);                                     \
	sd2 = vis_fxor((dy), offset);                                     \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdh = vis_fpadd32(rdhh, rdhl);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd2));   \
	rdh_1 = vis_fpadd32(sd2ad, sd1ad);                                \
	rdh = vis_fpadd32(rdh, offset3);                                  \
	rdh = vis_fpsub32(rdh, rdh_1);                                    \
	rdh = vis_fxor(rdh, offset2);                                     \
	c1 += (mlib_d64)(((mlib_s32 *)(&rdh))[0]);                        \
	c2 += (mlib_d64)(((mlib_s32 *)(&rdh))[1]);                        \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdl = vis_fpadd32(rdlh, rdll);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd2));   \
	rdl_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdl = vis_fpadd32(rdl, offset3);                                  \
	rdl = vis_fpsub32(rdl, rdl_1);                                    \
	rdl = vis_fxor(rdl, offset2);                                     \
	c3 += (mlib_d64)(((mlib_s32 *)(&rdl))[0]);                        \
	c1 += (mlib_d64)(((mlib_s32 *)(&rdl))[1]);                        \
	sd1 = vis_fxor((dx2), offset);                                    \
	sd2 = vis_fxor((dy1), offset);                                    \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdh = vis_fpadd32(rdhh, rdhl);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd2));   \
	rdh_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdh = vis_fpadd32(rdh, offset3);                                  \
	rdh = vis_fpsub32(rdh, rdh_1);                                    \
	rdh = vis_fxor(rdh, offset2);                                     \
	c2 += (mlib_d64)(((mlib_s32 *)(&rdh))[0]);                        \
	c3 += (mlib_d64)(((mlib_s32 *)(&rdh))[1]);                        \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdl = vis_fpadd32(rdlh, rdll);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd2));   \
	rdl_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdl = vis_fpadd32(rdl, offset3);                                  \
	rdl = vis_fpsub32(rdl, rdl_1);                                    \
	rdl = vis_fxor(rdl, offset2);                                     \
	c1 += (mlib_d64)(((mlib_s32 *)(&rdl))[0]);                        \
	c2 += (mlib_d64)(((mlib_s32 *)(&rdl))[1]);                        \
	sd1 = vis_fxor((dx3), offset);                                    \
	sd2 = vis_fxor((dy2), offset);                                    \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));       \
	rdh = vis_fpadd32(rdhh, rdhl);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd2));   \
	rdh_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdh = vis_fpadd32(rdh, offset3);                                  \
	rdh = vis_fpsub32(rdh, rdh_1);                                    \
	rdh = vis_fxor(rdh, offset2);                                     \
	c3 += (mlib_d64)(((mlib_s32 *)(&rdh))[0]);                        \
	c1 += (mlib_d64)(((mlib_s32 *)(&rdh))[1]);                        \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));       \
	rdl = vis_fpadd32(rdlh, rdll);                                    \
	sd1ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd1));   \
	sd2ad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd2));   \
	rdl_1 = vis_fpadd32(sd1ad, sd2ad);                                \
	rdl = vis_fpadd32(rdl, offset3);                                  \
	rdl = vis_fpsub32(rdl, rdl_1);                                    \
	rdl = vis_fxor(rdl, offset2);                                     \
	c2 += (mlib_d64)(((mlib_s32 *)(&rdl))[0]);                        \
	c3 += (mlib_d64)(((mlib_s32 *)(&rdl))[1])

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_U16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_d64 midd[3], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dx2, dx3, dy1, dy2, dy3;
	mlib_s32 num_row;

/* edge masks */
	mlib_s32 emask;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };
	mlib_s32 compens;

	mlib_d64 rdhh, rdhl;
	mlib_d64 rdlh, rdll;
	mlib_d64 rdh, rdl;
	mlib_d64 sd1, sd2, sd1ad, sd2ad, rdh_1, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 offset2 = vis_to_double_dup(0x80000000);
	mlib_d64 offset3 = vis_to_double_dup(0x40000000);
	mlib_d64 offset4 = (mlib_d64)0x100000000ULL;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = vis_fzero();

	for (i = 0; i < height; i++) {
		c1 = c2 = c3 = midd[0] = vis_fzero();

/* prepare the source address */
		off = (mlib_addr)psrc1 & 7;
		off >>= 1;
		emask = vis_edge16(psrc1, psrc1 + size_row - 1);
		num_row = size_row + off;
		dpsrc1 = (mlib_d64 *)(psrc1 - off);
		dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

		vis_pst_16(dpsrc1[0], &midd[0], emask);
		dx = midd[0];
		j = num_row >> 2;
		dy = vis_ld_d64_nf(dpsrc2);
		compens = 0;

		if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
			for (; j >= 3; j -= 3) {
				dx2 = dpsrc1[1];
				dy1 = dpsrc2[1];
				dx3 = dpsrc1[2];
				dy2 = dpsrc2[2];
				MUL_U16_D64_3;
				dpsrc1 += 3;
				dpsrc2 += 3;
				dx = vis_ld_d64_nf(dpsrc1);
				dy = vis_ld_d64_nf(dpsrc2);
				compens += 2;
			}
		} else {
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy, dy1);
			for (; j >= 3; j -= 3) {
				dx2 = dpsrc1[1];
				dy2 = dpsrc2[2];
				dy1 = vis_faligndata(dy1, dy2);
				dx3 = dpsrc1[2];
				dy3 = dpsrc2[3];
				dy2 = vis_faligndata(dy2, dy3);
				MUL_U16_D64_3;
				dpsrc1 += 3;
				dpsrc2 += 3;
				dx = vis_ld_d64_nf(dpsrc1);
				dy1 = vis_ld_d64_nf(dpsrc2 + 1);
				dy = vis_faligndata(dy3, dy1);
				compens += 2;
			}
		}

		num_row = j * 4 + (num_row & 3);

		if (num_row > 0) {
			mlib_d64 bufy[3];

			dy = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy2 = vis_ld_d64_nf(dpsrc2 + 2);
			dy3 = vis_ld_d64_nf(dpsrc2 + 3);
			bufy[0] = vis_faligndata(dy, dy1);
			bufy[1] = vis_faligndata(dy1, dy2);
			bufy[2] = vis_faligndata(dy2, dy3);
			midd[0] = midd[1] = midd[2] = vis_fzero();
			emask = 0xfff000 >> num_row;
			dx2 = vis_ld_d64_nf(dpsrc1 + 1);
			dx3 = vis_ld_d64_nf(dpsrc1 + 2);
			vis_pst_16(dx, &midd[0], emask >> 8);
			vis_pst_16(dx2, &midd[1], emask >> 4);
			vis_pst_16(dx3, &midd[2], emask);
			for (j = 0; j < 12; j += 3) {
				c1 +=
				    ((mlib_u16 *)midd)[j] *
				    (mlib_d64)((mlib_u16 *)bufy)[j];
				c2 +=
				    ((mlib_u16 *)midd)[j +
				    1] * (mlib_d64)((mlib_u16 *)bufy)[j + 1];
				c3 +=
				    ((mlib_u16 *)midd)[j +
				    2] * (mlib_d64)((mlib_u16 *)bufy)[j + 2];
			}
		}

		rez[al2off[off + 0]] += c1 + compens * offset4;
		rez[al2off[off + 2]] += c2 + compens * offset4;
		rez[al2off[off + 1]] += c3 + compens * offset4;

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}
}

/* *********************************************************** */

#define	MUL_S16_S32_A                                              \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr1 = vis_fpadd32(dr1, mone);                              \
/*                                                                 \
 * sub one                                                         \
 */                                                                \
	dr3 = vis_fpadd32(dr3, mone);                              \
/*                                                                 \
 * sub one                                                         \
 */                                                                \
	dc1.db = vis_fpadd32(dr1, dr2);                            \
	dc2.db = vis_fpadd32(dr3, dr4)

/* *********************************************************** */

#define	MUL_S16_S32_B                                              \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	dc1.db = vis_fpadd32(dc1.db, dr1);                         \
	dc1.db = vis_fpadd32(dc1.db, dr2);                         \
	dc2.db = vis_fpadd32(dc2.db, dr3);                         \
	dc2.db = vis_fpadd32(dc2.db, dr4)

/* *********************************************************** */

#define	SUM_D64                                                 \
	c1 += (mlib_d64)dc1.two_int.int0;                       \
	c2 += (mlib_d64)dc1.two_int.int1;                       \
	c3 += (mlib_d64)dc2.two_int.int0;                       \
	c4 += (mlib_d64)dc2.two_int.int1

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_S16_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);
	mlib_s32 num_row;

/* indices */
	mlib_s32 i, j;

/* edge masks */
	mlib_s32 emask;

/* offset of address alignment src1 */
	mlib_s32 off;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dy0, dy1, dr1, dr2, dr3, dr4;
	type_union_mlib_d64 dc1, dc2;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = rez[3] = vis_fzero();

	for (i = 0; i < height; i++) {
		midd[0] = midd[1] = c1 = c2 = c3 = c4 = vis_fzero();

/* prepare the source address */
		off = (mlib_addr)psrc1 & 7;
		off >>= 1;
		emask = vis_edge16(psrc1, psrc1 + size_row - 1);
		num_row = size_row + off;
		dpsrc1 = (mlib_d64 *)(psrc1 - off);
		dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

		vis_pst_16(dpsrc1[0], (void *)midd, emask);
		dx = midd[0];
		dy0 = vis_ld_d64_nf(dpsrc2);
		dy1 = vis_ld_d64_nf(dpsrc2 + 1);
		dy = vis_faligndata(dy0, dy1);
		compens = (num_row + 4) >> 3;

		if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
			for (j = 0; j <= (num_row - 8); j += 8) {
				MUL_S16_S32_A;
				dx = dpsrc1[1];
				dy = dpsrc2[1];
				MUL_S16_S32_B;
				SUM_D64;
				dx = vis_ld_d64_nf(dpsrc1 + 2);
				dy = vis_ld_d64_nf(dpsrc2 + 2);
				dpsrc1 += 2;
				dpsrc2 += 2;
			}
		} else {
			for (j = 0; j <= (num_row - 8); j += 8) {
				MUL_S16_S32_A;
				dx = dpsrc1[1];
				dy0 = vis_ld_d64_nf(dpsrc2 + 2);
				dy = vis_faligndata(dy1, dy0);
				MUL_S16_S32_B;
				SUM_D64;
				dx = vis_ld_d64_nf(dpsrc1 + 2);
				dy1 = vis_ld_d64_nf(dpsrc2 + 3);
				dy = vis_faligndata(dy0, dy1);
				dpsrc1 += 2;
				dpsrc2 += 2;
			}
		}

		dc1.db = dc2.db = vis_fzero();

		if (num_row & 4) {
			MUL_S16_S32_A;
			dpsrc1++;
			dpsrc2++;
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			dx = vis_ld_d64_nf(dpsrc1);
		}

		if (num_row & 3) {
			num_row &= 3;
/* prepare edge mask for the last bytes */
			emask = 0xf0 >> num_row;
			vis_pst_16(dx, (void *)(midd + 1), emask);
			dx = midd[1];
			MUL_S16_S32_B;
		}

		SUM_D64;
		rez[3 & (0 - off)] += c1 + (mlib_d64)compens;
		rez[3 & (1 - off)] += c2 + (mlib_d64)compens;
		rez[3 & (2 - off)] += c3 + (mlib_d64)compens;
		rez[3 & (3 - off)] += c4 + (mlib_d64)compens;

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}
}

/* *********************************************************** */

#define	MUL_S16_S32_3                                                \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));     \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));     \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));     \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));     \
	dr1 = vis_fpadd32(dr1, mone);                                \
/*                                                                   \
 * sub one                                                           \
 */                                                                  \
	dr3 = vis_fpadd32(dr3, mone);                                \
/*                                                                   \
 * sub one                                                           \
 */                                                                  \
	dc1.db = vis_fpadd32(dr1, dr2);                              \
	dc2.db = vis_fpadd32(dr3, dr4);                              \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx2), vis_read_hi(dy1));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx2), vis_read_hi(dy1));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx2), vis_read_lo(dy1));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx2), vis_read_lo(dy1));   \
	dr1 = vis_fpadd32(dr1, mone);                                \
/*                                                                   \
 * sub one                                                           \
 */                                                                  \
	dc3.db = vis_fpadd32(dr1, dr2);                              \
	dc1.db = vis_fpadd32(dc1.db, dr3);                           \
	dc1.db = vis_fpadd32(dc1.db, dr4);                           \
	c1 += (mlib_d64)dc1.two_int.int0;                            \
	c2 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx3), vis_read_hi(dy2));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx3), vis_read_hi(dy2));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx3), vis_read_lo(dy2));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx3), vis_read_lo(dy2));   \
	dc2.db = vis_fpadd32(dc2.db, dr1);                           \
	dc2.db = vis_fpadd32(dc2.db, dr2);                           \
	dc3.db = vis_fpadd32(dc3.db, dr3);                           \
	dc3.db = vis_fpadd32(dc3.db, dr4);                           \
	c3 += (mlib_d64)dc2.two_int.int0;                            \
	c1 += (mlib_d64)dc2.two_int.int1;                            \
	c2 += (mlib_d64)dc3.two_int.int0;                            \
	c3 += (mlib_d64)dc3.two_int.int1

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_S16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_d64 midd[3], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dx2, dx3, dy1, dy2, dy3, dr1, dr2, dr3, dr4;
	type_union_mlib_d64 dc1, dc2, dc3;
	mlib_s32 num_row;

/* edge masks */
	mlib_s32 emask;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = vis_fzero();

	for (i = 0; i < height; i++) {
		c1 = c2 = c3 = midd[0] = vis_fzero();

/* prepare the source address */
		off = (mlib_addr)psrc1 & 7;
		off >>= 1;
		emask = vis_edge16(psrc1, psrc1 + size_row - 1);
		num_row = size_row + off;
		dpsrc1 = (mlib_d64 *)(psrc1 - off);
		dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

		vis_pst_16(dpsrc1[0], &midd[0], emask);
		dx = midd[0];
		j = num_row >> 2;
		dy = vis_ld_d64_nf(dpsrc2);
		compens = 0;

		if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
			for (; j >= 3; j -= 3) {
				dx2 = dpsrc1[1];
				dy1 = dpsrc2[1];
				dx3 = dpsrc1[2];
				dy2 = dpsrc2[2];
				MUL_S16_S32_3;
				dpsrc1 += 3;
				dpsrc2 += 3;
				dx = vis_ld_d64_nf(dpsrc1);
				dy = vis_ld_d64_nf(dpsrc2);
				compens += 2;
			}
		} else {
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy, dy1);
			for (; j >= 3; j -= 3) {
				dx2 = vis_ld_d64_nf(dpsrc1 + 1);
				dy2 = vis_ld_d64_nf(dpsrc2 + 2);
				dy1 = vis_faligndata(dy1, dy2);
				dx3 = vis_ld_d64_nf(dpsrc1 + 2);
				dy3 = vis_ld_d64_nf(dpsrc2 + 3);
				dy2 = vis_faligndata(dy2, dy3);
				MUL_S16_S32_3;
				dpsrc1 += 3;
				dpsrc2 += 3;
				dx = vis_ld_d64_nf(dpsrc1);
				dy1 = vis_ld_d64_nf(dpsrc2 + 1);
				dy = vis_faligndata(dy3, dy1);
				compens += 2;
			}
		}

		num_row = j * 4 + (num_row & 3);

		if (num_row > 0) {
			mlib_d64 bufy[3];

			dy = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy2 = vis_ld_d64_nf(dpsrc2 + 2);
			dy3 = vis_ld_d64_nf(dpsrc2 + 3);
			bufy[0] = vis_faligndata(dy, dy1);
			bufy[1] = vis_faligndata(dy1, dy2);
			bufy[2] = vis_faligndata(dy2, dy3);
			midd[0] = midd[1] = midd[2] = vis_fzero();
			emask = 0xfff000 >> num_row;
			dx2 = vis_ld_d64_nf(dpsrc1 + 1);
			dx3 = vis_ld_d64_nf(dpsrc1 + 2);
			vis_pst_16(dx, &midd[0], emask >> 8);
			vis_pst_16(dx2, &midd[1], emask >> 4);
			vis_pst_16(dx3, &midd[2], emask);
			for (j = 0; j < 12; j += 3) {
				c1 +=
				    ((mlib_s16 *)midd)[j] *
				    (mlib_d64)((mlib_s16 *)bufy)[j];
				c2 +=
				    ((mlib_s16 *)midd)[j +
				    1] * (mlib_d64)((mlib_s16 *)bufy)[j + 1];
				c3 +=
				    ((mlib_s16 *)midd)[j +
				    2] * (mlib_d64)((mlib_s16 *)bufy)[j + 2];
			}
		}

		rez[al2off[off + 0]] += c1 + (mlib_d64)compens;
		rez[al2off[off + 2]] += c2 + (mlib_d64)compens;
		rez[al2off[off + 1]] += c3 + (mlib_d64)compens;

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}
}

/* *********************************************************** */

#define	MUL_S32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_S32_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0;
	rez[0] = rez[1] = rez[2] = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_S32(psrc1[j], psrc2[j]);
			c2 += MUL_S32(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S32(psrc1[j + 2], psrc2[j + 2]);
			c4 += MUL_S32(psrc1[j + 3], psrc2[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			rez[k] += MUL_S32(psrc1[j], psrc2[j]);
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	rez[0] += c1;
	rez[1] += c2;
	rez[2] += c3;
	rez[3] = c4;
}

/* *********************************************************** */

void
mlib_v_ImageCrossCorrel_S32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub - images and can be treated as a 1 - D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < size_row; j += 3) {
			c1 += MUL_S32(psrc1[j], psrc2[j]);
			c2 += MUL_S32(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S32(psrc1[j + 2], psrc2[j + 2]);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */
