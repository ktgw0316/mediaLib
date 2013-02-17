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

#pragma ident	"@(#)mlib_s_ImageSqrShift_f.c	9.7	07/11/05 SMI"


/*
 * FUNCTIONS
 *
 *      mlib_s_ImageSqrShift_U8  - MLIB_BYTE   type of image
 *      mlib_s_ImageSqrShift_S16 - MLIB_SHORT  type of image
 *      mlib_s_ImageSqrShift_U16 - MLIB_USHORT type of image
 *      mlib_s_ImageSqrShift_S32 - MLIB_INT    type of image
 *
 * ARGUMENT
 *      src     pointer to input image
 *      dst     pointer to output image
 *      shift   right shifting factor
 *
 * DESCRIPTION
 *      dst = (src * src) >> shift
 *      -- low level functions.
 *      These functions are separated from mlib_s_ImageSqrShift.c
 *      for excluding of compiler inlining and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageSqrShift.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SqrShift_U8(sp, dp)	\
\
	P_TYPE sd, s0, s1, a0, a1;	\
\
	sd = _mm_loadu_si128(sp);	\
\
	s0 = _mm_unpacklo_epi8(sd, mzero);	\
	s1 = _mm_unpackhi_epi8(sd, mzero);	\
\
	a0 = _mm_srli_epi16(_mm_mullo_epi16(s0, s0), shift);	\
	a1 = _mm_srli_epi16(_mm_mullo_epi16(s1, s1), shift);	\
\
	_mm_storeu_si128(dp, _mm_packus_epi16(a0, a1));	\
\
	sp ++;	\
	dp ++;

/* *********************************************************** */

#define	SATURATE_U8(ssp, ddp)	\
\
	mlib_s32 temp;	\
	temp = ((mlib_s32)(*ssp)) * (*ssp) >> shift; \
\
	if (temp > MLIB_U8_MAX) {	\
		*ddp = MLIB_U8_MAX;	\
	} else {	\
		*ddp = (mlib_u8)temp;	\
	}	\
\
	ssp ++;	\
	ddp ++;

/* *********************************************************** */

#define	SATURATE_S16(ssp, dpp)	\
\
	mlib_s32 temp;	\
\
	temp = (mlib_s32)(*ssp) * (*ssp) >> shift;	\
\
	if (temp > MLIB_S16_MAX) {	\
		*ddp = MLIB_S16_MAX;	\
	} else {	\
		*ddp = (mlib_s16)temp;	\
	}	\
\
	ssp ++;	\
	ddp ++;

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

#define	SATURATE_U16(spp, dpp)	\
\
	mlib_s32 temp;	\
	temp = (mlib_s64)(*ssp) * (*ssp) >> shift;	\
\
	if (temp > MLIB_U16_MAX) {	\
		*ddp = MLIB_U16_MAX;	\
	} else {	\
		*ddp = (mlib_u16)temp;	\
	}	\
\
	ssp ++;	\
	ddp ++;

#else

#define	SATURATE_U16(spp, dpp)	\
\
	mlib_u32 temp;	\
	temp = (mlib_u32)(*ssp) * (*ssp) >> shift;	\
\
	if (temp > MLIB_U16_MAX) {	\
		*ddp = MLIB_U16_MAX;	\
	} else {	\
		*ddp = (mlib_u16)temp;	\
	}	\
\
	ssp ++;	\
	ddp ++;

#endif /* !defined(_NO_LONGLONG) */

/* *********************************************************** */

void
mlib_s_ImageSqrShift_U8_D1(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 shift)
{

	P_TYPE *sp, *dp;
	P_TYPE mzero;
	mlib_u8 *ssp, *ddp;

	mlib_s32 i, j;
	mlib_s32 off, n;

	mzero = _mm_setzero_si128();

	sp = (P_TYPE *)src;
	dp = (P_TYPE *)dst;

	n = dsize / 16;
	off = dsize - n * 16;

	ssp = (mlib_u8 *)(src + 16 * n);
	ddp = (mlib_u8 *)(dst + 16 * n);


	for (i = 0; i < n; i++) {

		SqrShift_U8(sp, dp);
	}

	for (j = 0; j < off; j++) {

		SATURATE_U8(ssp, ddp);
	}
}

/* *********************************************************** */

void
mlib_s_ImageSqrShift_U8_D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{

	mlib_s32 i, j, k;
	mlib_s32 off, n;

	P_TYPE *sp, *dp;
	P_TYPE mzero;

	mlib_u8 *source, *dest;

	mzero = _mm_setzero_si128();

	n = xsize / 16;
	off = xsize - n * 16;

	source = src;
	dest = dst;

	for (i = 0; i < ysize; i++) {

		mlib_u8 *ssp, *ddp;

		ssp = (mlib_u8 *)(source + 16 * n);
		ddp = (mlib_u8 *)(dest + 16 * n);

		sp = (P_TYPE *)source;
		dp = (P_TYPE *)dest;

		for (k = 0; k < n; k++) {

			SqrShift_U8(sp, dp);
		}

		for (j = 0; j < off; j ++) {

			SATURATE_U8(ssp, ddp);
		}

		source += slb;
		dest += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageSqrShift_S16_D1(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 shift)
{

	P_TYPE *sp, *dp;
	P_TYPE mzero;

	mlib_s32 i, j;
	mlib_s32 off, n;
	mlib_s16 *ddp, *ssp;

	mzero = _mm_setzero_si128();

	sp = (P_TYPE *)src;
	dp = (P_TYPE *)dst;

	n = dsize / 8;
	off = dsize - n * 8;

	ssp = (mlib_s16 *)(src + 8 * n);
	ddp = (mlib_s16 *)(dst + 8 * n);

	for (i = 0; i < n; i++) {

		P_TYPE s0, s1, sd,
		m0_0, m0_1, m1_0, m1_1,
		b0, b1, b2, b3;

		sd = _mm_loadu_si128(sp);

		s0 = _mm_unpacklo_epi16(sd, mzero);
		s1 = _mm_unpackhi_epi16(sd, mzero);

		m0_0 = _mm_mullo_epi16(s0, s0);
		m0_1 = _mm_mulhi_epi16(s0, s0);
		m1_0 = _mm_mullo_epi16(s1, s1);
		m1_1 = _mm_mulhi_epi16(s1, s1);

		b0 = _mm_srli_epi32(_mm_unpacklo_epi16(m0_0, m0_1), shift);
		b1 = _mm_srli_epi32(_mm_unpackhi_epi16(m0_0, m0_1), shift);
		b2 = _mm_srli_epi32(_mm_unpacklo_epi16(m1_0, m1_1), shift);
		b3 = _mm_srli_epi32(_mm_unpackhi_epi16(m1_0, m1_1), shift);

		_mm_storeu_si128(dp, _mm_packs_epi32(
			_mm_packs_epi32(b0, b1),
			_mm_packs_epi32(b2, b3)));

		sp ++;
		dp ++;

	}

	for (j = 0; j < off; j++) {

		SATURATE_S16(ssp, ddp);
	}
}

/* *********************************************************** */

void
mlib_s_ImageSqrShift_S16_D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_s32 i, k, t;
	mlib_s32 off, n;

	P_TYPE *sp, *dp;
	P_TYPE sd;
	P_TYPE mzero;

	mlib_s16 *ssp, *ddp;
	mlib_s16 *source, *dest;

	mzero = _mm_setzero_si128();

	n = xsize / 8;
	off = xsize - n * 8;

	source = src;
	dest = dst;

	for (i = 0; i < ysize; i++) {

		ssp = (mlib_s16 *)(source + 8 * n);
		ddp = (mlib_s16 *)(dest + 8 * n);

		sp = (P_TYPE *)source;
		dp = (P_TYPE *)dest;

		for (k = 0; k < n; k++) {

			P_TYPE s0, s1,
			m0_0, m0_1, m1_0, m1_1,
			b0, b1, b2, b3;

			sd = _mm_loadu_si128(sp);

			s0 = _mm_unpacklo_epi16(sd, mzero);
			s1 = _mm_unpackhi_epi16(sd, mzero);

			m0_0 = _mm_mullo_epi16(s0, s0);
			m0_1 = _mm_mulhi_epi16(s0, s0);
			m1_0 = _mm_mullo_epi16(s1, s1);
			m1_1 = _mm_mulhi_epi16(s1, s1);

			b0 = _mm_srli_epi32(
					_mm_unpacklo_epi16(m0_0, m0_1), shift);
			b1 = _mm_srli_epi32(
					_mm_unpackhi_epi16(m0_0, m0_1), shift);
			b2 = _mm_srli_epi32(
					_mm_unpacklo_epi16(m1_0, m1_1), shift);
			b3 = _mm_srli_epi32(
					_mm_unpackhi_epi16(m1_0, m1_1), shift);

			_mm_storeu_si128(dp, _mm_packs_epi32(
				_mm_packs_epi32(b0, b1),
				_mm_packs_epi32(b2, b3)));

			sp ++;
			dp ++;

		}

		for (t = 0; t < off; t++) {

			SATURATE_S16(ssp, ddp);
		}

		source += slb;
		dest += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageSqrShift_U16_D1(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 dsize,
    mlib_s32 shift)
{

	P_TYPE *sp, *dp;
	P_TYPE sd;
	P_TYPE mzero, saturate_U16;

	mlib_s32 i, j;
	mlib_s32 off, n;

	mlib_u16 *ddp, *ssp;

	saturate_U16 = _mm_set1_epi32(65535);
	mzero = _mm_setzero_si128();

	sp = (P_TYPE *)src;
	dp = (P_TYPE *)dst;

	n = dsize / 8;
	off = dsize - n * 8;

	ssp = (mlib_u16 *)(src + 8 * n);
	ddp = (mlib_u16 *)(dst + 8 * n);

	for (i = 0; i < n; i++) {

		P_TYPE s0, s1,
		b0, b1, b2, b3,
		mask0, mask1, mask2, mask3,
		m0_0, m1_0, m2_0, m3_0,
		m0_1, m1_1, m2_1, m3_1,
		n0, n1, n2, n3,
		c0, c1, c2, c3;

		sd = _mm_loadu_si128(sp);
		s0 = _mm_unpacklo_epi16(sd, mzero);
		s1 = _mm_unpackhi_epi16(sd, mzero);

		m0_0 = _mm_mullo_epi16(s0, s0);
		m0_1 = _mm_mulhi_epu16(s0, s0);
		m1_0 = _mm_mullo_epi16(s1, s1);
		m1_1 = _mm_mulhi_epu16(s1, s1);

		b0 = _mm_srli_epi32(_mm_unpacklo_epi16(m0_0, m0_1), shift);
		b1 = _mm_srli_epi32(_mm_unpackhi_epi16(m0_0, m0_1), shift);
		b2 = _mm_srli_epi32(_mm_unpacklo_epi16(m1_0, m1_1), shift);
		b3 = _mm_srli_epi32(_mm_unpackhi_epi16(m1_0, m1_1), shift);

		mask0 = _mm_cmpeq_epi32(_mm_srli_epi32(b0, 16), mzero);
		mask1 = _mm_cmpeq_epi32(_mm_srli_epi32(b1, 16), mzero);
		mask2 = _mm_cmpeq_epi32(_mm_srli_epi32(b2, 16), mzero);
		mask3 = _mm_cmpeq_epi32(_mm_srli_epi32(b3, 16), mzero);

		n0 = _mm_or_si128(_mm_andnot_si128(mask0, saturate_U16),
				_mm_and_si128(mask0, b0));
		n1 = _mm_or_si128(_mm_andnot_si128(mask1, saturate_U16),
				_mm_and_si128(mask1, b1));
		n2 = _mm_or_si128(_mm_andnot_si128(mask2, saturate_U16),
				_mm_and_si128(mask2, b2));
		n3 = _mm_or_si128(_mm_andnot_si128(mask3, saturate_U16),
				_mm_and_si128(mask3, b3));

		c0 = _mm_unpacklo_epi16(n0, n1);
		c1 = _mm_unpackhi_epi16(n0, n1);
		c2 = _mm_unpacklo_epi16(n2, n3);
		c3 = _mm_unpackhi_epi16(n2, n3);

		_mm_storeu_si128(dp,
			_mm_unpacklo_epi64(_mm_unpacklo_epi16(c0, c1),
			_mm_unpacklo_epi16(c2, c3)));

		sp ++;
		dp ++;

	}

	for (j = 0; j < off; j++) {

		SATURATE_U16(spp, ddp);
	}
}

/* *********************************************************** */

void
mlib_s_ImageSqrShift_U16_D2(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	P_TYPE *sp, *dp;
	P_TYPE sd;
	P_TYPE mzero, saturate_U16;

	mlib_s32 i, j;
	mlib_s32 off, n;

	mlib_u16 *source, *dest;

	saturate_U16 = _mm_set1_epi32(65535);
	mzero = _mm_setzero_si128();

	n = xsize / 8;
	off = xsize - n * 8;

	source = src;
	dest = dst;

	for (i = 0; i < ysize; i++) {

		mlib_u16 *ssp = (mlib_u16 *)(source + 8 * n);
		mlib_u16 *ddp = (mlib_u16 *)(dest + 8 * n);

		sp = (__m128i *)source;
		dp = (__m128i *)dest;

		for (j = 0; j < n; j++) {

			P_TYPE s0, s1,
			b0, b1, b2, b3, d0, d1,
			mask0, mask1, mask2, mask3,
			m0_0, m1_0, m2_0, m3_0,
			m0_1, m1_1, m2_1, m3_1,
			n0, n1, n2, n3,
			c0, c1, c2, c3;

			sd = _mm_loadu_si128(sp);

			s0 = _mm_unpacklo_epi16(sd, mzero);
			s1 = _mm_unpackhi_epi16(sd, mzero);

			m0_0 = _mm_mullo_epi16(s0, s0);
			m0_1 = _mm_mulhi_epu16(s0, s0);
			m1_0 = _mm_mullo_epi16(s1, s1);
			m1_1 = _mm_mulhi_epu16(s1, s1);

			b0 = _mm_srli_epi32(
					_mm_unpacklo_epi16(m0_0, m0_1), shift);
			b1 = _mm_srli_epi32(
					_mm_unpackhi_epi16(m0_0, m0_1), shift);
			b2 = _mm_srli_epi32(
					_mm_unpacklo_epi16(m1_0, m1_1), shift);
			b3 = _mm_srli_epi32(
					_mm_unpackhi_epi16(m1_0, m1_1), shift);

			mask0 = _mm_cmpeq_epi32(_mm_srli_epi32(b0, 16), mzero);
			mask1 = _mm_cmpeq_epi32(_mm_srli_epi32(b1, 16), mzero);
			mask2 = _mm_cmpeq_epi32(_mm_srli_epi32(b2, 16), mzero);
			mask3 = _mm_cmpeq_epi32(_mm_srli_epi32(b3, 16), mzero);

			n0 = _mm_or_si128(_mm_andnot_si128(mask0, saturate_U16),
					_mm_and_si128(mask0, b0));
			n1 = _mm_or_si128(_mm_andnot_si128(mask1, saturate_U16),
					_mm_and_si128(mask1, b1));
			n2 = _mm_or_si128(_mm_andnot_si128(mask2, saturate_U16),
					_mm_and_si128(mask2, b2));
			n3 = _mm_or_si128(_mm_andnot_si128(mask3, saturate_U16),
					_mm_and_si128(mask3, b3));

			c0 = _mm_unpacklo_epi16(n0, n1);
			c1 = _mm_unpackhi_epi16(n0, n1);
			c2 = _mm_unpacklo_epi16(n2, n3);
			c3 = _mm_unpackhi_epi16(n2, n3);

			_mm_storeu_si128(dp,
				_mm_unpacklo_epi64(_mm_unpacklo_epi16(c0, c1),
				_mm_unpacklo_epi16(c2, c3)));

			sp ++;
			dp ++;

		}

		for (int t = 0; t < off; t++) {

			SATURATE_U16(spp, ddp);
		}

		source += slb;
		dest += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageSqrShift_S32(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_d64 dscale;
	mlib_s32 i, j;

	mlib_d64 temp;

/* dscale = 2 ** (-shift) */
	SCALBN(dscale, -shift);

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < xsize; i++) {

			dst[i] =
			    FLOAT2INT_CLAMP(src[i] * (mlib_d64)src[i] * dscale);

		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */
