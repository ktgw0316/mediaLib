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

#pragma ident	"@(#)mlib_v_ImageMinMaxFilter.c	9.2	07/11/05 SMI"

/*
 * Implementation for mlib_ImageMaxFilter* and mlib_ImageMinFilter*
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageLineMaxMin.h>
#include <mlib_v_ImageDilateErode.h>
#include <mlib_v_ImageDataTypeConvert.h>

/* *********************************************************** */

typedef void (*func_type1) (
	mlib_u8 *dst,
	mlib_s32 size);

typedef void (*func_type2) (
	mlib_u8 *src,
	mlib_u8 *dst,
	mlib_s32 size);

typedef void (*func_type3) (
	mlib_s16 *dst,
	mlib_s16 *src0,
	mlib_s16 *src1,
	mlib_s32 size);

/* *********************************************************** */

static const mlib_s32 row_ind_3x3[] = { 1, 2, 0, 1, 1, 3 };
static const mlib_s32 row_ind_5x5[] = { 3, 4, 1, 3, 0, 1, 1, 5 };
static const mlib_s32 row_ind_7x7[] = { 1, 3, 5, 6, 1, 5, 0, 1, 1, 7 };

/* *********************************************************** */

void
mlib_ImageLineMin_Dst_U8(
    mlib_s16 *dp,
    const mlib_s16 *sp0,
    const mlib_s16 *sp1,
    mlib_s32 n)
{
	mlib_ImageLineMin((void *)sp0, (void *)sp1, n);
	mlib_v_ImageDataTypeConvert_S16_U8_D1((void *)sp0, (void *)dp, n);
}

/* *********************************************************** */

void
mlib_ImageLineMin_Dst_U16(
    mlib_s16 *dp,
    const mlib_s16 *sp0,
    const mlib_s16 *sp1,
    mlib_s32 n)
{
	mlib_ImageLineMin((void *)sp0, (void *)sp1, n);
	mlib_ImageLineXor8000((void *)sp0, (void *)dp, 2 * n);
}

/* *********************************************************** */

void
mlib_ImageLineMin_Dst_S16(
    mlib_s16 *dp,
    const mlib_s16 *sp0,
    const mlib_s16 *sp1,
    mlib_s32 n)
{
	while (((mlib_s32)dp & 7) && n) {
		mlib_s32 x = (*sp0++);
		mlib_s32 y = (*sp1++);

		(*dp++) = (x < y) ? x : y;
		n--;
	}

	if (n) {
		mlib_ImageLineMin_SrcNa2((void *)dp, (void *)sp0, (void *)sp1,
		    n);
	}
}

/* *********************************************************** */

void
mlib_ImageLineMax_Dst_U8(
    mlib_s16 *dp,
    const mlib_s16 *sp0,
    const mlib_s16 *sp1,
    mlib_s32 n)
{
	mlib_ImageLineMax((void *)sp0, (void *)sp1, n);
	mlib_v_ImageDataTypeConvert_S16_U8_D1((void *)sp0, (void *)dp, n);
}

/* *********************************************************** */

void
mlib_ImageLineMax_Dst_U16(
    mlib_s16 *dp,
    const mlib_s16 *sp0,
    const mlib_s16 *sp1,
    mlib_s32 n)
{
	mlib_ImageLineMax((void *)sp0, (void *)sp1, n);
	mlib_ImageLineXor8000((void *)sp0, (void *)dp, 2 * n);
}

/* *********************************************************** */

void
mlib_ImageLineMax_Dst_S16(
    mlib_s16 *dp,
    const mlib_s16 *sp0,
    const mlib_s16 *sp1,
    mlib_s32 n)
{
	while (((mlib_s32)dp & 7) && n) {
		mlib_s32 x = (*sp0++);
		mlib_s32 y = (*sp1++);

		(*dp++) = (x > y) ? x : y;
		n--;
	}

	if (n) {
		mlib_ImageLineMax_SrcNa2((void *)dp, (void *)sp0, (void *)sp1,
		    n);
	}
}

/* *********************************************************** */

#define	MAX_KER	7

mlib_status
mlib_v_ImageMinMaxFilterXxX(
    mlib_u8 *dl,
    const mlib_u8 *sl,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 ws,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 ksize1,
    const mlib_s32 *row_ind,
    func_type2 func_CopySrc,
    func_type2 func_LineMinMax,
    func_type1 func_LineMinMax1,
    func_type3 func_LineMinMax_Dst)
{
	mlib_s32 num_buf = ksize1 + 2;
	mlib_s32 j, k, row = 0, bline;
	mlib_d64 *pbuff, *prow[2 * (MAX_KER + 1)];

	if (w <= 0 || h <= 0)
		return (MLIB_SUCCESS);

	bline = (w + ksize1 + 3) / 4;
	pbuff = __mlib_malloc(num_buf * sizeof (mlib_d64) * bline);

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (k = 0; k < num_buf; k++) {
		prow[k] = prow[num_buf + k] = pbuff + k * bline;
	}

	for (j = 0; j < ksize1; j++) {
		func_CopySrc((void *)sl, (void *)prow[j], ws);
		for (k = ksize1 - 1; k >= 0; k--) {
			func_LineMinMax1((void *)prow[j], w + k);
		}

		sl += slb;
	}

	if (ksize1 >= 4) {
		func_LineMinMax((void *)prow[1], (void *)prow[2], w);
	}

	if (ksize1 >= 6) {
		func_LineMinMax((void *)prow[3], (void *)prow[4], w);
	}

	for (j = 0; j < h; j += 2) {
		mlib_d64 **prow0 = prow + row;

/* copy two lines by one call */
		if (slb == ws && ((slb & 7) == 0) && j + 1 < h) {
			func_CopySrc((void *)sl, (void *)prow0[ksize1], 2 * ws);
		} else {
			func_CopySrc((void *)sl, (void *)prow0[ksize1], ws);

			if (j + 1 < h) {
				func_CopySrc((void *)(sl + slb),
				    (void *)prow0[ksize1 + 1], ws);
			}
		}

		for (k = ksize1 - 1; k >= 0; k--) {
			func_LineMinMax1((void *)prow0[ksize1], 2 * 4 * bline);
		}

		for (k = 0; k < ksize1; k += 2) {
			func_LineMinMax((void *)prow0[row_ind[k]],
			    (void *)prow0[row_ind[k + 1]], w);
		}

		func_LineMinMax_Dst((void *)dl, (void *)prow0[row_ind[k]],
		    (void *)prow0[row_ind[k + 1]], w);

		if (j + 1 < h) {
			func_LineMinMax_Dst((void *)(dl + dlb),
			    (void *)prow0[row_ind[k + 2]],
			    (void *)prow0[row_ind[k + 3]], w);
		}

		sl += 2 * slb;
		dl += 2 * dlb;
		row += 2;

		if (row >= num_buf)
			row = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	DEF_FUNC(FLT, K)                                                   \
	mlib_status                                                        \
	mlib_Image##FLT##Filter##K##x##K##_U8                              \
	    (mlib_u8 *dst, const mlib_u8 *src, mlib_s32 dlb,               \
	    mlib_s32 slb, mlib_s32 w, mlib_s32 h)                          \
	{                                                                  \
	    mlib_s32 k1 = K - 1;                                           \
	                                                                   \
	    dst += (k1 / 2) * (dlb + 1);                                   \
	    return mlib_v_ImageMinMaxFilterXxX((void *)dst, (void *)src,   \
		dlb, slb, w, w - k1, h - k1, k1,                           \
		row_ind_##K##x##K,                                         \
		(func_type2) & mlib_v_ImageDataTypeConvert_U8_S16_D1,      \
		(func_type2) & mlib_ImageLine##FLT,                        \
		(func_type1) & mlib_ImageLine##FLT##1,                     \
		(func_type3) & mlib_ImageLine##FLT##_Dst_U8);              \
	}                                                                  \
	                                                                   \
	mlib_status                                                        \
	mlib_Image##FLT##Filter##K##x##K##_S16                             \
	    (mlib_s16 *dst, const mlib_s16 *src, mlib_s32 dlb,             \
	    mlib_s32 slb, mlib_s32 w, mlib_s32 h)                          \
	{                                                                  \
	    mlib_s32 k1 = K - 1;                                           \
	                                                                   \
	    dst += (k1 / 2) * (dlb / 2 + 1);                               \
	    return mlib_v_ImageMinMaxFilterXxX((void *)dst, (void *)src,   \
		dlb, slb, 2 * w, w - k1, h - k1, k1,                       \
		row_ind_##K##x##K,                                         \
		(func_type2) & mlib_ImageCopy_na,                          \
		(func_type2) & mlib_ImageLine##FLT,                        \
		(func_type1) & mlib_ImageLine##FLT##1,                     \
		(func_type3) & mlib_ImageLine##FLT##_Dst_S16);             \
	}                                                                  \
	                                                                   \
	mlib_status                                                        \
	mlib_Image##FLT##Filter##K##x##K##_U16                             \
	    (mlib_u16 *dst, const mlib_u16 *src, mlib_s32 dlb,             \
	    mlib_s32 slb, mlib_s32 w, mlib_s32 h)                          \
	{                                                                  \
	    mlib_s32 k1 = K - 1;                                           \
	                                                                   \
	    dst += (k1 / 2) * (dlb / 2 + 1);                               \
	    return mlib_v_ImageMinMaxFilterXxX((void *)dst, (void *)src,   \
		dlb, slb, 2 * w, w - k1, h - k1, k1,                       \
		row_ind_##K##x##K,                                         \
		(func_type2) & mlib_ImageLineXor8000,                      \
		(func_type2) & mlib_ImageLine##FLT,                        \
		(func_type1) & mlib_ImageLine##FLT##1,                     \
		(func_type3) & mlib_ImageLine##FLT##_Dst_U16);             \
	}

/* *********************************************************** */

DEF_FUNC(Max, 3)
DEF_FUNC(Max, 5)
DEF_FUNC(Max, 7)
DEF_FUNC(Min, 3)
DEF_FUNC(Min, 5)
DEF_FUNC(Min, 7)

/* *********************************************************** */
