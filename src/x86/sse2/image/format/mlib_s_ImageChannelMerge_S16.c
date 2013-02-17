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

#pragma ident	"@(#)mlib_s_ImageChannelMerge_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_S16
 *      mlib_ImageChannelMerge3_S16
 *      mlib_ImageChannelMerge4_S16
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
mlib_ImageChannelMerge2_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	mlib_s32 i, j, l, n = width * 2;
	mlib_s32 *dsh;
	mlib_s16 *ptr0, *ptr1;
	__m128i *x_srd0, *x_srd1;
	__m128i *x_dstd;
	__m128i x_sd0, x_sd1, x_dd0, x_dd1;
	__m64 *m_srd0, *m_srd1;
	__m64 *m_dstd;
	__m64 m_sd0, m_sd1, m_dd0, m_dd1;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_s16_0;
		ptr1 = (void *)src_s16_1;
		dsh = (mlib_s32 *)dst_s16_0;
		x_dstd = (__m128i *)dsh;
		x_srd0 = (__m128i *)ptr0;
		x_srd1 = (__m128i *)ptr1;

		if ((!((mlib_addr)ptr0 & 15)) && (!((mlib_addr)ptr1 & 15)) &&
		    (!((mlib_addr)dsh & 15))) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 15); i += 16, l++) {
				x_sd0 = _mm_load_si128(x_srd0);
				x_sd1 = _mm_load_si128(x_srd1);
				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
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
			for (; i < (n - 7); i += 8) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				_mm_store_si128(x_dstd, x_dd0);
				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_dstd++;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; i < (n - 15); i += 16, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
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
			for (; i < (n - 7); i += 8) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				_mm_storeu_si128(x_dstd, x_dd0);
				x_srd0 = (__m128i *)((__m64 *)x_srd0 + 1);
				x_srd1 = (__m128i *)((__m64 *)x_srd1 + 1);
				x_dstd++;
			}

		}

		ptr0 = (mlib_s16 *)x_srd0;
		ptr1 = (mlib_s16 *)x_srd1;
		dsh = (mlib_s32 *)x_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 1); i += 2) {
			mlib_s32 a = (mlib_s32)((*ptr0++));
			mlib_s32 b = (mlib_s32)((*ptr1++));

			(*dsh++) = (b << 16) | (a & 0x0000ffff);
		}

		dst_s16_0 += dst_stride;
		src_s16_0 += src0_stride;
		src_s16_1 += src1_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge3_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    const mlib_s16 *src_s16_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride)
{
	mlib_s32 i, j, n = width * 3;
	mlib_s16 *f_ptr, *s_ptr, *t_ptr;
	mlib_u32 *dp, s0, s1, s2;

	for (j = 0; j < height; j++) {

		i = 0;

		if ((mlib_addr)dst_s16_0 & 3) {
			dst_s16_0[i] = src_s16_0[0];
			f_ptr = (mlib_s16 *)src_s16_1;
			s_ptr = (mlib_s16 *)src_s16_2;
			t_ptr = (mlib_s16 *)(src_s16_0 + 1);
			i++;
		} else {
			f_ptr = (mlib_s16 *)src_s16_0;
			s_ptr = (mlib_s16 *)src_s16_1;
			t_ptr = (mlib_s16 *)src_s16_2;
		}

		dp = (mlib_u32 *)(dst_s16_0 + i);

		if (n > 5) {
			if (((mlib_addr)f_ptr & 3) ||
			    ((mlib_addr)s_ptr & 3) || ((mlib_addr)t_ptr & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < (n - 5); i += 6) {
					mlib_u32 s0_0 = *(mlib_u16 *)f_ptr++;
					mlib_u32 s0_1 = *(mlib_u16 *)f_ptr++;
					mlib_u32 s1_0 = *(mlib_u16 *)s_ptr++;
					mlib_u32 s1_1 = *(mlib_u16 *)s_ptr++;
					mlib_u32 s2_0 = *(mlib_u16 *)t_ptr++;
					mlib_u32 s2_1 = *(mlib_u16 *)t_ptr++;

#ifdef _LITTLE_ENDIAN
					dp[0] = s0_0 + (s1_0 << 16);
					dp[1] = s2_0 + (s0_1 << 16);
					dp[2] = s1_1 + (s2_1 << 16);
#else /* _LITTLE_ENDIAN */
					dp[0] = (s0_0 << 16) + s1_0;
					dp[1] = (s2_0 << 16) + s0_1;
					dp[2] = (s1_1 << 16) + s2_1;
#endif /* _LITTLE_ENDIAN */
					dp += 3;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < (n - 5); i += 6) {
					s0 = *(mlib_u32 *)f_ptr;
					s1 = *(mlib_u32 *)s_ptr;
					s2 = *(mlib_u32 *)t_ptr;
#ifdef _LITTLE_ENDIAN
					dp[0] = (s0 & 0xffff) + (s1 << 16);
					dp[1] = (s2 & 0xffff) + (s0 & ~0xffff);
					dp[2] = (s1 >> 16) + (s2 & ~0xffff);
#else /* _LITTLE_ENDIAN */
					dp[0] = (s0 & ~0xFFFF) + (s1 >> 16);
					dp[1] = (s2 & ~0xFFFF) + (s0 & 0xFFFF);
					dp[2] = (s1 << 16) + (s2 & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
					f_ptr += 2;
					s_ptr += 2;
					t_ptr += 2;
					dp += 3;
				}
			}
		}

		if (i < (n - 2)) {
			dst_s16_0[i + 0] = (*f_ptr++);
			dst_s16_0[i + 1] = (*s_ptr++);
			dst_s16_0[i + 2] = (*t_ptr++);
			i += 3;
		}

		if (i < n) {
			dst_s16_0[i + 0] = *f_ptr;
			dst_s16_0[i + 1] = *s_ptr;
		}

		dst_s16_0 += dst_stride;
		src_s16_0 += src0_stride;
		src_s16_1 += src1_stride;
		src_s16_2 += src2_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    const mlib_s16 *src_s16_2,
    const mlib_s16 *src_s16_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride)
{
	mlib_s32 i, j, l, n = width * 4;
	mlib_s16 *dsh;
	mlib_s16 *ptr0, *ptr1, *ptr2, *ptr3;
	__m128i *x_srd0, *x_srd1, *x_srd2, *x_srd3;
	__m128i *x_dstd;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		ptr0 = (void *)src_s16_0;
		ptr1 = (void *)src_s16_1;
		ptr2 = (void *)src_s16_2;
		ptr3 = (void *)src_s16_3;
		dsh = (mlib_s16 *)dst_s16_0;
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
			for (l = 0; i < (n - 31); i += 32, l++) {
				x_sd0 = _mm_load_si128(x_srd0);
				x_sd1 = _mm_load_si128(x_srd1);
				x_sd2 = _mm_load_si128(x_srd2);
				x_sd3 = _mm_load_si128(x_srd3);

				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_dd3 = _mm_unpackhi_epi16(x_sd2, x_sd3);

				x_sd0 = _mm_unpacklo_epi32(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi32(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi32(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi32(x_dd1, x_dd3);

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
			for (; i < (n - 15); i += 16) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);
				x_sd3 = _mm_loadl_epi64(x_srd3);

				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_dd1 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_dd2 = _mm_unpacklo_epi32(x_dd0, x_dd1);
				x_dd3 = _mm_unpackhi_epi32(x_dd0, x_dd1);

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
			for (l = 0; i < (n - 31); i += 32, l++) {
				x_sd0 = _mm_loadu_si128(x_srd0);
				x_sd1 = _mm_loadu_si128(x_srd1);
				x_sd2 = _mm_loadu_si128(x_srd2);
				x_sd3 = _mm_loadu_si128(x_srd3);

				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_dd2 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_dd3 = _mm_unpackhi_epi16(x_sd2, x_sd3);

				x_sd0 = _mm_unpacklo_epi32(x_dd0, x_dd2);
				x_sd1 = _mm_unpackhi_epi32(x_dd0, x_dd2);
				x_sd2 = _mm_unpacklo_epi32(x_dd1, x_dd3);
				x_sd3 = _mm_unpackhi_epi32(x_dd1, x_dd3);

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
			for (; i < (n - 15); i += 16) {
				x_sd0 = _mm_loadl_epi64(x_srd0);
				x_sd1 = _mm_loadl_epi64(x_srd1);
				x_sd2 = _mm_loadl_epi64(x_srd2);
				x_sd3 = _mm_loadl_epi64(x_srd3);

				x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_dd1 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_dd2 = _mm_unpacklo_epi32(x_dd0, x_dd1);
				x_dd3 = _mm_unpackhi_epi32(x_dd0, x_dd1);

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

		ptr0 = (mlib_s16 *)x_srd0;
		ptr1 = (mlib_s16 *)x_srd1;
		ptr2 = (mlib_s16 *)x_srd2;
		ptr3 = (mlib_s16 *)x_srd3;
		dsh = (mlib_s16 *)x_dstd;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 3); i += 4) {
			*dsh++ = *ptr0++;
			*dsh++ = *ptr1++;
			*dsh++ = *ptr2++;
			*dsh++ = *ptr3++;
		}

		dst_s16_0 += dst_stride;
		src_s16_0 += src0_stride;
		src_s16_1 += src1_stride;
		src_s16_2 += src2_stride;
		src_s16_3 += src3_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
