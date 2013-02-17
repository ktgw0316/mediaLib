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

#pragma ident   "@(#)mlib_ImageDivShift.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivShift              - divide one image by another
 *      mlib_ImageDivShift1_Inp         - divide one image by another
 *      mlib_ImageDivShift2_Inp         - divide one image by another
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivShift(mlib_image       *dst,
 *                                     const mlib_image *src1,
 *                                     const mlib_image *src2,
 *                                     mlib_s32         shift)
 *      mlib_status mlib_ImageDivShift1_Inp(mlib_image       *src1dst,
 *                                          const mlib_image *src2,
 *                                          mlib_s32         shift)
 *      mlib_status mlib_ImageDivShift2_Inp(mlib_image       *src2dst,
 *                                          const mlib_image *src1,
 *                                          mlib_s32         shift)
 *
 * ARGUMENT
 *      src1     pointer to the first input image
 *      src2     pointer to the second input image
 *      dst      pointer to the output image
 *      src1dst  pointer to the first input and the output image
 *      src2dst  pointer to the second input and the output image
 *      shift    left shifting factor
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels. The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 *      0 <= shift <= 31
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = src1[x][y][c] / src2[x][y][c] * 2^shift
 *
 *      For in-place processing versions:
 *          mlib_ImageDivShift1_Inp:
 *              src1dst[x][y][c] = src1dst[x][y][c] / src2[x][y][c] * 2^shift
 *          mlib_ImageDivShift2_Inp:
 *              src2dst[x][y][c] = src1[x][y][c] / src2dst[x][y][c] * 2^shift
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageConstDiv.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivShift = __mlib_ImageDivShift
#pragma weak mlib_ImageDivShift1_Inp = __mlib_ImageDivShift1_Inp
#pragma weak mlib_ImageDivShift2_Inp = __mlib_ImageDivShift2_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivShift) mlib_ImageDivShift
    __attribute__((weak, alias("__mlib_ImageDivShift")));
__typeof__(__mlib_ImageDivShift1_Inp) mlib_ImageDivShift1_Inp
    __attribute__((weak, alias("__mlib_ImageDivShift1_Inp")));
__typeof__(__mlib_ImageDivShift2_Inp) mlib_ImageDivShift2_Inp
    __attribute__((weak, alias("__mlib_ImageDivShift2_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256


/* *********************************************************** */

#define	SAT_S16(DST, SRC)                                         \
	if (SRC > MLIB_S16_MAX)                                \
	    SRC = MLIB_S16_MAX;                                 \
	if (SRC < MLIB_S16_MIN)                                \
	    SRC = MLIB_S16_MIN;                                 \
	DST = (mlib_s32)SRC

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	if (SRC > MLIB_U16_MAX)                                \
	    SRC = MLIB_U16_MAX;                                 \
	if (SRC < MLIB_U16_MIN)                                \
	    SRC = MLIB_U16_MIN;                                 \
	DST = (mlib_s32)SRC

/* *********************************************************** */
#define	SAT_S32(DST, SRC)	\
	if (SRC > MLIB_S32_MAX)	\
		SRC = MLIB_S32_MAX;	\
	if (SRC < MLIB_S32_MIN)	\
		SRC = MLIB_S32_MIN;	\
	DST = (mlib_s32)SRC
/* *********************************************************** */

static mlib_status mlib_ImageDivShift_S16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

static mlib_status mlib_ImageDivShift_U16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

static mlib_status mlib_ImageDivShift_S32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

/* *********************************************************** */

mlib_status
__mlib_ImageDivShift(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_type type = mlib_ImageGetType(dst);

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	switch (type) {
	case MLIB_BYTE:
		mlib_ImageDivShift_U8(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	case MLIB_SHORT:
		mlib_ImageDivShift_S16(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_ImageDivShift_U16(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	case MLIB_INT:
		mlib_ImageDivShift_S32(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_S16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_s16 *dl, *sl1, *sl2, *dp, *sp1, *sp2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_s32 i, j, size, num;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 2;
	slb1 = mlib_ImageGetStride(src1) / 2;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 2;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

	for (j = 0; j < ysize; j++) {
		for (size = 0; size < xsize; size += num) {

			num = xsize - size;

			if (num > BUFF_SIZE)
				num = BUFF_SIZE;

			sp1 = sl1 + size;
			sp2 = sl2 + size;
			dp = dl + size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {

				mlib_s64 lval;

				if (sp2[i] != 0) {
					lval = ((mlib_s64)sp1[i] << shift)
						/ sp2[i];
					SAT_S16(dp[i], lval);
				} else if (sp1[i] == 0) {
					dp[i] = 0;
				} else if (sp1[i] > 0) {
					dp[i] = MLIB_S16_MAX;
				} else {
					dp[i] = MLIB_S16_MIN;
				}
			}
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_U16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_u16 *dl, *sl1, *sl2, *dp, *sp1, *sp2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_s32 i, j, size, num;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 2;
	slb1 = mlib_ImageGetStride(src1) / 2;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 2;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;


	for (j = 0; j < ysize; j++) {
		for (size = 0; size < xsize; size += num) {
			num = xsize - size;

			if (num > BUFF_SIZE)
				num = BUFF_SIZE;

			sp1 = sl1 + size;
			sp2 = sl2 + size;
			dp = dl + size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				mlib_s64 lval;

				if (sp2[i] != 0) {
					lval = ((mlib_s64)sp1[i] << shift)
						/ sp2[i];
					SAT_U16(dp[i], lval);
				} else if (sp1[i] > 0) {
					dp[i] = MLIB_U16_MAX;
				} else {
					dp[i] = MLIB_U16_MIN;
				}
			}
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_S32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_s32 *dl, *sl1, *sl2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 4;
	slb1 = mlib_ImageGetStride(src1) / 4;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 4;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;


	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			mlib_s64 lval;

			if (sl2[i] != 0) {
				lval = ((mlib_s64)sl1[i] << shift)
					/ sl2[i];
				SAT_S32(dl[i], lval);
			} else if (sl1[i] == 0) {
				dl[i] = 0;
			} else if (sl1[i] > 0) {
				dl[i] = MLIB_S32_MAX;
			} else {
				dl[i] = MLIB_S32_MIN;
			}
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageDivShift1_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 shift)
{
	return (__mlib_ImageDivShift(src1dst, src1dst, src2, shift));
}

/* *********************************************************** */

mlib_status
__mlib_ImageDivShift2_Inp(
    mlib_image *src2dst,
    const mlib_image *src1,
    mlib_s32 shift)
{
	return (__mlib_ImageDivShift(src2dst, src1, src2dst, shift));
}

/* *********************************************************** */
#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageConstDiv.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivShift = __mlib_ImageDivShift
#pragma weak mlib_ImageDivShift1_Inp = __mlib_ImageDivShift1_Inp
#pragma weak mlib_ImageDivShift2_Inp = __mlib_ImageDivShift2_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivShift) mlib_ImageDivShift
    __attribute__((weak, alias("__mlib_ImageDivShift")));
__typeof__(__mlib_ImageDivShift1_Inp) mlib_ImageDivShift1_Inp
    __attribute__((weak, alias("__mlib_ImageDivShift1_Inp")));
__typeof__(__mlib_ImageDivShift2_Inp) mlib_ImageDivShift2_Inp
    __attribute__((weak, alias("__mlib_ImageDivShift2_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256

#define	ZERO_DELTA	1e-16

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT16(DST, SRC)	DST = (mlib_s32) SRC

#define	SAT_U16(DST, SRC)                                           \
		DST = ((mlib_s32) ((SRC) - (mlib_f32)(0x80000000)))

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT16(DST, SRC)                                         \
	if (SRC >= MLIB_S16_MAX)                                \
	    SRC = MLIB_S16_MAX;                                 \
	if (SRC <= MLIB_S16_MIN)                                \
	    SRC = MLIB_S16_MIN;                                 \
	DST = (mlib_s32)SRC

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	if (SRC >= MLIB_U16_MAX)                                \
	    SRC = MLIB_U16_MAX;                                 \
	if (SRC <= MLIB_U16_MIN)                                \
	    SRC = MLIB_U16_MIN;                                 \
	DST = (mlib_s32)SRC

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static mlib_status mlib_ImageDivShift_S16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

static mlib_status mlib_ImageDivShift_U16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

static mlib_status mlib_ImageDivShift_S32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

/* *********************************************************** */

mlib_status
__mlib_ImageDivShift(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_type type = mlib_ImageGetType(dst);

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	switch (type) {
	case MLIB_BYTE:
		mlib_ImageDivShift_U8(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	case MLIB_SHORT:
		mlib_ImageDivShift_S16(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_ImageDivShift_U16(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	case MLIB_INT:
		mlib_ImageDivShift_S32(dst, src1, src2, shift);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_S16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_s32 buf1[BUFF_SIZE + 2];
	mlib_s32 buf2[BUFF_SIZE + 2];
	mlib_s32 buf3[BUFF_SIZE + 2];
	mlib_s16 *dl, *sl1, *sl2, *dp, *sp1, *sp2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_f32 fshift;
	mlib_d64 dshift;
	mlib_s32 i, j, size, num;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 2;
	slb1 = mlib_ImageGetStride(src1) / 2;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 2;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

#ifdef MLIB_USE_FTOI_CLAMPING
	SCALBN(dshift, 16 + shift);
#else /* MLIB_USE_FTOI_CLAMPING */
	SCALBN(dshift, shift);
#endif /* MLIB_USE_FTOI_CLAMPING */

	fshift = (mlib_f32)dshift;

	for (j = 0; j < ysize; j++) {
		for (size = 0; size < xsize; size += num) {

			num = xsize - size;

			if (num > BUFF_SIZE)
				num = BUFF_SIZE;

			sp1 = sl1 + size;
			sp2 = sl2 + size;
			dp = dl + size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				mlib_f32 fval;
				mlib_s32 ival;

				buf1[i] = sp1[i];
				buf2[i] = sp2[i];
				fval =
				    (fshift * (mlib_f32)buf1[i]) /
				    ((mlib_f32)buf2[i] + (mlib_f32)ZERO_DELTA);
				SAT16(ival, fval);
				buf3[i] = ival;
#ifdef MLIB_USE_FTOI_CLAMPING
				dp[i] = buf3[i] >> 16;
#else /* MLIB_USE_FTOI_CLAMPING */
				dp[i] = buf3[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
			}
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_U16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_s32 buf1[BUFF_SIZE + 2];
	mlib_s32 buf2[BUFF_SIZE + 2];
	mlib_s32 buf3[BUFF_SIZE + 2];
	mlib_u16 *dl, *sl1, *sl2, *dp, *sp1, *sp2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_f32 fshift;
	mlib_d64 dshift;
	mlib_s32 i, j, size, num;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 2;
	slb1 = mlib_ImageGetStride(src1) / 2;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 2;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

#ifdef MLIB_USE_FTOI_CLAMPING
	SCALBN(dshift, 16 + shift);
#else /* MLIB_USE_FTOI_CLAMPING */
	SCALBN(dshift, shift);
#endif /* MLIB_USE_FTOI_CLAMPING */

	fshift = (mlib_f32)dshift;

	for (j = 0; j < ysize; j++) {
		for (size = 0; size < xsize; size += num) {

			num = xsize - size;

			if (num > BUFF_SIZE)
				num = BUFF_SIZE;

			sp1 = sl1 + size;
			sp2 = sl2 + size;
			dp = dl + size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				mlib_f32 fval;
				mlib_s32 ival;

				buf1[i] = sp1[i];
				buf2[i] = sp2[i];
				fval =
				    (fshift * (mlib_f32)buf1[i]) /
				    ((mlib_f32)buf2[i] + (mlib_f32)ZERO_DELTA);
				SAT_U16(ival, fval);
				buf3[i] = ival;
#ifdef MLIB_USE_FTOI_CLAMPING
				dp[i] = (buf3[i] >> 16) ^ 0x8000;
#else /* MLIB_USE_FTOI_CLAMPING */
				dp[i] = buf3[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
			}
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_S32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_s32 *dl, *sl1, *sl2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_d64 ashift[1], dshift;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 4;
	slb1 = mlib_ImageGetStride(src1) / 4;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 4;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

	SCALBN(ashift[0], shift);

	for (j = 0; j < ysize; j++) {
		dshift = ashift[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			mlib_d64 sd, r_hi, r_lo, val;

			sd = (mlib_d64)sl2[i] + ZERO_DELTA;
			r_hi = (mlib_d64)(1.f / (mlib_f32)sd);
			r_lo = (1.0 - sd * r_hi) * r_hi;
			val = dshift * (mlib_d64)sl1[i] * (r_hi + r_lo);
			FLOAT2INT_CLAMP(dl[i], val);
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageDivShift1_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 shift)
{
	return (__mlib_ImageDivShift(src1dst, src1dst, src2, shift));
}

/* *********************************************************** */

mlib_status
__mlib_ImageDivShift2_Inp(
    mlib_image *src2dst,
    const mlib_image *src1,
    mlib_s32 shift)
{
	return (__mlib_ImageDivShift(src2dst, src1, src2dst, shift));
}

#endif
/* *********************************************************** */
