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

#ifndef _MLIB_IMAGEBLENDUTIL_H
#define	_MLIB_IMAGEBLENDUTIL_H

#pragma ident	"@(#)mlib_ImageBlendUtil.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>

#define	BLEND_VALIDATE                                                   \
	mlib_u8 *psrc1, *psrc2, *pdst;                                   \
	mlib_s32 dst_height, dst_width;                                  \
	mlib_s32 src1_stride, src2_stride, dst_stride;                   \
	mlib_s32 channels, i, j;                                         \
	                                                                 \
	MLIB_IMAGE_CHECK(src1);                                          \
	MLIB_IMAGE_CHECK(src2);                                          \
	MLIB_IMAGE_CHECK(dst);                                           \
	MLIB_IMAGE_FULL_EQUAL(src1, dst);                                \
	MLIB_IMAGE_FULL_EQUAL(src2, dst);                                \
	MLIB_IMAGE_HAVE_TYPE(dst, MLIB_BYTE);                            \
	MLIB_IMAGE_HAVE_3_OR_4_CHAN(dst);                                \
	channels = mlib_ImageGetChannels(dst);                           \
	if (channels == 4 && cmask != 1 && cmask != 8)                   \
	    return (MLIB_OUTOFRANGE);                                    \
	pdst = (mlib_u8 *)mlib_ImageGetData(dst);                        \
	psrc1 = (mlib_u8 *)mlib_ImageGetData(src1);                      \
	psrc2 = (mlib_u8 *)mlib_ImageGetData(src2);                      \
	dst_height = mlib_ImageGetHeight(dst);                           \
	dst_width = mlib_ImageGetWidth(dst);                             \
	src1_stride = mlib_ImageGetStride(src1);                         \
	src2_stride = mlib_ImageGetStride(src2);                         \
	dst_stride = mlib_ImageGetStride(dst);                           \
	if (dst_stride == (channels * dst_width) &&                      \
	    src1_stride == src2_stride && src1_stride == dst_stride) {   \
	    dst_width *= dst_height;                                     \
	    dst_height = 1;                                              \
	}
#define	BLEND_VALIDATE_1                                                 \
	mlib_u8 *psrc2, *pdst;                                           \
	mlib_s32 dst_height, dst_width;                                  \
	mlib_s32 src1_stride, src2_stride, dst_stride;                   \
	mlib_s32 channels, i, j;                                         \
	                                                                 \
	MLIB_IMAGE_CHECK(src1);                                          \
	MLIB_IMAGE_CHECK(src2);                                          \
	MLIB_IMAGE_CHECK(dst);                                           \
	MLIB_IMAGE_FULL_EQUAL(src1, dst);                                \
	MLIB_IMAGE_FULL_EQUAL(src2, dst);                                \
	MLIB_IMAGE_HAVE_TYPE(dst, MLIB_BYTE);                            \
	MLIB_IMAGE_HAVE_3_OR_4_CHAN(dst);                                \
	channels = mlib_ImageGetChannels(dst);                           \
	if (channels == 4 && cmask != 1 && cmask != 8)                   \
	    return (MLIB_OUTOFRANGE);                                    \
	pdst = (mlib_u8 *)mlib_ImageGetData(dst);                        \
	psrc2 = (mlib_u8 *)mlib_ImageGetData(src2);                      \
	dst_height = mlib_ImageGetHeight(dst);                           \
	dst_width = mlib_ImageGetWidth(dst);                             \
	src1_stride = mlib_ImageGetStride(src1);                         \
	src2_stride = mlib_ImageGetStride(src2);                         \
	dst_stride = mlib_ImageGetStride(dst);                           \
	if (dst_stride == (channels * dst_width) &&                      \
	    src1_stride == src2_stride && src1_stride == dst_stride) {   \
	    dst_width *= dst_height;                                     \
	    dst_height = 1;                                              \
	}
#define	MAX_SIZE	1024
#define	BUFFER_SIZE                                                      \
	if (dst_width > MAX_SIZE && dst_height == 1) {                   \
	    while ((dst_width & 1) == 0 && dst_width > MAX_SIZE) {       \
		dst_width >>= 1;                                         \
		dst_height *= 2;                                         \
	    }                                                            \
	    for (i = 3; dst_width > MAX_SIZE; i += 2) {                  \
		while ((dst_width % i) == 0 && dst_width > MAX_SIZE) {   \
		    dst_width /= i;                                      \
		    dst_height *= i;                                     \
		}                                                        \
	    }                                                            \
	    src1_stride = src2_stride = dst_stride = dst_width * 4;      \
	}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEBLENDUTIL_H */
