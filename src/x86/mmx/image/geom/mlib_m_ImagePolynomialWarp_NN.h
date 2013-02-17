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

#ifndef _MLIB_M_IMAGEPOLYNOMIALWARP_NN_H
#define	_MLIB_M_IMAGEPOLYNOMIALWARP_NN_H

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_NN.h	9.2	07/11/05 SMI"

#endif /* _MLIB_M_IMAGEPOLYNOMIALWARP_NN_H */

#ifdef __cplusplus
extern "C" {
#endif

#define	NAME(X)	mlib_ImagePolynomialWarp_##X##_NN_U8_1

/* *********************************************************** */
#define	LOCAL_VARS(TYPE)                                        \
	TYPE *sp, *dpt;                                         \
	mlib_s32 i, j, iwx, iwy, mask;
/* *********************************************************** */
#define	SAVE1	dpt[0] = sp[0];
/* *********************************************************** */
#define	SAVE2	dpt[0] = sp[0]; dpt[1] = sp[1];
/* *********************************************************** */
#define	SAVE3	dpt[0] = sp[0]; dpt[1] = sp[1]; dpt[2] = sp[2];
/* *********************************************************** */
#define	SAVE4                                                   \
	dpt[0] = sp[0];                                         \
	dpt[1] = sp[1];                                         \
	dpt[2] = sp[2];                                         \
	dpt[3] = sp[3];
/* *********************************************************** */
#define	FUNC(TYPE, chan, mul)                                        \
	{                                                            \
	    LOCAL_VARS(TYPE) VARS y = 0.5 + preShiftY;               \
	                                                             \
	    srcHeight++;                                             \
	    srcWidth++;                                              \
	    for (j = 0; j < dstHeight; j++) {                        \
		x = 0.5 + preShiftX;                                 \
		dpt = (TYPE *) dstData;                              \
		INIT_COEFFS for (i = 0; i < dstWidth; i++) {         \
		    iwx = (mlib_s32)wx;                              \
		    iwy = (mlib_s32)wy;                              \
		    mask =                                           \
			(~(iwx - 1) & (iwx - srcWidth) & ~(iwy -     \
			1) & (iwy - srcHeight)) >> 31;               \
		    if (mask != 0) {                                 \
			sp = (TYPE *) (lineAddr[iwy] + iwx * mul);   \
		    SAVE##chan}                                      \
		    DELTA dpt += chan;                               \
		}                                                    \
		y += 1.0;                                            \
		dstData += dstStride;                                \
	    }                                                        \
	}
/* *********************************************************** */
DEF_FUNC_2(NAME_U8_1)
    FUNC(mlib_u8,
    1,
    1)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_U8_2)
{
	CALL_FUNC(NAME_S16_1);
}

/* *********************************************************** */
DEF_FUNC_2(NAME_U8_3)
    FUNC(mlib_u8,
    3,
    3)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_U8_4)
{
	CALL_FUNC(NAME_S32_1);
}

/* *********************************************************** */
DEF_FUNC_2(NAME_S16_1)
    FUNC(mlib_u16,
    1,
    2)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_S16_2)
{
	CALL_FUNC(NAME_S32_1);
}

/* *********************************************************** */
DEF_FUNC_2(NAME_S16_3)
    FUNC(mlib_u16,
    3,
    6)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_S16_4)
{
	CALL_FUNC(NAME_S32_2);
}

/* *********************************************************** */
DEF_FUNC_2(NAME_S32_1)
    FUNC(mlib_s32,
    1,
    4)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_S32_2)
FUNC(
    mlib_s32,
    2,
    8)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_S32_3)
FUNC(
    mlib_s32,
    3,
    12)

/* *********************************************************** */
DEF_FUNC_2(
    NAME_S32_4)
FUNC(
    mlib_s32,
    4,
    16)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif
