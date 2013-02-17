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

#define	PROCESS_CH(N)                                           \
	maskh = vis_fcmpgt16(s##N, hh);                         \
	maskl = vis_fcmplt16(s##N, ll);                         \
	vis_pst_16(s##N, dp, emask);                            \
	vis_pst_16(hh, dp, emask & maskh);                      \
	vis_pst_16(ll, dp, emask & maskl);                      \
	dp++

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16_S16_aa(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n,
    mlib_s32 s_mask)
{
	mlib_s32 emask = 0xFF;
	mlib_s32 j;
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 *dp = (mlib_d64 *)dst;
	mlib_d64 s0;
	mlib_d64 ll =
	    vis_to_double_dup((low[0] << 16) | (low[s_mask] & 0xFFFF));
	mlib_d64 hh =
	    vis_to_double_dup((high[0] << 16) | (high[s_mask] & 0xFFFF));
	mlib_s32 maskl, maskh;

	emask = 0xFF;

#pragma pipeloop(0)
	for (j = 0; j <= (n - 4); j += 4) {
		s0 = sp[0];
		PROCESS_CH(0);
		sp++;
	}

	if (n & 3) {
		emask = 0xF0 >> (n & 3);
		s0 = sp[0];
		PROCESS_CH(0);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16_S16_na(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n,
    mlib_s32 s_mask)
{
	mlib_s32 emask = 0xFF;
	mlib_s16 *dend = dst + (n - 1);
	mlib_d64 *dp = (mlib_d64 *)((mlib_addr)dst & ~7);
	mlib_d64 *sp;
	mlib_d64 s0, s1, s2, sx;
	mlib_s32 j = (mlib_s16 *)dp - dst;
	mlib_s32 j1 = j & s_mask;
	mlib_d64 ll =
	    vis_to_double_dup((low[j1] << 16) | (low[s_mask - j1] & 0xFFFF));
	mlib_d64 hh =
	    vis_to_double_dup((high[j1] << 16) | (high[s_mask - j1] & 0xFFFF));
	mlib_s32 maskl, maskh;

	j *= sizeof (mlib_s16);
	n *= sizeof (mlib_s16);
	sp = (mlib_d64 *)vis_alignaddr((void *)src, j);
	emask = vis_edge16(dst, dend);

	s0 = vis_faligndata(sp[0], sp[1]);
	PROCESS_CH(0);
	sp++;
	j += 8;

	emask = 0xFF;
	sx = *sp;
#pragma pipeloop(0)
	for (; j <= (n - 24); j += 24) {
		s1 = sx;
		s2 = sp[1];
		s0 = sp[2];
		sx = sp[3];
		s1 = vis_faligndata(s1, s2);
		s2 = vis_faligndata(s2, s0);
		s0 = vis_faligndata(s0, sx);
		PROCESS_CH(1);
		PROCESS_CH(2);
		PROCESS_CH(0);
		sp += 3;
	}

	s1 = sx;
	s2 = sp[1];
	s0 = sp[2];
	sx = sp[3];
	s1 = vis_faligndata(s1, s2);
	s2 = vis_faligndata(s2, s0);
	s0 = vis_faligndata(s0, sx);

	if ((mlib_s16 *)dp <= dend) {
		emask = vis_edge16(dp, dend);
		PROCESS_CH(1);

		if ((mlib_s16 *)dp <= dend) {
			emask = vis_edge16(dp, dend);
			PROCESS_CH(2);

			if ((mlib_s16 *)dp <= dend) {
				emask = vis_edge16(dp, dend);
				PROCESS_CH(0);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	if ((low[0] > high[0]) || (n <= 0)) {
		return (MLIB_FAILURE);
	}

	if ((((mlib_s32)src | (mlib_s32)dst) & 7) == 0) {
		return (__mlib_SignalLimit_S16_S16_aa
				(dst, src, low, high, n, 0));
	} else {
		return (__mlib_SignalLimit_S16_S16_na
				(dst, src, low, high, n, 0));
	}
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
	if ((low[0] > high[0]) || (low[1] > high[1]) || (n <= 0)) {
		return (MLIB_FAILURE);
	}

	if ((((mlib_s32)src | (mlib_s32)dst) & 7) == 0) {
		return __mlib_SignalLimit_S16_S16_aa(dst, src, low, high, 2 * n,
		    1);
	} else {
		return __mlib_SignalLimit_S16_S16_na(dst, src, low, high, 2 * n,
		    1);
	}
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
