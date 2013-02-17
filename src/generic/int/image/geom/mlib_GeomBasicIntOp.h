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

#ifndef _MLIB_GEOMBASICINTOP_H
#define	_MLIB_GEOMBASICINTOP_H

#pragma ident	"@(#)mlib_GeomBasicIntOp.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/*
 * *************************************************************

BASIC_DTOLL and BASIC_FTOLL are used in following files:

	mlib_i_ImageAffineTable_func.h
	mlib_i_ImageFilteredSubsampleFunc.h
	mlib_i_ImageFilteredSubsample_f.h
	mlib_i_ImageSubsampleAverage.h
	mlib_ImageAffineEdge.c
	mlib_ImageAffineTransform.c
	mlib_ImagePolynomialWarpTable_f.c
	mlib_ImagePolynomialWarp_0.c
	mlib_ImagePolynomialWarp_NN_2.c
	mlib_ImagePolynomialWarp_NN_2_Fp.c
	mlib_ImagePolynomialWarp_NN_3.c
	mlib_ImagePolynomialWarp_NN_3_Fp.c
	mlib_ImagePolynomialWarp_NN_4.c
	mlib_ImagePolynomialWarp_NN_4_Fp.c
	mlib_ImagePolynomialWarp_NN_5.c
	mlib_ImagePolynomialWarp_NN_5_Fp.c
	mlib_i_ImagePolynomialWarp_BL.c
	mlib_i_ImageZoom_BC_S32.c

**************************************************************
*/

/* *********************************************************** */
/*
	should be equivalent to :
	mlib_s64 l;
	mlib_d64 d;
	l = (mlib_s64) (d * (1 << SHIFT));
*/

#ifdef	i386

#define	BASIC_DTOLL(l, d, SHIFT)	(l) = (mlib_s64) ((d) * (1 << SHIFT));

#else /* i386 */

#define	BASIC_DTOLL(l, d, SHIFT)                            \
{                                                           \
	mlib_s64 s, m, n;                                   \
	mlib_s32 e;                                         \
                                                            \
	m = *(mlib_s64 *)&(d);                              \
	s = m >> 63;                                        \
	e = 1023 + 52 - ((m >> 52) & 0x7FF) - SHIFT;        \
	if (e > 63) e = 63;                                 \
	n = (m & 0xFFFFFFFFFFFFF) | 0x10000000000000;       \
	n = n >> e;                                         \
	n = ((s & ((-n) ^ n)) ^ n);                         \
	(l) = (mlib_s64)n;                                  \
}

#endif /* i386 */

/* *********************************************************** */
/*
	should be equivalent to :
	mlib_s64 l;
	mlib_f32 f;
	l = (mlib_s64) (f * (1 << SHIFT));
*/

#ifdef	i386

#define	BASIC_FTOLL(l, f, SHIFT)	(l) = (mlib_s64) ((f) * (1 << SHIFT));

#else /* i386 */

#define	BASIC_FTOLL(l, f, SHIFT)                            \
{                                                           \
	mlib_s64 s, m, n;                                   \
	mlib_s32 m32;                                       \
	mlib_s32 e;                                         \
                                                            \
	m32 = *(mlib_s32 *)&(f);                            \
	m = m32;                                            \
	s = m << 32 >> 63;                                  \
	e = ((m >> 23) & 0xFF) - 127 - 23 + SHIFT;          \
	n = (m & 0x7FFFFF) | 0x800000;                      \
	if (e > 0) {                                        \
		n = n << e;                                 \
	} else {                                            \
		n = n >> (-e);                              \
	}                                                   \
	n = ((s & ((-n) ^ n)) ^ n);                         \
	(l) = (mlib_s64)n;                                  \
}

#endif /* i386 */

/* *********************************************************** */
/* *********************************************************** */
/*
	MUL255 should be equivalent to : (255 * x)
	used in following files:

	mlib_ImageZoomBlendEdge.c
	mlib_i_ImageZoomBlend_BC.c
	mlib_i_ImageZoomBlend_BL.c
	mlib_i_ImageZoomBlend_NN.c
	mlib_i_ImageZoomTransTableBlend_f.h
	mlib_i_ImageZoom_BC.h

*/

#define	MUL255(x)	(((x) << 8) - (x))

/* *********************************************************** */
/* *********************************************************** */
/*
	DIV255 and DIV65025  are used in following files:

	mlib_ImageZoomBlendEdge.c
	mlib_ImageZoomEdge.c
	mlib_ImageZoomTransTableBlEdge.c
	mlib_i_ImageZoomBlend_BC.c
	mlib_i_ImageZoomBlend_BL.c
	mlib_i_ImageZoomBlend_NN.c
	mlib_i_ImageZoomTransTableBlend_f.h
	mlib_i_ImageZoom_BC.h

*/
/* *********************************************************** */
/*
	DIV255(s, k) should be equivalent to :
	mlib_s64 k, s;
	s = k / 255;
*/

#ifdef	i386

#define	DIV255(s, k)	s = k / 255;

#else /* i386 */

#define	DIV255(s, k)                                            \
{                                                               \
	mlib_s64 n, m, x, div;                                  \
                                                                \
	m = (k);                                                \
	x = m >= 0 ? m : -m;                                    \
	div = 0;                                                \
	do {                                                    \
		n = x >> 8;                                     \
		div += n;                                       \
		x = x ^ (n << 8); /* same as x = x - n * 256 */ \
		x += n;                                         \
	} while (x > 255);                                      \
	if (x == 255) div ++;                                   \
	(s) = m >= 0 ? div : -div;                              \
}

#endif /* i386 */

/* *********************************************************** */
/*
	DIV65025(s, k) should be equivalent to :
	mlib_s64 s, k;
	s = k / 65025 = k / (255*255);
*/

#ifdef	i386

#define	DIV65025(s, k)	s = k / 65025;

#else /* i386 */

#define	DIV65025(s, k)                                                       \
{                                                                            \
	mlib_s64 n, m, x, div;                                               \
                                                                             \
	m = (k);                                                             \
	x = m >= 0 ? m : -m;                                                 \
	div = 0;                                                             \
	do {                                                                 \
		n = x >> 16;                                                 \
		div += n;                                                    \
		/* same as x = x - n * 65025 = x - n * (65536 - 512 + 1) */  \
		x = x - (n << 16) + (n << 9) - n;                            \
	} while (x > 65535);                                                 \
	if (x >= 65025) div ++;                                              \
	(s) = m >= 0 ? div : -div;                                           \
}

#endif /* i386 */

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GEOMBASICINTOP_H */
