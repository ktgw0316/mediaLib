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

#pragma ident	"@(#)mlib_c_VectorSplit.c	9.4	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSplit_U8_U8C    - split an 8-bit format vector
 *      mlib_VectorSplit_S8_S8C    - split an 8-bit format vector
 *      mlib_VectorSplit_S16_S16C  - split a 16-bit format vector
 *      mlib_VectorSplit_S32_S32C  - split a 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSplit_S8_S8C(mlib_s8       * r,
 *                                          mlib_s8       * i,
 *                                          const mlib_s8 * x,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorSplit_U8_U8C(mlib_u8       * r,
 *                                          mlib_u8       * i,
 *                                          const mlib_u8 * x,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorSplit_S16_S16C(mlib_s16       * r,
 *                                            mlib_s16       * i,
 *                                            const mlib_s16 * x,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorSplit_S32_S32C(mlib_s32       * r,
 *                                            mlib_s32       * i,
 *                                            const mlib_s32 * x,
 *                                            mlib_s32       n);
 *
 * ARGUMENTS
 *      r    pointer to the first element of the real part.
 *      i    pointer to the first element of the imaginary part.
 *      x    pointer to the first element of the Complex input vector.
 *           x[2*i] holds the real part, and x[2*i + 1] holds
 *           the imaginary parts.
 *      n    number of elements in the vectors.
 *
 * DESCRIPTION
 *
 *      X = R + J*I
 *      X = (r1 + j*i1 r2 + j*i2 ... rn + j*in)
 *
 *      R = (r1 r2 ... rn)
 *      I = (i1 i2 ... in)
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSplit_S8_S8C = __mlib_VectorSplit_S8_S8C
#pragma weak mlib_VectorSplit_S32_S32C = __mlib_VectorSplit_S32_S32C
#pragma weak mlib_VectorSplit_U8_U8C = __mlib_VectorSplit_U8_U8C
#pragma weak mlib_VectorSplit_S16_S16C = __mlib_VectorSplit_S16_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSplit_S8_S8C) mlib_VectorSplit_S8_S8C
	__attribute__((weak, alias("__mlib_VectorSplit_S8_S8C")));
__typeof__(__mlib_VectorSplit_S32_S32C) mlib_VectorSplit_S32_S32C
	__attribute__((weak, alias("__mlib_VectorSplit_S32_S32C")));
__typeof__(__mlib_VectorSplit_U8_U8C) mlib_VectorSplit_U8_U8C
	__attribute__((weak, alias("__mlib_VectorSplit_U8_U8C")));
__typeof__(__mlib_VectorSplit_S16_S16C) mlib_VectorSplit_S16_S16C
	__attribute__((weak, alias("__mlib_VectorSplit_S16_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SPLIT(format, z, n, out_r, out_i)                       \
	{                                                       \
	    format *dst_r = (format *) out_r;                   \
	    format *dst_i = (format *) out_i;                   \
	    format *src = (format *) z;                         \
	    mlib_s32 j;                                         \
	                                                        \
	    for (j = 0; j < n; j++) {                           \
		(*dst_r++) = (*src++);                          \
		(*dst_i++) = (*src++);                          \
	    }                                                   \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN

#define	MERGE_U8_S32(dst32, b1, b2, b3, b4)                         \
	(dst32) = ((b1) << 24) + ((b2) << 16) + ((b3) << 8) + b4;

#else /* _LITTLE_ENDIAN */

#define	MERGE_U8_S32(dst32, b1, b2, b3, b4)                         \
	(dst32) = ((b4) << 24) + ((b3) << 16) + ((b2) << 8) + b1;

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_U8_U8C(
	mlib_u8 *r,
	mlib_u8 *i,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u16 *dp;
	mlib_u32 d0, d1, reg;
	mlib_u16 *dst_r, *dst_i;
	mlib_s32 offset_r, offset_i;
	mlib_s32 j;
	mlib_s32 odd = 0;
	mlib_u8 *tmp;

	if (n < 8) {
		SPLIT(mlib_u8,
			x,
			n,
			r,
			i)
	}

	if ((mlib_addr)x & 1) {
		(*r++) = (*x++);
		odd = 1;
		tmp = (mlib_u8 *)r;
		r = i;
		i = (mlib_u8 *)tmp;
	}

	dp = (mlib_u16 *)x;
	offset_r = (mlib_addr)r & 1;
	offset_i = (mlib_addr)i & 1;

	if (offset_r == offset_i) {

		if (offset_r != 0) {
			d0 = dp[0];
#ifdef _LITTLE_ENDIAN
			(*i++) = (d0 >> 8);
			(*r++) = (d0 & 0xff);
#else /* _LITTLE_ENDIAN */
			(*r++) = (d0 >> 8);
			(*i++) = (d0 & 0xff);
#endif /* _LITTLE_ENDIAN */
			dp++;
			n--;
		}

		dst_r = (mlib_u16 *)r;
		dst_i = (mlib_u16 *)i;

		if (!odd) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < (n / 2); j++) {
				d0 = dp[0];
				d1 = dp[1];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xff) + (d1 << 8);
				*dst_i = (d0 >> 8) + (d1 & ~0xff);
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & (~0xff)) + (d1 >> 8);
				*dst_i = (d0 << 8) + (d1 & 0xff);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
				dst_r += 1;
				dst_i += 1;
			}

			if (n & 1) {
#ifdef _LITTLE_ENDIAN
				*((mlib_u8 *)dst_r) = dp[0] & 0xff;
				*((mlib_u8 *)dst_i) = dp[0] >> 8;
#else /* _LITTLE_ENDIAN */
				*((mlib_u8 *)dst_r) = dp[0] >> 8;
				*((mlib_u8 *)dst_i) = dp[0] & 0xff;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			for (j = 0; j < (n / 2) - 1; j++) {
				d0 = dp[0];
				d1 = dp[1];

#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xff) + (d1 << 8);
				*dst_i = (d0 >> 8) + (d1 & (~0xff));
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & (~0xff)) + (d1 >> 8);
				*dst_i = (d0 << 8) + (d1 & 0xff);
#endif /* _LITTLE_ENDIAN */
				dst_r++;
				dst_i++;
				dp += 2;
			}

			if (n & 1) {
				d0 = dp[0];
				d1 = dp[1];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xff) + (d1 << 8);
				*dst_i = (d1 & ~0xff) + (d0 >> 8);
#else /* _LITTLE_ENDIAN */
				*dst_r = (d1 >> 8) + (d0 & ~0xff);
				*dst_i = (d0 << 8) + (d1 & 0xff);
#endif /* _LITTLE_ENDIAN */
				dst_r++;
				*((mlib_u8 *)dst_r) = *((mlib_u8 *)(dp + 2));
			} else {
				d0 = dp[0];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xff) + (*((mlib_u8 *)(dp +
					1)) << 8);
				*((mlib_u8 *)dst_i) = d0 >> 8;
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & ~0xff) + (*((mlib_u8 *)(dp +
					1)) & 0xff);
				*((mlib_u8 *)dst_i) = d0 & 0xff;
#endif /* _LITTLE_ENDIAN */
			}
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	if ((mlib_addr)r & 1) {
		d0 = *dp;
		dp++;
#ifdef _LITTLE_ENDIAN
		*r = d0 & 0xff;
		*i = d0 >> 8;
#else /* _LITTLE_ENDIAN */
		*r = d0 >> 8;
		*i = d0 & 0xff;
#endif /* _LITTLE_ENDIAN */
		r++;
		i++;
		n--;
	}

	dst_r = (mlib_u16 *)r;
	dst_i = (mlib_u16 *)(i + 1);

	d1 = dp[1];
	d0 = dp[0];
#ifdef _LITTLE_ENDIAN
	*i = d0 >> 8;
	*dst_r = (d0 & 0xff) + (d1 << 8);
	reg = d1 >> 8;
#else /* _LITTLE_ENDIAN */
	*i = d0 & 0xff;
	*dst_r = (d0 & ~0xff) + (d1 >> 8);
	reg = d1 << 8;
#endif /* _LITTLE_ENDIAN */
	dst_r++;
	n -= 2;
	dp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (n / 2 - 1); j++) {
		d0 = dp[0];
		d1 = dp[1];
#ifdef _LITTLE_ENDIAN
		*dst_r = (d0 & 0xff) + (d1 << 8);
		*dst_i = (d0 & ~0xff) + reg;
		reg = (d1 >> 8);
#else /* _LITTLE_ENDIAN */
		*dst_r = (d0 & ~0xff) + (d1 >> 8);
		*dst_i = (d0 & 0xff) + reg;
		reg = d1 << 8;
#endif /* _LITTLE_ENDIAN */
		dp += 2;
		dst_r++;
		dst_i++;
	}

	d0 = dp[0];

	if (n & 1) {
		d1 = dp[1];
		dp += 2;
#ifdef _LITTLE_ENDIAN

		if (odd) {
			(*dst_r++) = (d0 & 0xff) + (d1 << 8);
			*dst_i = (d0 & ~0xff) + reg;
			*((mlib_u8 *)(dst_i + 1)) = d1 >> 8;
			*((mlib_u8 *)dst_r) = *((mlib_u8 *)dp);
		} else {
			(*dst_r++) = (d0 & 0xff) + (d1 << 8);
			(*dst_i++) = (d0 & ~0xff) + reg;
			d0 = dp[0];
			*dst_i = (d1 >> 8) + (d0 & ~0xff);
			*((mlib_u8 *)dst_r) = d0 & 0xff;
		}

#else /* _LITTLE_ENDIAN */

		if (odd) {
			*dst_r = (d0 & ~0xff) + (d1 >> 8);
			dst_r++;
			*dst_i = (d0 & 0xff) + reg;
			dst_i++;
			*((mlib_u8 *)dst_i) = d1 & 0xff;
			*((mlib_u8 *)dst_r) = *((mlib_u8 *)dp);
		} else {
			*dst_r = (d0 & ~0xff) + (d1 >> 8);
			dst_r++;
			*dst_i = (d0 & 0xff) + reg;
			dst_i++;
			d0 = dp[0];
			*dst_i = (d1 << 8) + (d0 & 0xff);
			*((mlib_u8 *)dst_r) = d0 >> 8;
		}

#endif /* _LITTLE_ENDIAN */
	} else {
		dp++;
#ifdef _LITTLE_ENDIAN
		*dst_i = (d0 & ~0xff) + reg;

		if (odd) {
			*dst_r = (*((mlib_u8 *)dp) << 8) + (d0 & 0xff);
		} else {
			d1 = dp[0];
			*dst_r = (d1 << 8) + (d0 & 0xff);
			dst_i++;
			*((mlib_u8 *)dst_i) = d1 >> 8;
		}

#else /* _LITTLE_ENDIAN */
		*dst_i = (d0 & 0xff) + reg;

		if (odd) {
			*dst_r = *((mlib_u8 *)dp) + (d0 & ~0xff);
		} else {
			d1 = dp[0];
			*dst_r = (d1 >> 8) + (d0 & ~0xff);
			dst_i++;
			*((mlib_u8 *)dst_i) = d1;
		}

#endif /* _LITTLE_ENDIAN */
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S8_S8C(
	mlib_s8 *r,
	mlib_s8 *i,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return __mlib_VectorSplit_U8_U8C((mlib_u8 *)r, (mlib_u8 *)i,
		(mlib_u8 *)x, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S16_S16C(
	mlib_s16 *r,
	mlib_s16 *i,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_u32 *dp;
	mlib_u32 d0, d1, reg;
	mlib_u32 *dst_r, *dst_i;
	mlib_s32 offset_r, offset_i;
	mlib_s32 j;
	mlib_u8 odd = 0;
	mlib_s16 *tmp;

	if (n < 8) {
		SPLIT(mlib_s16,
			x,
			n,
			r,
			i)
	}

	if ((mlib_addr)x & 3) {
		(*r++) = (*x++);
		odd = 1;
		tmp = r;
		r = i;
		i = tmp;
	}

	dp = (mlib_u32 *)x;
	offset_r = (mlib_addr)r & 3;
	offset_i = (mlib_addr)i & 3;

	if (offset_r == offset_i) {

		if (offset_r != 0) {
			d0 = dp[0];
#ifdef _LITTLE_ENDIAN
			(*i++) = (d0 >> 16);
			(*r++) = (d0 & 0xffff);
#else /* _LITTLE_ENDIAN */
			(*r++) = (d0 >> 16);
			(*i++) = (d0 & 0xffff);
#endif /* _LITTLE_ENDIAN */
			dp++;
			n--;
		}

		dst_r = (mlib_u32 *)r;
		dst_i = (mlib_u32 *)i;

		if (!odd) {
			for (j = 0; j < (n / 2); j++) {
				d0 = dp[0];
				d1 = dp[1];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffff) + (d1 << 16);
				*dst_i = (d0 >> 16) + (d1 & ~0xffff);
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & (~0xffff)) + (d1 >> 16);
				*dst_i = (d0 << 16) + (d1 & 0xffff);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
				dst_r++;
				dst_i++;
			}

			if (n & 1) {
#ifdef _LITTLE_ENDIAN
				*((mlib_s16 *)dst_r) = dp[0] & 0xffff;
				*((mlib_s16 *)dst_i) = dp[0] >> 16;
#else /* _LITTLE_ENDIAN */
				*((mlib_s16 *)dst_r) = dp[0] >> 16;
				*((mlib_s16 *)dst_i) = dp[0] & 0xffff;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			for (j = 0; j < (n / 2) - 1; j++) {
				d0 = dp[0];
				d1 = dp[1];

#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffff) + (d1 << 16);
				*dst_i = (d0 >> 16) + (d1 & (~0xffff));
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & (~0xffff)) + (d1 >> 16);
				*dst_i = (d0 << 16) + (d1 & 0xffff);
#endif /* _LITTLE_ENDIAN */
				dst_r++;
				dst_i++;
				dp += 2;
			}

			if (n & 1) {
				d0 = dp[0];
				d1 = dp[1];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffff) + (d1 << 16);
				*dst_i = (d1 & ~0xffff) + (d0 >> 16);
#else /* _LITTLE_ENDIAN */
				*dst_r = (d1 >> 16) + (d0 & ~0xffff);
				*dst_i = (d0 << 16) + (d1 & 0xffff);
#endif /* _LITTLE_ENDIAN */
				dst_r++;
				*((mlib_s16 *)dst_r) = *((mlib_s16 *)(dp + 2));
			} else {
				d0 = dp[0];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffff) + (*((mlib_u16 *)(dp +
					1)) << 16);
				*((mlib_s16 *)dst_i) = d0 >> 16;
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & ~0xffff) + (*((mlib_u16 *)(dp +
					1)) & 0xffff);
				*((mlib_s16 *)dst_i) = d0 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			}
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	if ((mlib_addr)r & 3) {
		d0 = *dp;
		dp++;
#ifdef _LITTLE_ENDIAN
		*r = d0 & 0xffff;
		*i = d0 >> 16;
#else /* _LITTLE_ENDIAN */
		*r = d0 >> 16;
		*i = d0 & 0xffff;
#endif /* _LITTLE_ENDIAN */
		r++;
		i++;
		n--;
	}

	dst_r = (mlib_u32 *)r;
	dst_i = (mlib_u32 *)(i + 1);

	d1 = dp[1];
	d0 = dp[0];
#ifdef _LITTLE_ENDIAN
	*i = d0 >> 16;
	*dst_r = (d0 & 0xffff) + (d1 << 16);
	reg = d1 >> 16;
#else /* _LITTLE_ENDIAN */
	*i = d0 & 0xffff;
	*dst_r = (d0 & ~0xffff) + (d1 >> 16);
	reg = d1 << 16;
#endif /* _LITTLE_ENDIAN */
	dst_r++;
	n -= 2;
	dp += 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (n / 2 - 1); j++) {
		d0 = dp[0];
		d1 = dp[1];
#ifdef _LITTLE_ENDIAN
		*dst_r = (d0 & 0xffff) + (d1 << 16);
		*dst_i = (d0 & ~0xffff) + reg;
		reg = (d1 >> 16);
#else /* _LITTLE_ENDIAN */
		*dst_r = (d0 & ~0xffff) + (d1 >> 16);
		*dst_i = (d0 & 0xffff) + reg;
		reg = d1 << 16;
#endif /* _LITTLE_ENDIAN */
		dp += 2;
		dst_r++;
		dst_i++;
	}

	d0 = dp[0];

	if (n & 1) {
		d1 = dp[1];
		dp += 2;
#ifdef _LITTLE_ENDIAN

		if (odd) {
			(*dst_r++) = (d0 & 0xffff) + (d1 << 16);
			*dst_i = (d0 & ~0xffff) + reg;
			*((mlib_s16 *)(dst_i + 1)) = d1 >> 16;
			*((mlib_s16 *)dst_r) = *((mlib_s16 *)dp);
		} else {
			(*dst_r++) = (d0 & 0xffff) + (d1 << 16);
			(*dst_i++) = (d0 & ~0xffff) + reg;
			d0 = dp[0];
			*dst_i = (d1 >> 16) + (d0 & ~0xffff);
			*((mlib_s16 *)dst_r) = d0 & 0xffff;
		}

#else /* _LITTLE_ENDIAN */

		if (odd) {
			*dst_r = (d0 & ~0xffff) + (d1 >> 16);
			dst_r++;
			*dst_i = (d0 & 0xffff) + reg;
			dst_i++;
			*((mlib_s16 *)dst_i) = d1 & 0xffff;
			*((mlib_s16 *)dst_r) = *((mlib_s16 *)dp);
		} else {
			*dst_r = (d0 & ~0xffff) + (d1 >> 16);
			dst_r++;
			*dst_i = (d0 & 0xffff) + reg;
			dst_i++;
			d0 = dp[0];
			*dst_i = (d1 << 16) + (d0 & 0xffff);
			*((mlib_s16 *)dst_r) = d0 >> 16;
		}

#endif /* _LITTLE_ENDIAN */
	} else {
		dp++;
#ifdef _LITTLE_ENDIAN
		*dst_i = (d0 & ~0xffff) + reg;

		if (odd) {
			*dst_r = (*((mlib_u16 *)dp) << 16) + (d0 & 0xffff);
		} else {
			d1 = dp[0];
			*dst_r = (d1 << 16) + (d0 & 0xffff);
			dst_i++;
			*((mlib_s16 *)dst_i) = d1 >> 16;
		}

#else /* _LITTLE_ENDIAN */
		*dst_i = (d0 & 0xffff) + reg;

		if (odd) {
			*dst_r = *((mlib_u16 *)dp) + (d0 & ~0xffff);
		} else {
			d1 = dp[0];
			*dst_r = (d1 >> 16) + (d0 & ~0xffff);
			dst_i++;
			*((mlib_s16 *)dst_i) = d1;
		}

#endif /* _LITTLE_ENDIAN */
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S32_S32C(
	mlib_s32 *r,
	mlib_s32 *i,
	const mlib_s32 *x,
	mlib_s32 n)
{
#ifndef _LP64
/* instead of _NO_LONGLONG - performance degraded for v8p version */
	SPLIT(mlib_s32,
		x,
		n,
		r,
		i)
#else /* _LP64 */
	mlib_u64 *dp;
	mlib_u64 d0, d1, reg;
	mlib_u64 *dst_r, *dst_i;
	mlib_s32 offset_r, offset_i;
	mlib_s32 j;
	mlib_u8 odd = 0;
	mlib_s32 *tmp;

	if (n < 6) {
		SPLIT(mlib_s32,
			x,
			n,
			r,
			i)
	}

	if ((mlib_addr)x & 7) {
		(*r++) = (*x++);
		odd = 1;
		tmp = r;
		r = i;
		i = tmp;
	}

	dp = (mlib_u64 *)x;
	offset_r = (mlib_addr)r & 7;
	offset_i = (mlib_addr)i & 7;

	if (offset_r == offset_i) {

		if (offset_r != 0) {
			d0 = dp[0];
#ifdef _LITTLE_ENDIAN
			(*i++) = (d0 >> 32);
			(*r++) = (d0 & 0xffffffffL);
#else /* _LITTLE_ENDIAN */
			(*r++) = (d0 >> 32);
			(*i++) = (d0 & 0xffffffffL);
#endif /* _LITTLE_ENDIAN */
			dp++;
			n--;
		}

		dst_r = (mlib_u64 *)r;
		dst_i = (mlib_u64 *)i;

		if (!odd) {
			for (j = 0; j < (n / 2); j++) {
				d0 = dp[0];
				d1 = dp[1];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d1 << 32) + (d0 & 0xffffffffL);
				*dst_i = (d1 & (~0xffffffffL)) + (d0 >> 32);
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & (~0xffffffffL)) + (d1 >> 32);
				*dst_i = (d0 << 32) + (d1 & 0xffffffffL);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
				dst_r++;
				dst_i++;
			}

			if (n & 1) {
#ifdef _LITTLE_ENDIAN
				*((mlib_s32 *)dst_r) = dp[0] & 0xffffffffL;
				*((mlib_s32 *)dst_i) = dp[0] >> 32;
#else /* _LITTLE_ENDIAN */
				*((mlib_s32 *)dst_r) = dp[0] >> 32;
				*((mlib_s32 *)dst_i) = dp[0] & 0xffffffffL;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			for (j = 0; j < (n / 2) - 1; j++) {
				d0 = dp[0];
				d1 = dp[1];

#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffffffffL) + (d1 << 32);
				*dst_i = (d0 >> 32) + (d1 & (~0xffffffffL));
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & (~0xffffffffL)) + (d1 >> 32);
				*dst_i = (d0 << 32) + (d1 & 0xffffffffL);
#endif /* _LITTLE_ENDIAN */
				dst_r++;
				dst_i++;
				dp += 2;
			}

			if (n & 1) {
				d0 = dp[0];
				d1 = dp[1];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffffffffL) + (d1 << 32);
				*dst_i = (d1 & ~0xffffffffL) + (d0 >> 32);
#else /* _LITTLE_ENDIAN */
				*dst_r = (d1 >> 32) + (d0 & ~0xffffffffL);
				*dst_i = (d0 << 32) + (d1 & 0xffffffffL);
#endif /* _LITTLE_ENDIAN */
				dst_r++;
				*((mlib_s32 *)dst_r) = *((mlib_s32 *)(dp + 2));
			} else {
				d0 = dp[0];
#ifdef _LITTLE_ENDIAN
				*dst_r = (d0 & 0xffffffffL) +
					((*(dp + 1)) << 32);
				*((mlib_s32 *)dst_i) = d0 >> 32;
#else /* _LITTLE_ENDIAN */
				*dst_r = (d0 & ~0xffffffffL) +
					(*((mlib_u32 *)(dp + 1)) & 0xffffffffL);
				*((mlib_s32 *)dst_i) = d0 & 0xffffffffL;
#endif /* _LITTLE_ENDIAN */
			}
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	if ((mlib_addr)r & 7) {
		d0 = *dp;
		dp++;
#ifdef _LITTLE_ENDIAN
		*r = d0 & 0xffffffffL;
		*i = d0 >> 32;
#else /* _LITTLE_ENDIAN */
		*r = d0 >> 32;
		*i = d0 & 0xffffffffL;
#endif /* _LITTLE_ENDIAN */
		r++;
		i++;
		n--;
	}

	dst_r = (mlib_u64 *)r;
	dst_i = (mlib_u64 *)(i + 1);

	d1 = dp[1];
	d0 = dp[0];
#ifdef _LITTLE_ENDIAN
	*i = d0 >> 32;
	*dst_r = (d0 & 0xffffffffL) + (d1 << 32);
	reg = d1 >> 32;
#else /* _LITTLE_ENDIAN */
	*i = d0 & 0xffffffffL;
	*dst_r = (d0 & ~0xffffffffL) + (d1 >> 32);
	reg = d1 << 32;
#endif /* _LITTLE_ENDIAN */
	dst_r++;
	n -= 2;
	dp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (n / 2 - 1); j++) {
		d0 = dp[0];
		d1 = dp[1];
#ifdef _LITTLE_ENDIAN
		*dst_r = (d0 & 0xffffffffL) + (d1 << 32);
		*dst_i = (d0 & ~0xffffffffL) + reg;
		reg = (d1 >> 32);
#else /* _LITTLE_ENDIAN */
		*dst_r = (d0 & ~0xffffffffL) + (d1 >> 32);
		*dst_i = (d0 & 0xffffffffL) + reg;
		reg = d1 << 32;
#endif /* _LITTLE_ENDIAN */
		dp += 2;
		dst_r++;
		dst_i++;
	}

	d0 = dp[0];

	if (n & 1) {
		d1 = dp[1];
		dp += 2;
#ifdef _LITTLE_ENDIAN

		if (odd) {
			(*dst_r++) = (d0 & 0xffffffffL) + (d1 << 32);
			*dst_i = (d0 & ~0xffffffffL) + reg;
			*((mlib_s32 *)(dst_i + 1)) = d1 >> 32;
			*((mlib_s32 *)dst_r) = *((mlib_s32 *)dp);
		} else {
			(*dst_r++) = (d0 & 0xffffffffL) + (d1 << 32);
			(*dst_i++) = (d0 & ~0xffffffffL) + reg;
			d0 = dp[0];
			*dst_i = (d1 >> 32) + (d0 & ~0xffffffffL);
			*((mlib_s32 *)dst_r) = d0 & 0xffffffffL;
		}

#else /* _LITTLE_ENDIAN */

		if (odd) {
			*dst_r = (d0 & ~0xffffffffL) + (d1 >> 32);
			dst_r++;
			*dst_i = (d0 & 0xffffffffL) + reg;
			dst_i++;
			*((mlib_s32 *)dst_i) = d1 & 0xffffffffL;
			*((mlib_s32 *)dst_r) = *((mlib_s32 *)dp);
		} else {
			*dst_r = (d0 & ~0xffffffffL) + (d1 >> 32);
			dst_r++;
			*dst_i = (d0 & 0xffffffffL) + reg;
			dst_i++;
			d0 = dp[0];
			*dst_i = (d1 << 32) + (d0 & 0xffffffffL);
			*((mlib_s32 *)dst_r) = d0 >> 32;
		}

#endif /* _LITTLE_ENDIAN */
	} else {
		dp++;
#ifdef _LITTLE_ENDIAN
		*dst_i = (d0 & ~0xffffffffL) + reg;

		if (odd) {
			*dst_r = (((mlib_u64)(*((mlib_u32 *)dp))) << 32) +
				(d0 & 0xffffffffL);
		} else {
			d1 = dp[0];
			*dst_r = (d1 << 32) + (d0 & 0xffffffffL);
			dst_i++;
			*((mlib_s32 *)dst_i) = d1 >> 32;
		}

#else /* _LITTLE_ENDIAN */
		*dst_i = (d0 & 0xffffffffL) + reg;

		if (odd) {
			*dst_r = *((mlib_u32 *)dp) + (d0 & ~0xffffffffL);
		} else {
			d1 = dp[0];
			*dst_r = (d1 >> 32) + (d0 & ~0xffffffffL);
			dst_i++;
			*((mlib_s32 *)dst_i) = d1;
		}

#endif /* _LITTLE_ENDIAN */
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* _LP64 */
}

/* *********************************************************** */
