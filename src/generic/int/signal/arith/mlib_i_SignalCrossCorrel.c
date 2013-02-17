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

#pragma ident	"@(#)mlib_i_SignalCrossCorrel.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_SignalCrossCorrel - calculates cross correlation
 *                               for the input signals
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalCrossCorrel_S16(mlib_d64       *correl,
 *                                             const mlib_s16 *src1,
 *                                             const mlib_s16 *src2,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalCrossCorrel_S16S(mlib_d64       *correl,
 *                                              const mlib_s16 *src1,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       n);
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      src1     the first input signal array
 *      src2     the second input signal array
 *      n        number of samples in the signal arrays
 *
 * DESCRIPTION
 *
 *           1    N-1
 *      c = --- * SUM (x[n] * y[n])
 *           N    n = 0
 */

#include <mlib_signal.h>
#include <mlib_SignalCorrel.h>

/* *********************************************************** */

#define	E	22	/* Enlarge Multiple for _S16 */
#define	ES	22	/* Enlarge Multiple for _S16S */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalCrossCorrel_S16 = __mlib_SignalCrossCorrel_S16
#pragma weak mlib_SignalCrossCorrel_S16S = __mlib_SignalCrossCorrel_S16S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalCrossCorrel_S16) mlib_SignalCrossCorrel_S16
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16")));
__typeof__(__mlib_SignalCrossCorrel_S16S) mlib_SignalCrossCorrel_S16S
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* Constants for IEEE754 floating point */

/* Exponent bits number for double precision */
#define	MLIB_D64_EXP	11

/* Significand bits number for double precision */
#define	MLIB_D64_FRAC	52

/* Bias for double precision */
#define	MLIB_D64_BIAS	1023

/* *********************************************************** */

/*
 * MACRO
 *	MLIB_ABS_S64(Q) - ABS macro for 64-bit signed integer
 *
 * SYNOPSIS
 *	MLIB_ABS_S64(Q)
 *
 * ARGUMENTS
 *	Input
 *		Q - 64-bit signed integer
 *	Ouput
 *		ABS value of Q
 */
#define	MLIB_ABS_S64(Q)	(Q ^ (Q >> 63)) - (Q >> 63)

/* *********************************************************** */
/*
 * MACRO
 *	MLIB_LEADZERO_S64(X, N) - Leading zero count for 64-bit
 *	                          signed integer
 *
 * SYNOPSIS
 *	MLIB_LEADZERO_S64(X, N)
 *
 * ARGUMENTS
 *	Input
 *		X - 64-bit signed integer
 *	Output
 *		N - leading zero bits number, saved as a 16-bit
 *		    signed integer
 */
#define	MLIB_LEADZERO_S64(X, N) {     \
	mlib_s64 m, x = X;            \
	mlib_s16 d, n = 0;            \
		                      \
	d = ~(x >> 63);               \
		                      \
	m = (-(x >> 31)) >> 33;       \
	m &= 32;                      \
	x >>= m;                      \
	n |= m;                       \
	                              \
	m = (-(x >> 15)) >> 17;       \
	m &= 16;                      \
	x >>= m;                      \
	n |= m;                       \
	                              \
	m = (-(x >> 7)) >> 9;         \
	m &= 8;                       \
	x >>= m;                      \
	n |= m;                       \
	                              \
	m = (-(x >> 3)) >> 5;         \
	m &= 4;                       \
	x >>= m;                      \
	n |= m;                       \
                                      \
	m = (-(x >> 1)) >> 3;         \
	m &= 2;                       \
	x >>= m;                      \
	n |= m;                       \
                                      \
	m = (-x) >> 2;                \
	m &= 1;                       \
	n |= m;                       \
                                      \
	n = 63 ^ n;                   \
	n++;                          \
	N = d & n;                    \
}

/* *********************************************************** */

/*
 * MACRO
 *	MLIB_INTEGER_TO_DOUBLE_DIV - After integer division, convert 64-bit
 *	                             signed scaled integer quotient and 64-bit
 *	                             unsigned integer remainder to IEEE double
 *	                             precision floating point quotient
 *
 * SYNOPSIS
 *	MLIB_INTEGER_TO_DOUBLE_DIV(Q, P, S, REM, DIV)
 *
 * ARGUMENTS
 * 	Input
 * 		P - 64-bit signed integer
 * 		S - 16-bit signed integer scaling factor
 * 		REM - remainder, 64-bit unsigned integer
 * 		DIV - divisor, 32-bit signed integer
 * 	Output
 * 		Q - IEEE double precision floating point representation
 *		in a 64-bit signed integer
 *
 * RESTRICTIONS
 *	-127 < S < 127
 *	DIV != 0
 *
 * DESCRIPTION
 *	MLIB_INTEGER_TO_DOUBLE_DIV converts 64-bit signed
 *	integer quotient(P * 2^S) and 64-bit unsigned integer
 *	remainder(REM) to IEEE double precision floating point
 *	representation quotient and save it in Q.
 *
 *	The exponent field of Q (bits 52-62) is set to following
 *
 *		(64 - LZ - 1) + 1023 - S
 *
 *	The fraction field of Q (bits 0-51) is set to following
 *
 *		(ABS(P) >> (11 - LZ)) | LS    if (LZ < 12)
 *	or
 *		(ABS(P) << (LZ - 11)) | LS    if (LZ >= 12)
 *
 *	    LS = (((REM << (LZ - 10)) / DIV) + 1) >> 1;
 *	where LZ is the number of leading zero bits in P. LS is
 *	the remainder part will be attached to tail of fraction.
 */
#define	MLIB_INTEGER_TO_DOUBLE_DIV(Q, P, S, REM, DIV)   \
{                                                       \
/* exponent */                                          \
	mlib_s64 e;                                     \
/* fraction */                                          \
	mlib_s64 s;                                     \
/* left remainder fraction part */                      \
	mlib_s64 ls;                                    \
	mlib_s64 r, mask;                               \
/* leading zero number */                               \
	mlib_s16 lz;                                    \
	mlib_s16 a;                                     \
		                                        \
/* abs of P */	                                        \
	mask = P >> 63;                                 \
	r = (P ^ mask) - mask;                          \
	                                                \
	MLIB_LEADZERO_S64(r, lz)                        \
                                                        \
	e = (63 - lz) + MLIB_D64_BIAS - S;              \
                                                        \
	a = ((lz - MLIB_D64_EXP - 1) >> 15);            \
	s = (a & (r >> (MLIB_D64_EXP - lz))) |          \
	    ((~a) & (r << (lz - MLIB_D64_EXP)));        \
                                                        \
	ls = (((REM << (lz - 10)) / DIV) + 1) >> 1;     \
	s = s + ls;                                     \
	Q = ((lz - 64) >> 16) &                         \
	    ((s & 0xfffffffffffff) |                    \
	    ((e & 0x7ff) << MLIB_D64_FRAC) |            \
	    (((-lz) >> 16) & P & 0x8000000000000000));  \
}

/* *********************************************************** */

#if ! defined(_NO_LONGLONG)

#define	CORREL_S16                               \
	mlib_s64 *rez = (mlib_s64 *)drez;        \
	mlib_s64 ret, ret1, retmp, mod;          \
	retmp = ((rez[0] + rez[1] +              \
	    rez[2] + rez[3]) << E);              \
	ret1 = retmp / n;                        \
	mod = MLIB_ABS_S64(retmp - (ret1 * n));  \
	MLIB_INTEGER_TO_DOUBLE_DIV(ret,          \
	    ret1, E - 30, mod, n);               \
	*((mlib_s64 *)&correl[0]) = ret

#define	CORREL_S16S                                \
	mlib_s64 *rez = (mlib_s64 *)drez;          \
	mlib_s64 rets1, ret1, rets2, ret2;         \
	mlib_s64 retmp1, retmp2;                   \
	mlib_s64 mod1, mod2;                       \
	retmp1 = ((rez[0] + rez[2]) << ES);        \
	rets1 = retmp1 / n;                        \
	mod1 = MLIB_ABS_S64(retmp1 - (rets1 * n)); \
	MLIB_INTEGER_TO_DOUBLE_DIV(ret1,           \
	    rets1, ES - 30, mod1, n);              \
	*((mlib_s64 *)&correl[0]) = ret1;          \
	                                           \
	retmp2 = ((rez[1] + rez[3]) << ES);        \
	rets2 = retmp2 / n;                        \
	mod2 = MLIB_ABS_S64(retmp2 - (rets2 * n)); \
	MLIB_INTEGER_TO_DOUBLE_DIV(ret2,           \
	    rets2, ES - 30, mod2, n);              \
	*((mlib_s64 *)&correl[1]) = ret2

#else /* ! defined(_NO_LONGLONG */

#define	CORREL_S16                               \
	correl[0] = (drez[0] + drez[1] +         \
	    drez[2] + drez[3]) /                 \
	    (n * (mlib_d64)(1 << 30))

#define	CORREL_S16S                              \
	correl[0] = (drez[0] + drez[2]) /        \
	    (n * (mlib_d64)(1 << 30));           \
	correl[1] = (drez[1] + drez[3]) /        \
	    (n * (mlib_d64)(1 << 30))

#endif /* ! defined(_NO_LONGLONG */

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 drez[4];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, drez, n);

#if ! defined(_NO_LONGLONG)
	/* double2int */
	*((mlib_s64 *)&drez[0]) = (mlib_s64)drez[0];
	*((mlib_s64 *)&drez[1]) = (mlib_s64)drez[1];
	*((mlib_s64 *)&drez[2]) = (mlib_s64)drez[2];
	*((mlib_s64 *)&drez[3]) = (mlib_s64)drez[3];
#endif

	CORREL_S16;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16S(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 drez[4];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, drez, n + n);

#if ! defined(_NO_LONGLONG)
	/* double2int */
	*((mlib_s64 *)&drez[0]) = (mlib_s64)drez[0];
	*((mlib_s64 *)&drez[1]) = (mlib_s64)drez[1];
	*((mlib_s64 *)&drez[2]) = (mlib_s64)drez[2];
	*((mlib_s64 *)&drez[3]) = (mlib_s64)drez[3];
#endif

	CORREL_S16S;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if ! defined(_NO_LONGLONG)

#define	MUL_S16(x, y)	((x) * (mlib_s32) (y));

#else

#define	MUL_S16(x, y)	((x) * (mlib_d64) (y))

#endif

/* *********************************************************** */

#if ! defined(_NO_LONGLONG)

#define	VAR_DEF                             \
/* pointer to the intermediate variable */  \
	mlib_s64 *prez = (mlib_s64 *)rez;   \
/* accumulators */                          \
	mlib_s64 c1, c2, c3, c4

#else

#define	VAR_DEF                             \
/* pointer to the intermediate variable */  \
	mlib_d64 *prez = rez;               \
/* accumulators */                          \
	mlib_d64 c1, c2, c3, c4

#endif

/* *********************************************************** */

void
mlib_SignalCrossCorrel_Intr_S16(
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_d64 *rez,
    mlib_s32 n)
{
/* intermediate variable and accumulators */
	VAR_DEF;
/* pointer to the data of first signal array */
	const mlib_s16 *psrc1 = src1;

/* pointer to the data of second signal array */
	const mlib_s16 *psrc2 = src2;

/* number of samples */
	mlib_s32 size = n;

/* indices */
	mlib_s32 j, k;

	c1 = c2 = c3 = c4 = 0;
	prez[0] = prez[1] = prez[2] = 0;
	for (j = 0; j <= (size - 4); j += 4) {
		c1 += MUL_S16(psrc1[j], psrc2[j]);
		c2 += MUL_S16(psrc1[j + 1], psrc2[j + 1]);
		c3 += MUL_S16(psrc1[j + 2], psrc2[j + 2]);
		c4 += MUL_S16(psrc1[j + 3], psrc2[j + 3]);
	}

	for (k = 0; j < size; j++, k++)
		prez[k] += MUL_S16(psrc1[j], psrc2[j]);
	prez[0] += c1;
	prez[1] += c2;
	prez[2] += c3;
	prez[3] = c4;

#if ! defined(_NO_LONGLONG)
	/* int2double */
	rez[0] = (mlib_d64)prez[0];
	rez[1] = (mlib_d64)prez[1];
	rez[2] = (mlib_d64)prez[2];
	rez[3] = (mlib_d64)prez[3];
#endif
}

/* *********************************************************** */
