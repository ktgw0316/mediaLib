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

#pragma ident	"@(#)mlib_s_ImageChannelMerge_FS32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_FS32
 *      mlib_ImageChannelMerge3_FS32
 *      mlib_ImageChannelMerge4_FS32
 *
 * DESCRIPTION
 *      Internal functions.
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
mlib_ImageChannelMerge2_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	__m128i *x_srd0, *x_srd1;
	__m128i *x_dstd;
	__m128i x_sd0, x_sd1, x_dd0, x_dd1;
	__m64 *m_srd0, *m_srd1;
	__m64 *m_dstd;
	__m64 m_sd0, m_sd1, m_dd0, m_dd1;
	mlib_s32 i, j, l, n = width * 2;
	mlib_s64 *dsh;
	mlib_s32 *ptr0, *ptr1;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_s32_0;
		ptr1 = (void *)src_s32_1;
		dsh = (mlib_s64 *)dst_s32_0;
		x_dstd = (__m128i *)dsh;
		x_srd0 = (__m128i *)ptr0;
		x_srd1 = (__m128i *)ptr1;

		if ((!((mlib_addr)ptr0 & 15)) && (!((mlib_addr)ptr1 & 15)) &&
		    (!((mlib_addr)dsh & 15))) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 7); i += 8, l++) {
				x_sd0 = _mm_load_si128(x_srd0);
				x_sd1 = _mm_load_si128(x_srd1);
				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				_mm_store_si128(x_dstd, x_dd0);
				x_dstd++;
				_mm_store_si128(x_dstd, x_dd1);
				x_srd0++;
				x_srd1++;
				x_dstd++;
			}

		m_srd0 = (__m64 *)x_srd0;
		m_srd1 = (__m64 *)x_srd1;
		m_dstd = (__m64 *)x_dstd;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				m_sd0 = *(m_srd0);
				m_sd1 = *(m_srd1);
				m_dd0 = _mm_unpacklo_pi32(m_sd0, m_sd1);
				m_dd1 = _mm_unpackhi_pi32(m_sd0, m_sd1);

				*m_dstd = m_dd0;
				m_dstd++;
				*m_dstd = m_dd1;
				m_dstd++;

				m_srd0++;
				m_srd1++;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 7); i += 8, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				_mm_storeu_si128(x_dstd, x_dd0);
				x_dstd++;
				_mm_storeu_si128(x_dstd, x_dd1);
				x_srd0++;
				x_srd1++;
				x_dstd++;
			}


		m_srd0 = (__m64 *)x_srd0;
		m_srd1 = (__m64 *)x_srd1;
		m_dstd = (__m64 *)x_dstd;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				m_sd0 = *(m_srd0);
				m_sd1 = *(m_srd1);
				m_dd0 = _mm_unpacklo_pi32(m_sd0, m_sd1);
				m_dd1 = _mm_unpackhi_pi32(m_sd0, m_sd1);

				*m_dstd = m_dd0;
				m_dstd++;
				*m_dstd = m_dd1;
				m_dstd++;

				m_srd0++;
				m_srd1++;
			}
		}

		ptr0 = (mlib_s32 *)m_srd0;
		ptr1 = (mlib_s32 *)m_srd1;
		dsh = (mlib_s64 *)m_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 1); i += 2) {
			mlib_s64 a = (mlib_s64)((*ptr0++));
			mlib_s64 b = (mlib_s64)((*ptr1++));

			(*dsh++) = (b << 32) | (a & 0x00000000ffffffff);
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge3_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    const mlib_s32 *src_s32_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride)
{
	const __m128i x_zero = _mm_setzero_si128();
	__m128i *x_srd0, *x_srd1, *x_srd2;
	__m128i *x_dstd;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;
	mlib_s32 i, j, l, n = width * 3;
	mlib_s32 *dsh;
	mlib_s32 *ptr0, *ptr1, *ptr2;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_s32_0;
		ptr1 = (void *)src_s32_1;
		ptr2 = (void *)src_s32_2;
		dsh = (mlib_s32 *)dst_s32_0;
		x_dstd = (__m128i *)dsh;
		x_srd0 = (__m128i *)ptr0;
		x_srd1 = (__m128i *)ptr1;
		x_srd2 = (__m128i *)ptr2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 11); i += 12, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_sd2 = _mm_loadu_si128(x_srd2);

				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi32(x_sd2, x_zero);
				x_dd3 = _mm_unpackhi_epi32(x_sd2, x_zero);

				x_sd0 = _mm_unpacklo_epi64(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi64(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi64(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi64(x_dd1, x_dd3);

				_mm_storeu_si128(x_dstd, x_sd0);
				x_dstd = (__m128i *)((mlib_s32 *)x_dstd + 3);
				_mm_storeu_si128(x_dstd, x_sd1);
				x_dstd = (__m128i *)((mlib_s32 *)x_dstd + 3);

				_mm_storeu_si128(x_dstd, x_sd2);
				x_dstd = (__m128i *)((mlib_s32 *)x_dstd + 3);

				*((__m64 *)x_dstd) = *((__m64 *)&x_sd3);
				x_dstd = (__m128i *)((__m64 *)x_dstd + 1);
				*((mlib_s32 *)x_dstd) =
				    *((mlib_s32 *)&x_sd3 + 2);
				x_dstd = (__m128i *)((mlib_s32 *)x_dstd + 1);

				x_srd0++;
				x_srd1++;
				x_srd2++;
			}

			for (l = 0; i < (n - 5); i += 6, l++) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);

				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi32(x_sd2, x_zero);

				x_sd0 = _mm_unpacklo_epi64(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi64(x_dd0, x_dd2);

				_mm_storeu_si128(x_dstd, x_sd0);
				x_dstd = (__m128i *)((mlib_s32 *)x_dstd + 3);

				*((__m64 *)x_dstd) = *((__m64 *)&x_sd1);
				x_dstd = (__m128i *)((__m64 *)x_dstd + 1);
				*((mlib_s32 *)x_dstd) =
				    *((mlib_s32 *)&x_sd1 + 2);
				x_dstd = (__m128i *)((mlib_s32 *)x_dstd + 1);

				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_srd2 = (__m128i *)((__m64 *)x_srd2 + 1);
			}

		ptr0 = (mlib_s32 *)x_srd0;
		ptr1 = (mlib_s32 *)x_srd1;
		ptr2 = (mlib_s32 *)x_srd2;
		dsh = (mlib_s32 *)x_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 2); i += 3) {
			(*dsh++) = (*ptr0++);
			(*dsh++) = (*ptr1++);
			(*dsh++) = (*ptr2++);
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    const mlib_s32 *src_s32_2,
    const mlib_s32 *src_s32_3,
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
	mlib_s64 *dsh;
	mlib_s32 *ptr0, *ptr1, *ptr2, *ptr3;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_s32_0;
		ptr1 = (void *)src_s32_1;
		ptr2 = (void *)src_s32_2;
		ptr3 = (void *)src_s32_3;
		dsh = (mlib_s64 *)dst_s32_0;
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
			for (l = 0; i < (n - 15); i += 16, l++) {
				x_sd0 = _mm_load_si128(x_srd0);
				x_sd1 = _mm_load_si128(x_srd1);
				x_sd2 = _mm_load_si128(x_srd2);
				x_sd3 = _mm_load_si128(x_srd3);

				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_dd3 = _mm_unpackhi_epi32(x_sd2, x_sd3);

				x_sd0 = _mm_unpacklo_epi64(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi64(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi64(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi64(x_dd1, x_dd3);

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
			for (; i < (n - 7); i += 8) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);
				x_sd3 = _mm_loadl_epi64(x_srd3);

				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_dd2 = _mm_unpacklo_epi64(x_dd0, x_dd1);
				x_dd3 = _mm_unpackhi_epi64(x_dd0, x_dd1);

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
			for (l = 0; i < (n - 15); i += 16, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_sd2 = _mm_loadu_si128(x_srd2);
				x_sd3 = _mm_loadu_si128(x_srd3);

				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_dd3 = _mm_unpackhi_epi32(x_sd2, x_sd3);

				x_sd0 = _mm_unpacklo_epi64(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi64(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi64(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi64(x_dd1, x_dd3);

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
			for (; i < (n - 7); i += 8) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);
				x_sd3 = _mm_loadl_epi64(x_srd3);

				x_dd0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_dd1 = _mm_unpacklo_epi32(x_sd2, x_sd3);

				x_dd2 = _mm_unpacklo_epi64(x_dd0, x_dd1);
				x_dd3 = _mm_unpackhi_epi64(x_dd0, x_dd1);

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

		ptr0 = (mlib_s32 *)x_srd0;
		ptr1 = (mlib_s32 *)x_srd1;
		ptr2 = (mlib_s32 *)x_srd2;
		ptr3 = (mlib_s32 *)x_srd3;
		dsh = (mlib_s64 *)x_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 3); i += 4) {
			mlib_s64 a = (mlib_s64)((*ptr0++));
			mlib_s64 b = (mlib_s64)((*ptr1++));
			mlib_s64 c = (mlib_s64)((*ptr2++));
			mlib_s64 d = (mlib_s64)((*ptr3++));

			(*dsh++) = (b << 32) |
						(a & 0x00000000ffffffff);
			(*dsh++) = (d << 32) |
						(c & 0x00000000ffffffff);
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
		src_s32_3 += src3_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
