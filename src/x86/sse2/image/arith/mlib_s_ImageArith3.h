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

#ifndef _MLIB_S_IMAGEARITH3_H
#define	_MLIB_S_IMAGEARITH3_H

#pragma ident	"@(#)mlib_s_ImageArith3.h	9.7	07/11/05 SMI"

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

typedef void (*expand_func_type)(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 size);

/* *********************************************************** */

#define	EXPAND_FUNCTION(TT)                                            \
	static void                                                    \
	mlib_Expand_2_##TT(mlib_u8 *src, mlib_u8 *dst,                 \
	    mlib_s32 size)                                             \
	{                                                              \
	    mlib_s32 i;                                                \
	    __m128i ss;                                                \
	                                                               \
	    for (i = 0; i < (size + 31) / 32; i++) {                   \
		LOADU__m128i(ss, src);                                 \
		((__m128i *) dst)[0] = _mm_unpacklo_epi##TT(ss, ss);   \
		((__m128i *) dst)[1] = _mm_unpackhi_epi##TT(ss, ss);   \
		src += 16;                                             \
		dst += 32;                                             \
	    }                                                          \
	}                                                              \
	static void                                                    \
	mlib_Expand_3_##TT(mlib_u8 *src, mlib_u8 *dst,                 \
	    mlib_s32 size)                                             \
	{                                                              \
	    mlib_s32 i;                                                \
	    mlib_u32 ss;                                               \
	                                                               \
	    for (i = 0; i < size / (3 * sizeof (mlib_u##TT)); i++) {   \
		mlib_u##TT ss = *(mlib_u##TT *) src;                   \
	                                                               \
		((mlib_u##TT *) dst)[0] = ss;                          \
		((mlib_u##TT *) dst)[1] = ss;                          \
		((mlib_u##TT *) dst)[2] = ss;                          \
		src += sizeof (mlib_u##TT);                            \
		dst += 3 * sizeof (mlib_u##TT);                        \
	    }                                                          \
	}                                                              \
	static void                                                    \
	mlib_Expand_4_##TT(mlib_u8 *src, mlib_u8 *dst,                 \
	    mlib_s32 size)                                             \
	{                                                              \
	    mlib_s32 i;                                                \
	    __m128i ss, s0, s1;                                        \
	                                                               \
	    for (i = 0; i < (size + 63) / 64; i++) {                   \
		LOADU__m128i(ss, src);                                 \
		s0 = _mm_unpacklo_epi##TT(ss, ss);                     \
		s1 = _mm_unpackhi_epi##TT(ss, ss);                     \
		((__m128i *) dst)[0] = _mm_unpacklo_epi##TT(s0, s0);   \
		((__m128i *) dst)[1] = _mm_unpackhi_epi##TT(s0, s0);   \
		((__m128i *) dst)[2] = _mm_unpacklo_epi##TT(s1, s1);   \
		((__m128i *) dst)[3] = _mm_unpackhi_epi##TT(s1, s1);   \
		src += 16;                                             \
		dst += 64;                                             \
	    }                                                          \
	}

EXPAND_FUNCTION(8)
    EXPAND_FUNCTION(16)
    EXPAND_FUNCTION(32)
    EXPAND_FUNCTION(64)

/* *********************************************************** */
#define	ARITH_FUNC(SSE_TYPE, TYPE)                                        \
	static void                                                       \
	mlib_s_ImageArith_##TYPE(mlib_u8 *dst, mlib_u8 *src1,             \
	    mlib_u8 *src2, mlib_u8 *src3, mlib_u8 *abuff,                 \
	    expand_func_type f_src_expand, mlib_s32 dlb, mlib_s32 slb1,   \
	    mlib_s32 slb2, mlib_s32 slb3, mlib_s32 dw, mlib_s32 dh)       \
	{                                                                 \
	    SSE_TYPE sd1, sd2, sd3, dd;                                   \
	    EXTRA_VARS_##TYPE mlib_u8 *dp, *dl, *dend;                    \
	    mlib_u8 *sp1, *sl1;                                           \
	    mlib_u8 *sp2, *sl2;                                           \
	    mlib_u8 *sp3, *sl3;                                           \
	    mlib_s32 nn, emask;                                           \
	                                                                  \
	    dp = dl = dst;                                                \
	    sp1 = sl1 = src1;                                             \
	    sp2 = sl2 = src2;                                             \
	    sp3 = sl3 = src3;                                             \
	    for (; dh > 0; dh--) {                                        \
		if (abuff != NULL) {                                      \
		    f_src_expand(sp3, abuff, dw);                         \
		    sp3 = abuff;                                          \
		}                                                         \
		dp = (void *)((mlib_addr)dl & ~15);                       \
		nn = dp - dl;                                             \
		dend = dl + dw - 1;                                       \
		emask = sse_edge8(dl, dend);                              \
		sp1 += nn;                                                \
		sp2 += nn;                                                \
		sp3 += nn;                                                \
		if (emask != 0xffff) {                                    \
		    LOADU##SSE_TYPE(sd1, sp1);                            \
		    LOADU##SSE_TYPE(sd2, sp2);                            \
		    LOADU##SSE_TYPE(sd3, sp3);                            \
		    ARITH_OPER_##TYPE(dd, sd1, sd2, sd3);                 \
		    sse2_pst_8(dd, dp, emask);                             \
		    nn += 16;                                             \
		    dp += 16;                                             \
		    sp1 += 16;                                            \
		    sp2 += 16;                                            \
		    sp3 += 16;                                            \
		}                                                         \
	                                                                  \
/*                                                                        \
 * 16-bytes loop                                                          \
 */                                                                       \
		if (!(((mlib_s32)sp1 | (mlib_s32)sp2 | (mlib_s32)sp3) &   \
		    15)) {                                                \
		    for (; nn <= dw - 16; nn += 16) {                     \
			sd1 = *(SSE_TYPE *) sp1;                          \
			sd2 = *(SSE_TYPE *) sp2;                          \
			sd3 = *(SSE_TYPE *) sp3;                          \
			ARITH_OPER_##TYPE(dd, sd1, sd2, sd3);             \
			*(SSE_TYPE *) dp = dd;                            \
			dp += 16;                                         \
			sp1 += 16;                                        \
			sp2 += 16;                                        \
			sp3 += 16;                                        \
		    }                                                     \
		} else {                                                  \
		    for (; nn <= dw - 16; nn += 16) {                     \
			LOADU##SSE_TYPE(sd1, sp1);                        \
			LOADU##SSE_TYPE(sd2, sp2);                        \
			LOADU##SSE_TYPE(sd3, sp3);                        \
			ARITH_OPER_##TYPE(dd, sd1, sd2, sd3);             \
			*(SSE_TYPE *) dp = dd;                            \
			dp += 16;                                         \
			sp1 += 16;                                        \
			sp2 += 16;                                        \
			sp3 += 16;                                        \
		    }                                                     \
		}                                                         \
		if (nn < dw) {                                            \
		    emask = sse_edge8(dp, dend);                          \
		    LOADU##SSE_TYPE(sd1, sp1);                            \
		    LOADU##SSE_TYPE(sd2, sp2);                            \
		    LOADU##SSE_TYPE(sd3, sp3);                            \
		    ARITH_OPER_##TYPE(dd, sd1, sd2, sd3);                 \
		    sse2_pst_8(dd, dp, emask);                             \
		}                                                         \
		dp = dl += dlb;                                           \
		sp1 = sl1 += slb1;                                        \
		sp2 = sl2 += slb2;                                        \
		sp3 = sl3 += slb3;                                        \
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
    static mlib_status mlib_arith_main(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_image *src3,
    mlib_s32 fp_flag)
{
	void *da, *sa1, *sa2, *sa3;
	mlib_s32 slb1, slb2, slb3, dlb, dw, dh, dchan, sw, sh, schan, tshift;
	mlib_type dtype, stype;
	mlib_u8 *abuff = NULL;
	expand_func_type ff = NULL;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_CHECK(src3);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dlb, da);
	MLIB_IMAGE_GET_ALL_PARAMS(src3, stype, schan, sw, sh, slb3, sa3);

	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa1 = mlib_ImageGetData(src1);
	sa2 = mlib_ImageGetData(src2);

	if (dtype == MLIB_FLOAT || dtype == MLIB_DOUBLE)
		fp_flag ^= 1;
	if (fp_flag)
		return (MLIB_FAILURE);

	dw *= dchan;

	switch (dtype) {
	case MLIB_BYTE:
		tshift = 0;
		break;
	case MLIB_SHORT:
		tshift = 1;
		break;
	case MLIB_USHORT:
		tshift = 1;
		break;
	case MLIB_INT:
		tshift = 2;
		break;
	case MLIB_FLOAT:
		tshift = 2;
		break;
	case MLIB_DOUBLE:
		tshift = 3;
		break;
	}

	dw <<= tshift;

	if (schan != dchan) {
		abuff = __mlib_malloc(dw + 5 * 16);
		abuff += 16;
		switch (4 * tshift + dchan) {
		case 2:
			ff = mlib_Expand_2_8;
			break;
		case 3:
			ff = mlib_Expand_3_8;
			break;
		case 4:
			ff = mlib_Expand_4_8;
			break;
		case 6:
			ff = mlib_Expand_2_16;
			break;
		case 7:
			ff = mlib_Expand_3_16;
			break;
		case 8:
			ff = mlib_Expand_4_16;
			break;
		case 10:
			ff = mlib_Expand_2_32;
			break;
		case 11:
			ff = mlib_Expand_3_32;
			break;
		case 12:
			ff = mlib_Expand_4_32;
			break;
		case 14:
			ff = mlib_Expand_2_64;
			break;
		case 15:
			ff = mlib_Expand_3_64;
			break;
		case 16:
			ff = mlib_Expand_4_64;
			break;
		}
	}

	if (schan == dchan &&
	    !mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(src3) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		dw *= dh;
		slb1 *= dh;
		slb2 *= dh;
		slb3 *= dh;
		dlb *= dh;
		dh = 1;
	}

	if (dtype == MLIB_BYTE) {
		mlib_s_ImageArith_U8(da, sa1, sa2, sa3, abuff, ff, dlb, slb1,
		    slb2, slb3, dw, dh);
	} else if (dtype == MLIB_SHORT) {
		mlib_s_ImageArith_S16(da, sa1, sa2, sa3, abuff, ff, dlb, slb1,
		    slb2, slb3, dw, dh);
	} else if (dtype == MLIB_USHORT) {
		mlib_s_ImageArith_U16(da, sa1, sa2, sa3, abuff, ff, dlb, slb1,
		    slb2, slb3, dw, dh);
	} else if (dtype == MLIB_INT) {
		mlib_s_ImageArith_S32(da, sa1, sa2, sa3, abuff, ff, dlb, slb1,
		    slb2, slb3, dw, dh);
	} else if (dtype == MLIB_FLOAT) {
		mlib_s_ImageArith_F32(da, sa1, sa2, sa3, abuff, ff, dlb, slb1,
		    slb2, slb3, dw, dh);
	} else if (dtype == MLIB_DOUBLE) {
		mlib_s_ImageArith_D64(da, sa1, sa2, sa3, abuff, ff, dlb, slb1,
		    slb2, slb3, dw, dh);
	} else {
		return (MLIB_FAILURE);
	}

	if (schan != dchan) {
		abuff -= 16;
		__mlib_free(abuff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
FUNCTION_NAME(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_image *src3)
{
	return (mlib_arith_main(dst, src1, src2, src3, 0));
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEARITH3_H */
