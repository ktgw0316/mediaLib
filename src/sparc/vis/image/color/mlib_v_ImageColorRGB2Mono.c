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

#pragma ident	"@(#)mlib_v_ImageColorRGB2Mono.c	9.2	07/11/05 SMI"

/*
 * FILENAME: mlib_v_ImageColorRGB2Mono.c
 *
 * FUNCTION
 *      mlib_ImageColorRGB2Mono - color to mono conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2Mono(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64   *weight);
 *
 *      mlib_status mlib_ImageColorRGB2CIEMono(mlib_image       *dst,
 *                                             const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      weight  Array of 3 blending coefficients
 *
 * RESTRICTION
 *      src and dst must be the same type, and the same size.
 *      The input must be a 3-channel image,
 *      and the output image must be a single channel image.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorRGB2Mono
 *        Z =  weight[0] * R +  weight[1] * G +  weight[2] * B
 *      mlib_ImageColorRGB2CIEMono
 *        Z =  0.2125 * R +  0.7154 * G +  0.0721 * B
 *
 *     it is required that all blending coefficients(weight) be no more
 *     than 1.0
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_v_ImageColorRGB2MonoFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2CIEMono = __mlib_ImageColorRGB2CIEMono
#pragma weak mlib_ImageColorRGB2Mono = __mlib_ImageColorRGB2Mono

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2CIEMono) mlib_ImageColorRGB2CIEMono
    __attribute__((weak, alias("__mlib_ImageColorRGB2CIEMono")));
__typeof__(__mlib_ImageColorRGB2Mono) mlib_ImageColorRGB2Mono
    __attribute__((weak, alias("__mlib_ImageColorRGB2Mono")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2Mono(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *weight)
{
	mlib_s32 X8 = 0x7;
	mlib_s32 X4 = 0x3;
	mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	mlib_s32 A8D2X8 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH8X;
	mlib_s32 A8D2X4 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH4X;
/* pointer for pixel in src */
	void *sp;

/* pointer for pixel in dst */
	void *dp;

/* for src and dst */
	mlib_s32 width, height;

/* strides in bytes for src */
	mlib_s32 strides;

/* strides in bytes for dst */
	mlib_s32 strided;
	mlib_s32 flags;
	mlib_s32 flagd;
	mlib_s32 dsize;
	mlib_f32 sum;
	mlib_s32 i;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(src, 3);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	for (i = 0; i < 3; i++)
		if (mlib_fabs(weight[i]) > 1.0)
			return (MLIB_OUTOFRANGE);

	width = mlib_ImageGetWidth(dst);
	height = mlib_ImageGetHeight(dst);
	strides = mlib_ImageGetStride(src);
	strided = mlib_ImageGetStride(dst);
	sp = mlib_ImageGetData(src);
	dp = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);
	dsize = width * height;

	switch (mlib_ImageGetType(dst)) {
	case MLIB_BYTE:

		if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
		    ((dsize & X8) == 0)) {

			mlib_v_ImageColorRGB2Mono_U8_A8D1X8((mlib_u8 *)sp,
			    (mlib_u8 *)dp, dsize, weight);
		} else if (((flags & A8D2X8) == 0) && ((flagd & A8D2X8) == 0)) {

			mlib_v_ImageColorRGB2Mono_U8_A8D2X8((mlib_u8 *)sp,
			    strides, (mlib_u8 *)dp, strided, width, height,
			    weight);
		} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
		    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
			mlib_v_ImageColorRGB2Mono_U8_D1((mlib_u8 *)sp,
			    (mlib_u8 *)dp, dsize, weight);
		} else {
			mlib_v_ImageColorRGB2Mono_U8((mlib_u8 *)sp, strides,
			    (mlib_u8 *)dp, strided, width, height, weight);
		}

		break;

	case MLIB_SHORT:
		sum =
		    mlib_fabs(weight[0]) + mlib_fabs(weight[1]) +
		    mlib_fabs(weight[2]);

		if (sum <= 1) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_S16_A8D1X4((mlib_s16
				    *)sp, (mlib_s16 *)dp, dsize, weight);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_S16_A8D2X4((mlib_s16
				    *)sp, strides, (mlib_s16 *)dp, strided,
				    width, height, weight);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageColorRGB2Mono_S16_D1((mlib_s16 *)sp,
				    (mlib_s16 *)dp, dsize, weight);
			} else {
				mlib_v_ImageColorRGB2Mono_S16((mlib_s16 *)sp,
				    strides, (mlib_s16 *)dp, strided, width,
				    height, weight);
			}
		} else {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_GEN_S16_A8D1X4(
				    (mlib_s16 *)sp, (mlib_s16 *)dp, dsize,
				    weight);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_GEN_S16_A8D2X4(
				    (mlib_s16 *)sp, strides, (mlib_s16 *)dp,
				    strided, width, height, weight);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageColorRGB2Mono_GEN_S16_D1((mlib_s16
				    *)sp, (mlib_s16 *)dp, dsize, weight);
			} else {
				mlib_v_ImageColorRGB2Mono_GEN_S16((mlib_s16 *)
				    sp, strides, (mlib_s16 *)dp, strided, width,
				    height, weight);
			}
		}

		break;

	case MLIB_USHORT:
		sum =
		    mlib_fabs(weight[0]) + mlib_fabs(weight[1]) +
		    mlib_fabs(weight[2]);

		if ((sum <= 1) && (weight[0] > 0) && (weight[1] > 0) &&
		    (weight[2] > 0)) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_U16_A8D1X4((mlib_u16
				    *)sp, (mlib_u16 *)dp, dsize, weight);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_U16_A8D2X4((mlib_u16
				    *)sp, strides, (mlib_u16 *)dp, strided,
				    width, height, weight);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageColorRGB2Mono_U16_D1((mlib_u16 *)sp,
				    (mlib_u16 *)dp, dsize, weight);
			} else {
				mlib_v_ImageColorRGB2Mono_U16((mlib_u16 *)sp,
				    strides, (mlib_u16 *)dp, strided, width,
				    height, weight);
			}
		} else {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_GEN_U16_A8D1X4(
				    (mlib_u16 *)sp, (mlib_u16 *)dp, dsize,
				    weight);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {
				mlib_v_ImageColorRGB2Mono_GEN_U16_A8D2X4(
				    (mlib_u16 *)sp, strides, (mlib_u16 *)dp,
				    strided, width, height, weight);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageColorRGB2Mono_GEN_U16_D1((mlib_u16
				    *)sp, (mlib_u16 *)dp, dsize, weight);
			} else {
				mlib_v_ImageColorRGB2Mono_GEN_U16((mlib_u16 *)
				    sp, strides, (mlib_u16 *)dp, strided, width,
				    height, weight);
			}
		}

		break;
	case MLIB_INT:
		mlib_v_ImageColorRGB2Mono_S32(sp, strides / 4, dp, strided / 4,
		    width, height, weight);
		break;
	case MLIB_BIT:
	default:
/* MLIB_BIT is not supported here */
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2CIEMono(
    mlib_image *dst,
    const mlib_image *src)
{
/* blending coefficients for CIE */
	mlib_d64 weight[3] = { 0.2125, 0.7154, 0.0721 };

	return (__mlib_ImageColorRGB2Mono(dst, src, weight));
}

/* *********************************************************** */
