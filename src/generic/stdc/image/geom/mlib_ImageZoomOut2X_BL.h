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

#ifndef _MLIB_IMAGEZOOMOUT2X_BL_H
#define	_MLIB_IMAGEZOOMOUT2X_BL_H

#pragma ident	"@(#)mlib_ImageZoomOut2X_BL.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

#if IMG_TYPE == 1

#define	STYPE	mlib_u8
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_U8_##SUFF

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s16
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_S16_##SUFF

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 */

#define	STYPE	mlib_u16
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_U16_##SUFF

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s32
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_S32_##SUFF

#elif IMG_TYPE == 5	/* IMG_TYPE == 1 */

#define	STYPE	mlib_f32
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_F32_##SUFF

#elif IMG_TYPE == 6	/* IMG_TYPE == 1 */

#define	STYPE	mlib_d64
#define	ZOOM_FUNC(SUFF)	mlib_ImageZoomOut2X_D64_##SUFF

#endif		   /* IMG_TYPE == 1 */

#if IMG_TYPE < 4

#define	AVE2(a, b)	(((a) + (b)) >> 1)
#define	AVE4(a, b, c, d)	(((a) + (b) + (c) + (d)) >> 2)

#elif IMG_TYPE == 4	/* IMG_TYPE < 4 */

#ifdef _NO_LONGLONG

#define	AVE2(a, b)	(((a) >> 1) + ((b) >> 1))
#define	AVE4(a, b, c, d)	(((a) >> 2) + ((b) >> 2) + \
				((c) >> 2) + ((d) >> 2))
#else		   /* _NO_LONGLONG */

#define	AVE2(a, b)	(((mlib_s64)(a) + (b)) >> 1)
#define	AVE4(a, b, c, d)	(((mlib_s64)(a) + (b) + (c) + (d)) >> 2)

#endif		   /* _NO_LONGLONG */

#else		   /* IMG_TYPE < 4 */

#define	AVE2(a, b)	(((a) + (b)) * (STYPE)0.5)
#define	AVE4(a, b, c, d)	(((a) + (b) + (c) + (d)) * (STYPE)0.25)

#endif		   /* IMG_TYPE < 4 */

#define	BL_PARAMETRS(type)                                              \
	type *sp, type * dp, mlib_s32 slb, mlib_s32 dlb,                \
	mlib_s32 height, mlib_s32 width, mlib_s32 dh, mlib_s32 dw

/* *********************************************************** */

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + 1]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[2 * j] = AVE2(sp[4 * j], sp[4 * j + 2]);
				dp[2 * j + 1] =
				    AVE2(sp[4 * j + 1], sp[4 * j + 3]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[2 * j] = AVE2(sp[4 * j], sp[4 * j + slb]);
				dp[2 * j + 1] =
				    AVE2(sp[4 * j + 1], sp[4 * j + slb + 1]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			dp[2 * j] =
			    AVE4(sp[4 * j], sp[4 * j + 2], sp[4 * j + slb],
			    sp[4 * j + slb + 2]);
			dp[2 * j + 1] =
			    AVE4(sp[4 * j + 1], sp[4 * j + 3],
			    sp[4 * j + slb + 1], sp[4 * j + slb + 3]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status ZOOM_FUNC(
    3_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[3 * j] = AVE2(sp[6 * j], sp[6 * j + 3]);
				dp[3 * j + 1] =
				    AVE2(sp[6 * j + 1], sp[6 * j + 4]);
				dp[3 * j + 2] =
				    AVE2(sp[6 * j + 2], sp[6 * j + 5]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[3 * j] = AVE2(sp[6 * j], sp[6 * j + slb]);
				dp[3 * j + 1] =
				    AVE2(sp[6 * j + 1], sp[6 * j + 1 + slb]);
				dp[3 * j + 2] =
				    AVE2(sp[6 * j + 2], sp[6 * j + 2 + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			dp[3 * j] =
			    AVE4(sp[6 * j], sp[6 * j + 3], sp[6 * j + slb],
			    sp[6 * j + slb + 3]);
			dp[3 * j + 1] =
			    AVE4(sp[6 * j + 1], sp[6 * j + 4],
			    sp[6 * j + slb + 1], sp[6 * j + slb + 4]);
			dp[3 * j + 2] =
			    AVE4(sp[6 * j + 2], sp[6 * j + 5],
			    sp[6 * j + slb + 2], sp[6 * j + slb + 5]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if IMG_TYPE == 1

mlib_status
mlib_ImageZoomOut2X_U8_4_Bilinear_al(
    BL_PARAMETRS(mlib_u8))
{
	mlib_u32 s0, s1, s2, s3;
	mlib_s32 i, j;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s0 = ((mlib_u32 *)(sp + 8 * j))[0];
				s1 = ((mlib_u32 *)(sp + 8 * j))[1];
				*(mlib_u32 *)(dp + 4 * j) =
				    ((s0 >> 1) & 0x7f7f7f7f) +
				    ((s1 >> 1) & 0x7f7f7f7f);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				s0 = *(mlib_u32 *)(sp + 8 * j);
				s1 = *(mlib_u32 *)(sp + 8 * j + slb);
				*(mlib_u32 *)(dp + 4 * j) =
				    ((s0 >> 1) & 0x7f7f7f7f) +
				    ((s1 >> 1) & 0x7f7f7f7f);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			s0 = ((mlib_u32 *)(sp + 8 * j))[0];
			s1 = ((mlib_u32 *)(sp + 8 * j))[1];
			s2 = ((mlib_u32 *)(sp + slb + 8 * j))[0];
			s3 = ((mlib_u32 *)(sp + slb + 8 * j))[1];
			*(mlib_u32 *)(dp + 4 * j) =
			    ((s0 >> 2) & 0x3f3f3f3f) +
			    ((s1 >> 2) & 0x3f3f3f3f) +
			    ((s2 >> 2) & 0x3f3f3f3f) +
			    ((s3 >> 2) & 0x3f3f3f3f) + 0x01010101;
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#endif /* IMG_TYPE == 1 */

/* *********************************************************** */

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

#if IMG_TYPE == 1

/* aligned case */
	if ((((mlib_s32)sp | (mlib_s32)dp | slb | dlb) & 3) == 0) {
		return mlib_ImageZoomOut2X_U8_4_Bilinear_al(sp, dp, slb, dlb,
		    height, width, dh, dw);
	}
#endif /* IMG_TYPE == 1 */

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[4 * j] = AVE2(sp[8 * j], sp[8 * j + 4]);
				dp[4 * j + 1] =
				    AVE2(sp[8 * j + 1], sp[8 * j + 5]);
				dp[4 * j + 2] =
				    AVE2(sp[8 * j + 2], sp[8 * j + 6]);
				dp[4 * j + 3] =
				    AVE2(sp[8 * j + 3], sp[8 * j + 7]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				dp[4 * j] = AVE2(sp[8 * j], sp[8 * j + slb]);
				dp[4 * j + 1] =
				    AVE2(sp[8 * j + 1], sp[8 * j + slb + 1]);
				dp[4 * j + 2] =
				    AVE2(sp[8 * j + 2], sp[8 * j + slb + 2]);
				dp[4 * j + 3] =
				    AVE2(sp[8 * j + 3], sp[8 * j + slb + 3]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			dp[4 * j] =
			    AVE4(sp[8 * j], sp[8 * j + 4], sp[8 * j + slb],
			    sp[8 * j + slb + 4]);
			dp[4 * j + 1] =
			    AVE4(sp[8 * j + 1], sp[8 * j + 5],
			    sp[8 * j + slb + 1], sp[8 * j + slb + 5]);
			dp[4 * j + 2] =
			    AVE4(sp[8 * j + 2], sp[8 * j + 6],
			    sp[8 * j + slb + 2], sp[8 * j + slb + 6]);
			dp[4 * j + 3] =
			    AVE4(sp[8 * j + 3], sp[8 * j + 7],
			    sp[8 * j + slb + 3], sp[8 * j + slb + 7]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEZOOMOUT2X_BL_H */
