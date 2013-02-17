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

#pragma ident	"@(#)mlib_i_ImageColorHSL2RGB.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorHSL2RGB - converts image from HSL to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorHSL2RGB(mlib_image       *dst,
 *                                         const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
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
 *      0 <= S, L,L', V,P, Q,T, R,G, B <= 1
 *
 *      L' = | L     if L <= 1/2
 *           | 1 - L if L >  1/2
 *      V  = L + S * L'
 *      P  = L - S * L'
 *      Q  = L + S * L'* (1-2*fraction(H*6))
 *      T  = L - S * L'* (1-2*fraction(H*6))
 *
 *              | V, T,P if 0   <= H < 1/6
 *              | Q, V,P if 1/6 <= H < 2/6
 *      R, G,B = | P, V,T if 2/6 <= H < 3/6
 *              | P, Q,V if 3/6 <= H < 3/6
 *              | T, P,V if 4/6 <= H < 5/6
 *              | V, P,Q if 5/6 <= H < 1
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorHSL2RGB = __mlib_ImageColorHSL2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorHSL2RGB) mlib_ImageColorHSL2RGB
    __attribute__((weak, alias("__mlib_ImageColorHSL2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_c_ImageColorHSL2RGB_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorHSL2RGB_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorHSL2RGB_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorHSL2RGB_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorHSL2RGB(
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
		mlib_c_ImageColorHSL2RGB_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_c_ImageColorHSL2RGB_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_c_ImageColorHSL2RGB_U16(dst, src);
		break;
	case MLIB_INT:
		mlib_c_ImageColorHSL2RGB_S32(dst, src);
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
		mlib_s64 sign = src >> 31;			\
		dst = (src ^ sign) - sign;			\
	}

#define	ABS(src, dst)						\
	{							\
		mlib_s32 sign = src >> 31;			\
		dst = (src ^ sign) - sign;			\
	}

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
mlib_c_ImageColorHSL2RGB_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294;
	mlib_u32 mid_ind = 0x861;
	mlib_u32 min_ind = 0x50a;

	PREPAREVARS(mlib_u8);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 h = ps[0];
			mlib_s32 l = ps[2];
			mlib_s32 s = ps[1];
			mlib_s32 v, p, q;
			mlib_s32 sl2, fsl;
			mlib_u32 in;
			mlib_s32 h_a, h_b;

			in = ((h * 3) >> 6) & 0xe;

			sl2 = (s * ((l < 128)? l : 255 - l) + 127)/255;
			h_a = 3 * h;
			ABS(((h_a & 0xff) * 4 - 512), h_b);
			fsl = (sl2 * (256 - h_b) + 128) >> 8;

			v = l + (mlib_s32)sl2;
			p = l - (mlib_s32)sl2;
			q = l + (mlib_s32)fsl;

			pd[(max_ind >> in) & 3] = v;
			pd[(min_ind >> in) & 3] = p;
			pd[(mid_ind >> in) & 3] = q;

			ps += 3;
			pd += 3;
		}

		ps += sstride - 3 * dw;
		pd += dstride - 3 * dw;
	}
}

/* *********************************************************** */

void
mlib_c_ImageColorHSL2RGB_S16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 1;
	mlib_u32 mid_ind = 0x861 << 1;
	mlib_u32 min_ind = 0x50a << 1;

	PREPAREVARS(mlib_s16);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 hi = ((mlib_u16 *)ps)[0] ^ 0x8000;
			mlib_s32 li = ps[2];
			mlib_s32 s = (((mlib_u16 *)ps)[1] ^ 0x8000);
			mlib_s32 h = hi;
			mlib_s32 l = li;
			mlib_s32 l2, sl2;
			mlib_s32 v, p, q;
			mlib_u32 in;
			mlib_s32 ls;
			mlib_s32 h_a, h_b;

			ls = l >> 31;
			l2 = 32767 - (ls ^ l);
			sl2 = (s * l2 + (1 << 15)) >> 16;

			h_a = 3 * h;
			ABS(((h_a & 0xffff) * 4 - (1 << 17)), h_b);

			v = li + (mlib_s32)sl2;
			p = li - (mlib_s32)sl2;
			q = li + (mlib_s32)((sl2 * ((1 << 16) - h_b) +
						(1 << 15)) >> 16);

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_s16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_s16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    v;
			*(mlib_s16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}

/* *********************************************************** */

void
mlib_c_ImageColorHSL2RGB_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 1;
	mlib_u32 mid_ind = 0x861 << 1;
	mlib_u32 min_ind = 0x50a << 1;

	PREPAREVARS(mlib_u16);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s32 hi = ps[0];
			mlib_s32 li = ps[2] - 32768;
			mlib_s32 s = ps[1];
			mlib_s32 h = hi;
			mlib_s32 l = li;
			mlib_s32 l2, sl2;
			mlib_s32 v, p, q;
			mlib_u32 in;
			mlib_s32 ls;
			mlib_s32 h_a, h_b;

			ls = l >> 31;
			l2 = 32767 - (ls ^ l);
			sl2 = (s * l2) >> 16;

			h_a = 3 * h;
			ABS(((h_a & 0xffff) * 4 - (1 << 17)), h_b);

			v = li + (mlib_s32)sl2;
			p = li - (mlib_s32)sl2;
			q = li + (mlib_s32)((sl2 * ((1 << 16) - h_b)) >> 16);

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_s16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p + 32768;
			*(mlib_s16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    v + 32768;
			*(mlib_s16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q + 32768;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorHSL2RGB_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 2;
	mlib_u32 mid_ind = 0x861 << 2;
	mlib_u32 min_ind = 0x50a << 2;

	PREPAREVARS(mlib_s32);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_d64 h = (mlib_d64)ps[0] - (mlib_d64)MLIB_S32_MIN;
			mlib_d64 s = (mlib_d64)ps[1] - (mlib_d64)MLIB_S32_MIN;
			mlib_d64 l = (mlib_d64)ps[2];
			mlib_d64 v, p, q;
			mlib_d64 l2, sl2, fh2 = h * (3. / (MLIB_U32_MAX + 1.));
			mlib_d64 f, fh = fh2 * 2.0;
			mlib_u32 in;

			l2 = ((mlib_d64)MLIB_S32_MAX + 0.5) - mlib_fabs(l);

			fh2 = fh2 - (mlib_s32)fh2;
			f = fh2 * 4.0 - 2.0;
			sl2 = s * l2 * (1. / (mlib_d64)MLIB_U32_MAX);

			v = l + sl2;
			p = l - sl2;

			q = v - sl2 * mlib_fabs(f);

			in = ((mlib_s32)fh) << 1;

			*(mlib_s32 *)((mlib_u8 *)pd + ((min_ind >> in) & 0xc)) =
			    (mlib_s32)p;
			*(mlib_s32 *)((mlib_u8 *)pd + ((max_ind >> in) & 0xc)) =
			    (mlib_s32)v;
			*(mlib_s32 *)((mlib_u8 *)pd + ((mid_ind >> in) & 0xc)) =
			    (mlib_s32)q;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}

/* *********************************************************** */
#else
/* *********************************************************** */

void
mlib_c_ImageColorHSL2RGB_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294 << 2;
	mlib_u32 mid_ind = 0x861 << 2;
	mlib_u32 min_ind = 0x50a << 2;

	PREPAREVARS(mlib_s32);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_s64 h = (mlib_s64)ps[0] - (mlib_s64)MLIB_S32_MIN;
			mlib_s64 s = (mlib_s64)ps[1] - (mlib_s64)MLIB_S32_MIN;
			mlib_s32 l = (mlib_s32)ps[2];
			mlib_s32 v, p, q;
			mlib_u32 l_a;
			mlib_s64 l2, sl2, fh2;
			mlib_s64 fh_a, fh_b;
			mlib_s64 f, fh;
			mlib_u32 in;

			ABS(l, l_a);
			l2 = ((mlib_s64)MLIB_S32_MAX) - l_a;

			fh_a = 3 * h;
			fh = (fh_a)  >> 31;

			f = (fh_a & 0xffffffff) * 4 - ((mlib_s64)1 << 33);
			sl2 = (s * l2 + ((mlib_s64) 1 << 31)) >> 32;

			v = l + sl2;
			p = l - sl2;

			ABS64(f, fh_b);
			q = v - ((sl2 * fh_b + ((mlib_s64) 1 << 31)) >> 32);

			in = ((mlib_s32)fh) << 1;

			*(mlib_s32 *)((mlib_u8 *)pd + ((min_ind >> in) & 0xc)) =
			    (mlib_s32)p;
			*(mlib_s32 *)((mlib_u8 *)pd + ((max_ind >> in) & 0xc)) =
			    (mlib_s32)v;
			*(mlib_s32 *)((mlib_u8 *)pd + ((mid_ind >> in) & 0xc)) =
			    (mlib_s32)q;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}
#endif
/* *********************************************************** */
