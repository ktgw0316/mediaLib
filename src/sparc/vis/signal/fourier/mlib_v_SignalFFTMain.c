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

#pragma ident	"@(#)mlib_v_SignalFFTMain.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *   Internal FFT functions to process one layer
 *
 */

#include <mlib_signal.h>
#include <vis_proto.h>
#include <mlib_v_SignalFFTDisp.h>

/* *********************************************************** */

#define	MULT16(d0, d1)                                                \
	vis_fpadd16(vis_fmul8sux16(d0, d1), vis_fmul8ulx16(d0, d1))

/* *********************************************************** */

#if 0	/* implemented on VIS */

void
mlib_v_fftK_Layer_Div2(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_d64 *tbl)
{
	mlib_s32 step = 1 << (layer - 3);
	mlib_s32 mask = step - 1;
	mlib_d64 *psrc0_r = (mlib_d64 *)data_r;
	mlib_d64 *psrc0_i = (mlib_d64 *)data_i;
	mlib_d64 *psrc1_r = psrc0_r + step;
	mlib_d64 *psrc1_i = psrc0_i + step;
	const mlib_f32 scale = vis_to_float(0x80808080);
	mlib_s32 j;

	tbl += 2 * step;

#pragma pipeloop(0)
	for (j = 0; j < (1 << (order - 3)); j++) {
		mlib_s32 ind, ind_t;
		mlib_d64 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9;
		mlib_d64 T2;

		ind = j + (j & ~mask);
		ind_t = 2 * (j & mask);

		b0 = psrc1_r[ind];
		b1 = psrc1_i[ind];
		b2 = tbl[ind_t];
		b3 = tbl[ind_t + 1];
		b4 = psrc0_r[ind];
		b5 = psrc0_i[ind];

		b8 = vis_fmul8sux16(b0, b2);
		b6 = vis_fmul8ulx16(b0, b2);
		b9 = vis_fmul8sux16(b1, b3);
		b7 = vis_fmul8ulx16(b1, b3);
		T2 = vis_fmul8sux16(b1, b2);
		b2 = vis_fmul8ulx16(b1, b2);
		b1 = vis_fmul8sux16(b0, b3);
		b3 = vis_fmul8ulx16(b0, b3);
		b4 = vis_fmul8x16(scale, b4);
		b5 = vis_fmul8x16(scale, b5);
		b6 = vis_fpadd16(b6, b8);
		b7 = vis_fpadd16(b7, b9);
		b2 = vis_fpadd16(b2, T2);
		b3 = vis_fpadd16(b3, b1);

		b6 = vis_fpsub16(b6, b7);
		b7 = vis_fpadd16(b3, b2);
		b8 = vis_fpadd16(b4, b6);
		b6 = vis_fpsub16(b4, b6);
		b9 = vis_fpadd16(b5, b7);
		b7 = vis_fpsub16(b5, b7);

		psrc0_r[ind] = b8;
		psrc1_r[ind] = b6;
		psrc0_i[ind] = b9;
		psrc1_i[ind] = b7;
	}
}

#endif

/* *********************************************************** */

void
mlib_v_fftK_Layer(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_d64 *tbl)
{
	mlib_s32 step = 1 << (layer - 3);
	mlib_s32 mask = step - 1;
	mlib_d64 *psrc0_r = (mlib_d64 *)data_r;
	mlib_d64 *psrc0_i = (mlib_d64 *)data_i;
	mlib_d64 *psrc1_r = psrc0_r + step;
	mlib_d64 *psrc1_i = psrc0_i + step;
	mlib_s32 j;

	tbl += 2 * step;

#pragma pipeloop(0)
	for (j = 0; j < (1 << (order - 3)); j++) {
		mlib_s32 ind;
		mlib_d64 s0_r, s0_i, s1_r, s1_i, t0_r, t0_i;
		mlib_d64 t1_r, t1_i, d0_r, d0_i, d1_r, d1_i;
		mlib_d64 w_r, w_i, dr_r, di_r, di_i, dr_i;

		ind = j + (j & ~mask);

		s1_r = psrc1_r[ind];
		s1_i = psrc1_i[ind];
		s0_r = psrc0_r[ind];
		s0_i = psrc0_i[ind];

		w_r = tbl[2 * (j & mask)];
		w_i = tbl[2 * (j & mask) + 1];

		dr_r = MULT16(s1_r, w_r);
		di_r = MULT16(s1_i, w_r);
		di_i = MULT16(s1_i, w_i);
		dr_i = MULT16(s1_r, w_i);
		t0_r = vis_fpsub16(dr_r, di_i);
		t0_i = vis_fpadd16(dr_i, di_r);

		t1_r = vis_fpadd16(t0_r, t0_r);
		t1_i = vis_fpadd16(t0_i, t0_i);

		d0_r = vis_fpadd16(s0_r, t1_r);
		d1_r = vis_fpsub16(s0_r, t1_r);
		d0_i = vis_fpadd16(s0_i, t1_i);
		d1_i = vis_fpsub16(s0_i, t1_i);

		psrc0_r[ind] = d0_r;
		psrc1_r[ind] = d1_r;
		psrc0_i[ind] = d0_i;
		psrc1_i[ind] = d1_i;
	}
}

/* *********************************************************** */
