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

#pragma ident "@(#)mlib_s_ImageAbs_Fp.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageAbs_Fp	- absolute value of an image
 *	mlib_ImageAbs_Fp_Inp	- absolute value of an image
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageAbs_Fp(mlib_image       *dst,
 *	                             const mlib_image *src);
 *	mlib_status mlib_ImageAbs_Fp_Inp(mlib_image *srcdst);
 *
 * ARGUMENT
 *	dst	pointer to destination image
 *	src	pointer to source image
 *	srcdst  pointer to input and output image
 *
 * RESTRICTION
 *	src, srcdst, and dst must be the same type, the same size,
 *	and the same number of channels.
 *	They can have 1, 2, 3, or 4 channels.
 *	They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = |src[x][y][c]|
 *	srcdst[x][y][c] = |srcdst[x][y][c]|
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAbs_Fp = __mlib_ImageAbs_Fp
#pragma weak mlib_ImageAbs_Fp_Inp = __mlib_ImageAbs_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageAbs_Fp) mlib_ImageAbs_Fp
	__attribute__((weak, alias("__mlib_ImageAbs_Fp")));
__typeof__(__mlib_ImageAbs_Fp_Inp) mlib_ImageAbs_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageAbs_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	mlib_ImageFunc_Fp	__mlib_ImageAbs_Fp

#define	mlib_s_ImageFunc_Fp_f32	mlib_s_ImageAbs_Fp_f32
#define	mlib_s_ImageFunc_Fp_d64	mlib_s_ImageAbs_Fp_d64

#define	MM_INST_F32		_mm_and_ps
#define	MM_INST_D64		_mm_and_pd

#define	MASK_HI_S32		0x7fffffff
#define	MASK_LO_S32		0xffffffff

#define	MLIB_C_IMAGEFUNC(a)	(mlib_fabs(a))

#include "mlib_s_ImageArith2_Fp.h"

/* *********************************************************** */

mlib_status
__mlib_ImageAbs_Fp_Inp(
	mlib_image *srcdst)
{
	return (__mlib_ImageAbs_Fp(srcdst, srcdst));
}

/* *********************************************************** */
