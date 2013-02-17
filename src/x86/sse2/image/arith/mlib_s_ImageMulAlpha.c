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

#pragma ident   "@(#)mlib_s_ImageMulAlpha.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMulAlpha - multiply color channels by the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMulAlpha(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         cmask)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *      cmask   Channel mask to indicate the alpha channel.
 *              Each bit of which represents a channel in the image. The
 *              channel corresponded to the 1 bit is the alpha channel.
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Multiply color channels by the alpha channel in the way of
 *      pixel by pixel.
 *
 *  The image must have at least two channels.
 *
 *  For an MLIB_BYTE image
 *    dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -8)
 *  For an MLIB_SHORT image
 *    dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -15)
 *  For an MLIB_USHORT image
 *    dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -16)
 *  For an MLIB_INT image
 *    dst->data[i][j] = src->data[i][j] * src->data[i][a] * pow(2, -31)
 *  where, j != a
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>
#include <mlib_c_ImageBlendTable.h>

#include <mlib_s_ImageMulAlpha.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMulAlpha = __mlib_ImageMulAlpha

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMulAlpha) mlib_ImageMulAlpha
    __attribute__((weak, alias("__mlib_ImageMulAlpha")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 - shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 - shift) << 20
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_ImageMulAlpha(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	mlib_s32 channel, width, height, stride, d_stride;
	void *s_data, *d_data;
	mlib_type d_type;
	mlib_s32 alpha;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, d_type, channel, width, height, d_stride,
	    d_data);
	stride = mlib_ImageGetStride(src);
	s_data = mlib_ImageGetData(src);

	if ((channel < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; (cmask & 0x1) == 0; alpha++)
		cmask >>= 1;

	if ((cmask >> 1) != 0)
		return (MLIB_OUTOFRANGE);

/*  0 <= alpha < channel */
	alpha = channel - alpha - 1;

	if (0 > alpha && alpha >= channel)
		return (MLIB_OUTOFRANGE);

	if (d_type == MLIB_BYTE) {
		if (channel == 4) {
			mlib_s_ImageMulAlpha_U8_4(s_data, stride, d_data,
			    d_stride, width, height, alpha);
		} else if (channel == 3) {
			mlib_s_ImageMulAlpha_U8_3(s_data, stride, d_data,
			    d_stride, width, height, alpha);
		} else if (channel == 2) {
			mlib_s_ImageMulAlpha_U8_2(s_data, stride, d_data,
			    d_stride, width, height, alpha);
		}

		return (MLIB_SUCCESS);
	} else if (d_type == MLIB_SHORT) {

		stride >>= 1;
		d_stride >>= 1;

		if (channel == 4) {
			mlib_s_ImageMulAlpha_S16_4((mlib_s16 *)s_data, stride,
			    (mlib_s16 *)d_data, d_stride, width, height, alpha);
		} else if (channel == 3) {
			mlib_s_ImageMulAlpha_S16_3((mlib_s16 *)s_data, stride,
			    (mlib_s16 *)d_data, d_stride, width, height, alpha);
		} else if (channel == 2) {
			mlib_s_ImageMulAlpha_S16_2((mlib_s16 *)s_data, stride,
			    (mlib_s16 *)d_data, d_stride, width, height, alpha);
		}

		return (MLIB_SUCCESS);

	} else if (d_type == MLIB_USHORT) {
		stride >>= 1;
		d_stride >>= 1;

		if (channel == 4) {
			mlib_s_ImageMulAlpha_U16_4((mlib_u16 *)s_data, stride,
			    (mlib_u16 *)d_data, d_stride, width, height, alpha);
		} else if (channel == 3) {
			mlib_s_ImageMulAlpha_U16_3((mlib_u16 *)s_data, stride,
			    (mlib_u16 *)d_data, d_stride, width, height, alpha);
		} else if (channel == 2) {
			mlib_s_ImageMulAlpha_U16_2((mlib_u16 *)s_data, stride,
			    (mlib_u16 *)d_data, d_stride, width, height, alpha);
		}

		return (MLIB_SUCCESS);
	} else if (d_type == MLIB_INT) {
		stride >>= 2;
		d_stride >>= 2;

		if (channel == 4) {
			mlib_s_ImageMulAlpha_S32_4((mlib_s32 *)s_data, stride,
			    (mlib_s32 *)d_data, d_stride, width, height, alpha);
		} else if (channel == 3) {
			mlib_s_ImageMulAlpha_S32_3((mlib_s32 *)s_data, stride,
			    (mlib_s32 *)d_data, d_stride, width, height, alpha);
		} else if (channel == 2) {
			mlib_s_ImageMulAlpha_S32_2((mlib_s32 *)s_data, stride,
			    (mlib_s32 *)d_data, d_stride, width, height, alpha);
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_s_ImageMulAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1;
	mlib_d64 shift_const = 1. / 2147483648.;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				d_0 = src[0];
				d_1 =
				    (mlib_s32)((mlib_d64)src[1] *
				    (mlib_d64)d_0 * shift_const);
				dst[0] = d_0;
				dst[1] = d_1;
				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			d_0 = src[1];
			d_1 =
			    (mlib_s32)((mlib_d64)src[0] * (mlib_d64)d_0 *
			    shift_const);
			dst[0] = d_1;
			dst[1] = d_0;
			src += 2;
			dst += 2;
		}

		src += s_stride;
		dst += d_stride;
	}
}


/* *********************************************************** */

void
mlib_s_ImageMulAlpha_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_f32 f_0, f_1, f_2;
	mlib_s32 i, j;
	mlib_s32 s_0, s_1, s_2;

	i = width + (width << 1);
	s_stride -= i;
	d_stride -= i;
	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				dst[0] = s_0;
				f_0 = mlib_c_blend_Q8[s_0];
				f_1 = mlib_U82F32[s_1];
				f_2 = mlib_U82F32[s_2];
				s_0 = src[3];
				s_1 = src[4];
				s_2 = src[5];
				f_1 *= f_0;
				f_2 *= f_0;
				dst[1] = (mlib_u8)f_1;
				dst[2] = (mlib_u8)f_2;
				dst += 3;
				src += 3;
			}
			dst[0] = s_0;
			f_0 = mlib_c_blend_Q8[s_0];
			f_1 = mlib_U82F32[s_1];
			f_2 = mlib_U82F32[s_2];
			f_1 *= f_0;
			f_2 *= f_0;
			dst[1] = (mlib_u8)f_1;
			dst[2] = (mlib_u8)f_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				f_0 = mlib_U82F32[s_0];
				f_1 = mlib_c_blend_Q8[s_1];
				f_2 = mlib_U82F32[s_2];
				s_0 = src[3];
				s_2 = src[5];
				f_0 *= f_1;
				f_2 *= f_1;
				dst[0] = (mlib_u8)f_0;
				dst[1] = s_1;
				dst[2] = (mlib_u8)f_2;
				s_1 = src[4];
				src += 3;
				dst += 3;
			}

			f_0 = mlib_U82F32[s_0];
			f_1 = mlib_c_blend_Q8[s_1];
			f_2 = mlib_U82F32[s_2];
			f_0 *= f_1;
			f_2 *= f_1;
			dst[0] = (mlib_u8)f_0;
			dst[1] = s_1;
			dst[2] = (mlib_u8)f_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}

		return;
	}

	for (i = 0; i < height; i++) {

		s_2 = src[2];
		s_0 = src[0];
		s_1 = src[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

		for (j = 0; j < width - 1; j++) {
			f_2 = mlib_c_blend_Q8[s_2];
			f_0 = mlib_U82F32[s_0];
			f_1 = mlib_U82F32[s_1];

			s_0 = src[3];
			s_1 = src[4];

			f_0 *= f_2;
			f_1 *= f_2;

			dst[0] = (mlib_u8)f_0;
			dst[1] = (mlib_u8)f_1;
			dst[2] = s_2;
			s_2 = src[5];
			src += 3;
			dst += 3;
		}

		f_2 = mlib_c_blend_Q8[s_2];
		f_0 = mlib_U82F32[s_0];
		f_1 = mlib_U82F32[s_1];

		f_0 *= f_2;
		f_1 *= f_2;

		dst[0] = (mlib_u8)f_0;
		dst[1] = (mlib_u8)f_1;
		dst[2] = s_2;

		src += s_stride + 3;
		dst += d_stride + 3;
	}
}

/* *********************************************************** */

void
mlib_s_ImageMulAlpha_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1, d_2;
	mlib_f32 a;

	s_stride -= width + (width << 1);
	d_stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_2 = src[0];
				a = (mlib_f32)d_2;
				d_0 = (mlib_s32)((mlib_f32)src[1] * a) >> 15;
				d_1 = (mlib_s32)((mlib_f32)src[2] * a) >> 15;
				dst[0] = d_2;
				dst[1] = d_0;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_2 = src[1];
				a = (mlib_f32)d_2;
				d_0 = (mlib_s32)((mlib_f32)src[0] * a) >> 15;
				d_1 = (mlib_s32)((mlib_f32)src[2] * a) >> 15;
				dst[0] = d_0;
				dst[1] = d_2;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			d_2 = src[2];
			a = (mlib_f32)d_2;
			d_0 = (mlib_s32)((mlib_f32)src[0] * a) >> 15;
			d_1 = (mlib_s32)((mlib_f32)src[1] * a) >> 15;
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			src += 3;
			dst += 3;
		}

		src += s_stride;
		dst += d_stride;
	}
}


/* *********************************************************** */



void
mlib_s_ImageMulAlpha_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1, d_2;
	mlib_f32 a;
	mlib_d64 dscale;
	mlib_f32 fscale;

	SCALBN(dscale, 16);
	fscale = (mlib_f32)dscale;

	s_stride -= width + (width << 1);
	d_stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_2 = src[0];
				a = (mlib_f32)d_2;
				d_0 = (mlib_s32)((mlib_f32)src[1] * a * fscale);
				d_1 = (mlib_s32)((mlib_f32)src[2] * a * fscale);
				dst[0] = d_2;
				dst[1] = d_0;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_2 = src[1];
				a = (mlib_f32)d_2;
				d_0 = (mlib_s32)((mlib_f32)src[0] * a * fscale);
				d_1 = (mlib_s32)((mlib_f32)src[2] * a * fscale);
				dst[0] = d_0;
				dst[1] = d_2;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			d_2 = src[2];
			a = (mlib_f32)d_2;
			d_0 = (mlib_s32)((mlib_f32)src[0] * a * fscale);
			d_1 = (mlib_s32)((mlib_f32)src[1] * a * fscale);
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			src += 3;
			dst += 3;
		}

		src += s_stride;
		dst += d_stride;
	}
}



/* *********************************************************** */

void
mlib_s_ImageMulAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1, d_2;
	mlib_d64 a;
	mlib_d64 shift_const = 1. / 2147483648.;

	s_stride -= width + (width << 1);
	d_stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				d_2 = src[0];
				a = (mlib_d64)d_2;
				d_0 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				dst[0] = d_2;
				dst[1] = d_0;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				d_2 = src[1];
				a = (mlib_d64)d_2;
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				dst[0] = d_0;
				dst[1] = d_2;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			d_2 = src[2];
			a = (mlib_d64)d_2;
			d_0 = (mlib_s32)((mlib_d64)src[0] * a * shift_const);
			d_1 = (mlib_s32)((mlib_d64)src[1] * a * shift_const);
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			src += 3;
			dst += 3;
		}

		src += s_stride;
		dst += d_stride;
	}
}


/* *********************************************************** */

void
mlib_s_ImageMulAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 d_0, d_1, d_2, d_3;
	mlib_d64 a;
	mlib_d64 shift_const = 1. / 2147483648.;

	s_stride -= width << 2;
	d_stride -= width << 2;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				d_3 = src[0];
				a = (mlib_d64)d_3;
				d_0 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				d_2 =
				    (mlib_s32)((mlib_d64)src[3] * a *
				    shift_const);
				dst[0] = d_3;
				dst[1] = d_0;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				d_3 = src[1];
				a = (mlib_d64)d_3;
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				d_2 =
				    (mlib_s32)((mlib_d64)src[3] * a *
				    shift_const);
				dst[0] = d_0;
				dst[1] = d_3;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 2) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				d_3 = src[2];
				a = (mlib_d64)d_3;
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_2 =
				    (mlib_s32)((mlib_d64)src[3] * a *
				    shift_const);
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_3;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			d_3 = src[3];
			a = (mlib_d64)d_3;
			d_0 = (mlib_s32)((mlib_d64)src[0] * a * shift_const);
			d_1 = (mlib_s32)((mlib_d64)src[1] * a * shift_const);
			d_2 = (mlib_s32)((mlib_d64)src[2] * a * shift_const);
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			dst[3] = d_3;
			src += 4;
			dst += 4;
		}

		src += s_stride;
		dst += d_stride;
	}
}

/* *********************************************************** */
