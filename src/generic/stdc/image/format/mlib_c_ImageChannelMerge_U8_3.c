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

#pragma ident	"@(#)mlib_c_ImageChannelMerge_U8_3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge3_U8
 *
 * DESCRIPTION
 *      Internal functions.
 *      There is one function on this file for work with 3 channel
 *      image only.
 */

#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge3_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    const mlib_u8 *src_u8_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride)
{
	mlib_s32 i, j, k, n = width * 3;
	mlib_u16 *dp, s0, s1, s2;

	width *= 3;
	for (j = 0, i = 0; j < height; j++, i = 0) {
		mlib_u8 *ptr0 = (void *)src_u8_0;
		mlib_u8 *ptr1 = (void *)src_u8_1;
		mlib_u8 *ptr2 = (void *)src_u8_2;

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i] = *src_u8_0;
			ptr0 = (void *)src_u8_1;
			ptr1 = (void *)src_u8_2;
			ptr2 = (void *)(src_u8_0 + 1);
			i++;
		}

		dp = (mlib_u16 *)(dst_u8_0 + i);

		if (((mlib_addr)ptr0 & 1) ||
		    ((mlib_addr)ptr1 & 1) || ((mlib_addr)ptr2 & 1)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (k = 0; i < (n - 6); k += 3, i += 6) {
#ifdef _LITTLE_ENDIAN
				s0 = (mlib_u16)((*ptr0++));
				s1 = (mlib_u16)((*ptr1++));
				s2 = (mlib_u16)((*ptr2++));
				s0 |= (mlib_u16)((*ptr0++)) << 8;
				s1 |= (mlib_u16)((*ptr1++)) << 8;
				s2 |= (mlib_u16)((*ptr2++)) << 8;
				dp[k + 2] = (s2 & ~0xFF) + (s1 >> 8);
				dp[k + 1] = (s0 & ~0xFF) + (s2 & 0xFF);
				dp[k + 0] = (s1 << 8) + (s0 & 0xFF);
#else /* _LITTLE_ENDIAN */
				s0 = (mlib_u16)((*ptr0++)) << 8;
				s1 = (mlib_u16)((*ptr1++)) << 8;
				s2 = (mlib_u16)((*ptr2++)) << 8;
				s0 |= (mlib_u16)((*ptr0++));
				s1 |= (mlib_u16)((*ptr1++));
				s2 |= (mlib_u16)((*ptr2++));
				dp[k + 0] = (s0 & ~0xFF) + (s1 >> 8);
				dp[k + 1] = (s2 & ~0xFF) + (s0 & 0xFF);
				dp[k + 2] = (s1 << 8) + (s2 & 0xFF);
#endif /* _LITTLE_ENDIAN */
			}
		} else {
/*  ALIGNED CASE  */
			mlib_u16 *sp0 = (mlib_u16 *)ptr0;
			mlib_u16 *sp1 = (mlib_u16 *)ptr1;
			mlib_u16 *sp2 = (mlib_u16 *)ptr2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (k = 0; i < (n - 6); k += 3, i += 6) {
				s0 = (*sp0++);
				s1 = (*sp1++);
				s2 = (*sp2++);
#ifdef _LITTLE_ENDIAN
				dp[k + 2] = (s2 & ~0xFF) + (s1 >> 8);
				dp[k + 1] = (s0 & ~0xFF) + (s2 & 0xFF);
				dp[k + 0] = (s1 << 8) + (s0 & 0xFF);
#else /* _LITTLE_ENDIAN */
				dp[k + 0] = (s0 & ~0xFF) + (s1 >> 8);
				dp[k + 1] = (s2 & ~0xFF) + (s0 & 0xFF);
				dp[k + 2] = (s1 << 8) + (s2 & 0xFF);
#endif /* _LITTLE_ENDIAN */
			}

			ptr0 = (mlib_u8 *)sp0;
			ptr1 = (mlib_u8 *)sp1;
			ptr2 = (mlib_u8 *)sp2;
		}

		for (; i < (n - 2); i += 3) {
			dst_u8_0[i + 0] = (*ptr0++);
			dst_u8_0[i + 1] = (*ptr1++);
			dst_u8_0[i + 2] = (*ptr2++);
		}

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i + 0] = *ptr0;
			dst_u8_0[i + 1] = *ptr1;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
		src_u8_2 += src2_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
