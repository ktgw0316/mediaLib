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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEARITH_H
#define	_MLIB_S_IMAGEARITH_H

#pragma ident	"@(#)mlib_s_ImageArith.h	9.5	07/11/05 SMI"

/*
 * Macro definitions for SSE version image arithmetic functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#ifndef EXTRA_VARS_U8
#define	EXTRA_VARS_U8
#endif

#ifndef EXTRA_VARS_S16
#define	EXTRA_VARS_S16
#endif

#ifndef EXTRA_VARS_U16
#define	EXTRA_VARS_U16
#endif

#ifndef EXTRA_VARS_S32
#define	EXTRA_VARS_S32
#endif

#ifndef EXTRA_VARS_F32
#define	EXTRA_VARS_F32
#endif

#ifndef EXTRA_VARS_D64
#define	EXTRA_VARS_D64
#endif

/* *********************************************************** */

#define	ARITH_FUNC(SSE_TYPE, TYPE)                                       \
	static void                                                      \
	mlib_s_ImageArith_##TYPE(mlib_u8 *dst, mlib_u8 *src1,            \
	    mlib_u8 *src2, mlib_s32 dlb, mlib_s32 slb1, mlib_s32 slb2,   \
	    mlib_s32 dw, mlib_s32 dh)                                    \
	{                                                                \
	    SSE_TYPE sd1, sd2, dd;                                       \
	    EXTRA_VARS_##TYPE mlib_u8 *dp, *dl, *dend;                   \
	    mlib_u8 *sp1, *sl1;                                          \
	    mlib_u8 *sp2, *sl2;                                          \
	    mlib_s32 nn, emask;                                          \
	                                                                 \
	    dp = dl = dst;                                               \
	    sp1 = sl1 = src1;                                            \
	    sp2 = sl2 = src2;                                            \
	    for (; dh > 0; dh--) {                                       \
		dp = (void *)((mlib_addr)dl & ~15);                      \
		nn = dp - dl;                                            \
		dend = dl + dw - 1;                                      \
		emask = sse_edge8(dl, dend);                             \
		sp1 += nn;                                               \
		sp2 += nn;                                               \
		if (emask != 0xffff) {                                   \
		    LOADU##SSE_TYPE(sd1, sp1);                           \
		    LOADU##SSE_TYPE(sd2, sp2);                           \
		    ARITH_OPER_##TYPE(dd, sd1, sd2);                     \
		    sse2_pst_8(dd, dp, emask);                            \
		    nn += 16;                                            \
		    dp += 16;                                            \
		    sp1 += 16;                                           \
		    sp2 += 16;                                           \
		}                                                        \
	                                                                 \
/*                                                                       \
 * 16-bytes loop                                                         \
 */                                                                      \
		if (!(((mlib_s32)sp1 | (mlib_s32)sp2) & 15)) {           \
		    for (; nn <= dw - 16; nn += 16) {                    \
			sd1 = *(SSE_TYPE *) sp1;                         \
			sd2 = *(SSE_TYPE *) sp2;                         \
			ARITH_OPER_##TYPE(dd, sd1, sd2);                 \
			*(SSE_TYPE *) dp = dd;                           \
			dp += 16;                                        \
			sp1 += 16;                                       \
			sp2 += 16;                                       \
		    }                                                    \
		} else {                                                 \
		    for (; nn <= dw - 16; nn += 16) {                    \
			LOADU##SSE_TYPE(sd1, sp1);                       \
			LOADU##SSE_TYPE(sd2, sp2);                       \
			ARITH_OPER_##TYPE(dd, sd1, sd2);                 \
			*(SSE_TYPE *) dp = dd;                           \
			dp += 16;                                        \
			sp1 += 16;                                       \
			sp2 += 16;                                       \
		    }                                                    \
		}                                                        \
		if (nn < dw) {                                           \
		    emask = sse_edge8(dp, dend);                         \
		    LOADU##SSE_TYPE(sd1, sp1);                           \
		    LOADU##SSE_TYPE(sd2, sp2);                           \
		    ARITH_OPER_##TYPE(dd, sd1, sd2);                     \
		    sse2_pst_8(dd, dp, emask);                            \
		}                                                        \
		dp = dl += dlb;                                          \
		sp1 = sl1 += slb1;                                       \
		sp2 = sl2 += slb2;                                       \
	    }                                                            \
	}

/* *********************************************************** */

ARITH_FUNC(__m128i, U8)
    ARITH_FUNC(__m128i, S16)
    ARITH_FUNC(__m128i, U16)
    ARITH_FUNC(__m128i, S32)
    ARITH_FUNC(__m128, F32)
    ARITH_FUNC(__m128d, D64)

/* *********************************************************** */
    mlib_status FUNCTION_NAME(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	void *da, *sa1, *sa2;
	mlib_s32 slb1, slb2, dlb, dw, dh, dchan;
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dlb, da);

	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa1 = mlib_ImageGetData(src1);
	sa2 = mlib_ImageGetData(src2);

	dw *= dchan;

	if (!mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		dw *= dh;
		slb1 *= dh;
		slb2 *= dh;
		dlb *= dh;
		dh = 1;
	}

	if (dtype == MLIB_BYTE) {
		mlib_s_ImageArith_U8(da, sa1, sa2, dlb, slb1, slb2, dw, dh);
	} else if (dtype == MLIB_SHORT) {
		mlib_s_ImageArith_S16(da, sa1, sa2, dlb, slb1, slb2, 2 * dw,
		    dh);
	} else if (dtype == MLIB_USHORT) {
		mlib_s_ImageArith_U16(da, sa1, sa2, dlb, slb1, slb2, 2 * dw,
		    dh);
	} else if (dtype == MLIB_INT) {
		mlib_s_ImageArith_S32(da, sa1, sa2, dlb, slb1, slb2, 4 * dw,
		    dh);
	} else {
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEARITH_H */
