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

#pragma ident	"@(#)mlib_m_ImageMaxFilter3x3_u16nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *              Internal functions for 3x3 maximum filter or minimum filter
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
 *      Does a 3x3 maximum filter on the input image. The filtered values are
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

#define	MAX_FILTER	1

/* *********************************************************** */
#define	STYPE	mlib_s16
#define	SSIZE	sizeof (STYPE)

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
mlib_ImageMaxFilter3x3_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else
mlib_status
mlib_ImageMinFilter3x3_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* MAX_FILTER */
{
	mlib_u8 *buff, *buff1;
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *dl;
	__m64 *dp0, *dp1;
	__m64 aa, bb, c0, c1, c2, cc, d0, d1, d2, dd, r0, r1;
	__m64 e_mask, mask8000;
	mlib_s32 i, j, wid8, tail;

	wid = (wid - 2) * SSIZE;
	wid8 = (wid + 7) & ~7;
	buff = mlib_malloc(2 * wid8);
	buff1 = buff + wid8;

	sl = (mlib_u8 *)src;
/* dst ptrs skip top j and left col */
	dl = (mlib_u8 *)dst + dlb + SSIZE;

	tail = wid & 7;
	e_mask = ((__m64 *) mlib_mask64_arr)[tail];
	mask8000 = mmx_from_int_dup(0x80008000);

	sp0 = buff;
	sp1 = buff1;
	sp2 = sl;
	sp3 = sp2 + slb;
	sl += 2 * slb;

	for (i = 0; i < wid; i += 8) {
		c0 = *(__m64 *) sp2;
		c1 = *(__m64 *) (sp2 + SSIZE);
		c2 = *(__m64 *) (sp2 + 2 * SSIZE);
		d0 = *(__m64 *) sp3;
		d1 = *(__m64 *) (sp3 + SSIZE);
		d2 = *(__m64 *) (sp3 + 2 * SSIZE);

		cc = C_COMP(c0, c1);
		dd = C_COMP(d0, d1);
		cc = C_COMP(cc, c2);
		dd = C_COMP(dd, d2);

		*(__m64 *) sp0 = cc;
		*(__m64 *) sp1 = dd;

		sp0 += 8;
		sp1 += 8;
		sp2 += 8;
		sp3 += 8;
	}

	for (j = 0; j <= (hgt - 2 - 2); j += 2) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff;
		sp1 = buff1;
		sp2 = sl;
		sp3 = sp2 + slb;

/*
 *    line0:     aa
 *    line1:     bb
 *    line2:  c0 c1 c2
 *    line3:  d0 d1 d2
 */

		for (i = 0; i <= wid - 8; i += 8) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			c0 = *(__m64 *) sp2;
			c1 = *(__m64 *) (sp2 + SSIZE);
			c2 = *(__m64 *) (sp2 + 2 * SSIZE);
			d0 = *(__m64 *) sp3;
			d1 = *(__m64 *) (sp3 + SSIZE);
			d2 = *(__m64 *) (sp3 + 2 * SSIZE);

			cc = C_COMP(c0, c1);
			dd = C_COMP(d0, d1);
			cc = C_COMP(cc, c2);
			dd = C_COMP(dd, d2);

			bb = C_COMP(bb, cc);
			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, dd);

			*(__m64 *) sp0 = cc;
			*(__m64 *) sp1 = dd;
			(*dp0++) = r0;
			(*dp1++) = r1;

			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
			sp3 += 8;
		}

		if (tail) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			c0 = *(__m64 *) sp2;
			c1 = *(__m64 *) (sp2 + SSIZE);
			c2 = *(__m64 *) (sp2 + 2 * SSIZE);
			d0 = *(__m64 *) sp3;
			d1 = *(__m64 *) (sp3 + SSIZE);
			d2 = *(__m64 *) (sp3 + 2 * SSIZE);

			cc = C_COMP(c0, c1);
			dd = C_COMP(d0, d1);
			cc = C_COMP(cc, c2);
			dd = C_COMP(dd, d2);

			bb = C_COMP(bb, cc);
			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, dd);

			*(__m64 *) sp0 = cc;
			*(__m64 *) sp1 = dd;

			*dp0 =
			    _mm_or_si64(_mm_and_si64(e_mask, r0),
			    _mm_andnot_si64(e_mask, *dp0));
			*dp1 =
			    _mm_or_si64(_mm_and_si64(e_mask, r1),
			    _mm_andnot_si64(e_mask, *dp1));
		}

		sl += 2 * slb;
		dl += 2 * dlb;
	}

/* last line */

	if (j == (hgt - 3)) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff;
		sp1 = buff1;
		sp2 = sl;

		for (i = 0; i <= wid - 8; i += 8) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			c0 = *(__m64 *) sp2;
			c1 = *(__m64 *) (sp2 + SSIZE);
			c2 = *(__m64 *) (sp2 + 2 * SSIZE);

			cc = C_COMP(c0, c1);
			cc = C_COMP(cc, c2);

			r0 = C_COMP(aa, bb);
			r0 = C_COMP(r0, cc);

			(*dp0++) = r0;

			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
		}

		if (tail) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			c0 = *(__m64 *) sp2;
			c1 = *(__m64 *) (sp2 + SSIZE);
			c2 = *(__m64 *) (sp2 + 2 * SSIZE);

			c1 = C_COMP(c0, c1);
			cc = C_COMP(c1, c2);

			r0 = C_COMP(aa, bb);
			r0 = C_COMP(r0, cc);

			*dp0 =
			    _mm_or_si64(_mm_and_si64(e_mask, r0),
			    _mm_andnot_si64(e_mask, *dp0));
		}
	}

	_mm_empty();

	mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
