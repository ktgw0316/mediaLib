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

#pragma ident	"@(#)mlib_m_ImageConv5x5_16nw_1.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_conv5x5_16nw.c - convolve an 8-bit image with a 5x5 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status mlib_m_conv5x5_16nw(mlib_image            *src,
 *                                  const mlib_image      *dst,
 *                                  const mlib_s32        *kernel,
 *                                  mlib_s32 scalef_expon
 *                                  mlib_s32 cmask )
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
 *      A 2-D convolution (5x5 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <mlib_mmx_utils.h>

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
#define	GET_KERN()                                                     \
	for (j = 0; j < 5; j++) {                                      \
	    for (i = 0; i < 5; i++) {                                  \
		__m64 kk;                                              \
	                                                               \
		*(mlib_s16 *)&kk = (kern[5 * j + i] >> 16) & 0xFFFF;   \
		kk = _mm_unpacklo_pi16(kk, kk);                        \
		ker[j][i] = _mm_unpacklo_pi16(kk, kk);                 \
	    }                                                          \
	}                                                              \
	                                                               \
	shift = scalef_expon - 16

/* *********************************************************** */
#define	PREP_5x5()                                              \
	d0 = d1;                                                \
	d1 = (*sp++);                                           \
	mmx_mul_16x16(sum0, d0, ker[0][0]);                     \
	mmx_mul_16x16(sum1, d0, ker[1][0]);                     \
	mmx_mul_16x16(sum2, d0, ker[2][0]);                     \
	mmx_mul_16x16(sum3, d0, ker[3][0]);                     \
	aa = ALGN(d0, d1, 16);                                  \
	mmx_madd_16x16(sum0, aa, ker[0][1]);                    \
	mmx_madd_16x16(sum1, aa, ker[1][1]);                    \
	mmx_madd_16x16(sum2, aa, ker[2][1]);                    \
	mmx_madd_16x16(sum3, aa, ker[3][1]);                    \
	bb = ALGN(d0, d1, 32);                                  \
	mmx_madd_16x16(sum0, bb, ker[0][2]);                    \
	mmx_madd_16x16(sum1, bb, ker[1][2]);                    \
	mmx_madd_16x16(sum2, bb, ker[2][2]);                    \
	mmx_madd_16x16(sum3, bb, ker[3][2]);                    \
	cc = ALGN(d0, d1, 48);                                  \
	mmx_madd_16x16(sum0, cc, ker[0][3]);                    \
	mmx_madd_16x16(sum1, cc, ker[1][3]);                    \
	mmx_madd_16x16(sum2, cc, ker[2][3]);                    \
	mmx_madd_16x16(sum3, cc, ker[3][3]);                    \
	mmx_madd_16x16(sum0, d1, ker[0][4]);                    \
	mmx_madd_16x16(sum1, d1, ker[1][4]);                    \
	mmx_madd_16x16(sum2, d1, ker[2][4]);                    \
	mmx_madd_16x16(sum3, d1, ker[3][4]);                    \
	buff0[2 * i] = sum0h;                                   \
	buff1[2 * i] = sum1h;                                   \
	buff2[2 * i] = sum2h;                                   \
	buff3[2 * i] = sum3h;                                   \
	buff0[2 * i + 1] = sum0l;                               \
	buff1[2 * i + 1] = sum1l;                               \
	buff2[2 * i + 1] = sum2l;                               \
	buff3[2 * i + 1] = sum3l

/* *********************************************************** */
#define	CONV_5x5(xx, i)                                         \
	d0 = d1;                                                \
	d1 = (*sp++);                                           \
	mmx_mul_16x16(sum0, d0, ker[0][0]);                     \
	mmx_mul_16x16(sum1, d0, ker[1][0]);                     \
	mmx_mul_16x16(sum2, d0, ker[2][0]);                     \
	mmx_mul_16x16(sum3, d0, ker[3][0]);                     \
	mmx_mul_16x16(sum4, d0, ker[4][0]);                     \
	aa = ALGN(d0, d1, 16);                                  \
	mmx_madd_16x16(sum0, aa, ker[0][1]);                    \
	mmx_madd_16x16(sum1, aa, ker[1][1]);                    \
	mmx_madd_16x16(sum2, aa, ker[2][1]);                    \
	mmx_madd_16x16(sum3, aa, ker[3][1]);                    \
	mmx_madd_16x16(sum4, aa, ker[4][1]);                    \
	bb = ALGN(d0, d1, 32);                                  \
	mmx_madd_16x16(sum0, bb, ker[0][2]);                    \
	mmx_madd_16x16(sum1, bb, ker[1][2]);                    \
	mmx_madd_16x16(sum2, bb, ker[2][2]);                    \
	mmx_madd_16x16(sum3, bb, ker[3][2]);                    \
	mmx_madd_16x16(sum4, bb, ker[4][2]);                    \
	cc = ALGN(d0, d1, 48);                                  \
	mmx_madd_16x16(sum0, cc, ker[0][3]);                    \
	mmx_madd_16x16(sum1, cc, ker[1][3]);                    \
	mmx_madd_16x16(sum2, cc, ker[2][3]);                    \
	mmx_madd_16x16(sum3, cc, ker[3][3]);                    \
	mmx_madd_16x16(sum4, cc, ker[4][3]);                    \
	mmx_madd_16x16(sum0, d1, ker[0][4]);                    \
	mmx_madd_16x16(sum1, d1, ker[1][4]);                    \
	mmx_madd_16x16(sum2, d1, ker[2][4]);                    \
	mmx_madd_16x16(sum3, d1, ker[3][4]);                    \
	mmx_madd_16x16(sum4, d1, ker[4][4]);                    \
	prev0h = buff0[2 * i];                                  \
	prev1h = buff1[2 * i];                                  \
	prev2h = buff2[2 * i];                                  \
	prev3h = buff3[2 * i];                                  \
	prev0l = buff0[2 * i + 1];                              \
	prev1l = buff1[2 * i + 1];                              \
	prev2l = buff2[2 * i + 1];                              \
	prev3l = buff3[2 * i + 1];                              \
	sum4h = _mm_add_pi32(sum4h, prev0h);                    \
	sum4l = _mm_add_pi32(sum4l, prev0l);                    \
	sum4h = _mm_add_pi32(sum4h, prev1h);                    \
	sum4l = _mm_add_pi32(sum4l, prev1l);                    \
	sum4h = _mm_add_pi32(sum4h, prev2h);                    \
	sum4l = _mm_add_pi32(sum4l, prev2l);                    \
	sum4h = _mm_add_pi32(sum4h, prev3h);                    \
	sum4l = _mm_add_pi32(sum4l, prev3l);                    \
	buff0[2 * i] = sum0h;                                   \
	buff1[2 * i] = sum1h;                                   \
	buff2[2 * i] = sum2h;                                   \
	buff3[2 * i] = sum3h;                                   \
	buff0[2 * i + 1] = sum0l;                               \
	buff1[2 * i + 1] = sum1l;                               \
	buff2[2 * i + 1] = sum2l;                               \
	buff3[2 * i + 1] = sum3l;                               \
	sum4h = _mm_srai_pi32(sum4h, shift);                    \
	sum4l = _mm_srai_pi32(sum4l, shift);                    \
	rr = _mm_packs_pi32(sum4h, sum4l)

/* *********************************************************** */
/*
 * buffers distribution
 *
 * 0
 * 1 2
 * 3 4 5
 * 6 7 8 9    <-- previous line
 *
 * 0 2 5 9 z  <-- current line
 *
 */

mlib_status
mlib_m_conv5x5_16nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kern,
    mlib_s32 scalef_expon)
{
	__m64 *pbuff, *buff_arr[20], **pbuff_arr = buff_arr;
	__m64 *buff0, *buff1, *buff2, *buff3;
	GET_SRC_DST_PARAMETERS(mlib_s16);
	__m64 ker[5][5];
	__m64 d0, d1, aa, bb, cc, rr, tmpa, tmpb;
	__m64 prev0h, prev1h, prev2h, prev3h, sum0h, sum1h, sum2h, sum3h, sum4h,
	    tmph;
	__m64 prev0l, prev1l, prev2l, prev3l, sum0l, sum1l, sum2l, sum3l, sum4l,
	    tmpl;
	__m64 *sp, *dp;
	mlib_s32 shift, ind;
	mlib_s32 row, wid4, i, j;

	width -= 4;
	height -= 4;
	dl += 2 * (dll + 1);

	wid4 = (width + 7) / 4;
	pbuff = mlib_malloc(sizeof (__m64) * 20 * wid4);

	GET_KERN();

	for (i = 0; i < 10; i++) {
		buff_arr[i] = pbuff + i * 2 * wid4;
	}

	ind = 0;
	for (j = 1; j <= 4; j++) {
		buff0 = buff_arr[ind];
		buff1 = buff_arr[ind + 1];
		buff2 = buff_arr[ind + 2];
		buff3 = buff_arr[ind + 3];

		sp = (__m64 *) sl;
		d1 = (*sp++);

		for (i = 0; i < wid4; i++) {
			PREP_5x5();
		}

		sl += sll;
		ind += j;
	}

	for (row = 0; row < height; row++) {
		sp = (__m64 *) sl;
		dp = (__m64 *) dl;

		buff0 = pbuff_arr[0];
		buff1 = pbuff_arr[2];
		buff2 = pbuff_arr[5];
		buff3 = pbuff_arr[9];

		d1 = (*sp++);

		for (i = 0; i < width / 4; i++) {
			CONV_5x5(hi, i);

			dp[i] = rr;
		}

		if (width & 3) {
			__m64 mask =
			    ((__m64 *) mlib_mask64_arr)[2 * (width & 3)];

			CONV_5x5(hi, i);

			dp[i] =
			    _mm_or_si64(_mm_and_si64(mask, rr),
			    _mm_andnot_si64(mask, dp[i]));
		}

		ind = (pbuff_arr == buff_arr) ? 10 : -10;
		pbuff_arr[ind + 0] = pbuff_arr[1];
		pbuff_arr[ind + 1] = pbuff_arr[3];
		pbuff_arr[ind + 2] = pbuff_arr[4];
		pbuff_arr[ind + 3] = pbuff_arr[6];
		pbuff_arr[ind + 4] = pbuff_arr[7];
		pbuff_arr[ind + 5] = pbuff_arr[8];
		pbuff_arr[ind + 6] = pbuff_arr[0];
		pbuff_arr[ind + 7] = pbuff_arr[2];
		pbuff_arr[ind + 8] = pbuff_arr[5];
		pbuff_arr[ind + 9] = pbuff_arr[9];
		pbuff_arr += ind;

		sl += sll;
		dl += dll;
	}

	_mm_empty();
	mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
