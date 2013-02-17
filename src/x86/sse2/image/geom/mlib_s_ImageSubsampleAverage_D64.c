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

#pragma ident	"@(#)mlib_s_ImageSubsampleAverage_D64.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_D64_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_d64
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

static void FUNC(
    y1i) (
    YTYPE * buff,
    DTYPE * sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	mlib_s32 i;
	__m128d t0, t1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize - 1; i += 2) {
		t0 = _mm_loadu_pd((sp + i));
		_mm_storeu_pd((buff + i), t0);
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
	    DTYPE *sp1 = sp + slb;				          \
	    __m128d t0, t1, t2, t3;	                                  \
	    for (i = 0; i < xsize - 1; i += 2) {                          \
		    t0 = _mm_loadu_pd(&sp[i]);			          \
		    t1 = _mm_loadu_pd(&sp1[i]);			          \
		    ADD##_SSE2(t0, t1);				          \
	    }							          \
	    for (; i < xsize; i++) {                                      \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i]);                     \
	    }                       		                          \
	}                                                                 \
	                                                                  \
/* *********************************************************** */	  \
	static void FUNC(y3##SUFF) (YTYPE * buff, DTYPE * sp,             \
	    mlib_s32 slb, mlib_s32 xsize)                                 \
	{                                                                 \
	    mlib_s32 i;                                                   \
	    DTYPE *sp1 = sp + slb;                                        \
	    DTYPE *sp2 = sp + 2 * slb;                                    \
	    __m128d t0, t1, t2, t3;	                                  \
	    for (i = 0; i < xsize - 1; i += 2) {                          \
		    t0 = _mm_loadu_pd(&sp[i]);			          \
		    t1 = _mm_loadu_pd(&sp1[i]);			          \
		    t2 = _mm_loadu_pd(&sp2[i]);			          \
		    ADD##_C3_SSE2(t0, t1, t2);			          \
	    }							          \
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
	    __m128d t0, t1, t2, t3;	                                  \
	    for (i = 0; i < xsize - 1; i += 2) {                          \
		    t0 = _mm_loadu_pd(&sp[i]);			          \
		    t1 = _mm_loadu_pd(&sp1[i]);			          \
		    t2 = _mm_loadu_pd(&sp2[i]);			          \
		    t3 = _mm_loadu_pd(&sp3[i]);			          \
		    ADD##_C4_SSE2(t0, t1, t2, t3);		          \
	    }							          \
	    for (; i < xsize; i++) {                                      \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i] + sp2[i] + sp3[i]);   \
	    }                                                             \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src

#define	STORE_SSE2(src0, src1)		\
	__m128d tmp = _mm_add_pd(src0, src1);\
	_mm_storeu_pd(&buff[i], tmp);

#define	STORE_C3_SSE2(src0, src1, src2)		\
	__m128d tmp = _mm_add_pd(src0, src1);   \
	tmp = _mm_add_pd(tmp, src2);		\
	_mm_storeu_pd(&buff[i], tmp);

#define	STORE_C4_SSE2(src0, src1, src2, src3)	\
	__m128d tmp = _mm_add_pd(src0, src1);   \
	tmp = _mm_add_pd(tmp, src2);		\
	tmp = _mm_add_pd(tmp, src3);		\
	_mm_storeu_pd(&buff[i], tmp);

#define	ADD(dst, src)	dst = dst + src

#define	ADD_SSE2(src0, src1)			\
	__m128d dst = _mm_loadu_pd(&buff[i]);	\
	dst = _mm_add_pd(dst, src0);		\
	dst = _mm_add_pd(dst, src1);		\
	_mm_storeu_pd(&buff[i], dst);

#define	ADD_C3_SSE2(src0, src1, src2)		\
	__m128d dst = _mm_loadu_pd(&buff[i]);	\
	dst = _mm_add_pd(dst, src0);		\
	dst = _mm_add_pd(dst, src1);		\
	dst = _mm_add_pd(dst, src2);		\
	_mm_storeu_pd(&buff[i], dst);

#define	ADD_C4_SSE2(src0, src1, src2, src3)	\
	__m128d dst = _mm_loadu_pd(&buff[i]);	\
	dst = _mm_add_pd(dst, src0);		\
	dst = _mm_add_pd(dst, src1);		\
	dst = _mm_add_pd(dst, src2);		\
	dst = _mm_add_pd(dst, src3);		\
	_mm_storeu_pd(&buff[i], dst);

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
	__m128d dst_xmm;

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
	__m128d rnd_ini_xmm = _mm_set1_pd(rnd_ini);		\
	__m128d d_mul_xmm = _mm_set1_pd(d_mul);

/* *********************************************************** */

#define	SAVE_ADD(src)	ss = *buff + src; (*buff++) = ss

#define	SAVE_ADD_SSE2(src)					\
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src);			\
	_mm_storeu_pd(buff, dst_xmm);				\
	buff += 2;


#define	SAVE_ADD_SSE2_x2(src0, src1)				\
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src0);			\
	dst_xmm = _mm_add_pd(dst_xmm, src1);			\
	_mm_storeu_pd(buff, dst_xmm);				\
	buff += 2;

#define	SAVE_ADD_SSE2_x3(src0, src1, src2)			\
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src0);			\
	dst_xmm = _mm_add_pd(dst_xmm, src1);			\
	dst_xmm = _mm_add_pd(dst_xmm, src2);			\
	_mm_storeu_pd(buff, dst_xmm);				\
	buff += 2;

#define	SAVE_ADD_SSE2_x4(src0, src1, src2, src3)		\
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src0);			\
	dst_xmm = _mm_add_pd(dst_xmm, src1);			\
	dst_xmm = _mm_add_pd(dst_xmm, src2);			\
	dst_xmm = _mm_add_pd(dst_xmm, src3);			\
	_mm_storeu_pd(buff, dst_xmm);				\
	buff += 2;
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

#define	SAVE_MUL_SSE2(src)                                      \
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src);			\
	dst_xmm = _mm_mul_pd(dst_xmm, d_mul_xmm);		\
	_mm_storeu_pd(dl, dst_xmm);				\
	_mm_storeu_pd(buff, rnd_ini_xmm);			\
	buff += 2;						\
	dl += 2;

#define	SAVE_MUL_SSE2_x2(src0, src1)                            \
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src0);			\
	dst_xmm = _mm_add_pd(dst_xmm, src1);			\
	dst_xmm = _mm_mul_pd(dst_xmm, d_mul_xmm);		\
	_mm_storeu_pd(dl, dst_xmm);				\
	_mm_storeu_pd(buff, rnd_ini_xmm);			\
	buff += 2;						\
	dl += 2;

#define	SAVE_MUL_SSE2_x3(src0, src1, src2)                      \
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src0);			\
	dst_xmm = _mm_add_pd(dst_xmm, src1);	 		\
	dst_xmm = _mm_add_pd(dst_xmm, src2);	 		\
	dst_xmm = _mm_mul_pd(dst_xmm, d_mul_xmm);		\
	_mm_storeu_pd(dl, dst_xmm);				\
	_mm_storeu_pd(buff, rnd_ini_xmm);			\
	buff += 2;						\
	dl += 2;

#define	SAVE_MUL_SSE2_x4(src0, src1, src2, src3)                \
	dst_xmm = _mm_loadu_pd(buff);				\
	dst_xmm = _mm_add_pd(dst_xmm, src0);			\
	dst_xmm = _mm_add_pd(dst_xmm, src1);	 		\
	dst_xmm = _mm_add_pd(dst_xmm, src2);	 		\
	dst_xmm = _mm_add_pd(dst_xmm, src3);	 		\
	dst_xmm = _mm_mul_pd(dst_xmm, d_mul_xmm);		\
	_mm_storeu_pd(dl, dst_xmm);				\
	_mm_storeu_pd(buff, rnd_ini_xmm);			\
	buff += 2;						\
	dl += 2;
/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)                                 \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm;					 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0];					 \
		src_xmm = _mm_set_pd(sp1, sp0);			 \
		SAVE##_SSE2(src_xmm);				 \
	    }							 \
	    for (; i < xsize; i++) {	                         \
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
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0];					 \
		sp1 = sp[1];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = sp[0];					 \
		sp3 = sp[1];					 \
		src_xmm0 = _mm_set_pd(sp2, sp0);		 \
		src_xmm1 = _mm_set_pd(sp3, sp1);		 \
		SAVE##_SSE2_x2(src_xmm0, src_xmm1);		 \
	    }							 \
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
	    mlib_d64 sp0, sp1, sp2, sp3, sp4, sp5;		 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d src_xmm0, src_xmm1, src_xmm2;		 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0];					 \
		sp1 = sp[1];					 \
		sp4 = sp[2];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = sp[0];					 \
		sp3 = sp[1];					 \
		sp5 = sp[2];					 \
		src_xmm0 = _mm_set_pd(sp2, sp0);		 \
		src_xmm1 = _mm_set_pd(sp3, sp1);		 \
		src_xmm2 = _mm_set_pd(sp5, sp4);		 \
		SAVE##_SSE2_x3(src_xmm0, src_xmm1, src_xmm2);	 \
	    }							 \
	    for (; i < xsize; i++) {	                         \
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
	    mlib_d64 sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;	 \
	    __m128d src_xmm0, src_xmm1, src_xmm2, src_xmm3;	 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0];					 \
		sp1 = sp[1];					 \
		sp4 = sp[2];					 \
		sp6 = sp[3];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp2 = sp[0];					 \
		sp3 = sp[1];					 \
		sp5 = sp[2];					 \
		sp7 = sp[3];					 \
		src_xmm0 = _mm_set_pd(sp2, sp0);		 \
		src_xmm1 = _mm_set_pd(sp3, sp1);		 \
		src_xmm2 = _mm_set_pd(sp5, sp4);		 \
		src_xmm3 = _mm_set_pd(sp7, sp6);		 \
		SAVE##_SSE2_x4(src_xmm0, src_xmm1, src_xmm2, src_xmm3);\
	    }							 \
	    for (; i < xsize; i++) {	                         \
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
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm0;					 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		SAVE##_SSE2(src_xmm0);				 \
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x2##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		SAVE##_SSE2_x2(src_xmm0, src_xmm1);		 \
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x3##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm0, src_xmm1, src_xmm2;		 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		src_xmm2 = _mm_loadu_pd(&sp[4]);		 \
		SAVE##_SSE2_x3(src_xmm0, src_xmm1, src_xmm2);	 \
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch2_x4##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128d src_xmm0, src_xmm1, src_xmm2, src_xmm3;	 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		src_xmm2 = _mm_loadu_pd(&sp[4]);		 \
		src_xmm3 = _mm_loadu_pd(&sp[6]);		 \
		SAVE##_SSE2_x4(src_xmm0, src_xmm1, src_xmm2, src_xmm3);\
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch3_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm0, src_xmm1, src_xmm2;		 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		sp0 = sp[2];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0];					 \
		src_xmm1 = _mm_set_pd(sp1, sp0);		 \
		src_xmm2 = _mm_loadu_pd(&sp[1]);		 \
		SAVE##_SSE2(src_xmm0);			 	 \
		SAVE##_SSE2(src_xmm1);			 	 \
		SAVE##_SSE2(src_xmm2);			 	 \
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
	    mlib_d64 sp0, sp1, sp2, sp3;			 \
	    __m128d src_xmm0, src_xmm1, src_xmm2;		 \
	    __m128d t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		t0 = _mm_loadu_pd(&sp[3]);			 \
		sp0 = sp[2];					 \
		sp2 = sp[5];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0];					 \
		sp3 = sp[3];					 \
		src_xmm1 = _mm_set_pd(sp1, sp0);		 \
		t1 = _mm_set_pd(sp3, sp2);			 \
		src_xmm2 = _mm_loadu_pd(&sp[1]);		 \
		t2 = _mm_loadu_pd(&sp[4]);			 \
		SAVE##_SSE2_x2(src_xmm0, t0);		 	 \
		SAVE##_SSE2_x2(src_xmm1, t1);		 	 \
		SAVE##_SSE2_x2(src_xmm2, t2);		 	 \
	    }							 \
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
	    mlib_d64 sp0, sp1, sp2, sp3, sp4, sp5;		 \
	    __m128d src_xmm0, src_xmm1, src_xmm2;		 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d r0, r1, r2, r3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		t0 = _mm_loadu_pd(&sp[3]);			 \
		r0 = _mm_loadu_pd(&sp[6]);			 \
		sp0 = sp[2];					 \
		sp2 = sp[5];					 \
		sp4 = sp[8];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0];					 \
		sp3 = sp[3];					 \
		sp5 = sp[6];					 \
		src_xmm1 = _mm_set_pd(sp1, sp0);		 \
		t1 = _mm_set_pd(sp3, sp2);			 \
		r1 = _mm_set_pd(sp5, sp4);			 \
		src_xmm2 = _mm_loadu_pd(&sp[1]);		 \
		t2 = _mm_loadu_pd(&sp[4]);			 \
		r2 = _mm_loadu_pd(&sp[7]);			 \
		SAVE##_SSE2_x3(src_xmm0, t0, r0);	 	 \
		SAVE##_SSE2_x3(src_xmm1, t1, r1);	 	 \
		SAVE##_SSE2_x3(src_xmm2, t2, r2);	 	 \
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
	    mlib_d64 sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;	 \
	    __m128d src_xmm0, src_xmm1, src_xmm2;		 \
	    __m128d t0, t1, t2, t3;				 \
	    __m128d r0, r1, r2, r3;				 \
	    __m128d x0, x1, x2, x3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		t0 = _mm_loadu_pd(&sp[3]);			 \
		r0 = _mm_loadu_pd(&sp[6]);			 \
		x0 = _mm_loadu_pd(&sp[9]);			 \
		sp0 = sp[2];					 \
		sp2 = sp[5];					 \
		sp4 = sp[8];					 \
		sp6 = sp[11];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0];					 \
		sp3 = sp[3];					 \
		sp5 = sp[6];					 \
		sp7 = sp[9];					 \
		src_xmm1 = _mm_set_pd(sp1, sp0);		 \
		t1 = _mm_set_pd(sp3, sp2);			 \
		r1 = _mm_set_pd(sp5, sp4);			 \
		x1 = _mm_set_pd(sp7, sp6);			 \
		src_xmm2 = _mm_loadu_pd(&sp[1]);		 \
		t2 = _mm_loadu_pd(&sp[4]);			 \
		r2 = _mm_loadu_pd(&sp[7]);			 \
		x2 = _mm_loadu_pd(&sp[10]);			 \
		SAVE##_SSE2_x4(src_xmm0, t0, r0, x0);	 	 \
		SAVE##_SSE2_x4(src_xmm1, t1, r1, x1);	 	 \
		SAVE##_SSE2_x4(src_xmm2, t2, r2, x2);	 	 \
	    }							 \
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
	    __m128d src_xmm0, src_xmm1;				 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		SAVE##_SSE2(src_xmm0);				 \
		SAVE##_SSE2(src_xmm1);				 \
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x2##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128d src_xmm0, src_xmm1;				 \
	    __m128d t0, t1;					 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		t0 = _mm_loadu_pd(&sp[4]);	  	 	 \
		t1 = _mm_loadu_pd(&sp[6]);	  	 	 \
		SAVE##_SSE2_x2(src_xmm0, t0);			 \
		SAVE##_SSE2_x2(src_xmm1, t1);			 \
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x3##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128d src_xmm0, src_xmm1;				 \
	    __m128d t0, t1;					 \
	    __m128d r0, r1;					 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		t0 = _mm_loadu_pd(&sp[4]);	  	 	 \
		t1 = _mm_loadu_pd(&sp[6]);	  	 	 \
		r0 = _mm_loadu_pd(&sp[8]);	  	 	 \
		r1 = _mm_loadu_pd(&sp[10]);	  	 	 \
		SAVE##_SSE2_x3(src_xmm0, t0, r0);		 \
		SAVE##_SSE2_x3(src_xmm1, t1, r1);		 \
	    }							 \
	}                                                        \
	                                                         \
/* ********************************************************** */ \
	static void FUNC(ch4_x4##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    __m128d src_xmm0, src_xmm1;				 \
	    __m128d t0, t1;					 \
	    __m128d r0, r1;					 \
	    __m128d x0, x1;					 \
	    for (i = 0; i < xsize; i++) {                        \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		src_xmm0 = _mm_loadu_pd(sp);			 \
		src_xmm1 = _mm_loadu_pd(&sp[2]);		 \
		t0 = _mm_loadu_pd(&sp[4]);	  	 	 \
		t1 = _mm_loadu_pd(&sp[6]);	  	 	 \
		r0 = _mm_loadu_pd(&sp[8]);	  	 	 \
		r1 = _mm_loadu_pd(&sp[10]);	  	 	 \
		x0 = _mm_loadu_pd(&sp[12]);	  	 	 \
		x1 = _mm_loadu_pd(&sp[14]);	  	 	 \
		SAVE##_SSE2_x4(src_xmm0, t0, r0, x0);		 \
		SAVE##_SSE2_x4(src_xmm1, t1, r1, x1);		 \
	    }							 \
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
