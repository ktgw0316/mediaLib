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

#pragma ident	"@(#)mlib_c_VideoCopyRefAve_U8_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRefAve_U8_U8_wxh,
 *      mlib_VideoCopyRefAve_U8_U8     - Copies and averages a wxh block from
 *                                       reference block to current block
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoCopyRefAve_U8_U8_wxh(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      stride)
 *    mlib_status mlib_VideoCopyRefAve_U8_U8(mlib_u8       *curr_block,
 *                                           const mlib_u8 *ref_block,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      stride)
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      stride          Stride in bytes between adjacent rows in both current
 *                      and reference block, must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRefAve_U8_U8 = __mlib_VideoCopyRefAve_U8_U8
#pragma weak mlib_VideoCopyRefAve_U8_U8_16x16 = \
	__mlib_VideoCopyRefAve_U8_U8_16x16
#pragma weak mlib_VideoCopyRefAve_U8_U8_16x8 = \
	__mlib_VideoCopyRefAve_U8_U8_16x8
#pragma weak mlib_VideoCopyRefAve_U8_U8_8x16 = \
	__mlib_VideoCopyRefAve_U8_U8_8x16
#pragma weak mlib_VideoCopyRefAve_U8_U8_8x4 = \
	__mlib_VideoCopyRefAve_U8_U8_8x4
#pragma weak mlib_VideoCopyRefAve_U8_U8_8x8 = \
	__mlib_VideoCopyRefAve_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRefAve_U8_U8) mlib_VideoCopyRefAve_U8_U8
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_16x16) mlib_VideoCopyRefAve_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_16x16")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_16x8) mlib_VideoCopyRefAve_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_16x8")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_8x16) mlib_VideoCopyRefAve_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_8x16")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_8x4) mlib_VideoCopyRefAve_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_8x4")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_8x8) mlib_VideoCopyRefAve_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MLIB_SPLIT(src, dst0, dst1)                             \
	dst1 = src >> 8;                                        \
	dst0 = src & 0xff

#else /* _LITTLE_ENDIAN */

#define	MLIB_SPLIT(src, dst0, dst1)                             \
	dst0 = src >> 8;                                        \
	dst1 = src & 0xff

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	mlib_u32 v0, v1, v2, v3;
	mlib_u32 d0, d1, d2, d3, d4, d5;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < 16; y++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (x = 0; x < 2; ++x) {

			d4 = *(mlib_u16 *)dd;
			v0 = sd[0];
			v1 = sd[1];
			MLIB_SPLIT(d4, d0, d1);
			d0 = (v0 + (d0 + 1)) >> 1;
			d1 = (v1 + (d1 + 1)) >> 1;

			d5 = *(mlib_u16 *)(dd + 2);
			v2 = sd[2];
			v3 = sd[3];
			MLIB_SPLIT(d5, d2, d3);
			d2 = (v2 + (d2 + 1)) >> 1;
			d3 = (v3 + (d3 + 1)) >> 1;

			d4 = *(mlib_u16 *)(dd + 4);

			v0 = sd[4];
			dd[0] = d0;
			v1 = sd[5];
			dd[1] = d1;
			dd[2] = d2;
			dd[3] = d3;

			d5 = *(mlib_u16 *)(dd + 6);
			MLIB_SPLIT(d4, d0, d1);
			d0 = (v0 + (d0 + 1)) >> 1;
			d1 = (v1 + (d1 + 1)) >> 1;

			v2 = sd[6];
			v3 = sd[7];
			MLIB_SPLIT(d5, d2, d3);
			d2 = (v2 + (d2 + 1)) >> 1;
			d3 = (v3 + (d3 + 1)) >> 1;

			dd[4] = d0;
			dd[5] = d1;
			dd[6] = d2;
			dd[7] = d3;
			sd += 8;
			dd += 8;
		}

		sl = sd = sl + stride;
		dl = dd = dl + stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_u32 v0, v1, v2, v3;
	mlib_u32 d0, d1, d2, d3, d4, d5;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < 16; y++) {

		d4 = *(mlib_u16 *)dd;

		v0 = sd[0];
		v1 = sd[1];
		MLIB_SPLIT(d4, d0, d1);
		d0 = (v0 + (d0 + 1)) >> 1;
		d1 = (v1 + (d1 + 1)) >> 1;

		d5 = *(mlib_u16 *)(dd + 2);
		v2 = sd[2];
		v3 = sd[3];
		MLIB_SPLIT(d5, d2, d3);
		d2 = (v2 + (d2 + 1)) >> 1;
		d3 = (v3 + (d3 + 1)) >> 1;

		d4 = *(mlib_u16 *)(dd + 4);

		v0 = sd[4];
		dd[0] = d0;
		v1 = sd[5];
		dd[1] = d1;
		dd[2] = d2;
		dd[3] = d3;

		d5 = *(mlib_u16 *)(dd + 6);
		MLIB_SPLIT(d4, d0, d1);
		d0 = (v0 + (d0 + 1)) >> 1;
		d1 = (v1 + (d1 + 1)) >> 1;

		v2 = sd[6];
		v3 = sd[7];
		MLIB_SPLIT(d5, d2, d3);
		d2 = (v2 + (d2 + 1)) >> 1;
		d3 = (v3 + (d3 + 1)) >> 1;

		dd[4] = d0;
		dd[5] = d1;
		dd[6] = d2;
		dd[7] = d3;
		sl = sd = sl + stride;
		dl = dd = dl + stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	mlib_u32 v0, v1, v2, v3;
	mlib_u32 d0, d1, d2, d3, d4, d5;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < 8; y++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (x = 0; x < 2; ++x) {

			d4 = *(mlib_u16 *)dd;
			v0 = sd[0];
			v1 = sd[1];
			MLIB_SPLIT(d4, d0, d1);
			d0 = (v0 + (d0 + 1)) >> 1;
			d1 = (v1 + (d1 + 1)) >> 1;

			d5 = *(mlib_u16 *)(dd + 2);
			v2 = sd[2];
			v3 = sd[3];
			MLIB_SPLIT(d5, d2, d3);
			d2 = (v2 + (d2 + 1)) >> 1;
			d3 = (v3 + (d3 + 1)) >> 1;

			d4 = *(mlib_u16 *)(dd + 4);

			v0 = sd[4];
			dd[0] = d0;
			v1 = sd[5];
			dd[1] = d1;
			dd[2] = d2;
			dd[3] = d3;

			d5 = *(mlib_u16 *)(dd + 6);
			MLIB_SPLIT(d4, d0, d1);
			d0 = (v0 + (d0 + 1)) >> 1;
			d1 = (v1 + (d1 + 1)) >> 1;

			v2 = sd[6];
			v3 = sd[7];
			MLIB_SPLIT(d5, d2, d3);
			d2 = (v2 + (d2 + 1)) >> 1;
			d3 = (v3 + (d3 + 1)) >> 1;

			dd[4] = d0;
			dd[5] = d1;
			dd[6] = d2;
			dd[7] = d3;
			sd += 8;
			dd += 8;
		}

		sl = sd = sl + stride;
		dl = dd = dl + stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_u32 v0, v1, v2, v3;
	mlib_u32 d0, d1, d2, d3, d4, d5;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < 8; y++) {

		d4 = *(mlib_u16 *)dd;
		v0 = sd[0];
		v1 = sd[1];
		MLIB_SPLIT(d4, d0, d1);
		d0 = (v0 + (d0 + 1)) >> 1;
		d1 = (v1 + (d1 + 1)) >> 1;

		d5 = *(mlib_u16 *)(dd + 2);
		v2 = sd[2];
		v3 = sd[3];
		MLIB_SPLIT(d5, d2, d3);
		d2 = (v2 + (d2 + 1)) >> 1;
		d3 = (v3 + (d3 + 1)) >> 1;

		d4 = *(mlib_u16 *)(dd + 4);

		v0 = sd[4];
		dd[0] = d0;
		v1 = sd[5];
		dd[1] = d1;
		dd[2] = d2;
		dd[3] = d3;

		d5 = *(mlib_u16 *)(dd + 6);
		MLIB_SPLIT(d4, d0, d1);
		d0 = (v0 + (d0 + 1)) >> 1;
		d1 = (v1 + (d1 + 1)) >> 1;

		v2 = sd[6];
		v3 = sd[7];
		MLIB_SPLIT(d5, d2, d3);
		d2 = (v2 + (d2 + 1)) >> 1;
		d3 = (v3 + (d3 + 1)) >> 1;

		dd[4] = d0;
		dd[5] = d1;
		dd[6] = d2;
		dd[7] = d3;
		sl = sd = sl + stride;
		dl = dd = dl + stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_u32 v0, v1, v2, v3;
	mlib_u32 d0, d1, d2, d3, d4, d5;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < 4; y++) {

		d4 = *(mlib_u16 *)dd;
		v0 = sd[0];
		v1 = sd[1];
		MLIB_SPLIT(d4, d0, d1);
		d0 = (v0 + (d0 + 1)) >> 1;
		d1 = (v1 + (d1 + 1)) >> 1;

		d5 = *(mlib_u16 *)(dd + 2);
		v2 = sd[2];
		v3 = sd[3];
		MLIB_SPLIT(d5, d2, d3);
		d2 = (v2 + (d2 + 1)) >> 1;
		d3 = (v3 + (d3 + 1)) >> 1;

		d4 = *(mlib_u16 *)(dd + 4);

		v0 = sd[4];
		dd[0] = d0;
		v1 = sd[5];
		dd[1] = d1;
		dd[2] = d2;
		dd[3] = d3;

		d5 = *(mlib_u16 *)(dd + 6);
		MLIB_SPLIT(d4, d0, d1);
		d0 = (v0 + (d0 + 1)) >> 1;
		d1 = (v1 + (d1 + 1)) >> 1;

		v2 = sd[6];
		v3 = sd[7];
		MLIB_SPLIT(d5, d2, d3);
		d2 = (v2 + (d2 + 1)) >> 1;
		d3 = (v3 + (d3 + 1)) >> 1;

		dd[4] = d0;
		dd[5] = d1;
		dd[6] = d2;
		dd[7] = d3;
		sl = sd = sl + stride;
		dl = dd = dl + stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	mlib_u32 v0, v1, v2, v3;
	mlib_u32 d0, d1, d2, d3, d4, d5;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;

	sl = sd = ref_block;
	dl = dd = curr_block;

	for (y = 0; y < height; y++) {

		x = (width >> 3);
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (; x > 0; x--) {

			d4 = *(mlib_u16 *)dd;
			v0 = sd[0];
			v1 = sd[1];
			MLIB_SPLIT(d4, d0, d1);
			d0 = (v0 + (d0 + 1)) >> 1;
			d1 = (v1 + (d1 + 1)) >> 1;

			d5 = *(mlib_u16 *)(dd + 2);
			v2 = sd[2];
			v3 = sd[3];
			MLIB_SPLIT(d5, d2, d3);
			d2 = (v2 + (d2 + 1)) >> 1;
			d3 = (v3 + (d3 + 1)) >> 1;

			d4 = *(mlib_u16 *)(dd + 4);

			v0 = sd[4];
			dd[0] = d0;
			v1 = sd[5];
			dd[1] = d1;
			dd[2] = d2;
			dd[3] = d3;

			d5 = *(mlib_u16 *)(dd + 6);
			MLIB_SPLIT(d4, d0, d1);
			d0 = (v0 + (d0 + 1)) >> 1;
			d1 = (v1 + (d1 + 1)) >> 1;

			v2 = sd[6];
			v3 = sd[7];
			MLIB_SPLIT(d5, d2, d3);
			d2 = (v2 + (d2 + 1)) >> 1;
			d3 = (v3 + (d3 + 1)) >> 1;

			dd[4] = d0;
			dd[5] = d1;
			dd[6] = d2;
			dd[7] = d3;
			sd += 8;
			dd += 8;
		}

		sl = sd = sl + stride;
		dl = dd = dl + stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
