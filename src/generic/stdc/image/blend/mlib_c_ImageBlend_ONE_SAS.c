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

#pragma ident	"@(#)mlib_c_ImageBlend_ONE_SAS.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_SAS - blend two images with
 *      SAS source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_SAS(mlib_image       *dst,
 *                                          const mlib_image *src1,
 *                                          const mlib_image *src2,
 *                                          mlib_s32         cmask);
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
 *      dst = src1 + src2 * (f, f, f, 1), f = min(1 - ALPHAsrc1, ALPHAsrc2)
 *
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_ONE_SAS = __mlib_ImageBlend_ONE_SAS

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_ONE_SAS) mlib_ImageBlend_ONE_SAS
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_SAS")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN(min, arg)	((min) + (((arg) - (min)) & (((arg) - (min)) >> 31)))

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_ONE_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_s16 src1_alpha, src2_alpha, alpha_256;
	mlib_f32 min, f_0, f_1, f_2, f_3;

	BLEND_VALIDATE;

	if (channels == 3)
		return (__mlib_ImageCopy(dst, src1));

	src1_stride -= dst_width << 2;
	src2_stride -= dst_width << 2;
	dst_stride -= dst_width << 2;

	if (cmask == 1) {

		for (j = 0; j < dst_height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dst_width; i++) {

				src1_alpha = psrc1[3];
				src2_alpha = psrc2[3];
				alpha_256 = 256 - src1_alpha;
				min =
				    mlib_c_blend_Q8[MIN(src2_alpha, alpha_256)];

				f_0 = mlib_c_blend_u8_sat[psrc1[0]];
				f_1 = mlib_c_blend_u8_sat[psrc1[1]];
				f_2 = mlib_c_blend_u8_sat[psrc1[2]];
				f_3 = mlib_c_blend_u8_sat[src1_alpha];

				f_0 += min * mlib_c_blend_u8_sat[psrc2[0]];
				f_1 += min * mlib_c_blend_u8_sat[psrc2[1]];
				f_2 += min * mlib_c_blend_u8_sat[psrc2[2]];
				f_3 += mlib_c_blend_u8_sat[src2_alpha];

				pdst[0] = FLOAT2INT_CLAMP(f_0) >> 23;
				pdst[1] = FLOAT2INT_CLAMP(f_1) >> 23;
				pdst[2] = FLOAT2INT_CLAMP(f_2) >> 23;
				pdst[3] = FLOAT2INT_CLAMP(f_3) >> 23;

				psrc1 += 4;
				psrc2 += 4;
				pdst += 4;
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
			pdst += dst_stride;
		}
	} else {
		for (j = 0; j < dst_height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dst_width; i++) {

				src1_alpha = psrc1[0];
				src2_alpha = psrc2[0];
				alpha_256 = 256 - src1_alpha;
				min =
				    mlib_c_blend_Q8[MIN(src2_alpha, alpha_256)];

				f_0 = mlib_c_blend_u8_sat[src1_alpha];
				f_1 = mlib_c_blend_u8_sat[psrc1[1]];
				f_2 = mlib_c_blend_u8_sat[psrc1[2]];
				f_3 = mlib_c_blend_u8_sat[psrc1[3]];

				f_0 += mlib_c_blend_u8_sat[src2_alpha];
				f_1 += min * mlib_c_blend_u8_sat[psrc2[1]];
				f_2 += min * mlib_c_blend_u8_sat[psrc2[2]];
				f_3 += min * mlib_c_blend_u8_sat[psrc2[3]];

				pdst[0] = FLOAT2INT_CLAMP(f_0) >> 23;
				pdst[1] = FLOAT2INT_CLAMP(f_1) >> 23;
				pdst[2] = FLOAT2INT_CLAMP(f_2) >> 23;
				pdst[3] = FLOAT2INT_CLAMP(f_3) >> 23;

				psrc1 += 4;
				psrc2 += 4;
				pdst += 4;
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
			pdst += dst_stride;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
