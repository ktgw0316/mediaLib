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

#ifndef _MLIB_S_IMAGECONSTARITH_H
#define	_MLIB_S_IMAGECONSTARITH_H

#pragma ident	"@(#)mlib_s_ImageConstArith.h	9.5	07/11/05 SMI"

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

#define	ARITH_FUNC(SSE_TYPE, TYPE)                                        \
	static void                                                       \
	mlib_s_ImageArith_##TYPE##_3(mlib_u8 *dst, mlib_u8 *src,          \
	    mlib_u8 *p_add, mlib_u8 *p_sub, mlib_s32 dlb, mlib_s32 slb,   \
	    mlib_s32 dw, mlib_s32 dh)                                     \
	{                                                                 \
	    SSE_TYPE dd0, dd1, dd2;                                       \
	    SSE_TYPE sd0, sd1, sd2;                                       \
	    SSE_TYPE add0, add1, add2;                                    \
	    SSE_TYPE sub0, sub1, sub2;                                    \
	    mlib_u8 *dp, *dl, *dend;                                      \
	    mlib_u8 *sp, *sl;                                             \
	    mlib_s32 nn, emask;                                           \
	                                                                  \
	    dp = dl = dst;                                                \
	    sp = sl = src;                                                \
	    for (; dh > 0; dh--) {                                        \
		dp = (void *)((mlib_addr)dl & ~15);                       \
		nn = dp - dl;                                             \
		dend = dl + dw - 1;                                       \
		emask = sse_edge8(dl, dend);                              \
		sp += nn;                                                 \
		LOADU##SSE_TYPE(add0, p_add + (nn % 12));                 \
		LOADU##SSE_TYPE(sub0, p_sub + (nn % 12));                 \
		if (emask != 0xffff) {                                    \
		    LOADU##SSE_TYPE(sd0, sp);                             \
		    ARITH_OPER_##TYPE(dd0, sd0, add0, sub0);              \
		    sse2_pst_8(dd0, dp, emask);                            \
		    nn += 16;                                             \
		    dp += 16;                                             \
		    sp += 16;                                             \
		}                                                         \
		LOADU##SSE_TYPE(add0, p_add + (nn % 12));                 \
		LOADU##SSE_TYPE(sub0, p_sub + (nn % 12));                 \
		LOADU##SSE_TYPE(add1, p_add + ((nn + 16) % 12));          \
		LOADU##SSE_TYPE(sub1, p_sub + ((nn + 16) % 12));          \
		LOADU##SSE_TYPE(add2, p_add + ((nn + 32) % 12));          \
		LOADU##SSE_TYPE(sub2, p_sub + ((nn + 32) % 12));          \
		for (; nn <= dw - 3 * 16; nn += 3 * 16) {                 \
		    LOADU##SSE_TYPE(sd0, sp);                             \
		    LOADU##SSE_TYPE(sd1, sp + 16);                        \
		    LOADU##SSE_TYPE(sd2, sp + 32);                        \
		    ARITH_OPER_##TYPE(dd0, sd0, add0, sub0);              \
		    ARITH_OPER_##TYPE(dd1, sd1, add1, sub1);              \
		    ARITH_OPER_##TYPE(dd2, sd2, add2, sub2);              \
		    ((SSE_TYPE *) dp)[0] = dd0;                           \
		    ((SSE_TYPE *) dp)[1] = dd1;                           \
		    ((SSE_TYPE *) dp)[2] = dd2;                           \
		    dp += 3 * 16;                                         \
		    sp += 3 * 16;                                         \
		}                                                         \
		if (nn < dw) {                                            \
		    emask = sse_edge8(dp, dend);                          \
		    LOADU##SSE_TYPE(sd0, sp);                             \
		    ARITH_OPER_##TYPE(dd0, sd0, add0, sub0);              \
		    sse2_pst_8(dd0, dp, emask);                            \
		    nn += 16;                                             \
		    dp += 16;                                             \
		    sp += 16;                                             \
		}                                                         \
		if (nn < dw) {                                            \
		    emask = sse_edge8(dp, dend);                          \
		    LOADU##SSE_TYPE(sd1, sp);                             \
		    ARITH_OPER_##TYPE(dd1, sd1, add1, sub1);              \
		    sse2_pst_8(dd1, dp, emask);                            \
		    nn += 16;                                             \
		    dp += 16;                                             \
		    sp += 16;                                             \
		}                                                         \
		if (nn < dw) {                                            \
		    emask = sse_edge8(dp, dend);                          \
		    LOADU##SSE_TYPE(sd2, sp);                             \
		    ARITH_OPER_##TYPE(dd2, sd2, add2, sub2);              \
		    sse2_pst_8(dd2, dp, emask);                            \
		    nn += 16;                                             \
		    dp += 16;                                             \
		    sp += 16;                                             \
		}                                                         \
		dp = dl += dlb;                                           \
		sp = sl += slb;                                           \
	    }                                                             \
	}                                                                 \
	                                                                  \
/*                                                                        \
 * ************************************************************           \
 */                                                                       \
	static void                                                       \
	mlib_s_ImageArith_##TYPE(mlib_u8 *dst, mlib_u8 *src,              \
	    mlib_u8 *p_add, mlib_u8 *p_sub, mlib_s32 dlb, mlib_s32 slb,   \
	    mlib_s32 dw, mlib_s32 dh, mlib_s32 nchan)                     \
	{                                                                 \
	    SSE_TYPE dd, sd1, add0, sub0;                                 \
	    mlib_u8 *dp, *dl, *dend;                                      \
	    mlib_u8 *sp, *sl;                                             \
	    mlib_s32 nn, emask;                                           \
	                                                                  \
	    dp = dl = dst;                                                \
	    sp = sl = src;                                                \
	    if (nchan == 3) {                                             \
		mlib_s_ImageArith_##TYPE##_3(dst, src, p_add,             \
		    p_sub, dlb, slb, dw, dh);                             \
		return;                                                   \
	    }                                                             \
	    for (; dh > 0; dh--) {                                        \
		dp = (void *)((mlib_addr)dl & ~15);                       \
		nn = dp - dl;                                             \
		dend = dl + dw - 1;                                       \
		emask = sse_edge8(dl, dend);                              \
		sp += nn;                                                 \
		LOADU##SSE_TYPE(add0, p_add + (nn & 15));                 \
		LOADU##SSE_TYPE(sub0, p_sub + (nn & 15));                 \
		if (emask != 0xffff) {                                    \
		    LOADU##SSE_TYPE(sd1, sp);                             \
		    ARITH_OPER_##TYPE(dd, sd1, add0, sub0);               \
		    sse2_pst_8(dd, dp, emask);                             \
		    nn += 16;                                             \
		    dp += 16;                                             \
		    sp += 16;                                             \
		}                                                         \
	                                                                  \
/*                                                                        \
 * 16-bytes loop                                                          \
 */                                                                       \
		if (!((mlib_s32)sp & 15)) {                               \
		    for (; nn <= dw - 16; nn += 16) {                     \
			sd1 = *(SSE_TYPE *) sp;                           \
			ARITH_OPER_##TYPE(dd, sd1, add0, sub0);           \
			*(SSE_TYPE *) dp = dd;                            \
			dp += 16;                                         \
			sp += 16;                                         \
		    }                                                     \
		} else {                                                  \
		    for (; nn <= dw - 16; nn += 16) {                     \
			LOADU##SSE_TYPE(sd1, sp);                         \
			ARITH_OPER_##TYPE(dd, sd1, add0, sub0);           \
			*(SSE_TYPE *) dp = dd;                            \
			dp += 16;                                         \
			sp += 16;                                         \
		    }                                                     \
		}                                                         \
		if (nn < dw) {                                            \
		    emask = sse_edge8(dp, dend);                          \
		    LOADU##SSE_TYPE(sd1, sp);                             \
		    ARITH_OPER_##TYPE(dd, sd1, add0, sub0);               \
		    sse2_pst_8(dd, dp, emask);                             \
		}                                                         \
		dp = dl += dlb;                                           \
		sp = sl += slb;                                           \
	    }                                                             \
	}

/* *********************************************************** */

ARITH_FUNC(__m128i, U8)
    ARITH_FUNC(__m128i, S16)
    ARITH_FUNC(__m128i, U16)
    ARITH_FUNC(__m128i, S32)
    ARITH_FUNC(__m128, F32)
    ARITH_FUNC(__m128d, D64)

/* *********************************************************** */
#if IMAGESUB_FLAG
#define	ARITH_FUN(x, c)	((c) - (x))
#else
#define	ARITH_FUN(x, c)	((x) + (c))
#endif
/* *********************************************************** */
    static void mlib_ImageConstArith_D64(
    mlib_d64 *dl,
    mlib_d64 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *consts)
{
	mlib_s32 i, j;
	mlib_d64 c0, c1, c2, c3;

	c0 = consts[0];
	c1 = consts[1];
	c2 = consts[2];
	c3 = consts[3];

	if (nchan != 3) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= xsize - 4; i += 4) {
				dl[i] = ARITH_FUN(sl[i], c0);
				dl[i + 1] = ARITH_FUN(sl[i + 1], c1);
				dl[i + 2] = ARITH_FUN(sl[i + 2], c2);
				dl[i + 3] = ARITH_FUN(sl[i + 3], c3);
			}

			if (i < xsize) {
				dl[i] = ARITH_FUN(sl[i], c0);
				i++;
			}

			if (i < xsize) {
				dl[i] = ARITH_FUN(sl[i], c1);
				i++;
			}

			if (i < xsize) {
				dl[i] = ARITH_FUN(sl[i], c2);
				i++;
			}

			sl += slb;
			dl += dlb;
		}
	} else {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= xsize - 3; i += 3) {
				dl[i] = ARITH_FUN(sl[i], c0);
				dl[i + 1] = ARITH_FUN(sl[i + 1], c1);
				dl[i + 2] = ARITH_FUN(sl[i + 2], c2);
			}

			sl += slb;
			dl += dlb;
		}
	}
}

/* *********************************************************** */

#define	CALC_CONST(TYPE, CTYPE)                                  \
	for (i = 0; i < dchan; i++) {                            \
	    ((TYPE *) c_add)[i] = ((CTYPE *) c)[i];              \
	}                                                        \
	                                                         \
	for (i = dchan; i < 32 / sizeof (TYPE); i++) {           \
	    ((TYPE *) c_add)[i] = ((TYPE *) c_add)[i - dchan];   \
	}

/* *********************************************************** */

#define	CALC_CONST2(TYPE, TMIN, TMAX)                             \
	for (i = 0; i < dchan; i++) {                             \
	    mlib_s32 cc = ((mlib_s32 *)c)[i];                     \
	                                                          \
	    ((TYPE *) c_add)[i] =                                 \
		(cc > TMAX) ? TMAX : (cc < TMIN) ? TMIN : cc;     \
	    cc -= ((TYPE *) c_add)[i];                            \
	    if (IMAGESUB_FLAG == 0)                               \
		cc = (cc == MLIB_S32_MIN) ? MLIB_S32_MAX : -cc;   \
	    ((TYPE *) c_sub)[i] =                                 \
		(cc > TMAX) ? TMAX : (cc < TMIN) ? TMIN : cc;     \
	}                                                         \
	                                                          \
	for (i = dchan; i < 32 / sizeof (TYPE); i++) {            \
	    ((TYPE *) c_add)[i] = ((TYPE *) c_add)[i - dchan];    \
	    ((TYPE *) c_sub)[i] = ((TYPE *) c_sub)[i - dchan];    \
	}

/* *********************************************************** */

static mlib_status
mlib_ImageConstArith(
    mlib_image *dst,
    const mlib_image *src,
    const void *c,
    mlib_s32 fp_flag)
{
	__m128i buff[4];
	void *c_add = buff, *c_sub = buff + 2;
	void *da, *sa;
	mlib_s32 slb, dlb, dw, dh, dchan, i;
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dlb, da);
	slb = mlib_ImageGetStride(src);
	sa = mlib_ImageGetData(src);

	if (dtype == MLIB_FLOAT || dtype == MLIB_DOUBLE)
		fp_flag ^= 1;
	if (fp_flag)
		return (MLIB_FAILURE);

	dw *= dchan;

	if (!mlib_ImageIsNotOneDvector(src) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		dw *= dh;
		slb *= dh;
		dlb *= dh;
		dh = 1;
	}

	if (dtype == MLIB_BYTE) {
		CALC_CONST2(mlib_u8,
		    MLIB_U8_MIN,
		    MLIB_U8_MAX)
		    mlib_s_ImageArith_U8(
		    da,
		    sa,
		    c_add,
		    c_sub,
		    dlb,
		    slb,
		    dw,
		    dh,
		    dchan);
	} else if (dtype == MLIB_SHORT) {
		CALC_CONST(mlib_s16,
		    mlib_s32)
		    mlib_s_ImageArith_S16(
		    da,
		    sa,
		    c_add,
		    c_sub,
		    dlb,
		    slb,
		    2 * dw,
		    dh,
		    dchan);
	} else if (dtype == MLIB_USHORT) {
		CALC_CONST2(mlib_u16,
		    MLIB_U16_MIN,
		    MLIB_U16_MAX)
		    mlib_s_ImageArith_U16(
		    da,
		    sa,
		    c_add,
		    c_sub,
		    dlb,
		    slb,
		    2 * dw,
		    dh,
		    dchan);
	} else if (dtype == MLIB_INT) {
		CALC_CONST(mlib_s32,
		    mlib_s32)
		    mlib_s_ImageArith_S32(
		    da,
		    sa,
		    c_add,
		    c_sub,
		    dlb,
		    slb,
		    4 * dw,
		    dh,
		    dchan);
	} else if (dtype == MLIB_FLOAT) {
		CALC_CONST(mlib_f32,
		    mlib_d64)
		    mlib_s_ImageArith_F32(
		    da,
		    sa,
		    c_add,
		    c_sub,
		    dlb,
		    slb,
		    4 * dw,
		    dh,
		    dchan);
	} else if (dtype == MLIB_DOUBLE) {
		CALC_CONST(mlib_d64,
		    mlib_d64)
		    mlib_ImageConstArith_D64(
		    da,
		    sa,
		    dw,
		    dh,
		    dchan,
		    slb / 8,
		    dlb / 8,
		    c_add);
	} else {
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
FUNCTION_NAME(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	return (mlib_ImageConstArith(dst, src, c, 0));
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECONSTARITH_H */
