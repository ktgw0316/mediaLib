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

#pragma ident	"@(#)mlib_c_SignalConv_f.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConv_[S16|S16S]_[S16|S16S]_Sat - convolution (mlib_s16 type)
 *      mlib_SignalConv_[F32|F32S]_[F32|F32S] - convolution (mlib_f32 type)
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConv_F32S_F32S(mlib_f32       *dst,
 *                                            const mlib_f32 *src1,
 *                                            const mlib_f32 *src2,
 *                                            mlib_s32       m,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalConv_F32_F32(mlib_f32       *dst,
 *                                          const mlib_f32 *src1,
 *                                          const mlib_f32 *src2,
 *                                          mlib_s32       m,
 *                                          mlib_s32       n);
 *      mlib_status mlib_SignalConv_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                const mlib_s16 *src1,
 *                                                const mlib_s16 *src2,
 *                                                mlib_s32       m,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalConv_S16_S16_Sat(mlib_s16       *dst,
 *                                              const mlib_s16 *src1,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       m,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      dst   Output signal array
 *      src1  Input signal array
 *      src2  Input signal array
 *      m     Number of samples in the first input signal array
 *      n     Number of samples in the second input signal array
 *
 *      dst_main - destination signal array
 *
 *      src_main - source signal array
 *
 *      order    - Order of the transformation. The base-2 logarithm of the
 *                 number of data samples
 *
 * DESCRIPTION
 *           min(n, m)-1
 *      z[i] = SUM (y[k]*x[i-k]) ,  i = 0...(n+m-2)
 *             k = 0
 *      dst_main = mlib_conv_fft( src_main, order )
 *
 * NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *
 * IMPLEMENTATION
 *
 *      mlib_conv_fft uses an external function
 *                    from signal/fourier section of mlib,
 *      mlib_fftDispatcher_D64C_D64C with dst_aux = src_aux = NULL,
 *                                   shiftscale = order
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalFFTUtil.h>
#include <mlib_Utils.h>
#include <mlib_SignalConv.h>

/* *********************************************************** */

#define	MLIB_FRAME	1024

/* *********************************************************** */

#define	FFT_SCALE	2.0

/* *********************************************************** */

mlib_status
mlib_fftConv_S16_S16_Sat(
    mlib_s16 *dst,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 z;
	mlib_s32 i, s;
	mlib_s32 k, order;
	mlib_d64 a_src1[MLIB_FRAME * 4];
	mlib_d64 *psrc1 = a_src1, *pdst1;

/* fft way */
	switch (m) {
	case 0:
		return (MLIB_SUCCESS);

	case 1:
		z = (mlib_d64)src1[0] * 2.;

		for (i = 0; i < n; i++) {
			dst[i] = FLOAT2INT_CLAMP1(z * src2[i]) >> 16;
		}

		return (MLIB_SUCCESS);

	case 2: {
		    mlib_d64 mult0 = src1[0] * 2., mult1 = src1[1] * 2.;

		    dst[0] = FLOAT2INT_CLAMP1(mult0 * src2[0]) >> 16;

		    for (i = 1; i < n; i++) {
			    dst[i] =
				FLOAT2INT_CLAMP1(mult0 * src2[i] +
				mult1 * src2[i - 1]) >> 16;
		    }

		    dst[n] = FLOAT2INT_CLAMP1(mult1 * src2[n - 1]) >> 16;
	    }

		return (MLIB_SUCCESS);
	}

	s = n + m - 1;

	for (order = 0, k = 1; k < s; ) {
		order++;
		k <<= 1;
	}

	if (k > MLIB_FRAME / 2) {
		if (!(psrc1 = __mlib_malloc(2 * 4 * k * sizeof (mlib_d64))))
			return (MLIB_FAILURE);
	}

	pdst1 = psrc1 + 4 * k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m; i++) {
		psrc1[2 * i] = src1[i];
		psrc1[2 * i + 1] = src2[i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	for (; i < n; i++) {
		psrc1[2 * i] = 0;
		psrc1[2 * i + 1] = src2[i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0;
	}

	mlib_conv_fft(pdst1, psrc1, order, FFT_SCALE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < s; i++)
		dst[i] = FLOAT2INT_CLAMP1(pdst1[i]) >> 16;

	if (k > MLIB_FRAME / 2)
		__mlib_free(psrc1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_fftConv_S16S_S16S_Sat(
    mlib_s16 *dst,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 z, z1;
	mlib_s32 i, s;
	mlib_s32 k, order;
	mlib_d64 a_src1[MLIB_FRAME * 4];
	mlib_d64 *psrc1 = a_src1, *pdst1;

/* fft way */

	switch (m) {
	case 0:
		return (MLIB_SUCCESS);
	case 1:
		z = (mlib_d64)src1[0] * 2.;
		z1 = (mlib_d64)src1[1] * 2.;
		for (i = 0; i < n; i++) {
			dst[2 * i] = FLOAT2INT_CLAMP1(z * src2[2 * i]) >> 16;
			dst[2 * i + 1] =
			    FLOAT2INT_CLAMP1(z1 * src2[2 * i + 1]) >> 16;
		}

		return (MLIB_SUCCESS);
	case 2: {
		    mlib_d64 mult0 = src1[0] * 2., mult1 = src1[2] * 2.;
		    mlib_d64 mult01 = src1[1] * 2., mult11 = src1[3] * 2.;

		    dst[0] = FLOAT2INT_CLAMP1(mult0 * src2[0]) >> 16;
		    dst[1] = FLOAT2INT_CLAMP1(mult01 * src2[1]) >> 16;
		    for (i = 1; i < n; i++) {
			    dst[2 * i] =
				FLOAT2INT_CLAMP1(mult0 * src2[2 * i] +
				mult1 * src2[2 * (i - 1)]) >> 16;
			    dst[2 * i + 1] =
				FLOAT2INT_CLAMP1(mult01 * src2[2 * i + 1] +
				mult11 * src2[2 * (i - 1) + 1]) >> 16;
		    }

		    dst[2 * n] =
			FLOAT2INT_CLAMP1(mult1 * src2[2 * (n - 1)]) >> 16;
		    dst[2 * n + 1] =
			FLOAT2INT_CLAMP1(mult11 * src2[2 * (n - 1) + 1]) >> 16;
	    }

		return (MLIB_SUCCESS);
	}

	s = n + m - 1;
	for (order = 0, k = 1; k < s; ) {
		order++;
		k <<= 1;
	}

	if (k > MLIB_FRAME / 2) {
		if (!(psrc1 = __mlib_malloc(2 * 4 * k * sizeof (mlib_d64))))
			return (MLIB_FAILURE);
	}

	pdst1 = psrc1 + 4 * k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m; i++) {
		psrc1[2 * i] = src1[2 * i];
		psrc1[2 * i + 1] = src2[2 * i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		psrc1[2 * i] = 0;
		psrc1[2 * i + 1] = src2[2 * i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0;
	}

	mlib_conv_fft(pdst1, psrc1, order, FFT_SCALE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < s; i++)
		dst[2 * i] = FLOAT2INT_CLAMP1(pdst1[i]) >> 16;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	for (i = 0; i < m; i++) {
		psrc1[2 * i] = src1[2 * i + 1];
		psrc1[2 * i + 1] = src2[2 * i + 1];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		psrc1[2 * i] = 0;
		psrc1[2 * i + 1] = src2[2 * i + 1];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0;
	}

	mlib_conv_fft(pdst1, psrc1, order, FFT_SCALE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < s; i++)
		dst[2 * i + 1] = FLOAT2INT_CLAMP1(pdst1[i]) >> 16;

	if (k > MLIB_FRAME / 2)
		__mlib_free(psrc1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
