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

#pragma ident   "@(#)mlib_ImageDivAlpha.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivAlpha - divide color channels by the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivAlpha(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         cmask)
 *
 * ARGUMENTS
 *  dst     Pointer to destination image.
 *  src     Pointer to source image.
 *  cmask   Channel mask to indicate the alpha channel.
 *          Each bit of which represents a channel in the image. The
 *          channel corresponded to the 1 bit is the alpha channel.
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Divide color channels by the alpha channel in the way of
 *      pixel by pixel.
 *
 * The image must have at least two channels.
 *
 * For an MLIB_BYTE image
 * 		dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -8)
 * For an MLIB_SHORT image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -15)
 * For an MLIB_USHORT image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -16)
 * For an MLIB_INT image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -31)
 *  where, j != a
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageVisVersion.h>
#include <mlib_ImageDivAlpha_f.h>

#ifndef _NO_LONGLONG
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivAlpha = __mlib_ImageDivAlpha

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivAlpha) mlib_ImageDivAlpha
    __attribute__((weak, alias("__mlib_ImageDivAlpha")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CONST_2P(sh)	(1 << (sh))

/* *********************************************************** */

#define	SAT_S16(DST, SRC)                                     \
	{                                                       \
	    mlib_s64 dd = SRC;                                  \
	                                                        \
	    if (dd >= MLIB_S16_MAX)                             \
		dd = MLIB_S16_MAX;                                  \
	    if (dd <= MLIB_S16_MIN)                             \
		dd = MLIB_S16_MIN;                                  \
	    DST = (mlib_s32)dd;                                 \
	}

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                   \
	{                                                       \
	    mlib_s64 dd = SRC;                                  \
	                                                        \
	    if (dd >= MLIB_U16_MAX)                             \
		dd = MLIB_U16_MAX;                                  \
	    if (dd <= MLIB_U16_MIN)                             \
		dd = MLIB_U16_MIN;                                  \
	    DST = (mlib_s32)dd;                                           \
	}

/* *********************************************************** */

#define	SAT_S32(DST, SRC)                                     \
	{                                                       \
	    mlib_s64 dd = SRC;                                  \
	                                                        \
	    if (dd >= MLIB_S32_MAX)                             \
		dd = MLIB_S32_MAX;                                  \
	    if (dd <= MLIB_S32_MIN)                             \
		dd = MLIB_S32_MIN;                                  \
	    DST = (mlib_s32)dd;                                           \
	}

/* *********************************************************** */

#define	SHIFT_S16(y, x)	\
	{	\
		if (x >= 0) {	\
			y = x >> 16;	\
		} else {	\
			y = -((-x) >> 16);	\
		}	\
	}
/* *********************************************************** */

static void mlib_ImageDivAlpha_S16_2(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S16_4(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U16_4(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

/* *********************************************************** */


mlib_status
__mlib_ImageDivAlpha(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 alpha, t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, channel, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if ((channel < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; !(cmask & 0x1); alpha++)
		cmask >>= 1;

	if (cmask >> 1)
		return (MLIB_OUTOFRANGE);

	alpha = channel - alpha - 1;

	if (alpha < 0 || alpha >= channel)
		return (MLIB_OUTOFRANGE);

	if (type == MLIB_BYTE) {
		t_sh = 0;
	} else if (type == MLIB_INT) {
		t_sh = 2;
	} else {
		t_sh = 1;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == channel * width) {
		mlib_s32 algn = 0;

		if (mlib_ImageVisVersion() && type == MLIB_BYTE)
			algn = 7;

/* for VIS 1-D vector only if aligned */
		if (!(((int)sdata | (int)ddata) & algn)) {
			width *= height;
			height = 1;
		}
	}

	switch (type) {
	case MLIB_BYTE:
		return mlib_ImageDivAlpha_U8(sdata, sstride, ddata, dstride,
		    width, height, channel, alpha);

	case MLIB_SHORT:

		if (channel == 4) {
			mlib_ImageDivAlpha_S16_4(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else if (channel == 3) {
			mlib_ImageDivAlpha_S16_3(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else {
/* if (channel == 2) */

			mlib_ImageDivAlpha_S16_2(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);

	case MLIB_USHORT:

		if (channel == 4) {
			mlib_ImageDivAlpha_U16_4(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else if (channel == 3) {
			mlib_ImageDivAlpha_U16_3(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else {
/* if (channel == 2) */

			mlib_ImageDivAlpha_U16_2(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);

	case MLIB_INT:

		if (channel == 4) {
			mlib_ImageDivAlpha_S32_4(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else if (channel == 3) {
			mlib_ImageDivAlpha_S32_3(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else {
/* if (channel == 2) */

			mlib_ImageDivAlpha_S32_2(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S16_2(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1;
	mlib_s64 d_0, d_1, v_0, v_1;
	mlib_s64 scale = CONST_2P(15);

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				v_1 = (scale << 32) * s_1
						/ (((mlib_s64)s_0 << 16) + 1);

				SHIFT_S16(d_1, v_1);
				SAT_S16(dst[1], d_1);

				dst[0] = s_0;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				v_0 = (scale << 32) * s_0
						/(((mlib_s64)s_1 << 16)	+ 1);

				SHIFT_S16(d_0, v_0);
				SAT_S16(dst[0], d_0);

				dst[1] = s_1;

				dst += 2;
				src += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1;
	mlib_s64 d_0, d_1;
	mlib_s64 scale = CONST_2P(16);

	s_stride -= width << 1; d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				d_1 = (scale << 31) * s_1
						/(((mlib_s64)s_0 << 16)
						+ 1) >> 15;

				SAT_U16(dst[1], d_1);

				dst[0] = s_0;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				d_0 = (scale << 31) * s_0
						/ (((mlib_s64)s_1 << 16)
						+ 1) >> 15;

				SAT_U16(dst[0], d_0);

				dst[1] = s_1;

				dst += 2;
				src += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1;
	mlib_s64 d_0, d_1;
	mlib_s64 scale = 1u << 31;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				if (s_0 != 0) {
					d_1 = scale * s_1 / s_0;
				} else {
					d_1 = (1u << 31) * s_1;
				}

				SAT_S32(dst[1], d_1);

				dst[0] = s_0;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				if (s_0 != 0) {
					d_0 = scale * s_0 / s_1;
				} else {
					d_0 = (1u << 31) * s_0;
				}

				SAT_S32(dst[0], d_0);

				dst[1] = s_1;

				dst += 2;
				src += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1, s_2;
	mlib_s64 d_0, d_1, d_2;
	mlib_s64 v_0, v_1, v_2;
	mlib_s64 scale = CONST_2P(15);

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				v_1 = (scale << 32) * s_1
					/ (((mlib_s64)s_0 << 16) + 1);
				v_2 = (scale << 32) * s_2
					/ (((mlib_s64)s_0 << 16) + 1);

				SHIFT_S16(d_1, v_1);
				SHIFT_S16(d_2, v_2);

				SAT_S16(dst[1], d_1);
				SAT_S16(dst[2], d_2);

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;

				s_0 = src[3];

				src += 3;
				dst += 3;
			}

			v_1 = (scale << 32) * s_1
					/ (((mlib_s64)s_0 << 16)
					+ 1);
			v_2 = (scale << 32) * s_2
					/ (((mlib_s64)s_0 << 16)
					+ 1);

			SHIFT_S16(d_1, v_1);
			SHIFT_S16(d_2, v_2);

			SAT_S16(dst[1], d_1);
			SAT_S16(dst[2], d_2);

			dst[0] = s_0;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				v_0 = (scale << 32) * s_0
						/ (((mlib_s64)s_1 << 16)
						+ 1);
				v_2 = (scale << 32) * s_2
						/ (((mlib_s64)s_1 << 16)
						+ 1);

				SHIFT_S16(d_0, v_0);
				SHIFT_S16(d_2, v_2);

				SAT_S16(dst[0], d_0);
				SAT_S16(dst[2], d_2);

				s_0 = src[3];
				s_2 = src[5];

				dst[1] = s_1;

				s_1 = src[4];

				src += 3;
				dst += 3;
			}

			v_0 = (scale << 32) * s_0
					/ (((mlib_s64)s_1 << 16)
					+ 1);
			v_2 = (scale << 32) * s_2
					/ (((mlib_s64)s_1 << 16)
					+ 1);

			SHIFT_S16(d_0, v_0);
			SHIFT_S16(d_2, v_2);

			SAT_S16(dst[0], d_0);
			SAT_S16(dst[2], d_2);

			dst[1] = s_1;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				v_0 = (scale << 32) * s_0
						/ (((mlib_s64)s_2 << 16)
						+ 1);
				v_1 = (scale << 32) * s_1
						/ (((mlib_s64)s_2 << 16)
						+ 1);

				SHIFT_S16(d_0, v_0);
				SHIFT_S16(d_1, v_1);

				SAT_S16(dst[0], d_0);
				SAT_S16(dst[1], d_1);

				s_0 = src[3];
				s_1 = src[4];

				dst[2] = s_2;

				s_2 = src[5];

				src += 3;
				dst += 3;
			}

			v_0 = (scale << 32) * s_0
					/ (((mlib_s64)s_2 << 16)
					+ 1);
			v_1 = (scale << 32) * s_1
					/ (((mlib_s64)s_2 << 16)
					+ 1);

			SHIFT_S16(d_0, v_0);
			SHIFT_S16(d_1, v_1);

			SAT_S16(dst[0], d_0);
			SAT_S16(dst[1], d_1);

			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_u16 s_0, s_1, s_2;
	mlib_s64 d_0, d_1, d_2;
	mlib_s64 scale = CONST_2P(16);

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				d_1 = (scale << 31) * s_1
						/ (((mlib_s64)s_0 << 16)
						+ 1) >> 15;
				d_2 = (scale << 31) * s_2
						/ (((mlib_s64)s_0 << 16)
						+ 1) >> 15;

				SAT_U16(dst[1], d_1);
				SAT_U16(dst[2], d_2);

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;

				s_0 = src[3];

				src += 3;
				dst += 3;
			}

			d_1 = (scale << 31) * s_1
					/(((mlib_s64)s_0 << 16) + 1) >> 15;
			d_2 = (scale << 31) * s_2
					/(((mlib_s64)s_0 << 16) + 1) >> 15;

			SAT_U16(dst[1], d_1);
			SAT_U16(dst[2], d_2);

			dst[0] = s_0;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				d_0 = (scale << 31) * s_0
						/(((mlib_s64)s_1 << 16)
						+ 1) >> 15;
				d_2 = (scale << 31) * s_2
						/(((mlib_s64)s_1 << 16)
						+ 1) >> 15;

				SAT_U16(dst[0], d_0);
				SAT_U16(dst[2], d_2);

				s_0 = src[3];
				s_2 = src[5];

				dst[1] = s_1;

				s_1 = src[4];

				src += 3;
				dst += 3;
			}

			d_0 = (scale << 31) * s_0
					/(((mlib_s64)s_1 << 16) + 1) >> 15;
			d_2 = (scale << 31) * s_2
					/(((mlib_s64)s_1 << 16) + 1) >> 15;

			SAT_U16(dst[0], d_0);
			SAT_U16(dst[2], d_2);

			dst[1] = s_1;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				d_0 = (scale << 31) * s_0
					/(((mlib_s64)s_2 << 16) + 1) >> 15;
				d_1 = (scale << 31) * s_1
					/(((mlib_s64)s_2 << 16) + 1) >> 15;

				SAT_U16(dst[0], d_0);
				SAT_U16(dst[1], d_1);

				s_0 = src[3];
				s_1 = src[4];

				dst[2] = s_2;

				s_2 = src[5];

				src += 3;
				dst += 3;
			}

			d_0 = (scale << 31) * s_0
				/ (((mlib_s64)s_2 << 16) + 1) >> 15;
			d_1 = (scale << 31) * s_1
				/ (((mlib_s64)s_2 << 16) + 1) >> 15;

			SAT_U16(dst[0], d_0);
			SAT_U16(dst[1], d_1);

			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1, s_2;
	mlib_d64 d_0, d_1, d_2;
	mlib_s64 scale = 1u << 31;

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				if (s_0 != 0) {
					d_1 = s_1 * scale / s_0;
					d_2 = s_2 * scale / s_0;
				} else {
					d_1 = (1u << 31) * s_1;
					d_2 = (1u << 31) * s_2;
				}

				SAT_S32(dst[1], d_1);
				SAT_S32(dst[2], d_2);

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;

				s_0 = src[3];

				src += 3;
				dst += 3;
			}


			if (s_0 != 0) {
				d_1 = s_1 * scale / s_0;
				d_2 = s_2 * scale / s_0;
			} else {
				d_1 = (1u << 31) * s_1;
				d_2 = (1u << 31) * s_1;
			}

			SAT_S32(dst[1], d_1);
			SAT_S32(dst[2], d_2);

			dst[0] = s_0;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				if (s_1 != 0) {
					d_0 = scale * s_0 / s_1;
					d_2 = scale * s_2 / s_1;
				} else {
					d_0 = (1u << 31) * s_0;
					d_2 = (1u << 31) * s_2;
				}

				SAT_S32(dst[0], d_0);
				SAT_S32(dst[2], d_2);

				s_0 = src[3];
				s_2 = src[5];

				dst[1] = s_1;

				s_1 = src[4];

				src += 3;
				dst += 3;
			}

			if (s_1 != 0) {
				d_0 = scale * s_0 / s_1;
				d_2 = scale * s_2 / s_1;
			} else {
				d_0 = (1u << 31) * s_0;
				d_2 = (1u << 31) * s_2;
			}

			SAT_S32(dst[0], d_0);
			SAT_S32(dst[2], d_2);

			dst[1] = s_1;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				if (s_2 != 0) {
					d_0 = scale * s_0 / s_2;
					d_1 = scale * s_1 / s_2;
				} else {
					d_0 = (1u << 31) * s_0;
					d_1 = (1u << 31) * s_1;
				}

				SAT_S32(dst[0], d_0);
				SAT_S32(dst[1], d_1);

				s_0 = src[3];
				s_1 = src[4];

				dst[2] = s_2;

				s_2 = src[5];

				src += 3;
				dst += 3;
			}


			if (s_2 != 0) {
				d_0 = scale * s_0 / s_2;
				d_1 = scale * s_1 / s_2;
			} else {
				d_0 = (1u << 31) * s_0;
				d_1 = (1u << 31) * s_1;
			}

			SAT_S32(dst[0], d_0);
			SAT_S32(dst[1], d_1);

			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S16_4(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s64 scale = CONST_2P(15);
	mlib_s64 d_0, d_1, d_2, d_3;
	mlib_s64 v_0, v_1, v_2, v_3;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				v_1 = (scale << 32) * src[1]
						/ (((mlib_s64)src[0] << 16)
						+ 1);
				v_2 = (scale << 32) * src[2]
						/ (((mlib_s64)src[0] << 16)
						+ 1);
				v_3 = (scale << 32) * src[3]
						/ (((mlib_s64)src[0] << 16)
						+ 1);

				SHIFT_S16(d_1, v_1);
				SHIFT_S16(d_2, v_2);
				SHIFT_S16(d_3, v_3);

				SAT_S16(dst[1], d_1);
				SAT_S16(dst[2], d_2);
				SAT_S16(dst[3], d_3);

				dst[0] = src[0];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				v_0 = (scale << 32) * src[0]
						/ (((mlib_s64)src[1] << 16)
						+ 1);
				v_2 = (scale << 32) * src[2]
						/ (((mlib_s64)src[1] << 16)
						+ 1);
				v_3 = (scale << 32) * src[3]
						/ (((mlib_s64)src[1] << 16)
						+ 1);

				SHIFT_S16(d_0, v_0);
				SHIFT_S16(d_2, v_2);
				SHIFT_S16(d_3, v_3);

				SAT_S16(dst[0], d_0);
				SAT_S16(dst[2], d_2);
				SAT_S16(dst[3], d_3);

				dst[1] = src[1];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				v_0 = (scale << 32) * src[0]
						/ (((mlib_s64)src[2] << 16)
						+ 1);
				v_1 = (scale << 32) * src[1]
						/ (((mlib_s64)src[2] << 16)
						+ 1);
				v_3 = (scale << 32) * src[3]
						/ (((mlib_s64)src[2] << 16)
						+ 1);

				SHIFT_S16(d_0, v_0);
				SHIFT_S16(d_1, v_1);
				SHIFT_S16(d_3, v_3);

				SAT_S16(dst[0], d_0);
				SAT_S16(dst[1], d_1);
				SAT_S16(dst[3], d_3);

				dst[2] = src[2];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				v_0 = (scale << 32) * src[0]
						/ (((mlib_s64)src[3] << 16)
						+ 1);
				v_1 = (scale << 32) * src[1]
						/ (((mlib_s64)src[3] << 16)
						+ 1);
				v_2 = (scale << 32) * src[2]
						/ (((mlib_s64)src[3] << 16)
						+ 1);

				SHIFT_S16(d_0, v_0);
				SHIFT_S16(d_1, v_1);
				SHIFT_S16(d_2, v_2);

				SAT_S16(dst[0], d_0);
				SAT_S16(dst[1], d_1);
				SAT_S16(dst[2], d_2);

				dst[3] = src[3];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U16_4(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s64 scale = CONST_2P(16);
	mlib_s64 d_0, d_1, d_2, d_3;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_1 = (scale << 31) * src[1]
						/ (((mlib_s64)src[0] << 16)
						+ 1) >> 15;
				d_2 = (scale << 31) * src[2]
						/ (((mlib_s64)src[0] << 16)
						+ 1) >> 15;
				d_3 = (scale << 31) * src[3]
						/ (((mlib_s64)src[0] << 16)
						+ 1) >> 15;

				SAT_U16(dst[1], d_1);
				SAT_U16(dst[2], d_2);
				SAT_U16(dst[3], d_3);

				dst[0] = src[0];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_0 = (scale << 31) * src[0]
						/ (((mlib_s64)src[1] << 16)
						+ 1) >> 15;
				d_2 = (scale << 31) * src[2]
						/ (((mlib_s64)src[1] << 16)
						+ 1) >> 15;
				d_3 = (scale << 31) * src[3]
						/ (((mlib_s64)src[1] << 16)
						+ 1) >> 15;

				SAT_U16(dst[0], d_0);
				SAT_U16(dst[2], d_2);
				SAT_U16(dst[3], d_3);

				dst[1] = src[1];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_0 = (scale << 31) * src[0]
						/ (((mlib_s64)src[2] << 16)
						+ 1) >> 15;
				d_1 = (scale << 31) * src[1]
						/ (((mlib_s64)src[2] << 16)
						+ 1) >> 15;
				d_3 = (scale << 31) * src[3]
					/ (((mlib_s64)src[2] << 16)
						+ 1) >> 15;

				SAT_U16(dst[0], d_0);
				SAT_U16(dst[1], d_1);
				SAT_U16(dst[3], d_3);

				dst[2] = src[2];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_0 = (scale << 31) * src[0]
						/ (((mlib_s64)src[3] << 16)
						+ 1) >> 15;
				d_1 = (scale << 31) * src[1]
						/ (((mlib_s64)src[3] << 16)
						+ 1) >> 15;
				d_2 = (scale << 31) * src[2]
						/ (((mlib_s64)src[3] << 16)
						+ 1) >> 15;

				SAT_U16(dst[0], d_0);
				SAT_U16(dst[1], d_1);
				SAT_U16(dst[2], d_2);

				dst[3] = src[3];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s64 scale = 1u << 31;
	mlib_s64 d_0, d_1, d_2, d_3;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (src[0] != 0) {
					d_1 = scale * src[1] / src[0];
					d_2 = scale * src[2] / src[0];
					d_3 = scale * src[3] / src[0];
				} else {
					d_1 = (1u << 31) * src[1];
					d_2 = (1u << 31) * src[2];
					d_3 = (1u << 31) * src[3];
				}

				SAT_S32(dst[1], d_1);
				SAT_S32(dst[2], d_2);
				SAT_S32(dst[3], d_3);

				dst[0] = src[0];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (src[1] != 0) {
					d_0 = scale * src[0] / src[1];
					d_2 = scale * src[2] / src[1];
					d_3 = scale * src[3] / src[1];
				} else {
					d_0 = (1u << 31) * src[0];
					d_2 = (1u << 31) * src[2];
					d_3 = (1u << 31) * src[3];
				}

				SAT_S32(dst[0], d_0);
				SAT_S32(dst[2], d_2);
				SAT_S32(dst[3], d_3);

				dst[1] = src[1];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (src[2] != 0) {
					d_0 = scale * src[0] / src[2];
					d_1 = scale * src[1] / src[2];
					d_3 = scale * src[3] / src[2];
				} else {
					d_0 = (1u << 31) * src[0];
					d_1 = (1u << 31) * src[0];
					d_3 = (1u << 31) * src[0];
				}

				SAT_S32(dst[0], d_0);
				SAT_S32(dst[1], d_1);
				SAT_S32(dst[3], d_3);

				dst[2] = src[2];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (src[3] != 0) {
					d_0 = scale * src[0] / src[3];
					d_1 = scale * src[1] / src[3];
					d_2 = scale * src[2] / src[3];
				} else {
					d_0 = (1u << 31) * src[0];
					d_1 = (1u << 31) * src[1];
					d_2 = (1u << 31) * src[2];
				}

				SAT_S32(dst[0], d_0);
				SAT_S32(dst[1], d_1);
				SAT_S32(dst[2], d_2);

				dst[3] = src[3];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageVisVersion.h>
#include <mlib_ImageDivAlpha_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivAlpha = __mlib_ImageDivAlpha

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivAlpha) mlib_ImageDivAlpha
    __attribute__((weak, alias("__mlib_ImageDivAlpha")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	CONST_2P(sh)	(1 << (sh))*65536.f

/* *********************************************************** */

#define	SAT16(DST, SRC)	DST = (mlib_s32) (SRC) >> 16

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)(SRC - sat)) >> 16) ^ 0x8000

/* *********************************************************** */

#define	SAT32(DST, SRC)	DST = (mlib_s32) (SRC)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CONST_2P(sh)	(1 << (sh))

/* *********************************************************** */

#define	SAT16(DST, SRC)                                         \
	{                                                       \
	    mlib_d64 dd = SRC;                                  \
	                                                        \
	    if (dd >= MLIB_S16_MAX)                             \
		dd = MLIB_S16_MAX;                              \
	    if (dd <= MLIB_S16_MIN)                             \
		dd = MLIB_S16_MIN;                              \
	    DST = (mlib_s32)dd;                                 \
	}

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	{                                                       \
	    mlib_d64 dd = SRC;                                  \
	                                                        \
	    if (dd >= MLIB_U16_MAX)                             \
		dd = MLIB_U16_MAX;                              \
	    if (dd <= MLIB_U16_MIN)                             \
		dd = MLIB_U16_MIN;                              \
	    DST = (mlib_s32)dd;                                 \
	}

/* *********************************************************** */

#define	SAT32(DST, SRC)                                         \
	{                                                       \
	    mlib_d64 dd = SRC;                                  \
	                                                        \
	    if (dd >= MLIB_S32_MAX)                             \
		dd = MLIB_S32_MAX;                              \
	    if (dd <= MLIB_S32_MIN)                             \
		dd = MLIB_S32_MIN;                              \
	    DST = (mlib_s32)dd;                                 \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FL_MIN	1e-20F
#define	DB_MIN	1e-100

/* *********************************************************** */

static void mlib_ImageDivAlpha_S16_2(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S16_4(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U16_4(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

/* *********************************************************** */

mlib_status
__mlib_ImageDivAlpha(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 alpha, t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, channel, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if ((channel < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; !(cmask & 0x1); alpha++)
		cmask >>= 1;

	if (cmask >> 1)
		return (MLIB_OUTOFRANGE);

	alpha = channel - alpha - 1;

	if (alpha < 0 || alpha >= channel)
		return (MLIB_OUTOFRANGE);

	if (type == MLIB_BYTE) {
		t_sh = 0;
	} else if (type == MLIB_INT) {
		t_sh = 2;
	} else {
		t_sh = 1;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == channel * width) {
		mlib_s32 algn = 0;

		if (mlib_ImageVisVersion() && type == MLIB_BYTE)
			algn = 7;

/* for VIS 1-D vector only if aligned */
		if (!(((int)sdata | (int)ddata) & algn)) {
			width *= height;
			height = 1;
		}
	}

	switch (type) {
	case MLIB_BYTE:
		return mlib_ImageDivAlpha_U8(sdata, sstride, ddata, dstride,
		    width, height, channel, alpha);

	case MLIB_SHORT:

		if (channel == 4) {
			mlib_ImageDivAlpha_S16_4(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else if (channel == 3) {
			mlib_ImageDivAlpha_S16_3(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else {
/* if (channel == 2) */

			mlib_ImageDivAlpha_S16_2(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);

	case MLIB_USHORT:

		if (channel == 4) {
			mlib_ImageDivAlpha_U16_4(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else if (channel == 3) {
			mlib_ImageDivAlpha_U16_3(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else {
/* if (channel == 2) */

			mlib_ImageDivAlpha_U16_2(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);

	case MLIB_INT:

		if (channel == 4) {
			mlib_ImageDivAlpha_S32_4(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else if (channel == 3) {
			mlib_ImageDivAlpha_S32_3(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		} else {
/* if (channel == 2) */

			mlib_ImageDivAlpha_S32_2(sdata, sstride, ddata, dstride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S16_2(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1, d_1;
	mlib_f32 f_alpha, scale = CONST_2P(15);

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				f_alpha =
				    scale / ((mlib_f32)s_0 + FL_MIN) * s_1;
				SAT16(d_1, f_alpha);

				dst[0] = s_0;
				dst[1] = d_1;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_1 = src[0];
				s_0 = src[1];

				f_alpha =
				    scale / ((mlib_f32)s_0 + FL_MIN) * s_1;
				SAT16(d_1, f_alpha);

				dst[0] = d_1;
				dst[1] = s_0;

				dst += 2;
				src += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1, d_1;
	mlib_f32 f_alpha, scale = CONST_2P(16);
	mlib_f32 sat = CONST_2P(15);

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				f_alpha =
				    scale / ((mlib_f32)s_0 + FL_MIN) * s_1;
				SAT_U16(d_1, f_alpha);

				dst[0] = s_0;
				dst[1] = d_1;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_1 = src[0];
				s_0 = src[1];

				f_alpha =
				    scale / ((mlib_f32)s_0 + FL_MIN) * s_1;
				SAT_U16(d_1, f_alpha);

				dst[0] = d_1;
				dst[1] = s_0;

				dst += 2;
				src += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, s_1, d_1;
	mlib_d64 f_alpha, scale = 2147483648.;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_0 = src[0];
				s_1 = src[1];

				f_alpha =
				    scale / ((mlib_d64)s_0 + DB_MIN) * s_1;
				SAT32(d_1, f_alpha);

				dst[0] = s_0;
				dst[1] = d_1;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s_1 = src[0];
				s_0 = src[1];

				f_alpha =
				    scale / ((mlib_d64)s_0 + DB_MIN) * s_1;
				SAT32(d_1, f_alpha);

				dst[0] = d_1;
				dst[1] = s_0;

				dst += 2;
				src += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_f32 f_alpha, scale = CONST_2P(15);

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		mlib_s32 s_0, d_1, d_2;
		mlib_f32 s_1, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_alpha = scale / ((mlib_f32)s_0 + FL_MIN);
				s_1 *= f_alpha;
				SAT16(d_1, s_1);
				s_2 *= f_alpha;
				SAT16(d_2, s_2);

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;
				dst[1] = d_1;
				dst[2] = d_2;

				s_0 = src[3];
				src += 3;
				dst += 3;
			}

			f_alpha = scale / ((mlib_f32)s_0 + FL_MIN);
			s_1 *= f_alpha;
			SAT16(d_1, s_1);
			s_2 *= f_alpha;
			SAT16(d_2, s_2);

			dst[0] = s_0;
			dst[1] = d_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else if (alpha == 1) {
		mlib_s32 s_1, d_0, d_2;
		mlib_f32 s_0, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_alpha = scale / ((mlib_f32)s_1 + FL_MIN);
				s_0 *= f_alpha;
				SAT16(d_0, s_0);
				s_2 *= f_alpha;
				SAT16(d_2, s_2);

				s_0 = src[3];
				s_2 = src[5];

				dst[0] = d_0;
				dst[1] = s_1;
				dst[2] = d_2;

				s_1 = src[4];
				src += 3;
				dst += 3;
			}

			f_alpha = scale / ((mlib_f32)s_1 + FL_MIN);
			s_0 *= f_alpha;
			SAT16(d_0, s_0);
			s_2 *= f_alpha;
			SAT16(d_2, s_2);

			dst[0] = d_0;
			dst[1] = s_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
		mlib_s32 s_2, d_0, d_1;
		mlib_f32 s_0, s_1;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_alpha = scale / ((mlib_f32)s_2 + FL_MIN);
				s_0 *= f_alpha;
				SAT16(d_0, s_0);
				s_1 *= f_alpha;
				SAT16(d_1, s_1);

				s_0 = src[3];
				s_1 = src[4];

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = s_2;

				s_2 = src[5];
				src += 3;
				dst += 3;
			}

			f_alpha = scale / ((mlib_f32)s_2 + FL_MIN);
			s_0 *= f_alpha;
			SAT16(d_0, s_0);
			s_1 *= f_alpha;
			SAT16(d_1, s_1);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_f32 f_alpha, scale = CONST_2P(16);
	mlib_f32 sat = CONST_2P(15);

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
/*
 *    mlib_s32 s_0, d_1, d_2;
 */
		mlib_u16 s_0, d_1, d_2;
		mlib_f32 s_1, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_alpha = scale / ((mlib_f32)s_0 + FL_MIN);
				s_1 *= f_alpha;
				SAT_U16(d_1, s_1);
				s_2 *= f_alpha;
				SAT_U16(d_2, s_2);

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;
				dst[1] = d_1;
				dst[2] = d_2;

				s_0 = src[3];
				src += 3;
				dst += 3;
			}

			f_alpha = scale / ((mlib_f32)s_0 + FL_MIN);
			s_1 *= f_alpha;
			SAT_U16(d_1, s_1);
			s_2 *= f_alpha;
			SAT_U16(d_2, s_2);

			dst[0] = s_0;
			dst[1] = d_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else if (alpha == 1) {
/*
 *    mlib_s32 s_1, d_0, d_2;
 */
		mlib_u16 s_1, d_0, d_2;
		mlib_f32 s_0, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_alpha = scale / ((mlib_f32)s_1 + FL_MIN);
				s_0 *= f_alpha;
				SAT_U16(d_0, s_0);
				s_2 *= f_alpha;
				SAT_U16(d_2, s_2);

				s_0 = src[3];
				s_2 = src[5];

				dst[0] = d_0;
				dst[1] = s_1;
				dst[2] = d_2;

				s_1 = src[4];
				src += 3;
				dst += 3;
			}

			f_alpha = scale / ((mlib_f32)s_1 + FL_MIN);
			s_0 *= f_alpha;
			SAT_U16(d_0, s_0);
			s_2 *= f_alpha;
			SAT_U16(d_2, s_2);

			dst[0] = d_0;
			dst[1] = s_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
/*
 *    mlib_s32 s_2, d_0, d_1;
 */
		mlib_u16 s_2, d_0, d_1;
		mlib_f32 s_0, s_1;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_alpha = scale / ((mlib_f32)s_2 + FL_MIN);
				s_0 *= f_alpha;
				SAT_U16(d_0, s_0);
				s_1 *= f_alpha;
				SAT_U16(d_1, s_1);

				s_0 = src[3];
				s_1 = src[4];

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = s_2;

				s_2 = src[5];
				src += 3;
				dst += 3;
			}

			f_alpha = scale / ((mlib_f32)s_2 + FL_MIN);
			s_0 *= f_alpha;
			SAT_U16(d_0, s_0);
			s_1 *= f_alpha;
			SAT_U16(d_1, s_1);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_d64 shift_const = 2147483648.;

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		mlib_s32 s_0, d_1, d_2;
		mlib_d64 s_1, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				SAT32(d_1,
				    s_1 * shift_const / ((mlib_d64)s_0 +
				    DB_MIN));
				SAT32(d_2,
				    s_2 * shift_const / ((mlib_d64)s_0 +
				    DB_MIN));

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;
				dst[1] = d_1;
				dst[2] = d_2;

				s_0 = src[3];
				src += 3;
				dst += 3;
			}

			SAT32(d_1,
			    s_1 * shift_const / ((mlib_d64)s_0 +
			    DB_MIN));
			SAT32(d_2,
			    s_2 * shift_const / ((mlib_d64)s_0 +
			    DB_MIN));

			dst[0] = s_0;
			dst[1] = d_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else if (alpha == 1) {
		mlib_s32 s_1, d_0, d_2;
		mlib_d64 s_0, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				SAT32(d_0,
				    s_0 * shift_const / ((mlib_d64)s_1 +
				    DB_MIN));
				SAT32(d_2,
				    s_2 * shift_const / ((mlib_d64)s_1 +
				    DB_MIN));

				s_0 = src[3];
				s_2 = src[5];

				dst[0] = d_0;
				dst[1] = s_1;
				dst[2] = d_2;

				s_1 = src[4];
				src += 3;
				dst += 3;
			}

			SAT32(d_0,
			    s_0 * shift_const / ((mlib_d64)s_1 +
			    DB_MIN));
			SAT32(d_2,
			    s_2 * shift_const / ((mlib_d64)s_1 +
			    DB_MIN));

			dst[0] = d_0;
			dst[1] = s_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
		mlib_s32 s_2, d_0, d_1;
		mlib_d64 s_0, s_1;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				SAT32(d_0,
				    s_0 * shift_const / ((mlib_d64)s_2 +
				    DB_MIN));
				SAT32(d_1,
				    s_1 * shift_const / ((mlib_d64)s_2 +
				    DB_MIN));

				s_0 = src[3];
				s_1 = src[4];

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = s_2;

				s_2 = src[5];
				src += 3;
				dst += 3;
			}

			SAT32(d_0,
			    s_0 * shift_const / ((mlib_d64)s_2 +
			    DB_MIN));
			SAT32(d_1,
			    s_1 * shift_const / ((mlib_d64)s_2 +
			    DB_MIN));

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S16_4(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a;
	mlib_f32 f_alpha, scale = CONST_2P(15), tmp;
	mlib_s32 d_0, d_1, d_2;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[0];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[1] * f_alpha;
				SAT16(d_0, tmp);
				tmp = (mlib_f32)src[2] * f_alpha;
				SAT16(d_1, tmp);
				tmp = (mlib_f32)src[3] * f_alpha;
				SAT16(d_2, tmp);
				dst[0] = a;
				dst[1] = d_0;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[1];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[0] * f_alpha;
				SAT16(d_0, tmp);
				tmp = (mlib_f32)src[2] * f_alpha;
				SAT16(d_1, tmp);
				tmp = (mlib_f32)src[3] * f_alpha;
				SAT16(d_2, tmp);
				dst[0] = d_0;
				dst[1] = a;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[2];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[0] * f_alpha;
				SAT16(d_0, tmp);
				tmp = (mlib_f32)src[1] * f_alpha;
				SAT16(d_1, tmp);
				tmp = (mlib_f32)src[3] * f_alpha;
				SAT16(d_2, tmp);
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = a;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[3];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[0] * f_alpha;
				SAT16(d_0, tmp);
				tmp = (mlib_f32)src[1] * f_alpha;
				SAT16(d_1, tmp);
				tmp = (mlib_f32)src[2] * f_alpha;
				SAT16(d_2, tmp);
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = a;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U16_4(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a;
	mlib_f32 f_alpha, scale = CONST_2P(16), tmp;
	mlib_f32 sat = CONST_2P(15);
	mlib_u16 d_0, d_1, d_2;

/*
 *  mlib_s32 d_0, d_1, d_2;
 */

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[0];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[1] * f_alpha;
				SAT_U16(d_0, tmp);
				tmp = (mlib_f32)src[2] * f_alpha;
				SAT_U16(d_1, tmp);
				tmp = (mlib_f32)src[3] * f_alpha;
				SAT_U16(d_2, tmp);
				dst[0] = a;
				dst[1] = d_0;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[1];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[0] * f_alpha;
				SAT_U16(d_0, tmp);
				tmp = (mlib_f32)src[2] * f_alpha;
				SAT_U16(d_1, tmp);
				tmp = (mlib_f32)src[3] * f_alpha;
				SAT_U16(d_2, tmp);
				dst[0] = d_0;
				dst[1] = a;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[2];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[0] * f_alpha;
				SAT_U16(d_0, tmp);
				tmp = (mlib_f32)src[1] * f_alpha;
				SAT_U16(d_1, tmp);
				tmp = (mlib_f32)src[3] * f_alpha;
				SAT_U16(d_2, tmp);
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = a;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[3];
				f_alpha = scale / ((mlib_f32)a + FL_MIN);
				tmp = (mlib_f32)src[0] * f_alpha;
				SAT_U16(d_0, tmp);
				tmp = (mlib_f32)src[1] * f_alpha;
				SAT_U16(d_1, tmp);
				tmp = (mlib_f32)src[2] * f_alpha;
				SAT_U16(d_2, tmp);
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = a;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a;
	mlib_d64 shift_const = 2147483648.;
	mlib_s32 d_0, d_1, d_2;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[0];
				SAT32(d_0,
				    src[1] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_1,
				    src[2] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_2,
				    src[3] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				dst[0] = a;
				dst[1] = d_0;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[1];
				SAT32(d_0,
				    src[0] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_1,
				    src[2] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_2,
				    src[3] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				dst[0] = d_0;
				dst[1] = a;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[2];
				SAT32(d_0,
				    src[0] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_1,
				    src[1] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_2,
				    src[3] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = a;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[3];
				SAT32(d_0,
				    src[0] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_1,
				    src[1] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				SAT32(d_2,
				    src[2] * shift_const / ((mlib_d64)a +
				    DB_MIN));
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = a;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}

#endif
/* *********************************************************** */
