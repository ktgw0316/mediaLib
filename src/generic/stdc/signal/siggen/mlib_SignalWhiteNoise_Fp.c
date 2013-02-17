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

#pragma ident	"@(#)mlib_SignalWhiteNoise_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalWhiteNoiseInit_F32 - allocate the memory for
 *           internal state structure and convert the
 *           parameter into internal representation
 *
 *      mlib_SignalWhiteNoise_F32 - generate one
 *           packet of white noise and update the internal states
 *
 *      mlib_SignalWhiteNoiseFree_F32 - release the memory allocated
 *           for the internal state structure
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalWhiteNoiseInit_F32(void     **state,
 *                                                mlib_f32 mag,
 *                                                mlib_f32 seed)
 *      mlib_status mlib_SignalWhiteNoise_F32(mlib_f32 *wnoise,
 *                                            void     *state,
 *                                            mlib_s32 n)
 *      void mlib_SignalWhiteNoiseFree_F32(void *state)
 *
 * ARGUMENTS
 *      state     Internal state structure
 *      mag       Magnitude of the whitenoise to be generated
 *      wnoise    Generated white noise array
 *      seed      Seed value for pseudo random number generator
 *      n         Length of the array in number of samples
 */

#include <mlib_signal.h>
#include <mlib_SignalSiggen.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalWhiteNoiseFree_F32 = __mlib_SignalWhiteNoiseFree_F32
#pragma weak mlib_SignalWhiteNoiseInit_F32 = __mlib_SignalWhiteNoiseInit_F32
#pragma weak mlib_SignalWhiteNoise_F32 = __mlib_SignalWhiteNoise_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalWhiteNoiseFree_F32) mlib_SignalWhiteNoiseFree_F32
    __attribute__((weak, alias("__mlib_SignalWhiteNoiseFree_F32")));
__typeof__(__mlib_SignalWhiteNoiseInit_F32) mlib_SignalWhiteNoiseInit_F32
    __attribute__((weak, alias("__mlib_SignalWhiteNoiseInit_F32")));
__typeof__(__mlib_SignalWhiteNoise_F32) mlib_SignalWhiteNoise_F32
    __attribute__((weak, alias("__mlib_SignalWhiteNoise_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                      \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <         \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

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

#define	MAX_SIZE	16384

/* *********************************************************** */

mlib_status
__mlib_SignalWhiteNoiseInit_F32(
    void **state,
    mlib_f32 mag,
    mlib_f32 seed)
{
	void *ptr;
	struct mlib_WhiteNoise_F32 *white_noise;
	mlib_s32 rand_base, rand_base1;
	mlib_s16 eseed = (mlib_s16)(FLOAT2INT_CLAMP(seed));

	ptr = __mlib_malloc(sizeof (struct mlib_WhiteNoise_F32));

	if (ptr == NULL)
		return (MLIB_NULLPOINTER);

	*state = ptr;
	white_noise = (struct mlib_WhiteNoise_F32 *)ptr;
	white_noise->mag = mag;

	rand_base = eseed * IA + IC;
	rand_base1 = 0x140000 | ((mlib_u32)rand_base >> 16);

#ifdef _LITTLE_ENDIAN
	*(((mlib_s32 *)&white_noise->seed) + 1) = rand_base1;
#else /* _LITTLE_ENDIAN */
	*((mlib_s32 *)&white_noise->seed) = rand_base1;
#endif /* _LITTLE_ENDIAN */

	rand_base = rand_base * IA + IC;
	rand_base1 = rand_base * IA + IC;
	rand_base = (rand_base & ~0xffff) | ((mlib_u32)rand_base1 >> 16);

#ifdef _LITTLE_ENDIAN
	*((mlib_s32 *)&white_noise->seed) = rand_base;
#else /* _LITTLE_ENDIAN */
	*(((mlib_s32 *)&white_noise->seed) + 1) = rand_base;
#endif /* _LITTLE_ENDIAN */

	rand_base = rand_base1 * IA + IC;
	rand_base1 = 0x140000 | ((mlib_u32)rand_base >> 16);

#ifdef _LITTLE_ENDIAN
	*(((mlib_s32 *)&white_noise->seed1) + 1) = rand_base1;
#else /* _LITTLE_ENDIAN */
	*((mlib_s32 *)&white_noise->seed1) = rand_base1;
#endif /* _LITTLE_ENDIAN */

	rand_base = rand_base * IA + IC;
	rand_base1 = rand_base * IA + IC;
	rand_base = (rand_base & ~0xffff) | ((mlib_u32)rand_base1 >> 16);

#ifdef _LITTLE_ENDIAN
	*((mlib_s32 *)&white_noise->seed1) = rand_base;
#else /* _LITTLE_ENDIAN */
	*(((mlib_s32 *)&white_noise->seed1) + 1) = rand_base;
#endif /* _LITTLE_ENDIAN */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalWhiteNoise_F32(
    mlib_f32 *wnoise,
    void *state,
    mlib_s32 n)
{
	mlib_s32 i, kol, k = 0;
	struct mlib_WhiteNoise_F32 *white_noise = state;
	mlib_union64 drandom0, drandom1;
	mlib_f32 mag = white_noise->mag;
	mlib_d64 sc;

	if (n <= 0)
		return (MLIB_FAILURE);

	drandom0.d64 = white_noise->seed;
	drandom1.d64 = white_noise->seed1;

	sc = mag / 2147483648.;

	if ((mlib_addr)wnoise & 7) {
		drandom0.d64 *= DA;
		(*wnoise++) = *((mlib_s32 *)&drandom0.f32x2.i1) * sc;
		n--;
	}

	while (k < n - 3) {
		kol = n - k;

		if (kol >= MAX_SIZE)
			kol = MAX_SIZE;
		else
			kol &= ~3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < kol >> 1; i++) {
			mlib_union64 dst0;

			drandom0.d64 *= DA;
			drandom1.d64 *= DA;
			dst0.f32x2.i0 = *((mlib_s32 *)&drandom0.f32x2.i1) * sc;
			dst0.f32x2.i1 = *((mlib_s32 *)&drandom1.f32x2.i1) * sc;
			((mlib_d64 *)wnoise)[i] = dst0.d64;
		}

		k += kol;
		wnoise += kol;
		CLEAR_SEED(drandom0.d64, drandom0.d64);
		CLEAR_SEED(drandom1.d64, drandom1.d64);
	}

	for (; k < n; k++) {
		drandom0.d64 *= DA;
		wnoise[0] = *((mlib_s32 *)&drandom0.f32x2.i1) * sc;
		wnoise++;
	}

	CLEAR_SEED(white_noise->seed, drandom0.d64);
	CLEAR_SEED(white_noise->seed1, drandom1.d64);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalWhiteNoiseFree_F32(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
