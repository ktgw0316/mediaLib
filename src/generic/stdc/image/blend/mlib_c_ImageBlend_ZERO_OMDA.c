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

#pragma ident	"@(#)mlib_c_ImageBlend_ZERO_OMDA.c	9.2	07/10/09 SMI"

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
#include <mlib_c_ImageBlendTable.h>

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
	for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < dst_width; j++) {
			mlib_f32 omda =
			    1 - mlib_c_blend_Q8[psrc1[i * src1_stride + 4 * j]];

			pdst[i * dst_stride + 4 * j] =
			    (mlib_u8)(omda * mlib_U82F32[psrc2[i * src2_stride +
			    4 * j]]);
			pdst[i * dst_stride + 4 * j + 1] =
			    (mlib_u8)(omda * mlib_U82F32[psrc2[i * src2_stride +
			    4 * j + 1]]);
			pdst[i * dst_stride + 4 * j + 2] =
			    (mlib_u8)(omda * mlib_U82F32[psrc2[i * src2_stride +
			    4 * j + 2]]);
			pdst[i * dst_stride + 4 * j + 3] =
			    (mlib_u8)(omda * mlib_U82F32[psrc2[i * src2_stride +
			    4 * j + 3]]);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
