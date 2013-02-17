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

#pragma ident	"@(#)mlib_c_VideoDownSample420_S16.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample420_S16 - 420 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample420_S16(mlib_s16       *dst,
 *                                              const mlib_s16 *src0,
 *                                              const mlib_s16 *src1,
 *                                              mlib_s32       n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src2      Pointer to source lower row, 8-byte aligned
 *      n         Length of the src0, src1 arrays
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample420_S16:
 *        dst[i] = (src0[2*i] + src0[2*i+1] +
 *                  src1[2*i] + src1[2*i+1] + (1 << (i&1))) / 4, 0 <= i < n/2
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample420_S16 = __mlib_VideoDownSample420_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample420_S16) mlib_VideoDownSample420_S16
	__attribute__((weak, alias("__mlib_VideoDownSample420_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample420_S16(
	mlib_s16 *dst,
	const mlib_s16 *src0,
	const mlib_s16 *src1,
	mlib_s32 n)
{
	mlib_s32 i, sa0, sa01, sa02, sa1, sa11, sa12;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n - 4; i += 4) {
		sa0 = *((mlib_s32 *)(src0 + i));
		sa01 = src0[i + 2];
		sa02 = src0[i + 3];
		sa1 = *((mlib_s32 *)(src1 + i));
		sa11 = src1[i + 2];
		sa12 = src1[i + 3];
		dst[0] = (((sa0 << 16) >> 16) + (sa0 >> 16) + 1 +
			((sa1 << 16) >> 16) + (sa1 >> 16)) >> 2;
		dst[1] = (sa01 + sa02 + 2 + sa11 + sa12) >> 2;
		dst += 2;
	}

	if (i < n)
		dst[0] = (src0[i] + src0[i + 1] + 1 + src1[i] + src1[i +
			1]) >> 2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
