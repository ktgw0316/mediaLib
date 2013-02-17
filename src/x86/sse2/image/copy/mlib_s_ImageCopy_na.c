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

#pragma ident	"@(#)mlib_s_ImageCopy_na.c	9.7	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopy - Direct copy from one image to another.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopy(mlib_image       *dst,
 *                                 const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output or destination image
 *      src     pointer to input or source image
 *
 * RESTRICTION
 *      src and dst must have the same size, type and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Direct copy from one image to another
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

void
mlib_ImageCopy_na(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 n)
{
	if (((mlib_addr)src ^ (mlib_addr)dst) & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(16)
#endif /* __SUNPRO_C */
		for (; n > 31; n -= 32) {
			__m128i xsd0, xsd1;
			xsd0 = _mm_loadu_si128((__m128i *)src);
			xsd1 = _mm_loadu_si128((__m128i *)(src + 16));
			_mm_storeu_si128((__m128i *)dst, xsd0);
			_mm_storeu_si128((__m128i *)(dst + 16), xsd1);
			src += 32;
			dst += 32;
		}

		if (n > 15) {
			_mm_storeu_si128((__m128i *)dst,
				_mm_loadu_si128((__m128i *)src));
			src += 16;
			dst += 16;
			n -= 16;
		}

	} else {

		for (; n > 0 && ((mlib_addr)dst & 15); n--) {
			(*dst++) = (*src++);
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(16)
#endif /* __SUNPRO_C */
		for (; n > 31; n -= 32) {
			__m128i xsd0, xsd1;
			xsd0 = _mm_load_si128((__m128i *)src);
			xsd1 = _mm_load_si128((__m128i *)(src + 16));
			_mm_store_si128((__m128i *)dst, xsd0);
			_mm_store_si128((__m128i *)(dst + 16), xsd1);
			src += 32;
			dst += 32;
		}

		if (n > 15) {
			_mm_store_si128((__m128i *)dst,
				_mm_load_si128((__m128i *)src));
			src += 16;
			dst += 16;
			n -= 16;
		}
	}

	if (n > 7) {
		_mm_storel_epi64((__m128i *)dst,
			_mm_loadl_epi64((__m128i *)src));
		src += 8;
		dst += 8;
		n -= 8;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; n > 0; n--) {
		(*dst++) = (*src++);
	}
}

/* *********************************************************** */
