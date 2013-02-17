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

#pragma ident	"@(#)mlib_c_ImageChannelSplit_U8_24.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_U8
 *      mlib_ImageChannelSplit4_U8
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, n = width * 2;
	mlib_u16 *dp0, *dp1, *sp0;
	mlib_u16 s0, s1, s2;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = 0;

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i0++] = src_u8_0[i++];

			if ((mlib_addr)dst_u8_1 & 1) {
				dst_u8_1[i1++] = src_u8_0[i++];
			}
		} else if ((mlib_addr)dst_u8_1 & 1) {
			dst_u8_0[i0++] = src_u8_0[i++];
			dst_u8_1[i1++] = src_u8_0[i++];

			if (i < n) {
				dst_u8_0[i0++] = src_u8_0[i++];
			}
		}

		if (i0 == i1) {
			dp0 = (mlib_u16 *)(dst_u8_0 + i0);
			dp1 = (mlib_u16 *)(dst_u8_1 + i1);
		} else {
			dp1 = (mlib_u16 *)(dst_u8_0 + i0);
			dp0 = (mlib_u16 *)(dst_u8_1 + i1);
		}

		if (((mlib_addr)(src_u8_0 + i) & 1) == 0) {
			sp0 = (mlib_u16 *)(src_u8_0 + i);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
				s0 = (*sp0++);
				s1 = (*sp0++);
#ifdef _LITTLE_ENDIAN
				(*dp0++) = (s1 << 8) + (s0 & 0xff);
				(*dp1++) = (s1 & ~0xff) + (s0 >> 8);
#else /* _LITTLE_ENDIAN */
				(*dp0++) = (s0 & ~0xff) + (s1 >> 8);
				(*dp1++) = (s0 << 8) + (s1 & 0xff);
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			sp0 = (mlib_u16 *)(src_u8_0 + i - 1);
			s0 = (*sp0++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
				s1 = (*sp0++);
				s2 = (*sp0++);
#ifdef _LITTLE_ENDIAN
				(*dp0++) = (s0 >> 8) + (s1 & ~0xff);
				(*dp1++) = (s1 & 0xff) + (s2 << 8);
#else /* _LITTLE_ENDIAN */
				(*dp0++) = (s0 << 8) + (s1 & 0xff);
				(*dp1++) = (s1 & ~0xff) + (s2 >> 8);
#endif /* _LITTLE_ENDIAN */
				s0 = s2;
			}
		}

		if (i < (n - 2)) {
			dst_u8_1[i1 + 0] = src_u8_0[i + 0];
			dst_u8_0[i0 + 0] = src_u8_0[i + 1];
			dst_u8_1[i1 + 1] = src_u8_0[i + 2];
		} else if (i < (n - 1)) {
			dst_u8_0[i0] = src_u8_0[i + 0];
			dst_u8_1[i1] = src_u8_0[i + 1];
		} else if (i < n) {
			dst_u8_1[i1] = src_u8_0[i];
		}

		dst_u8_0 += dst0_str;
		dst_u8_1 += dst1_str;
		src_u8_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    mlib_u8 *dst_u8_2,
    mlib_u8 *dst_u8_3,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 dst3_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, n = width * 4;
	mlib_u32 var_uint;
	mlib_u8 *dst0;
	mlib_u8 *dst1;
	mlib_u8 *dst2;
	mlib_u8 *dst3;
	mlib_u8 *src;

	for (j = 0; j < height; j++) {

		i = 0;

		dst0 = dst_u8_0;
		dst1 = dst_u8_1;
		dst2 = dst_u8_2;
		dst3 = dst_u8_3;
		src = (void *)src_u8_0;

		if ((mlib_addr)(src_u8_0) & 3) {
			dst_u8_0[0] = (*src++);
			i++;

			if ((mlib_addr)(src_u8_0 + i) & 3) {
				dst_u8_1[0] = (*src++);
				i++;

				if ((mlib_addr)(src_u8_0 + i) & 3) {
					dst_u8_2[0] = (*src++);
					i++;
				}
			}
		}

		if (i == 3) {
			dst0 = dst_u8_3 + 0;
			dst1 = dst_u8_0 + 1;
			dst2 = dst_u8_1 + 1;
			dst3 = dst_u8_2 + 1;
		} else if (i == 2) {
			dst0 = dst_u8_2 + 0;
			dst1 = dst_u8_3 + 0;
			dst2 = dst_u8_0 + 1;
			dst3 = dst_u8_1 + 1;
		} else if (i == 1) {
			dst0 = dst_u8_1 + 0;
			dst1 = dst_u8_2 + 0;
			dst2 = dst_u8_3 + 0;
			dst3 = dst_u8_0 + 1;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 3); i += 4) {
			var_uint = ((mlib_u32 *)src)[0];
#ifdef _LITTLE_ENDIAN
			(*dst3++) = var_uint >> 24;
			(*dst2++) = var_uint >> 16;
			(*dst1++) = var_uint >> 8;
			(*dst0++) = var_uint;
#else /* _LITTLE_ENDIAN */
			(*dst3++) = var_uint;
			(*dst2++) = var_uint >> 8;
			(*dst1++) = var_uint >> 16;
			(*dst0++) = var_uint >> 24;
#endif /* _LITTLE_ENDIAN */
			src += 4;
		}

		if (i < (n - 2)) {
			dst_u8_1[width - 1] = (*src++);
			dst_u8_2[width - 1] = (*src++);
			dst_u8_3[width - 1] = (*src++);
		} else if (i < (n - 1)) {
			dst_u8_2[width - 1] = (*src++);
			dst_u8_3[width - 1] = (*src++);
		} else if (i < n) {
			dst_u8_3[width - 1] = (*src++);
		}

		dst_u8_0 += dst0_str;
		dst_u8_1 += dst1_str;
		dst_u8_2 += dst2_str;
		dst_u8_3 += dst3_str;
		src_u8_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
