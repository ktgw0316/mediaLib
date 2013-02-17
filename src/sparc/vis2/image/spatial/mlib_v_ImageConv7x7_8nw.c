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

#pragma ident	"@(#)mlib_v_ImageConv7x7_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv7x7_8nw.c - convolve an 8-bit image with a 7x7 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *      mlib_statu mlib_conv7x7nw_u8(mlib_image       *src,
 *                                   const mlib_image *dst,
 *                                   const mlib_s32   *kernel,
 *                                   mlib_s32         scalef_expon,
 *                                   mlib_s32         cmask)
 * ARGUMENT
 *      src           Pointer to structure of source image
 *      dst           Pointer to structure of destination image
 *      kernel        Convolution kernel
 *      scalef_expons The scaling factor to convert the input integer
 *                    coefficients into floating-point coefficients:
 *                    floating-point coeff. = integer coeff. * 2^(-scale)
 *      cmask         Channel mask to indicate the channels to be convolved.
 *                    Each bit of which represents a channel in the image. The
 *                    channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      A 2-D convolution (7x7 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageConv.h>

/* *********************************************************** */

mlib_status
mlib_conv7x7nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	if (mlib_ImageConvVersion(7, 7, scalef_expon, MLIB_BYTE) == 0)
		return mlib_c_conv7x7nw_u8(dst, src, kernel, scalef_expon,
		    cmask);

	return mlib_convMxN_8nw_f(dst, src, 7, 7, 3, 3, kernel, scalef_expon,
	    cmask);
}

/* *********************************************************** */
