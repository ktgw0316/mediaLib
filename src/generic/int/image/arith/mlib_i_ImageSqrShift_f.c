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

#pragma ident   "@(#)mlib_i_ImageSqrShift_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_i_ImageSqrShift_U8  - MLIB_BYTE  type of image
 *      mlib_i_ImageSqrShift_S16 - MLIB_SHORT type of image
 *      mlib_i_ImageSqrShift_U16 - MLIB_USHORT type of image
 *      mlib_i_ImageSqrShift_S32 - MLIB_INT   type of image
 *      mlib_c_ImageSqrShift_S32 - MLIB_INT   type of image
 *
 * ARGUMENT
 *      src     pointer to input image
 *      dst     pointer to output image
 *      shift   right shifting factor
 *
 * DESCRIPTION
 *      dst = (src * src) >> shift
 *      -- C version low level functions.
 *      These functions are separated from mlib_i_ImageSqrShift.c
 *      for excluding of compiler inlining and structure clarity.
 */

#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_i_ImageSqrShift.h>

#include <stdio.h>
/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)(SRC - sat)) >> 16) ^ 0x8000

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	SRC -= sat;                                             \
	if (SRC >= MLIB_S32_MAX)                                \
	    SRC = MLIB_S32_MAX;                                 \
	DST = (((mlib_s32)SRC) >> 16) ^ 0x8000
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
mlib_i_ImageSqrShift_U8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 shift)
{
	mlib_s64 ilut[32];
	mlib_u8 *lut[4], *plut = (mlib_u8 *)ilut;
	mlib_s32 i, sqr, nchan = mlib_ImageGetChannels(src);

/* lut generation */
	plut[0] = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i < 256; i++) {
		sqr = (i * i) >> shift;
		plut[i] = sqr;

		if (sqr >= MLIB_U8_MAX)
			break;
	}

	for (; i < 256; i++)
		plut[i] = MLIB_U8_MAX;
	for (i = 0; i < nchan; i++)
		lut[i] = plut;

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

#define	SIZE	1024

/* *********************************************************** */

void
mlib_i_ImageSqrShift_S16(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_s32 num, col;
	mlib_s32 i, j;

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				mlib_s64 lval;
				lval = (mlib_s64)src[col + i]
					* src[col + i] >> shift;

				if (lval > MLIB_S16_MAX) {
					dst[col + i] = MLIB_S16_MAX;
				} else if (lval < MLIB_S16_MIN) {
					dst[col + i] = MLIB_S16_MIN;
				} else {
					dst[col + i] = lval;
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_i_ImageSqrShift_U16(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_s32 num, col;
	mlib_s32 i, j;

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				mlib_s64 lval;

				lval = (mlib_s64)src[col + i]
					* src[col + i] >> shift;

				if (lval > MLIB_U16_MAX) {
					dst[col + i] = MLIB_U16_MAX;
				} else if (lval < MLIB_U16_MIN) {
					dst[col + i] = MLIB_U16_MIN;
				} else {
					dst[col + i] = lval;
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_i_ImageSqrShift_S32(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_s32 i, j;

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			mlib_s64 lval;

			lval = (mlib_s64)src[i] * src[i] >> shift;

			if (lval > MLIB_S32_MAX) {
				dst[i] = MLIB_S32_MAX;
			} else if (lval < MLIB_S32_MIN) {
				dst[i] = MLIB_S32_MIN;
			} else {
				dst[i] = lval;
			}
		}

		src += slb;
		dst += dlb;
	}
}
/* *********************************************************** */
void
mlib_c_ImageSqrShift_S32(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_d64 dscale;
	mlib_s32 i, j;

/* dscale = 2 ** (-shift) */
	SCALBN(dscale, -shift);

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dst[i] =
			    FLOAT2INT_CLAMP(src[i] * (mlib_d64)src[i] * dscale);

		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */
#else

#include <mlib_image.h>
#include <mlib_c_ImageSqrShift.h>

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)(SRC - sat)) >> 16) ^ 0x8000

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	SRC -= sat;                                             \
	if (SRC >= MLIB_S32_MAX)                                \
	    SRC = MLIB_S32_MAX;                                 \
	DST = (((mlib_s32)SRC) >> 16) ^ 0x8000
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
mlib_c_ImageSqrShift_U8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 shift)
{
	mlib_d64 dlut[32];
	mlib_u8 *lut[4], *plut = (mlib_u8 *)dlut;
	mlib_s32 i, sqr, nchan = mlib_ImageGetChannels(src);

/* lut generation */
	plut[0] = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i < 256; i++) {
		sqr = (i * i) >> shift;
		plut[i] = sqr;

		if (sqr >= MLIB_U8_MAX)
			break;
	}

	for (; i < 256; i++)
		plut[i] = MLIB_U8_MAX;
	for (i = 0; i < nchan; i++)
		lut[i] = plut;

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

#define	SIZE	1024

/* *********************************************************** */

void
mlib_c_ImageSqrShift_S16(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_s32 buf[SIZE], num, col;
	mlib_d64 dscale;
	mlib_s32 i, j;

/* dscale = 2 ** (16 - shift) */
	SCALBN(dscale, 16 - shift);

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] = src[col + i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dst[col + i] =
				    FLOAT2INT_CLAMP(buf[i] * (mlib_d64)buf[i] *
				    dscale) >> 16;
			}
		}

/*
 * compiler bug - very slow code
 *
 *    for (i = 0; i < xsize; i ++) {
 *      dst[i] = FLOAT2INT_CLAMP(src[i] * (mlib_d64) src[i] * dscale) >> 16;
 *    }
 *
 */
		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_c_ImageSqrShift_U16(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_s32 buf[SIZE], num, col;
	mlib_d64 dscale;
	mlib_d64 sat = (mlib_d64)0x80000000;
	mlib_s32 i, j;

/* dscale = 2 ** (16 - shift) */
	SCALBN(dscale, 16 - shift);

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] = src[col + i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				mlib_d64 val =
				    buf[i] * (mlib_d64)buf[i] * dscale;
				SAT_U16(dst[col + i], val);
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_c_ImageSqrShift_S32(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_d64 dscale;
	mlib_s32 i, j;

/* dscale = 2 ** (-shift) */
	SCALBN(dscale, -shift);

/* if src and dst - 1 - D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dst[i] =
			    FLOAT2INT_CLAMP(src[i] * (mlib_d64)src[i] * dscale);
		}

		src += slb;
		dst += dlb;
	}
}

#endif
/* *********************************************************** */
