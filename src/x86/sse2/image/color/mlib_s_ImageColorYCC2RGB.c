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

#pragma ident	"@(#)mlib_s_ImageColorYCC2RGB.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorYCC2RGB - color conversion from YCC to RGB space
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorYCC2RGB(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *        |R|   |fmat[0] fmat[1] fmat[2]|   |Y |   |offset[0]|
 *        |G| = |fmat[3] fmat[4] fmat[5]| * |Cb| + |offset[1]|
 *        |B|   |fmat[6] fmat[7] fmat[8]|   |Cr|   |offset[2]|
 *
 *   where
 *
 *      fmat[] = { 298.082/256,  0,            408.583/256,
 *                 298.082/256, -100.291/256, -208.120/256,
 *                 298.082/256,  516.411/256,  0};
 *    offset[] = {-222.953, 135.575, -276.836};
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageColorTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorYCC2RGB = __mlib_ImageColorYCC2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorYCC2RGB) mlib_ImageColorYCC2RGB
    __attribute__((weak, alias("__mlib_ImageColorYCC2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageColorYCC2RGB(
    mlib_image *dst,
    const mlib_image *src)
{
/* Rec601 Y'CbCr to Computer R'G'B' */
	mlib_d64 fmat[9] = { 298.082 / 256, 0, 408.583 / 256,
		298.082 / 256, -100.291 / 256, -208.120 / 256,
		298.082 / 256, 516.411 / 256, 0
	};
	mlib_d64 offset[3] = { -222.953, 135.575, -276.836 };

	return (__mlib_ImageColorConvert2(dst, src, fmat, offset));
}

/* *********************************************************** */
