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

#pragma ident	"@(#)mlib_m_ImageSConv7x7_u16nw_3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_conv7x7_16nw.c - convolve an 8-bit image with a 7x7 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status
 *  mlib_m_conv7x7_16nw( mlib_image         *src,
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
 *      A 2-D convolution (7x7 kernel) for 8-bit images.
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
	shift = 2 * scalef_expon - (32 + 32 - 16);              \
	for (i = 0; i < 7; i++) {                               \
	    kerh_sum += (hkernel[i] >> 16);                     \
	    kerv_sum += (vkernel[i] >> 16);                     \
	}                                                       \
	                                                        \
	ker_sum =                                               \
	    ((mlib_d64)kerh_sum*kerv_sum +                      \
	    7*kerh_sum + 7*(1 << 16)) / (1 << (shift + 1));     \
	                                                        \
	ker_off = _m_from_int(ker_sum - 0x8000);                \
	ker_off = _mm_unpacklo_pi32(ker_off, ker_off);          \
	mask8000 = _m_from_int(0x80008000);                     \
	mask8000 = _mm_unpacklo_pi32(mask8000, mask8000);       \
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
	LOAD_KERN(vker6, vkernel[6])

/* *********************************************************** */
#define	CONV_V(vv)                                              \
	vv = _mm_mulhi_pi16(s0, vker0);                         \
	vv = _mm_add_pi16(vv, _mm_mulhi_pi16(s1, vker1));       \
	vv = _mm_add_pi16(vv, _mm_mulhi_pi16(s2, vker2));       \
	vv = _mm_add_pi16(vv, _mm_mulhi_pi16(s3, vker3));       \
	vv = _mm_add_pi16(vv, _mm_mulhi_pi16(s4, vker4));       \
	vv = _mm_add_pi16(vv, _mm_mulhi_pi16(s5, vker5));       \
	vv = _mm_add_pi16(vv, _mm_mulhi_pi16(s6, vker6));       \
	vv = _mm_adds_pi16(vv, _rnd)

/* *********************************************************** */
#define	PREP_V(res)                                             \
	s0 = (*sp0++);                                          \
	s1 = (*sp1++);                                          \
	s2 = (*sp2++);                                          \
	s3 = (*sp3++);                                          \
	s4 = (*sp4++);                                          \
	s5 = (*sp5++);                                          \
	s6 = (*sp6++);                                          \
	s0 = _mm_xor_si64(s0, mask8000);                        \
	s1 = _mm_xor_si64(s1, mask8000);                        \
	s2 = _mm_xor_si64(s2, mask8000);                        \
	s3 = _mm_xor_si64(s3, mask8000);                        \
	s4 = _mm_xor_si64(s4, mask8000);                        \
	s5 = _mm_xor_si64(s5, mask8000);                        \
	s6 = _mm_xor_si64(s6, mask8000);                        \
	CONV_V(res)

/* *********************************************************** */
#define	CONV_7x7()                                              \
	s0 = sp0[i];                                            \
	s1 = sp1[i];                                            \
	s2 = sp2[i];                                            \
	s3 = sp3[i];                                            \
	s4 = sp4[i];                                            \
	s5 = sp5[i];                                            \
	s6 = sp6[i];                                            \
	s0 = _mm_xor_si64(s0, mask8000);                        \
	s1 = _mm_xor_si64(s1, mask8000);                        \
	s2 = _mm_xor_si64(s2, mask8000);                        \
	s3 = _mm_xor_si64(s3, mask8000);                        \
	s4 = _mm_xor_si64(s4, mask8000);                        \
	s5 = _mm_xor_si64(s5, mask8000);                        \
	s6 = _mm_xor_si64(s6, mask8000);                        \
	v0 = v1;                                                \
	v1 = v2;                                                \
	v2 = v3;                                                \
	v3 = v4;                                                \
	v4 = v5;                                                \
	CONV_V(v5);                                             \
	rr = _mm_mulhi_pi16(v0, hker0);                         \
	aa = ALGN(v0, v1, 48);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(aa, hker1));       \
	bb = ALGN(v1, v2, 32);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(bb, hker2));       \
	cc = ALGN(v2, v3, 16);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(cc, hker3));       \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(v3, hker4));       \
	aa = ALGN(v3, v4, 48);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(aa, hker5));       \
	bb = ALGN(v4, v5, 32);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(bb, hker6));       \
	rr = _mm_adds_pi16(rr, _rnd);                           \
	rl = _mm_unpacklo_pi16(zero, rr);                       \
	rh = _mm_unpackhi_pi16(zero, rr);                       \
	rl = _mm_srai_pi32(rl, shift);                          \
	rh = _mm_srai_pi32(rh, shift);                          \
	rl = _mm_add_pi32(rl, ker_off);                         \
	rh = _mm_add_pi32(rh, ker_off);                         \
	rr = _mm_packs_pi32(rl, rh);                            \
	rr = _mm_xor_si64(rr, mask8000)

/* *********************************************************** */
#define	KSIZE	7
#define	KSIZE1	(KSIZE - 1)

#define	NCHAN	3

mlib_status
mlib_m_sconv7x7_u16nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
	GET_SRC_DST_PARAMETERS(mlib_s16);
	__m64 hker0, hker1, hker2, hker3, hker4, hker5, hker6;
	__m64 vker0, vker1, vker2, vker3, vker4, vker5, vker6;
	__m64 s0, s1, s2, s3, s4, s5, s6, v0, v1, v2, v3, v4, v5, rr, rh, rl;
	__m64 aa, bb, cc, zero, _rnd, ker_off, mask8000;
	__m64 *sp0, *sp1, *sp2, *sp3, *sp4, *sp5, *sp6, *dp;
	mlib_s32 shift, ker_sum, kerh_sum = 0, kerv_sum = 0;
	mlib_s32 row, wid4, i, j;

	width -= KSIZE1;
	height -= KSIZE1;
	width *= NCHAN;
	dl += (KSIZE / 2) * (dll + NCHAN);

	GET_KERN();

	zero = _mm_setzero_si64();

	for (j = 0; j < height; j++) {
		sp0 = (__m64 *) sl;
		sp1 = (__m64 *) (sl + sll);
		sp2 = (__m64 *) (sl + 2 * sll);
		sp3 = (__m64 *) (sl + 3 * sll);
		sp4 = (__m64 *) (sl + 4 * sll);
		sp5 = (__m64 *) (sl + 5 * sll);
		sp6 = (__m64 *) (sl + 6 * sll);
		dp = (__m64 *) dl;

		PREP_V(v1);
		PREP_V(v2);
		PREP_V(v3);
		PREP_V(v4);
		PREP_V(v5);

		for (i = 0; i < width / 4; i++) {
			CONV_7x7();

			dp[i] = rr;
		}

		if (width & 3) {
			__m64 mask =
			    ((__m64 *) mlib_mask64_arr)[2 * (width & 3)];

			CONV_7x7();

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
