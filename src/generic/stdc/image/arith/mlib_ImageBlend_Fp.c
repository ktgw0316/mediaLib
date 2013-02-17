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

#pragma ident	"@(#)mlib_ImageBlend_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_Fp - blend two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend_Fp(mlib_image       *dst,
 *                                     const mlib_image *src1,
 *                                     const mlib_image *src2,
 *                                     const mlib_image *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to alpha image
 *
 * RESTRICTION
 *      src1, src2, dst, and alpha must be the same type and the same size.
 *      src1, src2, and dst must be the same number of channels.
 *      They can have one to four channels. They can be in MLIB_FLOAT or
 *      MLIB_DOUBLE data type. Optionally, alpha can be single-channel image
 *      while src1, src2, and dst are multi-channel images.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (1-alpha)*src2
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_Fp = __mlib_ImageBlend_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_Fp) mlib_ImageBlend_Fp
    __attribute__((weak, alias("__mlib_ImageBlend_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BLEND(dst, src1, src2, alpha)                           \
	(dst) = ((src1) - (src2)) * (alpha) + (src2)

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
__mlib_ImageBlend_Fp(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_image *alpha)
{
	mlib_type dtype;
	mlib_s32 nchan, dwidth, t_dwidth, t_awidth, dheight;
	mlib_s32 achan;
	mlib_s32 dlb, slb1, slb2, alb;
	mlib_s32 xsize, ysize;
	mlib_u8 *pdst, *psrc1, *psrc2, *palp;
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_CHECK(alpha);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);
	MLIB_IMAGE_SIZE_EQUAL(dst, alpha);
	MLIB_IMAGE_TYPE_EQUAL(dst, alpha);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, dwidth, dheight, dlb,
	    pdst);
	achan = mlib_ImageGetChannels(alpha);
	alb = mlib_ImageGetStride(alpha);
	palp = (void *)mlib_ImageGetData(alpha);

	slb1 = mlib_ImageGetStride(src1);
	psrc1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	psrc2 = mlib_ImageGetData(src2);

	ysize = dheight;

	if (dtype == MLIB_FLOAT) {
		t_dwidth = dwidth * nchan * 4;
		t_awidth = dwidth * achan * 4;
	} else if (dtype == MLIB_DOUBLE) {
		t_dwidth = dwidth * nchan * 8;
		t_awidth = dwidth * achan * 8;
	} else
		return (MLIB_FAILURE);

	if ((t_dwidth == slb1) && (t_dwidth == slb2) && (t_dwidth == dlb) &&
	    (t_awidth == alb)) {
		dwidth *= ysize;
		ysize = 1;
	}

	xsize = dwidth;

	if (nchan == achan) {

		if (dtype == MLIB_FLOAT) {

			xsize = nchan * dwidth;

			for (j = 0; j < ysize; j++) {
				mlib_f32 *ps1 = (mlib_f32 *)psrc1,
				    *ps2 = (mlib_f32 *)psrc2,
				    *pd = (mlib_f32 *)pdst,
				    *pa = (mlib_f32 *)palp,
				    *pend = (mlib_f32 *)pdst + xsize;
				mlib_addr addr_sum =
				    ((mlib_addr)ps1 & 7) +
				    ((mlib_addr)ps2 & 7) + ((mlib_addr)pd & 7);
				mlib_f32 val;
				d64_2_f32 s1, s2, d, al;

				if (addr_sum > 4) {
					BLEND(*pd, *ps1, *ps2, *pa);
					pd++;
					ps1++;
					ps2++;
					pa++;
				}

				if (((mlib_addr)pa & 7) == 0) {

/* only ps1 unaligned */
					if (((mlib_addr)ps1 & 7)) {
						val = *ps1;
						ps1 += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (; pd < pend - 1;
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							al.d64 =
							    *(mlib_d64 *)pa;
							BLEND(d.f32s.f0, val,
							    s2.f32s.f0,
							    al.f32s.f0);
							BLEND(d.f32s.f1,
							    s1.f32s.f0,
							    s2.f32s.f1,
							    al.f32s.f1);
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
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							al.d64 =
							    *(mlib_d64 *)pa;
							BLEND(d.f32s.f0,
							    s1.f32s.f0, val,
							    al.f32s.f0);
							BLEND(d.f32s.f1,
							    s1.f32s.f1,
							    s2.f32s.f0,
							    al.f32s.f1);
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
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							al.d64 =
							    *(mlib_d64 *)pa;
							BLEND(*pd, s1.f32s.f0,
							    s2.f32s.f0,
							    al.f32s.f0);
							BLEND(*(pd + 1),
							    s1.f32s.f1,
							    s2.f32s.f1,
							    al.f32s.f1);
						}

					} else {
/* all aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (; pd < pend - 1;
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							al.d64 =
							    *(mlib_d64 *)pa;
							BLEND(d.f32s.f0,
							    s1.f32s.f0,
							    s2.f32s.f0,
							    al.f32s.f0);
							BLEND(d.f32s.f1,
							    s1.f32s.f1,
							    s2.f32s.f1,
							    al.f32s.f1);
							*(mlib_d64 *)pd = d.d64;
						}
					}

					if (pd < pend) {
						BLEND(*pd, *ps1, *ps2, *pa);
					}

				} else {

/* only ps1 unaligned */
					if (((mlib_addr)ps1 & 7)) {
						val = *ps1;
						ps1 += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (; pd < pend - 1;
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							BLEND(d.f32s.f0, val,
							    s2.f32s.f0, *pa);
							BLEND(d.f32s.f1,
							    s1.f32s.f0,
							    s2.f32s.f1,
							    *(pa + 1));
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
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							BLEND(d.f32s.f0,
							    s1.f32s.f0, val,
							    *pa);
							BLEND(d.f32s.f1,
							    s1.f32s.f1,
							    s2.f32s.f0,
							    *(pa + 1));
							*(mlib_d64 *)pd = d.d64;
							val = s2.f32s.f1;
						}

						ps2 -= 1;
					} else if (((mlib_addr)pd & 7)) {
/* only pd unaligned */
						val = *pa;
						pa += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (; pd < pend - 1;
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							al.d64 =
							    *(mlib_d64 *)pa;
							BLEND(*pd, s1.f32s.f0,
							    s2.f32s.f0, val);
							BLEND(*(pd + 1),
							    s1.f32s.f1,
							    s2.f32s.f1,
							    al.f32s.f0);
							val = al.f32s.f1;
						}

						pa -= 1;
					} else {
/* all aligned */
						val = *pa;
						pa += 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (; pd < pend - 1;
						    pd += 2, ps1 += 2, ps2 +=
						    2, pa += 2) {
							s1.d64 =
							    *(mlib_d64 *)ps1;
							s2.d64 =
							    *(mlib_d64 *)ps2;
							al.d64 =
							    *(mlib_d64 *)pa;
							BLEND(d.f32s.f0,
							    s1.f32s.f0,
							    s2.f32s.f0, val);
							BLEND(d.f32s.f1,
							    s1.f32s.f1,
							    s2.f32s.f1,
							    al.f32s.f0);
							*(mlib_d64 *)pd = d.d64;
							val = al.f32s.f1;
						}

						pa -= 1;
					}

					if (pd < pend) {
						BLEND(*pd, *ps1, *ps2, *pa);
					}
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
				palp += alb;
			}

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_DOUBLE) {

			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize * nchan; i++) {
					mlib_d64 alp = ((mlib_d64 *)palp)[i];

					((mlib_d64 *)pdst)[i] =
					    (((mlib_d64 *)psrc1)[i] -
					    ((mlib_d64 *)psrc2)[i]) * alp +
					    ((mlib_d64 *)psrc2)[i];
				}

				psrc1 += slb1;
				psrc2 += slb2;
				pdst += dlb;
				palp += alb;
			}

			return (MLIB_SUCCESS);
		}

	} else if (achan == 1) {

		if (dtype == MLIB_FLOAT) {

			if (nchan == 2) {

				for (j = 0; j < ysize; j++) {
					mlib_f32 alp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						alp = ((mlib_f32 *)palp)[i];
						((mlib_f32 *)pdst)[2 * i] =
						    (((mlib_f32 *)psrc1)[2 *
						    i] -
						    ((mlib_f32 *)psrc2)[2 *
						    i]) * alp +
						    ((mlib_f32 *)psrc2)[2 * i];
						((mlib_f32 *)pdst)[2 * i + 1] =
						    (((mlib_f32 *)psrc1)[2 * i +
						    1] -
						    ((mlib_f32 *)psrc2)[2 * i +
						    1]) * alp +
						    ((mlib_f32 *)psrc2)[2 * i +
						    1];
					}

					psrc1 += slb1;
					psrc2 += slb2;
					pdst += dlb;
					palp += alb;
				}

				return (MLIB_SUCCESS);
			} else if (nchan == 3) {

				for (j = 0; j < ysize; j++) {
					mlib_f32 alp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						alp = ((mlib_f32 *)palp)[i];
						((mlib_f32 *)pdst)[3 * i] =
						    (((mlib_f32 *)psrc1)[3 *
						    i] -
						    ((mlib_f32 *)psrc2)[3 *
						    i]) * alp +
						    ((mlib_f32 *)psrc2)[3 * i];
						((mlib_f32 *)pdst)[3 * i + 1] =
						    (((mlib_f32 *)psrc1)[3 * i +
						    1] -
						    ((mlib_f32 *)psrc2)[3 * i +
						    1]) * alp +
						    ((mlib_f32 *)psrc2)[3 * i +
						    1];
						((mlib_f32 *)pdst)[3 * i + 2] =
						    (((mlib_f32 *)psrc1)[3 * i +
						    2] -
						    ((mlib_f32 *)psrc2)[3 * i +
						    2]) * alp +
						    ((mlib_f32 *)psrc2)[3 * i +
						    2];
					}

					psrc1 += slb1;
					psrc2 += slb2;
					pdst += dlb;
					palp += alb;
				}

				return (MLIB_SUCCESS);
			} else if (nchan == 4) {

				for (j = 0; j < ysize; j++) {
					mlib_f32 alp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						alp = ((mlib_f32 *)palp)[i];
						((mlib_f32 *)pdst)[4 * i] =
						    (((mlib_f32 *)psrc1)[4 *
						    i] -
						    ((mlib_f32 *)psrc2)[4 *
						    i]) * alp +
						    ((mlib_f32 *)psrc2)[4 * i];
						((mlib_f32 *)pdst)[4 * i + 1] =
						    (((mlib_f32 *)psrc1)[4 * i +
						    1] -
						    ((mlib_f32 *)psrc2)[4 * i +
						    1]) * alp +
						    ((mlib_f32 *)psrc2)[4 * i +
						    1];
						((mlib_f32 *)pdst)[4 * i + 2] =
						    (((mlib_f32 *)psrc1)[4 * i +
						    2] -
						    ((mlib_f32 *)psrc2)[4 * i +
						    2]) * alp +
						    ((mlib_f32 *)psrc2)[4 * i +
						    2];
						((mlib_f32 *)pdst)[4 * i + 3] =
						    (((mlib_f32 *)psrc1)[4 * i +
						    3] -
						    ((mlib_f32 *)psrc2)[4 * i +
						    3]) * alp +
						    ((mlib_f32 *)psrc2)[4 * i +
						    3];
					}

					psrc1 += slb1;
					psrc2 += slb2;
					pdst += dlb;
					palp += alb;
				}

				return (MLIB_SUCCESS);
			} else
				return (MLIB_FAILURE);

		} else if (dtype == MLIB_DOUBLE) {

			if (nchan == 2) {

				for (j = 0; j < ysize; j++) {
					mlib_d64 alp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						alp = ((mlib_d64 *)palp)[i];
						((mlib_d64 *)pdst)[2 * i] =
						    (((mlib_d64 *)psrc1)[2 *
						    i] -
						    ((mlib_d64 *)psrc2)[2 *
						    i]) * alp +
						    ((mlib_d64 *)psrc2)[2 * i];
						((mlib_d64 *)pdst)[2 * i + 1] =
						    (((mlib_d64 *)psrc1)[2 * i +
						    1] -
						    ((mlib_d64 *)psrc2)[2 * i +
						    1]) * alp +
						    ((mlib_d64 *)psrc2)[2 * i +
						    1];
					}

					psrc1 += slb1;
					psrc2 += slb2;
					pdst += dlb;
					palp += alb;
				}

				return (MLIB_SUCCESS);
			} else if (nchan == 3) {

				for (j = 0; j < ysize; j++) {
					mlib_d64 alp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						alp = ((mlib_d64 *)palp)[i];
						((mlib_d64 *)pdst)[3 * i] =
						    (((mlib_d64 *)psrc1)[3 *
						    i] -
						    ((mlib_d64 *)psrc2)[3 *
						    i]) * alp +
						    ((mlib_d64 *)psrc2)[3 * i];
						((mlib_d64 *)pdst)[3 * i + 1] =
						    (((mlib_d64 *)psrc1)[3 * i +
						    1] -
						    ((mlib_d64 *)psrc2)[3 * i +
						    1]) * alp +
						    ((mlib_d64 *)psrc2)[3 * i +
						    1];
						((mlib_d64 *)pdst)[3 * i + 2] =
						    (((mlib_d64 *)psrc1)[3 * i +
						    2] -
						    ((mlib_d64 *)psrc2)[3 * i +
						    2]) * alp +
						    ((mlib_d64 *)psrc2)[3 * i +
						    2];
					}

					psrc1 += slb1;
					psrc2 += slb2;
					pdst += dlb;
					palp += alb;
				}

				return (MLIB_SUCCESS);
			} else if (nchan == 4) {

				for (j = 0; j < ysize; j++) {
					mlib_d64 alp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						alp = ((mlib_d64 *)palp)[i];
						((mlib_d64 *)pdst)[4 * i] =
						    (((mlib_d64 *)psrc1)[4 *
						    i] -
						    ((mlib_d64 *)psrc2)[4 *
						    i]) * alp +
						    ((mlib_d64 *)psrc2)[4 * i];
						((mlib_d64 *)pdst)[4 * i + 1] =
						    (((mlib_d64 *)psrc1)[4 * i +
						    1] -
						    ((mlib_d64 *)psrc2)[4 * i +
						    1]) * alp +
						    ((mlib_d64 *)psrc2)[4 * i +
						    1];
						((mlib_d64 *)pdst)[4 * i + 2] =
						    (((mlib_d64 *)psrc1)[4 * i +
						    2] -
						    ((mlib_d64 *)psrc2)[4 * i +
						    2]) * alp +
						    ((mlib_d64 *)psrc2)[4 * i +
						    2];
						((mlib_d64 *)pdst)[4 * i + 3] =
						    (((mlib_d64 *)psrc1)[4 * i +
						    3] -
						    ((mlib_d64 *)psrc2)[4 * i +
						    3]) * alp +
						    ((mlib_d64 *)psrc2)[4 * i +
						    3];
					}

					psrc1 += slb1;
					psrc2 += slb2;
					pdst += dlb;
					palp += alb;
				}

				return (MLIB_SUCCESS);
			} else
				return (MLIB_FAILURE);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
