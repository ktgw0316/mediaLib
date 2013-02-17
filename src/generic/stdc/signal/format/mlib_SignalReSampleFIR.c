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

#pragma ident	"@(#)mlib_SignalReSampleFIR.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalReSampleFIRInit_[S16|S16S]_[S16|S16S] - allocate the
 *            memory for internal filter structure and convert the
 *            parameters into internal representation
 *
 *      mlib_SignalReSampleFIR_[S16|S16S]_[S16|S16S]_Sat - perform the
 *           resampling and FIR filtering on one packet of signal and
 *           update the internal states
 *
 *      mlib_SignaReSamplelFIRFree_[S16|S16S]_[S16|S16S] - release the
 *           memory allocated for the internal state structure
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalReSampleFIRInit_S16S_S16S(void **state,
 *                                                       const mlib_f32 *flt,
 *                                                       mlib_s32 tap,
 *                                                       mlib_s32 ufactor,
 *                                                       mlib_s32 uphase,
 *                                                       mlib_s32 dfactor,
 *                                                       mlib_s32 dphase);
 *      mlib_status mlib_SignalReSampleFIRInit_S16_S16(void **state,
 *                                                     const mlib_f32 *flt,
 *                                                     mlib_s32 tap,
 *                                                     mlib_s32 ufactor,
 *                                                     mlib_s32 uphase,
 *                                                     mlib_s32 dfactor,
 *                                                     mlib_s32 dphase);
 *      mlib_status mlib_SignalReSampleFIR_S16S_S16S_Sat(mlib_s16 *dst,
 *                                                       const mlib_s16 *src,
 *                                                       void *state,
 *                                                       mlib_s32 n);
 *      mlib_status mlib_SignalReSampleFIR_S16_S16_Sat(mlib_s16 *dst,
 *                                                     const mlib_s16 *src,
 *                                                     void *state,
 *                                                     mlib_s32 n);
 *      void mlib_SignalReSampleFIRFree_S16S_S16S(void *state);
 *      void mlib_SignalReSampleFIRFree_S16_S16(void *state);
 *
 * ARGUMENTS
 *      state      Internal state structure
 *      flt        Filter coefficient array
 *      tap        Taps of the filter
 *      ufactor    Factor by which to upsample
 *      uphase     Parameter that determines relative
 *                 position of an input value, within the output signal
 *                 0 <= uphase < ufactor
 *      dfactor    Factor by which to downsample dfactor <= upfactor
 *      dphase     Parameter that determines relative
 *                 position of an input value, within the output signal
 *                 0 <= dphase < dfactor
 *      dst        Output signal array
 *      src        Input signal array
 *      n          Number of samples in the input signal array
 *
 * DESCRIPTION
 *            n-1
 *     Z(n) = SUM h(k) * x(n-k)   n = 1, 2, ...
 *            k = 0
 *
 *     Z     - output signal array
 *     h(k)  - filter coefficients
 *     x(i)  - input signal array
 *     X       =   x(n), n = 0, 1, ...
 *     Zup     =   z(n), n = 0, 1, ...
 *
 *                 x(k)  n = (ufactor*k + uphase)
 *             =
 *                  0     otherwise
 *     Zdown   =   z(n), n = 0, 1, ...
 *
 *             =   x(dfactor*k + dphase)
 */

#include <mlib_signal.h>
#include <mlib_SignalReSampleFIRFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalReSampleFIRFree_F32S_F32S = \
	__mlib_SignalReSampleFIRFree_F32S_F32S
#pragma weak mlib_SignalReSampleFIRFree_F32_F32 = \
	__mlib_SignalReSampleFIRFree_F32_F32
#pragma weak mlib_SignalReSampleFIRFree_S16S_S16S = \
	__mlib_SignalReSampleFIRFree_S16S_S16S
#pragma weak mlib_SignalReSampleFIRFree_S16_S16 = \
	__mlib_SignalReSampleFIRFree_S16_S16
#pragma weak mlib_SignalReSampleFIRInit_F32S_F32S = \
	__mlib_SignalReSampleFIRInit_F32S_F32S
#pragma weak mlib_SignalReSampleFIRInit_F32_F32 = \
	__mlib_SignalReSampleFIRInit_F32_F32
#pragma weak mlib_SignalReSampleFIRInit_S16S_S16S = \
	__mlib_SignalReSampleFIRInit_S16S_S16S
#pragma weak mlib_SignalReSampleFIRInit_S16_S16 = \
	__mlib_SignalReSampleFIRInit_S16_S16
#pragma weak mlib_SignalReSampleFIR_F32S_F32S = \
	__mlib_SignalReSampleFIR_F32S_F32S
#pragma weak mlib_SignalReSampleFIR_F32_F32 = \
	__mlib_SignalReSampleFIR_F32_F32
#pragma weak mlib_SignalReSampleFIR_S16S_S16S_Sat = \
	__mlib_SignalReSampleFIR_S16S_S16S_Sat
#pragma weak mlib_SignalReSampleFIR_S16_S16_Sat = \
	__mlib_SignalReSampleFIR_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalReSampleFIRFree_F32S_F32S)
    mlib_SignalReSampleFIRFree_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalReSampleFIRFree_F32S_F32S")));
__typeof__(__mlib_SignalReSampleFIRFree_F32_F32)
    mlib_SignalReSampleFIRFree_F32_F32
    __attribute__((weak, alias("__mlib_SignalReSampleFIRFree_F32_F32")));
__typeof__(__mlib_SignalReSampleFIRFree_S16S_S16S)
    mlib_SignalReSampleFIRFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalReSampleFIRFree_S16S_S16S")));
__typeof__(__mlib_SignalReSampleFIRFree_S16_S16)
    mlib_SignalReSampleFIRFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalReSampleFIRFree_S16_S16")));
__typeof__(__mlib_SignalReSampleFIRInit_F32S_F32S)
    mlib_SignalReSampleFIRInit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalReSampleFIRInit_F32S_F32S")));
__typeof__(__mlib_SignalReSampleFIRInit_F32_F32)
    mlib_SignalReSampleFIRInit_F32_F32
    __attribute__((weak, alias("__mlib_SignalReSampleFIRInit_F32_F32")));
__typeof__(__mlib_SignalReSampleFIRInit_S16S_S16S)
    mlib_SignalReSampleFIRInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalReSampleFIRInit_S16S_S16S")));
__typeof__(__mlib_SignalReSampleFIRInit_S16_S16)
    mlib_SignalReSampleFIRInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalReSampleFIRInit_S16_S16")));
__typeof__(__mlib_SignalReSampleFIR_F32S_F32S) mlib_SignalReSampleFIR_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalReSampleFIR_F32S_F32S")));
__typeof__(__mlib_SignalReSampleFIR_F32_F32) mlib_SignalReSampleFIR_F32_F32
    __attribute__((weak, alias("__mlib_SignalReSampleFIR_F32_F32")));
__typeof__(__mlib_SignalReSampleFIR_S16S_S16S_Sat)
    mlib_SignalReSampleFIR_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalReSampleFIR_S16S_S16S_Sat")));
__typeof__(__mlib_SignalReSampleFIR_S16_S16_Sat)
    mlib_SignalReSampleFIR_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalReSampleFIR_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	INIT_PARAMS                                                \
	void **state, const mlib_f32 *flt, mlib_s32 tap,           \
	    mlib_s32 ufactor, mlib_s32 uphase, mlib_s32 dfactor,   \
	    mlib_s32 dphase

/* *********************************************************** */

#define	INIT_PARAMS_CALL                                        \
	state, flt, tap, ufactor, uphase, dfactor, dphase

/* *********************************************************** */

static mlib_status
__mlib_SignalReSampleFIRInit_common(
    INIT_PARAMS,
    mlib_d64 scale,
    mlib_s32 stereo)
{
	mlib_s32 bufferSize, tapf;
	mlib_s32 i, memSize;
	mlib_d64 *pdbl;
	mlib_refir_data *refir_data_ptr;
	mlib_s32 struct_size;

	*state = NULL;

	if (tap <= 0)
		return (MLIB_OUTOFRANGE);

	if (uphase < 0 || uphase >= ufactor || ufactor < 1)
		return (MLIB_FAILURE);

	if (dphase < 0 || dphase >= dfactor || dfactor < 1)
		return (MLIB_FAILURE);

	bufferSize = ((FIR_BUFFER_SIZE >> stereo) + ufactor - 1) / ufactor;
	tapf = (tap + ufactor - 1) / ufactor;

	struct_size = (sizeof (mlib_refir_data) + 7) & ~7;

	memSize =
	    struct_size + ((tapf + 1) * ufactor + (tapf +
	    bufferSize) << stereo) * sizeof (mlib_d64);

	refir_data_ptr = (mlib_refir_data *) __mlib_malloc(memSize);

	if (refir_data_ptr == NULL)
		return (MLIB_NULLPOINTER);

	pdbl = (mlib_d64 *)((mlib_u8 *)refir_data_ptr + struct_size);

	refir_data_ptr->tap = tapf;
	refir_data_ptr->bufferSize = bufferSize;
	refir_data_ptr->ufactor = ufactor;
	refir_data_ptr->uphase = uphase;
	refir_data_ptr->dfactor = dfactor;
	refir_data_ptr->dphase = dphase;
	refir_data_ptr->flt = pdbl;
	refir_data_ptr->sbuff = pdbl + (tapf + 1) * ufactor;
	*state = refir_data_ptr;

	for (i = 0; i < tap; i++) {
		(*pdbl++) = flt[i] * scale;
	}

	for (; i < (tapf + 1) * ufactor + (tapf << stereo); i++) {
		(*pdbl++) = 0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIRInit_S16_S16(
    INIT_PARAMS)
{
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_d64 scale = 65536.0;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 scale = 1.0;
#endif /* MLIB_USE_FTOI_CLAMPING */

	return (
	    __mlib_SignalReSampleFIRInit_common
	    (INIT_PARAMS_CALL, scale, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIRInit_S16S_S16S(
    INIT_PARAMS)
{
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_d64 scale = 65536.0;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 scale = 1.0;
#endif /* MLIB_USE_FTOI_CLAMPING */

	return (
	    __mlib_SignalReSampleFIRInit_common
	    (INIT_PARAMS_CALL, scale, 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIRInit_F32_F32(
    INIT_PARAMS)
{
	mlib_d64 scale = 1.0;

	return (
	    __mlib_SignalReSampleFIRInit_common
	    (INIT_PARAMS_CALL, scale, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIRInit_F32S_F32S(
    INIT_PARAMS)
{
	mlib_d64 scale = 1.0;

	return (
	    __mlib_SignalReSampleFIRInit_common
	    (INIT_PARAMS_CALL, scale, 1));
}

/* *********************************************************** */

void
__mlib_SignalReSampleFIRFree_S16_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

void
__mlib_SignalReSampleFIRFree_S16S_S16S(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

void
__mlib_SignalReSampleFIRFree_F32_F32(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

void
__mlib_SignalReSampleFIRFree_F32S_F32S(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

#define	CALC_SUM_0()                                            \
	{                                                       \
	    mlib_d64 *flt1 = flt + flt_half;                    \
	    mlib_d64 *sp1 = sp - tap_half;                      \
	                                                        \
	    data1 = flt[0] * sp[0];                             \
	    data2 = flt1[0] * sp1[0];                           \
	    flt += ufactor;                                     \
	    flt1 += ufactor;                                    \
	    for (i = 1; i < tap_half; i++) {                    \
		data1 += flt[0] * sp[-i];                       \
		data2 += flt1[0] * sp1[-i];                     \
		flt += ufactor;                                 \
		flt1 += ufactor;                                \
	    }                                                   \
	    data1 += data2;                                     \
	}

/* *********************************************************** */

#define	STORE_f32_0()	(*dst++) = (mlib_f32)data1

/* *********************************************************** */

#define	CALC_SUM_1()                                            \
	data1 = flt[0] * sp[0];                                 \
	data2 = flt[0] * sp[1];                                 \
	flt += ufactor;                                         \
	sp -= 2;                                                \
	for (i = 1; i < tap; i++) {                             \
	    data1 += flt[0] * sp[0];                            \
	    data2 += flt[0] * sp[1];                            \
	    flt += ufactor;                                     \
	    sp -= 2;                                            \
	}

/* *********************************************************** */

#define	STORE_f32_1()                                           \
	(*dst++) = (mlib_f32)data1;                             \
	(*dst++) = (mlib_f32)data2

/* *********************************************************** */

#define	FUNC(TYPE, STEREO)                                             \
	mlib_d64 *filterData;                                          \
	mlib_d64 *sl;                                                  \
	mlib_d64 data1, data2;                                         \
	mlib_s32 tap, bufferSize;                                      \
	mlib_s32 uphase, ufactor, dphase, dfactor;                     \
	mlib_s32 i, j, k, kstep, off, count, ucount;                   \
	mlib_s32 fphase, ffactor, mask;                                \
	mlib_s32 tap_half, flt_half;                                   \
	mlib_refir_data *refir_data_ptr = (mlib_refir_data *) state;   \
	                                                               \
	tap = refir_data_ptr->tap;                                     \
	bufferSize = refir_data_ptr->bufferSize;                       \
	ufactor = refir_data_ptr->ufactor;                             \
	uphase = refir_data_ptr->uphase;                               \
	dfactor = refir_data_ptr->dfactor;                             \
	dphase = refir_data_ptr->dphase;                               \
	tap_half = (tap + 1) / 2;                                      \
	flt_half = ufactor * tap_half;                                 \
	if (tap <= 0 || n <= 0) {                                      \
	    return (MLIB_FAILURE);                                     \
	}                                                              \
	if (ufactor < 1 || uphase < 0 || uphase >= ufactor ||          \
	    dfactor < 1 || dphase < 0 || dphase >= dfactor) {          \
	    return (MLIB_FAILURE);                                     \
	}                                                              \
	INIT_GSR_SCALE();                                              \
	filterData = refir_data_ptr->flt;                              \
	sl = refir_data_ptr->sbuff;                                    \
	kstep = dfactor / ufactor;                                     \
	ffactor = dfactor - kstep * ufactor;                           \
	for (off = 0; off < n; off += count) {                         \
	    count = n - off;                                           \
	    if (count > bufferSize) {                                  \
		count = bufferSize;                                    \
	    }                                                          \
	    ucount = ufactor * count;                                  \
	    mlib_SignalFIR_##TYPE##_to_d64(sl + (tap << STEREO),       \
		(void *)src, count << STEREO);                         \
	    k = (dphase < uphase) ? (tap - 1) : tap;                   \
	    fphase = (ufactor + dphase - uphase) % ufactor;            \
	    for (j = dphase; j < ucount; j += dfactor) {               \
		mlib_d64 *flt = filterData + fphase;                   \
		mlib_d64 *sp = sl + (k << STEREO);                     \
	                                                               \
		CALC_SUM_##STEREO();                                   \
		STORE_##TYPE##_##STEREO();                             \
		k += kstep;                                            \
		fphase += ffactor;                                     \
		mask = (ufactor - fphase - 1) >> 31;                   \
		k -= mask;                                             \
/*                                                                     \
 * += (1 & mask);                                                      \
 */                                                                    \
		fphase -= ufactor & mask;                              \
	    }                                                          \
	    src += (count << STEREO);                                  \
	    dphase = j - ucount;                                       \
	    for (j = 0; j < (tap << STEREO); j++) {                    \
		sl[j] = sl[j + (count << STEREO)];                     \
	    }                                                          \
	}                                                              \
	                                                               \
	refir_data_ptr->dphase = dphase;                               \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIR_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *state,
    mlib_s32 n)
{
	FUNC(s16, 0);
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIR_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *state,
    mlib_s32 n)
{
	FUNC(s16, 1);
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIR_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *state,
    mlib_s32 n)
{
	FUNC(f32, 0);
}

/* *********************************************************** */

mlib_status
__mlib_SignalReSampleFIR_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *state,
    mlib_s32 n)
{
	FUNC(f32, 1);
}

/* *********************************************************** */
