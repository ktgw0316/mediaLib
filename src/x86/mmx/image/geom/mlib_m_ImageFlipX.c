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

#pragma ident	"@(#)mlib_m_ImageFlipX.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageFlipX - flip image on X axis
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipX(mlib_image *dst, const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     pointer to an dst image
 *      src     pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipX = __mlib_ImageFlipX

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageFlipX) mlib_ImageFlipX
    __attribute__((weak, alias("__mlib_ImageFlipX")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_c_ImageFlipX_BIT(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageFlipX_U8(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageFlipX_S16(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_ImageFlipX_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageFlipX(
    mlib_image *dst,
    const mlib_image *src)
{

/*  check for obvious errors  */

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	switch (mlib_ImageGetType(dst)) {

/*  handle MLIB_BIT data type of image  */

	case MLIB_BIT:
		mlib_c_ImageFlipX_BIT(dst, src);
		break;

/*  handle MLIB_BYTE data type of image  */

	case MLIB_BYTE:
		mlib_c_ImageFlipX_U8(dst, src);
		break;

/*  handle MLIB_SHORT data type of image  */

	case MLIB_SHORT:
	case MLIB_USHORT:
		mlib_c_ImageFlipX_S16(dst, src);
		break;

/*  handle MLIB_INT data type of image  */

	case MLIB_INT:
		mlib_ImageFlipX_S32(dst, src);
		break;

/*  discard any other data types  */

	default:
		return (MLIB_FAILURE);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PREPAREVARS(type)                                                 \
	type *psrc = (type *) mlib_ImageGetData(src);                     \
	type *pdst = (type *) mlib_ImageGetData(dst);                     \
	mlib_s32 src_height = mlib_ImageGetHeight(src);                   \
	mlib_s32 dst_height = mlib_ImageGetHeight(dst);                   \
	mlib_s32 src_width = mlib_ImageGetWidth(src);                     \
	mlib_s32 dst_width = mlib_ImageGetWidth(dst);                     \
	mlib_s32 src_stride = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                                      \
		dst_stride = mlib_ImageGetStride(dst) / sizeof (type);    \
	    mlib_s32                                                      \
		chan = mlib_ImageGetChannels(dst);                        \
	    mlib_s32                                                      \
		i,                                                        \
		j;                                                        \
	                                                                  \
	if (src_height > dst_height)                                      \
	{                                                                 \
	psrc += ((src_height - dst_height + 1) >> 1) * src_stride;        \
	src_height = dst_height;                                          \
	} else                                                            \
	    pdst += ((dst_height - src_height + 1) >> 1) * dst_stride;    \
	if (src_width > dst_width) {                                      \
	    psrc += ((src_width - dst_width + 1) >> 1) * chan;            \
	    src_width = dst_width;                                        \
	} else                                                            \
	    pdst += ((dst_width - src_width) >> 1) * chan;                \
	src_width *= chan;
/* *********************************************************** */

#define	STRIP(pd, ps, w, h, data_type)                                    \
	{                                                                 \
	    data_type s0, s1;                                             \
	                                                                  \
	    for (i = 0; i < h; i++) {                                     \
		if (j = w & 1)                                            \
		    pd[i * dst_stride] =                                  \
			ps[(src_height - 1 - i) * src_stride];            \
		for (; j < w; j += 2) {                                   \
		    s0 = ps[(src_height - 1 - i) * src_stride + j];       \
		    s1 = ps[(src_height - 1 - i) * src_stride + j + 1];   \
		    pd[i * dst_stride + j] = s0;                          \
		    pd[i * dst_stride + j + 1] = s1;                      \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

void
mlib_c_ImageFlipX_BIT(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(src);
	mlib_u8 *pdst = (mlib_u8 *)mlib_ImageGetData(dst);
	mlib_s32 src_height = mlib_ImageGetHeight(src);
	mlib_s32 dst_height = mlib_ImageGetHeight(dst);
	mlib_s32 src_width = mlib_ImageGetWidth(src);
	mlib_s32 dst_width = mlib_ImageGetWidth(dst);
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 dst_stride = mlib_ImageGetStride(dst);
	mlib_s32 chan = mlib_ImageGetChannels(dst);
	mlib_s32 j;
	mlib_s32 src_bitoffset = mlib_ImageGetBitOffset(src);
	mlib_s32 dst_bitoffset = mlib_ImageGetBitOffset(dst);

	if (src_height > dst_height) {
		psrc += ((src_height - dst_height + 1) >> 1) * src_stride;
		src_height = dst_height;
	} else
		pdst += ((dst_height - src_height + 1) >> 1) * dst_stride;

	if (src_width > dst_width) {
		psrc +=
		    (((src_width - dst_width + 1) >> 1) * chan +
		    src_bitoffset) / 8;
		src_bitoffset =
		    (((src_width - dst_width + 1) >> 1) * chan +
		    src_bitoffset) & 7;
		src_width = dst_width;
	} else {
		pdst +=
		    (((dst_width - src_width) >> 1) * chan + dst_bitoffset) / 8;
		dst_bitoffset =
		    (((dst_width - src_width) >> 1) * chan + dst_bitoffset) & 7;
	}

	src_width *= chan;
	psrc += (src_height - 1) * src_stride;

	if (src_bitoffset != dst_bitoffset) {
		for (j = 0; j < src_height; j++) {

			mlib_ImageCopy_bit_na(psrc, pdst, src_width,
			    src_bitoffset, dst_bitoffset);

			pdst += dst_stride;
			psrc -= src_stride;
		}
	} else {
		for (j = 0; j < src_height; j++) {

			mlib_ImageCopy_bit_al(psrc, pdst, src_width,
			    dst_bitoffset);
			pdst += dst_stride;
			psrc -= src_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageFlipX_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8);

	if (src_width < 16) {
		STRIP(pdst, psrc, src_width, src_height, mlib_u8);

		return;
	}

	psrc += (src_height - 1) * src_stride;

	for (i = 0; i < src_height; i++) {
		mlib_ImageCopy_na((void *)psrc, (void *)pdst, src_width);
		psrc -= src_stride;
		pdst += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageFlipX_S16(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16);

	if (src_width < 16) {
		STRIP(pdst, psrc, src_width, src_height, mlib_u16);

		return;
	}

	psrc += (src_height - 1) * src_stride;

	for (i = 0; i < src_height; i++) {
		mlib_ImageCopy_na((void *)psrc, (void *)pdst, 2 * src_width);
		psrc -= src_stride;
		pdst += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipX_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32);

	if (src_width < 16) {
		STRIP(pdst, psrc, src_width, src_height, mlib_u32);

		return;
	}

	psrc += (src_height - 1) * src_stride;

	for (i = 0; i < src_height; i++) {
		mlib_ImageCopy_na((void *)psrc, (void *)pdst, 4 * src_width);
		psrc -= src_stride;
		pdst += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipX_D64(
    mlib_image *dst,
    mlib_image *src)
{
	PREPAREVARS(mlib_d64);

	if (src_width < 16) {
		STRIP(pdst, psrc, src_width, src_height, mlib_d64);

		return;
	}

	psrc += (src_height - 1) * src_stride;

	for (i = 0; i < src_height; i++) {
		mlib_ImageCopy_na((void *)psrc, (void *)pdst, 8 * src_width);
		psrc -= src_stride;
		pdst += dst_stride;
	}
}

/* *********************************************************** */
