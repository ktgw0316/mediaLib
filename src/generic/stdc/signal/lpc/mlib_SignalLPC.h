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

#ifndef _MLIB_SIGNALLPC_H
#define	_MLIB_SIGNALLPC_H

#pragma ident	"@(#)mlib_SignalLPC.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_signal.h>

void mlib_SignalLPCCov_S162F32(
    mlib_f32 *signal_fp,
    const mlib_s16 *signal,
    mlib_s32 length);

mlib_status mlib_SignalLSP2LPC_double(
    mlib_d64 *lpc,
    const mlib_d64 *lsp,
    mlib_s32 order);

void mlib_corel_10(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 offset,
    mlib_s32 length);

void mlib_corel_4(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 offset,
    mlib_s32 length);

void mlib_corel_2(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 offset,
    mlib_s32 length);

void __mlib_SignalLPCAutoCorrel_F32_cor(
	mlib_f32 *cor,
	const mlib_f32 *signal,
	mlib_s32 order,
	mlib_s32 length);

void mlib_SignalLPCAutoCorrel_F32_cor(
	mlib_f32 *cor,
	const mlib_f32 *signal,
	mlib_s32 order,
	mlib_s32 length);

void __mlib_SignalLPCAutoCorrel_F32_cor_i(
	mlib_f32 *cor,
	const mlib_f32 *signal,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1);

void __mlib_SignalLPCAutoCorrel_S16_cor(
	void *cor,
	const void *signal,
	mlib_s32 order,
	mlib_s32 length);

void mlib_SignalLPCAutoCorrel_S16_cor(
	void *cor,
	const void *signal,
	mlib_s32 order,
	mlib_s32 length);

void __mlib_SignalLPCAutoCorrel_S16_cor_i(
	void *cor,
	const void *signal,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1);

#define	TYPE_CALC	mlib_f32

void
__mlib_LPC_CreateMatrix_i(
    TYPE_CALC * mat,
    const mlib_f32 *s,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 length,
    mlib_s32 ai0,
    mlib_s32 ai1);

void
__mlib_LPC_CreateMatrix(
    TYPE_CALC * mat,
    const mlib_f32 *s,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 length);

void
mlib_LPC_CreateMatrix(
    TYPE_CALC * mat,
    const mlib_f32 *s,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 length);

void
__mlib_SingalLPCPitchNorm_3(
	const mlib_f32 *sigwgt0,
	const mlib_f32 *sigwgt1,
	const mlib_f32 *sigwgt2,
	mlib_f32 *res,
	mlib_s32 length);

mlib_status
__mlib_SignalLPCPitch_F32_S16(
	mlib_f32 *dst,
	const mlib_s16 *src,
	mlib_s32 length);

void
__mlib_SingalLPCPitchCorel(mlib_f32 *ptr_corel,
	const mlib_f32 *sigwgt,
	mlib_s32 length,
	mlib_s32 upper0,
	mlib_s32 lower0,
	mlib_s32 j0,
	mlib_s32 j1,
	mlib_f32 *max,
	mlib_s32 *pitch);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALLPC_H */
