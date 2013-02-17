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

#pragma ident	"@(#)mlib_v_ImageDataTypeConvert.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *  mlib_ImageDataTypeConvert - Convert one data type of image
 *                              to another data type of image
 *
 * SYNOPSIS
 *  mlib_status mlib_ImageDataTypeConvert(mlib_image       *dst,
 *                                        const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      dst and src must have the same size and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Convert one data type of image to another data type of image.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageDataTypeConvert.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDataTypeConvert = __mlib_ImageDataTypeConvert

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDataTypeConvert) mlib_ImageDataTypeConvert
    __attribute__((weak, alias("__mlib_ImageDataTypeConvert")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageDataTypeConvert(
    mlib_image *dst,
    const mlib_image *src)
{
	const mlib_s32 X8 = 0x7;
	const mlib_s32 X4 = 0x3;
	const mlib_s32 D1 = MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

/* data types of src and dst */
	mlib_type stype, dtype;

/* pointer for pixel in src */
	void *sp;

/* pointer for pixel in dst */
	void *dp;

/* number of channels for src and dst */
	mlib_s32 channels;

/* for src and dst */
	mlib_s32 width, height;

/* strides in bytes for src */
	mlib_s32 strides;

/* strides in bytes for dst */
	mlib_s32 strided;
	mlib_s32 flags;
	mlib_s32 flagd;
	mlib_s32 dsize;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	channels = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	strides = mlib_ImageGetStride(src);
	strided = mlib_ImageGetStride(dst);
	sp = mlib_ImageGetData(src);
	dp = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);

	width = width * channels;
	dsize = width * height;

	switch (stype) {

	case MLIB_BIT:

		switch (dtype) {

		case MLIB_BYTE:

			if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_U8_A8D1X8(
				    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flagd & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_U8_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_U8_D1((mlib_u8
				    *)sp, (mlib_u8 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_BIT_U8((mlib_u8 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_USHORT:
/* BIT -> U16 equal to BIT -> S16 */

			if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S16_A8D1X8((void
				    *)sp, (void *)dp, dsize);
			} else if (((flagd & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S16_A8D2X8((void
				    *)sp, strides, (void *)dp, strided, width,
				    height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S16_D1((void *)
				    sp, (void *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_BIT_S16((void *)sp,
				    strides, (void *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_SHORT:

			if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S16_A8D1X8((void
				    *)sp, (void *)dp, dsize);
			} else if (((flagd & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S16_A8D2X8((void
				    *)sp, strides, (void *)dp, strided, width,
				    height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S16_D1((void *)
				    sp, (void *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_BIT_S16((void *)sp,
				    strides, (void *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_INT:

			if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & X8) == 0)) {

				mlib_v_ImageDataTypeConvert_BIT_S32_A8D1X8(
				    (mlib_u8 *)sp, (mlib_s32 *)dp, dsize);
			} else if (((flagd & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S32_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_s32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_S32_D1((mlib_u8
				    *)sp, (mlib_s32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_BIT_S32((mlib_u8 *)
				    sp, strides, (mlib_s32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_FLOAT:

			if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & A8D1) == 0) && ((dsize & X8) == 0)) {

				mlib_v_ImageDataTypeConvert_BIT_F32_A8D1X8(
				    (mlib_u8 *)sp, (mlib_f32 *)dp, dsize);
			} else if (((flagd & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_F32_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_f32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_BIT_F32_D1((mlib_u8
				    *)sp, (mlib_f32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_BIT_F32((mlib_u8 *)
				    sp, strides, (mlib_f32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_DOUBLE:
			mlib_v_ImageDataTypeConvert_BIT_D64(src, dst);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_BYTE:

		switch (dtype) {

		case MLIB_BIT:

			if (((flags & A8D1) == 0) && ((flagd & D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_BIT_A8D1X8(
				    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_BIT_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else {
				mlib_v_ImageDataTypeConvert_U8_BIT((mlib_u8 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_USHORT:
/* U8 -> U16 equal to U8 -> S16 */

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S16_A8D1X8((void
				    *)sp, (void *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S16_A8D2X8((void
				    *)sp, strides, (void *)dp, strided, width,
				    height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S16_D1((void *)
				    sp, (void *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U8_S16((void *)sp,
				    strides, (void *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_SHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S16_A8D1X8((void
				    *)sp, (void *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S16_A8D2X8((void
				    *)sp, strides, (void *)dp, strided, width,
				    height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S16_D1((void *)
				    sp, (void *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U8_S16((void *)sp,
				    strides, (void *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_INT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S32_A8D1X8(
				    (mlib_u8 *)sp, (mlib_s32 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S32_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_s32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_S32_D1((mlib_u8
				    *)sp, (mlib_s32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U8_S32((mlib_u8 *)
				    sp, strides, (mlib_s32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_FLOAT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_F32_A8D1X8(
				    (mlib_u8 *)sp, (mlib_f32 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_F32_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_f32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U8_F32_D1((mlib_u8
				    *)sp, (mlib_f32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U8_F32((mlib_u8 *)
				    sp, strides, (mlib_f32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_DOUBLE:
			mlib_v_ImageDataTypeConvert_U8_D64(src, dst);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_USHORT:

		switch (dtype) {

		case MLIB_BIT:

			if (((flags & A8D1) == 0) && ((flagd & D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_BIT_A8D1X8(
				    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_BIT_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else {
				mlib_v_ImageDataTypeConvert_U16_BIT((mlib_u8 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_BYTE:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_U8_A8D1X8(
				    (mlib_u16 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_U8_A8D2X8(
				    (mlib_u16 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_U8_D1((mlib_u16
				    *)sp, (mlib_u8 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U16_U8((mlib_u16 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_SHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_S16_A8D1X8(
				    (mlib_u16 *)sp, (mlib_s16 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_S16_A8D2X8(
				    (mlib_u16 *)sp, strides, (mlib_s16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_S16_D1((mlib_u16
				    *)sp, (mlib_s16 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U16_S16((mlib_u16 *)
				    sp, strides, (mlib_s16 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_INT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_S32_A8D1X8(
				    (mlib_u16 *)sp, (mlib_s32 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_S32_A8D2X8(
				    (mlib_u16 *)sp, strides, (mlib_s32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_S32_D1((mlib_u16
				    *)sp, (mlib_s32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U16_S32((mlib_u16 *)
				    sp, strides, (mlib_s32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_FLOAT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_F32_A8D1X8(
				    (mlib_u16 *)sp, (mlib_f32 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_F32_A8D2X8(
				    (mlib_u16 *)sp, strides, (mlib_f32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_F32_D1((mlib_u16
				    *)sp, (mlib_f32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U16_F32((mlib_u16 *)
				    sp, strides, (mlib_f32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_DOUBLE:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_D64_A8D1X8(
				    (mlib_u16 *)sp, (mlib_d64 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_D64_A8D2X8(
				    (mlib_u16 *)sp, strides, (mlib_d64 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_U16_D64_D1((mlib_u16
				    *)sp, (mlib_d64 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_U16_D64((mlib_u16 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height);
			}

			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_SHORT:

		switch (dtype) {

		case MLIB_BIT:

			if (((flags & A8D1) == 0) && ((flagd & D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_BIT_A8D1X8(
				    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_BIT_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else {
				mlib_v_ImageDataTypeConvert_S16_BIT((mlib_u8 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_BYTE:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_U8_A8D1X8(
				    (mlib_s16 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_U8_A8D2X8(
				    (mlib_s16 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_U8_D1((mlib_s16
				    *)sp, (mlib_u8 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S16_U8((mlib_s16 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_USHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_U16_A8D1X8(
				    (mlib_s16 *)sp, (mlib_u16 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_U16_A8D2X8(
				    (mlib_s16 *)sp, strides, (mlib_u16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_U16_D1((mlib_s16
				    *)sp, (mlib_u16 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S16_U16((mlib_s16 *)
				    sp, strides, (mlib_u16 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_INT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_S32_A8D1X8(
				    (mlib_s16 *)sp, (mlib_s32 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_S32_A8D2X8(
				    (mlib_s16 *)sp, strides, (mlib_s32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_S32_D1((mlib_s16
				    *)sp, (mlib_s32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S16_S32((mlib_s16 *)
				    sp, strides, (mlib_s32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_FLOAT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_F32_A8D1X8(
				    (mlib_s16 *)sp, (mlib_f32 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_F32_A8D2X8(
				    (mlib_s16 *)sp, strides, (mlib_f32 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_F32_D1((mlib_s16
				    *)sp, (mlib_f32 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S16_F32((mlib_s16 *)
				    sp, strides, (mlib_f32 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_DOUBLE:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_D64_A8D1X8(
				    (mlib_s16 *)sp, (mlib_d64 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_D64_A8D2X8(
				    (mlib_s16 *)sp, strides, (mlib_d64 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S16_D64_D1((mlib_s16
				    *)sp, (mlib_d64 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S16_D64((mlib_s16 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height);
			}

			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_INT:

		switch (dtype) {

		case MLIB_BIT:

			if (((flags & A8D1) == 0) && ((flagd & D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_BIT_A8D1X8((void
				    *)sp, (void *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_BIT_A8D2X8((void
				    *)sp, strides, (void *)dp, strided, width,
				    height);
			} else {
				mlib_v_ImageDataTypeConvert_S32_BIT((void *)sp,
				    strides, (void *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_BYTE:
			mlib_v_ImageDataTypeConvert_S32_U8(src, dst);
			break;

		case MLIB_USHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_U16_A8D1X8(
				    (mlib_s32 *)sp, (mlib_u16 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_U16_A8D2X8(
				    (mlib_s32 *)sp, strides, (mlib_u16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_U16_D1((mlib_s32
				    *)sp, (mlib_u16 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S32_U16((mlib_s32 *)
				    sp, strides, (mlib_u16 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_SHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_S16_A8D1X8(
				    (mlib_s32 *)sp, (mlib_s16 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_S16_A8D2X8(
				    (mlib_s32 *)sp, strides, (mlib_s16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_S16_D1((mlib_s32
				    *)sp, (mlib_s16 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_S32_S16((mlib_s32 *)
				    sp, strides, (mlib_s16 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_FLOAT:
			mlib_v_ImageDataTypeConvert_S32_F32(src, dst);
			break;

		case MLIB_DOUBLE:
			mlib_v_ImageDataTypeConvert_S32_D64(src, dst);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_FLOAT:

		switch (dtype) {

		case MLIB_BIT:
/* F32 -> BIT equal to S32 -> BIT */

			if (((flags & A8D1) == 0) && ((flagd & D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_BIT_A8D1X8((void
				    *)sp, (void *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
					((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_S32_BIT_A8D2X8((void
				    *)sp, strides, (void *)dp, strided, width,
				    height);
			} else {
				mlib_v_ImageDataTypeConvert_S32_BIT((void *)sp,
				    strides, (void *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_BYTE:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_F32_U8_A8D2X8(
				    (mlib_f32 *)sp, strides, (mlib_u8 *)dp,
				    strided, dsize, 1);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X8) == 0)) {
				mlib_v_ImageDataTypeConvert_F32_U8_A8D2X8(
				    (mlib_f32 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_F32_U8_D1((mlib_f32
				    *)sp, (mlib_u8 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_F32_U8((mlib_f32 *)
				    sp, strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_USHORT:
			mlib_v_ImageDataTypeConvert_F32_U16(src, dst);
			break;

		case MLIB_SHORT:
			mlib_v_ImageDataTypeConvert_F32_S16(src, dst);
			break;

		case MLIB_INT:
			mlib_v_ImageDataTypeConvert_F32_S32(src, dst);
			break;

		case MLIB_DOUBLE:
			mlib_v_ImageDataTypeConvert_F32_D64(src, dst);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_DOUBLE:

		switch (dtype) {

		case MLIB_BIT:
			mlib_v_ImageDataTypeConvert_D64_BIT(src, dst);
			break;

		case MLIB_BYTE:
			mlib_v_ImageDataTypeConvert_D64_U8(src, dst);
			break;

		case MLIB_USHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageDataTypeConvert_D64_U16_A8D1X4(
				    (mlib_d64 *)sp, (mlib_u16 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X4) == 0)) {
				mlib_v_ImageDataTypeConvert_D64_U16_A8D2X4(
				    (mlib_d64 *)sp, strides, (mlib_u16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_D64_U16_D1((mlib_d64
				    *)sp, (mlib_u16 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_D64_U16((mlib_d64 *)
				    sp, strides, (mlib_u16 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_SHORT:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageDataTypeConvert_D64_S16_A8D1X4(
				    (mlib_d64 *)sp, (mlib_s16 *)dp, dsize);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((width & X4) == 0)) {
				mlib_v_ImageDataTypeConvert_D64_S16_A8D2X4(
				    (mlib_d64 *)sp, strides, (mlib_s16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageDataTypeConvert_D64_S16_D1((mlib_d64
				    *)sp, (mlib_s16 *)dp, dsize);
			} else {
				mlib_v_ImageDataTypeConvert_D64_S16((mlib_d64 *)
				    sp, strides, (mlib_s16 *)dp, strided, width,
				    height);
			}

			break;

		case MLIB_INT:
			mlib_v_ImageDataTypeConvert_D64_S32(src, dst);
			break;

		case MLIB_FLOAT:
			mlib_v_ImageDataTypeConvert_D64_F32(src, dst);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
