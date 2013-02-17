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

#pragma ident	"@(#)mlib_s_SignalGaussNoise_Fp.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalGaussNoiseInit_F32 - allocate the memory for
 *           internal state structure and convert the
 *           parameter into internal representation
 *
 *      mlib_SignalGaussNoise_F32 - generate one
 *           packet of Gaussian noise and update the internal states
 *
 *      mlib_SignalGaussNoiseFree_F32 - release the memory allocated
 *           for the internal state structure
 *
 * SYNOPSIS
 *      void mlib_SignalGaussNoiseFree_F32(void *state);
 *      mlib_status mlib_SignalGaussNoiseInit_F32(void     **state,
 *                                                mlib_f32 mag,
 *                                                mlib_f32 mean,
 *                                                mlib_f32 stddev,
 *                                                mlib_f32 seed);
 *      mlib_status mlib_SignalGaussNoise_F32(mlib_f32 *gnoise,
 *                                            void     *state,
 *                                            mlib_s32 n);
 *
 * ARGUMENTS
 *      state         Internal state structure
 *      mag           Magnitude of the Gaussian noise to be generated
 *      stddev        Standard deviation of the Gaussian noise
 *      mean          Mean of the Gaussian noise
 *      gnoise        Generated Gaussian noise array
 *      seed          Seed value for pseudo random number generator
 *      n             Length of the array in number of samples
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalSiggen.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalGaussNoiseFree_F32 = __mlib_SignalGaussNoiseFree_F32
#pragma weak mlib_SignalGaussNoiseInit_F32 = __mlib_SignalGaussNoiseInit_F32
#pragma weak mlib_SignalGaussNoise_F32 = __mlib_SignalGaussNoise_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalGaussNoiseFree_F32) mlib_SignalGaussNoiseFree_F32
    __attribute__((weak, alias("__mlib_SignalGaussNoiseFree_F32")));
__typeof__(__mlib_SignalGaussNoiseInit_F32) mlib_SignalGaussNoiseInit_F32
    __attribute__((weak, alias("__mlib_SignalGaussNoiseInit_F32")));
__typeof__(__mlib_SignalGaussNoise_F32) mlib_SignalGaussNoise_F32
    __attribute__((weak, alias("__mlib_SignalGaussNoise_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
#ifdef _LITTLE_ENDIAN
	struct
	{
		mlib_f32 i1, i0;
	} f32x2;
#else		   /* _LITTLE_ENDIAN */
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
#endif		   /* _LITTLE_ENDIAN */
} mlib_union64;

/* *********************************************************** */

#define	MAX_SIZE	2048
#define	NUM	16

/* *********************************************************** */

mlib_status
__mlib_SignalGaussNoiseInit_F32(
    void **state,
    mlib_f32 mag,
    mlib_f32 mean,
    mlib_f32 stddev,
    mlib_f32 seed)
{
	void *ptr;
	struct mlib_GaussNoise_F32 *gauss_noise;
	mlib_s32 rand_base, rand_base1;

	if (mlib_fabs(mean) > mlib_fabs(mag))
		return (MLIB_FAILURE);

	ptr = __mlib_malloc(sizeof (struct mlib_GaussNoise_F32));

	if (!ptr)
		return (MLIB_NULLPOINTER);

	*state = ptr;
	gauss_noise = (struct mlib_GaussNoise_F32 *)ptr;
	gauss_noise->mag = mlib_fabs(mag);
	gauss_noise->mean = mean;
	gauss_noise->stddev = stddev * 0.4330127019;

#ifdef _LITTLE_ENDIAN
	rand_base = *(mlib_s16 *)&seed * IA + IC;
#else /* _LITTLE_ENDIAN */
	rand_base = *(((mlib_s16 *)&seed) + 1) * IA + IC;
#endif /* _LITTLE_ENDIAN */

	rand_base1 = 0x140000 | ((mlib_u32)rand_base >> 16);

#ifdef _LITTLE_ENDIAN
	*(((mlib_s32 *)&gauss_noise->seed) + 1) = rand_base1;
#else /* _LITTLE_ENDIAN */
	*((mlib_s32 *)&gauss_noise->seed) = rand_base1;
#endif /* _LITTLE_ENDIAN */

	rand_base = rand_base * IA + IC;
	rand_base1 = rand_base * IA + IC;
	rand_base = (rand_base & ~0xffff) | ((mlib_u32)rand_base1 >> 16);

#ifdef _LITTLE_ENDIAN
	*((mlib_s32 *)&gauss_noise->seed) = rand_base;
#else /* _LITTLE_ENDIAN */
	*(((mlib_s32 *)&gauss_noise->seed) + 1) = rand_base;
#endif /* _LITTLE_ENDIAN */

	rand_base = rand_base1 * IA + IC;
	rand_base1 = 0x140000 | ((mlib_u32)rand_base >> 16);

#ifdef _LITTLE_ENDIAN
	*(((mlib_s32 *)&gauss_noise->seed1) + 1) = rand_base1;
#else /* _LITTLE_ENDIAN */
	*((mlib_s32 *)&gauss_noise->seed1) = rand_base1;
#endif /* _LITTLE_ENDIAN */

	rand_base = rand_base * IA + IC;
	rand_base1 = rand_base * IA + IC;
	rand_base = (rand_base & ~0xffff) | ((mlib_u32)rand_base1 >> 16);

#ifdef _LITTLE_ENDIAN
	*((mlib_s32 *)&gauss_noise->seed1) = rand_base;
#else /* _LITTLE_ENDIAN */
	*(((mlib_s32 *)&gauss_noise->seed1) + 1) = rand_base;
#endif /* _LITTLE_ENDIAN */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalGaussNoise_F32(
    mlib_f32 *gnoise,
    void *state,
    mlib_s32 n)
{
	struct mlib_GaussNoise_F32 *gauss_noise = state;
	mlib_d64 mean = gauss_noise->mean;
	mlib_d64 stddev = gauss_noise->stddev;
	mlib_d64 dmag = gauss_noise->mag;
	mlib_union64 drandom;
	mlib_union64 drandom1;
	mlib_f32 wnoise[MAX_SIZE];
	mlib_f32 dnoise[MAX_SIZE / NUM];
	mlib_s32 i, j, kol, k = 0;
	mlib_d64 sc = stddev / (mlib_d64)MLIB_S32_MAX;
	__m128d sdrandom, sda;

	if (n <= 0)
		return (MLIB_FAILURE);

	drandom.d64 = gauss_noise->seed;
	drandom1.d64 = gauss_noise->seed1;

	sdrandom = _mm_set_pd(drandom1.d64, drandom.d64);
	sda = _mm_set1_pd(DA);

	while (k < n) {
		kol = n - k;

		if (kol > MAX_SIZE / NUM)
			kol = MAX_SIZE / NUM;

		for (i = 0; i < NUM * kol; i += 2) {
			sdrandom = _mm_mul_pd(sdrandom, sda);
			wnoise[i] = *((mlib_f32 *)&sdrandom);
			wnoise[i + 1] = *((mlib_f32 *)&sdrandom + 2);
		}

		for (j = 0; j < kol; j++) {
			mlib_s32 *pw = (mlib_s32 *)wnoise;
			mlib_d64 acc, int_acc = 0;

			for (i = 0; i < NUM; i += 2) {
				int_acc += (mlib_d64)pw[j * NUM + i] +
						pw[j * NUM + i + 1];
			}

			acc = int_acc * sc + mean;
			dnoise[j] = acc;
		}

		for (j = 0; j < kol; j++) {
			mlib_f32 acc = dnoise[j];

			if (mlib_fabsf(acc) < dmag)
				gnoise[k++] = acc;
		}

		CLEAR_SEED(*((mlib_d64 *)&sdrandom), *((mlib_d64 *)&sdrandom));
		CLEAR_SEED(*((mlib_d64 *)&sdrandom + 1),
				*((mlib_d64 *)&sdrandom + 1));
	}

	gauss_noise->seed = *((mlib_d64 *)&sdrandom);
	gauss_noise->seed1 = *((mlib_d64 *)&sdrandom + 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalGaussNoiseFree_F32(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
