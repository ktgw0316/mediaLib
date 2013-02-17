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

#pragma ident   "@(#)mlib_s_ImageBlendColor_S32.c	9.3	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor on S32 data type.
 */

#include <mlib_image.h>
#include <mlib_s_ImageBlendColor.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

#define	BLEND_COLOR_S32(dp, color, sp0, sp1)	\
	dp = ((mlib_s64)(sp0 & 0x7fffffff)) * sp1	\
			+ ((mlib_s64)((1u << 31) - (sp0 & 0x7fffffff)))	\
			* color >> 31;

/* *********************************************************** */

void
mlib_s_ImageBlendColor_S32_2(
    mlib_s32 *sp,
    mlib_s32 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	if (alpha == 0) {

		for (j = 0; j < height; j++) {

			sl = sp;
			dl = dp;

			for (i = 0; i < width; i ++) {

				dl[0] = MLIB_S32_MAX;
				BLEND_COLOR_S32(dl[1], color[1], sl[0], sl[1]);

				sl += 2;
				dl += 2;
			}

			sp += sstride;
			dp += dstride;
		}
	} else {

		for (j = 0; j < height; j++) {

			sl = sp;
			dl = dp;

			for (i = 0; i < width; i++) {

				dl[1] = MLIB_S32_MAX;
				BLEND_COLOR_S32(dl[0], color[0], sl[1], sl[0]);

				sl += 2;
				dl += 2;
			}

			sp += sstride;
			dp += dstride;
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageBlendColor_S32_4(
    mlib_s32 *sp,
    mlib_s32 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{

	mlib_s32 i, j;

	if (alpha == 0) {
		for (j = 0; j < height; j++) {
			mlib_s32 *sl = sp;
			mlib_s32 *dl = dp;

			for (i = 0; i < width; i ++) {
				dl[0] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[1], color[1], sl[0], sl[1]);
				BLEND_COLOR_S32(dl[2], color[2], sl[0], sl[2]);
				BLEND_COLOR_S32(dl[3], color[3], sl[0], sl[3]);

				sl += 4;
				dl += 4;
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (alpha == 1) {
		for (j = 0; j < height; j++) {

			mlib_s32 *sl = sp;
			mlib_s32 *dl = dp;

			for (i = 0; i < width; i++) {

				dl[1] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[0], color[0], sl[1], sl[0]);
				BLEND_COLOR_S32(dl[2], color[2], sl[1], sl[2]);
				BLEND_COLOR_S32(dl[3], color[3], sl[1], sl[3]);

				sl += 4;
				dl += 4;
			}
			sp += sstride;
			dp += dstride;
		}

	} else if (alpha == 2) {
		for (j = 0; j < height; j++) {
			mlib_s32 *sl = sp;
			mlib_s32 *dl = dp;

			for (i = 0; i < width; i ++) {

				dl[2] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[0], color[0], sl[2], sl[0]);
				BLEND_COLOR_S32(dl[1], color[1], sl[2], sl[1]);
				BLEND_COLOR_S32(dl[3], color[3], sl[2], sl[3]);

				sl += 4;
				dl += 4;
			}

			sp += sstride;
			dp += dstride;
		}


	} else {
		for (j = 0; j < height; j++) {
			mlib_s32 *sl = sp;
			mlib_s32 *dl = dp;

			for (i = 0; i < width; i ++) {

				dl[3] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[0], color[0], sl[3], sl[0]);
				BLEND_COLOR_S32(dl[1], color[1], sl[3], sl[1]);
				BLEND_COLOR_S32(dl[2], color[2], sl[3], sl[2]);

				sl += 4;
				dl += 4;
			}

			sp += sstride;
			dp += dstride;
		}

	}
}

/* *********************************************************** */

void
mlib_s_ImageBlendColor_S32_3(
    mlib_s32 *sp,
    mlib_s32 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	if (alpha == 0) {
		for (j = 0; j < height; j++) {

			sl = sp;
			dl = dp;

			for (i = 0; i < width; i ++) {

				dl[0] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[1], color[1], sl[0], sl[1]);
				BLEND_COLOR_S32(dl[2], color[2], sl[0], sl[2]);


				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}
	} else if (alpha == 1) {

		for (j = 0; j < height; j++) {

			sl = sp;
			dl = dp;

			for (i = 0; i < width; i++) {

				dl[1] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[0], color[0], sl[1], sl[0]);
				BLEND_COLOR_S32(dl[2], color[2], sl[1], sl[2]);

				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}
	} else {

		for (j = 0; j < height; j++) {

			sl = sp;
			dl = dp;

			for (i = 0; i < width; i ++) {

				dl[2] = MLIB_S32_MAX;

				BLEND_COLOR_S32(dl[0], color[0], sl[2], sl[0]);
				BLEND_COLOR_S32(dl[1], color[1], sl[2], sl[1]);

				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}
	}
}

/* *********************************************************** */
