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

#pragma ident	"@(#)mlib_c_VideoCopyRef_S16_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRef_S16_U8_wxh,
 *      mlib_VideoCopyRef_S16_U8    - Copies a wxh block from reference
 *                                    block to current block
 *                                    non-replenishment mode
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoCopyRef_S16_U8_wxh(mlib_s16      *mc_block,
 *                                             const mlib_u8 *ref_block,
 *                                             mlib_s32      stride)
 *    mlib_status mlib_VideoCopyRef_S16_U8(mlib_s16      *mc_block,
 *                                         const mlib_u8 *ref_block,
 *                                         mlib_s32      width,
 *                                         mlib_s32      height,
 *                                         mlib_s32      stride)
 *
 * ARGUMENT
 *      mc_block        Pointer to motion-compensated reference block,
 *                      must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      stride          Stride in bytes between adjacent rows in reference
 *                      block, must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRef_S16_U8 = __mlib_VideoCopyRef_S16_U8
#pragma weak mlib_VideoCopyRef_S16_U8_16x16 = \
	__mlib_VideoCopyRef_S16_U8_16x16
#pragma weak mlib_VideoCopyRef_S16_U8_16x8 = __mlib_VideoCopyRef_S16_U8_16x8
#pragma weak mlib_VideoCopyRef_S16_U8_8x16 = __mlib_VideoCopyRef_S16_U8_8x16
#pragma weak mlib_VideoCopyRef_S16_U8_8x4 = __mlib_VideoCopyRef_S16_U8_8x4
#pragma weak mlib_VideoCopyRef_S16_U8_8x8 = __mlib_VideoCopyRef_S16_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRef_S16_U8) mlib_VideoCopyRef_S16_U8
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8")));
__typeof__(__mlib_VideoCopyRef_S16_U8_16x16) mlib_VideoCopyRef_S16_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_16x16")));
__typeof__(__mlib_VideoCopyRef_S16_U8_16x8) mlib_VideoCopyRef_S16_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_16x8")));
__typeof__(__mlib_VideoCopyRef_S16_U8_8x16) mlib_VideoCopyRef_S16_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_8x16")));
__typeof__(__mlib_VideoCopyRef_S16_U8_8x4) mlib_VideoCopyRef_S16_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_8x4")));
__typeof__(__mlib_VideoCopyRef_S16_U8_8x8) mlib_VideoCopyRef_S16_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_S16_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MLIB_C_VIDEOCOPY(width, height)                         \
	sl = sd = ref_block;                                    \
	for (y = 0; y < height; y++) {                          \
	    for (x = 0; x < width >> 1; x++) {                  \
		((mlib_s32 *)(mc_block + y * width))[x] =       \
			(sd[1] << 16) | sd[0];                  \
		sd += 2;                                        \
	    }                                                   \
	    sl = sd = sl + stride;                              \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_C_VIDEOCOPY(width, height)                         \
	sl = sd = ref_block;                                    \
	for (y = 0; y < height; y++) {                          \
	    for (x = 0; x < width >> 1; x++) {                  \
		((mlib_s32 *)(mc_block + y * width))[x] =       \
			(sd[0] << 16) | sd[1];                  \
		sd += 2;                                        \
	    }                                                   \
	    sl = sd = sl + stride;                              \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_16x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;

	MLIB_C_VIDEOCOPY(16, 16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_8x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;

	MLIB_C_VIDEOCOPY(8, 16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_16x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;

	MLIB_C_VIDEOCOPY(16, 8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_8x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;

	MLIB_C_VIDEOCOPY(8, 8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8_8x4(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;

	MLIB_C_VIDEOCOPY(8, 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_S16_U8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;

	if (width == 8) {
		MLIB_C_VIDEOCOPY(8, height)
	} else {
/* ( width == 16) */

		MLIB_C_VIDEOCOPY(16, height)
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
