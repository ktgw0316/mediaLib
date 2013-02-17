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

#pragma ident	"@(#)mlib_v_ImageHistogram.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageHistogram - image histogram
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageHistogram(mlib_s32         **histo,
 *                                      const mlib_image *img);
 *
 * ARGUMENT
 *      histo   histogram table
 *      img     pointer to input image
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels,
 *      it can be of MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      The mlib_ImageHistogram function scans an image, counts number of
 *      pixels of each possible value for each channel of the image, and
 *      generates a histogram.
 *
 *      The size of arrays histo[k] for each channel k must be at least
 *        2^8  for MLIB_BYTE  image
 *        2^16 for MLIB_SHORT image
 *        2^16 for MLIB_USHORT image
 *        2^32 for MLIB_INT   image
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageHistogram = __mlib_ImageHistogram

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageHistogram) mlib_ImageHistogram
    __attribute__((weak, alias("__mlib_ImageHistogram")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TABLE_SHIFT_S32	(2147483648u)

/* *********************************************************** */

static void mlib_ImageHistogram_Clear_U8(
    mlib_s32 **histo,
    mlib_s32 nchan);

static void mlib_ImageHistogram_U8_1(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo);

static void mlib_ImageHistogram_U8_2(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo);

static void mlib_ImageHistogram_U8_3(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo);

static void mlib_ImageHistogram_U8_4(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo);

static void mlib_ImageHistogram_Clear_S16(
    mlib_s32 **histo,
    mlib_s32 nchan);

static void mlib_ImageHistogram_S16_1(
    const mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 *histo);

static void mlib_ImageHistogram_S16_1s(
    const mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 stride,
    mlib_s32 *histo);

static void mlib_ImageHistogram_U16_1(
    const mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 *histo);

static void mlib_ImageHistogram_U16_1s(
    const mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 stride,
    mlib_s32 *histo);

static void mlib_ImageHistogram_Clear_S32(
    mlib_s32 **histo,
    mlib_s32 nchan);

static void mlib_ImageHistogram_S32_1s(
    const mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 stride,
    mlib_s32 *histo);

/* *********************************************************** */

/*
 * clear the U8 histogram
 */

void
mlib_ImageHistogram_Clear_U8(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
	mlib_d64 *dp;
	mlib_d64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_d64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < 256 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#pragma pipeloop(0)
			for (i = 0; i < 254 / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][255] = 0;
		}
	}
}

/* *********************************************************** */

#define	UNPACK_4xU8_a(pack_src, pix0, pix1, pix2, pix3)         \
	pix0 = (pack_src >> 22) & 0x3FC;                        \
	pix1 = (pack_src >> 14) & 0x3FC;                        \
	pix2 = (pack_src >> 6) & 0x3FC;                         \
	pix3 = (pack_src << 2) & 0x3FC

/* *********************************************************** */

#define	UNPACK_2xU8_a(pack_src, pix0, pix1)                     \
	pix0 = (pack_src >> 6) & 0x3FC;                         \
	pix1 = (pack_src << 2) & 0x3FC

/* *********************************************************** */

#define	UNPACK_4xU8(pack_src, pix0, pix1, pix2, pix3)           \
	UNPACK_4xU8_a(pack_src, pix0, pix1, pix2, pix3)

/* *********************************************************** */

#define	UNPACK_2xU8(pack_src, pix0, pix1)                       \
	UNPACK_2xU8_a(pack_src, pix0, pix1)

/* *********************************************************** */

void
mlib_ImageHistogram_U8_1(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo)
{
	const mlib_u8 *sp = sl, *send;
	mlib_u32 *sp4;
	mlib_s32 *histo0 = histo[0];
	mlib_s32 j;

	for (j = 0; j < ysize; j++) {
		send = sp + xsize;

		while (((mlib_addr)sp & 3) && (sp < send)) {
			histo0[(*sp++)]++;
		}

		sp4 = (mlib_u32 *)sp;
		for (; (mlib_u8 *)sp4 <= (send - 4); sp4++) {
			mlib_u32 pack_src, pix0, pix1, pix2, pix3;

			pack_src = sp4[0];
			UNPACK_4xU8(pack_src, pix0, pix1, pix2, pix3);
			((mlib_s32 *)((mlib_u8 *)histo0 + pix0))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo0 + pix1))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo0 + pix2))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo0 + pix3))[0]++;
		}

		sp = (mlib_u8 *)sp4;

		if (sp < send)
			histo0[(*sp++)]++;

		if (sp < send)
			histo0[(*sp++)]++;

		if (sp < send)
			histo0[(*sp++)]++;

		sp = sl += stride;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram_U8_2(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo)
{
	const mlib_u8 *sp = sl, *send;
	mlib_u32 *sp4;
	mlib_s32 **p_histo;
	mlib_s32 *histo0;
	mlib_s32 *histo1;
	mlib_s32 j;

	for (j = 0; j < ysize; j++) {
		send = sp + xsize;
		p_histo = histo;

		while (((mlib_addr)sp & 3) && (sp < send)) {
			((*p_histo++))[(*sp++)]++;
		}

		histo0 = p_histo[0];
		histo1 = p_histo[1];

		sp4 = (mlib_u32 *)sp;
		for (; (mlib_u8 *)sp4 <= (send - 4); sp4++) {
			mlib_u32 pack_src, pix0, pix1, pix2, pix3;

			pack_src = sp4[0];
			UNPACK_4xU8(pack_src, pix0, pix1, pix2, pix3);
			((mlib_s32 *)((mlib_u8 *)histo0 + pix0))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo1 + pix1))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo0 + pix2))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo1 + pix3))[0]++;
		}

		sp = (mlib_u8 *)sp4;

		if (sp < send)
			histo0[(*sp++)]++;

		if (sp < send)
			histo1[(*sp++)]++;

		if (sp < send)
			histo0[(*sp++)]++;

		sp = sl += stride;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram_U8_3(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo)
{
	const mlib_u8 *sp = sl, *send;
	mlib_u16 *sp2;
	mlib_s32 **p_histo;
	mlib_s32 *histo0;
	mlib_s32 *histo1;
	mlib_s32 *histo2;
	mlib_s32 j;

	for (j = 0; j < ysize; j++) {
		send = sp + xsize;
		p_histo = histo;

		if ((mlib_addr)sp & 1) {
			((*p_histo++))[(*sp++)]++;
		}

		histo0 = p_histo[0];
		histo1 = p_histo[1];
		histo2 = p_histo[2];

/* first loop */
		sp2 = (mlib_u16 *)sp;
		for (; (mlib_u8 *)sp2 <= (send - 6); sp2 += 3) {
			mlib_s32 s0, s0a, s0b;
			mlib_s32 s1, s1a, s1b;

			s0 = sp2[0];
			s1 = sp2[1];
			UNPACK_2xU8(s0, s0a, s0b);
			UNPACK_2xU8(s1, s1a, s1b);
			((mlib_s32 *)((mlib_u8 *)histo0 + s0a))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo1 + s0b))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo0 + s1b))[0]++;
		}

/* second loop */
		sp2 = (mlib_u16 *)sp;
		for (; (mlib_u8 *)sp2 <= (send - 6); sp2 += 3) {
			mlib_s32 s1, s1a, s1b;
			mlib_s32 s2, s2a, s2b;

			s1 = sp2[1];
			s2 = sp2[2];
			UNPACK_2xU8(s1, s1a, s1b);
			UNPACK_2xU8(s2, s2a, s2b);
			((mlib_s32 *)((mlib_u8 *)histo2 + s1a))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo1 + s2a))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo2 + s2b))[0]++;
		}

		sp = (mlib_u8 *)sp2;

		if (sp < send)
			histo0[(*sp++)]++;

		if (sp < send)
			histo1[(*sp++)]++;

		if (sp < send)
			histo2[(*sp++)]++;

		if (sp < send)
			histo0[(*sp++)]++;

		if (sp < send)
			histo1[(*sp++)]++;

		sp = sl += stride;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram_U8_4(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo)
{
	const mlib_u8 *sp = sl, *send;
	mlib_u32 *sp4;
	mlib_s32 **p_histo;
	mlib_s32 *histo0;
	mlib_s32 *histo1;
	mlib_s32 *histo2;
	mlib_s32 *histo3;
	mlib_s32 j;

	for (j = 0; j < ysize; j++) {
		send = sp + xsize;
		p_histo = histo;

		while (((mlib_addr)sp & 3) && (sp < send)) {
			((*p_histo++))[(*sp++)]++;
		}

		histo0 = p_histo[0];
		histo1 = p_histo[1];
		histo2 = p_histo[2];
		histo3 = p_histo[3];

/* first loop */
		sp4 = (mlib_u32 *)sp;
#pragma nomemorydepend
		for (; (mlib_u8 *)sp4 <= (send - 4); sp4++) {
			mlib_u32 pack_src, pix0, pix1, pix2, pix3;

			pack_src = sp4[0];
			UNPACK_4xU8(pack_src, pix0, pix1, pix2, pix3);
			((mlib_s32 *)((mlib_u8 *)histo0 + pix0))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo1 + pix1))[0]++;
		}

/* second loop */
		sp4 = (mlib_u32 *)sp;
#pragma nomemorydepend
		for (; (mlib_u8 *)sp4 <= (send - 4); sp4++) {
			mlib_u32 pack_src, pix0, pix1, pix2, pix3;

			pack_src = sp4[0];
			UNPACK_4xU8(pack_src, pix0, pix1, pix2, pix3);
			((mlib_s32 *)((mlib_u8 *)histo2 + pix2))[0]++;
			((mlib_s32 *)((mlib_u8 *)histo3 + pix3))[0]++;
		}

		sp = (mlib_u8 *)sp4;

		if (sp < send)
			histo0[(*sp++)]++;

		if (sp < send)
			histo1[(*sp++)]++;

		if (sp < send)
			histo2[(*sp++)]++;

		sp = sl += stride;
	}
}

/* *********************************************************** */

/*
 * clear the S16 histogram
 */

void
mlib_ImageHistogram_Clear_S16(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
	mlib_d64 *dp;
	mlib_d64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_d64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < 65536 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#pragma pipeloop(0)
			for (i = 0; i < 65534 / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][65535] = 0;
		}
	}
}

/* *********************************************************** */

/* the histogram for S16 image is from -32768 to 0 to 32767    */

/* *********************************************************** */

void
mlib_ImageHistogram_S16_1(
    const mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 *histo)
{
	mlib_u32 *sp;
	mlib_s32 i, j, n;

	for (j = 0; j < ysize; j++) {

		n = xsize;

		if ((mlib_addr)sl & 3) {
			histo[sl[0] ^ 0x8000]++;
			sp = (mlib_u32 *)(sl + 1);
			n--;
		} else {
			sp = (mlib_u32 *)sl;
		}

		for (i = 0; i < n / 2; i++) {
			mlib_u32 pix01, pix0, pix1;

			pix01 = sp[i] ^ 0x80008000u;

#ifdef _LITTLE_ENDIAN
			pix1 = pix01 >> 16;
			pix0 = pix01 & 0xFFFF;
#else /* _LITTLE_ENDIAN */
			pix0 = pix01 >> 16;
			pix1 = pix01 & 0xFFFF;
#endif /* _LITTLE_ENDIAN */

			histo[pix0]++;
			histo[pix1]++;
		}

		if (n & 1)
			histo[((mlib_u16 *)sp)[n - 1] ^ 0x8000]++;

		sl += stride;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram_S16_1s(
    const mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 stride,
    mlib_s32 *histo)
{
	mlib_s32 i, j;

	histo += 0x8000;

	for (j = 0; j < ysize; j++) {

		for (i = 0; i < xsize; i++) {
			histo[sl[i * nchan]]++;
		}

		sl += stride;
	}
}

/* *********************************************************** */

/*		the histogram for U16 image			*/

/* *********************************************************** */

void
mlib_ImageHistogram_U16_1(
    const mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 *histo)
{
	mlib_u32 *sp;
	mlib_s32 i, j, n;

	for (j = 0; j < ysize; j++) {

		n = xsize;

		if ((mlib_addr)sl & 3) {
			histo[sl[0]]++;
			sp = (mlib_u32 *)(sl + 1);
			n--;
		} else {
			sp = (mlib_u32 *)sl;
		}

		for (i = 0; i < n / 2; i++) {
			mlib_u32 pix01, pix0, pix1;

			pix01 = sp[i];

#ifdef _LITTLE_ENDIAN
			pix1 = pix01 >> 16;
			pix0 = pix01 & 0xFFFF;
#else /* _LITTLE_ENDIAN */
			pix0 = pix01 >> 16;
			pix1 = pix01 & 0xFFFF;
#endif /* _LITTLE_ENDIAN */

			histo[pix0]++;
			histo[pix1]++;
		}

		if (n & 1)
			histo[((mlib_u16 *)sp)[n - 1]]++;

		sl += stride;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram_U16_1s(
    const mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 stride,
    mlib_s32 *histo)
{
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++) {

		for (i = 0; i < xsize; i++) {
			histo[sl[i * nchan]]++;
		}

		sl += stride;
	}
}

/* *********************************************************** */

/*
 * clear the histogram
 */

void
mlib_ImageHistogram_Clear_S32(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
	mlib_d64 *dp;
	mlib_d64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_d64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < 0x80000000 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#pragma pipeloop(0)
			for (i = 0; i < 0x7FFFFFFE / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][2 * TABLE_SHIFT_S32 - 1] = 0;
		}
	}
}

/* *********************************************************** */

/* the histogram for mlib_s32 image is from -0x80000000 to 0 to 0x7FFFFFFF */

void
mlib_ImageHistogram_S32_1s(
    const mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 stride,
    mlib_s32 *histo)
{
	mlib_s32 i, j;

	histo += TABLE_SHIFT_S32;

	for (j = 0; j < ysize; j++) {

		for (i = 0; i < xsize; i++) {
			histo[sl[i * nchan]]++;
		}

		sl += stride;
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageHistogram(
    mlib_s32 **histo,
    const mlib_image *img)
{
	mlib_s32 stride, xsize, ysize, nchan, k;
	mlib_type type;
	void *src;
	mlib_s32 *mhisto[8];

	MLIB_IMAGE_CHECK(img);

	if (histo == NULL) {
		return (MLIB_NULLPOINTER);
	}

	type = mlib_ImageGetType(img);
	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	stride = mlib_ImageGetStride(img);
	src = mlib_ImageGetData(img);

	if (nchan > 4)
		return (MLIB_FAILURE);

	if (type == MLIB_BYTE) {

		xsize *= nchan;

		if (stride == xsize) {
			xsize *= ysize;
			ysize = 1;
		}

		mlib_ImageHistogram_Clear_U8(histo, nchan);
		for (k = 0; k < nchan; k++)
			mhisto[k] = histo[k];
		for (k = nchan; k < 8; k++)
			mhisto[k] = mhisto[k - nchan];

		switch (nchan) {
		case 1:
			mlib_ImageHistogram_U8_1(src, xsize, ysize, stride,
			    mhisto);
			break;

		case 2:
			mlib_ImageHistogram_U8_2(src, xsize, ysize, stride,
			    mhisto);
			break;

		case 3:
			mlib_ImageHistogram_U8_3(src, xsize, ysize, stride,
			    mhisto);
			break;

		case 4:
			mlib_ImageHistogram_U8_4(src, xsize, ysize, stride,
			    mhisto);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (type == MLIB_SHORT) {

		mlib_ImageHistogram_Clear_S16(histo, nchan);

		stride >>= 1;

		if (stride == xsize * nchan) {
			xsize *= ysize;
			ysize = 1;
		}

		if (nchan == 1) {
			mlib_ImageHistogram_S16_1(src, xsize, ysize, stride,
			    histo[0]);
		} else {
			for (k = 0; k < nchan; k++) {
				mlib_ImageHistogram_S16_1s((mlib_s16 *)src + k,
				    xsize, ysize, nchan, stride, histo[k]);
			}
		}

		return (MLIB_SUCCESS);
	} else if (type == MLIB_USHORT) {

		mlib_ImageHistogram_Clear_S16(histo, nchan);

		stride >>= 1;

		if (stride == xsize * nchan) {
			xsize *= ysize;
			ysize = 1;
		}

		if (nchan == 1) {
			mlib_ImageHistogram_U16_1(src, xsize, ysize, stride,
			    histo[0]);
		} else {
			for (k = 0; k < nchan; k++) {
				mlib_ImageHistogram_U16_1s((mlib_u16 *)src + k,
				    xsize, ysize, nchan, stride, histo[k]);
			}
		}

		return (MLIB_SUCCESS);
	} else if (type == MLIB_INT) {

		stride >>= 2;

		if (stride == xsize * nchan) {
			xsize *= ysize;
			ysize = 1;
		}

		for (k = 0; k < nchan; k++) {
			mlib_ImageHistogram_S32_1s((mlib_s32 *)src + k, xsize,
			    ysize, nchan, stride, histo[k]);
		}

		return (MLIB_SUCCESS);
	} else {

		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
