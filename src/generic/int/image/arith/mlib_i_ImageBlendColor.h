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

#ifndef _MLIB_I_IMAGEBLENDCOLOR_H
#define	_MLIB_I_IMAGEBLENDCOLOR_H

#pragma ident	"@(#)mlib_i_ImageBlendColor.h	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal macro for mlib_ImageBlendColor.
 *
 *   Also file contains macro of C implementation for any type:
 *     FUNC_NAME define must be set to name of internal function.
 *     DTYPE define must be set to data type of image.
 *     FTYPE define must be set to type of floating-point operations.
 *     TTYPE define must be set to type of internal arithmetic.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>
#ifdef _NO_LONGLONG
#include <mlib_ImageDivTables.h>
#endif /* _NO_LONGLONG */

/* *********************************************************** */

#define	DEF_FUNC(FUNC_NAME, DTYPE, COLOR_TYPE)                            \
	mlib_status                                                       \
	FUNC_NAME(const DTYPE * sl, DTYPE * dl, mlib_s32 sstride,         \
	    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,            \
	    mlib_s32 channel, mlib_s32 alpha, const COLOR_TYPE * color)

DEF_FUNC(mlib_ImageBlendColor_U8, mlib_u8,
	mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_S16, mlib_s16,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_U16, mlib_u16,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_S32, mlib_s32,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_F32, mlib_f32,
    mlib_d64);
DEF_FUNC(mlib_ImageBlendColor_D64, mlib_d64,
    mlib_d64);

DEF_FUNC(mlib_ImageBlendColor_Inp_U8, mlib_u8,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_Inp_S16, mlib_s16,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_Inp_U16, mlib_u16,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_Inp_S32, mlib_s32,
    mlib_s32);
DEF_FUNC(mlib_ImageBlendColor_Inp_F32, mlib_f32,
    mlib_d64);
DEF_FUNC(mlib_ImageBlendColor_Inp_D64, mlib_d64,
    mlib_d64);

#ifndef _NO_LONGLONG

/* *********************************************************** */
#define	LINE_BLEND_CH2(sl, ALP, S_1)                            \
	fcol1 =  color[S_1];                                    \
	for (i = 0; i < width; i++) {                           \
	    fa = sl[2 * i + ALP];                               \
	    fsrc1 = sl[2 * i + S_1];                            \
	    dl[2 * i + ALP] = MAX_VALUE;                        \
	    dl[2 * i + S_1] = BLEND_COLOR(fsrc1, fcol1);        \
	}                                                       \
	                                                        \
	sl += sstride;                                          \
	dl += dstride
/* *********************************************************** */

#define	LINE_BLEND_CH3(sl, ALP, S_1, S_2)                       \
	fcol1 = (TTYPE) color[S_1];                             \
	fcol2 = (TTYPE) color[S_2];                             \
	for (i = 0; i < width; i++) {                           \
	    fa = sl[3 * i + ALP];                               \
	    fsrc1 = sl[3 * i + S_1];                            \
	    fsrc2 = sl[3 * i + S_2];                            \
	    dl[3 * i + ALP] = MAX_VALUE;                        \
	    dl[3 * i + S_1] = BLEND_COLOR(fsrc1, fcol1);        \
	    dl[3 * i + S_2] = BLEND_COLOR(fsrc2, fcol2);        \
	}                                                       \
	                                                        \
	sl += sstride;                                          \
	dl += dstride

/* *********************************************************** */

#define	LINE_BLEND_CH4(sl, ALP, S_1, S_2, S_3)                  \
	fcol1 = (TTYPE) color[S_1];                             \
	fcol2 = (TTYPE) color[S_2];                             \
	fcol3 = (TTYPE) color[S_3];                             \
	for (i = 0; i < width; i++) {                           \
	    fa = sl[4 * i + ALP];                               \
	    fsrc1 = sl[4 * i + S_1];                            \
	    fsrc2 = sl[4 * i + S_2];                            \
	    fsrc3 = sl[4 * i + S_3];                            \
	    dl[4 * i + ALP] = MAX_VALUE;                        \
	    dl[4 * i + S_1] = BLEND_COLOR(fsrc1, fcol1);        \
	    dl[4 * i + S_2] = BLEND_COLOR(fsrc2, fcol2);        \
	    dl[4 * i + S_3] = BLEND_COLOR(fsrc3, fcol3);        \
	}                                                       \
	                                                        \
	sl += sstride;                                          \
	dl += dstride

/* *********************************************************** */

#else /* _NO_LONGLONG */

/* *********************************************************** */

#define	LINE_BLEND_CH2(sl, ALP, S_1)                            \
	fcol1 = (TTYPE) color[S_1];                             \
	for (i = 0; i < width; i++) {                           \
	    fa = LD_ALPHA(sl[2 * i + ALP]);                     \
	    fsrc1 = TBL(sl[2 * i + S_1]);                       \
	    dl[2 * i + ALP] = MAX_VALUE;                        \
	    dl[2 * i + S_1] = BLEND_COLOR(fsrc1, fcol1);        \
	}                                                       \
	                                                        \
	sl += sstride;                                          \
	dl += dstride

/* *********************************************************** */

#define	LINE_BLEND_CH3(sl, ALP, S_1, S_2)                       \
	fcol1 = (TTYPE) color[S_1];                             \
	fcol2 = (TTYPE) color[S_2];                             \
	for (i = 0; i < width; i++) {                           \
	    fa = LD_ALPHA(sl[3 * i + ALP]);                     \
	    fsrc1 = TBL(sl[3 * i + S_1]);                       \
	    fsrc2 = TBL(sl[3 * i + S_2]);                       \
	    dl[3 * i + ALP] = MAX_VALUE;                        \
	    dl[3 * i + S_1] = BLEND_COLOR(fsrc1, fcol1);        \
	    dl[3 * i + S_2] = BLEND_COLOR(fsrc2, fcol2);        \
	}                                                       \
	                                                        \
	sl += sstride;                                          \
	dl += dstride

/* *********************************************************** */

#define	LINE_BLEND_CH4(sl, ALP, S_1, S_2, S_3)                  \
	fcol1 = (TTYPE) color[S_1];                             \
	fcol2 = (TTYPE) color[S_2];                             \
	fcol3 = (TTYPE) color[S_3];                             \
	for (i = 0; i < width; i++) {                           \
	    fa = LD_ALPHA(sl[4 * i + ALP]);                     \
	    fsrc1 = TBL(sl[4 * i + S_1]);                       \
	    fsrc2 = TBL(sl[4 * i + S_2]);                       \
	    fsrc3 = TBL(sl[4 * i + S_3]);                       \
	    dl[4 * i + ALP] = MAX_VALUE;                        \
	    dl[4 * i + S_1] = BLEND_COLOR(fsrc1, fcol1);        \
	    dl[4 * i + S_2] = BLEND_COLOR(fsrc2, fcol2);        \
	    dl[4 * i + S_3] = BLEND_COLOR(fsrc3, fcol3);        \
	}                                                       \
	                                                        \
	sl += sstride;                                          \
	dl += dstride

/* *********************************************************** */

#endif /* _NO_LONGLONG */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGEBLENDCOLOR_H */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _NO_LONGLONG
#ifndef TBL
#define	TBL(x)	(x)
#endif /* TBL */
#ifndef LD_ALPHA
#define	LD_ALPHA(x)	(x)
#endif /* LD_ALPHA */
#endif /* _NO_LONGLONG */

#ifndef SRC_PTR
#define	SRC_PTR	sl
#endif /* SRC_PTR */

/* *********************************************************** */
#ifdef FUNC_NAME
DEF_FUNC(
    FUNC_NAME,
    DTYPE,
    COLOR_TYPE)
{
	mlib_s32 i, j;

#ifdef ONE
	FTYPE fone = ONE;
#endif /* ONE */
	TTYPE fsrc1, fsrc2, fsrc3;
	TTYPE fcol1, fcol2, fcol3;
	TTYPE fa;

	if (channel == 2) {
		if (alpha == 0) {

			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH2(SRC_PTR, 0, 1);
			}

			return (MLIB_SUCCESS);
		} else {
/* alpha == 1 */
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH2(SRC_PTR, 1, 0);
			}

			return (MLIB_SUCCESS);
		}

	} else if (channel == 3) {

		if (alpha == 0) {
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH3(SRC_PTR, 0, 1, 2);
			}

			return (MLIB_SUCCESS);

		} else if (alpha == 1) {
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH3(SRC_PTR, 1, 2, 0);
			}

			return (MLIB_SUCCESS);

		} else {
/* alpha == 2 */
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH3(SRC_PTR, 2, 0, 1);
			}

			return (MLIB_SUCCESS);
		}

	} else {
/* channel == 4 */

		if (alpha == 0) {
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH4(SRC_PTR, 0, 1, 2, 3);
			}

			return (MLIB_SUCCESS);

		} else if (alpha == 1) {
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH4(SRC_PTR, 1, 2, 3, 0);
			}

			return (MLIB_SUCCESS);

		} else if (alpha == 2) {
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH4(SRC_PTR, 2, 3, 0, 1);
			}

			return (MLIB_SUCCESS);

		} else {
/* alpha == 3 */
			for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				LINE_BLEND_CH4(SRC_PTR, 3, 0, 1, 2);
			}

			return (MLIB_SUCCESS);
		}
	}
}

#endif /* FUNC_NAME */

/* *********************************************************** */

#ifdef _NO_LONGLONG
#undef  TBL
#undef  LD_ALPHA
#endif /* _NO_LONGLONG */

#undef  FUNC_NAME
#undef  SHIFT
#undef  MAX_VALUE
#undef  ONE
#undef  DTYPE
#undef  TTYPE
#undef  FTYPE
#undef  BLEND_COLOR

#ifdef __cplusplus
}
#endif
