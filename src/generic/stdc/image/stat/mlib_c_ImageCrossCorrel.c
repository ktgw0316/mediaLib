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

#pragma ident	"@(#)mlib_c_ImageCrossCorrel.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageCrossCorrel - calculates image cross correlation
 *                              for the input images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCrossCorrel(mlib_d64         *correl,
 *                                        const mlib_image *img1,
 *                                        const mlib_image *img2);
 *
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      img1     pointer to the first input image
 *      img2     pointer to the second input image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      c = --- * SUM SUM(x[i][j] * y[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageCorrel.h>
#include <mlib_ImageDivTables.h>
#include <stdio.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCrossCorrel = __mlib_ImageCrossCorrel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCrossCorrel) mlib_ImageCrossCorrel
    __attribute__((weak, alias("__mlib_ImageCrossCorrel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCrossCorrel(
    mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2)
{
	mlib_s32 num_pixel;
	mlib_d64 res[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img1);
	MLIB_IMAGE_CHECK(img2);
	MLIB_IMAGE_SIZE_EQUAL(img1, img2);
	MLIB_IMAGE_TYPE_EQUAL(img1, img2);
	MLIB_IMAGE_CHAN_EQUAL(img1, img2);

	if (correl == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img1) * mlib_ImageGetHeight(img1);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img1)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageCrossCorrel_U8_3(img1, img2, res);
		else
			mlib_c_ImageCrossCorrel_U8_124(img1, img2, res);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageCrossCorrel_U16_3(img1, img2, res);
		else
			mlib_c_ImageCrossCorrel_U16_124(img1, img2, res);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageCrossCorrel_S16_3(img1, img2, res);
		else
			mlib_c_ImageCrossCorrel_S16_124(img1, img2, res);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageCrossCorrel_S32_3(img1, img2, res);
		else
			mlib_c_ImageCrossCorrel_S32_124(img1, img2, res);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img1)) {
	case 1:
		correl[0] = (res[0] + res[1] + res[2] + res[3]) * divider;
		break;

	case 2:
		correl[0] = (res[0] + res[2]) * divider;
		correl[1] = (res[1] + res[3]) * divider;
		break;

	case 4:
		correl[3] = res[3] * divider;
	case 3:
		correl[0] = res[0] * divider;
		correl[1] = res[1] * divider;
		correl[2] = res[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_U8(x, y)	(mlib_U82D64[(x)] * mlib_U82D64[(y)])

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_U8_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_u8 *psrc2;

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* values by channels */
	mlib_d64 d1, d2, d3, d4;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	res[0] = res[1] = res[2] = res[3] = 0.0;

	if (size_row < 4) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
			for (j = 0; j < size_row; j++) {
			    res[j] += point_src1[j] * (mlib_d64)point_src2[j];
			}
			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}
		return;
	}

	c1 = c2 = c3 = c4 = 0.0;

	for (i = 0; i < height; i++) {
		mlib_s32 nalign = (mlib_addr)point_src1 & 3;
		mlib_s32 nbytes = size_row + nalign;
		mlib_s32 *p32 = (mlib_s32 *)(point_src1 - nalign);
		mlib_s32 val = *(p32++);

#ifdef _LITTLE_ENDIAN
		mlib_u32 mask = (mlib_u32)0xffffffff << (nalign << 3);
#else /* _LITTLE_ENDIAN */
		mlib_u32 mask = (mlib_u32)0xffffffff >> (nalign << 3);
#endif /* _LITTLE_ENDIAN */
		val &= mask;
		psrc2 = point_src2 - nalign;

		if (((mlib_addr)psrc2 & ~7) != ((mlib_addr)point_src2 & ~7)) {
#ifdef _LITTLE_ENDIAN
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
			val = *(p32++);
			if (nalign < 3) {
				c3 += d3 * mlib_U82D64[psrc2[2]];
				if (nalign < 2)
					c2 += d2 * mlib_U82D64[psrc2[1]];
			}
			c4 += d4 * mlib_U82D64[psrc2[3]];
			psrc2 += 4;
			nbytes -= 4;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (nbytes - 8); j += 4) {
#ifdef _LITTLE_ENDIAN
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val << 3) & 0x7f8));
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 21) & 0x7f8));
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
			val = *(p32++);
			c1 += d1 * mlib_U82D64[psrc2[j]];
			c2 += d2 * mlib_U82D64[psrc2[j + 1]];
			c3 += d3 * mlib_U82D64[psrc2[j + 2]];
			c4 += d4 * mlib_U82D64[psrc2[j + 3]];
		}

		if (j <= (nbytes - 4)) {
#ifdef _LITTLE_ENDIAN
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val << 3) & 0x7f8));
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 21) & 0x7f8));
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
			c1 += d1 * mlib_U82D64[psrc2[j]];
			c2 += d2 * mlib_U82D64[psrc2[j + 1]];
			c3 += d3 * mlib_U82D64[psrc2[j + 2]];
			c4 += d4 * mlib_U82D64[psrc2[j + 3]];
			j += 4;
			nbytes &= 3;
			if (nbytes)
				val = *p32;
		}

		if (nbytes) {
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (nbytes << 3);
			val = val & ~mask;
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val << 3) & 0x7f8));
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (nbytes << 3);
			val = val & ~mask;
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 21) & 0x7f8));
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
			c1 += d1 * mlib_U82D64[psrc2[j]];
			if (nbytes > 1) {
				c2 += d2 * mlib_U82D64[psrc2[j + 1]];
				if (nbytes > 2) {
					c3 += d3 * mlib_U82D64[psrc2[j + 2]];
				}
			}
		}

		res[3 & (0 - nalign)] += c1;
		res[3 & (1 - nalign)] += c2;
		res[3 & (2 - nalign)] += c3;
		res[3 & (3 - nalign)] += c4;
		c1 = c2 = c3 = c4 = 0.0;

		point_src1 += src1_stride;
		point_src2 += src2_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_U8_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_u8 *psrc1, *psrc2;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		psrc1 = point_src1;
		psrc2 = point_src2;
		j = 0;

		if ((mlib_addr)psrc1 & 1) {
			c1 += MUL_U8(psrc1[j], psrc2[j]);
			c2 += MUL_U8(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_U8(psrc1[j + 2], psrc2[j + 2]);
			j += 3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			mlib_u32 u1, u2, u3;

			u1 = ((mlib_u16 *)(psrc1 + j))[0];
			u2 = ((mlib_u16 *)(psrc1 + j))[1];
			u3 = ((mlib_u16 *)(psrc1 + j))[2];
#ifdef _LITTLE_ENDIAN
			c1 += MUL_U8((u1 & 0xff), psrc2[j]);
			c2 += MUL_U8((u1 >> 8), psrc2[j + 1]);
			c3 += MUL_U8((u2 & 0xff), psrc2[j + 2]);
			c1 += MUL_U8((u2 >> 8), psrc2[j + 3]);
			c2 += MUL_U8((u3 & 0xff), psrc2[j + 4]);
			c3 += MUL_U8((u3 >> 8), psrc2[j + 5]);
#else /* _LITTLE_ENDIAN */
			c1 += MUL_U8((u1 >> 8), psrc2[j]);
			c2 += MUL_U8((u1 & 0xff), psrc2[j + 1]);
			c3 += MUL_U8((u2 >> 8), psrc2[j + 2]);
			c1 += MUL_U8((u2 & 0xff), psrc2[j + 3]);
			c2 += MUL_U8((u3 >> 8), psrc2[j + 4]);
			c3 += MUL_U8((u3 & 0xff), psrc2[j + 5]);
#endif /* _LITTLE_ENDIAN */
		}

		if (j < size_row) {
			c1 += MUL_U8(psrc1[j], psrc2[j]);
			c2 += MUL_U8(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_U8(psrc1[j + 2], psrc2[j + 2]);
		}

		point_src1 += src1_stride;
		point_src2 += src2_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
}

/* *********************************************************** */

#define	MUL_S16(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_U16_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;

#ifdef _LITTLE_ENDIAN
	mlib_s32 k;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0;
	res[0] = res[1] = res[2] = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_S16(psrc1[j], psrc2[j]);
			c2 += MUL_S16(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S16(psrc1[j + 2], psrc2[j + 2]);
			c4 += MUL_S16(psrc1[j + 3], psrc2[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			res[k] += MUL_S16(psrc1[j], psrc2[j]);
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] += c1;
	res[1] += c2;
	res[2] += c3;
	res[3] = c4;

#else /* _LITTLE_ENDIAN */

	mlib_s32 a1, a2, a3, a4;
	mlib_s32 align;
	union
	{
		mlib_d64 db;
		mlib_s32 s[2];
		mlib_u16 u[4];
	} b[4],
	    r1;

	b[0].db = b[1].db = b[2].db = b[3].db = 0.0;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0;
	res[0] = res[1] = res[2] = res[3] = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc1) {
			a1 = psrc1[0];
			a2 = psrc2[0];
			c4 = a1 * (mlib_d64)a2;
			j = 1;
		}

		align = j;

/* same alignment */
		if (!(2 & (mlib_addr)(psrc2 + j))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 4); j += 4) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a3 = ((mlib_s32 *)(psrc2 + j))[0];
				a4 = ((mlib_s32 *)(psrc2 + j))[1];

				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[0].u[3] = (mlib_u32)a3 >> 16;
				b[1].u[1] = a1;
				b[1].u[3] = a3;
				b[2].u[1] = (mlib_u32)a2 >> 16;
				b[2].u[3] = (mlib_u32)a4 >> 16;
				b[3].u[1] = a2;
				b[3].u[3] = a4;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[3].db;
				c4 += (mlib_d64)r1.s[0] * r1.s[1];
			}
		} else {
			a4 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 4); j += 4) {
				b[0].u[3] = a4;
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
				a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];

				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[1].u[1] = a1;
				b[1].u[3] = (mlib_u32)a3 >> 16;
				b[2].u[1] = (mlib_u32)a2 >> 16;
				b[2].u[3] = a3;
				b[3].u[1] = a2;
				b[3].u[3] = (mlib_u32)a4 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[3].db;
				c4 += (mlib_d64)r1.s[0] * r1.s[1];
			}
		}

		if (j < size_row) {
			a1 = ((mlib_s32 *)(psrc1 + j))[0];
			b[0].u[1] = (mlib_u32)a1 >> 16;
			b[0].u[3] = psrc2[j];
			r1.db = b[0].db;
			c1 += (mlib_d64)r1.s[0] * r1.s[1];
			j++;

			if (j < size_row) {
				b[1].u[1] = a1;
				b[1].u[3] = psrc2[j];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				j++;

				if (j < size_row) {
					a1 = psrc1[j];
					a2 = psrc2[j];
					c3 += (mlib_d64)a1 *a2;
				}
			}
		}

		res[align] += c1;
		res[1 + align] += c2;
		res[2 + align] += c3;
		res[3 & (3 + align)] += c4;
		c1 = c2 = c3 = c4 = 0.0;
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_U16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

#ifdef _LITTLE_ENDIAN

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < size_row; j += 3) {
			c1 += MUL_S16(psrc1[j], psrc2[j]);
			c2 += MUL_S16(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S16(psrc1[j + 2], psrc2[j + 2]);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
#else /* _LITTLE_ENDIAN */
	mlib_s32 a1, a2, a3, a4, a5, a6;
	union
	{
		mlib_d64 db;
		mlib_s32 s[2];
		mlib_u16 u[4];
	} b[3],
	    r1;

	b[0].db = b[1].db = b[2].db = 0.0;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc1) {
			a1 = psrc1[0];
			a2 = psrc2[0];
			a3 = psrc1[1];
			a4 = psrc2[1];
			a5 = psrc1[2];
			a6 = psrc2[2];
			c1 += a1 * (mlib_d64)a2;
			c2 += a3 * (mlib_d64)a4;
			c3 += a5 * (mlib_d64)a6;
			j = 3;
			if (j >= size_row) {
				psrc1 += src1_stride;
				psrc2 += src2_stride;
				continue;
			}
		}
/* same alignment */
		if (!(2 & (mlib_addr)(psrc2 + j))) {
			a1 = ((mlib_s32 *)(psrc1 + j))[0];
			a3 = ((mlib_s32 *)(psrc2 + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 12); j += 6) {
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a4 = ((mlib_s32 *)(psrc2 + j))[1];
				a5 = ((mlib_s32 *)(psrc1 + j))[2];
				a6 = ((mlib_s32 *)(psrc2 + j))[2];

				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[0].u[3] = (mlib_u32)a3 >> 16;
				b[1].u[1] = a1;
				b[1].u[3] = a3;
				b[2].u[1] = (mlib_u32)a2 >> 16;
				b[2].u[3] = (mlib_u32)a4 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				b[0].u[1] = a2;
				b[0].u[3] = a4;
				b[1].u[1] = (mlib_u32)a5 >> 16;
				b[1].u[3] = (mlib_u32)a6 >> 16;
				b[2].u[1] = a5;
				b[2].u[3] = a6;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				a1 = ((mlib_s32 *)(psrc1 + j))[3];
				a3 = ((mlib_s32 *)(psrc2 + j))[3];
			}

			if (j <= (size_row - 6)) {
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a4 = ((mlib_s32 *)(psrc2 + j))[1];
				a5 = ((mlib_s32 *)(psrc1 + j))[2];
				a6 = ((mlib_s32 *)(psrc2 + j))[2];

				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[0].u[3] = (mlib_u32)a3 >> 16;
				b[1].u[1] = a1;
				b[1].u[3] = a3;
				b[2].u[1] = (mlib_u32)a2 >> 16;
				b[2].u[3] = (mlib_u32)a4 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				b[0].u[1] = a2;
				b[0].u[3] = a4;
				b[1].u[1] = (mlib_u32)a5 >> 16;
				b[1].u[3] = (mlib_u32)a6 >> 16;
				b[2].u[1] = a5;
				b[2].u[3] = a6;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
				j += 6;
			}
		} else {
			a6 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 6); j += 6) {
				b[0].u[3] = a6;
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
				a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];
				a5 = ((mlib_s32 *)(psrc1 + j))[2];
				a6 = ((mlib_s32 *)(psrc2 + j + 1))[2];

				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[1].u[1] = a1;
				b[1].u[3] = (mlib_u32)a3 >> 16;
				b[2].u[1] = (mlib_u32)a2 >> 16;
				b[2].u[3] = a3;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				b[0].u[1] = a2;
				b[0].u[3] = (mlib_u32)a4 >> 16;
				b[1].u[1] = (mlib_u32)a5 >> 16;
				b[1].u[3] = a4;
				b[2].u[1] = a5;
				b[2].u[3] = (mlib_u32)a6 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
			}
		}

		if (j < size_row) {
			a1 = psrc1[j];
			a2 = psrc2[j];
			a3 = psrc1[j + 1];
			a4 = psrc2[j + 1];
			a5 = psrc1[j + 2];
			a6 = psrc2[j + 2];
			c1 += a1 * (mlib_d64)a2;
			c2 += a3 * (mlib_d64)a4;
			c3 += a5 * (mlib_d64)a6;
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_S16_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;

#ifdef _LITTLE_ENDIAN
	mlib_s32 k;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0;
	res[0] = res[1] = res[2] = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_S16(psrc1[j], psrc2[j]);
			c2 += MUL_S16(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S16(psrc1[j + 2], psrc2[j + 2]);
			c4 += MUL_S16(psrc1[j + 3], psrc2[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			res[k] += MUL_S16(psrc1[j], psrc2[j]);
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] += c1;
	res[1] += c2;
	res[2] += c3;
	res[3] = c4;
#else /* _LITTLE_ENDIAN */
	mlib_s32 a1, a2, a3, a4;
	mlib_s32 align;
	union
	{
		mlib_d64 db;
		mlib_s32 s[2];
		mlib_u16 u[4];
	} b[4],
	    r1;

	b[0].db = b[1].db = b[2].db = b[3].db = 0.0;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0;
	res[0] = res[1] = res[2] = res[3] = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc1) {
			a1 = ((mlib_u16 *)psrc1)[0] << 16;
			a2 = ((mlib_u16 *)psrc2)[0] << 16;
			c4 = a1 * (mlib_d64)a2;
			j = 1;
		}

		align = j;

/* same alignment */
		if (!(2 & (mlib_addr)(psrc2 + j))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 4); j += 4) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a3 = ((mlib_s32 *)(psrc2 + j))[0];
				a4 = ((mlib_s32 *)(psrc2 + j))[1];

				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[0].u[2] = (mlib_u32)a3 >> 16;
				b[1].u[0] = a1;
				b[1].u[2] = a3;
				b[2].u[0] = (mlib_u32)a2 >> 16;
				b[2].u[2] = (mlib_u32)a4 >> 16;
				b[3].u[0] = a2;
				b[3].u[2] = a4;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[3].db;
				c4 += (mlib_d64)r1.s[0] * r1.s[1];
			}
		} else {
			a4 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 4); j += 4) {
				b[0].u[2] = a4;
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
				a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];

				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[1].u[0] = a1;
				b[1].u[2] = (mlib_u32)a3 >> 16;
				b[2].u[0] = (mlib_u32)a2 >> 16;
				b[2].u[2] = a3;
				b[3].u[0] = a2;
				b[3].u[2] = (mlib_u32)a4 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[3].db;
				c4 += (mlib_d64)r1.s[0] * r1.s[1];
			}
		}

		if (j < size_row) {
			a1 = ((mlib_s32 *)(psrc1 + j))[0];
			b[0].u[0] = (mlib_u32)a1 >> 16;
			b[0].u[2] = ((mlib_u16 *)psrc2)[j];
			r1.db = b[0].db;
			c1 += (mlib_d64)r1.s[0] * r1.s[1];
			j++;

			if (j < size_row) {
				b[1].u[0] = a1;
				b[1].u[2] = ((mlib_u16 *)psrc2)[j];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				j++;

				if (j < size_row) {
					a1 = ((mlib_u16 *)psrc1)[j] << 16;
					a2 = ((mlib_u16 *)psrc2)[j] << 16;
					c3 += (mlib_d64)a1 *a2;
				}
			}
		}

		res[align] += c1 * (1. / (65536.0 * 65536.0));
		res[1 + align] += c2 * (1. / (65536.0 * 65536.0));
		res[2 + align] += c3 * (1. / (65536.0 * 65536.0));
		res[3 & (3 + align)] += c4 * (1. / (65536.0 * 65536.0));
		c1 = c2 = c3 = c4 = 0.0;
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_S16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

#ifdef _LITTLE_ENDIAN

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < size_row; j += 3) {
			c1 += MUL_S16(psrc1[j], psrc2[j]);
			c2 += MUL_S16(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S16(psrc1[j + 2], psrc2[j + 2]);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;

#else /* _LITTLE_ENDIAN */

	mlib_s32 a1, a2, a3, a4, a5, a6;
	union
	{
		mlib_d64 db;
		mlib_s32 s[2];
		mlib_u16 u[4];
	} b[3],
	    r1;

	b[0].db = b[1].db = b[2].db = 0.0;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc1) {
			a1 = ((mlib_u16 *)psrc1)[0] << 16;
			a2 = ((mlib_u16 *)psrc2)[0] << 16;
			a3 = ((mlib_u16 *)psrc1)[1] << 16;
			a4 = ((mlib_u16 *)psrc2)[1] << 16;
			a5 = ((mlib_u16 *)psrc1)[2] << 16;
			a6 = ((mlib_u16 *)psrc2)[2] << 16;
			c1 += a1 * (mlib_d64)a2;
			c2 += a3 * (mlib_d64)a4;
			c3 += a5 * (mlib_d64)a6;
			j = 3;
			if (j >= size_row) {
				psrc1 += src1_stride;
				psrc2 += src2_stride;
				continue;
			}
		}

/* same alignment */
		if (!(2 & (mlib_addr)(psrc2 + j))) {
			a1 = ((mlib_s32 *)(psrc1 + j))[0];
			a3 = ((mlib_s32 *)(psrc2 + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 12); j += 6) {
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a4 = ((mlib_s32 *)(psrc2 + j))[1];
				a5 = ((mlib_s32 *)(psrc1 + j))[2];
				a6 = ((mlib_s32 *)(psrc2 + j))[2];

				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[0].u[2] = (mlib_u32)a3 >> 16;
				b[1].u[0] = a1;
				b[1].u[2] = a3;
				b[2].u[0] = (mlib_u32)a2 >> 16;
				b[2].u[2] = (mlib_u32)a4 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				b[0].u[0] = a2;
				b[0].u[2] = a4;
				b[1].u[0] = (mlib_u32)a5 >> 16;
				b[1].u[2] = (mlib_u32)a6 >> 16;
				b[2].u[0] = a5;
				b[2].u[2] = a6;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				a1 = ((mlib_s32 *)(psrc1 + j))[3];
				a3 = ((mlib_s32 *)(psrc2 + j))[3];
			}

			if (j <= (size_row - 6)) {
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a4 = ((mlib_s32 *)(psrc2 + j))[1];
				a5 = ((mlib_s32 *)(psrc1 + j))[2];
				a6 = ((mlib_s32 *)(psrc2 + j))[2];

				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[0].u[2] = (mlib_u32)a3 >> 16;
				b[1].u[0] = a1;
				b[1].u[2] = a3;
				b[2].u[0] = (mlib_u32)a2 >> 16;
				b[2].u[2] = (mlib_u32)a4 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				b[0].u[0] = a2;
				b[0].u[2] = a4;
				b[1].u[0] = (mlib_u32)a5 >> 16;
				b[1].u[2] = (mlib_u32)a6 >> 16;
				b[2].u[0] = a5;
				b[2].u[2] = a6;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
				j += 6;
			}
		} else {
			a6 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 6); j += 6) {
				b[0].u[2] = a6;
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a2 = ((mlib_s32 *)(psrc1 + j))[1];
				a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
				a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];
				a5 = ((mlib_s32 *)(psrc1 + j))[2];
				a6 = ((mlib_s32 *)(psrc2 + j + 1))[2];

				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[1].u[0] = a1;
				b[1].u[2] = (mlib_u32)a3 >> 16;
				b[2].u[0] = (mlib_u32)a2 >> 16;
				b[2].u[2] = a3;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];

				b[0].u[0] = a2;
				b[0].u[2] = (mlib_u32)a4 >> 16;
				b[1].u[0] = (mlib_u32)a5 >> 16;
				b[1].u[2] = a4;
				b[2].u[0] = a5;
				b[2].u[2] = (mlib_u32)a6 >> 16;

				r1.db = b[0].db;
				c1 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[1].db;
				c2 += (mlib_d64)r1.s[0] * r1.s[1];
				r1.db = b[2].db;
				c3 += (mlib_d64)r1.s[0] * r1.s[1];
			}
		}

		if (j < size_row) {
			a1 = ((mlib_u16 *)psrc1)[j] << 16;
			a2 = ((mlib_u16 *)psrc2)[j] << 16;
			a3 = ((mlib_u16 *)psrc1)[j + 1] << 16;
			a4 = ((mlib_u16 *)psrc2)[j + 1] << 16;
			a5 = ((mlib_u16 *)psrc1)[j + 2] << 16;
			a6 = ((mlib_u16 *)psrc2)[j + 2] << 16;
			c1 += a1 * (mlib_d64)a2;
			c2 += a3 * (mlib_d64)a4;
			c3 += a5 * (mlib_d64)a6;
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] = c1 * (1. / (65536.0 * 65536.0));
	res[1] = c2 * (1. / (65536.0 * 65536.0));
	res[2] = c3 * (1. / (65536.0 * 65536.0));
#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

#define	MUL_S32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_S32_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0;
	res[0] = res[1] = res[2] = 0;
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_S32(psrc1[j], psrc2[j]);
			c2 += MUL_S32(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S32(psrc1[j + 2], psrc2[j + 2]);
			c4 += MUL_S32(psrc1[j + 3], psrc2[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			res[k] += MUL_S32(psrc1[j], psrc2[j]);
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] += c1;
	res[1] += c2;
	res[2] += c3;
	res[3] = c4;
}

/* *********************************************************** */

void
mlib_c_ImageCrossCorrel_S32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < size_row; j += 3) {
			c1 += MUL_S32(psrc1[j], psrc2[j]);
			c2 += MUL_S32(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_S32(psrc1[j + 2], psrc2[j + 2]);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
}

/* *********************************************************** */
