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

#pragma ident	"@(#)mlib_s_ImageZoomBlend_NN.c	9.8	07/11/05 SMI"

/*
 * Local functions for
 *	mlib_ImageZoomBlend()
 *	mlib_ImageZoomTranslateBlend()
 */

#include <mlib_image.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

typedef union
{
	__m128 m128;
	__m128d m128d;
	__m128i m128i;
} mlib_m128;

void mlib_s_ImageChannelExtract_U8_43R_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageChannelExtract_U8_43L_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

/* *********************************************************** */

#define	VARIABLE(FORMAT, FL)                                              \
	mlib_s32 j, dx = GetElemStruct(DX), dy = GetElemStruct(DY), x =   \
	GetElemSubStruct(current, srcX), y =                              \
	GetElemSubStruct(current, srcY), src_stride =                     \
	GetElemStruct(src_stride), dst_stride =                           \
	GetElemStruct(dst_stride), width =                                \
	GetElemSubStruct(current, width), height =                        \
	GetElemSubStruct(current, height);                                \
	ALP_IND_##FL

#define	ALP_IND_YES	mlib_u8 *tsp; mlib_s32 alp_ind = param -> alp_ind

#define	ALP_IND_NO	mlib_u8  *tsp

/* *********************************************************** */

#define	FP_TYPE	mlib_d64
#define	F_ONE	((FP_TYPE)1)
#define	I2D(x)	mlib_U82D64[x]

/* *********************************************************** */

#define	BLEND(dst, src)

/* *********************************************************** */

#define	BLEND_A0(dst, src)	dst = (mlib_s32)(alp*I2D(src))

/* *********************************************************** */

#define	BLEND2(dst, src)                                        \
	dst = (mlib_s32)(dst + (I2D(src) - I2D(dst)) * _rw)

#define	BLEND2z(dst, src)                                           \
	dst = (mlib_s32)((_w0 * I2D(src) + _w1 * I2D(dst)) * _rw)

/* *********************************************************** */

typedef union
{
	struct
	{
		unsigned int i[4];
	} int4;
	__m128i m128i;
} mlib_m128i_union;

mlib_m128i_union mlib_emask_m128i[] = {
	{0, 0, 0, 0},
	{0xFF, 0, 0, 0},
	{0xFFFF, 0, 0, 0},
	{0xFFFFFF, 0, 0, 0},
	{0xFFFFFFFF, 0, 0, 0},
	{0xFFFFFFFF, 0xFF, 0, 0},
	{0xFFFFFFFF, 0xFFFF, 0, 0},
	{0xFFFFFFFF, 0xFFFFFF, 0, 0},
	{0xFFFFFFFF, 0xFFFFFFFF, 0, 0},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFF, 0},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF, 0},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFF, 0},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFF},
	{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
};

/* *********************************************************** */

#define	BUFF_SIZE	1024

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_33_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, NO);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2, ialp;
	mlib_s32 cx, y0 = -1;
	FP_TYPE alp = (param->alpha) * (F_ONE / 255);
	mlib_u8 *pz;
	__m128i buff[BUFF_SIZE / 16], *pbuff = buff;
	__m128i mzero, emask;
	__m128i dalp, ralp, ss, dd, s0, s1, d0, d1, dr;

	if (3 * width > BUFF_SIZE) {
		pbuff = (__m128i *)__mlib_malloc(3 * width);
		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	ialp = alp * (1 << 15);
	dalp = _mm_set1_epi16(ialp);
	ralp = _mm_set1_epi16((1 << 15) - ialp);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	mzero = _mm_setzero_si128();
	emask = mlib_emask_m128i[(3 * width) & 15].m128i;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		pz = (mlib_u8 *)pbuff;
		/* to avoid buffer overrun, load the last pixel separately */
		for (i = 0; i < width - 1; i++) {
			cx = (x >> MLIB_SHIFT) * 3;
			x += dx;

			*(mlib_s32 *)pz = *(mlib_s32 *)(tsp + cx);
			pz += 3;
		}
		cx = (x >> MLIB_SHIFT) * 3;
		*pz++ = *(tsp + cx);
		*pz++ = *(tsp + cx + 1);
		*pz   = *(tsp + cx + 2);

		pz = (void *)pbuff;
		tdp = dp;
		for (i = 0; i <= 3 * width - 16; i += 16) {
			ss = _mm_load_si128((__m128i *) (pz + i));
			dd = _mm_loadu_si128((__m128i *) (tdp + i));
			s0 = _mm_unpacklo_epi8(mzero, ss);
			s1 = _mm_unpackhi_epi8(mzero, ss);
			d0 = _mm_unpacklo_epi8(mzero, dd);
			d1 = _mm_unpackhi_epi8(mzero, dd);
			d0 = _mm_add_epi16(_mm_mulhi_epu16(s0, dalp),
			    _mm_mulhi_epu16(d0, ralp));
			d1 = _mm_add_epi16(_mm_mulhi_epu16(s1, dalp),
			    _mm_mulhi_epu16(d1, ralp));
			d0 = _mm_srli_epi16(d0, 7);
			d1 = _mm_srli_epi16(d1, 7);
			dr = _mm_packus_epi16(d0, d1);
			_mm_storeu_si128((__m128i *) (tdp + i), dr);
		}

		if (i < 3 * width) {
			ss = _mm_load_si128((__m128i *) (pz + i));
			dd = _mm_loadu_si128((__m128i *) (tdp + i));
			s0 = _mm_unpacklo_epi8(mzero, ss);
			s1 = _mm_unpackhi_epi8(mzero, ss);
			d0 = _mm_unpacklo_epi8(mzero, dd);
			d1 = _mm_unpackhi_epi8(mzero, dd);
			d0 = _mm_add_epi16(_mm_mulhi_epu16(s0, dalp),
			    _mm_mulhi_epu16(d0, ralp));
			d1 = _mm_add_epi16(_mm_mulhi_epu16(s1, dalp),
			    _mm_mulhi_epu16(d1, ralp));
			d0 = _mm_srli_epi16(d0, 7);
			d1 = _mm_srli_epi16(d1, 7);
			dr = _mm_packus_epi16(d0, d1);

			dr = _mm_or_si128(_mm_and_si128(emask, dr),
			    _mm_andnot_si128(emask, dd));

			_mm_storeu_si128((__m128i *) (tdp + i), dr);
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	if (pbuff != buff) {
		__mlib_free(pbuff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BLEND43_SRC(bmask)                                                 \
	s0u.m128 = _mm_load_ss((mlib_f32 *)tsp + (x >> MLIB_SHIFT));       \
	s1u.m128 = _mm_load_ss((mlib_f32 *)tsp + ((x + dx) >> MLIB_SHIFT));\
	s2u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)tsp + ((x + 2 * dx) >> MLIB_SHIFT));       \
	s3u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)tsp + ((x + 3 * dx) >> MLIB_SHIFT));       \
	x += 4 * dx;                                                       \
	s0 = _mm_unpacklo_epi32(s0u.m128i, s1u.m128i);                     \
	s1 = _mm_unpacklo_epi32(s2u.m128i, s3u.m128i);                     \
	s0 = _mm_unpacklo_epi8(mzero, s0);                                 \
	a0 = _mm_shufflelo_epi16(s0, bmask);                               \
	a0 = _mm_shufflehi_epi16(a0, bmask);                               \
	a0 = _mm_mulhi_epu16(a0, dalp);                                    \
	d0 = _mm_mulhi_epu16(a0, s0);                                      \
	s1 = _mm_unpacklo_epi8(mzero, s1);                                 \
	a1 = _mm_shufflelo_epi16(s1, bmask);                               \
	a1 = _mm_shufflehi_epi16(a1, bmask);                               \
	a1 = _mm_mulhi_epu16(a1, dalp);                                    \
	d1 = _mm_mulhi_epu16(a1, s1);                                      \
	d0 = _mm_srli_epi16(d0, 6);                                        \
	d1 = _mm_srli_epi16(d1, 6);                                        \
	pbuff[i] = _mm_packus_epi16(d0, d1)

/* *********************************************************** */

#define	BLEND43_SRC_OVER(bmask)                                            \
	s0u.m128 = _mm_load_ss((mlib_f32 *)tsp + (x >> MLIB_SHIFT));       \
	s1u.m128 = _mm_load_ss((mlib_f32 *)tsp + ((x + dx) >> MLIB_SHIFT));\
	s2u.m128 =                                                         \
	    _mm_load_ss((mlib_f32 *)tsp + ((x + 2 * dx) >> MLIB_SHIFT));   \
	s3u.m128 =                                                         \
	    _mm_load_ss((mlib_f32 *)tsp + ((x + 3 * dx) >> MLIB_SHIFT));   \
	x += 4 * dx;                                                       \
	dd = _mm_load_si128(pbuff + i);                                    \
	d0 = _mm_unpacklo_epi8(mzero, dd);                                 \
	d1 = _mm_unpackhi_epi8(mzero, dd);                                 \
	s0 = _mm_unpacklo_epi32(s0u.m128i, s1u.m128i);                     \
	s1 = _mm_unpacklo_epi32(s2u.m128i, s3u.m128i);                     \
	s0 = _mm_unpacklo_epi8(mzero, s0);                                 \
	a0 = _mm_shufflelo_epi16(s0, bmask);                               \
	a0 = _mm_shufflehi_epi16(a0, bmask);                               \
	a0 = _mm_mulhi_epu16(a0, dalp);                                    \
	r0 = _mm_sub_epi16(done, a0);                                      \
	d0 = _mm_adds_epu16(_mm_mulhi_epu16(a0, s0),                       \
			    _mm_mulhi_epu16(r0, d0));                      \
	d0 = _mm_adds_epu16(d0, d_rnd);                                    \
	s1 = _mm_unpacklo_epi8(mzero, s1);                                 \
	a1 = _mm_shufflelo_epi16(s1, bmask);                               \
	a1 = _mm_shufflehi_epi16(a1, bmask);                               \
	a1 = _mm_mulhi_epu16(a1, dalp);                                    \
	r1 = _mm_sub_epi16(done, a1);                                      \
	d1 = _mm_adds_epu16(_mm_mulhi_epu16(a1, s1),                       \
			    _mm_mulhi_epu16(r1, d1));                      \
	d1 = _mm_adds_epu16(d1, d_rnd);                                    \
	d0 = _mm_srli_epi16(d0, 6);                                        \
	d1 = _mm_srli_epi16(d1, 6);                                        \
	pbuff[i] = _mm_packus_epi16(d0, d1)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_43_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;
	mlib_s32 alp = param->alpha;
	__m128i buff[BUFF_SIZE / 16], *pbuff = buff;
	__m128i mzero, done;
	__m128i dalp, ralp, ss, dd, s0, s1, d0, d1, a0, a1, r0, r1, dr;
	__m128i d_rnd = _mm_set1_epi16(0x20);
	mlib_m128 s0u, s1u, s2u, s3u;
	mlib_s32 alp_bmask, width4;

	width4 = (width + 3) / 4;

	if (16 * width > BUFF_SIZE) {
		pbuff = __mlib_malloc(16 * width);
		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	if (alp_ind == -1) {
		sp--;
		alp_bmask = 0;
	} else {
		alp_bmask = 0xff;
	}

	if (alp < 0) {
		dalp = _mm_set1_epi16((1 << 14) * 256 / 255);
	} else {
		dalp = _mm_set1_epi16((1 << 14) * 256 * alp / (255 * 255));
	}

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	mzero = _mm_setzero_si128();
	done = _mm_set1_epi16(1 << 14);

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (alp < 0) {
			if (alp_ind == -1) {
				for (i = 0; i < width4; i++) {
					BLEND43_SRC(0);
				}
			} else {
				for (i = 0; i < width4; i++) {
					BLEND43_SRC(0xff);
				}
			}
		} else {

			if (alp_ind == -1) {
				tdp--;
			}

			for (i = 0; i < width; i++) {
				((mlib_s32 *)pbuff)[i] =
				    *(mlib_s32 *)(tdp + 3 * i);
			}

			if (alp_ind == -1) {
				for (i = 0; i < width4; i++) {
					BLEND43_SRC_OVER(0);
				}
			} else {
				for (i = 0; i < width4; i++) {
					BLEND43_SRC_OVER(0xff);
				}
			}
		}

		if (alp_ind == -1) {
			mlib_s_ImageChannelExtract_U8_43R_D1((void *)pbuff, dp,
			    width);
		} else {
			mlib_s_ImageChannelExtract_U8_43L_D1((void *)pbuff, dp,
			    width);
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	if (pbuff != buff) {
		__mlib_free(pbuff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	GET_ALPHA_CHANNEL_0(dd)	_mm_and_si128(dd, amask)

#define	GET_ALPHA_CHANNEL_3(dd)	_mm_srli_epi32(dd, 24)

#define	PUT_ALPHA_CHANNEL_0(aa)	aa

#define	PUT_ALPHA_CHANNEL_3(aa)	_mm_slli_epi32(aa, 24)

/* *********************************************************** */

#define	BLEND34_SRC_OVER2(alpind)                                          \
	s0u.m128 = _mm_load_ss((mlib_f32 *)(tsp + 3 * (x >> MLIB_SHIFT))); \
	s1u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 3 * ((x + dx) >> MLIB_SHIFT)));     \
	s2u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 3 * ((x + 2 * dx) >> MLIB_SHIFT))); \
	s3u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 3 * ((x + 3 * dx) >> MLIB_SHIFT))); \
	x += 4 * dx;                                                       \
	dd = _mm_loadu_si128((__m128i *) tdp);                             \
	aa = GET_ALPHA_CHANNEL_##alpind(dd);                               \
	af = _mm_cvtepi32_ps(aa);                                          \
	w1 = _mm_mul_ps(w1s, af);                                          \
	w = _mm_add_ps(w0, w1);                                            \
	rw = _mm_div_ps(w0, w);                                            \
	rw = _mm_mul_ps(rw, scale);                                        \
	aa = _mm_cvtps_epi32(rw);                                          \
	wi = PUT_ALPHA_CHANNEL_##alpind(_mm_cvtps_epi32(w));               \
	a0 = _mm_unpacklo_epi16(aa, mzero);                                \
	a1 = _mm_unpackhi_epi16(aa, mzero);                                \
	d0 = _mm_unpacklo_epi8(mzero, dd);                                 \
	d1 = _mm_unpackhi_epi8(mzero, dd);                                 \
	s0 = _mm_unpacklo_epi32(s0u.m128i, s1u.m128i);                     \
	s1 = _mm_unpacklo_epi32(s2u.m128i, s3u.m128i);                     \
	s0 = _mm_unpacklo_epi8(mzero, s0);                                 \
	a0 = _mm_shufflelo_epi16(a0, 0);                                   \
	a0 = _mm_shufflehi_epi16(a0, 0);                                   \
	r0 = _mm_sub_epi16(done, a0);                                      \
	d0 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a0, s0), _mm_mulhi_epu16(r0, d0));   \
	s1 = _mm_unpacklo_epi8(mzero, s1);                                 \
	a1 = _mm_shufflelo_epi16(a1, 0);                                   \
	a1 = _mm_shufflehi_epi16(a1, 0);                                   \
	r1 = _mm_sub_epi16(done, a1);                                      \
	d1 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a1, s1), _mm_mulhi_epu16(r1, d1));   \
	d0 = _mm_srli_epi16(d0, 7);                                        \
	d1 = _mm_srli_epi16(d1, 7);                                        \
	dr = _mm_packus_epi16(d0, d1);                                     \
	dr = _mm_or_si128(wi, _mm_andnot_si128(amask, dr));

/* *********************************************************** */

#define	BLEND34_SRC_OVER(alpind)                                           \
	s0u.m128 = _mm_load_ss((mlib_f32 *)(tsp + 3 * (x >> MLIB_SHIFT))); \
	s1u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 3 * ((x + dx) >> MLIB_SHIFT)));     \
	s2u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 3 * ((x + 2 * dx) >> MLIB_SHIFT))); \
	s3u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 3 * ((x + 3 * dx) >> MLIB_SHIFT))); \
	x += 4 * dx;                                                       \
	dd = _mm_loadu_si128((__m128i *) tdp);                             \
	aa = GET_ALPHA_CHANNEL_##alpind(dd);                               \
	af = _mm_cvtepi32_ps(aa);                                          \
	w1 = _mm_mul_ps(w1s, af);                                          \
	w = _mm_add_ps(w0, w1);                                            \
	rw = _mm_div_ps(scale, w);                                         \
	w0r = _mm_mul_ps(w0, rw);                                          \
	w1r = _mm_mul_ps(w1, rw);                                          \
	aa = _mm_cvtps_epi32(w0r);                                         \
	rr = _mm_cvtps_epi32(w1r);                                         \
	wi = PUT_ALPHA_CHANNEL_##alpind(_mm_cvtps_epi32(w));               \
	a0 = _mm_unpacklo_epi16(aa, mzero);                                \
	a1 = _mm_unpackhi_epi16(aa, mzero);                                \
	r0 = _mm_unpacklo_epi16(rr, mzero);                                \
	r1 = _mm_unpackhi_epi16(rr, mzero);                                \
	d0 = _mm_unpacklo_epi8(mzero, dd);                                 \
	d1 = _mm_unpackhi_epi8(mzero, dd);                                 \
	s0 = _mm_unpacklo_epi32(s0u.m128i, s1u.m128i);                     \
	s1 = _mm_unpacklo_epi32(s2u.m128i, s3u.m128i);                     \
	s0 = _mm_unpacklo_epi8(mzero, s0);                                 \
	a0 = _mm_shufflelo_epi16(a0, 0);                                   \
	a0 = _mm_shufflehi_epi16(a0, 0);                                   \
	r0 = _mm_shufflelo_epi16(r0, 0);                                   \
	r0 = _mm_shufflehi_epi16(r0, 0);                                   \
	d0 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a0, s0), _mm_mulhi_epu16(r0, d0));   \
	s1 = _mm_unpacklo_epi8(mzero, s1);                                 \
	a1 = _mm_shufflelo_epi16(a1, 0);                                   \
	a1 = _mm_shufflehi_epi16(a1, 0);                                   \
	r1 = _mm_shufflelo_epi16(r1, 0);                                   \
	r1 = _mm_shufflehi_epi16(r1, 0);                                   \
	d1 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a1, s1), _mm_mulhi_epu16(r1, d1));   \
	d0 = _mm_srli_epi16(d0, 7);                                        \
	d1 = _mm_srli_epi16(d1, 7);                                        \
	dr = _mm_packus_epi16(d0, d1);                                     \
	dr = _mm_or_si128(wi, _mm_andnot_si128(amask, dr));

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_34_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 cx, y0 = -1;
	mlib_s32 mask255;
	mlib_blend blend = param->blend;
	FP_TYPE _w0 = param->alpha;
	FP_TYPE _w1s = F_ONE - _w0 * (F_ONE / 255);
	__m128i buff[1];
	__m128i mzero, done;
	__m128i dalp, ralp, ss, dd, s0, s1, d0, d1, a0, a1, r0, r1, rr, dr;
	__m128i wi, aa, amask;
	__m128 af, w0, w1, w1s, w, rw, w0r, w1r, scale;
	mlib_m128 s0u, s1u, s2u, s3u;

	if (alp_ind == -1) {
		sp--;
		dp--;
		mask255 = 0xFF;
	} else {
		mask255 = 0xFF000000;
	}

	mzero = _mm_setzero_si128();
	done = _mm_set1_epi16(1 << 15);
	amask = _mm_set1_epi32(mask255);

	w0 = _mm_set_ps1(_w0);
	w1s = _mm_set_ps1(_w1s);
	scale = _mm_set_ps1(1 << 15);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend == MLIB_BLEND_GTK_SRC) {
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				*(mlib_s32 *)tdp =
				    *(mlib_s32 *)(tsp + cx) | mask255;
				tdp += 4;
			}
		} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
			if (alp_ind == -1) {
				for (i = 0; i < width / 4; i++) {
					BLEND34_SRC_OVER2(0);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND34_SRC_OVER2(0);
					buff[0] = dr;
				}
			} else {
				for (i = 0; i < width / 4; i++) {
					BLEND34_SRC_OVER2(3);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND34_SRC_OVER2(3);
					buff[0] = dr;
				}
			}
		} else {
			if (alp_ind == -1) {
				for (i = 0; i < width / 4; i++) {
					BLEND34_SRC_OVER(0);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND34_SRC_OVER(0);
					buff[0] = dr;
				}
			} else {
				for (i = 0; i < width / 4; i++) {
					BLEND34_SRC_OVER(3);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND34_SRC_OVER(3);
					buff[0] = dr;
				}
			}
		}

		if (blend != MLIB_BLEND_GTK_SRC) {
			for (i = 0; i < (width & 3); i++) {
				((mlib_s32 *)tdp)[i] = ((mlib_s32 *)buff)[i];
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BLEND44_SRC_OVER2(alpind)                                          \
	s0u.m128 = _mm_load_ss((mlib_f32 *)(tsp + 4 * (x >> MLIB_SHIFT))); \
	s1u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 4 * ((x + dx) >> MLIB_SHIFT)));     \
	s2u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 4 * ((x + 2 * dx) >> MLIB_SHIFT))); \
	s3u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 4 * ((x + 3 * dx) >> MLIB_SHIFT))); \
	x += 4 * dx;                                                       \
	s0 = _mm_unpacklo_epi32(s0u.m128i, s1u.m128i);                     \
	s1 = _mm_unpacklo_epi32(s2u.m128i, s3u.m128i);                     \
	ss = _mm_unpacklo_epi64(s0, s1);                                   \
	ss = GET_ALPHA_CHANNEL_##alpind(ss);                               \
	dd = _mm_loadu_si128((__m128i *) tdp);                             \
	aa = GET_ALPHA_CHANNEL_##alpind(dd);                               \
	af = _mm_cvtepi32_ps(aa);                                          \
	sf = _mm_cvtepi32_ps(ss);                                          \
	w0 = _mm_mul_ps(dalp, sf);                                         \
	w1 = _mm_mul_ps(w0, div255);                                       \
	w1 = _mm_sub_ps(fone, w1);                                         \
	w1 = _mm_mul_ps(w1, af);                                           \
	w = _mm_add_ps(w0, w1);                                            \
	rw = _mm_div_ps(w0, w);                                            \
	rw = _mm_and_ps(rw, _mm_cmpge_ps(w, fone));                        \
	rw = _mm_mul_ps(rw, scale);                                        \
	aa = _mm_cvttps_epi32(rw);                                         \
	wi = PUT_ALPHA_CHANNEL_##alpind(_mm_cvttps_epi32(w));              \
	a0 = _mm_unpacklo_epi16(aa, mzero);                                \
	a1 = _mm_unpackhi_epi16(aa, mzero);                                \
	d0 = _mm_unpacklo_epi8(mzero, dd);                                 \
	d1 = _mm_unpackhi_epi8(mzero, dd);                                 \
	s0 = _mm_unpacklo_epi8(mzero, s0);                                 \
	a0 = _mm_shufflelo_epi16(a0, 0);                                   \
	a0 = _mm_shufflehi_epi16(a0, 0);                                   \
	r0 = _mm_sub_epi16(done, a0);                                      \
	d0 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a0, s0), _mm_mulhi_epu16(r0, d0));   \
	s1 = _mm_unpacklo_epi8(mzero, s1);                                 \
	a1 = _mm_shufflelo_epi16(a1, 0);                                   \
	a1 = _mm_shufflehi_epi16(a1, 0);                                   \
	r1 = _mm_sub_epi16(done, a1);                                      \
	d1 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a1, s1), _mm_mulhi_epu16(r1, d1));   \
	d0 = _mm_srli_epi16(d0, 6);                                        \
	d1 = _mm_srli_epi16(d1, 6);                                        \
	dr = _mm_packus_epi16(d0, d1);                                     \
	dr = _mm_or_si128(wi, _mm_andnot_si128(amask, dr));

/* *********************************************************** */

#define	BLEND44_SRC_OVER(alpind)                                           \
	s0u.m128 = _mm_load_ss((mlib_f32 *)(tsp + 4 * (x >> MLIB_SHIFT))); \
	s1u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 4 * ((x + dx) >> MLIB_SHIFT)));     \
	s2u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 4 * ((x + 2 * dx) >> MLIB_SHIFT))); \
	s3u.m128 =                                                         \
	_mm_load_ss((mlib_f32 *)(tsp + 4 * ((x + 3 * dx) >> MLIB_SHIFT))); \
	x += 4 * dx;                                                       \
	s0 = _mm_unpacklo_epi32(s0u.m128i, s1u.m128i);                     \
	s1 = _mm_unpacklo_epi32(s2u.m128i, s3u.m128i);                     \
	ss = _mm_unpacklo_epi64(s0, s1);                                   \
	ss = GET_ALPHA_CHANNEL_##alpind(ss);                               \
	dd = _mm_loadu_si128((__m128i *) tdp);                             \
	aa = GET_ALPHA_CHANNEL_##alpind(dd);                               \
	af = _mm_cvtepi32_ps(aa);                                          \
	sf = _mm_cvtepi32_ps(ss);                                          \
	w0 = _mm_mul_ps(dalp, sf);                                         \
	w1 = _mm_mul_ps(w0, div255);                                       \
	w1 = _mm_sub_ps(fone, w1);                                         \
	w1 = _mm_mul_ps(w1, af);                                           \
	w = _mm_add_ps(w0, w1);                                            \
	rw = _mm_div_ps(w0, w);                                            \
	mask = _mm_cmpge_ps(w, fone);                                      \
	rw = _mm_mul_ps(rw, scale);                                        \
	aa = _mm_cvttps_epi32(_mm_and_ps(rw, mask));                       \
	rr = _mm_cvttps_epi32(_mm_and_ps(_mm_sub_ps(scale, rw), mask));    \
	wi = PUT_ALPHA_CHANNEL_##alpind(_mm_cvttps_epi32(w));              \
	a0 = _mm_unpacklo_epi16(aa, mzero);                                \
	a1 = _mm_unpackhi_epi16(aa, mzero);                                \
	r0 = _mm_unpacklo_epi16(rr, mzero);                                \
	r1 = _mm_unpackhi_epi16(rr, mzero);                                \
	d0 = _mm_unpacklo_epi8(mzero, dd);                                 \
	d1 = _mm_unpackhi_epi8(mzero, dd);                                 \
	s0 = _mm_unpacklo_epi8(mzero, s0);                                 \
	a0 = _mm_shufflelo_epi16(a0, 0);                                   \
	a0 = _mm_shufflehi_epi16(a0, 0);                                   \
	r0 = _mm_shufflelo_epi16(r0, 0);                                   \
	r0 = _mm_shufflehi_epi16(r0, 0);                                   \
	d0 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a0, s0), _mm_mulhi_epu16(r0, d0));   \
	s1 = _mm_unpacklo_epi8(mzero, s1);                                 \
	a1 = _mm_shufflelo_epi16(a1, 0);                                   \
	a1 = _mm_shufflehi_epi16(a1, 0);                                   \
	r1 = _mm_shufflelo_epi16(r1, 0);                                   \
	r1 = _mm_shufflehi_epi16(r1, 0);                                   \
	d1 =                                                               \
	_mm_add_epi16(_mm_mulhi_epu16(a1, s1), _mm_mulhi_epu16(r1, d1));   \
	d0 = _mm_srli_epi16(d0, 6);                                        \
	d1 = _mm_srli_epi16(d1, 6);                                        \
	dr = _mm_packus_epi16(d0, d1);                                     \
	dr = _mm_or_si128(wi, _mm_andnot_si128(amask, dr));

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_44_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 cx, y0 = -1;
	mlib_blend blend = param->blend;
	FP_TYPE alp = (param->alpha) * (F_ONE / 255);
	FP_TYPE _w0, _w1, _w, _rw;
	mlib_s32 mask255;
	__m128i buff[1];
	__m128i mzero, done;
	__m128i ss, dd, s0, s1, d0, d1, a0, a1, r0, r1, rr, dr;
	__m128i wi, aa, amask;
	__m128 dalp, div255, fone, af, sf, w0, w1, w1s, w, rw, w0r, w1r, scale,
	    mask;
	mlib_m128 s0u, s1u, s2u, s3u;

	if (alp_ind == -1) {
		sp--;
		dp--;
		mask255 = 0xFF;
	} else {
		mask255 = 0xFF000000;
	}

	mzero = _mm_setzero_si128();
	done = _mm_set1_epi16(1 << 14);
	amask = _mm_set1_epi32(mask255);

	dalp = _mm_set_ps1(alp);
	fone = _mm_set_ps1(1.0);
	div255 = _mm_set_ps1(1.0 / 255);
	scale = _mm_set_ps1(1 << 14);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
			if (alp_ind == -1) {
				for (i = 0; i < width / 4; i++) {
					BLEND44_SRC_OVER2(0);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND44_SRC_OVER2(0);
					buff[0] = dr;
				}
			} else {
				for (i = 0; i < width / 4; i++) {
					BLEND44_SRC_OVER2(3);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND44_SRC_OVER2(3);
					buff[0] = dr;
				}
			}
		} else {
			if (alp_ind == -1) {
				for (i = 0; i < width / 4; i++) {
					BLEND44_SRC_OVER(0);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND44_SRC_OVER(0);
					buff[0] = dr;
				}
			} else {
				for (i = 0; i < width / 4; i++) {
					BLEND44_SRC_OVER(3);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND44_SRC_OVER(3);
					buff[0] = dr;
				}
			}
		}

		for (i = 0; i < (width & 3); i++) {
			((mlib_s32 *)tdp)[i] = ((mlib_s32 *)buff)[i];
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
