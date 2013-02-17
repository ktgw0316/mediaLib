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

#pragma ident   "@(#)mlib_s_ImageMinFilter5x5_D64.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMinFilter5x5_D64 - internal function for
 *      mlib_ImageMinFilter5x5
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMinFilter.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	MAX_FILTER  0

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
#define	C_COMP(a, b)  _mm_max_pd(a, b)
#define	F_NAMET(TYPE)   mlib_ImageMaxFilter5x5_##TYPE

#else
#define	C_COMP(a, b)  _mm_min_pd(a, b)
#define	F_NAMET(TYPE)   mlib_ImageMinFilter5x5_##TYPE

#endif /* MAX_FILTER */

/* *********************************************************** */

#define	F_NAME	F_NAMET(D64)
#define	STYPE	mlib_d64

/* *********************************************************** */
#define	SSIZE	sizeof (STYPE)
#define	KSIZE1  (5 - 1)
/* *********************************************************** */

mlib_status
F_NAME(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
{
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buff3, *buffT;
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *sp4, *sp5, *dl;
	__m128d *dp0, *dp1;
	__m128d aa, bb, cc, dd, e0, e1, e2, e3, e4, ee, f0, f1, f2, f3, f4, ff,
	    r0, r1, t0, t1;
	__m128d e_mask;
	mlib_s32 i, j, wid16, tail;

	wid = (wid - KSIZE1) * SSIZE;
	wid16 = (wid + 15) & ~15;
	pbuff = __mlib_malloc(4 * wid16);
	buff0 = pbuff;
	buff1 = buff0 + wid16;
	buff2 = buff1 + wid16;
	buff3 = buff2 + wid16;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst + 2 * (dlb + SSIZE);

	tail = wid & 15;

	((mlib_d64 *)&e_mask)[0] =
		((mlib_d64 *)((__m128d *) mlib_mask128i_arr + tail))[0];
	((mlib_d64 *)&e_mask)[1] =
		((mlib_d64 *)((__m128d *) mlib_mask128i_arr + tail))[1];

	for (j = 0; j < 2; j++) {
		sp0 = buff0;
		sp1 = buff1;
		sp4 = sl;
		sp5 = sl + slb;
		sl += 2 * slb;

		for (i = 0; i < wid; i += 16) {
			e0 = _mm_loadu_pd((mlib_d64 *)sp4);
			e1 = _mm_loadu_pd((mlib_d64 *)(sp4 + SSIZE));
			e2 = _mm_loadu_pd((mlib_d64 *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_pd((mlib_d64 *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_pd((mlib_d64 *)(sp4 + 4 * SSIZE));
			f0 = _mm_loadu_pd((mlib_d64 *)sp5);
			f1 = _mm_loadu_pd((mlib_d64 *)(sp5 + SSIZE));
			f2 = _mm_loadu_pd((mlib_d64 *)(sp5 + 2 * SSIZE));
			f3 = _mm_loadu_pd((mlib_d64 *)(sp5 + 3 * SSIZE));
			f4 = _mm_loadu_pd((mlib_d64 *)(sp5 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			_mm_storeu_pd((mlib_d64 *)sp0, ee);
			_mm_storeu_pd((mlib_d64 *)sp1, ff);

			sp0 += 16;
			sp1 += 16;
			sp4 += 16;
			sp5 += 16;
		}

		buffT = buff0;
		buff0 = buff2;
		buff2 = buffT;
		buffT = buff1;
		buff1 = buff3;
		buff3 = buffT;
	}

	for (j = 0; j <= (hgt - KSIZE1 - 2); j += 2) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff0;
		sp1 = buff1;
		sp2 = buff2;
		sp3 = buff3;
		sp4 = sl;
		sp5 = sl + slb;

/*
 *    line0:        aa
 *    line1:        bb
 *    line2:        cc
 *    line3:        dd
 *    line4:  e0 e1 e2 e3 e4
 *    line5:  f0 f1 f2 f3 f4
 */

		for (i = 0; i <= wid - 16; i += 16) {
			aa = _mm_loadu_pd((mlib_d64 *)sp0);
			bb = _mm_loadu_pd((mlib_d64 *)sp1);
			cc = _mm_loadu_pd((mlib_d64 *)sp2);
			dd = _mm_loadu_pd((mlib_d64 *)sp3);
			e0 = _mm_loadu_pd((mlib_d64 *)sp4);
			e1 = _mm_loadu_pd((mlib_d64 *)(sp4 + SSIZE));
			e2 = _mm_loadu_pd((mlib_d64 *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_pd((mlib_d64 *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_pd((mlib_d64 *)(sp4 + 4 * SSIZE));
			f0 = _mm_loadu_pd((mlib_d64 *)sp5);
			f1 = _mm_loadu_pd((mlib_d64 *)(sp5 + SSIZE));
			f2 = _mm_loadu_pd((mlib_d64 *)(sp5 + 2 * SSIZE));
			f3 = _mm_loadu_pd((mlib_d64 *)(sp5 + 3 * SSIZE));
			f4 = _mm_loadu_pd((mlib_d64 *)(sp5 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, ff);

			_mm_storeu_pd((mlib_d64 *)sp0, ee);
			_mm_storeu_pd((mlib_d64 *)sp1, ff);

			_mm_storeu_pd((mlib_d64 *)dp0, r0);
			dp0++;
			_mm_storeu_pd((mlib_d64 *)dp1, r1);
			dp1++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
			sp4 += 16;
			sp5 += 16;
		}

		if (tail) {
			aa = _mm_loadu_pd((mlib_d64 *)sp0);
			bb = _mm_loadu_pd((mlib_d64 *)sp1);
			cc = _mm_loadu_pd((mlib_d64 *)sp2);
			dd = _mm_loadu_pd((mlib_d64 *)sp3);
			e0 = _mm_loadu_pd((mlib_d64 *)sp4);
			e1 = _mm_loadu_pd((mlib_d64 *)(sp4 + SSIZE));
			e2 = _mm_loadu_pd((mlib_d64 *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_pd((mlib_d64 *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_pd((mlib_d64 *)(sp4 + 4 * SSIZE));
			f0 = _mm_loadu_pd((mlib_d64 *)sp5);
			f1 = _mm_loadu_pd((mlib_d64 *)(sp5 + SSIZE));
			f2 = _mm_loadu_pd((mlib_d64 *)(sp5 + 2 * SSIZE));
			f3 = _mm_loadu_pd((mlib_d64 *)(sp5 + 3 * SSIZE));
			f4 = _mm_loadu_pd((mlib_d64 *)(sp5 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, ff);

			_mm_storeu_pd((mlib_d64 *)sp0, ee);
			_mm_storeu_pd((mlib_d64 *)sp1, ff);

			t0 = _mm_loadu_pd((mlib_d64 *)dp0);
			t1 = _mm_loadu_pd((mlib_d64 *)dp1);
			t0 =
			    _mm_or_pd(_mm_and_pd(e_mask, r0),
			    _mm_andnot_pd(e_mask, t0));
			t1 =
			    _mm_or_pd(_mm_and_pd(e_mask, r1),
			    _mm_andnot_pd(e_mask, t1));
			_mm_storeu_pd((mlib_d64 *)dp0, t0);
			_mm_storeu_pd((mlib_d64 *)dp1, t1);
		}

		buffT = buff0;
		buff0 = buff2;
		buff2 = buffT;
		buffT = buff1;
		buff1 = buff3;
		buff3 = buffT;

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
		sp4 = sl;

		for (i = 0; i <= wid - 16; i += 16) {
			aa = _mm_loadu_pd((mlib_d64 *)sp0);
			bb = _mm_loadu_pd((mlib_d64 *)sp1);
			cc = _mm_loadu_pd((mlib_d64 *)sp2);
			dd = _mm_loadu_pd((mlib_d64 *)sp3);
			e0 = _mm_loadu_pd((mlib_d64 *)sp4);
			e1 = _mm_loadu_pd((mlib_d64 *)(sp4 + SSIZE));
			e2 = _mm_loadu_pd((mlib_d64 *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_pd((mlib_d64 *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_pd((mlib_d64 *)(sp4 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			e2 = C_COMP(e2, e3);
			ee = C_COMP(ee, e4);
			ee = C_COMP(ee, e2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);

			_mm_storeu_pd((mlib_d64 *)dp0, r0);
			dp0++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
			sp4 += 16;
		}

		if (tail) {
			aa = _mm_loadu_pd((mlib_d64 *)sp0);
			bb = _mm_loadu_pd((mlib_d64 *)sp1);
			cc = _mm_loadu_pd((mlib_d64 *)sp2);
			dd = _mm_loadu_pd((mlib_d64 *)sp3);
			e0 = _mm_loadu_pd((mlib_d64 *)sp4);
			e1 = _mm_loadu_pd((mlib_d64 *)(sp4 + SSIZE));
			e2 = _mm_loadu_pd((mlib_d64 *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_pd((mlib_d64 *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_pd((mlib_d64 *)(sp4 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			e2 = C_COMP(e2, e3);
			ee = C_COMP(ee, e4);
			ee = C_COMP(ee, e2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);

			t0 = _mm_loadu_pd((mlib_d64 *)dp0);
			t0 =
			    _mm_or_pd(_mm_and_pd(e_mask, r0),
			    _mm_andnot_pd(e_mask, t0));
			_mm_storeu_pd((mlib_d64 *)dp0, t0);
		}
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
