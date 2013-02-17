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
#pragma ident	"@(#)mlib_s_VideoDCT16x16_S16_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDCT16x16_S16_S16 - Computes the forward DCT in the inter mode.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoDCT16x16_S16_S16(mlib_s16       *coeffs,
 *                                           const mlib_s16 *block)
 *
 * ARGUMENTS
 *    coeffs          Pointer to the output DCT coefficients
 *    block           Pointer to an 16x16 motion-compensated block which is the
 *                    difference between the reference block and current block
 *
 *    coeffs and  block both must be 8-byte aligned
 *
 *    Input block values should be in [-128, 127] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1, ... ,15; y = 0, 1, ... ,15
 *   F = F(u, v)  u = 0, 1, ... ,15; v = 0, 1, ... ,15
 *
 *             1             15  15             pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             8            x = 0 y = 0             32                  32
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT16x16_S16_S16 = __mlib_VideoDCT16x16_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT16x16_S16_S16) mlib_VideoDCT16x16_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT16x16_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */



#define	MM_MUL_SU_16(x, y)                  \
		_mm_add_epi16(_mm_mulhi_epi16(x, y),\
		_mm_srai_epi16(x, 1));

#define	SSE2_TRANSPOSE8x8(n)				\
	t4 = _mm_unpacklo_epi16(tmp4##n, tmp5##n);	\
	t5 = _mm_unpackhi_epi16(tmp4##n, tmp5##n);	\
	t6 = _mm_unpacklo_epi16(tmp6##n, tmp7##n);	\
	t7 = _mm_unpackhi_epi16(tmp6##n, tmp7##n);	\
	txmm4 = _mm_unpacklo_epi32(t4, t6);		\
	txmm6 = _mm_unpackhi_epi32(t4, t6);		\
	txmm5 = _mm_unpacklo_epi32(t5, t7);		\
	txmm7 = _mm_unpackhi_epi32(t5, t7);		\
	t0 = _mm_unpacklo_epi16(tmp0##n, tmp1##n);	\
	t1 = _mm_unpackhi_epi16(tmp0##n, tmp1##n);	\
	t2 = _mm_unpacklo_epi16(tmp2##n, tmp3##n);	\
	t3 = _mm_unpackhi_epi16(tmp2##n, tmp3##n);	\
	txmm0 = _mm_unpacklo_epi32(t0, t2);		\
	txmm2 = _mm_unpackhi_epi32(t0, t2);		\
	txmm1 = _mm_unpacklo_epi32(t1, t3);		\
	txmm3 = _mm_unpackhi_epi32(t1, t3);		\
	tmp7##n = _mm_unpackhi_epi64(txmm3, txmm7);	\
	tmp6##n = _mm_unpacklo_epi64(txmm3, txmm7);	\
	tmp5##n = _mm_unpackhi_epi64(txmm1, txmm5);	\
	tmp4##n = _mm_unpacklo_epi64(txmm1, txmm5);	\
	tmp3##n = _mm_unpackhi_epi64(txmm2, txmm6);	\
	tmp2##n = _mm_unpacklo_epi64(txmm2, txmm6);	\
	tmp1##n = _mm_unpackhi_epi64(txmm0, txmm4);	\
	tmp0##n = _mm_unpacklo_epi64(txmm0, txmm4);

#define	LOAD_DATA(j, m, n)					\
	tmp0##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 0]);	\
	tmp7##n   = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 15]);	\
	tmp1##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 1]);	\
	tmp6##n   = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 14]);	\
	tmp2##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 2]);	\
	tmp5##n   = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 13]);	\
	tmp3##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 3]);	\
	tmp4##n   = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 12]);	\
	tmp4##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 4]);	\
	tmp3##n   = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 11]);	\
	tmp5##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 5]);	\
	tmp2##n   = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 10]);	\
	tmp6##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 6]);	\
	tmp1##n    = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 9]);	\
	tmp7##m = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 7]);	\
	tmp0##n    = _mm_loadu_si128((__m128i *)&pSrc[j + 16 * 8]);	\
	tmp0##m = _mm_srai_epi16(tmp0##m, 1);			\
	tmp1##m = _mm_srai_epi16(tmp1##m, 1);			\
	tmp2##m = _mm_srai_epi16(tmp2##m, 1);			\
	tmp3##m = _mm_srai_epi16(tmp3##m, 1);			\
	tmp4##m = _mm_srai_epi16(tmp4##m, 1);			\
	tmp5##m = _mm_srai_epi16(tmp5##m, 1);			\
	tmp6##m = _mm_srai_epi16(tmp6##m, 1);			\
	tmp7##m = _mm_srai_epi16(tmp7##m, 1);			\
	tmp0##n = _mm_srai_epi16(tmp0##n, 1);			\
	tmp1##n = _mm_srai_epi16(tmp1##n, 1);			\
	tmp2##n = _mm_srai_epi16(tmp2##n, 1);			\
	tmp3##n = _mm_srai_epi16(tmp3##n, 1);			\
	tmp4##n = _mm_srai_epi16(tmp4##n, 1);			\
	tmp5##n = _mm_srai_epi16(tmp5##n, 1);			\
	tmp6##n = _mm_srai_epi16(tmp6##n, 1);			\
	tmp7##n = _mm_srai_epi16(tmp7##n, 1);

#define	ROW_DCT16x16(m, n)					\
/* first butter-fly */						\
	xmmc0 = _mm_add_epi16(tmp0##m, tmp7##n);			\
	t7 = _mm_sub_epi16(tmp0##m, tmp7##n);			\
	xmmc1 = _mm_add_epi16(tmp1##m, tmp6##n);			\
	t6 = _mm_sub_epi16(tmp1##m, tmp6##n);			\
	xmmc2 = _mm_add_epi16(tmp2##m, tmp5##n);			\
	t5 = _mm_sub_epi16(tmp2##m, tmp5##n);			\
	xmmc3 = _mm_add_epi16(tmp3##m, tmp4##n);			\
	t4 = _mm_sub_epi16(tmp3##m, tmp4##n);			\
	xmmc4 = _mm_add_epi16(tmp4##m, tmp3##n);			\
	t3 = _mm_sub_epi16(tmp4##m, tmp3##n);			\
	xmmc5 = _mm_add_epi16(tmp5##m, tmp2##n);			\
	t2 = _mm_sub_epi16(tmp5##m, tmp2##n);			\
	xmmc6 = _mm_add_epi16(tmp6##m, tmp1##n);			\
	t1 = _mm_sub_epi16(tmp6##m, tmp1##n);			\
	xmmc7 = _mm_add_epi16(tmp7##m, tmp0##n);			\
	t0 = _mm_sub_epi16(tmp7##m, tmp0##n);			\
/* second butter-fly */						\
	txmm0 = _mm_add_epi16(xmmc0, xmmc7);			\
	txmm1 = _mm_add_epi16(xmmc1, xmmc6);			\
	txmm2 = _mm_add_epi16(xmmc2, xmmc5);			\
	txmm3 = _mm_add_epi16(xmmc3, xmmc4);			\
	txmm4 = _mm_sub_epi16(xmmc3, xmmc4);			\
	txmm5 = _mm_sub_epi16(xmmc2, xmmc5);			\
	txmm6 = _mm_sub_epi16(xmmc1, xmmc6);			\
	txmm7 = _mm_sub_epi16(xmmc0, xmmc7);			\
	xmmc0 = _mm_sub_epi16(t5, t2);				\
	xmmc3 = _mm_add_epi16(t5, t2);				\
	t2  = MM_MUL_SU_16(xmmc0, COS_4);			\
	t5  = MM_MUL_SU_16(xmmc3, COS_4);			\
	xmmc1 = _mm_sub_epi16(t4, t3);				\
	xmmc2 = _mm_add_epi16(t4, t3);				\
	t3 = MM_MUL_SU_16(xmmc1, COS_4);			\
	t4 = MM_MUL_SU_16(xmmc2, COS_4);			\
/* third butter-fly */						\
	xmmc0 = _mm_add_epi16(txmm0, txmm3);			\
	xmmc1 = _mm_add_epi16(txmm1, txmm2);			\
	xmmc2 = _mm_sub_epi16(txmm1, txmm2);			\
	xmmc3 = _mm_sub_epi16(txmm0, txmm3);			\
	xmmc4 = txmm4;						\
	txmm0 = _mm_sub_epi16(txmm6, txmm5);			\
	xmmc5 = MM_MUL_SU_16(txmm0, COS_4);			\
	txmm1 = _mm_add_epi16(txmm6, txmm5);			\
	xmmc6 = MM_MUL_SU_16(txmm1, COS_4);			\
	xmmc7 = txmm7;						\
	txmm0 = _mm_add_epi16(t0, t3);				\
	txmm1 = _mm_add_epi16(t1, t2);				\
	txmm2 = _mm_sub_epi16(t1, t2);				\
	txmm3 = _mm_sub_epi16(t0, t3);				\
	txmm4 = _mm_sub_epi16(t7, t4);				\
	txmm5 = _mm_sub_epi16(t6, t5);				\
	txmm6 = _mm_add_epi16(t6, t5);				\
	txmm7 = _mm_add_epi16(t7, t4);				\
/* fourth butter-fly */						\
	t0 = _mm_add_epi16(xmmc0, xmmc1);			\
	tmp0##m = MM_MUL_SU_16(t0, COS_4);			\
	t1 = _mm_sub_epi16(xmmc0, xmmc1);			\
	tmp0##n = MM_MUL_SU_16(t1, COS_4);			\
	t4 = MM_MUL_SU_16(xmmc2, SIN_8);			\
	t5 = MM_MUL_SU_16(xmmc3, COS_8);			\
	tmp4##m = _mm_add_epi16(t4, t5);				\
	t4 = MM_MUL_SU_16(xmmc3, SIN_8);			\
	t5 = MM_MUL_SU_16(xmmc2, COS_8);			\
	tmp4##n = _mm_sub_epi16(t4, t5);				\
	t4 = _mm_add_epi16(xmmc4, xmmc5);			\
	t5 = _mm_sub_epi16(xmmc4, xmmc5);			\
	t6 = _mm_sub_epi16(xmmc7, xmmc6);			\
	t7 = _mm_add_epi16(xmmc7, xmmc6);			\
	xmmc0 = MM_MUL_SU_16(txmm6, SIN_8);			\
	xmmc1 = MM_MUL_SU_16(txmm1, COS_8);			\
	xmmc2 = MM_MUL_SU_16(txmm2, SIN_8);			\
	xmmc3 = MM_MUL_SU_16(txmm5, COS_8);			\
	xmmc4 = MM_MUL_SU_16(txmm5, SIN_8);			\
	xmmc5 = MM_MUL_SU_16(txmm2, COS_8);			\
	xmmc6 = MM_MUL_SU_16(txmm1, SIN_8);			\
	xmmc7 = MM_MUL_SU_16(txmm6, COS_8);			\
	txmm1 = _mm_sub_epi16(xmmc0, xmmc1);			\
	txmm2 = _mm_add_epi16(xmmc2, xmmc3);			\
	txmm5 = _mm_sub_epi16(xmmc4, xmmc5);			\
	txmm6 = _mm_add_epi16(xmmc6, xmmc7);			\
/* fifth butter-fly */						\
	xmmc0 = MM_MUL_SU_16(t4, SIN_16);			\
	xmmc1 = MM_MUL_SU_16(t7, COS_16);			\
	xmmc2 = MM_MUL_SU_16(t5, COS_3_16);			\
	xmmc3 = MM_MUL_SU_16(t6, SIN_3_16);			\
	xmmc4 = MM_MUL_SU_16(t6, COS_3_16);			\
	xmmc5 = MM_MUL_SU_16(t5, SIN_3_16);			\
	xmmc6 = MM_MUL_SU_16(t7, SIN_16);			\
	xmmc7 = MM_MUL_SU_16(t4, COS_16);			\
	tmp2##m = _mm_add_epi16(xmmc0, xmmc1);			\
	tmp2##n = _mm_add_epi16(xmmc2, xmmc3);			\
	tmp6##m = _mm_sub_epi16(xmmc4, xmmc5);			\
	tmp6##n = _mm_sub_epi16(xmmc6, xmmc7);			\
	t0 = _mm_add_epi16(txmm0, txmm1);			\
	t1 = _mm_sub_epi16(txmm0, txmm1);			\
	t2 = _mm_add_epi16(txmm3, txmm2);			\
	t3 = _mm_sub_epi16(txmm3, txmm2);			\
	t4 = _mm_add_epi16(txmm4, txmm5);			\
	t5 = _mm_sub_epi16(txmm4, txmm5);			\
	t6 = _mm_sub_epi16(txmm7, txmm6);			\
	t7 = _mm_add_epi16(txmm7, txmm6);			\
/* sixth butter-fly */						\
	xmmc0 = MM_MUL_SU_16(t0, SIN_32);			\
	xmmc1 = MM_MUL_SU_16(t7, COS_32);			\
	tmp1##m = _mm_add_epi16(xmmc0, xmmc1);			\
	xmmc2 = MM_MUL_SU_16(t1, COS_7_32);			\
	xmmc3 = MM_MUL_SU_16(t6, SIN_7_32);			\
	tmp1##n = _mm_add_epi16(xmmc2, xmmc3);			\
	xmmc4 = MM_MUL_SU_16(t2, SIN_5_32);			\
	xmmc5 = MM_MUL_SU_16(t5, COS_5_32);			\
	tmp5##m = _mm_add_epi16(xmmc4, xmmc5);			\
	xmmc6 = MM_MUL_SU_16(t3, COS_3_32);			\
	xmmc7 = MM_MUL_SU_16(t4, SIN_3_32);			\
	tmp5##n = _mm_add_epi16(xmmc6, xmmc7);			\
	xmmc0 = MM_MUL_SU_16(t4, COS_3_32);			\
	xmmc1 = MM_MUL_SU_16(t3, SIN_3_32);			\
	tmp3##m = _mm_sub_epi16(xmmc0, xmmc1);			\
	xmmc2 = MM_MUL_SU_16(t5, SIN_5_32);			\
	xmmc3 = MM_MUL_SU_16(t2, COS_5_32);			\
	tmp3##n = _mm_sub_epi16(xmmc2, xmmc3);			\
	xmmc4 = MM_MUL_SU_16(t6, COS_7_32);			\
	xmmc5 = MM_MUL_SU_16(t1, SIN_7_32);			\
	tmp7##m = _mm_sub_epi16(xmmc4, xmmc5);			\
	xmmc6 = MM_MUL_SU_16(t7, SIN_32);			\
	xmmc7 = MM_MUL_SU_16(t0, COS_32);			\
	tmp7##n = _mm_sub_epi16(xmmc6, xmmc7);

#define	STORE_ALL(m, n)						\
	tmp0##m = _mm_srai_epi16(tmp0##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp0##m);			\
	tmp0##n = _mm_srai_epi16(tmp0##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp0##n);			\
	tmp1##m = _mm_srai_epi16(tmp1##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp1##m);			\
	tmp1##n = _mm_srai_epi16(tmp1##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp1##n);			\
	tmp2##m = _mm_srai_epi16(tmp2##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp2##m);			\
	tmp2##n = _mm_srai_epi16(tmp2##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp2##n);			\
	tmp3##m = _mm_srai_epi16(tmp3##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp3##m);			\
	tmp3##n = _mm_srai_epi16(tmp3##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp3##n);			\
	tmp4##m = _mm_srai_epi16(tmp4##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp4##m);			\
	tmp4##n = _mm_srai_epi16(tmp4##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp4##n);			\
	tmp5##m = _mm_srai_epi16(tmp5##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp5##m);			\
	tmp5##n = _mm_srai_epi16(tmp5##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp5##n);			\
	tmp6##m = _mm_srai_epi16(tmp6##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp6##m);			\
	tmp6##n = _mm_srai_epi16(tmp6##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp6##n);			\
	tmp7##m = _mm_srai_epi16(tmp7##m, 2);			\
	_mm_storeu_si128(dataptr++, tmp7##m);			\
	tmp7##n = _mm_srai_epi16(tmp7##n, 2);			\
	_mm_storeu_si128(dataptr++, tmp7##n);

/* *********************************************************** */

mlib_status
__mlib_VideoDCT16x16_S16_S16(
	mlib_s16 *coeffs,
	const mlib_s16 *block)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	__m128i tmp00, tmp10, tmp20, tmp30, tmp40, tmp50, tmp60, tmp70;
	__m128i tmp01, tmp11, tmp21, tmp31, tmp41, tmp51, tmp61, tmp71;
	__m128i tmp02, tmp12, tmp22, tmp32, tmp42, tmp52, tmp62, tmp72;
	__m128i tmp03, tmp13, tmp23, tmp33, tmp43, tmp53, tmp63, tmp73;
	__m128i xmmc0, xmmc1, xmmc2, xmmc3, xmmc4, xmmc5, xmmc6, xmmc7;
	__m128i t0, t1, t2, t3, t4, t5, t6, t7;
	mlib_s16 *pSrc = (void *)block;

/* be care: all const multiplied by 1<<16 and subed by 32768 */
	__m128i COS_4 = _mm_set1_epi16(0x3504);
	__m128i SIN_8 = _mm_set1_epi16(-0x1E09);
	__m128i COS_8 = _mm_set1_epi16(0x6C82);
	__m128i SIN_16 = _mm_set1_epi16(-0x4E0F);
	__m128i COS_16 = _mm_set1_epi16(0x7B13);
	__m128i COS_3_16 = _mm_set1_epi16(0x54DA);
	__m128i SIN_3_16 = _mm_set1_epi16(0xE39);

	__m128i SIN_32 = _mm_set1_epi16(-0x66E8);
	__m128i COS_32 = _mm_set1_epi16(0x7EC3);
	__m128i COS_3_32 = _mm_set1_epi16(0x74F9);
	__m128i SIN_3_32 = _mm_set1_epi16(-0x35B0);
	__m128i COS_5_32 = _mm_set1_epi16(0x61C5);
	__m128i SIN_5_32 = _mm_set1_epi16(-0x753);
	__m128i COS_7_32 = _mm_set1_epi16(0x45E3);
	__m128i SIN_7_32 = _mm_set1_epi16(0x2267);

	__m128i *dataptr = (__m128i *)coeffs;

	LOAD_DATA(0, 0, 2);
	ROW_DCT16x16(0, 2);

	LOAD_DATA(8, 1, 3);
	ROW_DCT16x16(1, 3);

	SSE2_TRANSPOSE8x8(0);
	SSE2_TRANSPOSE8x8(1);
	SSE2_TRANSPOSE8x8(2);
	SSE2_TRANSPOSE8x8(3);

	ROW_DCT16x16(0, 1);
	ROW_DCT16x16(2, 3);

	SSE2_TRANSPOSE8x8(0);
	SSE2_TRANSPOSE8x8(1);
	SSE2_TRANSPOSE8x8(2);
	SSE2_TRANSPOSE8x8(3);

	STORE_ALL(0, 1);
	STORE_ALL(2, 3);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
