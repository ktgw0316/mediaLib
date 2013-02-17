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

#pragma ident	"@(#)mlib_c_ImageBlend_ZERO_OMSA.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_OMSA - blend two images with
 *      OMSA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_OMSA(mlib_image       *dst,
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
 *      dst = src2 * (1 - ALPHAsrc2)
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_ZERO_OMSA = __mlib_ImageBlend_ZERO_OMSA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_ZERO_OMSA) mlib_ImageBlend_ZERO_OMSA
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_OMSA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_ZERO_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_u8 a0, a1, a2, a3;
	mlib_s32 help_alpha, color[] = { 0, 0, 0, 0 };
	mlib_f32 a;

	BLEND_VALIDATE_1;

	if (channels == 3)
		return (__mlib_ImageClear(dst, color));

	src2_stride -= dst_width << 2;
	dst_stride -= dst_width << 2;

	if (cmask == 1) {

		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				help_alpha = psrc2[3];
				a = mlib_c_blend_Q8[256 - help_alpha];
				a0 = (mlib_u8)(mlib_U82F32[psrc2[0]] * a);
				a1 = (mlib_u8)(mlib_U82F32[psrc2[1]] * a);
				a2 = (mlib_u8)(mlib_U82F32[psrc2[2]] * a);
				a3 = (mlib_u8)(mlib_U82F32[help_alpha] * a);
				pdst[0] = a0;
				pdst[1] = a1;
				pdst[2] = a2;
				pdst[3] = a3;
				psrc2 += 4;
				pdst += 4;
			}

			psrc2 += src2_stride;
			pdst += dst_stride;
		}
	} else {
		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				help_alpha = psrc2[0];
				a = mlib_c_blend_Q8[256 - help_alpha];
				a0 = (mlib_u8)(mlib_U82F32[help_alpha] * a);
				a1 = (mlib_u8)(mlib_U82F32[psrc2[1]] * a);
				a2 = (mlib_u8)(mlib_U82F32[psrc2[2]] * a);
				a3 = (mlib_u8)(mlib_U82F32[psrc2[3]] * a);
				pdst[0] = a0;
				pdst[1] = a1;
				pdst[2] = a2;
				pdst[3] = a3;
				psrc2 += 4;
				pdst += 4;
			}

			psrc2 += src2_stride;
			pdst += dst_stride;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
