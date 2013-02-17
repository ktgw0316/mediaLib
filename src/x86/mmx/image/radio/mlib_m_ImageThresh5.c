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

#pragma ident	"@(#)mlib_m_ImageThresh5.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh5 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh5(mlib_image *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thigh,
 *                                    const mlib_s32   *tlow,
 *                                    const mlib_s32   *gmid);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     ponter to input image
 *      thigh   array of high thresholds
 *      tlow    array of low thresholds
 *      gmid    array of values between thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      If the pixel channel value is below the thigh for that channel
 *      and the pixel channel value is above the tlow for that channel,
 *      set the destination to the gmid value for that channel.
 *
 *                      +- src[x][y][c] src[x][y][c] < tlow[c]
 *                      |
 *      dst[x][y][c]  = +- gmid[c]      tlow[c] <= src[x][y][c] <= thigh[c]
 *                      |
 *                      +- src[x][y][c] src[x][y][c] > thigh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh5 = __mlib_ImageThresh5

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageThresh5) mlib_ImageThresh5
    __attribute__((weak, alias("__mlib_ImageThresh5")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	THRESH_U8(dd, x, N)                                            \
	ss = x;                                                        \
	s1 = _mm_xor_si64(ss, mask80);                                 \
	mask0 = _mm_cmpgt_pi8(tl##N, s1);                              \
	mask1 = _mm_cmpgt_pi8(s1, th##N);                              \
	mask = _mm_or_si64(mask0, mask1);                              \
	dd =                                                           \
	_mm_or_si64(_mm_andnot_si64(mask, gm##N), _mm_and_si64(mask,   \
	    ss))

#define	THRESH_S16(dd, x, N)                                           \
	ss = x;                                                        \
	mask0 = _mm_cmpgt_pi16(tl##N, ss);                             \
	mask1 = _mm_cmpgt_pi16(ss, th##N);                             \
	mask = _mm_or_si64(mask0, mask1);                              \
	dd =                                                           \
	_mm_or_si64(_mm_andnot_si64(mask, gm##N), _mm_and_si64(mask,   \
	    ss))

#define	THRESH_S32(dd, x, N)                                           \
	ss = x;                                                        \
	mask0 = _mm_cmpgt_pi32(tl##N, ss);                             \
	mask1 = _mm_cmpgt_pi32(ss, th##N);                             \
	mask = _mm_or_si64(mask0, mask1);                              \
	dd =                                                           \
	_mm_or_si64(_mm_andnot_si64(mask, gm##N), _mm_and_si64(mask,   \
	    ss))

/* *********************************************************** */

#define	SAVE_EDGE(dst, dd, emask)                                     \
	dst =                                                         \
	_mm_or_si64(_mm_and_si64(emask, dd), _mm_andnot_si64(emask,   \
	    dst))

/* *********************************************************** */

#define	SIZEOF_U8	1
#define	SIZEOF_S16	2
#define	SIZEOF_S32	4

/* *********************************************************** */

#define	FUNC_THRESH_1(TYPE)                                            \
	void                                                           \
	mlib_m_ImageThresh5_##TYPE##_1(mlib_u8 *psrc,                  \
	    mlib_s32 src_stride, mlib_u8 *pdst, mlib_s32 dst_stride,   \
	    mlib_s32 width, mlib_s32 height, mlib_u8 *thigh,           \
	    mlib_u8 *tlow, mlib_u8 *gmid, mlib_s32 nchan)              \
	{                                                              \
	    __m64 th0, tl0, gm0, ss, s1, dd;                           \
	    __m64 emask, mask80, mask0, mask1, mask;                   \
	    mlib_s32 i, j, tail0, tail1 = 0;                           \
	                                                               \
	    th0 = *(__m64 *) thigh;                                    \
	    tl0 = *(__m64 *) tlow;                                     \
	    gm0 = *(__m64 *) gmid;                                     \
	    if (SIZEOF_##TYPE == 1) {                                  \
		mask80 = mmx_from_int_dup(0x80808080);                 \
		th0 = _mm_xor_si64(th0, mask80);                       \
		tl0 = _mm_xor_si64(tl0, mask80);                       \
	    }                                                          \
	    width *= SIZEOF_##TYPE * nchan;                            \
	    tail0 = width & 15;                                        \
	    if (tail0 > 8) {                                           \
		tail1 = tail0 - 8;                                     \
		tail0 = 8;                                             \
	    }                                                          \
	    for (i = 0; i < height; i++) {                             \
		__m64 *sp = (__m64 *) (psrc + i * src_stride);         \
		__m64 *dp = (__m64 *) (pdst + i * dst_stride);         \
	                                                               \
		for (j = 0; j <= width / 8 - 2; j += 2) {              \
		    THRESH_##TYPE(dp[j], sp[j], 0);                    \
		    THRESH_##TYPE(dp[j + 1], sp[j + 1], 0);            \
		}                                                      \
		if (tail0) {                                           \
		    THRESH_##TYPE(dd, sp[j], 0);                       \
		    emask = ((__m64 *) mlib_mask64_arr)[tail0];        \
		    SAVE_EDGE(dp[j], dd, emask);                       \
		}                                                      \
		if (tail1) {                                           \
		    THRESH_##TYPE(dd, sp[j + 1], 0);                   \
		    emask = ((__m64 *) mlib_mask64_arr)[tail1];        \
		    SAVE_EDGE(dp[j + 1], dd, emask);                   \
		}                                                      \
	    }                                                          \
	    _mm_empty();                                               \
	}

/* *********************************************************** */

#define	FUNC_THRESH_2(TYPE)                                            \
	void                                                           \
	mlib_m_ImageThresh5_##TYPE##_2(mlib_u8 *psrc,                  \
	    mlib_s32 src_stride, mlib_u8 *pdst, mlib_s32 dst_stride,   \
	    mlib_s32 width, mlib_s32 height, mlib_u8 *thigh,           \
	    mlib_u8 *tlow, mlib_u8 *gmid, mlib_s32 nchan)              \
	{                                                              \
	    __m64 th0, tl0, gm0, th1, tl1, gm1;                        \
	    __m64 ss, s1, dd;                                          \
	    __m64 emask, mask80, mask0, mask1, mask;                   \
	    mlib_s32 i, j, val, wid8, tail;                            \
	                                                               \
	    th0 = *(__m64 *) thigh;                                    \
	    tl0 = *(__m64 *) tlow;                                     \
	    gm0 = *(__m64 *) gmid;                                     \
	    th1 = *(__m64 *) (thigh + 8);                              \
	    tl1 = *(__m64 *) (tlow + 8);                               \
	    gm1 = *(__m64 *) (gmid + 8);                               \
	    width *= SIZEOF_##TYPE * nchan;                            \
	    for (i = 0; i < height; i++) {                             \
		__m64 *sp = (__m64 *) (psrc + i * src_stride);         \
		__m64 *dp = (__m64 *) (pdst + i * dst_stride);         \
	                                                               \
		for (j = 0; j < width / 16; j++) {                     \
		    THRESH_##TYPE(dp[2 * j], sp[2 * j], 0);            \
		    THRESH_##TYPE(dp[2 * j + 1], sp[2 * j + 1], 1);    \
		}                                                      \
	    }                                                          \
	    _mm_empty();                                               \
	}

/* *********************************************************** */

#define	FUNC_THRESH_3(TYPE)                                               \
	void                                                              \
	mlib_m_ImageThresh5_##TYPE##_3(mlib_u8 *psrc,                     \
	    mlib_s32 src_stride, mlib_u8 *pdst, mlib_s32 dst_stride,      \
	    mlib_s32 width, mlib_s32 height, mlib_u8 *thigh,              \
	    mlib_u8 *tlow, mlib_u8 *gmid, mlib_s32 off1, mlib_s32 off2)   \
	{                                                                 \
	    __m64 th0, tl0, gm0, th1, tl1, gm1, th2, tl2, gm2;            \
	    __m64 ss, s1, dd;                                             \
	    __m64 emask, mask80, mask0, mask1, mask;                      \
	    mlib_s32 i, j, val, wid8, tail;                               \
	                                                                  \
	    th0 = *(__m64 *) thigh;                                       \
	    tl0 = *(__m64 *) tlow;                                        \
	    gm0 = *(__m64 *) gmid;                                        \
	    th1 = *(__m64 *) (thigh + off1);                              \
	    tl1 = *(__m64 *) (tlow + off1);                               \
	    gm1 = *(__m64 *) (gmid + off1);                               \
	    th2 = *(__m64 *) (thigh + off2);                              \
	    tl2 = *(__m64 *) (tlow + off2);                               \
	    gm2 = *(__m64 *) (gmid + off2);                               \
	    if (SIZEOF_##TYPE == 1) {                                     \
		mask80 = mmx_from_int_dup(0x80808080);                    \
		th0 = _mm_xor_si64(th0, mask80);                          \
		tl0 = _mm_xor_si64(tl0, mask80);                          \
		th1 = _mm_xor_si64(th1, mask80);                          \
		tl1 = _mm_xor_si64(tl1, mask80);                          \
		th2 = _mm_xor_si64(th2, mask80);                          \
		tl2 = _mm_xor_si64(tl2, mask80);                          \
	    }                                                             \
	    width *= SIZEOF_##TYPE;                                       \
	    wid8 = width / 8;                                             \
	    tail = 3 * (width - 8 * wid8);                                \
	    for (i = 0; i < height; i++) {                                \
		__m64 *sp = (__m64 *) (psrc + i * src_stride);            \
		__m64 *dp = (__m64 *) (pdst + i * dst_stride);            \
	                                                                  \
		for (j = 0; j < wid8; j++) {                              \
		    THRESH_##TYPE(dp[3 * j], sp[3 * j], 0);               \
		    THRESH_##TYPE(dp[3 * j + 1], sp[3 * j + 1], 1);       \
		    THRESH_##TYPE(dp[3 * j + 2], sp[3 * j + 2], 2);       \
		}                                                         \
		if (tail) {                                               \
		    emask = ((__m64 *) mlib_mask64_arr)[tail & 7];        \
		    THRESH_##TYPE(dd, sp[3 * j], 0);                      \
		    if (tail <= 8) {                                      \
			SAVE_EDGE(dp[3 * j], dd, emask);                  \
		    } else {                                              \
			dp[3 * j] = dd;                                   \
			THRESH_##TYPE(dd, sp[3 * j + 1], 1);              \
			if (tail <= 16) {                                 \
			    SAVE_EDGE(dp[3 * j + 1], dd, emask);          \
			} else {                                          \
			    dp[3 * j + 1] = dd;                           \
			    THRESH_##TYPE(dd, sp[3 * j + 2], 2);          \
			    SAVE_EDGE(dp[3 * j + 2], dd, emask);          \
			}                                                 \
		    }                                                     \
		}                                                         \
	    }                                                             \
	    _mm_empty();                                                  \
	}

/* *********************************************************** */

FUNC_THRESH_1(U8)
    FUNC_THRESH_3(U8)

    FUNC_THRESH_1(S16)
    FUNC_THRESH_3(S16)

    FUNC_THRESH_1(S32)
    FUNC_THRESH_2(S32)
    FUNC_THRESH_3(S32)

/* *********************************************************** */

#define	INIT_THRESH0(n)                                         \
	thigh0 = thigh[(n)];                                    \
	tlow0 = tlow[(n)];                                      \
	gmid0 = gmid[(n)];

#define	INIT_THRESH1(n)               \
	thigh1 = thigh[(n)];          \
	tlow1  = tlow[(n)];           \
	gmid1  = gmid[(n)];           \

#define	INIT_THRESH2(n)                                         \
	thigh2 = thigh[(n)];                                    \
	tlow2 = tlow[(n)];                                      \
	gmid2 = gmid[(n)];

#define	INIT_THRESH3(n)               \
	thigh3 = thigh[(n)];          \
	tlow3  = tlow[(n)];           \
	gmid3  = gmid[(n)];           \

/* *********************************************************** */

#define	DO_THRESH(s0, th, tl, gm)                                  \
	((s0 & (((s0 - tl) | (th - s0)) >> 31)) | (gm & ~(((s0 -   \
	    tl) | (th - s0)) >> 31)))
#define	THRESH0(s0)	DO_THRESH(s0, thigh0, tlow0, gmid0)
#define	THRESH1(s0)	DO_THRESH(s0, thigh1, tlow1, gmid1)
#define	THRESH2(s0)	DO_THRESH(s0, thigh2, tlow2, gmid2)
#define	THRESH3(s0)	DO_THRESH(s0, thigh3, tlow3, gmid3)

/* *********************************************************** */

void
mlib_m_ImageThresh5_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, tlow0;
	mlib_s32 gmid0;

	INIT_THRESH0(0);
	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH0(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH0(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH0(psrc_row[j + 7]);
		}
		for (; j < width; j++) {
			pdst_row[j] = THRESH0(psrc_row[j]);
		}
	}
}

/* *********************************************************** */

void
mlib_m_ImageThresh5_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 gmid0, gmid1;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	src_stride >>= 1;
	dst_stride >>= 1;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH1(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
		}
		for (; j < width; j += 2) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}
	}
}

/* *********************************************************** */

void
mlib_m_ImageThresh5_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 gmid0, gmid1, gmid2;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 12); j += 12) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH1(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH2(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
			pdst_row[j + 8] = THRESH2(psrc_row[j + 8]);
			pdst_row[j + 9] = THRESH0(psrc_row[j + 9]);
			pdst_row[j + 10] = THRESH1(psrc_row[j + 10]);
			pdst_row[j + 11] = THRESH2(psrc_row[j + 11]);
		}
		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
	}
}

/* *********************************************************** */

void
mlib_m_ImageThresh5_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1, thigh2, thigh3;
	mlib_s32 tlow0, tlow1, tlow2, tlow3;
	mlib_s32 gmid0, gmid1, gmid2, gmid3;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	INIT_THRESH3(3);
	width <<= 2;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH2(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH3(psrc_row[j + 7]);
		}
		if (j < width) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh5(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;
	mlib_s32 i;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(src, dst);
	dtype = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	sstride = mlib_ImageGetStride(src);
	dstride = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	for (i = 0; i < nchan; i++) {
		if (thigh[i] < tlow[i])
			return (MLIB_FAILURE);
	}

	if (dtype == MLIB_BYTE) {
		mlib_u8 th[10], tl[10], gm[10];

		for (i = 0; i < nchan; i++) {
			gm[i] = gmid[i];
			th[i] = thigh[i];
			tl[i] = tlow[i];
			if ((thigh[i] < 0) || (tlow[i] > MLIB_U8_MAX)) {
				gm[i] = th[i] = tl[i] = 0;
			} else {
				if (thigh[i] > MLIB_U8_MAX)
					th[i] = MLIB_U8_MAX;
				if (tlow[i] < 0)
					tl[i] = 0;
			}
		}
		for (i = nchan; i < 10; i++) {
			gm[i] = gm[i - nchan];
			th[i] = th[i - nchan];
			tl[i] = tl[i - nchan];
		}

		if (nchan != 3) {
			mlib_m_ImageThresh5_U8_1(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm, nchan);
		} else {
			mlib_m_ImageThresh5_U8_3(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm, 2, 1);
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {
		mlib_s16 th[8], tl[8], gm[8];

		for (i = 0; i < nchan; i++) {
			gm[i] = gmid[i];
			th[i] = thigh[i];
			tl[i] = tlow[i];
			if ((tlow[i] > MLIB_S16_MAX) ||
			    (thigh[i] < MLIB_S16_MIN)) {
				gm[i] = th[i] = tl[i] = 0;
			} else {
				if (thigh[i] > MLIB_S16_MAX)
					th[i] = MLIB_S16_MAX;
				if (tlow[i] < MLIB_S16_MIN)
					tl[i] = MLIB_S16_MIN;
			}
		}
		for (i = nchan; i < 8; i++) {
			gm[i] = gm[i - nchan];
			th[i] = th[i - nchan];
			tl[i] = tl[i - nchan];
		}

		if (nchan != 3) {
			mlib_m_ImageThresh5_S16_1(psrc, sstride, pdst, dstride,
			    width, height, (void *)th, (void *)tl, (void *)gm,
			    nchan);
		} else {
			mlib_m_ImageThresh5_S16_3(psrc, sstride, pdst, dstride,
			    width, height, (void *)th, (void *)tl, (void *)gm,
			    2, 4);
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {
		mlib_s32 th[4], tl[4], gm[4];

		for (i = 0; i < nchan; i++) {
			gm[i] = (gmid[i] << 16) >> 16;
			th[i] = thigh[i];
			tl[i] = tlow[i];
			if ((tl[i] > MLIB_U16_MAX) || (th[i] < MLIB_U16_MIN))
				gm[i] = th[i] = tl[i] = 0;
			else {
				if (th[i] > MLIB_U16_MAX)
					th[i] = MLIB_U16_MAX;
				if (tl[i] < MLIB_U16_MIN)
					tl[i] = MLIB_U16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_m_ImageThresh5_U161(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gm);
			break;
		case 2:
			mlib_m_ImageThresh5_U162(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gm);
			break;
		case 3:
			mlib_m_ImageThresh5_U163(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gm);
			break;
		case 4:
			mlib_m_ImageThresh5_U164(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gm);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {
		mlib_s32 th[8], tl[8], gm[8];

		for (i = 0; i < nchan; i++) {
			gm[i] = gmid[i];
			th[i] = thigh[i];
			tl[i] = tlow[i];
		}
		for (i = nchan; i < 8; i++) {
			gm[i] = gm[i - nchan];
			th[i] = th[i - nchan];
			tl[i] = tl[i - nchan];
		}

		if (nchan <= 2) {
			mlib_m_ImageThresh5_S32_1(psrc, sstride, pdst, dstride,
			    width, height, (void *)th, (void *)tl, (void *)gm,
			    nchan);
		} else if (nchan == 3) {
			mlib_m_ImageThresh5_S32_3(psrc, sstride, pdst, dstride,
			    width, height, (void *)th, (void *)tl, (void *)gm,
			    8, 4);
		} else {
			mlib_m_ImageThresh5_S32_2(psrc, sstride, pdst, dstride,
			    width, height, (void *)th, (void *)tl, (void *)gm,
			    nchan);
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
