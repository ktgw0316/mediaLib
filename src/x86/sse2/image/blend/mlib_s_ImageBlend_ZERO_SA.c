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

#pragma ident	"@(#)mlib_s_ImageBlend_ZERO_SA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_SA - blend two images with
 *      SA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_SA(mlib_image       *dst,
 *                                          const mlib_image *src1,
 *                                          const mlib_image *src2,
 *                                          mlib_s32         cmask);
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1, src2 and dst must be the same type,
 *      the same size and the same number of channels.
 *      They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 1 or 8
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 * DESCRIPTION
 *      dst = src2 * ALPHAsrc2
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_ZERO_SA = __mlib_ImageBlend_ZERO_SA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_ZERO_SA) mlib_ImageBlend_ZERO_SA
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_SA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	UNPACK_UNSIGN_BYTE                                \
{                                                         \
	dy_1 = _mm_unpackhi_epi8(dy, dall_zero);              \
	dy_0 = _mm_unpacklo_epi8(dy, dall_zero);              \
}

/* *********************************************************** */
#define	PROCESS_DATA_8(ssrc2, sdst)                       \
{                                                         \
	__m128i dalpha1, dalpha2;                             \
                                                          \
	dalpha1 = _mm_unpacklo_epi16(ssrc2, ssrc2);           \
	dalpha1 = _mm_unpacklo_epi16(dalpha1, dalpha1);       \
	dalpha2 = _mm_unpackhi_epi16(ssrc2, ssrc2);           \
	dalpha2 = _mm_unpacklo_epi16(dalpha2, dalpha2);       \
	dalpha1 = _mm_unpacklo_epi64(dalpha1, dalpha2);       \
                                                          \
	dalpha1 = _mm_mullo_epi16(dalpha1, ssrc2);            \
	sdst = _mm_srli_epi16(dalpha1, 8);                    \
}

/* *********************************************************** */
#define	PROCESS_DATA_1(ssrc2, sdst)                       \
{                                                         \
	__m128i dalpha1, dalpha2;                             \
                                                          \
	dalpha1 = _mm_unpacklo_epi16(ssrc2, ssrc2);           \
	dalpha1 = _mm_unpackhi_epi16(dalpha1, dalpha1);       \
	dalpha2 = _mm_unpackhi_epi16(ssrc2, ssrc2);           \
	dalpha2 = _mm_unpackhi_epi16(dalpha2, dalpha2);       \
	dalpha1 = _mm_unpackhi_epi64(dalpha1, dalpha2);       \
                                                          \
	dalpha1 = _mm_mullo_epi16(dalpha1, ssrc2);            \
	sdst = _mm_srli_epi16(dalpha1, 8);                    \
}

/* *********************************************************** */
#define	DO_REST                                           \
{                                                         \
	for (k = i; k < dst_width; k++) {                     \
		a = psrc2[4 * k + alpha];                         \
                                                          \
		a0 = (psrc2[4 * k + 0] * a) >> 8;                 \
		a1 = (psrc2[4 * k + 1] * a) >> 8;                 \
		a2 = (psrc2[4 * k + 2] * a) >> 8;                 \
		a3 = (psrc2[4 * k + 3] * a) >> 8;                 \
                                                          \
		pdst[4 * k + 0] = a0;                             \
		pdst[4 * k + 1] = a1;                             \
		pdst[4 * k + 2] = a2;                             \
		pdst[4 * k + 3] = a3;                             \
	}                                                     \
}
/* *********************************************************** */

mlib_status
__mlib_ImageBlend_ZERO_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_u8 a0, a1, a2, a3;
	mlib_s32 help_alpha;
	mlib_s32 a;

	BLEND_VALIDATE_1;

	if (channels == 3)
		return (__mlib_ImageCopy(dst, src2));

	mlib_s32 alpha;

	int k;
	alpha = ((cmask << 1) + cmask) & 0x3;
	help_alpha = alpha ^ 3;
	__m128i *py, *pz;
	__m128i dy;
/* upper - 1 lower - 0 */
	__m128i dy_1, dy_0, dz_1, dz_0;
	__m128i dall_zero;

	dall_zero = _mm_setzero_si128();
	if (cmask == 8) {
		if (0 == (((((mlib_addr)psrc2 |
					(mlib_addr)pdst)) & 0xf)) &&
				(0 == (((src1_stride | src2_stride |
			dst_stride) & 0xf) || (1 == dst_height)))) {
			for (j = 0; j < dst_height; j++) {
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4;	i += 4) {
					dy = _mm_load_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_8(dy_1, dz_1);
					PROCESS_DATA_8(dy_0, dz_0);
					dz_0 = _mm_packus_epi16(dz_0, dz_1);
					_mm_store_si128(pz, dz_0);
					py++;
					pz++;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		} else {
			for (j = 0; j < dst_height; j++) {
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dy = _mm_loadu_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_8(dy_1, dz_1);
					PROCESS_DATA_8(dy_0, dz_0);
					dz_0 = _mm_packus_epi16(dz_0, dz_1);
					_mm_storeu_si128(pz, dz_0);
					py++;
					pz++;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		}
	} else {
		if (0 == (((((mlib_addr)psrc2 |
					(mlib_addr)pdst)) & 0xf)) &&
				(0 == (((src1_stride | src2_stride |
				dst_stride) & 0xf) || (1 == dst_height)))) {
			for (j = 0; j < dst_height; j++) {
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dy = _mm_load_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_1(dy_1, dz_1);
					PROCESS_DATA_1(dy_0, dz_0);
					dz_0 = _mm_packus_epi16(dz_0, dz_1);
					_mm_store_si128(pz, dz_0);
					py++;
					pz++;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		} else {
			for (j = 0; j < dst_height; j++) {
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dy = _mm_loadu_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_1(dy_1, dz_1);
					PROCESS_DATA_1(dy_0, dz_0);
					dz_0 = _mm_packus_epi16(dz_0, dz_1);
					_mm_storeu_si128(pz, dz_0);
					py++;
					pz++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		}
	}

	return (MLIB_SUCCESS);
}
