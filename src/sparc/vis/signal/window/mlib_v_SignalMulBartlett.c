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

#pragma ident	"@(#)mlib_v_SignalMulBartlett.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulBartlett_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulBartlett_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulBartlett_S16(mlib_s16 *data,
 *                                             mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulBartlett_S16S(mlib_s16 *data,
 *                                              mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulBartlett_S16_S16(mlib_s16 *dst,
 *                                                 mlib_s16 *src,
 *                                                 mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulBartlett_S16S_S16S(mlib_s16 *dst,
 *                                                   mlib_s16 *src,
 *                                                   mlib_s32 n)
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *      Bartlett    :   window[i] = ((i <= (N - 1)/2) ? (2*i/(N -1)) :
 *                        (2 - (2 * i)/(N-1)))
 *
 */

#include <mlib_signal.h>
#include <vis_proto.h>
#include <mlib_SignalWindow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulBartlett_S16 = __mlib_SignalMulBartlett_S16
#pragma weak mlib_SignalMulBartlett_S16S = __mlib_SignalMulBartlett_S16S
#pragma weak mlib_SignalMulBartlett_S16S_S16S = \
	__mlib_SignalMulBartlett_S16S_S16S
#pragma weak mlib_SignalMulBartlett_S16_S16 = \
	__mlib_SignalMulBartlett_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulBartlett_S16) mlib_SignalMulBartlett_S16
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16")));
__typeof__(__mlib_SignalMulBartlett_S16S) mlib_SignalMulBartlett_S16S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16S")));
__typeof__(__mlib_SignalMulBartlett_S16S_S16S) mlib_SignalMulBartlett_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16S_S16S")));
__typeof__(__mlib_SignalMulBartlett_S16_S16) mlib_SignalMulBartlett_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_SIGNAL_MASK	0x7FFFFFFF

/* *********************************************************** */

#define	MLIB_BARTLETT                                           \
	for (i = 0; i < offset; i++) {                          \
	    result = base >> 15;                                \
	    base += step;                                       \
	    dw[i] = (result * ds[i]) >> 15;                     \
	}                                                       \
	                                                        \
	dw += offset;                                           \
	left -= offset

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16S(
    mlib_s16 *data,
    mlib_s32 n)
{
	return (__mlib_SignalMulBartlett_S16S_S16S(data, data, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (n < 16) {
		mlib_d64 step, base;
		mlib_s32 n2, n1, i;

		n1 = n - 1;
		n2 = (n1 >> 1);
		base = 0;
		step = 2. / n1;

#pragma pipeloop(0)
		for (i = 0; i <= n2; i++) {
			mlib_d64 dat1 = src[i], dat2 = src[n1 - i];

			dst[i] = base * dat1;
			dst[n1 - i] = base * dat2;
			base += step;
		}
	} else {
		mlib_s32 step, base, n2, n1, i, offset, off, m, t;
		mlib_s32 step2;
		mlib_d64 v_step, v_base, v_base2, v_src, v_dst;
		mlib_d64 *dps, *dpd, v_src0, v_res, v_win;
		mlib_u8 mask;

		n1 = n - 1;
		n2 = (n1 >> 1) + 1;
		m = n - n2;
		base = 0;
		step = (MLIB_SIGNAL_MASK) / n1;
		vis_write_gsr(1 << 3);

		step2 = step << 1;
		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(vis_ld_d64_nf(dps), v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		t = (off >> 1) * step;
		v_base = vis_to_double(base + t, base + step + t);
		v_step = vis_to_double(step2, step2);
		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m - 1)));
		v_src = vis_faligndata(v_src0, dps[0]);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);

		dst += m;
		src += m;
		m = n - m;
		base = (m - 1) * step;
		step = -step;

		step2 = step << 1;
		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(dps[0], v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		t = (off >> 1) * step;
		v_base = vis_to_double(base + t, base + step + t);
		v_step = vis_to_double(step2, step2);
		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m - 1)));
		v_src = vis_faligndata(v_src0, vis_ld_d64_nf(dps));
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_SignalMulBartlett_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (n < 16 || i1 - i0 < 16) {
		mlib_d64 step, base;
		mlib_s32 n1, i;

		n1 = n - 1;
		step = 2. / n1;
		base = i0 * step;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			mlib_d64 dat1 = src[i], dat2 = src[n1 - i];

			dst[i] = base * dat1;
			dst[n1 - i] = base * dat2;
			base += step;
		}
	} else {
		mlib_s32 step, base, n1, i, offset, off, m, t;
		mlib_s32 step2;
		mlib_d64 v_step, v_base, v_base2, v_src, v_dst;
		mlib_d64 *dps, *dpd, v_src0, v_res, v_win;
		mlib_u8 mask;
		mlib_s32 ddelta = (i1 == ((n - 1) >> 1) + 1) ? (n & 1) : 0;
		mlib_s16 *rdst = dst;
		mlib_s16 stmp;

		n1 = n - 1;
		m = i1 - i0;
		step = (MLIB_SIGNAL_MASK) / n1;
		base = i0 * step;
		vis_write_gsr(1 << 3);

		dst += i0;
		src += i0;
		step2 = step << 1;
		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(vis_ld_d64_nf(dps), v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		t = (off >> 1) * step;
		v_base = vis_to_double(base + t, base + step + t);
		v_step = vis_to_double(step2, step2);
		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);
#pragma pipeloop(0)
		for (i = offset; i < m - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];
			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m - 1)));
		v_src = vis_faligndata(v_src0, dps[0]);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dst += n1 - i0 - (i1 - 1);
		src += n1 - i0 - (i1 - 1);
		m = i1 - i0;
		base = (i1 - 1) * step;
		step = -step;

		if (ddelta) stmp = rdst[n/2];
		step2 = step << 1;
		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(dps[0], v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		t = (off >> 1) * step;
		v_base = vis_to_double(base + t, base + step + t);
		v_step = vis_to_double(step2, step2);
		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m - 1)));
		v_src = vis_faligndata(v_src0, vis_ld_d64_nf(dps));
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		if (ddelta) rdst[n / 2] = stmp;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (n < 16) {
		mlib_d64 step, base;
		mlib_s32 n2, n1, i, j, e0, e1;

		e1 = (n - 1) << 1;
		e0 = e1 + 1;
		n1 = (n - 1);
		n2 = (n1) >> 1;
		base = 0;
		step = 2. / n1;

#pragma pipeloop(0)
		for (i = 0, j = 0; i <= n2; i++, j += 2) {
			mlib_d64 src0 = src[j], src1 = src[j + 1], src2 =
			    src[e1 - j], src3 = src[e0 - j];

			dst[j] = base * src0;
			dst[j + 1] = base * src1;
			dst[e1 - j] = base * src2;
			dst[e0 - j] = base * src3;
			base += step;
		}
	} else {
		mlib_s32 step, base, n2, n1, i, offset, off, m, m2;
		mlib_d64 v_step, v_base, v_base2, v_src, v_dst;
		mlib_d64 *dps, *dpd, v_src0, v_res, v_win;
		mlib_u8 mask;

		n1 = n - 1;
		n2 = (n1 >> 1) + 1;
		m = n - n2;
		m2 = m << 1;
		base = 0;
		step = (MLIB_SIGNAL_MASK) / n1;
		vis_write_gsr(1 << 3);

		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(vis_ld_d64_nf(dps), v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		v_step = vis_to_double(step, step);
		switch (off) {
		case 0:
			v_base = vis_to_double(base, base);
			break;
		case -6:
			v_base = vis_to_double(base - step - step, base - step);
			break;
		case -4:
			v_base = vis_to_double(base - step, base - step);
			break;
		case -2:
			v_base = vis_to_double(base - step, base);
			break;
		default:
			return (MLIB_FAILURE);
		}

		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m2 - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m2 - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m2 - 1)));
		v_src = vis_faligndata(v_src0, dps[0]);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);

		dst += m2;
		src += m2;
		m = n - m;
		m2 = m << 1;
		base = (m - 1) * step;
		step = -step;

		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(dps[0], v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		v_step = vis_to_double(step, step);
		switch (off) {
		case 0:
			v_base = vis_to_double(base, base);
			break;
		case -6:
			v_base = vis_to_double(base - step - step, base - step);
			break;
		case -4:
			v_base = vis_to_double(base - step, base - step);
			break;
		case -2:
			v_base = vis_to_double(base - step, base);
			break;
		default:
			return (MLIB_FAILURE);
		}

		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m2 - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m2 - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m2 - 1)));
		v_src = vis_faligndata(v_src0, vis_ld_d64_nf(dps));
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_SignalMulBartlett_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (n < 16 || i1 - i0 < 16) {
		mlib_d64 step, base;
		mlib_s32 n1, i, j, e0, e1;

		e1 = ((n - 1) << 1);
		e0 = e1 + 1;
		n1 = (n - 1);
		step = 2. / n1;
		base = step * i0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0, j = i0 << 1; i < i1; i++, j += 2) {
			mlib_d64 src0 = src[j];
			mlib_d64 src1 = src[j + 1];
			mlib_d64 src2 = src[e1 - j];
			mlib_d64 src3 = src[e0 - j];

			dst[j] = base * src0;
			dst[j + 1] = base * src1;
			dst[e1 - j] = base * src2;
			dst[e0 - j] = base * src3;
			base += step;
		}
	} else {
		mlib_s32 step, base, n1, i, offset, off, m, m2;
		mlib_d64 v_step, v_base, v_base2, v_src, v_dst;
		mlib_d64 *dps, *dpd, v_src0, v_res, v_win;
		mlib_u8 mask;
		mlib_s16 *rdst = dst;
		mlib_s16 stmp0, stmp1;
		mlib_s32 ddelta = (i1 == ((n - 1) >> 1) + 1) ? (n & 1) : 0;

		n1 = n - 1;
		m = i1 - i0;
		m2 = m << 1;
		step = (MLIB_SIGNAL_MASK) / n1;
		base = i0 * step;
		vis_write_gsr(1 << 3);

		dst += i0 * 2;
		src += i0 * 2;
		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(vis_ld_d64_nf(dps), v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		v_step = vis_to_double(step, step);
		switch (off) {
		case 0:
			v_base = vis_to_double(base, base);
			break;
		case -6:
			v_base = vis_to_double(base - step - step, base - step);
			break;
		case -4:
			v_base = vis_to_double(base - step, base - step);
			break;
		case -2:
			v_base = vis_to_double(base - step, base);
			break;
		default:
			return (MLIB_FAILURE);
		}

		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m2 - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m2 - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m2 - 1)));
		v_src = vis_faligndata(v_src0, dps[0]);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);

		dst += (n1 - i0 - (i1 - 1)) * 2;
		src += (n1 - i0 - (i1 - 1)) * 2;
		m = i1 - i0;
		m2 = m << 1;
		base = (i1 - 1) * step;
		step = -step;

		if (ddelta) {
			stmp0 = rdst[n / 2 * 2];
			stmp1 = rdst[n / 2 * 2 + 1];
		}

		dpd = vis_alignaddr(dst, 0);
		mask = vis_edge16(dst, dst + 4);
		off = ((mlib_addr)dpd - (mlib_addr)dst);
		dps = vis_alignaddr((void *)src, off);
		v_src0 = dps[1];
		v_src = vis_faligndata(dps[0], v_src0);
		dps += 2;
		offset = (8 + off) >> 1;
		v_step = vis_to_double(step, step);
		switch (off) {
		case 0:
			v_base = vis_to_double(base, base);
			break;
		case -6:
			v_base = vis_to_double(base - step - step, base - step);
			break;
		case -4:
			v_base = vis_to_double(base - step, base - step);
			break;
		case -2:
			v_base = vis_to_double(base - step, base);
			break;
		default:
			return (MLIB_FAILURE);
		}

		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);
		dpd++;
		v_base = vis_fpadd32(v_base, v_step);
		v_base2 = vis_fpadd32(v_base2, v_step);

#pragma pipeloop(0)
		for (i = offset; i < m2 - 8; i += 8) {
			mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

			v_src = vis_faligndata(v_src0, v_src1);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst, v_src0 = v_src2;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			v_src = vis_faligndata(v_src1, v_src2);
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[1] = v_dst;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);

			dpd += 2;
			dps += 2;
		}

		if (i < m2 - 4) {
			v_src = vis_faligndata(v_src0, dps[0]);
			v_src0 = dps[0];
			v_win = vis_fpackfix_pair(v_base, v_base2);
			v_res = vis_fmul8ulx16(v_src, v_win);
			v_dst = vis_fmul8sux16(v_src, v_win);
			v_dst = vis_fpadd16(v_res, v_dst);
			v_dst = vis_fpadd16(v_dst, v_dst);
			dpd[0] = v_dst;
			dpd++;
			dps++;
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
		}

		mask = vis_edge16(dpd, (void *)((mlib_addr)(dst + m2 - 1)));
		v_src = vis_faligndata(v_src0, vis_ld_d64_nf(dps));
		v_win = vis_fpackfix_pair(v_base, v_base2);
		v_res = vis_fmul8ulx16(v_src, v_win);
		v_dst = vis_fmul8sux16(v_src, v_win);
		v_dst = vis_fpadd16(v_res, v_dst);
		v_dst = vis_fpadd16(v_dst, v_dst);
		vis_pst_16(v_dst, dpd, mask);

		if (ddelta) {
			rdst[n / 2 * 2] = stmp0;
			rdst[n / 2 * 2 + 1] = stmp1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16(
    mlib_s16 *data,
    mlib_s32 n)
{
	return (__mlib_SignalMulBartlett_S16_S16(data, data, n));
}

/* *********************************************************** */
