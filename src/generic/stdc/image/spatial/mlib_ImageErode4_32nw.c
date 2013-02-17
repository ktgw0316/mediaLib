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

#pragma ident	"@(#)mlib_ImageErode4_32nw.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      Does Dilate4 or Erode4 filter. Edge policy = no write.
 *
 * DESCRIPTION
 *      The compiler switch DILATE_FILTER controls the macro C_COMP,
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
 */

#include <mlib_image.h>
#include <mlib_ImageErode4.h>

/* *********************************************************** */

#define	DILATE_FILTER	0

/* *********************************************************** */

#define	F_NAME(NAME)	mlib_Image##NAME##_S32

/* *********************************************************** */

#define	STYPE	mlib_s32
#define	TTYPE	mlib_d64

/* *********************************************************** */

#if DILATE_FILTER
#define	C_COMP(a, b)	(a > b ? a : b)
#else /* DILATE_FILTER */

#define	C_COMP(a, b)	(a < b ? a : b)
#endif /* DILATE_FILTER */

/* *********************************************************** */

#if DILATE_FILTER
mlib_status F_NAME(
    Dilate4) (
    STYPE * dst,
    const STYPE * src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else /* DILATE_FILTER */
mlib_status F_NAME(
    Erode4) (
    STYPE * dst,
    const STYPE * src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* DILATE_FILTER */
{
	STYPE *sl0, *sl1, *sl2, *sl3, *dl0, *dl1;
	TTYPE a1, b0, b1, b2, c0, c1, c2, d1;
	TTYPE vv, x0, y0, x1, y1;
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
 * line0:     a1
 * line1:  b0 b1 b2
 * line2:  c0 c1 c2
 * line3:     d1
 */

		b0 = sl1[0];
		c0 = sl2[0];
		b1 = sl1[1];
		c1 = sl2[1];
		vv = C_COMP(b1, c1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid - 2; i++) {
			a1 = sl0[i + 1];
			b2 = sl1[i + 2];
			c2 = sl2[i + 2];
			d1 = sl3[i + 1];

			x0 = C_COMP(a1, b2);
			x1 = C_COMP(d1, c2);

			y0 = C_COMP(vv, b0);
			y1 = C_COMP(vv, c0);

			dl0[i] = (STYPE) C_COMP(x0, y0);
			dl1[i] = (STYPE) C_COMP(x1, y1);

			vv = C_COMP(b2, c2);
			b0 = b1;
			c0 = c1;
			b1 = b2;
			c1 = c2;
		}

		sl0 = sl2;
		dl0 = dl1 + dlb;
	}

/* last line */

	if (j == (hgt - 3)) {
		TTYPE a2, b3, h1, g1, g2, v1, v2;

		sl1 = sl0 + slb;
		sl2 = sl1 + slb;

		b0 = sl1[0];
		b1 = sl1[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i <= (wid - 4); i += 2) {
			a1 = sl0[i + 1];
			a2 = sl0[i + 2];
			b2 = sl1[i + 2];
			b3 = sl1[i + 3];
			c1 = sl2[i + 1];
			c2 = sl2[i + 2];

			v1 = C_COMP(b0, c1);
			v2 = C_COMP(b3, c2);
			h1 = C_COMP(b1, b2);

			g1 = C_COMP(a1, v1);
			g2 = C_COMP(a2, v2);

			dl0[i] = (STYPE) C_COMP(h1, g1);
			dl0[i + 1] = (STYPE) C_COMP(h1, g2);

			b0 = b2;
			b1 = b3;
		}

		if (i < (wid - 2)) {
			a1 = sl0[i + 1];
			b2 = sl1[i + 2];
			c1 = sl2[i + 1];

			v1 = C_COMP(b0, c1);
			h1 = C_COMP(b1, b2);

			g1 = C_COMP(a1, v1);

			dl0[i] = (STYPE) C_COMP(h1, g1);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
