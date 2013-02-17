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

#pragma ident	"@(#)mlib_v_ImageNot.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot   - not an image (VIS version)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNot(mlib_image *dst,
 *                                const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same dsize.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,
 *      MLIB_INT or MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:  dst = ~src
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>
#include <mlib_v_ImageLogic_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNot = __mlib_ImageNot

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNot) mlib_ImageNot
    __attribute__((weak, alias("__mlib_ImageNot")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CASHSIZE	(256 * 1024)

/* *********************************************************** */

mlib_status
__mlib_ImageNot(
    mlib_image *dst,
    const mlib_image *src)
{
/* start point in source */
	mlib_u8 *sa;

/* start points in destination */
	mlib_u8 *da;

/* width in bytes of src and dst */
	mlib_s32 width;

/* height in lines of src and dst */
	mlib_s32 height;

/* stride in bytes in src */
	mlib_s32 stride;

/* stride in bytes in dst */
	mlib_s32 dstride;

/* indices for x, y */
	mlib_s32 j;
	mlib_s32 size;
	mlib_s32 type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	width = mlib_ImageGetWidth(dst) * mlib_ImageGetChannels(dst);
	height = mlib_ImageGetHeight(dst);
	sa = (mlib_u8 *)mlib_ImageGetData(src);
	da = (mlib_u8 *)mlib_ImageGetData(dst);
	type = mlib_ImageGetType(dst);

	if ((type != MLIB_BYTE) && (type != MLIB_SHORT) &&
	    (type != MLIB_USHORT) && (type != MLIB_BIT) &&
	    (type != MLIB_INT))
		return (MLIB_FAILURE);

	if (type != MLIB_BIT) {

		switch (type) {
		case MLIB_BYTE:
			break;
		case MLIB_SHORT:
		case MLIB_USHORT:
			width *= 2;
			break;
		case MLIB_INT:
			width *= 4;
			break;
		default:
			return (MLIB_FAILURE);
		}

		size = height * width;

		if (!mlib_ImageIsNotOneDvector(src) &&
		    !mlib_ImageIsNotOneDvector(dst) &&
		    ((((mlib_addr)sa ^ (mlib_addr)da) & 7) == 0) &&
		    (size > CASHSIZE)) {
			mlib_s32 tail = 0x40 - ((mlib_addr)da & 0x3F);

			mlib_v_ImageNot_na(sa, da, tail & 0x3F);
			sa += tail & 0x3F;
			da += tail & 0x3F;
			size -= tail & 0x3F;
/* (size >> 6) should be > 1 */
			mlib_v_ImageNot_blk(sa, da, size >> 6);
			sa += size & ~0x3F;
			da += size & ~0x3F;
			mlib_v_ImageNot_na(sa, da, size & 0x3F);
			return (MLIB_SUCCESS);
		} else {

/* in byte */
			stride = mlib_ImageGetStride(src);
/* in byte */
			dstride = mlib_ImageGetStride(dst);

			if (width > stride || width > dstride)
				return (MLIB_FAILURE);

			for (j = 0; j < height; j++) {
				mlib_v_ImageNot_na(sa, da, width);
				sa += stride;
				da += dstride;
			}
		}

		return (MLIB_SUCCESS);
	} else {

		if (mlib_ImageGetBitOffset(src) ==
		    mlib_ImageGetBitOffset(dst)) {
			return (mlib_v_ImageNot_Bit(dst, src));
		} else {
			return (mlib_ImageNot_Bit(dst, src));
		}
	}
}

/* *********************************************************** */
