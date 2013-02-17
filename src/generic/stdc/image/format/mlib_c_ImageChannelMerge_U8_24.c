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

#pragma ident	"@(#)mlib_c_ImageChannelMerge_U8_24.c	9.2	07/10/09 SMI"

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
	mlib_s32 i, j, n = width * 2;
	mlib_u16 *dsh;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		mlib_u8 *ptr0 = (void *)src_u8_0;
		mlib_u8 *ptr1 = (void *)src_u8_1;

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i] = *src_u8_0;
			ptr0 = (void *)src_u8_1;
			ptr1 = (void *)(src_u8_0 + 1);
			i++;
		}

		dsh = (mlib_u16 *)(dst_u8_0 + i);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 1); i += 2) {
			mlib_u16 a = (mlib_u16)((*ptr0++));
			mlib_u16 b = (mlib_u16)((*ptr1++));

#ifdef _LITTLE_ENDIAN
			(*dsh++) = a | (b << 8);
#else /* _LITTLE_ENDIAN */
			(*dsh++) = (a << 8) | b;
#endif /* _LITTLE_ENDIAN */
		}

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i] = *ptr0;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
	}

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
	mlib_s32 j, i, preread, n = width * 4;
	mlib_u8 *ptr0 = NULL, *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL;

	for (j = 0; j < height; j++) {
		preread = (4 - (((mlib_addr)dst_u8_0) & 3)) & 3;
		i = preread;

		if (preread == 3) {
			dst_u8_0[0] = src_u8_0[0];
			dst_u8_0[1] = src_u8_1[0];
			dst_u8_0[2] = src_u8_2[0];
			ptr0 = (mlib_u8 *)src_u8_3;
			ptr1 = (mlib_u8 *)(src_u8_0 + 1);
			ptr2 = (mlib_u8 *)(src_u8_1 + 1);
			ptr3 = (mlib_u8 *)(src_u8_2 + 1);
		} else if (preread == 2) {
			dst_u8_0[0] = src_u8_0[0];
			dst_u8_0[1] = src_u8_1[0];
			ptr0 = (mlib_u8 *)src_u8_2;
			ptr1 = (mlib_u8 *)src_u8_3;
			ptr2 = (mlib_u8 *)(src_u8_0 + 1);
			ptr3 = (mlib_u8 *)(src_u8_1 + 1);
		} else if (preread == 1) {
			dst_u8_0[0] = src_u8_0[0];
			ptr0 = (mlib_u8 *)src_u8_1;
			ptr1 = (mlib_u8 *)src_u8_2;
			ptr2 = (mlib_u8 *)src_u8_3;
			ptr3 = (mlib_u8 *)(src_u8_0 + 1);
		} else if (preread == 0) {
			ptr0 = (mlib_u8 *)src_u8_0;
			ptr1 = (mlib_u8 *)src_u8_1;
			ptr2 = (mlib_u8 *)src_u8_2;
			ptr3 = (mlib_u8 *)src_u8_3;
		}

		if (((mlib_addr)(dst_u8_0 + i) & 7) && (i < (n - 3))) {
			dst_u8_0[i + 0] = (*ptr0++);
			dst_u8_0[i + 1] = (*ptr1++);
			dst_u8_0[i + 2] = (*ptr2++);
			dst_u8_0[i + 3] = (*ptr3++);
			i += 4;
		}

		for (; i < (n - 3); i += 4) {
			mlib_u32 a = (mlib_u32)((*ptr0++));
			mlib_u32 b = (mlib_u32)((*ptr1++));
			mlib_u32 c = (mlib_u32)((*ptr2++));
			mlib_u32 d = (mlib_u32)((*ptr3++));

#ifdef _LITTLE_ENDIAN
			((mlib_s32 *)(dst_u8_0 + i))[0] =
			    (((d << 8) | c) << 16) | (b << 8) | a;
#else /* _LITTLE_ENDIAN */
			((mlib_s32 *)(dst_u8_0 + i))[0] =
			    (((a << 8) | b) << 16) | (c << 8) | d;
#endif /* _LITTLE_ENDIAN */
		}

		if (preread == 3) {
			dst_u8_0[i + 0] = *ptr0;
		} else if (preread == 2) {
			dst_u8_0[i + 0] = *ptr0;
			dst_u8_0[i + 1] = *ptr1;
		} else if (preread == 1) {
			dst_u8_0[i + 0] = *ptr0;
			dst_u8_0[i + 1] = *ptr1;
			dst_u8_0[i + 2] = *ptr2;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
		src_u8_2 += src2_stride;
		src_u8_3 += src3_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
