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

#pragma ident	"@(#)mlib_v_SignalConvertShift_S8.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S8_S32|S8S_S32S]_Sat,
 *      mlib_SignalConvertShift_[S8_S16|S8S_S16S]_Sat,
 *      mlib_SignalConvertShift_[S8_U8|S8S_U8S]_Sat,
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalConvertShift_S8S_S16S_Sat(mlib_s8        *dst,
 *                                                       const mlib_s16 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8S_S32S_Sat(mlib_s8        *dst,
 *                                                       const mlib_s32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8S_U8S_Sat(mlib_s8       *dst,
 *                                                      const mlib_u8 *src,
 *                                                      mlib_s32      shift,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S8_S16_Sat(mlib_s8        *dst,
 *                                                     const mlib_s16 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8_S32_Sat(mlib_s8        *dst,
 *                                                     const mlib_s32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8_U8_Sat(mlib_s8       *dst,
 *                                                    const mlib_u8 *src,
 *                                                    mlib_s32      shift,
 *                                                    mlib_s32      n);
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

#pragma weak mlib_SignalConvertShift_S8_S16_Sat = \
	__mlib_SignalConvertShift_S8_S16_Sat
#pragma weak mlib_SignalConvertShift_S8_S32_Sat = \
	__mlib_SignalConvertShift_S8_S32_Sat
#pragma weak mlib_SignalConvertShift_S8_U8_Sat = \
	__mlib_SignalConvertShift_S8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_S8_S16_Sat)
    mlib_SignalConvertShift_S8_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_S32_Sat)
    mlib_SignalConvertShift_S8_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_U8_Sat)
    mlib_SignalConvertShift_S8_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_U8_Sat(
    mlib_s8 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dx0, dx1, dy0, dy1, dy2, dy3;
	mlib_f32 fzero = vis_read_hi(vis_fzero());
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 8)
		shift = 8;
	ashift = shift - 1;

	if (shift <= -7) {
/* set output to zero */
		return (__mlib_VectorZero_S8(dst, n));
	}

	if (n < 8 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_u8 *psrc_n = (mlib_u8 *)src;
		mlib_s8 *pdst_n = (mlib_s8 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			for (i = 0; i < count; i++) {
				mlib_u32 t = psrc_n[i] << ashift;
				mlib_s32 mask = (0x7f - ((mlib_s32)t)) >> 31;

				pdst_n[i] = (t | mask) & 0x7f;
			}
		}

		n -= count;
		dst = (void *)((mlib_s8 *)dst + count);
		src = (void *)((mlib_u8 *)src + count);
	}

/* pdst is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc) /* && shift <= 0 */) {
		if (shift <= 0) {
			vis_write_gsr((shift + 6) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc[i];
				dy0 = vis_fpmerge(fzero, vis_read_hi(dx0));
				dy1 = vis_fpmerge(fzero, vis_read_lo(dx0));
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		} else {
			vis_write_gsr((shift + 7) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(fzero, vis_read_hi(dx0));
				dy1 = vis_fpmerge(fzero, vis_read_lo(dx0));
				dy2 = vis_fpmerge(fzero, vis_read_hi(dx1));
				dy3 = vis_fpmerge(fzero, vis_read_lo(dx1));

				dx0 = vis_fpackfix_pair(dy0, dy1);
				dx1 = vis_fpackfix_pair(dy2, dy3);
				{
					mlib_d64 m04 =
					    vis_fpmerge(vis_read_hi(dx0),
					    vis_read_hi(dx1));
					mlib_d64 m26 =
					    vis_fpmerge(vis_read_lo(dx0),
					    vis_read_lo(dx1));
					mlib_d64 m06 =
					    vis_fpmerge(vis_read_hi(m04),
					    vis_read_hi(m26));
					mlib_d64 m17 =
					    vis_fpmerge(vis_read_lo(m04),
					    vis_read_lo(m26));

					dx = vis_fpmerge(vis_read_hi(m06),
					    vis_read_hi(m17));
				}

				pdst[i] = dx;
			}
		}
	} else {
		mlib_d64 *psrc_a = (mlib_d64 *)vis_alignaddr(psrc, 0);

		if (shift <= 0) {
			vis_write_gsr((shift + 6) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc_a[i];
				dx1 = psrc_a[i + 1];
				dx0 = vis_faligndata(dx0, dx1);

				dy0 = vis_fpmerge(fzero, vis_read_hi(dx0));
				dy1 = vis_fpmerge(fzero, vis_read_lo(dx0));
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		} else {
			vis_write_gsr((shift + 7) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dy2, dy3;

				dx0 = psrc_a[i];
				dx1 = psrc_a[i + 1];
				dx = vis_faligndata(dx0, dx1);
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(fzero, vis_read_hi(dx0));
				dy1 = vis_fpmerge(fzero, vis_read_lo(dx0));
				dy2 = vis_fpmerge(fzero, vis_read_hi(dx1));
				dy3 = vis_fpmerge(fzero, vis_read_lo(dx1));
				dx0 = vis_fpackfix_pair(dy0, dy1);
				dx1 = vis_fpackfix_pair(dy2, dy3);
				{
					mlib_d64 m04 =
					    vis_fpmerge(vis_read_hi(dx0),
					    vis_read_hi(dx1));
					mlib_d64 m26 =
					    vis_fpmerge(vis_read_lo(dx0),
					    vis_read_lo(dx1));
					mlib_d64 m06 =
					    vis_fpmerge(vis_read_hi(m04),
					    vis_read_hi(m26));
					mlib_d64 m17 =
					    vis_fpmerge(vis_read_lo(m04),
					    vis_read_lo(m26));

					dx = vis_fpmerge(vis_read_hi(m06),
					    vis_read_hi(m17));
				}

				pdst[i] = dx;
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_u8 *psrc_n = (mlib_u8 *)psrc;
		mlib_s8 *pdst_n = (mlib_s8 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			for (; i < n; i++) {
				mlib_u32 t = psrc_n[i] << ashift;
				mlib_s32 mask = (0x7f - ((mlib_s32)t)) >> 31;

				pdst_n[i] = (t | mask) & 0x7f;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_S16_Sat(
    mlib_s8 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dx0, dx1, dy0, dy1;
	mlib_f32 fmulter;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 15)
		shift = 15;

	if (shift < -7)
		shift = -7;
	ashift = shift - 8;

	if (n < 8 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_s16 *psrc_n = (mlib_s16 *)src;
		mlib_s8 *pdst_n = (mlib_s8 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << (shift + 16);
			for (i = 0; i < count; i++) {
				SAT8(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		n -= count;
		dst = (void *)((mlib_s8 *)dst + count);
		src = (void *)((mlib_s16 *)src + count);
	}

/* pdst is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {
		if (shift < 0) {
			fmulter = vis_to_float(0x01010101 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else if (shift == 0) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else if (shift < 14) {
			fmulter = vis_to_float(0x40004000);
			vis_write_gsr((2 + shift) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dz0, dz1, dz2, dz3;

				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dz0 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy0));
				dz1 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy0));
				dz2 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy1));
				dz3 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy1));
				dy0 = vis_fpackfix_pair(dz0, dz1);
				dy1 = vis_fpackfix_pair(dz2, dz3);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else if (shift == 14) {
			fmulter = vis_to_float(0x80008000);
			vis_write_gsr((15) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dz0, dz1, dz2, dz3;

				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dz0 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy0));
				dz1 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy0));
				dz2 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy1));
				dz3 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy1));
				dy0 = vis_fpackfix_pair(dz0, dz1);
				dy1 = vis_fpackfix_pair(dz2, dz3);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = vis_fnot(dx);
			}
		} else {
			mlib_d64 dzero = vis_fzero();
			mlib_d64 dminus = vis_to_double_dup(0x80808080);
			mlib_d64 dplus = vis_fnot(dminus);

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_s32 mm0, mp0;

				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];

				pdst[i] = dzero;
				mm0 = vis_fcmplt16(dy0, dzero);
				mm0 = (mm0 << 4) | (vis_fcmplt16(dy1, dzero));
				vis_pst_8(dminus, pdst + i, mm0);
				mp0 = vis_fcmpgt16(dy0, dzero);
				mp0 = (mp0 << 4) | (vis_fcmpgt16(dy1, dzero));
				vis_pst_8(dplus, pdst + i, mp0);
			}
		}
	} else {
		mlib_d64 *psrc_a = (mlib_d64 *)vis_alignaddr(psrc, 0);
		mlib_d64 prevx;

		if (shift < 0) {
			fmulter = vis_to_float(0x01010101 << (8 + shift));
			prevx = psrc_a[0];
			++psrc_a;
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc_a[2 * i + 0];
				dx1 = psrc_a[2 * i + 1];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				prevx = dx1;
				pdst[i] = dx;
			}
		} else if (shift == 0) {
			prevx = psrc_a[0];
			++psrc_a;
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc_a[2 * i + 0];
				dx1 = psrc_a[2 * i + 1];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				prevx = dx1;
				pdst[i] = dx;
			}
		} else if (shift < 14) {
			fmulter = vis_to_float(0x40004000);
			vis_write_gsr((2 + shift) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dz0, dz1, dz2, dz3;

				prevx = psrc_a[2 * i];
				dx0 = psrc_a[2 * i + 1];
				dx1 = psrc_a[2 * i + 2];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);

				dz0 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy0));
				dz1 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy0));
				dz2 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy1));
				dz3 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy1));
				dy0 = vis_fpackfix_pair(dz0, dz1);
				dy1 = vis_fpackfix_pair(dz2, dz3);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else if (shift == 14) {
			fmulter = vis_to_float(0x80008000);
			vis_write_gsr((15) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dz0, dz1, dz2, dz3;

				prevx = psrc_a[2 * i];
				dx0 = psrc_a[2 * i + 1];
				dx1 = psrc_a[2 * i + 2];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);

				dz0 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy0));
				dz1 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy0));
				dz2 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy1));
				dz3 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy1));
				dy0 = vis_fpackfix_pair(dz0, dz1);
				dy1 = vis_fpackfix_pair(dz2, dz3);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = vis_fnot(dx);
			}
		} else {
			mlib_d64 dzero = vis_fzero();
			mlib_d64 dminus = vis_to_double_dup(0x80808080);
			mlib_d64 dplus = vis_fnot(dminus);

			prevx = psrc_a[0];
			++psrc_a;
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_s32 mm0, mp0;

				dx0 = psrc_a[2 * i + 0];
				dx1 = psrc_a[2 * i + 1];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);
				prevx = dx1;

				pdst[i] = dzero;
				mm0 = vis_fcmplt16(dy0, dzero);
				mm0 = (mm0 << 4) | (vis_fcmplt16(dy1, dzero));
				vis_pst_8(dminus, pdst + i, mm0);
				mp0 = vis_fcmpgt16(dy0, dzero);
				mp0 = (mp0 << 4) | (vis_fcmpgt16(dy1, dzero));
				vis_pst_8(dplus, pdst + i, mp0);
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_s16 *psrc_n = (mlib_s16 *)psrc;
		mlib_s8 *pdst_n = (mlib_s8 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << (shift + 16);
			for (; i < n; i++) {
				SAT8(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_S32_Sat(
    mlib_s8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dy0, dy1, dy2, dy3;
	mlib_f32 fmulter;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 31)
		shift = 31;

	if (shift < -7)
		shift = -7;
	ashift = shift - 24;

	if (n < 7 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)src;
		mlib_s8 *pdst_n = (mlib_s8 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << shift;
			for (i = 0; i < count; i++) {
				SAT8(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		if (n < 8)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_s8 *)dst + count);
		src = (void *)((mlib_s32 *)src + count);
	}

/* pdst    is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {

		if (shift < 0) {
			fmulter = vis_to_float(0x01000100 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				dy2 = vis_fmul8x16(fmulter, dy2);
				dy3 = vis_fmul8x16(fmulter, dy3);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy2));
					mlib_d64 dy15 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_hi(dy1),
					    vis_read_hi(dy3));
					mlib_d64 dy37 =
					    vis_fpmerge(vis_read_lo(dy1),
					    vis_read_lo(dy3));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_hi(dy15),
					    vis_read_hi(dy37));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else if (shift == 0) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy2));
					mlib_d64 dy15 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_hi(dy1),
					    vis_read_hi(dy3));
					mlib_d64 dy37 =
					    vis_fpmerge(vis_read_lo(dy1),
					    vis_read_lo(dy3));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_hi(dy15),
					    vis_read_hi(dy37));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		}
#if MLIB_VIS >= 0x200
		else if (shift < 32) {
#else /* MLIB_VIS >= 0x200 */
		else if (shift < 16) {
#endif /* MLIB_VIS >= 0x200 */
			vis_write_gsr(shift << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				dy0 = vis_fpackfix_pair(dy0, dy1);
				dy2 = vis_fpackfix_pair(dy2, dy3);

				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy2));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else {
			mlib_d64 d_val;

			vis_write_gsr(0 << 3);
			d_val = ((mlib_u32)1) << shift;
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_d64 dz0, dz1, dz2, dz3;

				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[0],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[1],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz2)[0],
				    d_val * ((mlib_s32 *)&dy2)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz2)[1],
				    d_val * ((mlib_s32 *)&dy2)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz3)[0],
				    d_val * ((mlib_s32 *)&dy3)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz3)[1],
				    d_val * ((mlib_s32 *)&dy3)[1]);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dz0),
					    vis_read_hi(dz2));
					mlib_d64 dy15 =
					    vis_fpmerge(vis_read_lo(dz0),
					    vis_read_lo(dz2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_hi(dz1),
					    vis_read_hi(dz3));
					mlib_d64 dy37 =
					    vis_fpmerge(vis_read_lo(dz1),
					    vis_read_lo(dz3));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_hi(dy15),
					    vis_read_hi(dy37));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		}
	} else {
/* if( (mlib_s32)psrc&7 ) */
		mlib_d64 prev;

		psrc = (mlib_d64 *)(((mlib_f32 *)psrc) + 1);

		if (shift < 0) {
			fmulter = vis_to_float(0x01000100 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				prev = psrc[4 * i - 1];
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				prev = vis_fmul8x16(fmulter, prev);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				dy2 = vis_fmul8x16(fmulter, dy2);
				dy3 = vis_fmul8x16(fmulter, dy3);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_lo(prev),
					    vis_read_lo(dy1));
					mlib_d64 dy15 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy2));
					mlib_d64 dy37 =
					    vis_fpmerge(vis_read_hi(dy1),
					    vis_read_hi(dy3));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_hi(dy15),
					    vis_read_hi(dy37));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else if (shift == 0) {

/* replacing code after comments in loop with commented */
/* causes COMPILER ERROR while optimizing by SC5.0 */

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
/* prev = psrc[4*i-1]; */
				mlib_f32 f0, f3;

				f0 = ((mlib_f32 *)psrc)[8 * i - 1];

				dy0 = psrc[4 * i + 0];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
/* dy3 = psrc[4*i+3]; */
				f3 = ((mlib_f32 *)psrc)[8 * i + 6];

				{
/* mlib_d64 dy04 = vis_fpmerge(vis_read_lo(prev), vis_read_lo(dy1)); */
					mlib_d64 dy04 =
					    vis_fpmerge(f0, vis_read_lo(dy1));

					mlib_d64 dy15 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy2));

/* mlib_d64 dy37 = vis_fpmerge(vis_read_hi(dy1), vis_read_hi(dy3)); */
					mlib_d64 dy37 =
					    vis_fpmerge(vis_read_hi(dy1), f3);

					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_hi(dy15),
					    vis_read_hi(dy37));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		}
#if MLIB_VIS >= 0x200
		else if (shift < 32) {
#else /* MLIB_VIS >= 0x200 */
		else if (shift < 16) {
#endif /* MLIB_VIS >= 0x200 */
			vis_write_gsr((shift << 3) + 4);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				prev = psrc[4 * i - 1];
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				prev = vis_faligndata(prev, dy0);
				dy0 = vis_faligndata(dy0, dy1);
				dy1 = vis_faligndata(dy1, dy2);
				dy2 = vis_faligndata(dy2, dy3);
				dy0 = vis_fpackfix_pair(prev, dy0);
				dy2 = vis_fpackfix_pair(dy1, dy2);

				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy2));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_lo(dy04),
					    vis_read_lo(dy26));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		} else {
			mlib_d64 d_val, dz0, dz1, dz2, dz3;

			vis_write_gsr(0 << 3);
			d_val = ((mlib_u32)1) << shift;
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {

				prev = psrc[4 * i - 1];
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[0],
				    d_val * ((mlib_s32 *)&prev)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz0)[1],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[0],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz1)[1],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz2)[0],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz2)[1],
				    d_val * ((mlib_s32 *)&dy2)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz3)[0],
				    d_val * ((mlib_s32 *)&dy2)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dz3)[1],
				    d_val * ((mlib_s32 *)&dy3)[0]);
				{
					mlib_d64 dy04 =
					    vis_fpmerge(vis_read_hi(dz0),
					    vis_read_hi(dz2));
					mlib_d64 dy15 =
					    vis_fpmerge(vis_read_lo(dz0),
					    vis_read_lo(dz2));
					mlib_d64 dy26 =
					    vis_fpmerge(vis_read_hi(dz1),
					    vis_read_hi(dz3));
					mlib_d64 dy37 =
					    vis_fpmerge(vis_read_lo(dz1),
					    vis_read_lo(dz3));
					mlib_d64 dy0246 =
					    vis_fpmerge(vis_read_hi(dy04),
					    vis_read_hi(dy26));
					mlib_d64 dy1357 =
					    vis_fpmerge(vis_read_hi(dy15),
					    vis_read_hi(dy37));

					dx = vis_fpmerge(vis_read_hi(dy0246),
					    vis_read_hi(dy1357));
				}

				pdst[i] = dx;
			}
		}

		psrc = (mlib_d64 *)(((mlib_f32 *)psrc) - 1);
	}

	i <<= 3;

	if (i < n) {
		mlib_s32 *psrc_n = (mlib_s32 *)psrc;
		mlib_s8 *pdst_n = (mlib_s8 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			mlib_d64 d_val = ((mlib_u32)1) << shift;

			for (; i < n; i++) {
				SAT8(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
