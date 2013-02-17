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

#pragma ident	"@(#)mlib_s_SignalMulS.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulS_[S16|S16S]_Sat,
 *      mlib_SignalMulS_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal array to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulS_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                const mlib_s16 *c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16S_Sat(mlib_s16       *data,
 *                                           const mlib_s16 *c,
 *                                           mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16_S16_Sat(mlib_s16       *dst,
 *                                              const mlib_s16 *src,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16_Sat(mlib_s16       *data,
 *                                          const mlib_s16 *c,
 *                                          mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src * c
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

#pragma weak mlib_SignalMulS_S16S_S16S_Sat = __mlib_SignalMulS_S16S_S16S_Sat
#pragma weak mlib_SignalMulS_S16_S16_Sat = __mlib_SignalMulS_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulS_S16S_S16S_Sat) mlib_SignalMulS_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulS_S16_S16_Sat) mlib_SignalMulS_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */


/* *********************************************************** */

#define	MUL_S16_S16(ld1, st)                                   \
{                                                              \
	dx = _mm_##ld1##_si128(dpx);                               \
	dz = _mm_mulhi_epi16(dx, dy);                              \
	dz = _mm_min_epi16(dz, dMAX);                              \
	dz = _mm_slli_epi16(dz, 1);                                \
	_mm_##st##_si128(dpz, dz);                                 \
	dpx++;                                                     \
	dpz++;                                                     \
}
/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 n)
{
	__m128i *dpx, *dpz;
	__m128i dx, dy, dz;

	dy = _mm_set1_epi16(c[0]);
	__m128i dMAX = _mm_set1_epi16(0x3fff);
	__m128i dtemp;

	mlib_s32 i, ncurrent, dst1;
	int npre1 = (mlib_addr)src & 0xf;
	int npre2 = (mlib_addr)dst & 0xf;

	int n1 = ((16 - npre1) & 0xf) >> 1;
	int n2 = ((n - n1)) >> 3;
	if (n2 > 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n1; i++) {
			dst1 = ((mlib_s32)src[i] * c[0]) >> 15;
			dst[i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}

		src += n1;
		dst += n1;
		dpx = (__m128i *) src;
		dpz = (__m128i *) dst;
		if ((npre1 == npre2)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n2; i++) {
				MUL_S16_S16(load, store);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n2; i++) {
				MUL_S16_S16(load, storeu);
			}
		}

		ncurrent = n2 << 3;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = ncurrent; i < n - n1; i++) {
			dst1 = ((mlib_s32)src[i] * c[0]) >> 15;
			dst[i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n; i++) {
			dst1 = ((mlib_s32)src[i] * c[0]) >> 15;
			dst[i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16S_S16S_Sat(
	mlib_s16 *dst,
	const mlib_s16 *src,
	const mlib_s16 *c,
	mlib_s32 n)
{
	__m128i *dpx, *dpz;
	__m128i dx, dy, dz;

	__m128i dMAX = _mm_set1_epi16(0x3fff);
	__m128i dtemp;

	mlib_s32 i, ncurrent, dst1;
	int npre1 = (mlib_addr)src & 0xf;
	int npre2 = (mlib_addr)dst & 0xf;

	int n1 = ((16 - npre1) & 0xf) >> 1;
	int n2 = (((n << 1) - n1)) >> 3;
	if (n2 > 0) {
		if ((n1 & 0x1) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < (n1 >> 1); i++) {
				dst1 = ((mlib_s32)src[2 * i] * c[0]) >> 15;
				dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
				dst1 = ((mlib_s32)src[2 * i + 1] * c[1]) >> 15;
				dst[2 * i + 1] = dst1 - (dst1 >> 15) +
					(dst1 >> 16);
			}
			src += n1;
			dst += n1;
			dpx = (__m128i *) src;
			dpz = (__m128i *) dst;
			dy = _mm_set_epi16(c[1], c[0], c[1], c[0],
					c[1], c[0], c[1], c[0]);
			if ((npre1 == npre2)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
				for (i = 0; i < n2; i++) {
					MUL_S16_S16(load, store);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
				for (i = 0; i < n2; i++) {
					MUL_S16_S16(load, storeu);
				}
			}

			ncurrent = n2 << 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = ncurrent; i < n - (n1 >> 1); i++) {
				dst1 = ((mlib_s32)src[2 * i] * c[0]) >> 15;
				dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
				dst1 = ((mlib_s32)src[2 * i + 1] * c[1]) >> 15;
				dst[2 * i + 1] = dst1 - (dst1 >> 15) +
					(dst1 >> 16);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < (n1 >> 1); i++) {
				dst1 = ((mlib_s32)src[2 * i] * c[0]) >> 15;
				dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
				dst1 = ((mlib_s32)src[2 * i + 1] * c[1]) >> 15;
				dst[2 * i + 1] = dst1 - (dst1 >> 15) +
					(dst1 >> 16);
			}
			dst1 = ((mlib_s32)src[2 * i] * c[0]) >> 15;
			dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
			src += n1;
			dst += n1;
			dpx = (__m128i *) src;
			dpz = (__m128i *) dst;
			dy = _mm_set_epi16(c[0], c[1], c[0],
					c[1], c[0], c[1], c[0], c[1]);
			if ((npre1 == npre2)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
				for (i = 0; i < n2; i++) {
					MUL_S16_S16(load, store);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
				for (i = 0; i < n2; i++) {
					MUL_S16_S16(load, storeu);
				}
			}

			ncurrent = n2 << 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = ncurrent; i < n - (n1 >> 1) - 1; i++) {
				dst1 = ((mlib_s32)src[2 * i] * c[1]) >> 15;
				dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
				dst1 = ((mlib_s32)src[2 * i + 1] * c[0]) >> 15;
				dst[2 * i + 1] = dst1 - (dst1 >> 15) +
					(dst1 >> 16);
			}
			dst1 = ((mlib_s32)src[2 * i] * c[1]) >> 15;
			dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n; i++) {
			dst1 = ((mlib_s32)src[2 * i] * c[0]) >> 15;
			dst[2 * i] = dst1 - (dst1 >> 15) + (dst1 >> 16);
			dst1 = ((mlib_s32)src[2 * i + 1] * c[1]) >> 15;
			dst[2 * i + 1] = dst1 - (dst1 >> 15) + (dst1 >> 16);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
