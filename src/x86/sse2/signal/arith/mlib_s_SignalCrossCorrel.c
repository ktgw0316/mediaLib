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

#pragma ident	"@(#)mlib_s_SignalCrossCorrel.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_SignalCrossCorrel - calculates image cross correlation
 *                               for the input signals
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalCrossCorrel_S16(mlib_d64       *correl,
 *                                             const mlib_s16 *src1,
 *                                             const mlib_s16 *src2,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalCrossCorrel_S16S(mlib_d64       *correl,
 *                                              const mlib_s16 *src1,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       n);
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      src1     the first input signal array
 *      src2     the second input signal array
 *      n        number of samples in the signal arrays
 *
 * DESCRIPTION
 *
 *           1    N-1
 *      c = --- * SUM (x[n] * y[n])
 *           N    n = 0
 */

#include <mlib_signal.h>
#include <mlib_SignalCorrel.h>
#include <mlib_s_SignalArith.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalCrossCorrel_S16 = __mlib_SignalCrossCorrel_S16
#pragma weak mlib_SignalCrossCorrel_S16S = __mlib_SignalCrossCorrel_S16S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalCrossCorrel_S16) mlib_SignalCrossCorrel_S16
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16")));
__typeof__(__mlib_SignalCrossCorrel_S16S) mlib_SignalCrossCorrel_S16S
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	MUL_S16(x, y)  ((x) * (mlib_d64) (y))

/* *********************************************************** */
#define	LOAD_X_AND_Y  \
{  \
	if (0 == ((mlib_addr)src1 & 0xf)) {\
		dx = _mm_load_si128(dpx);\
	} else {\
		dx = _mm_loadu_si128(dpx);\
	}\
	if (0 == ((mlib_addr)src2 & 0xf)) {\
		dy = _mm_load_si128(dpy);\
	} else {\
		dy = _mm_loadu_si128(dpy);\
	}\
}

/* *********************************************************** */
#define	MUL_S16_S32 \
{ \
		dtmp1 = _mm_mullo_epi16(dx, dy);\
		dtmp2 = _mm_mulhi_epi16(dx, dy);\
	    __m128i dtmp3; \
	    __m128i dtmp4; \
		dtmp3 = _mm_unpacklo_epi16(dtmp1, dtmp2);\
		dtmp4 = _mm_unpackhi_epi16(dtmp1, dtmp2);\
	    dlower.dmmx = dtmp3; \
	    dupper.dmmx = dtmp4; \
}
/* *********************************************************** */
#ifdef _NO_LONGLONG
#define	SUM_D128 \
{ \
	c1 += (mlib_d64) (dlower.four_int.int0);\
	c2 += (mlib_d64) (dlower.four_int.int1);\
	c3 += (mlib_d64) (dlower.four_int.int2);\
	c4 += (mlib_d64) (dlower.four_int.int3);\
	c1 += (mlib_d64) (dupper.four_int.int0);\
	c2 += (mlib_d64) (dupper.four_int.int1);\
	c3 += (mlib_d64) (dupper.four_int.int2);\
	c4 += (mlib_d64) (dupper.four_int.int3);\
}
#else
#define	SUM_D128 \
{ \
	c1 += dlower.four_int.int0;\
	c2 += dlower.four_int.int1;\
	c3 += dlower.four_int.int2;\
	c4 += dlower.four_int.int3;\
	c1 += dupper.four_int.int0;\
	c2 += dupper.four_int.int1;\
	c3 += dupper.four_int.int2;\
	c4 += dupper.four_int.int3;\
}
#endif
/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 rez[8];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, n);
	correl[0] =
	    (rez[0] + rez[1] + rez[2] + rez[3]) / (n * (mlib_d64)(1 << 30));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16S(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 rez[8];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, n + n);
	correl[0] = (rez[0] + rez[2]) / (n * (mlib_d64)(1 << 30));
	correl[1] = (rez[1] + rez[3]) / (n * (mlib_d64)(1 << 30));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
void
mlib_SignalCrossCorrel_Intr_S16(
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_d64 *rez,
    mlib_s32 n)
{

	__m128i *dpx, *dpy;
	__m128i dx, dy, dz;
	__m128i dtmp1, dtmp2;
	mlib_s32 i;

	dpx = (__m128i *) src1;
	dpy = (__m128i *) src2;
#ifdef _NO_LONGLONG
	mlib_d64 c1, c2, c3, c4;
#else
	mlib_s64 c1, c2, c3, c4;
#endif
	mlib_s32 size = n;
	mlib_s32 j, k;
	type_union_mlib_m128 dlower, dupper;

	c1 = c2 = c3 = c4 = 0;
	rez[0] = rez[1] = rez[2] = rez[3] = 0;
	rez[4] = rez[5] = rez[6] = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j <= (size - 8); j += 8) {
		LOAD_X_AND_Y;
		MUL_S16_S32;
		SUM_D128;
		dpx++;
		dpy++;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (k = 0; j < size; j++, k++) {
		rez[k] += MUL_S16(src1[j], src2[j]);
	}
	rez[0] += c1;
	rez[1] += c2;
	rez[2] += c3;
	rez[3] += c4;
	rez[0] += rez[4];
	rez[1] += rez[5];
	rez[2] += rez[6];
}

/* *********************************************************** */
