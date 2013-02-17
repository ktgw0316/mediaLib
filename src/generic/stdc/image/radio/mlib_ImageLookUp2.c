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

#pragma ident	"@(#)mlib_ImageLookUp2.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUp2 - table lookup with offsets
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLookUp2(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const void       **table,
 *                                    const mlib_s32   *offsets,
 *                                    mlib_s32         channels)
 *
 * ARGUMENT
 *      dst      Pointer to destination image.
 *      src      Pointer to source image.
 *      table    Lookup table.
 *      offsets  Offset values deducted from the src pixel before table lookup.
 *      channels Number of channels of the lookup table.
 *
 * DESCRIPTION
 *      The mlib_ImageLookUp2 function performs general table lookup on an
 *      image. The destination image is obtained by passing a source image
 *      through a lookup table.
 *
 *      The source image may be 1-, 2-, 3-, or 4-channeled of data types
 *      MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT. The lookup
 *      table may be 1-, 2-, 3-, or 4-channeled of data types MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT, MLIB_FLOAT, or MLIB_DOUBLE.
 *      The destination image must have the same number of channels as
 *      either source image or the lookup table, whichever is greater,
 *      and the same data type as the lookup table.
 *
 *      It is the user's responsibility to make sure that the lookup table
 *      supplied is suitable for the source image. Specifically, the table
 *      entries cover the entire range of source data. Otherwise, the result
 *      of this function is undefined.
 *
 *      The pixel values of the destination image are defined as the following:
 *
 *      If the source image is single-channeled and the lookup table is
 *      single- or multi-channeled, then the destination image has the same
 *      number of channels as the lookup table:
 *
 *          dst[x][y][c] = table[c][src[x][y][0] - offsets[c]]
 *
 *      If the source image is multi-channeled and the lookup table is
 *      single-channeled, then the destination image has the same number of
 *      channels as the source image:
 *
 *          dst[x][y][c] = table[0][src[x][y][c] - offsets[0]]
 *
 *      If the source image is multi-channeled and the lookup table is
 *      multi-channeled, with the same number of channels as the source image,
 *      then the destination image will have the same number of channels as
 *      the source image:
 *
 *          dst[x][y][c] = table[c][src[x][y][c] - offsets[c]]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLookUp2 = __mlib_ImageLookUp2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageLookUp2) mlib_ImageLookUp2
    __attribute__((weak, alias("__mlib_ImageLookUp2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageLookUp2(
    mlib_image *dst,
    const mlib_image *src,
    const void **table,
    const mlib_s32 *offsets,
    mlib_s32 channels)
{
	mlib_u8 *new_table[4];
	mlib_s32 dchan, schan, i, size = 1, shift = MLIB_U8_MIN;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	schan = mlib_ImageGetChannels(src);
	dchan = mlib_ImageGetChannels(dst);

	if (schan == 1) {
		if (dchan != channels)
			return (MLIB_FAILURE);
	} else {
		if (channels != 1 && schan != channels)
			return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetType(dst)) {

	case MLIB_USHORT:
	case MLIB_SHORT:
		size = 2;
		break;

	case MLIB_INT:
	case MLIB_FLOAT:
		size = 4;
		break;

	case MLIB_DOUBLE:
		size = 8;
		break;
	default:
		size = 1;
		break;
	}

	switch (mlib_ImageGetType(src)) {

	case MLIB_BIT:

		if (schan != 1)
			return (MLIB_FAILURE);
		else
			break;

	case MLIB_USHORT:
		shift = 0;
		break;

	case MLIB_SHORT:
		shift = MLIB_S16_MIN;
		break;

	case MLIB_INT:
		shift = MLIB_S32_MIN;
		break;
	default:
		break;
	}

	for (i = 0; i < channels; i++) {

#ifdef _LP64

		new_table[i] =
		    (mlib_u8 *)table[i] + size * (shift - (mlib_s64)offsets[i]);

#else /* _LP64 */

		new_table[i] =
		    (mlib_u8 *)table[i] + size * (shift - offsets[i]);

#endif /* _LP64 */
	}

	if (channels == 1) {
		for (i = 1; i < dchan; i++) {
			new_table[i] = new_table[0];
		}
	}

	return (__mlib_ImageLookUp(dst, src, (const void **)new_table));
}

/* *********************************************************** */
