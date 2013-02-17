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

#pragma ident	"@(#)mlib_m_ImageSConv3x3_16nw_2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_conv3x3_16nw.c - convolve an 8-bit image with a 3x3 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status
 *  mlib_m_conv3x3_16nw( mlib_image         *src,
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
#define	KER	3

/* *********************************************************** */
#define	GET_KERN()                                                      \
	hker0 = _mm_set1_pi16(hkernel[0] >> 16);                        \
	hker1 = _mm_set1_pi16(hkernel[1] >> 16);                        \
	hker2 = _mm_set1_pi16(hkernel[2] >> 16);                        \
	vker0 = _mm_set1_pi16(vkernel[0] >> 16);                        \
	vker1 = _mm_set1_pi16(vkernel[1] >> 16);                        \
	vker2 = _mm_set1_pi16(vkernel[2] >> 16);                        \
	shift = 2 * scalef_expon - (32 + 32 - 16);                      \
	kerh_sum = 0;                                                   \
	for (i = 0; i < KER; i++) {                                     \
	    kerh_sum += (hkernel[i] >> 16);                             \
	}                                                               \
	kerh_sum = ((KER*kerh_sum + KER*(1 << 16)) >> (shift + 1));     \
	_rnd = _mm_set1_pi32(kerh_sum)

/* *********************************************************** */
#define	PREP_V()                                                \
	s0 = (*sp0++);                                          \
	s1 = (*sp1++);                                          \
	s2 = (*sp2++);                                          \
	v1 = _mm_mulhi_pi16(s0, vker0);                         \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s1, vker1));       \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s2, vker2))

/* *********************************************************** */
#define	CONV_3x3()                                              \
	s0 = sp0[i];                                            \
	s1 = sp1[i];                                            \
	s2 = sp2[i];                                            \
	v0 = v1;                                                \
	v1 = _mm_mulhi_pi16(s0, vker0);                         \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s1, vker1));       \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s2, vker2));       \
	rr = _mm_mulhi_pi16(v0, hker0);                         \
	aa = ALGN(v0, v1, 32);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(aa, hker1));       \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(v1, hker2));       \
	rl = _mm_unpacklo_pi16(zero, rr);                       \
	rh = _mm_unpackhi_pi16(zero, rr);                       \
	rl = _mm_srai_pi32(rl, shift);                          \
	rh = _mm_srai_pi32(rh, shift);                          \
	rl = _mm_add_pi32(rl, _rnd);                            \
	rh = _mm_add_pi32(rh, _rnd);                            \
	rr = _mm_packs_pi32(rl, rh)

/* *********************************************************** */
#define	NCHAN	2

mlib_status
mlib_m_sconv3x3_16nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
	GET_SRC_DST_PARAMETERS(mlib_s16);
	__m64 hker0, hker1, hker2, vker0, vker1, vker2;
	__m64 s0, s1, s2, v0, v1, aa, bb, rr, rh, rl;
	__m64 *sp0, *sp1, *sp2, *dp;
	__m64 zero, _rnd;
	mlib_s32 shift, kerh_sum;
	mlib_s32 i, j;

	width -= 2;
	height -= 2;
	width *= NCHAN;
	dl += dll + NCHAN;

	GET_KERN();

	zero = _mm_setzero_si64();

	for (j = 0; j < height; j++) {
		sp0 = (__m64 *) sl;
		sp1 = (__m64 *) (sl + sll);
		sp2 = (__m64 *) (sl + 2 * sll);
		dp = (__m64 *) dl;

		PREP_V();

		for (i = 0; i < width / 4; i++) {
			CONV_3x3();

			dp[i] = rr;
		}

		if (width & 3) {
			__m64 mask =
			    ((__m64 *) mlib_mask64_arr)[2 * (width & 3)];

			CONV_3x3();

			dp[i] =
			    _mm_or_si64(_mm_and_si64(mask, rr),
			    _mm_andnot_si64(mask, dp[i]));
		}

		sl += sll;
		dl += dll;
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
