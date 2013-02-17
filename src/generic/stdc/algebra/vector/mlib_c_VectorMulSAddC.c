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

#pragma ident	"@(#)mlib_c_VectorMulSAddC.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulSAdd_U8C_U8C_Sat   - linear function on unsigned 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_U8C_Sat       - linear function on unsigned 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S8C_S8C_Sat   - linear function on signed 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S8C_Sat       - linear function on signed 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S16C_S16C_Sat - linear function on signed 16-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S16C_Sat      - linear function on signed 16-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S32C_S32C_Sat - linear function on signed 32-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S32C_Sat      - linear function on signed 32-bit
 *                                         format complex vector
 *                                         (inplace version)
 *
 *      mlib_VectorMulSAdd_U8C_U8C_Mod   - linear function on unsigned 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_U8C_Mod       - linear function on unsigned 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S8C_S8C_Mod   - linear function on signed 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S8C_Mod       - linear function on signed 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S16C_S16C_Mod - linear function on signed 16-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S16C_Mod      - linear function on signed 16-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S32C_S32C_Mod - linear function on signed 32-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S32C_Mod      - linear function on signed 32-bit
 *                                         format complex vector
 *                                         (inplace version)
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulSAdd_U8C_U8C_Sat(mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *c,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_U8C_U8C_Mod(mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *c,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8C_S8C_Sat(mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 const mlib_s8 *c,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8C_S8C_Mod(mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 const mlib_s8 *c,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16C_S16C_Sat(mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S16C_S16C_Mod(mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_S32C_Sat(mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   const mlib_s32 *y,
 *                                                   const mlib_s32 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_S32C_Mod(mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   const mlib_s32 *y,
 *                                                   const mlib_s32 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S16C_U8C_Sat(mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16C_U8C_Mod(mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16C_S8C_Sat(mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  const mlib_s8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16C_S8C_Mod(mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  const mlib_s8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S32C_S16C_Sat(mlib_s32       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_S16C_Mod(mlib_s32       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_U8C_Sat(mlib_u8       *xz,
 *                                             const mlib_u8 *y,
 *                                             const mlib_u8 *c,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_U8C_Mod(mlib_u8       *xz,
 *                                             const mlib_u8 *y,
 *                                             const mlib_u8 *c,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8C_Sat(mlib_s8       *xz,
 *                                             const mlib_s8 *y,
 *                                             const mlib_s8 *c,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8C_Mod(mlib_s8       *xz,
 *                                             const mlib_s8 *y,
 *                                             const mlib_s8 *c,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16C_Sat(mlib_s16       *xz,
 *                                              const mlib_s16 *y,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S16C_Mod(mlib_s16       *xz,
 *                                              const mlib_s16 *y,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_Sat(mlib_s32       *xz,
 *                                              const mlib_s32 *y,
 *                                              const mlib_s32 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_Mod(mlib_s32       *xz,
 *                                              const mlib_s32 *y,
 *                                              const mlib_s32 *c,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first element of the first input complex vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first element of the second input complex vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex input scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c * y
 */

#include <mlib_algebra.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulSAdd_S32C_Sat = __mlib_VectorMulSAdd_S32C_Sat
#pragma weak mlib_VectorMulSAdd_S8C_S8C_Sat = \
	__mlib_VectorMulSAdd_S8C_S8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_Sat = __mlib_VectorMulSAdd_S16C_Sat
#pragma weak mlib_VectorMulSAdd_U8C_U8C_Sat = \
	__mlib_VectorMulSAdd_U8C_U8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_S8C_Mod = \
	__mlib_VectorMulSAdd_S16C_S8C_Mod
#pragma weak mlib_VectorMulSAdd_S16C_U8C_Mod = \
	__mlib_VectorMulSAdd_S16C_U8C_Mod
#pragma weak mlib_VectorMulSAdd_S8C_Mod = __mlib_VectorMulSAdd_S8C_Mod
#pragma weak mlib_VectorMulSAdd_U8C_Mod = __mlib_VectorMulSAdd_U8C_Mod
#pragma weak mlib_VectorMulSAdd_S16C_S8C_Sat = \
	__mlib_VectorMulSAdd_S16C_S8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_U8C_Sat = \
	__mlib_VectorMulSAdd_S16C_U8C_Sat
#pragma weak mlib_VectorMulSAdd_S32C_S32C_Mod = \
	__mlib_VectorMulSAdd_S32C_S32C_Mod
#pragma weak mlib_VectorMulSAdd_S8C_Sat = __mlib_VectorMulSAdd_S8C_Sat
#pragma weak mlib_VectorMulSAdd_S32C_S16C_Mod = \
	__mlib_VectorMulSAdd_S32C_S16C_Mod
#pragma weak mlib_VectorMulSAdd_U8C_Sat = __mlib_VectorMulSAdd_U8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_S16C_Mod = \
	__mlib_VectorMulSAdd_S16C_S16C_Mod
#pragma weak mlib_VectorMulSAdd_S32C_Mod = __mlib_VectorMulSAdd_S32C_Mod
#pragma weak mlib_VectorMulSAdd_S8C_S8C_Mod = \
	__mlib_VectorMulSAdd_S8C_S8C_Mod
#pragma weak mlib_VectorMulSAdd_S16C_Mod = __mlib_VectorMulSAdd_S16C_Mod
#pragma weak mlib_VectorMulSAdd_S32C_S32C_Sat = \
	__mlib_VectorMulSAdd_S32C_S32C_Sat
#pragma weak mlib_VectorMulSAdd_U8C_U8C_Mod = \
	__mlib_VectorMulSAdd_U8C_U8C_Mod
#pragma weak mlib_VectorMulSAdd_S32C_S16C_Sat = \
	__mlib_VectorMulSAdd_S32C_S16C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_S16C_Sat = \
	__mlib_VectorMulSAdd_S16C_S16C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMulSAdd_S32C_Sat) mlib_VectorMulSAdd_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S8C_S8C_Sat) mlib_VectorMulSAdd_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_Sat) mlib_VectorMulSAdd_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_Sat")));
__typeof__(__mlib_VectorMulSAdd_U8C_U8C_Sat) mlib_VectorMulSAdd_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_S8C_Mod) mlib_VectorMulSAdd_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16C_U8C_Mod) mlib_VectorMulSAdd_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8C_Mod) mlib_VectorMulSAdd_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8C_Mod) mlib_VectorMulSAdd_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16C_S8C_Sat) mlib_VectorMulSAdd_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_U8C_Sat) mlib_VectorMulSAdd_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32C_S32C_Mod) mlib_VectorMulSAdd_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8C_Sat) mlib_VectorMulSAdd_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32C_S16C_Mod) mlib_VectorMulSAdd_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8C_Sat) mlib_VectorMulSAdd_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_S16C_Mod) mlib_VectorMulSAdd_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32C_Mod) mlib_VectorMulSAdd_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8C_S8C_Mod) mlib_VectorMulSAdd_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16C_Mod) mlib_VectorMulSAdd_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32C_S32C_Sat) mlib_VectorMulSAdd_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulSAdd_U8C_U8C_Mod) mlib_VectorMulSAdd_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32C_S16C_Sat) mlib_VectorMulSAdd_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_S16C_Sat) mlib_VectorMulSAdd_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S16C_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MULSADDC_SAT(form_in, form, MAX, MIN)                          \
	mlib_s32 i;                                                    \
	form_in *px = (form_in *) x, *py = (form_in *) y, *pz =        \
		(form_in *) z;                                         \
	form val_c_r = ((form_in *) c)[0], val_c_i =                   \
		((form_in *) c)[1], mul_add_r, mul_add_i;              \
	/*                                                             \
	 * (a + jb) * (c + jd) = (ac - bd) + j(bc + ad)                \
	 */                                                            \
	for (i = 0; i < n; i++) {                                      \
	    mul_add_r =                                                \
		    px[2 * i] + py[2 * i] * val_c_r - py[2 * i +       \
		    1] * val_c_i;                                      \
	    mul_add_i =                                                \
		    px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +   \
		    1] * val_c_r;                                      \
	    if (mul_add_r > MAX)                                       \
		mul_add_r = MAX;                                       \
	    else if (mul_add_r < MIN)                                  \
		mul_add_r = MIN;                                       \
	    if (mul_add_i > MAX)                                       \
		mul_add_i = MAX;                                       \
	    else if (mul_add_i < MIN)                                  \
		mul_add_i = MIN;                                       \
	    pz[2 * i] = mul_add_r;                                     \
	    pz[2 * i + 1] = mul_add_i;                                 \
	}                                                              \
	                                                               \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	MULSADDC_MOD(form_in, form)                                    \
	mlib_s32 i;                                                    \
	form_in *px = (form_in *) x, *py = (form_in *) y, *pz =        \
		(form_in *) z;                                         \
	form val_c_r = ((form_in *) c)[0], val_c_i =                   \
		((form_in *) c)[1], mul_add_r, mul_add_i;              \
	/*                                                             \
	 * (a + jb) * (c + jd) = (ac - bd) + j(bc + ad)                \
	 */                                                            \
	for (i = 0; i < n; i++) {                                      \
	    mul_add_r =                                                \
		    px[2 * i] + py[2 * i] * val_c_r - py[2 * i +       \
		    1] * val_c_i;                                      \
	    mul_add_i =                                                \
		    px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +   \
		    1] * val_c_r;                                      \
	    pz[2 * i] = mul_add_r;                                     \
	    pz[2 * i + 1] = mul_add_i;                                 \
	}                                                              \
	                                                               \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 val_c_r = ((mlib_u8 *)c)[0], val_c_i =
		((mlib_u8 *)c)[1], mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	for (i = 0; i < n; i++) {
		mul_add_r =
			px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
			1] * val_c_i;
		mul_add_i =
			px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
			1] * val_c_r;

		if (mul_add_r > MLIB_U8_MAX)
			mul_add_r = MLIB_U8_MAX;
		else if (mul_add_r < 0)
			mul_add_r = 0;

		if (mul_add_i > MLIB_U8_MAX)
			mul_add_i = MLIB_U8_MAX;
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return __mlib_VectorMulSAdd_S8C_S8C_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)y, (mlib_s8 *)c, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	MULSADDC_SAT(mlib_s8,
		mlib_s32,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	mlib_d64 val_c_r = ((mlib_s8 *)c)[0], val_c_i = ((mlib_s8 *)c)[1];
	mlib_d64 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */

	if (val_c_r == MLIB_S8_MIN && val_c_i == MLIB_S8_MIN)
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + (py[2 * i] - py[2 * i +
				1]) * MLIB_S8_MIN;
			mul_add_i =
				px[2 * i + 1] + (py[2 * i] + py[2 * i +
				1]) * MLIB_S8_MIN;
			pz[2 * i] = mul_add_r;
			pz[2 * i + 1] = mul_add_i;
	} else
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i;
			mul_add_i =
				px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r;
			pz[2 * i] = mul_add_r;
			pz[2 * i + 1] = mul_add_i;
		}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	MULSADDC_SAT(mlib_s16,
		mlib_d64,
		MLIB_S16_MAX,
		MLIB_S16_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	mlib_d64 val_c_r = ((mlib_s16 *)c)[0], val_c_i = ((mlib_s16 *)c)[1];
	mlib_d64 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */

	if (val_c_r == MLIB_S16_MIN && val_c_i == MLIB_S16_MIN)
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + (py[2 * i] - py[2 * i +
				1]) * MLIB_S16_MIN;
			mul_add_i =
				px[2 * i + 1] + (py[2 * i] + py[2 * i +
				1]) * MLIB_S16_MIN;

			if (mul_add_i > MLIB_S32_MAX)
				mul_add_i -= MLIB_S32_MAX;
			pz[2 * i] = mul_add_r;
			pz[2 * i + 1] = mul_add_i;
	} else
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i;
			mul_add_i =
				px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r;
			pz[2 * i] = mul_add_r;
			pz[2 * i + 1] = mul_add_i;
		}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
#define	MAX	0x1fffff
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_d64 val_c_r = ((mlib_s32 *)c)[0], val_c_i = ((mlib_s32 *)c)[1];
	mlib_d64 rr, ii, rr_hi, rr_lo, ii_hi, ii_lo;

	if ((mlib_fabs(val_c_r) < MAX) && (fabs(val_c_i) < MAX)) {
		for (i = 0; i < n; i++) {
			rr = px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i;
			ii = px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r;
			pz[2 * i] = FLOAT2INT_CLAMP(rr);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(ii);
		}
	} else {
		mlib_d64 val_c_r_lo = ((mlib_s32 *)c)[0] & MAX, val_c_i_lo =
			((mlib_s32 *)c)[1] & MAX;

		val_c_r -= val_c_r_lo;
		val_c_i -= val_c_i_lo;
		for (i = 0; i < n; i++) {
			rr_lo = px[2 * i] + (py[2 * i] * val_c_r_lo - py[2 * i +
				1] * val_c_i_lo);
			rr_hi = py[2 * i] * val_c_r - py[2 * i + 1] * val_c_i;
			ii_lo = px[2 * i + 1] + (py[2 * i] * val_c_i_lo +
				py[2 * i + 1] * val_c_r_lo);
			ii_hi = py[2 * i] * val_c_i + py[2 * i + 1] * val_c_r;
			rr = rr_hi + rr_lo;
			ii = ii_hi + ii_lo;
			pz[2 * i] = FLOAT2INT_CLAMP(rr);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(ii);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	MULSADDC_MOD(mlib_s32,
		mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 val_c_r = ((mlib_u8 *)c)[0], val_c_i = ((mlib_u8 *)c)[1];
	mlib_s32 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	for (i = 0; i < n; i++) {
		mul_add_r =
			px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
			1] * val_c_i;
		mul_add_i =
			px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
			1] * val_c_r;

		if (mul_add_r > MLIB_S16_MAX)
			mul_add_r = MLIB_S16_MAX;
		else if (mul_add_r < MLIB_S16_MIN)
			mul_add_r = MLIB_S16_MIN;

		if (mul_add_i > MLIB_S16_MAX)
			mul_add_i = MLIB_S16_MAX;
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_d64 val_c_r = ((mlib_u8 *)c)[0], val_c_i = ((mlib_u8 *)c)[1];
	mlib_d64 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	for (i = 0; i < n; i++) {
		mul_add_r =
			px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
			1] * val_c_i;
		mul_add_i =
			px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
			1] * val_c_r;
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 val_c_r = ((mlib_s8 *)c)[0], val_c_i = ((mlib_s8 *)c)[1];
	mlib_s32 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	for (i = 0; i < n; i++) {
		mul_add_r =
			px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
			1] * val_c_i;
		mul_add_i =
			px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
			1] * val_c_r;

		if (mul_add_i > MLIB_S16_MAX)
			mul_add_i = MLIB_S16_MAX;
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_d64 val_c_r = ((mlib_s8 *)c)[0], val_c_i = ((mlib_s8 *)c)[1];
	mlib_d64 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	for (i = 0; i < n; i++) {
		mul_add_r =
			px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
			1] * val_c_i;
		mul_add_i =
			px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
			1] * val_c_r;
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 val_c_r = ((mlib_s16 *)c)[0], val_c_i = ((mlib_s16 *)c)[1];
	mlib_d64 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	for (i = 0; i < n; i++) {
		mul_add_r =
			px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
			1] * val_c_i;
		mul_add_i =
			px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
			1] * val_c_r;
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 val_c_r = ((mlib_s16 *)c)[0], val_c_i = ((mlib_s16 *)c)[1];
	mlib_d64 mul_add_r, mul_add_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */

	if (val_c_r == MLIB_S16_MIN && val_c_i == MLIB_S16_MIN)
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i;
			mul_add_i =
				px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r;
			pz[2 * i] = mul_add_r;

			if (mul_add_i > MLIB_S32_MAX)
				mul_add_i += 2.0 * MLIB_S32_MIN;
			pz[2 * i + 1] = mul_add_i;
	} else {
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i;
			mul_add_i =
				px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r;
			pz[2 * i] = mul_add_r;
			pz[2 * i + 1] = mul_add_i;
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8C_U8C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8C_U8C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8C_S8C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8C_S8C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16C_S16C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16C_S16C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32C_S32C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32C_S32C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */
