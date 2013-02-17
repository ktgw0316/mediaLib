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

#pragma ident	"@(#)mlib_m_ImageMinFilter7x7_u16nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *              Internal functions for 7x7 maximum filter or minimum filter
 *              on the input image. Edge = no write.
 *              The compiler switch MAX_FILTER controls the macro
 *              C_COMP, #defining it as max() if true, or
 *              as min() if false. It also controls the function
 *              name. (Comments and #defined macro names always refer
 *              to "max" instead of "min".)
 *
 * ARGUMENT
 *     dst    Pointer to the first byte for first pixel of destination
 *     src    Pointer to the first byte for first pixel of source
 *     slb    Bytes count of the source row
 *     dlb    Bytes count of the destination row
 *     wid    number of elements in a row of source
 *     hgt    number of rows of source
 *
 * DESCRIPTION
 *      Does a 7x7 maximum filter on the input image. The filtered values are
 *      returned in the destination image.
 *
 * NOTE TO MAINTENANCE PROGRAMMERS !!!!!!!!!
 *
 *      If you change this min or max filter, just change one, then copy it
 *      over the other one, changing only the true/false (1/0) value of
 *      the #defined switch MAX_FILTER. For library purposes, two files are
 *      probably necessary, but keep them in sync so that their only
 *      difference is the value of the MAX_FILTER switch.
 *
 */

#include <mlib_image.h>
#include <mlib_mmx_utils.h>

#define	MAX_FILTER	0

/* *********************************************************** */
#define	STYPE	mlib_u16
#define	SSIZE	sizeof (STYPE)

#define	KSIZE1	(7 - 1)

/* *********************************************************** */
#if MAX_FILTER

#define	C_COMP(a, b)                                                    \
	_mm_add_pi16(a, _mm_and_si64(_mm_sub_pi16(b, a),                \
	    _mm_cmpgt_pi16(_mm_xor_si64(b, mask8000), _mm_xor_si64(a,   \
	    mask8000))))

#else

#define	C_COMP(a, b)                                                    \
	_mm_add_pi16(a, _mm_and_si64(_mm_sub_pi16(b, a),                \
	    _mm_cmpgt_pi16(_mm_xor_si64(a, mask8000), _mm_xor_si64(b,   \
	    mask8000))))

#endif /* MAX_FILTER */

/* *********************************************************** */
#if MAX_FILTER
mlib_status
mlib_ImageMaxFilter7x7_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else
mlib_status
mlib_ImageMinFilter7x7_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* MAX_FILTER */
{
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buff3, *buff4, *buff5, *buffT;
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *sp4, *sp5, *sp6, *sp7, *dl;
	__m64 *dp0, *dp1;
	__m64 aa, bb, cc, dd, ee, ff, r0, r1;
	__m64 g0, g1, g2, g3, g4, g5, g6, gg;
	__m64 h0, h1, h2, h3, h4, h5, h6, hh;
	__m64 e_mask, mask8000;
	mlib_s32 i, j, wid8, tail;

	wid = (wid - KSIZE1) * SSIZE;
	wid8 = (wid + 7) & ~7;
	pbuff = mlib_malloc(KSIZE1 * wid8);
	buff0 = pbuff;
	buff1 = buff0 + wid8;
	buff2 = buff1 + wid8;
	buff3 = buff2 + wid8;
	buff4 = buff3 + wid8;
	buff5 = buff4 + wid8;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst + (KSIZE1 / 2) * (dlb + SSIZE);

	tail = wid & 7;
	e_mask = ((__m64 *) mlib_mask64_arr)[tail];
	mask8000 = mmx_from_int_dup(0x80008000);

	for (j = 0; j < 3; j++) {
		sp0 = buff4;
		sp1 = buff5;
		sp6 = sl;
		sp7 = sl + slb;
		sl += 2 * slb;

		for (i = 0; i < wid; i += 8) {
			g0 = *(__m64 *) sp6;
			g1 = *(__m64 *) (sp6 + SSIZE);
			g2 = *(__m64 *) (sp6 + 2 * SSIZE);
			g3 = *(__m64 *) (sp6 + 3 * SSIZE);
			g4 = *(__m64 *) (sp6 + 4 * SSIZE);
			g5 = *(__m64 *) (sp6 + 5 * SSIZE);
			g6 = *(__m64 *) (sp6 + 6 * SSIZE);
			h0 = *(__m64 *) sp7;
			h1 = *(__m64 *) (sp7 + SSIZE);
			h2 = *(__m64 *) (sp7 + 2 * SSIZE);
			h3 = *(__m64 *) (sp7 + 3 * SSIZE);
			h4 = *(__m64 *) (sp7 + 4 * SSIZE);
			h5 = *(__m64 *) (sp7 + 5 * SSIZE);
			h6 = *(__m64 *) (sp7 + 6 * SSIZE);

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

			*(__m64 *) sp0 = gg;
			*(__m64 *) sp1 = hh;

			sp0 += 8;
			sp1 += 8;
			sp6 += 8;
			sp7 += 8;
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

		for (i = 0; i <= wid - 8; i += 8) {
			g0 = *(__m64 *) sp6;
			g1 = *(__m64 *) (sp6 + SSIZE);
			g2 = *(__m64 *) (sp6 + 2 * SSIZE);
			g3 = *(__m64 *) (sp6 + 3 * SSIZE);
			g4 = *(__m64 *) (sp6 + 4 * SSIZE);
			g5 = *(__m64 *) (sp6 + 5 * SSIZE);
			g6 = *(__m64 *) (sp6 + 6 * SSIZE);
			h0 = *(__m64 *) sp7;
			h1 = *(__m64 *) (sp7 + SSIZE);
			h2 = *(__m64 *) (sp7 + 2 * SSIZE);
			h3 = *(__m64 *) (sp7 + 3 * SSIZE);
			h4 = *(__m64 *) (sp7 + 4 * SSIZE);
			h5 = *(__m64 *) (sp7 + 5 * SSIZE);
			h6 = *(__m64 *) (sp7 + 6 * SSIZE);

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

			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			ee = *(__m64 *) sp4;
			ff = *(__m64 *) sp5;

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, hh);

			*(__m64 *) sp0 = gg;
			*(__m64 *) sp1 = hh;
			(*dp0++) = r0;
			(*dp1++) = r1;

			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
			sp3 += 8;
			sp4 += 8;
			sp5 += 8;
			sp6 += 8;
			sp7 += 8;
		}

		if (tail) {
			g0 = *(__m64 *) sp6;
			g1 = *(__m64 *) (sp6 + SSIZE);
			g2 = *(__m64 *) (sp6 + 2 * SSIZE);
			g3 = *(__m64 *) (sp6 + 3 * SSIZE);
			g4 = *(__m64 *) (sp6 + 4 * SSIZE);
			g5 = *(__m64 *) (sp6 + 5 * SSIZE);
			g6 = *(__m64 *) (sp6 + 6 * SSIZE);
			h0 = *(__m64 *) sp7;
			h1 = *(__m64 *) (sp7 + SSIZE);
			h2 = *(__m64 *) (sp7 + 2 * SSIZE);
			h3 = *(__m64 *) (sp7 + 3 * SSIZE);
			h4 = *(__m64 *) (sp7 + 4 * SSIZE);
			h5 = *(__m64 *) (sp7 + 5 * SSIZE);
			h6 = *(__m64 *) (sp7 + 6 * SSIZE);

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

			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			ee = *(__m64 *) sp4;
			ff = *(__m64 *) sp5;

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, hh);

			*(__m64 *) sp0 = gg;
			*(__m64 *) sp1 = hh;

			*dp0 =
			    _mm_or_si64(_mm_and_si64(e_mask, r0),
			    _mm_andnot_si64(e_mask, *dp0));
			*dp1 =
			    _mm_or_si64(_mm_and_si64(e_mask, r1),
			    _mm_andnot_si64(e_mask, *dp1));
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

		for (i = 0; i <= wid - 8; i += 8) {
			g0 = *(__m64 *) sp6;
			g1 = *(__m64 *) (sp6 + SSIZE);
			g2 = *(__m64 *) (sp6 + 2 * SSIZE);
			g3 = *(__m64 *) (sp6 + 3 * SSIZE);
			g4 = *(__m64 *) (sp6 + 4 * SSIZE);
			g5 = *(__m64 *) (sp6 + 5 * SSIZE);
			g6 = *(__m64 *) (sp6 + 6 * SSIZE);

			gg = C_COMP(g0, g1);
			g2 = C_COMP(g2, g3);
			g4 = C_COMP(g4, g5);
			gg = C_COMP(gg, g2);
			gg = C_COMP(gg, g4);
			gg = C_COMP(gg, g6);

			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			ee = *(__m64 *) sp4;
			ff = *(__m64 *) sp5;

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);
			(*dp0++) = r0;

			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
			sp3 += 8;
			sp4 += 8;
			sp5 += 8;
			sp6 += 8;
		}

		if (tail) {
			g0 = *(__m64 *) sp6;
			g1 = *(__m64 *) (sp6 + SSIZE);
			g2 = *(__m64 *) (sp6 + 2 * SSIZE);
			g3 = *(__m64 *) (sp6 + 3 * SSIZE);
			g4 = *(__m64 *) (sp6 + 4 * SSIZE);
			g5 = *(__m64 *) (sp6 + 5 * SSIZE);
			g6 = *(__m64 *) (sp6 + 6 * SSIZE);

			gg = C_COMP(g0, g1);
			g2 = C_COMP(g2, g3);
			g4 = C_COMP(g4, g5);
			gg = C_COMP(gg, g2);
			gg = C_COMP(gg, g4);
			gg = C_COMP(gg, g6);

			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			ee = *(__m64 *) sp4;
			ff = *(__m64 *) sp5;

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			ff = C_COMP(ff, gg);
			bb = C_COMP(bb, dd);
			bb = C_COMP(bb, ff);

			r0 = C_COMP(aa, bb);

			*dp0 =
			    _mm_or_si64(_mm_and_si64(e_mask, r0),
			    _mm_andnot_si64(e_mask, *dp0));
		}
	}

	_mm_empty();

	mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
