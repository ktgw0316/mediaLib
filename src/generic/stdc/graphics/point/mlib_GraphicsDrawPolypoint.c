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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_GraphicsDrawPolypoint.c	9.3	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawPointSet_[8|32] - draw set of points at
 *                                       (x1, y1), (x2, y2)...(xn, yn)
 *                                       in color cn
 *    mlib_GraphicsDrawPointSet_X_[8|32] - draw set of points at
 *                                         (x1, y1), (x2, y2)...(xn, yn)
 *                                         in XOR mode
 *    mlib_GraphicsDrawPolypoint_[8|32] - draw points at
 *                                        (x1, y1), (x2, y2)...(xn, yn)
 *                                        in color cn
 *    mlib_GraphicsDrawPolypoint_X_[8|32] - draw points at
 *                                          (x1, y1), (x2, y2)...(xn, yn)
 *                                          in XOR mode
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawPointSet_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c);
 *    mlib_status mlib_GraphicsDrawPointSet_X_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c,
 *                                       mlib_s32       c2);
 *    mlib_status mlib_GraphicsDrawPolypoint_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c);
 *    mlib_status mlib_GraphicsDrawPolypoint_X_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c,
 *                                       mlib_s32       c2);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       Pointer to array of X coordinates of the points.
 *    y       Pointer to array of Y coordinates of the points.
 *    npoints Number of points in the arrays.
 *    c       Color  used in the drawing.
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
 *
 *  NOTE
 *    mlib_GraphicsDrawPolypoint* functions are analogue of corresponding
 *    mlib_GraphicsDrawPointSet* functions.
 */

#include <stdlib.h>
#include <mlib_graphics.h>
#include <mlib_ImageRowTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawPointSet_32 = __mlib_GraphicsDrawPointSet_32
#pragma weak mlib_GraphicsDrawPointSet_8 = __mlib_GraphicsDrawPointSet_8
#pragma weak mlib_GraphicsDrawPointSet_X_32 = \
	__mlib_GraphicsDrawPointSet_X_32
#pragma weak mlib_GraphicsDrawPointSet_X_8 = __mlib_GraphicsDrawPointSet_X_8
#pragma weak mlib_GraphicsDrawPolypoint_32 = __mlib_GraphicsDrawPolypoint_32
#pragma weak mlib_GraphicsDrawPolypoint_8 = __mlib_GraphicsDrawPolypoint_8
#pragma weak mlib_GraphicsDrawPolypoint_X_32 = \
	__mlib_GraphicsDrawPolypoint_X_32
#pragma weak mlib_GraphicsDrawPolypoint_X_8 = \
	__mlib_GraphicsDrawPolypoint_X_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawPointSet_32) mlib_GraphicsDrawPointSet_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPointSet_32")));
__typeof__(__mlib_GraphicsDrawPointSet_8) mlib_GraphicsDrawPointSet_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPointSet_8")));
__typeof__(__mlib_GraphicsDrawPointSet_X_32) mlib_GraphicsDrawPointSet_X_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPointSet_X_32")));
__typeof__(__mlib_GraphicsDrawPointSet_X_8) mlib_GraphicsDrawPointSet_X_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPointSet_X_8")));
__typeof__(__mlib_GraphicsDrawPolypoint_32) mlib_GraphicsDrawPolypoint_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPolypoint_32")));
__typeof__(__mlib_GraphicsDrawPolypoint_8) mlib_GraphicsDrawPolypoint_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPolypoint_8")));
__typeof__(__mlib_GraphicsDrawPolypoint_X_32) mlib_GraphicsDrawPolypoint_X_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPolypoint_X_32")));
__typeof__(__mlib_GraphicsDrawPolypoint_X_8) mlib_GraphicsDrawPolypoint_X_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPolypoint_X_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK                                                   \
	if (npoints <= 0)                                       \
	    return (MLIB_FAILURE);                              \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	if ((rtable = mlib_ImageGetRowTable(buffer)) == NULL)   \
	    rtable = mlib_ImageCreateRowTable(buffer)

/* *********************************************************** */

#ifdef _LP64
#define	MLIB_POINTER_SHIFT	3
#else /* _LP64 */
#define	MLIB_POINTER_SHIFT	2
#endif /* _LP64 */

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPointSet_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	mlib_u8 **rtable, *data0, *data1;
	mlib_s16 *xend;
	mlib_s32 buf_width, buf_height;
	mlib_s32 tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
	mlib_s32 z0, z1, z2 = 0, z3;

	CHECK;

	buf_width--;
	buf_height--;

/*
 * Theoretical minimum is less than 5 ticks per pixel, but I don't
 * know how to force compiler to make such unroll
 */

/*
 *  for(i = 0; i < npoints; i++)
 *  {
 *    ty = y[i];
 *    tx = x[i];
 *    z = (ty | (buf_height - ty) | tx | (buf_width - tx)) >> 31;
 *    ty |= z; tx |= z;
 *    data = rtable[ty];
 *    data[tx] = c;
 *  }
 *
 */

	xend = (void *)(x + npoints);

	ty0 = -1 << MLIB_POINTER_SHIFT;
	tx0 = -1;
	ty1 = -1 << MLIB_POINTER_SHIFT;
	tx1 = -1;
	ty2 = -1;
	tx2 = -1;
	ty3 = -1;
	tx3 = -1;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (; x < xend - 3; x += 4, y += 4) {
		z3 = (buf_height - ty3) | (buf_width - tx3);
		z0 = (ty2 | tx2 | z2) >> 31;
		ty2 |= z0;
		tx2 |= z0;
		ty2 <<= MLIB_POINTER_SHIFT;

		z1 = (ty3 | tx3 | z3) >> 31;
		ty3 |= z1;
		tx3 |= z1;
		ty3 <<= MLIB_POINTER_SHIFT;

		data0 = *(mlib_u8 **)((mlib_u8 *)rtable + ty0);
		data1 = *(mlib_u8 **)((mlib_u8 *)rtable + ty1);
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		data0[tx0] = c;
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		ty1 = *(mlib_s16 *)((mlib_u8 *)y + 2);
		data1[tx1] = c;
		tx1 = *(mlib_s16 *)((mlib_u8 *)x + 2);

		z2 = (buf_height - ty0) | (buf_width - tx0);
		z3 = (buf_height - ty1) | (buf_width - tx1);
		z0 = (ty0 | tx0 | z2) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		ty0 <<= MLIB_POINTER_SHIFT;

		z1 = (ty1 | tx1 | z3) >> 31;
		ty1 |= z1;
		tx1 |= z1;
		ty1 <<= MLIB_POINTER_SHIFT;

		data0 = *(mlib_u8 **)((mlib_u8 *)rtable + ty2);
		data1 = *(mlib_u8 **)((mlib_u8 *)rtable + ty3);
		ty2 = *(mlib_s16 *)((mlib_u8 *)y + 4);
		data0[tx2] = c;
		tx2 = *(mlib_s16 *)((mlib_u8 *)x + 4);
		ty3 = *(mlib_s16 *)((mlib_u8 *)y + 6);
		data1[tx3] = c;
		tx3 = *(mlib_s16 *)((mlib_u8 *)x + 6);
		z2 = (buf_height - ty2) | (buf_width - tx2);
	}

	data0 = *(mlib_u8 **)((mlib_u8 *)rtable + ty0);
	data0[tx0] = c;
	data1 = *(mlib_u8 **)((mlib_u8 *)rtable + ty1);
	data1[tx1] = c;

	z3 = (buf_height - ty3) | (buf_width - tx3);
	z0 = (ty2 | tx2 | z2) >> 31;
	ty2 |= z0;
	tx2 |= z0;
	ty2 <<= MLIB_POINTER_SHIFT;

	z1 = (ty3 | tx3 | z3) >> 31;
	ty3 |= z1;
	tx3 |= z1;
	ty3 <<= MLIB_POINTER_SHIFT;

	data0 = *(mlib_u8 **)((mlib_u8 *)rtable + ty2);
	data1 = *(mlib_u8 **)((mlib_u8 *)rtable + ty3);
	data0[tx2] = c;
	data1[tx3] = c;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; x < xend; x++, y++) {
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		z0 = (ty0 | (buf_height - ty0) | tx0 | (buf_width - tx0)) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		data0 = rtable[ty0];
		data0[tx0] = c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPointSet_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	mlib_s32 **rtable, *data0, *data1;
	mlib_s16 *xend;
	mlib_s32 buf_width, buf_height;
	mlib_s32 tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
	mlib_s32 z0, z1, z2 = 0, z3;

	CHECK;

	c |= 0xFF000000;

	buf_width--;
	buf_height--;

	xend = (void *)(x + npoints);

	ty0 = -1 << MLIB_POINTER_SHIFT;
	tx0 = -1;
	ty1 = -1 << MLIB_POINTER_SHIFT;
	tx1 = -1;
	ty2 = -1;
	tx2 = -1;
	ty3 = -1;
	tx3 = -1;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (; x < xend - 3; x += 4, y += 4) {
		z3 = (buf_height - ty3) | (buf_width - tx3);
		z0 = (ty2 | tx2 | z2) >> 31;
		ty2 |= z0;
		tx2 |= z0;
		ty2 <<= MLIB_POINTER_SHIFT;

		z1 = (ty3 | tx3 | z3) >> 31;
		ty3 |= z1;
		tx3 |= z1;
		ty3 <<= MLIB_POINTER_SHIFT;

		data0 = *(mlib_s32 **)((mlib_u8 *)rtable + ty0);
		data1 = *(mlib_s32 **)((mlib_u8 *)rtable + ty1);
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		data0[tx0] = c;
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		ty1 = *(mlib_s16 *)((mlib_u8 *)y + 2);
		data1[tx1] = c;
		tx1 = *(mlib_s16 *)((mlib_u8 *)x + 2);

		z2 = (buf_height - ty0) | (buf_width - tx0);
		z3 = (buf_height - ty1) | (buf_width - tx1);
		z0 = (ty0 | tx0 | z2) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		ty0 <<= MLIB_POINTER_SHIFT;

		z1 = (ty1 | tx1 | z3) >> 31;
		ty1 |= z1;
		tx1 |= z1;
		ty1 <<= MLIB_POINTER_SHIFT;

		data0 = *(mlib_s32 **)((mlib_u8 *)rtable + ty2);
		data1 = *(mlib_s32 **)((mlib_u8 *)rtable + ty3);
		ty2 = *(mlib_s16 *)((mlib_u8 *)y + 4);
		data0[tx2] = c;
		tx2 = *(mlib_s16 *)((mlib_u8 *)x + 4);
		ty3 = *(mlib_s16 *)((mlib_u8 *)y + 6);
		data1[tx3] = c;
		tx3 = *(mlib_s16 *)((mlib_u8 *)x + 6);
		z2 = (buf_height - ty2) | (buf_width - tx2);
	}

	data0 = *(mlib_s32 **)((mlib_u8 *)rtable + ty0);
	data0[tx0] = c;
	data1 = *(mlib_s32 **)((mlib_u8 *)rtable + ty1);
	data1[tx1] = c;

	z3 = (buf_height - ty3) | (buf_width - tx3);
	z0 = (ty2 | tx2 | z2) >> 31;
	ty2 |= z0;
	tx2 |= z0;
	ty2 <<= MLIB_POINTER_SHIFT;

	z1 = (ty3 | tx3 | z3) >> 31;
	ty3 |= z1;
	tx3 |= z1;
	ty3 <<= MLIB_POINTER_SHIFT;

	data0 = *(mlib_s32 **)((mlib_u8 *)rtable + ty2);
	data1 = *(mlib_s32 **)((mlib_u8 *)rtable + ty3);
	data0[tx2] = c;
	data1[tx3] = c;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; x < xend; x++, y++) {
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		z0 = (ty0 | (buf_height - ty0) | tx0 | (buf_width - tx0)) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		data0 = rtable[ty0];
		data0[tx0] = c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPointSet_X_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 c2)
{
	mlib_u8 **rtable, *data0;
	mlib_s16 *xend;
	mlib_s32 buf_width, buf_height;
	mlib_s32 tx0, ty0;
	mlib_s32 z0, z1;
	mlib_s32 cx = c ^ c2;

	CHECK;

	buf_width--;
	buf_height--;

	xend = (void *)(x + npoints);

	for (; x < xend; x++, y++) {
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		z1 = (buf_height - ty0) | (buf_width - tx0);
		z0 = (ty0 | tx0 | z1) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		data0 = rtable[ty0];
		data0[tx0] ^= cx;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPointSet_X_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 c2)
{
	mlib_s32 **rtable, *data0;
	mlib_s16 *xend;
	mlib_s32 buf_width, buf_height;
	mlib_s32 tx0, ty0;
	mlib_s32 z0, z1;
	mlib_s32 cx = (c ^ c2) &~ 0xFF000000;

	CHECK;

	buf_width--;
	buf_height--;

	xend = (void *)(x + npoints);

	for (; x < xend; x++, y++) {
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		z1 = (buf_height - ty0) | (buf_width - tx0);
		z0 = (ty0 | tx0 | z1) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		data0 = rtable[ty0];
		data0[tx0] ^= cx;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolypoint_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	return (__mlib_GraphicsDrawPointSet_8(buffer, x, y, npoints, c));
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolypoint_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	return (__mlib_GraphicsDrawPointSet_32(buffer, x, y, npoints, c));
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolypoint_X_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 c2)
{
	return (__mlib_GraphicsDrawPointSet_X_8(buffer, x, y, npoints, c, c2));
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolypoint_X_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 c2)
{
	return (__mlib_GraphicsDrawPointSet_X_32(buffer, x, y, npoints, c, c2));
}

/* *********************************************************** */
