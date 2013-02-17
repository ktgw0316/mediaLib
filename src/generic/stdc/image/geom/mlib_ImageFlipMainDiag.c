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

#pragma ident	"@(#)mlib_ImageFlipMainDiag.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageFlipMainDiag - flip image on main diag
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipMainDiag(mlib_image       *dst,
 *                                         const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can be in MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT data type.
 *      Images of MLIB_BIT date type can have only 1 channel,
 *      images of other date types can have 1, 2, 3 or 4 channels.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipMainDiag.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipMainDiag = __mlib_ImageFlipMainDiag

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageFlipMainDiag) mlib_ImageFlipMainDiag
    __attribute__((weak, alias("__mlib_ImageFlipMainDiag")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	dw	sh
#define	dh	sw

/* *********************************************************** */

#define	PREPAREVARS(data_type, num_ch)                                  \
	mlib_s32 sstride =                                              \
		mlib_ImageGetStride(src) / sizeof (data_type);          \
	mlib_s32 dstride =                                              \
		mlib_ImageGetStride(dst) / sizeof (data_type);          \
	mlib_s32 ws = mlib_ImageGetWidth(src);                          \
	mlib_s32 hs = mlib_ImageGetHeight(src);                         \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                          \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                         \
	mlib_s32 sw = (ws < hd) ? ws : hd;                              \
	mlib_s32 sh = (hs < wd) ? hs : wd;                              \
	mlib_s32 dx = (mlib_s32)ws - (mlib_s32)hd;                      \
	mlib_s32 dy = (mlib_s32)hs - (mlib_s32)wd;                      \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);                  \
	mlib_s32 dyd = ((dx > 0) ? 0 : (-dx - x_dir) >> 1);             \
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);                  \
	mlib_s32 dxd = ((dy > 0) ? 0 : (-dy - y_dir) >> 1);             \
	data_type *sdata =                                              \
		(data_type *) mlib_ImageGetData(src) + dxs * num_ch +   \
		sstride * dys;                                          \
	data_type *ddata =                                              \
		(data_type *) mlib_ImageGetData(dst) + dxd * num_ch +   \
		dstride * dyd;                                          \
	data_type *sp, *dp;                                             \
	mlib_s32 schan = (x_dir >= 0) ? num_ch : -num_ch;               \
	mlib_s32 i, j;                                                  \
	                                                                \
	if (y_dir < 0)                                                  \
	{                                                               \
	sdata += (sh - 1) * sstride;                                    \
	sstride = -sstride;                                             \
	}                                                               \
	if (x_dir < 0)                                                  \
	    sdata += (sw - 1) * num_ch;                                 \
	sp = sdata;                                                     \
	dp = ddata

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN

/* *********************************************************** */

#define	MERGE_4U8(s0, s1, s2, s3)	(s0 << 24) | (s1 << 16) | \
					(s2 << 8) | (s3)
#define	MERGE_S16HI(x, y)	(((x) & 0xffff0000) | ((y) >> 16))
#define	MERGE_S16LO(x, y)	(((x) << 16) | ((y) & 0x0000ffff))

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	MERGE_4U8(s0, s1, s2, s3)	(s3 << 24) | (s2 << 16) | \
					(s1 << 8) | (s0)
#define	MERGE_S16HI(x, y)	(((x) & 0x0000ffff) | ((y) << 16))
#define	MERGE_S16LO(x, y)	(((x) >> 16) | ((y) & 0xffff0000))

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SHIFTU8	0
#define	SHIFTU16	1

#define	SHIFTF32	2

/* *********************************************************** */

#define	ABS(x)	((x) > 0 ? (x) : -(x))

/* *********************************************************** */

#define	PREP(CHAN, xs)                                          \
	if (x_dir < 0) {                                        \
	    sp = sdata += (xs - 1) * schan;                     \
	    dp = ddata += (xs - 1) * dstride;                   \
	    dstride = -dstride;                                 \
	}

/* *********************************************************** */

#define	TAIL(CHAN, xs)                                          \
	if (x_dir < 0) {                                        \
	    dstride = -dstride;                                 \
	    sp = sdata -= (xs - 1) * schan;                     \
	    dp = ddata -= (xs - 1) * dstride;                   \
	}                                                       \
	for (; j < sw; j++) {                                   \
	    FLIPLINE_##CHAN;                                    \
	}

/* *********************************************************** */

#define	FLIPLINE_CH1                                            \
	for (i = 0; i < sh; i++) {                              \
	    dp[i] = sp[i * sstride];                            \
	}                                                       \
	                                                        \
	sp = sdata += schan;                                    \
	dp = ddata += dstride

/* *********************************************************** */

#define	FLIPLINE_CH1_4x1                                        \
	for (i = 0; i < sh; i++) {                              \
	    dp[i] = sp[i * sstride];                            \
	    dp[dstride + i] = sp[i * sstride + 1];              \
	    dp[2 * dstride + i] = sp[i * sstride + 2];          \
	    dp[3 * dstride + i] = sp[i * sstride + 3];          \
	}                                                       \
	                                                        \
	sp = sdata += 4 * schan;                                \
	dp = ddata += 4 * ABS(dstride)

/* *********************************************************** */

#define	FLIP_1x4                                                \
	a0 = sp[0];                                             \
	a1 = sp[sstride];                                       \
	a2 = sp2[0];                                            \
	a3 = sp2[sstride];                                      \
	sp += 1;                                                \
	sp2 += 1;                                               \
	dp[0] = a0;                                             \
	dp[1] = a1;                                             \
	dp[2] = a2;                                             \
	dp[3] = a3;                                             \
	dp += dstride

/* *********************************************************** */

#define	FLIPLINE_CH1_4x4                                        \
	for (i = 0; i < (sh / 4); i++) {                        \
	    mlib_s32 a0, a1, a2, a3;                            \
	    mlib_u32 *sp2 = sp + 2 * sstride;                   \
	                                                        \
	    FLIP_1x4;                                           \
	    FLIP_1x4;                                           \
	    FLIP_1x4;                                           \
	    FLIP_1x4;                                           \
	    sp += 4 * sstride - 4;                              \
	    dp += 4 - 4 * dstride;                              \
	}                                                       \
	                                                        \
	for (i = 0; i < (sh & 3); i++) {                        \
	    dp[i] = sp[i * sstride];                            \
	    dp[dstride + i] = sp[i * sstride + 1];              \
	    dp[2 * dstride + i] = sp[i * sstride + 2];          \
	    dp[3 * dstride + i] = sp[i * sstride + 3];          \
	}                                                       \
	                                                        \
	sp = sdata += 4 * schan;                                \
	dp = ddata += 4 * ABS(dstride)

/* *********************************************************** */

#define	FLIPLINE_CH2                                            \
	for (i = 0; i < sh; i++) {                              \
	    dp[2 * i] = sp[i * sstride];                        \
	    dp[2 * i + 1] = sp[i * sstride + 1];                \
	}                                                       \
	                                                        \
	sp = sdata += schan;                                    \
	dp = ddata += dstride

/* *********************************************************** */

#define	FLIPLINE_CH2_4x1                                        \
	for (i = 0; i < sh; i++) {                              \
	    dp[0] = sp[0];                                      \
	    dp[1] = sp[1];                                      \
	    dp[dstride] = sp[2];                                \
	    dp[dstride + 1] = sp[3];                            \
	    dp[2 * dstride] = sp[4];                            \
	    dp[2 * dstride + 1] = sp[5];                        \
	    dp[3 * dstride] = sp[6];                            \
	    dp[3 * dstride + 1] = sp[7];                        \
	    dp += 2;                                            \
	    sp += sstride;                                      \
	}                                                       \
	                                                        \
	sp = sdata += 4 * schan;                                \
	dp = ddata += 4 * ABS(dstride)

/* *********************************************************** */

#define	FLIPLINE_CH3                                            \
	for (i = 0; i < sh; i++) {                              \
	    dp[3 * i] = sp[i * sstride];                        \
	    dp[3 * i + 1] = sp[i * sstride + 1];                \
	    dp[3 * i + 2] = sp[i * sstride + 2];                \
	}                                                       \
	                                                        \
	sp = sdata += schan;                                    \
	dp = ddata += dstride

/* *********************************************************** */

#define	FLIPLINE_CH4                                            \
	for (i = 0; i < sh; i++) {                              \
	    dp[4 * i] = sp[i * sstride];                        \
	    dp[4 * i + 1] = sp[i * sstride + 1];                \
	    dp[4 * i + 2] = sp[i * sstride + 2];                \
	    dp[4 * i + 3] = sp[i * sstride + 3];                \
	}                                                       \
	                                                        \
	sp = sdata += schan;                                    \
	dp = ddata += dstride

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN
#define	HI(x, SH)	((x) >> SH)
#define	LO(x, SH)	(x)

#else /* _LITTLE_ENDIAN */

#define	HI(x, SH)	(x)
#define	LO(x, SH)	((x) >> SH)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	FLIP_AL(TTYPE, SH)                                      \
	s0 = *(TTYPE *) (sp);                                   \
	s1 = *(TTYPE *) (sp + 2);                               \
	s2 = *(TTYPE *) (sp + 4);                               \
	s3 = *(TTYPE *) (sp + 6);                               \
	dp[0] = HI(s0, SH);                                     \
	dp[1] = LO(s0, SH);                                     \
	dp[dstride] = HI(s1, SH);                               \
	dp[dstride + 1] = LO(s1, SH);                           \
	dp[2 * dstride] = HI(s2, SH);                           \
	dp[2 * dstride + 1] = LO(s2, SH);                       \
	dp[3 * dstride] = HI(s3, SH);                           \
	dp[3 * dstride + 1] = LO(s3, SH);                       \
	dp += 2;                                                \
	sp += sstride

/* *********************************************************** */

#define	FLIP_NA(TTYPE, SH)                                      \
	s0 = *(TTYPE *) (sp - 1);                               \
	s1 = *(TTYPE *) (sp + 1);                               \
	s2 = *(TTYPE *) (sp + 3);                               \
	s3 = *(TTYPE *) (sp + 5);                               \
	s4 = *(TTYPE *) (sp + 7);                               \
	dp[0] = LO(s0, SH);                                     \
	dp[1] = HI(s1, SH);                                     \
	dp[dstride] = LO(s1, SH);                               \
	dp[dstride + 1] = HI(s2, SH);                           \
	dp[2 * dstride] = LO(s2, SH);                           \
	dp[2 * dstride + 1] = HI(s3, SH);                       \
	dp[3 * dstride] = LO(s3, SH);                           \
	dp[3 * dstride + 1] = HI(s4, SH);                       \
	dp += 2;                                                \
	sp += sstride

/* *********************************************************** */

#define	FLIPLINE_CH2_4x1_X2(TTYPE, SH)                          \
	mlib_s32 s0, s1, s2, s3, s4;                            \
	                                                        \
	if (sstride & 1) {                                      \
	    i = 0;                                              \
	    if ((mlib_s32)sp & (sizeof (TTYPE) / 2)) {          \
		FLIP_NA(TTYPE, SH);                             \
		i = 1;                                          \
	    }                                                   \
	    for (; i <= sh - 2; i += 2) {                       \
		FLIP_AL(TTYPE, SH);                             \
		FLIP_NA(TTYPE, SH);                             \
	    }                                                   \
	    if (i < sh) {                                       \
		FLIP_AL(TTYPE, SH);                             \
	    }                                                   \
	} else {                                                \
	    if ((mlib_s32)sp & (sizeof (TTYPE) / 2)) {          \
		for (i = 0; i < sh; i++) {                      \
		    FLIP_NA(TTYPE, SH);                         \
		}                                               \
	    } else {                                            \
		for (i = 0; i < sh; i++) {                      \
		    FLIP_AL(TTYPE, SH);                         \
		}                                               \
	    }                                                   \
	}                                                       \
	                                                        \
	sp = sdata += 4 * schan;                                \
	dp = ddata += 4 * ABS(dstride)

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_U8_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u8, 1);

	PREP(CH1, 4);

	if (((mlib_s32)dp | dstride) & 3) {
		for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FLIPLINE_CH1_4x1;
		}

		TAIL(CH1, 4);
		return;
	}

	for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (sh / 4); i++) {
			mlib_u8 a0, a1, a2, a3;
			mlib_u8 b0, b1, b2, b3;
			mlib_u8 *sp1 = sp + (sstride);
			mlib_u8 *sp2 = sp + (sstride << 1);
			mlib_u8 *sp3 = sp1 + (sstride << 1);

			a0 = sp[0];
			a1 = sp1[0];
			a2 = sp2[0];
			a3 = sp3[0];
			b0 = sp[1];
			b1 = sp1[1];
			b2 = sp2[1];
			b3 = sp3[1];
			*(mlib_u32 *)dp = MERGE_4U8(a0, a1, a2, a3);
			dp += dstride;
			a0 = sp[2];
			a1 = sp1[2];
			a2 = sp2[2];
			a3 = sp3[2];
			*(mlib_u32 *)dp = MERGE_4U8(b0, b1, b2, b3);
			dp += dstride;
			b0 = sp[3];
			b1 = sp1[3];
			b2 = sp2[3];
			b3 = sp3[3];
			*(mlib_u32 *)dp = MERGE_4U8(a0, a1, a2, a3);
			dp += dstride;
			sp += 4;
			*(mlib_u32 *)dp = MERGE_4U8(b0, b1, b2, b3);
			dp += dstride;
			sp += 4 * sstride - 4;
			dp += 4 - 4 * dstride;
		}

		for (i = 0; i < (sh & 3); i++) {
			dp[i] = sp[i * sstride];
			dp[dstride + i] = sp[i * sstride + 1];
			dp[2 * dstride + i] = sp[i * sstride + 2];
			dp[3 * dstride + i] = sp[i * sstride + 3];
		}

		sp = sdata += 4 * schan;
		dp = ddata += 4 * ABS(dstride);
	}

	TAIL(CH1, 4);
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S16_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u16, 1);

	PREP(CH1, 4);

	if (((((mlib_s32)sdata | (mlib_s32)ddata) >> 1) | sstride | dstride) &
	    1) {
		for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FLIPLINE_CH1_4x1;
		}

		TAIL(CH1, 4);
		return;
	}

	for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (sh / 4); i++) {
			mlib_u32 w0, w1, w2, w3, w4, w5, w6, w7;
			mlib_u16 *sp1 = sp + sstride;
			mlib_u16 *sp2 = sp1 + sstride;
			mlib_u16 *sp3 = sp2 + sstride;

			w0 = *(mlib_u32 *)(sp);
			w1 = *(mlib_u32 *)(sp + 2);
			w2 = *(mlib_u32 *)(sp1);
			w3 = *(mlib_u32 *)(sp1 + 2);
			w4 = *(mlib_u32 *)(sp2);
			w5 = *(mlib_u32 *)(sp2 + 2);
			w6 = *(mlib_u32 *)(sp3);
			w7 = *(mlib_u32 *)(sp3 + 2);
			sp += 4;
			*(mlib_u32 *)(dp) = MERGE_S16HI(w0, w2);
			*(mlib_u32 *)(dp + 2) = MERGE_S16HI(w4, w6);
			dp += dstride;
			*(mlib_u32 *)(dp) = MERGE_S16LO(w0, w2);
			*(mlib_u32 *)(dp + 2) = MERGE_S16LO(w4, w6);
			dp += dstride;
			*(mlib_u32 *)(dp) = MERGE_S16HI(w1, w3);
			*(mlib_u32 *)(dp + 2) = MERGE_S16HI(w5, w7);
			dp += dstride;
			*(mlib_u32 *)(dp) = MERGE_S16LO(w1, w3);
			*(mlib_u32 *)(dp + 2) = MERGE_S16LO(w5, w7);
			dp += dstride;
			sp += 4 * sstride - 4;
			dp += 4 - 4 * dstride;
		}

		for (i = 0; i < (sh & 3); i++) {
			dp[i] = sp[i * sstride];
			dp[dstride + i] = sp[i * sstride + 1];
			dp[2 * dstride + i] = sp[i * sstride + 2];
			dp[3 * dstride + i] = sp[i * sstride + 3];
		}

		sp = sdata += 4 * schan;
		dp = ddata += 4 * ABS(dstride);
	}

	TAIL(CH1, 4);
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_U8_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u8, 2);

	PREP(CH2, 4);
	for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH2_4x1_X2(mlib_u16, 8);
	}

	TAIL(CH2, 4);
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_U8_3_na(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u8, 3);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH3;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_U8_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u8, 4);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH4;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S16_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u16, 2);

	PREP(CH2, 4);
	for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH2_4x1_X2(mlib_u32,
		    16);
	}

	TAIL(CH2, 4);
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S16_3_na(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u16, 3);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH3;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S16_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u16, 4);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH4;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S32_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u32, 1);

	PREP(CH1, 4);
	for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH1_4x4;
	}

	TAIL(CH1, 4);
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S32_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_f32, 2);

	PREP(CH2, 4);
	for (j = 0; j <= sw - 4; j += 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH2_4x1;
	}

	TAIL(CH2, 4);
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S32_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_f32, 3);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH3;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S32_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_f32, 4);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH4;
	}
}

/* *********************************************************** */

#if 0
void
mlib_ImageFlipMainDiag_D64_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_d64, 1);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH1;
	}
}
#endif

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_D64_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_d64, 2);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH2;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_D64_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_d64, 3);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH3;
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_D64_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_d64, 4);

	for (j = 0; j < sw; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FLIPLINE_CH4;
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageFlipMainDiag(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if (type == MLIB_FLOAT || type == MLIB_DOUBLE) {
		return (MLIB_FAILURE);
	}

	return (mlib_ImageFlipRotate(dst, src, 0, 0));
}

/* *********************************************************** */
