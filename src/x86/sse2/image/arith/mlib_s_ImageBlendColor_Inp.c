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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_s_ImageBlendColor_Inp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlendColor_Inp - blend an image and a color
 *                             with the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlendColor_Inp(mlib_image       *srcdst,
 *                                           const mlib_s32   *color,
 *                                           mlib_s32         cmask);
 *
 * ARGUMENT
 *      srcdst     pointer to output image
 *      color   array of constant color components
 *      cmask   Channel mask to indicate the alpha channel.
 *              Each bit of cmask represents a channel in the image.
 *              The channel corresponding to the highest bit with
 *              value 1 is the alpha channel.
 *
 * RESTRICTION
 *      srcdst can has two through four channels.It can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type. For MLIB_BYTE images,
 *      alpha coefficients are in Q8 format. For MLIB_SHORT images,
 *      alpha coefficients are in Q15 format and must be positive. For
 *      MLIB_USHORT images, alpha coefficients are in Q16 format.For
 *      MLIB_INT images, alpha coefficients are in Q31 format and must be
 *      positive. The length of color array must be not less than the
 *      number of channels in the images.
 *
 * DESCRIPTION
 *      Blend an image and a color with alpha channel.
 *
 *          dst[y][x][c] = srcdst[y][x][a] * srcdst[y][x][c]
 *                       + (1.0 - srcdst[y][x][a]) * color[c]
 *
 *      where c != a, otherwise
 *
 *          srcdst[y][x][a] = 1.0
 *
 *      Parameter cmask specifies which channel in the image is the alpha
 *      channel. The values in the alpha channel of the destination image
 *      is set to 1.0 to specify that all pixels in the image are fully
 *      opaque. For images of integral data types, the alpha value 1.0 is
 *      equivalent to the maximum value of the data type, e.g. MLIB_U8_MAX
 *      for an MLIB_BYTE image.
 *
 *      If any color[c] is out of range for the given image data type
 *      it will be saturated to the range prior to calculations.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageBlendColor_Inp.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlendColor_Inp = __mlib_ImageBlendColor_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlendColor_Inp) mlib_ImageBlendColor_Inp
    __attribute__((weak, alias("__mlib_ImageBlendColor_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SATUR_COLOR(TT)                                         \
	for (k = 0; k < channel; k++) {                         \
	    mlib_s32 col = color[k];                            \
	                                                        \
	    if (col >= MLIB_##TT##_MAX)                         \
		col = MLIB_##TT##_MAX;                          \
	    if (col <= MLIB_##TT##_MIN)                         \
		col = MLIB_##TT##_MIN;                          \
	    color1[k] = col;                                    \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageBlendColor_Inp(
    mlib_image *srcdst,
    const mlib_s32 *color,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, sstride, dstride;
	void *ddata;
	mlib_type type;
	mlib_s32 alpha, t_sh, k;
	mlib_s32 color1[4];

	MLIB_IMAGE_CHECK(srcdst);

	if (color == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(srcdst, type, channel, width, height, dstride,
	    ddata);

	if ((channel < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; !(cmask & 0x1); alpha++)
		cmask >>= 1;
	if (cmask >> 1)
		return (MLIB_OUTOFRANGE);

	alpha = channel - alpha - 1;

	if (alpha < 0 || alpha >= channel)
		return (MLIB_OUTOFRANGE);

	if (type == MLIB_BYTE) {

		SATUR_COLOR(U8);

		if (channel == 4) {
			mlib_s_ImageBlendColor_Inp_U8_4(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		} else if (channel == 3) {
			mlib_s_ImageBlendColor_Inp_U8_3(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		} else if (channel == 2) {
			mlib_s_ImageBlendColor_Inp_U8_2(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		}

		return (MLIB_SUCCESS);

	} else if (type == MLIB_SHORT) {

		SATUR_COLOR(S16);

		sstride >>= 1;
		dstride >>= 1;

		if (channel == 4) {
			mlib_s_ImageBlendColor_Inp_S16_4(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);

		} else if (channel == 3) {
			mlib_s_ImageBlendColor_Inp_S16_3(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		} else if (channel == 2) {

			mlib_s_ImageBlendColor_Inp_S16_2(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		}

		return (MLIB_SUCCESS);

	} else if (type == MLIB_USHORT) {

		SATUR_COLOR(U16);

		sstride >>= 1;
		dstride >>= 1;

		if (channel == 4) {
			mlib_s_ImageBlendColor_Inp_U16_4(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		} else if (channel == 3) {
			mlib_s_ImageBlendColor_Inp_U16_3(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		} else if (channel == 2) {
			mlib_s_ImageBlendColor_Inp_U16_2(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color1);
		}

		return (MLIB_SUCCESS);

	} else if (type == MLIB_INT) {

		sstride >>= 2;
		dstride >>= 2;

		if (channel == 4) {
			mlib_s_ImageBlendColor_Inp_S32_4(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color);
		} else if (channel == 3) {
			mlib_s_ImageBlendColor_Inp_S32_3(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color);
		} else if (channel == 2) {
			mlib_s_ImageBlendColor_Inp_S32_2(
					ddata, ddata, dstride, dstride,
					width, height, channel, alpha, color);
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}
/* *********************************************************** */
