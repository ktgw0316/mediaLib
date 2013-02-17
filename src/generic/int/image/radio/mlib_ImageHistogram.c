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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_ImageHistogram.c	9.2	07/11/05 SMI"

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

#ifdef _MSC_VER

#define	TABLE_SHIFT_S32	((mlib_u32)2147483648)

#else /* _MSC_VER */

#define	TABLE_SHIFT_S32	(2147483648u)

#endif /* _MSC_VER */

/* *********************************************************** */

static void mlib_ImageHistogram_Clear_U8(
    mlib_s32 **histo,
    mlib_s32 nchan);

static void mlib_ImageHistogram_U8_124_D1(
    const mlib_u8 *sl,
    mlib_s32 n,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

static void mlib_ImageHistogram_U8_124(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo);

static void mlib_ImageHistogram_U8_3_D1(
    const mlib_u8 *sl,
    mlib_s32 n,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2);

static void mlib_ImageHistogram_U8_3(
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
#if defined(_NO_LONGLONG)
/* *********************************************************** */

mlib_status
__mlib_ImageHistogram(
    mlib_s32 **histo,
    const mlib_image *img)
{
	mlib_s32 stride, xsize, ysize, nchan, i, k;
	mlib_type type;
	void *src;
	mlib_d64 histo_buff[512];
	mlib_s32 *mhisto0, *mhisto1, *mhisto2, *mhisto3, *mhisto[8];
	mlib_s32 *histo0, *histo1;

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

		if (nchan >= 3) {
			mlib_ImageHistogram_Clear_U8(histo, nchan);
			for (k = 0; k < nchan; k++)
				mhisto[k] = histo[k];
			for (k = 0; k < nchan; k++)
				mhisto[k + nchan] = mhisto[k];
		} else {
			for (i = 0; i < 512; i++)
				histo_buff[i] = 0;
			for (k = 0; k < 4; k++)
				mhisto[k] = (mlib_s32 *)histo_buff + k * 256;
			for (k = 0; k < 4; k++)
				mhisto[k + 4] = mhisto[k];
		}

		if (nchan != 3) {
			mlib_ImageHistogram_U8_124(src, xsize, ysize, stride,
			    mhisto);

			mhisto0 = mhisto[0];
			mhisto1 = mhisto[1];
			mhisto2 = mhisto[2];
			mhisto3 = mhisto[3];

			if (nchan == 1) {
				histo0 = histo[0];
				for (i = 0; i < 256; i++) {
					histo0[i] =
					    mhisto0[i] + mhisto1[i] +
					    mhisto2[i] + mhisto3[i];
				}
			} else {
				if (nchan == 2) {
					histo0 = histo[0];
					histo1 = histo[1];
					for (i = 0; i < 256; i++) {
						histo0[i] =
						    mhisto0[i] + mhisto2[i];
						histo1[i] =
						    mhisto1[i] + mhisto3[i];
					}
				}
			}
		} else {
			mlib_ImageHistogram_U8_3(src, xsize, ysize, stride,
			    mhisto);
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 256 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 254 / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][255] = 0;
		}
	}
}

/* *********************************************************** */
#else
/* *********************************************************** */

mlib_status
__mlib_ImageHistogram(
    mlib_s32 **histo,
    const mlib_image *img)
{
	mlib_s32 stride, xsize, ysize, nchan, i, k;
	mlib_type type;
	void *src;
	mlib_s64 histo_buff[512];
	mlib_s32 *mhisto0, *mhisto1, *mhisto2, *mhisto3, *mhisto[8];
	mlib_s32 *histo0, *histo1;

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

		if (nchan >= 3) {
			mlib_ImageHistogram_Clear_U8(histo, nchan);
			for (k = 0; k < nchan; k++)
				mhisto[k] = histo[k];
			for (k = 0; k < nchan; k++)
				mhisto[k + nchan] = mhisto[k];
		} else {
			for (i = 0; i < 512; i++)
				histo_buff[i] = 0;
			for (k = 0; k < 4; k++)
				mhisto[k] = (mlib_s32 *)histo_buff + k * 256;
			for (k = 0; k < 4; k++)
				mhisto[k + 4] = mhisto[k];
		}

		if (nchan != 3) {
			mlib_ImageHistogram_U8_124(src, xsize, ysize, stride,
			    mhisto);

			mhisto0 = mhisto[0];
			mhisto1 = mhisto[1];
			mhisto2 = mhisto[2];
			mhisto3 = mhisto[3];

			if (nchan == 1) {
				histo0 = histo[0];
				for (i = 0; i < 256; i++) {
					histo0[i] =
					    mhisto0[i] + mhisto1[i] +
					    mhisto2[i] + mhisto3[i];
				}
			} else {
				if (nchan == 2) {
					histo0 = histo[0];
					histo1 = histo[1];
					for (i = 0; i < 256; i++) {
						histo0[i] =
						    mhisto0[i] + mhisto2[i];
						histo1[i] =
						    mhisto1[i] + mhisto3[i];
					}
				}
			}
		} else {
			mlib_ImageHistogram_U8_3(src, xsize, ysize, stride,
			    mhisto);
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

/*
 * clear the U8 histogram
 */

void
mlib_ImageHistogram_Clear_U8(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
	mlib_s64 *dp;
	mlib_s64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_s64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 256 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 254 / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][255] = 0;
		}
	}
}

/* *********************************************************** */
#endif
/* *********************************************************** */

/* *********************************************************** */

void
mlib_ImageHistogram_U8_124_D1(
    const mlib_u8 *sl,
    mlib_s32 n,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	const mlib_u8 *sp = sl, *send = sl + n;
	mlib_u32 *sp4;

	sp4 = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (; (mlib_u8 *)sp4 <= (send - 4); sp4++) {
		mlib_u32 pack_src, pix0, pix1, pix2, pix3;

		pack_src = sp4[0];
#ifdef _LITTLE_ENDIAN
		pix3 = (pack_src >> 24);
		pix2 = (pack_src >> 16) & 0xFF;
		pix1 = (pack_src >> 8) & 0xFF;
		pix0 = pack_src & 0xFF;
#else /* _LITTLE_ENDIAN */
		pix0 = (pack_src >> 24);
		pix1 = (pack_src >> 16) & 0xFF;
		pix2 = (pack_src >> 8) & 0xFF;
		pix3 = pack_src & 0xFF;
#endif /* _LITTLE_ENDIAN */
		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;
	}

	sp = (mlib_u8 *)sp4;

	if (sp < send)
		histo0[sp[0]]++;
	sp++;

	if (sp < send)
		histo1[sp[0]]++;
	sp++;

	if (sp < send)
		histo2[sp[0]]++;
	sp++;
}

/* *********************************************************** */

void
mlib_ImageHistogram_U8_124(
    const mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 stride,
    mlib_s32 **histo)
{
	const mlib_u8 *sp = sl, *send;
	mlib_s32 **p_histo;
	mlib_s32 j, n;

	for (j = 0; j < ysize; j++) {
		send = sp + xsize;
		p_histo = histo;
		while (((mlib_addr)sp & 3) && (sp < send)) {
			p_histo[0][*sp]++;
			p_histo++;
			sp++;
		}

		n = send - sp;
		mlib_ImageHistogram_U8_124_D1(sp, n, p_histo[0], p_histo[1],
		    p_histo[2], p_histo[3]);
		sp = sl += stride;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram_U8_3_D1(
    const mlib_u8 *sl,
    mlib_s32 n,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2)
{
	const mlib_u8 *sp = sl, *send = sl + n;
	mlib_u16 *sp2;

	sp2 = (mlib_u16 *)sp;

	for (; (mlib_u8 *)sp2 <= (send - 6); sp2 += 3) {
		mlib_u32 pix01, pix23, pix45;
		mlib_u32 pix0, pix1, pix2, pix3, pix4, pix5;

		pix01 = sp2[0];
		pix23 = sp2[1];
		pix45 = sp2[2];

#ifdef _LITTLE_ENDIAN
		pix1 = pix01 >> 8;
		pix0 = pix01 & 0xFF;
		pix3 = pix23 >> 8;
		pix2 = pix23 & 0xFF;
		pix5 = pix45 >> 8;
		pix4 = pix45 & 0xFF;
#else /* _LITTLE_ENDIAN */
		pix0 = pix01 >> 8;
		pix1 = pix01 & 0xFF;
		pix2 = pix23 >> 8;
		pix3 = pix23 & 0xFF;
		pix4 = pix45 >> 8;
		pix5 = pix45 & 0xFF;
#endif /* _LITTLE_ENDIAN */
		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo0[pix3]++;
		histo1[pix4]++;
		histo2[pix5]++;
	}

	sp = (mlib_u8 *)sp2;

	if (sp < send)
		histo0[sp[0]]++;
	sp++;

	if (sp < send)
		histo1[sp[0]]++;
	sp++;

	if (sp < send)
		histo2[sp[0]]++;
	sp++;

	if (sp < send)
		histo0[sp[0]]++;
	sp++;

	if (sp < send)
		histo1[sp[0]]++;
	sp++;
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
	mlib_s32 **p_histo;
	mlib_s32 j, n;

	for (j = 0; j < ysize; j++) {
		send = sp + xsize;
		p_histo = histo;

		if (((mlib_addr)sp & 1) && (sp < send)) {
			p_histo[0][*sp]++;
			p_histo++;
			sp++;
		}

		n = send - sp;
		mlib_ImageHistogram_U8_3_D1(sp, n, p_histo[0], p_histo[1],
		    p_histo[2]);
		sp = sl += stride;
	}
}
/* *********************************************************** */
#if defined(_NO_LONGLONG)
/* *********************************************************** */

/*
 * clear the S16 histogram
 */

void
mlib_ImageHistogram_Clear_S16(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
/*
 *  mlib_s32   k;
 *
 *  for (k = 0; k < nchan; k++) {
 *    __mlib_memset(histo[k], 0, 65536 * sizeof (mlib_s32));
 *  }
 *
 */

	mlib_d64 *dp;
	mlib_d64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_d64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 65536 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 65534 / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][65535] = 0;
		}
	}
}

/* *********************************************************** */
#else
/* *********************************************************** */

/*
 * clear the S16 histogram
 */

void
mlib_ImageHistogram_Clear_S16(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
/*
 *  mlib_s32   k;
 *
 *  for (k = 0; k < nchan; k++) {
 *    __mlib_memset(histo[k], 0, 65536 * sizeof (mlib_s32));
 *  }
 *
 */

	mlib_s64 *dp;
	mlib_s64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_s64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 65536 / 2; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 65534 / 2; i++) {
				(*dp++) = zeros;
			}

			histo[k][65535] = 0;
		}
	}
}
/* *********************************************************** */
#endif
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
#if defined(_NO_LONGLONG)
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 0x40000000; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 0x3FFFFFFF; i++) {
				(*dp++) = zeros;
			}

			*((mlib_s32 *)dp) = 0;
		}
	}
}


/* *********************************************************** */
#else
/* *********************************************************** */

/*
 * clear the histogram
 */

void
mlib_ImageHistogram_Clear_S32(
    mlib_s32 **histo,
    mlib_s32 nchan)
{
	mlib_s64 *dp;
	mlib_s64 zeros = 0.;
	mlib_s32 i, k;

	for (k = 0; k < nchan; k++) {
		dp = (mlib_s64 *)((mlib_addr)histo[k] & (~7));

		if (((mlib_addr)histo[k] & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 0x40000000; i++) {
				(*dp++) = zeros;
			}
		} else {
			histo[k][0] = 0;
			dp++;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 0x3FFFFFFF; i++) {
				(*dp++) = zeros;
			}

			*((mlib_s32 *)dp) = 0;
		}
	}
}
/* *********************************************************** */
#endif
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

#ifndef _MSC_VER
	histo += TABLE_SHIFT_S32;
#endif		   /* _MSC_VER */

	for (j = 0; j < ysize; j++) {

		for (i = 0; i < xsize; i++) {
			histo[sl[i * nchan]]++;
		}

		sl += stride;
	}
}

/* *********************************************************** */
