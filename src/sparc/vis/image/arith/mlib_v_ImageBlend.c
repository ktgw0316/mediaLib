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

#pragma ident	"@(#)mlib_v_ImageBlend.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend - blend two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend(mlib_image       *dst,
 *                                  const mlib_image *src1,
 *                                  const mlib_image *src2,
 *                                  const mlib_image *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to alpha image
 *
 * RESTRICTION
 *      src1, src2, dst, and alpha must be the same type,
 *      the same size and the same number of channels.
 *      They can have one to four channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type. Optionally, alpha can be
 *      single-channel image while src1, src2, and dst are multi-channel images.
 *      MLIB_BYTE alpha coefficients are in Q8 format.
 *      MLIB_SHORT alpha coefficients are in Q15 format and must be positive.
 *      MLIB_USHORT alpha coefficients are in Q16 format.
 *      MLIB_INT alpha coefficients are in Q31 format and must be positive.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (1-alpha)*src2
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageBlendFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend = __mlib_ImageBlend

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend) mlib_ImageBlend
    __attribute__((weak, alias("__mlib_ImageBlend")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_image *alpha)
{
	mlib_s32 slb1, slb2, dlb, alb, xsize, ysize, nchan, achan;
	mlib_type dtype;
	void *sa1, *sa2, *da, *aa;
	const mlib_s32 D1 = MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;
	mlib_s32 flag1, flag2, flagd, flaga;
	mlib_s32 dsize;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_CHECK(alpha);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);
	MLIB_IMAGE_TYPE_EQUAL(dst, alpha);
	MLIB_IMAGE_SIZE_EQUAL(dst, alpha);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);
	slb1 = mlib_ImageGetStride(src1);
	sa1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa2 = mlib_ImageGetData(src2);
	alb = mlib_ImageGetStride(alpha);
	aa = mlib_ImageGetData(alpha);
	achan = mlib_ImageGetChannels(alpha);

	flagd = mlib_ImageGetFlags(dst);
	flag1 = mlib_ImageGetFlags(src1);
	flag2 = mlib_ImageGetFlags(src2);
	flaga = mlib_ImageGetFlags(alpha);

	xsize = xsize * achan;
	dsize = xsize * ysize;

	if (achan == nchan) {

		if (dtype == MLIB_BYTE) {

/* initialize GSR scale factor */
			vis_write_gsr(3 << 3);

			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flaga & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 7) == 0)) {

				mlib_v_ImageBlend_U8_A8D1X8((mlib_u8 *)sa1,
				    (mlib_u8 *)sa2,
				    (mlib_u8 *)da, (mlib_u8 *)aa, dsize);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flaga & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageBlend_U8_A8D2((mlib_u8 *)sa1, slb1,
				    (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb,
				    (mlib_u8 *)aa, alb, xsize, ysize);
			} else {

				mlib_v_ImageBlend_U8((mlib_u8 *)sa1, slb1,
				    (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb,
				    (mlib_u8 *)aa, alb, xsize, ysize);
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_SHORT) {

			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flaga & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 3) == 0)) {

				mlib_v_ImageBlend_S16_A8D1X4((mlib_s16 *)sa1,
				    (mlib_s16 *)sa2,
				    (mlib_s16 *)da, (mlib_s16 *)aa, dsize);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flaga & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageBlend_S16_A8D2((mlib_s16 *)sa1,
				    slb1, (mlib_s16 *)sa2, slb2, (mlib_s16 *)da,
				    dlb, (mlib_s16 *)aa, alb, xsize, ysize);
			} else {

				mlib_v_ImageBlend_S16((mlib_s16 *)sa1, slb1,
				    (mlib_s16 *)sa2, slb2,
				    (mlib_s16 *)da, dlb,
				    (mlib_s16 *)aa, alb, xsize, ysize);
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_USHORT) {

			if (((flag1 & A8D1) == 0) && ((flag2 & A8D1) == 0) &&
			    ((flaga & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 3) == 0)) {

				mlib_v_ImageBlend_U16_A8D1X4((mlib_u16 *)sa1,
				    (mlib_u16 *)sa2,
				    (mlib_u16 *)da, (mlib_u16 *)aa, dsize);

			} else if (((flag1 & A8D2) == 0) &&
			    ((flag2 & A8D2) == 0) && ((flaga & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageBlend_U16_A8D2((mlib_u16 *)sa1,
				    slb1, (mlib_u16 *)sa2, slb2, (mlib_u16 *)da,
				    dlb, (mlib_u16 *)aa, alb, xsize, ysize);
			} else {

				mlib_v_ImageBlend_U16((mlib_u16 *)sa1, slb1,
				    (mlib_u16 *)sa2, slb2,
				    (mlib_u16 *)da, dlb,
				    (mlib_u16 *)aa, alb, xsize, ysize);
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_INT) {

			mlib_v_ImageBlend_S32((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb,
			    (mlib_s32 *)aa, alb, xsize, ysize);
			return (MLIB_SUCCESS);
		}

	} else if (achan == 1) {

		if (dtype == MLIB_BYTE) {

/* initialize GSR scale factor */
			vis_write_gsr(3 << 3);

			switch (nchan) {

			case 2:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & 7) == 0)) {

					mlib_v_ImageBlendSA_U8_2_A8D1X8((mlib_u8
					    *)sa1, (mlib_u8 *)sa2,
					    (mlib_u8 *)da, (mlib_u8 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flaga & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_U8_2_A8D2((mlib_u8
					    *)sa1, slb1, (mlib_u8 *)sa2, slb2,
					    (mlib_u8 *)da, dlb, (mlib_u8 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_U8_2((mlib_u8 *)sa1,
					    slb1, (mlib_u8 *)sa2, slb2,
					    (mlib_u8 *)da, dlb, (mlib_u8 *)aa,
					    alb, xsize, ysize);
				}

				break;

			case 3:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & A8D1) == 0) &&
				    ((flagd & A8D1) == 0)) {

					mlib_v_ImageBlendSA_U8_3_A8D1((mlib_u8
					    *)sa1, (mlib_u8 *)sa2,
					    (mlib_u8 *)da, (mlib_u8 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flaga & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_U8_3_A8D2((mlib_u8
					    *)sa1, slb1, (mlib_u8 *)sa2, slb2,
					    (mlib_u8 *)da, dlb, (mlib_u8 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_U8_3((mlib_u8 *)sa1,
					    slb1, (mlib_u8 *)sa2, slb2,
					    (mlib_u8 *)da, dlb, (mlib_u8 *)aa,
					    alb, xsize, ysize);
				}

				break;

			case 4:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & 1) == 0)) {

					mlib_v_ImageBlendSA_U8_4_A8D1X8((mlib_u8
					    *)sa1, (mlib_u8 *)sa2,
					    (mlib_u8 *)da, (mlib_u8 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_U8_4_A8D2((mlib_u8
					    *)sa1, slb1, (mlib_u8 *)sa2, slb2,
					    (mlib_u8 *)da, dlb, (mlib_u8 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_U8_4((mlib_u8 *)sa1,
					    slb1, (mlib_u8 *)sa2, slb2,
					    (mlib_u8 *)da, dlb, (mlib_u8 *)aa,
					    alb, xsize, ysize);
				}

				break;

			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_SHORT) {

			switch (nchan) {

			case 2:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & 3) == 0)) {

					mlib_v_ImageBlendSA_S16_2_A8D1X4(
					    (mlib_s16 *)sa1, (mlib_s16 *)sa2,
					    (mlib_s16 *)da, (mlib_s16 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flaga & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_S16_2_A8D2((mlib_s16
					    *)sa1, slb1, (mlib_s16 *)sa2, slb2,
					    (mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_S16_2((mlib_s16 *)
					    sa1, slb1, (mlib_s16 *)sa2, slb2,
					    (mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					    alb, xsize, ysize);
				}

				break;

			case 3:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & D1) == 0) &&
				    ((flagd & A8D1) == 0)) {

					mlib_v_ImageBlendSA_S16_3_A8D1((mlib_s16
					    *)sa1, (mlib_s16 *)sa2,
					    (mlib_s16 *)da, (mlib_s16 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_S16_3_A8D2((mlib_s16
					    *)sa1, slb1, (mlib_s16 *)sa2, slb2,
					    (mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_S16_3((mlib_s16 *)
					    sa1, slb1, (mlib_s16 *)sa2, slb2,
					    (mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					    alb, xsize, ysize);
				}

				break;

			case 4:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & D1) == 0) &&
				    ((flagd & A8D1) == 0)) {

					mlib_v_ImageBlendSA_S16_4_A8D1((mlib_s16
					    *)sa1, (mlib_s16 *)sa2,
					    (mlib_s16 *)da, (mlib_s16 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_S16_4_A8D2((mlib_s16
					    *)sa1, slb1, (mlib_s16 *)sa2, slb2,
					    (mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_S16_4((mlib_s16 *)
					    sa1, slb1, (mlib_s16 *)sa2, slb2,
					    (mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					    alb, xsize, ysize);
				}

				break;

			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_USHORT) {

			switch (nchan) {

			case 2:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & 3) == 0)) {

					mlib_v_ImageBlendSA_U16_2_A8D1X4(
					    (mlib_u16 *)sa1, (mlib_u16 *)sa2,
					    (mlib_u16 *)da, (mlib_u16 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flaga & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_U16_2_A8D2((mlib_u16
					    *)sa1, slb1, (mlib_u16 *)sa2, slb2,
					    (mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_U16_2((mlib_u16 *)
					    sa1, slb1, (mlib_u16 *)sa2, slb2,
					    (mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					    alb, xsize, ysize);
				}

				break;

			case 3:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & D1) == 0) &&
				    ((flagd & A8D1) == 0)) {

					mlib_v_ImageBlendSA_U16_3_A8D1((mlib_u16
					    *)sa1, (mlib_u16 *)sa2,
					    (mlib_u16 *)da, (mlib_u16 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_U16_3_A8D2((mlib_u16
					    *)sa1, slb1, (mlib_u16 *)sa2, slb2,
					    (mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_U16_3((mlib_u16 *)
					    sa1, slb1, (mlib_u16 *)sa2, slb2,
					    (mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					    alb, xsize, ysize);
				}

				break;

			case 4:

				if (((flag1 & A8D1) == 0) &&
				    ((flag2 & A8D1) == 0) &&
				    ((flaga & D1) == 0) &&
				    ((flagd & A8D1) == 0)) {

					mlib_v_ImageBlendSA_U16_4_A8D1((mlib_u16
					    *)sa1, (mlib_u16 *)sa2,
					    (mlib_u16 *)da, (mlib_u16 *)aa,
					    dsize);

				} else if (((flag1 & A8D2) == 0) &&
				    ((flag2 & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {

					mlib_v_ImageBlendSA_U16_4_A8D2((mlib_u16
					    *)sa1, slb1, (mlib_u16 *)sa2, slb2,
					    (mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					    alb, xsize, ysize);
				} else {

					mlib_v_ImageBlendSA_U16_4((mlib_u16 *)
					    sa1, slb1, (mlib_u16 *)sa2, slb2,
					    (mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					    alb, xsize, ysize);
				}

				break;

			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_INT) {

			mlib_v_ImageBlendSA_S32((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb,
			    (mlib_s32 *)aa, alb, xsize, ysize, nchan);
			return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
