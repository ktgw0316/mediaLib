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

#pragma ident	"@(#)mlib_i_ImageSubsampleAverage_U8.c	9.3	07/11/05"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_U8_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	YTYPE	mlib_u32

#define	XTYPE	YTYPE

#define	USE_SHIFT

#define	FLT_SHIFT	16

#include <mlib_i_ImageSubsampleAverage.h>

/* *********************************************************** */

#undef	FUNC
#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_U8_##SUFF##_int

/* *********************************************************** */

static void FUNC(
    y1i) (
    YTYPE * buff,
    DTYPE * sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
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
	    for (i = 0; i < xsize; i++) {                                 \
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
	    for (i = 0; i < xsize; i++) {                                 \
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
	    for (i = 0; i < xsize; i++) {                                 \
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
	YTYPE *sp;                                              \
	XTYPE ss

/* *********************************************************** */

#define	INIT_SHF                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_s32 shift = ws->shift

/* *********************************************************** */

#if 0
#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_d64 d_mul = ws->d_mul
#else
#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_s32 d_mul_int;                                     \
	d_mul_int = *((mlib_s32 *)(&(ws->d_mul)))
#endif

/* *********************************************************** */

#define	SAVE_ADD(src)	ss = *buff + src; (*buff++) = ss

/* *********************************************************** */

#define	SAVE_SHF(src)                                           \
	ss = *buff + src;                                       \
	(*dl++) = ss >> shift;                                  \
	(*buff++) = rnd_ini

/* *********************************************************** */

#if 0
#define	SAVE_MUL(src)                                           \
	ss = *buff + src;                                       \
	(*dl++) = (DTYPE) (ss * d_mul);                         \
	(*buff++) = rnd_ini
#else
#define	SAVE_MUL(src)                                           \
	ss = *buff + src;                                       \
	(*dl++) = (DTYPE) ((ss * d_mul_int)>>FLT_SHIFT);        \
	(*buff++) = rnd_ini
#endif

/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)                                 \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap * ws,      \
	    mlib_u8 *sl)                                         \
	{                                                        \
	    INIT;                                                \
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
	    for (i = 0; i < xsize; i++) {                        \
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
