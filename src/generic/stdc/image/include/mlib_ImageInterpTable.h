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

#ifndef _MLIB_IMAGEINTERPTABLE_H
#define	_MLIB_IMAGEINTERPTABLE_H

#pragma ident	"@(#)mlib_ImageInterpTable.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

typedef struct
{
/* The width  of the interpolation kernel in pixels. */
	mlib_s32 width;
/* The height of the interpolation kernel in pixels. */
	mlib_s32 height;
	mlib_s32 leftPadding;	/* The number of pixels lying to the left of */
				/* the interpolation kernel key position. */
	mlib_s32 topPadding;	/* The number of pixels lying above the */
				/* interpolation kernel key position. */
	mlib_s32 subsampleBitsH;	/* The numbers of bits used for the */
					/* horizontal subsample position. */
	mlib_s32 subsampleBitsV;	/* The numbers of bits used for the */
					/* vertical subsample position. */
	mlib_s32 precisionBits;	/* The number of fractional bits used to */
				/* describe the coefficients. */
/* The numbers of bits used for x position in kernel. */
	mlib_s32 width_bits;
/* The numbers of bits used for x position in kernel. */
	mlib_s32 vis_width_bits;
/* The numbers of bits used for y position in kernel. */
	mlib_s32 height_bits;
/* The numbers of bits used for y position in kernel. */
	mlib_s32 vis_height_bits;
/* The horizontal coefficient data in f32 format. */
	mlib_f32 *dataH_f32;
/* The vertical   coefficient data in f32 format. */
	mlib_f32 *dataV_f32;
/* The horizontal coefficient data in d64 format. */
	mlib_d64 *dataH_d64;
/* The vertical   coefficient data in d64 format. */
	mlib_d64 *dataV_d64;
/* Shift value for VIS version */
	mlib_s32 shift_vis;
/* Shift value for VIS version */
	mlib_s32 shift_vis_affine;
/* The horizontal coefficient data in s16 format. */
	mlib_d64 *dataH_s16;
/* The vertical   coefficient data in s16 format. */
	mlib_d64 *dataV_s16;
/* The horizontal coefficient data in s16 format. */
	mlib_d64 *dataH_s16_1;
/* The vertical   coefficient data in s16 format. */
	mlib_d64 *dataV_s16_1;
/* The horizontal coefficient data in s16 format. */
	mlib_d64 *dataH_s16_3;
/* The horizontal coefficient data in s16 format. */
	mlib_d64 *dataH_s16_4;
} mlib_interp_table;

#define	mlib_ImageGetInterpWidth(interp_table)                        \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->width))

#define	mlib_ImageGetInterpHeight(interp_table)                        \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->height))

#define	mlib_ImageGetInterpLeftPadding(interp_table)            \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->    \
	    leftPadding))

#define	mlib_ImageGetInterpTopPadding(interp_table)                        \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->topPadding))

#define	mlib_ImageGetInterpSubsampleBitsH(interp_table)         \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->    \
	    subsampleBitsH))

#define	mlib_ImageGetInterpSubsampleBitsV(interp_table)         \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->    \
	    subsampleBitsV))

#define	mlib_ImageGetInterpPrecisionBits(interp_table)          \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->    \
	    precisionBits))

#define	mlib_ImageGetInterpWidthBits(interp_table)                         \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->width_bits))

#define	mlib_ImageGetInterpHeightBits(interp_table)             \
	((mlib_s32)(((mlib_interp_table *) (interp_table))->    \
	    height_bits))

#define	mlib_ImageGetInterpShortDataH(interp_table)              \
	((mlib_s16 *)(((mlib_interp_table *) (interp_table))->   \
	    dataH_s16))

#define	mlib_ImageGetInterpShortDataV(interp_table)              \
	((mlib_s16 *)(((mlib_interp_table *) (interp_table))->   \
	    dataV_s16))

#define	mlib_ImageGetInterpFloatDataH(interp_table)              \
	((mlib_f32 *)(((mlib_interp_table *) (interp_table))->   \
	    dataH_f32))

#define	mlib_ImageGetInterpFloatDataV(interp_table)              \
	((mlib_f32 *)(((mlib_interp_table *) (interp_table))->   \
	    dataV_f32))

#define	mlib_ImageGetInterpDoubleDataH(interp_table)             \
	((mlib_d64 *)(((mlib_interp_table *) (interp_table))->   \
	    dataH_d64))

#define	mlib_ImageGetInterpDoubleDataV(interp_table)             \
	((mlib_d64 *)(((mlib_interp_table *) (interp_table))->   \
	    dataV_d64))

#define	mlib_ImageGetInterpShortDataH_1(interp_table)            \
	((mlib_s16 *)(((mlib_interp_table *) (interp_table))->   \
	    dataH_s16_1))

#define	mlib_ImageGetInterpShortDataV_1(interp_table)            \
	((mlib_s16 *)(((mlib_interp_table *) (interp_table))->   \
	    dataV_s16_1))

#define	mlib_ImageGetInterpShortDataH_3(interp_table)            \
	((mlib_s16 *)(((mlib_interp_table *) (interp_table))->   \
	    dataH_s16_3))

#define	mlib_ImageGetInterpShortDataH_4(interp_table)            \
	((mlib_s16 *)(((mlib_interp_table *) (interp_table))->   \
	    dataH_s16_4))

/* *********************************************************** */

mlib_status mlib_ImageInitInterpTable_S16(
    mlib_interp_table *table);
mlib_status __mlib_ImageInitInterpTable_S16(
    mlib_interp_table *table);

mlib_status mlib_ImageInitInterpTableAffine_S16(
    mlib_interp_table *table,
    mlib_s32 nchan);
mlib_status __mlib_ImageInitInterpTableAffine_S16(
    mlib_interp_table *table,
    mlib_s32 nchan);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEINTERPTABLE_H */
