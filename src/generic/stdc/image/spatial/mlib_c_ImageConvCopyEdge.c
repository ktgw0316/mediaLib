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

#pragma ident	"@(#)mlib_c_ImageConvCopyEdge.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageConvCopyEdge  - Copy src edges to dst edges
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConvCopyEdge(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         mlib_s32         dx_l,
 *                                         mlib_s32         dx_r,
 *                                         mlib_s32         dy_t,
 *                                         mlib_s32         dy_b,
 *                                         mlib_s32         cmask)
 *
 * ARGUMENT
 *      dst       Pointer to an dst image.
 *      src       Pointer to an src image.
 *      dx_l      Number of columns on the left side of the
 *                image to be copyed.
 *      dx_r      Number of columns on the right side of the
 *                image to be copyed.
 *      dy_t      Number of rows on the top edge of the
 *                image to be copyed.
 *      dy_b      Number of rows on the top edge of the
 *                image to be copyed.
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *
 * RESTRICTION
 *      The src and the dst must be the same type, same width, same height
 *      and have same number of channels (1, 2, 3, or 4). The unselected
 *      channels are not overwritten. If both src and dst have just one
 *      channel, cmask is ignored.
 *
 * DESCRIPTION
 *      Copy src edges to dst edges.
 *
 *      The unselected channels are not overwritten.
 *      If src and dst have just one channel,
 *      cmask is ignored.
 */

#include <mlib_image.h>
#include <mlib_ImageConvEdge.h>

/* *********************************************************** */

#define	EDGES(chan, type, mask)                                            \
	{                                                                  \
	    type *pdst = (type *) mlib_ImageGetData(dst);                  \
	    type *psrc = (type *) mlib_ImageGetData(src);                  \
	    mlib_s32 dst_stride =                                          \
		mlib_ImageGetStride(dst) / sizeof (type);                  \
	    mlib_s32 src_stride =                                          \
		mlib_ImageGetStride(src) / sizeof (type);                  \
	    mlib_s32 i, j, l;                                              \
	    mlib_s32 testchan;                                             \
	                                                                   \
	    testchan = 1;                                                  \
	    for (l = chan - 1; l >= 0; l--) {                              \
		if ((mask & testchan) == 0) {                              \
		    testchan <<= 1;                                        \
		    continue;                                              \
		}                                                          \
		testchan <<= 1;                                            \
		for (j = 0; j < dx_l; j++) {                               \
		    for (i = dy_t; i < (img_height - dy_b); i++) {         \
			pdst[i * dst_stride + l + j * chan] =              \
			    psrc[i * src_stride + l + j * chan];           \
		    }                                                      \
		}                                                          \
		for (j = 0; j < dx_r; j++) {                               \
		    for (i = dy_t; i < (img_height - dy_b); i++) {         \
			pdst[i * dst_stride + l + (img_width - 1 -         \
			    j) * chan] =                                   \
			    psrc[i * src_stride + l + (img_width - 1 -     \
			    j) * chan];                                    \
		    }                                                      \
		}                                                          \
		for (i = 0; i < dy_t; i++) {                               \
		    for (j = 0; j < img_width; j++) {                      \
			pdst[i * dst_stride + l + j * chan] =              \
			    psrc[i * src_stride + l + j * chan];           \
		    }                                                      \
		}                                                          \
		for (i = 0; i < dy_b; i++) {                               \
		    for (j = 0; j < img_width; j++) {                      \
			pdst[(img_height - 1 - i) * dst_stride + l +       \
			    j * chan] =                                    \
			    psrc[(img_height - 1 - i) * src_stride + l +   \
			    j * chan];                                     \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

mlib_status
mlib_ImageConvCopyEdge(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 cmask)
{
	mlib_s32 img_width = mlib_ImageGetWidth(dst);
	mlib_s32 img_height = mlib_ImageGetHeight(dst);
	mlib_s32 channel = mlib_ImageGetChannels(dst);

	if (dx_l + dx_r > img_width) {
		dx_l = img_width;
		dx_r = 0;
	}

	if (dy_t + dy_b > img_height) {
		dy_t = img_height;
		dy_b = 0;
	}

	if (channel == 1)
		cmask = 1;

	switch (mlib_ImageGetType(src)) {
	case MLIB_BIT:
		return mlib_ImageConvCopyEdge_Bit(dst, src, dx_l, dx_r, dy_t,
		    dy_b, cmask);
	case MLIB_BYTE:
		EDGES(channel, mlib_u8,
		    cmask)
		    break;

	case MLIB_SHORT:
	case MLIB_USHORT:
		EDGES(channel, mlib_u16,
		    cmask)
		    break;

	case MLIB_INT:
	case MLIB_FLOAT:
		EDGES(channel, mlib_u32,
		    cmask)
		    break;

	case MLIB_DOUBLE:
		EDGES(channel, mlib_d64,
		    cmask)
		    break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
