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

#ifndef _MLIB_SIGNALUPSAMPLEFIRFUNC_H
#define	_MLIB_SIGNALUPSAMPLEFIRFUNC_H

#pragma ident	"@(#)mlib_SignalUpSampleFIRFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */

typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_s32 factor;
	mlib_s32 phase;
	mlib_d64 *data;
} mlib_upfir_data;

typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_s32 factor;
	mlib_s32 phase;
	mlib_s32 *data;
} mlib_i_upfir_data;

typedef struct
{
    mlib_s64 x;
    mlib_s64 y;
} mlib_s64s;

/* *********************************************************** */

#define	FIR_BUFFER_SIZE	512

void mlib_SignalMerge_D64_S16(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor);

void mlib_SignalMerge_D64_S16S(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor);

void mlib_SignalMerge_S16(
    mlib_s16 *pdst,
    mlib_s64 *psrc,
    mlib_s32 n,
    mlib_s32 factor);

void mlib_SignalMerge_S16S(
    mlib_s16 *pdst,
    mlib_s64 *psrc,
    mlib_s32 n,
    mlib_s32 factor);

void mlib_SignalMerge_D64_F32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor);

void mlib_SignalMerge_D64_F32S(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALUPSAMPLEFIRFUNC_H */
