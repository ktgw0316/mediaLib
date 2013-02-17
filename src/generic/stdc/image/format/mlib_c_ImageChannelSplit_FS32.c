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

#pragma ident	"@(#)mlib_c_ImageChannelSplit_FS32.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_FS32
 *      mlib_ImageChannelSplit3_FS32
 *      mlib_ImageChannelSplit4_FS32
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i;

#ifdef _NO_LONGLONG

	dst0_str >>= 2;
	dst1_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_s32_0[i] = src_s32_0[2 * i];
			dst_s32_1[i] = src_s32_0[2 * i + 1];
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		src_s32_0 += src0_str;
	}

#else /* _NO_LONGLONG */

	mlib_s32 i0, i1, n = width * 2;
	mlib_u64 *dp0, *dp1, *sp0;
	mlib_u64 s0, s1, s2;

	dst0_str >>= 2;
	dst1_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[i0++] = src_s32_0[i++];

			if ((mlib_addr)dst_s32_1 & 7) {
				dst_s32_1[i1++] = src_s32_0[i++];
			}
		} else if ((mlib_addr)dst_s32_1 & 7) {
			dst_s32_0[i0++] = src_s32_0[i++];
			dst_s32_1[i1++] = src_s32_0[i++];

			if (i < n) {
				dst_s32_0[i0++] = src_s32_0[i++];
			}
		}

		if (i0 == i1) {
			dp0 = (mlib_u64 *)(dst_s32_0 + i0);
			dp1 = (mlib_u64 *)(dst_s32_1 + i1);
		} else {
			dp1 = (mlib_u64 *)(dst_s32_0 + i0);
			dp0 = (mlib_u64 *)(dst_s32_1 + i1);
		}

		if (((mlib_addr)(src_s32_0 + i) & 7) == 0) {
			sp0 = (mlib_u64 *)(src_s32_0 + i);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
				s0 = (*sp0++);
				s1 = (*sp0++);
#ifdef _LITTLE_ENDIAN
				(*dp0++) = (s1 << 32) + (s0 & 0xffffffff);
				(*dp1++) =
				    (s1 & ~((mlib_u64)0xffffffff)) + (s0 >> 32);
#else /* _LITTLE_ENDIAN */
				(*dp0++) =
				    (s0 & ~((mlib_u64)0x0ffffffff)) +
				    (s1 >> 32);
				(*dp1++) = (s0 << 32) + (s1 & 0xffffffff);
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			sp0 = (mlib_u64 *)(src_s32_0 + i - 1);
			s0 = (*sp0++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
				s1 = (*sp0++);
				s2 = (*sp0++);
#ifdef _LITTLE_ENDIAN
				(*dp0++) =
				    (s0 >> 32) + (s1 & ~((mlib_u64)0xffffffff));
				(*dp1++) = (s1 & 0xffffffff) + (s2 << 32);
#else /* _LITTLE_ENDIAN */
				(*dp0++) = (s0 << 32) + (s1 & 0xffffffff);
				(*dp1++) =
				    (s1 & ~((mlib_u64)0x0ffffffff)) +
				    (s2 >> 32);
#endif /* _LITTLE_ENDIAN */
				s0 = s2;
			}
		}

		if (i < (n - 2)) {
			dst_s32_1[i1 + 0] = src_s32_0[i + 0];
			dst_s32_0[i0 + 0] = src_s32_0[i + 1];
			dst_s32_1[i1 + 1] = src_s32_0[i + 2];
		} else if (i < (n - 1)) {
			dst_s32_0[i0] = src_s32_0[i + 0];
			dst_s32_1[i1] = src_s32_0[i + 1];
		} else if (i < n) {
			dst_s32_1[i1] = src_s32_0[i];
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		src_s32_0 += src0_str;
	}

#endif /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    mlib_s32 *dst_s32_2,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i;

#ifdef _NO_LONGLONG

	dst0_str >>= 2;
	dst1_str >>= 2;
	dst2_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_s32_0[i] = src_s32_0[3 * i];
			dst_s32_1[i] = src_s32_0[3 * i + 1];
			dst_s32_2[i] = src_s32_0[3 * i + 2];
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		dst_s32_2 += dst2_str;
		src_s32_0 += src0_str;
	}

#else /* _NO_LONGLONG */

	mlib_s32 n = width * 3;
	mlib_u64 udb0, udb1, udb2;
	mlib_s32 *dst0, *dst1, *dst2, *src;

	dst0_str >>= 2;
	dst1_str >>= 2;
	dst2_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

		i = 0;

		dst0 = dst_s32_0;
		dst1 = dst_s32_1;
		dst2 = dst_s32_2;
		src = (void *)src_s32_0;

		if ((mlib_addr)(src_s32_0 + i) & 7) {
			dst_s32_0[0] = (*src++);
			i++;

			if ((mlib_addr)(src_s32_0 + i) & 7) {
				dst_s32_1[0] = (*src++);
				i++;

				if ((mlib_addr)(src_s32_0 + i) & 7) {
					dst_s32_2[0] = (*src++);
					i++;
				}
			}
		}

		if (i == 3) {
			dst0 = dst_s32_0 + 1;
			dst1 = dst_s32_1 + 1;
			dst2 = dst_s32_2 + 1;
		} else if (i == 2) {
			dst1 = dst_s32_0 + 1;
			dst2 = dst_s32_1 + 1;
			dst0 = dst_s32_2 + 0;
		} else if (i == 1) {
			dst2 = dst_s32_0 + 1;
			dst0 = dst_s32_1 + 0;
			dst1 = dst_s32_2 + 0;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 5); i += 6) {
			udb0 = ((mlib_u64 *)src)[0];
			udb1 = ((mlib_u64 *)src)[1];
			udb2 = ((mlib_u64 *)src)[2];
#ifdef _LITTLE_ENDIAN
			dst0[0] = udb0;
			dst0[1] = udb1 >> 32;

			dst1[0] = udb0 >> 32;
			dst1[1] = udb2;

			dst2[0] = udb1;
			dst2[1] = udb2 >> 32;
#else /* _LITTLE_ENDIAN */
			dst1[0] = udb0;
			dst0[0] = udb0 >> 32;

			dst0[1] = udb1;
			dst2[0] = udb1 >> 32;

			dst2[1] = udb2;
			dst1[1] = udb2 >> 32;
#endif /* _LITTLE_ENDIAN */
			dst0 += 2;
			dst1 += 2;
			dst2 += 2;
			src += 6;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 2); i += 3) {
			(*dst0++) = (*src++);
			(*dst1++) = (*src++);
			(*dst2++) = (*src++);
		}

		if (i < (n - 1)) {
			dst_s32_1[width - 1] = (*src++);
			dst_s32_2[width - 1] = (*src++);
		} else if (i < n) {
			dst_s32_2[width - 1] = (*src++);
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		dst_s32_2 += dst2_str;
		src_s32_0 += src0_str;
	}

#endif /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    mlib_s32 *dst_s32_2,
    mlib_s32 *dst_s32_3,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 dst3_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i;

#ifdef _NO_LONGLONG

	dst0_str >>= 2;
	dst1_str >>= 2;
	dst2_str >>= 2;
	dst3_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_s32_0[i] = src_s32_0[4 * i];
			dst_s32_1[i] = src_s32_0[4 * i + 1];
			dst_s32_2[i] = src_s32_0[4 * i + 2];
			dst_s32_3[i] = src_s32_0[4 * i + 3];
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		dst_s32_2 += dst2_str;
		dst_s32_3 += dst3_str;
		src_s32_0 += src0_str;
	}

#else /* _NO_LONGLONG */

	mlib_s32 n = width * 4;
	mlib_u64 udb0, udb1;
	mlib_s32 *dst0, *dst1, *dst2, *dst3, *src;

	dst0_str >>= 2;
	dst1_str >>= 2;
	dst2_str >>= 2;
	dst3_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

		i = 0;

		dst0 = dst_s32_0;
		dst1 = dst_s32_1;
		dst2 = dst_s32_2;
		dst3 = dst_s32_3;
		src = (void *)src_s32_0;

		if ((mlib_addr)src_s32_0 & 7) {
			dst_s32_0[0] = (*src++);
			dst0 = dst_s32_1 + 0;
			dst1 = dst_s32_2 + 0;
			dst2 = dst_s32_3 + 0;
			dst3 = dst_s32_0 + 1;
			i++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 3); i += 4) {
			udb0 = ((mlib_u64 *)src)[0];
			udb1 = ((mlib_u64 *)src)[1];
#ifdef _LITTLE_ENDIAN
			(*dst0++) = udb0;
			(*dst1++) = udb0 >> 32;
			(*dst2++) = udb1;
			(*dst3++) = udb1 >> 32;
#else /* _LITTLE_ENDIAN */
			(*dst1++) = udb0;
			(*dst0++) = udb0 >> 32;
			(*dst3++) = udb1;
			(*dst2++) = udb1 >> 32;
#endif /* _LITTLE_ENDIAN */
			src += 4;
		}

		if (i < n) {
			dst_s32_1[width - 1] = (*src++);
			dst_s32_2[width - 1] = (*src++);
			dst_s32_3[width - 1] = (*src++);
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		dst_s32_2 += dst2_str;
		dst_s32_3 += dst3_str;
		src_s32_0 += src0_str;
	}

#endif /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
