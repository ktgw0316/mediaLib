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

#pragma ident	"@(#)mlib_c_VideoUpSample422.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample422_Nearest - 422 upsampling with nearest filter
 *      mlib_VideoUpSample422         - 422 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample422_Nearest(mlib_u8       *dst,
 *                                                const mlib_u8 *src,
 *                                                mlib_s32      n)
 *
 *      mlib_status mlib_VideoUpSample422(mlib_u8       *dst,
 *                                        const mlib_u8 *src,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: length of dst array must be 2*n
 *                      For mlib_VideoUpSample422: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample422_Nearest:
 *        dst[i]   = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample422:
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

#pragma weak mlib_VideoUpSample422 = __mlib_VideoUpSample422
#pragma weak mlib_VideoUpSample422_Nearest = __mlib_VideoUpSample422_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample422) mlib_VideoUpSample422
	__attribute__((weak, alias("__mlib_VideoUpSample422")));
__typeof__(__mlib_VideoUpSample422_Nearest) mlib_VideoUpSample422_Nearest
	__attribute__((weak, alias("__mlib_VideoUpSample422_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_Nearest(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s16 *sp = (mlib_s16 *)src;
	mlib_s32 *dp = (mlib_s32 *)dst;
	mlib_s32 sa, da, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i += 2) {
		sa = *sp;
		da = ((sa & 0xFF00) << 8) + (sa & 0xFF);
		*dp = (da << 8) + da;
		sp++;
		dp++;
	}

	if ((n & 1) != 0) {
		sa = *((mlib_u8 *)sp);
		da = ((sa & 0xFF) << 8) + (sa & 0xFF);
		*((mlib_s16 *)dp) = (mlib_s16)da;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s32 invalue, colctr = 0;
	mlib_u8 *dst1 = dst + 1;

	invalue = ((*src++));
	dst[2 * colctr] = invalue;
	dst1[2 * colctr] = ((invalue * 3 + (*src) + 2) >> 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (colctr = 1; colctr < n - 1; colctr++) {
/* General case: 3/4 * nearer pixel + 1/4 * further pixel */
		invalue = ((*src++)) * 3;
		dst[2 * colctr] = ((invalue + (src[-2]) + 1) >> 2);
		dst1[2 * colctr] = ((invalue + (*src) + 2) >> 2);
	}

/* Special case for last column */
	invalue = (*src);
	dst[2 * colctr] = ((invalue * 3 + (src[-1]) + 1) >> 2);
	dst1[2 * colctr] = invalue;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
