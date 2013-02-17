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

#pragma ident	"@(#)mlib_c_ImageMin.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_c_ImageMin - get min pixel from two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMin(mlib_image       *dst,
 *                                const mlib_image *src1,
 *                                const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Min two images for each channel:  dst = min(src1, src2)
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMin = __mlib_ImageMin

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMin) mlib_ImageMin
    __attribute__((weak, alias("__mlib_ImageMin")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	IMAGE_MIN_FAST(s1, s2)                                  \
	((s2) + (((s1) - (s2)) & (((s1) - (s2)) >> 31)))

/* *********************************************************** */

#define	IMAGE_MIN_S32_MASK(s1, s2)                                      \
	((((s1) - (s2)) ^ (((s1) ^ (s2)) & ~((s2) ^ ((s1) - (s2))))))

/* *********************************************************** */

#define	IMAGE_MIN_S32_FAST(s1, s2)                              \
	((s2) + (((s1) - (s2)) & ((IMAGE_MIN_S32_MASK((s1),     \
	    (s2))) >> 31)))

/* *********************************************************** */

mlib_status
__mlib_ImageMin(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_type dtype;
	mlib_u8 *psrc1, *psrc2, *pdst;

/* indices for x, y  */
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(src1, dst);
	MLIB_IMAGE_FULL_EQUAL(src2, dst);

	dtype = mlib_ImageGetType(src1);
	nchan = mlib_ImageGetChannels(src1);
	xsize = mlib_ImageGetWidth(src1) * nchan;
	ysize = mlib_ImageGetHeight(src1);
	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	dlb = mlib_ImageGetStride(dst);
	psrc1 = (mlib_u8 *)mlib_ImageGetData(src1);
	psrc2 = (mlib_u8 *)mlib_ImageGetData(src2);
	pdst = (mlib_u8 *)mlib_ImageGetData(dst);

	if (dtype == MLIB_BYTE) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				pdst[i] = IMAGE_MIN_FAST(psrc1[i], psrc2[i]);
				pdst[i + 1] =
				    IMAGE_MIN_FAST(psrc1[i + 1], psrc2[i + 1]);
			}

			if (i < xsize) {
				pdst[i] = IMAGE_MIN_FAST(psrc1[i], psrc2[i]);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				((mlib_s16 *)pdst)[i] =
				    IMAGE_MIN_FAST(((mlib_s16 *)psrc1)[i],
				    ((mlib_s16 *)psrc2)[i]);
				((mlib_s16 *)pdst)[i + 1] =
				    IMAGE_MIN_FAST(((mlib_s16 *)psrc1)[i + 1],
				    ((mlib_s16 *)psrc2)[i + 1]);
			}

			if (i < xsize) {
				((mlib_s16 *)pdst)[i] =
				    IMAGE_MIN_FAST(((mlib_s16 *)psrc1)[i],
				    ((mlib_s16 *)psrc2)[i]);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				((mlib_u16 *)pdst)[i] =
				    IMAGE_MIN_FAST(((mlib_u16 *)psrc1)[i],
				    ((mlib_u16 *)psrc2)[i]);
				((mlib_u16 *)pdst)[i + 1] =
				    IMAGE_MIN_FAST(((mlib_u16 *)psrc1)[i + 1],
				    ((mlib_u16 *)psrc2)[i + 1]);
			}

			if (i < xsize) {
				((mlib_u16 *)pdst)[i] =
				    IMAGE_MIN_FAST(((mlib_u16 *)psrc1)[i],
				    ((mlib_u16 *)psrc2)[i]);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				((mlib_s32 *)pdst)[i] =
				    IMAGE_MIN_S32_FAST(((mlib_s32 *)psrc1)[i],
				    ((mlib_s32 *)psrc2)[i]);
				((mlib_s32 *)pdst)[i + 1] =
				    IMAGE_MIN_S32_FAST(((mlib_s32 *)psrc1)[i +
				    1], ((mlib_s32 *)psrc2)[i + 1]);
			}

			if (i < xsize) {
				((mlib_s32 *)pdst)[i] =
				    IMAGE_MIN_S32_FAST(((mlib_s32 *)psrc1)[i],
				    ((mlib_s32 *)psrc2)[i]);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
