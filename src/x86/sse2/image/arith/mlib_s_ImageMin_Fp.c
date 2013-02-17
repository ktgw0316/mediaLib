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

#pragma ident	"@(#)mlib_s_ImageMin_Fp.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageMin_Fp	- get min pixel from two images
 *	mlib_ImageMin_Fp_Inp	- get min pixel from two images
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageMin_Fp(mlib_image       *dst,
 *	                             const mlib_image *src1,
 *                                   const mlib_image *src2);
 *	mlib_status mlib_ImageMin_Fp_Inp(mlib_image       *src1dst,
 *	                                 const mlib_image *src2);
 *
 * ARGUMENT
 *	dst     pointer to output image
 *	src1    pointer to first input image
 *	src2    pointer to second input image
 *	src1dst	pointer to first input and output image
 *
 * RESTRICTION
 *	src1, src2, src1dst, and dst must be the same type,
 *	the same size, and the same number of channels.
 *	They can have 1, 2, 3, or 4 channels.
 *	They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = MIN(src1[x][y][c], src2[x][y][c])
 *	src1dst[x][y][c] = MIN(src1dst[x][y][c], src2[x][y][c])
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMin_Fp = __mlib_ImageMin_Fp
#pragma weak mlib_ImageMin_Fp_Inp = __mlib_ImageMin_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMin_Fp) mlib_ImageMin_Fp
	__attribute__((weak, alias("__mlib_ImageMin_Fp")));
__typeof__(__mlib_ImageMin_Fp_Inp) mlib_ImageMin_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageMin_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	mlib_ImageFunc_Fp	__mlib_ImageMin_Fp

#define	mlib_s_ImageFunc_Fp_f32	mlib_s_ImageMin_Fp_f32
#define	mlib_s_ImageFunc_Fp_d64	mlib_s_ImageMin_Fp_d64

#define	MM_INST_F32		_mm_min_ps
#define	MM_INST_D64		_mm_min_pd

#define	MLIB_C_IMAGEFUNC(a, b)	(((a) < b) ? (a) : (b))

#include "mlib_s_ImageArith_Fp.h"

/* *********************************************************** */

mlib_status
__mlib_ImageMin_Fp_Inp(
	mlib_image *src1dst,
	const mlib_image *src2)
{
	return (__mlib_ImageMin_Fp(src1dst, src1dst, src2));
}

/* *********************************************************** */
