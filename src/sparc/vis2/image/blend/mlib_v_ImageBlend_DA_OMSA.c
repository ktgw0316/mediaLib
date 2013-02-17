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

#pragma ident	"@(#)mlib_v_ImageBlend_DA_OMSA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_OMSA - blend two images with
 *      OMSA source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend_DA_OMSA(mlib_image       *dst,
 *                                          const mlib_image *src1,
 *                                          const mlib_image *src2,
 *                                          mlib_s32         cmask);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1, src2 and dst must be the same type,
 *      the same size and the same number of channels.
 *      They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 * DESCRIPTION
 *      dst = src1 * ALPHAsrc1 + src2 * (1 - ALPHAsrc2)
 *
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <mlib_v_ImageBlendTable.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_DA_OMSA = __mlib_ImageBlend_DA_OMSA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_DA_OMSA) mlib_ImageBlend_DA_OMSA
    __attribute__((weak, alias("__mlib_ImageBlend_DA_OMSA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_DA_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_d64 alpha1, alpha2, alpha3, alpha4;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2, sum1, sum2, dpix1;
	mlib_d64 res1, res2, res3, res4, res;
	mlib_s32 width, mask, off = 0;
	mlib_u8 *psrc1buf, *psrc2buf, *pdstbuf;
	mlib_d64 *mlib_v_blend_alpha64 = (mlib_d64 *)mlib_table_blend_alpha_s32;
	mlib_u64 mask1, idx1, u64one;

	BLEND_VALIDATE;

	if (channels == 3)
		return (__mlib_ImageBlend_ONE_ZERO(dst, src1, src2, cmask));

	BUFFER_SIZE;

	width = ((dst_width + 1) >> 1) * sizeof (mlib_d64);
	psrc1buf = (mlib_u8 *)__mlib_malloc(3 * width);

	if (psrc1buf == NULL)
		return (MLIB_FAILURE);

	psrc2buf = psrc1buf + width;
	pdstbuf = psrc2buf + width;

	vis_write_gsr(3 << 3);

	mask1 = 0x000000ff000000ff;
	u64one = 0x0000010000000100;

	if (cmask == 1) {
		off = 3;
		mask = 0x33bb77ff;
	} else
		mask = 0x008844cc;

	vis_write_bmask(mask, 0);

	for (j = 0; j < dst_height; j++) {

		dpsrc1 = (mlib_d64 *)psrc1;
		dpsrc2 = (mlib_d64 *)psrc2;
		dpdst = (mlib_d64 *)pdst;

		if (((mlib_addr)psrc1 & 7) != 0) {

			mlib_ImageCopy_na(psrc1, psrc1buf, dst_width * 4);
			dpsrc1 = (mlib_d64 *)psrc1buf;
		}

		if (((mlib_addr)psrc2 & 7) != 0) {

			mlib_ImageCopy_na(psrc2, psrc2buf, dst_width * 4);
			dpsrc2 = (mlib_d64 *)psrc2buf;
		}

		if (((mlib_addr)pdst & 7) != 0) {
			dpdst = (mlib_d64 *)pdstbuf;
		}
#pragma pipeloop(0)
		for (i = 0; i < (dst_width - 1); i += 2) {

			pix1 = dpsrc1[0];
			pix2 = dpsrc2[0];

			idx1 = (*(mlib_u64 *)dpsrc2 >> (24 - 8 * off)) & mask1;
			idx1 = (u64one - idx1) << 3;

			dpix1 = vis_bshuffle(pix1, pix1);

			alpha1 = vis_fexpand(vis_read_hi(dpix1));
			alpha2 =
			    *(mlib_d64 *)((mlib_u8 *)mlib_v_blend_alpha64 +
			    (idx1 >> 32));
			alpha3 = vis_fexpand(vis_read_lo(dpix1));
			alpha4 =
			    *(mlib_d64 *)((mlib_u8 *)mlib_v_blend_alpha64 +
			    (idx1 & 0xff8));

			res1 =
			    vis_fmul8x16al(vis_read_hi(pix1),
			    vis_read_lo(alpha1));
			res2 = vis_fmul8x16(vis_read_hi(pix2), alpha2);
			sum1 = vis_fpadd16(res1, res2);

			res3 =
			    vis_fmul8x16al(vis_read_lo(pix1),
			    vis_read_lo(alpha3));
			res4 = vis_fmul8x16(vis_read_lo(pix2), alpha4);
			sum2 = vis_fpadd16(res3, res4);

			res = vis_fpack16_pair(sum1, sum2);
			dpdst[0] = res;

			dpdst = (void *)((mlib_u8 *)dpdst + 8);
			dpsrc1 = (void *)((mlib_u8 *)dpsrc1 + 8);
			dpsrc2 = (void *)((mlib_u8 *)dpsrc2 + 8);
		}

		for (; i < dst_width; i++) {

			alpha1 =
			    vis_fexpand(vis_read_lo(vis_ld_u8((mlib_u8 *)dpsrc1
			    + off)));
			alpha2 =
			    mlib_v_blend_alpha64[256 -
			    ((mlib_u8 *)dpsrc2)[off]];
			res1 =
			    vis_fmul8x16al(*(mlib_f32 *)dpsrc1,
			    vis_read_lo(alpha1));
			res2 = vis_fmul8x16(*(mlib_f32 *)dpsrc2, alpha2);
			sum1 = vis_fpadd16(res1, res2);

			*(mlib_f32 *)dpdst = vis_fpack16(sum1);
		}

		if (((mlib_addr)pdst & 7) != 0) {
			mlib_ImageCopy_na(pdstbuf, pdst, dst_width * 4);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
		pdst += dst_stride;
	}

	__mlib_free(psrc1buf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
