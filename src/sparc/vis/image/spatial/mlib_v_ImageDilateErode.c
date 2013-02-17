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

#pragma ident	"@(#)mlib_v_ImageDilateErode.c	9.2	07/11/05 SMI"

/*
 * Implementation for mlib_ImageDilate4 and mlib_ImageErode4
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageDataTypeConvert.h>
#include <mlib_v_ImageLineMaxMin.h>
#include <mlib_v_ImageDilateErode.h>

/* *********************************************************** */

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

typedef void (*func_type2) (
	mlib_u8 *src,
	mlib_u8 *dst,
	mlib_s32 size);

typedef void (*func_type3) (
	mlib_u8 *dst,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	mlib_s32 size);

/* *********************************************************** */

static mlib_status mlib_v_ImageDilateErode(
    mlib_u8 *dl,
    const mlib_u8 *sl,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 type_sh,
    func_type2 func_CopySrc,
    func_type2 func_LineMinMax,
    func_type3 func_LineMinMax_SrcNa,
    func_type2 func_CopyDst);

/* *********************************************************** */

mlib_status
mlib_ImageDilate4_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	dst += dlb + 1;
	return mlib_v_ImageDilateErode((void *)dst, (void *)src, dlb, slb, w, h,
	    0, (func_type2) & mlib_v_ImageDataTypeConvert_U8_S16_D1,
	    (func_type2) & mlib_ImageLineMax, mlib_ImageLineMax_SrcNa,
	    (func_type2) & mlib_v_ImageDataTypeConvert_S16_U8_D1);
}

/* *********************************************************** */

mlib_status
mlib_ImageDilate4_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	dst += dlb / 2 + 1;
	return mlib_v_ImageDilateErode((void *)dst, (void *)src, dlb, slb, w, h,
	    1, (func_type2) & mlib_ImageCopy_na,
	    (func_type2) & mlib_ImageLineMax, mlib_ImageLineMax_SrcNa,
	    (func_type2) & mlib_ImageCopy_na);
}

/* *********************************************************** */

mlib_status
mlib_ImageDilate4_U16(
    mlib_u16 *dst,
    const mlib_u16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	dst += dlb / 2 + 1;
	return mlib_v_ImageDilateErode((void *)dst, (void *)src, dlb, slb, w, h,
	    1, (func_type2) & mlib_ImageLineXor8000,
	    (func_type2) & mlib_ImageLineMax, mlib_ImageLineMax_SrcNa,
	    (func_type2) & mlib_ImageLineXor8000);
}

/* *********************************************************** */

mlib_status
mlib_ImageErode4_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	dst += dlb + 1;
	return mlib_v_ImageDilateErode((void *)dst, (void *)src, dlb, slb, w, h,
	    0, (func_type2) & mlib_v_ImageDataTypeConvert_U8_S16_D1,
	    (func_type2) & mlib_ImageLineMin, mlib_ImageLineMin_SrcNa,
	    (func_type2) & mlib_v_ImageDataTypeConvert_S16_U8_D1);
}

/* *********************************************************** */

mlib_status
mlib_ImageErode4_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	dst += dlb / 2 + 1;
	return mlib_v_ImageDilateErode((void *)dst, (void *)src, dlb, slb, w, h,
	    1, (func_type2) & mlib_ImageCopy_na,
	    (func_type2) & mlib_ImageLineMin, mlib_ImageLineMin_SrcNa,
	    (func_type2) & mlib_ImageCopy_na);
}

/* *********************************************************** */

mlib_status
mlib_ImageErode4_U16(
    mlib_u16 *dst,
    const mlib_u16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	dst += dlb / 2 + 1;
	return mlib_v_ImageDilateErode((void *)dst, (void *)src, dlb, slb, w, h,
	    1, (func_type2) & mlib_ImageLineXor8000,
	    (func_type2) & mlib_ImageLineMin, mlib_ImageLineMin_SrcNa,
	    (func_type2) & mlib_ImageLineXor8000);
}

/* *********************************************************** */

void
mlib_ImageLineXor8000(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 size)
{
	mlib_u8 *dend;
	mlib_d64 *dptr;
	mlib_d64 *sptr;
	mlib_d64 s0, s1;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	mlib_s32 j;
	mlib_s32 emask;

/* prepare the destination addresses */
	dptr = (mlib_d64 *)((mlib_addr)dst & (~7));
	j = (mlib_addr)dptr - (mlib_addr)dst;
	dend = (mlib_u8 *)dst + size - 1;

/* prepare the source address */
	sptr = (mlib_d64 *)VIS_ALIGNADDR(src, j);
/* generate edge mask for the start point */
	emask = vis_edge8(dst, dend);

	s1 = vis_ld_d64_nf(sptr);

	if (emask != 0xff) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sptr + 1);
		s0 = vis_fxor(vis_faligndata(s0, s1), mask8000);
		vis_pst_8(s0, dptr++, emask);
		sptr++;
		j += 8;
	}

#pragma pipeloop(0)
	for (; j <= (size - 16); j += 8) {
		s0 = s1;
		s1 = sptr[1];
		(*dptr++) = vis_fxor(vis_faligndata(s0, s1), mask8000);
		sptr++;
	}

	if (j <= (size - 8)) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sptr + 1);
		(*dptr++) = vis_fxor(vis_faligndata(s0, s1), mask8000);
		sptr++;
		j += 8;
	}

	if (j < size) {
		s0 = vis_fxor(vis_faligndata(s1, vis_ld_d64_nf(sptr + 1)),
		    mask8000);
		emask = vis_edge8(dptr, dend);
		vis_pst_8(s0, dptr, emask);
	}
}

/* *********************************************************** */

mlib_status
mlib_v_ImageDilateErode(
    mlib_u8 *dl,
    const mlib_u8 *sl,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 type_sh,
    func_type2 func_CopySrc,
    func_type2 func_LineMinMax,
    func_type3 func_LineMinMax_SrcNa,
    func_type2 func_CopyDst)
{
	mlib_s32 num_buf = 6;
	mlib_s32 j, k, ws, row = 0, bline;
	mlib_d64 *pbuff, *prow[2 * 6];

	if (w <= 2 || h <= 2)
		return (MLIB_SUCCESS);
	ws = w << type_sh;

	bline = (w + 3) / 4;
	pbuff = __mlib_malloc(num_buf * sizeof (mlib_d64) * bline);

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (k = 0; k < num_buf; k++) {
		prow[k] = prow[num_buf + k] = pbuff + k * bline;
	}

	for (j = 0; j < 2; j++) {
		func_CopySrc((void *)sl, (void *)prow[j], ws);
		sl += slb;
	}

	w -= 2;
	h -= 2;

	for (j = 0; j < h; j += 2) {
		mlib_d64 **prow0 = prow + row;

/* copy two lines by one call */
		if (slb == ws && ((slb & 7) == 0) && j + 1 < h) {
			func_CopySrc((void *)sl, (void *)prow0[2], 2 * ws);
		} else {
			func_CopySrc((void *)sl, (void *)prow0[2], ws);

			if (j + 1 < h) {
				func_CopySrc((void *)(sl + slb),
				    (void *)prow0[3], ws);
			}
		}

		func_LineMinMax_SrcNa((void *)prow0[4], (void *)prow0[1],
		    (void *)((mlib_s16 *)prow0[1] + 2), w);
		func_LineMinMax_SrcNa((void *)prow0[5], (void *)prow0[2],
		    (void *)((mlib_s16 *)prow0[2] + 2), w);

		func_LineMinMax((void *)prow0[1], (void *)prow0[2], w + 1);
		func_LineMinMax((void *)prow0[0], (void *)prow0[1], w + 1);
		func_LineMinMax((void *)prow0[1], (void *)prow0[3], w + 1);

		func_LineMinMax_SrcNa((void *)prow0[0], (void *)prow0[0],
		    (void *)((mlib_s16 *)prow0[4] - 1), 2 * 4 * bline);

		func_CopyDst((void *)((mlib_s16 *)prow0[0] + 1), (void *)dl,
		    w << type_sh);

		if (j + 1 < h) {
			func_CopyDst((void *)((mlib_s16 *)prow0[1] + 1),
			    (void *)(dl + dlb), w << type_sh);
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
