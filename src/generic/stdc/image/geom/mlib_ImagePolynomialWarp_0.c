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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_0.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	S0(dx)	(-0.5 * dx * dx * dx + 1.0 * dx * dx - 0.5 * dx)
#define	S1(dx)	(1.5 * dx * dx * dx - 2.5 * dx * dx + 1.0)
#define	S2(dx)	(-1.5 * dx * dx * dx + 2.0 * dx * dx + 0.5 * dx)
#define	S3(dx)	(0.5 * dx * dx * dx - 0.5 * dx * dx)

/* *********************************************************** */

#define	S0_BC2(dx)	(-1.0 * dx * dx * dx + 2.0 * dx * dx - 1.0 * dx)
#define	S1_BC2(dx)	(1.0 * dx * dx * dx - 2.0 * dx * dx + 1.0)
#define	S2_BC2(dx)	(-1.0 * dx * dx * dx + 1.0 * dx * dx + 1.0 * dx)
#define	S3_BC2(dx)	(1.0 * dx * dx * dx - 1.0 * dx * dx)

/* *********************************************************** */

static mlib_status mlib_ImagePolynomialWarp_0_NN(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy);

static mlib_status mlib_ImagePolynomialWarp_0_BL(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy);

static mlib_status mlib_ImagePolynomialWarp_0_BC(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy);

static mlib_status mlib_ImagePolynomialWarp_0_BC2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy);

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarp_0(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_d64 postScaleX,
    mlib_d64 postScaleY,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_d64 wx, wy, Xstart, Ystart, Xstop, Ystop;
	mlib_s32 srcWidth, srcHeight;

	srcWidth = mlib_ImageGetWidth(src);
	srcHeight = mlib_ImageGetHeight(src);

	if (filter == MLIB_BILINEAR && (srcWidth < 2 || srcHeight < 2))
		return (MLIB_SUCCESS);

	if ((filter == MLIB_BICUBIC || filter == MLIB_BICUBIC2) &&
	    (srcWidth < 4 || srcHeight < 4))
		return (MLIB_SUCCESS);

	wx = xCoeffs[0] * postScaleX - postShiftX;
	wx -= (filter != MLIB_NEAREST) ? 0.5 : 0.0;

	wy = yCoeffs[0] * postScaleY - postShiftY;
	wy -= (filter != MLIB_NEAREST) ? 0.5 : 0.0;

	Xstart =
	    (filter == MLIB_NEAREST) ? 0.0 : ((filter ==
	    MLIB_BILINEAR) ? 0.5 : 1.5);
	Ystart =
	    (filter == MLIB_NEAREST) ? 0.0 : ((filter ==
	    MLIB_BILINEAR) ? 0.5 : 1.5);
	Xstop = srcWidth - Xstart;
	Ystop = srcHeight - Ystart;

	if (edge == MLIB_EDGE_SRC_PADDED) {
		mlib_u8 *paddings = mlib_ImageGetPaddings(src);

		if (paddings[0] > Xstart)
			Xstart = paddings[0];

		if (paddings[1] > Ystart)
			Ystart = paddings[1];

		if ((srcWidth - paddings[2]) < Xstop)
			Xstop = srcWidth - paddings[2];

		if ((srcHeight - paddings[3]) < Ystop)
			Ystop = srcHeight - paddings[3];
	}

	if (!(wx >= Xstart && wx < Xstop && wy >= Ystart && wy < Ystop))
		return (MLIB_SUCCESS);

	switch (filter) {
	case MLIB_NEAREST:
		return (mlib_ImagePolynomialWarp_0_NN(dst, src, wx, wy));

	case MLIB_BILINEAR:
		return (mlib_ImagePolynomialWarp_0_BL(dst, src, wx, wy));

	case MLIB_BICUBIC:
		return (mlib_ImagePolynomialWarp_0_BC(dst, src, wx, wy));

	case MLIB_BICUBIC2:
		return (mlib_ImagePolynomialWarp_0_BC2(dst, src, wx, wy));

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarp_0_NN(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy)
{
	mlib_s32 srcYStride, srcChannels, i, color[4];
	mlib_type srcType;

	srcType = mlib_ImageGetType(src);
	srcYStride = mlib_ImageGetStride(src);
	srcChannels = mlib_ImageGetChannels(src);

	switch (srcType) {
	case MLIB_BYTE:
	    {
		    mlib_u8 *srcData = mlib_ImageGetData(src);

		    srcData = srcData + (mlib_s32)wy *srcYStride;
		    srcData += (mlib_s32)wx *srcChannels;

		    for (i = 0; i < srcChannels; i++)
			color[i] = srcData[i];
	    }

		break;

	case MLIB_USHORT:
	case MLIB_SHORT:
	    {
		    mlib_s16 *srcData = mlib_ImageGetData(src);

		    srcData =
			(void *)((mlib_u8 *)srcData +
			(mlib_s32)wy * srcYStride);
		    srcData += (mlib_s32)wx *srcChannels;

		    for (i = 0; i < srcChannels; i++)
			color[i] = srcData[i];
	    }

		break;

	case MLIB_INT:
	    {
		    mlib_s32 *srcData = mlib_ImageGetData(src);

		    srcData =
			(void *)((mlib_u8 *)srcData +
			(mlib_s32)wy * srcYStride);
		    srcData += (mlib_s32)wx *srcChannels;

		    for (i = 0; i < srcChannels; i++)
			color[i] = srcData[i];
	    }

		break;
	default:
		return (MLIB_FAILURE);
	}

	return (__mlib_ImageClear(dst, color));
}

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarp_0_BL(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy)
{
	mlib_d64 a0, a1, a2, a3, r0, r1, dx, dy;
	mlib_s32 i, srcYStride, srcChannels, color[4];
	mlib_type srcType;

	srcType = mlib_ImageGetType(src);
	srcYStride = mlib_ImageGetStride(src);
	srcChannels = mlib_ImageGetChannels(src);

	dx = wx - (mlib_s32)wx;
	dy = wy - (mlib_s32)wy;

	switch (srcType) {
	case MLIB_BYTE:
	    {
		    mlib_u8 *srcData0 = mlib_ImageGetData(src), *srcData1;

		    srcData0 = srcData0 + (mlib_s32)wy *srcYStride;
		    srcData0 += (mlib_s32)wx *srcChannels;

		    srcData1 = srcData0 + srcYStride;

		    for (i = 0; i < srcChannels; i++) {
			    a0 = srcData0[i];
			    a1 = srcData0[srcChannels + i];
			    a2 = srcData1[srcChannels + i];
			    a3 = srcData1[i];

			    r0 = a0 + (a1 - a0) * dx;
			    r1 = a3 + (a2 - a3) * dx;
			    color[i] = (mlib_s32)(r0 + (r1 - r0) * dy);
		    }
	    }

		break;

	case MLIB_SHORT:
	    {
		    mlib_s16 *srcData0 = mlib_ImageGetData(src), *srcData1;

		    srcData0 =
			(void *)((mlib_u8 *)srcData0 +
			(mlib_s32)wy * srcYStride);
		    srcData0 += (mlib_s32)wx *srcChannels;

		    srcData1 = (void *)((mlib_u8 *)srcData0 + srcYStride);

		    for (i = 0; i < srcChannels; i++) {
			    a0 = srcData0[i];
			    a1 = srcData0[srcChannels + i];
			    a2 = srcData1[srcChannels + i];
			    a3 = srcData1[i];

			    r0 = a0 + (a1 - a0) * dx;
			    r1 = a3 + (a2 - a3) * dx;
			    color[i] = (mlib_s32)(r0 + (r1 - r0) * dy);
		    }
	    }

		break;

	case MLIB_USHORT:
	    {
		    mlib_u16 *srcData0 = mlib_ImageGetData(src), *srcData1;

		    srcData0 =
			(void *)((mlib_u8 *)srcData0 +
			(mlib_s32)wy * srcYStride);
		    srcData0 += (mlib_s32)wx *srcChannels;

		    srcData1 = (void *)((mlib_u8 *)srcData0 + srcYStride);

		    for (i = 0; i < srcChannels; i++) {
			    a0 = srcData0[i];
			    a1 = srcData0[srcChannels + i];
			    a2 = srcData1[srcChannels + i];
			    a3 = srcData1[i];

			    r0 = a0 + (a1 - a0) * dx;
			    r1 = a3 + (a2 - a3) * dx;
			    color[i] = (mlib_s32)(r0 + (r1 - r0) * dy);
		    }
	    }

		break;

	case MLIB_INT:
	    {
		    mlib_s32 *srcData0 = mlib_ImageGetData(src), *srcData1;

		    srcData0 =
			(void *)((mlib_u8 *)srcData0 +
			(mlib_s32)wy * srcYStride);
		    srcData0 += (mlib_s32)wx *srcChannels;

		    srcData1 = (void *)((mlib_u8 *)srcData0 + srcYStride);

		    for (i = 0; i < srcChannels; i++) {
			    a0 = srcData0[i];
			    a1 = srcData0[srcChannels + i];
			    a2 = srcData1[srcChannels + i];
			    a3 = srcData1[i];

			    r0 = a0 + (a1 - a0) * dx;
			    r1 = a3 + (a2 - a3) * dx;
			    color[i] = (mlib_s32)(r0 + (r1 - r0) * dy);
		    }
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (__mlib_ImageClear(dst, color));
}

/* *********************************************************** */

#define	CALC_BICUBIC                                                 \
	for (i = 0; i < srcChannels; i++) {                          \
	    srcData0 = srcData;                                      \
	    a0 = srcData0[0 * srcChannels + i];                      \
	    a1 = srcData0[1 * srcChannels + i];                      \
	    a2 = srcData0[2 * srcChannels + i];                      \
	    a3 = srcData0[3 * srcChannels + i];                      \
	    r0 = fx0 * a0 + fx1 * a1 + fx2 * a2 + fx3 * a3;          \
	    srcData0 = (void *)((mlib_u8 *)srcData0 + srcYStride);   \
	    a0 = srcData0[0 * srcChannels + i];                      \
	    a1 = srcData0[1 * srcChannels + i];                      \
	    a2 = srcData0[2 * srcChannels + i];                      \
	    a3 = srcData0[3 * srcChannels + i];                      \
	    r1 = fx0 * a0 + fx1 * a1 + fx2 * a2 + fx3 * a3;          \
	    srcData0 = (void *)((mlib_u8 *)srcData0 + srcYStride);   \
	    a0 = srcData0[0 * srcChannels + i];                      \
	    a1 = srcData0[1 * srcChannels + i];                      \
	    a2 = srcData0[2 * srcChannels + i];                      \
	    a3 = srcData0[3 * srcChannels + i];                      \
	    r2 = fx0 * a0 + fx1 * a1 + fx2 * a2 + fx3 * a3;          \
	    srcData0 = (void *)((mlib_u8 *)srcData0 + srcYStride);   \
	    a0 = srcData0[0 * srcChannels + i];                      \
	    a1 = srcData0[1 * srcChannels + i];                      \
	    a2 = srcData0[2 * srcChannels + i];                      \
	    a3 = srcData0[3 * srcChannels + i];                      \
	    r3 = fx0 * a0 + fx1 * a1 + fx2 * a2 + fx3 * a3;          \
	    res = fy0 * r0 + fy1 * r1 + fy2 * r2 + fy3 * r3;         \
	    res = res < limit_lo ? limit_lo : res;                   \
	    res = res > limit_hi ? limit_hi : res;                   \
	    color[i] = (mlib_s32)res;                                \
	}

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarp_0_BC(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy)
{
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, res;
	mlib_d64 fx0, fx1, fx2, fx3;
	mlib_d64 fy0, fy1, fy2, fy3;
	mlib_d64 dx, dy;
	mlib_s32 i, srcYStride, srcChannels, color[4];
	mlib_type srcType;
	mlib_d64 limit_lo, limit_hi;

	srcType = mlib_ImageGetType(src);
	srcYStride = mlib_ImageGetStride(src);
	srcChannels = mlib_ImageGetChannels(src);

	dx = wx - (mlib_s32)wx;
	dy = wy - (mlib_s32)wy;

	fx0 = S0(dx);
	fx1 = S1(dx);
	fx2 = S2(dx);
	fx3 = S3(dx);
	fy0 = S0(dy);
	fy1 = S1(dy);
	fy2 = S2(dy);
	fy3 = S3(dy);

	switch (srcType) {
	case MLIB_BYTE:
	    {
		    mlib_u8 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_U8_MIN;
		    limit_hi = MLIB_U8_MAX;

		    srcData = srcData + ((mlib_s32)wy - 1) * srcYStride;
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	case MLIB_SHORT:
	    {
		    mlib_s16 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_S16_MIN;
		    limit_hi = MLIB_S16_MAX;

		    srcData =
			(void *)((mlib_u8 *)srcData + ((mlib_s32)wy -
			1) * srcYStride);
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	case MLIB_USHORT:
	    {
		    mlib_u16 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_U16_MIN;
		    limit_hi = MLIB_U16_MAX;

		    srcData =
			(void *)((mlib_u8 *)srcData + ((mlib_s32)wy -
			1) * srcYStride);
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	case MLIB_INT:
	    {
		    mlib_s32 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_S32_MIN;
		    limit_hi = MLIB_S32_MAX;

		    srcData =
			(void *)((mlib_u8 *)srcData + ((mlib_s32)wy -
			1) * srcYStride);
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (__mlib_ImageClear(dst, color));
}

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarp_0_BC2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 wx,
    mlib_d64 wy)
{
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, res;
	mlib_d64 fx0, fx1, fx2, fx3;
	mlib_d64 fy0, fy1, fy2, fy3;
	mlib_d64 dx, dy;
	mlib_s32 i, srcYStride, srcChannels, color[4];
	mlib_type srcType;
	mlib_d64 limit_lo, limit_hi;

	srcType = mlib_ImageGetType(src);
	srcYStride = mlib_ImageGetStride(src);
	srcChannels = mlib_ImageGetChannels(src);

	dx = wx - (mlib_s32)wx;
	dy = wy - (mlib_s32)wy;

	fx0 = S0_BC2(dx);
	fx1 = S1_BC2(dx);
	fx2 = S2_BC2(dx);
	fx3 = S3_BC2(dx);
	fy0 = S0_BC2(dy);
	fy1 = S1_BC2(dy);
	fy2 = S2_BC2(dy);
	fy3 = S3_BC2(dy);

	switch (srcType) {
	case MLIB_BYTE:
	    {
		    mlib_u8 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_U8_MIN;
		    limit_hi = MLIB_U8_MAX;

		    srcData = srcData + ((mlib_s32)wy - 1) * srcYStride;
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	case MLIB_SHORT:
	    {
		    mlib_s16 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_S16_MIN;
		    limit_hi = MLIB_S16_MAX;

		    srcData =
			(void *)((mlib_u8 *)srcData + ((mlib_s32)wy -
			1) * srcYStride);
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	case MLIB_USHORT:
	    {
		    mlib_u16 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_U16_MIN;
		    limit_hi = MLIB_U16_MAX;

		    srcData =
			(void *)((mlib_u8 *)srcData + ((mlib_s32)wy -
			1) * srcYStride);
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	case MLIB_INT:
	    {
		    mlib_s32 *srcData0, *srcData = mlib_ImageGetData(src);

		    limit_lo = MLIB_S32_MIN;
		    limit_hi = MLIB_S32_MAX;

		    srcData =
			(void *)((mlib_u8 *)srcData + ((mlib_s32)wy -
			1) * srcYStride);
		    srcData += ((mlib_s32)wx - 1) * srcChannels;

		    CALC_BICUBIC;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (__mlib_ImageClear(dst, color));
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
