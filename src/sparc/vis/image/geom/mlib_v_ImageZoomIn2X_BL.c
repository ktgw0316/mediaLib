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

#pragma ident	"@(#)mlib_v_ImageZoomIn2X_BL.c	9.3	07/11/05 SMI"

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
 *      edge    Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 */

#include <vis_proto.h>
#include <mlib_image.h>

/* *********************************************************** */

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))
#define	VIS_ALIGNADDRL(X, Y)	vis_alignaddrl((void *)(X), (Y))

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                              \
	MLIB_TYPE  *pdst,                                             \
	const MLIB_TYPE  *psrc,                                       \
	mlib_s32 src_width,                                           \
	mlib_s32 src_height,                                          \
	mlib_s32 dst_stride,                                          \
	mlib_s32 src_stride,                                          \
	mlib_s32 src_w_beg,                                           \
	mlib_s32 src_h_beg,                                           \
	mlib_s32 channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE)                                \
	(MLIB_TYPE *) pdst, (MLIB_TYPE *) psrc, src_width, src_height,   \
	dst_stride, src_stride, src_w_beg, src_h_beg, channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM_BL(MLIB_TYPE)                           \
	MLIB_TYPE  *pdst,                                             \
	const MLIB_TYPE  *psrc,                                       \
	mlib_s32 src_width,                                           \
	mlib_s32 src_height,                                          \
	mlib_s32 dst_stride,                                          \
	mlib_s32 src_stride

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM_BL(MLIB_TYPE)                             \
	(MLIB_TYPE *) pdst, (MLIB_TYPE *) psrc, src_width, src_height,   \
	dst_stride, src_stride

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL0	(9*a00 + 3*a01 + 3*a10 + a11) >> 4
#define	MLIB_ZOOMIN2X_UR_FL0	(3*a00 + 9*a01 + a10 + 3*a11) >> 4

#define	MLIB_ZOOMIN2X_DL_FL0	(3*a00 + a01 + 9*a10 + 3*a11) >> 4

#define	MLIB_ZOOMIN2X_DR_FL0	(a00 + 3*a01 + 3*a10 + 9*a11) >> 4

#define	MLIB_ZOOMIN2X_UL_FL1	(3*a00 + a01) >> 2

#define	MLIB_ZOOMIN2X_UR_FL1	(a00 + 3*a01) >> 2

#define	MLIB_ZOOMIN2X_DL_FL1	(3*a00 + a01 + 3*a10 + a11) >> 3

#define	MLIB_ZOOMIN2X_DR_FL1	(a00 + 3*a01 + a10 + 3*a11) >> 3

#define	MLIB_ZOOMIN2X_UL_FL2	(3*a00 + a10) >> 2

#define	MLIB_ZOOMIN2X_UR_FL2	(3*a00 + 3*a01 + a10 + a11) >> 3

#define	MLIB_ZOOMIN2X_DL_FL2	(a00 + 3*a10) >> 2

#define	MLIB_ZOOMIN2X_DR_FL2	(a00 + a01 + 3*a10 + 3*a11) >> 3

#define	MLIB_ZOOMIN2X_UL_FL3	a00

#define	MLIB_ZOOMIN2X_UR_FL3	(a00 + a01) >> 1

#define	MLIB_ZOOMIN2X_DL_FL3	(a00 + a10) >> 1

#define	MLIB_ZOOMIN2X_DR_FL3	(a00 + a01 + a10 + a11) >> 2

/* *********************************************************** */

typedef union
{
	mlib_d64 db;
	struct
	{
		mlib_f32 fl0;
		mlib_f32 fl1;
	} two_float;
} type_union_mlib_d64;

#define	SIZE_OF_CACH	16384

/* *********************************************************** */

#define	SHAG_STR(type)                                          \
	shag_row = SIZE_OF_CACH / sizeof (type) / src_stride;   \
	if (shag_row < 4)                                       \
	    shag_row = 4

/* *********************************************************** */

/*
 * COMMON MACROS STRUCTURE OF mlib_v_ZoomIn2X_*BL* FUNCTIONS
 *
 * LOAD_xxx - load to w_src10, w_src11; w_src10 consists of
 *   left bottom values, w_src11 consists of right bottom values
 *
 * MAKE_xxx - do some transforming w_src10, w_src11 to w_tmp_xxx
 *
 * for(xxx) { - cycle by rows
 *
 *   MOVE_xxx - move some w_tmp_xxx to w_old_xxx
 *
 *   LOAD_xxx - load again to w_src10, w_src11
 *
 *   MAKE_xxx - do some transforming w_src10, w_src11 to w_tmp_xxx
 *
 *   TAIL_xxx - make w_res0, w_res1, w_res2, w_res3 from w_old_xxx,
 *              w_tmp_xxx; w_res0, w_res2 is result's upper values,
 *              w_res1, w_res3 is result's bottom values
 *
 *  STORE_xxx - store w_res0, w_res1, w_res2, w_res3
 *
 * } - end of cycle block
 *
 * *FL3 functions left from old code
 *
 * Don't convolve others to the one macro, cause it make
 * impossible to optimize separate functions
 *
 */

/* *********************************************************** */

#define	LOAD_1U8A(l0, l1, psrc, index)                          \
	{                                                       \
	    mlib_f32 *ps = (mlib_f32 *)(psrc + index);          \
	                                                        \
	    l0 = vis_freg_pair(ps[0], ps[1]);                   \
	    l1 = vis_freg_pair(ps[2], v_zero);                  \
	    l1 = vis_faligndata(l0, l1);                        \
	}

/* *********************************************************** */

#define	STORE_1U8A(pdst, index, r0, r1)                         \
	((mlib_d64 *)(pdst + index))[0] = r0;                   \
	((mlib_d64 *)(pdst + index))[1] = r1

/* *********************************************************** */

#define	LOAD_1U8G(l0, l1, psrc, index)                          \
	{                                                       \
	    mlib_d64 *ps = VIS_ALIGNADDR(psrc, index);          \
	                                                        \
	    l0 = ps[0];                                         \
	    l1 = ps[1];                                         \
	    l0 = vis_faligndata(l0, l1);                        \
	    l1 = vis_faligndata(l1, ps[2]);                     \
	    VIS_ALIGNADDR(0, channels);                         \
	    l1 = vis_faligndata(l0, l1);                        \
	}

/* *********************************************************** */

#define	LOAD_1U8G_NF(l0, l1, psrc, index)                       \
	{                                                       \
	    mlib_d64 *ps = VIS_ALIGNADDR(psrc, index);          \
	                                                        \
	    l0 = vis_ld_d64_nf(ps);                             \
	    l1 = vis_ld_d64_nf(ps + 1);                         \
	    l0 = vis_faligndata(l0, l1);                        \
	    l1 = vis_faligndata(l1, vis_ld_d64_nf(ps + 2));     \
	    VIS_ALIGNADDR(0, channels);                         \
	    l1 = vis_faligndata(l0, l1);                        \
	}

/* *********************************************************** */

#define	STORE_1U8G(pdst, index, r0, r1)                                \
	{                                                              \
	    mlib_d64 *pd =                                             \
		(mlib_d64 *)((mlib_addr)(pdst + index - 1) & ~7);      \
	    mlib_s32 emask =                                           \
		0xFF >> ((mlib_addr)(pdst + index) - (mlib_addr)pd);   \
	    VIS_ALIGNADDRL(pdst + index, 0);                           \
	    vis_pst_8(vis_faligndata(r0, r0), pd, emask);              \
	    pd[1] = vis_faligndata(r0, r1);                            \
	    vis_pst_8(vis_faligndata(r1, r1), pd + 2, ~emask);         \
	}

/* *********************************************************** */

#define	STORE_1U8G_NF(pdst, index, r0, r1)                             \
	{                                                              \
	    mlib_d64 *pd =                                             \
		(mlib_d64 *)((mlib_addr)(pdst + index - 1) & ~7);      \
	    mlib_s32 emask =                                           \
		0xFF >> ((mlib_addr)(pdst + index) - (mlib_addr)pd);   \
	    VIS_ALIGNADDRL(pdst + index, 0);                           \
	    if (emask)                                                 \
		vis_pst_8(vis_faligndata(r0, r0), pd, emask);          \
	    pd[1] = vis_faligndata(r0, r1);                            \
	    vis_pst_8(vis_faligndata(r1, r1), pd + 2, ~emask);         \
	}

/* *********************************************************** */

#define	STORE_1U8G_LAST(pdst, index, r0, r1)                            \
	{                                                               \
	    mlib_d64 *pd =                                              \
		(mlib_d64 *)((mlib_addr)(pdst + index - 1) & ~7);       \
	    mlib_s32 emask0 =                                           \
		lmask >> (8 + ((mlib_addr)(pdst + index) -              \
		(mlib_addr)pd));                                        \
	    mlib_s32 emask1 =                                           \
		lmask >> ((mlib_addr)(pdst + index) - (mlib_addr)pd);   \
	    mlib_s32 emask2 =                                           \
		lmask << (8 - (mlib_addr)(pdst + index) +               \
		(mlib_addr)pd);                                         \
	    VIS_ALIGNADDRL(pdst + index, 0);                            \
	    if (emask0 & 0xFF)                                          \
		vis_pst_8(vis_faligndata(r0, r0), pd, emask0 & 0xFF);   \
	    if (emask1 & 0xFF)                                          \
		vis_pst_8(vis_faligndata(r0, r1), pd + 1,               \
		    emask1 & 0xFF);                                     \
	    if (emask2 & 0xFF)                                          \
		vis_pst_8(vis_faligndata(r1, r1), pd + 2,               \
		    emask2 & 0xFF);                                     \
	}

/* *********************************************************** */

#define	MOVE_1U8_FL0                                            \
	w_old_13h = w_tmp_13h;                                  \
	w_old_31h = w_tmp_31h;                                  \
	w_old_93h = w_tmp_93h;                                  \
	w_old_39h = w_tmp_39h;                                  \
	w_old_13l = w_tmp_13l;                                  \
	w_old_31l = w_tmp_31l;                                  \
	w_old_93l = w_tmp_93l;                                  \
	w_old_39l = w_tmp_39l

/* *********************************************************** */

#define	MOVE_2U8_FL0                                            \
	w_old_13h = vis_fmul8x16(v_coeff_3, w_tmp_39h);         \
	w_old_31h = vis_fmul8x16(v_coeff_3, w_tmp_93h);         \
	w_old_93h = w_tmp_93h;                                  \
	w_old_39h = w_tmp_39h;                                  \
	w_old_13l = vis_fmul8x16(v_coeff_3, w_tmp_39l);         \
	w_old_31l = vis_fmul8x16(v_coeff_3, w_tmp_93l);         \
	w_old_93l = w_tmp_93l;                                  \
	w_old_39l = w_tmp_39l

/* *********************************************************** */

#define	MAKE_1U8_FL0                                                \
	w_tmp10_1 = vis_fmul8x16(vis_read_hi(w_src10), w_coeff1);   \
	w_tmp10_3 = vis_fmul8x16(vis_read_hi(w_src10), w_coeff3);   \
	w_tmp10_9 = vis_fmul8x16(vis_read_hi(w_src10), w_coeff9);   \
	w_tmp11_1 = vis_fmul8x16(vis_read_hi(w_src11), w_coeff1);   \
	w_tmp11_3 = vis_fmul8x16(vis_read_hi(w_src11), w_coeff3);   \
	w_tmp11_9 = vis_fmul8x16(vis_read_hi(w_src11), w_coeff9);   \
	w_tmp_13h = vis_fpadd16(w_tmp10_1, w_tmp11_3);              \
	w_tmp_31h = vis_fpadd16(w_tmp10_3, w_tmp11_1);              \
	w_tmp_39h = vis_fpadd16(w_tmp10_3, w_tmp11_9);              \
	w_tmp_93h = vis_fpadd16(w_tmp10_9, w_tmp11_3);              \
	w_tmp10_1 = vis_fmul8x16(vis_read_lo(w_src10), w_coeff1);   \
	w_tmp10_3 = vis_fmul8x16(vis_read_lo(w_src10), w_coeff3);   \
	w_tmp10_9 = vis_fmul8x16(vis_read_lo(w_src10), w_coeff9);   \
	w_tmp11_1 = vis_fmul8x16(vis_read_lo(w_src11), w_coeff1);   \
	w_tmp11_3 = vis_fmul8x16(vis_read_lo(w_src11), w_coeff3);   \
	w_tmp11_9 = vis_fmul8x16(vis_read_lo(w_src11), w_coeff9);   \
	w_tmp_13l = vis_fpadd16(w_tmp10_1, w_tmp11_3);              \
	w_tmp_31l = vis_fpadd16(w_tmp10_3, w_tmp11_1);              \
	w_tmp_39l = vis_fpadd16(w_tmp10_3, w_tmp11_9);              \
	w_tmp_93l = vis_fpadd16(w_tmp10_9, w_tmp11_3)

/* *********************************************************** */

#define	TAIL_1U8_FL0(RESULT)                                    \
	w_res01 = vis_fpadd16(w_old_39h, w_tmp_13h);            \
	w_res00 = vis_fpadd16(w_old_93h, w_tmp_31h);            \
	RESULT(w_res0, w_res00, w_res01);                       \
	w_res11 = vis_fpadd16(w_old_13h, w_tmp_39h);            \
	w_res10 = vis_fpadd16(w_old_31h, w_tmp_93h);            \
	RESULT(w_res1, w_res10, w_res11);                       \
	w_res01 = vis_fpadd16(w_old_39l, w_tmp_13l);            \
	w_res00 = vis_fpadd16(w_old_93l, w_tmp_31l);            \
	RESULT(w_res2, w_res00, w_res01);                       \
	w_res11 = vis_fpadd16(w_old_13l, w_tmp_39l);            \
	w_res10 = vis_fpadd16(w_old_31l, w_tmp_93l);            \
	RESULT(w_res3, w_res10, w_res11)

/* *********************************************************** */

#define	TAIL_3U8_FL0(RESULT)                                    \
	w_res01 = vis_fpadd16(w_old_39h, w_tmp_13h);            \
	w_res00 = vis_fpadd16(w_old_93h, w_tmp_31h);            \
	w_res11 = vis_fpadd16(w_old_39l, w_tmp_13l);            \
	w_res10 = vis_fpadd16(w_old_93l, w_tmp_31l);            \
	RESULT(w_res2, w_res01, w_res11);                       \
	RESULT(w_res0, w_res00, w_res10);                       \
	w_res01 = vis_fpadd16(w_old_13h, w_tmp_39h);            \
	w_res00 = vis_fpadd16(w_old_31h, w_tmp_93h);            \
	w_res11 = vis_fpadd16(w_old_13l, w_tmp_39l);            \
	w_res10 = vis_fpadd16(w_old_31l, w_tmp_93l);            \
	RESULT(w_res3, w_res01, w_res11);                       \
	RESULT(w_res1, w_res00, w_res10)

/* *********************************************************** */

#define	RESULT_1U8(w_res, s0, s1)                               \
	w_res = vis_fpmerge(vis_fpack16(s0), vis_fpack16(s1))

/* *********************************************************** */

#define	RESULT_2U8(w_res, s0, s1)                                      \
	{                                                              \
	    mlib_f32 v_tmp = vis_read_hi(s1);                          \
	                                                               \
	    s1 = vis_write_hi(s1, vis_read_lo(s0));                    \
	    s0 = vis_write_lo(s0, v_tmp);                              \
	    w_res = vis_freg_pair(vis_fpack16(s0), vis_fpack16(s1));   \
	}

/* *********************************************************** */

#define	RESULT_4U8(w_res, s0, s1)                                 \
	w_res = vis_freg_pair(vis_fpack16(s0), vis_fpack16(s1))

/* *********************************************************** */

#define	TRANSFORM_3U8(r0, r1)                                   \
	{                                                       \
	    mlib_d64 result1, result2;                          \
	                                                        \
	    result2 = vis_faligndata(result2, r0);              \
	    r0 = vis_faligndata(r0, r0);                        \
	    result2 = vis_faligndata(result2, r1);              \
	    r1 = vis_faligndata(r1, result2);                   \
	    result2 = vis_faligndata(result2, r0);              \
	    result1 = vis_faligndata(r1, result2);              \
	    result2 = vis_faligndata(result2, r1);              \
	    r1 = vis_faligndata(result1, result2);              \
	    result2 = vis_faligndata(result2, result2);         \
	    r0 = vis_faligndata(r0, r1);                        \
	    r1 = vis_faligndata(r1, result2);                   \
	    result2 = vis_faligndata(result2, result2);         \
	    r0 = vis_faligndata(r0, r1);                        \
	    r1 = vis_faligndata(r1, result2);                   \
	    r0 = vis_faligndata(r0, r1);                        \
	    r1 = vis_faligndata(r1, r1);                        \
	}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_1BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 1;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_d64 w_coeff9 = vis_to_double_dup((9 << 8) | (9 << 24));
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93h, w_tmp_39h, w_tmp_31h, w_tmp_13h;
	mlib_d64 w_tmp_93l, w_tmp_39l, w_tmp_31l, w_tmp_13l;
	mlib_d64 w_old_93h, w_old_39h, w_old_31h, w_old_13h;
	mlib_d64 w_old_93l, w_old_39l, w_old_31l, w_old_13l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((3 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL0;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U8_FL0;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_1U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL0;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL0;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_1U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL0;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_1U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL0;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL0;

				TAIL_1U8_FL0(RESULT_1U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_2BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 2;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_d64 w_coeff9 = vis_to_double_dup((9 << 8) | (9 << 24));
	const mlib_f32 v_coeff_3 =
	    vis_to_float((85 << 24) | (85 << 16) | (85 << 8) | 85);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93h, w_tmp_39h, w_tmp_31h, w_tmp_13h;
	mlib_d64 w_tmp_93l, w_tmp_39l, w_tmp_31l, w_tmp_13l;
	mlib_d64 w_old_93h, w_old_39h, w_old_31h, w_old_13h;
	mlib_d64 w_old_93l, w_old_39l, w_old_31l, w_old_13l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((3 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL0;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_2U8_FL0;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_2U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL0;

				if (src_height / 2 > 0) {

					MOVE_2U8_FL0;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_2U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_2U8_FL0;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_2U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_2U8_FL0;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL0;

				TAIL_1U8_FL0(RESULT_2U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_3BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 3;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_d64 w_coeff9 = vis_to_double_dup((9 << 8) | (9 << 24));
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93h, w_tmp_39h, w_tmp_31h, w_tmp_13h;
	mlib_d64 w_tmp_93l, w_tmp_39l, w_tmp_31l, w_tmp_13l;
	mlib_d64 w_old_93h, w_old_39h, w_old_31h, w_old_13h;
	mlib_d64 w_old_93l, w_old_39l, w_old_31l, w_old_13l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row, lmask;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {

		lmask = 0xFFF0;
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((3 << 3) | channels);

		for (j = 0; j < src_width / 2 - 5; j += 6) {

			LOAD_1U8G(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL0;

				LOAD_1U8G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL0;

				TAIL_3U8_FL0(RESULT_4U8);

				TRANSFORM_3U8(w_res0, w_res2);

				TRANSFORM_3U8(w_res1, w_res3);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		if (j < src_width / 2) {
			lmask = (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL0;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL0;

				TAIL_3U8_FL0(RESULT_4U8);

				TRANSFORM_3U8(w_res0, w_res2);

				TRANSFORM_3U8(w_res1, w_res3);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_4BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 4;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_d64 w_coeff9 = vis_to_double_dup((9 << 8) | (9 << 24));
	const mlib_f32 v_coeff_3 =
	    vis_to_float((85 << 24) | (85 << 16) | (85 << 8) | 85);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93h, w_tmp_39h, w_tmp_31h, w_tmp_13h;
	mlib_d64 w_tmp_93l, w_tmp_39l, w_tmp_31l, w_tmp_13l;
	mlib_d64 w_old_93h, w_old_39h, w_old_31h, w_old_13h;
	mlib_d64 w_old_93l, w_old_39l, w_old_31l, w_old_13l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((3 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL0;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_2U8_FL0;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_4U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL0;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL0;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_4U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL0;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL0;

					TAIL_1U8_FL0(RESULT_4U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL0;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL0;

				TAIL_1U8_FL0(RESULT_4U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	MOVE_1U8_FL1                                            \
	w_old_30h = w_tmp_30h;                                  \
	w_old_33h = w_tmp_33h;                                  \
	w_old_30l = w_tmp_30l;                                  \
	w_old_33l = w_tmp_33l

/* *********************************************************** */

#define	MAKE_1U8_FL1                                                \
	w_tmp_10h = vis_fmul8x16(vis_read_hi(w_src10), w_coeff1);   \
	w_tmp_33h = vis_fmul8x16(vis_read_hi(w_src11), w_coeff3);   \
	w_tmp_30h = vis_fmul8x16(vis_read_hi(w_src10), w_coeff3);   \
	w_tmp_11h = vis_fmul8x16(vis_read_hi(w_src11), w_coeff1);   \
	w_tmp_33h = vis_fpadd16(w_tmp_33h, w_tmp_10h);              \
	w_tmp_30h = vis_fpadd16(w_tmp_30h, w_tmp_11h);              \
	w_tmp_10l = vis_fmul8x16(vis_read_lo(w_src10), w_coeff1);   \
	w_tmp_33l = vis_fmul8x16(vis_read_lo(w_src11), w_coeff3);   \
	w_tmp_30l = vis_fmul8x16(vis_read_lo(w_src10), w_coeff3);   \
	w_tmp_11l = vis_fmul8x16(vis_read_lo(w_src11), w_coeff1);   \
	w_tmp_33l = vis_fpadd16(w_tmp_33l, w_tmp_10l);              \
	w_tmp_30l = vis_fpadd16(w_tmp_30l, w_tmp_11l)

/* *********************************************************** */

#define	TAIL_1U8_FL1(RESULT)                                    \
	w_res01 = vis_fpadd16(w_old_33h, w_old_33h);            \
	w_res00 = vis_fpadd16(w_old_30h, w_old_30h);            \
	RESULT(w_res0, w_res00, w_res01);                       \
	w_res11 = vis_fpadd16(w_old_33h, w_tmp_33h);            \
	w_res10 = vis_fpadd16(w_old_30h, w_tmp_30h);            \
	RESULT(w_res1, w_res10, w_res11);                       \
	w_res01 = vis_fpadd16(w_old_33l, w_old_33l);            \
	w_res00 = vis_fpadd16(w_old_30l, w_old_30l);            \
	RESULT(w_res2, w_res00, w_res01);                       \
	w_res11 = vis_fpadd16(w_old_33l, w_tmp_33l);            \
	w_res10 = vis_fpadd16(w_old_30l, w_tmp_30l);            \
	RESULT(w_res3, w_res10, w_res11)

/* *********************************************************** */

#define	TAIL_3U8_FL1(RESULT)                                    \
	w_res01 = vis_fpadd16(w_old_33h, w_old_33h);            \
	w_res00 = vis_fpadd16(w_old_30h, w_old_30h);            \
	w_res11 = vis_fpadd16(w_old_33l, w_old_33l);            \
	w_res10 = vis_fpadd16(w_old_30l, w_old_30l);            \
	RESULT(w_res2, w_res01, w_res11);                       \
	RESULT(w_res0, w_res00, w_res10);                       \
	w_res01 = vis_fpadd16(w_old_33h, w_tmp_33h);            \
	w_res00 = vis_fpadd16(w_old_30h, w_tmp_30h);            \
	w_res11 = vis_fpadd16(w_old_33l, w_tmp_33l);            \
	w_res10 = vis_fpadd16(w_old_30l, w_tmp_30l);            \
	RESULT(w_res3, w_res01, w_res11);                       \
	RESULT(w_res1, w_res00, w_res10)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_1BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 1;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_30h, w_old_33h;
	mlib_d64 w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL1;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U8_FL1;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_1U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL1;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL1;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_1U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL1;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_1U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL1;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL1;

				TAIL_1U8_FL1(RESULT_1U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_2BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 2;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_30h, w_old_33h;
	mlib_d64 w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL1;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U8_FL1;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_2U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL1;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL1;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_2U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL1;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_2U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL1;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL1;

				TAIL_1U8_FL1(RESULT_2U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_3BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 3;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_30h, w_old_33h;
	mlib_d64 w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;
	mlib_s32 lmask;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {

		lmask = 0xFFF0;
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		for (j = 0; j < src_width / 2 - 5; j += 6) {

			LOAD_1U8G(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL1;

				LOAD_1U8G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL1;

				TAIL_3U8_FL1(RESULT_4U8);

				TRANSFORM_3U8(w_res0, w_res2);

				TRANSFORM_3U8(w_res1, w_res3);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		if (j < src_width / 2) {
			lmask = (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL1;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL1;

				TAIL_3U8_FL1(RESULT_4U8);

				TRANSFORM_3U8(w_res0, w_res2);

				TRANSFORM_3U8(w_res1, w_res3);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_4BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 4;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_30h, w_old_33h;
	mlib_d64 w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL1;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U8_FL1;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_4U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL1;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL1;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_4U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL1;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL1;

					TAIL_1U8_FL1(RESULT_4U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL1;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL1;

				TAIL_1U8_FL1(RESULT_4U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	MOVE_1U8_FL2                                            \
	w_old_10h = w_tmp_10h;                                  \
	w_old_11h = w_tmp_11h;                                  \
	w_old_30h = w_tmp_30h;                                  \
	w_old_33h = w_tmp_33h;                                  \
	w_old_10l = w_tmp_10l;                                  \
	w_old_11l = w_tmp_11l;                                  \
	w_old_30l = w_tmp_30l;                                  \
	w_old_33l = w_tmp_33l

/* *********************************************************** */

#define	MOVE_2U8_FL2                                            \
	w_old_10h = vis_fmul8x16(v_coeff_3, w_tmp_30h);         \
	w_old_11h = vis_fmul8x16(v_coeff_3, w_tmp_33h);         \
	w_old_10l = vis_fmul8x16(v_coeff_3, w_tmp_30l);         \
	w_old_11l = vis_fmul8x16(v_coeff_3, w_tmp_33l);         \
	w_old_30h = w_tmp_30h;                                  \
	w_old_33h = w_tmp_33h;                                  \
	w_old_30l = w_tmp_30l;                                  \
	w_old_33l = w_tmp_33l

/* *********************************************************** */

#define	MAKE_1U8_FL2                                                \
	w_tmp_30h = vis_fmul8x16(vis_read_hi(w_src10), w_coeff3);   \
	w_tmp_33h = vis_fmul8x16(vis_read_hi(w_src11), w_coeff3);   \
	w_tmp_10h = vis_fmul8x16(vis_read_hi(w_src10), w_coeff1);   \
	w_tmp_11h = vis_fmul8x16(vis_read_hi(w_src11), w_coeff1);   \
	w_tmp_33h = vis_fpadd16(w_tmp_33h, w_tmp_30h);              \
	w_tmp_30h = vis_fpadd16(w_tmp_30h, w_tmp_30h);              \
	w_tmp_11h = vis_fpadd16(w_tmp_11h, w_tmp_10h);              \
	w_tmp_10h = vis_fpadd16(w_tmp_10h, w_tmp_10h);              \
	w_tmp_30l = vis_fmul8x16(vis_read_lo(w_src10), w_coeff3);   \
	w_tmp_33l = vis_fmul8x16(vis_read_lo(w_src11), w_coeff3);   \
	w_tmp_10l = vis_fmul8x16(vis_read_lo(w_src10), w_coeff1);   \
	w_tmp_11l = vis_fmul8x16(vis_read_lo(w_src11), w_coeff1);   \
	w_tmp_33l = vis_fpadd16(w_tmp_33l, w_tmp_30l);              \
	w_tmp_30l = vis_fpadd16(w_tmp_30l, w_tmp_30l);              \
	w_tmp_11l = vis_fpadd16(w_tmp_11l, w_tmp_10l);              \
	w_tmp_10l = vis_fpadd16(w_tmp_10l, w_tmp_10l)

/* *********************************************************** */

#define	TAIL_1U8_FL2(RESULT)                                    \
	w_res01 = vis_fpadd16(w_old_33h, w_tmp_11h);            \
	w_res00 = vis_fpadd16(w_old_30h, w_tmp_10h);            \
	RESULT(w_res0, w_res00, w_res01);                       \
	w_res11 = vis_fpadd16(w_old_11h, w_tmp_33h);            \
	w_res10 = vis_fpadd16(w_old_10h, w_tmp_30h);            \
	RESULT(w_res1, w_res10, w_res11);                       \
	w_res01 = vis_fpadd16(w_old_33l, w_tmp_11l);            \
	w_res00 = vis_fpadd16(w_old_30l, w_tmp_10l);            \
	RESULT(w_res2, w_res00, w_res01);                       \
	w_res11 = vis_fpadd16(w_old_11l, w_tmp_33l);            \
	w_res10 = vis_fpadd16(w_old_10l, w_tmp_30l);            \
	RESULT(w_res3, w_res10, w_res11)

/* *********************************************************** */

#define	TAIL_3U8_FL2(RESULT)                                    \
	w_res01 = vis_fpadd16(w_old_33h, w_tmp_11h);            \
	w_res00 = vis_fpadd16(w_old_30h, w_tmp_10h);            \
	w_res11 = vis_fpadd16(w_old_33l, w_tmp_11l);            \
	w_res10 = vis_fpadd16(w_old_30l, w_tmp_10l);            \
	RESULT(w_res2, w_res01, w_res11);                       \
	RESULT(w_res0, w_res00, w_res10);                       \
	w_res01 = vis_fpadd16(w_old_11h, w_tmp_33h);            \
	w_res00 = vis_fpadd16(w_old_10h, w_tmp_30h);            \
	w_res11 = vis_fpadd16(w_old_11l, w_tmp_33l);            \
	w_res10 = vis_fpadd16(w_old_10l, w_tmp_30l);            \
	RESULT(w_res3, w_res01, w_res11);                       \
	RESULT(w_res1, w_res00, w_res10)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_1BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 1;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_10h, w_old_11h, w_old_30h, w_old_33h;
	mlib_d64 w_old_10l, w_old_11l, w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL2;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U8_FL2;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_1U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL2;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL2;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_1U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL2;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_1U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL2;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL2;

				TAIL_1U8_FL2(RESULT_1U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_2BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 2;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_coeff_3 =
	    vis_to_float((85 << 24) | (85 << 16) | (85 << 8) | 85);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_10h, w_old_11h, w_old_30h, w_old_33h;
	mlib_d64 w_old_10l, w_old_11l, w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL2;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_2U8_FL2;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_2U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL2;

				if (src_height / 2 > 0) {

					MOVE_2U8_FL2;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_2U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_2U8_FL2;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_2U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_2U8_FL2;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL2;

				TAIL_1U8_FL2(RESULT_2U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_3BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 3;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_coeff_3 =
	    vis_to_float((85 << 24) | (85 << 16) | (85 << 8) | 85);
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_10h, w_old_11h, w_old_30h, w_old_33h;
	mlib_d64 w_old_10l, w_old_11l, w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;
	mlib_s32 lmask;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {

		lmask = 0xFFF0;
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		for (j = 0; j < src_width / 2 - 5; j += 6) {

			LOAD_1U8G(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_2U8_FL2;

				LOAD_1U8G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL2;

				TAIL_3U8_FL2(RESULT_4U8);

				TRANSFORM_3U8(w_res0, w_res2);

				TRANSFORM_3U8(w_res1, w_res3);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		if (j < src_width / 2) {
			lmask = (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_2U8_FL2;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL2;

				TAIL_3U8_FL2(RESULT_4U8);

				TRANSFORM_3U8(w_res0, w_res2);

				TRANSFORM_3U8(w_res1, w_res3);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_4BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, channels = 4;
	const mlib_d64 w_coeff1 = vis_to_double_dup((1 << 8) | (1 << 24));
	const mlib_d64 w_coeff3 = vis_to_double_dup((3 << 8) | (3 << 24));
	const mlib_f32 v_coeff_3 =
	    vis_to_float((85 << 24) | (85 << 16) | (85 << 8) | 85);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_res00, w_res01, w_res10, w_res11;
	mlib_d64 w_tmp_10h, w_tmp_11h, w_tmp_30h, w_tmp_33h;
	mlib_d64 w_tmp_10l, w_tmp_11l, w_tmp_30l, w_tmp_33l;
	mlib_d64 w_old_10h, w_old_11h, w_old_30h, w_old_33h;
	mlib_d64 w_old_10l, w_old_11l, w_old_30l, w_old_33l;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr((4 << 3) | channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8A(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL2;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_2U8_FL2;

					LOAD_1U8A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_4U8);

					STORE_1U8A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 7; j += 8) {

				LOAD_1U8G(w_src10, w_src11, psrc, j);

				MAKE_1U8_FL2;

				if (src_height / 2 > 0) {

					MOVE_1U8_FL2;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_4U8);

					STORE_1U8G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U8_FL2;

					LOAD_1U8G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U8_FL2;

					TAIL_1U8_FL2(RESULT_4U8);

					STORE_1U8G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U8G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (2 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U8G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U8_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U8_FL2;

				LOAD_1U8G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U8_FL2;

				TAIL_1U8_FL2(RESULT_4U8);

				STORE_1U8G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U8G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	BILINEAR_U8_14_PART1	dr = vis_fpack16_pair(da1h, da1l)

/* *********************************************************** */

#define	BILINEAR_U8_14_PART2                                    \
	dr1l = vis_fpadd16(dal, ddl);                           \
	dr1h = vis_fpadd16(dah, ddh);                           \
	dr1h = vis_fpack16_pair(dr1h, dr1l);                    \
	da1l = vis_fpadd16(da1l, dd1l);                         \
	da1h = vis_fpadd16(da1h, dd1h);                         \
	da1l = vis_fmul8x16(scale05, da1l);                     \
	da1h = vis_fmul8x16(scale05, da1h);                     \
	dr1l = vis_fpack16_pair(da1h, da1l)

/* *********************************************************** */

#define	BILINEAR_U8_14_TAIL                                     \
	da = dd;                                                \
	da1h = dd1h;                                            \
	da1l = dd1l;                                            \
	dal = vis_fmul8x16al(vis_read_lo(da), fone);            \
	dah = vis_fmul8x16al(vis_read_hi(da), fone)

/* *********************************************************** */

#define	STORE_RES_U8_COMMON(pointer, res1, res2)                      \
	{                                                             \
	    mlib_d64 *dpz = (mlib_d64 *)VIS_ALIGNADDR(pointer, -1);   \
	    mlib_s32 emask =                                          \
		0xff >> ((mlib_addr)pointer - (mlib_addr)dpz);        \
	    (void *)VIS_ALIGNADDRL(pointer, 0);                       \
	    vis_pst_8(vis_faligndata(res1, res1), dpz, emask);        \
	    dpz[1] = vis_faligndata(res1, res2);                      \
	    vis_pst_8(vis_faligndata(res2, res2), dpz + 2, ~emask);   \
	}

/* *********************************************************** */

#define	STORE_RES_U8_COMMON_NF(pointer, res1, res2)                   \
	{                                                             \
	    mlib_d64 *dpz = (mlib_d64 *)VIS_ALIGNADDR(pointer, -1);   \
	    mlib_s32 emask =                                          \
		0xff >> ((mlib_addr)pointer - (mlib_addr)dpz);        \
	    (void *)VIS_ALIGNADDRL(pointer, 0);                       \
	    if (emask)                                                \
		vis_pst_8(vis_faligndata(res1, res1), dpz, emask);    \
	    dpz[1] = vis_faligndata(res1, res2);                      \
	    vis_pst_8(vis_faligndata(res2, res2), dpz + 2, ~emask);   \
	}

/* *********************************************************** */

#define	GET_STR_U8_1_BILINEAR(ind, da, dah, dal, da1h, da1l)    \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = dprow[0];                                          \
	da1h = dprow[1];                                        \
	da1l = dprow[2];                                        \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fmul8x16al(vis_read_lo(da), fone);            \
	dah = vis_fmul8x16al(vis_read_hi(da), fone);            \
	da1h = vis_faligndata(da1h, da1l);                      \
	VIS_ALIGNADDR(dprow, 1);                                \
	da1h = vis_faligndata(da, da1h);                        \
/* << 8 */                                                      \
	da1l = vis_fpmerge(fzero, vis_read_lo(da1h));           \
	da1h = vis_fpmerge(fzero, vis_read_hi(da1h));           \
	da1l = vis_fpadd16(da1l, dal);                          \
	da1h = vis_fpadd16(da1h, dah)

/* *********************************************************** */

#define	GET_STR_U8_1_BILINEAR_NF(ind, da, dah, dal, da1h, da1l) \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = vis_ld_d64_nf(dprow);                              \
	da1h = vis_ld_d64_nf(dprow + 1);                        \
	da1l = vis_ld_d64_nf(dprow + 2);                        \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fmul8x16al(vis_read_lo(da), fone);            \
	dah = vis_fmul8x16al(vis_read_hi(da), fone);            \
	da1h = vis_faligndata(da1h, da1l);                      \
	VIS_ALIGNADDR(dprow, 1);                                \
	da1h = vis_faligndata(da, da1h);                        \
/* << 8 */                                                      \
	da1l = vis_fpmerge(fzero, vis_read_lo(da1h));           \
	da1h = vis_fpmerge(fzero, vis_read_hi(da1h));           \
	da1l = vis_fpadd16(da1l, dal);                          \
	da1h = vis_fpadd16(da1h, dah)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_1BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u8 *prow;
	mlib_u8 *pz_row;
	mlib_d64 da, dah, dal, da1h, da1l, dd, ddh, ddl, dd1h, dd1l;
	mlib_d64 *dprow, dr, drh, drl, dr1h, dr1l;
	mlib_f32 fzero = 0, fone = vis_to_float(0x100), scale05 =
	    vis_to_float(0x80808080);

/* initialize GSR scale factor */
	vis_write_gsr(6 << 3);

	if (!(((mlib_s32)pdst & 1) | (dst_stride & 7))) {
		mlib_s32 beg = ((8 - (mlib_addr)pdst) & 7) >> 1;

		if (beg > (src_width / 2))
			beg = src_width / 2;
		for (j = 0; j < beg; j++) {
			mlib_s32 a00 = psrc[j], a01 = psrc[j + 1];
			mlib_s32 a10, a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 = psrc[(i + 1) * src_stride + j];
				a11 = psrc[(i + 1) * src_stride + j + 1];
				pdst[2 * i * dst_stride + 2 * j] = a00;
				pdst[2 * i * dst_stride + 2 * j + 1] =
				    (a00 + a01) >> 1;
				pdst[(2 * i + 1) * dst_stride + 2 * j] =
				    (a00 + a10) >> 1;
				pdst[(2 * i + 1) * dst_stride + 2 * j + 1] =
				    (a00 + a01 + a10 + a11) >> 2;
				a00 = a10;
				a01 = a11;
			}
		}

		src_width -= 2 * beg;
		psrc += beg;
		pdst += 2 * beg;
	}

	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		for (j = 0; j <= (src_width / 2 - 8); j += 8) {
			prow = psrc + j;
			pz_row = pdst + 2 * j;

			GET_STR_U8_1_BILINEAR(0, da, dah, dal, da1h, da1l);

			prow += src_stride;

/* common case */
			if (((mlib_s32)pdst & 1) | (dst_stride & 7)) {
				if ((src_height / 2) > 0) {
					GET_STR_U8_1_BILINEAR_NF(0, dd, ddh,
					    ddl, dd1h, dd1l);
					BILINEAR_U8_14_PART1;
					drh =
					    vis_fpmerge(vis_read_hi(da),
					    vis_read_hi(dr));
					drl =
					    vis_fpmerge(vis_read_lo(da),
					    vis_read_lo(dr));
					STORE_RES_U8_COMMON_NF(pz_row, drh,
					    drl);
					BILINEAR_U8_14_PART2;
					drh =
					    vis_fpmerge(vis_read_hi(dr1h),
					    vis_read_hi(dr1l));
					drl =
					    vis_fpmerge(vis_read_lo(dr1h),
					    vis_read_lo(dr1l));
					pz_row += dst_stride;
					STORE_RES_U8_COMMON(pz_row, drh, drl);
					BILINEAR_U8_14_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
				for (i = 1; i < (src_height / 2); i++) {
					GET_STR_U8_1_BILINEAR_NF(0, dd, ddh,
					    ddl, dd1h, dd1l);
					BILINEAR_U8_14_PART1;
					drh =
					    vis_fpmerge(vis_read_hi(da),
					    vis_read_hi(dr));
					drl =
					    vis_fpmerge(vis_read_lo(da),
					    vis_read_lo(dr));
					STORE_RES_U8_COMMON(pz_row, drh, drl);
					BILINEAR_U8_14_PART2;
					drh =
					    vis_fpmerge(vis_read_hi(dr1h),
					    vis_read_hi(dr1l));
					drl =
					    vis_fpmerge(vis_read_lo(dr1h),
					    vis_read_lo(dr1l));
					pz_row += dst_stride;
					STORE_RES_U8_COMMON(pz_row, drh, drl);
					BILINEAR_U8_14_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			} else {
/* aligned case */
#pragma pipeloop(0)
				for (i = 0; i < (src_height / 2); i++) {
					GET_STR_U8_1_BILINEAR(0, dd, ddh, ddl,
					    dd1h, dd1l);
					BILINEAR_U8_14_PART1;
					drh =
					    vis_fpmerge(vis_read_hi(da),
					    vis_read_hi(dr));
					drl =
					    vis_fpmerge(vis_read_lo(da),
					    vis_read_lo(dr));
					((mlib_d64 *)pz_row)[0] = drh;
					((mlib_d64 *)pz_row)[1] = drl;
					BILINEAR_U8_14_PART2;
					drh =
					    vis_fpmerge(vis_read_hi(dr1h),
					    vis_read_hi(dr1l));
					drl =
					    vis_fpmerge(vis_read_lo(dr1h),
					    vis_read_lo(dr1l));
					pz_row += dst_stride;
					((mlib_d64 *)pz_row)[0] = drh;
					((mlib_d64 *)pz_row)[1] = drl;
					BILINEAR_U8_14_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			}
		}

		for (j = src_width / 16 * 8; j < (src_width / 2); j++) {
			mlib_s32 a00 = psrc[j], a01 = psrc[j + 1];

			for (i = 0; i < (src_height / 2); i++) {
				mlib_s32 a10 = psrc[(i + 1) * src_stride + j],
				    a11 = psrc[(i + 1) * src_stride + j + 1];
				pdst[2 * i * dst_stride + 2 * j] = a00;
				pdst[2 * i * dst_stride + 2 * j + 1] =
				    (a00 + a01) >> 1;
				pdst[(2 * i + 1) * dst_stride + 2 * j] =
				    (a00 + a10) >> 1;
				pdst[(2 * i + 1) * dst_stride + 2 * j + 1] =
				    (a00 + a01 + a10 + a11) >> 2;
				a00 = a10;
				a01 = a11;
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	BILINEAR_U8_2_PART1                                     \
	drl = vis_fmul8x16(scale05, da1l);                      \
	drh = vis_fmul8x16(scale05, da1h);                      \
	dr = vis_for(vis_fpack32(dal, dzero), drl);             \
	drh = vis_for(vis_fpack32(dah, dzero), drh);            \
	drl = vis_fpmerge(vis_read_lo(drh), vis_read_lo(dr));   \
	drh = vis_fpmerge(vis_read_hi(drh), vis_read_hi(dr))

/* *********************************************************** */

#define	BILINEAR_U8_2_PART2                                       \
	dr1l = vis_fpadd16(dal, ddl);                             \
	dr1h = vis_fpadd16(dah, ddh);                             \
	dr1l = vis_fmul8x16(scale05, dr1l);                       \
	dr1h = vis_fmul8x16(scale05, dr1h);                       \
	da1l = vis_fpadd16(da1l, dd1l);                           \
	da1h = vis_fpadd16(da1h, dd1h);                           \
	da1l = vis_fmul8x16(scale025, da1l);                      \
	da1h = vis_fmul8x16(scale025, da1h);                      \
	dr = vis_for(vis_fpack32(dr1l, dzero), da1l);             \
	dr1h = vis_for(vis_fpack32(dr1h, dzero), da1h);           \
	dr1l = vis_fpmerge(vis_read_lo(dr1h), vis_read_lo(dr));   \
	dr1h = vis_fpmerge(vis_read_hi(dr1h), vis_read_hi(dr))

/* *********************************************************** */

#define	BILINEAR_U8_2_TAIL                                      \
	da = dd;                                                \
	da1h = dd1h;                                            \
	da1l = dd1l;                                            \
	dal = ddl;                                              \
	dah = ddh

/* *********************************************************** */

#define	GET_STR_U8_2_BILINEAR(ind, da, dah, dal, da1h, da1l)    \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = dprow[0];                                          \
	da1h = dprow[1];                                        \
	da1l = dprow[2];                                        \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fand(da, mask_u8);                            \
	dah = vis_fmul8sux16(da, done);                         \
	dah = vis_fand(dah, mask_u8);                           \
	da1h = vis_faligndata(da1h, da1l);                      \
	VIS_ALIGNADDR(dprow, 2);                                \
	da1h = vis_faligndata(da, da1h);                        \
/* << 16 */                                                     \
	da1l = vis_fand(da1h, mask_u8);                         \
	da1h = vis_fmul8sux16(da1h, done);                      \
	da1h = vis_fand(da1h, mask_u8);                         \
	da1l = vis_fpadd16(da1l, dal);                          \
	da1h = vis_fpadd16(da1h, dah)

/* *********************************************************** */

#define	GET_STR_U8_2_BILINEAR_NF(ind, da, dah, dal, da1h, da1l) \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = vis_ld_d64_nf(dprow);                              \
	da1h = vis_ld_d64_nf(dprow + 1);                        \
	da1l = vis_ld_d64_nf(dprow + 2);                        \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fand(da, mask_u8);                            \
	dah = vis_fmul8sux16(da, done);                         \
	dah = vis_fand(dah, mask_u8);                           \
	da1h = vis_faligndata(da1h, da1l);                      \
	VIS_ALIGNADDR(dprow, 2);                                \
	da1h = vis_faligndata(da, da1h);                        \
/* << 16 */                                                     \
	da1l = vis_fand(da1h, mask_u8);                         \
	da1h = vis_fmul8sux16(da1h, done);                      \
	da1h = vis_fand(da1h, mask_u8);                         \
	da1l = vis_fpadd16(da1l, dal);                          \
	da1h = vis_fpadd16(da1h, dah)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_2BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u8 *prow;
	mlib_u8 *pz_row;
	mlib_d64 da, dah, dal, da1h, da1l, dd, ddh, ddl, dd1h, dd1l;
	mlib_d64 *dprow, dr, drh, drl, dr1h, dr1l;
	mlib_d64 done = vis_to_double_dup(0x1000100),
	    dzero = vis_fzero(), mask_u8 = vis_to_double_dup(0xff00ff);
	mlib_f32 scale025 = vis_to_float(0x40404040), scale05 =
	    vis_to_float(0x80808080);

	if (!((dst_stride & 7) | ((mlib_addr)pdst & 3))) {
		mlib_s32 beg = ((mlib_addr)pdst & 7) >> 2;

		if (beg > 0 && (src_width / 2) > 0) {
			mlib_s32 a00_1 = psrc[0], a00_2 = psrc[1], a01_1 =
			    psrc[2], a01_2 = psrc[3];
			for (i = 0; i < (src_height / 2); i++) {
				mlib_s32 a10_1 = psrc[(i + 1) * src_stride],
				    a10_2 = psrc[(i + 1) * src_stride + 1],
				    a11_1 =
				    psrc[(i + 1) * src_stride + 2], a11_2 =
				    psrc[(i + 1) * src_stride + 3];
				pdst[2 * i * dst_stride] = a00_1;
				pdst[2 * i * dst_stride + 2] =
				    (a00_1 + a01_1) >> 1;
				pdst[(2 * i + 1) * dst_stride] =
				    (a00_1 + a10_1) >> 1;
				pdst[(2 * i + 1) * dst_stride + 2] =
				    (a00_1 + a01_1 + a10_1 + a11_1) >> 2;
				pdst[2 * i * dst_stride + 1] = a00_2;
				pdst[2 * i * dst_stride + 3] =
				    (a00_2 + a01_2) >> 1;
				pdst[(2 * i + 1) * dst_stride + 1] =
				    (a00_2 + a10_2) >> 1;
				pdst[(2 * i + 1) * dst_stride + 3] =
				    (a00_2 + a01_2 + a10_2 + a11_2) >> 2;
				a00_1 = a10_1;
				a00_2 = a10_2;
				a01_1 = a11_1;
				a01_2 = a11_2;
			}

			src_width -= 2;
			psrc += 2;
			pdst += 4;
		}
	}

	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		for (j = 0; j <= ((src_width / 2) - 4); j += 4) {
			prow = psrc + 2 * j;
			pz_row = pdst + 4 * j;

			GET_STR_U8_2_BILINEAR(0, da, dah, dal, da1h, da1l);

			prow += src_stride;

/* common case */
			if (((mlib_s32)pdst | dst_stride) & 7) {
				if (src_height / 2 > 0) {
					GET_STR_U8_2_BILINEAR_NF(0, dd, ddh,
					    ddl, dd1h, dd1l);
					BILINEAR_U8_2_PART1;
					STORE_RES_U8_COMMON_NF(pz_row, drh,
						drl);

					BILINEAR_U8_2_PART2;
					pz_row += dst_stride;
					STORE_RES_U8_COMMON(pz_row, dr1h, dr1l);
					BILINEAR_U8_2_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
				for (i = 1; i < (src_height / 2); i++) {
					GET_STR_U8_2_BILINEAR_NF(0, dd, ddh,
					    ddl, dd1h, dd1l);
					BILINEAR_U8_2_PART1;
					STORE_RES_U8_COMMON(pz_row, drh, drl);

					BILINEAR_U8_2_PART2;
					pz_row += dst_stride;
					STORE_RES_U8_COMMON(pz_row, dr1h, dr1l);
					BILINEAR_U8_2_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			} else {
/* aligned case */
				for (i = 0; i < (src_height / 2); i++) {
					GET_STR_U8_2_BILINEAR(0, dd, ddh, ddl,
					    dd1h, dd1l);
					BILINEAR_U8_2_PART1;
					((mlib_d64 *)pz_row)[1] = drl;
					((mlib_d64 *)pz_row)[0] = drh;

					BILINEAR_U8_2_PART2;
					pz_row += dst_stride;
					((mlib_d64 *)pz_row)[1] = dr1l;
					((mlib_d64 *)pz_row)[0] = dr1h;
					BILINEAR_U8_2_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			}
		}

		for (j = src_width / 8 * 4; j < (src_width / 2); j++) {
			mlib_s32 a00_1 = psrc[2 * j],
			    a00_2 = psrc[2 * j + 1], a01_1 =
			    psrc[2 * j + 2], a01_2 = psrc[2 * j + 3];
			for (i = 0; i < (src_height / 2); i++) {
				mlib_s32 a10_1 =
				    psrc[(i + 1) * src_stride + 2 * j], a10_2 =
				    psrc[(i + 1) * src_stride + 2 * j + 1],
				    a11_1 =
				    psrc[(i + 1) * src_stride + 2 * j + 2],
				    a11_2 =
				    psrc[(i + 1) * src_stride + 2 * j + 3];
				pdst[2 * i * dst_stride + 4 * j] = a00_1;
				pdst[2 * i * dst_stride + 4 * j + 2] =
				    (a00_1 + a01_1) >> 1;
				pdst[(2 * i + 1) * dst_stride + 4 * j] =
				    (a00_1 + a10_1) >> 1;
				pdst[(2 * i + 1) * dst_stride + 4 * j + 2] =
				    (a00_1 + a01_1 + a10_1 + a11_1) >> 2;
				pdst[2 * i * dst_stride + 4 * j + 1] = a00_2;
				pdst[2 * i * dst_stride + 4 * j + 3] =
				    (a00_2 + a01_2) >> 1;
				pdst[(2 * i + 1) * dst_stride + 4 * j + 1] =
				    (a00_2 + a10_2) >> 1;
				pdst[(2 * i + 1) * dst_stride + 4 * j + 3] =
				    (a00_2 + a01_2 + a10_2 + a11_2) >> 2;
				a00_1 = a10_1;
				a00_2 = a10_2;
				a01_1 = a11_1;
				a01_2 = a11_2;
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	COMPOSE_U8_3(dy, dy1, dy1h, dy1l, drh, drm, drl)        \
	{                                                       \
	    mlib_d64 dxh, dxl, dx1h, dx1l;                      \
	                                                        \
	    (void *)VIS_ALIGNADDR(dprow, 1);                    \
	    drl = vis_faligndata(dy1h, dy1l);                   \
	    dxl = vis_faligndata(dy1, dy1);                     \
	    dx1h = vis_faligndata(dy1l, dy1l);                  \
	    (void *)VIS_ALIGNADDR(dprow, 6);                    \
	    dx1l = vis_faligndata(dy1h, dy1h);                  \
	    drm = vis_faligndata(dy, dy1);                      \
	    (void *)VIS_ALIGNADDR(dprow, 3);                    \
	    dy1l = vis_faligndata(dy, dy);                      \
	    drh = vis_faligndata(dy1l, dy1h);                   \
	    drm = vis_faligndata(dx1l, drm);                    \
	    dxh = vis_faligndata(dy1l, dy1l);                   \
	    drl = vis_faligndata(drl, dxl);                     \
	    drl = vis_faligndata(drl, dx1h);                    \
	    (void *)VIS_ALIGNADDR(dprow, 2);                    \
	    drh = vis_faligndata(drh, dy1l);                    \
	    drm = vis_faligndata(drm, dx1l);                    \
	    (void *)VIS_ALIGNADDR(dprow, 7);                    \
	    drm = vis_faligndata(dxh, drm);                     \
	}

/* *********************************************************** */

#define	BILINEAR_U8_3_PART1                                     \
	{                                                       \
	    type_union_mlib_d64 dun;                            \
	                                                        \
	    dun.two_float.fl0 = vis_fpack16(da1l);              \
	    dr = vis_fpack16_pair(da1h, da1m);                  \
	    COMPOSE_U8_3(da, da1, dr, dun.db, drh, drm, drl);   \
	}

/* *********************************************************** */

#define	BILINEAR_U8_3_PART2                                             \
	{                                                               \
	    type_union_mlib_d64 dun, dun1;                              \
	                                                                \
	    dr1l = vis_fpadd16(dal, ddl);                               \
	    dr1h = vis_fpadd16(dah, ddh);                               \
	    dr1m = vis_fpadd16(dam, ddm);                               \
	    dun.two_float.fl0 = vis_fpack16(dr1l);                      \
	    dr1h = vis_fpack16_pair(dr1h, dr1m);                        \
	    da1l = vis_fpadd16(da1l, dd1l);                             \
	    da1h = vis_fpadd16(da1h, dd1h);                             \
	    da1m = vis_fpadd16(da1m, dd1m);                             \
	    da1l = vis_fmul8x16(scale05, da1l);                         \
	    da1h = vis_fmul8x16(scale05, da1h);                         \
	    da1m = vis_fmul8x16(scale05, da1m);                         \
	    dun1.two_float.fl0 = vis_fpack16(da1l);                     \
	    dr1m = vis_fpack16_pair(da1h, da1m);                        \
	    COMPOSE_U8_3(dr1h, dun.db, dr1m, dun1.db, drh, drm, drl);   \
	}

/* *********************************************************** */

#define	BILINEAR_U8_3_TAIL                                      \
	da1 = dd1;                                              \
	da = dd;                                                \
	da1l = dd1l;                                            \
	da1h = dd1h;                                            \
	da1m = dd1m;                                            \
	dal = vis_fmul8x16al(vis_read_hi(da1), fone);           \
	dah = vis_fmul8x16al(vis_read_hi(da), fone);            \
	dam = vis_fmul8x16al(vis_read_lo(da), fone)

/* *********************************************************** */

#define	GET_STR_U8_3_BILINEAR(ind, da, da1, dah, dam, dal,      \
	da1h, da1m, da1l)                                       \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da1h = dprow[1];                                        \
	da1l = dprow[2];                                        \
	da = dprow[0];                                          \
	da1 = vis_faligndata(da1h, da1l);                       \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fmul8x16al(vis_read_hi(da1), fone);           \
	dah = vis_fmul8x16al(vis_read_hi(da), fone);            \
	dam = vis_fmul8x16al(vis_read_lo(da), fone);            \
	VIS_ALIGNADDR(dprow, 3);                                \
	da1l = vis_faligndata(da1, da1);                        \
	da1m = vis_faligndata(da, da1);                         \
/* << 24 */                                                     \
	da1l = vis_fmul8x16al(vis_read_hi(da1l), fone);         \
	da1h = vis_fmul8x16al(vis_read_hi(da1m), fone);         \
	da1m = vis_fmul8x16al(vis_read_lo(da1m), fone);         \
	da1l = vis_fpadd16(da1l, dal);                          \
	da1h = vis_fpadd16(da1h, dah);                          \
	da1m = vis_fpadd16(da1m, dam)

/* *********************************************************** */

#define	GET_STR_U8_3_BILINEAR_NF(ind, da, da1, dah, dam, dal,   \
	da1h, da1m, da1l)                                       \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da1h = vis_ld_d64_nf(dprow + 1);                        \
	da1l = vis_ld_d64_nf(dprow + 2);                        \
	da = vis_ld_d64_nf(dprow);                              \
	da1 = vis_faligndata(da1h, da1l);                       \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fmul8x16al(vis_read_hi(da1), fone);           \
	dah = vis_fmul8x16al(vis_read_hi(da), fone);            \
	dam = vis_fmul8x16al(vis_read_lo(da), fone);            \
	VIS_ALIGNADDR(dprow, 3);                                \
	da1l = vis_faligndata(da1, da1);                        \
	da1m = vis_faligndata(da, da1);                         \
/* << 24 */                                                     \
	da1l = vis_fmul8x16al(vis_read_hi(da1l), fone);         \
	da1h = vis_fmul8x16al(vis_read_hi(da1m), fone);         \
	da1m = vis_fmul8x16al(vis_read_lo(da1m), fone);         \
	da1l = vis_fpadd16(da1l, dal);                          \
	da1h = vis_fpadd16(da1h, dah);                          \
	da1m = vis_fpadd16(da1m, dam)

/* *********************************************************** */

#define	STORE_RES_U8_3_COMMON(pointer, res1, res2, res3)              \
	{                                                             \
	    mlib_d64 *dpz = (mlib_d64 *)VIS_ALIGNADDR(pointer, -1);   \
	                                                              \
	    emask = 0xff >> ((mlib_addr)pointer - (mlib_addr)dpz);    \
	    (void *)VIS_ALIGNADDRL(pointer, 0);                       \
	    vis_pst_8(vis_faligndata(res1, res1), dpz, emask);        \
	    dpz[1] = vis_faligndata(res1, res2);                      \
	    dpz[2] = vis_faligndata(res2, res3);                      \
	    vis_pst_8(vis_faligndata(res3, res3), dpz + 3, ~emask);   \
	}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_3BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, l;
	mlib_s32 num_of_rows, shag_row;
	mlib_u8 *pz_row;
	const mlib_u8 *prow;
	mlib_d64 da, da1, dah, dam, dal, da1h, da1m, da1l;
	mlib_d64 dd, dd1, ddh, ddm, ddl, dd1h, dd1m, dd1l;
	mlib_d64 *dprow, dr, drh, drm, drl, dr1h, dr1m, dr1l;
	mlib_f32 fone = vis_to_float(0x100), scale05 = vis_to_float(0x80808080);

	if (((dst_stride & 7) | ((mlib_addr)pdst & 1)) == 0) {
		mlib_s32 beg = ((mlib_addr)pdst & 7) >> 1;

		if (beg > (src_width / 2))
			beg = src_width / 2;
		for (j = 0; j < beg; j++) {
			for (l = 0; l < 3; l++) {
				mlib_s32 a00 = psrc[3 * j + l], a10;
				mlib_s32 a01 = psrc[3 * j + 3 + l], a11;

				for (i = 0; i < (src_height / 2); i++) {
					a10 = psrc[(i + 1) * src_stride +
					    3 * j + l];
					a11 = psrc[(i + 1) * src_stride +
					    3 * j + 3 + l];
					pdst[2 * i * dst_stride + 6 * j + l] =
					    a00;
					pdst[2 * i * dst_stride + 6 * j + 3 +
					    l] = (a00 + a01) >> 1;
					pdst[(2 * i + 1) * dst_stride + 6 * j +
					    l] = (a00 + a10) >> 1;
					pdst[(2 * i + 1) * dst_stride + 6 * j +
					    3 + l] =
					    (a00 + a01 + a10 + a11) >> 2;
					a00 = a10;
					a01 = a11;
				}
			}
		}

		src_width -= 2 * beg;
		psrc += 3 * beg;
		pdst += 6 * beg;
/* initialize GSR scale factor */
		vis_write_gsr(6 << 3);

		num_of_rows = src_height / 2;
		SHAG_STR(mlib_u8);

		while (num_of_rows > 0) {
			src_height =
			    2 * (num_of_rows >
			    shag_row ? shag_row : num_of_rows);
			num_of_rows -= src_height / 2;

			for (j = 0; j <= ((src_width / 2) - 4); j += 4) {
				prow = psrc + 3 * j;
				pz_row = pdst + 6 * j;

				GET_STR_U8_3_BILINEAR(0, da, da1, dah, dam, dal,
				    da1h, da1m, da1l);

				prow += src_stride;
				GET_STR_U8_3_BILINEAR_NF(0, dd, dd1, ddh, ddm,
				    ddl, dd1h, dd1m, dd1l);
				BILINEAR_U8_3_PART1;

#pragma pipeloop(0)
				for (i = 0; i < (src_height / 2); i++) {
					((mlib_d64 *)pz_row)[2] = drl;
					((mlib_d64 *)pz_row)[0] = drh;
					((mlib_d64 *)pz_row)[1] = drm;
					prow += src_stride;

					BILINEAR_U8_3_PART2;
					pz_row += dst_stride;
					((mlib_d64 *)pz_row)[2] = drl;
					((mlib_d64 *)pz_row)[0] = drh;
					((mlib_d64 *)pz_row)[1] = drm;
					BILINEAR_U8_3_TAIL;
					GET_STR_U8_3_BILINEAR_NF(0, dd, dd1,
					    ddh, ddm, ddl, dd1h, dd1m, dd1l);
					BILINEAR_U8_3_PART1;
					pz_row += dst_stride;
				}
			}

			for (j = src_width / 8 * 4; j < (src_width / 2); j++) {
				for (l = 0; l < 3; l++) {
					mlib_s32 a00 = psrc[3 * j + l], a10;
					mlib_s32 a01 = psrc[3 * j + 3 + l], a11;

					for (i = 0; i < (src_height / 2); i++) {
						a10 = psrc[(i + 1) *
						    src_stride + 3 * j + l];
						a11 = psrc[(i + 1) *
						    src_stride + 3 * j + 3 + l];
						pdst[2 * i * dst_stride +
						    6 * j + l] = a00;
						pdst[2 * i * dst_stride +
						    6 * j + 3 + l] =
						    (a00 + a01) >> 1;
						pdst[(2 * i + 1) * dst_stride +
						    6 * j + l] =
						    (a00 + a10) >> 1;
						pdst[(2 * i + 1) * dst_stride +
						    6 * j + 3 + l] =
						    (a00 + a01 + a10 +
						    a11) >> 2;
						a00 = a10;
						a01 = a11;
					}
				}
			}

			psrc += src_height / 2 * src_stride;
			pdst += src_height / 2 * 2 * dst_stride;
		}
	} else {
		for (i = 0; i < (src_height / 2); i++) {
			mlib_s32 a00_1 = psrc[i * src_stride],
			    a10_1 = psrc[(i + 1) * src_stride],
			    a00_2 = psrc[i * src_stride + 1],
			    a10_2 = psrc[(i + 1) * src_stride + 1],
			    a00_3 = psrc[i * src_stride + 2],
			    a10_3 = psrc[(i + 1) * src_stride + 2];
			for (j = 0; j < (src_width / 2); j++) {
				mlib_s32 a01_1 =
				    psrc[i * src_stride + 3 * j + 3], a11_1 =
				    psrc[(i + 1) * src_stride + 3 * j + 3],
				    a01_2 =
				    psrc[i * src_stride + 3 * j + 4], a11_2 =
				    psrc[(i + 1) * src_stride + 3 * j + 4],
				    a01_3 =
				    psrc[i * src_stride + 3 * j + 5], a11_3 =
				    psrc[(i + 1) * src_stride + 3 * j + 5];
				pdst[2 * i * dst_stride + 6 * j] = a00_1;
				pdst[2 * i * dst_stride + 6 * j + 3] =
				    (a00_1 + a01_1) >> 1;
				pdst[(2 * i + 1) * dst_stride + 6 * j] =
				    (a00_1 + a10_1) >> 1;
				pdst[(2 * i + 1) * dst_stride + 6 * j + 3] =
				    (a00_1 + a01_1 + a10_1 + a11_1) >> 2;
				pdst[2 * i * dst_stride + 6 * j + 1] = a00_2;
				pdst[2 * i * dst_stride + 6 * j + 4] =
				    (a00_2 + a01_2) >> 1;
				pdst[(2 * i + 1) * dst_stride + 6 * j + 1] =
				    (a00_2 + a10_2) >> 1;
				pdst[(2 * i + 1) * dst_stride + 6 * j + 4] =
				    (a00_2 + a01_2 + a10_2 + a11_2) >> 2;
				pdst[2 * i * dst_stride + 6 * j + 2] = a00_3;
				pdst[2 * i * dst_stride + 6 * j + 5] =
				    (a00_3 + a01_3) >> 1;
				pdst[(2 * i + 1) * dst_stride + 6 * j + 2] =
				    (a00_3 + a10_3) >> 1;
				pdst[(2 * i + 1) * dst_stride + 6 * j + 5] =
				    (a00_3 + a01_3 + a10_3 + a11_3) >> 2;
				a00_1 = a01_1;
				a00_2 = a01_2;
				a00_3 = a01_3;
				a10_1 = a11_1;
				a10_2 = a11_2;
				a10_3 = a11_3;
			}
		}
	}
}

/* *********************************************************** */

#define	GET_STR_U8_4_BILINEAR(ind, da, dah, dal, da1h, da1l)    \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = dprow[0];                                          \
	da1h = dprow[1];                                        \
	da1l = dprow[2];                                        \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fpmerge(fzero, vis_read_lo(da));              \
	dah = vis_fpmerge(fzero, vis_read_hi(da));              \
	da1l = vis_faligndata(da1h, da1l);                      \
	da1l = vis_fpmerge(fzero, vis_read_hi(da1l));           \
	da1h = vis_fpadd16(dal, dah);                           \
	da1l = vis_fpadd16(da1l, dal)

/* *********************************************************** */

#define	GET_STR_U8_4_BILINEAR_NF(ind, da, dah, dal, da1h, da1l) \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = vis_ld_d64_nf(dprow);                              \
	da1h = vis_ld_d64_nf(dprow + 1);                        \
	da1l = vis_ld_d64_nf(dprow + 2);                        \
	da = vis_faligndata(da, da1h);                          \
	dal = vis_fpmerge(fzero, vis_read_lo(da));              \
	dah = vis_fpmerge(fzero, vis_read_hi(da));              \
	da1l = vis_faligndata(da1h, da1l);                      \
	da1l = vis_fpmerge(fzero, vis_read_hi(da1l));           \
	da1h = vis_fpadd16(dal, dah);                           \
	da1l = vis_fpadd16(da1l, dal)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_4BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u8))
{
/* indices */
	mlib_s32 i, j, l;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u8 *prow;
	mlib_u8 *pz_row;
	mlib_d64 da, dah, dal, da1h, da1l, dd, ddh, ddl, dd1h, dd1l;
	mlib_d64 *dprow, dr, drh, drl, dr1h, dr1l;
	mlib_f32 fzero = 0, fone = vis_to_float(0x100), scale05 =
	    vis_to_float(0x80808080);

/* initialize GSR scale factor */
	vis_write_gsr(6 << 3);
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u8);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		for (j = 0; j <= ((src_width / 2) - 2); j += 2) {
			prow = psrc + 4 * j;
			pz_row = pdst + 8 * j;

			GET_STR_U8_4_BILINEAR(0, da, dah, dal, da1h, da1l);

			prow += src_stride;

/* common case */
			if (((mlib_s32)pdst | dst_stride) & 3) {

				mlib_d64 dr2h, dr2l;

				GET_STR_U8_4_BILINEAR_NF(0, dd, ddh, ddl, dd1h,
				    dd1l);
				BILINEAR_U8_14_PART1;
				drh =
				    vis_freg_pair(vis_read_hi(da),
				    vis_read_hi(dr));
				drl =
				    vis_freg_pair(vis_read_lo(da),
				    vis_read_lo(dr));
				BILINEAR_U8_14_PART2;
				dr2h =
				    vis_freg_pair(vis_read_hi(dr1h),
				    vis_read_hi(dr1l));
				dr2l =
				    vis_freg_pair(vis_read_lo(dr1h),
				    vis_read_lo(dr1l));

				for (i = 0; i < (src_height / 2); i++) {
					STORE_RES_U8_COMMON(pz_row, drh, drl);
					pz_row += dst_stride;
					STORE_RES_U8_COMMON(pz_row, dr2h, dr2l);
					BILINEAR_U8_14_TAIL;
					prow += src_stride;
					pz_row += dst_stride;

					GET_STR_U8_4_BILINEAR_NF(0, dd, ddh,
					    ddl, dd1h, dd1l);
					BILINEAR_U8_14_PART1;
					drh =
					    vis_freg_pair(vis_read_hi(da),
					    vis_read_hi(dr));
					drl =
					    vis_freg_pair(vis_read_lo(da),
					    vis_read_lo(dr));
					BILINEAR_U8_14_PART2;
					dr2h =
					    vis_freg_pair(vis_read_hi(dr1h),
					    vis_read_hi(dr1l));
					dr2l =
					    vis_freg_pair(vis_read_lo(dr1h),
					    vis_read_lo(dr1l));

/*
 * compiler bug  ->  bad performance
 *
 *        GET_STR_U8_4_BILINEAR(0, dd, ddh, ddl, dd1h, dd1l);
 *        BILINEAR_U8_14_PART1;
 *        drh = vis_freg_pair(vis_read_hi(da), vis_read_hi(dr));
 *        drl = vis_freg_pair(vis_read_lo(da), vis_read_lo(dr));
 *        STORE_RES_U8_COMMON(pz_row, drh, drl);
 *
 *        BILINEAR_U8_14_PART2;
 *        drh = vis_freg_pair(vis_read_hi(dr1h), vis_read_hi(dr1l));
 *        drl = vis_freg_pair(vis_read_lo(dr1h), vis_read_lo(dr1l));
 *        pz_row += dst_stride;
 *        STORE_RES_U8_COMMON(pz_row, drh, drl);
 *        BILINEAR_U8_14_TAIL;
 *        prow += src_stride;
 *        pz_row += dst_stride;
 */
				}
			} else {
/* aligned case */
				for (i = 0; i < (src_height / 2); i++) {
					GET_STR_U8_4_BILINEAR_NF(0, dd, ddh,
					    ddl, dd1h, dd1l);
					BILINEAR_U8_14_PART1;
					((mlib_f32 *)pz_row)[0] =
					    vis_read_hi(da);
					((mlib_f32 *)pz_row)[2] =
					    vis_read_lo(da);
					((mlib_f32 *)pz_row)[1] =
					    vis_read_hi(dr);
					((mlib_f32 *)pz_row)[3] =
					    vis_read_lo(dr);

					BILINEAR_U8_14_PART2;
					pz_row += dst_stride;
					((mlib_f32 *)pz_row)[0] =
					    vis_read_hi(dr1h);
					((mlib_f32 *)pz_row)[2] =
					    vis_read_lo(dr1h);
					((mlib_f32 *)pz_row)[1] =
					    vis_read_hi(dr1l);
					((mlib_f32 *)pz_row)[3] =
					    vis_read_lo(dr1l);
					BILINEAR_U8_14_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			}
		}

		if ((j = src_width / 4 * 2) < (src_width / 2)) {
			for (l = 0; l < 4; l++) {
				mlib_s32 a00 = psrc[4 * j + l], a10;
				mlib_s32 a01 = psrc[4 * j + 4 + l], a11;

				for (i = 0; i < (src_height / 2); i++) {
					a10 = psrc[(i + 1) * src_stride +
					    4 * j + l];
					a11 = psrc[(i + 1) * src_stride +
					    4 * j + 4 + l];
					pdst[2 * i * dst_stride + 8 * j + l] =
					    a00;
					pdst[2 * i * dst_stride + 8 * j + 4 +
					    l] = (a00 + a01) >> 1;
					pdst[(2 * i + 1) * dst_stride + 8 * j +
					    l] = (a00 + a10) >> 1;
					pdst[(2 * i + 1) * dst_stride + 8 * j +
					    4 + l] =
					    (a00 + a01 + a10 + a11) >> 2;
					a00 = a10;
					a01 = a11;
				}
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k;

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k],
				a10 = psrc[src_stride + k];
			mlib_s32 l = k + channels;
			mlib_s32 a01, a11;

			if (j = src_w_beg & 1) {
				a01 = psrc[l];
				a11 = psrc[src_stride + l];
				pdst[k] = MLIB_ZOOMIN2X_DR_FL0;
				a00 = a01;
				a10 = a11;
			}

			for (j++; j < src_width; j += 2) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL0;
				pdst[j * channels + k] = MLIB_ZOOMIN2X_DR_FL0;
				a00 = a01;
				a10 = a11;
			}

			if (j == src_width) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL0;
			}
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k], a10;
			mlib_s32 l = k + channels;
			mlib_s32 a01 = psrc[l], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL0;
				pdst[(2 * i + 1) * dst_stride + k] =
				    MLIB_ZOOMIN2X_DR_FL0;
				a00 = a10;
				a01 = a11;
			}

			if (src_height & 1) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL0;
			}
		}

		psrc += channels;
		pdst += channels;
		src_width--;
	}

	i = src_height / 2;
	j = src_width / 2;

	if (i && j)
		switch (channels) {
		case 1:
			mlib_v_ImageZoomIn2X_U8_1BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 2:
			mlib_v_ImageZoomIn2X_U8_2BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 3:
			mlib_v_ImageZoomIn2X_U8_3BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 4:
			mlib_v_ImageZoomIn2X_U8_4BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		}

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[i * src_stride + k],
				a10 = psrc[(i + 1) * src_stride + k];
			mlib_s32 l = k + channels;
			mlib_s32 a01, a11;

			for (j = 0; j < (src_width / 2); j++) {
				a01 = psrc[i * src_stride + j * channels + l];
				a11 = psrc[(i + 1) * src_stride +
				    j * channels + l];
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] = MLIB_ZOOMIN2X_UL_FL0;
				pdst[2 * i * dst_stride + 2 * j * channels +
				    l] = MLIB_ZOOMIN2X_UR_FL0;
				a00 = a01;
				a10 = a11;
			}
		}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[j * channels + k], a10;
			mlib_s32 l = k + channels;
			mlib_s32 a01 = psrc[j * channels + l], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				a11 =
				    psrc[(i + 1) * src_stride + j * channels +
				    l];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL0;
				pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_DL_FL0;
				a00 = a10;
				a01 = a11;
			}

			if (src_height & 1) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				a11 =
				    psrc[(i + 1) * src_stride + j * channels +
				    l];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL0;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k;

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k],
				a10 = psrc[src_stride + k];
			mlib_s32 l = k + channels;
			mlib_s32 a01, a11;

			if (j = src_w_beg & 1) {
				a01 = psrc[l];
				a11 = psrc[src_stride + l];
				pdst[k] = MLIB_ZOOMIN2X_DR_FL1;
				a00 = a01;
				a10 = a11;
			}

			for (j++; j < src_width; j += 2) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL1;
				pdst[j * channels + k] = MLIB_ZOOMIN2X_DR_FL1;
				a00 = a01;
				a10 = a11;
			}

			if (j == src_width) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL1;
			}
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k], a10;
			mlib_s32 l = k + channels;
			mlib_s32 a01 = psrc[l], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL1;
				pdst[(2 * i + 1) * dst_stride + k] =
				    MLIB_ZOOMIN2X_DR_FL1;
				a00 = a10;
				a01 = a11;
			}

			if (src_height & 1) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL1;
			}
		}

		psrc += channels;
		pdst += channels;
		src_width--;
	}

	i = src_height / 2;
	j = src_width / 2;

	if (i && j)
		switch (channels) {
		case 1:
			mlib_v_ImageZoomIn2X_U8_1BL_FL1
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 2:
			mlib_v_ImageZoomIn2X_U8_2BL_FL1
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 3:
			mlib_v_ImageZoomIn2X_U8_3BL_FL1
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 4:
			mlib_v_ImageZoomIn2X_U8_4BL_FL1
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		}

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[i * src_stride + k], a01;
			mlib_s32 l = k + channels;

			for (j = 0; j < (src_width / 2); j++) {
				a01 = psrc[i * src_stride + j * channels + l];
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] = MLIB_ZOOMIN2X_UL_FL1;
				pdst[2 * i * dst_stride + 2 * j * channels +
				    l] = MLIB_ZOOMIN2X_UR_FL1;
				a00 = a01;
			}
		}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[j * channels + k], a10;
			mlib_s32 l = k + channels;
			mlib_s32 a01 = psrc[j * channels + l], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				a11 =
				    psrc[(i + 1) * src_stride + j * channels +
				    l];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL1;
				pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_DL_FL1;
				a00 = a10;
				a01 = a11;
			}

			if (src_height & 1) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				a11 =
				    psrc[(i + 1) * src_stride + j * channels +
				    l];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL1;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k;

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k],
				a10 = psrc[src_stride + k];
			mlib_s32 l = k + channels;
			mlib_s32 a01, a11;

			if (j = src_w_beg & 1) {
				a01 = psrc[l];
				a11 = psrc[src_stride + l];
				pdst[k] = MLIB_ZOOMIN2X_DR_FL2;
				a00 = a01;
				a10 = a11;
			}

			for (j++; j < src_width; j += 2) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL2;
				pdst[j * channels + k] = MLIB_ZOOMIN2X_DR_FL2;
				a00 = a01;
				a10 = a11;
			}

			if (j == src_width) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL2;
			}
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k], a10;
			mlib_s32 l = k + channels;
			mlib_s32 a01 = psrc[l], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL2;
				pdst[(2 * i + 1) * dst_stride + k] =
				    MLIB_ZOOMIN2X_DR_FL2;
				a00 = a10;
				a01 = a11;
			}

			if (src_height & 1) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL2;
			}
		}

		psrc += channels;
		pdst += channels;
		src_width--;
	}

	i = src_height / 2;
	j = src_width / 2;

	if (i && j)
		switch (channels) {
		case 1:
			mlib_v_ImageZoomIn2X_U8_1BL_FL2
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 2:
			mlib_v_ImageZoomIn2X_U8_2BL_FL2
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 3:
			mlib_v_ImageZoomIn2X_U8_3BL_FL2
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		case 4:
			mlib_v_ImageZoomIn2X_U8_4BL_FL2
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u8));
			break;
		}

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[i * src_stride + k],
				a10 = psrc[(i + 1) * src_stride + k];
			mlib_s32 l = k + channels;
			mlib_s32 a01, a11;

			for (j = 0; j < (src_width / 2); j++) {
				a01 = psrc[i * src_stride + j * channels + l];
				a11 = psrc[(i + 1) * src_stride + j * channels +
				    l];
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] = MLIB_ZOOMIN2X_UL_FL2;
				pdst[2 * i * dst_stride + 2 * j * channels +
				    l] = MLIB_ZOOMIN2X_UR_FL2;
				a00 = a01;
				a10 = a11;
			}
		}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[j * channels + k];
			mlib_s32 a10;

			for (i = 0; i < (src_height / 2); i++) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL2;
				pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_DL_FL2;
				a00 = a10;
			}

			if (src_height & 1) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL2;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U8_BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k;

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k], a10 = psrc[src_stride + k];
			mlib_s32 l = k + channels;
			mlib_s32 a01, a11;

			if (j = src_w_beg & 1) {
				a01 = psrc[l];
				a11 = psrc[src_stride + l];
				pdst[k] = MLIB_ZOOMIN2X_DR_FL3;
				a00 = a01;
				a10 = a11;
			}

			for (j++; j < src_width; j += 2) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL3;
				pdst[j * channels + k] = MLIB_ZOOMIN2X_DR_FL3;
				a00 = a01;
				a10 = a11;
			}

			if (j == src_width) {
				a01 = psrc[(j >> 1) * channels + l];
				a11 =
				    psrc[src_stride + (j >> 1) * channels + l];
				pdst[j * channels - channels + k] =
				    MLIB_ZOOMIN2X_DL_FL3;
			}
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[k], a10;
			mlib_s32 l = k + channels;
			mlib_s32 a01 = psrc[l], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL3;
				pdst[(2 * i + 1) * dst_stride + k] =
				    MLIB_ZOOMIN2X_DR_FL3;
				a00 = a10;
				a01 = a11;
			}

			if (src_height & 1) {
				a10 = psrc[(i + 1) * src_stride + k];
				a11 = psrc[(i + 1) * src_stride + l];
				pdst[2 * i * dst_stride + k] =
				    MLIB_ZOOMIN2X_UR_FL3;
			}
		}

		psrc += channels;
		pdst += channels;
		src_width--;
	}

	switch (channels) {
	case 1:
		mlib_v_ImageZoomIn2X_U8_1BL_FL3(MLIB_ZOOMIN2XCALLPARAM_BL
		    (mlib_u8));
		break;
	case 2:
		mlib_v_ImageZoomIn2X_U8_2BL_FL3(MLIB_ZOOMIN2XCALLPARAM_BL
		    (mlib_u8));
		break;
	case 3:
		mlib_v_ImageZoomIn2X_U8_3BL_FL3(MLIB_ZOOMIN2XCALLPARAM_BL
		    (mlib_u8));
		break;
	case 4:
		mlib_v_ImageZoomIn2X_U8_4BL_FL3(MLIB_ZOOMIN2XCALLPARAM_BL
		    (mlib_u8));
		break;
	}

	i = src_height / 2;
	j = src_width / 2;

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[i * src_stride + k], a01;
			mlib_s32 l = k + channels;

			for (j = 0; j < (src_width / 2); j++) {
				a01 = psrc[i * src_stride + j * channels + l];
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] = MLIB_ZOOMIN2X_UL_FL3;
				pdst[2 * i * dst_stride + 2 * j * channels +
				    l] = MLIB_ZOOMIN2X_UR_FL3;
				a00 = a01;
			}
		}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++) {
			mlib_s32 a00 = psrc[j * channels + k];
			mlib_s32 a10;

			for (i = 0; i < (src_height / 2); i++) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL3;
				pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_DL_FL3;
				a00 = a10;
			}

			if (src_height & 1) {
				a10 =
				    psrc[(i + 1) * src_stride + j * channels +
				    k];
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    MLIB_ZOOMIN2X_UL_FL3;
			}
		}
	}
}

#include <mlib_v_ImageZoomIn2X.h>

/* *********************************************************** */

#define	STYPE	mlib_u8

#define	FTYPE	mlib_f32

/* *********************************************************** */

#define	DEF_CONST_V0() mlib_d64 q = vis_to_double(0x3000, 0x1000)

/* *********************************************************** */

#define	DEF_CONST_V1() mlib_d64 q = vis_to_double(0x4000, 0x2000)

/* *********************************************************** */

#define	DEF_CONST_H0()                                          \
	mlib_d64 g = vis_to_double(0xC0C0C0C0, 0x40404040)

/* *********************************************************** */

#define	DEF_CONST_H1() mlib_d64 g = vis_to_double_dup(0x80808080)

/* *********************************************************** */

#define	SUMV_A0(a, b)                                                      \
	vis_fpadd16(vis_fmul8x16al(a, vis_read_hi(q)), vis_fmul8x16al(b,   \
	    vis_read_lo(q)))

/* *********************************************************** */

#define	SUMV_B0(a, b)                                                      \
	vis_fpadd16(vis_fmul8x16al(a, vis_read_lo(q)), vis_fmul8x16al(b,   \
	    vis_read_hi(q)))

/* *********************************************************** */

#define	SUMV_A1(a, b)	vis_fmul8x16al(a, vis_read_hi(q))

/* *********************************************************** */

#define	SUMV_B1(a, b)                                                      \
	vis_fpadd16(vis_fmul8x16al(a, vis_read_lo(q)), vis_fmul8x16al(b,   \
	    vis_read_lo(q)))

/* *********************************************************** */

#define	SUMH_A0(a, b)                                           \
	vis_fpadd16(vis_fmul8x16(vis_read_hi(g), a),            \
	    vis_fmul8x16(vis_read_lo(g), b))

/* *********************************************************** */

#define	SUMH_B0(a, b)                                           \
	vis_fpadd16(vis_fmul8x16(vis_read_lo(g), a),            \
	    vis_fmul8x16(vis_read_hi(g), b))

#define	SUMH_A1(a, b)	(a)

/* *********************************************************** */

#define	SUMH_B1(a, b)                                           \
	vis_fpadd16(vis_fmul8x16(vis_read_hi(g), a),            \
	    vis_fmul8x16(vis_read_lo(g), b))

/* *********************************************************** */

FUNC_ZOOM_BL(U8, 2, 0, 0)
    FUNC_ZOOM_BL(U8, 2, 0, 1)
    FUNC_ZOOM_BL(U8, 2, 1, 0)
    FUNC_ZOOM_BL(U8, 4, 0, 0)
    FUNC_ZOOM_BL(U8, 4, 1, 0)

/* *********************************************************** */
