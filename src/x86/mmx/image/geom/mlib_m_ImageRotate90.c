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

#pragma ident	"@(#)mlib_m_ImageRotate90.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageRotate90 - rotate image 90 degrees clockwise
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRotate90(mlib_image *dst, const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRotate90 = __mlib_ImageRotate90

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageRotate90) mlib_ImageRotate90
    __attribute__((weak, alias("__mlib_ImageRotate90")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_ImageFlipMainDiag_BIT_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

    void mlib_m_ImageRotate90_U8_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate90_U8_2(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_m_ImageRotate90_U8_3(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate90_U8_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_m_ImageRotate90_S16_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate90_S16_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_m_ImageRotate90_S16_3(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate90_S16_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_ImageRotate90_S32_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate90_S32_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_ImageRotate90_D64_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate90_D64_2(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate90_D64_3(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageRotate90_D64_4(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageRotate90(
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
			return (mlib_ImageFlipMainDiag_BIT_1(dst, src, 0, -1));
		default:
			return (MLIB_FAILURE);
		}
/*  handle MLIB_BYTE data type of image  */
	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_m_ImageRotate90_U8_1(dst, src);
			break;
		case 2:
			mlib_c_ImageRotate90_U8_2(dst, src);
			break;
		case 3:
			mlib_c_ImageRotate90_U8_3(dst, src);
			break;
		case 4:
			mlib_ImageRotate90_S32_1(dst, src);
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
			mlib_c_ImageRotate90_S16_1(dst, src);
			break;
		case 2:
			mlib_ImageRotate90_S32_1(dst, src);
			break;
		case 3:
			mlib_c_ImageRotate90_S16_3(dst, src);
			break;
		case 4:
			mlib_ImageRotate90_D64_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_INT data type of image  */
	case MLIB_INT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_ImageRotate90_S32_1(dst, src);
			break;
		case 2:
			mlib_ImageRotate90_D64_1(dst, src);
			break;
		case 3:
			mlib_ImageRotate90_S32_3(dst, src);
			break;
		case 4:
			mlib_ImageRotate90_D64_2(dst, src);
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

#define	dw	sh
#define	dh	sw

#define	PREPAREVARS(data_type, num_ch)                               \
	const mlib_s32 sstride = mlib_ImageGetStride(src);           \
	const mlib_s32 dstride = mlib_ImageGetStride(dst);           \
	const mlib_s32 ws = mlib_ImageGetWidth(src);                 \
	const mlib_s32 hs = mlib_ImageGetHeight(src);                \
	const mlib_s32 wd = mlib_ImageGetWidth(dst);                 \
	const mlib_s32 hd = mlib_ImageGetHeight(dst);                \
	const mlib_s32 sw = (ws < hd) ? ws : hd;                     \
	const mlib_s32 sh = (hs < wd) ? hs : wd;                     \
	const mlib_s32 dx = (int)ws - (int)hd;                       \
	const mlib_s32 dy = (int)hs - (int)wd;                       \
	const mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);         \
	const mlib_s32 dyd = ((dx > 0) ? 0 : (-dx) >> 1);            \
	const mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);         \
	const mlib_s32 dxd = ((dy > 0) ? 0 : -(dy >> 1));            \
	const mlib_s32 s_head = 0;                                   \
	const mlib_s32 d_head = 0;                                   \
	const data_type *const sdata =                               \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(src) +       \
	    sstride * dys) + dxs * num_ch;                           \
	const data_type *const sdata1 =                              \
	    (data_type *) ((mlib_u8 *)sdata + (sh - 1) * sstride);   \
	data_type *const ddata =                                     \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(dst) +       \
	    dstride * dyd) + dxd * num_ch;

/* *********************************************************** */

#define	TRANSPOSE_8X8_1                                         \
	{                                                       \
	    __m64 m04, m26, m15, m37, m0426, m1537;             \
	                                                        \
	    m04 = _mm_unpacklo_pi8(wi0, wi4);                   \
	    m26 = _mm_unpacklo_pi8(wi2, wi6);                   \
	    m15 = _mm_unpacklo_pi8(wi1, wi5);                   \
	    m37 = _mm_unpacklo_pi8(wi3, wi7);                   \
	    m0426 = _mm_unpacklo_pi8(m04, m26);                 \
	    m1537 = _mm_unpacklo_pi8(m15, m37);                 \
	    wo0 = _mm_unpacklo_pi8(m0426, m1537);               \
	    wo1 = _mm_unpackhi_pi8(m0426, m1537);               \
	    m0426 = _mm_unpackhi_pi8(m04, m26);                 \
	    m1537 = _mm_unpackhi_pi8(m15, m37);                 \
	    wo2 = _mm_unpacklo_pi8(m0426, m1537);               \
	    wo3 = _mm_unpackhi_pi8(m0426, m1537);               \
	    m04 = _mm_unpackhi_pi8(wi0, wi4);                   \
	    m26 = _mm_unpackhi_pi8(wi2, wi6);                   \
	    m15 = _mm_unpackhi_pi8(wi1, wi5);                   \
	    m37 = _mm_unpackhi_pi8(wi3, wi7);                   \
	    m0426 = _mm_unpacklo_pi8(m04, m26);                 \
	    m1537 = _mm_unpacklo_pi8(m15, m37);                 \
	    wo4 = _mm_unpacklo_pi8(m0426, m1537);               \
	    wo5 = _mm_unpackhi_pi8(m0426, m1537);               \
	    m0426 = _mm_unpackhi_pi8(m04, m26);                 \
	    m1537 = _mm_unpackhi_pi8(m15, m37);                 \
	    wo6 = _mm_unpacklo_pi8(m0426, m1537);               \
	    wo7 = _mm_unpackhi_pi8(m0426, m1537);               \
	}

/* *********************************************************** */

#define	TRANSPOSE_4X4_2                                         \
	{                                                       \
	    __m64 m0, m1, m2, m3;                               \
	                                                        \
	    m0 = _mm_unpacklo_pi16(wi0, wi2);                   \
	    m1 = _mm_unpackhi_pi16(wi0, wi2);                   \
	    m2 = _mm_unpacklo_pi16(wi1, wi3);                   \
	    m3 = _mm_unpackhi_pi16(wi1, wi3);                   \
	    wo0 = _mm_unpacklo_pi16(m0, m2);                    \
	    wo1 = _mm_unpackhi_pi16(m0, m2);                    \
	    wo2 = _mm_unpacklo_pi16(m1, m3);                    \
	    wo3 = _mm_unpackhi_pi16(m1, m3);                    \
	}

/* *********************************************************** */

#define	TRANSPOSE_2X2_2                                         \
	wo0 = _mm_unpacklo_pi32(wi0, wi1);                      \
	wo1 = _mm_unpackhi_pi32(wi0, wi1);

/* *********************************************************** */

#define	ALOAD_4X4_1S16                                          \
	{                                                       \
	    const mlib_u16 *const psq2 =                        \
		(mlib_u16 *)((mlib_u8 *)pssq - 2 * sstride);    \
	    wi0 = *(__m64 *) (pssq);                            \
	    wi1 = *(__m64 *) ((mlib_u8 *)pssq - sstride);       \
	    wi2 = *(__m64 *) (psq2);                            \
	    wi3 = *(__m64 *) ((mlib_u8 *)psq2 - sstride);       \
	    pssq += 4;                                          \
	}

/* *********************************************************** */

#define	ALOAD_8X8_1U8                                           \
	{                                                       \
	    const mlib_u8 *const psq2 = pssq - 2 * sstride;     \
	    const mlib_u8 *const psq4 = psq2 - 2 * sstride;     \
	    const mlib_u8 *const psq6 = psq4 - 2 * sstride;     \
	                                                        \
	    wi0 = *(__m64 *) (pssq);                            \
	    wi1 = *(__m64 *) (pssq - sstride);                  \
	    wi2 = *(__m64 *) (psq2);                            \
	    wi3 = *(__m64 *) (psq2 - sstride);                  \
	    wi4 = *(__m64 *) (psq4);                            \
	    wi5 = *(__m64 *) (psq4 - sstride);                  \
	    wi6 = *(__m64 *) (psq6);                            \
	    wi7 = *(__m64 *) (psq6 - sstride);                  \
	    pssq += 8;                                          \
	}

/* *********************************************************** */

#define	ASTORE_4X4_1S16                                         \
	* (__m64 *) pdsq = wo0;                                 \
	pdsq = (void *)((mlib_u8 *)pdsq + dstride);             \
	*(__m64 *) pdsq = wo1;                                  \
	pdsq = (void *)((mlib_u8 *)pdsq + dstride);             \
	*(__m64 *) pdsq = wo2;                                  \
	pdsq = (void *)((mlib_u8 *)pdsq + dstride);             \
	*(__m64 *) pdsq = wo3;                                  \
	pdsq = (void *)((mlib_u8 *)pdsq + dstride);

/* *********************************************************** */

#define	ASTORE_8X8_1U8                                          \
	* (__m64 *) pdsq = wo0;                                 \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo1;                                  \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo2;                                  \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo3;                                  \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo4;                                  \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo5;                                  \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo6;                                  \
	pdsq += dstride;                                        \
	*(__m64 *) pdsq = wo7;                                  \
	pdsq += dstride;

/* *********************************************************** */

#define	ALOAD_2X2_1S32                                          \
	{                                                       \
	    wi0 = *(__m64 *) (pssq);                            \
	    wi1 = *(__m64 *) ((mlib_u8 *)pssq - sstride);       \
	    pssq += 2;                                          \
	}

/* *********************************************************** */

#define	ASTORE_2X2_1S32                                         \
	* (__m64 *) pdsq = wo0;                                 \
	pdsq = (void *)((mlib_u8 *)pdsq + dstride);             \
	*(__m64 *) pdsq = wo1;                                  \
	pdsq = (void *)((mlib_u8 *)pdsq + dstride);

/* *********************************************************** */

/*  process strip of single channel image  */
#define	STRIP_1(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
		for (y = 0; y < _h; ++y) {                        \
		    dpix[0] = spix[0];                            \
		    spix = (void *)((mlib_u8 *)spix - sstride);   \
		    dpix += 1;                                    \
		}                                                 \
		spix = _ps + x;                                   \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

#define	STRIPH_1(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
		for (x = 0; x < _w; ++x) {                        \
		    dpix[0] = spix[0];                            \
		    dpix = (void *)((mlib_u8 *)dpix + dstride);   \
		    spix += 1;                                    \
		}                                                 \
		dpix = _pd + y;                                   \
		spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 2 channels image ; w and h in pixels!  */
#define	STRIP_2(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
		for (y = 0; y < _h; ++y) {                        \
		    data_type c1, c2;                             \
	                                                          \
		    c1 = spix[0];                                 \
		    c2 = spix[1];                                 \
		    dpix[0] = c1;                                 \
		    dpix[1] = c2;                                 \
		    spix = (void *)((mlib_u8 *)spix - sstride);   \
		    dpix += 2;                                    \
		}                                                 \
		spix = _ps + (x << 1);                            \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STRIPH_2(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
		for (x = 0; x < _w; ++x) {                        \
		    data_type c1, c2;                             \
	                                                          \
		    c1 = spix[0];                                 \
		    c2 = spix[1];                                 \
		    dpix[0] = c1;                                 \
		    dpix[1] = c2;                                 \
		    dpix = (void *)((mlib_u8 *)dpix + dstride);   \
		    spix += 2;                                    \
		}                                                 \
		dpix = _pd + (y << 1);                            \
		spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 4 channels image ; w and h in pixels!  */
#define	STRIP_4(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
		for (y = 0; y < _h; ++y) {                        \
		    data_type c1, c2, c3, c4;                     \
	                                                          \
		    c1 = spix[0];                                 \
		    c2 = spix[1];                                 \
		    c3 = spix[2];                                 \
		    c4 = spix[3];                                 \
		    dpix[0] = c1;                                 \
		    dpix[1] = c2;                                 \
		    dpix[2] = c3;                                 \
		    dpix[3] = c4;                                 \
		    spix = (void *)((mlib_u8 *)spix - sstride);   \
		    dpix += 4;                                    \
		}                                                 \
		spix = _ps + (x << 2);                            \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STRIPH_4(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
		for (x = 0; x < _w; ++x) {                        \
		    data_type c1, c2, c3, c4;                     \
	                                                          \
		    c1 = spix[0];                                 \
		    c2 = spix[1];                                 \
		    c3 = spix[2];                                 \
		    c4 = spix[3];                                 \
		    dpix[0] = c1;                                 \
		    dpix[1] = c2;                                 \
		    dpix[2] = c3;                                 \
		    dpix[3] = c4;                                 \
		    dpix = (void *)((mlib_u8 *)dpix + dstride);   \
		    spix += 4;                                    \
		}                                                 \
		dpix = _pd + (y << 2);                            \
		spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 3 channels image ; w and h in pixels!  */
#define	STRIP_3(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
		for (y = 0; y < _h; ++y) {                        \
		    data_type c1, c2, c3;                         \
	                                                          \
		    c1 = spix[0];                                 \
		    c2 = spix[1];                                 \
		    c3 = spix[2];                                 \
		    dpix[0] = c1;                                 \
		    dpix[1] = c2;                                 \
		    dpix[2] = c3;                                 \
		    spix = (void *)((mlib_u8 *)spix - sstride);   \
		    dpix += 3;                                    \
		}                                                 \
		spix = _ps + ((x << 1) + x);                      \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STRIPH_3(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
		for (x = 0; x < _w; ++x) {                        \
		    data_type c1, c2, c3;                         \
	                                                          \
		    c1 = spix[0];                                 \
		    c2 = spix[1];                                 \
		    c3 = spix[2];                                 \
		    dpix[0] = c1;                                 \
		    dpix[1] = c2;                                 \
		    dpix[2] = c3;                                 \
		    dpix = (void *)((mlib_u8 *)dpix + dstride);   \
		    spix += 3;                                    \
		}                                                 \
		dpix = _pd + ((y << 1) + y);                      \
		spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, data_type, num_ch)                         \
	const int s_tail = (mlib_addr)(sw - s_head) % xpix;              \
	const int d_tail = (mlib_addr)(dw - d_head) % ypix;              \
	const int numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;   \
	const int numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;   \
	const data_type *const srcstart =                                \
	    (data_type *) ((mlib_u8 *)sdata + sstride * (sh - 1));       \
	const data_type *const pssq_a =                                  \
	    (data_type *) ((mlib_u8 *)srcstart - d_head * sstride) +     \
	    s_head * num_ch;                                             \
	data_type *const pdsq_a =                                        \
	    (data_type *) ((mlib_u8 *)ddata + s_head * dstride) +        \
	    d_head * num_ch;                                             \
	const data_type *pssq = pssq_a;                                  \
	data_type *pdsq = pdsq_a;                                        \
	int ysq;

/*  body  */

/* *********************************************************** */

#define	BRAK_CLOSE(xpix, ypix, data_type, num_ch)                           \
	if (d_head)                                                         \
	    STRIPH_##num_ch(ddata, srcstart, (sw - s_tail), d_head,         \
		data_type);                                                 \
	if (s_head)                                                         \
	    STRIP_##num_ch(ddata + num_ch * d_head,                         \
		(mlib_u8 *)srcstart - sstride * d_head, s_head,             \
		(dw - d_head - d_tail), data_type);                         \
	if (d_tail)                                                         \
	    STRIPH_##num_ch(ddata + num_ch * (dw - d_tail),                 \
		(mlib_u8 *)sdata + sstride * (d_tail - 1), sw, d_tail,      \
		data_type);                                                 \
	if (s_tail)                                                         \
	    STRIP_##num_ch((mlib_u8 *)ddata + dstride * (dh - s_tail),      \
		srcstart + num_ch * (sw - s_tail), s_tail, (dw - d_tail),   \
		data_type);

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                                \
	pssq = (void *)((mlib_u8 *)pssq_a - ysq * ypix * sstride);   \
	pdsq = pdsq_a + ysq * (ypix * num_ch);

/* *********************************************************** */

void
mlib_m_ImageRotate90_U8_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 1)

	if (sw < 8 || sh < 8) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_u8);
	} else {
		BRAK_OPEN(8, 8, mlib_u8, 1)

		if (numhorzsq < 1) {
			STRIP_1(ddata, srcstart, sw, sh, mlib_u8);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			__m64 wi0, wi1, wi2, wi3, wi4, wi5, wi6, wi7;
			__m64 wo0, wo1, wo2, wo3, wo4, wo5, wo6, wo7;
			const mlib_u8 *const psend = pssq + numhorzsq * 8;

			ALOAD_8X8_1U8;
			while (pssq < psend) {
				TRANSPOSE_8X8_1;
				ASTORE_8X8_1U8;
				ALOAD_8X8_1U8;
			}
			TRANSPOSE_8X8_1;
			ASTORE_8X8_1U8;
			INCREMENT(8, 8, 1);
		}
		BRAK_CLOSE(8, 8, mlib_u8, 1)

		_mm_empty();
	}
}

/* *********************************************************** */

void
mlib_m_ImageRotate90_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16, 1)
	if (sw < 4 || sh < 4) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_u16);
	} else {
		BRAK_OPEN(4, 4, mlib_u16, 1)

		if (numvertsq < 1 || numhorzsq < 1) {
			STRIP_1(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			int xsq;
			__m64 wi0, wi1, wi2, wi3;
			__m64 wo0, wo1, wo2, wo3;

			ALOAD_4X4_1S16;
			for (xsq = 1; xsq < numhorzsq; ++xsq) {
				TRANSPOSE_4X4_2;
				ASTORE_4X4_1S16;
				ALOAD_4X4_1S16;
			}
			TRANSPOSE_4X4_2;
			ASTORE_4X4_1S16;
			INCREMENT(4, 4, 1);
		}
		BRAK_CLOSE(4, 4, mlib_u16, 1)

		_mm_empty();
	}
}

/* *********************************************************** */

void
mlib_ImageRotate90_S32_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32, 1)
	if (sw < 2 || sh < 2) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_u32);
	} else {
		BRAK_OPEN(2, 2, mlib_u32, 1)

		if (numvertsq < 1 || numhorzsq < 1) {
			STRIP_1(ddata, sdata1, sw, sh, mlib_u32);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			int xsq;
			__m64 wi0, wi1;
			__m64 wo0, wo1;

			ALOAD_2X2_1S32;
			for (xsq = 1; xsq < numhorzsq; ++xsq) {
				TRANSPOSE_2X2_2;
				ASTORE_2X2_1S32;
				ALOAD_2X2_1S32;
			}
			TRANSPOSE_2X2_2;
			ASTORE_2X2_1S32;
			INCREMENT(2, 2, 1);
		}
		BRAK_CLOSE(2, 2, mlib_u32, 1)

		_mm_empty();
	}
}

/* *********************************************************** */

void
mlib_ImageRotate90_S32_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32,
	    3)
	    STRIP_3(
	    ddata,
	    sdata1,
	    sw,
	    sh,
	    mlib_u32);
}

/* *********************************************************** */

void
mlib_ImageRotate90_D64_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 1)
	    STRIP_1(ddata, sdata1, sw, sh, __m64);
	_mm_empty();
}

/* *********************************************************** */

void
mlib_ImageRotate90_D64_2(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 2)
	    STRIP_2(ddata, sdata1, sw, sh, __m64);
	_mm_empty();
}

/* *********************************************************** */

void
mlib_ImageRotate90_D64_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 3)
	    STRIP_3(ddata, sdata1, sw, sh, __m64);
	_mm_empty();
}

/* *********************************************************** */

void
mlib_ImageRotate90_D64_4(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(__m64, 4)
	    STRIP_4(ddata, sdata1, sw, sh, __m64);
	_mm_empty();
}

/* *********************************************************** */

/*  C version  */

#undef PREPAREVARS
#undef STRIP_1
#undef STRIPH_1
#undef STRIP_3
#undef STRIPH_3
#undef BRAK_OPEN
#undef BRAK_CLOSE
#undef INCREMENT

#define	dw	sh
#define	dh	sw
#define	PREPAREVARS(DATA_TYPE, num_ch)                                \
	mlib_s32 numc = num_ch;                                       \
	mlib_s32 sstride =                                            \
	mlib_ImageGetStride(src) / sizeof (DATA_TYPE);                \
	    mlib_s32 dstride =                                        \
		mlib_ImageGetStride(dst) / sizeof (DATA_TYPE);        \
	    mlib_s32 ws = mlib_ImageGetWidth(src);                    \
	    mlib_s32 hs = mlib_ImageGetHeight(src);                   \
	    mlib_s32 wd = mlib_ImageGetWidth(dst);                    \
	    mlib_s32 hd = mlib_ImageGetHeight(dst);                   \
	    mlib_s32 sw = (ws < hd) ? ws : hd;                        \
	    mlib_s32 sh = (hs < wd) ? hs : wd;                        \
	    mlib_s32 dx = (mlib_s32)ws - (mlib_s32)hd;                \
	    mlib_s32 dy = (mlib_s32)hs - (mlib_s32)wd;                \
	    mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);            \
	    mlib_s32 dyd = ((dx > 0) ? 0 : (-dx) >> 1);               \
	    mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);            \
	    mlib_s32 dxd = ((dy > 0) ? 0 : -(dy >> 1));               \
	    DATA_TYPE *sdata =                                        \
		(DATA_TYPE *) mlib_ImageGetData(src) + dxs * numc +   \
	    sstride * dys;                                            \
	    DATA_TYPE *ddata =                                        \
		(DATA_TYPE *) mlib_ImageGetData(dst) + dxd * numc +   \
	    dstride * dyd;

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, DATA_TYPE)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
		for (y = 0; y < _h; ++y) {                      \
		    dpix[0] = spix[0];                          \
		    spix -= sstride;                            \
		    dpix += 1;                                  \
		}                                               \
		spix = _ps + x;                                 \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIPH_1(pd, ps, w, h, DATA_TYPE)                       \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (y = 1; y <= _h; ++y) {                         \
		for (x = 0; x < _w; ++x) {                      \
		    dpix[0] = spix[0];                          \
		    dpix += dstride;                            \
		    spix += 1;                                  \
		}                                               \
		dpix = _pd + y;                                 \
		spix = _ps - y * sstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIP_3(pd, ps, w, h, DATA_TYPE)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
		for (y = 0; y < _h; ++y) {                      \
		    DATA_TYPE c1, c2, c3;                       \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    c3 = spix[2];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    dpix[2] = c3;                               \
		    spix -= sstride;                            \
		    dpix += 3;                                  \
		}                                               \
		spix = _ps + ((x << 1) + x);                    \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIPH_3(pd, ps, w, h, DATA_TYPE)                       \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (y = 1; y <= _h; ++y) {                         \
		for (x = 0; x < _w; ++x) {                      \
		    DATA_TYPE c1, c2, c3;                       \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    c3 = spix[2];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    dpix[2] = c3;                               \
		    dpix += dstride;                            \
		    spix += 3;                                  \
		}                                               \
		dpix = _pd + ((y << 1) + y);                    \
		spix = _ps - y * sstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, DATA_TYPE, num_ch)                          \
	mlib_s32 s_tail = (mlib_u32)(sw - s_head) % xpix;                 \
	mlib_s32 d_tail = (mlib_u32)(dw - d_head) % ypix;                 \
	mlib_s32 numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;     \
	mlib_s32 numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;     \
	DATA_TYPE *srcstart = sdata + sstride * (sh - 1);                 \
	DATA_TYPE *pssq_a =                                               \
	    srcstart + s_head * num_ch - d_head * sstride;                \
	DATA_TYPE *pdsq_a = ddata + d_head * num_ch + s_head * dstride;   \
	DATA_TYPE *pssq = pssq_a;                                         \
	DATA_TYPE *pdsq = pdsq_a;                                         \
	mlib_s32 ysq;

/* *********************************************************** */

#define	BRAK_CLOSE(xpix, ypix, DATA_TYPE, num_ch)                              \
	if (d_head)                                                            \
	    STRIPH_##num_ch(ddata, srcstart, (sw - s_tail), d_head,            \
		DATA_TYPE);                                                    \
	if (s_head)                                                            \
	    STRIP_##num_ch(ddata + num_ch * d_head,                            \
		srcstart - sstride * d_head, s_head, (dw - d_head - d_tail),   \
		DATA_TYPE);                                                    \
	if (d_tail)                                                            \
	    STRIPH_##num_ch(ddata + num_ch * (dw - d_tail),                    \
		sdata + sstride * (d_tail - 1), sw, d_tail, DATA_TYPE);        \
	if (s_tail)                                                            \
	    STRIP_##num_ch(ddata + dstride * (dh - s_tail),                    \
		srcstart + num_ch * (sw - s_tail), s_tail, (dw - d_tail),      \
		DATA_TYPE);

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                           \
	pssq = pssq_a - ysq * ypix * sstride;                   \
	pdsq = pdsq_a + ysq * (ypix * num_ch);

/* *********************************************************** */

void
mlib_c_ImageRotate90_U8_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 3);

	if (!((sstride | dstride) & 3)) {
		mlib_s32 s_head = (mlib_addr)sdata & 3;
		mlib_s32 d_head = (mlib_addr)ddata & 3;

		BRAK_OPEN(4, 4, mlib_u8, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u8);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u32 w0, w1, w2, w3, w4, w5, w6, w7;
				mlib_u8 *psq2 = pssq - 2 * sstride;

				w0 = *(mlib_u32 *)pssq;
				w1 = *(mlib_u32 *)(pssq - sstride);
				w2 = *(mlib_u32 *)(psq2);
				w3 = *(mlib_u32 *)(psq2 - sstride);

				*(mlib_u32 *)pdsq =
				    (w0 & 0xffffff) | (w1 << 24);
				*(mlib_u32 *)(pdsq + 4) =
				    ((w1 & 0xffff00) >> 8) | (w2 << 16);
				*(mlib_u32 *)(pdsq + 8) =
				    ((w2 & 0xff0000) >> 16) | (w3 << 8);

				w4 = *(mlib_u32 *)(pssq + 4);
				w5 = *(mlib_u32 *)(pssq - sstride + 4);
				w6 = *(mlib_u32 *)(psq2 + 4);
				w7 = *(mlib_u32 *)(psq2 - sstride + 4);

				pdsq += dstride;
				*(mlib_u32 *)(pdsq) =
				    w0 >> 24 | ((w4 << 8) & 0xffffff) | (w1 &
				    0xff000000);
				*(mlib_u32 *)(pdsq + 4) =
				    (w5 & 0xffff) | ((w2 & 0xff000000) >> 8) |
				    (w6 << 24);
				*(mlib_u32 *)(pdsq + 8) =
				    ((w6 >> 8) & 0xff) | ((w3 >> 16) & 0xff00) |
				    (w7 << 16);

				w0 = *(mlib_u32 *)(pssq + 8);
				w1 = *(mlib_u32 *)(pssq - sstride + 8);
				w2 = *(mlib_u32 *)(psq2 + 8);
				w3 = *(mlib_u32 *)(psq2 - sstride + 8);

				pdsq += dstride;
				*(mlib_u32 *)(pdsq) =
				    (w4 >> 16) | ((w0 & 0xff) << 16) | ((w5 &
				    0xff0000) << 8);
				*(mlib_u32 *)(pdsq + 4) =
				    (w5 >> 24) | ((w1 & 0xff) << 8) | (w6 &
				    0xffff0000);
				*(mlib_u32 *)(pdsq + 8) =
				    (w2 & 0xff) | ((w7 >> 8) & 0xffff00) | (w3
				    << 24);

				pdsq += dstride;
				*(mlib_u32 *)(pdsq) =
				    (w0 >> 8) | ((w1 & 0xff00) << 16);
				*(mlib_u32 *)(pdsq + 4) =
				    (w1 >> 16) | ((w2 & 0xffff00) << 8);
				*(mlib_u32 *)(pdsq + 8) =
				    (w2 >> 24) | (w3 & 0xffffff00);

				pssq += 12;
				pdsq += dstride;
			}
			INCREMENT(4, 4, 3)
		}
		BRAK_CLOSE(4, 4, mlib_u8, 3)
	} else {
/*  if any stride %4 != 0  */
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(2, 2, mlib_u8, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u8);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u8 a0, a1, a2, a3, a4, a5;

				a0 = pssq[0];
				a1 = pssq[1];
				a2 = pssq[2];
				a3 = pssq[-sstride];
				a4 = pssq[-sstride + 1];
				a5 = pssq[-sstride + 2];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;
				pdsq += dstride;

				a0 = pssq[3];
				a1 = pssq[4];
				a2 = pssq[5];
				a3 = pssq[-sstride + 3];
				a4 = pssq[-sstride + 4];
				a5 = pssq[-sstride + 5];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;

				pssq += 6;
				pdsq += dstride;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u8, 3)
	}
}

/* *********************************************************** */

void
mlib_c_ImageRotate90_S16_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16, 3)

	if (!((sstride | dstride) & 1)) {
		mlib_s32 s_head = ((mlib_addr)sdata & 2) >> 1;
		mlib_s32 d_head = ((mlib_addr)ddata & 2) >> 1;

		BRAK_OPEN(2, 2, mlib_u16, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u32 w0, w1, w2, w3, w4, w5;
				mlib_u16 *psq2 = pssq - sstride;

				w0 = *(mlib_u32 *)pssq;
				w1 = *(mlib_u32 *)(pssq + 2);
				w2 = *(mlib_u32 *)(pssq + 4);
				w3 = *(mlib_u32 *)psq2;
				w4 = *(mlib_u32 *)(psq2 + 2);
				w5 = *(mlib_u32 *)(psq2 + 4);

				*(mlib_u32 *)pdsq = w0;
				*(mlib_u32 *)(pdsq + 2) =
				    (w1 & 0xffff) | (w3 << 16);
				*(mlib_u32 *)(pdsq + 4) =
				    (w3 >> 16) | (w4 << 16);
				pdsq += dstride;

				*(mlib_u32 *)pdsq = (w1 >> 16) | (w2 << 16);
				*(mlib_u32 *)(pdsq + 2) =
				    (w2 >> 16) | (w4 & 0xffff0000);
				*(mlib_u32 *)(pdsq + 4) = w5;

				pssq += 6;
				pdsq += dstride;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u16, 3)
	} else {
/*  if any stride %4 != 0  */
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(2, 2, mlib_u16, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u16 a0, a1, a2, a3, a4, a5;
				mlib_u16 *psq2 = pssq - sstride;

				a0 = pssq[0];
				a1 = pssq[1];
				a2 = pssq[2];
				a3 = psq2[0];
				a4 = psq2[1];
				a5 = psq2[2];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;
				pdsq += dstride;

				a0 = pssq[3];
				a1 = pssq[4];
				a2 = pssq[5];
				a3 = psq2[3];
				a4 = psq2[4];
				a5 = psq2[5];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;

				pssq += 6;
				pdsq += dstride;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u16, 3)
	}
}

/* *********************************************************** */

#define	LOAD_1X4_2U8                                            \
	a0 = pssq[0];                                           \
	a1 = pssq[-sstride];                                    \
	a2 = psq2[0];                                           \
	a3 = psq2[-sstride];

/* *********************************************************** */

#define	LOAD_1X4_2U8_NEXT                                       \
	a4 = pssq[0];                                           \
	a5 = pssq[-sstride];                                    \
	a6 = psq2[0];                                           \
	a7 = psq2[-sstride];

/* *********************************************************** */

#define	STORE_8X1_2U8                                           \
	pdsq[0] = a0;                                           \
	pdsq[1] = a4;                                           \
	pdsq[2] = a1;                                           \
	pdsq[3] = a5;                                           \
	pdsq[4] = a2;                                           \
	pdsq[5] = a6;                                           \
	pdsq[6] = a3;                                           \
	pdsq[7] = a7;

/* *********************************************************** */

#define	DIRECT_TRANSP_4X4_2U8_INC                               \
	{                                                       \
	    mlib_u8 *psq2 = pssq - sstride * 2;                 \
	                                                        \
	    LOAD_1X4_2U8;                                       \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    LOAD_1X4_2U8_NEXT;                                  \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    STORE_8X1_2U8;                                      \
	    pdsq += dstride;                                    \
	    LOAD_1X4_2U8;                                       \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    LOAD_1X4_2U8_NEXT;                                  \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    STORE_8X1_2U8;                                      \
	    pdsq += dstride;                                    \
	    LOAD_1X4_2U8;                                       \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    LOAD_1X4_2U8_NEXT;                                  \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    STORE_8X1_2U8;                                      \
	    pdsq += dstride;                                    \
	    LOAD_1X4_2U8;                                       \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    LOAD_1X4_2U8_NEXT;                                  \
	    pssq += 1;                                          \
	    STORE_8X1_2U8;                                      \
	    pdsq += dstride;                                    \
	}

/* *********************************************************** */

void
mlib_c_ImageRotate90_U8_2(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 2)
	if (sw < 4 || sh < 4) {
		STRIP_2(ddata, (sdata + sstride * (sh - 1)), sw, sh, mlib_u8);
		return;
	}
	if (sstride & 1 || dstride & 1 || (mlib_addr)sdata & 1 ||
	    (mlib_addr)ddata & 1) {
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(4, 4, mlib_u8, 2)
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u8 a0, a1, a2, a3, a4, a5, a6, a7;

				DIRECT_TRANSP_4X4_2U8_INC;
			}
			INCREMENT(4, 4, 2)
		}
		BRAK_CLOSE(4, 4, mlib_u8, 2)
	} else {
		mlib_c_ImageRotate90_S16_1(dst, src);
	}
}

/* *********************************************************** */

#define	LOAD_4X2_S16_1                                          \
	{                                                       \
	    mlib_u16 *psq1 = pssq - sstride;                    \
	    mlib_u16 *psq2 = psq1 - sstride;                    \
	    mlib_u16 *psq3 = psq2 - sstride;                    \
	                                                        \
	    w0 = *(mlib_u32 *)(pssq);                           \
	    w1 = *(mlib_u32 *)(pssq + 2);                       \
	    w2 = *(mlib_u32 *)(psq1);                           \
	    w3 = *(mlib_u32 *)(psq1 + 2);                       \
	    w4 = *(mlib_u32 *)(psq2);                           \
	    w5 = *(mlib_u32 *)(psq2 + 2);                       \
	    w6 = *(mlib_u32 *)(psq3);                           \
	    w7 = *(mlib_u32 *)(psq3 + 2);                       \
	    pssq += 4;                                          \
	}

/* *********************************************************** */

#define	TRANSTORE_2X4_S16_1_BE                                      \
	* (mlib_u32 *)(pdsq) = (w0 & 0xffff0000) | (w2 >> 16);      \
	*(mlib_u32 *)(pdsq + 2) = (w4 & 0xffff0000) | (w6 >> 16);   \
	pdsq += dstride;                                            \
	*(mlib_u32 *)(pdsq) = (w0 << 16) | (w2 & 0xffff);           \
	*(mlib_u32 *)(pdsq + 2) = (w4 << 16) | (w6 & 0xffff);       \
	pdsq += dstride;                                            \
	*(mlib_u32 *)(pdsq) = (w1 & 0xffff0000) | (w3 >> 16);       \
	*(mlib_u32 *)(pdsq + 2) = (w5 & 0xffff0000) | (w7 >> 16);   \
	pdsq += dstride;                                            \
	*(mlib_u32 *)(pdsq) = (w1 << 16) | (w3 & 0xffff);           \
	*(mlib_u32 *)(pdsq + 2) = (w5 << 16) | (w7 & 0xffff);       \
	pdsq += dstride;

/* *********************************************************** */

#define	TRANSTORE_2X4_S16_1_LE                                      \
	* (mlib_u32 *)(pdsq) = (w0 & 0xffff) | (w2 << 16);          \
	*(mlib_u32 *)(pdsq + 2) = (w4 & 0xffff) | (w6 << 16);       \
	pdsq += dstride;                                            \
	*(mlib_u32 *)(pdsq) = (w0 >> 16) | (w2 & 0xffff0000);       \
	*(mlib_u32 *)(pdsq + 2) = (w4 >> 16) | (w6 & 0xffff0000);   \
	pdsq += dstride;                                            \
	*(mlib_u32 *)(pdsq) = (w1 & 0xffff) | (w3 << 16);           \
	*(mlib_u32 *)(pdsq + 2) = (w5 & 0xffff) | (w7 << 16);       \
	pdsq += dstride;                                            \
	*(mlib_u32 *)(pdsq) = (w1 >> 16) | (w3 & 0xffff0000);       \
	*(mlib_u32 *)(pdsq + 2) = (w5 >> 16) | (w7 & 0xffff0000);   \
	pdsq += dstride;

/* *********************************************************** */

#define	DIRECT_TRANSP_4X4_INC                                   \
	{                                                       \
	    mlib_u16 *psq2 = pssq - 2 * sstride;                \
	                                                        \
	    a0 = pssq[0];                                       \
	    a1 = pssq[-sstride];                                \
	    a2 = psq2[0];                                       \
	    a3 = psq2[-sstride];                                \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    pdsq[0] = a0;                                       \
	    pdsq[1] = a1;                                       \
	    pdsq[2] = a2;                                       \
	    pdsq[3] = a3;                                       \
	    pdsq += dstride;                                    \
	    a0 = pssq[0];                                       \
	    a1 = pssq[-sstride];                                \
	    a2 = psq2[0];                                       \
	    a3 = psq2[-sstride];                                \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    pdsq[0] = a0;                                       \
	    pdsq[1] = a1;                                       \
	    pdsq[2] = a2;                                       \
	    pdsq[3] = a3;                                       \
	    pdsq += dstride;                                    \
	    a0 = pssq[0];                                       \
	    a1 = pssq[-sstride];                                \
	    a2 = psq2[0];                                       \
	    a3 = psq2[-sstride];                                \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    pdsq[0] = a0;                                       \
	    pdsq[1] = a1;                                       \
	    pdsq[2] = a2;                                       \
	    pdsq[3] = a3;                                       \
	    pdsq += dstride;                                    \
	    a0 = pssq[0];                                       \
	    a1 = pssq[-sstride];                                \
	    a2 = psq2[0];                                       \
	    a3 = psq2[-sstride];                                \
	    pssq += 1;                                          \
	    pdsq[0] = a0;                                       \
	    pdsq[1] = a1;                                       \
	    pdsq[2] = a2;                                       \
	    pdsq[3] = a3;                                       \
	    pdsq += dstride;                                    \
	}

/* *********************************************************** */

void
mlib_c_ImageRotate90_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16, 1)

/*  assume data % 2 == 0 && stride % 2 =0  */
	if (!((sstride | dstride) & 1)) {
		mlib_s32 s_head = ((mlib_addr)sdata & 3) / 2;
		mlib_s32 d_head = ((mlib_addr)ddata & 3) / 2;

		BRAK_OPEN(4, 4, mlib_u16, 1)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_1(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u32 w0, w1, w2, w3, w4, w5, w6, w7;

				LOAD_4X2_S16_1;
				TRANSTORE_2X4_S16_1_LE;
			}
			INCREMENT(4, 4, 1)
		}
		BRAK_CLOSE(4, 4, mlib_u16, 1)
	} else {
/*  if any stride %4 != 0  */
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(4, 4, mlib_u16, 1)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_1(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}

		for (ysq = 1; ysq <= numvertsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				mlib_u16 a0, a1, a2, a3;

				DIRECT_TRANSP_4X4_INC;
			}
			INCREMENT(4, 4, 1)
		}
		BRAK_CLOSE(4, 4, mlib_u16, 1)
	}
}

/* *********************************************************** */
