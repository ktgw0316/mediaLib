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

#pragma ident	"@(#)mlib_s_ImageSubsampleAverage_F32.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_F32_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_f32
#define	YTYPE	mlib_f32

#define	XTYPE	YTYPE


/* #define USE_SHIFT */

#include <mlib_ImageSubsampleAverage.h>
#include <mlib_sse_utils.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	_MM_SHUFFLE_1(z, y, x, w)		 \
	((z << 6) | (y << 4) | (x << 2) | w)

/* *********************************************************** */


static void FUNC(
    y1i) (
    YTYPE * buff,
    DTYPE * sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	mlib_s32 i;
	__m128 t0, t1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize - 3; i += 4) {
		t0 = _mm_loadu_ps((sp + i));
		_mm_storeu_ps((buff + i), t0);
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
		__m128 txmm0, txmm1;				          \
		for (i = 0; i < xsize - 3; i += 4) {	                  \
		    txmm0 = _mm_loadu_ps(&sp[i]);		          \
		    txmm1 = _mm_loadu_ps(&sp1[i]);		          \
		    txmm0 = _mm_add_ps(txmm0, txmm1);		          \
		    ADD##_SSE2(txmm0);				          \
	    }		    					          \
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
	                                                                  \
		__m128 txmm0, txmm1, txmm2;			          \
		for (i = 0; i < xsize - 3; i += 4) {	                  \
		    txmm0 = _mm_loadu_ps(&sp[i]);		          \
		    txmm1 = _mm_loadu_ps(&sp1[i]);		          \
		    txmm2 = _mm_loadu_ps(&sp2[i]);		          \
		    txmm0 = _mm_add_ps(txmm0, txmm1);		          \
		    txmm0 = _mm_add_ps(txmm0, txmm2);		          \
		    ADD##_SSE2(txmm0);				          \
	    }		    					          \
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
		__m128 txmm0, txmm1, txmm2, txmm3;		          \
		for (i = 0; i < xsize - 3; i += 4) {	                  \
		    txmm0 = _mm_loadu_ps(&sp[i]);		          \
		    txmm1 = _mm_loadu_ps(&sp1[i]);		          \
		    txmm2 = _mm_loadu_ps(&sp2[i]);		          \
		    txmm3 = _mm_loadu_ps(&sp3[i]);		          \
		    txmm0 = _mm_add_ps(txmm0, txmm1);		          \
		    txmm0 = _mm_add_ps(txmm0, txmm2);		          \
		    txmm0 = _mm_add_ps(txmm0, txmm3);		          \
		    ADD##_SSE2(txmm0);				          \
	    }		                                                  \
	    for (; i < xsize; i++) {                                      \
		ADD(buff[i], (YTYPE) sp[i] + sp1[i] + sp2[i] + sp3[i]);   \
	    }                                                             \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src

#define	STORE_SSE2(src)				\
	_mm_storeu_ps((buff + i), src);

#define	ADD(dst, src)	dst = dst + src

#define	ADD_SSE2(src)				\
	__m128 dst = _mm_loadu_ps((buff + i));	\
	dst = _mm_add_ps(dst, src);		\
	_mm_storeu_ps((buff + i), dst);

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
	__m128 txmm0, txmm1, txmm2, txmm3;			\
	__m128 dst_xmm;

/* *********************************************************** */

#define	INIT_SHF                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_s32 shift = ws->shift;

/* *********************************************************** */

#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_d64 d_mul = ws->d_mul;				\
	__m128 rnd_ini_xmm = _mm_set1_ps(rnd_ini);		\
	__m128 d_mul_xmm = _mm_set1_ps((mlib_f32)d_mul);

/* *********************************************************** */

#define	SAVE_ADD(src)	ss = *buff + src; (*buff++) = ss

#define	SAVE_ADD_SSE2(src)					\
	dst_xmm = _mm_loadu_ps(buff);				\
	dst_xmm = _mm_add_ps(dst_xmm, src);			\
	_mm_storeu_ps(buff, dst_xmm);				\
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

#define	SAVE_MUL_SSE2(src)                                      \
	dst_xmm = _mm_loadu_ps(buff);				\
	dst_xmm = _mm_add_ps(dst_xmm, src);			\
	dst_xmm = _mm_mul_ps(dst_xmm, d_mul_xmm);		\
	_mm_storeu_ps(dl, dst_xmm);				\
	_mm_storeu_ps(buff, rnd_ini_xmm);			\
	buff += 4;						\
	dl += 4;


/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)                                 \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    mlib_f32 sp0, sp1, sp2, sp3;			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0];					 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0];					 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = sp[0];					 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = sp[0];					 \
		src_xmm = _mm_set_ps(sp3, sp2, sp1, sp0);	 \
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
	    mlib_f32 sp0, sp1, sp2, sp3;			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0] + sp[1];				 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0] + sp[1];				 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = sp[0] + sp[1];				 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = sp[0] + sp[1];				 \
		src_xmm = _mm_set_ps(sp3, sp2, sp1, sp0);	 \
		SAVE##_SSE2(src_xmm);				 \
	    }							 \
	    for (; i < xsize; i++) {                             \
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
	    mlib_f32 sp0, sp1, sp2, sp3;			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0] + sp[1] + sp[2];			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0] + sp[1] + sp[2];			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = sp[0] + sp[1] + sp[2];			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = sp[0] + sp[1] + sp[2];			 \
		src_xmm = _mm_set_ps(sp3, sp2, sp1, sp0);	 \
		SAVE##_SSE2(src_xmm);				 \
	    }							 \
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
	    mlib_f32 sp0, sp1, sp2, sp3;			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = sp[0] + sp[1] + sp[2] + sp[3];		 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = sp[0] + sp[1] + sp[2] + sp[3];		 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = sp[0] + sp[1] + sp[2] + sp[3];		 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = sp[0] + sp[1] + sp[2] + sp[3];		 \
		src_xmm = _mm_set_ps(sp3, sp2, sp1, sp0);	 \
		SAVE##_SSE2(src_xmm);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		sp0 = _mm_movelh_ps(sp0, sp1);		         \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		sp2 = _mm_movelh_ps(sp2, sp3);		         \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp2);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 tp0, tp1, tp2, tp3;	 			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		tp0 = _mm_movehl_ps(tp0, sp0);			 \
		sp0 = _mm_add_ps(sp0, tp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		tp1 = _mm_movehl_ps(tp1, sp1);			 \
		sp1 = _mm_add_ps(sp1, tp1);			 \
		sp0 = _mm_movelh_ps(sp0, sp1);		         \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		tp2 = _mm_movehl_ps(tp2, sp2);			 \
		sp2 = _mm_add_ps(sp2, tp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		tp3 = _mm_movehl_ps(tp3, sp3);			 \
		sp3 = _mm_add_ps(sp3, tp3);			 \
		sp2 = _mm_movelh_ps(sp2, sp3);		         \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp2);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 tp0, tp1, tp2, tp3;	 			 \
	    __m128 rp0, rp1, rp2, rp3;	 			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		tp0 = _mm_movehl_ps(tp0, sp0);			 \
		rp0 = _mm_loadu_ps(&sp[4]);			 \
		sp0 = _mm_add_ps(sp0, tp0);			 \
		sp0 = _mm_add_ps(sp0, rp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		tp1 = _mm_movehl_ps(tp1, sp1);			 \
		rp1 = _mm_loadu_ps(&sp[4]);			 \
		sp1 = _mm_add_ps(sp1, tp1);			 \
		sp1 = _mm_add_ps(sp1, rp1);			 \
		sp0 = _mm_movelh_ps(sp0, sp1);		         \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		tp2 = _mm_movehl_ps(tp2, sp2);			 \
		rp2 = _mm_loadu_ps(&sp[4]);			 \
		sp2 = _mm_add_ps(sp2, tp2);			 \
		sp2 = _mm_add_ps(sp2, rp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		tp3 = _mm_movehl_ps(tp3, sp3);			 \
		rp3 = _mm_loadu_ps(&sp[4]);			 \
		sp3 = _mm_add_ps(sp3, tp3);			 \
		sp3 = _mm_add_ps(sp3, rp3);			 \
		sp2 = _mm_movelh_ps(sp2, sp3);		         \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp2);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 tp0, tp1, tp2, tp3;	 			 \
	    __m128 rp0, rp1, rp2, rp3;	 			 \
	    __m128 xp0, xp1, xp2, xp3;	 			 \
	    __m128 src_xmm;					 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		tp0 = _mm_movehl_ps(tp0, sp0);			 \
		rp0 = _mm_loadu_ps(&sp[4]);			 \
		xp0 = _mm_movehl_ps(xp0, rp0);			 \
		sp0 = _mm_add_ps(sp0, tp0);			 \
		sp0 = _mm_add_ps(sp0, rp0);			 \
		sp0 = _mm_add_ps(sp0, xp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		tp1 = _mm_movehl_ps(tp1, sp1);			 \
		rp1 = _mm_loadu_ps(&sp[4]);			 \
		xp1 = _mm_movehl_ps(xp1, rp1);			 \
		sp1 = _mm_add_ps(sp1, tp1);			 \
		sp1 = _mm_add_ps(sp1, rp1);			 \
		sp1 = _mm_add_ps(sp1, xp1);			 \
		sp0 = _mm_movelh_ps(sp0, sp1);		         \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		tp2 = _mm_movehl_ps(tp2, sp2);			 \
		rp2 = _mm_loadu_ps(&sp[4]);			 \
		xp2 = _mm_movehl_ps(xp2, rp2);			 \
		sp2 = _mm_add_ps(sp2, tp2);			 \
		sp2 = _mm_add_ps(sp2, rp2);			 \
		sp2 = _mm_add_ps(sp2, xp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		tp3 = _mm_movehl_ps(tp3, sp3);			 \
		rp3 = _mm_loadu_ps(&sp[4]);			 \
		xp3 = _mm_movehl_ps(xp3, rp3);			 \
		sp3 = _mm_add_ps(sp3, tp3);			 \
		sp3 = _mm_add_ps(sp3, rp3);			 \
		sp3 = _mm_add_ps(sp3, xp3);			 \
		sp2 = _mm_movelh_ps(sp2, sp3);		         \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp2);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t0 = _mm_movehl_ps(t0, sp0);			 \
		sp1 = _mm_loadu_ps(sp);				 \
		t0 = _mm_unpacklo_ps(t0, sp1);			 \
		sp0 = _mm_movelh_ps(sp0, t0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		sp1 = _mm_shuffle_ps(sp1, sp2, 			 \
				_MM_SHUFFLE_1(1, 0, 2, 1));	 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		t1 = _mm_movehl_ps(t1, sp2);			 \
		t1 = _mm_unpacklo_ps(t1, sp3);			 \
		sp2 = _mm_shuffle_ps(t1, sp3, 			 \
				_MM_SHUFFLE_1(2, 1, 1, 0));	 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
		SAVE##_SSE2(sp2);				 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 tp0, tp1, tp2, tp3;	 			 \
	    __m128 t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		tp0 = _mm_loadu_ps(&sp[3]);			 \
		sp0 = _mm_add_ps(sp0, tp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t0 = _mm_movehl_ps(t0, sp0);			 \
		sp1 = _mm_loadu_ps(sp);				 \
		tp1 = _mm_loadu_ps(&sp[3]);			 \
		sp1 = _mm_add_ps(sp1, tp1);			 \
		t0 = _mm_unpacklo_ps(t0, sp1);			 \
		sp0 = _mm_movelh_ps(sp0, t0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		tp2 = _mm_loadu_ps(&sp[3]);			 \
		sp2 = _mm_add_ps(sp2, tp2);			 \
		sp1 = _mm_shuffle_ps(sp1, sp2, 			 \
				_MM_SHUFFLE_1(1, 0, 2, 1));	 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		tp3 = _mm_loadu_ps(&sp[3]);			 \
		sp3 = _mm_add_ps(sp3, tp3);			 \
		t1 = _mm_movehl_ps(t1, sp2);			 \
		t1 = _mm_unpacklo_ps(t1, sp3);			 \
		sp2 = _mm_shuffle_ps(t1, sp3, 			 \
				_MM_SHUFFLE_1(2, 1, 1, 0));	 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
		SAVE##_SSE2(sp2);				 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 tp0, tp1, tp2, tp3;	 			 \
	    __m128 rp0, rp1, rp2, rp3;	 			 \
	    __m128 t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		tp0 = _mm_loadu_ps(&sp[3]);			 \
		rp0 = _mm_loadu_ps(&sp[6]);			 \
		sp0 = _mm_add_ps(sp0, rp0);			 \
		sp0 = _mm_add_ps(sp0, tp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t0 = _mm_movehl_ps(t0, sp0);			 \
		sp1 = _mm_loadu_ps(sp);				 \
		tp1 = _mm_loadu_ps(&sp[3]);			 \
		rp1 = _mm_loadu_ps(&sp[6]);			 \
		sp1 = _mm_add_ps(sp1, rp1);			 \
		sp1 = _mm_add_ps(sp1, tp1);			 \
		t0 = _mm_unpacklo_ps(t0, sp1);			 \
		sp0 = _mm_movelh_ps(sp0, t0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		tp2 = _mm_loadu_ps(&sp[3]);			 \
		rp2 = _mm_loadu_ps(&sp[6]);			 \
		sp2 = _mm_add_ps(sp2, rp2);			 \
		sp2 = _mm_add_ps(sp2, tp2);			 \
		sp1 = _mm_shuffle_ps(sp1, sp2, 			 \
				_MM_SHUFFLE_1(1, 0, 2, 1));	 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		tp3 = _mm_loadu_ps(&sp[3]);			 \
		rp3 = _mm_loadu_ps(&sp[6]);			 \
		sp3 = _mm_add_ps(sp3, rp3);			 \
		sp3 = _mm_add_ps(sp3, tp3);			 \
		t1 = _mm_movehl_ps(t1, sp2);			 \
		t1 = _mm_unpacklo_ps(t1, sp3);			 \
		sp2 = _mm_shuffle_ps(t1, sp3, 			 \
				_MM_SHUFFLE_1(2, 1, 1, 0));	 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
		SAVE##_SSE2(sp2);				 \
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
	    __m128 sp0, sp1, sp2, sp3;	 			 \
	    __m128 tp0, tp1, tp2, tp3;	 			 \
	    __m128 rp0, rp1, rp2, rp3;	 			 \
	    __m128 xp0, xp1, xp2, xp3;	 			 \
	    __m128 t0, t1, t2, t3;				 \
	    for (i = 0; i < xsize - 3; i += 4) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		tp0 = _mm_loadu_ps(&sp[3]);			 \
		rp0 = _mm_loadu_ps(&sp[6]);			 \
		xp0 = _mm_loadu_ps(&sp[9]);			 \
		sp0 = _mm_add_ps(sp0, xp0);			 \
		sp0 = _mm_add_ps(sp0, rp0);			 \
		sp0 = _mm_add_ps(sp0, tp0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		t0 = _mm_movehl_ps(t0, sp0);			 \
		sp1 = _mm_loadu_ps(sp);				 \
		tp1 = _mm_loadu_ps(&sp[3]);			 \
		rp1 = _mm_loadu_ps(&sp[6]);			 \
		xp1 = _mm_loadu_ps(&sp[9]);			 \
		sp1 = _mm_add_ps(sp1, xp1);			 \
		sp1 = _mm_add_ps(sp1, rp1);			 \
		sp1 = _mm_add_ps(sp1, tp1);			 \
		t0 = _mm_unpacklo_ps(t0, sp1);			 \
		sp0 = _mm_movelh_ps(sp0, t0);			 \
		sp = (YTYPE *) (sl + x_ind[i + 2]);              \
		sp2 = _mm_loadu_ps(sp);				 \
		tp2 = _mm_loadu_ps(&sp[3]);			 \
		rp2 = _mm_loadu_ps(&sp[6]);			 \
		xp2 = _mm_loadu_ps(&sp[9]);			 \
		sp2 = _mm_add_ps(sp2, xp2);			 \
		sp2 = _mm_add_ps(sp2, rp2);			 \
		sp2 = _mm_add_ps(sp2, tp2);			 \
		sp1 = _mm_shuffle_ps(sp1, sp2, 			 \
				_MM_SHUFFLE_1(1, 0, 2, 1));	 \
		sp = (YTYPE *) (sl + x_ind[i + 3]);              \
		sp3 = _mm_loadu_ps(sp);				 \
		tp3 = _mm_loadu_ps(&sp[3]);			 \
		rp3 = _mm_loadu_ps(&sp[6]);			 \
		xp3 = _mm_loadu_ps(&sp[9]);			 \
		sp3 = _mm_add_ps(sp3, xp3);			 \
		sp3 = _mm_add_ps(sp3, rp3);			 \
		sp3 = _mm_add_ps(sp3, tp3);			 \
		t1 = _mm_movehl_ps(t1, sp2);			 \
		t1 = _mm_unpacklo_ps(t1, sp3);			 \
		sp2 = _mm_shuffle_ps(t1, sp3, 			 \
				_MM_SHUFFLE_1(2, 1, 1, 0));	 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
		SAVE##_SSE2(sp2);				 \
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
	    __m128 sp0, sp1, sp2, sp3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		sp2 = _mm_loadu_ps(&sp[4]);			 \
		sp0 = _mm_add_ps(sp0, sp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		sp3 = _mm_loadu_ps(&sp[4]);			 \
		sp1 = _mm_add_ps(sp1, sp3);			 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;				 \
	    __m128 sp4, sp5, sp6, sp7;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		sp2 = _mm_loadu_ps(&sp[4]);			 \
		sp4 = _mm_loadu_ps(&sp[8]);			 \
		sp0 = _mm_add_ps(sp0, sp4);			 \
		sp0 = _mm_add_ps(sp0, sp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		sp3 = _mm_loadu_ps(&sp[4]);			 \
		sp5 = _mm_loadu_ps(&sp[8]);			 \
		sp1 = _mm_add_ps(sp1, sp5);			 \
		sp1 = _mm_add_ps(sp1, sp3);			 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
	    }							 \
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
	    __m128 sp0, sp1, sp2, sp3;				 \
	    __m128 sp4, sp5, sp6, sp7;				 \
	    for (i = 0; i < xsize - 1; i += 2) {                 \
		sp = (YTYPE *) (sl + x_ind[i + 0]);              \
		sp0 = _mm_loadu_ps(sp);				 \
		sp2 = _mm_loadu_ps(&sp[4]);			 \
		sp4 = _mm_loadu_ps(&sp[8]);			 \
		sp6 = _mm_loadu_ps(&sp[12]);			 \
		sp0 = _mm_add_ps(sp0, sp6);			 \
		sp0 = _mm_add_ps(sp0, sp4);			 \
		sp0 = _mm_add_ps(sp0, sp2);			 \
		sp = (YTYPE *) (sl + x_ind[i + 1]);              \
		sp1 = _mm_loadu_ps(sp);				 \
		sp3 = _mm_loadu_ps(&sp[4]);			 \
		sp5 = _mm_loadu_ps(&sp[8]);			 \
		sp7 = _mm_loadu_ps(&sp[12]);			 \
		sp1 = _mm_add_ps(sp1, sp7);			 \
		sp1 = _mm_add_ps(sp1, sp5);			 \
		sp1 = _mm_add_ps(sp1, sp3);			 \
		SAVE##_SSE2(sp0);				 \
		SAVE##_SSE2(sp1);				 \
	    }							 \
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
