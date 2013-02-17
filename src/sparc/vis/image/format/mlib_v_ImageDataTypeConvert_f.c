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

#pragma ident	"@(#)mlib_v_ImageDataTypeConvert_f.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageDataTypeConvert_BIT_U8_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_U8_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_U8_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_BIT_U8         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_BIT_S16_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_S16_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_S16_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_BIT_S16        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_BIT_S32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_S32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_S32_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_BIT_S32        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_BIT_F32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_F32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_BIT_F32_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_BIT_F32        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_BIT_D64        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U8_BIT_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_BIT_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_BIT_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U8_BIT         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U8_S16_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_S16_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_S16_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U8_S16         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U8_S32_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_S32_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_S32_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U8_S32         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U8_F32_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_F32_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U8_F32_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U8_F32         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U8_D64         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U16_BIT_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_BIT_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_BIT        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U16_U8_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_U8_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_U8_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U16_U8         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U16_S16_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_S16_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_S16_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U16_S16        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U16_S32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_S32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_S32_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U16_S32        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U16_F32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_F32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_F32_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U16_F32        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_U16_D64_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_D64_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_U16_D64_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_U16_D64        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S16_BIT_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_BIT_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_BIT        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S16_U8_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_U8_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_U8_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S16_U8         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S16_U16_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_U16_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_U16_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S16_U16         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S16_S32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_S32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_S32_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S16_S32        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S16_F32_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_F32_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_F32_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S16_F32        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S16_D64_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_D64_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S16_D64_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S16_D64        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S32_BIT_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_BIT_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_BIT        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S32_U8         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S32_U16_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_U16_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_U16_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S32_U16        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S32_S16_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_S16_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_S16_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S32_S16        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_S32_F32        - 2-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S32_D64        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_F32_BIT_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_F32_BIT_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_F32_BIT_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_F32_BIT        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_F32_U8_A8D1X8  - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_F32_U8_A8D2X8  - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_F32_U8_D1      - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_F32_U8         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_F32_U16        - 2-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_F32_S16        - 2-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_F32_S32        - 2-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_F32_D64        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_D64_BIT        - 2-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_D64_U8         - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_D64_U16_A8D1X4 - 1-D, Aligned8, dsize 4x
 *      mlib_v_ImageDataTypeConvert_D64_U16_A8D2X4 - 2-D, Aligned8, dsize 4x
 *      mlib_v_ImageDataTypeConvert_D64_U16_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_D64_U16        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_D64_S16_A8D1X4 - 1-D, Aligned8, dsize 4x
 *      mlib_v_ImageDataTypeConvert_D64_S16_A8D2X4 - 2-D, Aligned8, dsize 4x
 *      mlib_v_ImageDataTypeConvert_D64_S16_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_D64_S16        - 2-D, not aligned8
 *
 *      mlib_v_ImageDataTypeConvert_D64_S32        - 2-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_D64_F32        - 2-D, not aligned8
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      src     pointer to source image data
 *      dst     pointer to destination image data
 *      dsize   data size in pixels
 *      slb     source image line stride in bytes
 *      dlb     destination image line stride in bytes
 *      xsize   image width in pixels
 *      ysize   image height in lines
 *
 * RESTRICTION
 *      dst and src must have the same size and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Data format conversion.
 *      These functions are separated from mlib_v_ImageDataTypeConvert.c
 *      for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageDivTables.h>
#include <mlib_v_ImageDataTypeConvert.h>

/* *********************************************************** */

static const mlib_u64 mlib_table[4] =
{ 0x0ULL, 0x1ULL, 0x100000000ULL, 0x100000001ULL };

/* *********************************************************** */

#pragma align 8(mlib_table_F32)
static const mlib_f32 mlib_table_F32[8] =
{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

/* *********************************************************** */

typedef union
{
	mlib_d64 value;
	struct
	{
		mlib_s32 int0, int1;
	} twoint;
	struct
	{
		mlib_f32 float0, float1;
	} twofloat;
} type_union_mlib_d64;

/* *********************************************************** */

#define	SIZE	128

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT8(DST, SRC)	DST = ((mlib_u8) (SRC))

#define	SAT_U16(DST, SRC)                                                \
	(DST) =                                                          \
	    ((mlib_s32)(((SRC) - 32767.5f) * 65536.0f) >> 16) ^ 0x8000

#define	SAT16(DST, SRC)	DST = ((mlib_s16) (SRC))

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT8(DST, SRC)                                          \
	if (SRC >= MLIB_U8_MAX) {                               \
	    DST = MLIB_U8_MAX;                                  \
	} else {                                                \
	    if (SRC <= MLIB_U8_MIN) {                           \
		DST = MLIB_U8_MIN;                              \
	    } else {                                            \
	    DST = (mlib_u8)SRC}                                 \
	}

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

#define	SAT16(DST, SRC)                                         \
	if (SRC >= MLIB_S16_MAX) {                              \
	    DST = MLIB_S16_MAX;                                 \
	} else {                                                \
	    if (SRC <= MLIB_S16_MIN) {                          \
		DST = MLIB_S16_MIN;                             \
	    } else {                                            \
	    DST = (mlib_s16)SRC}                                \
	}

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

#define	LOAD_CONVERT_STORE_BIT_U8_A8                            \
	sd = *sa++;                                             \
	vis_pst_8(ones, dp, sd);                                \
	vis_pst_8(zeros, dp++, ~sd)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_U8                               \
	sd0 = sd1;                                              \
	sd1 = *sa++;                                            \
	sd = (sd0 << (8 - off)) | (sd1 >> off);                 \
	vis_pst_8(ones, dp, sd);                                \
	vis_pst_8(zeros, dp++, ~sd)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_U8_EMASK                         \
	sd0 = sd1;                                              \
	sd1 = vis_ld_u8_nf(sa); sa++;                           \
	sd = (sd0 << (8 - off)) | (sd1 >> off);                 \
	vis_pst_8(ones, dp, sd & emask);                        \
	vis_pst_8(zeros, dp++, (~sd) & emask)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_BIT_U8_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_u8 *sa;
	mlib_d64 *dp;
	mlib_d64 zeros = vis_fzero();
	mlib_d64 ones = vis_to_double_dup(0x01010101);
	mlib_s32 sd;
	mlib_s32 i;

	sa = (void *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_BIT_U8_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_BIT_U8_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer for src */
	mlib_u8 *sa, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 zeros = vis_fzero();
	mlib_d64 ones = vis_to_double_dup(0x01010101);
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	sa = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_BIT_U8_A8;
		}

		sa = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_BIT_U8_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_u8 *da;

/* end point in destination */
	mlib_u8 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_d64 zeros = vis_fzero();
	mlib_d64 ones = vis_to_double_dup(0x01010101);
	mlib_s32 sd0, sd1, sd;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)(mlib_addr)da & 7;
	dend = da + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

	sd1 = 0;
	LOAD_CONVERT_STORE_BIT_U8_EMASK;

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		LOAD_CONVERT_STORE_BIT_U8;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		LOAD_CONVERT_STORE_BIT_U8_EMASK;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_BIT_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_BIT_U8_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_S16_A8                           \
	sd0 = *sa++;                                            \
	dd0 = sd0 >> 4;                                         \
	vis_pst_16(ones, dp, dd0);                              \
	vis_pst_16(zeros, dp++, ~dd0);                          \
	vis_pst_16(ones, dp, sd0);                              \
	vis_pst_16(zeros, dp++, ~sd0)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_S16                                \
	sd0 = sd1;                                                \
	sd1 = *sa++;                                              \
	dd0 = (sd0 << (4 - off)) | ((sd1 >> (4 + off)) & 0x0f);   \
	vis_pst_16(ones, dp, dd0);                                \
	vis_pst_16(zeros, dp++, ~dd0);                            \
	dd1 = sd1 >> off;                                         \
	vis_pst_16(ones, dp, dd1);                                \
	vis_pst_16(zeros, dp++, ~dd1)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_S16_EMASK                          \
	sd0 = sd1;                                                \
	sd1 = vis_ld_u8_nf(sa); sa++;                             \
	dd0 = (sd0 << (4 - off)) | ((sd1 >> (4 + off)) & 0x0f);   \
	vis_pst_16(ones, dp, dd0 & emask);                        \
	vis_pst_16(zeros, dp++, (~dd0) & emask);                  \
	if ((mlib_addr)dp <= (mlib_addr)dend) {                   \
	    emask = vis_edge16(dp, dend);                         \
	    dd1 = sd1 >> off;                                     \
	    vis_pst_16(ones, dp, dd1 & emask);                    \
	    vis_pst_16(zeros, dp++, (~dd1) & emask);              \
	}

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_BIT_S16_A8D1X8(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_u8 *sa;
	mlib_d64 *dp;
	mlib_d64 zeros = vis_fzero();
	mlib_d64 ones = vis_to_double_dup(0x00010001);
	mlib_s32 sd0, dd0;
	mlib_s32 i;

	sa = (void *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_BIT_S16_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_BIT_S16_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer for src */
	mlib_u8 *sa, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 zeros = vis_fzero();
	mlib_d64 ones = vis_to_double_dup(0x00010001);
	mlib_s32 sd0, dd0;

/* indices for x, y */
	mlib_s32 i, j;

	sa = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_BIT_S16_A8;
		}

		sa = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_BIT_S16_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_s16 *da;

/* end point in destination */
	mlib_s16 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_d64 zeros = vis_fzero();
	mlib_d64 ones = vis_to_double_dup(0x00010001);
	mlib_s32 sd0, sd1;
	mlib_s32 dd0, dd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = ((mlib_addr)da & 7) >> 1;
	dend = da + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge16(da, dend);

	sd1 = 0;

	LOAD_CONVERT_STORE_BIT_S16_EMASK;

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 16;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		LOAD_CONVERT_STORE_BIT_S16;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		LOAD_CONVERT_STORE_BIT_S16_EMASK;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_BIT_S16(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sa, *sl;
	mlib_s16 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_BIT_S16_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_U8_BIT_A8(dm)                              \
	sd = *sp++;                                             \
	sdh = vis_fpmerge(vis_fzeros(), vis_read_hi(sd));       \
	sdl = vis_fpmerge(vis_fzeros(), vis_read_lo(sd));       \
	dmh = vis_fcmpgt16(sdh, vis_fzero());                   \
	dml = vis_fcmpgt16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

/* *********************************************************** */

#define	LOAD_CONVERT_U8_BIT(shl, shr, dm)                       \
	LOAD_CONVERT_U8_BIT_A8(d1);                             \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

#define	LOAD_CONVERT_U8_BIT_A8_NF(dm)                           \
	sd = vis_ld_d64_nf(sp); sp++;                           \
	sdh = vis_fpmerge(vis_fzeros(), vis_read_hi(sd));       \
	sdl = vis_fpmerge(vis_fzeros(), vis_read_lo(sd));       \
	dmh = vis_fcmpgt16(sdh, vis_fzero());                   \
	dml = vis_fcmpgt16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

/* *********************************************************** */

#define	LOAD_CONVERT_U8_BIT_NF(shl, shr, dm)                    \
	LOAD_CONVERT_U8_BIT_A8_NF(d1);                          \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* 8-byte aligned pointer to source */
	mlib_d64 *sp;

/* pointer to destination */
	mlib_u8 *dp;
	mlib_d64 sd, sdh, sdl;
	mlib_s32 dmh, dml, dm;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_U8_BIT_A8(dm);
		(*dp++) = dm;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_U8_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp, *sl;
	mlib_d64 sd, sdh, sdl;
	mlib_s32 dmh, dml, dm;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_U8_BIT_A8(dm);
			(*dp++) = dm;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U8_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source line */
	mlib_u8 *sl;

/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp;
	mlib_d64 sd, sdh, sdl;
	mlib_s32 dmh, dml, d0, d1, dm, shl, shr;
	mlib_s32 i, j;

	sl = (void *)src;
	dl = dst;

	for (j = 0; j < ysize; j++) {

/* prepare addresses */
		sp = (mlib_d64 *)((mlib_addr)sl & (~7));
		dp = dl;

		shl = sl - (mlib_u8 *)sp;
		shr = 8 - shl;

/* initial value */
		LOAD_CONVERT_U8_BIT_A8(d1);
		d0 = d1 << shl;

#pragma pipeloop(0)
		for (i = 0; i < (xsize / 8) - 1; i++) {
			LOAD_CONVERT_U8_BIT(shl, shr, dm);
			(*dp++) = dm;
		}

		if (i < xsize / 8) {
			LOAD_CONVERT_U8_BIT_NF(shl, shr, dm);
			(*dp++) = dm;
		}

		if (xsize & 7) {
			mlib_s32 mask = 0xFF00 >> (xsize & 7);

			LOAD_CONVERT_U8_BIT_NF(shl, shr, dm);
			dp[0] = (dm & mask) | (dp[0] & ~mask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_U16_BIT_A8(dm)                             \
	sdh = *sp++;                                            \
	sdl = *sp++;                                            \
	dmh = vis_fcmpne16(sdh, vis_fzero());                   \
	dml = vis_fcmpne16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

/* *********************************************************** */

#define	LOAD_CONVERT_U16_BIT(shl, shr, dm)                      \
	LOAD_CONVERT_U16_BIT_A8(d1);                            \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

#define	LOAD_CONVERT_U16_BIT_A8_NF(dm)                          \
	sdh = vis_ld_d64_nf(sp);                                \
	sdl = vis_ld_d64_nf(sp + 1);                            \
	sp += 2;                                                \
	dmh = vis_fcmpne16(sdh, vis_fzero());                   \
	dml = vis_fcmpne16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

/* *********************************************************** */

#define	LOAD_CONVERT_U16_BIT_NF(shl, shr, dm)                   \
	LOAD_CONVERT_U16_BIT_A8_NF(d1);                         \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* 8-byte aligned pointer to source */
	mlib_d64 *sp;

/* pointer to destination */
	mlib_u8 *dp;
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml, dm;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_U16_BIT_A8(dm);
		(*dp++) = dm;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_U16_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp, *sl;
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml, dm;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_U16_BIT_A8(dm);
			(*dp++) = dm;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U16_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source line */
	mlib_u8 *sl;

/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp;
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml, d0, d1, dm, shl, shr;
	mlib_s32 i, j;

	sl = (void *)src;
	dl = dst;

	for (j = 0; j < ysize; j++) {

/* prepare addresses */
		sp = (mlib_d64 *)((mlib_addr)sl & (~7));
		dp = dl;

		shl = (mlib_u16 *)sl - (mlib_u16 *)sp;
		shr = 8 - shl;

/* initial value */
		LOAD_CONVERT_U16_BIT_A8_NF(d1);
		d0 = d1 << shl;

#pragma pipeloop(0)
		for (i = 0; i < (xsize / 8) - 1; i++) {
			LOAD_CONVERT_U16_BIT(shl, shr, dm);
			(*dp++) = dm;
		}

		if (i < xsize / 8) {
			LOAD_CONVERT_U16_BIT_NF(shl, shr, dm);
			(*dp++) = dm;
		}

		if (xsize & 7) {
			mlib_s32 mask = 0xFF00 >> (xsize & 7);

			LOAD_CONVERT_U16_BIT_NF(shl, shr, dm);
			dp[0] = (dm & mask) | (dp[0] & ~mask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_S16_BIT_A8(dm)                             \
	sdh = *sp++;                                            \
	sdl = *sp++;                                            \
	dmh = vis_fcmpgt16(sdh, vis_fzero());                   \
	dml = vis_fcmpgt16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

/* *********************************************************** */

#define	LOAD_CONVERT_S16_BIT(shl, shr, dm)                      \
	LOAD_CONVERT_S16_BIT_A8(d1);                            \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

#define	LOAD_CONVERT_S16_BIT_A8_NF(dm)                          \
	sdh = vis_ld_d64_nf(sp);                                \
	sdl = vis_ld_d64_nf(sp + 1);                            \
	sp += 2;                                                \
	dmh = vis_fcmpgt16(sdh, vis_fzero());                   \
	dml = vis_fcmpgt16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

/* *********************************************************** */

#define	LOAD_CONVERT_S16_BIT_NF(shl, shr, dm)                   \
	LOAD_CONVERT_S16_BIT_A8_NF(d1);                         \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* 8-byte aligned pointer to source */
	mlib_d64 *sp;

/* pointer to destination */
	mlib_u8 *dp;
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml, dm;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_S16_BIT_A8(dm);
		(*dp++) = dm;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_S16_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp, *sl;
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml, dm;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_S16_BIT_A8(dm);
			(*dp++) = dm;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S16_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source line */
	mlib_u8 *sl;

/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp;
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml, d0, d1, dm, shl, shr;
	mlib_s32 i, j;

	sl = (void *)src;
	dl = dst;

	for (j = 0; j < ysize; j++) {

/* prepare addresses */
		sp = (mlib_d64 *)((mlib_addr)sl & (~7));
		dp = dl;

		shl = (mlib_s16 *)sl - (mlib_s16 *)sp;
		shr = 8 - shl;

/* initial value */
		LOAD_CONVERT_S16_BIT_A8_NF(d1);
		d0 = d1 << shl;

#pragma pipeloop(0)
		for (i = 0; i < (xsize / 8) - 1; i++) {
			LOAD_CONVERT_S16_BIT(shl, shr, dm);
			(*dp++) = dm;
		}

		if (i < xsize / 8) {
			LOAD_CONVERT_S16_BIT_NF(shl, shr, dm);
			(*dp++) = dm;
		}

		if (xsize & 7) {
			mlib_s32 mask = 0xFF00 >> (xsize & 7);

			LOAD_CONVERT_S16_BIT_NF(shl, shr, dm);
			dp[0] = (dm & mask) | (dp[0] & ~mask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_S32_BIT_A8(dm)                             \
	sd0 = sp[0];                                            \
	sd1 = sp[1];                                            \
	sd2 = sp[2];                                            \
	sd3 = sp[3];                                            \
	dm0 = vis_fcmpgt32(sd0, vis_fzero());                   \
	dm1 = vis_fcmpgt32(sd1, vis_fzero());                   \
	dm2 = vis_fcmpgt32(sd2, vis_fzero());                   \
	dm3 = vis_fcmpgt32(sd3, vis_fzero());                   \
	dm = ((dm0 << 6) | (dm1 << 4) | (dm2 << 2) | dm3);      \
	sp += 4

/* *********************************************************** */

#define	LOAD_CONVERT_S32_BIT(shl, shr, dm)                      \
	LOAD_CONVERT_S32_BIT_A8(d1);                            \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

#define	LOAD_CONVERT_S32_BIT_A8_NF(dm)                          \
	sd0 = vis_ld_d64_nf(sp);                                \
	sd1 = vis_ld_d64_nf(sp + 1);                            \
	sd2 = vis_ld_d64_nf(sp + 2);                            \
	sd3 = vis_ld_d64_nf(sp + 3);                            \
	dm0 = vis_fcmpgt32(sd0, vis_fzero());                   \
	dm1 = vis_fcmpgt32(sd1, vis_fzero());                   \
	dm2 = vis_fcmpgt32(sd2, vis_fzero());                   \
	dm3 = vis_fcmpgt32(sd3, vis_fzero());                   \
	dm = ((dm0 << 6) | (dm1 << 4) | (dm2 << 2) | dm3);      \
	sp += 4

/* *********************************************************** */

#define	LOAD_CONVERT_S32_BIT_NF(shl, shr, dm)                   \
	LOAD_CONVERT_S32_BIT_A8_NF(d1);                         \
	dm = d0 | (d1 >> shr);                                  \
	d0 = (d1 << shl)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S32_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* 8-byte aligned pointer to source */
	mlib_d64 *sp;

/* pointer to destination */
	mlib_u8 *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 dm0, dm1, dm2, dm3, dm;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_S32_BIT_A8(dm);
		(*dp++) = dm;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_S32_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp, *sl;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 dm0, dm1, dm2, dm3, dm;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_S32_BIT_A8(dm);
			(*dp++) = dm;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S32_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source line */
	mlib_u8 *sl;

/* pointer to destination */
	mlib_u8 *dl, *dp;

/* 8-byte aligned pointer to source */
	mlib_d64 *sp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 dm0, dm1, dm2, dm3, dm, d0, d1, shl, shr;
	mlib_s32 i, j;

	sl = (void *)src;
	dl = dst;

	for (j = 0; j < ysize; j++) {

/* prepare addresses */
		sp = (mlib_d64 *)((mlib_addr)sl & (~7));
		dp = dl;

		shl = (mlib_s32 *)sl - (mlib_s32 *)sp;
		shr = 8 - shl;

/* initial value */
		LOAD_CONVERT_S32_BIT_A8_NF(d1);
		d0 = d1 << shl;

#pragma pipeloop(0)
		for (i = 0; i < (xsize / 8) - 1; i++) {
			LOAD_CONVERT_S32_BIT(shl, shr, dm);
			(*dp++) = dm;
		}

		if (i < xsize / 8) {
			LOAD_CONVERT_S32_BIT_NF(shl, shr, dm);
			(*dp++) = dm;
		}

		if (xsize & 7) {
			mlib_s32 mask = 0xFF00 >> (xsize & 7);

			LOAD_CONVERT_S32_BIT_NF(shl, shr, dm);
			dp[0] = (dm & mask) | (dp[0] & ~mask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U8_S16_A8                            \
	sd = *sp++;                                             \
	(*dp++) = vis_fpmerge(zeros, vis_read_hi(sd));          \
	(*dp++) = vis_fpmerge(zeros, vis_read_lo(sd))

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_S16_A8D1X8(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 sd;
	mlib_f32 zeros = vis_fzeros();
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_U8_S16_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_S16_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd;
	mlib_f32 zeros = vis_fzeros();
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_U8_S16_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U8_S16_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* end point in destination */
	mlib_s16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 s0, s1, sd;

/* 8-byte destination data */
	mlib_d64 dd0, dd1;
	mlib_f32 zeros = vis_fzeros();

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)dst;
/* in SHORTs */
	dend = dst + dsize - 1;

/* prepare the source address and GSR.offset */
/* destination offset has half effect on source, off is 0 or even */
	sp = (mlib_d64 *)vis_alignaddr((void *)src, off / 2);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

/* load 8 bytes source data */
	s0 = vis_ld_d64_nf(sp);
	s1 = vis_ld_d64_nf(sp + 1);
	sp += 2;
	sd = vis_faligndata(s0, s1);
/* convert the 1st 4 bytes into 4 shorts, and store */
	dd0 = vis_fpmerge(zeros, vis_read_hi(sd));
	vis_pst_16(dd0, dp++, emask);

/* convert the 2nd 4 bytes into 4 shorts */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		dd1 = vis_fpmerge(zeros, vis_read_lo(sd));
		vis_pst_16(dd1, dp++, emask);
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 16;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp);
		sp++;
		sd = vis_faligndata(s0, s1);
		(*dp++) = vis_fpmerge(zeros, vis_read_hi(sd));
		(*dp++) = vis_fpmerge(zeros, vis_read_lo(sd));
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s1;
		s1 = vis_ld_d64_nf(sp);
		sd = vis_faligndata(s0, s1);

		dd0 = vis_fpmerge(zeros, vis_read_hi(sd));
		vis_pst_16(dd0, dp++, emask);

/* avoid the last wrong write */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			dd1 = vis_fpmerge(zeros, vis_read_lo(sd));
			vis_pst_16(dd1, dp, emask);
		}
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U8_S16(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sl;
	mlib_s16 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
/* in pixel */
	dlb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U8_S16_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U8_S32_A8                            \
	sd = *sp++;                                             \
	d1 = vis_fmul8x16al(vis_read_hi(sd), fone1);            \
	d0 = vis_fpmerge(fzero, vis_read_hi(d1));               \
	d1 = vis_fpmerge(fzero, vis_read_lo(d1));               \
	d3 = vis_fpmerge(vis_read_lo(sd), vis_read_lo(sd));     \
	d2 = vis_fmuld8ulx16(vis_read_hi(d3), fone2);           \
	d3 = vis_fmuld8ulx16(vis_read_lo(d3), fone2);           \
	(*dp++) = d0;                                           \
	(*dp++) = d1;                                           \
	(*dp++) = d2;                                           \
	(*dp++) = d3

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_S32_A8D1X8(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 sd;
	mlib_f32 fzero = vis_fzero(),
	    fone1 = vis_to_float(0x100), fone2 = vis_to_float(0x10001);
	mlib_d64 d0, d1, d2, d3;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_U8_S32_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_S32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd;
	mlib_d64 d0, d1, d2, d3;
	mlib_f32 fzero = vis_fzero(),
	    fone1 = vis_to_float(0x100), fone2 = vis_to_float(0x10001);
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_U8_S32_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U8_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
	mlib_u8 *psrc = (mlib_u8 *)src;
	mlib_s32 *pdst = (mlib_s32 *)dst;
	mlib_f32 fzero = vis_fzero(),
	    fone1 = vis_to_float(0x100), fone2 = vis_to_float(0x10001);
	mlib_d64 *dpsrc, dsrc0, dsrc1, dsrc, dst0, dst1, dst2, dst3;
	mlib_s32 i = 0;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
	dsrc = vis_ld_d64_nf(dpsrc);

	if ((mlib_addr)psrc & 7) {
		dsrc1 = vis_ld_d64_nf(dpsrc + 1);
		dsrc = vis_faligndata(dsrc, dsrc1);

#pragma pipeloop(0)
		for (; i <= (dsize - 8); i += 8) {
			dst1 = vis_fmul8x16al(vis_read_hi(dsrc), fone1);
			dst0 = vis_fpmerge(fzero, vis_read_hi(dst1));
			dst1 = vis_fpmerge(fzero, vis_read_lo(dst1));
			dst3 =
			    vis_fpmerge(vis_read_lo(dsrc), vis_read_lo(dsrc));
			dst2 = vis_fmuld8ulx16(vis_read_hi(dst3), fone2);
			dst3 = vis_fmuld8ulx16(vis_read_lo(dst3), fone2);

			dsrc0 = dpsrc[1];
			dsrc1 = vis_ld_d64_nf(dpsrc + 2);
			dsrc = vis_faligndata(dsrc0, dsrc1);

			((mlib_d64 *)pdst)[0] = dst0;
			((mlib_d64 *)pdst)[1] = dst1;
			((mlib_d64 *)pdst)[2] = dst2;
			((mlib_d64 *)pdst)[3] = dst3;
			pdst += 8;
			psrc += 8;
			dpsrc++;
		}
	} else {

#pragma pipeloop(0)
		for (; i <= (dsize - 8); i += 8) {
			dst1 = vis_fmul8x16al(vis_read_hi(dsrc), fone1);
			dst0 = vis_fpmerge(fzero, vis_read_hi(dst1));
			dst1 = vis_fpmerge(fzero, vis_read_lo(dst1));
			dst3 =
			    vis_fpmerge(vis_read_lo(dsrc), vis_read_lo(dsrc));
			dst2 = vis_fmuld8ulx16(vis_read_hi(dst3), fone2);
			dst3 = vis_fmuld8ulx16(vis_read_lo(dst3), fone2);

			dsrc = vis_ld_d64_nf(dpsrc + 1);
			((mlib_d64 *)pdst)[0] = dst0;
			((mlib_d64 *)pdst)[1] = dst1;
			((mlib_d64 *)pdst)[2] = dst2;
			((mlib_d64 *)pdst)[3] = dst3;
			pdst += 8;
			psrc += 8;
			dpsrc++;
		}
	}

	for (; i < dsize; i++)
		(*pdst++) = (*psrc++);
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U8_S32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sl;
	mlib_s32 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
/* in pixel */
	dlb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U8_S32_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_U8_A8                            \
	sd0 = *sp++;                                            \
	sd1 = *sp++;                                            \
	mask0 = vis_fcmplt16(sd0, zero);                        \
	mask1 = vis_fcmplt16(sd1, zero);                        \
	mask1 |= mask0 << 4;                                    \
	*dp = vis_fpack16_pair(sd0, sd1);                       \
	vis_pst_8(dlog_max8, dp, mask1);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_U16_U8_NF                                  \
	s1 = vis_ld_d64_nf(sp);                                 \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	mask0 = vis_fcmplt16(sd0, zero);                        \
	mask1 = vis_fcmplt16(sd1, zero);                        \
	mask1 |= mask0 << 4;                                    \
	dd = vis_fpack16_pair(sd0, sd1)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_U8_A8D1X8(
    const mlib_u16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 i;
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max8 = vis_to_double_dup(0xffffffff);
	mlib_s32 mask0, mask1;

/* set GSR.scale_factor */
	vis_write_gsr(7 << 3);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_U16_U8_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_U16_U8_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 sd0, sd1;

/* indices for x, y */
	mlib_s32 i, j;
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max8 = vis_to_double_dup(0xffffffff);
	mlib_s32 mask0, mask1;

/* set GSR.scale_factor */
	vis_write_gsr(7 << 3);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_U16_U8_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U16_U8_D1(
    const mlib_u16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* start point in source */
	mlib_u16 *sa;

/* start point in destination */
	mlib_u8 *da;

/* end point in destination */
	mlib_u8 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 s0, s1, s2;
	mlib_d64 sd0, sd1;

/* 8-byte destination data */
	mlib_d64 dd;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max8 = vis_to_double_dup(0xffffffff);
	mlib_s32 mask0, mask1;

/* set GSR.scale_factor */
	vis_write_gsr(7 << 3);

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address and GSR.offset */
/* destination offset has mlib_d64 effect on source */
	sp = (mlib_d64 *)vis_alignaddr(sa, off * 2);

/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

/* convert the 1st 16 bytes into 8 bytes */
	s0 = vis_ld_d64_nf(sp);
	sp++;
	LOAD_CONVERT_U16_U8_NF;
	vis_pst_8(dd, dp, emask);
	vis_pst_8(dlog_max8, dp, mask1 & emask);
	dp++;

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s2;
		LOAD_CONVERT_U16_U8_NF;
		*dp = dd;
		vis_pst_8(dlog_max8, dp, mask1);
		dp++;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		s0 = s2;
		LOAD_CONVERT_U16_U8_NF;
		vis_pst_8(dd, dp, emask);
		vis_pst_8(dlog_max8, dp, mask1 & emask);
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U16_U8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;
/* in pixel */
	slb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U16_U8_D1(sa, da, xsize);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S32_A8                                    \
	d0 = *sp++;                                                      \
	d1 = *sp++;                                                      \
	(*dp++) =                                                        \
	vis_fand(vis_fmuld8ulx16(two_16_ones, vis_read_hi(d0)), mask);   \
	(*dp++) =                                                        \
	vis_fand(vis_fmuld8ulx16(two_16_ones, vis_read_lo(d0)), mask);   \
	(*dp++) =                                                        \
	vis_fand(vis_fmuld8ulx16(two_16_ones, vis_read_hi(d1)), mask);   \
	(*dp++) =                                                        \
	vis_fand(vis_fmuld8ulx16(two_16_ones, vis_read_lo(d1)), mask)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_S32_A8D1X8(
    const mlib_u16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 d0, d1;
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 mask = vis_to_double_dup(0xffff);
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_U16_S32_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_S32_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 d0, d1;
	mlib_s32 i, j;
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 mask = vis_to_double_dup(0xffff);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_U16_S32_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U16_S32_D1(
    const mlib_u16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
	mlib_u16 *sp = (void *)src;
	mlib_s32 *dp = dst;
	mlib_d64 *ddsp, *ddp;
	mlib_s32 len_64, even_length, rest_64, length = dsize;
	mlib_d64 dd1, dd2, dd3, dd4;
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_s32 i;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	while ((mlib_addr)dp & 7) {
		(*dp++) = *sp++;
		length--;
	}

	ddsp = (mlib_d64 *)vis_alignaddr(sp, 0);
	ddp = (mlib_d64 *)dp;
	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	dd2 = vis_ld_d64_nf(ddsp);

	if (!((mlib_addr)(sp) & 7)) {

/*
 * Source vector is 8-bytes aligned. We can process it without
 * vis_faligndata.
 */

/* Peeling of 1 iteration.  */

		if (i = (len_64 & 1)) {
			dd3 = dd2;
			ddsp++;
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(dd3)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(dd3)), mask);
		}
#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			dd3 = (*ddsp++);
			dd4 = (*ddsp++);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(dd3)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(dd3)), mask);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(dd4)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(dd4)), mask);
		}
	} else {

/* Source vector is not 8-bytes aligned. Use vis_faligndata. */

/* Peeling of 1 iteration.  */

		i = 1;

		if ((len_64 & 1)) {
			i++;
			dd1 = dd2;
			dd2 = ddsp[1];
			dd3 = vis_faligndata(dd1, dd2);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(dd3)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(dd3)), mask);
		}

/* Now loop with step == 2.  */

#pragma pipeloop(0)
		for (; i <= len_64; i += 2) {
			dd1 = dd2;
			dd2 = ddsp[i];
			dd3 = vis_faligndata(dd1, dd2);
			dd1 = dd2;
			dd2 = ddsp[i + 1];
			dd4 = vis_faligndata(dd1, dd2);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(dd3)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(dd3)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(dd4)), mask);
			(*ddp++) =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(dd4)), mask);
		}
	}

	for (i = 0; i < rest_64; i++)
		dp[even_length + i] = sp[even_length + i];
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U16_S32(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sl;
	mlib_s32 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U16_S32_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16_A8                             \
	sd0 = sp[0];                                              \
	sd1 = sp[1];                                              \
	sp += 2;                                                  \
	mask0 = vis_fcmpgt32(sd0, d_Max_U16);                     \
	mask1 = vis_fcmpgt32(sd1, d_Max_U16);                     \
	mask2 = vis_fcmplt32(sd0, d_Min_U16);                     \
	mask3 = vis_fcmplt32(sd1, d_Min_U16);                     \
	mask1 |= mask0 << 2;                                      \
	mask3 |= mask2 << 2;                                      \
	sd0 = vis_fpsub32(sd0, shift1);                           \
	sd1 = vis_fpsub32(sd1, shift1);                           \
	sd0 = vis_fpadd32(sd0, sd0);                              \
	sd1 = vis_fpadd32(sd1, sd1);                              \
	*dp = vis_fpadd16(vis_fpackfix_pair(sd0, sd1), shift2);   \
	vis_pst_16(dlog_max, dp, mask1);                          \
	vis_pst_16(dlog_min, dp, mask3);                          \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16                                \
	s1 = sp[0];                                               \
	s2 = vis_ld_d64_nf(sp + 1);                               \
	sp += 2;                                                  \
	sd0 = vis_faligndata(s0, s1);                             \
	sd1 = vis_faligndata(s1, s2);                             \
	mask0 = vis_fcmpgt32(sd0, d_Max_U16);                     \
	mask1 = vis_fcmpgt32(sd1, d_Max_U16);                     \
	mask2 = vis_fcmplt32(sd0, d_Min_U16);                     \
	mask3 = vis_fcmplt32(sd1, d_Min_U16);                     \
	mask1 |= mask0 << 2;                                      \
	mask3 |= mask2 << 2;                                      \
	sd0 = vis_fpsub32(sd0, shift1);                           \
	sd1 = vis_fpsub32(sd1, shift1);                           \
	sd0 = vis_fpadd32(sd0, sd0);                              \
	sd1 = vis_fpadd32(sd1, sd1);                              \
	*dp = vis_fpadd16(vis_fpackfix_pair(sd0, sd1), shift2);   \
	vis_pst_16(dlog_max, dp, mask1);                          \
	vis_pst_16(dlog_min, dp, mask3);                          \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16_EMASK                         \
	s1 = vis_ld_d64_nf(sp);                                  \
	s2 = vis_ld_d64_nf(sp + 1);                              \
	sp += 2;                                                 \
	sd0 = vis_faligndata(s0, s1);                            \
	sd1 = vis_faligndata(s1, s2);                            \
	mask0 = vis_fcmpgt32(sd0, d_Max_U16);                    \
	mask1 = vis_fcmpgt32(sd1, d_Max_U16);                    \
	mask2 = vis_fcmplt32(sd0, d_Min_U16);                    \
	mask3 = vis_fcmplt32(sd1, d_Min_U16);                    \
	mask1 |= mask0 << 2;                                     \
	mask3 |= mask2 << 2;                                     \
	sd0 = vis_fpsub32(sd0, shift1);                          \
	sd1 = vis_fpsub32(sd1, shift1);                          \
	sd0 = vis_fpadd32(sd0, sd0);                             \
	sd1 = vis_fpadd32(sd1, sd1);                             \
	dd = vis_fpadd16(vis_fpackfix_pair(sd0, sd1), shift2);   \
	vis_pst_16(dd, dp, emask);                               \
	vis_pst_16(dlog_max, dp, mask1 & emask);                 \
	vis_pst_16(dlog_min, dp, mask3 & emask);                 \
	dp++

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S32_U16_A8D1X8(
    const mlib_s32 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 d_Min_U16 = vis_to_double_dup(MLIB_U16_MIN),
	    d_Max_U16 = vis_to_double_dup(MLIB_U16_MAX);
	mlib_d64 dlog_max = vis_to_double_dup(0xffffffff),
	    dlog_min = vis_fzero();
	mlib_d64 shift1 = vis_to_double_dup(0x8000);
	mlib_d64 shift2 = vis_to_double_dup(0x80008000);
	mlib_s32 mask0, mask1, mask2, mask3;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(15 << 3);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_S32_U16_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_S32_U16_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 sd0, sd1;
	mlib_d64 d_Min_U16 = vis_to_double_dup(MLIB_U16_MIN),
	    d_Max_U16 = vis_to_double_dup(MLIB_U16_MAX);
	mlib_d64 dlog_max = vis_to_double_dup(0xffffffff),
	    dlog_min = vis_fzero();
	mlib_d64 shift1 = vis_to_double_dup(0x8000);
	mlib_d64 shift2 = vis_to_double_dup(0x80008000);
	mlib_s32 mask0, mask1, mask2, mask3;

/* indices for x, y */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(15 << 3);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			LOAD_CONVERT_STORE_S32_U16_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S32_U16_D1(
    const mlib_s32 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer for source */
	mlib_s32 *sa;

/* pointer for destination */
	mlib_u16 *da;

/* end point in destination */
	mlib_u16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp, dd;

/* source data */
	mlib_d64 s0, s1, s2, sd0, sd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 d_Min_U16 = vis_to_double_dup(MLIB_U16_MIN);
	mlib_d64 d_Max_U16 = vis_to_double_dup(MLIB_U16_MAX);
	mlib_d64 dlog_max = vis_to_double_dup(0xffffffff),
	    dlog_min = vis_fzero();
	mlib_d64 shift1 = vis_to_double_dup(0x8000);
	mlib_d64 shift2 = vis_to_double_dup(0x80008000);
	mlib_s32 mask0, mask1, mask2, mask3;

	sa = (void *)src;
	da = dst;

	vis_write_gsr(15 << 3);

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)vis_alignaddr(sa, 2 * off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	s0 = vis_ld_d64_nf(sp); sp++;
	LOAD_CONVERT_STORE_S32_U16_EMASK;

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s2;
		LOAD_CONVERT_STORE_S32_U16_EMASK;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s2;
		LOAD_CONVERT_STORE_S32_U16;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s2;
		LOAD_CONVERT_STORE_S32_U16_EMASK;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S32_U16(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sl;
	mlib_u16 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;

/* in pixel */
	dlb >>= 1;
/* in pixel */
	slb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S32_U16_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U8_A8                            \
	sd0 = *sp++;                                            \
	sd1 = *sp++;                                            \
	(*dp++) = vis_fpack16_pair(sd0, sd1)

/* *********************************************************** */

#define	LOAD_CONVERT_S16_U8                                     \
	s1 = *sp++;                                             \
	s2 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	dd = vis_fpack16_pair(sd0, sd1)

/* *********************************************************** */

#define	LOAD_CONVERT_S16_U8_NF                                  \
	s1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	s2 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	dd = vis_fpack16_pair(sd0, sd1)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_U8_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 i;

/* set GSR.scale_factor */
	vis_write_gsr(7 << 3);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_S16_U8_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_S16_U8_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 sd0, sd1;

/* indices for x, y */
	mlib_s32 i, j;

/* set GSR.scale_factor */
	vis_write_gsr(7 << 3);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_S16_U8_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S16_U8_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* start point in source */
	mlib_s16 *sa;

/* start point in destination */
	mlib_u8 *da;

/* end point in destination */
	mlib_u8 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 s0, s1, s2;
	mlib_d64 sd0, sd1;

/* 8-byte destination data */
	mlib_d64 dd;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

/* set GSR.scale_factor */
	vis_write_gsr(7 << 3);

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address and GSR.offset */
/* destination offset has mlib_d64 effect on source */
	sp = (mlib_d64 *)vis_alignaddr(sa, off * 2);

/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

/* convert the 1st 16 bytes into 8 bytes */
	s0 = vis_ld_d64_nf(sp);
	sp++;
	LOAD_CONVERT_S16_U8_NF;
	vis_pst_8(dd, dp++, emask);

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s2;
		LOAD_CONVERT_S16_U8;
		(*dp++) = dd;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		s0 = s2;
		LOAD_CONVERT_S16_U8_NF;
		vis_pst_8(dd, dp, emask);
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S16_U8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;
/* in pixel */
	slb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S16_U8_D1(sa, da, xsize);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_S32_A8                              \
	d0 = *sp++;                                                \
	d1 = *sp++;                                                \
	(*dp++) = vis_fmuld8ulx16(two_16_ones, vis_read_hi(d0));   \
	(*dp++) = vis_fmuld8ulx16(two_16_ones, vis_read_lo(d0));   \
	(*dp++) = vis_fmuld8ulx16(two_16_ones, vis_read_hi(d1));   \
	(*dp++) = vis_fmuld8ulx16(two_16_ones, vis_read_lo(d1))

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_S32_A8D1X8(
    const mlib_s16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 d0, d1;
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_S16_S32_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_S32_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 d0, d1;
	mlib_s32 i, j;
	mlib_f32 two_16_ones = vis_to_float(0x10001);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_S16_S32_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S16_S32_D1(
    const mlib_s16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
	mlib_s16 *sp = (void *)src;
	mlib_s32 *dp = dst;
	mlib_d64 *ddsp, *ddp;
	mlib_s32 len_64, even_length, rest_64, length = dsize;
	mlib_d64 dd1, dd2, dd3, dd4;
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_s32 i;

	while ((mlib_addr)dp & 7) {
		(*dp++) = *sp++;
		length--;
	}

	ddsp = (mlib_d64 *)vis_alignaddr(sp, 0);
	ddp = (mlib_d64 *)dp;
	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	dd2 = vis_ld_d64_nf(ddsp);

	if (!((mlib_addr)(sp) & 7)) {

/*
 * Source vector is 8-bytes aligned. We can process it without
 * vis_faligndata.
 */

/* Peeling of 1 iteration.  */

		if (i = (len_64 & 1)) {
			dd3 = dd2;
			ddsp++;
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
		}
#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			dd3 = (*ddsp++);
			dd4 = (*ddsp++);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd4));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd4));
		}
	} else {

/* Source vector is not 8-bytes aligned. Use vis_faligndata. */

/* Peeling of 1 iteration.  */

		i = 1;

		if ((len_64 & 1)) {
			i++;
			dd1 = dd2;
			dd2 = ddsp[1];
			dd3 = vis_faligndata(dd1, dd2);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
		}

/* Now loop with step == 2.  */

#pragma pipeloop(0)
		for (; i <= len_64; i += 2) {
			dd1 = dd2;
			dd2 = ddsp[i];
			dd3 = vis_faligndata(dd1, dd2);
			dd1 = dd2;
			dd2 = ddsp[i + 1];
			dd4 = vis_faligndata(dd1, dd2);
/* Now obtaining of the 4*32-signed objects   */
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd4));
			(*ddp++) =
			    vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd4));
		}
	}

	for (i = 0; i < rest_64; i++)
		dp[even_length + i] = sp[even_length + i];
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S16_S32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sl;
	mlib_s32 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S16_S32_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_S16_A8                           \
	sd0 = sp[0];                                            \
	sd1 = sp[1];                                            \
	sp += 2;                                                \
	mask0 = vis_fcmpgt32(sd0, d_Max_S16);                   \
	mask1 = vis_fcmpgt32(sd1, d_Max_S16);                   \
	mask2 = vis_fcmplt32(sd0, d_Min_S16);                   \
	mask3 = vis_fcmplt32(sd1, d_Min_S16);                   \
	mask1 |= mask0 << 2;                                    \
	mask3 |= mask2 << 2;                                    \
	sd0 = vis_fpadd32(sd0, sd0);                            \
	sd1 = vis_fpadd32(sd1, sd1);                            \
	*dp = vis_fpackfix_pair(sd0, sd1);                      \
	vis_pst_16(dlog_max, dp, mask1);                        \
	vis_pst_16(dlog_min, dp, mask3);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_S16                              \
	s1 = sp[0];                                             \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	mask0 = vis_fcmpgt32(sd0, d_Max_S16);                   \
	mask1 = vis_fcmpgt32(sd1, d_Max_S16);                   \
	mask2 = vis_fcmplt32(sd0, d_Min_S16);                   \
	mask3 = vis_fcmplt32(sd1, d_Min_S16);                   \
	mask1 |= mask0 << 2;                                    \
	mask3 |= mask2 << 2;                                    \
	sd0 = vis_fpadd32(sd0, sd0);                            \
	sd1 = vis_fpadd32(sd1, sd1);                            \
	*dp = vis_fpackfix_pair(sd0, sd1);                      \
	vis_pst_16(dlog_max, dp, mask1);                        \
	vis_pst_16(dlog_min, dp, mask3);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_S16_EMASK                        \
	s1 = vis_ld_d64_nf(sp);                                 \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	mask0 = vis_fcmpgt32(sd0, d_Max_S16);                   \
	mask1 = vis_fcmpgt32(sd1, d_Max_S16);                   \
	mask2 = vis_fcmplt32(sd0, d_Min_S16);                   \
	mask3 = vis_fcmplt32(sd1, d_Min_S16);                   \
	mask1 |= mask0 << 2;                                    \
	mask3 |= mask2 << 2;                                    \
	sd0 = vis_fpadd32(sd0, sd0);                            \
	sd1 = vis_fpadd32(sd1, sd1);                            \
	dd = vis_fpackfix_pair(sd0, sd1);                       \
	vis_pst_16(dd, dp, emask);                              \
	vis_pst_16(dlog_max, dp, mask1 & emask);                \
	vis_pst_16(dlog_min, dp, mask3 & emask);                \
	dp++

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S32_S16_A8D1X8(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 d_Min_S16 = vis_to_double_dup(MLIB_S16_MIN),
	    d_Max_S16 = vis_to_double_dup(MLIB_S16_MAX);
	mlib_d64 dlog_min = vis_to_double_dup(0x80008000),
	    dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0, mask1, mask2, mask3;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(15 << 3);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_S32_S16_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_S32_S16_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 sd0, sd1;
	mlib_d64 d_Min_S16 = vis_to_double_dup(MLIB_S16_MIN),
	    d_Max_S16 = vis_to_double_dup(MLIB_S16_MAX);
	mlib_d64 dlog_min = vis_to_double_dup(0x80008000),
	    dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0, mask1, mask2, mask3;

/* indices for x, y */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(15 << 3);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			LOAD_CONVERT_STORE_S32_S16_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S32_S16_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for source */
	mlib_s32 *sa;

/* pointer for destination */
	mlib_s16 *da;

/* end point in destination */
	mlib_s16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp, dd;

/* source data */
	mlib_d64 s0, s1, s2, sd0, sd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 d_Min_S16 = vis_to_double_dup(MLIB_S16_MIN);
	mlib_d64 d_Max_S16 = vis_to_double_dup(MLIB_S16_MAX);
	mlib_d64 dlog_min = vis_to_double_dup(0x80008000);
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0, mask1, mask2, mask3;

	sa = (void *)src;
	da = dst;

	vis_write_gsr(15 << 3);

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)vis_alignaddr(sa, 2 * off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	s0 = vis_ld_d64_nf(sp); sp++;
	LOAD_CONVERT_STORE_S32_S16_EMASK;

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16_EMASK;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16_EMASK;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S32_S16(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sl;
	mlib_s16 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;

/* in pixel */
	dlb >>= 1;
/* in pixel */
	slb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S32_S16_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_CONVERT_F32_U8_1                                            \
	sd0.value = *sp++;                                               \
	FLOAT2INT_CLAMP(sd0.twoint.int0, sd0.twofloat.float0 * 256.f);   \
	FLOAT2INT_CLAMP(sd0.twoint.int1, sd0.twofloat.float1 * 256.f);   \
	(*dbuf++) = sd0.value

/* *********************************************************** */

#define	MLIB_CONVERT_F32_U8_2(dd)                               \
	dd = vis_fpack32(dbuf[0], dbuf[0]);                     \
	dd = vis_fpack32(dd, dbuf[1]);                          \
	dd = vis_fpack32(dd, dbuf[2]);                          \
	dd = vis_fpack32(dd, dbuf[3]);                          \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd));     \
	dbuf += 4

/* *********************************************************** */

#define	MLIB_CONVERT_F32_U8_1_NF                                         \
	sd0.value = vis_ld_d64_nf(sp);                                   \
	sp++;                                                            \
	FLOAT2INT_CLAMP(sd0.twoint.int0, sd0.twofloat.float0 * 256.f);   \
	FLOAT2INT_CLAMP(sd0.twoint.int1, sd0.twofloat.float1 * 256.f);   \
	(*dbuf++) = sd0.value

/* *********************************************************** */

#define	MLIB_CONVERT_F32_U8_NF(dd)                              \
	dbuf = buf;                                             \
	MLIB_CONVERT_F32_U8_1_NF;                               \
	MLIB_CONVERT_F32_U8_1_NF;                               \
	MLIB_CONVERT_F32_U8_1_NF;                               \
	MLIB_CONVERT_F32_U8_1_NF;                               \
	dbuf = buf;                                             \
	MLIB_CONVERT_F32_U8_2(dd)

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_F32_U8_A8D2X8(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 buf[SIZE / 2], *dbuf;

/* indices for x, y */
	mlib_s32 i, j, num, size;

/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 dd;
	type_union_mlib_d64 sd0;

	vis_write_gsr(15 << 3);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

		for (size = 0; size < xsize; size += num) {
			num = xsize - size;

			if (num > SIZE)
				num = SIZE;

			dbuf = buf;

#pragma pipeloop(0)
			for (i = 0; i < num / 2; i++) {
				MLIB_CONVERT_F32_U8_1;
			}

			dbuf = buf;

#pragma pipeloop(0)
			for (i = 0; i < num / 8; i++) {
				MLIB_CONVERT_F32_U8_2(dd);
				(*dp++) = dd;
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_F32_U8_D1(
    const mlib_f32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_d64 buf[SIZE / 2], *dbuf;

/* start point in source */
	mlib_f32 *sa;

/* start point in destination */
	mlib_u8 *da;

/* end point in destination */
	mlib_u8 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_d64 dd0, dd1, dd;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, size, num;
	type_union_mlib_d64 sd0;

	sa = (void *)src;
	da = dst;

/* prepare the source address */
	off = ((mlib_addr)sa & 7) / 4;
	sp = (mlib_d64 *)(sa - off);

/* prepare the destination address and GSR.offset */
	dp = (mlib_d64 *)((mlib_addr)(da - off) & (~7));
	off = (mlib_addr)dp - (mlib_addr)(da - off);
	off &= 7;
	vis_write_gsr((15 << 3) | off);

	dend = da + dsize - 1;

/* process start point */
	MLIB_CONVERT_F32_U8_NF(dd0);

	if ((mlib_u8 *)(dp + 1) > da) {
		emask = vis_edge8(da, dend);
		dd = vis_faligndata(dd0, dd0);
		vis_pst_8(dd, dp, emask);
	}

	dp++;

	if (off == 0) {
		MLIB_CONVERT_F32_U8_NF(dd0);
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1) / 8;

	for (size = 0; size < n; size += num) {
		num = n - size;

		if (8 * num > SIZE)
			num = SIZE / 8;

		dbuf = buf;

#pragma pipeloop(0)
		for (i = 0; i < 4 * num - 4; i++) {
			MLIB_CONVERT_F32_U8_1;
		}

		for (; i < 4 * num; i++) {
			MLIB_CONVERT_F32_U8_1_NF;
		}

		dbuf = buf;

#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			MLIB_CONVERT_F32_U8_2(dd1);
			(*dp++) = vis_faligndata(dd0, dd1);
			dd0 = dd1;
		}
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		MLIB_CONVERT_F32_U8_NF(dd1);
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dp, emask);
		dp++;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_F32_U8(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;
/* in pixel */
	slb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_F32_U8_D1(sa, da, xsize);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_D64_S16                                 \
	sd0 = sp[0];                                               \
	sd1 = sp[1];                                               \
	sd2 = sp[2];                                               \
	sd3 = sp[3];                                               \
	sp += 4;                                                   \
	d0.twoint.int0 = (mlib_s32)(sd0 * scale);                  \
	d1.twoint.int0 = (mlib_s32)(sd1 * scale);                  \
	d0.twoint.int1 = (mlib_s32)(sd2 * scale);                  \
	d1.twoint.int1 = (mlib_s32)(sd3 * scale);                  \
	vis_pst_16(d0.value, dp, 0xA);                             \
	vis_pst_16(vis_faligndata(d1.value, d1.value), dp, 0x5);   \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_D64_S16_EDGE                                    \
	sd0 = vis_ld_d64_nf(sp);                                           \
	sd1 = vis_ld_d64_nf(sp + 1);                                       \
	sd2 = vis_ld_d64_nf(sp + 2);                                       \
	sd3 = vis_ld_d64_nf(sp + 3);                                       \
	sp += 4;                                                           \
	d0.twoint.int0 = (mlib_s32)(sd0 * scale);                          \
	d1.twoint.int0 = (mlib_s32)(sd1 * scale);                          \
	d0.twoint.int1 = (mlib_s32)(sd2 * scale);                          \
	d1.twoint.int1 = (mlib_s32)(sd3 * scale);                          \
	vis_pst_16(d0.value, dp, 0xA & emask);                             \
	vis_pst_16(vis_faligndata(d1.value, d1.value), dp, 0x5 & emask);   \
	dp++

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 4.
 */

void
mlib_v_ImageDataTypeConvert_D64_S16_A8D1X4(
    const mlib_d64 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_s32 i;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3, scale = 65536.0;
	type_union_mlib_d64 d0, d1;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr((15 << 3) | 6);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_D64_S16;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_D64_S16_A8D2X4(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 i, j;
	mlib_d64 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_d64 sd0, sd1, sd2, sd3, scale = 65536.0;
	type_union_mlib_d64 d0, d1;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr((15 << 3) | 6);

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			LOAD_CONVERT_STORE_D64_S16;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_D64_S16_D1(
    const mlib_d64 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sa;
	mlib_s16 *da;
	mlib_d64 *sp, *dp;
	mlib_s32 i;
	mlib_d64 sd0, sd1, sd2, sd3, scale = 65536.0;
	type_union_mlib_d64 d0, d1;

	sa = (void *)src;
	da = dst;

	vis_write_gsr((15 << 3) | 6);

	while (((mlib_s32)da & 7) && dsize) {
		(*da++) = (mlib_s32)((*sa++) * scale) >> 16;
		dsize--;
	}

	sp = (mlib_d64 *)sa;
	dp = (mlib_d64 *)da;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_D64_S16;
	}

	if (dsize & 3) {
		mlib_s32 emask = 0xF0 >> (dsize & 3);

		LOAD_CONVERT_STORE_D64_S16_EDGE;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_D64_S16(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sa, *sl;
	mlib_s16 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

/* in pixel */
	slb >>= 3;
/* in pixel */
	dlb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_D64_S16_D1(sa, da, xsize);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_D64_U16                                 \
	sd0 = sp[0];                                               \
	sd1 = sp[1];                                               \
	sd2 = sp[2];                                               \
	sd3 = sp[3];                                               \
	sp += 4;                                                   \
	d0.twoint.int0 = (mlib_s32)((sd0 - soff) * scale);         \
	d1.twoint.int0 = (mlib_s32)((sd1 - soff) * scale);         \
	d0.twoint.int1 = (mlib_s32)((sd2 - soff) * scale);         \
	d1.twoint.int1 = (mlib_s32)((sd3 - soff) * scale);         \
	d0.value = vis_fxor(d0.value, mask8000);                   \
	d1.value = vis_fxor(d1.value, mask8000);                   \
	vis_pst_16(d0.value, dp, 0xA);                             \
	vis_pst_16(vis_faligndata(d1.value, d1.value), dp, 0x5);   \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_D64_U16_EDGE                                    \
	sd0 = vis_ld_d64_nf(sp);                                           \
	sd1 = vis_ld_d64_nf(sp + 1);                                       \
	sd2 = vis_ld_d64_nf(sp + 2);                                       \
	sd3 = vis_ld_d64_nf(sp + 3);                                       \
	sp += 4;                                                           \
	d0.twoint.int0 = (mlib_s32)((sd0 - soff) * scale);                 \
	d1.twoint.int0 = (mlib_s32)((sd1 - soff) * scale);                 \
	d0.twoint.int1 = (mlib_s32)((sd2 - soff) * scale);                 \
	d1.twoint.int1 = (mlib_s32)((sd3 - soff) * scale);                 \
	d0.value = vis_fxor(d0.value, mask8000);                           \
	d1.value = vis_fxor(d1.value, mask8000);                           \
	vis_pst_16(d0.value, dp, 0xA & emask);                             \
	vis_pst_16(vis_faligndata(d1.value, d1.value), dp, 0x5 & emask);   \
	dp++

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 4.
 */

void
mlib_v_ImageDataTypeConvert_D64_U16_A8D1X4(
    const mlib_d64 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
	mlib_s32 i;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3, scale = 65536.0, soff = 1 << 15;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 d0, d1;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr((15 << 3) | 6);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_D64_U16;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_D64_U16_A8D2X4(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 i, j;
	mlib_d64 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_d64 sd0, sd1, sd2, sd3, scale = 65536.0, soff = 1 << 15;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 d0, d1;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr((15 << 3) | 6);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			LOAD_CONVERT_STORE_D64_U16;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_D64_U16_D1(
    const mlib_d64 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sa;
	mlib_u16 *da;
	mlib_d64 *sp, *dp;
	mlib_s32 i;
	mlib_d64 sd0, sd1, sd2, sd3, scale = 65536.0, soff = 1 << 15;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 d0, d1;

	sa = (void *)src;
	da = dst;

	vis_write_gsr((15 << 3) | 6);

	while (((mlib_s32)da & 7) && dsize) {
		(*da++) = ((mlib_s32)(((*sa++) - soff) * scale) >> 16) ^ 0x8000;
		dsize--;
	}

	sp = (mlib_d64 *)sa;
	dp = (mlib_d64 *)da;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_D64_U16;
	}

	if (dsize & 3) {
		mlib_s32 emask = 0xF0 >> (dsize & 3);

		LOAD_CONVERT_STORE_D64_U16_EDGE;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_D64_U16(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sa, *sl;
	mlib_u16 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

/* in pixel */
	slb >>= 3;
/* in pixel */
	dlb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_D64_U16_D1(sa, da, xsize);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDataTypeConvert_BIT_S32_A8D1X8(
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

#pragma pipeloop(0)
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
mlib_v_ImageDataTypeConvert_BIT_S32_A8D2X8(
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

#pragma pipeloop(0)
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
mlib_v_ImageDataTypeConvert_BIT_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_s32 *da;

/* end point in destination */
	mlib_s32 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_u8 sd0, sd1;
	mlib_d64 dd0;
	mlib_u8 dd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 *pt;

	sa = (void *)src;
	da = dst;
	pt = (mlib_d64 *)mlib_table;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = ((mlib_u8 *)da - (mlib_u8 *)dp) >> 2;
	dend = da + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge32(da, dend);

	sd1 = *sa++;
	dd0 = pt[(sd1 >> (6 + off))];
	vis_pst_32(dd0, dp++, emask);

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		sd0 = sd1;
		sd1 = *sa++;
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		(*dp++) = pt[(dd1 >> 6)];
		(*dp++) = pt[(dd1 >> 4) & 3];
		(*dp++) = pt[(dd1 >> 2) & 3];
		(*dp++) = pt[(dd1 & 3)];
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		emask = vis_edge32(dp, dend);
		sd0 = sd1;
		sd1 = vis_ld_u8_nf(sa);
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		dd0 = pt[(dd1 >> 6)];
		vis_pst_32(dd0, dp++, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge32(dp, dend);
			dd0 = pt[(dd1 >> 4) & 3];
			vis_pst_32(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				dd0 = pt[(dd1 >> 2) & 3];
				vis_pst_32(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					dd0 = pt[(dd1 & 3)];
					vis_pst_32(dd0, dp++, emask);
				}
			}
		}
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_BIT_S32(
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
		mlib_v_ImageDataTypeConvert_BIT_S32_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDataTypeConvert_BIT_F32_A8D1X8(
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

#pragma pipeloop(0)
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
mlib_v_ImageDataTypeConvert_BIT_F32_A8D2X8(
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

#pragma pipeloop(0)
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
mlib_v_ImageDataTypeConvert_BIT_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_f32 *da;

/* end point in destination */
	mlib_f32 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_u8 sd0, sd1;
	mlib_d64 dd0;
	mlib_u8 dd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 *pt;

	sa = (void *)src;
	da = dst;
	pt = (mlib_d64 *)mlib_table_F32;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = ((mlib_u8 *)da - (mlib_u8 *)dp) >> 2;
	dend = da + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge32(da, dend);

	sd1 = *sa++;
	dd0 = pt[(sd1 >> (6 + off))];
	vis_pst_32(dd0, dp++, emask);

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		sd0 = sd1;
		sd1 = *sa++;
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		(*dp++) = pt[(dd1 >> 6)];
		(*dp++) = pt[(dd1 >> 4) & 3];
		(*dp++) = pt[(dd1 >> 2) & 3];
		(*dp++) = pt[(dd1 & 3)];
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {

		emask = vis_edge32(dp, dend);
		sd0 = sd1;
		sd1 = vis_ld_u8_nf(sa);
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		dd0 = pt[(dd1 >> 6)];
		vis_pst_32(dd0, dp++, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge32(dp, dend);
			dd0 = pt[(dd1 >> 4) & 3];
			vis_pst_32(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				dd0 = pt[(dd1 >> 2) & 3];
				vis_pst_32(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					dd0 = pt[(dd1 & 3)];
					vis_pst_32(dd0, dp++, emask);
				}
			}
		}
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_BIT_F32(
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
		mlib_v_ImageDataTypeConvert_BIT_F32_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}
}

/* *********************************************************** */

#define	ITOF(reg)	((mlib_f32)*(mlib_s32 *) & reg)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_F32_A8D1X8(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
	mlib_d64 sd;
	mlib_f32 fzero = vis_fzeros(),
	    fone1 = vis_to_float(0x100), fone2 = vis_to_float(0x10001);
	mlib_d64 d1, d2;
	mlib_s32 i, j, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	dsize >>= 3;
	num = dsize;

	for (size = 0; size < dsize; size += num) {

		num = dsize - size;
		num = (num > SIZE) ? SIZE : num;

		pb = buf;

#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			sd = sp[0];
			d1 = vis_fmul8x16al(vis_read_hi(sd), fone1);
			d2 = vis_fpmerge(vis_read_lo(sd), vis_read_lo(sd));
			pb[0] = vis_fpmerge(fzero, vis_read_hi(d1));
			pb[1] = vis_fpmerge(fzero, vis_read_lo(d1));
			pb[2] = vis_fmuld8ulx16(vis_read_hi(d2), fone2);
			pb[3] = vis_fmuld8ulx16(vis_read_lo(d2), fone2);
			sp += 1;
			pb += 4;
		}

#pragma pipeloop(0)
		for (j = 0; j < 4 * num; j++) {
			mlib_d64 dst0;
			mlib_f32 s0, s1;

			dst0 = buf[j];
			s0 = vis_read_hi(dst0);
			s1 = vis_read_lo(dst0);
			(*dp++) = vis_freg_pair(ITOF(s0), ITOF(s1));
		}
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U8_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd;
	mlib_d64 d1, d2;
	mlib_f32 fzero = vis_fzeros(),
	    fone1 = vis_to_float(0x100), fone2 = vis_to_float(0x10001);
	mlib_s32 i, j, k, size, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {

		for (size = 0; size < xsize; size += num) {

			num = xsize - size;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (i = 0; i < num; i++) {
				sd = sp[0];
				d1 = vis_fmul8x16al(vis_read_hi(sd), fone1);
				d2 = vis_fpmerge(vis_read_lo(sd),
				    vis_read_lo(sd));
				pb[0] = vis_fpmerge(fzero, vis_read_hi(d1));
				pb[1] = vis_fpmerge(fzero, vis_read_lo(d1));
				pb[2] = vis_fmuld8ulx16(vis_read_hi(d2), fone2);
				pb[3] = vis_fmuld8ulx16(vis_read_lo(d2), fone2);
				sp += 1;
				pb += 4;
			}

#pragma pipeloop(0)
			for (k = 0; k < 4 * num; k++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[k];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = vis_freg_pair(ITOF(s0), ITOF(s1));
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U8_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
	mlib_d64 d1, d2;
	mlib_f32 fzero = vis_fzeros(),
	    fone1 = vis_to_float(0x100), fone2 = vis_to_float(0x10001);
	mlib_s32 j, k, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;
	mlib_u8 *psrc = (void *)src;
	mlib_f32 *pdst = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1, dsrc;
	mlib_s32 i = 0;

	num = dsize;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);

	if ((mlib_addr)psrc & 7) {

		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;
			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dsrc0 = dpsrc[0];
				dsrc1 = dpsrc[1];
				dsrc = vis_faligndata(dsrc0, dsrc1);
				d1 = vis_fmul8x16al(vis_read_hi(dsrc), fone1);
				d2 = vis_fpmerge(vis_read_lo(dsrc),
				    vis_read_lo(dsrc));
				pb[0] = vis_fpmerge(fzero, vis_read_hi(d1));
				pb[1] = vis_fpmerge(fzero, vis_read_lo(d1));
				pb[2] = vis_fmuld8ulx16(vis_read_hi(d2), fone2);
				pb[3] = vis_fmuld8ulx16(vis_read_lo(d2), fone2);
				psrc += 8;
				dpsrc++;
				pb += 4;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				*(mlib_d64 *)pdst =
				    vis_freg_pair(ITOF(s0), ITOF(s1));
				pdst += 2;
			}
		}
	} else {

		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;
			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dsrc = dpsrc[0];
				d1 = vis_fmul8x16al(vis_read_hi(dsrc), fone1);
				d2 = vis_fpmerge(vis_read_lo(dsrc),
				    vis_read_lo(dsrc));
				pb[0] = vis_fpmerge(fzero, vis_read_hi(d1));
				pb[1] = vis_fpmerge(fzero, vis_read_lo(d1));
				pb[2] = vis_fmuld8ulx16(vis_read_hi(d2), fone2);
				pb[3] = vis_fmuld8ulx16(vis_read_lo(d2), fone2);
				psrc += 8;
				dpsrc++;
				pb += 4;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				*(mlib_d64 *)pdst =
				    vis_freg_pair(ITOF(s0), ITOF(s1));
				pdst += 2;
			}
		}
	}

	for (; i < dsize; i++)
		(*pdst++) = (*psrc++);
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U8_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sl;
	mlib_f32 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
/* in pixel */
	dlb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U8_F32_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_F32_A8D1X8(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	dsize >>= 3;
	num = dsize;
	for (size = 0; size < dsize; size += num) {

		num = dsize - size;
		num = (num > SIZE) ? SIZE : num;

		pb = buf;
		sd0 = sp[0];
		sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			pb[0] = vis_fmuld8ulx16(two_16_ones, vis_read_hi(sd0));
			pb[1] = vis_fmuld8ulx16(two_16_ones, vis_read_lo(sd0));
			pb[2] = vis_fmuld8ulx16(two_16_ones, vis_read_hi(sd1));
			pb[3] = vis_fmuld8ulx16(two_16_ones, vis_read_lo(sd1));
			sp += 2;
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			pb += 4;
		}

#pragma pipeloop(0)
		for (j = 0; j < 4 * num; j++) {
			mlib_d64 dst0;
			mlib_f32 s0, s1;

			dst0 = buf[j];
			s0 = vis_read_hi(dst0);
			s1 = vis_read_lo(dst0);
			(*dp++) = vis_freg_pair(ITOF(s0), ITOF(s1));
		}
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_F32_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, k, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;
	mlib_d64 *sl, *dl;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {

		for (size = 0; size < xsize; size += num) {
			num = xsize - size;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;
			sd0 = sp[0];
			sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
			for (i = 0; i < num; i++) {
				pb[0] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd0));
				pb[1] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd0));
				pb[2] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd1));
				pb[3] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd1));
				sp += 2;
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				pb += 4;
			}

#pragma pipeloop(0)
			for (k = 0; k < 4 * num; k++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[k];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = vis_freg_pair(ITOF(s0), ITOF(s1));
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S16_F32_D1(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 d1, d2;
	mlib_s32 j, k, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;
	mlib_s16 *psrc = (void *)src;
	mlib_f32 *pdst = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1;
	mlib_s32 i = 0;

	num = dsize;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);

	if ((mlib_addr)psrc & 7) {
		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;
			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dsrc0 = dpsrc[0];
				dsrc1 = dpsrc[1];
				d1 = vis_faligndata(dsrc0, dsrc1);
				dsrc0 = dsrc1;
				dsrc1 = dpsrc[2];
				d2 = vis_faligndata(dsrc0, dsrc1);
				pb[0] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1));
				pb[1] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1));
				pb[2] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2));
				pb[3] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2));
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				*(mlib_d64 *)pdst =
				    vis_freg_pair(ITOF(s0), ITOF(s1));
				pdst += 2;
			}
		}
	} else {

		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;
			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				d1 = dpsrc[0];
				d2 = dpsrc[1];
				pb[0] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1));
				pb[1] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1));
				pb[2] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2));
				pb[3] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2));
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				*(mlib_d64 *)pdst =
				    vis_freg_pair(ITOF(s0), ITOF(s1));
				pdst += 2;
			}
		}
	}

	for (; i < dsize; i++)
		(*pdst++) = (*psrc++);
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S16_F32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sl;
	mlib_f32 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S16_F32_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16_A8                           \
	d0 = *sp++;                                             \
	*dp = d0;                                               \
	mask0 = vis_fcmplt16(d0, dlog_min);                     \
	vis_pst_16(dlog_min, dp, mask0);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16_EMASK                        \
	d1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(d0, d1);                           \
	vis_pst_16(sd0, dp, emask);                             \
	mask0 = vis_fcmplt16(sd0, dlog_min);                    \
	vis_pst_16(dlog_min, dp, mask0 & emask);                \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16                              \
	d1 = *sp++;                                             \
	sd0 = vis_faligndata(d0, d1);                           \
	*dp = sd0;                                              \
	mask0 = vis_fcmplt16(sd0, dlog_min);                    \
	vis_pst_16(dlog_min, dp, mask0);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16_NF                           \
	d1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(d0, d1);                           \
	*dp = sd0;                                              \
	mask0 = vis_fcmplt16(sd0, dlog_min);                    \
	vis_pst_16(dlog_min, dp, mask0);                        \
	dp++

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_U16_A8D1X8(
    const mlib_s16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 d0;
	mlib_s32 i;
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		LOAD_CONVERT_STORE_S16_U16_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_U16_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;
	mlib_d64 d0;
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			LOAD_CONVERT_STORE_S16_U16_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S16_U16_D1(
    const mlib_s16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer for source */
	mlib_s16 *sa;

/* pointer for destination */
	mlib_u16 *da;

/* end point in destination */
	mlib_u16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 d0, d1;

/* 8-byte destination data */
	mlib_d64 sd0;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask0;

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	d0 = vis_ld_d64_nf(sp); sp++;

	LOAD_CONVERT_STORE_S16_U16_EMASK;

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		d0 = d1;
		LOAD_CONVERT_STORE_S16_U16_EMASK;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n - 1; i++) {
		d0 = d1;
		LOAD_CONVERT_STORE_S16_U16;
	}

	if (i < n) {
		d0 = d1;
		LOAD_CONVERT_STORE_S16_U16_NF;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		d0 = d1;
		LOAD_CONVERT_STORE_S16_U16_EMASK;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S16_U16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sl;
	mlib_u16 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S16_U16_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	ITOD(reg)	((mlib_d64)*(mlib_s32 *) & reg)

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_D64_A8D1X8(
    const mlib_s16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;

	sp = (mlib_d64 *)src;
	dp = dst;

	dsize >>= 3;
	num = dsize;
	for (size = 0; size < dsize; size += num) {

		num = dsize - size;
		num = (num > SIZE) ? SIZE : num;

		pb = buf;
		sd0 = sp[0];
		sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			pb[0] = vis_fmuld8ulx16(two_16_ones, vis_read_hi(sd0));
			pb[1] = vis_fmuld8ulx16(two_16_ones, vis_read_lo(sd0));
			pb[2] = vis_fmuld8ulx16(two_16_ones, vis_read_hi(sd1));
			pb[3] = vis_fmuld8ulx16(two_16_ones, vis_read_lo(sd1));
			sp += 2;
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			pb += 4;
		}

#pragma pipeloop(0)
		for (j = 0; j < 4 * num; j++) {
			mlib_d64 dst0;
			mlib_f32 s0, s1;

			dst0 = buf[j];
			s0 = vis_read_hi(dst0);
			s1 = vis_read_lo(dst0);
			(*dp++) = ITOD(s0);
			(*dp++) = ITOD(s1);
		}
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S16_D64_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, k, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;
	mlib_d64 *sl, *dl;

	sp = sl = (mlib_d64 *)src;
	dp = dl = dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {
		for (size = 0; size < xsize; size += num) {
			num = xsize - size;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;
			sd0 = sp[0];
			sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
			for (i = 0; i < num; i++) {
				pb[0] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd0));
				pb[1] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd0));
				pb[2] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd1));
				pb[3] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd1));
				sp += 2;
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				pb += 4;
			}

#pragma pipeloop(0)
			for (k = 0; k < 4 * num; k++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[k];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = ITOD(s0);
				(*dp++) = ITOD(s1);
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S16_D64_D1(
    const mlib_s16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 d1, d2;
	mlib_s32 j, k, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;
	mlib_s16 *psrc = (void *)src;
	mlib_d64 *dp = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1;
	mlib_s32 i = 0;

	num = dsize;

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);

	if ((mlib_addr)psrc & 7) {
		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dsrc0 = dpsrc[0];
				dsrc1 = dpsrc[1];
				d1 = vis_faligndata(dsrc0, dsrc1);
				dsrc0 = dsrc1;
				dsrc1 = dpsrc[2];
				d2 = vis_faligndata(dsrc0, dsrc1);
				pb[0] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1));
				pb[1] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1));
				pb[2] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2));
				pb[3] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2));
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = ITOD(s0);
				(*dp++) = ITOD(s1);
			}
		}
	} else {

		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				d1 = dpsrc[0];
				d2 = dpsrc[1];
				pb[0] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1));
				pb[1] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1));
				pb[2] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2));
				pb[3] =
				    vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2));
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = ITOD(s0);
				(*dp++) = ITOD(s1);
			}
		}
	}

	for (; i < dsize; i++)
		(*dp++) = (*psrc++);
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sl;
	mlib_d64 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 3;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S16_D64_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_F32_A8D1X8(
    const mlib_u16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	dsize >>= 3;
	num = dsize;
	for (size = 0; size < dsize; size += num) {

		num = dsize - size;
		num = (num > SIZE) ? SIZE : num;

		pb = buf;
		sd0 = sp[0];
		sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			pb[0] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(sd0)), mask);
			pb[1] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(sd0)), mask);
			pb[2] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(sd1)), mask);
			pb[3] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(sd1)), mask);
			sp += 2;
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			pb += 4;
		}

#pragma pipeloop(0)
		for (j = 0; j < 4 * num; j++) {
			mlib_d64 dst0;
			mlib_f32 s0, s1;

			dst0 = buf[j];
			s0 = vis_read_hi(dst0);
			s1 = vis_read_lo(dst0);
			(*dp++) = vis_freg_pair(ITOF(s0), ITOF(s1));
		}
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_F32_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, k, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;
	mlib_d64 *sl, *dl;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {

		for (size = 0; size < xsize; size += num) {
			num = xsize - size;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;
			sd0 = sp[0];
			sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
			for (i = 0; i < num; i++) {
				pb[0] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd0)), mask);
				pb[1] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd0)), mask);
				pb[2] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd1)), mask);
				pb[3] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd1)), mask);
				sp += 2;
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				pb += 4;
			}

#pragma pipeloop(0)
			for (k = 0; k < 4 * num; k++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[k];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = vis_freg_pair(ITOF(s0), ITOF(s1));
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U16_F32_D1(
    const mlib_u16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 d1, d2;
	mlib_s32 j, k, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;
	mlib_u16 *psrc = (void *)src;
	mlib_f32 *pdst = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1;
	mlib_s32 i = 0;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	num = dsize;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);

	if ((mlib_addr)psrc & 7) {
		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dsrc0 = dpsrc[0];
				dsrc1 = dpsrc[1];
				d1 = vis_faligndata(dsrc0, dsrc1);
				dsrc0 = dsrc1;
				dsrc1 = dpsrc[2];
				d2 = vis_faligndata(dsrc0, dsrc1);
				pb[0] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1)), mask);
				pb[1] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1)), mask);
				pb[2] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2)), mask);
				pb[3] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2)), mask);
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				*(mlib_d64 *)pdst =
				    vis_freg_pair(ITOF(s0), ITOF(s1));
				pdst += 2;
			}
		}
	} else {

		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				d1 = dpsrc[0];
				d2 = dpsrc[1];
				pb[0] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1)), mask);
				pb[1] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1)), mask);
				pb[2] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2)), mask);
				pb[3] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2)), mask);
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				*(mlib_d64 *)pdst =
				    vis_freg_pair(ITOF(s0), ITOF(s1));
				pdst += 2;
			}
		}
	}

	for (; i < dsize; i++)
		(*pdst++) = (*psrc++);
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U16_F32(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sl;
	mlib_f32 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U16_F32_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_D64_A8D1X8(
    const mlib_u16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	sp = (mlib_d64 *)src;
	dp = dst;

	dsize >>= 3;
	num = dsize;

	for (size = 0; size < dsize; size += num) {

		num = dsize - size;
		num = (num > SIZE) ? SIZE : num;

		pb = buf;
		sd0 = sp[0];
		sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
		for (i = 0; i < num; i++) {
			pb[0] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(sd0)), mask);
			pb[1] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(sd0)), mask);
			pb[2] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_hi(sd1)), mask);
			pb[3] =
			    vis_fand(vis_fmuld8ulx16(two_16_ones,
			    vis_read_lo(sd1)), mask);
			sp += 2;
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			pb += 4;
		}

#pragma pipeloop(0)
		for (j = 0; j < 4 * num; j++) {
			mlib_d64 dst0;
			mlib_f32 s0, s1;

			dst0 = buf[j];
			s0 = vis_read_hi(dst0);
			s1 = vis_read_lo(dst0);
			(*dp++) = ITOD(s0);
			(*dp++) = ITOD(s1);
		}
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_D64_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, k, size, num;
	mlib_d64 buf[4 * SIZE];

/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *pb;
	mlib_d64 *sl, *dl;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	sp = sl = (mlib_d64 *)src;
	dp = dl = dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {

		for (size = 0; size < xsize; size += num) {
			num = xsize - size;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;
			sd0 = sp[0];
			sd1 = vis_ld_d64_nf(sp + 1);

#pragma pipeloop(0)
			for (i = 0; i < num; i++) {
				pb[0] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd0)), mask);
				pb[1] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd0)), mask);
				pb[2] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(sd1)), mask);
				pb[3] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(sd1)), mask);
				sp += 2;
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				pb += 4;
			}

#pragma pipeloop(0)
			for (k = 0; k < 4 * num; k++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[k];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = ITOD(s0);
				(*dp++) = ITOD(s1);
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U16_D64_D1(
    const mlib_u16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 d1, d2;
	mlib_s32 j, k, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;
	mlib_u16 *psrc = (void *)src;
	mlib_d64 *dp = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1;
	mlib_s32 i = 0;
	mlib_d64 mask = vis_to_double_dup(0xffff);

	num = dsize;

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);

	if ((mlib_addr)psrc & 7) {
		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dsrc0 = dpsrc[0];
				dsrc1 = dpsrc[1];
				d1 = vis_faligndata(dsrc0, dsrc1);
				dsrc0 = dsrc1;
				dsrc1 = dpsrc[2];
				d2 = vis_faligndata(dsrc0, dsrc1);
				pb[0] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1)), mask);
				pb[1] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1)), mask);
				pb[2] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2)), mask);
				pb[3] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2)), mask);
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = ITOD(s0);
				(*dp++) = ITOD(s1);
			}
		}
	} else {

		for (; i <= (dsize - 8); i += 8 * num) {
			num = (dsize - i) / 8;
			num = (num > SIZE) ? SIZE : num;

			pb = buf;

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				d1 = dpsrc[0];
				d2 = dpsrc[1];
				pb[0] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d1)), mask);
				pb[1] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d1)), mask);
				pb[2] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_hi(d2)), mask);
				pb[3] =
				    vis_fand(vis_fmuld8ulx16(two_16_ones,
				    vis_read_lo(d2)), mask);
				dpsrc += 2;
				pb += 4;
				psrc += 8;
			}

#pragma pipeloop(0)
			for (j = 0; j < 4 * num; j++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[j];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				(*dp++) = ITOD(s0);
				(*dp++) = ITOD(s1);
			}
		}
	}

	for (; i < dsize; i++)
		(*dp++) = (*psrc++);
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sl;
	mlib_d64 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 3;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U16_D64_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16_A8                           \
	d0 = *sp++;                                             \
	*dp = d0;                                               \
	mask0 = vis_fcmplt16(d0, zero);                         \
	vis_pst_16(dlog_max, dp, mask0);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16_EMASK                        \
	d1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(d0, d1);                           \
	vis_pst_16(sd0, dp, emask);                             \
	mask0 = vis_fcmplt16(sd0, zero);                        \
	vis_pst_16(dlog_max, dp, mask0 & emask);                \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16                              \
	d1 = *sp++;                                             \
	sd0 = vis_faligndata(d0, d1);                           \
	*dp = sd0;                                              \
	mask0 = vis_fcmplt16(sd0, zero);                        \
	vis_pst_16(dlog_max, dp, mask0);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16_NF                           \
	d1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(d0, d1);                           \
	*dp = sd0;                                              \
	mask0 = vis_fcmplt16(sd0, zero);                        \
	vis_pst_16(dlog_max, dp, mask0);                        \
	dp++

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_S16_A8D1X8(
    const mlib_u16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 d0;
	mlib_s32 i;
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_U16_S16_A8;
		LOAD_CONVERT_STORE_U16_S16_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * slb and dlb are multiple of 8.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_U16_S16_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;
	mlib_d64 d0;
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_d64 zero = vis_fzero();
	mlib_s32 mask0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			LOAD_CONVERT_STORE_U16_S16_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_U16_S16_D1(
    const mlib_u16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for source */
	mlib_u16 *sa;

/* pointer for destination */
	mlib_s16 *da;

/* end point in destination */
	mlib_s16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 d0, d1;

/* 8-byte destination data */
	mlib_d64 sd0;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_d64 zero = vis_fzero();
	mlib_s32 mask0;

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	d0 = vis_ld_d64_nf(sp); sp++;

	LOAD_CONVERT_STORE_U16_S16_EMASK;

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		d0 = d1;
		LOAD_CONVERT_STORE_U16_S16_EMASK;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		d0 = d1;
		LOAD_CONVERT_STORE_U16_S16_NF;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		d0 = d1;
		LOAD_CONVERT_STORE_U16_S16_EMASK;
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_U16_S16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sl;
	mlib_s16 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;
	slb >>= 1;
/* in pixel */
	dlb >>= 1;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_U16_S16_D1(sl, dl, xsize);
		sl += slb;
		dl += dlb;
	}
}

/*   From C version   */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0, f1;
	} f32s;
	struct
	{
		mlib_s32 f0, f1;
	} i32s;
} d64_2x32;

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
 * Convert a 1-bit image into a d64-bit image.
 */

void
mlib_v_ImageDataTypeConvert_BIT_D64(
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
	mlib_s32 i, j, k, num, size;

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

#pragma pipeloop(0)
			for (i = 0; i < (num - 1) / 8 + 1; i++) {
				bitdata = *sp++;
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

#pragma pipeloop(0)
			for (k = 0; k < num; k++) {
				dp[k] = (mlib_d64)buf[k];
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an 8-bit image into a d64-bit image.
 */

void
mlib_v_ImageDataTypeConvert_U8_D64(
    const mlib_image *src,
    mlib_image *dst)
{
	mlib_d64 *lut[4] = { (void *)mlib_U82D64, (void *)mlib_U82D64,
		(void *)mlib_U82D64, (void *)mlib_U82D64
	};

	__mlib_ImageLookUp(dst, src, (const void **)lut);
}

/* *********************************************************** */

/*
 * Convert a 32-bit image into an 8-bit image.
 */

void
mlib_v_ImageDataTypeConvert_S32_U8(
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

#pragma pipeloop(0)
		for (i = 0; i < width * channels; i++) {
			sd = *sp++;
			MLIB_CLAMP_U8(*dp, sd);
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
mlib_v_ImageDataTypeConvert_S32_F32(
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
	mlib_d64 *sp2, *dp2;
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
			(*dp++) = (mlib_f32)*sp++;
		}

		dp2 = (mlib_d64 *)dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*sp2++);
				dd.f32s.f0 = (mlib_f32)dd.i32s.f0;
				dd.f32s.f1 = (mlib_f32)dd.i32s.f1;
				dp2[0] = dd.d64;
			}

			sp = (mlib_s32 *)sp2;
		} else {

#pragma pipeloop(0)
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.f32s.f0 = (mlib_f32)*sp++;
				dd.f32s.f1 = (mlib_f32)*sp++;
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
mlib_v_ImageDataTypeConvert_S32_D64(
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

#pragma pipeloop(0)
		for (i = 0; i < width * channels; i++) {
			(*dp++) = (mlib_d64)((*sp++));
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 16-bit image.
 */

void
mlib_v_ImageDataTypeConvert_F32_U16(
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
				    ((mlib_s32)(((*sp++) -
				    32767.5f) * 65536.0f) >> 16) ^ 0x8000;
				n--;
			}

			dp2 = (mlib_s32 *)dp;

#pragma pipeloop(0)
			for (i = 0; i < n / 2; i++) {
				dp2[i] =
				    (mlib_s32)((sp[2 * i] -
				    32767.5f) * 65536.0f) ^ 0x80000000;
			}

#pragma pipeloop(0)
			for (i = 0; i < n / 2; i++) {
				buf[i] =
				    (mlib_s32)((sp[2 * i + 1] -
				    32767.5f) * 65536.0f);
			}

			dp1 = dp + 1;

#pragma pipeloop(0)
			for (i = 0; i < n / 2; i++)
				dp1[2 * i] = (buf[i] >> 16) ^ 0x8000;

			if (n & 1) {
				dp[n - 1] =
				    ((mlib_s32)((sp[n - 1] -
				    32767.5f) * 65536.0f) >> 16) ^ 0x8000;
			}
		}

#else /* MLIB_USE_FTOI_CLAMPING */
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT_U16((*dp++), *sp)
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
mlib_v_ImageDataTypeConvert_F32_S16(
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

#pragma pipeloop(0)
			for (i = 0; i < n / 2; i++) {
				dp2[i] = (mlib_s32)(sp[2 * i] * 65536.0f);
			}

#pragma pipeloop(0)
			for (i = 0; i < n / 2; i++) {
				buf[i] = (mlib_s32)(sp[2 * i + 1] * 65536.0f);
			}

			dp1 = dp + 1;

#pragma pipeloop(0)
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
			SAT16((*dp++), *sp)
			    sp++;
		}

#endif /* MLIB_USE_FTOI_CLAMPING */

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 32-bit image.
 */

void
mlib_v_ImageDataTypeConvert_F32_S32(
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
	mlib_d64 *sp2, *dp2;
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
			FLOAT2INT_CLAMP((*dp++), *sp);
			sp++;
		}

		dp2 = (mlib_d64 *)dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (; (mlib_s32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*sp2++);
				FLOAT2INT_CLAMP(dd.i32s.f0, dd.f32s.f0);
				FLOAT2INT_CLAMP(dd.i32s.f1, dd.f32s.f1);
				dp2[0] = dd.d64;
			}

			sp = (mlib_f32 *)sp2;
		} else {

#pragma pipeloop(0)
			for (; (mlib_s32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				FLOAT2INT_CLAMP(dd.i32s.f0, *sp);
				sp++;
				FLOAT2INT_CLAMP(dd.i32s.f1, *sp);
				sp++;
				dp2[0] = dd.d64;
			}
		}

		dp = (mlib_s32 *)dp2;

		if (dp < dend) {
			FLOAT2INT_CLAMP(*dp, *sp);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert an f32-bit image into a d64-bit image.
 */

void
mlib_v_ImageDataTypeConvert_F32_D64(
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

#pragma pipeloop(0)
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
mlib_v_ImageDataTypeConvert_D64_BIT(
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

#pragma pipeloop(0)
		for (i = 0; i < dsize / 8; i++) {
			s0 = -(((mlib_s16 *)sp)[0] | sp[1] | sp[2] | sp[3]);
			s1 = -(((mlib_s16 *)sp)[4] | sp[5] | sp[6] | sp[7]);
			s2 = -(((mlib_s16 *)sp)[8] | sp[9] | sp[10] | sp[11]);
			s3 = -(((mlib_s16 *)sp)[12] | sp[13] | sp[14] | sp[15]);
			s4 = -(((mlib_s16 *)sp)[16] | sp[17] | sp[18] | sp[19]);
			s5 = -(((mlib_s16 *)sp)[20] | sp[21] | sp[22] | sp[23]);
			s6 = -(((mlib_s16 *)sp)[24] | sp[25] | sp[26] | sp[27]);
			s7 = -(((mlib_s16 *)sp)[28] | sp[29] | sp[30] | sp[31]);
			bitdata =
			    (s0 & 0x80000000) | (s1 & 0x40000000) | (s2 &
			    0x20000000) | (s3 & 0x10000000) | (s4 & 0x08000000)
			    | (s5 & 0x04000000) | (s6 & 0x02000000) | (s7 &
			    0x01000000);
			(*dp++) = bitdata >> 24;
			sp += 32;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
				s0 = -(((mlib_s16 *)sp)[0] | sp[1] | sp[2] |
					sp[3]);
				bitdata |= (s0 & and);
				and >>= 1;
				sp += 4;

			}
			dp[0] = (bitdata >> 24) | (dp[0] & ~mask);
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
mlib_v_ImageDataTypeConvert_D64_U8(
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

#pragma pipeloop(0)
			for (i = 0; i < num; i++)
				buf[i] = (mlib_s32)((sp[i] - 128.) * 16777216.);

#pragma pipeloop(0)
			for (i = 0; i < num; i++)
				dp[i] = (buf[i] >> 24) + 128;
#else /* MLIB_USE_FTOI_CLAMPING */
			for (i = 0; i < num; i++) {
				SAT8(dp[i], sp[i])
			}

#endif /* MLIB_USE_FTOI_CLAMPING */
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 32-bit image.
 */

void
mlib_v_ImageDataTypeConvert_D64_S32(
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

#pragma pipeloop(0)
		for (i = 0; i < width * channels; i++) {
			FLOAT2INT_CLAMP(sd, *sp);
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
mlib_v_ImageDataTypeConvert_D64_F32(
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

	sp = (mlib_d64 *)mlib_ImageGetData(src);
	dp = (mlib_f32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {

#pragma pipeloop(0)
		for (i = 0; i < width * channels; i++) {
			(*dp++) = (mlib_f32)((*sp++));
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */
