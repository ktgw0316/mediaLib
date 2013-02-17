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

#pragma ident	"@(#)mlib_SignalMelCepstral_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalMelCepstral_F32  - cepstral analysis in mel
 *                                  frequency scale
 *
 * SYNOPSIS
 *     mlib_status mlib_SignalMelCepstralInit_F32(void **state,
 *                                                mlib_s32 nlinear,
 *                                                mlib_s32 nmel,
 *                                                mlib_f32 melbgn,
 *                                                mlib_f32 melend,
 *                                                mlib_f32 meldiv,
 *                                                mlib_s32 order);
 *     mlib_status mlib_SignalMelCepstral_F32(mlib_f32 *cepst,
 *                                            const mlib_f32 *signal,
 *                                            void *state);
 *     mlib_status mlib_SignalMelCepstralFree_F32(void *state);
 *
 * ARGUMENTS
 *    cepst    the cepstral coefficients, where its length = nlinear + nmel.
 *    signal   the input signal vector, where its length = 2^order.
 *    nlinear  the number of band pass filters in linear frequency scale.
 *    nmel     the number of band pass filters in mel frequency scale.
 *    melbgn   the begin radian frequency of the mel scale filter
 *             bank defined in linear frequency scale, where
 *             0 <= melbgn < melend <= PI, melbgn is ignored if nlinear = 0.
 *    melend   the end radian frequency of the mel scale filter
 *             bank defined in linear frequency scale, where
 *             0 <= melbgn < melend <= PI.
 *    meldiv   the dividing factor in linear to mel scale conversion,
 *             linear scale is measured in radians, with PI corresponding
 *             to half the sampling rate.
 *    order    the order of the input signal vector where length = 2^order.
 *    state    the internal state structure.
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
 *                   +-----------+        +-----------+
 *                   |  Linear   |        |  Inverse  |
 *        ... ------>|    to     |------->|  Fourier  |----->
 *             X'(k) | Mel Scale | X''(m) | Transform | c(n)
 *                   +-----------+        +-----------+
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

#include <mlib_signal.h>
#include <mlib_SignalCepstral.h>
#include <mlib_SignalWindowUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMelCepstralFree_F32 = \
	__mlib_SignalMelCepstralFree_F32
#pragma weak mlib_SignalMelCepstralInit_F32 = \
	__mlib_SignalMelCepstralInit_F32
#pragma weak mlib_SignalMelCepstral_F32 = __mlib_SignalMelCepstral_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMelCepstralFree_F32) mlib_SignalMelCepstralFree_F32
    __attribute__((weak, alias("__mlib_SignalMelCepstralFree_F32")));
__typeof__(__mlib_SignalMelCepstralInit_F32) mlib_SignalMelCepstralInit_F32
    __attribute__((weak, alias("__mlib_SignalMelCepstralInit_F32")));
__typeof__(__mlib_SignalMelCepstral_F32) mlib_SignalMelCepstral_F32
    __attribute__((weak, alias("__mlib_SignalMelCepstral_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	THRESH	0.0000001

/* *********************************************************** */

mlib_status
__mlib_SignalMelCepstralInit_F32(
    void **state,
    mlib_s32 nlinear,
    mlib_s32 nmel,
    mlib_f32 melbgn,
    mlib_f32 melend,
    mlib_f32 meldiv,
    mlib_s32 order)
{
	mlib_melcepstral *str_cepst;
	mlib_u8 *mem;
	deal_t *tmp_ptr;
	mlib_s32 *index, *index_coeff;
	deal_t lin_step, mel_step, scale, scale1, slope0, slope1, tmp;
	deal_t fbegp0, fbegp, fvert0, fvert, fendp, sum;
	deal_t beg, end;
	mlib_s32 begp, vert, endp, itmp;
	mlib_s32 size_all, nfilters, tmp_nlinear;
	mlib_s32 size = (sizeof (mlib_melcepstral) + 7) & ~7;
	mlib_s32 len_buf, h_lenm1, count, count_lin, count_lin1, count_mel;
	mlib_s32 shift, dct_size;
	mlib_s32 i, j, fcind;

	if (order < 1)
		return (MLIB_FAILURE);

	if ((nlinear <= 0) && (nmel <= 0))
		return (MLIB_FAILURE);

	if (nlinear < 0)
		nlinear = 0;

	if (nmel < 0)
		nmel = 0;

	if ((nmel > 0) && (meldiv <= 0))
		return (MLIB_FAILURE);

	if (nlinear == 0)
		melbgn = 0;

	if ((melbgn < 0) || (melend <= melbgn) || (melend > M_PI))
		return (MLIB_FAILURE);

	len_buf = 1 << order;
	scale = len_buf / (2 * M_PI);
	nfilters = nlinear + nmel;
	h_lenm1 = (len_buf >> 1) - 1;

#if !REAL_FFT_NEEDWASTE
	len_buf = len_buf + 2;
	dct_size = nfilters + 2;
#else /* !REAL_FFT_NEEDWASTE */
	dct_size = 2 * nfilters;
	len_buf *= 2;
#endif /* !REAL_FFT_NEEDWASTE */

	tmp_nlinear = nlinear;

	if (nmel > 0)
		tmp_nlinear--;

	fbegp = (deal_t)(-0.5);
	count_lin = 0;
	count_lin1 = 0;
	count_mel = 0;

	if (tmp_nlinear > 0) {
		lin_step = melbgn * scale / (tmp_nlinear + 1);
		fendp = fbegp + lin_step;
		begp = 0;
		vert = (mlib_s32)(fendp + 1) - 1;

		for (i = 0; i < tmp_nlinear; i++) {
			fbegp = fendp;
			fendp += lin_step;
			endp = (mlib_s32)(fendp + 1) - 1;
			itmp = endp - begp + 1;

			if (itmp < 1)
				itmp = 1;
			count_lin += itmp;
			begp = vert + 1;
			vert = endp;
		}
	}

	fbegp0 = fbegp;

	if (nmel > 0) {
/* mel scale filter bank exist */

		scale1 = meldiv * scale + (deal_t)(0.5);

		tmp = 1 + melbgn / meldiv;
		mlib_vlog(&beg, &tmp, 1);
		tmp = 1 + melend / meldiv;
		mlib_vlog(&end, &tmp, 1);
		mel_step = mlib_exp((end - beg) / (nmel + 1), &shift);
		tmp = (1 << 30);
		while (shift > 30) {
			mel_step *= tmp;
			shift -= 30;
		}

		mel_step *= (1 << shift);

		tmp = (meldiv + melbgn) * mel_step * scale;

		fbegp = melbgn * scale - (deal_t)(0.5);
		fvert0 = fvert = tmp - scale1;
		tmp *= mel_step;
		begp = (mlib_s32)(fbegp + 1);
		vert = (mlib_s32)(fvert + 1) - 1;

		for (i = nlinear; i < nlinear + nmel; i++) {
			fendp = tmp - scale1;
			tmp *= mel_step;
			endp = (mlib_s32)(fendp + 1) - 1;
			itmp = endp - begp + 1;

			if (itmp < 1)
				itmp = 1;
			count_mel += itmp;
			begp = vert + 1;
			vert = endp;
		}

		if (nlinear != 0) {
			begp = (mlib_s32)(fbegp0 + 1);
			endp = (mlib_s32)(fvert0 + 1) - 1;
			itmp = endp - begp + 1;

			if (itmp < 1)
				itmp = 1;
			count_lin1 = itmp;
		}
	}

	count = count_lin + count_mel + count_lin1;

	size_all = size +
	    (count + 2 * (dct_size + 1) + len_buf * 2) * sizeof (deal_t) +
	    (3 * nfilters + 1) * sizeof (mlib_s32);

	mem = __mlib_malloc(size_all);

	if (mem == NULL)
		return (MLIB_FAILURE);

	str_cepst = *state = (void *)mem;
	str_cepst->order = order;
	str_cepst->dct_order = -1;
	str_cepst->nfilters = nfilters;
	str_cepst->dct_coeff = (deal_t)mlib_cos(M_PI / (2 * nfilters));
	str_cepst->dct_scale = (deal_t)mlib_sqrt(2.0 / nfilters);
	str_cepst->filt_coeff = (deal_t *)(mem + size);
	str_cepst->buffer = str_cepst->filt_coeff + count;
	str_cepst->buffer1 = str_cepst->buffer + len_buf;
	str_cepst->dct_in = str_cepst->buffer1 + len_buf;
	str_cepst->dct_out = str_cepst->dct_in + dct_size + 1;
	str_cepst->index = (mlib_s32 *)(str_cepst->dct_out + dct_size + 1);
	str_cepst->index_coeff = str_cepst->index + 2 * nfilters;
	fcind = 0;
	str_cepst->dct_in[nfilters] = 0;

	fbegp = (deal_t)(-0.5);
	index = str_cepst->index;
	index_coeff = str_cepst->index_coeff;

	if (tmp_nlinear > 0) {
		tmp_ptr = str_cepst->filt_coeff;
		slope0 = 1 / lin_step;
		fvert = fbegp + lin_step;
		begp = 0;
		vert = (mlib_s32)(fvert + 1) - 1;

		for (i = 0; i < tmp_nlinear; i++) {
			fendp = fvert + lin_step;
			endp = (mlib_s32)(fendp + 1) - 1;

			if ((endp - begp) < 1) {
				if (endp == begp) {
					*tmp_ptr = 1;
					index[2 * i] = endp;
					index[2 * i + 1] = 1;
					index_coeff[i] = fcind;
					fcind += 1;
					tmp_ptr++;
				} else {
					itmp = (mlib_s32)(fvert + (deal_t)0.5);

					if (itmp > h_lenm1)
						itmp = h_lenm1;
					*tmp_ptr = 1;
					index[2 * i] = itmp;
					index[2 * i + 1] = 1;
					index_coeff[i] = fcind;
					fcind += 1;
					tmp_ptr++;
				}
			} else {
				sum = 0;
				beg = (begp - fbegp) * slope0;
				for (j = 0; j <= vert - begp; j++) {
					tmp_ptr[j] = beg;
					sum += beg;
					beg += slope0;
				}

				beg = (fendp - endp) * slope0;
				for (j = endp - begp; j > vert - begp; j--) {
					tmp_ptr[j] = beg;
					sum += beg;
					beg += slope0;
				}

				sum = 1 / sum;
				for (j = begp; j <= endp; j++) {
					tmp_ptr[0] *= sum;
					tmp_ptr++;
				}

				index[2 * i] = begp;
				index[2 * i + 1] = endp - begp + 1;
				index_coeff[i] = fcind;
				fcind += endp - begp + 1;
			}

			fbegp = fvert;
			fvert = fendp;
			begp = vert + 1;
			vert = endp;
		}
	}

	fbegp0 = fbegp;

	if (nmel > 0) {
		tmp = (meldiv + melbgn) * mel_step * scale;
		fvert0 = fvert = tmp - scale1;

/* mel scale filter bank exist */
		if (nlinear != 0) {
			tmp_ptr = str_cepst->filt_coeff + count_lin;
			fbegp = fbegp0;
			fendp = fvert0;
			fvert = 0.5 * (fbegp + fendp);
			begp = (mlib_s32)(fbegp + 1);
			vert = (mlib_s32)(fvert + 1) - 1;
			endp = (mlib_s32)(fendp + 1) - 1;
			i = nlinear - 1;

			if ((endp - begp) < 1) {
				if (endp == begp) {
					*tmp_ptr = 1;
					index[2 * i] = endp;
					index[2 * i + 1] = 1;
					index_coeff[i] = fcind;
					fcind += 1;
					tmp_ptr++;
				} else {
					itmp = (mlib_s32)(fvert + (deal_t)0.5);

					if (itmp > h_lenm1)
						itmp = h_lenm1;
					*tmp_ptr = 1;
					index[2 * i] = itmp;
					index[2 * i + 1] = 1;
					index_coeff[i] = fcind;
					fcind += 1;
					tmp_ptr++;
				}
			} else {
				slope0 = 1 / (fvert - fbegp);
				sum = 0;
				beg = (begp - fbegp) * slope0;
				for (j = 0; j <= vert - begp; j++) {
					tmp_ptr[j] = beg;
					sum += beg;
					beg += slope0;
				}

				beg = (fendp - endp) * slope0;
				for (j = endp - begp; j > vert - begp; j--) {
					tmp_ptr[j] = beg;
					sum += beg;
					beg += slope0;
				}

				sum = 1 / sum;
				for (j = begp; j <= endp; j++) {
					tmp_ptr[0] *= sum;
					tmp_ptr++;
				}

				index[2 * i] = begp;
				index[2 * i + 1] = endp - begp + 1;
				index_coeff[i] = fcind;
				fcind += endp - begp + 1;
			}
		}

		tmp_ptr = str_cepst->filt_coeff + count_lin + count_lin1;

		fbegp = melbgn * scale - (deal_t)(0.5);
		fvert0 = fvert = tmp - scale1;
		tmp *= mel_step;
		begp = (mlib_s32)(fbegp + 1);
		vert = (mlib_s32)(fvert + 1) - 1;
		slope0 = 1 / (fvert - fbegp);

		for (i = nlinear; i < nlinear + nmel; i++) {
			fendp = tmp - scale1;
			tmp *= mel_step;
			endp = (mlib_s32)(fendp + 1) - 1;
			slope1 = 1 / (fendp - fvert);

			if ((endp - begp) < 1) {
				if (endp == begp) {
					*tmp_ptr = 1;
					index[2 * i] = endp;
					index[2 * i + 1] = 1;
					index_coeff[i] = fcind;
					fcind += 1;
					tmp_ptr++;
				} else {
					itmp = (mlib_s32)(fvert + (deal_t)0.5);

					if (itmp > h_lenm1)
						itmp = h_lenm1;
					*tmp_ptr = 1;
					index[2 * i] = itmp;
					index[2 * i + 1] = 1;
					index_coeff[i] = fcind;
					fcind += 1;
					tmp_ptr++;
				}
			} else {
				sum = 0;
				beg = (begp - fbegp) * slope0;
				for (j = 0; j <= vert - begp; j++) {
					tmp_ptr[j] = beg;
					sum += beg;
					beg += slope0;
				}

				beg = (fendp - endp) * slope1;
				for (j = endp - begp; j > vert - begp; j--) {
					tmp_ptr[j] = beg;
					sum += beg;
					beg += slope1;
				}

				sum = 1 / sum;
				for (j = begp; j <= endp; j++) {
					tmp_ptr[0] *= sum;
					tmp_ptr++;
				}

				index[2 * i] = begp;
				index[2 * i + 1] = endp - begp + 1;
				index_coeff[i] = fcind;
				fcind += endp - begp + 1;
			}

			fbegp = fvert;
			fvert = fendp;
			begp = vert + 1;
			vert = endp;
			slope0 = slope1;
		}
	}

	if ((nfilters & (nfilters - 1)) == 0) {
		mlib_f32 y = (mlib_f32)nfilters;
		mlib_s32 z = *((mlib_s32 *)&y);
		mlib_s32 exp = (z >> 23) & 0xff;

		str_cepst->dct_order =
		    ((exp - 127) & ((nfilters | (-nfilters)) >> 31));
	}

	index_coeff[nfilters] = fcind;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMelCepstral_F32(
    mlib_f32 *cepst,
    const mlib_f32 *signal,
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
	deal_t sum, y0, y1, y2, c;
	deal_t sum0, sum1, x0_0, x1_0, x2_0, x0_1, x1_1, x2_1;
	mlib_s32 len_full;
	mlib_s32 i, j;

	len_full = 1 << order;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		buffer[i] = signal[i];

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

/* sqrt(0.5) */
		cepst[0] = dct_scale * sum * 0.70710678118654752440;

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
			cepst[i] = dct_scale * sum;
			y0 = y1;
			y1 = y2;
/* cos((pi*(i+1))/(2*nfilters)) */
			y2 = dct_coeff * y1 - y0;
		}
	} else {
		mlib_fft_r2r2(dct_in, dct_order);
		for (i = 0; i < nfilters; i++)
			cepst[i] = dct_scale * dct_in[i];

/* sqrt(0.5) */
		cepst[0] *= 0.70710678118654752440;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalMelCepstralFree_F32(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
