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

#pragma ident	"@(#)mlib_v_SignalMulWindow.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulWindow_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulWindow_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulWindow_S16(mlib_s16 *data,
 *                                           const mlib_s16 *window,
 *                                           mlib_s32 n);
 *      mlib_status mlib_SignalMulWindow_S16S(mlib_s16 *data,
 *                                            const mlib_s16 *window,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalMulWindow_S16S_S16S(mlib_s16 *dst,
 *                                                 const mlib_s16 *src,
 *                                                 const mlib_s16 *window,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_SignalMulWindow_S16_S16(mlib_s16 *dst,
 *                                               const mlib_s16 *src,
 *                                               const mlib_s16 *window,
 *                                               mlib_s32 n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      window   Window coefficient array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulWindow_S16 = __mlib_SignalMulWindow_S16
#pragma weak mlib_SignalMulWindow_S16S = __mlib_SignalMulWindow_S16S
#pragma weak mlib_SignalMulWindow_S16S_S16S = \
	__mlib_SignalMulWindow_S16S_S16S
#pragma weak mlib_SignalMulWindow_S16_S16 = __mlib_SignalMulWindow_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulWindow_S16) mlib_SignalMulWindow_S16
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16")));
__typeof__(__mlib_SignalMulWindow_S16S) mlib_SignalMulWindow_S16S
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16S")));
__typeof__(__mlib_SignalMulWindow_S16S_S16S) mlib_SignalMulWindow_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16S_S16S")));
__typeof__(__mlib_SignalMulWindow_S16_S16) mlib_SignalMulWindow_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc >> 16;                         \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_OFFSET8(ptr)	((8  - ((mlib_addr)ptr & 7)) & 7)
#define	MLIB_OFFSET16(ptr)	(MLIB_OFFSET8(ptr) >> 1)

#define	MAXN	1024

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *window,
    mlib_s32 n)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

	while (n && ((mlib_addr)dst & 7)) {
		mlib_d64 tmp = ((*src++)) * (2. * ((*window++)));

		FLOAT2INT_CLAMP((*dst++), tmp);
		n--;
	}

	{
		mlib_d64 w_const = vis_to_double_dup(0x10001);
		mlib_d64 dsrc[MAXN / 4];
		mlib_d64 *pdst = (mlib_d64 *)dst;
		mlib_d64 *psrc = (mlib_d64 *)src;
		mlib_d64 *pwin;
		mlib_d64 w_src, w_win, w_dst, w_res, w_last, w_now;
		mlib_s32 i;

		if ((mlib_addr)src & 7) {
			psrc =
			    (n <=
			    MAXN) ? dsrc : __mlib_malloc(sizeof (mlib_s16) * n);
			__mlib_VectorCopy_S16((mlib_s16 *)psrc, (mlib_s16 *)src,
			    n);
		}

		pwin = vis_alignaddr((void *)window, 0);

		if ((mlib_addr)window & 7) {
			w_last = vis_ld_d64_nf(pwin);
			pwin++;

#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {
				w_src = psrc[i];
				w_now = pwin[i];
				w_win = vis_faligndata(w_last, w_now);
				w_last = w_now;
				w_res = vis_fmul8ulx16(w_src, w_win);
				w_dst = vis_fmul8sux16(w_src, w_win);
				w_dst = vis_fpadd16(w_res, w_dst);
				w_dst = vis_fpadd16(w_dst, w_dst);
				w_dst = vis_fpsub16(w_dst, w_const);
				pdst[i] = w_dst;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {
				w_src = psrc[i];
				w_win = pwin[i];
				w_res = vis_fmul8ulx16(w_src, w_win);
				w_dst = vis_fmul8sux16(w_src, w_win);
				w_dst = vis_fpadd16(w_res, w_dst);
				w_dst = vis_fpadd16(w_dst, w_dst);
				w_dst = vis_fpsub16(w_dst, w_const);
				pdst[i] = w_dst;
			}

			if (n & 3)
				w_last = vis_ld_d64_nf(pwin + i);
		}

		if (n & 3) {
			w_src = vis_ld_d64_nf(psrc + i);
			w_now = vis_ld_d64_nf(pwin + i);
			w_win = vis_faligndata(w_last, w_now);
			w_res = vis_fmul8ulx16(w_src, w_win);
			w_dst = vis_fmul8sux16(w_src, w_win);
			w_dst = vis_fpadd16(w_res, w_dst);
			w_dst = vis_fpadd16(w_dst, w_dst);
			w_dst = vis_fpsub16(w_dst, w_const);
			vis_pst_16(w_dst, pdst + i, (0xF0 >> (n & 3)));
		}

		if (n > MAXN) {
			if ((mlib_addr)src & 7)
				__mlib_free(psrc);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *window,
    mlib_s32 n)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

	if (((mlib_addr)dst & 4)) {
		mlib_d64 tmp = ((*src++)) * (2. * (*window));

		FLOAT2INT_CLAMP((*dst++), tmp);
		tmp = ((*src++)) * (2. * ((*window++)));
		FLOAT2INT_CLAMP((*dst++), tmp);
		n--;
	}

	{
		mlib_d64 w_const = vis_to_double_dup(0x10001);
		mlib_d64 w_zero = vis_fzero();
		mlib_f32 v_mul0 = vis_to_float(0x80018001);
		mlib_d64 w_and0 = vis_to_double_dup(0x0000FFFF);
		mlib_d64 ddst[2 * MAXN / 4], dsrc[2 * MAXN / 4], dwin[MAXN / 4];
		mlib_d64 *pdst = (mlib_d64 *)dst;
		mlib_d64 *psrc = (mlib_d64 *)src;
		mlib_d64 *pwin = (mlib_d64 *)window;
		mlib_d64 w_src0, w_src1, w_dst0, w_dst1, w_res0, w_res1;
		mlib_d64 w_win, w_win0, w_win1;
		mlib_s32 i;

		if ((mlib_addr)dst & 7) {
			pdst =
			    (n <=
			    MAXN) ? ddst : __mlib_malloc(sizeof (mlib_s16) * 2 *
			    n);
		}

		if ((mlib_addr)src & 7) {
			psrc =
			    (n <=
			    MAXN) ? dsrc : __mlib_malloc(sizeof (mlib_s16) * 2 *
			    n);
			__mlib_VectorCopy_S16((mlib_s16 *)psrc, src, 2 * n);
		}

		if ((mlib_addr)window & 7) {
			pwin =
			    (n <=
			    MAXN) ? dwin : __mlib_malloc(sizeof (mlib_s16) * n);
			__mlib_VectorCopy_S16((mlib_s16 *)pwin, window, n);
		}

		vis_alignaddr(0, 2);

#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			w_src0 = psrc[2 * i];
			w_src1 = psrc[2 * i + 1];
			w_win = pwin[i];

			{
				mlib_d64 dr0, dr1;

				dr0 =
				    vis_fmuld8ulx16(v_mul0, vis_read_hi(w_win));
				dr0 = vis_fand(w_and0, dr0);
				w_win0 =
				    vis_for(vis_fpack32(vis_fpack32(dr0,
				    w_zero), w_zero), dr0);
				dr1 =
				    vis_fmuld8ulx16(v_mul0, vis_read_lo(w_win));
				dr1 = vis_fand(w_and0, dr1);
				w_win1 =
				    vis_for(vis_fpack32(vis_fpack32(dr1,
				    w_zero), w_zero), dr1);
			}

			w_res0 = vis_fmul8ulx16(w_src0, w_win0);
			w_dst0 = vis_fmul8sux16(w_src0, w_win0);
			w_dst0 = vis_fpadd16(w_res0, w_dst0);
			w_dst0 = vis_fpadd16(w_dst0, w_dst0);
			w_dst0 = vis_fpsub16(w_dst0, w_const);

			w_res1 = vis_fmul8ulx16(w_src1, w_win1);
			w_dst1 = vis_fmul8sux16(w_src1, w_win1);
			w_dst1 = vis_fpadd16(w_res1, w_dst1);
			w_dst1 = vis_fpadd16(w_dst1, w_dst1);
			w_dst1 = vis_fpsub16(w_dst1, w_const);

			pdst[2 * i] = w_dst0;
			pdst[2 * i + 1] = w_dst1;
		}

		if (n & 3) {

			w_src0 = vis_ld_d64_nf(psrc + 2 * i);
			w_src1 = vis_ld_d64_nf(psrc + 2 * i + 1);
			w_win = vis_ld_d64_nf(pwin + i);

			{
				mlib_d64 dr0, dr1;

				dr0 =
				    vis_fmuld8ulx16(v_mul0, vis_read_hi(w_win));
				dr0 = vis_fand(w_and0, dr0);
				w_win0 =
				    vis_for(vis_fpack32(vis_fpack32(dr0,
				    w_zero), w_zero), dr0);
				dr1 =
				    vis_fmuld8ulx16(v_mul0, vis_read_lo(w_win));
				dr1 = vis_fand(w_and0, dr1);
				w_win1 =
				    vis_for(vis_fpack32(vis_fpack32(dr1,
				    w_zero), w_zero), dr1);
			}

			w_res0 = vis_fmul8ulx16(w_src0, w_win0);
			w_dst0 = vis_fmul8sux16(w_src0, w_win0);
			w_dst0 = vis_fpadd16(w_res0, w_dst0);
			w_dst0 = vis_fpadd16(w_dst0, w_dst0);
			w_dst0 = vis_fpsub16(w_dst0, w_const);

			w_res1 = vis_fmul8ulx16(w_src1, w_win1);
			w_dst1 = vis_fmul8sux16(w_src1, w_win1);
			w_dst1 = vis_fpadd16(w_res1, w_dst1);
			w_dst1 = vis_fpadd16(w_dst1, w_dst1);
			w_dst1 = vis_fpsub16(w_dst1, w_const);

			switch (n & 3) {
			case 3:
				((mlib_f32 *)pdst)[4 * i + 2] =
				    vis_read_hi(w_dst1);
			case 2:
				((mlib_f32 *)pdst)[4 * i + 1] =
				    vis_read_lo(w_dst0);
			case 1:
				((mlib_f32 *)pdst)[4 * i + 0] =
				    vis_read_hi(w_dst0);
			}
		}

		if ((mlib_addr)dst & 7)
			__mlib_VectorCopy_S16(dst, (mlib_s16 *)pdst, 2 * n);

		if (n > MAXN) {
			if ((mlib_addr)window & 7)
				__mlib_free(pwin);

			if ((mlib_addr)src & 7)
				__mlib_free(psrc);

			if ((mlib_addr)dst & 7)
				__mlib_free(pdst);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16(
    mlib_s16 *data,
    const mlib_s16 *window,
    mlib_s32 n)
{
	return (__mlib_SignalMulWindow_S16_S16(data, data, window, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16S(
    mlib_s16 *data,
    const mlib_s16 *window,
    mlib_s32 n)
{
	return (__mlib_SignalMulWindow_S16S_S16S(data, data, window, n));
}

/* *********************************************************** */
