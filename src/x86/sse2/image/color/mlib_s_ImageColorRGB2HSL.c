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

#pragma ident	"@(#)mlib_s_ImageColorRGB2HSL.c	9.4	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

static void mlib_s_ImageColorRGB2HSL_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_s_ImageColorRGB2HSL_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_s_ImageColorRGB2HSL_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_s_ImageColorRGB2HSL_S32(
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
		mlib_s_ImageColorRGB2HSL_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_s_ImageColorRGB2HSL_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_s_ImageColorRGB2HSL_U16(dst, src);
		break;
/*  handle MLIB_INT data type of image  */
	case MLIB_INT:
		mlib_s_ImageColorRGB2HSL_S32(dst, src);
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
mlib_s_ImageColorRGB2HSL_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8);
	sstride -= 3 * dw;
	dstride -= 3 * dw;

	mlib_s32 nblock = dw >> 3;
	__m128i rr, gg, bb, rl, gl, bl, rh, gh, bh, P, V, S, H, L, VP, SS;
	__m128i zero, ones, shft0, shft1, shft2, div, s510;
	__m128i bmask, mask1, mask2, mask3, maska, maskb, maskc, maskd;
	zero  = _mm_setzero_si128();
	ones  = _mm_set1_epi16(0xffff);
	s510  = _mm_set1_epi16(510);
	shft0 = _mm_set1_epi32(128);
	shft1 = _mm_set1_epi16(21845);
	shft2 = _mm_set1_epi16(43690);
	bmask = _mm_set1_epi16(0x00ff);
	maska = _mm_setr_epi32(0, 0xffffffff, 0, 0xffffffff);
	maskb = _mm_setr_epi32(0xffffffff, 0, 0xffffffff, 0);
	maskc = _mm_setr_epi32(0, 0, 0xffffffff, 0xffffffff);
	maskd = _mm_setr_epi32(0xffffffff, 0xffffffff, 0, 0);

	for (j = 0; j < dh; ++j) {

		for (i = 0; i < nblock; ++i) {

			/* load and shuffle */
			rr = _mm_loadl_epi64((__m128i *)(ps));
			gg = _mm_loadl_epi64((__m128i *)(ps + 8));
			bb = _mm_loadl_epi64((__m128i *)(ps + 16));
			rr = _mm_unpacklo_epi8(rr, zero);
			gg = _mm_unpacklo_epi8(gg, zero);
			bb = _mm_unpacklo_epi8(bb, zero);

			rl = _mm_unpacklo_epi16(rr, _mm_srli_si128(gg, 8));
			gl = _mm_unpacklo_epi16(_mm_srli_si128(rr, 8), bb);
			bl = _mm_unpacklo_epi16(gg, _mm_srli_si128(bb, 8));

			rh = _mm_unpacklo_epi16(rl, _mm_srli_si128(gl, 8));
			gh = _mm_unpacklo_epi16(_mm_srli_si128(rl, 8), bl);
			bh = _mm_unpacklo_epi16(gl, _mm_srli_si128(bl, 8));

			rr = _mm_unpacklo_epi16(rh, _mm_srli_si128(gh, 8));
			gg = _mm_unpacklo_epi16(_mm_srli_si128(rh, 8), bh);
			bb = _mm_unpacklo_epi16(gh, _mm_srli_si128(bh, 8));

			/* 3-channel units are stored in rr, gg, bb */

			P = _mm_min_epi16(rr, gg);
			V = _mm_max_epi16(rr, gg);
			P = _mm_min_epi16(P, bb);
			V = _mm_max_epi16(V, bb);	/* V */
			L = _mm_add_epi16(V, P);	/* V + P */
			VP = _mm_sub_epi16(V, P);	/* V - P */

			mlib_u16 *ss;
			mask1 = _mm_cmpgt_epi16(L, bmask);
			SS = _mm_and_si128(_mm_sub_epi16(s510, L), mask1);
			mask2 = _mm_andnot_si128(mask1, ones);
			SS = _mm_or_si128(SS, _mm_and_si128(L, mask2));
			ss = (mlib_u16 *)&SS;
			div = _mm_setr_epi16(
				mlib_div1_tab[ss[0]], mlib_div1_tab[ss[1]],
				mlib_div1_tab[ss[2]], mlib_div1_tab[ss[3]],
				mlib_div1_tab[ss[4]], mlib_div1_tab[ss[5]],
				mlib_div1_tab[ss[6]], mlib_div1_tab[ss[7]]);
			rl = _mm_mullo_epi16(VP, div);
			rh = _mm_mulhi_epu16(VP, div);
			gl = _mm_unpacklo_epi16(rl, rh);
			gh = _mm_unpackhi_epi16(rl, rh);
			gl = _mm_add_epi32(gl, shft0);
			gh = _mm_add_epi32(gh, shft0);
			gl = _mm_srli_epi32(gl, 8);
			gh = _mm_srli_epi32(gh, 8);
			S = _mm_packs_epi32(gl, gh); /* S */

			mlib_u16 *vp;
			vp = (mlib_u16 *)&VP;
			div = _mm_setr_epi16(
				mlib_div6_tab[vp[0]], mlib_div6_tab[vp[1]],
				mlib_div6_tab[vp[2]], mlib_div6_tab[vp[3]],
				mlib_div6_tab[vp[4]], mlib_div6_tab[vp[5]],
				mlib_div6_tab[vp[6]], mlib_div6_tab[vp[7]]);

			mask1 = _mm_cmpeq_epi16(rr, V);
			mask2 = _mm_cmpeq_epi16(gg, V);
			mask2 = _mm_and_si128(mask2,
					_mm_andnot_si128(mask1, ones));
			mask3 = _mm_andnot_si128(
					_mm_or_si128(mask1, mask2), ones);

			rl = _mm_sub_epi16(gg, bb);
			gl = _mm_and_si128(mask1, zero);
			gh = _mm_and_si128(mask1, rl);

			rl = _mm_sub_epi16(bb, rr);
			gl = _mm_or_si128(gl, _mm_and_si128(mask2, shft1));
			gh = _mm_or_si128(gh, _mm_and_si128(mask2, rl));

			rl = _mm_sub_epi16(rr, gg);
			gl = _mm_or_si128(gl, _mm_and_si128(mask3, shft2));
			gh = _mm_or_si128(gh, _mm_and_si128(mask3, rl));

			bl = _mm_mullo_epi16(gh, div);
			bh = _mm_mulhi_epi16(gh, div);
			rl = _mm_unpacklo_epi16(bl, bh);
			rh = _mm_unpackhi_epi16(bl, bh);
			rl = _mm_add_epi32(rl, _mm_unpacklo_epi16(gl, zero));
			rh = _mm_add_epi32(rh, _mm_unpackhi_epi16(gl, zero));
			rl = _mm_srai_epi32(rl, 8);
			rh = _mm_srai_epi32(rh, 8);
			H = _mm_packs_epi32(rl, rh);
			H = _mm_and_si128(H, bmask);	/* H */

			__m128i x, y, z, r, g;
			L = _mm_srli_epi16(L, 1);
			x = _mm_or_si128(H, _mm_slli_epi16(S, 8));
			r = _mm_unpacklo_epi16(x, L);
			g = _mm_unpackhi_epi16(x, L);

			x = _mm_and_si128(r, maska);
			x = _mm_srli_si128(x, 1);
			r = _mm_and_si128(r, maskb);
			r = _mm_or_si128(x, r);
			x = _mm_and_si128(r, maskc);
			x = _mm_srli_si128(x, 2);
			r = _mm_and_si128(r, maskd);
			r = _mm_or_si128(x, r);

			y = _mm_and_si128(g, maska);
			y = _mm_srli_si128(y, 1);
			g = _mm_and_si128(g, maskb);
			g = _mm_or_si128(y, g);
			y = _mm_and_si128(g, maskc);
			y = _mm_srli_si128(y, 2);
			g = _mm_and_si128(g, maskd);
			g = _mm_or_si128(y, g);

			x = _mm_slli_si128(g, 12);
			x = _mm_or_si128(x, r);
			_mm_storeu_si128((__m128i *)(pd), x);

			y = _mm_srli_si128(g, 4);
			_mm_storel_epi64((__m128i *)(pd + 16), y);

			ps += 24;
			pd += 24;
		}
		/* handle the tails */
		for (i = nblock * 8; i < dw; ++i) {
			mlib_u8 r = ps[0];
			mlib_u8 g = ps[1];
			mlib_u8 b = ps[2];
			mlib_u32 v, p, s;
			mlib_u32 fh, delta;
			mlib_u32 shft = 43690;
			mlib_s32 dmin = r - g;

			v = p = r;

			if (r > g)
				p = g;
			else
				v = g;
			if (p > b)
				p = b;
			if (b > v)
				v = b;

			if (v + p <= 255)
				s = (v - p) * mlib_div1_tab[v + p];
			else
				s = (v - p) * mlib_div1_tab[510 - v - p];

			if (r == v) {
				shft = 0;
				dmin = g - b;
			} else if (g == v) {
				shft = 21845;
				dmin = b - r;
			}

			delta = mlib_div6_tab[(v - p)];
			fh = shft + dmin * delta;

			pd[2] = (v + p) >> 1;
			pd[1] = s >> 8;
			pd[0] = fh >> 8;

			ps += 3;
			pd += 3;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageColorRGB2HSL_S16(
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
			mlib_s32 v, p, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = p = r;

			if (r > g)
				p = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (p > b)
				p = b;

			if (v == p) {
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

			delta = (v - p);
			sum = v + p + 1;
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

/* *********************************************************** */

void
mlib_s_ImageColorRGB2HSL_U16(
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
			mlib_s32 v, p, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = p = r;

			if (r > g)
				p = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (p > b)
				p = b;

			if (v == p) {
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

			delta = (v - p);
			sum = v + p + 1;
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

/* *********************************************************** */

void
mlib_s_ImageColorRGB2HSL_S32(
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
			mlib_d64 v, p, h, s;
			mlib_d64 delta, vdiv, sum;
			mlib_d64 rdelta;
			mlib_d64 shft = 2.0 / 3.0;
			mlib_d64 dmin = r - g;

			v = p = r;

			if (r > g)
				p = g;

			if (r <= g)
				v = g;

			if (p > b)
				p = b;

			if (b > v)
				v = b;

			if (v == p) {
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

			delta = (v - p);
			rdelta = 1.0 / delta;
			rdelta *= (1.0 / 6.0);

			sum = v + p;

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
