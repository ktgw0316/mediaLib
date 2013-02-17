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

#pragma ident	"@(#)mlib_v_VideoColorJFIFYCC2RGB422.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB422 - color conversion with 422 upsampling
 *                                      with bilinear filter.
 *                                      Only Cr and Cb components
 *                                      are under upsampling.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorJFIFYCC2RGB422(mlib_u8       *rgb,
 *                                                const mlib_u8 *y,
 *                                                const mlib_u8 *cb,
 *                                                const mlib_u8 *cr,
 *                                                mlib_s32      n);
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorJFIFYCC2RGB422:
 *        composition of mlib_VideoUpSample422 with
 *                       mlib_VideoColorJFIFYCC2RGB444
 *
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB422 = __mlib_VideoColorJFIFYCC2RGB422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB422) mlib_VideoColorJFIFYCC2RGB422
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2RGB422(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	mlib_d64 arrJFIFYCC42[512];
	mlib_s32 m = ((n - 1) & ~7) + 8;
	mlib_u8 *buff;
	mlib_u8 *cb2, *cr2;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (m > 2048) {
		buff = (mlib_u8 *)__mlib_malloc(m * 2);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = (mlib_u8 *)arrJFIFYCC42;

	cb2 = buff;
	cr2 = buff + m;

	__mlib_VideoUpSample422(cb2, cb, n >> 1);
	__mlib_VideoUpSample422(cr2, cr, n >> 1);
	__mlib_VideoColorJFIFYCC2RGB444(rgb, y, cb2, cr2, n);

	if (buff != (mlib_u8 *)arrJFIFYCC42)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
