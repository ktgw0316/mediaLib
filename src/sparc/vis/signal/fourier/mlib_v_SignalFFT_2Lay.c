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

#pragma ident	"@(#)mlib_v_SignalFFT_2Lay.c	9.2	07/11/05 SMI"

#include <mlib_signal.h>
#include <mlib_v_SignalFFT_Tbl.h>
#include <mlib_v_SignalFFTDisp.h>
#include <vis_proto.h>

/* *********************************************************** */

#define	MULT16(d0, d1)                                                \
	vis_fpadd16(vis_fmul8sux16(d0, d1), vis_fmul8ulx16(d0, d1))

/* *********************************************************** */

#define	DEF_CVAR4(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i;                \
	mlib_d64 x##2_r, x##2_i, x##3_r, x##3_i

/* *********************************************************** */

#define	CSCALE(z, x)                                            \
	z##_r = vis_fmul8x16(scale, x##_r);                     \
	z##_i = vis_fmul8x16(scale, x##_i)

/* *********************************************************** */

#define	CMUL(z, x, y)                                           \
	{                                                       \
	    mlib_d64 zr_r, zr_i, zi_r, zi_i;                    \
	                                                        \
	    zr_r = MULT16(x##_r, y##_r);                        \
	    zi_i = MULT16(x##_i, y##_i);                        \
	    zr_i = MULT16(x##_r, y##_i);                        \
	    zi_r = MULT16(x##_i, y##_r);                        \
	    z##_r = vis_fpsub16(zr_r, zi_i);                    \
	    z##_i = vis_fpadd16(zr_i, zi_r);                    \
	}

/* *********************************************************** */

#define	CADD(z, x, y)                                           \
	z##_r = vis_fpadd16(x##_r, y##_r);                      \
	z##_i = vis_fpadd16(x##_i, y##_i)

/* *********************************************************** */

#define	CSUB(z, x, y)                                           \
	z##_r = vis_fpsub16(x##_r, y##_r);                      \
	z##_i = vis_fpsub16(x##_i, y##_i)

/* *********************************************************** */

#define	RSUB(z, x, y)                                           \
	z##_i = vis_fpsub16(x##_r, y##_r);                      \
	z##_r = vis_fpsub16(y##_i, x##_i)

/* *********************************************************** */

#define	ISUB(z, x, y)                                           \
	z##_i = vis_fpsub16(y##_r, x##_r);                      \
	z##_r = vis_fpsub16(x##_i, y##_i)

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
mlib_v_fftK_2Layers_P_Div(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag)
{
	mlib_s32 step = 1 << (layer - 3);
	mlib_s32 mask = step - 1;
	mlib_d64 *tbl_r1 =
	    (div_flag) ? (void *)mlib_v_fftK_WTable_1D_P : (void *)
	    mlib_v_fftK_WTable_1_P;
	mlib_d64 *tbl_r3 =
	    (div_flag) ? (void *)mlib_v_fftK_WTable_3D_P : (void *)
	    mlib_v_fftK_WTable_3_P;
	mlib_d64 *tbl1 = tbl_r1 + 2 * step;
	mlib_d64 *tbl2 = tbl_r1 + 4 * step;
	mlib_d64 *tbl3 = tbl_r3 + 4 * step;
	mlib_d64 *psrc0_r = (mlib_d64 *)data_r;
	mlib_d64 *psrc0_i = (mlib_d64 *)data_i;
	mlib_d64 *psrc1_r = psrc0_r + step;
	mlib_d64 *psrc1_i = psrc0_i + step;
	mlib_d64 *psrc2_r = psrc1_r + step;
	mlib_d64 *psrc2_i = psrc1_i + step;
	mlib_d64 *psrc3_r = psrc2_r + step;
	mlib_d64 *psrc3_i = psrc2_i + step;
	mlib_f32 scale =
	    (div_flag) ? vis_to_float(0x40404040) : vis_to_float(0x80808080);
	mlib_s32 j, ind = 0, old_ind, t_ind;
	mlib_d64 r1_r, r1_i, r2_r, r2_i, r3_r, r3_i;

	DEF_CVAR4(s);
	DEF_CVAR4(a);
	DEF_CVAR4(b);
	DEF_CVAR4(c);

	s3_r = psrc3_r[0];
	s3_i = psrc3_i[0];
	s2_r = psrc2_r[0];
	s2_i = psrc2_i[0];
	s1_r = psrc1_r[0];
	s1_i = psrc1_i[0];
	s0_r = psrc0_r[0];
	s0_i = psrc0_i[0];

	r1_r = tbl1[0];
	r1_i = tbl1[1];
	r2_r = tbl2[0];
	r2_i = tbl2[1];
	r3_r = tbl3[0];
	r3_i = tbl3[1];

/* rotate sources */
	CSCALE(a0, s0);
	CMUL(a1, s1, r1);
	CMUL(a2, s2, r2);
	CMUL(a3, s3, r3);

#pragma unroll(1)
	for (j = 1; j < (1 << (order - 4)); j++) {

		old_ind = ind;
		ind = (j & mask) + ((j & ~mask) << 2);
		s3_r = psrc3_r[ind];
		s3_i = psrc3_i[ind];
		s2_r = psrc2_r[ind];
		s2_i = psrc2_i[ind];
		s1_r = psrc1_r[ind];
		s1_i = psrc1_i[ind];
		s0_r = psrc0_r[ind];
		s0_i = psrc0_i[ind];

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		RSUB(b3, a2, a3);

		t_ind = 2 * (j & mask);
		r1_r = tbl1[t_ind];
		r1_i = tbl1[t_ind + 1];
		r2_r = tbl2[t_ind];
		r2_i = tbl2[t_ind + 1];
		r3_r = tbl3[t_ind];
		r3_i = tbl3[t_ind + 1];

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
		psrc3_r[old_ind] = c3_r;
		psrc3_i[old_ind] = c3_i;
		psrc2_r[old_ind] = c2_r;
		psrc2_i[old_ind] = c2_i;
		psrc1_r[old_ind] = c1_r;
		psrc1_i[old_ind] = c1_i;
		psrc0_r[old_ind] = c0_r;
		psrc0_i[old_ind] = c0_i;
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
	psrc3_r[ind] = c3_r;
	psrc3_i[ind] = c3_i;
	psrc2_r[ind] = c2_r;
	psrc2_i[ind] = c2_i;
	psrc1_r[ind] = c1_r;
	psrc1_i[ind] = c1_i;
	psrc0_r[ind] = c0_r;
	psrc0_i[ind] = c0_i;
}

/* *********************************************************** */

void
mlib_v_fftK_2Layers_M_Div(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag)
{
	mlib_s32 step = 1 << (layer - 3);
	mlib_s32 mask = step - 1;
	mlib_d64 *tbl_r1 =
	    (div_flag) ? (void *)mlib_v_fftK_WTable_1D_M : (void *)
	    mlib_v_fftK_WTable_1_M;
	mlib_d64 *tbl_r3 =
	    (div_flag) ? (void *)mlib_v_fftK_WTable_3D_M : (void *)
	    mlib_v_fftK_WTable_3_M;
	mlib_d64 *tbl1 = tbl_r1 + 2 * step;
	mlib_d64 *tbl2 = tbl_r1 + 4 * step;
	mlib_d64 *tbl3 = tbl_r3 + 4 * step;
	mlib_d64 *psrc0_r = (mlib_d64 *)data_r;
	mlib_d64 *psrc0_i = (mlib_d64 *)data_i;
	mlib_d64 *psrc1_r = psrc0_r + step;
	mlib_d64 *psrc1_i = psrc0_i + step;
	mlib_d64 *psrc2_r = psrc1_r + step;
	mlib_d64 *psrc2_i = psrc1_i + step;
	mlib_d64 *psrc3_r = psrc2_r + step;
	mlib_d64 *psrc3_i = psrc2_i + step;
	mlib_f32 scale =
	    (div_flag) ? vis_to_float(0x40404040) : vis_to_float(0x80808080);
	mlib_s32 j, ind = 0, old_ind, t_ind;
	mlib_d64 r1_r, r1_i, r2_r, r2_i, r3_r, r3_i;

	DEF_CVAR4(s);
	DEF_CVAR4(a);
	DEF_CVAR4(b);
	DEF_CVAR4(c);

	s3_r = psrc3_r[0];
	s3_i = psrc3_i[0];
	s2_r = psrc2_r[0];
	s2_i = psrc2_i[0];
	s1_r = psrc1_r[0];
	s1_i = psrc1_i[0];
	s0_r = psrc0_r[0];
	s0_i = psrc0_i[0];

	r1_r = tbl1[0];
	r1_i = tbl1[1];
	r2_r = tbl2[0];
	r2_i = tbl2[1];
	r3_r = tbl3[0];
	r3_i = tbl3[1];

/* rotate sources */
	CSCALE(a0, s0);
	CMUL(a1, s1, r1);
	CMUL(a2, s2, r2);
	CMUL(a3, s3, r3);

#pragma unroll(1)
	for (j = 1; j < (1 << (order - 4)); j++) {

		old_ind = ind;
		ind = (j & mask) + ((j & ~mask) << 2);
		s3_r = psrc3_r[ind];
		s3_i = psrc3_i[ind];
		s2_r = psrc2_r[ind];
		s2_i = psrc2_i[ind];
		s1_r = psrc1_r[ind];
		s1_i = psrc1_i[ind];
		s0_r = psrc0_r[ind];
		s0_i = psrc0_i[ind];

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		ISUB(b3, a2, a3);

		t_ind = 2 * (j & mask);
		r1_r = tbl1[t_ind];
		r1_i = tbl1[t_ind + 1];
		r2_r = tbl2[t_ind];
		r2_i = tbl2[t_ind + 1];
		r3_r = tbl3[t_ind];
		r3_i = tbl3[t_ind + 1];

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
		psrc3_r[old_ind] = c3_r;
		psrc3_i[old_ind] = c3_i;
		psrc2_r[old_ind] = c2_r;
		psrc2_i[old_ind] = c2_i;
		psrc1_r[old_ind] = c1_r;
		psrc1_i[old_ind] = c1_i;
		psrc0_r[old_ind] = c0_r;
		psrc0_i[old_ind] = c0_i;
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
	psrc3_r[ind] = c3_r;
	psrc3_i[ind] = c3_i;
	psrc2_r[ind] = c2_r;
	psrc2_i[ind] = c2_i;
	psrc1_r[ind] = c1_r;
	psrc1_i[ind] = c1_i;
	psrc0_r[ind] = c0_r;
	psrc0_i[ind] = c0_i;
}

/* *********************************************************** */
