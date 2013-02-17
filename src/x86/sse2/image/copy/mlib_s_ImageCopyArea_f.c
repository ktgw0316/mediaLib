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

#pragma ident	"@(#)mlib_s_ImageCopyArea_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageCopyArea_a64_r   - 2-D, Aligned8, width 8x, reversed
 *      mlib_s_ImageCopyArea_a128_r   - 2-D, Aligned16, width 16x, reversed
 *      mlib_ImageCopyArea_na_r     - BYTE, non-aligned, reversed
 *      mlib_ImageCopyArea_bit_al_r - BIT, aligned, reversed
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      sp        pointer to source data
 *      dp        pointer to destination data
 *      size      size in 8-bytes, bytes, or SHORTs
 *      width     image width in 8-bytes
 *      height    image height in lines
 *      ystride   image line stride in 8-bytes
 *      s_offset  source image line bit offset
 *      d_offset  destination image line bit offset
 *
 * DESCRIPTION
 *      Copy a specified rectangular area from one portion of the
 *      image to another (within the same image) --
 *      low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageCopyArea.c for
 *      loop unrolling and structure clarity.
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_s_ImageCopyArea_f.h>

/* *********************************************************** */

/*
 *      Both source and destination data are 16-byte aligned.
 *      And ystride and width are in 16-bytes.
 *
 *      Data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_s_ImageCopyArea_a128_r(
    __m128i *sp,
    __m128i *dp,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 ystride)
{
/* 8-byte aligned pointer for line */
	__m128i *spl;

/* 8-byte aligned pointer for line */
	__m128i *dpl;

/* indices for x, y */
	mlib_s32 i, j;

	spl = sp;
	dpl = dp;

/* row loop */
	for (j = 0; j < height; j++) {
/* 8-byte column loop */
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			*dp-- = *sp--;
		}

		sp = spl -= ystride;
		dp = dpl -= ystride;
	}
}
/* *********************************************************** */

/*
 *      Both source and destination data are 8-byte aligned.
 *      And ystride and width are in 8-bytes.
 *
 *      Data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_s_ImageCopyArea_a64_r(
    mlib_d64 *sp,
    mlib_d64 *dp,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 ystride)
{
/* 8-byte aligned pointer for line */
	mlib_d64 *spl;

/* 8-byte aligned pointer for line */
	mlib_d64 *dpl;

/* indices for x, y */
	mlib_s32 i, j;

	spl = sp;
	dpl = dp;

/* row loop */
	for (j = 0; j < height; j++) {
/* 8-byte column loop */
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			*dp-- = *sp--;
		}

		sp = spl -= ystride;
		dp = dpl -= ystride;
	}
}

/* *********************************************************** */

/*
 *      Either source or destination data are not 8-byte aligned.
 *      And size is is in bytes.
 *
 *      Data are copied beginning from the end and proceeding
 *      to the star of the area.
 */

void
mlib_ImageCopyArea_na_r(
    mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size)
{

	mlib_u8 *dp = da + 1;
	mlib_u8 *sp = sa + 1;

	mlib_s32 i, n = size;

	for (; (n > 0) && (((mlib_addr)dp) & 15); n--)
		(*--dp) = (*--sp);

	for (; n > 15; n -= 16) {
		sp -= 16;
		dp -= 16;
		_mm_store_si128((__m128i *)dp,
		    _mm_loadu_si128((__m128i *)sp));
	}

	for (; n > 0; n--) {
		(*--dp) = (*--sp);
	}
}

/* *********************************************************** */

/*
 * Both bit offsets of source and destination are the same
 */

void
mlib_ImageCopyArea_bit_al_r(
    mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset)
{
	mlib_s32 b_size, i, j;
	mlib_u8 mask0 = 0xFF;
	mlib_u8 src, mask;

	if (size <= offset) {
		mask = mask0 << (8 - size);
		mask >>= (offset - size);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
		return;
	}

	mask = mask0 << (8 - offset);
	src = da[0];
	da[0] = (src & (~mask)) | (sa[0] & mask);
	size = size - offset;
/* size in bytes */
	b_size = size >> 3;

	for (j = 0; (j < b_size) && (((mlib_addr)da & 15) != 0); j++)
		(*--da) = (*--sa);

	if ((((mlib_addr)sa & 15) ^ ((mlib_addr)da & 15)) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (b_size - 16); j += 16) {
			sa -= 16;
			da -= 16;
			*(__m128i *) da = *(__m128i *) sa;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (b_size - 16); j += 16) {
			sa -= 16;
			da -= 16;
			_mm_store_si128((__m128i *)da,
			    _mm_loadu_si128((__m128i *)sa));

		}
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < b_size; j++)
		(*--da) = (*--sa);

	j = size & 7;

	if (j > 0) {
		da--;
		sa--;
		mask = mask0 >> (8 - j);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
	}
}

/* *********************************************************** */
