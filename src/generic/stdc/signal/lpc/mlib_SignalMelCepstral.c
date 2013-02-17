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

#pragma ident	"@(#)mlib_SignalMelCepstral.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMelCepstral_S16  - cepstral analysis in mel
 *                                    frequency scale
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMelCepstralInit_S16(void     **state,
 *                                                 mlib_s32 nlinear,
 *                                                 mlib_s32 nmel,
 *                                                 mlib_f32 melbgn,
 *                                                 mlib_f32 melend,
 *                                                 mlib_f32 meldiv,
 *                                                 mlib_s32 order);
 *      mlib_status mlib_SignalMelCepstral_S16(mlib_s16       *cepst,
 *                                             mlib_s32       cscale,
 *                                             const mlib_s16 *signal,
 *                                             void           *state);
 *      mlib_status mlib_SignalMelCepstral_S16_Adp(mlib_s16       *cepst,
 *                                                 mlib_s32       *cscale,
 *                                                 const mlib_s16 *signal,
 *                                                 void           *state);
 *      void mlib_SignalMelCepstralFree_S16(void *state);
 *
 * ARGUMENTS
 *      cepst    the cepstral coefficients, where its length = nlinear + nmel.
 *      cscale   the scaling factor of the cepstral coefficients,
 *               where actual_data = output_data * 2^(-scaling_factor).
 *      signal   the input signal vector, the signal samples
 *               are in Q15 format, and its length = 2^order.
 *      nlinear  the number of band pass filters in linear frequency scale.
 *      nmel     the number of band pass filters in mel frequency scale.
 *      melbgn   the begin radian frequency of the mel scale filter
 *               bank defined in linear frequency scale, where
 *               0 <= melbgn < melend <= PI, melbgn is ignored if nlinear = 0.
 *      melend   the end radian frequency of the mel scale filter
 *               bank defined in linear frequency scale, where
 *               0 <= melbgn < melend <= PI.
 *      meldiv   the dividing factor in linear to mel scale conversion,
 *               linear scale is measured in radians, with PI corresponding
 *               to half the sampling rate.
 *      order    the order of the input signal vector where length = 2^order.
 *      state    the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the cepstral analysis is successful,
 *      MLIB_FAILURE is returned when error is encountered.
 *
 * RESTRICTIONS
 *      The init functions should only perform internal structure
 *      allocation and global initialization. Per function call
 *      initialization should be done in function, so the
 *      same internal structure can be reused for multiple
 *      function calls.
 *
 * DESCRIPTION
 *      The first two steps of mel scale cepstral analysis is the
 *      same as in general cepstral analysis. After the logarithm of
 *      the spectrum magnitude is obtained, it is converted into mel
 *      frequency scale before the inverse Fourier transform.
 *
 *                  +-----------+        +-----------+
 *                  |  Linear   |        |  Inverse  |
 *       ... ------>|    to     |------->|  Fourier  |----->
 *            X'(k) | Mel Scale | X''(m) | Transform | c(n)
 *                  +-----------+        +-----------+
 *
 *      where X'(k) is defined in linear frequency scale and X''(m)
 *      is defined in mel frequency scale.
 *
 *      The mel frequency scale is defined as following.
 *
 *        freq_mel = melmul * LOG10(1 + freq_linear / meldiv)
 *
 *      where freq_mel is the frequency in mel scale, freq_linear
 *      is the frequency in linear scale, melmul is the multiplying
 *      factor, muldiv is the dividing factor.
 *
 *      Optionally, a bank of band pass filters in linear frequency
 *      scale can be used below the bank of band pass filters in
 *      mel frequency scale, as shown below in linear frequency scale.
 *
 *        0   f1  f2  f3    fp  fp+1 fp+2  fp+3  fp+q
 *        |---|---|---| ... |---|----|-----| ... | ... -> freq
 *
 *      where fp = melbgn, fp+q = melend, p = nlinear, q = nmel;
 *      the filters number 1 to p are defined in linear frequency
 *      scale which have equal bandwidth in linear frequency scale;
 *      the filters number p+1 to p+q are defined in mel frequency
 *      scale which have equal bandwidth in mel frequency scale and
 *      increasing bandwidth in linear frequency scale.
 *
 *      See "Digital Signal Processing" by Alan V. Oppenheim and
 *      Ronald W. Schafer, Prentice Hall, 1974.
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 */

#include <mlib_SignalCepstral.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMelCepstralFree_S16 = \
	__mlib_SignalMelCepstralFree_S16
#pragma weak mlib_SignalMelCepstralInit_S16 = \
	__mlib_SignalMelCepstralInit_S16
#pragma weak mlib_SignalMelCepstral_S16 = __mlib_SignalMelCepstral_S16
#pragma weak mlib_SignalMelCepstral_S16_Adp = \
	__mlib_SignalMelCepstral_S16_Adp
#pragma weak mlib_SignalMelCepstral_S16_Middle = \
	__mlib_SignalMelCepstral_S16_Middle

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMelCepstralFree_S16) mlib_SignalMelCepstralFree_S16
    __attribute__((weak, alias("__mlib_SignalMelCepstralFree_S16")));
__typeof__(__mlib_SignalMelCepstralInit_S16) mlib_SignalMelCepstralInit_S16
    __attribute__((weak, alias("__mlib_SignalMelCepstralInit_S16")));
__typeof__(__mlib_SignalMelCepstral_S16) mlib_SignalMelCepstral_S16
    __attribute__((weak, alias("__mlib_SignalMelCepstral_S16")));
__typeof__(__mlib_SignalMelCepstral_S16_Middle)
    mlib_SignalMelCepstral_S16_Middle
    __attribute__((weak, alias("__mlib_SignalMelCepstral_S16_Middle")));
__typeof__(__mlib_SignalMelCepstral_S16_Adp) mlib_SignalMelCepstral_S16_Adp
    __attribute__((weak, alias("__mlib_SignalMelCepstral_S16_Adp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	THRESH	0.0000001

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	D64_TO_S16(x)	(((mlib_s32)(x*dct_scale+32768)) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	D64_TO_S16(x)                                                     \
	(((mlib_s32)(((x * dct_scale + 32768) >=                          \
	    MLIB_S32_MAX) ? MLIB_S32_MAX : (((x * dct_scale + 32768) <=   \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (x * dct_scale +               \
	    32768)))) >> 16)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + (shift)) << 20

#else /* _LITTLE_ENDIAN */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + (shift)) << 20

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
__mlib_SignalMelCepstral_S16_Middle1(
	void *state,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_melcepstral *str_cepst = state;
	deal_t *buffer = str_cepst->buffer;
	deal_t *buffer1 = str_cepst->buffer1;
	deal_t tmp;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		tmp = buffer[2 * i + 0] * buffer[2 * i + 0];
		tmp += buffer[2 * i + 1] * buffer[2 * i + 1];
		buffer1[i] = mlib_sqrt(tmp);
	}
}

void
__mlib_SignalMelCepstral_S16_Middle2(
	void *state,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_melcepstral *str_cepst = state;
	deal_t *filt_coeff = str_cepst->filt_coeff;
	deal_t *buffer = str_cepst->buffer1;
	deal_t *dct_in = str_cepst->dct_in;
	deal_t *dct_out = str_cepst->dct_out;
	mlib_s32 *index = str_cepst->index;
	mlib_s32 *index_coeff = str_cepst->index_coeff;
	deal_t sum;
	mlib_s32 len, begp;
	mlib_s32 i, j;

	filt_coeff += index_coeff[i0];
	index += 2 * i0;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		sum = 0;
		begp = index[0];
		len = index[1];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = begp; j < begp + len; j++) {
			sum += buffer[j] * filt_coeff[0];
			filt_coeff++;
		}

		sum = (sum > THRESH) ? sum : THRESH;
		dct_out[i] = sum;
		index += 2;
	}

	mlib_vlog(dct_in + i0, dct_out + i0, i1 - i0);
}

void
__mlib_SignalMelCepstral_S16_Middle(
	void *state,
	mlib_s32 nfilters,
	mlib_s32 len_full)
{
	mlib_melcepstral *str_cepst = state;
	deal_t *filt_coeff = str_cepst->filt_coeff;
	deal_t *buffer = str_cepst->buffer;
	deal_t *dct_in = str_cepst->dct_in;
	deal_t *dct_out = str_cepst->dct_out;
	mlib_s32 *index = str_cepst->index;
	deal_t sum, tmp;
	mlib_s32 len, begp;
	mlib_s32 i, j;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (len_full >> 1); i++) {
		tmp = buffer[2 * i + 0] * buffer[2 * i + 0];
		tmp += buffer[2 * i + 1] * buffer[2 * i + 1];
		buffer[i] = mlib_sqrt(tmp);
	}

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < nfilters; i++) {
		sum = 0;
		begp = index[0];
		len = index[1];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = begp; j < begp + len; j++) {
			sum += buffer[j] * filt_coeff[0];
			filt_coeff++;
		}

		sum = (sum > THRESH) ? sum : THRESH;
		dct_out[i] = sum;
		index += 2;
	}

	mlib_vlog(dct_in, dct_out, nfilters);
}


/* *********************************************************** */

mlib_status
__mlib_SignalMelCepstralInit_S16(
    void **state,
    mlib_s32 nlinear,
    mlib_s32 nmel,
    mlib_f32 melbgn,
    mlib_f32 melend,
    mlib_f32 meldiv,
    mlib_s32 order)
{
	return __mlib_SignalMelCepstralInit_F32(state, nlinear, nmel, melbgn,
	    melend, meldiv, order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMelCepstral_S16(
    mlib_s16 *cepst,
    mlib_s32 cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_melcepstral *str_cepst = state;
	deal_t *buffer = str_cepst->buffer;
	deal_t *dct_in = str_cepst->dct_in;
	mlib_s32 order = str_cepst->order;
	mlib_s32 dct_order = str_cepst->dct_order;
	mlib_s32 nfilters = str_cepst->nfilters;
	deal_t dct_coeff = str_cepst->dct_coeff;
	deal_t dct_scale = str_cepst->dct_scale;
	deal_t sum, tmp, y0, y1, y2, c;
	deal_t sum0, sum1, x0_0, x1_0, x2_0, x0_1, x1_1, x2_1;
	mlib_s32 len_full;
	mlib_s32 i, j;
	mlib_d64 dscale;

	len_full = 1 << order;

	tmp = ((deal_t)1.0) / 32768;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		buffer[i] = tmp * signal[i];
	mlib_fft_r2c(buffer, order);

	mlib_SignalMelCepstral_S16_Middle(state, nfilters, len_full);

	SCALBN(dscale, cscale + 16);

	dct_scale *= dscale;

	if (dct_order == -1) {

		sum = dct_in[0];
#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 1; j < nfilters; j++)
			sum += dct_in[j];

/* sum*sqrt(0.5) */
		cepst[0] = D64_TO_S16(sum * 0.70710678118654752440);

/* cos(pi/(2*nfilters)) */
		y2 = dct_coeff;
		y1 = 1;
/* 2*cos(pi/(2*nfilters)) */
		dct_coeff *= 2;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 1; i < nfilters; i++) {
/* cos((pi*i)/(2*nfilters)) */
			x1_0 = y2;
/* cos((pi*i)/(nfilters)) */
			c = 2 * x1_0 * x1_0 - 1;
/* cos((3*pi*i)/(2*nfilters)) */
			x1_1 = (2 * c - 1) * x1_0;
/* 2*cos((2*pi*i)/(nfilters)) */
			c = 4 * c * c - 2;
/* cos((5*pi*i)/(2*nfilters)) */
			x2_0 = c * x1_0 - x1_1;
/* cos((7*pi*i)/(2*nfilters)) */
			x2_1 = c * x1_1 - x1_0;
			sum0 = x1_0 * dct_in[0];
			sum1 = x1_1 * dct_in[1];
#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 2; j < nfilters; j += 2) {
				sum0 += x2_0 * dct_in[j];
				x0_0 = x1_0;
				x1_0 = x2_0;
/* cos((pi*i*(2*(j+2)+1))/(2*nfilters)) */
				x2_0 = c * x1_0 - x0_0;

				sum1 += x2_1 * dct_in[j + 1];
				x0_1 = x1_1;
				x1_1 = x2_1;
/* cos((pi*i*(2*(j+3)+1))/(2*nfilters)) */
				x2_1 = c * x1_1 - x0_1;
			}

			sum = sum0 + sum1;
			cepst[i] = D64_TO_S16(sum);
			y0 = y1;
			y1 = y2;
/* cos((pi*(i+1))/(2*nfilters)) */
			y2 = dct_coeff * y1 - y0;
		}
	} else {
		mlib_fft_r2r2(dct_in, dct_order);
		for (i = 1; i < nfilters; i++)
			cepst[i] = D64_TO_S16(dct_in[i]);

/* dct_in[0]*sqrt(0.5) */
		cepst[0] = D64_TO_S16(dct_in[0] * 0.70710678118654752440);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMelCepstral_S16_Adp(
    mlib_s16 *cepst,
    mlib_s32 *cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_melcepstral *str_cepst = state;
	deal_t *buffer = str_cepst->buffer;
	deal_t *dct_in = str_cepst->dct_in;
	deal_t *dct_out = str_cepst->dct_out;
	mlib_s32 order = str_cepst->order;
	mlib_s32 dct_order = str_cepst->dct_order;
	mlib_s32 nfilters = str_cepst->nfilters;
	deal_t dct_coeff = str_cepst->dct_coeff;
	deal_t dct_scale = str_cepst->dct_scale;
	deal_t sum, tmp, y0, y1, y2, c;
	deal_t sum0, sum1, x0_0, x1_0, x2_0, x0_1, x1_1, x2_1;
	mlib_s32 len_full;
	mlib_s32 i, j, iscale;

	len_full = 1 << order;

	tmp = ((deal_t)1.0) / 32768;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		buffer[i] = tmp * signal[i];

	mlib_fft_r2c(buffer, order);

	mlib_SignalMelCepstral_S16_Middle(state, nfilters, len_full);

	if (dct_order == -1) {

		sum = dct_in[0];
#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 1; j < nfilters; j++)
			sum += dct_in[j];

/* *sum*sqrt(0.5) */
		dct_out[0] = dct_scale * sum * 0.70710678118654752440;

/* cos(pi/(2*nfilters)) */
		y2 = dct_coeff;
		y1 = 1;
/* 2*cos(pi/(2*nfilters)) */
		dct_coeff *= 2;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 1; i < nfilters; i++) {
/* cos((pi*i)/(2*nfilters)) */
			x1_0 = y2;
/* cos((pi*i)/(nfilters)) */
			c = 2 * x1_0 * x1_0 - 1;
/* cos((3*pi*i)/(2*nfilters)) */
			x1_1 = (2 * c - 1) * x1_0;
/* 2*cos((2*pi*i)/(nfilters)) */
			c = 4 * c * c - 2;
/* cos((5*pi*i)/(2*nfilters)) */
			x2_0 = c * x1_0 - x1_1;
/* cos((7*pi*i)/(2*nfilters)) */
			x2_1 = c * x1_1 - x1_0;
			sum0 = x1_0 * dct_in[0];
			sum1 = x1_1 * dct_in[1];
#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 2; j < nfilters; j += 2) {
				sum0 += x2_0 * dct_in[j];
				x0_0 = x1_0;
				x1_0 = x2_0;
/* cos((pi*i*(2*(j+2)+1))/(2*nfilters)) */
				x2_0 = c * x1_0 - x0_0;

				sum1 += x2_1 * dct_in[j + 1];
				x0_1 = x1_1;
				x1_1 = x2_1;
/* cos((pi*i*(2*(j+3)+1))/(2*nfilters)) */
				x2_1 = c * x1_1 - x0_1;
			}

			sum = sum0 + sum1;
			dct_out[i] = dct_scale * sum;
			y0 = y1;
			y1 = y2;
/* cos((pi*(i+1))/(2*nfilters)) */
			y2 = dct_coeff * y1 - y0;
		}
	} else {
		mlib_fft_r2r2(dct_in, dct_order);
		for (i = 0; i < nfilters; i++)
			dct_out[i] = dct_scale * dct_in[i];

/* sqrt(0.5) */
		dct_out[0] *= 0.70710678118654752440;
	}

	tmp = mlib_fabs(dct_out[0]);
	for (i = 1; i < nfilters; i++)
		tmp =
		    (tmp > mlib_fabs(dct_out[i])) ? tmp : mlib_fabs(dct_out[i]);

	iscale = 14 - mlib_ilogb(tmp);
	SCALBN(dct_scale, 16 + iscale);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < nfilters; i++) {
		cepst[i] = D64_TO_S16(dct_out[i]);
	}

	*cscale = iscale;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalMelCepstralFree_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
