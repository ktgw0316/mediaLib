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

#pragma ident	"@(#)mlib_s_ImageConstAdd_Fp.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageConstAdd_Fp		- add an image to a constant
 *	mlib_ImageConstAdd_Fp_Inp	- add an image to a constant
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageConstAdd_Fp(mlib_image       *dst,
 *	                                  const mlib_image *src,
 *	                                  const mlib_d64   *consts)
 *	mlib_status mlib_ImageConstAdd_Fp_Inp(mlib_image     *srcdst,
 *	                                      const mlib_d64 *consts)
 *
 * ARGUMENT
 *	src     pointer to the input image
 *	dst     pointer to the output image
 *	srcdst  pointer to the input and the output image
 *	consts  array of the constants for each channel
 *
 * RESTRICTION
 *	src and dst must have the same type, the same size,
 *	and the same number of channels.
 *	The images can have 1, 2, 3, or 4 channels.
 *	The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = consts[c] + src[x][y][c]
 *	srcdst[x][y][c] = consts[c] + srcdst[x][y][c]
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstAdd_Fp = __mlib_ImageConstAdd_Fp
#pragma weak mlib_ImageConstAdd_Fp_Inp = __mlib_ImageConstAdd_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConstAdd_Fp) mlib_ImageConstAdd_Fp
	__attribute__((weak, alias("__mlib_ImageConstAdd_Fp")));
__typeof__(__mlib_ImageConstAdd_Fp_Inp) mlib_ImageConstAdd_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageConstAdd_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	mlib_ImageConstFunc_Fp		__mlib_ImageConstAdd_Fp

#define	mlib_s_ImageConstFunc_Fp_f32	mlib_s_ImageConstAdd_Fp_f32
#define	mlib_s_ImageConstFunc_Fp_d64	mlib_s_ImageConstAdd_Fp_d64

#define	MM_INST_F32			_mm_add_ps
#define	MM_INST_D64			_mm_add_pd

#define	MLIB_C_IMAGECONSTFUNC(a, b)	((a) + (b))

#include "mlib_s_ImageConstArith_Fp.h"

/* *********************************************************** */

mlib_status
__mlib_ImageConstAdd_Fp_Inp(
	mlib_image *srcdst,
	const mlib_d64 *consts)
{
	return (__mlib_ImageConstAdd_Fp(srcdst, srcdst, consts));
}

/* *********************************************************** */
