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

#pragma ident	"@(#)mlib_v_SignalConvertShift_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S16_S32|S16S_S32S]_Sat,
 *      mlib_SignalConvertShift_[S16_S8|S16S_S8S]_Sat,
 *      mlib_SignalConvertShift_[S16_U8|S16S_U8S]_Sat,
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_S16S_S32S_Sat(mlib_s16       *dst,
 *                                                        const mlib_s32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16S_S8S_Sat(mlib_s16      *dst,
 *                                                       const mlib_s8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16S_U8S_Sat(mlib_s16      *dst,
 *                                                       const mlib_u8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16_S32_Sat(mlib_s16       *dst,
 *                                                      const mlib_s32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16_S8_Sat(mlib_s16      *dst,
 *                                                     const mlib_s8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16_U8_Sat(mlib_s16      *dst,
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

#pragma weak mlib_SignalConvertShift_S16_S32_Sat = \
	__mlib_SignalConvertShift_S16_S32_Sat
#pragma weak mlib_SignalConvertShift_S16_S8_Sat = \
	__mlib_SignalConvertShift_S16_S8_Sat
#pragma weak mlib_SignalConvertShift_S16_U8_Sat = \
	__mlib_SignalConvertShift_S16_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_S16_S32_Sat)
    mlib_SignalConvertShift_S16_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_S8_Sat)
    mlib_SignalConvertShift_S16_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_S8_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_U8_Sat)
    mlib_SignalConvertShift_S16_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_S8_Sat(
    mlib_s16 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dx0, dx1, dy0, dy1, dmulter;
	mlib_f32 fmulter, fzero = vis_read_hi(vis_fzero());
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 7)
		shift = 7;

	if (shift < -15)
		shift = -15;
	ashift = shift + 8;

	if (n < 8 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_s8 *psrc_n = (mlib_s8 *)src;
		mlib_s16 *pdst_n = (mlib_s16 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7) >> 1;

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] =
				    ((ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift)));
			}
		} else {
			d_val = ((mlib_u32)1) << (shift + 24);
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		n -= count;
		dst = (void *)((mlib_s16 *)dst + count);
		src = (void *)((mlib_s8 *)src + count);
	}

/* pdst is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {
		if (ashift < 0) {
			dmulter = vis_to_double_dup(0x00010001 << (16 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 = vis_fmul8ulx16(dmulter, dx0);
				pdst[2 * i] = dy0;
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy1 = vis_fmul8ulx16(dmulter, dx1);
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift < 8) {
			fmulter = vis_to_float(0x01010101 << ashift);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 = vis_fmul8x16(fmulter, dx0);
				pdst[2 * i] = dy0;
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy1 = vis_fmul8x16(fmulter, dx1);
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift == 8) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				pdst[2 * i] = dx0;
				pdst[2 * i + 1] = dx1;
			}
		} else {
			vis_write_gsr((shift) << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				pdst[2 * i] = vis_fpackfix_pair(dy0, dy1);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx1), fzero);
				pdst[2 * i + 1] = vis_fpackfix_pair(dy0, dy1);
			}
		}
	} else {
		mlib_d64 *psrc_a = (mlib_d64 *)vis_alignaddr(psrc, 0);
		mlib_d64 nextx, prevx = psrc_a[0];

		++psrc_a;

		if (ashift < 0) {
			dmulter = vis_to_double_dup(0x00010001 << (16 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = psrc_a[i];
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fmul8ulx16(dmulter, dx0);
				pdst[2 * i] = dy0;
				dy1 = vis_fmul8ulx16(dmulter, dx1);
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift < 8) {
			fmulter = vis_to_float(0x01010101 << ashift);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = psrc_a[i];
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 = vis_fmul8x16(fmulter, dx0);
				pdst[2 * i] = dy0;
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy1 = vis_fmul8x16(fmulter, dx1);
				pdst[2 * i + 1] = dy1;
			}
		} else if (ashift == 8) {
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = psrc_a[i];
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				pdst[2 * i] = dx0;
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				pdst[2 * i + 1] = dx1;
			}
		} else {
			vis_write_gsr((shift) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = psrc_a[i];
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx0), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx0), fzero);
				pdst[2 * i] = vis_fpackfix_pair(dy0, dy1);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(vis_read_hi(dx1), fzero);
				dy1 = vis_fpmerge(vis_read_lo(dx1), fzero);
				pdst[2 * i + 1] = vis_fpackfix_pair(dy0, dy1);
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_s8 *psrc_n = (mlib_s8 *)psrc;
		mlib_s16 *pdst_n = (mlib_s16 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] =
				    (ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift));
			}
		} else {
			d_val = ((mlib_u32)1) << (shift + 24);
			for (; i < n; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK32TO16(out, in1, in2)                                      \
	{                                                              \
	    mlib_d64 tmp0 =                                            \
		vis_fpmerge(vis_read_hi(in1), vis_read_hi(in2));       \
	    mlib_d64 tmp1 =                                            \
		vis_fpmerge(vis_read_lo(in1), vis_read_lo(in2));       \
	    mlib_d64 tmp2 =                                            \
		vis_fpmerge(vis_read_hi(tmp0), vis_read_hi(tmp1));     \
	    out = vis_fpmerge(vis_read_hi(tmp2), vis_read_lo(tmp2));   \
	}

/* *********************************************************** */

#define	PACKF32TO16(out, f0, d0, f1)                                   \
	{                                                              \
	    mlib_d64 tmp0 = vis_fpmerge(f0, vis_read_lo(d0));          \
	    mlib_d64 tmp1 = vis_fpmerge(vis_read_hi(d0), f1);          \
	    mlib_d64 tmp2 =                                            \
		vis_fpmerge(vis_read_hi(tmp0), vis_read_hi(tmp1));     \
	    out = vis_fpmerge(vis_read_hi(tmp2), vis_read_lo(tmp2));   \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_U8_Sat(
    mlib_s16 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dx0, dx1, dy0, dy1;
	mlib_f32 fmulter, fzero = vis_read_hi(vis_fzero());
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 8)
		shift = 8;
	ashift = shift + 7;

	if (shift <= -15) {
/* set output to zero */
		return (__mlib_VectorZero_S16(dst, n));
	}

	if (n < 8 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_u8 *psrc_n = (mlib_u8 *)src;
		mlib_s16 *pdst_n = (mlib_s16 *)dst;

		count = (n < 8) ? n : ((8 - (mlib_addr)dst) & 7) >> 1;

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] =
				    ((ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift)));
			}
		} else {
			d_val = ((mlib_u32)1) << (ashift + 16);
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		n -= count;
		dst = (void *)((mlib_s16 *)dst + count);
		src = (void *)((mlib_u8 *)src + count);
	}

/* pdst is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc) && shift <= 0) {
		if (shift < 0) {
			fmulter = vis_to_float(0x1 << (15 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dy0 = vis_fmul8x16al(vis_read_hi(dx), fmulter);
				dy1 = vis_fmul8x16al(vis_read_lo(dx), fmulter);
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else {
			fmulter = vis_to_float(0x4000);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				dx = psrc[i];
				dy0 = vis_fmul8x16al(vis_read_hi(dx), fmulter);
				dy1 = vis_fmul8x16al(vis_read_lo(dx), fmulter);
				dy0 = vis_fpadd16(dy0, dy0);
				dy1 = vis_fpadd16(dy1, dy1);
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		}
	} else {
		mlib_d64 *psrc_a = (mlib_d64 *)vis_alignaddr(psrc, 0);
		mlib_d64 nextx, prevx = vis_ld_d64_nf(psrc_a);

		++psrc_a;

		if (shift < 0) {
			fmulter = vis_to_float(0x1 << (15 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = vis_ld_d64_nf(psrc_a + i);
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dy0 = vis_fmul8x16al(vis_read_hi(dx), fmulter);
				dy1 = vis_fmul8x16al(vis_read_lo(dx), fmulter);
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else if (shift == 0) {
			fmulter = vis_to_float(0x4000);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = vis_ld_d64_nf(psrc_a + i);
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dy0 = vis_fmul8x16al(vis_read_hi(dx), fmulter);
				dy1 = vis_fmul8x16al(vis_read_lo(dx), fmulter);
				dy0 = vis_fpadd16(dy0, dy0);
				dy1 = vis_fpadd16(dy1, dy1);
				pdst[2 * i] = dy0;
				pdst[2 * i + 1] = dy1;
			}
		} else {
			vis_write_gsr((shift + 7) << 3);
			vis_alignaddr(psrc, 0);
#pragma pipeloop(0)
			for (i = 0; i < n >> 3; i++) {
				nextx = vis_ld_d64_nf(psrc_a + i);
				dx = vis_faligndata(prevx, nextx);
				prevx = nextx;
				dx0 = vis_fpmerge(vis_read_hi(dx), fzero);
				dx1 = vis_fpmerge(vis_read_lo(dx), fzero);
				dy0 = vis_fpmerge(fzero, vis_read_hi(dx0));
				dy1 = vis_fpmerge(fzero, vis_read_lo(dx0));
				pdst[2 * i] = vis_fpackfix_pair(dy0, dy1);
				dy0 = vis_fpmerge(fzero, vis_read_hi(dx1));
				dy1 = vis_fpmerge(fzero, vis_read_lo(dx1));
				pdst[2 * i + 1] = vis_fpackfix_pair(dy0, dy1);
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_u8 *psrc_n = (mlib_u8 *)psrc;
		mlib_s16 *pdst_n = (mlib_s16 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] =
				    (ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift));
			}
		} else {
			d_val = ((mlib_u32)1) << (ashift + 16);
			for (; i < n; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_S32_Sat(
    mlib_s16 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dmulter;
	mlib_d64 *psrc;
	mlib_d64 *pdst;
	mlib_d64 dx, dy0, dy1;
	mlib_f32 fmulter;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 31)
		shift = 31;

	if (shift < -15)
		shift = -15;
	ashift = shift - 16;

	if (n < 7 || (mlib_addr)dst & 7) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)src;
		mlib_s16 *pdst_n = (mlib_s16 *)dst;

		count = (n < 7) ? n : ((8 - (mlib_addr)dst) & 7) >> 1;

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << shift;
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		if (n < 7)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_s16 *)dst + count);
		src = (void *)((mlib_s32 *)src + count);
	}

/* pdst    is d64-aligned now */

	psrc = (mlib_d64 *)src;
	pdst = (mlib_d64 *)dst;

	if (!(7 & (mlib_addr)psrc)) {

		if (shift < -8) {
			vis_write_gsr(0 << 3);
			dmulter = vis_to_double_dup(0x00010001 << (16 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dx = vis_fpackfix_pair(dy0, dy1);
				dx = vis_fmul8ulx16(dmulter, dx);
				pdst[i] = dx;
			}
		} else if (shift < 0) {
			vis_write_gsr(0 << 3);
			fmulter = vis_to_float(0x01010101 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dx = vis_fpackfix_pair(dy0, dy1);
				dx = vis_fmul8x16(fmulter, dx);
				pdst[i] = dx;
			}
		}
#if MLIB_VIS >= 0x200
		else if (shift <= 31) {
#else /* MLIB_VIS >= 0x200 */
		else if (shift <= 15) {
#endif /* MLIB_VIS >= 0x200 */
			vis_write_gsr(shift << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				dx = vis_fpackfix_pair(dy0, dy1);
				pdst[i] = dx;
			}
		} else {
			mlib_d64 d_val;

			vis_write_gsr(0 << 3);
			d_val = ((mlib_u32)1) << shift;
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc[2 * i];
				dy1 = psrc[2 * i + 1];
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy0)[0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy0)[1],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy1)[0],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy1)[1],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				dx = vis_fpackfix_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
	} else {
/* if( (mlib_s32)pdst&7 ) */

		if (shift < -8) {
			mlib_d64 *psrc_d = (mlib_d64 *)vis_alignaddr(psrc, 0);
			mlib_d64 prev = *psrc_d;

			++psrc_d;
			dmulter = vis_to_double_dup(0x00010001 << (16 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc_d[0];
				dy1 = psrc_d[1];
				PACKF32TO16(dx, vis_read_lo(prev), dy0,
				    vis_read_hi(dy1));
				prev = dy1;
				psrc_d += 2;
				dx = vis_fmul8ulx16(dmulter, dx);
				pdst[i] = dx;
			}
		} else if (shift < 0) {
			mlib_d64 *psrc_d = (mlib_d64 *)vis_alignaddr(psrc, 0);
			mlib_d64 prev = *psrc_d;

			++psrc_d;
			fmulter = vis_to_float(0x01010101 << (8 + shift));
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc_d[0];
				dy1 = psrc_d[1];
				PACKF32TO16(dx, vis_read_lo(prev), dy0,
				    vis_read_hi(dy1));
				prev = dy1;
				psrc_d += 2;
				dx = vis_fmul8x16(fmulter, dx);
				pdst[i] = dx;
			}
		} else if (shift == 0) {
			mlib_d64 *psrc_d = (mlib_d64 *)vis_alignaddr(psrc, 0);
			mlib_d64 prev = *psrc_d;

			++psrc_d;
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc_d[0];
				dy1 = psrc_d[1];
				PACKF32TO16(dx, vis_read_lo(prev), dy0,
				    vis_read_hi(dy1));
				prev = dy1;
				psrc_d += 2;
				pdst[i] = dx;
			}
		}
#if MLIB_VIS >= 0x200
		else if (shift <= 31) {
#else /* MLIB_VIS >= 0x200 */
		else if (shift <= 15) {
#endif /* MLIB_VIS >= 0x200 */
			mlib_d64 *psrc_d = (mlib_d64 *)vis_alignaddr(psrc, 0);
			mlib_d64 prev = *psrc_d;

			++psrc_d;
			vis_write_gsr((shift << 3) + 4);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 = psrc_d[0];
				dy1 = psrc_d[1];
				prev = vis_faligndata(prev, dy0);
				dy0 = vis_faligndata(dy0, dy1);
				dx = vis_fpackfix_pair(prev, dy0);
				prev = dy1;
				psrc_d += 2;
				pdst[i] = dx;
			}
		} else {
			mlib_d64 d_val;
			mlib_f32 *psrc_f = (mlib_f32 *)psrc;

			d_val = ((mlib_u32)1) << shift;
			vis_write_gsr(0 << 3);
#pragma pipeloop(0)
			for (i = 0; i < n >> 2; i++) {
				dy0 =
				    vis_freg_pair(psrc_f[4 * i],
				    psrc_f[4 * i + 1]);
				dy1 =
				    vis_freg_pair(psrc_f[4 * i + 2],
				    psrc_f[4 * i + 3]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy0)[0],
				    d_val * ((mlib_s32 *)&dy0)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy0)[1],
				    d_val * ((mlib_s32 *)&dy0)[1]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy1)[0],
				    d_val * ((mlib_s32 *)&dy1)[0]);
				FLOAT2INT_CLAMP(((mlib_s32 *)&dy1)[1],
				    d_val * ((mlib_s32 *)&dy1)[1]);
				dx = vis_fpackfix_pair(dy0, dy1);
				pdst[i] = dx;
			}
		}
	}

	if (n & 3) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)&psrc[2 * i];
		mlib_s16 *pdst_n = (mlib_s16 *)&pdst[i];

		count = n & 3;

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << shift;
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
