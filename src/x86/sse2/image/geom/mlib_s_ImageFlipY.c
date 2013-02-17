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

#pragma ident	"@(#)mlib_s_ImageFlipY.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageFlipY - flip image on Y axis
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipY(mlib_image *dst, const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     pointer to an dst image
 *      src     pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipY = __mlib_ImageFlipY

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageFlipY) mlib_ImageFlipY
    __attribute__((weak, alias("__mlib_ImageFlipY")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_ImageFlipY_BIT_1(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

#define	dw	sw
#define	dh	sh

#define	PREPAREVARS(data_type, num_ch)                           \
	mlib_s32 i, j;                                           \
	mlib_s32 numc = num_ch;                                  \
	mlib_s32 sstride = mlib_ImageGetStride(src);             \
	mlib_s32 dstride = mlib_ImageGetStride(dst);             \
	mlib_s32 ws = mlib_ImageGetWidth(src);                   \
	mlib_s32 hs = mlib_ImageGetHeight(src);                  \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                   \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                  \
	mlib_s32 sw = (ws < wd) ? ws : wd;                       \
	mlib_s32 sh = (hs < hd) ? hs : hd;                       \
	mlib_s32 dx = ws - wd;                                   \
	mlib_s32 dy = hs - hd;                                   \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);           \
	mlib_s32 dxd = ((dx > 0) ? 0 : -(dx >> 1));              \
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);           \
	mlib_s32 dyd = ((dy > 0) ? 0 : (-dy) >> 1);              \
	data_type *sdata =                                       \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(src) +   \
	    sstride * dys) + dxs * numc;                         \
	data_type *ddata =                                       \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(dst) +   \
	    dstride * dyd) + dxd * numc;

/* *********************************************************** */

#define	STRIP_1(dl, sl, w, h, dtype)                            \
	sl += (w - 1);                                          \
	for (i = 0; i < h; i++) {                               \
	    dtype *sp = sl;                                     \
	    dtype *dp = dl;                                     \
	                                                        \
	    for (j = 0; j <= w - 2; j += 2) {                   \
		dp[0] = sp[0];                                  \
		dp[1] = sp[-1];                                 \
		sp -= 2;                                        \
		dp += 2;                                        \
	    }                                                   \
	    if (j < w) {                                        \
		dp[0] = sp[0];                                  \
	    }                                                   \
	    sl = (void *)((mlib_u8 *)sl + sstride);             \
	    dl = (void *)((mlib_u8 *)dl + dstride);             \
	}

/* *********************************************************** */

void
mlib_s_ImageFlipY_U8_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 1);

	if (sw < 16) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u8);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + sw - 1;
		mlib_u8 *pdst_row = ddata + i * dstride;
		__m128i s0, s1, d0, d1;

		s0 = _mm_loadu_si128((__m128i *)(psrc_row - 15));

		for (j = 0; j <= (sw - 32); j += 16) {
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(s0, 8), s0);
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(d0, 8), d0);
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(d0, 8), d0);
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(d0, 8), d0);

			s0 = _mm_loadu_si128((__m128i *)(psrc_row - 31));

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 16;
			pdst_row += 16;
		}

		if (j <= (sw - 16)) {
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(s0, 8), s0);
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(d0, 8), d0);
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(d0, 8), d0);
			d0 = _mm_unpacklo_epi8(_mm_srli_si128(d0, 8), d0);

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 16;
			pdst_row += 16;
			j += 16;
		}

		for (; j < sw; j++) {
			(*pdst_row++) = *psrc_row--;
		}
	}
}

/* *********************************************************** */
#if 0	/* not used (slower ???) */

void
mlib_c_ImageFlipY_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s16, 1);

	if (sw < 8) {
		STRIP_1(ddata, sdata, sw, sh, mlib_s16);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_s16 *psrc_row =
			(mlib_s16 *)((mlib_u8 *)(sdata + sw - 1) + i * sstride);
		mlib_s16 *pdst_row =
			(mlib_s16 *)((mlib_u8 *)ddata + i * dstride);
		__m128i s0, s1, d0, d1;

		s0 = _mm_loadu_si128((__m128i *)(psrc_row - 7));

		for (j = 0; j <= (sw - 16); j += 8) {
			d0 = _mm_unpacklo_epi16(_mm_srli_si128(s0, 8), s0);
			d0 = _mm_unpacklo_epi16(_mm_srli_si128(d0, 8), d0);
			d0 = _mm_unpacklo_epi16(_mm_srli_si128(d0, 8), d0);

			s0 = _mm_loadu_si128((__m128i *)(psrc_row - 15));

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 8;
			pdst_row += 8;
		}

		if (j <= (sw - 8)) {
			d0 = _mm_unpacklo_epi16(_mm_srli_si128(s0, 8), s0);
			d0 = _mm_unpacklo_epi16(_mm_srli_si128(d0, 8), d0);
			d0 = _mm_unpacklo_epi16(_mm_srli_si128(d0, 8), d0);

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 8;
			pdst_row += 8;
			j += 8;
		}

		for (; j < sw; j++) {
			(*pdst_row++) = *psrc_row--;
		}
	}
}

#endif

/* *********************************************************** */

void
mlib_ImageFlipY_S32_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32, 1);

	if (sw < 4) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u32);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u32 *psrc_row =
			(mlib_u32 *)((mlib_u8 *)(sdata + sw - 1) + i * sstride);
		mlib_u32 *pdst_row =
			(mlib_u32 *)((mlib_u8 *)ddata + i * dstride);
		__m128i s0, s1, d0, d1;

		s0 = _mm_loadu_si128((__m128i *)(psrc_row - 3));

		for (j = 0; j <= (sw - 8); j += 4) {
			d0 = _mm_unpacklo_epi32(_mm_srli_si128(s0, 8), s0);
			d0 = _mm_unpacklo_epi32(_mm_srli_si128(d0, 8), d0);

			s0 = _mm_loadu_si128((__m128i *)(psrc_row - 7));

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 4;
			pdst_row += 4;
		}

		if (j <= (sw - 4)) {
			d0 = _mm_unpacklo_epi32(_mm_srli_si128(s0, 8), s0);
			d0 = _mm_unpacklo_epi32(_mm_srli_si128(d0, 8), d0);

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 4;
			pdst_row += 4;
			j += 4;
		}

		for (; j < sw; j++) {
			(*pdst_row++) = *psrc_row--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_D64_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s64, 1);

	STRIP_1(ddata, sdata, sw, sh, mlib_s64);
}

#if 0	/* don't used (slower ???) */

void
mlib_ImageFlipY_D64_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s64, 1);

	if (sw < 2) {
		STRIP_1(ddata, sdata, sw, sh, mlib_s64);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_s64 *psrc_row =
			(mlib_s64 *)((mlib_u8 *)(sdata + sw - 1) + i * sstride);
		mlib_s64 *pdst_row =
			(mlib_s64 *)((mlib_u8 *)ddata + i * dstride);
		__m128i s0, s1, d0, d1;

		s0 = _mm_loadu_si128((__m128i *)(psrc_row - 1));

		for (j = 0; j <= (sw - 4); j += 2) {
			d0 = _mm_unpacklo_epi64(_mm_srli_si128(s0, 8), s0);

			s0 = _mm_loadu_si128((__m128i *)(psrc_row - 3));

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 2;
			pdst_row += 2;
		}

		if (j <= (sw - 2)) {
			d0 = _mm_unpacklo_epi64(_mm_srli_si128(s0, 8), s0);

			_mm_storeu_si128((__m128i *)(pdst_row), d0);

			psrc_row -= 2;
			pdst_row += 2;
			j += 2;
		}

		for (; j < sw; j++) {
			(*pdst_row++) = *psrc_row--;
		}
	}
}

#endif

/* *********************************************************** */

#define	STRIP_2(dl, sl, w, h)                                   \
	sl += 2 * (w - 1);                                      \
	for (i = 0; i < h; i++) {                               \
	    __m128i *sp = (__m128i *)sl;                        \
	    __m128i *dp = (__m128i *)dl;                        \
	    __m128i d;                                          \
	                                                        \
	    for (j = 0; j < w; j++) {                           \
		d = _mm_loadu_si128(sp);                        \
		_mm_storeu_si128(dp, d);                        \
		sp --;                                          \
		dp ++;                                          \
	    }                                                   \
	    sl = (void *)((mlib_u8 *)sl + sstride);             \
	    dl = (void *)((mlib_u8 *)dl + dstride);             \
	}

/* *********************************************************** */

void
mlib_ImageFlipY_D64_2(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 2);

	STRIP_2(ddata, sdata, sw, sh);
}

/* *********************************************************** */

#define	STRIP_3(dl, sl, w, h, dtype)                            \
	sl += 3 * (w - 1);                                      \
	for (i = 0; i < h; i++) {                               \
	    dtype *sp = sl;                                     \
	    dtype *dp = dl;                                     \
	                                                        \
	    for (j = 0; j < w; j++) {                           \
		dp[0] = sp[0];                                  \
		dp[1] = sp[1];                                  \
		dp[2] = sp[2];                                  \
		sp -= 3;                                        \
		dp += 3;                                        \
	    }                                                   \
	    sl = (void *)((mlib_u8 *)sl + sstride);             \
	    dl = (void *)((mlib_u8 *)dl + dstride);             \
	}

/* *********************************************************** */

#define	STRIP_3a(dl, sl, w, h, dtype, ttype)                    \
	sl += 3 * (w - 1);                                      \
	for (i = 0; i < h; i++) {                               \
	    dtype *sp = sl;                                     \
	    dtype *dp = dl;                                     \
	                                                        \
	    for (j = 0; j <= (w - 1) - 2; j += 2) {             \
		*(ttype *) dp = *(ttype *) sp;                  \
		*(ttype *) (dp + 3) = *(ttype *) (sp - 3);      \
		sp -= 6;                                        \
		dp += 6;                                        \
	    }                                                   \
	    if (j < w - 1) {                                    \
		*(ttype *) dp = *(ttype *) sp;                  \
		sp -= 3;                                        \
		dp += 3;                                        \
	    }                                                   \
	    dp[0] = sp[0];                                      \
	    dp[1] = sp[1];                                      \
	    dp[2] = sp[2];                                      \
	    sl = (void *)((mlib_u8 *)sl + sstride);             \
	    dl = (void *)((mlib_u8 *)dl + dstride);             \
	}

/* *********************************************************** */

void
mlib_s_ImageFlipY_U8_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 3);

	STRIP_3a(ddata, sdata, sw, sh, mlib_u8,
	    mlib_u32);
}

/* *********************************************************** */

void
mlib_s_ImageFlipY_S16_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16, 3);

	STRIP_3a(ddata, sdata, sw, sh, mlib_u16, mlib_s64);
}

/* *********************************************************** */

void
mlib_ImageFlipY_S32_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32, 3);

	STRIP_3a(ddata, sdata, sw, sh, mlib_u32, __m128i);
}

/* *********************************************************** */

void
mlib_ImageFlipY_D64_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 3);

	STRIP_3(ddata, sdata, sw, sh, mlib_d64);
}

/* *********************************************************** */

#define	STRIP_4(dl, sl, w, h)                                   \
	sl += 4 * (w - 1);                                      \
	for (i = 0; i < h; i++) {                               \
	    __m128i *sp = (__m128i *)sl;                        \
	    __m128i *dp = (__m128i *)dl;                        \
	    __m128i d0, d1;                                     \
	                                                        \
	    for (j = 0; j < w; j++) {                           \
		d0 = _mm_loadu_si128(sp);                       \
		d1 = _mm_loadu_si128(sp + 1);                   \
		_mm_storeu_si128(dp, d0);                       \
		_mm_storeu_si128(dp + 1, d1);                   \
		sp -= 2;                                        \
		dp += 2;                                        \
	    }                                                   \
	    sl = (void *)((mlib_u8 *)sl + sstride);             \
	    dl = (void *)((mlib_u8 *)dl + dstride);             \
	}

/* *********************************************************** */

void
mlib_ImageFlipY_D64_4(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 4);

	STRIP_4(ddata, sdata, sw, sh);
}

/* *********************************************************** */

/*  C version  */

#undef PREPAREVARS
#undef STRIP_1
#undef STRIP_2

#define	dw	sw
#define	dh	sh
#define	PREPAREVARS(data_type, num_ch)                                \
	mlib_s32 numc = num_ch;                                       \
	mlib_s32 sstride =                                            \
	mlib_ImageGetStride(src) / sizeof (data_type);                \
	    mlib_s32 dstride =                                        \
		mlib_ImageGetStride(dst) / sizeof (data_type);        \
	    mlib_s32 ws = mlib_ImageGetWidth(src);                    \
	    mlib_s32 hs = mlib_ImageGetHeight(src);                   \
	    mlib_s32 wd = mlib_ImageGetWidth(dst);                    \
	    mlib_s32 hd = mlib_ImageGetHeight(dst);                   \
	    mlib_s32 sw = (ws < wd) ? ws : wd;                        \
	    mlib_s32 sh = (hs < hd) ? hs : hd;                        \
	    mlib_s32 dx = ((int)ws - (int)wd);                        \
	    mlib_s32 dy = ((int)hs - (int)hd);                        \
	    mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);            \
	    mlib_s32 dxd = ((dx > 0) ? 0 : -(dx >> 1));               \
	    mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);            \
	    mlib_s32 dyd = ((dy > 0) ? 0 : (-dy) >> 1);               \
	    data_type *sdata =                                        \
		(data_type *) mlib_ImageGetData(src) + dxs * numc +   \
	    sstride * dys;                                            \
	    data_type *ddata =                                        \
		(data_type *) mlib_ImageGetData(dst) + dxd * numc +   \
	    dstride * dyd;

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, data_type)                                \
	{                                                               \
	    mlib_s32 i0, j0, _w = (w), _h = (h);                        \
	    data_type *_ps = (ps), *_pd = (pd);                         \
	    data_type s0, s1;                                           \
	                                                                \
	    for (i0 = 0; i0 < _h; i0++) {                               \
		if (j0 = _w & 1)                                        \
		    _pd[i0 * dstride + (_w - 1)] = _ps[i0 * sstride];   \
		for (; j0 < _w; j0 += 2) {                              \
		    s0 = _ps[i0 * sstride + j0];                        \
		    s1 = _ps[i0 * sstride + j0 + 1];                    \
		    _pd[i0 * dstride + (_w - j0 - 1)] = s0;             \
		    _pd[i0 * dstride + (_w - j0 - 2)] = s1;             \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	STRIP_2(pd, ps, w, h, data_type)                            \
	{                                                           \
	    int i, j, _w = (w), _h = (h);                           \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1;                                       \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 2 * j];                  \
		    s1 = _ps[i * sstride + 2 * j + 1];              \
		    _pd[i * dstride + 2 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 2 * (_w - j - 1) + 1] = s1;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_c_ImageFlipY_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u16, 1);

	if (sw < 8) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u16);

		return;
	}
	for (i = 0; i < sh; i++) {
		mlib_u16 *psrc_row = sdata + i * sstride + sw - 1, *pdst_row =
		    ddata + i * dstride;

		if (!(((mlib_addr)(psrc_row + 1) ^ (mlib_addr)pdst_row) & 3)) {
			if (j = (int)(((mlib_addr)pdst_row) & 2)) {
				pdst_row[0] = psrc_row[0];
				j--;
			}
			for (; j <= (sw - 4); j += 4) {
				mlib_u32 src0 =
				    *((mlib_u32 *)(psrc_row - j - 1));
				mlib_u32 src1 =
				    *((mlib_u32 *)(psrc_row - j - 3));

				*((mlib_u32 *)(pdst_row + j)) =
				    (src0 >> 16) | (src0 << 16);
				*((mlib_u32 *)(pdst_row + j + 2)) =
				    (src1 >> 16) | (src1 << 16);
			}
		} else {
			if (j = (int)(((mlib_addr)pdst_row) & 2)) {
				pdst_row[0] = psrc_row[0];
				j--;
			}
			for (; j <= (sw - 4); j += 4) {
				mlib_u32 src0 = *((mlib_u16 *)(psrc_row - j)),
				    src1 = *((mlib_u16 *)(psrc_row - j - 1)),
				    src2 = *((mlib_u16 *)(psrc_row - j - 2)),
				    src3 = *((mlib_u16 *)(psrc_row - j - 3));

				*((mlib_u32 *)(pdst_row + j)) =
				    (src1 << 16) | src0;
				*((mlib_u32 *)(pdst_row + j + 2)) =
				    (src3 << 16) | src2;
			}
		}
		for (; j < sw; j++)
			pdst_row[j] = psrc_row[-j];
	}
}

/* *********************************************************** */

void
mlib_c_ImageFlipY_U8_2(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u8, 2);

	if ((((mlib_addr)sdata | sstride | (mlib_addr)ddata | dstride) & 1) ==
	    0) {
		mlib_c_ImageFlipY_S16_1(dst, src);
		return;
	}
	if (sw < 16) {
		STRIP_2(ddata, sdata, sw, sh, mlib_u8);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + (sw - 1) * 2,
		    *pdst_row = ddata + i * dstride;
		if (!((mlib_addr)pdst_row & 1)) {

			if (!(((mlib_addr)(psrc_row + 2) ^ (4 -
			    (mlib_addr)pdst_row)) & 3)) {
				if (j = (int)(((mlib_addr)pdst_row & 2) >> 1)) {
					*(mlib_u16 *)(pdst_row) =
					    *(mlib_u16 *)(psrc_row);
				}
				for (; j <= (sw - 2); j += 2) {
					mlib_u32 dst1 =
					    *((mlib_u32 *)(psrc_row - j * 2 -
					    2));

					*((mlib_u32 *)(pdst_row + j * 2)) =
					    (dst1 << 16) | (dst1 >> 16);
				}
				psrc_row++;
			} else {
				if (!((mlib_addr)psrc_row & 1)) {
					for (j = 0; j <= (sw - 2); j += 2) {
						mlib_u16 dst1 =
						    *((mlib_u16 *)(psrc_row -
						    j * 2)), dst0 =
						    *((mlib_u16 *)(psrc_row -
						    j * 2 - 2));

						*((mlib_u16 *)(pdst_row +
						    j * 2)) = dst1;
						*((mlib_u16 *)(pdst_row +
						    j * 2 + 2)) = dst0;
					}
					psrc_row++;
				} else {
					j = 0;
					psrc_row++;
				}
			}
			for (; j < sw; j++) {
				mlib_u16 dst1 =
				    *((mlib_u8 *)(psrc_row - j * 2)), dst0 =
				    *((mlib_u8 *)(psrc_row - j * 2 - 1));

				*((mlib_u16 *)(pdst_row + j * 2)) =
				    (dst1 << 8) | dst0;
			}
		} else {
			psrc_row = psrc_row + 1;
			for (j = 0; j < sw; j++) {
				mlib_u8 s1 = psrc_row[-j * 2], s0 =
				    psrc_row[-j * 2 - 1];

				pdst_row[j * 2] = s0;
				pdst_row[j * 2 + 1] = s1;
			}
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageFlipY(
    mlib_image *dst,
    const mlib_image *src)
{
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	switch (mlib_ImageGetType(src)) {

/*  handle MLIB_BIT data type of image  */

	case MLIB_BIT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_ImageFlipY_BIT_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_BYTE data type of image  */

	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageFlipY_U8_1(dst, src);
			break;
		case 2:
			mlib_c_ImageFlipY_U8_2(dst, src);
			break;
		case 3:
			mlib_s_ImageFlipY_U8_3(dst, src);
			break;
		case 4:
			mlib_ImageFlipY_S32_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_SHORT data type of image  */

	case MLIB_SHORT:
	case MLIB_USHORT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_c_ImageFlipY_S16_1(dst, src);
			break;
		case 2:
			mlib_ImageFlipY_S32_1(dst, src);
			break;
		case 3:
			mlib_s_ImageFlipY_S16_3(dst, src);
			break;
		case 4:
			mlib_ImageFlipY_D64_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_INT data type of image  */

	case MLIB_INT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_ImageFlipY_S32_1(dst, src);
			break;
		case 2:
			mlib_ImageFlipY_D64_1(dst, src);
			break;
		case 3:
			mlib_ImageFlipY_S32_3(dst, src);
			break;
		case 4:
			mlib_ImageFlipY_D64_2(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  discard any other data types  */

	default:
		return (MLIB_FAILURE);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
