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

#pragma ident	"@(#)mlib_ImageMax_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMax_Fp - get max pixel from two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMax_Fp(mlib_image       *dst,
 *                                   const mlib_image *src1,
 *                                   const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels. They can have 1, 2, 3 or 4 channels.
 *      They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Max two images for each channel:
 *         dst = max(src1, src2)
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMax_Fp = __mlib_ImageMax_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMax_Fp) mlib_ImageMax_Fp
    __attribute__((weak, alias("__mlib_ImageMax_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX(x, y)	(x > y) ? x : y

/* *********************************************************** */

mlib_status
__mlib_ImageMax_Fp(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_type dst_type;
	mlib_s32 dst_chan, dst_width, dst_height;
	mlib_s32 dst_stride, src1_stride, src2_stride;
	mlib_u8 *dst_data, *src1_data, *src2_data;
	mlib_s32 xsize;
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dst_type, dst_chan, dst_width,
	    dst_height, dst_stride, dst_data);
	src1_stride = mlib_ImageGetStride(src1);
	src1_data = mlib_ImageGetData(src1);
	src2_stride = mlib_ImageGetStride(src2);
	src2_data = mlib_ImageGetData(src2);

	xsize = dst_width * dst_chan;

	if (dst_type == MLIB_FLOAT) {
		for (i = 0; i < dst_height; i++) {
			for (j = 0; j < xsize - 3; j += 4) {
				mlib_f32 x0 = ((mlib_f32 *)src1_data)[j];
				mlib_f32 y0 = ((mlib_f32 *)src2_data)[j];
				mlib_f32 x1 = ((mlib_f32 *)src1_data)[j + 1];
				mlib_f32 y1 = ((mlib_f32 *)src2_data)[j + 1];
				mlib_f32 x2 = ((mlib_f32 *)src1_data)[j + 2];
				mlib_f32 y2 = ((mlib_f32 *)src2_data)[j + 2];
				mlib_f32 x3 = ((mlib_f32 *)src1_data)[j + 3];
				mlib_f32 y3 = ((mlib_f32 *)src2_data)[j + 3];

				((mlib_f32 *)dst_data)[j] = MAX(x0, y0);
				((mlib_f32 *)dst_data)[j + 1] = MAX(x1, y1);
				((mlib_f32 *)dst_data)[j + 2] = MAX(x2, y2);
				((mlib_f32 *)dst_data)[j + 3] = MAX(x3, y3);
			}

			for (; j < xsize; j++) {
				mlib_f32 x = ((mlib_f32 *)src1_data)[j];
				mlib_f32 y = ((mlib_f32 *)src2_data)[j];

				((mlib_f32 *)dst_data)[j] = MAX(x, y);
			}

			dst_data += dst_stride;
			src1_data += src1_stride;
			src2_data += src2_stride;
		}

		return (MLIB_SUCCESS);
	} else if (dst_type == MLIB_DOUBLE) {
		for (i = 0; i < dst_height; i++) {
			for (j = 0; j < xsize - 3; j += 4) {
				mlib_d64 x0 = ((mlib_d64 *)src1_data)[j];
				mlib_d64 y0 = ((mlib_d64 *)src2_data)[j];
				mlib_d64 x1 = ((mlib_d64 *)src1_data)[j + 1];
				mlib_d64 y1 = ((mlib_d64 *)src2_data)[j + 1];
				mlib_d64 x2 = ((mlib_d64 *)src1_data)[j + 2];
				mlib_d64 y2 = ((mlib_d64 *)src2_data)[j + 2];
				mlib_d64 x3 = ((mlib_d64 *)src1_data)[j + 3];
				mlib_d64 y3 = ((mlib_d64 *)src2_data)[j + 3];

				((mlib_d64 *)dst_data)[j] = MAX(x0, y0);
				((mlib_d64 *)dst_data)[j + 1] = MAX(x1, y1);
				((mlib_d64 *)dst_data)[j + 2] = MAX(x2, y2);
				((mlib_d64 *)dst_data)[j + 3] = MAX(x3, y3);
			}

			for (; j < xsize; j++) {
				mlib_d64 x = ((mlib_d64 *)src1_data)[j];
				mlib_d64 y = ((mlib_d64 *)src2_data)[j];

				((mlib_d64 *)dst_data)[j] = MAX(x, y);
			}

			dst_data += dst_stride;
			src1_data += src1_stride;
			src2_data += src2_stride;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
