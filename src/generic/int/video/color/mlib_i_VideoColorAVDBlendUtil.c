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

#pragma ident	"@(#)mlib_i_VideoColorAVDBlendUtil.c	9.2	07/11/05 SMI"
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
 *
 */

#include <mlib_algebra.h>
#include <mlib_VideoColorAVDBlend_proto.h>
#include <mlib_c_ImageBlendTable.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#define	SAT_U8_U8C(dst, src)	dst = (((src << 23) >> 31) | src)

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
	mlib_s32 alpha1, alpha2, rez;
	mlib_u8 src20, src21, src22, src23;
	mlib_u8 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = psrc1[4 * i + off];
			alpha2 = 256 - psrc2[4 * i + off];

			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];

			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];

			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];

			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				(src10 * alpha1 + src20 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 1],
				(src11 * alpha1 + src21 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 2],
				(src12 * alpha1 + src22 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 3],
				(src13 * alpha1 + src23 * alpha2) >> 8);
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
	mlib_s32 alpha1, alpha2;
	mlib_u8 src20, src21, src22, src23;
	mlib_u8 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = psrc1[4 * i + off];
			alpha2 = psrc2[4 * i + off];

			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];

			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];

			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];

			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				(src10 * alpha1 + src20 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 1],
				(src11 * alpha1 + src21 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 2],
				(src12 * alpha1 + src22 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 3],
				(src13 * alpha1 + src23 * alpha2) >> 8);
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
	mlib_u8 src10, src20;
	mlib_u8 src11, src21;
	mlib_u8 src12, src22;
	mlib_u8 src13, src23;
	mlib_s32 alpha1, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = 256 - psrc1[4 * i + off];
			alpha2 = 256 - psrc2[4 * i + off];

			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];

			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];

			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];

			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				(src10 * alpha1 + src20 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 1],
				(src11 * alpha1 + src21 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 2],
				(src12 * alpha1 + src22 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 3],
				(src13 * alpha1 + src23 * alpha2) >> 8);
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
	mlib_u8 src10, src20;
	mlib_u8 src11, src21;
	mlib_u8 src12, src22;
	mlib_u8 src13, src23;
	mlib_s32 alpha1, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = 256 - psrc1[4 * i + off];
			alpha2 = 256 - psrc2[4 * i + off];
			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];

			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];

			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];

			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				((src10 * alpha2 + src20 * alpha1) >> 8));
			SAT_U8_U8C(pdst[4 * i + 1],
				((src11 * alpha2 + src21 * alpha1) >> 8));
			SAT_U8_U8C(pdst[4 * i + 2],
				((src12 * alpha2 + src22 * alpha1) >> 8));
			SAT_U8_U8C(pdst[4 * i + 3],
				((src13 * alpha2 + src23 * alpha1) >> 8));
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
	mlib_s32 fsrc0, fsrc1, fsrc2, fsrc3, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha2 = 256 - psrc2[4 * i + off];

			fsrc0 = psrc1[4 * i + 0] + psrc2[4 * i + 0];
			fsrc1 = psrc1[4 * i + 1] + psrc2[4 * i + 1];
			fsrc2 = psrc1[4 * i + 2] + psrc2[4 * i + 2];
			fsrc3 = psrc1[4 * i + 3] + psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0], (fsrc0 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 1],  (fsrc1 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 2],  (fsrc2 * alpha2) >> 8);
			SAT_U8_U8C(pdst[4 * i + 3], (fsrc3 * alpha2) >> 8);
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
	mlib_u8 src10, src20;
	mlib_u8 src11, src21;
	mlib_u8 src12, src22;
	mlib_u8 src13, src23;
	mlib_s32 alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha2 = psrc2[4 * i + off];
			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];

			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];

			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];

			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				(src10 + (((src20 - src10) * alpha2) >> 8)));
			SAT_U8_U8C(pdst[4 * i + 1],
				(src11 + (((src21 - src11) * alpha2) >> 8)));
			SAT_U8_U8C(pdst[4 * i + 2],
				(src12 + (((src22 - src12) * alpha2) >> 8)));
			SAT_U8_U8C(pdst[4 * i + 3],
				(src13 + (((src23 - src13) * alpha2) >> 8)));
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
	mlib_s32 d_s0, d_s1, d_s2, a;

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
			d_s0 = psrc1[1];
			d_s1 = psrc1[2];
			d_s2 = psrc1[help_alpha];
			a = psrc1[alpha];

			d_s0 += (psrc2[1] * a) >> 8;
			d_s1 += (psrc2[2] * a) >> 8;
			d_s2 += (psrc2[help_alpha] * a) >> 8;

			SAT_U8_U8C(pdst[alpha],
				(a + ((psrc2[alpha] * a) >> 8)));
			SAT_U8_U8C(pdst[1], d_s0);
			SAT_U8_U8C(pdst[2], d_s1);
			SAT_U8_U8C(pdst[help_alpha], d_s2);

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
	mlib_s32 d_s0, d_s1, d_s2, a;

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
			d_s0 = psrc1[1];
			d_s1 = psrc1[2];
			d_s2 = psrc1[help_alpha];
			a = 256 - psrc1[alpha];

			d_s0 += (psrc2[1] * a) >> 8;
			d_s1 += (psrc2[2] * a) >> 8;
			d_s2 += (psrc2[help_alpha] * a) >> 8;

			SAT_U8_U8C(pdst[alpha],
				(psrc1[alpha] + ((psrc2[alpha] * a) >> 8)));
			SAT_U8_U8C(pdst[1], d_s0);
			SAT_U8_U8C(pdst[2], d_s1);
			SAT_U8_U8C(pdst[help_alpha], d_s2);

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
	mlib_s32 d_s0, d_s1, d_s2, a;

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
			d_s0 = psrc1[1];
			d_s1 = psrc1[2];
			d_s2 = psrc1[help_alpha];
			a = 256 - psrc2[alpha];

			d_s0 += (psrc2[1] * a) >> 8;
			d_s1 += (psrc2[2] * a) >> 8;
			d_s2 += (psrc2[help_alpha] * a) >> 8;

			SAT_U8_U8C(pdst[alpha],
				(psrc1[alpha] + ((psrc2[alpha] * a) >> 8)));
			SAT_U8_U8C(pdst[1], d_s0);
			SAT_U8_U8C(pdst[2], d_s1);
			SAT_U8_U8C(pdst[help_alpha], d_s2);

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
			mlib_s32 alpha = psrc2[4 * i + off];

			SAT_U8_U8C(pdst[4 * i],
			(((psrc2[4 * i] * alpha) >> 8) + psrc1[4 * i]));
			SAT_U8_U8C(pdst[4 * i + 1],
			(((psrc2[4 * i + 1] * alpha) >> 8) + \
			psrc1[4 * i + 1]));
			SAT_U8_U8C(pdst[4 * i + 2],
			(((psrc2[4 * i + 2] * alpha) >> 8) + \
			psrc1[4 * i + 2]));
			SAT_U8_U8C(pdst[4 * i + 3],
			(((psrc2[4 * i + 3] * alpha) >> 8) + \
			psrc1[4 * i + 3]));
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
	mlib_s32 alpha1, alpha2;
	mlib_u8 src20, src21, src22, src23;
	mlib_u8 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = psrc1[4 * i + off];
			alpha2 = psrc2[4 * i + off];

			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];

			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];

			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];

			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				(src10 * alpha2 + src20 * alpha1) >> 8);
			SAT_U8_U8C(pdst[4 * i + 1],
				(src11 * alpha2 + src21 * alpha1) >> 8);
			SAT_U8_U8C(pdst[4 * i + 2],
				(src12 * alpha2 + src22 * alpha1) >> 8);
			SAT_U8_U8C(pdst[4 * i + 3],
				(src13 * alpha2 + src23 * alpha1) >> 8);
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
	mlib_s32 alpha1, alpha2;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha1 = 256 - psrc1[4 * i + off];
			alpha2 = psrc2[4 * i + off];

			SAT_U8_U8C(pdst[4 * i + 0],
				(psrc1[4 * i + 0] * alpha2 + psrc2[4 * i +
				0] * alpha1) >> 8);
			SAT_U8_U8C(pdst[4 * i + 1],
				(psrc1[4 * i + 1] * alpha2 + psrc2[4 * i +
				1] * alpha1) >> 8);
			SAT_U8_U8C(pdst[4 * i + 2],
				(psrc1[4 * i + 2] * alpha2 + psrc2[4 * i +
				2] * alpha1) >> 8);
			SAT_U8_U8C(pdst[4 * i + 3],
				(psrc1[4 * i + 3] * alpha2 + psrc2[4 * i +
				3] * alpha1) >> 8);
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
	mlib_s32 alpha;
	mlib_u8 src20, src21, src22, src23;
	mlib_u8 src10, src11, src12, src13;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha = psrc2[4 * i + off];

			src10 = psrc1[4 * i + 0];
			src20 = psrc2[4 * i + 0];
			src11 = psrc1[4 * i + 1];
			src21 = psrc2[4 * i + 1];
			src12 = psrc1[4 * i + 2];
			src22 = psrc2[4 * i + 2];
			src13 = psrc1[4 * i + 3];
			src23 = psrc2[4 * i + 3];

			SAT_U8_U8C(pdst[4 * i + 0],
				((((src10 - src20) * alpha) >> 8) +
				src20));
			SAT_U8_U8C(pdst[4 * i + 1],
				((((src11 - src21) * alpha) >> 8) +
				src21));
			SAT_U8_U8C(pdst[4 * i + 2],
				((((src12 - src22) * alpha) >> 8) +
				src22));
			SAT_U8_U8C(pdst[4 * i + 3],
				((((src13 - src23) * alpha) >> 8) +
				src23));
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
	mlib_s32 alpha, sum0, sum1, sum2, sum3;

	BLEND_VALIDATE;

	for (j = 0; j < h; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {

			alpha = psrc2[4 * i + off];

			sum0 = psrc1[4 * i + 0] + psrc2[4 * i + 0];
			SAT_U8_U8C(pdst[4 * i + 0], (sum0 * alpha) >> 8);

			sum1 = psrc1[4 * i + 1] + psrc2[4 * i +	1];
			SAT_U8_U8C(pdst[4 * i + 1], (sum1 * alpha) >> 8);

			sum2 = psrc1[4 * i + 2] + psrc2[4 * i + 2];
			SAT_U8_U8C(pdst[4 * i + 2], (sum2 * alpha) >> 8);

			sum3 = psrc1[4 * i + 3] + psrc2[4 * i + 3];
			SAT_U8_U8C(pdst[4 * i + 3], (sum3 * alpha) >> 8);
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
	mlib_s32 a;

	BLEND_VALIDATE;

	slb1 -= w << 2;
	slb2 -= w << 2;
	dlb -= w << 2;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			a = psrc1[0];
			a0 = (psrc2[0] * a) >> 8;
			a1 = (psrc2[1] * a) >> 8;
			a2 = (psrc2[2] * a) >> 8;
			a3 = (psrc2[3] * a) >> 8;
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
			mlib_s32 omda =
				256 - psrc1[i * slb1 + 4 * j];

			pdst[i * dlb + 4 * j + 0] =
				(omda * psrc2[i * slb2 + 4 * j + 0]) >> 8;
			pdst[i * dlb + 4 * j + 1] =
				(omda * psrc2[i * slb2 + 4 * j + 1]) >> 8;
			pdst[i * dlb + 4 * j + 2] =
				(omda * psrc2[i * slb2 + 4 * j + 2]) >> 8;
			pdst[i * dlb + 4 * j + 3] =
				(omda * psrc2[i * slb2 + 4 * j + 3]) >> 8;
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
	mlib_s32 a;

	BLEND_VALIDATE_1;

	slb2 -= w << 2;
	dlb -= w << 2;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			help_alpha = psrc2[0];
			a = 256 - help_alpha;
			a0 = (help_alpha * a) >> 8;
			a1 = (psrc2[1] * a) >> 8;
			a2 = (psrc2[2] * a) >> 8;
			a3 = (psrc2[3] * a) >> 8;
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
	mlib_s32 a;

	BLEND_VALIDATE_1;

	slb2 -= w << 2;
	dlb -= w << 2;

	for (i = 0; i < h; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			help_alpha = psrc2[0];
			a = help_alpha;
			pdst[0] = (help_alpha * a) >> 8;
			pdst[1] = (psrc2[1] * a) >> 8;
			pdst[2] = (psrc2[2] * a) >> 8;
			pdst[3] = (psrc2[3] * a) >> 8;
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
