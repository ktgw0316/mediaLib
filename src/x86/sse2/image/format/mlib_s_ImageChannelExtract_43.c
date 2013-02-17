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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageChannelExtract_43.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageChannelExtract_U8_43R_A16D1
 *      mlib_s_ImageChannelExtract_U8_43R_A16D2
 *      mlib_s_ImageChannelExtract_U8_43R_D1
 *      mlib_s_ImageChannelExtract_U8_43R
 *      mlib_s_ImageChannelExtract_S16_43R_A16D1
 *      mlib_s_ImageChannelExtract_S16_43R_A16D2
 *      mlib_s_ImageChannelExtract_S16_43R_D1
 *      mlib_s_ImageChannelExtract_S16_43R
 *      mlib_s_ImageChannelExtract_U8_43L_A16D1
 *      mlib_s_ImageChannelExtract_U8_43L_A16D2
 *      mlib_s_ImageChannelExtract_U8_43L_D1
 *      mlib_s_ImageChannelExtract_U8_43L
 *      mlib_s_ImageChannelExtract_S16_43L_A16D1
 *      mlib_s_ImageChannelExtract_S16_43L_A16D2
 *      mlib_s_ImageChannelExtract_S16_43L_D1
 *      mlib_s_ImageChannelExtract_S16_43L
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      src    pointer to source image data
 *      dst    pointer to destination image data
 *      slb    source image line stride in bytes
 *      dlb    destination image line stride in bytes
 *      dsize image data size in pixels
 *      xsize  image width in pixels
 *      ysize  image height in lines
 *      cmask channel mask
 *
 * DESCRIPTION
 *      extract the right or left 3 channels of a 4-channel image to
 *      a 3-channel image -- VIS version low level functions.
 *
 *      ABGR => BGR   (43R), or  RGBA => RGB  (43L)
 *
 * NOTE
 *      These functions are separated from mlib_s_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_s_ImageChannelExtract.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

/*
 * Both source and destination image data are 1-d vectors.
 */

void
mlib_s_ImageChannelExtract_U8_43R_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_s32 *sp;
	mlib_s32 n1;
	mlib_u8 *dp;

	sp = (mlib_s32 *)src;
	dp = (mlib_u8 *)dst;

	n1 = dsize;

	for (; n1 > 4; n1 -= 4) {
		*((mlib_s32 *)(dp + 0)) = sp[0] >> 8;
		*((mlib_s32 *)(dp + 3)) = sp[1] >> 8;
		*((mlib_s32 *)(dp + 6)) = sp[2] >> 8;
		*((mlib_s32 *)(dp + 9)) = sp[3] >> 8;
		dp += 12;
		sp += 4;
	}

	for (; n1 > 0; n1--) {
		*(dp + 0) = *((mlib_u8 *)sp + 1);
		*(dp + 1) = *((mlib_u8 *)sp + 2);
		*(dp + 2) = *((mlib_u8 *)sp + 3);
		dp += 3;
		sp += 1;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_U8_43R(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;

	sp = sl = (mlib_s32 *)src;
	dp = dl = (mlib_u8 *)dst;

	for (j = 0; j < ysize; j++) {
		for (n1 = xsize; n1 > 4; n1 -= 4) {
			*((mlib_s32 *)(dp + 0)) = sp[0] >> 8;
			*((mlib_s32 *)(dp + 3)) = sp[1] >> 8;
			*((mlib_s32 *)(dp + 6)) = sp[2] >> 8;
			*((mlib_s32 *)(dp + 9)) = sp[3] >> 8;
			dp += 12;
			sp += 4;
		}

		for (; n1 > 0; n1--) {
			*(dp + 0) = *((mlib_u8 *)sp + 1);
			*(dp + 1) = *((mlib_u8 *)sp + 2);
			*(dp + 2) = *((mlib_u8 *)sp + 3);
			dp += 3;
			sp += 1;
		}

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* more than 2 pixels */
#define	CHANNELEXTRACT_S16_43_G2A(ld, nsize, tf)	\
	for (; i <= (nsize - 4); i += 2) {		\
		x_sd0 = _mm_##ld##_si128(x_sp);		\
		x_sp++;					\
		mlib_u8 *pout = (mlib_u8 *)(&x_sd0);	\
		pout += tf;				\
		*((mlib_s64*)dp) = *((mlib_s64*)pout);	\
		dp += 3;				\
		pout += 8;				\
		*((mlib_s64*)dp) = *((mlib_s64*)pout);	\
		dp += 3;				\
	}

/* 2 pixels */
#define	CHANNELEXTRACT_S16_43_2A(ld, nsize, tf) 	\
	if (i <= (nsize - 2)) {				\
		x_sd0 = _mm_##ld##_si128(x_sp);		\
		x_sp++;					\
		mlib_u8 *pout = (mlib_u8 *)(&x_sd0);	\
		pout += tf;				\
		*((mlib_s64*)dp) = *((mlib_s64*)pout);	\
		dp += 3;				\
		pout += 8;				\
		*((mlib_s32*)dp) = *((mlib_s32*)pout);	\
		dp += 2;				\
		pout += 4;				\
		*((mlib_s16*)dp) = *((mlib_s16*)pout);	\
		dp++;					\
		i = i + 2;				\
	}

/* 1 pixels */
#define	CHANNELEXTRACT_S16_43_1A(ld, nsize, tf) 	\
	if (i <= (nsize - 1)) {				\
		x_sd0 = _mm_loadl_epi64(x_sp);		\
		x_sp = (__m128i *)(((__m64 *)x_sp) + 1);\
		mlib_u8 *pout = (mlib_u8 *)(&x_sd0);	\
		pout += tf;				\
		*((mlib_s32*)dp) = *((mlib_s32*)pout);	\
		dp += 2;				\
		pout += 4;				\
		*((mlib_s16*)dp) = *((mlib_s16*)pout);	\
		dp++;					\
		i = i + 1;				\
	}

/* more than 2 pixels, not 16byte aligned */
#define	CHANNELEXTRACT_S16_43_G2NA(ld, nsize, tf) 	\
	for (; i <= (nsize - 4); i += 2) {		\
		x_sd0 = _mm_loadu_si128(x_sp);		\
		x_sp++;					\
		mlib_u8 *pout = (mlib_u8*)(&x_sd0);	\
		pout += tf;				\
		*((mlib_s64*)dp) = *((mlib_s64*)pout);	\
		dp += 3;				\
		pout += 8;				\
		*((mlib_s64*)dp) = *((mlib_s64*)pout);	\
		dp += 3;				\
	}

#define	CHANNELEXTRACT_S16_43_2NA(ld, nsize, tf) 	\
	if (i <= (nsize - 2)) {				\
		x_sd0 = _mm_##ld##_si128(x_sp);		\
		x_sp++;					\
		mlib_u8 *pout = (mlib_u8 *)(&x_sd0);	\
		pout += tf;				\
		*((mlib_s64*)dp) = *((mlib_s64*)pout);	\
		dp += 3;				\
		pout += 8;				\
		*((mlib_s32*)dp) = *((mlib_s32*)pout);	\
		dp += 2;				\
		pout += 4;				\
		*((mlib_s16*)dp) = *((mlib_s16*)pout);	\
		dp++;					\
		i = i + 2;				\
	}

#define	CHANNELEXTRACT_S16_43_1NA(ld, nsize, tf) 	\
	if (i <= (nsize - 1)) {				\
		x_sd0 = _mm_loadl_epi64(x_sp);		\
		x_sp = (__m128i *)(((__m64 *)x_sp) + 1);\
		mlib_u8 *pout = (mlib_u8 *)(&x_sd0);	\
		pout += tf;				\
		*((mlib_s32*)dp) = *((mlib_s32*)pout);	\
		dp += 2;				\
		pout += 4;				\
		*((mlib_s16*)dp) = *((mlib_s16*)pout);	\
		dp++;					\
		i = i + 1;				\
	}

/* *********************************************************** */

/*
 * Both source and destination image data are 1-d vectors and
 * 16-byte aligned. And size is in 8-pixels.
 */

void
mlib_s_ImageChannelExtract_S16_43R_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m128i *x_sp, *x_dp;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_ts1, x_ts2;
	__m128i x_dd;
	mlib_s32 i;
	mlib_s16 *sp, *dp;
	mlib_s16 sd0, sd1, sd2;
	mlib_s16 dd;

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	CHANNELEXTRACT_S16_43_G2A(load, dsize, 2)
	CHANNELEXTRACT_S16_43_2A(load, dsize, 2)
	CHANNELEXTRACT_S16_43_1A(load, dsize, 2)

	_mm_empty();
}

/* *********************************************************** */

/*
 * Either source or destination image data are not 1-d vectors, but
 * they are 16-byte aligned. The xsize is multiple of 8.
 * slb and dlb are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_S16_43R_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_ts1, x_ts2;
	__m128i x_dd;
	mlib_s32 i, j;

	mlib_s16 *dp, *dl;
	dp = dl = dst;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0; j < ysize; j++) {

		i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S16_43_G2A(load, xsize, 2)
		CHANNELEXTRACT_S16_43_2A(load, xsize, 2)
		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
	_mm_empty();
}

/* *********************************************************** */

/*
 * Either source or destination data are not 16-byte aligned.
 */

void
mlib_s_ImageChannelExtract_S16_43R_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m128i *x_sp, *x_dp;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_ts1, x_ts2;
	__m128i x_dd;
	mlib_s16 *sp, *dp;
	mlib_s16 sd0, sd1, sd2;
	mlib_s16 dd;
	mlib_s32 i;

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;
	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	CHANNELEXTRACT_S16_43_G2NA(loadu, dsize, 2)
	CHANNELEXTRACT_S16_43_2NA(loadu, dsize, 2)
	CHANNELEXTRACT_S16_43_1NA(loadu, dsize, 2)
	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S16_43R(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_s_ImageChannelExtract_S16_43R_D1(sa, da, xsize);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/*
 * Both source and destination image data are 1-d vectors.
 */

void
mlib_s_ImageChannelExtract_U8_43L_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_s32 *sp;
	mlib_s32 n1;
	mlib_u8 *dp;

	sp = (mlib_s32 *)src;
	dp = (mlib_u8 *)dst;

	n1 = dsize;

	for (; n1 > 4; n1 -= 4) {
		*((mlib_s32 *)(dp + 0)) = sp[0];
		*((mlib_s32 *)(dp + 3)) = sp[1];
		*((mlib_s32 *)(dp + 6)) = sp[2];
		*((mlib_s32 *)(dp + 9)) = sp[3];
		dp += 12;
		sp += 4;
	}

	for (; n1 > 0; n1--) {
		*(dp + 0) = *((mlib_u8 *)sp + 0);
		*(dp + 1) = *((mlib_u8 *)sp + 1);
		*(dp + 2) = *((mlib_u8 *)sp + 2);
		dp += 3;
		sp += 1;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_U8_43L(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;

	sp = sl = (mlib_s32 *)src;
	dp = dl = (mlib_u8 *)dst;

	for (j = 0; j < ysize; j++) {
		for (n1 = xsize; n1 > 4; n1 -= 4) {
			*((mlib_s32 *)(dp + 0)) = sp[0];
			*((mlib_s32 *)(dp + 3)) = sp[1];
			*((mlib_s32 *)(dp + 6)) = sp[2];
			*((mlib_s32 *)(dp + 9)) = sp[3];
			dp += 12;
			sp += 4;
		}

		for (; n1 > 0; n1--) {
			*(dp + 0) = *((mlib_u8 *)sp + 0);
			*(dp + 1) = *((mlib_u8 *)sp + 1);
			*(dp + 2) = *((mlib_u8 *)sp + 2);
			dp += 3;
			sp += 1;
		}

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/*
 * Both source and destination image data are 1-d vectors and
 * 16-byte aligned. And dsize is multiple of 8.
 */

void
mlib_s_ImageChannelExtract_S16_43L_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m128i *x_sp, *x_dp;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_ts1, x_ts2;
	__m128i x_dd;
	mlib_s32 i;
	mlib_s16 *sp, *dp;
	mlib_s16 sd0, sd1, sd2;
	mlib_s16 dd;

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	CHANNELEXTRACT_S16_43_G2A(load, dsize, 0)
	CHANNELEXTRACT_S16_43_2A(load, dsize, 0)
	CHANNELEXTRACT_S16_43_1A(load, dsize, 0)
	_mm_empty();
}

/* *********************************************************** */

/*
 * Either source or destination image data are not 1-d vectors, but
 * they are 16-byte aligned. The xsize is multiple of 8.
 * And slb and dlb are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_S16_43L_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_ts1, x_ts2;
	__m128i x_dd;
	mlib_s32 i, j;

	mlib_s16 *dp, *dl;
	dp = dl = dst;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0; j < ysize; j++) {

		i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S16_43_G2A(load, xsize, 0)
		CHANNELEXTRACT_S16_43_2A(load, xsize, 0)
		CHANNELEXTRACT_S16_43_1A(load, xsize, 0)

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

/*
 * Either source or destination data are not 16-byte aligned.
 */

void
mlib_s_ImageChannelExtract_S16_43L_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m128i *x_sp, *x_dp;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_st0, x_st1, x_st2, x_st3;
	__m128i x_ts1, x_ts2;
	__m128i x_dd;
	mlib_s16 *sp, *dp;
	mlib_s16 sd0, sd1, sd2;
	mlib_s16 dd;
	mlib_s32 i;

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;
	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;


	i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	CHANNELEXTRACT_S16_43_G2NA(loadu, dsize, 0)
	CHANNELEXTRACT_S16_43_2NA(loadu, dsize, 0)
	CHANNELEXTRACT_S16_43_1NA(loadu, dsize, 0)
	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S16_43L(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_s_ImageChannelExtract_S16_43L_D1(sa, da, xsize);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
