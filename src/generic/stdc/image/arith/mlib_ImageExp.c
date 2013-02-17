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

#pragma ident	"@(#)mlib_ImageExp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageExp           - compute the exponential of the pixel values
 *      mlib_ImageExp_Inp
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageExp(mlib_image       *dst,
 *                                const mlib_image *src)
 *      mlib_status mlib_ImageExp_Inp(mlib_image *srcdst)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = e^src[x][y][c]
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = e^srcdst[x][y][c]
 *
 *      where, e = 2.718...
 *
 *      The result will be rounded and clamped as needed.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageExp = __mlib_ImageExp
#pragma weak mlib_ImageExp_Inp = __mlib_ImageExp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageExp) mlib_ImageExp
    __attribute__((weak, alias("__mlib_ImageExp")));
__typeof__(__mlib_ImageExp_Inp) mlib_ImageExp_Inp
    __attribute__((weak, alias("__mlib_ImageExp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u8 mlib_tbl_expU8[] = {
	1, 3, 7, 20, 55, 148, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255
};

/* *********************************************************** */

static const mlib_s32 mlib_tbl_expS32[] = {
	0x00000001, 0x00000003, 0x00000007, 0x00000014, 0x00000037, 0x00000094,
	    0x00000193, 0x00000449,
	0x00000ba5, 0x00001fa7, 0x0000560a, 0x0000e9e2, 0x00027bc3, 0x0006c02d,
	    0x001259ac, 0x0031e199,
	0x0087975f, 0x01709349, 0x03e9e441, 0x0aa36c7d, 0x1ceb088b, 0x4e9b87f6,
	    0x7fffffff, 0x7fffffff,
	0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff,
	    0x7fffffff, 0x7fffffff
};

/* *********************************************************** */

mlib_status
__mlib_ImageExp(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_s32 nchan, xsize, ysize, dsize, dlb, slb;
	mlib_u8 *lut[4] = { (void *)mlib_tbl_expU8,
		(void *)mlib_tbl_expU8,
		(void *)mlib_tbl_expU8,
		(void *)mlib_tbl_expU8
	};
	mlib_u8 *pdst, *psrc;
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);
	xsize *= nchan;

	if (type == MLIB_BYTE) {
		return (__mlib_ImageLookUp(dst, src, (const void **)lut));
	}

	if (type == MLIB_SHORT || type == MLIB_USHORT)
		dsize = xsize * 2;
	else if (type == MLIB_INT)
		dsize = xsize * 4;
	else
		return (MLIB_FAILURE);

	if ((dsize == slb) && (dsize == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

	if (type == MLIB_SHORT) {

		for (j = 0; j < ysize; j++) {
			mlib_s16 *sp = (mlib_s16 *)psrc;
			mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_s32 s, d;

				s = sp[i];
				d = (mlib_tbl_expS32[s & 0x1F] | ((10 -
				    s) >> 31)) & ~(s >> 31);
				dp[i] = d & 0x7FFF;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (type == MLIB_USHORT) {

		for (j = 0; j < ysize; j++) {
			mlib_u16 *sp = (mlib_u16 *)psrc;
			mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_s32 s, d;

				s = sp[i];
				d = (mlib_tbl_expS32[s & 0x1F] | ((11 -
				    s) >> 31));
				dp[i] = d;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (type == MLIB_INT) {

		for (j = 0; j < ysize; j++) {
			mlib_s32 *sp = (mlib_s32 *)psrc;
			mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_s32 s, d;

				s = sp[i];
				d = (mlib_tbl_expS32[s & 0x1F] | ((30 -
				    s) >> 31)) & ~(s >> 31);
				dp[i] = d & 0x7FFFFFFF;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageExp_Inp(
    mlib_image *srcdst)
{
	return (__mlib_ImageExp(srcdst, srcdst));
}

/* *********************************************************** */
