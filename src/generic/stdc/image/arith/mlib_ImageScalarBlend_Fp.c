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

#pragma ident	"@(#)mlib_ImageScalarBlend_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageScalarBlend_Fp - blend two images with a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScalarBlend_Fp(mlib_image       *dst,
 *                                           const mlib_image *src1,
 *                                           const mlib_image *src2,
 *                                           const mlib_d64   *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to array of alpha coefficients
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (1-alpha)*src2,
 *      (0 <= alpha <= 1)
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScalarBlend_Fp = __mlib_ImageScalarBlend_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScalarBlend_Fp) mlib_ImageScalarBlend_Fp
    __attribute__((weak, alias("__mlib_ImageScalarBlend_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _MSC_VER

/* *********************************************************** */

#define	SCALAR_BLEND(dst, src1, src2, alpha, _1malpha)          \
	(dst) = ((src1) - (src2)) * (alpha) + (src2)

#else /* _MSC_VER */

/* *********************************************************** */

#define	SCALAR_BLEND(dst, src1, src2, alpha, _1malpha)          \
	(dst) = (src1) * alpha + (src2) * _1malpha
#endif /* _MSC_VER */

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
__mlib_ImageScalarBlend_Fp(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_d64 *alpha)
{
	mlib_type dtype;
	mlib_s32 nchan;
	mlib_s32 dwidth, t_dwidth = 0;
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

	if (alpha == NULL)
		return (MLIB_FAILURE);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, dwidth, dheight, dlb,
	    pdst);
	slb1 = mlib_ImageGetStride(src1);
	psrc1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	psrc2 = mlib_ImageGetData(src2);

	for (i = 0; i < nchan; i++) {
		if (alpha[i] < 0.0 || alpha[i] > 1.0) {
			return (MLIB_FAILURE);
		}
	}

	ysize = dheight;
	xsize = dwidth * nchan;

	if (dtype == MLIB_FLOAT)
		t_dwidth = xsize * 4;
	else if (dtype == MLIB_DOUBLE)
		t_dwidth = xsize * 8;

	if ((t_dwidth == slb1) && (t_dwidth == slb2) && (t_dwidth == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

	if (dtype == MLIB_FLOAT) {
		mlib_f32 alp1, alp2;
		mlib_f32 _1ma1, _1ma2;

		if ((nchan == 1) || (nchan == 2)) {

			if (nchan == 1) {
				alp1 = alp2 = (mlib_f32)alpha[0];
				_1ma1 = _1ma2 = (mlib_f32)(1.0 - alpha[0]);
			} else {
				alp1 = (mlib_f32)alpha[0];
				alp2 = (mlib_f32)alpha[1];
				_1ma1 = (mlib_f32)(1.0 - alpha[0]);
				_1ma2 = (mlib_f32)(1.0 - alpha[1]);
			}

			for (j = 0; j < ysize; j++) {
				mlib_f32 *ps1 = (mlib_f32 *)psrc1,
				    *ps2 = (mlib_f32 *)psrc2,
				    *pd = (mlib_f32 *)pdst,
				    *pend = (mlib_f32 *)pdst + xsize;
				mlib_addr addr_sum =
				    ((mlib_addr)ps1 & 7) +
				    ((mlib_addr)ps2 & 7) + ((mlib_addr)pd & 7);
				mlib_f32 val;
				mlib_f32 alpha1, alpha2;
				mlib_f32 _1malpha1, _1malpha2;
				d64_2_f32 s1, s2, d;

				if (addr_sum > 4) {
					SCALAR_BLEND(*pd, *ps1, *ps2, alp1,
					    _1ma1);
					pd++;
					ps1++;
					ps2++;
					alpha1 = alp2;
					alpha2 = alp1;
					_1malpha1 = _1ma2;
					_1malpha2 = _1ma1;
				} else {
					alpha1 = alp1;
					alpha2 = alp2;
					_1malpha1 = _1ma1;
					_1malpha2 = _1ma2;
				}

/* only ps1 unaligned */
				if (((mlib_addr)ps1 & 7)) {
					val = *ps1;
					ps1 += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; pd < pend - 1;
					    pd += 2, ps1 += 2, ps2 += 2) {
						s1.d64 = *(mlib_d64 *)ps1;
						s2.d64 = *(mlib_d64 *)ps2;
						SCALAR_BLEND(d.f32s.f0, val,
						    s2.f32s.f0, alpha1,
						    _1malpha1);
						SCALAR_BLEND(d.f32s.f1,
						    s1.f32s.f0, s2.f32s.f1,
						    alpha2, _1malpha2);
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
					for (; pd < pend - 1;
					    pd += 2, ps1 += 2, ps2 += 2) {
						s1.d64 = *(mlib_d64 *)ps1;
						s2.d64 = *(mlib_d64 *)ps2;
						SCALAR_BLEND(d.f32s.f0,
						    s1.f32s.f0, val, alpha1,
						    _1malpha1);
						SCALAR_BLEND(d.f32s.f1,
						    s1.f32s.f1, s2.f32s.f0,
						    alpha2, _1malpha2);
						*(mlib_d64 *)pd = d.d64;
						val = s2.f32s.f1;
					}

					ps2 -= 1;
				} else if (((mlib_addr)pd & 7)) {
/* only pd unaligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; pd < pend - 1;
					    pd += 2, ps1 += 2, ps2 += 2) {
						s1.d64 = *(mlib_d64 *)ps1;
						s2.d64 = *(mlib_d64 *)ps2;
						SCALAR_BLEND(*pd, s1.f32s.f0,
						    s2.f32s.f0, alpha1,
						    _1malpha1);
						SCALAR_BLEND(*(pd + 1),
						    s1.f32s.f1, s2.f32s.f1,
						    alpha2, _1malpha2);
					}

				} else {
/* all aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; pd < pend - 1;
					    pd += 2, ps1 += 2, ps2 += 2) {
						s1.d64 = *(mlib_d64 *)ps1;
						s2.d64 = *(mlib_d64 *)ps2;
						SCALAR_BLEND(d.f32s.f0,
						    s1.f32s.f0, s2.f32s.f0,
						    alpha1, _1malpha1);
						SCALAR_BLEND(d.f32s.f1,
						    s1.f32s.f1, s2.f32s.f1,
						    alpha2, _1malpha2);
						*(mlib_d64 *)pd = d.d64;
					}
				}

				if (pd < pend) {
					SCALAR_BLEND(*pd, *ps1, *ps2, alpha1,
					    _1malpha1);
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}

		} else if (nchan == 3) {
			for (j = 0; j < ysize; j++) {
				mlib_f32 alp1 = (mlib_f32)alpha[0];
				mlib_f32 alp2 = (mlib_f32)alpha[1];
				mlib_f32 alp3 = (mlib_f32)alpha[2];
				mlib_f32 _1ma1 = (mlib_f32)(1.0 - alpha[0]);
				mlib_f32 _1ma2 = (mlib_f32)(1.0 - alpha[1]);
				mlib_f32 _1ma3 = (mlib_f32)(1.0 - alpha[2]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i += 3) {
					((mlib_f32 *)pdst)[i] =
					    ((mlib_f32 *)psrc1)[i] * alp1 +
					    ((mlib_f32 *)psrc2)[i] * _1ma1;
					((mlib_f32 *)pdst)[i + 1] =
					    ((mlib_f32 *)psrc1)[i + 1] * alp2 +
					    ((mlib_f32 *)psrc2)[i + 1] * _1ma2;
					((mlib_f32 *)pdst)[i + 2] =
					    ((mlib_f32 *)psrc1)[i + 2] * alp3 +
					    ((mlib_f32 *)psrc2)[i + 2] * _1ma3;
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}

		} else {
			for (j = 0; j < ysize; j++) {
				mlib_f32 alp1 = (mlib_f32)alpha[0];
				mlib_f32 alp2 = (mlib_f32)alpha[1];
				mlib_f32 alp3 = (mlib_f32)alpha[2];
				mlib_f32 alp4 = (mlib_f32)alpha[3];
				mlib_f32 _1ma1 = (mlib_f32)(1.0 - alpha[0]);
				mlib_f32 _1ma2 = (mlib_f32)(1.0 - alpha[1]);
				mlib_f32 _1ma3 = (mlib_f32)(1.0 - alpha[2]);
				mlib_f32 _1ma4 = (mlib_f32)(1.0 - alpha[3]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i += 4) {
					((mlib_f32 *)pdst)[i] =
					    ((mlib_f32 *)psrc1)[i] * alp1 +
					    ((mlib_f32 *)psrc2)[i] * _1ma1;
					((mlib_f32 *)pdst)[i + 1] =
					    ((mlib_f32 *)psrc1)[i + 1] * alp2 +
					    ((mlib_f32 *)psrc2)[i + 1] * _1ma2;
					((mlib_f32 *)pdst)[i + 2] =
					    ((mlib_f32 *)psrc1)[i + 2] * alp3 +
					    ((mlib_f32 *)psrc2)[i + 2] * _1ma3;
					((mlib_f32 *)pdst)[i + 3] =
					    ((mlib_f32 *)psrc1)[i + 3] * alp4 +
					    ((mlib_f32 *)psrc2)[i + 3] * _1ma4;
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_DOUBLE) {

		if (nchan == 1) {
			mlib_d64 alp0 = alpha[0];
			mlib_d64 _1ma0 = 1 - alp0;

			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					((mlib_d64 *)pdst)[i] =
					    ((mlib_d64 *)psrc1)[i] * alp0 +
					    ((mlib_d64 *)psrc2)[i] * _1ma0;
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}

		} else if (nchan == 2) {
			mlib_d64 alp0 = alpha[0];
			mlib_d64 alp1 = alpha[1];
			mlib_d64 _1ma0 = 1 - alp0;
			mlib_d64 _1ma1 = 1 - alp1;

			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i += 2) {
					((mlib_d64 *)pdst)[i] =
					    ((mlib_d64 *)psrc1)[i] * alp0 +
					    ((mlib_d64 *)psrc2)[i] * _1ma0;
					((mlib_d64 *)pdst)[i + 1] =
					    ((mlib_d64 *)psrc1)[i + 1] * alp1 +
					    ((mlib_d64 *)psrc2)[i + 1] * _1ma1;
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}

		} else if (nchan == 3) {
			mlib_d64 alp0 = alpha[0];
			mlib_d64 alp1 = alpha[1];
			mlib_d64 alp2 = alpha[2];
			mlib_d64 _1ma0 = 1 - alp0;
			mlib_d64 _1ma1 = 1 - alp1;
			mlib_d64 _1ma2 = 1 - alp2;

			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i += 3) {
					((mlib_d64 *)pdst)[i] =
					    ((mlib_d64 *)psrc1)[i] * alp0 +
					    ((mlib_d64 *)psrc2)[i] * _1ma0;
					((mlib_d64 *)pdst)[i + 1] =
					    ((mlib_d64 *)psrc1)[i + 1] * alp1 +
					    ((mlib_d64 *)psrc2)[i + 1] * _1ma1;
					((mlib_d64 *)pdst)[i + 2] =
					    ((mlib_d64 *)psrc1)[i + 2] * alp2 +
					    ((mlib_d64 *)psrc2)[i + 2] * _1ma2;
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}

		} else {
			mlib_d64 alp0 = alpha[0];
			mlib_d64 alp1 = alpha[1];
			mlib_d64 alp2 = alpha[2];
			mlib_d64 alp3 = alpha[3];
			mlib_d64 _1ma0 = 1 - alp0;
			mlib_d64 _1ma1 = 1 - alp1;
			mlib_d64 _1ma2 = 1 - alp2;
			mlib_d64 _1ma3 = 1 - alp3;

			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i += 4) {
					((mlib_d64 *)pdst)[i] =
					    ((mlib_d64 *)psrc1)[i] * alp0 +
					    ((mlib_d64 *)psrc2)[i] * _1ma0;
					((mlib_d64 *)pdst)[i + 1] =
					    ((mlib_d64 *)psrc1)[i + 1] * alp1 +
					    ((mlib_d64 *)psrc2)[i + 1] * _1ma1;
					((mlib_d64 *)pdst)[i + 2] =
					    ((mlib_d64 *)psrc1)[i + 2] * alp2 +
					    ((mlib_d64 *)psrc2)[i + 2] * _1ma2;
					((mlib_d64 *)pdst)[i + 3] =
					    ((mlib_d64 *)psrc1)[i + 3] * alp3 +
					    ((mlib_d64 *)psrc2)[i + 3] * _1ma3;
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
