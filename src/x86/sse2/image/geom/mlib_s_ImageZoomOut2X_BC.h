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

#ifndef _MLIB_S_IMAGEZOOMOUT2X_BC_H
#define	_MLIB_S_IMAGEZOOMOUT2X_BC_H

#pragma ident	"@(#)mlib_s_ImageZoomOut2X_BC.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageZoom2X.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if IMG_TYPE == 1

#define	STYPE	mlib_u8
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_U8_##SUFF
#define	SAT_STORE	STORE_U8

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s16
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_S16_##SUFF
#define	SAT_STORE	STORE_S16

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 */

#define	STYPE	mlib_u16
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_U16_##SUFF
#define	SAT_STORE	STORE_U16

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s32
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_S32_##SUFF
#define	SAT_STORE	STORE_S32

#elif IMG_TYPE == 5	/* IMG_TYPE == 1 */

#define	STYPE	mlib_f32
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_F32_##SUFF
#define	SAT_STORE	STORE_FP

#elif IMG_TYPE == 6	/* IMG_TYPE == 1 */

#define	STYPE	mlib_d64
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_D64_##SUFF
#define	SAT_STORE	STORE_FP
#endif		   /* IMG_TYPE == 1 */

/* *********************************************************** */

#if defined(USED_FILTER_BICUBIC)
#define	BC_SUM_CONST	9
#define	BC_SUM1_CONST	4
#define	BC_SUM2_CONST	8
#define	BC_SUM1_DIV	16
#define	BC_SUM2_DIV	256
#else		   /* defined(USED_FILTER_BICUBIC) */
#define	BC_SUM_CONST	5
#define	BC_SUM1_CONST	3
#define	BC_SUM2_CONST	6
#define	BC_SUM1_DIV	8
#define	BC_SUM2_DIV	64
#endif		   /* defined(USED_FILTER_BICUBIC) */

/* *********************************************************** */

#define	BC_SUM(a, b, c, d)	(BC_SUM_CONST*((TTYPE)(b) + (c)) - \
				((TTYPE)(a) + (d)))

#define	BCHAN1 CHAN2 * j + CHAN1
#define	BCHAN1N CHAN2 * (j + 1) + CHAN1
#define	BCHAN2 CHAN2 * j + CHAN2
#define	BCHAN2N CHAN2 * (j + 1) + CHAN2


#define	MLIB_BIC_LDPD(chan, k, x0, x1, x2, x3)	                    \
	x0 = _mm_loadu_pd((void *)(sp + CHAN2 * (k) + chan - slb));     \
	x1 = _mm_loadu_pd((void *)(sp + CHAN2 * (k) + chan));           \
	x2 = _mm_loadu_pd((void *)(sp + CHAN2 * (k) + chan + slb));     \
	x3 = _mm_loadu_pd((void *)(sp + CHAN2 * (k) + chan + 2 * slb));

#define	MLIB_BIC_LDPS(chan, k, x0, x1, x2, x3)	                    \
	x0 = _mm_loadu_ps((void *)(sp + CHAN2 * (k) + chan - slb));     \
	x1 = _mm_loadu_ps((void *)(sp + CHAN2 * (k) + chan));           \
	x2 = _mm_loadu_ps((void *)(sp + CHAN2 * (k) + chan + slb));     \
	x3 = _mm_loadu_ps((void *)(sp + CHAN2 * (k) + chan + 2 * slb));

#define	MLIB_BIC_LDSI(chan, k, x0, x1, x2, x3)	                      \
	x0 = _mm_loadu_si128((void *)(sp + CHAN2 * (k) + chan - slb));    \
	x1 = _mm_loadu_si128((void *)(sp + CHAN2 * (k) + chan));          \
	x2 = _mm_loadu_si128((void *)(sp + CHAN2 * (k) + chan + slb));    \
	x3 = _mm_loadu_si128((void *)(sp + CHAN2 * (k) + chan + 2 * slb));

#define	MLIB_BIC_LDLSI(chan, k, x0, x1, x2, x3)	                      \
	x0 = _mm_loadl_epi64((void *)(sp + CHAN2 * (k) + chan - slb));    \
	x1 = _mm_loadl_epi64((void *)(sp + CHAN2 * (k) + chan));          \
	x2 = _mm_loadl_epi64((void *)(sp + CHAN2 * (k) + chan + slb));    \
	x3 = _mm_loadl_epi64((void *)(sp + CHAN2 * (k) + chan + 2 * slb));

/* *********************************************************** */

#if IMG_TYPE < 4

#define	TTYPE	mlib_s32
#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SUM1_CONST)
#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SUM2_CONST)

#elif IMG_TYPE == 5	/* IMG_TYPE < 4 */

#define	TTYPE	mlib_f32
#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) * (1.f / BC_SUM1_DIV))
#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) * (1.f / BC_SUM2_DIV))

#else		   /* IMG_TYPE < 4 */

#define	TTYPE	mlib_d64
#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) * (1.0 / BC_SUM1_DIV))
#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) * (1.0 / BC_SUM2_DIV))

#endif		   /* IMG_TYPE < 4 */

#if IMG_TYPE == 2 || IMG_TYPE == 3

#define	DEF_VARS()                                              \
	STYPE *sp, *dp;                                         \
	TTYPE a0, a1, a2, a3, res;                              \
	mlib_u32 mask;                                          \
	mlib_s32 i, j, k

#else		   /* IMG_TYPE == 2 || IMG_TYPE == 3 */

#define	DEF_VARS()                                              \
	STYPE *sp, *dp;                                         \
	TTYPE a0, a1, a2, a3, res;                              \
	mlib_s32 i, j, k

#endif		   /* IMG_TYPE == 2 || IMG_TYPE == 3 */

/* *********************************************************** */

#define	CHAN1	1
#define	CHAN2	(2*CHAN1)

/* *********************************************************** */

#if defined(USED_FILTER_BICUBIC)

void ZOOM_FUNC(
    1_Bicubic) (
    BC_PARAMETERS(STYPE))

#else		   /* defined(USED_FILTER_BICUBIC) */

void ZOOM_FUNC(
    1_Bicubic2) (
    BC_PARAMETERS(STYPE))

#endif		   /* defined(USED_FILTER_BICUBIC) */

#if IMG_TYPE == 6	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	sp = p_src;
	dp = p_dst;

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	mlib_d64 bc_sum2_div = 1.0 / BC_SUM2_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	const __m128d x_bc_sum2_div = _mm_set1_pd(bc_sum2_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	__m128d x_sd0, x_sd1, x_sd2, x_sd3;
	__m128d x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_st0, x_st1, x_st2, x_st3;
	__m128d x_st4, x_st5, x_st6, x_st7;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;
	mlib_d64 res0, res1;
	mlib_d64 *sa = (mlib_d64 *)&x_dd0;
	mlib_d64 *sb = (mlib_d64 *)&x_dd2;

/* averaging along X axis */
	if (dh) {

		for (i = 0; i < height; i++) {
			x_dd0 = _mm_loadu_pd(sp - CHAN1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_dd1 = _mm_loadu_pd(sp + BCHAN1);
				x_dd3 = _mm_loadu_pd(sp + BCHAN1N);
				x_st0 = _mm_unpacklo_pd(x_dd0, x_dd1);
				x_st1 = _mm_unpackhi_pd(x_dd0, x_dd1);
				x_st2 = _mm_unpacklo_pd(x_dd1, x_dd3);
				x_st3 = _mm_unpackhi_pd(x_dd1, x_dd3);
				x_st4 = _mm_add_pd(x_st0, x_st3);
				x_st5 = _mm_add_pd(x_st1, x_st2);
				x_st6 = _mm_mul_pd(x_st5, x_const);
				x_st7 = _mm_sub_pd(x_st6, x_st4);
				x_dd2 = _mm_mul_pd(x_st7, x_bc_sum1_div);
				SAT_STORE(dp[j], sb[0]);
				SAT_STORE(dp[j + 1], sb[1]);
				x_dd0 = x_dd3;
			}

			if (j < width) {
				a0 = sa[0];
				a1 = sa[1];
				a2 = sp[BCHAN1];
				a3 = sp[BCHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDPD(0, j, x_sd0, x_sd1, x_sd2, x_sd3)
				x_st0 = _mm_add_pd(x_sd1, x_sd2);
				x_st1 = _mm_add_pd(x_sd0, x_sd3);
				x_st2 = _mm_mul_pd(x_st0, x_const);
				x_st3 = _mm_sub_pd(x_st2, x_st1);
				x_dd0 = _mm_mul_pd(x_st3, x_bc_sum1_div);
				SAT_STORE(dp[j], sa[0]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		x_sd4 = _mm_loadu_pd((void *)(sp - 1 - slb));
		x_sd5 = _mm_loadu_pd((void *)(sp - 1));
		x_sd6 = _mm_loadu_pd((void *)(sp - 1 + slb));
		x_sd7 = _mm_loadu_pd((void *)(sp - 1 + 2 * slb));

		x_st4 = _mm_add_pd(x_sd5, x_sd6);
		x_st5 = _mm_add_pd(x_sd4, x_sd7);
		x_st6 = _mm_mul_pd(x_st4, x_const);
		x_dd0 = _mm_sub_pd(x_st6, x_st5);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDPD(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd1 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1, j + 1, x_sd4, x_sd5, x_sd6, x_sd7)

			x_st4 = _mm_add_pd(x_sd5, x_sd6);
			x_st5 = _mm_add_pd(x_sd4, x_sd7);
			x_st6 = _mm_mul_pd(x_st4, x_const);
			x_dd3 = _mm_sub_pd(x_st6, x_st5);

			x_st0 = _mm_unpacklo_pd(x_dd0, x_dd1);
			x_st1 = _mm_unpackhi_pd(x_dd0, x_dd1);
			x_st2 = _mm_unpacklo_pd(x_dd1, x_dd3);
			x_st3 = _mm_unpackhi_pd(x_dd1, x_dd3);
			x_st4 = _mm_add_pd(x_st0, x_st3);
			x_st5 = _mm_add_pd(x_st1, x_st2);
			x_st6 = _mm_mul_pd(x_st5, x_const);
			x_st7 = _mm_sub_pd(x_st6, x_st4);

			x_dd2 = _mm_mul_pd(x_st7, x_bc_sum2_div);
			SAT_STORE(dp[j], sb[0]);
			SAT_STORE(dp[j + 1], sb[1]);
			x_dd0 = x_dd3;
		}

		if (j < width) {
			a0 = sa[0];
			a1 = sa[1];
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1], sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2], sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[j], res);
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

#elif IMG_TYPE == 5	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	sp = p_src;
	dp = p_dst;

	mlib_f32 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128 x_bc_sum1_div = _mm_set1_ps(bc_sum1_div);
	const __m128 x_const = _mm_set1_ps(BC_SUM_CONST);
	__m128 x_sd0, x_sd1, x_sd2, x_sd3;
	__m128 x_sd4, x_sd5, x_sd6, x_sd7;
	__m128 x_sd8, x_sd9, x_sd10, x_sd11;
	__m128 x_st0, x_st1, x_st2, x_dd0;
	__m128 x_st3, x_st4, x_st5, x_dd1;
	__m128 x_st6, x_st7, x_st8, x_dd2;
	mlib_f32 *sa = (mlib_f32 *)&x_dd0;
	mlib_f32 *sb = (mlib_f32 *)&x_dd1;
	mlib_f32 *sc = (mlib_f32 *)&x_dd2;
	mlib_f32 res0, res1, res2, res3, res4, res5;

/* averaging along X axis */
	if (dh) {

		for (i = 0; i < height; i++) {
			a2 = sp[-CHAN1];
			a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[BCHAN1];
				a3 = sp[BCHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_BIC_LDPS(0, j, x_sd0, x_sd1, x_sd2, x_sd3)
				x_st0 = _mm_add_ps(x_sd1, x_sd2);
				x_st1 = _mm_add_ps(x_sd0, x_sd3);
				x_st2 = _mm_mul_ps(x_st0, x_const);
				x_st3 = _mm_sub_ps(x_st2, x_st1);
				x_dd0 = _mm_mul_ps(x_st3, x_bc_sum1_div);
				SAT_STORE(dp[j], sa[0]);
				SAT_STORE(dp[j + 1], sa[2]);
			}

			if (j < width) {
				MLIB_BIC_LDPS(0, j, x_sd0, x_sd1, x_sd2, x_sd3)
				x_st0 = _mm_add_ps(x_sd1, x_sd2);
				x_st1 = _mm_add_ps(x_sd0, x_sd3);
				x_st2 = _mm_mul_ps(x_st0, x_const);
				x_st3 = _mm_sub_ps(x_st2, x_st1);
				x_dd0 = _mm_mul_ps(x_st3, x_bc_sum1_div);
				SAT_STORE(dp[j], sa[0]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 5; j += 6) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			k = j + 2;
			MLIB_BIC_LDPS(CHAN1, k, x_sd4, x_sd5, x_sd6, x_sd7)

			k = j + 4;
			MLIB_BIC_LDPS(CHAN1, k, x_sd8, x_sd9, x_sd10, x_sd11)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd0 = _mm_sub_ps(x_st2, x_st1);

			x_st3 = _mm_add_ps(x_sd5, x_sd6);
			x_st4 = _mm_add_ps(x_sd4, x_sd7);
			x_st5 = _mm_mul_ps(x_st3, x_const);
			x_dd1 = _mm_sub_ps(x_st5, x_st4);

			x_st6 = _mm_add_ps(x_sd9, x_sd10);
			x_st7 = _mm_add_ps(x_sd8, x_sd11);
			x_st8 = _mm_mul_ps(x_st6, x_const);
			x_dd2 = _mm_sub_ps(x_st8, x_st7);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			res2 = BC_SUM2(sa[2], sa[3], sb[0], sb[1]);
			res3 = BC_SUM2(sb[0], sb[1], sb[2], sb[3]);
			res4 = BC_SUM2(sb[2], sb[3], sc[0], sc[1]);
			res5 = BC_SUM2(sc[0], sc[1], sc[2], sc[3]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			SAT_STORE(dp[j + 2], res2);
			SAT_STORE(dp[j + 3], res3);
			SAT_STORE(dp[j + 4], res4);
			SAT_STORE(dp[j + 5], res5);
			a0 = sc[2];
			a1 = sc[3];
		}

		if (j < width - 3) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			k = j + 2;
			MLIB_BIC_LDPS(CHAN1, k, x_sd4, x_sd5, x_sd6, x_sd7)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd0 = _mm_sub_ps(x_st2, x_st1);

			x_st3 = _mm_add_ps(x_sd5, x_sd6);
			x_st4 = _mm_add_ps(x_sd4, x_sd7);
			x_st5 = _mm_mul_ps(x_st3, x_const);
			x_dd1 = _mm_sub_ps(x_st5, x_st4);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			res2 = BC_SUM2(sa[2], sa[3], sb[0], sb[1]);
			res3 = BC_SUM2(sb[0], sb[1], sb[2], sb[3]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			SAT_STORE(dp[j + 2], res2);
			SAT_STORE(dp[j + 3], res3);
			a0 = sb[2];
			a1 = sb[3];
			j += 4;
		}

		if (j < width - 1) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd0 = _mm_sub_ps(x_st2, x_st1);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			a0 = sa[2];
			a1 = sa[3];
			j += 2;
		}

		if (j < width) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd0 = _mm_sub_ps(x_st2, x_st1);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			SAT_STORE(dp[j], res0);
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

#elif IMG_TYPE == 4	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	sp = p_src;
	dp = p_dst;

/* averaging along X axis */
	if (dh) {

		for (i = 0; i < height; i++) {
			a2 = sp[-CHAN1];
			a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[BCHAN1];
				a3 = sp[BCHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128d x_dm0, x_dm1, x_dm2, x_dm3;
	__m128d x_dm4, x_dm5, x_dm6, x_dm7;
	__m128d x_mm0, x_mm1, x_mm2, x_mm3;
	__m128d x_mm4, x_mm5, x_mm6, x_mm7;
	mlib_d64 res0, res1;
	mlib_d64 *sa, *sb;
	sa = (mlib_d64 *)&x_mm6;
	sb = (mlib_d64 *)&x_mm7;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpackhi_epi64(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi64(x_sd1, x_zero);
				x_st2 = _mm_unpackhi_epi64(x_sd2, x_zero);
				x_st3 = _mm_unpackhi_epi64(x_sd3, x_zero);

				x_dm0 = _mm_cvtepi32_pd(x_sd0);
				x_dm1 = _mm_cvtepi32_pd(x_st0);
				x_dm2 = _mm_cvtepi32_pd(x_sd1);
				x_dm3 = _mm_cvtepi32_pd(x_st1);
				x_dm4 = _mm_cvtepi32_pd(x_sd2);
				x_dm5 = _mm_cvtepi32_pd(x_st2);
				x_dm6 = _mm_cvtepi32_pd(x_sd3);
				x_dm7 = _mm_cvtepi32_pd(x_st3);

				x_mm0 = _mm_add_pd(x_dm2, x_dm4);
				x_mm1 = _mm_add_pd(x_dm3, x_dm5);
				x_mm2 = _mm_add_pd(x_dm0, x_dm6);
				x_mm3 = _mm_add_pd(x_dm1, x_dm7);

				x_mm4 = _mm_mul_pd(x_mm0, x_const);
				x_mm5 = _mm_mul_pd(x_mm1, x_const);
				x_dm0 = _mm_sub_pd(x_mm4, x_mm2);
				x_dm1 = _mm_sub_pd(x_mm5, x_mm3);
				x_mm6 = _mm_mul_pd(x_dm0, x_bc_sum1_div);
				x_mm7 = _mm_mul_pd(x_dm1, x_bc_sum1_div);
				SAT_STORE(dp[j], sa[0]);
				SAT_STORE(dp[j + 1], sb[0]);
			}

			if (j < width) {
				a0 = sp[CHAN2 * j - slb];
				a1 = sp[CHAN2 * j];
				a2 = sp[CHAN2 * j + slb];
				a3 = sp[CHAN2 * j + 2 * slb];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpackhi_epi64(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi64(x_sd1, x_zero);
			x_st2 = _mm_unpackhi_epi64(x_sd2, x_zero);
			x_st3 = _mm_unpackhi_epi64(x_sd3, x_zero);

			x_dm0 = _mm_cvtepi32_pd(x_sd0);
			x_dm1 = _mm_cvtepi32_pd(x_st0);
			x_dm2 = _mm_cvtepi32_pd(x_sd1);
			x_dm3 = _mm_cvtepi32_pd(x_st1);
			x_dm4 = _mm_cvtepi32_pd(x_sd2);
			x_dm5 = _mm_cvtepi32_pd(x_st2);
			x_dm6 = _mm_cvtepi32_pd(x_sd3);
			x_dm7 = _mm_cvtepi32_pd(x_st3);

			x_mm0 = _mm_add_pd(x_dm2, x_dm4);
			x_mm1 = _mm_add_pd(x_dm3, x_dm5);
			x_mm2 = _mm_add_pd(x_dm0, x_dm6);
			x_mm3 = _mm_add_pd(x_dm1, x_dm7);

			x_mm4 = _mm_mul_pd(x_mm0, x_const);
			x_mm5 = _mm_mul_pd(x_mm1, x_const);
			x_mm6 = _mm_sub_pd(x_mm4, x_mm2);
			x_mm7 = _mm_sub_pd(x_mm5, x_mm3);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sb[0], sb[1]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			a0 = sb[0];
			a1 = sb[1];
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1], sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2], sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[j], res);
			a0 = a2;
			a1 = a3;
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

#elif IMG_TYPE == 3	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	sp = p_src;
	dp = p_dst;

/* averaging along X axis */
	if (dh) {

		for (i = 0; i < height; i++) {
			a2 = sp[-CHAN1];
			a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[BCHAN1];
				a3 = sp[BCHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;

	}

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_st4, x_st5, x_st6, x_st7;
	__m128i x_sl0, x_sl1, x_sl2, x_sl3, x_sl4, x_sl5;
	mlib_s32 *sa = (mlib_s32 *)&x_sl4;
	mlib_s32 *sb = (mlib_s32 *)&x_sl5;
	mlib_s32 res0, res1, res2, res3;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_st3 = _mm_unpackhi_epi16(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_st5 = _mm_unpackhi_epi16(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi16(x_sd3, x_zero);
				x_st7 = _mm_unpackhi_epi16(x_sd3, x_zero);

				x_sl0 = _mm_add_epi32(x_st2, x_st4);
				x_sl1 = _mm_add_epi32(x_st3, x_st5);
				x_sl2 = _mm_add_epi32(x_st0, x_st6);
				x_sl3 = _mm_add_epi32(x_st1, x_st7);

				x_st0 = _mm_mul_epu32(x_sl0, x_const);
				x_st1 = _mm_mul_epu32(
				    _mm_srli_epi64(x_sl0, 32), x_const);
				x_st2 = _mm_unpacklo_epi32(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi32(x_st0, x_st1);
				x_sl0 = _mm_unpacklo_epi64(x_st2, x_st3);

				x_st4 = _mm_mul_epu32(x_sl1, x_const);
				x_st5 = _mm_mul_epu32(
				    _mm_srli_epi64(x_sl1, 32), x_const);
				x_st6 = _mm_unpacklo_epi32(x_st4, x_st5);
				x_st7 = _mm_unpackhi_epi32(x_st4, x_st5);
				x_sl1 = _mm_unpacklo_epi64(x_st6, x_st7);

				x_sl4 = _mm_sub_epi32(x_sl0, x_sl2);
				x_sl5 = _mm_sub_epi32(x_sl1, x_sl3);
				x_sl4 = _mm_srai_epi32(x_sl4, BC_SUM1_CONST);
				x_sl5 = _mm_srai_epi32(x_sl5, BC_SUM1_CONST);
				SAT_STORE(dp[j], sa[0]);
				SAT_STORE(dp[j + 1], sa[2]);
				SAT_STORE(dp[j + 2], sb[0]);
				SAT_STORE(dp[j + 3], sb[2]);
			}

			for (j = 0; j < width; j++) {
				a0 = sp[CHAN2 * j - slb];
				a1 = sp[CHAN2 * j];
				a2 = sp[CHAN2 * j + slb];
				a3 = sp[CHAN2 * j + 2 * slb];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_sl0 = _mm_add_epi32(x_st2, x_st4);
			x_sl1 = _mm_add_epi32(x_st3, x_st5);
			x_sl2 = _mm_add_epi32(x_st0, x_st6);
			x_sl3 = _mm_add_epi32(x_st1, x_st7);

			x_st0 = _mm_mul_epu32(x_sl0, x_const);
			x_st1 = _mm_mul_epu32(
			    _mm_srli_epi64(x_sl0, 32), x_const);
			x_st2 = _mm_unpacklo_epi32(x_st0, x_st1);
			x_st3 = _mm_unpackhi_epi32(x_st0, x_st1);
			x_sl0 = _mm_unpacklo_epi64(x_st2, x_st3);

			x_st4 = _mm_mul_epu32(x_sl1, x_const);
			x_st5 = _mm_mul_epu32(
			    _mm_srli_epi64(x_sl1, 32), x_const);
			x_st6 = _mm_unpacklo_epi32(x_st4, x_st5);
			x_st7 = _mm_unpackhi_epi32(x_st4, x_st5);
			x_sl1 = _mm_unpacklo_epi64(x_st6, x_st7);

			x_sl4 = _mm_sub_epi32(x_sl0, x_sl2);
			x_sl5 = _mm_sub_epi32(x_sl1, x_sl3);
			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			res2 = BC_SUM2(sa[2], sa[3], sb[0], sb[1]);
			res3 = BC_SUM2(sb[0], sb[1], sb[2], sb[3]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			SAT_STORE(dp[j + 2], res2);
			SAT_STORE(dp[j + 3], res3);
			a0 = sb[2];
			a1 = sb[3];
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1], sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2], sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[j], res);
			a0 = a2;
			a1 = a3;
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

#elif IMG_TYPE == 2	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	sp = p_src;
	dp = p_dst;

/* averaging along X axis */
	if (dh) {

		for (i = 0; i < height; i++) {
			a2 = sp[-CHAN1];
			a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[BCHAN1];
				a3 = sp[BCHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_st4, x_st5, x_st6, x_st7;
	__m128i x_sl0, x_sl1, x_sl2, x_sl3, x_sl4, x_sl5;
	mlib_s32 *sa = (mlib_s32 *)&x_sl4;
	mlib_s32 *sb = (mlib_s32 *)&x_sl5;
	mlib_s32 res0, res1, res2, res3;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sl0 = _mm_srai_epi16(x_sd0, 15);
				x_sl1 = _mm_srai_epi16(x_sd1, 15);
				x_sl2 = _mm_srai_epi16(x_sd2, 15);
				x_sl3 = _mm_srai_epi16(x_sd3, 15);

				x_st0 = _mm_unpacklo_epi16(x_sd0, x_sl0);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_sl0);
				x_st2 = _mm_unpacklo_epi16(x_sd1, x_sl1);
				x_st3 = _mm_unpackhi_epi16(x_sd1, x_sl1);
				x_st4 = _mm_unpacklo_epi16(x_sd2, x_sl2);
				x_st5 = _mm_unpackhi_epi16(x_sd2, x_sl2);
				x_st6 = _mm_unpacklo_epi16(x_sd3, x_sl3);
				x_st7 = _mm_unpackhi_epi16(x_sd3, x_sl3);

				x_sl0 = _mm_add_epi32(x_st2, x_st4);
				x_sl1 = _mm_add_epi32(x_st3, x_st5);
				x_sl2 = _mm_add_epi32(x_st0, x_st6);
				x_sl3 = _mm_add_epi32(x_st1, x_st7);

				x_st0 = _mm_mul_epu32(x_sl0, x_const);
				x_st1 = _mm_mul_epu32(
				    _mm_srli_epi64(x_sl0, 32), x_const);
				x_st2 = _mm_unpacklo_epi32(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi32(x_st0, x_st1);
				x_sl0 = _mm_unpacklo_epi64(x_st2, x_st3);

				x_st4 = _mm_mul_epu32(x_sl1, x_const);
				x_st5 = _mm_mul_epu32(
				    _mm_srli_epi64(x_sl1, 32), x_const);
				x_st6 = _mm_unpacklo_epi32(x_st4, x_st5);
				x_st7 = _mm_unpackhi_epi32(x_st4, x_st5);
				x_sl1 = _mm_unpacklo_epi64(x_st6, x_st7);

				x_sl4 = _mm_sub_epi32(x_sl0, x_sl2);
				x_sl5 = _mm_sub_epi32(x_sl1, x_sl3);
				x_sl4 = _mm_srai_epi32(x_sl4, BC_SUM1_CONST);
				x_sl5 = _mm_srai_epi32(x_sl5, BC_SUM1_CONST);
				SAT_STORE(dp[j], sa[0]);
				SAT_STORE(dp[j + 1], sa[2]);
				SAT_STORE(dp[j + 2], sb[0]);
				SAT_STORE(dp[j + 3], sb[2]);
			}

			for (; j < width; j++) {
				a0 = sp[CHAN2 * j - slb];
				a1 = sp[CHAN2 * j];
				a2 = sp[CHAN2 * j + slb];
				a3 = sp[CHAN2 * j + 2 * slb];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sl0 = _mm_srai_epi16(x_sd0, 15);
			x_sl1 = _mm_srai_epi16(x_sd1, 15);
			x_sl2 = _mm_srai_epi16(x_sd2, 15);
			x_sl3 = _mm_srai_epi16(x_sd3, 15);

			x_st0 = _mm_unpacklo_epi16(x_sd0, x_sl0);
			x_st1 = _mm_unpackhi_epi16(x_sd0, x_sl0);
			x_st2 = _mm_unpacklo_epi16(x_sd1, x_sl1);
			x_st3 = _mm_unpackhi_epi16(x_sd1, x_sl1);
			x_st4 = _mm_unpacklo_epi16(x_sd2, x_sl2);
			x_st5 = _mm_unpackhi_epi16(x_sd2, x_sl2);
			x_st6 = _mm_unpacklo_epi16(x_sd3, x_sl3);
			x_st7 = _mm_unpackhi_epi16(x_sd3, x_sl3);

			x_sl0 = _mm_add_epi32(x_st2, x_st4);
			x_sl1 = _mm_add_epi32(x_st3, x_st5);
			x_sl2 = _mm_add_epi32(x_st0, x_st6);
			x_sl3 = _mm_add_epi32(x_st1, x_st7);

			x_st0 = _mm_mul_epu32(x_sl0, x_const);
			x_st1 = _mm_mul_epu32(
			    _mm_srli_epi64(x_sl0, 32), x_const);
			x_st2 = _mm_unpacklo_epi32(x_st0, x_st1);
			x_st3 = _mm_unpackhi_epi32(x_st0, x_st1);
			x_sl0 = _mm_unpacklo_epi64(x_st2, x_st3);

			x_st4 = _mm_mul_epu32(x_sl1, x_const);
			x_st5 = _mm_mul_epu32(
			    _mm_srli_epi64(x_sl1, 32), x_const);
			x_st6 = _mm_unpacklo_epi32(x_st4, x_st5);
			x_st7 = _mm_unpackhi_epi32(x_st4, x_st5);
			x_sl1 = _mm_unpacklo_epi64(x_st6, x_st7);

			x_sl4 = _mm_sub_epi32(x_sl0, x_sl2);
			x_sl5 = _mm_sub_epi32(x_sl1, x_sl3);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			res2 = BC_SUM2(sa[2], sa[3], sb[0], sb[1]);
			res3 = BC_SUM2(sb[0], sb[1], sb[2], sb[3]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			SAT_STORE(dp[j + 2], res2);
			SAT_STORE(dp[j + 3], res3);
			a0 = sb[2];
			a1 = sb[3];
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1], sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2], sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[j], res);
			a0 = a2;
			a1 = a3;
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

#elif IMG_TYPE == 1	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	sp = p_src;
	dp = p_dst;

/* averaging along X axis */
	if (dh) {

		for (i = 0; i < height; i++) {
			a2 = sp[-CHAN1];
			a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[BCHAN1];
				a3 = sp[BCHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

	const __m128i x_const = _mm_set1_epi16(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_st4, x_st5, x_st6, x_st7;
	__m128i x_sl0, x_sl1, x_sl2, x_sl3, x_sl4, x_sl5;
	mlib_s16 *sa = (mlib_s16 *)&x_sl4;
	mlib_s16 *sb = (mlib_s16 *)&x_sl5;
	mlib_s32 res0, res1, res2, res3, res4, res5, res6, res7;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 7; j += 8) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
				x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

				x_sl0 = _mm_add_epi16(x_st2, x_st4);
				x_sl1 = _mm_add_epi16(x_st3, x_st5);
				x_sl2 = _mm_add_epi16(x_st0, x_st6);
				x_sl3 = _mm_add_epi16(x_st1, x_st7);

				x_st0 = _mm_mullo_epi16(x_sl0, x_const);
				x_st1 = _mm_mullo_epi16(x_sl1, x_const);
				x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
				x_sl5 = _mm_sub_epi16(x_st1, x_sl3);
				x_sl4 = _mm_srai_epi16(x_sl4, BC_SUM1_CONST);
				x_sl5 = _mm_srai_epi16(x_sl5, BC_SUM1_CONST);
				SAT_STORE(dp[j], sa[0]);
				SAT_STORE(dp[j + 1], sa[2]);
				SAT_STORE(dp[j + 2], sa[4]);
				SAT_STORE(dp[j + 3], sa[6]);
				SAT_STORE(dp[j + 4], sb[0]);
				SAT_STORE(dp[j + 5], sb[2]);
				SAT_STORE(dp[j + 6], sb[4]);
				SAT_STORE(dp[j + 7], sb[6]);
			}

			if (j < width - 3) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

				x_sl0 = _mm_add_epi16(x_st2, x_st4);
				x_sl2 = _mm_add_epi16(x_st0, x_st6);

				x_st0 = _mm_mullo_epi16(x_sl0, x_const);
				x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
				x_sl4 = _mm_srai_epi16(x_sl4, BC_SUM1_CONST);
				SAT_STORE(dp[j], sa[0]);
				SAT_STORE(dp[j + 1], sa[2]);
				SAT_STORE(dp[j + 2], sa[4]);
				SAT_STORE(dp[j + 3], sa[6]);
				j += 4;
			}

			for (; j < width; j++) {
				a0 = sp[CHAN2 * j - slb];
				a1 = sp[CHAN2 * j];
				a2 = sp[CHAN2 * j + slb];
				a3 = sp[CHAN2 * j + 2 * slb];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {

		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 7; j += 8) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
			x_sl5 = _mm_sub_epi16(x_st1, x_sl3);

			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			res2 = BC_SUM2(sa[2], sa[3], sa[4], sa[5]);
			res3 = BC_SUM2(sa[4], sa[5], sa[6], sa[7]);
			res4 = BC_SUM2(sa[6], sa[7], sb[0], sb[1]);
			res5 = BC_SUM2(sb[0], sb[1], sb[2], sb[3]);
			res6 = BC_SUM2(sb[2], sb[3], sb[4], sb[5]);
			res7 = BC_SUM2(sb[4], sb[5], sb[6], sb[7]);

			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			SAT_STORE(dp[j + 2], res2);
			SAT_STORE(dp[j + 3], res3);
			SAT_STORE(dp[j + 4], res4);
			SAT_STORE(dp[j + 5], res5);
			SAT_STORE(dp[j + 6], res6);
			SAT_STORE(dp[j + 7], res7);
			a0 = sb[6];
			a1 = sb[7];
		}

		if (j < width - 3) {
			MLIB_BIC_LDLSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
			res0 = BC_SUM2(a0, a1, sa[0], sa[1]);
			res1 = BC_SUM2(sa[0], sa[1], sa[2], sa[3]);
			res2 = BC_SUM2(sa[2], sa[3], sa[4], sa[5]);
			res3 = BC_SUM2(sa[4], sa[5], sa[6], sa[7]);
			SAT_STORE(dp[j], res0);
			SAT_STORE(dp[j + 1], res1);
			SAT_STORE(dp[j + 2], res2);
			SAT_STORE(dp[j + 3], res3);
			a0 = sa[6];
			a1 = sa[7];
			j += 4;
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1], sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2], sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[j], res);
			a0 = a2;
			a1 = a3;
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

#endif

/* *********************************************************** */

#undef  CHAN1
#define	CHAN1	2

/* *********************************************************** */

#if defined(USED_FILTER_BICUBIC)

void ZOOM_FUNC(
    2_Bicubic) (
    BC_PARAMETERS(STYPE))

#else /* defined(USED_FILTER_BICUBIC) */

void ZOOM_FUNC(
    2_Bicubic2) (
    BC_PARAMETERS(STYPE))

#endif		   /* defined(USED_FILTER_BICUBIC) */

#if IMG_TYPE == 6	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	__m128d x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_sm0, x_sm1, x_sm2, x_sm3, x_sm4, x_sm5, x_sm6, x_sm7;
	__m128d x_st0, x_st1, x_st2, x_st3, x_st4, x_st5, x_st6, x_st7;
	__m128d x_sn0, x_sn1, x_sn2, x_sn3, x_sn4, x_sn5, x_sn6, x_sn7;
	mlib_d64 b0, b1, b2, b3;
	mlib_d64 res0, res1, res2, res3;
	mlib_d64 *sa = (mlib_d64 *)&x_st3;
	mlib_d64 *sb = (mlib_d64 *)&x_st7;
	mlib_d64 *sc = (mlib_d64 *)&x_sn3;
	mlib_d64 *sd = (mlib_d64 *)&x_sn7;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
			x_sd0 = _mm_loadu_pd(sp - CHAN1);
			x_sd1 = _mm_loadu_pd(sp);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				x_sd2 = _mm_loadu_pd(sp + BCHAN1);
				x_sd3 = _mm_loadu_pd(sp + BCHAN2);

				x_st0 = _mm_add_pd(x_sd1, x_sd2);
				x_st1 = _mm_add_pd(x_sd0, x_sd3);
				x_st2 = _mm_mul_pd(x_st0, x_const);
				x_st4 = _mm_sub_pd(x_st2, x_st1);
				x_st3 = _mm_mul_pd(x_st4, x_bc_sum1_div);
				_mm_storeu_pd(dp + CHAN1 * j, x_st3);
				x_sd0 = x_sd2;
				x_sd1 = x_sd3;
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDPD(0, j, x_sd0, x_sd1, x_sd2, x_sd3)
				x_st0 = _mm_add_pd(x_sd1, x_sd2);
				x_st1 = _mm_add_pd(x_sd0, x_sd3);
				x_st2 = _mm_mul_pd(x_st0, x_const);
				x_st3 = _mm_sub_pd(x_st2, x_st1);
				x_st4 = _mm_mul_pd(x_st3, x_bc_sum1_div);
				_mm_storeu_pd(dp + CHAN1 * j, x_st4);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
		    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
		b0 = BC_SUM(sp[-slb - CHAN1 + 1], sp[-CHAN1 + 1],
		    sp[slb - CHAN1 + 1], sp[2 * slb - CHAN1 + 1]);
		b1 = BC_SUM(sp[-slb + 1], sp[1], sp[slb + 1], sp[2 * slb + 1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDPD(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			MLIB_BIC_LDPD(CHAN2, j, x_sd4, x_sd5, x_sd6, x_sd7)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_st3 = _mm_sub_pd(x_st2, x_st1);

			x_st4 = _mm_add_pd(x_sd5, x_sd6);
			x_st5 = _mm_add_pd(x_sd4, x_sd7);
			x_st6 = _mm_mul_pd(x_st4, x_const);
			x_st7 = _mm_sub_pd(x_st6, x_st5);

			k = j + 1;
			MLIB_BIC_LDPD(CHAN1, k, x_sm0, x_sm1, x_sm2, x_sm3)

			MLIB_BIC_LDPD(CHAN2, k, x_sm4, x_sm5, x_sm6, x_sm7)

			x_sn0 = _mm_add_pd(x_sm1, x_sm2);
			x_sn1 = _mm_add_pd(x_sm0, x_sm3);
			x_sn2 = _mm_mul_pd(x_sn0, x_const);
			x_sn3 = _mm_sub_pd(x_sn2, x_sn1);

			x_sn4 = _mm_add_pd(x_sm5, x_sm6);
			x_sn5 = _mm_add_pd(x_sm4, x_sm7);
			x_sn6 = _mm_mul_pd(x_sn4, x_const);
			x_sn7 = _mm_sub_pd(x_sn6, x_sn5);

			res0 = BC_SUM2(a0, a1, sa[0], sb[0]);
			res1 = BC_SUM2(b0, b1, sa[1], sb[1]);
			res2 = BC_SUM2(sa[0], sb[0], sc[0], sd[0]);
			res3 = BC_SUM2(sa[1], sb[1], sc[1], sd[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);
			a0 = sc[0];
			a1 = sd[0];
			b0 = sc[1];
			b1 = sd[1];
		}

		if (j < width) {
			MLIB_BIC_LDPD(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			MLIB_BIC_LDPD(CHAN2, j, x_sd4, x_sd5, x_sd6, x_sd7)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_st3 = _mm_sub_pd(x_st2, x_st1);

			x_st4 = _mm_add_pd(x_sd5, x_sd6);
			x_st5 = _mm_add_pd(x_sd4, x_sd7);
			x_st6 = _mm_mul_pd(x_st4, x_const);
			x_st7 = _mm_sub_pd(x_st6, x_st5);

			res0 = BC_SUM2(a0, a1, sa[0], sb[0]);
			res1 = BC_SUM2(b0, b1, sa[1], sb[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
		}
	}
}

#elif IMG_TYPE == 5	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	mlib_f32 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128 x_bc_sum1_div = _mm_set1_ps(bc_sum1_div);
	const __m128 x_const = _mm_set1_ps(BC_SUM_CONST);
	__m128 x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128 x_st0, x_st1, x_st2, x_st3, x_st4, x_st5, x_st6, x_st7;
	mlib_f32 b0, b1, b2, b3;
	mlib_f32 res0, res1, res2, res3;
	mlib_f32 *sa = (mlib_f32 *)&x_st3;
	mlib_f32 *sb = (mlib_f32 *)&x_st7;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {

					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDPS(0, j, x_sd0, x_sd1, x_sd2, x_sd3)
				x_st0 = _mm_add_ps(x_sd1, x_sd2);
				x_st1 = _mm_add_ps(x_sd0, x_sd3);
				x_st2 = _mm_mul_ps(x_st0, x_const);
				x_st4 = _mm_sub_ps(x_st2, x_st1);
				x_st3 = _mm_mul_ps(x_st4, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
		    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
		b0 = BC_SUM(sp[-slb - CHAN1 + 1], sp[-CHAN1 + 1],
		    sp[slb - CHAN1 + 1], sp[2 * slb - CHAN1 + 1]);
		b1 = BC_SUM(sp[-slb + 1], sp[1], sp[slb + 1], sp[2 * slb + 1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			k = j + 1;
			MLIB_BIC_LDPS(CHAN1, k, x_sd4, x_sd5, x_sd6, x_sd7)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_st3 = _mm_sub_ps(x_st2, x_st1);

			x_st4 = _mm_add_ps(x_sd5, x_sd6);
			x_st5 = _mm_add_ps(x_sd4, x_sd7);
			x_st6 = _mm_mul_ps(x_st4, x_const);
			x_st7 = _mm_sub_ps(x_st6, x_st5);

			res0 = BC_SUM2(a0, a1, sa[0], sa[2]);
			res1 = BC_SUM2(b0, b1, sa[1], sa[3]);
			res2 = BC_SUM2(sa[0], sa[2], sb[0], sb[2]);
			res3 = BC_SUM2(sa[1], sa[3], sb[1], sb[3]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);
			a0 = sb[0];
			a1 = sb[2];
			b0 = sb[1];
			b1 = sb[3];
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1],
			    sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2],
			    sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res0 = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[CHAN1 * j], res0);

			b2 = BC_SUM(sp[BCHAN1 - slb + 1],
			    sp[BCHAN1 + 1],
			    sp[BCHAN1 + slb + 1],
			    sp[BCHAN1 + 2 * slb + 1]);
			b3 = BC_SUM(sp[BCHAN2 - slb + 1],
			    sp[BCHAN2 + 1],
			    sp[BCHAN2 + slb + 1],
			    sp[BCHAN2 + 2 * slb + 1]);
			res1 = BC_SUM2(b0, b1, b2, b3);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			a0 = a2;
			a1 = a3;
			b0 = b2;
			b1 = b3;
		}
	}
}

#elif IMG_TYPE == 4	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128d x_sd4_l, x_sd5_l, x_sd6_l, x_sd7_l;
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128d x_sd4_h, x_sd5_h, x_sd6_h, x_sd7_h;
	__m128d x_st0_l, x_st1_l, x_st2_l, x_st3_l;
	__m128d x_st4_l, x_st5_l, x_st6_l, x_st7_l;
	__m128d x_st0_h, x_st1_h, x_st2_h, x_st3_h;
	__m128d x_st4_h, x_st5_h, x_st6_h, x_st7_h;
	mlib_d64 b0, b1, b2, b3;
	mlib_d64 res0, res1, res2, res3;
	mlib_d64 *sal = (mlib_d64 *)&x_st3_l;
	mlib_d64 *sah = (mlib_d64 *)&x_st3_h;
	mlib_d64 *sbl = (mlib_d64 *)&x_st7_l;
	mlib_d64 *sbh = (mlib_d64 *)&x_st7_h;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i + k;
			dp = p_dst + dlb * i + k;
			x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1));
			x_sd0_l = _mm_cvtepi32_pd(x_sd0);
			x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				x_sd1 = _mm_loadu_si128((void *)(sp + BCHAN1));
				x_sd1_l = _mm_cvtepi32_pd(x_sd1);
				x_sd1_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));

				x_st0_l = _mm_add_pd(x_sd0_h, x_sd1_l);
				x_st1_l = _mm_add_pd(x_sd0_l, x_sd1_h);
				x_st2_l = _mm_mul_pd(x_st0_l, x_const);
				x_st4_l = _mm_sub_pd(x_st2_l, x_st1_l);
				x_st3_l = _mm_mul_pd(x_st4_l, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
				x_sd0_l = x_sd1_l;
				x_sd0_h = x_sd1_h;
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_cvtepi32_pd(x_sd0);
				x_sd1_l = _mm_cvtepi32_pd(x_sd1);
				x_sd2_l = _mm_cvtepi32_pd(x_sd2);
				x_sd3_l = _mm_cvtepi32_pd(x_sd3);

				x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
				x_st2_l = _mm_mul_pd(x_st0_l, x_const);
				x_st4_l = _mm_sub_pd(x_st2_l, x_st1_l);
				x_st3_l = _mm_mul_pd(x_st4_l, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
		    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
		b0 = BC_SUM(sp[-slb - CHAN1 + 1], sp[-CHAN1 + 1],
		    sp[slb - CHAN1 + 1], sp[2 * slb - CHAN1 + 1]);
		b1 = BC_SUM(sp[-slb + 1], sp[1], sp[slb + 1], sp[2 * slb + 1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			k = j + 1;
			MLIB_BIC_LDSI(CHAN1, k, x_sd4, x_sd5, x_sd6, x_sd7)

			x_sd0_l = _mm_cvtepi32_pd(x_sd0);
			x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
			x_sd1_l = _mm_cvtepi32_pd(x_sd1);
			x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
			x_sd2_l = _mm_cvtepi32_pd(x_sd2);
			x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
			x_sd3_l = _mm_cvtepi32_pd(x_sd3);
			x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

			x_sd4_l = _mm_cvtepi32_pd(x_sd4);
			x_sd4_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd4, 8));
			x_sd5_l = _mm_cvtepi32_pd(x_sd5);
			x_sd5_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd5, 8));
			x_sd6_l = _mm_cvtepi32_pd(x_sd6);
			x_sd6_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd6, 8));
			x_sd7_l = _mm_cvtepi32_pd(x_sd7);
			x_sd7_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd7, 8));

			x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
			x_st2_l = _mm_mul_pd(x_st0_l, x_const);
			x_st2_h = _mm_mul_pd(x_st0_h, x_const);
			x_st3_l = _mm_sub_pd(x_st2_l, x_st1_l);
			x_st3_h = _mm_sub_pd(x_st2_h, x_st1_h);

			x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
			x_st4_h = _mm_add_pd(x_sd5_h, x_sd6_h);
			x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
			x_st5_h = _mm_add_pd(x_sd4_h, x_sd7_h);
			x_st6_l = _mm_mul_pd(x_st4_l, x_const);
			x_st6_h = _mm_mul_pd(x_st4_h, x_const);
			x_st7_l = _mm_sub_pd(x_st6_l, x_st5_l);
			x_st7_h = _mm_sub_pd(x_st6_h, x_st5_h);

			res0 = BC_SUM2(a0, a1, sal[0], sah[0]);
			res1 = BC_SUM2(b0, b1, sal[1], sah[1]);
			res2 = BC_SUM2(sal[0], sah[0], sbl[0], sbh[0]);
			res3 = BC_SUM2(sal[1], sah[1], sbl[1], sbh[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);
			a0 = sbl[0];
			a1 = sbh[0];
			b0 = sbl[1];
			b1 = sbh[1];
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1],
			    sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2],
			    sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res0 = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[CHAN1 * j], res0);

			b2 = BC_SUM(sp[BCHAN1 - slb + 1],
			    sp[BCHAN1 + 1],
			    sp[BCHAN1 + slb + 1],
			    sp[BCHAN1 + 2 * slb + 1]);
			b3 = BC_SUM(sp[BCHAN2 - slb + 1],
			    sp[BCHAN2 + 1],
			    sp[BCHAN2 + slb + 1],
			    sp[BCHAN2 + 2 * slb + 1]);
			res1 = BC_SUM2(b0, b1, b2, b3);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			a0 = a2;
			a1 = a3;
			b0 = b2;
			b1 = b3;
		}
	}
}

#elif IMG_TYPE == 3	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0_l, x_st1_l, x_st2_l, x_st3_l, x_st4_l, x_st5_l, x_st6_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st3_h, x_st4_h, x_st5_h, x_st6_h;
	__m128i x_dd0_l, x_dd1_l, x_dd2_l;
	__m128i x_dd0_h, x_dd1_h, x_dd2_h;

	mlib_s32 b0, b1, b2, b3;
	mlib_s32 res0, res1, res2, res3;
	mlib_s32 *sal = (mlib_s32 *)&x_st5_l;
	mlib_s32 *sah = (mlib_s32 *)&x_st5_h;
	mlib_s32 *sbl = (mlib_s32 *)&x_st6_l;
	mlib_s32 *sbh = (mlib_s32 *)&x_st6_h;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
			x_sd0 = _mm_loadl_epi64((void *)(sp - CHAN1));
			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_dd0_l = _mm_unpacklo_epi64(x_sd0_l, x_zero);
			x_dd0_h = _mm_unpackhi_epi64(x_sd0_l, x_zero);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_sd0 = _mm_loadu_si128((void *)(sp + BCHAN1));
				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_dd1_l = _mm_unpacklo_epi64(x_sd0_l, x_zero);
				x_dd1_h = _mm_unpackhi_epi64(x_sd0_l, x_zero);
				x_dd2_l = _mm_unpacklo_epi64(x_sd0_h, x_zero);
				x_dd2_h = _mm_unpackhi_epi64(x_sd0_h, x_zero);

				x_st0_l = _mm_add_epi32(x_dd0_h, x_dd1_l);
				x_st0_h = _mm_add_epi32(x_dd0_l, x_dd1_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
				x_st4_h = _mm_sub_epi32(x_st4_l, x_st0_h);
				x_st5_l =
				    _mm_srai_epi32(x_st4_h, BC_SUM1_CONST);

				x_st0_l = _mm_add_epi32(x_dd1_h, x_dd2_l);
				x_st0_h = _mm_add_epi32(x_dd1_l, x_dd2_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
				x_st4_h = _mm_sub_epi32(x_st4_l, x_st0_h);
				x_st5_h =
				    _mm_srai_epi32(x_st4_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
				SAT_STORE(dp[CHAN1 * (j + 1)], sah[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sah[1]);
				x_dd0_l = x_dd2_l;
				x_dd0_h = x_dd2_h;
			}

			if (j < width) {
				x_sd0 = _mm_loadl_epi64((void *)(sp + BCHAN1));
				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_dd1_l = _mm_unpacklo_epi64(x_sd0_l, x_zero);
				x_dd1_h = _mm_unpackhi_epi64(x_sd0_l, x_zero);

				x_st0_l = _mm_add_epi32(x_dd0_h, x_dd1_l);
				x_st0_h = _mm_add_epi32(x_dd0_l, x_dd1_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
				x_st4_h = _mm_sub_epi32(x_st4_l, x_st0_h);
				x_st5_l =
				    _mm_srai_epi32(x_st4_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_st5_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_st5_l =
				    _mm_srai_epi32(x_st5_l, BC_SUM1_CONST);
				x_st5_h =
				    _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
				SAT_STORE(dp[CHAN1 * (j + 1)], sah[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sah[1]);
			}

			if (j < width) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_st5_l =
				    _mm_srai_epi32(x_st5_l, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
		    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
		b0 = BC_SUM(sp[-slb - CHAN1 + 1], sp[-CHAN1 + 1],
		    sp[slb - CHAN1 + 1], sp[2 * slb - CHAN1 + 1]);
		b1 = BC_SUM(sp[-slb + 1], sp[1], sp[slb + 1], sp[2 * slb + 1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_st5_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(a0, a1, sal[0], sal[2]);
			res1 = BC_SUM2(b0, b1, sal[1], sal[3]);
			res2 = BC_SUM2(sal[0], sal[2], sah[0], sah[2]);
			res3 = BC_SUM2(sal[1], sal[3], sah[1], sah[3]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);

			k = j + 2;
			MLIB_BIC_LDSI(CHAN1, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st6_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_st6_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(sah[0], sah[2], sbl[0], sbl[2]);
			res1 = BC_SUM2(sah[1], sah[3], sbl[1], sbl[3]);
			res2 = BC_SUM2(sbl[0], sbl[2], sbh[0], sbh[2]);
			res3 = BC_SUM2(sbl[1], sbl[3], sbh[1], sbh[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * (k + 1)], res2);
			SAT_STORE(dp[CHAN1 * (k + 1) + 1], res3);

			a0 = sbh[0];
			a1 = sbh[2];
			b0 = sbh[1];
			b1 = sbh[3];
		}

		if (j < width - 1) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_st5_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(a0, a1, sal[0], sal[2]);
			res1 = BC_SUM2(b0, b1, sal[1], sal[3]);
			res2 = BC_SUM2(sal[0], sal[2], sah[0], sah[2]);
			res3 = BC_SUM2(sal[1], sal[3], sah[1], sah[3]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);

			a0 = sah[0];
			a1 = sah[2];
			b0 = sah[1];
			b1 = sah[3];
			j += 2;
		}

		if (j < width) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1],
			    sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2],
			    sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res0 = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[CHAN1 * j], res0);

			b2 = BC_SUM(sp[BCHAN1 - slb + 1],
			    sp[BCHAN1 + 1],
			    sp[BCHAN1 + slb + 1],
			    sp[BCHAN1 + 2 * slb + 1]);
			b3 = BC_SUM(sp[BCHAN2 - slb + 1],
			    sp[BCHAN2 + 1],
			    sp[BCHAN2 + slb + 1],
			    sp[BCHAN2 + 2 * slb + 1]);
			res1 = BC_SUM2(b0, b1, b2, b3);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
		}
	}
}

#elif IMG_TYPE == 2	/* IMG_TYPE == 6 */

{
	DEF_VARS();

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_si0, x_si1, x_si2, x_si3;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0_l, x_st1_l, x_st2_l, x_st3_l, x_st4_l, x_st5_l, x_st6_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st3_h, x_st4_h, x_st5_h, x_st6_h;
	__m128i x_dd0_l, x_dd1_l, x_dd2_l;
	__m128i x_dd0_h, x_dd1_h, x_dd2_h;
	mlib_s32 b0, b1, b2, b3;
	mlib_s32 res0, res1, res2, res3;
	mlib_s32 *sal = (mlib_s32 *)&x_st5_l;
	mlib_s32 *sah = (mlib_s32 *)&x_st5_h;
	mlib_s32 *sbl = (mlib_s32 *)&x_st6_l;
	mlib_s32 *sbh = (mlib_s32 *)&x_st6_h;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
			x_sd0 = _mm_loadl_epi64((void *)(sp - CHAN1));
			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_dd0_l = _mm_unpacklo_epi64(x_sd0_l, x_zero);
			x_dd0_h = _mm_unpackhi_epi64(x_sd0_l, x_zero);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_sd0 = _mm_loadu_si128((void *)(sp + BCHAN1));
				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
				x_dd1_l = _mm_unpacklo_epi64(x_sd0_l, x_zero);
				x_dd1_h = _mm_unpackhi_epi64(x_sd0_l, x_zero);
				x_dd2_l = _mm_unpacklo_epi64(x_sd0_h, x_zero);
				x_dd2_h = _mm_unpackhi_epi64(x_sd0_h, x_zero);

				x_st0_l = _mm_add_epi32(x_dd0_h, x_dd1_l);
				x_st0_h = _mm_add_epi32(x_dd0_l, x_dd1_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
				x_st4_h = _mm_sub_epi32(x_st4_l, x_st0_h);
				x_st5_l =
				    _mm_srai_epi32(x_st4_h, BC_SUM1_CONST);

				x_st0_l = _mm_add_epi32(x_dd1_h, x_dd2_l);
				x_st0_h = _mm_add_epi32(x_dd1_l, x_dd2_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
				x_st4_h = _mm_sub_epi32(x_st4_l, x_st0_h);
				x_st5_h =
				    _mm_srai_epi32(x_st4_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
				SAT_STORE(dp[CHAN1 * (j + 1)], sah[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sah[1]);
				x_dd0_l = x_dd2_l;
				x_dd0_h = x_dd2_h;
			}

			if (j < width) {
				x_sd0 = _mm_loadl_epi64((void *)(sp + BCHAN1));
				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_dd1_l = _mm_unpacklo_epi64(x_sd0_l, x_zero);
				x_dd1_h = _mm_unpackhi_epi64(x_sd0_l, x_zero);

				x_st0_l = _mm_add_epi32(x_dd0_h, x_dd1_l);
				x_st0_h = _mm_add_epi32(x_dd0_l, x_dd1_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
				x_st4_h = _mm_sub_epi32(x_st4_l, x_st0_h);
				x_st5_l =
				    _mm_srai_epi32(x_st4_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_st5_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_st5_l =
				    _mm_srai_epi32(x_st5_l, BC_SUM1_CONST);
				x_st5_h =
				    _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
				SAT_STORE(dp[CHAN1 * (j + 1)], sah[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sah[1]);
			}

			if (j < width) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_st5_l =
				    _mm_srai_epi32(x_st5_l, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sal[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sal[1]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
		    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
		b0 = BC_SUM(sp[-slb - CHAN1 + 1], sp[-CHAN1 + 1],
		    sp[slb - CHAN1 + 1], sp[2 * slb - CHAN1 + 1]);
		b1 = BC_SUM(sp[-slb + 1], sp[1], sp[slb + 1], sp[2 * slb + 1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_st5_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(a0, a1, sal[0], sal[2]);
			res1 = BC_SUM2(b0, b1, sal[1], sal[3]);
			res2 = BC_SUM2(sal[0], sal[2], sah[0], sah[2]);
			res3 = BC_SUM2(sal[1], sal[3], sah[1], sah[3]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);

			k = j + 2;
			MLIB_BIC_LDSI(CHAN1, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st6_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_st6_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(sah[0], sah[2], sbl[0], sbl[2]);
			res1 = BC_SUM2(sah[1], sah[3], sbl[1], sbl[3]);
			res2 = BC_SUM2(sbl[0], sbl[2], sbh[0], sbh[2]);
			res3 = BC_SUM2(sbl[1], sbl[3], sbh[1], sbh[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * (k + 1)], res2);
			SAT_STORE(dp[CHAN1 * (k + 1) + 1], res3);

			a0 = sbh[0];
			a1 = sbh[2];
			b0 = sbh[1];
			b1 = sbh[3];
		}

		if (j < width - 1) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st5_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_st5_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(a0, a1, sal[0], sal[2]);
			res1 = BC_SUM2(b0, b1, sal[1], sal[3]);
			res2 = BC_SUM2(sal[0], sal[2], sah[0], sah[2]);
			res3 = BC_SUM2(sal[1], sal[3], sah[1], sah[3]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);

			a0 = sah[0];
			a1 = sah[2];
			b0 = sah[1];
			b1 = sah[3];
			j += 2;
		}

		if (j < width) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1],
			    sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2],
			    sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res0 = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[CHAN1 * j], res0);

			b2 = BC_SUM(sp[BCHAN1 - slb + 1],
			    sp[BCHAN1 + 1],
			    sp[BCHAN1 + slb + 1],
			    sp[BCHAN1 + 2 * slb + 1]);
			b3 = BC_SUM(sp[BCHAN2 - slb + 1],
			    sp[BCHAN2 + 1],
			    sp[BCHAN2 + slb + 1],
			    sp[BCHAN2 + 2 * slb + 1]);
			res1 = BC_SUM2(b0, b1, b2, b3);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
		}
	}
}

#elif IMG_TYPE == 1	/* IMG_TYPE == 6 */

{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	const __m128i x_const = _mm_set1_epi16(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_st0, x_st1, x_st2, x_st3, x_st4, x_st5, x_st6, x_st7;
	__m128i x_sl0, x_sl1, x_sl2, x_sl3, x_sl4, x_sl5;
	mlib_s32 b0, b1, b2, b3;
	mlib_s32 res0, res1, res2, res3, res4, res5, res6, res7;
	mlib_s16 *sa = (mlib_s16 *)&x_sl4;
	mlib_s16 *sb = (mlib_s16 *)&x_sl5;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {

				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
				x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

				x_sl0 = _mm_add_epi16(x_st2, x_st4);
				x_sl1 = _mm_add_epi16(x_st3, x_st5);
				x_sl2 = _mm_add_epi16(x_st0, x_st6);
				x_sl3 = _mm_add_epi16(x_st1, x_st7);

				x_st0 = _mm_mullo_epi16(x_sl0, x_const);
				x_st1 = _mm_mullo_epi16(x_sl1, x_const);
				x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
				x_sl5 = _mm_sub_epi16(x_st1, x_sl3);
				x_sl4 = _mm_srai_epi16(x_sl4, BC_SUM1_CONST);
				x_sl5 = _mm_srai_epi16(x_sl5, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * (j + 1)], sa[4]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[5]);

				SAT_STORE(dp[CHAN1 * (j + 2)], sb[0]);
				SAT_STORE(dp[CHAN1 * (j + 2) + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * (j + 3)], sb[4]);
				SAT_STORE(dp[CHAN1 * (j + 3) + 1], sb[5]);
			}

			if (j < width - 1) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

				x_sl0 = _mm_add_epi16(x_st2, x_st4);
				x_sl2 = _mm_add_epi16(x_st0, x_st6);

				x_st0 = _mm_mullo_epi16(x_sl0, x_const);
				x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
				x_sl4 = _mm_srai_epi16(x_sl4, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * (j + 1)], sa[4]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[5]);
				j += 2;
			}

			if (j < width) {
				a0 = sp[CHAN2 * j - slb];
				a1 = sp[CHAN2 * j];
				a2 = sp[CHAN2 * j + slb];
				a3 = sp[CHAN2 * j + 2 * slb];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[CHAN1 * j], res);

				a0 = sp[CHAN2 * j - slb + 1];
				a1 = sp[CHAN2 * j + 1];
				a2 = sp[CHAN2 * j + slb + 1];
				a3 = sp[CHAN2 * j + 2 * slb + 1];
				res = BC_SUM1(a0, a1, a2, a3);
				SAT_STORE(dp[CHAN1 * j + 1], res);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		a0 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
		    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
		a1 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
		b0 = BC_SUM(sp[-slb - CHAN1 + 1], sp[-CHAN1 + 1],
		    sp[slb - CHAN1 + 1], sp[2 * slb - CHAN1 + 1]);
		b1 = BC_SUM(sp[-slb + 1], sp[1], sp[slb + 1], sp[2 * slb + 1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_sl4 = _mm_sub_epi16(x_st0, x_sl2);
			x_sl5 = _mm_sub_epi16(x_st1, x_sl3);

			res0 = BC_SUM2(a0, a1, sa[0], sa[2]);
			res1 = BC_SUM2(b0, b1, sa[1], sa[3]);
			res2 = BC_SUM2(sa[0], sa[2], sa[4], sa[6]);
			res3 = BC_SUM2(sa[1], sa[3], sa[5], sa[7]);
			res4 = BC_SUM2(sa[4], sa[6], sb[0], sb[2]);
			res5 = BC_SUM2(sa[5], sa[7], sb[1], sb[3]);
			res6 = BC_SUM2(sb[0], sb[2], sb[4], sb[6]);
			res7 = BC_SUM2(sb[1], sb[3], sb[5], sb[7]);

			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1)], res2);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res3);
			SAT_STORE(dp[CHAN1 * (j + 2)], res4);
			SAT_STORE(dp[CHAN1 * (j + 2) + 1], res5);
			SAT_STORE(dp[CHAN1 * (j + 3)], res6);
			SAT_STORE(dp[CHAN1 * (j + 3) + 1], res7);

			a0 = sb[4];
			a1 = sb[6];
			b0 = sb[5];
			b1 = sb[7];
		}

		for (; j < width; j++) {
			a2 = BC_SUM(sp[BCHAN1 - slb],
			    sp[BCHAN1],
			    sp[BCHAN1 + slb],
			    sp[BCHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[BCHAN2 - slb],
			    sp[BCHAN2],
			    sp[BCHAN2 + slb],
			    sp[BCHAN2 + 2 * slb]);
			res0 = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[CHAN1 * j], res0);

			b2 = BC_SUM(sp[BCHAN1 - slb + 1],
			    sp[BCHAN1 + 1],
			    sp[BCHAN1 + slb + 1],
			    sp[BCHAN1 + 2 * slb + 1]);
			b3 = BC_SUM(sp[BCHAN2 - slb + 1],
			    sp[BCHAN2 + 1],
			    sp[BCHAN2 + slb + 1],
			    sp[BCHAN2 + 2 * slb + 1]);
			res1 = BC_SUM2(b0, b1, b2, b3);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			a0 = a2;
			a1 = a3;
			b0 = b2;
			b1 = b3;
		}
	}
}

#endif

/* *********************************************************** */

#undef  CHAN1
#define	CHAN1	3

/* *********************************************************** */

#if defined(USED_FILTER_BICUBIC)

void ZOOM_FUNC(
    3_Bicubic) (
    BC_PARAMETERS(STYPE))

#else /* defined(USED_FILTER_BICUBIC) */

void ZOOM_FUNC(
    3_Bicubic2) (
    BC_PARAMETERS(STYPE))

#endif		   /* defined(USED_FILTER_BICUBIC) */

#if IMG_TYPE == 6	/* IMG_TYPE == 6 */
{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	__m128d x_sd0, x_sd1, x_sd2, x_sd3;
	__m128d x_st0, x_st1, x_st2;
	__m128d x_dd0, x_dd1, x_dd2;
	__m128d x_dd3, x_dd4, x_dd5;
	__m128d x_dd6, x_dd7, x_dd8;
	mlib_d64 b0, b1, b2, b3, c0, c1, c2, c3;
	mlib_d64 res0, res1, res2;
	mlib_d64 *sa = (mlib_d64 *)&x_dd0;
	mlib_d64 *sb = (mlib_d64 *)&x_dd1;
	mlib_d64 *sc = (mlib_d64 *)&x_dd2;
	mlib_d64 *sd = (mlib_d64 *)&x_dd3;
	mlib_d64 *se = (mlib_d64 *)&x_dd4;
	mlib_d64 *sf = (mlib_d64 *)&x_dd5;
	mlib_d64 *sg = (mlib_d64 *)&x_dd6;
	mlib_d64 *sh = (mlib_d64 *)&x_dd7;
	mlib_d64 *si = (mlib_d64 *)&x_dd8;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				x_sd0 = _mm_loadu_pd(sp + CHAN2 * j - slb);
				x_sd1 = _mm_loadu_pd(sp + CHAN2 * j);
				x_sd2 = _mm_loadu_pd(sp + CHAN2 * j + slb);
				x_sd3 = _mm_loadu_pd(sp + CHAN2 * j + 2 * slb);

				x_st0 = _mm_add_pd(x_sd1, x_sd2);
				x_st1 = _mm_add_pd(x_sd0, x_sd3);
				x_st2 = _mm_mul_pd(x_st0, x_const);
				x_dd0 = _mm_sub_pd(x_st2, x_st1);
				x_dd1 = _mm_mul_pd(x_dd0, x_bc_sum1_div);
				_mm_storeu_pd(dp + CHAN1 * j, x_dd1);

				x_sd0 = _mm_loadu_pd(sp + CHAN2 * j - slb + 2);
				x_sd1 = _mm_loadu_pd(sp + CHAN2 * j + 2);
				x_sd2 = _mm_loadu_pd(sp + CHAN2 * j + slb + 2);
				x_sd3 =
				    _mm_loadu_pd(sp + CHAN2 * j + 2 * slb + 2);

				x_st0 = _mm_add_pd(x_sd1, x_sd2);
				x_st1 = _mm_add_pd(x_sd0, x_sd3);
				x_st2 = _mm_mul_pd(x_st0, x_const);
				x_dd0 = _mm_sub_pd(x_st2, x_st1);
				x_dd1 = _mm_mul_pd(x_dd0, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		x_sd0 = _mm_loadu_pd(sp - CHAN1 - slb);
		x_sd1 = _mm_loadu_pd(sp - CHAN1);
		x_sd2 = _mm_loadu_pd(sp - CHAN1 + slb);
		x_sd3 = _mm_loadu_pd(sp - CHAN1 + 2 * slb);

		x_st0 = _mm_add_pd(x_sd1, x_sd2);
		x_st1 = _mm_add_pd(x_sd0, x_sd3);
		x_st2 = _mm_mul_pd(x_st0, x_const);
		x_dd0 = _mm_sub_pd(x_st2, x_st1);

		x_sd0 = _mm_loadu_pd(sp - CHAN1 + 2 - slb);
		x_sd1 = _mm_loadu_pd(sp - CHAN1 + 2);
		x_sd2 = _mm_loadu_pd(sp - CHAN1 + slb + 2);
		x_sd3 = _mm_loadu_pd(sp - CHAN1 + 2 * slb + 2);

		x_st0 = _mm_add_pd(x_sd1, x_sd2);
		x_st1 = _mm_add_pd(x_sd0, x_sd3);
		x_st2 = _mm_mul_pd(x_st0, x_const);
		x_dd1 = _mm_sub_pd(x_st2, x_st1);

		x_sd0 = _mm_loadu_pd(sp - CHAN1 + 4 - slb);
		x_sd1 = _mm_loadu_pd(sp - CHAN1 + 4);
		x_sd2 = _mm_loadu_pd(sp - CHAN1 + slb + 4);
		x_sd3 = _mm_loadu_pd(sp - CHAN1 + 2 * slb + 4);

		x_st0 = _mm_add_pd(x_sd1, x_sd2);
		x_st1 = _mm_add_pd(x_sd0, x_sd3);
		x_st2 = _mm_mul_pd(x_st0, x_const);
		x_dd2 = _mm_sub_pd(x_st2, x_st1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDPD(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd3 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1 + 2, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd4 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1 + 4, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd5 = _mm_sub_pd(x_st2, x_st1);

			res0 = BC_SUM2(sa[0], sb[1], sd[0], se[1]);
			res1 = BC_SUM2(sa[1], sc[0], sd[1], sf[0]);
			res2 = BC_SUM2(sb[0], sc[1], se[0], sf[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			MLIB_BIC_LDPD(CHAN1, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd6 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1 + 2, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd7 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1 + 4, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd8 = _mm_sub_pd(x_st2, x_st1);

			res0 = BC_SUM2(sd[0], se[1], sg[0], sh[1]);
			res1 = BC_SUM2(sd[1], sf[0], sg[1], si[0]);
			res2 = BC_SUM2(se[0], sf[1], sh[0], si[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			x_dd0 = x_dd6;
			x_dd1 = x_dd7;
			x_dd2 = x_dd8;
		}

		if (j < width) {
			MLIB_BIC_LDPD(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd3 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1 + 2, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd4 = _mm_sub_pd(x_st2, x_st1);

			MLIB_BIC_LDPD(CHAN1 + 4, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_pd(x_sd1, x_sd2);
			x_st1 = _mm_add_pd(x_sd0, x_sd3);
			x_st2 = _mm_mul_pd(x_st0, x_const);
			x_dd5 = _mm_sub_pd(x_st2, x_st1);

			res0 = BC_SUM2(sa[0], sb[1], sd[0], se[1]);
			res1 = BC_SUM2(sa[1], sc[0], sd[1], sf[0]);
			res2 = BC_SUM2(sb[0], sc[1], se[0], sf[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);
		}
	}
}

#elif IMG_TYPE == 5	/* IMG_TYPE == 6 */
{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	mlib_f32 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128 x_bc_sum1_div = _mm_set1_ps(bc_sum1_div);
	const __m128 x_const = _mm_set1_ps(BC_SUM_CONST);
	__m128 x_sd0, x_sd1, x_sd2, x_sd3;
	__m128 x_st0, x_st1, x_st2;
	__m128 x_dd0, x_dd1, x_dd2;
	mlib_f32 b0, b1, b2, b3, c0, c1, c2, c3;
	mlib_f32 res0, res1, res2;
	mlib_f32 *sa = (mlib_f32 *)&x_dd0;
	mlib_f32 *sb = (mlib_f32 *)&x_dd1;
	mlib_f32 *sc = (mlib_f32 *)&x_dd2;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDPS(
				    0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_add_ps(x_sd1, x_sd2);
				x_st1 = _mm_add_ps(x_sd0, x_sd3);
				x_st2 = _mm_mul_ps(x_st0, x_const);
				x_dd0 = _mm_sub_ps(x_st2, x_st1);
				x_dd1 = _mm_mul_ps(x_dd0, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;
		x_sd0 = _mm_loadu_ps(sp - CHAN1 - slb);
		x_sd1 = _mm_loadu_ps(sp - CHAN1);
		x_sd2 = _mm_loadu_ps(sp - CHAN1 + slb);
		x_sd3 = _mm_loadu_ps(sp - CHAN1 + 2 * slb);

		x_st0 = _mm_add_ps(x_sd1, x_sd2);
		x_st1 = _mm_add_ps(x_sd0, x_sd3);
		x_st2 = _mm_mul_ps(x_st0, x_const);
		x_dd0 = _mm_sub_ps(x_st2, x_st1);

		x_sd0 = _mm_loadu_ps(sp - CHAN1 - slb + 4);
		x_sd1 = _mm_loadu_ps(sp - CHAN1 + 4);
		x_sd2 = _mm_loadu_ps(sp - CHAN1 + slb + 4);
		x_sd3 = _mm_loadu_ps(sp - CHAN1 + 2 * slb + 4);

		x_st0 = _mm_add_ps(x_sd1, x_sd2);
		x_st1 = _mm_add_ps(x_sd0, x_sd3);
		x_st2 = _mm_mul_ps(x_st0, x_const);
		x_dd1 = _mm_sub_ps(x_st2, x_st1);
		a0 = sa[0];
		a1 = sa[3];
		b0 = sa[1];
		b1 = sb[0];
		c0 = sa[2];
		c1 = sb[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd0 = _mm_sub_ps(x_st2, x_st1);

			MLIB_BIC_LDPS(CHAN1 + 4, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd1 = _mm_sub_ps(x_st2, x_st1);

			MLIB_BIC_LDPS(CHAN1 + 8, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd2 = _mm_sub_ps(x_st2, x_st1);

			res0 = BC_SUM2(a0, a1, sa[0], sa[3]);
			res1 = BC_SUM2(b0, b1, sa[1], sb[0]);
			res2 = BC_SUM2(c0, c1, sa[2], sb[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			res0 = BC_SUM2(sa[0], sa[3], sb[2], sc[1]);
			res1 = BC_SUM2(sa[1], sb[0], sb[3], sc[2]);
			res2 = BC_SUM2(sa[2], sb[1], sc[0], sc[3]);
			SAT_STORE(dp[CHAN1 * (j + 1)], res0);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], res1);
			SAT_STORE(dp[CHAN1 * (j + 1) + 2], res2);
			a0 = sb[2];
			a1 = sc[1];
			b0 = sb[3];
			b1 = sc[2];
			c0 = sc[0];
			c1 = sc[3];
		}

		if (j < width) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd0 = _mm_sub_ps(x_st2, x_st1);

			MLIB_BIC_LDPS(CHAN1 + 4, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd1 = _mm_sub_ps(x_st2, x_st1);

			res0 = BC_SUM2(a0, a1, sa[0], sa[3]);
			res1 = BC_SUM2(b0, b1, sa[1], sb[0]);
			res2 = BC_SUM2(c0, c1, sa[2], sb[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);
		}
	}
}

#elif IMG_TYPE == 4	/* IMG_TYPE == 6 */
{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_sd8, x_sd9, x_sd10, x_sd11;
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128d x_sd4_l, x_sd5_l, x_sd6_l, x_sd7_l;
	__m128d x_sd4_h, x_sd5_h, x_sd6_h, x_sd7_h;
	__m128d x_sd8_l, x_sd9_l, x_sd10_l, x_sd11_l;
	__m128d x_sd8_h, x_sd9_h, x_sd10_h, x_sd11_h;

	__m128d x_st0_l, x_st1_l, x_st2_l, x_st3_l;
	__m128d x_st0_h, x_st1_h, x_st2_h, x_st3_h;
	__m128d x_st4_l, x_st5_l, x_st6_l, x_st7_l;
	__m128d x_st4_h, x_st5_h, x_st6_h, x_st7_h;
	__m128d x_st8_l, x_st9_l, x_st10_l, x_st11_l;
	__m128d x_st8_h, x_st9_h, x_st10_h, x_st11_h;

	__m128d x_dd0_l, x_dd0_h, x_dd1_l;

	mlib_d64 res0, res1, res2;

	mlib_d64 *sa = (mlib_d64 *)&x_dd0_l;
	mlib_d64 *sb = (mlib_d64 *)&x_dd0_h;
	mlib_d64 *sc = (mlib_d64 *)&x_dd1_l;
	mlib_d64 *sd = (mlib_d64 *)&x_st3_l;
	mlib_d64 *se = (mlib_d64 *)&x_st3_h;
	mlib_d64 *sf = (mlib_d64 *)&x_st7_l;
	mlib_d64 *sg = (mlib_d64 *)&x_st7_h;
	mlib_d64 *sh = (mlib_d64 *)&x_st11_l;
	mlib_d64 *si = (mlib_d64 *)&x_st11_h;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_cvtepi32_pd(x_sd0);
				x_sd0_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
				x_sd1_l = _mm_cvtepi32_pd(x_sd1);
				x_sd1_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
				x_sd2_l = _mm_cvtepi32_pd(x_sd2);
				x_sd2_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
				x_sd3_l = _mm_cvtepi32_pd(x_sd3);
				x_sd3_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

				x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
				x_st2_l = _mm_mul_pd(x_st0_l, x_const);
				x_st2_h = _mm_mul_pd(x_st0_h, x_const);
				x_st3_l = _mm_sub_pd(x_st2_l, x_st1_l);
				x_st3_h = _mm_sub_pd(x_st2_h, x_st1_h);
				x_dd0_l = _mm_mul_pd(x_st3_l, x_bc_sum1_div);
				x_dd0_h = _mm_mul_pd(x_st3_h, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_si128((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_si128((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_si128((void *)(sp - CHAN1 + 2 * slb));

		x_sd4 = _mm_loadl_epi64((void *)(sp - CHAN1 - slb + 4));
		x_sd5 = _mm_loadl_epi64((void *)(sp - CHAN1 + 4));
		x_sd6 = _mm_loadl_epi64((void *)(sp - CHAN1 + slb + 4));
		x_sd7 = _mm_loadl_epi64((void *)(sp - CHAN1 + 2 * slb + 4));

		x_sd0_l = _mm_cvtepi32_pd(x_sd0);
		x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
		x_sd1_l = _mm_cvtepi32_pd(x_sd1);
		x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
		x_sd2_l = _mm_cvtepi32_pd(x_sd2);
		x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
		x_sd3_l = _mm_cvtepi32_pd(x_sd3);
		x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

		x_sd4_l = _mm_cvtepi32_pd(x_sd4);
		x_sd5_l = _mm_cvtepi32_pd(x_sd5);
		x_sd6_l = _mm_cvtepi32_pd(x_sd6);
		x_sd7_l = _mm_cvtepi32_pd(x_sd7);

		x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
		x_st2_l = _mm_mul_pd(x_st0_l, x_const);
		x_st2_h = _mm_mul_pd(x_st0_h, x_const);
		x_dd0_l = _mm_sub_pd(x_st2_l, x_st1_l);
		x_dd0_h = _mm_sub_pd(x_st2_h, x_st1_h);

		x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
		x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
		x_st6_l = _mm_mul_pd(x_st4_l, x_const);
		x_dd1_l = _mm_sub_pd(x_st6_l, x_st5_l);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			MLIB_BIC_LDSI(CHAN1 + 4, j, x_sd4, x_sd5, x_sd6, x_sd7)

			MLIB_BIC_LDSI(
			    CHAN1 + 8, j, x_sd8, x_sd9, x_sd10, x_sd11)

			x_sd0_l = _mm_cvtepi32_pd(x_sd0);
			x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
			x_sd1_l = _mm_cvtepi32_pd(x_sd1);
			x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
			x_sd2_l = _mm_cvtepi32_pd(x_sd2);
			x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
			x_sd3_l = _mm_cvtepi32_pd(x_sd3);
			x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

			x_sd4_l = _mm_cvtepi32_pd(x_sd4);
			x_sd4_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd4, 8));
			x_sd5_l = _mm_cvtepi32_pd(x_sd5);
			x_sd5_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd5, 8));
			x_sd6_l = _mm_cvtepi32_pd(x_sd6);
			x_sd6_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd6, 8));
			x_sd7_l = _mm_cvtepi32_pd(x_sd7);
			x_sd7_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd7, 8));

			x_sd8_l = _mm_cvtepi32_pd(x_sd8);
			x_sd8_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd8, 8));
			x_sd9_l = _mm_cvtepi32_pd(x_sd9);
			x_sd9_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd9, 8));
			x_sd10_l = _mm_cvtepi32_pd(x_sd10);
			x_sd10_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd10, 8));
			x_sd11_l = _mm_cvtepi32_pd(x_sd11);
			x_sd11_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd11, 8));

			x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
			x_st2_l = _mm_mul_pd(x_st0_l, x_const);
			x_st2_h = _mm_mul_pd(x_st0_h, x_const);
			x_st3_l = _mm_sub_pd(x_st2_l, x_st1_l);
			x_st3_h = _mm_sub_pd(x_st2_h, x_st1_h);

			x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
			x_st4_h = _mm_add_pd(x_sd5_h, x_sd6_h);
			x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
			x_st5_h = _mm_add_pd(x_sd4_h, x_sd7_h);
			x_st6_l = _mm_mul_pd(x_st4_l, x_const);
			x_st6_h = _mm_mul_pd(x_st4_h, x_const);
			x_st7_l = _mm_sub_pd(x_st6_l, x_st5_l);
			x_st7_h = _mm_sub_pd(x_st6_h, x_st5_h);

			x_st8_l = _mm_add_pd(x_sd9_l, x_sd10_l);
			x_st8_h = _mm_add_pd(x_sd9_h, x_sd10_h);
			x_st9_l = _mm_add_pd(x_sd8_l, x_sd11_l);
			x_st9_h = _mm_add_pd(x_sd8_h, x_sd11_h);
			x_st10_l = _mm_mul_pd(x_st8_l, x_const);
			x_st10_h = _mm_mul_pd(x_st8_h, x_const);
			x_st11_l = _mm_sub_pd(x_st10_l, x_st9_l);
			x_st11_h = _mm_sub_pd(x_st10_h, x_st9_h);

			res0 = BC_SUM2(sa[0], sb[1], sd[0], se[1]);
			res1 = BC_SUM2(sa[1], sc[0], sd[1], sf[0]);
			res2 = BC_SUM2(sb[0], sc[1], se[0], sf[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sd[0], se[1], sg[0], sh[1]);
			res1 = BC_SUM2(sd[1], sf[0], sg[1], si[0]);
			res2 = BC_SUM2(se[0], sf[1], sh[0], si[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			x_dd0_l = x_st7_h;
			x_dd0_h = x_st11_l;
			x_dd1_l = x_st11_h;
		}

		if (j < width) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd4 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 - slb + 4));
			x_sd5 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + 4));
			x_sd6 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + slb + 4));
			x_sd7 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + 2 * slb + 4));

			x_sd0_l = _mm_cvtepi32_pd(x_sd0);
			x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
			x_sd1_l = _mm_cvtepi32_pd(x_sd1);
			x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
			x_sd2_l = _mm_cvtepi32_pd(x_sd2);
			x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
			x_sd3_l = _mm_cvtepi32_pd(x_sd3);
			x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

			x_sd4_l = _mm_cvtepi32_pd(x_sd4);
			x_sd5_l = _mm_cvtepi32_pd(x_sd5);
			x_sd6_l = _mm_cvtepi32_pd(x_sd6);
			x_sd7_l = _mm_cvtepi32_pd(x_sd7);

			x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
			x_st2_l = _mm_mul_pd(x_st0_l, x_const);
			x_st2_h = _mm_mul_pd(x_st0_h, x_const);
			x_st3_l = _mm_sub_pd(x_st2_l, x_st1_l);
			x_st3_h = _mm_sub_pd(x_st2_h, x_st1_h);

			x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
			x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
			x_st6_l = _mm_mul_pd(x_st4_l, x_const);
			x_st7_l = _mm_sub_pd(x_st6_l, x_st5_l);

			res0 = BC_SUM2(sa[0], sb[1], sd[0], se[1]);
			res1 = BC_SUM2(sa[1], sc[0], sd[1], sf[0]);
			res2 = BC_SUM2(sb[0], sc[1], se[0], sf[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);
		}
	}
}

#elif IMG_TYPE == 3	/* IMG_TYPE == 6 */
{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_sd8, x_sd9, x_sd10, x_sd11;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_sd4_l, x_sd5_l, x_sd6_l, x_sd7_l;
	__m128i x_sd4_h, x_sd5_h, x_sd6_h, x_sd7_h;
	__m128i x_sd8_l, x_sd9_l, x_sd10_l, x_sd11_l;
	__m128i x_sd8_h, x_sd9_h, x_sd10_h, x_sd11_h;

	__m128i x_st0_l, x_st1_l, x_st2_l, x_st3_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st3_h;
	__m128i x_st4_l, x_st5_l, x_st6_l, x_st7_l;
	__m128i x_st4_h, x_st5_h, x_st6_h, x_st7_h;
	__m128i x_st8_l, x_st9_l, x_st10_l, x_st11_l;
	__m128i x_st8_h, x_st9_h, x_st10_h, x_st11_h;

	__m128i x_dd0_l, x_dd0_h, x_dd1_l, x_dd1_h;
	__m128i x_dd2_l, x_dd2_h, x_dd3_l, x_dd3_h;

	mlib_s32 res0, res1, res2;

	mlib_s32 sa[6];
	mlib_s32 *sb = (mlib_s32 *)&x_dd1_l;
	mlib_s32 *sc = (mlib_s32 *)&x_dd1_h;
	mlib_s32 *sd = (mlib_s32 *)&x_dd2_l;
	mlib_s32 *se = (mlib_s32 *)&x_dd2_h;
	mlib_s32 *sf = (mlib_s32 *)&x_dd3_l;
	mlib_s32 *sg = (mlib_s32 *)&x_dd3_h;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {

				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_dd1_l =
				    _mm_srai_epi32(x_dd1_l, BC_SUM1_CONST);
				x_dd1_h =
				    _mm_srai_epi32(x_dd1_h, BC_SUM1_CONST);

				MLIB_BIC_LDSI(8, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd2_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd2_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_dd2_l =
				    _mm_srai_epi32(x_dd2_l, BC_SUM1_CONST);
				x_dd2_h =
				    _mm_srai_epi32(x_dd2_h, BC_SUM1_CONST);

				MLIB_BIC_LDSI(16, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd3_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd3_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_dd3_l =
				    _mm_srai_epi32(x_dd3_l, BC_SUM1_CONST);
				x_dd3_h =
				    _mm_srai_epi32(x_dd3_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
				k = j + 1;
				SAT_STORE(dp[CHAN1 * k], sc[2]);
				SAT_STORE(dp[CHAN1 * k + 1], sc[3]);
				SAT_STORE(dp[CHAN1 * k + 2], sd[0]);
				k = j + 2;
				SAT_STORE(dp[CHAN1 * k], se[0]);
				SAT_STORE(dp[CHAN1 * k + 1], se[1]);
				SAT_STORE(dp[CHAN1 * k + 2], se[2]);
				k = j + 3;
				SAT_STORE(dp[CHAN1 * k], sf[2]);
				SAT_STORE(dp[CHAN1 * k + 1], sf[3]);
				SAT_STORE(dp[CHAN1 * k + 2], sg[0]);
			}

			for (; j < width; j++) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd1_l =
				    _mm_srai_epi32(x_dd1_l, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_si128((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_si128((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_si128((void *)(sp - CHAN1 + 2 * slb));

		x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
		x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
		x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
		x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
		x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
		x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
		x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

		x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

		x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_l, 32), x_const);
		x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

		x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_h, 32), x_const);
		x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

		x_dd0_l = _mm_sub_epi32(x_st4_l, x_st1_l);
		x_dd0_h = _mm_sub_epi32(x_st4_h, x_st1_h);
		_mm_storeu_si128((void *)(sa), x_dd0_l);
		_mm_storel_epi64((void *)(sa + 4), x_dd0_h);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			MLIB_BIC_LDSI(CHAN1 + 8, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd2_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd2_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			MLIB_BIC_LDSI(CHAN1 + 16, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd3_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd3_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sc[0]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sc[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sc[2], sd[1]);
			res1 = BC_SUM2(sb[1], sc[0], sc[3], sd[2]);
			res2 = BC_SUM2(sb[2], sc[1], sd[0], sd[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 2;
			res0 = BC_SUM2(sc[2], sd[1], se[0], se[3]);
			res1 = BC_SUM2(sc[3], sd[2], se[1], sf[0]);
			res2 = BC_SUM2(sd[0], sd[3], se[2], sf[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 3;
			res0 = BC_SUM2(se[0], se[3], sf[2], sg[1]);
			res1 = BC_SUM2(se[1], sf[0], sf[3], sg[2]);
			res2 = BC_SUM2(se[2], sf[1], sg[0], sg[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			sa[0] = sf[2];
			sa[1] = sf[3];
			sa[2] = sg[0];
			sa[3] = sg[1];
			sa[4] = sg[2];
			sa[5] = sg[3];
		}

		if (j < width - 1) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			MLIB_BIC_LDSI(CHAN1 + 8, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
			x_dd2_l = _mm_sub_epi32(x_st4_l, x_st1_l);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sc[0]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sc[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sc[2], sd[1]);
			res1 = BC_SUM2(sb[1], sc[0], sc[3], sd[2]);
			res2 = BC_SUM2(sb[2], sc[1], sd[0], sd[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			sa[0] = sc[2];
			sa[1] = sc[3];
			sa[2] = sd[0];
			sa[3] = sd[1];
			sa[4] = sd[2];
			sa[5] = sd[3];
			j += 2;
		}

		if (j < width) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sc[0]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sc[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);
		}
	}
}

#elif IMG_TYPE == 2	/* IMG_TYPE == 6 */
{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_si0, x_si1, x_si2, x_si3;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0_l, x_st1_l, x_st2_l, x_st3_l, x_st4_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st3_h, x_st4_h;
	__m128i x_dd0_l, x_dd0_h, x_dd1_l, x_dd1_h, x_dd2_l;
	__m128i x_dd2_h, x_dd3_l, x_dd3_h;

	mlib_s32 res0, res1, res2;
	mlib_s32 sa[6];
	mlib_s32 *sb = (mlib_s32 *)&x_dd1_l;
	mlib_s32 *sc = (mlib_s32 *)&x_dd1_h;
	mlib_s32 *sd = (mlib_s32 *)&x_dd2_l;
	mlib_s32 *se = (mlib_s32 *)&x_dd2_h;
	mlib_s32 *sf = (mlib_s32 *)&x_dd3_l;
	mlib_s32 *sg = (mlib_s32 *)&x_dd3_h;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {

				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_dd1_l =
				    _mm_srai_epi32(x_dd1_l, BC_SUM1_CONST);
				x_dd1_h =
				    _mm_srai_epi32(x_dd1_h, BC_SUM1_CONST);

				MLIB_BIC_LDSI(8, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd2_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd2_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_dd2_l =
				    _mm_srai_epi32(x_dd2_l, BC_SUM1_CONST);
				x_dd2_h =
				    _mm_srai_epi32(x_dd2_h, BC_SUM1_CONST);

				MLIB_BIC_LDSI(16, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
				x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_h, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd3_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd3_h = _mm_sub_epi32(x_st4_h, x_st1_h);
				x_dd3_l =
				    _mm_srai_epi32(x_dd3_l, BC_SUM1_CONST);
				x_dd3_h =
				    _mm_srai_epi32(x_dd3_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
				k = j + 1;
				SAT_STORE(dp[CHAN1 * k], sc[2]);
				SAT_STORE(dp[CHAN1 * k + 1], sc[3]);
				SAT_STORE(dp[CHAN1 * k + 2], sd[0]);
				k = j + 2;
				SAT_STORE(dp[CHAN1 * k], se[0]);
				SAT_STORE(dp[CHAN1 * k + 1], se[1]);
				SAT_STORE(dp[CHAN1 * k + 2], se[2]);
				k = j + 3;
				SAT_STORE(dp[CHAN1 * k], sf[2]);
				SAT_STORE(dp[CHAN1 * k + 1], sf[3]);
				SAT_STORE(dp[CHAN1 * k + 2], sg[0]);
			}

			for (; j < width; j++) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

				x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
				x_dd1_l =
				    _mm_srai_epi32(x_dd1_l, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_si128((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_si128((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_si128((void *)(sp - CHAN1 + 2 * slb));

		x_si0 = _mm_srai_epi16(x_sd0, 15);
		x_si1 = _mm_srai_epi16(x_sd1, 15);
		x_si2 = _mm_srai_epi16(x_sd2, 15);
		x_si3 = _mm_srai_epi16(x_sd3, 15);

		x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
		x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
		x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
		x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
		x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
		x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
		x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
		x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

		x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

		x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_l, 32), x_const);
		x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

		x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_h, 32), x_const);
		x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

		x_dd0_l = _mm_sub_epi32(x_st4_l, x_st1_l);
		x_dd0_h = _mm_sub_epi32(x_st4_h, x_st1_h);
		_mm_storeu_si128((void *)(sa), x_dd0_l);
		_mm_storel_epi64((void *)(sa + 4), x_dd0_h);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			MLIB_BIC_LDSI(CHAN1 + 8, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd2_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd2_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			MLIB_BIC_LDSI(CHAN1 + 16, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd3_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd3_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sc[0]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sc[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sc[2], sd[1]);
			res1 = BC_SUM2(sb[1], sc[0], sc[3], sd[2]);
			res2 = BC_SUM2(sb[2], sc[1], sd[0], sd[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 2;
			res0 = BC_SUM2(sc[2], sd[1], se[0], se[3]);
			res1 = BC_SUM2(sc[3], sd[2], se[1], sf[0]);
			res2 = BC_SUM2(sd[0], sd[3], se[2], sf[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 3;
			res0 = BC_SUM2(se[0], se[3], sf[2], sg[1]);
			res1 = BC_SUM2(se[1], sf[0], sf[3], sg[2]);
			res2 = BC_SUM2(se[2], sf[1], sg[0], sg[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			sa[0] = sf[2];
			sa[1] = sf[3];
			sa[2] = sg[0];
			sa[3] = sg[1];
			sa[4] = sg[2];
			sa[5] = sg[3];
		}

		if (j < width - 1) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			MLIB_BIC_LDSI(CHAN1 + 8, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);
			x_dd2_l = _mm_sub_epi32(x_st4_l, x_st1_l);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sc[0]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sc[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sc[2], sd[1]);
			res1 = BC_SUM2(sb[1], sc[0], sc[3], sd[2]);
			res2 = BC_SUM2(sb[2], sc[1], sd[0], sd[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			sa[0] = sc[2];
			sa[1] = sc[3];
			sa[2] = sd[0];
			sa[3] = sd[1];
			sa[4] = sd[2];
			sa[5] = sd[3];
			j += 2;
		}

		if (j < width) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_l = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st3_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st3_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpacklo_epi64(x_st3_l, x_st3_h);

			x_dd1_l = _mm_sub_epi32(x_st4_l, x_st1_l);
			x_dd1_h = _mm_sub_epi32(x_st4_h, x_st1_h);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sc[0]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sc[1]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);
		}
	}
}

#elif IMG_TYPE == 1	/* IMG_TYPE == 6 */
{
	DEF_VARS();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
				a2 = sp[-CHAN1];
				a3 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = a2;
					a1 = a3;
					a2 = sp[BCHAN1];
					a3 = sp[BCHAN2];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

	const __m128i x_const = _mm_set1_epi16(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sl0, x_sl1, x_sl2, x_sl3;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0, x_st1, x_st2, x_st3, x_st4;
	__m128i x_st5, x_st6, x_st7, x_st8, x_st9;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;

	mlib_s16 res0, res1, res2;
	mlib_s16 sa[8];
	mlib_s16 *sb = (mlib_s16 *)&x_dd0;
	mlib_s16 *sc = (mlib_s16 *)&x_dd1;
	mlib_s16 *sd = (mlib_s16 *)&x_dd2;
	mlib_s16 *se = (mlib_s16 *)&x_dd3;
	mlib_s16 *sf = (mlib_s16 *)&x_dd4;
	mlib_s16 *sg = (mlib_s16 *)&x_dd5;

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 2; j += 3) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
				x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

				x_sl0 = _mm_add_epi16(x_st2, x_st4);
				x_sl1 = _mm_add_epi16(x_st3, x_st5);
				x_sl2 = _mm_add_epi16(x_st0, x_st6);
				x_sl3 = _mm_add_epi16(x_st1, x_st7);

				x_st0 = _mm_mullo_epi16(x_sl0, x_const);
				x_st1 = _mm_mullo_epi16(x_sl1, x_const);
				x_dd0 = _mm_sub_epi16(x_st0, x_sl2);
				x_dd1 = _mm_sub_epi16(x_st1, x_sl3);
				x_dd0 = _mm_srai_epi16(x_dd0, BC_SUM1_CONST);
				x_dd1 = _mm_srai_epi16(x_dd1, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);

				k = j + 1;
				SAT_STORE(dp[CHAN1 * k], sb[6]);
				SAT_STORE(dp[CHAN1 * k + 1], sb[7]);
				SAT_STORE(dp[CHAN1 * k + 2], sc[0]);
				k = j + 2;
				SAT_STORE(dp[CHAN1 * k], sc[4]);
				SAT_STORE(dp[CHAN1 * k + 1], sc[5]);
				SAT_STORE(dp[CHAN1 * k + 2], sc[6]);
			}

			for (; j < width; j++) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

				x_sl0 = _mm_add_epi16(x_st2, x_st4);
				x_sl2 = _mm_add_epi16(x_st0, x_st6);

				x_st0 = _mm_mullo_epi16(x_sl0, x_const);
				x_dd0 = _mm_sub_epi16(x_st0, x_sl2);
				x_dd0 = _mm_srai_epi16(x_dd0, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadl_epi64((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadl_epi64((void *)(sp - CHAN1));
		x_sd2 = _mm_loadl_epi64((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadl_epi64((void *)(sp - CHAN1 + 2 * slb));

		x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
		x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
		x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
		x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

		x_sl0 = _mm_add_epi16(x_st2, x_st4);
		x_sl2 = _mm_add_epi16(x_st0, x_st6);
		x_st0 = _mm_mullo_epi16(x_sl0, x_const);
		x_dd0 = _mm_sub_epi16(x_st0, x_sl2);
		_mm_storeu_si128((void *)sa, x_dd0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 7; j += 8) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_dd0 = _mm_sub_epi16(x_st0, x_sl2);
			x_dd1 = _mm_sub_epi16(x_st1, x_sl3);

			MLIB_BIC_LDSI(CHAN1 + 16, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_dd2 = _mm_sub_epi16(x_st0, x_sl2);
			x_dd3 = _mm_sub_epi16(x_st1, x_sl3);

			MLIB_BIC_LDSI(CHAN1 + 32, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_dd4 = _mm_sub_epi16(x_st0, x_sl2);
			x_dd5 = _mm_sub_epi16(x_st1, x_sl3);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sb[4]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sb[5]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sb[6], sc[1]);
			res1 = BC_SUM2(sb[1], sb[4], sb[7], sc[2]);
			res2 = BC_SUM2(sb[2], sb[5], sc[0], sc[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 2;
			res0 = BC_SUM2(sb[6], sc[1], sc[4], sc[7]);
			res1 = BC_SUM2(sb[7], sc[2], sc[5], sd[0]);
			res2 = BC_SUM2(sc[0], sc[3], sc[6], sd[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 3;
			res0 = BC_SUM2(sc[4], sc[7], sd[2], sd[5]);
			res1 = BC_SUM2(sc[5], sd[0], sd[3], sd[6]);
			res2 = BC_SUM2(sc[6], sd[1], sd[4], sd[7]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 4;
			res0 = BC_SUM2(sd[2], sd[5], se[0], se[3]);
			res1 = BC_SUM2(sd[3], sd[6], se[1], se[4]);
			res2 = BC_SUM2(sd[4], sd[7], se[2], se[5]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 5;
			res0 = BC_SUM2(se[0], se[3], se[6], sf[1]);
			res1 = BC_SUM2(se[1], se[4], se[7], sf[2]);
			res2 = BC_SUM2(se[2], se[5], sf[0], sf[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 6;
			res0 = BC_SUM2(se[6], sf[1], sf[4], sf[7]);
			res1 = BC_SUM2(se[7], sf[2], sf[5], sg[0]);
			res2 = BC_SUM2(sf[0], sf[3], sf[6], sg[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 7;
			res0 = BC_SUM2(sf[4], sf[7], sg[2], sg[5]);
			res1 = BC_SUM2(sf[5], sg[0], sg[3], sg[6]);
			res2 = BC_SUM2(sf[6], sg[1], sg[4], sg[7]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);
			sa[0] = sg[2];
			sa[1] = sg[3];
			sa[2] = sg[4];
			sa[3] = sg[5];
			sa[4] = sg[6];
			sa[5] = sg[7];
		}

		if (j < width - 3) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_dd0 = _mm_sub_epi16(x_st0, x_sl2);
			x_dd1 = _mm_sub_epi16(x_st1, x_sl3);

			x_sd0 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 - slb + 16));
			x_sd1 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + 16));
			x_sd2 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + slb + 16));
			x_sd3 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + 2 * slb + 16));

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_dd2 = _mm_sub_epi16(x_st0, x_sl2);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sb[4]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sb[5]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sb[6], sc[1]);
			res1 = BC_SUM2(sb[1], sb[4], sb[7], sc[2]);
			res2 = BC_SUM2(sb[2], sb[5], sc[0], sc[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 2;
			res0 = BC_SUM2(sb[6], sc[1], sc[4], sc[7]);
			res1 = BC_SUM2(sb[7], sc[2], sc[5], sd[0]);
			res2 = BC_SUM2(sc[0], sc[3], sc[6], sd[1]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			k = j + 3;
			res0 = BC_SUM2(sc[4], sc[7], sd[2], sd[5]);
			res1 = BC_SUM2(sc[5], sd[0], sd[3], sd[6]);
			res2 = BC_SUM2(sc[6], sd[1], sd[4], sd[7]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);

			j += 4;
			sa[0] = sd[2];
			sa[1] = sd[3];
			sa[2] = sd[4];
			sa[3] = sd[5];
			sa[4] = sd[6];
			sa[5] = sd[7];
		}

		if (j < width - 1) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st1 = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st3 = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st5 = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_st7 = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl1 = _mm_add_epi16(x_st3, x_st5);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);
			x_sl3 = _mm_add_epi16(x_st1, x_st7);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_st1 = _mm_mullo_epi16(x_sl1, x_const);
			x_dd0 = _mm_sub_epi16(x_st0, x_sl2);
			x_dd1 = _mm_sub_epi16(x_st1, x_sl3);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sb[4]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sb[5]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);

			k = j + 1;
			res0 = BC_SUM2(sb[0], sb[3], sb[6], sc[1]);
			res1 = BC_SUM2(sb[1], sb[4], sb[7], sc[2]);
			res2 = BC_SUM2(sb[2], sb[5], sc[0], sc[3]);
			SAT_STORE(dp[CHAN1 * k], res0);
			SAT_STORE(dp[CHAN1 * k + 1], res1);
			SAT_STORE(dp[CHAN1 * k + 2], res2);
			j += 2;
			sa[0] = sb[6];
			sa[1] = sb[7];
			sa[2] = sc[0];
			sa[3] = sc[1];
			sa[4] = sc[2];
			sa[5] = sc[3];
		}

		if (j < width) {
			x_sd0 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 - slb));
			x_sd1 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1));
			x_sd2 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + slb));
			x_sd3 = _mm_loadl_epi64(
			    (void *)(sp + BCHAN1 + 2 * slb));

			x_st0 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_st2 = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_st4 = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_st6 = _mm_unpacklo_epi8(x_sd3, x_zero);

			x_sl0 = _mm_add_epi16(x_st2, x_st4);
			x_sl2 = _mm_add_epi16(x_st0, x_st6);

			x_st0 = _mm_mullo_epi16(x_sl0, x_const);
			x_dd0 = _mm_sub_epi16(x_st0, x_sl2);

			res0 = BC_SUM2(sa[0], sa[3], sb[0], sb[3]);
			res1 = BC_SUM2(sa[1], sa[4], sb[1], sb[4]);
			res2 = BC_SUM2(sa[2], sa[5], sb[2], sb[5]);
			SAT_STORE(dp[CHAN1 * j], res0);
			SAT_STORE(dp[CHAN1 * j + 1], res1);
			SAT_STORE(dp[CHAN1 * j + 2], res2);
		}
	}
}

#endif

/* *********************************************************** */

#undef  CHAN1
#define	CHAN1	4

/* *********************************************************** */

#if defined(USED_FILTER_BICUBIC)

void ZOOM_FUNC(
    4_Bicubic) (
    BC_PARAMETERS(STYPE))

#else /* defined(USED_FILTER_BICUBIC) */

void ZOOM_FUNC(
    4_Bicubic2) (
    BC_PARAMETERS(STYPE))

#endif		   /* defined(USED_FILTER_BICUBIC) */

#if IMG_TYPE == 6	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128d x_sd4_l, x_sd5_l, x_sd6_l, x_sd7_l;
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128d x_sd4_h, x_sd5_h, x_sd6_h, x_sd7_h;
	__m128d x_st0_l, x_st1_l, x_st2_l, x_st4_l, x_st5_l, x_st6_l;
	__m128d x_st0_h, x_st1_h, x_st2_h, x_st4_h, x_st5_h, x_st6_h;
	__m128d x_dd0_l, x_dd1_l, x_dd2_l, x_dd3_l, x_dd4_l;
	__m128d x_dd0_h, x_dd1_h, x_dd2_h, x_dd3_h, x_dd4_h;

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	mlib_d64 bc_sum2_div = 1.0 / BC_SUM2_DIV;
	const __m128d x_bc_sum2_div = _mm_set1_pd(bc_sum2_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	mlib_d64 *sa = (mlib_d64 *)&x_dd4_l;
	mlib_d64 *sb = (mlib_d64 *)&x_dd4_h;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;

			x_dd0_l = _mm_loadu_pd((void *)(sp - CHAN1));
			x_dd0_h = _mm_loadu_pd((void *)(sp - CHAN1 + 2));
			x_dd1_l = _mm_loadu_pd((void *)(sp));
			x_dd1_h = _mm_loadu_pd((void *)(sp + 2));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				x_dd2_l = _mm_loadu_pd(sp + BCHAN1);
				x_dd2_h = _mm_loadu_pd(sp + BCHAN1 + 2);
				x_dd3_l = _mm_loadu_pd(sp + BCHAN2);
				x_dd3_h = _mm_loadu_pd(sp + BCHAN2 + 2);

				x_sd0_l = _mm_add_pd(x_dd1_l, x_dd2_l);
				x_sd0_h = _mm_add_pd(x_dd1_h, x_dd2_h);
				x_sd1_l = _mm_add_pd(x_dd0_l, x_dd3_l);
				x_sd1_h = _mm_add_pd(x_dd0_h, x_dd3_h);
				x_sd2_l = _mm_mul_pd(x_sd0_l, x_const);
				x_sd2_h = _mm_mul_pd(x_sd0_h, x_const);
				x_sd3_l = _mm_sub_pd(x_sd2_l, x_sd1_l);
				x_sd3_h = _mm_sub_pd(x_sd2_h, x_sd1_h);
				x_dd4_l = _mm_mul_pd(x_sd3_l, x_bc_sum1_div);
				x_dd4_h = _mm_mul_pd(x_sd3_h, x_bc_sum1_div);

				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 3], sb[1]);
				x_dd0_l = x_dd2_l;
				x_dd0_h = x_dd2_h;
				x_dd1_l = x_dd3_l;
				x_dd1_h = x_dd3_h;
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDPD(
				    0, j, x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l)
				MLIB_BIC_LDPD(
				    2, j, x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h)

				x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
				x_st2_l = _mm_mul_pd(x_st0_l, x_const);
				x_st2_h = _mm_mul_pd(x_st0_h, x_const);
				x_dd2_l = _mm_sub_pd(x_st2_l, x_st1_l);
				x_dd2_h = _mm_sub_pd(x_st2_h, x_st1_h);
				x_dd3_l = _mm_mul_pd(x_dd2_l, x_bc_sum1_div);
				x_dd3_h = _mm_mul_pd(x_dd2_h, x_bc_sum1_div);
				_mm_storeu_pd((dp + CHAN1 * j), x_dd3_l);
				_mm_storeu_pd((dp + CHAN1 * j + 2), x_dd3_h);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0_l = _mm_loadu_pd((void *)(sp - CHAN1 - slb));
		x_sd1_l = _mm_loadu_pd((void *)(sp - CHAN1));
		x_sd2_l = _mm_loadu_pd((void *)(sp - CHAN1 + slb));
		x_sd3_l = _mm_loadu_pd((void *)(sp - CHAN1 + 2 * slb));
		x_sd0_h = _mm_loadu_pd((void *)(sp - CHAN1 - slb + 2));
		x_sd1_h = _mm_loadu_pd((void *)(sp - CHAN1 + 2));
		x_sd2_h = _mm_loadu_pd((void *)(sp - CHAN1 + slb + 2));
		x_sd3_h = _mm_loadu_pd((void *)(sp - CHAN1 + 2 * slb + 2));

		x_sd4_l = _mm_loadu_pd((void *)(sp - slb));
		x_sd5_l = _mm_loadu_pd((void *)(sp));
		x_sd6_l = _mm_loadu_pd((void *)(sp + slb));
		x_sd7_l = _mm_loadu_pd((void *)(sp + 2 * slb));
		x_sd4_h = _mm_loadu_pd((void *)(sp - slb + 2));
		x_sd5_h = _mm_loadu_pd((void *)(sp + 2));
		x_sd6_h = _mm_loadu_pd((void *)(sp + slb + 2));
		x_sd7_h = _mm_loadu_pd((void *)(sp + 2 * slb + 2));

		x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
		x_st2_l = _mm_mul_pd(x_st0_l, x_const);
		x_st2_h = _mm_mul_pd(x_st0_h, x_const);
		x_dd0_l = _mm_sub_pd(x_st2_l, x_st1_l);
		x_dd0_h = _mm_sub_pd(x_st2_h, x_st1_h);

		x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
		x_st4_h = _mm_add_pd(x_sd5_h, x_sd6_h);
		x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
		x_st5_h = _mm_add_pd(x_sd4_h, x_sd7_h);
		x_st6_l = _mm_mul_pd(x_st4_l, x_const);
		x_st6_h = _mm_mul_pd(x_st4_h, x_const);
		x_dd1_l = _mm_sub_pd(x_st6_l, x_st5_l);
		x_dd1_h = _mm_sub_pd(x_st6_h, x_st5_h);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			MLIB_BIC_LDPD(
			    CHAN1, j, x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l)
			MLIB_BIC_LDPD(
			    CHAN1 + 2, j, x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h)

			MLIB_BIC_LDPD(
			    CHAN2, j, x_sd4_l, x_sd5_l, x_sd6_l, x_sd7_l)
			MLIB_BIC_LDPD(
			    CHAN2 + 2, j, x_sd4_h, x_sd5_h, x_sd6_h, x_sd7_h)

			x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
			x_st2_l = _mm_mul_pd(x_st0_l, x_const);
			x_st2_h = _mm_mul_pd(x_st0_h, x_const);
			x_dd2_l = _mm_sub_pd(x_st2_l, x_st1_l);
			x_dd2_h = _mm_sub_pd(x_st2_h, x_st1_h);

			x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
			x_st4_h = _mm_add_pd(x_sd5_h, x_sd6_h);
			x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
			x_st5_h = _mm_add_pd(x_sd4_h, x_sd7_h);
			x_st6_l = _mm_mul_pd(x_st4_l, x_const);
			x_st6_h = _mm_mul_pd(x_st4_h, x_const);
			x_dd3_l = _mm_sub_pd(x_st6_l, x_st5_l);
			x_dd3_h = _mm_sub_pd(x_st6_h, x_st5_h);

			x_sd0_l = _mm_add_pd(x_dd1_l, x_dd2_l);
			x_sd0_h = _mm_add_pd(x_dd1_h, x_dd2_h);
			x_sd1_l = _mm_add_pd(x_dd0_l, x_dd3_l);
			x_sd1_h = _mm_add_pd(x_dd0_h, x_dd3_h);
			x_sd2_l = _mm_mul_pd(x_sd0_l, x_const);
			x_sd2_h = _mm_mul_pd(x_sd0_h, x_const);
			x_sd3_l = _mm_sub_pd(x_sd2_l, x_sd1_l);
			x_sd3_h = _mm_sub_pd(x_sd2_h, x_sd1_h);
			x_dd4_l = _mm_mul_pd(x_sd3_l, x_bc_sum2_div);
			x_dd4_h = _mm_mul_pd(x_sd3_h, x_bc_sum2_div);

			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
			SAT_STORE(dp[CHAN1 * j + 3], sb[1]);
			x_dd0_l = x_dd2_l;
			x_dd0_h = x_dd2_h;
			x_dd1_l = x_dd3_l;
			x_dd1_h = x_dd3_h;
		}
	}
}

#elif IMG_TYPE == 5	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	__m128 x_sd0, x_sd1, x_sd2, x_sd3;
	__m128 x_st0, x_st1, x_st2, x_st3;
	__m128 x_st4, x_st5, x_st6, x_st7;
	__m128 x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	mlib_f32 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128 x_bc_sum1_div = _mm_set1_ps(bc_sum1_div);
	mlib_f32 bc_sum2_div = 1.0 / BC_SUM2_DIV;
	const __m128 x_bc_sum2_div = _mm_set1_ps(bc_sum2_div);
	const __m128 x_const = _mm_set1_ps(BC_SUM_CONST);
	const __m128 x_zero = _mm_setzero_ps();

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;

			x_dd0 = _mm_loadu_ps((void *)(sp - CHAN1));
			x_dd1 = _mm_loadu_ps((void *)(sp));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_dd2 = _mm_loadu_ps(sp + BCHAN1);
				x_dd3 = _mm_loadu_ps(sp + BCHAN2);
				x_dd4 = _mm_loadu_ps(sp + BCHAN1N);
				x_dd5 = _mm_loadu_ps(sp + BCHAN2N);

				x_st4 = _mm_add_ps(x_dd1, x_dd2);
				x_st5 = _mm_add_ps(x_dd0, x_dd3);
				x_st6 = _mm_mul_ps(x_st4, x_const);
				x_st7 = _mm_sub_ps(x_st6, x_st5);
				x_st0 = _mm_mul_ps(x_st7, x_bc_sum1_div);

				x_st4 = _mm_add_ps(x_dd3, x_dd4);
				x_st5 = _mm_add_ps(x_dd2, x_dd5);
				x_st6 = _mm_mul_ps(x_st4, x_const);
				x_st7 = _mm_sub_ps(x_st6, x_st5);
				x_st1 = _mm_mul_ps(x_st7, x_bc_sum1_div);

				_mm_storeu_ps((dp + CHAN1 * j), x_st0);
				_mm_storeu_ps((dp + CHAN1 * (j + 1)), x_st1);
				x_dd0 = x_dd4;
				x_dd1 = x_dd5;
			}

			if (j < width) {
				x_dd2 = _mm_loadu_ps(sp + BCHAN1);
				x_dd3 = _mm_loadu_ps(sp + BCHAN2);

				x_st4 = _mm_add_ps(x_dd1, x_dd2);
				x_st5 = _mm_add_ps(x_dd0, x_dd3);
				x_st6 = _mm_mul_ps(x_st4, x_const);
				x_st7 = _mm_sub_ps(x_st6, x_st5);
				x_st0 = _mm_mul_ps(x_st7, x_bc_sum1_div);

				_mm_storeu_ps((void *)(dp + CHAN1 * j), x_st0);
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDPS(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_st0 = _mm_add_ps(x_sd1, x_sd2);
				x_st1 = _mm_add_ps(x_sd0, x_sd3);
				x_st2 = _mm_mul_ps(x_st0, x_const);
				x_dd2 = _mm_sub_ps(x_st2, x_st1);
				x_dd3 = _mm_mul_ps(x_dd2, x_bc_sum1_div);
				_mm_storeu_ps((dp + CHAN1 * j), x_dd3);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_ps((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_ps((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_ps((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_ps((void *)(sp - CHAN1 + 2 * slb));

		x_st0 = _mm_add_ps(x_sd1, x_sd2);
		x_st1 = _mm_add_ps(x_sd0, x_sd3);
		x_st2 = _mm_mul_ps(x_st0, x_const);
		x_dd0 = _mm_sub_ps(x_st2, x_st1);

		x_sd0 = _mm_loadu_ps((void *)(sp - slb));
		x_sd1 = _mm_loadu_ps((void *)(sp));
		x_sd2 = _mm_loadu_ps((void *)(sp + slb));
		x_sd3 = _mm_loadu_ps((void *)(sp + 2 * slb));

		x_st0 = _mm_add_ps(x_sd1, x_sd2);
		x_st1 = _mm_add_ps(x_sd0, x_sd3);
		x_st2 = _mm_mul_ps(x_st0, x_const);
		x_dd1 = _mm_sub_ps(x_st2, x_st1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd2 = _mm_sub_ps(x_st2, x_st1);

			MLIB_BIC_LDPS(CHAN2, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd3 = _mm_sub_ps(x_st2, x_st1);

			k = j + 1;
			MLIB_BIC_LDPS(CHAN1, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd4 = _mm_sub_ps(x_st2, x_st1);

			MLIB_BIC_LDPS(CHAN2, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd5 = _mm_sub_ps(x_st2, x_st1);

			x_st4 = _mm_add_ps(x_dd1, x_dd2);
			x_st5 = _mm_add_ps(x_dd0, x_dd3);
			x_st6 = _mm_mul_ps(x_st4, x_const);
			x_st7 = _mm_sub_ps(x_st6, x_st5);
			x_st0 = _mm_mul_ps(x_st7, x_bc_sum2_div);

			x_st4 = _mm_add_ps(x_dd3, x_dd4);
			x_st5 = _mm_add_ps(x_dd2, x_dd5);
			x_st6 = _mm_mul_ps(x_st4, x_const);
			x_st7 = _mm_sub_ps(x_st6, x_st5);
			x_st1 = _mm_mul_ps(x_st7, x_bc_sum2_div);

			_mm_storeu_ps((void *)(dp + CHAN1 * j), x_st0);
			_mm_storeu_ps((void *)(dp + CHAN1 * (j + 1)), x_st1);
			x_dd0 = x_dd4;
			x_dd1 = x_dd5;
		}

		if (j < width) {
			MLIB_BIC_LDPS(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd2 = _mm_sub_ps(x_st2, x_st1);

			MLIB_BIC_LDPS(CHAN2, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_st0 = _mm_add_ps(x_sd1, x_sd2);
			x_st1 = _mm_add_ps(x_sd0, x_sd3);
			x_st2 = _mm_mul_ps(x_st0, x_const);
			x_dd3 = _mm_sub_ps(x_st2, x_st1);

			x_st4 = _mm_add_ps(x_dd1, x_dd2);
			x_st5 = _mm_add_ps(x_dd0, x_dd3);
			x_st6 = _mm_mul_ps(x_st4, x_const);
			x_st7 = _mm_sub_ps(x_st6, x_st5);
			x_st0 = _mm_mul_ps(x_st7, x_bc_sum2_div);

			_mm_storeu_ps((void *)(dp + CHAN1 * j), x_st0);
			x_dd0 = x_dd2;
			x_dd1 = x_dd3;
		}
	}
}

#elif IMG_TYPE == 4	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128d x_sd4_l, x_sd5_l, x_sd6_l, x_sd7_l;
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128d x_sd4_h, x_sd5_h, x_sd6_h, x_sd7_h;
	__m128d x_st0_l, x_st1_l, x_st2_l, x_st4_l, x_st5_l, x_st6_l;
	__m128d x_st0_h, x_st1_h, x_st2_h, x_st4_h, x_st5_h, x_st6_h;
	__m128d x_dd0_l, x_dd1_l, x_dd2_l, x_dd3_l, x_dd4_l;
	__m128d x_dd0_h, x_dd1_h, x_dd2_h, x_dd3_h, x_dd4_h;

	mlib_d64 bc_sum1_div = 1.0 / BC_SUM1_DIV;
	const __m128d x_bc_sum1_div = _mm_set1_pd(bc_sum1_div);
	mlib_d64 bc_sum2_div = 1.0 / BC_SUM2_DIV;
	const __m128d x_bc_sum2_div = _mm_set1_pd(bc_sum2_div);
	const __m128d x_const = _mm_set1_pd(BC_SUM_CONST);
	mlib_d64 *sa = (mlib_d64 *)&x_dd4_l;
	mlib_d64 *sb = (mlib_d64 *)&x_dd4_h;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;

			x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1));
			x_dd0_l = _mm_cvtepi32_pd(x_sd0);
			x_dd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));

			x_sd4 = _mm_loadu_si128((void *)(sp));
			x_dd1_l = _mm_cvtepi32_pd(x_sd4);
			x_dd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd4, 8));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				x_sd0 = _mm_loadu_si128((void *)(sp + BCHAN1));
				x_sd1 = _mm_loadu_si128((void *)(sp + BCHAN2));

				x_dd2_l = _mm_cvtepi32_pd(x_sd0);
				x_dd2_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
				x_dd3_l = _mm_cvtepi32_pd(x_sd1);
				x_dd3_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));

				x_sd0_l = _mm_add_pd(x_dd1_l, x_dd2_l);
				x_sd0_h = _mm_add_pd(x_dd1_h, x_dd2_h);
				x_sd1_l = _mm_add_pd(x_dd0_l, x_dd3_l);
				x_sd1_h = _mm_add_pd(x_dd0_h, x_dd3_h);
				x_sd2_l = _mm_mul_pd(x_sd0_l, x_const);
				x_sd2_h = _mm_mul_pd(x_sd0_h, x_const);
				x_sd3_l = _mm_sub_pd(x_sd2_l, x_sd1_l);
				x_sd3_h = _mm_sub_pd(x_sd2_h, x_sd1_h);
				x_dd4_l = _mm_mul_pd(x_sd3_l, x_bc_sum1_div);
				x_dd4_h = _mm_mul_pd(x_sd3_h, x_bc_sum1_div);

				_mm_store_pd(sa, x_dd4_l);
				_mm_store_pd(sb, x_dd4_h);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 3], sb[1]);
				x_dd0_l = x_dd2_l;
				x_dd0_h = x_dd2_h;
				x_dd1_l = x_dd3_l;
				x_dd1_h = x_dd3_h;
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l =
					_mm_cvtepi32_pd(x_sd0);
				x_sd0_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
				x_sd1_l =
				    _mm_cvtepi32_pd(x_sd1);
				x_sd1_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
				x_sd2_l = _mm_cvtepi32_pd(x_sd2);
				x_sd2_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
				x_sd3_l = _mm_cvtepi32_pd(x_sd3);
				x_sd3_h =
				    _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

				x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
				x_st2_l = _mm_mul_pd(x_st0_l, x_const);
				x_st2_h = _mm_mul_pd(x_st0_h, x_const);
				x_dd2_l = _mm_sub_pd(x_st2_l, x_st1_l);
				x_dd2_h = _mm_sub_pd(x_st2_h, x_st1_h);
				x_dd4_l = _mm_mul_pd(x_dd2_l, x_bc_sum1_div);
				x_dd4_h = _mm_mul_pd(x_dd2_h, x_bc_sum1_div);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 3], sb[1]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_si128((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_si128((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_si128((void *)(sp - CHAN1 + 2 * slb));

		x_sd0_l = _mm_cvtepi32_pd(x_sd0);
		x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
		x_sd1_l = _mm_cvtepi32_pd(x_sd1);
		x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
		x_sd2_l = _mm_cvtepi32_pd(x_sd2);
		x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
		x_sd3_l = _mm_cvtepi32_pd(x_sd3);
		x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

		x_sd4 = _mm_loadu_si128((void *)(sp - slb));
		x_sd5 = _mm_loadu_si128((void *)(sp));
		x_sd6 = _mm_loadu_si128((void *)(sp + slb));
		x_sd7 = _mm_loadu_si128((void *)(sp + 2 * slb));

		x_sd4_l = _mm_cvtepi32_pd(x_sd4);
		x_sd4_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd4, 8));
		x_sd5_l = _mm_cvtepi32_pd(x_sd5);
		x_sd5_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd5, 8));
		x_sd6_l = _mm_cvtepi32_pd(x_sd6);
		x_sd6_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd6, 8));
		x_sd7_l = _mm_cvtepi32_pd(x_sd7);
		x_sd7_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd7, 8));

		x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
		x_st2_l = _mm_mul_pd(x_st0_l, x_const);
		x_st2_h = _mm_mul_pd(x_st0_h, x_const);
		x_dd0_l = _mm_sub_pd(x_st2_l, x_st1_l);
		x_dd0_h = _mm_sub_pd(x_st2_h, x_st1_h);

		x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
		x_st4_h = _mm_add_pd(x_sd5_h, x_sd6_h);
		x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
		x_st5_h = _mm_add_pd(x_sd4_h, x_sd7_h);
		x_st6_l = _mm_mul_pd(x_st4_l, x_const);
		x_st6_h = _mm_mul_pd(x_st4_h, x_const);
		x_dd1_l = _mm_sub_pd(x_st6_l, x_st5_l);
		x_dd1_h = _mm_sub_pd(x_st6_h, x_st5_h);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_cvtepi32_pd(x_sd0);
			x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));
			x_sd1_l = _mm_cvtepi32_pd(x_sd1);
			x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));
			x_sd2_l = _mm_cvtepi32_pd(x_sd2);
			x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
			x_sd3_l = _mm_cvtepi32_pd(x_sd3);
			x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));

			MLIB_BIC_LDSI(CHAN2, j, x_sd4, x_sd5, x_sd6, x_sd7)

			x_sd4_l = _mm_cvtepi32_pd(x_sd4);
			x_sd4_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd4, 8));
			x_sd5_l = _mm_cvtepi32_pd(x_sd5);
			x_sd5_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd5, 8));
			x_sd6_l = _mm_cvtepi32_pd(x_sd6);
			x_sd6_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd6, 8));
			x_sd7_l = _mm_cvtepi32_pd(x_sd7);
			x_sd7_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd7, 8));

			x_st0_l = _mm_add_pd(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_pd(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_pd(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_pd(x_sd0_h, x_sd3_h);
			x_st2_l = _mm_mul_pd(x_st0_l, x_const);
			x_st2_h = _mm_mul_pd(x_st0_h, x_const);
			x_dd2_l = _mm_sub_pd(x_st2_l, x_st1_l);
			x_dd2_h = _mm_sub_pd(x_st2_h, x_st1_h);

			x_st4_l = _mm_add_pd(x_sd5_l, x_sd6_l);
			x_st4_h = _mm_add_pd(x_sd5_h, x_sd6_h);
			x_st5_l = _mm_add_pd(x_sd4_l, x_sd7_l);
			x_st5_h = _mm_add_pd(x_sd4_h, x_sd7_h);
			x_st6_l = _mm_mul_pd(x_st4_l, x_const);
			x_st6_h = _mm_mul_pd(x_st4_h, x_const);
			x_dd3_l = _mm_sub_pd(x_st6_l, x_st5_l);
			x_dd3_h = _mm_sub_pd(x_st6_h, x_st5_h);

			x_sd0_l = _mm_add_pd(x_dd1_l, x_dd2_l);
			x_sd0_h = _mm_add_pd(x_dd1_h, x_dd2_h);
			x_sd1_l = _mm_add_pd(x_dd0_l, x_dd3_l);
			x_sd1_h = _mm_add_pd(x_dd0_h, x_dd3_h);
			x_sd2_l = _mm_mul_pd(x_sd0_l, x_const);
			x_sd2_h = _mm_mul_pd(x_sd0_h, x_const);
			x_sd3_l = _mm_sub_pd(x_sd2_l, x_sd1_l);
			x_sd3_h = _mm_sub_pd(x_sd2_h, x_sd1_h);
			x_dd4_l = _mm_mul_pd(x_sd3_l, x_bc_sum2_div);
			x_dd4_h = _mm_mul_pd(x_sd3_h, x_bc_sum2_div);

			_mm_store_pd(sa, x_dd4_l);
			_mm_store_pd(sb, x_dd4_h);
			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sb[0]);
			SAT_STORE(dp[CHAN1 * j + 3], sb[1]);
			x_dd0_l = x_dd2_l;
			x_dd0_h = x_dd2_h;
			x_dd1_l = x_dd3_l;
			x_dd1_h = x_dd3_h;
		}
	}
}

#elif IMG_TYPE == 3	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0_l, x_st1_l, x_st2_l, x_st4_l, x_st5_l, x_st6_l, x_st7_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st4_h, x_st5_h, x_st6_h, x_st7_h;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6;

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 *sa = (mlib_s32 *)&x_dd6;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;

			x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1));
			x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_dd1 = _mm_unpackhi_epi16(x_sd0, x_zero);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_sd2 =
				    _mm_loadu_si128((void *)(sp + BCHAN1));
				x_sd3 =
				    _mm_loadu_si128((void *)(sp + BCHAN1N));
				x_dd2 = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_dd3 = _mm_unpackhi_epi16(x_sd2, x_zero);
				x_dd4 = _mm_unpacklo_epi16(x_sd3, x_zero);
				x_dd5 = _mm_unpackhi_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
				x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

				x_st0_l = _mm_add_epi32(x_dd3, x_dd4);
				x_st0_h = _mm_add_epi32(x_dd2, x_dd5);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * (j + 1)], sa[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 3], sa[3]);

				x_dd0 = x_dd4;
				x_dd1 = x_dd5;
			}

			if (j < width) {
				x_sd2 =
				    _mm_loadu_si128((void *)(sp + BCHAN1));
				x_dd2 = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_dd3 = _mm_unpackhi_epi16(x_sd2, x_zero);

				x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
				x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

				x_dd0 = x_dd2;
				x_dd1 = x_dd3;
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

				x_dd2 = _mm_sub_epi32(x_st5_l, x_st1_l);
				x_dd6 = _mm_srai_epi32(x_dd2, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_si128((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_si128((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_si128((void *)(sp - CHAN1 + 2 * slb));

		x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
		x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
		x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
		x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
		x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
		x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
		x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

		x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

		x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_l, 32), x_const);
		x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

		x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_h, 32), x_const);
		x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

		x_dd0 = _mm_sub_epi32(x_st5_l, x_st1_l);
		x_dd1 = _mm_sub_epi32(x_st5_h, x_st1_h);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_dd2 = _mm_sub_epi32(x_st5_l, x_st1_l);
			x_dd3 = _mm_sub_epi32(x_st5_h, x_st1_h);

			k = j + 1;
			MLIB_BIC_LDSI(CHAN1, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_dd4 = _mm_sub_epi32(x_st5_l, x_st1_l);
			x_dd5 = _mm_sub_epi32(x_st5_h, x_st1_h);

			x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
			x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

			x_st0_l = _mm_add_epi32(x_dd3, x_dd4);
			x_st0_h = _mm_add_epi32(x_dd2, x_dd5);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * (j + 1)], sa[0]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 3], sa[3]);

			x_dd0 = x_dd4;
			x_dd1 = x_dd5;
		}

		if (j < width) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_zero);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_zero);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_zero);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_zero);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_dd2 = _mm_sub_epi32(x_st5_l, x_st1_l);
			x_dd3 = _mm_sub_epi32(x_st5_h, x_st1_h);

			x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
			x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
			x_dd0 = x_dd2;
			x_dd1 = x_dd3;
		}
	}
}

#elif IMG_TYPE == 2	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_si0, x_si1, x_si2, x_si3;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0_l, x_st1_l, x_st2_l, x_st4_l, x_st5_l, x_st6_l, x_st7_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st4_h, x_st5_h, x_st6_h, x_st7_h;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6;

	const __m128i x_const = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 *sa = (mlib_s32 *)&x_dd6;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
			x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1));
			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_dd0 = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_dd1 = _mm_unpackhi_epi16(x_sd0, x_si0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_sd2 = _mm_loadu_si128((void *)(sp + BCHAN1));
				x_sd3 = _mm_loadu_si128((void *)(sp + BCHAN1N));
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);
				x_dd2 = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_dd3 = _mm_unpackhi_epi16(x_sd2, x_si2);
				x_dd4 = _mm_unpacklo_epi16(x_sd3, x_si3);
				x_dd5 = _mm_unpackhi_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
				x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

				x_st0_l = _mm_add_epi32(x_dd3, x_dd4);
				x_st0_h = _mm_add_epi32(x_dd2, x_dd5);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * (j + 1)], sa[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 3], sa[3]);

				x_dd0 = x_dd4;
				x_dd1 = x_dd5;
			}

			if (j < width) {
				x_sd2 = _mm_loadu_si128(
				    (void *)(sp + BCHAN1));
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_dd2 = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_dd3 = _mm_unpackhi_epi16(x_sd2, x_si2);

				x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
				x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_si0 = _mm_srai_epi16(x_sd0, 15);
				x_si1 = _mm_srai_epi16(x_sd1, 15);
				x_si2 = _mm_srai_epi16(x_sd2, 15);
				x_si3 = _mm_srai_epi16(x_sd3, 15);

				x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
				x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
				x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
				x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);

				x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

				x_dd2 = _mm_sub_epi32(x_st5_l, x_st1_l);
				x_dd6 = _mm_srai_epi32(x_dd2, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadu_si128((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadu_si128((void *)(sp - CHAN1));
		x_sd2 = _mm_loadu_si128((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadu_si128((void *)(sp - CHAN1 + 2 * slb));

		x_si0 = _mm_srai_epi16(x_sd0, 15);
		x_si1 = _mm_srai_epi16(x_sd1, 15);
		x_si2 = _mm_srai_epi16(x_sd2, 15);
		x_si3 = _mm_srai_epi16(x_sd3, 15);

		x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
		x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
		x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
		x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
		x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
		x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
		x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
		x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

		x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
		x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
		x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
		x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

		x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_l, 32), x_const);
		x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

		x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
		x_st2_h = _mm_mul_epu32(
		    _mm_srli_epi64(x_st0_h, 32), x_const);
		x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
		x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
		x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

		x_dd0 = _mm_sub_epi32(x_st5_l, x_st1_l);
		x_dd1 = _mm_sub_epi32(x_st5_h, x_st1_h);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_dd2 = _mm_sub_epi32(x_st5_l, x_st1_l);
			x_dd3 = _mm_sub_epi32(x_st5_h, x_st1_h);

			k = j + 1;
			MLIB_BIC_LDSI(CHAN1, k, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_dd4 = _mm_sub_epi32(x_st5_l, x_st1_l);
			x_dd5 = _mm_sub_epi32(x_st5_h, x_st1_h);

			x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
			x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

			x_st0_l = _mm_add_epi32(x_dd3, x_dd4);
			x_st0_h = _mm_add_epi32(x_dd2, x_dd5);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * (j + 1)], sa[0]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 3], sa[3]);

			x_dd0 = x_dd4;
			x_dd1 = x_dd5;
		}

		if (j < width) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_si0 = _mm_srai_epi16(x_sd0, 15);
			x_si1 = _mm_srai_epi16(x_sd1, 15);
			x_si2 = _mm_srai_epi16(x_sd2, 15);
			x_si3 = _mm_srai_epi16(x_sd3, 15);

			x_sd0_l = _mm_unpacklo_epi16(x_sd0, x_si0);
			x_sd1_l = _mm_unpacklo_epi16(x_sd1, x_si1);
			x_sd2_l = _mm_unpacklo_epi16(x_sd2, x_si2);
			x_sd3_l = _mm_unpacklo_epi16(x_sd3, x_si3);
			x_sd0_h = _mm_unpackhi_epi16(x_sd0, x_si0);
			x_sd1_h = _mm_unpackhi_epi16(x_sd1, x_si1);
			x_sd2_h = _mm_unpackhi_epi16(x_sd2, x_si2);
			x_sd3_h = _mm_unpackhi_epi16(x_sd3, x_si3);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_st2_l = _mm_mul_epu32(x_st0_h, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_h, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_h = _mm_unpacklo_epi64(x_st4_l, x_st4_h);

			x_dd2 = _mm_sub_epi32(x_st5_l, x_st1_l);
			x_dd3 = _mm_sub_epi32(x_st5_h, x_st1_h);

			x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
			x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
			x_dd0 = x_dd2;
			x_dd1 = x_dd3;
		}
	}
}

#elif IMG_TYPE == 1	/* IMG_TYPE == 6 */
{
	DEF_VARS();

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;
	__m128i x_st0_l, x_st1_l, x_st2_l, x_st3_l, x_st4_l, x_st5_l, x_st6_l;
	__m128i x_st0_h, x_st1_h, x_st2_h, x_st3_h, x_st4_h, x_st5_h, x_st6_h;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6;

	const __m128i x_const = _mm_set1_epi16(BC_SUM_CONST);
	const __m128i x_const1 = _mm_set1_epi32(BC_SUM_CONST);
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 *sa = (mlib_s32 *)&x_dd6;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
			x_sd0 = _mm_loadl_epi64((void *)(sp - CHAN1));
			x_st0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_dd0 = _mm_unpacklo_epi16(x_st0_l, x_zero);
			x_dd1 = _mm_unpackhi_epi16(x_st0_l, x_zero);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				x_sd0 = _mm_loadu_si128((void *)(sp + BCHAN1));
				x_st0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_dd2 = _mm_unpacklo_epi16(x_st0_l, x_zero);
				x_dd3 = _mm_unpackhi_epi16(x_st0_l, x_zero);
				x_st0_h = _mm_unpackhi_epi8(x_sd0, x_zero);
				x_dd4 = _mm_unpacklo_epi16(x_st0_h, x_zero);
				x_dd5 = _mm_unpackhi_epi16(x_st0_h, x_zero);

				x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
				x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const1);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const1);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

				x_st0_l = _mm_add_epi32(x_dd3, x_dd4);
				x_st0_h = _mm_add_epi32(x_dd2, x_dd5);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const1);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const1);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);

				SAT_STORE(dp[CHAN1 * (j + 1)], sa[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 3], sa[3]);

				x_dd0 = x_dd4;
				x_dd1 = x_dd5;
			}

			if (j < width) {
				x_sd0 = _mm_loadl_epi64(
				    (void *)(sp + BCHAN1));
				x_st0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_dd2 = _mm_unpacklo_epi16(x_st0_l, x_zero);
				x_dd3 = _mm_unpackhi_epi16(x_st0_l, x_zero);

				x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
				x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
				x_st2_l = _mm_mul_epu32(x_st0_l, x_const1);
				x_st2_h = _mm_mul_epu32(
				    _mm_srli_epi64(x_st0_l, 32), x_const1);
				x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
				x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
				x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
				x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
				x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sa[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
			}
		}

		return;
	}

/* averaging along Y axis */
	if (dw) {
		__m128i x_dd7;
		mlib_s16 *sb = (mlib_s16 *)&x_dd7;
		for (i = 0; i < height; i++) {
			sp = p_src + 2 * slb * i;
			dp = p_dst + dlb * i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_BIC_LDSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi8(x_sd3, x_zero);
				x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);
				x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);
				x_sd2_h = _mm_unpackhi_epi8(x_sd2, x_zero);
				x_sd3_h = _mm_unpackhi_epi8(x_sd3, x_zero);

				x_st0_l = _mm_add_epi16(x_sd1_l, x_sd2_l);
				x_st0_h = _mm_add_epi16(x_sd1_h, x_sd2_h);
				x_st1_l = _mm_add_epi16(x_sd0_l, x_sd3_l);
				x_st1_h = _mm_add_epi16(x_sd0_h, x_sd3_h);

				x_st2_l = _mm_mullo_epi16(x_st0_l, x_const);
				x_st2_h = _mm_mullo_epi16(x_st0_h, x_const);

				x_st3_l = _mm_sub_epi16(x_st2_l, x_st1_l);
				x_st3_h = _mm_sub_epi16(x_st2_h, x_st1_h);
				x_dd7 = _mm_srai_epi16(x_st3_l, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sb[3]);
				x_dd7 = _mm_srai_epi16(x_st3_h, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * (j + 1)], sb[0]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 2], sb[2]);
				SAT_STORE(dp[CHAN1 * (j + 1) + 3], sb[3]);
			}

			if (j < width) {
				MLIB_BIC_LDLSI(0, j, x_sd0, x_sd1, x_sd2, x_sd3)

				x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
				x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);
				x_sd2_l = _mm_unpacklo_epi8(x_sd2, x_zero);
				x_sd3_l = _mm_unpacklo_epi8(x_sd3, x_zero);

				x_st0_l = _mm_add_epi16(x_sd1_l, x_sd2_l);
				x_st1_l = _mm_add_epi16(x_sd0_l, x_sd3_l);

				x_st2_l = _mm_mullo_epi16(x_st0_l, x_const);

				x_st3_l = _mm_sub_epi16(x_st2_l, x_st1_l);
				x_dd7 = _mm_srai_epi16(x_st3_l, BC_SUM1_CONST);
				SAT_STORE(dp[CHAN1 * j], sb[0]);
				SAT_STORE(dp[CHAN1 * j + 1], sb[1]);
				SAT_STORE(dp[CHAN1 * j + 2], sb[2]);
				SAT_STORE(dp[CHAN1 * j + 3], sb[3]);
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		sp = p_src + 2 * slb * i;
		dp = p_dst + dlb * i;

		x_sd0 = _mm_loadl_epi64((void *)(sp - CHAN1 - slb));
		x_sd1 = _mm_loadl_epi64((void *)(sp - CHAN1));
		x_sd2 = _mm_loadl_epi64((void *)(sp - CHAN1 + slb));
		x_sd3 = _mm_loadl_epi64((void *)(sp - CHAN1 + 2 * slb));

		x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
		x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);
		x_sd2_l = _mm_unpacklo_epi8(x_sd2, x_zero);
		x_sd3_l = _mm_unpacklo_epi8(x_sd3, x_zero);

		x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
		x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);

		x_st2_l = _mm_mullo_epi16(x_st0_l, x_const);
		x_st2_h = _mm_sub_epi16(x_st2_l, x_st1_l);
		x_st4_l = _mm_srai_epi16(x_st2_h, 15);
		x_dd0 = _mm_unpacklo_epi16(x_st2_h, x_st4_l);
		x_dd1 = _mm_unpackhi_epi16(x_st2_h, x_st4_l);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_BIC_LDSI(CHAN1, j, x_sd0, x_sd1, x_sd2, x_sd3)

			x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi8(x_sd3, x_zero);
			x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);
			x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);
			x_sd2_h = _mm_unpackhi_epi8(x_sd2, x_zero);
			x_sd3_h = _mm_unpackhi_epi8(x_sd3, x_zero);

			x_st0_l = _mm_add_epi32(x_sd1_l, x_sd2_l);
			x_st0_h = _mm_add_epi32(x_sd1_h, x_sd2_h);
			x_st1_l = _mm_add_epi32(x_sd0_l, x_sd3_l);
			x_st1_h = _mm_add_epi32(x_sd0_h, x_sd3_h);

			x_st2_l = _mm_mullo_epi16(x_st0_l, x_const);
			x_st2_h = _mm_mullo_epi16(x_st0_h, x_const);

			x_st3_l = _mm_sub_epi16(x_st2_l, x_st1_l);
			x_st3_h = _mm_sub_epi16(x_st2_h, x_st1_h);

			x_st4_l = _mm_srai_epi16(x_st3_l, 15);
			x_st4_h = _mm_srai_epi16(x_st3_h, 15);

			x_dd2 = _mm_unpacklo_epi16(x_st3_l, x_st4_l);
			x_dd3 = _mm_unpackhi_epi16(x_st3_l, x_st4_l);
			x_dd4 = _mm_unpacklo_epi16(x_st3_h, x_st4_h);
			x_dd5 = _mm_unpackhi_epi16(x_st3_h, x_st4_h);

			x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
			x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const1);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const1);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);
			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * j + 3], sa[3]);

			x_st0_l = _mm_add_epi32(x_dd3, x_dd4);
			x_st0_h = _mm_add_epi32(x_dd2, x_dd5);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const1);
			x_st2_h = _mm_mul_epu32(
			    _mm_srli_epi64(x_st0_l, 32), x_const1);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);

			SAT_STORE(dp[CHAN1 * (j + 1)], sa[0]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * (j + 1) + 3], sa[3]);

			x_dd0 = x_dd4;
			x_dd1 = x_dd5;
		}

		if (j < width) {
			mlib_s32 tab = BCHAN1;
			x_sd0 = _mm_loadl_epi64((void *)(sp + tab - slb));
			x_sd1 = _mm_loadl_epi64((void *)(sp + tab));
			x_sd2 = _mm_loadl_epi64((void *)(sp + tab + slb));
			x_sd3 = _mm_loadl_epi64((void *)(sp + tab + 2 * slb));

			x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);
			x_sd2_l = _mm_unpacklo_epi8(x_sd2, x_zero);
			x_sd3_l = _mm_unpacklo_epi8(x_sd3, x_zero);

			x_st0_l = _mm_add_epi16(x_sd1_l, x_sd2_l);
			x_st1_l = _mm_add_epi16(x_sd0_l, x_sd3_l);

			x_st2_l = _mm_mullo_epi16(x_st0_l, x_const);
			x_st3_l = _mm_sub_epi16(x_st2_l, x_st1_l);
			x_st4_l = _mm_srai_epi16(x_st3_l, 15);

			x_dd2 = _mm_unpacklo_epi16(x_st3_l, x_st4_l);
			x_dd3 = _mm_unpackhi_epi16(x_st3_l, x_st4_l);

			x_st0_l = _mm_add_epi32(x_dd1, x_dd2);
			x_st0_h = _mm_add_epi32(x_dd0, x_dd3);
			x_st2_l = _mm_mul_epu32(x_st0_l, x_const1);
			x_st2_h = _mm_mul_epu32(_mm_srli_epi64(x_st0_l, 32),
			    x_const1);
			x_st4_l = _mm_unpacklo_epi32(x_st2_l, x_st2_h);
			x_st4_h = _mm_unpackhi_epi32(x_st2_l, x_st2_h);
			x_st5_l = _mm_unpacklo_epi64(x_st4_l, x_st4_h);
			x_st5_h = _mm_sub_epi32(x_st5_l, x_st0_h);
			x_dd6 = _mm_srai_epi32(x_st5_h, BC_SUM2_CONST);
			SAT_STORE(dp[CHAN1 * j], sa[0]);
			SAT_STORE(dp[CHAN1 * j + 1], sa[1]);
			SAT_STORE(dp[CHAN1 * j + 2], sa[2]);
			SAT_STORE(dp[CHAN1 * j + 3], sa[3]);
		}
	}
}

#endif

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEZOOMOUT2X_BC_H */
