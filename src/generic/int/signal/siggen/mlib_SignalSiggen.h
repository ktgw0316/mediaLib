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

#ifndef _MLIB_SIGNALSIGGEN_H
#define	_MLIB_SIGNALSIGGEN_H

#pragma ident	"@(#)mlib_SignalSiggen.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */

typedef struct mlib_WhiteNoise_F32
{
	mlib_f32 mag;
	mlib_d64 seed;
	mlib_d64 seed1;
} mlib_WhiteNoise_F32_struct;

typedef struct mlib_WhiteNoise_S16
{
	mlib_s32 mag;
	mlib_d64 seed;
	mlib_d64 seed1;
} mlib_WhiteNoise_S16_struct;

/* *********************************************************** */

typedef struct mlib_GaussNoise_F32
{
	mlib_d64 mag;
	mlib_d64 mean;
	mlib_d64 stddev;
	mlib_d64 seed;
	mlib_d64 seed1;
} mlib_GaussNoise_F32_struct;

typedef struct mlib_GaussNoise_S16
{
	mlib_d64 mag;
	mlib_d64 min;
	mlib_d64 mean;
	mlib_f32 stddev;
	mlib_d64 seed;
	mlib_d64 seed1;
} mlib_GaussNoise_S16_struct;

typedef struct mlib_i_GaussNoise_S16
{
	mlib_s16 mag;
	mlib_d64 min;
	mlib_f32 mean;
	mlib_f32 stddev;
	mlib_d64 seed;
	mlib_d64 seed1;
} mlib_i_GaussNoise_S16_struct;

/* *********************************************************** */

#define	IA	1103515245
#define	IC	12345
/* 0x3ff07193, 0x9b6977b1 */
#define	DA	1.02772865970426274806

#ifdef _LITTLE_ENDIAN

#define	CLEAR_SEED(result, rand)                                \
	{                                                       \
	    mlib_s32 exp;                                       \
	                                                        \
	    result = rand;                                      \
	    exp = *(((mlib_s16 *)&result) + 3);                 \
	    exp = (exp & 0xf) + 0x10;                           \
	    *(((mlib_s16 *)&result) + 3) = exp;                 \
	}

#else /* _LITTLE_ENDIAN */

#define	CLEAR_SEED(result, rand)                                \
	{                                                       \
	    mlib_s32 exp;                                       \
	                                                        \
	    result = rand;                                      \
	    exp = *((mlib_s16 *)&result);                       \
	    exp = (exp & 0xf) + 0x10;                           \
	    *((mlib_s16 *)&result) = exp;                       \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	MLIB_SINE_F32	13

/* *********************************************************** */

typedef struct mlib_sinewave
{
	mlib_s32 type;
	mlib_d64 sbase1, sbase2, sbase3, sbase4;
	mlib_d64 sbase5, sbase6, sbase7, sbase8;
	mlib_d64 cstep;
} mlib_sinewave_f32_struct;

/* *********************************************************** */

#define	MLIB_SINE_S16	11

/* *********************************************************** */

typedef struct mlib_sinewave_s16
{
	mlib_s32 type, flag;
	mlib_d64 cstep, w_cstep8;
	mlib_d64 cstep1, cstep8, cstep16, cstep32, cstep64, cstep128;
	mlib_d64 sbase1, sbase2, sbase3, sbase4;
	mlib_d64 sbase5, sbase6, sbase7, sbase8;
} mlib_sinewave_s16_struct;

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALSIGGEN_H */
