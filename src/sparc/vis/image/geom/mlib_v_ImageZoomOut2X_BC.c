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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageZoomOut2X_BC.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *  mlib_ImageZoomOut2X_U8_x_Bicubic - image 0.5X scaling,
 *                                     image channel(s) x = 1, 2,3, 4,
 *                                     filter is MLIB_BICUBIC/MLIB_BICUBIC2.
 * SYNOPSIS
 *  mlib_status mlib_ImageZoomOut2X_U8_x_Bicubic(mlib_u8   *p_src,
 *                                               mlib_u8   *p_dst,
 *                                               mlib_s32  src_stride,
 *                                               mlib_s32  dst_stride,
 *                                               mlib_s32  dst_height,
 *                                               mlib_s32  dst_width,
 *                                               mlib_s32  dh,
 *                                               mlib_s32  dw)
 *
 * ARGUMENTS
 *       p_dst       Aligned pointer to destination image.
 *       p_src       Aligned pointer to source image.
 *       src_stride  Bytes count in source image string.
 *       dst_stride  Bytes count in destination image string.
 *       dst_height  Height of destination image.
 *       dst_width   Width of destination image.
 *       dh, dw      Pixel center shift from source to destination images.
 *
 * DESCRIPTION
 *       VIS functions without edge condition processing
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageZoom2X.h>
#include <mlib_ImageZoomOut2X.h>

/* *********************************************************** */

static const mlib_u32 mlib_coef_bc[] = {
	0x01000100,
	0x09000900,
	0x00400040, /*  1 << 6 */
	0x02400240, /*  9 << 6 */
	0x14401440, /* 81 << 6 */
	0x10101010,
	0x90909090,
	4,
};

static const mlib_u32 mlib_coef_bc2[] = {
	0x01000100,
	0x05000500,
	0x01000100, /*  1 << 8 */
	0x05000500, /*  5 << 8 */
	0x19001900, /* 25 << 8 */
	0x20202020,
	0xA0A0A0A0,
	3,
};

/* *********************************************************** */

#undef NEAREST_PARAMETERS

/* *********************************************************** */

#define	NEAREST_PARAMETERS(type)                                      \
	type	*p_src,                                               \
	type	*p_dst,                                               \
	mlib_s32 src_stride,                                          \
	mlib_s32 dst_stride,                                          \
	mlib_s32 dst_height,                                          \
	mlib_s32 dst_width,                                           \
	const mlib_u32 *coef

/* *********************************************************** */

#define	NEAREST_PARAMETERS1(type)                                     \
	type	*s_data,                                              \
	type	*d_data,                                              \
	mlib_s32 s_stride,                                            \
	mlib_s32 d_stride,                                            \
	mlib_s32 height,                                              \
	mlib_s32 d_width,                                             \
	const mlib_u32 *coef

/* *********************************************************** */

static void mlib_ImageZoomOut2X_U8_2_Bicubic_X(
    NEAREST_PARAMETERS(mlib_u8));
static void mlib_ImageZoomOut2X_U8_2_Bicubic_XY(
    NEAREST_PARAMETERS(mlib_u8));
static void mlib_ImageZoomOut2X_U8_2_Bicubic_XY_NotAl(
    NEAREST_PARAMETERS1(mlib_u8));
static void mlib_ImageZoomOut2X_U8_3_Bicubic_X(
    NEAREST_PARAMETERS1(mlib_u8));
static void mlib_ImageZoomOut2X_U8_3_Bicubic_XY(
    NEAREST_PARAMETERS1(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_X(
    NEAREST_PARAMETERS(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_Y(
    NEAREST_PARAMETERS(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_XY(
    NEAREST_PARAMETERS(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_Float_X(
    NEAREST_PARAMETERS(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_X_NotAl(
    NEAREST_PARAMETERS1(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_Y_NotAl(
    NEAREST_PARAMETERS1(mlib_u8));
static void mlib_ImageZoomOut2X_U8_4_Bicubic_XY_NotAl(
    NEAREST_PARAMETERS1(mlib_u8));
static void mlib_ImageZoomOut2X_U8_1_Bicubic_XY(
    NEAREST_PARAMETERS(mlib_u8));

/* *********************************************************** */

#define	PARAM1                                                  \
	(void *)p_src, (void *)p_dst, src_stride, dst_stride,   \
	dst_height, dst_width

/* *********************************************************** */

#define	PARAM_BC	PARAM1, mlib_coef_bc

/* *********************************************************** */

#define	PARAM_BC2	PARAM1, mlib_coef_bc2

/* *********************************************************** */

#define	LOAD_COEF()                                             \
	mlib_s32 BC_SCL = coef[7];                              \
	mlib_s32 BC_SC2 = 2 * BC_SCL;                           \
	mlib_s32 BC_SHF = BC_SCL - 1;                           \
	mlib_u32 c1 = coef[0];                                  \
	mlib_u32 c2 = coef[1]

#define	BC_CF(x)	(((x) << BC_SHF) + (x))

/* *********************************************************** */

#define	MERGE(a, b)	(((a) & 0xFFFF0000) | ((b) & 0x0000FFFF))

/* *********************************************************** */

#define	DM_000XXXXX	d_mask_000XXXXX = vis_to_double(0x00FFFFFF, 0xFFFFFFFF)
#define	DMF_X000	f_mask_X000 = vis_to_float(0xFF000000)
#define	DMF_0000	f_mask_0000 = vis_fzeros()

/* *********************************************************** */

#define	DM_1111	d_mask_1111 = vis_to_double(c1, c1)
#define	DM_9999	d_mask_9999 = vis_to_double(c2, c2)

#define	DM_0111	d_mask_0111 = vis_to_double(c1 >> 16, c1)

#define	DM_0999	d_mask_0999 = vis_to_double(c2 >> 16, c2)

#define	DM_1110	d_mask_1110 = vis_to_double(c1, c1 << 16)

#define	DM_9990	d_mask_9990 = vis_to_double(c2, c2 << 16)

#define	DM_1199	d_mask_1199 = vis_to_double(c1, c2)

#define	DM_9911	d_mask_9911 = vis_to_double(c2, c1)

/* *********************************************************** */

#define	DM_1199_4	d_mask_1199_4 = vis_to_double(coef[2], coef[3])
#define	DM_9911_4	d_mask_9911_4 = vis_to_double(coef[3], coef[2])

#define	DM_9_9_81_81_4	d_mask_9_9_81_81_4 = vis_to_double(coef[3], coef[4])

#define	DM_81_81_9_9_4	d_mask_81_81_9_9_4 = vis_to_double(coef[4], coef[3])

/* *********************************************************** */

#define	DMF_10101010	f_mask_10101010 = vis_to_float(coef[5])
#define	DMF_90909090	f_mask_90909090 = vis_to_float(coef[6])

#define	DMF_10101000	f_mask_10101000 = vis_to_float(coef[5] << 8)

#define	DMF_90909000	f_mask_90909000 = vis_to_float(coef[6] << 8)

#define	DMF_10109090	f_mask_10109090 = vis_to_float(MERGE(coef[5], coef[6]))

#define	DMF_90901010	f_mask_90901010 = vis_to_float(MERGE(coef[6], coef[5]))

/* *********************************************************** */

#define	MASK_DEFINITION_U8_2                                    \
	LOAD_COEF();                                            \
	mlib_d64 DM_9_9_81_81_4,                                \
		DM_81_81_9_9_4,                                 \
		DM_1199,                                        \
		DM_9911,                                        \
		DM_1199_4,                                      \
		DM_9911_4

/* *********************************************************** */

#define	MASK_DEFINITION_U8_2XY                                  \
	LOAD_COEF();                                            \
	mlib_d64 DM_1111,                                       \
		DM_9999;                                        \
	mlib_f32 DMF_10109090,                                  \
		DMF_90901010,                                   \
		DMF_0000

/* *********************************************************** */

#define	MASK_DEFINITION_U8_3X                                   \
	LOAD_COEF();                                            \
	mlib_d64 DM_0111,                                       \
		DM_0999,                                        \
		DM_1110,                                        \
		DM_9990,                                        \
		DM_000XXXXX;                                    \
	mlib_f32 DMF_X000

/* *********************************************************** */

#define	MASK_DEFINITION_U8_3XY                                  \
	LOAD_COEF();                                            \
	mlib_d64 DM_1110,                                       \
		DM_9990,                                        \
		DM_0111,                                        \
		DM_0999,                                        \
		DM_9999,                                        \
		DM_1111;                                        \
	mlib_f32 DMF_10101000,                                  \
		DMF_90909000,                                   \
		DMF_0000

/* *********************************************************** */

#define	MASK_DEFINITION_U8_4                                    \
	LOAD_COEF();                                            \
	mlib_d64 DM_1111,                                       \
		DM_9999;                                        \
	mlib_f32 DMF_10101010,                                  \
		DMF_90909090,                                   \
		DMF_0000

/* *********************************************************** */

#define	CHAN	2
#define	CHAN_2	4

#define	CHAN_4	8

/* *********************************************************** */

#define	BICUBIC_U8_2_X(d_s_1v_1w, d_s_1v_2w, d_s_2v_1w,                   \
	d_s_2v_2w, d_dest)                                                \
	_d_s16_odd_1 =                                                    \
	vis_fmul8x16(vis_read_hi(d_s_1v_1w), d_mask_1199);                \
	_d_s16_odd_2 =                                                    \
	vis_fmul8x16(vis_read_lo(d_s_1v_1w), d_mask_9911);                \
	_d_s16_odd_3 =                                                    \
	vis_fmul8x16(vis_read_hi(d_s_1v_2w), d_mask_1199);                \
	_d_s16_odd_4 =                                                    \
	vis_fmul8x16(vis_read_lo(d_s_1v_2w), d_mask_9911);                \
	_d_s16_even_1 =                                                   \
	vis_fmul8x16(vis_read_hi(d_s_2v_1w), d_mask_1199);                \
	_d_s16_even_2 =                                                   \
	vis_fmul8x16(vis_read_lo(d_s_2v_1w), d_mask_9911);                \
	_d_s16_even_3 =                                                   \
	vis_fmul8x16(vis_read_hi(d_s_2v_2w), d_mask_1199);                \
	_d_s16_even_4 =                                                   \
	vis_fmul8x16(vis_read_lo(d_s_2v_2w), d_mask_9911);                \
	_d_s16_1_common = vis_fpsub16(_d_s16_odd_1, _d_s16_odd_2);        \
	_d_s16_2_common = vis_fpsub16(_d_s16_odd_3, _d_s16_odd_4);        \
	_d_s16_3_common = vis_fpsub16(_d_s16_even_1, _d_s16_even_2);      \
	_d_s16_4_common = vis_fpsub16(_d_s16_even_3, _d_s16_even_4);      \
	vis_alignaddr(d_p_str_src, 4);                                    \
	_d_s16_1_shift =                                                  \
	vis_faligndata(_d_s16_1_common, _d_s16_1_common);                 \
	_d_s16_2_shift =                                                  \
	vis_faligndata(_d_s16_2_common, _d_s16_2_common);                 \
	_d_s16_3_shift =                                                  \
	vis_faligndata(_d_s16_3_common, _d_s16_3_common);                 \
	_d_s16_4_shift =                                                  \
	vis_faligndata(_d_s16_4_common, _d_s16_4_common);                 \
	_d_s16_1_finish = vis_fpsub16(_d_s16_1_shift, _d_s16_1_common);   \
	_d_s16_2_finish = vis_fpsub16(_d_s16_2_shift, _d_s16_2_common);   \
	_d_s16_3_finish = vis_fpsub16(_d_s16_3_shift, _d_s16_3_common);   \
	_d_s16_4_finish = vis_fpsub16(_d_s16_4_shift, _d_s16_4_common);   \
	_d_s16____1w =                                                    \
	vis_freg_pair(vis_read_hi(_d_s16_1_finish),                       \
	    vis_read_hi(_d_s16_3_finish));                                \
	_d_s16____2w =                                                    \
	vis_freg_pair(vis_read_hi(_d_s16_2_finish),                       \
	    vis_read_hi(_d_s16_4_finish));                                \
	d_dest = vis_fpack16_pair(_d_s16____1w, _d_s16____2w)

/* *********************************************************** */

#define	BICUBIC_U8_2_Y(d_s_1w, d_s_2w, d_s_3w, d_s_4w, f_dest)             \
	d_s_1w = vis_fpmerge(vis_read_hi(d_s_1w), vis_read_lo(d_s_1w));    \
	d_s_2w = vis_fpmerge(vis_read_hi(d_s_2w), vis_read_lo(d_s_2w));    \
	d_s_3w = vis_fpmerge(vis_read_hi(d_s_3w), vis_read_lo(d_s_3w));    \
	d_s_4w = vis_fpmerge(vis_read_hi(d_s_4w), vis_read_lo(d_s_4w));    \
	_d_s16_help_1 = vis_fmul8x16(vis_read_hi(d_s_1w), d_mask_1111);    \
	_d_s16_help_2 = vis_fmul8x16(vis_read_hi(d_s_2w), d_mask_9999);    \
	_d_s16_help_3 = vis_fmul8x16(vis_read_hi(d_s_3w), d_mask_9999);    \
	_d_s16_help_4 = vis_fmul8x16(vis_read_hi(d_s_4w), d_mask_1111);    \
	_d_s16_help_1 = vis_fpadd16(_d_s16_help_1, _d_s16_help_4);         \
	_d_s16_help_2 = vis_fpadd16(_d_s16_help_2, _d_s16_help_3);         \
	_d_s16_help_2 = vis_fpsub16(_d_s16_help_2, _d_s16_help_1);         \
	f_dest = vis_fpack16(_d_s16_help_2);                               \
	_d_s16_help_1 = vis_fpmerge(f_dest, vis_fzeros());                 \
	_d_s16_help_2 = vis_fpmerge(vis_fzeros(), f_dest);                 \
	f_dest =                                                           \
	vis_fors(vis_read_hi(_d_s16_help_1), vis_read_lo(_d_s16_help_2))

/* *********************************************************** */

#define	BICUBIC_U8_2_XY_HELP(d_s_1w, d_s_2w, d_s_3w, d_s_4w,              \
	f_dest)                                                           \
	_d_s16_1w_1 = vis_fmul8x16(vis_read_hi(d_s_1w), d_mask_1199_4);   \
	_d_s16_1w_2 = vis_fmul8x16(vis_read_lo(d_s_1w), d_mask_9911_4);   \
	_d_s16_2w_1 =                                                     \
	vis_fmul8x16(vis_read_hi(d_s_2w), d_mask_9_9_81_81_4);            \
	_d_s16_2w_2 =                                                     \
	vis_fmul8x16(vis_read_lo(d_s_2w), d_mask_81_81_9_9_4);            \
	_d_s16_3w_1 =                                                     \
	vis_fmul8x16(vis_read_hi(d_s_3w), d_mask_9_9_81_81_4);            \
	_d_s16_3w_2 =                                                     \
	vis_fmul8x16(vis_read_lo(d_s_3w), d_mask_81_81_9_9_4);            \
	_d_s16_4w_1 = vis_fmul8x16(vis_read_hi(d_s_4w), d_mask_1199_4);   \
	_d_s16_4w_2 = vis_fmul8x16(vis_read_lo(d_s_4w), d_mask_9911_4);   \
	_d_s16_1w = vis_fpsub16(_d_s16_1w_1, _d_s16_1w_2);                \
	_d_s16_2w = vis_fpsub16(_d_s16_2w_1, _d_s16_2w_2);                \
	_d_s16_3w = vis_fpsub16(_d_s16_3w_1, _d_s16_3w_2);                \
	_d_s16_4w = vis_fpsub16(_d_s16_4w_1, _d_s16_4w_2);                \
	_d_s16_1w4 = vis_fpadd16(_d_s16_1w, _d_s16_4w);                   \
	_d_s16_2w3 = vis_fpadd16(_d_s16_2w, _d_s16_3w);                   \
	_d_s16_1234w = vis_fpsub16(_d_s16_1w4, _d_s16_2w3);               \
	f_dest =                                                          \
	vis_fpsub16s(vis_read_hi(_d_s16_1234w),                           \
	    vis_read_lo(_d_s16_1234w))

/* *********************************************************** */

#define	BICUBIC_U8_2_XY(d_s_1w1, d_s_2w1, d_s_3w1, d_s_4w1,          \
	d_s_1w2, d_s_2w2, d_s_3w2, d_s_4w2, f_dest)                  \
	BICUBIC_U8_2_XY_HELP(d_s_1w1, d_s_2w1, d_s_3w1, d_s_4w1,     \
	    _f_help_1);                                              \
	d_s_1w1 =                                                    \
	vis_freg_pair(vis_read_lo(d_s_1w1), vis_read_hi(d_s_1w2));   \
	d_s_2w1 =                                                    \
	vis_freg_pair(vis_read_lo(d_s_2w1), vis_read_hi(d_s_2w2));   \
	d_s_3w1 =                                                    \
	vis_freg_pair(vis_read_lo(d_s_3w1), vis_read_hi(d_s_3w2));   \
	d_s_4w1 =                                                    \
	vis_freg_pair(vis_read_lo(d_s_4w1), vis_read_hi(d_s_4w2));   \
	BICUBIC_U8_2_XY_HELP(d_s_1w1, d_s_2w1, d_s_3w1, d_s_4w1,     \
	    _f_help_2);                                              \
	_d_s16_help = vis_freg_pair(_f_help_1, _f_help_2);           \
	f_dest = vis_fpack16(_d_s16_help)

/* *********************************************************** */

#define	BICUBIC_U8_2_XY_PREV(d_0, d_1, d_2, d_3, dst)            \
	_d_0_v1 = vis_fpmerge(f_mask_0000, vis_read_lo(d_0));    \
	_d_1_v1 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9999);   \
	_d_3_v1 = vis_fpmerge(f_mask_0000, vis_read_lo(d_3));    \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	dst = vis_fpsub16(_d_12_v1, _d_03_v1)

/* *********************************************************** */

#define	BICUBIC_U8_2_XY_COMMON_HELP(d_0, d_1, d_2, d_3,          \
	dst_1, dst_2)                                            \
	_d_0_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_0));    \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_9999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_9999);   \
	_d_3_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_3));    \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	dst_1 = vis_fpsub16(_d_12_v1, _d_03_v1);                 \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_1111);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9999);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9999);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_1111);   \
	_d_03_v2 = vis_fpadd16(_d_0_v2, _d_3_v2);                \
	_d_12_v2 = vis_fpadd16(_d_1_v2, _d_2_v2);                \
	dst_2 = vis_fpsub16(_d_12_v2, _d_03_v2)

/* *********************************************************** */

#define	BICUBIC_U8_2_XY_COMMON_COUNT(d_0, d_1, d_2, dst)              \
	_d_0_v1 = vis_fmul8x16(f_mask_10109090, d_0);                 \
	_d_3_v1 = vis_fmul8x16(f_mask_90901010, d_1);                 \
	_d_03_v1 = vis_fpsub16(_d_3_v1, _d_0_v1);                     \
	f_0 =                                                         \
	vis_fpsub16s(vis_read_hi(_d_03_v1), vis_read_lo(_d_03_v1));   \
	_d_0_v2 = vis_fmul8x16(f_mask_10109090, d_1);                 \
	_d_3_v2 = vis_fmul8x16(f_mask_90901010, d_2);                 \
	_d_03_v2 = vis_fpsub16(_d_3_v2, _d_0_v2);                     \
	f_1 =                                                         \
	vis_fpsub16s(vis_read_hi(_d_03_v2), vis_read_lo(_d_03_v2));   \
	dst = vis_freg_pair(f_0, f_1)

/* *********************************************************** */

#define	BICUBIC_U8_2_XY_DOUBLE_COMMON(d_0_0, d_0_1, d_0_2,        \
	d_0_3, d_1_0, d_1_1, d_1_2, d_1_3, d_dest, prev)          \
	BICUBIC_U8_2_XY_COMMON_HELP(d_0_0, d_0_1, d_0_2, d_0_3,   \
	    _d_s16_3, _d_s16_4);                                  \
	BICUBIC_U8_2_XY_COMMON_COUNT(prev, _d_s16_3, _d_s16_4,    \
	    _d_s16____1w);                                        \
	prev = _d_s16_4;                                          \
	BICUBIC_U8_2_XY_COMMON_HELP(d_1_0, d_1_1, d_1_2, d_1_3,   \
	    _d_s16_3, _d_s16_4);                                  \
	BICUBIC_U8_2_XY_COMMON_COUNT(prev, _d_s16_3, _d_s16_4,    \
	    _d_s16____2w);                                        \
	prev = _d_s16_4;                                          \
	d_dest = vis_fpack16_pair(_d_s16____1w, _d_s16____2w)

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Bicubic_X(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_d64 _d_s16_odd_1, _d_s16_odd_2,
	    _d_s16_odd_3, _d_s16_odd_4,
	    _d_s16_even_1, _d_s16_even_2, _d_s16_even_3, _d_s16_even_4;
	mlib_d64 _d_s16_1_common, _d_s16_2_common, _d_s16_3_common,
	    _d_s16_4_common, _d_s16_1_finish, _d_s16_2_finish, _d_s16_3_finish,
	    _d_s16_4_finish, _d_s16_1_shift, _d_s16_2_shift, _d_s16_3_shift,
	    _d_s16_4_shift;
	mlib_d64 _d_s16____1w, _d_s16____2w;

	mlib_d64 d_s_1w_hi, d_s_1w_lo, d_s_2w_hi, d_s_2w_lo, d_dest;
	mlib_d64 *d_p_str_src, *d_p_str_dst, *d_p_str_src_1, *d_p_str_src_2;

	MASK_DEFINITION_U8_2;

	mlib_d64 d_s_1v_1w, d_s_1v_2w, d_s_2v_1w, d_s_2v_2w;

	mlib_s32 off, emask;
	mlib_s32 src_stride_2 = src_stride << 1;
	mlib_u8 *p_str_src, *p_str_dst, *p_str_dst_end;

	mlib_s32 i, j;
	mlib_s32 res;

	vis_write_gsr((7 - BC_SCL) << 3);
	d_p_str_src = 0;

	if (((((mlib_addr)p_dst) | dst_stride) & 1) == 0) {
		for (i = 0; i < dst_height; i++) {
			p_str_src = p_src + i * src_stride_2 - 2; /* -1 */
			p_str_dst = p_dst + i * dst_stride;
			p_str_dst_end = p_str_dst + (dst_width << 1) - 1;

			d_p_str_dst =
			    (mlib_d64 *)(((mlib_addr)p_str_dst) & (~7));
			off = (mlib_addr)d_p_str_dst - (mlib_addr)p_str_dst;

			d_p_str_src_1 =
			    (mlib_d64 *)vis_alignaddr(p_str_src, off + off);

			d_s_1w_hi = d_p_str_src_1[0];
			d_s_1w_lo = d_p_str_src_1[1];
			d_s_2w_hi = d_p_str_src_1[2];
			d_s_2w_lo = d_p_str_src_1[3];
			d_s_1v_1w = vis_faligndata(d_s_1w_hi, d_s_1w_lo);
			d_s_1v_2w = vis_faligndata(d_s_1w_lo, d_s_2w_hi);
			d_p_str_src_2 =
			    (mlib_d64 *)vis_alignaddr(p_str_src, off + off + 4);

			if (d_p_str_src_2 == d_p_str_src_1) {
				d_s_2v_1w =
				    vis_faligndata(d_s_1w_hi, d_s_1w_lo);
				d_s_2v_2w =
				    vis_faligndata(d_s_1w_lo, d_s_2w_hi);
			} else {
				d_s_2v_1w =
				    vis_faligndata(d_s_1w_lo, d_s_2w_hi);
				d_s_2v_2w =
				    vis_faligndata(d_s_2w_hi, d_s_2w_lo);
			}

			BICUBIC_U8_2_X(d_s_1v_1w, d_s_1v_2w, d_s_2v_1w,
			    d_s_2v_2w, d_dest);
			emask = vis_edge8(p_str_dst, p_str_dst_end);
/* store first bytes of result */
			vis_pst_8(d_dest, d_p_str_dst, emask);
			d_p_str_dst++;
			d_p_str_src = d_p_str_src_1 + 2;

			if (d_p_str_src_2 == d_p_str_src_1) {
#pragma pipeloop(0)
				for (j = 8 + off; j <= ((dst_width << 1) - 8);
				    j += 8) {
					d_s_1w_hi = d_s_2w_hi;
					d_s_1w_lo = d_p_str_src[1];
					d_s_2w_hi = d_p_str_src[2];

					vis_alignaddr(p_str_src, off + off);
					d_s_1v_1w =
					    vis_faligndata(d_s_1w_hi,
					    d_s_1w_lo);
					d_s_1v_2w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					vis_alignaddr(p_str_src, off + off + 4);
					d_s_2v_1w =
					    vis_faligndata(d_s_1w_hi,
					    d_s_1w_lo);
					d_s_2v_2w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);

					BICUBIC_U8_2_X(d_s_1v_1w, d_s_1v_2w,
					    d_s_2v_1w, d_s_2v_2w, d_dest);
					*d_p_str_dst = d_dest;
					d_p_str_dst++;
					d_p_str_src += 2;
				}

				if (j < (dst_width << 1)) {
					d_s_1w_hi = d_s_2w_hi;
					d_s_1w_lo = d_p_str_src[1];
					d_s_2w_hi = d_p_str_src[2];

					vis_alignaddr(p_str_src, off + off);
					d_s_1v_1w =
					    vis_faligndata(d_s_1w_hi,
					    d_s_1w_lo);
					d_s_1v_2w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					vis_alignaddr(p_str_src, off + off + 4);
					d_s_2v_1w =
					    vis_faligndata(d_s_1w_hi,
					    d_s_1w_lo);
					d_s_2v_2w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					BICUBIC_U8_2_X(d_s_1v_1w, d_s_1v_2w,
					    d_s_2v_1w, d_s_2v_2w, d_dest);
					emask =
					    vis_edge8(d_p_str_dst,
					    p_str_dst_end);
/* store last bytes of result */
					vis_pst_8(d_dest, d_p_str_dst, emask);
				}

			} else {
#pragma pipeloop(0)
				for (j = 8 + off; j <= ((dst_width << 1) - 8);
				    j += 8) {
					d_s_1w_hi = d_s_2w_hi;
					d_s_1w_lo = d_s_2w_lo;
					d_s_2w_hi = d_p_str_src[2];
					d_s_2w_lo = d_p_str_src[3];
					vis_alignaddr(p_str_src, off + off);
					d_s_1v_1w =
					    vis_faligndata(d_s_1w_hi,
					    d_s_1w_lo);
					d_s_1v_2w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					vis_alignaddr(p_str_src, off + off + 4);
					d_s_2v_1w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					d_s_2v_2w =
					    vis_faligndata(d_s_2w_hi,
					    d_s_2w_lo);
					BICUBIC_U8_2_X(d_s_1v_1w, d_s_1v_2w,
					    d_s_2v_1w, d_s_2v_2w, d_dest);
					*d_p_str_dst = d_dest;
					d_p_str_dst++;
					d_p_str_src += 2;
				}

				if (j < (dst_width << 1)) {
					d_s_1w_hi = d_s_2w_hi;
					d_s_1w_lo = d_s_2w_lo;
					d_s_2w_hi = d_p_str_src[2];
					d_s_2w_lo = d_p_str_src[3];
					vis_alignaddr(p_str_src, off + off);
					d_s_1v_1w =
					    vis_faligndata(d_s_1w_hi,
					    d_s_1w_lo);
					d_s_1v_2w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					vis_alignaddr(p_str_src, off + off + 4);
					d_s_2v_1w =
					    vis_faligndata(d_s_1w_lo,
					    d_s_2w_hi);
					d_s_2v_2w =
					    vis_faligndata(d_s_2w_hi,
					    d_s_2w_lo);
					BICUBIC_U8_2_X(d_s_1v_1w, d_s_1v_2w,
					    d_s_2v_1w, d_s_2v_2w, d_dest);
					emask =
					    vis_edge8(d_p_str_dst,
					    p_str_dst_end);
/* store last bytes of result */
					vis_pst_8(d_dest, d_p_str_dst, emask);
				}
			}
		}

		return;
	}

	for (i = 0; i < dst_height; i++) {
		mlib_s32 st0, st1, st2, st3, res2;
		mlib_u8 *p_str_src = p_src, *p_str_dst = p_dst;

		st0 = p_str_src[-CHAN];
		st2 = p_str_src[1 - CHAN];
		st1 = p_str_src[0];
		st3 = p_str_src[1];

		for (j = 0; j < dst_width; j++) {
			res = BC_CF(st1) - st0;
			res2 = BC_CF(st3) - st2;

			st0 = p_str_src[CHAN];
			st2 = p_str_src[1 + CHAN];
			st1 = p_str_src[CHAN_2];
			st3 = p_str_src[1 + CHAN_2];

			res = (res + BC_CF(st0) - st1) >> BC_SCL;
			res2 = (res2 + BC_CF(st2) - st3) >> BC_SCL;

			p_str_dst[0] = SAT_U8(res);
			p_str_dst[1] = SAT_U8(res2);

			p_str_src += CHAN_2;
			p_str_dst += CHAN;
		}

		p_src += src_stride_2;
		p_dst += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Bicubic_XY(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_d64 _d_s16_help;
	mlib_d64 _d_s16_1w, _d_s16_2w, _d_s16_3w, _d_s16_4w;

	mlib_d64 _d_s16_1w_1, _d_s16_1w_2, _d_s16_2w_1, _d_s16_2w_2,
	    _d_s16_3w_1, _d_s16_3w_2, _d_s16_4w_1, _d_s16_4w_2;
	mlib_d64 _d_s16_1w4, _d_s16_2w3, _d_s16_1234w;
	mlib_d64 *d_p_str_src;

	MASK_DEFINITION_U8_2;

	mlib_d64 d_s_1w1, d_s_1w2, d_s_2w1, d_s_2w2, d_s_3w1, d_s_3w2, d_s_4w1,
	    d_s_4w2;
	mlib_f32 f_dest, _f_help_1, _f_help_2;
	mlib_s32 src_stride_4, src_stride_8;

	mlib_s32 beg;
	mlib_s32 src_stride_2 = src_stride << 1;

	mlib_s32 i, j, l;
	mlib_s32 res;

	if (((dst_stride & 3) == 0) || (dst_width <= 4)) {
		beg = (dst_width <= 4) ? dst_width : (4 - (mlib_addr)p_dst) & 3;

		if (beg > 0) {
			for (i = 0; i < dst_height; i++) {
				for (l = 0; l < CHAN; l++) {
					mlib_u8 *p_str_src =
					    p_src + i * src_stride_2 + l,
					    *p_str_dst =
					    p_dst + i * dst_stride + l;
					mlib_s32 st0, st1;

					st0 = BC_CF(p_str_src[-CHAN] +
					    p_str_src[src_stride - CHAN]) -
					    p_str_src[-src_stride - CHAN] -
					    p_str_src[src_stride_2 - CHAN];
					st1 = BC_CF(p_str_src[0] +
					    p_str_src[src_stride]) -
					    p_str_src[-src_stride] -
					    p_str_src[src_stride_2];
					for (j = 0; j < beg; j++) {
						res = BC_CF(st1) - st0;
						p_str_src += CHAN;
						st0 = BC_CF(p_str_src[0] +
						    p_str_src[src_stride]) -
						    p_str_src[-src_stride] -
						    p_str_src[src_stride_2];
						p_str_src += CHAN;
						st1 = BC_CF(p_str_src[0] +
						    p_str_src[src_stride]) -
						    p_str_src[-src_stride] -
						    p_str_src[src_stride_2];
						res =
						    (res + BC_CF(st0) -
						    st1) >> BC_SC2;
						*p_str_dst = SAT_U8(res);
						p_str_dst += CHAN;
					}
				}
			}
		}

		dst_width -= beg;
		p_src += CHAN_2 * beg;
		p_dst += CHAN * beg;
	}

	if ((((((mlib_addr)p_dst) | dst_stride) & 3) |	/* aligned case */
	    ((((mlib_addr)p_src - 2) | src_stride) & 7)) == 0) {
		mlib_f32 *f_p_str_dst;

		vis_write_gsr(1 << 3);

		src_stride_4 = src_stride >> 2;
		src_stride_8 = src_stride >> 3;

		for (i = 0; i < dst_height; i++) {
			d_p_str_src =
			    (mlib_d64 *)(p_src + i * src_stride_2 - 2);
			f_p_str_dst = (mlib_f32 *)(p_dst + i * dst_stride);
#pragma pipeloop(0)
			for (j = 0; j <= dst_width - 2; j += 2) {
				d_s_1w1 = d_p_str_src[-src_stride_8];
				d_s_2w1 = d_p_str_src[0];
				d_s_3w1 = d_p_str_src[src_stride_8];
				d_s_4w1 = d_p_str_src[src_stride_4];
				d_s_1w2 = d_p_str_src[-src_stride_8 + 1];
				d_s_2w2 = d_p_str_src[1];
				d_s_3w2 = d_p_str_src[src_stride_8 + 1];
				d_s_4w2 = d_p_str_src[src_stride_4 + 1];

				BICUBIC_U8_2_XY(d_s_1w1, d_s_2w1, d_s_3w1,
				    d_s_4w1, d_s_1w2, d_s_2w2, d_s_3w2, d_s_4w2,
				    f_dest);
				f_p_str_dst[0] = f_dest;
				d_p_str_src++;
				f_p_str_dst++;
			}

			if (j < dst_width) { /* Finish count */
				for (l = 0; l < CHAN; l++) {
					mlib_u8 *p_str_src =
					    p_src + i * src_stride_2 + l +
					    CHAN_2 * j, *p_str_dst =
					    p_dst + i * dst_stride + l +
					    CHAN * j;
					mlib_s32 st0, st1;

					st0 = BC_CF(p_str_src[-CHAN] +
					    p_str_src[src_stride - CHAN]) -
					    p_str_src[-src_stride - CHAN] -
					    p_str_src[src_stride_2 - CHAN];
					st1 = BC_CF(p_str_src[0] +
					    p_str_src[src_stride]) -
					    p_str_src[-src_stride] -
					    p_str_src[src_stride_2];
					res = BC_CF(st1) - st0;
					p_str_src += CHAN;
					st0 = BC_CF(p_str_src[0] +
					    p_str_src[src_stride]) -
					    p_str_src[-src_stride] -
					    p_str_src[src_stride_2];
					p_str_src += CHAN;
					st1 = BC_CF(p_str_src[0] +
					    p_str_src[src_stride]) -
					    p_str_src[-src_stride] -
					    p_str_src[src_stride_2];
					res =
					    (res + BC_CF(st0) - st1) >> BC_SC2;
					*p_str_dst = SAT_U8(res);
					p_str_dst += CHAN;
				}
			}
		}

		return;
	}

	mlib_ImageZoomOut2X_U8_2_Bicubic_XY_NotAl(PARAM1, coef);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Bicubic_XY_NotAl(
    NEAREST_PARAMETERS1(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1,
	    _d_0_v2, _d_1_v2, _d_2_v2, _d_3_v2,
	    _d_03_v1, _d_12_v1, _d_03_v2, _d_12_v2;
	mlib_d64 _d_s16____1w, _d_s16____2w;
	mlib_d64 _d_s16_3, _d_s16_4;

	MASK_DEFINITION_U8_2XY;

	mlib_d64 *d_address, *s_address;
	mlib_d64 s_1, result;
	mlib_d64 d_prev;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;
	mlib_d64 s_help_0, s_help_1, s_help_2, s_help_3;
	mlib_f32 f_result, f_0, f_1;
	mlib_s32 i, j;
	mlib_s32 help_d_width = d_width - 4, s_stride_2 = s_stride << 1;
	mlib_u8 *s_line, *d_line, *d_end, *help;
	mlib_u8 *s_begin;
	mlib_u8 emask, no_emask;
	void *u_align;

	vis_write_gsr((7 - BC_SCL) << 3);

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;

		d_address = (mlib_d64 *)vis_alignaddr(d_line, 0);
		emask = vis_edge8(d_line, d_address + 1);
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		s_begin -= 6;

		s_address = (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
		s_1 = vis_ld_d64_nf(s_address);
		s_help_0 = s_address[1];
		s_dd00 = vis_faligndata(s_1, s_help_0);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
		s_1 = s_address[0];
		s_help_1 = s_address[1];
		s_dd10 = vis_faligndata(s_1, s_help_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
		s_1 = s_address[0];
		s_help_2 = s_address[1];
		s_dd20 = vis_faligndata(s_1, s_help_2);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
		s_1 = s_address[0];
		s_help_3 = s_address[1];
		s_dd30 = vis_faligndata(s_1, s_help_3);

		BICUBIC_U8_2_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30, d_prev);
		s_begin += 8;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 4) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride);
				s_1 = s_address[1];
				s_dd00 = vis_faligndata(s_help_0, s_1);
				s_help_0 = s_address[2];
				s_dd01 = vis_faligndata(s_1, s_help_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					0);
				s_1 = s_address[1];
				s_dd10 = vis_faligndata(s_help_1, s_1);
				s_help_1 = s_address[2];
				s_dd11 = vis_faligndata(s_1, s_help_1);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride);
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_help_2, s_1);
				s_help_2 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_help_2);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_2);
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_help_3, s_1);
				s_help_3 = vis_ld_d64_nf(s_address + 2);
				s_dd31 = vis_faligndata(s_1, s_help_3);

				s_begin += 16;

				BICUBIC_U8_2_XY_DOUBLE_COMMON(s_dd00, s_dd10,
					s_dd20, s_dd30, s_dd01, s_dd11, s_dd21,
					s_dd31, result, d_prev);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address++;
			}
		} else {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 4) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride);
				s_1 = s_address[1];
				s_dd00 = vis_faligndata(s_help_0, s_1);
				s_help_0 = s_address[2];
				s_dd01 = vis_faligndata(s_1, s_help_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					0);
				s_1 = s_address[1];
				s_dd10 = vis_faligndata(s_help_1, s_1);
				s_help_1 = s_address[2];
				s_dd11 = vis_faligndata(s_1, s_help_1);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride);
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_help_2, s_1);
				s_help_2 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_help_2);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_2);
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_help_3, s_1);
				s_help_3 = vis_ld_d64_nf(s_address + 2);
				s_dd31 = vis_faligndata(s_1, s_help_3);

				s_begin += 16;

				BICUBIC_U8_2_XY_DOUBLE_COMMON(s_dd00, s_dd10,
					s_dd20, s_dd30, s_dd01, s_dd11, s_dd21,
					s_dd31, result, d_prev);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address++;
				vis_pst_8(result, d_address, no_emask);
			}
		}

		if (i <= d_width - 2) {
			d_end = d_line + (i << 1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
			s_1 = s_address[1];
			s_dd00 = vis_faligndata(s_help_0, s_1);
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_1 = s_address[1];
			s_dd10 = vis_faligndata(s_help_1, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_help_2, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd30 = vis_faligndata(s_help_3, s_1);

			s_begin += 16;

			BICUBIC_U8_2_XY_COMMON_HELP(s_dd00, s_dd10, s_dd20,
			    s_dd30, _d_s16_3, _d_s16_4);
			BICUBIC_U8_2_XY_COMMON_COUNT(d_prev, _d_s16_3, _d_s16_4,
			    _d_s16____1w);
			d_prev = _d_s16_4;
			f_result = vis_fpack16(_d_s16____1w);
			help = (mlib_u8 *)&f_result;
			d_end[0] = help[0];
			d_end[1] = help[1];
			d_end[2] = help[2];
			d_end[3] = help[3];
			i += 2;
		}

		if (i < d_width) {
			mlib_s32 l, res;

			for (l = 0; l < CHAN; l++) {
				mlib_u8 *ps = s_line + CHAN_2 * i + l,
				    *pd = d_line + CHAN * i + l;
				mlib_s32 st0 =
				    BC_CF(ps[-CHAN] + ps[s_stride - CHAN]) -
				    ps[-s_stride - CHAN] - ps[s_stride_2 -
				    CHAN], st1 =
				    BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];
#pragma pipeloop(0)
				res = BC_CF(st1) - st0;
				ps += CHAN;
				st0 = BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];
				ps += CHAN;
				st1 = BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];
				res = (res + BC_CF(st0) - st1) >> BC_SC2;
				pd[0] = SAT_U8(res);
				pd += CHAN;
			}
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

#undef CHAN
#undef CHAN_2

/* *********************************************************** */

#define	CHAN	3
#define	CHAN_2	6

/* *********************************************************** */

#define	BICUBIC_U8_3_PIXEL_X_HELP(_d_0, _d_1)                     \
	_d_0_v1 = vis_fmul8x16(vis_read_hi(_d_0), d_mask_0111);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(_d_1), d_mask_0999);   \
	_d_02_v1 = vis_fpsub16(_d_2_v1, _d_0_v1);                 \
	_d_1_v1 = vis_fmul8x16(vis_read_lo(_d_0), d_mask_9990);   \
	_d_3_v1 = vis_fmul8x16(vis_read_lo(_d_1), d_mask_1110);   \
	_d_13_v1 = vis_fpsub16(_d_1_v1, _d_3_v1);

/* *********************************************************** */

#define	BICUBIC_U8_3_RIGHT_PIXEL_X_HELP(_d_0, _d_1, f_dest)     \
	BICUBIC_U8_3_PIXEL_X_HELP(_d_0, _d_1);                  \
	_d_13_v1 = vis_faligndata(_d_13_v1, _d_13_v1);          \
	_d_02_v1 = vis_fpadd16(_d_02_v1, _d_13_v1);             \
	f_dest = vis_fpack16(_d_02_v1)

/* *********************************************************** */

#define	BICUBIC_U8_3_LEFT_PIXEL_X_HELP(_d_0, _d_1, f_dest)      \
	BICUBIC_U8_3_PIXEL_X_HELP(_d_0, _d_1);                  \
	_d_02_v1 = vis_faligndata(_d_02_v1, _d_02_v1);          \
	_d_02_v1 = vis_fpadd16(_d_02_v1, _d_13_v1);             \
	f_dest = vis_fpack16(_d_02_v1)

/* *********************************************************** */

#define	BICUBIC_U8_3_X(_d_0, _d_1, _d_2, _d_3, _d_4, d_dst, f_dst)         \
	vis_alignaddr(d_p_str_src, 2);                                     \
	BICUBIC_U8_3_LEFT_PIXEL_X_HELP(_d_0, _d_1, _f_s16____1w);          \
	BICUBIC_U8_3_LEFT_PIXEL_X_HELP(_d_2, _d_3, _f_s16____3w);          \
	vis_alignaddr(d_p_str_src, 6);                                     \
	BICUBIC_U8_3_RIGHT_PIXEL_X_HELP(_d_1, _d_2, _f_s16____2w);         \
	_d_dst_help = vis_freg_pair(_f_s16____2w, _f_s16____3w);           \
	_d_dst_help = vis_faligndata(_d_dst_help, _d_dst_help);            \
	d_dst = vis_fand(_d_dst_help, d_mask_000XXXXX);                    \
	d_dst =                                                            \
	vis_write_hi(d_dst, vis_fors(vis_read_hi(d_dst), _f_s16____1w));   \
	BICUBIC_U8_3_RIGHT_PIXEL_X_HELP(_d_3, _d_4, _f_s16____4w);         \
	f_dst = vis_fands(vis_read_hi(_d_dst_help), f_mask_X000);          \
	f_dst = vis_fors(f_dst, _f_s16____4w);

/* *********************************************************** */

#define	BICUBIC_U8_3_DOUBLE_X(_d_0, _d_1, _d_2, _d_3, _d_4,           \
	_d_5, _d_6, _d_7, _d_8, dest_1, dest_2, dest_3)               \
	BICUBIC_U8_3_X(_d_0, _d_1, _d_2, _d_3, _d_4, dest_1, _f_0);   \
	BICUBIC_U8_3_X(_d_4, _d_5, _d_6, _d_7, _d_8, dest_3, _f_1);   \
	dest_2 = vis_freg_pair(_f_0, vis_read_hi(dest_3));            \
	dest_3 = vis_freg_pair(vis_read_lo(dest_3), _f_1)

/* *********************************************************** */

#define	BICUBIC_U8_3_Y_HELP_1(d_0, d_1, d_2, d_3, d_dst)         \
	_d_0_v1 = vis_fmul8x16(vis_read_hi(d_0), d_mask_1110);   \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_9990);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_9990);   \
	_d_3_v1 = vis_fmul8x16(vis_read_hi(d_3), d_mask_1110);   \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	_d_dst_help = vis_fpsub16(_d_12_v1, _d_03_v1);           \
	d_dst = vis_write_hi(d_dst, vis_fpack16(_d_dst_help));   \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_0011);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_0099);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_0099);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_0011);   \
	_d_03_v2 = vis_fpadd16(_d_0_v2, _d_3_v2);                \
	_d_12_v2 = vis_fpadd16(_d_1_v2, _d_2_v2);                \
	_d_dst_help = vis_fpsub16(_d_12_v2, _d_03_v2);           \
	d_dst = vis_write_lo(d_dst, vis_fpack16(_d_dst_help))

/* *********************************************************** */

#define	BICUBIC_U8_3_Y_HELP_2(d_0, d_1, d_2, d_3, d_dst)         \
	_d_0_v1 = vis_fmul8x16(vis_read_hi(d_0), d_mask_1000);   \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_9000);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_9000);   \
	_d_3_v1 = vis_fmul8x16(vis_read_hi(d_3), d_mask_1000);   \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	_d_dst_help = vis_fpsub16(_d_12_v1, _d_03_v1);           \
	d_dst = vis_write_hi(d_dst, vis_fpack16(_d_dst_help));   \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_1110);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9990);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9990);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_1110);   \
	_d_03_v2 = vis_fpadd16(_d_0_v2, _d_3_v2);                \
	_d_12_v2 = vis_fpadd16(_d_1_v2, _d_2_v2);                \
	_d_dst_help = vis_fpsub16(_d_12_v2, _d_03_v2);           \
	d_dst = vis_write_lo(d_dst, vis_fpack16(_d_dst_help))

/* *********************************************************** */

#define	BICUBIC_U8_3_Y_HELP_3(d_0, d_1, d_2, d_3, d_dst)         \
	_d_0_v1 = vis_fmul8x16(vis_read_hi(d_0), d_mask_0011);   \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_0099);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_0099);   \
	_d_3_v1 = vis_fmul8x16(vis_read_hi(d_3), d_mask_0011);   \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	_d_dst_help = vis_fpsub16(_d_12_v1, _d_03_v1);           \
	d_dst = vis_write_hi(d_dst, vis_fpack16(_d_dst_help));   \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_1000);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9000);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9000);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_1000);   \
	_d_03_v2 = vis_fpadd16(_d_0_v2, _d_3_v2);                \
	_d_12_v2 = vis_fpadd16(_d_1_v2, _d_2_v2);                \
	_d_dst_help = vis_fpsub16(_d_12_v2, _d_03_v2);           \
	d_dst = vis_write_lo(d_dst, vis_fpack16(_d_dst_help))

/* *********************************************************** */

#define	BICUBIC_U8_3_Y(_d_0_0, _d_0_1, _d_0_2, _d_1_0,                   \
	_d_1_1, _d_1_2, _d_2_0, _d_2_1, _d_2_2, _d_3_0, _d_3_1,          \
	_d_3_2, d_dest, f_dest)                                          \
	BICUBIC_U8_3_Y_HELP_1(_d_0_0, _d_1_0, _d_2_0, _d_3_0,            \
	    _d_help_0);                                                  \
	BICUBIC_U8_3_Y_HELP_2(_d_0_1, _d_1_1, _d_2_1, _d_3_1,            \
	    _d_help_1);                                                  \
	BICUBIC_U8_3_Y_HELP_3(_d_0_2, _d_1_2, _d_2_2, _d_3_2,            \
	    _d_help_2);                                                  \
	vis_alignaddr(d_p_str_src, 7);                                   \
	d_dest =                                                         \
	vis_freg_pair(vis_read_lo(_d_help_0), vis_read_hi(_d_help_1));   \
	d_dest = vis_faligndata(d_dest, d_dest);                         \
	vis_alignaddr(d_p_str_src, 6);                                   \
	_d_dst_help =                                                    \
	vis_freg_pair(f_mask_0000, vis_read_lo(_d_help_1));              \
	_d_dst_help = vis_faligndata(_d_dst_help, _d_dst_help);          \
	d_dest =                                                         \
	vis_freg_pair(vis_fors(vis_read_hi(d_dest),                      \
	    vis_read_hi(_d_help_0)), vis_fors(vis_read_lo(d_dest),       \
	    vis_read_lo(_d_dst_help)));                                  \
	vis_alignaddr(d_p_str_src, 1);                                   \
	_d_help_2 = vis_faligndata(_d_help_2, _d_help_2);                \
	f_dest =                                                         \
	vis_fors(vis_read_hi(_d_help_2), vis_read_hi(_d_dst_help))

/* *********************************************************** */

#define	BICUBIC_U8_3_XY_PREV(d_0, d_1, d_2, d_3, dst_1, dst_2)   \
	_d_0_v1 = vis_fmul8x16(vis_read_hi(d_0), d_mask_0111);   \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_0999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_0999);   \
	_d_3_v1 = vis_fmul8x16(vis_read_hi(d_3), d_mask_0111);   \
	dst_1 = vis_fpsub16(_d_1_v1, _d_0_v1);                   \
	dst_1 = vis_fpadd16(dst_1, _d_2_v1);                     \
	dst_1 = vis_fpsub16(dst_1, _d_3_v1);                     \
	dst_1 = vis_faligndata(dst_1, dst_1);                    \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_1110);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9990);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9990);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_1110);   \
	dst_2 = vis_fpsub16(_d_1_v2, _d_0_v2);                   \
	dst_2 = vis_fpadd16(dst_2, _d_2_v2);                     \
	dst_2 = vis_fpsub16(dst_2, _d_3_v2)

/* *********************************************************** */

#define	BICUBIC_U8_3_XY_COUNT_PREV(_d_src_1, _d_src_2, d_dst)   \
	_d_help_1 = vis_fmul8x16(f_mask_90909000, _d_src_1);    \
	d_dst = vis_fmul8x16(f_mask_10101000, _d_src_2);        \
	d_dst = vis_fpsub16(_d_help_1, d_dst)

/* *********************************************************** */

#define	BICUBIC_U8_4_XY_PREV(d_0, d_1, d_2, d_3, dst_1, dst_2)   \
	_d_0_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_0));    \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_9999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_9999);   \
	_d_3_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_3));    \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	dst_1 = vis_fpsub16(_d_12_v1, _d_03_v1);                 \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_1111);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9999);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9999);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_1111);   \
	_d_03_v2 = vis_fpadd16(_d_0_v2, _d_3_v2);                \
	_d_12_v2 = vis_fpadd16(_d_1_v2, _d_2_v2);                \
	dst_2 = vis_fpsub16(_d_12_v2, _d_03_v2)

/* *********************************************************** */

#define	BICUBIC_U8_3_XY(_d_0, _d_1, _d_2, _d_3, _d_4, _d_5,              \
	_d_6, _d_7, _d_8, _d_9, _d_10, _d_11, prev_1, prev_2,            \
	f_dest_0, f_dest_1, f_dest_2)                                    \
	BICUBIC_U8_4_XY_PREV(_d_0, _d_1, _d_2, _d_3, _d_dest_0,          \
	    _d_dest_1);                                                  \
	BICUBIC_U8_4_XY_PREV(_d_4, _d_5, _d_6, _d_7, _d_dest_2,          \
	    _d_dest_3);                                                  \
	BICUBIC_U8_4_XY_PREV(_d_8, _d_9, _d_10, _d_11, _d_dest_4,        \
	    _d_dest_5);                                                  \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_2, prev_1, _d_s16____1w);        \
	prev_1 = _d_dest_0;                                              \
	vis_alignaddr(d_p_str_src, 6);                                   \
	prev_2 = vis_faligndata(_d_dest_0, _d_dest_1);                   \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_1, prev_2, _d_s16____2w);        \
	d_dest = vis_fpadd16(_d_s16____1w, _d_s16____2w);                \
	_f_0 = vis_fpack16(d_dest);                                      \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_2, prev_1, _d_s16____1w);        \
	vis_alignaddr(d_p_str_src, 4);                                   \
	prev_1 = vis_faligndata(_d_dest_1, _d_dest_2);                   \
	vis_alignaddr(d_p_str_src, 2);                                   \
	prev_2 = vis_faligndata(_d_dest_2, _d_dest_2);                   \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_1, prev_2, _d_s16____2w);        \
	d_dest = vis_fpadd16(_d_s16____1w, _d_s16____2w);                \
	_f_1 = vis_fpack16(d_dest);                                      \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_2, prev_1, _d_s16____1w);        \
	prev_1 = _d_dest_3;                                              \
	vis_alignaddr(d_p_str_src, 6);                                   \
	prev_2 = vis_faligndata(_d_dest_3, _d_dest_4);                   \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_1, prev_2, _d_s16____2w);        \
	d_dest = vis_fpadd16(_d_s16____1w, _d_s16____2w);                \
	_f_2 = vis_fpack16(d_dest);                                      \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_2, prev_1, _d_s16____1w);        \
	vis_alignaddr(d_p_str_src, 4);                                   \
	prev_1 = vis_faligndata(_d_dest_4, _d_dest_5);                   \
	vis_alignaddr(d_p_str_src, 2);                                   \
	prev_2 = vis_faligndata(_d_dest_5, _d_dest_5);                   \
	BICUBIC_U8_3_XY_COUNT_PREV(prev_1, prev_2, _d_s16____2w);        \
	d_dest = vis_fpadd16(_d_s16____1w, _d_s16____2w);                \
	_f_3 = vis_fpack16(d_dest);                                      \
	vis_alignaddr(d_p_str_src, 1);                                   \
	d_dest = vis_freg_pair(f_mask_0000, _f_1);                       \
	d_dest = vis_faligndata(d_dest, d_dest);                         \
	f_dest_0 = vis_fors(vis_read_hi(d_dest), _f_0);                  \
	vis_alignaddr(d_p_str_src, 2);                                   \
	_d_s16____1w = vis_freg_pair(f_mask_0000, _f_2);                 \
	_d_s16____1w = vis_faligndata(_d_s16____1w, _d_s16____1w);       \
	f_dest_1 =                                                       \
	vis_fors(vis_read_lo(d_dest), vis_read_hi(_d_s16____1w));        \
	vis_alignaddr(d_p_str_src, 7);                                   \
	_d_s16____2w = vis_freg_pair(f_mask_0000, _f_3);                 \
	_d_s16____2w = vis_faligndata(_d_s16____2w, _d_s16____2w);       \
	f_dest_2 =                                                       \
	vis_fors(vis_read_lo(_d_s16____2w), vis_read_lo(_d_s16____1w))

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Bicubic_X(
    NEAREST_PARAMETERS1(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_2_v1, _d_02_v1, _d_1_v1, _d_3_v1, _d_13_v1;
	mlib_d64 _d_dst_help;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_0, s_1;
	mlib_d64 s_dd0, s_dd1, s_dd2, s_dd3, s_dd4, s_dd5, s_dd6, s_dd7, s_dd8;
	mlib_d64 res_0, res_1, res_2;
	mlib_d64 *d_p_str_src = 0;
	mlib_f32 _f_s16____1w, _f_s16____2w, _f_s16____3w, _f_s16____4w;
	mlib_f32 _f_0, _f_1;
	mlib_f32 f_result;

	MASK_DEFINITION_U8_3X;

	mlib_s32 off_dst;
	mlib_s32 s_stride_2 = s_stride << 1, help_width = d_width - 8;
	mlib_s32 i, j, beg;
	mlib_u8 *s_line, *d_line, *s_begin;

	vis_write_gsr((7 - BC_SCL) << 3);

	d_p_str_src = 0;
	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
/* prepare the destination address */
		d_address = (mlib_d64 *)vis_alignaddr(d_line, 0);
		off_dst = (mlib_addr)d_line - (mlib_addr)d_address;

/*  START OFFSET  */

		if ((mlib_addr)d_line > (mlib_addr)d_address) {
			mlib_s32 l;

			beg = off_dst ^ ((off_dst & 1) << 2);

			if (beg > d_width)
				beg = d_width;

			for (l = 0; l < CHAN; l++) {
				mlib_u8 *ps = s_data + s_stride_2 * j + l,
				    *pd = d_line + l;
				mlib_s32 st0 = ps[-CHAN], st1 = ps[0], k = i;

				for (; k < beg; k++) {
					mlib_s32 res = BC_CF(st1) - st0;

					st0 = ps[CHAN], st1 = ps[CHAN_2];
					res =
					    (res + BC_CF(st0) - st1) >> BC_SCL;
					*pd = SAT_U8(res);
					ps += CHAN_2;
					pd += CHAN;
				}
			}

			d_address =
			    (mlib_d64 *)vis_alignaddr(d_line, CHAN * beg);
			s_begin += CHAN_2 * beg;
			i = beg;
		}

		s_begin -= 4;
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd8 = vis_faligndata(s_0, s_1);
/*  MAIN COUNT  */

		for (; i <= help_width; i += 8) {

			s_dd0 = s_dd8;

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 6);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd1 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 12);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd2 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 18);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd3 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 24);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd4 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 30);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd5 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 36);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd6 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 42);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd7 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 48);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd8 = vis_faligndata(s_0, s_1);

			s_begin += 48;

			BICUBIC_U8_3_DOUBLE_X(s_dd0, s_dd1, s_dd2, s_dd3, s_dd4,
			    s_dd5, s_dd6, s_dd7, s_dd8, res_0, res_1, res_2);

			d_address[0] = res_0;
			d_address[1] = res_1;
			d_address[2] = res_2;
			d_address += 3;
		}

		if (i <= d_width - 4) {
			s_dd0 = s_dd8;

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 6);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd1 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 12);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd2 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 18);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd3 = vis_faligndata(s_0, s_1);

			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 24);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd4 = vis_faligndata(s_0, s_1);

			s_begin += 24;

			BICUBIC_U8_3_X(s_dd0, s_dd1, s_dd2, s_dd3, s_dd4, res_0,
			    f_result);

			d_address[0] = res_0;
			((mlib_f32 *)d_address)[2] = f_result;
			i += 4;
		}

		if (i < d_width) {
			mlib_s32 l;

			for (l = 0; l < CHAN; l++) {
				mlib_u8 *ps = s_line + CHAN_2 * i + l,
				    *pd = d_line + CHAN * i + l;
				mlib_s32 st0 = ps[-CHAN], st1 = ps[0], k = i;

#pragma pipeloop(0)
				for (; k < d_width; k++) {
					mlib_s32 res = BC_CF(st1) - st0;

					st0 = ps[CHAN], st1 = ps[CHAN_2];
					res =
					    (res + BC_CF(st0) - st1) >> BC_SCL;
					*pd = SAT_U8(res);
					ps += CHAN_2;
					pd += CHAN;
				}
			}
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Bicubic_XY(
    NEAREST_PARAMETERS1(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1,
	    _d_0_v2, _d_1_v2, _d_2_v2, _d_3_v2,
	    _d_03_v1, _d_12_v1, _d_03_v2, _d_12_v2;
	mlib_d64 _d_s16____1w, _d_s16____2w;
	mlib_d64 _d_help_1;
	mlib_d64 _d_dest_0, _d_dest_1, _d_dest_2, _d_dest_3, _d_dest_4,
	    _d_dest_5, d_dest;
	mlib_f32 _f_0, _f_1, _f_2, _f_3;

	MASK_DEFINITION_U8_3XY;

	mlib_u8 *s_line, *d_line;
	mlib_d64 *s_address;
	mlib_f32 f_result_0, f_result_1, f_result_2, *f_dst_address;
	mlib_d64 s_0, s_1;
	mlib_s32 i, j;

	mlib_u8 *s_begin;
	mlib_d64 s_dd00, s_dd01, s_dd02,
	    s_dd10, s_dd11, s_dd12,
	    s_dd20, s_dd21, s_dd22, s_dd30, s_dd31, s_dd32;

	mlib_d64 d_prev_1, d_prev_2;
	mlib_d64 *d_p_str_src = 0;
	mlib_s32 off_dst;

	mlib_s32 s_stride_2 = s_stride << 1, help_width = d_width - 4;

	vis_write_gsr((7 - BC_SCL) << 3);

	d_p_str_src = 0;
	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		f_dst_address = (mlib_f32 *)(((mlib_addr)d_line) & (~3));
		off_dst = (mlib_addr)d_line - (mlib_addr)f_dst_address;

		if ((mlib_addr)d_line > (mlib_addr)f_dst_address) {
			mlib_s32 l, beg = 0;

			beg = (off_dst > d_width) ? d_width : off_dst;

			for (l = 0; l < CHAN; l++) {
				mlib_u8 *ps = s_line + l, *pd = d_line + l;
				mlib_s32 k, st0 =
				    BC_CF(ps[-CHAN] + ps[s_stride - CHAN]) -
				    ps[-s_stride - CHAN] - ps[s_stride_2 -
				    CHAN], st1 =
				    BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];
#pragma pipeloop(0)
				for (k = i; k < beg; k++) {
					mlib_s32 res = BC_CF(st1) - st0;

					ps += CHAN;
					st0 = BC_CF(ps[0] + ps[s_stride]) -
					    ps[-s_stride] - ps[s_stride_2];
					ps += CHAN;
					st1 = BC_CF(ps[0] + ps[s_stride]) -
					    ps[-s_stride] - ps[s_stride_2];
					res =
					    (res + BC_CF(st0) - st1) >> BC_SC2;
					*pd = SAT_U8(res);
					pd += CHAN;
				}
			}

			f_dst_address = (mlib_f32 *)(d_line + CHAN * beg);
			s_begin += CHAN_2 * beg;
			i = beg;
		} else {
			f_dst_address = (mlib_f32 *)d_line;
		}

		s_begin -= 4;

		s_address = (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
		s_0 = vis_ld_d64_nf(s_address);
		s_1 = s_address[1];
		s_dd00 = vis_faligndata(s_0, s_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd10 = vis_faligndata(s_0, s_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd20 = vis_faligndata(s_0, s_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
		s_0 = s_address[0];
		s_1 = vis_ld_d64_nf(s_address + 1);
		s_dd30 = vis_faligndata(s_0, s_1);

		vis_alignaddr(d_p_str_src, 2);
		BICUBIC_U8_3_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30, d_prev_1,
		    d_prev_2);
		s_begin += 7;

#pragma pipeloop(0)
		for (; i <= help_width; i += 4) {

			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd00 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd01 = vis_faligndata(s_1, s_0);
			s_1 = s_address[3];
			s_dd02 = vis_faligndata(s_0, s_1);
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd10 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd11 = vis_faligndata(s_1, s_0);
			s_1 = s_address[3];
			s_dd12 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd21 = vis_faligndata(s_1, s_0);
			s_1 = s_address[3];
			s_dd22 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd30 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd31 = vis_faligndata(s_1, s_0);
			s_1 = vis_ld_d64_nf(s_address + 3);
			s_dd32 = vis_faligndata(s_0, s_1);

			s_begin += 24;

			BICUBIC_U8_3_XY(s_dd00, s_dd10, s_dd20, s_dd30,
			    s_dd01, s_dd11, s_dd21, s_dd31,
			    s_dd02, s_dd12, s_dd22, s_dd32,
			    d_prev_1, d_prev_2, f_result_0, f_result_1,
			    f_result_2);

			f_dst_address[0] = f_result_0;
			f_dst_address[1] = f_result_1;
			f_dst_address[2] = f_result_2;
			f_dst_address += 3;
		}

		if (i < d_width) {
			mlib_s32 l;

			for (l = 0; l < CHAN; l++) {
				mlib_u8 *ps = s_line + CHAN_2 * i + l,
				    *pd = d_line + CHAN * i + l;
				mlib_s32 k, st0 =
				    BC_CF(ps[-CHAN] + ps[s_stride - CHAN]) -
				    ps[-s_stride - CHAN] - ps[s_stride_2 -
				    CHAN], st1 =
				    BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];

#pragma pipeloop(0)
				for (k = i; k < d_width; k++) {
					mlib_s32 res = BC_CF(st1) - st0;

					ps += CHAN;
					st0 = BC_CF(ps[0] + ps[s_stride]) -
					    ps[-s_stride] - ps[s_stride_2];
					ps += CHAN;
					st1 = BC_CF(ps[0] + ps[s_stride]) -
					    ps[-s_stride] - ps[s_stride_2];
					res =
					    (res + BC_CF(st0) - st1) >> BC_SC2;
					*pd = SAT_U8(res);
					pd += CHAN;
				}
			}
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

#undef CHAN
#undef CHAN_2
#undef CHAN_4

/* *********************************************************** */

#define	CHAN	4
#define	CHAN_2	8

#define	CHAN_4	16

/* *********************************************************** */

#define	BICUBIC_U8_4_FLOAT_X_HELP(d_0, d_1, d_dst)               \
	_d_0_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_0));    \
	_d_1_v1 = vis_fmul8x16(vis_read_lo(d_0), d_mask_9999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_9999);   \
	_d_3_v1 = vis_fmul8x16(vis_read_lo(d_1), d_mask_1111);   \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	d_dst = vis_fpsub16(_d_12_v1, _d_03_v1)

/* *********************************************************** */

#define	BICUBIC_U8_4_FLOAT_X(d_0, d_1, f_dst)                   \
	BICUBIC_U8_4_FLOAT_X_HELP(d_0, d_1, _d_dst_help);       \
	f_dst = vis_fpack16(_d_dst_help)

/* *********************************************************** */

#define	BICUBIC_U8_4_DOUBLE_X(d_0, d_1, d_2, d_dst)             \
	BICUBIC_U8_4_FLOAT_X_HELP(d_0, d_1, _d_s16____1w);      \
	BICUBIC_U8_4_FLOAT_X_HELP(d_1, d_2, _d_s16____2w);      \
	d_dst = vis_fpack16_pair(_d_s16____1w, _d_s16____2w)

/* *********************************************************** */

#define	BICUBIC_U8_4_FLOAT_Y_ALIGN(f_0, f_1, f_2, f_3, f_dst)   \
	_d_0_v1 = vis_fpmerge(f_mask_0000, f_0);                \
	_d_3_v1 = vis_fmul8x16(f_3, d_mask_1111);               \
	_d_1_v1 = vis_fmul8x16(f_1, d_mask_9999);               \
	_d_2_v1 = vis_fmul8x16(f_2, d_mask_9999);               \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);               \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);               \
	_d_dst_help = vis_fpsub16(_d_12_v1, _d_03_v1);          \
	f_dst = vis_fpack16(_d_dst_help)

/* *********************************************************** */

#define	BICUBIC_U8_4_FLOAT_Y_HELP(d_0, d_1, d_2, d_3, d_dst)     \
	_d_0_v1 = vis_fpmerge(f_mask_0000, vis_read_lo(d_0));    \
	_d_1_v1 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9999);   \
	_d_3_v1 = vis_fpmerge(f_mask_0000, vis_read_lo(d_3));    \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	d_dst = vis_fpsub16(_d_12_v1, _d_03_v1)

/* *********************************************************** */

#define	BICUBIC_U8_4_FLOAT_Y(d_0, d_1, d_2, d_3, f_dst)               \
	BICUBIC_U8_4_FLOAT_Y_HELP(d_0, d_1, d_2, d_3, _d_dst_help);   \
	f_dst = vis_fpack16(_d_dst_help)

/* *********************************************************** */

#define	BICUBIC_U8_4_DOUBLE_Y(d_0, d_1, d_2, d_3, d_4, d_5,            \
	d_6, d_7, d_dst)                                               \
	BICUBIC_U8_4_FLOAT_Y_HELP(d_0, d_1, d_2, d_3, _d_s16____1w);   \
	BICUBIC_U8_4_FLOAT_Y_HELP(d_4, d_5, d_6, d_7, _d_s16____2w);   \
	d_dst = vis_fpack16_pair(_d_s16____1w, _d_s16____2w)

/* *********************************************************** */

#define	BICUBIC_U8_4_XY_PREV(d_0, d_1, d_2, d_3, dst_1, dst_2)   \
	_d_0_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_0));    \
	_d_1_v1 = vis_fmul8x16(vis_read_hi(d_1), d_mask_9999);   \
	_d_2_v1 = vis_fmul8x16(vis_read_hi(d_2), d_mask_9999);   \
	_d_3_v1 = vis_fpmerge(f_mask_0000, vis_read_hi(d_3));    \
	_d_03_v1 = vis_fpadd16(_d_0_v1, _d_3_v1);                \
	_d_12_v1 = vis_fpadd16(_d_1_v1, _d_2_v1);                \
	dst_1 = vis_fpsub16(_d_12_v1, _d_03_v1);                 \
	_d_0_v2 = vis_fmul8x16(vis_read_lo(d_0), d_mask_1111);   \
	_d_1_v2 = vis_fmul8x16(vis_read_lo(d_1), d_mask_9999);   \
	_d_2_v2 = vis_fmul8x16(vis_read_lo(d_2), d_mask_9999);   \
	_d_3_v2 = vis_fmul8x16(vis_read_lo(d_3), d_mask_1111);   \
	_d_03_v2 = vis_fpadd16(_d_0_v2, _d_3_v2);                \
	_d_12_v2 = vis_fpadd16(_d_1_v2, _d_2_v2);                \
	dst_2 = vis_fpsub16(_d_12_v2, _d_03_v2)

/* *********************************************************** */

#define	BICUBIC_U8_4_FLOAT_XY(d_0, d_1, d_2, d_3, f_dst,                \
	prev_1, prev_2)                                                 \
	BICUBIC_U8_4_XY_PREV(d_0, d_1, d_2, d_3, _d_s16_3, _d_s16_4);   \
	_d_s16_14 = vis_fpadd16(prev_1, _d_s16_4);                      \
	_d_s16_14 = vis_fmul8x16(f_mask_10101010, _d_s16_14);           \
	_d_s16_23 = vis_fpadd16(prev_2, _d_s16_3);                      \
	_d_s16_23 = vis_fmul8x16(f_mask_90909090, _d_s16_23);           \
	_d_s16_23 = vis_fpsub16(_d_s16_23, _d_s16_14);                  \
	f_dst = vis_fpack16(_d_s16_23);                                 \
	prev_1 = _d_s16_3;                                              \
	prev_2 = _d_s16_4

/* *********************************************************** */

#define	BICUBIC_U8_4_DOUBLE_XY(d_0_0, d_0_1, d_0_2, d_0_3,           \
	d_1_0, d_1_1, d_1_2, d_1_3, d_dest, prev_1, prev_2)          \
	BICUBIC_U8_4_XY_PREV(d_0_0, d_0_1, d_0_2, d_0_3, _d_s16_3,   \
	    _d_s16_4);                                               \
	_d_s16_14 = vis_fpadd16(prev_1, _d_s16_4);                   \
	_d_s16_14 = vis_fmul8x16(f_mask_10101010, _d_s16_14);        \
	_d_s16_23 = vis_fpadd16(prev_2, _d_s16_3);                   \
	_d_s16_23 = vis_fmul8x16(f_mask_90909090, _d_s16_23);        \
	_d_s16____1w = vis_fpsub16(_d_s16_23, _d_s16_14);            \
	BICUBIC_U8_4_XY_PREV(d_1_0, d_1_1, d_1_2, d_1_3, prev_1,     \
	    prev_2);                                                 \
	_d_s16_14 = vis_fpadd16(_d_s16_3, prev_2);                   \
	_d_s16_14 = vis_fmul8x16(f_mask_10101010, _d_s16_14);        \
	_d_s16_23 = vis_fpadd16(_d_s16_4, prev_1);                   \
	_d_s16_23 = vis_fmul8x16(f_mask_90909090, _d_s16_23);        \
	_d_s16____2w = vis_fpsub16(_d_s16_23, _d_s16_14);            \
	d_dest = vis_fpack16_pair(_d_s16____1w, _d_s16____2w)

/* *********************************************************** */

void mlib_ImageZoomOut2X_U8_4_Bicubic_Float_X(
    NEAREST_PARAMETERS(mlib_u8));

void
mlib_ImageZoomOut2X_U8_4_Bicubic_X(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1, _d_03_v1, _d_12_v1;
	mlib_d64 _d_s16____1w, _d_s16____2w, _d_dst_help;
	mlib_s32 i, j;
	mlib_u8 *p_str_src, *p_str_dst;
	mlib_d64 d_s_1w, d_s_2w, d_s_3w;
	mlib_d64 d_dest;
	mlib_d64 *d_p_str_src, *d_p_str_dst;
	mlib_f32 f_dest;
	mlib_s32 src_stride_2 = src_stride << 1;

	MASK_DEFINITION_U8_4;

	vis_write_gsr((7 - BC_SCL) << 3);
	d_p_str_src = 0;

	if ((((mlib_addr)p_dst | dst_stride) & 3) == 0) {
		mlib_s32 help_width;

		if (((((mlib_addr)(p_src - CHAN)) | src_stride |
		    ((mlib_addr)p_dst) | dst_stride) & 7) == 0) {
			help_width = dst_width - 2;

			for (i = 0; i < dst_height; i++) {
/* +4 */
				p_str_src = p_src + i * src_stride_2 + CHAN;
				p_str_dst = p_dst + i * dst_stride;

				d_p_str_src = (mlib_d64 *)p_str_src;
				d_p_str_dst = (mlib_d64 *)p_str_dst;

				d_s_3w = d_p_str_src[-1];
#pragma pipeloop(0)
				for (j = 0; j <= help_width; j += 2) {
					d_s_1w = d_s_3w;
					d_s_2w = d_p_str_src[0];
					d_s_3w = d_p_str_src[1];

					BICUBIC_U8_4_DOUBLE_X(d_s_1w, d_s_2w,
					    d_s_3w, d_dest);
					*d_p_str_dst = d_dest;
					d_p_str_dst++;
					d_p_str_src += 2;
				}

				if (j != dst_width) {
					d_s_1w = d_s_3w;
					d_s_2w = d_p_str_src[0];

					BICUBIC_U8_4_FLOAT_X(d_s_1w, d_s_2w,
					    f_dest);
					*(mlib_f32 *)d_p_str_dst = f_dest;
				}
			}

			return;
		}

		mlib_ImageZoomOut2X_U8_4_Bicubic_Float_X(PARAM1, coef);
		return;
	}

	mlib_ImageZoomOut2X_U8_4_Bicubic_X_NotAl(PARAM1, coef);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic_Y(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1, _d_03_v1, _d_12_v1;
	mlib_d64 _d_dst_help;
	mlib_s32 i, j;
	mlib_f32 f_s_1w, f_s_2w, f_s_3w, f_s_4w;
	mlib_f32 f_dest;
	mlib_s32 src_stride_2 = src_stride << 1;

	MASK_DEFINITION_U8_4;

	vis_write_gsr((7 - BC_SCL) << 3);

	if (!(((mlib_addr)p_src | src_stride) & 3)) {
		mlib_f32 *f_p_str_src, *f_p_str_dst;
		mlib_s32 f_stride_1 = src_stride >> 2, f_stride_2 =
		    src_stride >> 1;

		if (!(((mlib_addr)p_dst | dst_stride) & 3)) {
			for (i = 0; i < dst_height; i++) {
				f_p_str_src =
				    (mlib_f32 *)(p_src + i * src_stride_2);
				f_p_str_dst =
				    (mlib_f32 *)(p_dst + i * dst_stride);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					f_s_1w = f_p_str_src[-f_stride_1];
					f_s_2w = f_p_str_src[0];
					f_s_3w = f_p_str_src[f_stride_1];
					f_s_4w = f_p_str_src[f_stride_2];
					BICUBIC_U8_4_FLOAT_Y_ALIGN(f_s_1w,
					    f_s_2w, f_s_3w, f_s_4w, f_dest);
					*f_p_str_dst = f_dest;
					f_p_str_src += 2;
					f_p_str_dst++;
				}
			}

		} else {
			for (i = 0; i < dst_height; i++) {
				mlib_u8 *u_p_str_dst = p_dst + i * dst_stride;

				f_p_str_src =
				    (mlib_f32 *)(p_src + i * src_stride_2);
#pragma pipeloop(0)
				for (j = 0; j < dst_width; j++) {
					f_s_1w = f_p_str_src[-f_stride_1];
					f_s_2w = f_p_str_src[0];
					f_s_3w = f_p_str_src[f_stride_1];
					f_s_4w = f_p_str_src[f_stride_2];
					BICUBIC_U8_4_FLOAT_Y_ALIGN(f_s_1w,
					    f_s_2w, f_s_3w, f_s_4w, f_dest);
					{
						mlib_u8 *ptu =
						    (mlib_u8 *)(&f_dest);
						(*u_p_str_dst++) = (*ptu++);
						(*u_p_str_dst++) = (*ptu++);
						(*u_p_str_dst++) = (*ptu++);
						(*u_p_str_dst++) = *ptu;
					}

					f_p_str_src += 2;
				}
			}
		}

		return;
	} /* aligned */

	mlib_ImageZoomOut2X_U8_4_Bicubic_Y_NotAl(PARAM1, coef);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic_XY(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1,
	    _d_0_v2, _d_1_v2, _d_2_v2, _d_3_v2,
	    _d_03_v1, _d_12_v1, _d_03_v2, _d_12_v2;
	mlib_d64 _d_s16_3, _d_s16_4, _d_s16_14, _d_s16_23;
	mlib_s32 i, j, src_stride_4, src_stride_8;
	mlib_u8 *p_str_src, *p_str_dst;
	mlib_d64 *d_p_str_src;
	mlib_d64 d_s_1, d_s_2, d_s_3, d_s_4;
	mlib_d64 d_prev_0, d_prev_1;
	mlib_f32 f_dest, *f_p_str_dst;
	mlib_s32 src_stride_2 = src_stride << 1;

	MASK_DEFINITION_U8_4;

	vis_write_gsr((7 - BC_SCL) << 3);

	if (((((mlib_addr)p_dst | dst_stride) & 3) |
	    (((mlib_addr)(p_src - CHAN) | src_stride) & 7)) == 0) {

		src_stride_4 = src_stride >> 2;
		src_stride_8 = src_stride >> 3;
		p_src -= CHAN;
		p_str_src = p_src;
		p_str_dst = p_dst;

		for (i = 0; i < dst_height; i++) {

			d_p_str_src = (mlib_d64 *)p_str_src;
			f_p_str_dst = (mlib_f32 *)p_str_dst;

			BICUBIC_U8_4_XY_PREV(d_p_str_src[-src_stride_8],
			    d_p_str_src[0],
			    d_p_str_src[src_stride_8],
			    d_p_str_src[src_stride_4], d_prev_0, d_prev_1);

			d_p_str_src += 1;

#pragma pipeloop(0)
			for (j = 0; j <= dst_width - 1; j += 1) {
				d_s_1 = d_p_str_src[-src_stride_8];
				d_s_2 = d_p_str_src[0];
				d_s_3 = d_p_str_src[src_stride_8];
				d_s_4 = d_p_str_src[src_stride_4];
				d_p_str_src++;

				BICUBIC_U8_4_FLOAT_XY(d_s_1, d_s_2, d_s_3,
				    d_s_4, f_dest, d_prev_0, d_prev_1);
				*f_p_str_dst = f_dest;
				f_p_str_dst++;
			}

			p_str_src += src_stride_2;
			p_str_dst += dst_stride;
		}

		return;
	}

	mlib_ImageZoomOut2X_U8_4_Bicubic_XY_NotAl(PARAM1, coef);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic_Float_X(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1, _d_03_v1, _d_12_v1;
	mlib_d64 _d_dst_help;

	mlib_s32 i, j;
	mlib_u8 *p_str_src, *p_str_dst;
	mlib_d64 d_s_1w, d_s_2w;

	mlib_d64 d_1_l, d_1_m, d_1_h;
	mlib_d64 *d_p_str_src;

	mlib_f32 f_dest, *f_p_str_dst;

	MASK_DEFINITION_U8_4;

	mlib_s32 src_stride_2 = src_stride << 1;

	for (i = 0; i < dst_height; i++) {
		p_str_src = p_src + i * src_stride_2 - CHAN; /* -4 */
		p_str_dst = p_dst + i * dst_stride;
		d_p_str_src = (mlib_d64 *)vis_alignaddr(p_str_src, 8);
		f_p_str_dst = (mlib_f32 *)p_str_dst;
		d_1_m = d_p_str_src[-1];
		d_1_l = d_p_str_src[0];
#pragma pipeloop(0)
		for (j = 0; j <= dst_width - 1; j++) {
			d_1_h = d_1_m;
			d_1_m = d_1_l;
			d_1_l = d_p_str_src[1];
			d_s_1w = vis_faligndata(d_1_h, d_1_m);
			d_s_2w = vis_faligndata(d_1_m, d_1_l);
			BICUBIC_U8_4_FLOAT_X(d_s_1w, d_s_2w, f_dest);
			*f_p_str_dst = f_dest;
			d_p_str_src++;
			f_p_str_dst++;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic_X_NotAl(
    NEAREST_PARAMETERS1(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1, _d_03_v1, _d_12_v1;
	mlib_d64 _d_s16____1w, _d_s16____2w, _d_dst_help;

	mlib_d64 *d_address, *s_address;
	mlib_d64 s_0, s_1;
	mlib_d64 s_dd0, s_dd1, s_dd2, s_dd3, s_dd4;
	mlib_d64 res_0, res_1;
	mlib_f32 f_result;

	MASK_DEFINITION_U8_4;

	mlib_u8 *s_line, *d_line, *s_begin, *d_end, *help;
	mlib_u8 emask, no_emask;
	mlib_s32 i, j;
	mlib_s32 help_d_width = d_width - 4, s_stride_2 = s_stride << 1;
	void *u_align;

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		d_address = (mlib_d64 *)vis_alignaddr(d_line, 0);

		emask = vis_edge8(d_line, d_address + 1);
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, -CHAN);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_begin += 16;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (i = 0; i <= help_d_width; i += 4) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-CHAN);
				s_dd0 = vis_faligndata(s_0, s_1);
				s_0 = s_address[0];
				s_dd1 = vis_faligndata(s_1, s_0);
				s_1 = s_address[1];
				s_dd2 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd3 = vis_faligndata(s_1, s_0);
				s_1 = s_address[3];
				s_dd4 = vis_faligndata(s_0, s_1);
				s_begin += 32; /* == 40-8 ! */

				BICUBIC_U8_4_DOUBLE_X(s_dd0, s_dd1, s_dd2,
					res_0);
				BICUBIC_U8_4_DOUBLE_X(s_dd2, s_dd3, s_dd4,
					res_1);

				vis_alignaddr(u_align, 1);

				res_0 = vis_faligndata(res_0, res_0);
				res_1 = vis_faligndata(res_1, res_1);

				d_address[0] = res_0;
				d_address++;
				d_address[0] = res_1;
				d_address++;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i <= help_d_width; i += 4) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-CHAN);
				s_dd0 = vis_faligndata(s_0, s_1);
				s_0 = s_address[0];
				s_dd1 = vis_faligndata(s_1, s_0);
				s_1 = s_address[1];
				s_dd2 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd3 = vis_faligndata(s_1, s_0);
				s_1 = s_address[3];
				s_dd4 = vis_faligndata(s_0, s_1);
				s_begin += 32; /* == 40-8 ! */

				BICUBIC_U8_4_DOUBLE_X(s_dd0, s_dd1, s_dd2,
					res_0);
				BICUBIC_U8_4_DOUBLE_X(s_dd2, s_dd3, s_dd4,
					res_1);

				vis_alignaddr(u_align, 1);

				res_0 = vis_faligndata(res_0, res_0);
				res_1 = vis_faligndata(res_1, res_1);

				vis_pst_8(res_0, d_address, emask);
				d_address++;
				vis_pst_8(res_0, d_address, no_emask);
				vis_pst_8(res_1, d_address, emask);
				d_address++;
				vis_pst_8(res_1, d_address, no_emask);
			}
		}

		if (i <= d_width - 2) {
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, -CHAN);
			s_dd0 = vis_faligndata(s_0, s_1);
			s_0 = s_address[0];
			s_dd1 = vis_faligndata(s_1, s_0);
			s_1 = s_address[1];
			s_dd2 = vis_faligndata(s_0, s_1);
			s_begin += 16;

			BICUBIC_U8_4_DOUBLE_X(s_dd0, s_dd1, s_dd2, res_0);

			vis_alignaddr(u_align, 1);

			res_0 = vis_faligndata(res_0, res_0);

			vis_pst_8(res_0, d_address, emask);
			d_address++;
			vis_pst_8(res_0, d_address, no_emask);
			i += 2;
		}

		if (i < d_width) {
			d_end = d_line + (d_width << 2) - 4;
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, -CHAN);
			s_dd0 = vis_faligndata(s_0, s_1);
			s_0 = s_address[0];
			s_dd1 = vis_faligndata(s_1, s_0);
			s_1 = s_address[1];
			s_dd2 = vis_faligndata(s_0, s_1);
			s_begin += 16;

			BICUBIC_U8_4_FLOAT_X(s_dd0, s_dd1, f_result);

			help = (mlib_u8 *)(&f_result);
			d_end[0] = help[0];
			d_end[1] = help[1];
			d_end[2] = help[2];
			d_end[3] = help[3];
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic_Y_NotAl(
    NEAREST_PARAMETERS1(mlib_u8))
{
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1, _d_03_v1, _d_12_v1;
	mlib_d64 _d_s16____1w, _d_s16____2w;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_0, s_1, result;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;

	MASK_DEFINITION_U8_4;

	mlib_f32 f_result;
	mlib_s32 i, j;
	mlib_s32 help_d_width = d_width - 2, s_stride_2 = s_stride << 1;
	mlib_u8 *s_line, *d_line, *d_end, *help;
	mlib_u8 *s_begin;
	mlib_u8 emask, no_emask;
	void *u_align;

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
/* prepare the destination address */
		d_address = (mlib_d64 *)vis_alignaddr(d_line, 0);

		emask = vis_edge8(d_line, d_address + 1);
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		s_begin -= CHAN;

		if (emask == 0xff) {
/* 8-pixel column loop */
#pragma pipeloop(0)
			for (i = 0; i <= help_d_width; i += 2) {
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd00 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd01 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					0);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd10 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd11 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_2);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd31 = vis_faligndata(s_1, s_0);

				s_begin += 16;

				BICUBIC_U8_4_DOUBLE_Y(s_dd00, s_dd10, s_dd20,
					s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
					result);
				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address += 1;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i <= help_d_width; i += 2) {
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd00 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd01 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					0);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd10 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd11 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_2);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd31 = vis_faligndata(s_1, s_0);

				s_begin += 16;

				BICUBIC_U8_4_DOUBLE_Y(s_dd00, s_dd10, s_dd20,
					s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
					result);
				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address += 1;
				vis_pst_8(result, d_address, no_emask);
			}
		}

/* last 4-pixel column */

		if (i < d_width) {
			d_end = d_line + (d_width << 2) - 4;
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd00 = vis_faligndata(s_0, s_1);
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd10 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd30 = vis_faligndata(s_0, s_1);

			BICUBIC_U8_4_FLOAT_Y_HELP(s_dd00, s_dd10, s_dd20,
			    s_dd30, _d_s16____1w);
			f_result = vis_fpack16(_d_s16____1w);
			help = (mlib_u8 *)(&f_result);
			d_end[0] = help[0];
			d_end[1] = help[1];
			d_end[2] = help[2];
			d_end[3] = help[3];
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic_XY_NotAl(
    NEAREST_PARAMETERS1(mlib_u8))
{
	mlib_d64 d_prev_0, d_prev_1;
	mlib_d64 _d_0_v1, _d_1_v1, _d_2_v1, _d_3_v1,
	    _d_0_v2, _d_1_v2, _d_2_v2, _d_3_v2,
	    _d_03_v1, _d_12_v1, _d_03_v2, _d_12_v2;
	mlib_d64 _d_s16____1w, _d_s16____2w;
	mlib_d64 _d_s16_3, _d_s16_4, _d_s16_14, _d_s16_23;
	mlib_d64 s_help_0, s_help_1, s_help_2, s_help_3;
	mlib_d64 s_1, result;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;

	MASK_DEFINITION_U8_4;

	mlib_f32 f_result;
	mlib_s32 i, j;
	mlib_s32 help_d_width = d_width - 2, s_stride_2 = s_stride << 1;
	mlib_u8 *s_begin;
	mlib_u8 *s_line, *d_line, *d_end, *help;
	mlib_u8 emask, no_emask;
	void *u_align;

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;

		d_address = (mlib_d64 *)vis_alignaddr(d_line, 0);
		emask = vis_edge8(d_line, d_address + 1);
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		s_begin -= CHAN;

		s_address = (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
		s_1 = s_address[0];
		s_help_0 = s_address[1];
		s_dd00 = vis_faligndata(s_1, s_help_0);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
		s_1 = s_address[0];
		s_help_1 = s_address[1];
		s_dd10 = vis_faligndata(s_1, s_help_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
		s_1 = s_address[0];
		s_help_2 = s_address[1];
		s_dd20 = vis_faligndata(s_1, s_help_2);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
		s_1 = s_address[0];
		s_help_3 = vis_ld_d64_nf(s_address + 1);
		s_dd30 = vis_faligndata(s_1, s_help_3);

		BICUBIC_U8_4_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30, d_prev_0,
		    d_prev_1);
		s_begin += CHAN_2;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 2) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride);
				s_1 = s_address[1];
				s_dd00 = vis_faligndata(s_help_0, s_1);
				s_help_0 = s_address[2];
				s_dd01 = vis_faligndata(s_1, s_help_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					0);
				s_1 = s_address[1];
				s_dd10 = vis_faligndata(s_help_1, s_1);
				s_help_1 = s_address[2];
				s_dd11 = vis_faligndata(s_1, s_help_1);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride);
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_help_2, s_1);
				s_help_2 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_help_2);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_2);
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_help_3, s_1);
				s_help_3 = vis_ld_d64_nf(s_address + 2);
				s_dd31 = vis_faligndata(s_1, s_help_3);

				s_begin += CHAN_4;

				BICUBIC_U8_4_DOUBLE_XY(s_dd00, s_dd10, s_dd20,
					s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
					result, d_prev_0, d_prev_1);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address++;
			}
		} else {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 2) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride);
				s_1 = s_address[1];
				s_dd00 = vis_faligndata(s_help_0, s_1);
				s_help_0 = s_address[2];
				s_dd01 = vis_faligndata(s_1, s_help_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					0);
				s_1 = s_address[1];
				s_dd10 = vis_faligndata(s_help_1, s_1);
				s_help_1 = s_address[2];
				s_dd11 = vis_faligndata(s_1, s_help_1);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride);
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_help_2, s_1);
				s_help_2 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_help_2);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_2);
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_help_3, s_1);
				s_help_3 = vis_ld_d64_nf(s_address + 2);
				s_dd31 = vis_faligndata(s_1, s_help_3);

				s_begin += CHAN_4;

				BICUBIC_U8_4_DOUBLE_XY(s_dd00, s_dd10, s_dd20,
					s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
					result, d_prev_0, d_prev_1);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address++;
				vis_pst_8(result, d_address, no_emask);
			}
		}

		if (i < d_width) {
			d_end = d_line + (d_width << 2) - 4;
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride);
			s_1 = s_address[1];
			s_dd00 = vis_faligndata(s_help_0, s_1);
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_1 = s_address[1];
			s_dd10 = vis_faligndata(s_help_1, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride);
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_help_2, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd30 = vis_faligndata(s_help_3, s_1);

			BICUBIC_U8_4_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30,
			    _d_s16_3, _d_s16_4);
			_d_s16_14 = vis_fpadd16(d_prev_0, _d_s16_4);
			_d_s16_14 = vis_fmul8x16(f_mask_10101010, _d_s16_14);
			_d_s16_23 = vis_fpadd16(d_prev_1, _d_s16_3);
			_d_s16_23 = vis_fmul8x16(f_mask_90909090, _d_s16_23);
			_d_s16_23 = vis_fpsub16(_d_s16_23, _d_s16_14);
			f_result = vis_fpack16(_d_s16_23);
			help = (mlib_u8 *)(&f_result);
			d_end[0] = help[0];
			d_end[1] = help[1];
			d_end[2] = help[2];
			d_end[3] = help[3];
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

/* new functions */

#define	PSC(sy, sc)	ps[sy * src_stride + sc]

/* *********************************************************** */

/* averaging along both axes */
#define	BC_CONDITION_1(chan, CST_BC, typeD, typeO)              \
	if (dw == dh) {                                         \
	    INIT_BC_CYCLE_1(chan, CST_BC, typeD, typeO)

/* *********************************************************** */

#define	INIT_BC_CYCLE_1(chan, CST_BC, typeD, typeO)                        \
	for (i = 0; i < dst_height; i++) {                                 \
	    for (l = 0; l < chan; l++) {                                   \
		typeD *ps0 =                                               \
		    p_src + (i * 2 - 1) * src_stride - chan + l;           \
		typeD *ps1 = ps0 + src_stride;                             \
		typeD *ps2 = ps1 + src_stride;                             \
		typeD *ps3 = ps2 + src_stride;                             \
		typeD *pd = p_dst + i * dst_stride + l;                    \
		mlib_s32 j, j2;                                            \
		typeO st0, st1;                                            \
	                                                                   \
		st0 = CST_BC((typeO) ps1[0] + ps2[0]) - ps0[0] - ps3[0];   \
		st1 =                                                      \
		    CST_BC((typeO) ps1[chan] + ps2[chan]) - ps0[chan] -    \
		    ps3[chan]

/* *********************************************************** */

#define	BC_CYCLE_1(chan, chn2, SATUR, CST_BC, NORMAL2)               \
	for (j = 0, j2 = chn2; j < dst_width * chan;                 \
	    j += chan, j2 += chn2) {                                 \
	    res = CST_BC(st1) - st0;                                 \
	    st0 = CST_BC(ps1[j2] + ps2[j2]) - ps0[j2] - ps3[j2];     \
	    st1 =                                                    \
		CST_BC(ps1[j2 + chan] + ps2[j2 + chan]) - ps0[j2 +   \
		chan] - ps3[j2 + chan];                              \
	    res = (res + CST_BC(st0) - st1) >> NORMAL2;              \
	    pd[j] = SATUR(res);                                      \
	}

/* *********************************************************** */

/* averaging along X axis */
#define	BC_CONDITION_2(chan, typeD, typeO)                      \
		}                                               \
	    }                                                   \
	    return;                                             \
	}                                                       \
	                                                        \
	if (dh) {                                               \
	    INIT_BC_CYCLE_2(chan, typeD, typeO)

/* *********************************************************** */

#define	INIT_BC_CYCLE_2(chan, typeD, typeO)                         \
	for (i = 0; i < dst_height; i++) {                          \
	    for (l = 0; l < chan; l++) {                            \
		typeD *ps = p_src + 2 * i * src_stride + l,         \
		    *pd = p_dst + i * dst_stride + l;               \
		typeO a1 = PSC(0, 0), a0 = PSC(0, -chan)

/* *********************************************************** */

#define	BC_CYCLE_2(chan, chn2, SATUR, CST_BC, NORMAL)           \
	for (j = 0; j < dst_width; j++) {                       \
	    res = CST_BC(a1) - a0;                              \
	    a0 = PSC(0, chan);                                  \
	    a1 = PSC(0, chn2);                                  \
	    ps += chn2;                                         \
	    res = (res + CST_BC(a0) - a1) >> NORMAL;            \
	    *pd = SATUR(res);                                   \
	    pd += chan;                                         \
	}

/* *********************************************************** */

/* averaging along Y axis */
#define	BC_CONDITION_3(chan, typeD, typeO)                      \
		}                                               \
	    }                                                   \
	    return;                                             \
	}                                                       \
	                                                        \
	INIT_BC_CYCLE_3(chan, typeD, typeO)

/* *********************************************************** */

#define	INIT_BC_CYCLE_3(chan, typeD, typeO)                         \
	{                                                           \
	    typeD *ps0, *ps1, *ps2, *ps3, *ps4, *ps5, *pd0, *pd1;   \
	    mlib_s32 j, j2;                                         \
	    typeO res0, res1;                                       \
	                                                            \
	    for (i = 0; i < (dst_height & (~1)); i += 2) {          \
		ps0 = p_src + (2 * i - 1) * src_stride;             \
		ps1 = ps0 + src_stride;                             \
		ps2 = ps1 + src_stride;                             \
		ps3 = ps2 + src_stride;                             \
		ps4 = ps3 + src_stride;                             \
		ps5 = ps4 + src_stride;                             \
		pd0 = p_dst + i * dst_stride;                       \
		pd1 = pd0 + dst_stride

/* *********************************************************** */

#define	BC_CYCLE_3(chan, chn2, SATUR, CST_BC, NORMAL)                   \
	    for (l = 0; l < chan; l++) {                                \
		for (j = 0, j2 = 0; j < dst_width * chan;               \
		    j += chan, j2 += chn2) {                            \
		    res0 =                                              \
			(CST_BC(ps1[j2 + l] + ps2[j2 + l]) - ps0[j2 +   \
			l] - ps3[j2 + l]) >> NORMAL;                    \
		    pd0[j + l] = SATUR(res0);                           \
		    res1 =                                              \
			(CST_BC(ps3[j2 + l] + ps4[j2 + l]) - ps2[j2 +   \
			l] - ps5[j2 + l]) >> NORMAL;                    \
		    pd1[j + l] = SATUR(res1);                           \
		}                                                       \
	    }                                                           \
	}                                                               \
	                                                                \
	if (dst_height & 1) {                                           \
	    ps0 = p_src + (2 * i - 1) * src_stride;                     \
	    ps1 = ps0 + src_stride;                                     \
	    ps2 = ps1 + src_stride;                                     \
	    ps3 = ps2 + src_stride;                                     \
	    pd0 = p_dst + i * dst_stride;                               \
	    for (l = 0; l < chan; l++) {                                \
		for (j = 0, j2 = 0; j < dst_width * chan;               \
		    j += chan, j2 += chn2) {                            \
		    res0 =                                              \
			(CST_BC(ps1[j2 + l] + ps2[j2 + l]) - ps0[j2 +   \
			l] - ps3[j2 + l]) >> NORMAL;                    \
		    pd0[j + l] = SATUR(res0);                           \
		}                                                       \
	    }                                                           \
	}                                                               \
}

/* *********************************************************** */

/*  u8 c1 bicubic2 4x4 part	  1 -5 -5  1			*/
/*			    x    -5 25 25 -5    / 64		*/
/*				 -5 25 25 -5			*/
/*				  1 -5 -5  1			*/

/* *********************************************************** */

#define	VIS_DATA_INIT                                           \
	INIT_ALIGN(0, 07, 08);                                  \
	INIT_ALIGN(1, 09, 10);                                  \
	INIT_ALIGN(2, 11, 12);                                  \
	INIT_ALIGN(3, 13, 14);                                  \
	INIT_ALIGN_NF(4, 15, 16);                               \
	INIT_ALIGN_NF(5, 17, 18)

#define	INIT_ALIGN(NUM, BN1, BN2)                               \
	psd##NUM = (mlib_d64 *)vis_alignaddr(ps##NUM + js, 0);  \
	r##BN1 = *psd##NUM;                                     \
	r##BN2 = *(psd##NUM + 1)

#define	INIT_ALIGN_NF(NUM, BN1, BN2)                            \
	psd##NUM = (mlib_d64 *)vis_alignaddr(ps##NUM + js, 0);  \
	r##BN1 = vis_ld_d64_nf(psd##NUM);                       \
	r##BN2 = vis_ld_d64_nf(psd##NUM + 1)

/* *********************************************************** */

#define	VIS_DATA_LOAD                                           \
	VIS_ALIGN(0, 07, 08);                                   \
	VIS_ALIGN(1, 09, 10);                                   \
	VIS_ALIGN(2, 11, 12);                                   \
	VIS_ALIGN(3, 13, 14);                                   \
	VIS_ALIGN(4, 15, 16);                                   \
	VIS_ALIGN(5, 17, 18);                                   \
	VIS_DATA_SHIFT;

#define	VIS_ALIGN(NUM, BN1, BN2)                                \
	psd##NUM = (mlib_d64 *)vis_alignaddr(ps##NUM + js, 0);  \
	r0##NUM = vis_faligndata(r##BN1, r##BN2)

/* *********************************************************** */

#define	VIS_DATA_SHIFT                                          \
	ALIGN_SHIFT(0, 08, 07);                                 \
	ALIGN_SHIFT(1, 10, 09);                                 \
	ALIGN_SHIFT(2, 12, 11);                                 \
	ALIGN_SHIFT_NF(3, 14, 13);                              \
	ALIGN_SHIFT_NF(4, 16, 15);                              \
	ALIGN_SHIFT_NF(5, 18, 17);

#define	ALIGN_SHIFT(NUM, R1, R2)	r##R2 = r##R1; r##R1 = *(psd##NUM+2)

#define	ALIGN_SHIFT_NF(NUM, R1, R2)                             \
	r##R2 = r##R1; r##R1 = vis_ld_d64_nf(psd##NUM+2)

/* *********************************************************** */

#define	INIT_N_STRING(NUM, NH, NL)                                \
	r##NH = vis_fmul8x16al(vis_read_hi(r0##NUM), f_mask_9);   \
	r##NL = vis_fmul8x16al(vis_read_lo(r0##NUM), f_mask_9)

/* *********************************************************** */

#define	INIT_E_STRING(NUM, NH, NL)                              \
	r##NH = vis_fpmerge(fmask_0, vis_read_hi(r0##NUM));     \
	r##NL = vis_fpmerge(fmask_0, vis_read_lo(r0##NUM))

/* *********************************************************** */

#define	VIS_CALC_RES0(NUM, R1, R2)	res0##NUM = vis_fpsub16(r##R1, r##R2);
#define	VIS_CALC_RES1(NUM, R1, R2)	res1##NUM = vis_fpsub16(r##R1, r##R2);

#define	VIS_ADD(R1, R2, R3)	r##R1 = vis_fpadd16(r##R2, r##R3)

/* *********************************************************** */

#define	VIS_CALC_ROWS0                                          \
	INIT_N_STRING(1, 21, 22);                               \
	INIT_N_STRING(2, 23, 24);                               \
	VIS_ADD(25, 21, 23);                                    \
	VIS_ADD(26, 22, 24);                                    \
	INIT_E_STRING(0, 21, 22);                               \
	INIT_E_STRING(3, 23, 24);                               \
	VIS_ADD(19, 21, 23);                                    \
	VIS_ADD(20, 22, 24);                                    \
	VIS_CALC_RES0(1, 25, 19);                               \
	VIS_CALC_RES0(2, 26, 20)

/* *********************************************************** */

#define	VIS_CALC_ROWS1                                          \
	INIT_N_STRING(3, 21, 22);                               \
	INIT_N_STRING(4, 23, 24);                               \
	VIS_ADD(25, 21, 23);                                    \
	VIS_ADD(26, 22, 24);                                    \
	INIT_E_STRING(2, 21, 22);                               \
	INIT_E_STRING(5, 23, 24);                               \
	VIS_ADD(19, 21, 23);                                    \
	VIS_ADD(20, 22, 24);                                    \
	VIS_CALC_RES1(1, 25, 19);                               \
	VIS_CALC_RES1(2, 26, 20)

/* *********************************************************** */

#define	CALC_PIXEL(N, J)                                        \
	{                                                       \
	    mlib_s32 r1 =                                       \
		(BC_CF(cl##N[3] + cl##N[4]) - cl##N[2] -        \
		cl##N[5]) >> BC_SC2;                            \
	    pd##N[J] = SAT_U8(r1);                              \
	}                                                       \
	                                                        \
	{                                                       \
	    mlib_s32 r2 =                                       \
		(BC_CF(cl##N[5] + cl##N[6]) - cl##N[4] -        \
		cl##N[7]) >> BC_SC2;                            \
	    pd##N[J + 1] = SAT_U8(r2);                          \
	}                                                       \
	                                                        \
	{                                                       \
	    mlib_s32 r3 =                                       \
		(BC_CF(cl##N[7] + cl##N[8]) - cl##N[6] -        \
		cl##N[9]) >> BC_SC2;                            \
	    pd##N[J + 2] = SAT_U8(r3);                          \
	}                                                       \
	                                                        \
	{                                                       \
	    mlib_s32 r4 =                                       \
		(BC_CF(cl##N[9] + cl##N[10]) - cl##N[8] -       \
		cl##N[11]) >> BC_SC2;                           \
	    pd##N[J + 3] = SAT_U8(r4);                          \
	}

/* *********************************************************** */

#define	CALC_LAST_PIXEL(N, J)                                             \
	{                                                                 \
	    mlib_s32 jl, jl2;                                             \
	                                                                  \
	    for (jl = 0, jl2 = 2; jl < dst_width - J; jl++, jl2 += 2) {   \
		mlib_s32 r =                                              \
		    (BC_CF(cl##N[jl2 + 1] + cl##N[jl2 + 2]) -             \
		    cl##N[jl2] - cl##N[jl2 + 3]) >> BC_SC2;               \
		pd##N[J + jl] = SAT_U8(r);                                \
	    }                                                             \
	}

/* *********************************************************** */

#define	INIT_VARIABLE                                                     \
	MASK_U8_INIT                                                      \
	mlib_u8 *ps0 = p_src + (2 * i - 1) * src_stride - 1;              \
	mlib_u8 *ps1 = ps0 + src_stride;                                  \
	mlib_u8 *ps2 = ps1 + src_stride;                                  \
	mlib_u8 *ps3 = ps2 + src_stride;                                  \
	mlib_u8 *ps4 = ps3 + src_stride;                                  \
	mlib_u8 *ps5 = ps4 + src_stride;                                  \
	mlib_u8 *pd0 = p_dst + i * dst_stride;                            \
	mlib_u8 *pd1 = pd0 + dst_stride;                                  \
	mlib_d64 *psd0, *psd1, *psd2, *psd3, *psd4, *psd5;                \
	mlib_s32 k, j, jd, js;                                            \
	mlib_f32 fmask_0 = vis_fzeros();                                  \
	mlib_f32 f_mask_9 = vis_to_float(BC_CF(1) << 8);                  \
	mlib_d64 md0[3], md1[3];                                          \
	mlib_s16 *cl0 = (mlib_s16 *)md0, *cl1 = (mlib_s16 *)md1;          \
	mlib_d64 res01, res02, res11, res12;                              \
	mlib_d64 r00, r01, r02, r03, r04, r05, r07, r08, r09, r10, r11,   \
	    r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23,   \
	    r24, r25, r26

/* *********************************************************** */

#define	CALC_ROWS_BEGIN                                                \
	cl0[2] = BC_CF((mlib_s32)ps1[0] + ps2[0]) - ps0[0] - ps3[0];   \
	cl0[3] = BC_CF((mlib_s32)ps1[1] + ps2[1]) - ps0[1] - ps3[1];   \
	cl1[2] = BC_CF((mlib_s32)ps3[0] + ps4[0]) - ps2[0] - ps5[0];   \
	cl1[3] = BC_CF((mlib_s32)ps3[1] + ps4[1]) - ps2[1] - ps5[1]

#define	CALC_ROWS_BEGIN_NF                                                \
	cl0[2] = BC_CF((mlib_s32)vis_ld_u8_nf(ps1) + vis_ld_u8_nf(ps2)) - \
		vis_ld_u8_nf(ps0) - vis_ld_u8_nf(ps3);                    \
	cl0[3] = BC_CF((mlib_s32)vis_ld_u8_nf(ps1 + 1) + vis_ld_u8_nf(    \
		ps2 + 1)) - vis_ld_u8_nf(ps0 + 1) - vis_ld_u8_nf(ps3 + 1);\
	cl1[2] = BC_CF((mlib_s32)vis_ld_u8_nf(ps3) + vis_ld_u8_nf(ps4)) - \
		vis_ld_u8_nf(ps2) - vis_ld_u8_nf(ps5);                    \
	cl1[3] = BC_CF((mlib_s32)vis_ld_u8_nf(ps3 + 1) + vis_ld_u8_nf(    \
		ps4 + 1)) - vis_ld_u8_nf(ps2 + 1) - vis_ld_u8_nf(ps5 + 1)

/* *********************************************************** */

#define	LOAD_PR_INT_DATA                                        \
	md0[1] = res01;                                         \
	md0[2] = res02;                                         \
	md1[1] = res11;                                         \
	md1[2] = res12

/* *********************************************************** */

#define	LOAD_INT_DATA                                           \
	* ((mlib_s32 *)cl0 + 1) = *((mlib_s32 *)cl0 + 5);       \
	*((mlib_s32 *)cl1 + 1) = *((mlib_s32 *)cl1 + 5);        \
	LOAD_PR_INT_DATA

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Bicubic_XY(
    NEAREST_PARAMETERS(mlib_u8))
{
	LOAD_COEF();
	mlib_s32 i;

	if (dst_width < 12) {
		mlib_s32 l, res;
		MASK_U8_INIT
		INIT_BC_CYCLE_1(1, BC_CF, mlib_u8, mlib_s32);

#pragma pipeloop(0)
			BC_CYCLE_1(1, 2, SAT_U8, BC_CF, BC_SC2);
		}
		}
		return;
	}

	vis_write_gsr(1 << 3);
	for (i = 0; i < (dst_height & ~1); i += 2) {
		INIT_VARIABLE;
		js = 2;
		VIS_DATA_INIT;
		VIS_DATA_LOAD;
		CALC_ROWS_BEGIN;
		VIS_CALC_ROWS0;
		VIS_CALC_ROWS1;
		LOAD_PR_INT_DATA;
#pragma pipeloop(0)
		for (j = 1, jd = 0, js = 10; j < ((dst_width + 3) / 4);
		    j++, jd += 4, js += 8) {
			VIS_DATA_LOAD;
			VIS_CALC_ROWS0;
			VIS_CALC_ROWS1;
			CALC_PIXEL(0, jd);
			CALC_PIXEL(1, jd);
			LOAD_INT_DATA;
		}

		CALC_LAST_PIXEL(0, jd);
		CALC_LAST_PIXEL(1, jd);
	}

	if (dst_height & 1) {
		INIT_VARIABLE;
		js = 2;
		VIS_DATA_INIT;
		VIS_DATA_LOAD;
		CALC_ROWS_BEGIN_NF;
		VIS_CALC_ROWS0;
		LOAD_PR_INT_DATA;
#pragma pipeloop(0)
		for (j = 1, jd = 0, js = 10; j < ((dst_width + 3) / 4);
		    j++, jd += 4, js += 8) {
			VIS_DATA_LOAD;
			VIS_CALC_ROWS0;
			CALC_PIXEL(0, jd);
			LOAD_INT_DATA;
		}

		CALC_LAST_PIXEL(0, jd);
	}
}

/* *********************************************************** */

/*   u8 c1 bicubic2 1x4 part	   -1				*/
/*				x   5    / 8			*/
/*				    5				*/
/*				   -1				*/

#define	U8_1_Y_INIT_DATA                                        \
	INIT_ALIGN(0, 06, 07);                                  \
	INIT_ALIGN(1, 08, 09);                                  \
	INIT_ALIGN(2, 10, 11);                                  \
	INIT_ALIGN(3, 12, 13);                                  \
	INIT_ALIGN_NF(4, 14, 15);                               \
	INIT_ALIGN_NF(5, 16, 17)

/* *********************************************************** */

#define	U8_1_Y_LOAD_DATA                                        \
	VIS_ALIGN(0, 06, 07);                                   \
	VIS_ALIGN(1, 08, 09);                                   \
	VIS_ALIGN(2, 10, 11);                                   \
	VIS_ALIGN(3, 12, 13);                                   \
	VIS_ALIGN(4, 14, 15);                                   \
	VIS_ALIGN(5, 16, 17)

/* *********************************************************** */

#define	U8_1_Y_DATA_SHIFT                                       \
	ALIGN_SHIFT(0, 07, 06);                                 \
	ALIGN_SHIFT(1, 09, 08);                                 \
	ALIGN_SHIFT(2, 11, 10);                                 \
	ALIGN_SHIFT_NF(3, 13, 12);                              \
	ALIGN_SHIFT_NF(4, 15, 14);                              \
	ALIGN_SHIFT_NF(5, 17, 16)

/* *********************************************************** */

#define	VIS_AND(R1, NUM)	r##R1 = vis_fand(r0##NUM, dmask_0);
#define	VIS_MUL5(R1, R2)	r##R1 = vis_fmul8x16(f_mask_90909090, r##R2);

#define	VIS_MUL1(R1, R2)	r##R1 = vis_fmul8x16(f_mask_10101010, r##R2);

#define	VIS_RES(R1, R2, R3)	r##R1 = vis_fpsub16(r##R2, r##R3)

/* *********************************************************** */

#define	U8_1_Y_CALC_ROW0                                        \
	VIS_AND(18, 0);                                         \
	VIS_AND(19, 1);                                         \
	VIS_AND(20, 2);                                         \
	VIS_AND(21, 3);                                         \
	VIS_ADD(22, 19, 20);                                    \
	VIS_MUL5(23, 22);                                       \
	VIS_ADD(24, 18, 21);                                    \
	VIS_MUL1(25, 24);                                       \
	VIS_RES(26, 23, 25)

/* *********************************************************** */

#define	U8_1_Y_CALC_ROW1                                        \
	VIS_AND(18, 4);                                         \
	VIS_AND(19, 5);                                         \
	VIS_ADD(22, 18, 21);                                    \
	VIS_MUL5(23, 22);                                       \
	VIS_ADD(24, 20, 19);                                    \
	VIS_MUL1(25, 24);                                       \
	VIS_RES(27, 23, 25)

#define	U8_1_Y_CALC_ROW2

/* *********************************************************** */

#define	VIS_PACK(R1, R2)	md = vis_fpack16_pair(r##R1, r##R2); umd = md;
#define	U8_1_Y_INT_DATA_SAVE	VIS_PACK(26, 27)

/* *********************************************************** */

#define	U8_1_Y_SAVE_DIST_0(J)                                   \
	pd0[J + 0] = ud[0];                                     \
	pd0[J + 1] = ud[1];                                     \
	pd0[J + 2] = ud[2];                                     \
	pd0[J + 3] = ud[3]

/* *********************************************************** */

#define	U8_1_Y_SAVE_DIST(J)                                     \
	U8_1_Y_SAVE_DIST_0(J);                                  \
	pd1[J + 0] = ud[4];                                     \
	pd1[J + 1] = ud[5];                                     \
	pd1[J + 2] = ud[6];                                     \
	pd1[J + 3] = ud[7]

/* *********************************************************** */

#define	U8_1_Y_SAVE_LAST_DIST(J)                                \
	{                                                       \
	    mlib_s32 jl;                                        \
	                                                        \
	    for (jl = 0; jl < dst_width - J; jl++) {            \
		pd0[J + jl] = ud[jl];                           \
		pd1[J + jl] = ud[jl + 4];                       \
	    }                                                   \
	}

#define	U8_1_Y_SAVE_LAST_DIST_0(J)                              \
	{                                                       \
	    mlib_s32 jl;                                        \
	                                                        \
	    for (jl = 0; jl < dst_width - J; jl++) {            \
		pd0[J + jl] = ud[jl];                           \
	    }                                                   \
	}

/* *********************************************************** */

#define	U8_1_Y_INIT_VARIABLES                                             \
	mlib_f32 DMF_10101010;                                            \
	mlib_f32 DMF_90909090;                                            \
	mlib_u8 *ps0 = p_src + (2 * i - 1) * src_stride - 1;              \
	mlib_u8 *ps1 = ps0 + src_stride;                                  \
	mlib_u8 *ps2 = ps1 + src_stride;                                  \
	mlib_u8 *ps3 = ps2 + src_stride;                                  \
	mlib_u8 *ps4 = ps3 + src_stride;                                  \
	mlib_u8 *ps5 = ps4 + src_stride;                                  \
	mlib_u8 *pd0 = p_dst + i * dst_stride;                            \
	mlib_u8 *pd1 = pd0 + dst_stride;                                  \
	mlib_d64 *psd0, *psd1, *psd2, *psd3, *psd4, *psd5;                \
	mlib_d64 dmask_0 = vis_to_double_dup(0x00ff00ff);                 \
	mlib_d64 md, umd;                                                 \
	mlib_u8 *ud = (mlib_u8 *)&umd;                                    \
	mlib_s32 j, jd, js = 0;                                           \
	mlib_d64 r00, r01, r02, r03, r04, r05, r06, r07, r08, r09, r10,   \
	    r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22,   \
	    r23, r24, r25, r26, r27

/* *********************************************************** */

#define	U8_1_Y_PRE_FOR(N)                                       \
	U8_1_Y_INIT_VARIABLES;                                  \
	U8_1_Y_INIT_DATA;                                       \
	U8_1_Y_LOAD_DATA;                                       \
	U8_1_Y_DATA_SHIFT;                                      \
	U8_1_Y_CALC_ROW0;                                       \
	U8_1_Y_CALC_ROW##N;                                     \
	U8_1_Y_INT_DATA_SAVE

/* *********************************************************** */

#define	U8_1_Y_CYCLE_BODY(N, RW)                                \
	for (j = 0, jd = 0, js = 8; j < (dst_width) / 4;        \
	    j++, jd += 4, js += 8) {                            \
	    U8_1_Y_LOAD_DATA;                                   \
	    U8_1_Y_DATA_SHIFT;                                  \
	    U8_1_Y_CALC_ROW0;                                   \
	    U8_1_Y_CALC_ROW##N;                                 \
	    U8_1_Y_SAVE_##RW(jd);                               \
	    U8_1_Y_INT_DATA_SAVE;                               \
	}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Bicubic_Y(
    NEAREST_PARAMETERS(mlib_u8))
{
	LOAD_COEF();
	mlib_s32 i;

	if (dst_width < 16) {
		mlib_s32 l;
		MASK_U8_INIT
		INIT_BC_CYCLE_3(1, mlib_u8, mlib_s32);

#pragma pipeloop(0)
		BC_CYCLE_3(1, 2, SAT_U8, BC_CF, BC_SCL);
		return;
	}

	vis_write_gsr(7 << 3);
	for (i = 0; i < (dst_height & ~1); i += 2) {
		U8_1_Y_PRE_FOR(1);
#pragma pipeloop(0)
		U8_1_Y_CYCLE_BODY(1, DIST);
		U8_1_Y_SAVE_LAST_DIST(jd);
	}

	if (dst_height & 1) {
		U8_1_Y_PRE_FOR(2);

#pragma pipeloop(0)
		U8_1_Y_CYCLE_BODY(1, DIST_0);
		U8_1_Y_SAVE_LAST_DIST_0(jd);
	}
}

/* *********************************************************** */

/*    u8 c1 bicubic2 4x1 part    x    -1  5  5 -1    / 8	*/

/* *********************************************************** */

#define	U8_1_X_INIT_VARIABLES                                             \
	mlib_f32 DMF_10101010;                                            \
	mlib_f32 DMF_90909090;                                            \
	mlib_u8 *ps0 = p_src + (2 * i) * src_stride - 2;                  \
	mlib_u8 *pd0 = p_dst + i * dst_stride;                            \
	mlib_d64 *psd, *psd0, *psd1, *psd2, *psd3;                        \
	mlib_d64 dmask_0 = vis_to_double_dup(0x00ff00ff);                 \
	mlib_d64 md0, md1, umd[2];                                        \
	mlib_u8 *ud = (mlib_u8 *)umd;                                     \
	mlib_s32 j, jd, js = 0, pr_S, lc = (mlib_addr)pd0 & 7;            \
	mlib_d64 *pdd = (mlib_d64 *)(pd0 - lc), *pd =                     \
	    (lc) ? pdd + 1 : pdd;                                         \
	mlib_d64 r00, r01, r02, r03, r04, r05, r06, r07, r08, r09, r10,   \
	    r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22,   \
	    r23, r24, r25, r26, r27, r28;                                 \
	lc = (8 - lc) & ~8

/* *********************************************************** */

#define	U8_1_X_INIT_DATA                                          \
	pr_S = ((mlib_addr)ps0 & 4) ? ((mlib_addr)ps0 & 3) : 0;   \
	psd = (mlib_d64 *)vis_alignaddr(ps0, 0);                  \
	r24 = *(psd + 0);                                         \
	r25 = *(psd + 1);                                         \
	r26 = *(psd + 2);                                         \
	r27 = *(psd + 3);                                         \
	r28 = *(psd + 4);                                         \
	r04 = *(psd + 5)

/* *********************************************************** */

#define	X_VIS_ALIGN_E(NUM, R1, R2, R3, R4)                      \
	psd##NUM = (mlib_d64 *)vis_alignaddr(ps0 + NUM, 0);     \
	r##R1 = vis_faligndata(r24, r25);                       \
	r##R2 = vis_faligndata(r25, r26);                       \
	r##R3 = vis_faligndata(r26, r27);                       \
	r##R4 = vis_faligndata(r27, r28);

#define	X_VIS_ALIGN_S(NUM, R1, R2, R3, R4)                      \
	psd##NUM = (mlib_d64 *)vis_alignaddr(ps0 + NUM, 0);     \
	r##R1 = vis_faligndata(r25, r26);                       \
	r##R2 = vis_faligndata(r26, r27);                       \
	r##R3 = vis_faligndata(r27, r28);                       \
	r##R4 = vis_faligndata(r28, r04);

#define	U8_1_X_LOAD_DATA(N1, N2, N3, N4)                        \
	X_VIS_ALIGN_##N1(0, 00, 01, 02, 03);                    \
	X_VIS_ALIGN_##N2(1, 05, 06, 07, 08);                    \
	X_VIS_ALIGN_##N3(2, 09, 10, 11, 12);                    \
	X_VIS_ALIGN_##N4(3, 13, 14, 15, 16)

/* *********************************************************** */

#define	U8_1_X_DATA_SHIFT                                       \
	r24 = r28;                                              \
	r25 = r04;                                              \
	r26 = *(psd + js + 6);                                  \
	r27 = *(psd + js + 7);                                  \
	r28 = *(psd + js + 8);                                  \
	r04 = *(psd + js + 9)

/* *********************************************************** */

/*
 * #define VIS_RES(R1, R2, R3) r##R1 = vis_fpsub16(r##R2, r##R3);
 */
#define	VIS_PACK_P(R1, R2, R3)	R1 = vis_fpack16_pair(r##R2, r##R3);
#define	VIS_ANDX(R1, R2)	r##R1 = vis_fand(r##R2, dmask_0)

/* *********************************************************** */

#define	U8_1_X_CALC(RR, R1, R2, R3, R4)                         \
	VIS_ANDX(17, R1);                                       \
	VIS_ANDX(18, R2);                                       \
	VIS_ADD(19, 17, 18);                                    \
	VIS_MUL1(18, 19);                                       \
	VIS_ANDX(20, R3);                                       \
	VIS_ANDX(21, R4);                                       \
	VIS_ADD(22, 20, 21);                                    \
	VIS_MUL5(17, 22);                                       \
	VIS_RES(RR, 17, 18)

/* *********************************************************** */

#define	U8_1_X_CALC_ROW                                         \
	U8_1_X_CALC(23, 00, 13, 05, 09);                        \
	U8_1_X_CALC(19, 01, 14, 06, 10);                        \
	VIS_PACK_P(md0, 23, 19);                                \
	U8_1_X_CALC(23, 02, 15, 07, 11);                        \
	U8_1_X_CALC(19, 03, 16, 08, 12);                        \
	VIS_PACK_P(md1, 23, 19)

#define	U8_1_X_INT_DATA_SAVE	umd[0] = md0; umd[1] = md1

/* *********************************************************** */

#define	U8_1_X_SAVE_DIST(J)	pd[J] = umd[0]; pd[J+1] = umd[1]

/* *********************************************************** */

#define	U8_1_X_SAVE_LAST_DIST(J)                                \
	{                                                       \
	    mlib_s32 jl;                                        \
	                                                        \
	    J += j * 8;                                         \
	    for (jl = 0; jl < dst_width - J; jl++)              \
		pd0[J + jl] = ud[jl];                           \
	}

/* *********************************************************** */

#define	U8_1_X_C_ALIGN_FOR                                              \
	for (jd = 0; jd < lc; jd++) {                                   \
	    mlib_s32 res =                                              \
		(BC_CF(ps0[2] + ps0[3]) - ps0[1] - ps0[4]) >> BC_SCL;   \
	    ps0 += 2;                                                   \
	    pd0[jd] = SAT_U8(res);                                      \
	}

/* *********************************************************** */

#define	U8_1_X_PRE_FOR(N1, N2, N3, N4)                          \
	U8_1_X_LOAD_DATA(N1, N2, N3, N4);                       \
	U8_1_X_DATA_SHIFT;                                      \
	U8_1_X_CALC_ROW;                                        \
	U8_1_X_INT_DATA_SAVE

/* *********************************************************** */

#define	U8_1_X_CYCLE_BODY(N1, N2, N3, N4)                       \
	for (j = 0, js = 4; j < (dst_width - lc - 8) / 8;       \
	    j += 2, js += 4) {                                  \
	    U8_1_X_LOAD_DATA(N1, N2, N3, N4);                   \
	    U8_1_X_DATA_SHIFT;                                  \
	    U8_1_X_CALC_ROW;                                    \
	    U8_1_X_SAVE_DIST(j);                                \
	    U8_1_X_INT_DATA_SAVE;                               \
	}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Bicubic_X(
    NEAREST_PARAMETERS(mlib_u8))
{
	LOAD_COEF();
	mlib_s32 i;

	if (dst_width < 16) {
		mlib_s32 j, l, res;
		MASK_U8_INIT
		INIT_BC_CYCLE_2(1, mlib_u8, mlib_s32);

#pragma pipeloop(0)
			BC_CYCLE_2(1, 2, SAT_U8, BC_CF, BC_SCL);
			}
		}
		return;
	}

	vis_write_gsr(7 << 3);
	for (i = 0; i < dst_height; i++) {
		MASK_U8_INIT
		U8_1_X_INIT_VARIABLES;
		U8_1_X_C_ALIGN_FOR;
		U8_1_X_INIT_DATA;

		if (pr_S) {
			if (pr_S == 1) {
				U8_1_X_PRE_FOR(E, E, E, S);
#pragma pipeloop(0)
				U8_1_X_CYCLE_BODY(E, E, E, S);
			} else if (pr_S == 2) {
				U8_1_X_PRE_FOR(E, E, S, S);
#pragma pipeloop(0)
				U8_1_X_CYCLE_BODY(E, E, S, S);
			} else {
				U8_1_X_PRE_FOR(E, S, S, S);
#pragma pipeloop(0)
				U8_1_X_CYCLE_BODY(E, S, S, S);
			}

		} else {
			U8_1_X_PRE_FOR(E, E, E, E);
#pragma pipeloop(0)
			U8_1_X_CYCLE_BODY(E, E, E, E);
		}

		U8_1_X_SAVE_LAST_DIST(jd);
	}
}

/* *********************************************************** */

/*   u8 c2 bicubic2 1x4 part	   -1				*/
/*				x   5    / 8			*/
/*				    5				*/
/*				   -1				*/

#define	U8_2_Y_INIT_DATA                                        \
	INIT_ALIGN(0, 06, 07);                                  \
	INIT_ALIGN(1, 08, 09);                                  \
	INIT_ALIGN(2, 10, 11);                                  \
	INIT_ALIGN(3, 12, 13);                                  \
	INIT_ALIGN_NF(4, 14, 15);                               \
	INIT_ALIGN_NF(5, 16, 17)

/* *********************************************************** */

#define	U8_2_Y_LOAD_DATA                                        \
	VIS_ALIGN(0, 06, 07);                                   \
	VIS_ALIGN(1, 08, 09);                                   \
	VIS_ALIGN(2, 10, 11);                                   \
	VIS_ALIGN(3, 12, 13);                                   \
	VIS_ALIGN(4, 14, 15);                                   \
	VIS_ALIGN(5, 16, 17)

/* *********************************************************** */

#define	U8_2_Y_DATA_SHIFT                                       \
	ALIGN_SHIFT(0, 07, 06);                                 \
	ALIGN_SHIFT(1, 09, 08);                                 \
	ALIGN_SHIFT(2, 11, 10);                                 \
	ALIGN_SHIFT_NF(3, 13, 12);                              \
	ALIGN_SHIFT_NF(4, 15, 14);                              \
	ALIGN_SHIFT_NF(5, 17, 16)

/* *********************************************************** */

#define	VIS_EXPAND_MRG(R1, R2, R3)                              \
	r##R1 = vis_fpmerge(fmask_0, vis_read_hi(r0##R3));      \
	r##R2 = vis_fpmerge(fmask_0, vis_read_lo(r0##R3))

/* *********************************************************** */

#define	VIS_EXPAND_MUL(R1, R2, R3)                               \
	r##R1 = vis_fmul8x16al(vis_read_hi(r0##R3), f_mask_9);   \
	r##R2 = vis_fmul8x16al(vis_read_lo(r0##R3), f_mask_9)

/* *********************************************************** */

#define	U8_2_Y_CALC_ROW0                                        \
	VIS_EXPAND_MRG(18, 19, 0);                              \
	VIS_EXPAND_MUL(20, 21, 1);                              \
	VIS_EXPAND_MUL(22, 23, 2);                              \
	VIS_EXPAND_MRG(24, 25, 3);                              \
	VIS_ADD(26, 20, 22);                                    \
	VIS_ADD(27, 21, 23);                                    \
	VIS_ADD(20, 18, 24);                                    \
	VIS_ADD(22, 19, 25);                                    \
	VIS_RES(21, 26, 20);                                    \
	VIS_RES(23, 27, 22);                                    \
	VIS_PACK_P(md0, 21, 23)

/* *********************************************************** */

#define	U8_2_Y_CALC_ROW1                                        \
	VIS_EXPAND_MRG(18, 19, 2);                              \
	VIS_EXPAND_MUL(20, 21, 3);                              \
	VIS_EXPAND_MUL(22, 23, 4);                              \
	VIS_EXPAND_MRG(24, 25, 5);                              \
	VIS_ADD(26, 20, 22);                                    \
	VIS_ADD(27, 21, 23);                                    \
	VIS_ADD(20, 18, 24);                                    \
	VIS_ADD(22, 19, 25);                                    \
	VIS_RES(21, 26, 20);                                    \
	VIS_RES(23, 27, 22);                                    \
	VIS_PACK_P(md1, 21, 23)

#define	U8_2_Y_INT_DATA_SAVE	umd0 = md0; umd1 = md1

/* *********************************************************** */

#define	U8_2_Y_SAVE_DIST(J)                                     \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd0[J + 2] = ud0[4];                                    \
	pd0[J + 3] = ud0[5];                                    \
	pd1[J] = ud1[0];                                        \
	pd1[J + 1] = ud1[1];                                    \
	pd1[J + 2] = ud1[4];                                    \
	pd1[J + 3] = ud1[5]

/* *********************************************************** */

#define	U8_2_Y_SAVE_DIST0(J)                                    \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd0[J + 2] = ud0[4];                                    \
	pd0[J + 3] = ud0[5]

/* *********************************************************** */

#define	U8_2_Y_SAVE_LAST_DIST(J)                                \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd1[J] = ud1[0];                                        \
	pd1[J + 1] = ud1[1];                                    \
	if (J < dst_width2) {                                   \
	    pd0[J + 2] = ud0[4];                                \
	    pd0[J + 3] = ud0[5];                                \
	    pd1[J + 2] = ud1[4];                                \
	    pd1[J + 3] = ud1[5];                                \
	}

/* *********************************************************** */

#define	U8_2_Y_SAVE_LAST_DIST0(J)                               \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	if (J < dst_width2) {                                   \
	    pd0[J + 2] = ud0[4];                                \
	    pd0[J + 3] = ud0[5];                                \
	}

/* *********************************************************** */

#define	U8_2_Y_INIT_VARIABLES                                             \
	mlib_u8 *ps0 = p_src + (2 * i - 1) * src_stride;                  \
	mlib_u8 *ps1 = ps0 + src_stride;                                  \
	mlib_u8 *ps2 = ps1 + src_stride;                                  \
	mlib_u8 *ps3 = ps2 + src_stride;                                  \
	mlib_u8 *ps4 = ps3 + src_stride;                                  \
	mlib_u8 *ps5 = ps4 + src_stride;                                  \
	mlib_u8 *pd0 = p_dst + i * dst_stride;                            \
	mlib_u8 *pd1 = pd0 + dst_stride;                                  \
	mlib_d64 *psd0, *psd1, *psd2, *psd3, *psd4, *psd5;                \
	mlib_f32 f_mask_9 = vis_to_float(BC_CF(1) << 8);                  \
	mlib_f32 fmask_0 = vis_fzeros();                                  \
	mlib_d64 md0, md1, umd0, umd1;                                    \
	mlib_u8 *ud0 = (mlib_u8 *)&umd0, *ud1 = (mlib_u8 *)&umd1;         \
	mlib_s32 j, jd, js = 0, dst_width2 = (dst_width << 1) - 2;        \
	mlib_d64 r00, r01, r02, r03, r04, r05, r06, r07, r08, r09, r10,   \
	    r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22,   \
	    r23, r24, r25, r26, r27

#define	U8_2_Y_SAVE_PUSTO(J)

/* *********************************************************** */

#define	U8_2_Y_CALC_CUR	U8_2_Y_CALC_ROW0; U8_2_Y_CALC_ROW1

/* *********************************************************** */

#define	U8_2_Y_CALC_LAST	U8_2_Y_CALC_ROW0

/* *********************************************************** */

#define	U8_2_Y_CYCLE_BODY(DIST, PRZ)                            \
	U8_2_Y_LOAD_DATA;                                       \
	U8_2_Y_DATA_SHIFT;                                      \
	U8_2_Y_CALC##PRZ;                                       \
	U8_2_Y_SAVE##DIST(jd);                                  \
	U8_2_Y_INT_DATA_SAVE

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Bicubic_Y(
    NEAREST_PARAMETERS(mlib_u8))
{
	LOAD_COEF();
	mlib_s32 i;

	if (dst_width < 8) {
		mlib_s32 l;
		MASK_U8_INIT
		INIT_BC_CYCLE_3(2, mlib_u8, mlib_s32);

#pragma pipeloop(0)
		BC_CYCLE_3(2, 4, SAT_U8, BC_CF, BC_SCL);
		return;
	}

	vis_write_gsr((7 - BC_SCL) << 3);
	for (i = 0; i < (dst_height & ~1); i += 2) {
		U8_2_Y_INIT_VARIABLES;
		U8_2_Y_INIT_DATA;
		U8_2_Y_CYCLE_BODY(_PUSTO, _CUR);
#pragma pipeloop(0)
		for (j = 0, jd = 0, js = 8; j < (dst_width - 1) / 2;
		    j++, js += 8, jd += 4) {
			U8_2_Y_CYCLE_BODY(_DIST, _CUR);
		}

		U8_2_Y_SAVE_LAST_DIST(jd);
	}

	if (dst_height & 1) {
		U8_2_Y_INIT_VARIABLES;
		U8_2_Y_INIT_DATA;
		U8_2_Y_CYCLE_BODY(_PUSTO, _LAST);
#pragma pipeloop(0)
		for (j = 0, jd = 0, js = 8; j < (dst_width - 1) / 2;
		    j++, js += 8, jd += 4) {
			U8_2_Y_CYCLE_BODY(_DIST0, _LAST);
		}

		U8_2_Y_SAVE_LAST_DIST0(jd);
	}
}

/* *********************************************************** */

/*   u8 c3 bicubic2 1x4 part	   -1				*/
/*				x   5    / 8			*/
/*				    5				*/
/*				   -1				*/

#define	U8_3_Y_INIT_DATA                                        \
	INIT_ALIGN(0, 06, 07);                                  \
	INIT_ALIGN(1, 08, 09);                                  \
	INIT_ALIGN(2, 10, 11);                                  \
	INIT_ALIGN(3, 12, 13);                                  \
	INIT_ALIGN_NF(4, 14, 15);                               \
	INIT_ALIGN_NF(5, 16, 17)

/* *********************************************************** */

#define	U8_3_Y_LOAD_DATA                                        \
	VIS_ALIGN(0, 06, 07);                                   \
	VIS_ALIGN(1, 08, 09);                                   \
	VIS_ALIGN(2, 10, 11);                                   \
	VIS_ALIGN(3, 12, 13);                                   \
	VIS_ALIGN(4, 14, 15);                                   \
	VIS_ALIGN(5, 16, 17)

/* *********************************************************** */

#define	U8_3_Y_DATA_SHIFT                                       \
	ALIGN_SHIFT(0, 07, 06);                                 \
	ALIGN_SHIFT(1, 09, 08);                                 \
	ALIGN_SHIFT(2, 11, 10);                                 \
	ALIGN_SHIFT_NF(3, 13, 12);                              \
	ALIGN_SHIFT_NF(4, 15, 14);                              \
	ALIGN_SHIFT_NF(5, 17, 16)

/* *********************************************************** */

#define	U8_3_Y_CALC_ROW0                                        \
	VIS_EXPAND_MRG(18, 19, 0);                              \
	VIS_EXPAND_MUL(20, 21, 1);                              \
	VIS_EXPAND_MUL(22, 23, 2);                              \
	VIS_EXPAND_MRG(24, 25, 3);                              \
	VIS_ADD(26, 20, 22);                                    \
	VIS_ADD(27, 21, 23);                                    \
	VIS_ADD(20, 18, 24);                                    \
	VIS_ADD(22, 19, 25);                                    \
	VIS_RES(21, 26, 20);                                    \
	VIS_RES(23, 27, 22);                                    \
	VIS_PACK_P(md0, 21, 23)

/* *********************************************************** */

#define	U8_3_Y_CALC_ROW1                                        \
	VIS_EXPAND_MRG(18, 19, 2);                              \
	VIS_EXPAND_MUL(20, 21, 3);                              \
	VIS_EXPAND_MUL(22, 23, 4);                              \
	VIS_EXPAND_MRG(24, 25, 5);                              \
	VIS_ADD(26, 20, 22);                                    \
	VIS_ADD(27, 21, 23);                                    \
	VIS_ADD(20, 18, 24);                                    \
	VIS_ADD(22, 19, 25);                                    \
	VIS_RES(21, 26, 20);                                    \
	VIS_RES(23, 27, 22);                                    \
	VIS_PACK_P(md1, 21, 23)

#define	U8_3_Y_CALC_ROW2

#define	U8_3_Y_INT_DATA_SAVE	umd0 = md0; umd1 = md1

/* *********************************************************** */

#define	U8_3_Y_SAVE_DIST_0(J)                                   \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd0[J + 2] = ud0[2];                                    \
	pd0[J + 3] = ud0[6];                                    \
	pd0[J + 4] = ud0[7];                                    \
	pd1[J] = ud1[0];                                        \
	pd1[J + 1] = ud1[1];                                    \
	pd1[J + 2] = ud1[2];                                    \
	pd1[J + 3] = ud1[6];                                    \
	pd1[J + 4] = ud1[7]

/* *********************************************************** */

#define	U8_3_Y_SAVE_DIST_1(J)                                   \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[4];                                    \
	pd0[J + 2] = ud0[5];                                    \
	pd0[J + 3] = ud0[6];                                    \
	pd1[J] = ud1[0];                                        \
	pd1[J + 1] = ud1[4];                                    \
	pd1[J + 2] = ud1[5];                                    \
	pd1[J + 3] = ud1[6]

/* *********************************************************** */

#define	U8_3_Y_SAVE_DIST_2(J)                                   \
	pd0[J] = ud0[2];                                        \
	pd0[J + 1] = ud0[3];                                    \
	pd0[J + 2] = ud0[4];                                    \
	pd1[J] = ud1[2];                                        \
	pd1[J + 1] = ud1[3];                                    \
	pd1[J + 2] = ud1[4]

/* *********************************************************** */

#define	U8_3_Y_SAVE_DIST_0_0(J)                                 \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd0[J + 2] = ud0[2];                                    \
	pd0[J + 3] = ud0[6];                                    \
	pd0[J + 4] = ud0[7]

/* *********************************************************** */

#define	U8_3_Y_SAVE_DIST_1_0(J)                                 \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[4];                                    \
	pd0[J + 2] = ud0[5];                                    \
	pd0[J + 3] = ud0[6]

/* *********************************************************** */

#define	U8_3_Y_SAVE_DIST_2_0(J)                                 \
	pd0[J] = ud0[2];                                        \
	pd0[J + 1] = ud0[3];                                    \
	pd0[J + 2] = ud0[4]

/* *********************************************************** */

#define	U8_3_Y_SAVE_LAST_DIST_1(J)                              \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd0[J + 2] = ud0[2];                                    \
	pd1[J] = ud1[0];                                        \
	pd1[J + 1] = ud1[1];                                    \
	pd1[J + 2] = ud1[2]

/* *********************************************************** */

#define	U8_3_Y_SAVE_LAST_DIST_2(J)                              \
	pd0[J + 0] = ud0[0];                                    \
	pd1[J + 0] = ud1[0]

/* *********************************************************** */

#define	U8_3_Y_SAVE_LAST_DIST_1_0(J)                            \
	pd0[J] = ud0[0];                                        \
	pd0[J + 1] = ud0[1];                                    \
	pd0[J + 2] = ud0[2]

/* *********************************************************** */

#define	U8_3_Y_SAVE_LAST_DIST_2_0(J)	pd0[J+0] = ud0[0]

/* *********************************************************** */

#define	U8_3_Y_INIT_VARIABLES                                             \
	mlib_u8 *ps0 = p_src + (2 * i - 1) * src_stride;                  \
	mlib_u8 *ps1 = ps0 + src_stride;                                  \
	mlib_u8 *ps2 = ps1 + src_stride;                                  \
	mlib_u8 *ps3 = ps2 + src_stride;                                  \
	mlib_u8 *ps4 = ps3 + src_stride;                                  \
	mlib_u8 *ps5 = ps4 + src_stride;                                  \
	mlib_u8 *pd0 = p_dst + i * dst_stride;                            \
	mlib_u8 *pd1 = pd0 + dst_stride;                                  \
	mlib_d64 *psd0, *psd1, *psd2, *psd3, *psd4, *psd5;                \
	mlib_f32 f_mask_9 = vis_to_float(BC_CF(1) << 8);                  \
	mlib_f32 fmask_0 = vis_fzeros();                                  \
	mlib_d64 md0, md1, umd0, umd1;                                    \
	mlib_u8 *ud0 = (mlib_u8 *)&umd0, *ud1 = (mlib_u8 *)&umd1;         \
	mlib_s32 j, jd, js = 0;                                           \
	mlib_d64 r00, r01, r02, r03, r04, r05, r06, r07, r08, r09, r10,   \
	    r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22,   \
	    r23, r24, r25, r26, r27, r28

/* *********************************************************** */

#define	U8_3_Y_CYCLE_BODY(R, N, D)                              \
	U8_3_Y_LOAD_DATA;                                       \
	U8_3_Y_DATA_SHIFT;                                      \
	U8_3_Y_CALC_ROW0;                                       \
	U8_3_Y_CALC_ROW##R;                                     \
	U8_3_Y_SAVE_DIST##N(jd);                                \
	U8_3_Y_INT_DATA_SAVE;                                   \
	js += 8;                                                \
	jd += D

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Bicubic_Y(
    NEAREST_PARAMETERS(mlib_u8))
{
	LOAD_COEF();
	mlib_s32 i;
	mlib_s32 pr = (dst_width) & 3;

	if (dst_width < 16) {
		mlib_s32 l;
		MASK_U8_INIT
		INIT_BC_CYCLE_3(3, mlib_u8, mlib_s32);

#pragma pipeloop(0)
		BC_CYCLE_3(3, 6, SAT_U8, BC_CF, BC_SCL);
		return;
	}

	vis_write_gsr((7 - BC_SCL) << 3);
	for (i = 0; i < (dst_height & ~1); i += 2) {
		U8_3_Y_INIT_VARIABLES;
		U8_3_Y_INIT_DATA;
		U8_3_Y_LOAD_DATA;
		U8_3_Y_DATA_SHIFT;
		U8_3_Y_CALC_ROW0;
		U8_3_Y_CALC_ROW1;
		U8_3_Y_INT_DATA_SAVE;
#pragma pipeloop(0)
		for (j = 0, jd = 0, js = 8; j < (dst_width) / 4; j++) {
			U8_3_Y_CYCLE_BODY(1, _0, 5);
			U8_3_Y_CYCLE_BODY(1, _1, 4);
			U8_3_Y_CYCLE_BODY(1, _2, 3);
		}

		if (pr) {
			if (pr == 1) {
				U8_3_Y_SAVE_LAST_DIST_1(jd);
			} else if (pr == 2) {
				U8_3_Y_CYCLE_BODY(1, _0, 5);
				U8_3_Y_SAVE_LAST_DIST_2(jd);
			} else {
				U8_3_Y_CYCLE_BODY(1, _0, 5);
				U8_3_Y_CYCLE_BODY(1, _1, 4);
			}
		}
	}

	if (dst_height & 1) {
		U8_3_Y_INIT_VARIABLES;
		U8_3_Y_INIT_DATA;
		U8_3_Y_LOAD_DATA;
		U8_3_Y_DATA_SHIFT;
		U8_3_Y_CALC_ROW0;
		U8_3_Y_INT_DATA_SAVE;
#pragma pipeloop(0)
		for (j = 0, jd = 0, js = 8; j < (dst_width) / 4; j++) {
			U8_3_Y_CYCLE_BODY(2, _0_0, 5);
			U8_3_Y_CYCLE_BODY(2, _1_0, 4);
			U8_3_Y_CYCLE_BODY(2, _2_0, 3);
		}

		if (pr) {
			if (pr == 1) {
				U8_3_Y_SAVE_LAST_DIST_1_0(jd);
			} else if (pr == 2) {
				U8_3_Y_CYCLE_BODY(2, _0_0, 5);
				U8_3_Y_SAVE_LAST_DIST_2_0(jd);
			} else {
				U8_3_Y_CYCLE_BODY(2, _0_0, 5);
				U8_3_Y_CYCLE_BODY(2, _1_0, 4);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_1_Bicubic_XY(PARAM_BC);
	else if (dw)
		mlib_ImageZoomOut2X_U8_1_Bicubic_Y(PARAM_BC);
	else if (dh)
		mlib_ImageZoomOut2X_U8_1_Bicubic_X(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_2_Bicubic_XY(PARAM_BC);
	else if (dw)
		mlib_ImageZoomOut2X_U8_2_Bicubic_Y(PARAM_BC);
	else if (dh)
		mlib_ImageZoomOut2X_U8_2_Bicubic_X(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_3_Bicubic_XY(PARAM_BC);
	else if (dw)
		mlib_ImageZoomOut2X_U8_3_Bicubic_Y(PARAM_BC);
	else if (dh)
		mlib_ImageZoomOut2X_U8_3_Bicubic_X(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_4_Bicubic_XY(PARAM_BC);
	else if (dw)
		mlib_ImageZoomOut2X_U8_4_Bicubic_Y(PARAM_BC);
	else if (dh)
		mlib_ImageZoomOut2X_U8_4_Bicubic_X(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_1_Bicubic_XY(PARAM_BC2);
	else if (dw)
		mlib_ImageZoomOut2X_U8_1_Bicubic_Y(PARAM_BC2);
	else if (dh)
		mlib_ImageZoomOut2X_U8_1_Bicubic_X(PARAM_BC2);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_2_Bicubic_XY(PARAM_BC2);
	else if (dw)
		mlib_ImageZoomOut2X_U8_2_Bicubic_Y(PARAM_BC2);
	else if (dh)
		mlib_ImageZoomOut2X_U8_2_Bicubic_X(PARAM_BC2);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_3_Bicubic_XY(PARAM_BC2);
	else if (dw)
		mlib_ImageZoomOut2X_U8_3_Bicubic_Y(PARAM_BC2);
	else if (dh)
		mlib_ImageZoomOut2X_U8_3_Bicubic_X(PARAM_BC2);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_u8))
{
	if (dw == dh)
		mlib_ImageZoomOut2X_U8_4_Bicubic_XY(PARAM_BC2);
	else if (dw)
		mlib_ImageZoomOut2X_U8_4_Bicubic_Y(PARAM_BC2);
	else if (dh)
		mlib_ImageZoomOut2X_U8_4_Bicubic_X(PARAM_BC2);
}

/* *********************************************************** */
