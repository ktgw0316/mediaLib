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

#pragma ident	"%w%	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorHSV2RGB - converts image from HSV to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorHSV2RGB(mlib_image       *dst,
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
 *      0 <= S, V,P, Q,T, R,G, B <= 1
 *
 *      P  = V * (1 - S)
 *      Q  = V * (1 - S * fraction(H*6))
 *      T  = V * (1 - S * (1-fraction(H*6)))
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

#pragma weak mlib_ImageColorHSV2RGB = __mlib_ImageColorHSV2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorHSV2RGB) mlib_ImageColorHSV2RGB
    __attribute__((weak, alias("__mlib_ImageColorHSV2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_c_ImageColorHSV2RGB_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorHSV2RGB_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorHSV2RGB_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_c_ImageColorHSV2RGB_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

mlib_status
__mlib_ImageColorHSV2RGB(
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
		mlib_c_ImageColorHSV2RGB_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_c_ImageColorHSV2RGB_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_c_ImageColorHSV2RGB_U16(dst, src);
		break;
	case MLIB_INT:
		mlib_c_ImageColorHSV2RGB_S32(dst, src);
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

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorHSV2RGB_U8(
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
			mlib_s32 v = ps[2];
			mlib_s32 s = ps[1];
			mlib_s32 p, q;
			mlib_f32 sv, fsv;
			mlib_u32 in;

			in = ((h * 3) >> 6) & 0xe;

			sv = mlib_U82F32[s] * mlib_U82F32[v] * (.5f / 255.f);
			fsv = sv - sv * mlib_HSL2RGB_F[h];
			p = v - (mlib_s32)(sv + sv);

			q = v - (mlib_s32)fsv;

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
#else
/* *********************************************************** */

void
mlib_c_ImageColorHSV2RGB_U8(
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
			mlib_s32 v = ps[2];
			mlib_s32 s = ps[1];
			mlib_s32 p, q;
			mlib_s32 h_a, h_b, sv1;
			mlib_s32 sv, fsv;
			mlib_u32 in;
			mlib_s64 sv_a;

			in = ((h * 3) >> 6) & 0xe;

			sv_a = ((mlib_s64)s * (mlib_s64)v * 538976288);
			h_a = h * 3;
			ABS(((h_a & 0xff) * 4 - 512), h_b);
			fsv = (mlib_s32)(((mlib_s64)sv_a * h_b + \
					((mlib_s64)1 << 45)) >> 46);
			p = v - (mlib_s32)((sv_a + 		 \
					((mlib_s64) 1 << 36))>> 37);

			q = v - (mlib_s32)fsv;

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
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorHSV2RGB_S16(
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
			mlib_s32 vi = ps[2];
			mlib_d64 s = (((mlib_u16 *)ps)[1] ^ 0x8000);
			mlib_d64 h = hi;
			mlib_d64 v = (vi + 0x8000);
			mlib_d64 sv;
			mlib_s32 p, q;
			mlib_u32 in;

			sv = s * v * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 2.0 - 1.0;

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)(sv * mlib_fabs(h));

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_s16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_s16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
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
#else
/* *********************************************************** */

void
mlib_c_ImageColorHSV2RGB_S16(
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
			mlib_s32 vi = ps[2];
			mlib_s32 s = (((mlib_u16 *)ps)[1] ^ 0x8000);
			mlib_s32 h = hi;
			mlib_s32 v = (vi + 0x8000);
			mlib_s32 sv;
			mlib_s32 p, q;
			mlib_u32 in;
			mlib_s32 h_a, h_b;

			sv = ((mlib_s64)s * (mlib_s64)v + (1 << 15)) >> 16;

			h_a = 3 * h;
			ABS(((h_a & 0xffff) * 2 - (1 << 16)), h_b);

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)((sv * h_b + (1 << 15)) >> 16);

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_s16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_s16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_s16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorHSV2RGB_U16(
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
			mlib_s32 vi = ps[2];
			mlib_d64 s = ps[1];
			mlib_d64 h = hi;
			mlib_d64 v = vi;
			mlib_d64 sv;
			mlib_s32 p, q;
			mlib_u32 in;

			sv = s * v * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 2.0 - 1.0;

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)(sv * mlib_fabs(h));

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_u16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_u16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_u16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
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
mlib_c_ImageColorHSV2RGB_U16(
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
			mlib_s32 vi = ps[2];
			mlib_s32 s = ps[1];
			mlib_s32 h = hi;
			mlib_s32 v = vi;
			mlib_s32 sv;
			mlib_s32 p, q;
			mlib_u32 in;
			mlib_s32 h_a, h_b;

			sv = ((mlib_s64)s * (mlib_s64)v + (1 << 15)) >> 16;

			h_a = 3 * h;
			ABS(((h_a & 0xffff) * 2 - (1 << 16)), h_b);

			p = vi - (mlib_s32)sv;
			q = vi - (mlib_s32)((sv * h_b + (1 << 15)) >> 16);

			in = ((hi * 3) >> 14) & 0xe;

			*(mlib_u16 *)((mlib_u8 *)pd + ((min_ind >> in) & 6)) =
			    p;
			*(mlib_u16 *)((mlib_u8 *)pd + ((max_ind >> in) & 6)) =
			    vi;
			*(mlib_u16 *)((mlib_u8 *)pd + ((mid_ind >> in) & 6)) =
			    q;
			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

void
mlib_c_ImageColorHSV2RGB_S32(
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
			mlib_d64 v0 = (mlib_d64)ps[2];
			mlib_d64 v = v0 - (mlib_d64)MLIB_S32_MIN;
			mlib_d64 p, q;
			mlib_d64 sv, fh2 = h * (3. / (MLIB_U32_MAX + 1.));
			mlib_d64 f, fh = fh2 * 2.0;
			mlib_s32 vi = ps[2];
			mlib_u32 in;

			fh2 = fh2 - (mlib_s32)fh2;
			f = fh2 * 2.0 - 1.0;
			sv = s * v * (1. / (mlib_d64)MLIB_U32_MAX);

			p = v0 - sv;
			q = v0 - sv * mlib_fabs(f);

			in = ((mlib_s32)fh) << 1;

			*(mlib_s32 *)((mlib_u8 *)pd + ((max_ind >> in) & 0xc)) =
			    vi;
			*(mlib_s32 *)((mlib_u8 *)pd + ((min_ind >> in) & 0xc)) =
			    (mlib_s32)p;
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
mlib_c_ImageColorHSV2RGB_S32(
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
			mlib_s64 v0 = (mlib_s64)ps[2];
			mlib_s64 v = v0 - (mlib_s64)MLIB_S32_MIN;
			mlib_s32 p, q;
			mlib_s64 fh_a, fh_b;
			mlib_s64 sv, fh2;
			mlib_u64 tmpsv;
			mlib_s64 f, fh;
			mlib_s32 vi = ps[2];
			mlib_u32 in;

			fh2 = h * 3;
			fh = fh2 >> 31;
			f = (fh2 & 0xffffffff) * 2 - ((mlib_s64)1 << 32);
			tmpsv = (mlib_u64)s * (mlib_u64)v;
			sv = (tmpsv + (tmpsv >> 32)) >> 32;

			p = v0 - sv;
			ABS64(f, fh_b);
			q = v0 - ((sv * fh_b + ((mlib_s64) 1 << 31)) >> 32);

			in = ((mlib_s32)fh) << 1;

			*(mlib_s32 *)((mlib_u8 *)pd + ((max_ind >> in) & 0xc)) =
			    vi;
			*(mlib_s32 *)((mlib_u8 *)pd + ((min_ind >> in) & 0xc)) =
			    (mlib_s32)p;
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

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
#endif
/* *********************************************************** */
