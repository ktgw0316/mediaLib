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

#pragma ident	"@(#)mlib_i_ImageBlend_SA_SAS.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_SAS - blend two images with
 *      SAS source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_SAS(mlib_image       *dst,
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
 *      dst = src1 * ALPHAsrc2 + src2 * (f, f, f, 1)
 *
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_SA_SAS = __mlib_ImageBlend_SA_SAS

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_SA_SAS) mlib_ImageBlend_SA_SAS
    __attribute__((weak, alias("__mlib_ImageBlend_SA_SAS")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN(min, arg)	((min) + (((arg) - (min)) & (((arg) - (min)) >> 31)))


#define	SAT_U8_U8C(dst, src)	dst = (((src << 23) >> 31) | src)
/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_SA_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_s32 src_alpha, dst_alpha;
	mlib_u8  src20, src21, src22;
	mlib_u8 src23, src10, src11, src12, src13;
	mlib_s32 min, alpha2;
	BLEND_VALIDATE;

	if (channels == 3)
		return (__mlib_ImageBlend_SA_ZERO(dst, src1, src2, cmask));

	if (cmask == 1) {

		for (j = 0; j < dst_height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dst_width; i++) {

				src_alpha = psrc2[4 * i + 3];
				dst_alpha = 256 - psrc1[4 * i + 3];
				alpha2 = src_alpha;
				min = MIN(src_alpha, dst_alpha);

				src10 = psrc1[4 * i + 0];
				src20 = psrc2[4 * i + 0];

				SAT_U8_U8C(pdst[4 * i + 0], ((src10 * \
					alpha2 + src20 * min) >> 8));

				src11 = psrc1[4 * i + 1];
				src21 = psrc2[4 * i + 1];

				SAT_U8_U8C(pdst[4 * i + 1], ((src11 * \
					alpha2 + src21 * min) >> 8));

				src12 = psrc1[4 * i + 2];
				src22 = psrc2[4 * i + 2];

				SAT_U8_U8C(pdst[4 * i + 2], ((src12 * \
					alpha2 + src22 * min) >> 8));

				src13 = psrc1[4 * i + 3];
				src23 = psrc2[4 * i + 3];

				SAT_U8_U8C(pdst[4 * i + 3], (((src13 * \
					alpha2) >>8)  + src23));
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

				src_alpha = psrc2[4 * i];
				dst_alpha = 256 - psrc1[4 * i];
				alpha2 = src_alpha;
				min = MIN(src_alpha, dst_alpha);

				src10 = psrc1[4 * i + 0];
				src20 = psrc2[4 * i + 0];

				SAT_U8_U8C(pdst[4 * i + 0], (((src10 * \
					alpha2) >> 8) + src20));

				src11 = psrc1[4 * i + 1];
				src21 = psrc2[4 * i + 1];

				SAT_U8_U8C(pdst[4 * i + 1], ((src11 * \
					alpha2 + src21 * min) >> 8));

				src12 = psrc1[4 * i + 2];
				src22 = psrc2[4 * i + 2];

				SAT_U8_U8C(pdst[4 * i + 2], ((src12 * \
					alpha2 + src22 * min) >> 8));

				src13 = psrc1[4 * i + 3];
				src23 = psrc2[4 * i + 3];

				SAT_U8_U8C(pdst[4 * i + 3], ((src13 * \
					alpha2 + src23 * min) >> 8));
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
			pdst += dst_stride;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
