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

#pragma ident	"@(#)mlib_v_SignalEmphasize.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalEmphasizeInit_[S16|S16S]_[S16|S16S] - allocates memory
 *          for internal state structure and converts the filter
 *          coefficients into internal representation
 *
 *      mlib_SignalEmphasize_[S16|S16S]_[S16|S16S]_Sat - apply the
 *         preemphasizer to one packet of signal and update the filter states
 *
 *      mlib_SignalEmphasizeFree_[S16|S16S]_[S16|S16S] - releases the memory
 *          allocated for the internal states structure
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalEmphasizeInit_S16S_S16S(void     **filter,
 *                                                     mlib_f32 alpha);
 *      mlib_status mlib_SignalEmphasizeInit_S16_S16(void     **filter,
 *                                                   mlib_f32 alpha);
 *      mlib_status mlib_SignalEmphasize_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                     const mlib_s16 *src,
 *                                                     void           *filter,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalEmphasize_S16_S16_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src,
 *                                                   void           *filter,
 *                                                   mlib_s32       n);
 *      void mlib_SignalEmphasizeFree_S16S_S16S(void *filter);
 *      void mlib_SignalEmphasizeFree_S16_S16(void *filter);
 *
 * ARGUMENTS
 *      filter   Internal filter structure
 *      alpha    Emphasizing coefficients, 0 < alpha < 1.0
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *     Pre-Emphasizing:     z[i] = x[i] - alpha*x[i-1]
 */

/* *********************************************************** */

#include <mlib_signal.h>
#include <vis_proto.h>
#include <stdio.h>
#include <stdlib.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalEmphasizeFree_S16S_S16S = \
	__mlib_SignalEmphasizeFree_S16S_S16S
#pragma weak mlib_SignalEmphasizeFree_S16_S16 = \
	__mlib_SignalEmphasizeFree_S16_S16
#pragma weak mlib_SignalEmphasizeInit_S16S_S16S = \
	__mlib_SignalEmphasizeInit_S16S_S16S
#pragma weak mlib_SignalEmphasizeInit_S16_S16 = \
	__mlib_SignalEmphasizeInit_S16_S16
#pragma weak mlib_SignalEmphasize_S16S_S16S_Sat = \
	__mlib_SignalEmphasize_S16S_S16S_Sat
#pragma weak mlib_SignalEmphasize_S16_S16_Sat = \
	__mlib_SignalEmphasize_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalEmphasizeFree_S16S_S16S)
    mlib_SignalEmphasizeFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalEmphasizeFree_S16S_S16S")));
__typeof__(__mlib_SignalEmphasizeFree_S16_S16) mlib_SignalEmphasizeFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalEmphasizeFree_S16_S16")));
__typeof__(__mlib_SignalEmphasizeInit_S16S_S16S)
    mlib_SignalEmphasizeInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalEmphasizeInit_S16S_S16S")));
__typeof__(__mlib_SignalEmphasizeInit_S16_S16) mlib_SignalEmphasizeInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalEmphasizeInit_S16_S16")));
__typeof__(__mlib_SignalEmphasize_S16S_S16S_Sat)
    mlib_SignalEmphasize_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalEmphasize_S16S_S16S_Sat")));
__typeof__(__mlib_SignalEmphasize_S16_S16_Sat) mlib_SignalEmphasize_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalEmphasize_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_EMPH	12

/* *********************************************************** */

typedef struct mlib_emphasize
{
	mlib_s32 type;
	mlib_f32 v_alpha;
	mlib_f32 v16_last0, v16_last1;
} mlib_emphasize_struct;

/* *********************************************************** */

#define	MLIB_MUL8                                               \
	dr0 = vis_fmuld8sux16(v_mask, vis_read_hi(w_src));      \
	dr1 = vis_fmuld8sux16(v_mask, vis_read_lo(w_src));      \
	dr4 = vis_fmuld8ulx16(vis_read_hi(w_lsrc), v_alpha);    \
	dr5 = vis_fmuld8sux16(vis_read_hi(w_lsrc), v_alpha);    \
	dr6 = vis_fmuld8ulx16(vis_read_lo(w_lsrc), v_alpha);    \
	dr7 = vis_fmuld8sux16(vis_read_lo(w_lsrc), v_alpha)

/* *********************************************************** */

#define	MLIB_ADD6                                               \
	dr2 = vis_fpsub32(dr4, dr0);                            \
	dr2 = vis_fpadd32(dr2, dr5);                            \
	dr3 = vis_fpsub32(dr6, dr1);                            \
	dr3 = vis_fpadd32(dr3, dr7)

/* *********************************************************** */

#define	MLIB_MIX	MLIB_ADD6; MLIB_MUL8

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;
	mlib_d64 w_maskand = vis_to_double(0xFFFFFFFF, 0xFFFF0000);
	mlib_d64 w_maskor  = vis_freg_pair(0.f, fist->v16_last0);
	mlib_f32 v_mask    = vis_to_float(0x80008000);
	mlib_f32 v_alpha   = fist->v_alpha;
	mlib_s16 *fdst	   = dst + n - 1;
	mlib_d64 *dpd, *dps;
	mlib_d64 w_dst, w_src, w_src0, w_src1, w_lsrc;
	mlib_d64 dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
	mlib_s32 i, times, t1, t2;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH)) {
		return (MLIB_FAILURE);
	}

	vis_write_gsr(1 << 3);
	w_maskor = vis_fand(w_maskor, vis_to_double(0xFFFFFFFF, 0xFFFF));
	dpd = vis_alignaddr(dst, 0);
	times = (mlib_d64 *)vis_alignaddr(fdst, 0) - dpd;
	t1 = -((mlib_addr)dst & 7);
	t2 = t1 - 2;

	if (((mlib_addr)dst ^ (mlib_addr)src) & 7) {
		vis_alignaddr((void *)-(mlib_addr)src, 0);
		w_maskand = vis_faligndata(w_maskand, w_maskand);
		w_maskor = vis_faligndata(w_maskor, w_maskor);
		dps = vis_alignaddr((void *)src, t2);
		w_src0 = vis_ld_d64_nf(dps);
		dps++;
		w_src1 = vis_ld_d64_nf(dps);
		dps++;

		if (((mlib_addr)dps - (mlib_addr)src) >= 8) {
			w_src0 = vis_fand(w_maskand, w_src0);
			w_src0 = vis_for(w_maskor, w_src0);
		} else {
			w_src1 = vis_fand(w_maskand, w_src1);
			w_src1 = vis_for(w_maskor, w_src1);
		}

		w_lsrc = vis_faligndata(w_src0, w_src1);
		vis_alignaddr((void *)src, t1);
		w_src = vis_faligndata(w_src0, w_src1);
		MLIB_MUL8;

		if ((mlib_addr)dst & 7) {
			times--;
			w_src0 = w_src1;
			w_src1 = vis_ld_d64_nf(dps);
			vis_alignaddr((void *)src, t2);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			vis_alignaddr((void *)src, t1);
			w_src = vis_faligndata(w_src0, w_src1);
			dps++;

			MLIB_MIX;

			w_dst = vis_fpackfix_pair(dr2, dr3);
			vis_pst_16(w_dst, dpd, vis_edge16(dst, fdst));
			dpd++;
		}

		for (i = 0; i < times; i++) {
			w_src0 = w_src1;
			w_src1 = vis_ld_d64_nf(dps);
			vis_alignaddr((void *)src, t2);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			vis_alignaddr((void *)src, t1);
			w_src = vis_faligndata(w_src0, w_src1);

			MLIB_MIX;

			w_dst = vis_fpackfix_pair(dr2, dr3);
			*dpd = w_dst;
			dps++;
			dpd++;
		}
	} else {
		dps = vis_alignaddr((void *)src, t2);
		w_src0 = vis_ld_d64_nf(dps);
		dps++;
		w_src1 = vis_ld_d64_nf(dps);
		dps++;
		w_src = w_src1;

		if ((mlib_addr)src & 7) {
			vis_alignaddr((void *)(-(mlib_addr)src), 0);
			w_maskand = vis_faligndata(w_maskand, w_maskand);
			w_maskor = vis_faligndata(w_maskor, w_maskor);
			vis_alignaddr((void *)src, t2);
			times--;
			w_src1 = vis_fand(w_maskand, w_src1);
			w_src1 = vis_for(w_maskor, w_src1);
			w_lsrc = vis_faligndata(w_src0, w_src1);

			MLIB_MUL8;

			w_src0 = w_src1;
			w_src1 = vis_ld_d64_nf(dps);
			w_src = w_src1;
			w_lsrc = vis_faligndata(w_src0, w_src1);
			dps++;

			MLIB_MIX;

			w_dst = vis_fpackfix_pair(dr2, dr3);
			vis_pst_16(w_dst, dpd, vis_edge16(dst, fdst));
			dpd ++;
		} else {
			w_src0 = vis_fand(w_maskand, w_src0);
			w_src0 = vis_for(w_maskor, w_src0);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			MLIB_MUL8;
		}


		for (i = 0; i < times; i++) {
			w_src0 = w_src1;
			w_src1 = vis_ld_d64_nf(dps);
			w_src = w_src1;
			w_lsrc = vis_faligndata(w_src0, w_src1);

			MLIB_MIX;

			w_dst = vis_fpackfix_pair(dr2, dr3);
			*dpd = w_dst;
			dps++;
			dpd++;
		}
	}
	if (times >= 0) {
		MLIB_ADD6;
		w_dst = vis_fpackfix_pair(dr2, dr3);
		vis_pst_16(w_dst, dpd, vis_edge16(dpd, fdst));
	}

	((mlib_s16 *)&fist->v16_last0)[1] = src[n - 1];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasizeInit_S16_S16(
    void **filter,
    mlib_f32 alpha)
{
	mlib_emphasize_struct *fist;
	mlib_s32 t;

/* check for obvious errors */

	if (filter == NULL) {
		return (MLIB_FAILURE);
	}

	fist = __mlib_malloc(sizeof (mlib_emphasize_struct));

	if (fist == NULL) {
		return (MLIB_FAILURE);
	}
	fist->type = MLIB_EMPH;
	t = -alpha * 32768.;
	fist->v_alpha = vis_to_float((t & 0xFFFF) | (t << 16));
	fist->v16_last0 = 0;
	fist->v16_last1 = 0;
	*filter = fist;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalEmphasizeFree_S16_S16(
    void *filter)
{
	mlib_emphasize_struct *fist = filter;

/* check for obvious errors */

	if ((filter == NULL) || (fist->type != MLIB_EMPH))
		return; /* MLIB_FAILURE */

	fist->type = 0;
	__mlib_free(filter);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;
	mlib_d64 w_maskand0 = vis_to_double(0xFFFFFFFF, 0xFFFF);
	mlib_d64 w_maskor0  = vis_freg_pair(0.f, fist->v16_last0);
	mlib_d64 w_maskand1 = vis_to_double(0xFFFFFFFF, 0xFFFF0000);
	mlib_d64 w_maskor1  = vis_freg_pair(0.f, fist->v16_last1);
	mlib_f32 v_mask	    = vis_to_float(0x80008000);
	mlib_f32 v_alpha    = fist->v_alpha;
	mlib_s16 *fdst	    = dst + n + n - 1;
	mlib_d64 *dpd, *dps, *dsrct1;
	mlib_d64 w_dst, w_src, w_src0, w_src1, w_src2, w_lsrc;
	mlib_d64 dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
	mlib_s32 i, times, t1, t2;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH)) {
		return (MLIB_FAILURE);
	}

	vis_write_gsr(1 << 3);
	w_maskor0 = vis_fand(w_maskor0, w_maskand1);
	w_maskor1 = vis_fand(w_maskor1, w_maskand0);

	vis_alignaddr((void *)(-(mlib_addr)src), 0);
	w_maskand0 = vis_faligndata(w_maskand0, w_maskand0);
	w_maskor0 = vis_faligndata(w_maskor0, w_maskor0);
	w_maskand1 = vis_faligndata(w_maskand1, w_maskand1);
	w_maskor1 = vis_faligndata(w_maskor1, w_maskor1);

	dpd = vis_alignaddr(dst, 0);
	times = (mlib_d64 *)vis_alignaddr(fdst, 0) - dpd;
	t1 = -((mlib_addr)(dst) & 7);
	t2 = t1 - 4;
	dps = vis_alignaddr((void *)src, t2);
	w_src0 = vis_ld_d64_nf(dps);
	dps++;
	w_src1 = vis_ld_d64_nf(dps);
	dps++;

	if ((((mlib_addr)dst ^ (mlib_addr)src) & 7)) {
		if (((mlib_addr)dps - (mlib_addr)src) >= 6) {
			w_src0 = vis_fand(w_maskand0, w_src0);
			w_src0 = vis_for(w_maskor0, w_src0);
		} else {
			w_src1 = vis_fand(w_maskand0, w_src1);
			w_src1 = vis_for(w_maskor0, w_src1);
		}

		if (((mlib_addr)dps - (mlib_addr)src) >= 8) {
			w_src0 = vis_fand(w_maskand1, w_src0);
			w_src0 = vis_for(w_maskor1, w_src0);
		} else {
			w_src1 = vis_fand(w_maskand1, w_src1);
			w_src1 = vis_for(w_maskor1, w_src1);
		}

		w_lsrc = vis_faligndata(w_src0, w_src1);
		dsrct1 = vis_alignaddr((void *)src, t1);

		if (dps - 2 != dsrct1) {
			w_src2 = *dps;
			dps++;
			w_src = vis_faligndata(w_src1, w_src2);

			MLIB_MUL8;

			if ((mlib_addr)dst & 7) {
				times--;
				w_src0 = w_src1;
				w_src1 = w_src2;
				w_src2 = *dps;
				vis_alignaddr((void *)src, t2);
				w_lsrc = vis_faligndata(w_src0, w_src1);
				vis_alignaddr((void *)src, t1);
				w_src = vis_faligndata(w_src1, w_src2);
				dps++;

				MLIB_MIX;

				w_dst = vis_fpackfix_pair(dr2, dr3);
				vis_pst_16(w_dst, dpd, vis_edge16(dst, fdst));
				dpd++;
			}

			w_src0 = w_src1;
			w_src1 = w_src2;
			w_src2 = vis_ld_d64_nf(dps);
			vis_alignaddr((void *)src, t2);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			vis_alignaddr((void *)src, t1);
			w_src = vis_faligndata(w_src1, w_src2);

			MLIB_MIX;

			w_dst = vis_fpackfix_pair(dr2, dr3);
			dps++;
			w_src0 = w_src1;
			w_src1 = w_src2;
			w_src2 = vis_ld_d64_nf(dps);
			vis_alignaddr((void *)src, t2);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			vis_alignaddr((void *)src, t1);
			w_src = vis_faligndata(w_src1, w_src2);
			dps++;

			for (i = 0; i < times; i++) {
				*dpd = w_dst;
				MLIB_MIX;

				w_dst = vis_fpackfix_pair(dr2, dr3);
				w_src0 = w_src1;
				w_src1 = w_src2;
				w_src2 = vis_ld_d64_nf(dps);
				vis_alignaddr((void *)src, t2);
				w_lsrc = vis_faligndata(w_src0, w_src1);
				vis_alignaddr((void *)src, t1);
				w_src = vis_faligndata(w_src1, w_src2);
				dpd++;
				dps++;
			}
		} else {
			w_src = vis_faligndata(w_src0, w_src1);

			MLIB_MUL8;

			if ((mlib_addr)dst & 7) {
				times--;
				w_src0 = w_src1;
				w_src1 = vis_ld_d64_nf(dps);
				vis_alignaddr((void *)src, t2);
				w_lsrc = vis_faligndata(w_src0, w_src1);
				vis_alignaddr((void *)src, t1);
				w_src = vis_faligndata(w_src0, w_src1);
				dps++;

				MLIB_MIX;

				w_dst = vis_fpackfix_pair(dr2, dr3);
				vis_pst_16(w_dst, dpd, vis_edge16(dst, fdst));
				dpd++;
			}

			w_src0 = w_src1;

			w_src1 = vis_ld_d64_nf(dps);
			vis_alignaddr((void *)src, t2);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			vis_alignaddr((void *)src, t1);
			w_src = vis_faligndata(w_src0, w_src1);
			MLIB_MIX;
			w_dst = vis_fpackfix_pair(dr2, dr3);
			dps++;
			w_src0 = w_src1;
			w_src1 = vis_ld_d64_nf(dps);
			vis_alignaddr((void *)src, t2);
			w_lsrc = vis_faligndata(w_src0, w_src1);
			vis_alignaddr((void *)src, t1);
			w_src = vis_faligndata(w_src0, w_src1);
			dps++;

			for (i = 0; i < times; i++) {
				*dpd = w_dst;
				MLIB_MIX;
				w_dst = vis_fpackfix_pair(dr2, dr3);
				w_src0 = w_src1;

				w_src1 = vis_ld_d64_nf(dps);
				vis_alignaddr((void *)src, t2);
				w_lsrc = vis_faligndata(w_src0, w_src1);
				vis_alignaddr((void *)src, t1);
				w_src = vis_faligndata(w_src0, w_src1);
				dps++;
				dpd++;
			}
		}
	} else {
		w_src = w_src1;

		if ((mlib_addr)src & 7) {
			times--;

			if (((mlib_addr)src & 7) == 2) {
				w_src0 = vis_fand(w_maskand0, w_src0);
				w_src0 = vis_for(w_maskor0, w_src0);
			} else {
				w_src1 = vis_fand(w_maskand0, w_src1);
				w_src1 = vis_for(w_maskor0, w_src1);
			}

			w_src1 = vis_fand(w_maskand1, w_src1);
			w_src1 = vis_for(w_maskor1, w_src1);
			w_lsrc = vis_faligndata(w_src0, w_src1);

			MLIB_MUL8;

			w_src0 = w_src1;
			w_src1 = *dps;
			w_src = w_src1;
			w_lsrc = vis_faligndata(w_src0, w_src1);
			dps++;

			MLIB_MIX;

			w_dst = vis_fpackfix_pair(dr2, dr3);
			vis_pst_16(w_dst, dpd, vis_edge16(dst, fdst));
			dpd++;
		} else {
			w_src0 = vis_fand(w_maskand0, w_src0);
			w_src0 = vis_for(w_maskor0, w_src0);
			w_src0 = vis_fand(w_maskand1, w_src0);
			w_src0 = vis_for(w_maskor1, w_src0);
			w_lsrc = vis_faligndata(w_src0, w_src1);

			MLIB_MUL8;
		}

		w_src = vis_ld_d64_nf(dps);
		w_lsrc = vis_faligndata(w_src1, w_src);

		MLIB_MIX;

		w_src1 = w_src;
		w_dst = vis_fpackfix_pair(dr2, dr3);
		dps++;
		w_src = vis_ld_d64_nf(dps);
		w_lsrc = vis_faligndata(w_src1, w_src);
		dps++;

		for (i = 0; i < times; i++) {
			*dpd = w_dst;

			MLIB_MIX;

			w_src1 = w_src;
			w_src = vis_ld_d64_nf(dps);
			w_lsrc = vis_faligndata(w_src1, w_src);
			w_dst = vis_fpackfix_pair(dr2, dr3);
			dps++;
			dpd++;

		}
	}

	if (times >= 0) {
		vis_pst_16(w_dst, dpd, vis_edge16(dpd, fdst));
	}
	((mlib_s16 *)&fist->v16_last0)[0] = src[2 * n - 2];
	((mlib_s16 *)&fist->v16_last1)[1] = src[2 * n - 1];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasizeInit_S16S_S16S(
    void **filter,
    mlib_f32 alpha)
{
	return (__mlib_SignalEmphasizeInit_S16_S16(filter, alpha));
}

/* *********************************************************** */

void
__mlib_SignalEmphasizeFree_S16S_S16S(
    void *filter)
{
	__mlib_SignalEmphasizeFree_S16_S16(filter);
}

/* *********************************************************** */
