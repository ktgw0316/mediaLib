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

#ifndef _MLIB_SIGNALWINDOW_H
#define	_MLIB_SIGNALWINDOW_H

#pragma ident	"@(#)mlib_SignalWindow.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

mlib_status
mlib_SignalGenBartlett_F32_i(
    mlib_f32 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenBlackman_F32_i(
    mlib_f32 *window,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenHamming_F32_i(
    mlib_f32 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenHanning_F32_i(
    mlib_f32 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);


mlib_status
mlib_SignalGenKaiser_F32_i(
    mlib_f32 *window,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend);

mlib_status
mlib_SignalGenBartlett_S16_i(
    mlib_s16 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenBlackman_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenHamming_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenHanning_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalGenKaiser_S16_i(
    mlib_s16 *window,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend);

mlib_status
mlib_SignalMulBartlett_F32S_i(
    mlib_f32 *data,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBartlett_F32S_F32S_i(
    mlib_f32 *data,
    const mlib_f32 *data1,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBartlett_F32_i(
    mlib_f32 *data,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBartlett_F32_F32_i(
    mlib_f32 *data,
    const mlib_f32 *data2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBlackman_F32_F32_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBlackman_F32S_F32S_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHamming_F32_F32_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHamming_F32S_F32S_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHanning_F32_F32_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHanning_F32S_F32S_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulKaiser_F32_F32_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulKaiser_F32S_F32S_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBartlett_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBartlett_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulKaiser_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulKaiser_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBlackman_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulBlackman_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHanning_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHanning_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHamming_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_status
mlib_SignalMulHamming_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALWINDOW_H */
