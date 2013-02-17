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

#pragma ident	"@(#)mlib_VideoColorAVDBlend_RA.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlendABGR_ResetAlpha - image blend with alpha reset
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlendABGR_ResetAlpha
 *                                (mlib_u32       *dst,
 *                                 const mlib_u32 *src1,
 *                                 const mlib_u32 *src2,
 *                                 mlib_s32       src1_width,
 *                                 mlib_s32       src1_height,
 *                                 mlib_s32       src2_width,
 *                                 mlib_s32       src2_height,
 *                                 mlib_s32       src2_left,
 *                                 mlib_s32       src2_top,
 *                                 mlib_s32       dst_stride,
 *                                 mlib_s32       src1_stride,
 *                                 mlib_s32       src2_stride,
 *                                 mlib_blend     src1_blend,
 *                                 mlib_blend     src2_blend)
 *
 * ARGUMENTS
 *      dst          pointer to output image
 *      src1         pointer to 1st input image
 *      src2         pointer to 2nd input image
 *      src1_width   1st input image and output image width in pixels
 *      src1_height  1st input image and output image height in rows
 *      src2_width   2nd input image width in pixels
 *      src2_height  2nd input image height in rows
 *      src2_left    x offset of the top left  corner of 2nd input image
 *                   relative to the top left corner of 1st input image
 *      src2_top     y offset of the top left  corner of 2nd input image
 *                   relative to the top left corner of 1st input image
 *      dst_stride   linebytes for output image
 *      src1_stride  linebytes for 1st input image
 *      src2_stride  linebytes for 2nd input image
 *      src1_blend   blend factor for src1 image
 *      src2_blend   blend factor for src2 image
 *
 *  DESCRIPTION
 *      The two multi-banded source images, src1 and src2, are blended
 *      together and stored in the destination image, dst. The image
 *      buffers pointed to by dst, src1, and src2 contain 4-banded ABGR
 *      images; 8-bits per component.
 *      src1_width and src1_height are the dimensions of the src1 input
 *      buffer. src2_width and src2_height are the dimensions of the src2
 *      input buffer. The output buffer must be at least as large as the
 *      src1 input buffer. src2_left and src2_top are the offset of the
 *      src2 input buffer relative to src1. Where pixels in src2 overlap
 *      pixels in src1, the pixels are blended. Pixels in src1 which are
 *      outside of src2 are copied into dst. Pixels in the dst image
 *      outside of src1 are left unchanged.
 *
 *      src1_blend specifies the blend function to be applied to the pixels
 *      of src1 image and src2_blend specifies the blend function to be
 *      applied to the pixels of src2.
 *
 *        dst = (src1 * src1_blend) + (src2 * src2_blend)
 *
 *      The alpha value of every pixel in dst is set to 0 after blend is
 *      complete.
 *
 *      Possible blend factors are:
 *        MLIB_BLEND_ZERO
 *        MLIB_BLEND_ONE
 *        MLIB_BLEND_SRC_ALPHA
 *        MLIB_BLEND_ONE_MINUS_SRC_ALPHA
 *        MLIB_BLEND_DST_ALPHA
 *        MLIB_BLEND_ONE_MINUS_DST_ALPHA
 *
 *      MLIB_BLEND_SRC_ALPHA is the alpha component of image src2 scaled
 *      to the range 0.0 to 1.0.
 *      MLIB_BLEND_DST_ALPHA is the alpha component of image src1 scaled
 *      to the range 0.0 to 1.0.
 *      All pixel components are treated as unsigned 8-bit quantities and
 *      the output pixel component values are clamped to the range 0 to 255.
 */

#include <mlib_video.h>
#include <mlib_VideoColorAVDBlend_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorBlendABGR_ResetAlpha = \
	__mlib_VideoColorBlendABGR_ResetAlpha

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBlendABGR_ResetAlpha)
	mlib_VideoColorBlendABGR_ResetAlpha
	__attribute__((weak, alias("__mlib_VideoColorBlendABGR_ResetAlpha")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	OP	MLIB_BLEND_ONE_MINUS_SRC_ALPHA
#define	OP1	MLIB_BLEND_ONE_MINUS_DST_ALPHA

void
__mlib_VideoColorBlendABGR_ResetAlpha(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 src1_width,
	mlib_s32 src1_height,
	mlib_s32 src2_width,
	mlib_s32 src2_height,
	mlib_s32 src2_left,
	mlib_s32 src2_top,
	mlib_s32 dst_stride,
	mlib_s32 src1_stride,
	mlib_s32 src2_stride,
	mlib_blend src1_blend,
	mlib_blend src2_blend)
{
	mlib_s32 left_copy, top_copy, width_copy, height_copy;
	mlib_s32 left_blend, top_blend, width_blend, height_blend;
	mlib_s32 left_blend2, top_blend2;
	mlib_s32 dst_width = src1_width;
	mlib_s32 dst_height = src1_height;
	mlib_s32 dlb = dst_stride >> 2;
	mlib_s32 slb1 = src1_stride >> 2;
	mlib_s32 slb2 = src2_stride >> 2;

	if (src2_top >= dst_height || src2_left >= dst_width ||
		(src2_top + src2_height <= 0) ||
		(src2_left + src2_width <= 0)) {
		mlib_VideoColorCopy_RA(dst, src1, dst_width, dst_height,
			dst_stride, src1_stride);
		return;
	}

	if (src2_top > 0)
		mlib_VideoColorCopy_RA(dst, src1, dst_width, src2_top,
			dst_stride, src1_stride);

	if (src2_top + src2_height < dst_height)
		mlib_VideoColorCopy_RA(dst + (src2_top + src2_height) * dlb,
			src1 + (src2_top + src2_height) * slb1, dst_width,
			dst_height - (src2_top + src2_height), dst_stride,
			src1_stride);

	top_copy = src2_top > 0 ? src2_top : 0;
	height_copy =
		(src2_top + src2_height) <
		dst_height ? src2_top + src2_height : dst_height;
	height_copy -= top_copy;

	left_copy = dst_width;
	left_blend = 0;

	if (src2_left > 0 && src2_left + src2_width < dst_width) {
		left_blend = width_copy = src2_left;
		left_copy = src2_left + src2_width;
		mlib_VideoColorCopy2_RA(dst + top_copy * dlb,
			src1 + top_copy * slb1, dst_width, height_copy,
			width_copy, left_copy, dst_stride, src1_stride);
	} else {
		if (src2_left > 0) {
			left_blend = width_copy = src2_left;
			mlib_VideoColorCopy_RA(dst + top_copy * dlb,
				src1 + top_copy * slb1, width_copy, height_copy,
				dst_stride, src1_stride);
		}

		if (src2_left + src2_width < dst_width) {
			left_copy = src2_left + src2_width;
			width_copy = dst_width - left_copy;
			mlib_VideoColorCopy_RA(dst + top_copy * dlb + left_copy,
				src1 + top_copy * slb1 + left_copy, width_copy,
				height_copy, dst_stride, src1_stride);
		}
	}

	top_blend = top_copy;
	height_blend = height_copy;
	width_blend = left_copy - left_blend;

	top_blend2 = top_blend - src2_top;
	left_blend2 = left_blend - src2_left;

	dst += top_blend * dlb + left_blend;
	src1 += top_blend * slb1 + left_blend;
	src2 += top_blend2 * slb2 + left_blend2;

	if (dst_stride == (4 * width_blend) && src1_stride == src2_stride &&
		src1_stride == dst_stride) {
		width_blend *= height_blend;
		height_blend = 1;
	}

	switch ((src2_blend << 4) | src1_blend) {

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_ZERO_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_ONE_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_SA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_OMSA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_DA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_OMDA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ZERO_ONE_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_ONE_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_SA_RA(dst, src1, src2, width_blend,
			    height_blend, dst_stride, src1_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_OMSA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_DA_RA(dst, src1, src2, width_blend,
			    height_blend, dst_stride, src1_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_OMDA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_ZERO_DA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_ONE_DA_RA(dst, src2, src1, width_blend,
			    height_blend, dst_stride, src2_stride, src1_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_SA_RA(dst, src1, src2, width_blend,
			    height_blend, dst_stride, src1_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_OMSA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_DA_RA(dst, src1, src2, width_blend,
			    height_blend, dst_stride, src1_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_OMDA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_ZERO_OMDA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_ONE_OMDA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_OMSA_SA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | OP:
		    mlib_VideoColorBlend_OMSA_OMSA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_OMDA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | OP:
		    mlib_VideoColorBlend_OMSA_OMDA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_ZERO_SA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_ONE_SA_RA(dst, src2, src1, width_blend,
			    height_blend, dst_stride, src2_stride, src1_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_SA_RA(dst, src1, src2, width_blend,
			    height_blend, dst_stride, src1_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_OMSA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_SA_SA_RA(dst, src2, src1, width_blend,
			    height_blend, dst_stride, src2_stride, src1_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_OMSA_SA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_ZERO_OMSA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_ONE_OMSA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_DA_OMSA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | OP1:
		    mlib_VideoColorBlend_OMDA_OMSA_RA(dst, src1, src2,
			    width_blend, height_blend, dst_stride, src1_stride,
			    src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_SA_OMSA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | OP1:
		    mlib_VideoColorBlend_OMSA_OMSA_RA(dst, src2, src1,
			    width_blend, height_blend, dst_stride, src2_stride,
			    src1_stride);
		    break;

	    default:
		    break;
	}
}

/* *********************************************************** */
