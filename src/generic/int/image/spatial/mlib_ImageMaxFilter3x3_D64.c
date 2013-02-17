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

#pragma ident   "@(#)mlib_ImageMaxFilter3x3_D64.c	9.2    07/11/05 SMI"

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
#include <mlib_ImageMaxFilter.h>

/* *********************************************************** */

#define	MAX_FILTER	1

/* *********************************************************** */
#if defined(_NO_LONGLONG)
/* *********************************************************** */

#define	STYPE	mlib_d64
#define	TTYPE	mlib_d64

#define	BUFF_SIZE	512

#if MAX_FILTER
#define	C_COMP(a, b)	(a > b ? a : b)
#else /* DILATE_FILTER */

#define	C_COMP(a, b)	(a < b ? a : b)
#endif /* DILATE_FILTER */
/* *********************************************************** */
#else /* else if _NO_LONGLONG */
/* *********************************************************** */
#define	STYPE	mlib_s64
#define	TTYPE	mlib_s64

#define	BUFF_SIZE	512

#if MAX_FILTER
#define	C_COMP(a, b)                                         \
	((a & 0x8000000000000000) && (b & 0x8000000000000000)) ? \
		((a < b) ? (a) : (b)) :                              \
		((a <= b) ? (b) : (a))
#else
#define	C_COMP(a, b)                                         \
	((a & 0x8000000000000000) && (b & 0x8000000000000000)) ? \
		((a > b) ? (a) : (b)) :                              \
		((a >= b) ? (b) : (a))
#endif
/* *********************************************************** */
#endif /* end _NO_LONGLONG */
/* *********************************************************** */

#if MAX_FILTER
mlib_status
mlib_ImageMaxFilter3x3_D64(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#else /* MAX_FILTER */
mlib_status
mlib_ImageMinFilter3x3_D64(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
#endif		   /* MAX_FILTER */
{
	STYPE buff_lcl[BUFF_SIZE], *buff = buff_lcl;
	STYPE *sl0, *sl1, *sl2, *sl3, *tl0, *tl1, *dl0, *dl1;
	TTYPE a0, a1, a2, a3, b0, b1, b2, b3, a12, a30, b12, b30;
	TTYPE u0, u1, u2, u3, v0, v1, v2, v3, c0, c1, c2, c3, d0, d1, d2, d3;
	mlib_s32 i, j, bsize = wid + 4;

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	if (2 * bsize > BUFF_SIZE)
		buff = __mlib_malloc(2 * bsize * sizeof (TTYPE));
	tl0 = buff;
	tl1 = buff + bsize;

	sl0 = (STYPE *) src;
	sl1 = sl0 + slb;
/* dst ptrs skip top line and left col */
	dl0 = (STYPE *) dst + dlb + 1;

	a0 = sl0[0];
	a1 = sl0[1];
	b0 = sl1[0];
	b1 = sl1[1];

	if (hgt > 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (wid - 2); i += 4) {
		a2 = sl0[i + 2];
		b2 = sl1[i + 2];
		a3 = sl0[i + 3];
		b3 = sl1[i + 3];
		a12 = C_COMP(a1, a2);
		b12 = C_COMP(b1, b2);

		tl0[i] = C_COMP(a0, a12);
		tl1[i] = C_COMP(b0, b12);
		tl0[i + 1] = C_COMP(a12, a3);
		tl1[i + 1] = C_COMP(b12, b3);

		a0 = sl0[i + 4];
		b0 = sl1[i + 4];
		a1 = sl0[i + 5];
		b1 = sl1[i + 5];
		a30 = C_COMP(a3, a0);
		b30 = C_COMP(b3, b0);

		tl0[i + 2] = C_COMP(a2, a30);
		tl1[i + 2] = C_COMP(b2, b30);
		tl0[i + 3] = C_COMP(a30, a1);
		tl1[i + 3] = C_COMP(b30, b1);
	}

	for (j = 0; j <= (hgt - 2 - 2); j += 2) {
		dl1 = dl0 + dlb;
		sl1 = sl0 + slb;
		sl2 = sl1 + slb;
		sl3 = sl2 + slb;

/*
 * line0:     u1 u2 u3 u0
 * line1:     v1 v2 v3 v0
 * line2:  a0 a1 a2 a3 a0 a1
 * line3:  b0 b1 b2 b3 b0 b1
 */

		a0 = sl2[0];
		a1 = sl2[1];
		b0 = sl3[0];
		b1 = sl3[1];

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 0; i <= (wid - 2 - 4); i += 4) {
			u1 = tl0[i];
			v1 = tl1[i];
			u2 = tl0[i + 1];
			v2 = tl1[i + 1];
			u3 = tl0[i + 2];
			v3 = tl1[i + 2];
			u0 = tl0[i + 3];
			v0 = tl1[i + 3];

			a2 = sl2[i + 2];
			b2 = sl3[i + 2];
			a3 = sl2[i + 3];
			b3 = sl3[i + 3];
			a12 = C_COMP(a1, a2);
			b12 = C_COMP(b1, b2);

			c1 = C_COMP(a0, a12);
			d1 = C_COMP(b0, b12);
			c2 = C_COMP(a12, a3);
			d2 = C_COMP(b12, b3);

			a0 = sl2[i + 4];
			b0 = sl3[i + 4];
			a1 = sl2[i + 5];
			b1 = sl3[i + 5];
			a30 = C_COMP(a3, a0);
			b30 = C_COMP(b3, b0);

			c3 = C_COMP(a2, a30);
			d3 = C_COMP(b2, b30);
			c0 = C_COMP(a30, a1);
			d0 = C_COMP(b30, b1);

			v1 = C_COMP(v1, c1);
			v2 = C_COMP(v2, c2);
			v3 = C_COMP(v3, c3);
			v0 = C_COMP(v0, c0);

			tl0[i] = c1;
			tl1[i] = d1;
			tl0[i + 1] = c2;
			tl1[i + 1] = d2;
			tl0[i + 2] = c3;
			tl1[i + 2] = d3;
			tl0[i + 3] = c0;
			tl1[i + 3] = d0;

			dl0[i] = C_COMP(u1, v1);
			dl0[i + 1] = C_COMP(u2, v2);
			dl0[i + 2] = C_COMP(u3, v3);
			dl0[i + 3] = C_COMP(u0, v0);

			dl1[i] = C_COMP(v1, d1);
			dl1[i + 1] = C_COMP(v2, d2);
			dl1[i + 2] = C_COMP(v3, d3);
			dl1[i + 3] = C_COMP(v0, d0);
		}

		if (i < (wid - 2)) {
			u1 = tl0[i];
			v1 = tl1[i];

			a2 = sl2[i + 2];
			b2 = sl3[i + 2];
			a12 = C_COMP(a1, a2);
			b12 = C_COMP(b1, b2);
			c1 = C_COMP(a0, a12);
			d1 = C_COMP(b0, b12);
			v1 = C_COMP(v1, c1);

			tl0[i] = c1;
			tl1[i] = d1;
			dl0[i] = C_COMP(u1, v1);
			dl1[i] = C_COMP(v1, d1);

			if ((i + 1) < (wid - 2)) {
				u2 = tl0[i + 1];
				v2 = tl1[i + 1];
				a3 = sl2[i + 3];
				b3 = sl3[i + 3];
				c2 = C_COMP(a12, a3);
				d2 = C_COMP(b12, b3);
				v2 = C_COMP(v2, c2);

				tl0[i + 1] = c2;
				tl1[i + 1] = d2;
				dl0[i + 1] = C_COMP(u2, v2);
				dl1[i + 1] = C_COMP(v2, d2);

				if ((i + 2) < (wid - 2)) {
					u3 = tl0[i + 2];
					v3 = tl1[i + 2];
					a0 = sl2[i + 4];
					b0 = sl3[i + 4];
					a30 = C_COMP(a3, a0);
					b30 = C_COMP(b3, b0);
					c3 = C_COMP(a2, a30);
					d3 = C_COMP(b2, b30);
					v3 = C_COMP(v3, c3);

					tl0[i + 2] = c3;
					tl1[i + 2] = d3;
					dl0[i + 2] = C_COMP(u3, v3);
					dl1[i + 2] = C_COMP(v3, d3);
				}
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
			u1 = tl0[i];
			v1 = tl1[i];
			a0 = sl2[i];
			a1 = sl2[i + 1];
			a2 = sl2[i + 2];

			u1 = C_COMP(u1, v1);
			a1 = C_COMP(a1, a0);
			a1 = C_COMP(a1, a2);

			dl0[i] = C_COMP(u1, a1);
		}
	}

	if (buff != buff_lcl)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
