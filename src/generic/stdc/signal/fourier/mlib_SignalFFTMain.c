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

#pragma ident	"@(#)mlib_SignalFFTMain.c	9.3	07/10/09 SMI"

/* *********************************************************** */

#include <mlib_signal.h>
#include <mlib_SysMath.h>	/* for fabs */

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

/* macros, typedefs */

/*
 *    macros for VeryLast2Layers routine
 *    using "all_ors" to accumulate estimates
 */

#define	ST_ORS(smth)	(all_ors |= (mlib_s32)mlib_fabs(smth))

/* *********************************************************** */

#define	BUTTERFLY_CMPLX_CMPLX_L(dest1, dest2, src1, src2)       \
	{                                                       \
	    ST_ORS(dest1->r = src1##_r + src2##_r);             \
	    ST_ORS(dest1->i = src1##_i + src2##_i);             \
	    ST_ORS(dest2->r = src1##_r - src2##_r);             \
	    ST_ORS(dest2->i = src1##_i - src2##_i);             \
	}

/* *********************************************************** */

#define	STORE_SIMPLE_BUTTERFLIES_L(dst, src)                    \
	BUTTERFLY_CMPLX_CMPLX_L(dst##_0, dst##_2,               \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX_L(dst##_1, dst##_3,               \
	    src##_0_mns_1, src##_2_mns_3)

/* *********************************************************** */

#define	STORE_USUAL_BUTTERFLIES_L(dst, tag)                     \
	ST_ORS(dst##_0->r = aaa0_##tag + bbb0_##tag);           \
	ST_ORS(dst##_0->i = aaa2_##tag + bbb2_##tag);           \
	ST_ORS(dst##_1->r = aaa1_##tag - bbb3_##tag);           \
	ST_ORS(dst##_1->i = aaa3_##tag - bbb1_##tag);           \
	ST_ORS(dst##_2->r = aaa0_##tag - bbb0_##tag);           \
	ST_ORS(dst##_2->i = aaa2_##tag - bbb2_##tag);           \
	ST_ORS(dst##_3->r = aaa1_##tag + bbb3_##tag);           \
	ST_ORS(dst##_3->i = aaa3_##tag + bbb1_##tag)

/* *********************************************************** */

#define	DEAL_4_SIMPLE_L(ptr_beg, tagstep, iSIGNed)              \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                 \
	DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES_L(d0, c0)

/* *********************************************************** */

#define	DEAL_4_HALFWAY_L(ptr_beg, tagstep, iSIGNed)             \
	DCL_4_CMPLX_PTRS(d2, ptr_beg, tagstep);                 \
	DCL_CALC_HALFWAY_BUTT(d2, 2, iSIGNed);                  \
	STORE_USUAL_BUTTERFLIES_L(d2, 2)

/* *********************************************************** */

#define	DEAL_8_USUAL_MIRR_L(ptr_beg, ptr_mirr, tagstep, rot,         \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS(d1, ptr_mirr, tagstep);                     \
	DCL_CALC_NORMAL_BUTT(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_MIRROR_BUTT(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_L(d0, 0);                            \
	STORE_USUAL_BUTTERFLIES_L(d1, 1)

/* *********************************************************** */

#define	DEAL_ALL_ORD4_L(isign, ptr_beg, iSIGNed)                         \
	{                                                                \
	    DEAL_4_SIMPLE_L(ptr_beg, 4, iSIGNed);                        \
	}                                                                \
	                                                                 \
	{                                                                \
	    DEAL_4_HALFWAY_L(ptr_beg + 2, 4, iSIGNed);                   \
	}                                                                \
	                                                                 \
	{                                                                \
	    CONST precise_t rot_r = SIN_PI_BY4, rot_i =                  \
		iSIGNed(SIN_PI_BY4), rot_half_r =                        \
		COS_PI_BY8, rot_half_i = iSIGNed(SIN_PI_BY8), r_rh_r =   \
		SIN_PI_BY8, r_rh_i = iSIGNed(COS_PI_BY8);                \
	    DEAL_8_USUAL_MIRR_L(ptr_beg + 1, ptr_beg + 3, 4, rot,        \
		rot_half, r_rh, iSIGNed);                                \
	}

/* *********************************************************** */

mlib_s32
mlib_fftK_2LayersVeryLast_0(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign)
{
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;
	CONST mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	mlib_s32 all_ors = 0;

	if (isign == MORE_ZERO_ISIGN) {
		DEAL_4_SIMPLE_L(dat, n_step, AS_IS);
		{
			DEAL_4_HALFWAY_L(dat + n_step_half, n_step, AS_IS);
		}

	} else {
		DEAL_4_SIMPLE_L(dat, n_step, MINUS);
		{
			DEAL_4_HALFWAY_L(dat + n_step_half, n_step, MINUS);
		}

	}
	return (all_ors);
}

mlib_s32
mlib_fftK_2LayersVeryLast_i(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign,
    mlib_s32 iii0,
    mlib_s32 iii1,
    mlib_d64 rot_r,
    mlib_d64 rot_i,
    mlib_d64 rot_half_r,
    mlib_d64 rot_half_i)
{
/* by the way searches the most significant bit of values to normalize them */
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	precise_t wtmp;
	mlib_s32 iii;

/* scale accumulator */
	mlib_s32 all_ors;

	if (isign == MORE_ZERO_ISIGN) {
		for (iii = iii0; iii < iii1; iii++) {
			DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, AS_IS);
			ROTATE_CMPLX_TMP(rot, incr, wtmp);
			ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
		}
	} else {
		for (iii = iii0; iii < iii1; iii++) {
			DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, MINUS);
			ROTATE_CMPLX_TMP(rot, incr, wtmp);
			ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
		}

	}

	return (all_ors);
}

/* *********************************************************** */
