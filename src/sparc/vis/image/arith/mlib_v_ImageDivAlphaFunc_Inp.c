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

#pragma ident	"@(#)mlib_v_ImageDivAlphaFunc_Inp.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_1(mlib_d64 *src_dst,
 *                                               mlib_s32  size);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_2(mlib_d64 *src_dst,
 *                                               mlib_s32  size);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_3(mlib_d64 *src_dst,
 *                                               mlib_s32  size);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_4(mlib_d64 *src_dst,
 *                                               mlib_s32  size);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_1(mlib_d64 *src_dst,
 *                                                 mlib_s32  src_dst_stride,
 *                                                 mlib_s32  size,
 *                                                 mlib_s32  height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_2(mlib_d64 *src_dst,
 *                                                 mlib_s32  src_dst_stride,
 *                                                 mlib_s32  size,
 *                                                 mlib_s32  height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_3(mlib_d64 *src_dst,
 *                                                 mlib_s32  src_dst_stride,
 *                                                 mlib_s32  size,
 *                                                 mlib_s32  height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_4(mlib_d64 *src_dst,
 *                                                 mlib_s32  src_dst_stride,
 *                                                 mlib_s32  size,
 *                                                 mlib_s32  height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_1(mlib_d64 *src_dst,
 *                                               mlib_s32  size);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_2(mlib_d64 *src_dst,
 *                                               mlib_s32  size);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_1(mlib_d64 *src_dst,
 *                                                 mlib_s32  stride,
 *                                                 mlib_s32  size,
 *                                                 mlib_s32  height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_2(mlib_d64 *src_dst,
 *                                                 mlib_s32  stride,
 *                                                 mlib_s32  size,
 *                                                 mlib_s32  height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_2_OFF_1(mlib_u8 *s_data,
 *                                             mlib_s32 stride,
 *                                             mlib_s32 width,
 *                                             mlib_s32 height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_2_OFF_2(mlib_u8 *s_data,
 *                                             mlib_s32 stride,
 *                                             mlib_s32 width,
 *                                             mlib_s32 height);
 *
 *     void mlib_v_ImageDivAlpha_Inp_S16_2(mlib_s16 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_S32_2(mlib_s32 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U8_3(mlib_u8 *src,
 *                                       mlib_s32 s_stride,
 *                                       mlib_s32 width,
 *                                       mlib_s32 height,
 *                                       mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_S16_3(mlib_s16 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_S32_3(mlib_s32 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_S16_4(mlib_s16 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_S32_4(mlib_s32 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U16_4(mlib_u16 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U16_3(mlib_u16 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageDivAlpha_Inp_U16_2(mlib_u16 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageDivAlpha.h>

/* *********************************************************** */

#define	FL_MIN	1e-20F
#define	DB_MIN	1e-100

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	SAT8(DST, SRC)	DST = (mlib_s32) (SRC) >> 23

/* *********************************************************** */

#define	SAT16(DST, SRC)	DST = (mlib_s32) (SRC) >> 16

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)(SRC - sat)) >> 16) ^ 0x8000

/* *********************************************************** */

#define	SAT32(DST, SRC)	DST = (mlib_s32) (SRC)

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT8(DST, SRC)                                          \
	{                                                       \
	    mlib_f32 sat8_tmp = (SRC);                          \
	                                                        \
	    if (sat8_tmp >= MLIB_U8_MAX)                        \
		sat8_tmp = MLIB_U8_MAX;                         \
	    DST = (mlib_u8)sat8_tmp;                            \
	}

/* *********************************************************** */

#define	SAT16(DST, SRC)                                         \
	{                                                       \
	    mlib_f32 sat16_tmp = (SRC);                         \
	                                                        \
	    if (sat16_tmp >= MLIB_S16_MAX)                      \
		sat16_tmp = MLIB_S16_MAX;                       \
	    if (sat16_tmp <= MLIB_S16_MIN)                      \
		sat16_tmp = MLIB_S16_MIN;                       \
	    DST = (mlib_s32)sat16_tmp;                          \
	}

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	{                                                       \
	    mlib_f32 sat16_tmp = (SRC);                         \
	                                                        \
	    if (sat16_tmp >= MLIB_U16_MAX)                      \
		sat16_tmp = MLIB_U16_MAX;                       \
	    if (sat16_tmp <= MLIB_U16_MIN)                      \
		sat16_tmp = MLIB_U16_MIN;                       \
	    DST = ((mlib_s32)sat16_tmp);                        \
	}

/* *********************************************************** */

#define	SAT32(DST, SRC)                                         \
	{                                                       \
	    mlib_d64 sat32_tmp = (SRC);                         \
	                                                        \
	    if (sat32_tmp >= MLIB_S32_MAX)                      \
		sat32_tmp = MLIB_S32_MAX;                       \
	    if (sat32_tmp <= MLIB_S32_MIN)                      \
		sat32_tmp = MLIB_S32_MIN;                       \
	    DST = (mlib_s32)sat32_tmp;                          \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MACR_DIV_NOOFF_U8(table)                                      \
	for (i = 0; i < size; i++) {                                  \
	    p_src_dst = *src_dst;                                     \
	    alphas = vis_fand(p_src_dst, emask);                      \
	    alpha_1 = ((mlib_d64 *)table)[*alpha];                    \
	    dst_hi = vis_fmul8x16(vis_read_hi(p_src_dst), alpha_1);   \
	    dst_hi = vis_fand(dst_hi, minus_mask);                    \
	    alpha_2 = ((mlib_d64 *)table)[*(alpha + 4)];              \
	    dst_lo = vis_fmul8x16(vis_read_lo(p_src_dst), alpha_2);   \
	    dst_lo = vis_fand(dst_lo, minus_mask);                    \
	    result = vis_fpack16_pair(dst_hi, dst_lo);                \
	    *src_dst = vis_for(result, alphas);                       \
	    alpha += 8;                                               \
	    src_dst++;                                                \
	}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_s32 i;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;

	emask = vis_to_double_dup(0xFF000000);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);
	alpha = (mlib_u8 *)src_dst;
	size >>= 3;
#pragma pipeloop(0)
	MACR_DIV_NOOFF_U8(mlib_v_tab1);
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_s32 i;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;

	emask = vis_to_double_dup(0x00FF0000);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);
	alpha = 1 + (mlib_u8 *)src_dst;
	size >>= 3;
#pragma pipeloop(0)
	MACR_DIV_NOOFF_U8(mlib_v_tab2);
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_s32 i;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;

	emask = vis_to_double_dup(0x0000FF00);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);
	alpha = 2 + (mlib_u8 *)src_dst;
	size >>= 3;
#pragma pipeloop(0)
	MACR_DIV_NOOFF_U8(mlib_v_tab3);
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_s32 i;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;

	emask = vis_to_double_dup(0x000000FF);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);
	alpha = 3 + (mlib_u8 *)src_dst;
	size >>= 3;
#pragma pipeloop(0)
	MACR_DIV_NOOFF_U8(mlib_v_tab4);
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;
	mlib_s32 src_dst_offset, i, j;

	emask = vis_to_double_dup(0xFF000000);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);

	size >>= 1;

	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
		alpha = (mlib_u8 *)src_dst;
#pragma pipeloop(0)
		MACR_DIV_NOOFF_U8(mlib_v_tab1);
		src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;
	mlib_s32 src_dst_offset, i, j;

	emask = vis_to_double_dup(0x00FF0000);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);

	size >>= 1;

	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
		alpha = 1 + (mlib_u8 *)src_dst;
#pragma pipeloop(0)
		MACR_DIV_NOOFF_U8(mlib_v_tab2);
		src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;
	mlib_s32 src_dst_offset, i, j;

	emask = vis_to_double_dup(0x0000FF00);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);

	size >>= 1;

	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
		alpha = 2 + (mlib_u8 *)src_dst;
#pragma pipeloop(0)
		MACR_DIV_NOOFF_U8(mlib_v_tab3);
		src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask;
	mlib_d64 dst_hi, dst_lo;
	mlib_s32 src_dst_offset, i, j;

	emask = vis_to_double_dup(0x000000FF);
	minus_mask = vis_to_double_dup(0x7FFF7FFF);
	vis_write_gsr(56);

	size >>= 1;

	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
		alpha = 3 + (mlib_u8 *)src_dst;
#pragma pipeloop(0)
		MACR_DIV_NOOFF_U8(mlib_v_tab4);
		src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

#undef MACR_DIV_NOOFF_U8
#define	MACR_DIV_NOOFF_U8(table)                                      \
	for (i = 0; i < size; i++) {                                  \
	    p_src_dst = *src_dst;                                     \
	    alphas = vis_fand(p_src_dst, emask);                      \
	    alpha_1 = ((mlib_d64 *)table)[*alpha];                    \
	    dst_h = vis_fmul8x16(vis_read_hi(p_src_dst), alpha_1);    \
	    dst_h = vis_fand(dst_h, minus_mask_1);                    \
	    alpha_2 = ((mlib_d64 *)table)[*(alpha + 2)];              \
	    dst_hi = vis_fmul8x16(vis_read_hi(p_src_dst), alpha_2);   \
	    dst_hi = vis_fand(dst_hi, minus_mask_2);                  \
	    dst_hi = vis_for(dst_hi, dst_h);                          \
	    alpha_1 = ((mlib_d64 *)table)[*(alpha + 4)];              \
	    dst_l = vis_fmul8x16(vis_read_lo(p_src_dst), alpha_1);    \
	    dst_l = vis_fand(dst_l, minus_mask_1);                    \
	    alpha_2 = ((mlib_d64 *)table)[*(alpha + 6)];              \
	    dst_lo = vis_fmul8x16(vis_read_lo(p_src_dst), alpha_2);   \
	    dst_lo = vis_fand(dst_lo, minus_mask_2);                  \
	    dst_lo = vis_for(dst_lo, dst_l);                          \
	    result = vis_fpack16_pair(dst_hi, dst_lo);                \
	    *src_dst = vis_for(result, alphas);                       \
	    alpha += 8;                                               \
	    src_dst++;                                                \
	}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_s32 i;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask_1, minus_mask_2;
	mlib_d64 dst_hi, dst_lo, dst_h, dst_l;

	emask = vis_to_double_dup(0xFF00FF00);
	minus_mask_1 = vis_to_double(0x00007FFF, 0x00000000);
	minus_mask_2 = vis_to_double(0x00000000, 0x00007FFF);
	vis_write_gsr(56);
	alpha = (mlib_u8 *)src_dst;
	size >>= 3;
#pragma pipeloop(0)
	MACR_DIV_NOOFF_U8(mlib_v_tab1);
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_s32 i;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask_1, minus_mask_2;
	mlib_d64 dst_hi, dst_lo, dst_h, dst_l;

	emask = vis_to_double_dup(0x00FF00FF);
	minus_mask_1 = vis_to_double(0x7FFF0000, 0x00000000);
	minus_mask_2 = vis_to_double(0x00000000, 0x7FFF0000);
	vis_write_gsr(56);
	alpha = 1 + (mlib_u8 *)src_dst;
	size >>= 3;
#pragma pipeloop(0)
	MACR_DIV_NOOFF_U8(mlib_v_tab2);
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask_1, minus_mask_2;
	mlib_d64 dst_hi, dst_lo, dst_h, dst_l;
	mlib_s32 src_dst_offset, i, j;

	emask = vis_to_double_dup(0xFF00FF00);
	minus_mask_1 = vis_to_double(0x00007FFF, 0x00000000);
	minus_mask_2 = vis_to_double(0x00000000, 0x00007FFF);
	vis_write_gsr(56);

	size >>= 2;

	src_dst_offset = (stride >> 3) - size;

	for (j = 0; j < height; j++) {
		alpha = (mlib_u8 *)src_dst;
#pragma pipeloop(0)
		MACR_DIV_NOOFF_U8(mlib_v_tab1);
		src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 *alpha;
	mlib_d64 p_src_dst, result;
	mlib_d64 alpha_1, alpha_2, alphas, emask, minus_mask_1, minus_mask_2;
	mlib_d64 dst_hi, dst_lo, dst_h, dst_l;
	mlib_s32 src_dst_offset, i, j;

	emask = vis_to_double_dup(0x00FF00FF);
	minus_mask_1 = vis_to_double(0x7FFF0000, 0x00000000);
	minus_mask_2 = vis_to_double(0x00000000, 0x7FFF0000);
	vis_write_gsr(56);

	size >>= 2;

	src_dst_offset = (stride >> 3) - size;

	for (j = 0; j < height; j++) {
		alpha = (mlib_u8 *)src_dst + 1;
#pragma pipeloop(0)
		MACR_DIV_NOOFF_U8(mlib_v_tab2);
		src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

#define	MACR_DIV_OFF_U8(p_src_dst, result, table, Const)          \
	alpha = Const + (mlib_u8 *)&p_src_dst;                    \
	alphas = vis_fand(p_src_dst, e_mask);                     \
	alpha_1 = ((mlib_d64 *)table)[*alpha];                    \
	dst_h = vis_fmul8x16(vis_read_hi(p_src_dst), alpha_1);    \
	dst_h = vis_fand(dst_h, minus_mask_1);                    \
	alpha_2 = ((mlib_d64 *)table)[*(alpha + 2)];              \
	dst_hi = vis_fmul8x16(vis_read_hi(p_src_dst), alpha_2);   \
	dst_hi = vis_fand(dst_hi, minus_mask_2);                  \
	dst_hi = vis_for(dst_hi, dst_h);                          \
	alpha_1 = ((mlib_d64 *)table)[*(alpha + 4)];              \
	dst_l = vis_fmul8x16(vis_read_lo(p_src_dst), alpha_1);    \
	dst_l = vis_fand(dst_l, minus_mask_1);                    \
	alpha_2 = ((mlib_d64 *)table)[*(alpha + 6)];              \
	dst_lo = vis_fmul8x16(vis_read_lo(p_src_dst), alpha_2);   \
	dst_lo = vis_fand(dst_lo, minus_mask_2);                  \
	dst_lo = vis_for(dst_lo, dst_l);                          \
	result = vis_fpack16_pair(dst_hi, dst_lo);                \
	result = vis_for(result, alphas)

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_2_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *s_line, *s_begin, *s_end, *help;
	mlib_d64 *s_address;
	mlib_d64 s_0, s_1, result, minus_mask_1, minus_mask_2;
	mlib_d64 s_dd0, s_dd1, s_dd2, s_dd3, s_dd4, s_dd5;
	mlib_s32 i, j, n;
	mlib_s32 emask, no_emask;

	mlib_u8 *alpha;
	mlib_d64 alpha_1, alpha_2, alphas, e_mask;
	mlib_d64 dst_hi, dst_lo, dst_h, dst_l;

	e_mask = vis_to_double_dup(0xFF00FF00);
	minus_mask_1 = vis_to_double(0x00007FFF, 0x00000000);
	minus_mask_2 = vis_to_double(0x00000000, 0x00007FFF);
	vis_write_gsr(56);

	s_line = s_begin = s_data;

	for (j = 0; j < height; j++) {
/* prepare the destination address */
		s_address = (mlib_d64 *)vis_alignaddr(s_line, 0);
		s_end = s_line + (width << 1) - 1;
		emask = vis_edge8(s_line, s_address + 1);
		no_emask = ~emask;

/* 48-pixel column loop */
		n = (mlib_s32)(s_end - s_begin) / 48;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd0 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd1 = vis_faligndata(s_1, s_0);
			s_1 = s_address[3];
			s_dd2 = vis_faligndata(s_0, s_1);
			s_0 = s_address[4];
			s_dd3 = vis_faligndata(s_1, s_0);
			s_1 = s_address[5];
			s_dd4 = vis_faligndata(s_0, s_1);
			s_0 = s_address[6];
			s_dd5 = vis_faligndata(s_1, s_0);
			s_begin += 48;

			MACR_DIV_OFF_U8(s_dd0, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd1, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd2, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd3, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd4, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd5, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);
		}

/* 8-pixel column loop  */
		n = (mlib_s32)(s_end - s_begin + 1) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
/* load 8 bytes of source data */
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd0 = vis_faligndata(s_0, s_1);
			s_begin += 8;

			MACR_DIV_OFF_U8(s_dd0, result, mlib_v_tab1, 0);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address += 1;
			if (emask != 0xff)
				vis_pst_8(result, s_address, no_emask);
		}

/* last 8-pixel column */
		n = s_end - s_begin;

		if (n >= 0) {
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd0 = vis_faligndata(s_0, s_1);
			vis_write_gsr(56);
			MACR_DIV_OFF_U8(s_dd0, result, mlib_v_tab1, 0);
			help = (mlib_u8 *)&result;
			while (n > 0) {
				*s_end = help[n];
				n -= 2;
				s_end -= 2;
			}
		}

		s_line = s_begin = s_line + stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_2_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *s_line, *s_begin, *s_end, *help;
	mlib_d64 *s_address;
	mlib_d64 s_0, s_1, result, minus_mask_1, minus_mask_2;
	mlib_d64 s_dd0, s_dd1, s_dd2, s_dd3, s_dd4, s_dd5;
	mlib_s32 i, j, n;
	mlib_s32 emask, no_emask;

	mlib_u8 *alpha;
	mlib_d64 alpha_1, alpha_2, alphas, e_mask;
	mlib_d64 dst_hi, dst_lo, dst_h, dst_l;

	e_mask = vis_to_double_dup(0x00FF00FF);
	minus_mask_1 = vis_to_double(0x7FFF0000, 0x00000000);
	minus_mask_2 = vis_to_double(0x00000000, 0x7FFF0000);
	vis_write_gsr(56);

	s_line = s_begin = s_data;

	for (j = 0; j < height; j++) {
/* prepare the destination address */
		s_address = (mlib_d64 *)vis_alignaddr(s_line, 0);
		s_end = s_line + (width << 1) - 1;
		emask = vis_edge8(s_line, s_address + 1);
		no_emask = ~emask;

/* 48-pixel column loop */
		n = (mlib_s32)(s_end - s_begin) / 48;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd0 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd1 = vis_faligndata(s_1, s_0);
			s_1 = s_address[3];
			s_dd2 = vis_faligndata(s_0, s_1);
			s_0 = s_address[4];
			s_dd3 = vis_faligndata(s_1, s_0);
			s_1 = s_address[5];
			s_dd4 = vis_faligndata(s_0, s_1);
			s_0 = s_address[6];
			s_dd5 = vis_faligndata(s_1, s_0);
			s_begin += 48;

			MACR_DIV_OFF_U8(s_dd0, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd1, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd2, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd3, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd4, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);

			MACR_DIV_OFF_U8(s_dd5, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address++;
			vis_pst_8(result, s_address, no_emask);
		}

/* 8-pixel column loop  */
		n = (mlib_s32)(s_end - s_begin + 1) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
/* load 8 bytes of source data */
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd0 = vis_faligndata(s_0, s_1);
			s_begin += 8;

			MACR_DIV_OFF_U8(s_dd0, result, mlib_v_tab2, 1);
			vis_alignaddr((void *)~(mlib_u32)s_line, 1);
			result = vis_faligndata(result, result);
			vis_pst_8(result, s_address, emask);
			s_address += 1;
			if (emask != 0xff)
				vis_pst_8(result, s_address, no_emask);
		}

/* last 8-pixel column */
		n = s_end - s_begin;

		if (n >= 0) {
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd0 = vis_faligndata(s_0, s_1);
			vis_write_gsr(56);
			MACR_DIV_OFF_U8(s_dd0, result, mlib_v_tab2, 1);
			help = (mlib_u8 *)&result;
			while (n > 0) {
				*(s_end - 1) = help[n - 1];
				n -= 2;
				s_end -= 2;
			}
		}

		s_line = s_begin = s_line + stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_S16_2(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, d_1;
	mlib_f32 s_1;
	mlib_f32 f_alpha;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 const_2v31 = 0x80000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_f32 const_2v31 = 0x8000;
#endif /* MLIB_USE_FTOI_CLAMPING */

	s_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha =
				    s_1 * (const_2v31 / ((mlib_f32)s_0 +
				    FL_MIN));
				SAT16(d_1, f_alpha);

				s_0 = vis_ld_s16_nf(src + 2);
				s_1 = vis_ld_s16_nf(src + 3);

				src[1] = d_1;

				src += 2;
			}

			src += s_stride;
		}

	} else {
		for (i = 0; i < height; i++) {
			s_1 = src[0];
			s_0 = src[1];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha =
				    s_1 * (const_2v31 / ((mlib_f32)s_0 +
				    FL_MIN));
				SAT16(d_1, f_alpha);

				s_0 = vis_ld_s16_nf(src + 3);
				s_1 = vis_ld_s16_nf(src + 2);

				src[0] = d_1;

				src += 2;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 s_0, d_1;
	mlib_d64 s_1;
	mlib_d64 d_alpha, shift_const = 2147483648.;

	s_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_alpha =
				    shift_const / ((mlib_d64)s_0 + DB_MIN);
				SAT32(d_1, s_1 * d_alpha);

				s_0 = vis_ld_s32_nf(src + 2);
				s_1 = vis_ld_s32_nf(src + 3);

				src[1] = d_1;

				src += 2;
			}

			src += s_stride;
		}

	} else {
		for (i = 0; i < height; i++) {
			s_1 = src[0];
			s_0 = src[1];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_alpha =
				    shift_const / ((mlib_d64)s_0 + DB_MIN);
				SAT32(d_1, s_1 * d_alpha);

				s_0 = vis_ld_s32_nf(src + 3);
				s_1 = vis_ld_s32_nf(src + 2);

				src[0] = d_1;

				src += 2;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 d_0, d_1, d_2;
	mlib_f32 s_0, s_1, s_2;
	mlib_s32 i, j;

	s_stride -= 3 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {

				s_0 = mlib_div_tab_Q8[d_0];
				s_1 = mlib_v_blend_u8_sat[d_1] * s_0;
				s_2 = mlib_v_blend_u8_sat[d_2] * s_0;
				SAT8(d_1, s_1);
				SAT8(d_2, s_2);

				d_0 = vis_ld_u8_nf(src + 3);

				src[1] = d_1;
				src[2] = d_2;

				d_1 = vis_ld_u8_nf(src + 4);
				d_2 = vis_ld_u8_nf(src + 5);

				src += 3;
			}

			src += s_stride;
		}

	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {

				s_1 = mlib_div_tab_Q8[d_1];
				s_0 = mlib_v_blend_u8_sat[d_0] * s_1;
				s_2 = mlib_v_blend_u8_sat[d_2] * s_1;
				SAT8(d_0, s_0);
				SAT8(d_2, s_2);

				src[0] = d_0;
				src[2] = d_2;

				d_0 = vis_ld_u8_nf(src + 3);
				d_1 = vis_ld_u8_nf(src + 4);
				d_2 = vis_ld_u8_nf(src + 5);

				src += 3;
			}

			src += s_stride;
		}

	} else {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {

				s_2 = mlib_div_tab_Q8[d_2];
				s_0 = mlib_v_blend_u8_sat[d_0] * s_2;
				s_1 = mlib_v_blend_u8_sat[d_1] * s_2;
				SAT8(d_0, s_0);
				SAT8(d_1, s_1);

				src[0] = d_0;
				src[1] = d_1;

				d_0 = vis_ld_u8_nf(src + 3);
				d_1 = vis_ld_u8_nf(src + 4);
				d_2 = vis_ld_u8_nf(src + 5);

				src += 3;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_f32 f_alpha;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 const_2v31 = 0x80000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_f32 const_2v31 = 0x8000;
#endif /* MLIB_USE_FTOI_CLAMPING */
	s_stride -= 3 * width;

	if (alpha == 0) {
		mlib_s32 s_0, d_1, d_2;
		mlib_f32 s_1, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha = const_2v31 / ((mlib_f32)s_0 + FL_MIN);
				s_1 *= f_alpha;
				s_2 *= f_alpha;
				SAT16(d_1, s_1);
				SAT16(d_2, s_2);

				s_0 = vis_ld_s16_nf(src + 3);
				s_1 = vis_ld_s16_nf(src + 4);
				s_2 = vis_ld_s16_nf(src + 5);

				src[1] = d_1;
				src[2] = d_2;

				src += 3;
			}

			src += s_stride;
		}

	} else if (alpha == 1) {
		mlib_s32 s_1, d_0, d_2;
		mlib_f32 s_0, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha = const_2v31 / ((mlib_f32)s_1 + FL_MIN);
				s_0 *= f_alpha;
				s_2 *= f_alpha;
				SAT16(d_0, s_0);
				SAT16(d_2, s_2);

				s_0 = vis_ld_s16_nf(src + 3);
				s_1 = vis_ld_s16_nf(src + 4);
				s_2 = vis_ld_s16_nf(src + 5);

				src[0] = d_0;
				src[2] = d_2;

				src += 3;
			}

			src += s_stride;
		}

	} else {
		mlib_s32 s_2, d_0, d_1;
		mlib_f32 s_0, s_1;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha = const_2v31 / ((mlib_f32)s_2 + FL_MIN);
				s_0 *= f_alpha;
				s_1 *= f_alpha;
				SAT16(d_0, s_0);
				SAT16(d_1, s_1);

				s_0 = vis_ld_s16_nf(src + 3);
				s_1 = vis_ld_s16_nf(src + 4);
				s_2 = vis_ld_s16_nf(src + 5);

				src[0] = d_0;
				src[1] = d_1;

				src += 3;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_d64 d_alpha, shift_const = 2147483648.;

	s_stride -= 3 * width;

	if (alpha == 0) {
		mlib_s32 s_0, d_1, d_2;
		mlib_d64 s_1, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_alpha =
				    shift_const / ((mlib_d64)s_0 + DB_MIN);
				SAT32(d_1, s_1 * d_alpha);
				SAT32(d_2, s_2 * d_alpha);

				s_0 = vis_ld_s32_nf(src + 3);
				s_1 = vis_ld_s32_nf(src + 4);
				s_2 = vis_ld_s32_nf(src + 5);

				src[1] = d_1;
				src[2] = d_2;

				src += 3;
			}

			src += s_stride;
		}

	} else if (alpha == 1) {
		mlib_s32 s_1, d_0, d_2;
		mlib_d64 s_0, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_alpha =
				    shift_const / ((mlib_d64)s_1 + DB_MIN);
				SAT32(d_0, s_0 * d_alpha);
				SAT32(d_2, s_2 * d_alpha);

				s_0 = vis_ld_s32_nf(src + 3);
				s_1 = vis_ld_s32_nf(src + 4);
				s_2 = vis_ld_s32_nf(src + 5);

				src[0] = d_0;
				src[2] = d_2;

				src += 3;
			}

			src += s_stride;
		}

	} else {
		mlib_s32 s_2, d_0, d_1;
		mlib_d64 s_0, s_1;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_alpha =
				    shift_const / ((mlib_d64)s_2 + DB_MIN);
				SAT32(d_0, s_0 * d_alpha);
				SAT32(d_1, s_1 * d_alpha);

				s_0 = vis_ld_s32_nf(src + 3);
				s_1 = vis_ld_s32_nf(src + 4);
				s_2 = vis_ld_s32_nf(src + 5);

				src[0] = d_0;
				src[1] = d_1;

				src += 3;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_S16_4(
    mlib_s16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a, d_0, d_1, d_2;
	mlib_f32 f_alpha;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 const_2v31 = 0x80000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_f32 const_2v31 = 0x8000;
#endif /* MLIB_USE_FTOI_CLAMPING */
	stride -= 4 * width;

	if (alpha == 0) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[0];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);

				SAT16(d_0, ((mlib_f32)src[1] * f_alpha));
				SAT16(d_1, ((mlib_f32)src[2] * f_alpha));
				SAT16(d_2, ((mlib_f32)src[3] * f_alpha));

				src[0] = a;
				src[1] = d_0;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else if (alpha == 1) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[1];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);

				SAT16(d_0, ((mlib_f32)src[0] * f_alpha));
				SAT16(d_1, ((mlib_f32)src[2] * f_alpha));
				SAT16(d_2, ((mlib_f32)src[3] * f_alpha));

				src[0] = d_0;
				src[1] = a;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else if (alpha == 2) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[2];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);

				SAT16(d_0, ((mlib_f32)src[0] * f_alpha));
				SAT16(d_1, ((mlib_f32)src[1] * f_alpha));
				SAT16(d_2, ((mlib_f32)src[3] * f_alpha));

				src[0] = d_0;
				src[1] = d_1;
				src[2] = a;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[3];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);

				SAT16(d_0, ((mlib_f32)src[0] * f_alpha));
				SAT16(d_1, ((mlib_f32)src[1] * f_alpha));
				SAT16(d_2, ((mlib_f32)src[2] * f_alpha));

				src[0] = d_0;
				src[1] = d_1;
				src[2] = d_2;
				src[3] = a;
				src += 4;
			}

			src += stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_S32_4(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a, d_0, d_1, d_2;
	mlib_d64 d_alpha, shift_const = 2147483648.;

	stride -= 4 * width;

	if (alpha == 0) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[0];
				d_alpha = shift_const / ((mlib_d64)a + DB_MIN);
				SAT32(d_0, src[1] * d_alpha);
				SAT32(d_1, src[2] * d_alpha);
				SAT32(d_2, src[3] * d_alpha);
				src[0] = a;
				src[1] = d_0;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else if (alpha == 1) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[1];
				d_alpha = shift_const / ((mlib_d64)a + DB_MIN);
				SAT32(d_0, src[0] * d_alpha);
				SAT32(d_1, src[2] * d_alpha);
				SAT32(d_2, src[3] * d_alpha);
				src[0] = d_0;
				src[1] = a;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else if (alpha == 2) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[2];
				d_alpha = shift_const / ((mlib_d64)a + DB_MIN);
				SAT32(d_0, src[0] * d_alpha);
				SAT32(d_1, src[1] * d_alpha);
				SAT32(d_2, src[3] * d_alpha);
				src[0] = d_0;
				src[1] = d_1;
				src[2] = a;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[3];
				d_alpha = shift_const / ((mlib_d64)a + DB_MIN);
				SAT32(d_0, src[0] * d_alpha);
				SAT32(d_1, src[1] * d_alpha);
				SAT32(d_2, src[2] * d_alpha);
				src[0] = d_0;
				src[1] = d_1;
				src[2] = d_2;
				src[3] = a;
				src += 4;
			}

			src += stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U16_4(
    mlib_u16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a;
	mlib_u16 d_0, d_1, d_2;
	mlib_f32 f_alpha;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 const_2v31 = 0x100000000, sat = 0x80000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_f32 const_2v31 = 0x10000;
#endif /* MLIB_USE_FTOI_CLAMPING */

	stride -= 4 * width;

	if (alpha == 0) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[0];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);
				SAT_U16(d_0, ((mlib_f32)src[1] * f_alpha));
				SAT_U16(d_1, ((mlib_f32)src[2] * f_alpha));
				SAT_U16(d_2, ((mlib_f32)src[3] * f_alpha));
				src[0] = a;
				src[1] = d_0;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else if (alpha == 1) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[1];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);
				SAT_U16(d_0, ((mlib_f32)src[0] * f_alpha));
				SAT_U16(d_1, ((mlib_f32)src[2] * f_alpha));
				SAT_U16(d_2, ((mlib_f32)src[3] * f_alpha));
				src[0] = d_0;
				src[1] = a;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else if (alpha == 2) {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[2];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);
				SAT_U16(d_0, ((mlib_f32)src[0] * f_alpha));
				SAT_U16(d_1, ((mlib_f32)src[1] * f_alpha));
				SAT_U16(d_2, ((mlib_f32)src[3] * f_alpha));
				src[0] = d_0;
				src[1] = d_1;
				src[2] = a;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				a = src[3];
				f_alpha = const_2v31 / ((mlib_f32)a + FL_MIN);
				SAT_U16(d_0, ((mlib_f32)src[0] * f_alpha));
				SAT_U16(d_1, ((mlib_f32)src[1] * f_alpha));
				SAT_U16(d_2, ((mlib_f32)src[2] * f_alpha));

				src[0] = d_0;
				src[1] = d_1;
				src[2] = d_2;
				src[3] = a;
				src += 4;
			}

			src += stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_f32 f_alpha;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 const_2v31 = 0x100000000, sat = 0x80000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_f32 const_2v31 = 0x10000;
#endif /* MLIB_USE_FTOI_CLAMPING */

	s_stride -= 3 * width;

	if (alpha == 0) {
		mlib_u16 s_0, d_1, d_2;
		mlib_f32 s_1, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha = const_2v31 / ((mlib_f32)s_0 + FL_MIN);
				s_1 *= f_alpha;
				s_2 *= f_alpha;
				SAT_U16(d_1, s_1);
				SAT_U16(d_2, s_2);

				s_0 = vis_ld_u16_nf(src + 3);
				s_1 = vis_ld_u16_nf(src + 4);
				s_2 = vis_ld_u16_nf(src + 5);

				src[1] = d_1;
				src[2] = d_2;

				src += 3;
			}

			src += s_stride;
		}

	} else if (alpha == 1) {
		mlib_u16 s_1, d_0, d_2;
		mlib_f32 s_0, s_2;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha = const_2v31 / ((mlib_f32)s_1 + FL_MIN);
				s_0 *= f_alpha;
				s_2 *= f_alpha;
				SAT_U16(d_0, s_0);
				SAT_U16(d_2, s_2);

				s_0 = vis_ld_u16_nf(src + 3);
				s_1 = vis_ld_u16_nf(src + 4);
				s_2 = vis_ld_u16_nf(src + 5);

				src[0] = d_0;
				src[2] = d_2;

				src += 3;
			}

			src += s_stride;
		}

	} else {
		mlib_u16 s_2, d_0, d_1;
		mlib_f32 s_0, s_1;

		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha = const_2v31 / ((mlib_f32)s_2 + FL_MIN);
				s_0 *= f_alpha;
				s_1 *= f_alpha;
				SAT_U16(d_0, s_0);
				SAT_U16(d_1, s_1);

				s_0 = vis_ld_u16_nf(src + 3);
				s_1 = vis_ld_u16_nf(src + 4);
				s_2 = vis_ld_u16_nf(src + 5);

				src[0] = d_0;
				src[1] = d_1;

				src += 3;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageDivAlpha_Inp_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_u16 s_0, d_1;
	mlib_f32 s_1;
	mlib_f32 f_alpha;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 const_2v31 = 0x100000000, sat = 0x80000000;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_f32 const_2v31 = 0x10000;
#endif /* MLIB_USE_FTOI_CLAMPING */

	s_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha =
				    s_1 * (const_2v31 / ((mlib_f32)s_0 +
				    FL_MIN));
				SAT_U16(d_1, f_alpha);

				s_0 = vis_ld_u16_nf(src + 2);
				s_1 = vis_ld_u16_nf(src + 3);

				src[1] = d_1;

				src += 2;
			}

			src += s_stride;
		}

	} else {
		for (i = 0; i < height; i++) {
			s_1 = src[0];
			s_0 = src[1];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				f_alpha =
				    s_1 * (const_2v31 / ((mlib_f32)s_0 +
				    FL_MIN));
				SAT_U16(d_1, f_alpha);

				s_0 = vis_ld_u16_nf(src + 3);
				s_1 = vis_ld_u16_nf(src + 2);

				src[0] = d_1;

				src += 2;
			}

			src += s_stride;
		}
	}
}

/* *********************************************************** */
