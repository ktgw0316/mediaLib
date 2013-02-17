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

#pragma ident	"@(#)mlib_s_ImageBlendRGBA2BGRA.c	9.6	07/11/05 SMI"

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
 *      where, src_{r, g, b, a} are the RGB and alpha values of a src pixel;
 *      dst_{r, g, b, a} are the RGB and alpha values of the corresponding dst
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
#include <mlib_ImageCopy.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlendRGBA2BGRA = __mlib_ImageBlendRGBA2BGRA

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageBlendRGBA2BGRA) mlib_ImageBlendRGBA2BGRA
    __attribute__((weak, alias("__mlib_ImageBlendRGBA2BGRA")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union {
	__m128 m128;
	__m128d m128d;
	__m128i m128i;
} mlib_m128;

/* *********************************************************** */

#define	P_TYPE	__m128i

/* *********************************************************** */

#define	BLEND(dres, alp, ss, dd)                                \
	ralp = _mm_sub_epi16(const255, alp);                    \
	d0 = _mm_mullo_epi16(ss, alp);                          \
	d1 = _mm_mullo_epi16(dd, ralp);                         \
	dres = _mm_add_epi16(d0, d1);                           \
	drnd = _mm_srli_epi16(dres, 8);                         \
	dres = _mm_add_epi16(dres, d_half);                     \
	dres = _mm_add_epi16(dres, drnd);                       \
	dres = _mm_srli_epi16(dres, 8)

/* *********************************************************** */

mlib_status
__mlib_ImageBlendRGBA2BGRA(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_u8 *sl, *dl;
	mlib_s32 slb, dlb, nchan, width, height;
	mlib_s32 i, j, ii, off;
	P_TYPE *sp, *dp;
	P_TYPE ss, aa, ds, dd, d_h, d_l;
	P_TYPE mzero, const255, mask64, d_half;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dlb, dl);
	slb = mlib_ImageGetStride(src);
	sl = mlib_ImageGetData(src);

	if (type != MLIB_BYTE || nchan != 4) {
		return (MLIB_FAILURE);
	}

	mzero = _mm_setzero_si128();
	const255 = _mm_set1_epi32(0x00ff00ff);
	mask64 = _mm_set1_epi32(0x00ffffff);
	d_half = _mm_set1_epi32(0x00800080);

	for (j = 0; j < height; j++) {
		P_TYPE alp, a0, a1, ralp, s0, s1, d0, d1, drnd;
		mlib_m128 s0u, s1u;

		sp = (void *)sl;
		dp = (void *)dl;

		{
			for (i = 0; i < (width / 4); i++) {
				s0u.m128d = _mm_load_sd((mlib_d64 *)sp);
				s1u.m128d = _mm_load_sd((mlib_d64 *)sp + 1);
				s0 = _mm_unpacklo_epi8(s0u.m128i, mzero);
				s1 = _mm_unpacklo_epi8(s1u.m128i, mzero);
				dd = _mm_loadu_si128(dp);

				a0 = _mm_shufflelo_epi16(s0, 0xff);
				a0 = _mm_shufflehi_epi16(a0, 0xff);
				s0 = _mm_shufflelo_epi16(s0, 0xc6);
				s0 = _mm_shufflehi_epi16(s0, 0xc6);
				BLEND(d_h, a0, s0, _mm_unpacklo_epi8(dd,
				    mzero));
				a1 = _mm_shufflelo_epi16(s1, 0xff);
				a1 = _mm_shufflehi_epi16(a1, 0xff);
				s1 = _mm_shufflelo_epi16(s1, 0xc6);
				s1 = _mm_shufflehi_epi16(s1, 0xc6);
				BLEND(d_l, a1, s1, _mm_unpackhi_epi8(dd,
				    mzero));

				d_h = _mm_packus_epi16(d_h, d_l);
				d_h = _mm_or_si128(_mm_and_si128(mask64, d_h),
				    _mm_andnot_si128(mask64, dd));

				_mm_storeu_si128(dp, d_h);

				sp++;
				dp++;
			}
		}

		if (width & 3) {
			s0u.m128d = _mm_load_sd((mlib_d64 *)sp);
			s1u.m128d = _mm_load_sd((mlib_d64 *)sp + 1);
			s0 = _mm_unpacklo_epi8(s0u.m128i, mzero);
			s1 = _mm_unpacklo_epi8(s1u.m128i, mzero);
			dd = _mm_loadu_si128(dp);

			a0 = _mm_shufflelo_epi16(s0, 0xff);
			a0 = _mm_shufflehi_epi16(a0, 0xff);
			s0 = _mm_shufflelo_epi16(s0, 0xc6);
			s0 = _mm_shufflehi_epi16(s0, 0xc6);
			BLEND(d_h, a0, s0, _mm_unpacklo_epi8(dd, mzero));
			a1 = _mm_shufflelo_epi16(s1, 0xff);
			a1 = _mm_shufflehi_epi16(a1, 0xff);
			s1 = _mm_shufflelo_epi16(s1, 0xc6);
			s1 = _mm_shufflehi_epi16(s1, 0xc6);
			BLEND(d_l, a1, s1, _mm_unpackhi_epi8(dd, mzero));

			d_h = _mm_packus_epi16(d_h, d_l);
			d_h = _mm_or_si128(_mm_and_si128(mask64, d_h),
			    _mm_andnot_si128(mask64, dd));

			for (ii = 0; ii < (width & 3); ii++) {
				((mlib_s32 *)dp)[ii] = ((mlib_s32 *)&d_h)[ii];
			}
		}

		sl += slb;
		dl += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
