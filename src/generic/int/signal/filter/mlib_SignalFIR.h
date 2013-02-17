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

#ifndef _MLIB_SIGNALFIR_H
#define	_MLIB_SIGNALFIR_H

#pragma ident	"@(#)mlib_SignalFIR.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */

#define	FIR_BUFFER_SIZE	512

typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_d64 *data;
} mlib_fir_data;

/* *********************************************************** */

typedef struct
{
	mlib_d64 x;
	mlib_d64 y;
} mlib_d64s;

void mlib_SignalFIR_s16_to_d64(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n);

void mlib_SignalFIR_f32_to_d64(
    mlib_d64 *pdst,
    mlib_f32 *psrc,
    mlib_s32 n);

void mlib_SignalFIR_d64_to_s16(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n);

void mlib_SignalFIR_d64_to_f32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n);

void mlib_FIR_tap4_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap4f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap3f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap1f_d64(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap4_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap4f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap3f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_tap1f_d64s(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_d64 *pflt,
    mlib_s32 n);

void mlib_FIR_coef_init_s16(mlib_d64 *pdbl,
	const mlib_f32 *flt,
	mlib_s32 stride,
	mlib_s32 tap);

void mlib_FIR_coef_init_f32(mlib_d64 *pdbl,
	const mlib_f32 *flt,
	mlib_s32 stride,
	mlib_s32 tap);

/* *********************************************************** */

typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_s32 *data;
} mlib_i_fir_data;

/* *********************************************************** */

typedef struct
{
	mlib_s16 x;
	mlib_s16 y;
} mlib_s32s;

void mlib_FIR_tap4_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap4f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap3f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap1f_s32(
    mlib_s64 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap4_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap4f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap3f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap2_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

void mlib_FIR_tap1f_s32s(
    mlib_s64 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 *pflt,
    mlib_s32 n);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALFIR_H */
