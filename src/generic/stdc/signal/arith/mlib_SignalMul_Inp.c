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

#pragma ident	"@(#)mlib_SignalMul_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMul* calls for inplace functions.
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSAdd_S16S_Sat = __mlib_SignalMulSAdd_S16S_Sat
#pragma weak mlib_SignalMulSAdd_S16_Sat = __mlib_SignalMulSAdd_S16_Sat
#pragma weak mlib_SignalMulSShiftAdd_S16S_Sat = \
	__mlib_SignalMulSShiftAdd_S16S_Sat
#pragma weak mlib_SignalMulSShiftAdd_S16_Sat = \
	__mlib_SignalMulSShiftAdd_S16_Sat
#pragma weak mlib_SignalMulSShift_S16S_Sat = __mlib_SignalMulSShift_S16S_Sat
#pragma weak mlib_SignalMulSShift_S16_Sat = __mlib_SignalMulSShift_S16_Sat
#pragma weak mlib_SignalMulS_S16S_Sat = __mlib_SignalMulS_S16S_Sat
#pragma weak mlib_SignalMulS_S16_Sat = __mlib_SignalMulS_S16_Sat
#pragma weak mlib_SignalMulShift_S16S_S16S_Sat = \
	__mlib_SignalMulShift_S16S_S16S_Sat
#pragma weak mlib_SignalMulShift_S16S_Sat = __mlib_SignalMulShift_S16S_Sat
#pragma weak mlib_SignalMulShift_S16_Sat = __mlib_SignalMulShift_S16_Sat
#pragma weak mlib_SignalMul_S16S_S16S_Sat = __mlib_SignalMul_S16S_S16S_Sat
#pragma weak mlib_SignalMul_S16S_Sat = __mlib_SignalMul_S16S_Sat
#pragma weak mlib_SignalMul_S16_Sat = __mlib_SignalMul_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSAdd_S16S_Sat) mlib_SignalMulSAdd_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16S_Sat")));
__typeof__(__mlib_SignalMulSAdd_S16_Sat) mlib_SignalMulSAdd_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16_Sat")));
__typeof__(__mlib_SignalMulSShiftAdd_S16S_Sat) mlib_SignalMulSShiftAdd_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShiftAdd_S16S_Sat")));
__typeof__(__mlib_SignalMulSShiftAdd_S16_Sat) mlib_SignalMulSShiftAdd_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShiftAdd_S16_Sat")));
__typeof__(__mlib_SignalMulSShift_S16S_Sat) mlib_SignalMulSShift_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShift_S16S_Sat")));
__typeof__(__mlib_SignalMulSShift_S16_Sat) mlib_SignalMulSShift_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShift_S16_Sat")));
__typeof__(__mlib_SignalMulS_S16S_Sat) mlib_SignalMulS_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16S_Sat")));
__typeof__(__mlib_SignalMulS_S16_Sat) mlib_SignalMulS_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16_Sat")));
__typeof__(__mlib_SignalMulShift_S16S_S16S_Sat)
    mlib_SignalMulShift_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulShift_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulShift_S16S_Sat) mlib_SignalMulShift_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulShift_S16S_Sat")));
__typeof__(__mlib_SignalMulShift_S16_Sat) mlib_SignalMulShift_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulShift_S16_Sat")));
__typeof__(__mlib_SignalMul_S16S_S16S_Sat) mlib_SignalMul_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMul_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMul_S16S_Sat) mlib_SignalMul_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMul_S16S_Sat")));
__typeof__(__mlib_SignalMul_S16_Sat) mlib_SignalMul_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMul_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalMul_S16_Sat(
    mlib_s16 *src1dst,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	return (__mlib_SignalMul_S16_S16_Sat(src1dst, src1dst, src2, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMul_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	return (__mlib_SignalMul_S16_S16_Sat(dst, src1, src2, 2 * n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMul_S16S_Sat(
    mlib_s16 *src1dst,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	return (__mlib_SignalMul_S16_S16_Sat(src1dst, src1dst, src2, 2 * n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16_Sat(
    mlib_s16 *data,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_SignalMulS_S16_S16_Sat(data, data, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16S_Sat(
    mlib_s16 *data,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_SignalMulS_S16S_S16S_Sat(data, data, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16_Sat(
    mlib_s16 *src1dst,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_SignalMulSAdd_S16_S16_Sat(src1dst, src1dst, src2, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16S_Sat(
    mlib_s16 *src1dst,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (
	    __mlib_SignalMulSAdd_S16S_S16S_Sat
	    (src1dst, src1dst, src2, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShift_S16_Sat(
    mlib_s16 *data,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (
	    __mlib_SignalMulSShift_S16_S16_Sat
	    (data, data, c, shift, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShift_S16S_Sat(
    mlib_s16 *data,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalMulSShift_S16S_S16S_Sat(data, data, c, shift, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShiftAdd_S16_Sat(
    mlib_s16 *data,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	return __mlib_SignalMulSShiftAdd_S16_S16_Sat(data, data, src2, c, shift,
	    n);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShiftAdd_S16S_Sat(
    mlib_s16 *data,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	return __mlib_SignalMulSShiftAdd_S16S_S16S_Sat(data, data, src2, c,
	    shift, n);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulShift_S16_Sat(
    mlib_s16 *data,
    const mlib_s16 *src2,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalMulShift_S16_S16_Sat(data, data, src2, shift, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulShift_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (
	    __mlib_SignalMulShift_S16_S16_Sat
	    (dst, src1, src2, shift, 2 * n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulShift_S16S_Sat(
    mlib_s16 *data,
    const mlib_s16 *src2,
    mlib_s32 shift,
    mlib_s32 n)
{
	return __mlib_SignalMulShift_S16_S16_Sat(data, data, src2, shift,
	    2 * n);
}

/* *********************************************************** */
