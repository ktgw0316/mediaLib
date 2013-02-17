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

#pragma ident	"@(#)mlib_c_ImageReplaceColor.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageReplaceColor - replace a color in the image with
 *                               another color.
 *      mlib_ImageReplaceColor_Inp - replace a color in the image with
 *                                   another color.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageReplaceColor(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         const mlib_s32   *color1,
 *                                         const mlib_s32   *color2);
 *      mlib_status mlib_ImageReplaceColor_Inp(mlib_image     *srcdst,
 *                                             const mlib_s32 *color1,
 *                                             const mlib_s32 *color2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      srcdst  pointer to input and output image
 *      color1  array of color to find in the source image
 *      color2  array of color to replace color1 with
 *
 * RESTRICTION
 *      src and dst must be of the same type, the same size and
 *      the same number of channels. They can have one through four
 *      channels. They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type. The length of color array must
 *      be not less than the number of channels in the images.
 *
 *      srcdst can has one through four channels. It can be in
 *      MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      The length of color array must be not less than the number
 *      of channels in the image.
 *
 * DESCRIPTION
 *      Function scans the source image for all pixels with color value
 *      equal to color1 and replaces these pixels with color2.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageReplaceColor = __mlib_ImageReplaceColor
#pragma weak mlib_ImageReplaceColor_Inp = __mlib_ImageReplaceColor_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageReplaceColor) mlib_ImageReplaceColor
    __attribute__((weak, alias("__mlib_ImageReplaceColor")));
__typeof__(__mlib_ImageReplaceColor_Inp) mlib_ImageReplaceColor_Inp
    __attribute__((weak, alias("__mlib_ImageReplaceColor_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	COLOR_TYPE	mlib_s32

#define	TTYPE	mlib_s32

/* *********************************************************** */

#define	GET_CONST(N)                                            \
	COLOR_TYPE a##N = color1[N];                            \
	COLOR_TYPE b##N = color2[N]

#define	MASK(N)	((-(s##N ^ a##N)) >> 31)

#define	PROCESS_CH(N)	s##N + ((b##N - s##N) &~ MASK(N))

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageReplaceColor_U8
#define	DTYPE	mlib_u8

#include <mlib_ImageReplaceColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageReplaceColor_U16
#define	DTYPE	mlib_u16

#include <mlib_ImageReplaceColor.h>

/* *********************************************************** */

#undef  TTYPE
#undef  PROCESS_CH

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageReplaceColor_S32
#define	DTYPE	mlib_s32

/* *********************************************************** */

#ifdef _NO_LONGLONG

#define	TTYPE	mlib_s32

/* *********************************************************** */

#define	PROCESS_CH(N)	(s##N == a##N) ? b##N : s##N

#else /* _NO_LONGLONG */

#define	TTYPE	mlib_s64

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	s##N + ((b##N - s##N) & ~(((s##N -                      \
	    a##N) | (a##N - s##N)) >> 63))

#endif /* _NO_LONGLONG */

#include <mlib_ImageReplaceColor.h>

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *color1,
    const mlib_s32 *color2)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	mlib_s32 col1[4];
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 i, t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color1 == NULL)
		return (MLIB_NULLPOINTER);
	if (color2 == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if (type == MLIB_BYTE) {
		t_sh = 0;
	} else if (type == MLIB_INT) {
		t_sh = 2;
	} else {
		t_sh = 1;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == nchan * width) {
		width *= height;
		height = 1;
	}

	switch (type) {
	case MLIB_BYTE:
		for (i = 0; i < nchan; i++) {
			if (color1[i] & ~0xFF) {
				col1[i] = 0x100;
			} else {
				col1[i] = color1[i];
			}
		}

		return mlib_ImageReplaceColor_U8(sdata, ddata, sstride, dstride,
		    width, height, nchan, col1, color2);

	case MLIB_SHORT:
		for (i = 0; i < nchan; i++) {
			if (color1[i] > MLIB_S16_MAX ||
			    color1[i] < MLIB_S16_MIN) {
				col1[i] = 0x10000;
			} else {
				col1[i] = color1[i] & 0xFFFF;
			}
		}

		return mlib_ImageReplaceColor_U16(sdata, ddata, sstride,
		    dstride, width, height, nchan, col1, color2);

	case MLIB_USHORT:
		for (i = 0; i < nchan; i++) {
			if (color1[i] & ~0xFFFF) {
				col1[i] = 0x10000;
			} else {
				col1[i] = color1[i];
			}
		}

		return mlib_ImageReplaceColor_U16(sdata, ddata, sstride,
		    dstride, width, height, nchan, col1, color2);

	case MLIB_INT:
		return mlib_ImageReplaceColor_S32(sdata, ddata, sstride,
		    dstride, width, height, nchan, color1, color2);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Inp(
    mlib_image *srcdst,
    const mlib_s32 *color1,
    const mlib_s32 *color2)
{
	return (__mlib_ImageReplaceColor(srcdst, srcdst, color1, color2));
}

/* *********************************************************** */
