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

#ifndef _MLIB_SIGNALRESAMPLEFIRFUNC_H
#define	_MLIB_SIGNALRESAMPLEFIRFUNC_H

#pragma ident	"@(#)mlib_SignalReSampleFIRFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */

#define	FIR_BUFFER_SIZE	512

/* *********************************************************** */

typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_s32 ufactor;
	mlib_s32 uphase;
	mlib_s32 dfactor;
	mlib_s32 dphase;
	mlib_d64 *flt;
	mlib_d64 *sbuff;
} mlib_refir_data;


typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_s32 ufactor;
	mlib_s32 uphase;
	mlib_s32 dfactor;
	mlib_s32 dphase;
	mlib_s32 *flt;
	mlib_s32 *sbuff;
} mlib_i_refir_data;

/* *********************************************************** */

void mlib_SignalFIR_s16_to_d64(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n);

void mlib_SignalFIR_f32_to_d64(
    mlib_d64 *pdst,
    mlib_f32 *psrc,
    mlib_s32 n);

#define	INIT_GSR_SCALE()

#ifdef MLIB_USE_FTOI_CLAMPING

#define	STORE_s16(data)	(*dst++) = ((mlib_s32)data) >> 16

#else /* MLIB_USE_FTOI_CLAMPING */

#define	STORE_s16(data)                                         \
	if (data >= MLIB_S16_MAX)                               \
	    data = MLIB_S16_MAX;                                \
	if (data <= MLIB_S16_MIN)                               \
	    data = MLIB_S16_MIN;                                \
	(*dst++) = (mlib_s32)data

#endif /* MLIB_USE_FTOI_CLAMPING */

#define	STORE_s16_0()	STORE_s16(data1)

#define	STORE_s16_1()	STORE_s16(data1); STORE_s16(data2)

/* *********************************************************** */

#define	STORE_INT_s16(data)                                     \
	data = (data + (1 << 22)) >> 23;                        \
	if (data >= MLIB_S16_MAX)                               \
	    data = MLIB_S16_MAX;                                \
	if (data <= MLIB_S16_MIN)                               \
	    data = MLIB_S16_MIN;                                \
	(*dst++) = (mlib_s32)data

#define	STORE_INT_s16_0()	STORE_INT_s16(data1)

#define	STORE_INT_s16_1()	STORE_INT_s16(data1); STORE_INT_s16(data2)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALRESAMPLEFIRFUNC_H */
