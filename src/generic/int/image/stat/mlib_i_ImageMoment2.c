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

#pragma ident	"@(#)mlib_i_ImageMoment2.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMoment2 - calculates moment of power two
 *                          for the input image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMoment2(mlib_d64         *moment,
 *                                    const mlib_image *img)
 *
 * ARGUMENTS
 *      moment   pointer to moment array
 *      img      pointer to an image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1         2
 *      c = --- * SUM SUM(x[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>
#include <mlib_c_ImageMoment2.h>

#ifndef _NO_LONGLONG /* for 64-bit platform */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMoment2 = __mlib_ImageMoment2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMoment2) mlib_ImageMoment2
    __attribute__((weak, alias("__mlib_ImageMoment2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMoment2(
    mlib_d64 *moment,
    const mlib_image *img)
{
/* total number of pixels */
	mlib_s32 num_pixel;
	mlib_d64 rez[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (moment == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img) * mlib_ImageGetHeight(img);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_U8_3(img, rez);
		else
			mlib_c_ImageMoment2_U8_124(img, rez);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_U16_3(img, rez);
		else
			mlib_c_ImageMoment2_U16_124(img, rez);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_S16_3(img, rez);
		else
			mlib_c_ImageMoment2_S16_124(img, rez);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_S32_3(img, rez);
		else
			mlib_c_ImageMoment2_S32_124(img, rez);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		moment[0] = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		break;

	case 2:
		moment[0] = (rez[0] + rez[2]) * divider;
		moment[1] = (rez[1] + rez[3]) * divider;
		break;

	case 4:
		moment[3] = rez[3] * divider;
	case 3:
		moment[0] = rez[0] * divider;
		moment[1] = rez[1] * divider;
		moment[2] = rez[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U8_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
	mlib_s64 lrez[4];
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_s64 c1, c2, c3, c4;

/* values by channels */
	mlib_s64 d1, d2, d3, d4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 nchan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	lrez[0] = lrez[1] = lrez[2] = lrez[3] = 0;
	c1 = c2 = c3 = c4 = 0;
	for (i = 0; i < height; i++) {
		mlib_s32 nalign = (mlib_addr)psrc & 3;
		mlib_s32 nbytes = size_row + nalign;
		mlib_s32 *p32 = (mlib_s32 *)(psrc - nalign);
		mlib_s32 valb = ((mlib_u8 *)p32)[0];
		mlib_s32 val = *(p32++);

#ifdef _LITTLE_ENDIAN  /* for x86 */
		mlib_u32 mask = (mlib_u32)0xffffffff << (nalign << 3);
		valb = (valb & mask) << 3;
#else /* _LITTLE_ENDIAN */
		mlib_u32 mask = (mlib_u32)0xffffffff >> (nalign << 3);
		valb = ((valb << 3) & (mask >> 21));
#endif /* _LITTLE_ENDIAN */
		val &= mask;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (nbytes - 8); j += 4) {
			d1 = valb;
#ifdef _LITTLE_ENDIAN
			d2 = (val >> 5) & 0x7f8;
			d3 = (val >> 13) & 0x7f8;
			d4 = (val >> 21) & 0x7f8;
#else /* _LITTLE_ENDIAN */
			d2 = (val >> 13) & 0x7f8;
			d3 = (val >> 5) & 0x7f8;
			d4 = (val << 3) & 0x7f8;
#endif /* _LITTLE_ENDIAN */
			c1 += d1 * d1;
			c2 += d2 * d2;
			c3 += d3 * d3;
			c4 += d4 * d4;
			valb = ((mlib_u8 *)p32)[0] << 3;
			val = *(p32++);
		}

		if (j <= (nbytes - 4)) {
			d1 = valb;
#ifdef _LITTLE_ENDIAN
			d2 = (val >> 5) & 0x7f8;
			d3 = (val >> 13) & 0x7f8;
			d4 = (val >> 21) & 0x7f8;
#else /* _LITTLE_ENDIAN */
			d2 = (val >> 13) & 0x7f8;
			d3 = (val >> 5) & 0x7f8;
			d4 = (val << 3) & 0x7f8;
#endif /* _LITTLE_ENDIAN */
			c1 += d1 * d1;
			c2 += d2 * d2;
			c3 += d3 * d3;
			c4 += d4 * d4;
			nbytes &= 3;
			if (nbytes) {
				valb = ((mlib_u8 *)p32)[0] << 3;
				val = *p32;
			}
		}

		if (nbytes) {
			d1 = valb;
			c1 += d1 * d1;
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (nbytes << 3);
			val = val & ~mask;
			d2 = (val >> 5) & 0x7f8;
			d3 = (val >> 13) & 0x7f8;
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (nbytes << 3);
			val = val & ~mask;
			d2 = (val >> 13) & 0x7f8;
			d3 = (val >> 5) & 0x7f8;
#endif /* _LITTLE_ENDIAN */
			c2 += d2 * d2;
			c3 += d3 * d3;
		}

		lrez[3 & (0 - nalign)] += c1;
		lrez[3 & (1 - nalign)] += c2;
		lrez[3 & (2 - nalign)] += c3;
		lrez[3 & (3 - nalign)] += c4;

		c1 = c2 = c3 = c4 = 0;
		psrc += src_stride;
	}
	rez[0] = lrez[0] >> 6;
	rez[1] = lrez[1] >> 6;
	rez[2] = lrez[2] >> 6;
	rez[3] = lrez[3] >> 6;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U8_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_s64 c1, c2, c3;
/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if ((mlib_addr)psrc & 1) {
			c1 += (mlib_s64)psrc[j] * (mlib_s64)psrc[j];
			c2 += (mlib_s64)psrc[j+1] * (mlib_s64)psrc[j+1];
			c3 += (mlib_s64)psrc[j+2] * (mlib_s64)psrc[j+2];
			j += 3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			mlib_u32 u1, u2;

			u1 = ((mlib_u16 *)(psrc + j))[0];
			u2 = ((mlib_u16 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			c1 += (u1 & 0xff) * (u1 & 0xff);
			c2 += (u1 >> 8) * (u1 >> 8);
			c3 += (u2 & 0xff) * (u2 & 0xff);
			c1 += (u2 >> 8) * (u2 >> 8);
#else /* _LITTLE_ENDIAN */
			c1 += (u1 >> 8) * (u1 >> 8);
			c2 += (u1 & 0xff) * (u1 & 0xff);
			c3 += (u2 >> 8) * (u2 >> 8);
			c1 += (u2 & 0xff) * (u2 & 0xff);
#endif /* _LITTLE_ENDIAN */
			c2 += (mlib_s64)psrc[j + 4] * (mlib_s64)psrc[j + 4];
			c3 += (mlib_s64)psrc[j + 5] * (mlib_s64)psrc[j + 5];
		}

		if (j < size_row) {
			c1 += (mlib_s64)psrc[j] * (mlib_s64)psrc[j];
			c2 += (mlib_s64)psrc[j + 1] * (mlib_s64)psrc[j + 1];
			c3 += (mlib_s64)psrc[j + 2] * (mlib_s64)psrc[j + 2];
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U16_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
	mlib_u64 lrez[4];
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_u64 c1, c2, c3, c4;
/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 ch1, ch2, ch3, ch4;
	mlib_u32 a1, a2;
	mlib_s32 align;

	lrez[0] = lrez[1] = lrez[2] = lrez[3] = c1 = c2 = c3 = c4 = 0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch4 = ((mlib_u16 *)psrc)[0];
			c4 += (mlib_s64)ch4 * (mlib_s64)ch4;
			j = 1;
		}

		align = j;
		if (j <= (size_row - 2))
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 8); j += 4) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
			ch3 = a2 & 0xffff;
			ch4 = a2 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
			ch3 = a2 >> 16;
			ch4 = a2 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			a1 = ((mlib_s32 *)(psrc + j))[2];
			c1 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c2 += (mlib_s64)ch2 * (mlib_s64)ch2;
			c3 += (mlib_s64)ch3 * (mlib_s64)ch3;
			c4 += (mlib_s64)ch4 * (mlib_s64)ch4;
		}

		if (j <= (size_row - 4)) {

			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
			ch3 = a2 & 0xffff;
			ch4 = a2 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
			ch3 = a2 >> 16;
			ch4 = a2 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c2 += (mlib_s64)ch2 * (mlib_s64)ch2;
			c3 += (mlib_s64)ch3 * (mlib_s64)ch3;
			c4 += (mlib_s64)ch4 * (mlib_s64)ch4;
			j += 4;
		}

		if (size_row > j) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch1 * (mlib_s64)ch1;
			j++;

			if (size_row > j) {
				c2 += (mlib_s64)ch2 * (mlib_s64)ch2;
				j++;

				if (size_row > j) {
					ch3 = ((mlib_u16 *)psrc)[j];
					c3 += (mlib_s64)ch3 * (mlib_s64)ch3;
				}
			}
		}

		lrez[0 + align] += c1;
		lrez[1 + align] += c2;
		lrez[2 + align] += c3;
		lrez[3 & (3 + align)] += c4;
		c1 = c2 = c3 = c4 = 0;
		psrc += src_stride;
	}
	rez[0] = lrez[0];
	rez[1] = lrez[1];
	rez[2] = lrez[2];
	rez[3] = lrez[3];
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U16_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_u64 c1, c2, c3;
/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_u32 a1, a2, a3;
	mlib_s32 ch0, ch1, ch2;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch0 = ((mlib_u16 *)psrc)[0];
			a3 = ((mlib_s32 *)(psrc + 1))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch0 * (mlib_s64)ch0;
			c2 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c3 += (mlib_s64)ch2 * (mlib_s64)ch2;
			j = 3;
		}

		if (j < size_row)
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 12); j += 6) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
			ch2 = a2 & 0xffff;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
			ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch0 * (mlib_s64)ch0;
			c2 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c3 += (mlib_s64)ch2 * (mlib_s64)ch2;

			a1 = ((mlib_s32 *)(psrc + j))[3];

#ifdef _LITTLE_ENDIAN
			ch0 = a2 >> 16;
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 & 0xffff;
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch0 * (mlib_s64)ch0;
			c2 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c3 += (mlib_s64)ch2 * (mlib_s64)ch2;
		}

		if (j <= (size_row - 6)) {

			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
			ch2 = a2 & 0xffff;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
			ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch0 * (mlib_s64)ch0;
			c2 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c3 += (mlib_s64)ch2 * (mlib_s64)ch2;

#ifdef _LITTLE_ENDIAN
			ch0 = a2 >> 16;
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 & 0xffff;
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			j += 6;
		}
		if (j < size_row) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
			ch2 = ((mlib_u16 *)psrc)[j + 2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += (mlib_s64)ch0 * (mlib_s64)ch0;
			c2 += (mlib_s64)ch1 * (mlib_s64)ch1;
			c3 += (mlib_s64)ch2 * (mlib_s64)ch2;
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S16_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
	mlib_s64 lrez[4];
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_s64 c1, c2, c3, c4;
/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 ch1, ch2, ch3, ch4;
	mlib_s32 a1, a2;
	mlib_s32 align;

	lrez[0] = lrez[1] = lrez[2] = lrez[3] = c1 = c2 = c3 = c4 = 0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch4 = ((mlib_u16 *)psrc)[0] << 16;
			c4 += ((mlib_s64)ch4 * (mlib_s64)ch4) >> 32;
			j = 1;
		}

		align = j;
		if (j <= (size_row - 2))
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 8); j += 4) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 << 16;
			ch2 = a1 & 0xffff0000;
			ch3 = a2 << 16;
			ch4 = a2 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 & 0xffff0000;
			ch2 = a1 << 16;
			ch3 = a2 & 0xffff0000;
			ch4 = a2 << 16;
#endif /* _LITTLE_ENDIAN */
			a1 = ((mlib_s32 *)(psrc + j))[2];
			c1 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c2 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
			c3 += ((mlib_s64)ch3 * (mlib_s64)ch3) >> 32;
			c4 += ((mlib_s64)ch4 * (mlib_s64)ch4) >> 32;
		}

		if (j <= (size_row - 4)) {

			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 << 16;
			ch2 = a1 & 0xffff0000;
			ch3 = a2 << 16;
			ch4 = a2 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 & 0xffff0000;
			ch2 = a1 << 16;
			ch3 = a2 & 0xffff0000;
			ch4 = a2 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c2 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
			c3 += ((mlib_s64)ch3 * (mlib_s64)ch3) >> 32;
			c4 += ((mlib_s64)ch4 * (mlib_s64)ch4) >> 32;
			j += 4;
		}

		if (size_row > j) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 << 16;
			ch2 = a1 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 & 0xffff0000;
			ch2 = a1 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			j++;

			if (size_row > j) {
				c2 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
				j++;

				if (size_row > j) {
					ch3 = ((mlib_u16 *)psrc)[j] << 16;
					c3 += ((mlib_s64)ch3 * (mlib_s64)ch3)
							>> 32;
				}
			}
		}

		lrez[0 + align] += c1;
		lrez[1 + align] += c2;
		lrez[2 + align] += c3;
		lrez[3 & (3 + align)] += c4;
		c1 = c2 = c3 = c4 = 0;
		psrc += src_stride;
	}
	rez[0] = lrez[0];
	rez[1] = lrez[1];
	rez[2] = lrez[2];
	rez[3] = lrez[3];
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S16_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_s64 c1, c2, c3;
/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3;
	mlib_s32 ch0, ch1, ch2;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch0 = ((mlib_u16 *)psrc)[0] << 16;
			a3 = ((mlib_s32 *)(psrc + 1))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a3 << 16;
			ch2 = a3 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a3 & 0xffff0000;
			ch2 = a3 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch0 * (mlib_s64)ch0) >> 32;
			c2 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c3 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
			j = 3;
		}

		if (j < size_row)
			a1 = ((mlib_s32 *)(psrc + j))[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 12); j += 6) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 << 16;
			ch1 = a1 & 0xffff0000;
			ch2 = a2 << 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 & 0xffff0000;
			ch1 = a1 << 16;
			ch2 = a2 & 0xffff0000;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch0 * (mlib_s64)ch0) >> 32;
			c2 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c3 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
			a1 = ((mlib_s32 *)(psrc + j))[3];

#ifdef _LITTLE_ENDIAN
			ch0 = a2 & 0xffff0000;
			ch1 = a3 << 16;
			ch2 = a3 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 << 16;
			ch1 = a3 & 0xffff0000;
			ch2 = a3 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch0 * (mlib_s64)ch0) >> 32;
			c2 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c3 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
		}

		if (j <= (size_row - 6)) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 << 16;
			ch1 = a1 & 0xffff0000;
			ch2 = a2 << 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 & 0xffff0000;
			ch1 = a1 << 16;
			ch2 = a2 & 0xffff0000;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch0 * (mlib_s64)ch0) >> 32;
			c2 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c3 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;

#ifdef _LITTLE_ENDIAN
			ch0 = a2 & 0xffff0000;
			ch1 = a3 << 16;
			ch2 = a3 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 << 16;
			ch1 = a3 & 0xffff0000;
			ch2 = a3 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch0 * (mlib_s64)ch0) >> 32;
			c2 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c3 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
			j  += 6;
		}

		if (j < size_row) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
			ch2 = ((mlib_u16 *)psrc)[j + 2] << 16;
#ifdef _LITTLE_ENDIAN
			ch0 = a1 << 16;
			ch1 = a1 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 & 0xffff0000;
			ch1 = a1 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ((mlib_s64)ch0 * (mlib_s64)ch0) >> 32;
			c2 += ((mlib_s64)ch1 * (mlib_s64)ch1) >> 32;
			c3 += ((mlib_s64)ch2 * (mlib_s64)ch2) >> 32;
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S32_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_s64 c1_lo, c1_hi, c1_err;
	mlib_s64 c2_lo, c2_hi, c2_err;
	mlib_s64 c3_lo, c3_hi, c3_err;
	mlib_s64 c4_lo, c4_hi, c4_err;
	mlib_s64 hi, lo;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j, k;

	rez[0] = rez[1] = rez[2] = 0.0;
	c1_lo = c1_hi = c1_err = 0;
	c2_lo = c2_hi = c2_err = 0;
	c3_lo = c3_hi = c3_err = 0;
	c4_lo = c4_hi = c4_err = 0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			hi = psrc[j] >> 16;
			lo = psrc[j] & 0xffff;
			c1_lo += lo * lo;
			c1_hi += hi * hi;
			c1_err += lo * hi;

			hi = psrc[j+1] >> 16;
			lo = psrc[j+1] & 0xffff;
			c2_lo += lo * lo;
			c2_hi += hi * hi;
			c2_err += lo * hi;

			hi = psrc[j + 2] >> 16;
			lo = psrc[j + 2] & 0xffff;
			c3_lo += lo * lo;
			c3_hi += hi * hi;
			c3_err += lo * hi;

			hi = psrc[j + 3] >> 16;
			lo = psrc[j + 3] & 0xffff;
			c4_lo += lo * lo;
			c4_hi += hi * hi;
			c4_err += lo * hi;
		}

		for (k = 0; j < size_row; j++, k++) {
			rez[k] += (mlib_s64)psrc[j] * (mlib_s64)psrc[j];
		}
			psrc += src_stride;
	}
	rez[0] += c1_lo + (mlib_d64)c1_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c1_err * (1 << 17);
	rez[1] += c2_lo + (mlib_d64)c2_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c2_err * (1 << 17);
	rez[2] += c3_lo + (mlib_d64)c3_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c3_err * (1 << 17);
	rez[3] = c4_lo + (mlib_d64)c4_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c4_err * (1 << 17);
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S32_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_s64 c1_lo, c1_hi, c1_err;
	mlib_s64 c2_lo, c2_hi, c2_err;
	mlib_s64 c3_lo, c3_hi, c3_err;
	mlib_s64 hi, lo;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	c1_lo = c1_hi = c1_err = 0;
	c2_lo = c2_hi = c2_err = 0;
	c3_lo = c3_hi = c3_err = 0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			hi = psrc[j] >> 16;
			lo = psrc[j] & 0xffff;
			c1_lo += lo * lo;
			c1_hi += hi * hi;
			c1_err += lo * hi;

			hi = psrc[j+1] >> 16;
			lo = psrc[j+1] & 0xffff;
			c2_lo += lo * lo;
			c2_hi += hi * hi;
			c2_err += lo * hi;

			hi = psrc[j + 2] >> 16;
			lo = psrc[j + 2] & 0xffff;
			c3_lo += lo * lo;
			c3_hi += hi * hi;
			c3_err += lo * hi;
		}

		psrc += src_stride;
	}

	rez[0] = c1_lo + (mlib_d64)c1_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c1_err * (1 << 17);
	rez[1] = c2_lo + (mlib_d64)c2_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c2_err * (1 << 17);
	rez[2] = c3_lo + (mlib_d64)c3_hi * ((mlib_s64)1 << 32) +
				(mlib_d64)c3_err * (1 << 17);
}

/* *********************************************************** */
#else /* _NO_LONGLONG */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMoment2 = __mlib_ImageMoment2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMoment2) mlib_ImageMoment2
    __attribute__((weak, alias("__mlib_ImageMoment2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMoment2(
    mlib_d64 *moment,
    const mlib_image *img)
{
/* total number of pixels */
	mlib_s32 num_pixel;
	mlib_d64 rez[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (moment == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img) * mlib_ImageGetHeight(img);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_U8_3(img, rez);
		else
			mlib_c_ImageMoment2_U8_124(img, rez);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_U16_3(img, rez);
		else
			mlib_c_ImageMoment2_U16_124(img, rez);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_S16_3(img, rez);
		else
			mlib_c_ImageMoment2_S16_124(img, rez);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_c_ImageMoment2_S32_3(img, rez);
		else
			mlib_c_ImageMoment2_S32_124(img, rez);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		moment[0] = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		break;

	case 2:
		moment[0] = (rez[0] + rez[2]) * divider;
		moment[1] = (rez[1] + rez[3]) * divider;
		break;

	case 4:
		moment[3] = rez[3] * divider;
	case 3:
		moment[0] = rez[0] * divider;
		moment[1] = rez[1] * divider;
		moment[2] = rez[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_U8(x)	(mlib_U82D64[(x)] * mlib_U82D64[(x)])
#define	MUL_S16(x)	((x) * (mlib_d64) (x))

#define	MUL_S32(x)	((x) * (mlib_d64) (x))

/* *********************************************************** */

void
mlib_c_ImageMoment2_U8_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* values by channels */
	mlib_d64 d1, d2, d3, d4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 nchan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = rez[3] = 0;
	c1 = c2 = c3 = c4 = 0.0;
	for (i = 0; i < height; i++) {
		mlib_s32 nalign = (mlib_addr)psrc & 3;
		mlib_s32 nbytes = size_row + nalign;
		mlib_s32 *p32 = (mlib_s32 *)(psrc - nalign);
		mlib_s32 valb = ((mlib_u8 *)p32)[0];
		mlib_s32 val = *(p32++);

#ifdef _LITTLE_ENDIAN
		mlib_u32 mask = (mlib_u32)0xffffffff << (nalign << 3);

		valb = (valb & mask) << 3;
#else /* _LITTLE_ENDIAN */
		mlib_u32 mask = (mlib_u32)0xffffffff >> (nalign << 3);

		valb = ((valb << 3) & (mask >> 21));
#endif /* _LITTLE_ENDIAN */
		val &= mask;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (nbytes - 8); j += 4) {
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 + valb);
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
			c1 += d1 * d1;
			c2 += d2 * d2;
			c3 += d3 * d3;
			c4 += d4 * d4;
			valb = ((mlib_u8 *)p32)[0] << 3;
			val = *(p32++);
		}

		if (j <= (nbytes - 4)) {
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 + valb);
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
			c1 += d1 * d1;
			c2 += d2 * d2;
			c3 += d3 * d3;
			c4 += d4 * d4;
			nbytes &= 3;
			if (nbytes) {
				valb = ((mlib_u8 *)p32)[0] << 3;
				val = *p32;
			}
		}

		if (nbytes) {
			d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 + valb);
			c1 += d1 * d1;
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (nbytes << 3);
			val = val & ~mask;
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (nbytes << 3);
			val = val & ~mask;
			d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 13) & 0x7f8));
			d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
			    ((val >> 5) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
			c2 += d2 * d2;
			c3 += d3 * d3;
		}

		rez[3 & (0 - nalign)] += c1;
		rez[3 & (1 - nalign)] += c2;
		rez[3 & (2 - nalign)] += c3;
		rez[3 & (3 - nalign)] += c4;
		c1 = c2 = c3 = c4 = 0.0;

		psrc += src_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U8_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if ((mlib_addr)psrc & 1) {
			c1 += MUL_U8(psrc[j]);
			c2 += MUL_U8(psrc[j + 1]);
			c3 += MUL_U8(psrc[j + 2]);
			j += 3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			mlib_u32 u1, u2;

			u1 = ((mlib_u16 *)(psrc + j))[0];
			u2 = ((mlib_u16 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			c1 += MUL_U8(u1 & 0xff);
			c2 += MUL_U8(u1 >> 8);
			c3 += MUL_U8(u2 & 0xff);
			c1 += MUL_U8(u2 >> 8);
#else /* _LITTLE_ENDIAN */
			c1 += MUL_U8(u1 >> 8);
			c2 += MUL_U8(u1 & 0xff);
			c3 += MUL_U8(u2 >> 8);
			c1 += MUL_U8(u2 & 0xff);
#endif /* _LITTLE_ENDIAN */
			c2 += MUL_U8(psrc[j + 4]);
			c3 += MUL_U8(psrc[j + 5]);
		}

		if (j < size_row) {
			c1 += MUL_U8(psrc[j]);
			c2 += MUL_U8(psrc[j + 1]);
			c3 += MUL_U8(psrc[j + 2]);
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U16_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 ch1, ch2, ch3, ch4;
	mlib_u32 a1, a2;
	mlib_s32 align;

	rez[0] = rez[1] = rez[2] = rez[3] = c1 = c2 = c3 = c4 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch4 = ((mlib_u16 *)psrc)[0];
			c4 += ch4 * (mlib_d64)ch4;
			j = 1;
		}

		align = j;
		if (j <= (size_row - 2))
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 8); j += 4) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
			ch3 = a2 & 0xffff;
			ch4 = a2 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
			ch3 = a2 >> 16;
			ch4 = a2 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			a1 = ((mlib_s32 *)(psrc + j))[2];
			c1 += ch1 * (mlib_d64)ch1;
			c2 += ch2 * (mlib_d64)ch2;
			c3 += ch3 * (mlib_d64)ch3;
			c4 += ch4 * (mlib_d64)ch4;
		}

		if (j <= (size_row - 4)) {

			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
			ch3 = a2 & 0xffff;
			ch4 = a2 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
			ch3 = a2 >> 16;
			ch4 = a2 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch1 * (mlib_d64)ch1;
			c2 += ch2 * (mlib_d64)ch2;
			c3 += ch3 * (mlib_d64)ch3;
			c4 += ch4 * (mlib_d64)ch4;
			j += 4;
		}

		if (size_row > j) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch1 * (mlib_d64)ch1;
			j++;

			if (size_row > j) {
				c2 += ch2 * (mlib_d64)ch2;
				j++;

				if (size_row > j) {
					ch3 = ((mlib_u16 *)psrc)[j];
					c3 += ch3 * (mlib_d64)ch3;
				}
			}
		}

		rez[0 + align] += c1;
		rez[1 + align] += c2;
		rez[2 + align] += c3;
		rez[3 & (3 + align)] += c4;
		c1 = c2 = c3 = c4 = 0.0;
		psrc += src_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_U16_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_u32 a1, a2, a3;
	mlib_s32 ch0, ch1, ch2;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0.0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch0 = ((mlib_u16 *)psrc)[0];
			a3 = ((mlib_s32 *)(psrc + 1))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			j = 3;
		}

		if (j < size_row)
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 12); j += 6) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
			ch2 = a2 & 0xffff;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
			ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			a1 = ((mlib_s32 *)(psrc + j))[3];

#ifdef _LITTLE_ENDIAN
			ch0 = a2 >> 16;
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 & 0xffff;
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
		}

		if (j <= (size_row - 6)) {

			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
			ch2 = a2 & 0xffff;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
			ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;

#ifdef _LITTLE_ENDIAN
			ch0 = a2 >> 16;
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 & 0xffff;
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			j += 6;
		}
		if (j < size_row) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
			ch2 = ((mlib_u16 *)psrc)[j + 2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S16_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 ch1, ch2, ch3, ch4;
	mlib_s32 a1, a2;
	mlib_s32 align;

	rez[0] = rez[1] = rez[2] = rez[3] = c1 = c2 = c3 = c4 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch4 = ((mlib_u16 *)psrc)[0] << 16;
			c4 += ch4 * (mlib_d64)ch4;
			j = 1;
		}

		align = j;
		if (j <= (size_row - 2))
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 8); j += 4) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 << 16;
			ch2 = a1 & 0xffff0000;
			ch3 = a2 << 16;
			ch4 = a2 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 & 0xffff0000;
			ch2 = a1 << 16;
			ch3 = a2 & 0xffff0000;
			ch4 = a2 << 16;
#endif /* _LITTLE_ENDIAN */
			a1 = ((mlib_s32 *)(psrc + j))[2];
			c1 += ch1 * (mlib_d64)ch1;
			c2 += ch2 * (mlib_d64)ch2;
			c3 += ch3 * (mlib_d64)ch3;
			c4 += ch4 * (mlib_d64)ch4;
		}

		if (j <= (size_row - 4)) {

			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 << 16;
			ch2 = a1 & 0xffff0000;
			ch3 = a2 << 16;
			ch4 = a2 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 & 0xffff0000;
			ch2 = a1 << 16;
			ch3 = a2 & 0xffff0000;
			ch4 = a2 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch1 * (mlib_d64)ch1;
			c2 += ch2 * (mlib_d64)ch2;
			c3 += ch3 * (mlib_d64)ch3;
			c4 += ch4 * (mlib_d64)ch4;
			j += 4;
		}

		if (size_row > j) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 << 16;
			ch2 = a1 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 & 0xffff0000;
			ch2 = a1 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch1 * (mlib_d64)ch1;
			j++;

			if (size_row > j) {
				c2 += ch2 * (mlib_d64)ch2;
				j++;

				if (size_row > j) {
					ch3 = ((mlib_u16 *)psrc)[j] << 16;
					c3 += ch3 * (mlib_d64)ch3;
				}
			}
		}

		rez[0 + align] += c1 * (1. / (65536.0 * 65536.0));
		rez[1 + align] += c2 * (1. / (65536.0 * 65536.0));
		rez[2 + align] += c3 * (1. / (65536.0 * 65536.0));
		rez[3 & (3 + align)] += c4 * (1. / (65536.0 * 65536.0));
		c1 = c2 = c3 = c4 = 0.0;
		psrc += src_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S16_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3;
	mlib_s32 ch0, ch1, ch2;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0.0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch0 = ((mlib_u16 *)psrc)[0] << 16;
			a3 = ((mlib_s32 *)(psrc + 1))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a3 << 16;
			ch2 = a3 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch1 = a3 & 0xffff0000;
			ch2 = a3 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			j = 3;
		}

		if (j < size_row)
			a1 = ((mlib_s32 *)(psrc + j))[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 12); j += 6) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 << 16;
			ch1 = a1 & 0xffff0000;
			ch2 = a2 << 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 & 0xffff0000;
			ch1 = a1 << 16;
			ch2 = a2 & 0xffff0000;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			a1 = ((mlib_s32 *)(psrc + j))[3];

#ifdef _LITTLE_ENDIAN
			ch0 = a2 & 0xffff0000;
			ch1 = a3 << 16;
			ch2 = a3 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 << 16;
			ch1 = a3 & 0xffff0000;
			ch2 = a3 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
		}

		if (j <= (size_row - 6)) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 << 16;
			ch1 = a1 & 0xffff0000;
			ch2 = a2 << 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 & 0xffff0000;
			ch1 = a1 << 16;
			ch2 = a2 & 0xffff0000;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;

#ifdef _LITTLE_ENDIAN
			ch0 = a2 & 0xffff0000;
			ch1 = a3 << 16;
			ch2 = a3 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 << 16;
			ch1 = a3 & 0xffff0000;
			ch2 = a3 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			j  += 6;
		}

		if (j < size_row) {
			a1 = ((mlib_s32 *)(psrc + j))[0];
			ch2 = ((mlib_u16 *)psrc)[j + 2] << 16;
#ifdef _LITTLE_ENDIAN
			ch0 = a1 << 16;
			ch1 = a1 & 0xffff0000;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 & 0xffff0000;
			ch1 = a1 << 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
		}

		psrc += src_stride;
	}

	rez[0] = c1 * (1. / (65536.0 * 65536.0));
	rez[1] = c2 * (1. / (65536.0 * 65536.0));
	rez[2] = c3 * (1. / (65536.0 * 65536.0));
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S32_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j, k;

	rez[0] = rez[1] = rez[2] = c1 = c2 = c3 = c4 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_S32(psrc[j]);
			c2 += MUL_S32(psrc[j + 1]);
			c3 += MUL_S32(psrc[j + 2]);
			c4 += MUL_S32(psrc[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			rez[k] += MUL_S32(psrc[j]);
		psrc += src_stride;
	}

	rez[0] += c1;
	rez[1] += c2;
	rez[2] += c3;
	rez[3] = c4;
}

/* *********************************************************** */

void
mlib_c_ImageMoment2_S32_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	c1 = c2 = c3 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			c1 += MUL_S32(psrc[j]);
			c2 += MUL_S32(psrc[j + 1]);
			c3 += MUL_S32(psrc[j + 2]);
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}
/* *********************************************************** */
#endif /* end of ifndef _NO_LONGLONG */
/* *********************************************************** */
