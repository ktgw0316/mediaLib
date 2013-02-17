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

#ifndef _MLIB_SIGNALCEPSTRAL_H
#define	_MLIB_SIGNALCEPSTRAL_H

#pragma ident	"@(#)mlib_SignalCepstral.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalFFTUtil.h>

/*
 * controls some implementation details
 *
 * whether to use mlib functions in FFT_standard
 */
#define	USE_MLIB_FFT_STD	0
#define	USE_MLIB_FFT_F32	DEAL_F32
#define	USE_MLIB_FFT_D64	1
/*
 *  setting a pi value
 */
#ifndef M_PI
/*
 * #define M_PI      3.141592653589793238462643383279502884
 */
#define	M_PI	3.14159265358979323846
#endif		   /* M_PI */
/*
 * whether to fill symmetric part of output in mlib_fft_r2c
 */
#define	REAL_FFT_NEEDWASTE	0
/*
 * whether to fill symmetric part of output in mlib_intr_ifft_r2c
 */
#define	REAL_IFFT_NEEDWASTE	0
/*
 * whether to fill symmetric part of output in mlib_fft_r2r
 */
#define	ICOS_FFT_NEEDWASTE	0
/* *********************************************************** */
void mlib_fft_r2c(
    deal_t data[],
    mlib_s32 order);
void mlib_fft_r2r(
    deal_t data[],
    mlib_s32 order);
void mlib_fft_r2r2(
    deal_t data[],
    mlib_s32 order);
void mlib_vlog(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 n);
void
__mlib_fft_r2c_ll_i(
	deal_t data[],
	mlib_s32 order,
	mlib_d64 sign,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i);
void
mlib_fft_r2c_ll(
	deal_t data[],
	mlib_s32 order,
	mlib_d64 sign);
void
__mlib_fft_r2c_ll(
	deal_t data[],
	mlib_s32 order,
	mlib_d64 sign);
mlib_d64
__mlib_fft_r2r_1l(
	deal_t data[],
	mlib_s32 order);
mlib_d64
mlib_fft_r2r_1l(
	deal_t data[],
	mlib_s32 order);
mlib_d64
__mlib_fft_r2r_1l_i(
	deal_t data[],
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 wr_,
	mlib_d64 wi_);

void
mlib_SignalCepstral_S16_Middle(
	deal_t *buffer,
	deal_t *coeff,
	deal_t *buffer1,
	deal_t *coeff_s,
	mlib_s32 i0,
	mlib_s32 i1);
void
__mlib_SignalCepstral_S16_Middle(
	deal_t *buffer,
	deal_t *coeff,
	deal_t *buffer1,
	deal_t *coeff_s,
	mlib_s32 i0,
	mlib_s32 i1);
void
__mlib_SignalMelCepstral_S16_Middle1(
	void *state,
	mlib_s32 i0,
	mlib_s32 i1);
void
__mlib_SignalMelCepstral_S16_Middle2(
	void *state,
	mlib_s32 i0,
	mlib_s32 i1);
void
__mlib_SignalMelCepstral_S16_Middle(
	void *state,
	mlib_s32 nfilters,
	mlib_s32 len_full);
void
mlib_SignalMelCepstral_S16_Middle(
	void *state,
	mlib_s32 nfilters,
	mlib_s32 len_full);

/* *********************************************************** */

typedef struct
{
	void *buffer;
	void *coeff;
	mlib_s32 order;
	void *coeff_s;
} mlib_cepstral;

typedef struct
{
	deal_t *filt_coeff;
	deal_t *buffer;
	deal_t *buffer1;
	deal_t *dct_in;
	deal_t *dct_out;
	mlib_s32 *index;
	mlib_s32 *index_coeff;
	mlib_s32 order;
	mlib_s32 dct_order;
	mlib_s32 nfilters;
	deal_t dct_coeff;
	deal_t dct_scale;
} mlib_melcepstral;

/* *********************************************************** */
#define	SET_FLOAT_ORDER(FLT, ODR)                               \
	* (mlib_s32 *)&(FLT) = (0x7F + (ODR)) << 23

/* *********************************************************** */
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_16(tmp)	((mlib_s32)((tmp) * 65536) >> 16)
#else /* MLIB_USE_FTOI_CLAMPING */
#define	SAT_16(tmp)                                             \
	((tmp) < MLIB_S16_MIN ? MLIB_S16_MIN : (tmp) >          \
	    MLIB_S16_MAX ? MLIB_S16_MAX : (tmp))
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALCEPSTRAL_H */
