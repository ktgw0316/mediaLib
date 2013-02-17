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

#pragma ident	"@(#)mlib_v_SignalMulSAdd.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSAdd_[S16|S16S]_Sat,
 *      mlib_SignalMulSAdd_[S16|S16S]_[S16|S16S]_Sat - linear function on
 *                     mlib_s32 16-bit format signal array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulSAdd_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src1,
 *                                                   const mlib_s16 *src2,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16S_Sat(mlib_s16       *src1dst,
 *                                              const mlib_s16 *src2,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16_S16_Sat(mlib_s16       *dst,
 *                                                 const mlib_s16 *src1,
 *                                                 const mlib_s16 *src2,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16_Sat(mlib_s16       *src1dst,
 *                                             const mlib_s16 *src2,
 *                                             const mlib_s16 *c,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      src1dst  The first input and output signal array
 *      dst      Output signal array
 *      src1     The first input signal array
 *      src2     The second input signal array
 *      c        Scaling factor. The scaling factor is in Q15 format
 *      n        Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 + c * src2
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSAdd_S16S_S16S_Sat = \
	__mlib_SignalMulSAdd_S16S_S16S_Sat
#pragma weak mlib_SignalMulSAdd_S16_S16_Sat = \
	__mlib_SignalMulSAdd_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSAdd_S16S_S16S_Sat) mlib_SignalMulSAdd_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSAdd_S16_S16_Sat) mlib_SignalMulSAdd_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MULS_S16                                                \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), fc);             \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), fc);             \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), fc);             \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), fc);             \
	dx_hi = vis_fmuld8sux16(two_in_15, vis_read_hi(dx));    \
	dx_lo = vis_fmuld8sux16(two_in_15, vis_read_lo(dx))

/* *********************************************************** */

#define	ADD_AND_PACK_S16                                        \
	dr5 = vis_fpadd32(dr1, dr2);                            \
	dr6 = vis_fpadd32(dr3, dr4);                            \
	dr5 = vis_fpsub32(dr5, dx_hi);                          \
	dr6 = vis_fpsub32(dr6, dx_lo);                          \
	dr = vis_fpackfix_pair(dr5, dr6)

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_u32 sc = ((mlib_u16 *)c)[0];
	mlib_s16 *pzend = dst + n - 1;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1, dx_hi, dx_lo;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_f32 fc = vis_to_float((sc << 16) | (sc));
	mlib_f32 two_in_15 = vis_to_float(0x80008000);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(1 << 3);

/* prepare the destination address */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dpz - (mlib_addr)dst;

/* prepare the source address */
	dpy = (mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dpx = (mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, pzend);

	MULS_S16;
	ADD_AND_PACK_S16;

	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 2);
	dy = vis_faligndata(dy0, dy1);
	MULS_S16;
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 3);
	dy = vis_faligndata(dy0, dy1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 3);

/* store first bytes of result */
	vis_pst_16(dr, dpz, emask);
	dpz++;
	len = (mlib_s32)((mlib_addr)dpzend - (mlib_addr)dpz);

	if ((((mlib_addr)src2 ^ (mlib_addr)dst) & 7) == 0) {

/*
 * if((((mlib_addr) src1 ^ (mlib_addr) src2) & 7) == 0) {
 *       dx = dx0;
 *       #pragma pipeloop(0)
 *       for(i = 0; i < len; i+=8){
 *       ADD_AND_PACK_S16;
 *       MULS_S16;
 *       dx = dpx[3];
 *       dy = dpy[3];
 *       dpz[0] = dr;
 *       dpx++;
 *       dpy++;
 *       dpz++;
 *       }
 *       }
 *       else
 */
		{
#pragma pipeloop(0)
			for (i = 0; i < len; i += 8) {
				dx = vis_faligndata(dx0, dx1);
				ADD_AND_PACK_S16;
				MULS_S16;
				dy = vis_ld_d64_nf(dpy + 3);
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}
		}
	}

	else {
#pragma pipeloop(0)
		for (i = 0; i < len - 8; i += 16) {
			dx = vis_faligndata(dx0, dx1);
			dx0 = vis_ld_d64_nf(dpx + 4);
			dy0 = vis_ld_d64_nf(dpy + 4);
			ADD_AND_PACK_S16;
			MULS_S16;
			dx = vis_faligndata(dx1, dx0);
			(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
			dy = vis_faligndata(dy1, dy0);
			dy1 = vis_ld_d64_nf(dpy + 5);
			dx1 = vis_ld_d64_nf(dpx + 5);
			dpz[0] = dr;	/* store 8 bytes of result */

			ADD_AND_PACK_S16;
			MULS_S16;
			dy = vis_faligndata(dy0, dy1);
			(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
/* store 8 bytes of result */
			dpz[1] = dr;
			dpx += 2;
			dpy += 2;
			dpz += 2;
		}

		if (i < len) {
			dx = vis_faligndata(dx0, dx1);
			ADD_AND_PACK_S16;
			MULS_S16;
/* store 8 bytes of result */
			dpz[0] = dr;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		ADD_AND_PACK_S16;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpz, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_u32 sc0 = ((mlib_u16 *)c)[0];
	mlib_u32 sc1 = ((mlib_u16 *)c)[1];
	mlib_s16 *pzend = dst + 2 * n - 1;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1, dx_hi, dx_lo;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_f32 fc = ((mlib_addr)dst & 2) ? vis_to_float((sc1 << 16) | (sc0))
	    : vis_to_float((sc0 << 16) | (sc1));
	mlib_f32 two_in_15 = vis_to_float(0x80008000);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(1 << 3);

/* prepare the destination address */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dpz - (mlib_addr)dst;

/* prepare the source address */
	dpy = (mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dpx = (mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, pzend);

	MULS_S16;
	ADD_AND_PACK_S16;

	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 2);
	dy = vis_faligndata(dy0, dy1);
	MULS_S16;
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 3);
	dy = vis_faligndata(dy0, dy1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 3);

/* store first bytes of result */
	vis_pst_16(dr, dpz, emask);
	dpz++;
	len = (mlib_s32)((mlib_addr)dpzend - (mlib_addr)dpz);

	if ((((mlib_addr)src2 ^ (mlib_addr)dst) & 7) == 0) {

/*
 * if((((mlib_addr) src1 ^ (mlib_addr) src2) & 7) == 0) {
 *       dx = dx0;
 *       #pragma pipeloop(0)
 *       for(i = 0; i < len; i+=8){
 *       ADD_AND_PACK_S16;
 *       MULS_S16;
 *       dx = dpx[3];
 *       dy = dpy[3];
 *       dpz[0] = dr;
 *       dpx++;
 *       dpy++;
 *       dpz++;
 *       }
 *       }
 *       else
 */
		{
#pragma pipeloop(0)
			for (i = 0; i < len; i += 8) {
				dx = vis_faligndata(dx0, dx1);
				ADD_AND_PACK_S16;
				MULS_S16;
				dy = vis_ld_d64_nf(dpy + 3);
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}
		}
	}

	else {
#pragma pipeloop(0)
		for (i = 0; i < len - 8; i += 16) {
			dx = vis_faligndata(dx0, dx1);
			dx0 = vis_ld_d64_nf(dpx + 4);
			dy0 = vis_ld_d64_nf(dpy + 4);
			ADD_AND_PACK_S16;
			MULS_S16;
			dx = vis_faligndata(dx1, dx0);
			(void *)(mlib_d64 *)vis_alignaddr((void *)src2, off);
			dy = vis_faligndata(dy1, dy0);
			dy1 = vis_ld_d64_nf(dpy + 5);
			dx1 = vis_ld_d64_nf(dpx + 5);
			dpz[0] = dr;	/* store 8 bytes of result */

			ADD_AND_PACK_S16;
			MULS_S16;
			dy = vis_faligndata(dy0, dy1);
			(void *)(mlib_d64 *)vis_alignaddr((void *)src1, off);
/* store 8 bytes of result */
			dpz[1] = dr;
			dpx += 2;
			dpy += 2;
			dpz += 2;
		}

		if (i < len) {
			dx = vis_faligndata(dx0, dx1);
			ADD_AND_PACK_S16;
			MULS_S16;
/* store 8 bytes of result */
			dpz[0] = dr;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		ADD_AND_PACK_S16;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpz, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
