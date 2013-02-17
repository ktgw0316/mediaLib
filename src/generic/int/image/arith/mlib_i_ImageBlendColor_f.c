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

#pragma ident   "@(#)mlib_i_ImageBlendColor_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlendColor - blend an image and a color
 *                             with the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlendColor(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_s32   *color,
 *                                       mlib_s32         cmask);
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
 *      channels. They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type. For MLIB_BYTE images, alpha coefficients
 *      are in Q8 format. For MLIB_SHORT images, alpha coefficients are
 *      in Q15 format and must be positive. For MLIB_USHORT images,
 *      alpha coefficients are in Q16 format. For MLIB_INT images, alpha
 *      coefficients are in Q31 format and must be positive. The length
 *      of color array must be not less than the number of channels in
 *      the images.
 *
 * DESCRIPTION
 *      Blend an image and a color with alpha channel.
 *
 *          dst[y][x][c] = src[y][x][a] * src[y][x][c]
 *                       + (1.0 - src[y][x][a]) * color[c]
 *
 *      where c != a, otherwise
 *
 *          dst[y][x][a] = 1.0
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
#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#define	COLOR_TYPE	mlib_s32

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_U8
#define	SHIFT	8
#define	MAX_VALUE	((1u << SHIFT) - 1)
#define	DTYPE	mlib_u8
#define	TTYPE	mlib_s32
#define	FTYPE	mlib_s32

/* *********************************************************** */

#define	BLEND_COLOR(src, color)                                 \
	(color << SHIFT) + fa * (src -  color) >> SHIFT

#include <mlib_i_ImageBlendColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_S16
#define	SHIFT	15
#define	MAX_VALUE	((1u << SHIFT) - 1)
#define	DTYPE	mlib_s16
#define	TTYPE	mlib_s32
#define	FTYPE	mlib_s64

/* *********************************************************** */

#define	BLEND_COLOR(src, color)                                 \
	(color << SHIFT) + (fa & 0x7FFF) * (src -  color) >> SHIFT

#include <mlib_i_ImageBlendColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_U16
#define	SHIFT	16
#define	MAX_VALUE	((1u << SHIFT) - 1)
#define	DTYPE	mlib_u16
#define	TTYPE	mlib_s32
#define	FTYPE	mlib_s64

/* *********************************************************** */

#define	BLEND_COLOR(src, color)                                   \
	(color << SHIFT) + (fa * (src - color)) >> SHIFT

#include <mlib_i_ImageBlendColor.h>

/* *********************************************************** */
#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#define	COLOR_TYPE	mlib_s32

/* *********************************************************** */

#define	TBL(x)	mlib_U82F32[x]
#define	LD_ALPHA(x)	mlib_c_blend_Q8[x]

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_U8
#define	SHIFT	8
#define	MAX_VALUE	((1u << SHIFT) - 1)
#define	ONE	1.0
#define	DTYPE	mlib_u8
#define	TTYPE	mlib_f32
#define	FTYPE	mlib_f32

/* *********************************************************** */

#define	BLEND_COLOR(src, color)                                 \
	(mlib_s32)(fa * src + (fone - fa) * color)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_S16
#define	SHIFT	15
#define	MAX_VALUE	((1u << SHIFT) - 1)
#define	ONE	1.0/(1 << SHIFT)
#define	DTYPE	mlib_s16
#define	TTYPE	mlib_s32
#define	FTYPE	mlib_d64

/* *********************************************************** */

#define	BLEND_COLOR(src, color)                                 \
	color + (mlib_s32)((fa & 0x7FFF) * (mlib_d64)(src -     \
	    color) * fone)

#include <mlib_ImageBlendColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageBlendColor_U16
#define	SHIFT	16
#define	MAX_VALUE	((1u << SHIFT) - 1)
#define	ONE	1.0/(1 << SHIFT)
#define	DTYPE	mlib_u16
#define	TTYPE	mlib_s32
#define	FTYPE	mlib_d64

/* *********************************************************** */

#define	BLEND_COLOR(src, color)                                   \
	color + (mlib_s32)(fa * (mlib_d64)(src - color) * fone)

#include <mlib_ImageBlendColor.h>

#endif
/* *********************************************************** */
