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

#pragma ident	"@(#)mlib_s_SignalFFTMain.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *   Internal FFT functions to process one layer
 *
 */

#include <mlib_signal.h>
#include <mlib_s_sse_int.h>
#include <mlib_s_SignalFFTDisp.h>

/* *********************************************************** */

#if 1	/* implemented on VIS */

void
mlib_s_fftK_Layer_Div2(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    __m128i *tbl)
{
	DEF_ADDER(1);
	mlib_s32 step = 1 << (layer - 4);
	mlib_s32 mask = step - 1;
	__m128i *psrc0_r = (__m128i *)data_r;
	__m128i *psrc0_i = (__m128i *)data_i;
	__m128i *psrc1_r = psrc0_r + step;
	__m128i *psrc1_i = psrc0_i + step;
	mlib_s32 j;

	tbl += 2 * step;

#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < (1 << (order - 4)); j++) {
		mlib_s32 ind, ind_t;
		__m128i b0, b1, b2, b3, b4, b5, b6, b7, b8, b9;

		ind = j + (j & ~mask);
		ind_t = 2 * (j & mask);

		b0 = _mm_loadu_si128(psrc1_r + ind);
		b1 = _mm_loadu_si128(psrc1_i + ind);
		b2 = _mm_loadu_si128(tbl + ind_t);
		b3 = _mm_loadu_si128(tbl + ind_t + 1);
		b4 = _mm_loadu_si128(psrc0_r + ind);
		b5 = _mm_loadu_si128(psrc0_i + ind);

		b4 = _mm_fmul8x16_vis(b4, 1);
		b5 = _mm_fmul8x16_vis(b5, 1);
		b6 = _mm_mulhi_epi16_vis(b0, b2);
		b7 = _mm_mulhi_epi16_vis(b1, b3);
		b2 = _mm_mulhi_epi16_vis(b1, b2);
		b3 = _mm_mulhi_epi16_vis(b0, b3);

		b6 = _mm_sub_epi16(b6, b7);
		b7 = _mm_add_epi16(b3, b2);
		b8 = _mm_add_epi16(b4, b6);
		b6 = _mm_sub_epi16(b4, b6);
		b9 = _mm_add_epi16(b5, b7);
		b7 = _mm_sub_epi16(b5, b7);

		_mm_storeu_si128(psrc0_r + ind, b8);
		_mm_storeu_si128(psrc1_r + ind, b6);
		_mm_storeu_si128(psrc0_i + ind, b9);
		_mm_storeu_si128(psrc1_i + ind, b7);
	}
}

#endif

/* *********************************************************** */

void
mlib_s_fftK_Layer(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    __m128i *tbl)
{
	mlib_s32 step = 1 << (layer - 4);
	mlib_s32 mask = step - 1;
	__m128i *psrc0_r = (__m128i *)data_r;
	__m128i *psrc0_i = (__m128i *)data_i;
	__m128i *psrc1_r = (__m128i *)(psrc0_r + step);
	__m128i *psrc1_i = (__m128i *)(psrc0_i + step);
	mlib_s32 j;

	tbl += 2 * step;

#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < (1 << (order - 4)); j++) {
		mlib_s32 ind;
		__m128i s0_r, s0_i, s1_r, s1_i, t0_r, t0_i;
		__m128i t1_r, t1_i, d0_r, d0_i, d1_r, d1_i;
		__m128i w_r, w_i, dr_r, di_r, di_i, dr_i;

		ind = j + (j & ~mask);

		s1_r = _mm_loadu_si128(psrc1_r + ind);
		s1_i = _mm_loadu_si128(psrc1_i + ind);
		s0_r = _mm_loadu_si128(psrc0_r + ind);
		s0_i = _mm_loadu_si128(psrc0_i + ind);

		w_r = _mm_loadu_si128(tbl + 2 * (j & mask));
		w_i = _mm_loadu_si128(tbl + 2 * (j & mask) + 1);

		dr_r = _mm_mulhi_epi16_vis(s1_r, w_r);
		di_r = _mm_mulhi_epi16_vis(s1_i, w_r);
		di_i = _mm_mulhi_epi16_vis(s1_i, w_i);
		dr_i = _mm_mulhi_epi16_vis(s1_r, w_i);
		t0_r = _mm_sub_epi16(dr_r, di_i);
		t0_i = _mm_add_epi16(dr_i, di_r);

		t1_r = _mm_add_epi16(t0_r, t0_r);
		t1_i = _mm_add_epi16(t0_i, t0_i);

		d0_r = _mm_add_epi16(s0_r, t1_r);
		d1_r = _mm_sub_epi16(s0_r, t1_r);
		d0_i = _mm_add_epi16(s0_i, t1_i);
		d1_i = _mm_sub_epi16(s0_i, t1_i);

		_mm_storeu_si128(psrc0_r + ind, d0_r);
		_mm_storeu_si128(psrc1_r + ind, d1_r);
		_mm_storeu_si128(psrc0_i + ind, d0_i);
		_mm_storeu_si128(psrc1_i + ind, d1_i);
	}
}

/* *********************************************************** */
