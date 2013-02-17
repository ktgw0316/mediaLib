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

#pragma ident	"@(#)mlib_c_ImageMulShift.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMulShift - multiply two images with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMulShift(mlib_image       *dst,
 *                                     const mlib_image *src1,
 *                                     const mlib_image *src2,
 *                                     mlib_s32         shift);
 *
 * ARGUMENT
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      dst     pointer to output image
 *      shift   right shifting factor
 *
 * NOTE
 *      ranges of valid shift are:
 *           0 <= shift <= 11 for MLIB_BYTE  type of image
 *           0 <= shift <= 16 for MLIB_SHORT type of image
 *           0 <= shift <= 16 for MLIB_USHORT type of image
 *           0 <= shift <= 31 for MLIB_INT   type of image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst = (src1 * src2) >> shift
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMulShift = __mlib_ImageMulShift

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMulShift) mlib_ImageMulShift
    __attribute__((weak, alias("__mlib_ImageMulShift")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(X)                                      \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <         \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_ImageMulShift(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_type dtype;
	mlib_u8 *sa1, *sa2, *da;
	mlib_s32 slb1, slb2, dlb, nchan, width, height;
	mlib_d64 dscale;
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, width, height, dlb, da);
	slb1 = mlib_ImageGetStride(src1);
	sa1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa2 = mlib_ImageGetData(src2);

	width *= nchan;

	if (dtype == MLIB_BYTE) {
		mlib_f32 scale;

		if (shift < 0 || shift > 11)
			return (MLIB_OUTOFRANGE);

/* dscale = 2 ** (23-shift) */
		SCALBN(dscale, 23 - shift);
		scale = (mlib_f32)dscale;

		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				da[i] = FLOAT2INT_CLAMP(mlib_U82F32[sa1[i]] *
				    mlib_U82F32[sa2[i]] * scale) >> 23;
			}

			sa1 += slb1;
			sa2 += slb2;
			da += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_SHORT) {
		mlib_f32 scale;

		if (shift < 0 || shift > 16)
			return (MLIB_OUTOFRANGE);

/* dscale = 2 ** (16-shift) */
		SCALBN(dscale, 16 - shift);
		scale = (mlib_f32)dscale;

		for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				((mlib_s16 *)da)[i] =
				    FLOAT2INT_CLAMP((mlib_f32)((mlib_s16 *)
				    sa1)[i] * scale *
				    (mlib_f32)((mlib_s16 *)sa2)[i]) >> 16;
			}

			sa1 += slb1;
			sa2 += slb2;
			da += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_USHORT) {
		mlib_f32 scale;

		if (shift < 0 || shift > 16)
			return (MLIB_OUTOFRANGE);

/* dscale = 2 ** (16-shift) */
		SCALBN(dscale, 16 - shift);
		scale = (mlib_f32)dscale;

		for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				mlib_f32 f32 =
				    (mlib_f32)((mlib_u16 *)sa1)[i] * scale *
				    (mlib_f32)((mlib_u16 *)sa2)[i];
				f32 -= (mlib_f32)0x80000000;
				((mlib_u16 *)da)[i] =
				    (FLOAT2INT_CLAMP(f32) >> 16) ^ 0x8000;
			}

			sa1 += slb1;
			sa2 += slb2;
			da += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_INT) {

		if (shift < 0 || shift > 31)
			return (MLIB_OUTOFRANGE);

/* dscale = 2 ** (-shift) */
		SCALBN(dscale, -shift);

		for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				((mlib_s32 *)da)[i] =
				    FLOAT2INT_CLAMP((mlib_d64)((mlib_s32 *)
				    sa1)[i] * dscale *
				    (mlib_d64)((mlib_s32 *)sa2)[i]);
			}

			sa1 += slb1;
			sa2 += slb2;
			da += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
