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

#pragma ident	"@(#)mlib_SignalFIR_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_FIR_tap4f_d64
 *      mlib_FIR_tap4_d64
 *      mlib_FIR_tap3f_d64
 *      mlib_FIR_tap2f_d64
 *      mlib_FIR_tap2_d64
 *      mlib_FIR_tap1f_d64
 *      mlib_FIR_tap4f_d64s
 *      mlib_FIR_tap4_d64s
 *      mlib_FIR_tap3f_d64s
 *      mlib_FIR_tap2f_d64s
 *      mlib_FIR_tap2_d64s
 *      mlib_FIR_tap1f_d64s - Internal function
 *
 *      mlib_FIR_tap4f_s32
 *      mlib_FIR_tap4_s32
 *      mlib_FIR_tap3f_s32
 *      mlib_FIR_tap2f_s32
 *      mlib_FIR_tap2_s32
 *      mlib_FIR_tap1f_s32
 *      mlib_FIR_tap4f_s32s
 *      mlib_FIR_tap4_s32s
 *      mlib_FIR_tap3f_s32s
 *      mlib_FIR_tap2f_s32s
 *      mlib_FIR_tap2_s32s
 *      mlib_FIR_tap1f_s32s - Integer internal function
 *
 * DESCRIPTION
 *            n-1
 *     Z(n) = SUM h(k) * x(n-k)   n = 1, 2, ...
 *            k = 0
 *
 *     Z     - output signal array
 *     h(k)  - filter coefficients
 *     x(i)  - input signal array
 */

#include <mlib_signal.h>
#include <mlib_SignalFIR.h>

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

void
mlib_FIR_tap4f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1, src2, src3, src4;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2], dflt4 =
	    pflt[3];

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4 = psrc[3];

		pdst[0] =
		    dflt4 * src1 + dflt3 * src2 + dflt2 * src3 + dflt1 * src4;

		psrc++;
		pdst++;

		src1 = src2;
		src2 = src3;
		src3 = src4;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap4_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1, src2, src3, src4;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2], dflt4 =
	    pflt[3];

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4 = psrc[3];

		pdst[0] +=
		    dflt4 * src1 + dflt3 * src2 + dflt2 * src3 + dflt1 * src4;

		psrc++;
		pdst++;

		src1 = src2;
		src2 = src3;
		src3 = src4;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap3f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1, src2, src3;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2];

	src1 = psrc[0];
	src2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src3 = psrc[2];

		pdst[0] = dflt3 * src1 + dflt2 * src2 + dflt1 * src3;

		psrc++;
		pdst++;

		src1 = src2;
		src2 = src3;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1, src2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1];

	src1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2 = psrc[1];

		pdst[0] = dflt2 * src1 + dflt1 * src2;

		psrc++;
		pdst++;

		src1 = src2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1, src2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1];

	src1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2 = psrc[1];

		pdst[0] += dflt2 * src1 + dflt1 * src2;

		psrc++;
		pdst++;

		src1 = src2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap1f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 dflt1 = pflt[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		pdst[0] = dflt1 * psrc[0];
		pdst++;
		psrc++;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap4f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1_1, src2_1, src3_1, src4_1;
	mlib_d64 src1_2, src2_2, src3_2, src4_2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2], dflt4 =
	    pflt[3];

	src1_1 = psrc[0];
	src1_2 = psrc[1];
	src2_1 = psrc[2];
	src2_2 = psrc[3];
	src3_1 = psrc[4];
	src3_2 = psrc[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4_1 = psrc[6];
		src4_2 = psrc[7];

		pdst[0] =
		    dflt4 * src1_1 + dflt3 * src2_1 + dflt2 * src3_1 +
		    dflt1 * src4_1;
		pdst[1] =
		    dflt4 * src1_2 + dflt3 * src2_2 + dflt2 * src3_2 +
		    dflt1 * src4_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
		src2_1 = src3_1;
		src2_2 = src3_2;
		src3_1 = src4_1;
		src3_2 = src4_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap4_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1_1, src2_1, src3_1, src4_1;
	mlib_d64 src1_2, src2_2, src3_2, src4_2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2], dflt4 =
	    pflt[3];

	src1_1 = psrc[0];
	src1_2 = psrc[1];
	src2_1 = psrc[2];
	src2_2 = psrc[3];
	src3_1 = psrc[4];
	src3_2 = psrc[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4_1 = psrc[6];
		src4_2 = psrc[7];

		pdst[0] +=
		    dflt4 * src1_1 + dflt3 * src2_1 + dflt2 * src3_1 +
		    dflt1 * src4_1;
		pdst[1] +=
		    dflt4 * src1_2 + dflt3 * src2_2 + dflt2 * src3_2 +
		    dflt1 * src4_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
		src2_1 = src3_1;
		src2_2 = src3_2;
		src3_1 = src4_1;
		src3_2 = src4_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap3f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1_1, src2_1, src3_1;
	mlib_d64 src1_2, src2_2, src3_2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2];

	src1_1 = psrc[0];
	src1_2 = psrc[1];
	src2_1 = psrc[2];
	src2_2 = psrc[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src3_1 = psrc[4];
		src3_2 = psrc[5];

		pdst[0] = dflt3 * src1_1 + dflt2 * src2_1 + dflt1 * src3_1;
		pdst[1] = dflt3 * src1_2 + dflt2 * src2_2 + dflt1 * src3_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
		src2_1 = src3_1;
		src2_2 = src3_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1_1, src2_1;
	mlib_d64 src1_2, src2_2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1];

	src1_1 = psrc[0];
	src1_2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2_1 = psrc[2];
		src2_2 = psrc[3];

		pdst[0] = dflt2 * src1_1 + dflt1 * src2_1;
		pdst[1] = dflt2 * src1_2 + dflt1 * src2_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 src1_1, src2_1;
	mlib_d64 src1_2, src2_2;
	mlib_d64 dflt1 = pflt[0], dflt2 = pflt[1];

	src1_1 = psrc[0];
	src1_2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2_1 = psrc[2];
		src2_2 = psrc[3];

		pdst[0] += dflt2 * src1_1 + dflt1 * src2_1;
		pdst[1] += dflt2 * src1_2 + dflt1 * src2_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap1f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_d64 dflt1 = pflt[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		pdst[0] = dflt1 * psrc[0];
		pdst[1] = dflt1 * psrc[1];
		pdst += 2;
		psrc += 2;
	}
}

/* *********************************************************** */

/* Integer internal functions */

#if !defined(_NO_LONGLONG)

/*
 * MACRO
 *  MLIB_FIR_MUL32(SFLT, SRC, TDST) - Multiplication for filter coefficients
 *                                    (SFLT) and input signal array(SRC)
 *
 * SYNOPSIS
 *  MLIB_FIR_MUL32(SFLT, SRC, TDST)
 *
 * ARGUMENTS
 *  Input
 *      SELT - filter coefficients, IEEE single precision floating
 *      point representation in a 32-bit signed integer
 *
 *      SRC - input signal array, 32-bit signed integer
 *
 *  Output
 *      TDST - Multiplication for SFLT * SRC, saved as 64-bit signed
 *      integer
 */


#define	MLIB_FIR_MUL32(SFLT, SRC, TDST)		\
{						\
	mlib_s64 temp;				\
	mlib_s32 frac;				\
	mlib_s16 flt_exp;			\
	frac = SFLT & 0x7fffff | 0x800000;	\
	frac = ((SFLT >> 31) & (-frac)) |	\
	    ((~(SFLT >> 31)) & frac);		\
	temp = (mlib_s64)frac * (SRC);		\
	flt_exp = 127 - (SFLT >> 23 & 0xff);	\
	temp = ((temp + (((-flt_exp) >> 15) &	\
	    (1 << (flt_exp - 1)))) >> flt_exp);	\
	TDST = temp;				\
}

/* *********************************************************** */

void
mlib_FIR_tap4f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1, src2, src3, src4;
	mlib_s64 sum1, sum2, sum3, sum4;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1];
	mlib_s32 dflt3 = pflt[2], dflt4 = pflt[3];

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4 = psrc[3];

		MLIB_FIR_MUL32(dflt4, src1, sum1);
		MLIB_FIR_MUL32(dflt3, src2, sum2);
		MLIB_FIR_MUL32(dflt2, src3, sum3);
		MLIB_FIR_MUL32(dflt1, src4, sum4);

		pdst[0] = (sum1 + sum2 + sum3 + sum4);

		psrc++;
		pdst++;

		src1 = src2;
		src2 = src3;
		src3 = src4;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap4_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1, src2, src3, src4;
	mlib_s64 sum1, sum2, sum3, sum4;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1];
	mlib_s32 dflt3 = pflt[2], dflt4 = pflt[3];

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4 = psrc[3];

		MLIB_FIR_MUL32(dflt4, src1, sum1);
		MLIB_FIR_MUL32(dflt3, src2, sum2);
		MLIB_FIR_MUL32(dflt2, src3, sum3);
		MLIB_FIR_MUL32(dflt1, src4, sum4);

		pdst[0] += (sum1 + sum2 + sum3 + sum4);

		psrc++;
		pdst++;

		src1 = src2;
		src2 = src3;
		src3 = src4;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap3f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1, src2, src3;
	mlib_s64 sum1, sum2, sum3;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2];

	src1 = psrc[0];
	src2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src3 = psrc[2];
		MLIB_FIR_MUL32(dflt3, src1, sum1);
		MLIB_FIR_MUL32(dflt2, src2, sum2);
		MLIB_FIR_MUL32(dflt1, src3, sum3);
		pdst[0] = sum1 + sum2 + sum3;

		psrc++;
		pdst++;

		src1 = src2;
		src2 = src3;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1, src2;
	mlib_s64 sum1, sum2;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1];

	src1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2 = psrc[1];
		MLIB_FIR_MUL32(dflt2, src1, sum1);
		MLIB_FIR_MUL32(dflt1, src2, sum2);
		pdst[0] = sum1 + sum2;

		psrc++;
		pdst++;

		src1 = src2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1, src2;
	mlib_s64 sum1, sum2;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1];

	src1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2 = psrc[1];

		MLIB_FIR_MUL32(dflt2, src1, sum1);
		MLIB_FIR_MUL32(dflt1, src2, sum2);
		pdst[0] += sum1 + sum2;

		psrc++;
		pdst++;

		src1 = src2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap1f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 dflt1 = pflt[0];
	mlib_s32 src1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src1 = psrc[0];
		MLIB_FIR_MUL32(dflt1, src1, pdst[0]);
		pdst++;
		psrc++;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap4f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1_1, src2_1, src3_1, src4_1;
	mlib_s32 src1_2, src2_2, src3_2, src4_2;
	mlib_s64 sum1_1, sum2_1, sum3_1, sum4_1;
	mlib_s64 sum1_2, sum2_2, sum3_2, sum4_2;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2], dflt4 =
	    pflt[3];

	src1_1 = psrc[0];
	src1_2 = psrc[1];
	src2_1 = psrc[2];
	src2_2 = psrc[3];
	src3_1 = psrc[4];
	src3_2 = psrc[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4_1 = psrc[6];
		src4_2 = psrc[7];

		MLIB_FIR_MUL32(dflt4, src1_1, sum1_1);
		MLIB_FIR_MUL32(dflt3, src2_1, sum2_1);
		MLIB_FIR_MUL32(dflt2, src3_1, sum3_1);
		MLIB_FIR_MUL32(dflt1, src4_1, sum4_1);

		MLIB_FIR_MUL32(dflt4, src1_2, sum1_2);
		MLIB_FIR_MUL32(dflt3, src2_2, sum2_2);
		MLIB_FIR_MUL32(dflt2, src3_2, sum3_2);
		MLIB_FIR_MUL32(dflt1, src4_2, sum4_2);

		pdst[0] = sum1_1 + sum2_1 + sum3_1 + sum4_1;
		pdst[1] = sum1_2 + sum2_2 + sum3_2 + sum4_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
		src2_1 = src3_1;
		src2_2 = src3_2;
		src3_1 = src4_1;
		src3_2 = src4_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap4_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1_1, src2_1, src3_1, src4_1;
	mlib_s32 src1_2, src2_2, src3_2, src4_2;
	mlib_s64 sum1_1, sum2_1, sum3_1, sum4_1;
	mlib_s64 sum1_2, sum2_2, sum3_2, sum4_2;

	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2], dflt4 =
	    pflt[3];

	src1_1 = psrc[0];
	src1_2 = psrc[1];
	src2_1 = psrc[2];
	src2_2 = psrc[3];
	src3_1 = psrc[4];
	src3_2 = psrc[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src4_1 = psrc[6];
		src4_2 = psrc[7];

		MLIB_FIR_MUL32(dflt4, src1_1, sum1_1);
		MLIB_FIR_MUL32(dflt3, src2_1, sum2_1);
		MLIB_FIR_MUL32(dflt2, src3_1, sum3_1);
		MLIB_FIR_MUL32(dflt1, src4_1, sum4_1);

		MLIB_FIR_MUL32(dflt4, src1_2, sum1_2);
		MLIB_FIR_MUL32(dflt3, src2_2, sum2_2);
		MLIB_FIR_MUL32(dflt2, src3_2, sum3_2);
		MLIB_FIR_MUL32(dflt1, src4_2, sum4_2);

		pdst[0] += sum1_1 + sum2_1 + sum3_1 + sum4_1;
		pdst[1] += sum1_2 + sum2_2 + sum3_2 + sum4_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
		src2_1 = src3_1;
		src2_2 = src3_2;
		src3_1 = src4_1;
		src3_2 = src4_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap3f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1_1, src2_1, src3_1;
	mlib_s32 src1_2, src2_2, src3_2;
	mlib_s64 sum1_1, sum2_1, sum3_1;
	mlib_s64 sum1_2, sum2_2, sum3_2;

	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1], dflt3 = pflt[2];

	src1_1 = psrc[0];
	src1_2 = psrc[1];
	src2_1 = psrc[2];
	src2_2 = psrc[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src3_1 = psrc[4];
		src3_2 = psrc[5];

		MLIB_FIR_MUL32(dflt3, src1_1, sum1_1);
		MLIB_FIR_MUL32(dflt2, src2_1, sum2_1);
		MLIB_FIR_MUL32(dflt1, src3_1, sum3_1);
		MLIB_FIR_MUL32(dflt3, src1_2, sum1_2);
		MLIB_FIR_MUL32(dflt2, src2_2, sum2_2);
		MLIB_FIR_MUL32(dflt1, src3_2, sum3_2);

		pdst[0] = sum1_1 + sum2_1 + sum3_1;
		pdst[1] = sum1_2 + sum2_2 + sum3_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
		src2_1 = src3_1;
		src2_2 = src3_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1_1, src2_1;
	mlib_s32 src1_2, src2_2;
	mlib_s64 sum1_1, sum2_1;
	mlib_s64 sum1_2, sum2_2;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1];

	src1_1 = psrc[0];
	src1_2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2_1 = psrc[2];
		src2_2 = psrc[3];

		MLIB_FIR_MUL32(dflt2, src1_1, sum1_1);
		MLIB_FIR_MUL32(dflt1, src2_1, sum2_1);
		MLIB_FIR_MUL32(dflt2, src1_2, sum1_2);
		MLIB_FIR_MUL32(dflt1, src2_2, sum2_2);

		pdst[0] = sum1_1 + sum2_1;
		pdst[1] = sum1_2 + sum2_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap2_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 src1_1, src2_1;
	mlib_s32 src1_2, src2_2;
	mlib_s64 sum1_1, sum2_1;
	mlib_s64 sum1_2, sum2_2;
	mlib_s32 dflt1 = pflt[0], dflt2 = pflt[1];

	src1_1 = psrc[0];
	src1_2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		src2_1 = psrc[2];
		src2_2 = psrc[3];

		MLIB_FIR_MUL32(dflt2, src1_1, sum1_1);
		MLIB_FIR_MUL32(dflt1, src2_1, sum2_1);
		MLIB_FIR_MUL32(dflt2, src1_2, sum1_2);
		MLIB_FIR_MUL32(dflt1, src2_2, sum2_2);
		pdst[0] += sum1_1 + sum2_1;
		pdst[1] += sum1_2 + sum2_2;

		psrc += 2;
		pdst += 2;

		src1_1 = src2_1;
		src1_2 = src2_2;
	}
}

/* *********************************************************** */

void
mlib_FIR_tap1f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n)
{
	mlib_s32 j;
	mlib_s32 dflt1 = pflt[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		MLIB_FIR_MUL32(dflt1, psrc[0], pdst[0]);
		MLIB_FIR_MUL32(dflt1, psrc[1], pdst[1]);
		pdst += 2;
		psrc += 2;
	}
}

/* *********************************************************** */

#endif /* if !defined(_NO_LONGLONG) */

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
