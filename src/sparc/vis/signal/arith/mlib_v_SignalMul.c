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

#pragma ident	"@(#)mlib_v_SignalMul.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMul_[S16|S16S]_Sat,
 *      mlib_SignalMul_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     signed 16-bit format signal arrays
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMul_S16S_S16S_Sat(mlib_s16 *dst,
 *                                               const mlib_s16 *src1,
 *                                               const mlib_s16 *src2,
 *                                               mlib_s32 n);
 *      mlib_status mlib_SignalMul_S16S_Sat(mlib_s16 *src1dst,
 *                                          const mlib_s16 *src2,
 *                                          mlib_s32 n);
 *      mlib_status mlib_SignalMul_S16_S16_Sat(mlib_s16 *dst,
 *                                             const mlib_s16 *src1,
 *                                             const mlib_s16 *src2,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalMul_S16_Sat(mlib_s16 *src1dst,
 *                                         const mlib_s16 *src2,
 *                                         mlib_s32 n);
 *
 * ARGUMENTS
 *   src1dst   The first input and the output signal array
 *      dst    Output signal array
 *      src1   The first input signal array
 *      src2   The second input signal array
 *      n      Number of samples in the input signal arrays
 *
 * DESCRIPTION
 *      dst = src1 * src2
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMul_S16_S16_Sat = __mlib_SignalMul_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMul_S16_S16_Sat) mlib_SignalMul_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMul_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_S16                                                 \
	dr1 = vis_fmul8sux16(dx, dy);                           \
	dr2 = vis_fmul8ulx16(dx, dy)

/* *********************************************************** */

#define	MUL2_S16                                                \
	dr1 = vis_fmul8sux16(dx2, dy2);                         \
	dr2 = vis_fmul8ulx16(dx2, dy2)

/* *********************************************************** */

#define	ADD_S16                                                 \
	dr = vis_fpadd16(dr1, dr2);                             \
	dr = vis_fpadd16(dr, dr);                               \
	dr = vis_fpadd16(dr, dmone)

/* *********************************************************** */

mlib_status
__mlib_SignalMul_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend = dst + n - 1;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1, dx2, dy2;
	mlib_d64 dr, dr1, dr2;
	mlib_d64 dmone = vis_fone();

	if (n <= 0)
		return (MLIB_FAILURE);

/* prepare the destination address */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dpz - (mlib_addr)dst;

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, pzend);
	MUL_S16;
	ADD_S16;
/* store first bytes of result */
	vis_pst_16(dr, dpz, emask);
	dpz++;

	(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 2);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16;
	(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 3);
	dx = vis_faligndata(dx0, dx1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 3);
	dy = vis_faligndata(dy0, dy1);

	len = dpzend - dpz;

	if ((((mlib_addr)src1 ^ (mlib_addr)dst) & 7) == 0) {

		if ((((mlib_addr)src1 ^ (mlib_addr)src2) & 7) == 0) {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				ADD_S16;
				MUL_S16;
				dy = vis_ld_d64_nf(dpy + 3);
				dx = vis_ld_d64_nf(dpx + 3);
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}
		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy + 4);
				ADD_S16;
				MUL_S16;
				dx = vis_ld_d64_nf(dpx + 3);
				dpz[0] = dr;
				dy = vis_faligndata(dy0, dy1);
				dpx++;
				dpy++;
				dpz++;
			}
		}
	} else {
#pragma pipeloop(0)
		(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
		for (; len > 1; len -= 2) {
			dx0 = vis_ld_d64_nf(dpx + 4);
			ADD_S16;
			MUL_S16;
			dx2 = vis_faligndata(dx1, dx0);
			dx1 = vis_ld_d64_nf(dpx + 5);
			dx = vis_faligndata(dx0, dx1);
			dy0 = vis_ld_d64_nf(dpy + 4);
			(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
			dy2 = vis_faligndata(dy1, dy0);
			dy1 = vis_ld_d64_nf(dpy + 5);
			dy = vis_faligndata(dy0, dy1);

			dpz[0] = dr;
			ADD_S16;
			MUL2_S16;
			dpz[1] = dr;
			dpx += 2;
			dpy += 2;
			dpz += 2;
			(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
		}

		if (len > 0) {
			ADD_S16;
			MUL_S16;
			dpz[0] = dr;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		ADD_S16;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpz, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
