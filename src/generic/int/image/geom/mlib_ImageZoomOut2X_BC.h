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

#ifndef _MLIB_IMAGEZOOMOUT2X_BC_H
#define	_MLIB_IMAGEZOOMOUT2X_BC_H

#pragma ident	"@(#)mlib_ImageZoomOut2X_BC.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageZoom2X.h>

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

/* *********************************************************** */

#if IMG_TYPE < 4

#define	TTYPE	mlib_s32
#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SUM1_CONST)
#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SUM2_CONST)

#elif IMG_TYPE == 4	/* IMG_TYPE < 4 */

#ifndef	_NO_LONGLONG

#define	TTYPE	mlib_s64
#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SUM1_CONST)
#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SUM2_CONST)

#else /* _NO_LONGLONG */

#define	TTYPE	mlib_d64
#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) * (1.0 / BC_SUM1_DIV))
#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) * (1.0 / BC_SUM2_DIV))

#endif /* _NO_LONGLONG */

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
	mlib_s32 i, j

#else		   /* IMG_TYPE == 2 || IMG_TYPE == 3 */

#define	DEF_VARS()                                              \
	STYPE *sp, *dp;                                         \
	TTYPE a0, a1, a2, a3, res;                              \
	mlib_s32 i, j

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
				a2 = sp[CHAN2 * j + CHAN1];
				a3 = sp[CHAN2 * j + CHAN2];
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
		a2 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a3 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			a0 = a2;
			a1 = a3;
			a2 = BC_SUM(sp[CHAN2 * j + CHAN1 - slb],
			    sp[CHAN2 * j + CHAN1], sp[CHAN2 * j + CHAN1 + slb],
			    sp[CHAN2 * j + CHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[CHAN2 * j + CHAN2 - slb],
			    sp[CHAN2 * j + CHAN2], sp[CHAN2 * j + CHAN2 + slb],
			    sp[CHAN2 * j + CHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			SAT_STORE(dp[j], res);
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

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
{
	DEF_VARS();
	mlib_s32 k;

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
					a2 = sp[CHAN2 * j + CHAN1];
					a3 = sp[CHAN2 * j + CHAN2];
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
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = sp[CHAN2 * j - slb];
					a1 = sp[CHAN2 * j];
					a2 = sp[CHAN2 * j + slb];
					a3 = sp[CHAN2 * j + 2 * slb];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (k = 0; k < CHAN1; k++) {
			sp = p_src + 2 * slb * i + k;
			dp = p_dst + dlb * i + k;
			a2 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
			    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
			a3 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = BC_SUM(sp[CHAN2 * j + CHAN1 - slb],
				    sp[CHAN2 * j + CHAN1],
				    sp[CHAN2 * j + CHAN1 + slb],
				    sp[CHAN2 * j + CHAN1 + 2 * slb]);
				a3 = BC_SUM(sp[CHAN2 * j + CHAN2 - slb],
				    sp[CHAN2 * j + CHAN2],
				    sp[CHAN2 * j + CHAN2 + slb],
				    sp[CHAN2 * j + CHAN2 + 2 * slb]);
				res = BC_SUM2(a0, a1, a2, a3);
				SAT_STORE(dp[CHAN1 * j], res);
			}
		}
	}
}

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
{
	DEF_VARS();
	mlib_s32 k;

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
					a2 = sp[CHAN2 * j + CHAN1];
					a3 = sp[CHAN2 * j + CHAN2];
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
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = sp[CHAN2 * j - slb];
					a1 = sp[CHAN2 * j];
					a2 = sp[CHAN2 * j + slb];
					a3 = sp[CHAN2 * j + 2 * slb];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (k = 0; k < CHAN1; k++) {
			sp = p_src + 2 * slb * i + k;
			dp = p_dst + dlb * i + k;
			a2 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
			    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
			a3 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = BC_SUM(sp[CHAN2 * j + CHAN1 - slb],
				    sp[CHAN2 * j + CHAN1],
				    sp[CHAN2 * j + CHAN1 + slb],
				    sp[CHAN2 * j + CHAN1 + 2 * slb]);
				a3 = BC_SUM(sp[CHAN2 * j + CHAN2 - slb],
				    sp[CHAN2 * j + CHAN2],
				    sp[CHAN2 * j + CHAN2 + slb],
				    sp[CHAN2 * j + CHAN2 + 2 * slb]);
				res = BC_SUM2(a0, a1, a2, a3);
				SAT_STORE(dp[CHAN1 * j], res);
			}
		}
	}
}

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
{
	DEF_VARS();
	mlib_s32 k;

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
					a2 = sp[CHAN2 * j + CHAN1];
					a3 = sp[CHAN2 * j + CHAN2];
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
			for (k = 0; k < CHAN1; k++) {
				sp = p_src + 2 * slb * i + k;
				dp = p_dst + dlb * i + k;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					a0 = sp[CHAN2 * j - slb];
					a1 = sp[CHAN2 * j];
					a2 = sp[CHAN2 * j + slb];
					a3 = sp[CHAN2 * j + 2 * slb];
					res = BC_SUM1(a0, a1, a2, a3);
					SAT_STORE(dp[CHAN1 * j], res);
				}
			}
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (k = 0; k < CHAN1; k++) {
			sp = p_src + 2 * slb * i + k;
			dp = p_dst + dlb * i + k;
			a2 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1],
			    sp[slb - CHAN1], sp[2 * slb - CHAN1]);
			a3 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = BC_SUM(sp[CHAN2 * j + CHAN1 - slb],
				    sp[CHAN2 * j + CHAN1],
				    sp[CHAN2 * j + CHAN1 + slb],
				    sp[CHAN2 * j + CHAN1 + 2 * slb]);
				a3 = BC_SUM(sp[CHAN2 * j + CHAN2 - slb],
				    sp[CHAN2 * j + CHAN2],
				    sp[CHAN2 * j + CHAN2 + slb],
				    sp[CHAN2 * j + CHAN2 + 2 * slb]);
				res = BC_SUM2(a0, a1, a2, a3);
				SAT_STORE(dp[CHAN1 * j], res);
			}
		}
	}
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEZOOMOUT2X_BC_H */
