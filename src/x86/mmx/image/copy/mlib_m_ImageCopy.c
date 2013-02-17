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

#pragma ident	"@(#)mlib_m_ImageCopy.c	9.6	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopy - Direct copy from one image to another.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopy(mlib_image       *dst,
 *                                 const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output or destination image
 *      src     pointer to input or source image
 *
 * RESTRICTION
 *      src and dst must have the same size, type and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Direct copy from one image to another
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopy = __mlib_ImageCopy

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageCopy) mlib_ImageCopy
    __attribute__((weak, alias("__mlib_ImageCopy")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    static void mlib_ImageCopy_u8(
    const mlib_image *src,
    mlib_image *dst);
    static void mlib_ImageCopy_s16(
    const mlib_image *src,
    mlib_image *dst);
    static void mlib_ImageCopy_s32(
    const mlib_image *src,
    mlib_image *dst);
    static void mlib_ImageCopy_f32(
    const mlib_image *src,
    mlib_image *dst);
    static void mlib_ImageCopy_d64(
    const mlib_image *src,
    mlib_image *dst);

/* *********************************************************** */

mlib_status
__mlib_ImageCopy(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 s_offset, d_offset, width, height;
	mlib_s32 size, s_stride, d_stride;
	mlib_u8 *sa, *da;
	mlib_s32 j;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	switch (mlib_ImageGetType(dst)) {
	case MLIB_BIT:

		sa = (mlib_u8 *)mlib_ImageGetData(src);
		da = (mlib_u8 *)mlib_ImageGetData(dst);

		width = mlib_ImageGetWidth(src) * mlib_ImageGetChannels(src);
		height = mlib_ImageGetHeight(src);

		if (!mlib_ImageIsNotOneDvector(src) &&
		    !mlib_ImageIsNotOneDvector(dst)) {
			size = height * (width >> 3);
			mlib_ImageCopy_na(sa, da, size);
		} else {
/* in byte */
			s_stride = mlib_ImageGetStride(src);
/* in byte */
			d_stride = mlib_ImageGetStride(dst);
/* in bits */
			s_offset = mlib_ImageGetBitOffset(src);
/* in bits */
			d_offset = mlib_ImageGetBitOffset(dst);

			if (s_offset == d_offset) {
				for (j = 0; j < height; j++) {
					mlib_ImageCopy_bit_al(sa, da, width,
					    s_offset);
					sa += s_stride;
					da += d_stride;
				}
			} else {
				for (j = 0; j < height; j++) {
					mlib_ImageCopy_bit_na(sa, da, width,
					    s_offset, d_offset);
					sa += s_stride;
					da += d_stride;
				}
			}
		}

		break;

	case MLIB_BYTE:
		mlib_ImageCopy_u8(src, dst);
		break;

	case MLIB_SHORT:
	case MLIB_USHORT:
		mlib_ImageCopy_s16(src, dst);
		break;

	case MLIB_INT:
	case MLIB_FLOAT:
		mlib_ImageCopy_s32(src, dst);
		break;

	case MLIB_DOUBLE:
		mlib_ImageCopy_d64(src, dst);
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PREPARE_VARS(type)                                                \
	mlib_s32 i, j;                                                    \
	                                                                  \
/*                                                                        \
 * indices                                                                \
 */                                                                       \
	type *psrc = (type *) mlib_ImageGetData(src);                     \
	                                                                  \
/*                                                                        \
 * pointer to the data of src-image                                       \
 */                                                                       \
	type *pdst = (type *) mlib_ImageGetData(dst);                     \
	                                                                  \
/*                                                                        \
 * pointer to the data of dst-image                                       \
 */                                                                       \
	mlib_s32 src_height = mlib_ImageGetHeight(src);                   \
	                                                                  \
/*                                                                        \
 * height of source image                                                 \
 */                                                                       \
	mlib_s32 src_width = mlib_ImageGetWidth(src);                     \
	                                                                  \
/*                                                                        \
 * width of source image                                                  \
 */                                                                       \
	mlib_s32 src_stride = mlib_ImageGetStride(src) / sizeof (type);   \
	                                                                  \
/*                                                                        \
 * elements to next row                                                   \
 */                                                                       \
	    mlib_s32                                                      \
		dst_stride = mlib_ImageGetStride(dst) / sizeof (type);    \
	                                                                  \
/*                                                                        \
 * elements to next row                                                   \
 */                                                                       \
	    mlib_s32                                                      \
		channels = mlib_ImageGetChannels(dst);                    \
	                                                                  \
/*                                                                        \
 * number of channels                                                     \
 */                                                                       \
	src_width *= channels;                                            \
	if (src_width == src_stride && src_width == dst_stride)           \
	{                                                                 \
	src_width *= src_height;                                          \
	src_height = 1;                                                   \
	}

/* *********************************************************** */

#define	STRIP(pdst, psrc, w, h, data_type)                         \
	{                                                          \
	    data_type s0, s1;                                      \
	                                                           \
	    for (i = 0; i < h; i++) {                              \
		if (j = w & 1)                                     \
		    pdst[i * dst_stride] = psrc[i * src_stride];   \
		for (; j < w; j += 2) {                            \
		    s0 = psrc[i * src_stride + j];                 \
		    s1 = psrc[i * src_stride + j + 1];             \
		    pdst[i * dst_stride + j] = s0;                 \
		    pdst[i * dst_stride + j + 1] = s1;             \
		}                                                  \
	    }                                                      \
	}

/* *********************************************************** */

void
mlib_ImageCopy_u8(
    const mlib_image *src,
    mlib_image *dst)
{
	PREPARE_VARS(mlib_u8);

	if (src_width < 16) {
		STRIP(pdst, psrc, src_width, src_height, mlib_u8);

		return;
	}

	for (i = 0; i < src_height; i++) {
		mlib_u8 *srow = psrc + i * src_stride;
		mlib_u8 *drow = pdst + i * dst_stride;

		mlib_ImageCopy_na(srow, drow, src_width);
	}
}

/* *********************************************************** */

void
mlib_ImageCopy_s16(
    const mlib_image *src,
    mlib_image *dst)
{
	PREPARE_VARS(mlib_u16);

	if (src_width < 8) {
		STRIP(pdst, psrc, src_width, src_height, mlib_u16);

		return;
	}

	for (i = 0; i < src_height; i++) {
		mlib_u8 *srow = (mlib_u8 *)(psrc + i * src_stride);
		mlib_u8 *drow = (mlib_u8 *)(pdst + i * dst_stride);

		mlib_ImageCopy_na(srow, drow, src_width << 1);
	}
}

/* *********************************************************** */

void
mlib_ImageCopy_s32(
    const mlib_image *src,
    mlib_image *dst)
{
	PREPARE_VARS(mlib_u32);

	if (src_width < 4) {
		STRIP(pdst, psrc, src_width, src_height, mlib_u32);

		return;
	}

	for (i = 0; i < src_height; i++) {
		mlib_u8 *srow = (mlib_u8 *)(psrc + i * src_stride);
		mlib_u8 *drow = (mlib_u8 *)(pdst + i * dst_stride);

		mlib_ImageCopy_na(srow, drow, src_width << 2);
	}
}

/* *********************************************************** */

void
mlib_ImageCopy_d64(
    const mlib_image *src,
    mlib_image *dst)
{
	PREPARE_VARS(mlib_d64);

	for (i = 0; i < src_height; i++) {
		mlib_u8 *srow = (mlib_u8 *)(psrc + i * src_stride);
		mlib_u8 *drow = (mlib_u8 *)(pdst + i * dst_stride);

		mlib_ImageCopy_na(srow, drow, src_width << 3);
	}
}

/* *********************************************************** */
/*
 * Both bit offsets of source and destination are the same
 */

void
mlib_ImageCopy_bit_al(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset)
{
	mlib_s32 b_size, i, j;
	mlib_u8 mask0 = 0xFF;
	mlib_u8 src, mask;

	if (size <= (8 - offset)) {
		mask = mask0 << (8 - size);
		mask >>= offset;
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
		return;
	}

	mask = mask0 >> offset;
	src = da[0];
	da[0] = (src & (~mask)) | (sa[0] & mask);
	da++;
	sa++;
	size = size - 8 + offset;
/* size in bytes */
	b_size = size >> 3;

	for (j = 0; (j < b_size) && (((mlib_addr)da & 7) != 0); j++)
		(*da++) = (*sa++);

	if ((((mlib_addr)sa & 7) ^ ((mlib_addr)da & 7)) == 0) {
		__m64 *sp = (__m64 *) sa;
		__m64 *dp = (__m64 *) da;

		for (i = 0; j <= (b_size - 8); j += 8, i++) {
			dp[i] = sp[i];
		}

		sa += i << 3;
		da += i << 3;
	} else {
		if ((((mlib_addr)sa & 3) ^ ((mlib_addr)da & 3)) == 0) {
			mlib_u32 *pws, *pwd;

			pws = (mlib_u32 *)sa;
			pwd = (mlib_u32 *)da;

			for (i = 0; j <= (b_size - 4); j += 4, i++) {
				pwd[i] = pws[i];
			}

			sa += i << 2;
			da += i << 2;
		} else {
			mlib_u32 *pws, *pwd, src0, src1;
			mlib_s32 lshift = (mlib_addr)sa & 3, rshift;

			pwd = (mlib_u32 *)da;
			pws = (mlib_u32 *)(sa - lshift);
			lshift <<= 3;
			rshift = 32 - lshift;

			src1 = pws[0];

			for (i = 0; j <= (b_size - 4); j += 4, i++) {
				src0 = src1;
				src1 = pws[i + 1];
				pwd[i] = (src0 >> lshift) | (src1 << rshift);
			}

			sa += i << 2;
			da += i << 2;
		}
	}

	for (; j < b_size; j++)
		(*da++) = (*sa++);

	j = size & 7;

	if (j > 0) {
		mask = mask0 << (8 - j);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
	}

	_mm_empty();
}

/* *********************************************************** */
