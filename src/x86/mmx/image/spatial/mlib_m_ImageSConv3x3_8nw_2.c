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

#pragma ident	"@(#)mlib_m_ImageSConv3x3_8nw_2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_conv3x3_8nw.c - convolve an 8-bit image with a 3x3 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status
 *  mlib_m_conv3x3_8nw( mlib_image          *src,
 *                    mlib_image            *dst,
 *                    mlib_s32              *kernel,
 *                    mlib_s32 scalef_expon
 *                    mlib_s32 cmask )
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
	* (mlib_s16 *)&hker0 = (hkernel[0] >> 16) & 0xFFFF;     \
	*(mlib_s16 *)&hker1 = (hkernel[1] >> 16) & 0xFFFF;      \
	*(mlib_s16 *)&hker2 = (hkernel[2] >> 16) & 0xFFFF;      \
	*(mlib_s16 *)&vker0 = (vkernel[0] >> 16) & 0xFFFF;      \
	*(mlib_s16 *)&vker1 = (vkernel[1] >> 16) & 0xFFFF;      \
	*(mlib_s16 *)&vker2 = (vkernel[2] >> 16) & 0xFFFF;      \
	hker0 = _mm_unpacklo_pi16(hker0, hker0);                \
	hker1 = _mm_unpacklo_pi16(hker1, hker1);                \
	hker2 = _mm_unpacklo_pi16(hker2, hker2);                \
	vker0 = _mm_unpacklo_pi16(vker0, vker0);                \
	vker1 = _mm_unpacklo_pi16(vker1, vker1);                \
	vker2 = _mm_unpacklo_pi16(vker2, vker2);                \
	hker0 = _mm_unpacklo_pi16(hker0, hker0);                \
	hker1 = _mm_unpacklo_pi16(hker1, hker1);                \
	hker2 = _mm_unpacklo_pi16(hker2, hker2);                \
	vker0 = _mm_unpacklo_pi16(vker0, vker0);                \
	vker1 = _mm_unpacklo_pi16(vker1, vker1);                \
	vker2 = _mm_unpacklo_pi16(vker2, vker2);                \
	shift = 2 * scalef_expon - (32 + 32 - 7)

/* *********************************************************** */
#define	PREP_3x3(xx, i)                                         \
	d0 = d1;                                                \
	UNPACK_SRC(d1, xx);                                     \
	aa = ALGN(d0, d1, 32);                                  \
	sum2 = _mm_mulhi_pi16(aa, hker1);                       \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(d1, hker2));   \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(d0, hker0));   \
	buff0[i] = sum2

/* *********************************************************** */
#define	CONV_3x3(xx, i)                                         \
	sum0 = buff0[i];                                        \
	sum1 = buff1[i];                                        \
	PREP_3x3(xx, i);                                        \
	res_##xx = _mm_mulhi_pi16(sum0, vker0);                 \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum1, vker1));    \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum2, vker2));    \
	res_##xx = _mm_srai_pi16(res_##xx, shift)

/* *********************************************************** */
#define	UNPACK_SRC(dd, xx)                                      \
	dd = _mm_unpack##xx##_pi8(s0, zero);                    \
	dd = _mm_slli_pi16(dd, 7)

/* *********************************************************** */
#define	NCHAN	2

mlib_status
mlib_m_sconv3x3_8nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
	__m64 buff_loc[3 * BUFF_LINE], *pbuff = buff_loc;
	__m64 *buff0, *buff1, *buffT;
	GET_SRC_DST_PARAMETERS(mlib_u8);
	__m64 hker0, hker1, hker2, vker0, vker1, vker2;
	__m64 s0, d0, d1, sum0, sum1, sum2, aa, res_hi, res_lo;
	__m64 zero = _m_zero;
	mlib_s32 shift;
	mlib_s32 *sp;
	mlib_s32 row, wid4, i, j;

	width -= 2;
	height -= 2;
	width *= NCHAN;
	dl += dll + NCHAN;

	wid4 = (width + 7) / 4;

	if (wid4 > BUFF_LINE) {
		pbuff = mlib_malloc(sizeof (__m64) * 3 * wid4);
	}

	GET_KERN();

	buff0 = pbuff;
	buff1 = buff0 + wid4;

	for (j = 0; j < 2; j++) {
		sp = (mlib_s32 *)sl;

		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d1, lo);

		for (i = 0; i < wid4; i++) {
			*(mlib_s32 *)&s0 = sp[i];

			PREP_3x3(lo, i);
		}

		sl += sll;

		buffT = buff1;
		buff1 = buff0;
		buff0 = buffT;
	}

	for (row = 0; row < height; row++) {
		__m64 *sp = (__m64 *) sl;
		__m64 *dp = (__m64 *) dl;

		s0 = (*sp++);
		UNPACK_SRC(d1, lo);

		for (i = 0; i < width / 8; i++) {
			CONV_3x3(hi, 2 * i);
			s0 = sp[i];
			CONV_3x3(lo, 2 * i + 1);

			dp[i] = _mm_packs_pu16(res_hi, res_lo);
		}

		if (width & 7) {
			__m64 mask = ((__m64 *) mlib_mask64_arr)[width & 7];

			CONV_3x3(hi, 2 * i);
			s0 = sp[i];
			CONV_3x3(lo, 2 * i + 1);
			res_hi = _mm_packs_pu16(res_hi, res_lo);

			dp[i] =
			    _mm_or_si64(_mm_and_si64(mask, res_hi),
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
