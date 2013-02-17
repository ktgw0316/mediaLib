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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_ImageDataTypeConvert_f.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_c_ImageDataTypeConvert_BIT_U8
 *      mlib_c_ImageDataTypeConvert_BIT_S16
 *
 *      mlib_c_ImageDataTypeConvert_BIT_S32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_c_ImageDataTypeConvert_BIT_S32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_c_ImageDataTypeConvert_BIT_S32_D1     - 1-D, not aligned8
 *      mlib_c_ImageDataTypeConvert_BIT_S32        - 2-D, not aligned8
 *
 *      mlib_c_ImageDataTypeConvert_BIT_F32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_c_ImageDataTypeConvert_BIT_F32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_c_ImageDataTypeConvert_BIT_F32_D1     - 1-D, not aligned8
 *      mlib_c_ImageDataTypeConvert_BIT_F32        - 2-D, not aligned8
 *
 *      mlib_c_ImageDataTypeConvert_BIT_D64
 *
 *      mlib_c_ImageDataTypeConvert_U8_BIT
 *      mlib_c_ImageDataTypeConvert_U8_S16
 *      mlib_c_ImageDataTypeConvert_U8_S32
 *      mlib_c_ImageDataTypeConvert_U8_F32
 *      mlib_c_ImageDataTypeConvert_U8_D64
 *
 *      mlib_c_ImageDataTypeConvert_U16_BIT
 *      mlib_c_ImageDataTypeConvert_U16_U8
 *      mlib_c_ImageDataTypeConvert_U16_S16
 *      mlib_c_ImageDataTypeConvert_U16_S32
 *      mlib_c_ImageDataTypeConvert_U16_F32
 *      mlib_c_ImageDataTypeConvert_U16_D64
 *
 *      mlib_c_ImageDataTypeConvert_S16_BIT
 *      mlib_c_ImageDataTypeConvert_S16_U8
 *      mlib_c_ImageDataTypeConvert_S16_U16
 *      mlib_c_ImageDataTypeConvert_S16_S32
 *      mlib_c_ImageDataTypeConvert_S16_F32
 *      mlib_c_ImageDataTypeConvert_S16_D64
 *
 *      mlib_c_ImageDataTypeConvert_S32_BIT
 *      mlib_c_ImageDataTypeConvert_S32_U8
 *      mlib_c_ImageDataTypeConvert_S32_U16
 *      mlib_c_ImageDataTypeConvert_S32_S16
 *      mlib_c_ImageDataTypeConvert_S32_F32
 *      mlib_c_ImageDataTypeConvert_S32_D64
 *
 *      mlib_c_ImageDataTypeConvert_F32_U8
 *      mlib_c_ImageDataTypeConvert_F32_U16
 *      mlib_c_ImageDataTypeConvert_F32_S16
 *      mlib_c_ImageDataTypeConvert_F32_S32
 *      mlib_c_ImageDataTypeConvert_F32_D64
 *
 *      mlib_c_ImageDataTypeConvert_D64_BIT
 *      mlib_c_ImageDataTypeConvert_D64_U8
 *      mlib_c_ImageDataTypeConvert_D64_U16
 *      mlib_c_ImageDataTypeConvert_D64_S16
 *      mlib_c_ImageDataTypeConvert_D64_S32
 *      mlib_c_ImageDataTypeConvert_D64_F32
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      dst and src must have the same size and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Data format conversion.
 *      These functions are separated from mlib_c_ImageDataTypeConvert.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_c_ImageDataTypeConvert.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined _NO_LONGLONG

#ifdef _LITTLE_ENDIAN

static const mlib_u64 mlib_table[4] =
{ 0x0ULL, 0x100000000ULL, 0x1ULL, 0x100000001ULL };

#else /* _LITTLE_ENDIAN */

static const mlib_u64 mlib_table[4] =
{ 0x0ULL, 0x1ULL, 0x100000000ULL, 0x100000001ULL };

#endif /* _LITTLE_ENDIAN */

#endif /* ! defined _NO_LONGLONG */

/* *********************************************************** */

#ifdef __SUNPRO_C
#pragma align 8(mlib_table_F32)
#endif /* __SUNPRO_C */
static const mlib_f32 mlib_table_F32[8] =
{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

/* *********************************************************** */

#ifdef i386	/* do not perform the copying by mlib_d64 data type for x86 */

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

#define	TYPE_64BIT	mlib_d64
#endif /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

/* *********************************************************** */

typedef union
{
	TYPE_64BIT d64;
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

#define	SIZE	1024

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT8(DST, SRC)	DST = (mlib_u8) SRC
#define	SAT16(DST, SRC)	DST = (mlib_s16) SRC

#define	SAT32(DST, SRC)	DST = (mlib_s32) SRC

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT8(DST, SRC)                                          \
	if (SRC >= MLIB_U8_MAX)                                 \
	    DST = MLIB_U8_MAX;                                  \
	else                                                    \
	    if (SRC <= MLIB_U8_MIN)                             \
		DST = MLIB_U8_MIN;                              \
	    else                                                \
		DST = (mlib_u8)SRC

#define	SAT16(DST, SRC)                                         \
	if (SRC >= MLIB_S16_MAX)                                \
	    DST = MLIB_S16_MAX;                                 \
	else                                                    \
	    if (SRC <= MLIB_S16_MIN)                            \
		DST = MLIB_S16_MIN;                             \
	    else                                                \
		DST = (mlib_s16)SRC

#define	SAT_U16(DST, SRC)                                       \
	{                                                       \
	    mlib_d64 s0 = (mlib_d64)(SRC);                      \
	                                                        \
	    if (s0 > (mlib_d64)MLIB_U16_MAX)                    \
		s0 = (mlib_d64)MLIB_U16_MAX;                    \
	    if (s0 < (mlib_d64)MLIB_U16_MIN)                    \
		s0 = (mlib_d64)MLIB_U16_MIN;                    \
	    (DST) = (mlib_s32)s0;                               \
	}

#define	SAT32(DST, SRC)                                         \
	if (SRC >= MLIB_S32_MAX)                                \
	    DST = MLIB_S32_MAX;                                 \
	else                                                    \
	    if (SRC <= MLIB_S32_MIN)                            \
		DST = MLIB_S32_MIN;                             \
	    else                                                \
		DST = (mlib_s32)SRC

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_CLAMP_U8(dst, src)                                 \
	{                                                       \
	    mlib_s32 v = src, mask = (v - 0xff) >> 31;          \
	                                                        \
	    dst = (v | ~mask) & ~(v >> 31);                     \
	}

/* *********************************************************** */

#define	MLIB_CLAMP_S16(dst, src)                                  \
	{                                                         \
	    mlib_s32 s = src;                                     \
	                                                          \
	    dst =                                                 \
		(((s | ((32767 - s) >> 31)) & 0x7FFF) & ~(((s +   \
		32767) & s) >> 31) | ((s >> 31) & 0x8000));       \
	}

/* *********************************************************** */

/*
 * Convert a 1-bit image into an 8-bit image.
 */

void
mlib_c_ImageDataTypeConvert_BIT_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* intermediate value */
	mlib_s32 bitdata32;

/* indices for x, y and bit */
	mlib_s32 i, j, k;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			bitdata = (*sp++);
/* get bits 7 */
			dp[0] = (mlib_u8)((bitdata >> 7));
/* get bits 6 */
			dp[1] = (mlib_u8)((bitdata >> 6) & 1);
/* get bits 5 */
			dp[2] = (mlib_u8)((bitdata >> 5) & 1);
/* get bits 4 */
			dp[3] = (mlib_u8)((bitdata >> 4) & 1);
/* get bits 3 */
			dp[4] = (mlib_u8)((bitdata >> 3) & 1);
/* get bits 2 */
			dp[5] = (mlib_u8)((bitdata >> 2) & 1);
/* get bits 1 */
			dp[6] = (mlib_u8)((bitdata >> 1) & 1);
/* get bits 0 */
			dp[7] = (mlib_u8)((bitdata) & 1);
			dp += 8;
		}

		if (dsize & 7) {
			bitdata32 = (mlib_s32)((*sp++));
			for (k = 0; k < (dsize % 8); k++) {
				bitdata32 <<= 1;
				(*dp++) = (mlib_u8)(bitdata32 >> 8);
				bitdata32 &= 0xff;
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 1-bit image into a 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_BIT_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* intermediate value */
	mlib_s32 bitdata32;

/* indices for x, y and bit */
	mlib_s32 i, j, k;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_s16 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst) / 2;

	dsize = width * channels;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			bitdata = (*sp++);
/* get bits 7 */
			dp[0] = (mlib_s16)((bitdata >> 7));
/* get bits 6 */
			dp[1] = (mlib_s16)((bitdata >> 6) & 1);
/* get bits 5 */
			dp[2] = (mlib_s16)((bitdata >> 5) & 1);
/* get bits 4 */
			dp[3] = (mlib_s16)((bitdata >> 4) & 1);
/* get bits 3 */
			dp[4] = (mlib_s16)((bitdata >> 3) & 1);
/* get bits 2 */
			dp[5] = (mlib_s16)((bitdata >> 2) & 1);
/* get bits 1 */
			dp[6] = (mlib_s16)((bitdata >> 1) & 1);
/* get bits 0 */
			dp[7] = (mlib_s16)((bitdata) & 1);
			dp += 8;
		}

		if (dsize & 7) {
			bitdata32 = (mlib_s32)((*sp++));
			for (k = 0; k < (dsize % 8); k++) {
				bitdata32 <<= 1;
				(*dp++) = (mlib_s16)(bitdata32 >> 8);
				bitdata32 &= 0xff;
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

#ifdef _NO_LONGLONG

/*
 * Convert a 1-bit image into a 32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_BIT_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* intermediate value */
	mlib_s32 bitdata32;

/* indices for x, y and bit */
	mlib_s32 i, j, k;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_s32 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst) / 4;

	dsize = width * channels;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			bitdata = (*sp++);
/* get bits 7 */
			dp[0] = (mlib_s32)((bitdata >> 7));
/* get bits 6 */
			dp[1] = (mlib_s32)((bitdata >> 6) & 1);
/* get bits 5 */
			dp[2] = (mlib_s32)((bitdata >> 5) & 1);
/* get bits 4 */
			dp[3] = (mlib_s32)((bitdata >> 4) & 1);
/* get bits 3 */
			dp[4] = (mlib_s32)((bitdata >> 3) & 1);
/* get bits 2 */
			dp[5] = (mlib_s32)((bitdata >> 2) & 1);
/* get bits 1 */
			dp[6] = (mlib_s32)((bitdata >> 1) & 1);
/* get bits 0 */
			dp[7] = (mlib_s32)((bitdata) & 1);
			dp += 8;
		}

		if (dsize & 7) {
			bitdata32 = (mlib_s32)((*sp++));
			for (k = 0; k < (dsize % 8); k++) {
				bitdata32 <<= 1;
				(*dp++) = (mlib_s32)(bitdata32 >> 8);
				bitdata32 &= 0xff;
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

#else /* _NO_LONGLONG */

void
mlib_c_ImageDataTypeConvert_BIT_S32_A8D1X8(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* intermediate value */
	mlib_u8 bitdata;

/* indices for x, y and bit */
	mlib_s32 i;
	mlib_d64 *pt;

	sp = (void *)src;
	dp = (mlib_d64 *)dst;
	pt = (mlib_d64 *)mlib_table;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < dsize / 8; i++) {
		bitdata = sp[i];
		dp[0] = pt[(bitdata >> 6)];
		dp[1] = pt[(bitdata >> 4) & 3];
		dp[2] = pt[(bitdata >> 2) & 3];
		dp[3] = pt[(bitdata & 3)];
		dp += 4;
	}
}

void
mlib_c_ImageDataTypeConvert_BIT_S32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer for src */
	mlib_u8 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;

/* indices for x, y */
	mlib_s32 i, j;

/* intermediate value */
	mlib_u8 bitdata;
	mlib_d64 *pt;

	sp = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;
	pt = (mlib_d64 *)mlib_table;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize / 8; i++) {
			bitdata = sp[i];
			dp[0] = pt[(bitdata >> 6)];
			dp[1] = pt[(bitdata >> 4) & 3];
			dp[2] = pt[(bitdata >> 2) & 3];
			dp[3] = pt[(bitdata & 3)];
			dp += 4;
		}

		sp = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

void
mlib_c_ImageDataTypeConvert_BIT_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_s32 *da, *dp32;

/* end point in destination */
	mlib_s32 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_u8 sd0, sd1;
	mlib_u8 dd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;
	mlib_s32 i, k, num;
	mlib_d64 *pt;

/* intermediate value */
	mlib_s32 bitdata32;

	sa = (void *)src;
	da = dst;
	pt = (mlib_d64 *)mlib_table;

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = ((mlib_u8 *)da - (mlib_u8 *)dp) >> 2;
	dend = da + dsize - 1;
	sd1 = (*sa++);

/* left end handling */

	if ((mlib_s32 *)dp != da) {
		*da = (mlib_s32)((sd1 >> 7));
		dp++;
	} else {
		if (da == dend) {
			*da = (mlib_s32)((sd1 >> 7));
			dp++;
		} else {
			(*dp++) = pt[(sd1 >> 6)];
		}
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		sd0 = sd1;
		sd1 = (*sa++);
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		(*dp++) = pt[(dd1 >> 6)];
		(*dp++) = pt[(dd1 >> 4) & 3];
		(*dp++) = pt[(dd1 >> 2) & 3];
		(*dp++) = pt[(dd1 & 3)];
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		dp32 = (mlib_s32 *)dp;
		num = (dend + 1) - dp32;
		sd0 = sd1;
		sd1 = (*sa++);
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		bitdata32 = (mlib_s32)dd1;
		for (k = 0; k < num; k++) {
			bitdata32 <<= 1;
			(*dp32++) = (mlib_s32)(bitdata32 >> 8);
			bitdata32 &= 0xff;
		}
	}
}

/* 2-D non-aligned image */

void
mlib_c_ImageDataTypeConvert_BIT_S32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sa, *sl;
	mlib_s32 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;
/* in pixel */
	dlb >>= 2;
	for (j = 0; j < ysize; j++) {
		mlib_c_ImageDataTypeConvert_BIT_S32_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */

void
mlib_c_ImageDataTypeConvert_BIT_F32_A8D1X8(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* intermediate value */
	mlib_u8 bitdata;

/* indices for x, y and bit */
	mlib_s32 i;
	mlib_d64 *pt;

	sp = (void *)src;
	dp = (mlib_d64 *)dst;
	pt = (mlib_d64 *)mlib_table_F32;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < dsize / 8; i++) {
		bitdata = sp[i];
		dp[0] = pt[(bitdata >> 6)];
		dp[1] = pt[(bitdata >> 4) & 3];
		dp[2] = pt[(bitdata >> 2) & 3];
		dp[3] = pt[(bitdata & 3)];
		dp += 4;
	}
}

/* *********************************************************** */

void
mlib_c_ImageDataTypeConvert_BIT_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer for src */
	mlib_u8 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;

/* indices for x, y */
	mlib_s32 i, j;

/* intermediate value */
	mlib_u8 bitdata;
	mlib_d64 *pt;

	sp = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;
	pt = (mlib_d64 *)mlib_table_F32;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize / 8; i++) {
			bitdata = sp[i];
			dp[0] = pt[(bitdata >> 6)];
			dp[1] = pt[(bitdata >> 4) & 3];
			dp[2] = pt[(bitdata >> 2) & 3];
			dp[3] = pt[(bitdata & 3)];
			dp += 4;
		}

		sp = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageDataTypeConvert_BIT_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_f32 *da, *dp32;

/* end point in destination */
	mlib_f32 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_u8 sd0, sd1;
	mlib_u8 dd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;
	mlib_s32 i, k, num;
	mlib_d64 *pt;

/* intermediate value */
	mlib_s32 bitdata32;

	sa = (void *)src;
	da = dst;
	pt = (mlib_d64 *)mlib_table_F32;

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = ((mlib_u8 *)da - (mlib_u8 *)dp) >> 2;
	dend = da + dsize - 1;
	sd1 = (*sa++);

/* left end handling */

	if ((mlib_f32 *)dp != da) {
		*da = (mlib_f32)((sd1 >> 7));
		dp++;
	} else {
		if (da == dend) {
			*da = (mlib_f32)((sd1 >> 7));
			dp++;
		} else {
			(*dp++) = pt[(sd1 >> 6)];
		}
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		sd0 = sd1;
		sd1 = (*sa++);
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		(*dp++) = pt[(dd1 >> 6)];
		(*dp++) = pt[(dd1 >> 4) & 3];
		(*dp++) = pt[(dd1 >> 2) & 3];
		(*dp++) = pt[(dd1 & 3)];
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		dp32 = (mlib_f32 *)dp;
		num = (dend + 1) - dp32;
		sd0 = sd1;
		if (num > (6 + off))
			sd1 = *sa;
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		bitdata32 = (mlib_s32)dd1;
		for (k = 0; k < num; k++) {
			bitdata32 <<= 1;
			(*dp32++) = (mlib_f32)(bitdata32 >> 8);
			bitdata32 &= 0xff;
		}
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_c_ImageDataTypeConvert_BIT_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sa, *sl;
	mlib_f32 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;
/* in pixel */
	dlb >>= 2;
	for (j = 0; j < ysize; j++) {
		mlib_c_ImageDataTypeConvert_BIT_F32_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}
}

/* *********************************************************** */

/*
 * Convert a 1-bit image into a d64-bit image.
 */

void
mlib_c_ImageDataTypeConvert_BIT_D64(
    const mlib_image *src,
    mlib_image *dst)
{
	mlib_s32 buf[SIZE + 8], *sbuf;

/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_d64 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* indices for x, y and bit */
	mlib_s32 i, j, num, size;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sl = (mlib_u8 *)mlib_ImageGetData(src);
	dl = (mlib_d64 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst) >> 3;

	dsize = width * channels;

	for (j = 0; j < height; j++) {

		for (size = 0; size < dsize; size += num) {

			sp = sl + size / 8;
			dp = dl + size;

			num = dsize - size;

			if (num > SIZE)
				num = SIZE;

			sbuf = (mlib_s32 *)buf;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (num - 1) / 8 + 1; i++) {
				bitdata = (*sp++);
				sbuf[0] = ((bitdata >> 7));
				sbuf[1] = ((bitdata >> 6) & 1);
				sbuf[2] = ((bitdata >> 5) & 1);
				sbuf[3] = ((bitdata >> 4) & 1);
				sbuf[4] = ((bitdata >> 3) & 1);
				sbuf[5] = ((bitdata >> 2) & 1);
				sbuf[6] = ((bitdata >> 1) & 1);
				sbuf[7] = ((bitdata) & 1);
				sbuf += 8;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dp[i] = (mlib_d64)buf[i];
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 8-bit image into a 1-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U8_BIT(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_s32 bitdata;

/* indices for x, y and bit */
	mlib_s32 i, j;
	mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			s0 = -sp[0];
			s1 = -sp[1];
			s2 = -sp[2];
			s3 = -sp[3];
			s4 = -sp[4];
			s5 = -sp[5];
			s6 = -sp[6];
			s7 = -sp[7];
			bitdata = (s0 & 0x80000000) | (s1 & 0x40000000) |
			    (s2 & 0x20000000) | (s3 & 0x10000000) |
			    (s4 & 0x08000000) | (s5 & 0x04000000) |
			    (s6 & 0x02000000) | (s7 & 0x01000000);
			(*dp++) = bitdata >> 24;
			sp += 8;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
				s0 = -sp[0];
				bitdata |= (s0 & and);
				and >>= 1;
				sp++;

			}
			dl[dsize / 8] = (bitdata >> 24) |
				(dl[dsize / 8] & ~mask);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 8-bit image into a 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U8_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for pixel in destination */
	mlib_s16 *dend;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s32 *dp2;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_u32 val0, val1;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src);
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
		dend = dp + width * channels;

		if ((mlib_addr)dp & 3) {
			(*dp++) = (*sp++);
		}

		dp2 = (mlib_s32 *)dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_u16 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s16 *)dp2 <= (dend - 2); dp2++) {
				val0 = (*sp2++);
				dp2[0] = ((val0 & 0xFF00) << 8) | (val0 & 0xFF);
			}

			sp = (mlib_u8 *)sp2;
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s16 *)dp2 <= (dend - 2); dp2++) {
				val0 = sp[0];
				val1 = sp[1];
#ifdef _LITTLE_ENDIAN
				dp2[0] = val0 | (val1 << 16);
#else /* _LITTLE_ENDIAN */
				dp2[0] = (val0 << 16) | val1;
#endif /* _LITTLE_ENDIAN */
				sp += 2;
			}
		}

		dp = (mlib_s16 *)dp2;

		if (dp < dend) {
			*dp = *sp;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 8-bit image into a 32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U8_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_s32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src);
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			(*dp++) = (mlib_s32)((*sp++));
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 8-bit image into a f32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U8_F32(
    const mlib_image *src,
    mlib_image *dst)
{
	const mlib_f32 *lut[4] =
	    { mlib_U82F32, mlib_U82F32, mlib_U82F32, mlib_U82F32 };

	__mlib_ImageLookUp(dst, src, (const void **)lut);
}

/* *********************************************************** */

/*
 * Convert an 8-bit image into a d64-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U8_D64(
    const mlib_image *src,
    mlib_image *dst)
{
	const mlib_d64 *lut[4] =
	    { mlib_U82D64, mlib_U82D64, mlib_U82D64, mlib_U82D64 };

	__mlib_ImageLookUp(dst, src, (const void **)lut);
}

/* *********************************************************** */

/*
 * Convert a 16-bit image into a 1-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U16_BIT(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_u32 s0, s1, s2, s3, s4, s5, s6, s7, bitdata;
	mlib_s32 dsize;

/* indices for x, y and bit */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u16 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			s0 = -sp[0];
			s1 = -sp[1];
			s2 = -sp[2];
			s3 = -sp[3];
			s4 = -sp[4];
			s5 = -sp[5];
			s6 = -sp[6];
			s7 = -sp[7];
			bitdata = (s0 & 0x80000000) |
			    (s1 & 0x40000000) |
			    (s2 & 0x20000000) |
			    (s3 & 0x10000000) |
			    (s4 & 0x08000000) |
			    (s5 & 0x04000000) | (s6 & 0x02000000) | (s7 &
			    0x01000000);
			(*dp++) = bitdata >> 24;
			sp += 8;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
				s0 = -sp[0];
				bitdata |= (s0 & and);
				and >>= 1;
				sp++;

			}
			dl[dsize / 8] = (bitdata >> 24) |
				(dl[dsize / 8] & ~mask);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 16-bit image into an 8-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U16_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_u16 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			MLIB_CLAMP_U8(*dp, sd);
			dp++;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

void
mlib_c_ImageDataTypeConvert_U16_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width, height, channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_u16 *)mlib_ImageGetData(src);
	dp = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			(*dp++) =
			    (((((mlib_s32)(32767 - sd)) >> 31) | sd) & 0x7fff);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 16-bit image into a 32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U16_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_s32 *dl, *dend;
	mlib_u32 *sp2;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;
	mlib_s32 dsize;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_u32 val0, val1;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	dsize = width * channels;

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	sp = sl = (mlib_u16 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s32 *)mlib_ImageGetData(dst);

	for (j = 0; j < height; j++) {
		dend = dp + dsize;

		if ((mlib_addr)sp & 3) {
			(*dp++) = (*sp++);
		}

		sp2 = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dp <= (dend - 4); dp += 4) {
#ifdef _LITTLE_ENDIAN
			val0 = sp2[0];
			dp[0] = (val0 << 16) >> 16;
			dp[1] = (val0 >> 16);

			val1 = sp2[1];
			dp[2] = (val1 << 16) >> 16;
			dp[3] = (val1 >> 16);
#else /* _LITTLE_ENDIAN */
			val0 = sp2[0];
			dp[0] = (val0 >> 16);
			dp[1] = (val0 << 16) >> 16;

			val1 = sp2[1];
			dp[2] = (val1 >> 16);
			dp[3] = (val1 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */

			sp2 += 2;
		}

		sp = (mlib_u16 *)sp2;
		while (dp < dend) {
			(*dp++) = (*sp++);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 16-bit image into a f32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U16_F32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_f32 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;
	mlib_u32 *sp2;
	mlib_s32 buf_arr[SIZE + 6], *buf, *bend;
	TYPE_64BIT *buf2, *dp2;
	mlib_f32 *dend;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_u16 *)mlib_ImageGetData(src);
	dl = (mlib_f32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;

			buf = buf_arr + (((mlib_addr)dp & 7) >> 2);
			bend = buf + num;

			if ((mlib_addr)sp & 3) {
				(*buf++) = (*sp++);
			}

			sp2 = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; buf < (bend - 2); buf += 4) {
				mlib_u32 val0, val1;

#ifdef _LITTLE_ENDIAN
				val0 = sp2[0];
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);

				val1 = sp2[1];
				buf[2] = (val1 << 16) >> 16;
				buf[3] = (val1 >> 16);
#else /* _LITTLE_ENDIAN */
				val0 = sp2[0];
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;

				val1 = sp2[1];
				buf[2] = (val1 >> 16);
				buf[3] = (val1 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */

				sp2 += 2;
			}

			if (buf < bend) {
				mlib_u32 val0 = sp2[0];
#ifdef _LITTLE_ENDIAN
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);
#else /* _LITTLE_ENDIAN */
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
			}

			dend = dp + num;
			buf = buf_arr;

			if ((mlib_addr)dp & 7) {
				(*dp++) = (mlib_f32)buf[1];
				buf += 2;
			}

			buf2 = (TYPE_64BIT *) buf;
			dp2 = (TYPE_64BIT *) dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*buf2++);
				dd.f32s.f0 = (mlib_f32)dd.i32s.f0;
				dd.f32s.f1 = (mlib_f32)dd.i32s.f1;
				dp2[0] = dd.d64;
			}

			if ((mlib_f32 *)dp2 < dend) {
				((mlib_f32 *)dp2)[0] =
				    (mlib_f32)((mlib_s32 *)buf2)[0];
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 16-bit image into a d64-bit image.
 */

void
mlib_c_ImageDataTypeConvert_U16_D64(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_d64 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;
	mlib_u32 *sp2;
	mlib_s32 buf_arr[SIZE + 4], *buf, *bend;
	mlib_u32 val0, val1;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_u16 *)mlib_ImageGetData(src);
	dl = (mlib_d64 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 3;

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;

			buf = buf_arr;
			bend = buf_arr + num;

			if ((mlib_addr)sp & 3) {
				(*buf++) = (*sp++);
			}

			sp2 = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; buf < (bend - 2); buf += 4) {
#ifdef _LITTLE_ENDIAN
				val0 = sp2[0];
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);

				val1 = sp2[1];
				buf[2] = (val1 << 16) >> 16;
				buf[3] = (val1 >> 16);
#else /* _LITTLE_ENDIAN */
				val0 = sp2[0];
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;

				val1 = sp2[1];
				buf[2] = (val1 >> 16);
				buf[3] = (val1 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */

				sp2 += 2;
			}

			if (buf < bend) {
				val0 = sp2[0];
#ifdef _LITTLE_ENDIAN
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);
#else /* _LITTLE_ENDIAN */
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				dp[i] = buf_arr[i];
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 16-bit image into a 1-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S16_BIT(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_u32 s0, s1, s2, s3, s4, s5, s6, s7, bitdata;
	mlib_s32 dsize;

/* indices for x, y and bit */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_s16 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			s0 = -sp[0];
			s1 = -sp[1];
			s2 = -sp[2];
			s3 = -sp[3];
			s4 = -sp[4];
			s5 = -sp[5];
			s6 = -sp[6];
			s7 = -sp[7];
			bitdata = (s0 & 0x80000000) |
			    (s1 & 0x40000000) |
			    (s2 & 0x20000000) |
			    (s3 & 0x10000000) |
			    (s4 & 0x08000000) |
			    (s5 & 0x04000000) | (s6 & 0x02000000) | (s7 &
			    0x01000000);
			(*dp++) = bitdata >> 24;
			sp += 8;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
				s0 = -sp[0];
				bitdata |= (s0 & and);
				and >>= 1;
				sp++;

			}
			dl[dsize / 8] = (bitdata >> 24) |
				(dl[dsize / 8] & ~mask);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 16-bit image into an 8-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S16_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_s16 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			MLIB_CLAMP_U8(*dp, sd);
			dp++;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

void
mlib_c_ImageDataTypeConvert_S16_U16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_u16 *dl;
	mlib_s32 width, height, channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_s16 *)mlib_ImageGetData(src);
	dp = (mlib_u16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			(*dp++) = sd & ~(sd >> 31);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 16-bit image into a 32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S16_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	mlib_s32 *sp2, *dend;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;
	mlib_s32 dsize;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 val0, val1;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	dsize = width * channels;

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	sp = sl = (mlib_s16 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s32 *)mlib_ImageGetData(dst);

	for (j = 0; j < height; j++) {
		dend = dp + dsize;

		if ((mlib_addr)sp & 3) {
			(*dp++) = (*sp++);
		}

		sp2 = (mlib_s32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dp <= (dend - 4); dp += 4) {
#ifdef _LITTLE_ENDIAN
			val0 = sp2[0];
			dp[0] = (val0 << 16) >> 16;
			dp[1] = (val0 >> 16);

			val1 = sp2[1];
			dp[2] = (val1 << 16) >> 16;
			dp[3] = (val1 >> 16);
#else /* _LITTLE_ENDIAN */
			val0 = sp2[0];
			dp[0] = (val0 >> 16);
			dp[1] = (val0 << 16) >> 16;

			val1 = sp2[1];
			dp[2] = (val1 >> 16);
			dp[3] = (val1 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */

			sp2 += 2;
		}

		sp = (mlib_s16 *)sp2;
		while (dp < dend) {
			(*dp++) = (*sp++);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 16-bit image into a f32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S16_F32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_f32 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;
	mlib_s32 *sp2;
	mlib_s32 buf_arr[SIZE + 6], *buf, *bend;
	TYPE_64BIT *buf2, *dp2;
	mlib_f32 *dend;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_s16 *)mlib_ImageGetData(src);
	dl = (mlib_f32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;

			buf = buf_arr + (((mlib_addr)dp & 7) >> 2);
			bend = buf + num;

			if ((mlib_addr)sp & 3) {
				(*buf++) = (*sp++);
			}

			sp2 = (mlib_s32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; buf < (bend - 2); buf += 4) {
				mlib_s32 val0, val1;
#ifdef _LITTLE_ENDIAN
				val0 = sp2[0];
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);

				val1 = sp2[1];
				buf[2] = (val1 << 16) >> 16;
				buf[3] = (val1 >> 16);
#else /* _LITTLE_ENDIAN */
				val0 = sp2[0];
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;

				val1 = sp2[1];
				buf[2] = (val1 >> 16);
				buf[3] = (val1 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */

				sp2 += 2;
			}

			if (buf < bend) {
				mlib_s32 val0 = sp2[0];
#ifdef _LITTLE_ENDIAN
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);
#else /* _LITTLE_ENDIAN */
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
			}

			dend = dp + num;
			buf = buf_arr;

			if ((mlib_addr)dp & 7) {
				(*dp++) = (mlib_f32)buf[1];
				buf += 2;
			}

			buf2 = (TYPE_64BIT *) buf;
			dp2 = (TYPE_64BIT *) dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*buf2++);
				dd.f32s.f0 = (mlib_f32)dd.i32s.f0;
				dd.f32s.f1 = (mlib_f32)dd.i32s.f1;
				dp2[0] = dd.d64;
			}

			if ((mlib_f32 *)dp2 < dend) {
				((mlib_f32 *)dp2)[0] =
				    (mlib_f32)((mlib_s32 *)buf2)[0];
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 16-bit image into a d64-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S16_D64(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_d64 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;
	mlib_s32 *sp2;
	mlib_s32 buf_arr[SIZE + 4], *buf, *bend;
	mlib_s32 val0, val1;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_s16 *)mlib_ImageGetData(src);
	dl = (mlib_d64 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 1;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 3;

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;

			buf = buf_arr;
			bend = buf_arr + num;

			if ((mlib_addr)sp & 3) {
				(*buf++) = (*sp++);
			}

			sp2 = (mlib_s32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; buf < (bend - 2); buf += 4) {
#ifdef _LITTLE_ENDIAN
				val0 = sp2[0];
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);

				val1 = sp2[1];
				buf[2] = (val1 << 16) >> 16;
				buf[3] = (val1 >> 16);
#else /* _LITTLE_ENDIAN */
				val0 = sp2[0];
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;

				val1 = sp2[1];
				buf[2] = (val1 >> 16);
				buf[3] = (val1 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */

				sp2 += 2;
			}

			if (buf < bend) {
				val0 = sp2[0];
#ifdef _LITTLE_ENDIAN
				buf[0] = (val0 << 16) >> 16;
				buf[1] = (val0 >> 16);
#else /* _LITTLE_ENDIAN */
				buf[0] = (val0 >> 16);
				buf[1] = (val0 << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				dp[i] = buf_arr[i];
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 32-bit image into a 1-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S32_BIT(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_s16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_s32 bitdata;

/* indices for x, y and bit */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_s16 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in s16's */
	strides = mlib_ImageGetStride(src) >> 1;
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
		mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
#ifdef _LITTLE_ENDIAN
			s0 = -(sp[1] | ((mlib_u16 *)sp)[0]);
			s1 = -(sp[3] | ((mlib_u16 *)sp)[2]);
			s2 = -(sp[5] | ((mlib_u16 *)sp)[4]);
			s3 = -(sp[7] | ((mlib_u16 *)sp)[6]);
			s4 = -(sp[9] | ((mlib_u16 *)sp)[8]);
			s5 = -(sp[11] | ((mlib_u16 *)sp)[10]);
			s6 = -(sp[13] | ((mlib_u16 *)sp)[12]);
			s7 = -(sp[15] | ((mlib_u16 *)sp)[14]);
#else /* _LITTLE_ENDIAN */
			s0 = -(sp[0] | ((mlib_u16 *)sp)[1]);
			s1 = -(sp[2] | ((mlib_u16 *)sp)[3]);
			s2 = -(sp[4] | ((mlib_u16 *)sp)[5]);
			s3 = -(sp[6] | ((mlib_u16 *)sp)[7]);
			s4 = -(sp[8] | ((mlib_u16 *)sp)[9]);
			s5 = -(sp[10] | ((mlib_u16 *)sp)[11]);
			s6 = -(sp[12] | ((mlib_u16 *)sp)[13]);
			s7 = -(sp[14] | ((mlib_u16 *)sp)[15]);
#endif /* _LITTLE_ENDIAN */
			bitdata = (s0 & 0x80000000) |
			    (s1 & 0x40000000) |
			    (s2 & 0x20000000) |
			    (s3 & 0x10000000) |
			    (s4 & 0x08000000) |
			    (s5 & 0x04000000) |
			    (s6 & 0x02000000) |
			    (s7 & 0x01000000);

			(*dp++) = bitdata >> 24;
			sp += 16;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
#ifdef _LITTLE_ENDIAN
				s0 = -(sp[1] | ((mlib_u16 *)sp)[0]);
#else /* _LITTLE_ENDIAN */
				s0 = -(sp[0] | ((mlib_u16 *)sp)[1]);
#endif /* _LITTLE_ENDIAN */
				bitdata |= (s0 & and);
				and >>= 1;
				sp += 2;

			}
			dl[dsize / 8] = (bitdata >> 24) |
				(dl[dsize / 8] & ~mask);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 32-bit image into an 8-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S32_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_s32 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_s32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			MLIB_CLAMP_U8(*dp, sd);
			dp++;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 32-bit image into an 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S32_U16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

/* pointer for line in source */
	mlib_s32 *sl;

/* pointer for line in destination */
	mlib_u16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_s32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_u16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			(*dp++) = (sd | ((65535 - sd) >> 31)) & ~(sd >> 31);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 32-bit image into an 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S32_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_s32 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_s32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			sd = (*sp++);
			MLIB_CLAMP_S16(*dp, sd);
			dp++;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 32-bit image into a f32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S32_F32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for line in source */
	mlib_s32 *sl;

/* pointer for line in destination */
	mlib_f32 *dl;
	TYPE_64BIT *sp2, *dp2;
	mlib_f32 *dend;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_s32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_f32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {
		dend = dp + width * channels;

		if ((mlib_addr)dp & 7) {
			(*dp++) = (mlib_f32)(*sp++);
		}

		dp2 = (TYPE_64BIT *) dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (TYPE_64BIT *) sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*sp2++);
				dd.f32s.f0 = (mlib_f32)dd.i32s.f0;
				dd.f32s.f1 = (mlib_f32)dd.i32s.f1;
				dp2[0] = dd.d64;
			}

			sp = (mlib_s32 *)sp2;
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.f32s.f0 = (mlib_f32)(*sp++);
				dd.f32s.f1 = (mlib_f32)(*sp++);
				dp2[0] = dd.d64;
			}
		}

		dp = (mlib_f32 *)dp2;

		if (dp < dend) {
			*dp = (mlib_f32)*sp;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 32-bit image into a d64-bit image.
 */

void
mlib_c_ImageDataTypeConvert_S32_D64(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* pointer for line in source */
	mlib_s32 *sl;

/* pointer for line in destination */
	mlib_d64 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_s32 *)mlib_ImageGetData(src);
	dp = (mlib_d64 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 3;

	sl = sp;
	dl = dp;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			(*dp++) = (mlib_d64)((*sp++));
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 8-bit image.
 */

void
mlib_c_ImageDataTypeConvert_F32_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s32 buf[SIZE];
#endif /* MLIB_USE_FTOI_CLAMPING */
/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_f32 *)mlib_ImageGetData(src);
	dl = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;

#ifdef MLIB_USE_FTOI_CLAMPING
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] =
				    (mlib_s32)((sp[i] - 128.f) * 16777216.f);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				dp[i] = (buf[i] >> 24) + 128;
#else /* MLIB_USE_FTOI_CLAMPING */
			for (i = 0; i < num; i++) {
				SAT8(dp[i], sp[i]);
			}

#endif /* MLIB_USE_FTOI_CLAMPING */
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_F32_U16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_u16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_u16 *dp1;
	mlib_s32 *dp2;
	mlib_s32 size, num, n;
	mlib_s32 buf[SIZE / 2 + 2];
#endif /* MLIB_USE_FTOI_CLAMPING */
/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_f32 *)mlib_ImageGetData(src);
	dl = (mlib_u16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {

#ifdef MLIB_USE_FTOI_CLAMPING
		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;
			n = num;

			if ((mlib_addr)dp & 3) {
				(*dp++) =
				    ((mlib_s32)((((*sp++)) -
				    32767.5) * 65536.0f) >> 16) ^ 0x8000;
				n--;
			}

			dp2 = (mlib_s32 *)dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				dp2[i] =
				    (mlib_s32)((sp[2 * i] -
				    32767.5) * 65536.0f) ^ 0x80000000;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				buf[i] =
				    (mlib_s32)((sp[2 * i + 1] -
				    32767.5) * 65536.0f);
			}

			dp1 = dp + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++)
				dp1[2 * i] = (buf[i] >> 16) ^ 0x8000;

			if (n & 1) {
				dp[n - 1] =
				    ((mlib_s32)((sp[n - 1] -
				    32767.5) * 65536.0f) >> 16) ^ 0x8000;
			}
		}

#else /* MLIB_USE_FTOI_CLAMPING */
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT_U16((*dp++), *sp);
			sp++;
		}

#endif /* MLIB_USE_FTOI_CLAMPING */

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_F32_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s16 *dp1;
	mlib_s32 *dp2;
	mlib_s32 size, num, n;
	mlib_s32 buf[SIZE / 2 + 2];
#endif /* MLIB_USE_FTOI_CLAMPING */
/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_f32 *)mlib_ImageGetData(src);
	dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {

#ifdef MLIB_USE_FTOI_CLAMPING
		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;
			n = num;

			if ((mlib_addr)dp & 3) {
				(*dp++) = (mlib_s32)((*sp++) * 65536.0f) >> 16;
				n--;
			}

			dp2 = (mlib_s32 *)dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				dp2[i] = (mlib_s32)(sp[2 * i] * 65536.0f);
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				buf[i] = (mlib_s32)(sp[2 * i + 1] * 65536.0f);
			}

			dp1 = dp + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++)
				dp1[2 * i] = (buf[i] >> 16);

			if (n & 1) {
				dp[n - 1] =
				    (mlib_s32)(sp[n - 1] * 65536.0f) >> 16;
			}
		}

#else /* MLIB_USE_FTOI_CLAMPING */
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT16((*dp++), *sp);
			sp++;
		}

#endif /* MLIB_USE_FTOI_CLAMPING */

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_F32_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	TYPE_64BIT *sp2, *dp2;
	mlib_s32 *dend;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_f32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {
		dend = dp + width * channels;

		if ((mlib_addr)dp & 7) {
			SAT32((*dp++), *sp);
			sp++;
		}

		dp2 = (TYPE_64BIT *) dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (TYPE_64BIT *) sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*sp2++);
				SAT32(dd.i32s.f0, dd.f32s.f0);
				SAT32(dd.i32s.f1, dd.f32s.f1);
				dp2[0] = dd.d64;
			}

			sp = (mlib_f32 *)sp2;
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				SAT32(dd.i32s.f0, *sp);
				sp++;
				SAT32(dd.i32s.f1, *sp);
				sp++;
				dp2[0] = dd.d64;
			}
		}

		dp = (mlib_s32 *)dp2;

		if (dp < dend) {
			SAT32(*dp, *sp);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */

/* *********************************************************** */

/*
 * Convert an f32-bit image into a d64-bit image.
 */

void
mlib_c_ImageDataTypeConvert_F32_D64(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_d64 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = (mlib_f32 *)mlib_ImageGetData(src);
	dp = (mlib_d64 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 3;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			(*dp++) = (mlib_d64)((*sp++));
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an d64-bit image into a 1-bit image.
 */

void
mlib_c_ImageDataTypeConvert_D64_BIT(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_s32 bitdata;

/* indices for x, y and bit */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = sl = mlib_ImageGetData(src);
	dp = dl = mlib_ImageGetData(dst);

/* in u16's */
	strides = mlib_ImageGetStride(src) >> 1;
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	for (j = 0; j < height; j++) {
		mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
#ifdef _LITTLE_ENDIAN
			s0 = -(((mlib_s16 *)sp)[3] | sp[0] | sp[1] | sp[2]);
			s1 = -(((mlib_s16 *)sp)[7] | sp[4] | sp[5] | sp[6]);
			s2 = -(((mlib_s16 *)sp)[11] | sp[8] | sp[9] | sp[10]);
			s3 = -(((mlib_s16 *)sp)[15] | sp[12] | sp[13] | sp[14]);
			s4 = -(((mlib_s16 *)sp)[19] | sp[16] | sp[17] | sp[18]);
			s5 = -(((mlib_s16 *)sp)[23] | sp[20] | sp[21] | sp[22]);
			s6 = -(((mlib_s16 *)sp)[27] | sp[24] | sp[25] | sp[26]);
			s7 = -(((mlib_s16 *)sp)[31] | sp[28] | sp[29] | sp[30]);
#else /* _LITTLE_ENDIAN */
			s0 = -(((mlib_s16 *)sp)[0] | sp[1] | sp[2] | sp[3]);
			s1 = -(((mlib_s16 *)sp)[4] | sp[5] | sp[6] | sp[7]);
			s2 = -(((mlib_s16 *)sp)[8] | sp[9] | sp[10] | sp[11]);
			s3 = -(((mlib_s16 *)sp)[12] | sp[13] | sp[14] | sp[15]);
			s4 = -(((mlib_s16 *)sp)[16] | sp[17] | sp[18] | sp[19]);
			s5 = -(((mlib_s16 *)sp)[20] | sp[21] | sp[22] | sp[23]);
			s6 = -(((mlib_s16 *)sp)[24] | sp[25] | sp[26] | sp[27]);
			s7 = -(((mlib_s16 *)sp)[28] | sp[29] | sp[30] | sp[31]);
#endif /* _LITTLE_ENDIAN */
			bitdata = (s0 & 0x80000000) |
			    (s1 & 0x40000000) |
			    (s2 & 0x20000000) |
			    (s3 & 0x10000000) |
			    (s4 & 0x08000000) |
			    (s5 & 0x04000000) | (s6 & 0x02000000) | (s7 &
			    0x01000000);

			(*dp++) = bitdata >> 24;
			sp += 32;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
#ifdef _LITTLE_ENDIAN
				s0 = -(((mlib_s16 *)sp)[3] | sp[0] | sp[1] |
					sp[2]);
#else /* _LITTLE_ENDIAN */
				s0 = -(((mlib_s16 *)sp)[0] | sp[1] | sp[2] |
					sp[3]);
#endif /* _LITTLE_ENDIAN */
				bitdata |= (s0 & and);
				and >>= 1;
				sp += 4;

			}
			dl[dsize / 8] = (bitdata >> 24) |
				(dl[dsize / 8] & ~mask);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 8-bit image.
 */

void
mlib_c_ImageDataTypeConvert_D64_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s32 buf[SIZE];
#endif /* MLIB_USE_FTOI_CLAMPING */
/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_d64 *)mlib_ImageGetData(src);
	dl = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;

#ifdef MLIB_USE_FTOI_CLAMPING
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] = (mlib_s32)((sp[i] - 128.) * 16777216.);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				dp[i] = (buf[i] >> 24) + 128;
#else /* MLIB_USE_FTOI_CLAMPING */
			for (i = 0; i < num; i++) {
				SAT8(dp[i], sp[i]);
			}

#endif /* MLIB_USE_FTOI_CLAMPING */
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_D64_U16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_u16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_u16 *dp1;
	mlib_s32 *dp2;
	mlib_s32 size, num, n;
	mlib_s32 buf[SIZE / 2 + 2];
#endif /* MLIB_USE_FTOI_CLAMPING */
/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_d64 *)mlib_ImageGetData(src);
	dl = (mlib_u16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {

#ifdef MLIB_USE_FTOI_CLAMPING
		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;
			n = num;

			if ((mlib_addr)dp & 3) {
				(*dp++) =
				    ((mlib_s32)(((*sp++) -
				    32767.5) * 65536.0) >> 16) ^ 0x8000;
				n--;
			}

			dp2 = (mlib_s32 *)dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				dp2[i] =
				    (mlib_s32)((sp[2 * i] -
				    32767.5) * 65536.0) ^ 0x80000000;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				buf[i] =
				    (mlib_s32)((sp[2 * i + 1] -
				    32767.5) * 65536.0);
			}

			dp1 = dp + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++)
				dp1[2 * i] = (buf[i] >> 16) ^ 0x8000;

			if (n & 1) {
				dp[n - 1] =
				    ((mlib_s32)((sp[n - 1] -
				    32767.5) * 65536.0) >> 16) ^ 0x8000;
			}
		}

#else /* MLIB_USE_FTOI_CLAMPING */
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT_U16((*dp++), *sp);
			sp++;
		}

#endif /* MLIB_USE_FTOI_CLAMPING */

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 16-bit image.
 */

void
mlib_c_ImageDataTypeConvert_D64_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s16 *dp1;
	mlib_s32 *dp2;
	mlib_s32 size, num, n;
	mlib_s32 buf[SIZE / 2 + 2];
#endif /* MLIB_USE_FTOI_CLAMPING */
/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_d64 *)mlib_ImageGetData(src);
	dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {

#ifdef MLIB_USE_FTOI_CLAMPING
		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;
			n = num;

			if ((mlib_addr)dp & 3) {
				(*dp++) = (mlib_s32)((*sp++) * 65536.0) >> 16;
				n--;
			}

			dp2 = (mlib_s32 *)dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				dp2[i] = (mlib_s32)(sp[2 * i] * 65536.0);
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				buf[i] = (mlib_s32)(sp[2 * i + 1] * 65536.0);
			}

			dp1 = dp + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++)
				dp1[2 * i] = (buf[i] >> 16);

			if (n & 1) {
				dp[n - 1] =
				    (mlib_s32)(sp[n - 1] * 65536.0) >> 16;
			}
		}

#else /* MLIB_USE_FTOI_CLAMPING */
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT16((*dp++), *sp);
			sp++;
		}

#endif /* MLIB_USE_FTOI_CLAMPING */

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_D64_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_d64 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			SAT32(sd, *sp);
			sp++;
			(*dp++) = sd;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an d64-bit image into a f32-bit image.
 */

void
mlib_c_ImageDataTypeConvert_D64_F32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_f32 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_d64 *)mlib_ImageGetData(src);
	dp = dl = (mlib_f32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			(*dp++) = (mlib_f32)((*sp++));
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */
