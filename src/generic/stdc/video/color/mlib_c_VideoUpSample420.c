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

#pragma ident	"@(#)mlib_c_VideoUpSample420.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample420_Nearest - 420 upsampling with nearest filter
 *      mlib_VideoUpSample420         - 420 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample420_Nearest(mlib_u8       *dst0,
 *                                                mlib_u8       *dst1,
 *                                                const mlib_u8 *src,
 *                                                mlib_s32      n)
 *
 *      mlib_status mlib_VideoUpSample420(mlib_u8       *dst0,
 *                                        mlib_u8       *dst1,
 *                                        const mlib_u8 *src0,
 *                                        const mlib_u8 *src1,
 *                                        const mlib_u8 *src2,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      dst0      Pointer to destination upper row, 8-byte aligned
 *      dst1      Pointer to destination lower row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src1      Pointer to source middle row, 8-byte aligned
 *      src2      Pointer to source lower row, 8-byte aligned
 *      n         Length of the source arrays
 *                Note: length of dst0 and dst1 arrays must be 2*n
 *                      For mlib_VideoUpSample420: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample420_Nearest:
 *        dst0[i]  = src[i/2],   0 <= i < 2*n
 *        dst1[i]  = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample420:
 *        dst0[0]     = (4*(3*src1[0]   + src0[0]) + 8) /16
 *        dst0[1]     = (3*(3*src1[0]   + src0[0]) +
 *                         (3*src1[1]   + src0[1]) + 7) / 16
 *        dst0[2*i]   = (3*(3*src1[i]   + src0[i]) +
 *                         (3*src1[i-1] + src0[i-1]) + 8) / 16,  0 < i < n-1
 *        dst0[2*i+1] = (3*(3*src1[i]   + src0[i]) +
 *                         (3*src1[i+1] + src0[i+1]) + 7) / 16,  0 < i < n-1
 *        dst0[2*n-2] = (3*(3*src1[n-1] + src0[n-1]) +
 *                         (3*src1[n-2] + src0[n-2]) + 8) / 16
 *        dst0[2*n-1] = (4*(3*src1[n-1] + src0[n-1]) + 7) / 16
 *        dst1[0]     = (4*(3*src1[0]   + src2[0]) + 8) /16
 *        dst1[1]     = (3*(3*src1[0]   + src2[0]) +
 *                         (3*src1[1]   + src2[1]) + 7) / 16
 *        dst1[2*i]   = (3*(3*src1[i]   + src2[i]) +
 *                         (3*src1[i-1] + src2[i-1]) + 8) / 16,  0 < i < n-1
 *        dst1[2*i+1] = (3*(3*src1[i]   + src2[i]) +
 *                         (3*src1[i+1] + src2[i+1]) + 7) / 16,  0 < i < n-1
 *        dst1[2*n-2] = (3*(3*src1[n-1] + src2[n-1]) +
 *                         (3*src1[n-2] + src2[n-2]) + 8) / 16
 *        dst1[2*n-1] = (4*(3*src1[n-1] + src2[n-1]) + 7) / 16
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoUpSample420 = __mlib_VideoUpSample420
#pragma weak mlib_VideoUpSample420_Nearest = __mlib_VideoUpSample420_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample420) mlib_VideoUpSample420
	__attribute__((weak, alias("__mlib_VideoUpSample420")));
__typeof__(__mlib_VideoUpSample420_Nearest) mlib_VideoUpSample420_Nearest
	__attribute__((weak, alias("__mlib_VideoUpSample420_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420_Nearest(
	mlib_u8 *dst0,
	mlib_u8 *dst1,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s16 *sp = (mlib_s16 *)src;
	mlib_s32 *dp0 = (mlib_s32 *)dst0;
	mlib_s32 *dp1 = (mlib_s32 *)dst1;
	mlib_s32 sa, da, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i += 2) {
		sa = *sp;
		da = ((sa & 0xFF00) << 8) + (sa & 0xFF);
		*dp0 = *dp1 = (da << 8) + da;
		sp++;
		dp0++;
		dp1++;
	}

	if ((n & 1) != 0) {
		sa = *((mlib_u8 *)sp);
		da = ((sa & 0xFF) << 8) + (sa & 0xFF);
		*((mlib_s16 *)dp0) = *((mlib_s16 *)dp1) = (mlib_s16)da;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420(
	mlib_u8 *dst0,
	mlib_u8 *dst1,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	const mlib_u8 *src2,
	mlib_s32 n)
{
	mlib_s32 thiscolsum0, nextcolsum0, lastcolsum0, colctr = 0;
	mlib_s32 thiscolsum1, nextcolsum1, lastcolsum1;
	mlib_u8 *dst10 = dst0 + 1;
	mlib_u8 *dst11 = dst1 + 1;

	thiscolsum0 = (*src1) * 3 + ((*src0++));
	thiscolsum1 = ((*src1++)) * 3 + ((*src2++));
	nextcolsum0 = (*src1) * 3 + ((*src0++));
	nextcolsum1 = ((*src1++)) * 3 + ((*src2++));
	dst0[2 * colctr] = ((thiscolsum0 * 4 + 8) >> 4);
	dst10[2 * colctr] = ((thiscolsum0 * 3 + nextcolsum0 + 7) >> 4);
	dst1[2 * colctr] = ((thiscolsum1 * 4 + 8) >> 4);
	dst11[2 * colctr] = ((thiscolsum1 * 3 + nextcolsum1 + 7) >> 4);
	lastcolsum0 = thiscolsum0;
	thiscolsum0 = nextcolsum0;
	lastcolsum1 = thiscolsum1;
	thiscolsum1 = nextcolsum1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (colctr = 1; colctr < n - 1; colctr++) {
/* General case: 3/4 * nearer pixel + 1/4 * further pixel in each */
/* dimension, thus 9/16, 3/16, 3/16, 1/16 overall */
		nextcolsum0 = (*src1) * 3 + (*src0);
		nextcolsum1 = (*src1) * 3 + (*src2);
		dst0[2 * colctr] = ((thiscolsum0 * 3 + lastcolsum0 + 8) >> 4);
		dst10[2 * colctr] = ((thiscolsum0 * 3 + nextcolsum0 + 7) >> 4);
		dst1[2 * colctr] = ((thiscolsum1 * 3 + lastcolsum1 + 8) >> 4);
		dst11[2 * colctr] = ((thiscolsum1 * 3 + nextcolsum1 + 7) >> 4);
		lastcolsum0 = thiscolsum0;
		thiscolsum0 = nextcolsum0;
		lastcolsum1 = thiscolsum1;
		thiscolsum1 = nextcolsum1;
		src0++;
		src1++;
		src2++;
	}

/* Special case for last column */
	dst0[2 * colctr] = ((thiscolsum0 * 3 + lastcolsum0 + 8) >> 4);
	dst10[2 * colctr] = ((thiscolsum0 * 4 + 7) >> 4);
	dst1[2 * colctr] = ((thiscolsum1 * 3 + lastcolsum1 + 8) >> 4);
	dst11[2 * colctr] = ((thiscolsum1 * 4 + 7) >> 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
