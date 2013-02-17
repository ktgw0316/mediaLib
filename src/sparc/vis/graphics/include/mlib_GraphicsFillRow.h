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

#ifndef _MLIB_GRAPHICSFILLROW_H
#define	_MLIB_GRAPHICSFILLROW_H

#pragma ident	"@(#)mlib_GraphicsFillRow.h	9.2	07/11/05 SMI"

/*
 * MACROS
 *      MLIB_FILL_ROW_[8 | 32] - draw scanline
 *      MLIB_FILL_ROW_X_[8 | 32] - draw scanline in XOR mode
 *      MLIB_FILL_ROW_B_[8 | 32] - draw scanline with alpha blending
 *
 * SYNOPSIS
 *    MLIB_FILL_ROW_8(prow, xb, xe, color, dcolor)
 *    MLIB_FILL_ROW_32(prow, xb, xe, color, dcolor)
 *    MLIB_FILL_ROW_X_8(prow, xb, xe, color, dcolor)
 *    MLIB_FILL_ROW_X_32(prow, xb, xe, color, dcolor)
 *    MLIB_FILL_ROW_B_8(prow, xb, xe, ca, a_1, vca, va_1)
 *    MLIB_FILL_ROW_B_32(prow, xb, xe, ca1, ca2, ca3, a_1, vca, va_1)
 *
 * ARGUMENTS
 *    prow      Pointer to the row of image which the function is drawing into.
 *    xb        X coordinate of the left boundary of the scanline.
 *    xe        X coordinate of the right boundary of the scanline.
 *    color     Color used in the drawing.
 *    dcolor    Color used in the drawing as mlib_d64 value.
 *    vca       (color * alpha / 255.) as mlib_d64 16-bit partitioned data.
 *    va_1      (alpha / 255. - 1) as mlib_d64 (B_32) or mlib_f32 (B_8)
 *               16-bit partitioned data.
 *    ca*, a_1  Unused in VIS version
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <vis_proto.h>

/* *********************************************************** */
#define	DOUBLE_FROM_INT(dc, c)	dc = vis_to_double_dup(c)
/* *********************************************************** */
#define	MLIB_FILL_ROW_8(prow, xb, xe, color, dcolor)              \
	{                                                         \
	    mlib_s32 _i, _w = (xe) - (xb);                        \
	    mlib_u8 *_pdst_row = (prow) + (xb);                   \
	    mlib_u8 *_pdst_end = _pdst_row + _w;                  \
	    mlib_d64 *_dpdst_row = vis_alignaddr(_pdst_row, 0);   \
	    mlib_s32 _mask = vis_edge8(_pdst_row, _pdst_end);     \
	                                                          \
	    if (_w >= 0)                                          \
		vis_pst_8(dcolor, _dpdst_row, _mask);             \
	    _dpdst_row++;                                         \
	    _i = (mlib_addr)_dpdst_row - (mlib_addr)_pdst_row;    \
	    for (; _i <= (_w - 7); _i += 8)                       \
		(*_dpdst_row++) = dcolor;                         \
	    if (_i <= _w) {                                       \
		_mask = vis_edge8(_dpdst_row, _pdst_end);         \
		vis_pst_8(dcolor, _dpdst_row, _mask);             \
	    }                                                     \
	}
/* *********************************************************** */
#define	MLIB_FILL_ROW_32(prow, xb, xe, color, dcolor)           \
	{                                                       \
	    mlib_s32 *_pdst_row = (prow) + (xb);                \
	    mlib_s32 _i = 0, _w = (xe) - (xb);                  \
	                                                        \
	    if ((((mlib_addr)_pdst_row) & 4) && _w >= 0)        \
		_pdst_row[_i++] = (color);                      \
	    for (; _i <= (_w - 1); _i += 2)                     \
		*((mlib_d64 *)(_pdst_row + _i)) = dcolor;       \
	    if (_i <= _w)                                       \
		_pdst_row[_i] = (color);                        \
	}
/* *********************************************************** */
#define	MLIB_FILL_ROW_X_8(prow, xb, xe, color, dcolor)                     \
	{                                                                  \
	    mlib_s32 _i, _w = (xe) - (xb);                                 \
	    mlib_u8 *_pdst_row = (prow) + (xb);                            \
	    mlib_u8 *_pdst_end = _pdst_row + _w;                           \
	    mlib_s32 _mask = vis_edge8(_pdst_row, _pdst_end);              \
	    mlib_d64 *_dpdst_row = vis_alignaddr(_pdst_row, 0), _ddata =   \
		*_dpdst_row;                                               \
	    _ddata = vis_fxor(_ddata, dcolor);                             \
	    if (_w >= 0)                                                   \
		vis_pst_8(_ddata, _dpdst_row, _mask);                      \
	    _dpdst_row++;                                                  \
	    _i = (mlib_addr)_dpdst_row - (mlib_addr)_pdst_row;             \
	    for (; _i <= (_w - 7); _i += 8) {                              \
		_ddata = *_dpdst_row;                                      \
		_ddata = vis_fxor(_ddata, dcolor);                         \
		(*_dpdst_row++) = _ddata;                                  \
	    }                                                              \
	    if (_i <= _w) {                                                \
		_mask = vis_edge8(_dpdst_row, _pdst_end);                  \
		_ddata = *_dpdst_row;                                      \
		_ddata = vis_fxor(_ddata, dcolor);                         \
		vis_pst_8(_ddata, _dpdst_row, _mask);                      \
	    }                                                              \
	}
/* *********************************************************** */
#define	MLIB_FILL_ROW_X_32(prow, xb, xe, color, dcolor)         \
	{                                                       \
	    mlib_s32 *_pdst_row = (prow) + (xb), _i = 0, _w =   \
		(xe) - (xb);                                    \
	    mlib_d64 _ddata;                                    \
	                                                        \
	    if ((((mlib_addr)_pdst_row) & 4) && _w >= 0)        \
		_pdst_row[_i++] ^= (color);                     \
	    for (; _i <= (_w - 1); _i += 2) {                   \
		_ddata = *((mlib_d64 *)(_pdst_row + _i));       \
		_ddata = vis_fxor(_ddata, dcolor);              \
		*((mlib_d64 *)(_pdst_row + _i)) = _ddata;       \
	    }                                                   \
	    if (_i <= _w)                                       \
		_pdst_row[_i] ^= (color);                       \
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_B_8(prow, xb, xe, ca, a_1, vca, va_1)            	       \
	{                                                       	       \
	    mlib_s32 _i, _w = (xe) - (xb);                                     \
	    mlib_u8 *_pdst_row = (prow) + (xb);                                \
	    mlib_u8 *_pdst_end = _pdst_row + _w;                               \
	    mlib_s32 _mask;              				       \
	    mlib_d64 _ddata; 						       \
	    mlib_f32 *_fpdst_row; 					       \
	    mlib_f32 fdst;	 					       \
	    if (_w >= 0) { 						       \
		_fpdst_row = (mlib_f32 *)((mlib_addr)_pdst_row &~ 3);          \
		_mask = vis_edge8(_pdst_row, _pdst_end);              	       \
		_mask &= 0xf0 >> ((mlib_addr)_pdst_row & 4);                   \
		MLIB_GRAPHICS_BLEND_F(fdst, *_fpdst_row) 		       \
		_ddata = vis_freg_pair(fdst, fdst); 			       \
		vis_pst_8(_ddata, (void *)((mlib_addr)_pdst_row &~ 7), _mask); \
		_fpdst_row++;                                                  \
		_i = (mlib_addr)_fpdst_row - (mlib_addr)_pdst_row;             \
		for (; _i <= (_w - 3); _i += 4) {                              \
			MLIB_GRAPHICS_BLEND_F(*_fpdst_row, *_fpdst_row)        \
			_fpdst_row++;                                  	       \
		}                                                              \
		if (_i <= _w) {                                                \
			_mask = vis_edge8(_fpdst_row, _pdst_end);              \
			MLIB_GRAPHICS_BLEND_F(fdst, *_fpdst_row) 	       \
			_ddata = vis_freg_pair(fdst, fdst); 		       \
			vis_pst_8(_ddata, (void *)((mlib_addr)_fpdst_row &~ 7),\
					_mask);                      	       \
		} 							       \
	    }								       \
	}

#define	MLIB_FILL_ROW_B_8_START(prow, xb, xe, ca, a_1, vca, va_1)	       \
	{                                                       	       \
	    mlib_s32 _i, _w = (xe) - (xb);                                     \
	    mlib_u8 *_pdst_row = (prow) + (xb);                                \
	    mlib_u8 *_pdst_end = _pdst_row + _w;                               \
	    mlib_s32 _mask;              				       \
	    mlib_d64 _ddata; 						       \
	    mlib_f32 *_fpdst_row; 					       \
	    mlib_f32 fdst;	 					       \
	    if (_w >= 0) { 						       \
		_fpdst_row = (mlib_f32 *)((mlib_addr)_pdst_row &~ 3);          \
		_mask = vis_edge8(_pdst_row, _pdst_end);              	       \
		_mask &= 0xf0 >> ((mlib_addr)_pdst_row & 4);                   \
		MLIB_GRAPHICS_BLEND_F(fdst, *_fpdst_row) 		       \
		_ddata = vis_freg_pair(fdst, fdst); 			       \
		vis_pst_8(_ddata, (void *)((mlib_addr)_pdst_row &~ 7), _mask); \
		_fpdst_row++;                                                  \
		_i = (mlib_addr)_fpdst_row - (mlib_addr)_pdst_row;

#define	MLIB_FILL_ROW_B_8_END						       \
		for (; _i <= (_w - 3); _i += 4) {                              \
			MLIB_GRAPHICS_BLEND_F(*_fpdst_row, *_fpdst_row)        \
			_fpdst_row++;                                  	       \
		}                                                              \
		if (_i <= _w) {                                                \
			_mask = vis_edge8(_fpdst_row, _pdst_end);              \
			MLIB_GRAPHICS_BLEND_F(fdst, *_fpdst_row) 	       \
			_ddata = vis_freg_pair(fdst, fdst); 		       \
			vis_pst_8(_ddata, (void *)((mlib_addr)_fpdst_row &~ 7),\
					_mask);                      	       \
		} 							       \
	    }								       \
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_B_32(prow, xb, xe, ca1, ca2, ca3, a_1, vca, va_1) \
	{                                                       	\
	    mlib_s32 *_pdst_row = (prow) + (xb);                	\
	    mlib_s32 _i, _w = (xe) - (xb);                      	\
	                                                        	\
	    for (_i = 0; _i <= _w; _i++)                        	\
		MLIB_GRAPHICS_BLEND_32(_pdst_row + _i)   		\
	}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GRAPHICSFILLROW_H */
