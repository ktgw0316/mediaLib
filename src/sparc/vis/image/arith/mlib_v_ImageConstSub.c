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

#pragma ident	"@(#)mlib_v_ImageConstSub.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstSub - subtraction image from constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstSub(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *c);
 *
 * ARGUMENT
 *      src     pointer to input image
 *      dst     pointer to output image
 *      c       array of constants each pixel to be subtracted from
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst(i, j) = c - src(i, j).
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageConstSubFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstSub = __mlib_ImageConstSub

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstSub) mlib_ImageConstSub
    __attribute__((weak, alias("__mlib_ImageConstSub")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConstSub(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s32 slb, dlb, xsize, ysize, dsize, nchan, flags, flagd, i;
	mlib_s32 c_new[4];
	mlib_type dtype;
	void *sa, *da;
	mlib_d64 dc, dc1, dc2;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (c == NULL)
		return (MLIB_FAILURE);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);
	slb = mlib_ImageGetStride(src);
	sa = mlib_ImageGetData(src);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);

	xsize = xsize * nchan;
	dsize = xsize * ysize;

	if ((dtype == MLIB_INT) || (dtype == MLIB_USHORT)) {
		if (dtype == MLIB_USHORT) {
			for (i = 0; i < nchan; i++) {
				c_new[i] = c[i];

				if (c[i] > 2 * MLIB_U16_MAX)
					c_new[i] = 2 * MLIB_U16_MAX;
				else if (c[i] < MLIB_U16_MIN)
					c_new[i] = MLIB_U16_MIN;
				c_new[i] = (c_new[i] - 65536) * 2;
			}

			c = c_new;
		}

		switch (nchan) {
		case 1:
			dc1 = dc2 = vis_to_double_dup(c[0]);
			break;

		case 2:
			dc1 = dc2 = vis_to_double(c[0], c[1]);
			break;

		case 3:
			dc1 = vis_to_double(c[0], c[1]);
			dc2 = vis_to_double(c[2], c[0]);
			break;

		case 4:
			dc1 = vis_to_double(c[0], c[1]);
			dc2 = vis_to_double(c[2], c[3]);
			break;

		default:
			return (MLIB_FAILURE);
		}
	} else {
		if (dtype == MLIB_BYTE) {
			for (i = 0; i < nchan; i++) {
				c_new[i] = c[i];

				if (c[i] > MLIB_S16_MAX)
					c_new[i] = MLIB_S16_MAX;
				else if (c[i] < MLIB_U8_MIN)
					c_new[i] = MLIB_U8_MIN;
			}

			c = c_new;
		}

		switch (nchan) {
		case 1:
			dc = vis_to_double_dup((c[0] << 16) | (c[0] & 0xffff));
			break;

		case 2:
			dc = vis_to_double_dup((c[0] << 16) | (c[1] & 0xffff));
			break;

		case 3:
			dc = vis_to_double((c[0] << 16) | (c[1] & 0xffff),
			    (c[2] << 16) | (c[0] & 0xffff));
			break;

		case 4:
			dc = vis_to_double((c[0] << 16) | (c[1] & 0xffff),
			    (c[2] << 16) | (c[3] & 0xffff));
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	if (dtype == MLIB_BYTE) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageConstSub_U8_3_A8D1((mlib_u8 *)sa,
				    (mlib_u8 *)da, dsize, dc);

			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageConstSub_U8_3_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize, dc);
			} else {

				mlib_v_ImageConstSub_U8_3((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, dc);
			}
		} else {
/* 1, 2 or 4 channels */
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 7) == 0)) {

				mlib_v_ImageConstSub_U8_124_A8D1X8((mlib_u8 *)
				    sa, (mlib_u8 *)da, dsize, dc);

			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageConstSub_U8_124_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize, dc);
			} else {

				mlib_v_ImageConstSub_U8_124((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, dc);
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageConstSub_S16_3_A8D1((mlib_s16 *)sa,
				    (mlib_s16 *)da, dsize, dc);

			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageConstSub_S16_3_A8D2((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize, dc);
			} else {

				mlib_v_ImageConstSub_S16_3((mlib_s16 *)sa, slb,
				    (mlib_s16 *)da, dlb, xsize, ysize, dc);
			}
		} else {
/* 1, 2 or 4 channels */

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 3) == 0)) {

				mlib_v_ImageConstSub_S16_124_A8D1X4((mlib_s16 *)
				    sa, (mlib_s16 *)da, dsize, dc);

			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageConstSub_S16_124_A8D2((mlib_s16 *)
				    sa, slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    dc);
			} else {

				mlib_v_ImageConstSub_S16_124((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize, dc);
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageConstSub_U16_3((mlib_s16 *)sa, slb,
			    (mlib_s16 *)da, dlb, xsize, ysize, dc1, dc2);
		} else {
/* 1, 2 or 4 channels */

			mlib_v_ImageConstSub_U16_124((mlib_s16 *)sa, slb,
			    (mlib_s16 *)da, dlb, xsize, ysize, dc1, dc2);
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageConstSub_S32_3_A8D1((mlib_s32 *)sa,
				    (mlib_s32 *)da, dsize, dc1, dc2);

			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageConstSub_S32_3_A8D2((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize, dc1,
				    dc2);
			} else {

				mlib_v_ImageConstSub_S32_3((mlib_s32 *)sa, slb,
				    (mlib_s32 *)da, dlb,
				    xsize, ysize, dc1, dc2);
			}
		} else {
/* 1, 2 or 4 channels */

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 3) == 0)) {

				mlib_v_ImageConstSub_S32_124_A8D1X4((mlib_s32 *)
				    sa, (mlib_s32 *)da, dsize, dc1, dc2);

			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageConstSub_S32_124_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    dc1, dc2);
			} else {

				mlib_v_ImageConstSub_S32_124((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize, dc1,
				    dc2);
			}
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
