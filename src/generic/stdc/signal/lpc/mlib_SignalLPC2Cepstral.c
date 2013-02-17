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

#pragma ident	"@(#)mlib_SignalLPC2Cepstral.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLPC2Cepstral_S16      - convert linear prediction
 *                                        coefficients to cepstral
 *                                        coefficients
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPC2Cepstral_S16(mlib_s16 *cepst,
 *                                              mlib_s32 cscale,
 *                                              const mlib_s16 *lpc,
 *                                              mlib_s32 lscale,
 *                                              mlib_s16 gain,
 *                                              mlib_s32 gscale,
 *                                              mlib_s32 length,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalLPC2Cepstral_S16_Adp(mlib_s16 *cepst,
 *                                                  mlib_s32 *cscale,
 *                                                  const mlib_s16 *lpc,
 *                                                  mlib_s32 lscale,
 *                                                  mlib_s16 gain,
 *                                                  mlib_s32 gscale,
 *                                                  mlib_s32 length,
 *                                                  mlib_s32 order);
 *
 * ARGUMENTS
 *      cepst   the cepstral coefficients.
 *      cscale  the scaling factor of the cepstral coefficients,
 *              where actual_data = output_data * 2^(-scaling_factor).
 *      lpc     the linear prediction coefficients.
 *      lscale  the scaling factor of the linear prediction coefficients,
 *              where actual_data = input_data * 2^(-scaling_factor).
 *      gain    the gain of the LPC model.
 *      gscale  the scaling factor of the gain of the LPC model,
 *              where actual_data = input_data * 2^(-scaling_factor).
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
 *      Note for functions with adaptive scaling (with _Adp postfix),
 *      the scaling factor of the output data will be calculated based
 *      on the actual data; for functions with non-adaptive scaling
 *      (without _Adp postfix), the user supplied scaling factor will
 *      be used and the output will be saturated if necessary.
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

#pragma weak mlib_SignalLPC2Cepstral_S16 = __mlib_SignalLPC2Cepstral_S16
#pragma weak mlib_SignalLPC2Cepstral_S16_Adp = \
	__mlib_SignalLPC2Cepstral_S16_Adp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPC2Cepstral_S16) mlib_SignalLPC2Cepstral_S16
    __attribute__((weak, alias("__mlib_SignalLPC2Cepstral_S16")));
__typeof__(__mlib_SignalLPC2Cepstral_S16_Adp) mlib_SignalLPC2Cepstral_S16_Adp
    __attribute__((weak, alias("__mlib_SignalLPC2Cepstral_S16_Adp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUF_LENGTH	256

/* *********************************************************** */

mlib_status
__mlib_SignalLPC2Cepstral_S16(
    mlib_s16 *cepst,
    mlib_s32 cscale,
    const mlib_s16 *lpc,
    mlib_s32 lscale,
    mlib_s16 gain,
    mlib_s32 gscale,
    mlib_s32 length,
    mlib_s32 order)
{
	mlib_f32 fdiv = 1.f;
	mlib_f32 fscale = 1.f / (1 << 30);
	mlib_f32 fgain = gain * fscale * (1 << (30 - gscale));
	mlib_f32 flscale = fscale * (1 << (30 - lscale));
	mlib_f32 fcscale = 1 << cscale;
	mlib_f32 fmul = flscale / fdiv;
	mlib_d64 dlog, dgain = fgain;
	mlib_f32 fcepstbuf[BUF_LENGTH], flpcbuf[BUF_LENGTH];
	mlib_f32 *fcepst = fcepstbuf, *flpc = flpcbuf;
	mlib_s32 i, j, k, olength = (length > order) ? order : length;

	if (length > BUF_LENGTH) {
		fcepst = __mlib_malloc(2 * sizeof (mlib_f32) * length);
		flpc = fcepst + length;
	}

	mlib_vlog(&dlog, &dgain, 1);

	cepst[0] = fcscale * (mlib_f32)dlog;

	cepst++;
	lpc++;

	for (i = 0; i < olength; i++) {
		mlib_f32 sum = flscale * lpc[i];
		mlib_f32 sum2 = 0.f;
		mlib_s32 nn = i;
		mlib_f32 fstep = fmul;
		mlib_f32 fmul2 = fmul + fstep;

		fdiv += 1.f;
		fstep += fstep;
		flpc[i] = lpc[i];

		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < nn - 1; j += 2) {
				mlib_f32 fc0 = fcepst[j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];
				mlib_f32 fc1 = fcepst[j + 1];
				mlib_f32 lpc1 = flpc[nn - j - 2];

				sum += (fmul * fc0) * lpc0;
				fmul += fstep;
				sum2 += (fmul2 * fc1) * lpc1;
				fmul2 += fstep;
			}

			if (nn & 1) {
				mlib_f32 fc0 = fcepst[j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];

				sum += fmul * fc0 * lpc0;
			}
		}

		fmul = flscale / fdiv;
		fcepst[i] = sum + sum2;
	}

	k = 0;

	for (; i < length; i++, k++) {
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
				mlib_f32 fc0 = fcepst[k + j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];
				mlib_f32 fc1 = fcepst[k + j + 1];
				mlib_f32 lpc1 = flpc[nn - j - 2];

				sum += (fmul * fc0) * lpc0;
				fmul += fstep;
				sum2 += (fmul2 * fc1) * lpc1;
				fmul2 += fstep;
			}

			if (nn & 1) {
				mlib_f32 fc0 = fcepst[k + j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];

				sum += fmul * fc0 * lpc0;
			}
		}

		fmul = flscale / fdiv;
		fcepst[i] = sum + sum2;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length; i++)
		cepst[i] = fcepst[i] * fcscale;

	if (fcepst != fcepstbuf)
		__mlib_free(fcepst);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPC2Cepstral_S16_Adp(
    mlib_s16 *cepst,
    mlib_s32 *cscale,
    const mlib_s16 *lpc,
    mlib_s32 lscale,
    mlib_s16 gain,
    mlib_s32 gscale,
    mlib_s32 length,
    mlib_s32 order)
{
	mlib_f32 fdiv = 1.f;
	mlib_f32 fscale = 1.f / (1 << 30);
	mlib_f32 fgain = gain * fscale * (1 << (30 - gscale));
	mlib_f32 flscale = fscale * (1 << (30 - lscale));

/* 1 << cscale; */
	mlib_f32 fcscale = 0;
	mlib_f32 fmul = flscale / fdiv;
	mlib_f32 fcepstbuf[BUF_LENGTH], flpcbuf[BUF_LENGTH];
	mlib_f32 *fcepst = fcepstbuf, *flpc = flpcbuf;
	mlib_s32 i, j, k, olength = (length > order) ? order : length;
	mlib_d64 dlog, dgain = fgain;
	mlib_f32 flog, fmincepst, fmaxcepst;

	if (length > BUF_LENGTH) {
		fcepst = __mlib_malloc(2 * sizeof (mlib_f32) * length);
		flpc = fcepst + length;
	}

	mlib_vlog(&dlog, &dgain, 1);
	fmaxcepst = fmincepst = flog = dlog;
	lpc++;

	for (i = 0; i < olength; i++) {
		mlib_f32 sum = flscale * lpc[i];
		mlib_f32 sum2 = 0.f;
		mlib_s32 nn = i;
		mlib_f32 fstep = fmul;
		mlib_f32 fmul2 = fmul + fstep;

		fdiv += 1.f;
		fstep += fstep;
		flpc[i] = lpc[i];

		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < nn - 1; j += 2) {
				mlib_f32 fc0 = fcepst[j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];
				mlib_f32 fc1 = fcepst[j + 1];
				mlib_f32 lpc1 = flpc[nn - j - 2];

				sum += (fmul * fc0) * lpc0;
				fmul += fstep;
				sum2 += (fmul2 * fc1) * lpc1;
				fmul2 += fstep;
			}

			if (nn & 1) {
				mlib_f32 fc0 = fcepst[j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];

				sum += fmul * fc0 * lpc0;
			}
		}

		fmul = flscale / fdiv;
		fcepst[i] = sum + sum2;
		fmaxcepst = (fcepst[i] > fmaxcepst) ? fcepst[i] : fmaxcepst;
		fmincepst = (fcepst[i] < fmincepst) ? fcepst[i] : fmincepst;
	}

	k = 0;

	for (; i < length; i++, k++) {
		mlib_f32 sum = 0.f;
		mlib_f32 sum2 = 0.f;
		mlib_s32 nn = order;
		mlib_f32 fstep = fmul;
		mlib_f32 fmul2 = fmul + fstep;

		fdiv += 1.f;
		fmul += k * fstep;
		fmul2 = fmul + fstep;
		fstep += fstep;

		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < nn - 1; j += 2) {
				mlib_f32 fc0 = fcepst[k + j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];
				mlib_f32 fc1 = fcepst[k + j + 1];
				mlib_f32 lpc1 = flpc[nn - j - 2];

				sum += (fmul * fc0) * lpc0;
				fmul += fstep;
				sum2 += (fmul2 * fc1) * lpc1;
				fmul2 += fstep;
			}

			if (nn & 1) {
				mlib_f32 fc0 = fcepst[k + j];
				mlib_f32 lpc0 = flpc[nn - 1 - j];

				sum += fmul * fc0 * lpc0;
			}
		}

		fmul = flscale / fdiv;
		fcepst[i] = sum + sum2;
		fmaxcepst = (fcepst[i] > fmaxcepst) ? fcepst[i] : fmaxcepst;
		fmincepst = (fcepst[i] < fmincepst) ? fcepst[i] : fmincepst;
	}

	for (i = 0; i < 31; i++, fmaxcepst *= 2.f, fmincepst *= 2.f) {
		if (fmaxcepst > 16383.5f || fmincepst < -16384.f)
			break;
	}

	cscale[0] = i;

	fcscale = 1 << i;

	cepst[0] = flog * fcscale;
	cepst++;

	for (i = 0; i < length; i++) {
		cepst[i] = fcepst[i] * fcscale;
	}

	if (fcepst != fcepstbuf)
		__mlib_free(fcepst);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
