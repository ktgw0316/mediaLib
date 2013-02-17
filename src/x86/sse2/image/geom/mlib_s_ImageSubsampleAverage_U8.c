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

#pragma ident   "@(#)mlib_s_ImageSubsampleAverage_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageSubsampleAverage_U8_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	YTYPE	mlib_s32

#define	XTYPE	YTYPE

#define	USE_SHIFT

static unsigned int mlib_mask128i_arr[] = {
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0xFF000000, 0x00000000, 0x00000000, 0x00000000,
	0xFFFF0000, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFF00, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFF000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFF0000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFF00, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFF00, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

#include <mlib_ImageSubsampleAverage.h>

/* *********************************************************** */
#define	LOAD_128(p) _mm_loadu_si128(p)
#define	LOAD_64(p) _mm_loadl_epi64(p)
#define	STORE_128(dp, x) _mm_storeu_si128(dp, x)
#define	STORE_64(dp, x) _mm_storel_epi64(dp, x)

#define	EXPAND_S16_HI(x)  _mm_unpackhi_epi8(x, zero)
#define	EXPAND_S16_LO(x)  _mm_unpacklo_epi8(x, zero)
#define	EXPAND_S32_HI(x) _mm_unpackhi_epi16(x, zero)
#define	EXPAND_S32_LO(x) _mm_unpacklo_epi16(x, zero)

static void FUNC(
    y1i) (
    YTYPE * buff,
    DTYPE * sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	mlib_s32 i;
	__m128i ss, ss0, ss1, dd0, dd1, dd2, dd3;
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i *ssp = (__m128i *)sp, *ddp = (__m128i *)buff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */


	for (i = 0; i < (xsize - 15); i += 16) {
		ss = LOAD_128(ssp);
		ss0 = EXPAND_S16_LO(ss);
		ss1 = EXPAND_S16_HI(ss);
		dd0 = EXPAND_S32_LO(ss0);
		dd1 = EXPAND_S32_HI(ss0);
		dd2 = EXPAND_S32_LO(ss1);
		dd3 = EXPAND_S32_HI(ss1);

		STORE_128(ddp, dd0);
		ddp++;
		STORE_128(ddp, dd1);
		ddp++;
		STORE_128(ddp, dd2);
		ddp++;
		STORE_128(ddp, dd3);
		ddp++;
		ssp++;
	}
	for (; i < xsize; i++) {
		buff[i] = sp[i];
	}
}

/* *********************************************************** */
#define	FUNC_Y(SUFF, ADD, ADD_SSE2)                           \
	static void FUNC(y2##SUFF) (YTYPE * buff, DTYPE * sp,     \
	    mlib_s32 slb, mlib_s32 xsize)                         \
	{                                                         \
	    mlib_s32 i;                                           \
	    DTYPE *sp1 = sp + slb;                                \
		__m128i *ssp0 = (__m128i *)sp, *ssp1 = (__m128i *)sp1;\
		__m128i *ddp = (__m128i *)buff;                       \
		__m128i a0, a1, b0, b1;                               \
		const __m128i zero = _mm_set1_epi8(0x00);             \
		                                                      \
		for (i = 0; i < (xsize - 15); i += 16) {              \
			a0 = LOAD_128(ssp0);                              \
			a1 = LOAD_128(ssp1);                              \
			b0 = _mm_add_epi16(EXPAND_S16_LO(a0),             \
				EXPAND_S16_LO(a1));                           \
			b1 = _mm_add_epi16(EXPAND_S16_HI(a0),             \
				EXPAND_S16_HI(a1));                           \
			ADD_SSE2(ddp, EXPAND_S32_LO(b0)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_HI(b0)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_LO(b1)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_HI(b1)); ddp++;          \
			ssp0++; ssp1++;                                   \
		}                                                     \
	    for (; i < xsize; i++) {                              \
			ADD(buff[i], (YTYPE) sp[i] + sp1[i]);             \
	    }                                                     \
	}                                                         \
	                                                          \
/* *************************************************** */	  \
	static void FUNC(y3##SUFF) (YTYPE * buff, DTYPE * sp,     \
	    mlib_s32 slb, mlib_s32 xsize)                         \
	{                                                         \
	    mlib_s32 i;                                           \
	    DTYPE *sp1 = sp + slb;                                \
	    DTYPE *sp2 = sp + 2 * slb;                            \
		__m128i *ssp0 = (__m128i *)sp;                        \
		__m128i *ssp1 = (__m128i *)sp1;                       \
		__m128i *ssp2 = (__m128i *)sp2;                       \
		__m128i *ddp = (__m128i *)buff;                       \
		__m128i a0, a1, a2, b0, b1;                           \
		const __m128i zero = _mm_set1_epi8(0x00);             \
		                                                      \
		for (i = 0; i < (xsize - 15); i += 16) {              \
			a0 = LOAD_128(ssp0);                              \
			a1 = LOAD_128(ssp1);                              \
			a2 = LOAD_128(ssp2);                              \
			b0 = _mm_add_epi16(EXPAND_S16_LO(a0),             \
				EXPAND_S16_LO(a1));                           \
			b0 = _mm_add_epi16(b0, EXPAND_S16_LO(a2));        \
			b1 = _mm_add_epi16(EXPAND_S16_HI(a0),             \
				EXPAND_S16_HI(a1));                           \
			b1 = _mm_add_epi16(b1, EXPAND_S16_HI(a2));        \
			ADD_SSE2(ddp, EXPAND_S32_LO(b0)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_HI(b0)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_LO(b1)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_HI(b1)); ddp++;          \
			ssp0++; ssp1++; ssp2++;                           \
		}                                                     \
                                                              \
	    for (; i < xsize; i++) {                              \
			ADD(buff[i], (YTYPE) sp[i] + sp1[i] + sp2[i]);    \
	    }                                                     \
	}                                                         \
	                                                          \
/* *************************************************** */	  \
	static void FUNC(y4##SUFF) (YTYPE * buff, DTYPE * sp,     \
	    mlib_s32 slb, mlib_s32 xsize)                         \
	{                                                         \
	    mlib_s32 i;                                           \
	    DTYPE *sp1 = sp + slb;                                \
	    DTYPE *sp2 = sp + 2 * slb;                            \
	    DTYPE *sp3 = sp + 3 * slb;                            \
		__m128i *ssp0 = (__m128i *)sp;                        \
		__m128i *ssp1 = (__m128i *)sp1;                       \
		__m128i *ssp2 = (__m128i *)sp2;                       \
		__m128i *ssp3 = (__m128i *)sp3;                       \
		__m128i *ddp = (__m128i *)buff;                       \
		__m128i a0, a1, a2, a3, b0, b1;                       \
		const __m128i zero = _mm_set1_epi8(0x00);             \
		                                                      \
		for (i = 0; i < (xsize - 15); i += 16) {              \
			a0 = LOAD_128(ssp0);                              \
			a1 = LOAD_128(ssp1);                              \
			a2 = LOAD_128(ssp2);                              \
			a3 = LOAD_128(ssp3);                              \
			b0 = _mm_add_epi16(EXPAND_S16_LO(a0),             \
				EXPAND_S16_LO(a1));                           \
			b0 = _mm_add_epi16(b0, EXPAND_S16_LO(a2));        \
			b0 = _mm_add_epi16(b0, EXPAND_S16_LO(a3));        \
			b1 = _mm_add_epi16(EXPAND_S16_HI(a0),             \
				EXPAND_S16_HI(a1));                           \
			b1 = _mm_add_epi16(b1, EXPAND_S16_HI(a2));        \
			b1 = _mm_add_epi16(b1, EXPAND_S16_HI(a3));        \
			ADD_SSE2(ddp, EXPAND_S32_LO(b0)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_HI(b0)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_LO(b1)); ddp++;          \
			ADD_SSE2(ddp, EXPAND_S32_HI(b1)); ddp++;          \
			ssp0++; ssp1++;                                   \
			ssp2++; ssp3++;                                   \
		}                                                     \
                                                              \
	    for (; i < xsize; i++) {                              \
			ADD(buff[i],                                      \
				(YTYPE) sp[i] + sp1[i] + sp2[i] + sp3[i]);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src
#define	STORE_SSE2(dst, src)                                      \
	STORE_128(dst, src)
#define	ADD(dst, src)	dst = dst + src
#define	ADD_SSE2(dst, src)                                        \
	STORE_128(dst, _mm_add_epi32(LOAD_128(dst), src))

FUNC_Y(i, STORE, STORE_SSE2)
FUNC_Y(a, ADD, ADD_SSE2)

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
	mlib_s32 shift = ws->shift;                             \
	__m128i rnd_ini_128i =                                  \
		_mm_set1_epi32((mlib_s32)rnd_ini)

/* *********************************************************** */

#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE rnd_ini = *(DTYPE *) & (ws->d_ini);               \
	DTYPE *dl = ws->dl;                                     \
	mlib_d64 d_mul = ws->d_mul;                             \
	__m128i rnd_ini_128i =                                  \
		_mm_set1_epi32((mlib_s32)rnd_ini);                  \
	__m128i mul_128i =                                      \
		_mm_set1_epi32((mlib_s32)(d_mul * (1 << 30)));      \
	__m128i mul_16 =                                           \
		_mm_set1_epi16((mlib_s32)(d_mul * (1 << 16)));      \


/* *********************************************************** */

#define	SAVE_ADD(src)	ss = *buff + src; (*buff++) = ss
#define	SAVE_ADD_SSE2(src0, src1)                           \
	STORE_128(ddp, _mm_add_epi32(LOAD_128(ddp), src0));     \
	ddp++;                                                  \
	STORE_128(ddp, _mm_add_epi32(LOAD_128(ddp), src1));     \
	ddp++;                                                  \
	buff += 8

/* *********************************************************** */

#define	SAVE_SHF(src)                                       \
	ss = *buff + src;                                       \
	(*dl++) = ss >> shift;                                  \
	(*buff++) = rnd_ini

#define	SAVE_SHF_SSE2(src0, src1)                           \
	dd0 = _mm_add_epi32(LOAD_128(ddp), src0);               \
	dd1 = _mm_add_epi32(LOAD_128(ddp + 1), src1);           \
	dd = _mm_packs_epi32(_mm_srli_epi32(dd0, shift),        \
			_mm_srli_epi32(dd1, shift));                    \
	dd = _mm_packus_epi16(dd, zero);                        \
	STORE_64((__m128i *)dl, dd);                            \
	STORE_128(ddp++, rnd_ini_128i);                         \
	STORE_128(ddp++, rnd_ini_128i);                         \
	dl += 8; buff += 8

/* *********************************************************** */

#define	SAVE_MUL(src)                                       \
	ss = *buff + src;                                       \
	(*dl++) = (DTYPE) (ss * d_mul);                         \
	(*buff++) = rnd_ini

#define	SAVE_MUL_SSE2(src0, src1)                           \
	dd0 = _mm_add_epi32(LOAD_128(ddp), src0);               \
	dd1 = _mm_add_epi32(LOAD_128(ddp + 1), src1);           \
	d0 = _mm_mul_epu32(dd0, mul_128i);                      \
	d1 = _mm_mul_epu32(_mm_srli_epi64(dd0, 32), mul_128i);  \
	d2 = _mm_mul_epu32(dd1, mul_128i);                      \
	d3 = _mm_mul_epu32(_mm_srli_epi64(dd1, 32), mul_128i);  \
	dd0 = _mm_or_si128(_mm_srli_epi64(d0, 30),              \
			_mm_slli_epi64(_mm_srli_epi64(d1, 30), 32));    \
	dd1 = _mm_or_si128(_mm_srli_epi64(d2, 30),              \
			_mm_slli_epi64(_mm_srli_epi64(d3, 30), 32));    \
	dd = _mm_packs_epi32(dd0, dd1);                         \
	dd = _mm_packus_epi16(dd, zero);                        \
	STORE_64((__m128i *)dl, dd);                            \
	STORE_128(ddp++, rnd_ini_128i);                         \
	STORE_128(ddp++, rnd_ini_128i);                         \
	dl += 8; buff += 8
/* *********************************************************** */
#define	COMP_4x32(a0, a1, a2, a3)                           \
	_mm_unpacklo_epi64(                                     \
		_mm_unpacklo_epi32(a0, a1),                         \
		_mm_unpacklo_epi32(a2, a3))
/* *********************************************************** */
#define	ADD_LO_2x32(a0)                                     \
	_mm_add_epi32(_mm_and_si128(a0, mask_lo2x32),           \
		_mm_srli_epi64(a0, 32))

/* *********************************************************** */
#ifndef _MM_SHUFFLE
#define	_MM_SHUFFLE(fp3, fp2, fp1, fp0)                     \
		(((fp3) << 6) | ((fp2) << 4) | ((fp1) << 2) | (fp0))
#endif

#define	SHUFFLE_S32(a0, fp3, fp2, fp1, fp0)                 \
	_mm_shuffle_epi32(a0, _MM_SHUFFLE(fp3, fp2, fp1, fp0))

#define	MM_SL1_S32(a0)  SHUFFLE_S32(a0, 0, 3, 2, 1)
#define	MM_SL2_S32(a0)  SHUFFLE_S32(a0, 1, 0, 3, 2)
#define	MM_SL3_S32(a0)  SHUFFLE_S32(a0, 2, 1, 0, 3)

/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE, SAVE_SSE2)                 \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap * ws,     \
	    mlib_u8 *sl)                                        \
	{                                                       \
	    INIT;                                               \
		__m128i *ddp = (__m128i *)buff;                     \
		mlib_s32 sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                               \
		__m128i d0, d1, d2, d3;                             \
		const __m128i zero = _mm_set1_epi8(0x00);           \
		for (i = 0; i < (xsize - 7); i += 8) {              \
			sp0 = ((YTYPE *) (sl + x_ind[i+ 0]))[0];        \
			sp1 = ((YTYPE *) (sl + x_ind[i+ 1]))[0];        \
			sp2 = ((YTYPE *) (sl + x_ind[i+ 2]))[0];        \
			sp3 = ((YTYPE *) (sl + x_ind[i+ 3]))[0];        \
			sp4 = ((YTYPE *) (sl + x_ind[i+ 4]))[0];        \
			sp5 = ((YTYPE *) (sl + x_ind[i+ 5]))[0];        \
			sp6 = ((YTYPE *) (sl + x_ind[i+ 6]))[0];        \
			sp7 = ((YTYPE *) (sl + x_ind[i+ 7]))[0];        \
			SAVE_SSE2(_mm_setr_epi32(sp0, sp1, sp2, sp3),   \
				_mm_setr_epi32(sp4, sp5, sp6, sp7));        \
		}                                                   \
	    for (; i < xsize; i++) {                            \
			sp = (YTYPE *) (sl + x_ind[i]);                 \
			SAVE(sp[0]);                                    \
	    }                                                   \
	}                                                       \
	                                                        \
/* ********************************************************** */ \
	static void FUNC(ch1_x2##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i mask_lo2x32 = _mm_unpacklo_epi32(          \
					LOAD_128(((__m128i *)mlib_mask128i_arr)\
					+ 8), zero);                           \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_64((__m128i *)(sl + x_ind[i+ 0]));  \
			sp0 = ADD_LO_2x32(sp0);                        \
			sp1 = LOAD_64((__m128i *)(sl + x_ind[i+ 1]));  \
			sp1 = ADD_LO_2x32(sp1);                        \
			sp2 = LOAD_64((__m128i *)(sl + x_ind[i+ 2]));  \
			sp2 = ADD_LO_2x32(sp2);                        \
			sp3 = LOAD_64((__m128i *)(sl + x_ind[i+ 3]));  \
			sp3 = ADD_LO_2x32(sp3);                        \
			sp4 = LOAD_64((__m128i *)(sl + x_ind[i+ 4]));  \
			sp4 = ADD_LO_2x32(sp4);                        \
			sp5 = LOAD_64((__m128i *)(sl + x_ind[i+ 5]));  \
			sp5 = ADD_LO_2x32(sp5);                        \
			sp6 = LOAD_64((__m128i *)(sl + x_ind[i+ 6]));  \
			sp6 = ADD_LO_2x32(sp6);                        \
			sp7 = LOAD_64((__m128i *)(sl + x_ind[i+ 7]));  \
			sp7 = ADD_LO_2x32(sp7);                        \
			SAVE_SSE2(COMP_4x32(sp0, sp1, sp2, sp3),       \
				COMP_4x32(sp4, sp5, sp6, sp7));            \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[1]);                           \
	    }                                                  \
	}                                                      \
	                                                       \
/* ********************************************************** */ \
	static void FUNC(ch1_x3##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i mask_lo2x32 = _mm_unpacklo_epi32(          \
					LOAD_128(((__m128i *)                  \
					mlib_mask128i_arr) + 8), zero);        \
		__m128i emask = LOAD_128(                          \
					((__m128i *)mlib_mask128i_arr) + 12);  \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 0])), emask);\
			sp0 = ADD_LO_2x32(sp0);                        \
			sp0 = ADD_LO_2x32(SHUFFLE_S32(sp0, 3, 1, 2, 0)); \
			sp1 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 1])), emask);\
			sp1 = ADD_LO_2x32(sp1);                        \
			sp1 = ADD_LO_2x32(SHUFFLE_S32(sp1, 3, 1, 2, 0)); \
			sp2 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 2])), emask);\
			sp2 = ADD_LO_2x32(sp2);                        \
			sp2 = ADD_LO_2x32(SHUFFLE_S32(sp2, 3, 1, 2, 0)); \
			sp3 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 3])), emask);\
			sp3 = ADD_LO_2x32(sp3);                        \
			sp3 = ADD_LO_2x32(SHUFFLE_S32(sp3, 3, 1, 2, 0)); \
			sp4 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 4])), emask);\
			sp4 = ADD_LO_2x32(sp4);                        \
			sp4 = ADD_LO_2x32(SHUFFLE_S32(sp4, 3, 1, 2, 0)); \
			sp5 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 5])), emask);\
			sp5 = ADD_LO_2x32(sp5);                        \
			sp5 = ADD_LO_2x32(SHUFFLE_S32(sp5, 3, 1, 2, 0)); \
			sp6 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 6])), emask);\
			sp6 = ADD_LO_2x32(sp6);                        \
			sp6 = ADD_LO_2x32(SHUFFLE_S32(sp6, 3, 1, 2, 0)); \
			sp7 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 7])), emask);\
			sp7 = ADD_LO_2x32(sp7);                        \
			sp7 = ADD_LO_2x32(SHUFFLE_S32(sp7, 3, 1, 2, 0)); \
			SAVE_SSE2(COMP_4x32(sp0, sp1, sp2, sp3),       \
				COMP_4x32(sp4, sp5, sp6, sp7));            \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[1] + sp[2]);                   \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch1_x4##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i mask_lo2x32 = _mm_unpacklo_epi32(          \
					LOAD_128(((__m128i *)                  \
					mlib_mask128i_arr) + 8), zero);        \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl + x_ind[i+ 0])); \
			sp0 = ADD_LO_2x32(sp0);                        \
			sp0 = ADD_LO_2x32(SHUFFLE_S32(sp0, 3, 1, 2, 0)); \
			sp1 = LOAD_128((__m128i *)(sl + x_ind[i+ 1])); \
			sp1 = ADD_LO_2x32(sp1);                        \
			sp1 = ADD_LO_2x32(SHUFFLE_S32(sp1, 3, 1, 2, 0)); \
			sp2 = LOAD_128((__m128i *)(sl + x_ind[i+ 2])); \
			sp2 = ADD_LO_2x32(sp2);                        \
			sp2 = ADD_LO_2x32(SHUFFLE_S32(sp2, 3, 1, 2, 0)); \
			sp3 = LOAD_128((__m128i *)(sl + x_ind[i+ 3])); \
			sp3 = ADD_LO_2x32(sp3);                        \
			sp3 = ADD_LO_2x32(SHUFFLE_S32(sp3, 3, 1, 2, 0)); \
			sp4 = LOAD_128((__m128i *)(sl + x_ind[i+ 4])); \
			sp4 = ADD_LO_2x32(sp4);                        \
			sp4 = ADD_LO_2x32(SHUFFLE_S32(sp4, 3, 1, 2, 0)); \
			sp5 = LOAD_128((__m128i *)(sl + x_ind[i+ 5])); \
			sp5 = ADD_LO_2x32(sp5);                        \
			sp5 = ADD_LO_2x32(SHUFFLE_S32(sp5, 3, 1, 2, 0)); \
			sp6 = LOAD_128((__m128i *)(sl + x_ind[i+ 6])); \
			sp6 = ADD_LO_2x32(sp6);                        \
			sp6 = ADD_LO_2x32(SHUFFLE_S32(sp6, 3, 1, 2, 0)); \
			sp7 = LOAD_128((__m128i *)(sl + x_ind[i+ 7])); \
			sp7 = ADD_LO_2x32(sp7);                        \
			sp7 = ADD_LO_2x32(SHUFFLE_S32(sp7, 3, 1, 2, 0)); \
			SAVE_SSE2(COMP_4x32(sp0, sp1, sp2, sp3),       \
				COMP_4x32(sp4, sp5, sp6, sp7));            \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[1] + sp[2] + sp[3]);           \
	    }                                                  \
	}                                                      \
	                                                       \
/* ********************************************************** */ \
	static void FUNC(ch2_x1##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_64((__m128i *)(sl + x_ind[i+ 0]));  \
			sp1 = LOAD_64((__m128i *)(sl + x_ind[i+ 1]));  \
			sp2 = LOAD_64((__m128i *)(sl + x_ind[i+ 2]));  \
			sp3 = LOAD_64((__m128i *)(sl + x_ind[i+ 3]));  \
			sp4 = LOAD_64((__m128i *)(sl + x_ind[i+ 4]));  \
			sp5 = LOAD_64((__m128i *)(sl + x_ind[i+ 5]));  \
			sp6 = LOAD_64((__m128i *)(sl + x_ind[i+ 6]));  \
			sp7 = LOAD_64((__m128i *)(sl + x_ind[i+ 7]));  \
			SAVE_SSE2(_mm_unpacklo_epi64(sp0, sp1),        \
				_mm_unpacklo_epi64(sp2, sp3));             \
			SAVE_SSE2(_mm_unpacklo_epi64(sp4, sp5),        \
				_mm_unpacklo_epi64(sp6, sp7));             \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0]);                                   \
			SAVE(sp[1]);                                   \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch2_x2##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl + x_ind[i+ 0])); \
			sp0 = _mm_add_epi32(MM_SL2_S32(sp0), sp0);     \
			sp1 = LOAD_128((__m128i *)(sl + x_ind[i+ 1])); \
			sp1 = _mm_add_epi32(MM_SL2_S32(sp1), sp1);     \
			sp2 = LOAD_128((__m128i *)(sl + x_ind[i+ 2])); \
			sp2 = _mm_add_epi32(MM_SL2_S32(sp2), sp2);     \
			sp3 = LOAD_128((__m128i *)(sl + x_ind[i+ 3])); \
			sp3 = _mm_add_epi32(MM_SL2_S32(sp3), sp3);     \
			sp4 = LOAD_128((__m128i *)(sl + x_ind[i+ 4])); \
			sp4 = _mm_add_epi32(MM_SL2_S32(sp4), sp4);     \
			sp5 = LOAD_128((__m128i *)(sl + x_ind[i+ 5])); \
			sp5 = _mm_add_epi32(MM_SL2_S32(sp5), sp5);     \
			sp6 = LOAD_128((__m128i *)(sl + x_ind[i+ 6])); \
			sp6 = _mm_add_epi32(MM_SL2_S32(sp6), sp6);     \
			sp7 = LOAD_128((__m128i *)(sl + x_ind[i+ 7])); \
			sp7 = _mm_add_epi32(MM_SL2_S32(sp7), sp7);     \
			SAVE_SSE2(_mm_unpacklo_epi64(sp0, sp1),        \
				_mm_unpacklo_epi64(sp2, sp3));             \
			SAVE_SSE2(_mm_unpacklo_epi64(sp4, sp5),        \
				_mm_unpacklo_epi64(sp6, sp7));             \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[2]);                           \
			SAVE(sp[1] + sp[3]);                           \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch2_x3##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i sp01, sp11, sp21, sp31, sp41, sp51, sp61, sp71;\
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 0])) + 1);                    \
			sp0 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp0), sp0), sp01);                     \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 1])) + 1);                    \
			sp1 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp1), sp1), sp11);                     \
			sp2 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 2]));                         \
			sp21 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 2])) + 1);                    \
			sp2 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp2), sp2), sp21);                     \
			sp3 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 3]));                         \
			sp31 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 3])) + 1);                    \
			sp3 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp3), sp3), sp31);                     \
			sp4 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 4]));                         \
			sp41 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 4])) + 1);                    \
			sp4 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp4), sp4), sp41);                     \
			sp5 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 5]));                         \
			sp51 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 5])) + 1);                    \
			sp5 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp5), sp5), sp51);                     \
			sp6 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 6]));                         \
			sp61 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 6])) + 1);                    \
			sp6 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp6), sp6), sp61);                     \
			sp7 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 7]));                         \
			sp71 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 7])) + 1);                    \
			sp7 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(              \
					sp7), sp7), sp71);                     \
			SAVE_SSE2(_mm_unpacklo_epi64(sp0, sp1),        \
				_mm_unpacklo_epi64(sp2, sp3));             \
			SAVE_SSE2(_mm_unpacklo_epi64(sp4, sp5),        \
				_mm_unpacklo_epi64(sp6, sp7));             \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[2] + sp[4]);                   \
			SAVE(sp[1] + sp[3] + sp[5]);                   \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch2_x4##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i sp01, sp11, sp21, sp31, sp41, sp51, sp61, sp71;\
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 1);                    \
			sp0 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp0), sp0),   \
					_mm_add_epi32(MM_SL2_S32(sp01), sp01));\
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 1);                    \
			sp1 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp1), sp1),   \
					_mm_add_epi32(MM_SL2_S32(sp11), sp11));\
			sp2 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 2]));                         \
			sp21 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 2])) + 1);                    \
			sp2 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp2), sp2),   \
					_mm_add_epi32(MM_SL2_S32(sp21), sp21));\
			sp3 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 3]));                         \
			sp31 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 3])) + 1);                    \
			sp3 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp3), sp3),   \
					_mm_add_epi32(MM_SL2_S32(sp31), sp31));\
			sp4 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 4]));                         \
			sp41 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 4])) + 1);                    \
			sp4 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp4), sp4),   \
					_mm_add_epi32(MM_SL2_S32(sp41), sp41));\
			sp5 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 5]));                         \
			sp51 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 5])) + 1);                    \
			sp5 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp5), sp5),   \
					_mm_add_epi32(MM_SL2_S32(sp51), sp51));\
			sp6 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 6]));                         \
			sp61 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 6])) + 1);                    \
			sp6 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp6), sp6),   \
					_mm_add_epi32(MM_SL2_S32(sp61), sp61));\
			sp7 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 7]));                         \
			sp71 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 7])) + 1);                    \
			sp7 = _mm_add_epi32(                           \
					_mm_add_epi32(MM_SL2_S32(sp7), sp7),   \
					_mm_add_epi32(MM_SL2_S32(sp71), sp71));\
			SAVE_SSE2(_mm_unpacklo_epi64(sp0, sp1),        \
				_mm_unpacklo_epi64(sp2, sp3));             \
			SAVE_SSE2(_mm_unpacklo_epi64(sp4, sp5),        \
				_mm_unpacklo_epi64(sp6, sp7));             \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[2] + sp[4] + sp[6]);           \
			SAVE(sp[1] + sp[3] + sp[5] + sp[7]);           \
	    }                                                  \
	}                                                      \
	                                                       \
/* ********************************************************** */ \
	static void FUNC(ch3_x1##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i emask = LOAD_128(                          \
					((__m128i *)mlib_mask128i_arr) + 12);  \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 0])), emask);\
			sp1 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 1])), emask);\
			sp1 = MM_SL1_S32(sp1);                         \
			sp2 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 2])), emask);\
			sp2 = MM_SL2_S32(sp2);                         \
			sp3 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 3])), emask);\
			sp3 = MM_SL3_S32(sp3);                         \
			sp4 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 4])), emask);\
			sp5 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 5])), emask);\
			sp5 = MM_SL1_S32(sp5);                         \
			sp6 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 6])), emask);\
			sp6 = MM_SL2_S32(sp6);                         \
			sp7 = _mm_and_si128(LOAD_128(                  \
					(__m128i *)(sl + x_ind[i+ 7])), emask);\
			sp7 = MM_SL3_S32(sp7);                         \
                                                           \
			sp0 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp1), sp0);   \
			sp1 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp1, zero),         \
					_mm_unpackhi_epi64(zero, sp2));        \
			sp2 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp2, zero), sp3);   \
			sp4 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp5), sp4);   \
			sp5 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp5, zero),         \
					_mm_unpackhi_epi64(zero, sp6));        \
			sp6 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp6, zero), sp7);   \
			SAVE_SSE2(sp0, sp1);                           \
			SAVE_SSE2(sp2, sp4);                           \
			SAVE_SSE2(sp5, sp6);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0]);                                   \
			SAVE(sp[1]);                                   \
			SAVE(sp[2]);                                   \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch3_x2##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i sp01, sp11, sp21, sp31, sp41, sp51, sp61, sp71;\
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i emask = LOAD_128(                          \
					((__m128i *)mlib_mask128i_arr) + 12);  \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 0])) + 1);                    \
			sp0 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp01, 4),               \
					_mm_and_si128(sp0, emask)),            \
					_mm_srli_si128(sp0, 12));              \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 1])) + 1);                    \
			sp1 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp11, 4),               \
					_mm_and_si128(sp1, emask)),            \
					_mm_srli_si128(sp1, 12));              \
			sp1 = MM_SL1_S32(sp1);                         \
			sp2 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 2]));                         \
			sp21 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 2])) + 1);                    \
			sp2 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp21, 4),               \
					_mm_and_si128(sp2, emask)),            \
					_mm_srli_si128(sp2, 12));              \
			sp2 = MM_SL2_S32(sp2);                         \
			sp3 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 3]));                         \
			sp31 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 3])) + 1);                    \
			sp3 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp31, 4),               \
					_mm_and_si128(sp3, emask)),            \
					_mm_srli_si128(sp3, 12));              \
			sp3 = MM_SL3_S32(sp3);                         \
			sp4 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 4]));                         \
			sp41 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 4])) + 1);                    \
			sp4 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp41, 4),               \
					_mm_and_si128(sp4, emask)),            \
					_mm_srli_si128(sp4, 12));              \
			sp5 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 5]));                         \
			sp51 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 5])) + 1);                    \
			sp5 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp51, 4),               \
					_mm_and_si128(sp5, emask)),            \
					_mm_srli_si128(sp5, 12));              \
			sp5 = MM_SL1_S32(sp5);                         \
			sp6 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 6]));                         \
			sp61 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 6])) + 1);                    \
			sp6 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp61, 4),               \
					_mm_and_si128(sp6, emask)),            \
					_mm_srli_si128(sp6, 12));              \
			sp6 = MM_SL2_S32(sp6);                         \
			sp7 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 7]));                         \
			sp71 = LOAD_64(((__m128i *)(sl +               \
					x_ind[i+ 7])) + 1);                    \
			sp7 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp71, 4),               \
					_mm_and_si128(sp7, emask)),            \
					_mm_srli_si128(sp7, 12));              \
			sp7 = MM_SL3_S32(sp7);                         \
                                                           \
			sp0 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp1), sp0);   \
			sp1 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp1, zero),         \
					_mm_unpackhi_epi64(zero, sp2));        \
			sp2 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp2, zero), sp3);   \
			sp4 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp5), sp4);   \
			sp5 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp5, zero),         \
					_mm_unpackhi_epi64(zero, sp6));        \
			sp6 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp6, zero), sp7);   \
			SAVE_SSE2(sp0, sp1);                           \
			SAVE_SSE2(sp2, sp4);                           \
			SAVE_SSE2(sp5, sp6);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[3]);                           \
			SAVE(sp[1] + sp[4]);                           \
			SAVE(sp[2] + sp[5]);                           \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch3_x3##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i sp01, sp11, sp21, sp31, sp41, sp51, sp61, sp71;\
		__m128i sp02, sp12, sp22, sp32, sp42, sp52, sp62, sp72;\
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i emasklo = LOAD_128(                        \
					((__m128i *)mlib_mask128i_arr) + 4);   \
		__m128i emask = LOAD_128(                          \
					((__m128i *)mlib_mask128i_arr) + 12);  \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 1);                    \
			sp02 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 0])) + 2), emasklo);          \
			sp0 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp02, 8),               \
					_mm_and_si128(sp0, emask)),            \
					_mm_srli_si128(sp0, 12));              \
			sp0 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp01, 4), emask), sp0), \
					_mm_srli_si128(sp01, 8));              \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 1);                    \
			sp12 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 1])) + 2), emasklo);          \
			sp1 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp12, 8),               \
					_mm_and_si128(sp1, emask)),            \
					_mm_srli_si128(sp1, 12));              \
			sp1 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp11, 4), emask), sp1), \
					_mm_srli_si128(sp11, 8));              \
			sp1 = MM_SL1_S32(sp1);                         \
			sp2 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 2]));                         \
			sp21 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 2])) + 1);                    \
			sp22 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 2])) + 2), emasklo);          \
			sp2 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp22, 8),               \
					_mm_and_si128(sp2, emask)),            \
					_mm_srli_si128(sp2, 12));              \
			sp2 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp21, 4), emask), sp2), \
					_mm_srli_si128(sp21, 8));              \
			sp2 = MM_SL2_S32(sp2);                         \
			sp3 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 3]));                         \
			sp31 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 3])) + 1);                    \
			sp32 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 3])) + 2), emasklo);          \
			sp3 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp32, 8),               \
					_mm_and_si128(sp3, emask)),            \
					_mm_srli_si128(sp3, 12));              \
			sp3 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp31, 4), emask), sp3), \
					_mm_srli_si128(sp31, 8));              \
			sp3 = MM_SL3_S32(sp3);                         \
			sp4 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 4]));                         \
			sp41 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 4])) + 1);                    \
			sp42 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 4])) + 2), emasklo);          \
			sp4 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp42, 8),               \
					_mm_and_si128(sp4, emask)),            \
					_mm_srli_si128(sp4, 12));              \
			sp4 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp41, 4), emask), sp4), \
					_mm_srli_si128(sp41, 8));              \
			sp5 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 5]));                         \
			sp51 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 5])) + 1);                    \
			sp52 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 5])) + 2), emasklo);          \
			sp5 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp52, 8),               \
					_mm_and_si128(sp5, emask)),            \
					_mm_srli_si128(sp5, 12));              \
			sp5 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp51, 4), emask), sp5), \
					_mm_srli_si128(sp51, 8));              \
			sp5 = MM_SL1_S32(sp5);                         \
			sp6 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 6]));                         \
			sp61 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 6])) + 1);                    \
			sp62 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 6])) + 2), emasklo);          \
			sp6 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp62, 8),               \
					_mm_and_si128(sp6, emask)),            \
					_mm_srli_si128(sp6, 12));              \
			sp6 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp61, 4), emask), sp6), \
					_mm_srli_si128(sp61, 8));              \
			sp6 = MM_SL2_S32(sp6);                         \
			sp7 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 7]));                         \
			sp71 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 7])) + 1);                    \
			sp72 = _mm_and_si128(                          \
					LOAD_128(((__m128i *)(sl +             \
					x_ind[i+ 7])) + 2), emasklo);          \
			sp7 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_slli_si128(sp72, 8),               \
					_mm_and_si128(sp7, emask)),            \
					_mm_srli_si128(sp7, 12));              \
			sp7 = _mm_add_epi32(_mm_add_epi32(             \
					_mm_and_si128(                         \
					_mm_slli_si128(sp71, 4), emask), sp7), \
					_mm_srli_si128(sp71, 8));              \
			sp7 = MM_SL3_S32(sp7);                         \
                                                           \
			sp0 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp1), sp0);   \
			sp1 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp1, zero),         \
					_mm_unpackhi_epi64(zero, sp2));        \
			sp2 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp2, zero), sp3);   \
			sp4 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp5), sp4);   \
			sp5 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp5, zero),         \
					_mm_unpackhi_epi64(zero, sp6));        \
			sp6 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp6, zero), sp7);   \
			SAVE_SSE2(sp0, sp1);                           \
			SAVE_SSE2(sp2, sp4);                           \
			SAVE_SSE2(sp5, sp6);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[3] + sp[6]);                   \
			SAVE(sp[1] + sp[4] + sp[7]);                   \
			SAVE(sp[2] + sp[5] + sp[8]);                   \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch3_x4##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;    \
		__m128i sp01, sp11, sp21, sp31, sp41, sp51, sp61, sp71;\
		__m128i sp02, sp12, sp22, sp32, sp42, sp52, sp62, sp72;\
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		__m128i emask = LOAD_128(                          \
					((__m128i *)mlib_mask128i_arr) + 12);  \
		for (i = 0; i < (xsize - 7); i += 8) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 1);                    \
			sp02 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 2);                    \
			sp0 = _mm_add_epi32(                           \
					_mm_and_si128(sp0, emask),             \
					_mm_srli_si128(sp0, 12));              \
			sp01 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp01, 4), emask),       \
					_mm_srli_si128(sp01, 8));              \
			sp02 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp02, 8), emask),       \
					_mm_srli_si128(sp02, 4));              \
			sp0 = _mm_add_epi32(                           \
					_mm_add_epi32(sp0, sp01), sp02);       \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 1);                    \
			sp12 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 2);                    \
			sp1 = _mm_add_epi32(                           \
					_mm_and_si128(sp1, emask),             \
					_mm_srli_si128(sp1, 12));              \
			sp11 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp11, 4), emask),       \
					_mm_srli_si128(sp11, 8));              \
			sp12 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp12, 8), emask),       \
					_mm_srli_si128(sp12, 4));              \
			sp1 = _mm_add_epi32(                           \
					_mm_add_epi32(sp1, sp11), sp12);       \
			sp1 = MM_SL1_S32(sp1);                         \
			sp2 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 2]));                         \
			sp21 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 2])) + 1);                    \
			sp22 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 2])) + 2);                    \
			sp2 = _mm_add_epi32(                           \
					_mm_and_si128(sp2, emask),             \
					_mm_srli_si128(sp2, 12));              \
			sp21 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp21, 4), emask),       \
					_mm_srli_si128(sp21, 8));              \
			sp22 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp22, 8), emask),       \
					_mm_srli_si128(sp22, 4));              \
			sp2 = _mm_add_epi32(                           \
					_mm_add_epi32(sp2, sp21), sp22);       \
			sp2 = MM_SL2_S32(sp2);                         \
			sp3 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 3]));                         \
			sp31 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 3])) + 1);                    \
			sp32 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 3])) + 2);                    \
			sp3 = _mm_add_epi32(                           \
					_mm_and_si128(sp3, emask),             \
					_mm_srli_si128(sp3, 12));              \
			sp31 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp31, 4), emask),       \
					_mm_srli_si128(sp31, 8));              \
			sp32 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp32, 8), emask),       \
					_mm_srli_si128(sp32, 4));              \
			sp3 = _mm_add_epi32(                           \
					_mm_add_epi32(sp3, sp31), sp32);       \
			sp3 = MM_SL3_S32(sp3);                         \
			sp4 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 4]));                         \
			sp41 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 4])) + 1);                    \
			sp42 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 4])) + 2);                    \
			sp4 = _mm_add_epi32(                           \
					_mm_and_si128(sp4, emask),             \
					_mm_srli_si128(sp4, 12));              \
			sp41 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp41, 4), emask),       \
					_mm_srli_si128(sp41, 8));              \
			sp42 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp42, 8), emask),       \
					_mm_srli_si128(sp42, 4));              \
			sp4 = _mm_add_epi32(                           \
					_mm_add_epi32(sp4, sp41), sp42);       \
			sp5 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 5]));                         \
			sp51 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 5])) + 1);                    \
			sp52 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 5])) + 2);                    \
			sp5 = _mm_add_epi32(                           \
					_mm_and_si128(sp5, emask),             \
					_mm_srli_si128(sp5, 12));              \
			sp51 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp51, 4), emask),       \
					_mm_srli_si128(sp51, 8));              \
			sp52 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp52, 8), emask),       \
					_mm_srli_si128(sp52, 4));              \
			sp5 = _mm_add_epi32(                           \
					_mm_add_epi32(sp5, sp51), sp52);       \
			sp5 = MM_SL1_S32(sp5);                         \
			sp6 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 6]));                         \
			sp61 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 6])) + 1);                    \
			sp62 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 6])) + 2);                    \
			sp6 = _mm_add_epi32(                           \
					_mm_and_si128(sp6, emask),             \
					_mm_srli_si128(sp6, 12));              \
			sp61 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp61, 4), emask),       \
					_mm_srli_si128(sp61, 8));              \
			sp62 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp62, 8), emask),       \
					_mm_srli_si128(sp62, 4));              \
			sp6 = _mm_add_epi32(                           \
					_mm_add_epi32(sp6, sp61), sp62);       \
			sp6 = MM_SL2_S32(sp6);                         \
			sp7 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 7]));                         \
			sp71 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 7])) + 1);                    \
			sp72 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 7])) + 2);                    \
			sp7 = _mm_add_epi32(                           \
					_mm_and_si128(sp7, emask),             \
					_mm_srli_si128(sp7, 12));              \
			sp71 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp71, 4), emask),       \
					_mm_srli_si128(sp71, 8));              \
			sp72 = _mm_add_epi32(                          \
					_mm_and_si128(                         \
					_mm_slli_si128(sp72, 8), emask),       \
					_mm_srli_si128(sp72, 4));              \
			sp7 = _mm_add_epi32(                           \
					_mm_add_epi32(sp7, sp71), sp72);       \
			sp7 = MM_SL3_S32(sp7);                         \
                                                           \
			sp0 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp1), sp0);   \
			sp1 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp1, zero),         \
					_mm_unpackhi_epi64(zero, sp2));        \
			sp2 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp2, zero), sp3);   \
			sp4 = _mm_or_si128(                            \
					_mm_unpackhi_epi64(zero, sp5), sp4);   \
			sp5 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp5, zero),         \
					_mm_unpackhi_epi64(zero, sp6));        \
			sp6 = _mm_or_si128(                            \
					_mm_unpacklo_epi64(sp6, zero), sp7);   \
			SAVE_SSE2(sp0, sp1);                           \
			SAVE_SSE2(sp2, sp4);                           \
			SAVE_SSE2(sp5, sp6);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[3] + sp[6] + sp[9]);           \
			SAVE(sp[1] + sp[4] + sp[7] + sp[10]);          \
			SAVE(sp[2] + sp[5] + sp[8] + sp[11]);          \
	    }                                                  \
	}                                                      \
/* ******************************************************** */ \
	static void FUNC(ch4_x1##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1;                                  \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 1); i += 2) {             \
			sp0 = LOAD_128((__m128i *)(sl + x_ind[i+ 0])); \
			sp1 = LOAD_128((__m128i *)(sl + x_ind[i+ 1])); \
			SAVE_SSE2(sp0, sp1);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0]);                                   \
			SAVE(sp[1]);                                   \
			SAVE(sp[2]);                                   \
			SAVE(sp[3]);                                   \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch4_x2##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1;                                  \
		__m128i sp01, sp11;                                \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 1); i += 2) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 1);                    \
			sp0 = _mm_add_epi32(sp0, sp01);                \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 1);                    \
			sp1 = _mm_add_epi32(sp1, sp11);                \
			SAVE_SSE2(sp0, sp1);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[4]);                           \
			SAVE(sp[1] + sp[5]);                           \
			SAVE(sp[2] + sp[6]);                           \
			SAVE(sp[3] + sp[7]);                           \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch4_x3##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2;                             \
		__m128i sp01, sp11, sp21;                          \
		__m128i sp02, sp12, sp22;                          \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 1); i += 2) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 1);                    \
			sp02 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 2);                    \
			sp0 = _mm_add_epi32(_mm_add_epi32(             \
					sp0, sp01), sp02);                     \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 1);                    \
			sp12 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 2);                    \
			sp1 = _mm_add_epi32(_mm_add_epi32(             \
					sp1, sp11), sp12);                     \
			SAVE_SSE2(sp0, sp1);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[4] + sp[8]);                   \
			SAVE(sp[1] + sp[5] + sp[9]);                   \
			SAVE(sp[2] + sp[6] + sp[10]);                  \
			SAVE(sp[3] + sp[7] + sp[11]);                  \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static void FUNC(ch4_x4##SUFF) (mlib_SubsAdap * ws,    \
	    mlib_u8 *sl)                                       \
	{                                                      \
	    INIT;                                              \
		__m128i *ddp = (__m128i *)buff;                    \
		__m128i sp0, sp1, sp2;                             \
		__m128i sp01, sp11, sp21;                          \
		__m128i sp02, sp12, sp22;                          \
		__m128i sp03, sp13, sp23;                          \
		__m128i dd, dd0, dd1;                              \
		__m128i d0, d1, d2, d3;                            \
		const __m128i zero = _mm_set1_epi8(0x00);          \
		for (i = 0; i < (xsize - 1); i += 2) {             \
			sp0 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 0]));                         \
			sp01 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 1);                    \
			sp02 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 2);                    \
			sp03 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 0])) + 3);                    \
			sp0 = _mm_add_epi32(_mm_add_epi32(             \
					sp0, sp01), _mm_add_epi32(             \
					sp02, sp03));                          \
			sp1 = LOAD_128((__m128i *)(sl +                \
					x_ind[i+ 1]));                         \
			sp11 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 1);                    \
			sp12 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 2);                    \
			sp13 = LOAD_128(((__m128i *)(sl +              \
					x_ind[i+ 1])) + 3);                    \
			sp1 = _mm_add_epi32(_mm_add_epi32(             \
					sp1, sp11), _mm_add_epi32(             \
					sp12, sp13));                          \
			SAVE_SSE2(sp0, sp1);                           \
		}                                                  \
	    for (; i < xsize; i++) {                           \
			sp = (YTYPE *) (sl + x_ind[i]);                \
			SAVE(sp[0] + sp[4] + sp[8] + sp[12]);          \
			SAVE(sp[1] + sp[5] + sp[9] + sp[13]);          \
			SAVE(sp[2] + sp[6] + sp[10] + sp[14]);         \
			SAVE(sp[3] + sp[7] + sp[11] + sp[15]);         \
	    }                                                  \
	}                                                      \
	                                                       \
/* ******************************************************** */ \
	static const func_x func_arr_x##SUFF[] = {             \
	    FUNC(ch1_x1##SUFF), FUNC(ch1_x2##SUFF),            \
		FUNC(ch1_x3##SUFF), FUNC(ch1_x4##SUFF),            \
		FUNC(ch2_x1##SUFF), FUNC(ch2_x2##SUFF),            \
		FUNC(ch2_x3##SUFF), FUNC(ch2_x4##SUFF),            \
		FUNC(ch3_x1##SUFF), FUNC(ch3_x2##SUFF),            \
		FUNC(ch3_x3##SUFF), FUNC(ch3_x4##SUFF),            \
		FUNC(ch4_x1##SUFF), FUNC(ch4_x2##SUFF),            \
		FUNC(ch4_x3##SUFF), FUNC(ch4_x4##SUFF)             \
	}

/* *********************************************************** */

FUNC_X(a, INIT_ADD, SAVE_ADD, SAVE_ADD_SSE2);
FUNC_X(m, INIT_MUL, SAVE_MUL, SAVE_MUL_SSE2);

#ifdef USE_SHIFT
FUNC_X(s, INIT_SHF, SAVE_SHF, SAVE_SHF_SSE2);
#endif /* USE_SHIFT */

/* *********************************************************** */
