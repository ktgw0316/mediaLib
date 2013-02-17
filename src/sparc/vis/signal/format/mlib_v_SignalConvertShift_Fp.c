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

#pragma ident	"@(#)mlib_v_SignalConvertShift_Fp.c	9.2	07/11/05 SMI"

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
 *    - left shift of Q-format signal arrays
 *      dst = src * 2**shift;
 *      dst = saturate( dst );
 */

#include <vis_proto.h>
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
__mlib_SignalConvertShift_S8_F32_Sat(
    mlib_s8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c;
	mlib_d64 buf[BUFSIZE];
	mlib_s32 nb;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 7);
	fl_c *= (1 << 24);

	while (((mlib_addr)dst & 3) && n > 0) {
		SAT8(dst[0], src[0] * fl_c);
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {

		if ((mlib_addr)src & 7) {
			if (n > 1) {
				SAT_S32(((mlib_s32 *)buf)[0], src[0] * fl_c);
				SAT_S32(((mlib_s32 *)buf)[(n & ~1) - 1],
				    src[(n & ~1) - 1] * fl_c);
			}
#pragma pipeloop(0)
			for (i = 0; i < (n >> 1) - 1; i++) {
				mlib_f32 mul0, mul1;
				mlib_d64 src0;

				src0 = ((mlib_d64 *)(src + 1))[i];

				mul0 = vis_read_hi(src0) * fl_c;
				mul1 = vis_read_lo(src0) * fl_c;

				SAT_S32(((mlib_s32 *)buf + 1)[2 * i], mul0);
				SAT_S32(((mlib_s32 *)buf + 1)[2 * i + 1], mul1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 src64, dst0;

				src64 = ((mlib_d64 *)src)[i];

				SAT_S32(((mlib_s32 *)&dst0)[0],
				    vis_read_hi(src64) * fl_c);
				SAT_S32(((mlib_s32 *)&dst0)[1],
				    vis_read_lo(src64) * fl_c);

				buf[i] = dst0;
			}
		}

#pragma pipeloop(0)
		for (i = 0; i < n >> 2; i++) {
			mlib_d64 r0, r1, t0, t1, t2;

			r0 = buf[2 * i];
			r1 = buf[2 * i + 1];
			t0 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));
			t1 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));
			t2 = vis_fpmerge(vis_read_hi(t0), vis_read_hi(t1));

			((mlib_f32 *)dst)[i] = vis_read_hi(t2);
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++)
		SAT8(dst[i], src[i] * fl_c);

	return (MLIB_SUCCESS);
}

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
	mlib_f32 fl_c24;
	mlib_d64 buf[BUFSIZE];
	mlib_s32 nb;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	vis_write_gsr(7 << 3);

	PREPARE_CONST(fl_c, shift + 8);
	fl_c24 = fl_c * (1 << 16);
	fl_c = fl_c * (1 << 24);

	while (((mlib_addr)dst & 7) && n > 0) {
		SAT_U8(dst[0], src[0] * fl_c);
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 3 > 0; ) {
		if ((mlib_addr)src & 7) {
			if (n > 1) {
				SAT_S32(((mlib_s32 *)buf)[0], src[0] * fl_c24);
				SAT_S32(((mlib_s32 *)buf)[(n & ~1) - 1],
				    src[(n & ~1) - 1] * fl_c24);
			}
#pragma pipeloop(0)
			for (i = 0; i < (n >> 1) - 1; i++) {
				mlib_f32 mul0, mul1;
				mlib_d64 src0;

				src0 = ((mlib_d64 *)(src + 1))[i];

				mul0 = vis_read_hi(src0) * fl_c24;
				mul1 = vis_read_lo(src0) * fl_c24;

				SAT_S32(((mlib_s32 *)buf + 1)[2 * i], mul0);
				SAT_S32(((mlib_s32 *)buf + 1)[2 * i + 1], mul1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 src64, dst0;

				src64 = ((mlib_d64 *)src)[i];

				SAT_S32(((mlib_s32 *)&dst0)[0],
				    vis_read_hi(src64) * fl_c24);
				SAT_S32(((mlib_s32 *)&dst0)[1],
				    vis_read_lo(src64) * fl_c24);

				buf[i] = dst0;
			}
		}

#pragma pipeloop(0)
		for (i = 0; i < n >> 3; i++) {
			mlib_d64 r0, r1, r2, r3, res0;

			r0 = buf[4 * i];
			r1 = buf[4 * i + 1];
			r2 = buf[4 * i + 2];
			r3 = buf[4 * i + 3];

			res0 = vis_fpack32(r0, r0);
			res0 = vis_fpack32(res0, r1);
			res0 = vis_fpack32(res0, r2);
			res0 = vis_fpack32(res0, r3);

			((mlib_d64 *)dst)[i] =
			    vis_fpmerge(vis_read_hi(res0), vis_read_lo(res0));
		}

		nb -= (n & ~7);
		src += (n & ~7);
		dst += (n & ~7);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++)
		SAT_U8(dst[i], src[i] * fl_c);

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
	mlib_d64 buf[BUFSIZE];
	mlib_s32 nb;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	vis_write_gsr(0);

	PREPARE_CONST(fl_c, shift + 15);
	fl_c *= (1 << 16);

	while (((mlib_addr)dst & 7) && n > 0) {
		SAT16(dst[0], src[0] * fl_c);
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {
		if ((mlib_addr)src & 7) {
			if (n > 1) {
				SAT_S32(((mlib_s32 *)buf)[0], src[0] * fl_c);
				SAT_S32(((mlib_s32 *)buf)[(n & ~1) - 1],
				    src[(n & ~1) - 1] * fl_c);
			}
#pragma pipeloop(0)
			for (i = 0; i < (n >> 1) - 1; i++) {
				mlib_f32 mul0, mul1;
				mlib_d64 src0;

				src0 = ((mlib_d64 *)(src + 1))[i];

				mul0 = vis_read_hi(src0) * fl_c;
				mul1 = vis_read_lo(src0) * fl_c;

				SAT_S32(((mlib_s32 *)buf + 1)[2 * i], mul0);
				SAT_S32(((mlib_s32 *)buf + 1)[2 * i + 1], mul1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 src64, dst0;

				src64 = ((mlib_d64 *)src)[i];

				SAT_S32(((mlib_s32 *)&dst0)[0],
				    vis_read_hi(src64) * fl_c);
				SAT_S32(((mlib_s32 *)&dst0)[1],
				    vis_read_lo(src64) * fl_c);

				buf[i] = dst0;
			}
		}

#pragma pipeloop(0)
		for (i = 0; i < n >> 2; i++) {
			mlib_d64 r0, r1, res0;

			r0 = buf[2 * i];
			r1 = buf[2 * i + 1];

			((mlib_f32 *)&res0)[0] = vis_fpackfix(r0);
			((mlib_f32 *)&res0)[1] = vis_fpackfix(r1);

			((mlib_d64 *)dst)[i] = res0;
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++)
		SAT16(dst[i], src[i] * fl_c);

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
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_s32 nb;
	mlib_d64 buf[BUFSIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c8, shift - 8);

	while (((mlib_addr)src & 7) && n > 0) {
		dst[0] = src[0] * fl_c8;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 3 > 0; ) {
#pragma pipeloop(0)
		for (i = 0; i < n >> 3; i++) {
			mlib_d64 s0;
			mlib_d64 t0, t1;

			s0 = ((mlib_d64 *)src)[i];
			t0 = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s0));
			t1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s0));
			buf[4 * i] = vis_fmuld8ulx16(vis_read_hi(t0), fone);
			buf[4 * i + 1] = vis_fmuld8ulx16(vis_read_lo(t0), fone);
			buf[4 * i + 2] = vis_fmuld8ulx16(vis_read_hi(t1), fone);
			buf[4 * i + 3] = vis_fmuld8ulx16(vis_read_lo(t1), fone);
		}

		if ((mlib_addr)dst & 7) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[i];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				dst[2 * i] = ITOF(s0) * fl_c8;
				dst[2 * i + 1] = ITOF(s1) * fl_c8;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 dst0, res0;
				mlib_f32 s0, s1;

				dst0 = buf[i];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				res0 =
				    vis_freg_pair(ITOF(s0) * fl_c8,
				    ITOF(s1) * fl_c8);
				((mlib_d64 *)dst)[i] = res0;
			}
		}

		nb -= (n & ~7);
		src += (n & ~7);
		dst += (n & ~7);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++)
		dst[i] = src[i] * fl_c8;

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
	mlib_d64 buf[BUFSIZE];
	mlib_f32 fone = vis_to_float(0x10001);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c8, shift - 7);
	fl_c32 = fl_c8 * (1.f / (1 << 8));

	while (((mlib_addr)src & 7) && n > 0) {
		dst[0] = src[0] * fl_c8;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 3 > 0; ) {
#pragma pipeloop(0)
		for (i = 0; i < n >> 3; i++) {
			mlib_d64 s0;
			mlib_d64 t0, t1;

			s0 = ((mlib_d64 *)src)[i];
			t0 = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s0));
			t1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s0));
			buf[4 * i] = vis_fmuld8sux16(vis_read_hi(t0), fone);
			buf[4 * i + 1] = vis_fmuld8sux16(vis_read_lo(t0), fone);
			buf[4 * i + 2] = vis_fmuld8sux16(vis_read_hi(t1), fone);
			buf[4 * i + 3] = vis_fmuld8sux16(vis_read_lo(t1), fone);
		}

		if ((mlib_addr)dst & 7) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[i];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				dst[2 * i] = ITOF(s0) * fl_c32;
				dst[2 * i + 1] = ITOF(s1) * fl_c32;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 dst0, res0;
				mlib_f32 s0, s1;

				dst0 = buf[i];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				res0 =
				    vis_freg_pair(ITOF(s0) * fl_c32,
				    ITOF(s1) * fl_c32);
				((mlib_d64 *)dst)[i] = res0;
			}
		}

		nb -= (n & ~7);
		src += (n & ~7);
		dst += (n & ~7);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++)
		dst[i] = src[i] * fl_c8;

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
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_d64 buf[BUFSIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c16, shift - 15);

	while (((mlib_addr)src & 7) && n > 0) {
		dst[0] = src[0] * fl_c16;
		dst++;
		src++;
		n--;
	}

	nb = n;

	for (n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2); n >> 2 > 0; ) {
#pragma pipeloop(0)
		for (i = 0; i < n >> 2; i++) {
			mlib_d64 src0;
			mlib_d64 dst0, dst1;

			src0 = ((mlib_d64 *)src)[i];
			dst0 = vis_fmuld8ulx16(fone, vis_read_hi(src0));
			dst1 = vis_fmuld8ulx16(fone, vis_read_lo(src0));
			buf[2 * i] = dst0;
			buf[2 * i + 1] = dst1;
		}

		if ((mlib_addr)dst & 7) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 dst0;
				mlib_f32 s0, s1;

				dst0 = buf[i];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				dst[2 * i] = ITOF(s0) * fl_c16;
				dst[2 * i + 1] = ITOF(s1) * fl_c16;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 1; i++) {
				mlib_d64 dst0, res0;
				mlib_f32 s0, s1;

				dst0 = buf[i];
				s0 = vis_read_hi(dst0);
				s1 = vis_read_lo(dst0);
				res0 =
				    vis_freg_pair(ITOF(s0) * fl_c16,
				    ITOF(s1) * fl_c16);
				((mlib_d64 *)dst)[i] = res0;
			}
		}

		nb -= (n & ~3);
		src += (n & ~3);
		dst += (n & ~3);
		n = (nb < (BUFSIZE * 2)) ? nb : (BUFSIZE * 2);
	}

	for (i = 0; i < n; i++)
		dst[i] = src[i] * fl_c16;

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
