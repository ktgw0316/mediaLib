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

#pragma ident	"@(#)mlib_v_ImageBlend_ONE_SA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_SA - blend two images with
 *      SA source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_SA(mlib_image       *dst,
 *                                         const mlib_image *src1,
 *                                         const mlib_image *src2,
 *                                         mlib_s32         cmask);
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
 *      dst = src1 + src2 * ALPHAsrc2
 *
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <mlib_v_ImageBlendTable.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_ONE_SA = __mlib_ImageBlend_ONE_SA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_ONE_SA) mlib_ImageBlend_ONE_SA
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_SA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BLEND_ONE_SA                                                     \
	alpha1 = vis_ld_u8(psrc2 + 8 * i + off);                         \
	alpha1 = vis_fexpand(vis_read_lo(alpha1));                       \
	res1 = vis_fmul8x16al(vis_read_hi(pix2), vis_read_lo(alpha1));   \
	res1 = vis_fpadd16(vis_fexpand(vis_read_hi(pix1)), res1);        \
	alpha2 = vis_ld_u8(psrc2 + 8 * i + 4 + off);                     \
	alpha2 = vis_fmul8x16al(vis_read_lo(alpha2), fone);              \
	res2 = vis_fmul8x16al(vis_read_lo(pix2), vis_read_lo(alpha2));   \
	res2 = vis_fpadd16(vis_fexpand(vis_read_lo(pix1)), res2)

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_ONE_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha2, pix1, pix2, res1, res2;
	mlib_f32 fone = vis_to_float(0x1000);
	mlib_s32 width, off = (cmask == 1) ? 3 : 0;
	mlib_u8 *psrc1buf, *psrc2buf, *pdstbuf;

	BLEND_VALIDATE;

	if (channels == 3)
		return (__mlib_ImageBlend_ONE_ONE(dst, src1, src2, cmask));

	vis_write_gsr(3 << 3);

	BUFFER_SIZE;

	width = ((dst_width + 1) >> 1) * sizeof (mlib_d64);
	psrc1buf = (mlib_u8 *)__mlib_malloc(width * 3);
	psrc2buf = psrc1buf + width;
	pdstbuf = psrc2buf + width;

	if (psrc1buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < dst_height; j++) {
		if ((mlib_addr)psrc1 & 7) {
			mlib_ImageCopy_na(psrc1, psrc1buf, dst_width * 4);
			dpsrc1 = (mlib_d64 *)psrc1buf;
		} else
			dpsrc1 = (mlib_d64 *)psrc1;

		if ((mlib_addr)psrc2 & 7) {
			mlib_ImageCopy_na(psrc2, psrc2buf, dst_width * 4);
			dpsrc2 = (mlib_d64 *)psrc2buf;
		} else
			dpsrc2 = (mlib_d64 *)psrc2;
		dpdst = (mlib_d64 *)(((mlib_addr)pdst & 7) ? pdstbuf : pdst);

#pragma pipeloop(0)
		for (i = 0; i < (dst_width >> 1); i++) {
			pix1 = (*dpsrc1++);
			pix2 = (*dpsrc2++);
			BLEND_ONE_SA;
			(*dpdst++) = vis_fpack16_pair(res1, res2);
		}

		if (dst_width & 1) {
			alpha1 = vis_ld_u8(psrc2 + 8 * i + off);
			alpha1 = vis_fexpand(vis_read_lo(alpha1));
			res1 =
			    vis_fmul8x16al(*((mlib_f32 *)dpsrc2),
			    vis_read_lo(alpha1));
			res1 =
			    vis_fpadd16(vis_fexpand(*((mlib_f32 *)dpsrc1)),
			    res1);
			*((mlib_f32 *)dpdst) = vis_fpack16(res1);
		}

		if ((mlib_addr)pdst & 7) {
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
