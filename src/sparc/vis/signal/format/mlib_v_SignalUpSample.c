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

#pragma ident	"@(#)mlib_v_SignalUpSample.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalUpSample_[S16|S16S]_[S16|S16S] - upsampling
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalUpSample_S16S_S16S(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                mlib_s32       factor,
 *                                                mlib_s32       phase,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalUpSample_S16_S16(mlib_s16       *dst,
 *                                              const mlib_s16 *src,
 *                                              mlib_s32       factor,
 *                                              mlib_s32       phase,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      dst     Output signal array
 *      src     Input signal array
 *      factor  Factor by which to upsample
 *      phase   Parameter that determines relative position of an input
 *              value, within the output signal. 0 <= phase < factor.
 *      n       Number of samples in the input signal array.
 *
 * DESCRIPTION
 *      X       = x(n), n = 0, 1, ...
 *      Zup     = z(n), n = 0, 1, ...
 *
 *                  x(k),  n = (factor*k + phase)
 *              =
 *                  0   ,  otherwise
 *
 * COMMENTS
 *      A user should supply an output vector
 *      big enough to hold all samples.
 *
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalUpSample_S16S_S16S = __mlib_SignalUpSample_S16S_S16S
#pragma weak mlib_SignalUpSample_S16_S16 = __mlib_SignalUpSample_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalUpSample_S16S_S16S) mlib_SignalUpSample_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalUpSample_S16S_S16S")));
__typeof__(__mlib_SignalUpSample_S16_S16) mlib_SignalUpSample_S16_S16
    __attribute__((weak, alias("__mlib_SignalUpSample_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	UPSAMPLE_S16()                                          \
	ss = (*sp++);                                           \
	d0 = vis_fmuld8ulx16(fone, vis_read_hi(ss));            \
	d1 = vis_fmuld8ulx16(fone, vis_read_lo(ss));            \
	d0 = vis_fand(d0, dmask);                               \
	d1 = vis_fand(d1, dmask)

/* *********************************************************** */

#define	UPSAMPLE_S16_NF()                                       \
	ss = vis_ld_d64_nf(sp); sp++;                           \
	d0 = vis_fmuld8ulx16(fone, vis_read_hi(ss));            \
	d1 = vis_fmuld8ulx16(fone, vis_read_lo(ss));            \
	d0 = vis_fand(d0, dmask);                               \
	d1 = vis_fand(d1, dmask)

/* *********************************************************** */

mlib_status
__mlib_SignalUpSample_S16_S16_f2(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i, off;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, d0, d1, dd_old;
	mlib_f32 fone = vis_to_float(0x00010001);
	mlib_d64 dmask = vis_to_double_dup(0x0000FFFF);

	if (!phase) {
		(*dst++) = (*src++);
		n--;
	}

	while (((mlib_s32)src & 7) && (n > 0)) {
		(*dst++) = 0;
		(*dst++) = (*src++);
		n--;
	}

	if (!n) {
		if (!phase)
			*dst = 0;
		return (MLIB_SUCCESS);
	}

	off = (mlib_s32)dst & 7;
	sp = (void *)src;
	dp = (void *)((mlib_u8 *)dst - off);

	n *= 2;

	if (off == 0) {
#pragma pipeloop(0)
		for (i = 0; i <= (n - 8); i += 8) {
			UPSAMPLE_S16();
			(*dp++) = d0;
			(*dp++) = d1;
		}

		if (i < n) {
			mlib_s32 emask = 0xFF00 >> (n - i);

			UPSAMPLE_S16();
			vis_pst_16(d0, dp, emask >> 4);
			emask &= 0xF;

			if (emask)
				vis_pst_16(d1, dp + 1, emask);
		}

	} else {
		mlib_s32 emask = 0xF >> (off >> 1);

		vis_alignaddrl((void *)0, off);
		i = 8 - (off >> 1);
		UPSAMPLE_S16_NF();

		if (i <= n) {
			dd_old = vis_faligndata(d0, d0);
			vis_pst_16(dd_old, dp, emask);
			dp++;
			(*dp++) = vis_faligndata(d0, d1);
			dd_old = d1;

#pragma pipeloop(0)
			for (; i <= (n - 8); i += 8) {
				UPSAMPLE_S16();
				(*dp++) = vis_faligndata(dd_old, d0);
				(*dp++) = vis_faligndata(d0, d1);
				dd_old = d1;
			}

			if (i < n) {
				mlib_s32 emask = 0xFF00 >> (n - i);

				UPSAMPLE_S16_NF();
				dd_old = vis_faligndata(dd_old, d0);
				d0 = vis_faligndata(d0, d1);
				vis_pst_16(dd_old, dp, emask >> 4);
				emask &= 0xF;

				if (emask)
					vis_pst_16(d0, dp + 1, emask);
			}
		} else {
			mlib_s32 emask2 = 0xFF00 >> (n - i + 8);
			dd_old = vis_faligndata(d0, d0);
			vis_pst_16(dd_old, dp, (emask2 >> 4) & emask);
			d0 = vis_faligndata(d0, d1);
			emask2 &= 0xF;

			if (emask2)
				vis_pst_16(d0, dp + 1, emask2);
		}
	}

	if (!phase) {
		dst[n] = 0;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSample_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i, k, offset = phase;
	mlib_s32 twoFactor = 2 * factor,
		fourFactor = 4 * factor,
		sixthFactor = 6 * factor;
	mlib_s32 align, times;
	mlib_s16 *pSrc = (void *)src, *pDst;
	mlib_d64 dSrc0, dSrc1, *dpSrc;

/* Check for obvious errors */

	if (n <= 0)
		return (MLIB_FAILURE);

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (factor == 2)
		return (__mlib_SignalUpSample_S16_S16_f2(dst, src, phase, n));

	__mlib_VectorZero_S16((void *)dst, n * factor);

	align = ((mlib_addr)pSrc & 7);

	for (k = 0; k < n && align != 0; k++, offset += factor) {
		dst[offset] = src[k];
		pSrc++;
		align = ((mlib_addr)pSrc & 7);
	}

	dpSrc = (mlib_d64 *)((mlib_addr)pSrc & ~7);
	pDst = dst + offset;
	times = (n - k) >> 2;

	vis_alignaddr(dpSrc, 6);

	dSrc0 = vis_ld_d64_nf(dpSrc);

	for (i = 0; i < times; i++, dpSrc++, k += 4) {
		dSrc1 = vis_ld_d64_nf(dpSrc + 1);

		vis_st_u16_i(dSrc0, pDst, sixthFactor);
		dSrc0 = vis_faligndata(dSrc0, dSrc0);
		vis_st_u16_i(dSrc0, pDst, fourFactor);
		dSrc0 = vis_faligndata(dSrc0, dSrc0);
		vis_st_u16_i(dSrc0, pDst, twoFactor);
		dSrc0 = vis_faligndata(dSrc0, dSrc0);
		vis_st_u16_i(dSrc0, pDst, 0);

		dSrc0 = dSrc1;
		pDst += fourFactor;
	}

	for (; k < n; k++, pDst += factor)
		*pDst = src[k];
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSample_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 srcAlign, dstAlign;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (src == NULL || dst == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if ((phase < 0) || (factor <= 0) || (phase >= factor)) {
		return (MLIB_OUTOFRANGE);
	}

	srcAlign = (mlib_addr)src & 3;
	dstAlign = (mlib_addr)dst & 3;

	if ((srcAlign | dstAlign) == 0) {
		return __mlib_SignalUpSample_F32_F32((mlib_f32 *)dst,
		    (mlib_f32 *)src, factor, phase, n);
	}

	{
		mlib_s16 *pSrc = (void *)src;
		mlib_s16 *pDst;
		mlib_s32 twoN = 2 * n;
		mlib_s32 twoPhase = 2 * phase;
		mlib_s32 twoFactor = 2 * factor;
		mlib_s32 fourFactor = 2 * twoFactor;
		mlib_s32 sixteenFactor = 2 * 2 * twoFactor;
		mlib_s32 offset = twoPhase;
		mlib_s32 align;
		mlib_s32 i, k, times;
		mlib_d64 dSrc0, dSrc1, *dpSrc;

		__mlib_VectorZero_S16C((void *)dst, n * factor);

/* Source aligned at S32 boundary */

		if (srcAlign == 0) {
			align = ((mlib_addr)pSrc & 7);

			for (k = 0; k < twoN && align != 0;
			    k += 2, offset += twoFactor) {
				dst[offset] = src[k];
				dst[offset + 1] = src[k + 1];
				pSrc += 2;
				align = ((mlib_addr)pSrc & 7);
			}

			dpSrc = (mlib_d64 *)((mlib_addr)pSrc);
			pDst = dst + offset;
			times =
			    ((twoN - k) >> 2) - 1 >
			    0 ? ((twoN - k) >> 2) - 1 : 0;

			vis_alignaddr(dpSrc, 6);

			dSrc0 = vis_ld_d64_nf(dpSrc);
			for (i = 0; i < times; i++, dpSrc++) {
				dSrc1 = dpSrc[1];

				vis_st_u16_i(dSrc0, pDst, fourFactor + 2);
				dSrc0 = vis_faligndata(dSrc0, dSrc0);
				vis_st_u16_i(dSrc0, pDst, fourFactor);
				dSrc0 = vis_faligndata(dSrc0, dSrc0);
				vis_st_u16_i(dSrc0, pDst, 2);
				dSrc0 = vis_faligndata(dSrc0, dSrc0);
				vis_st_u16_i(dSrc0, pDst, 0);

				dSrc0 = dSrc1;
				pDst += fourFactor;
			}

			k = k + 4 * times;
			offset = twoPhase + k * factor;

			for (; k < twoN; k += 2, offset += twoFactor) {
				dst[offset] = src[k];
				dst[offset + 1] = src[k + 1];
			}
		}

/* Source and destination unaligned */
		else {
			align = 0;

			for (k = 0; k < twoN && align != 2;
			    k += 2, offset += twoFactor) {
				dst[offset] = src[k];
				dst[offset + 1] = src[k + 1];
				pSrc += 2;
				align = ((mlib_addr)pSrc & 7);
			}

			dpSrc = (mlib_d64 *)(((mlib_addr)pSrc - 2) & ~7);
			pDst = dst + (offset - twoFactor);
			times =
			    ((twoN - k) >> 2) - 1 >
			    0 ? ((twoN - k) >> 2) - 1 : 0;

			vis_alignaddr(dpSrc, 6);

			dSrc0 = vis_ld_d64_nf(dpSrc);
			for (i = 0; i < times; i++, dpSrc++) {
				dSrc1 = dpSrc[1];

				vis_st_u16_i(dSrc0, pDst, sixteenFactor);
				dSrc0 = vis_faligndata(dSrc0, dSrc0);
				vis_st_u16_i(dSrc0, pDst, fourFactor + 2);
				dSrc0 = vis_faligndata(dSrc0, dSrc0);
				vis_st_u16_i(dSrc0, pDst, fourFactor);
				dSrc0 = vis_faligndata(dSrc0, dSrc0);
				vis_st_u16_i(dSrc0, pDst, 2);

				dSrc0 = dSrc1;
				pDst += fourFactor;
			}

			k = k + 4 * times - 2;
			offset = twoPhase + k * factor;

			for (; k < twoN; k += 2, offset += twoFactor) {
				dst[offset] = src[k];
				dst[offset + 1] = src[k + 1];
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
