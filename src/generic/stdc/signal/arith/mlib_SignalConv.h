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

#ifndef _MLIB_SIGNALCONV_H
#define	_MLIB_SIGNALCONV_H

#pragma ident	"@(#)mlib_SignalConv.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_SignalFFTUtil.h>

void mlib_conv_fft(
    deal_t *dst_main,
    deal_t *src_main,
    mlib_s32 order,
    mlib_d64 scale);

mlib_status mlib_fftConv_S16_S16_Sat(
    mlib_s16 *dst,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n);
mlib_status mlib_fftConv_S16S_S16S_Sat(
    mlib_s16 *dst,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n);

void __mlib_SignalConvKernel_F32_a(
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void __mlib_SignalConvKernel_F32_b(
	mlib_f32 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void
mlib_SignalConvKernel_F32(
	mlib_f32 *dst,
	const mlib_f32 *src1,
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void
__mlib_SignalConvKernel_F32(
	mlib_f32 *dst,
	const mlib_f32 *src1,
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void __mlib_SignalConvKernel_F32S_a(
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void __mlib_SignalConvKernel_F32S_b(
	mlib_f32 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void
mlib_SignalConvKernel_F32S(
	mlib_f32 *dst,
	const mlib_f32 *src1,
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void
__mlib_SignalConvKernel_F32S(
	mlib_f32 *dst,
	const mlib_f32 *src1,
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void __mlib_SignalConvKernel_S16_a(
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void __mlib_SignalConvKernel_S16_b(
	mlib_s16 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void
mlib_SignalConvKernel_S16(
	mlib_s16 *dst,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void
__mlib_SignalConvKernel_S16(
	mlib_s16 *dst,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void __mlib_SignalConvKernel_S16S_a(
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void __mlib_SignalConvKernel_S16S_b(
	mlib_s16 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1);

void
mlib_SignalConvKernel_S16S(
	mlib_s16 *dst,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

void
__mlib_SignalConvKernel_S16S(
	mlib_s16 *dst,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALCONV_H */
