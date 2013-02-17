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

#pragma ident	"@(#)mlib_c_ImageZoomOut2XIndex.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2XIndex - indexed color image 0.5X scaling
 *                                   with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2XIndex(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           mlib_filter      filter,
 *                                           mlib_edge        edge,
 *                                           const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      The resampling filter can be one of the following:
 *              MLIB_NEAREST
 *              MLIB_BILINEAR
 *              MLIB_BICUBIC
 *              MLIB_BICUBIC2
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE        (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_OP_NEAREST
 *              MLIB_EDGE_SRC_EXTEND
 *              MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and have just one channel.
 *      They can be in MLIB_BYTE or MLIB_SHORT data type.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageZoomOut2X.h>
#include <mlib_ImageZoom2X.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomOut2XIndex = __mlib_ImageZoomOut2XIndex

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomOut2XIndex) mlib_ImageZoomOut2XIndex
    __attribute__((weak, alias("__mlib_ImageZoomOut2XIndex")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_c_ImageZoomOut2XIndex_U8_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_S16_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_c_ImageZoomOut2XIndex_U8_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));

/* *********************************************************** */

#define	HFILE	0

/* *********************************************************** */

#define	ch0	0
#define	ch1	1

#define	ch2	2

#define	ch3	3

/* *********************************************************** */

mlib_status
__mlib_ImageZoomOut2XIndex(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge,
    const void *colormap)
{
	mlib_status res = MLIB_SUCCESS;

	if (filter == MLIB_NEAREST)
		return (__mlib_ImageZoomOut2X(dst, src, filter, edge));

/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_HAVE_CHAN(src, 1);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	switch (filter) {

	case MLIB_BILINEAR:
		switch (mlib_ImageGetType(src)) {

		case MLIB_BYTE: {
			    BOUND_INDEX_BILINEAR(mlib_u8);

			    SWITCH_INDEX_BIxxx_CHANNELS(_c_, U8, Bilinear);
		    }

		case MLIB_SHORT: {
			    BOUND_INDEX_BILINEAR(mlib_s16);

			    SWITCH_INDEX_BIxxx_CHANNELS(_c_, S16, Bilinear);
		    }

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC:
		switch (mlib_ImageGetType(src)) {

		case MLIB_BYTE: {
			    BOUND_INDEX_BICUBIC(mlib_u8);

			    SWITCH_INDEX_BIxxx_CHANNELS(_c_, U8, Bicubic);
		    }

		case MLIB_SHORT: {
			    BOUND_INDEX_BICUBIC(mlib_s16);

			    SWITCH_INDEX_BIxxx_CHANNELS(_c_, S16, Bicubic);
		    }

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC2:
		switch (mlib_ImageGetType(src)) {

		case MLIB_BYTE: {
			    BOUND_INDEX_BICUBIC(mlib_u8);

			    SWITCH_INDEX_BIxxx_CHANNELS(_c_, U8, Bicubic2);
		    }

		case MLIB_SHORT: {
			    BOUND_INDEX_BICUBIC(mlib_s16);

			    SWITCH_INDEX_BIxxx_CHANNELS(_c_, S16, Bicubic2);
		    }

		default:
			return (MLIB_FAILURE);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BL_U8;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				mlib_s32 color0 =
				    lut[p_src[2 * i * src_stride + 2 * j]],
				    color1 =
				    lut[p_src[(2 * i + 1) * src_stride +
				    2 * j]], color2 =
				    lut[p_src[2 * i * src_stride + 2 * j + 1]],
				    color3 =
				    lut[p_src[(2 * i + 1) * src_stride + 2 * j +
				    1]];
				color0 =
				    AVE_U8_4(color0, color1, color2, color3);
				STORE_U8_4(color0);
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[2 * i * src_stride +
					    2 * j + 1]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[(2 * i + 1) * src_stride +
					    2 * j]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BL_U8;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				mlib_s32 color0 =
				    lut[p_src[2 * i * src_stride + 2 * j]],
				    color1 =
				    lut[p_src[(2 * i + 1) * src_stride +
				    2 * j]], color2 =
				    lut[p_src[2 * i * src_stride + 2 * j + 1]],
				    color3 =
				    lut[p_src[(2 * i + 1) * src_stride + 2 * j +
				    1]];
				color0 =
				    AVE_U8_4(color0, color1, color2, color3);
				STORE_U8_4(color0);
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_4(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[2 * i * src_stride +
					    2 * j + 1]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4(dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[(2 * i + 1) * src_stride +
					    2 * j]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4(dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BL_S16;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			for (j = 0; j < dst_width; j++) {
				mlib_s16 *pcolor0 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j]),
				    *pcolor1 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j]), *pcolor2 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j + 1]),
				    *pcolor3 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j + 1]);
				dst_color[3 * j] =
				    (pcolor0[ch1] + pcolor1[ch1] +
				    pcolor2[ch1] + pcolor3[ch1]) >> 2;
				dst_color[3 * j + 1] =
				    (pcolor0[ch2] + pcolor1[ch2] +
				    pcolor2[ch2] + pcolor3[ch2]) >> 2;
				dst_color[3 * j + 2] =
				    (pcolor0[ch3] + pcolor1[ch3] +
				    pcolor2[ch3] + pcolor3[ch3]) >> 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_3(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j +
					    1]);
					dst_color[3 * j] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[3 * j + 1] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[3 * j + 2] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut + p_src[(2 * i +
					    1) * src_stride + 2 * j]);
					dst_color[3 * j] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[3 * j + 1] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[3 * j + 2] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BL_S16;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			for (j = 0; j < dst_width; j++) {
				mlib_s16 *pcolor0 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j]),
				    *pcolor1 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j]), *pcolor2 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j + 1]),
				    *pcolor3 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j + 1]);
				dst_color[4 * j] =
				    (pcolor0[ch0] + pcolor1[ch0] +
				    pcolor2[ch0] + pcolor3[ch0]) >> 2;
				dst_color[4 * j + 1] =
				    (pcolor0[ch1] + pcolor1[ch1] +
				    pcolor2[ch1] + pcolor3[ch1]) >> 2;
				dst_color[4 * j + 2] =
				    (pcolor0[ch2] + pcolor1[ch2] +
				    pcolor2[ch2] + pcolor3[ch2]) >> 2;
				dst_color[4 * j + 3] =
				    (pcolor0[ch3] + pcolor1[ch3] +
				    pcolor2[ch3] + pcolor3[ch3]) >> 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_4(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j +
					    1]);
					dst_color[4 * j] =
					    (pcolor0[ch0] + pcolor1[ch0]) >> 1;
					dst_color[4 * j + 1] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[4 * j + 2] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[4 * j + 3] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut + p_src[(2 * i +
					    1) * src_stride + 2 * j]);
					dst_color[4 * j] =
					    (pcolor0[ch0] + pcolor1[ch0]) >> 1;
					dst_color[4 * j + 1] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[4 * j + 2] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[4 * j + 3] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BL_U8;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				mlib_s32 color0 =
				    lut[p_src[2 * i * src_stride + 2 * j]],
				    color1 =
				    lut[p_src[(2 * i + 1) * src_stride +
				    2 * j]], color2 =
				    lut[p_src[2 * i * src_stride + 2 * j + 1]],
				    color3 =
				    lut[p_src[(2 * i + 1) * src_stride + 2 * j +
				    1]];
				color0 =
				    AVE_U8_4(color0, color1, color2, color3);
				STORE_U8_4(color0);
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[2 * i * src_stride +
					    2 * j + 1]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[(2 * i + 1) * src_stride +
					    2 * j]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BL_U8;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			for (j = 0; j < dst_width; j++) {
				mlib_s32 color0 =
				    lut[p_src[2 * i * src_stride + 2 * j]],
				    color1 =
				    lut[p_src[(2 * i + 1) * src_stride +
				    2 * j]], color2 =
				    lut[p_src[2 * i * src_stride + 2 * j + 1]],
				    color3 =
				    lut[p_src[(2 * i + 1) * src_stride + 2 * j +
				    1]];
				color0 =
				    AVE_U8_4(color0, color1, color2, color3);
				STORE_U8_4(color0);
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_4(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[2 * i * src_stride +
					    2 * j + 1]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				for (j = 0; j < dst_width; j++) {
					mlib_s32 color0 =
					    lut[p_src[2 * i * src_stride +
					    2 * j]], color1 =
					    lut[p_src[(2 * i + 1) * src_stride +
					    2 * j]];
					color0 = AVE_U8_2(color0, color1);
					STORE_U8_4(color0);
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BL_S16;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				mlib_s16 *pcolor0 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j]),
				    *pcolor1 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j]), *pcolor2 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j + 1]),
				    *pcolor3 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j + 1]);
				dst_color[3 * j] =
				    (pcolor0[ch1] + pcolor1[ch1] +
				    pcolor2[ch1] + pcolor3[ch1]) >> 2;
				dst_color[3 * j + 1] =
				    (pcolor0[ch2] + pcolor1[ch2] +
				    pcolor2[ch2] + pcolor3[ch2]) >> 2;
				dst_color[3 * j + 2] =
				    (pcolor0[ch3] + pcolor1[ch3] +
				    pcolor2[ch3] + pcolor3[ch3]) >> 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_3(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j +
					    1]);
					dst_color[3 * j] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[3 * j + 1] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[3 * j + 2] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut + p_src[(2 * i +
					    1) * src_stride + 2 * j]);
					dst_color[3 * j] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[3 * j + 1] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[3 * j + 2] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BL_S16;

/* averaging along both axes */

	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < dst_width; j++) {
				mlib_s16 *pcolor0 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j]),
				    *pcolor1 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j]), *pcolor2 =
				    (mlib_s16 *)(lut +
				    p_src[2 * i * src_stride + 2 * j + 1]),
				    *pcolor3 =
				    (mlib_s16 *)(lut + p_src[(2 * i +
				    1) * src_stride + 2 * j + 1]);
				dst_color[4 * j] =
				    (pcolor0[ch0] + pcolor1[ch0] +
				    pcolor2[ch0] + pcolor3[ch0]) >> 2;
				dst_color[4 * j + 1] =
				    (pcolor0[ch1] + pcolor1[ch1] +
				    pcolor2[ch1] + pcolor3[ch1]) >> 2;
				dst_color[4 * j + 2] =
				    (pcolor0[ch2] + pcolor1[ch2] +
				    pcolor2[ch2] + pcolor3[ch2]) >> 2;
				dst_color[4 * j + 3] =
				    (pcolor0[ch3] + pcolor1[ch3] +
				    pcolor2[ch3] + pcolor3[ch3]) >> 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_4(dst_color,
			    p_dst + i * dst_stride, dst_width, colormap);
		}
	} else {
/* averaging along X axis */

		if (dh)
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j +
					    1]);
					dst_color[4 * j] =
					    (pcolor0[ch0] + pcolor1[ch0]) >> 1;
					dst_color[4 * j + 1] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[4 * j + 2] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[4 * j + 3] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
		} else
/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dst_width; j++) {
					mlib_s16 *pcolor0 =
					    (mlib_s16 *)(lut +
					    p_src[2 * i * src_stride + 2 * j]),
					    *pcolor1 =
					    (mlib_s16 *)(lut + p_src[(2 * i +
					    1) * src_stride + 2 * j]);
					dst_color[4 * j] =
					    (pcolor0[ch0] + pcolor1[ch0]) >> 1;
					dst_color[4 * j + 1] =
					    (pcolor0[ch1] + pcolor1[ch1]) >> 1;
					dst_color[4 * j + 2] =
					    (pcolor0[ch2] + pcolor1[ch2]) >> 1;
					dst_color[4 * j + 3] =
					    (pcolor0[ch3] + pcolor1[ch3]) >> 1;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    (dst_color, p_dst + i * dst_stride,
				    dst_width, colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	EFILE	0

/* *********************************************************** */

#define	FOR_PREP_XY(Type, Typec, CH, Bccf)                                \
	for (i = 0; i < dst_height; i++) {                                \
	    for (l = 0; l < CH; l++) {                                    \
		Type *ps = p_src + 2 * i * src_stride;                    \
		Typec *src_color = lut[l] - offset;                       \
		mlib_s32 st0 =                                            \
		    Bccf * (GET_CHAN(ps[-1]) + GET_CHAN(ps[src_stride -   \
		    1])) - GET_CHAN(ps[-src_stride - 1]) -                \
		    GET_CHAN(ps[2 * src_stride - 1]), st1 =               \
		    Bccf * (GET_CHAN(ps[0]) +                             \
		    GET_CHAN(ps[src_stride])) -                           \
		    GET_CHAN(ps[-src_stride]) -                           \
		    GET_CHAN(ps[2 * src_stride])

/* *********************************************************** */

#define	FOR_XY(TP, TPO, CH, Bccf, Normal)                        \
		for (j = 0; j < dst_width; j++) {                \
		    mlib_s32 res = Bccf * st1 - st0;             \
	                                                         \
		    st0 =                                        \
			Bccf * (GET_CHAN(ps[1]) +                \
			GET_CHAN(ps[src_stride + 1])) -          \
			GET_CHAN(ps[-src_stride + 1]) -          \
			GET_CHAN(ps[2 * src_stride + 1]);        \
		    st1 =                                        \
			Bccf * (GET_CHAN(ps[2]) +                \
			GET_CHAN(ps[src_stride + 2])) -          \
			GET_CHAN(ps[-src_stride + 2]) -          \
			GET_CHAN(ps[2 * src_stride + 2]);        \
		    res = (res + Bccf * st0 - st1) Normal;       \
		    dst_color[CH * j + l] = SAT_##TPO(res);      \
		    ps += 2;                                     \
		}                                                \
	    }                                                    \
	    mlib_ImageColorTrue2IndexLine_##TPO##_##TP##_##CH    \
		(dst_color, p_dst + i * dst_stride, dst_width,   \
		colormap);                                       \
	}

/* *********************************************************** */

#define	FOR_PREP_X(Type, Typec, CH)                                     \
	for (i = 0; i < dst_height; i++) {                              \
	    for (l = 0; l < CH; l++) {                                  \
		Type *ps = p_src + 2 * i * src_stride;                  \
		Typec *src_color = lut[l] - offset;                     \
		mlib_s32 a1 = GET_CHAN(ps[0]), a0 = GET_CHAN(ps[-1])

/* *********************************************************** */

#define	FOR_X(TP, TPO, CH, Bccf, Normal)                         \
		for (j = 0; j < dst_width; j++) {                \
		    mlib_s32 res = Bccf * a1 - a0;               \
	                                                         \
		    a0 = GET_CHAN(ps[1]);                        \
		    a1 = GET_CHAN(ps[2]);                        \
		    res = (res + Bccf * a0 - a1) Normal;         \
		    dst_color[CH * j + l] = SAT_##TPO(res);      \
		    ps += 2;                                     \
		}                                                \
	    }                                                    \
	    mlib_ImageColorTrue2IndexLine_##TPO##_##TP##_##CH    \
		(dst_color, p_dst + i * dst_stride, dst_width,   \
		colormap);                                       \
	}

/* *********************************************************** */

#define	FOR_PREP_Y(Type, Typec, CH)                             \
	for (i = 0; i < dst_height; i++) {                      \
	    for (l = 0; l < CH; l++) {                          \
		Type *ps = p_src + 2 * i * src_stride;          \
		Typec *src_color = lut[l] - offset

/* *********************************************************** */

#define	FOR_Y(TP, TPO, CH, Bccf, Normal)                         \
		for (j = 0; j < dst_width; j++) {                \
		    mlib_s32 res =                               \
			(Bccf * (GET_CHAN(ps[0]) +               \
			GET_CHAN(ps[src_stride])) -              \
			GET_CHAN(ps[-src_stride]) -              \
			GET_CHAN(ps[2 * src_stride])) Normal;    \
		    dst_color[CH * j + l] = SAT_##TPO(res);      \
		    ps += 2;                                     \
		}                                                \
	    }                                                    \
	    mlib_ImageColorTrue2IndexLine_##TPO##_##TP##_##CH    \
		(dst_color, p_dst + i * dst_stride, dst_width,   \
		colormap);                                       \
	}

/* *********************************************************** */

#define	BC_CF	9
#define	BC_CD	8

#define	BC_NRML	>>4

#define	BC_NRM2	>>8

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_u8,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, U8, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, U8, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, U8, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_u8,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, U8, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, U8, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, U8, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_u8,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, U8, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, U8, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, U8, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_u8,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, U8, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, U8, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, U8, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_s16,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, S16, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, S16, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, S16, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_s16,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, S16, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, S16, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, S16, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_s16,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, S16, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, S16, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, S16, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_s16,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, S16, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, S16, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, S16, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef BC_CF
#undef BC_CD
#undef BC_NRML
#undef BC_NRM2

/* *********************************************************** */

#define	BC_CF	5
#define	BC_CD	4

#define	BC_NRML	>>3

#define	BC_NRM2	>>6

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_u8,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, U8, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, U8, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, U8, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_u8,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, U8, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, U8, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, U8, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_u8,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, U8, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, U8, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_u8,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, U8, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_U8;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_u8,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, U8, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, U8, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_u8,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, U8, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_s16,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, S16, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, S16, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, S16, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_S16_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_s16,
		    mlib_s16,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(S16, S16, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_s16,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(S16, S16, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_s16,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(S16, S16, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_s16,
		    3,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, S16, 3, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, S16, 3, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_s16,
			    3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, S16, 3, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomOut2XIndex_U8_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	ALLOC_MEM_BC_S16;

	if (dh == dw) {
		FOR_PREP_XY(mlib_u8,
		    mlib_s16,
		    4,
		    BC_CF);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		FOR_XY(U8, S16, 4, BC_CF, BC_NRM2)
	} else {

		if (dh)
			FOR_PREP_X(mlib_u8,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_X(U8, S16, 4, BC_CF, BC_NRML)
		else
			FOR_PREP_Y(mlib_u8,
			    mlib_s16,
			    4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			FOR_Y(U8, S16, 4, BC_CF, BC_NRML)
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
