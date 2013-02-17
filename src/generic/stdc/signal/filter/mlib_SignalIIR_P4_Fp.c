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

#pragma ident	"@(#)mlib_SignalIIR_P4_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalIIRInit_P4_[F32|F32S]_[F32|F32S] - allocate the
 *           memory for internal filter structure and convert the
 *           filter coefficients into internal representation
 *
 *      mlib_SignalIIR_P4_[F32|F32S]_[F32|F32S] - apply the IIR filter
 *           one packet of signal and update the filter states
 *
 *      mlib_SignalIIRFree_P4_[F32|F32S]_[F32|F32S] - release the memory
 *           allocated for the internal filter structure
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalIIRInit_P4_F32S_F32S(void **filter,
 *                                                  const mlib_f32 *flt);
 *      mlib_status mlib_SignalIIRInit_P4_F32_F32(void **filter,
 *                                                const mlib_f32 *flt);
 *      mlib_status mlib_SignalIIR_P4_F32S_F32S(mlib_f32 *dst,
 *                                              const mlib_f32 *src,
 *                                              void *filter,
 *                                              mlib_s32 n);
 *      mlib_status mlib_SignalIIR_P4_F32_F32(mlib_f32 *dst,
 *                                            const mlib_f32 *src,
 *                                            void *filter,
 *                                            mlib_s32 n);
 *      void mlib_SignalIIRFree_P4_F32S_F32S(void *filter);
 *      void mlib_SignalIIRFree_P4_F32_F32(void *filter);
 *
 * ARGUMENTS
 *      filter   Internal filter structure
 *
 *      flt      Filter coefficient array
 *
 *      dst      Output signal array
 *
 *      src      Input signal array
 *
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *             N                   M
 *     Z(n) = SUM a(k) * x(n-k) + SUM b(k) * z(n - k)   n = 0, 1, 2, ...
 *            k = 0                 k = 1
 *
 *     Z     - output signal array
 *     a(k),
 *     b(k)  - filter coefficients
 *     x(i)  - input signal array
 *
 * ALGORITHM
 *
 *     z1(n) = a00 * x(n) + a10 * x(n - 1) +
 *             b10 * z1(n - 1) + b20 * z1(n - 2)
 *     z2(n) = a01 * x(n) + a11 * x(n - 1) +
 *             b11 * z2(n - 1) + b21 * z2(n - 2)
 *     z(n) = C * x(n) + z1(n) + z2(n)
 *
 * INTERNAL FILTER STRUCTURE (F32_F32)
 *
 *   filter.C   = C
 *   filter.a00 = a[0]
 *   filter.a01 = a[1]
 *   filter.a10 = a[2]
 *   filter.a11 = a[3]
 *   filter.a01 = a[4]
 *   filter.b10 = b[1]
 *   filter.b11 = b[2]
 *   filter.b20 = b[3]
 *   filter.b21 = b[4]
 *   filter.y11 = z1[1]
 *   filter.y12 = z1[2]
 *   filter.y21 = z2[1]
 *   filter.y22 = z2[2]
 *   filter.x1  = x[1]
 *
 * INTERNAL FILTER STRUCTURE (F32S_F32S)
 *
 *   filter.C   = C
 *   filter.a00 = a[0]
 *   filter.a01 = a[1]
 *   filter.a10 = a[2]
 *   filter.a11 = a[3]
 *   filter.a01 = a[4]
 *   filter.b10 = b[1]
 *   filter.b11 = b[2]
 *   filter.b20 = b[3]
 *   filter.b21 = b[4]
 *   filter.y11_0 = z1[1]
 *   filter.y12_0 = z1[2]
 *   filter.y21_0 = z1[3]
 *   filter.y22_0 = z1[4]
 *   filter.y11_1 = z2[1]
 *   filter.y12_1 = z2[2]
 *   filter.y21_1 = z2[3]
 *   filter.y22_1 = z2[4]
 *   filter.x1    = x1[1]
 *   filter.x2    = x2[1]
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalIIRFree_P4_F32S_F32S = \
	__mlib_SignalIIRFree_P4_F32S_F32S
#pragma weak mlib_SignalIIRFree_P4_F32_F32 = __mlib_SignalIIRFree_P4_F32_F32
#pragma weak mlib_SignalIIRInit_P4_F32S_F32S = \
	__mlib_SignalIIRInit_P4_F32S_F32S
#pragma weak mlib_SignalIIRInit_P4_F32_F32 = __mlib_SignalIIRInit_P4_F32_F32
#pragma weak mlib_SignalIIR_P4_F32S_F32S = __mlib_SignalIIR_P4_F32S_F32S
#pragma weak mlib_SignalIIR_P4_F32_F32 = __mlib_SignalIIR_P4_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalIIRFree_P4_F32S_F32S) mlib_SignalIIRFree_P4_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalIIRFree_P4_F32S_F32S")));
__typeof__(__mlib_SignalIIRFree_P4_F32_F32) mlib_SignalIIRFree_P4_F32_F32
    __attribute__((weak, alias("__mlib_SignalIIRFree_P4_F32_F32")));
__typeof__(__mlib_SignalIIRInit_P4_F32S_F32S) mlib_SignalIIRInit_P4_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalIIRInit_P4_F32S_F32S")));
__typeof__(__mlib_SignalIIRInit_P4_F32_F32) mlib_SignalIIRInit_P4_F32_F32
    __attribute__((weak, alias("__mlib_SignalIIRInit_P4_F32_F32")));
__typeof__(__mlib_SignalIIR_P4_F32S_F32S) mlib_SignalIIR_P4_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalIIR_P4_F32S_F32S")));
__typeof__(__mlib_SignalIIR_P4_F32_F32) mlib_SignalIIR_P4_F32_F32
    __attribute__((weak, alias("__mlib_SignalIIR_P4_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
/* coeff filter */
	mlib_d64 C;
	mlib_d64 a00;
	mlib_d64 a01;
	mlib_d64 a10;
	mlib_d64 a11;
	mlib_d64 b10;
	mlib_d64 b11;
	mlib_d64 b20;
	mlib_d64 b21;
/* delay element */
	mlib_d64 y10;
	mlib_d64 y20;
	mlib_d64 y11;
	mlib_d64 y21;
	mlib_d64 x1;
} mlib_IIR_filt_F32_P4;

/* *********************************************************** */

typedef struct
{
/* coeff filter */
	mlib_d64 C;
	mlib_d64 a00;
	mlib_d64 a01;
	mlib_d64 a10;
	mlib_d64 a11;
	mlib_d64 b10;
	mlib_d64 b11;
	mlib_d64 b20;
	mlib_d64 b21;
/* delay element */
	mlib_d64 y10_0;
	mlib_d64 y20_0;
	mlib_d64 y11_0;
	mlib_d64 y21_0;
	mlib_d64 y10_1;
	mlib_d64 y20_1;
	mlib_d64 y11_1;
	mlib_d64 y21_1;
	mlib_d64 x1_0;
	mlib_d64 x1_1;
	mlib_u8 flags;
} mlib_IIR_filt_F32S_P4;

/* *********************************************************** */

/* Mono signal */

/* *********************************************************** */

mlib_status
__mlib_SignalIIRInit_P4_F32_F32(
    void **filter,
    const mlib_f32 *flt)
{
	mlib_IIR_filt_F32_P4 *pflt;

	if (flt == NULL)
		return (MLIB_NULLPOINTER);

	*filter = (void *)__mlib_malloc(sizeof (mlib_IIR_filt_F32_P4));

	if (*filter == NULL)
		return (MLIB_NULLPOINTER);

	pflt = (mlib_IIR_filt_F32_P4 *) (*filter);

	pflt->C = flt[0];

	pflt->a00 = flt[1];
	pflt->a10 = flt[2];
	pflt->b10 = flt[3];
	pflt->b20 = flt[4];

	pflt->a01 = flt[5];
	pflt->a11 = flt[6];
	pflt->b11 = flt[7];
	pflt->b21 = flt[8];

	pflt->y10 = 0;
	pflt->y20 = 0;
	pflt->y11 = 0;
	pflt->y21 = 0;

	pflt->x1 = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIIR_P4_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_s32 i;

	mlib_d64 C, a00, a10, b10, b20, a01, a11, b11, b21;
	mlib_d64 y00, y10, y20, y01, y11, y21, x1, r0, x0;

	mlib_IIR_filt_F32_P4 *pflt = (mlib_IIR_filt_F32_P4 *) filter;

	if (filter == NULL || src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	C = pflt->C;

	a00 = pflt->a00;
	a10 = pflt->a10;

	a01 = pflt->a01;
	a11 = pflt->a11;

	b10 = pflt->b10;
	b20 = pflt->b20;

	b11 = pflt->b11;
	b21 = pflt->b21;

	y10 = pflt->y10;
	y20 = pflt->y20;
	y11 = pflt->y11;
	y21 = pflt->y21;

	x1 = pflt->x1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		x0 = src[i];

		y00 = a00 * x0 + a10 * x1 + b10 * y10 + b20 * y20;

		y01 = a01 * x0 + a11 * x1 + b11 * y11 + b21 * y21;

		r0 = C * x0 + y01 + y00;

		y20 = y10;
		y10 = y00;

		x1 = x0;

		y21 = y11;
		y11 = y01;

		dst[i] = r0;
	}

	pflt->y10 = y10;
	pflt->y20 = y20;
	pflt->y11 = y11;
	pflt->y21 = y21;

	pflt->x1 = x1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalIIRFree_P4_F32_F32(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

/* Stereo signal */

/* *********************************************************** */

mlib_status
__mlib_SignalIIRInit_P4_F32S_F32S(
    void **filter,
    const mlib_f32 *flt)
{
	mlib_IIR_filt_F32S_P4 *pflt;

	if (flt == NULL)
		return (MLIB_NULLPOINTER);

	*filter = (void *)__mlib_malloc(sizeof (mlib_IIR_filt_F32S_P4));

	if (*filter == NULL)
		return (MLIB_NULLPOINTER);

	pflt = (mlib_IIR_filt_F32S_P4 *) (*filter);

	pflt->C = flt[0];

	pflt->a00 = flt[1];
	pflt->a10 = flt[2];
	pflt->b10 = flt[3];
	pflt->b20 = flt[4];

	pflt->a01 = flt[5];
	pflt->a11 = flt[6];
	pflt->b11 = flt[7];
	pflt->b21 = flt[8];

	pflt->y10_0 = 0;
	pflt->y20_0 = 0;
	pflt->y11_0 = 0;
	pflt->y21_0 = 0;

	pflt->y10_1 = 0;
	pflt->y20_1 = 0;
	pflt->y11_1 = 0;
	pflt->y21_1 = 0;

	pflt->x1_0 = 0;
	pflt->x1_1 = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIIR_P4_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_s32 i;

	mlib_d64 C, a00, a10, b10, b20, a01, a11, b11, b21;
	mlib_d64 y00_0, y10_0, y20_0, y01_0, y11_0, y21_0;
	mlib_d64 x1_0, r0_0, x0_0, y00_1, y10_1, y20_1, y01_1;
	mlib_d64 y11_1, y21_1, x1_1, r0_1, x0_1;
	mlib_IIR_filt_F32S_P4 *pflt = (mlib_IIR_filt_F32S_P4 *) filter;

	if (filter == NULL || src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	C = pflt->C;

	a00 = pflt->a00;
	a10 = pflt->a10;

	a01 = pflt->a01;
	a11 = pflt->a11;

	b10 = pflt->b10;
	b20 = pflt->b20;

	b11 = pflt->b11;
	b21 = pflt->b21;

	y10_0 = pflt->y10_0;
	y20_0 = pflt->y20_0;
	y11_0 = pflt->y11_0;
	y21_0 = pflt->y21_0;

	x1_0 = pflt->x1_0;

	y10_1 = pflt->y10_1;
	y20_1 = pflt->y20_1;
	y11_1 = pflt->y11_1;
	y21_1 = pflt->y21_1;

	x1_1 = pflt->x1_1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		x0_0 = src[i * 2];

		y00_0 = a00 * x0_0 + a10 * x1_0 + b10 * y10_0 + b20 * y20_0;
		y01_0 = a01 * x0_0 + a11 * x1_0 + b11 * y11_0 + b21 * y21_0;
		r0_0 = C * x0_0 + y01_0 + y00_0;

		y20_0 = y10_0;
		y10_0 = y00_0;
		y21_0 = y11_0;
		y11_0 = y01_0;
		x1_0 = x0_0;

		dst[i * 2] = r0_0;

		x0_1 = src[2 * i + 1];

		y00_1 = a00 * x0_1 + a10 * x1_1 + b10 * y10_1 + b20 * y20_1;
		y01_1 = a01 * x0_1 + a11 * x1_1 + b11 * y11_1 + b21 * y21_1;
		r0_1 = C * x0_1 + y01_1 + y00_1;

		y20_1 = y10_1;
		y10_1 = y00_1;
		y21_1 = y11_1;
		y11_1 = y01_1;

		x1_1 = x0_1;

		dst[i * 2 + 1] = r0_1;
	}

	pflt->y10_0 = y10_0;
	pflt->y20_0 = y20_0;
	pflt->y11_0 = y11_0;
	pflt->y21_0 = y21_0;
	pflt->x1_0 = x1_0;

	pflt->y10_1 = y10_1;
	pflt->y20_1 = y20_1;
	pflt->y11_1 = y11_1;
	pflt->y21_1 = y21_1;
	pflt->x1_1 = x1_1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalIIRFree_P4_F32S_F32S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
