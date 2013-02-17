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

#pragma ident	"@(#)mlib_c_VectorMerge.c	9.5	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMerge_U8C_U8    - merge two  8-bit format vectors
 *      mlib_VectorMerge_S8C_S8    - merge two  8-bit format vectors
 *      mlib_VectorMerge_S16C_S16  - merge two 16-bit format vectors
 *      mlib_VectorMerge_S32C_S32  - merge two 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMerge_U8C_U8(mlib_u8       *z,
 *                                          const mlib_u8 *r,
 *                                          const mlib_u8 *i,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorMerge_S8C_S8(mlib_s8       *z,
 *                                          const mlib_s8 *r,
 *                                          const mlib_s8 *i,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorMerge_S16C_S16(mlib_s16       *z,
 *                                            const mlib_s16 *r,
 *                                            const mlib_s16 *i,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorMerge_S32C_S32(mlib_s32       *z,
 *                                            const mlib_s32 *r,
 *                                            const mlib_s32 *i,
 *                                            mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the resultant vector
 *      r    pointer to the first element of the 1st input vector (real parts)
 *      i    pointer to the first element of the 2nd input vector (imag. parts)
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *
 *      Z = R + J*I
 *
 *      R = (r1 r2 ... rn)
 *      I = (i1 i2 ... in)
 *      Z = (r1 + j*i1 r2 + j*i2 ... rn + j*in)
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMerge_U8C_U8 = __mlib_VectorMerge_U8C_U8
#pragma weak mlib_VectorMerge_S16C_S16 = __mlib_VectorMerge_S16C_S16
#pragma weak mlib_VectorMerge_S8C_S8 = __mlib_VectorMerge_U8C_U8
#pragma weak mlib_VectorMerge_S32C_S32 = __mlib_VectorMerge_S32C_S32
#pragma weak __mlib_VectorMerge_S8C_S8 = __mlib_VectorMerge_U8C_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMerge_U8C_U8) mlib_VectorMerge_U8C_U8
	__attribute__((weak, alias("__mlib_VectorMerge_U8C_U8")));
__typeof__(__mlib_VectorMerge_S16C_S16) mlib_VectorMerge_S16C_S16
	__attribute__((weak, alias("__mlib_VectorMerge_S16C_S16")));
__typeof__(__mlib_VectorMerge_U8C_U8) mlib_VectorMerge_S8C_S8
	__attribute__((weak, alias("__mlib_VectorMerge_U8C_U8")));
__typeof__(__mlib_VectorMerge_S32C_S32) mlib_VectorMerge_S32C_S32
	__attribute__((weak, alias("__mlib_VectorMerge_S32C_S32")));
/*
 * __typeof__(__mlib_VectorMerge_U8C_U8) __mlib_VectorMerge_S8C_S8
 *	__attribute__((weak, alias("__mlib_VectorMerge_U8C_U8")));
 */

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#if defined(__GNUC__) || defined(_MSC_VER)

mlib_status __mlib_VectorMerge_S8C_S8(
    mlib_s8 *z,
    const mlib_s8 *r,
    const mlib_s8 *i,
    mlib_s32 n)
{
	return (__mlib_VectorMerge_U8C_U8(z, r, i, n));
}

#endif /* defined(__GNUC__) || defined(_MSC_VER) */

/* *********************************************************** */

#define	MERGE(format, inp_r, inp_i, n, z)                       \
	format *src_r = (format *) inp_r;                       \
	format *src_i = (format *) inp_i;                       \
	format *dst = (format *) z;                             \
	mlib_s32 k;                                             \
	                                                        \
	for (k = 0; k < n; k++) {                               \
	    (*dst++) = (*src_r++);                              \
	    (*dst++) = (*src_i++);                              \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_VectorMerge_U8C_U8(
	mlib_u8 *z,
	const mlib_u8 *r,
	const mlib_u8 *i,
	mlib_s32 n)
{
	mlib_u16 *dp;
	mlib_u16 s0, s1, s2;
	mlib_u16 *sp0, *sp1;
	mlib_s32 offset_r, offset_i;
	mlib_s32 j;
	mlib_u8 odd = 0;
	const mlib_u8 *tmp;

	if (n < 6) {
		MERGE(mlib_u8, r, i, n, z);
	}

	if ((mlib_addr)z & 1) {
		(*z++) = (*r++);
		odd = 1;
		tmp = r;
		r = i;
		i = tmp;
	}

	dp = (mlib_u16 *)z;
	offset_r = (mlib_addr)r & 1;
	offset_i = (mlib_addr)i & 1;

	if (offset_r == offset_i) {

		if (offset_r != 0) {
#ifdef _LITTLE_ENDIAN
			(*dp++) = ((mlib_s32)((*i++)) << 8) + (((*r++)) & 0xff);
#else /* _LITTLE_ENDIAN */
			(*dp++) = ((mlib_s32)((*r++)) << 8) + (((*i++)) & 0xff);
#endif /* _LITTLE_ENDIAN */
			n--;
		}

		sp0 = (mlib_u16 *)r;
		sp1 = (mlib_u16 *)i;

		if (!odd) {
			for (j = 0; j < (n / 2); j++) {
				s0 = (*sp0++);
				s1 = (*sp1++);
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xff) + (s1 << 8);
				dp[1] = (s0 >> 8) + (s1 & ~0xff);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 8) + (s1 & 0xff);
				dp[0] = (s0 & (~0xff)) + (s1 >> 8);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
			}

			if (n & 1) {
#ifdef _LITTLE_ENDIAN
				dp[0] = (*((mlib_u8 *)sp0) & 0xff) +
					(*((mlib_u8 *)sp1) << 8);
#else /* _LITTLE_ENDIAN */
				dp[0] = (mlib_u16)(*((mlib_u8 *)sp0) << 8) +
					(mlib_u16)*((mlib_u8 *)sp1);
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			for (j = 0; j < (n / 2) - 1; j++) {
				s0 = (*sp0++);
				s1 = (*sp1++);

#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xff) + (s1 << 8);
				dp[1] = (s0 >> 8) + (s1 & ~0xff);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 8) + (s1 & 0xff);
				dp[0] = (s0 & (~0xff)) + (s1 >> 8);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
			}

			if (n & 1) {
				s0 = (*sp0++);
				s1 = *sp1;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xff) + (s1 << 8);
				dp[1] = (s0 >> 8) + (s1 & ~0xff);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 8) + (s1 & 0xff);
				dp[0] = (s0 & ~0xff) + (s1 >> 8);
#endif /* _LITTLE_ENDIAN */
				*((mlib_u8 *)(dp + 2)) = *((mlib_u8 *)sp0);
			} else {
				s0 = *sp0;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xff) + (*((mlib_u8 *)sp1) << 8);
				*((mlib_u8 *)(dp + 1)) = s0 >> 8;
#else /* _LITTLE_ENDIAN */
				dp[0] = (s0 & ~0xff) +
					(*((mlib_u8 *)sp1) & 0xff);
				*((mlib_u8 *)(dp + 1)) = s0 & 0xff;
#endif /* _LITTLE_ENDIAN */
			}
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	if ((mlib_addr)r & 1) {
#ifdef _LITTLE_ENDIAN
		(*dp++) = ((mlib_u16)((*i++)) << 8) + (((*r++)) & 0xff);
#else /* _LITTLE_ENDIAN */
		(*dp++) = ((mlib_u16)((*r++)) << 8) + (((*i++)) & 0xff);
#endif /* _LITTLE_ENDIAN */
		n--;
	}

	sp0 = (mlib_u16 *)r;
	sp1 = (mlib_u16 *)(i + 1);

	s1 = (*sp1++);
	s0 = (*sp0++);
#ifdef _LITTLE_ENDIAN
	dp[0] = ((*i) << 8) + (s0 & 0xff);
	dp[1] = (s1 << 8) + (s0 >> 8);
#else /* _LITTLE_ENDIAN */
	dp[0] = (s0 & ~0xff) + ((*i) & 0xff);
	dp[1] = (s0 << 8) + (s1 >> 8);
#endif /* _LITTLE_ENDIAN */

	n -= 2;

	dp += 2;
	for (j = 0; j < ((n) / 2 - 1); j++) {
		s0 = (*sp0++);
		s2 = (*sp1++);
#ifdef _LITTLE_ENDIAN
		dp[0] = (s1 & ~0xff) + (s0 & 0xff);
		dp[1] = (s0 >> 8) + (s2 << 8);
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 & ~0xff) + (s1 & 0xff);
		dp[1] = (s0 << 8) + (s2 >> 8);
#endif /* _LITTLE_ENDIAN */
		s1 = s2;
		dp += 2;
	}

	if (n & 1) {
#ifdef _LITTLE_ENDIAN
		s0 = (*sp0++);

		if (odd) {
			(*dp++) = (s0 & 0xff) + (s1 & ~0xff);
			(*dp++) = (s0 >> 8) + ((*((mlib_u8 *)sp1)) << 8);
			*((mlib_u8 *)dp) = *((mlib_u8 *)sp0);
		} else {
			(*dp++) = (s0 & 0xff) + (s1 & ~0xff);
			s1 = *sp1;
			(*dp++) = (s0 >> 8) + (s1 << 8);
			dp[0] = (s1 & ~0xff) + *((mlib_u8 *)sp0);
		}

#else /* _LITTLE_ENDIAN */
		s0 = (*sp0++);

		if (odd) {
			(*dp++) = (s0 & ~0xff) + (s1 & 0xff);
			(*dp++) = (s0 << 8) + *((mlib_u8 *)sp1);
			*((mlib_u8 *)dp) = *((mlib_u8 *)sp0);
		} else {
			(*dp++) = (s0 & ~0xff) + (s1 & 0xff);
			s1 = *sp1;
			(*dp++) = (s0 << 8) + (s1 >> 8);
			dp[0] = ((*((mlib_u8 *)sp0)) << 8) + (s1 & 0xff);
		}

#endif /* _LITTLE_ENDIAN */
	} else {
		s0 = *sp0;
#ifdef _LITTLE_ENDIAN
		(*dp++) = (s1 & ~0xff) + (s0 & 0xff);

		if (odd) {
			*((mlib_u8 *)dp) = (s0 >> 8);
		} else {
			*dp = (s0 >> 8) + (*((mlib_u8 *)sp1) << 8);
		}

#else /* _LITTLE_ENDIAN */
		(*dp++) = (s0 & ~0xff) + (s1 & 0xff);

		if (odd) {
			*((mlib_u8 *)dp) = (s0);
		} else {
			*dp = (s0 << 8) + *((mlib_u8 *)sp1);
		}

#endif /* _LITTLE_ENDIAN */
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMerge_S16C_S16(
	mlib_s16 *z,
	const mlib_s16 *r,
	const mlib_s16 *i,
	mlib_s32 n)
{
	mlib_u32 *dp;
	mlib_u32 s0, s1, s2;
	mlib_u32 *sp0, *sp1;
	mlib_s32 offset_r, offset_i;
	mlib_s32 j;
	mlib_u8 odd = 0;
	const mlib_s16 *tmp;

	if (n < 6) {
		MERGE(mlib_s16, r, i, n, z);
	}

	if ((mlib_addr)z & 3) {
		(*z++) = (*r++);
		odd = 1;
		tmp = r;
		r = i;
		i = tmp;
	}

	dp = (mlib_u32 *)z;
	offset_r = (mlib_addr)r & 3;
	offset_i = (mlib_addr)i & 3;

	if (offset_r == offset_i) {

		if (offset_r != 0) {
#ifdef _LITTLE_ENDIAN
			(*dp++) =
				((mlib_s32)((*i++)) << 16) +
				(((*r++)) & 0xffff);
#else /* _LITTLE_ENDIAN */
			(*dp++) =
				((mlib_s32)((*r++)) << 16) +
				(((*i++)) & 0xffff);
#endif /* _LITTLE_ENDIAN */
			n--;
		}

		sp0 = (mlib_u32 *)r;
		sp1 = (mlib_u32 *)i;

		if (!odd) {
			for (j = 0; j < (n / 2); j++) {
				s0 = (*sp0++);
				s1 = (*sp1++);
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffff) + (s1 << 16);
				dp[1] = (s0 >> 16) + (s1 & ~0xffff);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 16) + (s1 & 0xffff);
				dp[0] = (s0 & (~0xffff)) + (s1 >> 16);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
			}

			if (n & 1) {
#ifdef _LITTLE_ENDIAN
				dp[0] = (*((mlib_u16 *)sp0) & 0xffff) +
					(*((mlib_u16 *)sp1) << 16);
#else /* _LITTLE_ENDIAN */
				dp[0] = (mlib_u32)(*((mlib_u16 *)sp0) << 16) +
					(mlib_u32)*((mlib_u16 *)sp1);
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			for (j = 0; j < (n / 2) - 1; j++) {
				s0 = (*sp0++);
				s1 = (*sp1++);

#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffff) + (s1 << 16);
				dp[1] = (s0 >> 16) + (s1 & ~0xffff);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 16) + (s1 & 0xffff);
				dp[0] = (s0 & (~0xffff)) + (s1 >> 16);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
			}

			if (n & 1) {
				s0 = (*sp0++);
				s1 = *sp1;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffff) + (s1 << 16);
				dp[1] = (s0 >> 16) + (s1 & ~0xffff);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 16) + (s1 & 0xffff);
				dp[0] = (s0 & ~0xffff) + (s1 >> 16);
#endif /* _LITTLE_ENDIAN */
				*((mlib_s16 *)(dp + 2)) = *((mlib_s16 *)sp0);
			} else {
				s0 = *sp0;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffff) +
					(*((mlib_s16 *)sp1) << 16);
				*((mlib_s16 *)(dp + 1)) = s0 >> 16;
#else /* _LITTLE_ENDIAN */
				dp[0] = (s0 & ~0xffff) +
					(*((mlib_s16 *)sp1) & 0xffff);
				*((mlib_s16 *)(dp + 1)) = s0 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			}
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	if ((mlib_addr)r & 3) {
#ifdef _LITTLE_ENDIAN
		(*dp++) = ((mlib_u32)((*i++)) << 16) + (((*r++)) & 0xffff);
#else /* _LITTLE_ENDIAN */
		(*dp++) = ((mlib_u32)((*r++)) << 16) + (((*i++)) & 0xffff);
#endif /* _LITTLE_ENDIAN */
		n--;
	}

	sp0 = (mlib_u32 *)r;
	sp1 = (mlib_u32 *)(i + 1);

	s1 = (*sp1++);
	s0 = (*sp0++);
#ifdef _LITTLE_ENDIAN
	dp[0] = ((*i) << 16) + (s0 & 0xffff);
	dp[1] = (s1 << 16) + (s0 >> 16);
#else /* _LITTLE_ENDIAN */
	dp[0] = (s0 & ~0xffff) + ((*i) & 0xffff);
	dp[1] = (s0 << 16) + (s1 >> 16);
#endif /* _LITTLE_ENDIAN */

	n -= 2;

	dp += 2;
	for (j = 0; j < ((n) / 2 - 1); j++) {
		s0 = (*sp0++);
		s2 = (*sp1++);
#ifdef _LITTLE_ENDIAN
		dp[0] = (s1 & ~0xffff) + (s0 & 0xffff);
		dp[1] = (s0 >> 16) + (s2 << 16);
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 & ~0xffff) + (s1 & 0xffff);
		dp[1] = (s0 << 16) + (s2 >> 16);
#endif /* _LITTLE_ENDIAN */
		s1 = s2;
		dp += 2;
	}

	if (n & 1) {
#ifdef _LITTLE_ENDIAN
		s0 = (*sp0++);

		if (odd) {
			(*dp++) = (s0 & 0xffff) + (s1 & ~0xffff);
			(*dp++) = (s0 >> 16) + ((*((mlib_s16 *)sp1)) << 16);
			*((mlib_s16 *)dp) = *((mlib_s16 *)sp0);
		} else {
			(*dp++) = (s0 & 0xffff) + (s1 & ~0xffff);
			s1 = *sp1;
			(*dp++) = (s0 >> 16) + (s1 << 16);
			dp[0] = (s1 & ~0xffff) + *((mlib_u16 *)sp0);
		}

#else /* _LITTLE_ENDIAN */
		s0 = (*sp0++);

		if (odd) {
			(*dp++) = (s0 & ~0xffff) + (s1 & 0xffff);
			(*dp++) = (s0 << 16) + *((mlib_u16 *)sp1);
			*((mlib_s16 *)dp) = *((mlib_s16 *)sp0);
		} else {
			(*dp++) = (s0 & ~0xffff) + (s1 & 0xffff);
			s1 = *sp1;
			(*dp++) = (s0 << 16) + (s1 >> 16);
			dp[0] = ((*((mlib_s16 *)sp0)) << 16) + (s1 & 0xffff);
		}

#endif /* _LITTLE_ENDIAN */
	} else {
		s0 = *sp0;
#ifdef _LITTLE_ENDIAN
		(*dp++) = (s1 & ~0xffff) + (s0 & 0xffff);

		if (odd) {
			*((mlib_s16 *)dp) = (s0 >> 16);
		} else {
			*dp = (s0 >> 16) + (*((mlib_u16 *)sp1) << 16);
		}

#else /* _LITTLE_ENDIAN */
		(*dp++) = (s0 & ~0xffff) + (s1 & 0xffff);

		if (odd) {
			*((mlib_s16 *)dp) = (s0);
		} else {
			*dp = (s0 << 16) + *((mlib_u16 *)sp1);
		}

#endif /* _LITTLE_ENDIAN */
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMerge_S32C_S32(
	mlib_s32 *z,
	const mlib_s32 *r,
	const mlib_s32 *i,
	mlib_s32 n)
{
#ifndef _LP64

/*
 * instead of _NO_LONGLONG - performance degraded for v8p version
 */
	MERGE(mlib_u32, r, i, n, z);

#else /* _LP64 */
	mlib_u64 *dp;
	mlib_u64 s0, s1, s2;
	mlib_u64 *sp0, *sp1;
	mlib_s32 offset_r, offset_i;
	mlib_s32 j;
	mlib_u8 odd = 0;
	const mlib_s32 *tmp;

	if (n < 6) {
		MERGE(mlib_s32, r, i, n, z);
	}

	if ((mlib_addr)z & 7) {
		(*z++) = (*r++);
		odd = 1;
		tmp = r;
		r = i;
		i = tmp;
	}

	dp = (mlib_u64 *)z;
	offset_r = (mlib_addr)r & 7;
	offset_i = (mlib_addr)i & 7;

	if (offset_r == offset_i) {

		if (offset_r != 0) {
#ifdef _LITTLE_ENDIAN
			(*dp++) =
				((mlib_u64)((*i++)) << 32) +
				(((*r++)) & 0xffffffffLL);
#else /* _LITTLE_ENDIAN */
			(*dp++) =
				((mlib_u64)((*r++)) << 32) +
				(((*i++)) & 0xffffffffLL);
#endif /* _LITTLE_ENDIAN */
			n--;
		}

		sp0 = (mlib_u64 *)r;
		sp1 = (mlib_u64 *)i;

		if (!odd) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < (n / 2); j++) {
				s0 = (*sp0++);
				s1 = (*sp1++);
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffffffffLL) + (s1 << 32);
				dp[1] = (s0 >> 32) + (s1 & ~0xffffffffLL);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 32) + (s1 & 0xffffffffLL);
				dp[0] = (s0 & ~0xffffffffLL) + (s1 >> 32);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
			}

			if (n & 1) {
#ifdef _LITTLE_ENDIAN
				dp[0] = (((mlib_u64)*((mlib_u32 *)sp1)) << 32) +
					(mlib_u64)*((mlib_u32 *)sp0);
#else /* _LITTLE_ENDIAN */
				dp[0] = (((mlib_u64)*((mlib_u32 *)sp0)) << 32) +
					(mlib_u64)*((mlib_u32 *)sp1);
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			for (j = 0; j < (n / 2) - 1; j++) {
				s0 = (*sp0++);
				s1 = (*sp1++);
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffffffffLL) + (s1 << 32);
				dp[1] = (s0 >> 32) + (s1 & ~0xffffffffLL);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 32) + (s1 & 0xffffffffLL);
				dp[0] = (s0 & (~0xffffffffLL)) + (s1 >> 32);
#endif /* _LITTLE_ENDIAN */
				dp += 2;
			}

			if (n & 1) {
				s0 = (*sp0++);
				s1 = *sp1;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffffffffLL) + (s1 << 32);
				dp[1] = (s0 >> 32) + (s1 & ~0xffffffffLL);
#else /* _LITTLE_ENDIAN */
				dp[1] = (s0 << 32) + (s1 & 0xffffffffLL);
				dp[0] = (s0 & ~0xffffffffLL) + (s1 >> 32);
#endif /* _LITTLE_ENDIAN */
				*((mlib_s32 *)(dp + 2)) = *((mlib_s32 *)sp0);
			} else {
				s0 = *sp0;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & 0xffffffffLL) +
					(((mlib_u64)*((mlib_u32 *)sp1)) << 32);
				*((mlib_s32 *)(dp + 1)) = s0 >> 32;
#else /* _LITTLE_ENDIAN */
				dp[0] = (s0 & ~0xffffffffLL) +
					(((mlib_u64)*((mlib_u32 *)sp1)) &
					0xffffffffLL);
				*((mlib_s32 *)(dp + 1)) = s0 & 0xffffffffLL;
#endif /* _LITTLE_ENDIAN */
			}
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	if ((mlib_addr)r & 7) {
#ifdef _LITTLE_ENDIAN
		(*dp++) =
			((mlib_u64)((*i++)) << 32) +
			((mlib_u64)((*r++)) & 0xffffffffLL);
#else /* _LITTLE_ENDIAN */
		(*dp++) =
			((mlib_u64)((*r++)) << 32) +
			((mlib_u64)((*i++)) & 0xffffffffLL);
#endif /* _LITTLE_ENDIAN */
		n--;
	}

	sp0 = (mlib_u64 *)r;
	sp1 = (mlib_u64 *)(i + 1);

	s1 = (*sp1++);
	s0 = (*sp0++);
#ifdef _LITTLE_ENDIAN
	dp[0] = (((mlib_u64)*((mlib_u32 *)i)) << 32) + (s0 & 0xffffffffLL);
	dp[1] = (s1 << 32) + (s0 >> 32);
#else /* _LITTLE_ENDIAN */
	dp[0] = (s0 & ~0xffffffffLL) + ((*i) & 0xffffffffLL);
	dp[1] = (s0 << 32) + (s1 >> 32);
#endif /* _LITTLE_ENDIAN */

	n -= 2;

	dp += 2;
	for (j = 0; j < ((n) / 2 - 1); j++) {
		s0 = (*sp0++);
		s2 = (*sp1++);
#ifdef _LITTLE_ENDIAN
		dp[0] = (s1 & ~0xffffffffLL) + (s0 & 0xffffffffLL);
		dp[1] = (s2 << 32) + (s0 >> 32);
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 & ~0xffffffffLL) + (s1 & 0xffffffffLL);
		dp[1] = (s0 << 32) + (s2 >> 32);
#endif /* _LITTLE_ENDIAN */
		s1 = s2;
		dp += 2;
	}

	if (n & 1) {
#ifdef _LITTLE_ENDIAN
		s0 = (*sp0++);

		if (odd) {
			(*dp++) = (s0 & 0xffffffffLL) + (s1 & ~0xffffffffLL);
			(*dp++) =
				(s0 >> 32) +
				(((mlib_u64)*((mlib_u32 *)sp1)) << 32);
			*((mlib_s32 *)dp) = *((mlib_s32 *)sp0);
		} else {
			(*dp++) = (s0 & 0xffffffffLL) + (s1 & ~0xffffffffLL);
			s1 = *sp1;
			(*dp++) = (s0 >> 32) + (s1 << 32);
			dp[0] = (s1 & ~0xffffffffLL) +
				(mlib_u64)*((mlib_u32 *)sp0);
		}

#else /* _LITTLE_ENDIAN */
		s0 = (*sp0++);

		if (odd) {
			(*dp++) = (s0 & ~0xffffffffLL) + (s1 & 0xffffffffLL);
			(*dp++) = (s0 << 32) + (mlib_u64)*((mlib_u32 *)sp1);
			*((mlib_s32 *)dp) = *((mlib_s32 *)sp0);
		} else {
			(*dp++) = (s0 & ~0xffffffffLL) + (s1 & 0xffffffffLL);
			s1 = *sp1;
			(*dp++) = (s0 << 32) + (s1 >> 32);
			dp[0] = (((mlib_u64)*((mlib_u32 *)sp0)) << 32) +
				(s1 & 0xffffffffLL);
		}

#endif /* _LITTLE_ENDIAN */
	} else {
		s0 = *sp0;
#ifdef _LITTLE_ENDIAN
		(*dp++) = (s1 & ~0xffffffffLL) + (s0 & 0xffffffffLL);

		if (odd) {
			*((mlib_s32 *)dp) = (s0 >> 32);
		} else {
			*dp = (s0 >> 32) + ((*sp1) << 32);
		}

#else /* _LITTLE_ENDIAN */
		(*dp++) = (s0 & ~0xffffffffLL) + (s1 & 0xffffffffLL);

		if (odd) {
			*((mlib_s32 *)dp) = (s0);
		} else {
			*dp = (s0 << 32) + *((mlib_u32 *)sp1);
		}

#endif /* _LITTLE_ENDIAN */
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* _LP64 */
}

/* *********************************************************** */
