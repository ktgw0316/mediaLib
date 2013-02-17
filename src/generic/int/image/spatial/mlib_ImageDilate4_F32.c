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

#pragma ident   "@(#)mlib_ImageDilate4_F32.c	9.2    07/11/05 SMI"

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

/* *********************************************************** */

#define	DILATE_FILTER	1

/* *********************************************************** */

#define	STYPE	mlib_s32
#define	TTYPE	mlib_s32

/* *********************************************************** */
#if DILATE_FILTER
#define	C_COMP(a, b)                         \
	((a & 0x80000000) && (b & 0x80000000)) ? \
		((a < b) ? (a) : (b)) :              \
		((a <= b) ? (b) : (a))
#else
#define	C_COMP(a, b)                         \
	((a & 0x80000000) && (b & 0x80000000)) ? \
		((a > b) ? (a) : (b)) :              \
		((a >= b) ? (b) : (a))
#endif

/* *********************************************************** */

#if DILATE_FILTER
mlib_status
mlib_ImageDilate4_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else /* DILATE_FILTER */
mlib_status
mlib_ImageErode4_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* DILATE_FILTER */
{
	STYPE *sl0, *sl1, *sl2, *sl3, *dl0, *dl1;
	TTYPE a1, a2, a3, b0, b1, b2, c0, c1, c2, d1, d2, d3;
	TTYPE v1, v2, v3, w1, w2, w3, h1, h2, h3, g1, g2, g3;
	mlib_s32 i, j;

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	sl0 = (STYPE *) src;
/* dst ptrs skip top j and left col */
	dl0 = (STYPE *) dst + dlb + 1;

	for (j = 0; j <= (hgt - 2 - 2); j += 2) {
		dl1 = dl0 + dlb;
		sl1 = sl0 + slb;
		sl2 = sl1 + slb;
		sl3 = sl2 + slb;

/*
 * line0:     a1 a2 a3
 * line1:  b0 b1 b2 b0 b1
 * line2:  c0 c1 c2 c0 c1
 * line3:     d1 d2 d3
 */

		b0 = sl1[0];
		b1 = sl1[1];
		c0 = sl2[0];
		c1 = sl2[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i <= (wid - 2 - 3); i += 3) {
			a1 = sl0[i + 1];
			a2 = sl0[i + 2];
			a3 = sl0[i + 3];
			b2 = sl1[i + 2];
			c2 = sl2[i + 2];
			d1 = sl3[i + 1];
			d2 = sl3[i + 2];
			d3 = sl3[i + 3];

			v1 = C_COMP(b1, c1);
			v2 = C_COMP(b2, c2);
			h1 = C_COMP(b0, b2);
			g1 = C_COMP(c0, c2);

			b0 = sl1[i + 3];
			c0 = sl2[i + 3];
			v3 = C_COMP(b0, c0);
			h2 = C_COMP(b1, b0);
			g2 = C_COMP(c1, c0);

			b1 = sl1[i + 4];
			c1 = sl2[i + 4];
			h3 = C_COMP(b2, b1);
			g3 = C_COMP(c2, c1);

			w1 = C_COMP(a1, v1);
			w2 = C_COMP(a2, v2);
			w3 = C_COMP(a3, v3);
			v1 = C_COMP(d1, v1);
			v2 = C_COMP(d2, v2);
			v3 = C_COMP(d3, v3);

			dl0[i] = C_COMP(h1, w1);
			dl0[i + 1] = C_COMP(h2, w2);
			dl0[i + 2] = C_COMP(h3, w3);

			dl1[i] = C_COMP(g1, v1);
			dl1[i + 1] = C_COMP(g2, v2);
			dl1[i + 2] = C_COMP(g3, v3);
		}

		if (i < (wid - 2)) {
			a1 = sl0[i + 1];
			a2 = sl0[i + 2];
			b2 = sl1[i + 2];
			c2 = sl2[i + 2];
			d1 = sl3[i + 1];
			d2 = sl3[i + 2];

			v1 = C_COMP(b1, c1);
			h1 = C_COMP(b0, b2);
			g1 = C_COMP(c0, c2);
			a1 = C_COMP(a1, v1);
			d1 = C_COMP(d1, v1);
			dl0[i] = C_COMP(h1, a1);
			dl1[i] = C_COMP(g1, d1);

			if ((i + 1) < (wid - 2)) {
				b0 = sl1[i + 3];
				c0 = sl2[i + 3];
				v2 = C_COMP(b2, c2);
				h2 = C_COMP(b1, b0);
				g2 = C_COMP(c1, c0);
				a2 = C_COMP(a2, v2);
				d2 = C_COMP(d2, v2);
				dl0[i + 1] = C_COMP(h2, a2);
				dl1[i + 1] = C_COMP(g2, d2);
			}
		}

		sl0 = sl2;
		dl0 = dl1 + dlb;
	}

/* last line */

	if (j == (hgt - 3)) {
		sl1 = sl0 + slb;
		sl2 = sl1 + slb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (wid - 2); i++) {
			a1 = sl0[i + 1];
			b0 = sl1[i];
			b1 = sl1[i + 1];
			b2 = sl1[i + 2];
			c1 = sl2[i + 1];
			v1 = C_COMP(b1, c1);
			h1 = C_COMP(b0, b2);
			a1 = C_COMP(a1, v1);
			dl0[i] = C_COMP(h1, a1);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
