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

#pragma ident	"@(#)mlib_ImageMaxFilter7x7_D64.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMaxFilter7x7_D64 - internal function for
 *      mlib_ImageMaxFilter7x7
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMaxFilter.h>

/* *********************************************************** */

#define	STYPE	mlib_d64
#define	TTYPE	mlib_d64

#define	C_COMP(a, b)	((a > b) ? a : b)

/* *********************************************************** */

#define	NEXT_LINE(prowx)                                        \
	a3 = sl[1];                                             \
	a4 = sl[2];                                             \
	a5 = sl[3];                                             \
	a6 = sl[4];                                             \
	a7 = sl[5];                                             \
	a8 = sl[6];                                             \
	x1 = C_COMP(a3, a4);                                    \
	x2 = C_COMP(a5, a6);                                    \
	for (i = 0; i <= w - 2; i += 2) {                       \
	    x0 = x1;                                            \
	    x1 = x2;                                            \
	    a8 = sl[i + 6];                                     \
	    x2 = C_COMP(a7, a8);                                \
	    xx = C_COMP(x0, x1);                                \
	    xx = C_COMP(xx, x2);                                \
	    a0 = sl[i];                                         \
	    prowx[i] = C_COMP(a0, xx);                          \
	    a7 = sl[i + 7];                                     \
	    prowx[i + 1] = C_COMP(xx, a7);                      \
	}                                                       \
	if (i < w) {                                            \
	    x0 = x1;                                            \
	    x1 = x2;                                            \
	    a8 = sl[i + 6];                                     \
	    x2 = C_COMP(a7, a8);                                \
	    xx = C_COMP(x0, x1);                                \
	    xx = C_COMP(xx, x2);                                \
	    a0 = sl[i];                                         \
	    prowx[i] = C_COMP(a0, xx);                          \
	}                                                       \
	                                                        \
	sl += slb

/* *********************************************************** */

#define	KSIZE	7

mlib_status
mlib_ImageMaxFilter7x7_D64(
    STYPE *dst,
    const STYPE *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_s32 i, j, k, row = 0, w2;
	STYPE *sl, *dl;
	TTYPE *rowbf, *prow[2 * (KSIZE + 1)], **prowc;
	TTYPE *prow0, *prow1, *prow2, *prow3, *prow4, *prow5, *prow6, *prow7;
	TTYPE a0, a3, a4, a5, a6, a7, a8, x0, x1, x2, xx;

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);
	sl = (void *)src;
	dl = dst + (KSIZE / 2) * (dlb + 1);
	w -= (KSIZE - 1);
	h -= (KSIZE - 1);

	if (w <= 0 || h <= 0)
		return (MLIB_SUCCESS);

	w2 = (w + 1) & ~1;
	rowbf = __mlib_malloc((KSIZE + 1) * sizeof (TTYPE) * w2);

	if (rowbf == NULL)
		return (MLIB_FAILURE);

	for (k = 0; k < (KSIZE + 1); k++) {
		prow[k] = prow[k + (KSIZE + 1)] = rowbf + k * w2;
	}

	for (j = 0; j < (KSIZE - 1); j++) {
		prow0 = prow[j];
		NEXT_LINE(prow0);
	}

	prow1 = prow[1];
	prow2 = prow[2];
	for (i = 0; i < w; i++) {
		prow1[i] = C_COMP(prow1[i], prow2[i]);
	}

	prow3 = prow[3];
	prow4 = prow[4];
	for (i = 0; i < w; i++) {
		prow3[i] = C_COMP(prow3[i], prow4[i]);
	}

	for (j = 0; j <= h - 2; j += 2) {
		prowc = prow + row;
		prow0 = prowc[0];
		prow1 = prowc[1];
		prow3 = prowc[3];
		prow5 = prowc[5];
		prow6 = prowc[6];
		prow7 = prowc[7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		NEXT_LINE(prow6);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		NEXT_LINE(prow7);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			a0 = prow0[i];
			a5 = prow5[i];
			a6 = prow6[i];
			a7 = prow7[i];
			x0 = prow1[i];
			x1 = prow3[i];
			x2 = C_COMP(a5, a6);
			xx = C_COMP(x0, x1);
			xx = C_COMP(xx, x2);
			dl[i] = C_COMP(a0, xx);
			dl[i + dlb] = C_COMP(xx, a7);
			prow5[i] = x2;
		}

		dl += 2 * dlb;
		row += 2;

		if (row >= (KSIZE + 1))
			row = 0;
	}

/* last line */

	if (j < h) {
		prowc = prow + row;
		prow0 = prowc[0];
		prow1 = prowc[1];
		prow3 = prowc[3];
		prow5 = prowc[5];
		prow6 = prowc[6];

		NEXT_LINE(prow6);

		for (i = 0; i < w; i++) {
			a0 = prow0[i];
			a5 = prow5[i];
			a6 = prow6[i];
			x0 = prow1[i];
			x1 = prow3[i];
			x2 = C_COMP(a5, a6);
			xx = C_COMP(x0, x1);
			xx = C_COMP(xx, x2);
			dl[i] = C_COMP(a0, xx);
		}
	}

	__mlib_free(rowbf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
