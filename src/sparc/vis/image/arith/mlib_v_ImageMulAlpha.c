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

#pragma ident	"@(#)mlib_v_ImageMulAlpha.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMulAlpha - multiply color channels by the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMulAlpha(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         cmask)
 *
 * ARGUMENTS
 *    dst     Pointer to destination image.
 *    src     Pointer to source image.
 *    cmask   Channel mask to indicate the alpha channel.
 *            Each bit of which represents a channel in the image. The
 *            channel corresponded to the 1 bit is the alpha channel.
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Multiply color channels by the alpha channel in the way of
 *      pixel by pixel.
 *
 *  The image must have at least two channels.
 *
 *  For an MLIB_BYTE image
 *   dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -8)
 *  For an MLIB_SHORT image
 *   dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -15)
 *  For an MLIB_USHORT image
 *   dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -16)
 *  For an MLIB_INT image
 *   dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -31)
 *  where, j != a
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageMulAlpha.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMulAlpha = __mlib_ImageMulAlpha

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMulAlpha) mlib_ImageMulAlpha
    __attribute__((weak, alias("__mlib_ImageMulAlpha")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMulAlpha(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 alpha, t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

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

	if (type == MLIB_BYTE) {
		t_sh = 0;
	} else if (type == MLIB_INT) {
		t_sh = 2;
	} else {
		t_sh = 1;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == channel * width) {
		width *= height;
		height = 1;
	}

	switch (type) {
	case MLIB_BYTE:
		return mlib_ImageMulAlpha_U8(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha);

	case MLIB_SHORT:
		return mlib_ImageMulAlpha_S16(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha);

	case MLIB_USHORT:
		return mlib_ImageMulAlpha_U16(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha);

	case MLIB_INT:
		return mlib_ImageMulAlpha_S32(sdata, ddata, sstride, dstride,
		    width, height, channel, alpha);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
