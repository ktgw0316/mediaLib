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

#pragma ident	"@(#)mlib_ImageZoomTransTableBlEdge.c	9.2	07/11/05 SMI"

#include <mlib_image.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageAffine.h>
#include <mlib_ImageZoomTranslateTable.h>

#ifndef _NO_LONGLONG

/* *********************************************************** */

#define	DTYPE	mlib_u8

#define	AMP_FT 20
#define	HALF (1ll << (AMP_FT - 1))
/* *********************************************************** */

static void
blend_zero(
    DTYPE * dPtr,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend,
    mlib_s32 size)
{
	mlib_s32 alpha_ind = 3;
	mlib_s32 i;

	if (blend != MLIB_BLEND_GTK_SRC) {
		if ((blend == MLIB_BLEND_GTK_SRC_OVER) && (dchan == 4)) {

			if ((alpha_shift == 1) && (dchan == 4)) {
				alpha_ind = -1;
				dPtr++;
			}

			for (i = 0; i < size; i++) {
				if (dPtr[4 * i + alpha_ind] == 0) {
					dPtr[4 * i] = 0;
					dPtr[4 * i + 1] = 0;
					dPtr[4 * i + 2] = 0;
				}
			}
		}
	} else {
		for (i = 0; i < dchan * size; i++) {
			dPtr[i] = 0;
		}

		if ((schan == 3) && (dchan == 4)) {

			if ((alpha_shift == 1) && (dchan == 4)) {
				alpha_ind = -1;
				dPtr++;
			}

			for (i = 0; i < size; i++) {
				dPtr[4 * i + alpha_ind] = 255;
			}
		}
	}
}

/* *********************************************************** */

static void
blend_nearest(
    DTYPE * dPtr,
    DTYPE * Sp,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend,
    mlib_s32 size,
    mlib_s32 X,
    mlib_s32 dX)
{
	DTYPE *sPtr;
	mlib_s32 alpha_ind = 3;
	mlib_s32 i;

	if (alpha_shift == 1) {
		alpha_ind = -1;
		if (dchan == 4)
			dPtr++;
		if (schan == 4)
			Sp++;
	}

	if (blend == MLIB_BLEND_GTK_SRC_OVER) {
		for (i = 0; i < size; i++) {
			mlib_s64 a0 = 255;
			mlib_s64 a1 = 255;
			mlib_s64 sr, sg, sb;
			mlib_s64 dr = (dPtr[dchan * i]);
			mlib_s64 dg = (dPtr[dchan * i + 1]);
			mlib_s64 db = (dPtr[dchan * i + 2]);
			mlib_s64 scale;
			mlib_s64 ialp;

			sPtr = Sp + (X >> MLIB_SHIFT) * schan;

			sr = (sPtr[0]);
			sg = (sPtr[1]);
			sb = (sPtr[2]);

			if (schan == 4)
				a0 = (sPtr[alpha_ind]);
			if (dchan == 4)
				a1 = (dPtr[dchan * i + alpha_ind]);

			a1 = (255 - a0) * (a1 << AMP_FT) / 255;
			a0 <<= AMP_FT;
			ialp = a0 + a1 + HALF;
			if (a0 + a1 == 0)
				scale = 0;
			else
				scale = (1ll << AMP_FT * 2) / (a0 + a1);

			dPtr[dchan * i] =
				(((a0 * sr + a1 * dr) * scale >> AMP_FT) +
				HALF) >> AMP_FT;
			dPtr[dchan * i + 1] =
				(((a0 * sg + a1 * dg) * scale >> AMP_FT) +
				HALF) >> AMP_FT;
			dPtr[dchan * i + 2] =
				(((a0 * sb + a1 * db) * scale >> AMP_FT) +
				HALF) >> AMP_FT;

			if (dchan == 4)
				dPtr[dchan * i + alpha_ind] = ialp >> AMP_FT;

			X += dX;
		}
	} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
		for (i = 0; i < size; i++) {
			mlib_s64 a0 = 255;
			mlib_s64 a1 = 255;
			mlib_s64 sr, sg, sb;
			mlib_s64 dr = (dPtr[dchan * i]);
			mlib_s64 dg = (dPtr[dchan * i + 1]);
			mlib_s64 db = (dPtr[dchan * i + 2]);
			mlib_s64 scale;
			mlib_s64 ialp;

			sPtr = Sp + (X >> MLIB_SHIFT) * schan;

			sr = (sPtr[0]);
			sg = (sPtr[1]);
			sb = (sPtr[2]);

			if (schan == 4)
				a0 = (sPtr[alpha_ind]);
			if (dchan == 4)
				a1 = (dPtr[dchan * i + alpha_ind]);

			a1 = (255 - a0) * (a1 << AMP_FT) / 255;
			a0 <<= AMP_FT;
			ialp = a0 + a1 + HALF;
			if (a0 + a1 == 0)
				scale = 0;
			else
				scale = (1ll << AMP_FT * 2) / (a0 + a1);


			dPtr[dchan * i] =
				((a0 * (sr - dr) * scale >> AMP_FT) +
				HALF + (dr << AMP_FT)) >> AMP_FT;
			dPtr[dchan * i + 1] =
				((a0 * (sg - dg) * scale >> AMP_FT) +
				HALF + (dg << AMP_FT)) >> AMP_FT;
			dPtr[dchan * i + 2] =
				((a0 * (sb - db) * scale >> AMP_FT) +
				HALF + (db << AMP_FT)) >> AMP_FT;

			if (dchan == 4)
				dPtr[dchan * i + alpha_ind] = ialp >> AMP_FT;

			X += dX;
		}
	} else {
		for (i = 0; i < size; i++) {
			mlib_s64 a0 = 255;
			mlib_s64 sr, sg, sb;
			mlib_s64 scale;

			sPtr = Sp + (X >> MLIB_SHIFT) * schan;

			sr = (sPtr[0]);
			sg = (sPtr[1]);
			sb = (sPtr[2]);

			if (schan == 4)
				a0 = (sPtr[alpha_ind]);

			if (dchan == 4) {
				dPtr[dchan * i] = ((sr << 1) + 1) >> 1;
				dPtr[dchan * i + 1] = ((sg << 1) + 1) >> 1;
				dPtr[dchan * i + 2] = ((sb << 1) + 1) >> 1;
				dPtr[dchan * i + alpha_ind] = a0;
			} else {
				scale = (1ll << AMP_FT) / 255;
				dPtr[dchan * i] =
					(a0 * sr * scale + HALF) >> AMP_FT;
				dPtr[dchan * i + 1] =
					(a0 * sg * scale + HALF) >> AMP_FT;
				dPtr[dchan * i + 2] =
					(a0 * sb * scale + HALF) >> AMP_FT;
			}

			X += dX;
		}
	}
}

/* *********************************************************** */
#define	MLIB_EDGE_ZERO_LINE(Left, Right)                             \
	blend_zero((DTYPE *) data + channels * Left, dchan, schan,   \
	    alpha_shift, blend, Right - Left)

/* *********************************************************** */

void
mlib_ImageZoomTranslateTableBlendEdgeZero(
    mlib_affine_param *param,
    mlib_affine_param *param_e,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend)
{
	mlib_image *dst = param->dst;
	mlib_s32 *leftEdges = param->leftEdges;
	mlib_s32 *rightEdges = param->rightEdges;
	mlib_s32 *leftEdgesE = param_e->leftEdges;
	mlib_s32 *rightEdgesE = param_e->rightEdges;
	mlib_s32 channels = mlib_ImageGetChannels(dst);
	mlib_s32 dstStride = mlib_ImageGetStride(dst);
	mlib_s32 yStart = param->yStart;
	mlib_s32 yFinish = param->yFinish;
	mlib_s32 yStartE = param_e->yStart;
	mlib_s32 yFinishE = param_e->yFinish;
	mlib_u8 *data = param_e->dstData;
	mlib_s32 xLeft, xRight, xLeftE, xRightE;
	mlib_s32 i;

	for (i = yStartE; i < yStart; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		data += dstStride;

		MLIB_EDGE_ZERO_LINE(xLeftE, xRightE);
	}

	for (; i <= yFinish; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		xLeft = leftEdges[i];
		xRight = rightEdges[i] + 1;
		data += dstStride;

		if (xLeft < xRight) {
			MLIB_EDGE_ZERO_LINE(xLeftE, xLeft);
		} else {
			xRight = xLeftE;
		}

		MLIB_EDGE_ZERO_LINE(xRight, xRightE);
	}

	for (; i <= yFinishE; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		data += dstStride;

		MLIB_EDGE_ZERO_LINE(xLeftE, xRightE);
	}
}

/* *********************************************************** */

#define	MLIB_EDGE_NEAREST_LINE(Left, Right)                        \
	blend_nearest((DTYPE *) data + channels * Left,            \
	    (DTYPE *) lineAddr[(Y >> MLIB_SHIFT)], dchan, schan,   \
	    alpha_shift, blend, Right - Left, X, dX)

/* *********************************************************** */

void
mlib_ImageZoomTranslateTableBlendEdgeNearest(
    mlib_affine_param *param,
    mlib_affine_param *param_e,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend)
{
	mlib_image *dst = param->dst;
	mlib_s32 *leftEdges = param->leftEdges;
	mlib_s32 *rightEdges = param->rightEdges;
	mlib_s32 *leftEdgesE = param_e->leftEdges;
	mlib_s32 *rightEdgesE = param_e->rightEdges;
	mlib_s32 channels = mlib_ImageGetChannels(dst);
	mlib_s32 dstStride = mlib_ImageGetStride(dst);
	mlib_s32 yStart = param->yStart;
	mlib_s32 yFinish = param->yFinish;
	mlib_s32 yStartE = param_e->yStart;
	mlib_s32 yFinishE = param_e->yFinish;
	mlib_u8 *data = param_e->dstData;
	mlib_s32 xLeft, xRight, xLeftE, xRightE;
	mlib_s32 *xStartsE = param_e->xStarts;
	mlib_s32 *yStartsE = param_e->yStarts;
	mlib_u8 **lineAddr = param->lineAddr;
	mlib_s32 dX = param_e->dX;
	mlib_s32 X, Y, i;

	for (i = yStartE; i < yStart; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		X = xStartsE[i];
		Y = yStartsE[i];
		data += dstStride;

		MLIB_EDGE_NEAREST_LINE(xLeftE, xRightE);
	}

	for (; i <= yFinish; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		xLeft = leftEdges[i];
		xRight = rightEdges[i] + 1;
		X = xStartsE[i];
		Y = yStartsE[i];
		data += dstStride;

		if (xLeft < xRight) {
			MLIB_EDGE_NEAREST_LINE(xLeftE, xLeft);
		} else {
			xRight = xLeftE;
		}

		X = xStartsE[i] + dX * (xRight - xLeftE);
		Y = yStartsE[i];
		MLIB_EDGE_NEAREST_LINE(xRight, xRightE);
	}

	for (; i <= yFinishE; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		X = xStartsE[i];
		Y = yStartsE[i];
		data += dstStride;

		MLIB_EDGE_NEAREST_LINE(xLeftE, xRightE);
	}
}

/* *********************************************************** */

#else	/* _NO_LONG_LONG */


#define	FILTER_TYPE	mlib_f32
#define	FP_TYPE	mlib_f32

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	SRC2FP(x)	U82FP(x)

#ifdef _MSC_VER

#define	U82FP(x)	(x)

#else /* _MSC_VER */

#define	U82FP(x)	mlib_U82F32[x]

#endif /* _MSC_VER */

/* *********************************************************** */

#define	ZERO_EDGE	(FP_TYPE)0.5

/* *********************************************************** */

#define	SCALE	(((FP_TYPE)1)/((FP_TYPE)255))

/* *********************************************************** */

static void
blend_zero(
    DTYPE * dPtr,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend,
    mlib_s32 size)
{
	mlib_s32 alpha_ind = 3;
	mlib_s32 i;

	if (blend != MLIB_BLEND_GTK_SRC) {
		if ((blend == MLIB_BLEND_GTK_SRC_OVER) && (dchan == 4)) {

			if ((alpha_shift == 1) && (dchan == 4)) {
				alpha_ind = -1;
				dPtr++;
			}

			for (i = 0; i < size; i++) {
				if (dPtr[4 * i + alpha_ind] == 0) {
					dPtr[4 * i] = 0;
					dPtr[4 * i + 1] = 0;
					dPtr[4 * i + 2] = 0;
				}
			}
		}
	} else {
		for (i = 0; i < dchan * size; i++) {
			dPtr[i] = 0;
		}

		if ((schan == 3) && (dchan == 4)) {

			if ((alpha_shift == 1) && (dchan == 4)) {
				alpha_ind = -1;
				dPtr++;
			}

			for (i = 0; i < size; i++) {
				dPtr[4 * i + alpha_ind] = 255;
			}
		}
	}
}

/* *********************************************************** */

static void
blend_nearest(
    DTYPE * dPtr,
    DTYPE * Sp,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend,
    mlib_s32 size,
    mlib_s32 X,
    mlib_s32 dX)
{
	DTYPE *sPtr;
	mlib_s32 alpha_ind = 3;
	mlib_s32 i;

	if (alpha_shift == 1) {
		alpha_ind = -1;
		if (dchan == 4)
			dPtr++;
		if (schan == 4)
			Sp++;
	}

	if (blend == MLIB_BLEND_GTK_SRC_OVER) {
		for (i = 0; i < size; i++) {
			FP_TYPE a0 = (FP_TYPE) 255;
			FP_TYPE a1 = (FP_TYPE) 255;
			FP_TYPE sr, sg, sb;
			FP_TYPE dr = SRC2FP(dPtr[dchan * i]);
			FP_TYPE dg = SRC2FP(dPtr[dchan * i + 1]);
			FP_TYPE db = SRC2FP(dPtr[dchan * i + 2]);
			FP_TYPE scale;
			mlib_s32 ialp;

			sPtr = Sp + (X >> MLIB_SHIFT) * schan;

			sr = SRC2FP(sPtr[0]);
			sg = SRC2FP(sPtr[1]);
			sb = SRC2FP(sPtr[2]);

			if (schan == 4)
				a0 = SRC2FP(sPtr[alpha_ind]);
			if (dchan == 4)
				a1 = SRC2FP(dPtr[dchan * i + alpha_ind]);

			a1 = (255 - a0) * a1 * SCALE;
			ialp = (mlib_s32)(a0 + a1 + (FP_TYPE) 0.5);
			FP_INV(scale, (a0 + a1));

			dPtr[dchan * i] =
			    (DTYPE) ((a0 * sr + a1 * dr) * scale +
			    (FP_TYPE) 0.5);
			dPtr[dchan * i + 1] =
			    (DTYPE) ((a0 * sg + a1 * dg) * scale +
			    (FP_TYPE) 0.5);
			dPtr[dchan * i + 2] =
			    (DTYPE) ((a0 * sb + a1 * db) * scale +
			    (FP_TYPE) 0.5);

			if (dchan == 4)
				dPtr[dchan * i + alpha_ind] = ialp;

			X += dX;
		}
	} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
		for (i = 0; i < size; i++) {
			FP_TYPE a0 = (FP_TYPE) 255;
			FP_TYPE a1 = (FP_TYPE) 255;
			FP_TYPE sr, sg, sb;
			FP_TYPE dr = SRC2FP(dPtr[dchan * i]);
			FP_TYPE dg = SRC2FP(dPtr[dchan * i + 1]);
			FP_TYPE db = SRC2FP(dPtr[dchan * i + 2]);
			FP_TYPE scale;
			mlib_s32 ialp;

			sPtr = Sp + (X >> MLIB_SHIFT) * schan;

			sr = SRC2FP(sPtr[0]);
			sg = SRC2FP(sPtr[1]);
			sb = SRC2FP(sPtr[2]);

			if (schan == 4)
				a0 = SRC2FP(sPtr[alpha_ind]);
			if (dchan == 4)
				a1 = SRC2FP(dPtr[dchan * i + alpha_ind]);

			a1 = (255 - a0) * a1 * SCALE;
			ialp = (mlib_s32)(a0 + a1 + (FP_TYPE) 0.5);
			FP_INV(scale, (a0 + a1));

			dPtr[dchan * i] =
			    (DTYPE) ((a0 * (sr - dr)) * scale +
			    (FP_TYPE) 0.5 + dr);
			dPtr[dchan * i + 1] =
			    (DTYPE) ((a0 * (sg - dg)) * scale +
			    (FP_TYPE) 0.5 + dg);
			dPtr[dchan * i + 2] =
			    (DTYPE) ((a0 * (sb - db)) * scale +
			    (FP_TYPE) 0.5 + db);

			if (dchan == 4)
				dPtr[dchan * i + alpha_ind] = ialp;

			X += dX;
		}
	} else {
		for (i = 0; i < size; i++) {
			FP_TYPE a0 = (FP_TYPE) 255;
			FP_TYPE sr, sg, sb;

			sPtr = Sp + (X >> MLIB_SHIFT) * schan;

			sr = SRC2FP(sPtr[0]);
			sg = SRC2FP(sPtr[1]);
			sb = SRC2FP(sPtr[2]);

			if (schan == 4)
				a0 = SRC2FP(sPtr[alpha_ind]);

			if (dchan == 4) {
				dPtr[dchan * i] = (DTYPE) (sr + (FP_TYPE) 0.5);
				dPtr[dchan * i + 1] =
				    (DTYPE) (sg + (FP_TYPE) 0.5);
				dPtr[dchan * i + 2] =
				    (DTYPE) (sb + (FP_TYPE) 0.5);
				dPtr[dchan * i + alpha_ind] = (DTYPE) a0;
			} else {
				dPtr[dchan * i] =
				    (DTYPE) ((a0 * sr / (FP_TYPE) 255) +
				    (FP_TYPE) 0.5);
				dPtr[dchan * i + 1] =
				    (DTYPE) ((a0 * sg / (FP_TYPE) 255) +
				    (FP_TYPE) 0.5);
				dPtr[dchan * i + 2] =
				    (DTYPE) ((a0 * sb / (FP_TYPE) 255) +
				    (FP_TYPE) 0.5);
			}

			X += dX;
		}
	}
}

/* *********************************************************** */
#define	MLIB_EDGE_ZERO_LINE(Left, Right)                             \
	blend_zero((DTYPE *) data + channels * Left, dchan, schan,   \
	    alpha_shift, blend, Right - Left)

/* *********************************************************** */

void
mlib_ImageZoomTranslateTableBlendEdgeZero(
    mlib_affine_param *param,
    mlib_affine_param *param_e,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend)
{
	mlib_image *dst = param->dst;
	mlib_s32 *leftEdges = param->leftEdges;
	mlib_s32 *rightEdges = param->rightEdges;
	mlib_s32 *leftEdgesE = param_e->leftEdges;
	mlib_s32 *rightEdgesE = param_e->rightEdges;
	mlib_s32 channels = mlib_ImageGetChannels(dst);
	mlib_s32 dstStride = mlib_ImageGetStride(dst);
	mlib_s32 yStart = param->yStart;
	mlib_s32 yFinish = param->yFinish;
	mlib_s32 yStartE = param_e->yStart;
	mlib_s32 yFinishE = param_e->yFinish;
	mlib_u8 *data = param_e->dstData;
	mlib_s32 xLeft, xRight, xLeftE, xRightE;
	mlib_s32 i;

	for (i = yStartE; i < yStart; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		data += dstStride;

		MLIB_EDGE_ZERO_LINE(xLeftE, xRightE);
	}

	for (; i <= yFinish; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		xLeft = leftEdges[i];
		xRight = rightEdges[i] + 1;
		data += dstStride;

		if (xLeft < xRight) {
			MLIB_EDGE_ZERO_LINE(xLeftE, xLeft);
		} else {
			xRight = xLeftE;
		}

		MLIB_EDGE_ZERO_LINE(xRight, xRightE);
	}

	for (; i <= yFinishE; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		data += dstStride;

		MLIB_EDGE_ZERO_LINE(xLeftE, xRightE);
	}
}

/* *********************************************************** */

#define	MLIB_EDGE_NEAREST_LINE(Left, Right)                        \
	blend_nearest((DTYPE *) data + channels * Left,            \
	    (DTYPE *) lineAddr[(Y >> MLIB_SHIFT)], dchan, schan,   \
	    alpha_shift, blend, Right - Left, X, dX)

/* *********************************************************** */

void
mlib_ImageZoomTranslateTableBlendEdgeNearest(
    mlib_affine_param *param,
    mlib_affine_param *param_e,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 alpha_shift,
    mlib_blend blend)
{
	mlib_image *dst = param->dst;
	mlib_s32 *leftEdges = param->leftEdges;
	mlib_s32 *rightEdges = param->rightEdges;
	mlib_s32 *leftEdgesE = param_e->leftEdges;
	mlib_s32 *rightEdgesE = param_e->rightEdges;
	mlib_s32 channels = mlib_ImageGetChannels(dst);
	mlib_s32 dstStride = mlib_ImageGetStride(dst);
	mlib_s32 yStart = param->yStart;
	mlib_s32 yFinish = param->yFinish;
	mlib_s32 yStartE = param_e->yStart;
	mlib_s32 yFinishE = param_e->yFinish;
	mlib_u8 *data = param_e->dstData;
	mlib_s32 xLeft, xRight, xLeftE, xRightE;
	mlib_s32 *xStartsE = param_e->xStarts;
	mlib_s32 *yStartsE = param_e->yStarts;
	mlib_u8 **lineAddr = param->lineAddr;
	mlib_s32 dX = param_e->dX;
	mlib_s32 X, Y, i;

	for (i = yStartE; i < yStart; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		X = xStartsE[i];
		Y = yStartsE[i];
		data += dstStride;

		MLIB_EDGE_NEAREST_LINE(xLeftE, xRightE);
	}

	for (; i <= yFinish; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		xLeft = leftEdges[i];
		xRight = rightEdges[i] + 1;
		X = xStartsE[i];
		Y = yStartsE[i];
		data += dstStride;

		if (xLeft < xRight) {
			MLIB_EDGE_NEAREST_LINE(xLeftE, xLeft);
		} else {
			xRight = xLeftE;
		}

		X = xStartsE[i] + dX * (xRight - xLeftE);
		Y = yStartsE[i];
		MLIB_EDGE_NEAREST_LINE(xRight, xRightE);
	}

	for (; i <= yFinishE; i++) {
		xLeftE = leftEdgesE[i];
		xRightE = rightEdgesE[i] + 1;
		X = xStartsE[i];
		Y = yStartsE[i];
		data += dstStride;

		MLIB_EDGE_NEAREST_LINE(xLeftE, xRightE);
	}
}

/* *********************************************************** */


#endif
