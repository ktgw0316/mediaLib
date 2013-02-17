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

#pragma ident	"@(#)mlib_SignalUpSampleFIR.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalUpSampleFIRInit_* - allocate the memory for
 *           internal filter structure and convert the parameters into internal
 *           representation
 *
 *      mlib_SignalUpSampleFIR_* - perform the upsampling
 *           and FIR filtering on one packet of signal and update the internal
 *           states
 *
 *      mlib_SignaUpSamplelFIRFree_* - release the memory allocated
 *           for the internal state structure
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalUpSampleFIRInit_S16S_S16S(void           **state,
 *                                                       const mlib_f32 *flt,
 *                                                       mlib_s32       tap,
 *                                                       mlib_s32       factor,
 *                                                       mlib_s32       phase);
 *      mlib_status mlib_SignalUpSampleFIRInit_S16_S16(void           **state,
 *                                                     const mlib_f32 *flt,
 *                                                     mlib_s32       tap,
 *                                                     mlib_s32       factor,
 *                                                     mlib_s32       phase);
 *      mlib_status mlib_SignalUpSampleFIR_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                       const mlib_s16 *src,
 *                                                       void           *state,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalUpSampleFIR_S16_S16_Sat(mlib_s16       *dst,
 *                                                     const mlib_s16 *src,
 *                                                     void           *state,
 *                                                     mlib_s32       n);
 *      void mlib_SignalUpSampleFIRFree_S16S_S16S(void *state);
 *      void mlib_SignalUpSampleFIRFree_S16_S16(void *state);
 *
 *      mlib_status mlib_SignalUpSampleFIRInit_F32S_F32S(void           **state,
 *                                                       const mlib_f32 *flt,
 *                                                       mlib_s32       tap,
 *                                                       mlib_s32       factor,
 *                                                       mlib_s32       phase);
 *      mlib_status mlib_SignalUpSampleFIRInit_F32_F32(void           **state,
 *                                                     const mlib_f32 *flt,
 *                                                     mlib_s32       tap,
 *                                                     mlib_s32       factor,
 *                                                     mlib_s32       phase);
 *      mlib_status mlib_SignalUpSampleFIR_F32S_F32S(mlib_f32       *dst,
 *                                                   const mlib_f32 *src,
 *                                                   void           *state,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalUpSampleFIR_F32_F32(mlib_f32       *dst,
 *                                                 const mlib_f32 *src,
 *                                                 void           *state,
 *                                                 mlib_s32       n);
 *      void mlib_SignalUpSampleFIRFree_F32S_F32S(void *state);
 *      void mlib_SignalUpSampleFIRFree_F32_F32(void *state);
 *
 * ARGUMENTS
 *      state      Internal state structure
 *      flt        Filter coefficient array
 *      tap        Taps of the filter
 *      factor     Factor by which to upsample
 *      phase      Parameter that determines relative
 *                 position of an input value, within the output signal
 *                 0 <= phase < factor
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
 *                 x(k)  n = (factor*k + phase)
 *             =
 *                  0     otherwise
 */

#include <mlib_signal.h>
#include <../filter/mlib_SignalFIR.h>
#include <mlib_SignalUpSampleFIRFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalUpSampleFIRFree_F32S_F32S = \
	__mlib_SignalUpSampleFIRFree_F32S_F32S
#pragma weak mlib_SignalUpSampleFIRFree_F32_F32 = \
	__mlib_SignalUpSampleFIRFree_F32_F32
#pragma weak mlib_SignalUpSampleFIRFree_S16S_S16S = \
	__mlib_SignalUpSampleFIRFree_S16S_S16S
#pragma weak mlib_SignalUpSampleFIRFree_S16_S16 = \
	__mlib_SignalUpSampleFIRFree_S16_S16
#pragma weak mlib_SignalUpSampleFIRInit_F32S_F32S = \
	__mlib_SignalUpSampleFIRInit_F32S_F32S
#pragma weak mlib_SignalUpSampleFIRInit_F32_F32 = \
	__mlib_SignalUpSampleFIRInit_F32_F32
#pragma weak mlib_SignalUpSampleFIRInit_S16S_S16S = \
	__mlib_SignalUpSampleFIRInit_S16S_S16S
#pragma weak mlib_SignalUpSampleFIRInit_S16_S16 = \
	__mlib_SignalUpSampleFIRInit_S16_S16
#pragma weak mlib_SignalUpSampleFIR_F32S_F32S = \
	__mlib_SignalUpSampleFIR_F32S_F32S
#pragma weak mlib_SignalUpSampleFIR_F32_F32 = \
	__mlib_SignalUpSampleFIR_F32_F32
#pragma weak mlib_SignalUpSampleFIR_S16S_S16S_Sat = \
	__mlib_SignalUpSampleFIR_S16S_S16S_Sat
#pragma weak mlib_SignalUpSampleFIR_S16_S16_Sat = \
	__mlib_SignalUpSampleFIR_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalUpSampleFIRFree_F32S_F32S)
    mlib_SignalUpSampleFIRFree_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRFree_F32S_F32S")));
__typeof__(__mlib_SignalUpSampleFIRFree_F32_F32)
    mlib_SignalUpSampleFIRFree_F32_F32
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRFree_F32_F32")));
__typeof__(__mlib_SignalUpSampleFIRFree_S16S_S16S)
    mlib_SignalUpSampleFIRFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRFree_S16S_S16S")));
__typeof__(__mlib_SignalUpSampleFIRFree_S16_S16)
    mlib_SignalUpSampleFIRFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRFree_S16_S16")));
__typeof__(__mlib_SignalUpSampleFIRInit_F32S_F32S)
    mlib_SignalUpSampleFIRInit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRInit_F32S_F32S")));
__typeof__(__mlib_SignalUpSampleFIRInit_F32_F32)
    mlib_SignalUpSampleFIRInit_F32_F32
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRInit_F32_F32")));
__typeof__(__mlib_SignalUpSampleFIRInit_S16S_S16S)
    mlib_SignalUpSampleFIRInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRInit_S16S_S16S")));
__typeof__(__mlib_SignalUpSampleFIRInit_S16_S16)
    mlib_SignalUpSampleFIRInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalUpSampleFIRInit_S16_S16")));
__typeof__(__mlib_SignalUpSampleFIR_F32S_F32S) mlib_SignalUpSampleFIR_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalUpSampleFIR_F32S_F32S")));
__typeof__(__mlib_SignalUpSampleFIR_F32_F32) mlib_SignalUpSampleFIR_F32_F32
    __attribute__((weak, alias("__mlib_SignalUpSampleFIR_F32_F32")));
__typeof__(__mlib_SignalUpSampleFIR_S16S_S16S_Sat)
    mlib_SignalUpSampleFIR_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalUpSampleFIR_S16S_S16S_Sat")));
__typeof__(__mlib_SignalUpSampleFIR_S16_S16_Sat)
    mlib_SignalUpSampleFIR_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalUpSampleFIR_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_SCALE_S16	(1 << 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT_SCALE_S16	1.0

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

static mlib_status
mlib_SignalUpSampleFIRInit_common(
    void **state,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_d64 scale,
    mlib_s32 stereo)
{
	mlib_s32 bufferSize, tapf;
	mlib_s32 i, j, memSize;
	mlib_d64 *pdbl;
	mlib_upfir_data *upfir_data_ptr;
	mlib_s32 struct_size;

	*state = NULL;

	if (tap <= 0)
		return (MLIB_OUTOFRANGE);

	if (phase >= factor || factor < 1)
		return (MLIB_FAILURE);

	bufferSize = ((FIR_BUFFER_SIZE >> stereo) + (factor - 1)) / factor;
	tapf = (tap + factor - 1) / factor;

	struct_size = (sizeof (mlib_upfir_data) + 7) & ~7;

/* table */
	memSize = tapf * factor;
/* src */
	memSize += (tapf + bufferSize) << stereo;
/* dst */
	memSize += (bufferSize * factor) << stereo;
	memSize = memSize * sizeof (mlib_d64) + struct_size;

	upfir_data_ptr = (mlib_upfir_data *) __mlib_malloc(memSize);

	if (upfir_data_ptr == NULL) {
		return (MLIB_NULLPOINTER);
	}

	pdbl = (mlib_d64 *)((mlib_u8 *)upfir_data_ptr + struct_size);

	upfir_data_ptr->tap = tapf;
	upfir_data_ptr->bufferSize = bufferSize;
	upfir_data_ptr->factor = factor;
	upfir_data_ptr->phase = phase;
	upfir_data_ptr->data = pdbl;

	*state = upfir_data_ptr;

	for (j = 0; j < factor; j++) {
		for (i = 0; i < tapf - 1; i++) {
			(*pdbl++) = flt[i * factor + j] * scale;
		}

		if (i * factor + j >= tap) {
			(*pdbl++) = 0;
		} else {
			(*pdbl++) = flt[i * factor + j] * scale;
		}
	}

/* clear start of src buffer */
	for (i = 0; i < (tapf << stereo); i++) {
		(*pdbl++) = 0;
	}

	return (MLIB_SUCCESS);
}


/* *********************************************************** */

#if !defined(_NO_LONGLONG)

static mlib_status
mlib_i_SignalUpSampleFIRInit_common(
    void **state,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_d64 scale,
    mlib_s32 stereo)
{
	mlib_s32 bufferSize, tapf;
	mlib_s32 i, j, memSize;
	mlib_s32 *pdbl;
	mlib_i_upfir_data *upfir_data_ptr;
	mlib_s32 struct_size;

	*state = NULL;

	if (tap <= 0)
		return (MLIB_OUTOFRANGE);

	if (phase >= factor || factor < 1)
		return (MLIB_FAILURE);

	bufferSize = ((FIR_BUFFER_SIZE >> stereo) + (factor - 1)) / factor;
	tapf = (tap + factor - 1) / factor;

	struct_size = (sizeof (mlib_i_upfir_data) + 7) & ~7;

/* table */
	memSize = tapf * factor;
/* src */
	memSize += (tapf + bufferSize) << stereo;
/* dst */
	memSize += (bufferSize * factor) << stereo;
	memSize = memSize * sizeof (mlib_s64) + struct_size;

	upfir_data_ptr = (mlib_i_upfir_data *) __mlib_malloc(memSize);

	if (upfir_data_ptr == NULL) {
		return (MLIB_NULLPOINTER);
	}

	pdbl = (mlib_s32 *)((mlib_u8 *)upfir_data_ptr + struct_size);

	upfir_data_ptr->tap = tapf;
	upfir_data_ptr->bufferSize = bufferSize;
	upfir_data_ptr->factor = factor;
	upfir_data_ptr->phase = phase;
	upfir_data_ptr->data = pdbl;

	*state = upfir_data_ptr;

	for (j = 0; j < factor; j++) {
		for (i = 0; i < tapf - 1; i++) {
			(*pdbl++) = *((mlib_s32 *)&flt[i * factor + j]);
		}

		if (i * factor + j >= tap) {
			(*pdbl++) = 0;
		} else {
			(*pdbl++) = *((mlib_s32 *)&flt[i * factor + j]);
		}
	}

/* clear start of src buffer */
	for (i = 0; i < (tapf << stereo); i++) {
		(*pdbl++) = 0;
	}

	return (MLIB_SUCCESS);
}

#endif /* !defined(_NO_LONGLONG) */

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIRInit_S16_S16(
    void **state,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_s32 factor,
    mlib_s32 phase)
{
#if !defined(_NO_LONGLONG)
	return mlib_i_SignalUpSampleFIRInit_common(state, flt, tap, factor,
	    phase, 1, 0);
#else
	mlib_d64 scale = SAT_SCALE_S16;
	return mlib_SignalUpSampleFIRInit_common(state, flt, tap, factor, phase,
	    scale, 0);
#endif
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIRInit_S16S_S16S(
    void **state,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_s32 factor,
    mlib_s32 phase)
{
#if !defined(_NO_LONGLONG)
	return mlib_i_SignalUpSampleFIRInit_common(state, flt, tap, factor,
	    phase, 1, 1);
#else
	mlib_d64 scale = SAT_SCALE_S16;
	return mlib_SignalUpSampleFIRInit_common(state, flt, tap, factor, phase,
	    scale, 1);
#endif
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIRInit_F32_F32(
    void **state,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_s32 factor,
    mlib_s32 phase)
{
	mlib_d64 scale = 1.0;

	return mlib_SignalUpSampleFIRInit_common(state, flt, tap, factor, phase,
	    scale, 0);
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIRInit_F32S_F32S(
    void **state,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_s32 factor,
    mlib_s32 phase)
{
	mlib_d64 scale = 1.0;

	return mlib_SignalUpSampleFIRInit_common(state, flt, tap, factor, phase,
	    scale, 1);
}

/* *********************************************************** */

void
__mlib_SignalUpSampleFIRFree_S16_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

void
__mlib_SignalUpSampleFIRFree_S16S_S16S(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

void
__mlib_SignalUpSampleFIRFree_F32_F32(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

void
__mlib_SignalUpSampleFIRFree_F32S_F32S(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */

#define	FUNC(TYPE, STEREO, SUFF, FP)                                   \
	mlib_d64 *filterData;                                          \
	mlib_##FP *srcBuffer;                                          \
	mlib_##FP *dstBuffer;                                          \
	mlib_s32 i, k, off, p, kf, count;                              \
	mlib_s32 tap, bufferSize;                                      \
	mlib_s32 phase, factor;                                        \
	mlib_upfir_data *upfir_data_ptr = (mlib_upfir_data *) state;   \
	                                                               \
	tap = upfir_data_ptr->tap;                                     \
	bufferSize = upfir_data_ptr->bufferSize;                       \
	factor = upfir_data_ptr->factor;                               \
	phase = upfir_data_ptr->phase;                                 \
	if (tap <= 0 || factor < 1 || phase < 0 || phase >= factor)    \
	    return (MLIB_FAILURE);                                     \
	if (n <= 0)                                                    \
	    return (MLIB_FAILURE);                                     \
	filterData = upfir_data_ptr->data;                             \
	srcBuffer = (void *)(filterData + tap * factor);               \
	srcBuffer += tap;                                              \
	dstBuffer = srcBuffer + bufferSize;                            \
	for (off = 0; off < n; off += count) {                         \
	    count = n - off;                                           \
	    if (count > bufferSize) {                                  \
		count = bufferSize;                                    \
	    }                                                          \
	    mlib_SignalFIR_##TYPE##_to_d64((void *)srcBuffer,          \
		(void *)src, count << STEREO);                         \
	    for (p = 0; p < factor; p++) {                             \
		mlib_##FP *sp = srcBuffer;                             \
		void *dp = dstBuffer + count * p;                      \
	                                                               \
		k = 0;                                                 \
		if (p < phase) {                                       \
		    kf = factor + (p - phase);                         \
		} else {                                               \
		    kf = p - phase;                                    \
		    sp++;                                              \
		}                                                      \
		kf *= tap;                                             \
		if (tap == 1) {                                        \
		    k += 1;                                            \
		    mlib_FIR_tap1f_##FP(dp, (void *)(sp - k),          \
			filterData + kf, count);                       \
		    kf += 1;                                           \
		} else if (tap == 2) {                                 \
		    k += 2;                                            \
		    mlib_FIR_tap2f_##FP(dp, (void *)(sp - k),          \
			filterData + kf, count);                       \
		    kf += 2;                                           \
		} else if (tap & 1) {                                  \
		    k += 3;                                            \
		    mlib_FIR_tap3f_##FP(dp, (void *)(sp - k),          \
			filterData + kf, count);                       \
		    kf += 3;                                           \
		} else {                                               \
		    k += 4;                                            \
		    mlib_FIR_tap4f_##FP(dp, (void *)(sp - k),          \
			filterData + kf, count);                       \
		    kf += 4;                                           \
		}                                                      \
		while (k <= (tap - 4)) {                               \
		    k += 4;                                            \
		    mlib_FIR_tap4_##FP(dp, (void *)(sp - k),           \
			filterData + kf, count);                       \
		    kf += 4;                                           \
		}                                                      \
		if (k < tap) {                                         \
		    k += 2;                                            \
		    mlib_FIR_tap2_##FP(dp, (void *)(sp - k),           \
			filterData + kf, count);                       \
		}                                                      \
	    }                                                          \
	    mlib_SignalMerge_D64_##SUFF(dst, (mlib_d64 *)dstBuffer,    \
		count, factor);                                        \
	    src += (count << STEREO);                                  \
	    dst += (count << STEREO) * factor;                         \
	    for (i = 0; i < tap; i++) {                                \
		srcBuffer[i - tap] = srcBuffer[count + (i - tap)];     \
	    }                                                          \
	}                                                              \
	                                                               \
	return MLIB_SUCCESS

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

#define	MLIB_SETFIR_s32                         \
	for (int kk = 0; kk < count; kk++) {    \
		srcBuffer[kk] = src[kk];        \
	}

#define	MLIB_SETFIR_s32s                             \
	for (int kk = 0, ss = 0; kk < count; kk++) { \
		srcBuffer[kk].x = src[ss++];         \
		srcBuffer[kk].y = src[ss++];         \
	}

/* *********************************************************** */

#define	MLIB_INT_UPSAMPLEFIR(TYPE, STEREO, SUFF, DUFF) {               \
	mlib_s32 *filterData;                                          \
	mlib_##TYPE *srcBuffer;                                        \
	mlib_##DUFF *dstBuffer;                                        \
	mlib_s32 i, k, off, p, kf, count;                              \
	mlib_s32 tap, bufferSize;                                      \
	mlib_s32 phase, factor;                                        \
	mlib_i_upfir_data *upfir_data_ptr = (mlib_i_upfir_data *) state; \
	                                                               \
	tap = upfir_data_ptr->tap;                                     \
	bufferSize = upfir_data_ptr->bufferSize;                       \
	factor = upfir_data_ptr->factor;                               \
	phase = upfir_data_ptr->phase;                                 \
	if (tap <= 0 || factor < 1 || phase < 0 || phase >= factor)    \
	    return (MLIB_FAILURE);                                     \
	if (n <= 0)                                                    \
	    return (MLIB_FAILURE);                                     \
	filterData = upfir_data_ptr->data;                             \
	srcBuffer = (void *)(filterData + tap * factor);               \
	srcBuffer += tap;                                              \
	dstBuffer = (mlib_##DUFF *)((mlib_s64 *)                       \
	    ((((mlib_addr)srcBuffer & ~7) + 8)) + bufferSize);         \
	for (off = 0; off < n; off += count) {                         \
	    count = n - off;                                           \
	    if (count > bufferSize) {                                  \
		count = bufferSize;                                    \
	    }                                                          \
		MLIB_SETFIR_##TYPE /* set source data buffer */        \
	    for (p = 0; p < factor; p++) {                             \
		mlib_##TYPE *sp = srcBuffer;                           \
		void *dp = dstBuffer + count * p;                      \
	                                                               \
		k = 0;                                                 \
		if (p < phase) {                                       \
		    kf = factor + (p - phase);                         \
		} else {                                               \
		    kf = p - phase;                                    \
		    sp++;                                              \
		}                                                      \
		kf *= tap;                                             \
		if (tap == 1) {                                        \
		    k += 1;                                            \
		    mlib_FIR_tap1f_##TYPE(dp, (void *)(sp - k),        \
			filterData + kf, count);                       \
		    kf += 1;                                           \
		} else if (tap == 2) {                                 \
		    k += 2;                                            \
		    mlib_FIR_tap2f_##TYPE(dp, (void *)(sp - k),        \
			filterData + kf, count);                       \
		    kf += 2;                                           \
		} else if (tap & 1) {                                  \
		    k += 3;                                            \
		    mlib_FIR_tap3f_##TYPE(dp, (void *)(sp - k),        \
			filterData + kf, count);                       \
		    kf += 3;                                           \
		} else {                                               \
		    k += 4;                                            \
		    mlib_FIR_tap4f_##TYPE(dp, (void *)(sp - k),        \
			filterData + kf, count);                       \
		    kf += 4;                                           \
		}                                                      \
		while (k <= (tap - 4)) {                               \
		    k += 4;                                            \
		    mlib_FIR_tap4_##TYPE(dp, (void *)(sp - k),         \
			filterData + kf, count);                       \
		    kf += 4;                                           \
		}                                                      \
		if (k < tap) {                                         \
		    k += 2;                                            \
		    mlib_FIR_tap2_##TYPE(dp, (void *)(sp - k),         \
			filterData + kf, count);                       \
		}                                                      \
	    }                                                          \
	    mlib_SignalMerge_##SUFF(dst, (mlib_s64 *)dstBuffer,        \
		count, factor);                                        \
	    src += (count << STEREO);                                  \
	    dst += (count << STEREO) * factor;                         \
	    for (i = 0; i < tap; i++) {                                \
		srcBuffer[i - tap] = srcBuffer[count + (i - tap)];     \
	    }                                                          \
	}                                                              \
	                                                               \
	return (MLIB_SUCCESS);                                         \
}

#endif /* if !defined(_NO_LONGLONG */

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIR_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *state,
    mlib_s32 n)
{
#if !defined(_NO_LONGLONG)
	MLIB_INT_UPSAMPLEFIR(s32, 0, S16, s64)
#else
	FUNC(s16, 0, S16, d64);
#endif
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIR_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *state,
    mlib_s32 n)
{

#if !defined(_NO_LONGLONG)
	MLIB_INT_UPSAMPLEFIR(s32s, 1, S16S, s64s)
#else
	FUNC(s16, 1, S16S, d64s);
#endif
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIR_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *state,
    mlib_s32 n)
{
	FUNC(f32, 0, F32, d64);
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSampleFIR_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *state,
    mlib_s32 n)
{
	FUNC(f32, 1, F32S, d64s);
}

/* *********************************************************** */
