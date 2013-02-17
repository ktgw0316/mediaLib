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

#ifndef _MLIB_IMAGEPOLYNOMIALWARPTABLE_F_H
#define	_MLIB_IMAGEPOLYNOMIALWARPTABLE_F_H

#pragma ident	"@(#)mlib_ImagePolynomialWarpTable_f.h	9.2	07/10/09 SMI"

#endif /* _MLIB_IMAGEPOLYNOMIALWARPTABLE_F_H */

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#if IMAGE_FORMAT == 1

#define	IMAGE_TYPE	mlib_u8
#define	IMAGE_CLASS	U8
#ifndef _USE_MLIB_U82D64_
#define	_USE_MLIB_U82D64_
#endif		   /* _USE_MLIB_U82D64_ */
#define	TABLE(x)	(*(mlib_d64 *)((mlib_u8 *)mlib_U82D64 + ((x) << 3)))

#elif IMAGE_FORMAT == 2	/* IMAGE_FORMAT == 1 */

#if ! defined(IMAGE_FORMAT_EXT)
#define	IMAGE_TYPE	mlib_s16
#define	IMAGE_CLASS	S16
#else		   /* ! defined(IMAGE_FORMAT_EXT) */
#define	IMAGE_TYPE	mlib_u16
#define	IMAGE_CLASS	U16
#endif		   /* ! defined(IMAGE_FORMAT_EXT) */
#define	TABLE(x)	(x)

#elif IMAGE_FORMAT == 3	/* IMAGE_FORMAT == 1 */

#define	IMAGE_TYPE	mlib_s32
#define	IMAGE_CLASS	S32
#define	TABLE(x)	(x)

#elif IMAGE_FORMAT == 4	/* IMAGE_FORMAT == 1 */

#define	IMAGE_TYPE	mlib_f32
#define	IMAGE_CLASS	F32
#define	TABLE(x)	(x)

#elif IMAGE_FORMAT == 5	/* IMAGE_FORMAT == 1 */

#define	IMAGE_TYPE	mlib_d64
#define	IMAGE_CLASS	D64
#define	TABLE(x)	(x)
#else		   /* IMAGE_FORMAT == 1 */
#error "*** ERROR: Not correct value for IMAGE_FORMAT"
#endif		   /* IMAGE_FORMAT == 1 */

/* *********************************************************** */

#define	REAL_NAME(X, Y, Z)	mlib_IPWT_##Y##_##X##x##Z

/* *********************************************************** */

#define	CALC_4                                                       \
	(TABLE(line[off0]) * pH[0] + TABLE(line[off1]) * pH[1] +     \
	    TABLE(line[off2]) * pH[2] + TABLE(line[off3]) * pH[3])

/* *********************************************************** */

#define	CALC_3                                                     \
	(TABLE(line[off0]) * pH[0] + TABLE(line[off1]) * pH[1] +   \
	    TABLE(line[off2]) * pH[2])

/* *********************************************************** */

#define	CALC_2                                                    \
	(TABLE(line[off0]) * pH[0] + TABLE(line[off1]) * pH[1])

/* *********************************************************** */

#define	CALC_1	(TABLE(line[off0]) * pH[0])

/* *********************************************************** */

#if KERNEL_X == 1
#define	CALC_LINE	CALC_1
#elif KERNEL_X == 2	/* KERNEL_X == 1 */
#define	CALC_LINE	CALC_2
#elif KERNEL_X == 3	/* KERNEL_X == 1 */
#define	CALC_LINE	CALC_3
#elif KERNEL_X == 4	/* KERNEL_X == 1 */
#define	CALC_LINE	CALC_4
#else		   /* KERNEL_X == 1 */
#error "*** ERROR: Not correct value for KERNEL_X"
#endif		   /* KERNEL_X == 1 */

/* *********************************************************** */

#if KERNEL_Y == 1
#define	DECLARE_FUNCTION(X, Y)	REAL_NAME(X, Y, 1)
#elif KERNEL_Y == 2	/* KERNEL_Y == 1 */
#define	DECLARE_FUNCTION(X, Y)	REAL_NAME(X, Y, 2)
#elif KERNEL_Y == 3	/* KERNEL_Y == 1 */
#define	DECLARE_FUNCTION(X, Y)	REAL_NAME(X, Y, 3)
#elif KERNEL_Y == 4	/* KERNEL_Y == 1 */
#define	DECLARE_FUNCTION(X, Y)	REAL_NAME(X, Y, 4)
#else		   /* KERNEL_Y == 1 */
#error "*** ERROR: Not correct value for KERNEL_Y"
#endif		   /* KERNEL_Y == 1 */

/* *********************************************************** */

void DECLARE_FUNCTION(
    KERNEL_X,
    IMAGE_CLASS) (
    void *ps,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n,
    mlib_s32 offH,
    mlib_s32 offV)
{
	mlib_d64 *pH, *pV;
	mlib_d64 *dstData;
	mlib_d64 r0;
	mlib_addr *dsrc_x = pws->dsrc_x;
	mlib_addr *dsrc_y = pws->dsrc_y;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 ch, i;
	mlib_s32 channels = pws->channels;
	IMAGE_TYPE *line;

#if KERNEL_Y > 1
	mlib_s32 stride = (lineAddr[*ys + 1] - lineAddr[*ys]);
#endif /* KERNEL_Y > 1 */
	mlib_s32 off0 = 0;

#if KERNEL_X > 1
	mlib_s32 off1 = channels;
#endif /* KERNEL_X > 1 */
#if KERNEL_X > 2
	mlib_s32 off2 = channels * 2;
#endif /* KERNEL_X > 2 */
#if KERNEL_X > 3
	mlib_s32 off3 = channels * 3;
#endif /* KERNEL_X > 3 */

	for (ch = 0; ch < channels; ch++) {
		dstData = (mlib_d64 *)ps + ch;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {

			pV = (mlib_d64 *)(dsrc_y[i]) + offV;
			pH = (mlib_d64 *)(dsrc_x[i]) + offH;

			line =
			    (IMAGE_TYPE *) lineAddr[ys[i] + offV] + (xs[i] +
			    offH) * channels + ch;
			r0 = CALC_LINE * pV[0];

#if KERNEL_Y > 1
			line = (IMAGE_TYPE *) ((mlib_u8 *)line + stride);
			r0 += CALC_LINE * pV[1];
#endif /* KERNEL_Y > 1 */

#if KERNEL_Y > 2
			line = (IMAGE_TYPE *) ((mlib_u8 *)line + stride);
			r0 += CALC_LINE * pV[2];
#endif /* KERNEL_Y > 2 */

#if KERNEL_Y > 3
			line = (IMAGE_TYPE *) ((mlib_u8 *)line + stride);
			r0 += CALC_LINE * pV[3];
#endif /* KERNEL_Y > 3 */

			dstData[0] += r0;
			dstData += channels;
		}
	}
}

/* *********************************************************** */

#undef KERNEL_Y
#undef KERNEL_X

#undef CALC_LINE
#undef CALC_4
#undef CALC_3
#undef CALC_2
#undef CALC_1

#undef REAL_NAME
#undef DECLARE_FUNCTION

#undef IMAGE_TYPE
#undef IMAGE_CLASS

#undef TABLE

/* *********************************************************** */

#ifdef __cplusplus
}
#endif
