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

#pragma ident	"@(#)mlib_s_VectorMulSAddC.c	9.3	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* ****************************************************************** */

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

/* ****************************************************************** */

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

/* ****************************************************************** */

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

/* ****************************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* ****************************************************************** */

#define	SAT_U8(x)		\
	if (x < 0)		\
	    x = 0;		\
	else if (x > 255)	\
	    x = 255

#define	SAT_S8(x)		\
	if (x < -128)		\
	    x = -128;		\
	else if (x > 127)	\
	    x = 127

#define	SAT_S16(x)			\
	if (x < MLIB_S16_MIN)		\
	    x = MLIB_S16_MIN;		\
	else if (x > MLIB_S16_MAX)	\
	    x = MLIB_S16_MAX

#define	SAT_S32(x)			\
	if (x < MLIB_S32_MIN)		\
	    x = MLIB_S32_MIN;		\
	else if (x > MLIB_S32_MAX)	\
	    x = MLIB_S32_MAX

/* ****************************************************************** */

#define	MULS_MOD						\
	sum0 = (mlib_s32)py[0] * c0 - (mlib_s32)py[1] * c1;	\
	sum1 = (mlib_s32)py[0] * c1 + (mlib_s32)py[1] * c0;	\
	pz[0] = sum0 + px[0];					\
	pz[1] = sum1 + px[1];					\
	px += 2;						\
	py += 2;						\
	pz += 2

#define	MULS_SAT(TYPE)						\
	sum0 = (mlib_s32)py[0] * c0 - (mlib_s32)py[1] * c1;	\
	sum1 = (mlib_s32)py[0] * c1 + (mlib_s32)py[1] * c0;	\
	sum0 += px[0];						\
	sum1 += px[1];						\
	SAT_##TYPE(sum0);					\
	SAT_##TYPE(sum1);					\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	py += 2;						\
	pz += 2

/* ****************************************************************** */

#define	MULS_U8C_SAT_128(ld1, ld2, st)			\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf2 = _mm_unpackhi_epi8(ybuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf3 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf4 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_adds_epi16(buf1, buf3);		\
	buf2 = _mm_adds_epi16(buf2, buf4);		\
	buf1 = _mm_packus_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_U8C_SAT_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(U8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(U8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(U8);
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_U8C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MULS_U8C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_U8C_SAT_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(U8);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_U8C_MOD_128(ld1, ld2, st)			\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf2 = _mm_unpackhi_epi8(ybuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask1);		\
	buf1 = _mm_slli_epi32(buf1, 8);			\
	buf4 = _mm_and_si128(buf4, mask1);		\
	buf2 = _mm_slli_epi32(buf2, 8);			\
	buf3 = _mm_or_si128(buf3, buf1);		\
	buf4 = _mm_or_si128(buf4, buf2);		\
							\
	buf1 = _mm_unpacklo_epi32(buf3, buf4);		\
	buf2 = _mm_unpackhi_epi32(buf3, buf4);		\
	buf3 = _mm_unpacklo_epi32(buf1, buf2);		\
	buf4 = _mm_unpackhi_epi32(buf1, buf2);		\
	buf3 = _mm_and_si128(buf3, mask2);		\
	buf4 = _mm_slli_epi32(buf4, 16);		\
	buf3 = _mm_or_si128(buf3, buf4);		\
	buf3 = _mm_add_epi8(buf3, xbuf);		\
	_mm_##st##_si128((__m128i *)pz, buf3);		\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask1, mask2;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	mask1 = _mm_set1_epi16(0x00ff);
	mask2 = _mm_set1_epi32(0xffff);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_U8C_MOD_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_U8C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MULS_U8C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_U8C_MOD_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S8C_SAT_128(ld1, ld2, st)			\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(zero, ybuf);		\
	buf2 = _mm_unpackhi_epi8(zero, ybuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf3 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf4 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf3 = _mm_srai_epi16(buf3, 8);			\
	buf4 = _mm_srai_epi16(buf4, 8);			\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_adds_epi16(buf1, buf3);		\
	buf2 = _mm_adds_epi16(buf2, buf4);		\
	buf1 = _mm_packs_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S8C_SAT_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(S8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(S8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(S8);
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S8C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MULS_S8C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S8C_SAT_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(S8);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return __mlib_VectorMulSAdd_U8C_U8C_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)y, (mlib_u8 *)c, n);
}

/* ****************************************************************** */

#define	MULS_S16C_SAT_128(ld1, ld2, st)			\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(ybuf, cbuf00);		\
	buf2 = _mm_madd_epi16(ybuf, cbuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	/* S16_MIN*S16_MIN+S16_MIN*S16_MIN > S32_MAX */	\
	ibuf = _mm_madd_epi16(ybuf, cbuf1);		\
	buf3 = _mm_cmpeq_epi32(ibuf, mask1);		\
	buf3 = _mm_slli_epi32(buf3, 16);		\
	ibuf = _mm_xor_si128(ibuf, buf3);		\
							\
	buf3 = _mm_unpacklo_epi16(zero, xbuf);		\
	buf4 = _mm_unpackhi_epi16(zero, xbuf);		\
	buf3 = _mm_srai_epi32(buf3, 16);		\
	buf4 = _mm_srai_epi32(buf4, 16);		\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
	buf1 = _mm_add_epi32(buf1, buf3);		\
	buf2 = _mm_add_epi32(buf2, buf4);		\
	buf1 = _mm_packs_epi32(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

#define	MULS_S16C_SAT						\
	sum0 = (mlib_s32)py[0] * c0 - (mlib_s32)py[1] * c1;	\
	sum1 = (mlib_s32)py[0] * c1 + (mlib_s32)py[1] * c0;	\
	if (sum1 == MLIB_S32_MIN)				\
		pz[1] = MLIB_S16_MAX;				\
	else {							\
		sum1 += px[1];					\
		SAT_S16(sum1);					\
		pz[1] = sum1;					\
	}							\
	sum0 += px[0];						\
	SAT_S16(sum0);						\
	pz[0] = sum0;						\
	px += 2;						\
	py += 2;						\
	pz += 2

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask1, mask2;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	mask1 = _mm_set1_epi32(0x80000000);
	mask2 = _mm_set1_epi32(0x7fff0000);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S16C_SAT_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_S16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_S16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_S16C_SAT;
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MULS_S16C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16C_SAT_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_S16C_SAT;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16C_MOD_128(ld1, ld2, st)			\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(ybuf, cbuf00);		\
	buf2 = _mm_madd_epi16(ybuf, cbuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	ibuf = _mm_madd_epi16(ybuf, cbuf1);		\
							\
	rbuf = _mm_and_si128(rbuf, mask);		\
	ibuf = _mm_slli_epi32(ibuf, 16);		\
	rbuf = _mm_or_si128(rbuf, ibuf);		\
	rbuf = _mm_add_epi16(rbuf, xbuf);		\
	_mm_##st##_si128((__m128i *)pz, rbuf);		\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	mask = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S16C_MOD_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MULS_S16C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16C_MOD_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16CU8C_SAT_128(ld1, ld2, st)		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf2 = _mm_unpackhi_epi8(ybuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_unpacklo_epi32(buf3, buf1);		\
	ybuf = _mm_unpackhi_epi32(buf3, buf1);		\
	ibuf = _mm_unpacklo_epi32(buf4, buf2);		\
	zbuf = _mm_unpackhi_epi32(buf4, buf2);		\
							\
	buf3 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf4 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf1 = _mm_unpacklo_epi16(buf3, zero);		\
	buf2 = _mm_unpackhi_epi16(buf3, zero);		\
	buf3 = _mm_unpacklo_epi16(buf4, zero);		\
	buf4 = _mm_unpackhi_epi16(buf4, zero);		\
	buf1 = _mm_add_epi32(buf1, rbuf);		\
	buf2 = _mm_add_epi32(buf2, ybuf);		\
	buf3 = _mm_add_epi32(buf3, ibuf);		\
	buf4 = _mm_add_epi32(buf4, zbuf);		\
	buf1 = _mm_packs_epi32(buf1, buf2);		\
	buf3 = _mm_packs_epi32(buf3, buf4);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf3);	\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CU8C_SAT_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(S16);
			}
			if (ax == ay && (ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CU8C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CU8C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MULS_S16CU8C_SAT_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16CU8C_MOD_128(ld1, ld2, st)		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf2 = _mm_unpackhi_epi8(ybuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask);		\
	buf1 = _mm_slli_epi32(buf1, 16);		\
	buf4 = _mm_and_si128(buf4, mask);		\
	buf2 = _mm_slli_epi32(buf2, 16);		\
	buf1 = _mm_or_si128(buf3, buf1);		\
	buf2 = _mm_or_si128(buf4, buf2);		\
							\
	buf3 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf4 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf1 = _mm_add_epi16(buf1, buf3);		\
	buf2 = _mm_add_epi16(buf2, buf4);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	mask = _mm_set1_epi32(0xffff);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CU8C_MOD_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if (ax == ay && (ax *2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CU8C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CU8C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MULS_S16CU8C_MOD_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16CS8C_SAT_128(ld1, ld2, st)		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(zero, ybuf);		\
	buf2 = _mm_unpackhi_epi8(zero, ybuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_unpacklo_epi32(buf3, buf1);		\
	ybuf = _mm_unpackhi_epi32(buf3, buf1);		\
	ibuf = _mm_unpacklo_epi32(buf4, buf2);		\
	zbuf = _mm_unpackhi_epi32(buf4, buf2);		\
							\
	buf3 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf4 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_unpacklo_epi16(zero, buf3);		\
	buf2 = _mm_unpackhi_epi16(zero, buf3);		\
	buf3 = _mm_unpacklo_epi16(zero, buf4);		\
	buf4 = _mm_unpackhi_epi16(zero, buf4);		\
	buf1 = _mm_srai_epi32(buf1, 24);		\
	buf2 = _mm_srai_epi32(buf2, 24);		\
	buf3 = _mm_srai_epi32(buf3, 24);		\
	buf4 = _mm_srai_epi32(buf4, 24);		\
	buf1 = _mm_add_epi32(buf1, rbuf);		\
	buf2 = _mm_add_epi32(buf2, ybuf);		\
	buf3 = _mm_add_epi32(buf3, ibuf);		\
	buf4 = _mm_add_epi32(buf4, zbuf);		\
	buf1 = _mm_packs_epi32(buf1, buf2);		\
	buf3 = _mm_packs_epi32(buf3, buf4);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf3);	\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CS8C_SAT_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(S16);
			}
			if (ax == ay && (ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CS8C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CS8C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MULS_S16CS8C_SAT_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16CS8C_MOD_128(ld1, ld2, st)		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	buf1 = _mm_unpacklo_epi8(zero, ybuf);		\
	buf2 = _mm_unpackhi_epi8(zero, ybuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask);		\
	buf1 = _mm_slli_epi32(buf1, 16);		\
	buf4 = _mm_and_si128(buf4, mask);		\
	buf2 = _mm_slli_epi32(buf2, 16);		\
	buf1 = _mm_or_si128(buf3, buf1);		\
	buf2 = _mm_or_si128(buf4, buf2);		\
							\
	buf3 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf4 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf3 = _mm_srai_epi16(buf3, 8);			\
	buf4 = _mm_srai_epi16(buf4, 8);			\
	buf1 = _mm_add_epi16(buf1, buf3);		\
	buf2 = _mm_add_epi16(buf2, buf4);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	mask = _mm_set1_epi32(0xffff);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CS8C_MOD_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if (ax == ay && (ax *2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CS8C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MULS_S16CS8C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MULS_S16CS8C_MOD_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S32CS16C_SAT_128(ld1, ld2, st)		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(ybuf, cbuf00);		\
	buf2 = _mm_madd_epi16(ybuf, cbuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	/* S16_MIN*S16_MIN+S16_MIN*S16_MIN > S32_MAX */	\
	ibuf = _mm_madd_epi16(ybuf, cbuf1);		\
	buf1 = _mm_cmpeq_epi32(ibuf, mask1);		\
	buf2 = _mm_cmpgt_epi16(xbuf, mask2);		\
	buf1 = _mm_and_si128(buf1, buf2);		\
	xbuf = _mm_or_si128(buf1, xbuf);		\
							\
	buf3 = _mm_unpacklo_epi16(zero, xbuf);		\
	buf4 = _mm_unpackhi_epi16(zero, xbuf);		\
	buf3 = _mm_srai_epi32(buf3, 16);		\
	buf4 = _mm_srai_epi32(buf4, 16);		\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
	buf1 = _mm_add_epi32(buf1, buf3);		\
	buf2 = _mm_add_epi32(buf2, buf4);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

#define	MULS_S32CS16C_SAT					\
	sum0 = (mlib_s32)py[0] * c0 - (mlib_s32)py[1] * c1;	\
	sum1 = (mlib_s32)py[0] * c1 + (mlib_s32)py[1] * c0;	\
	pz[0] = sum0 + px[0];					\
	if (sum1 == MLIB_S32_MIN && px[1] > -1)			\
		pz[1] = MLIB_S32_MAX;				\
	else							\
		pz[1] = sum1 + px[1];				\
	px += 2;						\
	py += 2;						\
	pz += 2

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask1, mask2;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi32(0x80000000);
	mask2 = _mm_set1_epi32(0xffff7fff);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S32CS16C_SAT_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_S32CS16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_S32CS16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_S32CS16C_SAT;
			}
			if (ax == ay && (ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MULS_S32CS16C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MULS_S32CS16C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MULS_S32CS16C_SAT_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_S32CS16C_SAT;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S32CS16C_MOD_128(ld1, ld2, st)		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(ybuf, cbuf00);		\
	buf2 = _mm_madd_epi16(ybuf, cbuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	ibuf = _mm_madd_epi16(ybuf, cbuf1);		\
							\
	buf3 = _mm_unpacklo_epi16(zero, xbuf);		\
	buf4 = _mm_unpackhi_epi16(zero, xbuf);		\
	buf3 = _mm_srai_epi32(buf3, 16);		\
	buf4 = _mm_srai_epi32(buf4, 16);		\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
	buf1 = _mm_add_epi32(buf1, buf3);		\
	buf2 = _mm_add_epi32(buf2, buf4);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, ay, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S32CS16C_MOD_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
				    MULS_S32CS16C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MULS_S32CS16C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MULS_S32CS16C_MOD_128(load, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

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

/* ****************************************************************** */

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

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8C_U8C_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8C_U8C_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8C_S8C_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8C_S8C_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16C_S16C_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16C_S16C_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32C_S32C_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32C_S32C_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */
