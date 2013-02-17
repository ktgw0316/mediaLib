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

#pragma ident	"@(#)mlib_i_ImageClear.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageClear - Clear an image to a specific color.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageClear(mlib_image     *img,
 *                                  const mlib_s32 *color);
 *
 * ARGUMENT
 *      img     Pointer to an image.
 *      color   Pointer to the color that the image is set to.
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Clear an image to a specific color.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageClear = __mlib_ImageClear

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageClear) mlib_ImageClear
    __attribute__((weak, alias("__mlib_ImageClear")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* do not perform the copying by mlib_s64 data type for _NO_LONGLONG */

#ifdef _NO_LONGLONG

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* _NO_LONGLONG */

#define	TYPE_64BIT	mlib_s64

#endif /* _NO_LONGLONG */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SIZE_IN_BYTES	1
#define	FORMAT	mlib_u8
#define	SFORMAT	mlib_s32
#define	UFORMAT	mlib_u32

#else /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG)

#define	SIZE_IN_BYTES	4
#define	FORMAT	mlib_u32
#define	SFORMAT	mlib_s32
#define	UFORMAT	mlib_u32

#else /* defined(_NO_LONGLONG) */

#define	SIZE_IN_BYTES	8
#define	FORMAT	mlib_u64
#define	SFORMAT	mlib_s64
#define	UFORMAT	mlib_u64

#endif /* defined(_NO_LONGLONG) */

#endif /* _LITTLE_ENDIAN */

#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1

#define	SWAP(c1, c2) {                               \
	UFORMAT tmp = c2; c2 = c1; c1 = tmp;         \
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	PREPARE_CONST()                                                  \
	if (nchannels == 1) {                                            \
	    c01 = (((SFORMAT) (c[0] & 1)) << 31) >> 31;                  \
	} else if (nchannels == 2) {                                     \
	    width = width * 2;                                           \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	} else if (nchannels == 3) {                                     \
	    width = width * 3;                                           \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c01 = (c01 << 2) | (c01 >> 4);                               \
	    c02 = ((c01 << 2) | (c01 >> 4)) & 0xFF;                      \
	    c03 = ((c01 << 1) | (c01 >> 5)) & 0xFF;                      \
	} else {                                                         \
	    width = width * 4;                                           \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	}

#else /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG)

#define	PREPARE_CONST()                                                  \
	if (nchannels == 1) {                                            \
	    c01 = (((SFORMAT) (c[0] & 1)) << 31) >> 31;                  \
	} else if (nchannels == 2) {                                     \
	    width = width * 2;                                           \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	} else if (nchannels == 3) {                                     \
	    width = width * 3;                                           \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c02 = (c01 << 6) | c01;                                      \
	    c02 |= (c02 << 12);                                          \
	    c01 = (c02 << 6) | c01;                                      \
	    c02 = (c01 << 4) | (c01 >> 26);                              \
	    c03 = (c01 << 3) | (c01 >> 27);                              \
	    c01 = (c01 << 2) | (c01 >> 28);                              \
	} else {                                                         \
	    width = width * 4;                                           \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	}

#else /* defined(_NO_LONGLONG) */

#define	PREPARE_CONST()                                                  \
	if (nchannels == 1) {                                            \
	    c01 = (((SFORMAT) (c[0] & 1)) << 63) >> 63;                  \
	} else if (nchannels == 2) {                                     \
	    width = width * 2;                                           \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	    c01 |= (c01 << 32);                                          \
	} else if (nchannels == 3) {                                     \
	    width = width * 3;                                           \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c01 = (c01 << 6) | c01;                                      \
	    c02 = (c01 << 12) | c01;                                     \
	    c02 |= (c02 << 24);                                          \
	    c01 = (c02 << 12) | c01;                                     \
	    c02 = (c01 << 5) | (c01 >> 55);                              \
	    c03 = (c01 << 6) | (c01 >> 54);                              \
	    c01 = (c01 << 4) | (c01 >> 56);                              \
	} else {                                                         \
	    width = width * 4;                                           \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	    c01 |= (c01 << 32);                                          \
	}

#endif /* defined(_NO_LONGLONG) */

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	EDGE_BIT_FIRST(mask, p1, off_p1, p2, off_p2)            \
	{                                                       \
	    mask = 0xFF;                                        \
	    mask >>= off_p1;                                    \
	    if ((mlib_addr)p1 == (mlib_addr)p2) {               \
		mlib_s32 mask1 = 0xFF;                          \
	                                                        \
		mask1 <<= (SIZE_IN_BITS - 1 - off_p2);          \
		mask &= mask1;                                  \
	    }                                                   \
	}

#define	EDGE_BIT_LAST(mask, p1, p2, off_p2)                     \
	{                                                       \
	    mask = 0xFF;                                        \
	    mask <<= (SIZE_IN_BITS - 1 - off_p2);               \
	}

#else /* _LITTLE_ENDIAN */

#define	EDGE_BIT_FIRST(mask, p1, off_p1, p2, off_p2)                    \
	{                                                               \
	    mask = ~((FORMAT) 0);                                       \
	    left = (mlib_addr)p1 & (MASK);                              \
	    right = (MASK);                                             \
	    mask >>= (left * 8 + off_p1);                               \
	    if ((((mlib_addr)p1 ^ (mlib_addr)p2) & (~(MASK))) == 0) {   \
		FORMAT mask1 = ~((FORMAT) 0);                           \
	                                                                \
		right = (mlib_addr)p2 & (MASK);                         \
		mask1 <<= (SIZE_IN_BITS - 1 - (right * 8 + off_p2));    \
		mask &= mask1;                                          \
	    }                                                           \
	}

#define	EDGE_BIT_LAST(mask, p1, p2, off_p2)                       \
	{                                                         \
	    mask = ~((FORMAT) 0);                                 \
	    left = 0;                                             \
	    right = (mlib_addr)p2 & (MASK);                       \
	    mask <<= (SIZE_IN_BITS - 1 - (right * 8 + off_p2));   \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	STORE_MASK(mask, p, a)	*p = (((*p) & (~mask)) | (a & mask))

#else /* _LITTLE_ENDIAN */

#define	STORE_MASK(mask, p, a)                                  \
	mask = (((*p) & (~mask)) | (a & mask));                 \
	mask >>= (((MASK) - right) * 8);                        \
	for (k = right; k >= left; k--) {                       \
	    ((mlib_u8 *)(p))[k] = (mask);                       \
	    mask >>= 8;                                         \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static mlib_status mlib_c_ImageClear_BIT(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_U8_1(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_U8_2(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_U8_3(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_U8_4(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S16_1(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S16_2(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S16_3(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S16_4(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S32_1(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S32_2(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S32_3(
    mlib_image *img,
    const mlib_s32 *color);
static void mlib_c_ImageClear_S32_4(
    mlib_image *img,
    const mlib_s32 *color);

/* *********************************************************** */

mlib_status
__mlib_ImageClear(
    mlib_image *img,
    const mlib_s32 *color)
{
	MLIB_IMAGE_CHECK(img);

	switch (mlib_ImageGetType(img)) {

	case MLIB_BIT:
		return (mlib_c_ImageClear_BIT(img, color));

	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClear_U8_1(img, color);
			break;

		case 2:
			mlib_c_ImageClear_U8_2(img, color);
			break;

		case 3:
			mlib_c_ImageClear_U8_3(img, color);
			break;

		case 4:
			mlib_c_ImageClear_U8_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_USHORT:
	case MLIB_SHORT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClear_S16_1(img, color);
			break;

		case 2:
			mlib_c_ImageClear_S16_2(img, color);
			break;

		case 3:
			mlib_c_ImageClear_S16_3(img, color);
			break;

		case 4:
			mlib_c_ImageClear_S16_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_INT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClear_S32_1(img, color);
			break;

		case 2:
			mlib_c_ImageClear_S32_2(img, color);
			break;

		case 3:
			mlib_c_ImageClear_S32_3(img, color);
			break;

		case 4:
			mlib_c_ImageClear_S32_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	COND1	((mlib_addr)dpp & 3) && i <= (amount - 3 * SIZE_IN_BITS)
#define	COND2	((mlib_addr)dpp & 3) && i <= (amount - SIZE_IN_BITS)

mlib_status
mlib_c_ImageClear_BIT(
    mlib_image *img,
    const mlib_s32 *c)
{
/* pointers for pixel and line of img */
	mlib_u8 *dp, *dl, *dend;
	FORMAT *dpp;
	UFORMAT c01, c02 = 0, c03 = 0;
	UFORMAT c1, c2, c3, emask;
	TYPE_64BIT dcolor0, dcolor1, dcolor2, dc[3];
	mlib_s32 offdst, width, height, nchannels;
	mlib_s32 amount, dend_offset;
	mlib_s32 strided, bitoffsetd;
	mlib_s32 shiftc, shiftc_;
	mlib_s32 i, j;

#ifndef _LITTLE_ENDIAN
	mlib_s32 k, left, right;
#endif /* _LITTLE_ENDIAN */

	dp = (mlib_u8 *)mlib_ImageGetData(img);
	height = mlib_ImageGetHeight(img);
	width = mlib_ImageGetWidth(img);
	strided = mlib_ImageGetStride(img);
	nchannels = mlib_ImageGetChannels(img);
	bitoffsetd = mlib_ImageGetBitOffset(img);

	PREPARE_CONST();

	if (width > strided * 8)
		return (MLIB_FAILURE);

	if (width == strided * 8) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	dend_offset = (bitoffsetd + amount - 1) & 7;
	dl = dp;

	if (nchannels == 3) {

		for (j = 0; j < height; j++) {

			dend = dp + ((bitoffsetd + amount - 1) >> 3);
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			offdst = offdst * 8 + bitoffsetd;

			i = -offdst;
			c1 = c01;
			c2 = c02;
			c3 = c03;

			if (i != 0) {
				shiftc = offdst;
				shiftc_ = (SIZE_IN_BITS) - offdst;
				c1 = (c01 >> shiftc) | (c03 << shiftc_);
				c2 = (c02 >> shiftc) | (c01 << shiftc_);
				c3 = (c03 >> shiftc) | (c02 << shiftc_);
				EDGE_BIT_FIRST(emask, dp, bitoffsetd, dend,
				    dend_offset);
				STORE_MASK(emask, dpp, c1);
#ifdef _LITTLE_ENDIAN
				c1 &= 0xff;
				c2 &= 0xff;
				c3 &= 0xff;
#endif /* _LITTLE_ENDIAN */
				SWAP(c1, c2)
				SWAP(c2, c3)
				i += (SIZE_IN_BITS);
				dpp++;
			}
#ifdef _LITTLE_ENDIAN
			for (; COND1; ) {
				dpp[0] = c1;
				dpp[1] = c2;
				dpp[2] = c3;
				i += (3 * SIZE_IN_BITS);
				dpp += 3;
			}
			c1 |= (c1 << 24) | (c3 << 16) | (c2 << 8);
			c3 |= (c1 << 8);
			c2 |= (c3 << 8);
#endif /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			if (((mlib_addr)dpp & 7) && i <= (amount - 96)) {
				((mlib_s32 *)dpp)[0] = c1;
				((mlib_s32 *)dpp)[1] = c2;
				((mlib_s32 *)dpp)[2] = c3;
				i += 96;
				dpp = (FORMAT *) (((mlib_s32 *)dpp) + 3);
			}
			((SFORMAT *) dc)[3] = c1;
			((SFORMAT *) dc)[4] = c2;
			((SFORMAT *) dc)[5] = c3;
#endif /* defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN) */

			((SFORMAT *) dc)[0] = c1;
			((SFORMAT *) dc)[1] = c2;
			((SFORMAT *) dc)[2] = c3;
			dcolor0 = dc[0];
			dcolor1 = dc[1];
			dcolor2 = dc[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - 192; i += 192) {
				((TYPE_64BIT *) dpp)[0] = dcolor0;
				((TYPE_64BIT *) dpp)[1] = dcolor1;
				((TYPE_64BIT *) dpp)[2] = dcolor2;
				dpp = (FORMAT *) (((TYPE_64BIT *) dpp) + 3);
			}

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			if (i <= (amount - 96)) {
				((mlib_s32 *)dpp)[0] = c1;
				((mlib_s32 *)dpp)[1] = c2;
				((mlib_s32 *)dpp)[2] = c3;
				i += 96;
				dpp = (FORMAT *) (((mlib_s32 *)dpp) + 3);
			}
#endif /* defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN) */

#ifdef _LITTLE_ENDIAN
			for (; i <= (amount - 3 * SIZE_IN_BITS); ) {
				dpp[0] = c1;
				dpp[1] = c2;
				dpp[2] = c3;
				i += (3 * SIZE_IN_BITS);
				dpp += 3;
			}
#endif /* _LITTLE_ENDIAN */

			if (i < amount) {

				if (i <= (amount - SIZE_IN_BITS)) {
					dpp[0] = c1;
					i += (SIZE_IN_BITS);
					dpp++;

					if (i <= (amount - SIZE_IN_BITS)) {
						dpp[0] = c2;
						i += (SIZE_IN_BITS);
						dpp++;
						c1 = c3;
					} else
						c1 = c2;
				}
				if (i < amount) {
					EDGE_BIT_LAST(emask, dpp, dend,
					    dend_offset);
					STORE_MASK(emask, dpp, c1);
				}
			}

			dp = dl += strided;
		}
	} else {

		c01 =
		    (c01 >> bitoffsetd) | (c01 << (sizeof (UFORMAT) * 8 -
		    bitoffsetd));

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
		((SFORMAT *) dc)[1] = c01;
#endif /* defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN) */

		((SFORMAT *) dc)[0] = c01;
		dcolor0 = dc[0];

		for (j = 0; j < height; j++) {

			dend = dp + ((bitoffsetd + amount - 1) >> 3);
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			offdst = offdst * 8 + bitoffsetd;

			i = -offdst;

			if (i != 0) {
				EDGE_BIT_FIRST(emask, dp, bitoffsetd, dend,
				    dend_offset);
				STORE_MASK(emask, dpp, c01);
				i += (SIZE_IN_BITS);
				dpp++;
			}
#ifdef _LITTLE_ENDIAN
			for (; COND2; ) {
				dpp[0] = c01;
				i += (SIZE_IN_BITS);
				dpp++;
			}
#endif /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			if (((mlib_addr)dpp & 7) && i <= (amount - 32)) {
				((mlib_s32 *)dpp)[0] = c01;
				i += 32;
				dpp = (FORMAT *) (((mlib_s32 *)dpp) + 1);
			}
#endif /* defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN) */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (amount - 64); i += 64) {
				((TYPE_64BIT *) dpp)[0] = dcolor0;
				dpp = (FORMAT *) (((TYPE_64BIT *) dpp) + 1);
			}

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			if (i <= (amount - 32)) {
				((mlib_s32 *)dpp)[0] = c01;
				i += 32;
				dpp = (FORMAT *) (((mlib_s32 *)dpp) + 1);
			}
#endif /* defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN) */

#ifdef _LITTLE_ENDIAN
			for (; i <= (amount - SIZE_IN_BITS); ) {
				dpp[0] = c01;
				i += (SIZE_IN_BITS);
				dpp++;
			}
#endif /* _LITTLE_ENDIAN */

			if (i < amount) {
				EDGE_BIT_LAST(emask, dpp, dend, dend_offset);
				STORE_MASK(emask, dpp, c01);
			}

			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PREPAREVARS(type, chan)                                           \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    TYPE_64BIT                                                    \
		dcolor,                                                   \
		dc[1];                                                    \
	                                                                  \
	if ((img_width * chan) == img_stride)                             \
	{                                                                 \
	img_width *= img_height;                                          \
	img_height = 1;                                                   \
	}

/* *********************************************************** */

#define	STRIP(pd, color, w, h, chan, data_type)                    \
	for (l = 0; l < chan; l++) {                               \
	    data_type color_i = color[l];                          \
	                                                           \
	    for (i = 0; i < h; i++) {                              \
		for (j = 0; j < w; j++)                            \
		    pd[i * img_stride + l + j * chan] = color_i;   \
	    }                                                      \
	}

/* *********************************************************** */

void
mlib_c_ImageClear_U8_1(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF;

	PREPAREVARS(mlib_u8,
	    1);

	if (img_width < 16) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_u8);

		return;
	}

	color0 |= (color0 << 8);
	color0 |= (color0 << 16);
	((mlib_s32 *)dc)[0] = ((mlib_s32 *)dc)[1] = color0;
	dcolor = dc[0];

	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (8 - (mlib_addr)pimg_row) & 7;

		for (j = 0; j < align; j++)
			pimg_row[j] = color0;

		for (; j <= (img_width - 8); j += 8)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;

		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_U8_2(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF, color1 = color[1] & 0xFF;
	mlib_u32 col, col0, col1;

	PREPAREVARS(mlib_u8,
	    2);

	if (img_width < 8) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_u8);

		return;
	}
#ifdef _LITTLE_ENDIAN
	col1 = (color0 << 8) | color1;
	col1 |= (col1 << 16);
	col0 = (col1 << 8) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 8) | color1;
	col0 |= (col0 << 16);
	col1 = (col0 << 8) | color0;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (8 - (mlib_addr)pimg_row) & 7;

		col = (align & 1) ? col1 : col0;
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}

		for (j = align; j <= (img_width * 2 - 8); j += 8) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (align & 1) {
			for (; j < (img_width * 2 - 1); j += 2) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color0;
			}

			pimg_row[j] = color1;
		} else {
			for (; j < img_width * 2; j += 2) {
				*((mlib_s16 *)(pimg_row + j)) = col0;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_U8_3(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF;
	mlib_u32 color1 = color[1] & 0xFF;
	mlib_u32 color2 = color[2] & 0xFF;
	mlib_u32 col0, col1, col2;
	TYPE_64BIT dcolor1, dcolor2;

	PREPAREVARS(mlib_u8,
	    3);

	if (img_width < 16) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_u8);

		return;
	}
#ifdef _LITTLE_ENDIAN
	col2 = (color2 << 16) | (color1 << 8) | color0;
	col2 = (col2 << 8) | color2;
	col1 = (col2 << 8) | color1;
	col0 = (col1 << 8) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | (color1 << 8) | color2;
	col0 = (col0 << 8) | color0;
	col1 = (col0 << 8) | color1;
	col2 = (col1 << 8) | color2;
#endif /* _LITTLE_ENDIAN */

	((mlib_u32 *)dc)[0] = col0;
	((mlib_u32 *)dc)[1] = col1;
	dcolor = dc[0];
	((mlib_u32 *)dc)[0] = col2;
	((mlib_u32 *)dc)[1] = col0;
	dcolor1 = dc[0];
	((mlib_u32 *)dc)[0] = col1;
	((mlib_u32 *)dc)[1] = col2;
	dcolor2 = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;

		for (j = 0; ((mlib_addr)(pimg_row + j) & 7) != 0; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		for (; j <= (img_width * 3 - 24); j += 24) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 8)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 16)) = dcolor2;
		}

		for (; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_U8_4(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF, color1 = color[1] & 0xFF;
	mlib_u32 color2 = color[2] & 0xFF, color3 = color[3] & 0xFF;
	mlib_u32 col, col0, col1, col2, col3;

	PREPAREVARS(mlib_u8,
	    4);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_u8);

		return;
	}
#ifdef _LITTLE_ENDIAN
	col0 = (color3 << 24) | (color2 << 16) | (color1 << 8) | color0;
	col3 = (col0 << 8) | color3;
	col2 = (col3 << 8) | color2;
	col1 = (col2 << 8) | color1;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 24) | (color1 << 16) | (color2 << 8) | color3;
	col1 = (col0 << 8) | color0;
	col2 = (col1 << 8) | color1;
	col3 = (col2 << 8) | color2;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (8 - (mlib_addr)pimg_row) & 7;

		col =
		    (align & 2) ? ((align & 1) ? col3 : col2) : ((align & 1) ?
		    col1 : col0);
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}

		for (j = align; j <= (img_width * 4 - 8); j += 8) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (j <= (img_width * 4 - 4)) {
			*((mlib_s32 *)(pimg_row + j)) = col;
			j += 4;
		}

		if (align & 2) {
			if (!(align & 1))
				pimg_row[j++] = color2;
			pimg_row[j] = color3;
		} else {
			if (align & 1) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color2;
				pimg_row[j + 2] = color3;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S16_1(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF;

	PREPAREVARS(mlib_s16,
	    1);

	if (img_width < 8) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_s16);

		return;
	}

	color0 |= (color0 << 16);
	((mlib_s32 *)dc)[0] = ((mlib_s32 *)dc)[1] = color0;
	dcolor = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		for (j = 0; j < align; j++)
			pimg_row[j] = color0;
		for (; j <= (img_width - 4); j += 4)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S16_2(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF;
	mlib_u32 color1 = color[1] & 0xFFFF;
	mlib_u32 col, col0, col1;

	PREPAREVARS(mlib_s16,
	    2);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_s16);

		return;
	}
#ifdef _LITTLE_ENDIAN
	col1 = (color0 << 16) | color1;
	col0 = (color1 << 16) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | color1;
	col1 = (color1 << 16) | color0;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		col = (align & 1) ? col1 : col0;
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}

		for (j = align; j <= (img_width * 2 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (align & 1) {
			if (j < (img_width * 2 - 1)) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color0;
				j += 2;
			}

			pimg_row[j] = color1;
		} else {
			if (j < img_width * 2)
				*((mlib_s32 *)(pimg_row + j)) = col0;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S16_3(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF;
	mlib_u32 color1 = color[1] & 0xFFFF;
	mlib_u32 color2 = color[2] & 0xFFFF;
	mlib_u32 col0, col1, col2;
	TYPE_64BIT dcolor1, dcolor2;

	PREPAREVARS(mlib_s16,
	    3);

	if (img_width < 8) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_s16);

		return;
	}
#ifdef _LITTLE_ENDIAN
	col2 = (color2 << 16) | color1;
	col1 = (color0 << 16) | color2;
	col0 = (color1 << 16) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | color1;
	col1 = (color2 << 16) | color0;
	col2 = (color1 << 16) | color2;
#endif /* _LITTLE_ENDIAN */

	((mlib_u32 *)dc)[0] = col0;
	((mlib_u32 *)dc)[1] = col1;
	dcolor = dc[0];
	((mlib_u32 *)dc)[0] = col2;
	((mlib_u32 *)dc)[1] = col0;
	dcolor1 = dc[0];
	((mlib_u32 *)dc)[0] = col1;
	((mlib_u32 *)dc)[1] = col2;
	dcolor2 = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;

		for (j = 0; ((mlib_addr)(pimg_row + j) & 7) != 0; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		for (; j <= (img_width * 3 - 12); j += 12) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 4)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 8)) = dcolor2;
		}

		for (; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S16_4(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF, color1 = color[1] & 0xFFFF;
	mlib_u32 color2 = color[2] & 0xFFFF, color3 = color[3] & 0xFFFF;
	mlib_u32 col0, col1, col2, col3, col00, col11;

	PREPAREVARS(mlib_s16,
	    4);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_s16);

		return;
	}
#ifdef _LITTLE_ENDIAN
	col2 = (color3 << 16) | color2;
	col1 = (color2 << 16) | color1;
	col0 = (color1 << 16) | color0;
	col3 = (color0 << 16) | color3;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | color1;
	col1 = (color1 << 16) | color2;
	col2 = (color2 << 16) | color3;
	col3 = (color3 << 16) | color0;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		if (align & 2)
			if (align & 1) {
				col00 = col3;
				col11 = col1;
			} else {
				col00 = col2;
				col11 = col0;
		} else {
			if (align & 1) {
				col00 = col1;
				col11 = col3;
			} else {
				col00 = col0;
				col11 = col2;
			}
		}

		((mlib_u32 *)dc)[0] = col00;
		((mlib_u32 *)dc)[1] = col11;
		dcolor = dc[0];
		for (j = 0; j < align; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}

		for (j = align; j <= (img_width * 4 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (j <= (img_width * 4 - 2)) {
			*((mlib_s32 *)(pimg_row + j)) = col00;
			j += 2;
		}

		if (align & 1)
			pimg_row[j] = color3;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S32_1(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0];

	PREPAREVARS(mlib_s32,
	    1);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_s32);

		return;
	}

	((mlib_s32 *)dc)[0] = ((mlib_s32 *)dc)[1] = color0;
	dcolor = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align)
			pimg_row[j++] = color0;
		for (; j <= (img_width - 2); j += 2)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S32_2(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1];

	PREPAREVARS(mlib_s32,
	    2);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_s32);

		return;
	}

	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_u32 *)dc)[0] = color1;
			((mlib_u32 *)dc)[1] = color0;
			pimg_row[j + img_width * 2 - 1] = color1;
			j++;
		} else {
			((mlib_u32 *)dc)[0] = color0;
			((mlib_u32 *)dc)[1] = color1;
		}

		dcolor = dc[0];
		for (; j <= (img_width * 2 - 2); j += 2) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S32_3(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1], color2 = color[2];
	TYPE_64BIT dcolor1, dcolor2;

	PREPAREVARS(mlib_s32,
	    3);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_s32);

		return;
	}

	((mlib_u32 *)dc)[0] = color0;
	((mlib_u32 *)dc)[1] = color1;
	dcolor = dc[0];
	((mlib_u32 *)dc)[0] = color2;
	((mlib_u32 *)dc)[1] = color0;
	dcolor1 = dc[0];
	((mlib_u32 *)dc)[0] = color1;
	((mlib_u32 *)dc)[1] = color2;
	dcolor2 = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			j += 3;
		}

		for (; j <= (img_width * 3 - 6); j += 6) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 2)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 4)) = dcolor2;
		}

		if (j < img_width * 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClear_S32_4(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1];
	mlib_u32 color2 = color[2], color3 = color[3];
	TYPE_64BIT dcolor1, dc1[1];

	PREPAREVARS(mlib_s32,
	    4);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_s32);

		return;
	}

	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_u32 *)dc)[0] = color1;
			((mlib_u32 *)dc)[1] = color2;
			((mlib_u32 *)dc1)[0] = color3;
			((mlib_u32 *)dc1)[1] = color0;
			*((TYPE_64BIT *) (pimg_row + j + img_width * 4 - 3)) =
			    dc[0];
			pimg_row[j + img_width * 4 - 1] = color3;
			j++;
		} else {
			((mlib_u32 *)dc)[0] = color0;
			((mlib_u32 *)dc)[1] = color1;
			((mlib_u32 *)dc1)[0] = color2;
			((mlib_u32 *)dc1)[1] = color3;
		}

		dcolor = dc[0];
		dcolor1 = dc1[0];
		for (; j <= (img_width * 4 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j) + 1) = dcolor1;
		}
	}
}

/* *********************************************************** */
