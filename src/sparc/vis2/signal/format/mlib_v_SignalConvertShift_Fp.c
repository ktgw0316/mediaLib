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

#define	CONVERT_U8_F32()                                            \
	sd0.value = sp[0];                                          \
	sd1.value = sp[1];                                          \
	sd2.value = sp[2];                                          \
	sd3.value = sp[3];                                          \
	sp += 4;                                                    \
	sd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0 * fl_c);   \
	sd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1 * fl_c);   \
	sd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0 * fl_c);   \
	sd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1 * fl_c);   \
	sd2.twoint.int0 = (mlib_s32)(sd2.twofloat.float0 * fl_c);   \
	sd2.twoint.int1 = (mlib_s32)(sd2.twofloat.float1 * fl_c);   \
	sd3.twoint.int0 = (mlib_s32)(sd3.twofloat.float0 * fl_c);   \
	sd3.twoint.int1 = (mlib_s32)(sd3.twofloat.float1 * fl_c);   \
	dd = vis_fpack32(sd0.value, sd0.value);                     \
	dd = vis_fpack32(dd, sd1.value);                            \
	dd = vis_fpack32(dd, sd2.value);                            \
	dd = vis_fpack32(dd, sd3.value);                            \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_F32_Sat(
    mlib_u8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 xsize)
{
	mlib_s32 i, off;
	mlib_d64 *sp, *dp;
	mlib_d64 dd, dd_old;
	type_union_mlib_d64 sd0, sd1, sd2, sd3;
	mlib_f32 fl_c;

	if (xsize <= 0)
		return (MLIB_FAILURE);
	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 8);

	if ((mlib_addr)src & 7) {
		mlib_f32 x = (*src++) * fl_c;

		if (x >= MLIB_U8_MAX)
			x = MLIB_U8_MAX;
		if (x <= MLIB_U8_MIN)
			x = MLIB_U8_MIN;
		(*dst++) = x;
		xsize--;
	}

	vis_write_gsr(23 << 3);

	off = ((mlib_addr)dst & 7);
	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)(dst - off);

	if (off == 0) {
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			CONVERT_U8_F32();
			(*dp++) = dd;
		}

		if (i < xsize) {
			mlib_s32 emask = 0xFF00 >> (xsize - i);

			CONVERT_U8_F32();
			vis_pst_8(dd, dp, emask);
		}
	} else {
		mlib_s32 emask = vis_edge8(dst, dst + xsize - 1);
		vis_alignaddrl((void *)0, off);
		xsize += off - 8;

		CONVERT_U8_F32();
		dd_old = vis_faligndata(dd, dd);
		vis_pst_8(dd_old, dp, emask);
		dp++;
		dd_old = dd;

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			CONVERT_U8_F32();
			(*dp++) = vis_faligndata(dd_old, dd);
			dd_old = dd;
		}

		if (i < xsize) {
			mlib_s32 emask = 0xFF00 >> (xsize - i);

			CONVERT_U8_F32();
			dd = vis_faligndata(dd_old, dd);
			vis_pst_8(dd, dp, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	CONVERT_S8_F32()                                        \
	sd0.value = sp[0];                                      \
	sd1.value = sp[1];                                      \
	sd2.value = sp[2];                                      \
	sd3.value = sp[3];                                      \
	sp += 4;                                                \
	sd0.twoint.int0 =                                       \
	    (mlib_s32)(sd0.twofloat.float0 * fl_c + sat_off);   \
	sd0.twoint.int1 =                                       \
	    (mlib_s32)(sd0.twofloat.float1 * fl_c + sat_off);   \
	sd1.twoint.int0 =                                       \
	    (mlib_s32)(sd1.twofloat.float0 * fl_c + sat_off);   \
	sd1.twoint.int1 =                                       \
	    (mlib_s32)(sd1.twofloat.float1 * fl_c + sat_off);   \
	sd2.twoint.int0 =                                       \
	    (mlib_s32)(sd2.twofloat.float0 * fl_c + sat_off);   \
	sd2.twoint.int1 =                                       \
	    (mlib_s32)(sd2.twofloat.float1 * fl_c + sat_off);   \
	sd3.twoint.int0 =                                       \
	    (mlib_s32)(sd3.twofloat.float0 * fl_c + sat_off);   \
	sd3.twoint.int1 =                                       \
	    (mlib_s32)(sd3.twofloat.float1 * fl_c + sat_off);   \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd));     \
	dd = vis_fxor(dd, mask_80)

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_F32_Sat(
    mlib_s8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 xsize)
{
	mlib_s32 i, off;
	mlib_d64 *sp, *dp;
	mlib_d64 dd, dd_old;
	type_union_mlib_d64 sd0, sd1, sd2, sd3;
	mlib_f32 fl_c;
	mlib_f32 sat_off = 128.f;
	mlib_d64 mask_80 = vis_to_double_dup(0x80808080);

	if (xsize <= 0)
		return (MLIB_FAILURE);
	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 7);

	if ((mlib_addr)src & 7) {
		mlib_f32 x = (*src++) * fl_c;

		if (x >= MLIB_S8_MAX)
			x = MLIB_S8_MAX;
		if (x <= MLIB_S8_MIN)
			x = MLIB_S8_MIN;
		(*dst++) = x;
		xsize--;
	}

	vis_write_gsr(23 << 3);

	off = ((mlib_addr)dst & 7);
	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)((mlib_u8 *)dst - off);

	if (off == 0) {
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			CONVERT_S8_F32();
			(*dp++) = dd;
		}

		if (i < xsize) {
			mlib_s32 emask = 0xFF00 >> (xsize - i);

			CONVERT_S8_F32();
			vis_pst_8(dd, dp, emask);
		}
	} else {
		mlib_s32 emask = vis_edge8(dst, dst + xsize - 1);
		vis_alignaddrl((void *)0, off);
		xsize += off - 8;

		CONVERT_S8_F32();
		dd_old = vis_faligndata(dd, dd);
		vis_pst_8(dd_old, dp, emask);
		dp++;
		dd_old = dd;

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			CONVERT_S8_F32();
			(*dp++) = vis_faligndata(dd_old, dd);
			dd_old = dd;
		}

		if (i < xsize) {
			mlib_s32 emask = 0xFF00 >> (xsize - i);

			CONVERT_S8_F32();
			dd = vis_faligndata(dd_old, dd);
			vis_pst_8(dd, dp, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	CONVERT_S16_F32()                                           \
	sd0.value = sp[0];                                          \
	sd1.value = sp[1];                                          \
	sp += 2;                                                    \
	sd0.twoint.int0 = (mlib_s32)(sd0.twofloat.float0 * fl_c);   \
	sd0.twoint.int1 = (mlib_s32)(sd0.twofloat.float1 * fl_c);   \
	sd1.twoint.int0 = (mlib_s32)(sd1.twofloat.float0 * fl_c);   \
	sd1.twoint.int1 = (mlib_s32)(sd1.twofloat.float1 * fl_c);   \
	dd = vis_fpackfix_pair(sd0.value, sd1.value)

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_F32_Sat(
    mlib_s16 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 xsize)
{
	mlib_s32 i, off;
	mlib_d64 *sp, *dp;
	mlib_d64 dd, dd_old;
	type_union_mlib_d64 sd0, sd1;
	mlib_f32 fl_c;

	if (xsize <= 0)
		return (MLIB_FAILURE);
	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 15);

	if ((mlib_addr)src & 7) {
		mlib_f32 x = (*src++) * fl_c;

		if (x >= MLIB_S16_MAX)
			x = MLIB_S16_MAX;
		if (x <= MLIB_S16_MIN)
			x = MLIB_S16_MIN;
		(*dst++) = x;
		xsize--;
	}

	vis_write_gsr(16 << 3);

	off = ((mlib_addr)dst & 7);
	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)((mlib_u8 *)dst - off);

	xsize *= 2;

	if (off == 0) {
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			CONVERT_S16_F32();
			(*dp++) = dd;
		}

		if (i < xsize) {
			mlib_s32 emask = 0xFF00 >> (xsize - i);

			CONVERT_S16_F32();
			vis_pst_8(dd, dp, emask);
		}
	} else {
		mlib_s32 emask = vis_edge16(dst, (mlib_u8 *)dst + xsize - 2);
		vis_alignaddrl((void *)0, off);
		xsize += off - 8;

		CONVERT_S16_F32();
		dd_old = vis_faligndata(dd, dd);
		vis_pst_16(dd_old, dp, emask);
		dp++;
		dd_old = dd;

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			CONVERT_S16_F32();
			(*dp++) = vis_faligndata(dd_old, dd);
			dd_old = dd;
		}

		if (i < xsize) {
			mlib_s32 emask = 0xFF00 >> (xsize - i);

			CONVERT_S16_F32();
			dd = vis_faligndata(dd_old, dd);
			vis_pst_8(dd, dp, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	CONVERT_F32_U8()                                        \
	d0 = vis_fpmerge(fzero, vis_read_hi(ss));               \
	d1 = vis_fpmerge(fzero, vis_read_lo(ss));               \
	dd0.d64 = vis_fmuld8ulx16(fone, vis_read_hi(d0));       \
	dd1.d64 = vis_fmuld8ulx16(fone, vis_read_lo(d0));       \
	dd2.d64 = vis_fmuld8ulx16(fone, vis_read_hi(d1));       \
	dd3.d64 = vis_fmuld8ulx16(fone, vis_read_lo(d1));       \
	dd0.f32s.f0 = (mlib_f32)(dd0.i32s.f0) * fl_c;           \
	dd0.f32s.f1 = (mlib_f32)(dd0.i32s.f1) * fl_c;           \
	dd1.f32s.f0 = (mlib_f32)(dd1.i32s.f0) * fl_c;           \
	dd1.f32s.f1 = (mlib_f32)(dd1.i32s.f1) * fl_c;           \
	dd2.f32s.f0 = (mlib_f32)(dd2.i32s.f0) * fl_c;           \
	dd2.f32s.f1 = (mlib_f32)(dd2.i32s.f1) * fl_c;           \
	dd3.f32s.f0 = (mlib_f32)(dd3.i32s.f0) * fl_c;           \
	dd3.f32s.f1 = (mlib_f32)(dd3.i32s.f1) * fl_c;           \
	dp[0] = dd0.d64;                                        \
	dp[1] = dd1.d64;                                        \
	dp[2] = dd2.d64;                                        \
	dp[3] = dd3.d64;                                        \
	dp += 4

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_U8(
    mlib_f32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 xsize)
{
	mlib_s32 i;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, s0, s1, d0, d1;
	d64_2x32 dd0, dd1, dd2, dd3;
	mlib_f32 fl_c;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 fone = vis_to_float(0x00010001);

	if (xsize <= 0)
		return (MLIB_FAILURE);
	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift - 8);

	if ((mlib_addr)dst & 7) {
		(*dst++) = (*src++) * fl_c;
		xsize--;
	}

	sp = vis_alignaddr((void *)src, 0);
	dp = (mlib_d64 *)dst;

	if (sp == (void *)src) {
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			ss = (*sp++);
			CONVERT_F32_U8();
		}
	} else {
		s1 = (*sp++);

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			s0 = s1;
			s1 = (*sp++);
			ss = vis_faligndata(s0, s1);
			CONVERT_F32_U8();
		}
	}

	for (; i < xsize; i++) {
		dst[i] = (mlib_f32)src[i] * fl_c;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	CONVERT_F32_S8()                                        \
	d0 = vis_fpmerge(vis_read_hi(ss), fzero);               \
	d1 = vis_fpmerge(vis_read_lo(ss), fzero);               \
	dd0.d64 = vis_fmuld8ulx16(fone, vis_read_hi(d0));       \
	dd1.d64 = vis_fmuld8ulx16(fone, vis_read_lo(d0));       \
	dd2.d64 = vis_fmuld8ulx16(fone, vis_read_hi(d1));       \
	dd3.d64 = vis_fmuld8ulx16(fone, vis_read_lo(d1));       \
	dd0.f32s.f0 = (mlib_f32)(dd0.i32s.f0) * fl_c;           \
	dd0.f32s.f1 = (mlib_f32)(dd0.i32s.f1) * fl_c;           \
	dd1.f32s.f0 = (mlib_f32)(dd1.i32s.f0) * fl_c;           \
	dd1.f32s.f1 = (mlib_f32)(dd1.i32s.f1) * fl_c;           \
	dd2.f32s.f0 = (mlib_f32)(dd2.i32s.f0) * fl_c;           \
	dd2.f32s.f1 = (mlib_f32)(dd2.i32s.f1) * fl_c;           \
	dd3.f32s.f0 = (mlib_f32)(dd3.i32s.f0) * fl_c;           \
	dd3.f32s.f1 = (mlib_f32)(dd3.i32s.f1) * fl_c;           \
	dp[0] = dd0.d64;                                        \
	dp[1] = dd1.d64;                                        \
	dp[2] = dd2.d64;                                        \
	dp[3] = dd3.d64;                                        \
	dp += 4

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S8(
    mlib_f32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 xsize)
{
	mlib_s32 i;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, s0, s1, d0, d1;
	d64_2x32 dd0, dd1, dd2, dd3;
	mlib_f32 fl_c;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 fone = vis_to_float(0x00010001);

	if (xsize <= 0)
		return (MLIB_FAILURE);
	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift - 7);

	if ((mlib_addr)dst & 7) {
		(*dst++) = (*src++) * fl_c;
		xsize--;
	}

/* S8 like S16 */
	fl_c *= 1.f / 256;

	sp = vis_alignaddr((void *)src, 0);
	dp = (mlib_d64 *)dst;

	if (sp == (void *)src) {
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			ss = (*sp++);
			CONVERT_F32_S8();
		}
	} else {
		s1 = (*sp++);

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			s0 = s1;
			s1 = (*sp++);
			ss = vis_faligndata(s0, s1);
			CONVERT_F32_S8();
		}
	}

/* restore */
	fl_c *= 256.f;

	for (; i < xsize; i++) {
		dst[i] = (mlib_f32)src[i] * fl_c;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	CONVERT_F32_S16()                                       \
	dd0.d64 = vis_fmuld8ulx16(fone, vis_read_hi(ss));       \
	dd1.d64 = vis_fmuld8ulx16(fone, vis_read_lo(ss));       \
	dd0.f32s.f0 = (mlib_f32)(dd0.i32s.f0) * fl_c;           \
	dd0.f32s.f1 = (mlib_f32)(dd0.i32s.f1) * fl_c;           \
	dd1.f32s.f0 = (mlib_f32)(dd1.i32s.f0) * fl_c;           \
	dd1.f32s.f1 = (mlib_f32)(dd1.i32s.f1) * fl_c;           \
	dp[0] = dd0.d64;                                        \
	dp[1] = dd1.d64;                                        \
	dp += 2

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S16(
    mlib_f32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 xsize)
{
	mlib_s32 i;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, s0, s1;
	d64_2x32 dd0, dd1;
	mlib_f32 fl_c;
	mlib_f32 fone = vis_to_float(0x00010001);

	if (xsize <= 0)
		return (MLIB_FAILURE);
	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift - 15);

	if ((mlib_addr)dst & 7) {
		(*dst++) = (*src++) * fl_c;
		xsize--;
	}

	sp = vis_alignaddr((void *)src, 0);
	dp = (mlib_d64 *)dst;

	if (sp == (void *)src) {
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			ss = (*sp++);
			CONVERT_F32_S16();
		}
	} else {
		s1 = (*sp++);

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			s0 = s1;
			s1 = (*sp++);
			ss = vis_faligndata(s0, s1);
			CONVERT_F32_S16();
		}
	}

	for (; i < xsize; i++) {
		dst[i] = (mlib_f32)src[i] * fl_c;
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
