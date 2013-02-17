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

#pragma ident	"@(#)mlib_s_SignalFIR.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalFIRInit_[S16|S16S]_[S16|S16S] - allocate the memory for
 *           internal filter structure and convert the
 *           filter coefficients into internal representation
 *
 *      mlib_SignalFIR_[S16|S16S]_[S16|S16S]_Sat - apply the FIR filter one
 *           packet of signal and update the filter states
 *
 *      mlib_SignalFIRFree_[S16|S16S]_[S16|S16S] - release the memory
 *           allocated for the internal filter structure
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalFIRInit_S16S_S16S(void           **filter,
 *                                               const mlib_f32 *flt,
 *                                               mlib_s32       tap);
 *      mlib_status mlib_SignalFIRInit_S16_S16(void           **filter,
 *                                             const mlib_f32 *flt,
 *                                             mlib_s32       tap);
 *      mlib_status mlib_SignalFIR_S16S_S16S_Sat(mlib_s16       *dst,
 *                                               const mlib_s16 *src,
 *                                               void           *filter,
 *                                               mlib_s32       n);
 *      mlib_status mlib_SignalFIR_S16_S16_Sat(mlib_s16       *dst,
 *                                             const mlib_s16 *src,
 *                                             void           *filter,
 *                                             mlib_s32       n);
 *
 *      void mlib_SignalFIRFree_S16S_S16S(void *filter);
 *      void mlib_SignalFIRFree_S16_S16(void *filter);
 *
 * ARGUMENTS
 *      filter   Internal filter structure
 *      flt      Filter coefficient array
 *      tap      Taps of the filter
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *            n-1
 *     Z(n) = SUM h(k) * x(n-k)   n = 1, 2, ...
 *            k = 0
 *
 *     Z     - output signal array
 *     h(k)  - filter coefficients
 *     x(i)  - input signal array
 */

#include <mlib_signal.h>
#include <mlib_s_SignalFIR.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalFIRFree_F32S_F32S = __mlib_SignalFIRFree_F32S_F32S
#pragma weak mlib_SignalFIRFree_F32_F32 = __mlib_SignalFIRFree_F32_F32
#pragma weak mlib_SignalFIRFree_S16S_S16S = __mlib_SignalFIRFree_S16S_S16S
#pragma weak mlib_SignalFIRFree_S16_S16 = __mlib_SignalFIRFree_S16_S16
#pragma weak mlib_SignalFIRInit_F32S_F32S = __mlib_SignalFIRInit_F32S_F32S
#pragma weak mlib_SignalFIRInit_F32_F32 = __mlib_SignalFIRInit_F32_F32
#pragma weak mlib_SignalFIRInit_S16S_S16S = __mlib_SignalFIRInit_S16S_S16S
#pragma weak mlib_SignalFIRInit_S16_S16 = __mlib_SignalFIRInit_S16_S16
#pragma weak mlib_SignalFIR_F32S_F32S = __mlib_SignalFIR_F32S_F32S
#pragma weak mlib_SignalFIR_F32_F32 = __mlib_SignalFIR_F32_F32
#pragma weak mlib_SignalFIR_S16S_S16S_Sat = __mlib_SignalFIR_S16S_S16S_Sat
#pragma weak mlib_SignalFIR_S16_S16_Sat = __mlib_SignalFIR_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalFIRFree_F32S_F32S) mlib_SignalFIRFree_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalFIRFree_F32S_F32S")));
__typeof__(__mlib_SignalFIRFree_F32_F32) mlib_SignalFIRFree_F32_F32
    __attribute__((weak, alias("__mlib_SignalFIRFree_F32_F32")));
__typeof__(__mlib_SignalFIRFree_S16S_S16S) mlib_SignalFIRFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalFIRFree_S16S_S16S")));
__typeof__(__mlib_SignalFIRFree_S16_S16) mlib_SignalFIRFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalFIRFree_S16_S16")));
__typeof__(__mlib_SignalFIRInit_F32S_F32S) mlib_SignalFIRInit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalFIRInit_F32S_F32S")));
__typeof__(__mlib_SignalFIRInit_F32_F32) mlib_SignalFIRInit_F32_F32
    __attribute__((weak, alias("__mlib_SignalFIRInit_F32_F32")));
__typeof__(__mlib_SignalFIRInit_S16S_S16S) mlib_SignalFIRInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalFIRInit_S16S_S16S")));
__typeof__(__mlib_SignalFIRInit_S16_S16) mlib_SignalFIRInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalFIRInit_S16_S16")));
__typeof__(__mlib_SignalFIR_F32S_F32S) mlib_SignalFIR_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalFIR_F32S_F32S")));
__typeof__(__mlib_SignalFIR_F32_F32) mlib_SignalFIR_F32_F32
    __attribute__((weak, alias("__mlib_SignalFIR_F32_F32")));
__typeof__(__mlib_SignalFIR_S16S_S16S_Sat) mlib_SignalFIR_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalFIR_S16S_S16S_Sat")));
__typeof__(__mlib_SignalFIR_S16_S16_Sat) mlib_SignalFIR_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalFIR_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalFIRInit_common(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_d64 scale,
    mlib_s32 stereo)
{
	mlib_s32 bufferSize = FIR_BUFFER_SIZE;
	mlib_s32 i, memSize;
	mlib_d64 *pdbl;
	mlib_fir_data *fir_data_ptr;
	mlib_s32 struct_size;

	*filter = NULL;

	if (tap <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	struct_size = (sizeof (mlib_fir_data) + 7) & ~7;

	memSize = struct_size;
	memSize +=
	    (tap + ((tap + 2 * bufferSize) << stereo)) * sizeof (mlib_d64);

	fir_data_ptr = (mlib_fir_data *) __mlib_malloc(memSize);

	if (fir_data_ptr == NULL) {
		return (MLIB_NULLPOINTER);
	}

	pdbl = (mlib_d64 *)((mlib_u8 *)fir_data_ptr + struct_size);

	fir_data_ptr->tap = tap;
	fir_data_ptr->bufferSize = bufferSize;
	fir_data_ptr->data = pdbl;

	*filter = fir_data_ptr;

	for (i = 0; i < tap; i++) {
		(*pdbl++) = flt[i] * scale;
	}

/* clear start of src buffer */
	for (i = 0; i < (tap << stereo); i++) {
		pdbl[i] = 0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFIRInit_S16_S16(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap)
{
	mlib_d64 scale = SAT_SCALE_S16;

	return (__mlib_SignalFIRInit_common(filter, flt, tap, scale, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFIRInit_S16S_S16S(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap)
{
	mlib_d64 scale = SAT_SCALE_S16;

	return (__mlib_SignalFIRInit_common(filter, flt, tap, scale, 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFIRInit_F32_F32(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap)
{
	mlib_d64 scale = 1.0;

	return (__mlib_SignalFIRInit_common(filter, flt, tap, scale, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFIRInit_F32S_F32S(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap)
{
	mlib_d64 scale = 1.0;

	return (__mlib_SignalFIRInit_common(filter, flt, tap, scale, 1));
}

/* *********************************************************** */

void mlib_FIR_coef_init_s16(mlib_d64 *pdbl,
	const mlib_f32 *flt,
	mlib_s32 stride,
	mlib_s32 tap)
{
	mlib_s32 i;
	mlib_d64 scale = SAT_SCALE_S16;

	for (i = 0; i < tap; i++) {
		(*pdbl++) = *flt * scale;
		flt += stride;
	}
}

void mlib_FIR_coef_init_f32(mlib_d64 *pdbl,
	const mlib_f32 *flt,
	mlib_s32 stride,
	mlib_s32 tap)
{
	mlib_s32 i;
	mlib_d64 scale = 1.0;

	for (i = 0; i < tap; i++) {
		(*pdbl++) = *flt * scale;
		flt += stride;
	}
}

/* *********************************************************** */

void
__mlib_SignalFIRFree_S16_S16(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalFIRFree_S16S_S16S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalFIRFree_F32_F32(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalFIRFree_F32S_F32S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

#define	FUNC64(TYPE, STEREO, FP)                                       \
	mlib_d64 *filterData;                                          \
	mlib_##FP *srcBuffer;                                          \
	mlib_##FP *dstBuffer;                                          \
	mlib_s32 i, k, off, count;                                     \
	mlib_s32 tap, bufferSize;                                      \
	mlib_fir_data *fir_data_ptr = (mlib_fir_data *) filter;        \
	                                                               \
	tap = fir_data_ptr->tap;                                       \
	bufferSize = fir_data_ptr->bufferSize;                         \
	if (tap <= 0)                                                  \
	    return (MLIB_FAILURE);                                     \
	if (n <= 0)                                                    \
	    return (MLIB_FAILURE);                                     \
	filterData = fir_data_ptr->data;                               \
	srcBuffer = (void *)(filterData + tap);                        \
	srcBuffer += tap;                                              \
	dstBuffer = srcBuffer + bufferSize;                            \
	for (off = 0; off < n; off += count) {                         \
	    mlib_##FP *sp = srcBuffer + 1;                             \
	    void *dp = dstBuffer;                                      \
	                                                               \
	    count = n - off;                                           \
	    if (count > bufferSize) {                                  \
		count = bufferSize;                                    \
	    }                                                          \
	    mlib_SignalFIR_##TYPE##_to_d64((void *)srcBuffer,          \
		(void *)src, count << STEREO);                         \
	    k = 0;                                                     \
	    if (tap == 1) {                                            \
		mlib_FIR_tap1f_##FP(dp, (void *)(sp - k - 1),          \
		    filterData + k, count);                            \
		k += 1;                                                \
	    } else if (tap == 2) {                                     \
		mlib_FIR_tap2f_##FP(dp, (void *)(sp - k - 2),          \
		    filterData + k, count);                            \
		k += 2;                                                \
	    } else if (tap & 1) {                                      \
		mlib_FIR_tap3f_##FP(dp, (void *)(sp - k - 3),          \
		    filterData + k, count);                            \
		k += 3;                                                \
	    } else {                                                   \
		mlib_FIR_tap4f_##FP(dp, (void *)(sp - k - 4),          \
		    filterData + k, count);                            \
		k += 4;                                                \
	    }                                                          \
	    while (k <= (tap - 4)) {                                   \
		mlib_FIR_tap4_##FP(dp, (void *)(sp - k - 4),           \
		    filterData + k, count);                            \
		k += 4;                                                \
	    }                                                          \
	    if (k < tap) {                                             \
		mlib_FIR_tap2_##FP(dp, (void *)(sp - k - 2),           \
		    filterData + k, count);                            \
	    }                                                          \
	    mlib_SignalFIR_d64_to_##TYPE(dst, (void *)dstBuffer,       \
		count << STEREO);                                      \
	    src += (count << STEREO);                                  \
	    dst += (count << STEREO);                                  \
	    for (i = 0; i < tap; i++) {                                \
		srcBuffer[i - tap] = srcBuffer[count + (i - tap)];     \
	    }                                                          \
	}                                                              \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_SignalFIR_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
/* SSE2 float */
	FUNC64(s16, 0, d64);
}

mlib_status
__mlib_SignalFIR_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
/* SSE2 float */
	FUNC64(s16, 1, d64s);
}

mlib_status
__mlib_SignalFIR_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *filter,
    mlib_s32 n)
{
/* SSE2 float */
	FUNC64(f32, 0, d64);
}

mlib_status
__mlib_SignalFIR_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *filter,
    mlib_s32 n)
{
	/* SSE2 float */
	FUNC64(f32, 1, d64s);
}

/* *********************************************************** */
