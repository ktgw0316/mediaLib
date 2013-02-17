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

#pragma ident	"@(#)mlib_v_ImageZoomOut2XIndex.c	9.2	07/11/05 SMI"

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
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageZoomOut2X.h>
#include <mlib_ImageColormap.h>

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

#define	ch0	0
#define	ch1	1
#define	ch2	2
#define	ch3	3

/* *********************************************************** */

static mlib_status mlib_v_ImageZoomOut2XIndex_U8_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_S16_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));
static mlib_status mlib_v_ImageZoomOut2XIndex_U8_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8));

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

			    SWITCH_INDEX_BIxxx_CHANNELS(_v_, U8, Bilinear);
		    }

		case MLIB_SHORT: {
			    BOUND_INDEX_BILINEAR(mlib_s16);

			    SWITCH_INDEX_BIxxx_CHANNELS(_v_, S16, Bilinear);
		    }

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC:
		switch (mlib_ImageGetType(src)) {

		case MLIB_BYTE: {
			    BOUND_INDEX_BICUBIC(mlib_u8);

			    SWITCH_INDEX_BIxxx_CHANNELS(_v_, U8, Bicubic);
		    }

		case MLIB_SHORT: {
			    BOUND_INDEX_BICUBIC(mlib_s16);

			    SWITCH_INDEX_BIxxx_CHANNELS(_v_, S16, Bicubic);
		    }

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC2:
		switch (mlib_ImageGetType(src)) {

		case MLIB_BYTE: {
			    BOUND_INDEX_BICUBIC(mlib_u8);

			    SWITCH_INDEX_BIxxx_CHANNELS(_v_, U8, Bicubic2);
		    }

		case MLIB_SHORT: {
			    BOUND_INDEX_BICUBIC(mlib_s16);

			    SWITCH_INDEX_BIxxx_CHANNELS(_v_, S16, Bicubic2);
		    }

		default:
			return (MLIB_FAILURE);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

/*			the Bilinear Part			*/

/* *********************************************************** */

#define	ALLOC_MEM_U8                                                      \
	mlib_s32 i, j;                                                    \
	mlib_d64 dx1, dx2, dx3, dx4, dr;                                  \
	mlib_f32 fr, fx, fx1, fx2, fx3,                                   \
		fzero = vis_to_float(0),                                  \
		fone = vis_to_float(0x100);                               \
	mlib_f32 *lut, *dst_color;                                        \
	                                                                  \
	if (dst_width <= 0 || dst_height <= 0)                            \
	    return (MLIB_SUCCESS);                                        \
	i = (dst_width < dst_height) ? dst_height : dst_width;            \
	dst_color = (mlib_f32 *)__mlib_malloc(4 * i);                     \
	if (dst_color == NULL)                                            \
	    return (MLIB_FAILURE);                                        \
	lut =                                                             \
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -           \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	ALLOC_MEM_S16                                                    \
	mlib_s32 i, j;                                                   \
	mlib_d64 *lut, *dst_color;                                       \
	mlib_d64 dx1, dx2, dx3, dx4, dr;                                 \
	mlib_d64 dmask_05 = vis_to_double_dup(0xfffefffe),               \
		dmask_025 = vis_to_double_dup(0xfffcfffc);               \
	mlib_f32 scale025 = vis_to_float(0x40404040),                    \
		scale05 = vis_to_float(0x80808080);                      \
	if (dst_width <= 0 || dst_height <= 0)                           \
	    return (MLIB_SUCCESS);                                       \
	i = (dst_width < dst_height) ? dst_height : dst_width;           \
	dst_color = (mlib_d64 *)__mlib_malloc(2 * 4 * i);                \
	if (dst_color == NULL)                                           \
	    return (MLIB_FAILURE);                                       \
	lut =                                                            \
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -          \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	FREE_MEM	__mlib_free(dst_color)

/* *********************************************************** */

#define	BILINEAR_U8_4                                           \
	dr = vis_fmul8x16al(fx, fone);                          \
	dx1 = vis_fpmerge(fzero, fx1);                          \
	dr = vis_fpadd16(dx1, dr);                              \
	fr = vis_fpack16(dr)

/* *********************************************************** */

#define	BILINEAR_U8_4_XY                                        \
	dx3 = vis_fmul8x16al(fx, fone);                         \
	dx1 = vis_fmul8x16al(fx1, fone);                        \
	dx4 = vis_fmul8x16al(fx2, fone);                        \
	dx2 = vis_fpmerge(fzero, fx3);                          \
	dx1 = vis_fpadd16(dx1, dx3);                            \
	dx2 = vis_fpadd16(dx2, dx4);                            \
	dr = vis_fpadd16(dx1, dx2);                             \
	fr = vis_fpack16(dr)

/* *********************************************************** */

#define	BILINEAR_S16_4                                          \
	dx1 = vis_fand(dx1, dmask_05);                          \
	dx1 = vis_fmul8x16(scale05, dx1);                       \
	dx2 = vis_fmul8x16(scale05, dx2);                       \
	dr = vis_fpadd16(dx1, dx2)

/* *********************************************************** */

#define	BILINEAR_S16_4_XY                                       \
	dx1 = vis_fand(dx1, dmask_025);                         \
	dx1 = vis_fmul8x16(scale025, dx1);                      \
	dx2 = vis_fmul8x16(scale025, dx2);                      \
	dx1 = vis_fpadd16(dx1, dx2);                            \
	dx3 = vis_fmul8x16(scale025, dx3);                      \
	dx4 = vis_fmul8x16(scale025, dx4);                      \
	dx3 = vis_fpadd16(dx3, dx4);                            \
	dr = vis_fpadd16(dx1, dx3)

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *pstr1;

	ALLOC_MEM_U8;

/* averaging along both axes */
	if (dh == dw) {
/* initialize GSR scale factor */
		vis_write_gsr(5 << 3);
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				fx = lut[pstr1[0]];
				fx1 = lut[pstr1[1]];
				fx2 = lut[pstr1[src_stride]];
				fx3 = lut[pstr1[src_stride + 1]];
				BILINEAR_U8_4_XY;
				dst_color[j] = fr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
/* initialize GSR scale factor */
		vis_write_gsr(6 << 3);

		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[1]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[src_stride]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *pstr1;

	ALLOC_MEM_U8;

/* averaging along both axes */
	if (dh == dw) {
/* initialize GSR scale factor */
		vis_write_gsr(5 << 3);
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				fx = lut[pstr1[0]];
				fx1 = lut[pstr1[1]];
				fx2 = lut[pstr1[src_stride]];
				fx3 = lut[pstr1[src_stride + 1]];
				BILINEAR_U8_4_XY;
				dst_color[j] = fr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
/* initialize GSR scale factor */
		vis_write_gsr(6 << 3);

		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[1]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[src_stride]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *pstr1;

	ALLOC_MEM_S16;

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx1 = lut[pstr1[0]];
				dx2 = lut[pstr1[1]];
				dx3 = lut[pstr1[src_stride]];
				dx4 = lut[pstr1[src_stride + 1]];
				BILINEAR_S16_4_XY;
				dst_color[j] = dr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[1]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[src_stride]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *pstr1;

	ALLOC_MEM_S16;

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx1 = lut[pstr1[0]];
				dx2 = lut[pstr1[1]];
				dx3 = lut[pstr1[src_stride]];
				dx4 = lut[pstr1[src_stride + 1]];
				BILINEAR_S16_4_XY;
				dst_color[j] = dr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[1]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[src_stride]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_U8_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *pstr1;

	ALLOC_MEM_U8;

/* averaging along both axes */
	if (dh == dw) {
/* initialize GSR scale factor */
		vis_write_gsr(5 << 3);
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				fx = lut[pstr1[0]];
				fx1 = lut[pstr1[1]];
				fx2 = lut[pstr1[src_stride]];
				fx3 = lut[pstr1[src_stride + 1]];
				BILINEAR_U8_4_XY;
				dst_color[j] = fr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
/* initialize GSR scale factor */
		vis_write_gsr(6 << 3);

		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[1]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[src_stride]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_U8_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *pstr1;

	ALLOC_MEM_U8;

/* averaging along both axes */
	if (dh == dw) {
/* initialize GSR scale factor */
		vis_write_gsr(5 << 3);
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				fx = lut[pstr1[0]];
				fx1 = lut[pstr1[1]];
				fx2 = lut[pstr1[src_stride]];
				fx3 = lut[pstr1[src_stride + 1]];
				BILINEAR_U8_4_XY;
				dst_color[j] = fr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
/* initialize GSR scale factor */
		vis_write_gsr(6 << 3);

		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[1]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx = lut[pstr1[0]];
					fx1 = lut[pstr1[src_stride]];
					BILINEAR_U8_4;
					dst_color[j] = fr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_S16_3_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *pstr1;

	ALLOC_MEM_S16;

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx1 = lut[pstr1[0]];
				dx2 = lut[pstr1[1]];
				dx3 = lut[pstr1[src_stride]];
				dx4 = lut[pstr1[src_stride + 1]];
				BILINEAR_S16_4_XY;
				dst_color[j] = dr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[1]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[src_stride]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_S16_4_Bilinear(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *pstr1;

	ALLOC_MEM_S16;

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dx1 = lut[pstr1[0]];
				dx2 = lut[pstr1[1]];
				dx3 = lut[pstr1[src_stride]];
				dx4 = lut[pstr1[src_stride + 1]];
				BILINEAR_S16_4_XY;
				dst_color[j] = dr;
				pstr1 += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[1]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				pstr1 = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx1 = lut[pstr1[0]];
					dx2 = lut[pstr1[src_stride]];
					BILINEAR_S16_4;
					dst_color[j] = dr;
					pstr1 += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

#undef ALLOC_MEM_U8
#undef ALLOC_MEM_S16

/* *********************************************************** */

/*			the Bicubic Part			*/

/* *********************************************************** */

#define	ALLOC_MEM_U8                                                    \
	mlib_s32 i, j;                                                  \
	mlib_d64 dx0, dx1, dx2, dx3, dst0, dst1, dr;                    \
	mlib_f32 fx0, fx1, fx2, fx3,                                    \
		fzero   = vis_to_float(0x00000000),                     \
		fone    = vis_to_float(0x00000100),                     \
		fnine   = vis_to_float(0x00000900),                     \
		fone16  = vis_to_float(0x10101010),                     \
		fnine16 = vis_to_float(0x90909090);                     \
	mlib_f32 *lut, *dst_color;                                      \
	                                                                \
	if (dst_width <= 0 || dst_height <= 0)                          \
	    return (MLIB_SUCCESS);                                      \
	i = (dst_width < dst_height) ? dst_height : dst_width;          \
	dst_color = (mlib_f32 *)__mlib_malloc(4 * i);                   \
	if (dst_color == NULL)                                          \
	    return (MLIB_FAILURE);                                      \
	lut =                                                           \
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -         \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	ALLOC_MEM_S16                                             \
	mlib_s32 i, j;                                            \
	mlib_d64 dx0, dx1, dx2, dx3, dst0, dst1, dr;              \
	mlib_f32 four    = vis_to_float(0x00040004),              \
		fnine16 = vis_to_float(0x90909090),  /* 9 / 16 */ \
		fone16  = vis_to_float(0x10101010),  /* 1 / 16 */ \
		fnine32 = vis_to_float(0x48484848),  /* 9 / 32 */ \
		fone32  = vis_to_float(0x08080808);  /* 1 / 32 */ \
	mlib_d64 *lut, *dst_color;                                \
	                                                          \
	if (dst_width <= 0 || dst_height <= 0)                    \
	    return (MLIB_SUCCESS);                                \
	i = (dst_width < dst_height) ? dst_height : dst_width;    \
	dst_color = (mlib_d64 *)__mlib_malloc(2 * 4 * i);         \
	if (dst_color == NULL)                                    \
	    return (MLIB_FAILURE);                                \
	lut =                                                     \
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -   \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	BICUBIC_U8_4(dst)                                       \
	dx1 = vis_fmul8x16al(fx1, fnine);                       \
	dx0 = vis_fpmerge(fzero, fx0);                          \
	dx2 = vis_fmul8x16al(fx2, fnine);                       \
	dx3 = vis_fmul8x16al(fx3, fone);                        \
	dst = vis_fpsub16(dx1, dx0);                            \
	dst = vis_fpadd16(dst, dx2);                            \
	dst = vis_fpsub16(dst, dx3)

/* *********************************************************** */

#define	BICUBIC_S16_4(dst)                                      \
	dx0 = vis_fmul8x16(fone32, dx0);                        \
	dx1 = vis_fmul8x16(fnine32, dx1);                       \
	dx2 = vis_fmul8x16(fnine32, dx2);                       \
	dx3 = vis_fmul8x16(fone32, dx3);                        \
	dst = vis_fpadd16(dx1, dx2);                            \
	dst = vis_fpsub16(dst, dx0);                            \
	dst = vis_fpsub16(dst, dx3)

/* *********************************************************** */

#define	BICUBIC_SAT_S16_4(dst)                                  \
	dx2 = vis_fmuld8ulx16(four, vis_read_hi(dst));          \
	dx3 = vis_fmuld8ulx16(four, vis_read_lo(dst));          \
	dst = vis_fpackfix_pair(dx2, dx3)

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_U8_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_U8_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_S16_3_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_S16_4_Bicubic(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

#undef ALLOC_MEM_U8
#undef ALLOC_MEM_S16

/* *********************************************************** */

/*			the Bicubic2 Part			*/

/* *********************************************************** */

#define	ALLOC_MEM_U8                                                    \
	mlib_s32 i, j;                                                  \
	mlib_d64 dx0, dx1, dx2, dx3, dst0, dst1, dr;                    \
	mlib_f32 fx0, fx1, fx2, fx3,                                    \
		fzero   = vis_to_float(0),                              \
		fone    = vis_to_float(0x00000100),                     \
		fnine   = vis_to_float(0x00000500),                     \
		fone16  = vis_to_float(0x20202020),                     \
		fnine16 = vis_to_float(0xa0a0a0a0);                     \
	mlib_f32 *lut, *dst_color;                                      \
	                                                                \
	if (dst_width <= 0 || dst_height <= 0)                          \
	    return (MLIB_SUCCESS);                                      \
	i = (dst_width < dst_height) ? dst_height : dst_width;          \
	dst_color = (mlib_f32 *)__mlib_malloc(4 * i);                   \
	if (dst_color == NULL)                                          \
	    return (MLIB_FAILURE);                                      \
	lut =                                                           \
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -         \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	BICUBIC2_U8_4(dst)                                      \
	dx1 = vis_fmul8x16al(fx1, fnine);                       \
	dx0 = vis_fpmerge(fzero, fx0);                          \
	dx2 = vis_fmul8x16al(fx2, fnine);                       \
	dx3 = vis_fmul8x16al(fx3, fone);                        \
	dst = vis_fpsub16(dx1, dx0);                            \
	dst = vis_fpadd16(dst, dx2);                            \
	dst = vis_fpsub16(dst, dx3)

#define	U8_GSR 4

/* *********************************************************** */

#define	ALLOC_MEM_S16                                                  \
	mlib_s32 i, j;                                                 \
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx5, dst0, dst1, dr;         \
	mlib_f32 four    = vis_to_float(0x00040004),                   \
		fnine16 = vis_to_float(0xa0a0a0a0),                    \
		fone16  = vis_to_float(0x20202020),                    \
		fnine32 = vis_to_float(0x50505050),                    \
		fone32  = vis_to_float(0x10101010);                    \
	mlib_d64 *lut, *dst_color;                                     \
	                                                               \
	if (dst_width <= 0 || dst_height <= 0)                         \
	    return (MLIB_SUCCESS);                                     \
	i = (dst_width < dst_height) ? dst_height : dst_width;         \
	dst_color = (mlib_d64 *)__mlib_malloc(2 * 4 * i);              \
	if (dst_color == NULL)                                         \
	    return (MLIB_FAILURE);                                     \
	lut =                                                          \
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -        \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#undef BICUBIC_S16_4
#define	BICUBIC_S16_4(dst)                                      \
	dx0 = vis_fmul8x16(fone32, dx0);                        \
	dx1 = vis_fmul8x16(fnine32, dx1);                       \
	dx2 = vis_fmul8x16(fnine32, dx2);                       \
	dx3 = vis_fmul8x16(fone32, dx3);                        \
	dst = vis_fpadd16(dx1, dx2);                            \
	dst = vis_fpsub16(dst, dx0);                            \
	dst = vis_fpsub16(dst, dx3)

/* *********************************************************** */

#undef BICUBIC_SAT_S16_4
#define	BICUBIC_SAT_S16_4(dst)                                  \
	dx4 = vis_fmuld8ulx16(four, vis_read_hi(dst));          \
	dx5 = vis_fmuld8ulx16(four, vis_read_lo(dst));          \
	dst = vis_fpackfix_pair(dx4, dx5)

#define	S16_GSR	15

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(U8_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC2_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC2_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC2_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC2_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC2_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(U8_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC2_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC2_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC2_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC2_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_U8_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC2_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_U8_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_U8_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(U8_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC2_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC2_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC2_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC2_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC2_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_U8_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_U8;

/* initialize GSR scale factor */
	vis_write_gsr(U8_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			fx0 = lut[ps[-src_stride - 1]];
			fx1 = lut[ps[-1]];
			fx2 = lut[ps[src_stride - 1]];
			fx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC2_U8_4(dst0);
			fx0 = lut[ps[-src_stride]];
			fx1 = lut[ps[0]];
			fx2 = lut[ps[src_stride]];
			fx3 = lut[ps[2 * src_stride]];
			BICUBIC2_U8_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				fx0 = lut[ps[-src_stride + 1]];
				fx1 = lut[ps[1]];
				fx2 = lut[ps[src_stride + 1]];
				fx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC2_U8_4(dst0);
				fx0 = lut[ps[-src_stride + 2]];
				fx1 = lut[ps[2]];
				fx2 = lut[ps[src_stride + 2]];
				fx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC2_U8_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				dst_color[j] = vis_fpack16(dr);
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_U8_S16_4
			    ((mlib_u8 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				fx1 = lut[ps[0]];
				fx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dr = vis_fmul8x16al(fx1, fnine);
					dx0 = vis_fmul8x16al(fx0, fone);
					dr = vis_fpsub16(dr, dx0);
					fx0 = lut[ps[1]];
					fx1 = lut[ps[2]];
					dx0 = vis_fmul8x16al(fx0, fnine);
					dx1 = vis_fpmerge(fzero, fx1);
					dr = vis_fpsub16(dr, dx1);
					dr = vis_fpadd16(dx0, dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					fx0 = lut[ps[-src_stride]];
					fx1 = lut[ps[0]];
					fx2 = lut[ps[src_stride]];
					fx3 = lut[ps[2 * src_stride]];
					BICUBIC2_U8_4(dr);
					dst_color[j] = vis_fpack16(dr);
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_U8_S16_4
				    ((mlib_u8 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(S16_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_S16_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_s16))
{
	mlib_s16 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(S16_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_S16_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_S16_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_S16_3_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(S16_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomOut2XIndex_U8_S16_4_Bicubic2(
    BILINEAR_INDEX_PARAMETERS(mlib_u8))
{
	mlib_u8 *ps;

	ALLOC_MEM_S16;
/* initialize GSR scale factor */
	vis_write_gsr(S16_GSR << 3);

/* averaging along both axes */
	if (dh == dw) {
		for (i = 0; i < dst_height; i++) {
			ps = p_src + 2 * i * src_stride;
			dx0 = lut[ps[-src_stride - 1]];
			dx1 = lut[ps[-1]];
			dx2 = lut[ps[src_stride - 1]];
			dx3 = lut[ps[2 * src_stride - 1]];
			BICUBIC_S16_4(dst0);
			dx0 = lut[ps[-src_stride]];
			dx1 = lut[ps[0]];
			dx2 = lut[ps[src_stride]];
			dx3 = lut[ps[2 * src_stride]];
			BICUBIC_S16_4(dst1);
#pragma pipeloop(0)
			for (j = 0; j < dst_width; j++) {
				dst0 = vis_fmul8x16(fone16, dst0);
				dst1 = vis_fmul8x16(fnine16, dst1);
				dr = vis_fpsub16(dst1, dst0);
				dx0 = lut[ps[-src_stride + 1]];
				dx1 = lut[ps[1]];
				dx2 = lut[ps[src_stride + 1]];
				dx3 = lut[ps[2 * src_stride + 1]];
				BICUBIC_S16_4(dst0);
				dx0 = lut[ps[-src_stride + 2]];
				dx1 = lut[ps[2]];
				dx2 = lut[ps[src_stride + 2]];
				dx3 = lut[ps[2 * src_stride + 2]];
				BICUBIC_S16_4(dst1);
				dx0 = vis_fmul8x16(fnine16, dst0);
				dx1 = vis_fmul8x16(fone16, dst1);
				dr = vis_fpadd16(dr, dx0);
				dr = vis_fpsub16(dr, dx1);
				BICUBIC_SAT_S16_4(dr);
				dst_color[j] = dr;
				ps += 2;
			}

			mlib_ImageColorTrue2IndexLine_S16_U8_4
			    ((mlib_s16 *)dst_color, p_dst + i * dst_stride,
			    dst_width, colormap);
		}
	} else {
		if (dh)	/* averaging along X axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
				dx1 = lut[ps[0]];
				dx0 = lut[ps[-1]];
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dst0 = vis_fmul8x16(fone32, dx0);
					dst1 = vis_fmul8x16(fnine32, dx1);
					dst0 = vis_fpsub16(dst1, dst0);
					dx0 = lut[ps[1]];
					dx1 = lut[ps[2]];
					dx2 = vis_fmul8x16(fnine32, dx0);
					dst0 = vis_fpadd16(dst0, dx2);
					dx3 = vis_fmul8x16(fone32, dx1);
					dr = vis_fpsub16(dst0, dx3);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
		} else	/* averaging along Y axis */
			for (i = 0; i < dst_height; i++) {
				ps = p_src + 2 * i * src_stride;
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					dx0 = lut[ps[-src_stride]];
					dx1 = lut[ps[0]];
					dx2 = lut[ps[src_stride]];
					dx3 = lut[ps[2 * src_stride]];
					BICUBIC_S16_4(dr);
					BICUBIC_SAT_S16_4(dr);
					dst_color[j] = dr;
					ps += 2;
				}

				mlib_ImageColorTrue2IndexLine_S16_U8_4
				    ((mlib_s16 *)dst_color,
				    p_dst + i * dst_stride, dst_width,
				    colormap);
			}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
