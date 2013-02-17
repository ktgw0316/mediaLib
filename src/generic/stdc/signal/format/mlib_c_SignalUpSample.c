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

#pragma ident	"@(#)mlib_c_SignalUpSample.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalUpSample_[S16|S16S]_[S16|S16S] - upsampling
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalUpSample_S16S_S16S(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                mlib_s32       factor,
 *                                                mlib_s32       phase,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalUpSample_S16_S16(mlib_s16       *dst,
 *                                              const mlib_s16 *src,
 *                                              mlib_s32       factor,
 *                                              mlib_s32       phase,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      dst     Output signal array
 *      src     Input signal array
 *      factor  Factor by which to upsample
 *      phase   Parameter that determines relative position of an input
 *              value, within the output signal. 0 <= phase < factor.
 *      n       Number of samples in the input signal array.
 *
 * DESCRIPTION
 *      X       = x(n), n = 0, 1, ...
 *      Zup     = z(n), n = 0, 1, ...
 *
 *                  x(k),  n = (factor*k + phase)
 *              =
 *                  0   ,  otherwise
 *
 * COMMENTS
 *      A user should supply an output vector
 *      big enough to hold all samples.
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalUpSample_S16S_S16S = __mlib_SignalUpSample_S16S_S16S
#pragma weak mlib_SignalUpSample_S16_S16 = __mlib_SignalUpSample_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalUpSample_S16S_S16S) mlib_SignalUpSample_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalUpSample_S16S_S16S")));
__typeof__(__mlib_SignalUpSample_S16_S16) mlib_SignalUpSample_S16_S16
    __attribute__((weak, alias("__mlib_SignalUpSample_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status
mlib_c_SignalUpSample2_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 *pdst32;
	mlib_s32 *psrc32;
	mlib_u32 data;
	mlib_s32 i, shift;

	if (2 & (mlib_addr)src) {
		if (phase != 0) {
			dst[0] = 0;
			dst[1] = src[0];
		} else {
			dst[0] = src[0];
			dst[1] = 0;
		}

		dst += 2;
		src++;
		n--;

		if (n == 0) {
			return (MLIB_SUCCESS);
		}
	}

	if (2 & (mlib_addr)dst) {
		shift = 1;

		if (phase != 0) {
			(*dst++) = 0;
			phase = 0;
		} else {
			(*dst++) = (*src++);
			(*dst++) = 0;
			n--;

			if (n == 0)
				return (MLIB_SUCCESS);
			(*dst++) = (*src++);
			n--;
			phase = 1;
		}
	} else
		shift = 0;

	psrc32 = (mlib_s32 *)src;
	pdst32 = (mlib_s32 *)dst;

	if (phase != 0) {
		for (i = 0; i < n - 2; i += 2) {
			data = (*psrc32++);
#ifdef _LITTLE_ENDIAN
			pdst32[0] = (data << 16);
			pdst32[1] = (data & 0xFFFF0000);
#else /* _LITTLE_ENDIAN */
			pdst32[0] = (data >> 16);
			pdst32[1] = (data & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
			pdst32 += 2;
		}

		dst = (mlib_s16 *)pdst32;
		src = (mlib_s16 *)psrc32;

		if (shift) {
			phase = 0;
			(*dst++) = 0;
		}
	} else {
		for (i = 0; i < n - 2; i += 2) {
			data = (*psrc32++);
#ifdef _LITTLE_ENDIAN
			pdst32[0] = (data & 0xFFFF);
			pdst32[1] = (data >> 16);
#else /* _LITTLE_ENDIAN */
			pdst32[0] = (data & 0xFFFF0000);
			pdst32[1] = (data << 16);
#endif /* _LITTLE_ENDIAN */
			pdst32 += 2;
		}

		dst = (mlib_s16 *)pdst32;
		src = (mlib_s16 *)psrc32;

		if (shift) {
			phase = 1;
			dst--;
		}
	}

	if (phase != 0) {
		for (; i < n; i++) {
			dst[0] = 0;
			dst[1] = (*src++);
			dst += 2;
		}
	} else {
		for (; i < n; i++) {
			dst[0] = (*src++);
			dst[1] = 0;
			dst += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef _NO_LONGLONG

mlib_status
__mlib_SignalUpSample_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 data;
	mlib_u32 *psrc32;
	mlib_s32 factorx2;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return (__mlib_VectorCopy_S16(dst, src, n));

	if (factor == 2)
		return (mlib_c_SignalUpSample2_S16_S16(dst, src, phase, n));

	__mlib_VectorZero_S16((void *)dst, n * factor);

	factorx2 = factor + factor;

	dst += phase;

	for (i = 0; i < n && (2 & (mlib_addr)src) != 0; i++, dst += factor)
		*dst = (*src++);

	psrc32 = (mlib_u32 *)src;

	for (; i < n - 1; i += 2, dst += factorx2) {
		data = (*psrc32++);

#ifdef _LITTLE_ENDIAN
		dst[factor] = (data >> 16);
		dst[0] = data;
#else /* _LITTLE_ENDIAN */
		dst[factor] = data;
		dst[0] = (data >> 16);

#endif /* _LITTLE_ENDIAN */
	}

	src = (mlib_s16 *)psrc32;

	for (; i < n; i++, dst += factor)
		*dst = (*src++);

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalUpSample_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_u32 data1, data2;
	mlib_u32 *psrc32;
	mlib_s32 i;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return (__mlib_VectorCopy_S16(dst, src, 2 * n));

	__mlib_VectorZero_S16((void *)dst, 2 * n * factor);

	dst += 2 * phase;

	factor <<= 1;

	i = 0;

	if ((2 & (mlib_addr)src) == 0) {
		psrc32 = (mlib_u32 *)src;

		if ((2 & (mlib_addr)dst) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, dst += factor) {
				*(mlib_s32 *)dst = (*psrc32++);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, dst += factor) {
				data1 = (*psrc32++);
#ifdef _LITTLE_ENDIAN
				dst[1] = (data1 >> 16);
				dst[0] = data1;
#else /* _LITTLE_ENDIAN */
				dst[1] = data1;
				dst[0] = (data1 >> 16);
#endif /* _LITTLE_ENDIAN */
			}
		}

		src = (mlib_s16 *)psrc32;
	} else {
		psrc32 = (mlib_u32 *)(src - 1);

		if ((2 & (mlib_addr)dst) == 0) {
			data1 = (*psrc32++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, dst += factor) {
				data2 = (*psrc32++);

#ifdef _LITTLE_ENDIAN
				*((mlib_u32 *)dst) =
				    (data2 << 16) + (data1 >> 16);
#else /* _LITTLE_ENDIAN */
				*((mlib_u32 *)dst) =
				    (data2 >> 16) + (data1 << 16);
#endif /* _LITTLE_ENDIAN */
				data1 = data2;
			}
		} else {
			data1 = (*psrc32++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, dst += factor) {
				data2 = (*psrc32++);
#ifdef _LITTLE_ENDIAN
				dst[1] = data2;
				dst[0] = (data1 >> 16);
#else /* _LITTLE_ENDIAN */
				dst[1] = (data2 >> 16);
				dst[0] = data1;

#endif /* _LITTLE_ENDIAN */
				data1 = data2;
			}
		}

		src = 1 + (mlib_s16 *)(psrc32 - 1);
	}

	for (; i < n; i++, dst += factor, src += 2) {
		dst[0] = src[0];
		dst[1] = src[1];
	}

	return (MLIB_SUCCESS);
}

#else /* _NO_LONGLONG */

mlib_status
__mlib_SignalUpSample_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_u64 data;
	mlib_u64 *psrc64;
	mlib_s32 factorx2, factorx3, factorx4;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return (__mlib_VectorCopy_S16(dst, src, n));

	if (factor == 2)
		return (mlib_c_SignalUpSample2_S16_S16(dst, src, phase, n));

	__mlib_VectorZero_S16((void *)dst, n * factor);

	factorx2 = factor + factor;
	factorx3 = factorx2 + factor;
	factorx4 = factorx2 + factorx2;

	dst += phase;

	for (i = 0; i < n && (6 & (mlib_addr)src) != 0; i++, dst += factor)
		*dst = (*src++);

	psrc64 = (mlib_u64 *)src;

	for (; i < n - 3; i += 4, dst += factorx4) {
		data = (*psrc64++);
#ifdef _LITTLE_ENDIAN
		dst[0] = data;
		dst[factor] = (data >>= 16);
		dst[factorx2] = (data >>= 16);
		dst[factorx3] = (data >>= 16);
#else /* _LITTLE_ENDIAN */
		dst[factorx3] = data;
		dst[factorx2] = (data >>= 16);
		dst[factor] = (data >>= 16);
		dst[0] = (data >>= 16);
#endif /* _LITTLE_ENDIAN */
	}

	src = (mlib_s16 *)psrc64;

	for (; i < n; i++, dst += factor)
		*dst = (*src++);

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalUpSample_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_u64 data1, data2;
	mlib_u64 *psrc64;
	mlib_s32 i, factorx2;
/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return (__mlib_VectorCopy_S16(dst, src, 2 * n));

	__mlib_VectorZero_S16((void *)dst, 2 * n * factor);

	dst += 2 * phase;

	factor <<= 1;
	factorx2 = factor + factor;

	i = 0;

	if ((4 & (mlib_addr)src) != 0 && i < n) {
		dst[0] = src[0];
		dst[1] = src[1];
		dst += factor;
		src += 2;
		i++;
	}

	if ((2 & (mlib_addr)src) == 0) {
		psrc64 = (mlib_u64 *)src;

		if ((2 & (mlib_addr)dst) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n - 1; i += 2, dst += factorx2) {
				data1 = (*psrc64++);
#ifdef _LITTLE_ENDIAN
				*((mlib_s32 *)dst) = data1;
				*((mlib_s32 *)(dst + factor)) = (data1 >> 32);
#else /* _LITTLE_ENDIAN */
				*((mlib_s32 *)dst) = (data1 >> 32);
				*((mlib_s32 *)(dst + factor)) = data1;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n - 1; i += 2, dst += factorx2) {
				data1 = (*psrc64++);
#ifdef _LITTLE_ENDIAN
				dst[0] = data1;
				dst[1] = (data1 >>= 16);
				dst[factor] = (data1 >>= 16);
				dst[factor + 1] = (data1 >>= 16);
#else /* _LITTLE_ENDIAN */
				dst[factor + 1] = data1;
				dst[factor] = (data1 >>= 16);
				dst[1] = (data1 >>= 16);
				dst[0] = (data1 >>= 16);
#endif /* _LITTLE_ENDIAN */
			}
		}

		src = (mlib_s16 *)psrc64;
	} else {
		psrc64 = (mlib_u64 *)(src - 1);

		if ((2 & (mlib_addr)dst) == 0) {
			data1 = (*psrc64++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n - 1; i += 2, dst += factorx2) {
				data2 = (*psrc64++);
#ifdef _LITTLE_ENDIAN
				*((mlib_s32 *)dst) = (data1 >> 16);
				*((mlib_s32 *)(dst + factor)) =
				    (data1 >> 48) + (data2 << 16);
#else /* _LITTLE_ENDIAN */
				*((mlib_s32 *)dst) = (data1 >> 16);
				*((mlib_s32 *)(dst + factor)) =
				    (data2 >> 48) + (data1 << 16);
#endif /* _LITTLE_ENDIAN */
				data1 = data2;
			}
		} else {
			data1 = (*psrc64++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n - 1; i += 2, dst += factorx2) {
				data2 = (*psrc64++);
#ifdef _LITTLE_ENDIAN
				dst[0] = (data1 >> 16);
				dst[1] = (data1 >> 32);
				dst[factor] = (data1 >> 48);
				dst[factor + 1] = data2;
#else /* _LITTLE_ENDIAN */
				dst[factor + 1] = (data2 >> 48);
				dst[factor] = data1;
				dst[1] = (data1 >>= 16);
				dst[0] = (data1 >>= 16);
#endif /* _LITTLE_ENDIAN */
				data1 = data2;
			}
		}

		src = 1 + (mlib_s16 *)(psrc64 - 1);
	}

	for (; i < n; i++, dst += factor, src += 2) {
		dst[0] = src[0];
		dst[1] = src[1];
	}

	return (MLIB_SUCCESS);
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */
