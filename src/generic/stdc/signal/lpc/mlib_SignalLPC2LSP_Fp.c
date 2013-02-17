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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_SignalLPC2LSP_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLPC2LSP_F32   - convert linear prediction coefficients
 *                                 to line spectral pair coefficients
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPC2LSP_F32(mlib_f32       *lsp,
 *                                         const mlib_f32 *lpc,
 *                                         mlib_s32       order);
 * ARGUMENTS
 *      lsp     the line spectral pair coefficients.
 *      lpc     the linear prediction coefficients.
 *      order   the order of the linear prediction filter.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if function completed successfully.
 *      MLIB_FAILURE is returned if error is encountered.
 *
 * DESCRIPTION
 *      The line spectral pair (LPS) coefficients are defined as the
 *      roots of the following two polynomials
 *
 *                             -(M+1)     -1
 *              P(z) = A(z) + z      * A(z  )
 *
 *                             -(M+1)     -1
 *              Q(z) = A(z) - z      * A(z  )
 *
 *      where A(z) is the inverse filter
 *
 *                         M          -i
 *              A(z) = 1- SUM a(i) * z
 *                        i = 1
 *
 *      Note that since P(z) is symmetric and Q(z) is antisymmetric
 *      all roots of these polynomials are on the unit circle and
 *      they alternate each other. In the case of even order
 *      P(z) has a root at z = -1 (w = PI) and Q(z) has a root at
 *      z = 1 (w = 0). In the case of odd order Q(z) has both
 *      (z = 1, z = -1) roots.
 *
 *      The line spectral frequency (LPF) are the angular frequency
 *      of the line spectral pair (LPS) coefficients.
 *
 *              q = cos(w)
 *
 *      where q is the LPS and w is the LPF.
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 *
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while lpc[0] element is not initialized and
 *      not used.
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPC2LSP_F32 = __mlib_SignalLPC2LSP_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPC2LSP_F32) mlib_SignalLPC2LSP_F32
    __attribute__((weak, alias("__mlib_SignalLPC2LSP_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_d64 mlib_chebps(
    mlib_d64 x,
    mlib_d64 *coeff,
    mlib_s32 n);

static mlib_d64 mlib_chebps_der(
    mlib_d64 *der_y,
    mlib_d64 x,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 n);

static mlib_status mlib_3_pow(
    mlib_d64 *root,
    mlib_d64 r,
    mlib_d64 s,
    mlib_d64 t);

static mlib_status mlib_4_pow(
    mlib_d64 *root,
    mlib_d64 b,
    mlib_d64 c,
    mlib_d64 d,
    mlib_d64 e);

static mlib_status mlib_SignalLPC2LSP_F32_5(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

static mlib_status mlib_SignalLPC2LSP_F32_6(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

static mlib_status mlib_SignalLPC2LSP_F32_7(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

static mlib_status mlib_SignalLPC2LSP_F32_8(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

static mlib_status mlib_SignalLPC2LSP_F32_9(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

static mlib_status mlib_SignalLPC2LSP_F32_10(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

static mlib_status mlib_SignalLPC2LSP_F32_COM(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order);

/* *********************************************************** */

#define	ORDER	100
#define	MIN_ITER	3
#define	MAX_ITER	4000
#define	STOP_BISECT	1.0e-7
#define	STOP_NEWTON	1.0e-4

/* *********************************************************** */

#define	SORT(x, y)	if (x < y) { ftmp = x; x = y; y = ftmp; }

/* *********************************************************** */

#define	DEF_VAR_5                                               \
	mlib_d64 x2, x4;                                        \
	mlib_d64 a0, a1, a2, a3, a4, a5;                        \
	mlib_d64 b0, b1, b2, b3, b4

/* *********************************************************** */

#define	LOAD_COEFF_5                                            \
	a0 = coeff[0 + ip];                                     \
	a1 = coeff[2 + ip];                                     \
	a2 = coeff[4 + ip];                                     \
	a3 = coeff[6 + ip];                                     \
	a4 = coeff[8 + ip];                                     \
	a5 = coeff[10 + ip]

/* *********************************************************** */

#define	LOAD_COEFF_DER_5                                        \
	b0 = der_coeff[0 + ip];                                 \
	b1 = der_coeff[2 + ip];                                 \
	b2 = der_coeff[4 + ip];                                 \
	b3 = der_coeff[6 + ip];                                 \
	b4 = der_coeff[8 + ip]

/* *********************************************************** */

#define	COUNT_POLYNOM_5(y, x)                                       \
	x2 = x * x;                                                 \
	x4 = x2 * x2;                                               \
	y = a0 + a1 * x + (a2 + a3 * x) * x2 + (a4 + a5 * x) * x4

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_5(y, der_y, x)                             \
	x2 = x * x;                                                  \
	x4 = x2 * x2;                                                \
	y = a0 + a1 * x + (a2 + a3 * x) * x2 + (a4 + a5 * x) * x4;   \
	der_y = b0 + b1 * x + (b2 + b3 * x) * x2 + b4 * x4

/* *********************************************************** */

#define	DEF_VAR_6                                               \
	mlib_d64 x2, x4;                                        \
	mlib_d64 a0, a1, a2, a3, a4, a5, a6;                    \
	mlib_d64 b0, b1, b2, b3, b4, b5

/* *********************************************************** */

#define	LOAD_COEFF_6                                            \
	a0 = coeff[0 + ip];                                     \
	a1 = coeff[2 + ip];                                     \
	a2 = coeff[4 + ip];                                     \
	a3 = coeff[6 + ip];                                     \
	a4 = coeff[8 + ip];                                     \
	a5 = coeff[10 + ip];                                    \
	a6 = coeff[12 + ip]

/* *********************************************************** */

#define	LOAD_COEFF_DER_6                                        \
	b0 = der_coeff[0 + ip];                                 \
	b1 = der_coeff[2 + ip];                                 \
	b2 = der_coeff[4 + ip];                                 \
	b3 = der_coeff[6 + ip];                                 \
	b4 = der_coeff[8 + ip];                                 \
	b5 = der_coeff[10 + ip]

/* *********************************************************** */

#define	COUNT_POLYNOM_6(y, x)                                   \
	x2 = x * x;                                             \
	x4 = x2 * x2;                                           \
	y = a0 + a1 * x + (a2 + a3 * x) * x2 + (a4 + a5 * x +   \
	    a6 * x2) * x4

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_6(y, der_y, x)                                \
	x2 = x * x;                                                     \
	x4 = x2 * x2;                                                   \
	y = a0 + a1 * x + (a2 + a3 * x) * x2 + (a4 + a5 * x +           \
	    a6 * x2) * x4;                                              \
	der_y = b0 + b1 * x + (b2 + b3 * x) * x2 + (b4 + b5 * x) * x4

/* *********************************************************** */

#define	DEF_VAR_7                                               \
	mlib_d64 x2, x3, x5;                                    \
	mlib_d64 a0, a1, a2, a3, a4, a5, a6, a7;                \
	mlib_d64 b0, b1, b2, b3, b4, b5, b6

/* *********************************************************** */

#define	LOAD_COEFF_7                                            \
	a0 = coeff[0 + ip];                                     \
	a1 = coeff[2 + ip];                                     \
	a2 = coeff[4 + ip];                                     \
	a3 = coeff[6 + ip];                                     \
	a4 = coeff[8 + ip];                                     \
	a5 = coeff[10 + ip];                                    \
	a6 = coeff[12 + ip];                                    \
	a7 = coeff[14 + ip]

/* *********************************************************** */

#define	LOAD_COEFF_DER_7                                        \
	b0 = der_coeff[0 + ip];                                 \
	b1 = der_coeff[2 + ip];                                 \
	b2 = der_coeff[4 + ip];                                 \
	b3 = der_coeff[6 + ip];                                 \
	b4 = der_coeff[8 + ip];                                 \
	b5 = der_coeff[10 + ip];                                \
	b6 = der_coeff[12 + ip]

/* *********************************************************** */

#define	COUNT_POLYNOM_7(y, x)                                             \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_7(y, der_y, x)                                  \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5;                                                \
	der_y =                                                           \
	    b0 + b1 * x + b2 * x2 + (b3 + b4 * x) * x3 + (b5 +            \
	    b6 * x) * x5

/* *********************************************************** */

#define	DEF_VAR_8                                               \
	mlib_d64 x2, x3, x5, x8;                                \
	mlib_d64 a0, a1, a2, a3, a4, a5, a6, a7, a8;            \
	mlib_d64 b0, b1, b2, b3, b4, b5, b6, b7

/* *********************************************************** */

#define	LOAD_COEFF_8                                            \
	a0 = coeff[0 + ip];                                     \
	a1 = coeff[2 + ip];                                     \
	a2 = coeff[4 + ip];                                     \
	a3 = coeff[6 + ip];                                     \
	a4 = coeff[8 + ip];                                     \
	a5 = coeff[10 + ip];                                    \
	a6 = coeff[12 + ip];                                    \
	a7 = coeff[14 + ip];                                    \
	a8 = coeff[16 + ip]

/* *********************************************************** */

#define	LOAD_COEFF_DER_8                                        \
	b0 = der_coeff[0 + ip];                                 \
	b1 = der_coeff[2 + ip];                                 \
	b2 = der_coeff[4 + ip];                                 \
	b3 = der_coeff[6 + ip];                                 \
	b4 = der_coeff[8 + ip];                                 \
	b5 = der_coeff[10 + ip];                                \
	b6 = der_coeff[12 + ip];                                \
	b7 = der_coeff[14 + ip]

/* *********************************************************** */

#define	COUNT_POLYNOM_8(y, x)                                             \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	x8 = x3 * x5;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5 + a8 * x8

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_8(y, der_y, x)                                  \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	x8 = x3 * x5;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5 + a8 * x8;                                      \
	der_y =                                                           \
	    b0 + b1 * x + b2 * x2 + (b3 + b4 * x) * x3 + (b5 + b6 * x +   \
	    b7 * x2) * x5

/* *********************************************************** */

#define	DEF_VAR_9                                               \
	mlib_d64 x2, x3, x5, x8;                                \
	mlib_d64 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;        \
	mlib_d64 b0, b1, b2, b3, b4, b5, b6, b7, b8

/* *********************************************************** */

#define	LOAD_COEFF_9                                            \
	a0 = coeff[0 + ip];                                     \
	a1 = coeff[2 + ip];                                     \
	a2 = coeff[4 + ip];                                     \
	a3 = coeff[6 + ip];                                     \
	a4 = coeff[8 + ip];                                     \
	a5 = coeff[10 + ip];                                    \
	a6 = coeff[12 + ip];                                    \
	a7 = coeff[14 + ip];                                    \
	a8 = coeff[16 + ip];                                    \
	a9 = coeff[18 + ip]

/* *********************************************************** */

#define	LOAD_COEFF_DER_9                                        \
	b0 = der_coeff[0 + ip];                                 \
	b1 = der_coeff[2 + ip];                                 \
	b2 = der_coeff[4 + ip];                                 \
	b3 = der_coeff[6 + ip];                                 \
	b4 = der_coeff[8 + ip];                                 \
	b5 = der_coeff[10 + ip];                                \
	b6 = der_coeff[12 + ip];                                \
	b7 = der_coeff[14 + ip];                                \
	b8 = der_coeff[16 + ip]

/* *********************************************************** */

#define	COUNT_POLYNOM_9(y, x)                                             \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	x8 = x3 * x5;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5 + (a8 + a9 * x) * x8

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_9(y, der_y, x)                                  \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	x8 = x3 * x5;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5 + (a8 + a9 * x) * x8;                           \
	der_y =                                                           \
	    b0 + b1 * x + b2 * x2 + (b3 + b4 * x) * x3 + (b5 + b6 * x +   \
	    b7 * x2) * x5 + b8 * x8

/* *********************************************************** */

#define	DEF_VAR_10                                              \
	mlib_d64 x2, x3, x5, x8;                                \
	mlib_d64 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;   \
	mlib_d64 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9

/* *********************************************************** */

#define	LOAD_COEFF_10                                           \
	a0 = coeff[0 + ip];                                     \
	a1 = coeff[2 + ip];                                     \
	a2 = coeff[4 + ip];                                     \
	a3 = coeff[6 + ip];                                     \
	a4 = coeff[8 + ip];                                     \
	a5 = coeff[10 + ip];                                    \
	a6 = coeff[12 + ip];                                    \
	a7 = coeff[14 + ip];                                    \
	a8 = coeff[16 + ip];                                    \
	a9 = coeff[18 + ip];                                    \
	a10 = coeff[20 + ip]

/* *********************************************************** */

#define	LOAD_COEFF_DER_10                                       \
	b0 = der_coeff[0 + ip];                                 \
	b1 = der_coeff[2 + ip];                                 \
	b2 = der_coeff[4 + ip];                                 \
	b3 = der_coeff[6 + ip];                                 \
	b4 = der_coeff[8 + ip];                                 \
	b5 = der_coeff[10 + ip];                                \
	b6 = der_coeff[12 + ip];                                \
	b7 = der_coeff[14 + ip];                                \
	b8 = der_coeff[16 + ip];                                \
	b9 = der_coeff[18 + ip]

/* *********************************************************** */

#define	COUNT_POLYNOM_10(y, x)                                            \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	x8 = x3 * x5;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5 + (a8 + a9 * x + a10 * x2) * x8

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_10(y, der_y, x)                                 \
	x2 = x * x;                                                       \
	x3 = x2 * x;                                                      \
	x5 = x2 * x3;                                                     \
	x8 = x3 * x5;                                                     \
	y = a0 + a1 * x + a2 * x2 + (a3 + a4 * x) * x3 + (a5 + a6 * x +   \
	    a7 * x2) * x5 + (a8 + a9 * x + a10 * x2) * x8;                \
	der_y =                                                           \
	    b0 + b1 * x + b2 * x2 + (b3 + b4 * x) * x3 + (b5 + b6 * x +   \
	    b7 * x2) * x5 + (b8 + b9 * x) * x8

/* *********************************************************** */

#define	DEF_VAR_COM	mlib_s32 order1 = (order+1) & ~1

/* *********************************************************** */

#define	LOAD_COEFF_COM

#define	LOAD_COEFF_DER_COM

/* *********************************************************** */

#define	COUNT_POLYNOM_COM(y, x)	y = mlib_chebps(x, coeff+ip, order1)

/* *********************************************************** */

#define	COUNT_DER_POLYNOM_COM(y, der_y, x)                               \
	y =                                                              \
	mlib_chebps_der(&der_y, x, coeff + ip, der_coeff + ip, order1)

/* *********************************************************** */

#define	MAIN_BODY(ord)                                                    \
	mlib_d64 x_l_save, delta;                                         \
	mlib_d64 y_h, y_l, ymid, der_ymid;                                \
	mlib_d64 x_l, x_h, xmid;                                          \
	mlib_d64 dx, dxold;                                               \
	mlib_s32 i, ip, n, n_max, num;                                    \
	                                                                  \
	DEF_VAR_##ord;                                                    \
	n = MIN_ITER * order;                                             \
	n_max = MAX_ITER * order;                                         \
	delta = 2.0 / n;                                                  \
	while (n <= n_max) {                                              \
	    ip = 0;                                                       \
	    LOAD_COEFF_##ord;                                             \
	    num = 0;                                                      \
	    x_l_save = 1;                                                 \
	    x_l = x_l_save;                                               \
	    COUNT_POLYNOM_##ord(y_l, x_l);                                \
/*                                                                        \
 * Roots of polynomials                                                   \
 */                                                                       \
	    for (i = 0; i <= n; i++) {                                    \
		x_h = x_l;                                                \
		y_h = y_l;                                                \
		x_l_save -= delta;                                        \
		x_l = x_l_save;                                           \
		COUNT_POLYNOM_##ord(y_l, x_l);                            \
		if (y_h * y_l <= 0) {                                     \
		    LOAD_COEFF_DER_##ord;                                 \
		    xmid = (x_h + x_l) / 2;                               \
		    dxold = (x_h - x_l);                                  \
		    dx = dxold;                                           \
		    COUNT_DER_POLYNOM_##ord(ymid, der_ymid, xmid);        \
		    while ((((xmid - x_h) * der_ymid - ymid) * ((xmid -   \
			x_l) * der_ymid - ymid) >= 0) ||                  \
			(mlib_fabs(2 * ymid) >                            \
			mlib_fabs(dxold * der_ymid))) {                   \
			dxold = dx;                                       \
			dx = 0.5 * (x_h - x_l);                           \
			if (mlib_fabs(dx) < STOP_BISECT)                  \
			    break;                                        \
			xmid = x_l + dx;                                  \
			COUNT_DER_POLYNOM_##ord(ymid, der_ymid,           \
			    xmid);                                        \
			if (y_l * ymid <= 0) {                            \
			    x_h = xmid;                                   \
			} else {                                          \
			    y_l = ymid;                                   \
			    x_l = xmid;                                   \
			}                                                 \
		    }                                                     \
		    dx = (mlib_d64)((mlib_f32)ymid /                      \
			(mlib_f32)der_ymid);                              \
		    while (mlib_fabs(dx) > STOP_NEWTON) {                 \
			xmid -= dx;                                       \
			COUNT_DER_POLYNOM_##ord(ymid, der_ymid,           \
			    xmid);                                        \
			dx = (mlib_d64)((mlib_f32)ymid /                  \
			    (mlib_f32)der_ymid);                          \
		    }                                                     \
		    xmid -= dx;                                           \
		    lsp[num] = xmid;                                      \
		    num++;                                                \
		    if (num >= order)                                     \
			return (MLIB_SUCCESS);                            \
		    x_l = xmid;                                           \
		    ip ^= 1;                                              \
		    LOAD_COEFF_##ord;                                     \
		    COUNT_POLYNOM_##ord(y_l, x_l);                        \
		}                                                         \
	    }                                                             \
	    delta *= 0.5;                                                 \
	    n *= 2;                                                       \
	}                                                                 \
	                                                                  \
	return MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_SignalLPC2LSP_F32(
    mlib_f32 *lsp,
    const mlib_f32 *lpc,
    mlib_s32 order)
{
	mlib_d64 x0, x1;
	mlib_d64 loc_tmp_buf[ORDER + 4];
	mlib_d64 loc_cheb_coeff[ORDER + 4];
	mlib_d64 loc_coeff[ORDER + 4];
	mlib_d64 loc_der_coeff[ORDER + 4];
	mlib_d64 loc_q_tmp[ORDER + 4];
	mlib_d64 *tmp_buf, *cheb_coeff;
	mlib_d64 *coeff, *der_coeff, *q_tmp;
	mlib_d64 *buf = NULL;
	mlib_d64 tmp;
	mlib_d64 a1, a2, a3, a4, a5, a6, a7, a8;
	mlib_d64 q0, q1, q2, q3, q4, q5, q6, q7;
	mlib_d64 d, d1, d2;
	mlib_d64 r, s, t, ftmp;
	mlib_d64 b, c, e;
	mlib_d64 root[4];
	mlib_s32 i, j, k, order1;
	mlib_status res;

	if (order < 1)
		return (MLIB_FAILURE);

	if (order < 9) {
		if (order == 1) {
			if (mlib_fabs(lpc[1]) >= 1)
				return (MLIB_FAILURE);
			lsp[0] = lpc[1];
			return (MLIB_SUCCESS);
		}

		if (order == 2) {
			q0 = (mlib_d64)0.5 *(
			    lpc[1] + lpc[2] + 1);
			q1 = (mlib_d64)0.5 *(
			    lpc[1] - lpc[2] - 1);

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1))
				return (MLIB_FAILURE);
			lsp[0] = q0;
			lsp[1] = q1;
			return (MLIB_SUCCESS);
		}

		if (order == 3) {
			a1 = lpc[1];
			a2 = lpc[2];
			a3 = lpc[3];
			d1 = (a1 + a3) * (a1 + a3) + 8 * (a2 + 1);

			if (d1 <= 0)
				return (MLIB_FAILURE);
			d1 = mlib_sqrt(d1);
			q0 = (mlib_d64)0.25 *((a1 + a3) + d1);
			q1 = (mlib_d64)0.5 *(a1 - a3);
			q2 = (mlib_d64)0.25 *((a1 + a3) - d1);

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1) ||
			    (mlib_fabs(q2) >= 1))
				return (MLIB_FAILURE);
			lsp[0] = q0;
			lsp[1] = q1;
			lsp[2] = q2;
			return (MLIB_SUCCESS);
		}

		if (order == 4) {
			a1 = lpc[1];
			a2 = lpc[2];
			a3 = lpc[3];
			a4 = lpc[4];
			d1 = ((a1 + a4) + 1) * ((a1 + a4) + 1) + 4 * (a2 + a3 -
			    (a1 + a4) + 1);

			if (d1 <= 0)
				return (MLIB_FAILURE);
			d2 = ((a1 - a4) - 1) * ((a1 - a4) - 1) + 4 * (a2 - a3 +
			    (a1 - a4) + 1);

			if (d2 <= 0)
				return (MLIB_FAILURE);
			d1 = mlib_sqrt(d1);
			d2 = mlib_sqrt(d2);
			q0 = (mlib_d64)0.25 * (((a1 + a4) + 1) + d1);
			q1 = (mlib_d64)0.25 * (((a1 - a4) - 1) + d2);
			q2 = (mlib_d64)0.25 * (((a1 + a4) + 1) - d1);
			q3 = (mlib_d64)0.25 * (((a1 - a4) - 1) - d2);

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1) ||
			    (mlib_fabs(q2) >= 1) || (mlib_fabs(q3) >= 1))
				return (MLIB_FAILURE);
			lsp[0] = q0;
			lsp[1] = q1;
			lsp[2] = q2;
			lsp[3] = q3;
			return (MLIB_SUCCESS);
		}

		if (order == 5) {

			a1 = lpc[1];
			a2 = lpc[2];
			a3 = lpc[3];
			a4 = lpc[4];
			a5 = lpc[5];

			d1 = (a1 - a5) * (a1 - a5) + 4 * (a2 - a4 + 1);

			if (d1 <= 0)
				return (MLIB_FAILURE);
			d1 = mlib_sqrt(d1);
			q1 = (mlib_d64)0.25 * ((a1 - a5) + d1);
			q3 = (mlib_d64)0.25 * ((a1 - a5) - d1);

			r = -(mlib_d64)0.5 * (a1 + a5);
			s = -(mlib_d64)0.25 * (a2 + a4 + 3);
			t = (mlib_d64)0.25 * ((a1 + a5) - a3);

			if (mlib_3_pow(root, r, s, t) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q0 = root[0];
			q2 = root[1];
			q4 = root[2];

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1) ||
			    (mlib_fabs(q2) >= 1) || (mlib_fabs(q3) >= 1) ||
			    (mlib_fabs(q4) >= 1))
				return (MLIB_FAILURE);

			SORT(q0, q2);
			SORT(q0, q4);
			SORT(q2, q4);

			lsp[0] = q0;
			lsp[1] = q1;
			lsp[2] = q2;
			lsp[3] = q3;
			lsp[4] = q4;
			return (MLIB_SUCCESS);
		}

		if (order == 6) {
			a1 = lpc[1];
			a2 = lpc[2];
			a3 = lpc[3];
			a4 = lpc[4];
			a5 = lpc[5];
			a6 = lpc[6];

			r = -(mlib_d64)0.5 * (a1 + a6 + 1);
			s = -(mlib_d64)0.25 * ((a2 + a5) - (a1 + a6 + 1) + 3);
			t = (mlib_d64)0.125 * ((a2 + a5) - (a3 + a4) +
			    (a1 + a6 + 1));

			if (mlib_3_pow(root, r, s, t) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q0 = root[0];
			q2 = root[1];
			q4 = root[2];

			r = -(mlib_d64)0.5 * (a1 - a6 - 1);
			s = -(mlib_d64)0.25 * ((a2 - a5) + (a1 - a6 - 1) + 3);
			t = (mlib_d64)0.125 * ((a4 - a3) - (a2 - a5) +
			    (a1 - a6 - 1));

			if (mlib_3_pow(root, r, s, t) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q1 = root[0];
			q3 = root[1];
			q5 = root[2];

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1) ||
			    (mlib_fabs(q2) >= 1) || (mlib_fabs(q3) >= 1) ||
			    (mlib_fabs(q4) >= 1) || (mlib_fabs(q5) >= 1))
				return (MLIB_FAILURE);

			SORT(q0, q2);
			SORT(q0, q4);
			SORT(q2, q4);
			SORT(q1, q3);
			SORT(q1, q5);
			SORT(q3, q5);

			lsp[0] = q0;
			lsp[1] = q1;
			lsp[2] = q2;
			lsp[3] = q3;
			lsp[4] = q4;
			lsp[5] = q5;
			return (MLIB_SUCCESS);
		}

		if (order == 7) {
			a1 = -(mlib_d64)lpc[1] - (mlib_d64)lpc[7];
			a2 = -(mlib_d64)lpc[1] + (mlib_d64)lpc[7];
			a3 = -(mlib_d64)lpc[2] - (mlib_d64)lpc[6];
			a4 = -(mlib_d64)lpc[2] + (mlib_d64)lpc[6] + 1;
			a5 = -(mlib_d64)lpc[3] - (mlib_d64)lpc[5];
			a6 = -(mlib_d64)lpc[3] + (mlib_d64)lpc[5] + a2;
			a7 = -(mlib_d64)lpc[4] - (mlib_d64)lpc[4];
			a6 *= (mlib_d64)0.5;
			a7 *= (mlib_d64)0.5;

			b = (mlib_d64)0.5 *a1;
			c = (mlib_d64)0.25 *(
			    a3 - 4);
			d = (mlib_d64)0.125 *(
			    a5 - 3 * a1);
			e = (mlib_d64)0.125 *(
			    1 - a3 + a7);

			if (mlib_4_pow(root, b, c, d, e) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q0 = root[0];
			q2 = root[1];
			q4 = root[2];
			q6 = root[3];

			r = (mlib_d64)0.5 *a2;
			s = (mlib_d64)0.25 *(
			    a4 - 3);
			t = (mlib_d64)0.25 *(
			    a6 - a2);

			if (mlib_3_pow(root, r, s, t) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q1 = root[0];
			q3 = root[1];
			q5 = root[2];

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1) ||
			    (mlib_fabs(q2) >= 1) || (mlib_fabs(q3) >= 1) ||
			    (mlib_fabs(q4) >= 1) || (mlib_fabs(q5) >= 1) ||
			    (mlib_fabs(q6) >= 1))
				return (MLIB_FAILURE);

			SORT(q0, q2);
			SORT(q4, q6);
			SORT(q2, q6);
			SORT(q0, q4);
			SORT(q2, q4);
			SORT(q1, q3);
			SORT(q1, q5);
			SORT(q3, q5);

			lsp[0] = q0;
			lsp[1] = q1;
			lsp[2] = q2;
			lsp[3] = q3;
			lsp[4] = q4;
			lsp[5] = q5;
			lsp[6] = q6;
			return (MLIB_SUCCESS);
		}

		if (order == 8) {
			a1 = -(mlib_d64)lpc[1] - (mlib_d64)lpc[8] - 1;
			a2 = -(mlib_d64)lpc[1] + (mlib_d64)lpc[8] + 1;
			a3 = -(mlib_d64)lpc[2] - (mlib_d64)lpc[7] - a1;
			a4 = -(mlib_d64)lpc[2] + (mlib_d64)lpc[7] + a2;
			a5 = -(mlib_d64)lpc[3] - (mlib_d64)lpc[6] - a3;
			a6 = -(mlib_d64)lpc[3] + (mlib_d64)lpc[6] + a4;
			a7 = -(mlib_d64)lpc[4] - (mlib_d64)lpc[5] - a5;
			a8 = -(mlib_d64)lpc[4] + (mlib_d64)lpc[5] + a6;
			a7 *= (mlib_d64)0.5;
			a8 *= (mlib_d64)0.5;

			b = (mlib_d64)0.5 *a1;
			c = (mlib_d64)0.25 *(
			    a3 - 4);
			d = (mlib_d64)0.125 *(
			    a5 - 3 * a1);
			e = (mlib_d64)0.125 *(
			    1 - a3 + a7);

			if (mlib_4_pow(root, b, c, d, e) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q0 = root[0];
			q2 = root[1];
			q4 = root[2];
			q6 = root[3];

			b = (mlib_d64)0.5 *a2;
			c = (mlib_d64)0.25 *(
			    a4 - 4);
			d = (mlib_d64)0.125 *(
			    a6 - 3 * a2);
			e = (mlib_d64)0.125 *(
			    1 - a4 + a8);

			if (mlib_4_pow(root, b, c, d, e) != MLIB_SUCCESS)
				return (MLIB_FAILURE);

			q1 = root[0];
			q3 = root[1];
			q5 = root[2];
			q7 = root[3];

			if ((mlib_fabs(q0) >= 1) || (mlib_fabs(q1) >= 1) ||
			    (mlib_fabs(q2) >= 1) || (mlib_fabs(q3) >= 1) ||
			    (mlib_fabs(q4) >= 1) || (mlib_fabs(q5) >= 1) ||
			    (mlib_fabs(q6) >= 1) || (mlib_fabs(q7) >= 1))
				return (MLIB_FAILURE);

			SORT(q0, q2);
			SORT(q4, q6);
			SORT(q2, q6);
			SORT(q0, q4);
			SORT(q2, q4);
			SORT(q1, q3);
			SORT(q5, q7);
			SORT(q3, q7);
			SORT(q1, q5);
			SORT(q3, q5);

			lsp[0] = q0;
			lsp[1] = q1;
			lsp[2] = q2;
			lsp[3] = q3;
			lsp[4] = q4;
			lsp[5] = q5;
			lsp[6] = q6;
			lsp[7] = q7;
			return (MLIB_SUCCESS);
		}
	}

	if (order > ORDER) {
		buf =
		    (mlib_d64 *)__mlib_malloc(5 * (order +
		    4) * sizeof (mlib_d64));

		if (buf == NULL)
			return (MLIB_FAILURE);

		tmp_buf = buf;
		cheb_coeff = tmp_buf + (order + 4);
		coeff = cheb_coeff + (order + 4);
		der_coeff = coeff + (order + 4);
		q_tmp = der_coeff + (order + 4);
	} else {
		tmp_buf = loc_tmp_buf;
		cheb_coeff = loc_cheb_coeff;
		coeff = loc_coeff;
		der_coeff = loc_der_coeff;
		q_tmp = loc_q_tmp;
	}

	if ((order & 1) == 0) {
		x0 = 1;
		x1 = 1;
		cheb_coeff[0] = 1;
		cheb_coeff[1] = 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 1; i <= (order >> 1); i++) {
			x0 = -(mlib_d64)lpc[i] - (mlib_d64)lpc[order - i + 1] -
			    x0;
			x1 = -(mlib_d64)lpc[i] + (mlib_d64)lpc[order - i + 1] +
			    x1;
			cheb_coeff[2 * i] = x0;
			cheb_coeff[2 * i + 1] = x1;
		}
	} else {
		cheb_coeff[0] = 1;
		cheb_coeff[1] = 0;
		cheb_coeff[2] = -(mlib_d64)lpc[1] - (mlib_d64)lpc[order];
		cheb_coeff[3] = 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 2; i <= ((order + 1) >> 1); i++) {
			cheb_coeff[2 * i] =
			    -(mlib_d64)lpc[i] - (mlib_d64)lpc[order - i + 1];
			cheb_coeff[2 * i + 1] =
			    -(mlib_d64)lpc[i - 1] + (mlib_d64)lpc[order - i +
			    2] + cheb_coeff[2 * i - 3];
		}
	}

	order1 = (order + 1) & ~1;

	if (order <= 20) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < order1 + 2; j++) {
			tmp_buf[j] = coeff[j] = 0;
		}

		coeff[0] = cheb_coeff[0];
		coeff[1] = cheb_coeff[1];

		for (j = 0; j <= order1 - 4; j += 2) {
			i = (j >> 1);

			for (k = 4 + j; k >= 2; k -= 2) {
				tmp = coeff[k];
				coeff[k] = 2 * coeff[k - 2] - tmp_buf[k];
				tmp_buf[k] = tmp;
				tmp = coeff[k + 1];
				coeff[k + 1] =
				    2 * coeff[k - 1] - tmp_buf[k + 1];
				tmp_buf[k + 1] = tmp;
			}

			tmp = coeff[0];
			coeff[0] = -tmp_buf[0] + cheb_coeff[2 * i + 2];
			tmp_buf[0] = tmp;
			tmp = coeff[1];
			coeff[1] = -tmp_buf[1] + cheb_coeff[2 * i + 3];
			tmp_buf[1] = tmp;
		}

		for (j = order1; j >= 2; j -= 2) {
			i = (j >> 1);
			coeff[j] = coeff[j - 2] - tmp_buf[j];
			coeff[j + 1] = coeff[j - 1] - tmp_buf[j + 1];

			der_coeff[j - 2] = coeff[j] * i;
			der_coeff[j - 1] = coeff[j + 1] * i;
		}

		coeff[0] = -tmp_buf[0] + 0.5 * cheb_coeff[order1];
		coeff[1] = -tmp_buf[1] + 0.5 * cheb_coeff[order1 + 1];
	} else {
		tmp = (order1 >> 1);

		der_coeff[0] = 2 * tmp * cheb_coeff[0];
		der_coeff[1] = 2 * tmp * cheb_coeff[1];
		tmp -= 1;
		der_coeff[2] = 2 * tmp * cheb_coeff[2];
		der_coeff[3] = 2 * tmp * cheb_coeff[3];
		tmp -= 1;

		for (i = 2; i < (order1 >> 1); i++) {
			der_coeff[2 * i] =
			    der_coeff[2 * i - 4] + 2 * tmp * cheb_coeff[2 * i];
			der_coeff[2 * i + 1] =
			    der_coeff[2 * i - 3] + 2 * tmp * cheb_coeff[2 * i +
			    1];
			tmp -= 1;
		}

		cheb_coeff[order1] *= 0.5;
		cheb_coeff[order1 + 1] *= 0.5;
		der_coeff[order1 - 2] *= 0.5;
		der_coeff[order1 - 1] *= 0.5;
	}

	switch ((order + 1) >> 1) {
	case 5:
		res = mlib_SignalLPC2LSP_F32_5(q_tmp, coeff, der_coeff, order);
		break;
	case 6:
		res = mlib_SignalLPC2LSP_F32_6(q_tmp, coeff, der_coeff, order);
		break;
	case 7:
		res = mlib_SignalLPC2LSP_F32_7(q_tmp, coeff, der_coeff, order);
		break;
	case 8:
		res = mlib_SignalLPC2LSP_F32_8(q_tmp, coeff, der_coeff, order);
		break;
	case 9:
		res = mlib_SignalLPC2LSP_F32_9(q_tmp, coeff, der_coeff, order);
		break;
	case 10:
		res = mlib_SignalLPC2LSP_F32_10(q_tmp, coeff, der_coeff, order);
		break;
	default:
		res =
		    mlib_SignalLPC2LSP_F32_COM(q_tmp, cheb_coeff, der_coeff,
		    order);
	}

	if (res == MLIB_SUCCESS) {
		for (i = 0; i < order; i++)
			lsp[i] = q_tmp[i];
	}

	if (buf != NULL)
		__mlib_free(buf);
	return (res);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_5(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(5);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_6(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(6);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_7(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(7);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_8(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(8);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_9(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(9);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_10(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(10);
}

/* *********************************************************** */

static mlib_status
mlib_SignalLPC2LSP_F32_COM(
    mlib_d64 *lsp,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 order)
{
	MAIN_BODY(COM);
}

/* *********************************************************** */

static mlib_d64
mlib_chebps(
    mlib_d64 x,
    mlib_d64 *coeff,
    mlib_s32 n)
{
	mlib_d64 x2, y, b0, b1, b2, a;
	mlib_s32 i;

	x2 = 2 * x;
	b0 = coeff[0];
	b1 = 0;

	a = coeff[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 2; i += 2) {
		b2 = b1;
		b1 = b0;
		b0 = a - b2 + x2 * b1;
		a = coeff[i + 4];
	}

	y = a - b1 + x * b0;
	return (y);
}

/* *********************************************************** */

mlib_d64
mlib_chebps_der(
    mlib_d64 *der_y,
    mlib_d64 x,
    mlib_d64 *coeff,
    mlib_d64 *der_coeff,
    mlib_s32 n)
{
	mlib_d64 x2, y0, b0_0, b1_0, b2_0, a0;
	mlib_d64 b0_1, b1_1, b2_1, a1;
	mlib_s32 i;

	x2 = 2 * x;
	b0_0 = coeff[0];
	b1_0 = 0;
	b0_1 = 0;
	b1_1 = 0;

	a0 = coeff[2];
	a1 = der_coeff[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 2; i += 2) {
		b2_0 = b1_0;
		b1_0 = b0_0;
		b0_0 = a0 - b2_0 + x2 * b1_0;
		a0 = coeff[i + 4];
		b2_1 = b1_1;
		b1_1 = b0_1;
		b0_1 = a1 - b2_1 + x2 * b1_1;
		a1 = der_coeff[i + 2];
	}

	y0 = a0 - b1_0 + x * b0_0;
	*der_y = a1 - b1_1 + x * b0_1;
	return (y0);
}

/* *********************************************************** */

mlib_status
mlib_3_pow(
    mlib_d64 *root,
    mlib_d64 r,
    mlib_d64 s,
    mlib_d64 t)
{
	mlib_d64 one_3, one_3_PI, two_27, sq3;
	mlib_d64 r1, r2, r3, d;
	mlib_d64 phi, cos_phi, sin_phi, p, q;

	one_3_PI = ((mlib_d64)1 / (mlib_d64)3) * M_1_PI;
	one_3 = ((mlib_d64)1 / (mlib_d64)3);
	two_27 = ((mlib_d64)2 / (mlib_d64)27);
	sq3 = 1.732050807568877293527;

	r1 = one_3 * r;
	r2 = r * r;
	p = s - one_3 * r2;
	q = two_27 * r2 * r - (one_3 * r * s) + t;

	r = one_3 * p;
	d = r * r * r + (mlib_d64)0.25 *q * q;

	if (d >= 0)
		return (MLIB_FAILURE);

	r = mlib_sqrt(-r);
	r3 = r * r * r;
	phi = one_3_PI * mlib_acos(-q / (2 * r3));

	sin_phi = mlib_sincospi(phi, &cos_phi);
	root[0] = 2 * r * cos_phi - r1;
	root[1] = r * (-cos_phi + sin_phi * sq3) - r1;
	root[2] = r * (-cos_phi - sin_phi * sq3) - r1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_4_pow(
    mlib_d64 *root,
    mlib_d64 b,
    mlib_d64 c,
    mlib_d64 d,
    mlib_d64 e)
{
	mlib_d64 b2, b3, b4;
	mlib_d64 p, q, r, d1;
	mlib_d64 q0, q1, q2;

	b2 = b * b;
	b3 = b * b2;
	b4 = b2 * b2;

	p = c - (mlib_d64)0.375 *b2;
	q = d - (mlib_d64)0.5 *c * b + (
	    mlib_d64)0.125 *b3;
	r = (mlib_d64)0.0625 *c * b2 - (
	    mlib_d64)0.01171875 *b4 - (
	    mlib_d64)0.25 *d * b + e;

	d1 = p * p - 4 * r;

	if (q == 0) {
		if (d1 <= 0)
			return (MLIB_FAILURE);
		d1 = mlib_sqrt(d1);

		q0 = (mlib_d64)0.5 *(
		    -p + d1);
		q1 = (mlib_d64)0.5 *(
		    -p - d1);

		if ((q0 <= 0) || (q1 <= 0))
			return (MLIB_FAILURE);

		root[0] = mlib_sqrt(q0);
		root[1] = mlib_sqrt(q1);
		root[2] = -mlib_sqrt(q0);
		root[3] = -mlib_sqrt(q1);

		return (MLIB_SUCCESS);
	}

	if (mlib_3_pow(root, 2 * p, d1, -q * q) != MLIB_SUCCESS)
		return (MLIB_FAILURE);

	q0 = root[0];
	q1 = root[1];
	q2 = root[2];
	b *= (mlib_d64)0.25;

	if ((q0 <= 0) || (q1 <= 0) || (q2 <= 0))
		return (MLIB_FAILURE);

	q0 = mlib_sqrt(q0);
	q1 = mlib_sqrt(q1);
	q2 = mlib_sqrt(q2);

	if (q > 0)
		q0 = -q0;

	root[0] = (mlib_d64)0.5 *(
	    q0 + q1 + q2) - b;
	root[1] = (mlib_d64)0.5 *(
	    q0 - q1 - q2) - b;
	root[2] = (mlib_d64)0.5 *(
	    q1 - q0 - q2) - b;
	root[3] = (mlib_d64)0.5 *(
	    q2 - q0 - q1) - b;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
