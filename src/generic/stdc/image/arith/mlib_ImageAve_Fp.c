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

#pragma ident	"@(#)mlib_ImageAve_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAve_Fp - get average of two images
 *
 * SYNOPSIS
 *      mlib_status  mlib_ImageAve_Fp(mlib_image *dst,
 *                                    const mlib_image *src1,
 *                                    const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT,
 *      MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      dst = (src1 + src2) / 2
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAve_Fp = __mlib_ImageAve_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAve_Fp) mlib_ImageAve_Fp
    __attribute__((weak, alias("__mlib_ImageAve_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ARITH_FUN_F32(dst, src1, src2)	(dst) = ((src1) + (src2)) *0.5f
#define	ARITH_FUN_D64(dst, src1, src2)	(dst) = ((src1) + (src2)) *0.5

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
} d64_2_f32;

/* *********************************************************** */

mlib_status
__mlib_ImageAve_Fp(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_type dtype;
	mlib_s32 nchan;
	mlib_s32 dwidth;
	mlib_s32 dheight;
	mlib_s32 dlb, slb1, slb2;
	mlib_s32 xsize, ysize;
	mlib_u8 *pdst, *psrc1, *psrc2;
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, dwidth, dheight, dlb,
	    pdst);
	slb1 = mlib_ImageGetStride(src1);
	psrc1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	psrc2 = mlib_ImageGetData(src2);

	xsize = dwidth * nchan;
	ysize = dheight;

	if (dtype == MLIB_FLOAT)
		dwidth = xsize * 4;
	else if (dtype == MLIB_DOUBLE)
		dwidth = xsize * 8;
	else
		return (MLIB_FAILURE);

	if ((dwidth == slb1) && (dwidth == slb2) && (dwidth == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

	if (dtype == MLIB_FLOAT) {

		for (j = 0; j < ysize; j++) {
			mlib_f32 *ps1 = (mlib_f32 *)psrc1;
			mlib_f32 *ps2 = (mlib_f32 *)psrc2;
			mlib_f32 *pd = (mlib_f32 *)pdst;
			mlib_f32 *pend = (mlib_f32 *)pdst + xsize;
			mlib_addr addr_sum =
			    ((mlib_addr)ps1 & 7) + ((mlib_addr)ps2 & 7) +
			    ((mlib_addr)pd & 7);
			mlib_f32 val;
			d64_2_f32 s1, s2, d;

			if (addr_sum > 4) {
				ARITH_FUN_F32(*pd, *ps1, *ps2);
				pd++;
				ps1++;
				ps2++;
			}

/* only ps1 unaligned */
			if (((mlib_addr)ps1 & 7)) {
				val = *ps1;
				ps1 += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd < (pend - 1);
				    pd += 2, ps1 += 2, ps2 += 2) {
					s1.d64 = *(mlib_d64 *)ps1;
					s2.d64 = *(mlib_d64 *)ps2;
					ARITH_FUN_F32(d.f32s.f0, val,
					    s2.f32s.f0);
					ARITH_FUN_F32(d.f32s.f1, s1.f32s.f0,
					    s2.f32s.f1);
					*(mlib_d64 *)pd = d.d64;
					val = s1.f32s.f1;
				}

				ps1 -= 1;
			} else if (((mlib_addr)ps2 & 7)) {
/* only ps2 unaligned */
				val = *ps2;
				ps2 += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd < (pend - 1);
				    pd += 2, ps1 += 2, ps2 += 2) {
					s1.d64 = *(mlib_d64 *)ps1;
					s2.d64 = *(mlib_d64 *)ps2;
					ARITH_FUN_F32(d.f32s.f0, val,
					    s1.f32s.f0);
					ARITH_FUN_F32(d.f32s.f1, s2.f32s.f0,
					    s1.f32s.f1);
					*(mlib_d64 *)pd = d.d64;
					val = s2.f32s.f1;
				}

				ps2 -= 1;
			} else if (((mlib_addr)pd & 7)) {
/* only pd unaligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd < (pend - 1);
				    pd += 2, ps1 += 2, ps2 += 2) {
					s1.d64 = *(mlib_d64 *)ps1;
					s2.d64 = *(mlib_d64 *)ps2;
					ARITH_FUN_F32(*pd, s1.f32s.f0,
					    s2.f32s.f0);
					ARITH_FUN_F32(*(pd + 1), s1.f32s.f1,
					    s2.f32s.f1);
				}
			} else {
/* all aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd < (pend - 1);
				    pd += 2, ps1 += 2, ps2 += 2) {
					s1.d64 = *(mlib_d64 *)ps1;
					s2.d64 = *(mlib_d64 *)ps2;
					ARITH_FUN_F32(d.f32s.f0, s1.f32s.f0,
					    s2.f32s.f0);
					ARITH_FUN_F32(d.f32s.f1, s1.f32s.f1,
					    s2.f32s.f1);
					*(mlib_d64 *)pd = d.d64;
				}
			}

			if (pd < pend) {
				ARITH_FUN_F32(*pd, *ps1, *ps2);
				pd++;
				ps1++;
				ps2++;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {

		for (j = 0; j < ysize; j++) {
			mlib_d64 *ps1 = (mlib_d64 *)psrc1;
			mlib_d64 *ps2 = (mlib_d64 *)psrc2;
			mlib_d64 *pd = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				ARITH_FUN_D64(*pd, *ps1, *ps2);
				pd++;
				ps1++;
				ps2++;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
