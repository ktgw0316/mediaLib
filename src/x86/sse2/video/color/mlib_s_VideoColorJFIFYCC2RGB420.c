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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoColorJFIFYCC2RGB420.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB420 - color conversion with 420 upsampling
 *                                      with bilinear filter
 *                                      Only Cr and Cb components are
 *                                      under upsampling
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2RGB420(mlib_u8       *rgb0,
 *                                                mlib_u8       *rgb1,
 *                                                const mlib_u8 *y0,
 *                                                const mlib_u8 *y1,
 *                                                const mlib_u8 *cb0,
 *                                                const mlib_u8 *cr0,
 *                                                const mlib_u8 *cb1,
 *                                                const mlib_u8 *cr1,
 *                                                const mlib_u8 *cb2,
 *                                                const mlib_u8 *cr2,
 *                                                mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb0      Pointer to upper RGB multicomponent row, 8-byte aligned
 *      rgb1      Pointer to lower RGB multicomponent row, 8-byte aligned
 *      y0        Pointer to upper Y component row, 8-byte aligned
 *      y1        Pointer to lower Y component row, 8-byte aligned
 *      cb0       Pointer to upper Cb component row, 8-byte aligned
 *      cr0       Pointer to upper Cr component row, 8-byte aligned
 *      cb1       Pointer to middle Cb component row, 8-byte aligned
 *      cr1       Pointer to middle Cr component row, 8-byte aligned
 *      cb2       Pointer to lower Cb component row, 8-byte aligned
 *      cr2       Pointer to lower Cr component row, 8-byte aligned
 *      n         Length of y0, y1 arrays
 *                Note: n must be even
 *                      length of cb0, cr0, cb1, cr1, cb2, cr2
 *                      arrays must be n/2
 *                      length of rgb0, rgb1 arrays must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorJFIFYCC2RGB420:
 *        composition of mlib_VideoUpSample420 with
 *                       mlib_VideoColorJFIFYCC2RGB444
 */

#include <mlib_video.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB420 = __mlib_VideoColorJFIFYCC2RGB420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB420) mlib_VideoColorJFIFYCC2RGB420
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2RGB420(
	mlib_u8 *rgb0,
	mlib_u8 *rgb1,
	const mlib_u8 *y0,
	const mlib_u8 *y1,
	const mlib_u8 *cb0,
	const mlib_u8 *cr0,
	const mlib_u8 *cb1,
	const mlib_u8 *cr1,
	const mlib_u8 *cb2,
	const mlib_u8 *cr2,
	mlib_s32 n)
{
	mlib_u8 *buff, *cb_0, *cr_0, *cb_1, *cr_1;
	mlib_s32 m = (n & ~0xf) + 16;

	buff = (mlib_u8 *)__mlib_malloc(m * 4);

	if (buff == NULL)
		return (MLIB_FAILURE);

	cb_0 = buff;
	cb_1 = cb_0 + m;
	cr_0 = cb_1 + m;
	cr_1 = cr_0 + m;

	__mlib_VideoUpSample420(cb_0, cb_1, cb0, cb1, cb2, n >> 1);
	__mlib_VideoUpSample420(cr_0, cr_1, cr0, cr1, cr2, n >> 1);
	__mlib_VideoColorJFIFYCC2RGB444(rgb0, y0, cb_0, cr_0, n);
	__mlib_VideoColorJFIFYCC2RGB444(rgb1, y1, cb_1, cr_1, n);

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
