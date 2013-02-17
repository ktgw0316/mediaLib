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

#pragma ident	"@(#)mlib_VideoColorAVDBlend_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlendABGR_Inp - inplace image blend
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlendABGR_Inp(mlib_u32       *src1dst,
 *                                        const mlib_u32 *src2,
 *                                        mlib_s32       src1dst_width,
 *                                        mlib_s32       src1dst_height,
 *                                        mlib_s32       src2_width,
 *                                        mlib_s32       src2_height,
 *                                        mlib_s32       src2_left,
 *                                        mlib_s32       src2_top,
 *                                        mlib_s32       src1dst_stride,
 *                                        mlib_s32       src2_stride,
 *                                        mlib_blend     src1dst_blend,
 *                                        mlib_blend     src2_blend)
 *
 * ARGUMENTS
 *      src1dst         pointer to output and 1st input image
 *      src2            pointer to 2nd input image
 *      src1dst_width   1st input image and output image width in pixels
 *      src1dst_height  1st input image and output image height in rows
 *      src2_width      2nd input image width in pixels
 *      src2_height     2nd input image height in rows
 *      src2_left       x offset of the top left  corner of 2nd input image
 *                      relative to the top left corner of 1st input image
 *      src2_top        y offset of the top left  corner of 2nd input image
 *                      relative to the top left corner of 1st input image
 *      src1dst_stride  linebytes for output and 1st input image
 *      src2_stride     linebytes for 2nd input image
 *      src1dst_blend   blend factor for 1st input image
 *      src2_blend      blend factor for 2nd input image
 *
 * DESCRIPTION
 *      The two multi-banded source images, src1dst and src2, are blended
 *      together and stored in the src1dst image. The image
 *      buffers pointed to by src1dst and src2 contain 4-banded ABGR
 *      images; 8-bits per component.
 *      src1dst_width and src1dst_height are the dimensions of the src1dst
 *      buffer. src2_width and src2_height are the dimensions of the src2
 *      input buffer. src2_left and src2_top are the offset of the
 *      src2 input buffer relative to src1dst. Where pixels in src2 overlap
 *      pixels in src1dst, the pixels are blended. Pixels in src1dst which are
 *      outside of src2 are left unchanged.
 *
 *      src1dst_blend specifies the blend function to be applied to the pixels
 *      of src1dst image and src2_blend specifies the blend function to be
 *      applied to the pixels of src2.
 *
 *        src1dst = (src1dst * src1dst_blend) + (src2 * src2_blend)
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
 *      MLIB_BLEND_DST_ALPHA is the alpha component of image src1dst scaled
 *      to the range 0.0 to 1.0.
 *      All pixel components are treated as unsigned 8-bit quantities and
 *      the output pixel component values are clamped to the range 0 to 255.
 */

#include <mlib_video.h>
#include <mlib_VideoColorAVDBlend_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorBlendABGR_Inp = __mlib_VideoColorBlendABGR_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBlendABGR_Inp) mlib_VideoColorBlendABGR_Inp
	__attribute__((weak, alias("__mlib_VideoColorBlendABGR_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	OP	MLIB_BLEND_ONE_MINUS_SRC_ALPHA
#define	OP1	MLIB_BLEND_ONE_MINUS_DST_ALPHA

void
__mlib_VideoColorBlendABGR_Inp(
	mlib_u32 *src1dst,
	const mlib_u32 *src2,
	mlib_s32 src1dst_width,
	mlib_s32 src1dst_height,
	mlib_s32 src2_width,
	mlib_s32 src2_height,
	mlib_s32 src2_left,
	mlib_s32 src2_top,
	mlib_s32 src1dst_stride,
	mlib_s32 src2_stride,
	mlib_blend src1dst_blend,
	mlib_blend src2_blend)
{
	mlib_s32 left_blend, top_blend, width_blend, height_blend;
	mlib_s32 left_blend2, top_blend2;
	mlib_s32 dlb = src1dst_stride >> 2;
	mlib_s32 slb2 = src2_stride >> 2;

	if (src2_top >= src1dst_height || src2_left >= src1dst_width ||
		(src2_top + src2_height <= 0) ||
		(src2_left + src2_width <= 0)) {
		return;
	}

	top_blend = src2_top > 0 ? src2_top : 0;
	height_blend =
		(src2_top + src2_height) <
		src1dst_height ? src2_top + src2_height : src1dst_height;
	height_blend -= top_blend;

	left_blend = (src2_left > 0) ? src2_left : 0;
	width_blend =
		(src2_left + src2_width <
		src1dst_width) ? src2_left + src2_width : src1dst_width;
	width_blend -= left_blend;

	top_blend2 = top_blend - src2_top;
	left_blend2 = left_blend - src2_left;

	src1dst += top_blend * dlb + left_blend;
	src2 += top_blend2 * slb2 + left_blend2;

	if (src1dst_stride == (4 * width_blend) &&
		src1dst_stride == src2_stride) {
		width_blend *= height_blend;
		height_blend = 1;
	}

	switch ((src2_blend << 4) | src1dst_blend) {

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_ZERO(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_ONE(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_SA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_OMSA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_DA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		    mlib_VideoColorBlend_ZERO_OMDA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE:
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_ONE(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_SA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_OMSA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_DA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		    mlib_VideoColorBlend_ONE_OMDA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_ZERO(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_ONE(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_SA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_OMSA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_DA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		    mlib_VideoColorBlend_SA_OMDA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_OMSA_ZERO(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_OMSA_ONE(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_OMSA_SA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | OP:
		    mlib_VideoColorBlend_OMSA_OMSA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		    mlib_VideoColorBlend_OMSA_DA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | OP:
		    mlib_VideoColorBlend_OMSA_OMDA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_ZERO(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_ONE(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_SA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_OMSA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_DA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		    mlib_VideoColorBlend_DA_OMDA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_OMDA_ZERO(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_OMDA_ONE(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_OMDA_SA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | OP1:
		    mlib_VideoColorBlend_OMDA_OMSA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		    mlib_VideoColorBlend_OMDA_DA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | OP1:
		    mlib_VideoColorBlend_OMDA_OMDA(src1dst, src1dst, src2,
			    width_blend, height_blend, src1dst_stride,
			    src1dst_stride, src2_stride);
		    break;

	    default:
		    break;
	}
}

/* *********************************************************** */
