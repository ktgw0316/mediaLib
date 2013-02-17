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

#pragma ident	"@(#)mlib_v_ImageBlend_OMSC_ONE.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_ONE - blend two images with
 *      ONE source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_ONE(mlib_image       *dst,
 *                                           const mlib_image *src1,
 *                                           const mlib_image *src2,
 *                                           mlib_s32         cmask);
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
 *      dst = src1 + src2 * (1 - src1)
 *
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <vis_proto.h>
#include <mlib_ImageBlendUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_OMSC_ONE = __mlib_ImageBlend_OMSC_ONE

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_OMSC_ONE) mlib_ImageBlend_OMSC_ONE
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_ONE")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	IMAGE_OMSC_ONE                                          \
	pix1hi = vis_fexpand(vis_read_hi(pix1));                \
	sum1 = vis_fpsub16(d256, pix1hi);                       \
	res1 = vis_fmul8x16(vis_read_hi(pix2), sum1);           \
	res1 = vis_fpadd16(res1, pix1hi);                       \
	pix1lo = vis_fmul8x16(vis_read_lo(pix1), d256);         \
	sum2 = vis_fpsub16(d256, pix1lo);                       \
	res2 = vis_fmul8x16(vis_read_lo(pix2), sum2);           \
	res2 = vis_fpadd16(res2, pix1lo);                       \
	res = vis_fpack16_pair(res1, res2)

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_OMSC_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 d256 = vis_to_double_dup(0x10001000);
	mlib_d64 tmp0, tmp1, tmp2, tmp3;
	mlib_d64 pix1, pix2, sum1, sum2;
	mlib_d64 res1, res2, res;
	mlib_d64 pix1hi, pix1lo;
	mlib_s32 mask, off, len;
	mlib_u8 *pdstend, *pdstend1;

	BLEND_VALIDATE;

	dst_width *= channels;
	vis_write_gsr(3 << 3);

	for (j = 0; j < dst_height; j++) {

		pdstend = pdst + dst_width - 1;
		pdstend1 = pdst + dst_width;

		dpdst = (mlib_d64 *)((mlib_addr)pdst & ~7);
		off = (mlib_addr)dpdst - (mlib_addr)pdst;

		dpsrc1 = (mlib_d64 *)vis_alignaddr(psrc1, off);
		dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, off);

		if (((((mlib_addr)pdst ^ (mlib_addr)psrc1) |
		    ((mlib_addr)pdst ^ (mlib_addr)psrc2)) & 7) == 0) {
/* dst, src1, src2 the some alignment */

			if (off) {

				mask = vis_edge8(pdst, pdstend);

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				IMAGE_OMSC_ONE;

				vis_pst_8(res, dpdst++, mask);
			}

			len = (mlib_addr)pdstend1 - (mlib_addr)dpdst;

#pragma pipeloop(0)
			for (i = 0; i < (len >> 3); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				IMAGE_OMSC_ONE;

				(*dpdst++) = res;
			}

			if ((mlib_addr)dpdst <= (mlib_addr)pdstend) {

				mask = vis_edge8(dpdst, pdstend);

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				IMAGE_OMSC_ONE;

				vis_pst_8(res, dpdst, mask);
			}
		} else {
			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
/* src1, src2 the some alignment */

				if (off) {

					mask = vis_edge8(pdst, pdstend);

					tmp0 = vis_ld_d64_nf(dpsrc1);
					dpsrc1++;
					tmp1 = vis_ld_d64_nf(dpsrc1);
					pix1 = vis_faligndata(tmp0, tmp1);

					tmp2 = vis_ld_d64_nf(dpsrc2);
					dpsrc2++;
					tmp3 = vis_ld_d64_nf(dpsrc2);
					pix2 = vis_faligndata(tmp2, tmp3);

					IMAGE_OMSC_ONE;

					vis_pst_8(res, dpdst++, mask);
				}

				len = (mlib_addr)pdstend1 - (mlib_addr)dpdst;

#pragma pipeloop(0)
				for (i = 0; i < (len >> 3); i++) {

					tmp0 = (*dpsrc1++);
					tmp1 = *dpsrc1;
					pix1 = vis_faligndata(tmp0, tmp1);

					tmp2 = (*dpsrc2++);
					tmp3 = *dpsrc2;
					pix2 = vis_faligndata(tmp2, tmp3);

					IMAGE_OMSC_ONE;

					(*dpdst++) = res;
				}

				if ((mlib_addr)dpdst <= (mlib_addr)pdstend) {

					mask = vis_edge8(dpdst, pdstend);

					tmp0 = vis_ld_d64_nf(dpsrc1);
					dpsrc1++;
					tmp1 = vis_ld_d64_nf(dpsrc1);
					pix1 = vis_faligndata(tmp0, tmp1);

					tmp2 = vis_ld_d64_nf(dpsrc2);
					dpsrc2++;
					tmp3 = vis_ld_d64_nf(dpsrc2);
					pix2 = vis_faligndata(tmp2, tmp3);

					IMAGE_OMSC_ONE;

					vis_pst_8(res, dpdst, mask);
				}
			} else {
				if ((((mlib_addr)pdst ^ (mlib_addr)psrc1) & 7)
				    == 0) {
/* dst, src1 the some alignment */

					if (off) {

						mask = vis_edge8(pdst, pdstend);

						pix1 = (*dpsrc1++);

						tmp2 =
							vis_ld_d64_nf
							(dpsrc2);
						dpsrc2++;
						tmp3 =
							vis_ld_d64_nf
							(dpsrc2);
						pix2 =
						    vis_faligndata(tmp2, tmp3);

						IMAGE_OMSC_ONE;

						vis_pst_8(res, dpdst++, mask);
					}

					len =
					    (mlib_addr)pdstend1 -
					    (mlib_addr)dpdst;

#pragma pipeloop(0)
					for (i = 0; i < (len >> 3); i++) {

						pix1 = (*dpsrc1++);

						tmp2 = (*dpsrc2++);
						tmp3 = *dpsrc2;
						pix2 =
						    vis_faligndata(tmp2, tmp3);

						IMAGE_OMSC_ONE;

						(*dpdst++) = res;
					}

					if ((mlib_addr)dpdst <=
					    (mlib_addr)pdstend) {

						mask =
						    vis_edge8(dpdst, pdstend);

						pix1 = *dpsrc1;

						tmp2 =
							vis_ld_d64_nf
							(dpsrc2);
						dpsrc2++;
						tmp3 =
							vis_ld_d64_nf
							(dpsrc2);
						pix2 =
						    vis_faligndata(tmp2, tmp3);

						IMAGE_OMSC_ONE;

						vis_pst_8(res, dpdst, mask);
					}
				} else {
					if ((((mlib_addr)pdst ^ (mlib_addr)
					    psrc2) & 7) == 0) {
/* dst, src2 the some alignment */

						vis_alignaddr(psrc1, off);

						if (off) {

							mask =
							    vis_edge8(pdst,
							    pdstend);

							tmp0 =
								vis_ld_d64_nf
								(dpsrc1);
							dpsrc1++;
							tmp1 =
								vis_ld_d64_nf
								(dpsrc1);
							pix1 =
							    vis_faligndata(tmp0,
							    tmp1);

							pix2 = (*dpsrc2++);

							IMAGE_OMSC_ONE;

							vis_pst_8(res, dpdst++,
							    mask);
						}

						len =
						    (mlib_addr)pdstend1 -
						    (mlib_addr)dpdst;

#pragma pipeloop(0)
						for (i = 0; i <
						    (len >> 3); i++) {

							tmp0 = (*dpsrc1++);
							tmp1 = *dpsrc1;
							pix1 =
							    vis_faligndata(tmp0,
							    tmp1);

							pix2 = (*dpsrc2++);

							IMAGE_OMSC_ONE;

							(*dpdst++) = res;
						}

						if ((mlib_addr)dpdst <=
						    (mlib_addr)pdstend) {

							mask =
							    vis_edge8(dpdst,
							    pdstend);

							tmp0 =
								vis_ld_d64_nf
								(dpsrc1);
							dpsrc1++;
							tmp1 =
								vis_ld_d64_nf
								(dpsrc1);
							pix1 =
							    vis_faligndata(tmp0,
							    tmp1);

							pix2 = *dpsrc2;

							IMAGE_OMSC_ONE;

							vis_pst_8(res, dpdst,
							    mask);
						}
					} else {
/* not alignment */

						if (off) {

							mask =
							    vis_edge8(pdst,
							    pdstend);

							vis_alignaddr(psrc1,
							    off);
							tmp0 =
								vis_ld_d64_nf
								(dpsrc1);
							dpsrc1++;
							tmp1 =
								vis_ld_d64_nf
								(dpsrc1);
							pix1 =
							    vis_faligndata(tmp0,
							    tmp1);

							vis_alignaddr(psrc2,
							    off);
							tmp2 =
								vis_ld_d64_nf
								(dpsrc2);
							dpsrc2++;
							tmp3 =
								vis_ld_d64_nf
								(dpsrc2);
							pix2 =
							    vis_faligndata(tmp2,
							    tmp3);

							IMAGE_OMSC_ONE;

							vis_pst_8(res, dpdst++,
							    mask);
						}

						len =
						    (mlib_addr)pdstend1 -
						    (mlib_addr)dpdst;

#pragma pipeloop(0)
						for (i = 0; i <
						    (len >> 3); i++) {

							vis_alignaddr(psrc1,
							    off);
							tmp0 = (*dpsrc1++);
							tmp1 = *dpsrc1;
							pix1 =
							    vis_faligndata(tmp0,
							    tmp1);

							vis_alignaddr(psrc2,
							    off);
							tmp2 = (*dpsrc2++);
							tmp3 = *dpsrc2;
							pix2 =
							    vis_faligndata(tmp2,
							    tmp3);

							IMAGE_OMSC_ONE;

							(*dpdst++) = res;
						}

						if ((mlib_addr)dpdst <=
						    (mlib_addr)pdstend) {

							mask =
							    vis_edge8(dpdst,
							    pdstend);

							vis_alignaddr(psrc1,
							    off);
							tmp0 =
								vis_ld_d64_nf
								(dpsrc1);
							dpsrc1++;
							tmp1 =
								vis_ld_d64_nf
								(dpsrc1);
							pix1 =
							    vis_faligndata(tmp0,
							    tmp1);

							vis_alignaddr(psrc2,
							    off);
							tmp2 =
								vis_ld_d64_nf
								(dpsrc2);
							dpsrc2++;
							tmp3 =
								vis_ld_d64_nf
								(dpsrc2);
							pix2 =
							    vis_faligndata(tmp2,
							    tmp3);

							IMAGE_OMSC_ONE;

							vis_pst_8(res, dpdst,
							    mask);
						}
					}
				}
			}
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
		pdst += dst_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
