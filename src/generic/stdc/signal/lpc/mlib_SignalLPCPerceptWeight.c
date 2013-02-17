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

#pragma ident	"@(#)mlib_SignalLPCPerceptWeight.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLPCPerceptWeight_S16  - perceptual weighting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLPCPerceptWeightInit_S16(void **state,
 *                                                      mlib_s32 length,
 *                                                      mlib_s32 order);
 *      mlib_status mlib_SignalLPCPerceptWeight_S16(mlib_s16 *sigwgt,
 *                                                  const mlib_s16 *signal,
 *                                                  const mlib_s16 *lpc,
 *                                                  mlib_s32 lscale,
 *                                                  mlib_s16 r1,
 *                                                  mlib_s16 r2,
 *                                                  void *state);
 *      void mlib_SignalLPCPerceptWeightFree_S16(void *state);
 *
 * ARGUMENTS
 *      sigwgt  the weighted signal vector, the signal
 *              samples are in Q15 format.
 *      signal  the input signal vector, the signal
 *              samples are in Q15 format.
 *      lpc     the linear prediction coefficients.
 *      lscale  the scaling factor of the linear prediction coefficients,
 *              where actual_data = input_data * 2^(-scaling_factor).
 *      r1      the perceptual weighting filter coefficient,
 *              the coefficient are in Q15 format, it is treated
 *              as 1 if 0 is supplied.
 *      r2      the perceptual weighting filter coefficient,
 *              the coefficient are in Q15 format, it is treated
 *              as 1 if 0 is supplied.
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

#include <stdlib.h>
#include <mlib_signal.h>
#include <mlib_SignalLPCPerceptWeight.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPCPerceptWeightFree_S16 = \
	__mlib_SignalLPCPerceptWeightFree_S16
#pragma weak mlib_SignalLPCPerceptWeightInit_S16 = \
	__mlib_SignalLPCPerceptWeightInit_S16
#pragma weak mlib_SignalLPCPerceptWeight_S16 = \
	__mlib_SignalLPCPerceptWeight_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCPerceptWeightFree_S16)
    mlib_SignalLPCPerceptWeightFree_S16
    __attribute__((weak, alias("__mlib_SignalLPCPerceptWeightFree_S16")));
__typeof__(__mlib_SignalLPCPerceptWeightInit_S16)
    mlib_SignalLPCPerceptWeightInit_S16
    __attribute__((weak, alias("__mlib_SignalLPCPerceptWeightInit_S16")));
__typeof__(__mlib_SignalLPCPerceptWeight_S16) mlib_SignalLPCPerceptWeight_S16
    __attribute__((weak, alias("__mlib_SignalLPCPerceptWeight_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)                             \
	DST = ((mlib_s32)((SRC) * (32768.0 * 65536.0)) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)((SRC) * (32768.0));      \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S16_MAX)                  \
		dsrc = (mlib_d64)MLIB_S16_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S16_MIN)                  \
		dsrc = (mlib_d64)MLIB_S16_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

static void
mlib_PerceptWeightOrderIsVerySmall_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    filter_state * filter)
{
	mlib_s32 order = filter->order;
	mlib_s32 length = filter->length;
	mlib_d64 *ZeroTable = filter->ZeroTable;
	mlib_d64 *PoleTable = filter->PoleTable;
	mlib_d64 *zero = filter->zero;
	mlib_d64 *pole = filter->pole;
	mlib_s32 i;
	mlib_d64 z1, z2, z3, z4, z5;
	mlib_d64 p1, p2, p3, p4, p5;
	mlib_d64 x0, x1, x2, x3, x4, x5;
	mlib_d64 y0, y1, y2, y3, y4, y5;
	mlib_d64 tmp, q15 = 1. / 32768;

	if (order == 1) {
		z1 = ZeroTable[1];
		p1 = PoleTable[1];
		x1 = zero[1];
		y1 = pole[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < length; i++) {
			x0 = src[i] * q15;
			y0 = x0 - z1 * x1;
			x1 = x0;

			tmp = y0 + p1 * y1;
			y1 = tmp;
			FLOAT2INT_CLAMP(dst[i], tmp);
		}

		zero[1] = x1;
		pole[1] = y1;
	} else {
		if (order == 2) {
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
				x0 = src[i] * q15;
				y0 = x0 - z2 * x2 - z1 * x1;
				x2 = x1;
				x1 = x0;

				tmp = y0 + p2 * y2 + p1 * y1;
				y2 = y1;
				y1 = tmp;
				FLOAT2INT_CLAMP(dst[i], tmp);
			}

			zero[1] = x1;
			pole[1] = y1;
			zero[2] = x2;
			pole[2] = y2;
		} else {
			if (order == 3) {
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
					x0 = src[i] * q15;
					y0 = x0 - z3 * x3 - z2 * x2 - z1 * x1;
					x3 = x2;
					x2 = x1;
					x1 = x0;

					tmp = y0 + p3 * y3 + p2 * y2 + p1 * y1;
					y3 = y2;
					y2 = y1;
					y1 = tmp;
					FLOAT2INT_CLAMP(dst[i], tmp);
				}

				zero[1] = x1;
				pole[1] = y1;
				zero[2] = x2;
				pole[2] = y2;
				zero[3] = x3;
				pole[3] = y3;
			} else {
				if (order == 4) {
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
					for (i = 0; i < length; i++) {
						x0 = src[i] * q15;
						y0 = x0 - z4 * x4 - z3 * x3 -
						    z2 * x2 - z1 * x1;
						x4 = x3;
						x3 = x2;
						x2 = x1;
						x1 = x0;

						tmp =
						    y0 + p4 * y4 + p3 * y3 +
						    p2 * y2 + p1 * y1;
						y4 = y3;
						y3 = y2;
						y2 = y1;
						y1 = tmp;
						FLOAT2INT_CLAMP(dst[i], tmp);
					}

					zero[1] = x1;
					pole[1] = y1;
					zero[2] = x2;
					pole[2] = y2;
					zero[3] = x3;
					pole[3] = y3;
					zero[4] = x4;
					pole[4] = y4;
				} else {
					if (order == 5) {
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
						for (i = 0; i < length; i++) {
							x0 = src[i] * q15;
							y0 = x0 - z5 * x5 -
							    z4 * x4 - z3 * x3 -
							    z2 * x2 - z1 * x1;
							x5 = x4;
							x4 = x3;
							x3 = x2;
							x2 = x1;
							x1 = x0;

							tmp =
							    y0 + p5 * y5 +
							    p4 * y4 + p3 * y3 +
							    p2 * y2 + p1 * y1;
							y5 = y4;
							y4 = y3;
							y3 = y2;
							y2 = y1;
							y1 = tmp;
							FLOAT2INT_CLAMP(dst[i],
							    tmp);
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
					}
				}
			}
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCPerceptWeight_S16(
    mlib_s16 *sigwgt,
    const mlib_s16 *signal,
    const mlib_s16 *lpc,
    mlib_s32 lscale,
    mlib_s16 r1,
    mlib_s16 r2,
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
	mlib_d64 gamma1, gamma2, scale;
	mlib_s32 i, j, k;
	mlib_d64 mul1, mul2, q15 = 1. / 32768;

	if ((state == NULL) || (signal == NULL) || (sigwgt == NULL) ||
	    (lpc == NULL))
		return (MLIB_FAILURE);

	if (order + 1 > length)
		return (MLIB_FAILURE);

	if ((r1 < 0) || (r2 < 0))
		return (MLIB_FAILURE);

	if (r1 == 0)
		gamma1 = 1;
	else
		gamma1 = (mlib_d64)r1 *q15;

	if (r2 == 0)
		gamma2 = 1;
	else
		gamma2 = (mlib_d64)r2 *q15;

	scale = 1;

	for (i = abs(lscale); i > 15; i -= 16)
		scale *= 65536;

	scale *= (1 << i);

	if (lscale > 0)
		scale = 1 / scale;

	for (j = 1; j <= order; j++) {
		mul1 = (mlib_d64)lpc[j] * scale;
		mul2 = (mlib_d64)lpc[j] * scale;
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

	if (order <= 5) {
		mlib_PerceptWeightOrderIsVerySmall_S16(sigwgt, signal, filter);
		return (MLIB_SUCCESS);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length - 1; i += 2) {
		src_buf[i] = signal[i] * q15;
		src_buf[i + 1] = signal[i + 1] * q15;
	}

	if (i < length) {
		src_buf[i] = signal[i] * q15;
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
	for (i = 0; i < length - 1; i += 2) {
		FLOAT2INT_CLAMP(sigwgt[i], dst_buf[i]);
		FLOAT2INT_CLAMP(sigwgt[i + 1], dst_buf[i + 1]);
	}

	if (i < length) {
		FLOAT2INT_CLAMP(sigwgt[i], dst_buf[i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCPerceptWeightInit_S16(
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

/* to be multiple of mlib_d64 */
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
__mlib_SignalLPCPerceptWeightFree_S16(
    void *state)
{
/* check for obvious errors */

	if (state == NULL)
		return; /* MLIB_FAILURE */

	__mlib_free(state);
}

/* *********************************************************** */
