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

#pragma ident	"@(#)mlib_c_VideoColorAVDBlendUtil.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *      [ZERO, ONE, DA, SA, OMDA, OMSA]
 *      blend two images with blend factors
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *           [ZERO, ONE, DA, SA, OMDA, OMSA]
 *                                          (mlib_u32       * dst,
 *                                           const mlib_u32 * src1,
 *                                           const mlib_u32 * src2,
 *                                           mlib_s32       w,
 *                                           mlib_s32       h,
 *                                           mlib_s32       dlb,
 *                                           mlib_s32       slb1,
 *                                           mlib_s32       slb2)
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      w       image width
 *      h       image height
 *      dlb     linebytes for output image
 *      slb1    linebytes for 1st input image
 *      slb2    linebytes for 2nd input image
 *
 * DESCRIPTION
 *        dst = (src1 * src1_blend) + (src2 * src2_blend)
 */

#include <mlib_algebra.h>
#include <mlib_VideoColorAVDBlend_proto.h>
#include <mlib_c_ImageBlendTable.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	BLEND_VALIDATE                                          \
	mlib_u8 *psrc1, *psrc2, *pdst;                          \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc1 = (mlib_u8 *)src1;                                \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

#define	BLEND_VALIDATE_1                                        \
	mlib_u8 *psrc2, *pdst;                                  \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

void
mlib_VideoColorCopy(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	if (dlb == (4 * w) && slb == dlb) {
		w *= h;
		h = 1;
	}

	for (i = 0; i < h; i++) {
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src, w);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy2(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 istop,
	mlib_s32 istart,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src, istop);
		__mlib_VectorCopy_S32((mlib_s32 *)dst + istart,
			(mlib_s32 *)src + istart, w - istart);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2, rez;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
			alpha2 = mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

#ifdef MLIB_USE_FTOI_CLAMPING
			pdst[4 * i + 0] =
				(mlib_s32)(src10 * alpha1 +
				src20 * alpha2) >> 23;
			pdst[4 * i + 1] =
				(mlib_s32)(src11 * alpha1 +
				src21 * alpha2) >> 23;
			pdst[4 * i + 2] =
				(mlib_s32)(src12 * alpha1 +
				src22 * alpha2) >> 23;
			pdst[4 * i + 3] =
				(mlib_s32)(src13 * alpha1 +
				src23 * alpha2) >> 23;
#else /* MLIB_USE_FTOI_CLAMPING */
			rez = src10 * alpha1 + src20 * alpha2;
			if (rez >= MLIB_S32_MAX)
				pdst[4 * i + 0] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 0] = (mlib_s32)rez >> 23;

			rez = src11 * alpha1 + src21 * alpha2;
			if (rez >= MLIB_S32_MAX)
				pdst[4 * i + 1] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 1] = (mlib_s32)rez >> 23;

			rez = src12 * alpha1 + src22 * alpha2;
			if (rez >= MLIB_S32_MAX)
				pdst[4 * i + 2] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 2] = (mlib_s32)rez >> 23;

			rez = src13 * alpha1 + src23 * alpha2;
			if (rez >= MLIB_S32_MAX)
				pdst[4 * i + 3] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 3] = (mlib_s32)rez >> 23;
#endif /* MLIB_USE_FTOI_CLAMPING */
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
			alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

			pdst[4 * i + 0] =
				FLOAT2INT_CLAMP(src10 * alpha1 +
				src20 * alpha2) >> 23;
			pdst[4 * i + 1] =
				FLOAT2INT_CLAMP(src11 * alpha1 +
				src21 * alpha2) >> 23;
			pdst[4 * i + 2] =
				FLOAT2INT_CLAMP(src12 * alpha1 +
				src22 * alpha2) >> 23;
			pdst[4 * i + 3] =
				FLOAT2INT_CLAMP(src13 * alpha1 +
				src23 * alpha2) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 src10, src20;
	mlib_f32 src11, src21;
	mlib_f32 src12, src22;
	mlib_f32 src13, src23;
	mlib_f32 alpha1, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
			alpha2 = mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

			pdst[4 * i + 0] =
				FLOAT2INT_CLAMP(src10 * alpha1 +
				src20 * alpha2) >> 23;
			pdst[4 * i + 1] =
				FLOAT2INT_CLAMP(src11 * alpha1 +
				src21 * alpha2) >> 23;
			pdst[4 * i + 2] =
				FLOAT2INT_CLAMP(src12 * alpha1 +
				src22 * alpha2) >> 23;
			pdst[4 * i + 3] =
				FLOAT2INT_CLAMP(src13 * alpha1 +
				src23 * alpha2) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 src10, src20;
	mlib_f32 src11, src21;
	mlib_f32 src12, src22;
	mlib_f32 src13, src23;
	mlib_f32 alpha1, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
			alpha2 = mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

			pdst[4 * i + 0] =
				FLOAT2INT_CLAMP(src10 * alpha2 +
				src20 * alpha1) >> 23;
			pdst[4 * i + 1] =
				FLOAT2INT_CLAMP(src11 * alpha2 +
				src21 * alpha1) >> 23;
			pdst[4 * i + 2] =
				FLOAT2INT_CLAMP(src12 * alpha2 +
				src22 * alpha1) >> 23;
			pdst[4 * i + 3] =
				FLOAT2INT_CLAMP(src13 * alpha2 +
				src23 * alpha1) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 fsrc0, fsrc1, fsrc2, fsrc3, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha2 = mlib_c_blend_u8_sat[256 - psrc2[4 * i + off]];

			fsrc0 = mlib_c_blend_Q8[psrc1[4 * i + 0] + psrc2[4 * i +
				0]];
			fsrc1 = mlib_c_blend_Q8[psrc1[4 * i + 1] + psrc2[4 * i +
				1]];
			fsrc2 = mlib_c_blend_Q8[psrc1[4 * i + 2] + psrc2[4 * i +
				2]];
			fsrc3 = mlib_c_blend_Q8[psrc1[4 * i + 3] + psrc2[4 * i +
				3]];

			pdst[4 * i + 0] = FLOAT2INT_CLAMP(fsrc0 * alpha2) >> 23;
			pdst[4 * i + 1] = FLOAT2INT_CLAMP(fsrc1 * alpha2) >> 23;
			pdst[4 * i + 2] = FLOAT2INT_CLAMP(fsrc2 * alpha2) >> 23;
			pdst[4 * i + 3] = FLOAT2INT_CLAMP(fsrc3 * alpha2) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 src10, src20;
	mlib_f32 src11, src21;
	mlib_f32 src12, src22;
	mlib_f32 src13, src23;
	mlib_f32 alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

			pdst[4 * i + 0] =
				(mlib_s32)(src10 + (src20 -
				src10) * alpha2) >> 23;
			pdst[4 * i + 1] =
				(mlib_s32)(src11 + (src21 -
				src11) * alpha2) >> 23;
			pdst[4 * i + 2] =
				(mlib_s32)(src12 + (src22 -
				src12) * alpha2) >> 23;
			pdst[4 * i + 3] =
				(mlib_s32)(src13 + (src23 -
				src13) * alpha2) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 alpha;
	mlib_s32 help_alpha;
	mlib_f32 d_s0, d_s1, d_s2, a;

	BLEND_VALIDATE;

	alpha = 0;
	slb1 -= w << 2;
	slb2 -= w << 2;
	dlb -= w << 2;
	help_alpha = 3;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			d_s0 = mlib_c_blend_u8_sat[psrc1[1]];
			d_s1 = mlib_c_blend_u8_sat[psrc1[2]];
			d_s2 = mlib_c_blend_u8_sat[psrc1[help_alpha]];
			a = mlib_c_blend_u8_sat[psrc1[alpha]];

			d_s0 += mlib_c_blend_Q8[psrc2[1]] * a;
			d_s1 += mlib_c_blend_Q8[psrc2[2]] * a;
			d_s2 += mlib_c_blend_Q8[psrc2[help_alpha]] * a;

			pdst[alpha] =
				FLOAT2INT_CLAMP(a +
				mlib_c_blend_Q8[psrc2[alpha]] * a) >> 23;
			pdst[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
			pdst[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
			pdst[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

			psrc1 += 4;
			psrc2 += 4;
			pdst += 4;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 alpha, help_alpha;
	mlib_f32 d_s0, d_s1, d_s2, a;

	BLEND_VALIDATE;
	alpha = 0;

	slb1 -= w << 2;
	slb2 -= w << 2;
	dlb -= w << 2;
	help_alpha = 3;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			d_s0 = mlib_c_blend_u8_sat[psrc1[1]];
			d_s1 = mlib_c_blend_u8_sat[psrc1[2]];
			d_s2 = mlib_c_blend_u8_sat[psrc1[help_alpha]];
			a = mlib_c_blend_u8_sat[256 - psrc1[alpha]];

			d_s0 += mlib_c_blend_Q8[psrc2[1]] * a;
			d_s1 += mlib_c_blend_Q8[psrc2[2]] * a;
			d_s2 += mlib_c_blend_Q8[psrc2[help_alpha]] * a;

			pdst[alpha] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc1
				[alpha]] +
				mlib_c_blend_Q8[psrc2[alpha]] * a) >> 23;
			pdst[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
			pdst[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
			pdst[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

			psrc1 += 4;
			psrc2 += 4;
			pdst += 4;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 alpha, help_alpha;
	mlib_f32 d_s0, d_s1, d_s2, a;

	BLEND_VALIDATE;

	alpha = 0;
	slb1 -= w << 2;
	slb2 -= w << 2;
	dlb -= w << 2;
	help_alpha = 3;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			d_s0 = mlib_c_blend_u8_sat[psrc1[1]];
			d_s1 = mlib_c_blend_u8_sat[psrc1[2]];
			d_s2 = mlib_c_blend_u8_sat[psrc1[help_alpha]];
			a = mlib_c_blend_u8_sat[256 - psrc2[alpha]];

			d_s0 += mlib_c_blend_Q8[psrc2[1]] * a;
			d_s1 += mlib_c_blend_Q8[psrc2[2]] * a;
			d_s2 += mlib_c_blend_Q8[psrc2[help_alpha]] * a;

			pdst[alpha] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc1
				[alpha]] +
				mlib_c_blend_Q8[psrc2[alpha]] * a) >> 23;
			pdst[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
			pdst[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
			pdst[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

			psrc1 += 4;
			psrc2 += 4;
			pdst += 4;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorAdd_U8_U8_Sat((mlib_u8 *)dst, (mlib_u8 *)src1,
			(mlib_u8 *)src2, w * 4);
		dst += dlb / 4;
		src1 += slb1 / 4;
		src2 += slb2 / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_f32 alpha = mlib_c_blend_Q8[psrc2[4 * i + off]];

			pdst[4 * i] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc2[4 *
				i]] * alpha +
				mlib_c_blend_u8_sat[psrc1[4 * i]]) >> 23;
			pdst[4 * i + 1] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc2[4 *
				i + 1]] * alpha +
				mlib_c_blend_u8_sat[psrc1[4 * i + 1]]) >> 23;
			pdst[4 * i + 2] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc2[4 *
				i + 2]] * alpha +
				mlib_c_blend_u8_sat[psrc1[4 * i + 2]]) >> 23;
			pdst[4 * i + 3] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc2[4 *
				i + 3]] * alpha +
				mlib_c_blend_u8_sat[psrc1[4 * i + 3]]) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy(dst, src1, w, h, dlb, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
			alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

			pdst[4 * i + 0] =
				FLOAT2INT_CLAMP(src10 * alpha2 +
				src20 * alpha1) >> 23;
			pdst[4 * i + 1] =
				FLOAT2INT_CLAMP(src11 * alpha2 +
				src21 * alpha1) >> 23;
			pdst[4 * i + 2] =
				FLOAT2INT_CLAMP(src12 * alpha2 +
				src22 * alpha1) >> 23;
			pdst[4 * i + 3] =
				FLOAT2INT_CLAMP(src13 * alpha2 +
				src23 * alpha1) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
			alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

			pdst[4 * i + 0] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc1[4 *
				i + 0]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i +
				0]] * alpha1) >> 23;
			pdst[4 * i + 1] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc1[4 *
				i + 1]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i +
				1]] * alpha1) >> 23;
			pdst[4 * i + 2] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc1[4 *
				i + 2]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i +
				2]] * alpha1) >> 23;
			pdst[4 * i + 3] =
				FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[psrc1[4 *
				i + 3]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i +
				3]] * alpha1) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 alpha;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha = mlib_c_blend_Q8[psrc2[4 * i + off]];

			src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
			src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];
			src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
			src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];
			src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
			src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];
			src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
			src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

			pdst[4 * i + 0] =
				(mlib_s32)((src10 - src20) * alpha +
				src20) >> 23;
			pdst[4 * i + 1] =
				(mlib_s32)((src11 - src21) * alpha +
				src21) >> 23;
			pdst[4 * i + 2] =
				(mlib_s32)((src12 - src22) * alpha +
				src22) >> 23;
			pdst[4 * i + 3] =
				(mlib_s32)((src13 - src23) * alpha +
				src23) >> 23;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 off = 0;
	mlib_f32 alpha, sum0, sum1, sum2, sum3;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha = mlib_c_blend_u8_sat[psrc2[4 * i + off]];

#ifdef MLIB_USE_FTOI_CLAMPING
			sum0 = mlib_c_blend_Q8[psrc1[4 * i + 0] + psrc2[4 * i +
				0]];
			pdst[4 * i + 0] = (mlib_s32)(sum0 * alpha) >> 23;

			sum1 = mlib_c_blend_Q8[psrc1[4 * i + 1] + psrc2[4 * i +
				1]];
			pdst[4 * i + 1] = (mlib_s32)(sum1 * alpha) >> 23;

			sum2 = mlib_c_blend_Q8[psrc1[4 * i + 2] + psrc2[4 * i +
				2]];
			pdst[4 * i + 2] = (mlib_s32)(sum2 * alpha) >> 23;

			sum3 = mlib_c_blend_Q8[psrc1[4 * i + 3] + psrc2[4 * i +
				3]];
			pdst[4 * i + 3] = (mlib_s32)(sum3 * alpha) >> 23;
#else /* MLIB_USE_FTOI_CLAMPING */
			sum0 = mlib_c_blend_Q8[psrc1[4 * i + 0] + psrc2[4 * i +
				0]] * alpha;

			if (sum0 >= MLIB_S32_MAX)
				pdst[4 * i + 0] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 0] = (mlib_s32)sum0 >> 23;

			sum1 = mlib_c_blend_Q8[psrc1[4 * i + 1] + psrc2[4 * i +
				1]] * alpha;

			if (sum1 >= MLIB_S32_MAX)
				pdst[4 * i + 1] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 1] = (mlib_s32)sum1 >> 23;

			sum2 = mlib_c_blend_Q8[psrc1[4 * i + 2] + psrc2[4 * i +
				2]] * alpha;

			if (sum2 >= MLIB_S32_MAX)
				pdst[4 * i + 2] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 2] = (mlib_s32)sum2 >> 23;

			sum3 = mlib_c_blend_Q8[psrc1[4 * i + 3] + psrc2[4 * i +
				3]] * alpha;

			if (sum3 >= MLIB_S32_MAX)
				pdst[4 * i + 3] = MLIB_S32_MAX >> 23;
			else
				pdst[4 * i + 3] = (mlib_s32)sum3 >> 23;
#endif /* MLIB_USE_FTOI_CLAMPING */
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_u8 a0, a1, a2, a3;
	mlib_f32 a;

	BLEND_VALIDATE;

	slb1 -= w << 2;
	slb2 -= w << 2;
	dlb -= w << 2;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			a = mlib_c_blend_Q8[psrc1[0]];
			a0 = mlib_U82F32[psrc2[0]] * a;
			a1 = mlib_U82F32[psrc2[1]] * a;
			a2 = mlib_U82F32[psrc2[2]] * a;
			a3 = mlib_U82F32[psrc2[3]] * a;
			pdst[0] = a0;
			pdst[1] = a1;
			pdst[2] = a2;
			pdst[3] = a3;
			psrc1 += 4;
			psrc2 += 4;
			pdst += 4;
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	BLEND_VALIDATE;

/* dst = src2 * (1 - ALPHAsrc1) */
	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			mlib_f32 omda =
				1 - mlib_c_blend_Q8[psrc1[i * slb1 + 4 * j]];

			pdst[i * dlb + 4 * j] =
				omda * mlib_U82F32[psrc2[i * slb2 + 4 * j]];
			pdst[i * dlb + 4 * j + 1] =
				omda * mlib_U82F32[psrc2[i * slb2 + 4 * j + 1]];
			pdst[i * dlb + 4 * j + 2] =
				omda * mlib_U82F32[psrc2[i * slb2 + 4 * j + 2]];
			pdst[i * dlb + 4 * j + 3] =
				omda * mlib_U82F32[psrc2[i * slb2 + 4 * j + 3]];
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_u8 a0, a1, a2, a3;
	mlib_s32 help_alpha;
	mlib_f32 a;

	BLEND_VALIDATE_1;

	slb2 -= w << 2;
	dlb -= w << 2;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			help_alpha = psrc2[0];
			a = mlib_c_blend_Q8[256 - help_alpha];
			a0 = mlib_U82F32[help_alpha] * a;
			a1 = mlib_U82F32[psrc2[1]] * a;
			a2 = mlib_U82F32[psrc2[2]] * a;
			a3 = mlib_U82F32[psrc2[3]] * a;
			pdst[0] = a0;
			pdst[1] = a1;
			pdst[2] = a2;
			pdst[3] = a3;
			psrc2 += 4;
			pdst += 4;
		}

		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy(dst, src2, w, h, dlb, slb2);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 help_alpha;
	mlib_f32 a;

	BLEND_VALIDATE_1;

	slb2 -= w << 2;
	dlb -= w << 2;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			help_alpha = psrc2[0];
			a = mlib_c_blend_Q8[help_alpha];
			pdst[0] = mlib_U82F32[help_alpha] * a;
			pdst[1] = mlib_U82F32[psrc2[1]] * a;
			pdst[2] = mlib_U82F32[psrc2[2]] * a;
			pdst[3] = mlib_U82F32[psrc2[3]] * a;
			psrc2 += 4;
			pdst += 4;
		}

		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorZero_S32((mlib_s32 *)dst, w);
		dst += dlb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_OMSA_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_OMSA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_DA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_DA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_DA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */
