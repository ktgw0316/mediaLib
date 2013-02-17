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

#pragma ident	"@(#)mlib_c_VideoUpSample422_S16.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample422_Nearest_S16 - 422 upsampling with nearest filter
 *      mlib_VideoUpSample422_S16         - 422 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample422_Nearest_S16(mlib_s16       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       n)
 *
 *      mlib_status mlib_VideoUpSample422_S16(mlib_s16       *dst,
 *                                            const mlib_s16 *src,
 *                                            mlib_s32       n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: length of dst array must be 2*n
 *                      For mlib_VideoUpSample422_S16: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample422_Nearest_S16:
 *        dst[i]   = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample422_S16:
 *        dst[0]     = src[0]
 *        dst[1]     = (3*src[0] + src[1] + 2) / 4
 *        dst[2*i]   = (3*src[i] + src[i-1] + 1) / 4,  0 < i < n-1
 *        dst[2*i+1] = (3*src[i] + src[i+1] + 2) / 4,  0 < i < n-1
 *        dst[2*n-2] = (3*src[n-1] + src[n-2] + 1) / 4
 *        dst[2*n-1] = src[n-1]
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoUpSample422_Nearest_S16 = \
	__mlib_VideoUpSample422_Nearest_S16
#pragma weak mlib_VideoUpSample422_S16 = __mlib_VideoUpSample422_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample422_Nearest_S16)
	mlib_VideoUpSample422_Nearest_S16
	__attribute__((weak, alias("__mlib_VideoUpSample422_Nearest_S16")));
__typeof__(__mlib_VideoUpSample422_S16) mlib_VideoUpSample422_S16
	__attribute__((weak, alias("__mlib_VideoUpSample422_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_Nearest_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_s32 *sp = (mlib_s32 *)src, sa, i;
	mlib_s32 *dp = (mlib_s32 *)dst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i += 2) {
		sa = sp[0];
#ifdef _LITTLE_ENDIAN
		dp[1] = ((mlib_u32)sa >> 16) + (sa & ~0xFFFF);
		dp[0] = (sa << 16) + (sa & 0xFFFF);
#else /* _LITTLE_ENDIAN */
		dp[0] = ((mlib_u32)sa >> 16) + (sa & ~0xFFFF);
		dp[1] = (sa << 16) + (sa & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
		sp++;
		dp += 2;
	}

	if (i < n) {
		sa = *((mlib_u16 *)sp);
		*((mlib_s32 *)dp) = (sa << 16) + sa;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_s32 invalue, colctr;

/* Special case for first column */
	invalue = ((*src++));
	dst[0] = invalue;
	dst[1] = (invalue * 3 + src[0] + 2) >> 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (colctr = 1; colctr < n - 1; colctr++) {
/* General case: 3/4 * nearer pixel + 1/4 * further pixel */
		invalue = src[0] * 3;
		dst[2 * colctr] = (invalue + src[-1] + 1) >> 2;
		dst[2 * colctr + 1] = (invalue + src[1] + 2) >> 2;
		src++;
	}

/* Special case for last column */
	invalue = *src;
	dst[2 * colctr] = (invalue * 3 + src[-1] + 1) >> 2;
	dst[2 * colctr + 1] = invalue;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
