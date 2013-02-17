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

#pragma ident	"@(#)mlib_s_ImageBlend_DA_OMSA.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_OMSA - blend two images with
 *      OMSA source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_OMSA(mlib_image       *dst,
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
 *      cmask has to contain only 1 bit corresponding to 1 or 8
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 * DESCRIPTION
 *      dst = src1 * ALPHAsrc1 + src2 * (1 - ALPHAsrc2)
 *
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

#pragma weak mlib_ImageBlend_DA_OMSA = __mlib_ImageBlend_DA_OMSA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_DA_OMSA) mlib_ImageBlend_DA_OMSA
    __attribute__((weak, alias("__mlib_ImageBlend_DA_OMSA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	SAT_U8_U8C(dst, src)    dst = (((src << 23) >> 31) | src)

/* *********************************************************** */
#define	UNPACK_UNSIGN_BYTE                                \
{                                                         \
	dx_1 = _mm_unpackhi_epi8(dx, dall_zero);              \
	dx_0 = _mm_unpacklo_epi8(dx, dall_zero);              \
	dy_1 = _mm_unpackhi_epi8(dy, dall_zero);              \
	dy_0 = _mm_unpacklo_epi8(dy, dall_zero);              \
}

/* *********************************************************** */
#define	PROCESS_DATA_0(ssrc1, ssrc2, sdst)                \
{                                                         \
	__m128i dalpha1, dalpha2, dalpha3;                    \
	dalpha1 = _mm_unpacklo_epi16(ssrc1, ssrc1);           \
	dalpha1 = _mm_unpacklo_epi16(dalpha1, dalpha1);       \
	dalpha3 = _mm_unpackhi_epi16(ssrc1, ssrc1);           \
	dalpha3 = _mm_unpacklo_epi16(dalpha3, dalpha3);       \
	dalpha1 = _mm_unpacklo_epi64(dalpha1, dalpha3);       \
	dalpha1 = _mm_mullo_epi16(dalpha1, ssrc1);            \
                                                          \
	dalpha2 = _mm_unpacklo_epi16(ssrc2, ssrc2);           \
	dalpha2 = _mm_unpacklo_epi16(dalpha2, dalpha2);       \
	dalpha3 = _mm_unpackhi_epi16(ssrc2, ssrc2);           \
	dalpha3 = _mm_unpacklo_epi16(dalpha3, dalpha3);       \
	dalpha2 = _mm_unpacklo_epi64(dalpha2, dalpha3);       \
	dalpha2 = _mm_xor_si128(dalpha2, df_f);            \
	dalpha2 = _mm_add_epi16(dalpha2, done_one);           \
	dalpha2 = _mm_mullo_epi16(dalpha2, ssrc2);            \
                                                          \
	sdst = _mm_adds_epu16(dalpha1, dalpha2);              \
	sdst = _mm_srli_epi16(sdst, 8);                       \
}

/* *********************************************************** */
#define	PROCESS_DATA_3(ssrc1, ssrc2, sdst)                \
{                                                         \
	__m128i dalpha1, dalpha2, dalpha3;                    \
	dalpha1 = _mm_unpacklo_epi16(ssrc1, ssrc1);           \
	dalpha1 = _mm_unpackhi_epi16(dalpha1, dalpha1);       \
	dalpha3 = _mm_unpackhi_epi16(ssrc1, ssrc1);           \
	dalpha3 = _mm_unpackhi_epi16(dalpha3, dalpha3);       \
	dalpha1 = _mm_unpackhi_epi64(dalpha1, dalpha3);       \
	dalpha1 = _mm_mullo_epi16(dalpha1, ssrc1);            \
                                                          \
	dalpha2 = _mm_unpacklo_epi16(ssrc2, ssrc2);           \
	dalpha2 = _mm_unpackhi_epi16(dalpha2, dalpha2);       \
	dalpha3 = _mm_unpackhi_epi16(ssrc2, ssrc2);           \
	dalpha3 = _mm_unpackhi_epi16(dalpha3, dalpha3);       \
	dalpha2 = _mm_unpackhi_epi64(dalpha2, dalpha3);       \
	dalpha2 = _mm_xor_si128(dalpha2, df_f);            \
	dalpha2 = _mm_add_epi16(dalpha2, done_one);           \
	dalpha2 = _mm_mullo_epi16(dalpha2, ssrc2);            \
                                                          \
	sdst = _mm_adds_epu16(dalpha1, dalpha2);              \
	sdst = _mm_srli_epi16(sdst, 8);                       \
}

/* *********************************************************** */
#define	DO_REST                                           \
{                                                         \
	for (k = i; k < dst_width; k++) {                     \
		mlib_u8 src20, src21, src22, src23;               \
		mlib_u8 src10, src11, src12, src13;               \
                                                          \
		a1 = psrc1[4 * k + off];                          \
		a2 = 256 - psrc2[4 * k + off];                    \
                                                          \
		src10 = psrc1[4 * k + 0];                         \
		src20 = psrc2[4 * k + 0];                         \
		src11 = psrc1[4 * k + 1];                         \
		src21 = psrc2[4 * k + 1];                         \
                                                          \
		src12 = psrc1[4 * k + 2];                         \
		src22 = psrc2[4 * k + 2];                         \
                                                          \
		src13 = psrc1[4 * k + 3];                         \
		src23 = psrc2[4 * k + 3];                         \
                                                          \
		SAT_U8_U8C(pdst[4 * k + 0],                       \
			(src10 * a1 + src20 * a2) >> 8);              \
		SAT_U8_U8C(pdst[4 * k + 1],                       \
			(src11 * a1 + src21 * a2) >> 8);              \
		SAT_U8_U8C(pdst[4 * k + 2],                       \
			(src12 * a1 + src22 * a2) >> 8);              \
		SAT_U8_U8C(pdst[4 * k + 3],                       \
			(src13 * a1 + src23 * a2) >> 8);              \
	}                                                     \
}
/* *********************************************************** */

mlib_status
__mlib_ImageBlend_DA_OMSA(
	mlib_image *dst,
	const mlib_image *src1,
	const mlib_image *src2,
	mlib_s32 cmask)
{
	mlib_s32 off = 0;
	mlib_s32 a1, a2;
	mlib_s32 k;

	__m128i *px, *py, *pz;
	__m128i dx, dy;
/* upper - 1 lower - 0 */
	__m128i dx_1, dx_0, dy_1, dy_0;
	__m128i dz_1, dz_0;
	__m128i dall_zero;

	dall_zero = _mm_setzero_si128();
	__m128i df_f = _mm_set1_epi32(0x00ff00ff);
	__m128i done_one = _mm_set1_epi32(0x00010001);

	BLEND_VALIDATE;
	if (channels == 3)
		return (__mlib_ImageCopy(dst, src1));

	if (cmask == 1)
		off = 3;
	if (0 == off) {
		if (0 == (((((mlib_addr) psrc1 | (mlib_addr)psrc2 |
					(mlib_addr)pdst)) & 0xf)) &&
				(0 == (((src1_stride | src2_stride |
				dst_stride) & 0xf) || (1 == dst_height)))) {
			for (j = 0; j < dst_height; j++) {
				px = (__m128i *)psrc1;
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dx = _mm_load_si128(px);
					dy = _mm_load_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_0(dx_1, dy_1, dz_1);
					PROCESS_DATA_0(dx_0, dy_0, dz_0);
					dz_0 = _mm_packus_epi16(
							dz_0, dz_1);
					_mm_store_si128(pz, dz_0);
					px++;
					py++;
					pz++;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;
				psrc1 += src1_stride;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		} else {
			for (j = 0; j < dst_height; j++) {
				px = (__m128i *)psrc1;
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dx = _mm_loadu_si128(px);
					dy = _mm_loadu_si128(py);

					UNPACK_UNSIGN_BYTE;
					PROCESS_DATA_0(dx_1, dy_1, dz_1);
					PROCESS_DATA_0(dx_0, dy_0, dz_0);
					dz_0 = _mm_packus_epi16(
							dz_0, dz_1);
					_mm_storeu_si128(pz, dz_0);

					px++;
					py++;
					pz++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;

				psrc1 += src1_stride;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		}
	} else {
		if (0 == (((((mlib_addr) psrc1 | (mlib_addr)psrc2 |
					(mlib_addr)pdst)) & 0xf)) &&
				(0 == (((src1_stride | src2_stride |
				dst_stride) & 0xf) || (1 == dst_height)))) {
			for (j = 0; j < dst_height; j++) {
				px = (__m128i *)psrc1;
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dx = _mm_load_si128(px);
					dy = _mm_load_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_3(dx_1, dy_1, dz_1);
					PROCESS_DATA_3(dx_0, dy_0, dz_0);
					dz_0 = _mm_packus_epi16(
							dz_0, dz_1);
					_mm_store_si128(pz, dz_0);
					px++;
					py++;
					pz++;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;

				psrc1 += src1_stride;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		} else {
			for (j = 0; j < dst_height; j++) {
				px = (__m128i *)psrc1;
				py = (__m128i *)psrc2;
				pz = (__m128i *)pdst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= dst_width - 4; i += 4) {
					dx = _mm_loadu_si128(px);
					dy = _mm_loadu_si128(py);

					UNPACK_UNSIGN_BYTE;

					PROCESS_DATA_3(dx_1, dy_1, dz_1);
					PROCESS_DATA_3(dx_0, dy_0, dz_0);
					dz_0 = _mm_packus_epi16(
							dz_0, dz_1);
					_mm_storeu_si128(pz, dz_0);
					px++;
					py++;
					pz++;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				DO_REST;

				psrc1 += src1_stride;
				psrc2 += src2_stride;
				pdst += dst_stride;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
