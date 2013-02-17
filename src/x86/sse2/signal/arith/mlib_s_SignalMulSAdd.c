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

#pragma ident	"@(#)mlib_s_SignalMulSAdd.c	9.6	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSAdd_[S16|S16S]_Sat,
 *      mlib_SignalMulSAdd_[S16|S16S]_[S16|S16S]_Sat - linear function on
 *                     mlib_s32 16-bit format signal array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulSAdd_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src1,
 *                                                   const mlib_s16 *src2,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16S_Sat(mlib_s16       *src1dst,
 *                                              const mlib_s16 *src2,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16_S16_Sat(mlib_s16       *dst,
 *                                                 const mlib_s16 *src1,
 *                                                 const mlib_s16 *src2,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16_Sat(mlib_s16       *src1dst,
 *                                             const mlib_s16 *src2,
 *                                             const mlib_s16 *c,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      src1dst  The first input and output signal array
 *      dst      Output signal array
 *      src1     The first input signal array
 *      src2     The second input signal array
 *      c        Scaling factor. The scaling factor is in Q15 format
 *      n        Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 + c * src2
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

#pragma weak mlib_SignalMulSAdd_S16S_S16S_Sat = \
	__mlib_SignalMulSAdd_S16S_S16S_Sat
#pragma weak mlib_SignalMulSAdd_S16_S16_Sat = \
	__mlib_SignalMulSAdd_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSAdd_S16S_S16S_Sat) mlib_SignalMulSAdd_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSAdd_S16_S16_Sat) mlib_SignalMulSAdd_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_S16_S16(ld1, ld2, st)                              \
{                                                              \
	__m128i dlower, dupper;                                    \
	__m128i dtmp1, dtmp2;                                      \
	dx = _mm_##ld1##_si128(dpx);                               \
	dy = _mm_##ld2##_si128(dpy);                               \
	dtmp1 = _mm_mullo_epi16(dc, dy);                           \
	dtmp2 = _mm_mulhi_epi16(dc, dy);                           \
	dlower = _mm_unpacklo_epi16(dtmp1, dtmp2);                 \
	dlower = _mm_srai_epi32(dlower, 15);                       \
	dupper = _mm_unpackhi_epi16(dtmp1, dtmp2);                 \
	dupper = _mm_srai_epi32(dupper, 15);                       \
	dz = _mm_packs_epi32(dlower, dupper);                      \
	dz = _mm_adds_epi16(dz, dx);                               \
                                                               \
	_mm_##st##_si128(dpz, dz);                                 \
	dpx++;                                                     \
	dpy++;                                                     \
	dpz++;                                                     \
}

/* *********************************************************** */
#define	MLIB_SATURATE_S16(X, dst)                              \
{                                                              \
	dst = (((X) > 0x7fff) ? MLIB_S16_MAX :                     \
			((X) < (-0x8000) ? MLIB_S16_MIN : (mlib_s16)(X))); \
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
	__m128i *dpx, *dpy, *dpz;
	__m128i dx, dy, dz, dc;
	__m128i dtemp;

	mlib_s32 i, ncurrent, dst1;
	int npre1 = (mlib_addr)src1 & 0xf;
	int npre2 = (mlib_addr)src2 & 0xf;
	int npre3 = (mlib_addr)dst & 0xf;

	int n1 = ((16 - npre1) & 0xf) >> 1;
	int n2 = ((n - n1)) >> 3;

	dpx = (__m128i *) src1;
	dpy = (__m128i *) src2;
	dpz = (__m128i *) dst;
	dc = _mm_set1_epi16(c[0]);

	if (n2 > 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n1; i++) {
			dst1 = (mlib_s32)src1[i] +
					(((mlib_s32)src2[i] * c[0]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[i]);
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
			dst1 = (mlib_s32)src1[i] +
					(((mlib_s32)src2[i] * c[0]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[i]);
		}
	} else {
		for (i = 0; i < n; i++) {
			dst1 = (mlib_s32)src1[i] +
					(((mlib_s32)src2[i] * c[0]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[i]);
		}
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
	__m128i *dpx, *dpy, *dpz;
	__m128i dx, dy, dz, dc;
	__m128i dtemp;

	mlib_s32 i, ncurrent, dst1;
	int npre1 = (mlib_addr)src1 & 0xf;
	int npre2 = (mlib_addr)src2 & 0xf;
	int npre3 = (mlib_addr)dst & 0xf;

	int n1 = ((16 - npre1) & 0xf) >> 1;
	int n2 = (((n << 1) - n1)) >> 3;

	dpx = (__m128i *) src1;
	dpy = (__m128i *) src2;
	dpz = (__m128i *) dst;
	if (n2 > 0) {
		if ((n1 & 0x1) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < (n1 >> 1); i++) {
				dst1 = (mlib_s32)src1[2 * i] +
					(((mlib_s32)src2[2 * i] * c[0]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i]);
				dst1 = (mlib_s32)src1[2 * i + 1] +
					(((mlib_s32)src2[2 * i + 1] *
					c[1]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i + 1]);
			}
			src1 += n1;
			src2 += n1;
			dst += n1;
			dpx = (__m128i *) src1;
			dpy = (__m128i *) src2;
			dpz = (__m128i *) dst;
			dc = _mm_set_epi16(c[1], c[0], c[1], c[0],
					c[1], c[0], c[1], c[0]);

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

			ncurrent = n2 << 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = ncurrent; i < n - (n1 >> 1); i++) {
				dst1 = (mlib_s32)src1[2 * i] +
					(((mlib_s32)src2[2 * i] * c[0]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i]);
				dst1 = (mlib_s32)src1[2 * i + 1] +
					(((mlib_s32)src2[2 * i + 1] *
					c[1]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i + 1]);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < (n1 >> 1); i++) {
				dst1 = (mlib_s32)src1[2 * i] +
					(((mlib_s32)src2[2 * i] * c[0]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i]);
				dst1 = (mlib_s32)src1[2 * i + 1] +
					(((mlib_s32)src2[2 * i + 1] *
					c[1]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i + 1]);
			}
			dst1 = (mlib_s32)src1[2 * i] +
				(((mlib_s32)src2[2 * i] * c[0]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[2 * i]);
			src1 += n1;
			src2 += n1;
			dst += n1;
			dpx = (__m128i *) src1;
			dpy = (__m128i *) src2;
			dpz = (__m128i *) dst;
			dc = _mm_set_epi16(c[0], c[1], c[0],
					c[1], c[0], c[1], c[0], c[1]);

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

			ncurrent = n2 << 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = ncurrent; i < n - (n1 >> 1) - 1; i++) {
				dst1 = (mlib_s32)src1[2 * i] +
					(((mlib_s32)src2[2 * i] * c[1]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i]);
				dst1 = (mlib_s32)src1[2 * i + 1] +
					(((mlib_s32)src2[2 * i + 1] *
					c[0]) >> 15);
				MLIB_SATURATE_S16(dst1, dst[2 * i + 1]);
			}
			dst1 = (mlib_s32)src1[2 * i] +
				(((mlib_s32)src2[2 * i] * c[1]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[2 * i]);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < n; i++) {
			dst1 = (mlib_s32)src1[2 * i] +
				(((mlib_s32)src2[2 * i] * c[0]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[2 * i]);
			dst1 = (mlib_s32)src1[2 * i + 1] +
				(((mlib_s32)src2[2 * i + 1] * c[1]) >> 15);
			MLIB_SATURATE_S16(dst1, dst[2 * i + 1]);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
