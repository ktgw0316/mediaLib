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

#pragma ident	"@(#)mlib_ImageConstMul_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstMul_Fp           - multiply an image with constants
 *      mlib_ImageConstMul_Fp_Inp       - multiply an image with constants
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstMul_Fp(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *consts)
 *      mlib_status mlib_ImageConstMul_Fp_Inp(mlib_image     *srcdst,
 *                                            const mlib_d64 *consts)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to be multiplied to each channel of the
 *              pixels
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = consts[c] * src[x][y][c]
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = consts[c] * srcdst[x][y][c]
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstMul_Fp = __mlib_ImageConstMul_Fp
#pragma weak mlib_ImageConstMul_Fp_Inp = __mlib_ImageConstMul_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstMul_Fp) mlib_ImageConstMul_Fp
    __attribute__((weak, alias("__mlib_ImageConstMul_Fp")));
__typeof__(__mlib_ImageConstMul_Fp_Inp) mlib_ImageConstMul_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageConstMul_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ARITH_FUN(x, c)	((x) * (c))

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

static void mlib_ImageConstMul_F32(
    mlib_f32 *dst,
    mlib_f32 *src,
    mlib_s32 dsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *consts);

static void mlib_ImageConstMul_D64(
    mlib_d64 *dst,
    mlib_d64 *src,
    mlib_s32 dsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *consts);

/* *********************************************************** */

mlib_status
__mlib_ImageConstMul_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_type type;
	void *psrc, *pdst;
	mlib_s32 slb, dlb, xsize, ysize, nchan;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	switch (type) {
	case MLIB_FLOAT:
		mlib_ImageConstMul_F32(pdst, psrc, xsize, ysize, nchan, slb / 4,
		    dlb / 4, consts);
		return (MLIB_SUCCESS);

	case MLIB_DOUBLE:
		mlib_ImageConstMul_D64(pdst, psrc, xsize, ysize, nchan, slb / 8,
		    dlb / 8, consts);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

static void
mlib_ImageConstMul_F32(
    mlib_f32 *dl,
    mlib_f32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *consts)
{
	mlib_f32 *sp, *dp, *pend;
	mlib_d64 *sp2, *dp2;
	mlib_s32 dsize = xsize * nchan;
	mlib_f32 dc[6], *c;
	mlib_s32 j, k;

	for (k = 0; k < nchan; k++) {
		dc[k] = (mlib_f32)consts[k];
	}

	for (k = nchan; k < 6; k++)
		dc[k] = dc[k - nchan];

	sp = sl;
	dp = dl;

	for (j = 0; j < ysize; j++) {
		pend = dp + dsize;

		c = dc;

		if ((mlib_addr)dp & 7) {
			(*dp++) = ARITH_FUN((*sp++), (*c++));
		}

		dp2 = (mlib_d64 *)dp;

/* nchan = 3, any aligning for src */
		if (nchan == 3) {
			mlib_f32 c0, c1, c2;

			c0 = c[0];
			c1 = c[1];
			c2 = c[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_f32 *)dp2 <= (pend - 6); dp2 += 3) {
				d64_2x32 d0, d1, d2;

				d0.f32s.f0 = ARITH_FUN(sp[0], c0);
				d0.f32s.f1 = ARITH_FUN(sp[1], c1);
				dp2[0] = d0.d64;

				d1.f32s.f0 = ARITH_FUN(sp[2], c2);
				d1.f32s.f1 = ARITH_FUN(sp[3], c0);
				dp2[1] = d1.d64;

				d2.f32s.f0 = ARITH_FUN(sp[4], c1);
				d2.f32s.f1 = ARITH_FUN(sp[5], c2);
				dp2[2] = d2.d64;

				sp += 6;
			}
		} else {
/* nchan != 3, all aligned */
			if (((mlib_addr)sp & 7) == 0) {
				mlib_f32 c0, c1, c2, c3;

				c0 = c[0];
				c1 = c[1];
				c2 = c[2];
				c3 = c[3];

				sp2 = (mlib_d64 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; (mlib_f32 *)dp2 <=
				    (pend - 4); dp2 += 2) {
					d64_2x32 s0, s1, d0, d1;

					s0.d64 = sp2[0];
					d0.f32s.f0 = ARITH_FUN(s0.f32s.f0, c0);
					d0.f32s.f1 = ARITH_FUN(s0.f32s.f1, c1);
					dp2[0] = d0.d64;

					s1.d64 = sp2[1];
					d1.f32s.f0 = ARITH_FUN(s1.f32s.f0, c2);
					d1.f32s.f1 = ARITH_FUN(s1.f32s.f1, c3);
					dp2[1] = d1.d64;

					sp2 += 2;
				}

				sp = (mlib_f32 *)sp2;
			} else {
/* nchan != 3, src unaligned */
				mlib_f32 c0, c1, c2, c3;

				c0 = c[0];
				c1 = c[1];
				c2 = c[2];
				c3 = c[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; (mlib_f32 *)dp2 <=
				    (pend - 4); dp2 += 2) {
					d64_2x32 d0, d1;

					d0.f32s.f0 = ARITH_FUN(sp[0], c0);
					d0.f32s.f1 = ARITH_FUN(sp[1], c1);
					dp2[0] = d0.d64;

					d1.f32s.f0 = ARITH_FUN(sp[2], c2);
					d1.f32s.f1 = ARITH_FUN(sp[3], c3);
					dp2[1] = d1.d64;

					sp += 4;
				}
			}
		}

		dp = (mlib_f32 *)dp2;
		while (dp < pend) {
			(*dp++) = ARITH_FUN((*sp++), (*c++));
		}

		sp = sl += slb;
		dp = dl += dlb;
	}
}

/* *********************************************************** */

static void
mlib_ImageConstMul_D64(
    mlib_d64 *dl,
    mlib_d64 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *consts)
{
	mlib_s32 i, j, k;

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
			mlib_d64 c = consts[k];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				dl[i * nchan + k] =
				    ARITH_FUN(sl[i * nchan + k], c);
			}
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageConstMul_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *consts)
{
	return (__mlib_ImageConstMul_Fp(srcdst, srcdst, consts));
}

/* *********************************************************** */
