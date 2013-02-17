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

#pragma ident	"@(#)mlib_SignalLPC2Cepstral_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLPC2Cepstral_F32      - convert linear prediction
 *                                         coefficients to cepstral
 *                                         coefficients
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPC2Cepstral_F32(mlib_f32       *cepst,
 *                                              const mlib_f32 *lpc,
 *                                              mlib_f32       gain,
 *                                              mlib_s32       length,
 *                                              mlib_s32       order);
 * ARGUMENTS
 *      cepst   the cepstral coefficients
 *      lpc     the linear prediction coefficients
 *      gain    the gain of the LPC model.
 *      length  the length of the cepstral coefficients.
 *      order   the order of the linear prediction filter.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if function completed successfully.
 *      MLIB_FAILURE is returned if error is encountered.
 *
 * DESCRIPTION
 *      The cepstral coefficients are the coefficients of the Fourier
 *      transform representation of the log magnitude spectrum.
 *
 *      The LPC cepstral coefficients can be derived recursively from
 *      the LPC coefficients as following.
 *
 *              c(0) = log(G)
 *
 *                            m-1  k
 *              c(m) = a(m) + SUM --- * c(k) * a(m-k), 1<=m<=M
 *                            k = 1  m
 *
 *                     m-1  k
 *              c(m) = SUM --- * c(k) * a(m-k), m>M
 *                     k = 1  m
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 *
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while lpc[0] element is not initialized and
 *      not used.
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalCepstral.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPC2Cepstral_F32 = __mlib_SignalLPC2Cepstral_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPC2Cepstral_F32) mlib_SignalLPC2Cepstral_F32
    __attribute__((weak, alias("__mlib_SignalLPC2Cepstral_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalLPC2Cepstral_F32(
    mlib_f32 *cepst,
    const mlib_f32 *lpc,
    mlib_f32 gain,
    mlib_s32 length,
    mlib_s32 order)
{
	mlib_f32 fdiv = 1.f;
	mlib_f32 fmul = 1.f / fdiv;
	mlib_d64 dlog, dgain = gain;
	mlib_s32 i, j, k;
	mlib_s32 lengtho = (length < order) ? length : order;
	mlib_s32 length8 = (length < 8) ? length : 8;

	length8 = (length8 < lengtho) ? length8 : lengtho;

	mlib_vlog(&dlog, &dgain, 1);
	cepst[0] = dlog;

	cepst++;
	lpc++;

	for (i = 0; i < length8; i++) {
		mlib_f32 sum = lpc[i];
		mlib_f32 sum2 = 0.f;
		mlib_s32 nn = i;
		mlib_f32 fstep = fmul;
		mlib_f32 fmul2 = fmul + fstep;

		fdiv += 1.f;
		fstep += fstep;

		j = 0;

		if (nn >= 4) {
			sum += (fmul * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul += fstep;
			sum2 += (fmul2 * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul2 += fstep;
			sum += (fmul * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul += fstep;
			sum2 += (fmul2 * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul2 += fstep;
		}

		if ((nn & 3) >= 2) {
			sum += (fmul * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul += fstep;
			sum2 += (fmul2 * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul2 += fstep;
		}

		if (nn & 1) {
			sum += (fmul * cepst[j]) * lpc[nn - 1 - j];
			j++;
			fmul += fstep;
		}

		fmul = 1.f / fdiv;
		cepst[i] = sum + sum2;
	}

	for (i = 8; i < lengtho; i++) {
		mlib_f32 sum = lpc[i];
		mlib_f32 sum2 = 0.f;
		mlib_s32 nn = i;
		mlib_f32 fstep = fmul;
		mlib_f32 fmul2 = fmul + fstep;

		fdiv += 1.f;
		fstep += fstep;

		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < nn - 1; j += 2) {
				mlib_f32 fc0 = cepst[j];
				mlib_f32 lpc0 = lpc[nn - 1 - j];
				mlib_f32 fc1 = cepst[j + 1];
				mlib_f32 lpc1 = lpc[nn - j - 2];

				sum += (fmul * fc0) * lpc0;
				fmul += fstep;
				sum2 += (fmul2 * fc1) * lpc1;
				fmul2 += fstep;
			}

			if (nn & 1) {
				mlib_f32 fc0 = cepst[j];
				mlib_f32 lpc0 = lpc[nn - 1 - j];

				sum += fmul * fc0 * lpc0;
			}
		}

		fmul = 1.f / fdiv;
		cepst[i] = sum + sum2;
	}

	for (i = lengtho, k = lengtho - order; i < length; i++, k++) {
		mlib_f32 sum = 0.f;
		mlib_f32 sum2 = 0.f;
		mlib_s32 nn = order;
		mlib_f32 fstep = fmul;
		mlib_f32 fmul2;

		fdiv += 1.f;
		fmul += k * fstep;
		fmul2 = fmul + fstep;
		fstep += fstep;

		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < nn - 1; j += 2) {
				mlib_f32 fc0 = cepst[k + j];
				mlib_f32 lpc0 = lpc[nn - 1 - j];
				mlib_f32 fc1 = cepst[k + j + 1];
				mlib_f32 lpc1 = lpc[nn - j - 2];

				sum += (fmul * fc0) * lpc0;
				fmul += fstep;
				sum2 += (fmul2 * fc1) * lpc1;
				fmul2 += fstep;
			}

			if (nn & 1) {
				mlib_f32 fc0 = cepst[k + j];
				mlib_f32 lpc0 = lpc[nn - 1 - j];

				sum += fmul * fc0 * lpc0;
			}
		}

		fmul = 1.f / fdiv;
		cepst[i] = sum + sum2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
