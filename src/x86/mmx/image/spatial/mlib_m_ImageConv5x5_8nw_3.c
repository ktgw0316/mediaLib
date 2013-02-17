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

#pragma ident	"@(#)mlib_m_ImageConv5x5_8nw_3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_conv5x5_8nw.c - convolve an 8-bit image with a 5x5 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status mlib_m_conv5x5_8nw(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 const mlib_s32   *kernel,
 *                                 mlib_s32         scalef_expon,
 *                                 mlib_s32         cmask)
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
	shift = scalef_expon - 25

/* *********************************************************** */
#define	PREP_5x5()                                                  \
	d0 = d1;                                                    \
	d1 = d2;                                                    \
	d2 = d3;                                                    \
	UNPACK_SRC(d3, lo);                                         \
	sum0 = _mm_mulhi_pi16(d0, ker[0][0]);                       \
	sum1 = _mm_mulhi_pi16(d0, ker[1][0]);                       \
	sum2 = _mm_mulhi_pi16(d0, ker[2][0]);                       \
	sum3 = _mm_mulhi_pi16(d0, ker[3][0]);                       \
	aa = ALGN(d0, d1, 48);                                      \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(aa, ker[0][1]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(aa, ker[1][1]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(aa, ker[2][1]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(aa, ker[3][1]));   \
	bb = ALGN(d1, d2, 32);                                      \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(bb, ker[0][2]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(bb, ker[1][2]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(bb, ker[2][2]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(bb, ker[3][2]));   \
	cc = ALGN(d2, d3, 16);                                      \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(cc, ker[0][3]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(cc, ker[1][3]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(cc, ker[2][3]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(cc, ker[3][3]));   \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(d3, ker[0][4]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(d3, ker[1][4]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(d3, ker[2][4]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(d3, ker[3][4]));   \
	buff0[i] = sum0;                                            \
	buff1[i] = sum1;                                            \
	buff2[i] = sum2;                                            \
	buff3[i] = sum3

/* *********************************************************** */
#define	CONV_5x5(xx, i)                                             \
	d0 = d1;                                                    \
	d1 = d2;                                                    \
	d2 = d3;                                                    \
	UNPACK_SRC(d3, xx);                                         \
	sum0 = _mm_mulhi_pi16(d0, ker[0][0]);                       \
	sum1 = _mm_mulhi_pi16(d0, ker[1][0]);                       \
	sum2 = _mm_mulhi_pi16(d0, ker[2][0]);                       \
	sum3 = _mm_mulhi_pi16(d0, ker[3][0]);                       \
	sum4 = _mm_mulhi_pi16(d0, ker[4][0]);                       \
	aa = ALGN(d0, d1, 48);                                      \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(aa, ker[0][1]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(aa, ker[1][1]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(aa, ker[2][1]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(aa, ker[3][1]));   \
	sum4 = _mm_add_pi16(sum4, _mm_mulhi_pi16(aa, ker[4][1]));   \
	bb = ALGN(d1, d2, 32);                                      \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(bb, ker[0][2]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(bb, ker[1][2]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(bb, ker[2][2]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(bb, ker[3][2]));   \
	sum4 = _mm_add_pi16(sum4, _mm_mulhi_pi16(bb, ker[4][2]));   \
	cc = ALGN(d2, d3, 16);                                      \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(cc, ker[0][3]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(cc, ker[1][3]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(cc, ker[2][3]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(cc, ker[3][3]));   \
	sum4 = _mm_add_pi16(sum4, _mm_mulhi_pi16(cc, ker[4][3]));   \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(d3, ker[0][4]));   \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(d3, ker[1][4]));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(d3, ker[2][4]));   \
	sum3 = _mm_add_pi16(sum3, _mm_mulhi_pi16(d3, ker[3][4]));   \
	sum4 = _mm_add_pi16(sum4, _mm_mulhi_pi16(d3, ker[4][4]));   \
	prev0 = buff0[i];                                           \
	prev1 = buff1[i];                                           \
	prev2 = buff2[i];                                           \
	prev3 = buff3[i];                                           \
	sum4 = _mm_add_pi16(sum4, prev0);                           \
	sum4 = _mm_add_pi16(sum4, prev1);                           \
	sum4 = _mm_add_pi16(sum4, prev2);                           \
	sum4 = _mm_add_pi16(sum4, prev3);                           \
	buff0[i] = sum0;                                            \
	buff1[i] = sum1;                                            \
	buff2[i] = sum2;                                            \
	buff3[i] = sum3;                                            \
	res_##xx = _mm_srai_pi16(sum4, shift)

/* *********************************************************** */
#define	UNPACK_SRC(dd, xx)                                      \
	dd = _mm_unpack##xx##_pi8(s0, zero);                    \
	dd = _mm_slli_pi16(dd, 7)

/* *********************************************************** */
#define	KSIZE	5
#define	NCHAN	3

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
mlib_m_conv5x5_8nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kern,
    mlib_s32 scalef_expon)
{
	__m64 *pbuff, *buff_arr[20], **pbuff_arr = buff_arr;
	__m64 *buff0, *buff1, *buff2, *buff3;
	GET_SRC_DST_PARAMETERS(mlib_u8);
	__m64 ker[5][5];
	__m64 s0, d0, d1, d2, d3, prev0, prev1, prev2, prev3;
	__m64 sum0, sum1, sum2, sum3, sum4, aa, bb, cc, res_hi, res_lo;
	__m64 zero = _m_zero;
	mlib_s32 shift, ind;
	mlib_s32 *sp;
	mlib_s32 row, wid4, i, j;

	width -= (KSIZE - 1);
	height -= (KSIZE - 1);
	width *= NCHAN;
	dl += ((KSIZE - 1) / 2) * (dll + NCHAN);

	wid4 = (width + 7) / 4;
	pbuff = mlib_malloc(sizeof (__m64) * 10 * wid4);

	GET_KERN();

	for (i = 0; i < 10; i++) {
		buff_arr[i] = pbuff + i * wid4;
	}

	ind = 0;
	for (j = 1; j <= 4; j++) {
		buff0 = buff_arr[ind];
		buff1 = buff_arr[ind + 1];
		buff2 = buff_arr[ind + 2];
		buff3 = buff_arr[ind + 3];

		sp = (mlib_s32 *)sl;

		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d1, lo);
		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d2, lo);
		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d3, lo);

		for (i = 0; i < wid4; i++) {
			*(mlib_s32 *)&s0 = sp[i];

			PREP_5x5();
		}

		sl += sll;
		ind += j;
	}

	for (row = 0; row < height; row++) {
		__m64 *sp = (__m64 *) sl;
		__m64 *dp = (__m64 *) dl;

		buff0 = pbuff_arr[0];
		buff1 = pbuff_arr[2];
		buff2 = pbuff_arr[5];
		buff3 = pbuff_arr[9];

		s0 = (*sp++);
		UNPACK_SRC(d1, lo);
		UNPACK_SRC(d2, hi);
		s0 = (*sp++);
		UNPACK_SRC(d3, lo);

		for (i = 0; i < width / 8; i++) {
			CONV_5x5(hi, 2 * i);
			s0 = sp[i];
			CONV_5x5(lo, 2 * i + 1);

			dp[i] = _mm_packs_pu16(res_hi, res_lo);
		}

		if (width & 7) {
			__m64 mask;

			mask = ((__m64 *) mlib_mask64_arr)[width & 7];

			CONV_5x5(hi, 2 * i);
			s0 = sp[i];
			CONV_5x5(lo, 2 * i + 1);
			res_hi = _mm_packs_pu16(res_hi, res_lo);

			dp[i] =
			    _mm_or_si64(_mm_and_si64(mask, res_hi),
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
