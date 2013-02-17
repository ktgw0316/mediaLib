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

#pragma ident	"@(#)mlib_i_VectorSubS.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8 - subtraction of unsigned 8-bit format vector
 *                           from a scalar
 *      mlib_VectorSubS_S8 - subtraction of signed 8-bit format vector
 *                           from a scalar
 *
 *      mlib_VectorSubS_S16 - subtraction of signed 16-bit format vector
 *                            from a scalar
 *      mlib_VectorSubS_S32 - subtraction of signed 32-bit format vector
 *                            from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16_S8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32_S16_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *c,
 *                                                    mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      c    pointer to a scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8C - subtraction of unsigned 8-bit format
 *                            complex vector from a scalar
 *      mlib_VectorSubS_S8C - subtraction of signed 8-bit format
 *                            complex vector from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                     const mlib_u8 *x,
 *                                                     const mlib_u8 *c,
 *                                                     mlib_s32      n);
 *
 *      mlib_status mlib_VectorSubS_S8C_S8C_[Sat|Mod](mlib_u8       *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                     const mlib_s8 *x,
 *                                                     const mlib_s8 *c,
 *                                                     mlib_s32      n);
 *
 *      mlib_status mlib_VectorSubS_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8 - subtraction of unsigned 8-bit format vector
 *                           to a scalar
 *      mlib_VectorSubS_S8 - subtraction of signed 8-bit format vector
 *                           to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S8_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *
 * ARGUMENTS
 *      xz    pointer to the first element of the input and the output vector
 *      c     pointer to a scalar
 *      n     number of elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8C - subtraction of unsigned 8-bit format
 *                            complex vector from a scalar
 *      mlib_VectorSubS_S8C - subtraction of signed 8-bit format
 *                            complex vector from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                const mlib_u8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                const mlib_s8 *c,
 *                                                mlib_s32      n);
 *
 * ARGUMENTS
 *      xz    pointer to the first complex element of the input and
 *            the output vector.
 *            x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *            parts
 *      c     pointer to a complex scalar. c[0] hold the real part,
 *            and c[1] hold the imaginary part
 *      n     number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSubS_S32_S32_Mod = __mlib_VectorSubS_S32_S32_Mod
#pragma weak mlib_VectorSubS_S32_S16_Mod = __mlib_VectorSubS_S32_S16_Mod
#pragma weak mlib_VectorSubS_S16_S16_Mod = __mlib_VectorSubS_S16_S16_Mod
#pragma weak mlib_VectorSubS_S8_Sat = __mlib_VectorSubS_S8_Sat
#pragma weak mlib_VectorSubS_S32C_S32C_Mod = __mlib_VectorSubS_S32C_S32C_Mod
#pragma weak mlib_VectorSubS_U8_Sat = __mlib_VectorSubS_U8_Sat
#pragma weak mlib_VectorSubS_S32C_S16C_Mod = __mlib_VectorSubS_S32C_S16C_Mod
#pragma weak mlib_VectorSubS_S32_S32_Sat = __mlib_VectorSubS_S32_S32_Sat
#pragma weak mlib_VectorSubS_S16C_S16C_Mod = __mlib_VectorSubS_S16C_S16C_Mod
#pragma weak mlib_VectorSubS_S16_S16_Sat = __mlib_VectorSubS_S16_S16_Sat
#pragma weak mlib_VectorSubS_S32C_S32C_Sat = __mlib_VectorSubS_S32C_S32C_Sat
#pragma weak mlib_VectorSubS_S16C_S16C_Sat = __mlib_VectorSubS_S16C_S16C_Sat
#pragma weak mlib_VectorSubS_S8C_Mod = __mlib_VectorSubS_S8C_Mod
#pragma weak mlib_VectorSubS_U8C_Mod = __mlib_VectorSubS_U8C_Mod
#pragma weak mlib_VectorSubS_S8_S8_Mod = __mlib_VectorSubS_S8_S8_Mod
#pragma weak mlib_VectorSubS_S8C_Sat = __mlib_VectorSubS_S8C_Sat
#pragma weak mlib_VectorSubS_S32_Mod = __mlib_VectorSubS_S32_Mod
#pragma weak mlib_VectorSubS_U8C_Sat = __mlib_VectorSubS_U8C_Sat
#pragma weak mlib_VectorSubS_U8_U8_Mod = __mlib_VectorSubS_U8_U8_Mod
#pragma weak mlib_VectorSubS_S16_Mod = __mlib_VectorSubS_S16_Mod
#pragma weak mlib_VectorSubS_S32C_Mod = __mlib_VectorSubS_S32C_Mod
#pragma weak mlib_VectorSubS_S8C_S8C_Mod = __mlib_VectorSubS_S8C_S8C_Mod
#pragma weak mlib_VectorSubS_S16C_Mod = __mlib_VectorSubS_S16C_Mod
#pragma weak mlib_VectorSubS_S8_S8_Sat = __mlib_VectorSubS_S8_S8_Sat
#pragma weak mlib_VectorSubS_U8C_U8C_Mod = __mlib_VectorSubS_U8C_U8C_Mod
#pragma weak mlib_VectorSubS_S32_Sat = __mlib_VectorSubS_S32_Sat
#pragma weak mlib_VectorSubS_U8_U8_Sat = __mlib_VectorSubS_U8_U8_Sat
#pragma weak mlib_VectorSubS_S16_Sat = __mlib_VectorSubS_S16_Sat
#pragma weak mlib_VectorSubS_S32C_Sat = __mlib_VectorSubS_S32C_Sat
#pragma weak mlib_VectorSubS_S8C_S8C_Sat = __mlib_VectorSubS_S8C_S8C_Sat
#pragma weak mlib_VectorSubS_S16_S8_Mod = __mlib_VectorSubS_S16_S8_Mod
#pragma weak mlib_VectorSubS_S16C_Sat = __mlib_VectorSubS_S16C_Sat
#pragma weak mlib_VectorSubS_S16_U8_Mod = __mlib_VectorSubS_S16_U8_Mod
#pragma weak mlib_VectorSubS_U8C_U8C_Sat = __mlib_VectorSubS_U8C_U8C_Sat
#pragma weak mlib_VectorSubS_S16C_S8C_Mod = __mlib_VectorSubS_S16C_S8C_Mod
#pragma weak mlib_VectorSubS_S8_Mod = __mlib_VectorSubS_S8_Mod
#pragma weak mlib_VectorSubS_S16C_U8C_Mod = __mlib_VectorSubS_S16C_U8C_Mod
#pragma weak mlib_VectorSubS_U8_Mod = __mlib_VectorSubS_U8_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSubS_S32_S32_Mod) mlib_VectorSubS_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S32_Mod")));
__typeof__(__mlib_VectorSubS_S32_S16_Mod) mlib_VectorSubS_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S16_Mod")));
__typeof__(__mlib_VectorSubS_S16_S16_Mod) mlib_VectorSubS_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_S16_Mod")));
__typeof__(__mlib_VectorSubS_S8_Sat) mlib_VectorSubS_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S32C_Mod) mlib_VectorSubS_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S32C_Mod")));
__typeof__(__mlib_VectorSubS_U8_Sat) mlib_VectorSubS_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S16C_Mod) mlib_VectorSubS_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S16C_Mod")));
__typeof__(__mlib_VectorSubS_S32_S32_Sat) mlib_VectorSubS_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S32_Sat")));
__typeof__(__mlib_VectorSubS_S16C_S16C_Mod) mlib_VectorSubS_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_S16C_Mod")));
__typeof__(__mlib_VectorSubS_S16_S16_Sat) mlib_VectorSubS_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16_S16_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S32C_Sat) mlib_VectorSubS_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S32C_Sat")));
__typeof__(__mlib_VectorSubS_S16C_S16C_Sat) mlib_VectorSubS_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_S16C_Sat")));
__typeof__(__mlib_VectorSubS_S8C_Mod) mlib_VectorSubS_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_Mod")));
__typeof__(__mlib_VectorSubS_U8C_Mod) mlib_VectorSubS_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_Mod")));
__typeof__(__mlib_VectorSubS_S8_S8_Mod) mlib_VectorSubS_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8_S8_Mod")));
__typeof__(__mlib_VectorSubS_S8C_Sat) mlib_VectorSubS_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_Sat")));
__typeof__(__mlib_VectorSubS_S32_Mod) mlib_VectorSubS_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_Mod")));
__typeof__(__mlib_VectorSubS_U8C_Sat) mlib_VectorSubS_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_Sat")));
__typeof__(__mlib_VectorSubS_U8_U8_Mod) mlib_VectorSubS_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8_U8_Mod")));
__typeof__(__mlib_VectorSubS_S16_Mod) mlib_VectorSubS_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_Mod")));
__typeof__(__mlib_VectorSubS_S32C_Mod) mlib_VectorSubS_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_Mod")));
__typeof__(__mlib_VectorSubS_S8C_S8C_Mod) mlib_VectorSubS_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_S8C_Mod")));
__typeof__(__mlib_VectorSubS_S16C_Mod) mlib_VectorSubS_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_Mod")));
__typeof__(__mlib_VectorSubS_S8_S8_Sat) mlib_VectorSubS_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8_S8_Sat")));
__typeof__(__mlib_VectorSubS_U8C_U8C_Mod) mlib_VectorSubS_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_U8C_Mod")));
__typeof__(__mlib_VectorSubS_S32_Sat) mlib_VectorSubS_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32_Sat")));
__typeof__(__mlib_VectorSubS_U8_U8_Sat) mlib_VectorSubS_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8_U8_Sat")));
__typeof__(__mlib_VectorSubS_S16_Sat) mlib_VectorSubS_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16_Sat")));
__typeof__(__mlib_VectorSubS_S32C_Sat) mlib_VectorSubS_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_Sat")));
__typeof__(__mlib_VectorSubS_S8C_S8C_Sat) mlib_VectorSubS_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_S8C_Sat")));
__typeof__(__mlib_VectorSubS_S16_S8_Mod) mlib_VectorSubS_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_S8_Mod")));
__typeof__(__mlib_VectorSubS_S16C_Sat) mlib_VectorSubS_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_Sat")));
__typeof__(__mlib_VectorSubS_S16_U8_Mod) mlib_VectorSubS_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_U8_Mod")));
__typeof__(__mlib_VectorSubS_U8C_U8C_Sat) mlib_VectorSubS_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_U8C_Sat")));
__typeof__(__mlib_VectorSubS_S16C_S8C_Mod) mlib_VectorSubS_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_S8C_Mod")));
__typeof__(__mlib_VectorSubS_S8_Mod) mlib_VectorSubS_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8_Mod")));
__typeof__(__mlib_VectorSubS_S16C_U8C_Mod) mlib_VectorSubS_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_U8C_Mod")));
__typeof__(__mlib_VectorSubS_U8_Mod) mlib_VectorSubS_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SUBS_MOD                                                \
	for (i = 0; i < n; i++)                                 \
	    pz[i] = cc - px[i];                                 \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, cc = *((mlib_u8 *)c);
	mlib_u8 *pz = (mlib_u8 *)z;
	mlib_s32 i, diff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {
		diff = cc - (mlib_s32)px[i];
		pz[i] = diff & ~(diff >> 8);
		i++;
		diff = cc - (mlib_s32)px[i];
		pz[i] = diff & ~(diff >> 8);
	}

	if (i < n) {
		diff = cc - (mlib_s32)px[i];
		pz[i] = diff & ~(diff >> 8);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, cc = *((mlib_u8 *)c);
	mlib_u8 *pz = (mlib_u8 *)z;
	mlib_s32 *spx, *spz;
	mlib_s32 scal = cc << 24 | cc << 16 | cc << 8 | cc;
	mlib_s32 sr1, sr2, sr3;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 alen;
	mlib_s32 alens;
	mlib_s32 fd;
	mlib_s32 i, j;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	if ((((mlib_addr)x | (mlib_addr)z) & 3) == 0) {
		spx = (mlib_s32 *)px;
		spz = (mlib_s32 *)pz;
		alen = n >> 2;
		alens = alen << 2;
		fd = n & 3;

		if (j = alen & 1) {
			sr1 = spx[0] ^ ~scal;
			sr2 = (scal | ~mask) - (spx[0] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[0] = sr3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j < alen; j += 2) {
			sr1 = spx[j] ^ ~scal;
			sr2 = (scal | ~mask) - (spx[j] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[j] = sr3;
			sr1 = spx[j + 1] ^ ~scal;
			sr2 = (scal | ~mask) - (spx[j + 1] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[j + 1] = sr3;
		}

		if (fd > 2)
			pz[alens + 2] = cc - px[alens + 2];

		if (fd > 1)
			pz[alens + 1] = cc - px[alens + 1];

		if (fd > 0)
			pz[alens] = cc - px[alens];
		return (MLIB_SUCCESS);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		SUBS_MOD;
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, cc = *((mlib_u8 *)c);
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

	for (i = 0; i < n; i++)
		(*pz++) = cc - (*px++);
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8_U8_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8_U8_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, cc = *((mlib_s8 *)c);
	mlib_s8 *pz = (mlib_s8 *)z;
	mlib_s32 i;
	mlib_s32 diff, imask;
	mlib_u32 mask1 = 0xff0000ff;
	mlib_u32 mask2 = 0x00807f00;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = cc - (mlib_s32)px[i];
		imask = (diff >> 4) & 0x18;
		pz[i] = (diff & (mask1 >> imask)) | (mask2 >> imask);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, cc = *((mlib_s8 *)c);
	mlib_s8 *pz = (mlib_s8 *)z;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBS_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, cc = *((mlib_s8 *)c);
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

	for (i = 0; i < n; i++)
		(*pz++) = cc - (*px++);
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8_S8_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8_S8_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, cc = *((mlib_s16 *)c);
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;
	mlib_s32 diff, imask;
	mlib_u32 mask32[] = { 0x0000ffff, 0x7fff0000, 0x80000000, 0x0000ffff };
	mlib_u8 *pmask = (mlib_u8 *)mask32;
	mlib_u32 mask;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {
		diff = cc - (mlib_s32)px[i];
		imask = (diff >> 13) & 0xc;
		mask = *((mlib_u32 *)(pmask + imask));
		pz[i] = (diff & mask) | (mask >> 16);
		i++;
		diff = cc - (mlib_s32)px[i];
		imask = (diff >> 13) & 0xc;
		mask = *((mlib_u32 *)(pmask + imask));
		pz[i] = (diff & mask) | (mask >> 16);
	}

	if (i < n) {
		diff = cc - (mlib_s32)px[i];
		imask = (diff >> 13) & 0xc;
		mask = *((mlib_u32 *)(pmask + imask));
		pz[i] = (diff & mask) | (mask >> 16);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, cc = *((mlib_s16 *)c);
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	SUBS_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, cc = *((mlib_s16 *)c);
	mlib_s32 *pz = (mlib_s32 *)z, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	SUBS_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16_S16_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16_S16_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, cc = *((mlib_s32 *)c);
	mlib_s32 *pz = (mlib_s32 *)z;

#ifdef	_NO_LONGLONG

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 i;
	mlib_d64 diff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = cc - (mlib_d64)px[i];

		if (diff > MLIB_S32_MAX)
			diff = MLIB_S32_MAX;
		else if (diff < MLIB_S32_MIN)
			diff = MLIB_S32_MIN;
		pz[i] = diff;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 diff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = cc - (mlib_d64)px[i];
		pz[i] = diff;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */

#else	/* _NO_LONGLONG */

	mlib_s32 i;
	mlib_s64 diff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff = cc - (mlib_s64)px[i];

		if (diff > MLIB_S32_MAX)
			diff = MLIB_S32_MAX;
		else if (diff < MLIB_S32_MIN)
			diff = MLIB_S32_MIN;
		pz[i] = diff;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif	/* _NO_LONGLONG */
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, cc = *((mlib_s32 *)c);
	mlib_s32 *pz = (mlib_s32 *)z, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	SUBS_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32_S32_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32_S32_Mod(xz, xz, c, n));
}

/* *********************************************************** */

#define	SUBSC_MOD                                               \
	for (i = 0; i < n; i++) {                               \
	    pz[2 * i] = c0 - px[2 * i], pz[2 * i + 1] =         \
		    c1 - px[2 * i + 1];                         \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, c0 = ((mlib_u8 *)c)[0];
	mlib_u8 c1 = ((mlib_u8 *)c)[1], *pz = (mlib_u8 *)z;
	mlib_s32 i;
	mlib_s32 diff_r, diff_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_s32)px[2 * i];
		diff_i = c1 - (mlib_s32)px[2 * i + 1];
		pz[2 * i] = diff_r & ~(diff_r >> 8);
		pz[2 * i + 1] = diff_i & ~(diff_i >> 8);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, c0 = ((mlib_u8 *)c)[0];
	mlib_u8 c1 = ((mlib_u8 *)c)[1], *pz = (mlib_u8 *)z;
	mlib_s32 *spx, *spz;

#ifdef _LITTLE_ENDIAN
	mlib_s32 scal = c1 << 24 | c0 << 16 | c1 << 8 | c0;
#else /* _LITTLE_ENDIAN */
	mlib_s32 scal = c0 << 24 | c1 << 16 | c0 << 8 | c1;
#endif /* _LITTLE_ENDIAN */
	mlib_s32 sr1, sr2, sr3;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 alen;
	mlib_s32 alens;
	mlib_s32 fd;
	mlib_s32 i, j, nn = 2 * n;

	if ((((mlib_addr)x | (mlib_addr)z) & 3) == 0) {
		if (n <= 0)
			return (MLIB_FAILURE);
		spx = (mlib_s32 *)px;
		spz = (mlib_s32 *)pz;
		alen = nn >> 2;
		alens = alen << 2;
		fd = nn & 3;

		if (j = alen & 1) {
			sr1 = spx[0] ^ ~scal;
			sr2 = (scal | ~mask) - (spx[0] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[0] = sr3;
		}

		for (; j < alen; j += 2) {
			sr1 = spx[j] ^ ~scal;
			sr2 = (scal | ~mask) - (spx[j] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[j] = sr3;
			sr1 = spx[j + 1] ^ ~scal;
			sr2 = (scal | ~mask) - (spx[j + 1] & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			spz[j + 1] = sr3;
		}

		if (fd > 0) {
			pz[alens] = c0 - px[alens];
			pz[alens + 1] = c1 - px[alens + 1];
		}

		return (MLIB_SUCCESS);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		SUBSC_MOD;
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, cc = *((mlib_u8 *)c);
	mlib_u8 cc1 = *((mlib_u8 *)c + 1);
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		(*pz++) = cc - (*px++);
		(*pz++) = cc1 - (*px++);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8C_U8C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8C_U8C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, c0 = ((mlib_s8 *)c)[0];
	mlib_s8 c1 = ((mlib_s8 *)c)[1], *pz = (mlib_s8 *)z;
	mlib_s32 i;
	mlib_s32 diff_r, diff_i;
	mlib_s32 imask_r, imask_i;
	mlib_u32 mask1 = 0xff0000ff;
	mlib_u32 mask2 = 0x00807f00;

	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_s32)px[2 * i];
		diff_i = c1 - (mlib_s32)px[2 * i + 1];
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
__mlib_VectorSubS_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, c0 = ((mlib_s8 *)c)[0];
	mlib_s8 c1 = ((mlib_s8 *)c)[1], *pz = (mlib_s8 *)z;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	SUBSC_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, cc = *((mlib_s8 *)c);
	mlib_s8 cc1 = *((mlib_s8 *)c + 1);
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		(*pz++) = cc - (*px++);
		(*pz++) = cc1 - (*px++);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8C_S8C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8C_S8C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, c0 = ((mlib_s16 *)c)[0];
	mlib_s16 c1 = ((mlib_s16 *)c)[1], *pz = (mlib_s16 *)z;
	mlib_s32 i;
	mlib_s32 diff_r, diff_i;
	mlib_s32 imask_r, imask_i;
	mlib_u32 mask32[] = { 0x0000ffff, 0x7fff0000, 0x80000000, 0x0000ffff };
	mlib_u8 *pmask = (mlib_u8 *)mask32;
	mlib_u32 mask_r, mask_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_s32)px[2 * i];
		diff_i = c1 - (mlib_s32)px[2 * i + 1];
		imask_r = (diff_r >> 13) & 0xc;
		mask_r = *((mlib_u32 *)(pmask + imask_r));
		pz[2 * i] = (diff_r & mask_r) | (mask_r >> 16);
		imask_i = (diff_i >> 13) & 0xc;
		mask_i = *((mlib_u32 *)(pmask + imask_i));
		pz[2 * i + 1] = (diff_i & mask_i) | (mask_i >> 16);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, c0 = ((mlib_s16 *)c)[0];
	mlib_s16 c1 = ((mlib_s16 *)c)[1], *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBSC_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, c0 = ((mlib_s16 *)c)[0];
	mlib_s16 c1 = ((mlib_s16 *)c)[1];
	mlib_s32 *pz = (mlib_s32 *)z, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBSC_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16C_S16C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16C_S16C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, c0 = ((mlib_s32 *)c)[0];
	mlib_s32 c1 = ((mlib_s32 *)c)[1], *pz = (mlib_s32 *)z;

#ifdef	_NO_LONGLONG

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 i;
	mlib_d64 diff_r, diff_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_d64)px[2 * i];
		diff_i = c1 - (mlib_d64)px[2 * i + 1];

		if (diff_r > MLIB_S32_MAX)
			diff_r = MLIB_S32_MAX;
		else if (diff_r < MLIB_S32_MIN)
			diff_r = MLIB_S32_MIN;
		if (diff_i > MLIB_S32_MAX)
			diff_i = MLIB_S32_MAX;
		else if (diff_i < MLIB_S32_MIN)
			diff_i = MLIB_S32_MIN;
		pz[2 * i] = diff_r;
		pz[2 * i + 1] = diff_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 diff_r, diff_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_d64)px[2 * i];
		diff_i = c1 - (mlib_d64)px[2 * i + 1];
		pz[2 * i] = diff_r;
		pz[2 * i + 1] = diff_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */

#else	/* _NO_LONGLONG */

	mlib_s32 i;
	mlib_s64 diff_r, diff_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_s64)px[2 * i];
		diff_i = c1 - (mlib_s64)px[2 * i + 1];

		if (diff_r > MLIB_S32_MAX)
			diff_r = MLIB_S32_MAX;
		else if (diff_r < MLIB_S32_MIN)
			diff_r = MLIB_S32_MIN;
		if (diff_i > MLIB_S32_MAX)
			diff_i = MLIB_S32_MAX;
		else if (diff_i < MLIB_S32_MIN)
			diff_i = MLIB_S32_MIN;
		pz[2 * i] = diff_r;
		pz[2 * i + 1] = diff_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif	/* _NO_LONGLONG */
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, c0 = ((mlib_s32 *)c)[0];
	mlib_s32 c1 = ((mlib_s32 *)c)[1], *pz = (mlib_s32 *)z, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	SUBSC_MOD;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32C_S32C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32C_S32C_Mod(xz, xz, c, n));
}

/* *********************************************************** */
