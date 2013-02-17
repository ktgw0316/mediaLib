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

#pragma ident	"@(#)mlib_c_VectorSub.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSub8  - subtraction of 8-bit format vectors
 *      mlib_VectorSub16 - subtraction of 16-bit format vectors
 *      mlib_VectorSub32 - subtraction of 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSub_U8_U8_[Sat|Mod](mlib_u8      *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8_S8_[Sat|Mod](mlib_s8      *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_S8_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_S16_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32_S16_[Sat|Mod](mlib_s32      *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32_S32_[Sat|Mod](mlib_s32      *z,
 *                                                  const mlib_s32 *x,
 *                                                  const mlib_s32 *y,
 *                                                  mlib_s32       n);
 *
 *      mlib_status mlib_VectorSub_U8_[Sat|Mod](mlib_u8       *xz,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8_[Sat|Mod](mlib_s8       *xz,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_[Sat|Mod](mlib_s16       *xz,
 *                                               const mlib_s16 *y,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32_[Sat|Mod](mlib_s32       *xz,
 *                                               const mlib_s32 *y,
 *                                               mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      xz   pointer to the first element of the first input and
 *           the output vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x - y
 */

/*
 * FUNCTIONS
 *      mlib_VectorSub8C  - subtraction of 8-bit format complex vectors
 *      mlib_VectorSub16C - subtraction of 16-bit format complex vectors
 *      mlib_VectorSub32C - subtraction of 32-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSub_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *y,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *y,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       n);
 *
 *      mlib_status mlib_VectorSub_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                const mlib_s16 *y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                const mlib_s32 *y,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      xz   pointer to the first element of the first input and
 *           the output vector
 *           zx[2*i] hold the real parts, and xz[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x - y
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSub_S16_Sat = __mlib_VectorSub_S16_Sat
#pragma weak mlib_VectorSub_S32C_Sat = __mlib_VectorSub_S32C_Sat
#pragma weak mlib_VectorSub_S8C_S8C_Sat = __mlib_VectorSub_S8C_S8C_Sat
#pragma weak mlib_VectorSub_S16_S8_Mod = __mlib_VectorSub_S16_S8_Mod
#pragma weak mlib_VectorSub_S16C_Sat = __mlib_VectorSub_S16C_Sat
#pragma weak mlib_VectorSub_S16_U8_Mod = __mlib_VectorSub_S16_U8_Mod
#pragma weak mlib_VectorSub_U8C_U8C_Sat = __mlib_VectorSub_U8C_U8C_Sat
#pragma weak mlib_VectorSub_S16C_S8C_Mod = __mlib_VectorSub_S16C_S8C_Mod
#pragma weak mlib_VectorSub_S8_Mod = __mlib_VectorSub_S8_Mod
#pragma weak mlib_VectorSub_S16C_U8C_Mod = __mlib_VectorSub_S16C_U8C_Mod
#pragma weak mlib_VectorSub_U8_Mod = __mlib_VectorSub_U8_Mod
#pragma weak mlib_VectorSub_S8C_Mod = __mlib_VectorSub_S8C_Mod
#pragma weak mlib_VectorSub_S32_S32_Mod = __mlib_VectorSub_S32_S32_Mod
#pragma weak mlib_VectorSub_U8C_Mod = __mlib_VectorSub_U8C_Mod
#pragma weak mlib_VectorSub_S32_S16_Mod = __mlib_VectorSub_S32_S16_Mod
#pragma weak mlib_VectorSub_S8_Sat = __mlib_VectorSub_S8_Sat
#pragma weak mlib_VectorSub_S16_S16_Mod = __mlib_VectorSub_S16_S16_Mod
#pragma weak mlib_VectorSub_S32C_S32C_Mod = __mlib_VectorSub_S32C_S32C_Mod
#pragma weak mlib_VectorSub_U8_Sat = __mlib_VectorSub_U8_Sat
#pragma weak mlib_VectorSub_S8_S8_Mod = __mlib_VectorSub_S8_S8_Mod
#pragma weak mlib_VectorSub_S8C_Sat = __mlib_VectorSub_S8C_Sat
#pragma weak mlib_VectorSub_S32C_S16C_Mod = __mlib_VectorSub_S32C_S16C_Mod
#pragma weak mlib_VectorSub_S32_S32_Sat = __mlib_VectorSub_S32_S32_Sat
#pragma weak mlib_VectorSub_U8C_Sat = __mlib_VectorSub_U8C_Sat
#pragma weak mlib_VectorSub_S32_Mod = __mlib_VectorSub_S32_Mod
#pragma weak mlib_VectorSub_S16C_S16C_Mod = __mlib_VectorSub_S16C_S16C_Mod
#pragma weak mlib_VectorSub_U8_U8_Mod = __mlib_VectorSub_U8_U8_Mod
#pragma weak mlib_VectorSub_S16_Mod = __mlib_VectorSub_S16_Mod
#pragma weak mlib_VectorSub_S32C_Mod = __mlib_VectorSub_S32C_Mod
#pragma weak mlib_VectorSub_S8C_S8C_Mod = __mlib_VectorSub_S8C_S8C_Mod
#pragma weak mlib_VectorSub_S16_S16_Sat = __mlib_VectorSub_S16_S16_Sat
#pragma weak mlib_VectorSub_S32C_S32C_Sat = __mlib_VectorSub_S32C_S32C_Sat
#pragma weak mlib_VectorSub_S16C_Mod = __mlib_VectorSub_S16C_Mod
#pragma weak mlib_VectorSub_S8_S8_Sat = __mlib_VectorSub_S8_S8_Sat
#pragma weak mlib_VectorSub_U8C_U8C_Mod = __mlib_VectorSub_U8C_U8C_Mod
#pragma weak mlib_VectorSub_S32_Sat = __mlib_VectorSub_S32_Sat
#pragma weak mlib_VectorSub_S16C_S16C_Sat = __mlib_VectorSub_S16C_S16C_Sat
#pragma weak mlib_VectorSub_U8_U8_Sat = __mlib_VectorSub_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSub_S16_Sat) mlib_VectorSub_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16_Sat")));
__typeof__(__mlib_VectorSub_S32C_Sat) mlib_VectorSub_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32C_Sat")));
__typeof__(__mlib_VectorSub_S8C_S8C_Sat) mlib_VectorSub_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8C_S8C_Sat")));
__typeof__(__mlib_VectorSub_S16_S8_Mod) mlib_VectorSub_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_S8_Mod")));
__typeof__(__mlib_VectorSub_S16C_Sat) mlib_VectorSub_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16C_Sat")));
__typeof__(__mlib_VectorSub_S16_U8_Mod) mlib_VectorSub_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_U8_Mod")));
__typeof__(__mlib_VectorSub_U8C_U8C_Sat) mlib_VectorSub_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8C_U8C_Sat")));
__typeof__(__mlib_VectorSub_S16C_S8C_Mod) mlib_VectorSub_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S8C_Mod")));
__typeof__(__mlib_VectorSub_S8_Mod) mlib_VectorSub_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8_Mod")));
__typeof__(__mlib_VectorSub_S16C_U8C_Mod) mlib_VectorSub_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_U8C_Mod")));
__typeof__(__mlib_VectorSub_U8_Mod) mlib_VectorSub_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8_Mod")));
__typeof__(__mlib_VectorSub_S8C_Mod) mlib_VectorSub_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8C_Mod")));
__typeof__(__mlib_VectorSub_S32_S32_Mod) mlib_VectorSub_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32_S32_Mod")));
__typeof__(__mlib_VectorSub_U8C_Mod) mlib_VectorSub_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8C_Mod")));
__typeof__(__mlib_VectorSub_S32_S16_Mod) mlib_VectorSub_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32_S16_Mod")));
__typeof__(__mlib_VectorSub_S8_Sat) mlib_VectorSub_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8_Sat")));
__typeof__(__mlib_VectorSub_S16_S16_Mod) mlib_VectorSub_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_S16_Mod")));
__typeof__(__mlib_VectorSub_S32C_S32C_Mod) mlib_VectorSub_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32C_S32C_Mod")));
__typeof__(__mlib_VectorSub_U8_Sat) mlib_VectorSub_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8_Sat")));
__typeof__(__mlib_VectorSub_S8_S8_Mod) mlib_VectorSub_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8_S8_Mod")));
__typeof__(__mlib_VectorSub_S8C_Sat) mlib_VectorSub_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8C_Sat")));
__typeof__(__mlib_VectorSub_S32C_S16C_Mod) mlib_VectorSub_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32C_S16C_Mod")));
__typeof__(__mlib_VectorSub_S32_S32_Sat) mlib_VectorSub_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32_S32_Sat")));
__typeof__(__mlib_VectorSub_U8C_Sat) mlib_VectorSub_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8C_Sat")));
__typeof__(__mlib_VectorSub_S32_Mod) mlib_VectorSub_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32_Mod")));
__typeof__(__mlib_VectorSub_S16C_S16C_Mod) mlib_VectorSub_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S16C_Mod")));
__typeof__(__mlib_VectorSub_U8_U8_Mod) mlib_VectorSub_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8_U8_Mod")));
__typeof__(__mlib_VectorSub_S16_Mod) mlib_VectorSub_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_Mod")));
__typeof__(__mlib_VectorSub_S32C_Mod) mlib_VectorSub_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32C_Mod")));
__typeof__(__mlib_VectorSub_S8C_S8C_Mod) mlib_VectorSub_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8C_S8C_Mod")));
__typeof__(__mlib_VectorSub_S16_S16_Sat) mlib_VectorSub_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16_S16_Sat")));
__typeof__(__mlib_VectorSub_S32C_S32C_Sat) mlib_VectorSub_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32C_S32C_Sat")));
__typeof__(__mlib_VectorSub_S16C_Mod) mlib_VectorSub_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_Mod")));
__typeof__(__mlib_VectorSub_S8_S8_Sat) mlib_VectorSub_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8_S8_Sat")));
__typeof__(__mlib_VectorSub_U8C_U8C_Mod) mlib_VectorSub_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8C_U8C_Mod")));
__typeof__(__mlib_VectorSub_S32_Sat) mlib_VectorSub_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32_Sat")));
__typeof__(__mlib_VectorSub_S16C_S16C_Sat) mlib_VectorSub_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S16C_Sat")));
__typeof__(__mlib_VectorSub_U8_U8_Sat) mlib_VectorSub_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SUB_MOD0(form)                                                   \
	{                                                                \
	    mlib_s32 i;                                                  \
	    form *px = (form *) x, *py = (form *) y, *pz = (form *) z;

/* *********************************************************** */

#define	SUB_MOD1                                                \
	    for (i = 0; i < n; i++)                             \
		pz[i] = px[i] - py[i];                          \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 diff;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {
		diff = px[i] - (mlib_s32)(py[i]);
		pz[i] = diff & ~(diff >> 8);
		i++;
		diff = px[i] - (mlib_s32)(py[i]);
		pz[i] = diff & ~(diff >> 8);
	}

	if (i < n) {
		diff = px[i] - (mlib_s32)(py[i]);
		pz[i] = diff & ~(diff >> 8);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 *spx, *spy, *spz;
	mlib_s32 sr1, sr2, sr3;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 alen;
	mlib_s32 alens;
	mlib_s32 fd;
	mlib_s32 j;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 3) == 0) {
		spx = (mlib_s32 *)px;
		spy = (mlib_s32 *)py;
		spz = (mlib_s32 *)pz;
		alen = n >> 2;
		alens = alen << 2;
		fd = n & 3;

		if (j = alen & 1) {
			sr1 = spx[0] ^ ~spy[0];
			sr2 = (spx[0] | ~mask) - (spy[0] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[0] = sr3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j < alen; j += 2) {
			sr1 = spx[j] ^ ~spy[j];
			sr2 = (spx[j] | ~mask) - (spy[j] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[j] = sr3;
			sr1 = spx[j + 1] ^ ~spy[j + 1];
			sr2 = (spx[j + 1] | ~mask) - (spy[j + 1] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[j + 1] = sr3;
		}

		if (fd > 2)
			pz[alens + 2] = px[alens + 2] - py[alens + 2];

		if (fd > 1)
			pz[alens + 1] = px[alens + 1] - py[alens + 1];

		if (fd > 0)
			pz[alens] = px[alens] - py[alens];
		return (MLIB_SUCCESS);
	} else
		SUB_MOD0(mlib_u8);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		SUB_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s16 *pz = z;
	const mlib_u8 *px = x;
	const mlib_u8 *py = y;
	mlib_s32 i;

	for (i = 0; i < n; i++)
		pz[i] = (mlib_s32)px[i] - py[i];
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s16 *pz = z;
	const mlib_s8 *px = x;
	const mlib_s8 *py = y;
	mlib_s32 i;

	for (i = 0; i < n; i++)
		pz[i] = (mlib_s32)px[i] - py[i];
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 diff, imask;
	mlib_u32 mask1 = 0xff0000ff;
	mlib_u32 mask2 = 0x00807f00;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = px[i] - (mlib_s32)(py[i]);
		imask = (diff >> 4) & 0x18;
		pz[i] = (diff & (mask1 >> imask)) | (mask2 >> imask);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	SUB_MOD0(mlib_s8);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUB_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 diff, imask;
	mlib_u32 mask32[] = { 0x0000ffff, 0x7fff0000, 0x80000000, 0x0000ffff };
	mlib_u8 *pmask = (mlib_u8 *)mask32;
	mlib_u32 mask;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {
		diff = px[i] - (mlib_s32)(py[i]);
		imask = (diff >> 13) & 0xc;
		mask = *((mlib_u32 *)(pmask + imask));
		pz[i] = (diff & mask) | (mask >> 16);
		i++;
		diff = px[i] - (mlib_s32)(py[i]);
		imask = (diff >> 13) & 0xc;
		mask = *((mlib_u32 *)(pmask + imask));
		pz[i] = (diff & mask) | (mask >> 16);
	}

	if (i < n) {
		diff = px[i] - (mlib_s32)(py[i]);
		imask = (diff >> 13) & 0xc;
		mask = *((mlib_u32 *)(pmask + imask));
		pz[i] = (diff & mask) | (mask >> 16);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	SUB_MOD0(mlib_s16);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUB_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 *pz = z;
	const mlib_s16 *px = x;
	const mlib_s16 *py = y;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++)
		pz[i] = (mlib_s32)px[i] - py[i];
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 i;
	mlib_d64 diff;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = px[i] - (mlib_d64)(py[i]);

		if (diff > MLIB_S32_MAX)
			diff = MLIB_S32_MAX;
		else if (diff < MLIB_S32_MIN)
			diff = MLIB_S32_MIN;
		pz[i] = (mlib_s32)diff;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */

	mlib_s32 i;
	mlib_d64 diff;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = px[i] - (mlib_d64)(py[i]);
		pz[i] = (mlib_s32)diff;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	SUB_MOD0(mlib_s32);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUB_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Mod(xz, xz, y, n));
}

/* *********************************************************** */

#define	SUBC_MOD0(form)                                                  \
	{                                                                \
	    mlib_s32 i;                                                  \
	    form *px = (form *) x, *py = (form *) y, *pz = (form *) z;

/* *********************************************************** */

#define	SUBC_MOD1                                                    \
	    for (i = 0; i < n; i++) {                                \
		pz[2 * i] = px[2 * i] - py[2 * i], pz[2 * i + 1] =   \
			px[2 * i + 1] - py[2 * i + 1];               \
	    }                                                        \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);          \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8C_U8C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_U8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8C_U8C_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 diff_r, diff_i;
	mlib_s32 imask_r, imask_i;
	mlib_u32 mask1 = 0xff0000ff;
	mlib_u32 mask2 = 0x00807f00;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff_r = px[2 * i] - (mlib_s32)(py[2 * i]);
		diff_i = px[2 * i + 1] - (mlib_s32)(py[2 * i + 1]);
		imask_r = (diff_r >> 4) & 0x18;
		pz[2 * i] = (diff_r & (mask1 >> imask_r)) | (mask2 >> imask_r);
		imask_i = (diff_i >> 4) & 0x18;
		pz[2 * i + 1] =
			(diff_i & (mask1 >> imask_i)) | (mask2 >> imask_i);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8C_S8C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	SUBC_MOD0(mlib_s8);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBC_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8C_S8C_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16C_S16C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	SUBC_MOD0(mlib_s16);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBC_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S16_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16C_S16C_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 i;
	mlib_d64 diff_r, diff_i;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

	for (i = 0; i < n; i++) {
		diff_r = px[2 * i] - (mlib_d64)py[2 * i];
		diff_i = px[2 * i + 1] - (mlib_d64)py[2 * i + 1];

		if (diff_r > MLIB_S32_MAX)
			diff_r = MLIB_S32_MAX;
		else if (diff_r < MLIB_S32_MIN)
			diff_r = MLIB_S32_MIN;
		if (diff_i > MLIB_S32_MAX)
			diff_i = MLIB_S32_MAX;
		else if (diff_i < MLIB_S32_MIN)
			diff_i = MLIB_S32_MIN;
		pz[2 * i] = (mlib_s32)diff_r;
		pz[2 * i + 1] = (mlib_s32)diff_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */

	mlib_s32 i;
	mlib_d64 diff_r, diff_i;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

	for (i = 0; i < n; i++) {
		diff_r = px[2 * i] - (mlib_d64)py[2 * i];
		diff_i = px[2 * i + 1] - (mlib_d64)py[2 * i + 1];
		pz[2 * i] = (mlib_s32)diff_r;
		pz[2 * i + 1] = (mlib_s32)diff_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	SUBC_MOD0(mlib_s32);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBC_MOD1
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */
