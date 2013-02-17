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

#pragma ident	"@(#)mlib_s_ImageSubsampleAverage_S32.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_S32_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_s32
#define	YTYPE	mlib_d64

#define	XTYPE	YTYPE

/* #define USE_SHIFT */

#include <mlib_ImageSubsampleAverage.h>
#include <mlib_sse_utils.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

/* *********************************************************** */

static void FUNC(
    y1i) (
    YTYPE * buff,
    DTYPE * sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	__m128i txmm0, txmm1, txmm2, txmm3;
	__m128d t0, t1;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize - 3; i += 4) {
		txmm0 = _mm_loadu_si128((__m128i *)(sp + i));
		t0 = _mm_cvtepi32_pd(txmm0);
		txmm0 = _mm_srli_si128(txmm0, 8);
		t1 = _mm_cvtepi32_pd(txmm0);
		_mm_storeu_pd(buff + i, t0);
		_mm_storeu_pd(buff + i + 2, t1);
	}
	for (; i < xsize; i++) {
		buff[i] = sp[i];
	}
}

/* *********************************************************** */

#define	FUNC_Y(SUFF, ADD)                                                 \
	static void FUNC(y2##SUFF) (YTYPE * buff, DTYPE * sp,             \
	    mlib_s32 slb, mlib_s32 xsize)                                 \
	{                                                                 \
	    mlib_s32 i;                                                   \
	    DTYPE *sp1 = sp + slb;                                        \
	                                                                  \
	    __m128i txmm0, txmm1;		\
	    __m128d dst_xmm0, dst_xmm1, t0, t1; \
	    __m128d t2, t3;			\
						\
	    for (i = 0; i < xsize - 3; i += 4) {	\
		txmm0 = _mm_loadu_si128((__m128i *)(sp + i));\
		txmm1 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		t0 = _mm_cvtepi32_pd(txmm0);		\
		t2 = _mm_cvtepi32_pd(txmm1);		\
		t0 = _mm_add_pd(t0, t2);		\
		txmm0 = _mm_srli_si128(txmm0, 8);	\
		txmm1 = _mm_srli_si128(txmm1, 8);	\
		t1 = _mm_cvtepi32_pd(txmm0);		\
		t3 = _mm_cvtepi32_pd(txmm1);		\
		t1 = _mm_add_pd(t1, t3);		\
		ADD##_SSE2(t0, t1);			\
	    }					\
	    for (; i < xsize; i++) {                     \
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
	                                                                  \
	    __m128i txmm0, txmm1, txmm2;	\
	    __m128d dst_xmm0, dst_xmm1, t0, t1; \
	    __m128d t2, t3;			\
	    __m128d t4, t5;			\
						\
	    for (i = 0; i < xsize - 3; i += 4) {	\
		txmm0 = _mm_loadu_si128((__m128i *)(sp + i));\
		txmm1 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		txmm2 = _mm_loadu_si128((__m128i *)(sp2 + i));\
		t0 = _mm_cvtepi32_pd(txmm0);		\
		t2 = _mm_cvtepi32_pd(txmm1);		\
		t4 = _mm_cvtepi32_pd(txmm2);		\
		t0 = _mm_add_pd(t0, t2);		\
		t0 = _mm_add_pd(t0, t4);		\
		txmm0 = _mm_srli_si128(txmm0, 8);	\
		txmm1 = _mm_srli_si128(txmm1, 8);	\
		txmm2 = _mm_srli_si128(txmm2, 8);	\
		t1 = _mm_cvtepi32_pd(txmm0);		\
		t3 = _mm_cvtepi32_pd(txmm1);		\
		t5 = _mm_cvtepi32_pd(txmm2);		\
		t1 = _mm_add_pd(t1, t3);		\
		t1 = _mm_add_pd(t1, t5);		\
		ADD##_SSE2(t0, t1);			\
	    }					\
	    for (; i < xsize; i++) {                                 \
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
	    __m128i txmm0, txmm1, txmm2, txmm3;	\
	    __m128d dst_xmm0, dst_xmm1, t0, t1; \
	    __m128d t2, t3;			\
	    __m128d t4, t5;			\
	    __m128d t6, t7;			\
						\
	    for (i = 0; i < xsize - 3; i += 4) {	\
		txmm0 = _mm_loadu_si128((__m128i *)(sp + i));\
		txmm1 = _mm_loadu_si128((__m128i *)(sp1 + i));\
		txmm2 = _mm_loadu_si128((__m128i *)(sp2 + i));\
		txmm3 = _mm_loadu_si128((__m128i *)(sp3 + i));\
		t0 = _mm_cvtepi32_pd(txmm0);		\
		t2 = _mm_cvtepi32_pd(txmm1);		\
		t4 = _mm_cvtepi32_pd(txmm2);		\
		t6 = _mm_cvtepi32_pd(txmm3);		\
		t0 = _mm_add_pd(t0, t2);		\
		t0 = _mm_add_pd(t0, t4);		\
		t0 = _mm_add_pd(t0, t6);		\
		txmm0 = _mm_srli_si128(txmm0, 8);	\
		txmm1 = _mm_srli_si128(txmm1, 8);	\
		txmm2 = _mm_srli_si128(txmm2, 8);	\
		txmm3 = _mm_srli_si128(txmm3, 8);	\
		t1 = _mm_cvtepi32_pd(txmm0);		\
		t3 = _mm_cvtepi32_pd(txmm1);		\
		t5 = _mm_cvtepi32_pd(txmm2);		\
		t7 = _mm_cvtepi32_pd(txmm3);		\
		t1 = _mm_add_pd(t1, t3);		\
		t1 = _mm_add_pd(t1, t5);		\
		t1 = _mm_add_pd(t1, t7);		\
		ADD##_SSE2(t0, t1);			\
	    }					\
	    for (; i < xsize; i++) {                                 \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i] + sp2[i] + sp3[i]);   \
	    }                                                             \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src

#define	STORE_SSE2(src0, src1)		\
	_mm_storeu_pd(buff + i, src0);	\
	_mm_storeu_pd(buff + i + 2, src1);

#define	ADD(dst, src)	dst = dst + src

#define	ADD_SSE2(src0, src1)			\
	dst_xmm0 = _mm_loadu_pd((buff + i)); 	\
	dst_xmm1 = _mm_loadu_pd((buff + i + 2));\
	dst_xmm0 = _mm_add_pd(dst_xmm0, src0);  \
	dst_xmm1 = _mm_add_pd(dst_xmm1, src1);  \
	_mm_storeu_pd((buff + i), dst_xmm0);	\
	_mm_storeu_pd((buff + i + 2), dst_xmm1);

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
	YTYPE *sp;                                              \
	XTYPE ss;						\
	__m128d txmm0, txmm1, txmm2, txmm3;			\
	__m128i ixmm0, ixmm1, ixmm2, ixmm3;			\
	__m128d mask, mask1, mask2;

/* *********************************************************** */

#define	INIT_SHF                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_s32 shift = ws->shift

/* *********************************************************** */

#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_d64 d_mul = ws->d_mul;				\
	__m128d d_mul_xmm = _mm_set1_pd(d_mul);			\
	__m128d rnd_ini_xmm = _mm_set1_pd((mlib_d64)rnd_ini);

/* *********************************************************** */

#define	SAVE_ADD(src)	ss = *buff + src; (*buff++) = ss

#define	SAVE_ADD_SSE2(src0, src1)				\
	txmm0 = _mm_loadu_pd(buff);				\
	txmm1 = _mm_loadu_pd(buff + 2);				\
	txmm0 = _mm_add_pd(txmm0, src0);			\
	_mm_storeu_pd(buff, txmm0);				\
	txmm1 = _mm_add_pd(txmm1, src1);			\
	_mm_storeu_pd(buff + 2, txmm1);				\
	buff += 4;

/* *********************************************************** */

#define	SAVE_SHF(src)                                           \
	ss = *buff + src;                                       \
	(*dl++) = ss >> shift;                                  \
	(*buff++) = rnd_ini

/* *********************************************************** */

#define	SAVE_MUL(src)                                           \
	ss = *buff + src;                                       \
	(*dl++) = (DTYPE) (ss * d_mul);                         \
	(*buff++) = rnd_ini

#define	SAVE_MUL_SSE2(src0, src1)                               \
	txmm0 = _mm_loadu_pd(buff);				\
	txmm1 = _mm_loadu_pd(buff + 2);				\
	txmm0 = _mm_add_pd(txmm0, src0);			\
	txmm0 = _mm_mul_pd(txmm0, d_mul_xmm);			\
	ixmm0 = _mm_cvtpd_epi32(txmm0);				\
	txmm1 = _mm_add_pd(txmm1, src1);			\
	txmm1 = _mm_mul_pd(txmm1, d_mul_xmm);			\
	ixmm1 = _mm_cvtpd_epi32(txmm1);				\
	ixmm0 = _mm_unpacklo_epi64(ixmm0, ixmm1);		\
	_mm_storeu_si128((__m128i *)(dl), ixmm0);		\
	_mm_storeu_pd(buff, rnd_ini_xmm);			\
	_mm_storeu_pd(buff + 2, rnd_ini_xmm);			\
	dl += 4;						\
	buff += 4;


/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)                                 \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp0 = *((YTYPE *) (sl + x_ind[i + 0]));          \
		sp1 = *((YTYPE *) (sl + x_ind[i + 1]));          \
		sp2 = *((YTYPE *) (sl + x_ind[i + 2]));          \
		sp3 = *((YTYPE *) (sl + x_ind[i + 3]));          \
		src_xmm0 = _mm_set_pd(sp1, sp0);	 	 \
		src_xmm1 = _mm_set_pd(sp3, sp2);	 	 \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d t0, t1;					 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		t0 = _mm_unpacklo_pd(sp0, sp1);			 \
		t1 = _mm_unpackhi_pd(sp0, sp1);			 \
		src_xmm0 = _mm_add_pd(t0, t1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		t0 = _mm_unpacklo_pd(sp2, sp3);			 \
		t1 = _mm_unpackhi_pd(sp2, sp3);			 \
		src_xmm1 = _mm_add_pd(t0, t1);			 \
		SAVE##_SSE2(src_xmm0, src_xmm1);                 \
	    }                                                    \
	    for (; i < xsize; i++) {	                         \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_load_sd(&sp[2]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp5 = _mm_load_sd(&sp[2]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		t0 = _mm_unpacklo_pd(sp0, sp1);			 \
		t1 = _mm_unpackhi_pd(sp0, sp1);			 \
		src_xmm0 = _mm_add_pd(t0, t1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp6 = _mm_load_sd(&sp[2]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		sp7 = _mm_load_sd(&sp[2]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		t0 = _mm_unpacklo_pd(sp2, sp3);			 \
		t1 = _mm_unpackhi_pd(sp2, sp3);			 \
		src_xmm1 = _mm_add_pd(t0, t1);			 \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		t0 = _mm_unpacklo_pd(sp0, sp1);			 \
		t1 = _mm_unpackhi_pd(sp0, sp1);			 \
		src_xmm0 = _mm_add_pd(t0, t1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp6 = _mm_loadu_pd(&sp[2]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		sp7 = _mm_loadu_pd(&sp[2]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		t0 = _mm_unpacklo_pd(sp2, sp3);			 \
		t1 = _mm_unpackhi_pd(sp2, sp3);			 \
		src_xmm1 = _mm_add_pd(t0, t1);			 \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		SAVE##_SSE2(sp0, sp1);                 	         \
		SAVE##_SSE2(sp2, sp3);                           \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp6 = _mm_loadu_pd(&sp[2]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		sp7 = _mm_loadu_pd(&sp[2]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		tp0 = _mm_loadu_pd(&sp[4]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp0 = _mm_add_pd(sp0, tp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		tp1 = _mm_loadu_pd(&sp[4]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		sp1 = _mm_add_pd(sp1, tp1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp6 = _mm_loadu_pd(&sp[2]);			 \
		tp2 = _mm_loadu_pd(&sp[4]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp2 = _mm_add_pd(sp2, tp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		sp7 = _mm_loadu_pd(&sp[2]);			 \
		tp3 = _mm_loadu_pd(&sp[4]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		sp3 = _mm_add_pd(sp3, tp3);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d tp4, tp5, tp6, tp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		tp0 = _mm_loadu_pd(&sp[4]);			 \
		tp4 = _mm_loadu_pd(&sp[6]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp0 = _mm_add_pd(sp0, tp0);			 \
		sp0 = _mm_add_pd(sp0, tp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		tp1 = _mm_loadu_pd(&sp[4]);			 \
		tp5 = _mm_loadu_pd(&sp[6]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		sp1 = _mm_add_pd(sp1, tp1);			 \
		sp1 = _mm_add_pd(sp1, tp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp6 = _mm_loadu_pd(&sp[2]);			 \
		tp2 = _mm_loadu_pd(&sp[4]);			 \
		tp6 = _mm_loadu_pd(&sp[6]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp2 = _mm_add_pd(sp2, tp2);			 \
		sp2 = _mm_add_pd(sp2, tp6);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		sp7 = _mm_loadu_pd(&sp[2]);			 \
		tp3 = _mm_loadu_pd(&sp[4]);			 \
		tp7 = _mm_loadu_pd(&sp[6]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		sp3 = _mm_add_pd(sp3, tp3);			 \
		sp3 = _mm_add_pd(sp3, tp7);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		sp1 = _mm_unpacklo_pd(sp4, sp1);		 \
		sp2 = _mm_loadu_pd(&sp[1]);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp4 = _mm_loadu_pd(sp);				 \
		sp4 = _mm_unpacklo_pd(sp5, sp4);		 \
		sp5 = _mm_loadu_pd(&sp[1]);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
		SAVE##_SSE2(sp4, sp5);                           \
	    }                                                    \
	    for (; i < xsize; i++) { 	                         \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d tp4, tp5, tp6, tp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		tp0 = _mm_loadu_pd(&sp[3]);			 \
		sp0 = _mm_add_pd(sp0, tp0);			 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		tp4 = _mm_loadu_pd(&sp[5]);			 \
		sp4 = _mm_add_pd(sp4, tp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		tp1 = _mm_loadu_pd(&sp[3]);			 \
		sp1 = _mm_add_pd(sp1, tp1);			 \
		sp1 = _mm_unpacklo_pd(sp4, sp1);		 \
		sp2 = _mm_loadu_pd(&sp[1]);			 \
		tp2 = _mm_loadu_pd(&sp[4]);			 \
		sp2 = _mm_add_pd(sp2, tp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		tp3 = _mm_loadu_pd(&sp[3]);			 \
		sp3 = _mm_add_pd(sp3, tp3);			 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		tp5 = _mm_loadu_pd(&sp[5]);			 \
		sp5 = _mm_add_pd(sp5, tp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp4 = _mm_loadu_pd(sp);				 \
		tp4 = _mm_loadu_pd(&sp[3]);			 \
		sp4 = _mm_add_pd(sp4, tp4);			 \
		sp4 = _mm_unpacklo_pd(sp5, sp4);		 \
		sp5 = _mm_loadu_pd(&sp[1]);			 \
		tp5 = _mm_loadu_pd(&sp[4]);			 \
		sp5 = _mm_add_pd(sp5, tp5);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
		SAVE##_SSE2(sp4, sp5);                           \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d tp4, tp5, tp6, tp7;			 	 \
	    __m128d rp0, rp1, rp2, rp3;			 	 \
	    __m128d rp4, rp5, rp6, rp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		tp0 = _mm_loadu_pd(&sp[3]);			 \
		rp0 = _mm_loadu_pd(&sp[6]);			 \
		sp0 = _mm_add_pd(sp0, tp0);			 \
		sp0 = _mm_add_pd(sp0, rp0);			 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		tp4 = _mm_loadu_pd(&sp[5]);			 \
		rp4 = _mm_loadu_pd(&sp[8]);			 \
		sp4 = _mm_add_pd(sp4, tp4);			 \
		sp4 = _mm_add_pd(sp4, rp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		tp1 = _mm_loadu_pd(&sp[3]);			 \
		rp1 = _mm_loadu_pd(&sp[6]);			 \
		sp1 = _mm_add_pd(sp1, tp1);			 \
		sp1 = _mm_add_pd(sp1, rp1);			 \
		sp1 = _mm_unpacklo_pd(sp4, sp1);		 \
		sp2 = _mm_loadu_pd(&sp[1]);			 \
		tp2 = _mm_loadu_pd(&sp[4]);			 \
		rp2 = _mm_loadu_pd(&sp[7]);			 \
		sp2 = _mm_add_pd(sp2, tp2);			 \
		sp2 = _mm_add_pd(sp2, rp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		tp3 = _mm_loadu_pd(&sp[3]);			 \
		rp3 = _mm_loadu_pd(&sp[6]);			 \
		sp3 = _mm_add_pd(sp3, tp3);			 \
		sp3 = _mm_add_pd(sp3, rp3);			 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		tp5 = _mm_loadu_pd(&sp[5]);			 \
		rp5 = _mm_loadu_pd(&sp[8]);			 \
		sp5 = _mm_add_pd(sp5, tp5);			 \
		sp5 = _mm_add_pd(sp5, rp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp4 = _mm_loadu_pd(sp);				 \
		tp4 = _mm_loadu_pd(&sp[3]);			 \
		rp4 = _mm_loadu_pd(&sp[6]);			 \
		sp4 = _mm_add_pd(sp4, tp4);			 \
		sp4 = _mm_add_pd(sp4, rp4);			 \
		sp4 = _mm_unpacklo_pd(sp5, sp4);		 \
		sp5 = _mm_loadu_pd(&sp[1]);			 \
		tp5 = _mm_loadu_pd(&sp[4]);			 \
		rp5 = _mm_loadu_pd(&sp[7]);			 \
		sp5 = _mm_add_pd(sp5, tp5);			 \
		sp5 = _mm_add_pd(sp5, rp5);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
		SAVE##_SSE2(sp4, sp5);                           \
	    }                                                    \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d tp4, tp5, tp6, tp7;			 	 \
	    __m128d rp0, rp1, rp2, rp3;			 	 \
	    __m128d rp4, rp5, rp6, rp7;			 	 \
	    __m128d xp0, xp1, xp2, xp3;			 	 \
	    __m128d xp4, xp5, xp6, xp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		tp0 = _mm_loadu_pd(&sp[3]);			 \
		rp0 = _mm_loadu_pd(&sp[6]);			 \
		xp0 = _mm_loadu_pd(&sp[9]);			 \
		sp0 = _mm_add_pd(sp0, tp0);			 \
		sp0 = _mm_add_pd(sp0, rp0);			 \
		sp0 = _mm_add_pd(sp0, xp0);			 \
		sp4 = _mm_loadu_pd(&sp[2]);			 \
		tp4 = _mm_loadu_pd(&sp[5]);			 \
		rp4 = _mm_loadu_pd(&sp[8]);			 \
		xp4 = _mm_loadu_pd(&sp[11]);			 \
		sp4 = _mm_add_pd(sp4, tp4);			 \
		sp4 = _mm_add_pd(sp4, rp4);			 \
		sp4 = _mm_add_pd(sp4, xp4);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_pd(sp);				 \
		tp1 = _mm_loadu_pd(&sp[3]);			 \
		rp1 = _mm_loadu_pd(&sp[6]);			 \
		xp1 = _mm_loadu_pd(&sp[9]);			 \
		sp1 = _mm_add_pd(sp1, tp1);			 \
		sp1 = _mm_add_pd(sp1, rp1);			 \
		sp1 = _mm_add_pd(sp1, xp1);			 \
		sp1 = _mm_unpacklo_pd(sp4, sp1);		 \
		sp2 = _mm_loadu_pd(&sp[1]);			 \
		tp2 = _mm_loadu_pd(&sp[4]);			 \
		rp2 = _mm_loadu_pd(&sp[7]);			 \
		xp2 = _mm_loadu_pd(&sp[10]);			 \
		sp2 = _mm_add_pd(sp2, tp2);			 \
		sp2 = _mm_add_pd(sp2, rp2);			 \
		sp2 = _mm_add_pd(sp2, xp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp3 = _mm_loadu_pd(sp);				 \
		tp3 = _mm_loadu_pd(&sp[3]);			 \
		rp3 = _mm_loadu_pd(&sp[6]);			 \
		xp3 = _mm_loadu_pd(&sp[9]);			 \
		sp3 = _mm_add_pd(sp3, tp3);			 \
		sp3 = _mm_add_pd(sp3, rp3);			 \
		sp3 = _mm_add_pd(sp3, xp3);			 \
		sp5 = _mm_loadu_pd(&sp[2]);			 \
		tp5 = _mm_loadu_pd(&sp[5]);			 \
		rp5 = _mm_loadu_pd(&sp[8]);			 \
		xp5 = _mm_loadu_pd(&sp[11]);			 \
		sp5 = _mm_add_pd(sp5, tp5);			 \
		sp5 = _mm_add_pd(sp5, rp5);			 \
		sp5 = _mm_add_pd(sp5, xp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp4 = _mm_loadu_pd(sp);				 \
		tp4 = _mm_loadu_pd(&sp[3]);			 \
		rp4 = _mm_loadu_pd(&sp[6]);			 \
		xp4 = _mm_loadu_pd(&sp[9]);			 \
		sp4 = _mm_add_pd(sp4, tp4);			 \
		sp4 = _mm_add_pd(sp4, rp4);			 \
		sp4 = _mm_add_pd(sp4, xp4);			 \
		sp4 = _mm_unpacklo_pd(sp5, sp4);		 \
		sp5 = _mm_loadu_pd(&sp[1]);			 \
		tp5 = _mm_loadu_pd(&sp[4]);			 \
		rp5 = _mm_loadu_pd(&sp[7]);			 \
		xp5 = _mm_loadu_pd(&sp[10]);			 \
		sp5 = _mm_add_pd(sp5, tp5);			 \
		sp5 = _mm_add_pd(sp5, rp5);			 \
		sp5 = _mm_add_pd(sp5, xp5);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
		SAVE##_SSE2(sp4, sp5);                           \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp1 = _mm_loadu_pd(&sp[2]);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp3 = _mm_loadu_pd(&sp[2]);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
	    }                                                    \
	    for (; i < xsize; i++) {                             \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp1 = _mm_loadu_pd(&sp[2]);			 \
		sp4 = _mm_loadu_pd(&sp[4]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp5 = _mm_loadu_pd(&sp[6]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp3 = _mm_loadu_pd(&sp[2]);			 \
		sp6 = _mm_loadu_pd(&sp[4]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp7 = _mm_loadu_pd(&sp[6]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d tp4, tp5, tp6, tp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp1 = _mm_loadu_pd(&sp[2]);			 \
		sp4 = _mm_loadu_pd(&sp[4]);			 \
		tp4 = _mm_loadu_pd(&sp[8]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp0 = _mm_add_pd(sp0, tp4);			 \
		sp5 = _mm_loadu_pd(&sp[6]);			 \
		tp5 = _mm_loadu_pd(&sp[10]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		sp1 = _mm_add_pd(sp1, tp5);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp3 = _mm_loadu_pd(&sp[2]);			 \
		sp6 = _mm_loadu_pd(&sp[4]);			 \
		tp6 = _mm_loadu_pd(&sp[8]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp2 = _mm_add_pd(sp2, tp6);			 \
		sp7 = _mm_loadu_pd(&sp[6]);			 \
		tp7 = _mm_loadu_pd(&sp[10]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		sp3 = _mm_add_pd(sp3, tp7);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
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
	    __m128d sp0, sp1, sp2, sp3;			 	 \
	    __m128d sp4, sp5, sp6, sp7;			 	 \
	    __m128d tp0, tp1, tp2, tp3;			 	 \
	    __m128d tp4, tp5, tp6, tp7;			 	 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_pd(sp);				 \
		sp1 = _mm_loadu_pd(&sp[2]);			 \
		sp4 = _mm_loadu_pd(&sp[4]);			 \
		tp4 = _mm_loadu_pd(&sp[8]);			 \
		tp0 = _mm_loadu_pd(&sp[12]);			 \
		sp0 = _mm_add_pd(sp0, sp4);			 \
		sp0 = _mm_add_pd(sp0, tp4);			 \
		sp0 = _mm_add_pd(sp0, tp0);			 \
		sp5 = _mm_loadu_pd(&sp[6]);			 \
		tp5 = _mm_loadu_pd(&sp[10]);			 \
		tp1 = _mm_loadu_pd(&sp[14]);			 \
		sp1 = _mm_add_pd(sp1, sp5);			 \
		sp1 = _mm_add_pd(sp1, tp5);			 \
		sp1 = _mm_add_pd(sp1, tp1);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = _mm_loadu_pd(sp);				 \
		sp3 = _mm_loadu_pd(&sp[2]);			 \
		sp6 = _mm_loadu_pd(&sp[4]);			 \
		tp6 = _mm_loadu_pd(&sp[8]);			 \
		tp2 = _mm_loadu_pd(&sp[12]);			 \
		sp2 = _mm_add_pd(sp2, sp6);			 \
		sp2 = _mm_add_pd(sp2, tp6);			 \
		sp2 = _mm_add_pd(sp2, tp2);			 \
		sp7 = _mm_loadu_pd(&sp[6]);			 \
		tp7 = _mm_loadu_pd(&sp[10]);			 \
		tp3 = _mm_loadu_pd(&sp[14]);			 \
		sp3 = _mm_add_pd(sp3, sp7);			 \
		sp3 = _mm_add_pd(sp3, tp7);			 \
		sp3 = _mm_add_pd(sp3, tp3);			 \
		SAVE##_SSE2(sp0, sp1);                           \
		SAVE##_SSE2(sp2, sp3);                           \
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
