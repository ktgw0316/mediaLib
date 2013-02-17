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

#pragma ident   "@(#)mlib_ImageMaxFilter5x5_32nw.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMaxFilter5x5_S32 - internal function for
 *      mlib_ImageMaxFilter5x5
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMaxFilter.h>

/* *********************************************************** */

#define	STYPE	mlib_s32
#define	TTYPE	mlib_s32

#define	C_COMP(a, b)	((a > b) ? a : b)

/* *********************************************************** */

#define	NEXT_LINE(prowx)                                        \
	a3 = sl[1];                                             \
	a4 = sl[2];                                             \
	a5 = sl[3];                                             \
	x1 = C_COMP(a3, a4);                                    \
	for (i = 0; i <= w - 2; i += 2) {                       \
	    x0 = x1;                                            \
	    a6 = sl[i + 4];                                     \
	    x1 = C_COMP(a5, a6);                                \
	    xx = C_COMP(x0, x1);                                \
	    a0 = sl[i];                                         \
	    prowx[i] = C_COMP(xx, a0);                          \
	    a5 = sl[i + 5];                                     \
	    prowx[i + 1] = C_COMP(xx, a5);                      \
	}                                                       \
	if (i < w) {                                            \
	    x0 = x1;                                            \
	    a6 = sl[i + 4];                                     \
	    x1 = C_COMP(a5, a6);                                \
	    xx = C_COMP(x0, x1);                                \
	    a0 = sl[i];                                         \
	    prowx[i] = C_COMP(xx, a0);                          \
	}                                                       \
	                                                        \
	sl += slb

/* *********************************************************** */

mlib_status
mlib_ImageMaxFilter5x5_S32(
    STYPE *dst,
    const STYPE *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_s32 i, j, k, row = 0, w2;
	STYPE *sl, *dl;
	TTYPE *rowbf, *prow[12], *temp, *prow0, *prow1, *prow2, *prow3, *prow4,
	    *prow5;
	TTYPE a0, a3, a4, a5, a6, x0, x1, xx;

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);
	sl = (void *)src;
	dl = dst + 2 * dlb + 2;
	w -= 4;
	h -= 4;

	if (w <= 0 || h <= 0)
		return (MLIB_SUCCESS);

	w2 = (w + 1) & ~1;
	rowbf = __mlib_malloc(7 * sizeof (TTYPE) * w2);

	if (rowbf == NULL)
		return (MLIB_FAILURE);

	for (k = 0; k < 6; k++) {
		prow[k] = prow[k + 6] = rowbf + k * w2;
	}

	temp = rowbf + 6 * w2;

	for (j = 0; j < 4; j++) {
		prow0 = prow[j];
		NEXT_LINE(prow0);
	}

	prow1 = prow[1];
	prow2 = prow[2];
	for (i = 0; i < w; i++) {
		temp[i] = C_COMP(prow1[i], prow2[i]);
	}

	for (j = 0; j <= h - 2; j += 2) {
		prow0 = prow[row];
		prow3 = prow[row + 3];
		prow4 = prow[row + 4];
		prow5 = prow[row + 5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		NEXT_LINE(prow4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		NEXT_LINE(prow5);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			a0 = prow0[i];
			a3 = prow3[i];
			a4 = prow4[i];
			a5 = prow5[i];
			x0 = temp[i];
			x1 = C_COMP(a3, a4);
			xx = C_COMP(x0, x1);
			dl[i] = (STYPE) C_COMP(a0, xx);
			dl[i + dlb] = (STYPE) C_COMP(xx, a5);
			temp[i] = x1;
		}

		dl += 2 * dlb;
		row += 2;

		if (row >= 6)
			row = 0;
	}

/* last line */

	if (j < h) {
		prow0 = prow[row];
		prow3 = prow[row + 3];
		prow4 = prow[row + 4];

		NEXT_LINE(prow4);

		for (i = 0; i < w; i++) {
			a0 = prow0[i];
			a3 = prow3[i];
			a4 = prow4[i];
			x0 = temp[i];
			x1 = C_COMP(a3, a4);
			xx = C_COMP(x0, x1);
			dl[i] = (STYPE) C_COMP(a0, xx);
		}
	}

	__mlib_free(rowbf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
