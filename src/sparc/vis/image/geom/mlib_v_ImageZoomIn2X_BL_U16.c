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

#pragma ident	"@(#)mlib_v_ImageZoomIn2X_BL_U16.c	9.3	07/11/05 SMI"

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

#include <mlib_image.h>
#include <vis_proto.h>

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
	    vis_pst_8(vis_faligndata(r0, r0), pd, emask0 & 0xFF);       \
	    if (emask1 & 0xFF)                                          \
		vis_pst_8(vis_faligndata(r0, r1), pd + 1,               \
		    emask1 & 0xFF);                                     \
	    if (emask2 & 0xFF)                                          \
		vis_pst_8(vis_faligndata(r1, r1), pd + 2,               \
		    emask2 & 0xFF);                                     \
	}

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

#define	LOAD_1U16A(l0, l1, psrc, index)	LOAD_1U8A(l0, l1, psrc, index)
#define	STORE_1U16A(pdst, index, r0, r1)	STORE_1U8A(pdst, index, r0, r1)

#define	LOAD_1U16G(l0, l1, psrc, index)	LOAD_1U8G(l0, l1, psrc, 2*(index))

#define	LOAD_1U16G_NF(l0, l1, psrc, index)                                   \
	LOAD_1U8G_NF(l0, l1, psrc, 2*(index))

#define	STORE_1U16G(pdst, index, r0, r1)	STORE_1U8G(pdst, index, r0, r1)

#define	STORE_1U16G_NF(pdst, index, r0, r1)                                  \
	STORE_1U8G_NF(pdst, index, r0, r1)

#define	STORE_1U16G_LAST(pdst, index, r0, r1)	STORE_1U8G_LAST(pdst, index, \
						r0, r1)

/* *********************************************************** */

#define	LOAD_4U16A(l0, l1, psrc, index)                         \
	{                                                       \
	    mlib_f32 *ps = (mlib_f32 *)(psrc + index);          \
	                                                        \
	    l0 = vis_freg_pair(ps[0], ps[1]);                   \
	    l1 = vis_freg_pair(ps[2], ps[3]);                   \
	}

/* *********************************************************** */

#define	LOAD_4U16G(l0, l1, psrc, index)                         \
	{                                                       \
	    mlib_d64 *ps = VIS_ALIGNADDR(psrc, 2 * (index));    \
	                                                        \
	    l0 = ps[0];                                         \
	    l1 = ps[1];                                         \
	    l0 = vis_faligndata(l0, l1);                        \
	    l1 = vis_faligndata(l1, ps[2]);                     \
	}

/* *********************************************************** */

#define	LOAD_4U16G_NF(l0, l1, psrc, index)                      \
	{                                                       \
	    mlib_d64 *ps = VIS_ALIGNADDR(psrc, 2 * (index));    \
	                                                        \
	    l0 = ps[0];                                         \
	    l1 = ps[1];                                         \
	    l0 = vis_faligndata(l0, l1);                        \
	    l1 = vis_faligndata(l1, vis_ld_d64_nf(ps + 2));     \
	}

/* *********************************************************** */

#define	MOVE_1U16_FL0                                           \
	w_old_13 = w_tmp_13;                                    \
	w_old_31 = w_tmp_31;                                    \
	w_old_93 = w_tmp_93;                                    \
	w_old_39 = w_tmp_39

/* *********************************************************** */

#define	MAKE_1U16_FL0                                           \
	w_src10 = vis_fand(w_src10, w_mask2);                   \
	w_src11 = vis_fand(w_src11, w_mask2);                   \
	w_src10 = vis_fxor(w_src10, mask8000);                  \
	w_src11 = vis_fxor(w_src11, mask8000);                  \
	w_tmp10_1 = vis_fmul8x16(v_coeff1, w_src10);            \
	w_tmp10_3 = vis_fmul8x16(v_coeff3, w_src10);            \
	w_tmp10_9 = vis_fmul8x16(v_coeff9, w_src10);            \
	w_tmp11_1 = vis_fmul8x16(v_coeff1, w_src11);            \
	w_tmp11_3 = vis_fmul8x16(v_coeff3, w_src11);            \
	w_tmp11_9 = vis_fmul8x16(v_coeff9, w_src11);            \
	w_tmp_13 = vis_fpadd16(w_tmp10_1, w_tmp11_3);           \
	w_tmp_31 = vis_fpadd16(w_tmp10_3, w_tmp11_1);           \
	w_tmp_39 = vis_fpadd16(w_tmp10_3, w_tmp11_9);           \
	w_tmp_93 = vis_fpadd16(w_tmp10_9, w_tmp11_3)

/* *********************************************************** */

#define	TAIL_1U16_FL0(RESULT)                                   \
	w_res2 = vis_fpadd16(w_old_39, w_tmp_13);               \
	w_res0 = vis_fpadd16(w_old_93, w_tmp_31);               \
	w_res2 = vis_fxor(w_res2, mask8000);                    \
	w_res0 = vis_fxor(w_res0, mask8000);                    \
	RESULT(w_res0, w_res2);                                 \
	w_res3 = vis_fpadd16(w_old_13, w_tmp_39);               \
	w_res1 = vis_fpadd16(w_old_31, w_tmp_93);               \
	w_res3 = vis_fxor(w_res3, mask8000);                    \
	w_res1 = vis_fxor(w_res1, mask8000);                    \
	RESULT(w_res1, w_res3)

/* *********************************************************** */

#define	RESULT_1U16(s0, s1)                                        \
	{                                                          \
	    mlib_d64 dr, dr1;                                      \
	                                                           \
	    s0 = vis_fpmerge(vis_read_hi(s0), vis_read_lo(s0));    \
	    s0 = vis_fpmerge(vis_read_hi(s0), vis_read_lo(s0));    \
	    s1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));    \
	    s1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));    \
	    dr = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s1));    \
	    dr1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));   \
	    s0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));   \
	    s1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));   \
	}

/* *********************************************************** */

#define	RESULT_2U16(s0, s1)                                     \
	{                                                       \
	    mlib_f32 v_tmp = vis_read_hi(s1);                   \
	                                                        \
	    s1 = vis_write_hi(s1, vis_read_lo(s0));             \
	    s0 = vis_write_lo(s0, v_tmp);                       \
	}

/* *********************************************************** */

#define	RESULT_3U16(r0, r1)                                     \
	{                                                       \
	    mlib_d64 result1, result2;                          \
	                                                        \
	    result2 = vis_faligndata(result2, r0);              \
	    result1 = vis_faligndata(result2, result2);         \
	    result2 = vis_faligndata(result2, r1);              \
	    r0 = vis_faligndata(result1, result1);              \
	    r1 = vis_faligndata(result1, result2);              \
	    result2 = vis_faligndata(result2, result2);         \
	    r0 = vis_faligndata(r0, r1);                        \
	    r1 = vis_faligndata(r1, result2);                   \
	}

#define	RESULT_4U16(s0, s1)

/* *********************************************************** */

#define	C1_FL0	16
#define	C3_FL0	48

#define	C9_FL0	144

void
mlib_v_ImageZoomIn2X_U16_1BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 1;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL0 << 24) |
	    (C1_FL0 << 16) | (C1_FL0 << 8) | C1_FL0);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL0 << 24) |
	    (C3_FL0 << 16) | (C3_FL0 << 8) | C3_FL0);
	const mlib_f32 v_coeff9 = vis_to_float(((mlib_u32)C9_FL0 << 24) |
	    (C9_FL0 << 16) | (C9_FL0 << 8) | C9_FL0);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93, w_tmp_39, w_tmp_31, w_tmp_13;
	mlib_d64 w_old_93, w_old_39, w_old_31, w_old_13;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr(channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL0;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL0;

					LOAD_1U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_1U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL0;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL0;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_1U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL0;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_1U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL0;

				LOAD_1U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL0;

				TAIL_1U16_FL0(RESULT_1U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_2BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 2;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL0 << 24) |
	    (C1_FL0 << 16) | (C1_FL0 << 8) | C1_FL0);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL0 << 24) |
	    (C3_FL0 << 16) | (C3_FL0 << 8) | C3_FL0);
	const mlib_f32 v_coeff9 = vis_to_float(((mlib_u32)C9_FL0 << 24) |
	    (C9_FL0 << 16) | (C9_FL0 << 8) | C9_FL0);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93, w_tmp_39, w_tmp_31, w_tmp_13;
	mlib_d64 w_old_93, w_old_39, w_old_31, w_old_13;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr(channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL0;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL0;

					LOAD_1U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_2U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL0;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL0;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_2U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL0;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_2U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL0;

				LOAD_1U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL0;

				TAIL_1U16_FL0(RESULT_2U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_3BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 3;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL0 << 24) |
	    (C1_FL0 << 16) | (C1_FL0 << 8) | C1_FL0);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL0 << 24) |
	    (C3_FL0 << 16) | (C3_FL0 << 8) | C3_FL0);
	const mlib_f32 v_coeff9 = vis_to_float(((mlib_u32)C9_FL0 << 24) |
	    (C9_FL0 << 16) | (C9_FL0 << 8) | C9_FL0);
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93, w_tmp_39, w_tmp_31, w_tmp_13;
	mlib_d64 w_old_93, w_old_39, w_old_31, w_old_13;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;
	mlib_s32 lmask;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;
		lmask = 0xFFF0;

		vis_write_gsr(channels);

		for (j = 0; j < src_width / 2 - 2; j += 3) {

			LOAD_1U16G(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL0;

				LOAD_1U16G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL0;

				TAIL_1U16_FL0(RESULT_3U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		if (j < src_width / 2) {
			lmask = (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL0;

				LOAD_1U16G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL0;

				TAIL_1U16_FL0(RESULT_3U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_4BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 4;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL0 << 24) |
	    (C1_FL0 << 16) | (C1_FL0 << 8) | C1_FL0);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL0 << 24) |
	    (C3_FL0 << 16) | (C3_FL0 << 8) | C3_FL0);
	const mlib_f32 v_coeff9 = vis_to_float(((mlib_u32)C9_FL0 << 24) |
	    (C9_FL0 << 16) | (C9_FL0 << 8) | C9_FL0);
	mlib_d64 w_tmp10_1, w_tmp10_3, w_tmp10_9;
	mlib_d64 w_tmp11_1, w_tmp11_3, w_tmp11_9;
	mlib_d64 w_tmp_93, w_tmp_39, w_tmp_31, w_tmp_13;
	mlib_d64 w_old_93, w_old_39, w_old_31, w_old_13;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_4U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL0;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL0;

					LOAD_4U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_4U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_4U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL0;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL0;

					LOAD_4U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_4U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL0;

					LOAD_4U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL0;

					TAIL_1U16_FL0(RESULT_4U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_4U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL0;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL0;

				LOAD_4U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL0;

				TAIL_1U16_FL0(RESULT_4U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	MOVE_1U16_FL1	w_old_33 = w_tmp_33; w_old_30 = w_tmp_30

/* *********************************************************** */

#define	MAKE_1U16_FL1                                           \
	w_src10 = vis_fand(w_src10, w_mask2);                   \
	w_src11 = vis_fand(w_src11, w_mask2);                   \
	w_src10 = vis_fxor(w_src10, mask8000);                  \
	w_src11 = vis_fxor(w_src11, mask8000);                  \
	w_tmp_30 = vis_fmul8x16(v_coeff3, w_src10);             \
	w_tmp_33 = vis_fmul8x16(v_coeff3, w_src11);             \
	w_tmp_10 = vis_fmul8x16(v_coeff1, w_src10);             \
	w_tmp_11 = vis_fmul8x16(v_coeff1, w_src11);             \
	w_tmp_33 = vis_fpadd16(w_tmp_33, w_tmp_10);             \
	w_tmp_30 = vis_fpadd16(w_tmp_30, w_tmp_11)

/* *********************************************************** */

#define	TAIL_1U16_FL1(RESULT)                                   \
	w_res2 = vis_fpadd16(w_old_33, w_old_33);               \
	w_res0 = vis_fpadd16(w_old_30, w_old_30);               \
	w_res2 = vis_fxor(w_res2, mask8000);                    \
	w_res0 = vis_fxor(w_res0, mask8000);                    \
	RESULT(w_res0, w_res2);                                 \
	w_res3 = vis_fpadd16(w_old_33, w_tmp_33);               \
	w_res1 = vis_fpadd16(w_old_30, w_tmp_30);               \
	w_res3 = vis_fxor(w_res3, mask8000);                    \
	w_res1 = vis_fxor(w_res1, mask8000);                    \
	RESULT(w_res1, w_res3)

/* *********************************************************** */

#define	C1_FL1	32
#define	C3_FL1	96

void
mlib_v_ImageZoomIn2X_U16_1BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 1;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL1 << 24) |
	    (C1_FL1 << 16) | (C1_FL1 << 8) | C1_FL1);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL1 << 24) |
	    (C3_FL1 << 16) | (C3_FL1 << 8) | C3_FL1);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr(channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL1;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL1;

					LOAD_1U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_1U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL1;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL1;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_1U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL1;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_1U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL1;

				LOAD_1U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL1;

				TAIL_1U16_FL1(RESULT_1U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_2BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 2;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL1 << 24) |
	    (C1_FL1 << 16) | (C1_FL1 << 8) | C1_FL1);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL1 << 24) |
	    (C3_FL1 << 16) | (C3_FL1 << 8) | C3_FL1);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr(channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL1;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL1;

					LOAD_1U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_2U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL1;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL1;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_2U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL1;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_2U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL1;

				LOAD_1U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL1;

				TAIL_1U16_FL1(RESULT_2U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_4BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 4;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL1 << 24) |
	    (C1_FL1 << 16) | (C1_FL1 << 8) | C1_FL1);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL1 << 24) |
	    (C3_FL1 << 16) | (C3_FL1 << 8) | C3_FL1);
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_4U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL1;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL1;

					LOAD_4U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_4U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_4U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL1;

				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL1;

					LOAD_4U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL1;

					TAIL_1U16_FL1(RESULT_4U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_4U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL1;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL1;

				LOAD_4U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL1;

				TAIL_1U16_FL1(RESULT_4U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	MOVE_1U16_FL2                                           \
	w_old_11 = w_tmp_11;                                    \
	w_old_33 = w_tmp_33;                                    \
	w_old_10 = w_tmp_10;                                    \
	w_old_30 = w_tmp_30

/* *********************************************************** */

#define	MAKE_1U16_FL2                                           \
	w_src10 = vis_fand(w_src10, w_mask2);                   \
	w_src11 = vis_fand(w_src11, w_mask2);                   \
	w_src10 = vis_fxor(w_src10, mask8000);                  \
	w_src11 = vis_fxor(w_src11, mask8000);                  \
	w_tmp_30 = vis_fmul8x16(v_coeff3, w_src10);             \
	w_tmp_33 = vis_fmul8x16(v_coeff3, w_src11);             \
	w_tmp_10 = vis_fmul8x16(v_coeff1, w_src10);             \
	w_tmp_11 = vis_fmul8x16(v_coeff1, w_src11);             \
	w_tmp_33 = vis_fpadd16(w_tmp_33, w_tmp_30);             \
	w_tmp_30 = vis_fpadd16(w_tmp_30, w_tmp_30);             \
	w_tmp_11 = vis_fpadd16(w_tmp_11, w_tmp_10);             \
	w_tmp_10 = vis_fpadd16(w_tmp_10, w_tmp_10)

/* *********************************************************** */

#define	TAIL_1U16_FL2(RESULT)                                   \
	w_res2 = vis_fpadd16(w_old_33, w_tmp_11);               \
	w_res0 = vis_fpadd16(w_old_30, w_tmp_10);               \
	w_res2 = vis_fxor(w_res2, mask8000);                    \
	w_res0 = vis_fxor(w_res0, mask8000);                    \
	RESULT(w_res0, w_res2);                                 \
	w_res3 = vis_fpadd16(w_old_11, w_tmp_33);               \
	w_res1 = vis_fpadd16(w_old_10, w_tmp_30);               \
	w_res3 = vis_fxor(w_res3, mask8000);                    \
	w_res1 = vis_fxor(w_res1, mask8000);                    \
	RESULT(w_res1, w_res3)

/* *********************************************************** */

#define	C1_FL2	32
#define	C3_FL2	96

void
mlib_v_ImageZoomIn2X_U16_1BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 1;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL2 << 24) |
	    (C1_FL2 << 16) | (C1_FL2 << 8) | C1_FL2);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL2 << 24) |
	    (C3_FL2 << 16) | (C3_FL2 << 8) | C3_FL2);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_10, w_old_11, w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr(channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL2;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL2;

					LOAD_1U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_1U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL2;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL2;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_1U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL2;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_1U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL2;

				LOAD_1U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL2;

				TAIL_1U16_FL2(RESULT_1U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_2BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 2;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL2 << 24) |
	    (C1_FL2 << 16) | (C1_FL2 << 8) | C1_FL2);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL2 << 24) |
	    (C3_FL2 << 16) | (C3_FL2 << 8) | C3_FL2);
	const mlib_f32 v_zero = vis_fzeros();
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_10, w_old_11, w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		vis_write_gsr(channels);

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL2;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL2;

					LOAD_1U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_2U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_1U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL2;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL2;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_2U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL2;

					LOAD_1U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_2U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL2;

				LOAD_1U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL2;

				TAIL_1U16_FL2(RESULT_2U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_3BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 3;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL2 << 24) |
	    (C1_FL2 << 16) | (C1_FL2 << 8) | C1_FL2);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL2 << 24) |
	    (C3_FL2 << 16) | (C3_FL2 << 8) | C3_FL2);
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_10, w_old_11, w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;
	mlib_s32 lmask;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;
		lmask = 0xFFF0;

		vis_write_gsr(channels);

		for (j = 0; j < src_width / 2 - 2; j += 3) {

			LOAD_1U16G(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL2;

				LOAD_1U16G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL2;

				TAIL_1U16_FL2(RESULT_3U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		if (j < src_width / 2) {
			lmask = (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_1U16G(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL2;

				LOAD_1U16G(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL2;

				TAIL_1U16_FL2(RESULT_3U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
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
mlib_v_ImageZoomIn2X_U16_4BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, channels = 4;
	const mlib_d64 w_mask2 = vis_to_double_dup(0xFFFEFFFE);
	const mlib_f32 v_coeff1 = vis_to_float((C1_FL2 << 24) |
	    (C1_FL2 << 16) | (C1_FL2 << 8) | C1_FL2);
	const mlib_f32 v_coeff3 = vis_to_float((C3_FL2 << 24) |
	    (C3_FL2 << 16) | (C3_FL2 << 8) | C3_FL2);
	mlib_d64 w_tmp_10, w_tmp_11, w_tmp_30, w_tmp_33;
	mlib_d64 w_old_10, w_old_11, w_old_30, w_old_33;
	mlib_d64 w_res0, w_res1, w_res2, w_res3;
	mlib_d64 w_src10, w_src11;
	mlib_s32 num_of_rows, shag_row;

	src_width = channels * src_width;
	channels *= 2;
	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

		if (((((mlib_addr)psrc | src_stride) & 3) == 0) &&
		    ((((mlib_addr)pdst | dst_stride) & 7) == 0))
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_4U16A(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL2;

#pragma pipeloop(0)
				for (i = 0; i < src_height / 2; i++) {

					MOVE_1U16_FL2;

					LOAD_4U16A(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_4U16);

					STORE_1U16A(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16A(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
		} else
			for (j = 0; j < src_width / 2 - 3; j += 4) {

				LOAD_4U16G(w_src10, w_src11, psrc, j);

				MAKE_1U16_FL2;

				if (src_height / 2 > 0) {

					MOVE_1U16_FL2;

					LOAD_4U16G_NF(w_src10, w_src11, psrc,
					    src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_4U16);

					STORE_1U16G_NF(pdst, 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst, dst_stride + 2 * j,
					    w_res1, w_res3);
				}
				for (i = 1; i < src_height / 2; i++) {

					MOVE_1U16_FL2;

					LOAD_4U16G_NF(w_src10, w_src11, psrc,
					    (i + 1) * src_stride + j);

					MAKE_1U16_FL2;

					TAIL_1U16_FL2(RESULT_4U16);

					STORE_1U16G(pdst,
					    2 * i * dst_stride + 2 * j, w_res0,
					    w_res2);

					STORE_1U16G(pdst,
					    (2 * i + 1) * dst_stride + 2 * j,
					    w_res1, w_res3);
				}
			}

		if (j < src_width / 2) {
			mlib_s32 lmask =
			    (0xFFFF >> (4 * (src_width / 2 - j))) ^ 0xFFFF;

			LOAD_4U16G_NF(w_src10, w_src11, psrc, j);

			MAKE_1U16_FL2;

			for (i = 0; i < src_height / 2; i++) {

				MOVE_1U16_FL2;

				LOAD_4U16G_NF(w_src10, w_src11, psrc,
				    (i + 1) * src_stride + j);

				MAKE_1U16_FL2;

				TAIL_1U16_FL2(RESULT_4U16);

				STORE_1U16G_LAST(pdst,
				    2 * i * dst_stride + 2 * j, w_res0, w_res2);

				STORE_1U16G_LAST(pdst,
				    (2 * i + 1) * dst_stride + 2 * j, w_res1,
				    w_res3);
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	STORE_RES_U16_1(dr, dr1, pz)                            \
	{                                                       \
	    mlib_d64 dd0, dd1;                                  \
	                                                        \
	    dd0 = vis_faligndata(dr, dr);                       \
/* << 16 */                                                     \
	    vis_st_u16(dd0, pz++);                              \
	    dd1 = vis_faligndata(dr1, dr1);                     \
/* << 16 */                                                     \
	    vis_st_u16(dd1, pz++);                              \
	    dd0 = vis_faligndata(dd0, dd0);                     \
/* << 16 */                                                     \
	    vis_st_u16(dd0, pz++);                              \
	    dd1 = vis_faligndata(dd1, dd1);                     \
/* << 16 */                                                     \
	    vis_st_u16(dd1, pz++);                              \
	    dd0 = vis_faligndata(dd0, dd0);                     \
/* << 16 */                                                     \
	    vis_st_u16(dd0, pz++);                              \
	    dd1 = vis_faligndata(dd1, dd1);                     \
/* << 16 */                                                     \
	    vis_st_u16(dd1, pz++);                              \
	    vis_st_u16(dr, pz++);                               \
	    vis_st_u16(dr1, pz++);                              \
	}

/* *********************************************************** */

#define	BILINEAR_U16_124                                        \
	dr = vis_fpadd16(da2, dd2);                             \
	dr1 = vis_fpadd16(da3, dd3)

/* *********************************************************** */

#define	BILINEAR_U16_124_TAIL                                   \
	da = dd;                                                \
	da1 = dd1;                                              \
	da2 = dd2;                                              \
	da3 = dd3

/* *********************************************************** */

#define	GET_STR_U16_1_BILINEAR(ind, da, da1, da2, da3)          \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = dprow[0];                                          \
	da1 = dprow[1];                                         \
	da3 = vis_ld_d64_nf(dprow + 2);                         \
	da = vis_fxor(da, mask8000);                            \
	da1 = vis_fxor(da1, mask8000);                          \
	da3 = vis_fxor(da3, mask8000);                          \
	da = vis_faligndata(da, da1);                           \
	da2 = vis_fand(da, dmask_even);                         \
	da2 = vis_fmul8x16(scale05, da2);                       \
	da1 = vis_faligndata(da1, da3);                         \
	VIS_ALIGNADDR(dprow, 2);                                \
	da1 = vis_faligndata(da, da1);                          \
/* << 16 */                                                     \
	da1 = vis_fmul8x16(scale05, da1);                       \
	da1 = vis_fpadd16(da2, da1);                            \
	da3 = vis_fand(da1, dmask_even);                        \
	da3 = vis_fmul8x16(scale05, da3)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U16_1BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u16 *prow;
	mlib_u16 *pz_row;
	mlib_d64 da, da1, da2, da3, dd, dd1, dd2, dd3;
	mlib_d64 *dprow, dr, dr1;
	mlib_d64 dmask_even = vis_to_double_dup(0xfffefffe);
	mlib_f32 scale05 = vis_to_float(0x80808080);

	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;
		for (j = 0; j <= ((src_width / 2) - 4); j += 4) {
			prow = psrc + j;
			pz_row = pdst + 2 * j;

			GET_STR_U16_1_BILINEAR(0, da, da1, da2, da3);
			prow += src_stride;

			for (i = 0; i < (src_height / 2); i++) {
				GET_STR_U16_1_BILINEAR(0, dd, dd1, dd2, dd3);
				da = vis_fxor(da, mask8000);
				da1 = vis_fxor(da1, mask8000);
				STORE_RES_U16_1(da, da1, pz_row);
				BILINEAR_U16_124;
				pz_row += dst_stride - 8;
				dr = vis_fxor(dr, mask8000);
				dr1 = vis_fxor(dr1, mask8000);
				STORE_RES_U16_1(dr, dr1, pz_row);
				BILINEAR_U16_124_TAIL;
				prow += src_stride;
				pz_row += dst_stride - 8;
			}
		}

		for (j = src_width / 8 * 4; j < (src_width / 2); j++) {
			mlib_s32 a00 = psrc[j], a10;
			mlib_s32 a01 = psrc[j + 1], a11;

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

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	GET_STR_U16_2_BILINEAR(ind, da, da1, da2, da3)          \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = dprow[0];                                          \
	da1 = dprow[1];                                         \
	da3 = vis_ld_d64_nf(dprow + 2);                         \
	da = vis_fxor(da, mask8000);                            \
	da1 = vis_fxor(da1, mask8000);                          \
	da3 = vis_fxor(da3, mask8000);                          \
	da = vis_faligndata(da, da1);                           \
	da2 = vis_fand(da, dmask_even);                         \
	da2 = vis_fmul8x16(scale05, da2);                       \
	da1 = vis_faligndata(da1, da3);                         \
	VIS_ALIGNADDR(dprow, 4);                                \
	da1 = vis_faligndata(da, da1);                          \
/* << 32 */                                                     \
	da1 = vis_fmul8x16(scale05, da1);                       \
	da1 = vis_fpadd16(da2, da1);                            \
	da3 = vis_fand(da1, dmask_even);                        \
	da3 = vis_fmul8x16(scale05, da3)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U16_2BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, l;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u16 *prow;
	mlib_u16 *pz_row;
	mlib_d64 da, da1, da2, da3, dd, dd1, dd2, dd3;
	mlib_d64 *dprow, dr, dr1;
	mlib_d64 dmask_even = vis_to_double_dup(0xfffefffe);
	mlib_f32 scale05 = vis_to_float(0x80808080);

	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;

/* aligned case */
		if (!(((mlib_addr)pdst & 3) | (dst_stride & 1))) {

			for (j = 0; j <= ((src_width / 2) - 2); j += 2) {
				prow = psrc + 2 * j;
				pz_row = pdst + 4 * j;

				GET_STR_U16_2_BILINEAR(0, da, da1, da2, da3);
				prow += src_stride;
				for (i = 0; i < (src_height / 2); i++) {
					GET_STR_U16_2_BILINEAR(0, dd, dd1, dd2,
					    dd3);
					da = vis_fxor(da, mask8000);
					da1 = vis_fxor(da1, mask8000);
					((mlib_f32 *)pz_row)[0] =
					    vis_read_hi(da);
					((mlib_f32 *)pz_row)[1] =
					    vis_read_hi(da1);
					((mlib_f32 *)pz_row)[2] =
					    vis_read_lo(da);
					((mlib_f32 *)pz_row)[3] =
					    vis_read_lo(da1);

					BILINEAR_U16_124;
					pz_row += dst_stride;
					dr = vis_fxor(dr, mask8000);
					dr1 = vis_fxor(dr1, mask8000);
					((mlib_f32 *)pz_row)[0] =
					    vis_read_hi(dr);
					((mlib_f32 *)pz_row)[1] =
					    vis_read_hi(dr1);
					((mlib_f32 *)pz_row)[2] =
					    vis_read_lo(dr);
					((mlib_f32 *)pz_row)[3] =
					    vis_read_lo(dr1);
					BILINEAR_U16_124_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			}

			if ((j = src_width / 4 * 2) < (src_width / 2)) {
				for (l = 0; l < 2; l++) {
					mlib_s32 a00 = psrc[2 * j + l], a10;
					mlib_s32 a01 = psrc[2 * j + 2 + l], a11;

					for (i = 0; i < (src_height / 2); i++) {
						a10 = psrc[(i + 1) *
						    src_stride + 2 * j + l];
						a11 = psrc[(i + 1) *
						    src_stride + 2 * j + 2 + l];
						pdst[2 * i * dst_stride +
						    4 * j + l] = a00;
						pdst[2 * i * dst_stride +
						    4 * j + 2 + l] =
						    (a00 + a01) >> 1;
						pdst[(2 * i + 1) * dst_stride +
						    4 * j + l] =
						    (a00 + a10) >> 1;
						pdst[(2 * i + 1) * dst_stride +
						    4 * j + 2 + l] =
						    (a00 + a01 + a10 +
						    a11) >> 2;
						a00 = a10;
						a01 = a11;
					}
				}
			}
		} else {
/*  common case  */
			for (i = 0; i < (src_height / 2); i++) {
				mlib_s32 a00_1 = psrc[i * src_stride],
					a10_1 = psrc[(i + 1) * src_stride],
					a00_2 = psrc[i * src_stride + 1],
					a10_2 = psrc[(i + 1) * src_stride + 1];
					mlib_s32 a01_1, a11_1, a01_2, a11_2;

				for (j = 0; j < (src_width / 2); j++) {
					a01_1 = psrc[i * src_stride + 2 * j +
					    2];
					a11_1 = psrc[(i + 1) * src_stride +
					    2 * j + 2];
					a01_2 = psrc[i * src_stride + 2 * j +
					    3];
					a11_2 = psrc[(i + 1) * src_stride +
					    2 * j + 3];
					pdst[2 * i * dst_stride + 4 * j] =
					    a00_1;
					pdst[2 * i * dst_stride + 4 * j + 2] =
					    (a00_1 + a01_1) >> 1;
					pdst[(2 * i + 1) * dst_stride + 4 * j] =
					    (a00_1 + a10_1) >> 1;
					pdst[(2 * i + 1) * dst_stride + 4 * j +
					    2] =
					    (a00_1 + a01_1 + a10_1 +
					    a11_1) >> 2;
					pdst[2 * i * dst_stride + 4 * j + 1] =
					    a00_2;
					pdst[2 * i * dst_stride + 4 * j + 3] =
					    (a00_2 + a01_2) >> 1;
					pdst[(2 * i + 1) * dst_stride + 4 * j +
					    1] = (a00_2 + a10_2) >> 1;
					pdst[(2 * i + 1) * dst_stride + 4 * j +
					    3] =
					    (a00_2 + a01_2 + a10_2 +
					    a11_2) >> 2;
					a00_1 = a01_1;
					a00_2 = a01_2;
					a10_1 = a11_1;
					a10_2 = a11_2;
				}
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	COMPOSE_U16_3(pointer, dx, dx1, dx2, dy, dy1, dy2,      \
	dr, dr1, dr2, dr3, dr4, dr5)                            \
	{                                                       \
	    mlib_d64 dr6, dr7;                                  \
	                                                        \
	    dr6 = vis_faligndata(dx, dx1);                      \
	    dr1 = vis_faligndata(dy, dy);                       \
	    dr2 = vis_faligndata(dy, dy1);                      \
	    dr7 = vis_faligndata(dr2, dx);                      \
	    (void *)VIS_ALIGNADDR(pointer, 2);                  \
	    dr = vis_faligndata(dr7, dy);                       \
	    dr3 = vis_faligndata(dx1, dx2);                     \
	    dr4 = vis_faligndata(dy1, dy2);                     \
	    dr5 = vis_faligndata(dy2, dr3);                     \
	    dr3 = vis_faligndata(dr3, dr7);                     \
	    dr7 = vis_faligndata(dx2, dx2);                     \
	    (void *)VIS_ALIGNADDR(pointer, 4);                  \
	    dr1 = vis_faligndata(dr1, dr6);                     \
	    dr4 = vis_faligndata(dr4, dr7);                     \
	    (void *)VIS_ALIGNADDR(pointer, 6);                  \
	    dr2 = vis_faligndata(dr5, dr2);                     \
	    dr5 = vis_faligndata(dx2, dr5);                     \
	}

/* *********************************************************** */

#define	GET_STR_U16_3_BILINEAR(ind, dah, dam, dal, da1h,        \
	da1m, da1l, da2h, da2m, da2l, da3h, da3m, da3l)         \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	dah = dprow[0];                                         \
	dam = dprow[1];                                         \
	dal = dprow[2];                                         \
	da1h = dprow[3];                                        \
	da1m = vis_ld_d64_nf(dprow + 4);                        \
	dah = vis_fxor(dah, mask8000);                          \
	dam = vis_fxor(dam, mask8000);                          \
	dal = vis_fxor(dal, mask8000);                          \
	da1h = vis_fxor(da1h, mask8000);                        \
	da1m = vis_fxor(da1m, mask8000);                        \
	dah = vis_faligndata(dah, dam);                         \
	dam = vis_faligndata(dam, dal);                         \
	dal = vis_faligndata(dal, da1h);                        \
	da2h = vis_fand(dah, dmask_even);                       \
	da2m = vis_fand(dam, dmask_even);                       \
	da2l = vis_fand(dal, dmask_even);                       \
	da2h = vis_fmul8x16(scale05, da2h);                     \
	da2m = vis_fmul8x16(scale05, da2m);                     \
	da2l = vis_fmul8x16(scale05, da2l);                     \
	da1l = vis_faligndata(da1h, da1m);                      \
	VIS_ALIGNADDR(dprow, 6);                                \
	da1h = vis_faligndata(dah, dam);                        \
/* << 48 */                                                     \
	da1m = vis_faligndata(dam, dal);                        \
	da1l = vis_faligndata(dal, da1l);                       \
	da1h = vis_fmul8x16(scale05, da1h);                     \
	da1m = vis_fmul8x16(scale05, da1m);                     \
	da1l = vis_fmul8x16(scale05, da1l);                     \
	da1h = vis_fpadd16(da2h, da1h);                         \
	da1m = vis_fpadd16(da2m, da1m);                         \
	da1l = vis_fpadd16(da2l, da1l);                         \
	da3h = vis_fand(da1h, dmask_even);                      \
	da3m = vis_fand(da1m, dmask_even);                      \
	da3l = vis_fand(da1l, dmask_even);                      \
	da3h = vis_fmul8x16(scale05, da3h);                     \
	da3m = vis_fmul8x16(scale05, da3m);                     \
	da3l = vis_fmul8x16(scale05, da3l)

/* *********************************************************** */

#define	BILINEAR_U16_3                                          \
	da2h = vis_fpadd16(da2h, dd2h);                         \
	da2m = vis_fpadd16(da2m, dd2m);                         \
	da2l = vis_fpadd16(da2l, dd2l);                         \
	da3h = vis_fpadd16(da3h, dd3h);                         \
	da3m = vis_fpadd16(da3m, dd3m);                         \
	da3l = vis_fpadd16(da3l, dd3l)

/* *********************************************************** */

#define	BILINEAR_U16_3_TAIL                                     \
	da2h = dd2h;                                            \
	da2m = dd2m;                                            \
	da2l = dd2l;                                            \
	da3h = dd3h;                                            \
	da3m = dd3m;                                            \
	da3l = dd3l

/* *********************************************************** */

#define	STORE_RES_U16_3_COMMON(pointer, res1, res2, res3,             \
	res4, res5, res6)                                             \
	{                                                             \
	    mlib_d64 *dpz = (mlib_d64 *)VIS_ALIGNADDR(pointer, -1);   \
	    mlib_s32 emask =                                          \
		0xff >> ((mlib_addr)pointer - (mlib_addr)dpz);        \
	    res1 = vis_fxor(res1, mask8000);                          \
	    res2 = vis_fxor(res2, mask8000);                          \
	    res3 = vis_fxor(res3, mask8000);                          \
	    res4 = vis_fxor(res4, mask8000);                          \
	    res5 = vis_fxor(res5, mask8000);                          \
	    res6 = vis_fxor(res6, mask8000);                          \
	    (void *)VIS_ALIGNADDRL(pointer, 0);                       \
	    vis_pst_8(vis_faligndata(res1, res1), dpz, emask);        \
	    dpz[1] = vis_faligndata(res1, res2);                      \
	    dpz[2] = vis_faligndata(res2, res3);                      \
	    dpz[3] = vis_faligndata(res3, res4);                      \
	    dpz[4] = vis_faligndata(res4, res5);                      \
	    dpz[5] = vis_faligndata(res5, res6);                      \
	    vis_pst_8(vis_faligndata(res6, res6), dpz + 6, ~emask);   \
	}

/* *********************************************************** */

#define	STORE_RES_U16_3_COMMON_NF(pointer, res1, res2, res3,          \
	res4, res5, res6)                                             \
	{                                                             \
	    mlib_d64 *dpz = (mlib_d64 *)VIS_ALIGNADDR(pointer, -1);   \
	    mlib_s32 emask =                                          \
		0xff >> ((mlib_addr)pointer - (mlib_addr)dpz);        \
	    res1 = vis_fxor(res1, mask8000);                          \
	    res2 = vis_fxor(res2, mask8000);                          \
	    res3 = vis_fxor(res3, mask8000);                          \
	    res4 = vis_fxor(res4, mask8000);                          \
	    res5 = vis_fxor(res5, mask8000);                          \
	    res6 = vis_fxor(res6, mask8000);                          \
	    (void *)VIS_ALIGNADDRL(pointer, 0);                       \
	    if (emask)                                                \
		vis_pst_8(vis_faligndata(res1, res1), dpz, emask);    \
	    dpz[1] = vis_faligndata(res1, res2);                      \
	    dpz[2] = vis_faligndata(res2, res3);                      \
	    dpz[3] = vis_faligndata(res3, res4);                      \
	    dpz[4] = vis_faligndata(res4, res5);                      \
	    dpz[5] = vis_faligndata(res5, res6);                      \
	    vis_pst_8(vis_faligndata(res6, res6), dpz + 6, ~emask);   \
	}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U16_3BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j, l;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u16 *prow;
	mlib_u16 *pz_row;
	mlib_d64 dah, dam, dal, da1h, da1m, da1l, da2h, da2m, da2l, da3h, da3m,
	    da3l;
	mlib_d64 ddh, ddm, ddl, dd1h, dd1m, dd1l, dd2h, dd2m, dd2l, dd3h, dd3m,
	    dd3l;
	mlib_d64 *dprow, drh, drm, drl, dr1h, dr1m, dr1l;
	mlib_d64 dmask_even = vis_to_double_dup(0xfffefffe);
	mlib_f32 scale05 = vis_to_float(0x80808080);

	if (((((mlib_addr)pdst & 7) | (dst_stride & 3)) == 4) &&
	    (src_width / 2) > 0) {
		for (l = 0; l < 3; l++) {
			mlib_s32 a00 = psrc[l], a10;
			mlib_s32 a01 = psrc[l + 3], a11;

			for (i = 0; i < (src_height / 2); i++) {
				a10 = psrc[(i + 1) * src_stride + l],
				a11 = psrc[(i + 1) * src_stride + 3 + l];
				pdst[2 * i * dst_stride + l] = a00;
				pdst[2 * i * dst_stride + 3 + l] =
				    (a00 + a01) >> 1;
				pdst[(2 * i + 1) * dst_stride + l] =
				    (a00 + a10) >> 1;
				pdst[(2 * i + 1) * dst_stride + 3 + l] =
				    (a00 + a01 + a10 + a11) >> 2;
				a00 = a10;
				a01 = a11;
			}
		}

		src_width -= 2;
		psrc += 3;
		pdst += 6;
	}

	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;
		for (j = 0; j <= ((src_width / 2) - 4); j += 4) {
			prow = psrc + 3 * j;
			pz_row = pdst + 6 * j;

			GET_STR_U16_3_BILINEAR(0, dah, dam, dal, da1h, da1m,
			    da1l, da2h, da2m, da2l, da3h, da3m, da3l);
			prow += src_stride;

			if (src_height > 0) {
				COMPOSE_U16_3(dprow, dah, dam, dal, da1h, da1m,
				    da1l, drh, drm, drl, dr1h, dr1m, dr1l);
				STORE_RES_U16_3_COMMON_NF(pz_row, drh, drm, drl,
				    dr1h, dr1m, dr1l);
				pz_row += dst_stride;

/* common case */
				if (((mlib_addr)pdst | dst_stride) & 3) {
#pragma pipeloop(0)
					for (i = 0; i < ((src_height / 2) - 1);
					    i++) {
						GET_STR_U16_3_BILINEAR(0, ddh,
						    ddm, ddl, dd1h, dd1m, dd1l,
						    dd2h, dd2m, dd2l, dd3h,
						    dd3m, dd3l);
						BILINEAR_U16_3;
						COMPOSE_U16_3(dprow, da2h, da2m,
						    da2l, da3h, da3m, da3l, drh,
						    drm, drl, dr1h, dr1m, dr1l);
						STORE_RES_U16_3_COMMON(pz_row,
						    drh, drm, drl, dr1h, dr1m,
						    dr1l);
						VIS_ALIGNADDR(dprow, 6);
						pz_row += dst_stride;

						BILINEAR_U16_3_TAIL;
						COMPOSE_U16_3(dprow, ddh, ddm,
						    ddl, dd1h, dd1m, dd1l, drh,
						    drm, drl, dr1h, dr1m, dr1l);
						STORE_RES_U16_3_COMMON(pz_row,
						    drh, drm, drl, dr1h, dr1m,
						    dr1l);
						prow += src_stride;
						pz_row += dst_stride;
					}
				} else {
/* aligned case */
					for (i = 0; i < ((src_height / 2) - 1);
					    i++) {
						GET_STR_U16_3_BILINEAR(0, ddh,
						    ddm, ddl, dd1h, dd1m, dd1l,
						    dd2h, dd2m, dd2l, dd3h,
						    dd3m, dd3l);
						BILINEAR_U16_3;
						COMPOSE_U16_3(dprow, da2h, da2m,
						    da2l, da3h, da3m, da3l, drh,
						    drm, drl, dr1h, dr1m, dr1l);
						((mlib_d64 *)pz_row)[0] =
						    vis_fxor(drh, mask8000);
						((mlib_d64 *)pz_row)[3] =
						    vis_fxor(dr1h, mask8000);
						((mlib_d64 *)pz_row)[1] =
						    vis_fxor(drm, mask8000);
						((mlib_d64 *)pz_row)[4] =
						    vis_fxor(dr1m, mask8000);
						((mlib_d64 *)pz_row)[2] =
						    vis_fxor(drl, mask8000);
						((mlib_d64 *)pz_row)[5] =
						    vis_fxor(dr1l, mask8000);
						pz_row += dst_stride;

						BILINEAR_U16_3_TAIL;
						COMPOSE_U16_3(dprow, ddh, ddm,
						    ddl, dd1h, dd1m, dd1l, drh,
						    drm, drl, dr1h, dr1m, dr1l);
						((mlib_d64 *)pz_row)[0] =
						    vis_fxor(drh, mask8000);
						((mlib_d64 *)pz_row)[3] =
						    vis_fxor(dr1h, mask8000);
						((mlib_d64 *)pz_row)[1] =
						    vis_fxor(drm, mask8000);
						((mlib_d64 *)pz_row)[4] =
						    vis_fxor(dr1m, mask8000);
						((mlib_d64 *)pz_row)[2] =
						    vis_fxor(drl, mask8000);
						((mlib_d64 *)pz_row)[5] =
						    vis_fxor(dr1l, mask8000);
						pz_row += dst_stride;
						prow += src_stride;
					}
				}

				GET_STR_U16_3_BILINEAR(0, ddh, ddm, ddl, dd1h,
				    dd1m, dd1l, dd2h, dd2m, dd2l, dd3h, dd3m,
				    dd3l);
				BILINEAR_U16_3;
				COMPOSE_U16_3(dprow, da2h, da2m, da2l, da3h,
				    da3m, da3l, drh, drm, drl, dr1h, dr1m,
				    dr1l);
				STORE_RES_U16_3_COMMON(pz_row, drh, drm, drl,
				    dr1h, dr1m, dr1l);
			}
		}

		for (j = (src_width / 8) * 4; j < (src_width / 2); j++) {
			for (l = 0; l < 3; l++) {
				mlib_s32 a00 = psrc[3 * j + l], a10;
				mlib_s32 a01 = psrc[3 * j + l + 3], a11;

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

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

#define	GET_STR_U16_4_BILINEAR(ind, da, da1, da2, da3)          \
	dprow = (mlib_d64 *)VIS_ALIGNADDR(prow, ind);           \
	da = dprow[0];                                          \
	da1 = dprow[1];                                         \
	da3 = vis_ld_d64_nf(dprow + 2);                         \
	da = vis_fxor(da, mask8000);                            \
	da1 = vis_fxor(da1, mask8000);                          \
	da3 = vis_fxor(da3, mask8000);                          \
	da = vis_faligndata(da, da1);                           \
	da2 = vis_fand(da, dmask_even);                         \
	da2 = vis_fmul8x16(scale05, da2);                       \
	da1 = vis_faligndata(da1, da3);                         \
	da1 = vis_fmul8x16(scale05, da1);                       \
	da1 = vis_fpadd16(da2, da1);                            \
	da3 = vis_fand(da1, dmask_even);                        \
	da3 = vis_fmul8x16(scale05, da3)

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U16_4BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM_BL(mlib_u16))
{
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

/* indices */
	mlib_s32 i, j;
	mlib_s32 num_of_rows, shag_row;
	const mlib_u16 *prow;
	mlib_u16 *pz_row;
	mlib_d64 da, da1, da2, da3, dd, dd1, dd2, dd3;
	mlib_d64 *dprow, dr, dr1;
	mlib_d64 dmask_even = vis_to_double_dup(0xfffefffe);
	mlib_f32 scale05 = vis_to_float(0x80808080);

	num_of_rows = src_height / 2;
	SHAG_STR(mlib_u16);

	while (num_of_rows > 0) {
		src_height =
		    2 * (num_of_rows > shag_row ? shag_row : num_of_rows);
		num_of_rows -= src_height / 2;
		for (j = 0; j < (src_width / 2); j++) {
			prow = psrc + 4 * j;
			pz_row = pdst + 8 * j;

			GET_STR_U16_4_BILINEAR(0, da, da1, da2, da3);
			prow += src_stride;

/* common case */
			if (((mlib_addr)pdst & 7) | (dst_stride & 3)) {
				if (src_height / 2 > 0) {
					GET_STR_U16_4_BILINEAR(0, dd, dd1, dd2,
					    dd3);
					da = vis_fxor(da, mask8000);
					da1 = vis_fxor(da1, mask8000);
					STORE_RES_U8_COMMON_NF(pz_row, da, da1);

					BILINEAR_U16_124;
					pz_row += dst_stride;
					dr = vis_fxor(dr, mask8000);
					dr1 = vis_fxor(dr1, mask8000);
					STORE_RES_U8_COMMON(pz_row, dr, dr1);
					BILINEAR_U16_124_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
#pragma pipeloop(0)
				for (i = 1; i < (src_height / 2); i++) {
					GET_STR_U16_4_BILINEAR(0, dd, dd1, dd2,
					    dd3);
					da = vis_fxor(da, mask8000);
					da1 = vis_fxor(da1, mask8000);
					STORE_RES_U8_COMMON(pz_row, da, da1);

					BILINEAR_U16_124;
					pz_row += dst_stride;
					dr = vis_fxor(dr, mask8000);
					dr1 = vis_fxor(dr1, mask8000);
					STORE_RES_U8_COMMON(pz_row, dr, dr1);
					BILINEAR_U16_124_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			} else {
/* aligned case */
#pragma pipeloop(0)
				for (i = 0; i < (src_height / 2); i++) {
					GET_STR_U16_4_BILINEAR(0, dd, dd1, dd2,
					    dd3);
					da = vis_fxor(da, mask8000);
					da1 = vis_fxor(da1, mask8000);
					((mlib_d64 *)pz_row)[0] = da;
					((mlib_d64 *)pz_row)[1] = da1;

					BILINEAR_U16_124;
					pz_row += dst_stride;
					dr = vis_fxor(dr, mask8000);
					dr1 = vis_fxor(dr1, mask8000);
					((mlib_d64 *)pz_row)[0] = dr;
					((mlib_d64 *)pz_row)[1] = dr1;
					BILINEAR_U16_124_TAIL;
					prow += src_stride;
					pz_row += dst_stride;
				}
			}
		}

		psrc += src_height / 2 * src_stride;
		pdst += src_height / 2 * 2 * dst_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageZoomIn2X_U16_BL_FL0(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u16))
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
			mlib_v_ImageZoomIn2X_U16_1BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
			break;
		case 2:
			mlib_v_ImageZoomIn2X_U16_2BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
			break;
		case 3:
			mlib_v_ImageZoomIn2X_U16_3BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
			break;
		case 4:
			mlib_v_ImageZoomIn2X_U16_4BL_FL0
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
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
mlib_v_ImageZoomIn2X_U16_BL_FL1(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u16))
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

	if (channels != 3) {
		i = src_height / 2;
		j = src_width / 2;

		if (i && j)
			switch (channels) {
			case 1:
				mlib_v_ImageZoomIn2X_U16_1BL_FL1
				    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
				break;
			case 2:
				mlib_v_ImageZoomIn2X_U16_2BL_FL1
				    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
				break;
			case 4:
				mlib_v_ImageZoomIn2X_U16_4BL_FL1
				    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
				break;
			}
	} else
		for (i = 0; i < (src_height / 2); i++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 a00 = psrc[i * src_stride + k],
					a10 = psrc[(i + 1) * src_stride + k];
				mlib_s32 l = k + channels;
				mlib_s32 a01, a11;

				for (j = 0; j < (src_width / 2); j++) {
					a01 = psrc[i * src_stride +
					    j * channels + l];
					a11 = psrc[(i + 1) * src_stride +
					    j * channels + l];
					pdst[2 * i * dst_stride +
					    2 * channels * j + k] =
					    MLIB_ZOOMIN2X_UL_FL1;
					pdst[2 * i * dst_stride +
					    2 * channels * j + l] =
					    MLIB_ZOOMIN2X_UR_FL1;
					pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + k] =
					    MLIB_ZOOMIN2X_DL_FL1;
					pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + l] =
					    MLIB_ZOOMIN2X_DR_FL1;
					a00 = a01;
					a10 = a11;
				}
			}
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
mlib_v_ImageZoomIn2X_U16_BL_FL2(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u16))
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

	if (channels != 3) {
		i = src_height / 2;
		j = src_width / 2;

		if (i && j)
			switch (channels) {
			case 1:
				mlib_v_ImageZoomIn2X_U16_1BL_FL2
				    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
				break;
			case 2:
				mlib_v_ImageZoomIn2X_U16_2BL_FL2
				    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
				break;
			case 4:
				mlib_v_ImageZoomIn2X_U16_4BL_FL2
				    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
				break;
			}
	} else
		for (i = 0; i < (src_height / 2); i++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 a00 = psrc[i * src_stride + k],
					a10 = psrc[(i + 1) * src_stride + k];
				mlib_s32 l = k + channels;
				mlib_s32 a01, a11;

				for (j = 0; j < (src_width / 2); j++) {
					a01 = psrc[i * src_stride +
					    j * channels + l];
					a11 = psrc[(i + 1) * src_stride +
					    j * channels + l];
					pdst[2 * i * dst_stride +
					    2 * channels * j + k] =
					    MLIB_ZOOMIN2X_UL_FL2;
					pdst[2 * i * dst_stride +
					    2 * channels * j + l] =
					    MLIB_ZOOMIN2X_UR_FL2;
					pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + k] =
					    MLIB_ZOOMIN2X_DL_FL2;
					pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + l] =
					    MLIB_ZOOMIN2X_DR_FL2;
					a00 = a01;
					a10 = a11;
				}
			}
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
mlib_v_ImageZoomIn2X_U16_BL_FL3(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u16))
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

	i = src_height / 2;
	j = src_width / 2;

	if (i && j)
		switch (channels) {
		case 1:
			mlib_v_ImageZoomIn2X_U16_1BL_FL3
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
			break;
		case 2:
			mlib_v_ImageZoomIn2X_U16_2BL_FL3
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
			break;
		case 3:
			mlib_v_ImageZoomIn2X_U16_3BL_FL3
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
			break;
		case 4:
			mlib_v_ImageZoomIn2X_U16_4BL_FL3
			    (MLIB_ZOOMIN2XCALLPARAM_BL(mlib_u16));
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

#define	STYPE	mlib_s16

#define	FTYPE	mlib_d64

/* *********************************************************** */

#define	DEF_CONST_V0()                                          \
	mlib_d64 mask2 = vis_to_double_dup(0xFFFCFFFC);         \
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);      \
	mlib_d64 q = vis_to_double(0xC0C0C0C0, 0x40404040)

/* *********************************************************** */

#define	DEF_CONST_V1()                                          \
	mlib_d64 mask2 = vis_to_double_dup(0xFFFEFFFE);         \
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);      \
	mlib_d64 q = vis_to_double(0x80808080, 0x80808080)

/* *********************************************************** */

#define	DEF_CONST_H0()                                          \
	mlib_d64 g = vis_to_double(0xC0C0C0C0, 0x40404040)

/* *********************************************************** */

#define	DEF_CONST_H1()                                          \
	mlib_d64 g = vis_to_double(0x80808080, 0x80808080)

/* *********************************************************** */

#define	SUMV_A0(a, b)                                                   \
	vis_fpadd16(vis_fmul8x16(vis_read_hi(q), vis_fxor(vis_fand(a,   \
	    mask2), mask8000)), vis_fmul8x16(vis_read_lo(q),            \
	    vis_fxor(vis_fand(b, mask2), mask8000)))

/* *********************************************************** */

#define	SUMV_B0(a, b)                                                   \
	vis_fpadd16(vis_fmul8x16(vis_read_lo(q), vis_fxor(vis_fand(a,   \
	    mask2), mask8000)), vis_fmul8x16(vis_read_hi(q),            \
	    vis_fxor(vis_fand(b, mask2), mask8000)))

#define	SUMV_A1(a, b)	vis_fxor(vis_fand(a, mask2), mask8000)

/* *********************************************************** */

#define	SUMV_B1(a, b)                                                   \
	vis_fpadd16(vis_fmul8x16(vis_read_hi(q), vis_fxor(vis_fand(a,   \
	    mask2), mask8000)), vis_fmul8x16(vis_read_lo(q),            \
	    vis_fxor(vis_fand(b, mask2), mask8000)))

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

/* without saturation */
#undef MERGE_S16_CH1
#undef MERGE_S16_CH2
#undef MERGE_S16_CH4

/* *********************************************************** */

#define	MERGE_S16_CH1(s0, s1)                                     \
	{                                                         \
	    mlib_d64 t0, t1, e0, e1;                              \
	                                                          \
	    t0 = vis_fpmerge(vis_read_hi(s0), vis_read_lo(s0));   \
	    t0 = vis_fpmerge(vis_read_hi(t0), vis_read_lo(t0));   \
	    t1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));   \
	    t1 = vis_fpmerge(vis_read_hi(t1), vis_read_lo(t1));   \
	    e0 = vis_fpmerge(vis_read_hi(t0), vis_read_hi(t1));   \
	    e1 = vis_fpmerge(vis_read_lo(t0), vis_read_lo(t1));   \
	    s0 = vis_fpmerge(vis_read_hi(e0), vis_read_hi(e1));   \
	    s1 = vis_fpmerge(vis_read_lo(e0), vis_read_lo(e1));   \
	}

/* *********************************************************** */

#define	MERGE_S16_CH2(s0, s1)                                   \
	{                                                       \
	    mlib_d64 t0, t1;                                    \
	                                                        \
	    t0 = vis_faligndata(s0, s0);                        \
	    t1 = vis_faligndata(s1, s1);                        \
	    t0 = vis_faligndata(t0, s1);                        \
	    s1 = vis_faligndata(s0, t1);                        \
	    s0 = t0;                                            \
	}

#define	MERGE_S16_CH4(s0, s1)

/* *********************************************************** */

FUNC_ZOOM_BL(U16, 2, 0, 0)
    FUNC_ZOOM_BL(U16, 2, 0, 1)
    FUNC_ZOOM_BL(U16, 2, 1, 0)
    FUNC_ZOOM_BL(U16, 4, 0, 0)
    FUNC_ZOOM_BL(U16, 4, 1, 0)

/* *********************************************************** */
