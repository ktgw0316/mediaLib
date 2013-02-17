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

#pragma ident	"@(#)mlib_s_ImageChannelCopy_f.c	9.6	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_s_ImageChannelCopy_U8_2_A16D1
 *  mlib_s_ImageChannelCopy_U8_2_A16D2
 *  mlib_s_ImageChannelCopy_U8_2
 *  mlib_s_ImageChannelCopy_U8_3_A16D1
 *  mlib_s_ImageChannelCopy_U8_3_A16D2
 *  mlib_s_ImageChannelCopy_U8_3
 *  mlib_s_ImageChannelCopy_U8_4_A16D1
 *  mlib_s_ImageChannelCopy_U8_4_A16D2
 *  mlib_s_ImageChannelCopy_U8_4
 *  mlib_s_ImageChannelCopy_S16_2_A16D1
 *  mlib_s_ImageChannelCopy_S16_2_A16D2
 *  mlib_s_ImageChannelCopy_S16_2
 *  mlib_s_ImageChannelCopy_S16_3_A16D1
 *  mlib_s_ImageChannelCopy_S16_3_A16D2
 *  mlib_s_ImageChannelCopy_S16_3
 *  mlib_s_ImageChannelCopy_S16_4_A16D1
 *  mlib_s_ImageChannelCopy_S16_4_A16D2
 *  mlib_s_ImageChannelCopy_S16_4D1
 *  mlib_s_ImageChannelCopy_S16_4
 *  mlib_s_ImageChannelCopy_u8
 *  mlib_s_ImageChannelCopy_s16
 *  mlib_s_ImageChannelCopy_s32
 *  mlib_s_ImageChannelCopy_d64
 *
 * DESCRIPTION
 *  Copy the selected channels of the source image into the
 *  destination image.
 *
 * NOTE
 *  These functions are separated from mlib_s_ImageChannelCopy.c
 *  for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_s_ImageChannelCopy.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

/* 32 bytes */
#define	MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(ld1, ld2, st)	\
	__m128i x_sd1, x_sd2, x_dd1, x_dd2;		\
	x_sd = _mm_##ld1##_si128(x_sp);			\
	x_sd1 = _mm_##ld1##_si128(x_sp + 1);		\
	x_dd = _mm_##ld2##_si128(x_dp);			\
	x_dd1 = _mm_##ld2##_si128(x_dp + 1);		\
	x_sd0 = _mm_and_si128(chanmask, x_sd);		\
	x_sd2 = _mm_and_si128(chanmask, x_sd1);		\
	x_dd0 = _mm_andnot_si128(chanmask, x_dd);	\
	x_dd2 = _mm_andnot_si128(chanmask, x_dd1);	\
	x_dd = _mm_or_si128(x_dd0, x_sd0);		\
	x_dd1 = _mm_or_si128(x_dd2, x_sd2);		\
	_mm_##st##_si128(x_dp, x_dd);			\
	x_dp++;						\
	_mm_##st##_si128(x_dp, x_dd1);			\
	x_dp++;						\
	x_sp++;						\
	x_sp++

/* 16 bytes */
#define	MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(ld1, ld2, st)	\
	x_sd = _mm_##ld1##_si128(x_sp);			\
	x_dd = _mm_##ld2##_si128(x_dp);			\
	x_sd0 = _mm_and_si128(chanmask, x_sd);		\
	x_dd0 = _mm_andnot_si128(chanmask, x_dd);	\
	x_dd = _mm_or_si128(x_dd0, x_sd0);		\
	_mm_##st##_si128(x_dp, x_dd);			\
	x_sp++;						\
	x_dp++

/* 8 bytes */
#define	MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(ld)	\
	x_sd = _mm_##ld##_si128(x_sp);			\
	x_dd = _mm_loadl_epi64(x_dp);			\
	x_sd0 = _mm_and_si128(chanmask, x_sd);		\
	x_dd0 = _mm_andnot_si128(chanmask, x_dd);	\
	x_dd = _mm_or_si128(x_dd0, x_sd0);		\
	_mm_storel_epi64(x_dp, x_dd);           \
	x_sp = (__m128i *)((mlib_u8 *)x_sp + 8); \
	x_dp = (__m128i *)((mlib_u8 *)x_dp + 8)

/* 4 bytes */
#define	MLIB_S_IMAGECHANNELCOPY24_U8_A4		\
	sd = *sp;				\
	dd = *dp;				\
	sd &= cm0;				\
	dd &= ~cm0;				\
	dd |= sd;				\
	*dp = dd;				\
	sp++;					\
	dp++

/* 2 bytes */
#define	MLIB_S_IMAGECHANNELCOPY24_U8_A2		\
	sds = *sps;				\
	dds = *dps;				\
	sds &= cm1;				\
	dds &= ~cm1;				\
	dds |= sds;				\
	*dps = dds;				\
	sps++;					\
	dps++

/* *********************************************************** */

/* channel mask const */
const mlib_u32 channelmasku82[2] = {0xff00ff00, 0x00ff00ff};
const mlib_u32 channelmasku82_1[2] = {0xff00, 0x00ff};

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_2_A16D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	mlib_u32 *sp;
	mlib_u16 *sps;

/* aligned dst address */
	mlib_u32 *dp;
	mlib_u16 *dps;

/* src data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;
	mlib_u16 sds, dds;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	const mlib_s32 cm0 = channelmasku82[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);
	const mlib_s32 cm1 = channelmasku82_1[cmask - 1];

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;

	if ((((mlib_addr)src & 15) == 0) &&
	    (((mlib_addr)dst & 15) == 0)) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
		}

	} else { /* if (((mlib_addr)src & 15) == 0) */

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
			    loadu, loadu, storeu);
		}
	}

/* 4-pixel */
	if (i < dsize - 3) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
		i += 4;
	}

/* 2-pixel */
	sp = (mlib_u32 *)x_sp;
	dp = (mlib_u32 *)x_dp;
	if (i < dsize - 1) {
		MLIB_S_IMAGECHANNELCOPY24_U8_A4;
		i += 2;
	}

/* 1-pixel */
	sps = (mlib_u16 *)sp;
	dps = (mlib_u16 *)dp;
	if (i < dsize) {
		MLIB_S_IMAGECHANNELCOPY24_U8_A2;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_2_A16D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;
	mlib_u16 *sps;

/* aligned dst address */
	mlib_u32 *dp;
	mlib_u16 *dps;

/* src data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;
	mlib_u16 sds, dds;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	const mlib_s32 cm0 = channelmasku82[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);
	const mlib_s32 cm1 = channelmasku82_1[cmask - 1];

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

/* row loop */
	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
		}

/* 4-pixel */
		if (i < xsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
			i += 4;
		}

/* 2-pixel */
		sp = (mlib_u32 *)x_sp;
		dp = (mlib_u32 *)x_dp;

		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A4;
			i += 2;
		}

/* 1-pixel */
		sps = (mlib_u16 *)sp;
		dps = (mlib_u16 *)dp;

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A2;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;
	mlib_u16 *sps;

/* not aligned dst address */
	mlib_u32 *dp;
	mlib_u16 *dps;

/* src data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;
	mlib_u16 sds, dds;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	const mlib_s32 cm0 = channelmasku82[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);
	const mlib_s32 cm1 = channelmasku82_1[cmask - 1];

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

/* row loop */
	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
			    loadu, loadu, storeu);
		}

/* 4-pixel */
		if (i < xsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
			i += 4;
		}

/* 2-pixel */
		sp = (mlib_u32 *)x_sp;
		dp = (mlib_u32 *)x_dp;

		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A4;
			i += 2;
		}

/* 1-pixel */
		sps = (mlib_u16 *)sp;
		dps = (mlib_u16 *)dp;

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A2;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_S_IMAGECHANNELCOPY3_U8_CMASKSET		\
	cm0 = _mm_setr_epi32(0x00ff0000, 0x0000ff00,	\
	    0xff0000ff, 0x00ff0000);			\
	cm1 = _mm_setr_epi32(0x0000ff00, 0xff0000ff,	\
	    0x00ff0000, 0x0000ff00);			\
	cm2 = _mm_setr_epi32(0xff0000ff, 0x00ff0000,	\
	    0x0000ff00, 0xff0000ff);			\
	cm3 = _mm_setr_epi32(0x00ffff00, 0xff00ffff,	\
	    0xffff00ff, 0x00ffff00);			\
	cm4 = _mm_setr_epi32(0xff00ffff, 0xffff00ff,	\
	    0x00ffff00, 0xff00ffff);			\
	cm5 = _mm_setr_epi32(0xffff00ff, 0x00ffff00,	\
	    0xff00ffff, 0xffff00ff);			\
	m_cm0 = _mm_setr_pi32(0x00ff0000, 0x0000ff00);	\
	m_cm1 = _mm_setr_pi32(0xff0000ff, 0x00ff0000);	\
	m_cm2 = _mm_setr_pi32(0x0000ff00, 0xff0000ff);	\
	m_cm3 = _mm_setr_pi32(0x00ffff00, 0xff0000ff);	\
	m_cm4 = _mm_setr_pi32(0xffff00ff, 0x00ffff00);	\
	m_cm5 = _mm_setr_pi32(0xff00ffff, 0xffff00ff);	\
	switch (cmask) {				\
		case 1:					\
			chanmask0 = cm0;		\
			chanmask1 = cm1;		\
			chanmask2 = cm2;		\
			break;				\
		case 2:					\
			chanmask0 = cm1;		\
			chanmask1 = cm2;		\
			chanmask2 = cm0;		\
			break;				\
		case 3:					\
			chanmask0 = cm3;		\
			chanmask1 = cm4;		\
			chanmask2 = cm5;		\
			break;				\
		case 4:					\
			chanmask0 = cm2;		\
			chanmask1 = cm0;		\
			chanmask2 = cm1;		\
			break;				\
		case 5:					\
			chanmask0 = cm5;		\
			chanmask1 = cm3;		\
			chanmask2 = cm4;		\
			break;				\
		case 6:					\
			chanmask0 = cm4;		\
			chanmask1 = cm5;		\
			chanmask2 = cm3;		\
			break;				\
	}						\
	switch (cmask) {				\
		case 1:					\
			m_chanmask0 = m_cm0;		\
			m_chanmask1 = m_cm1;		\
			m_chanmask2 = m_cm2;		\
			break;				\
		case 2:					\
			m_chanmask0 = m_cm2;		\
			m_chanmask1 = m_cm0;		\
			m_chanmask2 = m_cm1;		\
			break;				\
		case 3:					\
			m_chanmask0 = m_cm3;		\
			m_chanmask1 = m_cm4;		\
			m_chanmask2 = m_cm5;		\
			break;				\
		case 4:					\
			m_chanmask0 = m_cm1;		\
			m_chanmask1 = m_cm2;		\
			m_chanmask2 = m_cm0;		\
			break;				\
		case 5:					\
			m_chanmask0 = m_cm4;		\
			m_chanmask1 = m_cm5;		\
			m_chanmask2 = m_cm3;		\
			break;				\
		case 6:					\
			m_chanmask0 = m_cm5;		\
			m_chanmask1 = m_cm3;		\
			m_chanmask2 = m_cm4;		\
			break;				\
	}						\
	switch (cmask) {				\
		case 1:					\
			chanmask = 0x00ff0000;		\
			break;				\
		case 2:					\
			chanmask = 0x0000ff00;		\
			break;				\
		case 3:					\
			chanmask = 0x00ffff00;		\
			break;				\
		case 4:					\
			chanmask = 0x000000ff;		\
			break;				\
		case 5:					\
			chanmask = 0x00ff00ff;		\
			break;				\
		case 6:					\
			chanmask = 0x0000ffff;		\
			break;				\
	}                           \

/* 48 bytes, 16 pixel, 3 channels */
#define	MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(ld, st)	\
	x_sd0 = _mm_##ld##_si128(x_sp);			\
	x_sp++;						\
	x_sd1 = _mm_##ld##_si128(x_sp);			\
	x_sp++;						\
	x_sd2 = _mm_##ld##_si128(x_sp);			\
	x_sp++;						\
	x_dd0 = _mm_##ld##_si128(x_dp);			\
	x_dd1 = _mm_##ld##_si128(x_dp + 1);		\
	x_dd2 = _mm_##ld##_si128(x_dp + 2);		\
	x_sd0 = _mm_and_si128(chanmask0, x_sd0);	\
	x_sd1 = _mm_and_si128(chanmask1, x_sd1);	\
	x_sd2 = _mm_and_si128(chanmask2, x_sd2);	\
	x_dd0 = _mm_andnot_si128(chanmask0, x_dd0);	\
	x_dd1 = _mm_andnot_si128(chanmask1, x_dd1);	\
	x_dd2 = _mm_andnot_si128(chanmask2, x_dd2);	\
	x_dd0 = _mm_or_si128(x_sd0, x_dd0);		\
	x_dd1 = _mm_or_si128(x_sd1, x_dd1);		\
	x_dd2 = _mm_or_si128(x_sd2, x_dd2);		\
	_mm_##st##_si128(x_dp, x_dd0);			\
	x_dp++;						\
	_mm_##st##_si128(x_dp, x_dd1);			\
	x_dp++;						\
	_mm_##st##_si128(x_dp, x_dd2);			\
	x_dp++

/* 24 bytes, 8 pixel, 3 channels */
#define	MLIB_S_IMAGECHANNELCOPY3_U8S16_A8		\
	m_sd0 = *m_sp;					\
	m_sp++;						\
	m_sd1 = *m_sp;					\
	m_sp++;						\
	m_sd2 = *m_sp;					\
	m_sp++;						\
	m_dd0 = *m_dp;					\
	m_dd1 = *(m_dp + 1);				\
	m_dd2 = *(m_dp + 2);				\
	m_sd0 = _mm_and_si64(m_chanmask0, m_sd0);	\
	m_sd1 = _mm_and_si64(m_chanmask1, m_sd1);	\
	m_sd2 = _mm_and_si64(m_chanmask2, m_sd2);	\
	m_dd0 = _mm_andnot_si64(m_chanmask0, m_dd0);	\
	m_dd1 = _mm_andnot_si64(m_chanmask1, m_dd1);	\
	m_dd2 = _mm_andnot_si64(m_chanmask2, m_dd2);	\
	m_dd0 = _mm_or_si64(m_dd0, m_sd0);		\
	m_dd1 = _mm_or_si64(m_dd1, m_sd1);		\
	m_dd2 = _mm_or_si64(m_dd2, m_sd2);		\
	*m_dp = m_dd0;					\
	m_dp++;						\
	*m_dp = m_dd1;					\
	m_dp++;						\
	*m_dp = m_dd2;					\
	m_dp++

/* 3 bytes, 1 pixel, 3 channels */
#define	MLIB_S_IMAGECHANNELCOPY3_U8_1			\
	sd0 = *((mlib_u32 *)sp);			\
	sd0 = (sd0 & chanmask);				\
	dd0 = *((mlib_u32 *)dp);			\
	dd0 = dd0 & (~chanmask);			\
	dd0 = dd0 | sd0;				\
	*((mlib_u32 *)dp) = dd0;			\
	sp += 3;					\
	dp += 3

/* 3 bytes, 1 pixel, 3 channels */
#define	MLIB_S_IMAGECHANNELCOPY3_U8_1_LAST		\
	sd0 = *((mlib_u32 *)sp);			\
	sd0 = (sd0 & chanmask);				\
	dd0 = *((mlib_u32 *)dp);			\
	dd0 = dd0 & (~chanmask);			\
	dd0 = dd0 | sd0;				\
	*((mlib_u16 *)dp) = *((mlib_u16 *)&dd0);	\
	dp += 2;					\
	*((mlib_u8 *)dp) = *((mlib_u8 *)&dd0 + 2);	\
	sp += 3;					\
	dp += 1

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_3_A16D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m64 *m_sp, *m_dp;
	mlib_u8 *sp;

/* aligned dst address */
	mlib_u8 *dp;

/* src & dst data */
	__m128i x_sd0, x_dd0;
	__m128i x_sd1, x_dd1;
	__m128i x_sd2, x_dd2;
	__m64 m_sd0, m_dd0;
	__m64 m_sd1, m_dd1;
	__m64 m_sd2, m_dd2;
	mlib_u32 sd0, sd1, sd2;
	mlib_u32 dd0, dd1, dd2;

/* loop variables */
	mlib_s32 i;

/* channel mask */
	static __m128i cm0, cm1, cm2, cm3, cm4, cm5;
	static __m128i chanmask0, chanmask1, chanmask2;
	static __m64 m_cm0, m_cm1, m_cm2, m_cm3, m_cm4, m_cm5;
	static __m64 m_chanmask0, m_chanmask1, m_chanmask2;
	static mlib_s32 chanmask;

/* prepare the channel mask */
	MLIB_S_IMAGECHANNELCOPY3_U8_CMASKSET;

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	if ((((mlib_addr)src & 15) == 0) &&
	    (((mlib_addr)dst & 15) == 0)) {

/* 16-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 15; i += 16) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(load, store);
		}

	} else {

/* 16-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 15; i += 16) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(loadu, storeu);
		}
	}

/* 8-pixel loop */
	m_sp = (__m64 *)x_sp;
	m_dp = (__m64 *)x_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize - 7; i += 8) {
		MLIB_S_IMAGECHANNELCOPY3_U8S16_A8;
	}

/* 1-pixel loop */
	sp = (mlib_u8 *)m_sp;
	dp = (mlib_u8 *)m_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize - 1; i += 1) {
		MLIB_S_IMAGECHANNELCOPY3_U8_1;
	}

	if (i < dsize) {
		MLIB_S_IMAGECHANNELCOPY3_U8_1_LAST;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_3_A16D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	__m64 *m_sp, *m_dp;
	mlib_u8 *sp;

/* aligned dst address */
	mlib_u8 *dp;

/* src & dst data */
	__m128i x_sd0, x_dd0;
	__m128i x_sd1, x_dd1;
	__m128i x_sd2, x_dd2;
	__m64 m_sd0, m_dd0;
	__m64 m_sd1, m_dd1;
	__m64 m_sd2, m_dd2;
	mlib_u32 sd0, sd1, sd2;
	mlib_u32 dd0, dd1, dd2;

/* loop variables */
	mlib_s32 i, j;

/* channel mask */
	static __m128i cm0, cm1, cm2, cm3, cm4, cm5;
	static __m128i chanmask0, chanmask1, chanmask2;
	static __m64 m_cm0, m_cm1, m_cm2, m_cm3, m_cm4, m_cm5;
	static __m64 m_chanmask0, m_chanmask1, m_chanmask2;
	static mlib_s32 chanmask;

/* prepare the channel mask */
	MLIB_S_IMAGECHANNELCOPY3_U8_CMASKSET;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 16-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 15; i += 16) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(load, store);
		}

/* 8-pixel loop */
		m_sp = (__m64 *)x_sp;
		m_dp = (__m64 *)x_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A8;
		}

/* 1-pixel loop */
		sp = (mlib_u8 *)m_sp;
		dp = (mlib_u8 *)m_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 1; i += 1) {
			MLIB_S_IMAGECHANNELCOPY3_U8_1;
		}

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY3_U8_1_LAST;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	__m64 *m_sp, *m_dp;
	mlib_u8 *sp;

/* not aligned dst address */
	mlib_u8 *dp;

/* src & dst data */
	__m128i x_sd0, x_dd0;
	__m128i x_sd1, x_dd1;
	__m128i x_sd2, x_dd2;
	__m64 m_sd0, m_dd0;
	__m64 m_sd1, m_dd1;
	__m64 m_sd2, m_dd2;
	mlib_u32 sd0, sd1, sd2;
	mlib_u32 dd0, dd1, dd2;

/* loop variables */
	mlib_s32 i, j;

/* channel mask */
	static __m128i cm0, cm1, cm2, cm3, cm4, cm5;
	static __m128i chanmask0, chanmask1, chanmask2;
	static __m64 m_cm0, m_cm1, m_cm2, m_cm3, m_cm4, m_cm5;
	static __m64 m_chanmask0, m_chanmask1, m_chanmask2;
	mlib_s32 chanmask;

/* prepare the channel mask */
	MLIB_S_IMAGECHANNELCOPY3_U8_CMASKSET;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 16-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 15; i += 16) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(loadu, storeu);
		}

/* 8-pixel loop */
		m_sp = (__m64 *)x_sp;
		m_dp = (__m64 *)x_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A8;
		}

/* 1-pixel loop */
		sp = (mlib_u8 *)m_sp;
		dp = (mlib_u8 *)m_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 1; i += 1) {
			MLIB_S_IMAGECHANNELCOPY3_U8_1;
		}

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY3_U8_1_LAST;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

/* channel masks const */
const mlib_u32 channelmasku84[14] = {
	0xff000000, 0x00ff0000,	0xffff0000, 0x0000ff00,
	0xff00ff00, 0x00ffff00,	0xffffff00, 0x000000ff,
	0xff0000ff, 0x00ff00ff,	0xffff00ff, 0x0000ffff,
	0xff00ffff, 0x00ffffff};

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_4_A16D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	mlib_u32 *sp;

/* aligned dst address */
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* loop variables */
	mlib_s32 i;

/* prepare the channel masks */
	const mlib_s32 cm0 = channelmasku84[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;

	if ((((mlib_addr)src & 15) == 0) &&
	    (((mlib_addr)dst & 15) == 0)) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    load, load, store);
		}

/* 4-pixel */
		if (i < dsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
			i += 4;
		}

	} else { /* if (((mlib_addr)src & 15) == 0) */

/* 8-pixel */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    loadu, loadu, storeu);
		}

/* 4-pixel */
		if (i < dsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
			    loadu, loadu, storeu);
			i += 4;
		}
	}

/* 2-pixel */
	if (i < dsize - 1) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
		i += 2;
	}

/* 1-pixel */
	sp = (mlib_u32 *)x_sp;
	dp = (mlib_u32 *)x_dp;

	if (i < dsize) {
		MLIB_S_IMAGECHANNELCOPY24_U8_A4;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_4_A16D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;

/* aligned dst address */
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* loop variables */
	mlib_s32 i, j;

/* channel masks */
	const mlib_s32 cm0 = channelmasku84[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(channelmasku84[cmask - 1]);

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    load, load, store);
		}

/* 4-pixel */
		if (i < xsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
			i += 4;
		}

/* 2-pixel */
		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
			i += 2;
		}

/* 1-pixel */
		sp = (mlib_u32 *)x_sp;
		dp = (mlib_u32 *)x_dp;

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A4;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	mlib_u32 *sp;

/* not aligned dst address */
	__m128i *x_sl, *x_dl;
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* loop variables */
	mlib_s32 i, j;

/* channel masks */
	const mlib_s32 cm0 = channelmasku84[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(channelmasku84[cmask - 1]);

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    loadu, loadu, storeu);
		}

/* 4-pixel */
		if (i < xsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
			    loadu, loadu, storeu);
			i += 4;
		}

/* 2-pixel */
		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
			i += 2;
		}

/* 1-pixel */
		sp = (mlib_u32 *)x_sp;
		dp = (mlib_u32 *)x_dp;

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A4;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_u8(
    const mlib_u8 *src,
    mlib_s32 src_stride,
    mlib_u8 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_s32 i, x, y;
	mlib_u8 *srcRow, *dstRow, *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4], cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (void *)src;
		dstRow = dst;
		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (srcRow + cc);
			pDst = (dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

/* slower then pure C implementation */
#if 0

/* channel mask const */
const mlib_u32 channelmasks162[2] = {0xffff0000, 0x0000ffff};

void
mlib_s_ImageChannelCopy_S16_2_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	mlib_u32 *sp;
	mlib_u16 *sps;

/* aligned dst address */
	mlib_u32 *dp;
	mlib_u16 *dps;

/* src data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;
	mlib_u16 sds, dds;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	const mlib_s32 cm0 = channelmasks162[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;

	if ((((mlib_addr)src & 15) == 0) &&
	    (((mlib_addr)dst & 15) == 0)) {

/* 8-pixel loop, aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    load, load, store);
		}

/* 4-pixel loop, aligned */
		if (i < dsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
			i += 4;
		}

	} else { /* if (((mlib_addr)src & 15) == 0) */

/* 8-pixel loop, not aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    loadu, loadu, storeu);
		}

/* 4-pixel loop, not aligned */
		if (i < dsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
			    loadu, loadu, storeu);
			i += 4;
		}
	}

/* 2-pixel loop */
	if (i < dsize - 1) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
		i += 2;
	}

/* 1-pixel loop */
	sp = (mlib_u32 *)x_sp;
	dp = (mlib_u32 *)x_dp;

	if (i < dsize) {
		MLIB_S_IMAGECHANNELCOPY24_U8_A4;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_2_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;
	mlib_u16 *sps;

/* aligned dst address */
	mlib_u32 *dp;
	mlib_u16 *dps;

/* src data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;
	mlib_u16 sds, dds;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	const mlib_s32 cm0 = channelmasks162[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

/* row loop */
	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop, aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    load, load, store);
		}

/* 4-pixel, aligned */
		if (i < xsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
			i += 4;
		}

/* 2-pixel */
		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
			i += 2;
		}

/* 1-pixel */
		sp = (mlib_u32 *)x_sp;
		dp = (mlib_u32 *)x_dp;

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A4;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;
	mlib_u16 *sps;

/* not aligned dst address */
	mlib_u32 *dp;
	mlib_u16 *dps;

/* src data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;
	mlib_u16 sds, dds;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	const mlib_s32 cm0 = channelmasks162[cmask - 1];
	const __m128i chanmask = _mm_set1_epi32(cm0);

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

/* row loop */
	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop, not aligned */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    loadu, loadu, storeu);
		}

/* 4-pixel, not aligned */
		if (i < xsize - 3) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
			    loadu, loadu, storeu);
			i += 4;
		}

/* 2-pixel */
		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
			i += 2;
		}

/* 1-pixel */
		sp = (mlib_u32 *)x_sp;
		dp = (mlib_u32 *)x_dp;

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8_A4;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

#endif /* #if 0 */

/* *********************************************************** */

#define	MLIB_S_IMAGECHANNELCOPY3_S16_CMASKSET		\
	cm0 = _mm_setr_epi32(0x00000000, 0x0000ffff,	\
	    0xffff0000, 0x00000000);			\
	cm1 = _mm_setr_epi32(0x0000ffff, 0xffff0000,	\
	    0x00000000, 0x0000ffff);			\
	cm2 = _mm_setr_epi32(0xffff0000, 0x00000000,	\
	    0x0000ffff, 0xffff0000);			\
	cm3 = _mm_setr_epi32(0xffff0000, 0x0000ffff,	\
	    0xffffffff, 0xffff0000);			\
	cm4 = _mm_setr_epi32(0x0000ffff, 0xffffffff,	\
	    0xffff0000, 0x0000ffff);			\
	cm5 = _mm_setr_epi32(0xffffffff, 0xffff0000,	\
	    0x0000ffff, 0xffffffff);			\
	m_cm0 = _mm_setr_pi32(0x00000000, 0x0000ffff);	\
	m_cm1 = _mm_setr_pi32(0xffff0000, 0x00000000);	\
	m_cm2 = _mm_setr_pi32(0x0000ffff, 0xffff0000);	\
	m_cm3 = _mm_setr_pi32(0xffff0000, 0x0000ffff);	\
	m_cm4 = _mm_setr_pi32(0xffffffff, 0xffff0000);	\
	m_cm5 = _mm_setr_pi32(0x0000ffff, 0xffffffff);	\
	switch (cmask) {				\
		case 1:					\
			chanmask0 = cm0;		\
			chanmask1 = cm1;		\
			chanmask2 = cm2;		\
			break;				\
		case 2:					\
			chanmask0 = cm2;		\
			chanmask1 = cm0;		\
			chanmask2 = cm1;		\
			break;				\
		case 3:					\
			chanmask0 = cm3;		\
			chanmask1 = cm4;		\
			chanmask2 = cm5;		\
			break;				\
		case 4:					\
			chanmask0 = cm1;		\
			chanmask1 = cm2;		\
			chanmask2 = cm0;		\
			break;				\
		case 5:					\
			chanmask0 = cm4;		\
			chanmask1 = cm5;		\
			chanmask2 = cm3;		\
			break;				\
		case 6:					\
			chanmask0 = cm5;		\
			chanmask1 = cm3;		\
			chanmask2 = cm4;		\
			break;				\
	}						\
	switch (cmask) {				\
		case 1:					\
			m_chanmask0 = m_cm0;		\
			m_chanmask1 = m_cm1;		\
			m_chanmask2 = m_cm2;		\
			break;				\
		case 2:					\
			m_chanmask0 = m_cm1;		\
			m_chanmask1 = m_cm2;		\
			m_chanmask2 = m_cm0;		\
			break;				\
		case 3:					\
			m_chanmask0 = m_cm3;		\
			m_chanmask1 = m_cm4;		\
			m_chanmask2 = m_cm5;		\
			break;				\
		case 4:					\
			m_chanmask0 = m_cm2;		\
			m_chanmask1 = m_cm0;		\
			m_chanmask2 = m_cm1;		\
			break;				\
		case 5:					\
			m_chanmask0 = m_cm5;		\
			m_chanmask1 = m_cm3;		\
			m_chanmask2 = m_cm4;		\
			break;				\
		case 6:					\
			m_chanmask0 = m_cm4;		\
			m_chanmask1 = m_cm5;		\
			m_chanmask2 = m_cm3;		\
			break;				\
	}						\
	switch (cmask) {				\
		case 1:					\
			m_chanmask = _mm_setr_pi32(	\
			    0x00000000, 0x0000ffff);	\
			break;				\
		case 2:					\
			m_chanmask = _mm_setr_pi32(	\
			    0xffff0000, 0x00000000);	\
			break;				\
		case 3:					\
			m_chanmask = _mm_setr_pi32(	\
			    0xffff0000, 0x0000ffff);	\
			break;				\
		case 4:					\
			m_chanmask = _mm_setr_pi32(	\
			    0x0000ffff, 0x00000000);	\
			break;				\
		case 5:					\
			m_chanmask = _mm_setr_pi32(	\
			    0x0000ffff, 0x0000ffff);	\
			break;				\
		case 6:					\
			m_chanmask = _mm_setr_pi32(	\
			    0xffffffff, 0x00000000);	\
			break;				\
	}                           \

/* *********************************************************** */

/* 6 bytes, 1 pixel, 3 channels */
#define	MLIB_S_IMAGECHANNELCOPY3_S16_1			\
	m_sd0 = *((__m64 *)sp);				\
	m_dd0 = *((__m64 *)dp);				\
	m_sd1 = _mm_and_si64(m_chanmask, m_sd0);	\
	m_dd1 = _mm_andnot_si64(m_chanmask, m_dd0);	\
	m_dd0 = _mm_or_si64(m_dd1, m_sd1);		\
	*((__m64 *)dp) = m_dd0;				\
	sp += 3;					\
	dp += 3

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_3_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m64 *m_sp, *m_dp;
	mlib_s16 *sp;

/* aligned dst address */
	mlib_s16 *dp;

/* src & dst data */
	__m128i x_sd0, x_dd0;
	__m128i x_sd1, x_dd1;
	__m128i x_sd2, x_dd2;
	__m64 m_sd0, m_dd0;
	__m64 m_sd1, m_dd1;
	__m64 m_sd2, m_dd2;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	static __m128i cm0, cm1, cm2, cm3, cm4, cm5;
	static __m128i chanmask0, chanmask1, chanmask2;
	static __m64 m_cm0, m_cm1, m_cm2, m_cm3, m_cm4, m_cm5;
	static __m64 m_chanmask, m_chanmask0, m_chanmask1, m_chanmask2;

/* prepare the channel mask */
	MLIB_S_IMAGECHANNELCOPY3_S16_CMASKSET;

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;

	if ((((mlib_addr)src & 15) == 0) &&
	    (((mlib_addr)dst & 15) == 0)) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(load, store);
		}

	} else {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(loadu, storeu);
		}
	}

/* 4-pixel loop */
	m_sp = (__m64 *)x_sp;
	m_dp = (__m64 *)x_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize - 3; i += 4) {
		MLIB_S_IMAGECHANNELCOPY3_U8S16_A8;
	}

/* 1-pixel loop */
	sp = (mlib_s16 *)m_sp;
	dp = (mlib_s16 *)m_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize; i += 1) {
		MLIB_S_IMAGECHANNELCOPY3_S16_1;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_3_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	__m64 *m_sp, *m_dp;
	mlib_s16 *sp;

/* aligned dst address */
	mlib_s16 *dp;

/* src & dst data */
	__m128i x_sd0, x_dd0;
	__m128i x_sd1, x_dd1;
	__m128i x_sd2, x_dd2;
	__m64 m_sd0, m_dd0;
	__m64 m_sd1, m_dd1;
	__m64 m_sd2, m_dd2;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	static __m128i cm0, cm1, cm2, cm3, cm4, cm5;
	static __m128i chanmask0, chanmask1, chanmask2;
	static __m64 m_cm0, m_cm1, m_cm2, m_cm3, m_cm4, m_cm5;
	static __m64 m_chanmask, m_chanmask0, m_chanmask1, m_chanmask2;

/* prepare the channel mask */
	MLIB_S_IMAGECHANNELCOPY3_S16_CMASKSET;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(load, store);
		}

/* 4-pixel loop */
		m_sp = (__m64 *)x_sp;
		m_dp = (__m64 *)x_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 3; i += 4) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A8;
		}

/* 1-pixel loop */
		sp = (mlib_s16 *)m_sp;
		dp = (mlib_s16 *)m_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize; i += 1) {
			MLIB_S_IMAGECHANNELCOPY3_S16_1;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	__m64 *m_sp, *m_dp;
	mlib_s16 *sp;

/* not aligned dst address */
	mlib_s16 *dp;

/* src & dst data */
	__m128i x_sd0, x_dd0;
	__m128i x_sd1, x_dd1;
	__m128i x_sd2, x_dd2;
	__m64 m_sd0, m_dd0;
	__m64 m_sd1, m_dd1;
	__m64 m_sd2, m_dd2;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	static __m128i cm0, cm1, cm2, cm3, cm4, cm5;
	static __m128i chanmask0, chanmask1, chanmask2;
	static __m64 m_cm0, m_cm1, m_cm2, m_cm3, m_cm4, m_cm5;
	static __m64 m_chanmask, m_chanmask0, m_chanmask1, m_chanmask2;

/* prepare the channel mask */
	MLIB_S_IMAGECHANNELCOPY3_S16_CMASKSET;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 8-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 7; i += 8) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A16(loadu, storeu);
		}

/* 4-pixel loop */
		m_sp = (__m64 *)x_sp;
		m_dp = (__m64 *)x_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 3; i += 4) {
			MLIB_S_IMAGECHANNELCOPY3_U8S16_A8;
		}

/* 1-pixel loop */
		sp = (mlib_s16 *)m_sp;
		dp = (mlib_s16 *)m_dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize; i += 1) {
			MLIB_S_IMAGECHANNELCOPY3_S16_1;
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

/* channel masks const */
const mlib_u32 channelmasks164[28] = {
	0x00000000, 0xffff0000,	0x00000000, 0x0000ffff,
	0x00000000, 0xffffffff,	0xffff0000, 0x00000000,
	0xffff0000, 0xffff0000,	0xffff0000, 0x0000ffff,
	0xffff0000, 0xffffffff,	0x0000ffff, 0x00000000,
	0x0000ffff, 0xffff0000,	0x0000ffff, 0x0000ffff,
	0x0000ffff, 0xffffffff,	0xffffffff, 0x00000000,
	0xffffffff, 0xffff0000,	0xffffffff, 0x0000ffff
};

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_4_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	mlib_u32 *sp;

/* aligned dst address */
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* channel masks */
	mlib_u32 m0 = channelmasks164[2 * (cmask - 1)];
	mlib_u32 m1 = channelmasks164[2 * (cmask - 1) + 1];
	const __m128i chanmask = _mm_setr_epi32(m0, m1, m0, m1);

/* loop variables */
	mlib_s32 i;

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

	i = 0;

/* 4-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize - 3; i += 4) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(load, load, store);
	}

/* 2-pixel */
	if (i < dsize - 1) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
		i += 2;
	}

	if (i < dsize) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_4_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;

/* aligned dst address */
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* channel masks */
	mlib_u32 m0 = channelmasks164[2 * (cmask - 1)];
	mlib_u32 m1 = channelmasks164[2 * (cmask - 1) + 1];
	const __m128i chanmask = _mm_setr_epi32(m0, m1, m0, m1);

/* loop variables */
	mlib_s32 i, j;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

/* 4-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < xsize - 3; i += 4) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
			    load, load, store);
		}

/* 2-pixel */
		if (i < xsize - 1) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(load, load, store);
			i += 2;
		}

		if (i < xsize) {
			MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_4D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	mlib_u32 *sp;

/* not aligned dst address */
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* channel masks */
	mlib_u32 m0 = channelmasks164[2 * (cmask - 1)];
	mlib_u32 m1 = channelmasks164[2 * (cmask - 1) + 1];
	const __m128i chanmask = _mm_setr_epi32(m0, m1, m0, m1);

/* loop variables */
	mlib_s32 i;

	x_sp = (__m128i *)src;
	x_dp = (__m128i *)dst;

/* 4-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < dsize - 3; i += 4) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(loadu, loadu, storeu);
	}

/* 2-pixel */
	if (i < dsize - 1) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(loadu, loadu, storeu);
		i += 2;
	}

	if (i < dsize) {
		MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelCopy_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* not aligned src address */
	__m128i *x_sp, *x_dp;
	__m128i *x_sl, *x_dl;
	mlib_u32 *sp;

/* not aligned dst address */
	mlib_u32 *dp;

/* src & dst data */
	__m128i x_sd, x_dd;
	__m128i x_sd0, x_dd0;
	mlib_u32 sd, dd;

/* channel masks */
	mlib_u32 m0 = channelmasks164[2 * (cmask - 1)];
	mlib_u32 m1 = channelmasks164[2 * (cmask - 1) + 1];
	const __m128i chanmask = _mm_setr_epi32(m0, m1, m0, m1);

/* loop variables */
	mlib_s32 i, j;

	x_sp = x_sl = (__m128i *)src;
	x_dp = x_dl = (__m128i *)dst;

	for (j = 0, i = 0; j < ysize; j++, i = 0) {

		if (((mlib_addr)x_sp & 15) == 0) {
/* 4-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < xsize - 3; i += 4) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
				    load, loadu, storeu);
			}

/* 2-pixel */
			if (i < xsize - 1) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
				    load, loadu, storeu);
				i += 2;
			}

			if (i < xsize) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(load);
			}

		} else if (((mlib_addr)x_dp & 15) == 0) {

/* 4-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < xsize - 3; i += 4) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
				    loadu, load, store);
			}

/* 2-pixel */
			if (i < xsize - 1) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
				    loadu, load, store);
				i += 2;
			}

			if (i < xsize) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
			}

		} else {

/* 4-pixel loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < xsize - 3; i += 4) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A16X2(
				    loadu, loadu, storeu);
			}

/* 2-pixel */
			if (i < xsize - 1) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A16(
				    loadu, loadu, storeu);
				i += 2;
			}

			if (i < xsize) {
				MLIB_S_IMAGECHANNELCOPY24_U8S16_A8(loadu);
			}
		}

		x_sp = x_sl = (__m128i *)((mlib_u8 *)x_sl + slb);
		x_dp = x_dl = (__m128i *)((mlib_u8 *)x_dl + dlb);
	}
}

/* *********************************************************** */

/* Pure C implement */

void
mlib_s_ImageChannelCopy_s16(
    const mlib_s16 *src,
    mlib_s32 src_stride,
    mlib_s16 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_s16 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 2;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_s16 *)(srcRow + cc);
			pDst = (mlib_s16 *)(dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

/* Pure C implement */

void
mlib_s_ImageChannelCopy_s32(
    const mlib_s32 *src,
    mlib_s32 src_stride,
    mlib_s32 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_s32 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 4;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_s32 *)(srcRow + cc);
			pDst = (mlib_s32 *)(dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

/* Pure C implement */

void
mlib_s_ImageChannelCopy_d64(
    const mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_d64 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_d64 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 8;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_d64 *)(srcRow + cc);
			pDst = (mlib_d64 *)(dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */
