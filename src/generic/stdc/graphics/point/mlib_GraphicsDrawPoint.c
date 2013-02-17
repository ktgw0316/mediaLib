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

#pragma ident	"@(#)mlib_GraphicsDrawPoint.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawPoint_[8|32]   - draw point at (x, y) in color c
 *    mlib_GraphicsDrawPoint_X_[8|32] - Xor point at (x, y)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawPoint_[8|32]
 *                                             (mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   c);
 *    mlib_status mlib_GraphicsDrawPoint_X_[8|32]
 *                                             (mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   c,
 *                                              mlib_s32   c2);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       X coordinate of the point.
 *    y       Y coordinate of the point.
 *    c       Color used in the drawing.
 *    c2      The second color.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 *    In XOR rendering mode, a second color c2 is used in addition to
 *    the drawing color c. When a primitive is rendered, pixels which are
 *    original in color c in drawable buffer will be changer to color c2
 *    and vice versa. Pixels of colors other than those will be changed
 *    unpredictable, but reversible manner.
 */

#include <mlib_graphics.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawPoint_32 = __mlib_GraphicsDrawPoint_32
#pragma weak mlib_GraphicsDrawPoint_8 = __mlib_GraphicsDrawPoint_8
#pragma weak mlib_GraphicsDrawPoint_X_32 = __mlib_GraphicsDrawPoint_X_32
#pragma weak mlib_GraphicsDrawPoint_X_8 = __mlib_GraphicsDrawPoint_X_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawPoint_32) mlib_GraphicsDrawPoint_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPoint_32")));
__typeof__(__mlib_GraphicsDrawPoint_8) mlib_GraphicsDrawPoint_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPoint_8")));
__typeof__(__mlib_GraphicsDrawPoint_X_32) mlib_GraphicsDrawPoint_X_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPoint_X_32")));
__typeof__(__mlib_GraphicsDrawPoint_X_8) mlib_GraphicsDrawPoint_X_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPoint_X_8")));

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
__mlib_GraphicsDrawPoint_8(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 c)
{
	mlib_s32 x = xx;
	mlib_s32 y = yy;
	mlib_u8 *buf_data;
	mlib_s32 buf_stride;

	CHECK_WITHOUT_TEST;

	buf_data[buf_stride * y + x] = c;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPoint_32(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 c)
{
	mlib_s32 x = xx;
	mlib_s32 y = yy;
	mlib_s32 *buf_data;
	mlib_s32 buf_stride;

	CHECK_WITHOUT_TEST;

	buf_data[(buf_stride >> 2) * y + x] = c | 0xFF000000;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPoint_X_8(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 c,
	mlib_s32 c2)
{
	mlib_s32 x = xx;
	mlib_s32 y = yy;
	mlib_u8 *buf_data, cx = c ^ c2;
	mlib_s32 buf_stride;

	CHECK_WITHOUT_TEST;

	buf_data[buf_stride * y + x] ^= cx;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPoint_X_32(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 c,
	mlib_s32 c2)
{
	mlib_s32 x = xx;
	mlib_s32 y = yy;
	mlib_s32 *buf_data, cx = (c ^ c2) &~ 0xFF000000;
	mlib_s32 buf_stride;

	CHECK_WITHOUT_TEST;

	buf_data[(buf_stride >> 2) * y + x] ^= cx;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
