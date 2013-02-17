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

#pragma ident	"@(#)mlib_GraphicsDrawPoint_B.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawPoint_B_[8|32] - draw point with alpha blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawPoint_B_[8|32]
 *                                             (mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   c,
 *                                              mlib_s32   a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       X coordinate of the point.
 *    y       Y coordinate of the point.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image.
 *
 *    Pixels falling outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 */

#include <mlib_graphics.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawPoint_B_32 = __mlib_GraphicsDrawPoint_B_32
#pragma weak mlib_GraphicsDrawPoint_B_8 = __mlib_GraphicsDrawPoint_B_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawPoint_B_32) mlib_GraphicsDrawPoint_B_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPoint_B_32")));
__typeof__(__mlib_GraphicsDrawPoint_B_8) mlib_GraphicsDrawPoint_B_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPoint_B_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_NOTVISIBLE	MLIB_SUCCESS

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_stride = mlib_ImageGetStride(buffer);               \
	buf_data = mlib_ImageGetData(buffer);                   \
	if ((x < 0) || (y < 0))                                 \
	    return (MLIB_NOTVISIBLE);                           \
	if (((mlib_ImageGetWidth(buffer)) <= x) ||              \
		((mlib_ImageGetHeight(buffer)) <= y))           \
	    return MLIB_NOTVISIBLE

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPoint_B_8(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 c,
	mlib_s32 a)
{
	mlib_s32 x = xx;
	mlib_s32 y = yy;
	mlib_u8 *buf_data;
	mlib_s32 buf_stride, s;
	mlib_d64 da, ds, dc;

	CHECK_WITHOUT_TEST;

	s = buf_data[buf_stride * y + x];
	da = mlib_U82D64[a & 0xff] * (1./255.);
	dc = mlib_U82D64[c & 0xff];
	ds = mlib_U82D64[s];

	buf_data[buf_stride * y + x] = ds + da * (dc - ds);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPoint_B_32(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 c,
	mlib_s32 a)
{
	mlib_s32 x = xx;
	mlib_s32 y = yy;
	mlib_s32 *buf_data, *bdptr, s;
	mlib_s32 buf_stride;
	mlib_d64 da;
	mlib_d64 cb1, cb2, cb3;
	mlib_d64 sb1, sb2, sb3;
	mlib_s32 res;

	CHECK_WITHOUT_TEST;

	da = mlib_U82D64[a & 0xff] * (1./255.);

	buf_stride >>= 2;

	bdptr = &buf_data[buf_stride * y + x];
	s = *bdptr;

	res = (mlib_s32)0xff000000;

	cb1 = mlib_U82D64[(c >> 16) & 0xff];
	sb1 = mlib_U82D64[(s >> 16) & 0xff];

	cb2 = mlib_U82D64[(c >> 8) & 0xff];
	sb2 = mlib_U82D64[(s >> 8) & 0xff];

	cb3 = mlib_U82D64[c & 0xff];
	sb3 = mlib_U82D64[s & 0xff];

	res |=  (mlib_s32)(sb1 + da * (cb1 - sb1)) << 16;
	res |=  (mlib_s32)(sb2 + da * (cb2 - sb2)) << 8;
	res |=  (mlib_s32)(sb3 + da * (cb3 - sb3));
	*bdptr = res;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
