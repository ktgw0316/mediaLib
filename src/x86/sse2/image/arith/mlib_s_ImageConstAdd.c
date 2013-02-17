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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageConstAdd.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstAdd - add image with constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstAdd(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *c);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      c       array of constants to be added to each pixel
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels. They can have 1, 2, 3 or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst(i, j) = c + src(i, j).
 */

#include <mlib_image.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstAdd = __mlib_ImageConstAdd

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConstAdd) mlib_ImageConstAdd
    __attribute__((weak, alias("__mlib_ImageConstAdd")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNCTION_NAME	__mlib_ImageConstAdd

#define	IMAGESUB_FLAG	0

#define	ARITH_OPER_U8(d, s1, c_add, c_sub) \
	d = _mm_subs_epu8(_mm_adds_epu8(s1, c_add), c_sub)
#define	ARITH_OPER_S16(d, s1, c_add, c_sub)	d = _mm_add_epi16(s1, c_add)
#define	ARITH_OPER_U16(d, s1, c_add, c_sub) \
	d = _mm_subs_epu16(_mm_adds_epu16(s1, c_add), c_sub)
#define	ARITH_OPER_S32(d, s1, c_add, c_sub)	d = _mm_add_epi32(s1, c_add)
#define	ARITH_OPER_F32(d, s1, c_add, c_sub)	d = _mm_add_ps(s1, c_add)
#define	ARITH_OPER_D64(d, s1, c_add, c_sub)	d = _mm_add_pd(s1, c_add)

/* *********************************************************** */

#include <mlib_s_ImageConstArith.h>

/* *********************************************************** */
