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

#pragma ident	"@(#)mlib_s_ImageAve.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAve - get average of two images
 *
 * SYNOPSIS
 *      mlib_status  mlib_ImageAve(mlib_image       *dst,
 *                                 const mlib_image *src1,
 *                                 const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst = (src1 + src2 + 1) / 2
 */

#include <mlib_image.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAve = __mlib_ImageAve

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageAve) mlib_ImageAve
    __attribute__((weak, alias("__mlib_ImageAve")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNCTION_NAME	__mlib_ImageAve

#define	EXTRA_VARS_S16	__m128i mask_8000 = _mm_set1_epi16(0x8000);
#define	EXTRA_VARS_S32	__m128i mask_00000001 = _mm_set1_epi32(1);
#define	EXTRA_VARS_F32	__m128 f_half = _mm_set1_ps(0.5f);
#define	EXTRA_VARS_D64	__m128d f_half = _mm_set1_pd(0.5);

#define	ARITH_OPER_U8(d, s1, s2)	d = _mm_avg_epu8(s1, s2)
#define	ARITH_OPER_S16(d, s1, s2)                                       \
	d = _mm_avg_epu16(_mm_xor_si128(s1, mask_8000),                 \
		_mm_xor_si128(s2, mask_8000));                          \
	d = _mm_xor_si128(d, mask_8000)
#define	ARITH_OPER_U16(d, s1, s2)	d = _mm_avg_epu16(s1, s2)
#define	ARITH_OPER_S32(d, s1, s2)                                       \
	d = _mm_add_epi32(_mm_and_si128(_mm_or_si128(s1, s2),		\
					mask_00000001),			\
			_mm_add_epi32(_mm_srai_epi32(s1, 1),		\
					_mm_srai_epi32(s2, 1)))
#define	ARITH_OPER_F32(d, s1, s2)                                       \
	d = _mm_mul_ps(_mm_add_ps(s1, s2), f_half)
#define	ARITH_OPER_D64(d, s1, s2)                                       \
	d = _mm_mul_pd(_mm_add_pd(s1, s2), f_half)

#include <mlib_s_ImageArith.h>

/* *********************************************************** */
