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

#pragma ident	"@(#)mlib_m_ImageRotate180.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageRotate180 - rotate image 180 degrees clockwise
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRotate180(mlib_image *dst, const mlib_image *src)
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

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRotate180 = __mlib_ImageRotate180

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageRotate180) mlib_ImageRotate180
    __attribute__((weak, alias("__mlib_ImageRotate180")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_ImageRotate180_BIT_1(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_m_ImageRotate180_U8_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate180_U8_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_m_ImageRotate180_S16_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_m_ImageRotate180_S16_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_ImageRotate180_S32_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate180_S32_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_ImageRotate180_D64_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate180_D64_2(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate180_D64_3(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate180_D64_4(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageRotate180(
    mlib_image *dst,
    const mlib_image *src)
{

/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	switch (mlib_ImageGetType(src)) {

/*  handle MLIB_BIT data type of image  */

	case MLIB_BIT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_ImageRotate180_BIT_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_BYTE data type of image  */

	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_m_ImageRotate180_U8_1(dst, src);
			break;
		case 2:
			mlib_m_ImageRotate180_S16_1(dst, src);
			break;
		case 3:
			mlib_c_ImageRotate180_U8_3(dst, src);
			break;
		case 4:
			mlib_ImageRotate180_S32_1(dst, src);
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
			mlib_m_ImageRotate180_S16_1(dst, src);
			break;
		case 2:
			mlib_ImageRotate180_S32_1(dst, src);
			break;
		case 3:
			mlib_m_ImageRotate180_S16_3(dst, src);
			break;
		case 4:
			mlib_ImageRotate180_D64_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_INT data type of image  */

	case MLIB_INT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_ImageRotate180_S32_1(dst, src);
			break;
		case 2:
			mlib_ImageRotate180_D64_1(dst, src);
			break;
		case 3:
			mlib_ImageRotate180_S32_3(dst, src);
			break;
		case 4:
			mlib_ImageRotate180_D64_2(dst, src);
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

#define	dw	sw
#define	dh	sh

#define	PREPAREVARS(data_type, num_ch)                           \
	int numc = num_ch, i, j;                                 \
	mlib_s32 sstride = mlib_ImageGetStride(src);             \
	mlib_s32 dstride = mlib_ImageGetStride(dst);             \
	mlib_s32 ws = mlib_ImageGetWidth(src);                   \
	mlib_s32 hs = mlib_ImageGetHeight(src);                  \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                   \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                  \
	mlib_s32 sw = (ws < wd) ? ws : wd;                       \
	mlib_s32 sh = (hs < hd) ? hs : hd;                       \
	mlib_s32 dx = ((int)ws - (int)wd);                       \
	mlib_s32 dy = ((int)hs - (int)hd);                       \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);           \
	mlib_s32 dxd = ((dx > 0) ? 0 : -(dx >> 1));              \
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);           \
	mlib_s32 dyd = ((dy > 0) ? 0 : -(dy >> 1));              \
	data_type *sdata =                                       \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(src) +   \
	    sstride * dys) + dxs * numc;                         \
	data_type *ddata =                                       \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(dst) +   \
	    dstride * dyd) + dxd * numc;

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, data_type)                              \
	{                                                             \
	    int _w = (w), _h = (h);                                   \
	                                                              \
	    for (i = 0; i < _h; i++) {                                \
		data_type *_ps =                                      \
		    (data_type *) ((mlib_u8 *)(ps) + i * sstride) +   \
		    (_w - 1);                                         \
		data_type *_pd =                                      \
		    (data_type *) ((mlib_u8 *)(pd) + (_h - i -        \
		    1) * dstride);                                    \
		if (j = _w & 1) {                                     \
		    (*_pd++) = *_ps--;                                \
		}                                                     \
		for (; j < _w; j += 2) {                              \
		    _pd[0] = _ps[0];                                  \
		    _pd[1] = _ps[-1];                                 \
		    _pd += 2;                                         \
		    _ps -= 2;                                         \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

void
mlib_m_ImageRotate180_U8_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8,
	    1);

	if (sw < 16) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u8);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + sw - 1;
		mlib_u8 *pdst_row = ddata + (sh - i - 1) * dstride;
		__m64 s0, s1, d0, d1;

		s0 = *(__m64 *) (psrc_row - 7);
		s1 = *(__m64 *) (psrc_row - 15);

		for (j = 0; j <= (sw - 32); j += 16) {
			d0 = _mm_unpacklo_pi8(_mm_srli_si64(s0, 32), s0);
			d1 = _mm_unpacklo_pi8(_mm_srli_si64(s1, 32), s1);
			d0 = _mm_unpacklo_pi8(_mm_srli_si64(d0, 32), d0);
			d1 = _mm_unpacklo_pi8(_mm_srli_si64(d1, 32), d1);
			d0 = _mm_unpacklo_pi8(_mm_srli_si64(d0, 32), d0);
			d1 = _mm_unpacklo_pi8(_mm_srli_si64(d1, 32), d1);

			s0 = *(__m64 *) (psrc_row - 23);
			s1 = *(__m64 *) (psrc_row - 31);

			*(__m64 *) (pdst_row) = d0;
			*(__m64 *) (pdst_row + 8) = d1;

			psrc_row -= 16;
			pdst_row += 16;
		}

		if (j <= (sw - 16)) {
			d0 = _mm_unpacklo_pi8(_mm_srli_si64(s0, 32), s0);
			d1 = _mm_unpacklo_pi8(_mm_srli_si64(s1, 32), s1);
			d0 = _mm_unpacklo_pi8(_mm_srli_si64(d0, 32), d0);
			d1 = _mm_unpacklo_pi8(_mm_srli_si64(d1, 32), d1);
			d0 = _mm_unpacklo_pi8(_mm_srli_si64(d0, 32), d0);
			d1 = _mm_unpacklo_pi8(_mm_srli_si64(d1, 32), d1);

			*(__m64 *) (pdst_row) = d0;
			*(__m64 *) (pdst_row + 8) = d1;

			psrc_row -= 16;
			pdst_row += 16;
			j += 16;
		}

		for (; j < sw; j++) {
			(*pdst_row++) = *psrc_row--;
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageRotate180_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16,
	    1);

	if (sw < 8) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u16);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u16 *psrc_row =
		    (mlib_u16 *)((mlib_u8 *)sdata + i * sstride) + (sw - 1);
		mlib_u16 *pdst_row =
		    (mlib_u16 *)((mlib_u8 *)ddata + (sh - i - 1) * dstride);
		__m64 s0, s1, d0, d1;

		s0 = *(__m64 *) (psrc_row - 3);
		s1 = *(__m64 *) (psrc_row - 7);

		for (j = 0; j <= (sw - 16); j += 8) {
			d0 = _mm_unpacklo_pi16(_mm_srli_si64(s0, 32), s0);
			d1 = _mm_unpacklo_pi16(_mm_srli_si64(s1, 32), s1);
			d0 = _mm_unpacklo_pi16(_mm_srli_si64(d0, 32), d0);
			d1 = _mm_unpacklo_pi16(_mm_srli_si64(d1, 32), d1);

			s0 = *(__m64 *) (psrc_row - 11);
			s1 = *(__m64 *) (psrc_row - 15);

			*(__m64 *) (pdst_row) = d0;
			*(__m64 *) (pdst_row + 4) = d1;

			psrc_row -= 8;
			pdst_row += 8;
		}

		if (j <= (sw - 8)) {
			d0 = _mm_unpacklo_pi16(_mm_srli_si64(s0, 32), s0);
			d1 = _mm_unpacklo_pi16(_mm_srli_si64(s1, 32), s1);
			d0 = _mm_unpacklo_pi16(_mm_srli_si64(d0, 32), d0);
			d1 = _mm_unpacklo_pi16(_mm_srli_si64(d1, 32), d1);

			*(__m64 *) (pdst_row) = d0;
			*(__m64 *) (pdst_row + 4) = d1;

			psrc_row -= 8;
			pdst_row += 8;
			j += 8;
		}

		for (; j < sw; j++) {
			(*pdst_row++) = *psrc_row--;
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_ImageRotate180_S32_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s32,
	    1);

	for (i = 0; i < sh; i++) {
		mlib_s32 *ps =
		    (mlib_s32 *)((mlib_u8 *)sdata + i * sstride) + (sw - 1);
		mlib_s32 *pd =
		    (mlib_s32 *)((mlib_u8 *)ddata + (sh - i - 1) * dstride);

		j = 0;
		if (((int)pd & 7) == 4) {
			(*pd++) = *ps--;
			j = 1;
		}
		for (; j <= (sw - 8); j += 8) {
			((__m64 *) pd)[0] = mmx_freg_pair(ps[0], ps[-1]);
			((__m64 *) pd)[1] = mmx_freg_pair(ps[-2], ps[-3]);
			((__m64 *) pd)[2] = mmx_freg_pair(ps[-4], ps[-5]);
			((__m64 *) pd)[3] = mmx_freg_pair(ps[-6], ps[-7]);
			pd += 8;
			ps -= 8;
		}
		for (; j < sw; j++) {
			(*pd++) = *ps--;
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_ImageRotate180_D64_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 1);

	STRIP_1(ddata, sdata, sw, sh, __m64);

	_mm_empty();
}

/* *********************************************************** */

#define	STRIP_2(pd, ps, w, h, data_type)                              \
	{                                                             \
	    int _w = (w), _h = (h);                                   \
	                                                              \
	    for (i = 0; i < _h; i++) {                                \
		data_type *_ps =                                      \
		    (data_type *) ((mlib_u8 *)(ps) + i * sstride) +   \
		    2 * (_w - 1);                                     \
		data_type *_pd =                                      \
		    (data_type *) ((mlib_u8 *)(pd) + (_h - i -        \
		    1) * dstride);                                    \
		for (j = 0; j < _w; j++) {                            \
		    _pd[0] = _ps[0];                                  \
		    _pd[1] = _ps[1];                                  \
		    _pd += 2;                                         \
		    _ps -= 2;                                         \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

void
mlib_ImageRotate180_D64_2(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 2);

	STRIP_2(ddata, sdata, sw, sh, __m64);

	_mm_empty();
}

/* *********************************************************** */

#define	STRIP_3(pd, ps, w, h, data_type)                              \
	{                                                             \
	    int _w = (w), _h = (h);                                   \
	                                                              \
	    for (i = 0; i < _h; i++) {                                \
		data_type *_ps =                                      \
		    (data_type *) ((mlib_u8 *)(ps) + i * sstride) +   \
		    3 * (_w - 1);                                     \
		data_type *_pd =                                      \
		    (data_type *) ((mlib_u8 *)(pd) + (_h - i -        \
		    1) * dstride);                                    \
		for (j = 0; j < _w; j++) {                            \
		    _pd[0] = _ps[0];                                  \
		    _pd[1] = _ps[1];                                  \
		    _pd[2] = _ps[2];                                  \
		    _pd += 3;                                         \
		    _ps -= 3;                                         \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	STRIP_3a(pd, ps, w, h, data_type, ttype)                      \
	{                                                             \
	    int _w = (w), _h = (h);                                   \
	                                                              \
	    for (i = 0; i < _h; i++) {                                \
		data_type *_ps =                                      \
		    (data_type *) ((mlib_u8 *)(ps) + i * sstride) +   \
		    3 * (_w - 1);                                     \
		data_type *_pd =                                      \
		    (data_type *) ((mlib_u8 *)(pd) + (_h - i -        \
		    1) * dstride);                                    \
		for (j = 0; j < _w - 2; j += 2) {                     \
		    *(ttype *) _pd = *(ttype *) _ps;                  \
		    *(ttype *) (_pd + 3) = *(ttype *) (_ps - 3);      \
		    _pd += 6;                                         \
		    _ps -= 6;                                         \
		}                                                     \
		for (; j < _w - 1; j++) {                             \
		    *(ttype *) _pd = *(ttype *) _ps;                  \
		    _pd += 3;                                         \
		    _ps -= 3;                                         \
		}                                                     \
		if (_w) {                                             \
		    _pd[0] = _ps[0];                                  \
		    _pd[1] = _ps[1];                                  \
		    _pd[2] = _ps[2];                                  \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

void
mlib_m_ImageRotate180_S16_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16,
	    3);

	STRIP_3a(ddata, sdata, sw, sh, mlib_u16,
	    __m64);

	_mm_empty();
}

/* *********************************************************** */

void
mlib_ImageRotate180_S32_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32,
	    3);

	STRIP_3(ddata, sdata, sw, sh, mlib_u32);
}

/* *********************************************************** */

void
mlib_ImageRotate180_D64_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 3);

	STRIP_3(ddata, sdata, sw, sh, __m64);

	_mm_empty();
}

/* *********************************************************** */

#define	STRIP_4(pd, ps, w, h, data_type)                             \
	{                                                            \
	    int _w = (w), _h = (h);                                  \
	    data_type s0, s1, s2, s3;                                \
	                                                             \
	    for (i = 0; i < _h; i++) {                               \
		data_type *_ps =                                     \
		    (data_type *) ((mlib_u8 *)(ps) + i * sstride);   \
		data_type *_pd =                                     \
		    (data_type *) ((mlib_u8 *)(pd) + (_h - i -       \
		    1) * dstride);                                   \
		for (j = 0; j < _w; j++) {                           \
		    s0 = _ps[4 * j];                                 \
		    s1 = _ps[4 * j + 1];                             \
		    s2 = _ps[4 * j + 2];                             \
		    s3 = _ps[4 * j + 3];                             \
		    _pd[4 * (_w - j - 1)] = s0;                      \
		    _pd[4 * (_w - j - 1) + 1] = s1;                  \
		    _pd[4 * (_w - j - 1) + 2] = s2;                  \
		    _pd[4 * (_w - j - 1) + 3] = s3;                  \
		}                                                    \
	    }                                                        \
	}

/* *********************************************************** */

void
mlib_ImageRotate180_D64_4(
    mlib_image *dst,
    const mlib_image *src)
{

	PREPAREVARS(__m64, 4);

	STRIP_4(ddata, sdata, sw, sh, __m64);

	_mm_empty();
}

/* *********************************************************** */

void
mlib_c_ImageRotate180_U8_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8,
	    3);

	if (sw < 8) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u8);

		return;
	}
	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + (sw - 1) * 3,
		    *pdst_row = ddata + (sh - i - 1) * dstride;

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 1)) {
			if (sw & 1) {
				pdst_row[(sw - 1) * 3] = psrc_row[(1 - sw) * 3];
				pdst_row[sw * 3 - 2] = psrc_row[4 - sw * 3];
				pdst_row[sw * 3 - 1] = psrc_row[5 - sw * 3];
			}
			if (!((mlib_addr)pdst_row & 1)) {
				for (j = 0; j <= (sw - 2); j += 2) {
					mlib_u16 src0 =
					    *((mlib_u16 *)(psrc_row - j * 3));
					mlib_u16 src3 =
					    *((mlib_u16 *)(psrc_row - j * 3 -
					    2));
					mlib_u8 src1 =
					    *((mlib_u8 *)(psrc_row - j * 3 +
					    2));
					mlib_u8 src2 =
					    *((mlib_u8 *)(psrc_row - j * 3 -
					    3));

					*((mlib_u16 *)(pdst_row + j * 3)) =
					    src0;
					*((mlib_u8 *)(pdst_row + j * 3 + 2)) =
					    src1;
					*((mlib_u8 *)(pdst_row + j * 3 + 3)) =
					    src2;
					*((mlib_u16 *)(pdst_row + j * 3 + 4)) =
					    src3;
				}
			} else {
				for (j = 0; j <= (sw - 2); j += 2) {
					mlib_u8 src0 =
					    *((mlib_u8 *)(psrc_row - j * 3));
					mlib_u8 src3 =
					    *((mlib_u8 *)(psrc_row - j * 3 -
					    1));
					mlib_u16 src1 =
					    *((mlib_u16 *)(psrc_row - j * 3 +
					    1));
					mlib_u16 src2 =
					    *((mlib_u16 *)(psrc_row - j * 3 -
					    3));

					*((mlib_u8 *)(pdst_row + j * 3)) = src0;
					*((mlib_u16 *)(pdst_row + j * 3 + 1)) =
					    src1;
					*((mlib_u16 *)(pdst_row + j * 3 + 3)) =
					    src2;
					*((mlib_u8 *)(pdst_row + j * 3 + 5)) =
					    src3;
				}
			}
		} else {
			for (j = 0; j < sw; j++) {
				mlib_u8 s0, s1, s2;

				s0 = psrc_row[-j * 3];
				s1 = psrc_row[-j * 3 + 1];
				s2 = psrc_row[-j * 3 + 2];
				pdst_row[j * 3] = s0;
				pdst_row[j * 3 + 1] = s1;
				pdst_row[j * 3 + 2] = s2;
			}
		}
	}
}

/* *********************************************************** */

extern mlib_u8 mlib_fftK_Rev8[];

#define	TABLE	mlib_fftK_Rev8

void
mlib_ImageRotate180_BIT_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	mlib_u32 pix_sp, pix_dp;
	mlib_s32 dp_offset_end, sp_offset_orig;
	mlib_s32 dp_mask_end;
	mlib_s32 dp_mask_start;
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 dst_stride = mlib_ImageGetStride(dst);
	mlib_s32 ws = mlib_ImageGetWidth(src);
	mlib_s32 hs = mlib_ImageGetHeight(src);
	mlib_s32 wd = mlib_ImageGetWidth(dst);
	mlib_s32 hd = mlib_ImageGetHeight(dst);
	mlib_s32 sp_offset = mlib_ImageGetBitOffset(src);
	mlib_s32 dp_offset = mlib_ImageGetBitOffset(dst);
	mlib_s32 sw = (ws < wd) ? ws : wd;
	mlib_s32 sh = (hs < hd) ? hs : hd;
	mlib_s32 dx = ((int)ws - (int)wd);
	mlib_s32 dy = ((int)hs - (int)hd);
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);
	mlib_s32 dxd = ((dx > 0) ? 0 : -(dx >> 1));
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);
	mlib_s32 dyd = ((dy > 0) ? 0 : -(dy >> 1));
	mlib_u8 *sa =
	    (mlib_u8 *)mlib_ImageGetData(src) + (dxs + sp_offset) / 8 +
	    src_stride * dys;
	mlib_u8 *da =
	    (mlib_u8 *)mlib_ImageGetData(dst) + (dxd + dp_offset) / 8 +
	    dst_stride * dyd;
	mlib_u8 *sp;
	mlib_u8 *dp;

	sp_offset = (dxs + sp_offset) & 7;
	dp_offset = (dxd + dp_offset) & 7;
	dp_offset_end = (dp_offset + sw) & 7;
	dp_mask_end = 0xFF << (8 - dp_offset_end);
	dp_mask_start = 0xFF << (8 - dp_offset);
	da += (dp_offset + sw) / 8 + dst_stride * (sh - 1);
	sp_offset_orig = sp_offset;

	if (dp_offset + sw < 8)
		dp_mask_end ^= dp_mask_start;

	for (j = 0; j < sh; j++) {
		sp = sa;
		dp = da;

		if (sp_offset == dp_offset && dp_offset == 0 && !(sw & 7)) {
			dp--;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < sw / 8; i++)
				*dp-- = TABLE[(*sp++)];
		} else {

			pix_sp =
			    ((sp[0] << sp_offset) | (sp[1] >> (8 -
			    sp_offset))) & 0xFF;
			pix_dp = TABLE[pix_sp] << (8 - dp_offset_end);
			pix_dp =
			    (dp[0] & ~dp_mask_end) | (pix_dp & dp_mask_end);
			*dp-- = pix_dp;

			if (dp_offset + sw >= 8) {
				sp += (sp_offset + dp_offset_end) / 8;
				sp_offset = (dp_offset_end + sp_offset) & 7;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < (sw - dp_offset_end) / 8; i++) {
					pix_sp =
					    ((sp[0] << sp_offset) | (sp[1] >> (8
					    - sp_offset))) & 0xFF;
					*dp-- = TABLE[pix_sp];
					sp++;
				}

				if (dp_offset) {
					pix_sp =
					    ((sp[0] << sp_offset) | (sp[1] >> (8
					    - sp_offset))) & 0xFF;
					pix_dp = TABLE[pix_sp] & ~dp_mask_start;
					pix_dp |= dp[0] & dp_mask_start;
					*dp = pix_dp;
				}

			}
		}
		sa += src_stride;
		da -= dst_stride;
		sp_offset = sp_offset_orig;
	}
}

/* *********************************************************** */
