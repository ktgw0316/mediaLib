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

#pragma ident	"@(#)mlib_SignalLPCPerceptWeight_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLPCPerceptWeight_F32  - perceptual weighting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLPCPerceptWeightInit_F32(void     **state,
 *                                                      mlib_s32 length,
 *                                                      mlib_s32 order);
 *      mlib_status mlib_SignalLPCPerceptWeight_F32(mlib_f32       *sigwgt,
 *                                                  const mlib_f32 *signal,
 *                                                  const mlib_f32 *lpc,
 *                                                  mlib_f32       r1,
 *                                                  mlib_f32       r2,
 *                                                  void           *state);
 *      void mlib_SignalLPCPerceptWeightFree_F32(void *state);
 *
 * ARGUMENTS
 *      sigwgt  the weighted signal vector.
 *      signal  the input signal vector.
 *      lpc     the linear prediction coefficients.
 *      r1      the perceptual weighting filter coefficient,
 *              where 0 < r1 <= 1.
 *      r2      the perceptual weighting filter coefficient,
 *              where 0 < r2 <= 1.
 *      length  the length of the signal vectors.
 *      order   the order of the linear prediction filter.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if function completed successfully.
 *      MLIB_FAILURE is returned if error is encountered.
 *
 * RESTRICTIONS
 *      The init functions should only perform internal structure
 *      allocation and global initialization. Per LPC function call
 *      initialization should be done in LPC function, so the
 *      same internal structure can be reused for multiple LPC
 *      function calls.
 *
 * DESCRIPTION
 *      The perceptual weighting filter is defined as following.
 *
 *                      A(z*r1)
 *              W(z) = ---------
 *                      A(z*r2)
 *
 *      where A(z) is the inverse filter
 *
 *                         M          -i
 *              A(z) = 1- SUM a(i) * z
 *                        i = 1
 *
 *      See G.723.1, G.728, G.729, G.729A, GSM EFR standards.
 *
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while lpc[0] element is not initialized and
 *      not used.
 */

#include <mlib_signal.h>
#include <mlib_SignalLPCPerceptWeight.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPCPerceptWeightFree_F32 = \
	__mlib_SignalLPCPerceptWeightFree_F32
#pragma weak mlib_SignalLPCPerceptWeightInit_F32 = \
	__mlib_SignalLPCPerceptWeightInit_F32
#pragma weak mlib_SignalLPCPerceptWeight_F32 = \
	__mlib_SignalLPCPerceptWeight_F32
#pragma weak mlib_FIR_part_Main = __mlib_FIR_part_Main

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCPerceptWeightFree_F32)
    mlib_SignalLPCPerceptWeightFree_F32
    __attribute__((weak, alias("__mlib_SignalLPCPerceptWeightFree_F32")));
__typeof__(__mlib_SignalLPCPerceptWeightInit_F32)
    mlib_SignalLPCPerceptWeightInit_F32
    __attribute__((weak, alias("__mlib_SignalLPCPerceptWeightInit_F32")));
__typeof__(__mlib_SignalLPCPerceptWeight_F32) mlib_SignalLPCPerceptWeight_F32
    __attribute__((weak, alias("__mlib_SignalLPCPerceptWeight_F32")));
__typeof__(__mlib_FIR_part_Main) mlib_FIR_part_Main
    __attribute__((weak, alias("__mlib_FIR_part_Main")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SET_RANGE(beg0, end0, add) \
	beg = beg0; \
	end = end0; \
	if (beg + (add) < i0) beg = i0 - (add); \
	if (end + (add) > i1) end = i1 - (add); \
	if (beg > end) beg = end

#define	XINI(i) ((i) >= 0 ? src[(i)] : delay[-(i)])

void
__mlib_FIR_part_Main_i(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    mlib_d64 *delay,
    mlib_s32 length,
    mlib_s32 order,
    mlib_s32 i0,
    mlib_s32 i1)
{
/* Filter's coefficients */
	mlib_d64 a1, a2, a3, a4, a5;

/* Filter's coefficients */
	mlib_d64 a6, a7, a8;

/* Delayed elements */
	mlib_d64 x1, x2, x3, x4, x5;

/* Delayed elements */
	mlib_d64 x6, x7, x8;
	mlib_d64 x00, x01, x02, x03;
	mlib_d64 x04, x05, x06, x07;
	mlib_d64 y0, y1, y2, y3, y4;
	mlib_d64 y5, y6, y7, x0;
	mlib_s32 i, j, beg, end;

	for (i = 1; i <= order - 7; i += 8) {
		SET_RANGE(0, length - i + 1, i - 1);

		x1 = XINI(beg - 1);
		x2 = XINI(beg - 2);
		x3 = XINI(beg - 3);
		x4 = XINI(beg - 4);
		x5 = XINI(beg - 5);
		x6 = XINI(beg - 6);
		x7 = XINI(beg - 7);
		x8 = XINI(beg - 8);

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
		a3 = lpc[i + 2];
		a4 = lpc[i + 3];
		a5 = lpc[i + 4];
		a6 = lpc[i + 5];
		a7 = lpc[i + 6];
		a8 = lpc[i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end - 7; j += 8) {
			x00 = src[j + 0];
			x01 = src[j + 1];
			x02 = src[j + 2];
			x03 = src[j + 3];
			x04 = src[j + 4];
			x05 = src[j + 5];
			x06 = src[j + 6];
			x07 = src[j + 7];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 + a4 * x4 +
			    a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a8 * x7 + a7 * x6 + a6 * x5 + a5 * x4 + a4 * x3 +
			    a3 * x2 + a2 * x1 + a1 * x00;
			y2 = a8 * x6 + a7 * x5 + a6 * x4 + a5 * x3 + a4 * x2 +
			    a3 * x1 + a2 * x00 + a1 * x01;
			y3 = a8 * x5 + a7 * x4 + a6 * x3 + a5 * x2 + a4 * x1 +
			    a3 * x00 + a2 * x01 + a1 * x02;
			y4 = a8 * x4 + a7 * x3 + a6 * x2 + a5 * x1 + a4 * x00 +
			    a3 * x01 + a2 * x02 + a1 * x03;
			y5 = a8 * x3 + a7 * x2 + a6 * x1 + a5 * x00 + a4 * x01 +
			    a3 * x02 + a2 * x03 + a1 * x04;
			y6 = a8 * x2 + a7 * x1 + a6 * x00 + a5 * x01 +
			    a4 * x02 + a3 * x03 + a2 * x04 + a1 * x05;
			y7 = a8 * x1 + a7 * x00 + a6 * x01 + a5 * x02 +
			    a4 * x03 + a3 * x04 + a2 * x05 + a1 * x06;

			x8 = x00;
			x7 = x01;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			dst[j + i + 1] -= y2;
			dst[j + i + 2] -= y3;
			dst[j + i + 3] -= y4;
			dst[j + i + 4] -= y5;
			dst[j + i + 5] -= y6;
			dst[j + i + 6] -= y7;
		}

		if (j < end - 3) {
			x00 = src[j + 0];
			x01 = src[j + 1];
			x02 = src[j + 2];
			x03 = src[j + 3];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 +
			    a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a8 * x7 + a7 * x6 + a6 * x5 + a5 * x4 +
			    a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;
			y2 = a8 * x6 + a7 * x5 + a6 * x4 + a5 * x3 +
			    a4 * x2 + a3 * x1 + a2 * x00 + a1 * x01;
			y3 = a8 * x5 + a7 * x4 + a6 * x3 + a5 * x2 +
			    a4 * x1 + a3 * x00 + a2 * x01 + a1 * x02;

			x8 = x4;
			x7 = x3;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			dst[j + i + 1] -= y2;
			dst[j + i + 2] -= y3;
			j += 4;
		}

		if (j < end - 1) {
			x00 = src[j + 0];
			x01 = src[j + 1];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 +
			    a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a8 * x7 + a7 * x6 + a6 * x5 + a5 * x4 +
			    a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;

			x8 = x6;
			x7 = x5;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			j += 2;
		}

		if (j < end) {
			x00 = src[j + 0];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 +
			    a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;

			x8 = x7;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			dst[j + i - 1] -= y0;
		}
	}

	if (i <= order - 3) {
		SET_RANGE(0, length - i + 1, i - 1);

		x1 = XINI(beg - 1);
		x2 = XINI(beg - 2);
		x3 = XINI(beg - 3);
		x4 = XINI(beg - 4);

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
		a3 = lpc[i + 2];
		a4 = lpc[i + 3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end - 3; j += 4) {
			x00 = src[j + 0];
			x01 = src[j + 1];
			x02 = src[j + 2];
			x03 = src[j + 3];

			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;
			y2 = a4 * x2 + a3 * x1 + a2 * x00 + a1 * x01;
			y3 = a4 * x1 + a3 * x00 + a2 * x01 + a1 * x02;

			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			dst[j + i + 1] -= y2;
			dst[j + i + 2] -= y3;
		}

		if (j < end - 1) {
			x00 = src[j + 0];
			x01 = src[j + 1];

			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			j += 2;
		}

		if (j < end) {
			x00 = src[j + 0];

			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			dst[j + i - 1] -= y0;
		}

		i += 4;
	}

	if (i <= order - 1) {
		SET_RANGE(0, length - i + 1, i - 1);

		x1 = XINI(beg - 1);
		x2 = XINI(beg - 2);

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end - 1; j += 2) {
			x00 = src[j + 0];
			x01 = src[j + 1];

			y0 = a2 * x2 + a1 * x1;
			y1 = a2 * x1 + a1 * x00;
			x2 = x00;
			x1 = x01;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
		}

		if (j < end) {
			x00 = src[j + 0];

			y0 = a2 * x2 + a1 * x1;
			x2 = x1;
			x1 = x00;
			dst[j + i - 1] -= y0;
		}

		i += 2;
	}

	if (i <= order) {
		a1 = lpc[i + 0];

		SET_RANGE(0, length - i + 1, i - 1);

		x1 = XINI(beg - 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end; j++) {
			x0 = src[j];
			y0 = a1 * x1;

			x1 = x0;
			dst[j + i - 1] -= y0;
		}
	}
}

void
__mlib_FIR_part_Main(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    mlib_d64 *delay,
    mlib_s32 length,
    mlib_s32 order)
{
/* Filter's coefficients */
	mlib_d64 a1, a2, a3, a4, a5;

/* Filter's coefficients */
	mlib_d64 a6, a7, a8;

/* Delayed elements */
	mlib_d64 x1, x2, x3, x4, x5;

/* Delayed elements */
	mlib_d64 x6, x7, x8;
	mlib_d64 x00, x01, x02, x03;
	mlib_d64 x04, x05, x06, x07;
	mlib_d64 y0, y1, y2, y3, y4;
	mlib_d64 y5, y6, y7, x0;
	mlib_s32 i, j;

	for (i = 1; i <= order - 7; i += 8) {
		x1 = delay[1];
		x2 = delay[2];
		x3 = delay[3];
		x4 = delay[4];
		x5 = delay[5];
		x6 = delay[6];
		x7 = delay[7];
		x8 = delay[8];

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
		a3 = lpc[i + 2];
		a4 = lpc[i + 3];
		a5 = lpc[i + 4];
		a6 = lpc[i + 5];
		a7 = lpc[i + 6];
		a8 = lpc[i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= length - i - 7; j += 8) {
			x00 = src[j + 0];
			x01 = src[j + 1];
			x02 = src[j + 2];
			x03 = src[j + 3];
			x04 = src[j + 4];
			x05 = src[j + 5];
			x06 = src[j + 6];
			x07 = src[j + 7];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 + a4 * x4 +
			    a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a8 * x7 + a7 * x6 + a6 * x5 + a5 * x4 + a4 * x3 +
			    a3 * x2 + a2 * x1 + a1 * x00;
			y2 = a8 * x6 + a7 * x5 + a6 * x4 + a5 * x3 + a4 * x2 +
			    a3 * x1 + a2 * x00 + a1 * x01;
			y3 = a8 * x5 + a7 * x4 + a6 * x3 + a5 * x2 + a4 * x1 +
			    a3 * x00 + a2 * x01 + a1 * x02;
			y4 = a8 * x4 + a7 * x3 + a6 * x2 + a5 * x1 + a4 * x00 +
			    a3 * x01 + a2 * x02 + a1 * x03;
			y5 = a8 * x3 + a7 * x2 + a6 * x1 + a5 * x00 + a4 * x01 +
			    a3 * x02 + a2 * x03 + a1 * x04;
			y6 = a8 * x2 + a7 * x1 + a6 * x00 + a5 * x01 +
			    a4 * x02 + a3 * x03 + a2 * x04 + a1 * x05;
			y7 = a8 * x1 + a7 * x00 + a6 * x01 + a5 * x02 +
			    a4 * x03 + a3 * x04 + a2 * x05 + a1 * x06;

			x8 = x00;
			x7 = x01;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			dst[j + i + 1] -= y2;
			dst[j + i + 2] -= y3;
			dst[j + i + 3] -= y4;
			dst[j + i + 4] -= y5;
			dst[j + i + 5] -= y6;
			dst[j + i + 6] -= y7;
		}

		if (j <= length - i - 3) {
			x00 = src[j + 0];
			x01 = src[j + 1];
			x02 = src[j + 2];
			x03 = src[j + 3];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 +
			    a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a8 * x7 + a7 * x6 + a6 * x5 + a5 * x4 +
			    a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;
			y2 = a8 * x6 + a7 * x5 + a6 * x4 + a5 * x3 +
			    a4 * x2 + a3 * x1 + a2 * x00 + a1 * x01;
			y3 = a8 * x5 + a7 * x4 + a6 * x3 + a5 * x2 +
			    a4 * x1 + a3 * x00 + a2 * x01 + a1 * x02;

			x8 = x4;
			x7 = x3;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			dst[j + i + 1] -= y2;
			dst[j + i + 2] -= y3;
			j += 4;
		}

		if (j <= length - i - 1) {
			x00 = src[j + 0];
			x01 = src[j + 1];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 +
			    a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a8 * x7 + a7 * x6 + a6 * x5 + a5 * x4 +
			    a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;

			x8 = x6;
			x7 = x5;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			j += 2;
		}

		if (j <= length - i) {
			x00 = src[j + 0];

			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 +
			    a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;

			x8 = x7;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			dst[j + i - 1] -= y0;
		}
	}

	if (i <= order - 3) {
		x1 = delay[1];
		x2 = delay[2];
		x3 = delay[3];
		x4 = delay[4];

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
		a3 = lpc[i + 2];
		a4 = lpc[i + 3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= length - i - 3; j += 4) {
			x00 = src[j + 0];
			x01 = src[j + 1];
			x02 = src[j + 2];
			x03 = src[j + 3];

			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;
			y2 = a4 * x2 + a3 * x1 + a2 * x00 + a1 * x01;
			y3 = a4 * x1 + a3 * x00 + a2 * x01 + a1 * x02;

			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			dst[j + i + 1] -= y2;
			dst[j + i + 2] -= y3;
		}

		if (j <= length - i - 1) {
			x00 = src[j + 0];
			x01 = src[j + 1];

			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			y1 = a4 * x3 + a3 * x2 + a2 * x1 + a1 * x00;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
			j += 2;
		}

		if (j <= length - i) {
			x00 = src[j + 0];

			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			dst[j + i - 1] -= y0;
		}

		i += 4;
	}

	if (i <= order - 1) {
		x1 = delay[1];
		x2 = delay[2];

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= length - i - 1; j += 2) {
			x00 = src[j + 0];
			x01 = src[j + 1];

			y0 = a2 * x2 + a1 * x1;
			y1 = a2 * x1 + a1 * x00;
			x2 = x00;
			x1 = x01;
			dst[j + i - 1] -= y0;
			dst[j + i - 0] -= y1;
		}

		if (j <= length - i) {
			x00 = src[j + 0];

			y0 = a2 * x2 + a1 * x1;
			x2 = x1;
			x1 = x00;
			dst[j + i - 1] -= y0;
		}

		i += 2;
	}

	if (i <= order) {
		x1 = delay[1];
		a1 = lpc[i + 0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= length - i; j++) {
			x0 = src[j];
			y0 = a1 * x1;

			x1 = x0;
			dst[j + i - 1] -= y0;
		}
	}
}

void
mlib_FIR_part(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    filter_state * pfilter)
{
/* Filter's coefficients */
	mlib_d64 a1, a2, a3, a4, a5;

/* Filter's coefficients */
	mlib_d64 a6, a7, a8;

/* Delayed elements */
	mlib_d64 x0, x1, x2, x3, x4, x5;

/* Delayed elements */
	mlib_d64 x6, x7, x8;
	mlib_d64 y0;
	mlib_s32 i, j;
	mlib_s32 length = pfilter->length;
	mlib_s32 order = pfilter->order;
	mlib_d64 *delay = pfilter->zero;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		dst[i] = src[i];
	}

	for (i = 9; i <= order - 7; i += 8) {
		x1 = delay[i + 0];
		x2 = delay[i + 1];
		x3 = delay[i + 2];
		x4 = delay[i + 3];
		x5 = delay[i + 4];
		x6 = delay[i + 5];
		x7 = delay[i + 6];
		x8 = delay[i + 7];

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
		a3 = lpc[i + 2];
		a4 = lpc[i + 3];
		a5 = lpc[i + 4];
		a6 = lpc[i + 5];
		a7 = lpc[i + 6];
		a8 = lpc[i + 7];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i - 1; j > 0; j--) {
			x0 = delay[j];
			y0 = a8 * x8 + a7 * x7 + a6 * x6 + a5 * x5 + a4 * x4 +
			    a3 * x3 + a2 * x2 + a1 * x1;

			x8 = x7;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x0;
			dst[i - j - 1] -= y0;
		}
	}

	if (i <= order - 3) {
		x1 = delay[i + 0];
		x2 = delay[i + 1];
		x3 = delay[i + 2];
		x4 = delay[i + 3];

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
		a3 = lpc[i + 2];
		a4 = lpc[i + 3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i - 1; j > 0; j--) {
			x0 = delay[j];
			y0 = a4 * x4 + a3 * x3 + a2 * x2 + a1 * x1;

			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x0;
			dst[i - j - 1] -= y0;
		}

		i += 4;
	}

	if (i <= order - 1) {
		x1 = delay[i + 0];
		x2 = delay[i + 1];

		a1 = lpc[i + 0];
		a2 = lpc[i + 1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i - 1; j > 0; j--) {
			x0 = delay[j];
			y0 = a2 * x2 + a1 * x1;

			x2 = x1;
			x1 = x0;
			dst[i - j - 1] -= y0;
		}

		i += 2;
	}

	if (i <= order) {
		x1 = delay[i + 0];
		a1 = lpc[i + 0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i - 1; j > 0; j--) {
			x0 = delay[j];
			y0 = a1 * x1;

			x1 = x0;
			dst[i - j - 1] -= y0;
		}
	}

	mlib_FIR_part_Main(dst, src, lpc, delay, length, order);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		delay[i] = src[length - i];
	}
}

/* *********************************************************** */

void
mlib_IIR_part(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    filter_state * pfilter)
{
/* Filter's coefficients */
	mlib_d64 a1, a2, a3, a4, a5;

/* Filter's coefficients */
	mlib_d64 a6, a7, a8, a9, a10;

/* Delayed elements */
	mlib_d64 y0, y1, y2, y3, y4, y5;

/* Delayed elements */
	mlib_d64 y6, y7, y8, y9, y10;

/* Delayed elements */
	mlib_d64 y00, y01, y02, y03, y04, y05;

/* Delayed elements */
	mlib_d64 y06, y07, y08, y09;
	mlib_s32 i, j1, j2, tail;
	mlib_s32 length = pfilter->length;
	mlib_s32 order = pfilter->order;
	mlib_d64 *delay = pfilter->pole;

	tail = order / 10;
	tail = order - tail * 10;

	for (i = 0; i <= length - 10; i += 10) {
		if (tail == 9) {
			y1 = delay[order - 8];
			y2 = delay[order - 7];
			y3 = delay[order - 6];
			y4 = delay[order - 5];
			y5 = delay[order - 4];
			y6 = delay[order - 3];
			y7 = delay[order - 2];
			y8 = delay[order - 1];
			y9 = delay[order];
			y00 = delay[order - 9];
			y01 = delay[order - 10];
			y02 = delay[order - 11];
			y03 = delay[order - 12];
			y04 = delay[order - 13];
			y05 = delay[order - 14];
			y06 = delay[order - 15];
			y07 = delay[order - 16];
			y08 = delay[order - 17];
			y09 = delay[order - 18];

			a1 = lpc[order - 8];
			a2 = lpc[order - 7];
			a3 = lpc[order - 6];
			a4 = lpc[order - 5];
			a5 = lpc[order - 4];
			a6 = lpc[order - 3];
			a7 = lpc[order - 2];
			a8 = lpc[order - 1];
			a9 = lpc[order];

			dst[i + 0] =
			    src[i + 0] + a9 * y9 + a8 * y8 + a7 * y7 + a6 * y6 +
			    a5 * y5 + a4 * y4 + a3 * y3 + a2 * y2 + a1 * y1;
			dst[i + 1] =
			    src[i + 1] + a9 * y8 + a8 * y7 + a7 * y6 + a6 * y5 +
			    a5 * y4 + a4 * y3 + a3 * y2 + a2 * y1 + a1 * y00;
			dst[i + 2] =
			    src[i + 2] + a9 * y7 + a8 * y6 + a7 * y5 + a6 * y4 +
			    a5 * y3 + a4 * y2 + a3 * y1 + a2 * y00 + a1 * y01;
			dst[i + 3] =
			    src[i + 3] + a9 * y6 + a8 * y5 + a7 * y4 + a6 * y3 +
			    a5 * y2 + a4 * y1 + a3 * y00 + a2 * y01 + a1 * y02;
			dst[i + 4] =
			    src[i + 4] + a9 * y5 + a8 * y4 + a7 * y3 + a6 * y2 +
			    a5 * y1 + a4 * y00 + a3 * y01 + a2 * y02 + a1 * y03;
			dst[i + 5] =
			    src[i + 5] + a9 * y4 + a8 * y3 + a7 * y2 + a6 * y1 +
			    a5 * y00 + a4 * y01 + a3 * y02 + a2 * y03 +
			    a1 * y04;
			dst[i + 6] =
			    src[i + 6] + a9 * y3 + a8 * y2 + a7 * y1 +
			    a6 * y00 + a5 * y01 + a4 * y02 + a3 * y03 +
			    a2 * y04 + a1 * y05;
			dst[i + 7] =
			    src[i + 7] + a9 * y2 + a8 * y1 + a7 * y00 +
			    a6 * y01 + a5 * y02 + a4 * y03 + a3 * y04 +
			    a2 * y05 + a1 * y06;
			dst[i + 8] =
			    src[i + 8] + a9 * y1 + a8 * y00 + a7 * y01 +
			    a6 * y02 + a5 * y03 + a4 * y04 + a3 * y05 +
			    a2 * y06 + a1 * y07;
			dst[i + 9] =
			    src[i + 9] + a9 * y00 + a8 * y01 + a7 * y02 +
			    a6 * y03 + a5 * y04 + a4 * y05 + a3 * y06 +
			    a2 * y07 + a1 * y08;

			delay[order - 8] = y09;
			delay[order - 7] = y08;
			delay[order - 6] = y07;
			delay[order - 5] = y06;
			delay[order - 4] = y05;
			delay[order - 3] = y04;
			delay[order - 2] = y03;
			delay[order - 1] = y02;
			delay[order] = y01;
		} else {
			if (tail == 8) {
				y1 = delay[order - 7];
				y2 = delay[order - 6];
				y3 = delay[order - 5];
				y4 = delay[order - 4];
				y5 = delay[order - 3];
				y6 = delay[order - 2];
				y7 = delay[order - 1];
				y8 = delay[order];
				y00 = delay[order - 8];
				y01 = delay[order - 9];
				y02 = delay[order - 10];
				y03 = delay[order - 11];
				y04 = delay[order - 12];
				y05 = delay[order - 13];
				y06 = delay[order - 14];
				y07 = delay[order - 15];
				y08 = delay[order - 16];
				y09 = delay[order - 17];

				a1 = lpc[order - 7];
				a2 = lpc[order - 6];
				a3 = lpc[order - 5];
				a4 = lpc[order - 4];
				a5 = lpc[order - 3];
				a6 = lpc[order - 2];
				a7 = lpc[order - 1];
				a8 = lpc[order];

				dst[i + 0] =
				    src[i + 0] + a8 * y8 + a7 * y7 + a6 * y6 +
				    a5 * y5 + a4 * y4 + a3 * y3 + a2 * y2 +
				    a1 * y1;
				dst[i + 1] =
				    src[i + 1] + a8 * y7 + a7 * y6 + a6 * y5 +
				    a5 * y4 + a4 * y3 + a3 * y2 + a2 * y1 +
				    a1 * y00;
				dst[i + 2] =
				    src[i + 2] + a8 * y6 + a7 * y5 + a6 * y4 +
				    a5 * y3 + a4 * y2 + a3 * y1 + a2 * y00 +
				    a1 * y01;
				dst[i + 3] =
				    src[i + 3] + a8 * y5 + a7 * y4 + a6 * y3 +
				    a5 * y2 + a4 * y1 + a3 * y00 + a2 * y01 +
				    a1 * y02;
				dst[i + 4] =
				    src[i + 4] + a8 * y4 + a7 * y3 + a6 * y2 +
				    a5 * y1 + a4 * y00 + a3 * y01 + a2 * y02 +
				    a1 * y03;
				dst[i + 5] =
				    src[i + 5] + a8 * y3 + a7 * y2 + a6 * y1 +
				    a5 * y00 + a4 * y01 + a3 * y02 + a2 * y03 +
				    a1 * y04;
				dst[i + 6] =
				    src[i + 6] + a8 * y2 + a7 * y1 + a6 * y00 +
				    a5 * y01 + a4 * y02 + a3 * y03 + a2 * y04 +
				    a1 * y05;
				dst[i + 7] =
				    src[i + 7] + a8 * y1 + a7 * y00 + a6 * y01 +
				    a5 * y02 + a4 * y03 + a3 * y04 + a2 * y05 +
				    a1 * y06;
				dst[i + 8] =
				    src[i + 8] + a8 * y00 + a7 * y01 +
				    a6 * y02 + a5 * y03 + a4 * y04 + a3 * y05 +
				    a2 * y06 + a1 * y07;
				dst[i + 9] =
				    src[i + 9] + a8 * y01 + a7 * y02 +
				    a6 * y03 + a5 * y04 + a4 * y05 + a3 * y06 +
				    a2 * y07 + a1 * y08;

				delay[order - 7] = y09;
				delay[order - 6] = y08;
				delay[order - 5] = y07;
				delay[order - 4] = y06;
				delay[order - 3] = y05;
				delay[order - 2] = y04;
				delay[order - 1] = y03;
				delay[order] = y02;
			} else {
			    if (tail == 7) {
				y1 = delay[order - 6];
				y2 = delay[order - 5];
				y3 = delay[order - 4];
				y4 = delay[order - 3];
				y5 = delay[order - 2];
				y6 = delay[order - 1];
				y7 = delay[order];

				a1 = lpc[order - 6];
				a2 = lpc[order - 5];
				a3 = lpc[order - 4];
				a4 = lpc[order - 3];
				a5 = lpc[order - 2];
				a6 = lpc[order - 1];
				a7 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j1 = 0; j1 < 10; j1++) {
					dst[i + j1] = src[i + j1] + a7 * y7 +
						a6 * y6 + a5 * y5 + a4 * y4 +
						a3 * y3 + a2 * y2 + a1 * y1;

					y7 = y6;
					y6 = y5;
					y5 = y4;
					y4 = y3;
					y3 = y2;
					y2 = y1;
					y1 = delay[order - 7 - j1];
				}

				delay[order - 6] = y1;
				delay[order - 5] = y2;
				delay[order - 4] = y3;
				delay[order - 3] = y4;
				delay[order - 2] = y5;
				delay[order - 1] = y6;
				delay[order] = y7;
			    } else {
				if (tail == 6) {
				    y1 = delay[order - 5];
				    y2 = delay[order - 4];
				    y3 = delay[order - 3];
				    y4 = delay[order - 2];
				    y5 = delay[order - 1];
				    y6 = delay[order];

				    a1 = lpc[order - 5];
				    a2 = lpc[order - 4];
				    a3 = lpc[order - 3];
				    a4 = lpc[order - 2];
				    a5 = lpc[order - 1];
				    a6 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (j1 = 0; j1 < 10; j1++) {
					dst[i + j1] =
					    src[i + j1] +
					    a6 * y6 + a5 * y5 +
					    a4 * y4 + a3 * y3 +
					    a2 * y2 + a1 * y1;

					y6 = y5;
					y5 = y4;
					y4 = y3;
					y3 = y2;
					y2 = y1;
					y1 = delay[order - 6 - j1];
				    }

				    delay[order - 5] = y1;
				    delay[order - 4] = y2;
				    delay[order - 3] = y3;
				    delay[order - 2] = y4;
				    delay[order - 1] = y5;
				    delay[order] = y6;
				} else {
				    if (tail == 5) {
					y1 = delay[order - 4];
					y2 = delay[order - 3];
					y3 = delay[order - 2];
					y4 = delay[order - 1];
					y5 = delay[order];

					a1 = lpc[order - 4];
					a2 = lpc[order - 3];
					a3 = lpc[order - 2];
					a4 = lpc[order - 1];
					a5 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j1 = 0; j1 < 10; j1++) {
					    dst[i + j1] = src[i + j1] + a5 *
						y5 + a4 * y4 + a3 * y3 + a2 *
						y2 + a1 * y1;

					    y5 = y4;
					    y4 = y3;
					    y3 = y2;
					    y2 = y1;
					    y1 = delay[order - 5 - j1];
					}

					delay[order - 4] = y1;
					delay[order - 3] = y2;
					delay[order - 2] = y3;
					delay[order - 1] = y4;
					delay[order] = y5;
				    } else {
					if (tail == 4) {
						y1 = delay[order - 3];
						y2 = delay[order - 2];
						y3 = delay[order - 1];
						y4 = delay[order];

						a1 = lpc[order - 3];
						a2 = lpc[order - 2];
						a3 = lpc[order - 1];
						a4 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j1 = 0; j1 < 10; j1++) {
						    dst[i + j1] = src[i + j1] +
							a4 * y4 + a3 * y3 + a2 *
							y2 + a1 * y1;

						    y4 = y3;
						    y3 = y2;
						    y2 = y1;
						    y1 = delay[order - 4 - j1];
						}

						delay[order - 3] = y1;
						delay[order - 2] = y2;
						delay[order - 1] = y3;
						delay[order] = y4;
					} else {
					    if (tail == 3) {
						y1 = delay[order - 2];
						y2 = delay[order - 1];
						y3 = delay[order];

						a1 = lpc[order - 2];
						a2 = lpc[order - 1];
						a3 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j1 = 0; j1 < 10; j1++) {
							dst[i + j1] =
								src[i + j1] +
								a3 * y3 +
								a2 * y2 +
								a1 * y1;

							y3 = y2;
							y2 = y1;
							y1 = delay[order - 3 -
								j1];
						}

						delay[order - 2] = y1;
						delay[order - 1] = y2;
						delay[order] = y3;
					    } else {
						if (tail == 2) {
						    y1 = delay[order - 1];
						    y2 = delay[order];
						    a1 = lpc[order - 1];
						    a2 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						    for (j1 = 0; j1 < 10;
							j1++) {
							dst[i + j1] =
								src[i + j1] +
									a2 *
									y2 +
									a1 *
									y1;

							y2 = y1;
							y1 = delay[order -
								2 - j1];
						    }

						    delay[order - 1] = y1;
						    delay[order] = y2;
						} else {
						    if (tail == 1) {
							y1 = delay[order];
							a1 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (j1 = 0; j1 < 10;
							    j1++) {
							    dst[i + j1] =
								src[i + j1] +
								a1 * y1;

							    y1 = delay[order -
								1 - j1];
							}

							delay[order] = y1;
						    } else {
							dst[i + 0] = src[i + 0];
							dst[i + 1] = src[i + 1];
							dst[i + 2] = src[i + 2];
							dst[i + 3] = src[i + 3];
							dst[i + 4] = src[i + 4];
							dst[i + 5] = src[i + 5];
							dst[i + 6] = src[i + 6];
							dst[i + 7] = src[i + 7];
							dst[i + 8] = src[i + 8];
							dst[i + 9] = src[i + 9];
						    }
						}
					    }
					}
				    }
				}
			    }
			}
		}

		j1 = order - tail;
		y1 = delay[j1 - 9];
		y2 = delay[j1 - 8];
		y3 = delay[j1 - 7];
		y4 = delay[j1 - 6];
		y5 = delay[j1 - 5];
		y6 = delay[j1 - 4];
		y7 = delay[j1 - 3];
		y8 = delay[j1 - 2];
		y9 = delay[j1 - 1];
		y10 = delay[j1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j1 >= 19; j1 -= 10) {
			y00 = delay[j1 - 10];
			y01 = delay[j1 - 11];
			y02 = delay[j1 - 12];
			y03 = delay[j1 - 13];
			y04 = delay[j1 - 14];
			y05 = delay[j1 - 15];
			y06 = delay[j1 - 16];
			y07 = delay[j1 - 17];
			y08 = delay[j1 - 18];
			y09 = delay[j1 - 19];

			a1 = lpc[j1 - 9];
			a2 = lpc[j1 - 8];
			a3 = lpc[j1 - 7];
			a4 = lpc[j1 - 6];
			a5 = lpc[j1 - 5];
			a6 = lpc[j1 - 4];
			a7 = lpc[j1 - 3];
			a8 = lpc[j1 - 2];
			a9 = lpc[j1 - 1];
			a10 = lpc[j1];

			dst[i + 0] +=
			    a10 * y10 + a9 * y9 + a8 * y8 + a7 * y7 + a6 * y6 +
			    a5 * y5 + a4 * y4 + a3 * y3 + a2 * y2 + a1 * y1;
			dst[i + 1] +=
			    a10 * y9 + a9 * y8 + a8 * y7 + a7 * y6 + a6 * y5 +
			    a5 * y4 + a4 * y3 + a3 * y2 + a2 * y1 + a1 * y00;
			dst[i + 2] +=
			    a10 * y8 + a9 * y7 + a8 * y6 + a7 * y5 + a6 * y4 +
			    a5 * y3 + a4 * y2 + a3 * y1 + a2 * y00 + a1 * y01;
			dst[i + 3] +=
			    a10 * y7 + a9 * y6 + a8 * y5 + a7 * y4 + a6 * y3 +
			    a5 * y2 + a4 * y1 + a3 * y00 + a2 * y01 + a1 * y02;
			dst[i + 4] +=
			    a10 * y6 + a9 * y5 + a8 * y4 + a7 * y3 + a6 * y2 +
			    a5 * y1 + a4 * y00 + a3 * y01 + a2 * y02 + a1 * y03;
			dst[i + 5] +=
			    a10 * y5 + a9 * y4 + a8 * y3 + a7 * y2 + a6 * y1 +
			    a5 * y00 + a4 * y01 + a3 * y02 + a2 * y03 +
			    a1 * y04;
			dst[i + 6] +=
			    a10 * y4 + a9 * y3 + a8 * y2 + a7 * y1 + a6 * y00 +
			    a5 * y01 + a4 * y02 + a3 * y03 + a2 * y04 +
			    a1 * y05;
			dst[i + 7] +=
			    a10 * y3 + a9 * y2 + a8 * y1 + a7 * y00 + a6 * y01 +
			    a5 * y02 + a4 * y03 + a3 * y04 + a2 * y05 +
			    a1 * y06;
			dst[i + 8] +=
			    a10 * y2 + a9 * y1 + a8 * y00 + a7 * y01 +
			    a6 * y02 + a5 * y03 + a4 * y04 + a3 * y05 +
			    a2 * y06 + a1 * y07;
			dst[i + 9] +=
			    a10 * y1 + a9 * y00 + a8 * y01 + a7 * y02 +
			    a6 * y03 + a5 * y04 + a4 * y05 + a3 * y06 +
			    a2 * y07 + a1 * y08;

			y1 = y09;
			y2 = y08;
			y3 = y07;
			y4 = y06;
			y5 = y05;
			y6 = y04;
			y7 = y03;
			y8 = y02;
			y9 = y01;
			y10 = y00;
			delay[j1 - 9] = y09;
			delay[j1 - 8] = y08;
			delay[j1 - 7] = y07;
			delay[j1 - 6] = y06;
			delay[j1 - 5] = y05;
			delay[j1 - 4] = y04;
			delay[j1 - 3] = y03;
			delay[j1 - 2] = y02;
			delay[j1 - 1] = y01;
			delay[j1] = y00;
		}

		y1 = delay[1];
		y2 = delay[2];
		y3 = delay[3];
		y4 = delay[4];
		y5 = delay[5];
		y6 = delay[6];
		y7 = delay[7];
		y8 = delay[8];
		y9 = delay[9];
		y10 = delay[10];

		a1 = lpc[1];
		a2 = lpc[2];
		a3 = lpc[3];
		a4 = lpc[4];
		a5 = lpc[5];
		a6 = lpc[6];
		a7 = lpc[7];
		a8 = lpc[8];
		a9 = lpc[9];
		a10 = lpc[10];

		y00 =
		    dst[i + 0] + a10 * y10 + a9 * y9 + a8 * y8 + a7 * y7 +
		    a6 * y6 + a5 * y5 + a4 * y4 + a3 * y3 + a2 * y2 + a1 * y1;
		y01 =
		    dst[i + 1] + a10 * y9 + a9 * y8 + a8 * y7 + a7 * y6 +
		    a6 * y5 + a5 * y4 + a4 * y3 + a3 * y2 + a2 * y1 + a1 * y00;
		y02 =
		    dst[i + 2] + a10 * y8 + a9 * y7 + a8 * y6 + a7 * y5 +
		    a6 * y4 + a5 * y3 + a4 * y2 + a3 * y1 + a2 * y00 + a1 * y01;
		y03 =
		    dst[i + 3] + a10 * y7 + a9 * y6 + a8 * y5 + a7 * y4 +
		    a6 * y3 + a5 * y2 + a4 * y1 + a3 * y00 + a2 * y01 +
		    a1 * y02;
		y04 =
		    dst[i + 4] + a10 * y6 + a9 * y5 + a8 * y4 + a7 * y3 +
		    a6 * y2 + a5 * y1 + a4 * y00 + a3 * y01 + a2 * y02 +
		    a1 * y03;
		y05 =
		    dst[i + 5] + a10 * y5 + a9 * y4 + a8 * y3 + a7 * y2 +
		    a6 * y1 + a5 * y00 + a4 * y01 + a3 * y02 + a2 * y03 +
		    a1 * y04;
		y06 =
		    dst[i + 6] + a10 * y4 + a9 * y3 + a8 * y2 + a7 * y1 +
		    a6 * y00 + a5 * y01 + a4 * y02 + a3 * y03 + a2 * y04 +
		    a1 * y05;
		y07 =
		    dst[i + 7] + a10 * y3 + a9 * y2 + a8 * y1 + a7 * y00 +
		    a6 * y01 + a5 * y02 + a4 * y03 + a3 * y04 + a2 * y05 +
		    a1 * y06;
		y08 =
		    dst[i + 8] + a10 * y2 + a9 * y1 + a8 * y00 + a7 * y01 +
		    a6 * y02 + a5 * y03 + a4 * y04 + a3 * y05 + a2 * y06 +
		    a1 * y07;
		y09 =
		    dst[i + 9] + a10 * y1 + a9 * y00 + a8 * y01 + a7 * y02 +
		    a6 * y03 + a5 * y04 + a4 * y05 + a3 * y06 + a2 * y07 +
		    a1 * y08;

		dst[i + 0] = y00;
		dst[i + 1] = y01;
		dst[i + 2] = y02;
		dst[i + 3] = y03;
		dst[i + 4] = y04;
		dst[i + 5] = y05;
		dst[i + 6] = y06;
		dst[i + 7] = y07;
		dst[i + 8] = y08;
		dst[i + 9] = y09;

		delay[1] = y09;
		delay[2] = y08;
		delay[3] = y07;
		delay[4] = y06;
		delay[5] = y05;
		delay[6] = y04;
		delay[7] = y03;
		delay[8] = y02;
		delay[9] = y01;
		delay[10] = y00;
	}

	if (i < length) {
		if (tail == 9) {
			y1 = delay[order - 8];
			y2 = delay[order - 7];
			y3 = delay[order - 6];
			y4 = delay[order - 5];
			y5 = delay[order - 4];
			y6 = delay[order - 3];
			y7 = delay[order - 2];
			y8 = delay[order - 1];
			y9 = delay[order];

			a1 = lpc[order - 8];
			a2 = lpc[order - 7];
			a3 = lpc[order - 6];
			a4 = lpc[order - 5];
			a5 = lpc[order - 4];
			a6 = lpc[order - 3];
			a7 = lpc[order - 2];
			a8 = lpc[order - 1];
			a9 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] =
				    src[i + j1] + a9 * y9 + a8 * y8 + a7 * y7 +
				    a6 * y6 + a5 * y5 + a4 * y4 + a3 * y3 +
				    a2 * y2 + a1 * y1;

				y9 = y8;
				y8 = y7;
				y7 = y6;
				y6 = y5;
				y5 = y4;
				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[order - 9 - j1];
			}

			delay[order - 8] = y1;
			delay[order - 7] = y2;
			delay[order - 6] = y3;
			delay[order - 5] = y4;
			delay[order - 4] = y5;
			delay[order - 3] = y6;
			delay[order - 2] = y7;
			delay[order - 1] = y8;
			delay[order] = y9;
		} else if (tail == 8) {
			y1 = delay[order - 7];
			y2 = delay[order - 6];
			y3 = delay[order - 5];
			y4 = delay[order - 4];
			y5 = delay[order - 3];
			y6 = delay[order - 2];
			y7 = delay[order - 1];
			y8 = delay[order];

			a1 = lpc[order - 7];
			a2 = lpc[order - 6];
			a3 = lpc[order - 5];
			a4 = lpc[order - 4];
			a5 = lpc[order - 3];
			a6 = lpc[order - 2];
			a7 = lpc[order - 1];
			a8 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] =
				    src[i + j1] + a8 * y8 + a7 * y7 + a6 * y6 +
				    a5 * y5 + a4 * y4 + a3 * y3 + a2 * y2 +
				    a1 * y1;

				y8 = y7;
				y7 = y6;
				y6 = y5;
				y5 = y4;
				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[order - 8 - j1];
			}

			delay[order - 7] = y1;
			delay[order - 6] = y2;
			delay[order - 5] = y3;
			delay[order - 4] = y4;
			delay[order - 3] = y5;
			delay[order - 2] = y6;
			delay[order - 1] = y7;
			delay[order] = y8;
		} else if (tail == 7) {
			y1 = delay[order - 6];
			y2 = delay[order - 5];
			y3 = delay[order - 4];
			y4 = delay[order - 3];
			y5 = delay[order - 2];
			y6 = delay[order - 1];
			y7 = delay[order];

			a1 = lpc[order - 6];
			a2 = lpc[order - 5];
			a3 = lpc[order - 4];
			a4 = lpc[order - 3];
			a5 = lpc[order - 2];
			a6 = lpc[order - 1];
			a7 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] =
				    src[i + j1] + a7 * y7 + a6 * y6 + a5 * y5 +
				    a4 * y4 + a3 * y3 + a2 * y2 + a1 * y1;

				y7 = y6;
				y6 = y5;
				y5 = y4;
				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[order - 7 - j1];
			}

			delay[order - 6] = y1;
			delay[order - 5] = y2;
			delay[order - 4] = y3;
			delay[order - 3] = y4;
			delay[order - 2] = y5;
			delay[order - 1] = y6;
			delay[order] = y7;
		} else if (tail == 6) {
			y1 = delay[order - 5];
			y2 = delay[order - 4];
			y3 = delay[order - 3];
			y4 = delay[order - 2];
			y5 = delay[order - 1];
			y6 = delay[order];

			a1 = lpc[order - 5];
			a2 = lpc[order - 4];
			a3 = lpc[order - 3];
			a4 = lpc[order - 2];
			a5 = lpc[order - 1];
			a6 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] =
				    src[i + j1] + a6 * y6 + a5 * y5 + a4 * y4 +
				    a3 * y3 + a2 * y2 + a1 * y1;

				y6 = y5;
				y5 = y4;
				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[order - 6 - j1];
			}

			delay[order - 5] = y1;
			delay[order - 4] = y2;
			delay[order - 3] = y3;
			delay[order - 2] = y4;
			delay[order - 1] = y5;
			delay[order] = y6;
		} else if (tail == 5) {
			y1 = delay[order - 4];
			y2 = delay[order - 3];
			y3 = delay[order - 2];
			y4 = delay[order - 1];
			y5 = delay[order];

			a1 = lpc[order - 4];
			a2 = lpc[order - 3];
			a3 = lpc[order - 2];
			a4 = lpc[order - 1];
			a5 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] = src[i + j1] + a5 * y5 + a4 * y4 +
				    a3 * y3 + a2 * y2 + a1 * y1;

				y5 = y4;
				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[order - 5 - j1];
			}

			delay[order - 4] = y1;
			delay[order - 3] = y2;
			delay[order - 2] = y3;
			delay[order - 1] = y4;
			delay[order] = y5;
		} else if (tail == 4) {
			y1 = delay[order - 3];
			y2 = delay[order - 2];
			y3 = delay[order - 1];
			y4 = delay[order];

			a1 = lpc[order - 3];
			a2 = lpc[order - 2];
			a3 = lpc[order - 1];
			a4 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] =
				    src[i + j1] + a4 * y4 + a3 * y3 + a2 * y2 +
				    a1 * y1;

				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[order - 4 - j1];
			}

			delay[order - 3] = y1;
			delay[order - 2] = y2;
			delay[order - 1] = y3;
			delay[order] = y4;
		} else if (tail == 3) {
			y1 = delay[order - 2];
			y2 = delay[order - 1];
			y3 = delay[order];

			a1 = lpc[order - 2];
			a2 = lpc[order - 1];
			a3 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] =
				    src[i + j1] + a3 * y3 + a2 * y2 + a1 * y1;

				y3 = y2;
				y2 = y1;
				y1 = delay[order - 3 - j1];
			}

			delay[order - 2] = y1;
			delay[order - 1] = y2;
			delay[order] = y3;
		} else if (tail == 2) {
			y1 = delay[order - 1];
			y2 = delay[order];
			a1 = lpc[order - 1];
			a2 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] = src[i + j1] + a2 * y2 + a1 * y1;

				y2 = y1;
				y1 = delay[order - 2 - j1];
			}

			delay[order - 1] = y1;
			delay[order] = y2;
		} else if (tail == 1) {
			y1 = delay[order];
			a1 = lpc[order];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] = src[i + j1] + a1 * y1;

				y1 = delay[order - 1 - j1];
			}

			delay[order] = y1;
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j1 = 0; j1 < (length - i); j1++) {
				dst[i + j1] = src[i + j1];
			}
		}

		for (j1 = order - tail; j1 >= 19; j1 -= 10) {
			y1 = delay[j1 - 9];
			y2 = delay[j1 - 8];
			y3 = delay[j1 - 7];
			y4 = delay[j1 - 6];
			y5 = delay[j1 - 5];
			y6 = delay[j1 - 4];
			y7 = delay[j1 - 3];
			y8 = delay[j1 - 2];
			y9 = delay[j1 - 1];
			y10 = delay[j1];

			a1 = lpc[j1 - 9];
			a2 = lpc[j1 - 8];
			a3 = lpc[j1 - 7];
			a4 = lpc[j1 - 6];
			a5 = lpc[j1 - 5];
			a6 = lpc[j1 - 4];
			a7 = lpc[j1 - 3];
			a8 = lpc[j1 - 2];
			a9 = lpc[j1 - 1];
			a10 = lpc[j1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j2 = 0; j2 < (length - i); j2++) {
				y0 = dst[i + j2] + a10 * y10 + a9 * y9 +
				    a8 * y8 + a7 * y7 + a6 * y6 + a5 * y5 +
				    a4 * y4 + a3 * y3 + a2 * y2 + a1 * y1;

				y10 = y9;
				y9 = y8;
				y8 = y7;
				y7 = y6;
				y6 = y5;
				y5 = y4;
				y4 = y3;
				y3 = y2;
				y2 = y1;
				y1 = delay[j1 - 10 - j2];
				dst[i + j2] = y0;
			}

			delay[j1 - 9] = y1;
			delay[j1 - 8] = y2;
			delay[j1 - 7] = y3;
			delay[j1 - 6] = y4;
			delay[j1 - 5] = y5;
			delay[j1 - 4] = y6;
			delay[j1 - 3] = y7;
			delay[j1 - 2] = y8;
			delay[j1 - 1] = y9;
			delay[j1] = y10;
		}

		y1 = delay[1];
		y2 = delay[2];
		y3 = delay[3];
		y4 = delay[4];
		y5 = delay[5];
		y6 = delay[6];
		y7 = delay[7];
		y8 = delay[8];
		y9 = delay[9];
		y10 = delay[10];

		a1 = lpc[1];
		a2 = lpc[2];
		a3 = lpc[3];
		a4 = lpc[4];
		a5 = lpc[5];
		a6 = lpc[6];
		a7 = lpc[7];
		a8 = lpc[8];
		a9 = lpc[9];
		a10 = lpc[10];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j2 = 0; j2 < (length - i); j2++) {
			y0 = dst[i + j2] + a10 * y10 + a9 * y9 + a8 * y8 +
			    a7 * y7 + a6 * y6 + a5 * y5 + a4 * y4 + a3 * y3 +
			    a2 * y2 + a1 * y1;

			y10 = y9;
			y9 = y8;
			y8 = y7;
			y7 = y6;
			y6 = y5;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			dst[i + j2] = y0;
			y1 = y0;
		}

		delay[1] = y1;
		delay[2] = y2;
		delay[3] = y3;
		delay[4] = y4;
		delay[5] = y5;
		delay[6] = y6;
		delay[7] = y7;
		delay[8] = y8;
		delay[9] = y9;
		delay[10] = y10;
	}
}

/* *********************************************************** */

static void
PerceptWeightOrderIsVerySmall_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    filter_state * filter)
{
	mlib_s32 order = filter->order;
	mlib_s32 length = filter->length;
	mlib_d64 *ZeroTable = filter->ZeroTable;
	mlib_d64 *PoleTable = filter->PoleTable;
	mlib_d64 *zero = filter->zero;
	mlib_d64 *pole = filter->pole;
	mlib_s32 i;
	mlib_d64 z1, z2, z3;
	mlib_d64 p1, p2, p3;
	mlib_d64 x0, x1, x2, x3;
	mlib_d64 y0, y1, y2, y3;
	mlib_d64 tmp;

	if (order == 1) {
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		x1 = zero[1];
		y1 = pole[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length; i++) {
			x0 = src[i];
			y0 = x0 - z1 * x1;
			x1 = x0;

			tmp = y0 + p1 * y1;
			y1 = tmp;
			dst[i] = tmp;
		}

		zero[1] = x1;
		pole[1] = y1;
	} else if (order == 2) {
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length; i++) {
			x0 = src[i];
			y0 = x0 - z2 * x2 - z1 * x1;
			x2 = x1;
			x1 = x0;

			tmp = y0 + p2 * y2 + p1 * y1;
			y2 = y1;
			y1 = tmp;
			dst[i] = tmp;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
	} else if (order == 3) {
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length; i++) {
			x0 = src[i];
			y0 = x0 - z3 * x3 - z2 * x2 - z1 * x1;
			x3 = x2;
			x2 = x1;
			x1 = x0;

			tmp = y0 + p3 * y3 + p2 * y2 + p1 * y1;
			y3 = y2;
			y2 = y1;
			y1 = tmp;
			dst[i] = tmp;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
	}
}

/* *********************************************************** */

void
mlib_PerceptWeightOrderIsSmall(
    mlib_d64 *dst_buf,
    const mlib_d64 *src_buf,
    filter_state * filter)
{
	mlib_s32 order = filter->order;
	mlib_s32 length = filter->length;
	mlib_d64 *ZeroTable = filter->ZeroTable;
	mlib_d64 *PoleTable = filter->PoleTable;
	mlib_d64 *fir_buf = filter->fir_buf;
	mlib_d64 *zero = filter->zero;
	mlib_d64 *pole = filter->pole;
	mlib_s32 i;
	mlib_d64 z1, z2, z3, z4, z5, z6, z7, z8, z9, z10;
	mlib_d64 p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	mlib_d64 x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;
	mlib_d64 y1, y2, y3, y4, y5, y6, y7, y8, y9, y10;
	mlib_d64 x00, x01, x02, x03, x04, x05, x06, x07;
	mlib_d64 y00, y01, y02, y03, y04, y05, y06, y07;

	switch (order) {
	case 4:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 = z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 = z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			y04 = z4 * x00 + z3 * x01 + z2 * x02 + z1 * x03;
			y05 = z4 * x01 + z3 * x02 + z2 * x03 + z1 * x04;
			y06 = z4 * x02 + z3 * x03 + z2 * x04 + z1 * x05;
			y07 = z4 * x03 + z3 * x04 + z2 * x05 + z1 * x06;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 = z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 = z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 = z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 = z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 = y00 + p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			x01 = y01 + p4 * y3 + p3 * y2 + p2 * y1 + p1 * x00;
			x02 = y02 + p4 * y2 + p3 * y1 + p2 * x00 + p1 * x01;
			x03 = y03 + p4 * y1 + p3 * x00 + p2 * x01 + p1 * x02;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 = y00 + p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			x01 = y01 + p4 * y3 + p3 * y2 + p2 * y1 + p1 * x00;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 = y00 + p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		break;
	case 5:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		z5 = ZeroTable[5];
		p5 = PoleTable[5];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];
		x5 = zero[5];
		y5 = pole[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 = z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z5 * x3 + z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 =
			    z5 * x2 + z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			y04 =
			    z5 * x1 + z4 * x00 + z3 * x01 + z2 * x02 + z1 * x03;
			y05 =
			    z5 * x00 + z4 * x01 + z3 * x02 + z2 * x03 +
			    z1 * x04;
			y06 =
			    z5 * x01 + z4 * x02 + z3 * x03 + z2 * x04 +
			    z1 * x05;
			y07 =
			    z5 * x02 + z4 * x03 + z3 * x04 + z2 * x05 +
			    z1 * x06;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 = z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z5 * x3 + z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 =
			    z5 * x2 + z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 = z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 = z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 =
			    y00 + p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 +
			    p1 * y1;
			x01 =
			    y01 + p5 * y4 + p4 * y3 + p3 * y2 + p2 * y1 +
			    p1 * x00;
			x02 =
			    y02 + p5 * y3 + p4 * y2 + p3 * y1 + p2 * x00 +
			    p1 * x01;
			x03 =
			    y03 + p5 * y2 + p4 * y1 + p3 * x00 + p2 * x01 +
			    p1 * x02;
			y5 = y1;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 =
			    y00 + p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 +
			    p1 * y1;
			x01 =
			    y01 + p5 * y4 + p4 * y3 + p3 * y2 + p2 * y1 +
			    p1 * x00;
			y5 = y3;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 =
			    y00 + p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 +
			    p1 * y1;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		zero[5] = x5;
		pole[5] = y5;
		break;
	case 6:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		z5 = ZeroTable[5];
		p5 = PoleTable[5];
		z6 = ZeroTable[6];
		p6 = PoleTable[6];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];
		x5 = zero[5];
		y5 = pole[5];
		x6 = zero[6];
		y6 = pole[6];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 =
			    z6 * x6 + z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 +
			    z1 * x1;
			y01 =
			    z6 * x5 + z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 +
			    z1 * x00;
			y02 =
			    z6 * x4 + z5 * x3 + z4 * x2 + z3 * x1 + z2 * x00 +
			    z1 * x01;
			y03 =
			    z6 * x3 + z5 * x2 + z4 * x1 + z3 * x00 + z2 * x01 +
			    z1 * x02;
			y04 =
			    z6 * x2 + z5 * x1 + z4 * x00 + z3 * x01 + z2 * x02 +
			    z1 * x03;
			y05 =
			    z6 * x1 + z5 * x00 + z4 * x01 + z3 * x02 +
			    z2 * x03 + z1 * x04;
			y06 =
			    z6 * x00 + z5 * x01 + z4 * x02 + z3 * x03 +
			    z2 * x04 + z1 * x05;
			y07 =
			    z6 * x01 + z5 * x02 + z4 * x03 + z3 * x04 +
			    z2 * x05 + z1 * x06;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 =
			    z6 * x6 + z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 +
			    z1 * x1;
			y01 =
			    z6 * x5 + z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 +
			    z1 * x00;
			y02 =
			    z6 * x4 + z5 * x3 + z4 * x2 + z3 * x1 + z2 * x00 +
			    z1 * x01;
			y03 =
			    z6 * x3 + z5 * x2 + z4 * x1 + z3 * x00 + z2 * x01 +
			    z1 * x02;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 =
			    z6 * x6 + z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 +
			    z1 * x1;
			y01 =
			    z6 * x5 + z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 +
			    z1 * x00;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 =
			    z6 * x6 + z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 +
			    z1 * x1;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 =
			    y00 + p6 * y6 + p5 * y5 + p4 * y4 + p3 * y3 +
			    p2 * y2 + p1 * y1;
			x01 =
			    y01 + p6 * y5 + p5 * y4 + p4 * y3 + p3 * y2 +
			    p2 * y1 + p1 * x00;
			x02 =
			    y02 + p6 * y4 + p5 * y3 + p4 * y2 + p3 * y1 +
			    p2 * x00 + p1 * x01;
			x03 =
			    y03 + p6 * y3 + p5 * y2 + p4 * y1 + p3 * x00 +
			    p2 * x01 + p1 * x02;
			y6 = y2;
			y5 = y1;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 =
			    y00 + p6 * y6 + p5 * y5 + p4 * y4 + p3 * y3 +
			    p2 * y2 + p1 * y1;
			x01 =
			    y01 + p6 * y5 + p5 * y4 + p4 * y3 + p3 * y2 +
			    p2 * y1 + p1 * x00;
			y6 = y4;
			y5 = y3;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 =
			    y00 + p6 * y6 + p5 * y5 + p4 * y4 + p3 * y3 +
			    p2 * y2 + p1 * y1;
			y6 = y5;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		zero[5] = x5;
		pole[5] = y5;
		zero[6] = x6;
		pole[6] = y6;
		break;
	case 7:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		z5 = ZeroTable[5];
		p5 = PoleTable[5];
		z6 = ZeroTable[6];
		p6 = PoleTable[6];
		z7 = ZeroTable[7];
		p7 = PoleTable[7];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];
		x5 = zero[5];
		y5 = pole[5];
		x6 = zero[6];
		y6 = pole[6];
		x7 = zero[7];
		y7 = pole[7];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 = z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z7 * x6 + z6 * x5 + z5 * x4 + z4 * x3 +
			    z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z7 * x5 + z6 * x4 + z5 * x3 + z4 * x2 +
			    z3 * x1 + z2 * x00 + z1 * x01;
			y03 = z7 * x4 + z6 * x3 + z5 * x2 + z4 * x1 +
			    z3 * x00 + z2 * x01 + z1 * x02;
			y04 = z7 * x3 + z6 * x2 + z5 * x1 + z4 * x00 +
			    z3 * x01 + z2 * x02 + z1 * x03;
			y05 = z7 * x2 + z6 * x1 + z5 * x00 + z4 * x01 +
			    z3 * x02 + z2 * x03 + z1 * x04;
			y06 = z7 * x1 + z6 * x00 + z5 * x01 + z4 * x02 +
			    z3 * x03 + z2 * x04 + z1 * x05;
			y07 = z7 * x00 + z6 * x01 + z5 * x02 + z4 * x03 +
			    z3 * x04 + z2 * x05 + z1 * x06;
			x7 = x01;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 = z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z7 * x6 + z6 * x5 + z5 * x4 + z4 * x3 +
			    z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z7 * x5 + z6 * x4 + z5 * x3 + z4 * x2 +
			    z3 * x1 + z2 * x00 + z1 * x01;
			y03 = z7 * x4 + z6 * x3 + z5 * x2 + z4 * x1 +
			    z3 * x00 + z2 * x01 + z1 * x02;
			x7 = x3;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 = z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z7 * x6 + z6 * x5 + z5 * x4 + z4 * x3 +
			    z3 * x2 + z2 * x1 + z1 * x00;
			x7 = x5;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 = z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 = y00 + p7 * y7 + p6 * y6 + p5 * y5 + p4 * y4 +
			    p3 * y3 + p2 * y2 + p1 * y1;
			x01 = y01 + p7 * y6 + p6 * y5 + p5 * y4 + p4 * y3 +
			    p3 * y2 + p2 * y1 + p1 * x00;
			x02 = y02 + p7 * y5 + p6 * y4 + p5 * y3 + p4 * y2 +
			    p3 * y1 + p2 * x00 + p1 * x01;
			x03 = y03 + p7 * y4 + p6 * y3 + p5 * y2 + p4 * y1 +
			    p3 * x00 + p2 * x01 + p1 * x02;
			y7 = y3;
			y6 = y2;
			y5 = y1;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 = y00 + p7 * y7 + p6 * y6 + p5 * y5 + p4 * y4 +
			    p3 * y3 + p2 * y2 + p1 * y1;
			x01 = y01 + p7 * y6 + p6 * y5 + p5 * y4 + p4 * y3 +
			    p3 * y2 + p2 * y1 + p1 * x00;
			y7 = y5;
			y6 = y4;
			y5 = y3;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 = y00 + p7 * y7 + p6 * y6 + p5 * y5 + p4 * y4 +
			    p3 * y3 + p2 * y2 + p1 * y1;
			y7 = y6;
			y6 = y5;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		zero[5] = x5;
		pole[5] = y5;
		zero[6] = x6;
		pole[6] = y6;
		zero[7] = x7;
		pole[7] = y7;
		break;
	case 8:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		z5 = ZeroTable[5];
		p5 = PoleTable[5];
		z6 = ZeroTable[6];
		p6 = PoleTable[6];
		z7 = ZeroTable[7];
		p7 = PoleTable[7];
		z8 = ZeroTable[8];
		p8 = PoleTable[8];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];
		x5 = zero[5];
		y5 = pole[5];
		x6 = zero[6];
		y6 = pole[6];
		x7 = zero[7];
		y7 = pole[7];
		x8 = zero[8];
		y8 = pole[8];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 = z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z8 * x7 + z7 * x6 + z6 * x5 + z5 * x4 + z4 * x3 +
			    z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z8 * x6 + z7 * x5 + z6 * x4 + z5 * x3 + z4 * x2 +
			    z3 * x1 + z2 * x00 + z1 * x01;
			y03 = z8 * x5 + z7 * x4 + z6 * x3 + z5 * x2 + z4 * x1 +
			    z3 * x00 + z2 * x01 + z1 * x02;
			y04 = z8 * x4 + z7 * x3 + z6 * x2 + z5 * x1 + z4 * x00 +
			    z3 * x01 + z2 * x02 + z1 * x03;
			y05 =
			    z8 * x3 + z7 * x2 + z6 * x1 + z5 * x00 + z4 * x01 +
			    z3 * x02 + z2 * x03 + z1 * x04;
			y06 =
			    z8 * x2 + z7 * x1 + z6 * x00 + z5 * x01 + z4 * x02 +
			    z3 * x03 + z2 * x04 + z1 * x05;
			y07 =
			    z8 * x1 + z7 * x00 + z6 * x01 + z5 * x02 +
			    z4 * x03 + z3 * x04 + z2 * x05 + z1 * x06;
			x8 = x00;
			x7 = x01;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 = z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z8 * x7 + z7 * x6 + z6 * x5 + z5 * x4 + z4 * x3 +
			    z3 * x2 + z2 * x1 + z1 * x00;
			y02 = z8 * x6 + z7 * x5 + z6 * x4 + z5 * x3 + z4 * x2 +
			    z3 * x1 + z2 * x00 + z1 * x01;
			y03 = z8 * x5 + z7 * x4 + z6 * x3 + z5 * x2 + z4 * x1 +
			    z3 * x00 + z2 * x01 + z1 * x02;
			x8 = x4;
			x7 = x3;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 = z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			y01 = z8 * x7 + z7 * x6 + z6 * x5 + z5 * x4 + z4 * x3 +
			    z3 * x2 + z2 * x1 + z1 * x00;
			x8 = x6;
			x7 = x5;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 = z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 + z4 * x4 +
			    z3 * x3 + z2 * x2 + z1 * x1;
			x8 = x7;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 =
			    y00 + p8 * y8 + p7 * y7 + p6 * y6 + p5 * y5 +
			    p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			x01 =
			    y01 + p8 * y7 + p7 * y6 + p6 * y5 + p5 * y4 +
			    p4 * y3 + p3 * y2 + p2 * y1 + p1 * x00;
			x02 =
			    y02 + p8 * y6 + p7 * y5 + p6 * y4 + p5 * y3 +
			    p4 * y2 + p3 * y1 + p2 * x00 + p1 * x01;
			x03 =
			    y03 + p8 * y5 + p7 * y4 + p6 * y3 + p5 * y2 +
			    p4 * y1 + p3 * x00 + p2 * x01 + p1 * x02;
			y8 = y4;
			y7 = y3;
			y6 = y2;
			y5 = y1;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 =
			    y00 + p8 * y8 + p7 * y7 + p6 * y6 + p5 * y5 +
			    p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			x01 =
			    y01 + p8 * y7 + p7 * y6 + p6 * y5 + p5 * y4 +
			    p4 * y3 + p3 * y2 + p2 * y1 + p1 * x00;
			y8 = y6;
			y7 = y5;
			y6 = y4;
			y5 = y3;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 =
			    y00 + p8 * y8 + p7 * y7 + p6 * y6 + p5 * y5 +
			    p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			y8 = y7;
			y7 = y6;
			y6 = y5;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		zero[5] = x5;
		pole[5] = y5;
		zero[6] = x6;
		pole[6] = y6;
		zero[7] = x7;
		pole[7] = y7;
		zero[8] = x8;
		pole[8] = y8;
		break;
	case 9:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		z5 = ZeroTable[5];
		p5 = PoleTable[5];
		z6 = ZeroTable[6];
		p6 = PoleTable[6];
		z7 = ZeroTable[7];
		p7 = PoleTable[7];
		z8 = ZeroTable[8];
		p8 = PoleTable[8];
		z9 = ZeroTable[9];
		p9 = PoleTable[9];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];
		x5 = zero[5];
		y5 = pole[5];
		x6 = zero[6];
		y6 = pole[6];
		x7 = zero[7];
		y7 = pole[7];
		x8 = zero[8];
		y8 = pole[8];
		x9 = zero[9];
		y9 = pole[9];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 =
			    z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 +
			    z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 =
			    z9 * x8 + z8 * x7 + z7 * x6 + z6 * x5 + z5 * x4 +
			    z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 =
			    z9 * x7 + z8 * x6 + z7 * x5 + z6 * x4 + z5 * x3 +
			    z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 =
			    z9 * x6 + z8 * x5 + z7 * x4 + z6 * x3 + z5 * x2 +
			    z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			y04 =
			    z9 * x5 + z8 * x4 + z7 * x3 + z6 * x2 + z5 * x1 +
			    z4 * x00 + z3 * x01 + z2 * x02 + z1 * x03;
			y05 =
			    z9 * x4 + z8 * x3 + z7 * x2 + z6 * x1 + z5 * x00 +
			    z4 * x01 + z3 * x02 + z2 * x03 + z1 * x04;
			y06 =
			    z9 * x3 + z8 * x2 + z7 * x1 + z6 * x00 + z5 * x01 +
			    z4 * x02 + z3 * x03 + z2 * x04 + z1 * x05;
			y07 =
			    z9 * x2 + z8 * x1 + z7 * x00 + z6 * x01 + z5 * x02 +
			    z4 * x03 + z3 * x04 + z2 * x05 + z1 * x06;
			x9 = x1;
			x8 = x00;
			x7 = x01;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 =
			    z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 +
			    z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 =
			    z9 * x8 + z8 * x7 + z7 * x6 + z6 * x5 + z5 * x4 +
			    z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 =
			    z9 * x7 + z8 * x6 + z7 * x5 + z6 * x4 + z5 * x3 +
			    z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 =
			    z9 * x6 + z8 * x5 + z7 * x4 + z6 * x3 + z5 * x2 +
			    z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			x9 = x5;
			x8 = x4;
			x7 = x3;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 =
			    z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 +
			    z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 =
			    z9 * x8 + z8 * x7 + z7 * x6 + z6 * x5 + z5 * x4 +
			    z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			x9 = x7;
			x8 = x6;
			x7 = x5;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 =
			    z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 + z5 * x5 +
			    z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			x9 = x8;
			x8 = x7;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 =
			    y00 + p9 * y9 + p8 * y8 + p7 * y7 + p6 * y6 +
			    p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			x01 =
			    y01 + p9 * y8 + p8 * y7 + p7 * y6 + p6 * y5 +
			    p5 * y4 + p4 * y3 + p3 * y2 + p2 * y1 + p1 * x00;
			x02 =
			    y02 + p9 * y7 + p8 * y6 + p7 * y5 + p6 * y4 +
			    p5 * y3 + p4 * y2 + p3 * y1 + p2 * x00 + p1 * x01;
			x03 =
			    y03 + p9 * y6 + p8 * y5 + p7 * y4 + p6 * y3 +
			    p5 * y2 + p4 * y1 + p3 * x00 + p2 * x01 + p1 * x02;
			y9 = y5;
			y8 = y4;
			y7 = y3;
			y6 = y2;
			y5 = y1;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 =
			    y00 + p9 * y9 + p8 * y8 + p7 * y7 + p6 * y6 +
			    p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			x01 =
			    y01 + p9 * y8 + p8 * y7 + p7 * y6 + p6 * y5 +
			    p5 * y4 + p4 * y3 + p3 * y2 + p2 * y1 + p1 * x00;
			y9 = y7;
			y8 = y6;
			y7 = y5;
			y6 = y4;
			y5 = y3;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 =
			    y00 + p9 * y9 + p8 * y8 + p7 * y7 + p6 * y6 +
			    p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 + p1 * y1;
			y9 = y8;
			y8 = y7;
			y7 = y6;
			y6 = y5;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		zero[5] = x5;
		pole[5] = y5;
		zero[6] = x6;
		pole[6] = y6;
		zero[7] = x7;
		pole[7] = y7;
		zero[8] = x8;
		pole[8] = y8;
		zero[9] = x9;
		pole[9] = y9;
		break;
	case 10:
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		z2 = ZeroTable[2];
		p2 = PoleTable[2];
		z3 = ZeroTable[3];
		p3 = PoleTable[3];
		z4 = ZeroTable[4];
		p4 = PoleTable[4];
		z5 = ZeroTable[5];
		p5 = PoleTable[5];
		z6 = ZeroTable[6];
		p6 = PoleTable[6];
		z7 = ZeroTable[7];
		p7 = PoleTable[7];
		z8 = ZeroTable[8];
		p8 = PoleTable[8];
		z9 = ZeroTable[9];
		p9 = PoleTable[9];
		z10 = ZeroTable[10];
		p10 = PoleTable[10];
		x1 = zero[1];
		y1 = pole[1];
		x2 = zero[2];
		y2 = pole[2];
		x3 = zero[3];
		y3 = pole[3];
		x4 = zero[4];
		y4 = pole[4];
		x5 = zero[5];
		y5 = pole[5];
		x6 = zero[6];
		y6 = pole[6];
		x7 = zero[7];
		y7 = pole[7];
		x8 = zero[8];
		y8 = pole[8];
		x9 = zero[9];
		y9 = pole[9];
		x10 = zero[10];
		y10 = pole[10];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 7; i += 8) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			x04 = src_buf[i + 4];
			x05 = src_buf[i + 5];
			x06 = src_buf[i + 6];
			x07 = src_buf[i + 7];
			y00 =
			    z10 * x10 + z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 +
			    z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 =
			    z10 * x9 + z9 * x8 + z8 * x7 + z7 * x6 + z6 * x5 +
			    z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 =
			    z10 * x8 + z9 * x7 + z8 * x6 + z7 * x5 + z6 * x4 +
			    z5 * x3 + z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 =
			    z10 * x7 + z9 * x6 + z8 * x5 + z7 * x4 + z6 * x3 +
			    z5 * x2 + z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			y04 =
			    z10 * x6 + z9 * x5 + z8 * x4 + z7 * x3 + z6 * x2 +
			    z5 * x1 + z4 * x00 + z3 * x01 + z2 * x02 + z1 * x03;
			y05 =
			    z10 * x5 + z9 * x4 + z8 * x3 + z7 * x2 + z6 * x1 +
			    z5 * x00 + z4 * x01 + z3 * x02 + z2 * x03 +
			    z1 * x04;
			y06 =
			    z10 * x4 + z9 * x3 + z8 * x2 + z7 * x1 + z6 * x00 +
			    z5 * x01 + z4 * x02 + z3 * x03 + z2 * x04 +
			    z1 * x05;
			y07 =
			    z10 * x3 + z9 * x2 + z8 * x1 + z7 * x00 + z6 * x01 +
			    z5 * x02 + z4 * x03 + z3 * x04 + z2 * x05 +
			    z1 * x06;
			x10 = x2;
			x9 = x1;
			x8 = x00;
			x7 = x01;
			x6 = x02;
			x5 = x03;
			x4 = x04;
			x3 = x05;
			x2 = x06;
			x1 = x07;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			fir_buf[i + 4] = x04 - y04;
			fir_buf[i + 5] = x05 - y05;
			fir_buf[i + 6] = x06 - y06;
			fir_buf[i + 7] = x07 - y07;
		}

		if (i < length - 3) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			x02 = src_buf[i + 2];
			x03 = src_buf[i + 3];
			y00 =
			    z10 * x10 + z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 +
			    z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 =
			    z10 * x9 + z9 * x8 + z8 * x7 + z7 * x6 + z6 * x5 +
			    z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			y02 =
			    z10 * x8 + z9 * x7 + z8 * x6 + z7 * x5 + z6 * x4 +
			    z5 * x3 + z4 * x2 + z3 * x1 + z2 * x00 + z1 * x01;
			y03 =
			    z10 * x7 + z9 * x6 + z8 * x5 + z7 * x4 + z6 * x3 +
			    z5 * x2 + z4 * x1 + z3 * x00 + z2 * x01 + z1 * x02;
			x10 = x6;
			x9 = x5;
			x8 = x4;
			x7 = x3;
			x6 = x2;
			x5 = x1;
			x4 = x00;
			x3 = x01;
			x2 = x02;
			x1 = x03;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			fir_buf[i + 2] = x02 - y02;
			fir_buf[i + 3] = x03 - y03;
			i += 4;
		}

		if (i < length - 1) {
			x00 = src_buf[i + 0];
			x01 = src_buf[i + 1];
			y00 =
			    z10 * x10 + z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 +
			    z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			y01 =
			    z10 * x9 + z9 * x8 + z8 * x7 + z7 * x6 + z6 * x5 +
			    z5 * x4 + z4 * x3 + z3 * x2 + z2 * x1 + z1 * x00;
			x10 = x8;
			x9 = x7;
			x8 = x6;
			x7 = x5;
			x6 = x4;
			x5 = x3;
			x4 = x2;
			x3 = x1;
			x2 = x00;
			x1 = x01;
			fir_buf[i + 0] = x00 - y00;
			fir_buf[i + 1] = x01 - y01;
			i += 2;
		}

		if (i < length) {
			x00 = src_buf[i + 0];
			y00 =
			    z10 * x10 + z9 * x9 + z8 * x8 + z7 * x7 + z6 * x6 +
			    z5 * x5 + z4 * x4 + z3 * x3 + z2 * x2 + z1 * x1;
			x10 = x9;
			x9 = x8;
			x8 = x7;
			x7 = x6;
			x6 = x5;
			x5 = x4;
			x4 = x3;
			x3 = x2;
			x2 = x1;
			x1 = x00;
			fir_buf[i + 0] = x00 - y00;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length - 3; i += 4) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			y02 = fir_buf[i + 2];
			y03 = fir_buf[i + 3];
			x00 =
			    y00 + p10 * y10 + p9 * y9 + p8 * y8 + p7 * y7 +
			    p6 * y6 + p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 +
			    p1 * y1;
			x01 =
			    y01 + p10 * y9 + p9 * y8 + p8 * y7 + p7 * y6 +
			    p6 * y5 + p5 * y4 + p4 * y3 + p3 * y2 + p2 * y1 +
			    p1 * x00;
			x02 =
			    y02 + p10 * y8 + p9 * y7 + p8 * y6 + p7 * y5 +
			    p6 * y4 + p5 * y3 + p4 * y2 + p3 * y1 + p2 * x00 +
			    p1 * x01;
			x03 =
			    y03 + p10 * y7 + p9 * y6 + p8 * y5 + p7 * y4 +
			    p6 * y3 + p5 * y2 + p4 * y1 + p3 * x00 + p2 * x01 +
			    p1 * x02;
			y10 = y6;
			y9 = y5;
			y8 = y4;
			y7 = y3;
			y6 = y2;
			y5 = y1;
			y4 = x00;
			y3 = x01;
			y2 = x02;
			y1 = x03;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			dst_buf[i + 2] = x02;
			dst_buf[i + 3] = x03;
		}

		if (i < length - 1) {
			y00 = fir_buf[i + 0];
			y01 = fir_buf[i + 1];
			x00 =
			    y00 + p10 * y10 + p9 * y9 + p8 * y8 + p7 * y7 +
			    p6 * y6 + p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 +
			    p1 * y1;
			x01 =
			    y01 + p10 * y9 + p9 * y8 + p8 * y7 + p7 * y6 +
			    p6 * y5 + p5 * y4 + p4 * y3 + p3 * y2 + p2 * y1 +
			    p1 * x00;
			y10 = y8;
			y9 = y7;
			y8 = y6;
			y7 = y5;
			y6 = y4;
			y5 = y3;
			y4 = y2;
			y3 = y1;
			y2 = x00;
			y1 = x01;
			dst_buf[i + 0] = x00;
			dst_buf[i + 1] = x01;
			i += 2;
		}

		if (i < length) {
			y00 = fir_buf[i + 0];
			x00 =
			    y00 + p10 * y10 + p9 * y9 + p8 * y8 + p7 * y7 +
			    p6 * y6 + p5 * y5 + p4 * y4 + p3 * y3 + p2 * y2 +
			    p1 * y1;
			y10 = y9;
			y9 = y8;
			y8 = y7;
			y7 = y6;
			y6 = y5;
			y5 = y4;
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = x00;
			dst_buf[i + 0] = x00;
		}

		zero[1] = x1;
		pole[1] = y1;
		zero[2] = x2;
		pole[2] = y2;
		zero[3] = x3;
		pole[3] = y3;
		zero[4] = x4;
		pole[4] = y4;
		zero[5] = x5;
		pole[5] = y5;
		zero[6] = x6;
		pole[6] = y6;
		zero[7] = x7;
		pole[7] = y7;
		zero[8] = x8;
		pole[8] = y8;
		zero[9] = x9;
		pole[9] = y9;
		zero[10] = x10;
		pole[10] = y10;
		break;
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCPerceptWeight_F32(
    mlib_f32 *sigwgt,
    const mlib_f32 *signal,
    const mlib_f32 *lpc,
    mlib_f32 r1,
    mlib_f32 r2,
    void *state)
{
	filter_state *filter = state;
	mlib_s32 order = filter->order;
	mlib_s32 length = filter->length;
	mlib_d64 *ZeroTable = filter->ZeroTable;
	mlib_d64 *PoleTable = filter->PoleTable;
	mlib_d64 *src_buf = filter->src_buf;
	mlib_d64 *fir_buf = filter->fir_buf;
	mlib_d64 *dst_buf = filter->dst_buf;
	mlib_d64 gamma1, gamma2;
	mlib_s32 i, j, k;
	mlib_d64 mul1, mul2;

	if ((state == NULL) || (signal == NULL) || (sigwgt == NULL) ||
	    (lpc == NULL))
		return (MLIB_FAILURE);

	if (order + 1 > length)
		return (MLIB_FAILURE);

	if ((r1 <= 0) || (r1 > 1))
		return (MLIB_FAILURE);

	if ((r2 <= 0) || (r2 > 1))
		return (MLIB_FAILURE);

	gamma1 = (mlib_d64)r1;
	gamma2 = (mlib_d64)r2;

	for (j = 1; j <= order; j++) {
		mul1 = (mlib_d64)lpc[j];
		mul2 = (mlib_d64)lpc[j];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < j; k++) {
			mul1 *= gamma1;
			mul2 *= gamma2;
		}

		ZeroTable[j] = mul1;
		PoleTable[j] = mul2;
	}

	if (order <= 3) {
		PerceptWeightOrderIsVerySmall_F32(sigwgt, signal, filter);
		return (MLIB_SUCCESS);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++) {
		src_buf[i] = (mlib_d64)signal[i];
	}

	if (order <= 10) {
		mlib_PerceptWeightOrderIsSmall(dst_buf, src_buf, filter);
	} else {
		mlib_FIR_part(fir_buf, src_buf, ZeroTable, filter);
		mlib_IIR_part(dst_buf, fir_buf, PoleTable, filter);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++)
		sigwgt[i] = (mlib_f32)dst_buf[i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCPerceptWeightInit_F32(
    void **state,
    mlib_s32 length,
    mlib_s32 order)
{
	filter_state *state_ptr;
	mlib_s32 i, struct_size;
	mlib_d64 *zero, *pole, *ZeroTable, *PoleTable;

/* check for obvious errors */

	if (state == NULL)
		return (MLIB_FAILURE);

	if (order < 1)
		return (MLIB_FAILURE);

	if (order + 1 > length)
		return (MLIB_FAILURE);

/* to be multiple of double */
	struct_size = (sizeof (filter_state) + 7) & ~7;

	state_ptr =
	    (filter_state *) __mlib_malloc(struct_size +
	    (4 * (order + 1) + 3 * length) * sizeof (mlib_d64));

	if (state_ptr == NULL)
		return (MLIB_FAILURE);

	state_ptr->order = order;
	state_ptr->length = length;
	state_ptr->zero = (mlib_d64 *)((mlib_u8 *)state_ptr + struct_size);
	state_ptr->pole = (state_ptr->zero) + (order + 1);
	state_ptr->ZeroTable = (state_ptr->pole) + (order + 1);
	state_ptr->PoleTable = (state_ptr->ZeroTable) + (order + 1);
	state_ptr->src_buf = (state_ptr->PoleTable) + (order + 1);
	state_ptr->fir_buf = (state_ptr->src_buf) + length;
	state_ptr->dst_buf = (state_ptr->fir_buf) + length;

	zero = state_ptr->zero;
	pole = state_ptr->pole;
	ZeroTable = state_ptr->ZeroTable;
	PoleTable = state_ptr->PoleTable;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= order; i++) {
		zero[i] = 0;
		pole[i] = 0;
		ZeroTable[i] = 0;
		PoleTable[i] = 0;
	}

	*state = state_ptr;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalLPCPerceptWeightFree_F32(
    void *state)
{
/* check for obvious errors */

	if (state == NULL)
		return; /* MLIB_FAILURE */

	__mlib_free(state);
}

/* *********************************************************** */
