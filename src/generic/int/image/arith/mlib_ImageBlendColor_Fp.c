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

#pragma ident   "@(#)mlib_ImageBlendColor_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlendColor_Fp - blend an image and a color
 *                                with the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlendColor_Fp(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64   *color,
 *                                          mlib_s32         cmask);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      color   array of constant color components
 *      cmask   Channel mask to indicate the alpha channel.
 *              Each bit of cmask represents a channel in the image.
 *              The channel corresponding to the highest bit with
 *              value 1 is the alpha channel.
 *
 * RESTRICTION
 *      src and dst must be of the same type, the same size and
 *      the same number of channels. They can have two through four
 *      channels. They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *      The length of color array must be not less than the number
 *      of channels in the images.
 *
 * DESCRIPTION
 *      Blend an image and a color with alpha channel.
 *
 *          dst[y][x][c] = src[y][x][a] * src[y][x][c]
 *                         + (1.0 - src[y][x][a]) * color[c]
 *
 *      where c != a, otherwise
 *
 *          dst[y][x][a] = 1.0
 *
 *      Parameter cmask specifies which channel in the image is the alpha
 *      channel. The values in the alpha channel of the destination image
 *      is set to 1.0 to specify that all pixels in the image are fully opaque.
 *      The alpha coefficients are assumed to be in the range of [0.0, 1.0].
 *
 *      If any color[c] is out of range for the given image data type
 *      it will be saturated to the range prior to calculations.
 */

#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlendColor_Fp = __mlib_ImageBlendColor_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlendColor_Fp) mlib_ImageBlendColor_Fp
    __attribute__((weak, alias("__mlib_ImageBlendColor_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SATUR_COLOR(TT)                                         \
	for (k = 0; k < channel; k++) {                         \
	    mlib_f32 col = (mlib_f32)color[k];                  \
	                                                        \
	    if (col >= MLIB_##TT##_MAX)                         \
		col = MLIB_##TT##_MAX;                          \
	    if (col <= MLIB_##TT##_MIN)                         \
		col = MLIB_##TT##_MIN;                          \
	    color1[k] = col;                                    \
	}

/* *********************************************************** */

#define	COLOR_TYPE	mlib_d64

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_F32
#define	DTYPE	mlib_f32
#define	TTYPE	DTYPE
#define	FTYPE	DTYPE
#define	MAX_VALUE	1.0f

/* *********************************************************** */

#define	BLEND_COLOR(src, color)	color + fa*(src - color)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_D64
#define	DTYPE	mlib_d64
#define	TTYPE	DTYPE
#define	FTYPE	DTYPE
#define	MAX_VALUE	1.0

/* *********************************************************** */

#define	BLEND_COLOR(src, color)	color + fa*(src - color)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

#undef  COLOR_TYPE
#define	COLOR_TYPE	mlib_s32

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_S32
#define	SHIFT	31
#define	DTYPE	mlib_s32
#define	TTYPE	mlib_s64
#define	FTYPE	mlib_s64
#define	MAX_VALUE	MLIB_S32_MAX

/* *********************************************************** */

#define	BLEND_COLOR(src, color) \
	(color << SHIFT) + (fa & 0x7FFFFFFF) * (src - color) >> SHIFT

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

mlib_status
__mlib_ImageBlendColor_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *color,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 alpha, t_sh, k;
	mlib_f32 color1[4];

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, channel, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if ((channel < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; !(cmask & 0x1); alpha++)
		cmask >>= 1;
	if (cmask >> 1)
		return (MLIB_OUTOFRANGE);

	alpha = channel - alpha - 1;

	if (alpha < 0 || alpha >= channel)
		return (MLIB_OUTOFRANGE);

	if (type == MLIB_FLOAT) {
		t_sh = 2;
	} else {
		t_sh = 3;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == channel * width) {
		width *= height;
		height = 1;
	}

	switch (type) {
	case MLIB_FLOAT:
		SATUR_COLOR(F32);
		return mlib_ImageBlendColor_F32(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha, color);

	case MLIB_DOUBLE:
		return mlib_ImageBlendColor_D64(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha, color);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlendColor_Fp = __mlib_ImageBlendColor_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlendColor_Fp) mlib_ImageBlendColor_Fp
    __attribute__((weak, alias("__mlib_ImageBlendColor_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SATUR_COLOR(TT)                                         \
	for (k = 0; k < channel; k++) {                         \
	    mlib_f32 col = (mlib_f32)color[k];                  \
	                                                        \
	    if (col >= MLIB_##TT##_MAX)                         \
		col = MLIB_##TT##_MAX;                          \
	    if (col <= MLIB_##TT##_MIN)                         \
		col = MLIB_##TT##_MIN;                          \
	    color1[k] = col;                                    \
	}

/* *********************************************************** */

#define	COLOR_TYPE	mlib_d64

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_F32
#define	DTYPE	mlib_f32
#define	TTYPE	DTYPE
#define	FTYPE	DTYPE
#define	MAX_VALUE	1.0f

/* *********************************************************** */

#define	BLEND_COLOR(src, color)	color + fa*(src - color)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_D64
#define	DTYPE	mlib_d64
#define	TTYPE	DTYPE
#define	FTYPE	DTYPE
#define	MAX_VALUE	1.0

/* *********************************************************** */

#define	BLEND_COLOR(src, color)	color + fa*(src - color)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

#undef  COLOR_TYPE
#define	COLOR_TYPE	mlib_s32

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_S32
#define	SHIFT	31
#define	ONE	1.0/(1u << SHIFT)
#define	DTYPE	mlib_s32
#define	TTYPE	mlib_d64
#define	FTYPE	mlib_d64
#define	MAX_VALUE	MLIB_S32_MAX

#define	LD_ALPHA(x)	fone*((x) & 0x7FFFFFFF)

/* *********************************************************** */

#define	BLEND_COLOR(src, color)	(mlib_s32)(fa*src + (1.0 - fa)*color)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

mlib_status
__mlib_ImageBlendColor_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *color,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 alpha, t_sh, k;
	mlib_f32 color1[4];

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, channel, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if ((channel < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; !(cmask & 0x1); alpha++)
		cmask >>= 1;
	if (cmask >> 1)
		return (MLIB_OUTOFRANGE);

	alpha = channel - alpha - 1;

	if (alpha < 0 || alpha >= channel)
		return (MLIB_OUTOFRANGE);

	if (type == MLIB_FLOAT) {
		t_sh = 2;
	} else {
		t_sh = 3;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == channel * width) {
		width *= height;
		height = 1;
	}

	switch (type) {
	case MLIB_FLOAT:
		SATUR_COLOR(F32);
		return mlib_ImageBlendColor_F32(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha, color);

	case MLIB_DOUBLE:
		return mlib_ImageBlendColor_D64(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha, color);

	default:
		return (MLIB_FAILURE);
	}
}

#endif
/* *********************************************************** */
