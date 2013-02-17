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

#pragma ident	"@(#)mlib_v_ImageScalarBlend.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageScalarBlend - blend two images with a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScalarBlend(mlib_image       *dst,
 *                                        const mlib_image *src1,
 *                                        const mlib_image *src2,
 *                                        const mlib_s32   *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to array of alpha coefficients
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      alpha coefficients are in Q31 format and must be positive.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (0x7fffffff-alpha)*src2,
 *      (0 <= alpha <= 0x7fffffff)
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageScalarBlendFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScalarBlend = __mlib_ImageScalarBlend

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScalarBlend) mlib_ImageScalarBlend
    __attribute__((weak, alias("__mlib_ImageScalarBlend")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageScalarBlend(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_s32 *alpha)
{
	mlib_s32 slb1, slb2, dlb, xsize, ysize, dsize, nchan,
	    flag1, flag2, flagd, i;
	mlib_type dtype;
	void *sa1, *sa2, *da;
	mlib_d64 ad;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	if (alpha == NULL)
		return (MLIB_FAILURE);
	for (i = 0; i < mlib_ImageGetChannels(dst); i++) {
		if (alpha[i] < 0) {
			return (MLIB_OUTOFRANGE);
		}
	}

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);
	slb1 = mlib_ImageGetStride(src1);
	sa1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa2 = mlib_ImageGetData(src2);
	flag1 = mlib_ImageGetFlags(src1);
	flag2 = mlib_ImageGetFlags(src2);
	flagd = mlib_ImageGetFlags(dst);

	dsize = xsize * ysize;

	if (dtype != MLIB_INT) {
		xsize *= nchan;
		dsize *= nchan;
		switch (nchan) {
		case 1:
			ad = vis_to_double_dup(((alpha[0] & ~0xffff) |
			    ((mlib_u32)alpha[0] >> 16)) & 0x7fff7fff);
			break;

		case 2:
			ad = vis_to_double_dup(((alpha[0] & ~0xffff) |
			    ((mlib_u32)alpha[1] >> 16)) & 0x7fff7fff);
			break;

		case 3:
			ad = vis_to_double(((alpha[0] & ~0xffff) | ((mlib_u32)
			    alpha[1] >> 16)) & 0x7fff7fff,
			    ((alpha[2] & ~0xffff) | ((mlib_u32)alpha[0] >> 16))
			    & 0x7fff7fff);
			break;

		case 4:
			ad = vis_to_double(((alpha[0] & ~0xffff) | ((mlib_u32)
			    alpha[1] >> 16)) & 0x7fff7fff,
			    ((alpha[2] & ~0xffff) | ((mlib_u32)alpha[3] >> 16))
			    & 0x7fff7fff);
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	if (dtype == MLIB_BYTE) {

/* initialize GSR scale factor */
		vis_write_gsr(0);

/* 3 channels */
		if (nchan == 3) {
			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flagd & A8D1) == 0)) {

				mlib_v_ImageScalarBlend_U8_3_A8D1((mlib_u8 *)
				    sa1, (mlib_u8 *)sa2, (mlib_u8 *)da, dsize,
				    ad);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageScalarBlend_U8_3_A8D2((mlib_u8 *)
				    sa1, slb1, (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb, xsize, ysize, ad);
			} else {

				mlib_v_ImageScalarBlend_U8_3((mlib_u8 *)sa1,
				    slb1, (mlib_u8 *)sa2, slb2, (mlib_u8 *)da,
				    dlb, xsize, ysize, ad);
			}
		} else {
/* 1, 2 or 4 channels */
			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 7) == 0)) {

				mlib_v_ImageScalarBlend_U8_124_A8D1X8((mlib_u8
				    *)sa1, (mlib_u8 *)sa2, (mlib_u8 *)da, dsize,
				    ad);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageScalarBlend_U8_124_A8D2((mlib_u8 *)
				    sa1, slb1, (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb, xsize, ysize, ad);
			} else {

				mlib_v_ImageScalarBlend_U8_124((mlib_u8 *)sa1,
				    slb1, (mlib_u8 *)sa2, slb2, (mlib_u8 *)da,
				    dlb, xsize, ysize, ad);
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flagd & A8D1) == 0)) {

				mlib_v_ImageScalarBlend_S16_3_A8D1((mlib_s16 *)
				    sa1, (mlib_s16 *)sa2, (mlib_s16 *)da, dsize,
				    ad);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageScalarBlend_S16_3_A8D2((mlib_s16 *)
				    sa1, slb1, (mlib_s16 *)sa2, slb2,
				    (mlib_s16 *)da, dlb, xsize, ysize, ad);
			} else {

				mlib_v_ImageScalarBlend_S16_3((mlib_s16 *)sa1,
				    slb1, (mlib_s16 *)sa2, slb2, (mlib_s16 *)da,
				    dlb, xsize, ysize, ad);
			}
		} else {
/* 1, 2 or 4 channels */
			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 3) == 0)) {

				mlib_v_ImageScalarBlend_S16_124_A8D1X4((mlib_s16
				    *)sa1, (mlib_s16 *)sa2, (mlib_s16 *)da,
				    dsize, ad);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageScalarBlend_S16_124_A8D2((mlib_s16
				    *)sa1, slb1, (mlib_s16 *)sa2, slb2,
				    (mlib_s16 *)da, dlb, xsize, ysize, ad);
			} else {

				mlib_v_ImageScalarBlend_S16_124((mlib_s16 *)sa1,
				    slb1, (mlib_s16 *)sa2, slb2, (mlib_s16 *)da,
				    dlb, xsize, ysize, ad);
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flagd & A8D1) == 0)) {

				mlib_v_ImageScalarBlend_U16_3_A8D1((mlib_u16 *)
				    sa1, (mlib_u16 *)sa2, (mlib_u16 *)da, dsize,
				    ad);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageScalarBlend_U16_3_A8D2((mlib_u16 *)
				    sa1, slb1, (mlib_u16 *)sa2, slb2,
				    (mlib_u16 *)da, dlb, xsize, ysize, ad);
			} else {

				mlib_v_ImageScalarBlend_U16_3((mlib_u16 *)sa1,
				    slb1, (mlib_u16 *)sa2, slb2, (mlib_u16 *)da,
				    dlb, xsize, ysize, ad);
			}
		} else {
/* 1, 2 or 4 channels */
			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 3) == 0)) {

				mlib_v_ImageScalarBlend_U16_124_A8D1X4((mlib_u16
				    *)sa1, (mlib_u16 *)sa2, (mlib_u16 *)da,
				    dsize, ad);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageScalarBlend_U16_124_A8D2((mlib_u16
				    *)sa1, slb1, (mlib_u16 *)sa2, slb2,
				    (mlib_u16 *)da, dlb, xsize, ysize, ad);
			} else {

				mlib_v_ImageScalarBlend_U16_124((mlib_u16 *)sa1,
				    slb1, (mlib_u16 *)sa2, slb2, (mlib_u16 *)da,
				    dlb, xsize, ysize, ad);
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {

		switch (nchan) {

		case 1:
			mlib_v_ImageScalarBlend_S32_1((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, xsize, ysize, (void *)alpha);
			break;

		case 2:
			mlib_v_ImageScalarBlend_S32_2((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, xsize, ysize, (void *)alpha);
			break;

		case 3:
			mlib_v_ImageScalarBlend_S32_3((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, xsize, ysize, (void *)alpha);
			break;

		case 4:
			mlib_v_ImageScalarBlend_S32_4((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, xsize, ysize, (void *)alpha);
			break;

		default:
			return (MLIB_FAILURE);
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
