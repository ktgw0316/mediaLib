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

#pragma ident   "@(#)mlib_s_ImageReplaceColor.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageReplaceColor - replace a color in the image with
 *                               another color.
 *      mlib_ImageReplaceColor_Inp - replace a color in the image with
 *                                   another color.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageReplaceColor(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         const mlib_s32   *color1,
 *                                         const mlib_s32   *color2);
 *      mlib_status mlib_ImageReplaceColor_Inp(mlib_image     *srcdst,
 *                                             const mlib_s32 *color1,
 *                                             const mlib_s32 *color2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      srcdst  pointer to input and output image
 *      color1  array of color to find in the source image
 *      color2  array of color to replace color1 with
 *
 * RESTRICTION
 *      src and dst must be of the same type, the same size and
 *      the same number of channels. They can have one through four
 *      channels. They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type. The length of color array must
 *      be not less than the number of channels in the images.
 *
 *      srcdst can has one through four channels. It can be in
 *      MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      The length of color array must be not less than the number
 *      of channels in the image.
 *
 * DESCRIPTION
 *      Function scans the source image for all pixels with color value
 *      equal to color1 and replaces these pixels with color2.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageReplaceColor = __mlib_ImageReplaceColor
#pragma weak mlib_ImageReplaceColor_Inp = __mlib_ImageReplaceColor_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageReplaceColor) mlib_ImageReplaceColor
    __attribute__((weak, alias("__mlib_ImageReplaceColor")));
__typeof__(__mlib_ImageReplaceColor_Inp) mlib_ImageReplaceColor_Inp
    __attribute__((weak, alias("__mlib_ImageReplaceColor_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(align(16))
#elif __SUNPRO_C
#pragma align 16(mlib_mask128i_arr)
#endif /* defined(__ICL) || defined (__INTEL_COMPILER) */
unsigned int mlib_mask128i_arr[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x000000FF, 0x00000000, 0x00000000, 0x00000000,
    0x0000FFFF, 0x00000000, 0x00000000, 0x00000000,
    0x00FFFFFF, 0x00000000, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x000000FF, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x0000FFFF, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x00FFFFFF, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

/* *********************************************************** */

#define	SSE2_SCMP_8	    _mm_cmpeq_epi8
#define	SSE2_SCMP_16	_mm_cmpeq_epi16
#define	SSE2_SCMP_32	_mm_cmpeq_epi32

/* *********************************************************** */


#define	LOAD_CONST(N, off)                                     \
	c##N = _mm_load_si128(cnst + off);                         \
	r##N = _mm_load_si128(cnst + off + 3)

/* *********************************************************** */

#define	OFF_SET (width & 15)

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_s_ImageReplaceColor_U8##s

#define	DTYPE	mlib_u8

/* *********************************************************** */

#undef COMPARE_CH
#define	COMPARE_CH(N) cmask = SSE2_SCMP_8(s##N, c##N)

#define	PROCESS_CH(N)                                           \
	COMPARE_CH(N);                                              \
	REPLACE_STORE(dp, cmask, s##N, r##N)


/* *********************************************************** */

#include <mlib_s_ImageReplaceColor.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_s_ImageReplaceColor_S16##s

#define	DTYPE	mlib_s16

/* *********************************************************** */

#undef COMPARE_CH
#define	COMPARE_CH(N) cmask = SSE2_SCMP_16(s##N, c##N)

#define	PROCESS_CH(N)                                           \
	COMPARE_CH(N);                                              \
	REPLACE_STORE(dp, cmask, s##N, r##N)

/* *********************************************************** */

#include <mlib_s_ImageReplaceColor.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_s_ImageReplaceColor_S32##s

#define	DTYPE	mlib_s32

/* *********************************************************** */
#undef COMPARE_CH
#define	COMPARE_CH(N)  cmask = SSE2_SCMP_32(s##N, c##N)

#define	PROCESS_CH(N)                                           \
	COMPARE_CH(N);                                              \
	REPLACE_STORE(dp, cmask, s##N, r##N)

/* *********************************************************** */

#include <mlib_s_ImageReplaceColor.h>

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *color1,
    const mlib_s32 *color2)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;

	__m128i cnst[6];
	mlib_s32 i, t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color1 == NULL)
		return (MLIB_NULLPOINTER);

	if (color2 == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(
		dst, type, nchan, width,
		height, dstride, ddata);

	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if (type == MLIB_BYTE) {
		mlib_u8 *pcol1 = (void *)cnst;
		mlib_u8 *pcol2 = (void *)(cnst + 3);

		t_sh = 0;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U8(color1[i]);
			pcol2[i] = color2[i];

			if (pcol1[i] != color1[i])
				pcol2[i] = pcol1[i];
		}

		for (i = nchan; i < 48; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	} else if (type == MLIB_SHORT) {
		mlib_s16 *pcol1 = (void *)cnst;
		mlib_s16 *pcol2 = (void *)(cnst + 3);

		t_sh = 1;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_S16(color1[i]);
			pcol2[i] = color2[i];

			if (pcol1[i] != color1[i])
				pcol2[i] = pcol1[i];
		}

		for (i = nchan; i < 24; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	} else if (type == MLIB_USHORT) {
		mlib_u16 *pcol1 = (void *)cnst;
		mlib_u16 *pcol2 = (void *)(cnst + 3);

		t_sh = 1;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U16(color1[i]);
			pcol2[i] = color2[i];

			if (pcol1[i] != color1[i])
				pcol2[i] = pcol1[i];
		}

		for (i = nchan; i < 24; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	} else {
		mlib_s32 *pcol1 = (void *)cnst;
		mlib_s32 *pcol2 = (void *)(cnst + 3);

		t_sh = 2;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = color1[i];
			pcol2[i] = color2[i];
		}

		for (i = nchan; i < 12; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	}

	if (sstride == dstride && sstride ==
		((nchan * width) << t_sh)) {
		width *= height;
		height = 1;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	switch (type) {
	case MLIB_BYTE:
		if (nchan == 3) {
			return mlib_s_ImageReplaceColor_U8_3(
					sdata, ddata, sstride, dstride,
					width, height, nchan, cnst);
		} else {
			return mlib_s_ImageReplaceColor_U8_124(
					sdata, ddata, sstride, dstride,
					width, height, nchan, cnst);
		}

	case MLIB_SHORT:
	case MLIB_USHORT:

		if (nchan == 3) {
			return mlib_s_ImageReplaceColor_S16_3(
					sdata, ddata, sstride, dstride,
					width, height, nchan, cnst);
		} else {
			return mlib_s_ImageReplaceColor_S16_124(
					sdata, ddata, sstride, dstride,
					width, height, nchan, cnst);
		}

	case MLIB_INT:

		if (nchan == 3) {
			return mlib_s_ImageReplaceColor_S32_3(
					sdata, ddata, sstride, dstride,
					width, height, nchan, cnst);
		} else {
			return mlib_s_ImageReplaceColor_S32_124(
					sdata, ddata, sstride, dstride,
					width, height, nchan, cnst);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Inp(
    mlib_image *img,
    const mlib_s32 *color1,
    const mlib_s32 *color2)
{
	return (__mlib_ImageReplaceColor(img, img, color1, color2));
}

/* *********************************************************** */
