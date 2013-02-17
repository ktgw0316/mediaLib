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

#ifndef _MLIB_SIGNALIMDCTUTIL_H
#define	_MLIB_SIGNALIMDCTUTIL_H

#pragma ident	"@(#)mlib_SignalIMDCTUtil.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */
/* declarations */
typedef double deal_d64_t;
typedef float deal_f32_t;

/* *********************************************************** */
/* macros */

/* bitreversal index */
#define	REV_INDX_IMDCT(indx, shift)	((mlib_bitreversals[ indx ]) >> (shift))

/* pi value */
#define	PI	3.141592653589793238462643383279502884

/* complex multiply macros */
#define	MULT_RE()	(val_r * rot_r - val_i * rot_i)
#define	MULT_IM()	(val_r * rot_i + val_i * rot_r)

/* textually separates 'my' consts from 'normal' ones */
#define	CONST

/* tag to mark 'speed-important' variables */
#define	REGISTER

/* values ( kill ALL 'magic digits'! ) */
#define	ZERO	(WORKING_T)0.0
#define	ONE	(WORKING_T)1.0
#define	MINUS_ONE	(WORKING_T)-1.0
#define	TWO	(WORKING_T)2.0
#define	SIN_PI_BY4	(WORKING_T)0.7071067811865475244008443621048
#define	SIN_PI_BY8	(WORKING_T)0.3826834323650897717284599840304
#define	COS_PI_BY8	(WORKING_T)0.923879532511286
#define	SIN_PI_BY16	(WORKING_T)0.1950903220161282678482848684770
#define	COS_PI_BY16	(WORKING_T)0.980785280403230

/* to switch between -( value ) and -1.*( value ) */
#define	MINUS(value)	((MINUS_ONE) * (value))
/* to switch between MINUS( value ) and ( value ) */
#define	AS_IS(value)	(value)

/* read into deal buffer macro */
#define	LOAD_RI(dst, src_main, src_aux, i_src)                  \
	dst##_r = (WORKING_T) (src_main[i_src]);                \
	dst##_i = (WORKING_T) (src_aux[i_src]);

/* *********************************************************** */
#define	BUTTERFLY_CMPLX_CMPLX_RI(dest1, dest2, src1, src2)      \
	{                                                       \
	    *dest1##__r = src1##_r + src2##_r;                  \
	    *dest1##__i = src1##_i + src2##_i;                  \
	    *dest2##__r = src1##_r - src2##_r;                  \
	    *dest2##__i = src1##_i - src2##_i;                  \
	}

/* *********************************************************** */
#define	DCL_DST_PTR_RI(pbeg, dst, shift)                        \
	WORKING_T *pbeg##_r = (dst##_r) + shift, *pbeg##_i =    \
	    (dst##_i) + shift

/*
 *    declares and inits FOUR ptrs to dtCmplx from p_beg shifted by 1
 */
#define	DCL_4_CMPLX_PTRS_RI(tag, ptr_beg, step)                 \
	WORKING_T *CONST tag##_0__r =                           \
	    ptr_beg##_r, *CONST tag##_1__r =                    \
	    tag##_0__r + 1 * step, *CONST tag##_2__r =          \
	    tag##_0__r + 2 * step, *CONST tag##_3__r =          \
	    tag##_0__r + 3 * step, *CONST tag##_0__i =          \
	    ptr_beg##_i, *CONST tag##_1__i =                    \
	    tag##_0__i + 1 * step, *CONST tag##_2__i =          \
	    tag##_0__i + 2 * step, *CONST tag##_3__i =          \
	    tag##_0__i + 3 * step

/* *********************************************************** */
#define	DCL_8_CMPLX_PTRS_RI(tag0, tag1, ptr_beg, step)          \
	DCL_4_CMPLX_PTRS_RI(tag0, ptr_beg, step);               \
	WORKING_T *CONST next4_r =                              \
	    tag0##_0__r + 4 * step, *CONST next4_i =            \
	    tag0##_0__i + 4 * step;                             \
	DCL_4_CMPLX_PTRS_RI(tag1, next4, step)

/* declare/define components for simple butterflies */
#define	DCL_CALC_SIMPLE_BUTT(dst, src, iSIGNed)                 \
	CONST WORKING_T dst##_0_pls_1_r =                       \
	    src##_0_r + src##_1_r, dst##_0_pls_1_i =            \
	    src##_0_i + src##_1_i, dst##_0_mns_1_r =            \
	    src##_0_r - src##_1_r, dst##_0_mns_1_i =            \
	    src##_0_i - src##_1_i, dst##_2_pls_3_r =            \
	    src##_2_r + src##_3_r, dst##_2_pls_3_i =            \
	    src##_2_i + src##_3_i, dst##_2_mns_3_r =            \
	iSIGNed(src##_3_i - src##_2_i), dst##_2_mns_3_i =       \
	iSIGNed(src##_2_r - src##_3_r)

/* calc components of simple butterfly for 1st-3-layers */
#define	CALC_INTERMED_BUTT(dest1, dest2, src1, src2)            \
	dest1##_r = src1##_r + src2##_r;                        \
	dest1##_i = src1##_i + src2##_i;                        \
	dest2##_r = src1##_r - src2##_r;                        \
	dest2##_i = src1##_i - src2##_i

/* calc components of 2 simple butterflies for 1st-3-layers */
#define	CALC_SIMPLE_BUTTS(dst, src)                             \
	CALC_INTERMED_BUTT(dst##_0, dst##_2, src##_0_pls_1,     \
	    src##_2_pls_3);                                     \
	CALC_INTERMED_BUTT(dst##_1, dst##_3, src##_0_mns_1,     \
	    src##_2_mns_3)

/* declare/define components for 1st-3-layers butterflies */
#define	DCL_CALC_3RD_LAY_BUTT(src, iSIGNed)                     \
	CONST WORKING_T src##_0r_r = src##_0_r, src##_0r_i =    \
	    src##_0_i, src##_1r_r =                             \
	    SIN_PI_BY4 * (src##_1_r - iSIGNed(src##_1_i)),      \
	    src##_1r_i =                                        \
	    SIN_PI_BY4 * (src##_1_i + iSIGNed(src##_1_r)),      \
	    src##_2r_r = -iSIGNed(src##_2_i), src##_2r_i =      \
	iSIGNed(src##_2_r), src##_3r_r =                        \
	    (-SIN_PI_BY4) * (iSIGNed(src##_3_i) + src##_3_r),   \
	    src##_3r_i =                                        \
	    SIN_PI_BY4 * (iSIGNed(src##_3_r) - src##_3_i)

/* declare/define components for simple butterflies */
#define	DCL_CALC_SIMPLE_BUTT_RI(dst, src, iSIGNed)              \
	CONST WORKING_T dst##_0_pls_1_r =                       \
	    (*(src##_0##__r)) + (*(src##_1##__r)),              \
	    dst##_0_pls_1_i =                                   \
	    (*(src##_0##__i)) + (*(src##_1##__i)),              \
	    dst##_0_mns_1_r =                                   \
	    (*(src##_0##__r)) - (*(src##_1##__r)),              \
	    dst##_0_mns_1_i =                                   \
	    (*(src##_0##__i)) - (*(src##_1##__i)),              \
	    dst##_2_pls_3_r =                                   \
	    (*(src##_2##__r)) + (*(src##_3##__r)),              \
	    dst##_2_pls_3_i =                                   \
	    (*(src##_2##__i)) + (*(src##_3##__i)),              \
	    dst##_2_mns_3_r =                                   \
	iSIGNed((*(src##_3##__i)) - (*(src##_2##__i))),         \
	    dst##_2_mns_3_i =                                   \
	iSIGNed((*(src##_2##__r)) - (*(src##_3##__r)))

/* declare/define components for normal butterflies */
#define	DCL_CALC_NORMAL_BUTT_RI(src, tag, rot, rot_half,        \
	r_rh, iSIGNed)                                          \
	REGISTER WORKING_T x##tag;                              \
	CONST WORKING_T aaa0_##tag =                            \
	    (*(src##_0##__r)) + (x##tag =                       \
	    ((*(src##_1##__r)) * rot##_r) -                     \
	    ((*(src##_1##__i)) * rot##_i)), aaa1_##tag =        \
	    (*(src##_0##__r)) - x##tag;                         \
	REGISTER WORKING_T y##tag;                              \
	CONST WORKING_T aaa2_##tag =                            \
	    (*(src##_0##__i)) + (y##tag =                       \
	    ((*(src##_1##__i)) * rot##_r) +                     \
	    ((*(src##_1##__r)) * rot##_i)), aaa3_##tag =        \
	    (*(src##_0##__i)) - y##tag;                         \
	REGISTER WORKING_T a##tag, b##tag;                      \
	CONST WORKING_T bbb0_##tag = (a##tag =                  \
	    ((*(src##_2##__r)) * rot_half##_r) -                \
	    ((*(src##_2##__i)) * rot_half##_i)) +               \
	    (b##tag =                                           \
	    ((*(src##_3##__r)) * (r_rh##_r)) -                  \
	    ((*(src##_3##__i)) * (r_rh##_i))),                  \
	    bbb1_##tag = iSIGNed(b##tag - a##tag);              \
	REGISTER WORKING_T c##tag, d##tag;                      \
	CONST WORKING_T bbb2_##tag = (c##tag =                  \
	    ((*(src##_2##__i)) * rot_half##_r) +                \
	    ((*(src##_2##__r)) * rot_half##_i)) +               \
	    (d##tag =                                           \
	    ((*(src##_3##__i)) * (r_rh##_r)) +                  \
	    ((*(src##_3##__r)) * (r_rh##_i))),                  \
	    bbb3_##tag = iSIGNed(c##tag - d##tag)

/* declare/define components for mirror butterflies */
/*
 *    NOTE: 'mirrored' section taken from Mayer gives these benefits:
 *      1.Loop becomes shorter, its body longer
 *      2.Decrease of trigonometric mults/adds in rots.
 *      3.Last layers loops unrolling
 */
#define	DCL_CALC_MIRROR_BUTT_RI(src, tag, rot, rot_half,        \
	r_rh, iSIGNed)                                          \
	REGISTER WORKING_T x##tag;                              \
	CONST WORKING_T aaa0_##tag =                            \
	    (*(src##_0##__r)) - (x##tag =                       \
	    ((*(src##_1##__r)) * rot##_r) +                     \
	    ((*(src##_1##__i)) * rot##_i)), aaa1_##tag =        \
	    (*(src##_0##__r)) + x##tag;                         \
	REGISTER WORKING_T y##tag;                              \
	CONST WORKING_T aaa2_##tag =                            \
	    (*(src##_0##__i)) - (y##tag =                       \
	    ((*(src##_1##__i)) * rot##_r) -                     \
	    ((*(src##_1##__r)) * rot##_i)), aaa3_##tag =        \
	    (*(src##_0##__i)) + y##tag;                         \
	REGISTER WORKING_T a##tag, b##tag;                      \
	CONST WORKING_T bbb0_##tag = iSIGNed((a##tag =          \
	    ((*(src##_2##__r)) * rot_half##_i) -                \
	    ((*(src##_2##__i)) * rot_half##_r)) +               \
	    (b##tag =                                           \
	    ((*(src##_3##__i)) * (r_rh##_r)) -                  \
	    ((*(src##_3##__r)) * (r_rh##_i)))),                 \
	    bbb1_##tag = (b##tag - a##tag);                     \
	REGISTER WORKING_T c##tag, d##tag;                      \
	CONST WORKING_T bbb2_##tag = iSIGNed((c##tag =          \
	    ((*(src##_2##__i)) * rot_half##_i) +                \
	    ((*(src##_2##__r)) * rot_half##_r)) +               \
	    (d##tag =                                           \
	    ((*(src##_3##__r)) * (-r_rh##_r)) -                 \
	    ((*(src##_3##__i)) * (r_rh##_i)))),                 \
	    bbb3_##tag = (c##tag - d##tag)

/* declare/define components for half/way butterflies */
#define	DCL_CALC_HALFWAY_BUTT_RI(src, tag, iSIGNed)             \
	REGISTER WORKING_T x##tag;                              \
	CONST WORKING_T aaa0_##tag =                            \
	    (*(src##_0##__r)) - (x##tag =                       \
	    iSIGNed((*(src##_1##__i)))), aaa1_##tag =           \
	    (*(src##_0##__r)) + x##tag;                         \
	REGISTER WORKING_T y##tag;                              \
	CONST WORKING_T aaa2_##tag =                            \
	    (*(src##_0##__i)) + (y##tag =                       \
	    iSIGNed((*(src##_1##__r)))), aaa3_##tag =           \
	    (*(src##_0##__i)) - y##tag;                         \
	REGISTER WORKING_T a##tag, b##tag;                      \
	CONST WORKING_T bbb0_##tag =                            \
	    (iSIGNed(SIN_PI_BY4)) * ((a##tag =                  \
	    iSIGNed((*(src##_2##__r))) -                        \
	    (*(src##_2##__i))) - (b##tag =                      \
	    (*(src##_3##__i)) +                                 \
	    iSIGNed((*(src##_3##__r))))), bbb1_##tag =          \
	    -SIN_PI_BY4 * (b##tag + a##tag);                    \
	REGISTER WORKING_T c##tag, d##tag;                      \
	CONST WORKING_T bbb2_##tag =                            \
	    (iSIGNed(SIN_PI_BY4)) * ((c##tag =                  \
	    iSIGNed((*(src##_2##__i))) +                        \
	    (*(src##_2##__r))) + (d##tag =                      \
	    (*(src##_3##__r)) -                                 \
	    iSIGNed((*(src##_3##__i))))), bbb3_##tag =          \
	    SIN_PI_BY4 * (c##tag - d##tag)

/* perform & store simple butterflies */
#define	STORE_SIMPLE_BUTTERFLIES_RI(dst, src)                   \
	BUTTERFLY_CMPLX_CMPLX_RI(dst##_0, dst##_2,              \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX_RI(dst##_1, dst##_3,              \
	    src##_0_mns_1, src##_2_mns_3);

/* special for 1st-3layers */
#define	BUTTERFLY_CMPLX_CMPLX_ROT(dest1, dest2, rot2)           \
	BUTTERFLY_CMPLX_CMPLX_RI(dest1, dest2, dest1, rot2)

/* perform & store 1st-3-layers butterflies */
#define	STORE_3RD_LAY_BUTTERFLIES(src0, src1)                   \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_0, src1##_0,           \
	    src1##_0r);                                         \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_1, src1##_1,           \
	    src1##_1r);                                         \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_2, src1##_2,           \
	    src1##_2r);                                         \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_3, src1##_3,           \
	    src1##_3r);

/* sequences to perform 1st-3layers butt on 8 sequent with step 1 */
/* perform & store usual butterflies */
#define	STORE_USUAL_BUTTERFLIES_RI(dst, tag)                    \
	(*(dst##_0##__r)) = aaa0_##tag + bbb0_##tag;            \
	(*(dst##_0##__i)) = aaa2_##tag + bbb2_##tag;            \
	(*(dst##_1##__r)) = aaa1_##tag - bbb3_##tag;            \
	(*(dst##_1##__i)) = aaa3_##tag - bbb1_##tag;            \
	(*(dst##_2##__r)) = aaa0_##tag - bbb0_##tag;            \
	(*(dst##_2##__i)) = aaa2_##tag - bbb2_##tag;            \
	(*(dst##_3##__r)) = aaa1_##tag + bbb3_##tag;            \
	(*(dst##_3##__i)) = aaa3_##tag + bbb1_##tag;

/* sequences to perform simple butt on 4 complex elems */
#define	DEAL_4_2LAYS(ptr_beg, d0, d1, iSIGNed)                  \
	{                                                       \
	    DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);              \
	    DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, 1);                \
	    STORE_SIMPLE_BUTTERFLIES_RI(d0, c0);                \
	}

/* *********************************************************** */
#define	DEAL_8_3LAYS(ptr_beg, d0, d1, iSIGNed)                  \
	{                                                       \
	    DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);              \
	    DCL_CALC_SIMPLE_BUTT(c1, d1, iSIGNed);              \
	    CALC_SIMPLE_BUTTS(d0, c0);                          \
	    CALC_SIMPLE_BUTTS(d1, c1); {                        \
		DCL_8_CMPLX_PTRS_RI(d0, d1, ptr_beg, 1);        \
		DCL_CALC_3RD_LAY_BUTT(d1, iSIGNed);             \
		STORE_3RD_LAY_BUTTERFLIES(d0, d1);              \
	    }                                                   \
	}

/* sequence to perform simple butt on 4 complex elems */
#define	DEAL_4_SIMPLE_RI(ptr_beg, step, iSIGNed)                \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, step);                 \
	DCL_CALC_SIMPLE_BUTT_RI(c0, d0, iSIGNed);               \
	STORE_SIMPLE_BUTTERFLIES_RI(d0, c0);

/* sequence to perform usual butt on 4+4 complex elems mirrored */
#define	DEAL_8_USUAL_MIRR_RI(ptr_beg, ptr_mirr, step, rot,              \
	rot_half, r_rh, iSIGNed)                                        \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, step);                         \
	DCL_4_CMPLX_PTRS_RI(d1, ptr_mirr, step);                        \
	DCL_CALC_NORMAL_BUTT_RI(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_MIRROR_BUTT_RI(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_RI(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES_RI(d1, 1);

/* sequence to perform half-way butt on 4 complex elems */
#define	DEAL_4_HALFWAY_RI(ptr_beg, step, iSIGNed)               \
	DCL_4_CMPLX_PTRS_RI(d2, ptr_beg, step);                 \
	DCL_CALC_HALFWAY_BUTT_RI(d2, 2, iSIGNed);               \
	STORE_USUAL_BUTTERFLIES_RI(d2, 2);

/* perform ALL at order 2 */
#define	DEAL_ALL_ORD_2(iSIGNed, dst, src_main, src_aux)                 \
	{                                                               \
	    WORKING_T d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		d0_3_r, d0_3_i;                                         \
	    LOAD_RI(d0_0, src_main, src_aux, 0);                        \
	    LOAD_RI(d0_1, src_main, src_aux, 2);                        \
	    LOAD_RI(d0_2, src_main, src_aux, 1);                        \
	    LOAD_RI(d0_3, src_main, src_aux, 3); {                      \
		DCL_DST_PTR_RI(pbeg, dst, 0);                           \
		DCL_4_CMPLX_PTRS_RI(d0, pbeg, 1);                       \
		DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);                  \
		STORE_SIMPLE_BUTTERFLIES_RI(d0, c0);                    \
	    }                                                           \
	}

/* perform ALL at order 3 */
#define	DEAL_ALL_ORD_3(iSIGNed, dst, src_main, src_aux)                   \
	{                                                                 \
	    WORKING_T d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,     \
		d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i, d1_2_r,   \
		d1_2_i, d1_3_r, d1_3_i;                                   \
	    DCL_DST_PTR_RI(pbeg, dst, 0);                                 \
	    LOAD_RI(d0_0, src_main, src_aux, 0);                          \
	    LOAD_RI(d0_1, src_main, src_aux, 4);                          \
	    LOAD_RI(d0_2, src_main, src_aux, 2);                          \
	    LOAD_RI(d0_3, src_main, src_aux, 6);                          \
	    LOAD_RI(d1_0, src_main, src_aux, 1);                          \
	    LOAD_RI(d1_1, src_main, src_aux, 5);                          \
	    LOAD_RI(d1_2, src_main, src_aux, 3);                          \
	    LOAD_RI(d1_3, src_main, src_aux, 7);                          \
	    DEAL_8_3LAYS(pbeg, d0, d1, iSIGNed);                          \
	}

/* perform all ( i.e last 2 layers ) for order 4 */
#define	DEAL_ALL_ORD4_RI(isign, ptr_beg, iSIGNed)                          \
	{                                                                  \
	    DEAL_4_SIMPLE_RI(ptr_beg, 4, iSIGNed);                         \
	}                                                                  \
	                                                                   \
	{                                                                  \
	    WORKING_T *ptr_beg_2_r = ptr_beg##_r + 2, *ptr_beg_2_i =       \
		ptr_beg##_i + 2;                                           \
	    DEAL_4_HALFWAY_RI(ptr_beg_2, 4, iSIGNed);                      \
	}                                                                  \
	                                                                   \
	{                                                                  \
	    CONST WORKING_T rot_r = SIN_PI_BY4, rot_i =                    \
		iSIGNed(SIN_PI_BY4), rot_half_r =                          \
		COS_PI_BY8, rot_half_i = iSIGNed(SIN_PI_BY8), r_rh_r =     \
		SIN_PI_BY8, r_rh_i = iSIGNed(COS_PI_BY8);                  \
	    WORKING_T *ptr_beg_1_r = ptr_beg##_r + 1, *ptr_beg_1_i =       \
		ptr_beg##_i + 1, *ptr_beg_3_r =                            \
		ptr_beg##_r + 3, *ptr_beg_3_i = ptr_beg##_i + 3;           \
	    DEAL_8_USUAL_MIRR_RI(ptr_beg_1, ptr_beg_3, 4, rot, rot_half,   \
		r_rh, iSIGNed);                                            \
	}

/* reversion at even orders */
#define	LOOP_REV_2(leng, ord_arr, iSIGNed, dst, src_main,               \
	src_aux)                                                        \
	{                                                               \
	    mlib_s32 CONST nnn = 1 << ord_arr, o_2 =                    \
		ord_arr - 2, shift = leng - o_2;                        \
	    mlib_s32 iii, hgh;                                          \
	    mlib_s32 rev_1 = 2 << o_2, rev_2 = 1 << o_2, rev_3 =        \
		3 << o_2;                                               \
	    for (iii = hgh = 0; iii < nnn; hgh++, iii += 4) {           \
		mlib_s32 hgh_rev =                                      \
		    REV_INDX_IMDCT(hgh, shift), iii_0_rev =             \
		    hgh_rev, iii_1_rev = hgh_rev | rev_1, iii_2_rev =   \
		    hgh_rev | rev_2, iii_3_rev = hgh_rev | rev_3;       \
		WORKING_T d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r,       \
		    d0_2_i, d0_3_r, d0_3_i;                             \
		DCL_DST_PTR_RI(pbeg, dst, iii);                         \
		LOAD_RI(d0_0, src_main, src_aux, iii_0_rev);            \
		LOAD_RI(d0_1, src_main, src_aux, iii_1_rev);            \
		LOAD_RI(d0_2, src_main, src_aux, iii_2_rev);            \
		LOAD_RI(d0_3, src_main, src_aux, iii_3_rev);            \
		DEAL_4_2LAYS(pbeg, d0, d1, iSIGNed);                    \
	    }                                                           \
	}

/* reversion at odd orders */
#define	LOOP_REV_3(leng, ord_arr, iSIGNed, dst, src_main,               \
	src_aux)                                                        \
	{                                                               \
	    mlib_s32 CONST nnn = 1 << ord_arr, o_3 =                    \
		ord_arr - 3, shift = leng - o_3;                        \
	    mlib_s32 iii, hgh;                                          \
	    mlib_s32 rev_1 = 4 << o_3, rev_2 = 2 << o_3, rev_3 =        \
		6 << o_3, rev_4 = 1 << o_3, rev_5 = 5 << o_3, rev_6 =   \
		3 << o_3, rev_7 = 7 << o_3;                             \
	    for (iii = hgh = 0; iii < nnn; hgh++, iii += 8) {           \
		mlib_s32 hgh_rev =                                      \
		    REV_INDX_IMDCT(hgh, shift), iii_0_rev =             \
		    hgh_rev, iii_1_rev = hgh_rev | rev_1, iii_2_rev =   \
		    hgh_rev | rev_2, iii_3_rev =                        \
		    hgh_rev | rev_3, iii_4_rev =                        \
		    hgh_rev | rev_4, iii_5_rev =                        \
		    hgh_rev | rev_5, iii_6_rev =                        \
		    hgh_rev | rev_6, iii_7_rev = hgh_rev | rev_7;       \
		WORKING_T d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r,       \
		    d0_2_i, d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r,     \
		    d1_1_i, d1_2_r, d1_2_i, d1_3_r, d1_3_i;             \
		DCL_DST_PTR_RI(pbeg, dst, iii);                         \
		LOAD_RI(d0_0, src_main, src_aux, iii_0_rev);            \
		LOAD_RI(d0_1, src_main, src_aux, iii_1_rev);            \
		LOAD_RI(d0_2, src_main, src_aux, iii_2_rev);            \
		LOAD_RI(d0_3, src_main, src_aux, iii_3_rev);            \
		LOAD_RI(d1_0, src_main, src_aux, iii_4_rev);            \
		LOAD_RI(d1_1, src_main, src_aux, iii_5_rev);            \
		LOAD_RI(d1_2, src_main, src_aux, iii_6_rev);            \
		LOAD_RI(d1_3, src_main, src_aux, iii_7_rev);            \
		DEAL_8_3LAYS(pbeg, d0, d1, iSIGNed);                    \
	    }                                                           \
	}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALIMDCTUTIL_H */
