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

#pragma ident	"@(#)mlib_v_SignalConvertShift_S32.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S32_S16|S32S_S16S]_Sat,
 *      mlib_SignalConvertShift_[S32_S8|S32S_S8S]_Sat,
 *      mlib_SignalConvertShift_[S32_U8|S32S_U8S]_Sat,
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_S32S_S16S_Sat(mlib_s32       *dst,
 *                                                        const mlib_s16 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32S_S8S_Sat(mlib_s32      *dst,
 *                                                       const mlib_s8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32S_U8S_Sat(mlib_s32      *dst,
 *                                                       const mlib_u8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32_S16_Sat(mlib_s32       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32_S8_Sat(mlib_s32      *dst,
 *                                                     const mlib_s8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32_U8_Sat(mlib_s32      *dst,
 *                                                     const mlib_u8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *
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

#include <mlib_signal.h>
#include <mlib_algebra.h>
#include <vis_proto.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_S32_S16_Sat = \
	__mlib_SignalConvertShift_S32_S16_Sat
#pragma weak mlib_SignalConvertShift_S32_S8_Sat = \
	__mlib_SignalConvertShift_S32_S8_Sat
#pragma weak mlib_SignalConvertShift_S32_U8_Sat = \
	__mlib_SignalConvertShift_S32_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_S32_S16_Sat)
    mlib_SignalConvertShift_S32_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_S8_Sat)
    mlib_SignalConvertShift_S32_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_S8_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_U8_Sat)
    mlib_SignalConvertShift_S32_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_S8_Sat(
    mlib_s32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val, dmulter;
	mlib_d64 *pdst, *psrc;
	mlib_f32 fmulter, fmulter2, fzero = vis_read_hi(vis_fzero());
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 7)
		shift = 7;

	if (shift < -31)
		shift = -31;
	ashift = shift + 24;

	{
		mlib_s32 count = ((n < 8) ? n : (7 & (8 - (mlib_addr)src)));

		if (ashift < 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_s8 *)src)[i] >> (-ashift);
		else if (shift <= 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_s8 *)src)[i] << (ashift);
		else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (i = 0; i < count; ++i) {
				FLOAT2INT_CLAMP(((mlib_s32 *)dst)[i],
				    d_valc * ((mlib_s8 *)src)[i]);
			}
		}

		n -= count;
		psrc = (mlib_d64 *)(((mlib_s8 *)src) + count);
		pdst = (mlib_d64 *)(((mlib_s32 *)dst) + count);
	}

	if (shift <= -24) {
		dmulter = vis_to_double_dup(0x10001 << (shift + 32));
		fmulter = vis_to_float(0x10001);

		if ((mlib_addr)pdst & 7) {
			mlib_d64 *pdst_a =
			    (mlib_d64 *)(8 + (mlib_addr)pdst & ~7);

			vis_alignaddr(psrc, 1);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1;

				dx = psrc[i];
				dx = vis_faligndata(dx, dx);
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);

				dx0 = vis_fmul8sux16(dx0, dmulter);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx0));
				pdst_a[4 * i] = dy0;
				pdst_a[4 * i + 1] = dy1;

				dx1 = vis_fmul8sux16(dx1, dmulter);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx1));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx1));
				pdst_a[4 * i + 2] = dy0;
				((mlib_f32 *)pdst)[8 * i + 0] =
				    vis_read_lo(dy1);
				((mlib_f32 *)pdst)[8 * i + 7] =
				    vis_read_hi(dy1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1;

				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dx0 = vis_fmul8sux16(dx0, dmulter);
				dx1 = vis_fmul8sux16(dx1, dmulter);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx0));
				pdst[4 * i] = dy0;
				pdst[4 * i + 1] = dy1;
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx1));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx1));
				pdst[4 * i + 2] = dy0;
				pdst[4 * i + 3] = dy1;
			}
		}
	} else if (shift < -16) {
		fmulter2 = vis_to_float(0x01010101 << (shift + 24));
		fmulter = vis_to_float(0x00010001);

		if ((mlib_addr)pdst & 4) {
			mlib_d64 *pdst_a =
			    (mlib_d64 *)(8 + (mlib_addr)pdst & ~7);

			vis_alignaddr(psrc, 1);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1;

				dx = psrc[i];
				dx = vis_faligndata(dx, dx);

				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx0 = vis_fmul8x16(fmulter2, dx0);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx0));
				pdst_a[4 * i + 0] = dy0;
				pdst_a[4 * i + 1] = dy1;

				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dx1 = vis_fmul8x16(fmulter2, dx1);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx1));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx1));
				pdst_a[4 * i + 2] = dy0;
				((mlib_f32 *)pdst)[8 * i + 0] =
				    vis_read_lo(dy1);
				((mlib_f32 *)pdst)[8 * i + 7] =
				    vis_read_hi(dy1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1;

				dx = psrc[i];

				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx0 = vis_fmul8x16(fmulter2, dx0);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx0));
				pdst[4 * i + 0] = dy0;
				pdst[4 * i + 1] = dy1;

				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dx1 = vis_fmul8x16(fmulter2, dx1);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx1));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx1));
				pdst[4 * i + 2] = dy0;
				pdst[4 * i + 3] = dy1;
			}
		}
	} else if (shift <= -2) {
		fmulter = vis_to_float(0x00010001 << (shift + 16));

		if ((mlib_addr)pdst & 4) {
			mlib_d64 *pdst_a =
			    (mlib_d64 *)(8 + (mlib_addr)pdst & ~7);

			vis_alignaddr(psrc, 1);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;

				dx = psrc[i];
				dx = vis_faligndata(dx, dx);
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 =
				    vis_fmuld8sux16(vis_read_hi(dx0), fmulter);
				dy1 =
				    vis_fmuld8sux16(vis_read_lo(dx0), fmulter);
				pdst_a[4 * i + 0] = dy0;
				pdst_a[4 * i + 1] = dy1;

				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy2 =
				    vis_fmuld8sux16(vis_read_hi(dx1), fmulter);
				dy3 =
				    vis_fmuld8sux16(vis_read_lo(dx1), fmulter);
				pdst_a[4 * i + 2] = dy2;
				((mlib_f32 *)pdst)[8 * i + 0] =
				    vis_read_lo(dy3);
				((mlib_f32 *)pdst)[8 * i + 7] =
				    vis_read_hi(dy3);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;

				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 =
				    vis_fmuld8sux16(vis_read_hi(dx0), fmulter);
				dy1 =
				    vis_fmuld8sux16(vis_read_lo(dx0), fmulter);
				pdst[4 * i + 0] = dy0;
				pdst[4 * i + 1] = dy1;

				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy2 =
				    vis_fmuld8sux16(vis_read_hi(dx1), fmulter);
				dy3 =
				    vis_fmuld8sux16(vis_read_lo(dx1), fmulter);
				pdst[4 * i + 2] = dy2;
				pdst[4 * i + 3] = dy3;
			}
		}
	} else if (shift < 0) {
		fmulter = vis_to_float(0x01010101 << (shift + 8));

		if ((mlib_addr)pdst & 4) {
			mlib_d64 *pdst_a =
			    (mlib_d64 *)(8 + (mlib_addr)pdst & ~7);

			vis_alignaddr(psrc, 1);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;

				dx = psrc[i];
				dx = vis_faligndata(dx, dx);
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				pdst_a[4 * i + 0] = dy0;
				pdst_a[4 * i + 1] = dy1;
				dy2 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy3 = vis_fpmerge(vis_read_lo(dx1), fzero);
				dy2 = vis_fmul8x16(fmulter, dy2);
				dy3 = vis_fmul8x16(fmulter, dy3);
				pdst_a[4 * i + 2] = dy2;
				((mlib_f32 *)pdst)[8 * i + 0] =
				    vis_read_lo(dy3);
				((mlib_f32 *)pdst)[8 * i + 7] =
				    vis_read_hi(dy3);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;

				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				pdst[4 * i + 0] = dy0;
				pdst[4 * i + 1] = dy1;
				dy2 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy3 = vis_fpmerge(vis_read_lo(dx1), fzero);
				dy2 = vis_fmul8x16(fmulter, dy2);
				dy3 = vis_fmul8x16(fmulter, dy3);
				pdst[4 * i + 2] = dy2;
				pdst[4 * i + 3] = dy3;
			}
		}
	} else if (shift == 0) {
		if ((mlib_addr)pdst & 4) {
			mlib_d64 *pdst_a =
			    (mlib_d64 *)(8 + (mlib_addr)pdst & ~7);

			vis_alignaddr(psrc, 1);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;

				dx = psrc[i];
				dx = vis_faligndata(dx, dx);
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				pdst_a[4 * i + 0] = dy0;
				pdst_a[4 * i + 1] = dy1;
				dy2 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy3 = vis_fpmerge(vis_read_lo(dx1), fzero);
				pdst_a[4 * i + 2] = dy2;
				((mlib_f32 *)pdst)[8 * i + 0] =
				    vis_read_lo(dy3);
				((mlib_f32 *)pdst)[8 * i + 7] =
				    vis_read_hi(dy3);
			}
		} else {
			vis_alignaddr((void *)0, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;

				dx = psrc[i];
#if MLIB_VIS >= 0x200	/* for better code */
				dx = vis_faligndata(dx, dx);
#endif /* MLIB_VIS >= 0x200 ( for better code ) */
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				pdst[4 * i + 0] = dy0;
				pdst[4 * i + 1] = dy1;
				dy2 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy3 = vis_fpmerge(vis_read_lo(dx1), fzero);
				pdst[4 * i + 2] = dy2;
				pdst[4 * i + 3] = dy3;
			}
		}
	} else {	/* if (shift > 0) */

#if MLIB_VIS >= 0x200
		mlib_d64 ss, s0, s1, t0, t1, d0, d1, d2, d3;
		mlib_d64 dmask = vis_to_double_dup(0x00FF00FF);

		vis_write_gsr((shift << 3) | 1);

		if ((mlib_addr)pdst & 7) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				ss = psrc[i];
				s0 = vis_fpmerge(vis_read_hi(ss), fzero);
				s1 = vis_fpmerge(vis_read_lo(ss), fzero);
				d0 = vis_fpmerge(vis_read_hi(s0), fzero);
				d1 = vis_fpmerge(vis_read_lo(s0), fzero);
				d2 = vis_fpmerge(vis_read_hi(s1), fzero);
				d3 = vis_fpmerge(vis_read_lo(s1), fzero);
				s0 = vis_fpackfix_pair(d0, d1);
				s1 = vis_fpackfix_pair(d2, d3);

				t0 = vis_fand(s0, dmask);
				t1 = vis_fand(s1, dmask);
				t0 = vis_for(t0, vis_faligndata(t0, t0));
				t1 = vis_for(t1, vis_faligndata(t1, t1));
				d0 = vis_fpmerge(vis_read_hi(s0),
				    vis_read_hi(t0));
				d1 = vis_fpmerge(vis_read_lo(s0),
				    vis_read_lo(t0));
				d2 = vis_fpmerge(vis_read_hi(s1),
				    vis_read_hi(t1));
				d3 = vis_fpmerge(vis_read_lo(s1),
				    vis_read_lo(t1));

				((mlib_f32 *)pdst)[8 * i] = vis_read_hi(d0);
				((mlib_f32 *)pdst)[8 * i + 1] = vis_read_lo(d0);
				((mlib_f32 *)pdst)[8 * i + 2] = vis_read_hi(d1);
				((mlib_f32 *)pdst)[8 * i + 3] = vis_read_lo(d1);
				((mlib_f32 *)pdst)[8 * i + 4] = vis_read_hi(d2);
				((mlib_f32 *)pdst)[8 * i + 5] = vis_read_lo(d2);
				((mlib_f32 *)pdst)[8 * i + 6] = vis_read_hi(d3);
				((mlib_f32 *)pdst)[8 * i + 7] = vis_read_lo(d3);
			}
		} else {

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				ss = psrc[i];
				s0 = vis_fpmerge(vis_read_hi(ss), fzero);
				s1 = vis_fpmerge(vis_read_lo(ss), fzero);
				d0 = vis_fpmerge(vis_read_hi(s0), fzero);
				d1 = vis_fpmerge(vis_read_lo(s0), fzero);
				d2 = vis_fpmerge(vis_read_hi(s1), fzero);
				d3 = vis_fpmerge(vis_read_lo(s1), fzero);
				s0 = vis_fpackfix_pair(d0, d1);
				s1 = vis_fpackfix_pair(d2, d3);

				t0 = vis_fand(s0, dmask);
				t1 = vis_fand(s1, dmask);
				t0 = vis_for(t0, vis_faligndata(t0, t0));
				t1 = vis_for(t1, vis_faligndata(t1, t1));
				d0 = vis_fpmerge(vis_read_hi(s0),
				    vis_read_hi(t0));
				d1 = vis_fpmerge(vis_read_lo(s0),
				    vis_read_lo(t0));
				d2 = vis_fpmerge(vis_read_hi(s1),
				    vis_read_hi(t1));
				d3 = vis_fpmerge(vis_read_lo(s1),
				    vis_read_lo(t1));

				pdst[4 * i] = d0;
				pdst[4 * i + 1] = d1;
				pdst[4 * i + 2] = d2;
				pdst[4 * i + 3] = d3;
			}
		}

#else /* MLIB_VIS >= 0x200 */
		mlib_d64 dzero = vis_fzero();
		mlib_f32 fmulter1 = vis_to_float(0x1000100);
		mlib_d64 dx, dx0, dx1, dy0, dy1, dz0, dz1;

		fmulter = vis_to_float(0x10001);
		dmulter = vis_to_double_dup(0x1000100);
		d_val = 1u << (shift + 16);

		if (4 & (mlib_addr)pdst) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];

				dx0 = vis_fmul8sux16(dx, dmulter);
				dx1 = vis_fpack32(dx, dzero);
				dy0 =
				    vis_fmuld8sux16(fmulter1, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8sux16(vis_read_hi(dx1), fmulter);

				FLOAT2INT_CLAMP(((mlib_s32 *)pdst)[8 * i],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)pdst)[8 * i + 3],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				*(mlib_d64 *)(((mlib_s32 *)pdst) + 8 * i + 1) =
				    dz0;

				dy0 =
				    vis_fmuld8sux16(fmulter1, vis_read_lo(dx0));
				dy1 =
				    vis_fmuld8sux16(vis_read_lo(dx1), fmulter);

				FLOAT2INT_CLAMP(((mlib_s32 *)pdst)[8 * i + 4],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)pdst)[8 * i + 7],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				*(mlib_d64 *)(((mlib_s32 *)pdst) + 8 * i + 5) =
				    dz0;
			}
		} else {

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];

				dx0 = vis_fmul8sux16(dx, dmulter);
				dx1 = vis_fpack32(dx, dzero);
				dy0 =
				    vis_fmuld8sux16(fmulter1, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8sux16(vis_read_hi(dx1), fmulter);

				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[0],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[1],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				pdst[4 * i] = dz0;
				pdst[4 * i + 1] = dz1;
				dy0 =
				    vis_fmuld8sux16(fmulter1, vis_read_lo(dx0));
				dy1 =
				    vis_fmuld8sux16(vis_read_lo(dx1), fmulter);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[0],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[1],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				pdst[4 * i + 2] = dz0;
				pdst[4 * i + 3] = dz1;
			}
		}

#endif /* MLIB_VIS >= 0x200 */
	}

	i <<= 3;

	if (i < n) {
		mlib_s8 *psrc_n = (mlib_s8 *)psrc;
		mlib_s32 *pdst_n = (mlib_s32 *)pdst;

		if (ashift < 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] << ashift;
			}
		} else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (; i < n; i++) {
				FLOAT2INT_CLAMP(pdst_n[i], d_valc * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_U8_Sat(
    mlib_s32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	mlib_d64 *pdst, *psrc;
	mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;
	mlib_f32 fmulter, fmulter2, fzero = vis_read_hi(vis_fzero());
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 8)
		shift = 8;
	ashift = shift + 23;

/* set output to zero */
	if (shift <= -31) {
		return (__mlib_VectorZero_S32(dst, n));
	}

	if (shift > 0) {
		d64_2x32 s0, s1, s2, s3;
		mlib_d64 d0, d1, d2, d3;

		d_val = 1u << (shift + 23);
		dx = vis_to_double_dup(0);

#pragma pipeloop(0)
		for (i = 0; i < n - 4; i += 4) {
			s0.d64 = vis_ld_u8_i((void *)src, 0);
			s1.d64 = vis_ld_u8_i((void *)src, 1);
			s2.d64 = vis_ld_u8_i((void *)src, 2);
			s3.d64 = vis_ld_u8_i((void *)src, 3);
			d0 = d_val * s0.i32s.f1;
			d1 = d_val * s1.i32s.f1;
			d2 = d_val * s2.i32s.f1;
			d3 = d_val * s3.i32s.f1;
#ifndef MLIB_USE_FTOI_CLAMPING

			if (d0 > (mlib_d64)MLIB_S32_MAX)
				d0 = (mlib_d64)MLIB_S32_MAX;
			if (d1 > (mlib_d64)MLIB_S32_MAX)
				d1 = (mlib_d64)MLIB_S32_MAX;
			if (d2 > (mlib_d64)MLIB_S32_MAX)
				d2 = (mlib_d64)MLIB_S32_MAX;
			if (d3 > (mlib_d64)MLIB_S32_MAX)
				d3 = (mlib_d64)MLIB_S32_MAX;
#endif /* MLIB_USE_FTOI_CLAMPING */
			dst[0] = (mlib_s32)d0;
			dst[1] = (mlib_s32)d1;
			dst[2] = (mlib_s32)d2;
			dst[3] = (mlib_s32)d3;
			dst += 4;
			src += 4;
		}

		for (; i < n; i++) {
			s0.d64 = vis_ld_u8_i((void *)src, 0);
			d0 = d_val * s0.i32s.f1;
#ifndef MLIB_USE_FTOI_CLAMPING

			if (d0 > (mlib_d64)MLIB_S32_MAX)
				d0 = (mlib_d64)MLIB_S32_MAX;
#endif /* MLIB_USE_FTOI_CLAMPING */
			(*dst++) = d0;
			src++;
		}

		return (MLIB_SUCCESS);
	}

	if (n < 8 || (mlib_addr)src & 7) {
		mlib_s32 count;
		mlib_u8 *psrc_n = (mlib_u8 *)src;
		mlib_s32 *pdst_n = (mlib_s32 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)src) & 7);
		for (i = 0; i < count; i++) {
			pdst_n[i] =
			    ((ashift >=
			    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
			    (-ashift)));
		}

		if (n < 8)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_s32 *)dst + count);
		src = (void *)((mlib_u8 *)src + count);
	}

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!((mlib_addr)pdst & 7)) {
		if (shift < -23) {
			fmulter2 = vis_to_float(0x0001 << (31 + shift));
			fmulter = vis_to_float(0x10001);
			vis_alignaddr(pdst, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fmul8x16al(vis_read_hi(dx), fmulter2);
				dx1 = vis_fmul8x16al(vis_read_lo(dx), fmulter2);

				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx0));
				dy2 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx1));
				dy3 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx1));

#if MLIB_VIS >= 0x200	/* for better code */
				pdst[4 * i + 0] = vis_faligndata(dy0, dy1);
				pdst[4 * i + 1] = vis_faligndata(dy1, dy2);
				pdst[4 * i + 2] = vis_faligndata(dy2, dy3);
				pdst[4 * i + 3] = vis_faligndata(dy3, dy3);
#else /* MLIB_VIS >= 0x200 ( for better code ) */
				pdst[4 * i + 0] = dy0;
				pdst[4 * i + 1] = dy1;
				pdst[4 * i + 2] = dy2;
				pdst[4 * i + 3] = dy3;
#endif /* MLIB_VIS >= 0x200 ( for better code ) */
			}
		} else if (shift < -8) {
			fmulter = vis_to_float(0x10001 << (23 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(fzero, vis_read_hi(dx));
				dx1 = vis_fpmerge(fzero, vis_read_lo(dx));
				pdst[4 * i + 0] =
				    vis_fmuld8ulx16(vis_read_hi(dx0), fmulter);
				pdst[4 * i + 1] =
				    vis_fmuld8ulx16(vis_read_lo(dx0), fmulter);
				pdst[4 * i + 2] =
				    vis_fmuld8ulx16(vis_read_hi(dx1), fmulter);
				pdst[4 * i + 3] =
				    vis_fmuld8ulx16(vis_read_lo(dx1), fmulter);
			}
		} else {	/* if (shift <= 0) */

			mlib_d64 dzero = vis_fzero();

			if (shift < 0) {
				vis_write_gsr(1);
				fmulter = vis_to_float(0x10001 << (15 + shift));
			} else {
				vis_write_gsr(2);
				fmulter = vis_to_float(0x10001 << (7 + shift));
			}

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(fzero, vis_read_hi(dx));
				dx1 = vis_fpmerge(fzero, vis_read_lo(dx));

				dy0 =
				    vis_fmuld8ulx16(vis_read_hi(dx0), fmulter);
				dy1 =
				    vis_fmuld8ulx16(vis_read_lo(dx0), fmulter);
				dy2 =
				    vis_fmuld8ulx16(vis_read_hi(dx1), fmulter);
				dy3 =
				    vis_fmuld8ulx16(vis_read_lo(dx1), fmulter);
				pdst[4 * i + 0] = vis_faligndata(dy0, dzero);
				pdst[4 * i + 1] = vis_faligndata(dy1, dzero);
				pdst[4 * i + 2] = vis_faligndata(dy2, dzero);
#if MLIB_VIS >= 0x200	/* for better code */
				dy3 = vis_faligndata(dy3, dzero);
				((mlib_f32 *)pdst)[8 * i + 6] =
				    vis_read_hi(dy3);
				((mlib_f32 *)pdst)[8 * i + 7] =
				    vis_read_lo(dy3);
#else /* MLIB_VIS >= 0x200 ( for better code ) */
				pdst[4 * i + 3] = vis_faligndata(dy3, dzero);
#endif /* MLIB_VIS >= 0x200 ( for better code ) */
			}
		}
	} else {
		pdst = ((mlib_d64 *)vis_alignaddr(pdst, 0)) + 1;

		if (shift < -23) {
			fmulter2 = vis_to_float(0x1 << (31 + shift));
			fmulter = vis_to_float(0x10001);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fmul8x16al(vis_read_hi(dx), fmulter2);
				dx1 = vis_fmul8x16al(vis_read_lo(dx), fmulter2);

				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx0));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx0));
				dy2 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dx1));
				dy3 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dx1));
				((mlib_f32 *)pdst)[8 * i - 1] =
				    vis_read_hi(dy0);
				pdst[4 * i + 0] = vis_faligndata(dy0, dy1);
				pdst[4 * i + 1] = vis_faligndata(dy1, dy2);
				pdst[4 * i + 2] = vis_faligndata(dy2, dy3);
				((mlib_f32 *)pdst)[8 * i + 6] =
				    vis_read_lo(dy3);
			}
		} else if (shift < -8) {
			fmulter = vis_to_float(0x10001 << (23 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(fzero, vis_read_hi(dx));
				dx1 = vis_fpmerge(fzero, vis_read_lo(dx));

				dy0 =
				    vis_fmuld8ulx16(vis_read_hi(dx0), fmulter);
				dy1 =
				    vis_fmuld8ulx16(vis_read_lo(dx0), fmulter);
				dy2 =
				    vis_fmuld8ulx16(vis_read_hi(dx1), fmulter);
				dy3 =
				    vis_fmuld8ulx16(vis_read_lo(dx1), fmulter);
				((mlib_f32 *)pdst)[8 * i - 1] =
				    vis_read_hi(dy0);
				pdst[4 * i + 0] = vis_faligndata(dy0, dy1);
				pdst[4 * i + 1] = vis_faligndata(dy1, dy2);
				pdst[4 * i + 2] = vis_faligndata(dy2, dy3);
				((mlib_f32 *)pdst)[8 * i + 6] =
				    vis_read_lo(dy3);
			}
		} else if (shift < 0) {
			vis_write_gsr(5);
			fmulter = vis_to_float(0x10001 << (15 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(fzero, vis_read_hi(dx));
				dx1 = vis_fpmerge(fzero, vis_read_lo(dx));

				dy0 =
				    vis_fmuld8ulx16(vis_read_hi(dx0), fmulter);
				dy1 =
				    vis_fmuld8ulx16(vis_read_lo(dx0), fmulter);
				dy2 =
				    vis_fmuld8ulx16(vis_read_hi(dx1), fmulter);
				dy3 =
				    vis_fmuld8ulx16(vis_read_lo(dx1), fmulter);
				((mlib_f32 *)pdst)[8 * i - 1] =
				    vis_read_lo(vis_faligndata(dy0, dy0));
				pdst[4 * i + 0] = vis_faligndata(dy0, dy1);
				pdst[4 * i + 1] = vis_faligndata(dy1, dy2);
				pdst[4 * i + 2] = vis_faligndata(dy2, dy3);
				((mlib_f32 *)pdst)[8 * i + 6] =
				    vis_read_hi(vis_faligndata(dy3, dy3));
			}
		} else if (shift == 0) {
			vis_write_gsr(6);
			fmulter = vis_to_float(0x00800080);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(fzero, vis_read_hi(dx));
				dx1 = vis_fpmerge(fzero, vis_read_lo(dx));

				dy0 =
				    vis_fmuld8ulx16(vis_read_hi(dx0), fmulter);
				dy1 =
				    vis_fmuld8ulx16(vis_read_lo(dx0), fmulter);
				dy2 =
				    vis_fmuld8ulx16(vis_read_hi(dx1), fmulter);
				dy3 =
				    vis_fmuld8ulx16(vis_read_lo(dx1), fmulter);
				((mlib_f32 *)pdst)[8 * i - 1] =
				    vis_read_lo(vis_faligndata(dy0, dy0));
				pdst[4 * i + 0] = vis_faligndata(dy0, dy1);
				pdst[4 * i + 1] = vis_faligndata(dy1, dy2);
				pdst[4 * i + 2] = vis_faligndata(dy2, dy3);
				((mlib_f32 *)pdst)[8 * i + 6] =
				    vis_read_hi(vis_faligndata(dy3, dy3));
			}
		}

		pdst = (mlib_d64 *)(((mlib_f32 *)pdst) - 1);
	}

	i <<= 3;

	if (i < n) {
		mlib_u8 *psrc_n = (mlib_u8 *)src;
		mlib_s32 *pdst_n = (mlib_s32 *)dst;

		for (; i < n; i++) {
			pdst_n[i] =
			    ((ashift >=
			    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
			    (-ashift)));
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_S16_Sat(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dmulter;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dy0, dy1;
	mlib_f32 fmulter, fmulter2;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 15)
		shift = 15;

	if (shift < -32)
		shift = -32;
	ashift = shift + 16;

	if (n < 7 || (mlib_addr)src & 7) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s16 *psrc_n = (mlib_s16 *)src;
		mlib_s32 *pdst_n = (mlib_s32 *)dst;

		count = (n < 7) ? n : ((8 - (mlib_addr)src) & 7) >> 1;

		if (shift <= 0) {
			if (ashift >= 0)
				for (i = 0; i < count; i++) {
					pdst_n[i] = psrc_n[i] << ashift;
			} else
				for (i = 0; i < count; i++) {
					pdst_n[i] = psrc_n[i] >> (-ashift);
				}
		} else {
			d_val = ((mlib_u32)1) << ashift;
			for (i = 0; i < count; i++) {
				FLOAT2INT_CLAMP(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		if (n < 7)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_s32 *)dst + count);
		src = (void *)((mlib_s16 *)src + count);
	}

/* psrc is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)pdst)) {

		if (ashift < -8) {
			dmulter =
			    vis_to_double_dup(0x00010001 << (ashift + 16));
			fmulter = vis_to_float(0x00010001);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dx = vis_fmul8ulx16(dmulter, dx);
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift < 0) {
			fmulter2 = vis_to_float(0x01010101 << (ashift + 8));
			fmulter = vis_to_float(0x00010001);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dx = vis_fmul8x16(fmulter2, dx);
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift < 8) {
			fmulter = vis_to_float(0x00010001 << ashift);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift < 15) {
			fmulter = vis_to_float(0x00010001 << ashift);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8sux16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8sux16(fmulter, vis_read_lo(dx));
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift == 15) {
			fmulter = vis_to_float(0x40004000);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8sux16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8sux16(fmulter, vis_read_lo(dx));
				dy0 = vis_fpadd32(dy0, dy0);
				dy1 = vis_fpadd32(dy1, dy1);
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift == 16) {
			fmulter = vis_to_float(0x40004000);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8sux16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8sux16(fmulter, vis_read_lo(dx));
				dy0 = vis_fpadd32(dy0, dy0);
				dy1 = vis_fpadd32(dy1, dy1);
				dy0 = vis_fpadd32(dy0, dy0);
				dy1 = vis_fpadd32(dy1, dy1);
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else {	/* if (ashift > 16) */

			mlib_d64 d_val, dz0, dz1;

			fmulter = vis_to_float(0x00010001);
			d_val = ((mlib_u32)1) << ashift;
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[0],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[1],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				pdst[2 * i] = dz0;
				pdst[2 * i + 1] = dz1;
			}
		}
	} else {
/* if( (mlib_s32)pdst&7 ) */

		if (ashift < -8) {
			dmulter =
			    vis_to_double_dup(0x00010001 << (ashift + 16));
			fmulter = vis_to_float(0x00010001);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dx = vis_fmul8ulx16(dmulter, dx);
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(dy0);
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(dy0);
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(dy1);
				((mlib_f32 *)pdst)[4 * i + 3] =
				    vis_read_lo(dy1);
			}
		} else if (ashift < 0) {
			fmulter2 = vis_to_float(0x01010101 << (ashift + 8));
			fmulter = vis_to_float(0x00010001);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dx = vis_fmul8x16(fmulter2, dx);
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(dy0);
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(dy0);
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(dy1);
				((mlib_f32 *)pdst)[4 * i + 3] =
				    vis_read_lo(dy1);
			}
		} else if (ashift < 8) {
			fmulter = vis_to_float(0x00010001 << ashift);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(dy0);
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(dy0);
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(dy1);
				((mlib_f32 *)pdst)[4 * i + 3] =
				    vis_read_lo(dy1);
			}
		} else if (ashift < 15) {
			fmulter = vis_to_float(0x00010001 << ashift);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8sux16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8sux16(fmulter, vis_read_lo(dx));
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(dy0);
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(dy0);
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(dy1);
				((mlib_f32 *)pdst)[4 * i + 3] =
				    vis_read_lo(dy1);
			}
		} else if (ashift == 15) {
			fmulter = vis_to_float(0x40004000);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8sux16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8sux16(fmulter, vis_read_lo(dx));
				dy0 = vis_fpadd32(dy0, dy0);
				dy1 = vis_fpadd32(dy1, dy1);
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(dy0);
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(dy0);
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(dy1);
				((mlib_f32 *)pdst)[4 * i + 3] =
				    vis_read_lo(dy1);
			}
		} else if (ashift == 16) {
			fmulter = vis_to_float(0x40004000);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8sux16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8sux16(fmulter, vis_read_lo(dx));
				dy0 = vis_fpadd32(dy0, dy0);
				dy1 = vis_fpadd32(dy1, dy1);
				dy0 = vis_fpadd32(dy0, dy0);
				dy1 = vis_fpadd32(dy1, dy1);
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(dy0);
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(dy0);
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(dy1);
				((mlib_f32 *)pdst)[4 * i + 3] =
				    vis_read_lo(dy1);
			}
		} else {	/* if (ashift > 16) */

			mlib_d64 d_val, dz;

			fmulter = vis_to_float(0x00010001);
			d_val = ((mlib_u32)1) << ashift;
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dx = psrc[i];
				dy0 = vis_fmuld8ulx16(fmulter, vis_read_hi(dx));
				dy1 = vis_fmuld8ulx16(fmulter, vis_read_lo(dx));

				FLOAT2INT_CLAMP(((mlib_s32 *)&dz)[0],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz)[1],
				    d_val * ((mlib_s32 *)&dy1)[0]);

				FLOAT2INT_CLAMP(((mlib_s32 *)pdst)[4 * i + 0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				*(mlib_d64 *)(((mlib_f32 *)pdst) + 4 * i + 1) =
				    dz;
				FLOAT2INT_CLAMP(((mlib_s32 *)pdst)[4 * i + 3],
				    d_val * ((mlib_s32 *)&dy1)[1]);
			}
		}
	}

	if (n & 3) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s16 *psrc_n = (mlib_s16 *)&psrc[i];
		mlib_s32 *pdst_n = (mlib_s32 *)&pdst[2 * i];

		count = n & 3;

		if (shift <= 0) {
			if (ashift >= 0)
				for (i = 0; i < count; i++) {
					pdst_n[i] = psrc_n[i] << ashift;
			} else
				for (i = 0; i < count; i++) {
					pdst_n[i] = psrc_n[i] >> (-ashift);
				}
		} else {
			d_val = ((mlib_u32)1) << ashift;
			for (i = 0; i < count; i++) {
				FLOAT2INT_CLAMP(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
