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

#pragma ident	"@(#)mlib_ImageAbs_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAbs_Fp - absolute value of each Fp pixel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAbs_Fp(mlib_image       *dst,
 *                                   const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT,
 *      or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      dst(i, j) = |src(i, j)|
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAbs_Fp = __mlib_ImageAbs_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAbs_Fp) mlib_ImageAbs_Fp
    __attribute__((weak, alias("__mlib_ImageAbs_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ARITH_FUN(x)	mlib_fabs(x)

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
__mlib_ImageAbs_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_type dtype;
	mlib_u8 *psrc, *pdst;
	mlib_s32 j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	xsize *= nchan;

	if (dtype == MLIB_FLOAT) {

		for (j = 0; j < ysize; j++) {
			mlib_f32 *ps = (mlib_f32 *)psrc,
			    *pd = (mlib_f32 *)pdst,
			    *pend = (mlib_f32 *)pdst + xsize;

			if ((mlib_addr)pd & 7) {
				*pd = (mlib_f32)ARITH_FUN(*ps);
				ps++;
				pd++;
			}

			if (((mlib_addr)ps & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd < (pend - 1); pd += 2, ps += 2) {
					d64_2_f32 d;

					d.d64 = *(mlib_d64 *)ps;
					d.f32s.f0 =
					    (mlib_f32)ARITH_FUN(d.f32s.f0);
					d.f32s.f1 =
					    (mlib_f32)ARITH_FUN(d.f32s.f1);
					*(mlib_d64 *)pd = d.d64;
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd < (pend - 1); pd += 2, ps += 2) {
					d64_2_f32 d;

					d.f32s.f0 = (mlib_f32)ARITH_FUN(*ps);
					d.f32s.f1 =
					    (mlib_f32)ARITH_FUN(*(ps + 1));
					*(mlib_d64 *)pd = d.d64;
				}
			}

			if (pd < pend)
				*pd = (mlib_f32)ARITH_FUN(*ps);
			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_DOUBLE) {

		for (j = 0; j < ysize; j++) {
			mlib_d64 *ps = (mlib_d64 *)psrc,
			    *pd = (mlib_d64 *)pdst,
			    *pend = (mlib_d64 *)pdst + xsize;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; pd < pend; pd++, ps++) {
				mlib_d64 d;

				d = (mlib_d64)ARITH_FUN(*ps);
				*(mlib_d64 *)pd = d;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
