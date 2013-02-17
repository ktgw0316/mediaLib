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

#pragma ident	"@(#)mlib_m_ImageErode4_u16nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDilate4_U16.c -
 *      OR:
 *      mlib_ImageErode4_U16.c -
 *              Does a 3x3 erode 4 or dilate 4 filter on the
 *              8-bit input image. Edge policy = no write.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDilate4_U8(void     *dst,
 *                                       void     *src,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 slb,
 *                                       mlib_s32 wid,
 *                                       mlib_s32 hgt)
 *      OR
 *      mlib_status mlib_ImageErode4_U8(void     *dst,
 *                                      void     *src,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 slb,
 *                                      mlib_s32 wid,
 *                                      mlib_s32 hgt)
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
 * ***************************************************************************
 */

#include <mlib_image.h>
#include <mlib_mmx_utils.h>

#define	DILATE_FILTER	0

/* *********************************************************** */
#define	STYPE	mlib_u16
#define	SSIZE	sizeof (STYPE)

/* *********************************************************** */
#if DILATE_FILTER

#define	C_COMP(a, b)                                                  \
	_mm_add_pi16(a, _mm_and_si64(_mm_sub_pi16(b, a),              \
	    _mm_cmpgt_pi16(_mm_xor_si64(b, mask80), _mm_xor_si64(a,   \
	    mask80))))

#else

#define	C_COMP(a, b)                                                  \
	_mm_add_pi16(a, _mm_and_si64(_mm_sub_pi16(b, a),              \
	    _mm_cmpgt_pi16(_mm_xor_si64(a, mask80), _mm_xor_si64(b,   \
	    mask80))))

#endif /* DILATE_FILTER */

/* *********************************************************** */
#if DILATE_FILTER
mlib_status
mlib_ImageDilate4_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else
mlib_status
mlib_ImageErode4_U16(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* DILATE_FILTER */
{
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *dl;
	__m64 *dp0, *dp1;
	__m64 a1, b0, b1, b2, c0, c1, c2, d1, vv, h0, h1, r0, r1;
	__m64 mask, mask80;
	mlib_s32 i, j, tail;

	sl = (mlib_u8 *)src;
/* dst ptrs skip top j and left col */
	dl = (mlib_u8 *)dst + dlb + SSIZE;

	wid = (wid - 2) * SSIZE;
	tail = wid & 7;
	mask = ((__m64 *) mlib_mask64_arr)[tail];
	mask80 = mmx_from_int_dup(0x80008000);

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

		for (i = 0; i <= wid - 8; i += 8) {
			a1 = *(__m64 *) (sp0 + SSIZE);
			b0 = *(__m64 *) (sp1);
			b1 = *(__m64 *) (sp1 + SSIZE);
			b2 = *(__m64 *) (sp1 + 2 * SSIZE);
			c0 = *(__m64 *) (sp2);
			c1 = *(__m64 *) (sp2 + SSIZE);
			c2 = *(__m64 *) (sp2 + 2 * SSIZE);
			d1 = *(__m64 *) (sp3 + SSIZE);

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			h1 = C_COMP(c0, c2);

			r0 = C_COMP(vv, a1);
			r1 = C_COMP(vv, d1);
			r0 = C_COMP(r0, h0);
			r1 = C_COMP(r1, h1);

			(*dp0++) = r0;
			(*dp1++) = r1;
			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
			sp3 += 8;
		}

		if (tail) {
			a1 = *(__m64 *) (sp0 + SSIZE);
			b0 = *(__m64 *) (sp1);
			b1 = *(__m64 *) (sp1 + SSIZE);
			b2 = *(__m64 *) (sp1 + 2 * SSIZE);
			c0 = *(__m64 *) (sp2);
			c1 = *(__m64 *) (sp2 + SSIZE);
			c2 = *(__m64 *) (sp2 + 2 * SSIZE);
			d1 = *(__m64 *) (sp3 + SSIZE);

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			h1 = C_COMP(c0, c2);

			r0 = C_COMP(vv, a1);
			r1 = C_COMP(vv, d1);
			r0 = C_COMP(r0, h0);
			r1 = C_COMP(r1, h1);

			*dp0 =
			    _mm_or_si64(_mm_and_si64(mask, r0),
			    _mm_andnot_si64(mask, *dp0));
			*dp1 =
			    _mm_or_si64(_mm_and_si64(mask, r1),
			    _mm_andnot_si64(mask, *dp1));
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

		for (i = 0; i <= wid - 8; i += 8) {
			a1 = *(__m64 *) (sp0 + SSIZE);
			b0 = *(__m64 *) (sp1);
			b1 = *(__m64 *) (sp1 + SSIZE);
			b2 = *(__m64 *) (sp1 + 2 * SSIZE);
			c1 = *(__m64 *) (sp2 + SSIZE);

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			r0 = C_COMP(vv, a1);
			r0 = C_COMP(r0, h0);

			(*dp0++) = r0;
			sp0 += 8;
			sp1 += 8;
			sp2 += 8;
		}

		if (tail) {
			a1 = *(__m64 *) (sp0 + SSIZE);
			b0 = *(__m64 *) (sp1);
			b1 = *(__m64 *) (sp1 + SSIZE);
			b2 = *(__m64 *) (sp1 + 2 * SSIZE);
			c1 = *(__m64 *) (sp2 + SSIZE);

			vv = C_COMP(b1, c1);
			h0 = C_COMP(b0, b2);
			r0 = C_COMP(vv, a1);
			r0 = C_COMP(r0, h0);

			*dp0 =
			    _mm_or_si64(_mm_and_si64(mask, r0),
			    _mm_andnot_si64(mask, *dp0));
		}
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
