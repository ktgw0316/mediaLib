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

#ifndef _MLIB_GRAPHICSMACRO_H
#define	_MLIB_GRAPHICSMACRO_H

#pragma ident	"@(#)mlib_GraphicsMacro.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DEFINE
 *    NULL                       - 0
 *
 * MACROS
 *    MLIB_GRAPHICS(F)           - depends on MLIB_TEST definition returns
 *                                 mlib_Graphics##F or mlib_Graphics##F
 *    MLIB_GRAPHICS_COLOR_8(C)   - normalizes MLIB_BYTE color C
 *    MLIB_GRAPHICS_COLOR_32(C)  - normalizes MLIB_INT color C
 *    MLIB_GRAPHICS_CHECK(A, B)  - gets (width-1), (height-1), stride; checks
 *                                 params and A, B conditions if MLIB_TEST
 *                                 defined:
 *                                   if (a || b A) return (MLIB_NULLPOINTER);
 *                                   if (a || b B) return (MLIB_FAILURE);
 *                                 , so A and B must start with "||" operator
 *    MLIB_GRAPHICS_CHECKZ(A, B) - same with MLIB_GRAPHICS_CHECK, but also
 *                                 gets zstride
 *    MLIB_GRAPHICS_DECL_VAR     - defines standard graphics variables :
 *                                   rtable, (data), width, height, stride
 *    MLIB_GRAPHICS_DECL_VARZ    - same with MLIB_GRAPHICS_DECL_VAR, but also
 *                                 defines zrtable, zstride
 *    MLIB_GRAPHICS_DECL_L*S     - declare nlines variable :
 *                                   LS  eq LINESET      : nlines = npoints / 2
 *                                   LFS eq LINEFANSET   : nlines = npoints - 1
 *                                   LSS eq LINESTRIPSET : nlines = npoints - 1
 *    MLIB_GRAPHICS_DECL_T*S     - declare nlines variable :
 *                                   TS  eq TRIANGLESET
 *                                   TFS eq TRIANGLEFANSET
 *                                   TSS eq TRIANGLESTRIPSET
 *    MLIB_GD_CHECK_XY[|G|Z|GZ]  - check whether x, y, c, z arrays are NULL
 *    MLIB_GD_CHECK_L*S          - check npoints parameter for Line*Set
 *    MLIB_GD_CHECK_T*S          - check npoints parameter for Triangle*Set
 */

#include <stdlib.h>
#include <mlib_ImageRowTable.h>

/* *********************************************************** */
#define	MLIB_GRAPHICS_COLOR_8(C)                                \
	C &= 0xFF;                                              \
	C |= (C << 8);                                          \
	C |= (C << 16)

/* *********************************************************** */
#define	MLIB_GRAPHICS_COLOR_32(C) C |= 0xFF000000;

/* *********************************************************** */
#define	MLIB_GRAPHICS_TO_DOUBLE(D_C, C)                         \
	((mlib_s32 *)&D_C)[0] = C;                              \
	((mlib_s32 *)&D_C)[1] = C

/* *********************************************************** */
#define	MLIB_GRAPHICS_CHECK(A, B)                               \
	width = mlib_ImageGetWidth(buffer) - 1;                 \
	height = mlib_ImageGetHeight(buffer) - 1;               \
	stride = mlib_ImageGetStride(buffer);                   \
	if ((rtable = mlib_ImageGetRowTable(buffer)) == NULL)   \
	    rtable = mlib_ImageCreateRowTable(buffer)

/* *********************************************************** */
#define	MLIB_GRAPHICS_CHECKZ(A, B)                                \
	width = mlib_ImageGetWidth(buffer) - 1;                   \
	height = mlib_ImageGetHeight(buffer) - 1;                 \
	stride = mlib_ImageGetStride(buffer);                     \
	zstride = mlib_ImageGetStride(zbuffer);                   \
	if ((rtable = mlib_ImageGetRowTable(buffer)) == NULL)     \
	    rtable = mlib_ImageCreateRowTable(buffer);            \
	if ((zrtable = mlib_ImageGetRowTable(zbuffer)) == NULL)   \
	    zrtable = mlib_ImageCreateRowTable(zbuffer)

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_VAR                                  \
	mlib_u8 **rtable;                                       \
	mlib_s32 width, height, stride

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_VARZ                                 \
	mlib_u8 **rtable, **zrtable;                            \
	mlib_s32 width, height, stride, zstride

/* *********************************************************** */
#define	MLIB_GRAPHICS_CHECK_1(A, B)                             \
	width = mlib_ImageGetWidth(buffer) - 1;                 \
	height = mlib_ImageGetHeight(buffer) - 1;               \
	if ((rtable = mlib_ImageGetRowTable(buffer)) == NULL)   \
	    rtable = mlib_ImageCreateRowTable(buffer)

/* *********************************************************** */
#define	MLIB_GRAPHICS_CHECKZ_1(A, B)                              \
	width = mlib_ImageGetWidth(buffer) - 1;                   \
	height = mlib_ImageGetHeight(buffer) - 1;                 \
	if ((rtable = mlib_ImageGetRowTable(buffer)) == NULL)     \
	    rtable = mlib_ImageCreateRowTable(buffer);            \
	if ((zrtable = mlib_ImageGetRowTable(zbuffer)) == NULL)   \
	    zrtable = mlib_ImageCreateRowTable(zbuffer)

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_VAR_1                                \
	mlib_u8 **rtable;                                       \
	mlib_s32 width, height

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_VARZ_1                               \
	mlib_u8 **rtable, **zrtable;                            \
	mlib_s32 width, height

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_LS	mlib_s32 nlines = npoints >> 1

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_LFS	mlib_s32 nlines = npoints - 1

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_LSS	mlib_s32 nlines = npoints - 1

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_TS	mlib_s32 nlines = npoints

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_TFS	mlib_s32 nlines = 2*(npoints - 2)

/* *********************************************************** */
#define	MLIB_GRAPHICS_DECL_TSS	mlib_s32 nlines = 2*(npoints - 2)

/* *********************************************************** */
#define	MLIB_GD_CHECK_XY	|| x == NULL || y == NULL

/* *********************************************************** */
#define	MLIB_GD_CHECK_XYG	|| x == NULL || y == NULL || c == NULL

/* *********************************************************** */
#define	MLIB_GD_CHECK_XYZ	|| x == NULL || y == NULL || z == NULL

/* *********************************************************** */
#define	MLIB_GD_CHECK_XYZG	|| x == NULL || y == NULL || z == NULL || \
	c == NULL

/* *********************************************************** */
#define	MLIB_GD_CHECK_LS	|| (nlines <= 0) || (npoints & 1)

/* *********************************************************** */
#define	MLIB_GD_CHECK_LFS	|| (nlines <= 0)

/* *********************************************************** */
#define	MLIB_GD_CHECK_LSS	|| (nlines <= 0)

/* *********************************************************** */
#define	MLIB_GD_CHECK_TS	|| (npoints <= 2) || (npoints % 3)

/* *********************************************************** */
#define	MLIB_GD_CHECK_TFS	|| (npoints <= 2)

/* *********************************************************** */
#define	MLIB_GD_CHECK_TSS	|| (npoints <= 2)

/* *********************************************************** */
#define	MLIB_NOTHING	|| (0)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GRAPHICSMACRO_H */
