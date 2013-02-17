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

#pragma ident	"@(#)mlib_ImageColorRGB2HSV_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorRGB2HSV_Fp - converts image from RGB to HSV
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2HSV_Fp(mlib_image       *dst,
 *                                            const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 *      R, G,B, S,V are in range [0., 1.]
 *      H          is in range [0., 1.)
 *
 * MLIB_FLOAT case notes:
 *      if R==G==B then H = S=0.0f;
 *
 * MLIB_DOUBLE case notes:
 *      if R==G==B then H = S=0.0;
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number
 *      of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      P = min(R, G,B)
 *
 *      V = max(R, G,B)
 *
 *      S = (V - P) / V
 *
 *          | (5 + (V-B)/(V-P))/6  if R==V and G==P
 *          | (1 + (V-G)/(V-P))/6  if R==V and B==P
 *      H = | (1 + (V-R)/(V-P))/6  if G==V and B==P
 *          | (3 + (V-B)/(V-P))/6  if G==V and R==P
 *          | (3 + (V-G)/(V-P))/6  if B==V and R==P
 *          | (5 + (V-R)/(V-P))/6  if B==V and G==P
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2HSV_Fp = __mlib_ImageColorRGB2HSV_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2HSV_Fp) mlib_ImageColorRGB2HSV_Fp
    __attribute__((weak, alias("__mlib_ImageColorRGB2HSV_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageColorRGB2HSV_Fp_F32(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_ImageColorRGB2HSV_Fp_D64(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2HSV_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	switch (mlib_ImageGetType(src)) {
	case MLIB_FLOAT:
		mlib_ImageColorRGB2HSV_Fp_F32(dst, src);
		break;
	case MLIB_DOUBLE:
		mlib_ImageColorRGB2HSV_Fp_D64(dst, src);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	dw	sw
#define	dh	sh

#define	PREPAREVARS(data_type)                                     \
	mlib_s32 sstride =                                         \
	mlib_ImageGetStride(src) / sizeof (data_type);             \
	    mlib_s32 dstride =                                     \
		mlib_ImageGetStride(dst) / sizeof (data_type);     \
	    mlib_s32 ws_ = mlib_ImageGetWidth(src);                \
	    mlib_s32 hs_ = mlib_ImageGetHeight(src);               \
	    mlib_s32 wd_ = mlib_ImageGetWidth(dst);                \
	    mlib_s32 hd_ = mlib_ImageGetHeight(dst);               \
	    mlib_s32 sw = (ws_ < wd_) ? ws_ : wd_;                 \
	    mlib_s32 sh = (hs_ < hd_) ? hs_ : hd_;                 \
	    mlib_s32 dx = ((mlib_s32)ws_ - (mlib_s32)wd_) >> 1;    \
	    mlib_s32 dy = ((mlib_s32)hs_ - (mlib_s32)hd_) >> 1;    \
	    mlib_s32 dxs = ((dx > 0) ? dx : 0);                    \
	    mlib_s32 dxd = ((dx > 0) ? 0 : -dx);                   \
	    mlib_s32 dys = ((dy > 0) ? dy : 0);                    \
	    mlib_s32 dyd = ((dy > 0) ? 0 : -dy);                   \
	    data_type *sdata =                                     \
		(data_type *) mlib_ImageGetData(src) + dxs * 3 +   \
	    sstride * dys;                                         \
	    data_type *ddata =                                     \
		(data_type *) mlib_ImageGetData(dst) + dxd * 3 +   \
	    dstride * dyd;                                         \
	    data_type *ps = sdata;                                 \
	    data_type *pd = ddata;                                 \
	    mlib_s32 i, j

/* *********************************************************** */

void
mlib_ImageColorRGB2HSV_Fp_F32(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_f32);

	for (j = 0; j < dh; ++j) {
		mlib_f32 *psrc = ps, *pdst = pd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; i++) {
			mlib_f32 r = psrc[0];
			mlib_f32 g = psrc[1];
			mlib_f32 b = psrc[2];
			mlib_f32 v, minim, h, s;
			mlib_f32 delta;
			mlib_f32 rdelta;
			mlib_f32 shft;
			mlib_f32 dmin;

			v = minim = r;

			if (r > g)
				minim = g;
			if (r <= g)
				v = g;
			if (minim > b)
				minim = b;
			if (b > v)
				v = b;

			delta = v - minim;
			rdelta = (1.0f / 6.0f) / delta;

			shft = 2.0f / 3.0f;
			dmin = r - g;

			if (r == v)
				shft = 1.0f;
			if (r == v)
				dmin = g - b;
			if (g == v)
				shft = 1.0f / 3.0f;
			if (g == v)
				dmin = b - r;

			s = delta / v;

			h = shft + dmin * rdelta;
			h = h - (mlib_s32)h;

			if (v == minim)
				h = 0.0f;
			if (v == minim)
				s = 0.0f;

			pdst[0] = h;
			pdst[1] = s;
			pdst[2] = v;
			psrc += 3;
			pdst += 3;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */

void
mlib_ImageColorRGB2HSV_Fp_D64(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64);

	for (j = 0; j < dh; ++j) {
		mlib_d64 *psrc = ps, *pdst = pd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; i++) {
			mlib_d64 r = psrc[0];
			mlib_d64 g = psrc[1];
			mlib_d64 b = psrc[2];
			mlib_d64 v, minim, h, s;
			mlib_d64 delta;
			mlib_d64 rdelta;
			mlib_d64 shft;
			mlib_d64 dmin;

			v = minim = r;

			dmin = r - g;

			if (r > g)
				minim = g;
			if (r <= g)
				v = g;
			if (minim > b)
				minim = b;
			if (b > v)
				v = b;

			delta = v - minim;
			shft = 2.0 / 3.0;
			rdelta = (1.0 / 6.0) / delta;

			if (r == v)
				shft = 1.0;
			if (r == v)
				dmin = g - b;
			if (g == v)
				shft = 1.0 / 3.0;
			if (g == v)
				dmin = b - r;

			s = delta / v;

			h = shft + dmin * rdelta;
			h = h - (mlib_s32)h;

			if (v == minim)
				h = 0.0;
			if (v == minim)
				s = 0.0;

			pdst[0] = h;
			pdst[1] = s;
			pdst[2] = v;

			psrc += 3;
			pdst += 3;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */
