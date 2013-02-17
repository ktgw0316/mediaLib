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

#pragma ident	"@(#)mlib_v_GraphicsFillPolygon_G.c	9.3	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_G_[8 | 32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *    mlib_GraphicsFillPolygon_BG_[8 | 32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1) with alpha blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_G_[8 | 32]
 *                                      (mlib_image *buffer,
 *                                       mlib_s16   *x,
 *                                       mlib_s16   *y,
 *                                       mlib_s32   npoints,
 *                                       mlib_s32   *c);
 *    mlib_status mlib_GraphicsFillPolygon_BG_[8 | 32]
 *                                      (mlib_image *buffer,
 *                                       mlib_s16   *x,
 *                                       mlib_s16   *y,
 *                                       mlib_s32   npoints,
 *                                       mlib_s32   *c,
 *					 mlib_s32   alpha);
 *
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Pointer to array of color of the vertices.
 *    alpha        Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0 - 7 are used and the bits 8 - 23 should be zero. For 32-bit
 *    frame buffer, the bits 24 - 31 are reserved for alpha channel and
 *    should be 0xff.
 *    The shade of any point inside a polygon is determined by linear
 *    interpolation of vertex intensities along each edge and then between
 *    edges along each scanline.
 */

#include <mlib_graphics.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_G_32 = __mlib_GraphicsFillPolygon_G_32
#pragma weak mlib_GraphicsFillPolygon_G_8 = __mlib_GraphicsFillPolygon_G_8
#pragma weak mlib_GraphicsFillPolygon_BG_32 = \
	__mlib_GraphicsFillPolygon_BG_32
#pragma weak mlib_GraphicsFillPolygon_BG_8 = __mlib_GraphicsFillPolygon_BG_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_G_32) mlib_GraphicsFillPolygon_G_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_G_32")));
__typeof__(__mlib_GraphicsFillPolygon_G_8) mlib_GraphicsFillPolygon_G_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_G_8")));
__typeof__(__mlib_GraphicsFillPolygon_BG_32) mlib_GraphicsFillPolygon_BG_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BG_32")));
__typeof__(__mlib_GraphicsFillPolygon_BG_8) mlib_GraphicsFillPolygon_BG_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BG_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 c;

} SpanDataG8, *SpanDataG8Ptr;

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 r;
	mlib_s32 g;
	mlib_s32 b;

} SpanDataG32, *SpanDataG32Ptr;

/* *********************************************************** */

static mlib_s32 mlib_CountExtremaAndMinMax_G(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax);

static void mlib_ComputeSpansData_G8(
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG8Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans);

static void mlib_ComputeSpansData_G32(
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG32Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans);

/* *********************************************************** */

#define	_MLIB_G_FPACK(_a, _b, _c)                               \
	{                                                       \
	                                                        \
/*                                                              \
 * _c = vis_freg_pair(vis_fpack16(_a), vis_fpack16(_b));        \
 */                                                             \
	    _c = vis_fpack16_pair((_a), (_b));                  \
	}

/* *********************************************************** */

#define	_MLIB_G_FPADD(_a, _b, _c)                               \
	_a = vis_fpadd16((_a), (_c));                           \
	_b = vis_fpadd16((_b), (_c))

/* *********************************************************** */

#define	_MLIB_G_FPSUB(_a, _b, _c)                               \
	_a = vis_fpsub16((_a), (_c));                           \
	_b = vis_fpsub16((_b), (_c))

/* *********************************************************** */

#define	_MLIB_GOR_GORIZONTAL_LINE                               \
	* _wp_pdst_row = _w_data;                               \
	*_wp_pdst_row_end = _w_data_end;                        \
	_wp_pdst_row++;                                         \
	_wp_pdst_row_end--;                                     \
	_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                 \
	_MLIB_G_FPACK(_w_tc_0, _w_tc_1, _w_data);               \
	_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                 \
	_MLIB_G_FPACK(_w_tc_2, _w_tc_3, _w_data_end)

/*
 * the cb and ce - the begin and end colors are 16.16 mlib_s32 values
 */

/* *********************************************************** */

#define	MLIB_SHIFT_TO_7	9
#define	MASK	0xFFFF
#define	MLIB_FILL_ROW_G_8(prow, xb, xe, cb, ce)                        \
	{                                                              \
	    mlib_u8 *_pdst_row = (prow) + (xb);                        \
	    mlib_u8 *_pdst_row_end = (prow) + (xe);                    \
	    mlib_s32 _i, _w, _emask, _dc_2, _tc_0, _dc =               \
		((ce) - (cb)) / (mlib_f32)((xe) - (xb));               \
	    mlib_s32 _color = (cb), _color_end = (ce);                 \
	    mlib_f32 _f_tc_0, _f_tc_1, _f_dc;                          \
	    mlib_d64 _w_data, _w_data_end, _w_tc_0, _w_tc_1;           \
	    mlib_d64 _w_tc_2, _w_tc_3, _w_dc, _d_dc;                   \
	    mlib_d64 *_wp_pdst_row, *_wp_pdst_row_end;                 \
	                                                               \
	    _dc &= ((xb) - (xe)) >> 31;                                \
	    _color -= ((mlib_addr)(_pdst_row) & 7) * _dc;              \
	    _color_end -= ((mlib_addr)(_pdst_row_end) & 7) * _dc;      \
	    _color = _color >> MLIB_SHIFT_TO_7;                        \
	    _color_end = _color_end >> MLIB_SHIFT_TO_7;                \
	    _dc = _dc >> MLIB_SHIFT_TO_7;                              \
	    _dc_2 = _dc << 1;                                          \
	    _f_dc = vis_to_float((_dc_2 << 16) + (_dc_2 & MASK));      \
	    _tc_0 = _color << 16;                                      \
	    _color += _dc;                                             \
	    _tc_0 += _color & MASK;                                    \
	    _f_tc_0 = vis_to_float(_tc_0);                             \
	    _f_tc_1 = vis_fpadd16s(_f_tc_0, _f_dc);                    \
	    _w_tc_0 = vis_freg_pair(_f_tc_0, _f_tc_1);                 \
	    _d_dc =                                                    \
		vis_freg_pair(vis_fpadd16s(_f_dc, _f_dc),              \
		vis_fpadd16s(_f_dc, _f_dc));                           \
	    _w_tc_1 = vis_fpadd16(_w_tc_0, _d_dc);                     \
	    _wp_pdst_row = vis_alignaddr(_pdst_row, 0);                \
	    _wp_pdst_row_end = vis_alignaddr(_pdst_row_end, 0);        \
	    _w = (_wp_pdst_row_end - _wp_pdst_row) - 1;                \
	    _MLIB_G_FPACK(_w_tc_0, _w_tc_1, _w_data);                  \
	    _emask = vis_edge8(_pdst_row, _pdst_row_end);              \
	    vis_pst_8(_w_data, _wp_pdst_row, _emask);                  \
	    if (_w != -1) {                                            \
		_tc_0 = _color_end << 16;                              \
		_color_end += _dc;                                     \
		_tc_0 += _color_end & MASK;                            \
		_f_tc_0 = vis_to_float(_tc_0);                         \
		_f_tc_1 = vis_fpadd16s(_f_tc_0, _f_dc);                \
		_w_tc_2 = vis_freg_pair(_f_tc_0, _f_tc_1);             \
		_w_tc_3 = vis_fpadd16(_w_tc_2, _d_dc);                 \
		_w_dc = vis_fpadd16(_d_dc, _d_dc);                     \
		_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                \
		_MLIB_G_FPACK(_w_tc_2, _w_tc_3, _w_data_end);          \
		_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                \
		_emask = vis_edge8(_wp_pdst_row_end, _pdst_row_end);   \
		vis_pst_8(_w_data_end, _wp_pdst_row_end, _emask);      \
		_wp_pdst_row++;                                        \
		_wp_pdst_row_end--;                                    \
		_MLIB_G_FPACK(_w_tc_0, _w_tc_1, _w_data);              \
		_MLIB_G_FPACK(_w_tc_2, _w_tc_3, _w_data_end);          \
		for (_i = 0; _i < (_w >> 3); _i++) {                   \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		}                                                      \
		if ((_w & 4) != 0) {                                   \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		}                                                      \
		if ((_w & 2) != 0) {                                   \
		    _MLIB_GOR_GORIZONTAL_LINE;                         \
		}                                                      \
		if ((_w & 1) != 0)                                     \
		    *_wp_pdst_row = _w_data;                           \
	    }                                                          \
	}

/* *********************************************************** */

#define	_MLIB_BG_FPACK(psrc, c0, c1, ddst)                      \
	dld = *psrc;                                            \
	m00 = vis_fmul8x16(vis_read_hi(dld), donema);           \
	m01 = vis_fmul8x16(fa, c0);                             \
	sum0 = vis_fpadd16(m00, m01);                           \
	m10 = vis_fmul8x16(vis_read_lo(dld), donema);           \
	m11 = vis_fmul8x16(fa, c1);                             \
	sum1 = vis_fpadd16(m10, m11);                           \
	ddst = vis_fpack16_pair(sum0, sum1);

/* *********************************************************** */

#define	_MLIB_GOR_GORIZONTAL_LINE_BG                                      \
	* _wp_pdst_row = _w_data;                                         \
	*_wp_pdst_row_end = _w_data_end;                                  \
	_wp_pdst_row++;                                                   \
	_wp_pdst_row_end--;                                               \
	_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                           \
	_MLIB_BG_FPACK(_wp_pdst_row, _w_tc_0, _w_tc_1, _w_data);          \
	_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                           \
	_MLIB_BG_FPACK(_wp_pdst_row_end, _w_tc_2, _w_tc_3, _w_data_end)

/* *********************************************************** */

#define	MLIB_FILL_ROW_BG_8(prow, xb, xe, cb, ce)                           \
	{                                                                  \
	    mlib_u8 *_pdst_row = (prow) + (xb);                            \
	    mlib_u8 *_pdst_row_end = (prow) + (xe);                        \
	    mlib_s32 _i, _w, _emask, _dc_2, _tc_0, _dc =                   \
		((ce) - (cb)) / (mlib_f32)((xe) - (xb));                   \
	    mlib_s32 _color = (cb), _color_end = (ce);                     \
	    mlib_f32 _f_tc_0, _f_tc_1, _f_dc, fa;                          \
	    mlib_d64 _w_data, _w_data_end, _w_tc_0, _w_tc_1;               \
	    mlib_d64 _w_tc_2, _w_tc_3, _w_dc, _d_dc;                       \
	    mlib_d64 *_wp_pdst_row, *_wp_pdst_row_end;                     \
	    mlib_s32 sa, sonema;                                           \
	    mlib_d64 donema, dld, m00, m01, m10, m11, sum0, sum1;          \
	                                                                   \
	    sa = alpha & 0xFF;                                             \
	    sonema = ~alpha & 0xFF;                                        \
	    sa = (sa << 24) | (sa << 16) | (sa << 8) | sa;                 \
	    fa = vis_to_float(sa);                                         \
	    donema = vis_to_double_dup((sonema << 23) | (sonema << 7));    \
	    _dc &= ((xb) - (xe)) >> 31;                                    \
	    _color -= ((mlib_addr)(_pdst_row) & 7) * _dc;                  \
	    _color_end -= ((mlib_addr)(_pdst_row_end) & 7) * _dc;          \
	    _color = _color >> MLIB_SHIFT_TO_7;                            \
	    _color_end = _color_end >> MLIB_SHIFT_TO_7;                    \
	    _dc = _dc >> MLIB_SHIFT_TO_7;                                  \
	    _dc_2 = _dc << 1;                                              \
	    _f_dc = vis_to_float((_dc_2 << 16) + (_dc_2 & MASK));          \
	    _tc_0 = _color << 16;                                          \
	    _color += _dc;                                                 \
	    _tc_0 += _color & MASK;                                        \
	    _f_tc_0 = vis_to_float(_tc_0);                                 \
	    _f_tc_1 = vis_fpadd16s(_f_tc_0, _f_dc);                        \
	    _w_tc_0 = vis_freg_pair(_f_tc_0, _f_tc_1);                     \
	    _d_dc =                                                        \
		vis_freg_pair(vis_fpadd16s(_f_dc, _f_dc),                  \
		vis_fpadd16s(_f_dc, _f_dc));                               \
	    _w_tc_1 = vis_fpadd16(_w_tc_0, _d_dc);                         \
	    _wp_pdst_row = vis_alignaddr(_pdst_row, 0);                    \
	    _wp_pdst_row_end = vis_alignaddr(_pdst_row_end, 0);            \
	    _w = (_wp_pdst_row_end - _wp_pdst_row) - 1;                    \
	    _MLIB_BG_FPACK(_wp_pdst_row, _w_tc_0, _w_tc_1, _w_data);       \
	    _emask = vis_edge8(_pdst_row, _pdst_row_end);                  \
	    vis_pst_8(_w_data, _wp_pdst_row, _emask);                      \
	    if (_w != -1) {                                                \
		_tc_0 = _color_end << 16;                                  \
		_color_end += _dc;                                         \
		_tc_0 += _color_end & MASK;                                \
		_f_tc_0 = vis_to_float(_tc_0);                             \
		_f_tc_1 = vis_fpadd16s(_f_tc_0, _f_dc);                    \
		_w_tc_2 = vis_freg_pair(_f_tc_0, _f_tc_1);                 \
		_w_tc_3 = vis_fpadd16(_w_tc_2, _d_dc);                     \
		_w_dc = vis_fpadd16(_d_dc, _d_dc);                         \
		_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                    \
		_MLIB_BG_FPACK(_wp_pdst_row_end, _w_tc_2, _w_tc_3,         \
		    _w_data_end);                                          \
		_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                    \
		_emask = vis_edge8(_wp_pdst_row_end, _pdst_row_end);       \
		vis_pst_8(_w_data_end, _wp_pdst_row_end, _emask);          \
		_wp_pdst_row++;                                            \
		_wp_pdst_row_end--;                                        \
		_MLIB_BG_FPACK(_wp_pdst_row, _w_tc_0, _w_tc_1, _w_data);   \
		_MLIB_BG_FPACK(_wp_pdst_row_end, _w_tc_2, _w_tc_3,         \
		    _w_data_end);                                          \
		for (_i = 0; _i < (_w >> 3); _i++) {                       \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		}                                                          \
		if ((_w & 4) != 0) {                                       \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		}                                                          \
		if ((_w & 2) != 0) {                                       \
		    _MLIB_GOR_GORIZONTAL_LINE_BG;                          \
		}                                                          \
		if ((_w & 1) != 0)                                         \
		    *_wp_pdst_row = _w_data;                               \
	    }                                                              \
	}

/* *********************************************************** */

/*
 * all colors are  mlib_d64 values
 * d1 = 16.16 16.16
 * alpha blue
 * d2 = 16.16 16.16
 * green  red
 */
#define	MLIB_FILL_ROW_G_32(prow, xb, xe, d1b, d2b, d1e, d2e)            \
	{                                                               \
	    typedef union                                               \
	    {                                                           \
		mlib_d64 _value;                                        \
		struct                                                  \
		{                                                       \
		    mlib_s32 _int0, _int1;                              \
		}                                                       \
		_twoint;                                                \
	    }                                                           \
	    type_mlib_d64;                                              \
	    mlib_s32 *_pdst_row = (prow) + (xb);                        \
	    mlib_s32 *_pdst_row_end = (prow) + (xe);                    \
	    mlib_s32 _i, _w, _emask;                                    \
	    mlib_f32 _oneonw = 1 / (mlib_f32)((xe) - (xb));             \
	    mlib_d64 _w_data, _w_data_end, _w_tc_0, _w_tc_1;            \
	    mlib_d64 _w_tc_2, _w_tc_3, _w_dc;                           \
	    mlib_d64 *_wp_pdst_row, *_wp_pdst_row_end, _d_dc;           \
	    type_mlib_d64 _str1, _str2;                                 \
	                                                                \
	    vis_write_gsr(7 << 3);                                      \
	    _w_tc_0 = vis_fpackfix_pair((d1b), (d2b));                  \
	    _w_tc_2 = vis_fpackfix_pair((d1e), (d2e));                  \
	    _str1._value = vis_fpsub32((d1e), (d1b));                   \
	    _str2._value = vis_fpsub32((d2e), (d2b));                   \
	    _str1._twoint._int1 =                                       \
		(mlib_f32)_str1._twoint._int1 * _oneonw;                \
	    _str2._twoint._int0 =                                       \
		(mlib_f32)_str2._twoint._int0 * _oneonw;                \
	    _str2._twoint._int1 =                                       \
		(mlib_f32)_str2._twoint._int1 * _oneonw;                \
	    _d_dc = vis_fpackfix_pair(_str1._value, _str2._value);      \
	    vis_write_gsr(0);                                           \
	    if (((mlib_addr)(_pdst_row) & 7) != 0) {                    \
		_w_tc_1 = _w_tc_0;                                      \
		_w_tc_0 = vis_fpsub16(_w_tc_0, _d_dc);                  \
	    } else {                                                    \
		_w_tc_1 = vis_fpadd16(_w_tc_0, _d_dc);                  \
	    }                                                           \
	    _wp_pdst_row = vis_alignaddr(_pdst_row, 0);                 \
	    _wp_pdst_row_end = vis_alignaddr(_pdst_row_end, 0);         \
	    _w = (_wp_pdst_row_end - _wp_pdst_row) - 1;                 \
	    _MLIB_G_FPACK(_w_tc_0, _w_tc_1, _w_data);                   \
	    _emask = vis_edge32(_pdst_row, _pdst_row_end);              \
	    vis_pst_32(_w_data, _wp_pdst_row, _emask);                  \
	    if (_w != -1) {                                             \
		if (((mlib_addr)(_pdst_row_end) & 7) != 0) {            \
		    _w_tc_3 = _w_tc_2;                                  \
		    _w_tc_2 = vis_fpsub16(_w_tc_2, _d_dc);              \
		} else {                                                \
		    _w_tc_3 = vis_fpadd16(_w_tc_2, _d_dc);              \
		}                                                       \
		_w_dc = vis_fpadd16(_d_dc, _d_dc);                      \
		_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                 \
		_MLIB_G_FPACK(_w_tc_2, _w_tc_3, _w_data_end);           \
		_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                 \
		_emask = vis_edge32(_wp_pdst_row_end, _pdst_row_end);   \
		vis_pst_32(_w_data_end, _wp_pdst_row_end, _emask);      \
		_wp_pdst_row++;                                         \
		_wp_pdst_row_end--;                                     \
		_MLIB_G_FPACK(_w_tc_0, _w_tc_1, _w_data);               \
		_MLIB_G_FPACK(_w_tc_2, _w_tc_3, _w_data_end);           \
		for (_i = 0; _i < (_w >> 3); _i++) {                    \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		}                                                       \
		if ((_w & 4) != 0) {                                    \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		}                                                       \
		if ((_w & 2) != 0) {                                    \
		    _MLIB_GOR_GORIZONTAL_LINE;                          \
		}                                                       \
		if ((_w & 1) != 0)                                      \
		    *_wp_pdst_row = _w_data;                            \
	    }                                                           \
	}

/* *********************************************************** */

#define	_MLIB_BG32_FPACK(psrc, c0, c1, ddst)                    \
	dld = *psrc;                                            \
	m00 = vis_fmul8x16(vis_read_hi(dld), donema);           \
	m01 = vis_fmul8x16(fa, c0);                             \
	sum0 = vis_fpadd16(m00, m01);                           \
	m10 = vis_fmul8x16(vis_read_lo(dld), donema);           \
	m11 = vis_fmul8x16(fa, c1);                             \
	sum1 = vis_fpadd16(m10, m11);                           \
	ddst = vis_fpack16_pair(sum0, sum1);                    \
	ddst = vis_for(ddst, dalpha);

/* *********************************************************** */

#define	_MLIB_GOR_GORIZONTAL_LINE_BG32                               \
	* _wp_pdst_row = _w_data;                                    \
	*_wp_pdst_row_end = _w_data_end;                             \
	_wp_pdst_row++;                                              \
	_wp_pdst_row_end--;                                          \
	_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                      \
	_MLIB_BG32_FPACK(_wp_pdst_row, _w_tc_0, _w_tc_1, _w_data);   \
	_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                      \
	_MLIB_BG32_FPACK(_wp_pdst_row_end, _w_tc_2, _w_tc_3,         \
	    _w_data_end)

/* *********************************************************** */

#define	MLIB_FILL_ROW_BG_32(prow, xb, xe, d1b, d2b, d1e, d2e)            \
	{                                                                \
	    typedef union                                                \
	    {                                                            \
		mlib_d64 _value;                                         \
		struct                                                   \
		{                                                        \
		    mlib_s32 _int0, _int1;                               \
		}                                                        \
		_twoint;                                                 \
	    }                                                            \
	    type_mlib_d64;                                               \
	    mlib_s32 *_pdst_row = (prow) + (xb);                         \
	    mlib_s32 *_pdst_row_end = (prow) + (xe);                     \
	    mlib_s32 _i, _w, _emask;                                     \
	    mlib_f32 _oneonw = 1 / (mlib_f32)((xe) - (xb)), fa;          \
	    mlib_d64 _w_data, _w_data_end, _w_tc_0, _w_tc_1;             \
	    mlib_d64 _w_tc_2, _w_tc_3, _w_dc;                            \
	    mlib_d64 *_wp_pdst_row, *_wp_pdst_row_end, _d_dc;            \
	    type_mlib_d64 _str1, _str2;                                  \
	    mlib_s32 sa, sonema;                                         \
	    mlib_d64 donema, dld, m00, m01, m10, m11, sum0, sum1;        \
	    mlib_d64 dalpha = vis_to_double_dup(0xFF000000);             \
	                                                                 \
	    sa = alpha & 0xFF;                                           \
	    sonema = ~alpha & 0xFF;                                      \
	    sa = (sa << 16) | (sa << 8) | sa;                            \
	    fa = vis_to_float(sa);                                       \
	    donema =                                                     \
		vis_to_double((sonema << 7),                             \
		(sonema << 23) | (sonema << 7));                         \
	    vis_write_gsr(7 << 3);                                       \
	    _w_tc_0 = vis_fpackfix_pair((d1b), (d2b));                   \
	    _w_tc_2 = vis_fpackfix_pair((d1e), (d2e));                   \
	    _str1._value = vis_fpsub32((d1e), (d1b));                    \
	    _str2._value = vis_fpsub32((d2e), (d2b));                    \
	    _str1._twoint._int1 =                                        \
		(mlib_f32)_str1._twoint._int1 * _oneonw;                 \
	    _str2._twoint._int0 =                                        \
		(mlib_f32)_str2._twoint._int0 * _oneonw;                 \
	    _str2._twoint._int1 =                                        \
		(mlib_f32)_str2._twoint._int1 * _oneonw;                 \
	    _d_dc = vis_fpackfix_pair(_str1._value, _str2._value);       \
	    vis_write_gsr(0);                                            \
	    if (((mlib_addr)(_pdst_row) & 7) != 0) {                     \
		_w_tc_1 = _w_tc_0;                                       \
		_w_tc_0 = vis_fpsub16(_w_tc_0, _d_dc);                   \
	    } else {                                                     \
		_w_tc_1 = vis_fpadd16(_w_tc_0, _d_dc);                   \
	    }                                                            \
	    _wp_pdst_row = vis_alignaddr(_pdst_row, 0);                  \
	    _wp_pdst_row_end = vis_alignaddr(_pdst_row_end, 0);          \
	    _w = (_wp_pdst_row_end - _wp_pdst_row) - 1;                  \
	    _MLIB_BG32_FPACK(_wp_pdst_row, _w_tc_0, _w_tc_1, _w_data);   \
	    _emask = vis_edge32(_pdst_row, _pdst_row_end);               \
	    vis_pst_32(_w_data, _wp_pdst_row, _emask);                   \
	    if (_w != -1) {                                              \
		if (((mlib_addr)(_pdst_row_end) & 7) != 0) {             \
		    _w_tc_3 = _w_tc_2;                                   \
		    _w_tc_2 = vis_fpsub16(_w_tc_2, _d_dc);               \
		} else {                                                 \
		    _w_tc_3 = vis_fpadd16(_w_tc_2, _d_dc);               \
		}                                                        \
		_w_dc = vis_fpadd16(_d_dc, _d_dc);                       \
		_MLIB_G_FPADD(_w_tc_0, _w_tc_1, _w_dc);                  \
		_MLIB_BG32_FPACK(_wp_pdst_row_end, _w_tc_2, _w_tc_3,     \
		    _w_data_end);                                        \
		_MLIB_G_FPSUB(_w_tc_2, _w_tc_3, _w_dc);                  \
		_emask = vis_edge32(_wp_pdst_row_end, _pdst_row_end);    \
		vis_pst_32(_w_data_end, _wp_pdst_row_end, _emask);       \
		_wp_pdst_row++;                                          \
		_wp_pdst_row_end--;                                      \
		_MLIB_BG32_FPACK(_wp_pdst_row, _w_tc_0, _w_tc_1,         \
		    _w_data);                                            \
		_MLIB_BG32_FPACK(_wp_pdst_row_end, _w_tc_2, _w_tc_3,     \
		    _w_data_end);                                        \
		for (_i = 0; _i < (_w >> 3); _i++) {                     \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		}                                                        \
		if ((_w & 4) != 0) {                                     \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		}                                                        \
		if ((_w & 2) != 0) {                                     \
		    _MLIB_GOR_GORIZONTAL_LINE_BG32;                      \
		}                                                        \
		if ((_w & 1) != 0)                                       \
		    *_wp_pdst_row = _w_data;                             \
	    }                                                            \
	}

/* *********************************************************** */

#define	MLIB_SHIFT	16
#define	MLIB_05	0x8000
#define	MLIB_SIGNSHIFT	31
#define	COLOR_MASK	0xFF
#define	MLIB_INTMIN(a, b)	(a + ((b - a) & ((b - a) >> MLIB_SIGNSHIFT)))
#define	MLIB_INTMAX(a, b)	(a - ((a - b) & ((a - b) >> MLIB_SIGNSHIFT)))

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_data = mlib_ImageGetData(buffer);                   \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	buf_stride = mlib_ImageGetStride(buffer)

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_G_8(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG8Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 cFixed, cFixedStep, cFixedEnd;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_G(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG8Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG8) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_G8(x, y, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	rowPtr = pxTrans;

	vis_write_gsr(0);

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, rowPtr += intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			cFixed = rowPtr[i].c;
			cFixedStep =
			    (rowPtr[i + 1].c - rowPtr[i].c) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				cFixed += (cFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			cFixedEnd = cFixed + (xs2 - xs1) * cFixedStep;

			MLIB_FILL_ROW_G_8(buf_data, xs1, xs2, cFixed,
			    cFixedEnd);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_G_32(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 rFixed, rFixedStep, rFixedEnd;
	mlib_s32 gFixed, gFixedStep, gFixedEnd;
	mlib_s32 bFixed, bFixedStep, bFixedEnd;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG32Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	mlib_d64 d1b, d2b, d1e, d2e;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_G(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG32Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG32) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_G32(x, y, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, rowPtr += intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			rFixed = rowPtr[i].r;
			rFixedStep =
			    (rowPtr[i + 1].r - rowPtr[i].r) / (xs2 - xs1 + 1);
			gFixed = rowPtr[i].g;
			gFixedStep =
			    (rowPtr[i + 1].g - rowPtr[i].g) / (xs2 - xs1 + 1);
			bFixed = rowPtr[i].b;
			bFixedStep =
			    (rowPtr[i + 1].b - rowPtr[i].b) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				rFixed += (rFixedStep * (-xs1));
				gFixed += (gFixedStep * (-xs1));
				bFixed += (bFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			rFixedEnd = rFixed + (xs2 - xs1) * rFixedStep;
			gFixedEnd = gFixed + (xs2 - xs1) * gFixedStep;
			bFixedEnd = bFixed + (xs2 - xs1) * bFixedStep;

			((mlib_s32 *)&d1b)[0] = 0x7F000000;
			((mlib_s32 *)&d1b)[1] = bFixed;
			((mlib_s32 *)&d2b)[0] = gFixed;
			((mlib_s32 *)&d2b)[1] = rFixed;

			((mlib_s32 *)&d1e)[0] = 0x7F000000;
			((mlib_s32 *)&d1e)[1] = bFixedEnd;
			((mlib_s32 *)&d2e)[0] = gFixedEnd;
			((mlib_s32 *)&d2e)[1] = rFixedEnd;

			MLIB_FILL_ROW_G_32((mlib_s32 *)buf_data, xs1, xs2, d1b,
			    d2b, d1e, d2e);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BG_8(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 alpha)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG8Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 cFixed, cFixedStep, cFixedEnd;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_G(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG8Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG8) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_G8(x, y, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	rowPtr = pxTrans;

	vis_write_gsr(0);

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, rowPtr += intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			cFixed = rowPtr[i].c;
			cFixedStep =
			    (rowPtr[i + 1].c - rowPtr[i].c) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				cFixed += (cFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			cFixedEnd = cFixed + (xs2 - xs1) * cFixedStep;

			MLIB_FILL_ROW_BG_8(buf_data, xs1, xs2, cFixed,
			    cFixedEnd);

		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BG_32(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 alpha)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 rFixed, rFixedStep, rFixedEnd;
	mlib_s32 gFixed, gFixedStep, gFixedEnd;
	mlib_s32 bFixed, bFixedStep, bFixedEnd;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG32Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	mlib_d64 d1b, d2b, d1e, d2e;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_G(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG32Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG32) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_G32(x, y, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, rowPtr += intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			rFixed = rowPtr[i].r;
			rFixedStep =
			    (rowPtr[i + 1].r - rowPtr[i].r) / (xs2 - xs1 + 1);
			gFixed = rowPtr[i].g;
			gFixedStep =
			    (rowPtr[i + 1].g - rowPtr[i].g) / (xs2 - xs1 + 1);
			bFixed = rowPtr[i].b;
			bFixedStep =
			    (rowPtr[i + 1].b - rowPtr[i].b) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				rFixed += (rFixedStep * (-xs1));
				gFixed += (gFixedStep * (-xs1));
				bFixed += (bFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			rFixedEnd = rFixed + (xs2 - xs1) * rFixedStep;
			gFixedEnd = gFixed + (xs2 - xs1) * gFixedStep;
			bFixedEnd = bFixed + (xs2 - xs1) * bFixedStep;

			((mlib_s32 *)&d1b)[0] = 0x7F000000;
			((mlib_s32 *)&d1b)[1] = bFixed;
			((mlib_s32 *)&d2b)[0] = gFixed;
			((mlib_s32 *)&d2b)[1] = rFixed;

			((mlib_s32 *)&d1e)[0] = 0x7F000000;
			((mlib_s32 *)&d1e)[1] = bFixedEnd;
			((mlib_s32 *)&d2e)[0] = gFixedEnd;
			((mlib_s32 *)&d2e)[1] = rFixedEnd;

			MLIB_FILL_ROW_BG_32((mlib_s32 *)buf_data, xs1, xs2, d1b,
			    d2b, d1e, d2e);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * Count vertical limits of polygon and max number of intersections
 * with scan line (number of polygon extrema).
 */

static mlib_s32
mlib_CountExtremaAndMinMax_G(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax)
{
	mlib_s32 yMin, yMax, extrNum;
	mlib_s32 i, yCur, yPrev, dir, firstDir;

	yMin = MLIB_S32_MAX;
	yMax = MLIB_S32_MIN;

	yPrev = y[npoints - 1];
	dir = 0;
	firstDir = 0;
	extrNum = 0;

	for (i = 0; i < npoints; i++) {
		yCur = y[i];

		if (yCur > yPrev) {
			if (dir < 0)
				extrNum++;
			dir = 1;

			if (firstDir == 0)
				firstDir = 1;
		} else if (yCur < yPrev) {
			if (dir > 0)
				extrNum++;
			dir = -1;

			if (firstDir == 0)
				firstDir = -1;
		}

		if (yMin > yCur)
			yMin = yCur;

		if (yMax < yCur)
			yMax = yCur;

		yPrev = yCur;
	}

	if (firstDir != dir)
		extrNum++;

	*pyMin = yMin;
	*pyMax = yMax;

	return (extrNum);
}

/* *********************************************************** */

static void
mlib_ComputeSpansData_G8(
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG8Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans)
{
	SpanDataG8Ptr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 c1, c2, cPrev;
	mlib_s32 cFixed, cFixedStep;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++) {
		rowPtr->x = MLIB_S32_MAX;
		rowPtr++;
	}

	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		rowSpans[i] = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];
	cPrev = (COLOR_MASK & c[npoints - 1]);

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		c1 = cPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		c2 = cPrev = (COLOR_MASK & c[i]);

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			c1 ^= c2 ^= c1 ^= c2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		cFixed = (c1 << MLIB_SHIFT) + MLIB_05;
		cFixedStep = ((c2 - c1) << MLIB_SHIFT) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		cFixed += cFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].c = cFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));
			cFixed += cFixedStep;
		}
	}
}

/* *********************************************************** */

static void
mlib_ComputeSpansData_G32(
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG32Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans)
{
	SpanDataG32Ptr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 c1, c2, cPrev;
	mlib_s32 rFixed, rFixedStep;
	mlib_s32 gFixed, gFixedStep;
	mlib_s32 bFixed, bFixedStep;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++) {
		rowPtr->x = MLIB_S32_MAX;
		rowPtr++;
	}

	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		rowSpans[i] = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];
	cPrev = c[npoints - 1];

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		c1 = cPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		c2 = cPrev = c[i];

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			c1 ^= c2 ^= c1 ^= c2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		rFixed = ((c1 & 0xFF) << MLIB_SHIFT) + MLIB_05;
		rFixedStep = (((c2 & 0xFF) - (c1 & 0xFF)) << MLIB_SHIFT) / dy;

		gFixed = ((c1 & 0xFF00) << 8) + MLIB_05;
		gFixedStep = (((c2 & 0xFF00) - (c1 & 0xFF00)) << 8) / dy;

		bFixed = (c1 & 0xFF0000) + MLIB_05;
		bFixedStep = ((c2 & 0xFF0000) - (c1 & 0xFF0000)) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		rFixed += rFixedStep * dely;
		gFixed += gFixedStep * dely;
		bFixed += bFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].r = rFixed;
			rowPtr[j + 1].g = gFixed;
			rowPtr[j + 1].b = bFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));

			rFixed += rFixedStep;
			gFixed += gFixedStep;
			bFixed += bFixedStep;
		}
	}
}

/* *********************************************************** */
