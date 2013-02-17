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

#pragma ident	"@(#)mlib_m_ImageSConv3x3_u16nw_1.c	9.2	07/11/05 SMI"

/*
 * A 2-D separable convolution (3x3 kernel) for U16 images.
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
	shift = 2 * scalef_expon - (32 + 32 - 16);              \
	for (i = 0; i < 3; i++) {                               \
	    kerh_sum += (hkernel[i] >> 16);                     \
	    kerv_sum += (vkernel[i] >> 16);                     \
	}                                                       \
	                                                        \
	ker_sum =                                               \
	    ((mlib_d64)kerh_sum*kerv_sum +                      \
	    3*kerh_sum + 3*(1 << 16)) / (1 << (shift + 1));     \
	                                                        \
	ker_off = _m_from_int(ker_sum - 0x8000);                \
	ker_off = _mm_unpacklo_pi32(ker_off, ker_off);          \
	mask8000 = _m_from_int(0x80008000);                     \
	mask8000 = _mm_unpacklo_pi32(mask8000, mask8000);       \
	*(mlib_s16 *)&hker0 = (hkernel[0] >> 16);               \
	*(mlib_s16 *)&hker1 = (hkernel[1] >> 16);               \
	*(mlib_s16 *)&hker2 = (hkernel[2] >> 16);               \
	*(mlib_s16 *)&vker0 = (vkernel[0] >> 16);               \
	*(mlib_s16 *)&vker1 = (vkernel[1] >> 16);               \
	*(mlib_s16 *)&vker2 = (vkernel[2] >> 16);               \
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
	vker2 = _mm_unpacklo_pi16(vker2, vker2)

/* *********************************************************** */
#define	PREP_V()                                                \
	s0 = (*sp0++);                                          \
	s1 = (*sp1++);                                          \
	s2 = (*sp2++);                                          \
	s0 = _mm_xor_si64(s0, mask8000);                        \
	s1 = _mm_xor_si64(s1, mask8000);                        \
	s2 = _mm_xor_si64(s2, mask8000);                        \
	v1 = _mm_mulhi_pi16(s0, vker0);                         \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s1, vker1));       \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s2, vker2))

/* *********************************************************** */
#define	CONV_3x3()                                              \
	s0 = sp0[i];                                            \
	s1 = sp1[i];                                            \
	s2 = sp2[i];                                            \
	s0 = _mm_xor_si64(s0, mask8000);                        \
	s1 = _mm_xor_si64(s1, mask8000);                        \
	s2 = _mm_xor_si64(s2, mask8000);                        \
	v0 = v1;                                                \
	v1 = _mm_mulhi_pi16(s0, vker0);                         \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s1, vker1));       \
	v1 = _mm_add_pi16(v1, _mm_mulhi_pi16(s2, vker2));       \
	rr = _mm_mulhi_pi16(v0, hker0);                         \
	aa = ALGN(v0, v1, 16);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(aa, hker1));       \
	bb = ALGN(v0, v1, 32);                                  \
	rr = _mm_add_pi16(rr, _mm_mulhi_pi16(bb, hker2));       \
	rl = _mm_unpacklo_pi16(zero, rr);                       \
	rh = _mm_unpackhi_pi16(zero, rr);                       \
	rl = _mm_srai_pi32(rl, shift);                          \
	rh = _mm_srai_pi32(rh, shift);                          \
	rl = _mm_add_pi32(rl, ker_off);                         \
	rh = _mm_add_pi32(rh, ker_off);                         \
	rr = _mm_packs_pi32(rl, rh);                            \
	rr = _mm_xor_si64(rr, mask8000)

/* *********************************************************** */
#define	NCHAN	1

mlib_status
mlib_m_sconv3x3_u16nw_1(
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
	__m64 zero, ker_off, mask8000;
	mlib_s32 shift, ker_sum, kerh_sum = 0, kerv_sum = 0;
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
