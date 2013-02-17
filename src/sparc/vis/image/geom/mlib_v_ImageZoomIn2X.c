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

#pragma ident	"@(#)mlib_v_ImageZoomIn2X.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2X -  image 2X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2X(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_filter      filter,
 *                                     mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *    The center of the source image is mapped to the center of the
 *    destination image.
 *
 *    The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *    The resampling filter can be one of the following:
 *        MLIB_NEAREST
 *        MLIB_BILINEAR
 *        MLIB_BICUBIC
 *        MLIB_BICUBIC2
 *
 *    The edge condition can be one of the following:
 *        MLIB_EDGE_DST_NO_WRITE    (default)
 *        MLIB_EDGE_DST_FILL_ZERO
 *        MLIB_EDGE_OP_NEAREST
 *        MLIB_EDGE_SRC_EXTEND
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageZoomIn2X.h>
#include <mlib_v_ImageZoomIn2X.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomIn2X = __mlib_ImageZoomIn2X

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomIn2X) mlib_ImageZoomIn2X
    __attribute__((weak, alias("__mlib_ImageZoomIn2X")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                                 \
	MLIB_TYPE *pdst, const MLIB_TYPE *psrc, mlib_s32 src_width,      \
	mlib_s32 src_height, mlib_s32 dst_stride,                        \
	mlib_s32 src_stride, mlib_s32 src_w_beg, mlib_s32 src_h_beg,     \
	mlib_s32 channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE)                                \
	(MLIB_TYPE *) pdst, (MLIB_TYPE *) psrc, src_width, src_height,   \
	dst_stride, src_stride, src_w_beg, src_h_beg, channels

/* *********************************************************** */

#define	PARAM1                                                           \
	(void *)pdst, (void *)psrc, src_width, src_height, dst_stride,   \
	src_stride, src_w_beg, src_h_beg

/* *********************************************************** */

#define	PARAM2                                                           \
	(void *)pdst, (void *)psrc, src_width, src_height, dst_stride,   \
	src_stride, src_w_beg, src_h_beg, channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFEDGE(MLIB_TYPE)                            \
	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE), MLIB_TYPE * pdst_near,   \
	const MLIB_TYPE *psrc_near, mlib_s32 src_width_near,       \
	mlib_s32 src_height_near, mlib_s32 src_w_beg_near,         \
	mlib_s32 src_h_beg_near
/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLEDGE(MLIB_TYPE)                                \
	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE), (MLIB_TYPE *) pdst_near,     \
	(MLIB_TYPE *) psrc_near, src_width_near, src_height_near,       \
	src_w_beg_near, src_h_beg_near

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFEDGE_BC(MLIB_TYPE)                       \
	MLIB_ZOOMIN2XDEFEDGE(MLIB_TYPE), MLIB_TYPE * psrc_beg,   \
	mlib_s32 src_width_beg, mlib_s32 src_height_beg

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLEDGE_BC(MLIB_TYPE)                         \
	MLIB_ZOOMIN2XCALLEDGE(MLIB_TYPE), (MLIB_TYPE *) psrc_beg,   \
	src_width_beg, src_height_beg

/* *********************************************************** */

#define	SIZE(chan)                                                        \
	if ((2 * src_height) > dst_height) {                              \
	    src_h_beg = (2 * src_height - dst_height + 1) >> 1;           \
	    src_height = dst_height;                                      \
	} else {                                                          \
	    pdst += ((dst_height - 2 * src_height) >> 1) * dst_stride;    \
	    src_h_beg = 0;                                                \
	    src_height *= 2;                                              \
	}                                                                 \
	                                                                  \
	if ((2 * src_width) > dst_width) {                                \
	    src_w_beg = (2 * src_width - dst_width + 1) >> 1;             \
	    src_width = dst_width;                                        \
	} else {                                                          \
	    pdst += ((dst_width - 2 * src_width) >> 1) * chan;            \
	    src_w_beg = 0;                                                \
	    src_width *= 2;                                               \
	}                                                                 \
	                                                                  \
	psrc += (src_h_beg >> 1) * src_stride + (src_w_beg >> 1) * chan

/* *********************************************************** */

#define	SIZE_NN(chan)                                                  \
	pdst_near = pdst;                                              \
	psrc_near = psrc;                                              \
	if ((2 * src_height) > dst_height) {                           \
	    src_h_beg_near = (2 * src_height - dst_height + 1) >> 1;   \
	    src_height_near = dst_height;                              \
	} else {                                                       \
	    pdst_near +=                                               \
		((dst_height - 2 * src_height) >> 1) * dst_stride;     \
	    src_h_beg_near = 0;                                        \
	    src_height_near = 2 * src_height;                          \
	}                                                              \
	                                                               \
	if ((2 * src_width) > dst_width) {                             \
	    src_w_beg_near = (2 * src_width - dst_width + 1) >> 1;     \
	    src_width_near = dst_width;                                \
	} else {                                                       \
	    pdst_near += ((dst_width - 2 * src_width) >> 1) * chan;    \
	    src_w_beg_near = 0;                                        \
	    src_width_near = 2 * src_width;                            \
	}                                                              \
	                                                               \
	psrc_near +=                                                   \
	    (src_h_beg_near >> 1) * src_stride +                       \
	    (src_w_beg_near >> 1) * chan

/* *********************************************************** */

#define	SIZE_BL(chan)                                                      \
	if (src_height >= (dst_height + 2) >> 1) {                         \
	    src_h_beg = src_height - ((dst_height + 2) >> 1);              \
	    if (src_height == (dst_height + 1) >> 1)                       \
		src_height = dst_height - 1;                               \
	    else                                                           \
		src_height = dst_height;                                   \
	} else {                                                           \
	    pdst += (((dst_height + 2) >> 1) - src_height) * dst_stride;   \
	    src_h_beg = 0;                                                 \
	    src_height = 2 * src_height - 2;                               \
	}                                                                  \
	                                                                   \
	if (src_width >= (dst_width + 2) >> 1) {                           \
	    src_w_beg = src_width - ((dst_width + 2) >> 1);                \
	    if (src_width == (dst_width + 1) >> 1)                         \
		src_width = dst_width - 1;                                 \
	    else                                                           \
		src_width = dst_width;                                     \
	} else {                                                           \
	    pdst += (((dst_width + 2) >> 1) - src_width) * chan;           \
	    src_w_beg = 0;                                                 \
	    src_width = 2 * src_width - 2;                                 \
	}                                                                  \
	                                                                   \
	psrc += (src_h_beg >> 1) * src_stride + (src_w_beg >> 1) * chan

/* *********************************************************** */

#define	SIZE_BC(chan)                                                      \
	if (src_height >= (((dst_height + 2) >> 1) + 2)) {                 \
	    src_h_beg = src_height - ((dst_height + 2) >> 1);              \
	    if (src_height == (((dst_height + 1) >> 1) + 2))               \
		src_height = dst_height - 1;                               \
	    else                                                           \
		src_height = dst_height;                                   \
	} else {                                                           \
	    pdst +=                                                        \
		(((dst_height + 2) >> 1) - src_height + 2) * dst_stride;   \
	    src_h_beg = 2;                                                 \
	    src_height = 2 * src_height - 6;                               \
	}                                                                  \
	                                                                   \
	if (src_width >= (((dst_width + 2) >> 1) + 2)) {                   \
	    src_w_beg = src_width - ((dst_width + 2) >> 1);                \
	    if (src_width == (((dst_width + 1) >> 1) + 2))                 \
		src_width = dst_width - 1;                                 \
	    else                                                           \
		src_width = dst_width;                                     \
	} else {                                                           \
	    pdst += (((dst_width + 2) >> 1) - src_width + 2) * chan;       \
	    src_w_beg = 2;                                                 \
	    src_width = 2 * src_width - 6;                                 \
	}                                                                  \
	                                                                   \
	psrc += (src_h_beg >> 1) * src_stride + (src_w_beg >> 1) * chan

/* *********************************************************** */

#define	SIZE_SRC_PADDED(chan)                                           \
	{                                                               \
	    mlib_s32 minX, minY, maxX, maxY, deltaX, deltaY, right =    \
		1, bottom = 1;                                          \
	    mlib_u8 *paddings = mlib_ImageGetPaddings(src);             \
	                                                                \
	    minX = 2 * src_width_beg - src_width;                       \
	    minY = 2 * src_height_beg - src_height;                     \
	    maxX = 2 * src_width_beg + src_width;                       \
	    maxY = 2 * src_height_beg + src_height;                     \
	    if (minX < 4 * paddings[0]) {                               \
		deltaX = 4 * paddings[0] - minX;                        \
		if (filter == MLIB_NEAREST) {                           \
		    psrc +=                                             \
			((deltaX >> 2) + ((deltaX & 2) >> 1)) * chan;   \
		    src_w_beg = 0;                                      \
		} else {                                                \
		    psrc += (deltaX >> 2) * chan;                       \
		    src_w_beg = 1;                                      \
		}                                                       \
		pdst += (deltaX >> 1) * chan;                           \
		minX = 4 * paddings[0];                                 \
	    }                                                           \
	    if (minY < 4 * paddings[1]) {                               \
		deltaY = 4 * paddings[1] - minY;                        \
		if (filter == MLIB_NEAREST) {                           \
		    psrc +=                                             \
			((deltaY >> 2) +                                \
			((deltaY & 2) >> 1)) * src_stride;              \
		    src_h_beg = 0;                                      \
		} else {                                                \
		    psrc += (deltaY >> 2) * src_stride;                 \
		    src_h_beg = 1;                                      \
		}                                                       \
		pdst += (deltaY >> 1) * dst_stride;                     \
		minY = 4 * paddings[1];                                 \
	    }                                                           \
	    if (maxX >= 4 * (src_width_beg - paddings[2])) {            \
		maxX = 4 * (src_width_beg - paddings[2]);               \
		right = 0;                                              \
	    }                                                           \
	    if (maxY >= 4 * (src_height_beg - paddings[3])) {           \
		maxY = 4 * (src_height_beg - paddings[3]);              \
		bottom = 0;                                             \
	    }                                                           \
	    src_width = (maxX - minX + right) / 2;                      \
	    src_height = (maxY - minY + bottom) / 2;                    \
	}
/* *********************************************************** */

#define	SWITCH_FUNC(TT, FLT)                                    \
	switch (4 * (channels - 1) + flag) {                    \
	case 0:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_1##FLT##_FL00       \
		(PARAM1);                                       \
	    break;                                              \
	case 1:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_1##FLT##_FL01       \
		(PARAM1);                                       \
	    break;                                              \
	case 2:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_1##FLT##_FL10       \
		(PARAM1);                                       \
	    break;                                              \
	case 3:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_1##FLT##_FL11       \
		(PARAM1);                                       \
	    break;                                              \
	case 4:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_2##FLT##_FL00       \
		(PARAM1);                                       \
	    break;                                              \
	case 5:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_2##FLT##_FL01       \
		(PARAM1);                                       \
	    break;                                              \
	case 6:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_2##FLT##_FL10       \
		(PARAM1);                                       \
	    break;                                              \
	case 7:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_2##FLT##_FL11       \
		(PARAM1);                                       \
	    break;                                              \
	case 8:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_3##FLT##_FL00       \
		(PARAM1);                                       \
	    break;                                              \
	case 9:                                                 \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_3##FLT##_FL01       \
		(PARAM1);                                       \
	    break;                                              \
	case 10:                                                \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_3##FLT##_FL10       \
		(PARAM1);                                       \
	    break;                                              \
	case 11:                                                \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_3##FLT##_FL11       \
		(PARAM1);                                       \
	    break;                                              \
	case 12:                                                \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_4##FLT##_FL00       \
		(PARAM1);                                       \
	    break;                                              \
	case 13:                                                \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_4##FLT##_FL01       \
		(PARAM1);                                       \
	    break;                                              \
	case 14:                                                \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_4##FLT##_FL10       \
		(PARAM1);                                       \
	    break;                                              \
	case 15:                                                \
	    res =                                               \
		mlib_v_ImageZoomIn2X_##TT##_4##FLT##_FL11       \
		(PARAM1);                                       \
	    break;                                              \
	}                                                       \
	                                                        \
	if (res != MLIB_SUCCESS)                                \
	    return res

/* *********************************************************** */

#define	SWITCH_FUNC_BL(TT)                                      \
	res = MLIB_FAILURE;                                     \
	if (channels == 3 && flag != 3) {                       \
	    switch (flag) {                                     \
	    case 0:                                             \
		mlib_c_ImageZoomIn2X_##TT##_BL_FL0(PARAM2);     \
		break;                                          \
	    case 1:                                             \
		mlib_c_ImageZoomIn2X_##TT##_BL_FL1(PARAM2);     \
		break;                                          \
	    case 2:                                             \
		mlib_c_ImageZoomIn2X_##TT##_BL_FL2(PARAM2);     \
		break;                                          \
	    case 3:                                             \
		mlib_c_ImageZoomIn2X_##TT##_BL_FL3(PARAM2);     \
		break;                                          \
	    }                                                   \
	} else {                                                \
	    switch (flag) {                                     \
	    case 0:                                             \
		mlib_v_ImageZoomIn2X_##TT##_BL_FL0(PARAM2);     \
		break;                                          \
	    case 1:                                             \
		mlib_v_ImageZoomIn2X_##TT##_BL_FL1(PARAM2);     \
		break;                                          \
	    case 2:                                             \
		mlib_v_ImageZoomIn2X_##TT##_BL_FL2(PARAM2);     \
		break;                                          \
	    case 3:                                             \
		mlib_v_ImageZoomIn2X_##TT##_BL_FL3(PARAM2);     \
		break;                                          \
	    }                                                   \
	}

/* *********************************************************** */

/* sp0:  0, 1,2, 6, 7, 8  */
/* sp1:  3, 4,5, 9,10, 11  */

void
mlib_ImageMerge3ch_S16(
    const void *src0,
    const void *src1,
    void *dst,
    mlib_s32 n)
{
	const mlib_u16 *sp0 = src0;
	const mlib_u16 *sp1 = src1;
	mlib_u16 *dp = dst;
	mlib_s32 i;

	for (i = 0; i < n / 4; i++) {
		((mlib_s32 *)dp)[0] = ((mlib_s32 *)sp0)[0];
		((mlib_s32 *)dp)[1] = (sp0[2] << 16) | sp1[0];
		((mlib_s32 *)dp)[2] = (sp1[1] << 16) | sp1[2];
		((mlib_s32 *)dp)[3] = (sp0[3] << 16) | sp0[4];
		((mlib_s32 *)dp)[4] = (sp0[5] << 16) | sp1[3];
		((mlib_s32 *)dp)[5] = ((mlib_s32 *)sp1)[2];
		sp0 += 6;
		sp1 += 6;
		dp += 12;
	}

	if (n & 2) {
		((mlib_s32 *)dp)[0] = (sp0[0] << 16) | sp0[1];
		((mlib_s32 *)dp)[1] = (sp0[2] << 16) | sp1[0];
		((mlib_s32 *)dp)[2] = (sp1[1] << 16) | sp1[2];
		sp0 += 3;
		sp1 += 3;
		dp += 6;
	}

	if (n & 1) {
		dp[0] = sp0[0];
		dp[1] = sp0[1];
		dp[2] = sp0[2];
	}
}

/* *********************************************************** */

/* sp0:  0, 1,2, 6   7, 8, 12, 13  14, 18, 19, 20  */
/* sp1:  3, 4,5, 9  10, 11, 15, 16  17, 21, 22, 23  */

/* *********************************************************** */

#define	MERGE_CH3(dp, sp0, sp1)                                            \
	sd0 = vis_freg_pair(((mlib_f32 *)sp0)[0], ((mlib_f32 *)sp0)[1]);   \
	sd1 = vis_freg_pair(((mlib_f32 *)sp1)[0], vis_fzeros());           \
	dd0 = vis_fand(sd0, dmask0);                                       \
	dd1 = vis_fand(vis_faligndata(sd1, sd1), dmask1);                  \
	dd2 = vis_fand(vis_faligndata(sd0, sd0), dmask2);                  \
	*(mlib_d64 *)dp = vis_for(vis_for(dd0, dd1), dd2);                 \
	a1 = *(mlib_u16 *)(sp0 + 6);                                       \
	a2 = *(mlib_u32 *)(sp0 + 8);                                       \
	b0 = *(mlib_u16 *)(sp1 + 4);                                       \
	b1 = *(mlib_u16 *)(sp1 + 6);                                       \
	b2 = *(mlib_u32 *)(sp1 + 8);                                       \
	*(mlib_u32 *)(dp + 8) = (sp0[5] << 24) | (sp1[3] << 16) | b0;      \
	*(mlib_u32 *)(dp + 12) =                                           \
	    (a1 << 16) | ((a2 >> 24) << 8) | (b1 >> 8);                    \
	*(mlib_u32 *)(dp + 16) =                                           \
	    (((b1 << 8) | (b2 >> 24)) << 16) | ((a2 >> 8) & 0xFFFF);       \
	*(mlib_u32 *)(dp + 20) = (a2 << 24) | (b2 & 0x00FFFFFF)

/* *********************************************************** */

void
mlib_ImageMerge3ch_U8(
    const void *src0,
    const void *src1,
    void *dst,
    mlib_s32 n)
{
	mlib_d64 buff[3];
	mlib_u8 *pbuff = (mlib_u8 *)buff;
	const mlib_u8 *sp0 = src0;
	const mlib_u8 *sp1 = src1;
	mlib_u8 *dp = dst;
	mlib_d64 sd0, sd1, dd0, dd1, dd2;
	mlib_d64 dmask0 = vis_to_double(0xFFFFFF00, 0x00000000);
	mlib_d64 dmask1 = vis_to_double(0x000000FF, 0xFFFF0000);
	mlib_d64 dmask2 = vis_to_double(0x00000000, 0x0000FFFF);
	mlib_u32 a1, a2, b0, b1, b2;
	mlib_s32 i;

	vis_alignaddr((void *)5, 0);

	for (i = 0; i < n / 8; i++) {
		MERGE_CH3(dp, sp0, sp1);
		sp0 += 12;
		sp1 += 12;
		dp += 24;
	}

	if (n & 7) {
		MERGE_CH3(pbuff, sp0, sp1);
		for (i = 0; i < 3 * (n & 7); i++) {
			dp[i] = pbuff[i];
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageZoomIn2X(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_u8 *pdst, *psrc, *pdst_near, *psrc_near, *psrc_beg;
	mlib_s32 src_height, dst_height, src_width, dst_width, src_stride,
	    dst_stride;
	mlib_s32 type, channels;
	mlib_s32 src_w_beg, src_h_beg, flag;
	mlib_s32 src_width_near, src_height_near, src_w_beg_near,
	    src_h_beg_near;
	mlib_s32 src_width_beg, src_height_beg;
	mlib_status res;

/*  check for obvious errors  */

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	pdst = (mlib_u8 *)mlib_ImageGetData(dst);
	psrc = (mlib_u8 *)mlib_ImageGetData(src);
/* height of destination image */
	dst_height = mlib_ImageGetHeight(dst);
/* height of source image */
	src_height = mlib_ImageGetHeight(src);
/* width of destination image */
	dst_width = mlib_ImageGetWidth(dst);
/* width of source image */
	src_width = mlib_ImageGetWidth(src);
/* bytes to next row */
	dst_stride = mlib_ImageGetStride(dst);
/* bytes to next row */
	src_stride = mlib_ImageGetStride(src);
/* channels in destination image */
	channels = mlib_ImageGetChannels(dst);
/* type of destination image */
	type = mlib_ImageGetType(dst);

	if (type == MLIB_SHORT || type == MLIB_USHORT) {
		channels *= 2;
	} else if (type == MLIB_INT) {
		channels *= 4;
	}

	SIZE_NN(channels);

	src_width_beg = src_width;
	src_height_beg = src_height;

	if (filter == MLIB_NEAREST) {
		pdst = pdst_near;
		psrc = psrc_near;
		src_width = src_width_near;
		src_height = src_height_near;
		src_w_beg = src_w_beg_near;
		src_h_beg = src_h_beg_near;
	} else {
		flag = (dst_height & 1) | ((dst_width & 1) << 1);
		psrc_beg = psrc;

		if (filter == MLIB_BILINEAR) {
			SIZE_BL(channels);
		} else {
			SIZE_BC(channels);
		}
	}

	if (edge == MLIB_EDGE_SRC_PADDED) {
		SIZE_SRC_PADDED(channels);
	}

	if (src_width < 0)
		src_width = 0;

	if (src_height < 0)
		src_height = 0;

	if (type == MLIB_SHORT || type == MLIB_USHORT) {
		channels >>= 1;
		dst_stride >>= 1;
		src_stride >>= 1;
	} else if (type == MLIB_INT) {
		channels >>= 2;
		dst_stride >>= 2;
		src_stride >>= 2;
	}

	if ((src_width > 0) && (src_height > 0))
		switch (type) {
/* handle MLIB_BYTE data type of image */
		case MLIB_BYTE:
			switch (filter) {
			case MLIB_NEAREST:
				mlib_v_ImageZoomIn2X_U8_NN
				    (MLIB_ZOOMIN2XCALLPARAM(mlib_u8));
				break;
			case MLIB_BILINEAR:
				SWITCH_FUNC_BL(U8);
				break;
			case MLIB_BICUBIC:
				SWITCH_FUNC(U8, BC);
				break;
			case MLIB_BICUBIC2:
				SWITCH_FUNC(U8, BC2);
				break;
			}

			break;
/* handle MLIB_SHORT data type of image */
		case MLIB_SHORT:
			switch (filter) {
			case MLIB_NEAREST:
				mlib_v_ImageZoomIn2X_S16_NN
				    (MLIB_ZOOMIN2XCALLPARAM(mlib_s16));
				break;
			case MLIB_BILINEAR:
				SWITCH_FUNC_BL(S16);
				break;
			case MLIB_BICUBIC:
				SWITCH_FUNC(S16, BC);
				break;
			case MLIB_BICUBIC2:
				SWITCH_FUNC(S16, BC2);
				break;
			}

			break;
		case MLIB_USHORT:
			switch (filter) {
			case MLIB_NEAREST:
				mlib_v_ImageZoomIn2X_S16_NN
				    (MLIB_ZOOMIN2XCALLPARAM(mlib_s16));
				break;
			case MLIB_BILINEAR:
				SWITCH_FUNC_BL(U16);
				break;
			case MLIB_BICUBIC:
				SWITCH_FUNC(U16, BC);
				break;
			case MLIB_BICUBIC2:
				SWITCH_FUNC(U16, BC2);
				break;
			}

			break;
		case MLIB_INT:
			switch (filter) {
			case MLIB_NEAREST:

				if (channels <= 2) {
					channels *= 2;
					src_stride *= 2;
					dst_stride *= 2;
					mlib_v_ImageZoomIn2X_S16_NN
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s16));
					break;
				} else {
					mlib_ImageZoomIn2X_S32_NN
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				}

			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
					mlib_ImageZoomIn2X_S32_BL_FL0
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 1:
					mlib_ImageZoomIn2X_S32_BL_FL1
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 2:
					mlib_ImageZoomIn2X_S32_BL_FL2
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 3:
					mlib_ImageZoomIn2X_S32_BL_FL3
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
					mlib_ImageZoomIn2X_S32_BC_FL0
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 1:
					mlib_ImageZoomIn2X_S32_BC_FL1
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 2:
					mlib_ImageZoomIn2X_S32_BC_FL2
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 3:
					mlib_ImageZoomIn2X_S32_BC_FL3
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
					mlib_ImageZoomIn2X_S32_BC2_FL0
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 1:
					mlib_ImageZoomIn2X_S32_BC2_FL1
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 2:
					mlib_ImageZoomIn2X_S32_BC2_FL2
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				case 3:
					mlib_ImageZoomIn2X_S32_BC2_FL3
					    (MLIB_ZOOMIN2XCALLPARAM(mlib_s32));
					break;
				}

				break;
			}

			break;
		default:
			return (MLIB_FAILURE);
		}

	if ((filter != MLIB_NEAREST) && (edge != MLIB_EDGE_DST_NO_WRITE) &&
	    (edge != MLIB_EDGE_SRC_PADDED) && (src_width_near > 0) &&
	    (src_height_near > 0)) {
		switch (type) {
		case MLIB_BYTE:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_U8_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_u8));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_U8_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_u8));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U8_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					case 1:
						mlib_ImageZoomIn2X_U8_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					case 2:
						mlib_ImageZoomIn2X_U8_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					case 3:
						mlib_ImageZoomIn2X_U8_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U8_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 1:
						mlib_ImageZoomIn2X_U8_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 2:
						mlib_ImageZoomIn2X_U8_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 3:
						mlib_ImageZoomIn2X_U8_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 1:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 2:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 3:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					}

					break;
				}
			}

			break;
		case MLIB_SHORT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_S16_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_S16_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S16_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					case 1:
						mlib_ImageZoomIn2X_S16_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					case 2:
						mlib_ImageZoomIn2X_S16_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					case 3:
						mlib_ImageZoomIn2X_S16_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S16_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					case 1:
						mlib_ImageZoomIn2X_S16_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					case 2:
						mlib_ImageZoomIn2X_S16_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					case 3:
						mlib_ImageZoomIn2X_S16_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL0
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					case 1:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL1
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					case 2:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL2
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					case 3:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL3
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					}

					break;
				}
			}

			break;
		case MLIB_USHORT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_S16_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_S16_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U16_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					case 1:
						mlib_ImageZoomIn2X_U16_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					case 2:
						mlib_ImageZoomIn2X_U16_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					case 3:
						mlib_ImageZoomIn2X_U16_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U16_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					case 1:
						mlib_ImageZoomIn2X_U16_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					case 2:
						mlib_ImageZoomIn2X_U16_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					case 3:
						mlib_ImageZoomIn2X_U16_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL0
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					case 1:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL1
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					case 2:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL2
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					case 3:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL3
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					}

					break;
				}
			}

			break;
		case MLIB_INT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_S32_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s32));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_S32_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s32));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S32_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					case 1:
						mlib_ImageZoomIn2X_S32_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					case 2:
						mlib_ImageZoomIn2X_S32_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					case 3:
						mlib_ImageZoomIn2X_S32_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S32_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					case 1:
						mlib_ImageZoomIn2X_S32_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					case 2:
						mlib_ImageZoomIn2X_S32_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					case 3:
						mlib_ImageZoomIn2X_S32_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL0
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					case 1:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL1
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					case 2:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL2
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					case 3:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL3
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					}

					break;
				}
			}

			break;
		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_ImageZoomIn2X_Edge(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_u8 *pdst, *psrc, *pdst_near, *psrc_near, *psrc_beg;
	mlib_s32 src_height, dst_height, src_width, dst_width, src_stride,
	    dst_stride;
	mlib_s32 type, channels;
	mlib_s32 src_w_beg, src_h_beg, flag;
	mlib_s32 src_width_near, src_height_near, src_w_beg_near,
	    src_h_beg_near;
	mlib_s32 src_width_beg, src_height_beg;
	mlib_status res;

/*  check for obvious errors  */

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	pdst = (mlib_u8 *)mlib_ImageGetData(dst);
	psrc = (mlib_u8 *)mlib_ImageGetData(src);
/* height of destination image */
	dst_height = mlib_ImageGetHeight(dst);
/* height of source image */
	src_height = mlib_ImageGetHeight(src);
/* width of destination image */
	dst_width = mlib_ImageGetWidth(dst);
/* width of source image */
	src_width = mlib_ImageGetWidth(src);
/* bytes to next row */
	dst_stride = mlib_ImageGetStride(dst);
/* bytes to next row */
	src_stride = mlib_ImageGetStride(src);
/* channels in destination image */
	channels = mlib_ImageGetChannels(dst);
/* type of destination image */
	type = mlib_ImageGetType(dst);

	if (type == MLIB_SHORT || type == MLIB_USHORT) {
		channels *= 2;
	} else if (type == MLIB_INT) {
		channels *= 4;
	}

	SIZE_NN(channels);

	src_width_beg = src_width;
	src_height_beg = src_height;

	if (filter == MLIB_NEAREST) {
		pdst = pdst_near;
		psrc = psrc_near;
		src_width = src_width_near;
		src_height = src_height_near;
		src_w_beg = src_w_beg_near;
		src_h_beg = src_h_beg_near;
	} else {
		flag = (dst_height & 1) | ((dst_width & 1) << 1);
		psrc_beg = psrc;

		if (filter == MLIB_BILINEAR) {
			SIZE_BL(channels);
		} else {
			SIZE_BC(channels);
		}
	}

	if (edge == MLIB_EDGE_SRC_PADDED) {
		SIZE_SRC_PADDED(channels);
	}

	if (src_width < 0)
		src_width = 0;

	if (src_height < 0)
		src_height = 0;

	if (type == MLIB_SHORT || type == MLIB_USHORT) {
		channels >>= 1;
		dst_stride >>= 1;
		src_stride >>= 1;
	} else if (type == MLIB_INT) {
		channels >>= 2;
		dst_stride >>= 2;
		src_stride >>= 2;
	}

	if ((src_width_near > 0) && (src_height_near > 0)) {
		switch (type) {
		case MLIB_BYTE:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_U8_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_u8));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_U8_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_u8));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U8_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					case 1:
						mlib_ImageZoomIn2X_U8_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					case 2:
						mlib_ImageZoomIn2X_U8_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					case 3:
						mlib_ImageZoomIn2X_U8_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u8));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U8_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 1:
						mlib_ImageZoomIn2X_U8_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 2:
						mlib_ImageZoomIn2X_U8_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 3:
						mlib_ImageZoomIn2X_U8_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 1:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 2:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					case 3:
						mlib_ImageZoomIn2X_U8_SE_BC2_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u8));
						break;
					}

					break;
				}
			}

			break;
		case MLIB_SHORT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_S16_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_S16_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S16_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					case 1:
						mlib_ImageZoomIn2X_S16_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					case 2:
						mlib_ImageZoomIn2X_S16_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					case 3:
						mlib_ImageZoomIn2X_S16_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s16));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S16_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					case 1:
						mlib_ImageZoomIn2X_S16_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					case 2:
						mlib_ImageZoomIn2X_S16_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					case 3:
						mlib_ImageZoomIn2X_S16_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s16));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL0
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					case 1:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL1
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					case 2:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL2
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					case 3:
					    mlib_ImageZoomIn2X_S16_SE_BC2_FL3
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s16));
					    break;
					}

					break;
				}
			}

			break;
		case MLIB_USHORT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_S16_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_S16_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s16));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U16_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					case 1:
						mlib_ImageZoomIn2X_U16_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					case 2:
						mlib_ImageZoomIn2X_U16_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					case 3:
						mlib_ImageZoomIn2X_U16_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_u16));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_U16_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					case 1:
						mlib_ImageZoomIn2X_U16_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					case 2:
						mlib_ImageZoomIn2X_U16_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					case 3:
						mlib_ImageZoomIn2X_U16_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_u16));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL0
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					case 1:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL1
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					case 2:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL2
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					case 3:
					    mlib_ImageZoomIn2X_U16_SE_BC2_FL3
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_u16));
					    break;
					}

					break;
				}
			}

			break;
		case MLIB_INT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomIn2X_S32_CE(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s32));
				break;
			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomIn2X_S32_ON(MLIB_ZOOMIN2XCALLEDGE
				    (mlib_s32));
				break;
			case MLIB_EDGE_SRC_EXTEND:
				switch (filter) {
				case MLIB_BILINEAR:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S32_SE_BL_FL0
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					case 1:
						mlib_ImageZoomIn2X_S32_SE_BL_FL1
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					case 2:
						mlib_ImageZoomIn2X_S32_SE_BL_FL2
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					case 3:
						mlib_ImageZoomIn2X_S32_SE_BL_FL3
						    (MLIB_ZOOMIN2XCALLEDGE
						    (mlib_s32));
						break;
					}

					break;
				case MLIB_BICUBIC:
					switch (flag) {
					case 0:
						mlib_ImageZoomIn2X_S32_SE_BC_FL0
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					case 1:
						mlib_ImageZoomIn2X_S32_SE_BC_FL1
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					case 2:
						mlib_ImageZoomIn2X_S32_SE_BC_FL2
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					case 3:
						mlib_ImageZoomIn2X_S32_SE_BC_FL3
						    (MLIB_ZOOMIN2XCALLEDGE_BC
						    (mlib_s32));
						break;
					}

					break;
				case MLIB_BICUBIC2:
					switch (flag) {
					case 0:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL0
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					case 1:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL1
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					case 2:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL2
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					case 3:
					    mlib_ImageZoomIn2X_S32_SE_BC2_FL3
						(MLIB_ZOOMIN2XCALLEDGE_BC
						(mlib_s32));
					    break;
					}

					break;
				}
			}

			break;
		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
