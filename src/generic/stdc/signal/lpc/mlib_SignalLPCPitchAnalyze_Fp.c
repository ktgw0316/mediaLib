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

#pragma ident	"@(#)mlib_SignalLPCPitchAnalyze_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLPCPitchAnalyze_F32   - open-loop pitch analyze
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPCPitchAnalyze_F32(mlib_s32       *pitch,
 *                                                 const mlib_f32 *sigwgt,
 *                                                 const mlib_s32 *region,
 *                                                 mlib_s32       length);
 * ARGUMENTS
 *      pitch   the speech pitch estimated.
 *      sigwgt  the weighted signal vector. sigwgt points to
 *              the current sample of the weighted signal vector,
 *              length samples must be available after this point,
 *              and MAX{region[i], i = 0, 1,..., 5} samples must be
 *              available before this point.
 *      region  the lower/upper boundaries of the three
 *              search regions, where region[2*i] is the
 *              lower boundary of search region i and
 *              region[2*i+1] is the upper boundary of
 *              search region i.
 *      length  the length of the signal vectors over which
 *              the correlation is calculated.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if function completed successfully.
 *      MLIB_FAILURE is returned if error is encountered.
 *
 * DESCRIPTION
 *      The open-loop pitch analysis uses perceptual weighted signal
 *      and is done with following steps.
 *
 *      In the first step. three maxima of the correlation
 *
 *                     N-1
 *              R(k) = SUM sw(j) * sw(j-k)
 *                     j = 0
 *
 *      where N = length, is located for each of the three search
 *      regions.
 *
 *      In the second step, the retained maxima R(Ti), i = 0, 1,2 are
 *      normalized as following.
 *
 *                                R(Ti)
 *              Rn(ti) = ---------------------, i = 0, 1,2
 *                             N-1         2
 *                        SQRT(SUM sw(j-Ti) )
 *                             j = 0
 *
 *      where N = length.
 *
 *      In the third step, the best open-loop delay Topt is  determined
 *      as following.
 *
 *              Topt = T0
 *
 *              if (Rn(t1) >= (0.85 * Rn(Topt))
 *                      Topt = t1
 *
 *              if (Rn(t2) >= (0.85 * Rn(Topt))
 *                      Topt = t2
 *
 *      See G.729, G.729A, GSM EFR standards.
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalLPC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPCPitchAnalyze_F32 = \
	__mlib_SignalLPCPitchAnalyze_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCPitchAnalyze_F32) mlib_SignalLPCPitchAnalyze_F32
    __attribute__((weak, alias("__mlib_SignalLPCPitchAnalyze_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STD_CORELL_SIZE	1000

/* *********************************************************** */

void
mlib_corel_10(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 offset,
    mlib_s32 length)
{
	mlib_f32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;
	mlib_f32 y0, y1, y2, y3, y4, y5, y6, y7, y8, y9;
	const mlib_f32 *tmp_ptr;
	mlib_s32 i;

	y0 = 0;
	y1 = 0;
	y2 = 0;
	y3 = 0;
	y4 = 0;
	y5 = 0;
	y6 = 0;
	y7 = 0;
	y8 = 0;
	y9 = 0;

	x0 = src[-offset];
	x1 = src[-offset + 1];
	x2 = src[-offset + 2];
	x3 = src[-offset + 3];
	x4 = src[-offset + 4];
	x5 = src[-offset + 5];
	x6 = src[-offset + 6];
	x7 = src[-offset + 7];
	x8 = src[-offset + 8];

	tmp_ptr = src - offset + 9;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		x9 = tmp_ptr[i];
		x10 = src[i];
		y0 += x10 * x0;
		y1 += x10 * x1;
		y2 += x10 * x2;
		y3 += x10 * x3;
		y4 += x10 * x4;
		y5 += x10 * x5;
		y6 += x10 * x6;
		y7 += x10 * x7;
		y8 += x10 * x8;
		y9 += x10 * x9;

		x0 = x1;
		x1 = x2;
		x2 = x3;
		x3 = x4;
		x4 = x5;
		x5 = x6;
		x6 = x7;
		x7 = x8;
		x8 = x9;
	}

	dst[0] = y0;
	dst[1] = y1;
	dst[2] = y2;
	dst[3] = y3;
	dst[4] = y4;
	dst[5] = y5;
	dst[6] = y6;
	dst[7] = y7;
	dst[8] = y8;
	dst[9] = y9;
}

/* *********************************************************** */

void
mlib_corel_4(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 offset,
    mlib_s32 length)
{
	mlib_f32 x0, x1, x2, x3, x10;
	mlib_f32 y0, y1, y2, y3;
	const mlib_f32 *tmp_ptr;
	mlib_s32 i;

	y0 = 0;
	y1 = 0;
	y2 = 0;
	y3 = 0;

	x0 = src[-offset];
	x1 = src[-offset + 1];
	x2 = src[-offset + 2];

	tmp_ptr = src - offset + 3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		x3 = tmp_ptr[i];
		x10 = src[i];
		y0 += x10 * x0;
		y1 += x10 * x1;
		y2 += x10 * x2;
		y3 += x10 * x3;

		x0 = x1;
		x1 = x2;
		x2 = x3;
	}

	dst[0] = y0;
	dst[1] = y1;
	dst[2] = y2;
	dst[3] = y3;
}

/* *********************************************************** */

void
mlib_corel_2(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 offset,
    mlib_s32 length)
{
	mlib_f32 x0, x1, x10;
	mlib_f32 y0, y1;
	const mlib_f32 *tmp_ptr;
	mlib_s32 i;

	y0 = 0;
	y1 = 0;

	x0 = src[-offset];
	tmp_ptr = src - offset + 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		x1 = tmp_ptr[i];
		x10 = src[i];
		y0 += x10 * x0;
		y1 += x10 * x1;
		x0 = x1;
	}

	dst[0] = y0;
	dst[1] = y1;
}

/* *********************************************************** */

void
__mlib_SingalLPCPitchNorm_3(
	const mlib_f32 *sigwgt0,
	const mlib_f32 *sigwgt1,
	const mlib_f32 *sigwgt2,
	mlib_f32 *res,
	mlib_s32 length)
{
	mlib_f32 r0 = 0, r1 = 0, r2 = 0;
	mlib_s32 i;
	mlib_f32 temp0, temp1, temp2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		temp0 = sigwgt0[i];
		temp1 = sigwgt1[i];
		temp2 = sigwgt2[i];
		r0 += temp0 * temp0;
		r1 += temp1 * temp1;
		r2 += temp2 * temp2;
	}
	res[0] = r0;
	res[1] = r1;
	res[2] = r2;
}

void
__mlib_SingalLPCPitchCorel(mlib_f32 *ptr_corel,
	const mlib_f32 *sigwgt,
	mlib_s32 length,
	mlib_s32 upper0,
	mlib_s32 lower0,
	mlib_s32 j0,
	mlib_s32 j1,
	mlib_f32 *max,
	mlib_s32 *pitch)
{
	mlib_s32 i, j, k;
	mlib_f32 max0, r;
	mlib_s32 t0;
	mlib_s32 range0 = upper0 - lower0;

	for (i = upper0 - 1 - j0, j = j0; j <= j1 - 10; i -= 10, j += 10) {
		mlib_corel_10(ptr_corel + j, sigwgt, i, length);
	}

	for (; j <= j1 - 4; i -= 4, j += 4) {
		mlib_corel_4(ptr_corel + j, sigwgt, i, length);
	}

	for (; j <= j1 - 2; i -= 2, j += 2) {
		mlib_corel_2(ptr_corel + j, sigwgt, i, length);
	}

	for (; j < j1; i--, j++) {
		r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < length; k++) {
			r += sigwgt[k] * sigwgt[k - i];
		}

		ptr_corel[j] = r;
	}

	if (range0 == 0) {
		max0 = MLIB_F32_MIN;
		t0 = 0;
	} else {
		max0 = ptr_corel[j1 - 1];
		t0 = j1 - 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = j1 - 2; i >= j0; i--) {
			if (ptr_corel[i] > max0) {
				max0 = ptr_corel[i];
				t0 = i;
			}
		}
		t0 = upper0 - 1 - t0;
	}
	*max = max0;
	*pitch = t0;
}

mlib_status
__mlib_SignalLPCPitchAnalyze_F32(
    mlib_s32 *pitch,
    const mlib_f32 *sigwgt,
    const mlib_s32 *region,
    mlib_s32 length)
{
	mlib_s32 i, j, k;
	mlib_f32 *ptr_corel;
	mlib_f32 arr_corell[STD_CORELL_SIZE];
	mlib_f32 max, max0, max1, max2, r, temp_f32;
	mlib_s32 t0, t1, t2, T_opt;
	mlib_s32 lower0, upper0;
	mlib_s32 lower1, upper1;
	mlib_s32 lower2, upper2;
	mlib_s32 range0, range1, range2;
	mlib_s32 max_bound;

/* Checking input data */

	if ((!sigwgt) || (!region))
		return (MLIB_FAILURE);

	lower0 = region[0];
	upper0 = region[1];
	lower1 = region[2];
	upper1 = region[3];
	lower2 = region[4];
	upper2 = region[5];

	if (lower0 > upper0)
		return (MLIB_FAILURE);

	if (lower1 > upper1)
		return (MLIB_FAILURE);

	if (lower2 > upper2)
		return (MLIB_FAILURE);

	range0 = upper0 - lower0;
	max_bound = range0;
	range1 = upper1 - lower1;

	if (range1 > max_bound)
		max_bound = range1;
	range2 = upper2 - lower2;

	if (range2 > max_bound)
		max_bound = range2;

	if (max_bound == 0)
		return (MLIB_FAILURE);

/* creating some buffers if need */

	if (max_bound < STD_CORELL_SIZE) {
		ptr_corel = arr_corell;
	} else {
		ptr_corel = __mlib_malloc((max_bound + 1) * sizeof (mlib_f32));
	}

	if (!ptr_corel) {
		return (MLIB_FAILURE);
	}

/* Computation maxima correlation in range lower0 ... upper0 */
	for (i = upper0 - 1, j = 0; i >= (lower0 + 10); i -= 10, j += 10) {
		mlib_corel_10(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= (lower0 + 4); i -= 4, j += 4) {
		mlib_corel_4(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= (lower0 + 2); i -= 2, j += 2) {
		mlib_corel_2(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= lower0; i--, j++) {
		r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < length; k++) {
			r += sigwgt[k] * sigwgt[k - i];
		}

		ptr_corel[j] = r;
	}

	if (range0 == 0) {
		max0 = MLIB_F32_MIN;
		t0 = 0;
	} else {

		max0 = ptr_corel[range0 - 1];
		t0 = range0 - 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = range0 - 2; i >= 0; i--) {
			if (ptr_corel[i] > max0) {
				max0 = ptr_corel[i];
				t0 = i;
			}
		}

		t0 = upper0 - 1 - t0;
	}

/* Computation maxima correlation in range lower1 ... upper1 */
	for (i = upper1 - 1, j = 0; i >= (lower1 + 10); i -= 10, j += 10) {
		mlib_corel_10(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= (lower1 + 4); i -= 4, j += 4) {
		mlib_corel_4(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= (lower1 + 2); i -= 2, j += 2) {
		mlib_corel_2(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= lower1; i--, j++) {
		r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < length; k++) {
			r += sigwgt[k] * sigwgt[k - i];
		}

		ptr_corel[j] = r;
	}

	if (range1 == 0) {
		max1 = MLIB_F32_MIN;
		t1 = 0;
	} else {

		max1 = ptr_corel[range1 - 1];
		t1 = range1 - 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = range1 - 2; i >= 0; i--) {
			if (ptr_corel[i] > max1) {
				max1 = ptr_corel[i];
				t1 = i;
			}
		}

		t1 = upper1 - 1 - t1;
	}

/* Computation maxima correlation in range lower2 ... upper2 */

	for (i = upper2 - 1, j = 0; i >= (lower2 + 10); i -= 10, j += 10) {
		mlib_corel_10(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= (lower2 + 4); i -= 4, j += 4) {
		mlib_corel_4(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= (lower2 + 2); i -= 2, j += 2) {
		mlib_corel_2(ptr_corel + j, sigwgt, i, length);
	}

	for (; i >= lower2; i--, j++) {
		r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < length; k++) {
			r += sigwgt[k] * sigwgt[k - i];
		}

		ptr_corel[j] = r;
	}

	if (range2 == 0) {
		max2 = MLIB_F32_MIN;
		t2 = 0;
	} else {

		max2 = ptr_corel[range2 - 1];
		t2 = range2 - 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = range2 - 2; i >= 0; i--) {
			if (ptr_corel[i] > max2) {
				max2 = ptr_corel[i];
				t2 = i;
			}
		}

		t2 = upper2 - 1 - t2;
	}

/* Normalized maxima */
	r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		temp_f32 = sigwgt[i - t0];
		r += temp_f32 * temp_f32;
	}

	if (r == 0)
		max0 = 0;
	else
		max0 = max0 / mlib_sqrtf(r);

	r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		temp_f32 = sigwgt[i - t1];
		r += temp_f32 * temp_f32;
	}

	if (r == 0)
		max1 = 0;
	else
		max1 = max1 / mlib_sqrtf(r);

	r = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		temp_f32 = sigwgt[i - t2];
		r += temp_f32 * temp_f32;
	}

	if (r == 0)
		max2 = 0;
	else
		max2 = max2 / mlib_sqrtf(r);

	T_opt = t0;
	max = max0;

	if (max1 >= 0.85f * max) {
		T_opt = t1;
	}

	if (max2 >= 0.85f * max) {
		T_opt = t2;
		max = max2;
	}

	if (max_bound >= STD_CORELL_SIZE) {
		__mlib_free(ptr_corel);
	}

	*pitch = T_opt;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
