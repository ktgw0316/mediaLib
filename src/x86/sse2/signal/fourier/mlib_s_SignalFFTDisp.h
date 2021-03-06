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

#ifndef _MLIB_S_SIGNALFFTDISP_H
#define	_MLIB_S_SIGNALFFTDISP_H

#pragma ident	"@(#)mlib_s_SignalFFTDisp.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_s_sse_int.h>

/* reminder reminder: ***** should be removed later */
void mlib_v_fftK_3LayersFirst(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 isign);

void mlib_s_fftK_3LayersFirst(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 isign);

void mlib_s_fftK_3LayersFirst_nod3(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 isign);

/* ********* reminder: should be deleted later **************** */
void mlib_v_fftK_3LayersFirst_nod1(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 isign);

void mlib_s_fftK_3LayersFirst_nod1(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 isign);

void mlib_s_fftK_CalcTbl(
    void *buff,
    mlib_s32 order,
    mlib_s32 isign);

void mlib_s_fftK_Layer(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    __m128i *tbl);

void mlib_s_fftK_Layer_Div2(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    __m128i *tbl);

void mlib_s_fftK_2Layers_P_Div(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag);

void mlib_s_fftK_2Layers_M_Div(
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_SIGNALFFTDISP_H */
