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

#pragma ident	"@(#)mlib_c_VideoInterpX_S16_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpX_S16_U8_wxh,
 *      mlib_VideoInterpX_S16_U8     - Performs wxh block half-pixel
 *                                     interpolation in the X-direction for
 *                                     non-replenishment mode.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoInterpX_S16_U8_wxh(mlib_s16      *mc_block,
 *                                             const mlib_u8 *ref_block,
 *                                             mlib_s32      frame_stride,
 *                                             mlib_s32      field_stride)
 *    mlib_status mlib_VideoInterpX_S16_U8(mlib_s16      *mc_block,
 *                                         const mlib_u8 *ref_block,
 *                                         mlib_s32      width,
 *                                         mlib_s32      height,
 *                                         mlib_s32      frame_stride,
 *                                         mlib_s32      field_stride)
 *
 * ARGUMENT
 *      mc_block        Pointer to an wxh motion-compensated reference block,
 *                      must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      frame_stride    Stride in bytes between adjacent rows in a frame in
 *                      reference block, must be a multiply of 8
 *      field_stride    Stride in bytes between adjacent rows in a field in
 *                      reference block, must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpX_S16_U8 = __mlib_VideoInterpX_S16_U8
#pragma weak mlib_VideoInterpX_S16_U8_16x16 = \
	__mlib_VideoInterpX_S16_U8_16x16
#pragma weak mlib_VideoInterpX_S16_U8_16x8 = __mlib_VideoInterpX_S16_U8_16x8
#pragma weak mlib_VideoInterpX_S16_U8_8x16 = __mlib_VideoInterpX_S16_U8_8x16
#pragma weak mlib_VideoInterpX_S16_U8_8x4 = __mlib_VideoInterpX_S16_U8_8x4
#pragma weak mlib_VideoInterpX_S16_U8_8x8 = __mlib_VideoInterpX_S16_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpX_S16_U8) mlib_VideoInterpX_S16_U8
	__attribute__((weak, alias("__mlib_VideoInterpX_S16_U8")));
__typeof__(__mlib_VideoInterpX_S16_U8_16x16) mlib_VideoInterpX_S16_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpX_S16_U8_16x16")));
__typeof__(__mlib_VideoInterpX_S16_U8_16x8) mlib_VideoInterpX_S16_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpX_S16_U8_16x8")));
__typeof__(__mlib_VideoInterpX_S16_U8_8x16) mlib_VideoInterpX_S16_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpX_S16_U8_8x16")));
__typeof__(__mlib_VideoInterpX_S16_U8_8x4) mlib_VideoInterpX_S16_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpX_S16_U8_8x4")));
__typeof__(__mlib_VideoInterpX_S16_U8_8x8) mlib_VideoInterpX_S16_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpX_S16_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_C_VIDEOINTERP(width, height)                                 \
	for (y = 0; y < height; y++) {                                    \
	    for (x = 0; x < width; x++) {                                 \
		mc_block[y * width + x] = ((*sp + *(sp + 1) + 1) >> 1);   \
		sp++;                                                     \
	    }                                                             \
	    sl = sp = sl + field_stride;                                  \
	}

#ifdef _LITTLE_ENDIAN

#define	MLIB_C_VIDEOINTERP2(width, height)                        \
	for (y = 0; y < height; y++) {                            \
	    for (x = 0; x < width; x += 2) {                      \
		((mlib_s32 *)(mc_block + y * width + x))[0] =     \
			((((sp[2] + (sp[1] +                      \
			1)) & 0x1fe) << 16) | (sp[0] + (sp[1] +   \
			1))) >> 1;                                \
		sp += 2;                                          \
	    }                                                     \
	    sl = sp = sl + field_stride;                          \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_C_VIDEOINTERP2(width, height)                        \
	for (y = 0; y < height; y++) {                            \
	    for (x = 0; x < width; x += 2) {                      \
		((mlib_s32 *)(mc_block + y * width + x))[0] =     \
			((((sp[0] + (sp[1] +                      \
			1)) & 0x1fe) << 16) | (sp[2] + (sp[1] +   \
			1))) >> 1;                                \
		sp += 2;                                          \
	    }                                                     \
	    sl = sp = sl + field_stride;                          \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_S16_U8_8x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;

	sl = sp = ref_block;

	MLIB_C_VIDEOINTERP(8, 8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_S16_U8_8x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;

	sl = sp = ref_block;

	MLIB_C_VIDEOINTERP(8, 16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_S16_U8_8x4(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;

	sl = sp = ref_block;

	MLIB_C_VIDEOINTERP2(8, 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_S16_U8_16x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;

	sl = sp = ref_block;

	MLIB_C_VIDEOINTERP2(16, 8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_S16_U8_16x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;

	sl = sp = ref_block;

	MLIB_C_VIDEOINTERP2(16, 16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_S16_U8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;

	sl = sp = ref_block;

	if (width == 8) {
		MLIB_C_VIDEOINTERP(8, height);
	} else {
		MLIB_C_VIDEOINTERP2(16, height);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
