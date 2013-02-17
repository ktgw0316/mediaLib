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

#ifndef _MLIB_GRAPHICSFILLROW_H
#define	_MLIB_GRAPHICSFILLROW_H

#pragma ident	"@(#)mlib_GraphicsFillRow.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MLIB_VIS)
#error this include file can not be used with VIS
#endif

/*
 * MACROS
 *    MLIB_FILL_ROW_[8|32] - draw scanline
 *    MLIB_FILL_ROW_X_[8|32] - draw scanline in XOR mode
 *    MLIB_FILL_ROW_B_[8 | 32] - draw scanline with alpha blending
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
 *    ca*       (color * alpha / 255.) as mlib_d64 value.
 *    a_1       (alpha / 255. - 1) as mlib_d64 value.
 *    vca, va_1 Used in VIS version only.
 */

/* *********************************************************** */

#define	DOUBLE_FROM_INT(dc, c)                                  \
	((mlib_s32 *)&dc)[0] = c;                               \
	((mlib_s32 *)&dc)[1] = c

/* *********************************************************** */

#define	MLIB_FILL_ROW_8(prow, xb, xe, color, dcolor)            \
	{                                                       \
	    mlib_d64 *_dpdst_row;                               \
	    mlib_u8 *_pdst_row = (prow) + (xb);                 \
	    mlib_s32 _i, _w = (xe) - (xb) + 1, _all =           \
		    (8 - (mlib_addr)_pdst_row) & 7;             \
	    _all &= (_all - _w) >> 31;                          \
/*                                                              \
 * if (_all >= _w) _all = 0;                                    \
 */                                                             \
	    for (_i = 0; _i < _all; _i++)                       \
		_pdst_row[_i] = (color);                        \
	    _dpdst_row = (mlib_d64 *)(_pdst_row + _i);          \
	    for (; _i <= (_w - 8); _i += 8)                     \
		(*_dpdst_row++) = dcolor;                       \
	    for (; _i < _w; _i++)                               \
		_pdst_row[_i] = (color);                        \
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

#define	MLIB_FILL_ROW_X_8(prow, xb, xe, color, dcolor)          \
	{                                                       \
	    mlib_u8 *_pdst_row = (prow) + (xb);                 \
	    mlib_s32 _i, _w = (xe) - (xb) + 1, _all =           \
		    (4 - (mlib_addr)_pdst_row) & 3;             \
	    _all &= (_all - _w) >> 31;                          \
/*                                                              \
 * if (_all >= _w) _all = 0;                                    \
 */                                                             \
	    for (_i = 0; _i < _all; _i++)                       \
		_pdst_row[_i] ^= (color);                       \
	    for (; _i <= (_w - 4); _i += 4)                     \
		*((mlib_s32 *)(_pdst_row + _i)) ^= (color);     \
	    for (; _i < _w; _i++)                               \
		_pdst_row[_i] ^= (color);                       \
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_X_32(prow, xb, xe, color, dcolor)         \
	{                                                       \
	    mlib_s32 *_pdst_row = (prow) + (xb);                \
	    mlib_s32 _i, _w = (xe) - (xb);                      \
	                                                        \
	    for (_i = 0; _i <= _w; _i++)                        \
		*((mlib_s32 *)(_pdst_row + _i)) ^= (color);     \
	}


/* *********************************************************** */

#define	MLIB_FILL_ROW_B_8(prow, xb, xe, ca, a_1, vca, va_1)            	\
	{                                                       	\
	    mlib_u8 *_pdst_row = (prow) + (xb);                 	\
	    mlib_s32 _i, _w = (xe) - (xb) + 1, _all =           	\
		    (4 - (mlib_addr)_pdst_row) & 3;             	\
	    _all &= (_all - _w) >> 31;                          	\
/*                                                              	\
 * if (_all >= _w) _all = 0;                                    	\
 */                                                             	\
	    for (_i = 0; _i < _all; _i++)                       	\
		MLIB_GRAPHICS_BLEND_8(_pdst_row + _i)            	\
	    for (; _i <= (_w - 4); _i += 4)                     	\
		MLIB_GRAPHICS_BLEND_8_4((mlib_s32 *)(_pdst_row + _i))  	\
	    for (; _i < _w; _i++)                               	\
		MLIB_GRAPHICS_BLEND_8(_pdst_row + _i)                  	\
	}

#define	MLIB_FILL_ROW_B_8_START(prow, xb, xe, ca, a_1, vca, va_1)	\
	{                                                       	\
	    mlib_u8 *_pdst_row = (prow) + (xb);                 	\
	    mlib_s32 _i, _w = (xe) - (xb) + 1, _all =           	\
		    (4 - (mlib_addr)_pdst_row) & 3;             	\
	    _all &= (_all - _w) >> 31;                          	\
	    for (_i = 0; _i < _all; _i++)                       	\
		MLIB_GRAPHICS_BLEND_8(_pdst_row + _i)

#define	MLIB_FILL_ROW_B_8_END						\
	    for (; _i <= (_w - 4); _i += 4)                     	\
		MLIB_GRAPHICS_BLEND_8_4((mlib_s32 *)(_pdst_row + _i))  	\
	    for (; _i < _w; _i++)                               	\
		MLIB_GRAPHICS_BLEND_8(_pdst_row + _i)                  	\
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_B_32(prow, xb, xe, ca1, ca2, ca3, a_1, vca, va_1) \
	{                                                       	\
	    mlib_s32 *_pdst_row = (prow) + (xb);                	\
	    mlib_s32 _i, _w = (xe) - (xb);                      	\
	                                                        	\
	    for (_i = 0; _i <= _w; _i++)                        	\
		MLIB_GRAPHICS_BLEND_32((mlib_s32 *)(_pdst_row + _i))   	\
	}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GRAPHICSFILLROW_H */
