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
 *      mlib_v_ImageDataTypeConvert_U8_F32         - 2-D, not aligned8
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
 *      mlib_v_ImageDataTypeConvert_S32_U8_A8D1X8 - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_U8_A8D2X8 - 2-D, Aligned8, dsize 8x
 *      mlib_v_ImageDataTypeConvert_S32_U8_D1     - 1-D, not aligned8
 *      mlib_v_ImageDataTypeConvert_S32_U8        - 2-D, not aligned8
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

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

static const mlib_s32 mlib_mask[256] = {
	0x00000000, 0x00000001, 0x00000010, 0x00000011, 0x00000100, 0x00000101,
	    0x00000110, 0x00000111,
	0x00001000, 0x00001001, 0x00001010, 0x00001011, 0x00001100, 0x00001101,
	    0x00001110, 0x00001111,
	0x00010000, 0x00010001, 0x00010010, 0x00010011, 0x00010100, 0x00010101,
	    0x00010110, 0x00010111,
	0x00011000, 0x00011001, 0x00011010, 0x00011011, 0x00011100, 0x00011101,
	    0x00011110, 0x00011111,
	0x00100000, 0x00100001, 0x00100010, 0x00100011, 0x00100100, 0x00100101,
	    0x00100110, 0x00100111,
	0x00101000, 0x00101001, 0x00101010, 0x00101011, 0x00101100, 0x00101101,
	    0x00101110, 0x00101111,
	0x00110000, 0x00110001, 0x00110010, 0x00110011, 0x00110100, 0x00110101,
	    0x00110110, 0x00110111,
	0x00111000, 0x00111001, 0x00111010, 0x00111011, 0x00111100, 0x00111101,
	    0x00111110, 0x00111111,
	0x01000000, 0x01000001, 0x01000010, 0x01000011, 0x01000100, 0x01000101,
	    0x01000110, 0x01000111,
	0x01001000, 0x01001001, 0x01001010, 0x01001011, 0x01001100, 0x01001101,
	    0x01001110, 0x01001111,
	0x01010000, 0x01010001, 0x01010010, 0x01010011, 0x01010100, 0x01010101,
	    0x01010110, 0x01010111,
	0x01011000, 0x01011001, 0x01011010, 0x01011011, 0x01011100, 0x01011101,
	    0x01011110, 0x01011111,
	0x01100000, 0x01100001, 0x01100010, 0x01100011, 0x01100100, 0x01100101,
	    0x01100110, 0x01100111,
	0x01101000, 0x01101001, 0x01101010, 0x01101011, 0x01101100, 0x01101101,
	    0x01101110, 0x01101111,
	0x01110000, 0x01110001, 0x01110010, 0x01110011, 0x01110100, 0x01110101,
	    0x01110110, 0x01110111,
	0x01111000, 0x01111001, 0x01111010, 0x01111011, 0x01111100, 0x01111101,
	    0x01111110, 0x01111111,
	0x10000000, 0x10000001, 0x10000010, 0x10000011, 0x10000100, 0x10000101,
	    0x10000110, 0x10000111,
	0x10001000, 0x10001001, 0x10001010, 0x10001011, 0x10001100, 0x10001101,
	    0x10001110, 0x10001111,
	0x10010000, 0x10010001, 0x10010010, 0x10010011, 0x10010100, 0x10010101,
	    0x10010110, 0x10010111,
	0x10011000, 0x10011001, 0x10011010, 0x10011011, 0x10011100, 0x10011101,
	    0x10011110, 0x10011111,
	0x10100000, 0x10100001, 0x10100010, 0x10100011, 0x10100100, 0x10100101,
	    0x10100110, 0x10100111,
	0x10101000, 0x10101001, 0x10101010, 0x10101011, 0x10101100, 0x10101101,
	    0x10101110, 0x10101111,
	0x10110000, 0x10110001, 0x10110010, 0x10110011, 0x10110100, 0x10110101,
	    0x10110110, 0x10110111,
	0x10111000, 0x10111001, 0x10111010, 0x10111011, 0x10111100, 0x10111101,
	    0x10111110, 0x10111111,
	0x11000000, 0x11000001, 0x11000010, 0x11000011, 0x11000100, 0x11000101,
	    0x11000110, 0x11000111,
	0x11001000, 0x11001001, 0x11001010, 0x11001011, 0x11001100, 0x11001101,
	    0x11001110, 0x11001111,
	0x11010000, 0x11010001, 0x11010010, 0x11010011, 0x11010100, 0x11010101,
	    0x11010110, 0x11010111,
	0x11011000, 0x11011001, 0x11011010, 0x11011011, 0x11011100, 0x11011101,
	    0x11011110, 0x11011111,
	0x11100000, 0x11100001, 0x11100010, 0x11100011, 0x11100100, 0x11100101,
	    0x11100110, 0x11100111,
	0x11101000, 0x11101001, 0x11101010, 0x11101011, 0x11101100, 0x11101101,
	    0x11101110, 0x11101111,
	0x11110000, 0x11110001, 0x11110010, 0x11110011, 0x11110100, 0x11110101,
	    0x11110110, 0x11110111,
	0x11111000, 0x11111001, 0x11111010, 0x11111011, 0x11111100, 0x11111101,
	    0x11111110, 0x11111111
};

/* *********************************************************** */

static const mlib_s32 mlib_mask16[16] = {
	0x0000000, 0x0000001, 0x0000100, 0x0000101,
	0x0010000, 0x0010001, 0x0010100, 0x0010101,
	0x1000000, 0x1000001, 0x1000100, 0x1000101,
	0x1010000, 0x1010001, 0x1010100, 0x1010101
};

/* *********************************************************** */

static const mlib_u64 mlib_table[4] = {
	0x0ULL, 0x1ULL, 0x100000000ULL, 0x100000001ULL
};

/* *********************************************************** */

#pragma align 8(mlib_table_F32)

static const mlib_f32 mlib_table_F32[8] = {
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

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

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

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
	sd = (*sa++);                                           \
	vis_pst_8(ones, dp, sd);                                \
	vis_pst_8(zeros, dp++, ~sd)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_U8                               \
	sd0 = sd1;                                              \
	sd1 = (*sa++);                                          \
	sd = (sd0 << (8 - off)) | (sd1 >> off);                 \
	vis_pst_8(ones, dp, sd);                                \
	vis_pst_8(zeros, dp++, ~sd)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_U8_EMASK                         \
	sd0 = sd1;                                              \
	sd1 = (*sa++);                                          \
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
	mlib_s32 mask2 = 0x77777777;
	mlib_s32 i;

	sa = (void *)src;
	dp = (mlib_d64 *)dst;
	vis_write_bmask(mlib_mask[sa[0]], mask2);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		dp[i] = vis_bshuffle(zeros, ones);
		vis_write_bmask(mlib_mask[sa[i + 1]], mask2);
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

/* indices for x, y */
	mlib_s32 i, j;
	mlib_s32 mask2 = 0x77777777;

	sa = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			vis_write_bmask(mlib_mask[sa[i]], mask2);
			dp[i] = vis_bshuffle(zeros, ones);
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
	off = (mlib_addr)da & 7;
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

#define	LOAD_CONVERT_STORE_BIT_S16                                \
	sd0 = sd1;                                                \
	sd1 = (*sa++);                                            \
	dd0 = (sd0 << (4 - off)) | ((sd1 >> (4 + off)) & 0x0f);   \
	vis_pst_16(ones, dp, dd0);                                \
	vis_pst_16(zeros, dp++, ~dd0);                            \
	dd1 = sd1 >> off;                                         \
	vis_pst_16(ones, dp, dd1);                                \
	vis_pst_16(zeros, dp++, ~dd1)

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_BIT_S16_EMASK                          \
	sd0 = sd1;                                                \
	sd1 = (*sa++);                                            \
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
	mlib_d64 ones = vis_to_double_dup(0x01010101);
	mlib_s32 mask2 = 0x77777777;
	mlib_s32 i;

	sa = (void *)src;
	dp = (mlib_d64 *)dst;

	vis_write_bmask(mlib_mask16[sa[0] >> 4], mask2);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		(*dp++) = vis_bshuffle(zeros, ones);
		vis_write_bmask(mlib_mask16[sa[i] & 15], mask2);
		(*dp++) = vis_bshuffle(zeros, ones);
		vis_write_bmask(mlib_mask16[sa[i + 1] >> 4], mask2);
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

/* indices for x, y */
	mlib_s32 i, j;
	mlib_s32 mask2 = 0x77777777;
	mlib_d64 ones = vis_to_double_dup(0x01010101);

	sa = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			vis_write_bmask(mlib_mask16[sa[i] >> 4], mask2);
			(*dp++) = vis_bshuffle(zeros, ones);
			vis_write_bmask(mlib_mask16[sa[i] & 15], mask2);
			(*dp++) = vis_bshuffle(zeros, ones);
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

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_U8_BIT_A8(dm)                              \
	sd = (*sp++);                                           \
	sdh = vis_fpmerge(vis_fzeros(), vis_read_hi(sd));       \
	sdl = vis_fpmerge(vis_fzeros(), vis_read_lo(sd));       \
	dmh = vis_fcmpgt16(sdh, vis_fzero());                   \
	dml = vis_fcmpgt16(sdl, vis_fzero());                   \
	dm = ((dmh << 4) | dml)

#else /* MLIB_VIS < 0x300 */

#define	LOAD_CONVERT_U8_BIT_A8(dm)                              \
	sd = (*sp++);                                           \
	dm = vis_fucmpgt8(sd, vis_fzero())

#endif /* MLIB_VIS < 0x300 */

/* *********************************************************** */

#define	LOAD_CONVERT_U8_BIT(shl, shr, dm)                       \
	LOAD_CONVERT_U8_BIT_A8(d1);                             \
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
	mlib_d64 sd;
	mlib_s32 dm;
#if MLIB_VIS < 0x300
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml;
#endif /* MLIB_VIS < 0x300 */
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
	mlib_d64 sd;
	mlib_s32 dm;
#if MLIB_VIS < 0x300
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml;
#endif /* MLIB_VIS < 0x300 */
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
	mlib_d64 sd;
	mlib_s32 d0, d1, dm, shl, shr;
#if MLIB_VIS < 0x300
	mlib_d64 sdh, sdl;
	mlib_s32 dmh, dml;
#endif /* MLIB_VIS < 0x300 */
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
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_U8_BIT(shl, shr, dm);
			(*dp++) = dm;
		}

		if (xsize & 7) {
			mlib_s32 mask = 0xFF00 >> (xsize & 7);

			LOAD_CONVERT_U8_BIT(shl, shr, dm);
			dp[0] = (dm & mask) | (dp[0] & ~mask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	LOAD_CONVERT_U16_BIT_A8(dm)                             \
	sdh = (*sp++);                                          \
	sdl = (*sp++);                                          \
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
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_U16_BIT(shl, shr, dm);
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
	sdh = (*sp++);                                          \
	sdl = (*sp++);                                          \
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
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_S16_BIT(shl, shr, dm);
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
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_S32_BIT(shl, shr, dm);
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
	sd = (*sp++);                                           \
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
	sp = (mlib_d64 *)VIS_ALIGNADDR(src, off / 2);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

/* load 8 bytes source data */
	s0 = vis_ld_d64_nf(sp);
	s1 = vis_ld_d64_nf(sp + 1);
	sp += 2;
	sd = vis_faligndata(s0, s1);
/* convert the 1st 4 bytes into 8 bytes, and store */
	dd0 = vis_fpmerge(zeros, vis_read_hi(sd));
	vis_pst_16(dd0, dp++, emask);

/* convert the 2nd 4 bytes into 8 bytes */

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
	sd = (*sp++);                                           \
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
	    fone1 = vis_to_float(0x100),
	    fone2 = vis_to_float(0x10001);
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
	    fone1 = vis_to_float(0x100),
	    fone2 = vis_to_float(0x10001);
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
	    fone1 = vis_to_float(0x100),
	    fone2 = vis_to_float(0x10001);
	mlib_d64 *dpsrc, dsrc0, dsrc1, dsrc, dst0, dst1, dst2, dst3;
	mlib_s32 i = 0;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);
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

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_STORE_U16_U8_A8                            \
	sd0 = (*sp++);                                          \
	sd1 = (*sp++);                                          \
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

#else /* MLIB_VIS < 0x300 */

#define	LOAD_CONVERT_STORE_U16_U8_A8                            \
	sd0 = (*sp++);                                          \
	sd1 = (*sp++);                                          \
	sd0 = vis_fpsub16(sd0, displacement);                   \
	sd1 = vis_fpsub16(sd1, displacement);                   \
	sd0 = vis_fpsubs16(sd0, displacement);                  \
	sd1 = vis_fpsubs16(sd1, displacement);                  \
	*dp = vis_fpack16_pair(sd0, sd1);                       \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_U16_U8_NF                                  \
	s1 = vis_ld_d64_nf(sp);                                 \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	sd0 = vis_fpsub16(sd0, displacement);                   \
	sd1 = vis_fpsub16(sd1, displacement);                   \
	sd0 = vis_fpsubs16(sd0, displacement);                  \
	sd1 = vis_fpsubs16(sd1, displacement);                  \
	dd = vis_fpack16_pair(sd0, sd1)

#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max8 = vis_to_double_dup(0xffffffff);
	mlib_s32 mask0, mask1;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */


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
#if MLIB_VIS < 0x300
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max8 = vis_to_double_dup(0xffffffff);
	mlib_s32 mask0, mask1;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max8 = vis_to_double_dup(0xffffffff);
	mlib_s32 mask0, mask1;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

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
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, off * 2);

/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

/* convert the 1st 16 bytes into 8 bytes */
	s0 = vis_ld_d64_nf(sp);
	sp++;
	LOAD_CONVERT_U16_U8_NF;
	vis_pst_8(dd, dp, emask);
#if MLIB_VIS < 0x300
	vis_pst_8(dlog_max8, dp, mask1 & emask);
#endif /* MLIB_VIS < 0x300 */
	dp++;

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s2;
		LOAD_CONVERT_U16_U8_NF;
		*dp = dd;
#if MLIB_VIS < 0x300
		vis_pst_8(dlog_max8, dp, mask1);
#endif /* MLIB_VIS < 0x300 */
		dp++;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		s0 = s2;
		LOAD_CONVERT_U16_U8_NF;
		vis_pst_8(dd, dp, emask);
#if MLIB_VIS < 0x300
		vis_pst_8(dlog_max8, dp, mask1 & emask);
#endif /* MLIB_VIS < 0x300 */
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
	mlib_d64 d0, d1, d2, d3;
	mlib_s32 i;
	mlib_d64 zeros = vis_fzero();

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	VIS_ALIGNADDR(sp, 4);

	vis_write_bmask(0x008900ab, 0x0);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		d0 = (*sp++);
		d2 = (*sp++);
		d1 = vis_faligndata(d0, d0);
		d3 = vis_faligndata(d2, d2);
		dp[0] = vis_bshuffle(zeros, d0);
		dp[1] = vis_bshuffle(zeros, d1);
		dp[2] = vis_bshuffle(zeros, d2);
		dp[3] = vis_bshuffle(zeros, d3);
		dp += 4;
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
	mlib_d64 d0, d1, d2, d3;
	mlib_s32 i, j;
	mlib_d64 zeros = vis_fzero();

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	VIS_ALIGNADDR(sp, 4);

	vis_write_bmask(0x008900ab, 0x0);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			d0 = (*sp++);
			d2 = (*sp++);
			d1 = vis_faligndata(d0, d0);
			d3 = vis_faligndata(d2, d2);
			dp[0] = vis_bshuffle(zeros, d0);
			dp[1] = vis_bshuffle(zeros, d1);
			dp[2] = vis_bshuffle(zeros, d2);
			dp[3] = vis_bshuffle(zeros, d3);
			dp += 4;
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
		(*dp++) = (*sp++);
		length--;
	}

	ddsp = (mlib_d64 *)VIS_ALIGNADDR(sp, 0);
	ddp = (mlib_d64 *)dp;
	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	dd2 = ddsp[0];

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

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_STORE_S32_U16_A8                           \
	sd0 = sp[0];                                            \
	sd1 = sp[1];                                            \
	sp += 2;                                                \
	mask2 = vis_fcmplt32(sd0, dlog_min);                    \
	mask3 = vis_fcmplt32(sd1, dlog_min);                    \
	mask3 |= mask2 << 2;                                    \
	sd0 = vis_fpsub32(sd0, shift1);                         \
	sd1 = vis_fpsub32(sd1, shift1);                         \
	sd0 = vis_fpackfix_pair(sd0, sd1);                      \
	*dp = vis_fpadd16(sd0, shift2);                         \
	vis_pst_16(dlog_min, dp, mask3);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16                              \
	s1 = sp[0];                                             \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	mask2 = vis_fcmplt32(sd0, dlog_min);                    \
	mask3 = vis_fcmplt32(sd1, dlog_min);                    \
	mask3 |= mask2 << 2;                                    \
	sd0 = vis_fpsub32(sd0, shift1);                         \
	sd1 = vis_fpsub32(sd1, shift1);                         \
	sd0 = vis_fpackfix_pair(sd0, sd1);                      \
	*dp = vis_fpadd16(sd0, shift2);                         \
	vis_pst_16(dlog_min, dp, mask3);                        \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16_EMASK                        \
	s1 = vis_ld_d64_nf(sp);                                 \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	mask2 = vis_fcmplt32(sd0, dlog_min);                    \
	mask3 = vis_fcmplt32(sd1, dlog_min);                    \
	mask3 |= mask2 << 2;                                    \
	sd0 = vis_fpsub32(sd0, shift1);                         \
	sd1 = vis_fpsub32(sd1, shift1);                         \
	dd = vis_fpackfix_pair(sd0, sd1);                       \
	dd = vis_fpadd16(dd, shift2);                           \
	vis_pst_16(dd, dp, emask);                              \
	vis_pst_16(dlog_min, dp, mask3 & emask);                \
	dp++

#else /* MLIB_VIS < 0x300 */

#define	LOAD_CONVERT_STORE_S32_U16_A8                           \
	sd0 = sp[0];                                            \
	sd1 = sp[1];                                            \
	sp += 2;                                                \
	sd0 = vis_fpsubs32(sd0, shift1);                        \
	sd1 = vis_fpsubs32(sd1, shift1);                        \
	sd0 = vis_fpackfix_pair(sd0, sd1);                      \
	*dp = vis_fpadd16(sd0, shift2);                         \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16                              \
	s1 = sp[0];                                             \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	sd0 = vis_fpsubs32(sd0, shift1);                        \
	sd1 = vis_fpsubs32(sd1, shift1);                        \
	sd0 = vis_fpackfix_pair(sd0, sd1);                      \
	*dp = vis_fpadd16(sd0, shift2);                         \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U16_EMASK                        \
	s1 = vis_ld_d64_nf(sp);                                 \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	sd0 = vis_fpsubs32(sd0, shift1);                        \
	sd1 = vis_fpsubs32(sd1, shift1);                        \
	sd0 = vis_fpackfix_pair(sd0, sd1);                      \
	dd = vis_fpadd16(sd0, shift2);                          \
	vis_pst_16(dd, dp, emask);                              \
	dp++

#endif /* MLIB_VIS < 0x300 */

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
	mlib_d64 shift1 = vis_to_double_dup(0x8000);
	mlib_d64 shift2 = vis_to_double_dup(0x80008000);
#if MLIB_VIS < 0x300
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask2, mask3;
#endif /* MLIB_VIS < 0x300 */
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

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
	mlib_d64 shift1 = vis_to_double_dup(0x8000);
	mlib_d64 shift2 = vis_to_double_dup(0x80008000);
#if MLIB_VIS < 0x300
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask2, mask3;
#endif /* MLIB_VIS < 0x300 */

/* indices for x, y */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

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
	mlib_d64 shift1 = vis_to_double_dup(0x8000);
	mlib_d64 shift2 = vis_to_double_dup(0x80008000);
#if MLIB_VIS < 0x300
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask2, mask3;
#endif /* MLIB_VIS < 0x300 */

	sa = (void *)src;
	da = dst;

	vis_write_gsr(16 << 3);

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, 2 * off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	s0 = (*sp++);
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
	sd0 = (*sp++);                                          \
	sd1 = (*sp++);                                          \
	(*dp++) = vis_fpack16_pair(sd0, sd1)

/* *********************************************************** */

#define	LOAD_CONVERT_S16_U8                                     \
	s1 = (*sp++);                                           \
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
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, off * 2);

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
	d0 = (*sp++);                                              \
	d1 = (*sp++);                                              \
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
		(*dp++) = (*sp++);
		length--;
	}

	ddsp = (mlib_d64 *)VIS_ALIGNADDR(sp, 0);
	ddp = (mlib_d64 *)dp;
	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	dd2 = ddsp[0];

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
	* dp = vis_fpackfix_pair(sp[0], sp[1]);                 \
	dp++;                                                   \
	sp += 2

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_S16                              \
	s1 = sp[0];                                             \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	*dp = vis_fpackfix_pair(sd0, sd1);                      \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_S16_EMASK                        \
	s1 = vis_ld_d64_nf(sp);                                 \
	s2 = vis_ld_d64_nf(sp + 1);                             \
	sp += 2;                                                \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	dd = vis_fpackfix_pair(sd0, sd1);                       \
	vis_pst_16(dd, dp, emask);                              \
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
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		LOAD_CONVERT_STORE_S32_S16_A8;
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

/* indices for x, y */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_S32_S16_A8;
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

	sa = (void *)src;
	da = dst;

	vis_write_gsr(16 << 3);

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, 2 * off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	s0 = (*sp++);
	LOAD_CONVERT_STORE_S32_S16_EMASK;

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16_EMASK;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 16;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16;
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s2;
		LOAD_CONVERT_STORE_S32_S16_EMASK;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			s0 = s2;
			LOAD_CONVERT_STORE_S32_S16_EMASK;
		}
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

#if MLIB_VIS < 0x300

#define	MLIB_CONVERT_F32_U8(dd)                                 \
	sd0.value = sp[0];                                      \
	sd1.value = sp[1];                                      \
	sd2.value = sp[2];                                      \
	sd3.value = sp[3];                                      \
	sd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0);      \
	sd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1);      \
	sd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0);      \
	sd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1);      \
	sd2.twoint.int0 = (mlib_s32)(sd2.twofloat.float0);      \
	sd2.twoint.int1 = (mlib_s32)(sd2.twofloat.float1);      \
	sd3.twoint.int0 = (mlib_s32)(sd3.twofloat.float0);      \
	sd3.twoint.int1 = (mlib_s32)(sd3.twofloat.float1);      \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

#define	MLIB_CONVERT_F32_U8_NF(dd)                              \
	sd0.value = vis_ld_d64_nf(sp);                          \
	sd1.value = vis_ld_d64_nf(sp + 1);                      \
	sd2.value = vis_ld_d64_nf(sp + 2);                      \
	sd3.value = vis_ld_d64_nf(sp + 3);                      \
	sd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0);      \
	sd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1);      \
	sd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0);      \
	sd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1);      \
	sd2.twoint.int0 = (mlib_s32)(sd2.twofloat.float0);      \
	sd2.twoint.int1 = (mlib_s32)(sd2.twofloat.float1);      \
	sd3.twoint.int0 = (mlib_s32)(sd3.twofloat.float0);      \
	sd3.twoint.int1 = (mlib_s32)(sd3.twofloat.float1);      \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))


/* *********************************************************** */

#else /* MLIB_VIS < 0x300 */

#define	MLIB_CONVERT_F32_U8(dd)                                 \
	sd0.value = sp[0];                                      \
	sd1.value = sp[1];                                      \
	sd2.value = sp[2];                                      \
	sd3.value = sp[3];                                      \
	sd0.value = vis_sfstoi(sd0.value);                      \
	sd1.value = vis_sfstoi(sd1.value);                      \
	sd2.value = vis_sfstoi(sd2.value);                      \
	sd3.value = vis_sfstoi(sd3.value);                      \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

#define	MLIB_CONVERT_F32_U8_NF(dd)                              \
	sd0.value = vis_ld_d64_nf(sp);                          \
	sd1.value = vis_ld_d64_nf(sp + 1);                      \
	sd2.value = vis_ld_d64_nf(sp + 2);                      \
	sd3.value = vis_ld_d64_nf(sp + 3);                      \
	sd0.value = vis_sfstoi(sd0.value);                      \
	sd1.value = vis_sfstoi(sd1.value);                      \
	sd2.value = vis_sfstoi(sd2.value);                      \
	sd3.value = vis_sfstoi(sd3.value);                      \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

#endif /* MLIB_VIS < 0x300 */

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
/* indices for x, y */
	mlib_s32 i, j;

/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;
	mlib_d64 dd;
	type_union_mlib_d64 sd0, sd1, sd2, sd3;

	vis_write_gsr(23 << 3);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			MLIB_CONVERT_F32_U8(dd);
			(*dp++) = dd;
			sp += 4;
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
	mlib_s32 i;
	type_union_mlib_d64 sd0, sd1, sd2, sd3;

	sa = (void *)src;
	da = dst;

/* prepare the source address */
	off = ((mlib_addr)sa & 7) / 4;
	sp = (mlib_d64 *)(sa - off);

/* prepare the destination address and GSR.offset */
	dp = (mlib_d64 *)((mlib_addr)(da - off) & (~7));
	off = (mlib_addr)dp - (mlib_addr)(da - off);
	off &= 7;
	vis_write_gsr((23 << 3) | off);

	dend = da + dsize - 1;

/* process start point */
	MLIB_CONVERT_F32_U8_NF(dd0);
	sp += 4;

	if ((mlib_u8 *)(dp + 1) > da) {
		emask = vis_edge8(da, dend);
		dd = vis_faligndata(dd0, dd0);
		vis_pst_8(dd, dp, emask);
	}

	dp++;

	if (off == 0) {
		MLIB_CONVERT_F32_U8_NF(dd0);
		sp += 4;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1);

	for (i = 0; i <= (n - 8); i += 8) {
		MLIB_CONVERT_F32_U8(dd1);
		(*dp++) = vis_faligndata(dd0, dd1);
		dd0 = dd1;
		sp += 4;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		MLIB_CONVERT_F32_U8_NF(dd1);
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dp, emask);
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

#define	LOAD_CONVERT_D64_S16                                    \
	sd0.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sd0.twoint.int1 = (mlib_s32)(sp[1]);                    \
	sd1.twoint.int0 = (mlib_s32)(sp[2]);                    \
	sd1.twoint.int1 = (mlib_s32)(sp[3]);                    \
	d0 = vis_fpackfix_pair(sd0.value, sd1.value)

/* *********************************************************** */

#define	LOAD_CONVERT_D64_S16_NF                                 \
	sd0.twoint.int0 = (mlib_s32)vis_ld_d64_nf(sp);          \
	sd0.twoint.int1 = (mlib_s32)vis_ld_d64_nf(sp + 1);      \
	sd1.twoint.int0 = (mlib_s32)vis_ld_d64_nf(sp + 2);      \
	sd1.twoint.int1 = (mlib_s32)vis_ld_d64_nf(sp + 3);      \
	d0 = vis_fpackfix_pair(sd0.value, sd1.value)

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
	mlib_d64 *sp, *dp, d0;
	type_union_mlib_d64 sd0, sd1;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

#pragma pipeloop(0)
	for (i = 0; i < dsize; i += 4) {
		LOAD_CONVERT_D64_S16;
		*dp = d0;
		sp += 4;
		dp++;
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
/* indices for x, y */
	mlib_s32 i, j;

/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl, d0;
	type_union_mlib_d64 sd0, sd1;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize; i += 4) {
			LOAD_CONVERT_D64_S16;
			*dp = d0;
			sp += 4;
			dp++;
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
/* start point in source */
	mlib_d64 *sa;

/* start point in destination */
	mlib_s16 *da;

/* end point in destination */
	mlib_s16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;
	mlib_d64 d0;
	mlib_s32 i;
	type_union_mlib_d64 sd0, sd1;

	sa = (void *)src;
	da = dst;

	vis_write_gsr(16 << 3);

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, 4 * off);

	if (off != 0) {
		emask = vis_edge16(da, dend);
		LOAD_CONVERT_D64_S16_NF;
		vis_pst_16(d0, dp, emask);
		dsize -= ((8 + off) >> 1);
		sp += 4;
		dp++;
	}
#pragma pipeloop(0)
	for (i = 0; i <= (dsize - 4); i += 4) {
		LOAD_CONVERT_D64_S16;
		*dp = d0;
		sp += 4;
		dp++;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		LOAD_CONVERT_D64_S16_NF;
		vis_pst_16(d0, dp, emask);
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

#define	LOAD_CONVERT_D64_U16                                    \
	sd0.twoint.int0 = (mlib_s32)(sp[0] - soff);             \
	sd0.twoint.int1 = (mlib_s32)(sp[1] - soff);             \
	sd1.twoint.int0 = (mlib_s32)(sp[2] - soff);             \
	sd1.twoint.int1 = (mlib_s32)(sp[3] - soff);             \
	d0 = vis_fpackfix_pair(sd0.value, sd1.value);           \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_D64_U16_NF                                       \
	sd0.twoint.int0 = (mlib_s32)(vis_ld_d64_nf(sp) - soff);       \
	sd0.twoint.int1 = (mlib_s32)(vis_ld_d64_nf(sp + 1) - soff);   \
	sd1.twoint.int0 = (mlib_s32)(vis_ld_d64_nf(sp + 2) - soff);   \
	sd1.twoint.int1 = (mlib_s32)(vis_ld_d64_nf(sp + 3) - soff);   \
	d0 = vis_fpackfix_pair(sd0.value, sd1.value);                 \
	d0 = vis_fxor(d0, mask8000)

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
	mlib_d64 *sp, *dp, d0;
	mlib_d64 soff = (mlib_d64)(1 << 15);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 sd0, sd1;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

#pragma pipeloop(0)
	for (i = 0; i < dsize; i += 4) {
		LOAD_CONVERT_D64_U16;
		*dp = d0;
		sp += 4;
		dp++;
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
/* indices for x, y */
	mlib_s32 i, j;

/* 8-byte aligned pointer for src */
	mlib_d64 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl, d0;
	mlib_d64 soff = (mlib_d64)(1 << 15);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 sd0, sd1;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(16 << 3);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize; i += 4) {
			LOAD_CONVERT_D64_U16;
			*dp = d0;
			sp += 4;
			dp++;
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
/* start point in source */
	mlib_d64 *sa;

/* start point in destination */
	mlib_u16 *da;

/* end point in destination */
	mlib_u16 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;
	mlib_d64 d0;
	mlib_s32 i;
	mlib_d64 soff = (mlib_d64)(1 << 15);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 sd0, sd1;

	sa = (void *)src;
	da = dst;

	vis_write_gsr(16 << 3);

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, 4 * off);

	if (off != 0) {
		emask = vis_edge16(da, dend);
		LOAD_CONVERT_D64_U16_NF;
		vis_pst_16(d0, dp, emask);
		dsize -= ((8 + off) >> 1);
		sp += 4;
		dp++;
	}
#pragma pipeloop(0)
	for (i = 0; i <= (dsize - 4); i += 4) {
		LOAD_CONVERT_D64_U16;
		*dp = d0;
		sp += 4;
		dp++;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		LOAD_CONVERT_D64_U16_NF;
		vis_pst_16(d0, dp, emask);
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

	sd1 = (*sa++);
	dd0 = pt[(sd1 >> (6 + off))];
	vis_pst_32(dd0, dp++, emask);

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;

#pragma pipeloop(0)
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

		emask = vis_edge32(dp, dend);
		sd0 = sd1;
		sd1 = (*sa++);
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
		(*dp++) = pt[(bitdata >> 6)];
		(*dp++) = pt[(bitdata >> 4) & 3];
		(*dp++) = pt[(bitdata >> 2) & 3];
		(*dp++) = pt[(bitdata & 3)];
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
			bitdata = (*sp++);
			(*dp++) = pt[(bitdata >> 6)];
			(*dp++) = pt[(bitdata >> 4) & 3];
			(*dp++) = pt[(bitdata >> 2) & 3];
			(*dp++) = pt[(bitdata & 3)];
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

	sd1 = (*sa++);
	dd0 = pt[(sd1 >> (6 + off))];
	vis_pst_32(dd0, dp++, emask);

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;

#pragma pipeloop(0)
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

		emask = vis_edge32(dp, dend);
		sd0 = sd1;
		sd1 = (*sa++);
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
mlib_v_ImageDataTypeConvert_S16_F32_A8D1X8(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for line in source */
	mlib_d64 *sp = (mlib_d64 *)src;

/* pointer for line in destination */
	mlib_d64 *dp = (mlib_d64 *)dst;

	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i;
#if MLIB_VIS < 0x300
	mlib_s32 j, size, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;

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

#else /* MLIB_VIS < 0x300 */

#pragma pipeloop(0)
	for (i = 0; i < dsize/4; i++) {
		sd1 = sp[i];
		sd0 = vis_fmuld8ulx16(two_16_ones, vis_read_hi(sd1));
		sd1 = vis_fmuld8ulx16(two_16_ones, vis_read_lo(sd1));
		(*dp++) = vis_sfitos(sd0);
		(*dp++) = vis_sfitos(sd1);
	}

#endif /* MLIB_VIS < 0x300 */

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
/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *sl, *dl;

	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, num;
#if MLIB_VIS < 0x300
	mlib_d64 *pb;
	mlib_d64 buf[4 * SIZE];
	mlib_s32 k, size;
#endif /* MLIB_VIS < 0x300 */

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {

#if MLIB_VIS < 0x300

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

#else /* MLIB_VIS < 0x300 */

#pragma pipeloop(0)
		for (i = 0; i < 2*xsize; i++) {
			sd1 = sp[i];
			sd0 = vis_fmuld8ulx16(two_16_ones, vis_read_hi(sd1));
			sd1 = vis_fmuld8ulx16(two_16_ones, vis_read_lo(sd1));
			(*dp++) = vis_sfitos(sd0);
			(*dp++) = vis_sfitos(sd1);
		}
#endif /* MLIB_VIS < 0x300 */

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
	mlib_s16 *psrc = (void *)src;
	mlib_f32 *pdst = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1;
	mlib_s32 i = 0, num;

#if MLIB_VIS < 0x300
	mlib_d64 *pb;
	mlib_d64 buf[4 * SIZE];
	mlib_s32 j, k;
#endif /* MLIB_VIS < 0x300 */

	num = dsize;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);

	if ((mlib_addr)psrc & 7) {

#if MLIB_VIS < 0x300

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

#else /* MLIB_VIS < 0x300 */

#pragma pipeloop(0)
		for (; i <= (dsize - 8); i += 8) {
			dsrc0 = dpsrc[0];
			dsrc1 = dpsrc[1];
			d1 = vis_faligndata(dsrc0, dsrc1);
			dsrc0 = dsrc1;
			dsrc1 = dpsrc[2];
			d2 = vis_faligndata(dsrc0, dsrc1);
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d1)));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d1)));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d2)));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d2)));
			pdst += 2;
			dpsrc += 2;
			psrc += 8;
		}
	} else {

#pragma pipeloop(0)
		for (; i <= (dsize - 8); i += 8) {
			d1 = dpsrc[0];
			d2 = dpsrc[1];
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d1)));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d1)));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d2)));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d2)));
			pdst += 2;
			dpsrc += 2;
			psrc += 8;
		}

#endif /* MLIB_VIS < 0x300 */

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

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_STORE_S16_U16_A8                           \
	d0 = (*sp++);                                           \
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
	d1 = (*sp++);                                           \
	sd0 = vis_faligndata(d0, d1);                           \
	*dp = sd0;                                              \
	mask0 = vis_fcmplt16(sd0, dlog_min);                    \
	vis_pst_16(dlog_min, dp, mask0);                        \
	dp++

#else /* MLIB_VIS < 0x300 */

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16_A8                           \
	d0 = (*sp++);                                           \
	d0 = vis_fpadds16(d0, displacement);                    \
	*dp = vis_fxor(d0, displacement);                       \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16_EMASK                        \
	d1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(d0, d1);                           \
	sd0 = vis_fpadds16(sd0, displacement);                  \
	sd0 = vis_fxor(sd0, displacement);                      \
	vis_pst_16(sd0, dp, emask);                             \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S16_U16                              \
	d1 = (*sp++);                                           \
	sd0 = vis_faligndata(d0, d1);                           \
	sd0 = vis_fpadds16(sd0, displacement);                  \
	*dp = vis_fxor(sd0, displacement);                      \
	dp++

#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask0;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask0;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 dlog_min = vis_fzero();
	mlib_s32 mask0;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	d0 = (*sp++);

	LOAD_CONVERT_STORE_S16_U16_EMASK;

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		d0 = d1;
		LOAD_CONVERT_STORE_S16_U16_EMASK;
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 2) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		d0 = d1;
		LOAD_CONVERT_STORE_S16_U16;
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

	dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);

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
/* pointer for line in source */
	mlib_d64 *sp = (mlib_d64 *)src;

/* pointer for line in destination */
	mlib_d64 *dp = (mlib_d64 *)dst;

	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 mask = vis_to_double_dup(0xffff);
	mlib_d64 sd0, sd1;
	mlib_s32 i;

#if MLIB_VIS < 0x300
	mlib_s32 j, size, num;
	mlib_d64 buf[4 * SIZE];
	mlib_d64 *pb;

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

#else /* MLIB_VIS < 0x300 */

#pragma pipeloop(0)
	for (i = 0; i < dsize/4; i++) {
		sd1 = sp[i];
		sd0 = vis_fand(vis_fmuld8ulx16(
			two_16_ones, vis_read_hi(sd1)), mask);
		sd1 = vis_fand(vis_fmuld8ulx16(
			two_16_ones, vis_read_lo(sd1)), mask);
		(*dp++) = vis_sfitos(sd0);
		(*dp++) = vis_sfitos(sd1);
	}

#endif /* MLIB_VIS < 0x300 */

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
/* pointer for line in source */
	mlib_d64 *sp;

/* pointer for line in destination */
	mlib_d64 *dp;
	mlib_d64 *sl, *dl;

	mlib_f32 two_16_ones = vis_to_float(0x10001);
	mlib_d64 mask = vis_to_double_dup(0xffff);
	mlib_d64 sd0, sd1;
	mlib_s32 i, j, num;
#if MLIB_VIS < 0x300
	mlib_d64 *pb;
	mlib_d64 buf[4 * SIZE];
	mlib_s32 k, size;
#endif /* MLIB_VIS < 0x300 */

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	xsize >>= 3;
	num = xsize;

	for (j = 0; j < ysize; j++) {

#if MLIB_VIS < 0x300

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

#else /* MLIB_VIS < 0x300 */

#pragma pipeloop(0)
		for (i = 0; i < 2*xsize; i++) {
			sd1 = sp[i];
			sd0 = vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(sd1)), mask);
			sd1 = vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(sd1)), mask);
			(*dp++) = vis_sfitos(sd0);
			(*dp++) = vis_sfitos(sd1);
		}
#endif /* MLIB_VIS < 0x300 */

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
	mlib_d64 mask = vis_to_double_dup(0xffff);
	mlib_d64 d1, d2;
	mlib_u16 *psrc = (void *)src;
	mlib_f32 *pdst = dst;
	mlib_d64 *dpsrc, dsrc0, dsrc1;
	mlib_s32 i = 0, num;

#if MLIB_VIS < 0x300
	mlib_d64 *pb;
	mlib_d64 buf[4 * SIZE];
	mlib_s32 j, k;
#endif /* MLIB_VIS < 0x300 */

	num = dsize;

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);

	if ((mlib_addr)psrc & 7) {

#if MLIB_VIS < 0x300

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

#else /* MLIB_VIS < 0x300 */

#pragma pipeloop(0)
		for (; i <= (dsize - 8); i += 8) {
			dsrc0 = dpsrc[0];
			dsrc1 = dpsrc[1];
			d1 = vis_faligndata(dsrc0, dsrc1);
			dsrc0 = dsrc1;
			dsrc1 = dpsrc[2];
			d2 = vis_faligndata(dsrc0, dsrc1);
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d1)), mask));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d1)), mask));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d2)), mask));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d2)), mask));
			pdst += 2;
			dpsrc += 2;
			psrc += 8;
		}
	} else {

#pragma pipeloop(0)
		for (; i <= (dsize - 8); i += 8) {
			d1 = dpsrc[0];
			d2 = dpsrc[1];
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d1)), mask));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d1)), mask));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_hi(d2)), mask));
			pdst += 2;
			*(mlib_d64 *)pdst = vis_sfitos(vis_fand(vis_fmuld8ulx16(
				two_16_ones, vis_read_lo(d2)), mask));
			pdst += 2;
			dpsrc += 2;
			psrc += 8;
		}

#endif /* MLIB_VIS < 0x300 */

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

	dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);

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

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_STORE_U16_S16_A8                           \
	d0 = (*sp++);                                           \
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
	d1 = (*sp++);                                           \
	sd0 = vis_faligndata(d0, d1);                           \
	*dp = sd0;                                              \
	mask0 = vis_fcmplt16(sd0, zero);                        \
	vis_pst_16(dlog_max, dp, mask0);                        \
	dp++

#else /* MLIB_VIS < 0x300 */

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16_A8                           \
	d0 = (*sp++);                                           \
	d0 = vis_fxor(d0, displacement);                        \
	*dp = vis_fpsubs16(d0, displacement);                   \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16_EMASK                        \
	d1 = vis_ld_d64_nf(sp);                                 \
	sp++;                                                   \
	sd0 = vis_faligndata(d0, d1);                           \
	sd0 = vis_fxor(sd0, displacement);                      \
	sd0 = vis_fpsubs16(sd0, displacement);                  \
	vis_pst_16(sd0, dp, emask);                             \
	dp++

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_U16_S16                              \
	d1 = (*sp++);                                           \
	sd0 = vis_faligndata(d0, d1);                           \
	sd0 = vis_fxor(sd0, displacement);                      \
	*dp = vis_fpsubs16(sd0, displacement);                  \
	dp++

#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

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
#if MLIB_VIS < 0x300
	mlib_d64 zero = vis_fzero();
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask0;
#else /* MLIB_VIS < 0x300 */
	mlib_d64 displacement = vis_to_double_dup(0x80008000);
#endif /* MLIB_VIS < 0x300 */

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
/* in bytes */
	off = (mlib_addr)dp - (mlib_addr)da;
/* in SHORTs */
	dend = dst + dsize - 1;

	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, dend);

	d0 = (*sp++);

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
		LOAD_CONVERT_STORE_U16_S16;
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

/* *********************************************************** */

#define	LOAD_CONVERT_STORE_S32_U8_A8                              \
	d0 = sp[0];                                               \
	d1 = sp[1];                                               \
	d2 = sp[2];                                               \
	d3 = sp[3];                                               \
	sp += 4;                                                  \
	dd = vis_fpack32(d0, d0);                                 \
	dd = vis_fpack32(dd, d1);                                 \
	dd = vis_fpack32(dd, d2);                                 \
	dd = vis_fpack32(dd, d3);                                 \
	(*dp++) = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

#define	LOAD_CONVERT_S32_U8                                     \
	d1 = sp[0];                                             \
	d2 = sp[1];                                             \
	d3 = sp[2];                                             \
	d4 = sp[3];                                             \
	sp += 4;                                                \
	sd0 = vis_faligndata(d0, d1);                           \
	sd1 = vis_faligndata(d1, d2);                           \
	sd2 = vis_faligndata(d2, d3);                           \
	sd3 = vis_faligndata(d3, d4);                           \
	dd = vis_fpack32(sd0, sd0);                             \
	dd = vis_fpack32(dd, sd1);                              \
	dd = vis_fpack32(dd, sd2);                              \
	dd = vis_fpack32(dd, sd3);                              \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

#define	LOAD_CONVERT_S32_U8_NF                                  \
	d1 = vis_ld_d64_nf(sp);                                 \
	d2 = vis_ld_d64_nf(sp + 1);                             \
	d3 = vis_ld_d64_nf(sp + 2);                             \
	d4 = vis_ld_d64_nf(sp + 3);                             \
	sp += 4;                                                \
	sd0 = vis_faligndata(d0, d1);                           \
	sd1 = vis_faligndata(d1, d2);                           \
	sd2 = vis_faligndata(d2, d3);                           \
	sd3 = vis_faligndata(d3, d4);                           \
	dd = vis_fpack32(sd0, sd0);                             \
	dd = vis_fpack32(dd, sd1);                              \
	dd = vis_fpack32(dd, sd2);                              \
	dd = vis_fpack32(dd, sd3);                              \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageDataTypeConvert_S32_U8_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_d64 *sp;
	mlib_d64 *dp;
	mlib_d64 d0, d1, d2, d3, dd;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	vis_write_gsr(23 << 3);

#pragma pipeloop(0)
	for (; dsize > 15; dsize -= 16) {
		LOAD_CONVERT_STORE_S32_U8_A8;
		LOAD_CONVERT_STORE_S32_U8_A8;
	}

	for (; dsize > 7; dsize -= 8) {
		LOAD_CONVERT_STORE_S32_U8_A8;
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors, but
 * source and destination image data are 8-byte aligned.
 */

void
mlib_v_ImageDataTypeConvert_S32_U8_A8D2X8(
    const mlib_s32 *src,
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
	mlib_d64 d0, d1, d2, d3, dd;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	vis_write_gsr(23 << 3);

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			LOAD_CONVERT_STORE_S32_U8_A8;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_v_ImageDataTypeConvert_S32_U8_D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* start point in source */
	mlib_s32 *sa;

/* start point in destination */
	mlib_u8 *da;

/* end point in destination */
	mlib_u8 *dend;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 d0, d1, d2, d3, d4, dd;

/* offset of address alignment in destination */
	mlib_s32 off, n;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

/* set GSR.scale_factor */
	vis_write_gsr(23 << 3);

	sa = (void *)src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address and GSR.offset */
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, off * 4);

/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

/* convert the 1st 32 bytes into 8 bytes */
	d0 = (*sp++);
	LOAD_CONVERT_S32_U8_NF;
	vis_pst_8(dd, dp++, emask);

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 1) / 8;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		d0 = d4;
		LOAD_CONVERT_S32_U8;
		(*dp++) = dd;
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		d0 = d4;
		LOAD_CONVERT_S32_U8_NF;
		vis_pst_8(dd, dp, emask);
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_v_ImageDataTypeConvert_S32_U8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sl;
	mlib_u8 *dl;
	mlib_s32 j;

	sl = (void *)src;
	dl = dst;

/* in pixel */
	slb >>= 2;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageDataTypeConvert_S32_U8_D1(sl, dl, xsize);
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

#pragma pipeloop(0)
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

#pragma pipeloop(0)
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
 * Convert an 8-bit image into a f32-bit image.
 */

void
mlib_v_ImageDataTypeConvert_U8_F32(
    const mlib_image *src,
    mlib_image *dst)
{
	mlib_f32 *lut[4] = { (void *)mlib_U82F32, (void *)mlib_U82F32,
		(void *)mlib_U82F32, (void *)mlib_U82F32
	};

	__mlib_ImageLookUp(dst, src, (const void **)lut);
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

		dp2 = (mlib_d64 *)dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (; (mlib_f32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*sp2++);
#if MLIB_VIS < 0x300
				dd.f32s.f0 = (mlib_f32)dd.i32s.f0;
				dd.f32s.f1 = (mlib_f32)dd.i32s.f1;
#else /* MLIB_VIS < 0x300 */
				dd.d64 = vis_sfitos(dd.d64);
#endif /* MLIB_VIS < 0x300 */
				dp2[0] = dd.d64;
			}

			sp = (mlib_s32 *)sp2;
		} else {

#pragma pipeloop(0)
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

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_F32_S16_1                                  \
	sd0.value = *(mlib_d64 *)sp;                            \
	sd1.value = *(mlib_d64 *)(sp + 2);                      \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0);      \
	dd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1);      \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0);      \
	dd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1);      \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_S16_1_NF                               \
	sd0.value = vis_ld_d64_nf(sp);                          \
	sd1.value = vis_ld_d64_nf(sp + 2);                      \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0);      \
	dd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1);      \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0);      \
	dd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1);      \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value)

/* *********************************************************** */

#else /* MLIB_VIS < 0x300 */

#define	LOAD_CONVERT_F32_S16_1                                  \
	sd0.value = *(mlib_d64 *)sp;                            \
	sd1.value = *(mlib_d64 *)(sp + 2);                      \
	dd0.value = vis_sfstoi(sd0.value);                      \
	dd1.value = vis_sfstoi(sd1.value);                      \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_S16_1_NF                               \
	sd0.value = vis_ld_d64_nf(sp);                          \
	sd1.value = vis_ld_d64_nf(sp + 2);                      \
	dd0.value = vis_sfstoi(sd0.value);                      \
	dd1.value = vis_sfstoi(sd1.value);                      \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value)

#endif /* MLIB_VIS < 0x300 */

/* *********************************************************** */

#define	LOAD_CONVERT_F32_S16_2                                  \
	sd1.value = *(mlib_d64 *)sp;                            \
	sd2.value = *(mlib_d64 *)(sp + 2);                      \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float1);      \
	dd0.twoint.int1 = (mlib_s32)(sd1.twofloat.float0);      \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float1);      \
	dd1.twoint.int1 = (mlib_s32)(sd2.twofloat.float0);      \
	sd0.twofloat.float1 = sd2.twofloat.float1;              \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_S16_2_NF                               \
	sd1.value = vis_ld_d64_nf(sp);                          \
	sd2.value = vis_ld_d64_nf(sp + 2);                      \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float1);      \
	dd0.twoint.int1 = (mlib_s32)(sd1.twofloat.float0);      \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float1);      \
	dd1.twoint.int1 = (mlib_s32)(sd2.twofloat.float0);      \
	sd0.twofloat.float1 = sd2.twofloat.float1;              \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value)

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
	mlib_s16 *dend;
	mlib_d64 d0;
	mlib_s32 width, dsize;
	mlib_s32 height, off, emask;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;
	type_union_mlib_d64 sd0, sd1, sd2, dd0, dd1;

/* set GSR.scale_factor */
	vis_write_gsr(16 << 3);

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_f32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
		dsize = width * channels;
		dend = dl + dsize - 1;
		off = ((mlib_addr)dp & 7) >> 1;
		dp -= off;
		sp -= off;

		if (((mlib_addr)sp & 7) == 0) {
			if (off != 0) {
				emask = vis_edge16(dl, dend);
				LOAD_CONVERT_F32_S16_1_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
				dsize -= (4 - off);
				sp += 4;
				dp += 4;
			}
#pragma pipeloop(0)
			for (i = 0; i <= (dsize - 4); i += 4) {
				LOAD_CONVERT_F32_S16_1;
				*(mlib_d64 *)dp = d0;
				sp += 4;
				dp += 4;
			}

/* right end handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				LOAD_CONVERT_F32_S16_1_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
			}
		} else {
			sp--;
			sd0.value = *(mlib_d64 *)sp;
			sp += 2;

			if (off != 0) {
				emask = vis_edge16(dl, dend);
				LOAD_CONVERT_F32_S16_2_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
				dsize -= (4 - off);
				sp += 4;
				dp += 4;
			}
#pragma pipeloop(0)
			for (i = 0; i <= (dsize - 4); i += 4) {
				LOAD_CONVERT_F32_S16_2;
				*(mlib_d64 *)dp = d0;
				sp += 4;
				dp += 4;
			}

/* right end handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				LOAD_CONVERT_F32_S16_2_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

#if MLIB_VIS < 0x300

#define	LOAD_CONVERT_F32_U16_1                                      \
	sd0.value = *(mlib_d64 *)sp;                                \
	sd1.value = *(mlib_d64 *)(sp + 2);                          \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0 - soff);   \
	dd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1 - soff);   \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0 - soff);   \
	dd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1 - soff);   \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_U16_1_NF                                   \
	sd0.value = vis_ld_d64_nf(sp);                              \
	sd1.value = vis_ld_d64_nf(sp + 2);                          \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0 - soff);   \
	dd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1 - soff);   \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0 - soff);   \
	dd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1 - soff);   \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_U16_2                                      \
	sd1.value = *(mlib_d64 *)sp;                                \
	sd2.value = *(mlib_d64 *)(sp + 2);                          \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float1 - soff);   \
	dd0.twoint.int1 = (mlib_s32)(sd1.twofloat.float0 - soff);   \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float1 - soff);   \
	dd1.twoint.int1 = (mlib_s32)(sd2.twofloat.float0 - soff);   \
	sd0.twofloat.float1 = sd2.twofloat.float1;                  \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_U16_2_NF                                   \
	sd1.value = vis_ld_d64_nf(sp);                              \
	sd2.value = vis_ld_d64_nf(sp + 2);                          \
	dd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float1 - soff);   \
	dd0.twoint.int1 = (mlib_s32)(sd1.twofloat.float0 - soff);   \
	dd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float1 - soff);   \
	dd1.twoint.int1 = (mlib_s32)(sd2.twofloat.float0 - soff);   \
	sd0.twofloat.float1 = sd2.twofloat.float1;                  \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#else /* MLIB_VIS < 0x300 */

#define	LOAD_CONVERT_F32_U16_1                                      \
	sd0.value = *(mlib_d64 *)sp;                                \
	sd1.value = *(mlib_d64 *)(sp + 2);                          \
	dd0.twofloat.float0 = sd0.twofloat.float0 - soff;           \
	dd0.twofloat.float1 = sd0.twofloat.float1 - soff;           \
	dd1.twofloat.float0 = sd1.twofloat.float0 - soff;           \
	dd1.twofloat.float1 = sd1.twofloat.float1 - soff;           \
	dd0.value = vis_sfstoi(dd0.value);                          \
	dd1.value = vis_sfstoi(dd1.value);                          \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_U16_1_NF                                   \
	sd0.value = vis_ld_d64_nf(sp);                              \
	sd1.value = vis_ld_d64_nf(sp + 2);                          \
	dd0.twofloat.float0 = sd0.twofloat.float0 - soff;           \
	dd0.twofloat.float1 = sd0.twofloat.float1 - soff;           \
	dd1.twofloat.float0 = sd1.twofloat.float0 - soff;           \
	dd1.twofloat.float1 = sd1.twofloat.float1 - soff;           \
	dd0.value = vis_sfstoi(dd0.value);                          \
	dd1.value = vis_sfstoi(dd1.value);                          \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_U16_2                                      \
	sd1.value = *(mlib_d64 *)sp;                                \
	sd2.value = *(mlib_d64 *)(sp + 2);                          \
	dd0.twofloat.float0 = sd0.twofloat.float1 - soff;           \
	dd0.twofloat.float1 = sd1.twofloat.float0 - soff;           \
	dd1.twofloat.float0 = sd1.twofloat.float1 - soff;           \
	dd1.twofloat.float1 = sd2.twofloat.float0 - soff;           \
	dd0.value = vis_sfstoi(dd0.value);                          \
	dd1.value = vis_sfstoi(dd1.value);                          \
	sd0.twofloat.float1 = sd2.twofloat.float1;                  \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

/* *********************************************************** */

#define	LOAD_CONVERT_F32_U16_2_NF                                   \
	sd1.value = vis_ld_d64_nf(sp);                              \
	sd2.value = vis_ld_d64_nf(sp + 2);                          \
	dd0.twofloat.float0 = sd0.twofloat.float1 - soff;           \
	dd0.twofloat.float1 = sd1.twofloat.float0 - soff;           \
	dd1.twofloat.float0 = sd1.twofloat.float1 - soff;           \
	dd1.twofloat.float1 = sd2.twofloat.float0 - soff;           \
	dd0.value = vis_sfstoi(dd0.value);                          \
	dd1.value = vis_sfstoi(dd1.value);                          \
	sd0.twofloat.float1 = sd2.twofloat.float1;                  \
	d0 = vis_fpackfix_pair(dd0.value, dd1.value);               \
	d0 = vis_fxor(d0, mask8000)

#endif /* MLIB_VIS < 0x300 */

/* *********************************************************** */

/*
 * Convert a F32 image into an U16 image.
 */

void
mlib_v_ImageDataTypeConvert_F32_U16(
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
	mlib_s16 *dend;
	mlib_d64 d0;
	mlib_s32 width, dsize;
	mlib_s32 height, off, emask;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;
	mlib_f32 soff = (mlib_f32)(1 << 15);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 sd0, sd1, sd2, dd0, dd1;

/* set GSR.scale_factor */
	vis_write_gsr(16 << 3);

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_f32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
		dsize = width * channels;
		dend = dl + dsize - 1;
		off = ((mlib_addr)dp & 7) >> 1;
		dp -= off;
		sp -= off;

		if (((mlib_addr)sp & 7) == 0) {
			if (off != 0) {
				emask = vis_edge16(dl, dend);
				LOAD_CONVERT_F32_U16_1_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
				dsize -= (4 - off);
				sp += 4;
				dp += 4;
			}
#pragma pipeloop(0)
			for (i = 0; i <= (dsize - 4); i += 4) {
				LOAD_CONVERT_F32_U16_1;
				*(mlib_d64 *)dp = d0;
				sp += 4;
				dp += 4;
			}

/* right end handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				LOAD_CONVERT_F32_U16_1_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
			}
		} else {
			sp--;
			sd0.value = *(mlib_d64 *)sp;
			sp += 2;

			if (off != 0) {
				emask = vis_edge16(dl, dend);
				LOAD_CONVERT_F32_U16_2_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
				dsize -= (4 - off);
				sp += 4;
				dp += 4;
			}
#pragma pipeloop(0)
			for (i = 0; i <= (dsize - 4); i += 4) {
				LOAD_CONVERT_F32_U16_2;
				*(mlib_d64 *)dp = d0;
				sp += 4;
				dp += 4;
			}

/* right end handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				LOAD_CONVERT_F32_U16_2_NF;
				vis_pst_16(d0, (mlib_d64 *)dp, emask);
			}
		}

		sp = sl += strides;
		dp = dl += strided;
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
#if MLIB_VIS < 0x300
				FLOAT2INT_CLAMP(dd.i32s.f0, dd.f32s.f0);
				FLOAT2INT_CLAMP(dd.i32s.f1, dd.f32s.f1);
#else /* MLIB_VIS < 0x300 */
				dd.d64 = vis_sfstoi(dd.d64);
#endif /* MLIB_VIS < 0x300 */
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
	mlib_d64 dd;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 n, num, off;

/* indices for x, y */
	mlib_s32 i, j;
	type_union_mlib_d64 sd0, sd1, sd2, sd3;

	vis_write_gsr(23 << 3);

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

	n = width * channels;

	sp = sl;
	dp = dl;

	for (j = 0; j < height; j++) {
		off = (8 - ((mlib_addr)dp & 7)) & 7;

		off = (off < n) ? off : n;

		num = n - off;

		for (i = 0; i < off; i++) {
			sd0.twoint.int1 = (mlib_s32)(sp[0]);
			sd1.value = vis_fpack32(sd0.value, sd0.value);
			sp++;
			(*dp++) = sd1.twoint.int1;
		}

#pragma pipeloop(0)
		for (i = 0; i <= (num - 8); i += 8) {
			sd0.twoint.int0 = (mlib_s32)(sp[0]);
			sd1.twoint.int0 = (mlib_s32)(sp[1]);
			sd2.twoint.int0 = (mlib_s32)(sp[2]);
			sd3.twoint.int0 = (mlib_s32)(sp[3]);
			sd0.twoint.int1 = (mlib_s32)(sp[4]);
			sd1.twoint.int1 = (mlib_s32)(sp[5]);
			sd2.twoint.int1 = (mlib_s32)(sp[6]);
			sd3.twoint.int1 = (mlib_s32)(sp[7]);

			dd = vis_fpack32(sd0.value, sd0.value);
			dd = vis_fpack32(dd, sd1.value);
			dd = vis_fpack32(dd, sd2.value);
			dd = vis_fpack32(dd, sd3.value);

			*(mlib_d64 *)dp = dd;

			dp += 8;
			sp += 8;
		}

		for (; i < num; i++) {
			sd0.twoint.int1 = (mlib_s32)(sp[0]);
			sd1.value = vis_fpack32(sd0.value, sd0.value);
			sp++;
			(*dp++) = sd1.twoint.int1;
		}

		sp = sl += strides;
		dp = dl += strided;
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
