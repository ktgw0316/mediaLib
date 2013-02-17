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

#ifdef __SUNPRO_C
#pragma ident	"@(#)mlib_i_SignalSineWave.c	9.3	07/11/05 SMI"
#endif /* __SUNPRO_C */

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

#define	MLIB_SATURATE_S16(X)                                    \
	(((X) > 0x7fff) ? MLIB_S16_MAX : ((X) < (-0x8000) ?     \
	    MLIB_S16_MIN : (mlib_s16)(X)))

/* *********************************************************** */

#if ! defined(_NO_LONGLONG)

typedef union
{
	mlib_s64 d64;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_s32 i1, i0;
#else		   /* _LITTLE_ENDIAN */
		mlib_s32 i0, i1;
#endif		   /* _LITTLE_ENDIAN */
	} s32x2;
} mlib_union64;

#else /* if! defined(_NO_LONGLONG) */

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

#endif /* if! defined(_NO_LONGLONG) */

/* *********************************************************** */

#if ! defined(_NO_LONGLONG)

#define	EN	10
#define	EM	EN-1

#define	FN	28
#define	FM	FN-1

/* *********************************************************** */

/* Constants for IEEE754 floating point */

/* Exponent bits number for double precision */

#define	MLIB_D64_EXP	11

/* Significand bits number for double precision */

#define	MLIB_D64_FRAC	52

/* Bias for double precision */

#define	MLIB_D64_BIAS	1023

/* *********************************************************** */

/* mlib_d64 to mlib_s64 */
#define	MLIB_DB2LL(X, E)					\
	(((X) << 1) == 0 ? 0 :					\
	    ((((X) & 0x000fffffffffffff | 0x0010000000000000)	\
	    >> (52 - E -((((X) >> 52) & 0x7ff) - 1023)))	\
	    * (((X) >> 63 << 1) + 1)))

/* *********************************************************** */

/*
 * MACRO
 *	MLIB_LEADZERO1_64(X, N) - Leading zero count for 64-bit
 *	                          unsigned integer
 *
 * SYNOPSIS
 *	MLIB_LEADZERO1_64(X, N)
 *
 * ARGUMENTS
 *	Input
 *		X - 64-bit unsigned integer
 *	Output
 *		N - leading zero bits number, saved as mlib_s16
 */

#define	MLIB_LEADZERO1_64(X, N) {			\
	unsigned long long y, x = X;			\
	int n;						\
	n = 64;						\
	y = x >> 32;					\
	if (y != 0) {					\
		n = n - 32;				\
		x = y;					\
	}						\
	y = x >> 16;					\
	if (y != 0) {					\
		n = n - 16;				\
		x = y;					\
	}						\
	y = x >> 8;					\
	if (y != 0) {					\
		n = n - 8;				\
		x = y;					\
	}						\
	y = x >> 4;					\
	if (y != 0) {					\
		n = n - 4;				\
		x = y;					\
	}						\
	y = x >> 2;					\
	if (y != 0) {					\
		n = n - 2;				\
		x = y;					\
	}						\
	y = x >> 1;					\
	N = ((-(long long)y) >> 63) & (n - 2) |		\
	    ~((-(long long)y) >> 63) & (n - x);		\
}

/* *********************************************************** */

/*
 * MACRO
 *	MLIB_INTEGER_TO_DOUBLE - Convert 64-bit signed integer
 *				 representation to IEEE double
 *				 precision floating point
 *				 representation with scaling
 *
 * SYNOPSIS
 *	MLIB_INTEGER_TO_DOUBLE(Q, P, S)
 *
 * ARGUMENTS
 * 	Input
 * 		P - 64-bit signed integer
 * 		S - an integer scaling factor
 * 	Output
 * 		Q - IEEE double precision floating point
 *		representation in a 64-bit signed integer
 *
 * RESTRICTIONS
 *	-127 < S < 127
 *
 * DESCRIPTION
 *	MLIB_INTEGER_TO_DOUBLE converts (P * 2^S) from 64-bit signed
 *	integer representation to IEEE double precision floating
 *	point representation and save it in Q.
 *
 *	The exponent field of Q (bits 52-62) is set to following
 *
 *		(64 - LZ - 1) + 1023 - S
 *
 *	The fraction field of Q (bits 0-51) is set to following
 *
 *		(ABS(P) >> (11 - LZ)) | LS    if (LZ < 12)
 *	or
 *		(ABS(P) << (LZ - 11)) | LS    if (LZ >= 12)
 *
 *	where LZ is the number of leading zero bits in P.
 */

#define	MLIB_INTEGER_TO_DOUBLE(Q, P, S)                 \
{                                                       \
/* exponent */                                          \
	mlib_s64 e;                                     \
/* fraction */                                          \
	mlib_s64 f;                                     \
	mlib_s64 r, mask;                               \
/* leading zero number */                               \
	mlib_s16 lz;                                    \
	mlib_s16 a;                                     \
	mlib_s64 d;					\
     		                                        \
/* abs of P */	                                        \
	mask = P >> 63;                                 \
	r = (P ^ mask) - mask;                          \
	                                                \
	MLIB_LEADZERO1_64(r, lz)                        \
                                                        \
	e = (63 - lz) + MLIB_D64_BIAS - S;              \
                                                        \
	a = ((lz - MLIB_D64_EXP - 1) >> 15);            \
	f = (a & (r >> (MLIB_D64_EXP - lz))) |          \
	    ((~a) & (r << (lz - MLIB_D64_EXP)));        \
                                                        \
	d = ((lz - 64) >> 16) &                         \
	    ((f & 0xfffffffffffff) |                    \
	    ((e & 0x7ff) << MLIB_D64_FRAC) |            \
	    (((-lz) >> 16) & P & 0x8000000000000000));  \
	Q = *((mlib_d64 *)&d);				\
}

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
	    (sist->type != MLIB_SINE_S16)) {
		return (MLIB_FAILURE);
	}

	{
		mlib_union64 tmp0, tmp1;
		mlib_s64 tmp2[2];

		mlib_s64 sbase1 = (mlib_s64)((sist->sbase1) * (1LL << EN));
		mlib_s64 sbase2 = (mlib_s64)((sist->sbase2) * (1LL << EN));
		mlib_s64 sbase3 = (mlib_s64)((sist->sbase3) * (1LL << EN));
		mlib_s64 sbase4 = (mlib_s64)((sist->sbase4) * (1LL << EN));
		mlib_s64 sbase5 = (mlib_s64)((sist->sbase5) * (1LL << EN));
		mlib_s64 sbase6 = (mlib_s64)((sist->sbase6) * (1LL << EN));
		mlib_s64 sbase7 = (mlib_s64)((sist->sbase7) * (1LL << EN));
		mlib_s64 sbase8 = (mlib_s64)((sist->sbase8) * (1LL << EN));

		mlib_s64 cstep = MLIB_DB2LL((*((mlib_s64 *)&sist->cstep)), FN);

		mlib_s32 i;

		if ((mlib_addr)sine & 3) {
			mlib_s64 snext0 =
			    (((cstep * sbase5 + (1LL << FM)) >> FN) - sbase1);

			mlib_s64 stemp;
			mlib_s16 sinetemp;
			stemp = (sbase1 + (1 << EM)) >> EN;
			sine[0] = MLIB_SATURATE_S16(stemp);

			sbase1 = sbase2;
			sbase2 = sbase3;
			sbase3 = sbase4;
			sbase4 = sbase5;
			sbase5 = sbase6;
			sbase6 = sbase7;
			sbase7 = sbase8;
			sbase8 = snext0;
			n--;
			sine++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 2); i++) {
			mlib_s64 snext0 =
			    ((cstep * sbase5 + (1LL << FM)) >> FN) - sbase1;
			mlib_s64 snext1 =
			    ((cstep * sbase6 + (1LL << FM)) >> FN) - sbase2;
			mlib_s64 snext2 =
			    ((cstep * sbase7 + (1LL << FM)) >> FN) - sbase3;
			mlib_s64 snext3 =
			    ((cstep * sbase8 + (1LL << FM)) >> FN) - sbase4;

			tmp0.s32x2.i0 =
			    MLIB_SATURATE_S16((((sbase1 + (1LL << EM)) >> EN)));
			tmp0.s32x2.i1 =
			    MLIB_SATURATE_S16((((sbase2 + (1LL << EM)) >> EN)));
			tmp1.s32x2.i0 =
			    MLIB_SATURATE_S16((((sbase3 + (1LL << EM)) >> EN)));
			tmp1.s32x2.i1 =
			    MLIB_SATURATE_S16((((sbase4 + (1LL << EM)) >> EN)));

			sbase1 = sbase5;
			sbase2 = sbase6;
			sbase3 = sbase7;
			sbase4 = sbase8;
			sbase5 = snext0;
			sbase6 = snext1;
			sbase7 = snext2;
			sbase8 = snext3;
			tmp2[0] = tmp0.d64;
			tmp2[1] = tmp1.d64;

			((mlib_s32 *)sine)[2 * i] =
			    (((mlib_s32 *)tmp2)[0] << 16)
			    | (((mlib_s32 *)tmp2)[1] & 0xFFFF);
			((mlib_s32 *)sine)[2 * i + 1] =
			    (((mlib_s32 *)tmp2)[2] << 16)
			    | (((mlib_s32 *)tmp2)[3] & 0xFFFF);
		}

		i <<= 2;

		while (i < n) {
			mlib_s64 snext0 =
			    (((cstep * sbase5 + (1LL << FM)) >> FN) - sbase1);

			mlib_s64 stemp;
			mlib_s16 sinetemp;

			stemp = (sbase1 + (1LL << EM)) >> EN;
			sine[i++] = MLIB_SATURATE_S16(stemp);

			sbase1 = sbase2;
			sbase2 = sbase3;
			sbase3 = sbase4;
			sbase4 = sbase5;
			sbase5 = sbase6;
			sbase6 = sbase7;
			sbase7 = sbase8;
			sbase8 = snext0;
		}

		MLIB_INTEGER_TO_DOUBLE(sist->sbase1, (mlib_s64)sbase1, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase2, (mlib_s64)sbase2, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase3, (mlib_s64)sbase3, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase4, (mlib_s64)sbase4, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase5, (mlib_s64)sbase5, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase6, (mlib_s64)sbase6, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase7, (mlib_s64)sbase7, EN)
		MLIB_INTEGER_TO_DOUBLE(sist->sbase8, (mlib_s64)sbase8, EN)
	}

	return (MLIB_SUCCESS);
}

#else /* if! defined(_NO_LONGLONG) */

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
		mlib_d64 tmp2[2];
		mlib_d64 sbase1 = sist->sbase1, sbase2 = sist->sbase2;
		mlib_d64 sbase3 = sist->sbase3, sbase4 = sist->sbase4;
		mlib_d64 sbase5 = sist->sbase5, sbase6 = sist->sbase6;
		mlib_d64 sbase7 = sist->sbase7, sbase8 = sist->sbase8;
		mlib_d64 cstep = sist->cstep;
		mlib_s32 i;

		if ((mlib_addr)sine & 3) {
			mlib_d64 snext0 = cstep * sbase5 - sbase1;

			sine[0] = sbase1;
			sbase1 = sbase2;
			sbase2 = sbase3;
			sbase3 = sbase4;
			sbase4 = sbase5;
			sbase5 = sbase6;
			sbase6 = sbase7;
			sbase7 = sbase8;
			sbase8 = snext0;
			n--;
			sine++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 2); i++) {
			mlib_d64 snext0 = cstep * sbase5 - sbase1;
			mlib_d64 snext1 = cstep * sbase6 - sbase2;
			mlib_d64 snext2 = cstep * sbase7 - sbase3;
			mlib_d64 snext3 = cstep * sbase8 - sbase4;

			tmp0.s32x2.i0 = sbase1;
			tmp0.s32x2.i1 = sbase2;
			tmp1.s32x2.i0 = sbase3;
			tmp1.s32x2.i1 = sbase4;
			sbase1 = sbase5;
			sbase2 = sbase6;
			sbase3 = sbase7;
			sbase4 = sbase8;
			sbase5 = snext0;
			sbase6 = snext1;
			sbase7 = snext2;
			sbase8 = snext3;
			tmp2[0] = tmp0.d64;
			tmp2[1] = tmp1.d64;
			((mlib_s32 *)sine)[2 * i] =
			    (((mlib_s32 *)tmp2)[0] << 16)
			    | (((mlib_s32 *)tmp2)[1] & 0xFFFF);
			((mlib_s32 *)sine)[2 * i + 1] =
			    (((mlib_s32 *)tmp2)[2] << 16)
			    | (((mlib_s32 *)tmp2)[3] & 0xFFFF);
		}

		i <<= 2;

		while (i < n) {
			mlib_d64 snext0 = cstep * sbase5 - sbase1;

			sine[i++] = sbase1;
			sbase1 = sbase2;
			sbase2 = sbase3;
			sbase3 = sbase4;
			sbase4 = sbase5;
			sbase5 = sbase6;
			sbase6 = sbase7;
			sbase7 = sbase8;
			sbase8 = snext0;
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

#endif /* if! defined(_NO_LONGLONG) */

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

#if ! defined(_NO_LONGLONG)

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

#else /* if! defined(_NO_LONGLONG) */

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

#endif /* if! defined(_NO_LONGLONG) */

/* *********************************************************** */
