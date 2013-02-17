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

#pragma ident	"@(#)mlib_v_SignalConvertShift_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[U8_S32|U8S_S32S]_Sat,
 *      mlib_SignalConvertShift_[U8_S16|U8S_S16S]_Sat,
 *      mlib_SignalConvertShift_[U8_S8|U8S_S8S]_Sat,
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalConvertShift_U8S_S16S_Sat(mlib_u8        *dst,
 *                                                       const mlib_s16 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_S32S_Sat(mlib_u8        *dst,
 *                                                       const mlib_s32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_S8S_Sat(mlib_u8       *dst,
 *                                                      const mlib_s8 *src,
 *                                                      mlib_s32      shift,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_U8_S16_Sat(mlib_u8        *dst,
 *                                                     const mlib_s16 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_S32_Sat(mlib_u8        *dst,
 *                                                     const mlib_s32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_S8_Sat(mlib_u8       *dst,
 *                                                    const mlib_s8 *src,
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

#pragma weak mlib_SignalConvertShift_U8_S16_Sat = \
	__mlib_SignalConvertShift_U8_S16_Sat
#pragma weak mlib_SignalConvertShift_U8_S32_Sat = \
	__mlib_SignalConvertShift_U8_S32_Sat
#pragma weak mlib_SignalConvertShift_U8_S8_Sat = \
	__mlib_SignalConvertShift_U8_S8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_U8_S16_Sat)
    mlib_SignalConvertShift_U8_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S32_Sat)
    mlib_SignalConvertShift_U8_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S8_Sat)
    mlib_SignalConvertShift_U8_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S8_Sat(
    mlib_u8 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dx0, dx1, dy0, dy1;
	mlib_f32 fmulter, fzero = vis_read_hi(vis_fzero());
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 7)
		shift = 7;

	if (shift < -8)
		shift = -8;
	ashift = -(shift + 1);

	if (n < 8 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_s8 *psrc_n = (mlib_s8 *)src;
		mlib_u8 *pdst_n = (mlib_u8 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7);

		if (shift < 0) {
			for (i = 0; i < count; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 7);
			}
		} else {
			for (i = 0; i < count; i++) {
				mlib_u32 t0 = ((mlib_u8 *)psrc_n)[i], t;
				mlib_s32 mask;

				t = t0 << (-ashift);
				mask = (0xff - (mlib_s32)t) >> 31;
				pdst_n[i] =
				    (t | mask) & ~((mlib_s32)(t0 << 24) >> 31);
			}
		}

		n -= count;
		dst = (void *)((mlib_u8 *)dst + count);
		src = (void *)((mlib_s8 *)src + count);
	}

/* pdst is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {
		if (shift < 0) {
			vis_write_gsr((shift + 8) << 3);
			fmulter = vis_to_float(0x01010101);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc[i];
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		} else {
			vis_write_gsr(shift << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc[i];
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
	} else {
		mlib_d64 *psrc_a = (mlib_d64 *)vis_alignaddr(psrc, 0);

		if (shift < 0) {
			vis_write_gsr((shift + 8) << 3);
			fmulter = vis_to_float(0x01010101);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc_a[i];

				dx1 = psrc_a[i + 1];
				dx1 = vis_faligndata(dx0, dx1);
				dy0 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx1), fzero);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		} else {
			vis_write_gsr(shift << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc_a[i];

				dx1 = psrc_a[i + 1];
				dx1 = vis_faligndata(dx0, dx1);
				dy0 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx1), fzero);
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_s8 *psrc_n = (mlib_s8 *)psrc;
		mlib_u8 *pdst_n = (mlib_u8 *)pdst;

		if (shift < 0) {
			for (; i < n; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 7);
			}
		} else {
			for (; i < n; i++) {
				mlib_u32 t0 = ((mlib_u8 *)psrc_n)[i], t;
				mlib_s32 mask;

				t = t0 << (-ashift);
				mask = (0xff - (mlib_s32)t) >> 31;
				pdst_n[i] =
				    (t | mask) & ~((mlib_s32)(t0 << 24) >> 31);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S16_Sat(
    mlib_u8 *dst,
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

	if (shift < -8)
		shift = -8;
	ashift = 7 - shift;

	if (n < 8 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_s16 *psrc_n = (mlib_s16 *)src;
		mlib_u8 *pdst_n = (mlib_u8 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 15);
			}
		} else {
			d_val = (1 << shift) * (mlib_d64)(1 << (24 - 7));
			for (i = 0; i < count; i++) {
				SAT_U8(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		n -= count;
		dst = (void *)((mlib_u8 *)dst + count);
		src = (void *)((mlib_s16 *)src + count);
	}

/* pdst is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {
		if (shift < 0) {
			vis_write_gsr(0);
			fmulter = vis_to_float(0x01010101 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		} else {
			vis_write_gsr((shift) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
	} else {
		mlib_d64 *psrc_a = (mlib_d64 *)vis_alignaddr(psrc, 0);
		mlib_d64 prevx;

		if (shift < 0) {
			vis_write_gsr(0);
			fmulter = vis_to_float(0x01010101 << (8 + shift));
			prevx = psrc_a[0];
			++psrc_a;
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx0 = psrc_a[2 * i + 0];
				dx1 = psrc_a[2 * i + 1];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);
				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);
				dx = vis_fpack16_pair(dy0, dy1);
				prevx = dx1;
				pdst[i] = dx;
			}
		} else {
			vis_write_gsr((shift) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				prevx = psrc_a[2 * i];
				dx0 = psrc_a[2 * i + 1];
				dx1 = psrc_a[2 * i + 2];
				dy0 = vis_faligndata(prevx, dx0);
				dy1 = vis_faligndata(dx0, dx1);
				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_s16 *psrc_n = (mlib_s16 *)psrc;
		mlib_u8 *pdst_n = (mlib_u8 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 15);
			}
		} else {
			d_val = (1 << shift) * (mlib_d64)(1 << (24 - 7));
			for (; i < n; i++) {
				SAT_U8(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S32_Sat(
    mlib_u8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
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

	if (shift < -8)
		shift = -8;
	ashift = 23 - shift;

	if (n < 7 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)src;
		mlib_u8 *pdst_n = (mlib_u8 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 31);
			}
		} else {
			d_val = 2.0 * (((mlib_u32)1) << shift);
			for (i = 0; i < count; i++) {
				SAT_U8(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		if (n < 8)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_u8 *)dst + count);
		src = (void *)((mlib_s32 *)src + count);
	}

/* pdst    is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {

		if (shift < 0) {
			vis_write_gsr(0);
			fmulter = vis_to_float(0x01010101 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				dy0 = vis_fpackfix_pair(dy0, dy1);
				dy1 = vis_fpackfix_pair(dy2, dy3);

				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);

				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
#if MLIB_VIS >= 0x200
		else if (shift < 31) {
#else /* MLIB_VIS >= 0x200 */
		else if (shift < 16) {
#endif /* MLIB_VIS >= 0x200 */
			vis_write_gsr(shift << 3);

			dx = 0;
#pragma pipeloop(0)
			for (i = 0; i < 2 * (n >> 3); i++) {
				mlib_d64 dy02, dy13, dy0123;

				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dy0 = vis_fpack32(dx, dy0);
				dy1 = vis_fpack32(dx, dy1);

				dy02 =
				    vis_fpmerge(vis_read_hi(dy0),
				    vis_read_hi(dy1));
				dy13 =
				    vis_fpmerge(vis_read_lo(dy0),
				    vis_read_lo(dy1));
				dy0123 =
				    vis_fpmerge(vis_read_lo(dy02),
				    vis_read_lo(dy13));

				((mlib_f32 *)pdst)[i] = vis_read_lo(dy0123);
			}

			i >>= 1;
		}
#if MLIB_VIS < 0x200
		else if (shift < 22) {
			vis_write_gsr(15 << 3);
			fmulter = vis_to_float(0x00010001 << (shift - 14));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				dy1 = vis_fpackfix_pair(dy0, dy1);
				dy3 = vis_fpackfix_pair(dy2, dy3);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dy1));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dy1));
				dy2 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dy3));
				dy3 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dy3));
				dx = vis_fpack32(dx, dy0);
				dx = vis_fpack32(dx, dy1);
				dx = vis_fpack32(dx, dy2);
				dx = vis_fpack32(dx, dy3);

				dx = vis_fpmerge(vis_read_hi(dx),
				    vis_read_lo(dx));

				pdst[i] = dx;
			}
		} else if (shift < 24) {
			vis_write_gsr(15 << 3);
			fmulter = vis_to_float(0x00010001 << (shift - 14));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				dy1 = vis_fpackfix_pair(dy0, dy1);
				dy3 = vis_fpackfix_pair(dy2, dy3);
				dy0 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy1));
				dy1 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy1));
				dy2 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy3));
				dy3 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy3));
				dx = vis_fpack32(dx, dy0);
				dx = vis_fpack32(dx, dy1);
				dx = vis_fpack32(dx, dy2);
				dx = vis_fpack32(dx, dy3);

				dx = vis_fpmerge(vis_read_hi(dx),
				    vis_read_lo(dx));

				pdst[i] = dx;
			}
		} else if (shift < 31) {
			mlib_d64 dzero = vis_fzero();
			mlib_d64 dplus = vis_fone();
			mlib_d64 dmax = vis_to_double_dup(1 << (31 - shift));

			vis_write_gsr((shift - 16) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_s32 mask0, mask1;
				mlib_d64 dx0, dx1;

				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				{
					mlib_d64 a02 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 a13 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 a03 =
					    vis_fpmerge(vis_read_lo(a02),
					    vis_read_lo(a13));
					mlib_d64 a46 =
					    vis_fpmerge(vis_read_hi(dy2),
					    vis_read_hi(dy3));
					mlib_d64 a57 =
					    vis_fpmerge(vis_read_lo(dy2),
					    vis_read_lo(dy3));
					mlib_d64 a47 =
					    vis_fpmerge(vis_read_lo(a46),
					    vis_read_lo(a57));

					dx0 =
					    vis_fpmerge(dzero,
					    vis_read_lo(a03));
					dx1 =
					    vis_fpmerge(dzero,
					    vis_read_lo(a47));
				}

				dx = vis_fpack16_pair(dx0, dx1);
				pdst[i] = dx;
				mask1 =
				    ((vis_fcmpge32(dy0,
				    dmax) << 6) | (vis_fcmpge32(dy1,
				    dmax) << 4)) | ((vis_fcmpge32(dy2,
				    dmax) << 2) | (vis_fcmpge32(dy3,
				    dmax) << 0));
				vis_pst_8(dplus, pdst + i, mask1);
				mask0 =
				    ((vis_fcmple32(dy0,
				    dzero) << 6) | (vis_fcmple32(dy1,
				    dzero) << 4)) | ((vis_fcmple32(dy2,
				    dzero) << 2) | (vis_fcmple32(dy3,
				    dzero) << 0));
				vis_pst_8(dzero, pdst + i, mask0);
			}
		}
#endif /* MLIB_VIS < 0x200 */
		else {	/* if (shift == 31) */

			mlib_d64 dzero = vis_fzero();
			mlib_d64 dplus = vis_fone();

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_s32 mask;

				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				mask =
				    ((vis_fcmpgt32(dy0,
				    dzero) << 6) | (vis_fcmpgt32(dy1,
				    dzero) << 4)) | ((vis_fcmpgt32(dy2,
				    dzero) << 2) | (vis_fcmpgt32(dy3,
				    dzero) << 0));

				pdst[i] = dzero;
				vis_pst_8(dplus, pdst + i, mask);
			}
		}
	} else {
/* if( (mlib_s32)pdst&7 ) */
		mlib_d64 next;

		psrc = (mlib_d64 *)vis_alignaddr(psrc, 0);

		if (shift < 0) {
			vis_write_gsr(0 + 4);
			fmulter = vis_to_float(0x01010101 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				next = psrc[4 * i + 4];

				dy0 = vis_faligndata(dy0, dy1);
				dy1 = vis_faligndata(dy1, dy2);
				dy2 = vis_faligndata(dy2, dy3);
				dy3 = vis_faligndata(dy3, next);

				dy0 = vis_fpackfix_pair(dy0, dy1);
				dy1 = vis_fpackfix_pair(dy2, dy3);

				dy0 = vis_fmul8x16(fmulter, dy0);
				dy1 = vis_fmul8x16(fmulter, dy1);

				dx = vis_fpack16_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
#if MLIB_VIS >= 0x200
		else if (shift < 31) {
#else /* MLIB_VIS >= 0x200 */
		else if (shift < 16) {
#endif /* MLIB_VIS >= 0x200 */
			mlib_d64 dy2;

			vis_write_gsr((shift << 3) + 4);

			dx = 0;
			dy2 = psrc[0];
			dy2 = vis_fpack32(dx, dy2);
#pragma pipeloop(0)
			for (i = 0; i < 2 * (n >> 3); i++) {
				mlib_d64 dy02, dy13, dy0123;

				dy0 = dy2;
				dy1 = psrc[2 * i + 1];
				dy2 = psrc[2 * i + 2];
				dy1 = vis_fpack32(dx, dy1);
				dy2 = vis_fpack32(dx, dy2);

				dy02 =
				    vis_fpmerge(vis_read_lo(dy0),
				    vis_read_lo(dy1));
				dy13 =
				    vis_fpmerge(vis_read_hi(dy1),
				    vis_read_hi(dy2));
				dy0123 =
				    vis_fpmerge(vis_read_lo(dy02),
				    vis_read_lo(dy13));

				((mlib_f32 *)pdst)[i] = vis_read_lo(dy0123);
			}

			i >>= 1;
		}
#if MLIB_VIS < 0x200
		else if (shift < 22) {
			vis_write_gsr((15 << 3) + 4);
			fmulter = vis_to_float(0x00010001 << (shift - 14));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				next = psrc[4 * i + 4];
				dy0 = vis_faligndata(dy0, dy1);
				dy1 = vis_faligndata(dy1, dy2);
				dy2 = vis_faligndata(dy2, dy3);
				dy3 = vis_faligndata(dy3, next);

				dy1 = vis_fpackfix_pair(dy0, dy1);
				dy3 = vis_fpackfix_pair(dy2, dy3);
				dy0 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dy1));
				dy1 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dy1));
				dy2 =
				    vis_fmuld8ulx16(fmulter, vis_read_hi(dy3));
				dy3 =
				    vis_fmuld8ulx16(fmulter, vis_read_lo(dy3));
				dx = vis_fpack32(dx, dy0);
				dx = vis_fpack32(dx, dy1);
				dx = vis_fpack32(dx, dy2);
				dx = vis_fpack32(dx, dy3);

				dx = vis_fpmerge(vis_read_hi(dx),
				    vis_read_lo(dx));

				pdst[i] = dx;
			}
		} else if (shift < 24) {
			vis_write_gsr((15 << 3) + 4);
			fmulter = vis_to_float(0x00010001 << (shift - 14));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				next = psrc[4 * i + 4];
				dy0 = vis_faligndata(dy0, dy1);
				dy1 = vis_faligndata(dy1, dy2);
				dy2 = vis_faligndata(dy2, dy3);
				dy3 = vis_faligndata(dy3, next);
				dy1 = vis_fpackfix_pair(dy0, dy1);
				dy3 = vis_fpackfix_pair(dy2, dy3);
				dy0 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy1));
				dy1 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy1));
				dy2 =
				    vis_fmuld8sux16(fmulter, vis_read_hi(dy3));
				dy3 =
				    vis_fmuld8sux16(fmulter, vis_read_lo(dy3));
				dx = vis_fpack32(dx, dy0);
				dx = vis_fpack32(dx, dy1);
				dx = vis_fpack32(dx, dy2);
				dx = vis_fpack32(dx, dy3);

				dx = vis_fpmerge(vis_read_hi(dx),
				    vis_read_lo(dx));

				pdst[i] = dx;
			}
		} else if (shift < 31) {
			mlib_d64 dzero = vis_fzero();
			mlib_d64 dplus = vis_fone();
			mlib_d64 dmax = vis_to_double_dup(1 << (31 - shift));

			vis_write_gsr(((shift - 16) << 3) + 4);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_s32 mask0, mask1;
				mlib_d64 dx0, dx1;

				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				next = psrc[4 * i + 4];
				dy0 = vis_faligndata(dy0, dy1);
				dy1 = vis_faligndata(dy1, dy2);
				dy2 = vis_faligndata(dy2, dy3);
				dy3 = vis_faligndata(dy3, next);
				{
					mlib_d64 a02 =
					    vis_fpmerge(vis_read_hi(dy0),
					    vis_read_hi(dy1));
					mlib_d64 a13 =
					    vis_fpmerge(vis_read_lo(dy0),
					    vis_read_lo(dy1));
					mlib_d64 a03 =
					    vis_fpmerge(vis_read_lo(a02),
					    vis_read_lo(a13));
					mlib_d64 a46 =
					    vis_fpmerge(vis_read_hi(dy2),
					    vis_read_hi(dy3));
					mlib_d64 a57 =
					    vis_fpmerge(vis_read_lo(dy2),
					    vis_read_lo(dy3));
					mlib_d64 a47 =
					    vis_fpmerge(vis_read_lo(a46),
					    vis_read_lo(a57));

					dx0 =
					    vis_fpmerge(dzero,
					    vis_read_lo(a03));
					dx1 =
					    vis_fpmerge(dzero,
					    vis_read_lo(a47));
				}

				dx = vis_fpack16_pair(dx0, dx1);
				pdst[i] = dx;
				mask1 =
				    ((vis_fcmpge32(dy0,
				    dmax) << 6) | (vis_fcmpge32(dy1,
				    dmax) << 4)) | ((vis_fcmpge32(dy2,
				    dmax) << 2) | (vis_fcmpge32(dy3,
				    dmax) << 0));
				vis_pst_8(dplus, pdst + i, mask1);
				mask0 =
				    ((vis_fcmple32(dy0,
				    dzero) << 6) | (vis_fcmple32(dy1,
				    dzero) << 4)) | ((vis_fcmple32(dy2,
				    dzero) << 2) | (vis_fcmple32(dy3,
				    dzero) << 0));
				vis_pst_8(dzero, pdst + i, mask0);
			}
		}
#endif /* MLIB_VIS < 0x200 */
		else {	/* if (shift == 31) */

			mlib_d64 dzero = vis_fzero();
			mlib_d64 dplus = vis_fone();

#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				mlib_s32 mask;

				dy0 = psrc[4 * i];
				dy1 = psrc[4 * i + 1];
				dy2 = psrc[4 * i + 2];
				dy3 = psrc[4 * i + 3];
				next = psrc[4 * i + 4];

				dy0 = vis_faligndata(dy0, dy1);
				dy1 = vis_faligndata(dy1, dy2);
				dy2 = vis_faligndata(dy2, dy3);
				dy3 = vis_faligndata(dy3, next);
				mask =
				    ((vis_fcmpgt32(dy0,
				    dzero) << 6) | (vis_fcmpgt32(dy1,
				    dzero) << 4)) | ((vis_fcmpgt32(dy2,
				    dzero) << 2) | (vis_fcmpgt32(dy3,
				    dzero) << 0));

				pdst[i] = dzero;
				vis_pst_8(dplus, pdst + i, mask);
			}
		}

		psrc = (mlib_d64 *)(((mlib_f32 *)psrc) + 1);
	}

	i <<= 3;

	if (i < n) {
		mlib_s32 *psrc_n = (mlib_s32 *)psrc;
		mlib_u8 *pdst_n = (mlib_u8 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 31);
			}
		} else {
			d_val = 2.0 * (((mlib_u32)1) << shift);
			for (; i < n; i++) {
				SAT_U8(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
