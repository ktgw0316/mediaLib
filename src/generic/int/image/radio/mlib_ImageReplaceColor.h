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

#ifndef _MLIB_IMAGEREPLACECOLOR_H
#define	_MLIB_IMAGEREPLACECOLOR_H

#pragma ident	"@(#)mlib_ImageReplaceColor.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>

#if defined(_NO_LONGLONG)

/* *********************************************************** */
#define	DEF_FUNC(FUNC_NAME, DTYPE, COLOR_TYPE)                      \
	static mlib_status                                          \
	FUNC_NAME(const DTYPE * sl, DTYPE * dl, mlib_s32 sstride,   \
	    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,      \
	    mlib_s32 nchan, const COLOR_TYPE * color1,              \
	    const COLOR_TYPE * color2)

DEF_FUNC(mlib_ImageReplaceColor_U8, mlib_u8, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_S16, mlib_s16, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_U16, mlib_u16, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_S32, mlib_s32, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_F32, mlib_f32, mlib_d64);
DEF_FUNC(mlib_ImageReplaceColor_D64, mlib_d64, mlib_d64);

/* *********************************************************** */
#else
/* *********************************************************** */

#define	DEF_FUNC(FUNC_NAME, DTYPE, COLOR_TYPE)                      \
	static mlib_status                                          \
	FUNC_NAME(const DTYPE * sl, DTYPE * dl, mlib_s32 sstride,   \
	    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,      \
	    mlib_s32 nchan, const COLOR_TYPE * color1,              \
	    const COLOR_TYPE * color2)

DEF_FUNC(mlib_ImageReplaceColor_U8, mlib_u8, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_S16, mlib_s16, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_U16, mlib_u16, mlib_s32);
DEF_FUNC(mlib_ImageReplaceColor_S32, mlib_s32, mlib_s32);

static mlib_status
mlib_ImageReplaceColor_F32(
	const mlib_f32 * sl, mlib_f32 * dl, mlib_s32 sstride,
    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,
    mlib_s32 nchan, const mlib_d64 * color1,
    const mlib_d64 * color2);

static mlib_status
mlib_ImageReplaceColor_D64(
	const mlib_d64 * sl, mlib_d64 * dl, mlib_s32 sstride,
    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,
    mlib_s32 nchan, const mlib_d64 * color1,
    const mlib_d64 * color2);

/* *********************************************************** */

#endif /* _NO_LONGLONG */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEREPLACECOLOR_H */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FUNC_NAME
/*
 * FUNCTION
 *      Internal macro for mlib_ImageReplaceColor.
 *
 *   Also file contains macro of C implementation for any type:
 *     FUNC_NAME define must be set to name of internal function.
 *     DTYPE define must be set to data type of image.
 *     TTYPE define must be set to type of internal arithmetic.
 *
 */
DEF_FUNC(
    FUNC_NAME,
    DTYPE,
    COLOR_TYPE)
{
	mlib_s32 i, j;
	TTYPE s0, s1, s2, s3;

	if (nchan == 1) {
		GET_CONST(0);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= width - 4; i += 4) {
				s0 = sl[i];
				dl[i] = PROCESS_CH(0);
				s0 = sl[i + 1];
				dl[i + 1] = PROCESS_CH(0);
				s0 = sl[i + 2];
				dl[i + 2] = PROCESS_CH(0);
				s0 = sl[i + 3];
				dl[i + 3] = PROCESS_CH(0);
			}

			for (; i < width; i++) {
				s0 = sl[i];
				dl[i] = PROCESS_CH(0);
			}

			sl += sstride;
			dl += dstride;
		}

	} else if (nchan == 2) {
		GET_CONST(0);
		GET_CONST(1);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sl[2 * i];
				s1 = sl[2 * i + 1];
				dl[2 * i] = PROCESS_CH(0);
				dl[2 * i + 1] = PROCESS_CH(1);
			}

			sl += sstride;
			dl += dstride;
		}

	} else if (nchan == 3) {
		GET_CONST(0);
		GET_CONST(1);
		GET_CONST(2);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sl[3 * i];
				s1 = sl[3 * i + 1];
				s2 = sl[3 * i + 2];
				dl[3 * i] = PROCESS_CH(0);
				dl[3 * i + 1] = PROCESS_CH(1);
				dl[3 * i + 2] = PROCESS_CH(2);
			}

			sl += sstride;
			dl += dstride;
		}

	} else {	/* if (nchan == 4) */

		GET_CONST(0);
		GET_CONST(1);
		GET_CONST(2);
		GET_CONST(3);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sl[4 * i];
				s1 = sl[4 * i + 1];
				s2 = sl[4 * i + 2];
				s3 = sl[4 * i + 3];
				dl[4 * i] = PROCESS_CH(0);
				dl[4 * i + 1] = PROCESS_CH(1);
				dl[4 * i + 2] = PROCESS_CH(2);
				dl[4 * i + 3] = PROCESS_CH(3);
			}

			sl += sstride;
			dl += dstride;
		}
	}

	return (MLIB_SUCCESS);
}

#endif /* FUNC_NAME */

#undef FUNC_NAME
#undef DTYPE

#ifdef __cplusplus
}
#endif
