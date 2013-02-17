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

#pragma ident	"@(#)mlib_s_ImageConstSub.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstSub - subtraction image from constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstSub(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *c);
 *
 * ARGUMENT
 *      src     pointer to input image
 *      dst     pointer to output image
 *      c       array of constants each pixel to be subtracted from
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst(i, j) = c - src(i, j).
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstSub = __mlib_ImageConstSub

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConstSub) mlib_ImageConstSub
    __attribute__((weak, alias("__mlib_ImageConstSub")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNCTION_NAME	__mlib_ImageConstSub

#define	IMAGESUB_FLAG	1

#define	ARITH_OPER_U8(d, s1, c_add, c_sub) \
	d = _mm_adds_epu8(_mm_subs_epu8(c_add, s1), c_sub)
#define	ARITH_OPER_S16(d, s1, c_add, c_sub)	d = _mm_sub_epi16(c_add, s1)
#define	ARITH_OPER_U16(d, s1, c_add, c_sub) \
	d = _mm_adds_epu16(_mm_subs_epu16(c_add, s1), c_sub)
#define	ARITH_OPER_S32(d, s1, c_add, c_sub)	d = _mm_sub_epi32(c_add, s1)
#define	ARITH_OPER_F32(d, s1, c_add, c_sub)	d = _mm_sub_ps(c_add, s1)
#define	ARITH_OPER_D64(d, s1, c_add, c_sub)	d = _mm_sub_pd(c_add, s1)

/* *********************************************************** */

#include <mlib_s_ImageConstArith.h>

/* *********************************************************** */
