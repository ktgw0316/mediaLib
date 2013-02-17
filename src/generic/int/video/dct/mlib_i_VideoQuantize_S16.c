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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
#ifdef __SUNPRO_C
#pragma ident	"@(#)mlib_i_VideoQuantize_S16.c	9.6	07/11/05 SMI"
#endif

/*
 * FUNCTIONS
 *      mlib_VideoQuantizeInit_S16   - convert quantization table parameter for
 *                                     mlib_VideoQuantize_S16
 *      mlib_VideoQuantize_S16       - "round" divide of aligned S16 vectors
 *      mlib_VideoDeQuantizeInit_S16 - convert quantization table parameter for
 *                                     mlib_VideoDeQuantize_S16
 *      mlib_VideoDeQuantize_S16     - multiply of aligned S16 vectors
 *
 * SYNOPSIS
 *
 *      mlib_status  mlib_VideoQuantizeInit_S16(mlib_d64       dqtable[64],
 *                                              const mlib_s16 iqtable[64])
 *
 *      mlib_status  mlib_VideoQuantize_S16(mlib_s16       icoeffs[64],
 *                                          const mlib_d64 dqtable[64])
 *
 *      mlib_status  mlib_VideoDeQuantizeInit_S16(mlib_d64       dqtable[64],
 *                                                const mlib_s16 iqtable[64])
 *
 *      mlib_status  mlib_VideoDeQuantize_S16(mlib_s16       icoeffs[64],
 *                                            const mlib_d64 dqtable[64])
 *
 * ARGUMENTS
 *
 *      icoeffs     Pointer to DCT coefficients, 8-byte aligned
 *
 *      iqtable     Pointer to quantization table coefficients
 *
 *      dqtable     Pointer to converted quantization table coefficients,
 *                  different for Quantize and DeQuantize functions,
 *                  also different for C/VIS/SSE2 versions
 *
 * DESCRIPTION
 *
 *      mlib_VideoQuantize_S16:
 *        icoeffs[i] = "round"(icoeffs[i]/iqtable[i]), 0 <= i < 64
 *
 *        current formula of "round" :
 *          ivalue = (int)(dvalue + 32768.5) - (1 << 15)
 *
 *      mlib_VideoDeQuantize_S16:
 *        icoeffs[i] = icoeffs[i]*iqtable[i], 0 <= i < 64
 *
 * RESTRICTION
 *
 *           0 <  iqtable[i] < 256
 *      -16384 <= icoeffs[i] < 16384,           0 <= i < 64
 *
 *      Results of overflow are not controlled.
 *
 * NOTE
 *
 *   mlib_Video[De]QuantizeInit_S16 generate converted quantization tables
 *   for mlib_Vide[De]Quantize_S16, respectively, in different ways.  The
 *   converted tables for C and VIS versions of the same function can be
 *   different as well.  So never mix the converted quantization tables
 *   between Quantize and DeQuantize functions or between C and VIS versions.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDeQuantizeInit_S16 = __mlib_VideoDeQuantizeInit_S16
#pragma weak mlib_VideoDeQuantize_S16 = __mlib_VideoDeQuantize_S16
#pragma weak mlib_VideoQuantizeInit_S16 = __mlib_VideoQuantizeInit_S16
#pragma weak mlib_VideoQuantize_S16 = __mlib_VideoQuantize_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDeQuantizeInit_S16) mlib_VideoDeQuantizeInit_S16
	__attribute__((weak, alias("__mlib_VideoDeQuantizeInit_S16")));
__typeof__(__mlib_VideoDeQuantize_S16) mlib_VideoDeQuantize_S16
	__attribute__((weak, alias("__mlib_VideoDeQuantize_S16")));
__typeof__(__mlib_VideoQuantizeInit_S16) mlib_VideoQuantizeInit_S16
	__attribute__((weak, alias("__mlib_VideoQuantizeInit_S16")));
__typeof__(__mlib_VideoQuantize_S16) mlib_VideoQuantize_S16
	__attribute__((weak, alias("__mlib_VideoQuantize_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VideoQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		if (iqtable[i] <= 0)
			return (MLIB_FAILURE);
		dqtable[i] = 1.0f / iqtable[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++)
		icoeffs[i] =
			(mlib_s16)((mlib_s32)(icoeffs[i] * dqtable[i]
			+ 32768.5f) - (1 << 15));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++)
		dqtable[i] = iqtable[i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++)
		icoeffs[i] = (mlib_s16)(icoeffs[i] * dqtable[i]);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else
/* *********************************************************** */

#define	C_U52			(((mlib_s64)1 << 51))

#define	DOUBLELEADZERONUM(X, N) {      \
		mlib_s64 x, y;         \
		mlib_s16 m, n;         \
	                               \
		y = -(X >> 32);        \
		m = (y >> 32) & 32;    \
		n = 32 - m;            \
		x = X >> m;            \
				       \
		y = x - 0X10000;       \
		m = (y >> 32) & 16;    \
		n = n + m;             \
		x = x >> (16 - m);     \
				       \
		y = x - 0x100;         \
		m = (y >> 32) & 8;     \
		n = n + m;             \
		x = x << m;            \
				       \
		y = x - 0x1000;        \
		m = (y >> 32) & 4;     \
		n = n + m;             \
		x = x << m;            \
				       \
		y = x - 0x4000;        \
		m = (y >> 32) & 2;     \
		n = n + m;             \
		x = x << m;            \
       				       \
		y = x >> 14;           \
		m = y & ~(y >> 1);     \
		N =  n + 2 - m;        \
}


#define	FIND_LEADING_ZEROS16(a)                                   \
	n = 1;                                                    \
	if (a == 0) n = 2;					  \
	ux = (a) >> 8;                                            \
	if (ux == 0) {	n += 8; ux = a; }                         \
	if (ux >> 4 == 0) {	n += 4;	ux <<= 4; }               \
	if (ux >> 6 == 0) {	n += 2;	ux <<= 2; }               \
	n -= ux >> 7;


mlib_status
__mlib_VideoQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_s32 i;
	mlib_s32 N, iq, k;
	mlib_u32 ua, ux;
	mlib_s64 sa, qa, signa;
	mlib_u64 abs_qa, ea;
	mlib_s64 *ptable;

	ptable = (mlib_s64 *)dqtable;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		if (iqtable[i] <= 0)
			return (MLIB_FAILURE);
		qa = C_U52/iqtable[i];
		signa = qa >> 63;
		abs_qa = (qa ^ signa) - signa;
		DOUBLELEADZERONUM(abs_qa, N);
		k =  N - 11;
		ea = ((1023 + 12 - N));
		ea <<= 52;
		signa = signa & 0x8000000000000000;
		sa = ea | ((abs_qa << k) & 0xFFFFFFFFFFFFF)| signa;
		ptable[i] = sa;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_s32 i;
	mlib_s32 etable;
	mlib_s64 qtable, tmp, sign;
	mlib_s64 dqcoffes;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		dqcoffes = ((mlib_s64 *)dqtable)[i];
		sign = dqcoffes >> 63;
		etable	= 1023 - (dqcoffes >> 52) + 46;
		qtable 	= (dqcoffes & 0xFFFFFFFFFFFFF) \
			| 0x10000000000000;
		tmp = ((icoeffs[i] * (qtable >> 6)) + \
			(1 << (etable -1))) >> etable;
		icoeffs[i] = (mlib_s16)((mlib_s32)((tmp ^ sign) - sign));
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_s32 i;
	mlib_u16 ux;
	mlib_s16 sy, y;
	mlib_u16 ya;
	mlib_s32 n;
	mlib_u64 eq, qa, ta;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		y = iqtable[i];
		sy = y >> 15;
		ya = (y ^ sy) - sy;
		FIND_LEADING_ZEROS16(ya);
		eq = 16 - n + ((16 - n == 0)? 0 : 1022);
		eq <<= 52;
		ya = ya << (n + 1);
		qa = ya;
		qa <<= 36;
		ta = eq | qa | ((mlib_s64) sy & 0x8000000000000000);
		((mlib_u64 *)dqtable)[i] = ta;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_s32 i;
	mlib_s32 etable;
	mlib_s64 qtable, tmp;
	mlib_s64 dqcoffes, tmpa;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		dqcoffes = ((mlib_u64 *)dqtable)[i];
		tmpa = (dqcoffes >> 52);
		etable	=  tmpa - 1023;
		qtable 	= (((dqcoffes & 0xFFFFFFFFFFFFF) | \
		0x10000000000000)) >> ((52 - etable > 52 ? 0 : 52 - etable));
		tmp = icoeffs[i] * qtable;
		icoeffs[i] = (mlib_s16)(tmp);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#endif	/* _NO_LONGLONG */
