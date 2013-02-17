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

#pragma ident "@(#)mlib_i_SignalGaussNoise.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalGaussNoiseInit_S16 - allocate the memory for
 *           internal state structure and convert the
 *           parameter into internal representation
 *
 *      mlib_SignalGaussNoise_S16 - generate one
 *           packet of Gaussian noise and update the internal states
 *
 *      mlib_SignalGaussNoiseFree_S16 - release the memory allocated
 *           for the internal state structure
 *
 * SYNOPSIS
 *
 *      void mlib_SignalGaussNoiseFree_S16(void *state);
 *      mlib_status mlib_SignalGaussNoiseInit_S16(void     **state,
 *                                                mlib_s16 mag,
 *                                                mlib_f32 mean,
 *                                                mlib_f32 stddev,
 *                                                mlib_s16 seed);
 *      mlib_status mlib_SignalGaussNoise_S16(mlib_s16 *gnoise,
 *                                            void     *state,
 *                                            mlib_s32 n);
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

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalGaussNoiseFree_S16 = __mlib_SignalGaussNoiseFree_S16
#pragma weak mlib_SignalGaussNoiseInit_S16 = __mlib_SignalGaussNoiseInit_S16
#pragma weak mlib_SignalGaussNoise_S16 = __mlib_SignalGaussNoise_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalGaussNoiseFree_S16) mlib_SignalGaussNoiseFree_S16
    __attribute__((weak, alias("__mlib_SignalGaussNoiseFree_S16")));
__typeof__(__mlib_SignalGaussNoiseInit_S16) mlib_SignalGaussNoiseInit_S16
    __attribute__((weak, alias("__mlib_SignalGaussNoiseInit_S16")));
__typeof__(__mlib_SignalGaussNoise_S16) mlib_SignalGaussNoise_S16
    __attribute__((weak, alias("__mlib_SignalGaussNoise_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

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


/* *********************************************************** */

#define	MAX_SIZE	2048
#define	NUM	16

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

#define	MLIB_INIT_STDDEV(X, Y) {				\
    if (X == 0) Y = 0;						\
    else {                                                      \
	mlib_u64 base, tmp;					\
	mlib_s32 ext;                                   	\
	mlib_s32 base1;						\
	base = (mlib_u64)((X & 0x007fffff | 0x00800000)) * 	\
		    0xddb3d7;					\
	tmp = base >> 47;                                       \
	base1 = (base >> (23 + tmp)) & 0x7fffff;                \
	ext = ((X >> 23) - 2 + tmp) << 23;              	\
	base1 |= ext;                                           \
	Y = *(mlib_f32 *)&base1;                                \
	}							\
}

/* *********************************************************** */

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

#if !defined(_NO_LONGLONG)

mlib_status
__mlib_SignalGaussNoiseInit_S16(
    void **state,
    mlib_s16 mag,
    mlib_f32 mean,
    mlib_f32 stddev,
    mlib_s16 seed)
{
	void *ptr;
	struct mlib_GaussNoise_S16 *gauss_noise;
	mlib_s32 rand_base, rand_base1;
	mlib_s64 smean, mean_exp;
	mlib_s32 sstddev, stddev_exp;
	smean = *((mlib_s32 *)&mean);
	mean_exp = (smean >> 23) & 0xff;
	sstddev = *((mlib_s32 *)&stddev);
	stddev_exp = (sstddev >> 23) & 0xff;

/* if (mlib_fabs(mean) > 1) */
	if (mean_exp > 126)
		return (MLIB_FAILURE);

/* if (mlib_fabs(stddev) > 1) */
	if (stddev_exp > 126)
		return (MLIB_FAILURE);

	ptr = __mlib_malloc(sizeof (struct mlib_GaussNoise_S16));

	if (ptr == NULL)
		return (MLIB_NULLPOINTER);

	*state = ptr;
	gauss_noise = (struct mlib_GaussNoise_S16 *)ptr;
	gauss_noise->mag = mlib_fabs(mag); /* int2double */

/* gauss_noise->mean = mean * 32768. */;
	smean = (smean >> 31 << 63) |
		((mean_exp + 15 + (1023 - 127)) << 52) |
		((smean & 0x7fffff) << (52 - 23));
	*((mlib_s64 *)&gauss_noise->mean) = smean;

/* gauss_noise->stddev = (mlib_f32)(stddev * 0.4330127019); */
	MLIB_INIT_STDDEV(sstddev, gauss_noise->stddev)

	rand_base = seed * IA + IC;
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

#else /* if !defined(_NO_LONGLONG) */

mlib_status
__mlib_SignalGaussNoiseInit_S16(
    void **state,
    mlib_s16 mag,
    mlib_f32 mean,
    mlib_f32 stddev,
    mlib_s16 seed)
{
	void *ptr;
	struct mlib_GaussNoise_S16 *gauss_noise;
	mlib_s32 rand_base, rand_base1;

	if (mlib_fabs(mean) > 1)
		return (MLIB_FAILURE);

	if (mlib_fabs(stddev) > 1)
		return (MLIB_FAILURE);

	ptr = __mlib_malloc(sizeof (struct mlib_GaussNoise_S16));

	if (ptr == NULL)
		return (MLIB_NULLPOINTER);

	*state = ptr;
	gauss_noise = (struct mlib_GaussNoise_S16 *)ptr;
	gauss_noise->mag = mlib_fabs(mag);
	gauss_noise->mean = mean * 32768.;
	gauss_noise->stddev = (mlib_f32)(stddev * 0.4330127019);

	rand_base = seed * IA + IC;
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

#endif /* if !defined(_NO_LONGLONG) */

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

mlib_status
__mlib_SignalGaussNoise_S16(
    mlib_s16 *gnoise,
    void *state,
    mlib_s32 n)
{
	struct mlib_GaussNoise_S16 *gauss_noise = state;
	mlib_f32 mean32 = gauss_noise->mean;
	mlib_s32 smean = *((mlib_s32 *)&mean32);
	mlib_s32 stddev = *((mlib_s32 *)&gauss_noise->stddev);
	mlib_s16 dmag = gauss_noise->mag;
	mlib_s32 acc;
	type_union_mlib_u64 drandom, drandom1;
	mlib_s32 wnoise[MAX_SIZE / 2];
	mlib_s32 i, j, kol, k = 0;
	mlib_s16 sst, mean_exp;
	mlib_s32 mean;

	sst = smean >> 31;
	sst = (~sst & 1) | (sst & (-1));
	mean_exp = (23 - ((smean >> 23) & 0xff) + 127);
	if (mean_exp < 24) {
		mean = ((smean & 0x7FFFFF | 0x800000) >> mean_exp) * sst;
	} else {
		mean = 0;
	}

	if (n <= 0)
		return (MLIB_FAILURE);

	drandom.db = *((mlib_u64 *)&gauss_noise->seed);
	drandom1.db = *((mlib_u64 *)&gauss_noise->seed1);

	while (k < n) {
		kol = n - k;

		if (kol >= (MAX_SIZE / NUM - 4))
			kol = MAX_SIZE / NUM;
		else
			kol += 4;
		for (i = 0; i < NUM / 2 * kol; i += 2) {
			*(mlib_d64 *)&drandom.db *= DA;
			*(mlib_d64 *)&drandom1.db *= DA;
			wnoise[i] = drandom.two_int.fl1;
			wnoise[i + 1] = drandom1.two_int.fl1;
		}

		for (j = 0; j < kol; j++) {
			mlib_s16 *pw = (mlib_s16 *)wnoise;
			mlib_s32 int_acc = 0, abs_acc;
			mlib_s32 int_exp;

			for (i = 0; i < NUM; i++) {
				int_acc += pw[j * NUM + i];
			}

			int_exp = (23 - (stddev >> 23 & 0xff) + 127);
			sst = (stddev >> 31) ^ (int_acc >> 31);
			sst = (~sst & 1) | (sst & (-1));
			/* fabs of int_acc */
			abs_acc = (int_acc ^ (int_acc >> 31)) - (int_acc >> 31);
			int_acc = (mlib_s32)(((mlib_u64)(stddev & 0x7fffff |
			0x800000) * abs_acc + (1 << (int_exp - 1))) >> int_exp);
			acc = (int_acc * sst) + mean;

			if (mlib_fabs(acc) <= dmag && k < n)
				gnoise[k++] = acc;
		}

		CLEAR_SEED(drandom.db, drandom.db);
		CLEAR_SEED(drandom1.db, drandom1.db);
	}

	*((mlib_u64 *)&gauss_noise->seed) = drandom.db;
	*((mlib_u64 *)&gauss_noise->seed1) = drandom1.db;

	return (MLIB_SUCCESS);
}

#else /* if !defined(_NO_LONGLONG) */

mlib_status
__mlib_SignalGaussNoise_S16(
    mlib_s16 *gnoise,
    void *state,
    mlib_s32 n)
{
	struct mlib_GaussNoise_S16 *gauss_noise = state;
	mlib_d64 mean = gauss_noise->mean;
	mlib_d64 stddev = gauss_noise->stddev;
	mlib_d64 dmag = gauss_noise->mag;
	mlib_d64 acc;
	type_union_mlib_d64 drandom, drandom1;
	mlib_f32 wnoise[MAX_SIZE / 2];
	mlib_s32 i, j, kol, k = 0;

	if (n <= 0)
		return (MLIB_FAILURE);
	drandom.db = gauss_noise->seed;
	drandom1.db = gauss_noise->seed1;

	while (k < n) {
		kol = n - k;

		if (kol >= (MAX_SIZE / NUM - 4))
			kol = MAX_SIZE / NUM;
		else
			kol += 4;
		for (i = 0; i < NUM / 2 * kol; i += 2) {
			drandom.db *= DA;
			drandom1.db *= DA;
			wnoise[i] = drandom.two_float.fl1;
			wnoise[i + 1] = drandom1.two_float.fl1;
		}

		for (j = 0; j < kol; j++) {
			mlib_s16 *pw = (mlib_s16 *)wnoise;
			mlib_s32 int_acc = 0;

			for (i = 0; i < NUM; i++) {
				int_acc += pw[j * NUM + i];
			}

			acc = int_acc * stddev + mean;

			if (mlib_fabs(acc) <= dmag && k < n)
				gnoise[k++] = acc;
		}

		CLEAR_SEED(drandom.db, drandom.db);
		CLEAR_SEED(drandom1.db, drandom1.db);
	}

	gauss_noise->seed = drandom.db;
	gauss_noise->seed1 = drandom1.db;

	return (MLIB_SUCCESS);
}

#endif /* if !defined(_NO_LONGLONG) */

/* *********************************************************** */

void
__mlib_SignalGaussNoiseFree_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
