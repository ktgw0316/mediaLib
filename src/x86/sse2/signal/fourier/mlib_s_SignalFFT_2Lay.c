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

#pragma ident	"@(#)mlib_s_SignalFFT_2Lay.c	9.5	07/11/05 SMI"

#include <mlib_signal.h>
#include <mlib_s_SignalFFT_Tbl.h>
#include <mlib_s_SignalFFTDisp.h>

/* *********************************************************** */

#define	DEF_CVAR4(x)                                            \
	__m128i x##0_r, x##0_i, x##1_r, x##1_i;                 \
	__m128i x##2_r, x##2_i, x##3_r, x##3_i

/* *********************************************************** */

#define	CSCALE(z, x)                                            \
	z##_r = _mm_fmul8x16_vis(x##_r, scale);                   \
	z##_i = _mm_fmul8x16_vis(x##_i, scale);

/* *********************************************************** */

#define	CMUL(z, x, y)                                           \
	{                                                       \
	    __m128i zr_r, zr_i, zi_r, zi_i;                     \
	                                                        \
	    zr_r = _mm_mulhi_epi16_vis(x##_r, y##_r);               \
	    zi_i = _mm_mulhi_epi16_vis(x##_i, y##_i);               \
	    zr_i = _mm_mulhi_epi16_vis(x##_r, y##_i);               \
	    zi_r = _mm_mulhi_epi16_vis(x##_i, y##_r);               \
	    z##_r = _mm_sub_epi16(zr_r, zi_i);                  \
	    z##_i = _mm_add_epi16(zr_i, zi_r);                  \
	}

/* *********************************************************** */

#define	CADD(z, x, y)                                           \
	z##_r = _mm_add_epi16(x##_r, y##_r);                    \
	z##_i = _mm_add_epi16(x##_i, y##_i)

/* *********************************************************** */

#define	CSUB(z, x, y)                                           \
	z##_r = _mm_sub_epi16(x##_r, y##_r);                    \
	z##_i = _mm_sub_epi16(x##_i, y##_i)

/* *********************************************************** */

#define	RSUB(z, x, y)                                           \
	z##_i = _mm_sub_epi16(x##_r, y##_r);                    \
	z##_r = _mm_sub_epi16(y##_i, x##_i)

/* *********************************************************** */

#define	ISUB(z, x, y)                                           \
	z##_i = _mm_sub_epi16(y##_r, x##_r);                    \
	z##_r = _mm_sub_epi16(x##_i, y##_i)

/* *********************************************************** */

#define	CBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	CSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	RBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	RSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	IBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	ISUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

void
mlib_s_fftK_2Layers_P_Div(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag)
{
	mlib_s32 scale = (div_flag) ? 2 : 1;
	DEF_ADDER(scale);
	mlib_s32 step = 1 << (layer - 4);
	mlib_s32 mask = step - 1;
	__m128i *tbl_r1 =
	    (div_flag) ? (void *)mlib_s_fftK_WTable_1D_P : (void *)
	    mlib_s_fftK_WTable_1_P;
	__m128i *tbl_r3 =
	    (div_flag) ? (void *)mlib_s_fftK_WTable_3D_P : (void *)
	    mlib_s_fftK_WTable_3_P;
	__m128i *tbl1 = tbl_r1 + 2 * step;
	__m128i *tbl2 = tbl_r1 + 4 * step;
	__m128i *tbl3 = tbl_r3 + 4 * step;
	__m128i *psrc0_r = (__m128i *)data_r;
	__m128i *psrc0_i = (__m128i *)data_i;
	__m128i *psrc1_r = psrc0_r + step;
	__m128i *psrc1_i = psrc0_i + step;
	__m128i *psrc2_r = psrc1_r + step;
	__m128i *psrc2_i = psrc1_i + step;
	__m128i *psrc3_r = psrc2_r + step;
	__m128i *psrc3_i = psrc2_i + step;
	mlib_s32 j, ind = 0, old_ind, t_ind;
	__m128i r1_r, r1_i, r2_r, r2_i, r3_r, r3_i;

	DEF_CVAR4(s);
	DEF_CVAR4(a);
	DEF_CVAR4(b);
	DEF_CVAR4(c);

	s3_r = _mm_loadu_si128(psrc3_r);
	s3_i = _mm_loadu_si128(psrc3_i);
	s2_r = _mm_loadu_si128(psrc2_r);
	s2_i = _mm_loadu_si128(psrc2_i);
	s1_r = _mm_loadu_si128(psrc1_r);
	s1_i = _mm_loadu_si128(psrc1_i);
	s0_r = _mm_loadu_si128(psrc0_r);
	s0_i = _mm_loadu_si128(psrc0_i);

	r1_r = _mm_loadu_si128(tbl1);
	r1_i = _mm_loadu_si128(tbl1 + 1);
	r2_r = _mm_loadu_si128(tbl2);
	r2_i = _mm_loadu_si128(tbl2 + 1);
	r3_r = _mm_loadu_si128(tbl3);
	r3_i = _mm_loadu_si128(tbl3 + 1);

/* rotate sources */
	CSCALE(a0, s0);
	CMUL(a1, s1, r1);
	CMUL(a2, s2, r2);
	CMUL(a3, s3, r3);

#pragma unroll(1)
	for (j = 1; j < (1 << (order - 5)); j++) {

		old_ind = ind;
		ind = (j & mask) + ((j & ~mask) << 2);
		s3_r = _mm_loadu_si128(psrc3_r + ind);
		s3_i = _mm_loadu_si128(psrc3_i + ind);
		s2_r = _mm_loadu_si128(psrc2_r + ind);
		s2_i = _mm_loadu_si128(psrc2_i + ind);
		s1_r = _mm_loadu_si128(psrc1_r + ind);
		s1_i = _mm_loadu_si128(psrc1_i + ind);
		s0_r = _mm_loadu_si128(psrc0_r + ind);
		s0_i = _mm_loadu_si128(psrc0_i + ind);

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		RSUB(b3, a2, a3);

		t_ind = 2 * (j & mask);
		r1_r = _mm_loadu_si128(tbl1 + t_ind);
		r1_i = _mm_loadu_si128(tbl1 + t_ind + 1);
		r2_r = _mm_loadu_si128(tbl2 + t_ind);
		r2_i = _mm_loadu_si128(tbl2 + t_ind + 1);
		r3_r = _mm_loadu_si128(tbl3 + t_ind);
		r3_i = _mm_loadu_si128(tbl3 + t_ind + 1);

/* rotate sources */
		CSCALE(a0, s0);
		CMUL(a1, s1, r1);
		CMUL(a2, s2, r2);
		CMUL(a3, s3, r3);

/* second stage */
		CADD(c0, b0, b2);
		CSUB(c2, b0, b2);
		CADD(c1, b1, b3);
		CSUB(c3, b1, b3);

/* store */
		_mm_storeu_si128(psrc3_r + old_ind, c3_r);
		_mm_storeu_si128(psrc3_i + old_ind, c3_i);
		_mm_storeu_si128(psrc2_r + old_ind, c2_r);
		_mm_storeu_si128(psrc2_i + old_ind, c2_i);
		_mm_storeu_si128(psrc1_r + old_ind, c1_r);
		_mm_storeu_si128(psrc1_i + old_ind, c1_i);
		_mm_storeu_si128(psrc0_r + old_ind, c0_r);
		_mm_storeu_si128(psrc0_i + old_ind, c0_i);
	}

/* first stage */
	CADD(b0, a0, a1);
	CSUB(b1, a0, a1);
	CADD(b2, a2, a3);
	RSUB(b3, a2, a3);

/* second stage */
	CADD(c0, b0, b2);
	CSUB(c2, b0, b2);
	CADD(c1, b1, b3);
	CSUB(c3, b1, b3);

/* store */
	_mm_storeu_si128(psrc3_r + ind, c3_r);
	_mm_storeu_si128(psrc3_i + ind, c3_i);
	_mm_storeu_si128(psrc2_r + ind, c2_r);
	_mm_storeu_si128(psrc2_i + ind, c2_i);
	_mm_storeu_si128(psrc1_r + ind, c1_r);
	_mm_storeu_si128(psrc1_i + ind, c1_i);
	_mm_storeu_si128(psrc0_r + ind, c0_r);
	_mm_storeu_si128(psrc0_i + ind, c0_i);
}

/* *********************************************************** */

void
mlib_s_fftK_2Layers_M_Div(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag)
{
	mlib_s32 scale = (div_flag) ? 2 : 1;
	DEF_ADDER(scale);
	mlib_s32 step = 1 << (layer - 4);
	mlib_s32 mask = step - 1;
	__m128i *tbl_r1 =
	    (div_flag) ? (void *)mlib_s_fftK_WTable_1D_M : (void *)
	    mlib_s_fftK_WTable_1_M;
	__m128i *tbl_r3 =
	    (div_flag) ? (void *)mlib_s_fftK_WTable_3D_M : (void *)
	    mlib_s_fftK_WTable_3_M;
	__m128i *tbl1 = tbl_r1 + 2 * step;
	__m128i *tbl2 = tbl_r1 + 4 * step;
	__m128i *tbl3 = tbl_r3 + 4 * step;
	__m128i *psrc0_r = (__m128i *)data_r;
	__m128i *psrc0_i = (__m128i *)data_i;
	__m128i *psrc1_r = psrc0_r + step;
	__m128i *psrc1_i = psrc0_i + step;
	__m128i *psrc2_r = psrc1_r + step;
	__m128i *psrc2_i = psrc1_i + step;
	__m128i *psrc3_r = psrc2_r + step;
	__m128i *psrc3_i = psrc2_i + step;
	mlib_s32 j, ind = 0, old_ind, t_ind;
	__m128i r1_r, r1_i, r2_r, r2_i, r3_r, r3_i;

	DEF_CVAR4(s);
	DEF_CVAR4(a);
	DEF_CVAR4(b);
	DEF_CVAR4(c);

	s3_r = _mm_loadu_si128(psrc3_r);
	s3_i = _mm_loadu_si128(psrc3_i);
	s2_r = _mm_loadu_si128(psrc2_r);
	s2_i = _mm_loadu_si128(psrc2_i);
	s1_r = _mm_loadu_si128(psrc1_r);
	s1_i = _mm_loadu_si128(psrc1_i);
	s0_r = _mm_loadu_si128(psrc0_r);
	s0_i = _mm_loadu_si128(psrc0_i);

	r1_r = _mm_loadu_si128(tbl1);
	r1_i = _mm_loadu_si128(tbl1 + 1);
	r2_r = _mm_loadu_si128(tbl2);
	r2_i = _mm_loadu_si128(tbl2 + 1);
	r3_r = _mm_loadu_si128(tbl3);
	r3_i = _mm_loadu_si128(tbl3 + 1);

/* rotate sources */
	CSCALE(a0, s0);
	CMUL(a1, s1, r1);
	CMUL(a2, s2, r2);
	CMUL(a3, s3, r3);

#pragma unroll(1)
	for (j = 1; j < (1 << (order - 5)); j++) {

		old_ind = ind;
		ind = (j & mask) + ((j & ~mask) << 2);
		s3_r = _mm_loadu_si128(psrc3_r + ind);
		s3_i = _mm_loadu_si128(psrc3_i + ind);
		s2_r = _mm_loadu_si128(psrc2_r + ind);
		s2_i = _mm_loadu_si128(psrc2_i + ind);
		s1_r = _mm_loadu_si128(psrc1_r + ind);
		s1_i = _mm_loadu_si128(psrc1_i + ind);
		s0_r = _mm_loadu_si128(psrc0_r + ind);
		s0_i = _mm_loadu_si128(psrc0_i + ind);

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		ISUB(b3, a2, a3);

		t_ind = 2 * (j & mask);
		r1_r = _mm_loadu_si128(tbl1 + t_ind);
		r1_i = _mm_loadu_si128(tbl1 + t_ind + 1);
		r2_r = _mm_loadu_si128(tbl2 + t_ind);
		r2_i = _mm_loadu_si128(tbl2 + t_ind + 1);
		r3_r = _mm_loadu_si128(tbl3 + t_ind);
		r3_i = _mm_loadu_si128(tbl3 + t_ind + 1);

/* rotate sources */
		CSCALE(a0, s0);
		CMUL(a1, s1, r1);
		CMUL(a2, s2, r2);
		CMUL(a3, s3, r3);

/* second stage */
		CADD(c0, b0, b2);
		CSUB(c2, b0, b2);
		CADD(c1, b1, b3);
		CSUB(c3, b1, b3);

/* store */
		_mm_storeu_si128(psrc3_r + old_ind, c3_r);
		_mm_storeu_si128(psrc3_i + old_ind, c3_i);
		_mm_storeu_si128(psrc2_r + old_ind, c2_r);
		_mm_storeu_si128(psrc2_i + old_ind, c2_i);
		_mm_storeu_si128(psrc1_r + old_ind, c1_r);
		_mm_storeu_si128(psrc1_i + old_ind, c1_i);
		_mm_storeu_si128(psrc0_r + old_ind, c0_r);
		_mm_storeu_si128(psrc0_i + old_ind, c0_i);
	}

/* first stage */
	CADD(b0, a0, a1);
	CSUB(b1, a0, a1);
	CADD(b2, a2, a3);
	ISUB(b3, a2, a3);

/* second stage */
	CADD(c0, b0, b2);
	CSUB(c2, b0, b2);
	CADD(c1, b1, b3);
	CSUB(c3, b1, b3);

/* store */
	_mm_storeu_si128(psrc3_r + ind, c3_r);
	_mm_storeu_si128(psrc3_i + ind, c3_i);
	_mm_storeu_si128(psrc2_r + ind, c2_r);
	_mm_storeu_si128(psrc2_i + ind, c2_i);
	_mm_storeu_si128(psrc1_r + ind, c1_r);
	_mm_storeu_si128(psrc1_i + ind, c1_i);
	_mm_storeu_si128(psrc0_r + ind, c0_r);
	_mm_storeu_si128(psrc0_i + ind, c0_i);
}

/* *********************************************************** */
