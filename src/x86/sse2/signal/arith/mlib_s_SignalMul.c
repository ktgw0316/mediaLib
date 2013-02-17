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

#pragma ident	"@(#)mlib_s_SignalMul.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMul_[S16|S16S]_Sat,
 *      mlib_SignalMul_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal arrays
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMul_S16S_S16S_Sat(mlib_s16       *dst,
 *                                               const mlib_s16 *src1,
 *                                               const mlib_s16 *src2,
 *                                               mlib_s32       n2);
 *      mlib_status mlib_SignalMul_S16S_Sat(mlib_s16       *src1dst,
 *                                          const mlib_s16 *src2,
 *                                          mlib_s32       n);
 *      mlib_status mlib_SignalMul_S16_S16_Sat(mlib_s16       *dst,
 *                                             const mlib_s16 *src1,
 *                                             const mlib_s16 *src2,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalMul_S16_Sat(mlib_s16       *src1dst,
 *                                         const mlib_s16 *src2,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *   src1dst   The first input and the output signal array
 *      dst    Output signal array
 *      src1   The first input signal array
 *      src2   The second input signal array
 *      n      Number of samples in the input signal arrays
 *
 * DESCRIPTION
 *      dst = src1 * src2
 */

#include <mlib_signal.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMul_S16_S16_Sat = __mlib_SignalMul_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMul_S16_S16_Sat) mlib_SignalMul_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMul_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_S16_S16(ld1, ld2, st)                              \
{                                                              \
	dx = _mm_##ld1##_si128(dpx);                               \
	dy = _mm_##ld2##_si128(dpy);                               \
	dz = _mm_mulhi_epi16(dx, dy);                              \
	dz = _mm_slli_epi16(dz, 1);                                \
	dz = _mm_sub_epi16(dz, done);                              \
	_mm_##st##_si128(dpz, dz);                                 \
	dpx++;                                                     \
	dpy++;                                                     \
	dpz++;                                                     \
}
/* *********************************************************** */

mlib_status
__mlib_SignalMul_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	__m128i *dpx, *dpy, *dpz;
	__m128i dx, dy, dz;
	__m128i done = _mm_set1_epi16(1);
	mlib_s32 i, ncurrent, dst1;

	int npre1 = (mlib_addr)src1 & 0xf;
	int npre2 = (mlib_addr)src2 & 0xf;
	int npre3 = (mlib_addr)dst & 0xf;

	int n1 = ((16 - npre1) & 0xf) >> 1;
	int n2 = ((n - n1)) >> 3;
	if (n2 > 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n1; i++) {
			dst1 = ((mlib_s32)src1[i] * src2[i]) >> 15;
			dst[i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}

		src1 += n1;
		src2 += n1;
		dst += n1;
		dpx = (__m128i *) src1;
		dpy = (__m128i *) src2;
		dpz = (__m128i *) dst;
		if ((npre1 == npre2) && (npre2 == npre3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n2; i++) {
				MUL_S16_S16(load, load, store);
			}
		} else if (npre1 == npre2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n2; i++) {
				MUL_S16_S16(load, load, storeu);
			}
		} else if (npre1 == npre3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n2; i++) {
				MUL_S16_S16(load, loadu, store);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n2; i++) {
				MUL_S16_S16(load, loadu, storeu);
			}
		}

		ncurrent = n2 << 3;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = ncurrent; i < n - n1; i++) {
			dst1 = ((mlib_s32)src1[i] * src2[i]) >> 15;
			dst[i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n; i++) {
			dst1 = ((mlib_s32)src1[i] * src2[i]) >> 15;
			dst[i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
