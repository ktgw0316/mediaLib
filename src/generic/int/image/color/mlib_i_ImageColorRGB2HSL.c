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

#pragma ident	"@(#)mlib_i_ImageColorRGB2HSL.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorRGB2HSL - converts image from RGB to HSL
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2HSL(mlib_image       *dst,
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
 *      0 <= S, L,V, R,G, B,P <= 1
 *
 *      P = min(R, G,B)
 *      V = max(R, G,B)
 *
 *      L = (V + P) / 2
 *
 *      S = | (V - P) / (V + P)      if L <= 0.5
 *          | (V - P) / (2 - V - P)  if L > 0.5
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
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2HSL = __mlib_ImageColorRGB2HSL

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2HSL) mlib_ImageColorRGB2HSL
    __attribute__((weak, alias("__mlib_ImageColorRGB2HSL")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_c_ImageColorRGB2HSL_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorRGB2HSL_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorRGB2HSL_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorRGB2HSL_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2HSL(
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
		mlib_c_ImageColorRGB2HSL_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_c_ImageColorRGB2HSL_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_c_ImageColorRGB2HSL_U16(dst, src);
		break;
/*  handle MLIB_INT data type of image  */
	case MLIB_INT:
		mlib_c_ImageColorRGB2HSL_S32(dst, src);
		break;
/*  discard any other data types  */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	ABS64(src, dst)						\
	{							\
		mlib_s64 sign = (mlib_s64)src >> 63;			\
		dst = (src ^ sign) - sign;			\
	}

#define	dw	sw
#define	dh	sh

#define	PREPAREVARS(data_type)                                     \
	mlib_u32 sstride =                                         \
	mlib_ImageGetStride(src) / sizeof (data_type);             \
	    mlib_u32 dstride =                                     \
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
mlib_c_ImageColorRGB2HSL_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_u8 r = ps[i * 3 + 0];
			mlib_u8 g = ps[i * 3 + 1];
			mlib_u8 b = ps[i * 3 + 2];
			mlib_u8 v, minim;
			mlib_u32 fh, delta, sum, sumind, sub, s, div2;
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
			sum = v + minim;
			sumind =
			    ((sum ^ (((mlib_s32)sum << 23) >> 31)) -
			    (sum >> 8)) & 0xff;
			sub = v - minim;
			delta = mlib_div6_tab[sub];
			if (r == v) {
				dmin = g - b;
				shft = 0;
			} else if (g == v) {
				dmin = b - r;
				shft = 21845;
			}

			div2 = mlib_div1_tab[sumind];

			fh = (shft + dmin * (mlib_s32)delta) >> 8;
			s = ((mlib_s32)sub * (mlib_s32)div2 + 0x80) >> 8;

			pd[i * 3 + 2] = sum >> 1;
			pd[i * 3 + 0] = fh;
			pd[i * 3 + 1] = s;
		}

		ps += sstride;
		pd += dstride;
	}
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSL_S16(
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
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[i * 3 + 2] = v;
				pd[i * 3 + 1] = MLIB_S16_MIN;
				pd[i * 3 + 0] = MLIB_S16_MIN;
				continue;
			}

			if (r == v) {
				shft = 1.0f;
				dmin = (g - b);
			} else if (g == v) {
				shft = 0.333333333f;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			rdelta = (1.0f / 6.0f) / delta;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_f32)vdiv) - 32768;
			fh = shft + (mlib_f32)dmin *rdelta;

			h = ((mlib_s32)(fh * 65536.0f)) - 32768;

			pd[i * 3 + 2] = sum >> 1;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;
		}

		ps += sstride;
		pd += dstride;
	}
}
#else
/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSL_S16(
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
			mlib_s32 v, minim, s;
			mlib_s32 h;
			mlib_s32 delta, vdiv, sum;
			mlib_s32 fh, rdelta;
			mlib_s64 shft = 65536 * 4;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[i * 3 + 2] = v;
				pd[i * 3 + 1] = MLIB_S16_MIN;
				pd[i * 3 + 0] = MLIB_S16_MIN;
				continue;
			}

			if (r == v) {
				shft = 65536 * 6;
				dmin = (g - b);
			} else if (g == v) {
				shft = 65536 * 2;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_s32)vdiv) - 32768;

			h = ((mlib_s32)(shft + ((mlib_s64)dmin * \
				65536)/(delta)))/6 - 32768;

			pd[i * 3 + 2] = sum >> 1;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;
		}

		ps += sstride;
		pd += dstride;
	}
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSL_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 r = ps[i * 3 + 0] - 32768;
			mlib_s32 g = ps[i * 3 + 1] - 32768;
			mlib_s32 b = ps[i * 3 + 2] - 32768;
			mlib_s32 v, minim, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[i * 3 + 2] = v + 32768;
				pd[i * 3 + 1] = MLIB_U16_MIN;
				pd[i * 3 + 0] = MLIB_U16_MIN;
				continue;
			}

			if (r == v) {
				shft = 1.0f;
				dmin = (g - b);
			} else if (g == v) {
				shft = 0.333333333f;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			rdelta = (1.0f / 6.0f) / delta;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_f32)vdiv);
			fh = shft + (mlib_f32)dmin *rdelta;

			h = ((mlib_s32)(fh * 65536.0f));

			pd[i * 3 + 2] = (sum >> 1) + 32768;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;
		}

		ps += sstride;
		pd += dstride;
	}
}
#else

/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSL_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 r = ps[i * 3 + 0] - 32768;
			mlib_s32 g = ps[i * 3 + 1] - 32768;
			mlib_s32 b = ps[i * 3 + 2] - 32768;
			mlib_s32 v, minim, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_s32 fh, rdelta;
			mlib_s64 shft = 65536 * 4;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[i * 3 + 2] = v + 32768;
				pd[i * 3 + 1] = MLIB_U16_MIN;
				pd[i * 3 + 0] = MLIB_U16_MIN;
				continue;
			}

			if (r == v) {
				shft = 65536 * 6;
				dmin = (g - b);
			} else if (g == v) {
				shft = 65536 * 2;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_s32)vdiv);

			h = ((mlib_s32)(shft + ((mlib_s64)dmin * \
					65536)/delta)/6);

			pd[i * 3 + 2] = (sum >> 1) + 32768;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;
		}

		ps += sstride;
		pd += dstride;
	}
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSL_S32(
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
			mlib_d64 delta, vdiv, sum;
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

			sum = v + minim;

			vdiv = (mlib_d64)(MLIB_U32_MAX) - mlib_fabs(sum + 1.);
			s = delta / vdiv;

			s = s * (mlib_d64)MLIB_U32_MAX + (mlib_d64)MLIB_S32_MIN;
			h = shft + dmin * rdelta;
			h = h - (mlib_s32)h;
			h = h * (mlib_d64)MLIB_U32_MAX + (mlib_d64)MLIB_S32_MIN;

			pd[i * 3 + 2] = (mlib_s32)(sum * 0.5);
			pd[i * 3 + 1] = (mlib_s32)s;
			pd[i * 3 + 0] = (mlib_s32)h;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */
#else
/* *********************************************************** */

void
mlib_c_ImageColorRGB2HSL_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s32);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s64 r = ps[i * 3 + 0];
			mlib_s64 g = ps[i * 3 + 1];
			mlib_s64 b = ps[i * 3 + 2];
			mlib_s64 v, minim, s;
			mlib_u64 vdiv;
			mlib_s64 sum_a, h, delta, sum;
			mlib_s64 rdelta, s1;
			mlib_s64 shft = ((mlib_s64)MLIB_U32_MAX +1) * 2;
			mlib_s64 dmin = r - g;

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
				shft = ((mlib_s64)MLIB_U32_MAX + 1) * 3;
				dmin = g - b;
			} else if (g == v) {
				shft = ((mlib_s64)MLIB_U32_MAX + 1) * 1;
				dmin = b - r;
			}

			delta = (v - minim);

			sum = v + minim;

			ABS64((mlib_s64)(sum + 1), sum_a);
			vdiv = (mlib_s64)(MLIB_U32_MAX) - sum_a;
			s = ((mlib_u64)delta * ((mlib_u64)MLIB_S32_MAX \
						+ 1))/ vdiv;

			s = (s << 1) + (mlib_s64)MLIB_S32_MIN;
			h = (shft + (mlib_s64)(((mlib_s64)dmin * \
					(((mlib_u64)MLIB_U32_MAX + \
					1) >> 1)))/delta + 3)/6;
			h = ((mlib_s64)(h << 1) & (mlib_s64)0xffffffff)\
				+ (mlib_s64)MLIB_S32_MIN;

			pd[i * 3 + 2] = (mlib_s32)(sum >> 1);
			pd[i * 3 + 1] = (mlib_s32)s;
			pd[i * 3 + 0] = (mlib_s32)h;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */
#endif
/* *********************************************************** */
