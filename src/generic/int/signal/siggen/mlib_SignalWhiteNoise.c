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

#pragma ident	"@(#)mlib_SignalWhiteNoise.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalWhiteNoiseInit_S16 - allocate the memory for
 *           internal state structure and convert the
 *           parameter into internal representation
 *
 *      mlib_SignalWhiteNoise_S16 - generate one
 *           packet of white noise and update the internal states
 *
 *      mlib_SignalWhiteNoiseFree_S16 - release the memory allocated
 *           for the internal state structure
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalWhiteNoiseInit_S16(void     **state,
 *                                                mlib_s16 mag,
 *                                                mlib_s16 seed);
 *      mlib_status mlib_SignalWhiteNoise_S16(mlib_s16 *wnoise,
 *                                            void     *state,
 *                                            mlib_s32 n);
 *      void mlib_SignalWhiteNoiseFree_S16(void *state);
 *
 * ARGUMENTS
 *      state     Internal state structure
 *      mag       Magnitude of the whitenoise tobe generated
 *      wnoise    Generated white noise array
 *      seed      Seed value for pseudo random number generator
 *      n         Length of the array in number of samples
 */

#include <stdlib.h>
#include <mlib_signal.h>
#include <mlib_SignalSiggen.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalWhiteNoiseFree_S16 = __mlib_SignalWhiteNoiseFree_S16
#pragma weak mlib_SignalWhiteNoiseInit_S16 = __mlib_SignalWhiteNoiseInit_S16
#pragma weak mlib_SignalWhiteNoise_S16 = __mlib_SignalWhiteNoise_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalWhiteNoiseFree_S16) mlib_SignalWhiteNoiseFree_S16
    __attribute__((weak, alias("__mlib_SignalWhiteNoiseFree_S16")));
__typeof__(__mlib_SignalWhiteNoiseInit_S16) mlib_SignalWhiteNoiseInit_S16
    __attribute__((weak, alias("__mlib_SignalWhiteNoiseInit_S16")));
__typeof__(__mlib_SignalWhiteNoise_S16) mlib_SignalWhiteNoise_S16
    __attribute__((weak, alias("__mlib_SignalWhiteNoise_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

typedef union
{
	mlib_u64 db;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_s32 fl1, fl0;
#else		   /* _LITTLE_ENDIAN */
		mlib_s32 fl0, fl1;
#endif		   /* _LITTLE_ENDIAN */
	} two_int;
} type_union_mlib_u64;

#else /* if !defined(_NO_LONGLONG) */

typedef union
{
	mlib_d64 db;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_f32 fl1, fl0;
#else		   /* _LITTLE_ENDIAN */
		mlib_f32 fl0, fl1;
#endif		   /* _LITTLE_ENDIAN */
	} two_float;
} type_union_mlib_d64;

#endif /* if !defined(_NO_LONGLONG) */

/* *********************************************************** */

#define	MAX_NUM	4096

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

#define	DAL	0x1071939b
#define	DAR	0x6977b1

/*
 * MACRO
 *	MLIB_MUL_D64(A, B) - 64-bit floating point multiply
 *
 * SYNOPSIS
 *	MLIB_MUL_D64(A, B)
 *
 * ARGUMENTS
 *	Input
 *		A - 64-bit floating point
 *	Output
 *		B - 64-bit floating point
 *
 * DESCRIPTION
 * 	B = A * DA
 * 	A * DA = (base1 * 2^29 + base2) * (DAL * 2^29 + DAR)
 *
 * 	Keep the 54-bit temparary multiply result.
 *
 * 	DA = 1.02772865970426274806
 *       (0x3ff07193, 0x9b6977b1)
 *  DAL = 0x1071939b
 *  DAR = 0x6977b1
 */

#define	MLIB_MUL_D64(A, B) {						\
	mlib_u64 base, base1, base2, ext, tmp;				\
	base1 = ((A & 0x000fffffff000000) >> 24) | 0x0010000000;	\
	base2 = A & 0xffffff;						\
	base = ((base1 * 0x1071939b + (1 << 3)) >> 4) +			\
	    ((base1 * 0x6977b + (1 << 23)) >> 24) + 			\
	    ((base2 * 0x1071939b + (1 << 23)) >> 24);			\
									\
	tmp = base >> 53;						\
	base = (base >> tmp) & 0xfffffffffffff;				\
	ext = (A & 0xfff0000000000000) + (tmp << 52);			\
	B = base | ext;							\
}

#endif /* if !defined(_NO_LONGLONG) */

/* *********************************************************** */

mlib_status
__mlib_SignalWhiteNoiseInit_S16(
    void **state,
    mlib_s16 mag,
    mlib_s16 seed)
{
	void *ptr;
	struct mlib_WhiteNoise_S16 *white_noise;
	mlib_s32 rand_base, rand_base1;

	ptr = __mlib_malloc(sizeof (struct mlib_WhiteNoise_S16));

	if (ptr == NULL)
		return (MLIB_NULLPOINTER);

	*state = ptr;
	white_noise = (struct mlib_WhiteNoise_S16 *)ptr;
	white_noise->mag = mag;

	rand_base = seed * IA + IC;
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

#if !defined(_NO_LONGLONG)

mlib_status
__mlib_SignalWhiteNoise_S16(
    mlib_s16 *wnoise,
    void *state,
    mlib_s32 n)
{
	struct mlib_WhiteNoise_S16 *white_noise = state;
	type_union_mlib_u64 drandom, drandom1;
	mlib_s16 mag = white_noise->mag;
	mlib_s32 i, j, kol;

	if (n <= 0)
		return (MLIB_FAILURE);

	drandom.db = *((mlib_u64 *)&white_noise->seed);
	drandom1.db = *((mlib_u64 *)&white_noise->seed1);

	if (i = ((mlib_addr)wnoise & 2) >> 1) {
		mlib_u64 res;
		MLIB_MUL_D64(drandom.db, drandom.db);
#ifdef _LITTLE_ENDIAN
		wnoise[0] = ((mlib_s16 *)&drandom.db)[0];
#else /* _LITTLE_ENDIAN */
		wnoise[0] = ((mlib_s16 *)&drandom.db)[3];
#endif /* _LITTLE_ENDIAN */
	}

	while (i < n - 3) {
		kol = n - i;

		if (kol > MAX_NUM)
			kol = MAX_NUM;
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (j = 0; j < kol - 3; j += 4) {
			mlib_u64 res1, res2;
#if 0
			MLIB_MUL_D64(drandom.db, drandom.db);
			MLIB_MUL_D64(drandom1.db, drandom1.db);
#endif
			(*(mlib_d64 *)&drandom.db) *= DA;
			(*(mlib_d64 *)&drandom1.db) *= DA;
			*((mlib_s32 *)(wnoise + i + j)) = drandom.two_int.fl1;
			*((mlib_s32 *)(wnoise + i + j + 2)) =
				drandom1.two_int.fl1;
		}

		i += kol & ~3;
		CLEAR_SEED(drandom.db, drandom.db);
		CLEAR_SEED(drandom1.db, drandom1.db);
	}

	if (i < n) {
		if (i <= (n - 2)) {
			mlib_u64 res;
			MLIB_MUL_D64(drandom.db, drandom.db);
			*((mlib_s32 *)(wnoise + i)) = drandom.two_int.fl1;
			i += 2;
		}

		if (i < n) {
			mlib_u64 res;
			MLIB_MUL_D64(drandom1.db, drandom1.db);

#ifdef _LITTLE_ENDIAN
			wnoise[i] = ((mlib_s16 *)&drandom1.db)[0];
#else /* _LITTLE_ENDIAN */
			wnoise[i] = ((mlib_s16 *)&drandom1.db)[3];
#endif /* _LITTLE_ENDIAN */
		}
	}

	*((mlib_u64 *)&white_noise->seed) = drandom.db;
	*((mlib_u64 *)&white_noise->seed1) = drandom1.db;

	if (abs(mag) < 32767) {
		__mlib_SignalMulS_S16_Sat(wnoise, &mag, n);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#else /* if !defined(_NO_LONGLONG) */

/* *********************************************************** */

mlib_status
__mlib_SignalWhiteNoise_S16(
    mlib_s16 *wnoise,
    void *state,
    mlib_s32 n)
{
	struct mlib_WhiteNoise_S16 *white_noise = state;
	type_union_mlib_d64 drandom, drandom1;
	mlib_s16 mag = white_noise->mag;
	mlib_s32 i, j, kol;

	if (n <= 0)
		return (MLIB_FAILURE);

	drandom.db = white_noise->seed;
	drandom1.db = white_noise->seed1;

	if (i = ((mlib_addr)wnoise & 2) >> 1) {

		drandom.db *= DA;
#ifdef _LITTLE_ENDIAN
		wnoise[0] = ((mlib_s16 *)&drandom.db)[0];
#else /* _LITTLE_ENDIAN */
		wnoise[0] = ((mlib_s16 *)&drandom.db)[3];
#endif /* _LITTLE_ENDIAN */
	}

	while (i < n - 3) {
		kol = n - i;

		if (kol > MAX_NUM)
			kol = MAX_NUM;
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (j = 0; j < kol - 3; j += 4) {
			drandom.db *= DA;
			drandom1.db *= DA;
			*((mlib_f32 *)(wnoise + i + j)) = drandom.two_float.fl1;
			*((mlib_f32 *)(wnoise + i + j + 2)) =
			    drandom1.two_float.fl1;
		}

		i += kol & ~3;
		CLEAR_SEED(drandom.db, drandom.db);
		CLEAR_SEED(drandom1.db, drandom1.db);
	}

	if (i < n) {
		if (i <= (n - 2)) {
			drandom.db *= DA;
			*((mlib_f32 *)(wnoise + i)) = drandom.two_float.fl1;
			i += 2;
		}

		if (i < n) {
			drandom1.db *= DA;

#ifdef _LITTLE_ENDIAN
			wnoise[i] = ((mlib_s16 *)&drandom1.db)[0];
#else /* _LITTLE_ENDIAN */
			wnoise[i] = ((mlib_s16 *)&drandom1.db)[3];
#endif /* _LITTLE_ENDIAN */
		}
	}

	white_noise->seed = drandom.db;
	white_noise->seed1 = drandom1.db;

	if (abs(mag) < 32767) {
		__mlib_SignalMulS_S16_Sat(wnoise, &mag, n);
	}

	return (MLIB_SUCCESS);
}

#endif /* if !defined(_NO_LONGLONG) */

/* *********************************************************** */

void
__mlib_SignalWhiteNoiseFree_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
