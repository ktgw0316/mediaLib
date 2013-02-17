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

#pragma ident	"@(#)mlib_s_ImageMulAlpha_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMulAlpha_Fp - multiply color channels by the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMulAlpha_Fp(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        mlib_s32         cmask)
 *
 * ARGUMENTS
 *    dst     Pointer to destination image.
 *    src     Pointer to source image.
 *    cmask   Channel mask to indicate the alpha channel.
 *            Each bit of which represents a channel in the image. The
 *            channel corresponded to the 1 bit is the alpha channel.
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels. They can have 2, 3 or 4 channels.
 *      They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Multiply color channels by the alpha channel in the way of
 *      pixel by pixel.
 *
 *  The image must have at least two channels.
 *
 *  For an MLIB_FLOAT and MLIB_DOUBLE image
 *   dst->data[i][j] = src->data[i][j] * src->data[i][a]
 *   where, j != a
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMulAlpha_Fp = __mlib_ImageMulAlpha_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMulAlpha_Fp) mlib_ImageMulAlpha_Fp
    __attribute__((weak, alias("__mlib_ImageMulAlpha_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageMulAlpha_F32_2(
    mlib_f32 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_f32 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha);

static void mlib_ImageMulAlpha_F32_3(
    mlib_f32 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_f32 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha);

static void mlib_ImageMulAlpha_F32_4(
    mlib_f32 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_f32 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha);

static void mlib_ImageMulAlpha_D64_2(
    mlib_d64 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_d64 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha);

static void mlib_ImageMulAlpha_D64_3(
    mlib_d64 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_d64 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha);

static void mlib_ImageMulAlpha_D64_4(
    mlib_d64 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_d64 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha);

/* *********************************************************** */

mlib_status
__mlib_ImageMulAlpha_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	mlib_s32 alpha;
	mlib_s32 dst_chan, dst_width, dst_height, dst_stride, src_stride;
	mlib_type dst_type;
	mlib_u8 *dst_data, *src_data;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(src, dst);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dst_type, dst_chan, dst_width,
	    dst_height, dst_stride, dst_data);
	src_stride = mlib_ImageGetStride(src);
	src_data = mlib_ImageGetData(src);

	if ((dst_chan < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; (cmask & 0x1) == 0; alpha++)
		cmask >>= 1;

	if ((cmask >> 1) != 0)
		return (MLIB_OUTOFRANGE);

/*  0 <= alpha < channel */
	alpha = dst_chan - alpha - 1;

	if (0 > alpha && alpha >= dst_chan)
		return (MLIB_OUTOFRANGE);

	if (dst_type == MLIB_FLOAT) {
		if (dst_chan == 2) {
			mlib_ImageMulAlpha_F32_2((mlib_f32 *)dst_data,
			    dst_stride,
			    (mlib_f32 *)src_data,
			    src_stride, dst_height, dst_width, alpha);
		} else if (dst_chan == 3) {
			mlib_ImageMulAlpha_F32_3((mlib_f32 *)dst_data,
			    dst_stride,
			    (mlib_f32 *)src_data,
			    src_stride, dst_height, dst_width, alpha);
		} else if (dst_chan == 4) {
			mlib_ImageMulAlpha_F32_4((mlib_f32 *)dst_data,
			    dst_stride,
			    (mlib_f32 *)src_data,
			    src_stride, dst_height, dst_width, alpha);
		}

		return (MLIB_SUCCESS);
	} else if (dst_type == MLIB_DOUBLE) {
		if (dst_chan == 2) {
			mlib_ImageMulAlpha_D64_2((mlib_d64 *)dst_data,
			    dst_stride,
			    (mlib_d64 *)src_data,
			    src_stride, dst_height, dst_width, alpha);
		} else if (dst_chan == 3) {
			mlib_ImageMulAlpha_D64_3((mlib_d64 *)dst_data,
			    dst_stride,
			    (mlib_d64 *)src_data,
			    src_stride, dst_height, dst_width, alpha);
		} else if (dst_chan == 4) {
			mlib_ImageMulAlpha_D64_4((mlib_d64 *)dst_data,
			    dst_stride,
			    (mlib_d64 *)src_data,
			    src_stride, dst_height, dst_width, alpha);
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_F32_2(
    mlib_f32 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_f32 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha)
{
	mlib_s32 i, j;

	width *= 2;

	if (alpha == 0) {
			__m128 xone = _mm_set1_ps(1.0);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width - 2; j += 4) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(
				    ptr_src[j + 2], 1, ptr_src[j], 1);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storeu_ps(ptr_dst + j, xdst1);
			}

			if (j < width) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(1, 1, ptr_src[j], 1);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storel_pi((void *)(ptr_dst + j), xdst1);
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width - 2; j += 4) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(1,
				    ptr_src[j + 3], 1, ptr_src[j + 1]);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storeu_ps(ptr_dst + j, xdst1);
			}

			if (j < width) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(1, 1, 1, ptr_src[j + 1]);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storel_pi((void *)(ptr_dst + j), xdst1);
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_F32_3(
    mlib_f32 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_f32 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha)
{
	mlib_s32 i, j;

	width *= 3;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width - 9; j += 12) {
				__m128 xalp1, xalp2, xalp3;
				__m128 xsrc1, xsrc2, xsrc3;
				__m128 xdst1, xdst2, xdst3;
				xalp1 =
				    _mm_set_ps(1, ptr_src[j], ptr_src[j], 1);
				xalp2 = _mm_set_ps(ptr_src[j + 6], 1,
				    ptr_src[j + 3], ptr_src[j + 3]);
				xalp3 = _mm_set_ps(ptr_src[j + 9],
				    ptr_src[j + 9], 1, ptr_src[j + 6]);
				xsrc1 = _mm_loadu_ps(ptr_src + j);
				xsrc2 = _mm_loadu_ps(ptr_src + j + 4);
				xsrc3 = _mm_loadu_ps(ptr_src + j + 8);
				xdst1 = _mm_mul_ps(xsrc1, xalp1);
				xdst2 = _mm_mul_ps(xsrc2, xalp2);
				xdst3 = _mm_mul_ps(xsrc3, xalp3);
				_mm_storeu_ps((ptr_dst + j), xdst1);
				_mm_storeu_ps((ptr_dst + j + 4), xdst2);
				_mm_storeu_ps((ptr_dst + j + 8), xdst3);
			}
			if (j < width - 3) {
				__m128 xalp1, xalp2;
				__m128 xsrc1, xsrc2;
				__m128 xdst1, xdst2;
				xalp1 =
				    _mm_set_ps(1, ptr_src[j], ptr_src[j], 1);
				xalp2 = _mm_set_ps(1, 1, ptr_src[j + 3],
				    ptr_src[j + 3]);
				xsrc1 = _mm_loadu_ps(ptr_src + j);
				xsrc2 = _mm_loadu_ps(ptr_src + j + 4);
				xdst1 = _mm_mul_ps(xsrc1, xalp1);
				xdst2 = _mm_mul_ps(xsrc2, xalp2);
				_mm_storeu_ps((ptr_dst + j), xdst1);
				_mm_storel_pi((void *)(ptr_dst + j + 4), xdst2);
				j += 6;
			}

			if (j < width) {
				mlib_f32 src1 = ptr_src[j];
				mlib_f32 src2 = ptr_src[j + 1];
				mlib_f32 src3 = ptr_src[j + 2];

				ptr_dst[j] = src1;
				ptr_dst[j + 1] = src2 * src1;
				ptr_dst[j + 2] = src3 * src1;
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width - 9; j += 12) {
				__m128 xalp1, xalp2, xalp3;
				__m128 xsrc1, xsrc2, xsrc3;
				__m128 xdst1, xdst2, xdst3;
				xalp1 = _mm_set_ps(ptr_src[j + 4],
				    ptr_src[j + 1], 1, ptr_src[j + 1]);
				xalp2 = _mm_set_ps(1, ptr_src[j + 7],
				    ptr_src[j + 4], 1);
				xalp3 = _mm_set_ps(ptr_src[j + 10], 1,
				    ptr_src[j + 10], ptr_src[j + 7]);
				xsrc1 = _mm_loadu_ps(ptr_src + j);
				xsrc2 = _mm_loadu_ps(ptr_src + j + 4);
				xsrc3 = _mm_loadu_ps(ptr_src + j + 8);
				xdst1 = _mm_mul_ps(xsrc1, xalp1);
				xdst2 = _mm_mul_ps(xsrc2, xalp2);
				xdst3 = _mm_mul_ps(xsrc3, xalp3);
				_mm_storeu_ps((ptr_dst + j), xdst1);
				_mm_storeu_ps((ptr_dst + j + 4), xdst2);
				_mm_storeu_ps((ptr_dst + j + 8), xdst3);
			}

			if (j < width - 3) {
				__m128 xalp1, xalp2;
				__m128 xsrc1, xsrc2;
				__m128 xdst1, xdst2;
				xalp1 = _mm_set_ps(ptr_src[j + 4],
				    ptr_src[j + 1], 1, ptr_src[j + 1]);
				xalp2 = _mm_set_ps(1, 1, ptr_src[j + 4], 1);
				xsrc1 = _mm_loadu_ps(ptr_src + j);
				xsrc2 = _mm_loadu_ps(ptr_src + j + 4);
				xdst1 = _mm_mul_ps(xsrc1, xalp1);
				xdst2 = _mm_mul_ps(xsrc2, xalp2);
				_mm_storeu_ps((ptr_dst + j), xdst1);
				_mm_storel_pi((void *)(ptr_dst + j + 4), xdst2);
				j += 6;
			}

			if (j < width) {
				mlib_f32 src1 = ptr_src[j];
				mlib_f32 src2 = ptr_src[j + 1];
				mlib_f32 src3 = ptr_src[j + 2];

				ptr_dst[j] = src1 * src2;
				ptr_dst[j + 1] = src2;
				ptr_dst[j + 2] = src3 * src2;
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width - 9; j += 12) {
				__m128 xalp1, xalp2, xalp3;
				__m128 xsrc1, xsrc2, xsrc3;
				__m128 xdst1, xdst2, xdst3;
				xalp1 = _mm_set_ps(ptr_src[j + 5], 1,
				    ptr_src[j + 2], ptr_src[j + 2]);
				xalp2 = _mm_set_ps(ptr_src[j + 8],
				    ptr_src[j + 8], 1, ptr_src[j + 5]);
				xalp3 = _mm_set_ps(1, ptr_src[j + 11],
				    ptr_src[j + 11], 1);
				xsrc1 = _mm_loadu_ps(ptr_src + j);
				xsrc2 = _mm_loadu_ps(ptr_src + j + 4);
				xsrc3 = _mm_loadu_ps(ptr_src + j + 8);
				xdst1 = _mm_mul_ps(xsrc1, xalp1);
				xdst2 = _mm_mul_ps(xsrc2, xalp2);
				xdst3 = _mm_mul_ps(xsrc3, xalp3);
				_mm_storeu_ps((ptr_dst + j), xdst1);
				_mm_storeu_ps((ptr_dst + j + 4), xdst2);
				_mm_storeu_ps((ptr_dst + j + 8), xdst3);
			}

			if (j < width - 3) {
				__m128 xalp1, xalp2;
				__m128 xsrc1, xsrc2;
				__m128 xdst1, xdst2;
				xalp1 = _mm_set_ps(ptr_src[j + 5], 1,
				    ptr_src[j + 2], ptr_src[j + 2]);
				xalp2 = _mm_set_ps(1, 1, 1, ptr_src[j + 5]);
				xsrc1 = _mm_loadu_ps(ptr_src + j);
				xsrc2 = _mm_loadu_ps(ptr_src + j + 4);
				xdst1 = _mm_mul_ps(xsrc1, xalp1);
				xdst2 = _mm_mul_ps(xsrc2, xalp2);
				_mm_storeu_ps((ptr_dst + j), xdst1);
				_mm_storel_pi((void *)(ptr_dst + j + 4), xdst2);
				j += 6;
			}

			if (j < width) {
				mlib_f32 src1 = ptr_src[j];
				mlib_f32 src2 = ptr_src[j + 1];
				mlib_f32 src3 = ptr_src[j + 2];

				ptr_dst[j] = src1 * src3;
				ptr_dst[j + 1] = src2 * src3;
				ptr_dst[j + 2] = src3;
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_F32_4(
    mlib_f32 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_f32 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha)
{
	mlib_s32 i, j;

	width *= 4;

	if (alpha == 0) {
		__m128 xone = _mm_set1_ps(1.0);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_load1_ps(ptr_src + j);
				xsrc1 = _mm_move_ss(xsrc1, xone);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storeu_ps((ptr_dst + j), xdst1);
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(ptr_src[j + 1],
				    ptr_src[j + 1], 1, ptr_src[j + 1]);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storeu_ps((ptr_dst + j), xdst1);
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(ptr_src[j + 2], 1,
				    ptr_src[j + 2], ptr_src[j + 2]);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storeu_ps((ptr_dst + j), xdst1);
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128 xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_set_ps(1, ptr_src[j + 3],
				    ptr_src[j + 3], ptr_src[j + 3]);
				xsrc2 = _mm_loadu_ps(ptr_src + j);
				xdst1 = _mm_mul_ps(xsrc2, xsrc1);
				_mm_storeu_ps((ptr_dst + j), xdst1);
			}

			ptr_dst =
			    (mlib_f32 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_f32 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_D64_2(
    mlib_d64 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_d64 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha)
{
	mlib_s32 i, j;

	width *= 2;

	if (alpha == 0) {
		__m128d xone = _mm_set1_pd(1.0);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 2) {
				__m128d xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_loadu_pd(ptr_src + j);
				xsrc2 = _mm_unpacklo_pd(xone, xsrc1);
				xdst1 = _mm_mul_pd(xsrc1, xsrc2);
				_mm_storeu_pd(ptr_dst + j, xdst1);
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else {
		__m128d xone = _mm_set1_pd(1.0);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 2) {
				__m128d xsrc1, xsrc2, xdst1;
				xsrc1 = _mm_loadu_pd(ptr_src + j);
				xsrc2 = _mm_unpackhi_pd(xsrc1, xone);
				xdst1 = _mm_mul_pd(xsrc1, xsrc2);
				_mm_storeu_pd(ptr_dst + j, xdst1);
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_D64_3(
    mlib_d64 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_d64 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha)
{
	mlib_s32 i, j;

	width *= 3;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 3) {
				mlib_d64 src1 = ptr_src[j];
				mlib_d64 src2 = ptr_src[j + 1];
				mlib_d64 src3 = ptr_src[j + 2];

				ptr_dst[j] = src1;
				ptr_dst[j + 1] = src2 * src1;
				ptr_dst[j + 2] = src3 * src1;
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 3) {
				mlib_d64 src1 = ptr_src[j];
				mlib_d64 src2 = ptr_src[j + 1];
				mlib_d64 src3 = ptr_src[j + 2];

				ptr_dst[j] = src1 * src2;
				ptr_dst[j + 1] = src2;
				ptr_dst[j + 2] = src3 * src2;
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 3) {
				mlib_d64 src1 = ptr_src[j];
				mlib_d64 src2 = ptr_src[j + 1];
				mlib_d64 src3 = ptr_src[j + 2];

				ptr_dst[j] = src1 * src3;
				ptr_dst[j + 1] = src2 * src3;
				ptr_dst[j + 2] = src3;
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_D64_4(
    mlib_d64 *ptr_dst,
    mlib_s32 dst_stride,
    mlib_d64 *ptr_src,
    mlib_s32 src_stride,
    mlib_s32 height,
    mlib_s32 width,
    mlib_u32 alpha)
{
	mlib_s32 i, j;

	width *= 4;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128d xsrc1, xsrc2, xsrc3;
				__m128d xtrc1, xtrc2, xtrc3;
				xsrc1 = _mm_load1_pd(ptr_src + j);
				xsrc2 = _mm_loadu_pd(ptr_src + j);
				xsrc3 = _mm_loadu_pd(ptr_src + j + 2);
				xtrc1 = _mm_mul_pd(xsrc2, xsrc1);
				xtrc2 = _mm_mul_pd(xsrc3, xsrc1);
				xtrc3 = _mm_unpackhi_pd(xsrc1, xtrc1);
				_mm_storeu_pd((ptr_dst + j), xtrc3);
				_mm_storeu_pd((ptr_dst + j + 2), xtrc2);
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128d xsrc1, xsrc2, xsrc3;
				__m128d xtrc1, xtrc2, xtrc3;
				xsrc1 = _mm_load1_pd(ptr_src + j + 1);
				xsrc2 = _mm_loadu_pd(ptr_src + j);
				xsrc3 = _mm_loadu_pd(ptr_src + j + 2);
				xtrc1 = _mm_mul_sd(xsrc2, xsrc1);
				xtrc2 = _mm_mul_pd(xsrc3, xsrc1);
				_mm_storeu_pd((ptr_dst + j), xtrc1);
				_mm_storeu_pd((ptr_dst + j + 2), xtrc2);
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128d xsrc1, xsrc2, xsrc3;
				__m128d xtrc1, xtrc2, xtrc3;
				xsrc1 = _mm_load1_pd(ptr_src + j + 2);
				xsrc2 = _mm_loadu_pd(ptr_src + j);
				xsrc3 = _mm_loadu_pd(ptr_src + j + 2);
				xtrc1 = _mm_mul_pd(xsrc2, xsrc1);
				xtrc2 = _mm_mul_pd(xsrc3, xsrc1);
				xtrc3 = _mm_unpackhi_pd(xsrc1, xtrc2);
				_mm_storeu_pd((ptr_dst + j), xtrc1);
				_mm_storeu_pd((ptr_dst + j + 2), xtrc3);
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j += 4) {
				__m128d xsrc1, xsrc2, xsrc3;
				__m128d xtrc1, xtrc2, xtrc3;
				xsrc1 = _mm_load1_pd(ptr_src + j + 3);
				xsrc2 = _mm_loadu_pd(ptr_src + j);
				xsrc3 = _mm_loadu_pd(ptr_src + j + 2);
				xtrc1 = _mm_mul_pd(xsrc2, xsrc1);
				xtrc2 = _mm_mul_sd(xsrc3, xsrc1);
				_mm_storeu_pd((ptr_dst + j), xtrc1);
				_mm_storeu_pd((ptr_dst + j + 2), xtrc2);
			}

			ptr_dst =
			    (mlib_d64 *)(((mlib_u8 *)ptr_dst) + dst_stride);
			ptr_src =
			    (mlib_d64 *)(((mlib_u8 *)ptr_src) + src_stride);
		}
	}
}

/* *********************************************************** */
