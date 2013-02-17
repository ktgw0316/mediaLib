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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_SignalSineWave.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalSineWaveInit_S16 - allocates memory for
 *          internal state structure and converts the parameters
 *          of the wave into internal representation
 *
 *      mlib_SignalSineWave_S16 - generate one packet of sine
 *          wave and updates the internal states
 *
 *      mlib_SignalSineWaveFree_S16 - releases the memory
 *          allocated for the internal states structure
 *
 * SYNOPSIS
 *
 *      void mlib_SignalSineWaveFree_S16(void *state);
 *      mlib_status mlib_SignalSineWaveInit_S16(void     **state,
 *                                              mlib_s16 mag,
 *                                              mlib_f32 freq,
 *                                              mlib_f32 phase);
 *      mlib_status mlib_SignalSineWave_S16(mlib_s16 *sine,
 *                                          void     *state,
 *                                          mlib_s32 n);
 * ARGUMENTS
 *      state    Internal state structure
 *      mag      Magnitude of sine wave to be generated. The magnitude
 *               is in Q15 format
 *      freq     Angular frequency of sine wave to be generated. The
 *               angular frequency is measured in radians per sample
 *      phase    Start phase of sine wave to be generated. The phase
 *               is measured in radians
 *      sine     Generated sine wave array
 *      n        Length of generated sine wave array
 *
 * DESCRIPTION
 *     Tone Generation:     sine[i] = A*sin(w*i+O)
 *
 */
#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalSiggen.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalSineWaveFree_S16 = __mlib_SignalSineWaveFree_S16
#pragma weak mlib_SignalSineWaveInit_S16 = __mlib_SignalSineWaveInit_S16
#pragma weak mlib_SignalSineWave_S16 = __mlib_SignalSineWave_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalSineWaveFree_S16) mlib_SignalSineWaveFree_S16
    __attribute__((weak, alias("__mlib_SignalSineWaveFree_S16")));
__typeof__(__mlib_SignalSineWaveInit_S16) mlib_SignalSineWaveInit_S16
    __attribute__((weak, alias("__mlib_SignalSineWaveInit_S16")));
__typeof__(__mlib_SignalSineWave_S16) mlib_SignalSineWave_S16
    __attribute__((weak, alias("__mlib_SignalSineWave_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_s32 i1, i0;
#else		   /* _LITTLE_ENDIAN */
		mlib_s32 i0, i1;
#endif		   /* _LITTLE_ENDIAN */
	} s32x2;
} mlib_union64;

/* *********************************************************** */

mlib_status
__mlib_SignalSineWave_S16(
    mlib_s16 *sine,
    void *state,
    mlib_s32 n)
{
	mlib_sinewave_s16_struct *sist = state;

/* check for obvious errors */

	if ((sine == NULL) || (n <= 0) || (state == NULL) ||
	    (sist->type != MLIB_SINE_S16))
		return (MLIB_FAILURE);

	{
		mlib_union64 tmp0, tmp1;
		__m128d sb1, sb2, sb3, sb4, scstep;
		mlib_d64 tmp2[2];
		mlib_d64 sbase1, sbase2, sbase3, sbase4;
		mlib_d64 sbase5, sbase6, sbase7, sbase8;
		mlib_d64 cstep = sist->cstep;
		mlib_s32 i;

		if (n > 3) {
			sb1 = _mm_set_pd(sist->sbase2, sist->sbase1);
			sb2 = _mm_set_pd(sist->sbase4, sist->sbase3);
			sb3 = _mm_set_pd(sist->sbase6, sist->sbase5);
			sb4 = _mm_set_pd(sist->sbase8, sist->sbase7);
			scstep = _mm_set1_pd(sist->cstep);
		} else {
			sbase1 = sist->sbase1, sbase2 = sist->sbase2;
			sbase3 = sist->sbase3, sbase4 = sist->sbase4;
			sbase5 = sist->sbase5, sbase6 = sist->sbase6;
			sbase7 = sist->sbase7, sbase8 = sist->sbase8;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 2); i++) {
			__m128d snext0, snext1;
			__m128i stmp[2], sd0, sd1;
			snext0 = _mm_sub_pd(_mm_mul_pd(scstep, sb3), sb1);
			snext1 = _mm_sub_pd(_mm_mul_pd(scstep, sb4), sb2);

			stmp[0] = _mm_cvtpd_epi32(sb1);
			stmp[1] = _mm_cvtpd_epi32(sb2);

			sd0 = _mm_packs_epi32(stmp[0], stmp[1]);
			sd1 = _mm_unpacklo_epi32(sd0, _mm_srli_si128(sd0, 8));
			_mm_storel_epi64((void *)(sine + 4 * i), sd1);

			sb1 = sb3;
			sb2 = sb4;
			sb3 = snext0;
			sb4 = snext1;
		}

		mlib_d64 base1[2], base2[2], base3[2], base4[2];
		if (n > 3) {
			_mm_storeu_pd(base1, sb1);
			_mm_storeu_pd(base2, sb2);
			_mm_storeu_pd(base3, sb3);
			_mm_storeu_pd(base4, sb4);
			sbase1 = base1[0];
			sbase2 = base1[1];
			sbase3 = base2[0];
			sbase4 = base2[1];
			sbase5 = base3[0];
			sbase6 = base3[1];
			sbase7 = base4[0];
			sbase8 = base4[1];
		}

		i <<= 2;

		while (i < n) {
			mlib_d64 snext = cstep * sbase5 - sbase1;

			sine[i++] = sbase1;
			sbase1 = sbase2;
			sbase2 = sbase3;
			sbase3 = sbase4;
			sbase4 = sbase5;
			sbase5 = sbase6;
			sbase6 = sbase7;
			sbase7 = sbase8;
			sbase8 = snext;
		}

		sist->sbase1 = sbase1;
		sist->sbase2 = sbase2;
		sist->sbase3 = sbase3;
		sist->sbase4 = sbase4;
		sist->sbase5 = sbase5;
		sist->sbase6 = sbase6;
		sist->sbase7 = sbase7;
		sist->sbase8 = sbase8;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalSineWaveInit_S16(
    void **state,
    mlib_s16 mag,
    mlib_f32 freq,
    mlib_f32 phase)
{
	mlib_sinewave_s16_struct *sist;
	mlib_d64 dfreq = freq, dphase = phase, dmag = mag;

/* check for obvious errors */

	if (state == NULL)
		return (MLIB_FAILURE);

	sist = __mlib_malloc(sizeof (mlib_sinewave_s16_struct));

	if (sist == NULL)
		return (MLIB_NULLPOINTER);

	if (mag == -32768)
		dmag = -32767.99;

	sist->type = MLIB_SINE_S16;
	sist->cstep = 2 * mlib_cos(dfreq);
	sist->sbase1 = dmag * mlib_sin(dphase);
	sist->sbase2 = dmag * mlib_sin(dphase + dfreq);
	sist->sbase3 = sist->cstep * sist->sbase2 - sist->sbase1;
	sist->sbase4 = sist->cstep * sist->sbase3 - sist->sbase2;
	sist->sbase5 = sist->cstep * sist->sbase4 - sist->sbase3;
	sist->sbase6 = sist->cstep * sist->sbase5 - sist->sbase4;
	sist->sbase7 = sist->cstep * sist->sbase6 - sist->sbase5;
	sist->sbase8 = sist->cstep * sist->sbase7 - sist->sbase6;
	sist->cstep = 2 * mlib_cos(4 * dfreq);

	*state = sist;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalSineWaveFree_S16(
    void *state)
{
	mlib_sinewave_s16_struct *sist = state;

/* check for obvious errors */

	if ((state == NULL) || (sist->type != MLIB_SINE_S16))
		return; /* MLIB_FAILURE */

	sist->type = 0;
	__mlib_free(state);
}

/* *********************************************************** */
