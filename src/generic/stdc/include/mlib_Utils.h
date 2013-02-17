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

#ifndef _MLIB_UTILS_H
#define	_MLIB_UTILS_H

#pragma ident	"@(#)mlib_Utils.h	9.4	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/* *********************************************************** */
#define	FP_SAT_TYPE	mlib_d64
/* *********************************************************** */
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

#define	MAKE_RANGE_S32(x)                                       \
	if (x > (FP_SAT_TYPE) MLIB_S32_MAX)                     \
	    x = (FP_SAT_TYPE) MLIB_S32_MAX;                     \
	if (x < (FP_SAT_TYPE) MLIB_S32_MIN)                     \
	    x = (FP_SAT_TYPE) MLIB_S32_MIN

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_S8(DST, SRC)	DST = (mlib_s32)(SRC) >> 24;

#define	SAT_U8(DST, SRC)                                          \
	DST = (((mlib_s32)((SRC) - 2147483648.0)) >> 24) + 128;

#define	SAT_S16(DST, SRC)	DST = (mlib_s32)(SRC) >> 16;

#define	SAT_U16(DST, SRC)                                           \
	DST = (((mlib_s32)((SRC) - 2147483648.0)) >> 16) + 32768;

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC));

#else

#define	SAT_S8(DST, SRC)                                        \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    MAKE_RANGE_S32(dsrc);                               \
	    DST = ((mlib_s32)dsrc) >> 24;                       \
	}

#define	SAT_U8(DST, SRC)                                             \
	{                                                            \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC) - 2147483648.0;   \
	                                                             \
	    MAKE_RANGE_S32(dsrc);                                    \
	    DST = (((mlib_s32)dsrc) >> 24) + 128;                    \
	}

#define	SAT_S16(DST, SRC)                                       \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    MAKE_RANGE_S32(dsrc);                               \
	    DST = ((mlib_s32)dsrc) >> 16;                       \
	}

#define	SAT_U16(DST, SRC)                                            \
	{                                                            \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC) - 2147483648.0;   \
	                                                             \
	    MAKE_RANGE_S32(dsrc);                                    \
	    DST = (((mlib_s32)dsrc) >> 16) + 32768;                  \
	}

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    MAKE_RANGE_S32(dsrc);                               \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT8(DST, SRC)	SAT_S8(DST, SRC)
#define	SAT16(DST, SRC)	SAT_S16(DST, SRC)
#define	SAT32(DST, SRC)	FLOAT2INT_CLAMP(DST, SRC)
#define	SAT_S32(DST, SRC)	FLOAT2INT_CLAMP(DST, SRC)

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	CLAMP_S8	SAT_S8

#define	CLAMP_U8	SAT_U8

#define	CLAMP_S16	SAT_S16

#define	CLAMP_U16	SAT_U16

#define	CLAMP_S16_ARITH	SAT_S16

#else

#define	CLAMP_S8(DST, SRC)                                      \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    if (dsrc > (FP_SAT_TYPE) MLIB_S8_MAX)               \
		dsrc = (FP_SAT_TYPE) MLIB_S8_MAX;               \
	    if (dsrc < (FP_SAT_TYPE) MLIB_S8_MIN)               \
		dsrc = (FP_SAT_TYPE) MLIB_S8_MIN;               \
	    DST = (mlib_s32)dsrc;                               \
	}

#define	CLAMP_U8(DST, SRC)                                      \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    if (dsrc > (FP_SAT_TYPE) MLIB_U8_MAX)               \
		dsrc = (FP_SAT_TYPE) MLIB_U8_MAX;               \
	    if (dsrc < (FP_SAT_TYPE) MLIB_U8_MIN)               \
		dsrc = (FP_SAT_TYPE) MLIB_U8_MIN;               \
	    DST = (mlib_s32)dsrc;                               \
	}

#define	CLAMP_S16(DST, SRC)                                     \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    if (dsrc > (FP_SAT_TYPE) MLIB_S16_MAX)              \
		dsrc = (FP_SAT_TYPE) MLIB_S16_MAX;              \
	    if (dsrc < (FP_SAT_TYPE) MLIB_S16_MIN)              \
		dsrc = (FP_SAT_TYPE) MLIB_S16_MIN;              \
	    DST = (mlib_s32)dsrc;                               \
	}

#define	CLAMP_U16(DST, SRC)                                     \
	{                                                       \
	    FP_SAT_TYPE dsrc = (FP_SAT_TYPE) (SRC);             \
	                                                        \
	    if (dsrc > (FP_SAT_TYPE) MLIB_U16_MAX)              \
		dsrc = (FP_SAT_TYPE) MLIB_U16_MAX;              \
	    if (dsrc < (FP_SAT_TYPE) MLIB_U16_MIN)              \
		dsrc = (FP_SAT_TYPE) MLIB_U16_MIN;              \
	    DST = (mlib_s32)dsrc;                               \
	}

#ifdef __sparc	/* Ultra-III */

#ifdef _NO_LONGLONG   /* _NO_LONGLONG */
#define	MAX 32767
#define	MIN -32768

#define	CLAMP_S16_ARITH(DST, SRC)                               \
	{                                                       \
	    mlib_s32 x = (mlib_s32)(SRC);                       \
	    mlib_s32 x_a = x - MAX;                             \
	    mlib_s32 x_b = x - MIN;                             \
	    mlib_s32 maska = (x_a | x) >> 31;                   \
	    mlib_s32 maskb = (x_b & x) >> 31;                   \
	    DST = MAX + (x_a & maska) - (x_b & maskb);          \
	}

#else
#define	CLAMP_S16_ARITH(DST, SRC)                               \
	{                                                       \
	    mlib_s32 src32 = (mlib_s32)(SRC);                   \
	    mlib_s64 src64 = (mlib_s64)src32 + (1 << 15);       \
	    mlib_s64 maskn = src64 >> 63;                       \
	    mlib_s64 maskp = (((1 << 16) - 1) - src64) >> 63;   \
	                                                        \
	    src64 = (src64 & ~maskn) | maskp;                   \
	    DST = src64 - (1 << 15);                            \
	}
#endif /* NO_LONGLONG */

#else

#define	CLAMP_S16_ARITH(DST, SRC)	CLAMP_S16(DST, SRC)

#endif

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_SCALE_U8	(1 << 24)

#define	SAT_SCALE_S16	65536.0

#define	SATUR_U8(DST, SRC)	SAT_U8(DST,  (SRC)*SAT_SCALE_U8)
#define	SATUR_S8(DST, SRC)	SAT_S8(DST,  (SRC)*SAT_SCALE_U8)
#define	SATUR_S16(DST, SRC)	SAT_S16(DST, (SRC)*SAT_SCALE_S16)
#define	SATUR_U16(DST, SRC)	SAT_U16(DST, (SRC)*SAT_SCALE_S16)

#else

#define	SAT_SCALE_U8	1.0

#define	SAT_SCALE_S16	1.0

#define	SATUR_U8(DST, SRC)	CLAMP_U8(DST, SRC)
#define	SATUR_S8(DST, SRC)	CLAMP_S8(DST, SRC)
#define	SATUR_S16(DST, SRC)	CLAMP_S16(DST, SRC)
#define	SATUR_U16(DST, SRC)	CLAMP_U16(DST, SRC)

#endif

#define	SATUR_S32(DST, SRC)	SAT_S32(DST, SRC)

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP1(X)	((mlib_s32)(X))

#else

#define	FLOAT2INT_CLAMP1(X)                                     \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <         \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : ((mlib_s32)(X))))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

/* do not copy by mlib_d64 data type for x86 */

#ifdef i386

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else

#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

/* *********************************************************** */

typedef union
{
	TYPE_64BIT d64;
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
	struct
	{
		mlib_s32 i0, i1;
	} s32x2;
	struct
	{
		mlib_u32 i0, i1;
	} u32x2;
} mlib_union64;

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_UTILS_H */
