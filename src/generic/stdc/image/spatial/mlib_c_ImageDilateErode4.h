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

#ifndef _MLIB_C_IMAGEDILATEERODE4_H
#define	_MLIB_C_IMAGEDILATEERODE4_H

#pragma ident	"@(#)mlib_c_ImageDilateErode4.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#if DILATE_FILTER

#include <mlib_ImageDilate4.h>
#define	C_COMP(a, b)	(a + ((b - a) &~ ((b - a) >> 31)))
#define	F_NAMET(TYPE)	mlib_ImageDilate4_##TYPE

#else		   /* DILATE_FILTER */

#include <mlib_ImageErode4.h>
#define	C_COMP(a, b)	(a + ((b - a) & ((b - a) >> 31)))
#define	F_NAMET(TYPE)	mlib_ImageErode4_##TYPE

#endif		   /* DILATE_FILTER */

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	F_NAME	F_NAMET(U8)
#define	STYPE	mlib_u8
#define	TTYPE	mlib_s32

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	F_NAME	F_NAMET(S16)
#define	STYPE	mlib_s16
#define	TTYPE	mlib_s32

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	F_NAME	F_NAMET(U16)
#define	STYPE	mlib_u16
#define	TTYPE	mlib_s32

#endif		   /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */
mlib_status
F_NAME(
    STYPE *dst,
    const STYPE *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
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

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGEDILATEERODE4_H */
