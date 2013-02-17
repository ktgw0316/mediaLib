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

#ifndef _MLIB_S_VIDEO_H
#define	_MLIB_S_VIDEO_H

#pragma ident	"@(#)mlib_s_video.h	9.9	07/11/05 SMI"

/*
 * Macro definitions for SSE version video dct functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	C14	(1<<14)
#define	C0	2048
#define	C4	11585
#define	C1	16069
#define	C2	15137
#define	C3	13623
#define	C5	9102
#define	C6	6270
#define	C7	3196

#if defined(__ICL) || defined(__INTEL_COMPILER)
	_declspec(align(16))
#elif __SUNPRO_C
#pragma	align 16(IDCT_TABLE_16)
#endif /* __ICL || INTEL_COMPILER */
static const short IDCT_TABLE_16[] = {
	C2,  C3,  C4,  C3,  C2,  C3,  C4,  C3,
	C4,  C1,  C6, -C7, -C4,  C7, -C6, -C1,
	C4,  C5, -C2, -C5,  C4,  C5, -C2, -C5,
	C6,  C7, -C4, -C1, -C6, -C1,  C4,  C7,
	-C2, C5,  C4, -C5, -C2,  C5,  C4, -C5,
	C4, -C7, -C6,  C1, -C4,  C1,  C6, -C7,
	C4, -C3,  C2, -C3,  C4, -C3,  C2, -C3,
	-C6, C1, -C4, -C7,  C6,  C7,  C4, -C1,
	C0,  C0,  C0,  C0,  C0,  C0,  C0,  C0,
	C14,  0, C14,   0, C14,   0, C14,   0,
};

#if defined(__ICL) || defined(__INTEL_COMPILER)
	_declspec(align(16))
#elif __SUNPRO_C
#pragma	align 16(IDCT_TABLE_16_Q1)
#endif /* __ICL || INTEL_COMPILER */
static const short IDCT_TABLE_16_Q1[] = {
	C4,  C1,  C6, -C7,  C4,  C5, -C2, -C5,
	C2,  C3,  C4,  C3, -C6, -C1,  C4,  C7,
	C4, -C7, -C6,  C1,  C4, -C3,  C2, -C3,
	-C2,  C5,  C4, -C5,  C6,  C7,  C4, -C1,
	C0,  C0,  C0,  C0,  C0,  C0,  C0,  C0,
	C14,  0, C14,   0, C14,   0, C14,   0,
};

#if defined(__ICL) || defined(__INTEL_COMPILER)
	_declspec(align(16))
#elif __SUNPRO_C
#pragma align 16(TABLE_16)
#endif /* __ICL || INTEL_COMPILER */
static const short TABLE_16[] = {
	11585,  3196,  -6270, -16069, -11585, -16069,   6270,   3196,
	11585,  9102, -15137,  -9102,  11585,   9102, -15137,  -9102,
	11585, 13623,  15137,  13623,  11585,  13623,  15137,  13623,
	11585, 16069,   6270,  -3196, -11585,   3196,  -6270, -16069,
};

#if defined(__ICL) || defined(__INTEL_COMPILER)
	_declspec(align(16))
#elif __SUNPRO_C
#pragma align 16(COL_TABLE)
#endif /* __ICL || INTEL_COMPILER */
static const short COL_TABLE[] = {
	11585, 11585, 11585, 11585, 11585, 11585, 11585, 11585,
	3196,  3196,  3196,  3196,  3196,  3196,  3196,  3196,
	6270,  6270,  6270,  6270,  6270,  6270,  6270,  6270,
	13623, 13623, 13623, 13623, 13623, 13623, 13623, 13623,
	9102,  9102,  9102,  9102,  9102,  9102,  9102,  9102,
	16069, 16069, 16069, 16069, 16069, 16069, 16069, 16069,
	15137, 15137, 15137, 15137, 15137, 15137, 15137, 15137,
	3, 3, 3, 3, 3, 3, 3, 3,
};

#define	SSE2_TRANSPOSE4x4   				\
	t0 = _mm_unpacklo_epi16(t0, t1);		\
	t2 = _mm_unpacklo_epi16(t2, t3);		\
	tmp01 = _mm_unpacklo_epi32(t0, t2);		\
	tmp21 = _mm_unpackhi_epi32(t0, t2);		\
	tmp00 = _mm_unpacklo_epi64(tmp01, Czero);	\
	tmp10 = _mm_unpackhi_epi64(tmp01, Czero);	\
	tmp20 = _mm_unpacklo_epi64(tmp21, Czero);	\
	tmp30 = _mm_unpackhi_epi64(tmp21, Czero);	\
	t4 = _mm_setzero_si128();			\
	t5 = _mm_setzero_si128();			\
	t6 = _mm_setzero_si128();			\
	t7 = _mm_setzero_si128();

#define	SSE2_TRANSPOSE					\
	xmmc4 = t4;					\
	t4 = _mm_unpacklo_epi16(xmmc4, t5);		\
	t5 = _mm_unpackhi_epi16(xmmc4, t5);		\
	xmmc3 = t6;					\
	t6 = _mm_unpacklo_epi16(xmmc3, t7);		\
	t7 = _mm_unpackhi_epi16(xmmc3, t7);		\
	tmp41 = _mm_unpacklo_epi32(t4, t6);		\
	tmp61 = _mm_unpackhi_epi32(t4, t6);		\
	tmp51 = _mm_unpacklo_epi32(t5, t7);		\
	tmp71 = _mm_unpackhi_epi32(t5, t7);		\
	txmm8 = t0;					\
	t0 = _mm_unpacklo_epi16(txmm8, t1);		\
	t1 = _mm_unpackhi_epi16(txmm8, t1);		\
	xmmc1 = t2;	 				\
	t2 = _mm_unpacklo_epi16(xmmc1, t3);		\
	t3 = _mm_unpackhi_epi16(xmmc1, t3);		\
	tmp01 = _mm_unpacklo_epi32(t0, t2);		\
	tmp21 = _mm_unpackhi_epi32(t0, t2);		\
	tmp11 = _mm_unpacklo_epi32(t1, t3);		\
	tmp31 = _mm_unpackhi_epi32(t1, t3);		\
	tmp70 = _mm_unpackhi_epi64(tmp31, tmp71);	\
	tmp60 = _mm_unpacklo_epi64(tmp31, tmp71);	\
	tmp50 = _mm_unpackhi_epi64(tmp11, tmp51);	\
	tmp40 = _mm_unpacklo_epi64(tmp11, tmp51);	\
	tmp30 = _mm_unpackhi_epi64(tmp21, tmp61);	\
	tmp20 = _mm_unpacklo_epi64(tmp21, tmp61);	\
	tmp10 = _mm_unpackhi_epi64(tmp01, tmp41);	\
	tmp00 = _mm_unpacklo_epi64(tmp01, tmp41);

#define	IDCT_8x8_S16_S16_QM				\
	ROW_IDCT_Q1(00, 0);				\
	ROW_IDCT_Q1(10, 1);				\
	ROW_IDCT_Q1(20, 2);				\
	ROW_IDCT_Q1(30, 3);				\
	Y = _mm_set1_epi16(y);				\
	Cxmm = _mm_set_epi16(-C7, C5, -C3, C1, -C1, C3, -C5, C7);\
	t7 = _mm_mulhi_epi16(Y, Cxmm);

#define	IDCT_8x8_S16_S16_Q1				\
	ROW_IDCT_Q1(00, 0);				\
	ROW_IDCT_Q1(10, 1);				\
	ROW_IDCT_Q1(20, 2);				\
	ROW_IDCT_Q1(30, 3);

#define	IDCT_8x8_S16_S16				\
	ROW_IDCT(00, 0);				\
	ROW_IDCT(10, 1);				\
	ROW_IDCT(20, 2);				\
	ROW_IDCT(30, 3);				\
	ROW_IDCT(40, 4);				\
	ROW_IDCT(50, 5);				\
	ROW_IDCT(60, 6);				\
	ROW_IDCT(70, 7);

#define	LOADDATA_Q1					\
	t0 = _mm_loadu_si128(pSrc++);			\
	t1 = _mm_loadu_si128(pSrc++);			\
	t2 = _mm_loadu_si128(pSrc++);			\
	t3 = _mm_loadu_si128(pSrc++);

#define	LOAD_DEQUANT					\
	t0 = _mm_loadu_si128(pSrc++);			\
	txmm0 = _mm_loadu_si128(pQuant++);		\
	t0 = _mm_mullo_epi16(t0, txmm0);		\
	t1 = _mm_loadu_si128(pSrc++);			\
	txmm1 = _mm_loadu_si128(pQuant++);		\
	t1 = _mm_mullo_epi16(t1, txmm1);		\
	t2 = _mm_loadu_si128(pSrc++);			\
	txmm2 = _mm_loadu_si128(pQuant++);		\
	t2 = _mm_mullo_epi16(t2, txmm2);		\
	t3 = _mm_loadu_si128(pSrc++);			\
	txmm3 = _mm_loadu_si128(pQuant++);		\
	t3 = _mm_mullo_epi16(t3, txmm3);		\
	t4 = _mm_loadu_si128(pSrc++);			\
	txmm4 = _mm_loadu_si128(pQuant++);		\
	t4 = _mm_mullo_epi16(t4, txmm4);		\
	t5 = _mm_loadu_si128(pSrc++);			\
	txmm5 = _mm_loadu_si128(pQuant++);		\
	t5 = _mm_mullo_epi16(t5, txmm5);		\
	t6 = _mm_loadu_si128(pSrc++);			\
	txmm6 = _mm_loadu_si128(pQuant++);		\
	t6 = _mm_mullo_epi16(t6, txmm6);		\
	t7 = _mm_loadu_si128(pSrc++);			\
	txmm7 = _mm_loadu_si128(pQuant++);		\
	t7 = _mm_mullo_epi16(t7, txmm7);

#define	LOADDATA					\
	t0 = _mm_loadu_si128(pSrc++);			\
	t1 = _mm_loadu_si128(pSrc++);			\
	t2 = _mm_loadu_si128(pSrc++);			\
	t3 = _mm_loadu_si128(pSrc++);			\
	t4 = _mm_loadu_si128(pSrc++);			\
	t5 = _mm_loadu_si128(pSrc++);			\
	t6 = _mm_loadu_si128(pSrc++);			\
	t7 = _mm_loadu_si128(pSrc++);			\
	/* the following code used for COL_IDCT */	\
	/* t0 = _mm_add_epi16(t0,xmmc1);	*/	\
	/* t1 = _mm_add_epi16(t1,xmmc1);	*/	\
	/* t2 = _mm_add_epi16(t2,xmmc1);	*/	\
	/* t3 = _mm_add_epi16(t3,xmmc1);	*/	\
	/* t4 = _mm_add_epi16(t4,xmmc1);	*/	\
	/* t5 = _mm_add_epi16(t5,xmmc1);	*/	\
	/* t6 = _mm_add_epi16(t6,xmmc1);	*/	\
	/* t7 = _mm_add_epi16(t7,xmmc1);	*/	\
	/* txmm0 = _mm_slli_epi16(txmm0, 1);	*/	\
	/* txmm1 = _mm_slli_epi16(txmm1, 1);	*/	\
	/* txmm2 = _mm_slli_epi16(txmm2, 1);	*/	\
	/* txmm3 = _mm_slli_epi16(txmm3, 1);	*/	\
	/* txmm4 = _mm_slli_epi16(txmm4, 1);	*/	\
	/* txmm5 = _mm_slli_epi16(txmm5, 1);	*/	\
	/* txmm6 = _mm_slli_epi16(txmm6, 1);	*/	\
	/* txmm7 = _mm_slli_epi16(txmm7, 1);	*/

#define	COL_IDCT					\
	LOADCONT();					\
	LOADDATA;					\
	tmp40 = _mm_add_epi16(t0, t4);			\
	tmp41 = _mm_sub_epi16(t0, t4);	/* y0-y4 */	\
	t0 = _mm_mulhi_epi16(tmp40, txmm0);		\
	t4 = _mm_mulhi_epi16(tmp41, txmm0);		\
	/* TW * (y0 +/- y4) */				\
	tmp20 = _mm_mulhi_epi16(t2, txmm6);		\
	tmp21 = _mm_mulhi_epi16(t6, txmm2);		\
	tmp60 = _mm_mulhi_epi16(t2, txmm2);		\
	tmp61 = _mm_mulhi_epi16(t6, txmm6);		\
	t2 = _mm_add_epi16(tmp20, tmp21);		\
	/*  C6*y6 + C2* y2  */				\
	t6 = _mm_sub_epi16(tmp60, tmp61);		\
	/*  C6*y2 - C2* y6  */				\
	tmp10 = _mm_mulhi_epi16(t1, txmm5);		\
	/* C1*y1 */					\
	tmp11 = _mm_mulhi_epi16(t1, txmm3);		\
	/* C3*y1 */					\
	tmp00 = _mm_mulhi_epi16(t1, txmm1);		\
	/* C7*y1 */					\
	t1 = _mm_mulhi_epi16(t1, txmm4);		\
	/* C5*y1 */					\
	tmp30 = _mm_mulhi_epi16(t3, txmm5);		\
	/* C1*y3 */					\
	tmp31 = _mm_mulhi_epi16(t3, txmm3);		\
	/* C3*y3 */					\
	tmp20 = _mm_mulhi_epi16(t3, txmm4);		\
	/* C5*y3 */					\
	t3 = _mm_mulhi_epi16(t3, txmm1);		\
	/* C7*y3 */					\
	tmp50 = _mm_mulhi_epi16(t5, txmm5);		\
	/* C1*y5 */					\
	tmp51 = _mm_mulhi_epi16(t5, txmm4);		\
	/* C5*y5 */					\
	tmp40 = _mm_mulhi_epi16(t5, txmm3);		\
	/* C3*y5 */					\
	t5 = _mm_mulhi_epi16(t5, txmm1);		\
	/* C7*y5 */					\
	tmp70 = _mm_mulhi_epi16(t7, txmm3);		\
	/* C3*y7 */					\
	tmp71 = _mm_mulhi_epi16(t7, txmm4);		\
	/* C5*y7 */					\
	tmp60 = _mm_mulhi_epi16(t7, txmm5);		\
	/* C1*y7 */					\
	t7 = _mm_mulhi_epi16(t7, txmm1);		\
	/* C7*y7 */					\
	txmm0 = _mm_sub_epi16(t4, t6);			\
	txmm3 = _mm_add_epi16(t4, t6);			\
	txmm7 = _mm_add_epi16(t0, t2);			\
	txmm8 = _mm_sub_epi16(t0, t2);			\
	t7 = _mm_add_epi16(t7, tmp51);			\
	tmp31 = _mm_add_epi16(tmp31, tmp10);		\
	txmm1 = _mm_add_epi16(tmp31, t7);		\
	tmp71 = _mm_add_epi16(tmp71, tmp50);		\
	tmp11 = _mm_sub_epi16(tmp11, t3);		\
	txmm2 = _mm_sub_epi16(tmp11, tmp71);		\
	tmp70 = _mm_add_epi16(tmp70, t5);		\
	t1 = _mm_sub_epi16(t1, tmp30);			\
	txmm4 = _mm_add_epi16(tmp70, t1);		\
	tmp40 = _mm_sub_epi16(tmp40, tmp60);		\
	tmp00 = _mm_sub_epi16(tmp00, tmp20);		\
	txmm6 = _mm_add_epi16(tmp00, tmp40);		\
	tmp00 = _mm_add_epi16(txmm7, txmm1);		\
	tmp70 = _mm_sub_epi16(txmm7, txmm1);		\
	tmp10 = _mm_add_epi16(txmm3, txmm2);		\
	tmp60 = _mm_sub_epi16(txmm3, txmm2);		\
	tmp20 = _mm_add_epi16(txmm0, txmm4);		\
	tmp50 = _mm_sub_epi16(txmm0, txmm4);		\
	tmp30 = _mm_add_epi16(txmm8, txmm6);		\
	tmp40 = _mm_sub_epi16(txmm8, txmm6);

#define	LOAD_IDCT_ROW_C_Q1				 	\
	txmm0 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16_Q1[0]);	\
	txmm1 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16_Q1[8]);	\
	txmm2 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16_Q1[16]);	\
	txmm3 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16_Q1[24]);	\
	xmmc0 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16_Q1[32]);	\
	xmmc2 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16_Q1[40]);


#define	LOAD_IDCT_ROW_C					\
	txmm0 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[0]);	\
	txmm1 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[8]);	\
	txmm2 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[16]);	\
	txmm3 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[24]);	\
	txmm4 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[32]);	\
	txmm5 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[40]);	\
	txmm6 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[48]);	\
	txmm7 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[56]); \
	xmmc0 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[64]);	\
	xmmc2 = _mm_loadu_si128((__m128i *)&IDCT_TABLE_16[72]);

#define	ROW_IDCT_S16_Q1(m, n)				\
	ROW_IDCT_Q1(m, n)				\
	tmp##m = _mm_sub_epi16(t##n, xmmc0);		\
	_mm_storeu_si128((__m128i *)&dataptr[8*n], tmp##m);

#define	ROW_IDCT_S16(m, n)				\
	ROW_IDCT(m, n)					\
	tmp##m = _mm_sub_epi16(t##n, xmmc0);		\
	_mm_storeu_si128((__m128i *)&dataptr[8*n], tmp##m);

#define	ROW_IDCT_U8_Q1					\
	xmmc1 = _mm_set1_epi8(0x80);			\
	ROW_IDCT_Q1(00, 0);				\
	t0 = _mm_packs_epi16(t0, Czero);		\
	t0 = _mm_xor_si128(t0, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t0);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(10, 1);				\
	t1 = _mm_packs_epi16(t1, Czero);		\
	t1 = _mm_xor_si128(t1, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t1);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(20, 2);				\
	t2 = _mm_packs_epi16(t2, Czero);		\
	t2 = _mm_xor_si128(t2, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t2);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(30, 3);				\
	t3 = _mm_packs_epi16(t3, Czero);		\
	t3 = _mm_xor_si128(t3, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t3);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(40, 4);				\
	t4 = _mm_packs_epi16(t4, Czero);		\
	t4 = _mm_xor_si128(t4, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t4);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(50, 5);				\
	t5 = _mm_packs_epi16(t5, Czero);		\
	t5 = _mm_xor_si128(t5, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t5);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(60, 6);				\
	t6 = _mm_packs_epi16(t6, Czero);		\
	t6 = _mm_xor_si128(t6, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t6);	\
	dataptr += stride;				\
	ROW_IDCT_Q1(70, 7);				\
	t7 = _mm_packs_epi16(t7, Czero);		\
	t7 = _mm_xor_si128(t7, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t7);	\
	dataptr += stride;

#define	ROW_IDCT_U8					\
	xmmc1 = _mm_set1_epi8(0x80);			\
	ROW_IDCT(00, 0);				\
	t0 = _mm_packs_epi16(t0, Czero);		\
	t0 = _mm_xor_si128(t0, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t0);	\
	dataptr += stride;				\
	ROW_IDCT(10, 1);				\
	t1 = _mm_packs_epi16(t1, Czero);		\
	t1 = _mm_xor_si128(t1, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t1);	\
	dataptr += stride;				\
	ROW_IDCT(20, 2);				\
	t2 = _mm_packs_epi16(t2, Czero);		\
	t2 = _mm_xor_si128(t2, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t2);	\
	dataptr += stride;				\
	ROW_IDCT(30, 3);				\
	t3 = _mm_packs_epi16(t3, Czero);		\
	t3 = _mm_xor_si128(t3, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t3);	\
	dataptr += stride;				\
	ROW_IDCT(40, 4);				\
	t4 = _mm_packs_epi16(t4, Czero);		\
	t4 = _mm_xor_si128(t4, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t4);	\
	dataptr += stride;				\
	ROW_IDCT(50, 5);				\
	t5 = _mm_packs_epi16(t5, Czero);		\
	t5 = _mm_xor_si128(t5, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t5);	\
	dataptr += stride;				\
	ROW_IDCT(60, 6);				\
	t6 = _mm_packs_epi16(t6, Czero);		\
	t6 = _mm_xor_si128(t6, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t6);	\
	dataptr += stride;				\
	ROW_IDCT(70, 7);				\
	t7 = _mm_packs_epi16(t7, Czero);		\
	t7 = _mm_xor_si128(t7, xmmc1);			\
	_mm_storel_epi64((__m128i *)&dataptr[0], t7);	\
	dataptr += stride;


#define	ROW_IDCT(m, n)					\
	xt0 = _mm_shuffle_epi32(tmp##m, 0xB1);		\
	xt1 = _mm_shuffle_epi32(tmp##m, 0x4E);		\
	xt2 = _mm_shuffle_epi32(tmp##m, 0x1B);		\
	tmp01 = _mm_madd_epi16(xt0, txmm0);		\
	tmp11 = _mm_madd_epi16(tmp##m, txmm1);		\
	tmp21 = _mm_madd_epi16(xt1, txmm2);		\
	tmp31 = _mm_madd_epi16(xt2, txmm3);		\
	tmp01 = _mm_add_epi32(tmp01, tmp11);		\
	tmp21 = _mm_add_epi32(tmp21, tmp31);		\
	tmp01 = _mm_add_epi32(tmp01, tmp21);		\
	tmp01 = _mm_add_epi32(tmp01, xmmc2);		\
	tmp01 = _mm_srai_epi32(tmp01, 15);		\
	tmp41 = _mm_madd_epi16(xt0, txmm4);		\
	tmp51 = _mm_madd_epi16(tmp##m, txmm5);		\
	tmp61 = _mm_madd_epi16(xt1, txmm6);		\
	tmp71 = _mm_madd_epi16(xt2, txmm7);		\
	tmp41 = _mm_add_epi32(tmp41, tmp51);		\
	tmp61 = _mm_add_epi32(tmp61, tmp71);		\
	tmp41 = _mm_add_epi32(tmp41, tmp61);		\
	tmp41 = _mm_add_epi32(tmp41, xmmc2);		\
	tmp41 = _mm_srai_epi32(tmp41, 15);		\
	t##n = _mm_packs_epi32(tmp01, tmp41);


#define	ROW_IDCT_Q1(m, n)				\
	xt0 = _mm_unpacklo_epi64(tmp##m, tmp##m);	\
	xt1 = _mm_shuffle_epi32(xt0, 0xB1);		\
	tmp01 = _mm_madd_epi16(xt0, txmm0);		\
	tmp11 = _mm_madd_epi16(xt1, txmm1);		\
	tmp01 = _mm_add_epi32(tmp01, tmp11);		\
	tmp01 = _mm_add_epi32(tmp01, xmmc2);		\
	tmp01 = _mm_srai_epi32(tmp01, 15);		\
	tmp21 = _mm_madd_epi16(xt0, txmm2);		\
	tmp31 = _mm_madd_epi16(xt1, txmm3);		\
	tmp21 = _mm_add_epi32(tmp21, tmp31);		\
	tmp21 = _mm_add_epi32(tmp21, xmmc2);		\
	tmp21 = _mm_srai_epi32(tmp21, 15);		\
	t##n = _mm_packs_epi32(tmp01, tmp21);

#define	LOAD_ROW_TALBE						\
	xmmc0 = _mm_load_si128((__m128i *)&COL_TABLE[56]);	\
	xmmc1 = _mm_load_si128((__m128i *)&TABLE_16[0]);	\
	xmmc2 = _mm_load_si128((__m128i *)&TABLE_16[8]);	\
	xmmc3 = _mm_load_si128((__m128i *)&TABLE_16[16]);	\
	xmmc4 = _mm_load_si128((__m128i *)&TABLE_16[24]);

#define	ROW_DCT_U8(m)						\
	txmm7 = _mm_loadu_si128((__m128i *)pSrc);		\
	pSrc += stride;						\
	txmm0 = _mm_unpacklo_epi8(txmm7, Czero);		\
	ROW_DCT(m);

#define	ROW_DCT_S16(m)						\
	txmm0 = _mm_loadu_si128(pSrc++);			\
	ROW_DCT(m);

#define	ROW_DCT(m)					\
	txmm1 = _mm_shuffle_epi32(txmm0, 0x4E); /* 0x01001110 */\
	txmm1 = _mm_shufflehi_epi16(txmm1, 0x1B);		\
	txmm1 = _mm_shufflelo_epi16(txmm1, 0x1B);		\
	txmm2 = _mm_add_epi16(txmm1, txmm0);			\
	txmm3 = _mm_sub_epi16(txmm1, txmm0);			\
	txmm0 = _mm_unpackhi_epi16(txmm2, txmm3);		\
	txmm1 = _mm_shufflelo_epi16(txmm0, 0x4E);		\
	txmm1 = _mm_shufflehi_epi16(txmm1, 0x4E);		\
	txmm2 = _mm_shuffle_epi32(txmm0, 0x4E);			\
	txmm3 = _mm_shufflehi_epi16(txmm2, 0x4E);		\
	txmm3 = _mm_shufflelo_epi16(txmm3, 0x4E);		\
	txmm0 = _mm_slli_epi16(txmm0, 2);			\
	txmm1 = _mm_slli_epi16(txmm1, 2);			\
	txmm2 = _mm_slli_epi16(txmm2, 2);			\
	txmm3 = _mm_slli_epi16(txmm3, 2);			\
	txmm4 = _mm_mulhi_epi16(xmmc1, txmm0);			\
	txmm5 = _mm_mulhi_epi16(xmmc2, txmm1);			\
	txmm6 = _mm_mulhi_epi16(xmmc3, txmm2);			\
	txmm7 = _mm_mulhi_epi16(xmmc4, txmm3);			\
	txmm0 = _mm_add_epi16(txmm4, txmm5);			\
	txmm1 = _mm_add_epi16(txmm6, txmm7);			\
	tmp##m = _mm_add_epi16(txmm0, txmm1);			\
	txmm2 = _mm_cmpeq_epi16(tmp##m, Czero);			\
	txmm3 = _mm_andnot_si128(txmm2, xmmc0);			\
	tmp##m = _mm_add_epi16(tmp##m, txmm3);

#define	LOADCONT()				\
	txmm0 = _mm_load_si128((__m128i *)&COL_TABLE[0]);	\
	txmm1 = _mm_load_si128((__m128i *)&COL_TABLE[8]);	\
	txmm2 = _mm_load_si128((__m128i *)&COL_TABLE[16]);	\
	txmm3 = _mm_load_si128((__m128i *)&COL_TABLE[24]);	\
	txmm4 = _mm_load_si128((__m128i *)&COL_TABLE[32]);	\
	txmm5 = _mm_load_si128((__m128i *)&COL_TABLE[40]);	\
	txmm6 = _mm_load_si128((__m128i *)&COL_TABLE[48]);	\


#define	COL_DCT_S16()					\
	tmp01 = _mm_add_epi16(tmp00, tmp70);		\
	tmp71 = _mm_sub_epi16(tmp00, tmp70);		\
	tmp11 = _mm_add_epi16(tmp10, tmp60);		\
	tmp61 = _mm_sub_epi16(tmp10, tmp60);		\
	tmp21 = _mm_add_epi16(tmp20, tmp50);		\
	tmp51 = _mm_sub_epi16(tmp20, tmp50);		\
	tmp31 = _mm_add_epi16(tmp30, tmp40);		\
	tmp41 = _mm_sub_epi16(tmp30, tmp40);		\
	LOADCONT()					\
	/*  tmp01 = _mm_slli_epi16(tmp01, 1); */	\
	/*  tmp11 = _mm_slli_epi16(tmp11, 1); */  	\
	/*  tmp21 = _mm_slli_epi16(tmp21, 1); */  	\
	/*  tmp31 = _mm_slli_epi16(tmp31, 1); */  	\
	/*  tmp41 = _mm_slli_epi16(tmp41, 1); */  	\
	/*  tmp51 = _mm_slli_epi16(tmp51, 1); */  	\
	/*  tmp61 = _mm_slli_epi16(tmp61, 1); */  	\
	/*  tmp71 = _mm_slli_epi16(tmp71, 1); */	\
	tmp00 = _mm_add_epi16(tmp01, tmp31);		\
	tmp10 = _mm_add_epi16(tmp21, tmp11);		\
	tmp00 = _mm_mulhi_epi16(tmp00, txmm0);		\
	tmp10 = _mm_mulhi_epi16(tmp10, txmm0);		\
	t0 = _mm_add_epi16(tmp00, tmp10);		\
	/* t0 = _mm_srai_epi16(t0,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*0], t0);\
	t4 = _mm_sub_epi16(tmp00, tmp10);		\
	/* t4 = _mm_srai_epi16(t4,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*4], t4);\
	tmp40 = _mm_sub_epi16(tmp11, tmp21);		\
	tmp50 = _mm_mulhi_epi16(tmp40, txmm2);		\
	tmp60 = _mm_sub_epi16(tmp01, tmp31);		\
	tmp70 = _mm_mulhi_epi16(tmp60, txmm6);		\
	t2 = _mm_add_epi16(tmp50, tmp70);		\
	/* t2 = _mm_srai_epi16(t2,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*2], t2);\
	tmp40 = _mm_mulhi_epi16(tmp40, txmm6);		\
	tmp60 = _mm_mulhi_epi16(tmp60, txmm2);		\
	t6 = _mm_sub_epi16(tmp60, tmp40);		\
	/* t6 = _mm_srai_epi16(t6,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*6], t6);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm1);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm4);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm3);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm5);		\
	tmp40 = _mm_add_epi16(tmp00, tmp10);		\
	tmp50 = _mm_add_epi16(tmp20, tmp30);		\
	t1 = _mm_add_epi16(tmp40, tmp50);		\
	/* t1 = _mm_srai_epi16(t1,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*1], t1);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm4);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm5);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm1);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm3);		\
	tmp40 = _mm_add_epi16(tmp00, tmp10);		\
	tmp50 = _mm_sub_epi16(tmp30, tmp20);		\
	t3 = _mm_sub_epi16(tmp50, tmp40);		\
	/* t3 = _mm_srai_epi16(t3,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*3], t3);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm3);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm1);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm5);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm4);		\
	tmp40 = _mm_add_epi16(tmp00, tmp10);		\
	tmp50 = _mm_sub_epi16(tmp30, tmp20);		\
	t5 = _mm_add_epi16(tmp40, tmp50);		\
	/* t5 = _mm_srai_epi16(t5,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*5], t5);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm5);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm3);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm4);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm1);		\
	tmp40 = _mm_sub_epi16(tmp10, tmp00);		\
	tmp50 = _mm_sub_epi16(tmp30, tmp20);		\
	t7 = _mm_add_epi16(tmp40, tmp50);		\
	/* t7 = _mm_srai_epi16(t7,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*7], t7);

#define	COL_DCTQuantize_S16()				\
	tmp01 = _mm_add_epi16(tmp00, tmp70);		\
	tmp71 = _mm_sub_epi16(tmp00, tmp70);		\
	tmp11 = _mm_add_epi16(tmp10, tmp60);		\
	tmp61 = _mm_sub_epi16(tmp10, tmp60);		\
	tmp21 = _mm_add_epi16(tmp20, tmp50);		\
	tmp51 = _mm_sub_epi16(tmp20, tmp50);		\
	tmp31 = _mm_add_epi16(tmp30, tmp40);		\
	tmp41 = _mm_sub_epi16(tmp30, tmp40);		\
	LOADCONT()					\
	/*  tmp01 = _mm_slli_epi16(tmp01, 1); */	\
	/*  tmp11 = _mm_slli_epi16(tmp11, 1); */	\
	/*  tmp21 = _mm_slli_epi16(tmp21, 1); */	\
	/*  tmp31 = _mm_slli_epi16(tmp31, 1); */	\
	/*  tmp41 = _mm_slli_epi16(tmp41, 1); */	\
	/*  tmp51 = _mm_slli_epi16(tmp51, 1); */	\
	/*  tmp61 = _mm_slli_epi16(tmp61, 1); */	\
	/*  tmp71 = _mm_slli_epi16(tmp71, 1); */ 	\
	tmp00 = _mm_add_epi16(tmp01, tmp31);		\
	tmp10 = _mm_add_epi16(tmp21, tmp11);		\
	tmp00 = _mm_mulhi_epi16(tmp00, txmm0);		\
	tmp10 = _mm_mulhi_epi16(tmp10, txmm0);		\
	t0 = _mm_add_epi16(tmp00, tmp10);		\
	t2 = _mm_loadu_si128((__m128i *)ptable);	\
	t0 = _mm_slli_epi16(t0, 1);			\
	t0 = _mm_mulhi_epi16(t0, t2);			\
	/* t0 = _mm_srai_epi16(t0,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*0], t0);\
	t4 = _mm_sub_epi16(tmp00, tmp10);		\
	t2 = _mm_loadu_si128((__m128i *)(ptable + 4 * 8));\
	t4 = _mm_slli_epi16(t4, 1);			\
	t4 = _mm_mulhi_epi16(t4, t2);			\
	/* t4 = _mm_srai_epi16(t4,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*4], t4);\
	tmp40 = _mm_sub_epi16(tmp11, tmp21);		\
	tmp50 = _mm_mulhi_epi16(tmp40, txmm2);		\
	tmp60 = _mm_sub_epi16(tmp01, tmp31);		\
	tmp70 = _mm_mulhi_epi16(tmp60, txmm6);		\
	t2 = _mm_add_epi16(tmp50, tmp70);		\
	t0 = _mm_loadu_si128((__m128i *)(ptable + 2 * 8));\
	t2 = _mm_slli_epi16(t2, 1);			\
	t2 = _mm_mulhi_epi16(t2, t0);			\
	/* t2 = _mm_srai_epi16(t2,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*2], t2);\
	tmp40 = _mm_mulhi_epi16(tmp40, txmm6);		\
	tmp60 = _mm_mulhi_epi16(tmp60, txmm2);		\
	t6 = _mm_sub_epi16(tmp60, tmp40);		\
	t4 = _mm_loadu_si128((__m128i *)(ptable + 6 * 8));\
	t6 = _mm_slli_epi16(t6, 1);			\
	t6 = _mm_mulhi_epi16(t6, t4);		\
	/* t6 = _mm_srai_epi16(t6,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*6], t6);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm1);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm4);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm3);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm5);		\
	tmp40 = _mm_add_epi16(tmp00, tmp10);		\
	tmp50 = _mm_add_epi16(tmp20, tmp30);		\
	t1 = _mm_add_epi16(tmp40, tmp50);		\
	t0 = _mm_loadu_si128((__m128i *)(ptable + 1 * 8));\
	t1 = _mm_slli_epi16(t1, 1);			\
	t1 = _mm_mulhi_epi16(t1, t0);		\
	/* t1 = _mm_srai_epi16(t1,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*1], t1);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm4);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm5);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm1);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm3);		\
	tmp40 = _mm_add_epi16(tmp00, tmp10);		\
	tmp50 = _mm_sub_epi16(tmp30, tmp20);		\
	t3 = _mm_sub_epi16(tmp50, tmp40);		\
	t0 = _mm_loadu_si128((__m128i *)(ptable + 3 * 8));\
	t3 = _mm_slli_epi16(t3, 1);			\
	t3 = _mm_mulhi_epi16(t3, t0);		\
	/* t3 = _mm_srai_epi16(t3,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*3], t3);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm3);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm1);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm5);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm4);		\
	tmp40 = _mm_add_epi16(tmp00, tmp10);		\
	tmp50 = _mm_sub_epi16(tmp30, tmp20);		\
	t5 = _mm_add_epi16(tmp40, tmp50);		\
	t0 = _mm_loadu_si128((__m128i *)(ptable + 5 * 8));\
	t5 = _mm_slli_epi16(t5, 1);			\
	t5 = _mm_mulhi_epi16(t5, t0);		\
	/* t5 = _mm_srai_epi16(t5,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*5], t5);\
	tmp00 = _mm_mulhi_epi16(tmp41, txmm5);		\
	tmp10 = _mm_mulhi_epi16(tmp51, txmm3);		\
	tmp20 = _mm_mulhi_epi16(tmp61, txmm4);		\
	tmp30 = _mm_mulhi_epi16(tmp71, txmm1);		\
	tmp40 = _mm_sub_epi16(tmp10, tmp00);		\
	tmp50 = _mm_sub_epi16(tmp30, tmp20);		\
	t7 = _mm_add_epi16(tmp40, tmp50);		\
	t0 = _mm_loadu_si128((__m128i *)(ptable + 7 * 8));\
	t7 = _mm_slli_epi16(t7, 1);			\
	t7 = _mm_mulhi_epi16(t7, t0);		\
	/* t7 = _mm_srai_epi16(t7,1); */		\
	_mm_storeu_si128((__m128i *)&dataptr[DCTSIZE*7], t7);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_VIDEO_H */
