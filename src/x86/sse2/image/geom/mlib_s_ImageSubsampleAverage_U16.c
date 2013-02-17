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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageSubsampleAverage_U16.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_U16_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_u16
#define	YTYPE	mlib_u32

#define	XTYPE	YTYPE

#define	USE_SHIFT

#include <mlib_ImageSubsampleAverage.h>

/* *********************************************************** */

static void FUNC(
    y1i) (
    YTYPE * buff,
    DTYPE * sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	__m128i txmm0, txmm1, txmm3;
	__m128i txmm2 = _mm_setzero_si128();
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize - 7; i += 8) {
		txmm0 = _mm_loadu_si128((__m128i *)(sp + i));
		txmm1 = _mm_unpacklo_epi16(txmm0, txmm2);
		_mm_storeu_si128((__m128i *)(buff + i), txmm1);
		txmm3 = _mm_unpackhi_epi16(txmm0, txmm2);
		_mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);
	}

	for (; i < xsize; i++) {
		buff[i] = sp[i];
	}
}

#define	STORE2						\
{								\
		__m128i txmm0, txmm1, txmm2, txmm3, txmm4;	\
		__m128i txmm5, txmm6;				\
		txmm0 = _mm_setzero_si128();			\
	    for (i = 0; i < xsize - 7; i += 8) {                \
		    txmm1 = _mm_loadu_si128((__m128i *)(sp + i));\
		    txmm2 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		    txmm3 = _mm_unpackhi_epi16(txmm1, txmm0);	\
		    txmm1 = _mm_unpacklo_epi16(txmm1, txmm0);	\
		    txmm4 = _mm_unpackhi_epi16(txmm2, txmm0);	\
		    txmm2 = _mm_unpacklo_epi16(txmm2, txmm0);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm2);	\
		    _mm_storeu_si128((__m128i *)(buff + i), txmm1);\
		    txmm3 = _mm_add_epi32(txmm3, txmm4);	\
		    _mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);\
	    }							\
}

#define	ADD2							\
{								\
		__m128i txmm0, txmm1, txmm2, txmm3, txmm4;	\
		__m128i txmm5, txmm6;				\
		txmm0 = _mm_setzero_si128();			\
	    for (i = 0; i < xsize - 7; i += 8) {                \
		    txmm1 = _mm_loadu_si128((__m128i *)(sp + i));\
		    txmm2 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		    txmm5 = _mm_loadu_si128((__m128i *)(buff + i));\
		    txmm6 = _mm_loadu_si128((__m128i *)(buff + i + 4));\
		    txmm3 = _mm_unpackhi_epi16(txmm1, txmm0);	\
		    txmm1 = _mm_unpacklo_epi16(txmm1, txmm0);	\
		    txmm4 = _mm_unpackhi_epi16(txmm2, txmm0);	\
		    txmm2 = _mm_unpacklo_epi16(txmm2, txmm0);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm2);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm5);	\
		    _mm_storeu_si128((__m128i *)(buff + i), txmm1);\
		    txmm3 = _mm_add_epi32(txmm3, txmm4);	\
		    txmm3 = _mm_add_epi32(txmm3, txmm6);	\
		    _mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);\
	    }							\
}

#define	ADD3							\
{								\
		__m128i txmm0, txmm1, txmm2, txmm3, txmm4;	\
		__m128i txmm5, txmm6;				\
		__m128i xmm5, xmm6;				\
		txmm0 = _mm_setzero_si128();			\
	    for (i = 0; i < xsize - 7; i += 8) {		\
		    txmm1 = _mm_loadu_si128((__m128i *)(sp + i));\
		    txmm2 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		    xmm5 = _mm_loadu_si128((__m128i *)(sp2 + i));\
		    txmm5 = _mm_loadu_si128((__m128i *)(buff + i));\
		    txmm6 = _mm_loadu_si128((__m128i *)(buff + i + 4));\
		    txmm3 = _mm_unpackhi_epi16(txmm1, txmm0);	\
		    txmm1 = _mm_unpacklo_epi16(txmm1, txmm0);	\
		    txmm4 = _mm_unpackhi_epi16(txmm2, txmm0);	\
		    txmm2 = _mm_unpacklo_epi16(txmm2, txmm0);	\
		    xmm6 = _mm_unpackhi_epi16(xmm5, txmm0);	\
		    xmm5 = _mm_unpacklo_epi16(xmm5, txmm0);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm2);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm5);	\
		    txmm1 = _mm_add_epi32(txmm1, xmm5);	\
		    _mm_storeu_si128((__m128i *)(buff + i), txmm1);\
		    txmm3 = _mm_add_epi32(txmm3, txmm4);	\
		    txmm3 = _mm_add_epi32(txmm3, txmm6);	\
		    txmm3 = _mm_add_epi32(txmm3, xmm6);	\
		    _mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);\
	    }							\
}

#define	STORE3							\
{								\
		__m128i txmm0, txmm1, txmm2, txmm3, txmm4;	\
		__m128i txmm5, txmm6;				\
		__m128i xmm5, xmm6;				\
		txmm0 = _mm_setzero_si128();			\
	    for (i = 0; i < xsize - 7; i += 8) {		\
		    txmm1 = _mm_loadu_si128((__m128i *)(sp + i));\
		    txmm2 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		    xmm5 = _mm_loadu_si128((__m128i *)(sp2 + i));\
		    txmm3 = _mm_unpackhi_epi16(txmm1, txmm0);	\
		    txmm1 = _mm_unpacklo_epi16(txmm1, txmm0);	\
		    txmm4 = _mm_unpackhi_epi16(txmm2, txmm0);	\
		    txmm2 = _mm_unpacklo_epi16(txmm2, txmm0);	\
		    xmm6 = _mm_unpackhi_epi16(xmm5, txmm0);	\
		    xmm5 = _mm_unpacklo_epi16(xmm5, txmm0);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm2);	\
		    txmm1 = _mm_add_epi32(txmm1, xmm5);	\
		    _mm_storeu_si128((__m128i *)(buff + i), txmm1);\
		    txmm3 = _mm_add_epi32(txmm3, txmm4);	\
		    txmm3 = _mm_add_epi32(txmm3, xmm6);	\
		    _mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);\
	    }							\
}

#define	ADD4							\
{								\
		__m128i txmm0, txmm1, txmm2, txmm3, txmm4;	\
		__m128i txmm5, txmm6;				\
		__m128i xmm5, xmm6;				\
		__m128i rxmm1, rxmm2;				\
		txmm0 = _mm_setzero_si128();			\
	    for (i = 0; i < xsize - 7; i += 8) {		\
		    txmm1 = _mm_loadu_si128((__m128i *)(sp + i));\
		    txmm2 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		    xmm5 = _mm_loadu_si128((__m128i *)(sp2 + i));\
		    rxmm1 = _mm_loadu_si128((__m128i *)(sp3 + i)); \
		    txmm5 = _mm_loadu_si128((__m128i *)(buff + i));\
		    txmm6 = _mm_loadu_si128((__m128i *)(buff + i + 4));\
		    txmm3 = _mm_unpackhi_epi16(txmm1, txmm0);	\
		    txmm1 = _mm_unpacklo_epi16(txmm1, txmm0);	\
		    txmm4 = _mm_unpackhi_epi16(txmm2, txmm0);	\
		    txmm2 = _mm_unpacklo_epi16(txmm2, txmm0);	\
		    xmm6 = _mm_unpackhi_epi16(xmm5, txmm0);	\
		    xmm5 = _mm_unpacklo_epi16(xmm5, txmm0);	\
		    rxmm2 = _mm_unpackhi_epi16(rxmm1, txmm0);	\
		    rxmm1 = _mm_unpacklo_epi16(rxmm1, txmm0);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm2);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm5);	\
		    txmm1 = _mm_add_epi32(txmm1, xmm5);	\
		    txmm1 = _mm_add_epi32(txmm1, rxmm1);	\
		    _mm_storeu_si128((__m128i *)(buff + i), txmm1);\
		    txmm3 = _mm_add_epi32(txmm3, txmm4);	\
		    txmm3 = _mm_add_epi32(txmm3, txmm6);	\
		    txmm3 = _mm_add_epi32(txmm3, xmm6);	\
		    txmm3 = _mm_add_epi32(txmm3, rxmm2);	\
		    _mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);\
	    }							\
}

#define	STORE4							\
{								\
		__m128i txmm0, txmm1, txmm2, txmm3, txmm4;	\
		__m128i xmm5, xmm6;				\
		__m128i rxmm1, rxmm2;				\
		txmm0 = _mm_setzero_si128();			\
	    for (i = 0; i < xsize - 7; i += 8) { 		\
		    txmm1 = _mm_loadu_si128((__m128i *)(sp + i));\
		    txmm2 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		    xmm5 = _mm_loadu_si128((__m128i *)(sp2 + i));\
		    rxmm1 = _mm_loadu_si128((__m128i *)(sp3 + i)); \
		    txmm3 = _mm_unpackhi_epi16(txmm1, txmm0);	\
		    txmm1 = _mm_unpacklo_epi16(txmm1, txmm0);	\
		    txmm4 = _mm_unpackhi_epi16(txmm2, txmm0);	\
		    txmm2 = _mm_unpacklo_epi16(txmm2, txmm0);	\
		    xmm6 = _mm_unpackhi_epi16(xmm5, txmm0);	\
		    xmm5 = _mm_unpacklo_epi16(xmm5, txmm0);	\
		    rxmm2 = _mm_unpackhi_epi16(rxmm1, txmm0);	\
		    rxmm1 = _mm_unpacklo_epi16(rxmm1, txmm0);	\
		    txmm1 = _mm_add_epi32(txmm1, txmm2);	\
		    txmm1 = _mm_add_epi32(txmm1, xmm5);	\
		    txmm1 = _mm_add_epi32(txmm1, rxmm1);	\
		    _mm_storeu_si128((__m128i *)(buff + i), txmm1);\
		    txmm3 = _mm_add_epi32(txmm3, txmm4);	\
		    txmm3 = _mm_add_epi32(txmm3, xmm6);	\
		    txmm3 = _mm_add_epi32(txmm3, rxmm2);	\
		    _mm_storeu_si128((__m128i *)(buff + i + 4), txmm3);\
	    }							\
}

/* *********************************************************** */

#define	FUNC_Y(SUFF, ADD)                                                 \
	static void FUNC(y2##SUFF) (YTYPE * buff, DTYPE * sp,             \
	    mlib_s32 slb, mlib_s32 xsize)                                 \
	{                                                                 \
	    mlib_s32 i;                                                   \
	    DTYPE *sp1 = sp + slb;                                        \
		ADD##2;					                  \
	    for (; i < xsize; i++) {                                      \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i]);                     \
	    }                                                             \
	}                                                                 \
	                                                                  \
/* *********************************************************** */	  \
	static void FUNC(y3##SUFF) (YTYPE * buff, DTYPE * sp,             \
	    mlib_s32 slb, mlib_s32 xsize)                                 \
	{                                                                 \
	    mlib_s32 i;                                                   \
	    DTYPE *sp1 = sp + slb;                                        \
	    DTYPE *sp2 = sp + 2 * slb;                                    \
		ADD##3;	                                                  \
	    for (; i < xsize; i++) {                                      \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i] + sp2[i]);            \
	    }                                                             \
	}                                                                 \
	                                                                  \
/* *********************************************************** */	  \
	static void FUNC(y4##SUFF) (YTYPE * buff, DTYPE * sp,             \
	    mlib_s32 slb, mlib_s32 xsize)                                 \
	{                                                                 \
	    mlib_s32 i;                                                   \
	    DTYPE *sp1 = sp + slb;                                        \
	    DTYPE *sp2 = sp + 2 * slb;                                    \
	    DTYPE *sp3 = sp + 3 * slb;                                    \
	                                                                  \
		ADD##4;						          \
	    for (; i < xsize; i++) {                                      \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i] + sp2[i] + sp3[i]);   \
	    }                                                             \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src
#define	ADD(dst, src)	dst = dst + src

FUNC_Y(i, STORE)
FUNC_Y(a, ADD)

static const func_y func_arr_yi[] = {
	FUNC(y1i),
	FUNC(y2i),
	FUNC(y3i),
	FUNC(y4i)
};

static const func_y func_arr_ya[] = {
	NULL,
	FUNC(y2a),
	FUNC(y3a),
	FUNC(y4a)
};

/* *********************************************************** */

#define	INIT_ADD                                                \
	XTYPE *buff = ws->buff;                                 \
	mlib_s32 *x_ind = ws->x_ind;                            \
	mlib_s32 xsize = ws->xsize;                             \
	mlib_s32 i;                                             \
	__m128i txmm0, txmm1, txmm2, txmm3;			\
	YTYPE *sp;                                              \
	XTYPE ss

/* *********************************************************** */

#define	INIT_SHF                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	__m128i rnd_xmm0 = _mm_set1_epi32((mlib_s32)rnd_ini);	\
	DTYPE *dl = ws->dl;                                     \
	mlib_s32 shift = ws->shift;				\
	__m128i C1 = _mm_set1_epi32((mlib_s32)0x8000);		\
	__m128i C2 = _mm_set1_epi16((mlib_s16)(-0x8000));

/* *********************************************************** */

#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_d64 d_mul = ws->d_mul;				\
	mlib_s32 shift0 = -1;					\
	mlib_d64 d = d_mul;					\
	while (d * 2 < 1.0) 					\
	{							\
		d = d *2;					\
		shift0++;					\
	}							\
	mlib_d64 d_mul1 = d_mul * (1 << (16 + shift0));		\
	mlib_s32 d_int = (mlib_s32)(d_mul * (1 << (16 + shift0)));\
	mlib_s32 d_int2 = (d_int << 16) | d_int;		\
	__m128i d_mul_xmm = _mm_set1_epi32(d_int2);		\
	__m128i rnd_xmm1 = _mm_set1_epi32(rnd_ini);		\
	__m128i C1 = _mm_set1_epi32((mlib_s32)0x8000);		\
	__m128i C2 = _mm_set1_epi16((mlib_s16)(-0x8000));	\
	d_mul1 = d_mul1 - d_int;				\
	mlib_s32 round_flag = ((d_mul1 > 0) && (d_mul1 < 0.5))? 1 : 0;\
	__m128i round = _mm_set1_epi16(round_flag);		\
	mlib_s32 round_flag1 = ((d_mul1) >= 0.5)? 1 : 0;	\
	__m128i round1 = _mm_set1_epi16(round_flag1);		\
	d_mul_xmm = _mm_add_epi16(d_mul_xmm, round1);

/* *********************************************************** */

#define	SAVE_ADD(src)	ss = *buff + src; (*buff++) = ss

#define	SAVE_ADD_SSE2(src0, src1)				\
	txmm1 = _mm_loadu_si128((__m128i *)buff);		\
	txmm2 = _mm_loadu_si128((__m128i *)(buff + 4));		\
	txmm1 = _mm_add_epi32(txmm1, src0);			\
	_mm_storeu_si128((__m128i *)buff, txmm1);		\
	txmm2 = _mm_add_epi32(txmm2, src1);			\
	_mm_storeu_si128((__m128i *)(buff + 4), txmm2);		\
	buff += 8;
/* *********************************************************** */

#define	SAVE_SHF_SSE2(src0, src1)                               \
	txmm1 = _mm_loadu_si128((__m128i *)buff);		\
	txmm2 = _mm_loadu_si128((__m128i *)(buff + 4));		\
	txmm1 = _mm_add_epi32(txmm1, src0);			\
	txmm2 = _mm_add_epi32(txmm2, src1);			\
	txmm1 = _mm_srli_epi32(txmm1, shift);			\
	txmm1 = _mm_sub_epi32(txmm1, C1);			\
	txmm2 = _mm_srli_epi32(txmm2, shift);			\
	txmm2 = _mm_sub_epi32(txmm2, C1);			\
	txmm1 = _mm_packs_epi32(txmm1, txmm2);			\
	txmm1 = _mm_sub_epi16(txmm1, C2);			\
	_mm_storeu_si128((__m128i *)dl, txmm1);			\
	_mm_storeu_si128((__m128i *)buff, rnd_xmm0);		\
	_mm_storeu_si128((__m128i *)(buff + 4), rnd_xmm0);	\
	dl += 8;						\
	buff += 8;

#define	SAVE_SHF(src)     	                                \
	ss = *buff + src;                                       \
	(*dl++) = ss >> shift;                                  \
	(*buff++) = rnd_ini

/* *********************************************************** */

#define	SAVE_MUL(src)                                           \
	ss = *buff + src;                                       \
	(*dl++) = (DTYPE) (ss * d_mul);                         \
	(*buff++) = rnd_ini

#define	SAVE_MUL_SSE2(src0, src1)                               \
	txmm1 = _mm_loadu_si128((__m128i *)buff);		\
	txmm2 = _mm_loadu_si128((__m128i *)(buff + 4));		\
	txmm1 = _mm_add_epi32(txmm1, src0);			\
	txmm2 = _mm_add_epi32(txmm2, src1);			\
	txmm0 = _mm_mulhi_epu16(txmm1, d_mul_xmm);		\
	txmm1 = _mm_mullo_epi16(txmm1, d_mul_xmm);		\
	txmm3 = _mm_mulhi_epu16(txmm2, d_mul_xmm);		\
	txmm2 = _mm_mullo_epi16(txmm2, d_mul_xmm);		\
	txmm1 = _mm_srli_epi32(txmm1, 16);			\
	txmm2 = _mm_srli_epi32(txmm2, 16);			\
	txmm1 = _mm_add_epi32(txmm0, txmm1);			\
	txmm2 = _mm_add_epi32(txmm2, txmm3);			\
	txmm1 = _mm_srli_epi32(txmm1, shift0);			\
	txmm2 = _mm_srli_epi32(txmm2, shift0);			\
	txmm1 = _mm_sub_epi32(txmm1, C1);			\
	txmm2 = _mm_sub_epi32(txmm2, C1);			\
	txmm0 = _mm_packs_epi32(txmm1, txmm2);			\
	txmm0 = _mm_sub_epi16(txmm0, C2);			\
	txmm0 = _mm_add_epi16(txmm0, round);			\
	_mm_storeu_si128((__m128i *)(buff), rnd_xmm1);	 	\
	_mm_storeu_si128((__m128i *)(buff + 4), rnd_xmm1);	\
	_mm_storeu_si128((__m128i *)dl, txmm0);		 	\
	dl += 8;						\
	buff += 8;
/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)	                         \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp0 = *((YTYPE *) (sl + x_ind[i + 0]));          \
		sp1 = *((YTYPE *) (sl + x_ind[i + 1]));          \
		sp2 = *((YTYPE *) (sl + x_ind[i + 2]));          \
		sp3 = *((YTYPE *) (sl + x_ind[i + 3]));          \
		sp4 = *((YTYPE *) (sl + x_ind[i + 4]));          \
		sp5 = *((YTYPE *) (sl + x_ind[i + 5]));          \
		sp6 = *((YTYPE *) (sl + x_ind[i + 6]));          \
		sp7 = *((YTYPE *) (sl + x_ind[i + 7]));          \
		src_xmm0 = _mm_set_epi32(sp3, sp2, sp1, sp0);	 \
		src_xmm1 = _mm_set_epi32(sp7, sp6, sp5, sp4);	 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0]);                                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch1_x2##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		    sp0 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		    sp1 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		    sp2 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		    sp3 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		    sp4 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		    sp5 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		    sp6 = sp[0] + sp[1];			 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		    sp7 = sp[0] + sp[1];			 \
		src_xmm0 = _mm_set_epi32(sp3, sp2, sp1, sp0);	 \
		src_xmm1 = _mm_set_epi32(sp7, sp6, sp5, sp4);	 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {   		                 \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[1]);                             \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch1_x3##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		    sp0 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		    sp1 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		    sp2 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		    sp3 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		    sp4 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		    sp5 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		    sp6 = sp[0] + sp[1] + sp[2];		 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		    sp7 = sp[0] + sp[1] + sp[2];		 \
		src_xmm0 = _mm_set_epi32(sp3, sp2, sp1, sp0);	 \
		src_xmm1 = _mm_set_epi32(sp7, sp6, sp5, sp4);	 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[1] + sp[2]);                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch1_x4##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		    sp0 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		    sp1 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		    sp2 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		    sp3 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		    sp4 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		    sp5 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		    sp6 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		    sp7 = sp[0] + sp[1] + sp[2] + sp[3];	 \
		src_xmm0 = _mm_set_epi32(sp3, sp2, sp1, sp0);	 \
		src_xmm1 = _mm_set_epi32(sp7, sp6, sp5, sp4);	 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[1] + sp[2] + sp[3]);             \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3;				 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		src_xmm0 = _mm_unpacklo_epi64(t0, t1);		 \
		src_xmm1 = _mm_unpacklo_epi64(t2, t3);		 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {  	            	         \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0]);                                     \
		SAVE(sp[1]);                                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x2##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3;				 \
	    __m128i x0, x1, x2, x3;				 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		x0 = _mm_srli_si128(t0, 8);			 \
		t0 = _mm_add_epi32(t0, x0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		x1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_add_epi32(t1, x1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		x2 = _mm_srli_si128(t2, 8);			 \
		t2 = _mm_add_epi32(t2, x2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		x3 = _mm_srli_si128(t3, 8);			 \
		t3 = _mm_add_epi32(t3, x3);			 \
		src_xmm0 = _mm_unpacklo_epi64(t0, t1);		 \
		src_xmm1 = _mm_unpacklo_epi64(t2, t3);		 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[2]);                             \
		SAVE(sp[1] + sp[3]);                             \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x3##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3;				 \
	    __m128i x0, x1, x2, x3;				 \
	    __m128i y0, y1, y2, y3;				 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		y0 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		x0 = _mm_srli_si128(t0, 8);			 \
		t0 = _mm_add_epi32(t0, x0);			 \
		t0 = _mm_add_epi32(t0, y0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		y1 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		x1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_add_epi32(t1, x1);			 \
		t1 = _mm_add_epi32(t1, y1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		y2 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		x2 = _mm_srli_si128(t2, 8);			 \
		t2 = _mm_add_epi32(t2, x2);			 \
		t2 = _mm_add_epi32(t2, y2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		y3 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		x3 = _mm_srli_si128(t3, 8);			 \
		t3 = _mm_add_epi32(t3, x3);			 \
		t3 = _mm_add_epi32(t3, y3);			 \
		src_xmm0 = _mm_unpacklo_epi64(t0, t1);		 \
		src_xmm1 = _mm_unpacklo_epi64(t2, t3);		 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[2] + sp[4]);                     \
		SAVE(sp[1] + sp[3] + sp[5]);                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x4##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3;				 \
	    __m128i x0, x1, x2, x3;				 \
	    __m128i y0, y1, y2, y3;				 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		y0 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		t0 = _mm_add_epi32(t0, y0);			 \
		x0 = _mm_srli_si128(t0, 8);			 \
		t0 = _mm_add_epi32(t0, x0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		y1 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		t1 = _mm_add_epi32(t1, y1);			 \
		x1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_add_epi32(t1, x1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		y2 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		t2 = _mm_add_epi32(t2, y2);			 \
		x2 = _mm_srli_si128(t2, 8);			 \
		t2 = _mm_add_epi32(t2, x2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		y3 = _mm_loadu_si128((__m128i *)(sp + 4));	 \
		t3 = _mm_add_epi32(t3, y3);			 \
		x3 = _mm_srli_si128(t3, 8);			 \
		t3 = _mm_add_epi32(t3, x3);			 \
		src_xmm0 = _mm_unpacklo_epi64(t0, t1);		 \
		src_xmm1 = _mm_unpacklo_epi64(t2, t3);		 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[2] + sp[4] + sp[6]);             \
		SAVE(sp[1] + sp[3] + sp[5] + sp[7]);             \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch3_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3, t4, t5, t6, t7;		 \
	    __m128i x0, x1, x2, x3, x4, x5, x6, x7;		 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		x0 = _mm_slli_si128(t0, 4);			 \
		x0 = _mm_srli_si128(x0, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		t1 = _mm_slli_si128(t1, 4);			 \
		x1 = _mm_slli_si128(t1, 8);			 \
		t0 = _mm_add_epi32(x0, x1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		t1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_unpacklo_epi64(t1, t2);		 \
		t2 = _mm_slli_si128(t2, 4);			 \
		t2 = _mm_srli_si128(t2, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		x3 = _mm_slli_si128(t3, 4);			 \
		t2 = _mm_add_epi32(t2, x3);			 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		t4 = _mm_loadu_si128((__m128i *)sp);		 \
		x4 = _mm_slli_si128(t4, 4);			 \
		x4 = _mm_srli_si128(x4, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		t5 = _mm_loadu_si128((__m128i *)sp);		 \
		t5 = _mm_slli_si128(t5, 4);			 \
		x5 = _mm_slli_si128(t5, 8);			 \
		t3 = _mm_add_epi32(x4, x5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		t6 = _mm_loadu_si128((__m128i *)sp);		 \
		t5 = _mm_srli_si128(t5, 8);			 \
		t4 = _mm_unpacklo_epi64(t5, t6);		 \
		t6 = _mm_slli_si128(t6, 4);			 \
		t6 = _mm_srli_si128(t6, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		t7 = _mm_loadu_si128((__m128i *)sp);		 \
		x7 = _mm_slli_si128(t7, 4);			 \
		t5 = _mm_add_epi32(t6, x7);			 \
		SAVE##_SSE2(t0, t1);		                 \
		SAVE##_SSE2(t2, t3);		                 \
		SAVE##_SSE2(t4, t5);		                 \
	    }							 \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0]);                                     \
		SAVE(sp[1]);                                     \
		SAVE(sp[2]);                                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch3_x2##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3, t4, t5, t6, t7;		 \
	    __m128i x0, x1, x2, x3, x4, x5, x6, x7;		 \
	    __m128i y0, y1, y2, y3, y4, y5, y6, y7;		 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		y0 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t0 = _mm_add_epi32(t0, y0);			 \
		x0 = _mm_slli_si128(t0, 4);			 \
		x0 = _mm_srli_si128(x0, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		y1 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t1 = _mm_add_epi32(t1, y1);			 \
		t1 = _mm_slli_si128(t1, 4);			 \
		x1 = _mm_slli_si128(t1, 8);			 \
		t0 = _mm_add_epi32(x0, x1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		y2 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t2 = _mm_add_epi32(t2, y2);			 \
		t1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_unpacklo_epi64(t1, t2);		 \
		t2 = _mm_slli_si128(t2, 4);			 \
		t2 = _mm_srli_si128(t2, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		y3 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t3 = _mm_add_epi32(t3, y3);			 \
		x3 = _mm_slli_si128(t3, 4);			 \
		t2 = _mm_add_epi32(t2, x3);			 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		t4 = _mm_loadu_si128((__m128i *)sp);		 \
		y4 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t4 = _mm_add_epi32(t4, y4);			 \
		x4 = _mm_slli_si128(t4, 4);			 \
		x4 = _mm_srli_si128(x4, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		t5 = _mm_loadu_si128((__m128i *)sp);		 \
		y5 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t5 = _mm_add_epi32(t5, y5);			 \
		t5 = _mm_slli_si128(t5, 4);			 \
		x5 = _mm_slli_si128(t5, 8);			 \
		t3 = _mm_add_epi32(x4, x5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		t6 = _mm_loadu_si128((__m128i *)sp);		 \
		y6 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t6 = _mm_add_epi32(t6, y6);			 \
		t5 = _mm_srli_si128(t5, 8);			 \
		t4 = _mm_unpacklo_epi64(t5, t6);		 \
		t6 = _mm_slli_si128(t6, 4);			 \
		t6 = _mm_srli_si128(t6, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		t7 = _mm_loadu_si128((__m128i *)sp);		 \
		y7 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		t7 = _mm_add_epi32(t7, y7);			 \
		x7 = _mm_slli_si128(t7, 4);			 \
		t5 = _mm_add_epi32(t6, x7);			 \
		SAVE##_SSE2(t0, t1);		                 \
		SAVE##_SSE2(t2, t3);		                 \
		SAVE##_SSE2(t4, t5);		                 \
	    }							 \
	    for (; i < xsize; i++) {	                         \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[3]);                             \
		SAVE(sp[1] + sp[4]);                             \
		SAVE(sp[2] + sp[5]);                             \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch3_x3##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3, t4, t5, t6, t7;		 \
	    __m128i x0, x1, x2, x3, x4, x5, x6, x7;		 \
	    __m128i y0, y1, y2, y3, y4, y5, y6, y7;		 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		y0 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x0 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y0 = _mm_add_epi32(x0, y0);			 \
		t0 = _mm_add_epi32(t0, y0);			 \
		x0 = _mm_slli_si128(t0, 4);			 \
		x0 = _mm_srli_si128(x0, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		y1 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x1 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y1 = _mm_add_epi32(x1, y1);			 \
		t1 = _mm_add_epi32(t1, y1);			 \
		t1 = _mm_slli_si128(t1, 4);			 \
		x1 = _mm_slli_si128(t1, 8);			 \
		t0 = _mm_add_epi32(x0, x1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		y2 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x2 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y2 = _mm_add_epi32(x2, y2);			 \
		t2 = _mm_add_epi32(t2, y2);			 \
		t1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_unpacklo_epi64(t1, t2);		 \
		t2 = _mm_slli_si128(t2, 4);			 \
		t2 = _mm_srli_si128(t2, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		y3 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x3 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y3 = _mm_add_epi32(x3, y3);			 \
		t3 = _mm_add_epi32(t3, y3);			 \
		x3 = _mm_slli_si128(t3, 4);			 \
		t2 = _mm_add_epi32(t2, x3);			 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		t4 = _mm_loadu_si128((__m128i *)sp);		 \
		y4 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x4 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y4 = _mm_add_epi32(x4, y4);			 \
		t4 = _mm_add_epi32(t4, y4);			 \
		x4 = _mm_slli_si128(t4, 4);			 \
		x4 = _mm_srli_si128(x4, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		t5 = _mm_loadu_si128((__m128i *)sp);		 \
		y5 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x5 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y5 = _mm_add_epi32(x5, y5);			 \
		t5 = _mm_add_epi32(t5, y5);			 \
		t5 = _mm_slli_si128(t5, 4);			 \
		x5 = _mm_slli_si128(t5, 8);			 \
		t3 = _mm_add_epi32(x4, x5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		t6 = _mm_loadu_si128((__m128i *)sp);		 \
		y6 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x6 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y6 = _mm_add_epi32(x6, y6);			 \
		t6 = _mm_add_epi32(t6, y6);			 \
		t5 = _mm_srli_si128(t5, 8);			 \
		t4 = _mm_unpacklo_epi64(t5, t6);		 \
		t6 = _mm_slli_si128(t6, 4);			 \
		t6 = _mm_srli_si128(t6, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		t7 = _mm_loadu_si128((__m128i *)sp);		 \
		y7 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x7 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		y7 = _mm_add_epi32(x7, y7);			 \
		t7 = _mm_add_epi32(t7, y7);			 \
		x7 = _mm_slli_si128(t7, 4);			 \
		t5 = _mm_add_epi32(t6, x7);			 \
		SAVE##_SSE2(t0, t1);		                 \
		SAVE##_SSE2(t2, t3);		                 \
		SAVE##_SSE2(t4, t5);		                 \
	    }							 \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[3] + sp[6]);                     \
		SAVE(sp[1] + sp[4] + sp[7]);                     \
		SAVE(sp[2] + sp[5] + sp[8]);                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch3_x4##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_s32  sp0, sp1, sp2, sp3;			 \
	    mlib_s32  sp4, sp5, sp6, sp7;			 \
	    __m128i t0, t1, t2, t3, t4, t5, t6, t7;		 \
	    __m128i x0, x1, x2, x3, x4, x5, x6, x7;		 \
	    __m128i y0, y1, y2, y3, y4, y5, y6, y7;		 \
	    __m128i r0, r1, r2, r3, r4, r5, r6, r7;		 \
	    __m128i src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 7; i += 8) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		y0 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x0 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r0 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x0 = _mm_add_epi32(r0, x0);			 \
		y0 = _mm_add_epi32(x0, y0);			 \
		t0 = _mm_add_epi32(t0, y0);			 \
		x0 = _mm_slli_si128(t0, 4);			 \
		x0 = _mm_srli_si128(x0, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		y1 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x1 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r1 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x1 = _mm_add_epi32(r1, x1);			 \
		y1 = _mm_add_epi32(x1, y1);			 \
		t1 = _mm_add_epi32(t1, y1);			 \
		t1 = _mm_slli_si128(t1, 4);			 \
		x1 = _mm_slli_si128(t1, 8);			 \
		t0 = _mm_add_epi32(x0, x1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		t2 = _mm_loadu_si128((__m128i *)sp);		 \
		y2 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x2 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r2 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x2 = _mm_add_epi32(r2, x2);			 \
		y2 = _mm_add_epi32(x2, y2);			 \
		t2 = _mm_add_epi32(t2, y2);			 \
		t1 = _mm_srli_si128(t1, 8);			 \
		t1 = _mm_unpacklo_epi64(t1, t2);		 \
		t2 = _mm_slli_si128(t2, 4);			 \
		t2 = _mm_srli_si128(t2, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		t3 = _mm_loadu_si128((__m128i *)sp);		 \
		y3 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x3 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r3 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x3 = _mm_add_epi32(r3, x3);			 \
		y3 = _mm_add_epi32(x3, y3);			 \
		t3 = _mm_add_epi32(t3, y3);			 \
		x3 = _mm_slli_si128(t3, 4);			 \
		t2 = _mm_add_epi32(t2, x3);			 \
		sp = (YTYPE *) (sl + x_ind[i + 4]);              \
		t4 = _mm_loadu_si128((__m128i *)sp);		 \
		y4 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x4 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r4 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x4 = _mm_add_epi32(r4, x4);			 \
		y4 = _mm_add_epi32(x4, y4);			 \
		t4 = _mm_add_epi32(t4, y4);			 \
		x4 = _mm_slli_si128(t4, 4);			 \
		x4 = _mm_srli_si128(x4, 4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 5]);              \
		t5 = _mm_loadu_si128((__m128i *)sp);		 \
		y5 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x5 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r5 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x5 = _mm_add_epi32(r5, x5);			 \
		y5 = _mm_add_epi32(x5, y5);			 \
		t5 = _mm_add_epi32(t5, y5);			 \
		t5 = _mm_slli_si128(t5, 4);			 \
		x5 = _mm_slli_si128(t5, 8);			 \
		t3 = _mm_add_epi32(x4, x5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 6]);              \
		t6 = _mm_loadu_si128((__m128i *)sp);		 \
		y6 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x6 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r6 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x6 = _mm_add_epi32(r6, x6);			 \
		y6 = _mm_add_epi32(x6, y6);			 \
		t6 = _mm_add_epi32(t6, y6);			 \
		t5 = _mm_srli_si128(t5, 8);			 \
		t4 = _mm_unpacklo_epi64(t5, t6);		 \
		t6 = _mm_slli_si128(t6, 4);			 \
		t6 = _mm_srli_si128(t6, 12);			 \
		sp = (YTYPE *) (sl + x_ind[i + 7]);              \
		t7 = _mm_loadu_si128((__m128i *)sp);		 \
		y7 = _mm_loadu_si128((__m128i *)(sp + 3));	 \
		x7 = _mm_loadu_si128((__m128i *)(sp + 6));	 \
		r7 = _mm_loadu_si128((__m128i *)(sp + 9));	 \
		x7 = _mm_add_epi32(r7, x7);			 \
		y7 = _mm_add_epi32(x7, y7);			 \
		t7 = _mm_add_epi32(t7, y7);			 \
		x7 = _mm_slli_si128(t7, 4);			 \
		t5 = _mm_add_epi32(t6, x7);			 \
		SAVE##_SSE2(t0, t1);		                 \
		SAVE##_SSE2(t2, t3);		                 \
		SAVE##_SSE2(t4, t5);		                 \
	    }							 \
	    for (; i < xsize; i++) { 	                         \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[3] + sp[6] + sp[9]);             \
		SAVE(sp[1] + sp[4] + sp[7] + sp[10]);            \
		SAVE(sp[2] + sp[5] + sp[8] + sp[11]);            \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128i t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		SAVE##_SSE2(t0, t1); 		                 \
	    }                                                    \
	    for (; i < xsize; i++) { 	                         \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0]);                                     \
		SAVE(sp[1]);                                     \
		SAVE(sp[2]);                                     \
		SAVE(sp[3]);                                     \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x2##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128i t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		t2 = _mm_loadu_si128((__m128i *)(sp + 4));       \
		t0 = _mm_add_epi32(t0, t2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		t3 = _mm_loadu_si128((__m128i *)(sp + 4));       \
		t1 = _mm_add_epi32(t1, t3);			 \
		SAVE##_SSE2(t0, t1); 		                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[4]);                             \
		SAVE(sp[1] + sp[5]);                             \
		SAVE(sp[2] + sp[6]);                             \
		SAVE(sp[3] + sp[7]);                             \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x3##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128i t0, t1, t2, t3, t4, t5;			 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		t2 = _mm_loadu_si128((__m128i *)(sp + 4));       \
		t4 = _mm_loadu_si128((__m128i *)(sp + 8));       \
		t2 = _mm_add_epi32(t4, t2);			 \
		t0 = _mm_add_epi32(t0, t2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		t3 = _mm_loadu_si128((__m128i *)(sp + 4));       \
		t5 = _mm_loadu_si128((__m128i *)(sp + 8));       \
		t3 = _mm_add_epi32(t5, t3);			 \
		t1 = _mm_add_epi32(t1, t3);			 \
		SAVE##_SSE2(t0, t1); 		                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[4] + sp[8]);                     \
		SAVE(sp[1] + sp[5] + sp[9]);                     \
		SAVE(sp[2] + sp[6] + sp[10]);                    \
		SAVE(sp[3] + sp[7] + sp[11]);                    \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x4##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128i t0, t1, t2, t3, t4, t5, t6, t7;		 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		t0 = _mm_loadu_si128((__m128i *)sp);		 \
		t2 = _mm_loadu_si128((__m128i *)(sp + 4));       \
		t4 = _mm_loadu_si128((__m128i *)(sp + 8));       \
		t6 = _mm_loadu_si128((__m128i *)(sp + 12));      \
		t4 = _mm_add_epi32(t4, t6);			 \
		t2 = _mm_add_epi32(t4, t2);			 \
		t0 = _mm_add_epi32(t0, t2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t1 = _mm_loadu_si128((__m128i *)sp);		 \
		t3 = _mm_loadu_si128((__m128i *)(sp + 4));       \
		t5 = _mm_loadu_si128((__m128i *)(sp + 8));       \
		t7 = _mm_loadu_si128((__m128i *)(sp + 12));      \
		t5 = _mm_add_epi32(t5, t7);			 \
		t3 = _mm_add_epi32(t5, t3);			 \
		t1 = _mm_add_epi32(t1, t3);			 \
		SAVE##_SSE2(t0, t1); 		                 \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
		sp = (YTYPE *) (sl + x_ind[i]);                  \
		SAVE(sp[0] + sp[4] + sp[8] + sp[12]);            \
		SAVE(sp[1] + sp[5] + sp[9] + sp[13]);            \
		SAVE(sp[2] + sp[6] + sp[10] + sp[14]);           \
		SAVE(sp[3] + sp[7] + sp[11] + sp[15]);           \
	    }                                                    \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static const func_x func_arr_x##SUFF[] = {               \
	    FUNC(ch1_x1##SUFF), FUNC(ch1_x2##SUFF),              \
		FUNC(ch1_x3##SUFF), FUNC(ch1_x4##SUFF),          \
		FUNC(ch2_x1##SUFF), FUNC(ch2_x2##SUFF),          \
		FUNC(ch2_x3##SUFF), FUNC(ch2_x4##SUFF),          \
		FUNC(ch3_x1##SUFF), FUNC(ch3_x2##SUFF),          \
		FUNC(ch3_x3##SUFF), FUNC(ch3_x4##SUFF),          \
		FUNC(ch4_x1##SUFF), FUNC(ch4_x2##SUFF),          \
		FUNC(ch4_x3##SUFF), FUNC(ch4_x4##SUFF)           \
	}

/* *********************************************************** */

FUNC_X(a, INIT_ADD, SAVE_ADD);
FUNC_X(m, INIT_MUL, SAVE_MUL);

#ifdef USE_SHIFT
FUNC_X(s, INIT_SHF, SAVE_SHF);
#endif /* USE_SHIFT */

/* *********************************************************** */
