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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_SignalMulRectangular.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulRectangular_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulRectangular_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulRectangular_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulRectangular_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulRectangular_S16(mlib_s16 *data,
 *                                                mlib_s32 m,
 *                                                mlib_s32 n);
 *      mlib_status mlib_SignalMulRectangular_S16S(mlib_s16 *data,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_SignalMulRectangular_S16S_S16S(mlib_s16       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalMulRectangular_S16_S16(mlib_s16       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      m        Rectangular window parameter (m <= n)
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        : dst[i] = src[i] * window[i]    i = 0..(n-1)
 *      Stereo      : dst[i] = src[i] * window[i/2]  i = 0..(2*n-1)
 *      Rectangular : window[j] = (j < m) ? 1 : 0    j = 0..(n-1)
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulRectangular_S16 = __mlib_SignalMulRectangular_S16
#pragma weak mlib_SignalMulRectangular_S16S = \
	__mlib_SignalMulRectangular_S16S
#pragma weak mlib_SignalMulRectangular_S16S_S16S = \
	__mlib_SignalMulRectangular_S16S_S16S
#pragma weak mlib_SignalMulRectangular_S16_S16 = \
	__mlib_SignalMulRectangular_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulRectangular_S16) mlib_SignalMulRectangular_S16
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16")));
__typeof__(__mlib_SignalMulRectangular_S16S) mlib_SignalMulRectangular_S16S
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16S")));
__typeof__(__mlib_SignalMulRectangular_S16S_S16S)
    mlib_SignalMulRectangular_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16S_S16S")));
__typeof__(__mlib_SignalMulRectangular_S16_S16)
    mlib_SignalMulRectangular_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulRectangular_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* do not copy by mlib_d64 data type for x86 */
#ifdef i386

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 */

#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

/* *********************************************************** */

#define	MLIB_OFFSET8(ptr)	((8  - ((mlib_addr)ptr & 7)) & 7)
#define	MLIB_OFFSET16(ptr)	(MLIB_OFFSET8(ptr) >> 1)

/* *********************************************************** */

#define	MLIB_OFFSET8_32(ptr)	((8  - ((mlib_addr)ptr & 3)) & 3)
#define	MLIB_OFFSET16_32(ptr)	(MLIB_OFFSET8_32(ptr) >> 1)

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16(
    mlib_s16 *data,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 *pd, d64zero;
	mlib_s32 offset, i, times;
	mlib_s16 s16zero = 0;

/* check for obvious errors */

	if ((data == NULL) || (m < 0) || (n <= 0) || (m > n))
		return (MLIB_FAILURE);

	((mlib_d64 *)&d64zero)[0] = 0;
	data += m;
	m = n - m;
	offset = MLIB_OFFSET16(data);

	if (offset) {
		if (offset > m)
			offset = m;
		for (i = 0; i < offset; i++)
			data[i] = s16zero;
		m -= offset;
	}

	pd = (mlib_d64 *)&data[offset];
	times = m >> 2;
	for (i = 0; i < times; i++)
		pd[i] = d64zero;
	m -= times << 2;
	data = (mlib_s16 *)&pd[times];
	for (i = 0; i < m; i++)
		data[i] = s16zero;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16S(
    mlib_s16 *data,
    mlib_s32 m,
    mlib_s32 n)
{
	return (__mlib_SignalMulRectangular_S16(data, m + m, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 m,
    mlib_s32 n)
{
	TYPE_64BIT *pd, *ps, d64zero;
	mlib_s32 offset, i, times, m2 = n - m;
	mlib_s16 s16zero = 0;

/* check for obvious errors */

	if ((dst == NULL) || (m < 0) || (n <= 0) || (m > n) || (src == NULL))
		return (MLIB_FAILURE);

	((mlib_d64 *)&d64zero)[0] = 0;

	if ((((mlib_addr)dst ^ (mlib_addr)src) & 7) == 0) {
		offset = MLIB_OFFSET16(dst);

		if (offset) {
			if (offset > m)
				offset = m;
			for (i = 0; i < offset; i++)
				dst[i] = src[i];
			m -= offset;
		}

		pd = (TYPE_64BIT *) & dst[offset];
		ps = (TYPE_64BIT *) & src[offset];
		times = m >> 3;
		times <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < times; i += 2) {
			TYPE_64BIT ps1 = ps[i], ps2 = ps[i + 1];

			pd[i] = ps1;
			pd[i + 1] = ps2;
		}

		m -= times << 2;
		dst = (mlib_s16 *)&pd[times];
		src = (mlib_s16 *)&ps[times];
		for (i = 0; i < m; i++)
			dst[i] = src[i];
		dst += m;
		m = m2;
		offset = MLIB_OFFSET16(dst);

		if (offset) {
			if (offset > m)
				offset = m;
			for (i = 0; i < offset; i++)
				dst[i] = s16zero;
			m -= offset;
		}

		pd = (TYPE_64BIT *) & dst[offset];
		times = m >> 2;
		for (i = 0; i < times; i++)
			pd[i] = d64zero;
		m -= times << 2;
		dst = (mlib_s16 *)&pd[times];
		for (i = 0; i < m; i++)
			dst[i] = s16zero;
		return (MLIB_SUCCESS);
	} else {
		mlib_s32 src0, src1, src2;
		mlib_u16 *psrc = (mlib_u16 *)src;

		offset = MLIB_OFFSET16_32(dst);

		if (offset) {
			if (offset > m)
				offset = m;
			for (i = 0; i < offset; i++)
				dst[i] = src[i];
		}

		times = (((m - offset) & ~1) + offset) - 2;

#ifdef _LITTLE_ENDIAN
		src0 = psrc[offset];
		src1 = psrc[offset + 1] << 16;
#else /* _LITTLE_ENDIAN */
		src0 = psrc[offset] << 16;
		src1 = psrc[offset + 1];
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = offset; i < times; i += 2) {
			src2 = src1 | src0;
#ifdef _LITTLE_ENDIAN
			src0 = psrc[i + 2];
			src1 = psrc[i + 3] << 16;
#else /* _LITTLE_ENDIAN */
			src0 = psrc[i + 2] << 16;
			src1 = psrc[i + 3];
#endif /* _LITTLE_ENDIAN */
			*(mlib_s32 *)(dst + i) = src2;
		}

		for (; i < m; i++)
			dst[i] = src[i];
		dst += m;
		m = n - m;
		offset = MLIB_OFFSET16(dst);

		if (offset) {
			if (offset > m)
				offset = m;
			for (i = 0; i < offset; i++)
				dst[i] = s16zero;
		}

		times = ((m - offset) & ~3) + offset;
		for (i = offset; i < times; i += 4) {
			*(TYPE_64BIT *) (dst + i) = d64zero;
		}

		for (i = times; i < m; i++)
			dst[i] = s16zero;
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 m,
    mlib_s32 n)
{
	return (__mlib_SignalMulRectangular_S16_S16(dst, src, m + m, n + n));
}

/* *********************************************************** */
