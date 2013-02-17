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

#pragma ident   "@(#)mlib_s_ImageErode4_F32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDilate4_F32.c -
 *      OR:
 *      mlib_ImageErode4_F32.c -
 *              Does a 3x3 erode 4 or dilate 4 filter on the
 *              8-bit input image. Edge policy = no write.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDilate4_F32(void       *dst,
 *                                        const void *src,
 *                                        mlib_s32   dlb,
 *                                        mlib_s32   slb,
 *                                        mlib_s32   wid,
 *                                        mlib_s32   hgt)
 *      OR
 *      mlib_status mlib_ImageErode4_F32(void       *dst,
 *                                       const void *src,
 *                                       mlib_s32   dlb,
 *                                       mlib_s32   slb,
 *                                       mlib_s32   wid,
 *                                       mlib_s32   hgt)
 *
 * ARGUMENTS
 *      dst     Pointer to the first byte for first pixel of destination
 *      src     Pointer to the first byte for first pixel of source
 *      dlb     Bytes count of the destination row
 *      slb     Bytes count of the source row
 *      w       number of elements in a irow of source
 *      h       number of rows of source
 *
 * DESCRIPTION
 *        Does a 3x3 erode 4 or dilate 4 filter on the 8-bit input image.
 *      Edge = no write. Filtered values are returned in the destination
 *      image.
 *        The compiler switch DILATE_FILTER controls the macro C_COMP,
 *      #defining it as max() if true, or as min() if false. It also
 *      controls the function name. (Comments and #defined macro names
 *      usually refer to "max" instead of "min".)
 *        Uses the dilate4 neighborhood (not the dilate8):
 *              Dilate4 neighborhood:           Dilate8 neighborhood:
 *                        o                             o o o
 *                      o o o                           o o o
 *                        o                             o o o
 *
 * NOTE TO MAINTENANCE PROGRAMMERS !!!!!!!!!
 *
 *      If you change this erosion or dilation filter, just change one,
 *      then copy it over the other one, changing only the true/false (1/0)
 *      value of the #defined switch DILATE_FILTER. For library purposes,
 *      two files are probably necessary, but keep them in sync so that
 *      their only difference is the value of the DILATE_FILTER switch.
 *
 *
 */

#include <mlib_image.h>
#include <mlib_ImageErode4.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
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

#define	DILATE_FILTER	0

/* *********************************************************** */
#if DILATE_FILTER

#define	C_COMP(a, b)  _mm_max_ps(a, b)
#define	F_NAMET(TYPE)	mlib_ImageDilate4_##TYPE
#else

#define	C_COMP(a, b)  _mm_min_ps(a, b)
#define	F_NAMET(TYPE)	mlib_ImageErode4_##TYPE
#endif /* DILATE_FILTER */

/* *********************************************************** */

#define	F_NAME	F_NAMET(F32)
#define	STYPE	mlib_f32

/* *********************************************************** */
#define	SSIZE	sizeof (STYPE)
/* *********************************************************** */

mlib_status
F_NAME(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
{
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *dl;
	__m128 *dp0, *dp1;
	__m128 a1, b0, b1, b2, c0, c1, c2, d1, vv, h0, h1, r0, r1, t0, t1;
	__m128 mask;
	mlib_s32 i, j, tail;

	sl = (mlib_u8 *)src;
/* dst ptrs skip top j and left col */
	dl = (mlib_u8 *)dst + dlb + SSIZE;

	wid = (wid - 2) * SSIZE;
	tail = wid & 15;
	((mlib_d64 *)&mask)[0] =
		((mlib_d64 *)((__m128 *) mlib_mask128i_arr + tail))[0];
	((mlib_d64 *)&mask)[1] =
		((mlib_d64 *)((__m128 *) mlib_mask128i_arr + tail))[1];

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
			a1 = _mm_loadu_ps((mlib_f32 *)(sp0 + SSIZE));
			b0 = _mm_loadu_ps((mlib_f32 *)(sp1));
			b1 = _mm_loadu_ps((mlib_f32 *)(sp1 + SSIZE));
			b2 = _mm_loadu_ps((mlib_f32 *)(sp1 + 2 * SSIZE));
			c0 = _mm_loadu_ps((mlib_f32 *)(sp2));
			c1 = _mm_loadu_ps((mlib_f32 *)(sp2 + SSIZE));
			c2 = _mm_loadu_ps((mlib_f32 *)(sp2 + 2 * SSIZE));
			d1 = _mm_loadu_ps((mlib_f32 *)(sp3 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			h1 = C_COMP(c0, c2);

			r0 = C_COMP(vv, a1);
			r1 = C_COMP(vv, d1);
			r0 = C_COMP(r0, h0);
			r1 = C_COMP(r1, h1);

			_mm_storeu_ps((mlib_f32 *)dp0, r0);
			dp0++;
			_mm_storeu_ps((mlib_f32 *)dp1, r1);
			dp1++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
		}

		if (tail) {
			a1 = _mm_loadu_ps((mlib_f32 *)(sp0 + SSIZE));
			b0 = _mm_loadu_ps((mlib_f32 *)(sp1));
			b1 = _mm_loadu_ps((mlib_f32 *)(sp1 + SSIZE));
			b2 = _mm_loadu_ps((mlib_f32 *)(sp1 + 2 * SSIZE));
			c0 = _mm_loadu_ps((mlib_f32 *)(sp2));
			c1 = _mm_loadu_ps((mlib_f32 *)(sp2 + SSIZE));
			c2 = _mm_loadu_ps((mlib_f32 *)(sp2 + 2 * SSIZE));
			d1 = _mm_loadu_ps((mlib_f32 *)(sp3 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			h1 = C_COMP(c0, c2);

			r0 = C_COMP(vv, a1);
			r1 = C_COMP(vv, d1);
			r0 = C_COMP(r0, h0);
			r1 = C_COMP(r1, h1);

			t0 = _mm_loadu_ps((mlib_f32 *)dp0);
			t1 = _mm_loadu_ps((mlib_f32 *)dp1);
			t0 =
			    _mm_or_ps(_mm_and_ps(mask, r0),
			    _mm_andnot_ps(mask, t0));
			t1 =
			    _mm_or_ps(_mm_and_ps(mask, r1),
			    _mm_andnot_ps(mask, t1));

			_mm_storeu_ps((mlib_f32 *)dp0, t0);
			_mm_storeu_ps((mlib_f32 *)dp1, t1);
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
			a1 = _mm_loadu_ps((mlib_f32 *)(sp0 + SSIZE));
			b0 = _mm_loadu_ps((mlib_f32 *)(sp1));
			b1 = _mm_loadu_ps((mlib_f32 *)(sp1 + SSIZE));
			b2 = _mm_loadu_ps((mlib_f32 *)(sp1 + 2 * SSIZE));
			c1 = _mm_loadu_ps((mlib_f32 *)(sp2 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			r0 = C_COMP(vv, a1);
			r0 = C_COMP(r0, h0);

			_mm_storeu_ps((mlib_f32 *)dp0, r0);
			dp0++;
			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
		}

		if (tail) {
			a1 = _mm_loadu_ps((mlib_f32 *)(sp0 + SSIZE));
			b0 = _mm_loadu_ps((mlib_f32 *)(sp1));
			b1 = _mm_loadu_ps((mlib_f32 *)(sp1 + SSIZE));
			b2 = _mm_loadu_ps((mlib_f32 *)(sp1 + 2 * SSIZE));
			c1 = _mm_loadu_ps((mlib_f32 *)(sp2 + SSIZE));

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			r0 = C_COMP(vv, a1);
			r0 = C_COMP(r0, h0);

			t0 = _mm_loadu_ps((mlib_f32 *)dp0);
			t0 =
			    _mm_or_ps(_mm_and_ps(mask, r0),
			    _mm_andnot_ps(mask, t0));
			_mm_storeu_ps((mlib_f32 *)dp0, t0);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
