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

#pragma ident	"@(#)mlib_v_ImageCopyMask.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopyMask  - Copy one image to another via a mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopyMask(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_image *mask,
 *                                     const mlib_s32   *thresh);
 *
 * ARGUMENT
 *      dst     pointer to output or destination image
 *      src     pointer to input or source image
 *      mask    pointer to mask image
 *      thresh  array of thresholds
 *
 * RESTRICTION
 *      src, dst and mask images must have the same size, type and
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Copy one image to another via a mask.
 *      dst(i, j) = src(i, j) when mask(i, j) <= *thresh, or
 *                 dst(i, j) when mask(i, j) >  *thresh
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageCopyMask_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopyMask = __mlib_ImageCopyMask

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopyMask) mlib_ImageCopyMask
    __attribute__((weak, alias("__mlib_ImageCopyMask")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCopyMask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_image *mask,
    const mlib_s32 *thresh)
{
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;
	mlib_type dtype;
	mlib_s32 slb, mlb, dlb, xsize, ysize, nchan;
	void *sa, *ma, *da;
	mlib_d64 dthresh0, dthresh1;
	mlib_s32 flags, flagm, flagd;
	mlib_s32 dsize;
	mlib_s32 chmask = 0;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(mask);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_EQUAL(src, mask);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, mask);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, mask);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst) * nchan;
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	mlb = mlib_ImageGetStride(mask);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	ma = mlib_ImageGetData(mask);
	da = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagm = mlib_ImageGetFlags(mask);
	flagd = mlib_ImageGetFlags(dst);
	dsize = xsize * ysize;

	if (dtype == MLIB_INT) {
		switch (nchan) {
		case 1:
			dthresh0 = vis_to_double_dup(thresh[0]);
			dthresh1 = dthresh0;
			break;

		case 2:
			dthresh0 = vis_to_double(thresh[0], thresh[1]);
			dthresh1 = dthresh0;
			break;

		case 3:
			dthresh0 = vis_to_double(thresh[0], thresh[1]);
			dthresh1 = vis_to_double(thresh[2], thresh[0]);
			break;

		case 4:
			dthresh0 = vis_to_double(thresh[0], thresh[1]);
			dthresh1 = vis_to_double(thresh[2], thresh[3]);
			break;

		default:
			return (MLIB_FAILURE);
		}
	} else if (dtype == MLIB_USHORT) {
		mlib_u32 lthresh0, lthresh1, lthresh2, lthresh3;
		mlib_s32 i, ch;
		mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

		for (i = 0; i < nchan; i++) {
			ch = (thresh[i] >= MLIB_U16_MAX) ? 1 : 0;
			chmask = (chmask << 1) | ch;
		}

		switch (nchan) {
		case 1:
			lthresh0 = (thresh[0] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[0] < 0) ? 0 : (thresh[0] + 1));
			dthresh0 =
			    vis_to_double_dup((lthresh0 << 16) | (lthresh0 &
			    0xFFFF));
			break;

		case 2:
			lthresh0 = (thresh[0] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[0] < 0) ? 0 : (thresh[0] + 1));
			lthresh1 = (thresh[1] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[1] < 0) ? 0 : (thresh[1] + 1));
			dthresh0 =
			    vis_to_double_dup((lthresh0 << 16) | (lthresh1 &
			    0xFFFF));
			break;

		case 3:
			lthresh0 = (thresh[0] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[0] < 0) ? 0 : (thresh[0] + 1));
			lthresh1 = (thresh[1] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[1] < 0) ? 0 : (thresh[1] + 1));
			lthresh2 = (thresh[2] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[2] < 0) ? 0 : (thresh[2] + 1));
			dthresh0 =
			    vis_to_double((lthresh0 << 16) | (lthresh1 &
			    0xFFFF), (lthresh2 << 16) | (lthresh0 & 0xFFFF));
			break;

		case 4:
			lthresh0 = (thresh[0] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[0] < 0) ? 0 : (thresh[0] + 1));
			lthresh1 = (thresh[1] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[1] < 0) ? 0 : (thresh[1] + 1));
			lthresh2 = (thresh[2] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[2] < 0) ? 0 : (thresh[2] + 1));
			lthresh3 = (thresh[3] >= MLIB_U16_MAX) ? MLIB_U16_MAX :
			    ((thresh[3] < 0) ? 0 : (thresh[3] + 1));
			dthresh0 =
			    vis_to_double((lthresh0 << 16) | (lthresh1 &
			    0xFFFF), (lthresh2 << 16) | (lthresh3 & 0xFFFF));
			break;

		default:
			return (MLIB_FAILURE);
		}

		dthresh0 = vis_fxor(dthresh0, xor_mask);
	} else {
		mlib_s32 lthresh0, lthresh1, lthresh2, lthresh3;
		mlib_s32 i, ch;

		for (i = 0; i < nchan; i++) {
			ch = (thresh[i] >= MLIB_S16_MAX) ? 1 : 0;
			chmask = (chmask << 1) | ch;
		}

		switch (nchan) {
		case 1:
			lthresh0 = (thresh[0] >= MLIB_S16_MAX) ? MLIB_S16_MAX :
			    ((thresh[0] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[0] + 1));
			dthresh0 =
			    vis_to_double_dup((lthresh0 << 16) | (lthresh0 &
			    0xFFFF));
			break;

		case 2:
			lthresh0 = (thresh[0] >= MLIB_S16_MAX) ? MLIB_S16_MAX :
			    ((thresh[0] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[0] + 1));
			lthresh1 =
			    (thresh[1] >=
			    MLIB_S16_MAX) ? MLIB_S16_MAX : ((thresh[1] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[1] + 1));
			dthresh0 =
			    vis_to_double_dup((lthresh0 << 16) | (lthresh1 &
			    0xFFFF));
			break;

		case 3:
			lthresh0 = (thresh[0] >= MLIB_S16_MAX) ? MLIB_S16_MAX :
			    ((thresh[0] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[0] + 1));
			lthresh1 =
			    (thresh[1] >=
			    MLIB_S16_MAX) ? MLIB_S16_MAX : ((thresh[1] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[1] + 1));
			lthresh2 =
			    (thresh[2] >=
			    MLIB_S16_MAX) ? MLIB_S16_MAX : ((thresh[2] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[2] + 1));
			dthresh0 =
			    vis_to_double((lthresh0 << 16) | (lthresh1 &
			    0xFFFF), (lthresh2 << 16) | (lthresh0 & 0xFFFF));
			break;

		case 4:
			lthresh0 = (thresh[0] >= MLIB_S16_MAX) ? MLIB_S16_MAX :
			    ((thresh[0] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[0] + 1));
			lthresh1 =
			    (thresh[1] >=
			    MLIB_S16_MAX) ? MLIB_S16_MAX : ((thresh[1] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[1] + 1));
			lthresh2 =
			    (thresh[2] >=
			    MLIB_S16_MAX) ? MLIB_S16_MAX : ((thresh[2] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[2] + 1));
			lthresh3 =
			    (thresh[3] >=
			    MLIB_S16_MAX) ? MLIB_S16_MAX : ((thresh[3] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : (thresh[3] + 1));
			dthresh0 =
			    vis_to_double((lthresh0 << 16) | (lthresh1 &
			    0xFFFF), (lthresh2 << 16) | (lthresh3 & 0xFFFF));
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	if (dtype == MLIB_BYTE) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageCopyMask_U8_3_A8D1((mlib_u8 *)sa,
				    (mlib_u8 *)ma,
				    (mlib_u8 *)da, dsize, dthresh0);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_U8_3_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)ma, mlb, (mlib_u8 *)da, dlb,
				    xsize, ysize, dthresh0);
			} else {

				mlib_v_ImageCopyMask_U8_3((mlib_u8 *)sa, slb,
				    (mlib_u8 *)ma, mlb,
				    (mlib_u8 *)da, dlb, xsize, ysize, dthresh0);
			}
		} else {
/* 1, 2 or 4 channels */
			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 7) == 0)) {

				mlib_v_ImageCopyMask_U8_124_A8D1X8((mlib_u8 *)
				    sa, (mlib_u8 *)ma, (mlib_u8 *)da, dsize,
				    dthresh0);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_U8_124_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)ma, mlb, (mlib_u8 *)da, dlb,
				    xsize, ysize, dthresh0);
			} else {

				mlib_v_ImageCopyMask_U8_124((mlib_u8 *)sa, slb,
				    (mlib_u8 *)ma, mlb,
				    (mlib_u8 *)da, dlb, xsize, ysize, dthresh0);
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageCopyMask_S16_3_A8D1((mlib_s16 *)sa,
				    (mlib_s16 *)ma,
				    (mlib_s16 *)da, dsize, dthresh0);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_S16_3_A8D2((mlib_s16 *)sa,
				    slb, (mlib_s16 *)ma, mlb, (mlib_s16 *)da,
				    dlb, xsize, ysize, dthresh0);
			} else {

				mlib_v_ImageCopyMask_S16_3((mlib_s16 *)sa, slb,
				    (mlib_s16 *)ma, mlb,
				    (mlib_s16 *)da, dlb,
				    xsize, ysize, dthresh0);
			}
		} else {
/* 1, 2 or 4 channels */

			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 3) == 0)) {

				mlib_v_ImageCopyMask_S16_124_A8D1X4((mlib_s16 *)
				    sa, (mlib_s16 *)ma, (mlib_s16 *)da, dsize,
				    dthresh0);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_S16_124_A8D2((mlib_s16 *)
				    sa, slb, (mlib_s16 *)ma, mlb,
				    (mlib_s16 *)da, dlb, xsize, ysize,
				    dthresh0);
			} else {

				mlib_v_ImageCopyMask_S16_124((mlib_s16 *)sa,
				    slb, (mlib_s16 *)ma, mlb, (mlib_s16 *)da,
				    dlb, xsize, ysize, dthresh0);
			}
		}

		if (chmask != 0)
			__mlib_ImageChannelCopy(dst, (void *)src, chmask);

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageCopyMask_U16_3_A8D1((mlib_u16 *)sa,
				    (mlib_u16 *)ma,
				    (mlib_u16 *)da, dsize, dthresh0);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_U16_3_A8D2((mlib_u16 *)sa,
				    slb, (mlib_u16 *)ma, mlb, (mlib_u16 *)da,
				    dlb, xsize, ysize, dthresh0);
			} else {

				mlib_v_ImageCopyMask_U16_3((mlib_u16 *)sa, slb,
				    (mlib_u16 *)ma, mlb,
				    (mlib_u16 *)da, dlb,
				    xsize, ysize, dthresh0);
			}
		} else {
/* 1, 2 or 4 channels */

			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 3) == 0)) {

				mlib_v_ImageCopyMask_U16_124_A8D1X4((mlib_u16 *)
				    sa, (mlib_u16 *)ma, (mlib_u16 *)da, dsize,
				    dthresh0);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_U16_124_A8D2((mlib_u16 *)
				    sa, slb, (mlib_u16 *)ma, mlb,
				    (mlib_u16 *)da, dlb, xsize, ysize,
				    dthresh0);
			} else {

				mlib_v_ImageCopyMask_U16_124((mlib_u16 *)sa,
				    slb, (mlib_u16 *)ma, mlb, (mlib_u16 *)da,
				    dlb, xsize, ysize, dthresh0);
			}
		}

		if (chmask != 0) {

			__mlib_ImageChannelCopy(dst, (void *)src, chmask);
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {

/* 3 channels */
		if (nchan == 3) {
			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageCopyMask_S32_3_A8D1((mlib_s32 *)sa,
				    (mlib_s32 *)ma,
				    (mlib_s32 *)da, dsize, dthresh0, dthresh1);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_S32_3_A8D2((mlib_s32 *)sa,
				    slb, (mlib_s32 *)ma, mlb, (mlib_s32 *)da,
				    dlb, xsize, ysize, dthresh0, dthresh1);
			} else {

				mlib_v_ImageCopyMask_S32_3((mlib_s32 *)sa, slb,
				    (mlib_s32 *)ma, mlb,
				    (mlib_s32 *)da, dlb,
				    xsize, ysize, dthresh0, dthresh1);
			}
		} else {
/* 1, 2 or 4 channels */

			if (((flags & A8D1) == 0) &&
			    ((flagm & A8D1) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & 3) == 0)) {

				mlib_v_ImageCopyMask_S32_124_A8D1X4((mlib_s32 *)
				    sa, (mlib_s32 *)ma, (mlib_s32 *)da, dsize,
				    dthresh0, dthresh1);
			} else if (((flags & A8D2) == 0) &&
			    ((flagm & A8D2) == 0) && ((flagd & A8D2) == 0)) {

				mlib_v_ImageCopyMask_S32_124_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)ma, mlb,
				    (mlib_s32 *)da, dlb, xsize, ysize, dthresh0,
				    dthresh1);
			} else {

				mlib_v_ImageCopyMask_S32_124((mlib_s32 *)sa,
				    slb, (mlib_s32 *)ma, mlb, (mlib_s32 *)da,
				    dlb, xsize, ysize, dthresh0, dthresh1);
			}
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
