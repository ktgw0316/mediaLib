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

#pragma ident	"@(#)mlib_ImageColorHSL2RGB_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorHSL2RGB_Fp - converts image from HSL to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorHSL2RGB_Fp(mlib_image       *dst,
 *                                            const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 *      R, G,B, S,L are in range [0., 1.]
 *      H          is in range [0., 1.)
 *
 *      If S = 0.0 then R = G=B = L;
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

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorHSL2RGB_Fp = __mlib_ImageColorHSL2RGB_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorHSL2RGB_Fp) mlib_ImageColorHSL2RGB_Fp
    __attribute__((weak, alias("__mlib_ImageColorHSL2RGB_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageColorHSL2RGB_Fp_F32(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_ImageColorHSL2RGB_Fp_D64(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorHSL2RGB_Fp(
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
	case MLIB_FLOAT:
		mlib_ImageColorHSL2RGB_Fp_F32(dst, src);
		break;
	case MLIB_DOUBLE:
		mlib_ImageColorHSL2RGB_Fp_D64(dst, src);
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
mlib_ImageColorHSL2RGB_Fp_F32(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x0294 << 2;
	mlib_u32 mid_ind = 0x1861 << 2;
	mlib_u32 min_ind = 0x250a << 2;
	PREPAREVARS(mlib_f32);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_f32 h = ps[0];
			mlib_f32 s = ps[1];
			mlib_f32 l = ps[2];
			mlib_f32 v, p, q;
			mlib_f32 l2, sl2, fh2 = (h * 3.0f);
			mlib_f32 f;
			mlib_s32 in;

			l2 = 0.5f - mlib_fabsf(0.5f - l);
			in = (mlib_s32)(h * 6.0f) << 1;

			fh2 = fh2 - (mlib_s32)fh2;
			f = fh2 * 4.0f - 2.0f;
			sl2 = s * l2;

			v = l + sl2;
			p = l - sl2;

			q = v - sl2 * (mlib_fabsf(f));

			*(mlib_f32 *)((mlib_u8 *)pd + ((min_ind >> in) & 0xc)) =
			    p;
			*(mlib_f32 *)((mlib_u8 *)pd + ((max_ind >> in) & 0xc)) =
			    v;
			*(mlib_f32 *)((mlib_u8 *)pd + ((mid_ind >> in) & 0xc)) =
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
mlib_ImageColorHSL2RGB_Fp_D64(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x0294 << 3;
	mlib_u32 mid_ind = 0x1861 << 3;
	mlib_u32 min_ind = 0x250a << 3;
	PREPAREVARS(mlib_d64);

	for (j = 0; j < dh; ++j) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dw; ++i) {
			mlib_d64 h = ps[0];
			mlib_d64 s = ps[1];
			mlib_d64 l = ps[2];
			mlib_d64 v, p, q;
			mlib_d64 l2, sl2, fh2;
			mlib_d64 f;
			mlib_s32 in;

			l2 = 0.5f - mlib_fabs(0.5f - l);
			fh2 = (h * 3.0);
			fh2 = fh2 - (mlib_s32)fh2;
			f = fh2 * 4.0 - 2.0;
			sl2 = s * l2;

			v = l + sl2;
			p = l - sl2;

			q = v - sl2 * (mlib_fabs(f));
			in = (mlib_s32)(h * 6.0) << 1;

			*(mlib_d64 *)((mlib_u8 *)pd +
			    ((min_ind >> in) & 0x18)) = p;
			*(mlib_d64 *)((mlib_u8 *)pd +
			    ((max_ind >> in) & 0x18)) = v;
			*(mlib_d64 *)((mlib_u8 *)pd +
			    ((mid_ind >> in) & 0x18)) = q;

			ps += 3;
			pd += 3;
		}

		ps += sstride - dw * 3;
		pd += dstride - dw * 3;
	}
}

/* *********************************************************** */
