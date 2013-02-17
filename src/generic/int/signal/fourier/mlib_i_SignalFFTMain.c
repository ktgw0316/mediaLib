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

#pragma ident	"@(#)mlib_i_SignalFFTMain.c	9.3	07/11/05 SMI"

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#ifdef _NO_LONGLONG

/* *********************************************************** */

/* KERNEL */

/* *********************************************************** */

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_c_fftKernel_4          - kernel FFT function, performs
 *                                    direct and inverse FFT's on complex
 *                                    64bit-aligned array which is both source
 *                                    and destination
 *      mlib_c_fftK_2LayersVeryLast - same as mlib_fftK_2LayersLast, with
 *                                    counting scale by the way
 *
 *
 * SYNOPSIS
 *
 *      mlib_s32 mlib_c_fftKernel_4(deal_t *CONST  pal64data,
 *                                  mlib_s32 CONST order,
 *                                  mlib_s32 CONST isig);
 *
 * ARGUMENTS
 *      pal64data
 *              - 64bit-aligned complex array. Created dynamically inside
 *                mlib_c_fftDispatcherW ( from stack or __mlib_malloc ).
 *                Destination for mlib_c_fftDispatcherWGetData.
 *                Source for mlib_c_fftDispatcherWPutData.
 *                Source and destination for kernel FFT function.
 *                pal64data[ 2 * i ] hold the real parts, and
 *                pal64data[ 2 * i + 1 ] hold the imaginary parts
 *
 *      order   - Order of the transformation. The base-2 logarithm of the
 *                number of data samples
 *
 *      isign   - Controls type of the transformation - direct or inverse.
 *                Either DIRECT_FFT or INVERS_FFT values.
 *
 *      dat     - Same as pal64data
 *
 *      dat0    - Same as pal64data
 *
 *      n_ord_arr
 *              - Same as order
 *
 *      n_ord_lay
 *              - Number of currently processing layer
 *
 *      n_ord_chunk
 *              - The base-2 logarithm of the size of data-array chunk
 *
 *      n_nmb_chunk
 *              - Number of currently processing chunk of data array
 *
 *
 * DESCRIPTION
 *
 *      data = FFT|InverseFFT( data, order, isign )
 */

/* *********************************************************** */

/*
 * IMPLEMENTATION
 *
 * Here are some features differed from 'standard' FFT codes:
 *  1)Layers are processed simultaneously by two while it possible
 *    ( i.e. excluding last layer when order is odd )
 *    This decreases arithmetic and load/store operations
 *  2)Separate processing for first stages in main loops inside of layers,
 *    using that 'rotation' is quite simple there ( multiples of PI/2 )
 *    avoiding a lot of multiplications
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>	/* for fabs */

#define	FFT_UTIL_C

#include <mlib_i_SignalFFTUtil.h>

/* *********************************************************** */

/* function prototypes */

static mlib_s32 mlib_c_fftK_2LayersVeryLast(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign);

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

#define	FIND_SCALEF(result, all_ors)                            \
	{                                                       \
	    mlib_s32 cntr = 30;                                 \
	                                                        \
	    while (cntr)                                        \
		if (all_ors & (1 << cntr))                      \
		    break;                                      \
		else                                            \
		    cntr--;                                     \
	    result = 14 - cntr;                                 \
	}

/* *********************************************************** */

/* functions codes */

/* *********************************************************** */

/* KERNEL SERVICES */

/* *********************************************************** */

static mlib_s32
mlib_c_fftK_2LayersVeryLast(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign)
{
/* by the way searches max significant bit of values to normalize them */
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;
	CONST mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

/* scale accumulator */
	mlib_s32 all_ors = 0;

	if (isign == MORE_ZERO_ISIGN) {
		DEAL_4_SIMPLE_L(dat, n_step, AS_IS);
		{
			DEAL_4_HALFWAY_L(dat + n_step_half, n_step, AS_IS);
		}

		for (iii = 1; iii < n_step_half; iii++) {
			DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, AS_IS);
			ROTATE_CMPLX_TMP(rot, incr, wtmp);
			ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
		}
	} else {
/* if( isign != MORE_ZERO_ISIGN ) */
		DEAL_4_SIMPLE_L(dat, n_step, MINUS);
		{
			DEAL_4_HALFWAY_L(dat + n_step_half, n_step, MINUS);
		}

		for (iii = 1; iii < n_step_half; iii++) {
			DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, MINUS);
			ROTATE_CMPLX_TMP(rot, incr, wtmp);
			ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
		}

	}	/* if/else( isign == MORE_ZERO_ISIGN ) */

	FIND_SCALEF(iii, all_ors);
	return (iii);
}

/* *********************************************************** */

/* KERNEL MASTER */

/* *********************************************************** */

mlib_s32
mlib_c_fftKernel_4(
    deal_t *CONST pal64data,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	CONST mlib_s32 n_ord_arr = order;
	mlib_s32 n_ret_scale = 0;
	dtCmplx *CONST dat = (dtCmplx *) pal64data;

	if (n_ord_arr > 4) {
		mlib_s32 i, ord_blk = n_ord_arr - 2;

		if (n_ord_arr >= 4 + 2)
			for (i = 0; i < (1 << 2); i++) {
				mlib_c_fftKernel((void *)(dat + (i << ord_blk)),
				    ord_blk, isign);
			}

		n_ret_scale =
		    mlib_c_fftK_2LayersVeryLast(dat, n_ord_arr, isign);
	} else if (n_ord_arr == 4) {
/* accumulator */
		mlib_s32 all_ors = 0;

		if (isign == MORE_ZERO_ISIGN) {
			DEAL_ALL_ORD4_L(isign, dat, AS_IS);
		} else {
			DEAL_ALL_ORD4_L(isign, dat, MINUS);
		}

		FIND_SCALEF(n_ret_scale, all_ors);
	}

	return (n_ret_scale);
}

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#else

/* *********************************************************** */

/* KERNEL */

/* *********************************************************** */

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_c_fftKernel_4          - kernel FFT function, performs
 *                                    direct and inverse FFT's on complex
 *                                    64bit-aligned array which is both source
 *                                    and destination
 *      mlib_c_fftK_2LayersVeryLast - same as mlib_fftK_2LayersLast, with
 *                                    counting scale by the way
 *
 *
 * SYNOPSIS
 *
 *      mlib_s32 mlib_c_fftKernel_4(deal_t *CONST  pal64data,
 *                                  mlib_s32 CONST order,
 *                                  mlib_s32 CONST isig);
 *
 * ARGUMENTS
 *      pal64data
 *              - 64bit-aligned complex array. Created dynamically inside
 *                mlib_c_fftDispatcherW ( from stack or __mlib_malloc ).
 *                Destination for mlib_c_fftDispatcherWGetData.
 *                Source for mlib_c_fftDispatcherWPutData.
 *                Source and destination for kernel FFT function.
 *                pal64data[ 2 * i ] hold the real parts, and
 *                pal64data[ 2 * i + 1 ] hold the imaginary parts
 *
 *      order   - Order of the transformation. The base-2 logarithm of the
 *                number of data samples
 *
 *      isign   - Controls type of the transformation - direct or inverse.
 *                Either DIRECT_FFT or INVERS_FFT values.
 *
 *      dat     - Same as pal64data
 *
 *      dat0    - Same as pal64data
 *
 *      n_ord_arr
 *              - Same as order
 *
 *      n_ord_lay
 *              - Number of currently processing layer
 *
 *      n_ord_chunk
 *              - The base-2 logarithm of the size of data-array chunk
 *
 *      n_nmb_chunk
 *              - Number of currently processing chunk of data array
 *
 *
 * DESCRIPTION
 *
 *      data = FFT|InverseFFT( data, order, isign )
 */

/* *********************************************************** */

/*
 * IMPLEMENTATION
 *
 * Here are some features differed from 'standard' FFT codes:
 *  1)Layers are processed simultaneously by two while it possible
 *    ( i.e. excluding last layer when order is odd )
 *    This decreases arithmetic and load/store operations
 *  2)Separate processing for first stages in main loops inside of layers,
 *    using that 'rotation' is quite simple there ( multiples of PI/2 )
 *    avoiding a lot of multiplications
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>	/* for fabs */

#define	FFT_UTIL_C

#include <mlib_i_SignalFFTUtil.h>

/* *********************************************************** */

/* function prototypes */

static mlib_s32 mlib_c_fftK_2LayersVeryLast(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign);

/* *********************************************************** */

/* macros, typedefs */

/*
 *    macros for VeryLast2Layers routine
 *    using "all_ors" to accumulate estimates
 */

#define	ST_ORS(smth)	(all_ors |= (mlib_s32)mlib_fabs(smth))

#define	ST_ORS_i(smth)	(all_ors |= (mlib_s64)mlib_abs(smth))

/* *********************************************************** */

#define	BUTTERFLY_CMPLX_CMPLX_L(dest1, dest2, src1, src2)       \
	{                                                       \
	    ST_ORS(dest1->r = src1##_r + src2##_r);             \
	    ST_ORS(dest1->i = src1##_i + src2##_i);             \
	    ST_ORS(dest2->r = src1##_r - src2##_r);             \
	    ST_ORS(dest2->i = src1##_i - src2##_i);             \
	}

#define	BUTTERFLY_CMPLX_CMPLX_L_i(dest1, dest2, src1, src2)       \
	{                                                       \
	    ST_ORS_i(dest1->r = src1##_r + src2##_r);             \
	    ST_ORS_i(dest1->i = src1##_i + src2##_i);             \
	    ST_ORS_i(dest2->r = src1##_r - src2##_r);             \
	    ST_ORS_i(dest2->i = src1##_i - src2##_i);             \
	}

/* *********************************************************** */

#define	STORE_SIMPLE_BUTTERFLIES_L(dst, src)                    \
	BUTTERFLY_CMPLX_CMPLX_L(dst##_0, dst##_2,               \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX_L(dst##_1, dst##_3,               \
	    src##_0_mns_1, src##_2_mns_3)

#define	STORE_SIMPLE_BUTTERFLIES_L_i(dst, src)                    \
	BUTTERFLY_CMPLX_CMPLX_L_i(dst##_0, dst##_2,               \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX_L_i(dst##_1, dst##_3,               \
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

#define	STORE_USUAL_BUTTERFLIES_L_i(dst, tag)                     \
	ST_ORS_i(dst##_0->r = aaa0_##tag + bbb0_##tag);           \
	ST_ORS_i(dst##_0->i = aaa2_##tag + bbb2_##tag);           \
	ST_ORS_i(dst##_1->r = aaa1_##tag - bbb3_##tag);           \
	ST_ORS_i(dst##_1->i = aaa3_##tag - bbb1_##tag);           \
	ST_ORS_i(dst##_2->r = aaa0_##tag - bbb0_##tag);           \
	ST_ORS_i(dst##_2->i = aaa2_##tag - bbb2_##tag);           \
	ST_ORS_i(dst##_3->r = aaa1_##tag + bbb3_##tag);           \
	ST_ORS_i(dst##_3->i = aaa3_##tag + bbb1_##tag)

/* *********************************************************** */

#define	DEAL_4_SIMPLE_L(ptr_beg, tagstep, iSIGNed)              \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                 \
	DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES_L(d0, c0)

#define	DEAL_4_SIMPLE_L_i(ptr_beg, tagstep, iSIGNed)              \
	DCL_4_CMPLX_PTRS_i(d0, ptr_beg, tagstep);                 \
	DCL_CALC_SIMPLE_BUTT_i(c0, d0, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES_L_i(d0, c0)

/* *********************************************************** */

#define	DEAL_4_HALFWAY_L(ptr_beg, tagstep, iSIGNed)             \
	DCL_4_CMPLX_PTRS(d2, ptr_beg, tagstep);                 \
	DCL_CALC_HALFWAY_BUTT(d2, 2, iSIGNed);                  \
	STORE_USUAL_BUTTERFLIES_L(d2, 2)

#define	DEAL_4_HALFWAY_L_i(ptr_beg, tagstep, iSIGNed)             \
	DCL_4_CMPLX_PTRS_i(d2, ptr_beg, tagstep);                 \
	DCL_CALC_HALFWAY_BUTT_i(d2, 2, iSIGNed);                  \
	STORE_USUAL_BUTTERFLIES_L_i(d2, 2)

/* *********************************************************** */

#define	DEAL_8_USUAL_MIRR_L(ptr_beg, ptr_mirr, tagstep, rot,         \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS(d1, ptr_mirr, tagstep);                     \
	DCL_CALC_NORMAL_BUTT(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_MIRROR_BUTT(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_L(d0, 0);                            \
	STORE_USUAL_BUTTERFLIES_L(d1, 1)

#define	DEAL_8_USUAL_MIRR_L_i(ptr_beg, ptr_mirr, tagstep, rot,         \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS_i(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS_i(d1, ptr_mirr, tagstep);                     \
	DCL_CALC_NORMAL_BUTT_i(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_MIRROR_BUTT_i(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_L_i(d0, 0);                            \
	STORE_USUAL_BUTTERFLIES_L_i(d1, 1)

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

#define	DEAL_ALL_ORD4_L_i(isign, ptr_beg, iSIGNed)                         \
	{                                                                \
	    DEAL_4_SIMPLE_L_i(ptr_beg, 4, iSIGNed);                        \
	}                                                                \
	                                                                 \
	{                                                                \
	    DEAL_4_HALFWAY_L_i(ptr_beg + 2, 4, iSIGNed);                   \
	}                                                                \
	                                                                 \
	{                                                                \
	    CONST mlib_s64 rot_r = SIN_PI_BY4_i_Q, rot_i =                 \
		iSIGNed(SIN_PI_BY4_i_Q), rot_half_r =                      \
		COS_PI_BY8_i_Q, rot_half_i = iSIGNed(SIN_PI_BY8_i_Q),	   \
		r_rh_r = SIN_PI_BY8_i_Q, r_rh_i = iSIGNed(COS_PI_BY8_i_Q); \
	    DEAL_8_USUAL_MIRR_L_i(ptr_beg + 1, ptr_beg + 3, 4, rot,        \
		rot_half, r_rh, iSIGNed);                                  \
	}

/* *********************************************************** */

#define	FIND_SCALEF(result, all_ors)                            \
	{                                                       \
	    mlib_s32 cntr = 30;                                 \
	                                                        \
	    while (cntr)                                        \
		if (all_ors & (1 << cntr))                      \
		    break;                                      \
		else                                            \
		    cntr--;                                     \
	    result = 14 - cntr;                                 \
	}

#define	FIND_SCALEF_i(result, all_ors)               \
	{                                                       \
	    mlib_s32 cntr = 30 + 31;                   \
	    while (cntr)                                        \
		if (all_ors & (((mlib_s64)1) << cntr))                      \
		    break;                                      \
		else                                            \
		    cntr--;                                     \
	    result = 14 - cntr;                                 \
	}

/* *********************************************************** */

/* functions codes */

/* *********************************************************** */

/* KERNEL SERVICES */

/* *********************************************************** */

static mlib_s32
mlib_c_fftK_2LayersVeryLast(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign)
{
/* by the way searches max significant bit of values to normalize them */
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;
	CONST mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

/* scale accumulator */
	mlib_s32 all_ors = 0;

	if (isign == MORE_ZERO_ISIGN) {
		DEAL_4_SIMPLE_L(dat, n_step, AS_IS);
		{
			DEAL_4_HALFWAY_L(dat + n_step_half, n_step, AS_IS);
		}

		for (iii = 1; iii < n_step_half; iii++) {
			DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, AS_IS);
			ROTATE_CMPLX_TMP(rot, incr, wtmp);
			ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
		}
	} else {
/* if( isign != MORE_ZERO_ISIGN ) */
		DEAL_4_SIMPLE_L(dat, n_step, MINUS);
		{
			DEAL_4_HALFWAY_L(dat + n_step_half, n_step, MINUS);
		}

		for (iii = 1; iii < n_step_half; iii++) {
			DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, MINUS);
			ROTATE_CMPLX_TMP(rot, incr, wtmp);
			ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
		}

	}	/* if/else( isign == MORE_ZERO_ISIGN ) */

	FIND_SCALEF(iii, all_ors);
	return (iii);
}

static mlib_s32
mlib_c_fftK_2LayersVeryLast_i(
    dtCmplx_i * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign)
{
/* by the way searches max significant bit of values to normalize them */
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;
	CONST mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234_i(n_step, n_ord_lay);
	DCL_CALC_INCR_i(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR_i(incr, n_ord_lay, 1);
	DCL_INIT_ROT_i(rot, incr);
	DCL_INIT_ROT_i(rot_half, incr_half);
	mlib_s64 wtmp;
	mlib_s32 iii;

/* scale accumulator */
	mlib_s64 all_ors = 0;

	if (isign == MORE_ZERO_ISIGN) {
		DEAL_4_SIMPLE_L_i(dat, n_step, AS_IS_i);
		{
			DEAL_4_HALFWAY_L_i(dat + n_step_half, n_step, AS_IS_i);
		}

		for (iii = 1; iii < n_step_half; iii++) {
			DCL_MULT_ROT_ROT_HALF_i(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L_i(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, AS_IS_i);
			ROTATE_CMPLX_TMP_i(rot, incr, wtmp);
			ROTATE_CMPLX_TMP_i(rot_half, incr_half, wtmp);
		}
	} else {
/* if( isign != MORE_ZERO_ISIGN ) */
		DEAL_4_SIMPLE_L_i(dat, n_step, MINUS_i);
		{
			DEAL_4_HALFWAY_L_i(dat + n_step_half, n_step, MINUS_i);
		}

		for (iii = 1; iii < n_step_half; iii++) {
			DCL_MULT_ROT_ROT_HALF_i(rot, rot_half, r_rh);
			DEAL_8_USUAL_MIRR_L_i(dat + iii, dat + n_step - iii,
			    n_step, rot, rot_half, r_rh, MINUS_i);
			ROTATE_CMPLX_TMP_i(rot, incr, wtmp);
			ROTATE_CMPLX_TMP_i(rot_half, incr_half, wtmp);
		}

	}	/* if/else( isign == MORE_ZERO_ISIGN ) */

	FIND_SCALEF_i(iii, all_ors);
	return (iii);
}

/* *********************************************************** */

/* KERNEL MASTER */

/* *********************************************************** */

mlib_s32
mlib_c_fftKernel_4(
    deal_t *CONST pal64data,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	CONST mlib_s32 n_ord_arr = order;
	mlib_s32 n_ret_scale = 0;
	dtCmplx *CONST dat = (dtCmplx *) pal64data;

	if (n_ord_arr > 4) {
		mlib_s32 i, ord_blk = n_ord_arr - 2;

		if (n_ord_arr >= 4 + 2)
			for (i = 0; i < (1 << 2); i++) {
				mlib_c_fftKernel((void *)(dat + (i << ord_blk)),
				    ord_blk, isign);
			}

		n_ret_scale =
		    mlib_c_fftK_2LayersVeryLast(dat, n_ord_arr, isign);
	} else if (n_ord_arr == 4) {
/* accumulator */
		mlib_s32 all_ors = 0;

		if (isign == MORE_ZERO_ISIGN) {
			DEAL_ALL_ORD4_L(isign, dat, AS_IS);
		} else {
			DEAL_ALL_ORD4_L(isign, dat, MINUS);
		}

		FIND_SCALEF(n_ret_scale, all_ors);
	}

	return (n_ret_scale);
}

mlib_s32
mlib_c_fftKernel_4_i(
    mlib_s64 *CONST pal64data,
    mlib_s32 CONST order,
    mlib_s32 CONST isign,
    mlib_s32 CONST nNormIn)
{
	CONST mlib_s32 n_ord_arr = order;
	mlib_s32 n_ret_scale = -order;
	dtCmplx_i *CONST dat = (dtCmplx_i *) pal64data;

	if (n_ord_arr > 4) {
		mlib_s32 i, ord_blk = n_ord_arr - 2;

		if (n_ord_arr >= 4 + 2)
			for (i = 0; i < (1 << 2); i++) {
				mlib_c_fftKernel_i(
					(void *)(dat + (i << ord_blk)),
					ord_blk, isign);
			}

		n_ret_scale =
		    mlib_c_fftK_2LayersVeryLast_i(dat, n_ord_arr, isign);
	} else if (n_ord_arr == 4) {
/* accumulator */
		mlib_s64 all_ors = 0;

		if (isign == MORE_ZERO_ISIGN) {
			DEAL_ALL_ORD4_L_i(isign, dat, AS_IS_i);
		} else {
			DEAL_ALL_ORD4_L_i(isign, dat, MINUS_i);
		}

		FIND_SCALEF_i(n_ret_scale, all_ors);
	} else {
		/* do not forget to compensate norm_scale */
		n_ret_scale -= nNormIn;
	}

	return (n_ret_scale);
}

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#endif

/* *********************************************************** */
