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

#pragma ident   "@(#)mlib_s_ImageMaxFilter7x7_F32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMaxFilter7x7_F32 - internal function for
 *      mlib_ImageMaxFilter7x7
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMaxFilter.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	MAX_FILTER  1

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
#if MAX_FILTER

#define	C_COMP(a, b)   _mm_max_ps(a, b)
#define	F_NAMET(TYPE)   mlib_ImageMaxFilter7x7_##TYPE

#else

#define	C_COMP(a, b)   _mm_min_ps(a, b)
#define	F_NAMET(TYPE)   mlib_ImageMinFilter7x7_##TYPE

#endif /* MAX_FILTER */

/* *********************************************************** */

#define	F_NAME	F_NAMET(F32)
#define	STYPE	mlib_f32

/* *********************************************************** */
#define	SSIZE	sizeof (STYPE)
#define	KSIZE1  (7 - 1)
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
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buff3, *buff4, *buff5, *buffT;
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *sp4, *sp5, *sp6, *sp7, *dl;
	__m128 *dp0, *dp1;
	__m128 aa, bb, cc, dd, ee, ff, r0, r1, t0, t1;
	__m128 g0, g1, g2, g3, g4, g5, g6, gg;
	__m128 h0, h1, h2, h3, h4, h5, h6, hh;
	__m128 e_mask;
	mlib_s32 i, j, wid16, tail;

	wid = (wid - KSIZE1) * SSIZE;
	wid16 = (wid + 15) & ~15;
	pbuff = __mlib_malloc(KSIZE1 * wid16);
	buff0 = pbuff;
	buff1 = buff0 + wid16;
	buff2 = buff1 + wid16;
	buff3 = buff2 + wid16;
	buff4 = buff3 + wid16;
	buff5 = buff4 + wid16;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst + (KSIZE1 / 2) * (dlb + SSIZE);

	tail = wid & 15;

	((mlib_d64 *)&e_mask)[0] =
		((mlib_d64 *)((__m128 *) mlib_mask128i_arr + tail))[0];
	((mlib_d64 *)&e_mask)[1] =
		((mlib_d64 *)((__m128 *) mlib_mask128i_arr + tail))[1];

	for (j = 0; j < 3; j++) {
		sp0 = buff4;
		sp1 = buff5;
		sp6 = sl;
		sp7 = sl + slb;
		sl += 2 * slb;

		for (i = 0; i < wid; i += 16) {
			g0 = _mm_loadu_ps((mlib_f32 *)sp6);
			g1 = _mm_loadu_ps((mlib_f32 *)(sp6 + SSIZE));
			g2 = _mm_loadu_ps((mlib_f32 *)(sp6 + 2 * SSIZE));
			g3 = _mm_loadu_ps((mlib_f32 *)(sp6 + 3 * SSIZE));
			g4 = _mm_loadu_ps((mlib_f32 *)(sp6 + 4 * SSIZE));
			g5 = _mm_loadu_ps((mlib_f32 *)(sp6 + 5 * SSIZE));
			g6 = _mm_loadu_ps((mlib_f32 *)(sp6 + 6 * SSIZE));
			h0 = _mm_loadu_ps((mlib_f32 *)sp7);
			h1 = _mm_loadu_ps((mlib_f32 *)(sp7 + SSIZE));
			h2 = _mm_loadu_ps((mlib_f32 *)(sp7 + 2 * SSIZE));
			h3 = _mm_loadu_ps((mlib_f32 *)(sp7 + 3 * SSIZE));
			h4 = _mm_loadu_ps((mlib_f32 *)(sp7 + 4 * SSIZE));
			h5 = _mm_loadu_ps((mlib_f32 *)(sp7 + 5 * SSIZE));
			h6 = _mm_loadu_ps((mlib_f32 *)(sp7 + 6 * SSIZE));

			gg = C_COMP(g0, g1);
			hh = C_COMP(h0, h1);
			g2 = C_COMP(g2, g3);
			h2 = C_COMP(h2, h3);
			g4 = C_COMP(g4, g5);
			h4 = C_COMP(h4, h5);
			gg = C_COMP(gg, g2);
			hh = C_COMP(hh, h2);
			gg = C_COMP(gg, g4);
			hh = C_COMP(hh, h4);
			gg = C_COMP(gg, g6);
			hh = C_COMP(hh, h6);

			_mm_storeu_ps((mlib_f32 *)sp0, gg);
			_mm_storeu_ps((mlib_f32 *)sp1, hh);

			sp0 += 16;
			sp1 += 16;
			sp6 += 16;
			sp7 += 16;
		}

		if (j < 2) {
			buffT = buff0;
			buff0 = buff2;
			buff2 = buff4;
			buff4 = buffT;
			buffT = buff1;
			buff1 = buff3;
			buff3 = buff5;
			buff5 = buffT;
		}
	}

	for (j = 0; j <= (hgt - KSIZE1 - 2); j += 2) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff0;
		sp1 = buff1;
		sp2 = buff2;
		sp3 = buff3;
		sp4 = buff4;
		sp5 = buff5;
		sp6 = sl;
		sp7 = sl + slb;

/*
 *    line0:           aa
 *    line1:           bb
 *    line2:           cc
 *    line3:           dd
 *    line4:           ee
 *    line5:           ff
 *    line4:  g0 g1 g2 g3 g4 g5 g6
 *    line5:  h0 h1 h2 h3 h4 h5 h6
 */

		for (i = 0; i <= wid - 16; i += 16) {
			g0 = _mm_loadu_ps((mlib_f32 *)sp6);
			g1 = _mm_loadu_ps((mlib_f32 *)(sp6 + SSIZE));
			g2 = _mm_loadu_ps((mlib_f32 *)(sp6 + 2 * SSIZE));
			g3 = _mm_loadu_ps((mlib_f32 *)(sp6 + 3 * SSIZE));
			g4 = _mm_loadu_ps((mlib_f32 *)(sp6 + 4 * SSIZE));
			g5 = _mm_loadu_ps((mlib_f32 *)(sp6 + 5 * SSIZE));
			g6 = _mm_loadu_ps((mlib_f32 *)(sp6 + 6 * SSIZE));
			h0 = _mm_loadu_ps((mlib_f32 *)sp7);
			h1 = _mm_loadu_ps((mlib_f32 *)(sp7 + SSIZE));
			h2 = _mm_loadu_ps((mlib_f32 *)(sp7 + 2 * SSIZE));
			h3 = _mm_loadu_ps((mlib_f32 *)(sp7 + 3 * SSIZE));
			h4 = _mm_loadu_ps((mlib_f32 *)(sp7 + 4 * SSIZE));
			h5 = _mm_loadu_ps((mlib_f32 *)(sp7 + 5 * SSIZE));
			h6 = _mm_loadu_ps((mlib_f32 *)(sp7 + 6 * SSIZE));

			gg = C_COMP(g0, g1);
			hh = C_COMP(h0, h1);
			g2 = C_COMP(g2, g3);
			h2 = C_COMP(h2, h3);
			g4 = C_COMP(g4, g5);
			h4 = C_COMP(h4, h5);
			gg = C_COMP(gg, g2);
			hh = C_COMP(hh, h2);
			gg = C_COMP(gg, g4);
			hh = C_COMP(hh, h4);
			gg = C_COMP(gg, g6);
			hh = C_COMP(hh, h6);

			aa = _mm_loadu_ps((mlib_f32 *)sp0);
			bb = _mm_loadu_ps((mlib_f32 *)sp1);
			cc = _mm_loadu_ps((mlib_f32 *)sp2);
			dd = _mm_loadu_ps((mlib_f32 *)sp3);
			ee = _mm_loadu_ps((mlib_f32 *)sp4);
			ff = _mm_loadu_ps((mlib_f32 *)sp5);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, hh);

			_mm_storeu_ps((mlib_f32 *)sp0, gg);
			_mm_storeu_ps((mlib_f32 *)sp1, hh);

			_mm_storeu_ps((mlib_f32 *)dp0, r0);
			dp0++;
			_mm_storeu_ps((mlib_f32 *)dp1, r1);
			dp1++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
			sp4 += 16;
			sp5 += 16;
			sp6 += 16;
			sp7 += 16;
		}

		if (tail) {
			g0 = _mm_loadu_ps((mlib_f32 *)sp6);
			g1 = _mm_loadu_ps((mlib_f32 *)(sp6 + SSIZE));
			g2 = _mm_loadu_ps((mlib_f32 *)(sp6 + 2 * SSIZE));
			g3 = _mm_loadu_ps((mlib_f32 *)(sp6 + 3 * SSIZE));
			g4 = _mm_loadu_ps((mlib_f32 *)(sp6 + 4 * SSIZE));
			g5 = _mm_loadu_ps((mlib_f32 *)(sp6 + 5 * SSIZE));
			g6 = _mm_loadu_ps((mlib_f32 *)(sp6 + 6 * SSIZE));
			h0 = _mm_loadu_ps((mlib_f32 *)sp7);
			h1 = _mm_loadu_ps((mlib_f32 *)(sp7 + SSIZE));
			h2 = _mm_loadu_ps((mlib_f32 *)(sp7 + 2 * SSIZE));
			h3 = _mm_loadu_ps((mlib_f32 *)(sp7 + 3 * SSIZE));
			h4 = _mm_loadu_ps((mlib_f32 *)(sp7 + 4 * SSIZE));
			h5 = _mm_loadu_ps((mlib_f32 *)(sp7 + 5 * SSIZE));
			h6 = _mm_loadu_ps((mlib_f32 *)(sp7 + 6 * SSIZE));

			gg = C_COMP(g0, g1);
			hh = C_COMP(h0, h1);
			g2 = C_COMP(g2, g3);
			h2 = C_COMP(h2, h3);
			g4 = C_COMP(g4, g5);
			h4 = C_COMP(h4, h5);
			gg = C_COMP(gg, g2);
			hh = C_COMP(hh, h2);
			gg = C_COMP(gg, g4);
			hh = C_COMP(hh, h4);
			gg = C_COMP(gg, g6);
			hh = C_COMP(hh, h6);

			aa = _mm_loadu_ps((mlib_f32 *)sp0);
			bb = _mm_loadu_ps((mlib_f32 *)sp1);
			cc = _mm_loadu_ps((mlib_f32 *)sp2);
			dd = _mm_loadu_ps((mlib_f32 *)sp3);
			ee = _mm_loadu_ps((mlib_f32 *)sp4);
			ff = _mm_loadu_ps((mlib_f32 *)sp5);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, hh);

			_mm_storeu_ps((mlib_f32 *)sp0, gg);
			_mm_storeu_ps((mlib_f32 *)sp1, hh);

			t0 = _mm_loadu_ps((mlib_f32 *)dp0);
			t1 = _mm_loadu_ps((mlib_f32 *)dp1);
			t0 =
			    _mm_or_ps(_mm_and_ps(e_mask, r0),
			    _mm_andnot_ps(e_mask, t0));
			t1 =
			    _mm_or_ps(_mm_and_ps(e_mask, r1),
			    _mm_andnot_ps(e_mask, t1));
			_mm_storeu_ps((mlib_f32 *)dp0, t0);
			_mm_storeu_ps((mlib_f32 *)dp1, t1);
		}

		buffT = buff0;
		buff0 = buff2;
		buff2 = buff4;
		buff4 = buffT;
		buffT = buff1;
		buff1 = buff3;
		buff3 = buff5;
		buff5 = buffT;

		sl += 2 * slb;
		dl += 2 * dlb;
	}

/* last line */

	if (j == (hgt - KSIZE1 - 1)) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff0;
		sp1 = buff1;
		sp2 = buff2;
		sp3 = buff3;
		sp4 = buff4;
		sp5 = buff5;
		sp6 = sl;

		for (i = 0; i <= wid - 16; i += 16) {
			g0 = _mm_loadu_ps((mlib_f32 *)sp6);
			g1 = _mm_loadu_ps((mlib_f32 *)(sp6 + SSIZE));
			g2 = _mm_loadu_ps((mlib_f32 *)(sp6 + 2 * SSIZE));
			g3 = _mm_loadu_ps((mlib_f32 *)(sp6 + 3 * SSIZE));
			g4 = _mm_loadu_ps((mlib_f32 *)(sp6 + 4 * SSIZE));
			g5 = _mm_loadu_ps((mlib_f32 *)(sp6 + 5 * SSIZE));
			g6 = _mm_loadu_ps((mlib_f32 *)(sp6 + 6 * SSIZE));

			gg = C_COMP(g0, g1);
			g2 = C_COMP(g2, g3);
			g4 = C_COMP(g4, g5);
			gg = C_COMP(gg, g2);
			gg = C_COMP(gg, g4);
			gg = C_COMP(gg, g6);

			aa = _mm_loadu_ps((mlib_f32 *)sp0);
			bb = _mm_loadu_ps((mlib_f32 *)sp1);
			cc = _mm_loadu_ps((mlib_f32 *)sp2);
			dd = _mm_loadu_ps((mlib_f32 *)sp3);
			ee = _mm_loadu_ps((mlib_f32 *)sp4);
			ff = _mm_loadu_ps((mlib_f32 *)sp5);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);

			_mm_storeu_ps((mlib_f32 *)dp0, r0);
			dp0++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
			sp4 += 16;
			sp5 += 16;
			sp6 += 16;
		}

		if (tail) {
			g0 = _mm_loadu_ps((mlib_f32 *)sp6);
			g1 = _mm_loadu_ps((mlib_f32 *)(sp6 + SSIZE));
			g2 = _mm_loadu_ps((mlib_f32 *)(sp6 + 2 * SSIZE));
			g3 = _mm_loadu_ps((mlib_f32 *)(sp6 + 3 * SSIZE));
			g4 = _mm_loadu_ps((mlib_f32 *)(sp6 + 4 * SSIZE));
			g5 = _mm_loadu_ps((mlib_f32 *)(sp6 + 5 * SSIZE));
			g6 = _mm_loadu_ps((mlib_f32 *)(sp6 + 6 * SSIZE));

			gg = C_COMP(g0, g1);
			g2 = C_COMP(g2, g3);
			g4 = C_COMP(g4, g5);
			gg = C_COMP(gg, g2);
			gg = C_COMP(gg, g4);
			gg = C_COMP(gg, g6);

			aa = _mm_loadu_ps((mlib_f32 *)sp0);
			bb = _mm_loadu_ps((mlib_f32 *)sp1);
			cc = _mm_loadu_ps((mlib_f32 *)sp2);
			dd = _mm_loadu_ps((mlib_f32 *)sp3);
			ee = _mm_loadu_ps((mlib_f32 *)sp4);
			ff = _mm_loadu_ps((mlib_f32 *)sp5);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);

			t0 = _mm_loadu_ps((mlib_f32 *)dp0);
			t0 =
			    _mm_or_ps(_mm_and_ps(e_mask, r0),
			    _mm_andnot_ps(e_mask, t0));
			_mm_storeu_ps((mlib_f32 *)dp0, t0);
		}
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
