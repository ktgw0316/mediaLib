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

#pragma ident	"@(#)mlib_c_VideoInterpAveX_U8_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpAveX_U8_U8_wxh,
 *      mlib_VideoInterpAveX_U8_U8     - Performs half-pixel interpolation and
 *                                       averaging in the X-direction for
 *                                       replenishment mode.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoInterpAveX_U8_U8_wxh(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride)
 *    mlib_status mlib_VideoInterpAveX_U8_U8(mlib_u8       *curr_block,
 *                                           const mlib_u8 *ref_block,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      frame_stride,
 *                                           mlib_s32      field_stride)
 *
 * ARGUMENT
 *      curr_block      Pointer to current block of size wxh,
 *                      must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      frame_stride    Stride in bytes between adjacent rows in a frame in
 *                      both current and reference block,
 *                      must be a multiply of 8
 *      field_stride    Stride in bytes between adjacent rows in a field in
 *                      both current and reference block,
 *                      must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpAveX_U8_U8 = __mlib_VideoInterpAveX_U8_U8
#pragma weak mlib_VideoInterpAveX_U8_U8_16x16 = \
	__mlib_VideoInterpAveX_U8_U8_16x16
#pragma weak mlib_VideoInterpAveX_U8_U8_16x8 = \
	__mlib_VideoInterpAveX_U8_U8_16x8
#pragma weak mlib_VideoInterpAveX_U8_U8_8x16 = \
	__mlib_VideoInterpAveX_U8_U8_8x16
#pragma weak mlib_VideoInterpAveX_U8_U8_8x4 = \
	__mlib_VideoInterpAveX_U8_U8_8x4
#pragma weak mlib_VideoInterpAveX_U8_U8_8x8 = \
	__mlib_VideoInterpAveX_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpAveX_U8_U8) mlib_VideoInterpAveX_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpAveX_U8_U8")));
__typeof__(__mlib_VideoInterpAveX_U8_U8_16x16) mlib_VideoInterpAveX_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpAveX_U8_U8_16x16")));
__typeof__(__mlib_VideoInterpAveX_U8_U8_16x8) mlib_VideoInterpAveX_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpAveX_U8_U8_16x8")));
__typeof__(__mlib_VideoInterpAveX_U8_U8_8x16) mlib_VideoInterpAveX_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpAveX_U8_U8_8x16")));
__typeof__(__mlib_VideoInterpAveX_U8_U8_8x4) mlib_VideoInterpAveX_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpAveX_U8_U8_8x4")));
__typeof__(__mlib_VideoInterpAveX_U8_U8_8x8) mlib_VideoInterpAveX_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpAveX_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MLIB_C_VIDEOINTERPAVG(width, height)                               \
	sl = sd = ref_block;                                               \
	dl = dd = curr_block;                                              \
	for (y = 0; y < height; y++) {                                     \
	    mlib_u32 d0;                                                   \
	                                                                   \
	    for (x = 0; x < (width >> 2); x++) {                           \
		d0 = *(mlib_u32 *)dd;                                      \
		dd[0] = (mlib_u32)(((d0 & 0xff) << 1) + sd[0] + (sd[1] +   \
			3)) >> 2;                                          \
		dd[1] = (mlib_u32)(((d0 >> 7) & 0x1fe) + sd[2] +           \
			(sd[1] + 3)) >> 2;                                 \
		dd[2] = (mlib_u32)(((d0 >> 15) & 0x1fe) + sd[2] +          \
			(sd[3] + 3)) >> 2;                                 \
		dd[3] = (mlib_u32)(((d0 >> 23) & 0x1fe) + sd[4] +          \
			(sd[3] + 3)) >> 2;                                 \
		sd += 4;                                                   \
		dd += 4;                                                   \
	    }                                                              \
	    sl = sd = sl + field_stride;                                   \
	    dl = dd = dl + field_stride;                                   \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_C_VIDEOINTERPAVG(width, height)                               \
	sl = sd = ref_block;                                               \
	dl = dd = curr_block;                                              \
	for (y = 0; y < height; y++) {                                     \
	    mlib_u32 d0;                                                   \
	                                                                   \
	    for (x = 0; x < (width >> 2); x++) {                           \
		d0 = *(mlib_u32 *)dd;                                      \
		dd[0] = (mlib_u32)(((d0 >> 23) & 0x1fe) + sd[0] +          \
			(sd[1] + 3)) >> 2;                                 \
		dd[1] = (mlib_u32)(((d0 >> 15) & 0x1fe) + sd[2] +          \
			(sd[1] + 3)) >> 2;                                 \
		dd[2] = (mlib_u32)(((d0 >> 7) & 0x1fe) + sd[2] +           \
			(sd[3] + 3)) >> 2;                                 \
		dd[3] = (mlib_u32)(((d0 & 0xff) << 1) + sd[4] + (sd[3] +   \
			3)) >> 2;                                          \
		sd += 4;                                                   \
		dd += 4;                                                   \
	    }                                                              \
	    sl = sd = sl + field_stride;                                   \
	    dl = dd = dl + field_stride;                                   \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveX_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	MLIB_C_VIDEOINTERPAVG(8, 8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveX_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	MLIB_C_VIDEOINTERPAVG(8, 16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveX_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	MLIB_C_VIDEOINTERPAVG(8, 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveX_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	MLIB_C_VIDEOINTERPAVG(16, 8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveX_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	MLIB_C_VIDEOINTERPAVG(16, 16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveX_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	mlib_u32 d0;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < height; y++) {

		for (x = 0; x < (width >> 2); x++) {
			d0 = *(mlib_u32 *)dd;

#ifdef _LITTLE_ENDIAN
			dd[0] = (mlib_u32)(((d0 & 0xff) << 1) + sd[0] + (sd[1] +
				3)) >> 2;
			dd[1] = (mlib_u32)(((d0 >> 7) & 0x1fe) + sd[2] +
				(sd[1] + 3)) >> 2;
			dd[2] = (mlib_u32)(((d0 >> 15) & 0x1fe) + sd[2] +
				(sd[3] + 3)) >> 2;
			dd[3] = (mlib_u32)(((d0 >> 23) & 0x1fe) + sd[4] +
				(sd[3] + 3)) >> 2;
#else /* _LITTLE_ENDIAN */
			dd[0] = (mlib_u32)(((d0 >> 23) & 0x1fe) + sd[0] +
				(sd[1] + 3)) >> 2;
			dd[1] = (mlib_u32)(((d0 >> 15) & 0x1fe) + sd[2] +
				(sd[1] + 3)) >> 2;
			dd[2] = (mlib_u32)(((d0 >> 7) & 0x1fe) + sd[2] +
				(sd[3] + 3)) >> 2;
			dd[3] = (mlib_u32)(((d0 & 0xff) << 1) + sd[4] + (sd[3] +
				3)) >> 2;
#endif /* _LITTLE_ENDIAN */

			sd += 4;
			dd += 4;
		}

		sl = sd = sl + field_stride;
		dl = dd = dl + field_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
