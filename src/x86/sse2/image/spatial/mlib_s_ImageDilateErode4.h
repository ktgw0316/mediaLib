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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_MLIB_S_IMAGEDILATEERODE4_H
#define	_MLIB_S_IMAGEDILATEERODE4_H

#pragma ident	"@(#)mlib_s_ImageDilateErode4.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

static const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(align(16))
#elif __SUNPRO_C
#pragma align 16(mlib_mask128i_arr)
#endif /* defined(__ICL) || defined (__INTEL_COMPILER) */
unsigned int mlib_mask128i_arr[] = {
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x000000FF, 0x00000000, 0x00000000, 0x00000000,
	0x0000FFFF, 0x00000000, 0x00000000, 0x00000000,
	0x00FFFFFF, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x000000FF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x0000FFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x00FFFFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

/* *********************************************************** */

#if DILATE_FILTER

#define	C_COMP_T(a, b, type, maska, maskb)                          \
	_mm_add_epi##type(a, _mm_and_si128(_mm_sub_epi##type(b, a), \
	    _mm_cmpgt_epi##type(maskb, maska)))
#define	F_NAMET(TYPE)	mlib_ImageDilate4_##TYPE
#else

#define	C_COMP_T(a, b, type, maska, maskb)                          \
	_mm_add_epi##type(a, _mm_and_si128(_mm_sub_epi##type(b, a), \
	    _mm_cmpgt_epi##type(maska, maskb)))
#define	F_NAMET(TYPE)	mlib_ImageErode4_##TYPE
#endif /* DILATE_FILTER */

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	F_NAME	F_NAMET(U8)
#define	STYPE	mlib_u8
#define	MASK  0x80808080
#define	C_COMP(a, b)                                                \
	C_COMP_T(a, b, 8, _mm_xor_si128(a, mask80),                 \
		_mm_xor_si128(b, mask80))                           \

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 ( MLIB_SHORT ) */

#define	F_NAME	F_NAMET(S16)
#define	STYPE	mlib_s16
#define	MASK  0x0
#define	C_COMP(a, b)  C_COMP_T(a, b, 16, a, b)

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 ( MLIB_USHORT ) */

#define	F_NAME	F_NAMET(U16)
#define	STYPE	mlib_u16
#define	MASK  0x80008000
#define	C_COMP(a, b)                                                \
	C_COMP_T(a, b, 16, _mm_xor_si128(a, mask80),                \
		_mm_xor_si128(b, mask80))                           \

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 ( MLIB_INT ) */

#define	F_NAME	F_NAMET(S32)
#define	STYPE	mlib_s32
#define	MASK  0x0
#define	C_COMP(a, b)  C_COMP_T(a, b, 32, a, b)

#endif		   /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */
#define	SSIZE	sizeof (STYPE)
/* *********************************************************** */

mlib_status
F_NAME(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
{
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *dl;
	__m128i *dp0, *dp1;
	__m128i a1, b0, b1, b2, c0, c1, c2, d1, vv, h0, h1, r0, r1, t0, t1;
	__m128i mask, mask80;
	mlib_s32 i, j, tail;

	sl = (mlib_u8 *)src;
/* dst ptrs skip top j and left col */
	dl = (mlib_u8 *)dst + dlb + SSIZE;

	wid = (wid - 2) * SSIZE;
	tail = wid & 15;
	mask = _mm_loadu_si128(((__m128i *) mlib_mask128i_arr) + tail);
	mask80 = _mm_set1_epi32(MASK);

	for (j = 0; j <= (hgt - 2 - 2); j += 2) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = sl;
		sp1 = sp0 + slb;
		sp2 = sp1 + slb;
		sp3 = sp2 + slb;

/*
 *    line0:     a1
 *    line1:  b0 b1 b2
 *    line2:  c0 c1 c2
 *    line3:     d1
 */

		for (i = 0; i <= wid - 16; i += 16) {
			a1 = _mm_loadu_si128((__m128i *)(sp0 + SSIZE));
			b0 = _mm_loadu_si128((__m128i *)(sp1));
			b1 = _mm_loadu_si128((__m128i *)(sp1 + SSIZE));
			b2 = _mm_loadu_si128((__m128i *)(sp1 + 2 * SSIZE));
			c0 = _mm_loadu_si128((__m128i *)(sp2));
			c1 = _mm_loadu_si128((__m128i *)(sp2 + SSIZE));
			c2 = _mm_loadu_si128((__m128i *)(sp2 + 2 * SSIZE));
			d1 = _mm_loadu_si128((__m128i *)(sp3 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			h1 = C_COMP(c0, c2);

			r0 = C_COMP(vv, a1);
			r1 = C_COMP(vv, d1);
			r0 = C_COMP(r0, h0);
			r1 = C_COMP(r1, h1);

			_mm_storeu_si128(dp0, r0);
			dp0++;
			_mm_storeu_si128(dp1, r1);
			dp1++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
		}

		if (tail) {
			a1 = _mm_loadu_si128((__m128i *)(sp0 + SSIZE));
			b0 = _mm_loadu_si128((__m128i *)(sp1));
			b1 = _mm_loadu_si128((__m128i *)(sp1 + SSIZE));
			b2 = _mm_loadu_si128((__m128i *)(sp1 + 2 * SSIZE));
			c0 = _mm_loadu_si128((__m128i *)(sp2));
			c1 = _mm_loadu_si128((__m128i *)(sp2 + SSIZE));
			c2 = _mm_loadu_si128((__m128i *)(sp2 + 2 * SSIZE));
			d1 = _mm_loadu_si128((__m128i *)(sp3 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			h1 = C_COMP(c0, c2);

			r0 = C_COMP(vv, a1);
			r1 = C_COMP(vv, d1);
			r0 = C_COMP(r0, h0);
			r1 = C_COMP(r1, h1);

			t0 = _mm_loadu_si128(dp0);
			t1 = _mm_loadu_si128(dp1);
			t0 =
			    _mm_or_si128(_mm_and_si128(mask, r0),
			    _mm_andnot_si128(mask, t0));
			t1 =
			    _mm_or_si128(_mm_and_si128(mask, r1),
			    _mm_andnot_si128(mask, t1));

			_mm_storeu_si128(dp0, t0);
			_mm_storeu_si128(dp1, t1);
		}

		sl += 2 * slb;
		dl += 2 * dlb;
	}

/* last line */

	if (j == (hgt - 3)) {
		dp0 = (void *)dl;
		sp0 = sl;
		sp1 = sp0 + slb;
		sp2 = sp1 + slb;

		for (i = 0; i <= wid - 16; i += 16) {
			a1 = _mm_loadu_si128((__m128i *)(sp0 + SSIZE));
			b0 = _mm_loadu_si128((__m128i *)(sp1));
			b1 = _mm_loadu_si128((__m128i *)(sp1 + SSIZE));
			b2 = _mm_loadu_si128((__m128i *)(sp1 + 2 * SSIZE));
			c1 = _mm_loadu_si128((__m128i *)(sp2 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			r0 = C_COMP(vv, a1);
			r0 = C_COMP(r0, h0);

			_mm_storeu_si128(dp0, r0);
			dp0++;
			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
		}

		if (tail) {
			a1 = _mm_loadu_si128((__m128i *)(sp0 + SSIZE));
			b0 = _mm_loadu_si128((__m128i *)(sp1));
			b1 = _mm_loadu_si128((__m128i *)(sp1 + SSIZE));
			b2 = _mm_loadu_si128((__m128i *)(sp1 + 2 * SSIZE));
			c1 = _mm_loadu_si128((__m128i *)(sp2 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			r0 = C_COMP(vv, a1);
			r0 = C_COMP(r0, h0);

			t0 = _mm_loadu_si128(dp0);
			t0 =
			    _mm_or_si128(_mm_and_si128(mask, r0),
			    _mm_andnot_si128(mask, t0));
			_mm_storeu_si128(dp0, t0);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEDILATEERODE4_H */
