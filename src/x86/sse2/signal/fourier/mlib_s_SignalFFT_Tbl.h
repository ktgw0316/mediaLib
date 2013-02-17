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

#ifndef _MLIB_S_SIGNALFFT_TBL_H
#define	_MLIB_S_SIGNALFFT_TBL_H

#pragma ident	"@(#)mlib_s_SignalFFT_Tbl.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

extern const mlib_u32 mlib_v_fftK_WTable_1_M[];
extern const mlib_u32 mlib_v_fftK_WTable_1_P[];
extern const mlib_u32 mlib_v_fftK_WTable_3_M[];
extern const mlib_u32 mlib_v_fftK_WTable_3_P[];
extern const mlib_u32 mlib_v_fftK_WTable_1D_M[];
extern const mlib_u32 mlib_v_fftK_WTable_1D_P[];
extern const mlib_u32 mlib_v_fftK_WTable_3D_M[];
extern const mlib_u32 mlib_v_fftK_WTable_3D_P[];


extern const mlib_u32 mlib_s_fftK_WTable_1_M[];
extern const mlib_u32 mlib_s_fftK_WTable_1_P[];
extern const mlib_u32 mlib_s_fftK_WTable_3_M[];
extern const mlib_u32 mlib_s_fftK_WTable_3_P[];
extern const mlib_u32 mlib_s_fftK_WTable_1D_M[];
extern const mlib_u32 mlib_s_fftK_WTable_1D_P[];
extern const mlib_u32 mlib_s_fftK_WTable_3D_M[];
extern const mlib_u32 mlib_s_fftK_WTable_3D_P[];

extern const mlib_d64 mlib_s_fftK_SinCosTable[];

extern const mlib_u16 mlib_fft_rot[];

#ifdef _SIGNAL_FFT_TABLE
#pragma align 8(mlib_v_fftK_WTable_1_M, mlib_v_fftK_WTable_1_P)
#pragma align 8(mlib_v_fftK_WTable_3_M, mlib_v_fftK_WTable_3_P)
#pragma align 8(mlib_v_fftK_WTable_1D_M, mlib_v_fftK_WTable_1D_P)
#pragma align 8(mlib_v_fftK_WTable_3D_M, mlib_v_fftK_WTable_3D_P)


#pragma align 16(mlib_s_fftK_WTable_1_M, mlib_s_fftK_WTable_1_P)
#pragma align 16(mlib_s_fftK_WTable_3_M, mlib_s_fftK_WTable_3_P)
#pragma align 16(mlib_s_fftK_WTable_1D_M, mlib_s_fftK_WTable_1D_P)
#pragma align 16(mlib_s_fftK_WTable_3D_M, mlib_s_fftK_WTable_3D_P)
#pragma align 16(mlib_s_fftK_SinCosTable)

/*
 * reminder: in fact, only 64-bit alignment is needed, for _mm_loadl_epi64()
 * is used to do loading
 */
#pragma align 16(mlib_fft_rot)

#endif /* _SIGNAL_FFT_TABLE */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_SIGNALFFT_TBL_H */
