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

#ifndef _MLIB_S_IMAGEZOOMOUT2X_BL_H
#define	_MLIB_S_IMAGEZOOMOUT2X_BL_H

#pragma ident	"@(#)mlib_s_ImageZoomOut2X_BL.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */
#define	STU_PD	_mm_storeu_pd
#define	ST_PD	_mm_store_pd
#define	LDU_PD	_mm_loadu_pd
#define	LD_PD	_mm_load_pd

#define	STU_SI	_mm_storeu_si128
#define	ST_SI	_mm_store_si128
#define	STL_SI	_mm_storel_epi64
#define	LDU_SI	_mm_loadu_si128
#define	LD_SI	_mm_load_si128
#define	LDL_SI	_mm_loadl_epi64

#define	MLIB_S_3CHAN_LDSI(ld, t1, t2)                           \
	sd0 = _mm_##ld##_si128((void *)(sp + 6 * j));              \
	sd1 = _mm_##ld##_si128((void *)(sp + 6 * j + slb));        \
	sd2 = _mm_##ld##_si128((void *)(sp + 6 * j + t1));         \
	sd3 = _mm_##ld##_si128((void *)(sp + 6 * j + slb + t1));   \
	sd4 = _mm_##ld##_si128((void *)(sp + 6 * j + t2));         \
	sd5 = _mm_##ld##_si128((void *)(sp + 6 * j + slb + t2));

#define	MLIB_S_LDSI(ld, w, tap)                                \
	sd0 = _mm_##ld##_si128((void *)(sp + w * j));            \
	sd1 = _mm_##ld##_si128((void *)(sp + w * j + slb));      \
	sd2 = _mm_##ld##_si128((void *)(sp + w * j + tap));      \
	sd3 = _mm_##ld##_si128((void *)(sp + w * j + slb + tap))

#define	MLIB_S_LDSIP(ld, w, tap)                               \
	sd0 = _mm_##ld##_si128((void *)(sp + w * j));            \
	sd1 = _mm_##ld##_si128((void *)(sp + w * j + tap));      \

#define	MLIB_S_LDPS(ld, w, tap)                                \
	sd0 = _mm_##ld##_ps(sp + w * j);                         \
	sd1 = _mm_##ld##_ps(sp + w * j + slb);                   \
	sd2 = _mm_##ld##_ps(sp + w * j +  tap);                  \
	sd3 = _mm_##ld##_ps(sp + w * j + slb + tap);

#define	MLIB_S_LDPD(ld, w, tap)                                   \
	sd0 = _mm_##ld##_pd(sp + w * j);                         \
	sd1 = _mm_##ld##_pd(sp + w * j + slb);                   \
	sd2 = _mm_##ld##_pd(sp + w * j +  tap);                  \
	sd3 = _mm_##ld##_pd(sp + w * j + slb + tap);

/* *********************************************************** */

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

#if IMG_TYPE == 6

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	const __m128d ave2 = _mm_set1_pd(0.5);
	const __m128d ave4 = _mm_set1_pd(0.25);
	__m128d sd0, sd1, sd2, sd3, st0, st1, st2, st3, st4, dd0;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			j = 0;
			if ((mlib_addr)dp & 15) {
				dp[0] = AVE2(sp[0], sp[1]);
				j = 1;
			}

			if (0 == ((mlib_addr)sp & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 1; j += 2) {
					sd0 = LD_PD(sp + 2 * j);
					sd1 = LD_PD(sp + 2 * (j + 1));
					st0 = _mm_unpacklo_pd(sd0, sd1);
					st1 = _mm_unpackhi_pd(sd0, sd1);
					st2 = _mm_add_pd(st0, st1);
					dd0 = _mm_mul_pd(st2, ave2);
					ST_PD(dp + j, dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 1; j += 2) {
					sd0 = LDU_PD(sp + 2 * j);
					sd1 = LDU_PD(sp + 2 * (j + 1));
					st0 = _mm_unpacklo_pd(sd0, sd1);
					st1 = _mm_unpackhi_pd(sd0, sd1);
					st2 = _mm_add_pd(st0, st1);
					dd0 = _mm_mul_pd(st2, ave2);
					ST_PD(dp + j, dd0);
				}
			}

			if (j < width) {
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
			j = 0;
			if ((mlib_addr)dp & 15) {
				dp[0] = AVE2(sp[0], sp[slb]);
				j = 1;
			}

			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 1; j += 2) {
					MLIB_S_LDPD(load, 2, 2)
					st0 = _mm_unpacklo_pd(sd0, sd2);
					st1 = _mm_unpacklo_pd(sd1, sd3);
					st2 = _mm_add_pd(st0, st1);
					dd0 = _mm_mul_pd(st2, ave2);
					ST_PD(dp + j, dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 1; j += 2) {
					MLIB_S_LDPD(loadu, 2, 2)
					st0 = _mm_unpacklo_pd(sd0, sd2);
					st1 = _mm_unpacklo_pd(sd1, sd3);
					st2 = _mm_add_pd(st0, st1);
					dd0 = _mm_mul_pd(st2, ave2);
					ST_PD(dp + j, dd0);
				}
			}

			if (j < width) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		j = 0;
		if ((mlib_addr)dp & 15) {
			dp[0] =
			    AVE4(sp[0], sp[1], sp[slb],
			    sp[slb + 1]);
			j = 1;
		}

		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 1; j += 2) {
				MLIB_S_LDPD(load, 2, 2)
				st0 = _mm_add_pd(sd0, sd1);
				st1 = _mm_add_pd(sd2, sd3);
				st2 = _mm_unpacklo_pd(st0, st1);
				st3 = _mm_unpackhi_pd(st0, st1);
				st4 = _mm_add_pd(st2, st3);
				dd0 = _mm_mul_pd(st4, ave4);
				ST_PD(dp + j, dd0);
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 1; j += 2) {
				MLIB_S_LDPD(loadu, 2, 2)
				st0 = _mm_add_pd(sd0, sd1);
				st1 = _mm_add_pd(sd2, sd3);
				st2 = _mm_unpacklo_pd(st0, st1);
				st3 = _mm_unpackhi_pd(st0, st1);
				st4 = _mm_add_pd(st2, st3);
				dd0 = _mm_mul_pd(st4, ave4);
				ST_PD(dp + j, dd0);
			}
		}

		if (j < width) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 5 /* IMG_TYPE == 6 */

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	const __m128 ave2 = _mm_set1_ps(0.5);
	const __m128 ave4 = _mm_set1_ps(0.25);
	__m128 sd0, sd1, sd2, sd3, dd0;
	__m128 st0, st1, st2, st3, st4, st5, st6;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + 1]);
			}

			if (0 == ((mlib_addr)(sp + 2 * j) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					sd0 = _mm_load_ps(sp + 2 * j);
					sd1 = _mm_load_ps(sp + 2 * (j + 2));
					st0 = _mm_unpacklo_ps(sd0, sd1);
					st1 = _mm_unpackhi_ps(sd0, sd1);
					st2 = _mm_unpacklo_ps(st0, st1);
					st3 = _mm_unpackhi_ps(st0, st1);
					st4 = _mm_add_ps(st2, st3);
					dd0 = _mm_mul_ps(st4, ave2);
					_mm_store_ps(dp + j, dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					sd0 = _mm_loadu_ps(sp + 2 * j);
					sd1 = _mm_loadu_ps(sp + 2 * (j + 2));
					st0 = _mm_unpacklo_ps(sd0, sd1);
					st1 = _mm_unpackhi_ps(sd0, sd1);
					st2 = _mm_unpacklo_ps(st0, st1);
					st3 = _mm_unpackhi_ps(st0, st1);
					st4 = _mm_add_ps(st2, st3);
					dd0 = _mm_mul_ps(st4, ave2);
					_mm_store_ps(dp + j, dd0);
				}
			}

			for (; j < width; j++) {
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
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
			    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					MLIB_S_LDPS(load, 2, 4)
					st0 = _mm_add_ps(sd0, sd1);
					st1 = _mm_add_ps(sd2, sd3);
					st2 = _mm_unpacklo_ps(st0, st1);
					st3 = _mm_unpackhi_ps(st0, st1);
					st4 = _mm_unpacklo_ps(st2, st3);
					dd0 = _mm_mul_ps(st4, ave2);
					_mm_store_ps(dp + j, dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					MLIB_S_LDPS(loadu, 2, 4)
					st0 = _mm_add_ps(sd0, sd1);
					st1 = _mm_add_ps(sd2, sd3);
					st2 = _mm_unpacklo_ps(st0, st1);
					st3 = _mm_unpackhi_ps(st0, st1);
					st4 = _mm_unpacklo_ps(st2, st3);
					dd0 = _mm_mul_ps(st4, ave2);
					_mm_store_ps(dp + j, dd0);
				}
			}

			for (; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (j = 0; (j < width) && ((mlib_addr)(dp + j) & 15); j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
		    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 3; j += 4) {
				MLIB_S_LDPS(load, 2, 4)
				st0 = _mm_add_ps(sd0, sd1);
				st1 = _mm_add_ps(sd2, sd3);
				st2 = _mm_unpacklo_ps(st0, st1);
				st3 = _mm_unpackhi_ps(st0, st1);
				st4 = _mm_unpacklo_ps(st2, st3);
				st5 = _mm_unpackhi_ps(st2, st3);
				st6 = _mm_add_ps(st4, st5);
				dd0 = _mm_mul_ps(st6, ave4);
				_mm_store_ps(dp + j, dd0);
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 3; j += 4) {
				MLIB_S_LDPS(loadu, 2, 4)
				st0 = _mm_add_ps(sd0, sd1);
				st1 = _mm_add_ps(sd2, sd3);
				st2 = _mm_unpacklo_ps(st0, st1);
				st3 = _mm_unpackhi_ps(st0, st1);
				st4 = _mm_unpacklo_ps(st2, st3);
				st5 = _mm_unpackhi_ps(st2, st3);
				st6 = _mm_add_ps(st4, st5);
				dd0 = _mm_mul_ps(st6, ave4);
				_mm_store_ps(dp + j, dd0);
			}
		}

		for (; j < width; j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 4 /* IMG_TYPE == 6 */

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5, st6;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + 1]);
			}

			if (0 == ((mlib_addr)(sp + 2 * j) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					MLIB_S_LDSIP(load, 2, 4)
					st0 = _mm_unpacklo_epi32(sd0, sd1);
					st1 = _mm_unpackhi_epi32(sd0, sd1);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					st4 = _mm_srai_epi32(st2, 1);
					st5 = _mm_srai_epi32(st3, 1);
					dd0 = _mm_add_epi32(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					MLIB_S_LDSIP(loadu, 2, 4)
					st0 = _mm_unpacklo_epi32(sd0, sd1);
					st1 = _mm_unpackhi_epi32(sd0, sd1);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					st4 = _mm_srai_epi32(st2, 1);
					st5 = _mm_srai_epi32(st3, 1);
					dd0 = _mm_add_epi32(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			}

			for (; j < width; j++) {
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
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
			    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					MLIB_S_LDSI(load, 2, 4);
					sd0 = _mm_srai_epi32(sd0, 1);
					sd1 = _mm_srai_epi32(sd1, 1);
					sd2 = _mm_srai_epi32(sd2, 1);
					sd3 = _mm_srai_epi32(sd3, 1);
					st0 = _mm_add_epi32(sd0, sd1);
					st1 = _mm_add_epi32(sd2, sd3);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_unpacklo_epi32(st2, st3);
					ST_SI((void *)(dp + j), dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 3; j += 4) {
					MLIB_S_LDSI(loadu, 2, 4);
					sd0 = _mm_srai_epi32(sd0, 1);
					sd1 = _mm_srai_epi32(sd1, 1);
					sd2 = _mm_srai_epi32(sd2, 1);
					sd3 = _mm_srai_epi32(sd3, 1);
					st0 = _mm_add_epi32(sd0, sd1);
					st1 = _mm_add_epi32(sd2, sd3);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_unpacklo_epi32(st2, st3);
					ST_SI((void *)(dp + j), dd0);
				}
			}

			for (; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (j = 0; (j < width) && ((mlib_addr)(dp + j) & 15); j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
		    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 3; j += 4) {
				MLIB_S_LDSI(load, 2, 4);
				sd0 = _mm_srai_epi32(sd0, 2);
				sd1 = _mm_srai_epi32(sd1, 2);
				sd2 = _mm_srai_epi32(sd2, 2);
				sd3 = _mm_srai_epi32(sd3, 2);
				st0 = _mm_add_epi32(sd0, sd1);
				st1 = _mm_add_epi32(sd2, sd3);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_unpacklo_epi32(st2, st3);
				st5 = _mm_unpackhi_epi32(st2, st3);
				dd0 = _mm_add_epi32(st4, st5);
				ST_SI((void *)(dp + j), dd0);
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 3; j += 4) {
				MLIB_S_LDSI(loadu, 2, 4);
				sd0 = _mm_srai_epi32(sd0, 2);
				sd1 = _mm_srai_epi32(sd1, 2);
				sd2 = _mm_srai_epi32(sd2, 2);
				sd3 = _mm_srai_epi32(sd3, 2);
				st0 = _mm_add_epi32(sd0, sd1);
				st1 = _mm_add_epi32(sd2, sd3);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_unpacklo_epi32(st2, st3);
				st5 = _mm_unpackhi_epi32(st2, st3);
				dd0 = _mm_add_epi32(st4, st5);
				ST_SI((void *)(dp + j), dd0);
			}
		}

		for (; j < width; j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 3	/* IMG_TYPE == 6 */

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5, st6, st7;

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + 1]);
			}

			if (0 == ((mlib_addr)(sp + 2 * j) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSIP(load, 2, 8)
					st0 = _mm_unpacklo_epi16(sd0, sd1);
					st1 = _mm_unpackhi_epi16(sd0, sd1);
					st2 = _mm_unpacklo_epi16(st0, st1);
					st3 = _mm_unpackhi_epi16(st0, st1);
					st4 = _mm_unpacklo_epi16(st2, st3);
					st5 = _mm_unpackhi_epi16(st2, st3);
					dd0 = _mm_avg_epu16(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSIP(loadu, 2, 8)
					st0 = _mm_unpacklo_epi16(sd0, sd1);
					st1 = _mm_unpackhi_epi16(sd0, sd1);
					st2 = _mm_unpacklo_epi16(st0, st1);
					st3 = _mm_unpackhi_epi16(st0, st1);
					st4 = _mm_unpacklo_epi16(st2, st3);
					st5 = _mm_unpackhi_epi16(st2, st3);
					dd0 = _mm_avg_epu16(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			}

			for (; j < width; j++) {
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
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
			    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSI(load, 2, 8);
					st0 = _mm_avg_epu16(sd0, sd1);
					st1 = _mm_avg_epu16(sd2, sd3);
					st2 = _mm_unpacklo_epi16(st0, st1);
					st3 = _mm_unpackhi_epi16(st0, st1);
					st4 = _mm_unpacklo_epi16(st2, st3);
					st5 = _mm_unpackhi_epi16(st2, st3);
					dd0 = _mm_unpacklo_epi16(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSI(loadu, 2, 8);
					st0 = _mm_avg_epu16(sd0, sd1);
					st1 = _mm_avg_epu16(sd2, sd3);
					st2 = _mm_unpacklo_epi16(st0, st1);
					st3 = _mm_unpackhi_epi16(st0, st1);
					st4 = _mm_unpacklo_epi16(st2, st3);
					st5 = _mm_unpackhi_epi16(st2, st3);
					dd0 = _mm_unpacklo_epi16(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			}

			for (; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (j = 0; (j < width) && ((mlib_addr)(dp + j) & 15); j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
		    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 7; j += 8) {
				MLIB_S_LDSI(load, 2, 8);
				st0 = _mm_avg_epu16(sd0, sd1);
				st1 = _mm_avg_epu16(sd2, sd3);
				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				st6 = _mm_unpacklo_epi16(st4, st5);
				st7 = _mm_unpackhi_epi16(st4, st5);
				dd0 = _mm_avg_epu16(st6, st7);
				ST_SI((void *)(dp + j), dd0);
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 7; j += 8) {
				MLIB_S_LDSI(loadu, 2, 8);
				st0 = _mm_avg_epu16(sd0, sd1);
				st1 = _mm_avg_epu16(sd2, sd3);
				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				st6 = _mm_unpacklo_epi16(st4, st5);
				st7 = _mm_unpackhi_epi16(st4, st5);
				dd0 = _mm_avg_epu16(st6, st7);
				ST_SI((void *)(dp + j), dd0);
			}
		}

		for (; j < width; j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 2	/* IMG_TYPE == 6 */

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5, st6, st7;
	const __m128i one = _mm_set1_epi16(1);

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + 1]);
			}

			if (0 == ((mlib_addr)(sp + 2 * j) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSIP(load, 2, 8)
					st0 = _mm_srai_epi16(sd0, 1);
					st1 = _mm_srai_epi16(sd1, 1);
					st2 = _mm_madd_epi16(st0, one);
					st3 = _mm_madd_epi16(st1, one);
					dd0 = _mm_packs_epi32(st2, st3);
					ST_SI((void *)(dp + j), dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSIP(loadu, 2, 8)
					st0 = _mm_srai_epi16(sd0, 1);
					st1 = _mm_srai_epi16(sd1, 1);
					st2 = _mm_madd_epi16(st0, one);
					st3 = _mm_madd_epi16(st1, one);
					dd0 = _mm_packs_epi32(st2, st3);
					ST_SI((void *)(dp + j), dd0);
				}
			}

			for (; j < width; j++) {
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
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
			    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSI(load, 2, 8);
					sd0 = _mm_srai_epi16(sd0, 1);
					sd1 = _mm_srai_epi16(sd1, 1);
					sd2 = _mm_srai_epi16(sd2, 1);
					sd3 = _mm_srai_epi16(sd3, 1);
					st0 = _mm_add_epi16(sd0, sd1);
					st1 = _mm_add_epi16(sd2, sd3);
					st2 = _mm_unpacklo_epi16(st0, st1);
					st3 = _mm_unpackhi_epi16(st0, st1);
					st4 = _mm_unpacklo_epi16(st2, st3);
					st5 = _mm_unpackhi_epi16(st2, st3);
					dd0 = _mm_unpacklo_epi16(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width - 7; j += 8) {
					MLIB_S_LDSI(loadu, 2, 8);
					sd0 = _mm_srai_epi16(sd0, 1);
					sd1 = _mm_srai_epi16(sd1, 1);
					sd2 = _mm_srai_epi16(sd2, 1);
					sd3 = _mm_srai_epi16(sd3, 1);
					st0 = _mm_add_epi16(sd0, sd1);
					st1 = _mm_add_epi16(sd2, sd3);
					st2 = _mm_unpacklo_epi16(st0, st1);
					st3 = _mm_unpackhi_epi16(st0, st1);
					st4 = _mm_unpacklo_epi16(st2, st3);
					st5 = _mm_unpackhi_epi16(st2, st3);
					dd0 = _mm_unpacklo_epi16(st4, st5);
					ST_SI((void *)(dp + j), dd0);
				}
			}

			for (; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (j = 0; (j < width) && ((mlib_addr)(dp + j) & 15); j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
		    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 7; j += 8) {
				MLIB_S_LDSI(load, 2, 8);
				sd0 = _mm_srai_epi16(sd0, 2);
				sd1 = _mm_srai_epi16(sd1, 2);
				sd2 = _mm_srai_epi16(sd2, 2);
				sd3 = _mm_srai_epi16(sd3, 2);
				st0 = _mm_add_epi16(sd0, sd1);
				st1 = _mm_add_epi16(sd2, sd3);
				st2 = _mm_madd_epi16(st0, one);
				st3 = _mm_madd_epi16(st1, one);
				dd0 = _mm_packs_epi32(st2, st3);
				ST_SI((void *)(dp + j), dd0);
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 7; j += 8) {
				MLIB_S_LDSI(loadu, 2, 8);
				sd0 = _mm_srai_epi16(sd0, 2);
				sd1 = _mm_srai_epi16(sd1, 2);
				sd2 = _mm_srai_epi16(sd2, 2);
				sd3 = _mm_srai_epi16(sd3, 2);
				st0 = _mm_add_epi16(sd0, sd1);
				st1 = _mm_add_epi16(sd2, sd3);
				st2 = _mm_madd_epi16(st0, one);
				st3 = _mm_madd_epi16(st1, one);
				dd0 = _mm_packs_epi32(st2, st3);
				ST_SI((void *)(dp + j), dd0);
			}
		}

		for (; j < width; j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#else	/* IMG_TYPE == 6 */

mlib_status ZOOM_FUNC(
    1_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5, st6, st7, st8, st9;
	const __m128i zero = _mm_setzero_si128();
	const __m128i one = _mm_set1_epi16(1);

/* averaging along X axis */
	if (dh) {
		for (i = 0; i < height; i++) {
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + 1]);
			}

			if (0 == ((mlib_addr)(sp + 2 * j) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 15; j += 16) {
				MLIB_S_LDSIP(load, 2, 16)
				st0 = _mm_unpacklo_epi8(sd0, zero);
				st1 = _mm_unpackhi_epi8(sd0, zero);
				st2 = _mm_unpacklo_epi8(sd1, zero);
				st3 = _mm_unpackhi_epi8(sd1, zero);
				st4 = _mm_madd_epi16(st0, one);
				st5 = _mm_madd_epi16(st1, one);
				st6 = _mm_madd_epi16(st2, one);
				st7 = _mm_madd_epi16(st3, one);
				st0 = _mm_srli_epi32(st4, 1);
				st1 = _mm_srli_epi32(st5, 1);
				st2 = _mm_srli_epi32(st6, 1);
				st3 = _mm_srli_epi32(st7, 1);
				st8 = _mm_packs_epi32(st0, st1);
				st9 = _mm_packs_epi32(st2, st3);
				dd0 = _mm_packus_epi16(st8, st9);
				ST_SI((void *)(dp + j), dd0);
			}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 15; j += 16) {
				MLIB_S_LDSIP(loadu, 2, 16)
				st0 = _mm_unpacklo_epi8(sd0, zero);
				st1 = _mm_unpackhi_epi8(sd0, zero);
				st2 = _mm_unpacklo_epi8(sd1, zero);
				st3 = _mm_unpackhi_epi8(sd1, zero);
				st4 = _mm_madd_epi16(st0, one);
				st5 = _mm_madd_epi16(st1, one);
				st6 = _mm_madd_epi16(st2, one);
				st7 = _mm_madd_epi16(st3, one);

				st0 = _mm_srli_epi32(st4, 1);
				st1 = _mm_srli_epi32(st5, 1);
				st2 = _mm_srli_epi32(st6, 1);
				st3 = _mm_srli_epi32(st7, 1);
				st8 = _mm_packs_epi32(st0, st1);
				st9 = _mm_packs_epi32(st2, st3);
				dd0 = _mm_packus_epi16(st8, st9);
				ST_SI((void *)(dp + j), dd0);
			}

			}

			for (; j < width; j++) {
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
			for (j = 0; (j < width) &&
			    ((mlib_addr)(dp + j) & 15); j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
			    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 15; j += 16) {
				MLIB_S_LDSI(load, 2, 16);
				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);
				st2 = _mm_unpacklo_epi8(st0, st1);
				st3 = _mm_unpackhi_epi8(st0, st1);
				st4 = _mm_unpacklo_epi8(st2, st3);
				st5 = _mm_unpackhi_epi8(st2, st3);
				st6 = _mm_unpacklo_epi8(st4, st5);
				st7 = _mm_unpackhi_epi8(st4, st5);
				dd0 = _mm_unpacklo_epi8(st6, st7);
				ST_SI((void *)(dp + j), dd0);
			}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 15; j += 16) {
				MLIB_S_LDSI(loadu, 2, 16);
				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);
				st2 = _mm_unpacklo_epi8(st0, st1);
				st3 = _mm_unpackhi_epi8(st0, st1);
				st4 = _mm_unpacklo_epi8(st2, st3);
				st5 = _mm_unpackhi_epi8(st2, st3);
				st6 = _mm_unpacklo_epi8(st4, st5);
				st7 = _mm_unpackhi_epi8(st4, st5);
				dd0 = _mm_unpacklo_epi8(st6, st7);
				ST_SI((void *)(dp + j), dd0);
			}

			}

			for (; j < width; j++) {
				dp[j] = AVE2(sp[2 * j], sp[2 * j + slb]);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		for (j = 0; (j < width) && ((mlib_addr)(dp + j) & 15); j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		if ((0 == ((mlib_addr)(sp + 2 * j) & 15)) &&
		    (0 == slb & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 15; j += 16) {
				MLIB_S_LDSI(load, 2, 16);
				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);
				st2 = _mm_unpacklo_epi8(st0, st1);
				st3 = _mm_unpackhi_epi8(st0, st1);
				st4 = _mm_unpacklo_epi8(st2, st3);
				st5 = _mm_unpackhi_epi8(st2, st3);
				st6 = _mm_unpacklo_epi8(st4, st5);
				st7 = _mm_unpackhi_epi8(st4, st5);
				st8 = _mm_unpacklo_epi8(st6, st7);
				st9 = _mm_unpackhi_epi8(st6, st7);
				dd0 = _mm_avg_epu8(st8, st9);
				ST_SI((void *)(dp + j), dd0);
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width - 15; j += 16) {
				MLIB_S_LDSI(loadu, 2, 16);
				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);
				st2 = _mm_unpacklo_epi8(st0, st1);
				st3 = _mm_unpackhi_epi8(st0, st1);
				st4 = _mm_unpacklo_epi8(st2, st3);
				st5 = _mm_unpackhi_epi8(st2, st3);
				st6 = _mm_unpacklo_epi8(st4, st5);
				st7 = _mm_unpackhi_epi8(st4, st5);
				st8 = _mm_unpacklo_epi8(st6, st7);
				st9 = _mm_unpackhi_epi8(st6, st7);
				dd0 = _mm_avg_epu8(st8, st9);
				ST_SI((void *)(dp + j), dd0);
			}
		}

		for (; j < width; j++) {
			dp[j] =
			    AVE4(sp[2 * j], sp[2 * j + 1], sp[2 * j + slb],
			    sp[2 * j + (slb + 1)]);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#endif	  /* IMG_TYPE == 6 */

/* *********************************************************** */

#if IMG_TYPE == 6

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128d sd0, sd1, sd2, sd3, st0, st1, st2, dd0;
	const __m128d ave2 = _mm_set1_pd(0.5);
	const __m128d ave4 = _mm_set1_pd(0.25);

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				sd0 = LD_PD(sp + 4 * j);
				sd1 = LD_PD(sp + 4 * j + 2);
				st0 = _mm_add_pd(sd0, sd1);
				dd0 = _mm_mul_pd(st0, ave2);
				STU_PD(dp + 2 * j, dd0);
			}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				sd0 = LDU_PD(sp + 4 * j);
				sd1 = LDU_PD(sp + 4 * j + 2);
				st0 = _mm_add_pd(sd0, sd1);
				dd0 = _mm_mul_pd(st0, ave2);
				STU_PD(dp + 2 * j, dd0);
			}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		for (i = 0; i < height; i++) {
			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				sd0 = LD_PD(sp + 4 * j);
				sd1 = LD_PD(sp + 4 * j + slb);
				st0 = _mm_add_pd(sd0, sd1);
				dd0 = _mm_mul_pd(st0, ave2);
				STU_PD(dp + 2 * j, dd0);
			}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				sd0 = LDU_PD(sp + 4 * j);
				sd1 = LDU_PD(sp + 4 * j + slb);
				st0 = _mm_add_pd(sd0, sd1);
				dd0 = _mm_mul_pd(st0, ave2);
				STU_PD(dp + 2 * j, dd0);
			}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			MLIB_S_LDPD(load, 4, 2)
			st0 = _mm_add_pd(sd0, sd2);
			st1 = _mm_add_pd(sd1, sd3);
			st2 = _mm_add_pd(st0, st1);
			dd0 = _mm_mul_pd(st2, ave4);
			STU_PD(dp + 2 * j, dd0);
		}

		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			MLIB_S_LDPD(loadu, 4, 2)
			st0 = _mm_add_pd(sd0, sd2);
			st1 = _mm_add_pd(sd1, sd3);
			st2 = _mm_add_pd(st0, st1);
			dd0 = _mm_mul_pd(st2, ave4);
			STU_PD(dp + 2 * j, dd0);
		}
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 5

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128 sd0, sd1, sd2, sd3;
	__m128 st0, st1, st2, st3, st4, st5, dd0;
	const __m128 ave2 = _mm_set1_ps(0.5);
	const __m128 ave4 = _mm_set1_ps(0.25);
	const __m128 zero = _mm_setzero_ps();

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				sd0 = _mm_load_ps(sp + 4 * j);
				sd1 = _mm_load_ps(sp + 4 * j + 4);
				st0 = _mm_movelh_ps(sd0, sd1);
				st1 = _mm_movehl_ps(sd1, sd0);
				st2 = _mm_add_ps(st0, st1);
				dd0 = _mm_mul_ps(st2, ave2);
				_mm_storeu_ps(dp + 2 * j, dd0);
			}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				sd0 = _mm_loadu_ps(sp + 4 * j);
				sd1 = _mm_loadu_ps(sp + 4 * j + 4);
				st0 = _mm_movelh_ps(sd0, sd1);
				st1 = _mm_movehl_ps(sd1, sd0);
				st2 = _mm_add_ps(st0, st1);
				dd0 = _mm_mul_ps(st2, ave2);
				_mm_storeu_ps(dp + 2 * j, dd0);
			}
			}

			if (j < width) {
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
			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDPS(load, 4, 4)
				st0 = _mm_add_ps(sd0, sd1);
				st1 = _mm_add_ps(sd2, sd3);
				st2 = _mm_movelh_ps(st0, st1);
				dd0 = _mm_mul_ps(st2, ave2);
				_mm_storeu_ps(dp + 2 * j, dd0);
			}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDPS(loadu, 4, 4)
				st0 = _mm_add_ps(sd0, sd1);
				st1 = _mm_add_ps(sd2, sd3);
				st2 = _mm_movelh_ps(st0, st1);
				dd0 = _mm_mul_ps(st2, ave2);
				_mm_storeu_ps(dp + 2 * j, dd0);
			}
			}

			if (j < width) {
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
		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_S_LDPS(load, 4, 4)
			st0 = _mm_add_ps(sd0, sd1);
			st1 = _mm_add_ps(sd2, sd3);
			st2 = _mm_movelh_ps(st0, st1);
			st3 = _mm_movehl_ps(st1, st0);
			st4 = _mm_add_ps(st2, st3);
			dd0 = _mm_mul_ps(st4, ave4);
			_mm_storeu_ps(dp + 2 * j, dd0);
		}

		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_S_LDPS(loadu, 4, 4)
			st0 = _mm_add_ps(sd0, sd1);
			st1 = _mm_add_ps(sd2, sd3);
			st2 = _mm_movelh_ps(st0, st1);
			st3 = _mm_movehl_ps(st1, st0);
			st4 = _mm_add_ps(st2, st3);
			dd0 = _mm_mul_ps(st4, ave4);
			_mm_storeu_ps(dp + 2 * j, dd0);
		}
		}

		if (j < width) {
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

#elif IMG_TYPE == 4

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3, st0, st1, st2, st3, st4, st5, dd0;

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSIP(load, 4, 4)
				st0 = _mm_srai_epi32(sd0, 1);
				st1 = _mm_srai_epi32(sd1, 1);
				st2 = _mm_unpacklo_epi64(st0, st1);
				st3 = _mm_unpackhi_epi64(st0, st1);
				dd0 = _mm_add_epi32(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSIP(loadu, 4, 4)
				st0 = _mm_srai_epi32(sd0, 1);
				st1 = _mm_srai_epi32(sd1, 1);
				st2 = _mm_unpacklo_epi64(st0, st1);
				st3 = _mm_unpackhi_epi64(st0, st1);
				dd0 = _mm_add_epi32(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}
			}

			if (j < width) {
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
			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSI(load, 4, 4);
				st0 = _mm_unpacklo_epi64(sd0, sd2);
				st1 = _mm_unpacklo_epi64(sd1, sd3);
				st2 = _mm_srai_epi32(st0, 1);
				st3 = _mm_srai_epi32(st1, 1);
				dd0 = _mm_add_epi32(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSI(loadu, 4, 4);
				st0 = _mm_unpacklo_epi64(sd0, sd2);
				st1 = _mm_unpacklo_epi64(sd1, sd3);
				st2 = _mm_srai_epi32(st0, 1);
				st3 = _mm_srai_epi32(st1, 1);
				dd0 = _mm_add_epi32(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}
			}

			if (j < width) {
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
		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_S_LDSI(load, 4, 4);
			st0 = _mm_srai_epi32(sd0, 2);
			st1 = _mm_srai_epi32(sd1, 2);
			st2 = _mm_srai_epi32(sd2, 2);
			st3 = _mm_srai_epi32(sd3, 2);
			st4 = _mm_add_epi32(st0, st1);
			st5 = _mm_add_epi32(st2, st3);
			st0 = _mm_unpacklo_epi64(st4, st5);
			st1 = _mm_unpackhi_epi64(st4, st5);
			dd0 = _mm_add_epi32(st0, st1);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 1; j += 2) {
			MLIB_S_LDSI(loadu, 4, 4);
			st0 = _mm_srai_epi32(sd0, 2);
			st1 = _mm_srai_epi32(sd1, 2);
			st2 = _mm_srai_epi32(sd2, 2);
			st3 = _mm_srai_epi32(sd3, 2);
			st4 = _mm_add_epi32(st0, st1);
			st5 = _mm_add_epi32(st2, st3);
			st0 = _mm_unpacklo_epi64(st4, st5);
			st1 = _mm_unpackhi_epi64(st4, st5);
			dd0 = _mm_add_epi32(st0, st1);
			STU_SI((void *)(dp + 2 * j), dd0);
		}
		}

		if (j < width) {
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

#elif IMG_TYPE == 3

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3;
	__m128i st0, st1, st2, st3, st4, st5, dd0;
	const __m128i zero = _mm_setzero_si128();

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSIP(load, 4, 8)
				st0 = _mm_unpacklo_epi32(sd0, sd1);
				st1 = _mm_unpackhi_epi32(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				dd0 = _mm_avg_epu16(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				sd0 = LD_SI((void *)(sp + 4 * j));
				st0 = _mm_unpacklo_epi32(sd0, zero);
				st1 = _mm_unpackhi_epi32(sd0, zero);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				dd0 = _mm_avg_epu16(st2, st3);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSIP(loadu, 4, 8)
				st0 = _mm_unpacklo_epi32(sd0, sd1);
				st1 = _mm_unpackhi_epi32(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				dd0 = _mm_avg_epu16(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				sd0 = LDU_SI((void *)(sp + 4 * j));
				st0 = _mm_unpacklo_epi32(sd0, zero);
				st1 = _mm_unpackhi_epi32(sd0, zero);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				dd0 = _mm_avg_epu16(st2, st3);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}
			}

			for (; j < width; j++) {
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
			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSI(load, 4, 8);

				st0 = _mm_avg_epu16(sd0, sd1);
				st1 = _mm_avg_epu16(sd2, sd3);

				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				dd0 = _mm_unpacklo_epi32(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				MLIB_S_LDSIP(load, 4, slb)
				st0 = _mm_avg_epu16(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, zero);
				st3 = _mm_unpackhi_epi32(st0, zero);
				dd0 = _mm_unpacklo_epi32(st2, st3);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSI(loadu, 4, 8);

				st0 = _mm_avg_epu16(sd0, sd1);
				st1 = _mm_avg_epu16(sd2, sd3);

				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				dd0 = _mm_unpacklo_epi32(st2, st3);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				MLIB_S_LDSIP(loadu, 4, slb)
				st0 = _mm_avg_epu16(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, zero);
				st3 = _mm_unpackhi_epi32(st0, zero);
				dd0 = _mm_unpacklo_epi32(st2, st3);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}
			}

			for (; j < width; j++) {
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
		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_S_LDSI(load, 4, 8);
			st0 = _mm_avg_epu16(sd0, sd1);
			st1 = _mm_avg_epu16(sd2, sd3);
			st2 = _mm_unpacklo_epi32(st0, st1);
			st3 = _mm_unpackhi_epi32(st0, st1);
			st4 = _mm_unpacklo_epi32(st2, st3);
			st5 = _mm_unpackhi_epi32(st2, st3);
			dd0 = _mm_avg_epu16(st4, st5);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		if (j < width - 1) {
			MLIB_S_LDSIP(load, 4, slb)
			st0 = _mm_avg_epu16(sd0, sd1);
			st2 = _mm_unpacklo_epi32(st0, zero);
			st3 = _mm_unpackhi_epi32(st0, zero);
			st4 = _mm_unpacklo_epi32(st2, st3);
			st5 = _mm_unpackhi_epi32(st2, st3);
			dd0 = _mm_avg_epu16(st4, st5);
			STL_SI((void *)(dp + 2 * j), dd0);
			j += 2;
		}

		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_S_LDSI(loadu, 4, 8);
			st0 = _mm_avg_epu16(sd0, sd1);
			st1 = _mm_avg_epu16(sd2, sd3);
			st2 = _mm_unpacklo_epi32(st0, st1);
			st3 = _mm_unpackhi_epi32(st0, st1);
			st4 = _mm_unpacklo_epi32(st2, st3);
			st5 = _mm_unpackhi_epi32(st2, st3);
			dd0 = _mm_avg_epu16(st4, st5);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		if (j < width - 1) {
			MLIB_S_LDSIP(loadu, 4, slb)
			st0 = _mm_avg_epu16(sd0, sd1);
			st2 = _mm_unpacklo_epi32(st0, zero);
			st3 = _mm_unpackhi_epi32(st0, zero);
			st4 = _mm_unpacklo_epi32(st2, st3);
			st5 = _mm_unpackhi_epi32(st2, st3);
			dd0 = _mm_avg_epu16(st4, st5);
			STL_SI((void *)(dp + 2 * j), dd0);
			j += 2;
		}
		}

		for (; j < width; j++) {
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

#elif IMG_TYPE == 2

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3;
	__m128i st0, st1, st2, st3, st4, st5, dd0;
	const __m128i zero = _mm_setzero_si128();

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSIP(load, 4, 8)
				st0 = _mm_unpacklo_epi32(sd0, sd1);
				st1 = _mm_unpackhi_epi32(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_srai_epi16(st2, 1);
				st5 = _mm_srai_epi16(st3, 1);
				dd0 = _mm_add_epi16(st4, st5);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				sd0 = LD_SI((void *)(sp + 4 * j));
				st0 = _mm_unpacklo_epi32(sd0, zero);
				st1 = _mm_unpackhi_epi32(sd0, zero);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_srai_epi16(st2, 1);
				st5 = _mm_srai_epi16(st3, 1);
				dd0 = _mm_add_epi16(st4, st5);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSIP(loadu, 4, 8)
				st0 = _mm_unpacklo_epi32(sd0, sd1);
				st1 = _mm_unpackhi_epi32(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_srai_epi16(st2, 1);
				st5 = _mm_srai_epi16(st3, 1);
				dd0 = _mm_add_epi16(st4, st5);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				sd0 = LDU_SI((void *)(sp + 4 * j));
				st0 = _mm_unpacklo_epi32(sd0, zero);
				st1 = _mm_unpackhi_epi32(sd0, zero);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_srai_epi16(st2, 1);
				st5 = _mm_srai_epi16(st3, 1);
				dd0 = _mm_add_epi16(st4, st5);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}
			}

			for (; j < width; j++) {
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
			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSI(load, 4, 8);
				st0 = _mm_srai_epi16(sd0, 1);
				st1 = _mm_srai_epi16(sd1, 1);
				st2 = _mm_srai_epi16(sd2, 1);
				st3 = _mm_srai_epi16(sd3, 1);

				st4 = _mm_add_epi16(st0, st1);
				st5 = _mm_add_epi16(st2, st3);

				st0 = _mm_unpacklo_epi32(st4, st5);
				st1 = _mm_unpackhi_epi32(st4, st5);
				dd0 = _mm_unpacklo_epi32(st0, st1);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				MLIB_S_LDSIP(load, 4, slb)
				sd2 = _mm_srai_epi16(sd0, 1);
				sd3 = _mm_srai_epi16(sd1, 1);

				st4 = _mm_add_epi16(sd2, sd3);

				st0 = _mm_unpacklo_epi32(st4, zero);
				st1 = _mm_unpackhi_epi32(st4, zero);
				dd0 = _mm_unpacklo_epi32(st0, st1);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSI(loadu, 4, 8);
				st0 = _mm_srai_epi16(sd0, 1);
				st1 = _mm_srai_epi16(sd1, 1);
				st2 = _mm_srai_epi16(sd2, 1);
				st3 = _mm_srai_epi16(sd3, 1);

				st4 = _mm_add_epi16(st0, st1);
				st5 = _mm_add_epi16(st2, st3);

				st0 = _mm_unpacklo_epi32(st4, st5);
				st1 = _mm_unpackhi_epi32(st4, st5);
				dd0 = _mm_unpacklo_epi32(st0, st1);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 1) {
				MLIB_S_LDSIP(loadu, 4, slb)
				sd2 = _mm_srai_epi16(sd0, 1);
				sd3 = _mm_srai_epi16(sd1, 1);
				st4 = _mm_add_epi16(sd2, sd3);

				st0 = _mm_unpacklo_epi32(st4, zero);
				st1 = _mm_unpackhi_epi32(st4, zero);
				dd0 = _mm_unpacklo_epi32(st0, st1);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 2;
			}
			}

			for (; j < width; j++) {
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
		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_S_LDSI(load, 4, 8);

			st0 = _mm_srai_epi16(sd0, 2);
			st1 = _mm_srai_epi16(sd1, 2);
			st2 = _mm_srai_epi16(sd2, 2);
			st3 = _mm_srai_epi16(sd3, 2);

			st4 = _mm_add_epi16(st0, st1);
			st5 = _mm_add_epi16(st2, st3);

			st0 = _mm_unpacklo_epi32(st4, st5);
			st1 = _mm_unpackhi_epi32(st4, st5);
			st2 = _mm_unpacklo_epi32(st0, st1);
			st3 = _mm_unpackhi_epi32(st0, st1);
			dd0 = _mm_add_epi16(st2, st3);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		if (j < width - 1) {
			MLIB_S_LDSIP(load, 4, slb)

			sd2 = _mm_srai_epi16(sd0, 2);
			sd3 = _mm_srai_epi16(sd1, 2);

			st4 = _mm_add_epi16(sd2, sd3);

			st0 = _mm_unpacklo_epi32(st4, zero);
			st1 = _mm_unpackhi_epi32(st4, zero);
			st2 = _mm_unpacklo_epi32(st0, st1);
			st3 = _mm_unpackhi_epi32(st0, st1);
			dd0 = _mm_add_epi16(st2, st3);
			STL_SI((void *)(dp + 2 * j), dd0);
			j += 2;
		}

		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 3; j += 4) {
			MLIB_S_LDSI(loadu, 4, 8);
			st0 = _mm_srai_epi16(sd0, 2);
			st1 = _mm_srai_epi16(sd1, 2);
			st2 = _mm_srai_epi16(sd2, 2);
			st3 = _mm_srai_epi16(sd3, 2);

			st4 = _mm_add_epi16(st0, st1);
			st5 = _mm_add_epi16(st2, st3);

			st0 = _mm_unpacklo_epi32(st4, st5);
			st1 = _mm_unpackhi_epi32(st4, st5);
			st2 = _mm_unpacklo_epi32(st0, st1);
			st3 = _mm_unpackhi_epi32(st0, st1);
			dd0 = _mm_add_epi16(st2, st3);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		if (j < width - 1) {
			MLIB_S_LDSIP(loadu, 4, slb)
			sd2 = _mm_srai_epi16(sd0, 2);
			sd3 = _mm_srai_epi16(sd1, 2);
			st4 = _mm_add_epi16(sd2, sd3);

			st0 = _mm_unpacklo_epi32(st4, zero);
			st1 = _mm_unpackhi_epi32(st4, zero);
			st2 = _mm_unpacklo_epi32(st0, st1);
			st3 = _mm_unpackhi_epi32(st0, st1);
			dd0 = _mm_add_epi16(st2, st3);
			STL_SI((void *)(dp + 2 * j), dd0);
			j += 2;
		}
		}

		for (; j < width; j++) {
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

#elif IMG_TYPE == 1

mlib_status ZOOM_FUNC(
    2_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3;
	__m128i st0, st1, st2, st3, st4, st5, dd0;
	const __m128i zero = _mm_setzero_si128();

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 7; j += 8) {
				MLIB_S_LDSIP(load, 4, 16)
				st0 = _mm_unpacklo_epi16(sd0, sd1);
				st1 = _mm_unpackhi_epi16(sd0, sd1);
				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 3) {
				sd0 = LD_SI((void *)(sp + 4 * j));
				st0 = _mm_unpacklo_epi16(sd0, zero);
				st1 = _mm_unpackhi_epi16(sd0, zero);
				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 4;
			}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 7; j += 8) {
				MLIB_S_LDSIP(loadu, 4, 16)
				st0 = _mm_unpacklo_epi16(sd0, sd1);
				st1 = _mm_unpackhi_epi16(sd0, sd1);
				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 3) {
				sd0 = LDU_SI((void *)(sp + 4 * j));
				st0 = _mm_unpacklo_epi16(sd0, zero);
				st1 = _mm_unpackhi_epi16(sd0, zero);
				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 4;
			}

			}

			for (; j < width; j++) {
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
			if ((0 == ((mlib_addr)sp & 15)) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 7; j += 8) {
				MLIB_S_LDSI(load, 4, 16);

				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);

				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_unpacklo_epi16(st4, st5);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 3) {
				MLIB_S_LDSIP(load, 4, slb)
				st0 = _mm_avg_epu8(sd0, sd1);
				st2 = _mm_unpacklo_epi16(st0, zero);
				st3 = _mm_unpackhi_epi16(st0, zero);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_unpacklo_epi16(st4, st5);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 4;
			}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 7; j += 8) {
				MLIB_S_LDSI(loadu, 4, 16);

				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);

				st2 = _mm_unpacklo_epi16(st0, st1);
				st3 = _mm_unpackhi_epi16(st0, st1);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_unpacklo_epi16(st4, st5);
				STU_SI((void *)(dp + 2 * j), dd0);
			}

			if (j < width - 3) {
				MLIB_S_LDSIP(loadu, 4, slb)
				st0 = _mm_avg_epu8(sd0, sd1);
				st2 = _mm_unpacklo_epi16(st0, zero);
				st3 = _mm_unpackhi_epi16(st0, zero);
				st4 = _mm_unpacklo_epi16(st2, st3);
				st5 = _mm_unpackhi_epi16(st2, st3);
				dd0 = _mm_unpacklo_epi16(st4, st5);
				STL_SI((void *)(dp + 2 * j), dd0);
				j += 4;
			}
			}

			for (; j < width; j++) {
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
		if ((0 == ((mlib_addr)sp & 15)) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 7; j += 8) {
			MLIB_S_LDSI(load, 4, 16);
			st0 = _mm_avg_epu8(sd0, sd1);
			st1 = _mm_avg_epu8(sd2, sd3);
			st2 = _mm_unpacklo_epi16(st0, st1);
			st3 = _mm_unpackhi_epi16(st0, st1);
			st4 = _mm_unpacklo_epi16(st2, st3);
			st5 = _mm_unpackhi_epi16(st2, st3);
			st0 = _mm_unpacklo_epi16(st4, st5);
			st1 = _mm_unpackhi_epi16(st4, st5);
			dd0 = _mm_avg_epu8(st0, st1);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		if (j < width - 3) {
			MLIB_S_LDSIP(load, 4, slb)
			st0 = _mm_avg_epu8(sd0, sd1);
			st2 = _mm_unpacklo_epi16(st0, zero);
			st3 = _mm_unpackhi_epi16(st0, zero);
			st4 = _mm_unpacklo_epi16(st2, st3);
			st5 = _mm_unpackhi_epi16(st2, st3);
			st0 = _mm_unpacklo_epi16(st4, st5);
			st1 = _mm_unpackhi_epi16(st4, st5);
			dd0 = _mm_avg_epu8(st0, st1);
			STL_SI((void *)(dp + 2 * j), dd0);
			j += 4;
		}

		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width - 7; j += 8) {
			MLIB_S_LDSI(loadu, 4, 16);
			st0 = _mm_avg_epu8(sd0, sd1);
			st1 = _mm_avg_epu8(sd2, sd3);
			st2 = _mm_unpacklo_epi16(st0, st1);
			st3 = _mm_unpackhi_epi16(st0, st1);
			st4 = _mm_unpacklo_epi16(st2, st3);
			st5 = _mm_unpackhi_epi16(st2, st3);
			st0 = _mm_unpacklo_epi16(st4, st5);
			st1 = _mm_unpackhi_epi16(st4, st5);
			dd0 = _mm_avg_epu8(st0, st1);
			STU_SI((void *)(dp + 2 * j), dd0);
		}

		if (j < width - 3) {
			MLIB_S_LDSIP(loadu, 4, slb)
			st0 = _mm_avg_epu8(sd0, sd1);
			st2 = _mm_unpacklo_epi16(st0, zero);
			st3 = _mm_unpackhi_epi16(st0, zero);
			st4 = _mm_unpacklo_epi16(st2, st3);
			st5 = _mm_unpackhi_epi16(st2, st3);
			st0 = _mm_unpacklo_epi16(st4, st5);
			st1 = _mm_unpackhi_epi16(st4, st5);
			dd0 = _mm_avg_epu8(st0, st1);
			STL_SI((void *)(dp + 2 * j), dd0);
			j += 4;
		}
		}

		for (; j < width; j++) {
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

#endif

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

#if IMG_TYPE == 6

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

/* averaging along X axis */
	if (dh) {
		__m128d sd0, sd1, sd2, sd3, st0, st1, dd0, dd1;
		const __m128d avg2 = _mm_set1_pd(0.5);

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)(sp + 8 * j) & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					sd0 = LD_PD(sp + 8 * j);
					sd1 = LD_PD(sp + 8 * j + 4);
					sd2 = LD_PD(sp + 8 * j + 2);
					sd3 = LD_PD(sp + 8 * j + 6);
					st0 = _mm_add_pd(sd0, sd1);
					st1 = _mm_add_pd(sd2, sd3);
					dd0 = _mm_mul_pd(st0, avg2);
					dd1 = _mm_mul_pd(st1, avg2);
					STU_PD(dp + 4 * j, dd0);
					STU_PD(dp + 4 * j + 2, dd1);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					sd0 = LDU_PD(sp + 8 * j);
					sd1 = LDU_PD(sp + 8 * j + 4);
					sd2 = LDU_PD(sp + 8 * j + 2);
					sd3 = LDU_PD(sp + 8 * j + 6);
					st0 = _mm_add_pd(sd0, sd1);
					st1 = _mm_add_pd(sd2, sd3);
					dd0 = _mm_mul_pd(st0, avg2);
					dd1 = _mm_mul_pd(st1, avg2);
					STU_PD(dp + 4 * j, dd0);
					STU_PD(dp + 4 * j + 2, dd1);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		__m128d sd0, sd1, sd2, sd3, st0, st1, dd0, dd1;
		const __m128d avg2 = _mm_set1_pd(0.5);

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)(sp + 8 * j) & 15) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					MLIB_S_LDPD(load, 8, 2)
					st0 = _mm_add_pd(sd0, sd1);
					st1 = _mm_add_pd(sd2, sd3);
					dd0 = _mm_mul_pd(st0, avg2);
					dd1 = _mm_mul_pd(st1, avg2);
					STU_PD(dp + 4 * j, dd0);
					STU_PD(dp + 4 * j + 2, dd1);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					MLIB_S_LDPD(loadu, 8, 2)
					st0 = _mm_add_pd(sd0, sd1);
					st1 = _mm_add_pd(sd2, sd3);
					dd0 = _mm_mul_pd(st0, avg2);
					dd1 = _mm_mul_pd(st1, avg2);
					STU_PD(dp + 4 * j, dd0);
					STU_PD(dp + 4 * j + 2, dd1);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

	__m128d sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7;
	__m128d st0, st1, st2, st3, st4;
	const __m128d avg4 = _mm_set1_pd(0.25);

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if (0 == ((mlib_addr)(sp + 8 * j) & 15) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				sd0 = LD_PD(sp + 8 * j);
				sd1 = LD_PD(sp + 8 * j + slb);
				sd2 = LD_PD(sp + 8 * j +  4);
				sd3 = LD_PD(sp + 8 * j + slb + 4);
				sd4 = LD_PD(sp + 8 * j + 2);
				sd5 = LD_PD(sp + 8 * j + slb + 2);
				sd6 = LD_PD(sp + 8 * j +  6);
				sd7 = LD_PD(sp + 8 * j + slb + 6);
				st0 = _mm_add_pd(sd0, sd1);
				st1 = _mm_add_pd(sd2, sd3);
				st2 = _mm_add_pd(sd4, sd5);
				st3 = _mm_add_pd(sd6, sd7);

				sd0 = _mm_add_pd(st0, st1);
				sd1 = _mm_add_pd(st2, st3);
				sd2 = _mm_mul_pd(sd0, avg4);
				sd3 = _mm_mul_pd(sd1, avg4);
				STU_PD(dp + 4 * j, sd2);
				STU_PD(dp + 4 * j + 2, sd3);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_S_LDPD(loadu, 8, 4);
				sd4 = LDU_PD(sp + 8 * j + 2);
				sd5 = LDU_PD(sp + 8 * j + slb + 2);
				sd6 = LDU_PD(sp + 8 * j +  6);
				sd7 = LDU_PD(sp + 8 * j + slb + 6);
				st0 = _mm_add_pd(sd0, sd1);
				st1 = _mm_add_pd(sd2, sd3);
				st2 = _mm_add_pd(sd4, sd5);
				st3 = _mm_add_pd(sd6, sd7);

				sd0 = _mm_add_pd(st0, st1);
				sd1 = _mm_add_pd(st2, st3);
				sd2 = _mm_mul_pd(sd0, avg4);
				sd3 = _mm_mul_pd(sd1, avg4);
				STU_PD(dp + 4 * j, sd2);
				STU_PD(dp + 4 * j + 2, sd3);
			}
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 5

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

/* averaging along X axis */
	if (dh) {
		__m128 sd0, sd1, sd2, sd3, st0, st1, dd0, dd1;
		const __m128 avg2 = _mm_set1_ps(0.5);

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)(sp + 8 * j) & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					sd0 = _mm_load_ps(sp + 8 * j);
					sd1 = _mm_load_ps(sp + 8 * j + 4);
					st0 = _mm_add_ps(sd0, sd1);
					dd0 = _mm_mul_ps(st0, avg2);
					_mm_storeu_ps(dp + 4 * j, dd0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					sd0 = _mm_loadu_ps(sp + 8 * j);
					sd1 = _mm_loadu_ps(sp + 8 * j + 4);
					st0 = _mm_add_ps(sd0, sd1);
					dd0 = _mm_mul_ps(st0, avg2);
					_mm_storeu_ps(dp + 4 * j, dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		__m128 sd0, sd1, st0, dd0;
		const __m128 avg2 = _mm_set1_ps(0.5);

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)(sp + 8 * j) & 15) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					sd0 = _mm_load_ps(sp + 8 * j);
					sd1 = _mm_load_ps(sp + 8 * j + slb);
					st0 = _mm_add_ps(sd0, sd1);
					dd0 = _mm_mul_ps(st0, avg2);
					_mm_storeu_ps(dp + 4 * j, dd0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					sd0 = _mm_loadu_ps(sp + 8 * j);
					sd1 = _mm_loadu_ps(sp + 8 * j + slb);
					st0 = _mm_add_ps(sd0, sd1);
					dd0 = _mm_mul_ps(st0, avg2);
					_mm_storeu_ps(dp + 4 * j, dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

	__m128 sd0, sd1, sd2, sd3, st0, st1, st2, dd0;
	const __m128 avg4 = _mm_set1_ps(0.25);

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if (0 == ((mlib_addr)(sp + 8 * j) & 15) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_S_LDPS(load, 8, 4)
				st0 = _mm_add_ps(sd0, sd1);
				st1 = _mm_add_ps(sd2, sd3);
				st2 = _mm_add_ps(st0, st1);
				dd0 = _mm_mul_ps(st2, avg4);
				_mm_storeu_ps(dp + 4 * j, dd0);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_S_LDPS(loadu, 8, 4);
				st0 = _mm_add_ps(sd0, sd1);
				st1 = _mm_add_ps(sd2, sd3);
				st2 = _mm_add_ps(st0, st1);
				dd0 = _mm_mul_ps(st2, avg4);
				_mm_storeu_ps(dp + 4 * j, dd0);
			}
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 4

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;

/* averaging along X axis */
	if (dh) {
		__m128i sd0, sd1, st0, st1, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)(sp + 8 * j) & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					MLIB_S_LDSIP(load, 8, 4)
					st0 = _mm_srai_epi32(sd0, 1);
					st1 = _mm_srai_epi32(sd1, 1);
					dd0 = _mm_add_epi32(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					MLIB_S_LDSIP(loadu, 8, 4)
					st0 = _mm_srai_epi32(sd0, 1);
					st1 = _mm_srai_epi32(sd1, 1);
					dd0 = _mm_add_epi32(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		__m128i sd0, sd1, st0, st1, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)(sp + 8 * j) & 15) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					MLIB_S_LDSIP(load, 8, slb)
					st0 = _mm_srai_epi32(sd0, 1);
					st1 = _mm_srai_epi32(sd1, 1);
					dd0 = _mm_add_epi32(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width; j++) {
					MLIB_S_LDSIP(loadu, 8, slb)
					st0 = _mm_srai_epi32(sd0, 1);
					st1 = _mm_srai_epi32(sd1, 1);
					dd0 = _mm_add_epi32(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5;

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if (0 == ((mlib_addr)(sp + 8 * j) & 15) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_S_LDSI(load, 8, 4);
				st0 = _mm_srai_epi32(sd0, 2);
				st1 = _mm_srai_epi32(sd1, 2);
				st2 = _mm_srai_epi32(sd2, 2);
				st3 = _mm_srai_epi32(sd3, 2);
				st4 = _mm_add_epi32(st0, st1);
				st5 = _mm_add_epi32(st2, st3);
				dd0 = _mm_add_epi32(st4, st5);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {
				MLIB_S_LDSI(loadu, 8, 4);
				st0 = _mm_srai_epi32(sd0, 2);
				st1 = _mm_srai_epi32(sd1, 2);
				st2 = _mm_srai_epi32(sd2, 2);
				st3 = _mm_srai_epi32(sd3, 2);
				st4 = _mm_add_epi32(st0, st1);
				st5 = _mm_add_epi32(st2, st3);
				dd0 = _mm_add_epi32(st4, st5);
				STU_SI((void *)(dp + 4 * j), dd0);
			}
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 3

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	const __m128i zero = _mm_setzero_si128();

/* averaging along X axis */
	if (dh) {
		__m128i sd0, sd1, st0, st1, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSIP(load, 8, 8)
					st0 = _mm_unpacklo_epi64(sd0, sd1);
					st1 = _mm_unpackhi_epi64(sd0, sd1);
					dd0 = _mm_avg_epu16(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					sd0 = LD_SI((void *)(sp + 8 * j));
					st0 = _mm_unpacklo_epi64(sd0, zero);
					st1 = _mm_unpackhi_epi64(sd0, zero);
					dd0 = _mm_avg_epu16(st0, st1);
					STL_SI((void *)(dp + 4 * j), dd0);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSIP(loadu, 8, 8)
					st0 = _mm_unpacklo_epi64(sd0, sd1);
					st1 = _mm_unpackhi_epi64(sd0, sd1);
					dd0 = _mm_avg_epu16(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					sd0 = LDU_SI((void *)(sp + 8 * j));
					st0 = _mm_unpacklo_epi64(sd0, zero);
					st1 = _mm_unpackhi_epi64(sd0, zero);
					dd0 = _mm_avg_epu16(st0, st1);
					STL_SI((void *)(dp + 4 * j), dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		__m128i sd0, sd1, sd2, sd3, st0, st1, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSI(load, 8, 8);
					st0 = _mm_avg_epu16(sd0, sd1);
					st1 = _mm_avg_epu16(sd2, sd3);
					dd0 = _mm_unpacklo_epi64(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					MLIB_S_LDSIP(load, 8, slb)
					dd0 = _mm_avg_epu16(sd0, sd1);
					STL_SI((void *)(dp + 4 * j), dd0);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSI(loadu, 8, 8);
					st0 = _mm_avg_epu16(sd0, sd1);
					st1 = _mm_avg_epu16(sd2, sd3);
					dd0 = _mm_unpacklo_epi64(st0, st1);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					MLIB_S_LDSIP(loadu, 8, slb)
					dd0 = _mm_avg_epu16(sd0, sd1);
					STL_SI((void *)(dp + 4 * j), dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5;

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if (0 == ((mlib_addr)sp & 15) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSI(load, 8, 8);
				st0 = _mm_avg_epu16(sd0, sd1);
				st1 = _mm_avg_epu16(sd2, sd3);
				st2 = _mm_unpacklo_epi64(st0, st1);
				st3 = _mm_unpackhi_epi64(st0, st1);
				dd0 = _mm_avg_epu16(st2, st3);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

			if (j < width) {
				MLIB_S_LDSIP(load, 8, slb)
				st0 = _mm_avg_epu16(sd0, sd1);
				st2 = _mm_unpacklo_epi64(st0, zero);
				st3 = _mm_unpackhi_epi64(st0, zero);
				dd0 = _mm_avg_epu16(st2, st3);
				STL_SI((void *)(dp + 4 * j), dd0);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSI(loadu, 8, 8);
				st0 = _mm_avg_epu16(sd0, sd1);
				st1 = _mm_avg_epu16(sd2, sd3);
				st2 = _mm_unpacklo_epi64(st0, st1);
				st3 = _mm_unpackhi_epi64(st0, st1);
				dd0 = _mm_avg_epu16(st2, st3);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

			if (j < width) {
				MLIB_S_LDSIP(loadu, 8, slb)
				st0 = _mm_avg_epu16(sd0, sd1);
				st2 = _mm_unpacklo_epi64(st0, zero);
				st3 = _mm_unpackhi_epi64(st0, zero);
				dd0 = _mm_avg_epu16(st2, st3);
				STL_SI((void *)(dp + 4 * j), dd0);
			}
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 2

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	const __m128i zero = _mm_setzero_si128();

/* averaging along X axis */
	if (dh) {
		__m128i sd0, sd1, st0, st1, st2, st3, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSIP(load, 8, 8)
					st0 = _mm_unpacklo_epi64(sd0, sd1);
					st1 = _mm_unpackhi_epi64(sd0, sd1);
					st2 = _mm_srai_epi16(st0, 1);
					st3 = _mm_srai_epi16(st1, 1);
					dd0 = _mm_add_epi16(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					sd0 = LD_SI((void *)(sp + 8 * j));
					st0 = _mm_unpacklo_epi64(sd0, zero);
					st1 = _mm_unpackhi_epi64(sd0, zero);
					st2 = _mm_srai_epi16(st0, 1);
					st3 = _mm_srai_epi16(st1, 1);
					dd0 = _mm_add_epi16(st2, st3);
					STL_SI((void *)(dp + 4 * j), dd0);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSIP(loadu, 8, 8)
					st0 = _mm_unpacklo_epi64(sd0, sd1);
					st1 = _mm_unpackhi_epi64(sd0, sd1);
					st2 = _mm_srai_epi16(st0, 1);
					st3 = _mm_srai_epi16(st1, 1);
					dd0 = _mm_add_epi16(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					sd0 = LDU_SI((void *)(sp + 8 * j));
					st0 = _mm_unpacklo_epi64(sd0, zero);
					st1 = _mm_unpackhi_epi64(sd0, zero);
					st2 = _mm_srai_epi16(st0, 1);
					st3 = _mm_srai_epi16(st1, 1);
					dd0 = _mm_add_epi16(st2, st3);
					STL_SI((void *)(dp + 4 * j), dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		__m128i sd0, sd1, sd2, sd3;
		__m128i st0, st1, st2, st3, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSI(load, 8, 8);
					st0 = _mm_unpacklo_epi64(sd0, sd2);
					st1 = _mm_unpacklo_epi64(sd1, sd3);
					st2 = _mm_srai_epi16(st0, 1);
					st3 = _mm_srai_epi16(st1, 1);
					dd0 = _mm_add_epi16(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					MLIB_S_LDSIP(load, 8, slb)
					st0 = _mm_srai_epi16(sd0, 1);
					st1 = _mm_srai_epi16(sd1, 1);
					dd0 = _mm_add_epi16(st0, st1);
					STL_SI((void *)(dp + 4 * j), dd0);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 1; j += 2) {
					MLIB_S_LDSI(loadu, 8, 8);
					st0 = _mm_unpacklo_epi64(sd0, sd2);
					st1 = _mm_unpacklo_epi64(sd1, sd3);
					st2 = _mm_srai_epi16(st0, 1);
					st3 = _mm_srai_epi16(st1, 1);
					dd0 = _mm_add_epi16(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width) {
					MLIB_S_LDSIP(loadu, 8, slb)
					st0 = _mm_srai_epi16(sd0, 1);
					st1 = _mm_srai_epi16(sd1, 1);
					dd0 = _mm_add_epi16(st0, st1);
					STL_SI((void *)(dp + 4 * j), dd0);
				}
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i si0, si1, si2, si3;
	__m128i sd0_l, sd1_l, sd2_l, sd3_l;
	__m128i sd0_h, sd1_h, sd2_h, sd3_h;
	__m128i st0_l, st1_l, st2_l, st3_l, st4_l;
	__m128i st0_h, st1_h, st2_h, st3_h, st4_h;

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if (0 == ((mlib_addr)sp & 15) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSI(load, 8, 8);
				si0 = _mm_srai_epi16(sd0, 15);
				si1 = _mm_srai_epi16(sd1, 15);
				si2 = _mm_srai_epi16(sd2, 15);
				si3 = _mm_srai_epi16(sd3, 15);

				sd0_l = _mm_unpacklo_epi16(sd0, si0);
				sd0_h = _mm_unpackhi_epi16(sd0, si0);
				sd1_l = _mm_unpacklo_epi16(sd1, si1);
				sd1_h = _mm_unpackhi_epi16(sd1, si1);
				sd2_l = _mm_unpacklo_epi16(sd2, si2);
				sd2_h = _mm_unpackhi_epi16(sd2, si2);
				sd3_l = _mm_unpacklo_epi16(sd3, si3);
				sd3_h = _mm_unpackhi_epi16(sd3, si3);

				st0_l = _mm_add_epi32(sd0_l, sd1_l);
				st0_h = _mm_add_epi32(sd0_h, sd1_h);
				st1_l = _mm_add_epi32(sd2_l, sd3_l);
				st1_h = _mm_add_epi32(sd2_h, sd3_h);
				st2_l = _mm_add_epi32(st0_l, st0_h);
				st2_h = _mm_add_epi32(st1_l, st1_h);

				st0_l = _mm_srai_epi32(st2_l, 2);
				st0_h = _mm_srai_epi32(st2_h, 2);
				dd0 = _mm_packs_epi32(st0_l, st0_h);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

			if (j < width) {
				MLIB_S_LDSIP(load, 8, slb)
				si0 = _mm_srai_epi16(sd0, 15);
				si1 = _mm_srai_epi16(sd1, 15);

				sd0_l = _mm_unpacklo_epi16(sd0, si0);
				sd0_h = _mm_unpackhi_epi16(sd0, si0);
				sd1_l = _mm_unpacklo_epi16(sd1, si1);
				sd1_h = _mm_unpackhi_epi16(sd1, si1);

				st0_l = _mm_add_epi32(sd0_l, sd1_l);
				st0_h = _mm_add_epi32(sd0_h, sd1_h);
				st2_l = _mm_add_epi32(st0_l, st0_h);

				st0_l = _mm_srai_epi32(st2_l, 2);
				dd0 = _mm_packs_epi32(st0_l, st0_l);

				STL_SI((void *)(dp + 4 * j), dd0);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j += 2) {
				MLIB_S_LDSI(loadu, 8, 8);
				si0 = _mm_srai_epi16(sd0, 15);
				si1 = _mm_srai_epi16(sd1, 15);
				si2 = _mm_srai_epi16(sd2, 15);
				si3 = _mm_srai_epi16(sd3, 15);

				sd0_l = _mm_unpacklo_epi16(sd0, si0);
				sd0_h = _mm_unpackhi_epi16(sd0, si0);
				sd1_l = _mm_unpacklo_epi16(sd1, si1);
				sd1_h = _mm_unpackhi_epi16(sd1, si1);
				sd2_l = _mm_unpacklo_epi16(sd2, si2);
				sd2_h = _mm_unpackhi_epi16(sd2, si2);
				sd3_l = _mm_unpacklo_epi16(sd3, si3);
				sd3_h = _mm_unpackhi_epi16(sd3, si3);

				st0_l = _mm_add_epi32(sd0_l, sd1_l);
				st0_h = _mm_add_epi32(sd0_h, sd1_h);
				st1_l = _mm_add_epi32(sd2_l, sd3_l);
				st1_h = _mm_add_epi32(sd2_h, sd3_h);
				st2_l = _mm_add_epi32(st0_l, st0_h);
				st2_h = _mm_add_epi32(st1_l, st1_h);

				st0_l = _mm_srai_epi32(st2_l, 2);
				st0_h = _mm_srai_epi32(st2_h, 2);
				dd0 = _mm_packs_epi32(st0_l, st0_h);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

			if (j < width) {
				MLIB_S_LDSIP(loadu, 8, slb)
				si0 = _mm_srai_epi16(sd0, 15);
				si1 = _mm_srai_epi16(sd1, 15);

				sd0_l = _mm_unpacklo_epi16(sd0, si0);
				sd0_h = _mm_unpackhi_epi16(sd0, si0);
				sd1_l = _mm_unpacklo_epi16(sd1, si1);
				sd1_h = _mm_unpackhi_epi16(sd1, si1);

				st0_l = _mm_add_epi32(sd0_l, sd1_l);
				st0_h = _mm_add_epi32(sd0_h, sd1_h);
				st2_l = _mm_add_epi32(st0_l, st0_h);

				st0_l = _mm_srai_epi32(st2_l, 2);
				dd0 = _mm_packs_epi32(st0_l, st0_l);

				STL_SI((void *)(dp + 4 * j), dd0);
			}
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#elif IMG_TYPE == 1

mlib_status ZOOM_FUNC(
    4_Bilinear) (
    BL_PARAMETRS(STYPE))
{
	mlib_s32 i, j;
	const __m128i zero = _mm_setzero_si128();

/* averaging along X axis */
	if (dh) {
		__m128i sd0, sd1, st0, st1, st2, st3, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 3; j += 4) {
					MLIB_S_LDSIP(load, 8, 16)
					st0 = _mm_unpacklo_epi32(sd0, sd1);
					st1 = _mm_unpackhi_epi32(sd0, sd1);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_avg_epu8(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width - 1) {
					sd0 = LD_SI((void *)(sp + 8 * j));
					st0 = _mm_unpacklo_epi32(sd0, zero);
					st1 = _mm_unpackhi_epi32(sd0, zero);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_avg_epu8(st2, st3);
					STL_SI((void *)(dp + 4 * j), dd0);
					j += 2;
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 3; j += 4) {
					MLIB_S_LDSIP(loadu, 8, 16)
					st0 = _mm_unpacklo_epi32(sd0, sd1);
					st1 = _mm_unpackhi_epi32(sd0, sd1);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_avg_epu8(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width - 1) {
					sd0 = LDU_SI((void *)(sp + 8 * j));
					st0 = _mm_unpacklo_epi32(sd0, zero);
					st1 = _mm_unpackhi_epi32(sd0, zero);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_avg_epu8(st2, st3);
					STL_SI((void *)(dp + 4 * j), dd0);
					j += 2;
				}
			}

			if (j < width) {
				sd0 = LDL_SI((void *)(sp + 8 * j));
				st0 = _mm_unpacklo_epi32(sd0, zero);
				st2 = _mm_unpacklo_epi64(st0, zero);
				st3 = _mm_unpackhi_epi64(st0, zero);
				dd0 = _mm_avg_epu8(st2, st3);
				*((mlib_s32 *)(dp + 4 * j)) =
				    _mm_cvtsi128_si32(dd0);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

/* averaging along Y axis */
	if (dw) {
		__m128i sd0, sd1, sd2, sd3, st0, st1, st2, st3, dd0;

		for (i = 0; i < height; i++) {
			if (0 == ((mlib_addr)sp & 15) &&
			    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 3; j += 4) {
					MLIB_S_LDSI(load, 8, 16);
					st0 = _mm_avg_epu8(sd0, sd1);
					st1 = _mm_avg_epu8(sd2, sd3);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_unpacklo_epi32(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width - 1) {
					MLIB_S_LDSIP(load, 8, slb)
					st0 = _mm_avg_epu8(sd0, sd1);
					st2 = _mm_unpacklo_epi32(st0, zero);
					st3 = _mm_unpackhi_epi32(st0, zero);
					dd0 = _mm_unpacklo_epi32(st2, st3);
					STL_SI((void *)(dp + 4 * j), dd0);
					j += 2;
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < width - 3; j += 4) {
					MLIB_S_LDSI(loadu, 8, 16);
					st0 = _mm_avg_epu8(sd0, sd1);
					st1 = _mm_avg_epu8(sd2, sd3);
					st2 = _mm_unpacklo_epi32(st0, st1);
					st3 = _mm_unpackhi_epi32(st0, st1);
					dd0 = _mm_unpacklo_epi32(st2, st3);
					STU_SI((void *)(dp + 4 * j), dd0);
				}

				if (j < width - 1) {
					MLIB_S_LDSIP(loadu, 8, slb)
					st0 = _mm_avg_epu8(sd0, sd1);
					st2 = _mm_unpacklo_epi32(st0, zero);
					st3 = _mm_unpackhi_epi32(st0, zero);
					dd0 = _mm_unpacklo_epi32(st2, st3);
					STL_SI((void *)(dp + 4 * j), dd0);
					j += 2;
				}
			}

			if (j < width) {
				sd0 =
				    LDL_SI((void *)(sp + 8 * j));
				sd1 =
				    LDL_SI((void *)(sp + 8 * j + slb));
				dd0 = _mm_avg_epu8(sd0, sd1);
				*((mlib_s32 *)(dp + 4 * j)) =
				    _mm_cvtsi128_si32(dd0);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return (MLIB_SUCCESS);
	}

	__m128i sd0, sd1, sd2, sd3, dd0;
	__m128i st0, st1, st2, st3, st4, st5;

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		if (0 == ((mlib_addr)sp & 15) &&
		    (0 == (slb & 15))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSI(load, 8, 16);
				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_unpacklo_epi32(st2, st3);
				st5 = _mm_unpackhi_epi32(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

			if (j < width - 1) {
				MLIB_S_LDSIP(load, 8, slb)
				st0 = _mm_avg_epu8(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, zero);
				st3 = _mm_unpackhi_epi32(st0, zero);
				st4 = _mm_unpacklo_epi32(st2, st3);
				st5 = _mm_unpackhi_epi32(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STL_SI((void *)(dp + 4 * j), dd0);
				j += 2;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 3; j += 4) {
				MLIB_S_LDSI(loadu, 8, 16);
				st0 = _mm_avg_epu8(sd0, sd1);
				st1 = _mm_avg_epu8(sd2, sd3);
				st2 = _mm_unpacklo_epi32(st0, st1);
				st3 = _mm_unpackhi_epi32(st0, st1);
				st4 = _mm_unpacklo_epi32(st2, st3);
				st5 = _mm_unpackhi_epi32(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STU_SI((void *)(dp + 4 * j), dd0);
			}

			if (j < width - 1) {
				MLIB_S_LDSIP(loadu, 8, slb)
				st0 = _mm_avg_epu8(sd0, sd1);
				st2 = _mm_unpacklo_epi32(st0, zero);
				st3 = _mm_unpackhi_epi32(st0, zero);
				st4 = _mm_unpacklo_epi32(st2, st3);
				st5 = _mm_unpackhi_epi32(st2, st3);
				dd0 = _mm_avg_epu8(st4, st5);
				STL_SI((void *)(dp + 4 * j), dd0);
				j += 2;
			}
		}

		if (j < width) {
			sd0 = LDL_SI((void *)(sp + 8 * j));
			sd1 = LDL_SI((void *)(sp + 8 * j + slb));
			st0 = _mm_avg_epu8(sd0, sd1);
			st1 = _mm_unpacklo_epi32(st0, zero);
			st2 = _mm_unpacklo_epi64(st1, zero);
			st3 = _mm_unpackhi_epi64(st1, zero);
			dd0 = _mm_avg_epu8(st2, st3);
			*((mlib_s32 *)(dp + 4 * j)) = _mm_cvtsi128_si32(dd0);
		}

		sp += 2 * slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

#endif

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEZOOMOUT2X_BL_H */
