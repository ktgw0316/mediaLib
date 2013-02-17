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

#pragma ident	"@(#)mlib_c_ImageColorRGB2HSV.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorRGB2HSV - converts image from RGB to HSV
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2HSV(mlib_image       *dst,
 *                                         const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * MLIB_BYTE case notes:
 *      ranges for all values supposed to be [MLIB_U8_MIN; MLIB_U8_MAX]
 *      if R==G==B then H = S=MLIB_U8_MIN;
 *
 * MLIB_SHORT case notes:
 *      ranges for all values supposed to be [MLIB_S16_MIN; MLIB_S16_MAX]
 *      if R==G==B then H = S=MLIB_S16_MIN;
 *
 * MLIB_USHORT case notes:
 *      ranges for all values supposed to be [MLIB_U16_MIN; MLIB_U16_MAX]
 *      if R==G==B then H = S=MLIB_U16_MIN;
 *
 * MLIB_INT case notes:
 *      ranges for all values supposed to be [MLIB_S32_MIN; MLIB_S32_MAX]
 *      if R==G==B then H = S=MLIB_S32_MIN;
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number
 *      of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      data values are mapped to next ranges
 *      0 <= H < 1
 *      0 <= S, V,R, G,B, P <= 1
 *
 *      P = min(R, G,B)
 *
 *      V = max(R, G,B)
 *
 *      S = (V - P) / V
 *
 *          | (5 + (V-B)/(V-P))/6  if R==V and G==P
 *          | (1 - (V-G)/(V-P))/6  if R==V and B==P
 *      H = | (1 + (V-R)/(V-P))/6  if G==V and B==P
 *          | (3 - (V-B)/(V-P))/6  if G==V and R==P
 *          | (3 + (V-G)/(V-P))/6  if B==V and R==P
 *          | (5 - (V-R)/(V-P))/6  if B==V and G==P
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2HSV = __mlib_ImageColorRGB2HSV

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2HSV) mlib_ImageColorRGB2HSV
    __attribute__((weak, alias("__mlib_ImageColorRGB2HSV")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_c_ImageColorRGB2HSV_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorRGB2HSV_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorRGB2HSV_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorRGB2HSV_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2HSV(
    mlib_image *dst,
    const mlib_image *src)
{
/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	switch (mlib_ImageGetType(dst)) {
/*  handle MLIB_BYTE data type of image  */
	case MLIB_BYTE:
		mlib_c_ImageColorRGB2HSV_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_c_ImageColorRGB2HSV_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_c_ImageColorRGB2HSV_U16(dst, src);
		break;
/*  handle MLIB_INT data type of image  */
	case MLIB_INT:
		mlib_c_ImageColorRGB2HSV_S32(dst, src);
		break;
/*  discard any other data types  */
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
mlib_c_ImageColorRGB2HSV_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_u8 r = ps[0];
			mlib_u8 g = ps[1];
			mlib_u8 b = ps[2];
			mlib_u32 v, minim, s;
			mlib_u32 fh, delta;
			mlib_u32 shft = 43690;
			mlib_s32 dmin = r - g;

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;
			if (minim > b)
				minim = b;
			if (b > v)
				v = b;

			delta = mlib_div6_tab[(v - minim)];
			s = ((mlib_s32)(v -
			    minim) * (mlib_s32)mlib_div1_tab[v] + 0x80) >> 8;

			if (r == v) {
				shft = 0;
				dmin = g - b;
			} else if (g == v) {
				shft = 21845;
				dmin = b - r;
			}

			fh = shft + ((mlib_s32)(dmin) * (mlib_s32)(delta));

			pd[2] = v;
			pd[1] = s;
			pd[0] = ((fh) >> 8);

			ps += 3;
			pd += 3;
		}

		ps += sstride - 3 * dw;
		pd += dstride - 3 * dw;
	}
}

/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSV_S16(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s16);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 r = ps[i * 3 + 0];
			mlib_s32 g = ps[i * 3 + 1];
			mlib_s32 b = ps[i * 3 + 2];
			mlib_s32 v, minim, h, s;
			mlib_s32 dmin;
			mlib_f32 shft;
			mlib_f32 vdiv;
			mlib_f32 delta, rdelta, rvdiv;

			if (r > g) {
				minim = g;
				v = r;
				shft = (65536.0f - 32768.f);
				dmin = (g - b);
			} else {
				v = g;
				minim = r;
				shft = (0.333333333f * 65536.0f - 32768.f);
				dmin = (b - r);
			}

			if (b > v) {
				v = b;
				shft = (0.666666667f * 65536.0f - 32768.f);
				dmin = (r - g);
			}

			vdiv = (mlib_f32)(v + 32768);

			if (minim > b)
				minim = b;

			delta = (mlib_f32)(v - minim);

			rvdiv = 65535.0f / vdiv;
			rdelta = (65536.0f / 6.0f) / delta;

			s = (mlib_s32)(delta * rvdiv) - 32768;
			h = (mlib_s32)((mlib_f32)dmin * rdelta + shft);

			pd[i * 3 + 2] = v;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;

			if (v == minim) {
				pd[i * 3 + 1] = MLIB_S16_MIN;
				pd[i * 3 + 0] = MLIB_S16_MIN;
			}
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSV_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 r = ps[i * 3 + 0];
			mlib_s32 g = ps[i * 3 + 1];
			mlib_s32 b = ps[i * 3 + 2];
			mlib_s32 v, minim, h, s;
			mlib_s32 dmin;
			mlib_f32 shft;
			mlib_f32 vdiv;
			mlib_f32 delta, rdelta, rvdiv;

			if (r > g) {
				minim = g;
				v = r;
				shft = 65536.0f;
				dmin = (g - b);
			} else {
				v = g;
				minim = r;
				shft = 0.333333333f * 65536.0f;
				dmin = (b - r);
			}

			if (b > v) {
				v = b;
				shft = 0.666666667f * 65536.0f;
				dmin = (r - g);
			}

			vdiv = (mlib_f32)v;

			if (minim > b)
				minim = b;

			delta = (mlib_f32)(v - minim);

			rvdiv = 65535.0f / vdiv;
			rdelta = (65536.0f / 6.0f) / delta;

			s = (mlib_s32)(delta * rvdiv);
			h = (mlib_s32)((mlib_f32)dmin * rdelta + shft);

			pd[i * 3 + 2] = v;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;

			if (v == minim) {
				pd[i * 3 + 1] = MLIB_U16_MIN;
				pd[i * 3 + 0] = MLIB_U16_MIN;
			}
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSV_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s32);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_d64 r = ps[i * 3 + 0];
			mlib_d64 g = ps[i * 3 + 1];
			mlib_d64 b = ps[i * 3 + 2];
			mlib_d64 v, minim, h, s;
			mlib_d64 delta, vdiv;
			mlib_d64 rdelta;
			mlib_d64 shft = 2.0 / 3.0;
			mlib_d64 dmin = r - g;

			v = minim = r;

			if (r > g)
				minim = g;

			if (r <= g)
				v = g;

			if (minim > b)
				minim = b;

			if (b > v)
				v = b;

			if (v == minim) {
				pd[i * 3 + 2] = ps[i * 3 + 0];
				pd[i * 3 + 1] = MLIB_S32_MIN;
				pd[i * 3 + 0] = MLIB_S32_MIN;
				continue;
			}

			if (r == v) {
				shft = 1.0;
				dmin = g - b;
			} else if (g == v) {
				shft = 1.0 / 3.0;
				dmin = b - r;
			}

			delta = (v - minim);
			rdelta = 1.0 / delta;
			rdelta *= (1.0 / 6.0);

			vdiv = v - (mlib_d64)(MLIB_S32_MIN);
			s = delta / vdiv;

			s = s * (mlib_d64)MLIB_U32_MAX + (mlib_d64)MLIB_S32_MIN;
			h = shft + dmin * rdelta;
			h = h - (mlib_s32)h;
			h = h * (mlib_d64)MLIB_U32_MAX + (mlib_d64)MLIB_S32_MIN;

			pd[i * 3 + 2] = (mlib_s32)v;
			pd[i * 3 + 1] = (mlib_s32)s;
			pd[i * 3 + 0] = (mlib_s32)h;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */
