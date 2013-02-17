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

#pragma ident	"@(#)mlib_v_SignalCrossCorrel.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_SignalCrossCorrel - calculates image cross correlation
 *                               for the input signals
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalCrossCorrel_S16(mlib_d64       *correl,
 *                                             const mlib_s16 *src1,
 *                                             const mlib_s16 *src2,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalCrossCorrel_S16S(mlib_d64       *correl,
 *                                              const mlib_s16 *src1,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      src1     the first input signal array
 *      src2     the second input signal array
 *      n        number of samples in the signal arrays
 *
 * DESCRIPTION
 *
 *           1    N-1
 *      c = --- * SUM (x[n] * y[n])
 *           N    n = 0
 */

#include <mlib_signal.h>
#include <mlib_SignalCorrel.h>
#include <mlib_v_SignalArith.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalCrossCorrel_S16 = __mlib_SignalCrossCorrel_S16
#pragma weak mlib_SignalCrossCorrel_S16S = __mlib_SignalCrossCorrel_S16S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalCrossCorrel_S16) mlib_SignalCrossCorrel_S16
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16")));
__typeof__(__mlib_SignalCrossCorrel_S16S) mlib_SignalCrossCorrel_S16S
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 rez[4];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, n);
	correl[0] =
	    (rez[0] + rez[1] + rez[2] + rez[3]) / (n * (mlib_d64)(1 << 30));
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16S(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 rez[4];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, n + n);
	correl[0] = (rez[0] + rez[2]) / (n * (mlib_d64)(1 << 30));
	correl[1] = (rez[1] + rez[3]) / (n * (mlib_d64)(1 << 30));
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16_S32                                                \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	dc1.db = vis_fpadd32(dr1, dr2);                            \
	dc2.db = vis_fpadd32(dr3, dr4)

/* *********************************************************** */

#define	SUM_D64                                                 \
	c1 += (mlib_d64)dc1.two_int.int0;                       \
	c2 += (mlib_d64)dc1.two_int.int1;                       \
	c3 += (mlib_d64)dc2.two_int.int0;                       \
	c4 += (mlib_d64)dc2.two_int.int1

/* *********************************************************** */

void
mlib_SignalCrossCorrel_Intr_S16(
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_d64 *rez,
    mlib_s32 n)
{
/* pointer to the data of first signal array */
	mlib_s16 *psrc1 = (void *)src1;

/* pointer to the data of second signal array */
	mlib_s16 *psrc2 = (void *)src2;

/* accumulators */
	mlib_d64 c1, c2, c3, c4;

/* number of samples */
	mlib_s32 size = n;

/* indices */
	mlib_s32 j;

/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *psrc1_end;
	mlib_d64 midd[4], *dpsrc1, *dpsrc2, *dpsrc1_end;
	mlib_d64 dx, dy, dy0, dy1, dr1, dr2, dr3, dr4;
	type_union_mlib_d64 dc1, dc2;

	midd[0] = midd[1] = 0;
	rez[0] = rez[1] = rez[2] = rez[3] = 0;
	c1 = c2 = c3 = c4 = 0;

/* prepare the source address */
	psrc1_end = psrc1 + size - 1;
	dpsrc1_end = (mlib_d64 *)((mlib_addr)psrc1_end & (~7));
	dpsrc1 = (mlib_d64 *)((mlib_addr)psrc1 & (~7));
	off = (mlib_addr)dpsrc1 - (mlib_addr)psrc1;

/* generate edge mask for the start point */
	emask = vis_edge16(psrc1, psrc1_end);

	vis_pst_16(dpsrc1[0], midd, emask);
	dx = midd[0];
	dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, off);
	dy0 = vis_ld_d64_nf(dpsrc2);
	dy1 = vis_ld_d64_nf(dpsrc2 + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16_S32;
	SUM_D64;
	dpsrc1++;
	dpsrc2++;
	j = dpsrc1_end - dpsrc1;

	if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
		for (; j > 0; j--) {
			dx = dpsrc1[0];
			dy = dpsrc2[0];
			MUL_S16_S32;
			SUM_D64;
			dpsrc1++;
			dpsrc2++;
		}
	} else {
		for (; j > 0; j--) {
			dx = dpsrc1[0];
			dy0 = dy1;
			dy1 = dpsrc2[1];
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S32;
			SUM_D64;
			dpsrc1++;
			dpsrc2++;
		}
	}

	if ((mlib_addr)dpsrc1 <= (mlib_addr)psrc1_end) {
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpsrc1, psrc1_end);
		vis_pst_16(dpsrc1[0], midd + 1, emask);
		dx = midd[1];
		dy = vis_faligndata(vis_ld_d64_nf(dpsrc2),
			vis_ld_d64_nf(dpsrc2 + 1));
		MUL_S16_S32;
		SUM_D64;
	}

	midd[0] = c1;
	midd[1] = c2;
	midd[2] = c3;
	midd[3] = c4;
	rez[0] += midd[(-(off >>= 1)) & 3];
	rez[1] += midd[(1 - off) & 3];
	rez[2] += midd[(2 - off) & 3];
	rez[3] += midd[(3 - off) & 3];
}

/* *********************************************************** */
