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

#pragma ident	"@(#)mlib_SignalADPCM2Bits.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *   Function mlib_SignalADPCM2Bits2Linear() and mlib_SignalLinear2ADPCM2Bits()
 *   convert data between 16-bit linear PCM format and
 *   G.726 16kbps 2-bit ADPCM format.
 *
 *   mlib_SignalADPCM2Bits2Linear -  convert data to 16-bit PCM format
 *
 *   mlib_SignalLinear2ADPCM2Bits -  convert data to 2-bit ADPCM format
 *
 * SYNOPSIS
 *   mlib_status mlib_SignalADPCM2Bits2Linear(mlib_s16   *pcm,
 *                                            mlib_u8    *adpcm,
 *                                            const void *state,
 *                                            mlib_s32   n)
 *
 *   mlib_status mlib_SignalLinear2ADPCM2Bits(mlib_u8    *adpcm,
 *                                            mlib_s16   *pcm,
 *                                            const void *state,
 *                                            mlib_s32   n)
 *
 * ARGUMENTS
 *   pcm      Linear PCM sample array
 *   adpcm    ADPCM code array
 *   state    Internal encoder(decoder) structure
 *   n        Number of samples in the input array
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalADPCM2Bits2Linear = __mlib_SignalADPCM2Bits2Linear
#pragma weak mlib_SignalLinear2ADPCM2Bits = __mlib_SignalLinear2ADPCM2Bits

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalADPCM2Bits2Linear) mlib_SignalADPCM2Bits2Linear
    __attribute__((weak, alias("__mlib_SignalADPCM2Bits2Linear")));
__typeof__(__mlib_SignalLinear2ADPCM2Bits) mlib_SignalLinear2ADPCM2Bits
    __attribute__((weak, alias("__mlib_SignalLinear2ADPCM2Bits")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* Routine */

/* *********************************************************** */

static mlib_s32 mlib_quan1(
    mlib_s32 val);
static mlib_s32 mlib_fmult(
    mlib_s32 an,
    mlib_s32 srn);
static mlib_s32 mlib_reconstruct(
    mlib_s32 sign,
    mlib_s32 dqln,
    mlib_s32 y);
static mlib_s32 mlib_quantize(
    mlib_s32 d,
    mlib_s32 y);
static mlib_s32 mlib_step_size(
    mlib_G72x_state * state);
static mlib_s32 mlib_predictor_pole(
    mlib_G72x_state * state);
static mlib_s32 mlib_predictor_zero(
    mlib_G72x_state * state);

static void mlib_update(
    mlib_s32 y,
    mlib_s32 wi,
    mlib_s32 fi,
    mlib_s32 dq,
    mlib_s32 sr,
    mlib_s32 dqsez,
    mlib_G72x_state * state);

#ifdef __SUNPRO_C
#pragma inline(mlib_quan1, mlib_fmult, mlib_reconstruct)
#pragma inline(mlib_quantize, mlib_step_size)
#pragma inline(mlib_predictor_pole, mlib_predictor_zero, mlib_update)
#endif /* __SUNPRO_C */

/* *********************************************************** */

static mlib_s32
mlib_quan1(
    mlib_s32 val)
{
	mlib_s32 i;

	i = ((255 - val) >> 31) & 8;
	return (mlib_sgn[val >> i] + i);
}

/* *********************************************************** */

static mlib_s32
mlib_fmult(
    mlib_s32 an,
    mlib_s32 srn)
{
	mlib_s32 anmag, anexp, anmant;
	mlib_s32 wanexp, retval, mask;
	mlib_s32 wanmant;

	anmag = an + ((((-an) & 0x1FFF) - an) & (an >> 31));
	anexp = mlib_quan1(anmag);
	anmant = ((anmag << 6) >> anexp) & 0x1F;
	wanexp = anexp + (srn & 0xF);
	wanmant = mlib_ADPCM_mul[(srn & 0x3E0) | anmant];
	retval = ((wanmant << 9) >> (28 - wanexp)) & 0x7FFF;
	mask = (an ^ srn) >> 31;
	return ((retval ^ mask) - mask);
}

/* *********************************************************** */

static mlib_s32
mlib_predictor_zero(
    mlib_G72x_state * state)
{
	mlib_s32 sezi;

	sezi = mlib_fmult(state->b[0] >> 2, state->dq[0]);
	sezi += mlib_fmult(state->b[1] >> 2, state->dq[1]);
	sezi += mlib_fmult(state->b[2] >> 2, state->dq[2]);
	sezi += mlib_fmult(state->b[3] >> 2, state->dq[3]);
	sezi += mlib_fmult(state->b[4] >> 2, state->dq[4]);
	sezi += mlib_fmult(state->b[5] >> 2, state->dq[5]);
	return (sezi);
}

/* *********************************************************** */

static mlib_s32
mlib_predictor_pole(
    mlib_G72x_state * state)
{
	return (mlib_fmult(state->a[1] >> 2, state->sr[1]) +
	    mlib_fmult(state->a[0] >> 2, state->sr[0]));
}

/* *********************************************************** */

static mlib_s32
mlib_step_size(
    mlib_G72x_state * state)
{
	mlib_s32 y, dif, al, m1, m2;

	y = state->yl >> 6;
	dif = state->yu - y;
	al = state->ap >> 2;
	m1 = (dif - 1) >> 31;
	m2 = dif >> 31;
	y += (((dif * al) & (m2 | ~m1)) + (0x3F & m2)) >> 6;
	m1 = (state->ap - 256) >> 15;
	y = (y & m1) | (state->yu & ~m1);
	return (y);
}

/* *********************************************************** */

static mlib_s32
mlib_quantize(
    mlib_s32 d,
    mlib_s32 y)
{
/* Magnitude of 'd' */
	mlib_s32 dqm;

/* Integer part of base 2 log of 'd' */
	mlib_s32 exp;

/* Fractional part of base 2 log */
	mlib_s32 mant;

/* Log of magnitude of 'd' */
	mlib_s32 dl;

/* Step size scale factor normalized log */
	mlib_s32 dln;
	mlib_s32 i, m;

	dqm = abs(d);
	exp = mlib_quan1(dqm >> 1);
	mant = ((dqm << 7) >> exp) & 0x7F;
	dl = (exp << 7) + mant;

	dln = dl - (y >> 2);

	i = ((260 - dln) >> 31) & 1;

	m = d >> 31;

	return (((3 - i) & m) | (i & ~m));
}

/* *********************************************************** */

static mlib_s32
mlib_reconstruct(
    mlib_s32 sign,
    mlib_s32 dqln,
    mlib_s32 y)
{
/* Log of 'dq' magnitude */
	mlib_s32 dql;

/* Integer part of log */
	mlib_s32 dex;
	mlib_s32 dqt;

/* Reconstructed difference signal sample */
	mlib_s32 dq;

	dql = dqln + (y >> 2);

	dex = (dql >> 7) & 15;
	dqt = 128 + (dql & 127);
	dq = (dqt << 7) >> (14 - dex);

	dq &= ~((dql - 1) >> 15);
	dq -= sign << 14;

	return (dq);
}

/* *********************************************************** */

static void
mlib_update(
    mlib_s32 y,
    mlib_s32 wi,
    mlib_s32 fi,
    mlib_s32 dq,
    mlib_s32 sr,
    mlib_s32 dqsez,
    mlib_G72x_state * state)
{
	mlib_s32 cnt, m1, m2;

/* Adaptive predictor, FLOAT A */
	mlib_s32 mag, exp;

/* LIMC */
	mlib_s32 a2p, fa1;

/* UPA1 */
	mlib_s32 a1ul;

/* UPA2 */
	mlib_s32 pks1;

/* tone/transition detector */
	mlib_s32 tr;
	mlib_s32 ylint, thr2, dqthr, ylfrac, thr1, pk0;
	mlib_s32 yu, yl, statea0, statebcnt, stateap;
	mlib_s32 diff0, diff1, sign0, sign1;

/* needed in updating predictor poles */
	pk0 = (mlib_u32)dqsez >> 31;

/* prediction difference magnitude */
	mag = dq & 0x3FFF;

/* TRANS */
	yl = state->yl;
/* exponent part of yl */
	ylint = yl >> 15;
/* fractional part of yl */
	ylfrac = (yl >> 10) & 0x1F;
/* threshold */
	thr1 = (32 + ylfrac) << ylint;

	cnt = (ylint - 9) >> 31;
/* limit thr2 to 31 << 9 */
	thr2 = ((31 << 9) & ~cnt) | (thr1 & cnt);

/* dqthr = 0.75 * thr2 */
	dqthr = (thr2 + (thr2 >> 1)) >> 1;

	tr = (mlib_u32)(state->td | -state->td) >> 31;
	tr &= (mlib_u32)(dqthr - mag) >> 31;

/* Quantizer scale factor adaptation */

/* FUNCTW & FILTD & DELAY */
/* update non-steady state step size multiplier */
	yu = y + ((wi - y) >> 5);

/* LIMB */
	diff0 = yu - 544;
	diff1 = 5120 - yu;
	sign0 = diff0 >> 31;
	sign1 = diff1 >> 31;
	yu -= diff0 & sign0;
	yu += diff1 & sign1;

/* FILTE & DELAY */
/* update steady state step size multiplier */
	yl += yu + ((-yl) >> 6);
	state->yl = yl;
	state->yu = yu;

/* Adaptive predictor coefficients */

/* reset a's and b's for modem signal */
	if (tr == 1) {
		state->a[0] = state->a[1] =
		    state->b[0] = state->b[1] =
		    state->b[2] = state->b[3] = state->b[4] = state->b[5] = 0;
	} else {
/* update a's and b's */

/* UPA2 */
		pks1 = pk0 ^ state->pk[0];
		statea0 = state->a[0];

/* update predictor pole a[1] */
		a2p = state->a[1] - (state->a[1] >> 7);
		m1 = -pks1;
		m2 = 0;

		if (dqsez != 0) {

			fa1 = ((statea0 ^ (~m1)) - (~m1));

			fa1 >>= 5;
			diff0 = fa1 - (-0x100);
			diff1 = 0xFF - fa1;
			sign0 = diff0 >> 31;
			sign1 = diff1 >> 31;
			fa1 -= diff0 & sign0;
			fa1 += diff1 & sign1;

			a2p += fa1;

			a2p += (128 - ((pk0 ^ state->pk[1]) << 8));

			m2 = ((192 ^ m1) - m1);
		}

/* LIMC */
		diff0 = a2p - (-12288);
		diff1 = 12288 - a2p;
		sign0 = diff0 >> 31;
		sign1 = diff1 >> 31;
		a2p -= diff0 & sign0;
		a2p += diff1 & sign1;

/* TRIGB & DELAY */
		state->a[1] = a2p;

/* UPA1 */
/* update predictor pole a[0] */
		statea0 -= ((statea0 >> 8) - m2);

/* LIMD */
		a1ul = 15360 - a2p;

		diff0 = statea0 - (-a1ul);
		diff1 = a1ul - statea0;
		sign0 = diff0 >> 31;
		sign1 = diff1 >> 31;
		statea0 -= diff0 & sign0;
		statea0 += diff1 & sign1;
		state->a[0] = statea0;

/* UPB : update predictor zeros b[6] */

		for (cnt = 5; cnt >= 0; cnt--) {
			statebcnt = state->b[cnt];
			statebcnt -= statebcnt >> 8;

/* XOR */

			if (mag) {
				m1 = (dq ^ state->dq[cnt]) >> 31;
				m1 = (128 ^ m1) - m1;
				statebcnt += m1;
			}

/* may be overfloat */
			state->b[cnt] = (mlib_s32)(mlib_s16)statebcnt;
		}
	}

	for (cnt = 5; cnt > 0; cnt--)
		state->dq[cnt] = state->dq[cnt - 1];

/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
	exp = mlib_quan1(mag);

	state->dq[0] =
	    (exp + (((mag << 11) >> exp) & 0x3E0)) - ((dq >> 31) & 0x800);
	state->sr[1] = state->sr[0];

/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
	m1 = sr >> 31;
	mag = (sr ^ m1) - m1;
	exp = mlib_quan1((mag & 0x7FFF));
	m2 = (exp + (((mag << 11) >> exp) & 0x3E0));

	state->sr[0] = m2 - (m1 & 0x800);

/* DELAY A */
	state->pk[1] = state->pk[0];
	state->pk[0] = pk0;

/* TONE */
	state->td = (mlib_u8)((tr - 1) | -(tr - 1)) >> 7;
	state->td &= (mlib_u16)(a2p + 11776) >> 15;

/* Adaptation speed control */

/* FILTA */
	state->dms += (fi - state->dms) >> 5;

/* FILTB */
	state->dml += (((fi << 2) - state->dml) >> 7);

	stateap = state->ap;

	if (tr == 1)
		stateap = 256;
	else if (y < 1536)
/* SUBTC */
		stateap += (0x200 - stateap) >> 4;
	else if (state->td == 1)
		stateap += (0x200 - stateap) >> 4;
	else if (abs((state->dms << 2) - state->dml) >= (state->dml >> 3))
		stateap += (0x200 - stateap) >> 4;
	else
		stateap += (-stateap) >> 4;
	state->ap = stateap;
}

/* *********************************************************** */

/* Main */

/* *********************************************************** */

mlib_status
__mlib_SignalADPCM2Bits2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *adpcm,
    void *decode,
    mlib_s32 n)
{
	mlib_G72x_state *state = (mlib_G72x_state *) decode;
	mlib_u8 *sp = (void *)adpcm;
	mlib_u8 adpcm_val;
	mlib_s16 *dp = pcm;
	mlib_s16 sezi, sei, sez, se;
	mlib_s16 y, sr, dq, dqsez;
	mlib_s32 dqsign, v0, v1, srt;
	mlib_s32 diff0, diff1, sign0, sign1;
	mlib_s32 i;

	if (sp == NULL || decode == NULL || dp == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
/* mask to get proper bits */
		adpcm_val = sp[i] & 0x3;

		sezi = mlib_predictor_zero(state);
		sez = sezi >> 1;
		sei = sezi + mlib_predictor_pole(state);

/* se = estimated signal */
		se = sei >> 1;

/* dynamic quantizer step size */
		y = mlib_step_size(state);

/* quantized diff. */
		dq = mlib_reconstruct(adpcm_val & 0x02,
		    mlib_ADPCM2Bits_rsf[adpcm_val], y);

/* reconst. signal */
		v0 = se - (dq & 0x3FFF);
		v1 = se + dq;
		dqsign = (mlib_s32)dq >> 31;
		sr = v1 + ((v0 - v1) & dqsign);

/* pole prediction diff. */
		dqsez = sr - se + sez;

		diff0 = sr - (-8192);
		diff1 = 8191 - sr;
		sign0 = diff0 >> 31;
		sign1 = diff1 >> 31;
		srt = sr - (diff0 & sign0);
		srt += diff1 & sign1;

		dp[i] = srt << 2;

/* update state variables */
		mlib_update(y, mlib_ADPCM2Bits_sfm[adpcm_val],
		    mlib_ADPCM2Bits_st[adpcm_val], dq, sr, dqsez, state);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLinear2ADPCM2Bits(
    mlib_u8 *adpcm,
    const mlib_s16 *pcm,
    void *encode,
    mlib_s32 n)
{
	mlib_G72x_state *state = (mlib_G72x_state *) encode;
	mlib_u8 *dp = adpcm;
	mlib_s16 *sp = (void *)pcm;
	mlib_u8 adpcm_val;
	mlib_s16 sei, sezi, se, sez, sl;
	mlib_s16 d, y, sr, dqsez, dq;
	mlib_s32 dqsign, v0, v1;
	mlib_s32 i;

	if (sp == NULL || encode == NULL || dp == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		sl = sp[i] >> 2;

		sezi = mlib_predictor_zero(state);
		sez = sezi >> 1;
		sei = sezi + mlib_predictor_pole(state);

/* estimated signal */
		se = sei >> 1;

/* estimation difference */
		d = sl - se;

/* quantize the prediction difference */

/* quantizer step size */
		y = mlib_step_size(state);

/* adpcm_val = ADPCM code */
		adpcm_val = mlib_quantize(d, y);
		dp[i] = adpcm_val;

/* quantized est diff */
		dq = mlib_reconstruct(adpcm_val & 2,
		    mlib_ADPCM2Bits_rsf[adpcm_val], y);

/* reconst. signal */
		v0 = se - (dq & 0x3FFF);
		v1 = se + dq;
		dqsign = (mlib_s32)dq >> 31;
		sr = v1 + ((v0 - v1) & dqsign);

		dqsez = sr + sez - se;

/* update state variables */
		mlib_update(y, mlib_ADPCM2Bits_sfm[adpcm_val],
		    mlib_ADPCM2Bits_st[adpcm_val], dq, sr, dqsez, state);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
