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

#pragma ident	"@(#)mlib_v_ImageBlend_ZERO_OMDA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_OMDA - blend two images with
 *      OMDA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_OMDA(mlib_image       *dst,
 *                                            const mlib_image *src1,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask);
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
 *      dst = src2 * (1 - ALPHAsrc1)
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <vis_proto.h>
#include <mlib_v_ImageBlendTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_ZERO_OMDA = __mlib_ImageBlend_ZERO_OMDA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_ZERO_OMDA) mlib_ImageBlend_ZERO_OMDA
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_OMDA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_ZERO_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_d64 *dpsrc2;
	mlib_d64 done = vis_to_double_dup(0x10001000);
	mlib_d64 dsrc2, tmp0, tmp1, dres, dres1;
	mlib_u8 *psrc1_row, *psrc2_row, *pdst_row, *buf;
	mlib_s32 shag;

	BLEND_VALIDATE;

	if (channels == 3) {
/*  ALPHAsrc1 = 1  ==>  dst = src2 * (1 - ALPHAsrc1) = 0 */
		mlib_s32 color[] = { 0, 0, 0, 0 };

		return (__mlib_ImageClear(dst, color));
	}

/* channels == 4 */

	if (cmask == 1)
		psrc1 += 3;

/*  dst = src2 * (1 - ALPHAsrc1) */

	vis_write_gsr(3 << 3);

/* unaligned dst  */
	if (((mlib_addr)pdst | dst_stride) & 3) {
		buf = __mlib_malloc(dst_width * 4);

		if (buf == NULL)
			return (MLIB_FAILURE);
		shag = 0;
	} else {
		buf = pdst;
		shag = dst_stride;
	}

	for (i = 0; i < dst_height; i++) {
		psrc1_row = psrc1 + i * src1_stride;
		psrc2_row = psrc2 + i * src2_stride;
		pdst_row = buf + i * shag;

		if (j = ((mlib_addr)pdst_row >> 2) & 1) {
			dpsrc2 = vis_alignaddr(psrc2_row, 0);
			dsrc2 = vis_faligndata(
				vis_ld_d64_nf(dpsrc2),
				vis_ld_d64_nf(dpsrc2 + 1));
			dres = vis_ld_u8(psrc1_row);
			dres =
			    vis_fpsub16(done, vis_fexpand(vis_read_lo(dres)));
			dres =
			    vis_fmul8x16al(vis_read_hi(dsrc2),
			    vis_read_lo(dres));
			((mlib_f32 *)pdst_row)[0] = vis_fpack16(dres);
			psrc1_row += 4;
			psrc2_row += 4;
			pdst_row += 4;
		}

		dpsrc2 = vis_alignaddr(psrc2_row, 0);
		tmp1 = vis_ld_d64_nf(dpsrc2);

		if (((mlib_addr)psrc2_row & 7) == 0) {
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				dres = vis_ld_u8(psrc1_row);
				dres =
				    vis_fpsub16(done,
				    vis_fexpand(vis_read_lo(dres)));
				dres =
				    vis_fmul8x16al(vis_read_hi(tmp1),
				    vis_read_lo(dres));
				dres1 = vis_ld_u8(psrc1_row + 4);
				dres1 =
				    vis_fpsub16(done,
				    vis_fexpand(vis_read_lo(dres1)));
				dres1 =
				    vis_fmul8x16al(vis_read_lo(tmp1),
				    vis_read_lo(dres1));
				tmp1 = vis_ld_d64_nf(dpsrc2 + 1);
				((mlib_d64 *)pdst_row)[0] =
				    vis_fpack16_pair(dres, dres1);
				psrc1_row += 8;
				dpsrc2++;
				pdst_row += 8;
			}
		} else {
#pragma pipeloop(0)
			for (; j <= (dst_width - 2); j += 2) {
				tmp0 = tmp1;
				tmp1 = dpsrc2[1];
				dsrc2 = vis_faligndata(tmp0, tmp1);
				dres = vis_ld_u8(psrc1_row);
				dres =
				    vis_fpsub16(done,
				    vis_fexpand(vis_read_lo(dres)));
				dres =
				    vis_fmul8x16al(vis_read_hi(dsrc2),
				    vis_read_lo(dres));
				dres1 = vis_ld_u8(psrc1_row + 4);
				dres1 =
				    vis_fpsub16(done,
				    vis_fexpand(vis_read_lo(dres1)));
				dres1 =
				    vis_fmul8x16al(vis_read_lo(dsrc2),
				    vis_read_lo(dres1));
				((mlib_d64 *)pdst_row)[0] =
				    vis_fpack16_pair(dres, dres1);
				psrc1_row += 8;
				dpsrc2++;
				pdst_row += 8;
			}
		}

		if (j < dst_width) {
			dsrc2 = vis_faligndata(tmp1,
				vis_ld_d64_nf(dpsrc2 + 1));
			dres = vis_ld_u8(psrc1_row);
			dres =
			    vis_fpsub16(done, vis_fexpand(vis_read_lo(dres)));
			dres =
			    vis_fmul8x16al(vis_read_hi(dsrc2),
			    vis_read_lo(dres));
			((mlib_f32 *)pdst_row)[0] = vis_fpack16(dres);
		}

		if (shag == 0)
			mlib_ImageCopy_na(buf, pdst + i * dst_stride,
			    dst_width * 4);
	}

	if (shag == 0)
		__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
