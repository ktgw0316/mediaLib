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

#pragma ident	"@(#)mlib_v_SignalSineWave.c	9.2	07/11/05 SMI"

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
 *      mlib_status mlib_SignalSineWaveInit_S16(void                   **state,
 *                                              mlib_s16 mag, mlib_f32 freg,
 *                                              mlib_f32               phase)
 *
 *      mlib_status mlib_SignalSineWave_S16(mlib_s16              *sine,
 *                                          void *state, mlib_s32 n)
 *
 *      void mlib_SignalSineWaveFree_S16(void *state)
 *
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
 *     Tone Generation:     sine[i] = mag*sin(freq*i+phase)
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalSiggen.h>
#include <vis_proto.h>

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

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc >> 16;                         \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

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

#define	S16TOS32(X, Y)	(((X) << 16) | ((Y) & 0xFFFF))

/* *********************************************************** */

#define	S16TOD64(RES, X0, X1, X2, X3)                           \
	{                                                       \
	    mlib_union64 u_tmp0, u_tmp1;                        \
	                                                        \
	    u_tmp0.s32x2.i0 = X0;                               \
	    u_tmp0.s32x2.i1 = X1;                               \
	    u_tmp1.s32x2.i0 = X2;                               \
	    u_tmp1.s32x2.i1 = X3;                               \
	    u_tmp0.d64 = vis_fpadd32(u_tmp0.d64, u_tmp0.d64);   \
	    u_tmp1.d64 = vis_fpadd32(u_tmp1.d64, u_tmp1.d64);   \
	    RES = vis_fpackfix_pair(u_tmp0.d64, u_tmp1.d64);    \
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
	    (sist->type != MLIB_SINE_S16))
		return (MLIB_FAILURE);

	vis_write_gsr(15 << 3);

	{
		mlib_union64 tmp0, tmp1;
		mlib_d64 tmp2[2];
		mlib_d64 sbase1 = sist->sbase1, sbase2 = sist->sbase2;
		mlib_d64 sbase3 = sist->sbase3, sbase4 = sist->sbase4;
		mlib_d64 sbase5 = sist->sbase5, sbase6 = sist->sbase6;
		mlib_d64 sbase7 = sist->sbase7, sbase8 = sist->sbase8;
		mlib_d64 cstep = sist->cstep;
		mlib_s32 i;

		while (((mlib_addr)sine & 7) && (n > 0)) {
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

		if (sist->flag) {
			mlib_d64 w_cstep8 = sist->w_cstep8;
			mlib_d64 cstep1 = sist->cstep1;
			mlib_d64 cstep8 = sist->cstep8;
			mlib_d64 cstep16 = sist->cstep16;
			mlib_d64 cstep32 = sist->cstep32;
			mlib_d64 cstep64 = sist->cstep64;
			mlib_d64 cstep128 = sist->cstep128;

			while (n >= 256) {
				mlib_d64 w_sbase0, w_sbase1, w_sbase2, w_sbase3;
				mlib_d64 w_snext0, w_snext1;
				mlib_d64 sbase9, sbase10;

				S16TOD64(w_sbase0, sbase1, sbase2, sbase3,
				    sbase4);
				S16TOD64(w_sbase1, sbase5, sbase6, sbase7,
				    sbase8);

				sbase9 = cstep * sbase5 - sbase1;
				sbase10 = cstep * sbase6 - sbase2;
				sbase3 = cstep * sbase7 - sbase3;
				sbase4 = cstep * sbase8 - sbase4;
				sbase5 = cstep1 * sbase4 - sbase3;
				sbase6 = cstep1 * sbase5 - sbase4;
				sbase7 = cstep1 * sbase6 - sbase5;
				sbase8 = cstep1 * sbase7 - sbase6;

				S16TOD64(w_sbase2, sbase9, sbase10, sbase3,
				    sbase4);
				S16TOD64(w_sbase3, sbase5, sbase6, sbase7,
				    sbase8);

				sbase9 = cstep8 * sbase9 - sbase1;
				sbase10 = cstep8 * sbase10 - sbase2;
				sbase9 = cstep16 * sbase9 - sbase1;
				sbase10 = cstep16 * sbase10 - sbase2;
				sbase9 = cstep32 * sbase9 - sbase1;
				sbase10 = cstep32 * sbase10 - sbase2;
				sbase9 = cstep64 * sbase9 - sbase1;
				sbase10 = cstep64 * sbase10 - sbase2;
				sbase1 = cstep128 * sbase9 - sbase1;
				sbase2 = cstep128 * sbase10 - sbase2;
				sbase3 = cstep1 * sbase2 - sbase1;
				sbase4 = cstep1 * sbase3 - sbase2;
				sbase5 = cstep1 * sbase4 - sbase3;
				sbase6 = cstep1 * sbase5 - sbase4;
				sbase7 = cstep1 * sbase6 - sbase5;
				sbase8 = cstep1 * sbase7 - sbase6;

#pragma pipeloop(0)
				for (i = 0; i < (256 >> 3); i++) {
					mlib_d64 w_snext3, w_snext4;

					((mlib_d64 *)sine)[2 * i] = w_sbase0;
					((mlib_d64 *)sine)[2 * i + 1] =
					    w_sbase1;
					w_snext0 =
					    vis_fmul8sux16(w_sbase2, w_cstep8);
					w_snext1 =
					    vis_fmul8ulx16(w_sbase2, w_cstep8);
					w_snext0 =
					    vis_fpadd16(w_snext0, w_snext1);
					w_snext1 =
					    vis_fpsub16(w_snext0, w_sbase0);
					w_snext3 =
					    vis_fpadd16(w_snext0, w_snext1);

					w_snext0 =
					    vis_fmul8sux16(w_sbase3, w_cstep8);
					w_snext1 =
					    vis_fmul8ulx16(w_sbase3, w_cstep8);
					w_snext0 =
					    vis_fpadd16(w_snext0, w_snext1);
					w_snext1 =
					    vis_fpsub16(w_snext0, w_sbase1);
					w_snext4 =
					    vis_fpadd16(w_snext0, w_snext1);
					w_sbase0 = w_sbase2;
					w_sbase1 = w_sbase3;
					w_sbase2 = w_snext3;
					w_sbase3 = w_snext4;
				}

				sine += 256;
				n -= 256;
			}
		}
#pragma pipeloop(0)
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
			    (((mlib_s32 *)tmp2)[0] << 16) | (((mlib_s32 *)
			    tmp2)[1] & 0xFFFF);
			((mlib_s32 *)sine)[2 * i + 1] =
			    (((mlib_s32 *)tmp2)[2] << 16) | (((mlib_s32 *)
			    tmp2)[3] & 0xFFFF);
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

/* *********************************************************** */

mlib_status
__mlib_SignalSineWaveInit_S16(
    void **state,
    mlib_s16 mag,
    mlib_f32 freq,
    mlib_f32 phase)
{
	mlib_sinewave_s16_struct *sist;
	mlib_d64 dfreq = freq, dphase = phase, dmag = mag, dcos;
	mlib_s32 i_cstep;

/* check for obvious errors */

	if (state == NULL)
		return (MLIB_FAILURE);

	sist = __mlib_malloc(sizeof (mlib_sinewave_s16_struct));

	if (sist == NULL)
		return (MLIB_NULLPOINTER);

	if (mag == -32768)
		dmag = -32767.99;

	sist->type = MLIB_SINE_S16;
	dcos = mlib_cos(dfreq);
	sist->cstep1 = 2. * dcos;
	dcos = (2. * dcos) * dcos - 1;
	dcos = (2. * dcos) * dcos - 1;
	sist->cstep = 2. * dcos;
	dcos = (2. * dcos) * dcos - 1;
	sist->cstep8 = 2. * dcos;
	dcos = (2. * dcos) * dcos - 1;
	sist->cstep16 = 2. * dcos;
	dcos = (2. * dcos) * dcos - 1;
	sist->cstep32 = 2. * dcos;
	dcos = (2. * dcos) * dcos - 1;
	sist->cstep64 = 2. * dcos;
	dcos = (2. * dcos) * dcos - 1;
	sist->cstep128 = 2. * dcos;
	sist->sbase1 = dmag * mlib_sin(dphase);
	sist->sbase2 = dmag * mlib_sin(dphase + dfreq);
	sist->sbase3 = sist->cstep1 * sist->sbase2 - sist->sbase1;
	sist->sbase4 = sist->cstep1 * sist->sbase3 - sist->sbase2;
	sist->sbase5 = sist->cstep1 * sist->sbase4 - sist->sbase3;
	sist->sbase6 = sist->cstep1 * sist->sbase5 - sist->sbase4;
	sist->sbase7 = sist->cstep1 * sist->sbase6 - sist->sbase5;
	sist->sbase8 = sist->cstep1 * sist->sbase7 - sist->sbase6;
	sist->flag = (dmag <= 32500.) && (dmag >= -32500.) &&
	    (sist->cstep8 <= 0.999) && (sist->cstep8 >= -0.999);

	FLOAT2INT_CLAMP(i_cstep, (sist->cstep8 * 2147483648.) + 32768);
	sist->w_cstep8 = vis_to_double_dup(S16TOS32(i_cstep, i_cstep));

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
/* MLIB_FAILURE */
		return;

	sist->type = 0;
	__mlib_free(state);

/* MLIB_SUCCESS */
}

/* *********************************************************** */
