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

#pragma ident	"@(#)mlib_c_VectorConjRev.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConjRev_S8C_S8C_Sat   - computation of 8-bit format
 *                                         complex conjugated vector
 *      mlib_VectorConjRev_S16C_S16C_Sat - computation of 16-bit format
 *                                         complex conjugated vector
 *      mlib_VectorConjRev_S32C_S32C_Sat  - computation of 32-bit format
 *                                          complex conjugated vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConjRev_S8C_S8C_Sat(mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConjRev_S16C_S16C_Sat(mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorConjRev_S32C_S32C_Sat(mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *          *
 *      z = x
 */

/*
 * FUNCTIONS
 *      mlib_VectorConjSymExt_S8C_S8C_Sat - computation of complex conjugated
 *                                          extended vector having 8-bit format
 *      mlib_VectorConjSymExt_S16C_S16C_Sat - computation of complex conjugated
 *                                            extended vector having
 *                                            16-bit format
 *      mlib_VectorConjSymExt_S32C_S32C_Sat - computation of complex conjugated
 *                                            extended vector having
 *                                            32-bit format
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorConjSymExt_S8C_S8C_Sat(mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   mlib_s32      n);
 *     mlib_status mlib_VectorConjSymExt_S16C_S16C_Sat(mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     mlib_s32       n);
 *     mlib_status mlib_VectorConjSymExt_S32C_S32C_Sat(mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector,
 *           which has 2*n complex elements when n is iven, and 2*n - 1
 *           complex elements when n is odd. z[2*i] hold the real parts,
 *           and z[2*i + 1] hold the imaginary parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the input vector
 *
 * DESCRIPTION
 *            *    *        *    *      *        *
 *          ( x[1] x[2] ... x[n] x[n] x[n - 1] ... x[1] )  n - even
 *     z =    *    *        *      *        *
 *          ( x[1] x[2] ... x[n] x[n - 1] ... x[1] )       n - odd
 */

#include <mlib_algebra.h>
#include <mlib_VectorConjSymExt.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConjRev_S32C_S32C_Sat = \
	__mlib_VectorConjRev_S32C_S32C_Sat
#pragma weak mlib_VectorConjSymExt_S16C_S16C_Sat = \
	__mlib_VectorConjSymExt_S16C_S16C_Sat
#pragma weak mlib_VectorConjSymExt_S8C_S8C_Sat = \
	__mlib_VectorConjSymExt_S8C_S8C_Sat
#pragma weak mlib_VectorConjRev_S16C_S16C_Sat = \
	__mlib_VectorConjRev_S16C_S16C_Sat
#pragma weak mlib_VectorConjSymExt_S32C_S32C_Sat = \
	__mlib_VectorConjSymExt_S32C_S32C_Sat
#pragma weak mlib_VectorConjRev_S8C_S8C_Sat = \
	__mlib_VectorConjRev_S8C_S8C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConjRev_S32C_S32C_Sat) mlib_VectorConjRev_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConjRev_S32C_S32C_Sat")));
__typeof__(__mlib_VectorConjSymExt_S16C_S16C_Sat)
	mlib_VectorConjSymExt_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConjSymExt_S16C_S16C_Sat")));
__typeof__(__mlib_VectorConjSymExt_S8C_S8C_Sat)
	mlib_VectorConjSymExt_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConjSymExt_S8C_S8C_Sat")));
__typeof__(__mlib_VectorConjRev_S16C_S16C_Sat) mlib_VectorConjRev_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConjRev_S16C_S16C_Sat")));
__typeof__(__mlib_VectorConjSymExt_S32C_S32C_Sat)
	mlib_VectorConjSymExt_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConjSymExt_S32C_S32C_Sat")));
__typeof__(__mlib_VectorConjRev_S8C_S8C_Sat) mlib_VectorConjRev_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConjRev_S8C_S8C_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK                                                   \
	if ((xx == NULL) || (zz == NULL) || (n <= 0))           \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	CONJREVC(form, MAX, MIN)                                \
	{                                                       \
	    form *src = (form *) xx;                            \
	    form *dst = (form *) zz;                            \
	    mlib_s32 i;                                         \
	    form c;                                             \
	                                                        \
	    dst = &dst[2 * n];                                  \
	    for (i = 0; i < n * 2; i += 2) {                    \
		dst -= 2;                                       \
		dst[0] = src[i];                                \
		if ((c = src[i + 1]) == MIN)                    \
		    dst[1] = MAX;                               \
		else                                            \
		    dst[1] = -c;                                \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorConjRev_S8C_S8C_Sat(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 n)
{
	CHECK;
	CONJREVC(mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN)
}

/* *********************************************************** */

mlib_status
__mlib_VectorConjRev_S16C_S16C_Sat(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 n)
{
	CHECK;
	CONJREVC(mlib_s16,
		MLIB_S16_MAX,
		MLIB_S16_MIN)
}

/* *********************************************************** */

mlib_status
__mlib_VectorConjRev_S32C_S32C_Sat(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 n)
{
	const mlib_s32 *x = xx;
	mlib_s32 *z = zz;
	mlib_s32 *src = (mlib_s32 *)x, *dst = (mlib_s32 *)&z[2 * n];
	mlib_s32 i;
	mlib_s32 c;

	CHECK;

	for (i = 0; i < n * 2; i += 2) {
		dst -= 2;
		dst[0] = src[i];

		if ((c = src[i + 1]) == MLIB_S32_MIN)
			dst[1] = MLIB_S32_MAX;
		else
			dst[1] = -c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	CONJSYMEXTC(form, MAX, MIN)                       \
	{                                                       \
	    form *dst_end = z + n1 * 2, c, c1;                  \
	    mlib_s32 i;                                         \
	                                                        \
	    for (i = 0; i < n * 2; i += 2) {                    \
		z[i] = (c1 = x[i]);                             \
		if ((c = x[i + 1]) == MIN) {                    \
		    z[i + 1] = MAX;                             \
		    dst_end[-1] = MAX;                          \
		} else {                                        \
		    z[i + 1] = -c;                              \
		    dst_end[-1] = -c;                           \
		}                                               \
		dst_end[-2] = c1;                               \
		dst_end -= 2;                                   \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

mlib_status
mlib_VectorConjSymExt_S8C_S8C_Sat_N(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 n,
	mlib_s32 n1)
{
	const mlib_s8 *x = xx;
	mlib_s8 *z = zz;

	CHECK;
	CONJSYMEXTC(mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN)
}

/* *********************************************************** */

mlib_status
mlib_VectorConjSymExt_S16C_S16C_Sat_N(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 n,
	mlib_s32 n1)
{
	const mlib_s16 *x = xx;
	mlib_s16 *z = zz;

	CHECK;
	CONJSYMEXTC(mlib_s16,
		MLIB_S16_MAX,
		MLIB_S16_MIN)
}

/* *********************************************************** */

mlib_status
mlib_VectorConjSymExt_S32C_S32C_Sat_N(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 n,
	mlib_s32 n1)
{
	const mlib_s32 *x = xx;
	mlib_s32 *z = zz;

	CHECK;
	CONJSYMEXTC(mlib_s32,
		MLIB_S32_MAX,
		MLIB_S32_MIN)
}

/* *********************************************************** */

mlib_status __mlib_VectorConjSymExt_S8C_S8C_Sat(
    mlib_s8 *z,
    const mlib_s8 *x,
    mlib_s32 n)
{
	return mlib_VectorConjSymExt_S8C_S8C_Sat_N(z, x, n,
		(n * 2 - (n & 1)));
}

mlib_status __mlib_VectorConjSymExt_S16C_S16C_Sat(
    mlib_s16 *z,
    const mlib_s16 *x,
    mlib_s32 n)
{
	return mlib_VectorConjSymExt_S16C_S16C_Sat_N(z, x, n,
		(n * 2 - (n & 1)));
}

mlib_status __mlib_VectorConjSymExt_S32C_S32C_Sat(
    mlib_s32 *z,
    const mlib_s32 *x,
    mlib_s32 n)
{
	return mlib_VectorConjSymExt_S32C_S32C_Sat_N(z, x, n,
		(n * 2 - (n & 1)));
}

/* *********************************************************** */
