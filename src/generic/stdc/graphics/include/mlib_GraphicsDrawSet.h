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

#ifndef _MLIB_GRAPHICSDRAWSET_H
#define	_MLIB_GRAPHICSDRAWSET_H

#pragma ident	"@(#)mlib_GraphicsDrawSet.h	9.4	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  DEFINES
 *    MLIB_SHIFT  - shift value for fixed point operation
 *    MLIB_HALF   - (1 << (MLIB_SHIFT - 1))
 *    MLIB_ONE    - (1 << MLIB_SHIFT)
 *    MLIB_ONE_FL - mlib_f32 version of MLIB_ONE
 *
 *    MLIB_GDS_STARTX      - offset of startx value in coords array
 *    MLIB_GDS_STARTY      - offset of starty value in coords array
 *    MLIB_GDS_LENGTHLINES - offset of length of line value in coords array
 *    MLIB_GDS_STEPLINES   - offset of step value in coords array
 *    MLIB_GDS_NEXT        - value to reach next entry in coords array
 *
 *    MLIB_GDS_STARTZ      - offset of startz value in coords array
 *    MLIB_GDS_STEPZ       - offset of step Z-buffer value in coords array
 *    MLIB_GDS_STARTC      - offset of color8 value in coords array
 *    MLIB_GDS_STEPC       - offset of color8 step value in coords array
 *    MLIB_GDS_NEXT_GZ     - value to reach next entry in coords array with
 *                           Z-buffer and/or 8bit Gouraud shading modes
 *
 *    MLIB_GDS_STARTC1     - offset of 8..15 bits of color32 value
 *                           in coords array
 *    MLIB_GDS_STEPC1      - offset of 8..15 bits of color32 step value
 *                           in coords array
 *    MLIB_GDS_STARTC2     - offset of 16..23 bits of color32 value
 *                           in coords array
 *    MLIB_GDS_STEPC2      - offset of 16..23 bits of color32 step value
 *                           in coords array
 *    MLIB_GDS_NEXT_GZ32   - value to reach next entry in coords array with
 *                           32bit Gouraud shading with/without Z-buffer modes
 *
 *    MLIB_GDS_STARTX      - offset of stopx value in coords array, is using in
 *                           *Hor* functions
 *
 *    MLIB_STEP_TABLE_LENGTH  - see graphics/lineset/mlib_GraphicsDrawSet.c
 *
 *    MLIB_BG_COEFF        - coeff of alpha value for neighbors in horizontal/
 *                           vertical antialiased lines
 *    MLIB_DG_COEFF        - coeff of alpha value for neighbors in diagonal
 *                           antialiased lines
 *
 *  TABLES
 *    mlib_f32 *mlib_GraphicsStepTable_f32 - table of precalculated divisions
 *    mlib_GraphicsAliasTab_u8          - table of precalculated alpha values
 *    mlib_GraphicsSqrtTab_u8           - table of precalculated sqrt values
 *
 *  FUNCTIONS
 *    mlib_GraphicsClipLine           - clipping function for simple modes
 *    mlib_GraphicsClipLine_G_8       - clipping function for Gouraud shading
 *    mlib_GraphicsClipLine_G_32      - clipping function for Gouraud shading
 *    mlib_GraphicsClipLine_Z         - clipping function for Z-buffer mode
 *    mlib_GraphicsClipLine_GZ_8      - clipping function for Gouraud shading
 *                                      and Z-buffer modes
 *    mlib_GraphicsClipLine_GZ_32     - clipping function for Gouraud shading
 *                                      and Z-buffer modes
 *    mlib_GraphicsClipLineSet        - clipping function for simple modes
 *    mlib_GraphicsClipLineSet_AB     - clipping function for simple modes
 *    mlib_GraphicsClipLineSet_G_8    - clipping function for Gouraud shading
 *    mlib_GraphicsClipLineSet_G_32   - clipping function for Gouraud shading
 *    mlib_GraphicsClipLineSet_BG_8   - clipping function for Gouraud shading
 *					with blending
 *    mlib_GraphicsClipLineSet_BG_32  - clipping function for Gouraud shading
 *					with blending
 *    mlib_GraphicsClipLineSet_Z      - clipping function for Z-buffer mode
 *    mlib_GraphicsClipLineSet_BZ     - clipping function for Z-buffer and
 *					blending mode
 *    mlib_GraphicsClipLineSet_GZ_8   - clipping function for Gouraud shading
 *                                      and Z-buffer modes
 *    mlib_GraphicsClipLineSet_GZ_32  - clipping function for Gouraud shading
 *                                      and Z-buffer modes
 *    mlib_GraphicsClipLineSet_BGZ_8  - clipping function for Gouraud shading,
 *                                      Z-buffer and blending modes
 *    mlib_GraphicsClipLineSet_BGZ_32 - clipping function for Gouraud shading,
 *                                      Z-buffer and blending modes
 *    mlib_GraphicsSpamLineSet_8      - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_32     - line drawing function for mlib_s32 image
 *    mlib_GraphicsSpamLineSet_X_8    - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_X_32   - line drawing function for mlib_s32 image
 *    mlib_GraphicsSpamLineSet_G_8    - line drawing function with Gouraud
 *                                      shading
 *    mlib_GraphicsSpamLineSet_G_32   - line drawing function with Gouraud
 *                                      shading
 *    mlib_GraphicsSpamLineSet_BG_8   - line drawing function with Gouraud
 *                                      shading with blending
 *    mlib_GraphicsSpamLineSet_BG_32  - line drawing function with Gouraud
 *                                      shading with blending
 *    mlib_GraphicsSpamLineSet_Z_8    - line drawing function with Z-buffering
 *    mlib_GraphicsSpamLineSet_Z_32   - line drawing function with Z-buffering
 *    mlib_GraphicsSpamLineSet_BZ_8   - line drawing function with Z-buffering
 *					and blending
 *    mlib_GraphicsSpamLineSet_BZ_32  - line drawing function with Z-buffering
 *					and blending
 *    mlib_GraphicsSpamLineSet_GZ_8   - line drawing function with Gouraud
 *                                      shading and Z-buffering
 *    mlib_GraphicsSpamLineSet_GZ_32  - line drawing function with Gouraud
 *                                      shading and Z-buffering
 *    mlib_GraphicsSpamLineSet_BGZ_8  - line drawing function with Gouraud
 *                                      shading, Z-buffering and antialiasing
 *    mlib_GraphicsSpamLineSet_BGZ_32 - line drawing function with Gouraud
 *                                      shading, Z-buffering and antialiasing
 *    mlib_GraphicsSpamLineSet_A_8    - line drawing function with antialiasing
 *    mlib_GraphicsSpamLineSet_A_32   - line drawing function with antialiasing
 *    mlib_GraphicsSpamLineSet_AB_8   - line drawing function with antialiasing
 *					and alpha blending
 *    mlib_GraphicsSpamLineSet_AB_32  - line drawing function with antialiasing
 *					and alpha blending
 *    mlib_GraphicsSpamLineSet_AG_8   - line drawing function with Gouraud
 *                                      shading and antialiasing
 *    mlib_GraphicsSpamLineSet_AG_32  - line drawing function with Gouraud
 *					and alpha blending
 *    mlib_GraphicsSpamLineSet_ABG_8  - line drawing function with Gouraud
 *                                      shading, antialiasing and alpha blending
 *    mlib_GraphicsSpamLineSet_ABG_32 - line drawing function with Gouraud
 *                                      shading, antialiasing and alpha blending
 *    mlib_GraphicsSpamLineSet_AZ_8   - line drawing function with Z-buffering
 *                                      and antialiasing
 *    mlib_GraphicsSpamLineSet_AZ_32  - line drawing function with Z-buffering
 *                                      and antialiasing
 *    mlib_GraphicsSpamLineSet_ABZ_8  - line drawing function with Z-buffering,
 *                                      antialiasing and blending
 *    mlib_GraphicsSpamLineSet_ABZ_32 - line drawing function with Z-buffering,
 *                                      antialiasing and blending
 *    mlib_GraphicsSpamLineSet_AGZ_8  - line drawing function with Gouraud
 *                                      shading with Z-buffering
 *                                      and antialiasing
 *    mlib_GraphicsSpamLineSet_AGZ_32 - line drawing function with Gouraud
 *                                      shading with Z-buffering
 *                                      and antialiasing
 *    mlib_GraphicsSpamLineSet_ABGZ_8 - line drawing function with Gouraud
 *                                      shading, Z-buffering,
 *                                      antialiasing and blending
 *    mlib_GraphicsSpamLineSet_AGBZ_32- line drawing function with Gouraud
 *                                      shading, Z-buffering,
 *                                      antialiasing and blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipLineSet
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     mlib_s32 color,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_AB
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_G_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_G_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_BG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 alpha,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_BG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 alpha,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_Z
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     mlib_s32 color,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_BZ
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_GZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_GZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 a,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_BGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 a,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsClipLineSet_BGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *    void  mlib_GraphicsSpamLineSet_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_X_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_X_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_G_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineSet_G_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineSet_BG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineSet_BG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineSet_Z_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_Z_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_BZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_BZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_GZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineSet_GZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineSet_BGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineSet_BGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineSet_A_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_A_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AB_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AB_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineHor_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_B_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_B_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_X_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_X_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_G_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineHor_G_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineHor_BG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineHor_BG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineHor_Z_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_Z_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_BZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_BZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_GZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineHor_GZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineHor_BGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineHor_BGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 alpha);
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    zrtable     Pointer to array of starts of row of zbuffer, zrtable[-1][-1]
 *                  is buffer for unwanted stores
 *    width       Image.Width - 1, is using to discover whether clipping needed
 *    width       Image.Width - 2, is using to discover to clip neighbor in
 *                  *Spam_A* functions
 *    height      Image.Height - 1, is using to discover whether clipping needed
 *    height      Image.Height - 2, is using to discover to clip neighbor in
 *                  *Spam_A* functions
 *    x           Pointer to array of X coordinate of the points
 *    y           Pointer to array of Y coordinate of the points
 *    z           Pointer to array of Z coordinate of the points
 *    c           Pointer to array of colors of the points
 *    nlines      Number of lines
 *    dp0         Step from (x0, y0) to the (x0, y0) of the next line
 *    dp1         Step from (x1, y1) to the (x1, y1) of the next line,
 *                for example
 *                  DrawLineSet           dp0 = 2, dp1 = 2
 *                  DrawLineFanSet        dp0 = 0, dp1 = 1
 *                  DrawLineStripSet      dp0 = 1, dp1 = 1
 *                  DrawTriangleSet       dp0 = 3, dp1 = 3
 *                  DrawTriangleFanSet    dp0 = 0, dp1 = 1
 *                  DrawTriangleStripSet  dp0 = 1, dp1 = 1
 *    color       Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    color  = (color & 0xFF),
 *                    color |= (color << 8),
 *                    color |= (color << 16)
 *    alpha       Alpha value for blending.
 *    SpamLineSet Function is called to draw
 *    stride      Stride of the image
 *    zstride     Stride of the zbuffer image
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT]      holds X
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT]      holds Y
 *                  coords[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT] holds LL
 *                  coords[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT]   holds SL
 *                  if |dx| > |dy| then
 *                    X  = x0, Y = ~(MLIB_HALF + (y0 << MLIB_SHIFT))
 *                    LL = |dx|, SL = (mlib_s32) (32768. * dy / LL)
 *                  else
 *                    X = ~(MLIB_HALF + (x0 << MLIB_SHIFT)), Y = y0
 *                    LL = |dy|, SL = (mlib_s32) (32768. * dx / LL)
 *                  endif
 *                , so we could understand in *Spam* functions what is
 *                bigger |dx| or |dy| by negative value of opposite starting
 *                value. Note we always step by +1 by main axis, never -1.
 *   coordsend    for(; coords < coordsend; coords += MLIB_GDS_NEXT)
 *   dcolor       dcolor = vis_to_double_dup(color)
 *
 *  NOTE
 *   About mlib_ClipTriangle* functions see in corresponding files.
 */

#include <mlib_types.h>
#include <mlib_status.h>
#include <mlib_image_types.h>

#ifdef __cplusplus
extern "C" {
#endif		   /* __cplusplus */
/* *********************************************************** */
#undef  MLIB_SHIFT
#define	MLIB_SHIFT	15
#define	MLIB_HALF	16384
#define	MLIB_ONE	32768
#define	MLIB_ONE_FL	32768.f
/* *********************************************************** */
#define	MLIB_GDS_STARTX		0
#define	MLIB_GDS_STARTY		1
#define	MLIB_GDS_LENGTHLINES	2
#define	MLIB_GDS_STEPLINES	3
#define	MLIB_GDS_NEXT		4
#define	MLIB_GDS_STARTZ		4
#define	MLIB_GDS_STEPZ		5
#define	MLIB_GDS_STARTC		6
#define	MLIB_GDS_STEPC		7
#define	MLIB_GDS_NEXT_GZ	8
#define	MLIB_GDS_STARTC1	9
#define	MLIB_GDS_STEPC1		10
#define	MLIB_GDS_STARTC2	11
#define	MLIB_GDS_STEPC2		12
#define	MLIB_GDS_NEXT_GZ32	16
#define	MLIB_GDS_STOPX		2
/* *********************************************************** */
#define	MLIB_STEP_TABLE_LENGTH	256
extern mlib_f32 *mlib_GraphicsStepTable_f32;
extern mlib_u8 mlib_GraphicsSqrtTab_u8[];
extern mlib_u8 *mlib_GraphicsDivTab_u8;
extern mlib_u8 *mlib_GraphicsAliasTab_u8;
extern mlib_f32 mlib_GraphicsDivTable_f32[];

#define	MLIB_BG_COEFF	47
#define	MLIB_DG_COEFF	80

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineHor_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_B_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineHor_B_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_X_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineHor_X_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_G_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

void mlib_GraphicsSpamLineHor_G_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_BG_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

void mlib_GraphicsSpamLineHor_BG_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_Z_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineHor_Z_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_BZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineHor_BZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_GZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

void mlib_GraphicsSpamLineHor_GZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

/* *********************************************************** */
void mlib_GraphicsSpamLineHor_BGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

void mlib_GraphicsSpamLineHor_BGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineSet_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_X_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineSet_X_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_B_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_d64 ca,
    mlib_d64 a_1,
    mlib_d64 vca,
    mlib_f32 va_1);

void mlib_GraphicsSpamLineSet_B_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_d64 ca1,
    mlib_d64 ca2,
    mlib_d64 ca3,
    mlib_d64 a_1,
    mlib_d64 vc,
    mlib_d64 va_1);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_G_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

void mlib_GraphicsSpamLineSet_G_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_BG_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

void mlib_GraphicsSpamLineSet_BG_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_Z_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineSet_Z_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_BZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor);

void mlib_GraphicsSpamLineSet_BZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_GZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

void mlib_GraphicsSpamLineSet_GZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_BGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

void mlib_GraphicsSpamLineSet_BGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_AG_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_AG_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_ABG_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_ABG_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_A_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_A_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_AB_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_AB_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_AZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_AZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_ABZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_ABZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 color,
    mlib_s32 alpha,
    mlib_d64 dcolor,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_AGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_AGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsSpamLineSet_ABGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha,
    mlib_s32 width,
    mlib_s32 height);

void mlib_GraphicsSpamLineSet_ABGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha,
    mlib_s32 width,
    mlib_s32 height);

/* *********************************************************** */
void mlib_GraphicsClipLine(
    mlib_s32 *coords,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y);

/* *********************************************************** */
void mlib_GraphicsClipLine_G_8(
    mlib_s32 *coords,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c);

void mlib_GraphicsClipLine_G_32(
    mlib_s32 *coords,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c);

/* *********************************************************** */
void mlib_GraphicsClipLine_Z(
    mlib_s32 *coords,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z);

/* *********************************************************** */
void mlib_GraphicsClipLine_GZ_8(
    mlib_s32 *coords,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c);

void mlib_GraphicsClipLine_GZ_32(
    mlib_s32 *coords,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c);

/* *********************************************************** */
void mlib_GraphicsClipLineSet(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_AB(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    mlib_s32 alpha,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_G_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipLineSet_G_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_BG_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipLineSet_BG_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_Z(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_B_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    mlib_s32 alpha);

void mlib_GraphicsClipLineSet_B_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    mlib_s32 alpha);

/* *********************************************************** */
void mlib_GraphicsClipLineSet_BZ(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    mlib_s32 alpha,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_GZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipLineSet_GZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipLineSet_BGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipLineSet_BGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_B(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    mlib_s32 a,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_AB(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    mlib_s32 a,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_G_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipTriangleSet_G_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_BG_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipTriangleSet_BG_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_Z(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_BZ(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    mlib_s32 alpha,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_GZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipTriangleSet_GZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleSet_BGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

void mlib_GraphicsClipTriangleSet_BGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineSet) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_B(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    mlib_s32 a,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_G_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

void mlib_GraphicsClipTriangleHor_G_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_BG_8(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

void mlib_GraphicsClipTriangleHor_BG_32(
    mlib_u8 **rtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_Z(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_BZ(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 color,
    mlib_s32 alpha,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_GZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

void mlib_GraphicsClipTriangleHor_GZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

/* *********************************************************** */
void mlib_GraphicsClipTriangleHor_BGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

void mlib_GraphicsClipTriangleHor_BGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 *c,
    mlib_s32 a,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    void (*SpamLineHor) ());

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GRAPHICSDRAWSET_H */
