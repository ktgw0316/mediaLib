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

#pragma ident	"@(#)mlib_m_ImageConv3x3_16nw_4.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_conv3x3_16nw.c - convolve an 8-bit image with a 3x3 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status mlib_m_conv3x3_16nw(mlib_image       *dst,
 *                                  const mlib_image *src,
 *                                  const mlib_s32   *kernel,
 *                                  mlib_s32         scalef_expon,
 *                                  mlib_s32         cmask)
 *
 * ARGUMENT
 *      src           Pointer to structure of source image
 *      dst           Pointer to structure of destination image
 *      kernel        Convolution kernel
 *      scalef_expons The scaling factor to convert the input integer
 *                    coefficients into floating-point coefficients:
 *                    floating-point coefficient =
 *                    integer coefficient * 2^(-scale)
 *      cmask         Channel mask to indicate the channels to be convolved.
 *                    Each bit of which represents a channel in the image. The
 *                    channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      A 2-D convolution (3x3 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */
#define	BUFF_LINE	256

/* *********************************************************** */
#define	ALGN(x, y, sh)                                                        \
		_mm_or_si64(_mm_srli_si64(x, sh), _mm_slli_si64(y, 64 - sh))

/* *********************************************************** */
#define	GET_SRC_DST_PARAMETERS(type)                               \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                               \
		dll = mlib_ImageGetStride(dst) / sizeof (type);    \
	    type *                                                 \
		sl = (type *) mlib_ImageGetData(src);              \
	    type *                                                 \
		dl = (type *) mlib_ImageGetData(dst)

/* *********************************************************** */
#define	GET_KERN()                                              \
	* (mlib_s16 *)&ker1 = (kern[0] >> 16) & 0xFFFF;         \
	*(mlib_s16 *)&ker2 = (kern[1] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker3 = (kern[2] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker4 = (kern[3] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker5 = (kern[4] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker6 = (kern[5] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker7 = (kern[6] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker8 = (kern[7] >> 16) & 0xFFFF;          \
	*(mlib_s16 *)&ker9 = (kern[8] >> 16) & 0xFFFF;          \
	ker1 = _mm_unpacklo_pi16(ker1, ker1);                   \
	ker2 = _mm_unpacklo_pi16(ker2, ker2);                   \
	ker3 = _mm_unpacklo_pi16(ker3, ker3);                   \
	ker4 = _mm_unpacklo_pi16(ker4, ker4);                   \
	ker5 = _mm_unpacklo_pi16(ker5, ker5);                   \
	ker6 = _mm_unpacklo_pi16(ker6, ker6);                   \
	ker7 = _mm_unpacklo_pi16(ker7, ker7);                   \
	ker8 = _mm_unpacklo_pi16(ker8, ker8);                   \
	ker9 = _mm_unpacklo_pi16(ker9, ker9);                   \
	ker1 = _mm_unpacklo_pi16(ker1, ker1);                   \
	ker2 = _mm_unpacklo_pi16(ker2, ker2);                   \
	ker3 = _mm_unpacklo_pi16(ker3, ker3);                   \
	ker4 = _mm_unpacklo_pi16(ker4, ker4);                   \
	ker5 = _mm_unpacklo_pi16(ker5, ker5);                   \
	ker6 = _mm_unpacklo_pi16(ker6, ker6);                   \
	ker7 = _mm_unpacklo_pi16(ker7, ker7);                   \
	ker8 = _mm_unpacklo_pi16(ker8, ker8);                   \
	ker9 = _mm_unpacklo_pi16(ker9, ker9);                   \
	shift = scalef_expon - 16

/* *********************************************************** */
#define	PREP_3x3(i)                                             \
	d0 = d1;                                                \
	d1 = d2;                                                \
	d2 = sp[i];                                             \
	mmx_mul_16x16(sum0, d0, ker1);                          \
	mmx_mul_16x16(sum1, d0, ker4);                          \
	mmx_madd_16x16(sum0, d1, ker2);                         \
	mmx_madd_16x16(sum1, d1, ker5);                         \
	mmx_madd_16x16(sum0, d2, ker3);                         \
	mmx_madd_16x16(sum1, d2, ker6);                         \
	buff1[2 * i] = sum0h;                                   \
	buff2[2 * i] = sum1h;                                   \
	buff1[2 * i + 1] = sum0l;                               \
	buff2[2 * i + 1] = sum1l

/* *********************************************************** */
#define	CONV_3x3(i)                                             \
	d0 = d1;                                                \
	d1 = d2;                                                \
	d2 = sp[i];                                             \
	prev0h = buff0[2 * i];                                  \
	prev1h = buff2[2 * i];                                  \
	prev0l = buff0[2 * i + 1];                              \
	prev1l = buff2[2 * i + 1];                              \
	mmx_mul_16x16(sum0, d0, ker1);                          \
	mmx_mul_16x16(sum1, d0, ker4);                          \
	mmx_mul_16x16(sum2, d0, ker7);                          \
	mmx_madd_16x16(sum0, d1, ker2);                         \
	mmx_madd_16x16(sum1, d1, ker5);                         \
	mmx_madd_16x16(sum2, d1, ker8);                         \
	mmx_madd_16x16(sum0, d2, ker3);                         \
	mmx_madd_16x16(sum1, d2, ker6);                         \
	mmx_madd_16x16(sum2, d2, ker9);                         \
	mmx_add_d32(sum2, prev0);                               \
	mmx_add_d32(sum2, prev1);                               \
	buff0[2 * i] = sum0h;                                   \
	buff2[2 * i] = sum1h;                                   \
	buff0[2 * i + 1] = sum0l;                               \
	buff2[2 * i + 1] = sum1l;                               \
	sum2h = _mm_srai_pi32(sum2h, shift);                    \
	sum2l = _mm_srai_pi32(sum2l, shift);                    \
	rr = _mm_packs_pi32(sum2h, sum2l)

/* *********************************************************** */
#define	NCHAN	4

mlib_status
mlib_m_conv3x3_16nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon)
{
	__m64 buff_loc[6 * BUFF_LINE], *pbuff = buff_loc;
	__m64 *buff0, *buff1, *buff2, *buffT;
	GET_SRC_DST_PARAMETERS(mlib_s16);
	__m64 ker1, ker2, ker3, ker4, ker5, ker6, ker7, ker8, ker9;
	__m64 d0, d1, d2, rr, tmpa, tmpb;
	__m64 prev0h, prev1h, sum0h, sum1h, sum2h, tmph;
	__m64 prev0l, prev1l, sum0l, sum1l, sum2l, tmpl;
	__m64 *sp, *dp;
	mlib_s32 shift;
	mlib_s32 row, wid4, i, j;

	width -= 2;
	height -= 2;
	width *= NCHAN;
	dl += dll + NCHAN;

	wid4 = (width + 3) / 4;

	if (wid4 > BUFF_LINE) {
		pbuff = mlib_malloc(sizeof (__m64) * 6 * wid4);
	}

	GET_KERN();

	buff0 = pbuff;
	buff1 = buff0 + 2 * wid4;
	buff2 = buff1 + 2 * wid4;

	for (j = 0; j < 2; j++) {
		sp = (__m64 *) sl;

		d1 = (*sp++);
		d2 = (*sp++);
		for (i = 0; i < wid4; i++) {
			PREP_3x3(i);
		}

		sl += sll;

		if (j == 0) {
			buffT = buff1;
			buff1 = buff0;
			buff0 = buffT;
		}
	}

	for (row = 0; row < height; row++) {
		sp = (__m64 *) sl;
		dp = (__m64 *) dl;

		d1 = (*sp++);
		d2 = (*sp++);
		for (i = 0; i < width / 4; i++) {
			CONV_3x3(i);
			dp[i] = rr;
		}

		if (width & 3) {
			__m64 mask =
			    ((__m64 *) mlib_mask64_arr)[2 * (width & 3)];

			CONV_3x3(i);

			dp[i] =
			    _mm_or_si64(_mm_and_si64(mask, rr),
			    _mm_andnot_si64(mask, dp[i]));
		}

		buffT = buff1;
		buff1 = buff0;
		buff0 = buffT;

		sl += sll;
		dl += dll;
	}

	_mm_empty();

	if (pbuff != buff_loc)
		mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
