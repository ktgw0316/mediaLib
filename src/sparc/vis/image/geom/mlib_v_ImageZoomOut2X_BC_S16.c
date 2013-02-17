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

#pragma ident	"@(#)mlib_v_ImageZoomOut2X_BC_S16.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *  mlib_ImageZoomOut2X_S16_x_Bicubic - image 0.5X scaling,
 *                                      image channel(s) x = 1, 2,3, 4,
 *                                      filter is MLIB_BICUBIC/MLIB_BICUBIC2.
 * SYNOPSIS
 *  mlib_status mlib_ImageZoomOut2X_S16_x_Bicubic(const mlib_s16 *p_src,
 *                                                mlib_s16       *p_dst,
 *                                                mlib_s32       src_stride,
 *                                                mlib_s32       dst_stride,
 *                                                mlib_s32       dst_height,
 *                                                mlib_s32       dst_width,
 *                                                mlib_s32       dh,
 *                                                mlib_s32       dw)
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

#undef NEAREST_PARAMETERS

/* *********************************************************** */

#define	NEAREST_PARAMETERS(type)                                \
	type	*p_src,                                         \
	type	*p_dst,                                         \
	mlib_s32 src_stride,                                    \
	mlib_s32 dst_stride,                                    \
	mlib_s32 dst_height,                                    \
	mlib_s32 dst_width,                                     \
	const mlib_u32 *coef

/* *********************************************************** */

#define	NEAREST_PARAMETERS1(type)                               \
	type	*s_data,                                        \
	type	*d_data,                                        \
	mlib_s32 s_stride,                                      \
	mlib_s32 d_stride,                                      \
	mlib_s32 height,                                        \
	mlib_s32 d_width,                                       \
	const mlib_u32 *coef

/* *********************************************************** */

static void mlib_ImageZoomOut2X_S16_Y(
    mlib_s32 ch,
    NEAREST_PARAMETERS(mlib_s16));
static void mlib_ImageZoomOut2X_S16_1_All(
    BILINEAR_PARAMETERS(mlib_s16),
    const mlib_u32 *coef);
static void mlib_ImageZoomOut2X_S16_2_X(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_2_XY(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_2_Align_XY(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_3_XY(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_3_X(
    NEAREST_PARAMETERS(mlib_s16));
static void mlib_ImageZoomOut2X_S16_4_X(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_4_Align_X(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_4_XY(
    NEAREST_PARAMETERS1(mlib_s16));
static void mlib_ImageZoomOut2X_S16_4_Align_XY(
    NEAREST_PARAMETERS1(mlib_s16));

/* *********************************************************** */

#define	PARAM_BC                                                \
	(void *)p_src, (void *)p_dst, src_stride, dst_stride,   \
	dst_height, dst_width, mlib_coef_bc

/* *********************************************************** */

#define	PARAM_BC2                                               \
	(void *)p_src, (void *)p_dst, src_stride, dst_stride,   \
	dst_height, dst_width, mlib_coef_bc2

/* *********************************************************** */

static const mlib_u32 mlib_coef_bc[] = {
	0x00010001,
	0x00090009,
	0x08080808,
	0x48484848,
	4,
};

static const mlib_u32 mlib_coef_bc2[] = {
	0x00020002,
	0x000A000A,
	0x10101010,
	0x50505050,
	3,
};

/* *********************************************************** */

#define	DEF_COEF()                                              \
	mlib_s32 BC_SCL = coef[4];                              \
	mlib_s32 BC_SHF = BC_SCL - 1;                           \
	mlib_u32 mask

/* *********************************************************** */

#define	BC_CF(x)	(((x) << BC_SHF) + (x))
#define	BC_SUM(a, b, c, d)	(BC_CF((mlib_s32)(b) + (c)) - \
					((mlib_s32)(a) + (d)))

#define	BC_SUM1(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SCL)

#define	BC_SUM2(a, b, c, d)	(BC_SUM(a, b, c, d) >> BC_SC2)

/* *********************************************************** */

#define	XOR(x)	(x)
#define	XORS(x)	(x)

#define	MUL_8x16(x, y)	vis_fmul8x16(x, y)

/* *********************************************************** */

#define	MERGE(a, b)	(((a) & 0xFFFF0000) | ((b) & 0x0000FFFF))

/* *********************************************************** */

#define	DMF_0_1_0_1	f_mask_0_1_0_1 = vis_to_float(coef[0])
#define	DMF_0_1_0_9	f_mask_0_1_0_9 = vis_to_float(MERGE(coef[0], coef[1]))

#define	DMF_0_9_0_1	f_mask_0_9_0_1 = vis_to_float(MERGE(coef[1], coef[0]))

#define	DMF_0_9_0_9	f_mask_0_9_0_9 = vis_to_float(coef[1])

/* *********************************************************** */

#define	DMF_8_8_8_8	f_mask_8_8_8_8 = vis_to_float(coef[2])
#define	DMF_8_8_72_72	f_mask_8_8_72_72 = vis_to_float(MERGE(coef[2], coef[3]))

#define	DMF_72_72_8_8	f_mask_72_72_8_8 = vis_to_float(MERGE(coef[3], coef[2]))

#define	DMF_72_72_72_72	f_mask_72_72_72_72 = vis_to_float(coef[3])

#define	DMF_0_4_0_4	f_mask_0_4_0_4 = vis_to_float(0x00040004)

/* *********************************************************** */

#define	MASK_DEFINITION_S16_2X                                  \
	mlib_f32 DMF_8_8_72_72,                                 \
		DMF_72_72_8_8,                                  \
		DMF_0_4_0_4

/* *********************************************************** */

#define	MASK_DEFINITION_S16_2XY                                           \
	mlib_f32 DMF_8_8_8_8,                                   \
		DMF_72_72_72_72,                                \
		DMF_0_9_0_9,                                    \
		DMF_0_1_0_1

/* *********************************************************** */

#define	MASK_DEFINITION_S16_3XY                                            \
	mlib_f32 DMF_8_8_8_8,                                   \
		DMF_72_72_72_72,                                \
		DMF_0_9_0_9,                                    \
		DMF_0_1_0_1,                                    \
		DMF_0_9_0_1,                                    \
		DMF_0_1_0_9

/* *********************************************************** */

#define	MASK_DEFINITION_S16_4	                                \
	mlib_f32 DMF_8_8_8_8,                                   \
		DMF_72_72_72_72


/* *********************************************************** */

#define	CHAN	2
#define	CHAN_2	4

#define	CHAN_4	8

/* *********************************************************** */

/*	s16 bicubic2 1x4 part	   -1				*/
/*				x   5    / 8			*/
/*				    5				*/
/*				   -1				*/

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_Y(
    mlib_s32 ch,
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	DEF_COEF();

	for (i = 0; i < (dst_height & ~1); i += 2) {
		mlib_s16 *ps0 = p_src + (2 * i - 1) * src_stride;
		mlib_s16 *ps1 = ps0 + src_stride;
		mlib_s16 *ps2 = ps1 + src_stride;
		mlib_s16 *ps3 = ps2 + src_stride;
		mlib_s16 *ps4 = ps3 + src_stride;
		mlib_s16 *ps5 = ps4 + src_stride;
		mlib_s16 *pd0 = p_dst + i * dst_stride;
		mlib_s16 *pd1 = pd0 + dst_stride;
		mlib_s32 j, j2, j4, l;

#pragma pipeloop(0)
		for (l = 0; l < ch; l++) {
			for (j = 0, j2 = 0, j4 = 0; j < dst_width;
			    j++, j2 += ch, j4 += ch * 2) {
				mlib_s32 res0, res1;

				res0 =
				    (BC_CF((mlib_s32)ps1[j4 + l] + ps2[j4 +
				    l]) - ps0[j4 + l] - ps3[j4 + l]) >> BC_SCL;
				pd0[j2 + l] = SAT_S16(res0);
				res1 =
				    (BC_CF((mlib_s32)ps3[j4 + l] + ps4[j4 +
				    l]) - ps2[j4 + l] - ps5[j4 + l]) >> BC_SCL;
				pd1[j2 + l] = SAT_S16(res1);
			}
		}
	}

	if (dst_height & 1) {
		mlib_s16 *ps0 = p_src + (2 * i - 1) * src_stride;
		mlib_s16 *ps1 = ps0 + src_stride;
		mlib_s16 *ps2 = ps1 + src_stride;
		mlib_s16 *ps3 = ps2 + src_stride;
		mlib_s16 *pd0 = p_dst + i * dst_stride;
		mlib_s32 j, j2, j4, l;

#pragma pipeloop(0)
		for (l = 0; l < ch; l++) {
			for (j = 0, j2 = 0, j4 = 0; j < dst_width;
			    j++, j2 += ch, j4 += ch * 2) {
				mlib_s32 res0;

				res0 =
				    (BC_CF((mlib_s32)ps1[j4 + l] + ps2[j4 +
				    l]) - ps0[j4 + l] - ps3[j4 + l]) >> BC_SCL;
				pd0[j2 + l] = SAT_S16(res0);
			}
		}
	}
}

/* *********************************************************** */

#define	CHAN1	1
#define	CHAN2	(2*CHAN1)

void
mlib_ImageZoomOut2X_S16_1_All(
    BILINEAR_PARAMETERS(mlib_s16),
    const mlib_u32 *coef)
{
	mlib_s32 width = dst_width;
	mlib_s32 height = dst_height;
	mlib_s32 slb = src_stride;
	mlib_s32 dlb = dst_stride;
	mlib_s16 *sp, *dp;
	mlib_s32 a0, a1, a2, a3, res;

	DEF_COEF();
	mlib_s32 BC_SC2 = 2 * BC_SCL;
	mlib_s32 i, j;

	sp = p_src;
	dp = p_dst;

	if (dh) {
/* averaging along X axis */
		for (i = 0; i < height; i++) {
			a2 = sp[-CHAN1];
			a3 = sp[0];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[CHAN2 * j + CHAN1];
				a3 = sp[CHAN2 * j + CHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				STORE_S16(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along Y axis */

	if (dw) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a0 = sp[CHAN2 * j - slb];
				a1 = sp[CHAN2 * j];
				a2 = sp[CHAN2 * j + slb];
				a3 = sp[CHAN2 * j + 2 * slb];
				res = BC_SUM1(a0, a1, a2, a3);
				STORE_S16(dp[j], res);
			}

			sp += 2 * slb;
			dp += dlb;
		}

		return;
	}

/* averaging along both axes */
	for (i = 0; i < height; i++) {
		a2 = BC_SUM(sp[-slb - CHAN1], sp[-CHAN1], sp[slb - CHAN1],
		    sp[2 * slb - CHAN1]);
		a3 = BC_SUM(sp[-slb], sp[0], sp[slb], sp[2 * slb]);
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			a0 = a2;
			a1 = a3;
			a2 = BC_SUM(sp[CHAN2 * j + CHAN1 - slb],
			    sp[CHAN2 * j + CHAN1], sp[CHAN2 * j + CHAN1 + slb],
			    sp[CHAN2 * j + CHAN1 + 2 * slb]);
			a3 = BC_SUM(sp[CHAN2 * j + CHAN2 - slb],
			    sp[CHAN2 * j + CHAN2], sp[CHAN2 * j + CHAN2 + slb],
			    sp[CHAN2 * j + CHAN2 + 2 * slb]);
			res = BC_SUM2(a0, a1, a2, a3);
			STORE_S16(dp[j], res);
		}

		sp += 2 * slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_1_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16))
{
	mlib_ImageZoomOut2X_S16_1_All(BILINEAR_P, mlib_coef_bc);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_1_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16))
{
	mlib_ImageZoomOut2X_S16_1_All(BILINEAR_P, mlib_coef_bc2);
}

/* *********************************************************** */

#define	BICUBIC_S16_2_X_HELP(d_0, d_1, d_dst)                         \
	_d_s16____1w = MUL_8x16(f_mask_8_8_72_72, d_0);               \
	_d_s16____2w = MUL_8x16(f_mask_72_72_8_8, d_1);               \
	_d_s16____1w = vis_fpsub16(_d_s16____2w, _d_s16____1w);       \
	_d_s16____2w =                                                \
	vis_fmuld8ulx16(f_mask_0_4_0_4, vis_read_hi(_d_s16____1w));   \
	_d_s16____1w =                                                \
	vis_fmuld8ulx16(f_mask_0_4_0_4, vis_read_lo(_d_s16____1w));   \
	d_dst = vis_fpsub32(_d_s16____2w, _d_s16____1w)

/* *********************************************************** */

#define	BICUBIC_S16_2_DOUBLE_X(d_0, d_1, d_2, d_dst)            \
	BICUBIC_S16_2_X_HELP(d_0, d_1, _d_dst_help);            \
	BICUBIC_S16_2_X_HELP(d_1, d_2, _d_s16____2w);           \
	d_dst = vis_fpackfix_pair(_d_dst_help, _d_s16____2w);   \
	d_dst = XOR(d_dst)

/* *********************************************************** */

#define	BICUBIC_S16_2_XY_HELP(d_0, d_1, d_2, d_3, d_dst)        \
	d_0 = MUL_8x16(f_mask_8_8_8_8, d_0);                    \
	d_1 = MUL_8x16(f_mask_72_72_72_72, d_1);                \
	d_2 = MUL_8x16(f_mask_72_72_72_72, d_2);                \
	d_3 = MUL_8x16(f_mask_8_8_8_8, d_3);                    \
	_d_s16____1w = vis_fpadd16(d_0, d_3);                   \
	_d_s16____2w = vis_fpadd16(d_1, d_2);                   \
	d_dst = vis_fpsub16(_d_s16____2w, _d_s16____1w)

/* *********************************************************** */

#define	BICUBIC_S16_2_XY_COUNT(d_src_1, d_src_2, d_dst)          \
	_d_dst_help =                                            \
	vis_fmuld8ulx16(f_mask_0_9_0_9, vis_read_lo(d_src_1));   \
	_d_s16____1w =                                           \
	vis_fmuld8ulx16(f_mask_0_1_0_1, vis_read_hi(d_src_1));   \
	_d_s16____1w = vis_fpsub32(_d_dst_help, _d_s16____1w);   \
	_d_s16____2w =                                           \
	vis_fmuld8ulx16(f_mask_0_9_0_9, vis_read_hi(d_src_2));   \
	_d_dst_help =                                            \
	vis_fmuld8ulx16(f_mask_0_1_0_1, vis_read_lo(d_src_2));   \
	_d_s16____2w = vis_fpsub32(_d_s16____2w, _d_dst_help);   \
	d_dst = vis_fpadd32(_d_s16____1w, _d_s16____2w)

/* *********************************************************** */

#define	BICUBIC_S16_2_XY_PREV(_d_0, _d_1, _d_2, _d_3, d_prev)   \
	BICUBIC_S16_2_XY_HELP(_d_0, _d_1, _d_2, _d_3, d_prev)

/* *********************************************************** */

#define	BICUBIC_S16_2_DOUBLE_XY(_d_0, _d_1, _d_2, _d_3, _d_4,      \
	_d_5, _d_6, _d_7, d_prev, d_dst)                           \
	BICUBIC_S16_2_XY_HELP(_d_0, _d_1, _d_2, _d_3, _d_s16_0);   \
	BICUBIC_S16_2_XY_HELP(_d_4, _d_5, _d_6, _d_7, _d_s16_1);   \
	BICUBIC_S16_2_XY_COUNT(d_prev, _d_s16_0, d_prev);          \
	BICUBIC_S16_2_XY_COUNT(_d_s16_0, _d_s16_1, _d_s16_0);      \
	d_dst = vis_fpackfix_pair(d_prev, _d_s16_0);               \
	d_dst = XOR(d_dst);                                        \
	d_prev = _d_s16_1

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_X(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 _d_s16____1w, _d_s16____2w, _d_dst_help;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_0, s_1, result;
	mlib_d64 s_dd0, s_dd1, s_dd2;

	MASK_DEFINITION_S16_2X;

	mlib_f32 f_result;
	mlib_s32 i, j, s_stride_2 = s_stride << 1, help_d_width = d_width - 2;
	mlib_s16 *s_begin;
	mlib_s16 *s_line, *d_line, *d_end, *help;
	mlib_s16 emask, no_emask;
	void *u_align;

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		d_address = (mlib_d64 *)vis_alignaddr((void *)d_line, 0);

		emask = vis_edge8((void *)d_line, (void *)(d_address + 1));
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 2) {
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-CHAN_2);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd0 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd1 = vis_faligndata(s_1, s_0);
				s_1 = s_address[3];
				s_dd2 = vis_faligndata(s_0, s_1);

				s_begin += CHAN_4;

				BICUBIC_S16_2_DOUBLE_X(s_dd0, s_dd1, s_dd2,
					result);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address += 1;
			}
		} else {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 2) {
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-CHAN_2);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd0 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd1 = vis_faligndata(s_1, s_0);
				s_1 = s_address[3];
				s_dd2 = vis_faligndata(s_0, s_1);

				s_begin += CHAN_4;

				BICUBIC_S16_2_DOUBLE_X(s_dd0, s_dd1, s_dd2,
					result);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address += 1;
				vis_pst_8(result, d_address, no_emask);
			}
		}

		if (i < d_width) {
			d_end = d_line + (d_width << 1) - 2;
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, -CHAN_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd0 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd1 = vis_faligndata(s_1, s_0);
			BICUBIC_S16_2_X_HELP(s_dd0, s_dd1, result);
			f_result = XORS(vis_fpackfix(result));
			help = (mlib_s16 *)&f_result;
			d_end[0] = help[0];
			d_end[1] = help[1];
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_XY(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 d_prev;
	mlib_d64 _d_s16____1w, _d_s16____2w, _d_dst_help;
	mlib_d64 _d_s16_0, _d_s16_1;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_0, s_1, result;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;

	MASK_DEFINITION_S16_2XY;
	mlib_f32 f_result;
	mlib_s32 i, j,
	    s_stride_2 = s_stride << 1, s_stride_4 = s_stride << 2,
	    help_d_width = d_width - 2;
	mlib_s16 *s_begin;
	mlib_s16 *s_line, *d_line, *d_end;
	mlib_s16 emask, no_emask;
	void *u_align;

/* initialize GSR scale factor */
	vis_write_gsr(13 << 3);

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		d_address = (mlib_d64 *)vis_alignaddr((void *)d_line, 0);

		emask = vis_edge8((void *)d_line, (void *)(d_address + 1));
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		s_begin -= CHAN;

		s_address = (mlib_d64 *)vis_alignaddr(s_begin, -s_stride_2);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd00 = vis_faligndata(s_0, s_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd10 = vis_faligndata(s_0, s_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd20 = vis_faligndata(s_0, s_1);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_4);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd30 = vis_faligndata(s_0, s_1);

		BICUBIC_S16_2_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30, d_prev);
		s_begin += CHAN_2;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 2) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride_2);
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
					s_stride_2);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_4);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_0, s_1);
				s_0 = vis_ld_d64_nf(s_address + 2);
				s_dd31 = vis_faligndata(s_1, s_0);

				s_begin += CHAN_4;
				BICUBIC_S16_2_DOUBLE_XY(s_dd00, s_dd10, s_dd20,
					s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
					d_prev, result);
				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address += 1;
			}
		} else {
#pragma pipeloop(0)
			for (; i <= help_d_width; i += 2) {

				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					-s_stride_2);
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
					s_stride_2);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd20 = vis_faligndata(s_0, s_1);
				s_0 = s_address[2];
				s_dd21 = vis_faligndata(s_1, s_0);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
					s_stride_4);
				s_0 = s_address[0];
				s_1 = s_address[1];
				s_dd30 = vis_faligndata(s_0, s_1);
				s_0 = vis_ld_d64_nf(s_address + 2);
				s_dd31 = vis_faligndata(s_1, s_0);

				s_begin += CHAN_4;
				BICUBIC_S16_2_DOUBLE_XY(s_dd00, s_dd10, s_dd20,
					s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
					d_prev, result);
				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address += 1;
				vis_pst_8(result, d_address, no_emask);
			}
		}

		if (i < d_width) {
			d_end = &d_line[(d_width << 1) - 2];
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd00 = vis_faligndata(s_0, s_1);
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd10 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_4);
			s_0 = vis_ld_d64_nf(s_address);
			s_1 = vis_ld_d64_nf(s_address + 1);
			s_dd30 = vis_faligndata(s_0, s_1);
			BICUBIC_S16_2_XY_HELP(s_dd00, s_dd10, s_dd20, s_dd30,
			    result);
			BICUBIC_S16_2_XY_COUNT(d_prev, result, d_prev);
			f_result = XORS(vis_fpackfix(d_prev));
			s_begin = (mlib_s16 *)(&f_result);
			d_end[0] = s_begin[0];
			d_end[1] = s_begin[1];
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_Align_XY(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 _d_s16____1w, _d_s16____2w, _d_dst_help;
	mlib_d64 _d_s16_0, _d_s16_1;
	mlib_d64 result;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;
	mlib_d64 d_prev;

	MASK_DEFINITION_S16_2XY;

	mlib_s32 i, j, help_d_width = d_width - 2;
	mlib_s32 s_stride_d_1 = s_stride >> 2,
	    s_stride_d_2 = s_stride >> 1, s_stride_2 = s_stride << 1;
	mlib_s16 *s_line, *d_line;

/* initialize GSR scale factor */
	vis_write_gsr(13 << 3);

	s_line = s_data - CHAN;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		d_address = (mlib_d64 *)d_line;
		s_address = (mlib_d64 *)s_line;

		s_dd00 = s_address[-s_stride_d_1];
		s_dd10 = s_address[0];
		s_dd20 = s_address[s_stride_d_1];
		s_dd30 = s_address[s_stride_d_2];

		BICUBIC_S16_2_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30, d_prev);
		s_address += 1;
#pragma pipeloop(0)
		for (; i <= help_d_width; i += 2) {

			s_dd00 = s_address[-s_stride_d_1];
			s_dd01 = s_address[-s_stride_d_1 + 1];
			s_dd10 = s_address[0];
			s_dd11 = s_address[1];
			s_dd20 = s_address[s_stride_d_1];
			s_dd21 = s_address[s_stride_d_1 + 1];
			s_dd30 = s_address[s_stride_d_2];
			s_dd31 = s_address[s_stride_d_2 + 1];

			BICUBIC_S16_2_DOUBLE_XY(s_dd00, s_dd10, s_dd20, s_dd30,
			    s_dd01, s_dd11, s_dd21, s_dd31, d_prev, result);
			d_address[0] = result;
			d_address++;
			s_address += 2;
		}

		if (i < d_width) {
			s_dd00 = s_address[-s_stride_d_1];
			s_dd10 = s_address[0];
			s_dd20 = s_address[s_stride_d_1];
			s_dd30 = s_address[s_stride_d_2];
			BICUBIC_S16_2_XY_HELP(s_dd00, s_dd10, s_dd20, s_dd30,
			    result);
			BICUBIC_S16_2_XY_COUNT(d_prev, result, d_prev);
			((mlib_f32 *)d_address)[0] = XORS(vis_fpackfix(d_prev));
		}

		s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16))
{
	vis_write_gsr(15 << 3);

	if (dh) {
		mlib_ImageZoomOut2X_S16_2_X(PARAM_BC);
		return;
	}

	if (dw) {
		mlib_ImageZoomOut2X_S16_Y(2, PARAM_BC);
		return;
	}

	if (((((mlib_s32)p_dst) | dst_stride | ((mlib_s32)(p_src -
	    CHAN)) | src_stride) & 7) == 0) {
		mlib_ImageZoomOut2X_S16_2_Align_XY(PARAM_BC);
		return;
	}

	mlib_ImageZoomOut2X_S16_2_XY(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16))
{
	vis_write_gsr(15 << 3);

	if (dh) {
		mlib_ImageZoomOut2X_S16_2_X(PARAM_BC2);
		return;
	}

	if (dw) {
		mlib_ImageZoomOut2X_S16_Y(2, PARAM_BC2);
		return;
	}

	if (((((mlib_s32)p_dst) | dst_stride | ((mlib_s32)(p_src -
	    CHAN)) | src_stride) & 7) == 0) {
		mlib_ImageZoomOut2X_S16_2_Align_XY(PARAM_BC2);
		return;
	}

	mlib_ImageZoomOut2X_S16_2_XY(PARAM_BC2);
}

/* *********************************************************** */

#undef CHAN
#undef CHAN_2
#undef CHAN_4

/* *********************************************************** */

#define	CHAN	3
#define	CHAN_2	6

#define	CHAN_3	9

#define	CHAN_4	12

/* *********************************************************** */

#define	BICUBIC_S16_3_XY_HELP(d_0, d_1, d_2, d_3, d_dst)        \
	d_0 = MUL_8x16(f_mask_8_8_8_8, d_0);                    \
	d_1 = MUL_8x16(f_mask_72_72_72_72, d_1);                \
	d_2 = MUL_8x16(f_mask_72_72_72_72, d_2);                \
	d_3 = MUL_8x16(f_mask_8_8_8_8, d_3);                    \
	d_dst = vis_fpadd16(d_0, d_3);                          \
	d_dst = vis_fpsub16(d_1, d_dst);                        \
	d_dst = vis_fpadd16(d_dst, d_2)

/* *********************************************************** */

#define	BICUBIC_S16_3_XY_COUNT(f_0, f_1, f_2, f_3, f_4, f_5,    \
	d_dst_0, d_dst_1, d_dst_2)                              \
	d_dst_0 = vis_fmuld8ulx16(f_mask_0_9_0_9, f_3);         \
	_d_dst_help = vis_fmuld8ulx16(f_mask_0_1_0_1, f_0);     \
	d_dst_0 = vis_fpsub32(d_dst_0, _d_dst_help);            \
	d_dst_1 = vis_fmuld8ulx16(f_mask_0_9_0_1, f_4);         \
	_d_s16____1w = vis_fmuld8ulx16(f_mask_0_1_0_9, f_1);    \
	d_dst_1 = vis_fpsub32(d_dst_1, _d_s16____1w);           \
	d_dst_2 = vis_fmuld8ulx16(f_mask_0_1_0_1, f_5);         \
	_d_s16____2w = vis_fmuld8ulx16(f_mask_0_9_0_9, f_2);    \
	d_dst_2 = vis_fpsub32(d_dst_2, _d_s16____2w)

/* *********************************************************** */

#define	BICUBIC_S16_3_XY_PREV(d_0_0, d_0_1, d_0_2, d_0_3,                \
	d_1_0, d_1_1, d_1_2, d_1_3, f_prev, d_prev)                      \
	BICUBIC_S16_3_XY_HELP(d_0_0, d_0_1, d_0_2, d_0_3, d_0_0);        \
	BICUBIC_S16_3_XY_HELP(d_1_0, d_1_1, d_1_2, d_1_3, d_1_0);        \
	f_prev = vis_read_hi(d_0_0);                                     \
	d_prev = vis_freg_pair(vis_read_lo(d_0_0), vis_read_hi(d_1_0))

/* *********************************************************** */

#define	BICUBIC_S16_3_DOUBLE_XY(d_0_0, d_0_1, d_0_2, d_0_3,                \
	d_1_0, d_1_1, d_1_2, d_1_3, d_2_0, d_2_1, d_2_2, d_2_3,            \
	f_prev, d_prev, d_dst, f_dst)                                      \
	BICUBIC_S16_3_XY_HELP(d_0_0, d_0_1, d_0_2, d_0_3, d_0_0);          \
	BICUBIC_S16_3_XY_HELP(d_1_0, d_1_1, d_1_2, d_1_3, d_1_0);          \
	BICUBIC_S16_3_XY_HELP(d_2_0, d_2_1, d_2_2, d_2_3, d_2_0);          \
	BICUBIC_S16_3_XY_COUNT(f_prev, vis_read_hi(d_prev),                \
	    vis_read_lo(d_prev), vis_read_hi(d_0_0), vis_read_lo(d_0_0),   \
	    vis_read_hi(d_1_0), d_0_1, d_1_1, d_2_1);                      \
	f_prev = vis_read_lo(d_1_0);                                       \
	d_prev = d_2_0;                                                    \
	BICUBIC_S16_3_XY_COUNT(vis_read_hi(d_0_0), vis_read_lo(d_0_0),     \
	    vis_read_hi(d_1_0), f_prev, vis_read_hi(d_prev),               \
	    vis_read_lo(d_prev), d_0_2, d_1_2, d_2_2);                     \
	_d_dst_help =                                                      \
	vis_freg_pair(vis_read_lo(d_1_1), vis_read_hi(d_2_1));             \
	d_0_1 = vis_fpsub32(d_0_1, _d_dst_help);                           \
	_d_s16____1w =                                                     \
	vis_freg_pair(vis_read_hi(d_1_1), vis_read_hi(d_0_2));             \
	_d_s16____2w =                                                     \
	vis_freg_pair(vis_read_lo(d_2_1), vis_read_lo(d_1_2));             \
	d_1_1 = vis_fpsub32(_d_s16____1w, _d_s16____2w);                   \
	_d_dst_help =                                                      \
	vis_freg_pair(vis_read_lo(d_0_2), vis_read_hi(d_1_2));             \
	d_2_1 = vis_fpsub32(_d_dst_help, d_2_2);                           \
	d_dst = XOR(vis_fpackfix_pair(d_0_1, d_1_1));                      \
	f_dst = XORS(vis_fpackfix(d_2_1))

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_3_XY(
    NEAREST_PARAMETERS1(mlib_s16))
{
	DEF_COEF();
	mlib_s32 BC_SC2 = 2 * BC_SCL;
	mlib_d64 _d_s16____1w, _d_s16____2w, _d_dst_help;
	mlib_d64 *s_address;
	mlib_d64 s_0, s_1, d_result;
	mlib_d64 d_prev;
	mlib_d64 s_dd00, s_dd01, s_dd02, s_dd10, s_dd11, s_dd12,
	    s_dd20, s_dd21, s_dd22, s_dd30, s_dd31, s_dd32;

	MASK_DEFINITION_S16_3XY;

	mlib_f32 f_result, f_prev, *f_dst_address;
	mlib_s32 i, j, off_dst;
	mlib_s32 s_stride_2 = s_stride << 1, s_stride_4 = s_stride << 2;
	mlib_s16 *s_begin;
	mlib_s16 *s_line, *d_line;

	vis_write_gsr(13 << 3);

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;

		f_dst_address = (mlib_f32 *)(((mlib_addr)d_line) & (~3));
		off_dst = ((mlib_addr)d_line - (mlib_addr)f_dst_address) >> 1;

/*  START OFFSET  */

		if (off_dst > 0) {
			mlib_s32 l, res, beg = 0;

			beg = (off_dst > d_width) ? d_width : off_dst;

			for (l = 0; l < CHAN; l++) {
				mlib_s16 *ps = s_line + l, *pd = d_line + l;
				mlib_s32 k, st0 = BC_CF(ps[-CHAN] +
				    ps[s_stride - CHAN]) -
				    ps[-s_stride - CHAN] -
				    ps[s_stride_2 - CHAN],
				    st1 = BC_CF(ps[0] +
				    ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];

#pragma pipeloop(0)
				for (k = i; k < beg; k++) {
					res = BC_CF(st1) - st0;
					ps += CHAN;
					st0 = BC_CF(ps[0] +
					    ps[s_stride]) -
					    ps[-s_stride] - ps[s_stride_2];
					ps += CHAN;
					st1 = BC_CF(ps[0] +
					    ps[s_stride]) -
					    ps[-s_stride] - ps[s_stride_2];
					res =
					    (res + BC_CF(st0) - st1) >> BC_SC2;
					*pd = SAT_S16(res);
					pd += CHAN;
				}
			}

			f_dst_address = (mlib_f32 *)(d_line + CHAN * beg);
			s_begin += CHAN_2 * beg;
			i = beg;
		}

/*  MAIN COUNT  */

		if ((d_width - i) > 1) {
			s_begin -= CHAN;

			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd00 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd01 = vis_faligndata(s_1, s_0);
			s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd10 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd11 = vis_faligndata(s_1, s_0);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd21 = vis_faligndata(s_1, s_0);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_4);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd30 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd31 = vis_faligndata(s_1, s_0);

			BICUBIC_S16_3_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30,
			    s_dd01, s_dd11, s_dd21, s_dd31, f_prev, d_prev);
			s_begin += CHAN_2;
		}
#pragma pipeloop(0)
		for (; i <= d_width - 2; i += 2) {

			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, -s_stride_2);
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
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd20 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd21 = vis_faligndata(s_1, s_0);
			s_1 = s_address[3];
			s_dd22 = vis_faligndata(s_0, s_1);
			s_address =
			    (mlib_d64 *)vis_alignaddr(s_begin, s_stride_4);
			s_0 = s_address[0];
			s_1 = s_address[1];
			s_dd30 = vis_faligndata(s_0, s_1);
			s_0 = s_address[2];
			s_dd31 = vis_faligndata(s_1, s_0);
			s_1 = vis_ld_d64_nf(s_address + 3);
			s_dd32 = vis_faligndata(s_0, s_1);

			s_begin += CHAN_4;

			BICUBIC_S16_3_DOUBLE_XY(s_dd00, s_dd10, s_dd20, s_dd30,
			    s_dd01, s_dd11, s_dd21, s_dd31,
			    s_dd02, s_dd12, s_dd22, s_dd32,
			    f_prev, d_prev, d_result, f_result);
			f_dst_address[0] = vis_read_hi(d_result);
			f_dst_address[1] = vis_read_lo(d_result);
			f_dst_address[2] = f_result;
			f_dst_address += 3;
		}

		if (i < d_width) {
			mlib_s32 l, res;

			for (l = 0; l < CHAN; l++) {
				mlib_s16 *ps = s_line + l + CHAN_2 * i,
				    *pd = d_line + l + CHAN * i;
				mlib_s32 st0 =
				    BC_CF(ps[-CHAN] + ps[s_stride - CHAN]) -
				    ps[-s_stride - CHAN] - ps[s_stride_2 -
				    CHAN], st1 =
				    BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];

				res = BC_CF(st1) - st0;
				ps += CHAN;
				st0 = BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];
				ps += CHAN;
				st1 = BC_CF(ps[0] + ps[s_stride]) -
				    ps[-s_stride] - ps[s_stride_2];
				res = (res + BC_CF(st0) - st1) >> BC_SC2;
				*pd = SAT_S16(res);
				pd += CHAN;
			}
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

#undef  CHAN1
#define	CHAN1	3

void
mlib_ImageZoomOut2X_S16_3_X(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 width = dst_width;
	mlib_s32 height = dst_height;
	mlib_s32 slb = src_stride;
	mlib_s32 dlb = dst_stride;
	mlib_s16 *sp, *dp;
	mlib_s32 a0, a1, a2, a3, res;
	mlib_s32 i, j, k;

	DEF_COEF();

	for (i = 0; i < height; i++) {
		for (k = 0; k < CHAN1; k++) {
			sp = p_src + 2 * slb * i + k;
			dp = p_dst + dlb * i + k;
			a2 = sp[-CHAN1];
			a3 = sp[0];
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a0 = a2;
				a1 = a3;
				a2 = sp[CHAN2 * j + CHAN1];
				a3 = sp[CHAN2 * j + CHAN2];
				res = BC_SUM1(a0, a1, a2, a3);
				STORE_S16(dp[CHAN1 * j], res);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_3_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16))
{
	if (dh) {
		mlib_ImageZoomOut2X_S16_3_X(PARAM_BC);
		return;
	}

	if (dw) {
		mlib_ImageZoomOut2X_S16_Y(3, PARAM_BC);
		return;
	}

	mlib_ImageZoomOut2X_S16_3_XY(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_3_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16))
{
	if (dh) {
		mlib_ImageZoomOut2X_S16_3_X(PARAM_BC2);
		return;
	}

	if (dw) {
		mlib_ImageZoomOut2X_S16_Y(3, PARAM_BC2);
		return;
	}

	mlib_ImageZoomOut2X_S16_3_XY(PARAM_BC2);
}

/* *********************************************************** */

#undef CHAN
#undef CHAN_2
#undef CHAN_3

/* *********************************************************** */

#define	CHAN	4
#define	CHAN_2	8

#define	CHAN_3	12

/* *********************************************************** */

#define	BICUBIC_S16_4_XY_HELP(d_0, d_1, d_2, d_3, d_dst)        \
	d_0 = MUL_8x16(f_mask_8_8_8_8, d_0);                    \
	d_1 = MUL_8x16(f_mask_72_72_72_72, d_1);                \
	d_2 = MUL_8x16(f_mask_72_72_72_72, d_2);                \
	d_3 = MUL_8x16(f_mask_8_8_8_8, d_3);                    \
	_d_s16____1w = vis_fpadd16(d_0, d_3);                   \
	_d_s16____2w = vis_fpadd16(d_1, d_2);                   \
	d_dst = vis_fpsub16(_d_s16____2w, _d_s16____1w)

/* *********************************************************** */

#define	BICUBIC_S16_4_DOUBLE_X(d_0, d_1, d_2, d_3, d_dst)            \
	BICUBIC_S16_4_XY_HELP(d_0, d_1, d_2, d_3, d_dst);            \
	d_2 = vis_fmuld8ulx16(f_mask_0_4_0_4, vis_read_hi(d_dst));   \
	d_3 = vis_fmuld8ulx16(f_mask_0_4_0_4, vis_read_lo(d_dst));   \
	d_dst = XOR(vis_fpackfix_pair(d_2, d_3))

/* *********************************************************** */

#define	BICUBIC_S16_4_XY_COUNT(d_src_0, d_src_1, d_src_2,            \
	d_src_3, d_dst)                                              \
	_d_s16____0w =                                               \
	vis_fmuld8ulx16(f_mask_0_1_0_1, vis_read_hi(d_src_0));       \
	_d_s16____1w =                                               \
	vis_fmuld8ulx16(f_mask_0_9_0_9, vis_read_hi(d_src_1));       \
	_d_s16____2w =                                               \
	vis_fmuld8ulx16(f_mask_0_9_0_9, vis_read_hi(d_src_2));       \
	_d_s32____hi = vis_fpsub32(_d_s16____1w, _d_s16____0w);      \
	_d_s16____3w =                                               \
	vis_fmuld8ulx16(f_mask_0_1_0_1, vis_read_hi(d_src_3));       \
	_d_s32____hi = vis_fpadd32(_d_s32____hi, _d_s16____2w);      \
	_d_s16____0w =                                               \
	vis_fmuld8ulx16(f_mask_0_1_0_1, vis_read_lo(d_src_0));       \
	_d_s32____hi = vis_fpsub32(_d_s32____hi, _d_s16____3w);      \
	_d_s16____1w =                                               \
	vis_fmuld8ulx16(f_mask_0_9_0_9, vis_read_lo(d_src_1));       \
	_d_s16____2w =                                               \
	vis_fmuld8ulx16(f_mask_0_9_0_9, vis_read_lo(d_src_2));       \
	_d_s32____lo = vis_fpsub32(_d_s16____1w, _d_s16____0w);      \
	_d_s16____3w =                                               \
	vis_fmuld8ulx16(f_mask_0_1_0_1, vis_read_lo(d_src_3));       \
	_d_s32____lo = vis_fpadd32(_d_s32____lo, _d_s16____2w);      \
	_d_s32____lo = vis_fpsub32(_d_s32____lo, _d_s16____3w);      \
	d_dst = XOR(vis_fpackfix_pair(_d_s32____hi, _d_s32____lo))

/* *********************************************************** */

#define	BICUBIC_S16_4_XY_PREV(_d_0_0, _d_0_1, _d_0_2, _d_0_3,              \
	_d_1_0, _d_1_1, _d_1_2, _d_1_3, d_prev_0, d_prev_1)                \
	BICUBIC_S16_4_XY_HELP(_d_0_0, _d_0_1, _d_0_2, _d_0_3, d_prev_0);   \
	BICUBIC_S16_4_XY_HELP(_d_1_0, _d_1_1, _d_1_2, _d_1_3, d_prev_1)

/* *********************************************************** */

#define	BICUBIC_S16_4_DOUBLE_XY(_d_0_0, _d_0_1, _d_0_2,                    \
	_d_0_3, _d_1_0, _d_1_1, _d_1_2, _d_1_3, d_prev_0, d_prev_1,        \
	d_dst)                                                             \
	BICUBIC_S16_4_XY_HELP(_d_0_0, _d_0_1, _d_0_2, _d_0_3, _d_s16_0);   \
	BICUBIC_S16_4_XY_HELP(_d_1_0, _d_1_1, _d_1_2, _d_1_3, _d_s16_1);   \
	BICUBIC_S16_4_XY_COUNT(d_prev_0, d_prev_1, _d_s16_0, _d_s16_1,     \
	    d_dst);                                                        \
	d_prev_0 = _d_s16_0;                                               \
	d_prev_1 = _d_s16_1

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_X(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 _d_s16____1w, _d_s16____2w;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_0, s_1, result;
	mlib_d64 s_dd0, s_dd1, s_dd2, s_dd3, s_dd0_old, s_dd1_old;
	mlib_s32 i, j, s_stride_2 = s_stride << 1;
	mlib_s16 *s_line, *d_line;
	mlib_s16 emask, no_emask;
	void *u_align;

	MASK_DEFINITION_S16_4;
	mlib_f32 DMF_0_4_0_4;

	s_line = s_data - CHAN;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		d_address = (mlib_d64 *)vis_alignaddr((void *)d_line, 0);

		emask = vis_edge8((void *)d_line, (void *)(d_address + 1));
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		s_address = (mlib_d64 *)vis_alignaddr(s_line, 0);
		s_0 = s_address[0];
		s_1 = s_address[1];
		s_dd0_old = vis_faligndata(s_0, s_1);
		s_0 = s_address[2];
		s_dd1_old = vis_faligndata(s_1, s_0);
		s_address += 3;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (; i < d_width; i++) {
				vis_alignaddr(s_line, 0);
				s_dd0 = s_dd0_old;
				s_dd1 = s_dd1_old;
				s_1 = s_address[0];
				s_dd2 = vis_faligndata(s_0, s_1);
				s_0 = s_address[1];
				s_dd3 = vis_faligndata(s_1, s_0);
				s_address += 2;
				s_dd0_old = s_dd2;
				s_dd1_old = s_dd3;
				BICUBIC_S16_4_DOUBLE_X(s_dd0, s_dd1, s_dd2,
					s_dd3, result);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address++;
			}
		} else {
#pragma pipeloop(0)
			for (; i < d_width; i++) {
				vis_alignaddr(s_line, 0);
				s_dd0 = s_dd0_old;
				s_dd1 = s_dd1_old;
				s_1 = s_address[0];
				s_dd2 = vis_faligndata(s_0, s_1);
				s_0 = s_address[1];
				s_dd3 = vis_faligndata(s_1, s_0);
				s_address += 2;
				s_dd0_old = s_dd2;
				s_dd1_old = s_dd3;
				BICUBIC_S16_4_DOUBLE_X(s_dd0, s_dd1, s_dd2,
					s_dd3, result);

				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address++;
				vis_pst_8(result, d_address, no_emask);
			}
		}

		s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Align_X(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 _d_s16____1w, _d_s16____2w;
	mlib_d64 *d_address, *s_address;
	mlib_d64 result;
	mlib_d64 s_dd0, s_dd1, s_dd2, s_dd3, s_dd0_old, s_dd1_old;

	MASK_DEFINITION_S16_4;
	mlib_f32 DMF_0_4_0_4;

	mlib_s16 *s_line, *d_line;
	mlib_s32 i, j, s_stride_2 = s_stride << 1;

	s_line = s_data - CHAN;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		d_address = (mlib_d64 *)d_line;
		s_address = (mlib_d64 *)s_line;
		s_dd0_old = s_address[0];
		s_dd1_old = s_address[1];
		s_address += 2;
#pragma pipeloop(0)
		for (; i < d_width; i++) {
			s_dd0 = s_dd0_old;
			s_dd1 = s_dd1_old;
			s_dd2 = s_address[0];
			s_dd3 = s_address[1];
			s_address += 2;
			s_dd0_old = s_dd2;
			s_dd1_old = s_dd3;

			BICUBIC_S16_4_DOUBLE_X(s_dd0, s_dd1, s_dd2, s_dd3,
			    result);

			d_address[0] = result;
			d_address++;
		}

		s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_XY(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 _d_s16____0w, _d_s16____1w, _d_s16____2w, _d_s16____3w;
	mlib_d64 _d_s32____hi, _d_s32____lo;
	mlib_d64 _d_s16_0, _d_s16_1;
	mlib_d64 d_prev_1, d_prev_2;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;
	mlib_d64 *d_address, *s_address;
	mlib_d64 s_1, result, s_0_old, s_1_old, s_2_old, s_3_old;
	mlib_s32 i, j, s_stride_2 = s_stride << 1, s_stride_4 = s_stride << 2;
	mlib_s16 *s_line, *d_line;
	mlib_s16 *s_begin;
	mlib_s16 emask, no_emask;
	void *u_align;

	MASK_DEFINITION_S16_4;
	mlib_f32 DMF_0_1_0_1;
	mlib_f32 DMF_0_9_0_9;

/* initialize GSR scale factor */
	vis_write_gsr(13 << 3);

	s_line = s_begin = s_data;
	d_line = d_data;

	for (j = 0; j < height; j++) {
		i = 0;
		d_address = (mlib_d64 *)vis_alignaddr((void *)d_line, 0);

		emask = vis_edge8((void *)d_line, (void *)(d_address + 1));
		no_emask = ~emask;
		u_align = (void *)~(mlib_addr)d_line;

		s_begin -= CHAN;

		s_address = (mlib_d64 *)vis_alignaddr(s_begin, -s_stride_2);
		s_0_old = s_address[0];
		s_1 = s_address[1];
		s_dd00 = vis_faligndata(s_0_old, s_1);
		s_0_old = s_address[2];
		s_dd01 = vis_faligndata(s_1, s_0_old);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, 0);
		s_1_old = s_address[0];
		s_1 = s_address[1];
		s_dd10 = vis_faligndata(s_1_old, s_1);
		s_1_old = s_address[2];
		s_dd11 = vis_faligndata(s_1, s_1_old);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_2);
		s_2_old = s_address[0];
		s_1 = s_address[1];
		s_dd20 = vis_faligndata(s_2_old, s_1);
		s_2_old = s_address[2];
		s_dd21 = vis_faligndata(s_1, s_2_old);
		s_address = (mlib_d64 *)vis_alignaddr(s_begin, s_stride_4);
		s_3_old = s_address[0];
		s_1 = s_address[1];
		s_dd30 = vis_faligndata(s_3_old, s_1);
		s_3_old = s_address[2];
		s_dd31 = vis_faligndata(s_1, s_3_old);

		BICUBIC_S16_4_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30,
		    s_dd01, s_dd11, s_dd21, s_dd31, d_prev_1, d_prev_2);
		s_begin += CHAN_3;

		if (emask == 0xff) {
#pragma pipeloop(0)
			for (; i < d_width; i++) {
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    -s_stride_2);
				s_1 = s_address[0];
				s_dd00 = vis_faligndata(s_0_old, s_1);
				s_0_old = s_address[1];
				s_dd01 = vis_faligndata(s_1, s_0_old);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    0);
				s_1 = s_address[0];
				s_dd10 = vis_faligndata(s_1_old, s_1);
				s_1_old = s_address[1];
				s_dd11 = vis_faligndata(s_1, s_1_old);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    s_stride_2);
				s_1 = s_address[0];
				s_dd20 = vis_faligndata(s_2_old, s_1);
				s_2_old = s_address[1];
				s_dd21 = vis_faligndata(s_1, s_2_old);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    s_stride_4);
				s_1 = s_address[0];
				s_dd30 = vis_faligndata(s_3_old, s_1);
				s_3_old = vis_ld_d64_nf(s_address + 1);
				s_dd31 = vis_faligndata(s_1, s_3_old);

				s_begin += CHAN_2;
				BICUBIC_S16_4_DOUBLE_XY(s_dd00, s_dd10, s_dd20,
				    s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
				    d_prev_1, d_prev_2, result);
				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				d_address[0] = result;
				d_address++;
			}
		} else {
#pragma pipeloop(0)
			for (; i < d_width; i++) {
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    -s_stride_2);
				s_1 = s_address[0];
				s_dd00 = vis_faligndata(s_0_old, s_1);
				s_0_old = s_address[1];
				s_dd01 = vis_faligndata(s_1, s_0_old);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    0);
				s_1 = s_address[0];
				s_dd10 = vis_faligndata(s_1_old, s_1);
				s_1_old = s_address[1];
				s_dd11 = vis_faligndata(s_1, s_1_old);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    s_stride_2);
				s_1 = s_address[0];
				s_dd20 = vis_faligndata(s_2_old, s_1);
				s_2_old = s_address[1];
				s_dd21 = vis_faligndata(s_1, s_2_old);
				s_address = (mlib_d64 *)vis_alignaddr(s_begin,
				    s_stride_4);
				s_1 = s_address[0];
				s_dd30 = vis_faligndata(s_3_old, s_1);
				s_3_old = vis_ld_d64_nf(s_address + 1);
				s_dd31 = vis_faligndata(s_1, s_3_old);

				s_begin += CHAN_2;
				BICUBIC_S16_4_DOUBLE_XY(s_dd00, s_dd10, s_dd20,
				    s_dd30, s_dd01, s_dd11, s_dd21, s_dd31,
				    d_prev_1, d_prev_2, result);
				vis_alignaddr(u_align, 1);
				result = vis_faligndata(result, result);
				vis_pst_8(result, d_address, emask);
				d_address++;
				vis_pst_8(result, d_address, no_emask);
			}
		}

		s_begin = s_line += s_stride_2;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Align_XY(
    NEAREST_PARAMETERS1(mlib_s16))
{
	mlib_d64 _d_s16____0w, _d_s16____1w, _d_s16____2w, _d_s16____3w;
	mlib_d64 _d_s32____hi, _d_s32____lo;
	mlib_d64 _d_s16_0, _d_s16_1;
	mlib_d64 result;
	mlib_d64 s_dd00, s_dd01, s_dd10, s_dd11, s_dd20, s_dd21, s_dd30, s_dd31;
	mlib_d64 *d_address, *s_address;
	mlib_d64 d_prev_1, d_prev_2;

	MASK_DEFINITION_S16_4;
	mlib_f32 DMF_0_1_0_1;
	mlib_f32 DMF_0_9_0_9;

	mlib_s32 i, j, s_stride_byte = s_stride << 1,
	    s_stride_8_1 = s_stride >> 1, s_stride_8_2 = s_stride >> 2;
	mlib_s16 *s_line, *d_line;

/* initialize GSR scale factor */
	vis_write_gsr(13 << 3);

	s_line = s_data - CHAN;
	d_line = d_data;
	s_stride_byte = s_stride << 1;

	for (j = 0; j < height; j++) {

		s_address = (mlib_d64 *)s_line;
		d_address = (mlib_d64 *)d_line;

		s_dd00 = s_address[-s_stride_8_2];
		s_dd01 = s_address[-s_stride_8_2 + 1];
		s_dd10 = s_address[0];
		s_dd11 = s_address[1];
		s_dd20 = s_address[s_stride_8_2];
		s_dd21 = s_address[s_stride_8_2 + 1];
		s_dd30 = s_address[s_stride_8_1];
		s_dd31 = s_address[s_stride_8_1 + 1];

		BICUBIC_S16_4_XY_PREV(s_dd00, s_dd10, s_dd20, s_dd30,
		    s_dd01, s_dd11, s_dd21, s_dd31, d_prev_1, d_prev_2);
		s_address += 2;

#pragma pipeloop(0)
		for (i = 0; i < d_width; i++) {
			s_dd00 = s_address[-s_stride_8_2];
			s_dd01 = s_address[-s_stride_8_2 + 1];
			s_dd10 = s_address[0];
			s_dd11 = s_address[1];
			s_dd20 = s_address[s_stride_8_2];
			s_dd21 = s_address[s_stride_8_2 + 1];
			s_dd30 = s_address[s_stride_8_1];
			s_dd31 = s_address[s_stride_8_1 + 1];
			BICUBIC_S16_4_DOUBLE_XY(s_dd00, s_dd10, s_dd20, s_dd30,
			    s_dd01, s_dd11, s_dd21, s_dd31,
			    d_prev_1, d_prev_2, result);
			d_address[0] = result;
			d_address++;
			s_address += 2;
		}

		s_line += s_stride_byte;
		d_line += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Bicubic(
    BILINEAR_PARAMETERS(mlib_s16))
{
	vis_write_gsr(15 << 3);

	if (dh) {
		if (((((mlib_s32)p_dst) | dst_stride | ((mlib_s32)(p_src -
		    CHAN)) | src_stride) & 7) == 0) {
			mlib_ImageZoomOut2X_S16_4_Align_X(PARAM_BC);
			return;
		}

		mlib_ImageZoomOut2X_S16_4_X(PARAM_BC);
		return;
	}

	if (dw) {
		mlib_ImageZoomOut2X_S16_Y(4, PARAM_BC);
		return;
	}

	if (((((mlib_s32)p_dst) | dst_stride | ((mlib_s32)(p_src -
	    CHAN)) | src_stride) & 7) == 0) {
		mlib_ImageZoomOut2X_S16_4_Align_XY(PARAM_BC);
		return;
	}

	mlib_ImageZoomOut2X_S16_4_XY(PARAM_BC);
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Bicubic2(
    BILINEAR_PARAMETERS(mlib_s16))
{
	vis_write_gsr(15 << 3);

	if (dh) {
		if (((((mlib_s32)p_dst) | dst_stride | ((mlib_s32)(p_src -
		    CHAN)) | src_stride) & 7) == 0) {
			mlib_ImageZoomOut2X_S16_4_Align_X(PARAM_BC2);
			return;
		}

		mlib_ImageZoomOut2X_S16_4_X(PARAM_BC2);
		return;
	}

	if (dw) {
		mlib_ImageZoomOut2X_S16_Y(4, PARAM_BC2);
		return;
	}

	if (((((mlib_s32)p_dst) | dst_stride | ((mlib_s32)(p_src -
	    CHAN)) | src_stride) & 7) == 0) {
		mlib_ImageZoomOut2X_S16_4_Align_XY(PARAM_BC2);
		return;
	}

	mlib_ImageZoomOut2X_S16_4_XY(PARAM_BC2);
}

/* *********************************************************** */
