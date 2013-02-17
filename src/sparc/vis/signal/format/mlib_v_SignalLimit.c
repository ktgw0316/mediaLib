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

#pragma ident	"@(#)mlib_v_SignalLimit.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLimit_[S16|S16S]_[S16|S16S],
 *      mlib_SignalLimit_[S16|S16S]             - hard limiting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLimit_S16(mlib_s16 *data,
 *                                       const mlib_s16 *low,
 *                                       const mlib_s16 *high,
 *                                       mlib_s32 n);
 *      mlib_status mlib_SignalLimit_S16S(mlib_s16 *data,
 *                                        const mlib_s16 *low,
 *                                        const mlib_s16 *high,
 *                                        mlib_s32 n);
 *      mlib_status mlib_SignalLimit_S16S_S16S(mlib_s16 *dst,
 *                                             const mlib_s16 *src,
 *                                             const mlib_s16 *low,
 *                                             const mlib_s16 *high,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalLimit_S16_S16(mlib_s16 *dst,
 *                                           const mlib_s16 *src,
 *                                           const mlib_s16 *low,
 *                                           const mlib_s16 *high,
 *                                           mlib_s32 n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      low   Lower input limit
 *      high  Upper input limit
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst[i] = { t      , if src[i] < t
 *                  low                  low
 *                 src[i] , if t <= src[i] < t
 *                              low           high
 *                 t      , if src[i] >= t
 *                  high                  high
 *                }
 *
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLimit_S16 = __mlib_SignalLimit_S16
#pragma weak mlib_SignalLimit_S16S = __mlib_SignalLimit_S16S
#pragma weak mlib_SignalLimit_S16S_S16S = __mlib_SignalLimit_S16S_S16S
#pragma weak mlib_SignalLimit_S16_S16 = __mlib_SignalLimit_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLimit_S16) mlib_SignalLimit_S16
    __attribute__((weak, alias("__mlib_SignalLimit_S16")));
__typeof__(__mlib_SignalLimit_S16S) mlib_SignalLimit_S16S
    __attribute__((weak, alias("__mlib_SignalLimit_S16S")));
__typeof__(__mlib_SignalLimit_S16S_S16S) mlib_SignalLimit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalLimit_S16S_S16S")));
__typeof__(__mlib_SignalLimit_S16_S16) mlib_SignalLimit_S16_S16
    __attribute__((weak, alias("__mlib_SignalLimit_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 mask_low, mask_high, off;
	mlib_s32 len;
	mlib_u16 ulow = low[0];
	mlib_u16 uhigh = high[0];
	mlib_s16 *pzend = dst + n - 1;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dx2;
	mlib_d64 dlow = vis_to_double_dup((ulow << 16) | ulow);
	mlib_d64 dhigh = vis_to_double_dup((uhigh << 16) | uhigh);

	if ((low[0] > high[0]) || (n <= 0))
		return (MLIB_FAILURE);

/* prepare the destination address */
	dpz = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dpz - (mlib_addr)dst;

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr((void *)src, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, pzend);
	dx = vis_faligndata(dx0, dx1);
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpx++;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 1);
/* store first bytes of result */
	vis_pst_16(dx, dpz, emask);
	mask_low = vis_fcmplt16(dx, dlow);
	vis_pst_16(dlow, dpz, mask_low & emask);
	mask_high = vis_fcmpge16(dx, dhigh);
	vis_pst_16(dhigh, dpz, mask_high & emask);
	dpz++;
	dx = vis_faligndata(dx0, dx1);
	len = dpzend - dpz;
	mask_low = vis_fcmplt16(dx, dlow);
	mask_high = vis_fcmpge16(dx, dhigh);

	if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
		dx1 = vis_ld_d64_nf(dpx + 1);
		for (; len > 0; len--) {
			dpz[0] = dx;
			vis_pst_16(dlow, dpz, mask_low);
			mask_low = vis_fcmplt16(dx1, dlow);
			vis_pst_16(dhigh, dpz, mask_high);
			mask_high = vis_fcmpge16(dx1, dhigh);
			dx = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			dpx++;
			dpz++;
		}
	} else {
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx2 = vis_faligndata(dx0, dx1);
		for (; len > 0; len--) {
			dpz[0] = dx;
			dx = dx2;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			vis_pst_16(dlow, dpz, mask_low);
			mask_low = vis_fcmplt16(dx, dlow);
			vis_pst_16(dhigh, dpz, mask_high);
			mask_high = vis_fcmpge16(dx, dhigh);
			dx2 = vis_faligndata(dx0, dx1);
			dpx++;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dx, dpz, emask);
		vis_pst_16(dlow, dpz, mask_low & emask);
		vis_pst_16(dhigh, dpz, mask_high & emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16(
    mlib_s16 *data,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_S16_S16(data, data, low, high, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 mask_low, mask_high, off;
	mlib_s32 len;
	mlib_u16 ulow0 = low[0];
	mlib_u16 ulow1 = low[1];
	mlib_u16 uhigh0 = high[0];
	mlib_u16 uhigh1 = high[1];
	mlib_s16 *pzend = dst + 2 * n - 1;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dx2;
	mlib_d64 dlow =
	    ((mlib_addr)dst & 2) ? vis_to_double_dup((ulow1 << 16) | ulow0) :
	    vis_to_double_dup((ulow0 << 16) | ulow1);
	mlib_d64 dhigh =
	    ((mlib_addr)dst & 2) ? vis_to_double_dup((uhigh1 << 16) | uhigh0) :
	    vis_to_double_dup((uhigh0 << 16) | uhigh1);

	if ((low[0] > high[0]) || (low[1] > high[1]) || (n <= 0))
		return (MLIB_FAILURE);

/* prepare the destination address */
	dpz = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dpz - (mlib_addr)dst;

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr((void *)src, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, pzend);
	dx = vis_faligndata(dx0, dx1);
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpx++;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 1);
/* store first bytes of result */
	vis_pst_16(dx, dpz, emask);
	mask_low = vis_fcmplt16(dx, dlow);
	vis_pst_16(dlow, dpz, mask_low & emask);
	mask_high = vis_fcmpge16(dx, dhigh);
	vis_pst_16(dhigh, dpz, mask_high & emask);
	dpz++;
	dx = vis_faligndata(dx0, dx1);
	len = dpzend - dpz;
	mask_low = vis_fcmplt16(dx, dlow);
	mask_high = vis_fcmpge16(dx, dhigh);

	if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
		dx1 = vis_ld_d64_nf(dpx + 1);
		for (; len > 0; len--) {
			dpz[0] = dx;
			vis_pst_16(dlow, dpz, mask_low);
			mask_low = vis_fcmplt16(dx1, dlow);
			vis_pst_16(dhigh, dpz, mask_high);
			mask_high = vis_fcmpge16(dx1, dhigh);
			dx = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			dpx++;
			dpz++;
		}
	} else {
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx2 = vis_faligndata(dx0, dx1);
		for (; len > 0; len--) {
			dpz[0] = dx;
			dx = dx2;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			vis_pst_16(dlow, dpz, mask_low);
			mask_low = vis_fcmplt16(dx, dlow);
			vis_pst_16(dhigh, dpz, mask_high);
			mask_high = vis_fcmpge16(dx, dhigh);
			dx2 = vis_faligndata(dx0, dx1);
			dpx++;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dx, dpz, emask);
		vis_pst_16(dlow, dpz, mask_low & emask);
		vis_pst_16(dhigh, dpz, mask_high & emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16S(
    mlib_s16 *data,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_S16S_S16S(data, data, low, high, n));
}

/* *********************************************************** */
