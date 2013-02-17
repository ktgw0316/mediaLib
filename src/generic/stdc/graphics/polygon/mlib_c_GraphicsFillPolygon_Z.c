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

#pragma ident	"@(#)mlib_c_GraphicsFillPolygon_Z.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_Z_[8|32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_Z_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Color used in the drawing.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 */

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_BZ_[8|32] - draw filled polygon enclose
 *                                     (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_BZ_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c,
 *                                       mlib_s32       a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image. The Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer.
 *
 *    Pixels fall outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *    d = (s * (255 - a) + c * a) / 255
 */

#include <stdlib.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_Z_32 = __mlib_GraphicsFillPolygon_Z_32
#pragma weak mlib_GraphicsFillPolygon_Z_8 = __mlib_GraphicsFillPolygon_Z_8

#pragma weak mlib_GraphicsFillPolygon_BZ_32 = \
	__mlib_GraphicsFillPolygon_BZ_32
#pragma weak mlib_GraphicsFillPolygon_BZ_8 = __mlib_GraphicsFillPolygon_BZ_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_Z_32) mlib_GraphicsFillPolygon_Z_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_Z_32")));
__typeof__(__mlib_GraphicsFillPolygon_Z_8) mlib_GraphicsFillPolygon_Z_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_Z_8")));

__typeof__(__mlib_GraphicsFillPolygon_BZ_32) mlib_GraphicsFillPolygon_BZ_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BZ_32")));
__typeof__(__mlib_GraphicsFillPolygon_BZ_8) mlib_GraphicsFillPolygon_BZ_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BZ_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 z;
} SpanData, *SpanDataPtr;

/* *********************************************************** */

static mlib_s32 mlib_CountExtremaAndMinMax_Z(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax);

static void mlib_ComputeSpansData_Z(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataPtr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans);

/* *********************************************************** */

#define	MLIB_SHIFT	16

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_8(
    mlib_u8 *buf_data,
    mlib_u8 *zbuf_data,
    mlib_s32 xs1,
    mlib_s32 xs2,
    mlib_s32 c,
    mlib_s32 zstart,
    mlib_s32 zstep)
{
	mlib_u8 *dPtr = ((mlib_u8 *)buf_data) + xs1;
	mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xs1;
	mlib_s32 zb1, zc1;
	mlib_s32 count = xs2 - xs1 + 1;
	mlib_s32 i = 0;

	for (; i < count; i++) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			*dPtr = c;
		}

		dPtr++;
		zPtr++;
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_8(
    mlib_u8 *buf_data,
    mlib_u8 *zbuf_data,
    mlib_s32 xs1,
    mlib_s32 xs2,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 zstart,
    mlib_s32 zstep)
{
	mlib_u8 *dPtr = ((mlib_u8 *)buf_data) + xs1;
	mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xs1;
	mlib_s32 zb1, zc1;
	mlib_s32 count = xs2 - xs1 + 1;
	mlib_s32 i = 0;

	MLIB_GRAPHICS_DECL_VAR_B_8;
	MLIB_GRAPHICS_SET_VAR_B_8(c, a);

	for (; i < count; i++) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			MLIB_GRAPHICS_BLEND_8(dPtr);
		}

		dPtr++;
		zPtr++;
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_8_AA(
    mlib_u8 *buf_data,
    mlib_u8 *zbuf_data,
    mlib_s32 xs1,
    mlib_s32 xs2,
    mlib_s32 c,
    mlib_s32 zstart,
    mlib_s32 zstep)
{
	mlib_u8 *dPtr = ((mlib_u8 *)buf_data) + xs1;
	mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xs1;
	mlib_s32 zb, zb1, zb2, zc1, zc2;
	mlib_s32 count = xs2 - xs1 + 1;
	mlib_s32 i = 0, mask, c1;

	if ((1 & (mlib_addr)dPtr) && i < count) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			*dPtr = c;
		}

		dPtr++;
		zPtr++;
		i++;
	}

	if (i < count - 1) {
		if (2 & (mlib_addr)dPtr) {
			mask = 0;

			zb = *(mlib_s32 *)zPtr;
#ifdef _LITTLE_ENDIAN
			zb2 = (zb >> MLIB_SHIFT);
			zb1 = (mlib_s16)zb;
#else /* _LITTLE_ENDIAN */
			zb1 = (zb >> MLIB_SHIFT);
			zb2 = (mlib_s16)zb;
#endif /* _LITTLE_ENDIAN */

			zc1 = (zstart >> MLIB_SHIFT);
			zstart += zstep;
			zc2 = (zstart >> MLIB_SHIFT);
			zstart += zstep;

			if (zb1 >= zc1) {
				if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
					zb = (zc1 & 0xFFFF) +
					    (zc2 << MLIB_SHIFT);
#else /* _LITTLE_ENDIAN */
					zb = (zc2 & 0xFFFF) +
					    (zc1 << MLIB_SHIFT);
#endif /* _LITTLE_ENDIAN */
					mask = 0xFFFF;
				} else {
#ifdef _LITTLE_ENDIAN
					zb = (zb & 0xFFFF0000) + (zc1 & 0xFFFF);
					mask = 0xFF;
#else /* _LITTLE_ENDIAN */
					zb = (zb & 0xFFFF) +
					    (zc1 << MLIB_SHIFT);
					mask = 0xFF00;
#endif /* _LITTLE_ENDIAN */
				}
			} else if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
				zb = (zb & 0xFFFF) + (zc2 << MLIB_SHIFT);
				mask = 0xFF00;
#else /* _LITTLE_ENDIAN */
				zb = (zb & 0xFFFF0000U) + (zc2 & 0xFFFF);
				mask = 0xFF;
#endif /* _LITTLE_ENDIAN */
			}

			if (mask) {
				*(mlib_s32 *)zPtr = zb;

				if (mask == 0xFFFF)
					*(mlib_s16 *)dPtr = c;
				else {
					c1 = *(mlib_s16 *)dPtr;
					*(mlib_s16 *)dPtr =
					    (c1 & ~mask) + (c & mask);
				}
			}

			dPtr += 2;
			zPtr += 2;
			i += 2;
		}
	} else if (i < count) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			*dPtr = c;
		}

		i++;
	}

	for (; i < count - 3; i += 4) {
		mask = 0;

		zb = *(mlib_s32 *)zPtr;
#ifdef _LITTLE_ENDIAN
		zb2 = (zb >> MLIB_SHIFT);
		zb1 = (mlib_s16)zb;
#else /* _LITTLE_ENDIAN */
		zb1 = (zb >> MLIB_SHIFT);
		zb2 = (mlib_s16)zb;
#endif /* _LITTLE_ENDIAN */

		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;
		zc2 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
				zb = (zc1 & 0xFFFF) + (zc2 << MLIB_SHIFT);
				mask = 0x0000FFFF;
#else /* _LITTLE_ENDIAN */
				zb = (zc2 & 0xFFFF) + (zc1 << MLIB_SHIFT);
				mask = 0xFFFF0000U;
#endif /* _LITTLE_ENDIAN */
			} else {
#ifdef _LITTLE_ENDIAN
				zb = (zb & 0xFFFF0000U) + (zc1 & 0xFFFF);
				mask = 0x000000FF;
#else /* _LITTLE_ENDIAN */
				zb = (zb & 0xFFFF) + (zc1 << MLIB_SHIFT);
				mask = 0xFF000000U;
#endif /* _LITTLE_ENDIAN */
			}
		} else if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
			zb = (zb & 0xFFFF) + (zc2 << MLIB_SHIFT);
			mask = 0x0000FF00;
#else /* _LITTLE_ENDIAN */
			zb = (zb & 0xFFFF0000U) + (zc2 & 0xFFFF);
			mask = 0x00FF0000;
#endif /* _LITTLE_ENDIAN */
		}

		if (mask)
			*(mlib_s32 *)zPtr = zb;
		zPtr += 2;

		zb = *(mlib_s32 *)zPtr;
#ifdef _LITTLE_ENDIAN
		zb2 = (zb >> MLIB_SHIFT);
		zb1 = (mlib_s16)zb;
#else /* _LITTLE_ENDIAN */
		zb1 = (zb >> MLIB_SHIFT);
		zb2 = (mlib_s16)zb;
#endif /* _LITTLE_ENDIAN */

		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;
		zc2 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
				zb = (zc1 & 0xFFFF) + (zc2 << MLIB_SHIFT);
				mask |= 0xFFFF0000;
#else /* _LITTLE_ENDIAN */
				zb = (zc2 & 0xFFFF) + (zc1 << MLIB_SHIFT);
				mask |= 0x0000FFFF;
#endif /* _LITTLE_ENDIAN */
			} else {
#ifdef _LITTLE_ENDIAN
				zb = (zb & 0xFFFF0000) + (zc1 & 0xFFFF);
				mask |= 0x00FF0000;
#else /* _LITTLE_ENDIAN */
				zb = (zb & 0xFFFF) + (zc1 << MLIB_SHIFT);
				mask |= 0x0000FF00;
#endif /* _LITTLE_ENDIAN */
			}
		} else if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
			zb = (zb & 0xFFFF) + (zc2 << MLIB_SHIFT);
			mask |= 0xFF000000;
#else /* _LITTLE_ENDIAN */
			zb = (zb & 0xFFFF0000U) + (zc2 & 0xFFFF);
			mask |= 0x000000FF;
#endif /* _LITTLE_ENDIAN */
		}

		if (mask) {
#ifdef _LITTLE_ENDIAN

			if (mask & 0xFFFF0000)
#else /* _LITTLE_ENDIAN */

			if (mask & 0x0000FFFF)
#endif /* _LITTLE_ENDIAN */
				*(mlib_s32 *)zPtr = zb;

			if (mask == (mlib_s32)0xFFFFFFFFU)
				*(mlib_s32 *)dPtr = c;
			else {
				c1 = *(mlib_s32 *)dPtr;
				*(mlib_s32 *)dPtr = (c1 & ~mask) + (c & mask);
			}
		}

		zPtr += 2;
		dPtr += 4;
	}

	if (i < count - 1) {
		mask = 0;

		zb = *(mlib_s32 *)zPtr;
#ifdef _LITTLE_ENDIAN
		zb2 = (zb >> MLIB_SHIFT);
		zb1 = (mlib_s16)zb;
#else /* _LITTLE_ENDIAN */
		zb1 = (zb >> MLIB_SHIFT);
		zb2 = (mlib_s16)zb;
#endif /* _LITTLE_ENDIAN */

		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;
		zc2 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
				zb = (zc1 & 0xFFFF) + (zc2 << MLIB_SHIFT);
#else /* _LITTLE_ENDIAN */
				zb = (zc2 & 0xFFFF) + (zc1 << MLIB_SHIFT);
#endif /* _LITTLE_ENDIAN */
				mask = 0xFFFF;
			} else {
#ifdef _LITTLE_ENDIAN
				zb = (zb & 0xFFFF0000) + (zc1 & 0xFFFF);
				mask = 0xFF;
#else /* _LITTLE_ENDIAN */
				zb = (zb & 0xFFFF) + (zc1 << MLIB_SHIFT);
				mask = 0xFF00;
#endif /* _LITTLE_ENDIAN */
			}
		} else if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
			zb = (zb & 0xFFFF) + (zc2 << MLIB_SHIFT);
			mask = 0xFF00;
#else /* _LITTLE_ENDIAN */
			zb = (zb & 0xFFFF0000U) + (zc2 & 0xFFFF);
			mask = 0xFF;
#endif /* _LITTLE_ENDIAN */
		}

		if (mask) {
			*(mlib_s32 *)zPtr = zb;

			if (mask == 0xFFFF)
				*(mlib_s16 *)dPtr = c;
			else {
				c1 = *(mlib_s16 *)dPtr;
				*(mlib_s16 *)dPtr = (c1 & ~mask) + (c & mask);
			}
		}

		dPtr += 2;
		zPtr += 2;
		i += 2;
	}

	if (i < count) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			*dPtr = c;
		}
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_32(
    mlib_u8 *buf_data,
    mlib_u8 *zbuf_data,
    mlib_s32 xs1,
    mlib_s32 xs2,
    mlib_s32 c,
    mlib_s32 zstart,
    mlib_s32 zstep)
{
	mlib_s32 *dPtr = (mlib_s32 *)(buf_data) + (xs1);
	mlib_s16 *zPtr = (mlib_s16 *)(zbuf_data) + (xs1);
	mlib_s32 zb, zb1, zb2, zc1, zc2;
	mlib_s32 count = xs2 - xs1 + 1;
	mlib_s32 i = 0;

	if (i < count && (2 & (mlib_addr)zPtr)) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			*dPtr = c;
		}

		dPtr++;
		zPtr++;
		i++;
	}

	for (; i < count - 1; i += 2) {
		zb = *(mlib_s32 *)zPtr;
#ifdef _LITTLE_ENDIAN
		zb2 = (zb >> MLIB_SHIFT);
		zb1 = (mlib_s16)zb;
#else /* _LITTLE_ENDIAN */
		zb1 = (zb >> MLIB_SHIFT);
		zb2 = (mlib_s16)zb;
#endif /* _LITTLE_ENDIAN */

		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;
		zc2 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			*dPtr = c;

			if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
				zb = (zc2 << MLIB_SHIFT) + (zc1 & 0xFFFF);
#else /* _LITTLE_ENDIAN */
				zb = (zc1 << MLIB_SHIFT) + (zc2 & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
				dPtr[1] = c;
			} else
#ifdef _LITTLE_ENDIAN
				zb = (zc1 & 0xFFFF) + (zb & 0xFFFF0000);
#else /* _LITTLE_ENDIAN */
				zb = (zc1 << MLIB_SHIFT) + (zb & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
			*(mlib_s32 *)zPtr = zb;
		} else if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
			zb = (zb & 0xFFFF) + (zc2 << MLIB_SHIFT);
#else /* _LITTLE_ENDIAN */
			zb = (zb & 0xFFFF0000U) + (zc2 & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
			*(mlib_s32 *)zPtr = zb;
			dPtr[1] = c;
		}

		zPtr += 2;
		dPtr += 2;
	}

	if (i < count) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			*dPtr = c;
		}
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_32(
    mlib_u8 *buf_data,
    mlib_u8 *zbuf_data,
    mlib_s32 xs1,
    mlib_s32 xs2,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 zstart,
    mlib_s32 zstep)
{
	mlib_s32 *dPtr = (mlib_s32 *)(buf_data) + (xs1);
	mlib_s16 *zPtr = (mlib_s16 *)(zbuf_data) + (xs1);
	mlib_s32 zb, zb1, zb2, zc1, zc2;
	mlib_s32 count = xs2 - xs1 + 1;
	mlib_s32 i = 0;

	MLIB_GRAPHICS_DECL_VAR_B_32;
	MLIB_GRAPHICS_SET_VAR_B_32(c, a);

	if (i < count && (2 & (mlib_addr)zPtr)) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			MLIB_GRAPHICS_BLEND_32(dPtr);
		}

		dPtr++;
		zPtr++;
		i++;
	}

	for (; i < count - 1; i += 2) {
		zb = *(mlib_s32 *)zPtr;
#ifdef _LITTLE_ENDIAN
		zb2 = (zb >> MLIB_SHIFT);
		zb1 = (mlib_s16)zb;
#else /* _LITTLE_ENDIAN */
		zb1 = (zb >> MLIB_SHIFT);
		zb2 = (mlib_s16)zb;
#endif /* _LITTLE_ENDIAN */

		zc1 = (zstart >> MLIB_SHIFT);
		zstart += zstep;
		zc2 = (zstart >> MLIB_SHIFT);
		zstart += zstep;

		if (zb1 >= zc1) {
			MLIB_GRAPHICS_BLEND_32(dPtr);

			if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
				zb = (zc2 << MLIB_SHIFT) + (zc1 & 0xFFFF);
#else /* _LITTLE_ENDIAN */
				zb = (zc1 << MLIB_SHIFT) + (zc2 & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
				MLIB_GRAPHICS_BLEND_32(dPtr + 1);
			} else
#ifdef _LITTLE_ENDIAN
				zb = (zc1 & 0xFFFF) + (zb & 0xFFFF0000);
#else /* _LITTLE_ENDIAN */
				zb = (zc1 << MLIB_SHIFT) + (zb & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
			*(mlib_s32 *)zPtr = zb;
		} else if (zb2 >= zc2) {
#ifdef _LITTLE_ENDIAN
			zb = (zb & 0xFFFF) + (zc2 << MLIB_SHIFT);
#else /* _LITTLE_ENDIAN */
			zb = (zb & 0xFFFF0000U) + (zc2 & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
			*(mlib_s32 *)zPtr = zb;
			MLIB_GRAPHICS_BLEND_32(dPtr + 1);
		}

		zPtr += 2;
		dPtr += 2;
	}

	if (i < count) {
		zb1 = *(mlib_s16 *)zPtr;
		zc1 = (zstart >> MLIB_SHIFT);

		if (zb1 >= zc1) {
			*(mlib_s16 *)zPtr = zc1;
			MLIB_GRAPHICS_BLEND_32(dPtr);
		}
	}
}

/* *********************************************************** */

#define	BLENDING8(s)	(s * a + c) / 255;
#define	BLENDING32(s, c)	(s * a + c) / 255;

/* *********************************************************** */

#define	MLIB_05	0x8000
#define	MLIB_SIGNSHIFT	31

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_data = mlib_ImageGetData(buffer);                   \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	buf_stride = mlib_ImageGetStride(buffer);               \
	zbuf_data = mlib_ImageGetData(zbuffer);                 \
	zbuf_stride = mlib_ImageGetStride(zbuffer)

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_Z_8(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	if (yMin < 0)
		yMin = 0;

	if (yMax > buf_height)
		yMax = buf_height;

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

	c &= 0xff;
	c |= (c << 8);
	c |= (c << 16);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			if (xs2 > buf_width - 1)
				xs2 = buf_width - 1;

			if (((1 & (mlib_addr)buf_data) == 0 &&
			    (2 & (mlib_addr)zbuf_data) == 0) ||
			    ((1 & (mlib_addr)buf_data) != 0 &&
			    (2 & (mlib_addr)zbuf_data) != 0))
				MLIB_FILL_ROW_Z_8_AA(buf_data, zbuf_data, xs1,
				    xs2, c, zFixed, zFixedStep);
			else
				MLIB_FILL_ROW_Z_8(buf_data, zbuf_data, xs1, xs2,
				    c, zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BZ_8(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	if (yMin < 0)
		yMin = 0;

	if (yMax > buf_height)
		yMax = buf_height;

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			if (xs2 > buf_width - 1)
				xs2 = buf_width - 1;

			MLIB_FILL_ROW_BZ_8(buf_data, zbuf_data, xs1, xs2,
			    c, a, zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_Z_32(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	if (yMin < 0)
		yMin = 0;

	if (yMax > buf_height)
		yMax = buf_height;

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			if (xs2 > buf_width - 1)
				xs2 = buf_width - 1;

			MLIB_FILL_ROW_Z_32(buf_data, zbuf_data, xs1, xs2, c,
			    zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BZ_32(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	if (yMin < 0)
		yMin = 0;

	if (yMax > buf_height)
		yMax = buf_height;

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			if (xs2 > buf_width - 1)
				xs2 = buf_width - 1;

			MLIB_FILL_ROW_BZ_32(buf_data, zbuf_data, xs1, xs2, c, a,
			    zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * Count vertical limits of polygon and max number of intersections
 * with scan line (number of polygon extrema).
 */

static mlib_s32
mlib_CountExtremaAndMinMax_Z(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax)
{
	mlib_s32 yMin, yMax, extrNum;
	mlib_s32 i, yCur, yPrev, dir, firstDir;

	yMin = MLIB_S32_MAX;
	yMax = MLIB_S32_MIN;

	yPrev = y[npoints - 1];
	dir = 0;
	firstDir = 0;
	extrNum = 0;

	for (i = 0; i < npoints; i++) {
		yCur = y[i];

		if (yCur > yPrev) {
			if (dir < 0)
				extrNum++;
			dir = 1;

			if (firstDir == 0)
				firstDir = 1;
		} else if (yCur < yPrev) {
			if (dir > 0)
				extrNum++;
			dir = -1;

			if (firstDir == 0)
				firstDir = -1;
		}

		if (yMin > yCur)
			yMin = yCur;

		if (yMax < yCur)
			yMax = yCur;

		yPrev = yCur;
	}

	if (firstDir != dir)
		extrNum++;

	*pyMin = yMin;
	*pyMax = yMax;

	return (extrNum);
}

/* *********************************************************** */

static void
mlib_ComputeSpansData_Z(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataPtr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans)
{
	SpanDataPtr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 z1, z2, zPrev;
	mlib_s32 zFixed, zFixedStep;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++) {
		rowPtr->x = MLIB_S32_MAX;
		rowPtr++;
	}

	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		rowSpans[i] = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];
	zPrev = z[npoints - 1];

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		z1 = zPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		z2 = zPrev = z[i];

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			z1 ^= z2 ^= z1 ^= z2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		zFixed = (z1 << MLIB_SHIFT) + MLIB_05;
		zFixedStep = ((z2 - z1) << MLIB_SHIFT) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		zFixed += zFixedStep * dely;

		if (y2 > yMax)
			y2 = yMax;

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].z = zFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));
			zFixed += zFixedStep;
		}
	}
}

/* *********************************************************** */
