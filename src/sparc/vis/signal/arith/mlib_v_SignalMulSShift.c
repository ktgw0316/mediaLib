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

#pragma ident	"@(#)mlib_v_SignalMulSShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSShift_[S16|S16S]_Sat,
 *      mlib_SignalMulSShift_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal array by a scalar
 *                     with shifting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulSShift_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                     const mlib_s16 *src,
 *                                                     const mlib_s16 *c,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalMulSShift_S16S_Sat(mlib_s16       *data,
 *                                                const mlib_s16 *c,
 *                                                mlib_s32       shift,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalMulSShift_S16_S16_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       shift,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSShift_S16_Sat(mlib_s16       *data,
 *                                               const mlib_s16 *c,
 *                                               mlib_s32       shift,
 *                                               mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      shift Left shifting factor, may be negative
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src * c * (2 ** shift)
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSShift_S16S_S16S_Sat = \
	__mlib_SignalMulSShift_S16S_S16S_Sat
#pragma weak mlib_SignalMulSShift_S16_S16_Sat = \
	__mlib_SignalMulSShift_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSShift_S16S_S16S_Sat)
    mlib_SignalMulSShift_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShift_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSShift_S16_S16_Sat) mlib_SignalMulSShift_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShift_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_4MUL                                               \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src), v_mul);       \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src), v_mul);       \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src), v_mul);       \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src), v_mul)

/* *********************************************************** */

#define	MLIB_4ADD                                               \
	v_dst0 = vis_fpadd32(dr0, dr1);                         \
	v_dst1 = vis_fpadd32(dr2, dr3);                         \
	v_dst0 = vis_fpadd32(v_dst0, v_dst0);                   \
	v_dst1 = vis_fpadd32(v_dst1, v_dst1)

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShift_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_d64 *dpd, *dps, *dpzend;
	mlib_d64 dr0, dr1, dr2, dr3, v_dst, v_dst0, v_dst1, v_src, v_src0,
	    v_src1;
	mlib_f32 v_mul;
	mlib_s32 times, t;
	mlib_s32 c0 = *c;
	mlib_s32 i;
	mlib_s16 *pzend = dst + n - 1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (shift <= 0) {
		mlib_s16 scal;

		if (shift <= -16)
			scal = 0;
		else if (c0 >= 0)
			scal = c0 >> (-shift);
		else
			scal = -((-c0) >> (-shift));
		return (__mlib_SignalMulS_S16_S16_Sat(dst, src, &scal, n));
	}

	if (shift > 15) {
		if (shift > 30)
			shift = 30;

		c0 <<= shift - 15;
		shift = 15;

		if (c0 > 32767)
			c0 = 32767;
	}

	if (c0 < -32767)
		c0 = -32767;

	vis_write_gsr((shift) << 3);
	v_mul = vis_to_float((c0 & 0xFFFF) | (c0 << 16));

	dpd = (mlib_d64 *)((mlib_addr)dst & ~7);
	t = (mlib_addr)dpd - (mlib_addr)dst;
	dps = vis_alignaddr((void *)src, t);
	dpzend = (mlib_d64 *)((mlib_addr)(pzend) & (~7));

	v_src1 = vis_ld_d64_nf(dps);
	v_src0 = vis_ld_d64_nf(dps + 1);
	v_src = vis_faligndata(v_src1, v_src0);
	MLIB_4MUL;
	MLIB_4ADD;
	v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
	vis_pst_16(v_dst, dpd, vis_edge16(dst, pzend));
	dpd++;
	dps++;

	v_src1 = vis_ld_d64_nf(dps + 1);
	v_src = vis_faligndata(v_src0, v_src1);
	MLIB_4MUL;
	MLIB_4ADD;
	v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
	times = (dpzend - dpd);

	v_src0 = vis_ld_d64_nf(dps + 2);
	v_src = vis_faligndata(v_src1, v_src0);
	MLIB_4MUL;

	v_src1 = vis_ld_d64_nf(dps + 3);
	dps += 4;
	v_src = vis_faligndata(v_src0, v_src1);

	if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0)
#pragma pipeloop(0)
		for (i = 2; i < times; i += 2) {
			dpd[0] = v_dst;
			MLIB_4ADD;
			MLIB_4MUL;
			v_src = v_src1;

			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);

			MLIB_4ADD;
			MLIB_4MUL;
			v_src = vis_ld_d64_nf(dps);

			dpd[1] = v_dst;
			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
			v_src1 = vis_ld_d64_nf(dps + 1);
			dpd += 2;
			dps += 2;
	} else
#pragma pipeloop(0)
		for (i = 2; i < times; i += 2) {
			v_src0 = vis_ld_d64_nf(dps);
			MLIB_4ADD;
			MLIB_4MUL;
			v_src = vis_faligndata(v_src1, v_src0);

			v_src1 = vis_ld_d64_nf(dps + 1);
			dpd[0] = v_dst;
			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);

			MLIB_4ADD;
			MLIB_4MUL;
			v_src = vis_faligndata(v_src0, v_src1);

			dpd[1] = v_dst;
			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
			dpd += 2;
			dps += 2;
		}

	switch (i - times) {
	case 0:
		*dpd = v_dst;
		dpd++;
		MLIB_4ADD;
		MLIB_4MUL;
		v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
	case 1:
		*dpd = v_dst;
		dpd++;
		MLIB_4ADD;
		v_dst = vis_fpackfix_pair(v_dst0, v_dst1);

	case 2:
		vis_pst_16(v_dst, dpd, vis_edge16(dpd, pzend));

	default:
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShift_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_d64 *dpd, *dps, *dpzend;
	mlib_d64 dr0, dr1, dr2, dr3, v_dst, v_dst0, v_dst1, v_src, v_src0,
	    v_src1;
	mlib_f32 v_mul;
	mlib_s32 times, t;
	mlib_s32 c0 = c[0], c1 = c[1];
	mlib_s32 i;
	mlib_s16 *pzend = dst + 2 * n - 1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (shift <= 0) {
		mlib_s16 scal[2];

		if (shift <= -16)
			scal[0] = scal[1] = 0;
		else {
			if (c[0] >= 0)
				scal[0] = c[0] >> (-shift);
			else
				scal[0] = -((-c[0]) >> (-shift));

			if (c[1] >= 0)
				scal[1] = c[1] >> (-shift);
			else
				scal[1] = -((-c[1]) >> (-shift));
		}

		return (__mlib_SignalMulS_S16S_S16S_Sat(dst, src, scal, n));
	}

	if (shift > 15) {
		if (shift > 30)
			shift = 30;

		c0 <<= shift - 15;
		c1 <<= shift - 15;
		shift = 15;

		if (c0 > 32767)
			c0 = 32767;

		if (c1 > 32767)
			c1 = 32767;
	}

	if (c0 < -32767)
		c0 = -32767;

	if (c1 < -32767)
		c1 = -32767;

	vis_write_gsr((shift) << 3);

	if ((mlib_addr)dst & 2)
		v_mul = vis_to_float((c0 & 0xFFFF) | (c1 << 16));
	else
		v_mul = vis_to_float((c1 & 0xFFFF) | (c0 << 16));

	dpd = (mlib_d64 *)((mlib_addr)dst & ~7);
	t = (mlib_addr)dpd - (mlib_addr)dst;
	dps = vis_alignaddr((void *)src, t);
	dpzend = (mlib_d64 *)((mlib_addr)(pzend) & (~7));

	v_src1 = vis_ld_d64_nf(dps);
	v_src0 = vis_ld_d64_nf(dps + 1);
	v_src = vis_faligndata(v_src1, v_src0);
	MLIB_4MUL;
	MLIB_4ADD;
	v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
	vis_pst_16(v_dst, dpd, vis_edge16(dst, pzend));
	dpd++;
	dps++;

	v_src1 = vis_ld_d64_nf(dps + 1);
	v_src = vis_faligndata(v_src0, v_src1);
	MLIB_4MUL;
	MLIB_4ADD;
	v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
	times = (dpzend - dpd);

	v_src0 = vis_ld_d64_nf(dps + 2);
	v_src = vis_faligndata(v_src1, v_src0);
	MLIB_4MUL;

	v_src1 = vis_ld_d64_nf(dps + 3);
	dps += 4;
	v_src = vis_faligndata(v_src0, v_src1);

	if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0)
#pragma pipeloop(0)
		for (i = 2; i < times; i += 2) {
			dpd[0] = v_dst;
			MLIB_4ADD;
			MLIB_4MUL;
			v_src = v_src1;

			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);

			MLIB_4ADD;
			MLIB_4MUL;
			v_src = vis_ld_d64_nf(dps);

			dpd[1] = v_dst;
			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
			v_src1 = vis_ld_d64_nf(dps + 1);
			dpd += 2;
			dps += 2;
	} else
#pragma pipeloop(0)
		for (i = 2; i < times; i += 2) {
			v_src0 = vis_ld_d64_nf(dps);
			MLIB_4ADD;
			MLIB_4MUL;
			v_src = vis_faligndata(v_src1, v_src0);

			v_src1 = vis_ld_d64_nf(dps + 1);
			dpd[0] = v_dst;
			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);

			MLIB_4ADD;
			MLIB_4MUL;
			v_src = vis_faligndata(v_src0, v_src1);

			dpd[1] = v_dst;
			v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
			dpd += 2;
			dps += 2;
		}

	switch (i - times) {
	case 0:
		*dpd = v_dst;
		dpd++;
		MLIB_4ADD;
		MLIB_4MUL;
		v_dst = vis_fpackfix_pair(v_dst0, v_dst1);
	case 1:
		*dpd = v_dst;
		dpd++;
		MLIB_4ADD;
		v_dst = vis_fpackfix_pair(v_dst0, v_dst1);

	case 2:
		vis_pst_16(v_dst, dpd, vis_edge16(dpd, pzend));

	default:
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */
