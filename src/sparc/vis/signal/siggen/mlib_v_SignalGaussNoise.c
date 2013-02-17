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

#pragma ident	"@(#)mlib_v_SignalGaussNoise.c	9.3	07/11/05 SMI"

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
 *      mlib_status mlib_SignalGaussNoiseInit_S16
 *                                           (void     **state,
 *                                            mlib_s16 mag,
 *                                            mlib_f32 mean,
 *                                            mlib_f32 stddev,
 *                                            mlib_s16 seed)
 *      mlib_status mlib_SignalGaussNoise_S16
 *                                       (mlib_s16 *gnoise,
 *                                        void     *state,
 *                                        mlib_s32 n)
 *      void mlib_SignalGaussNoiseFree_S16 (void *state)
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

#include <stdlib.h>
#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalSiggen.h>
#include <vis_proto.h>

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
	mlib_d64 db;
	struct
	{
		mlib_f32 fl0, fl1;
	} two_float;
} type_union_mlib_d64;

/* *********************************************************** */

#define	MAX_SIZE	1024
#define	NUM	16

/* *********************************************************** */

#define	DR_CALC(i)                                                       \
	{                                                                \
	    dr = wnoise[NUM / 4 * i];                                    \
	    dr = vis_fmul8x16(foneby16, dr);                             \
	    dr4 = wnoise[NUM / 4 * i + 1];                               \
	    dr4 = vis_fmul8x16(foneby16, dr4);                           \
	    dr1 = wnoise[NUM / 4 * i + 2];                               \
	    dr1 = vis_fmul8x16(foneby16, dr1);                           \
	    dr = vis_fpadd16(dr, dr1);                                   \
	    dr5 = wnoise[NUM / 4 * i + 3];                               \
	    dr5 = vis_fmul8x16(foneby16, dr5);                           \
	    dr4 = vis_fpadd16(dr4, dr5);                                 \
	    dr2 = wnoise[NUM / 4 * i + 4];                               \
	    dr2 = vis_fmul8x16(foneby16, dr2);                           \
	    dr = vis_fpadd16(dr, dr2);                                   \
	    dr6 = wnoise[NUM / 4 * i + 5];                               \
	    dr6 = vis_fmul8x16(foneby16, dr6);                           \
	    dr4 = vis_fpadd16(dr4, dr6);                                 \
	    dr3 = wnoise[NUM / 4 * i + 6];                               \
	    dr3 = vis_fmul8x16(foneby16, dr3);                           \
	    dr = vis_fpadd16(dr, dr3);                                   \
	    dr7 = wnoise[NUM / 4 * i + 7];                               \
	    dr7 = vis_fmul8x16(foneby16, dr7);                           \
	    dr4 = vis_fpadd16(dr4, dr7);                                 \
	    dr1 = wnoise[NUM / 4 * i + 8];                               \
	    dr1 = vis_fmul8x16(foneby16, dr1);                           \
	    dr = vis_fpadd16(dr, dr1);                                   \
	    dr5 = wnoise[NUM / 4 * i + 9];                               \
	    dr5 = vis_fmul8x16(foneby16, dr5);                           \
	    dr4 = vis_fpadd16(dr4, dr5);                                 \
	    dr2 = wnoise[NUM / 4 * i + 10];                              \
	    dr2 = vis_fmul8x16(foneby16, dr2);                           \
	    dr = vis_fpadd16(dr, dr2);                                   \
	    dr6 = wnoise[NUM / 4 * i + 11];                              \
	    dr6 = vis_fmul8x16(foneby16, dr6);                           \
	    dr4 = vis_fpadd16(dr4, dr6);                                 \
	    dr3 = wnoise[NUM / 4 * i + 12];                              \
	    dr3 = vis_fmul8x16(foneby16, dr3);                           \
	    dr = vis_fpadd16(dr, dr3);                                   \
	    dr7 = wnoise[NUM / 4 * i + 13];                              \
	    dr7 = vis_fmul8x16(foneby16, dr7);                           \
	    dr4 = vis_fpadd16(dr4, dr7);                                 \
	    dr1 = wnoise[NUM / 4 * i + 14];                              \
	    dr1 = vis_fmul8x16(foneby16, dr1);                           \
	    dr = vis_fpadd16(dr, dr1);                                   \
	    dr5 = wnoise[NUM / 4 * i + 15];                              \
	    dr5 = vis_fmul8x16(foneby16, dr5);                           \
	    dr4 = vis_fpadd16(dr4, dr5);                                 \
	    dr = vis_fpadd16(dr, dr4);                                   \
	    dr1 = vis_fmuld8sux16(vis_read_hi(dr), fstddev);             \
	    dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fstddev);             \
	    dr1 = vis_fpadd32(dr1, dr2);                                 \
	    dr1 = vis_fpadd32(dr1, dmean);                               \
	    dr4 = vis_fmuld8sux16(vis_read_lo(dr), fstddev);             \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dr), fstddev);             \
	    dr5 = vis_fpadd32(dr4, dr5);                                 \
	    dr5 = vis_fpadd32(dr5, dmean);                               \
	    dr = vis_fpackfix_pair(dr1, dr5);                            \
	    mask1 = vis_fcmple32(dr1, dmax) & vis_fcmpge32(dr1, dmin);   \
	    mask2 = vis_fcmple32(dr5, dmax) & vis_fcmpge32(dr5, dmin);   \
	}

/* *********************************************************** */

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
	mlib_s32 rand_base, rand_base1, istddev, imean, imag;

	if (mlib_fabs(mean) > 1)
		return (MLIB_FAILURE);

	if (mlib_fabs(stddev) > 1)
		return (MLIB_FAILURE);

	ptr =
	    /* calloc(sizeof (struct mlib_GaussNoise_S16), 1); */
	    __mlib_malloc(sizeof (struct mlib_GaussNoise_S16));

	if (ptr == NULL)
		return (MLIB_NULLPOINTER);

	*state = ptr;
	gauss_noise = (struct mlib_GaussNoise_S16 *)ptr;

	imag = abs(mag);
/* convert mag = 0x8000 to 0x7fff */
	imag -= (imag >> 15);
	gauss_noise->mag = vis_to_double_dup(imag << 12);
	gauss_noise->min = vis_to_double_dup((-imag) << 12);
	imean = mean * 32768 * 4096;
	gauss_noise->mean = vis_to_double_dup(imean);
	istddev = stddev * 6.92820323 * 4096;
	gauss_noise->stddev = vis_to_float((istddev << 16) | istddev);

	rand_base = seed * IA + IC;
	rand_base1 = 0x140000 | ((mlib_u32)rand_base >> 16);
	*((mlib_s32 *)&gauss_noise->seed) = rand_base1;
	rand_base = rand_base * IA + IC;
	rand_base1 = rand_base * IA + IC;
	rand_base = (rand_base & ~0xffff) | ((mlib_u32)rand_base1 >> 16);
	*(((mlib_s32 *)&gauss_noise->seed) + 1) = rand_base;

	rand_base = rand_base1 * IA + IC;
	rand_base1 = 0x140000 | ((mlib_u32)rand_base >> 16);
	*((mlib_s32 *)&gauss_noise->seed1) = rand_base1;
	rand_base = rand_base * IA + IC;
	rand_base1 = rand_base * IA + IC;
	rand_base = (rand_base & ~0xffff) | ((mlib_u32)rand_base1 >> 16);
	*(((mlib_s32 *)&gauss_noise->seed1) + 1) = rand_base;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalGaussNoise_S16(
    mlib_s16 *gnoise,
    void *state,
    mlib_s32 n)
{
	struct mlib_GaussNoise_S16 *gauss_noise = state;
	mlib_d64 dmean = gauss_noise->mean, dmax = gauss_noise->mag;
	mlib_d64 dmin = gauss_noise->min, wnoise[MAX_SIZE / 4];
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
	type_union_mlib_d64 drandom, drandom1;
	mlib_f32 fstddev = gauss_noise->stddev, foneby16 =
	    vis_to_float(0x10101010);
	mlib_s32 i, kol, mask1, mask2, emask;

	mlib_s32 al_add, ost, lin, num, br = 1;
	mlib_d64 *al_src;

	al_add =
	    ((((((mlib_s64)gnoise) & ~7) - (mlib_s64)gnoise) == 0) ? 0 : 8);
	al_src = (mlib_d64 *)((((mlib_s64)gnoise) & ~7) + (mlib_s64)al_add);

	if (n <= 0)
		return (MLIB_FAILURE);
	drandom.db = gauss_noise->seed;
	drandom1.db = gauss_noise->seed1;

	vis_write_gsr((4 << 3));

	if (al_add != 0) {
		while (br) {
			for (i = 0; i < NUM * 2; i += 2) {
				drandom.db *= DA;
				drandom1.db *= DA;
				((mlib_f32 *)wnoise)[i] = drandom.two_float.fl1;
				((mlib_f32 *)wnoise)[i + 1] =
				    drandom1.two_float.fl1;
			}

			DR_CALC(0);

			if (mask1 & mask2 == 3) {
				emask = vis_edge16(gnoise, al_src);
				vis_pst_16(dr, (void *)(&(al_src[-1])), emask);
				br = 0;
			}

			CLEAR_SEED(drandom.db, drandom.db);
			CLEAR_SEED(drandom1.db, drandom1.db);
		}
	}

	num = (n - (((mlib_s32)al_src - (mlib_s32)gnoise) >> 1)) >> 2;
	lin = 0;

	while (lin < num) {
		kol = (num - lin) << 2;

		if (kol > MAX_SIZE / NUM)
			kol = MAX_SIZE / NUM;
		for (i = 0; i < NUM / 2 * kol; i += 2) {
			drandom.db *= DA;
			drandom1.db *= DA;
			((mlib_f32 *)wnoise)[i] = drandom.two_float.fl1;
			((mlib_f32 *)wnoise)[i + 1] = drandom1.two_float.fl1;
		}

		for (i = 0; i < kol; i += 4) {

			DR_CALC(i);

			if ((mask1 & mask2) == 3) {
				al_src[lin] = dr;
				lin++;
			}
		}

		CLEAR_SEED(drandom.db, drandom.db);
		CLEAR_SEED(drandom1.db, drandom1.db);
	}

	ost = n - (num << 2) - (((mlib_s32)al_src - (mlib_s32)gnoise) >> 1);
	br = 1;

	if (ost != 0) {
		while (br) {
			for (i = 0; i < NUM * 2; i += 2) {
				drandom.db *= DA;
				drandom1.db *= DA;
				((mlib_f32 *)wnoise)[i] = drandom.two_float.fl1;
				((mlib_f32 *)wnoise)[i + 1] =
				    drandom1.two_float.fl1;
			}

			DR_CALC(0);

			if (mask1 & mask2 == 3) {
				emask =
				    vis_edge16(&(al_src[lin]),
				    &(gnoise[n - 1]));
				vis_pst_16(dr, (void *)(&(al_src[lin])), emask);
				br = 0;
			}

			CLEAR_SEED(drandom.db, drandom.db);
			CLEAR_SEED(drandom1.db, drandom1.db);
		}
	}

	gauss_noise->seed = drandom.db;
	gauss_noise->seed1 = drandom1.db;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalGaussNoiseFree_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
