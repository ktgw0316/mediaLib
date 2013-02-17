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

#pragma ident	"@(#)mlib_ImageZoomOut2XIndex_Edge.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_status mlib_ImageZoomOut2XIndex_U8_Bilinear_Edge
 *      mlib_status mlib_ImageZoomOut2XIndex_S16_Bilinear_Edge
 *      mlib_status mlib_ImageZoomOut2XIndex_U8_Bicubic_Edge
 *      mlib_status mlib_ImageZoomOut2XIndex_S16_Bicubic_Edge
 *      mlib_status mlib_ImageZoomOut2XIndex_U8_Bicubic2_Edge
 *      mlib_status mlib_ImageZoomOut2XIndex_S16_Bicubic2_Edge
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2XIndex_*_*_Edge(mlib_image *dst,
 *                                                    const mlib_image *src,
 *                                                    mlib_edge edge,
 *                                                    void *colormap)
 *
 * ARGUMENTS
 *      src       Pointer to source image.
 *      dst       Pointer to destination image.
 *      edge      Type of edge condition.
 *      colormap  Pointer to mlib colormap structure.
 *
 *  The resampling filter can be one of the following:
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageZoomOut2X.h>
#include <mlib_ImageZoom2X.h>

/* *********************************************************** */

#define	ch0	0
#define	ch1	1

#define	ch2	2

#define	ch3	3

/* *********************************************************** */

#define	EDGE_INDEX_BILINEAR(type)                                      \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);            \
	mlib_type type_colormap = mlib_ImageGetLutType(colormap);      \
	mlib_s32 ind, channels = mlib_ImageGetLutChannels(colormap);   \
	                                                               \
	EDGE_BILINEAR(type)

/* *********************************************************** */

#define	CASE_EDGE_INDEX_FILL_ZERO                                    \
	if (top)                                                     \
	    for (j = 0; j < dst_width; j++)                          \
		p_dst[j] = offset;                                   \
	if (bottom)                                                  \
	    for (j = 0; j < dst_width; j++)                          \
		p_dst[(dst_height - 1) * dst_stride + j] = offset;   \
	if (left)                                                    \
	    for (i = 0; i < dst_height; i++)                         \
		p_dst[i * dst_stride] = offset;                      \
	if (right)                                                   \
	    for (i = 0; i < dst_height; i++)                         \
		p_dst[i * dst_stride + dst_width - 1] = offset

/* *********************************************************** */
#define	CASE_EDGE_INDEX_OP_NEAREST                                      \
	if (top)                                                        \
	    for (j = 0; j < dst_width; j++)                             \
		p_dst[j] = p_src[2 * j];                                \
	if (bottom)                                                     \
	    for (j = 0; j < dst_width; j++)                             \
		p_dst[(dst_height - 1) * dst_stride + j] =              \
		    p_src[2 * (dst_height - 1) * src_stride + 2 * j];   \
	if (left)                                                       \
	    for (i = 0; i < dst_height; i++)                            \
		p_dst[i * dst_stride] = p_src[2 * i * src_stride];      \
	if (right)                                                      \
	    for (i = 0; i < dst_height; i++)                            \
		p_dst[i * dst_stride + dst_width - 1] =                 \
		    p_src[2 * i * src_stride + 2 * (dst_width - 1)]

/* *********************************************************** */
#define	CALC_EDGE_U8_4(i1, i2, i3, ib, ie)                              \
	for (i3 = ib, ind = ib * 4; i3 < ie; i3++, ind += 4) {          \
	    mlib_s32 color0 = lut[i1];                                  \
	    mlib_s32 color1 = lut[i2];                                  \
	                                                                \
	    dst_color[ind] =                                            \
		(GET_CHAN_U8_0(color0) + GET_CHAN_U8_0(color1)) >> 1;   \
	    dst_color[ind + 1] =                                        \
		(GET_CHAN_U8_1(color0) + GET_CHAN_U8_1(color1)) >> 1;   \
	    dst_color[ind + 2] =                                        \
		(GET_CHAN_U8_2(color0) + GET_CHAN_U8_2(color1)) >> 1;   \
	    dst_color[ind + 3] =                                        \
		(GET_CHAN_U8_3(color0) + GET_CHAN_U8_3(color1)) >> 1;   \
	}
/* *********************************************************** */
#define	CALC_EDGE_U8_3(i1, i2, i3, ib, ie)                              \
	for (i3 = ib, ind = ib * 3; i3 < ie; i3++, ind += 3) {          \
	    mlib_s32 color0 = lut[i1];                                  \
	    mlib_s32 color1 = lut[i2];                                  \
	                                                                \
	    dst_color[ind] =                                            \
		(GET_CHAN_U8_1(color0) + GET_CHAN_U8_1(color1)) >> 1;   \
	    dst_color[ind + 1] =                                        \
		(GET_CHAN_U8_2(color0) + GET_CHAN_U8_2(color1)) >> 1;   \
	    dst_color[ind + 2] =                                        \
		(GET_CHAN_U8_3(color0) + GET_CHAN_U8_3(color1)) >> 1;   \
	}
/* *********************************************************** */
#define	CALC_EDGE_S16_4(I1, I2, IJ, IB, IE)                            \
	for (IJ = IB, ind = IB * 4; IJ < IE; IJ++, ind += 4) {         \
	    mlib_s16 *pcolor0 = (mlib_s16 *)(lut + I1),                \
		*pcolor1 = (mlib_s16 *)(lut + I2);                     \
	    dst_color[ind] = (pcolor0[ch0] + pcolor1[ch0]) >> 1;       \
	    dst_color[ind + 1] = (pcolor0[ch1] + pcolor1[ch1]) >> 1;   \
	    dst_color[ind + 2] = (pcolor0[ch2] + pcolor1[ch2]) >> 1;   \
	    dst_color[ind + 3] = (pcolor0[ch3] + pcolor1[ch3]) >> 1;   \
	}
/* *********************************************************** */
#define	CALC_EDGE_S16_3(I1, I2, IJ, IB, IE)                            \
	for (IJ = IB, ind = IB * 3; IJ < IE; IJ++, ind += 3) {         \
	    mlib_s16 *pcolor0 = (mlib_s16 *)(lut + I1),                \
		*pcolor1 = (mlib_s16 *)(lut + I2);                     \
	    dst_color[ind] = (pcolor0[ch1] + pcolor1[ch1]) >> 1;       \
	    dst_color[ind + 1] = (pcolor0[ch2] + pcolor1[ch2]) >> 1;   \
	    dst_color[ind + 2] = (pcolor0[ch3] + pcolor1[ch3]) >> 1;   \
	}
/* *********************************************************** */
#define	CASE_EDGE_INDEX_BILINEAR_EXTEND(Type, TP, TPC)                       \
	{                                                                    \
	    ALLOC_MEM_BL_##TPC;                                              \
	    if (top) {                                                       \
		if (dw)                                                      \
		    for (j = 0; j < dst_width; j++)                          \
			p_dst[j] = p_src[2 * j];                             \
		else {                                                       \
		    if (left)                                                \
			p_dst[0] = p_src[0];                                 \
		    if (channels == 4) {                                     \
			CALC_EDGE_##TPC##_4(p_src[2 * j - 1],                \
			    p_src[2 * j], j, left, dst_width - right);       \
			mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4       \
			    (dst_color + 4 * left, p_dst + left,             \
			    dst_width - right - left, colormap);             \
		    } else {                                                 \
			CALC_EDGE_##TPC##_3(p_src[2 * j - 1],                \
			    p_src[2 * j], j, left, dst_width - right);       \
			mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3       \
			    (dst_color + 3 * left, p_dst + left,             \
			    dst_width - right - left, colormap);             \
		    }                                                        \
		    if (right)                                               \
			p_dst[dst_width - 1] =                               \
			    p_src[2 * (dst_width - 1)];                      \
		}                                                            \
	    }                                                                \
	    if (bottom) {                                                    \
		if (dw) {                                                    \
		    for (j = 0; j < dst_width; j++) {                        \
			p_dst[(dst_height - 1) * dst_stride + j] =           \
			    p_src[2 * (dst_height - 1) * src_stride +        \
			    2 * j];                                          \
		    }                                                        \
		} else {                                                     \
		    if (left)                                                \
			p_dst[(dst_height - 1) * dst_stride] =               \
			    p_src[2 * (dst_height - 1) * src_stride];        \
		    if (channels == 4) {                                     \
			CALC_EDGE_##TPC##_4(p_src[2 *                        \
			    (dst_height - 1) * src_stride + 2 * j - 1],      \
			    p_src[2 * (dst_height - 1) * src_stride +        \
			    2 * j], j, left,                                 \
			    (dst_width -                                     \
			    right))                                          \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4   \
			    (dst_color + 4 * left,                           \
			    p_dst + (dst_height - 1) * dst_stride +          \
			    left, dst_width - right - left, colormap);       \
		    } else {                                                 \
			CALC_EDGE_##TPC##_3(p_src[2 *                        \
			    (dst_height - 1) * src_stride + 2 * j - 1],      \
			    p_src[2 * (dst_height - 1) * src_stride +        \
			    2 * j], j, left,                                 \
			    (dst_width -                                     \
			    right))                                          \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3   \
			    (dst_color + 3 * left,                           \
			    p_dst + (dst_height - 1) * dst_stride +          \
			    left, dst_width - right - left, colormap);       \
		    }                                                        \
		    if (right)                                               \
			p_dst[(dst_height - 1) * dst_stride +                \
			    dst_width - 1] =                                 \
			    p_src[2 * (dst_height - 1) * src_stride +        \
			    2 * (dst_width - 1)];                            \
		}                                                            \
	    }                                                                \
	    if (left) {                                                      \
		if (dh)                                                      \
		    for (i = 0; i < dst_height; i++)                         \
			p_dst[i * dst_stride] =                              \
			    p_src[2 * i * src_stride];                       \
		else {                                                       \
		    if (channels == 4) {                                     \
			CALC_EDGE_##TPC##_4(p_src[(2 * i -                   \
			    1) * src_stride], p_src[2 * i * src_stride],     \
			    i, top,                                          \
			    (dst_height -                                    \
			    bottom))                                         \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4   \
			    (dst_color + top * 4,                            \
			    (Type *) dst_color + top,                        \
			    dst_height - bottom - top, colormap);            \
		    } else {                                                 \
			CALC_EDGE_##TPC##_3(p_src[(2 * i -                   \
			    1) * src_stride], p_src[2 * i * src_stride],     \
			    i, top,                                          \
			    (dst_height -                                    \
			    bottom))                                         \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3   \
			    (dst_color + top * 3,                            \
			    (Type *) dst_color + top,                        \
			    dst_height - bottom - top, colormap);            \
		    }                                                        \
		    for (i = top; i < (dst_height - bottom); i++)            \
			p_dst[i * dst_stride] = ((Type *) dst_color)[i];     \
		}                                                            \
	    }                                                                \
	    if (right) {                                                     \
		if (dh)                                                      \
		    for (i = 0; i < dst_height; i++)                         \
			p_dst[i * dst_stride + dst_width - 1] =              \
			    p_src[2 * i * src_stride + 2 * (dst_width -      \
			    1)];                                             \
		else {                                                       \
		    if (channels == 4) {                                     \
			CALC_EDGE_##TPC##_4(p_src[(2 * i -                   \
			    1) * src_stride + 2 * (dst_width - 1)],          \
			    p_src[2 * i * src_stride + 2 * (dst_width -      \
			    1)], i, top,                                     \
			    (dst_height -                                    \
			    bottom))                                         \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4   \
			    (dst_color + top * 4,                            \
			    (Type *) dst_color + top,                        \
			    dst_height - bottom - top, colormap);            \
		    } else {                                                 \
			CALC_EDGE_##TPC##_3(p_src[(2 * i -                   \
			    1) * src_stride + 2 * (dst_width - 1)],          \
			    p_src[2 * i * src_stride + 2 * (dst_width -      \
			    1)], i, top,                                     \
			    (dst_height -                                    \
			    bottom))                                         \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3   \
			    (dst_color + top * 3,                            \
			    (Type *) dst_color + top,                        \
			    dst_height - bottom - top, colormap);            \
		    }                                                        \
		    for (i = top; i < (dst_height - bottom); i++)            \
			p_dst[i * dst_stride + dst_width - 1] =              \
			    ((Type *) dst_color)[i];                         \
		}                                                            \
	    }                                                                \
	    FREE_MEM;                                                        \
	}
/* *********************************************************** */
	mlib_status mlib_ImageZoomOut2XIndex_U8_Bilinear_Edge(
    EDGE_INDEX_PARAMETERS)
{
	EDGE_INDEX_BILINEAR(mlib_u8);

	l = (mlib_s32)edge;
	switch (l) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_INDEX_FILL_ZERO;
		break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_INDEX_OP_NEAREST;
		break;
	case MLIB_EDGE_SRC_EXTEND:

		if (type_colormap == MLIB_BYTE)
			CASE_EDGE_INDEX_BILINEAR_EXTEND(mlib_u8,
			    U8,
			    U8)
		else
			CASE_EDGE_INDEX_BILINEAR_EXTEND(mlib_u8,
			    U8,
			    S16)
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2XIndex_S16_Bilinear_Edge(
    EDGE_INDEX_PARAMETERS)
{
	EDGE_INDEX_BILINEAR(mlib_s16);

	l = (mlib_s32)edge;
	switch (l) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_INDEX_FILL_ZERO;
		break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_INDEX_OP_NEAREST;
		break;
	case MLIB_EDGE_SRC_EXTEND:

		if (type_colormap == MLIB_BYTE)
			CASE_EDGE_INDEX_BILINEAR_EXTEND(mlib_s16,
			    S16,
			    U8)
		else
			CASE_EDGE_INDEX_BILINEAR_EXTEND(mlib_s16,
			    S16,
			    S16)
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	EDGE_INDEX_BICUBIC(type)                                    \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);         \
	mlib_type type_colormap = mlib_ImageGetLutType(colormap);   \
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);     \
	                                                            \
	EDGE_BICUBIC(type)

/* *********************************************************** */

#define	CASE_EDGE_INDEX_BICUBIC_EXTEND(TP, TypeP, TPC, TypeC)                  \
	{                                                                      \
	    ALLOC_MEM_BC_##TPC;                                                \
	    if (top) {                                                         \
		if (dst_height == 1 && dh == 0) {                              \
/*                                                                             \
 * 1-3 size addition                                                           \
 */                                                                            \
		    if ((dw & 1)) {                                            \
/*                                                                             \
 * y-case                                                                      \
 */                                                                            \
			mlib_s32 pred_index = src_stride;                      \
	                                                                       \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
	                                                                       \
			    for (j = 0; j < dst_width; j++) {                  \
				res =                                          \
				    (BC_CD * (GET_CHAN(p_src[2 * j]) +         \
				    GET_CHAN(p_src[-pred_index +               \
				    2 * j]))) BC_NRML;                         \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
		    } else {                                                   \
/*                                                                             \
 * xy-case                                                                     \
 */                                                                            \
			mlib_s32 pred_index_h = src_stride;                    \
			mlib_s32 pred_index_w = exist_w | (dw == 0);           \
	                                                                       \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
			    mlib_s32 st0 =                                     \
				BC_CD * (GET_CHAN(p_src[-(exist_w +            \
				pred_index_w)]) +                              \
				GET_CHAN(p_src[-pred_index_h -                 \
				(exist_w + pred_index_w)])), st1 =             \
				BC_CD *                                        \
				(GET_CHAN(p_src[-pred_index_w]) +              \
				GET_CHAN(p_src[-pred_index_h -                 \
				pred_index_w])), st2, st3;                     \
			    for (j = 0; j < (dst_width - 1); j++) {            \
				st2 =                                          \
				    BC_CD * (GET_CHAN(p_src[2 * j]) +          \
				    GET_CHAN(p_src[-pred_index_h +             \
				    2 * j]));                                  \
				st3 =                                          \
				    BC_CD * (GET_CHAN(p_src[2 * j +            \
				    1]) + GET_CHAN(p_src[-pred_index_h +       \
				    2 * j + 1]));                              \
				res =                                          \
				    (BC_CF * (st1 + st2) - st0 -               \
				    st3) BC_NRM2;                              \
				st0 = st2;                                     \
				st1 = st3;                                     \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			    st2 =                                              \
				BC_CD * (GET_CHAN(p_src[2 * j]) +              \
				GET_CHAN(p_src[-pred_index_h + 2 * j]));       \
			    st3 =                                              \
				BC_CD * (GET_CHAN(p_src[2 * j +                \
				(exist_w | (dw ==                              \
				2))]) + GET_CHAN(p_src[-pred_index_h +         \
				2 * j + (exist_w | (dw == 2))]));              \
			    res =                                              \
				(BC_CF * (st1 + st2) - st0 -                   \
				st3) BC_NRM2;                                  \
			    dst_color[j * channels + l] =                      \
				SAT_##TPC(res);                                \
			}                                                      \
		    }                                                          \
		    if (channels == 4)                                         \
			mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4         \
			    (dst_color, p_dst, dst_width, colormap);           \
		    else                                                       \
			mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3         \
			    (dst_color, p_dst, dst_width, colormap);           \
		    bottom = 0;                                                \
		    left = 0;                                                  \
		    right = 0;                                                 \
		}                                                              \
/*                                                                             \
 * end addition                                                                \
 */                                                                            \
		else {                                                         \
		    if ((dw & 1) == 0) {                                       \
			mlib_s32 pred_index_w = exist_w | (dw == 0);           \
	                                                                       \
			if (dh & 1) {                                          \
			    for (l = 0; l < channels; l++) {                   \
				TypeC *src_color = lut[l] - offset;            \
				mlib_s32 a0 =                                  \
				    GET_CHAN(p_src[-(exist_w +                 \
				    pred_index_w)]), a1 =                      \
				    GET_CHAN(p_src[-pred_index_w]);            \
				for (j = 0; j < (dst_width - 1); j++) {        \
				    res =                                      \
					(BC_CF * (GET_CHAN(p_src[2 *           \
					j]) + a1) - a0 -                       \
					GET_CHAN(p_src[2 * j +                 \
					1])) BC_NRML;                          \
				    a0 = GET_CHAN(p_src[2 * j]);               \
				    a1 = GET_CHAN(p_src[2 * j + 1]);           \
				    dst_color[j * channels + l] =              \
					SAT_##TPC(res);                        \
				}                                              \
				res =                                          \
				    (BC_CF * (GET_CHAN(p_src[2 * j]) +         \
				    a1) - a0 - GET_CHAN(p_src[2 * j +          \
				    (exist_w | (dw == 2))])) BC_NRML;          \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			    if (channels == 4)                                 \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4 \
				    (dst_color, p_dst, dst_width,              \
				    colormap);                                 \
			    else                                               \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3 \
				    (dst_color, p_dst, dst_width,              \
				    colormap);                                 \
			} else {                                               \
			    mlib_s32 pred_index_h =                            \
				src_stride * (dh == 0);                        \
			    for (l = 0; l < channels; l++) {                   \
				TypeC *src_color = lut[l] - offset;            \
				mlib_s32 st0 =                                 \
				    BC_CF * GET_CHAN(p_src[-(exist_w +         \
				    pred_index_w)]) +                          \
				    BC_CD *                                    \
				    GET_CHAN(p_src[-pred_index_h -             \
				    (exist_w + pred_index_w)]) -               \
				    GET_CHAN(p_src[src_stride -                \
				    (exist_w + pred_index_w)]), st1 =          \
				    BC_CF *                                    \
				    GET_CHAN(p_src[-pred_index_w]) +           \
				    BC_CD *                                    \
				    GET_CHAN(p_src[-pred_index_h -             \
				    pred_index_w]) -                           \
				    GET_CHAN(p_src[src_stride -                \
				    pred_index_w]), st2, st3;                  \
				for (j = 0; j < (dst_width - 1); j++) {        \
				    st2 =                                      \
					BC_CF * GET_CHAN(p_src[2 * j]) +       \
					BC_CD *                                \
					GET_CHAN(p_src[-pred_index_h +         \
					2 * j]) -                              \
					GET_CHAN(p_src[src_stride +            \
					2 * j]);                               \
				    st3 =                                      \
					BC_CF * GET_CHAN(p_src[2 * j +         \
					1]) +                                  \
					BC_CD *                                \
					GET_CHAN(p_src[-pred_index_h +         \
					2 * j + 1]) -                          \
					GET_CHAN(p_src[src_stride +            \
					2 * j + 1]);                           \
				    res =                                      \
					(BC_CF * (st1 + st2) - st0 -           \
					st3) BC_NRM2;                          \
				    st0 = st2;                                 \
				    st1 = st3;                                 \
				    dst_color[j * channels + l] =              \
					SAT_##TPC(res);                        \
				}                                              \
				st2 =                                          \
				    BC_CF * GET_CHAN(p_src[2 * j]) +           \
				    BC_CD *                                    \
				    GET_CHAN(p_src[-pred_index_h +             \
				    2 * j]) -                                  \
				    GET_CHAN(p_src[src_stride + 2 * j]);       \
				st3 =                                          \
				    BC_CF * GET_CHAN(p_src[2 * j +             \
				    (exist_w | (dw ==                          \
				    2))]) +                                    \
				    BC_CD *                                    \
				    GET_CHAN(p_src[-pred_index_h +             \
				    2 * j + (exist_w | (dw ==                  \
				    2))]) - GET_CHAN(p_src[src_stride +        \
				    2 * j + (exist_w | (dw == 2))]);           \
				res =                                          \
				    (BC_CF * (st1 + st2) - st0 -               \
				    st3) BC_NRM2;                              \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color, p_dst, dst_width, colormap);       \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color, p_dst, dst_width, colormap);       \
		    } else {                                                   \
			if (dh & 1)                                            \
			    for (j = 0; j < dst_width; j++)                    \
				p_dst[j] = p_src[2 * j];                       \
			else {                                                 \
			    mlib_s32 pred_index =                              \
				src_stride * (dh == 0);                        \
			    for (l = 0; l < channels; l++) {                   \
				TypeC *src_color = lut[l] - offset;            \
	                                                                       \
				for (j = 0; j < dst_width; j++) {              \
				    res =                                      \
					(BC_CF * GET_CHAN(p_src[2 *            \
					j]) +                                  \
					BC_CD *                                \
					GET_CHAN(p_src[-pred_index +           \
					2 * j]) -                              \
					GET_CHAN(p_src[src_stride +            \
					2 * j])) BC_NRML;                      \
				    dst_color[j * channels + l] =              \
					SAT_##TPC(res);                        \
				}                                              \
			    }                                                  \
			    if (channels == 4)                                 \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4 \
				    (dst_color, p_dst, dst_width,              \
				    colormap);                                 \
			    else                                               \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3 \
				    (dst_color, p_dst, dst_width,              \
				    colormap);                                 \
			}                                                      \
		    }                                                          \
		}                                                              \
	    }                                                                  \
	    if (bottom) {                                                      \
		if ((dw & 1) == 0) {                                           \
/*                                                                             \
 * dw == 0 or 2                                                                \
 */                                                                            \
		    mlib_s32 pred_index_w = exist_w | (dw == 0);               \
	                                                                       \
		    if (dh & 1) {                                              \
/*                                                                             \
 * dh == 1 or 3                                                                \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
			    mlib_s32 a0 =                                      \
				GET_CHAN(p_src[2 * (dst_height -               \
				1) * src_stride - (exist_w +                   \
				pred_index_w)]), a1 =                          \
				GET_CHAN(p_src[2 * (dst_height -               \
				1) * src_stride - pred_index_w]);              \
			    for (j = 0; j < (dst_width - 1); j++) {            \
				res =                                          \
				    (BC_CF * (GET_CHAN(p_src[2 *               \
				    (dst_height - 1) * src_stride +            \
				    2 * j]) + a1) - a0 -                       \
				    GET_CHAN(p_src[2 * (dst_height -           \
				    1) * src_stride + 2 * j +                  \
				    1])) BC_NRML;                              \
				a0 = GET_CHAN(p_src[2 * (dst_height -          \
				    1) * src_stride + 2 * j]);                 \
				a1 = GET_CHAN(p_src[2 * (dst_height -          \
				    1) * src_stride + 2 * j + 1]);             \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			    res =                                              \
				(BC_CF * (GET_CHAN(p_src[2 *                   \
				(dst_height - 1) * src_stride +                \
				2 * j]) + a1) - a0 -                           \
				GET_CHAN(p_src[2 * (dst_height -               \
				1) * src_stride + 2 * j +                      \
				(exist_w | (dw == 2))])) BC_NRML;              \
			    dst_color[j * channels + l] =                      \
				SAT_##TPC(res);                                \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color,                                    \
				p_dst + (dst_height - 1) * dst_stride,         \
				dst_width, colormap);                          \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color,                                    \
				p_dst + (dst_height - 1) * dst_stride,         \
				dst_width, colormap);                          \
		    } else {                                                   \
/*                                                                             \
 * dh == 0 or 2                                                                \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
			    mlib_s32 st0 =                                     \
				BC_CD * GET_CHAN(p_src[2 * (dst_height -       \
				1) * src_stride - (exist_w +                   \
				pred_index_w)]) +                              \
				BC_CF * GET_CHAN(p_src[(2 *                    \
				(dst_height - 1) - 1) * src_stride -           \
				(exist_w + pred_index_w)]) -                   \
				GET_CHAN(p_src[(2 * (dst_height - 1) -         \
				2) * src_stride - (exist_w +                   \
				pred_index_w)]), st1 =                         \
				BC_CD * GET_CHAN(p_src[2 * (dst_height -       \
				1) * src_stride - pred_index_w]) +             \
				BC_CF * GET_CHAN(p_src[(2 *                    \
				(dst_height - 1) - 1) * src_stride -           \
				pred_index_w]) -                               \
				GET_CHAN(p_src[(2 * (dst_height - 1) -         \
				2) * src_stride - pred_index_w]), st2,         \
				st3;                                           \
			    for (j = 0; j < (dst_width - 1); j++) {            \
				st2 =                                          \
				    BC_CD * GET_CHAN(p_src[2 *                 \
				    (dst_height - 1) * src_stride +            \
				    2 * j]) +                                  \
				    BC_CF * GET_CHAN(p_src[(2 *                \
				    (dst_height - 1) - 1) * src_stride +       \
				    2 * j]) -                                  \
				    GET_CHAN(p_src[(2 * (dst_height -          \
				    1) - 2) * src_stride + 2 * j]);            \
				st3 =                                          \
				    BC_CD * GET_CHAN(p_src[2 *                 \
				    (dst_height - 1) * src_stride +            \
				    2 * j + 1]) +                              \
				    BC_CF * GET_CHAN(p_src[(2 *                \
				    (dst_height - 1) - 1) * src_stride +       \
				    2 * j + 1]) -                              \
				    GET_CHAN(p_src[(2 * (dst_height -          \
				    1) - 2) * src_stride + 2 * j + 1]);        \
				res =                                          \
				    (BC_CF * (st1 + st2) - st0 -               \
				    st3) BC_NRM2;                              \
				st0 = st2;                                     \
				st1 = st3;                                     \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			    st2 =                                              \
				BC_CD * GET_CHAN(p_src[2 * (dst_height -       \
				1) * src_stride + 2 * j]) +                    \
				BC_CF * GET_CHAN(p_src[(2 *                    \
				(dst_height - 1) - 1) * src_stride +           \
				2 * j]) -                                      \
				GET_CHAN(p_src[(2 * (dst_height - 1) -         \
				2) * src_stride + 2 * j]);                     \
			    st3 =                                              \
				BC_CD * GET_CHAN(p_src[2 * (dst_height -       \
				1) * src_stride + 2 * j +                      \
				(exist_w | (dw ==                              \
				2))]) +                                        \
				BC_CF * GET_CHAN(p_src[(2 *                    \
				(dst_height - 1) - 1) * src_stride +           \
				2 * j + (exist_w | (dw ==                      \
				2))]) -                                        \
				GET_CHAN(p_src[(2 * (dst_height - 1) -         \
				2) * src_stride + 2 * j +                      \
				(exist_w | (dw == 2))]);                       \
			    res =                                              \
				(BC_CF * (st1 + st2) - st0 -                   \
				st3) BC_NRM2;                                  \
			    dst_color[j * channels + l] =                      \
				SAT_##TPC(res);                                \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color,                                    \
				p_dst + (dst_height - 1) * dst_stride,         \
				dst_width, colormap);                          \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color,                                    \
				p_dst + (dst_height - 1) * dst_stride,         \
				dst_width, colormap);                          \
		    }                                                          \
		} else {                                                       \
/*                                                                             \
 * dw == 3 or 1                                                                \
 */                                                                            \
		    if (dh & 1)                                                \
			for (j = 0; j < dst_width; j++)                        \
			    p_dst[(dst_height - 1) * dst_stride + j] =         \
				p_src[2 * (dst_height -                        \
				1) * src_stride + 2 * j];                      \
		    else {                                                     \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
	                                                                       \
			    for (j = 0; j < dst_width; j++) {                  \
				res =                                          \
				    (BC_CD * GET_CHAN(p_src[2 *                \
				    (dst_height - 1) * src_stride +            \
				    2 * j]) +                                  \
				    BC_CF * GET_CHAN(p_src[(2 *                \
				    (dst_height - 1) - 1) * src_stride +       \
				    2 * j]) -                                  \
				    GET_CHAN(p_src[(2 * (dst_height -          \
				    1) - 2) * src_stride +                     \
				    2 * j])) BC_NRML;                          \
				dst_color[j * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color,                                    \
				p_dst + (dst_height - 1) * dst_stride,         \
				dst_width, colormap);                          \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color,                                    \
				p_dst + (dst_height - 1) * dst_stride,         \
				dst_width, colormap);                          \
		    }                                                          \
		}                                                              \
	    }                                                                  \
	    if (left) {                                                        \
		if (dst_width == 1 && dw == 0) {                               \
/*                                                                             \
 * 1-3 size addition                                                           \
 */                                                                            \
		    mlib_s32 pred_index = chan;                                \
	                                                                       \
		    if (dh & 1) {                                              \
/*                                                                             \
 * x case                                                                      \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
	                                                                       \
			    for (i = top; i < (dst_height - bottom);           \
				i++) {                                         \
				res =                                          \
				    (BC_CD * (GET_CHAN(p_src[2 * i *           \
				    src_stride - pred_index]) +                \
				    GET_CHAN(p_src[2 * i *                     \
				    src_stride]))) BC_NRML;                    \
				dst_color[i * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
		    } else {                                                   \
/*                                                                             \
 * xy case                                                                     \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
			    mlib_s32 st0 =                                     \
				BC_CD * (GET_CHAN(p_src[(2 * top -             \
				2) * src_stride - pred_index]) +               \
				GET_CHAN(p_src[(2 * top -                      \
				2) * src_stride])), st1 =                      \
				BC_CD * (GET_CHAN(p_src[(2 * top -             \
				1) * src_stride - pred_index]) +               \
				GET_CHAN(p_src[(2 * top -                      \
				1) * src_stride])), st2, st3;                  \
			    for (i = top; i < (dst_height - bottom);           \
				i++) {                                         \
				st2 =                                          \
				    BC_CD * (GET_CHAN(p_src[2 * i *            \
				    src_stride - pred_index]) +                \
				    GET_CHAN(p_src[2 * i *                     \
				    src_stride]));                             \
				st3 =                                          \
				    BC_CD * (GET_CHAN(p_src[(2 * i +           \
				    1) * src_stride - pred_index]) +           \
				    GET_CHAN(p_src[(2 * i +                    \
				    1) * src_stride]));                        \
				res =                                          \
				    (BC_CF * (st1 + st2) - st0 -               \
				    st3) BC_NRM2;                              \
				st0 = st2;                                     \
				st1 = st3;                                     \
				dst_color[i * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
		    }                                                          \
		    if (channels == 4)                                         \
			mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4         \
			    (dst_color + top * 4,                              \
			    (TypeP *) dst_color + top,                         \
			    dst_height - bottom - top, colormap);              \
		    else                                                       \
			mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3         \
			    (dst_color + top * 3,                              \
			    (TypeP *) dst_color + top,                         \
			    dst_height - bottom - top, colormap);              \
		    for (i = top; i < (dst_height - bottom); i++)              \
			p_dst[i * dst_stride] =                                \
			    ((TypeP *) dst_color)[i];                          \
		    right = 0;                                                 \
/*                                                                             \
 * end addition                                                                \
 */                                                                            \
		} else {                                                       \
		    if ((dw & 1) == 0) {                                       \
/*                                                                             \
 * dw == 0 or 2                                                                \
 */                                                                            \
			mlib_s32 pred_index = (dw == 0);                       \
	                                                                       \
			if (dh & 1) {                                          \
/*                                                                             \
 * dh == 1 or 3                                                                \
 */                                                                            \
			    for (l = 0; l < channels; l++) {                   \
				TypeC *src_color = lut[l] - offset;            \
	                                                                       \
				for (i = top; i < (dst_height - bottom);       \
				    i++) {                                     \
				    res =                                      \
					(BC_CD * GET_CHAN(p_src[2 * i *        \
					src_stride - pred_index]) +            \
					BC_CF * GET_CHAN(p_src[2 * i *         \
					src_stride]) -                         \
					GET_CHAN(p_src[2 * i *                 \
					src_stride + 1])) BC_NRML;             \
				    dst_color[i * channels + l] =              \
					SAT_##TPC(res);                        \
				}                                              \
			    }                                                  \
			    if (channels == 4)                                 \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4 \
				    (dst_color + top * 4,                      \
				    (TypeP *) dst_color + top,                 \
				    dst_height - bottom - top,                 \
				    colormap);                                 \
			    else                                               \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3 \
				    (dst_color + top * 3,                      \
				    (TypeP *) dst_color + top,                 \
				    dst_height - bottom - top,                 \
				    colormap);                                 \
			    for (i = top; i < (dst_height - bottom); i++)      \
				p_dst[i * dst_stride] =                        \
				    ((TypeP *) dst_color)[i];                  \
			} else {                                               \
/*                                                                             \
 * dh == 0 or 2                                                                \
 */                                                                            \
			    for (l = 0; l < channels; l++) {                   \
				TypeC *src_color = lut[l] - offset;            \
				mlib_s32 st0 =                                 \
				    BC_CD * GET_CHAN(p_src[(2 * top -          \
				    2) * src_stride - pred_index]) +           \
				    BC_CF * GET_CHAN(p_src[(2 * top -          \
				    2) * src_stride]) -                        \
				    GET_CHAN(p_src[(2 * top -                  \
				    2) * src_stride + 1]), st1 =               \
				    BC_CD * GET_CHAN(p_src[(2 * top -          \
				    1) * src_stride - pred_index]) +           \
				    BC_CF * GET_CHAN(p_src[(2 * top -          \
				    1) * src_stride]) -                        \
				    GET_CHAN(p_src[(2 * top -                  \
				    1) * src_stride + 1]), st2, st3;           \
				for (i = top; i < (dst_height - bottom); i++) {\
				    st2 =                                      \
					BC_CD * GET_CHAN(p_src[2 * i *         \
					src_stride - pred_index]) +            \
					BC_CF * GET_CHAN(p_src[2 * i *         \
					src_stride]) -                         \
					GET_CHAN(p_src[2 * i *                 \
					src_stride + 1]);                      \
				    st3 =                                      \
					BC_CD * GET_CHAN(p_src[(2 * i +        \
					1) * src_stride - pred_index]) +       \
					BC_CF * GET_CHAN(p_src[(2 * i +        \
					1) * src_stride]) -                    \
					GET_CHAN(p_src[(2 * i +                \
					1) * src_stride + 1]);                 \
				    res =                                      \
					(BC_CF * (st1 + st2) - st0 -           \
					st3) BC_NRM2;                          \
				    st0 = st2;                                 \
				    st1 = st3;                                 \
				    dst_color[i * channels + l] =              \
					SAT_##TPC(res);                        \
				}                                              \
			    }                                                  \
			    if (channels == 4)                                 \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4 \
				    (dst_color + top * 4,                      \
				    (TypeP *) dst_color + top,                 \
				    dst_height - bottom - top,                 \
				    colormap);                                 \
			    else                                               \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3 \
				    (dst_color + top * 3,                      \
				    (TypeP *) dst_color + top,                 \
				    dst_height - bottom - top,                 \
				    colormap);                                 \
			    for (i = top; i < (dst_height - bottom);           \
				i++)                                           \
				p_dst[i * dst_stride] =                        \
				    ((TypeP *) dst_color)[i];                  \
			}                                                      \
		    } else {                                                   \
/*                                                                             \
 * dw == 1 or 3                                                                \
 */                                                                            \
			if (dh & 1)                                            \
			    for (i = top; i < (dst_height - bottom); i++)      \
				p_dst[i * dst_stride] =                        \
				    p_src[2 * i * src_stride];                 \
			else {                                                 \
/*                                                                             \
 * dh == 0 or 2                                                                \
 */                                                                            \
			    for (l = 0; l < channels; l++) {                   \
				TypeC *src_color = lut[l] - offset;            \
	                                                                       \
				for (i = top; i < (dst_height - bottom);       \
				    i++) {                                     \
				    res =                                      \
					(BC_CF * (GET_CHAN(p_src[2 * i *       \
					src_stride]) +                         \
					GET_CHAN(p_src[(2 * i -                \
					1) * src_stride])) -                   \
					GET_CHAN(p_src[(2 * i -                \
					2) * src_stride]) -                    \
					GET_CHAN(p_src[(2 * i +                \
					1) * src_stride])) BC_NRML;            \
				    dst_color[i * channels + l] =              \
					SAT_##TPC(res);                        \
				}                                              \
			    }                                                  \
			    if (channels == 4)                                 \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4 \
				    (dst_color + top * 4,                      \
				    (TypeP *) dst_color + top,                 \
				    dst_height - bottom - top,                 \
				    colormap);                                 \
			    else                                               \
				mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3 \
				    (dst_color + top * 3,                      \
				    (TypeP *) dst_color + top,                 \
				    dst_height - bottom - top,                 \
				    colormap);                                 \
			    for (i = top; i < (dst_height - bottom);           \
				i++)                                           \
				p_dst[i * dst_stride] =                        \
				    ((TypeP *) dst_color)[i];                  \
			}                                                      \
		    }                                                          \
		}                                                              \
	    }                                                                  \
	    if (right) {                                                       \
		if ((dw & 1) == 0) {                                           \
/*                                                                             \
 * dw == 0 or 2                                                                \
 */                                                                            \
		    if (dh & 1) {                                              \
/*                                                                             \
 * dh == 1 or 3                                                                \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
	                                                                       \
			    for (i = top; i < (dst_height - bottom); i++) {    \
				res =                                          \
				    (BC_CF * GET_CHAN(p_src[2 * i *            \
				    src_stride + 2 * (dst_width - 1) -         \
				    1]) +                                      \
				    BC_CD * GET_CHAN(p_src[2 * i *             \
				    src_stride + 2 * (dst_width - 1)]) -       \
				    GET_CHAN(p_src[2 * i * src_stride +        \
				    2 * (dst_width - 1) - 2])) BC_NRML;        \
				dst_color[i * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color + top * 4,                          \
				(TypeP *) dst_color + top,                     \
				dst_height - bottom - top, colormap);          \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color + top * 3,                          \
				(TypeP *) dst_color + top,                     \
				dst_height - bottom - top, colormap);          \
			for (i = top; i < (dst_height - bottom); i++)          \
			    p_dst[i * dst_stride + dst_width - 1] =            \
				((TypeP *) dst_color)[i];                      \
		    } else {                                                   \
/*                                                                             \
 * dh == 0 or 2                                                                \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
			    mlib_s32 st0 =                                     \
				BC_CF * GET_CHAN(p_src[(2 * top -              \
				2) * src_stride + 2 * (dst_width - 1) -        \
				1]) + BC_CD * GET_CHAN(p_src[(2 * top -        \
				2) * src_stride + 2 * (dst_width -             \
				1)]) - GET_CHAN(p_src[(2 * top -               \
				2) * src_stride + 2 * (dst_width - 1) -        \
				2]), st1 =                                     \
				BC_CF * GET_CHAN(p_src[(2 * top -              \
				1) * src_stride + 2 * (dst_width - 1) -        \
				1]) + BC_CD * GET_CHAN(p_src[(2 * top -        \
				1) * src_stride + 2 * (dst_width -             \
				1)]) - GET_CHAN(p_src[(2 * top -               \
				1) * src_stride + 2 * (dst_width - 1) -        \
				2]), st2, st3;                                 \
			    for (i = top; i < (dst_height - bottom);           \
				i++) {                                         \
				st2 =                                          \
				    BC_CF * GET_CHAN(p_src[2 * i *             \
				    src_stride + 2 * (dst_width - 1) -         \
				    1]) +                                      \
				    BC_CD * GET_CHAN(p_src[2 * i *             \
				    src_stride + 2 * (dst_width - 1)]) -       \
				    GET_CHAN(p_src[2 * i * src_stride +        \
				    2 * (dst_width - 1) - 2]);                 \
				st3 =                                          \
				    BC_CF * GET_CHAN(p_src[(2 * i +            \
				    1) * src_stride + 2 * (dst_width -         \
				    1) - 1]) +                                 \
				    BC_CD * GET_CHAN(p_src[(2 * i +            \
				    1) * src_stride + 2 * (dst_width -         \
				    1)]) - GET_CHAN(p_src[(2 * i +             \
				    1) * src_stride + 2 * (dst_width -         \
				    1) - 2]);                                  \
				res =                                          \
				    (BC_CF * (st1 + st2) - st0 -               \
				    st3) BC_NRM2;                              \
				st0 = st2;                                     \
				st1 = st3;                                     \
				dst_color[i * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color + top * 4,                          \
				(TypeP *) dst_color + top,                     \
				dst_height - bottom - top, colormap);          \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color + top * 3,                          \
				(TypeP *) dst_color + top,                     \
				dst_height - bottom - top, colormap);          \
			for (i = top; i < (dst_height - bottom); i++)          \
			    p_dst[i * dst_stride + dst_width - 1] =            \
				((TypeP *) dst_color)[i];                      \
		    }                                                          \
		} else {                                                       \
/*                                                                             \
 * dw == 1 or 3                                                                \
 */                                                                            \
		    if (dh & 1)                                                \
			for (i = top; i < (dst_height - bottom); i++)          \
			    p_dst[i * dst_stride + dst_width - 1] =            \
				p_src[2 * i * src_stride +                     \
				2 * (dst_width - 1)];                          \
		    else {                                                     \
/*                                                                             \
 * dh == 0 or 2                                                                \
 */                                                                            \
			for (l = 0; l < channels; l++) {                       \
			    TypeC *src_color = lut[l] - offset;                \
	                                                                       \
			    for (i = top; i < (dst_height - bottom); i++) {    \
				res =                                          \
				    (BC_CF * (GET_CHAN(p_src[2 * i *           \
				    src_stride + 2 * (dst_width - 1)]) +       \
				    GET_CHAN(p_src[(2 * i -                    \
				    1) * src_stride + 2 * (dst_width -         \
				    1)])) - GET_CHAN(p_src[(2 * i -            \
				    2) * src_stride + 2 * (dst_width -         \
				    1)]) - GET_CHAN(p_src[(2 * i +             \
				    1) * src_stride + 2 * (dst_width -         \
				    1)])) BC_NRML;                             \
				dst_color[i * channels + l] =                  \
				    SAT_##TPC(res);                            \
			    }                                                  \
			}                                                      \
			if (channels == 4)                                     \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_4     \
				(dst_color + top * 4,                          \
				(TypeP *) dst_color + top,                     \
				dst_height - bottom - top, colormap);          \
			else                                                   \
			    mlib_ImageColorTrue2IndexLine_##TPC##_##TP##_3     \
				(dst_color + top * 3,                          \
				(TypeP *) dst_color + top,                     \
				dst_height - bottom - top, colormap);          \
			for (i = top; i < (dst_height - bottom); i++)          \
			    p_dst[i * dst_stride + dst_width - 1] =            \
				((TypeP *) dst_color)[i];                      \
		    }                                                          \
		}                                                              \
	    }                                                                  \
	    FREE_MEM;                                                          \
	}

/* *********************************************************** */

#define	BC_CF	9
#define	BC_CD	8

#define	BC_NRML	>>4

#define	BC_NRM2	>>8

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2XIndex_U8_Bicubic_Edge(
    EDGE_INDEX_PARAMETERS)
{
	EDGE_INDEX_BICUBIC(mlib_u8);

	l = (mlib_s32)edge;
	switch (l) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_INDEX_FILL_ZERO;
		break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_INDEX_OP_NEAREST;
		break;
	case MLIB_EDGE_SRC_EXTEND:

		if (type_colormap == MLIB_BYTE)
			CASE_EDGE_INDEX_BICUBIC_EXTEND(U8, mlib_u8,
			    U8,
			    mlib_u8)
		else
			CASE_EDGE_INDEX_BICUBIC_EXTEND(U8, mlib_u8,
			    S16,
			    mlib_s16)
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2XIndex_S16_Bicubic_Edge(
    EDGE_INDEX_PARAMETERS)
{
	EDGE_INDEX_BICUBIC(mlib_s16);

	l = (mlib_s32)edge;
	switch (l) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_INDEX_FILL_ZERO;
		break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_INDEX_OP_NEAREST;
		break;
	case MLIB_EDGE_SRC_EXTEND:

		if (type_colormap == MLIB_BYTE)
			CASE_EDGE_INDEX_BICUBIC_EXTEND(S16, mlib_s16,
			    U8,
			    mlib_u8)
		else
			CASE_EDGE_INDEX_BICUBIC_EXTEND(S16, mlib_s16,
			    S16,
			    mlib_s16)
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

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
mlib_ImageZoomOut2XIndex_U8_Bicubic2_Edge(
    EDGE_INDEX_PARAMETERS)
{
	EDGE_INDEX_BICUBIC(mlib_u8);

	l = (mlib_s32)edge;
	switch (l) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_INDEX_FILL_ZERO;
		break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_INDEX_OP_NEAREST;
		break;
	case MLIB_EDGE_SRC_EXTEND:

		if (type_colormap == MLIB_BYTE)
			CASE_EDGE_INDEX_BICUBIC_EXTEND(U8, mlib_u8,
			    U8,
			    mlib_u8)
		else
			    CASE_EDGE_INDEX_BICUBIC_EXTEND(U8, mlib_u8,
			    S16,
			    mlib_s16)
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2XIndex_S16_Bicubic2_Edge(
    EDGE_INDEX_PARAMETERS)
{
	EDGE_INDEX_BICUBIC(mlib_s16);

	l = (mlib_s32)edge;
	switch (l) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_INDEX_FILL_ZERO;
		break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_INDEX_OP_NEAREST;
		break;
	case MLIB_EDGE_SRC_EXTEND:

		if (type_colormap == MLIB_BYTE)
			CASE_EDGE_INDEX_BICUBIC_EXTEND(S16, mlib_s16,
			    U8,
			    mlib_u8)
		else
			CASE_EDGE_INDEX_BICUBIC_EXTEND(S16, mlib_s16,
			    S16,
			    mlib_s16)
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
