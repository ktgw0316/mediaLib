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

#pragma ident	"@(#)mlib_s_ImageChannelMerge_U8_24.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_U8
 *      mlib_ImageChannelMerge4_U8
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge2_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	__m128i *x_srd0, *x_srd1;
	__m128i *x_dstd;
	__m128i x_sd0, x_sd1, x_dd0, x_dd1;
	mlib_s32 i, j, l, n = width * 2;
	mlib_u16 *dsh;
	mlib_u8 *ptr0, *ptr1;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_u8_0;
		ptr1 = (void *)src_u8_1;
		dsh = (mlib_u16 *)dst_u8_0;
		x_dstd = (__m128i *)dsh;
		x_srd0 = (__m128i *)ptr0;
		x_srd1 = (__m128i *)ptr1;

		if ((!((mlib_addr)ptr0 & 15)) && (!((mlib_addr)ptr1 & 15)) &&
		    (!((mlib_addr)dsh & 15))) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 31); i += 32, l++) {
				x_sd0 = _mm_load_si128(x_srd0);
				x_sd1 = _mm_load_si128(x_srd1);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi8(x_sd0, x_sd1);
				_mm_store_si128(x_dstd, x_dd0);
				x_dstd++;
				_mm_store_si128(x_dstd, x_dd1);
				x_srd0++;
				x_srd1++;
				x_dstd++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 15); i += 16) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				_mm_store_si128(x_dstd, x_dd0);
				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_dstd++;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 31); i += 32, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi8(x_sd0, x_sd1);
				_mm_storeu_si128(x_dstd, x_dd0);
				x_dstd++;
				_mm_storeu_si128(x_dstd, x_dd1);
				x_srd0++;
				x_srd1++;
				x_dstd++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 15); i += 16) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				_mm_storeu_si128(x_dstd, x_dd0);
				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_dstd++;
			}
		}

		ptr0 = (mlib_u8 *)x_srd0;
		ptr1 = (mlib_u8 *)x_srd1;
		dsh = (mlib_u16 *)x_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 1); i += 2) {
			mlib_u16 a = (mlib_u16)((*ptr0++));
			mlib_u16 b = (mlib_u16)((*ptr1++));

			(*dsh++) = (b << 8) | a;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    const mlib_u8 *src_u8_2,
    const mlib_u8 *src_u8_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride)
{
	__m128i *x_srd0, *x_srd1, *x_srd2, *x_srd3;
	__m128i *x_dstd;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;
	mlib_s32 i, j, l, n = width * 4;
	mlib_u32 *dsh;
	mlib_u8 *ptr0, *ptr1, *ptr2, *ptr3;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_u8_0;
		ptr1 = (void *)src_u8_1;
		ptr2 = (void *)src_u8_2;
		ptr3 = (void *)src_u8_3;
		dsh = (mlib_u32 *)dst_u8_0;
		x_dstd = (__m128i *)dsh;
		x_srd0 = (__m128i *)ptr0;
		x_srd1 = (__m128i *)ptr1;
		x_srd2 = (__m128i *)ptr2;
		x_srd3 = (__m128i *)ptr3;

		if ((!((mlib_addr)ptr0 & 15)) && (!((mlib_addr)ptr1 & 15)) &&
		    (!((mlib_addr)ptr2 & 15)) && (!((mlib_addr)ptr3 & 15)) &&
		    (!((mlib_addr)dsh & 15))) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 63); i += 64, l++) {
				x_sd0 = _mm_load_si128(x_srd0);
				x_sd1 = _mm_load_si128(x_srd1);
				x_sd2 = _mm_load_si128(x_srd2);
				x_sd3 = _mm_load_si128(x_srd3);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi8(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_dd3 = _mm_unpackhi_epi8(x_sd2, x_sd3);

				x_sd0 = _mm_unpacklo_epi16(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi16(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi16(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi16(x_dd1, x_dd3);

				_mm_store_si128(x_dstd, x_sd0);
				x_dstd++;
				_mm_store_si128(x_dstd, x_sd1);
				x_dstd++;
				_mm_store_si128(x_dstd, x_sd2);
				x_dstd++;
				_mm_store_si128(x_dstd, x_sd3);
				x_dstd++;

				x_srd0++;
				x_srd1++;
				x_srd2++;
				x_srd3++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 31); i += 32) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);
				x_sd3 = _mm_loadl_epi64(x_srd3);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_dd1 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_dd2 = _mm_unpacklo_epi16(x_dd0, x_dd1);
				x_dd3 = _mm_unpackhi_epi16(x_dd0, x_dd1);
				_mm_store_si128(x_dstd, x_dd2);
				x_dstd++;
				_mm_store_si128(x_dstd, x_dd3);
				x_dstd++;
				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_srd2 = (__m128i *)((__m64 *)x_srd2 + 1);
				x_srd3 = (__m128i *)((__m64 *)x_srd3 + 1);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 63); i += 64, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_sd2 = _mm_loadu_si128(x_srd2);
				x_sd3 = _mm_loadu_si128(x_srd3);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi8(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_dd3 = _mm_unpackhi_epi8(x_sd2, x_sd3);

				x_sd0 = _mm_unpacklo_epi16(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi16(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi16(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi16(x_dd1, x_dd3);

				_mm_storeu_si128(x_dstd, x_sd0);
				x_dstd++;
				_mm_storeu_si128(x_dstd, x_sd1);
				x_dstd++;
				_mm_storeu_si128(x_dstd, x_sd2);
				x_dstd++;
				_mm_storeu_si128(x_dstd, x_sd3);
				x_dstd++;

				x_srd0++;
				x_srd1++;
				x_srd2++;
				x_srd3++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 31); i += 32) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);
				x_sd3 = _mm_loadl_epi64(x_srd3);
				x_dd0 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_dd1 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_dd2 = _mm_unpacklo_epi16(x_dd0, x_dd1);
				x_dd3 = _mm_unpackhi_epi16(x_dd0, x_dd1);
				_mm_storeu_si128(x_dstd, x_dd2);
				x_dstd++;
				_mm_storeu_si128(x_dstd, x_dd3);
				x_dstd++;
				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_srd2 = (__m128i *)((__m64 *)x_srd2 + 1);
				x_srd3 = (__m128i *)((__m64 *)x_srd3 + 1);
			}
		}

		ptr0 = (mlib_u8 *)x_srd0;
		ptr1 = (mlib_u8 *)x_srd1;
		ptr2 = (mlib_u8 *)x_srd2;
		ptr3 = (mlib_u8 *)x_srd3;
		dsh = (mlib_u32 *)x_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 3); i += 4) {
			mlib_u32 a = (mlib_u32)((*ptr0++));
			mlib_u32 b = (mlib_u32)((*ptr1++));
			mlib_u32 c = (mlib_u32)((*ptr2++));
			mlib_u32 d = (mlib_u32)((*ptr3++));

			(*dsh++) =
			    (((d << 8) | c) << 16) | (b << 8) | a;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
		src_u8_2 += src2_stride;
		src_u8_3 += src3_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
