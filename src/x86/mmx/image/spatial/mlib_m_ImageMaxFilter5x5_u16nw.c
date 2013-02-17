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

#pragma ident	"@(#)mlib_m_ImageMaxFilter5x5_u16nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *              Internal functions for 5x5 maximum filter or minimum filter
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
 *      Does a 5x5 maximum filter on the input image. The filtered values are
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
#define	STYPE	mlib_u16
#define	SSIZE	sizeof (STYPE)

#define	KSIZE1	(5 - 1)

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
mlib_ImageMaxFilter5x5_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else
mlib_status
mlib_ImageMinFilter5x5_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* MAX_FILTER */
{
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buff3, *buffT;
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *sp4, *sp5, *dl;
	__m64 *dp0, *dp1;
	__m64 aa, bb, cc, dd, e0, e1, e2, e3, e4, ee, f0, f1, f2, f3, f4, ff,
	    r0, r1;
	__m64 e_mask, mask8000;
	mlib_s32 i, j, wid8, tail;

	wid = (wid - KSIZE1) * SSIZE;
	wid8 = (wid + 7) & ~7;
	pbuff = mlib_malloc(4 * wid8);
	buff0 = pbuff;
	buff1 = buff0 + wid8;
	buff2 = buff1 + wid8;
	buff3 = buff2 + wid8;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst + 2 * (dlb + SSIZE);

	tail = wid & 7;
	e_mask = ((__m64 *) mlib_mask64_arr)[tail];
	mask8000 = mmx_from_int_dup(0x80008000);

	for (j = 0; j < 2; j++) {
		sp0 = buff0;
		sp1 = buff1;
		sp4 = sl;
		sp5 = sl + slb;
		sl += 2 * slb;

		for (i = 0; i < wid; i += 8) {
			e0 = *(__m64 *) sp4;
			e1 = *(__m64 *) (sp4 + SSIZE);
			e2 = *(__m64 *) (sp4 + 2 * SSIZE);
			e3 = *(__m64 *) (sp4 + 3 * SSIZE);
			e4 = *(__m64 *) (sp4 + 4 * SSIZE);
			f0 = *(__m64 *) sp5;
			f1 = *(__m64 *) (sp5 + SSIZE);
			f2 = *(__m64 *) (sp5 + 2 * SSIZE);
			f3 = *(__m64 *) (sp5 + 3 * SSIZE);
			f4 = *(__m64 *) (sp5 + 4 * SSIZE);

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			*(__m64 *) sp0 = ee;
			*(__m64 *) sp1 = ff;

			sp0 += 8;
			sp1 += 8;
			sp4 += 8;
			sp5 += 8;
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

		for (i = 0; i <= wid - 8; i += 8) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			e0 = *(__m64 *) sp4;
			e1 = *(__m64 *) (sp4 + SSIZE);
			e2 = *(__m64 *) (sp4 + 2 * SSIZE);
			e3 = *(__m64 *) (sp4 + 3 * SSIZE);
			e4 = *(__m64 *) (sp4 + 4 * SSIZE);
			f0 = *(__m64 *) sp5;
			f1 = *(__m64 *) (sp5 + SSIZE);
			f2 = *(__m64 *) (sp5 + 2 * SSIZE);
			f3 = *(__m64 *) (sp5 + 3 * SSIZE);
			f4 = *(__m64 *) (sp5 + 4 * SSIZE);

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

			*(__m64 *) sp0 = ee;
			*(__m64 *) sp1 = ff;
			(*dp0++) = r0;
			(*dp1++) = r1;

			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
			sp3 += 8;
			sp4 += 8;
			sp5 += 8;
		}

		if (tail) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			e0 = *(__m64 *) sp4;
			e1 = *(__m64 *) (sp4 + SSIZE);
			e2 = *(__m64 *) (sp4 + 2 * SSIZE);
			e3 = *(__m64 *) (sp4 + 3 * SSIZE);
			e4 = *(__m64 *) (sp4 + 4 * SSIZE);
			f0 = *(__m64 *) sp5;
			f1 = *(__m64 *) (sp5 + SSIZE);
			f2 = *(__m64 *) (sp5 + 2 * SSIZE);
			f3 = *(__m64 *) (sp5 + 3 * SSIZE);
			f4 = *(__m64 *) (sp5 + 4 * SSIZE);

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

			*(__m64 *) sp0 = ee;
			*(__m64 *) sp1 = ff;

			*dp0 =
			    _mm_or_si64(_mm_and_si64(e_mask, r0),
			    _mm_andnot_si64(e_mask, *dp0));
			*dp1 =
			    _mm_or_si64(_mm_and_si64(e_mask, r1),
			    _mm_andnot_si64(e_mask, *dp1));
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

		for (i = 0; i <= wid - 8; i += 8) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			e0 = *(__m64 *) sp4;
			e1 = *(__m64 *) (sp4 + SSIZE);
			e2 = *(__m64 *) (sp4 + 2 * SSIZE);
			e3 = *(__m64 *) (sp4 + 3 * SSIZE);
			e4 = *(__m64 *) (sp4 + 4 * SSIZE);

			ee = C_COMP(e0, e1);
			e2 = C_COMP(e2, e3);
			ee = C_COMP(ee, e4);
			ee = C_COMP(ee, e2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);
			(*dp0++) = r0;

			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
			sp3 += 8;
			sp4 += 8;
		}

		if (tail) {
			aa = *(__m64 *) sp0;
			bb = *(__m64 *) sp1;
			cc = *(__m64 *) sp2;
			dd = *(__m64 *) sp3;
			e0 = *(__m64 *) sp4;
			e1 = *(__m64 *) (sp4 + SSIZE);
			e2 = *(__m64 *) (sp4 + 2 * SSIZE);
			e3 = *(__m64 *) (sp4 + 3 * SSIZE);
			e4 = *(__m64 *) (sp4 + 4 * SSIZE);

			ee = C_COMP(e0, e1);
			e2 = C_COMP(e2, e3);
			ee = C_COMP(ee, e4);
			ee = C_COMP(ee, e2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

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
