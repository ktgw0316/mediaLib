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

#pragma ident	"@(#)mlib_m_ImageMinimum.c	9.9	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMinimum - calculate minimum for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMinimum(mlib_s32         *min,
 *                                    const mlib_image *img)
 *
 * ARGUMENTS
 *      min     pointer to min vector
 *      img     pointer to an image
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *      min = MIN (img[i][j])
 *            i, j
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMinimum = __mlib_ImageMinimum

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMinimum) mlib_ImageMinimum
    __attribute__((weak, alias("__mlib_ImageMinimum")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_m_ImageMinimum_U8_124(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_U8_3(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_S16_124(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_S16_3(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_U16_124(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_U16_3(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_S32_124(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_m_ImageMinimum_S32_3(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_c_ImageMinimum_S32_124(
    mlib_s32 *res32,
    const mlib_image *img);
    void mlib_c_ImageMinimum_S32_3(
    mlib_s32 *res32,
    const mlib_image *img);

/* *********************************************************** */

mlib_status
__mlib_ImageMinimum(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s32 j, channels;
	mlib_s32 res32[4];

	MLIB_IMAGE_CHECK(img);

	if (res == NULL)
		return (MLIB_NULLPOINTER);

	channels = mlib_ImageGetChannels(img);

	switch (mlib_ImageGetType(img)) {
	case MLIB_BYTE:
		switch (channels) {
		case 1:
		case 2:
		case 4:
			mlib_m_ImageMinimum_U8_124(res32, img);
			break;
		case 3:
			mlib_m_ImageMinimum_U8_3(res32, img);
			break;
		}

		break;

	case MLIB_SHORT:

		switch (channels) {
		case 1:
		case 2:
		case 4:
			mlib_m_ImageMinimum_S16_124(res32, img);
			break;
		case 3:
			mlib_m_ImageMinimum_S16_3(res32, img);
			break;
		}

		break;

	case MLIB_USHORT:

		switch (channels) {
		case 1:
		case 2:
		case 4:
			mlib_m_ImageMinimum_U16_124(res32, img);
			break;
		case 3:
			mlib_m_ImageMinimum_U16_3(res32, img);
			break;
		}

		break;

	case MLIB_INT:

		switch (channels) {
		case 1:
		case 2:
		case 4:
			mlib_c_ImageMinimum_S32_124(res32, img);
			break;
		case 3:
			mlib_c_ImageMinimum_S32_3(res32, img);
			break;
		}

		break;

	default:
		return (MLIB_FAILURE);
	}

	for (j = 0; j < channels; j++)
		res[j] = res32[j];

	return (MLIB_SUCCESS);
}

/*  CONVERT  */

#define	MLIB_M_CONVERT_8U8_4S16(_4s16_1, _4s16_2, _8u8)         \
	{                                                       \
	    _4s16_1 = _mm_unpackhi_pi8(_8u8, _m_zero);          \
	    _4s16_2 = _mm_unpacklo_pi8(_8u8, _m_zero);          \
	}

/* *********************************************************** */
#define	MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, _4s16)               \
	{                                                               \
	    _2s32_1 =                                                   \
		_mm_srai_pi32(_mm_unpackhi_pi16(_m_zero, _4s16), 16);   \
	    _2s32_2 =                                                   \
		_mm_srai_pi32(_mm_unpacklo_pi16(_m_zero, _4s16), 16);   \
	}

/* *********************************************************** */
#define	MLIB_M_CONVERT_4U16_2S32(_2s32_1, _2s32_2, _4u16)       \
	{                                                       \
	    _2s32_1 = _mm_unpackhi_pi16(_4u16, _m_zero);        \
	    _2s32_2 = _mm_unpacklo_pi16(_4u16, _m_zero);        \
	}

/* *********************************************************** */
#define	MLIB_M_CONVERT_2S32_S32(_s32_1, _s32_2, _2s32)             \
	{                                                          \
	    _s32_1 = _mm_cvtsi64_si32(_2s32);                      \
	    _s32_2 = _mm_cvtsi64_si32(_mm_srli_si64(_2s32, 32));   \
	}

/*  MINIMUM  */

#define	MLIB_M_IMAGE_MINIMUM(dd1, sd1, sd2)                     \
	{                                                       \
	    dd1 = (sd1 < sd2) ? sd1 : sd2;                      \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_U8(dd1, min, sd1)                    \
	{                                                         \
	    __m64 cmask =                                         \
		_mm_cmpeq_pi8(_mm_subs_pu8(sd1, min), _m_zero);   \
	    dd1 =                                                 \
		_mm_or_si64(_mm_and_si64(sd1, cmask),             \
		_mm_andnot_si64(cmask, min));                     \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_S16(dd1, min, sd1)                 \
	{                                                       \
	    __m64 cmask = _mm_cmpgt_pi16(min, sd1);             \
	                                                        \
	    dd1 =                                               \
		_mm_or_si64(_mm_and_si64(sd1, cmask),           \
		_mm_andnot_si64(cmask, min));                   \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_U16(dd1, min, sd1)                     \
	{                                                           \
	    __m64 cmask =                                           \
		_mm_cmpeq_pi16(_mm_subs_pu16(sd1, min), _m_zero);   \
	    dd1 =                                                   \
		_mm_or_si64(_mm_and_si64(sd1, cmask),               \
		_mm_andnot_si64(cmask, min));                       \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_S32(dd1, min, sd1)                 \
	{                                                       \
	    __m64 cmask = _mm_cmpgt_pi32(min, sd1);             \
	                                                        \
	    dd1 =                                               \
		_mm_or_si64(_mm_and_si64(sd1, cmask),           \
		_mm_andnot_si64(cmask, min));                   \
	}

/*  PROLOG MINIMUM  */

#define	MLIB_M_IMAGE_MINIMUM_U8_M32(dd1, min, sd1, emask)            \
	{                                                            \
	    __m64 cmask =                                            \
		_mm_cmpeq_pi8(_mm_subs_pu8(sd1, min), _m_zero);      \
	    cmask = _mm_and_si64(cmask, mask_pst_8[emask & 0xFF]);   \
	    dd1 =                                                    \
		_mm_or_si64(_mm_and_si64(sd1, cmask),                \
		_mm_andnot_si64(cmask, min));                        \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_U8_M64(dd1, min, sd1, emask)         \
	{                                                         \
	    __m64 cmask =                                         \
		_mm_cmpeq_pi8(_mm_subs_pu8(sd1, min), _m_zero);   \
	    cmask = _mm_and_si64(cmask, emask);                   \
	    dd1 =                                                 \
		_mm_or_si64(_mm_and_si64(sd1, cmask),             \
		_mm_andnot_si64(cmask, min));                     \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_S16_M32(dd1, min, sd1, emask)           \
	{                                                            \
	    __m64 cmask = _mm_cmpgt_pi16(min, sd1);                  \
	                                                             \
	    cmask = _mm_and_si64(cmask, mask_pst_16[emask & 0xF]);   \
	    dd1 =                                                    \
		_mm_or_si64(_mm_and_si64(sd1, cmask),                \
		_mm_andnot_si64(cmask, min));                        \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_U16_M32(dd1, min, sd1, emask)           \
	{                                                            \
	    __m64 cmask =                                            \
		_mm_cmpeq_pi16(_mm_subs_pu16(sd1, min), _m_zero);    \
	    cmask = _mm_and_si64(cmask, mask_pst_16[emask & 0xF]);   \
	    dd1 =                                                    \
		_mm_or_si64(_mm_and_si64(sd1, cmask),                \
		_mm_andnot_si64(cmask, min));                        \
	}

/* *********************************************************** */
#define	MLIB_M_IMAGE_MINIMUM_S32_M32(dd1, min, sd1, emask)           \
	{                                                            \
	    __m64 cmask = _mm_cmpgt_pi32(min, sd1);                  \
	                                                             \
	    cmask = _mm_and_si64(cmask, mask_pst_32[emask & 0x3]);   \
	    dd1 =                                                    \
		_mm_or_si64(_mm_and_si64(sd1, cmask),                \
		_mm_andnot_si64(cmask, min));                        \
	}

/* *********************************************************** */

void
mlib_m_ImageMinimum_U8_124(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min;

	__m64 _4s16_1, _4s16_2;
	__m64 _2s32_1, _2s32_2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * channels;

	mlib_s32 s1, s2;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

/* min values */
	min = _mm_set1_pi8(MLIB_U8_MAX);

	for (; height > 0; height--) {

		n1 = width;

		for (; n1 > 7; n1 -= 8) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U8(min, min, sd);
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd = *sp;
			MLIB_M_IMAGE_MINIMUM_U8_M32(min, min, sd, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	switch (channels) {
	case 1:
	    {
		    MLIB_M_CONVERT_8U8_4S16(_4s16_1, _4s16_2, min);
		    MLIB_M_IMAGE_MINIMUM_S16(_4s16_1, _4s16_1, _4s16_2);
		    MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, _4s16_1);
		    MLIB_M_IMAGE_MINIMUM_S32(_2s32_1, _2s32_1, _2s32_2);
		    MLIB_M_CONVERT_2S32_S32(s1, s2, _2s32_1);
		    MLIB_M_IMAGE_MINIMUM(res32[0], s1, s2);
		    break;
	    }

	case 2:
	    {
		    MLIB_M_CONVERT_8U8_4S16(_4s16_1, _4s16_2, min);
		    MLIB_M_IMAGE_MINIMUM_S16(_4s16_1, _4s16_1, _4s16_2);
		    MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, _4s16_1);
		    MLIB_M_IMAGE_MINIMUM_S32(_2s32_1, _2s32_1, _2s32_2);
		    ((__m64 *) res32)[0] = _2s32_1;
		    break;
	    }

	case 4:
	    {
		    MLIB_M_CONVERT_8U8_4S16(_4s16_1, _4s16_2, min);
		    MLIB_M_IMAGE_MINIMUM_S16(_4s16_1, _4s16_1, _4s16_2);
		    MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, _4s16_1);
		    ((__m64 *) res32)[0] = _2s32_2;
		    ((__m64 *) res32)[1] = _2s32_1;
		    break;
	    }
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageMinimum_U8_3(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min0, min1, min2, min3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * 3;

	mlib_u8 *dend;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

	min1 = _mm_set1_pi8(MLIB_U8_MAX);
	min2 = _mm_set1_pi8(MLIB_U8_MAX);
	min3 = _mm_set1_pi8(MLIB_U8_MAX);

	for (; height > 0; height--) {

		n1 = width;
		dend = (mlib_u8 *)sp + width;

		for (; n1 > 23; n1 -= 24) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U8(min1, min1, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U8(min2, min2, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U8(min3, min3, sd);
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U8_M32(min1, min1, sd, emask);

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)sp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_MINIMUM_U8_M32(min2, min2, sd,
				    emask);

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)sp);
				if (n1 > 0) {
					emask = (0xFF << (8 - n1));
					sd = *sp;
					MLIB_M_IMAGE_MINIMUM_U8_M32(min3, min3,
					    sd, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	MLIB_M_IMAGE_MINIMUM_U8_M64(min0, min1, _mm_srli_si64(min2, 8),
	    mmx_write_64(0x00ffffffffffffffll));
	MLIB_M_IMAGE_MINIMUM_U8_M64(min0, min0, _mm_slli_si64(min2, 16),
	    mmx_write_64(0x0000000000ff0000ll));
	MLIB_M_IMAGE_MINIMUM_U8_M64(min0, min0, _mm_srli_si64(min3, 16),
	    mmx_write_64(0x0000ffffffffffffll));
	MLIB_M_IMAGE_MINIMUM_U8_M64(min0, min0, _mm_slli_si64(min3, 8),
	    mmx_write_64(0x0000000000ffff00ll));
	MLIB_M_IMAGE_MINIMUM_U8_M64(min0, min0, _mm_srli_si64(min0, 24),
	    mmx_write_64(0x000000ffff000000ll));
	MLIB_M_IMAGE_MINIMUM_U8_M64(min0, min0, _mm_srli_si64(min0, 24),
	    mmx_write_64(0x0000000000ffffffll));

	res32[0] = _mm_cvtsi64_si32(_mm_and_si64(min0,
				mmx_write_64(0x00000000000000ffll)));
	res32[1] =
	    _mm_cvtsi64_si32(_mm_and_si64(_mm_srli_si64(min0, 8),
	    mmx_write_64(0x00000000000000ffll)));
	res32[2] =
	    _mm_cvtsi64_si32(_mm_and_si64(_mm_srli_si64(min0, 16),
	    mmx_write_64(0x00000000000000ffll)));

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageMinimum_S16_124(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min;

	__m64 _2s32_1, _2s32_2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * channels;

	mlib_s32 s1, s2;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

/* min values */
	min = _mm_set1_pi16(MLIB_S16_MAX);

	for (; height > 0; height--) {

		n1 = width;

		for (; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S16(min, min, sd);
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd = *sp;
			MLIB_M_IMAGE_MINIMUM_S16_M32(min, min, sd, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	switch (channels) {
	case 1:
	    {
		    MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, min);
		    MLIB_M_IMAGE_MINIMUM_S32(_2s32_1, _2s32_1, _2s32_2);
		    MLIB_M_CONVERT_2S32_S32(s1, s2, _2s32_1);
		    MLIB_M_IMAGE_MINIMUM(res32[0], s1, s2);
		    break;
	    }

	case 2:
	    {
		    MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, min);
		    MLIB_M_IMAGE_MINIMUM_S32(_2s32_1, _2s32_1, _2s32_2);
		    ((__m64 *) res32)[0] = _2s32_1;
		    break;
	    }

	case 4:
	    {
		    MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, min);
		    ((__m64 *) res32)[0] = _2s32_2;
		    ((__m64 *) res32)[1] = _2s32_1;
		    break;
	    }
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageMinimum_S16_3(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min1, min2, min3;

	__m64 _2s32_1, _2s32_2, _2s32_3, _2s32_4, _2s32_5, _2s32_6;
	mlib_s32 s1, s2, s3, s4, s5, s6;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * 3;

	mlib_s16 *dend;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

	min1 = _mm_set1_pi16(MLIB_S16_MAX);
	min2 = _mm_set1_pi16(MLIB_S16_MAX);
	min3 = _mm_set1_pi16(MLIB_S16_MAX);

	for (; height > 0; height--) {

		n1 = width;
		dend = (mlib_s16 *)sp + width;

		for (; n1 > 11; n1 -= 12) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S16(min1, min1, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S16(min2, min2, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S16(min3, min3, sd);
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S16_M32(min1, min1, sd, emask);

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)sp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_MINIMUM_S16_M32(min2, min2, sd,
				    emask);

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)sp);
				if (n1 > 0) {
					emask = (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_MINIMUM_S16_M32(min3, min3,
					    sd, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, min1);
	MLIB_M_CONVERT_4S16_2S32(_2s32_3, _2s32_4, min2);
	MLIB_M_CONVERT_4S16_2S32(_2s32_5, _2s32_6, min3);

	MLIB_M_IMAGE_MINIMUM_S32(min1, _2s32_1, _2s32_6);
	MLIB_M_IMAGE_MINIMUM_S32(min2, _2s32_2, _2s32_3);
	MLIB_M_IMAGE_MINIMUM_S32(min3, _2s32_4, _2s32_5);

	MLIB_M_CONVERT_2S32_S32(s1, s2, min1);
	MLIB_M_CONVERT_2S32_S32(s3, s4, min2);
	MLIB_M_CONVERT_2S32_S32(s5, s6, min3);

	MLIB_M_IMAGE_MINIMUM(res32[0], s2, s3);
	MLIB_M_IMAGE_MINIMUM(res32[1], s4, s5);
	MLIB_M_IMAGE_MINIMUM(res32[2], s1, s6);

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageMinimum_U16_124(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min;

	__m64 _2s32_1, _2s32_2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * channels;

	mlib_s32 s1, s2;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

/* min values */
	min = _mm_set1_pi16(MLIB_U16_MAX);

	for (; height > 0; height--) {

		n1 = width;

		for (; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U16(min, min, sd);
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd = *sp;
			MLIB_M_IMAGE_MINIMUM_U16_M32(min, min, sd, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	switch (channels) {
	case 1:
	    {
		    MLIB_M_CONVERT_4U16_2S32(_2s32_1, _2s32_2, min);
		    MLIB_M_IMAGE_MINIMUM_S32(_2s32_1, _2s32_1, _2s32_2);
		    MLIB_M_CONVERT_2S32_S32(s1, s2, _2s32_1);
		    MLIB_M_IMAGE_MINIMUM(res32[0], s1, s2);
		    break;
	    }

	case 2:
	    {
		    MLIB_M_CONVERT_4U16_2S32(_2s32_1, _2s32_2, min);
		    MLIB_M_IMAGE_MINIMUM_S32(_2s32_1, _2s32_1, _2s32_2);
		    ((__m64 *) res32)[0] = _2s32_1;
		    break;
	    }

	case 4:
	    {
		    MLIB_M_CONVERT_4U16_2S32(_2s32_1, _2s32_2, min);
		    ((__m64 *) res32)[0] = _2s32_2;
		    ((__m64 *) res32)[1] = _2s32_1;
		    break;
	    }
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageMinimum_U16_3(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min1, min2, min3;

	__m64 _2s32_1, _2s32_2, _2s32_3, _2s32_4, _2s32_5, _2s32_6;
	mlib_s32 s1, s2, s3, s4, s5, s6;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * 3;

	mlib_u16 *dend;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

	min1 = _mm_set1_pi16(MLIB_U16_MAX);
	min2 = _mm_set1_pi16(MLIB_U16_MAX);
	min3 = _mm_set1_pi16(MLIB_U16_MAX);

	for (; height > 0; height--) {

		n1 = width;
		dend = (mlib_u16 *)sp + width;

		for (; n1 > 11; n1 -= 12) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U16(min1, min1, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U16(min2, min2, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U16(min3, min3, sd);
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_U16_M32(min1, min1, sd, emask);

			n1 = ((mlib_u16 *)dend - (mlib_u16 *)sp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_MINIMUM_U16_M32(min2, min2, sd,
				    emask);

				n1 = ((mlib_u16 *)dend - (mlib_u16 *)sp);
				if (n1 > 0) {
					emask = (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_MINIMUM_U16_M32(min3, min3,
					    sd, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	MLIB_M_CONVERT_4U16_2S32(_2s32_1, _2s32_2, min1);
	MLIB_M_CONVERT_4U16_2S32(_2s32_3, _2s32_4, min2);
	MLIB_M_CONVERT_4U16_2S32(_2s32_5, _2s32_6, min3);

	MLIB_M_IMAGE_MINIMUM_S32(min1, _2s32_1, _2s32_6);
	MLIB_M_IMAGE_MINIMUM_S32(min2, _2s32_2, _2s32_3);
	MLIB_M_IMAGE_MINIMUM_S32(min3, _2s32_4, _2s32_5);

	MLIB_M_CONVERT_2S32_S32(s1, s2, min1);
	MLIB_M_CONVERT_2S32_S32(s3, s4, min2);
	MLIB_M_CONVERT_2S32_S32(s5, s6, min3);

	MLIB_M_IMAGE_MINIMUM(res32[0], s2, s3);
	MLIB_M_IMAGE_MINIMUM(res32[1], s4, s5);
	MLIB_M_IMAGE_MINIMUM(res32[2], s1, s6);

	_mm_empty();
}

/*  MMX SLOWER THEN PURE C  */

void
mlib_m_ImageMinimum_S32_124(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min1, min2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * channels;

	mlib_s32 s1, s2, s3, s4;

	mlib_s32 *dend;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

/* min values */
	min1 = _mm_set1_pi32(MLIB_S32_MAX);
/* min values */
	min2 = _mm_set1_pi32(MLIB_S32_MAX);

	for (; height > 0; height--) {

		n1 = width;
		dend = (mlib_s32 *)sp + width;

		for (; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32(min1, min1, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32(min2, min2, sd);
		}

		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32_M32(min1, min1, sd, emask);

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)sp);
			if (n1 > 0) {
				sd = *sp;
				MLIB_M_IMAGE_MINIMUM_S32_M32(min2, min2, sd, 2);
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	switch (channels) {
	case 1:
	    {
		    MLIB_M_IMAGE_MINIMUM_S32(min1, min1, min2);
		    MLIB_M_CONVERT_2S32_S32(s1, s2, min1);
		    MLIB_M_IMAGE_MINIMUM(res32[0], s1, s2);
		    break;
	    }

	case 2:
	    {
		    MLIB_M_IMAGE_MINIMUM_S32(min1, min1, min2);
		    ((__m64 *) res32)[0] = min1;
		    break;
	    }

	case 4:
	    {
		    ((__m64 *) res32)[0] = min1;
		    ((__m64 *) res32)[1] = min2;
		    break;
	    }
	}

	_mm_empty();
}

/*  MMX SLOWER THEN PURE C  */

void
mlib_m_ImageMinimum_S32_3(
    mlib_s32 *res32,
    const mlib_image *img)
{
/* src address */
	__m64 *sp, *sl;

/* src data */
	__m64 sd;

/* min values */
	__m64 min1, min2, min3;

	mlib_s32 s1, s2, s3, s4, s5, s6;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 slb = mlib_ImageGetStride(img);
	mlib_s32 width = mlib_ImageGetWidth(img) * 3;

	mlib_s32 *dend;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	sp = sl = (__m64 *) mlib_ImageGetData(img);

	min1 = _mm_set1_pi32(MLIB_S32_MAX);
	min2 = _mm_set1_pi32(MLIB_S32_MAX);
	min3 = _mm_set1_pi32(MLIB_S32_MAX);

	for (; height > 0; height--) {

		n1 = width;
		dend = (mlib_s32 *)sp + width;

		for (; n1 > 5; n1 -= 6) {
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32(min1, min1, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32(min2, min2, sd);
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32(min3, min3, sd);
		}

		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd = (*sp++);
			MLIB_M_IMAGE_MINIMUM_S32_M32(min1, min1, sd, emask);

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)sp);
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd = (*sp++);
				MLIB_M_IMAGE_MINIMUM_S32_M32(min2, min2, sd,
				    emask);

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)sp);
				if (n1 > 0) {
					sd = *sp;
					MLIB_M_IMAGE_MINIMUM_S32_M32(min3, min3,
					    sd, 2);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
	}

	MLIB_M_CONVERT_2S32_S32(s1, s2, min1);
	MLIB_M_CONVERT_2S32_S32(s3, s4, min2);
	MLIB_M_CONVERT_2S32_S32(s5, s6, min3);

	MLIB_M_IMAGE_MINIMUM(res32[0], s1, s4);
	MLIB_M_IMAGE_MINIMUM(res32[1], s2, s5);
	MLIB_M_IMAGE_MINIMUM(res32[2], s3, s6);

	_mm_empty();
}

/*  PURE C  */

#define	MLIB_C_MINIMUM_S32(res, x)	{ if (x < res) res = x; }

/*  PURE C  */

void
mlib_c_ImageMinimum_S32_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_type type;
	mlib_s32 *sl;
	mlib_s32 channels, width, height, slb;
	mlib_s32 res0, res1, res2, res3;
	mlib_s32 i, j;

	MLIB_IMAGE_GET_ALL_PARAMS(img, type, channels, width, height, slb, sl);
	slb /= 4;
	width *= channels;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	res0 = res1 = res2 = res3 = MLIB_S32_MAX;

	for (j = 0; j < height; j++) {
		for (i = 0; i <= (width - 4); i += 4) {
			MLIB_C_MINIMUM_S32(res0, sl[i + 0]);
			MLIB_C_MINIMUM_S32(res1, sl[i + 1]);
			MLIB_C_MINIMUM_S32(res2, sl[i + 2]);
			MLIB_C_MINIMUM_S32(res3, sl[i + 3]);
		}

		if (i < width)
			MLIB_C_MINIMUM_S32(res0, sl[i]);
		i++;
		if (i < width)
			MLIB_C_MINIMUM_S32(res1, sl[i]);
		i++;
		if (i < width)
			MLIB_C_MINIMUM_S32(res2, sl[i]);

		sl += slb;
	}

	switch (channels) {
	case 1:
	    {
		    MLIB_C_MINIMUM_S32(res0, res1);
		    MLIB_C_MINIMUM_S32(res0, res2);
		    MLIB_C_MINIMUM_S32(res0, res3);
		    res[0] = res0;
		    break;
	    }

	case 2:
	    {
		    MLIB_C_MINIMUM_S32(res0, res2);
		    MLIB_C_MINIMUM_S32(res1, res3);
		    res[0] = res0;
		    res[1] = res1;
		    break;
	    }

	case 4:
	    {
		    res[0] = res0;
		    res[1] = res1;
		    res[2] = res2;
		    res[3] = res3;
	    }

		break;
	}
}

/*  PURE C  */

void
mlib_c_ImageMinimum_S32_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_type type;
	mlib_s32 *sl;
	mlib_s32 channels, width, height, slb;
	mlib_s32 res0, res1, res2;
	mlib_s32 i, j;

	MLIB_IMAGE_GET_ALL_PARAMS(img, type, channels, width, height, slb, sl);
	slb /= 4;
	width *= channels;

	if (slb == width) {
		width *= height;
		height = 1;
	}

	res0 = res1 = res2 = MLIB_S32_MAX;

	for (j = 0; j < height; j++) {
		for (i = 0; i <= (width - 3); i += 3) {
			MLIB_C_MINIMUM_S32(res0, sl[i + 0]);
			MLIB_C_MINIMUM_S32(res1, sl[i + 1]);
			MLIB_C_MINIMUM_S32(res2, sl[i + 2]);
		}

		if (i < width)
			MLIB_C_MINIMUM_S32(res0, sl[i]);
		i++;
		if (i < width)
			MLIB_C_MINIMUM_S32(res1, sl[i]);

		sl += slb;
	}

	res[0] = res0;
	res[1] = res1;
	res[2] = res2;
}

/* *********************************************************** */
