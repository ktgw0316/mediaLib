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

#pragma ident	"@(#)mlib_c_VideoCopyRef_U8_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRef_U8_U8_wxh,
 *      mlib_VideoCopyRef_U8_U8      - Copies a wxh block from reference block
 *                                     to current block
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoCopyRef_U8_U8_wxh(mlib_u8       *curr_block,
 *                                            const mlib_u8 *ref_block,
 *                                            mlib_s32      stride)
 *    mlib_status mlib_VideoCopyRef_U8_U8(mlib_u8       *curr_block,
 *                                        const mlib_u8 *ref_block,
 *                                        mlib_s32      width,
 *                                        mlib_s32      height,
 *                                        mlib_s32      stride)
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
#include <mlib_Utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRef_U8_U8 = __mlib_VideoCopyRef_U8_U8
#pragma weak mlib_VideoCopyRef_U8_U8_16x16 = __mlib_VideoCopyRef_U8_U8_16x16
#pragma weak mlib_VideoCopyRef_U8_U8_16x8 = __mlib_VideoCopyRef_U8_U8_16x8
#pragma weak mlib_VideoCopyRef_U8_U8_8x16 = __mlib_VideoCopyRef_U8_U8_8x16
#pragma weak mlib_VideoCopyRef_U8_U8_8x4 = __mlib_VideoCopyRef_U8_U8_8x4
#pragma weak mlib_VideoCopyRef_U8_U8_8x8 = __mlib_VideoCopyRef_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRef_U8_U8) mlib_VideoCopyRef_U8_U8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8")));
__typeof__(__mlib_VideoCopyRef_U8_U8_16x16) mlib_VideoCopyRef_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_16x16")));
__typeof__(__mlib_VideoCopyRef_U8_U8_16x8) mlib_VideoCopyRef_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_16x8")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x16) mlib_VideoCopyRef_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x16")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x4) mlib_VideoCopyRef_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x4")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x8) mlib_VideoCopyRef_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (!((mlib_u32)ref_block & 7)) {
		TYPE_64BIT s0, s1, *sd, *dd;

		dd = (TYPE_64BIT *) curr_block;
		sd = (TYPE_64BIT *) ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 16; ++y) {
			s0 = sd[0];
			s1 = sd[1];
			dd[0] = s0;
			dd[1] = s1;
			sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
			dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
		}
	} else if (!((mlib_u32)ref_block & 3)) {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2, u3;

		sb = (mlib_u32 *)ref_block;
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 16; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
			u3 = sb[3];
			db[0] = u0;
			db[1] = u1;
			db[2] = u2;
			db[3] = u3;
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

/* src is not u32 aligned */
	else {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2, u3, u4;
		mlib_s32 lsh = ((mlib_s32)ref_block & 3) << 3;
		mlib_s32 rsh = 32 - lsh;

		sb = (mlib_u32 *)((mlib_addr)ref_block & ~3);
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 16; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
			u3 = sb[3];
			u4 = sb[4];
#ifdef _LITTLE_ENDIAN
			db[0] = (u0 >> lsh) | (u1 << rsh);
			db[1] = (u1 >> lsh) | (u2 << rsh);
			db[2] = (u2 >> lsh) | (u3 << rsh);
			db[3] = (u3 >> lsh) | (u4 << rsh);
#else /* _LITTLE_ENDIAN */
			db[0] = (u0 << lsh) | (u1 >> rsh);
			db[1] = (u1 << lsh) | (u2 >> rsh);
			db[2] = (u2 << lsh) | (u3 >> rsh);
			db[3] = (u3 << lsh) | (u4 >> rsh);
#endif /* _LITTLE_ENDIAN */
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (!((mlib_u32)ref_block & 7)) {
		TYPE_64BIT *sd, *dd;

		dd = (TYPE_64BIT *) curr_block;
		sd = (TYPE_64BIT *) ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 16; ++y) {
			*dd = *sd;
			sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
			dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
		}
	} else if (!((mlib_u32)ref_block & 3)) {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1;

		sb = (mlib_u32 *)ref_block;
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 16; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			db[0] = u0;
			db[1] = u1;
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

/* src is not u32 aligned */
	else {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2;
		mlib_s32 lsh = ((mlib_s32)ref_block & 3) << 3;
		mlib_s32 rsh = 32 - lsh;

		sb = (mlib_u32 *)((mlib_addr)ref_block & ~3);
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 16; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
#ifdef _LITTLE_ENDIAN
			db[0] = (u0 >> lsh) | (u1 << rsh);
			db[1] = (u1 >> lsh) | (u2 << rsh);
#else /* _LITTLE_ENDIAN */
			db[0] = (u0 << lsh) | (u1 >> rsh);
			db[1] = (u1 << lsh) | (u2 >> rsh);
#endif /* _LITTLE_ENDIAN */
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (!((mlib_u32)ref_block & 7)) {
		TYPE_64BIT s0, s1, *sd, *dd;

		dd = (TYPE_64BIT *) curr_block;
		sd = (TYPE_64BIT *) ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 8; ++y) {
			s0 = sd[0];
			s1 = sd[1];
			dd[0] = s0;
			dd[1] = s1;
			sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
			dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
		}
	} else if (!((mlib_u32)ref_block & 3)) {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2, u3;

		sb = (mlib_u32 *)ref_block;
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 8; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
			u3 = sb[3];
			db[0] = u0;
			db[1] = u1;
			db[2] = u2;
			db[3] = u3;
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

/* src is not u32 aligned */
	else {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2, u3, u4;
		mlib_s32 lsh = ((mlib_s32)ref_block & 3) << 3;
		mlib_s32 rsh = 32 - lsh;

		sb = (mlib_u32 *)((mlib_addr)ref_block & ~3);
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 8; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
			u3 = sb[3];
			u4 = sb[4];
#ifdef _LITTLE_ENDIAN
			db[0] = (u0 >> lsh) | (u1 << rsh);
			db[1] = (u1 >> lsh) | (u2 << rsh);
			db[2] = (u2 >> lsh) | (u3 << rsh);
			db[3] = (u3 >> lsh) | (u4 << rsh);
#else /* _LITTLE_ENDIAN */
			db[0] = (u0 << lsh) | (u1 >> rsh);
			db[1] = (u1 << lsh) | (u2 >> rsh);
			db[2] = (u2 << lsh) | (u3 >> rsh);
			db[3] = (u3 << lsh) | (u4 >> rsh);
#endif /* _LITTLE_ENDIAN */
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (!((mlib_u32)ref_block & 7)) {
		TYPE_64BIT *sd, *dd;

		dd = (TYPE_64BIT *) curr_block;
		sd = (TYPE_64BIT *) ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 8; ++y) {
			*dd = *sd;
			sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
			dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
		}
	} else if (!((mlib_u32)ref_block & 3)) {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1;

		sb = (mlib_u32 *)ref_block;
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 8; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			db[0] = u0;
			db[1] = u1;
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

/* src is not u32 aligned */
	else {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2;
		mlib_s32 lsh = ((mlib_s32)ref_block & 3) << 3;
		mlib_s32 rsh = 32 - lsh;

		sb = (mlib_u32 *)((mlib_addr)ref_block & ~3);
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 8; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
#ifdef _LITTLE_ENDIAN
			db[0] = (u0 >> lsh) | (u1 << rsh);
			db[1] = (u1 >> lsh) | (u2 << rsh);
#else /* _LITTLE_ENDIAN */
			db[0] = (u0 << lsh) | (u1 >> rsh);
			db[1] = (u1 << lsh) | (u2 >> rsh);
#endif /* _LITTLE_ENDIAN */
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (!((mlib_u32)ref_block & 7)) {
		TYPE_64BIT *sd, *dd;

		dd = (TYPE_64BIT *) curr_block;
		sd = (TYPE_64BIT *) ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 4; ++y) {
			*dd = *sd;
			sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
			dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
		}
	} else if (!((mlib_u32)ref_block & 3)) {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1;

		sb = (mlib_u32 *)ref_block;
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 4; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			db[0] = u0;
			db[1] = u1;
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

/* src is not u32 aligned */
	else {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2;
		mlib_s32 lsh = ((mlib_s32)ref_block & 3) << 3;
		mlib_s32 rsh = 32 - lsh;

		sb = (mlib_u32 *)((mlib_addr)ref_block & ~3);
		db = (mlib_u32 *)curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (y = 0; y < 4; ++y) {
			u0 = sb[0];
			u1 = sb[1];
			u2 = sb[2];
#ifdef _LITTLE_ENDIAN
			db[0] = (u0 >> lsh) | (u1 << rsh);
			db[1] = (u1 >> lsh) | (u2 << rsh);
#else /* _LITTLE_ENDIAN */
			db[0] = (u0 << lsh) | (u1 >> rsh);
			db[1] = (u1 << lsh) | (u2 >> rsh);
#endif /* _LITTLE_ENDIAN */
			sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
			db = (mlib_u32 *)((mlib_u8 *)db + stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (!((mlib_u32)ref_block & 7)) {
		TYPE_64BIT s0, s1, *sd, *dd;

		dd = (TYPE_64BIT *) curr_block;
		sd = (TYPE_64BIT *) ref_block;

		if (width == 8) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (y = 0; y < height; ++y) {
				*dd = *sd;
				sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
				dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
			}
		} else {
/* if (width==16) */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (y = 0; y < height; ++y) {
				s0 = sd[0];
				s1 = sd[1];
				dd[0] = s0;
				dd[1] = s1;
				sd = (TYPE_64BIT *) ((mlib_u8 *)sd + stride);
				dd = (TYPE_64BIT *) ((mlib_u8 *)dd + stride);
			}
		}
	} else if (!((mlib_u32)ref_block & 3)) {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2, u3;

		sb = (mlib_u32 *)ref_block;
		db = (mlib_u32 *)curr_block;

		if (width == 8) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (y = 0; y < height; ++y) {
				u0 = sb[0];
				u1 = sb[1];
				db[0] = u0;
				db[1] = u1;
				sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
				db = (mlib_u32 *)((mlib_u8 *)db + stride);
			}
		} else {
/* if (width==16) */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (y = 0; y < height; ++y) {
				u0 = sb[0];
				u1 = sb[1];
				u2 = sb[2];
				u3 = sb[3];
				db[0] = u0;
				db[1] = u1;
				db[2] = u2;
				db[3] = u3;
				sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
				db = (mlib_u32 *)((mlib_u8 *)db + stride);
			}
		}
	}

/* src is not u32 aligned */
	else {
		mlib_u32 *sb, *db;
		mlib_u32 u0, u1, u2, u3, u4;
		mlib_s32 lsh = ((mlib_s32)ref_block & 3) << 3;
		mlib_s32 rsh = 32 - lsh;

		sb = (mlib_u32 *)((mlib_addr)ref_block & ~3);
		db = (mlib_u32 *)curr_block;

		if (width == 8) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (y = 0; y < height; ++y) {
				u0 = sb[0];
				u1 = sb[1];
				u2 = sb[2];
#ifdef _LITTLE_ENDIAN
				db[0] = (u0 >> lsh) | (u1 << rsh);
				db[1] = (u1 >> lsh) | (u2 << rsh);
#else /* _LITTLE_ENDIAN */
				db[0] = (u0 << lsh) | (u1 >> rsh);
				db[1] = (u1 << lsh) | (u2 >> rsh);
#endif /* _LITTLE_ENDIAN */
				sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
				db = (mlib_u32 *)((mlib_u8 *)db + stride);
			}
		} else {
/* if (width==16) */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (y = 0; y < height; ++y) {
				u0 = sb[0];
				u1 = sb[1];
				u2 = sb[2];
				u3 = sb[3];
				u4 = sb[4];
#ifdef _LITTLE_ENDIAN
				db[0] = (u0 >> lsh) | (u1 << rsh);
				db[1] = (u1 >> lsh) | (u2 << rsh);
				db[2] = (u2 >> lsh) | (u3 << rsh);
				db[3] = (u3 >> lsh) | (u4 << rsh);
#else /* _LITTLE_ENDIAN */
				db[0] = (u0 << lsh) | (u1 >> rsh);
				db[1] = (u1 << lsh) | (u2 >> rsh);
				db[2] = (u2 << lsh) | (u3 >> rsh);
				db[3] = (u3 << lsh) | (u4 >> rsh);
#endif /* _LITTLE_ENDIAN */
				sb = (mlib_u32 *)((mlib_u8 *)sb + stride);
				db = (mlib_u32 *)((mlib_u8 *)db + stride);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
