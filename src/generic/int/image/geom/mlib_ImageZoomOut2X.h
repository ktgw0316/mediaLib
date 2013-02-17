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

#ifndef _MLIB_IMAGEZOOMOUT2X_H
#define	_MLIB_IMAGEZOOMOUT2X_H

#pragma ident	"@(#)mlib_ImageZoomOut2X.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#define	NEAREST_PARAMETERS(type)                                       \
	type *p_src, type * p_dst, mlib_s32 src_stride,                \
	mlib_s32 dst_stride, mlib_s32 dst_height, mlib_s32 dst_width

/* *********************************************************** */

#define	BILINEAR_PARAMETERS(type)                               \
	type *p_src, type * p_dst, mlib_s32 src_stride,         \
	mlib_s32 dst_stride, mlib_s32 dst_height,               \
	mlib_s32 dst_width, mlib_s32 dh, mlib_s32 dw

/* *********************************************************** */

#define	BC_PARAMETERS(type)                                         \
	type *p_src, type * p_dst, mlib_s32 slb, mlib_s32 dlb,      \
	mlib_s32 height, mlib_s32 width, mlib_s32 dh, mlib_s32 dw

/* *********************************************************** */

#define	EDGE_PARAMETERS                                          \
	const mlib_image *src, mlib_image *dst, mlib_edge edge

/* *********************************************************** */

#define	NEAREST_P                                               \
	(void *)p_src, (void *)p_dst, src_stride, dst_stride,   \
	dst_height, dst_width

/* *********************************************************** */

#define	BILINEAR_P	NEAREST_P, dh, dw

/* *********************************************************** */

#define	EDGE_P	src, dst, edge

/* *********************************************************** */

#define	BOUND_NEAREST()                                            \
	if (edge == MLIB_EDGE_SRC_PADDED) {                        \
	    BOUND_PADDING(0);                                      \
	    if ((src_height & 1) == 0)                             \
		p_src += src_stride;                               \
	    if ((src_width & 1) == 0)                              \
		p_src += bchan;                                    \
	} else {                                                   \
	    mlib_s32 src_w_beg, src_h_beg;                         \
	                                                           \
	    src_h_beg = src_height - 2 * dst_height;               \
	    if (src_h_beg <= 0) {                                  \
		mlib_s32 dst_h_beg = (1 - src_h_beg) >> 2;         \
	                                                           \
		p_dst += dst_h_beg * dst_stride;                   \
		dst_height -= 2 * dst_h_beg;                       \
		src_h_beg &= 3;                                    \
		dst_height -= (src_h_beg == 2);                    \
		src_h_beg = (src_h_beg < 2);                       \
	    } else                                                 \
		src_h_beg = (src_h_beg + 2) >> 1;                  \
	    src_w_beg = src_width - 2 * dst_width;                 \
	    if (src_w_beg <= 0) {                                  \
		mlib_s32 dst_w_beg = (1 - src_w_beg) >> 2;         \
	                                                           \
		p_dst += dst_w_beg * bchan;                        \
		dst_width -= 2 * dst_w_beg;                        \
		src_w_beg &= 3;                                    \
		dst_width -= (src_w_beg == 2);                     \
		src_w_beg = (src_w_beg < 2);                       \
	    } else                                                 \
		src_w_beg = (src_w_beg + 2) >> 1;                  \
	    p_src += src_h_beg * src_stride + src_w_beg * bchan;   \
	}

/* *********************************************************** */

#define	BOUND_BILINEAR()                                        \
	if (edge == MLIB_EDGE_SRC_PADDED) {                     \
	    BOUND_PADDING(1);                                   \
	} else {                                                \
	    mlib_s32 src_w_beg, src_h_beg;                      \
	                                                        \
	    src_h_beg = src_height - 2 * dst_height;            \
	    dh = src_h_beg & 1;                                 \
	    if (src_h_beg < 0) {                                \
		mlib_s32 dst_h_beg = (2 - src_h_beg) >> 2;      \
	                                                        \
		p_dst += dst_h_beg * dst_stride;                \
		dst_height -= 2 * dst_h_beg;                    \
		src_h_beg = (-src_h_beg) & 3;                   \
		dst_height -= (src_h_beg == 1);                 \
	    } else                                              \
		src_h_beg++;                                    \
	    p_src += (src_h_beg >> 1) * src_stride;             \
	    src_w_beg = src_width - 2 * dst_width;              \
	    dw = src_w_beg & 1;                                 \
	    if (src_w_beg < 0) {                                \
		mlib_s32 dst_w_beg = (2 - src_w_beg) >> 2;      \
	                                                        \
		p_dst += dst_w_beg * bchan;                     \
		dst_width -= 2 * dst_w_beg;                     \
		src_w_beg = (-src_w_beg) & 3;                   \
		dst_width -= (src_w_beg == 1);                  \
	    } else                                              \
		src_w_beg++;                                    \
	    p_src += (src_w_beg >> 1) * bchan;                  \
	}

/* *********************************************************** */

#define	BOUND_BICUBIC()                                            \
	if (edge == MLIB_EDGE_SRC_PADDED) {                        \
	    BOUND_PADDING(3);                                      \
	} else {                                                   \
	    mlib_s32 src_w_beg, dst_w_beg;                         \
	    mlib_s32 src_h_beg, dst_h_beg;                         \
	                                                           \
	    src_h_beg = src_height - 2 * dst_height;               \
	    dh = src_h_beg & 1;                                    \
	    if (src_h_beg <= 1) {                                  \
		src_h_beg = 4 - src_h_beg;                         \
		dst_h_beg = (src_h_beg >> 2);                      \
		p_dst += dst_h_beg * dst_stride;                   \
		src_h_beg &= 3;                                    \
		dst_height -= 2 * dst_h_beg + (src_h_beg == 3);    \
		src_h_beg = 2 - (src_h_beg >> 1);                  \
	    } else                                                 \
		src_h_beg = (src_h_beg + 1) >> 1;                  \
	    src_w_beg = src_width - 2 * dst_width;                 \
	    dw = src_w_beg & 1;                                    \
	    if (src_w_beg <= 1) {                                  \
		src_w_beg = 4 - src_w_beg;                         \
		dst_w_beg = (src_w_beg >> 2);                      \
		p_dst += dst_w_beg * bchan;                        \
		src_w_beg &= 3;                                    \
		dst_width -= 2 * dst_w_beg + (src_w_beg == 3);     \
		src_w_beg = 2 - (src_w_beg >> 1);                  \
	    } else                                                 \
		src_w_beg = (src_w_beg + 1) >> 1;                  \
	    p_src += src_h_beg * src_stride + src_w_beg * bchan;   \
	}

/* *********************************************************** */

#define	BOUND_PADDING(DEL)                                                 \
	mlib_s32 sxb, syb, sxe, sye, dpe;                                  \
	mlib_s32 dpd_x, dpd_y, dps_x, dps_y, t_w_d, t_h_d;                 \
	mlib_s32 dw2 = dst_width << 1, dh2 = dst_height << 1;              \
	mlib_u8 *paddings;                                                 \
	                                                                   \
	paddings = mlib_ImageGetPaddings(src);                             \
	dh = src_height & 1;                                               \
	dw = src_width & 1;                                                \
	sxb = (paddings[0] << 1) - src_width;                              \
	sxe = src_width - (paddings[2] << 1);                              \
	syb = (paddings[1] << 1) - src_height;                             \
	sye = src_height - (paddings[3] << 1);                             \
	if (sxb - DEL < -src_width)                                        \
	    sxb = -src_width + DEL;                                        \
	if (sxe + DEL > src_width)                                         \
	    sxe = src_width - DEL;                                         \
	if (dw2 >= sxb && -dw2 < sxe) {                                    \
	    if (-dw2 < sxb)                                                \
		dpd_x = (dst_width + ((sxb + 1) >> 1)) >> 1;               \
	    else                                                           \
		dpd_x = 0;                                                 \
	    dps_x = ((src_width + 1) >> 1) + (dpd_x << 1) - dst_width;     \
	    if (dw2 > sxe)                                                 \
		dpe = (sxe + 1) >> 1;                                      \
	    else                                                           \
		dpe = dst_width + 1;                                       \
	    t_w_d = ((dst_width + dpe) >> 1) - dpd_x;                      \
	} else {                                                           \
	    return (MLIB_SUCCESS);                                         \
	}                                                                  \
	                                                                   \
	if (syb - DEL < -src_height)                                       \
	    syb = -src_height + DEL;                                       \
	if (sye + DEL > src_height)                                        \
	    sye = src_height - DEL;                                        \
	if (dh2 >= syb && -dh2 < sye) {                                    \
	    if (-dh2 < syb)                                                \
		dpd_y = (dst_height + ((syb + 1) >> 1)) >> 1;              \
	    else                                                           \
		dpd_y = 0;                                                 \
	    dps_y = ((src_height + 1) >> 1) + (dpd_y << 1) - dst_height;   \
	    if (dh2 > sye)                                                 \
		dpe = (sye + 1) >> 1;                                      \
	    else                                                           \
		dpe = dst_height + 1;                                      \
	    t_h_d = ((dst_height + dpe) >> 1) - dpd_y;                     \
	} else {                                                           \
	    return (MLIB_SUCCESS);                                         \
	}                                                                  \
	                                                                   \
	p_src += dps_y * src_stride + dps_x * bchan;                       \
	p_dst += dpd_y * dst_stride + dpd_x * bchan;                       \
	dst_width = t_w_d;                                                 \
	dst_height = t_h_d

/* *********************************************************** */

#define	MASK_U8_INIT

/* *********************************************************** */

#define	MASK_S16_INIT	mlib_u32 mask

/* *********************************************************** */

#define	MASK_U16_INIT	mlib_u32 mask

/* *********************************************************** */

#define	ADD_S32_SAT(dst, a, b)                                  \
	{                                                       \
	    mlib_d64 sat_tmp = (a + (mlib_d64)b);               \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dst = (mlib_s32)sat_tmp;                            \
	}

/* *********************************************************** */

#define	SAT_U8(x)	mlib_satU8[x]

/* *********************************************************** */

#define	SAT_S16(x)                                              \
	((mask = *((mlib_u32 *)((mlib_u8 *)mlib_mask_s16 +      \
	    (((x) >> 13) & 0xc)))), ((x) & mask) | (mask >> 16))

/* *********************************************************** */

#define	SAT_U16(x)                                              \
	((mask = *((mlib_u32 *)((mlib_u8 *)mlib_mask_u16 +      \
	    (((x) >> 14) & 0xc)))), ((x) & mask) | (mask >> 16))

/* *********************************************************** */

#define	SAT_S32(src)                                            \
	((src > MLIB_S32_MAX) ? MLIB_S32_MAX : ((src <          \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)src))

#define	SAT_F32(src)	(src)

#define	SAT_D64(src)	(src)

/* *********************************************************** */

#define	STORE_U8(res, x)	res = mlib_satU8[x]

/* *********************************************************** */

#define	STORE_S16(res, x)                                       \
	mask =                                                  \
	    *((mlib_u32 *)((mlib_u8 *)mlib_mask_s16 +           \
	    (((x) >> 13) & 0xC)));                              \
	res = ((x) & mask) | (mask >> 16)

/* *********************************************************** */

#define	STORE_U16(res, x)                                       \
	mask =                                                  \
	    *((mlib_u32 *)((mlib_u8 *)mlib_mask_u16 +           \
	    (((x) >> 14) & 0xC)));                              \
	res = ((x) & mask) | (mask >> 16)

/* *********************************************************** */

#define	STORE_S32(res, x)                                       \
	if (x >= MLIB_S32_MAX)                                  \
	    x = MLIB_S32_MAX;                                   \
	if (x <= MLIB_S32_MIN)                                  \
	    x = MLIB_S32_MIN;                                   \
	res = (mlib_s32)(x)

/* *********************************************************** */

#define	STORE_FP(res, x)	res = (x)

/* *********************************************************** */

#define	INIT_COMMON_FILTER_DATA(type)                                     \
	type *p_src = (type *) mlib_ImageGetData(src);                    \
	type *p_dst = (type *) mlib_ImageGetData(dst);                    \
	mlib_s32 src_stride = mlib_ImageGetStride(src) / sizeof (type);   \
	mlib_s32 dst_stride = mlib_ImageGetStride(dst) / sizeof (type);   \
	mlib_s32 src_height = mlib_ImageGetHeight(src);                   \
	mlib_s32 dst_height = mlib_ImageGetHeight(dst);                   \
	mlib_s32 src_width = mlib_ImageGetWidth(src);                     \
	mlib_s32 dst_width = mlib_ImageGetWidth(dst);                     \
	mlib_s32 chan = mlib_ImageGetChannels(src)

/* *********************************************************** */

#define	EDGE_BILINEAR(type)                                       \
	INIT_COMMON_FILTER_DATA(type);                            \
	mlib_s32 i, j, l, dh, dw;                                 \
	mlib_s32 left = 0, right = 0, top = 0, bottom = 0; {      \
	    mlib_s32 src_w_beg, src_h_beg;                        \
	                                                          \
	    src_h_beg = src_height - 2 * dst_height;              \
	    dh = src_h_beg & 1;                                   \
	    if (src_h_beg <= 0) {                                 \
		mlib_s32 dst_h_beg = (1 - src_h_beg) >> 2;        \
	                                                          \
		p_dst += dst_h_beg * dst_stride;                  \
		dst_height -= 2 * dst_h_beg;                      \
		src_h_beg &= 3;                                   \
		dst_height -= (top = (src_h_beg == 2));           \
		bottom = (src_h_beg == 3);                        \
		src_h_beg = (src_h_beg < 2);                      \
	    } else                                                \
		src_h_beg = (src_h_beg + 2) >> 1;                 \
	    src_w_beg = src_width - 2 * dst_width;                \
	    dw = src_w_beg & 1;                                   \
	    if (src_w_beg <= 0) {                                 \
		mlib_s32 dst_w_beg = (1 - src_w_beg) >> 2;        \
	                                                          \
		p_dst += dst_w_beg * chan;                        \
		dst_width -= 2 * dst_w_beg;                       \
		src_w_beg &= 3;                                   \
		dst_width -= (left = (src_w_beg == 2));           \
		right = (src_w_beg == 3);                         \
		src_w_beg = (src_w_beg < 2);                      \
	    } else                                                \
		src_w_beg = (src_w_beg + 2) >> 1;                 \
	    p_src += src_h_beg * src_stride + src_w_beg * chan;   \
	}

/* *********************************************************** */

#define	EDGE_BICUBIC(type)                                              \
	INIT_COMMON_FILTER_DATA(type);                                  \
	mlib_s32 i, j, l, dh, dw;                                       \
	mlib_s32 exist_w, left = 0, right = 0, top = 0, bottom = 0; {   \
	    mlib_s32 src_w_beg, src_h_beg;                              \
	                                                                \
	    src_h_beg = src_height - 2 * dst_height;                    \
	    dh = src_h_beg & 3;                                         \
	    if (src_h_beg <= 1) {                                       \
		mlib_s32 dst_h_beg = (1 - src_h_beg) >> 2;              \
	                                                                \
		p_dst += dst_h_beg * dst_stride;                        \
		dst_height -= 2 * dst_h_beg;                            \
		dst_height -= 1 - (bottom = (dh != 2));                 \
		top = (dh != 1);                                        \
		src_h_beg = (dh < 2);                                   \
	    } else                                                      \
		src_h_beg = (src_h_beg + 2) >> 1;                       \
	    src_w_beg = src_width - 2 * dst_width;                      \
	    dw = src_w_beg & 3;                                         \
	    if (src_w_beg <= 1) {                                       \
		mlib_s32 dst_w_beg = (1 - src_w_beg) >> 2;              \
	                                                                \
		p_dst += dst_w_beg * chan;                              \
		dst_width -= 2 * dst_w_beg;                             \
		dst_width -= 1 - (right = (dw != 2));                   \
		left = (dw != 1);                                       \
		src_w_beg = (dw < 2);                                   \
	    } else                                                      \
		src_w_beg = (src_w_beg + 2) >> 1;                       \
	    exist_w = src_w_beg > 1;                                    \
	    p_src += src_h_beg * src_stride + src_w_beg * chan;         \
	    if (dst_width <= 0 || dst_height <= 0)                      \
		return (MLIB_SUCCESS);                                  \
	}

/* *********************************************************** */

#define	SWITCH_BC_CHANNELS(TP, FT)                                      \
	if (dst_width > 0 && dst_height > 0) {                          \
	    if (dh && dw) {                                             \
		mlib_ImageZoomOut2X_Nearest(NEAREST_P, tshift, chan);   \
	    } else {                                                    \
		switch (chan) {                                         \
		case 1:                                                 \
		    mlib_ImageZoomOut2X_##TP##_1_##FT                   \
			(BILINEAR_P);                                   \
		    break;                                              \
		case 2:                                                 \
		    mlib_ImageZoomOut2X_##TP##_2_##FT                   \
			(BILINEAR_P);                                   \
		    break;                                              \
		case 3:                                                 \
		    mlib_ImageZoomOut2X_##TP##_3_##FT                   \
			(BILINEAR_P);                                   \
		    break;                                              \
		case 4:                                                 \
		    mlib_ImageZoomOut2X_##TP##_4_##FT                   \
			(BILINEAR_P);                                   \
		    break;                                              \
		}                                                       \
	    }                                                           \
	}                                                               \
	if (edge == MLIB_EDGE_DST_NO_WRITE ||                           \
	    edge == MLIB_EDGE_SRC_PADDED) {                             \
	    return (MLIB_SUCCESS);                                      \
	} else {                                                        \
	    return                                                      \
		mlib_ImageZoomOut2X_##TP##_##FT##_Edge                  \
		(EDGE_P);                                               \
	}

/* *********************************************************** */

#define	SWITCH_BL_CHANNELS(TP)                                          \
	if (dst_width > 0 && dst_height > 0) {                          \
	    if (dh && dw) {                                             \
		mlib_ImageZoomOut2X_Nearest(NEAREST_P, tshift, chan);   \
	    } else {                                                    \
		mlib_s32 res = 0;                                       \
	                                                                \
		switch (chan) {                                         \
		case 1:                                                 \
		    res =                                               \
			mlib_ImageZoomOut2X_##TP##_1_Bilinear           \
			(BILINEAR_P);                                   \
		    break;                                              \
		case 2:                                                 \
		    res =                                               \
			mlib_ImageZoomOut2X_##TP##_2_Bilinear           \
			(BILINEAR_P);                                   \
		    break;                                              \
		case 3:                                                 \
		    res =                                               \
			mlib_ImageZoomOut2X_##TP##_3_Bilinear           \
			(BILINEAR_P);                                   \
		    break;                                              \
		case 4:                                                 \
		    res =                                               \
			mlib_ImageZoomOut2X_##TP##_4_Bilinear           \
			(BILINEAR_P);                                   \
		    break;                                              \
		}                                                       \
		if (res != MLIB_SUCCESS)                                \
		    return (res);                                       \
	    }                                                           \
	}                                                               \
	if (edge == MLIB_EDGE_DST_NO_WRITE ||                           \
	    edge == MLIB_EDGE_SRC_PADDED) {                             \
	    return (MLIB_SUCCESS);                                      \
	} else {                                                        \
	    return                                                      \
		mlib_ImageZoomOut2X_##TP##_Bilinear_Edge                \
		(EDGE_P);                                               \
	}

/* *********************************************************** */

/* Function prototypes */
void mlib_ImageZoomOut2X_U8_1_Nearest(
    NEAREST_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_2_Nearest(
    NEAREST_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_3_Nearest(
    NEAREST_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_4_Nearest(
    NEAREST_PARAMETERS(mlib_u8));

void mlib_ImageZoomOut2X_S16_1_Nearest(
    NEAREST_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_2_Nearest(
    NEAREST_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_3_Nearest(
    NEAREST_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_4_Nearest(
    NEAREST_PARAMETERS(mlib_s16));

void mlib_ImageZoomOut2X_S32_1_Nearest(
    NEAREST_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_2_Nearest(
    NEAREST_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_3_Nearest(
    NEAREST_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_4_Nearest(
    NEAREST_PARAMETERS(mlib_s32));

void mlib_ImageZoomOut2X_D64_1_Nearest(
    NEAREST_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_2_Nearest(
    NEAREST_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_3_Nearest(
    NEAREST_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_4_Nearest(
    NEAREST_PARAMETERS(mlib_d64));

mlib_status mlib_ImageZoomOut2X_U8_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8));
mlib_status mlib_ImageZoomOut2X_U8_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8));
mlib_status mlib_ImageZoomOut2X_U8_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8));
mlib_status mlib_ImageZoomOut2X_U8_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_u8));

mlib_status mlib_ImageZoomOut2X_S16_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16));
mlib_status mlib_ImageZoomOut2X_S16_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16));
mlib_status mlib_ImageZoomOut2X_S16_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16));
mlib_status mlib_ImageZoomOut2X_S16_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_s16));

mlib_status mlib_ImageZoomOut2X_U16_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16));
mlib_status mlib_ImageZoomOut2X_U16_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16));
mlib_status mlib_ImageZoomOut2X_U16_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16));
mlib_status mlib_ImageZoomOut2X_U16_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_u16));

mlib_status mlib_ImageZoomOut2X_S32_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_s32));
mlib_status mlib_ImageZoomOut2X_S32_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_s32));
mlib_status mlib_ImageZoomOut2X_S32_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_s32));
mlib_status mlib_ImageZoomOut2X_S32_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_s32));

void mlib_ImageZoomOut2X_U8_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8));

void mlib_ImageZoomOut2X_S16_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16));

void mlib_ImageZoomOut2X_U16_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_u16));
void mlib_ImageZoomOut2X_U16_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_u16));
void mlib_ImageZoomOut2X_U16_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_u16));
void mlib_ImageZoomOut2X_U16_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_u16));

void mlib_ImageZoomOut2X_S32_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_s32));

void mlib_ImageZoomOut2X_U8_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8));
void mlib_ImageZoomOut2X_U8_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8));

void mlib_ImageZoomOut2X_S16_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16));
void mlib_ImageZoomOut2X_S16_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16));

void mlib_ImageZoomOut2X_U16_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u16));
void mlib_ImageZoomOut2X_U16_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u16));
void mlib_ImageZoomOut2X_U16_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u16));
void mlib_ImageZoomOut2X_U16_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u16));

void mlib_ImageZoomOut2X_S32_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s32));
void mlib_ImageZoomOut2X_S32_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s32));

/*  Floating point functions  */
mlib_status mlib_ImageZoomOut2X_F32_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_f32));
mlib_status mlib_ImageZoomOut2X_F32_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_f32));
mlib_status mlib_ImageZoomOut2X_F32_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_f32));
mlib_status mlib_ImageZoomOut2X_F32_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_f32));

mlib_status mlib_ImageZoomOut2X_D64_1_Bilinear(
    BILINEAR_PARAMETERS(mlib_d64));
mlib_status mlib_ImageZoomOut2X_D64_2_Bilinear(
    BILINEAR_PARAMETERS(mlib_d64));
mlib_status mlib_ImageZoomOut2X_D64_3_Bilinear(
    BILINEAR_PARAMETERS(mlib_d64));
mlib_status mlib_ImageZoomOut2X_D64_4_Bilinear(
    BILINEAR_PARAMETERS(mlib_d64));

void mlib_ImageZoomOut2X_F32_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_f32));
void mlib_ImageZoomOut2X_F32_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_f32));
void mlib_ImageZoomOut2X_F32_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_f32));
void mlib_ImageZoomOut2X_F32_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_f32));

void mlib_ImageZoomOut2X_D64_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_d64));

void mlib_ImageZoomOut2X_F32_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_f32));
void mlib_ImageZoomOut2X_F32_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_f32));
void mlib_ImageZoomOut2X_F32_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_f32));
void mlib_ImageZoomOut2X_F32_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_f32));

void mlib_ImageZoomOut2X_D64_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_d64));
void mlib_ImageZoomOut2X_D64_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_d64));

/*   Edges functions    */
mlib_status mlib_ImageZoomOut2X_U8_Bilinear_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_S16_Bilinear_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_U16_Bilinear_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_S32_Bilinear_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_F32_Bilinear_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_D64_Bilinear_Edge(
    EDGE_PARAMETERS);

mlib_status mlib_ImageZoomOut2X_U8_Bicubic_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_S16_Bicubic_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_U16_Bicubic_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_S32_Bicubic_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_F32_Bicubic_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_D64_Bicubic_Edge(
    EDGE_PARAMETERS);

mlib_status mlib_ImageZoomOut2X_U8_Bicubic2_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_S16_Bicubic2_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_U16_Bicubic2_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_S32_Bicubic2_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_F32_Bicubic2_Edge(
    EDGE_PARAMETERS);
mlib_status mlib_ImageZoomOut2X_D64_Bicubic2_Edge(
    EDGE_PARAMETERS);

/* *********************************************************** */
/*		Index colormap Image type part			*/
/* *********************************************************** */

#define	BILINEAR_INDEX_PARAMETERS(type)                         \
	NEAREST_PARAMETERS(type), mlib_s32 dh, mlib_s32 dw,     \
	    const void *colormap

/* *********************************************************** */

#define	BILINEAR_INDEX_P                                                   \
	p_src, p_dst, src_stride, dst_stride, dst_height, dst_width, dh,   \
	dw, colormap

/* *********************************************************** */

#define	EDGE_INDEX_PARAMETERS                                     \
	const mlib_image *src, mlib_image *dst, mlib_edge edge,   \
	const void *colormap

#define	EDGE_INDEX_P	src, dst, edge, colormap

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	GET_CHAN_U8_0(color)	(color & 0xFF)
#define	GET_CHAN_U8_1(color)	((color >> 8) & 0xFF)
#define	GET_CHAN_U8_2(color)	((color >> 16) & 0xFF)
#define	GET_CHAN_U8_3(color)	(((mlib_u32) color) >> 24)

#else /* _LITTLE_ENDIAN */

#define	GET_CHAN_U8_0(color)	(((mlib_u32) color) >> 24)
#define	GET_CHAN_U8_1(color)	((color >> 16) & 0xFF)
#define	GET_CHAN_U8_2(color)	((color >> 8) & 0xFF)
#define	GET_CHAN_U8_3(color)	(color & 0xFF)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	AVE_U8_2(color0, color1)                                        \
	(((color0 >> 1) & 0x7f7f7f7f) + ((color1 >> 1) & 0x7f7f7f7f))

/* *********************************************************** */

#define	AVE_U8_4(color0, color1, color2, color3)                         \
	(((color0 >> 2) & 0x3f3f3f3f) + ((color1 >> 2) & 0x3f3f3f3f) +   \
	    ((color2 >> 2) & 0x3f3f3f3f) +                               \
	    ((color3 >> 2) & 0x3f3f3f3f) + 0x1010101)

/* *********************************************************** */

#define	STORE_U8_3(color)                                       \
	dst_color[3 * j + 2] = color;                           \
	dst_color[3 * j + 1] = (color >> 8);                    \
	dst_color[3 * j] = (color >> 16)

/* *********************************************************** */

#define	STORE_U8_4(color)	((mlib_s32 *)dst_color)[j] = color

#define	GET_CHAN(color)	src_color[color]

/* *********************************************************** */

#define	BOUND_INDEX_BILINEAR(type)                                        \
	mlib_type type_colormap = mlib_ImageGetLutType(colormap);         \
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);           \
	type *p_src = (type *) mlib_ImageGetData(src);                    \
	type *p_dst = (type *) mlib_ImageGetData(dst);                    \
	mlib_s32 src_stride = mlib_ImageGetStride(src) / sizeof (type);   \
	mlib_s32 dst_stride = mlib_ImageGetStride(dst) / sizeof (type);   \
	mlib_s32 src_height = mlib_ImageGetHeight(src);                   \
	mlib_s32 dst_height = mlib_ImageGetHeight(dst);                   \
	mlib_s32 src_width = mlib_ImageGetWidth(src);                     \
	mlib_s32 dst_width = mlib_ImageGetWidth(dst);                     \
	mlib_s32 chan = mlib_ImageGetChannels(src);                       \
	mlib_s32 dh = src_height & 1;                                     \
	mlib_s32 dw = src_width & 1;                                      \
	mlib_s32 bchan = chan;                                            \
	                                                                  \
	BOUND_BILINEAR()

/* *********************************************************** */

#define	BOUND_INDEX_BICUBIC(type)                                         \
	mlib_type type_colormap = mlib_ImageGetLutType(colormap);         \
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);           \
	type *p_src = (type *) mlib_ImageGetData(src);                    \
	type *p_dst = (type *) mlib_ImageGetData(dst);                    \
	mlib_s32 src_stride = mlib_ImageGetStride(src) / sizeof (type);   \
	mlib_s32 dst_stride = mlib_ImageGetStride(dst) / sizeof (type);   \
	mlib_s32 src_height = mlib_ImageGetHeight(src);                   \
	mlib_s32 dst_height = mlib_ImageGetHeight(dst);                   \
	mlib_s32 src_width = mlib_ImageGetWidth(src);                     \
	mlib_s32 dst_width = mlib_ImageGetWidth(dst);                     \
	mlib_s32 chan = mlib_ImageGetChannels(src);                       \
	mlib_s32 dh = src_height & 1;                                     \
	mlib_s32 dw = src_width & 1;                                      \
	mlib_s32 bchan = chan;                                            \
	                                                                  \
	BOUND_BICUBIC()

/* *********************************************************** */

#define	SWITCH_INDEX_BIxxx_CHANNELS(LG, TP, FT)                           \
	if (dst_width > 0 && dst_height > 0) {                            \
	    if (dh && dw) {                                               \
		mlib_ImageZoomOut2X_##TP##_1_Nearest(NEAREST_P);          \
		res = MLIB_SUCCESS;                                       \
	    } else if (channels == 3) {                                   \
		switch (type_colormap) {                                  \
		case MLIB_BYTE:                                           \
		    res =                                                 \
			mlib##LG##ImageZoomOut2XIndex_##TP##_U8_3_##FT    \
			(BILINEAR_INDEX_P);                               \
		    break;                                                \
		case MLIB_SHORT:                                          \
		    res =                                                 \
			mlib##LG##ImageZoomOut2XIndex_##TP##_S16_3_##FT   \
			(BILINEAR_INDEX_P);                               \
		    break;                                                \
		default:                                                  \
			res = MLIB_FAILURE;                               \
			break;                                            \
		}                                                         \
	    } else if (channels == 4) {                                   \
		switch (type_colormap) {                                  \
		case MLIB_BYTE:                                           \
		    res =                                                 \
			mlib##LG##ImageZoomOut2XIndex_##TP##_U8_4_##FT    \
			(BILINEAR_INDEX_P);                               \
		    break;                                                \
		case MLIB_SHORT:                                          \
		    res =                                                 \
			mlib##LG##ImageZoomOut2XIndex_##TP##_S16_4_##FT   \
			(BILINEAR_INDEX_P);                               \
		    break;                                                \
		default:                                                  \
			res = MLIB_FAILURE;                               \
			break;                                            \
		}                                                         \
	    } else                                                        \
		res = MLIB_FAILURE;                                       \
	}                                                                 \
	if (res != MLIB_SUCCESS || edge == MLIB_EDGE_DST_NO_WRITE ||      \
	    edge == MLIB_EDGE_SRC_PADDED)                                 \
	    return (res);                                                 \
	return                                                            \
	mlib_ImageZoomOut2XIndex_##TP##_##FT##_Edge                       \
	    (EDGE_INDEX_P)

/* *********************************************************** */

#define	ALLOC_MEM_BL_U8                                           \
	mlib_s32 i, j;                                            \
	mlib_u8 *dst_color;                                       \
	mlib_s32 *lut;                                            \
	                                                          \
	if (dst_width <= 0 || dst_height <= 0)                    \
	    return (MLIB_SUCCESS);                                \
	i = (dst_width < dst_height) ? dst_height : dst_width;    \
	dst_color = __mlib_malloc(4 * i);                         \
	if (dst_color == NULL)                                    \
	    return (MLIB_FAILURE);                                \
	lut =                                                     \
	    (mlib_s32 *)mlib_ImageGetLutNormalTable(colormap) -   \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	ALLOC_MEM_BL_S16                                          \
	mlib_s32 i, j;                                            \
	mlib_s16 *dst_color;                                      \
	mlib_d64 *lut;                                            \
	                                                          \
	if (dst_width <= 0 || dst_height <= 0)                    \
	    return (MLIB_SUCCESS);                                \
	i = (dst_width < dst_height) ? dst_height : dst_width;    \
	dst_color = __mlib_malloc(2 * 4 * i);                     \
	if (dst_color == NULL)                                    \
	    return (MLIB_FAILURE);                                \
	lut =                                                     \
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -   \
	mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	FREE_MEM	__mlib_free(dst_color)

/* *********************************************************** */

#define	ALLOC_MEM_BC_U8                                               \
	mlib_s32 i, j, res, l;                                        \
	mlib_u8 *dst_color;                                           \
	mlib_u8 **lut = (mlib_u8 **)mlib_ImageGetLutData(colormap);   \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);           \
	                                                              \
	if (dst_width <= 0 || dst_height <= 0)                        \
	    return (MLIB_SUCCESS);                                    \
	res = (dst_width < dst_height) ? dst_height : dst_width;      \
	dst_color = __mlib_malloc(4 * res);                           \
	if (dst_color == NULL)                                        \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	ALLOC_MEM_BC_S16                                                \
	mlib_s32 i, j, l, res;                                          \
	mlib_u32 mask;                                                  \
	mlib_s16 *dst_color;                                            \
	mlib_s16 **lut = (mlib_s16 **)mlib_ImageGetLutData(colormap);   \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);             \
	                                                                \
	if (dst_width <= 0 || dst_height <= 0)                          \
	    return (MLIB_SUCCESS);                                      \
	res = (dst_width < dst_height) ? dst_height : dst_width;        \
	dst_color = __mlib_malloc(2 * 4 * res);                         \
	if (dst_color == NULL)                                          \
	    return MLIB_FAILURE

/* *********************************************************** */

mlib_status mlib_ImageZoomOut2XIndex_U8_Bilinear_Edge(
    EDGE_INDEX_PARAMETERS);
mlib_status mlib_ImageZoomOut2XIndex_S16_Bilinear_Edge(
    EDGE_INDEX_PARAMETERS);
mlib_status mlib_ImageZoomOut2XIndex_U8_Bicubic_Edge(
    EDGE_INDEX_PARAMETERS);
mlib_status mlib_ImageZoomOut2XIndex_S16_Bicubic_Edge(
    EDGE_INDEX_PARAMETERS);
mlib_status mlib_ImageZoomOut2XIndex_U8_Bicubic2_Edge(
    EDGE_INDEX_PARAMETERS);
mlib_status mlib_ImageZoomOut2XIndex_S16_Bicubic2_Edge(
    EDGE_INDEX_PARAMETERS);

/* *********************************************************** */

void mlib_ImageZoomOut2X_Nearest(
    NEAREST_PARAMETERS(void),
    mlib_s32 tshift,
    mlib_s32 chan);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEZOOMOUT2X_H */
