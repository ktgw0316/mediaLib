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

#pragma ident	"@(#)mlib_v_SignalMulRectangular.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulRectangular_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulRectangular_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulRectangular_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulRectangular_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulRectangular_S16(mlib_s16      *data,
 *                                                mlib_s32 m,
 *                                                mlib_s32      n)
 *
 *      mlib_status mlib_SignalMulRectangular_S16S(mlib_s16     *data,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32     n)
 *
 *      mlib_status mlib_SignalMulRectangular_S16_S16(mlib_s16       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulRectangular_S16S_S16S(mlib_s16       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n)
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      m        Rectangular window parameter (m <= n)
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        : dst[i] = src[i] * window[i]    i = 0..(n-1)
 *      Stereo      : dst[i] = src[i] * window[i/2]  i = 0..(2*n-1)
 *      Rectangular : window[j] = (j < m) ? 1 : 0    j = 0..(n-1)
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulRectangular_S16 = __mlib_SignalMulRectangular_S16
#pragma weak mlib_SignalMulRectangular_S16S = \
	__mlib_SignalMulRectangular_S16S
#pragma weak mlib_SignalMulRectangular_S16S_S16S = \
	__mlib_SignalMulRectangular_S16S_S16S
#pragma weak mlib_SignalMulRectangular_S16_S16 = \
	__mlib_SignalMulRectangular_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulRectangular_S16) mlib_SignalMulRectangular_S16
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16")));
__typeof__(__mlib_SignalMulRectangular_S16S) mlib_SignalMulRectangular_S16S
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16S")));
__typeof__(__mlib_SignalMulRectangular_S16S_S16S)
    mlib_SignalMulRectangular_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16S_S16S")));
__typeof__(__mlib_SignalMulRectangular_S16_S16)
    mlib_SignalMulRectangular_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_OFFSET8(ptr)	((8  - ((mlib_addr)ptr & 7)) & 7)
#define	MLIB_OFFSET16(ptr)	(MLIB_OFFSET8(ptr) >> 1)

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16(
    mlib_s16 *data,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 *pd, d64zero;
	mlib_s32 offset, i, times;
	mlib_s16 s16zero = 0;

/* check for obvious errors */

	if ((data == NULL) || (m < 0) || (n <= 0) || (m > n))
		return (MLIB_FAILURE);

	((mlib_s32 *)&d64zero)[0] = 0;
	((mlib_s32 *)&d64zero)[1] = 0;
	data += m;
	m = n - m;
	offset = MLIB_OFFSET16(data);

	if (offset) {
		if (offset > m)
			offset = m;
		for (i = 0; i < offset; i++)
			data[i] = s16zero;
		m -= offset;
	}

	pd = (mlib_d64 *)&data[offset];
	times = m >> 2;
	for (i = 0; i < times; i++)
		pd[i] = d64zero;
	m -= times << 2;
	data = (mlib_s16 *)&pd[times];
	for (i = 0; i < m; i++)
		data[i] = s16zero;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16S(
    mlib_s16 *data,
    mlib_s32 m,
    mlib_s32 n)
{
	return (__mlib_SignalMulRectangular_S16(data, m + m, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 m,
    mlib_s32 n)
{
/* check for obvious errors */

	if ((dst == NULL) || (m < 0) || (n <= 0) || (m > n) || (src == NULL))
		return (MLIB_FAILURE);

	if (((mlib_addr)dst & 7) == ((mlib_addr)src & 7)) {
		mlib_d64 *pd, *ps, d64zero = vis_fzero();
		mlib_s32 offset, i, times, m2 = n - m;
		mlib_s16 s16zero = 0;

		offset = MLIB_OFFSET16(dst);

		if (offset) {
			if (offset > m)
				offset = m;
			for (i = 0; i < offset; i++)
				dst[i] = src[i];
			m -= offset;
		}

		pd = (mlib_d64 *)&dst[offset];
		ps = (mlib_d64 *)&src[offset];
		times = m >> 3;
		times <<= 1;

#pragma pipeloop(0)
		for (i = 0; i < times; i += 2) {
			mlib_d64 ps1 = ps[i], ps2 = ps[i + 1];

			pd[i] = ps1;
			pd[i + 1] = ps2;
		}

		m -= times << 2;
		dst = (mlib_s16 *)&pd[times];
		src = (mlib_s16 *)&ps[times];
		for (i = 0; i < m; i++)
			dst[i] = src[i];
		dst += m;
		m = m2;
		offset = MLIB_OFFSET16(dst);

		if (offset) {
			if (offset > m)
				offset = m;
			for (i = 0; i < offset; i++)
				dst[i] = s16zero;
			m -= offset;
		}

		pd = (mlib_d64 *)&dst[offset];
		times = m >> 2;
		for (i = 0; i < times; i++)
			pd[i] = d64zero;
		m -= times << 2;
		dst = (mlib_s16 *)&pd[times];
		for (i = 0; i < m; i++)
			dst[i] = s16zero;
		return (MLIB_SUCCESS);
	} else {
		mlib_d64 *dpd, *dps, v_src, v_src0, d64zero = vis_fzero();
		mlib_s32 off, i, m2 = n - m;
		mlib_s16 s16zero = 0;
		mlib_u8 mask;

		if (m < 8)
			for (i = 0; i < m; i++)
				dst[i] = src[i];
		else {
			dpd = vis_alignaddr(dst, 0);
			mask = vis_edge16(dst, dst + 4);
			off = ((mlib_addr)dpd - (mlib_addr)dst);
			dps = vis_alignaddr((void *)src, off);
			v_src0 = dps[1];
			v_src = vis_faligndata(vis_ld_d64_nf(dps), v_src0);
			dps += 2;
			vis_pst_16(v_src, dpd, mask);
			dpd++;

#pragma pipeloop(0)
			for (i = ((8 + off) >> 1); i < m - 8; i += 8) {
				mlib_d64 v_src1 = dps[0], v_src2 = dps[1];

				v_src = vis_faligndata(v_src0, v_src1);
				dpd[0] = v_src, v_src0 = v_src2;
				v_src = vis_faligndata(v_src1, v_src2);
				dpd[1] = v_src;

				dpd += 2;
				dps += 2;
			}

			if (i < m - 4) {
				v_src = vis_faligndata(v_src0, dps[0]);
				v_src0 = dps[0];
				dpd[0] = v_src;
				dpd++;
				dps++;
			}

			mask =
			    vis_edge16(dpd, (void *)((mlib_addr)(dst + m - 1)));
			v_src = vis_faligndata(v_src0, vis_ld_d64_nf(dps));
			vis_pst_16(v_src, dpd, mask);
		}

		dst += m;
		m = m2;

		if (m < 8)
			for (i = 0; i < m; i++)
				dst[i] = s16zero;
		else {
			dpd = vis_alignaddr(dst, 0);
			mask = vis_edge16(dst, dst + 4);
			off = ((mlib_addr)dpd - (mlib_addr)dst);
			vis_pst_16(d64zero, dpd, mask);
			dpd++;
#pragma pipeloop(0)
			for (i = ((8 + off) >> 1); i < m - 4; i += 4) {
				dpd[0] = d64zero;
				dpd++;
			}

			mask =
			    vis_edge16(dpd, (void *)((mlib_addr)(dst + m - 1)));
			vis_pst_16(d64zero, dpd, mask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 m,
    mlib_s32 n)
{
	return (__mlib_SignalMulRectangular_S16_S16(dst, src, m + m, n + n));
}

/* *********************************************************** */
