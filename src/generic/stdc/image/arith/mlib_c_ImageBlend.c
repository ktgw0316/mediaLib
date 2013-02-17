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

#pragma ident	"@(#)mlib_c_ImageBlend.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend - blend two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend(mlib_image       *dst,
 *                                  const mlib_image *src1,
 *                                  const mlib_image *src2,
 *                                  const mlib_image *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to alpha image
 *
 * RESTRICTION
 *      src1, src2, dst, and alpha must be the same type,
 *      the same size and the same number of channels.
 *      They can have one to four channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type. Optionally, alpha can be
 *      single-channel image while src1, src2, and dst are multi-channel images.
 *      MLIB_BYTE alpha coefficients are in Q8 format.
 *      MLIB_SHORT alpha coefficients are in Q15 format and must be positive.
 *      MLIB_USHORT alpha coefficients are in Q16 format.
 *      MLIB_INT alpha coefficients are in Q31 format and must be positive.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (1-alpha)*src2
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend = __mlib_ImageBlend

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend) mlib_ImageBlend
    __attribute__((weak, alias("__mlib_ImageBlend")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ONE_S16	0x7fff
#define	SIZE	512

/* *********************************************************** */

static void mlib_c_ImageBlendSA_U8(
    mlib_u8 *sa1,
    mlib_s32 slb1,
    mlib_u8 *sa2,
    mlib_s32 slb2,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_u8 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize);

static void mlib_c_ImageBlend_S16(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

static void mlib_c_ImageBlend_U16(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_u16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

static void mlib_c_ImageBlendSA_S16(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize);

static void mlib_c_ImageBlendSA_U16(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_u16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize);

static void mlib_c_ImageBlendSA_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize);

/* *********************************************************** */

mlib_status
__mlib_ImageBlend(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_image *alpha)
{
	mlib_s32 slb1, slb2, dlb, alb, xsize, ysize, nchan, achan;
	mlib_type dtype;
	mlib_u8 *sa1, *sa2, *da, *aa;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_CHECK(alpha);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);
	MLIB_IMAGE_TYPE_EQUAL(dst, alpha);
	MLIB_IMAGE_SIZE_EQUAL(dst, alpha);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);
	slb1 = mlib_ImageGetStride(src1);
	sa1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa2 = mlib_ImageGetData(src2);
	alb = mlib_ImageGetStride(alpha);
	aa = mlib_ImageGetData(alpha);
	achan = mlib_ImageGetChannels(alpha);

	if (nchan == achan) {

		if (dtype == MLIB_BYTE) {

			mlib_c_ImageBlendSA_U8((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2,
			    (mlib_u8 *)da, dlb,
			    (mlib_u8 *)aa, alb, xsize * nchan, ysize, 1);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_SHORT) {

			mlib_c_ImageBlend_S16((mlib_s16 *)sa1, slb1,
			    (mlib_s16 *)sa2, slb2,
			    (mlib_s16 *)da, dlb,
			    (mlib_s16 *)aa, alb, xsize * nchan, ysize);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_USHORT) {

			mlib_c_ImageBlend_U16((mlib_u16 *)sa1, slb1,
			    (mlib_u16 *)sa2, slb2,
			    (mlib_u16 *)da, dlb,
			    (mlib_u16 *)aa, alb, xsize * nchan, ysize);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_INT) {

/*
 * compiler's bug - the poor performance for function inline
 *
 *      mlib_c_ImageBlend_S32((mlib_s32 *) sa1, slb1,
 *                            (mlib_s32 *) sa2, slb2,
 *                            (mlib_s32 *) da, dlb,
 *                            (mlib_s32 *) aa, alb,
 *                            xsize*nchan, ysize);
 */

/* indices for x and y */
			mlib_s32 i, j;

/* variable for alpha coefficients */
			mlib_d64 fa, fshift = -1.0 / MLIB_S32_MIN;

			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < (xsize * nchan); i++) {
					fa = mlib_fabs((mlib_d64)((mlib_s32 *)
					    aa)[i]) * fshift;
					((mlib_s32 *)da)[i] =
					    (mlib_s32)(fa *
					    (mlib_d64)((mlib_s32 *)sa1)[i]
					    + (1 -
					    fa) *
					    (mlib_d64)((mlib_s32 *)sa2)[i]);
				}

				sa1 += slb1;
				sa2 += slb2;
				da += dlb;
				aa += alb;
			}

			return (MLIB_SUCCESS);
		}

	} else if (achan == 1) {

		if (dtype == MLIB_BYTE) {

			mlib_c_ImageBlendSA_U8((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2,
			    (mlib_u8 *)da, dlb,
			    (mlib_u8 *)aa, alb, xsize, ysize, nchan);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_SHORT) {

			mlib_c_ImageBlendSA_S16((mlib_s16 *)sa1, slb1,
			    (mlib_s16 *)sa2, slb2,
			    (mlib_s16 *)da, dlb,
			    (mlib_s16 *)aa, alb, xsize, ysize, nchan);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_USHORT) {

			mlib_c_ImageBlendSA_U16((mlib_u16 *)sa1, slb1,
			    (mlib_u16 *)sa2, slb2,
			    (mlib_u16 *)da, dlb,
			    (mlib_u16 *)aa, alb, xsize, ysize, nchan);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_INT) {

			mlib_c_ImageBlendSA_S32((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb,
			    (mlib_s32 *)aa, alb, xsize, ysize, nchan);
			return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_c_ImageBlendSA_U8(
    mlib_u8 *sa1,
    mlib_s32 slb1,
    mlib_u8 *sa2,
    mlib_s32 slb2,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_u8 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_f32 fa;

#define	table	mlib_U82F32

	if (csize == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = table[aa[i]];
				da[i] = (mlib_s32)(fa * table[sa1[i]]
				    + (256 - fa) * table[sa2[i]]) >> 8;
			}

			sa1 = sa1 + slb1;
			sa2 = sa2 + slb2;
			da = da + dlb;
			aa = aa + alb;
		}

		return;
	}

	if (csize == 2) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = table[aa[i]];
				da[2 * i] = (mlib_s32)(fa * table[sa1[2 * i]]
				    + (256 - fa) * table[sa2[2 * i]]) >> 8;
				da[2 * i + 1] =
				    (mlib_s32)(fa * table[sa1[2 * i + 1]]
				    + (256 - fa) * table[sa2[2 * i + 1]]) >> 8;
			}

			sa1 = sa1 + slb1;
			sa2 = sa2 + slb2;
			da = da + dlb;
			aa = aa + alb;
		}

		return;
	}

	if (csize == 3) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = table[aa[i]];
				da[3 * i] = (mlib_s32)(fa * table[sa1[3 * i]]
				    + (256 - fa) * table[sa2[3 * i]]) >> 8;
				da[3 * i + 1] =
				    (mlib_s32)(fa * table[sa1[3 * i + 1]]
				    + (256 - fa) * table[sa2[3 * i + 1]]) >> 8;
				da[3 * i + 2] =
				    (mlib_s32)(fa * table[sa1[3 * i + 2]]
				    + (256 - fa) * table[sa2[3 * i + 2]]) >> 8;
			}

			sa1 = sa1 + slb1;
			sa2 = sa2 + slb2;
			da = da + dlb;
			aa = aa + alb;
		}

		return;
	}

	if (csize == 4) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = table[aa[i]];
				da[4 * i] = (mlib_s32)(fa * table[sa1[4 * i]]
				    + (256 - fa) * table[sa2[4 * i]]) >> 8;
				da[4 * i + 1] =
				    (mlib_s32)(fa * table[sa1[4 * i + 1]]
				    + (256 - fa) * table[sa2[4 * i + 1]]) >> 8;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = table[aa[i]];
				da[4 * i + 2] =
				    (mlib_s32)(fa * table[sa1[4 * i + 2]]
				    + (256 - fa) * table[sa2[4 * i + 2]]) >> 8;
				da[4 * i + 3] =
				    (mlib_s32)(fa * table[sa1[4 * i + 3]]
				    + (256 - fa) * table[sa2[4 * i + 3]]) >> 8;
			}

			sa1 = sa1 + slb1;
			sa2 = sa2 + slb2;
			da = da + dlb;
			aa = aa + alb;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageBlend_S16(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_f32 fa;
	mlib_s32 buf[SIZE], num, col;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] = aa[col + i] & ONE_S16;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				fa = (mlib_f32)buf[i];
				da[col + i] =
				    ((mlib_s32)(fa * (mlib_f32)(sa1[col + i]
				    - sa2[col + i])) >> 15) + sa2[col + i];
			}
		}

		sa1 = (mlib_s16 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s16 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		aa = (mlib_s16 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageBlend_U16(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_u16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_f32 fa;
	mlib_s32 buf[SIZE], num, col;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] = aa[col + i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
/* to avoid overflow from 'mlib_s32' range */
				fa = (mlib_f32)buf[i] * (mlib_f32)0.5;
				da[col + i] =
				    ((mlib_s32)(fa * (mlib_f32)(sa1[col + i]
				    - sa2[col + i])) >> 15) + sa2[col + i];
			}
		}

		sa1 = (mlib_u16 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_u16 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_u16 *)((mlib_u8 *)da + dlb);
		aa = (mlib_u16 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */

#if 0

/*  See about the line 414. Compiler bug, too slow. */
void
mlib_c_ImageBlend_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 i, j;
	mlib_d64 fa, fshift = -1.0 / MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
			da[i] = (mlib_s32)(fa * (mlib_d64)sa1[i]
			    + (1 - fa) * (mlib_d64)sa2[i]);
		}

		sa1 = (mlib_s32 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s32 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_s32 *)((mlib_u8 *)da + dlb);
		aa = (mlib_s32 *)((mlib_u8 *)aa + alb);
	}
}

#endif /* 0 */

/* *********************************************************** */

void
mlib_c_ImageBlendSA_S16(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_f32 fa;
	mlib_s32 buf[4 * SIZE], num, col;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < csize * num; i++)
				buf[i] =
				    sa1[csize * col + i] - sa2[csize * col + i];
			if (csize == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)(aa[col + i] & ONE_S16);
					da[2 * (col + i)] =
					    ((mlib_s32)(fa * (mlib_f32)buf[2 *
					    i]) >> 15) + sa2[2 * (col + i)];
					da[2 * (col + i) + 1] =
					    ((mlib_s32)(fa * (mlib_f32)buf[2 *
					    i + 1]) >> 15) + sa2[2 * (col + i) +
					    1];
				}
			} else if (csize == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)(aa[col + i] & ONE_S16);
					da[3 * (col + i)] =
					    ((mlib_s32)(fa * (mlib_f32)buf[3 *
					    i]) >> 15) + sa2[3 * (col + i)];
					da[3 * (col + i) + 1] =
					    ((mlib_s32)(fa * (mlib_f32)buf[3 *
					    i + 1]) >> 15) + sa2[3 * (col + i) +
					    1];
					da[3 * (col + i) + 2] =
					    ((mlib_s32)(fa * (mlib_f32)buf[3 *
					    i + 2]) >> 15) + sa2[3 * (col + i) +
					    2];
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)(aa[col + i] & ONE_S16);
					da[4 * (col + i)] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i]) >> 15) + sa2[4 * (col + i)];
					da[4 * (col + i) + 1] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i + 1]) >> 15) + sa2[4 * (col + i) +
					    1];
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)(aa[col + i] & ONE_S16);
					da[4 * (col + i) + 2] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i + 2]) >> 15) + sa2[4 * (col + i) +
					    2];
					da[4 * (col + i) + 3] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i + 3]) >> 15) + sa2[4 * (col + i) +
					    3];
				}
			}
		}

		sa1 = (mlib_s16 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s16 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		aa = (mlib_s16 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageBlendSA_U16(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_u16 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_f32 fa;
	mlib_s32 buf[4 * SIZE], num, col;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < csize * num; i++)
				buf[i] =
				    sa1[csize * col + i] - sa2[csize * col + i];
			if (csize == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)aa[col +
					    i] * (mlib_f32)0.5;
					da[2 * (col + i)] =
					    ((mlib_s32)(fa * (mlib_f32)buf[2 *
					    i]) >> 15) + sa2[2 * (col + i)];
					da[2 * (col + i) + 1] =
					    ((mlib_s32)(fa * (mlib_f32)buf[2 *
					    i + 1]) >> 15) + sa2[2 * (col + i) +
					    1];
				}
			} else if (csize == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)aa[col +
					    i] * (mlib_f32)0.5;
					da[3 * (col + i)] =
					    ((mlib_s32)(fa * (mlib_f32)buf[3 *
					    i]) >> 15) + sa2[3 * (col + i)];
					da[3 * (col + i) + 1] =
					    ((mlib_s32)(fa * (mlib_f32)buf[3 *
					    i + 1]) >> 15) + sa2[3 * (col + i) +
					    1];
					da[3 * (col + i) + 2] =
					    ((mlib_s32)(fa * (mlib_f32)buf[3 *
					    i + 2]) >> 15) + sa2[3 * (col + i) +
					    2];
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)aa[col +
					    i] * (mlib_f32)0.5;
					da[4 * (col + i)] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i]) >> 15) + sa2[4 * (col + i)];
					da[4 * (col + i) + 1] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i + 1]) >> 15) + sa2[4 * (col + i) +
					    1];
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					fa = (mlib_f32)aa[col +
					    i] * (mlib_f32)0.5;
					da[4 * (col + i) + 2] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i + 2]) >> 15) + sa2[4 * (col + i) +
					    2];
					da[4 * (col + i) + 3] =
					    ((mlib_s32)(fa * (mlib_f32)buf[4 *
					    i + 3]) >> 15) + sa2[4 * (col + i) +
					    3];
				}
			}
		}

		sa1 = (mlib_u16 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_u16 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_u16 *)((mlib_u8 *)da + dlb);
		aa = (mlib_u16 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageBlendSA_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_d64 fa, fshift = -1.0 / MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		if (csize == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
				da[2 * i] = (mlib_s32)(fa * (mlib_d64)sa1[2 * i]
				    + (1 - fa) * (mlib_d64)sa2[2 * i]);
				da[2 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)sa1[2 * i + 1]
				    + (1 - fa) * (mlib_d64)sa2[2 * i + 1]);
			}
		} else if (csize == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
				da[3 * i] = (mlib_s32)(fa * (mlib_d64)sa1[3 * i]
				    + (1 - fa) * (mlib_d64)sa2[3 * i]);
				da[3 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)sa1[3 * i + 1]
				    + (1 - fa) * (mlib_d64)sa2[3 * i + 1]);
				da[3 * i + 2] =
				    (mlib_s32)(fa * (mlib_d64)sa1[3 * i + 2]
				    + (1 - fa) * (mlib_d64)sa2[3 * i + 2]);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
				da[4 * i] = (mlib_s32)(fa * (mlib_d64)sa1[4 * i]
				    + (1 - fa) * (mlib_d64)sa2[4 * i]);
				da[4 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i + 1]
				    + (1 - fa) * (mlib_d64)sa2[4 * i + 1]);
				da[4 * i + 2] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i + 2]
				    + (1 - fa) * (mlib_d64)sa2[4 * i + 2]);
				da[4 * i + 3] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i + 3]
				    + (1 - fa) * (mlib_d64)sa2[4 * i + 3]);
			}
		}

		sa1 = (mlib_s32 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s32 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_s32 *)((mlib_u8 *)da + dlb);
		aa = (mlib_s32 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */
