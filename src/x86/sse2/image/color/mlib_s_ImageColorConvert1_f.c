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

#pragma ident	"@(#)mlib_s_ImageColorConvert1_f.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorConvert - color conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorConvert1(mlib_image *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64 *fmat);
 *      mlib_status mlib_ImageColorConvert2(mlib_image *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64 *fmat,
 *                                          const mlib_d64 *offset);
 *      mlib_status mlib_ImageColorRGB2XYZ(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorXYZ2RGB(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorRGB2YCC(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorYCC2RGB(mlib_image *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      fmat    9 parameters of color matrix in row major order
 *      offset  3 offset values
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert1()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|
 *      mlib_ImageColorConvert2()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|   |offset[0]|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G| + |offset[1]|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|   |offset[2]|
 *      mlib_ImageColorRGB2XYZ()
 *        convert ITU-R Rec.708 RGB with D64 white point into CIE XYZ
 *      mlib_ImageColorXYZ2RGB()
 *        convert CIE XYZ into ITU-R Rec.708 RGB with D64 white point
 *      mlib_ImageColorRGB2YCC()
 *        convert computer R'G'B' into ITU-R Rec.601 Y'CbCr
 *      mlib_ImageColorYCC2RGB()
 *        convert ITU-R Rec.601 Y'CbCr into computer R'G'B'
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	SAT_U8(dst, res)		\
{					\
	if (res > 255)			\
		dst = 255;		\
	else if (res < 0)		\
		dst = 0;		\
	else				\
		dst = res;		\
}
#define	SAT_S16(dst, res)		\
{					\
	if (res > 32767)		\
		dst = 32767;		\
	else if (res < -32768)		\
		dst = -32768;		\
	else				\
		dst = res;		\
}
/* *********************************************************** */

#define	MLIB_RINT_S16(res, i, x, scalef)                        \
	if (x[i] >= 0)                                          \
	    k[i] = (mlib_s16)(x[i] * scalef + 0.5);             \
	else                                                    \
	    k[i] = (mlib_s16)(x[i] * scalef - 0.5);             \
	res##i = _mm_set1_epi16(k[i]);

/* *********************************************************** */

#define	MLIB_RINT1_S16(res, i, x, scalef)                       \
	if (x[i] >= 0)                                          \
	    tmp0 = (mlib_s16)(x[i] * scalef + 0.5);             \
	else                                                    \
	    tmp0 = (mlib_s16)(x[i] * scalef - 0.5);             \
	if (x[i + 1] >= 0)                                      \
	    tmp1 = (mlib_s16)(x[i + 1] * scalef + 0.5);         \
	else                                                    \
	    tmp1 = (mlib_s16)(x[i + 1] * scalef - 0.5);         \
	if (x[i + 2] >= 0)                                      \
	    tmp2 = (mlib_s16)(x[i + 2] * scalef + 0.5);         \
	else                                                    \
	    tmp2 = (mlib_s16)(x[i + 2] * scalef - 0.5);         \
	res##i##0 = tmp0;                                       \
	res##i##1 = tmp1;                                       \
	res##i##2 = tmp2;

/* *********************************************************** */

mlib_status
mlib_s_ImageColorConvert1_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    mlib_s32 scale)
{
	mlib_u8 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_d64 scalef;
	mlib_s32 i, j, shift;
	mlib_s16 k[9];
	__m128i k0, k1, k2, k3, k4, k5, k6, k7, k8;
	__m128i s0, s1, s2, s3, s4, s5, s6, s7;
	__m128i p0, p1, p2, p3;
	__m128i sum0, sum1, sum2;
	__m128i zero = _mm_setzero_si128();

	shift = 6 - scale;
	scalef = 1 << (9 + shift);

	MLIB_RINT_S16(k, 0, fmat, scalef);
	MLIB_RINT_S16(k, 1, fmat, scalef);
	MLIB_RINT_S16(k, 2, fmat, scalef);
	MLIB_RINT_S16(k, 3, fmat, scalef);
	MLIB_RINT_S16(k, 4, fmat, scalef);
	MLIB_RINT_S16(k, 5, fmat, scalef);
	MLIB_RINT_S16(k, 6, fmat, scalef);
	MLIB_RINT_S16(k, 7, fmat, scalef);
	MLIB_RINT_S16(k, 8, fmat, scalef);

	sa = sl = (mlib_u8 *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < (xsize - 8); i += 8) {

#if 0
			s0 = _mm_setr_epi16(
					sl[i*3],
					sl[i*3 + 3],
					sl[i*3 + 6],
					sl[i*3 + 9],
					sl[i*3 + 12],
					sl[i*3 + 15],
					sl[i*3 + 18],
					sl[i*3 + 21]);
			s1 = _mm_setr_epi16(
					sl[i*3 + 1],
					sl[i*3 + 4],
					sl[i*3 + 7],
					sl[i*3 + 10],
					sl[i*3 + 13],
					sl[i*3 + 16],
					sl[i*3 + 19],
					sl[i*3 + 22]);
			s2 = _mm_setr_epi16(
					sl[i*3 + 2],
					sl[i*3 + 5],
					sl[i*3 + 8],
					sl[i*3 + 11],
					sl[i*3 + 14],
					sl[i*3 + 17],
					sl[i*3 + 20],
					sl[i*3 + 23]);

#else

			__m128i d0, d1, d2, d3;

			s0 = _mm_cvtsi32_si128(*(mlib_s32 *)sl);
			s1 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 3));
			s2 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 6));
			s3 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 9));
			s4 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 12));
			s5 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 15));
			s6 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 18));
			s7 = _mm_cvtsi32_si128(*(mlib_s32 *)(sl + 21));

			d0 = _mm_unpacklo_epi8(s0, s4);
			d1 = _mm_unpacklo_epi8(s1, s5);
			d2 = _mm_unpacklo_epi8(s2, s6);
			d3 = _mm_unpacklo_epi8(s3, s7);
			s0 = _mm_unpacklo_epi8(d0, d2);
			s1 = _mm_unpacklo_epi8(d1, d3);
			d0 = _mm_unpacklo_epi8(s0, s1);
			d1 = _mm_unpackhi_epi8(s0, s1);

			s0 = _mm_unpacklo_epi8(d0, zero);
			s1 = _mm_unpackhi_epi8(d0, zero);
			s2 = _mm_unpacklo_epi8(d1, zero);

#endif
			s0 = _mm_slli_epi16(s0, 7);
			s1 = _mm_slli_epi16(s1, 7);
			s2 = _mm_slli_epi16(s2, 7);

			sum0 = _mm_mulhi_epi16(s0, k0);
			sum1 = _mm_mulhi_epi16(s0, k3);
			sum2 = _mm_mulhi_epi16(s0, k6);
			sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(s1, k1));
			sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(s1, k4));
			sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(s1, k7));
			sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(s2, k2));
			sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(s2, k5));
			sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(s2, k8));

			sum0 = _mm_srai_epi16(sum0, shift);
			sum1 = _mm_srai_epi16(sum1, shift);
			sum2 = _mm_srai_epi16(sum2, shift);

			p0 = _mm_packus_epi16(sum0, sum1);
			p1 = _mm_packus_epi16(sum2, zero);

			p2 = _mm_unpacklo_epi8(p0, p1);
			p3 = _mm_unpackhi_epi8(p0, zero);
			p0 = _mm_unpacklo_epi8(p2, p3);
			p1 = _mm_unpackhi_epi8(p2, p3);

			*(mlib_s32 *)(dl) = _mm_cvtsi128_si32(p0);
			p0 = _mm_srli_si128(p0, 4);
			*(mlib_s32 *)(dl + 3) = _mm_cvtsi128_si32(p0);
			p0 = _mm_srli_si128(p0, 4);
			*(mlib_s32 *)(dl + 6) = _mm_cvtsi128_si32(p0);
			p0 = _mm_srli_si128(p0, 4);
			*(mlib_s32 *)(dl + 9) = _mm_cvtsi128_si32(p0);

			*(mlib_s32 *)(dl + 12) = _mm_cvtsi128_si32(p1);
			p1 = _mm_srli_si128(p1, 4);
			*(mlib_s32 *)(dl + 15) = _mm_cvtsi128_si32(p1);
			p1 = _mm_srli_si128(p1, 4);
			*(mlib_s32 *)(dl + 18) = _mm_cvtsi128_si32(p1);
			p1 = _mm_srli_si128(p1, 4);
			*(mlib_s32 *)(dl + 21) = _mm_cvtsi128_si32(p1);

			sl += 24;
			dl += 24;
		}

		for (; i < xsize; i++) {
			int s0, s1, s2, res;
#if 0
			s0 = sa[i*3] << 7;
			s1 = sa[i*3 + 1] << 7;
			s2 = sa[i*3 + 2] << 7;
			res = (((s0 * k[0])>>16) + ((s1 * k[1])>>16) +
				((s2 * k[2])>>16)) >> (shift);
			SAT_U8(da[i*3], res);
			res = (((s0 * k[3])>>16) + ((s1 * k[4])>>16) +
				((s2 * k[5])>>16)) >> (shift);
			SAT_U8(da[i*3+ 1], res);
			res = (((s0 * k[6])>>16) + ((s1 * k[7])>>16) +
				((s2 * k[8])>>16)) >> (shift);
			SAT_U8(da[i*3+ 2], res);

#else

			s0 = sa[i*3];
			s1 = sa[i*3 + 1];
			s2 = sa[i*3 + 2];
			res = (s0 * k[0] + s1 * k[1] + s2 * k[2]) >> (9+shift);
			SAT_U8(da[i*3], res);
			res = (s0 * k[3] + s1 * k[4] + s2 * k[5]) >> (9+shift);
			SAT_U8(da[i*3+ 1], res);
			res = (s0 * k[6] + s1 * k[7] + s2 * k[8]) >> (9+shift);
			SAT_U8(da[i*3+ 2], res);
#endif
		}

		sl = sa = sa + slb;
		dl = da = da + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageColorConvert1_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    mlib_s32 scale)
{
	__m128i s0, s1, s2, s3, s4, s5;
	__m128i s6, s7, s8, s9, s10, s11;
	__m128i p0, p1, p2, p3;
	__m128i w0, w3, w6;
	mlib_s32 i, j, shift;
	mlib_s16 tmp0, tmp1, tmp2;
	mlib_d64 scalef;
	mlib_s32 k00, k01, k02;
	mlib_s32 k30, k31, k32;
	mlib_s32 k60, k61, k62;

	mlib_s16 *sa, *sl;
	mlib_s16 *da, *dl;

	shift = 16 - scale;
	scalef = 1 << shift;
	MLIB_RINT1_S16(k, 0, fmat, scalef);
	MLIB_RINT1_S16(k, 3, fmat, scalef);
	MLIB_RINT1_S16(k, 6, fmat, scalef);

	w0 = _mm_setr_epi16(k00, k01, k30, k31, k02, 0, k32, 0);
	w3 = _mm_setr_epi16(k60, k61, k00, k01, k62, 0, k02, 0);
	w6 = _mm_setr_epi16(k30, k31, k60, k61, k32, 0, k62, 0);

	sa = sl = (mlib_s16 *)src;
	da = dl = dst;
	for (j = 0; j < ysize; j++) {

		for (i = 0; i < (xsize - 8); i += 8) {
			__m128i x0, x1, x2, x3, x4, x5, x6, x7;
			__m128i t0, t1, t2, t3;

			x0 = _mm_loadl_epi64((__m128i *)sa);
			x1 = _mm_loadl_epi64((__m128i *)(sa + 3));
			x2 = _mm_loadl_epi64((__m128i *)(sa + 6));
			x3 = _mm_loadl_epi64((__m128i *)(sa + 9));
			x4 = _mm_loadl_epi64((__m128i *)(sa + 12));
			x5 = _mm_loadl_epi64((__m128i *)(sa + 15));
			x6 = _mm_loadl_epi64((__m128i *)(sa + 18));
			x7 = _mm_loadl_epi64((__m128i *)(sa + 21));

			s0 = _mm_unpacklo_epi32(x0, x0);
			s1 = _mm_unpacklo_epi32(x0, x1);
			s2 = _mm_unpacklo_epi32(x1, x1);
			s3 = _mm_unpacklo_epi32(x2, x2);
			s4 = _mm_unpacklo_epi32(x2, x3);
			s5 = _mm_unpacklo_epi32(x3, x3);
			s6 = _mm_unpacklo_epi32(x4, x4);
			s7 = _mm_unpacklo_epi32(x4, x5);
			s8 = _mm_unpacklo_epi32(x5, x5);
			s9 = _mm_unpacklo_epi32(x6, x6);
			s10 = _mm_unpacklo_epi32(x6, x7);
			s11 = _mm_unpacklo_epi32(x7, x7);

			t0 = _mm_madd_epi16(s0, w0);
			t1 = _mm_madd_epi16(s1, w3);
			t2 = _mm_madd_epi16(s2, w6);
			t3 = _mm_madd_epi16(s3, w0);

			p0 = _mm_unpacklo_epi64(t0, t1);
			p1 = _mm_unpackhi_epi64(t0, t1);
			p2 = _mm_unpacklo_epi64(t2, t3);
			p3 = _mm_unpackhi_epi64(t2, t3);

			t0 = _mm_add_epi32(p0, p1);
			t1 = _mm_add_epi32(p2, p3);

			p0 = _mm_srai_epi32(t0, shift);
			p1 = _mm_srai_epi32(t1, shift);

			s0 = _mm_packs_epi32(p0, p1);

			_mm_storeu_si128((__m128i *)da, s0);


			t0 = _mm_madd_epi16(s4, w3);
			t1 = _mm_madd_epi16(s5, w6);
			t2 = _mm_madd_epi16(s6, w0);
			t3 = _mm_madd_epi16(s7, w3);

			p0 = _mm_unpacklo_epi64(t0, t1);
			p1 = _mm_unpackhi_epi64(t0, t1);
			p2 = _mm_unpacklo_epi64(t2, t3);
			p3 = _mm_unpackhi_epi64(t2, t3);

			t0 = _mm_add_epi32(p0, p1);
			t1 = _mm_add_epi32(p2, p3);

			p0 = _mm_srai_epi32(t0, shift);
			p1 = _mm_srai_epi32(t1, shift);

			s0 = _mm_packs_epi32(p0, p1);

			_mm_storeu_si128((__m128i *)(da + 8), s0);


			t0 = _mm_madd_epi16(s8, w6);
			t1 = _mm_madd_epi16(s9, w0);
			t2 = _mm_madd_epi16(s10, w3);
			t3 = _mm_madd_epi16(s11, w6);

			p0 = _mm_unpacklo_epi64(t0, t1);
			p1 = _mm_unpackhi_epi64(t0, t1);
			p2 = _mm_unpacklo_epi64(t2, t3);
			p3 = _mm_unpackhi_epi64(t2, t3);

			t0 = _mm_add_epi32(p0, p1);
			t1 = _mm_add_epi32(p2, p3);

			p0 = _mm_srai_epi32(t0, shift);
			p1 = _mm_srai_epi32(t1, shift);

			s0 = _mm_packs_epi32(p0, p1);

			_mm_storeu_si128((__m128i *)(da + 16), s0);


			sa += 24;
			da += 24;
		}

		for (; i < xsize; i++) {
			mlib_s32 s0, s1, s2, res;
			s0 = sa[0];
			s1 = sa[1];
			s2 = sa[2];
			res = (s0 * k00 + s1 * k01 + s2 * k02) >> shift;
			SAT_S16(da[0], res);
			res = (s0 * k30 + s1 * k31 + s2 * k32) >> shift;
			SAT_S16(da[1], res);
			res = (s0 * k60 + s1 * k61 + s2 * k62) >> shift;
			SAT_S16(da[2], res);

			sa += 3;
			da += 3;
		}

		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
