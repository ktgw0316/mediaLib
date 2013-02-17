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

#pragma ident	"@(#)mlib_c_SignalConvertShift_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalConvertShift_[U8_F32|U8S_F32S]_Sat
 *      mlib_SignalConvertShift_[S8_F32|S8S_F32S]_Sat
 *      mlib_SignalConvertShift_[S16_F32|S16S_F32S]_Sat
 *
 *      mlib_SignalConvertShift_[F32_U8|F32S_U8S]
 *      mlib_SignalConvertShift_[F32_S8|F32S_S8S]
 *      mlib_SignalConvertShift_[F32_S16|F32S_S16S]
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_F32S_S16S(mlib_f32       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       shift,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32S_S8S(mlib_f32      *dst,
 *                                                   const mlib_s8 *src,
 *                                                   mlib_s32      shift,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_F32S_U8S(mlib_f32      *dst,
 *                                                   const mlib_u8 *src,
 *                                                   mlib_s32      shift,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_F32_S16(mlib_f32       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32_S8(mlib_f32      *dst,
 *                                                 const mlib_s8 *src,
 *                                                 mlib_s32      shift,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_F32_U8(mlib_f32      *dst,
 *                                                 const mlib_u8 *src,
 *                                                 mlib_s32      shift,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16S_F32S_Sat(mlib_s16       *dst,
 *                                                        const mlib_f32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16_F32_Sat(mlib_s16       *dst,
 *                                                      const mlib_f32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8S_F32S_Sat(mlib_s8        *dst,
 *                                                       const mlib_f32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8_F32_Sat(mlib_s8        *dst,
 *                                                     const mlib_f32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_F32S_Sat(mlib_u8        *dst,
 *                                                       const mlib_f32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_F32_Sat(mlib_u8        *dst,
 *                                                     const mlib_f32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 * ARGUMENTS
 *      dst   Output signal array
 *      src   Input signal array
 *      shift Left shifting factor, if negative - shifts right
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      left shift of Q-format signal arrays
 *      dst = src * 2**shift;
 *      dst = saturate( dst );
 */

#include <mlib_signal.h>
#include <mlib_SignalConvertShift.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_F32S_S16S = \
	__mlib_SignalConvertShift_F32S_S16S
#pragma weak mlib_SignalConvertShift_F32S_S8S = \
	__mlib_SignalConvertShift_F32S_S8S
#pragma weak mlib_SignalConvertShift_F32S_U8S = \
	__mlib_SignalConvertShift_F32S_U8S
#pragma weak mlib_SignalConvertShift_F32_S16 = \
	__mlib_SignalConvertShift_F32_S16
#pragma weak mlib_SignalConvertShift_F32_S8 = \
	__mlib_SignalConvertShift_F32_S8
#pragma weak mlib_SignalConvertShift_F32_U8 = \
	__mlib_SignalConvertShift_F32_U8
#pragma weak mlib_SignalConvertShift_S16S_F32S_Sat = \
	__mlib_SignalConvertShift_S16S_F32S_Sat
#pragma weak mlib_SignalConvertShift_S16_F32_Sat = \
	__mlib_SignalConvertShift_S16_F32_Sat
#pragma weak mlib_SignalConvertShift_S8S_F32S_Sat = \
	__mlib_SignalConvertShift_S8S_F32S_Sat
#pragma weak mlib_SignalConvertShift_S8_F32_Sat = \
	__mlib_SignalConvertShift_S8_F32_Sat
#pragma weak mlib_SignalConvertShift_U8S_F32S_Sat = \
	__mlib_SignalConvertShift_U8S_F32S_Sat
#pragma weak mlib_SignalConvertShift_U8_F32_Sat = \
	__mlib_SignalConvertShift_U8_F32_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_F32S_S16S)
    mlib_SignalConvertShift_F32S_S16S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_S16S")));
__typeof__(__mlib_SignalConvertShift_F32S_S8S) mlib_SignalConvertShift_F32S_S8S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_S8S")));
__typeof__(__mlib_SignalConvertShift_F32S_U8S) mlib_SignalConvertShift_F32S_U8S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_U8S")));
__typeof__(__mlib_SignalConvertShift_F32_S16) mlib_SignalConvertShift_F32_S16
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_S16")));
__typeof__(__mlib_SignalConvertShift_F32_S8) mlib_SignalConvertShift_F32_S8
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_S8")));
__typeof__(__mlib_SignalConvertShift_F32_U8) mlib_SignalConvertShift_F32_U8
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_U8")));
__typeof__(__mlib_SignalConvertShift_S16S_F32S_Sat)
    mlib_SignalConvertShift_S16S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_F32_Sat)
    mlib_SignalConvertShift_S16_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_F32_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_F32S_Sat)
    mlib_SignalConvertShift_S8S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_F32_Sat)
    mlib_SignalConvertShift_S8_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_F32_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_F32S_Sat)
    mlib_SignalConvertShift_U8S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_F32_Sat)
    mlib_SignalConvertShift_U8_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_F32_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_F32_Sat(
    mlib_u8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 8);
	fl_c *= (1 << 24);

	while (((mlib_addr)dst & 3) && n > 0) {
		mlib_d64 dd = src[0] * fl_c;

		dst[0] = (FLOAT2INT_CLAMP1(dd - 2147483648.f) >> 24) + 128;
		dst++;
		src++;
		n--;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (n >> 2); i++) {
		mlib_u32 s0, s1, s2, s3;

#ifdef _LITTLE_ENDIAN
		mlib_d64 d0 = src[4 * i + 3] * fl_c;
		mlib_d64 d1 = src[4 * i + 2] * fl_c;
		mlib_d64 d2 = src[4 * i + 1] * fl_c;
		mlib_d64 d3 = src[4 * i] * fl_c;

		s0 = (mlib_u32)(FLOAT2INT_CLAMP1(d0 - 2147483648.f) +
		    (128U << 24)) & ~0xFFFFFF;
		s1 = (mlib_u32)((FLOAT2INT_CLAMP1(d1 - 2147483648.f) +
		    (128U << 24)) & ~0xFFFFFF) >> 8;
		s2 = (mlib_u32)((FLOAT2INT_CLAMP1(d2 - 2147483648.f) +
		    (128U << 24)) & ~0xFFFFFF) >> 16;
		s3 = (mlib_u32)(FLOAT2INT_CLAMP1(d3 - 2147483648.f) +
		    (128U << 24)) >> 24;
#else /* _LITTLE_ENDIAN */
		s0 = (mlib_u32)(FLOAT2INT_CLAMP1(src[4 * i] * fl_c -
		    2147483648.f) + (128U << 24)) & ~0xFFFFFF;
		s1 = (mlib_u32)((FLOAT2INT_CLAMP1(src[4 * i + 1] * fl_c -
		    2147483648.f) + (128U << 24)) & ~0xFFFFFF) >> 8;
		s2 = (mlib_u32)((FLOAT2INT_CLAMP1(src[4 * i + 2] * fl_c -
		    2147483648.f) + (128U << 24)) & ~0xFFFFFF) >> 16;
		s3 = (mlib_u32)(FLOAT2INT_CLAMP1(src[4 * i + 3] * fl_c -
		    2147483648.f) + (128U << 24)) >> 24;
#endif /* _LITTLE_ENDIAN */
		((mlib_u32 *)dst)[i] = s0 | s1 | s2 | s3;
	}

	for (i <<= 2; i < n; i++) {
		mlib_d64 dd = src[i] * fl_c;

		dst[i] = (FLOAT2INT_CLAMP1(dd - 2147483648.f) >> 24) + 128;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_F32_Sat(
    mlib_s8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c;
	TYPE_64BIT buf[BUFSIZE];
	mlib_s32 nb;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 7);
	fl_c *= (1 << 24);

	while (((mlib_addr)dst & 3) && n > 0) {
		dst[0] = FLOAT2INT_CLAMP1(src[0] * fl_c) >> 24;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {
		if ((mlib_addr)src & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_f32 src0, src1;
				mlib_union64 dst0;

				src0 = src[2 * i];
				src1 = src[2 * i + 1];
				dst0.s32x2.i0 = FLOAT2INT_CLAMP1(src0 * fl_c);
				dst0.s32x2.i1 = FLOAT2INT_CLAMP1(src1 * fl_c);
				buf[i] = dst0.d64;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 src0, dst0;

				src0.d64 = ((TYPE_64BIT *) src)[i];
				dst0.s32x2.i0 =
				    FLOAT2INT_CLAMP1(src0.f32x2.i0 * fl_c);
				dst0.s32x2.i1 =
				    FLOAT2INT_CLAMP1(src0.f32x2.i1 * fl_c);
				buf[i] = dst0.d64;
			}
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_u32 r0, r1, r2, r3;

#ifdef _LITTLE_ENDIAN
			r0 = ((mlib_u32 *)buf)[4 * i + 3] & ~0xFFFFFF;
			r1 = (((mlib_u32 *)buf)[4 * i + 2] & ~0xFFFFFF) >> 8;
			r2 = (((mlib_u32 *)buf)[4 * i + 1] & ~0xFFFFFF) >> 16;
			r3 = ((mlib_u32 *)buf)[4 * i] >> 24;
#else /* _LITTLE_ENDIAN */
			r0 = ((mlib_u32 *)buf)[4 * i] & ~0xFFFFFF;
			r1 = (((mlib_u32 *)buf)[4 * i + 1] & ~0xFFFFFF) >> 8;
			r2 = (((mlib_u32 *)buf)[4 * i + 2] & ~0xFFFFFF) >> 16;
			r3 = ((mlib_u32 *)buf)[4 * i + 3] >> 24;
#endif /* _LITTLE_ENDIAN */
			((mlib_u32 *)dst)[i] = r0 | r1 | r2 | r3;
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++) {
		dst[i] = FLOAT2INT_CLAMP1(src[i] * fl_c) >> 24;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_F32_Sat(
    mlib_s16 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c;
	TYPE_64BIT buf[BUFSIZE];
	mlib_s32 nb;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 15);
	fl_c *= (1 << 16);

	while (((mlib_addr)dst & 3) && n > 0) {
		dst[0] = FLOAT2INT_CLAMP1(src[0] * fl_c) >> 16;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {
		if ((mlib_addr)src & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_f32 src0, src1;
				mlib_union64 dst0;

				src0 = src[2 * i];
				src1 = src[2 * i + 1];
				dst0.s32x2.i0 = FLOAT2INT_CLAMP1(src0 * fl_c);
				dst0.s32x2.i1 = FLOAT2INT_CLAMP1(src1 * fl_c);
				buf[i] = dst0.d64;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 src0, dst0;

				src0.d64 = ((TYPE_64BIT *) src)[i];
				dst0.s32x2.i0 =
				    FLOAT2INT_CLAMP1(src0.f32x2.i0 * fl_c);
				dst0.s32x2.i1 =
				    FLOAT2INT_CLAMP1(src0.f32x2.i1 * fl_c);
				buf[i] = dst0.d64;
			}
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_u32 dst0, s0, s1;

#ifdef _LITTLE_ENDIAN
			s0 = ((mlib_u32 *)buf)[2 * i + 1] & ~0xFFFF;
			s1 = ((mlib_u32 *)buf)[2 * i] >> 16;
#else /* _LITTLE_ENDIAN */
			s0 = ((mlib_u32 *)buf)[2 * i] & ~0xFFFF;
			s1 = ((mlib_u32 *)buf)[2 * i + 1] >> 16;
#endif /* _LITTLE_ENDIAN */
			dst0 = s0 | s1;
			((mlib_u32 *)dst)[i] = dst0;
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++) {
		dst[i] = FLOAT2INT_CLAMP1(src[i] * fl_c) >> 16;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_U8(
    mlib_f32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c8;
	mlib_f32 fl_c16;
	mlib_f32 fl_c24;
	mlib_s32 nb;
	TYPE_64BIT buf[BUFSIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c8, shift - 8);
	fl_c16 = fl_c8 * (1.f / (1 << 8));
	fl_c24 = fl_c8 * (1.f / (1 << 16));

	while (((mlib_addr)src & 3) && n > 0) {
		dst[0] = src[0] * fl_c8;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_u32 s0;

			s0 = ((mlib_u32 *)src)[i];
			((mlib_u32 *)buf)[4 * i] = s0 >> 24;
			((mlib_u32 *)buf)[4 * i + 1] = s0 & 0x00FF0000U;
			((mlib_u32 *)buf)[4 * i + 2] = s0 & 0x0000FF00U;
			((mlib_u32 *)buf)[4 * i + 3] = s0 & 0xFFU;
		}

		if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 2; i++) {
				mlib_union64 dst0, dst1;

				dst0.d64 = buf[2 * i];
				dst1.d64 = buf[2 * i + 1];
#ifdef _LITTLE_ENDIAN
				dst[4 * i + 3] = ITOF(dst0.f32x2.i0) * fl_c8;
				dst[4 * i + 2] = ITOF(dst0.f32x2.i1) * fl_c24;
				dst[4 * i + 1] = ITOF(dst1.f32x2.i0) * fl_c16;
				dst[4 * i] = ITOF(dst1.f32x2.i1) * fl_c8;
#else /* _LITTLE_ENDIAN */
				dst[4 * i] = ITOF(dst0.f32x2.i0) * fl_c8;
				dst[4 * i + 1] = ITOF(dst0.f32x2.i1) * fl_c24;
				dst[4 * i + 2] = ITOF(dst1.f32x2.i0) * fl_c16;
				dst[4 * i + 3] = ITOF(dst1.f32x2.i1) * fl_c8;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 2; i++) {
				mlib_union64 dst0, dst1, d0, d1;

				dst0.d64 = buf[2 * i];
				dst1.d64 = buf[2 * i + 1];
#ifdef _LITTLE_ENDIAN
				d1.f32x2.i1 = ITOF(dst0.f32x2.i0) * fl_c8;
				d1.f32x2.i0 = ITOF(dst0.f32x2.i1) * fl_c24;
				d0.f32x2.i1 = ITOF(dst1.f32x2.i0) * fl_c16;
				d0.f32x2.i0 = ITOF(dst1.f32x2.i1) * fl_c8;
#else /* _LITTLE_ENDIAN */
				d0.f32x2.i0 = ITOF(dst0.f32x2.i0) * fl_c8;
				d0.f32x2.i1 = ITOF(dst0.f32x2.i1) * fl_c24;
				d1.f32x2.i0 = ITOF(dst1.f32x2.i0) * fl_c16;
				d1.f32x2.i1 = ITOF(dst1.f32x2.i1) * fl_c8;
#endif /* _LITTLE_ENDIAN */
				((TYPE_64BIT *) dst)[2 * i] = d0.d64;
				((TYPE_64BIT *) dst)[2 * i + 1] = d1.d64;
			}
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++) {
		dst[i] = src[i] * fl_c8;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S8(
    mlib_f32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 nb;
	mlib_f32 fl_c8;
	mlib_f32 fl_c32;
	TYPE_64BIT buf[BUFSIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c8, shift - 7);
	fl_c32 = fl_c8 * (1.f / (1 << 24));

	while (((mlib_addr)src & 3) && n > 0) {
		dst[0] = src[0] * fl_c8;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_s32 s0;

			s0 = ((mlib_s32 *)src)[i];
#ifdef _LITTLE_ENDIAN
			((mlib_s32 *)buf)[4 * i + 3] = s0 & ~0xFFFFFF;
			((mlib_s32 *)buf)[4 * i + 2] = (s0 >> 16) << 24;
			((mlib_s32 *)buf)[4 * i + 1] = (s0 >> 8) << 24;
			((mlib_s32 *)buf)[4 * i] = s0 << 24;
#else /* _LITTLE_ENDIAN */
			((mlib_s32 *)buf)[4 * i] = s0 & ~0xFFFFFF;
			((mlib_s32 *)buf)[4 * i + 1] = (s0 >> 16) << 24;
			((mlib_s32 *)buf)[4 * i + 2] = (s0 >> 8) << 24;
			((mlib_s32 *)buf)[4 * i + 3] = s0 << 24;
#endif /* _LITTLE_ENDIAN */
		}

		if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 dst0;

				dst0.d64 = buf[i];
				dst[2 * i] = ITOF(dst0.f32x2.i0) * fl_c32;
				dst[2 * i + 1] = ITOF(dst0.f32x2.i1) * fl_c32;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 dst0, res0;

				dst0.d64 = buf[i];
				res0.f32x2.i0 = ITOF(dst0.f32x2.i0) * fl_c32;
				res0.f32x2.i1 = ITOF(dst0.f32x2.i1) * fl_c32;
				((TYPE_64BIT *) dst)[i] = res0.d64;
			}
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++) {
		dst[i] = src[i] * fl_c8;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S16(
    mlib_f32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 nb;
	mlib_f32 fl_c16;
	mlib_f32 fl_c32;
	TYPE_64BIT buf[BUFSIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c16, shift - 15);
	fl_c32 = fl_c16 * (1.f / (1 << 16));

	while (((mlib_addr)src & 3) && n > 0) {
		dst[0] = src[0] * fl_c16;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 1 > 0; ) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_s32 src0;

			src0 = ((mlib_s32 *)src)[i];
#ifdef _LITTLE_ENDIAN
			((mlib_s32 *)buf)[2 * i + 1] = src0 & ~0xFFFF;
			((mlib_s32 *)buf)[2 * i] = src0 << 16;
#else /* _LITTLE_ENDIAN */
			((mlib_s32 *)buf)[2 * i] = src0 & ~0xFFFF;
			((mlib_s32 *)buf)[2 * i + 1] = src0 << 16;
#endif /* _LITTLE_ENDIAN */
		}

		if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 dst0;

				dst0.d64 = buf[i];
				dst[2 * i] = ITOF(dst0.f32x2.i0) * fl_c32;
				dst[2 * i + 1] = ITOF(dst0.f32x2.i1) * fl_c32;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 dst0, res0;

				dst0.d64 = buf[i];
				res0.f32x2.i0 = ITOF(dst0.f32x2.i0) * fl_c32;
				res0.f32x2.i1 = ITOF(dst0.f32x2.i1) * fl_c32;
				((TYPE_64BIT *) dst)[i] = res0.d64;
			}
		}

		nb -= (n & ~1);
		src += (n & ~1);
		dst += (n & ~1);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++) {
		dst[i] = src[i] * fl_c16;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8S_F32S_Sat(
    mlib_u8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_U8_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8S_F32S_Sat(
    mlib_s8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S8_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16S_F32S_Sat(
    mlib_s16 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S16_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_U8S(
    mlib_f32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_U8(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_S8S(
    mlib_f32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_S8(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_S16S(
    mlib_f32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_S16(dst, src, shift, n << 1));
}

/* *********************************************************** */
