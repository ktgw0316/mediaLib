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

#pragma ident	"@(#)mlib_v_SignalMulS.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulS_[S16|S16S]_Sat,
 *      mlib_SignalMulS_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal array to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulS_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                const mlib_s16 *c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16S_Sat(mlib_s16       *data,
 *                                           const mlib_s16 *c,
 *                                           mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16_S16_Sat(mlib_s16       *dst,
 *                                              const mlib_s16 *src,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16_Sat(mlib_s16       *data,
 *                                          const mlib_s16 *c,
 *                                          mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src * c
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulS_S16S_S16S_Sat = __mlib_SignalMulS_S16S_S16S_Sat
#pragma weak mlib_SignalMulS_S16_S16_Sat = __mlib_SignalMulS_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulS_S16S_S16S_Sat) mlib_SignalMulS_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulS_S16_S16_Sat) mlib_SignalMulS_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MULS16_1                                                \
	dr1 = vis_fmul8sux16(dx, dc);                           \
	dr2 = vis_fmul8ulx16(dx, dc);                           \
	dr = vis_fpadd16(dr1, dr2)

/* *********************************************************** */

#define	MULS16_2	MULS16_1; dr = vis_fpadd16(dr, dr)

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 i, len;
	mlib_u16 uc = c[0];
	mlib_s16 *pzend = dst + n - 1;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2;
	mlib_d64 dc;

	if (n <= 0)
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

	if (*c <= -32767) {
		dr = vis_fnot(dx);
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);
		len = dpzend - dpz;

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx1 = dpx[1];
				dpz[0] = vis_fnot(dx);
				dx = dx1;
				dpx++;
				dpz++;
			}

		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				dpz[0] = vis_fnot(dx);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			dr = vis_fnot(dx);
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
		return (MLIB_SUCCESS);
	}

/* |scale| <= 2**14 */
	if ((uc >> 15) == (uc >> 14)) {

/* prepare the scaling factors */
		dc = vis_to_double_dup((uc << 17) | ((uc << 1) & 0xffff));

		MULS16_1;
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);
		len = dpzend - dpz;

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = (*dpx++);
				MULS16_1;
				(*dpz++) = dr;
			}

			dx = vis_ld_d64_nf(dpx);
		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS16_1;
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MULS16_1;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
	} else {

/* prepare the scaling factors */
		dc = vis_to_double_dup((uc << 16) | uc);

		MULS16_2;
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);
		len = dpzend - dpz;

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx1 = dpx[1];
				MULS16_2;
				dpz[0] = dr;
				dx = dx1;
				dpx++;
				dpz++;
			}

		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS16_2;
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MULS16_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 len;
	mlib_u16 uc0 = c[0];
	mlib_u16 uc1 = c[1];
	mlib_s16 *pzend = dst + 2 * n - 1;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2;
	mlib_d64 dc;

	if (n <= 0)
		return (MLIB_FAILURE);

/* prepare the destination address */
	dpz = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dpz - (mlib_addr)dst;

/* prepare the source address */
	dpx = (mlib_d64 *)
	    vis_alignaddr((void *)src, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/* generate edge mask for the start point */
	emask = vis_edge16(dst, pzend);
	dx = vis_faligndata(dx0, dx1);

	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpx++;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 1);

	if (*c <= -32767 || *(c + 1) <= -32767) {
		mlib_s32 mask;
		mlib_d64 d_over = vis_to_double_dup(0x80018001);
		mlib_d64 dr3;

/* prepare the scaling factors */
		dc = ((mlib_addr)dst & 2) ? vis_to_double_dup((uc1 << 16) | uc0)
		    : vis_to_double_dup((uc0 << 16) | uc1);

		mask = vis_fcmple16(dc, d_over);
		dr3 = vis_fnot(dx);
		MULS16_2;
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		vis_pst_16(dr3, dpz, emask & mask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);
		len = dpzend - dpz;

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx1 = dpx[1];
				dr3 = vis_fnot(dx);
				MULS16_2;
				dpz[0] = dr;
				vis_pst_16(dr3, dpz, mask);
				dx = dx1;
				dpx++;
				dpz++;
			}

		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				dr3 = vis_fnot(dx);
				MULS16_2;
				dpz[0] = dr;
				vis_pst_16(dr3, dpz, mask);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			dr3 = vis_fnot(dx);
			MULS16_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
			vis_pst_16(dr3, dpz, emask & mask);
		}
		return (MLIB_SUCCESS);
	}

/* |scale| <= 2**14 */
	if (((uc0 >> 15) == (uc0 >> 14)) && ((uc1 >> 15) == (uc1 >> 14))) {

/* prepare the scaling factors */
		dc = ((mlib_addr)dst & 2) ? vis_to_double_dup((uc1 << 17) |
		    ((uc0 << 1) & 0xffff))
		    : vis_to_double_dup((uc0 << 17) | ((uc1 << 1) & 0xffff));

		MULS16_1;
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);
		len = dpzend - dpz;

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx1 = dpx[1];
				MULS16_1;
				dpz[0] = dr;
				dx = dx1;
				dpx++;
				dpz++;
			}

		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS16_1;
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MULS16_1;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
	} else {

/* prepare the scaling factors */
		dc = ((mlib_addr)dst & 2) ? vis_to_double_dup((uc1 << 16) | uc0)
		    : vis_to_double_dup((uc0 << 16) | uc1);

		MULS16_2;
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);
		len = dpzend - dpz;

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx1 = dpx[1];
				MULS16_2;
				dpz[0] = dr;
				dx = dx1;
				dpx++;
				dpz++;
			}

		} else {
#pragma pipeloop(0)
			for (; len > 0; len--) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS16_2;
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MULS16_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
