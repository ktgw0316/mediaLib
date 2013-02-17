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

#pragma ident	"@(#)mlib_SignalUpSample_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalUpSample_[F32|F32S]_[F32|F32S] - upsampling
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalUpSample_F32S_F32S(mlib_f32       *dst,
 *                                                const mlib_f32 *src,
 *                                                mlib_s32       factor,
 *                                                mlib_s32       phase,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalUpSample_F32_F32(mlib_f32       *dst,
 *                                              const mlib_f32 *src,
 *                                              mlib_s32       factor,
 *                                              mlib_s32       phase,
 *                                              mlib_s32       n);
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

#pragma weak mlib_SignalUpSample_F32S_F32S = __mlib_SignalUpSample_F32S_F32S
#pragma weak mlib_SignalUpSample_F32_F32 = __mlib_SignalUpSample_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalUpSample_F32S_F32S) mlib_SignalUpSample_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalUpSample_F32S_F32S")));
__typeof__(__mlib_SignalUpSample_F32_F32) mlib_SignalUpSample_F32_F32
    __attribute__((weak, alias("__mlib_SignalUpSample_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _NO_LONGLONG

static mlib_status mlib_SignalUpSample2_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 phase,
    mlib_s32 n);

mlib_status
__mlib_SignalUpSample_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 data;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return __mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src,
		    n);

	if (factor == 2)
		return (mlib_SignalUpSample2_F32_F32(dst, src, phase, n));

	__mlib_VectorZero_S32((void *)dst, n * factor);

	dst += phase;
	data = *src;

	for (i = 0; i < n - 1; i++) {
		src++;
		*dst = data;
		dst += factor;
		data = *src;
	}
	*dst = data;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSample_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_f32 data1, data2;
	mlib_d64 *psrc64;
	mlib_d64 *pdst64;
	mlib_s32 i;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return __mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src,
		    2 * n);

	__mlib_VectorZero_S32((void *)dst, 2 * n * factor);

	dst += 2 * phase;

	i = 0;

	if (((4 & (mlib_addr)src) == 0) && ((4 & (mlib_addr)dst) == 0)) {
		psrc64 = (mlib_d64 *)src;
		pdst64 = (mlib_d64 *)dst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < n; i++, pdst64 += factor) {
			*pdst64 = (*psrc64++);
		}

		src = (mlib_f32 *)psrc64;
	} else {
		factor <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < n; i++, dst += factor, src += 2) {
			dst[0] = src[0];
			dst[1] = src[1];
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_SignalUpSample2_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i;

	if (phase != 0) {
		for (i = 0; i < n; i++) {
			dst[0] = 0;
			dst[1] = (*src++);
			dst += 2;
		}
	} else {
		for (i = 0; i < n; i++) {
			dst[0] = (*src++);
			dst[1] = 0;
			dst += 2;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#else /* _NO_LONGLONG */

static mlib_status mlib_SignalUpSample2_F32_F32(
    mlib_u32 *dst,
    const mlib_u32 *src,
    mlib_s32 phase,
    mlib_s32 n);

mlib_status
__mlib_SignalUpSample_F32_F32(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_u64 data;
	mlib_u32 *dst = (mlib_u32 *)pdst, *src = (mlib_u32 *)psrc;
	mlib_u64 *psrc64;
	mlib_s32 factorx2;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1)
		return __mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src,
		    n);

	if (factor == 2)
		return (mlib_SignalUpSample2_F32_F32(dst, src, phase, n));

	__mlib_VectorZero_S32((void *)dst, n * factor);

	i = 0;
	dst += phase;

	if (((mlib_addr)src & 7) != 0) {
		*dst = (*src++);
		i++;
		if (i == n)
			return (MLIB_SUCCESS);
		dst += factor;
	}

	factorx2 = factor << 1;
	psrc64 = (mlib_u64 *)src;

	data = *psrc64;
	for (; i < n - 3; i += 2, dst += factorx2) {
		psrc64++;
#ifdef _LITTLE_ENDIAN
		dst[0] = data;
		dst[factor] = (data >>= 32);
#else /* _LITTLE_ENDIAN */
		dst[factor] = data;
		dst[0] = (data >>= 32);
#endif /* _LITTLE_ENDIAN */
		data = *psrc64;
	}

	if (i < n - 1) {
#ifdef _LITTLE_ENDIAN
		dst[0] = data;
		dst[factor] = (data >>= 32);
#else /* _LITTLE_ENDIAN */
		dst[factor] = data;
		dst[0] = (data >>= 32);
#endif /* _LITTLE_ENDIAN */
		i += 2;
		psrc64++;
		dst += factorx2;
	}
	if (i < n) {
		*dst = *(mlib_u32 *)psrc64;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0, f1;
	} f32s;
	struct
	{
		mlib_s32 f0, f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

mlib_status
__mlib_SignalUpSample_F32S_F32S_f2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_s32 i;

	if (!phase) {
		(*pdst++) = (*psrc++);
		(*pdst++) = (*psrc++);
		n--;

		if (!n) {
			(*pdst++) = 0;
			(*pdst++) = 0;
			return (MLIB_SUCCESS);
		}
	}

	if (((mlib_s32)pdst & 7) == 0) {
		if (((mlib_s32)psrc & 7) == 0) {
			mlib_d64 *sp = (mlib_d64 *)psrc;
			mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n; i++) {
				dp[0] = 0;
				dp[1] = (*sp++);
				dp += 2;
			}

		} else {
			mlib_d64 *sp = (mlib_d64 *)(psrc - 1);
			mlib_d64 *dp = (mlib_d64 *)pdst;
			d64_2x32 s0, s1, dd;

			s1.d64 = (*sp++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n; i++) {
				s0.d64 = s1.d64;
				s1.d64 = (*sp++);
				dd.f32s.f0 = s0.f32s.f1;
				dd.f32s.f1 = s1.f32s.f0;
				dp[0] = 0;
				dp[1] = dd.d64;
				dp += 2;
			}
		}

	} else {
		mlib_f32 *sp = (mlib_f32 *)psrc;
		mlib_d64 *dp = (mlib_d64 *)(pdst + 1);
		d64_2x32 d0, d1;

		pdst[0] = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n - 1); i++) {
			d0.f32s.f0 = 0;
			d0.f32s.f1 = (*sp++);
			d1.f32s.f0 = (*sp++);
			d1.f32s.f1 = 0;
			dp[0] = d0.d64;
			dp[1] = d1.d64;
			dp += 2;
		}

		((mlib_f32 *)dp)[0] = 0;
		((mlib_f32 *)dp)[1] = sp[0];
		((mlib_f32 *)dp)[2] = sp[1];
	}

	if (!phase) {
		pdst[4 * n] = 0;
		pdst[4 * n + 1] = 0;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalUpSample_F32S_F32S(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_u64 data1, data2;
	mlib_u32 *dst = (mlib_u32 *)pdst;
	mlib_u32 *src = (mlib_u32 *)psrc;
	mlib_u64 *psrc64;
	mlib_u64 *pdst64;
	mlib_s32 i;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (factor == 1) {
		return __mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src,
		    2 * n);
	}

	if (factor == 2) {
		return (
		    __mlib_SignalUpSample_F32S_F32S_f2
		    (pdst, psrc, phase, n));
	}

	__mlib_VectorZero_S32((void *)dst, 2 * n * factor);

	dst += 2 * phase;

	i = 0;

	if ((4 & (mlib_addr)src) == 0) {
		psrc64 = (mlib_u64 *)src;

		if ((4 & (mlib_addr)dst) == 0) {
			pdst64 = (mlib_u64 *)dst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, pdst64 += factor) {
				*pdst64 = (*psrc64++);
			}
		} else {
			factor <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, dst += factor) {
				data1 = (*psrc64++);
#ifdef _LITTLE_ENDIAN
				dst[1] = (data1 >> 32);
				dst[0] = data1;
#else /* _LITTLE_ENDIAN */
				dst[1] = data1;
				dst[0] = (data1 >> 32);
#endif /* _LITTLE_ENDIAN */
			}
		}

		src = (mlib_u32 *)psrc64;
	} else {
		psrc64 = (mlib_u64 *)(src - 1);

		if ((4 & (mlib_addr)dst) == 0) {
			pdst64 = (mlib_u64 *)dst;
			data1 = (*psrc64++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, pdst64 += factor) {
				data2 = (*psrc64++);

#ifdef _LITTLE_ENDIAN
				*pdst64 = (data2 << 32) + (data1 >> 32);
#else /* _LITTLE_ENDIAN */
				*pdst64 = (data2 >> 32) + (data1 << 32);
#endif /* _LITTLE_ENDIAN */
				data1 = data2;
			}
		} else {
			factor <<= 1;
			data1 = (*psrc64++);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < n; i++, dst += factor) {
				data2 = (*psrc64++);
#ifdef _LITTLE_ENDIAN
				dst[1] = data2;
				dst[0] = (data1 >> 32);
#else /* _LITTLE_ENDIAN */
				dst[1] = (data2 >> 32);
				dst[0] = data1;

#endif /* _LITTLE_ENDIAN */
				data1 = data2;
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_SignalUpSample2_F32_F32(
    mlib_u32 *dst,
    const mlib_u32 *src,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_u64 *pdst64;
	mlib_u64 *psrc64;
	mlib_u64 data;
	mlib_s32 i, shift;

	if (7 & (mlib_addr)src) {
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

		if (n == 0)
			return (MLIB_SUCCESS);
	}

	if (7 & (mlib_addr)dst) {
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
	} else {
		shift = 0;
	}

	psrc64 = (mlib_u64 *)src;
	pdst64 = (mlib_u64 *)dst;

	if (phase != 0) {
		for (i = 0; i < n - 2; i += 2) {
			data = (*psrc64++);
#ifdef _LITTLE_ENDIAN
			pdst64[0] = (data << 32);
			pdst64[1] = (data & (mlib_u64)0xFFFFFFFF00000000ULL);
#else /* _LITTLE_ENDIAN */
			pdst64[0] = (data >> 32);
			pdst64[1] = (data & (mlib_u64)0x00000000FFFFFFFFULL);
#endif /* _LITTLE_ENDIAN */
			pdst64 += 2;
		}

		dst = (mlib_u32 *)pdst64;
		src = (mlib_u32 *)psrc64;

		if (shift) {
			phase = 0;
			(*dst++) = 0;
		}
	} else {
		for (i = 0; i < n - 2; i += 2) {
			data = (*psrc64++);
#ifdef _LITTLE_ENDIAN
			pdst64[0] = (data & (mlib_u64)0x00000000FFFFFFFFULL);
			pdst64[1] = (data >> 32);
#else /* _LITTLE_ENDIAN */
			pdst64[0] = (data & (mlib_u64)0xFFFFFFFF00000000ULL);
			pdst64[1] = (data << 32);
#endif /* _LITTLE_ENDIAN */
			pdst64 += 2;
		}

		dst = (mlib_u32 *)pdst64;
		src = (mlib_u32 *)psrc64;

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

#endif /* _NO_LONGLONG */

/* *********************************************************** */
