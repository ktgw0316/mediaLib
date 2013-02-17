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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_ImageChannelMerge_FS32.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_FS32
 *      mlib_ImageChannelMerge3_FS32
 *      mlib_ImageChannelMerge4_FS32
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

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
	mlib_s32 j, i;

#ifdef _NO_LONGLONG

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_s32_0[2 * i] = src_s32_0[i];
			dst_s32_0[2 * i + 1] = src_s32_1[i];
		}

		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		dst_s32_0 += dst_stride;
	}

#else /* _NO_LONGLONG */

	mlib_s32 n = width * 2;
	mlib_u64 *dp, s0, s1, mask = 0xFFFFFFFF;
	mlib_s32 *f_ptr, *s_ptr;

	for (j = 0; j < height; j++) {
		i = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[0] = src_s32_0[0];
			f_ptr = (mlib_s32 *)(src_s32_1);
			s_ptr = (mlib_s32 *)(src_s32_0 + 1);
			i++;
		} else {
			f_ptr = (mlib_s32 *)src_s32_0;
			s_ptr = (mlib_s32 *)src_s32_1;
		}

		if (((mlib_addr)s_ptr & 7) && ((mlib_addr)f_ptr & 7) &&
		    (i < (n - 1))) {
			dst_s32_0[i + 0] = (*f_ptr++);
			dst_s32_0[i + 1] = (*s_ptr++);
			i += 2;
		}

		dp = (mlib_u64 *)(dst_s32_0 + i);

		if (((mlib_addr)s_ptr & 7) == ((mlib_addr)f_ptr & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				s0 = *(mlib_u64 *)f_ptr;
				s1 = *(mlib_u64 *)s_ptr;

#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & mask) + (s1 << 32);
				dp[1] = (s0 >> 32) + (s1 & ~mask);
#else /* _LITTLE_ENDIAN */
				dp[0] = (s0 & ~mask) + (s1 >> 32);
				dp[1] = (s0 << 32) + (s1 & mask);
#endif /* _LITTLE_ENDIAN */
				f_ptr += 2;
				s_ptr += 2;
				dp += 2;
			}
		} else if ((mlib_addr)f_ptr & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				mlib_u64 s0_0 = *(mlib_u32 *)(f_ptr++);
				mlib_u64 s0_1 = *(mlib_u32 *)(f_ptr++);

				s1 = *(mlib_u64 *)s_ptr;
#ifdef _LITTLE_ENDIAN
				dp[0] = s0_0 + (s1 << 32);
				dp[1] = s0_1 + (s1 & ~mask);
#else /* _LITTLE_ENDIAN */
				dp[0] = (s0_0 << 32) + (s1 >> 32);
				dp[1] = (s0_1 << 32) + (s1 & mask);
#endif /* _LITTLE_ENDIAN */
				s_ptr += 2;
				dp += 2;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				mlib_u64 s1_0 = *(mlib_u32 *)(s_ptr++);
				mlib_u64 s1_1 = *(mlib_u32 *)(s_ptr++);

				s0 = *(mlib_u64 *)f_ptr;
#ifdef _LITTLE_ENDIAN
				dp[0] = (s0 & mask) + (s1_0 << 32);
				dp[1] = (s0 >> 32) + (s1_1 << 32);
#else /* _LITTLE_ENDIAN */
				dp[0] = (s0 & ~mask) + s1_0;
				dp[1] = (s0 << 32) + s1_1;
#endif /* _LITTLE_ENDIAN */
				f_ptr += 2;
				dp += 2;
			}
		}

		if (i < (n - 1)) {
			s0 = *((mlib_u32 *)f_ptr);
			s1 = *((mlib_u32 *)s_ptr);
#ifdef _LITTLE_ENDIAN
			dp[0] = (s1 << 32) + s0;
#else /* _LITTLE_ENDIAN */
			dp[0] = (s0 << 32) + s1;
#endif /* _LITTLE_ENDIAN */
			i += 2;
			f_ptr++;
			s_ptr++;
		}

		if (i < n)
			dst_s32_0[i] = *f_ptr;

		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		dst_s32_0 += dst_stride;
	}

#endif /* _NO_LONGLONG */

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
	mlib_s32 j, i;

#ifdef _NO_LONGLONG

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_s32_0[3 * i] = src_s32_0[i];
			dst_s32_0[3 * i + 1] = src_s32_1[i];
			dst_s32_0[3 * i + 2] = src_s32_2[i];
		}

		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
		dst_s32_0 += dst_stride;
	}

#else /* _NO_LONGLONG */

	mlib_s32 n = width * 3;
	mlib_s32 *f_ptr, *s_ptr, *t_ptr;
	mlib_u64 *dp, s0, s1, s2;
	mlib_u64 mask = 0x0FFFFFFFF;

	for (j = 0; j < height; j++) {

		i = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[i] = src_s32_0[0];
			f_ptr = (mlib_s32 *)src_s32_1;
			s_ptr = (mlib_s32 *)src_s32_2;
			t_ptr = (mlib_s32 *)(src_s32_0 + 1);
			i++;
		} else {
			f_ptr = (mlib_s32 *)src_s32_0;
			s_ptr = (mlib_s32 *)src_s32_1;
			t_ptr = (mlib_s32 *)src_s32_2;
		}

		dp = (mlib_u64 *)(dst_s32_0 + i);

		if (n > 5) {
			if (((mlib_addr)f_ptr & 7) ||
			    ((mlib_addr)s_ptr & 7) || ((mlib_addr)t_ptr & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < (n - 5); i += 6) {
					mlib_u64 s0_0 = *(mlib_u32 *)f_ptr++;
					mlib_u64 s0_1 = *(mlib_u32 *)f_ptr++;
					mlib_u64 s1_0 = *(mlib_u32 *)s_ptr++;
					mlib_u64 s1_1 = *(mlib_u32 *)s_ptr++;
					mlib_u64 s2_0 = *(mlib_u32 *)t_ptr++;
					mlib_u64 s2_1 = *(mlib_u32 *)t_ptr++;

#ifdef _LITTLE_ENDIAN
					dp[0] = s0_0 + (s1_0 << 32);
					dp[1] = s2_0 + (s0_1 << 32);
					dp[2] = s1_1 + (s2_1 << 32);
#else /* _LITTLE_ENDIAN */
					dp[0] = (s0_0 << 32) + s1_0;
					dp[1] = (s2_0 << 32) + s0_1;
					dp[2] = (s1_1 << 32) + s2_1;
#endif /* _LITTLE_ENDIAN */
					dp += 3;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < (n - 5); i += 6) {
					s0 = *(mlib_u64 *)f_ptr;
					s1 = *(mlib_u64 *)s_ptr;
					s2 = *(mlib_u64 *)t_ptr;
#ifdef _LITTLE_ENDIAN
					dp[0] = (s0 & mask) + (s1 << 32);
					dp[1] = (s2 & mask) + (s0 & ~mask);
					dp[2] = (s1 >> 32) + (s2 & ~mask);
#else /* _LITTLE_ENDIAN */
					dp[0] = (s0 & ~mask) + (s1 >> 32);
					dp[1] = (s2 & ~mask) + (s0 & mask);
					dp[2] = (s1 << 32) + (s2 & mask);
#endif /* _LITTLE_ENDIAN */
					f_ptr += 2;
					s_ptr += 2;
					t_ptr += 2;
					dp += 3;
				}
			}
		}

		if (i < (n - 2)) {
			dst_s32_0[i + 0] = (*f_ptr++);
			dst_s32_0[i + 1] = (*s_ptr++);
			dst_s32_0[i + 2] = (*t_ptr++);
			i += 3;
		}

		if (i < n) {
			dst_s32_0[i + 0] = *f_ptr;
			dst_s32_0[i + 1] = *s_ptr;
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
	}

#endif /* _NO_LONGLONG */

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
	mlib_s32 j, i;

#ifdef _NO_LONGLONG

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_s32_0[4 * i] = src_s32_0[i];
			dst_s32_0[4 * i + 1] = src_s32_1[i];
			dst_s32_0[4 * i + 2] = src_s32_2[i];
			dst_s32_0[4 * i + 3] = src_s32_3[i];
		}

		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
		src_s32_3 += src3_stride;
		dst_s32_0 += dst_stride;
	}

#else /* _NO_LONGLONG */

	mlib_s32 n = width << 2;
	mlib_s32 *fi_ptr, *se_ptr, *th_ptr, *fo_ptr;
	mlib_u64 *dp, s0, s1, s2, s3;
	mlib_u64 mask = 0x0ffffffff;

	for (j = 0; j < height; j++) {

		i = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[i] = src_s32_0[0];
			fi_ptr = (mlib_s32 *)src_s32_1;
			se_ptr = (mlib_s32 *)src_s32_2;
			th_ptr = (mlib_s32 *)src_s32_3;
			fo_ptr = (mlib_s32 *)(src_s32_0 + 1);
			i++;
		} else {
			fi_ptr = (mlib_s32 *)src_s32_0;
			se_ptr = (mlib_s32 *)src_s32_1;
			th_ptr = (mlib_s32 *)src_s32_2;
			fo_ptr = (mlib_s32 *)src_s32_3;
		}

		dp = (mlib_u64 *)(dst_s32_0 + i);

		if (n > 7) {
			if (((mlib_addr)fi_ptr & 7) ||
			    ((mlib_addr)se_ptr & 7) ||
			    ((mlib_addr)th_ptr & 7) ||
			    ((mlib_addr)fo_ptr & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < (n - 7); i += 8) {
					mlib_u64 s0_0 = *(mlib_u32 *)fi_ptr++;
					mlib_u64 s0_1 = *(mlib_u32 *)fi_ptr++;
					mlib_u64 s1_0 = *(mlib_u32 *)se_ptr++;
					mlib_u64 s1_1 = *(mlib_u32 *)se_ptr++;
					mlib_u64 s2_0 = *(mlib_u32 *)th_ptr++;
					mlib_u64 s2_1 = *(mlib_u32 *)th_ptr++;
					mlib_u64 s3_0 = *(mlib_u32 *)fo_ptr++;
					mlib_u64 s3_1 = *(mlib_u32 *)fo_ptr++;

#ifdef _LITTLE_ENDIAN
					dp[0] = (s1_0 << 32) + s0_0;
					dp[1] = (s3_0 << 32) + s2_0;
					dp[2] = (s1_1 << 32) + s0_1;
					dp[3] = (s3_1 << 32) + s2_1;
#else /* _LITTLE_ENDIAN */
					dp[0] = (s0_0 << 32) + s1_0;
					dp[1] = (s2_0 << 32) + s3_0;
					dp[2] = (s0_1 << 32) + s1_1;
					dp[3] = (s2_1 << 32) + s3_1;
#endif /* _LITTLE_ENDIAN */
					dp += 4;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < (n - 7); i += 8) {
					s0 = *(mlib_u64 *)fi_ptr;
					s1 = *(mlib_u64 *)se_ptr;
					s2 = *(mlib_u64 *)th_ptr;
					s3 = *(mlib_u64 *)fo_ptr;
#ifdef _LITTLE_ENDIAN
					dp[0] = (s0 & mask) + (s1 << 32);
					dp[1] = (s2 & mask) + (s3 << 32);
					dp[2] = (s0 >> 32) + (s1 & ~mask);
					dp[3] = (s2 >> 32) + (s3 & ~mask);
#else /* _LITTLE_ENDIAN */
					dp[0] = (s0 & ~mask) + (s1 >> 32);
					dp[1] = (s2 & ~mask) + (s3 >> 32);
					dp[2] = (s0 << 32) + (s1 & mask);
					dp[3] = (s2 << 32) + (s3 & mask);
#endif /* _LITTLE_ENDIAN */
					fi_ptr += 2;
					se_ptr += 2;
					th_ptr += 2;
					fo_ptr += 2;
					dp += 4;
				}
			}
		}

		if (i < (n - 3)) {
			dst_s32_0[i + 0] = (*fi_ptr++);
			dst_s32_0[i + 1] = (*se_ptr++);
			dst_s32_0[i + 2] = (*th_ptr++);
			dst_s32_0[i + 3] = (*fo_ptr++);
			i += 4;
		}

		if (i < n) {
			dst_s32_0[i + 0] = *fi_ptr;
			dst_s32_0[i + 1] = *se_ptr;
			dst_s32_0[i + 2] = *th_ptr;
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
		src_s32_3 += src3_stride;
	}

#endif /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
