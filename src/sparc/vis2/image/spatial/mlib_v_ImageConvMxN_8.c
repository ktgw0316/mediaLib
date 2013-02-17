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

#pragma ident	"@(#)mlib_v_ImageConvMxN_8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_v_convMxN_8nw - convolve a 8-bit image, MxN kernel,
 *                           edge = no write
 *
 * SYNOPSIS
 *      mlib_status mlib_v_convMxNnw_u8(mlib_image       *src,
 *                                      const mlib_image *dst,
 *                                      mlib_s32         kwid,
 *                                      mlib_s32         khgt,
 *                                      mlib_s32         khw,
 *                                      mlib_s32         khh,
 *                                      const mlib_s32   *skernel,
 *                                      mlib_s32         discardbits,
 *                                      mlib_s32         cmask)
 *
 * ARGUMENT
 *      src          Pointer to source image structure
 *      dst          Pointer to destination image structure
 *      khgt         Kernel height (# of rows)
 *      kwid         Kernel width (# of cols)
 *      skernel      Pointer to convolution kernel
 *      discardbits  The number of LSBits of the 32-bit accumulator that
 *                   are discarded when the 32-bit accumulator is converted
 *                   to 16-bit output data; discardbits must be 1-15 (it
 *                   cannot be zero). Same as exponent N for scalefac = 2**N.
 *      cmask        Channel mask to indicate the channels to be convolved.
 *                   Each bit of which represents a channel in the image. The
 *                   channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      A 2-D convolution (MxN kernel) for 8-bit images.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageConv.h>

/* *********************************************************** */

mlib_status
mlib_convMxNnw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 kwid,
    mlib_s32 khgt,
    mlib_s32 khw,
    mlib_s32 khh,
    mlib_s32 discardbits,
    mlib_s32 cmask)
{
	if (mlib_ImageConvVersion(kwid, khgt, discardbits, MLIB_BYTE) == 0)
		return mlib_c_convMxNnw_u8(dst, src, kernel, kwid, khgt, khw,
		    khh, discardbits, cmask);

	return mlib_convMxN_8nw_f(dst, src, kwid, khgt, khw, khh, kernel,
	    discardbits, cmask);
}

/* *********************************************************** */
