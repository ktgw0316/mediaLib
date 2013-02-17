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

#pragma ident	"@(#)mlib_m_ImageSConv7x7_8nw_3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_sconv7x7_8nw.c - sconvolve an 8-bit image with a 7x7 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status
 *  mlib_m_sconv7x7_8nw( mlib_image         *src,
 *                    mlib_image            *dst,
 *                    mlib_s32              *kernel,
 *                    mlib_s32 scalef_expon
 *                    mlib_s32 cmask )
 * ARGUMENT
 *      src           Pointer to structure of source image
 *      dst           Pointer to structure of destination image
 *      kernel        Sconvolution kernel
 *      scalef_expons The scaling factor to sconvert the input integer
 *                    coefficients into floating-point coefficients:
 *                    floating-point coefficient =
 *                    integer coefficient * 2^(-scale)
 *      cmask         Channel mask to indicate the channels to be sconvolved.
 *                    Each bit of which represents a channel in the image. The
 *                    channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      A 2-D sconvolution (7x7 kernel) for 8-bit images.
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
#define	LOAD_KERN(dker, ker)                                    \
	* (mlib_s16 *)&dker = (ker >> 16);                      \
	dker = _mm_unpacklo_pi16(dker, dker);                   \
	dker = _mm_unpacklo_pi16(dker, dker)

/* *********************************************************** */
#define	GET_KERN()                                              \
	LOAD_KERN(hker0, hkernel[0]);                           \
	LOAD_KERN(hker1, hkernel[1]);                           \
	LOAD_KERN(hker2, hkernel[2]);                           \
	LOAD_KERN(hker3, hkernel[3]);                           \
	LOAD_KERN(hker4, hkernel[4]);                           \
	LOAD_KERN(hker5, hkernel[5]);                           \
	LOAD_KERN(hker6, hkernel[6]);                           \
	LOAD_KERN(vker0, vkernel[0]);                           \
	LOAD_KERN(vker1, vkernel[1]);                           \
	LOAD_KERN(vker2, vkernel[2]);                           \
	LOAD_KERN(vker3, vkernel[3]);                           \
	LOAD_KERN(vker4, vkernel[4]);                           \
	LOAD_KERN(vker5, vkernel[5]);                           \
	LOAD_KERN(vker6, vkernel[6]);                           \
	shift = 2 * scalef_expon - (32 + 32 - 7)

/* *********************************************************** */
#define	PREP_7x7(xx, i)                                         \
	d0 = d1;                                                \
	d1 = d2;                                                \
	d2 = d3;                                                \
	d3 = d4;                                                \
	d4 = d5;                                                \
	UNPACK_SRC(d5, xx);                                     \
	sum6 = _mm_mulhi_pi16(d0, hker0);                       \
	aa = ALGN(d0, d1, 48);                                  \
	sum6 = _mm_add_pi16(sum6, _mm_mulhi_pi16(aa, hker1));   \
	bb = ALGN(d1, d2, 32);                                  \
	sum6 = _mm_add_pi16(sum6, _mm_mulhi_pi16(bb, hker2));   \
	cc = ALGN(d2, d3, 16);                                  \
	sum6 = _mm_add_pi16(sum6, _mm_mulhi_pi16(cc, hker3));   \
	sum6 = _mm_add_pi16(sum6, _mm_mulhi_pi16(d3, hker4));   \
	aa = ALGN(d3, d4, 48);                                  \
	sum6 = _mm_add_pi16(sum6, _mm_mulhi_pi16(aa, hker5));   \
	bb = ALGN(d4, d5, 32);                                  \
	sum6 = _mm_add_pi16(sum6, _mm_mulhi_pi16(bb, hker6));   \
	buff6[i] = sum6

/* *********************************************************** */
#define	CONV_7x7(xx, i)                                         \
	sum0 = buff0[i];                                        \
	sum1 = buff1[i];                                        \
	sum2 = buff2[i];                                        \
	sum3 = buff3[i];                                        \
	sum4 = buff4[i];                                        \
	sum5 = buff5[i];                                        \
	PREP_7x7(xx, i);                                        \
	res_##xx = _mm_mulhi_pi16(sum0, vker0);                 \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum1, vker1));    \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum2, vker2));    \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum3, vker3));    \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum4, vker4));    \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum5, vker5));    \
	res_##xx =                                              \
	_mm_add_pi16(res_##xx, _mm_mulhi_pi16(sum6, vker6));    \
	res_##xx = _mm_srai_pi16(res_##xx, shift)

/* *********************************************************** */
#define	UNPACK_SRC(dd, xx)                                      \
	dd = _mm_unpack##xx##_pi8(s0, zero);                    \
	dd = _mm_slli_pi16(dd, 7)

/* *********************************************************** */
#define	KSIZE	7
#define	KSIZE1	(KSIZE - 1)

#define	NCHAN	3

mlib_status
mlib_m_sconv7x7_8nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
	__m64 *pbuff, *buff_arr[KSIZE];
	__m64 *buff0, *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buffT;
	GET_SRC_DST_PARAMETERS(mlib_u8);
	__m64 hker0, hker1, hker2, hker3, hker4, hker5, hker6;
	__m64 vker0, vker1, vker2, vker3, vker4, vker5, vker6;
	__m64 s0, d0, d1, d2, d3, d4, d5, aa, bb, cc, res_hi, res_lo;
	__m64 sum0, sum1, sum2, sum3, sum4, sum5, sum6;
	__m64 zero = _m_zero;
	mlib_s32 shift, ind;
	mlib_s32 *sp;
	mlib_s32 row, wid4, i, j;

	width -= KSIZE1;
	height -= KSIZE1;
	width *= NCHAN;
	dl += (KSIZE / 2) * (dll + NCHAN);

	wid4 = 2 * ((width + 7) / 8);
	pbuff = mlib_malloc(sizeof (__m64) * 7 * wid4);

	GET_KERN();

	for (i = 0; i < KSIZE; i++) {
		buff_arr[i] = pbuff + i * wid4;
	}

	for (j = 0; j < KSIZE1; j++) {
		buff6 = buff_arr[j];

		sp = (mlib_s32 *)sl;

		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d1, lo);
		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d2, lo);
		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d3, lo);
		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d4, lo);
		*(mlib_s32 *)&s0 = (*sp++);
		UNPACK_SRC(d5, lo);

		for (i = 0; i < wid4; i++) {
			*(mlib_s32 *)&s0 = sp[i];

			PREP_7x7(lo, i);
		}

		sl += sll;
		ind++;
	}

	buff0 = buff_arr[0];
	buff1 = buff_arr[1];
	buff2 = buff_arr[2];
	buff3 = buff_arr[3];
	buff4 = buff_arr[4];
	buff5 = buff_arr[5];
	buff6 = buff_arr[6];

	for (row = 0; row < height; row++) {
		__m64 *sp = (__m64 *) sl;
		__m64 *dp = (__m64 *) dl;

		s0 = (*sp++);
		UNPACK_SRC(d1, lo);
		UNPACK_SRC(d2, hi);
		s0 = (*sp++);
		UNPACK_SRC(d3, lo);
		UNPACK_SRC(d4, hi);
		s0 = (*sp++);
		UNPACK_SRC(d5, lo);

		for (i = 0; i < width / 8; i++) {
			CONV_7x7(hi, 2 * i);
			s0 = sp[i];
			CONV_7x7(lo, 2 * i + 1);

			dp[i] = _mm_packs_pu16(res_hi, res_lo);
		}

		if (width & 7) {
			__m64 mask = ((__m64 *) mlib_mask64_arr)[width & 7];

			CONV_7x7(hi, 2 * i);
			s0 = sp[i];
			CONV_7x7(lo, 2 * i + 1);
			res_hi = _mm_packs_pu16(res_hi, res_lo);

			dp[i] =
			    _mm_or_si64(_mm_and_si64(mask, res_hi),
			    _mm_andnot_si64(mask, dp[i]));
		}

		buffT = buff0;
		buff0 = buff1;
		buff1 = buff2;
		buff2 = buff3;
		buff3 = buff4;
		buff4 = buff5;
		buff5 = buff6;
		buff6 = buffT;

		sl += sll;
		dl += dll;
	}

	_mm_empty();
	mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
