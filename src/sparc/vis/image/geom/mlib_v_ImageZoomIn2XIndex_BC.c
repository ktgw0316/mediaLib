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

#pragma ident	"@(#)mlib_v_ImageZoomIn2XIndex_BC.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2XIndex - indexed color image 2X scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2XIndex(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_filter      filter,
 *                                          mlib_edge        edge,
 *                                          const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      The resampling filter can be one of the following:
 *              MLIB_NEAREST
 *              MLIB_BILINEAR
 *              MLIB_BICUBIC
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_OP_NEAREST
 *              MLIB_EDGE_SRC_EXTEND
 */

/* *********************************************************** */

/*								*/
/*			filter = BICUBIC			*/
/*			(Internal functions)			*/
/*								*/

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_v_ImageZoomIn2XIndex.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXDEFPARAM(MLIB_TYPE)                         \
	MLIB_TYPE  *pdst,                                             \
	const MLIB_TYPE  *psrc,                                       \
	mlib_s32 src_width,                                           \
	mlib_s32 src_height,                                          \
	mlib_s32 dst_stride,                                          \
	mlib_s32 src_stride,                                          \
	mlib_s32 src_w_beg,                                           \
	mlib_s32 src_h_beg,                                           \
	const void *colormap

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLPARAM(MLIB_TYPE)                           \
	(MLIB_TYPE *) pdst, (MLIB_TYPE *) psrc, src_width, src_height,   \
	dst_stride, src_stride, src_w_beg, src_h_beg, colormap

/* *********************************************************** */

#define	IMAGE_COL_U8_BC_ODD1(ind, st, shift)                    \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	st = vis_fmul8x16au(fa, fone1)

/* *********************************************************** */

#define	IMAGE_COL_U8_BC_ODD2(ind, st, shift)                    \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	da0 = vis_fmul8x16al(fa, fone_nine);                    \
	da_1 = vis_fmul8x16au(fa_1, fone_nine);                 \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16al(fa1, fone_nine);                   \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16au(fa2, fone_nine);                   \
	st = vis_fpsub16(st, da2)

/* *********************************************************** */

#define	IMAGE_COL_U8_BC_EVEN1(ind, st, shift)                   \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	da0 = vis_fmul8x16au(fa, fhund_twen);                   \
	da_1 = vis_fmul8x16au(fa_1, fnine_three);               \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16al(fa1, fhund_twen);                  \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16al(fa2, fnine_three);                 \
	st = vis_fpsub16(st, da2)

/* *********************************************************** */

#define	IMAGE_COL_U8_BC_EVEN2(ind, st, shift)                   \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	da0 = vis_fmul8x16al(fa, fhund_twen);                   \
	da_1 = vis_fmul8x16al(fa_1, fnine_three);               \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16au(fa1, fhund_twen);                  \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16au(fa2, fnine_three);                 \
	st = vis_fpsub16(st, da2)

/* *********************************************************** */

#define	IMAGE_ROW_U8_BC_ODD1(ind)	dst_buf1[ind] = vis_fpack16(col1)

/* *********************************************************** */

#define	IMAGE_ROW_U8_BC_ODD2(ind)                               \
	da0 = vis_fpadd16(col1, col2);                          \
	da0 = vis_fmul8x16(fnine, da0);                         \
	da1 = vis_fpadd16(col0, col3);                          \
	da1 = vis_fmul8x16(fone, da1);                          \
	da0 = vis_fpsub16(da0, da1);                            \
	dst_buf1[ind] = vis_fpack16(da0)

/* *********************************************************** */

#define	IMAGE_ROW_U8_BC_EVEN1(ind)                              \
	da0 = vis_fmul8x16(fnine, col0);                        \
	da1 = vis_fmul8x16(fhund, col1);                        \
	da1 = vis_fpsub16(da1, da0);                            \
	da_1 = vis_fmul8x16(ftwen, col2);                       \
	da1 = vis_fpadd16(da1, da_1);                           \
	da2 = vis_fmul8x16(fthree, col3);                       \
	da1 = vis_fpsub16(da1, da2);                            \
	dst_buf1[ind] = vis_fpack16(da1)

/* *********************************************************** */

#define	IMAGE_ROW_U8_BC_EVEN2(ind)                              \
	da0 = vis_fmul8x16(fthree, col0);                       \
	da1 = vis_fmul8x16(ftwen, col1);                        \
	da1 = vis_fpsub16(da1, da0);                            \
	da_1 = vis_fmul8x16(fhund, col2);                       \
	da1 = vis_fpadd16(da1, da_1);                           \
	da2 = vis_fmul8x16(fnine, col3);                        \
	da1 = vis_fpsub16(da1, da2);                            \
	dst_buf1[ind] = vis_fpack16(da1)

/* *********************************************************** */

#define	IMAGE_COL_S16_BC_ODD1(ind, st, shift)                   \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	st = vis_fmul8x16(fone1, fa)

/* *********************************************************** */

#define	IMAGE_COL_S16_BC_ODD2(ind, st, shift)                   \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	da_1 = vis_fmul8x16(fone_by_32, fa_1);                  \
	da0 = vis_fmul8x16(fnine_by_32, fa);                    \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16(fnine_by_32, fa1);                   \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16(fone_by_32, fa2);                    \
	st = vis_fpsub16(st, da2)

/* *********************************************************** */

#define	IMAGE_COL_S16_BC_EVEN1(ind, st, shift)                  \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	da_1 = vis_fmul8x16(fnine_by_256, fa_1);                \
	da0 = vis_fmul8x16(fhund_by_256, fa);                   \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16(ftwen_by_256, fa1);                  \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16(fthree_by_256, fa2);                 \
	st = vis_fpsub16(st, da2)

/* *********************************************************** */

#define	IMAGE_COL_S16_BC_EVEN2(ind, st, shift)                  \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - shift]];                          \
	fa1 = lut[psrc[ind + shift]];                           \
	fa2 = lut[psrc[ind + 2 * shift]];                       \
	da_1 = vis_fmul8x16(fthree_by_256, fa_1);               \
	da0 = vis_fmul8x16(ftwen_by_256, fa);                   \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16(fhund_by_256, fa1);                  \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16(fnine_by_256, fa2);                  \
	st = vis_fpsub16(st, da2)

/* *********************************************************** */

#define	IMAGE_ROW_S16_BC_ODD1(ind)                              \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(col1));         \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(col1));         \
	dst_buf1[ind] = vis_fpackfix_pair(da0, da1)

/* *********************************************************** */

#define	IMAGE_ROW_S16_BC_ODD2(ind)                              \
	da0 = vis_fmul8x16(fnine_by_16, col1);                  \
	da1 = vis_fmul8x16(fnine_by_16, col2);                  \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fone_by_16, col0);                  \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fone_by_16, col3);                   \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf1[ind] = vis_fpackfix_pair(da0, da1)

/* *********************************************************** */

#define	IMAGE_ROW_S16_BC_EVEN1(ind)                             \
	da0 = vis_fmul8x16(fhund_by_128, col1);                 \
	da1 = vis_fmul8x16(ftwen_by_128, col2);                 \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fnine_by_128, col0);                \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fthree_by_128, col3);                \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf1[ind] = vis_fpackfix_pair(da0, da1)

/* *********************************************************** */

#define	IMAGE_ROW_S16_BC_EVEN2(ind)                             \
	da0 = vis_fmul8x16(ftwen_by_128, col1);                 \
	da1 = vis_fmul8x16(fhund_by_128, col2);                 \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fthree_by_128, col0);               \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fnine_by_128, col3);                 \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf1[ind] = vis_fpackfix_pair(da0, da1)

/* *********************************************************** */

#define	DEFINE_KOEFF_ROW_EVEN_U8()                              \
/* 111*2 */                                                     \
	mlib_f32 fhund = vis_to_float(0xDEDEDEDE),              \
/* 29*2 */                                                      \
	    ftwen = vis_to_float(0x3A3A3A3A),                   \
/* 9*2 */                                                       \
	    fnine = vis_to_float(0x12121212),                   \
/* 3*2 */                                                       \
	    fthree = vis_to_float(0x6060606)

/* *********************************************************** */

#define	DEFINE_KOEFF_COL_EVEN_U8()                              \
/* 111*64, 29*64 */                                             \
	mlib_f32 fhund_twen = vis_to_float(0x1BC00740),         \
/* 9*64, 3*64 */                                                \
	    fnine_three = vis_to_float(0x24000C0)

/* *********************************************************** */

#define	DEFINE_KOEFF_ROW_ODD_U8()                               \
	mlib_f32 fone = vis_to_float(0x10101010),               \
	    fnine = vis_to_float(0x90909090)

/* *********************************************************** */

#define	DEFINE_KOEFF_COL_ODD_U8()                                \
	mlib_f32 fone1 = vis_to_float(0x10001000),               \
	    fone_nine = vis_to_float(0x1000900)

/* *********************************************************** */

#define	DEFINE_KOEFF_ROW_EVEN_S16()                             \
/* 111*2 */                                                     \
	mlib_f32 fhund_by_128 = vis_to_float(0xDEDEDEDE),       \
/* 29*2 */                                                      \
	    ftwen_by_128 = vis_to_float(0x3A3A3A3A),            \
/* 9*2 */                                                       \
	    fnine_by_128 = vis_to_float(0x12121212),            \
/* 3*2 */                                                       \
	    fthree_by_128 = vis_to_float(0x06060606),           \
	    four = vis_to_float(0x40004)

/* *********************************************************** */

#define	DEFINE_KOEFF_COL_EVEN_S16()                             \
/* 111 */                                                       \
	mlib_f32 fhund_by_256 = vis_to_float(0x6F6F6F6F),       \
/* 29 */                                                        \
	    ftwen_by_256 = vis_to_float(0x1D1D1D1D),            \
/* 9 */                                                         \
	    fnine_by_256 = vis_to_float(0x09090909),            \
/* 3 */                                                         \
	    fthree_by_256 = vis_to_float(0x03030303)

/* *********************************************************** */

#define	DEFINE_KOEFF_ROW_ODD_S16()                              \
/* 1*16 */                                                      \
	mlib_f32 fone_by_16 = vis_to_float(0x10101010),         \
/* 9*16 */                                                      \
	    fnine_by_16 = vis_to_float(0x90909090),             \
	    four = vis_to_float(0x40004)

/* *********************************************************** */

#define	DEFINE_KOEFF_COL_ODD_S16()                              \
/* 1*128 */                                                     \
	mlib_f32 fone1 = vis_to_float(0x80808080),              \
/* 1*8 */                                                       \
	    fone_by_32 = vis_to_float(0x08080808),              \
/* 9*8 */                                                       \
	    fnine_by_32 = vis_to_float(0x48484848)


/* *********************************************************** */

#define	ROW_BICUBIC_FL0(j, src_type)	ROW_BICUBIC(j, src_type, EVEN, EVEN)
#define	COLUMN_BICUBIC_FL0(j, src_type)	COLUMN_BICUBIC(j, src_type, EVEN, EVEN)

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(j, src_type)	ROW_BICUBIC(j, src_type, ODD, EVEN)
#define	COLUMN_BICUBIC_FL1(j, src_type)	COLUMN_BICUBIC(j, src_type, EVEN, ODD)

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(j, src_type)	ROW_BICUBIC(j, src_type, EVEN, ODD)
#define	COLUMN_BICUBIC_FL2(j, src_type)	COLUMN_BICUBIC(j, src_type, ODD, EVEN)

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(j, src_type)	ROW_BICUBIC(j, src_type, ODD, ODD)
#define	COLUMN_BICUBIC_FL3(j, src_type)	COLUMN_BICUBIC(j, src_type, ODD, ODD)

/* *********************************************************** */

#define	CONST_EVEN	2
#define	CONST_ODD	3

#define	WRITE_GSR_U8(NAME)	vis_write_gsr(CONST_##NAME << 3);

#define	WRITE_GSR_S16(NAME)

/* *********************************************************** */

#define	ROW_BICUBIC(j, src_type, NAME_COL, NAME_ROW)                     \
	{                                                                \
	    mlib_s32 dh = (src_h_beg + j) & 1,                           \
		hx = (j + (src_h_beg & 1)) >> 1,                         \
		dw = (src_w_beg) & 1,                                    \
		jj = hx*src_stride+2;                                    \
	    DEFINE_KOEFF_ROW_##NAME_ROW##_##src_type();                  \
	    DEFINE_KOEFF_COL_##NAME_COL##_##src_type();                  \
	    WRITE_GSR_##src_type(NAME_COL);                              \
	    if (dh == 0) {                                               \
		IMAGE_COL_##src_type##_BC_##NAME_COL##1                  \
		    (hx * src_stride - 1, col0, src_stride);             \
		IMAGE_COL_##src_type##_BC_##NAME_COL##1                  \
		    (hx * src_stride, col1, src_stride);                 \
		IMAGE_COL_##src_type##_BC_##NAME_COL##1                  \
		    (hx * src_stride + 1, col2, src_stride);             \
		i = 0;                                                   \
		if (dw == 1) {                                           \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##1              \
			(jj, col3, src_stride);                          \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2              \
			(i);                                             \
		    col0 = col1;                                         \
		    col1 = col2;                                         \
		    col2 = col3;                                         \
		    i++;                                                 \
		    jj++;                                                \
		}                                                        \
		for (; i < src_width - 1; i += 2) {                      \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##1              \
			(jj, col3, src_stride);                          \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1              \
			(i);                                             \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2              \
			(i + 1);                                         \
		    col0 = col1;                                         \
		    col1 = col2;                                         \
		    col2 = col3;                                         \
		    jj++;                                                \
		}                                                        \
		if (i < src_width) {                                     \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##1              \
			(jj, col3, src_stride);                          \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1              \
			(i);                                             \
		}                                                        \
	    } else {                                                     \
		IMAGE_COL_##src_type##_BC_##NAME_COL##2                  \
		    (hx * src_stride - 1, col0, src_stride);             \
		IMAGE_COL_##src_type##_BC_##NAME_COL##2                  \
		    (hx * src_stride, col1, src_stride);                 \
		IMAGE_COL_##src_type##_BC_##NAME_COL##2                  \
		    (hx * src_stride + 1, col2, src_stride);             \
		i = 0;                                                   \
		if (dw == 1) {                                           \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##2              \
			(jj, col3, src_stride);                          \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2              \
			(i);                                             \
		    col0 = col1;                                         \
		    col1 = col2;                                         \
		    col2 = col3;                                         \
		    i++;                                                 \
		    jj++;                                                \
		}                                                        \
		for (; i < src_width - 1; i += 2) {                      \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##2              \
			(jj, col3, src_stride);                          \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1              \
			(i);                                             \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2              \
			(i + 1);                                         \
		    col0 = col1;                                         \
		    col1 = col2;                                         \
		    col2 = col3;                                         \
		    jj++;                                                \
		}                                                        \
		if (i < src_width) {                                     \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##2              \
			(jj, col3, src_stride);                          \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1              \
			(i);                                             \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC(j, src_type, NAME_COL, NAME_ROW)                 \
	{                                                               \
	    mlib_s32 dh = (src_h_beg) & 1,                              \
		dw = (src_w_beg + j) & 1,                               \
		wx = (j + (src_w_beg & 1)) >> 1,                        \
		jj = wx+2*src_stride;                                   \
	    DEFINE_KOEFF_ROW_##NAME_ROW##_##src_type();                 \
	    DEFINE_KOEFF_COL_##NAME_COL##_##src_type();                 \
	    WRITE_GSR_##src_type(NAME_COL);                             \
	    if (dw == 0) {                                              \
		IMAGE_COL_##src_type##_BC_##NAME_COL##1                 \
		    (wx - src_stride, col0, 1);                         \
		IMAGE_COL_##src_type##_BC_##NAME_COL##1                 \
		    (wx, col1, 1);                                      \
		IMAGE_COL_##src_type##_BC_##NAME_COL##1                 \
		    (wx + src_stride, col2, 1);                         \
		i = 0;                                                  \
		if (dh == 1) {                                          \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##1             \
			(jj, col3, 1);                                  \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2             \
			(i);                                            \
		    col0 = col1;                                        \
		    col1 = col2;                                        \
		    col2 = col3;                                        \
		    i++;                                                \
		    jj += src_stride;                                   \
		}                                                       \
		for (; i < src_height - 1; i += 2) {                    \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##1             \
			(jj, col3, 1);                                  \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1             \
			(i);                                            \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2             \
			(i + 1);                                        \
		    col0 = col1;                                        \
		    col1 = col2;                                        \
		    col2 = col3;                                        \
		    jj += src_stride;                                   \
		}                                                       \
		if (i < src_height) {                                   \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##1             \
			(jj, col3, 1);                                  \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1             \
			(i);                                            \
		}                                                       \
	    } else {                                                    \
		IMAGE_COL_##src_type##_BC_##NAME_COL##2                 \
		    (wx - src_stride, col0, 1);                         \
		IMAGE_COL_##src_type##_BC_##NAME_COL##2                 \
		    (wx, col1, 1);                                      \
		IMAGE_COL_##src_type##_BC_##NAME_COL##2                 \
		    (wx + src_stride, col2, 1);                         \
		i = 0;                                                  \
		if (dh == 1) {                                          \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##2             \
			(jj, col3, 1);                                  \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2             \
			(i);                                            \
		    col0 = col1;                                        \
		    col1 = col2;                                        \
		    col2 = col3;                                        \
		    i++;                                                \
		    jj += src_stride;                                   \
		}                                                       \
		for (; i < src_height - 1; i += 2) {                    \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##2             \
			(jj, col3, 1);                                  \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1             \
			(i);                                            \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##2             \
			(i + 1);                                        \
		    col0 = col1;                                        \
		    col1 = col2;                                        \
		    col2 = col3;                                        \
		    jj += src_stride;                                   \
		}                                                       \
		if (i < src_height) {                                   \
		    IMAGE_COL_##src_type##_BC_##NAME_COL##2             \
			(jj, col3, 1);                                  \
		    IMAGE_ROW_##src_type##_BC_##NAME_ROW##1             \
			(i);                                            \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	DO_UPPER_LEFT_BOUND(SHIFT, src_format, src_type)                 \
	if (src_h_beg & 1) {                                             \
/*                                                                       \
 * upper bound of image                                                  \
 */                                                                      \
	    ROW_BICUBIC_FL##SHIFT(0, src_type);                          \
	    (*p_proc_store) ((src_format *) dst_buf1, pdst, src_width,   \
		colormap);                                               \
	    src_h_beg = 0;                                               \
	    psrc += src_stride;                                          \
	    pdst += dst_stride;                                          \
	    src_height--;                                                \
	}                                                                \
	if (src_w_beg & 1) {                                             \
/*                                                                       \
 * left bound of image                                                   \
 */                                                                      \
	    COLUMN_BICUBIC_FL##SHIFT(0, src_type);                       \
	    (*p_proc_store) ((src_format *) dst_buf1, pdst_buf,          \
		src_height, colormap);                                   \
	    for (k = 0; k < src_height; k++)                             \
		pdst[k * dst_stride] = pdst_buf[k];                      \
	    src_w_beg = 0;                                               \
	    psrc += 1;                                                   \
	    pdst += 1;                                                   \
	    src_width--;                                                 \
	}

/* *********************************************************** */

#define	DO_LOWER_RIGHT_BOUND(SHIFT, src_format, src_type)               \
	if ((src_height & 1) && (src_height > 0)) {                     \
/*                                                                      \
 * lower bound of image                                                 \
 */                                                                     \
	    src_height--;                                               \
	    ROW_BICUBIC_FL##SHIFT(src_height, src_type);                \
	    (*p_proc_store) ((src_format *) dst_buf1,                   \
		pdst + src_height * dst_stride, src_width, colormap);   \
	}                                                               \
	if ((src_width & 1) && (src_width > 0)) {                       \
/*                                                                      \
 * right bound of image                                                 \
 */                                                                     \
	    src_width--;                                                \
	    COLUMN_BICUBIC_FL##SHIFT(src_width, src_type);              \
	    (*p_proc_store) ((src_format *) dst_buf1, pdst_buf,         \
		src_height, colormap);                                  \
	    for (k = 0; k < src_height; k++)                            \
		pdst[k * dst_stride + src_width] = pdst_buf[k];         \
	}

/* *********************************************************** */

#define	DO_COL_U8_BC_ODD(ind, st, st_)                          \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - src_stride]];                     \
	fa1 = lut[psrc[ind + src_stride]];                      \
	fa2 = lut[psrc[ind + 2 * src_stride]];                  \
	st = vis_fmul8x16au(fa, fone1);                         \
	da0 = vis_fmul8x16al(fa, fone_nine);                    \
	da_1 = vis_fmul8x16au(fa_1, fone_nine);                 \
	st_ = vis_fpsub16(da0, da_1);                           \
	da1 = vis_fmul8x16al(fa1, fone_nine);                   \
	st_ = vis_fpadd16(st_, da1);                            \
	da2 = vis_fmul8x16au(fa2, fone_nine);                   \
	st_ = vis_fpsub16(st_, da2)

/* *********************************************************** */

#define	DO_COL_U8_BC_EVEN(ind, st, st_)                         \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - src_stride]];                     \
	fa1 = lut[psrc[ind + src_stride]];                      \
	fa2 = lut[psrc[ind + 2 * src_stride]];                  \
	da0 = vis_fmul8x16au(fa, fhund_twen);                   \
	da_1 = vis_fmul8x16au(fa_1, fnine_three);               \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16al(fa1, fhund_twen);                  \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16al(fa2, fnine_three);                 \
	st = vis_fpsub16(st, da2);                              \
	da0 = vis_fmul8x16al(fa, fhund_twen);                   \
	da_1 = vis_fmul8x16al(fa_1, fnine_three);               \
	st_ = vis_fpsub16(da0, da_1);                           \
	da1 = vis_fmul8x16au(fa1, fhund_twen);                  \
	st_ = vis_fpadd16(st_, da1);                            \
	da2 = vis_fmul8x16au(fa2, fnine_three);                 \
	st_ = vis_fpsub16(st_, da2)

/* *********************************************************** */

#define	DO_ROW_U8_BC_ODD(ind)                                   \
	dst_buf1[ind] = vis_fpack16(col1);                      \
	da0 = vis_fpadd16(col1, col2);                          \
	dst_buf2[ind] = vis_fpack16(col1_);                     \
	da1 = vis_fpadd16(col0, col3);                          \
	da0 = vis_fmul8x16(fnine, da0);                         \
	da1 = vis_fmul8x16(fone, da1);                          \
	da0 = vis_fpsub16(da0, da1);                            \
	da_1 = vis_fpadd16(col1_, col2_);                       \
	dst_buf1[ind + 1] = vis_fpack16(da0);                   \
	da2 = vis_fpadd16(col0_, col3_);                        \
	da_1 = vis_fmul8x16(fnine, da_1);                       \
	da2 = vis_fmul8x16(fone, da2);                          \
	da_1 = vis_fpsub16(da_1, da2);                          \
	dst_buf2[ind + 1] = vis_fpack16(da_1)

/* *********************************************************** */

#define	DO_ROW_U8_BC_EVEN(ind)                                  \
	da0 = vis_fmul8x16(fnine, col0);                        \
	da1 = vis_fmul8x16(fhund, col1);                        \
	da1 = vis_fpsub16(da1, da0);                            \
	da_1 = vis_fmul8x16(ftwen, col2);                       \
	da1 = vis_fpadd16(da1, da_1);                           \
	da2 = vis_fmul8x16(fthree, col3);                       \
	da1 = vis_fpsub16(da1, da2);                            \
	dst_buf1[ind] = vis_fpack16(da1);                       \
	da0 = vis_fmul8x16(fthree, col0);                       \
	da1 = vis_fmul8x16(ftwen, col1);                        \
	da1 = vis_fpsub16(da1, da0);                            \
	da_1 = vis_fmul8x16(fhund, col2);                       \
	da1 = vis_fpadd16(da1, da_1);                           \
	da2 = vis_fmul8x16(fnine, col3);                        \
	da1 = vis_fpsub16(da1, da2);                            \
	dst_buf1[ind + 1] = vis_fpack16(da1);                   \
	da0 = vis_fmul8x16(fnine, col0_);                       \
	da1 = vis_fmul8x16(fhund, col1_);                       \
	da1 = vis_fpsub16(da1, da0);                            \
	da_1 = vis_fmul8x16(ftwen, col2_);                      \
	da1 = vis_fpadd16(da1, da_1);                           \
	da2 = vis_fmul8x16(fthree, col3_);                      \
	da1 = vis_fpsub16(da1, da2);                            \
	dst_buf2[ind] = vis_fpack16(da1);                       \
	da0 = vis_fmul8x16(fthree, col0_);                      \
	da1 = vis_fmul8x16(ftwen, col1_);                       \
	da1 = vis_fpsub16(da1, da0);                            \
	da_1 = vis_fmul8x16(fhund, col2_);                      \
	da1 = vis_fpadd16(da1, da_1);                           \
	da2 = vis_fmul8x16(fnine, col3_);                       \
	da1 = vis_fpsub16(da1, da2);                            \
	dst_buf2[ind + 1] = vis_fpack16(da1)

/* *********************************************************** */

#define	ALLOC_U8()                                                        \
	mlib_s32 size =                                                   \
	    (src_height > src_width ? src_height : src_width);            \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);               \
	mlib_f32 *lut =                                                   \
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) - offset;   \
	    mlib_f32 *dst_buf1, *dst_buf2;                                \
	dst_buf1 =                                                        \
	    (mlib_f32 *)__mlib_malloc((size + src_width +                 \
	    4) * sizeof (mlib_f32));                                      \
	if (dst_buf1 == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	dst_buf2 = dst_buf1 + (size + 2)

/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS_U8()                              \
	mlib_s32 i, j, k;                                       \
/*                                                              \
 * indices                                                      \
 */                                                             \
	mlib_d64 da_1, da0, da1, da2;                           \
	mlib_f32 fa_1, fa, fa1, fa2;                            \
	mlib_d64 col0, col1, col2, col3;                        \
	mlib_d64 col0_, col1_, col2_, col3_

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_U8();
	DEFINE_KOEFF_COL_EVEN_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_4;
	}

	DO_UPPER_LEFT_BOUND(0, mlib_u8, U8);

	vis_write_gsr(2 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_U8_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_U8_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(0, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_U8();
	DEFINE_KOEFF_COL_ODD_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_4;
	}

	DO_UPPER_LEFT_BOUND(1, mlib_u8, U8);

	vis_write_gsr(3 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_U8_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_ODD(i * src_stride + (j + 2), col3, col3_);
			DO_ROW_U8_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(1, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_U8();
	DEFINE_KOEFF_COL_EVEN_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_4;
	}

	DO_UPPER_LEFT_BOUND(2, mlib_u8, U8);

	vis_write_gsr(2 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_U8_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_U8_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(2, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_U8();
	DEFINE_KOEFF_COL_ODD_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_U8_4;
	}

	DO_UPPER_LEFT_BOUND(3, mlib_u8, U8);

	vis_write_gsr(3 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_U8_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_ODD(i * src_stride + (j + 2), col3, col3_);
			DO_ROW_U8_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(3, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_U8();
	DEFINE_KOEFF_COL_EVEN_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_4;
	}

	DO_UPPER_LEFT_BOUND(0, mlib_u8, U8);

	vis_write_gsr(2 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_U8_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_U8_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(0, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_U8();
	DEFINE_KOEFF_COL_ODD_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_4;
	}

	DO_UPPER_LEFT_BOUND(1, mlib_u8, U8);

	vis_write_gsr(3 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_U8_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_ODD(i * src_stride + (j + 2), col3, col3_);
			DO_ROW_U8_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(1, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_U8();
	DEFINE_KOEFF_COL_EVEN_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_4;
	}

	DO_UPPER_LEFT_BOUND(2, mlib_u8, U8);

	vis_write_gsr(2 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_U8_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_U8_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(2, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_U8();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_U8();
	DEFINE_KOEFF_COL_ODD_U8();
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_U8();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_U8_S16_4;
	}

	DO_UPPER_LEFT_BOUND(3, mlib_u8, U8);

	vis_write_gsr(3 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_U8_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_U8_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_U8_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_U8_BC_ODD(i * src_stride + (j + 2), col3, col3_);
			DO_ROW_U8_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,
		    2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_u8 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(3, mlib_u8, U8);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	DO_COL_S16_BC_ODD(ind, st, st_)                         \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - src_stride]];                     \
	fa1 = lut[psrc[ind + src_stride]];                      \
	fa2 = lut[psrc[ind + 2 * src_stride]];                  \
	st = vis_fmul8x16(fone1, fa);                           \
	da_1 = vis_fmul8x16(fone_by_32, fa_1);                  \
	da0 = vis_fmul8x16(fnine_by_32, fa);                    \
	st_ = vis_fpsub16(da0, da_1);                           \
	da1 = vis_fmul8x16(fnine_by_32, fa1);                   \
	st_ = vis_fpadd16(st_, da1);                            \
	da2 = vis_fmul8x16(fone_by_32, fa2);                    \
	st_ = vis_fpsub16(st_, da2)

/* *********************************************************** */

#define	DO_COL_S16_BC_EVEN(ind, st, st_)                        \
	fa = lut[psrc[ind]];                                    \
	fa_1 = lut[psrc[ind - src_stride]];                     \
	fa1 = lut[psrc[ind + src_stride]];                      \
	fa2 = lut[psrc[ind + 2 * src_stride]];                  \
	da_1 = vis_fmul8x16(fnine_by_256, fa_1);                \
	da0 = vis_fmul8x16(fhund_by_256, fa);                   \
	st = vis_fpsub16(da0, da_1);                            \
	da1 = vis_fmul8x16(ftwen_by_256, fa1);                  \
	st = vis_fpadd16(st, da1);                              \
	da2 = vis_fmul8x16(fthree_by_256, fa2);                 \
	st = vis_fpsub16(st, da2);                              \
	da_1 = vis_fmul8x16(fthree_by_256, fa_1);               \
	da0 = vis_fmul8x16(ftwen_by_256, fa);                   \
	st_ = vis_fpsub16(da0, da_1);                           \
	da1 = vis_fmul8x16(fhund_by_256, fa1);                  \
	st_ = vis_fpadd16(st_, da1);                            \
	da2 = vis_fmul8x16(fnine_by_256, fa2);                  \
	st_ = vis_fpsub16(st_, da2)

/* *********************************************************** */

#define	DO_ROW_S16_BC_ODD(ind)                                  \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(col1));         \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(col1));         \
	dst_buf1[ind] = vis_fpackfix_pair(da0, da1);            \
	da0 = vis_fmul8x16(fnine_by_16, col1);                  \
	da1 = vis_fmul8x16(fnine_by_16, col2);                  \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fone_by_16, col0);                  \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fone_by_16, col3);                   \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf1[ind + 1] = vis_fpackfix_pair(da0, da1);        \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(col1_));        \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(col1_));        \
	dst_buf2[ind] = vis_fpackfix_pair(da0, da1);            \
	da0 = vis_fmul8x16(fnine_by_16, col1_);                 \
	da1 = vis_fmul8x16(fnine_by_16, col2_);                 \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fone_by_16, col0_);                 \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fone_by_16, col3_);                  \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf2[ind + 1] = vis_fpackfix_pair(da0, da1)

/* *********************************************************** */

#define	DO_ROW_S16_BC_EVEN(ind)                                 \
	da0 = vis_fmul8x16(fhund_by_128, col1);                 \
	da1 = vis_fmul8x16(ftwen_by_128, col2);                 \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fnine_by_128, col0);                \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fthree_by_128, col3);                \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf1[ind] = vis_fpackfix_pair(da0, da1);            \
	da0 = vis_fmul8x16(ftwen_by_128, col1);                 \
	da1 = vis_fmul8x16(fhund_by_128, col2);                 \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fthree_by_128, col0);               \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fnine_by_128, col3);                 \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf1[ind + 1] = vis_fpackfix_pair(da0, da1);        \
	da0 = vis_fmul8x16(fhund_by_128, col1_);                \
	da1 = vis_fmul8x16(ftwen_by_128, col2_);                \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fnine_by_128, col0_);               \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fthree_by_128, col3_);               \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf2[ind] = vis_fpackfix_pair(da0, da1);            \
	da0 = vis_fmul8x16(ftwen_by_128, col1_);                \
	da1 = vis_fmul8x16(fhund_by_128, col2_);                \
	da0 = vis_fpadd16(da0, da1);                            \
	da_1 = vis_fmul8x16(fthree_by_128, col0_);              \
	da0 = vis_fpsub16(da0, da_1);                           \
	da2 = vis_fmul8x16(fnine_by_128, col3_);                \
	da0 = vis_fpsub16(da0, da2);                            \
	da1 = vis_fmuld8ulx16(four, vis_read_lo(da0));          \
	da0 = vis_fmuld8ulx16(four, vis_read_hi(da0));          \
	dst_buf2[ind + 1] = vis_fpackfix_pair(da0, da1)

/* *********************************************************** */

#define	ALLOC_S16()                                                       \
	mlib_s32 size =                                                   \
	    (src_height > src_width ? src_height : src_width);            \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);               \
	mlib_d64 *lut =                                                   \
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) - offset;   \
	    mlib_d64 *                                                    \
	    dst_buf1, *                                                   \
		dst_buf2;                                                 \
	dst_buf1 =                                                        \
	    (mlib_d64 *)__mlib_malloc((size + src_width +                 \
	    4) * sizeof (mlib_d64));                                      \
	if (dst_buf1 == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	dst_buf2 = dst_buf1 + (size + 2)

/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS_S16()                             \
	mlib_s32 i, j, k;                                       \
/*                                                              \
 * indices                                                      \
 */                                                             \
	mlib_d64 da_1, da0, da1, da2;                           \
	mlib_d64 fa_1, fa, fa1, fa2;                            \
	mlib_d64 col0, col1, col2, col3;                        \
	mlib_d64 col0_, col1_, col2_, col3_

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_S16();
	DEFINE_KOEFF_COL_EVEN_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(0, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_S16_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(0, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_S16();
	DEFINE_KOEFF_COL_ODD_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(1, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_S16_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_ODD(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(1, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_S16();
	DEFINE_KOEFF_COL_EVEN_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(2, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_S16_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(2, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_u8 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_S16();
	DEFINE_KOEFF_COL_ODD_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_u8 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_U8_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(3, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_S16_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_ODD(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(3, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_S16();
	DEFINE_KOEFF_COL_EVEN_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(0, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_S16_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(0, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_EVEN_S16();
	DEFINE_KOEFF_COL_ODD_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(1, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_S16_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_ODD(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_EVEN(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(1, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_S16();
	DEFINE_KOEFF_COL_EVEN_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(2, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_EVEN(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_EVEN(i * src_stride, col1, col1_);
		DO_COL_S16_BC_EVEN(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_EVEN(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(2, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16),
    mlib_s32 chan)
{
	DEFINE_IDENTIFICATORS_S16();
	mlib_s16 *pdst_buf;

	DEFINE_KOEFF_ROW_ODD_S16();
	DEFINE_KOEFF_COL_ODD_S16();
	void (
	    *p_proc_store) (
	    const mlib_s16 *,
	    mlib_s16 *,
	    mlib_s32,
	    const void *);

	ALLOC_S16();

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL) {
		__mlib_free(dst_buf1);
		return (MLIB_FAILURE);
	}

	if (chan == 3) {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4;
	} else {
		p_proc_store = &mlib_ImageColorTrue2IndexLine_S16_S16_4;
	}

	vis_write_gsr(15 << 3);
	DO_UPPER_LEFT_BOUND(3, mlib_s16, S16);

#pragma pipeloop(0)
	for (i = 0; i < (src_height / 2); i++) {
		DO_COL_S16_BC_ODD(i * src_stride - 1, col0, col0_);
		DO_COL_S16_BC_ODD(i * src_stride, col1, col1_);
		DO_COL_S16_BC_ODD(i * src_stride + 1, col2, col2_);

#pragma pipeloop(0)
		for (j = 0; j < (src_width / 2); j++) {
			DO_COL_S16_BC_ODD(i * src_stride + (j + 2), col3,
			    col3_);
			DO_ROW_S16_BC_ODD(2 * j);
			col0 = col1;
			col1 = col2;
			col2 = col3;
			col0_ = col1_;
			col1_ = col2_;
			col2_ = col3_;
		}

		(*p_proc_store) ((mlib_s16 *)dst_buf1,
		    pdst + 2 * i * dst_stride, 2 * (src_width / 2), colormap);
		(*p_proc_store) ((mlib_s16 *)dst_buf2,
		    pdst + (2 * i + 1) * dst_stride, 2 * (src_width / 2),
		    colormap);
	}

	DO_LOWER_RIGHT_BOUND(3, mlib_s16, S16);

	__mlib_free(dst_buf1);
	__mlib_free(pdst_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_IMAGEZOOMIN2XINDEX_BC(dst_format, dst_type, SHIFT)           \
	mlib_s32 lut_type = mlib_ImageGetLutType(colormap);               \
	mlib_s32 nchannels = mlib_ImageGetLutChannels(colormap);          \
	mlib_status res;                                                  \
	                                                                  \
	if ((nchannels != 3) && (nchannels != 4))                         \
	    return (MLIB_FAILURE);                                        \
	switch (lut_type) {                                               \
	case MLIB_BYTE:                                                   \
	    res =                                                         \
		mlib_v_ImageZoomIn2XIndex_U8_##dst_type##_BC_FL##SHIFT    \
		(MLIB_ZOOMIN2XINDEXCALLPARAM(dst_format), nchannels);     \
	    break;                                                        \
	case MLIB_SHORT:                                                  \
	    res =                                                         \
		mlib_v_ImageZoomIn2XIndex_S16_##dst_type##_BC_FL##SHIFT   \
		(MLIB_ZOOMIN2XINDEXCALLPARAM(dst_format), nchannels);     \
	    break;                                                        \
	}                                                                 \
	                                                                  \
	return res

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_BC_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    0);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_BC_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    1);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_BC_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    2);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_U8_BC_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    3);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_BC_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    0);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_BC_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    1);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_BC_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    2);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIn2XIndex_S16_BC_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    3);
}

/* *********************************************************** */
