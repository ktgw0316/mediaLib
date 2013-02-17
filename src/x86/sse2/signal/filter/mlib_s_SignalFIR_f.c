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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_SignalFIR_f.c	9.2	07/11/05 SMI"

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
#include <mlib_s_SignalFIR.h>
#include <mlib_sse_utils.h>

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
	__m128d sdflt1, sdflt2, sdflt3, sdflt4;
	__m128d ssrc1, ssrc2, ssrc3, ssrc4;
	__m128d smul1, smul2, smul3, smul4;

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];
	j = 0;
	if ((mlib_addr)psrc & 15) {
		src4 = psrc[3];

		pdst[0] =
		    dflt4 * src1 + dflt3 * src2 + dflt2 * src3 + dflt1 * src4;

		psrc++;
		pdst++;
		j++;

		src1 = src2;
		src2 = src3;
		src3 = src4;
	}

	sdflt4 = _mm_set1_pd(dflt4);
	sdflt3 = _mm_set1_pd(dflt3);
	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < n - 1; j += 2) {
		ssrc1 = _mm_load_pd(psrc);
		ssrc2 = _mm_loadu_pd(psrc + 1);
		ssrc3 = _mm_load_pd(psrc + 2);
		ssrc4 = _mm_loadu_pd(psrc + 3);

		smul1 = _mm_mul_pd(sdflt4, ssrc1);
		smul2 = _mm_mul_pd(sdflt3, ssrc2);
		smul3 = _mm_mul_pd(sdflt2, ssrc3);
		smul4 = _mm_mul_pd(sdflt1, ssrc4);

		smul1 = _mm_add_pd(smul1, smul2);
		smul3 = _mm_add_pd(smul3, smul4);
		smul1 = _mm_add_pd(smul1, smul3);

		_mm_storeu_pd(pdst, smul1);

		psrc += 2;
		pdst += 2;
	}

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

	for (; j < n; j++) {
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
	__m128d sdflt1, sdflt2, sdflt3, sdflt4;
	__m128d ssrc1, ssrc2, ssrc3, ssrc4;
	__m128d smul1, smul2, smul3, smul4;
	__m128d sdst;

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

	j = 0;
	if ((mlib_addr)psrc & 15) {
		src4 = psrc[3];

		pdst[0] +=
		    dflt4 * src1 + dflt3 * src2 + dflt2 * src3 + dflt1 * src4;

		psrc++;
		pdst++;
		j++;

		src1 = src2;
		src2 = src3;
		src3 = src4;
	}

	sdflt4 = _mm_set1_pd(dflt4);
	sdflt3 = _mm_set1_pd(dflt3);
	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);


#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < n - 1; j += 2) {
		ssrc1 = _mm_load_pd(psrc);
		ssrc2 = _mm_loadu_pd(psrc + 1);
		ssrc3 = _mm_load_pd(psrc + 2);
		ssrc4 = _mm_loadu_pd(psrc + 3);

		smul1 = _mm_mul_pd(sdflt4, ssrc1);
		smul2 = _mm_mul_pd(sdflt3, ssrc2);
		smul3 = _mm_mul_pd(sdflt2, ssrc3);
		smul4 = _mm_mul_pd(sdflt1, ssrc4);

		smul1 = _mm_add_pd(smul1, smul2);
		smul3 = _mm_add_pd(smul3, smul4);
		smul1 = _mm_add_pd(smul1, smul3);

		sdst = _mm_loadu_pd(pdst);
		sdst = _mm_add_pd(sdst, smul1);
		_mm_storeu_pd(pdst, sdst);

		psrc += 2;
		pdst += 2;
	}

	src1 = psrc[0];
	src2 = psrc[1];
	src3 = psrc[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < n; j++) {
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
	__m128d sdflt1, sdflt2, sdflt3;
	__m128d ssrc1, ssrc2, ssrc3;
	__m128d smul1, smul2, smul3;

	src1 = psrc[0];
	src2 = psrc[1];
	j = 0;
	if ((mlib_addr)psrc & 15) {
		src3 = psrc[2];

		pdst[0] =
		    dflt3 * src1 + dflt2 * src2 + dflt1 * src3;

		psrc++;
		pdst++;
		j++;

		src1 = src2;
		src2 = src3;
	}

	sdflt3 = _mm_set1_pd(dflt3);
	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < (n - 1); j += 2) {
		ssrc1 = _mm_load_pd(psrc);
		ssrc2 = _mm_loadu_pd(psrc + 1);
		ssrc3 = _mm_load_pd(psrc + 2);

		smul1 = _mm_mul_pd(sdflt3, ssrc1);
		smul2 = _mm_mul_pd(sdflt2, ssrc2);
		smul3 = _mm_mul_pd(sdflt1, ssrc3);

		smul1 = _mm_add_pd(smul1, smul2);
		smul1 = _mm_add_pd(smul1, smul3);

		_mm_storeu_pd(pdst, smul1);

		psrc += 2;
		pdst += 2;
	}

	src1 = psrc[0];
	src2 = psrc[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < n; j++) {
		src3 = psrc[2];

		pdst[0] =
		    dflt3 * src1 + dflt2 * src2 + dflt1 * src3;

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
	__m128d sdflt1, sdflt2;
	__m128d ssrc1, ssrc2;
	__m128d smul1, smul2;

	src1 = psrc[0];
	j = 0;
	if ((mlib_addr)psrc & 15) {
		src2 = psrc[1];

		pdst[0] = dflt2 * src1 + dflt1 * src2;

		psrc++;
		pdst++;
		j++;

		src1 = src2;
	}

	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < (n - 1); j += 2) {
		ssrc1 = _mm_load_pd(psrc);
		ssrc2 = _mm_loadu_pd(psrc + 1);

		smul1 = _mm_mul_pd(sdflt2, ssrc1);
		smul2 = _mm_mul_pd(sdflt1, ssrc2);

		smul1 = _mm_add_pd(smul1, smul2);

		_mm_storeu_pd(pdst, smul1);

		psrc += 2;
		pdst += 2;
	}

	src1 = psrc[0];

	for (; j < n; j++) {
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
	__m128d sdflt1, sdflt2;
	__m128d ssrc1, ssrc2;
	__m128d smul1, smul2;
	__m128d sdst;

	src1 = psrc[0];
	j = 0;
	if ((mlib_addr)psrc & 15) {
		src2 = psrc[1];
		pdst[0] += dflt2 * src1 + dflt1 * src2;

		psrc++;
		pdst++;
		j++;
		src1 = src2;
	}

	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < (n - 1); j += 2) {
		ssrc1 = _mm_load_pd(psrc);
		ssrc2 = _mm_loadu_pd(psrc + 1);

		smul1 = _mm_mul_pd(sdflt2, ssrc1);
		smul2 = _mm_mul_pd(sdflt1, ssrc2);

		smul1 = _mm_add_pd(smul1, smul2);

		sdst = _mm_loadu_pd(pdst);
		sdst = _mm_add_pd(sdst, smul1);
		_mm_storeu_pd(pdst, sdst);

		psrc += 2;
		pdst += 2;
	}

	src1 = psrc[0];

	for (; j < n; j++) {
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
	__m128d sdflt1;
	__m128d ssrc1;
	__m128d smul1;

	j = 0;
	if ((mlib_addr)psrc & 15) {
		pdst[0] = dflt1 * psrc[0];
		psrc++;
		pdst++;
		j++;
	}

	sdflt1 = _mm_set1_pd(dflt1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j < (n - 1); j += 2) {
		ssrc1 = _mm_load_pd(psrc);
		smul1 = _mm_mul_pd(sdflt1, ssrc1);
		_mm_storeu_pd(pdst, smul1);
		psrc += 2;
		pdst += 2;
	}

	for (; j < n; j++) {
		pdst[0] = dflt1 * psrc[0];
		psrc++;
		pdst++;
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
	__m128d sdflt1, sdflt2, sdflt3, sdflt4;
	__m128d ssrc1, ssrc2, ssrc3, ssrc4;
	__m128d smul1, smul2, smul3, smul4;

	sdflt4 = _mm_set1_pd(dflt4);
	sdflt3 = _mm_set1_pd(dflt3);
	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

	if ((mlib_addr)psrc & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_loadu_pd(psrc);
			ssrc2 = _mm_loadu_pd(psrc + 2);
			ssrc3 = _mm_loadu_pd(psrc + 4);
			ssrc4 = _mm_loadu_pd(psrc + 6);

			smul1 = _mm_mul_pd(sdflt4, ssrc1);
			smul2 = _mm_mul_pd(sdflt3, ssrc2);
			smul3 = _mm_mul_pd(sdflt2, ssrc3);
			smul4 = _mm_mul_pd(sdflt1, ssrc4);

			smul1 = _mm_add_pd(smul1, smul2);
			smul3 = _mm_add_pd(smul3, smul4);
			smul1 = _mm_add_pd(smul1, smul3);

			_mm_storeu_pd(pdst, smul1);

			psrc += 2;
			pdst += 2;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_load_pd(psrc);
			ssrc2 = _mm_load_pd(psrc + 2);
			ssrc3 = _mm_load_pd(psrc + 4);
			ssrc4 = _mm_load_pd(psrc + 6);

			smul1 = _mm_mul_pd(sdflt4, ssrc1);
			smul2 = _mm_mul_pd(sdflt3, ssrc2);
			smul3 = _mm_mul_pd(sdflt2, ssrc3);
			smul4 = _mm_mul_pd(sdflt1, ssrc4);

			smul1 = _mm_add_pd(smul1, smul2);
			smul3 = _mm_add_pd(smul3, smul4);
			smul1 = _mm_add_pd(smul1, smul3);

			_mm_storeu_pd(pdst, smul1);

			psrc += 2;
			pdst += 2;
		}
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
	__m128d sdflt1, sdflt2, sdflt3, sdflt4;
	__m128d ssrc1, ssrc2, ssrc3, ssrc4;
	__m128d smul1, smul2, smul3, smul4, sdst;

	sdflt4 = _mm_set1_pd(dflt4);
	sdflt3 = _mm_set1_pd(dflt3);
	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

	if ((mlib_addr)psrc & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_loadu_pd(psrc);
			ssrc2 = _mm_loadu_pd(psrc + 2);
			ssrc3 = _mm_loadu_pd(psrc + 4);
			ssrc4 = _mm_loadu_pd(psrc + 6);

			smul1 = _mm_mul_pd(sdflt4, ssrc1);
			smul2 = _mm_mul_pd(sdflt3, ssrc2);
			smul3 = _mm_mul_pd(sdflt2, ssrc3);
			smul4 = _mm_mul_pd(sdflt1, ssrc4);

			smul1 = _mm_add_pd(smul1, smul2);
			smul3 = _mm_add_pd(smul3, smul4);
			smul1 = _mm_add_pd(smul1, smul3);

			sdst = _mm_loadu_pd(pdst);
			sdst = _mm_add_pd(sdst, smul1);
			_mm_storeu_pd(pdst, sdst);

			psrc += 2;
			pdst += 2;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_load_pd(psrc);
			ssrc2 = _mm_load_pd(psrc + 2);
			ssrc3 = _mm_load_pd(psrc + 4);
			ssrc4 = _mm_load_pd(psrc + 6);

			smul1 = _mm_mul_pd(sdflt4, ssrc1);
			smul2 = _mm_mul_pd(sdflt3, ssrc2);
			smul3 = _mm_mul_pd(sdflt2, ssrc3);
			smul4 = _mm_mul_pd(sdflt1, ssrc4);

			smul1 = _mm_add_pd(smul1, smul2);
			smul3 = _mm_add_pd(smul3, smul4);
			smul1 = _mm_add_pd(smul1, smul3);

			sdst = _mm_loadu_pd(pdst);
			sdst = _mm_add_pd(sdst, smul1);
			_mm_storeu_pd(pdst, sdst);

			psrc += 2;
			pdst += 2;
		}
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
	__m128d sdflt1, sdflt2, ssrc1, ssrc2, smul1, smul2;

	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

	if ((mlib_addr)psrc & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_loadu_pd(psrc);
			ssrc2 = _mm_loadu_pd(psrc + 2);

			smul1 = _mm_mul_pd(sdflt2, ssrc1);
			smul2 = _mm_mul_pd(sdflt1, ssrc2);

			smul1 = _mm_add_pd(smul1, smul2);

			_mm_storeu_pd(pdst, smul1);

			psrc += 2;
			pdst += 2;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_load_pd(psrc);
			ssrc2 = _mm_load_pd(psrc + 2);

			smul1 = _mm_mul_pd(sdflt2, ssrc1);
			smul2 = _mm_mul_pd(sdflt1, ssrc2);

			smul1 = _mm_add_pd(smul1, smul2);

			_mm_storeu_pd(pdst, smul1);

			psrc += 2;
			pdst += 2;
		}
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
	__m128d sdflt1, sdflt2, ssrc1, ssrc2;
	__m128d smul1, smul2, sdst;

	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

	if ((mlib_addr)psrc & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_loadu_pd(psrc);
			ssrc2 = _mm_loadu_pd(psrc + 2);

			smul1 = _mm_mul_pd(sdflt2, ssrc1);
			smul2 = _mm_mul_pd(sdflt1, ssrc2);

			smul1 = _mm_add_pd(smul1, smul2);

			sdst = _mm_loadu_pd(pdst);
			sdst = _mm_add_pd(sdst, smul1);
			_mm_storeu_pd(pdst, sdst);

			psrc += 2;
			pdst += 2;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_load_pd(psrc);
			ssrc2 = _mm_load_pd(psrc + 2);

			smul1 = _mm_mul_pd(sdflt2, ssrc1);
			smul2 = _mm_mul_pd(sdflt1, ssrc2);

			smul1 = _mm_add_pd(smul1, smul2);

			sdst = _mm_loadu_pd(pdst);
			sdst = _mm_add_pd(sdst, smul1);
			_mm_storeu_pd(pdst, sdst);

			psrc += 2;
			pdst += 2;
		}
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

	__m128d sdflt1, sdflt2, sdflt3;
	__m128d ssrc1, ssrc2, ssrc3;
	__m128d smul1, smul2, smul3;

	sdflt3 = _mm_set1_pd(dflt3);
	sdflt2 = _mm_set1_pd(dflt2);
	sdflt1 = _mm_set1_pd(dflt1);

	if ((mlib_addr)psrc & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_loadu_pd(psrc);
			ssrc2 = _mm_loadu_pd(psrc + 2);
			ssrc3 = _mm_loadu_pd(psrc + 4);

			smul1 = _mm_mul_pd(sdflt3, ssrc1);
			smul2 = _mm_mul_pd(sdflt2, ssrc2);
			smul3 = _mm_mul_pd(sdflt1, ssrc3);

			smul1 = _mm_add_pd(smul1, smul2);
			smul1 = _mm_add_pd(smul1, smul3);
			_mm_storeu_pd(pdst, smul1);
			psrc += 2;
			pdst += 2;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
			ssrc1 = _mm_load_pd(psrc);
			ssrc2 = _mm_load_pd(psrc + 2);
			ssrc3 = _mm_load_pd(psrc + 4);

			smul1 = _mm_mul_pd(sdflt3, ssrc1);
			smul2 = _mm_mul_pd(sdflt2, ssrc2);
			smul3 = _mm_mul_pd(sdflt1, ssrc3);

			smul1 = _mm_add_pd(smul1, smul2);
			smul1 = _mm_add_pd(smul1, smul3);
			_mm_storeu_pd(pdst, smul1);

			psrc += 2;
			pdst += 2;
		}
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
	mlib_d64 src1_1, src1_2;
	mlib_d64 dflt1 = pflt[0];

	__m128d sdflt1, ssrc1, smul1;

	sdflt1 = _mm_set1_pd(dflt1);

	if ((mlib_addr)psrc & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {

			ssrc1 = _mm_loadu_pd(psrc);

			smul1 = _mm_mul_pd(sdflt1, ssrc1);

			_mm_storeu_pd(pdst, smul1);
			psrc += 2;
			pdst += 2;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			ssrc1 = _mm_load_pd(psrc);
			smul1 = _mm_mul_pd(sdflt1, ssrc1);
			_mm_storeu_pd(pdst, smul1);
			psrc += 2;
			pdst += 2;
		}
	}
}

/* *********************************************************** */
#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
