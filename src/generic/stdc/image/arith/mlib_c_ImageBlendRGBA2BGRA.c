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

#pragma ident	"@(#)mlib_c_ImageBlendRGBA2BGRA.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlendRGBA2BGRA - image blending and channel reordering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlendRGBA2BGRA(mlib_image       *dst,
 *                                           const mlib_image *src);
 *
 * ARGUMENTS
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *
 * DESCRIPTION
 *      Blend the source image of the RGBA format into the destination image
 *      of the BGRA format.
 *
 *      The alpha blending is performed with the following formulae:
 *
 *          Cd = Cs * As + Cd * (1 - As)
 *          Ad = Ad
 *
 *      and it can be implemented like the following pseudo code:
 *
 *          foreach pixel in dst
 *              int t;
 *
 *              t = src_a * src_r + (255 - src_a) * dst_r + 0x80;
 *              dst_r = (t + (t >> 8)) >> 8;
 *              t = src_a * src_g + (255 - src_a) * dst_g + 0x80;
 *              dst_g = (t + (t >> 8)) >> 8;
 *              t = src_a * src_b + (255 - src_a) * dst_b + 0x80;
 *              dst_b = (t + (t >> 8)) >> 8;
 *          end
 *
 *      where, src_{r, g,b, a} are the RGB and alpha values of a src pixel;
 *      dst_{r, g,b, a} are the RGB and alpha values of the corresponding dst
 *      pixel.
 *
 *      Notice that dst_a is neither used nor changed by this function.
 *
 * RESTRICTIONS
 *      Both src and dst must be 4-channel images of type MLIB_BYTE.
 *      They must be the same size.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlendRGBA2BGRA = __mlib_ImageBlendRGBA2BGRA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlendRGBA2BGRA) mlib_ImageBlendRGBA2BGRA
    __attribute__((weak, alias("__mlib_ImageBlendRGBA2BGRA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BLEND_(dst, src)                                        \
	dd = dst;                                               \
	dst = (mlib_s32)(alp * (src - dd)) + dd

/* *********************************************************** */

#define	BLEND(dst, src)                                               \
	dd = dst;                                                     \
	t = (mlib_s32)(alp * (src - dd)) + (dd << 8) + (0x80 - dd);   \
	dst = (t + (t >> 8)) >> 8

/* *********************************************************** */

mlib_status
__mlib_ImageBlendRGBA2BGRA(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_u8 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, nchan, width, height;
	mlib_s32 i, j;
	mlib_d64 alp;
	mlib_s32 dd, t;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dlb, dl);
	slb = mlib_ImageGetStride(src);
	sl = mlib_ImageGetData(src);

	if (type != MLIB_BYTE || nchan != 4) {
		return (MLIB_FAILURE);
	}

	for (j = 0; j < height; j++) {
		sp = sl;
		dp = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			alp = sp[3];
/* alp *= 1.0/255; */
			BLEND(dp[2], sp[0]);
			BLEND(dp[1], sp[1]);
			BLEND(dp[0], sp[2]);
			sp += 4;
			dp += 4;
		}

		sl += slb;
		dl += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
