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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageColorHSL2RGB.c	9.3	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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
			mlib_f32 sl2, fsl;
			mlib_u32 in;

			in = ((h * 3) >> 6) & 0xe;

			sl2 = mlib_U82F32[s] * mlib_HSL2RGB_L2[l];
			fsl = sl2 * mlib_HSL2RGB_F[h];

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
	mlib_u32 max_ind = 0x294;
	mlib_u32 mid_ind = 0x861;
	mlib_u32 min_ind = 0x50a;

	PREPAREVARS(mlib_s16);

	sstride -= 3 * dw;
	dstride -= 3 * dw;

	mlib_s32 nblock = dw >> 3;
	__m128i rl, gl, bl, rh, gh, bh, H, S, L, V, SL, H2, S2, L2, P, Q, T;
	__m128i zero, c12, c14, cmax, cf, shift, mask;

	zero = _mm_setzero_si128();
	c12  = _mm_set1_epi16(12);
	c14  = _mm_set1_epi16(14);
	cmax = _mm_set1_epi16(0x8000);
	cf   = _mm_set1_epi16(0x7fff);

	for (j = 0; j < dh; ++j) {
		for (i = 0; i < nblock; ++i) {
			/* load and shuffle */
			H = _mm_loadu_si128((__m128i *)(ps));
			S = _mm_loadu_si128((__m128i *)(ps + 8));
			L = _mm_loadu_si128((__m128i *)(ps + 16));

			rl = _mm_unpacklo_epi16(H, _mm_srli_si128(S, 8));
			gl = _mm_unpacklo_epi16(_mm_srli_si128(H, 8), L);
			bl = _mm_unpacklo_epi16(S, _mm_srli_si128(L, 8));

			rh = _mm_unpacklo_epi16(rl, _mm_srli_si128(gl, 8));
			gh = _mm_unpacklo_epi16(_mm_srli_si128(rl, 8), bl);
			bh = _mm_unpacklo_epi16(gl, _mm_srli_si128(bl, 8));

			H = _mm_unpacklo_epi16(rh, _mm_srli_si128(gh, 8));
			S = _mm_unpacklo_epi16(_mm_srli_si128(rh, 8), bh);
			L = _mm_unpacklo_epi16(gh, _mm_srli_si128(bh, 8));

			/* 3-channel units are stored in H, S, L */
			H = _mm_xor_si128(H, cmax);
			S = _mm_xor_si128(S, cmax);

			mask = _mm_cmpgt_epi16(zero, L);
			L2 = _mm_xor_si128(L, mask);
			L2 = _mm_sub_epi16(cf, L2);
			SL = _mm_mulhi_epu16(S, L2);

			H2 = _mm_slli_epi16(H, 1);
			H2 = _mm_add_epi16(H2, H);
			H2 = _mm_sub_epi16(H2, cmax);
			mask = _mm_cmpgt_epi16(zero, H2);
			H2 = _mm_xor_si128(H2, mask);
			H2 = _mm_sub_epi16(H2, mask);

			V = _mm_add_epi16(L, SL);
			P = _mm_sub_epi16(L, SL);

			Q = _mm_mulhi_epu16(H2, SL);
			T = _mm_mullo_epi16(H2, SL);
			Q = _mm_slli_epi16(Q, 2);
			T = _mm_srli_epi16(T, 14);
			Q = _mm_or_si128(Q, T);
			Q = _mm_sub_epi16(V, Q);

			shift = _mm_mulhi_epu16(H, c12);
			shift = _mm_and_si128(shift, c14);

			mlib_s16 v[8], p[8], q[8], in[8];
			_mm_storeu_si128((__m128i *)v, V);
			_mm_storeu_si128((__m128i *)p, P);
			_mm_storeu_si128((__m128i *)q, Q);
			_mm_storeu_si128((__m128i *)in, shift);
			for (int k = 0; k < 8; k++) {
				*(pd + ((max_ind >> in[k]) & 3)) = v[k];
				*(pd + ((mid_ind >> in[k]) & 3)) = q[k];
				*(pd + ((min_ind >> in[k]) & 3)) = p[k];
				pd += 3;
			}

			ps += 24;
		}
		for (i = nblock * 8; i < dw; ++i) {
			mlib_s32 hi = ((mlib_u16 *)ps)[0] ^ 0x8000;
			mlib_s32 li = ps[2];
			mlib_d64 s = (mlib_d64)(((mlib_u16 *)ps)[1] ^ 0x8000);
			mlib_d64 h = (mlib_d64)hi;
			mlib_d64 l = (mlib_d64)li;
			mlib_d64 l2, sl2;
			mlib_s32 v, p, q;
			mlib_u32 in;

			l2 = 32767.5 - mlib_fabs(l + 0.5);
			sl2 = s * l2 * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 4.0 - 2.0;

			v = li + (mlib_s32)sl2;
			p = li - (mlib_s32)sl2;
			q = li + (mlib_s32)(sl2 * (1. - mlib_fabs(h)));

			in = ((hi * 3) >> 14) & 0xe;

			*(pd + ((min_ind >> in) & 3)) = p;
			*(pd + ((max_ind >> in) & 3)) = v;
			*(pd + ((mid_ind >> in) & 3)) = q;
			ps += 3;
			pd += 3;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageColorHSL2RGB_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u32 max_ind = 0x294;
	mlib_u32 mid_ind = 0x861;
	mlib_u32 min_ind = 0x50a;

	PREPAREVARS(mlib_u16);

	sstride -= 3 * dw;
	dstride -= 3 * dw;

	mlib_s32 nblock = dw >> 3;
	__m128i rl, gl, bl, rh, gh, bh, H, S, L, V, SL, H2, S2, L2, P, Q, T;
	__m128i zero, c12, c14, cmax, shift, mask;

	zero = _mm_setzero_si128();
	c12  = _mm_set1_epi16(12);
	c14  = _mm_set1_epi16(14);
	cmax = _mm_set1_epi16(0x8000);

	for (j = 0; j < dh; ++j) {
		for (i = 0; i < nblock; ++i) {
			/* load and shuffle */
			H = _mm_loadu_si128((__m128i *)(ps));
			S = _mm_loadu_si128((__m128i *)(ps + 8));
			L = _mm_loadu_si128((__m128i *)(ps + 16));

			rl = _mm_unpacklo_epi16(H, _mm_srli_si128(S, 8));
			gl = _mm_unpacklo_epi16(_mm_srli_si128(H, 8), L);
			bl = _mm_unpacklo_epi16(S, _mm_srli_si128(L, 8));

			rh = _mm_unpacklo_epi16(rl, _mm_srli_si128(gl, 8));
			gh = _mm_unpacklo_epi16(_mm_srli_si128(rl, 8), bl);
			bh = _mm_unpacklo_epi16(gl, _mm_srli_si128(bl, 8));

			H = _mm_unpacklo_epi16(rh, _mm_srli_si128(gh, 8));
			S = _mm_unpacklo_epi16(_mm_srli_si128(rh, 8), bh);
			L = _mm_unpacklo_epi16(gh, _mm_srli_si128(bh, 8));

			/* 3-channel units are stored in H, S, L */

			mask = _mm_cmpgt_epi16(zero, L);
			L2 = _mm_sub_epi16(L, mask);
			L2 = _mm_xor_si128(L2, mask);
			L2 = _mm_sub_epi16(L2, mask);
			SL = _mm_mulhi_epu16(S, L2);

			H2 = _mm_slli_epi16(H, 1);
			H2 = _mm_add_epi16(H2, H);
			H2 = _mm_sub_epi16(H2, cmax);
			mask = _mm_cmpgt_epi16(zero, H2);
			H2 = _mm_xor_si128(H2, mask);
			H2 = _mm_sub_epi16(H2, mask);

			V = _mm_add_epi16(L, SL);
			P = _mm_sub_epi16(L, SL);

			Q = _mm_mulhi_epu16(H2, SL);
			T = _mm_mullo_epi16(H2, SL);
			Q = _mm_slli_epi16(Q, 2);
			T = _mm_srli_epi16(T, 14);
			Q = _mm_or_si128(Q, T);
			Q = _mm_sub_epi16(V, Q);

			shift = _mm_mulhi_epu16(H, c12);
			shift = _mm_and_si128(shift, c14);

			mlib_u16 v[8], p[8], q[8], in[8];
			_mm_storeu_si128((__m128i *)v, V);
			_mm_storeu_si128((__m128i *)p, P);
			_mm_storeu_si128((__m128i *)q, Q);
			_mm_storeu_si128((__m128i *)in, shift);
			for (int k = 0; k < 8; k++) {
				*(pd + ((max_ind >> in[k]) & 3)) = v[k];
				*(pd + ((mid_ind >> in[k]) & 3)) = q[k];
				*(pd + ((min_ind >> in[k]) & 3)) = p[k];
				pd += 3;
			}

			ps += 24;
		}
		for (i = nblock * 8; i < dw; ++i) {
			mlib_s32 hi = ps[0];
			mlib_s32 li = ps[2];
			mlib_d64 s = (mlib_d64)ps[1];
			mlib_d64 h = (mlib_d64)hi;
			mlib_d64 l = (mlib_d64)li - 32768;
			mlib_d64 l2, sl2;
			mlib_s32 v, p, q;
			mlib_u32 in;

			l2 = 32767.5 - mlib_fabs(l + 0.5);
			sl2 = s * l2 * (1. / (mlib_d64)MLIB_U16_MAX);

			h *= (3. / (MLIB_U16_MAX + 1.));
			h -= (mlib_s32)h;
			h = h * 4.0 - 2.0;

			v = li + (mlib_s32)sl2;
			p = li - (mlib_s32)sl2;
			q = li + (mlib_s32)(sl2 * (1. - mlib_fabs(h)));

			in = ((hi * 3) >> 14) & 0xe;

			*(pd + ((min_ind >> in) & 3)) = p;
			*(pd + ((max_ind >> in) & 3)) = v;
			*(pd + ((mid_ind >> in) & 3)) = q;
			ps += 3;
			pd += 3;
		}

		ps += sstride;
		pd += dstride;
	}
}

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
