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

#pragma ident	"@(#)mlib_c_VideoDownSample420.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample420 - 420 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample420(mlib_u8       *dst,
 *                                          const mlib_u8 *src0,
 *                                          const mlib_u8 *src1,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src1      Pointer to source lower row, 8-byte aligned
 *      n         Length of the src0, src1 arrays
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample420:
 *        dst[i] = (src0[2*i] + src0[2*i+1] +
 *                  src1[2*i] + src1[2*i+1] + (1 << (i&1))) / 4, 0 <= i < n/2
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample420 = __mlib_VideoDownSample420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample420) mlib_VideoDownSample420
	__attribute__((weak, alias("__mlib_VideoDownSample420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample420(
	mlib_u8 *dst,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 *sp0 = (mlib_u32 *)src0;
	mlib_u32 *sp1 = (mlib_u32 *)src1;
	mlib_u32 sa0, sa1, data0, data1, data;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n - 4; i += 4) {
		sa0 = (*sp0++);
		sa1 = (*sp1++);

		data0 = (sa0 & 0xFF00FF) + ((sa0 >> 8) & 0xFF00FF);
		data1 = (sa1 & 0xFF00FF) + ((sa1 >> 8) & 0xFF00FF);

#ifdef _LITTLE_ENDIAN

		data = (data0 + data1 + 0x20001) >> 2;
		(*dst++) = (mlib_u8)data;
		(*dst++) = (mlib_u8)(data >> 16);

#else /* _LITTLE_ENDIAN */

		data = (data0 + data1 + 0x10002) >> 2;
		(*dst++) = (mlib_u8)(data >> 16);
		(*dst++) = (mlib_u8)data;

#endif /* _LITTLE_ENDIAN */

	}

	if (i < n)
		(*dst++) =
			(src0[i] + src0[i + 1] + src1[i] + src1[i + 1] +
			1) >> 2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
