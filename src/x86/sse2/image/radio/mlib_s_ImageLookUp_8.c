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

#pragma ident   "@(#)mlib_s_ImageLookUp_8.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUp - table lookup
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLookUp(mlib_image *dst,
 *                                   const mlib_image *src,
 *                                   const void **table);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      table   lookup table
 *
 * DESCRIPTION
 *      dst = table[src]
 */

#include <mlib_types.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	FN_NAME(F)	MLIB_S_IMAGE_LOOKUP_##F

/* *********************************************************** */

#undef STYPE
#define	STYPE mlib_u8

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */
#define	MLIB_S_IMAGE_LOOKUP_U8_2INDEX(e0, e1, e2, e3, e4, e5,     \
	e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, sp)         \
	{                                                         \
	    e0 = ((STYPE *)(sp))[0];                              \
	    e1 = ((STYPE *)(sp))[1];                              \
	    e2 = ((STYPE *)(sp))[2];                              \
	    e3 = ((STYPE *)(sp))[3];                              \
	    e4 = ((STYPE *)(sp))[4];                              \
	    e5 = ((STYPE *)(sp))[5];                              \
	    e6 = ((STYPE *)(sp))[6];                              \
	    e7 = ((STYPE *)(sp))[7];                              \
	    e8 = ((STYPE *)(sp))[8];                              \
	    e9 = ((STYPE *)(sp))[9];                              \
	    e10 = ((STYPE *)(sp))[10];                            \
	    e11 = ((STYPE *)(sp))[11];                            \
	    e12 = ((STYPE *)(sp))[12];                            \
	    e13 = ((STYPE *)(sp))[13];                            \
	    e14 = ((STYPE *)(sp))[14];                            \
	    e15 = ((STYPE *)(sp))[15];                            \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_2INDEX(e0, e1, e2, e3, e4, e5, \
	e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, sp)        \
	{                                                        \
		mlib_u32 s0, s1, s2, s3;                             \
		s0 = ((mlib_u32 *)sp)[0];                            \
		s1 = ((mlib_u32 *)sp)[1];                            \
		s2 = ((mlib_u32 *)sp)[2];                            \
		s3 = ((mlib_u32 *)sp)[3];                            \
	    e0 = s0 & 0xFF;                                      \
	    e1 = (s0 >> 8) & 0xFF;                               \
	    e2 = (s0 >> 16) & 0xFF;                              \
	    e3 = s0 >> 24;                                       \
	    e4 = s1 & 0xFF;                                      \
	    e5 = (s1 >> 8) & 0xFF;                               \
	    e6 = (s1 >> 16) & 0xFF;                              \
	    e7 = s1 >> 24;                                       \
	    e8 = s2 & 0xFF;                                      \
	    e9 = (s2 >> 8) & 0xFF;                               \
	    e10 = (s2 >> 16) & 0xFF;                             \
	    e11 = s2 >> 24;                                      \
	    e12 = s3 & 0xFF;                                     \
	    e13 = (s3 >> 8) & 0xFF;                              \
	    e14 = (s3 >> 16) & 0xFF;                             \
	    e15 = s3 >> 24;                                      \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(e0, e1, e2, e3, e4, e5,  \
	e6, e7, sp)                                               \
	{                                                         \
	    e0 = ((STYPE *)(sp))[0];                              \
	    e1 = ((STYPE *)(sp))[1];                              \
	    e2 = ((STYPE *)(sp))[2];                              \
	    e3 = ((STYPE *)(sp))[3];                              \
	    e4 = ((STYPE *)(sp))[4];                              \
	    e5 = ((STYPE *)(sp))[5];                              \
	    e6 = ((STYPE *)(sp))[6];                              \
	    e7 = ((STYPE *)(sp))[7];                              \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(e0, e1, e2, e3, e4, e5,  \
	e6, e7, sp)                                               \
	{                                                         \
		mlib_u32 s0, s1;                                      \
		s0 = ((mlib_u32 *)sp)[0];                            \
		s1 = ((mlib_u32 *)sp)[1];                            \
	    e0 = s0 & 0xFF;                                      \
	    e1 = (s0 >> 8) & 0xFF;                               \
	    e2 = (s0 >> 16) & 0xFF;                              \
	    e3 = s0 >> 24;                                       \
	    e4 = s1 & 0xFF;                                      \
	    e5 = (s1 >> 8) & 0xFF;                               \
	    e6 = (s1 >> 16) & 0xFF;                              \
	    e7 = s1 >> 24;                                       \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_U8_1L(                         \
			dp, sp, t0, t1, t2, t3)                         \
	{                                                       \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,        \
			e9, e10, e11, e12, e13, e14, e15;               \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                      \
			e0, e1, e2, e3, e4, e5,                         \
			e6, e7, e8, e9, e10, e11,                       \
			e12, e13, e14, e15, sp);                        \
		dd1 = _mm_setr_epi8(                                \
			t0[e0], t1[e1], t2[e2], t3[e3],                 \
			t0[e4], t1[e5], t2[e6], t3[e7],                 \
			t0[e8], t1[e9], t2[e10], t3[e11],               \
			t0[e12], t1[e13], t2[e14], t3[e15]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_U8_1L(                           \
			dp, sp, t0, t1, t2, t3)                           \
	{                                                         \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7, e8,          \
			e9, e10, e11, e12, e13, e14, e15;                 \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                        \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, e8, e9, e10, e11,                         \
			e12, e13, e14, e15, sp);                          \
		e0 = t0[e0] | (t1[e1] << 8) | (t2[e2] << 16) |        \
			(t3[e3] << 24);                                   \
		e4 = t0[e4] | (t1[e5] << 8) | (t2[e6] << 16) |        \
			(t3[e7] << 24);                                   \
		e8 = t0[e8] | (t1[e9] << 8) | (t2[e10] << 16) |       \
			(t3[e11] << 24);                                  \
		e12 = t0[e12] | (t1[e13] << 8) | (t2[e14] << 16) |    \
			(t3[e15] << 24);                                  \
		((mlib_u32 *)dp)[0] = e0;                             \
		((mlib_u32 *)dp)[1] = e4;                             \
		((mlib_u32 *)dp)[2] = e8;                             \
		((mlib_u32 *)dp)[3] = e12;                            \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U8_U8_1E(                \
			dp, sp, t0, t1, t2, t3, n)               \
	{                                                \
		for (k = 0; k < n - 3; k += 4) {             \
			((mlib_u8 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                \
			((mlib_u8 *)dp)[k + 1] =                 \
				t1[((STYPE *)sp)[k + 1]];            \
			((mlib_u8 *)dp)[k + 2] =                 \
				t2[((STYPE *)sp)[k + 2]];            \
			((mlib_u8 *)dp)[k + 3] =                 \
				t3[((STYPE *)sp)[k + 3]];            \
		}                                            \
		if (k < n) {                                 \
			((mlib_u8 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                \
			if (k < n - 1) {                         \
				((mlib_u8 *)dp)[k + 1] =             \
					t1[((STYPE *)sp)[k + 1]];        \
				if (k < n - 2) {                     \
					((mlib_u8 *)dp)[k + 2] =         \
						t2[((STYPE *)sp)[k + 2]];    \
				}                                    \
			}                                        \
		}                                            \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_U8_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	mlib_u8 *tab0 = (mlib_u8 *)table[0];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(U8_U8_1L)(dp1, sp1, tab0, tab0, tab0,
			    tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U8_U8_1E)(dp1, sp1, tab0, tab0, tab0,
			    tab0, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	mlib_u8 *tab0, *tab1;

	tab0 = (mlib_u8 *)table[0];
	tab1 = (mlib_u8 *)table[1];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(U8_U8_1L)(dp1, sp1, tab0, tab1, tab0,
			    tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U8_U8_1E)(dp1, sp1, tab0, tab1, tab0,
			    tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	mlib_u8 *tab0, *tab1, *tab2, *tab3;

	tab0 = (mlib_u8 *)table[0];
	tab1 = (mlib_u8 *)table[1];
	tab2 = (mlib_u8 *)table[2];
	tab3 = (mlib_u8 *)table[3];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 2;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(U8_U8_1L)(dp1, sp1, tab0, tab1, tab2,
			    tab3);
			sp1++;
		}
		if (n1 > 0) {
			FN_NAME(U8_U8_1E)(dp1, sp1, tab0, tab1, tab2,
			    tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_U8_3L(dp, sp, t0, t1, t2)  \
	{                                                     \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,      \
			e9, e10, e11, e12, e13, e14, e15;             \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                    \
			e0, e1, e2, e3, e4, e5,                       \
			e6, e7, e8, e9, e10, e11,                     \
			e12, e13, e14, e15, sp);                      \
		dd1 = _mm_setr_epi8(                              \
			t0[e0], t1[e1], t2[e2], t0[e3],               \
			t1[e4], t2[e5], t0[e6], t1[e7],               \
			t2[e8], t0[e9], t1[e10], t2[e11],             \
			t0[e12], t1[e13], t2[e14], t0[e15]);          \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_U8_3L(dp, sp, t0, t1, t2)  \
	{                                                     \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7, e8,      \
			e9, e10, e11, e12, e13, e14, e15;             \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                    \
			e0, e1, e2, e3, e4, e5,                       \
			e6, e7, e8, e9, e10, e11,                     \
			e12, e13, e14, e15, sp);                      \
		e0 = t0[e0] | (t1[e1] << 8) | (t2[e2] << 16) |    \
			(t0[e3] << 24);                               \
		e4 = t1[e4] | (t2[e5] << 8) | (t0[e6] << 16) |    \
			(t1[e7] << 24);                               \
		e8 = t2[e8] | (t0[e9] << 8) | (t1[e10] << 16) |   \
			(t2[e11] << 24);                              \
		e12 = t0[e12] | (t1[e13] << 8) | (t2[e14] << 16) |\
			(t0[e15] << 24);                              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e4;                         \
		((mlib_u32 *)dp)[2] = e8;                         \
		((mlib_u32 *)dp)[3] = e12;                        \
		dp++;                                             \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U8_U8_3E(dp, sp, t0, t1, t2, n)  \
	{                                                    \
		for (k = 0; k < n - 2; k += 3) {                 \
			((mlib_u8 *)dp)[k] =                         \
				t0[((STYPE *)sp)[k]];                    \
			((mlib_u8 *)dp)[k + 1] =                     \
				t1[((STYPE *)sp)[k + 1]];                \
			((mlib_u8 *)dp)[k + 2] =                     \
				t2[((STYPE *)sp)[k + 2]];                \
		}                                                \
		if (k < n) {                                     \
			((mlib_u8 *)dp)[k] =                         \
				t0[((STYPE *)sp)[k]];                    \
			if (k < n - 1) {                             \
				((mlib_u8 *)dp)[k + 1] =                 \
					t1[((STYPE *)sp)[k + 1]];            \
			}                                            \
		}                                                \
	}
/* *********************************************************** */


void
mlib_s_ImageLookUp_U8_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	mlib_u8 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
		tab0 = (mlib_u8 *)table[0];
		tab1 = (mlib_u8 *)table[1];
		tab2 = (mlib_u8 *)table[2];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 47; n1 -= 48) {
			FN_NAME(U8_U8_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1++;
			FN_NAME(U8_U8_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1++;
			FN_NAME(U8_U8_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1++;
		}
		if (n1 > 0) {
			FN_NAME(U8_U8_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_2L(dp, sp, t0)           \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,          \
			e9, e10, e11, e12, e13, e14, e15;                 \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                        \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, e8, e9, e10, e11,                         \
			e12, e13, e14, e15, sp);                          \
		dd0 = _mm_setr_epi16(                                 \
		((mlib_s16 *)t0)[e0], ((mlib_s16 *)t0)[e1],           \
		((mlib_s16 *)t0)[e2], ((mlib_s16 *)t0)[e3],           \
		((mlib_s16 *)t0)[e4], ((mlib_s16 *)t0)[e5],           \
		((mlib_s16 *)t0)[e6], ((mlib_s16 *)t0)[e7]);          \
		_mm_storeu_si128(dp, dd0);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi16(                                 \
		((mlib_s16 *)t0)[e8], ((mlib_s16 *)t0)[e9],           \
		((mlib_s16 *)t0)[e10], ((mlib_s16 *)t0)[e11],         \
		((mlib_s16 *)t0)[e12], ((mlib_s16 *)t0)[e13],         \
		((mlib_s16 *)t0)[e14], ((mlib_s16 *)t0)[e15]);        \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_2L(dp, sp, t0)           \
	{                                                         \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7, e8,          \
			e9, e10, e11, e12, e13, e14, e15;                 \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                        \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, e8, e9, e10, e11,                         \
			e12, e13, e14, e15, sp);                          \
		e0 = ((mlib_u16 *)t0)[e0] |                           \
			(((mlib_u16 *)t0)[e1] << 16);                     \
		e2 = ((mlib_u16 *)t0)[e2] |                           \
			(((mlib_u16 *)t0)[e3] << 16);                     \
		e4 = ((mlib_u16 *)t0)[e4] |                           \
			(((mlib_u16 *)t0)[e5] << 16);                     \
		e6 = ((mlib_u16 *)t0)[e6] |                           \
			(((mlib_u16 *)t0)[e7] << 16);                     \
		((mlib_u32 *)dp)[0] = e0;                             \
		((mlib_u32 *)dp)[1] = e2;                             \
		((mlib_u32 *)dp)[2] = e4;                             \
		((mlib_u32 *)dp)[3] = e6;                             \
		dp++;                                                 \
		e8 = ((mlib_u16 *)t0)[e8] |                           \
			(((mlib_u16 *)t0)[e9] << 16);                     \
		e10 = ((mlib_u16 *)t0)[e10] |                         \
			(((mlib_u16 *)t0)[e11] << 16);                    \
		e12 = ((mlib_u16 *)t0)[e12] |                         \
			(((mlib_u16 *)t0)[e13] << 16);                    \
		e14 = ((mlib_u16 *)t0)[e14] |                         \
			(((mlib_u16 *)t0)[e15] << 16);                    \
		((mlib_u32 *)dp)[0] = e8;                             \
		((mlib_u32 *)dp)[1] = e10;                            \
		((mlib_u32 *)dp)[2] = e12;                            \
		((mlib_u32 *)dp)[3] = e14;                            \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_2E(dp, sp, t0, n)        \
	{                                                         \
		for (k = 0; k < (n >> 1); k++) {                      \
			((mlib_s16 *)dp)[k] =                             \
				((mlib_s16 *)t0)[((STYPE *)sp)[k]];           \
		}                                                     \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_U8_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 *table)
{

/* dst address */
	__m128i *dp1, *dl1, dd0, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 31; n1 -= 32) {
			FN_NAME(SI_U8_U8_2L)(dp1, sp1, table);
			sp1++;
		}
		if (n1 > 0) {
			FN_NAME(SI_U8_U8_2E)(dp1, sp1, table, n1);
		}


		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_3L(                      \
			dp, sp, t0, t1, t2)                               \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,          \
			e9, e10, e11, e12, e13, e14, e15;                 \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                        \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, e8, e9, e10, e11,                         \
			e12, e13, e14, e15, sp);                          \
		dd1 = _mm_setr_epi8(                                  \
			tab0[e0], tab1[e0], tab2[e0], tab0[e1],           \
			tab1[e1], tab2[e1], tab0[e2], tab1[e2],           \
			tab2[e2], tab0[e3], tab1[e3], tab2[e3],           \
			tab0[e4], tab1[e4], tab2[e4], tab0[e5]);          \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
                                                              \
		dd1 = _mm_setr_epi8(                                  \
				tab1[e5], tab2[e5], tab0[e6], tab1[e6],       \
				tab2[e6], tab0[e7], tab1[e7], tab2[e7],       \
				tab0[e8], tab1[e8], tab2[e8], tab0[e9],       \
				tab1[e9], tab2[e9], tab0[e10], tab1[e10]);    \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
                                                              \
		dd1 = _mm_setr_epi8(                                  \
			tab2[e10], tab0[e11], tab1[e11], tab2[e11],       \
			tab0[e12], tab1[e12], tab2[e12], tab0[e13],       \
			tab1[e13], tab2[e13], tab0[e14], tab1[e14],       \
			tab2[e14], tab0[e15], tab1[e15], tab2[e15]);      \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_3L(                      \
			dp, sp, t0)                                       \
	{                                                         \
		mlib_u32 e0, e1, e2, e3;                              \
		mlib_u32 s0, s1;                                      \
		                                                      \
		s0 = ((mlib_u32 *)sp)[0];                             \
	    e0 = s0 & 0xFF;                                       \
	    e1 = (s0 >> 8) & 0xFF;                                \
		e0 = ((mlib_u32 *)t0)[e0];                            \
		e1 = ((mlib_u32 *)t0)[e1];                            \
		((mlib_u32 *)dp)[0] = e0 | (e1 << 24);                \
	    e2 = (s0 >> 16) & 0xFF;                               \
		e2 = ((mlib_u32 *)t0)[e2];                            \
		((mlib_u32 *)dp)[1] = (e1 >> 8) | (e2 << 16);         \
	    e3 = s0 >> 24;                                        \
		e3 = ((mlib_u32 *)t0)[e3];                            \
		((mlib_u32 *)dp)[2] = (e2 >> 16) | (e3 << 8);         \
		dp = (__m128i *)((mlib_u8 *)dp + 12);                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_3E(                      \
			dp, sp, t0, t1, t2, n)                            \
{                                                             \
	mlib_u32 e0;                                              \
	for (k = 0, j = 0; k < n; k += 3, j++) {                  \
		e0 = ((STYPE *)sp)[j];                                \
		((mlib_u8 *)dp)[k] = t0[e0];                          \
		((mlib_u8 *)dp)[k + 1] = t1[e0];                      \
		((mlib_u8 *)dp)[k + 2] = t2[e0];                      \
	}                                                         \
}

/* *********************************************************** */
void
mlib_s_ImageLookUpSI_U8_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table,
    const mlib_u8 *stbl)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */
	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_u8 *tab0, *tab1, *tab2;

	tab0 = (mlib_u8 *)table[0];
	tab1 = (mlib_u8 *)table[1];
	tab2 = (mlib_u8 *)table[2];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
		n1 = dw;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (; n1 > 11; n1 -= 12) {
			FN_NAME(SI_U8_U8_3L)(
				dp1, sp1, stbl);
			sp1 = (__m128i *)((mlib_u8 *)sp1 + 4);
		}
		if (n1 > 0) {
			FN_NAME(SI_U8_U8_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_4L(dp, sp, t0)          \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,          \
			e9, e10, e11, e12, e13, e14, e15;                 \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                        \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, e8, e9, e10, e11,                         \
			e12, e13, e14, e15, sp);                          \
	    dd1 = _mm_setr_epi32(                                 \
			((mlib_s32 *)t0)[e0], ((mlib_s32 *)t0)[e1],       \
			((mlib_s32 *)t0)[e2], ((mlib_s32 *)t0)[e3]);      \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
			((mlib_s32 *)t0)[e4], ((mlib_s32 *)t0)[e5],       \
			((mlib_s32 *)t0)[e6], ((mlib_s32 *)t0)[e7]);      \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
			((mlib_s32 *)t0)[e8], ((mlib_s32 *)t0)[e9],       \
			((mlib_s32 *)t0)[e10], ((mlib_s32 *)t0)[e11]);    \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
			((mlib_s32 *)t0)[e12], ((mlib_s32 *)t0)[e13],     \
			((mlib_s32 *)t0)[e14], ((mlib_s32 *)t0)[e15]);    \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_4L(dp, sp, t0)          \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,          \
			e9, e10, e11, e12, e13, e14, e15;                 \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                        \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, e8, e9, e10, e11,                         \
			e12, e13, e14, e15, sp);                          \
		((mlib_u32 *)dp)[0] = ((mlib_u32 *)t0)[e0];           \
		((mlib_u32 *)dp)[1] = ((mlib_u32 *)t0)[e1];           \
		((mlib_u32 *)dp)[2] = ((mlib_u32 *)t0)[e2];           \
		((mlib_u32 *)dp)[3] = ((mlib_u32 *)t0)[e3];           \
		dp++;                                                 \
		((mlib_u32 *)dp)[0] = ((mlib_u32 *)t0)[e4];           \
		((mlib_u32 *)dp)[1] = ((mlib_u32 *)t0)[e5];           \
		((mlib_u32 *)dp)[2] = ((mlib_u32 *)t0)[e6];           \
		((mlib_u32 *)dp)[3] = ((mlib_u32 *)t0)[e7];           \
		dp++;                                                 \
		((mlib_u32 *)dp)[0] = ((mlib_u32 *)t0)[e8];           \
		((mlib_u32 *)dp)[1] = ((mlib_u32 *)t0)[e9];           \
		((mlib_u32 *)dp)[2] = ((mlib_u32 *)t0)[e10];          \
		((mlib_u32 *)dp)[3] = ((mlib_u32 *)t0)[e11];          \
		dp++;                                                 \
		((mlib_u32 *)dp)[0] = ((mlib_u32 *)t0)[e12];          \
		((mlib_u32 *)dp)[1] = ((mlib_u32 *)t0)[e13];          \
		((mlib_u32 *)dp)[2] = ((mlib_u32 *)t0)[e14];          \
		((mlib_u32 *)dp)[3] = ((mlib_u32 *)t0)[e15];          \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_U8_4E(                      \
			dp, sp, ts, n)                                    \
	{                                                         \
		for (k = 0; k < (n >> 2); k++) {                      \
			((mlib_s32 *)dp)[k] =                             \
				((mlib_s32 *)ts)[((STYPE *)sp)[k]];           \
		}                                                     \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U8_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 *table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;


	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 2;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
		n1 = dw;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (; n1 > 63; n1 -= 64) {
			FN_NAME(SI_U8_U8_4L)(
				dp1, sp1, table);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_U8_U8_4E)(
				dp1, sp1, table, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_S16_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,  \
			e9, e10, e11, e12, e13, e14, e15;         \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, e8, e9, e10, e11,                 \
			e12, e13, e14, e15, sp);                  \
	    dd1 = _mm_setr_epi16(                         \
			t0[e0], t1[e1], t2[e2], t3[e3],           \
			t0[e4], t1[e5], t2[e6], t3[e7]);          \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	    dd1 = _mm_setr_epi16(                         \
			t0[e8], t1[e9], t2[e10], t3[e11],         \
			t0[e12], t1[e13], t2[e14], t3[e15]);      \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_S16_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7, e8,  \
			e9, e10, e11, e12, e13, e14, e15;         \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX(                \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, e8, e9, e10, e11,                 \
			e12, e13, e14, e15, sp);                  \
		e0 = ((mlib_u16 *)t0)[e0] |                   \
			(((mlib_u16 *)t1)[e1] << 16);             \
		e2 = ((mlib_u16 *)t2)[e2] |                   \
			(((mlib_u16 *)t3)[e3] << 16);             \
		e4 = ((mlib_u16 *)t0)[e4] |                   \
			(((mlib_u16 *)t1)[e5] << 16);             \
		e6 = ((mlib_u16 *)t2)[e6] |                   \
			(((mlib_u16 *)t3)[e7] << 16);             \
		((mlib_u32 *)dp)[0] = e0;                     \
		((mlib_u32 *)dp)[1] = e2;                     \
		((mlib_u32 *)dp)[2] = e4;                     \
		((mlib_u32 *)dp)[3] = e6;                     \
		dp++;                                         \
		e8 = ((mlib_u16 *)t0)[e8] |                   \
			(((mlib_u16 *)t1)[e9] << 16);             \
		e10 = ((mlib_u16 *)t2)[e10] |                 \
			(((mlib_u16 *)t3)[e11] << 16);            \
		e12 = ((mlib_u16 *)t0)[e12] |                 \
			(((mlib_u16 *)t1)[e13] << 16);            \
		e14 = ((mlib_u16 *)t2)[e14] |                 \
			(((mlib_u16 *)t3)[e15] << 16);            \
		((mlib_u32 *)dp)[0] = e8;                     \
		((mlib_u32 *)dp)[1] = e10;                    \
		((mlib_u32 *)dp)[2] = e12;                    \
		((mlib_u32 *)dp)[3] = e14;                    \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U8_S16_1E(                \
			dp, sp, t0, t1, t2, t3, n)                \
	{                                                 \
		for (k = 0; k < n - 3; k += 4) {              \
			((mlib_s16 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                 \
			((mlib_s16 *)dp)[k + 1] =                 \
				t1[((STYPE *)sp)[k + 1]];             \
			((mlib_s16 *)dp)[k + 2] =                 \
				t2[((STYPE *)sp)[k + 2]];             \
			((mlib_s16 *)dp)[k + 3] =                 \
				t3[((STYPE *)sp)[k + 3]];             \
		}                                             \
		if (k < n) {                                  \
			((mlib_s16 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                 \
			if (k < n - 1) {                          \
				((mlib_s16 *)dp)[k + 1] =             \
					t1[((STYPE *)sp)[k + 1]];         \
				if (k < n - 2) {                      \
					((mlib_s16 *)dp)[k + 2] =         \
						t2[((STYPE *)sp)[k + 2]];     \
				}                                     \
			}                                         \
		}                                             \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S16_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(U8_S16_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U8_S16_1E)(
				dp1, sp1, tab0, tab0, tab0, tab0, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S16_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(U8_S16_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U8_S16_1E)(
				dp1, sp1, tab0, tab1, tab0, tab1, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S16_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];
	mlib_s16 *tab3 = (mlib_s16 *)table[3];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 2;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(U8_S16_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U8_S16_1E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_S16_3L(                \
			dp, sp, t0, t1, t2)                       \
	{                                                 \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;      \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(             \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, sp);                              \
	    dd1 = _mm_setr_epi16(                         \
			t0[e0], t1[e1], t2[e2], t0[e3],           \
			t1[e4], t2[e5], t0[e6], t1[e7]);          \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_S16_3L(                \
			dp, sp, t0, t1, t2)                       \
	{                                                 \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7;      \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(             \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, sp);                              \
		e0 = ((mlib_u16 *)t0)[e0] |                   \
			(((mlib_u16 *)t1)[e1] << 16);             \
		e2 = ((mlib_u16 *)t2)[e2] |                   \
			(((mlib_u16 *)t0)[e3] << 16);             \
		e4 = ((mlib_u16 *)t1)[e4] |                   \
			(((mlib_u16 *)t2)[e5] << 16);             \
		e6 = ((mlib_u16 *)t0)[e6] |                   \
			(((mlib_u16 *)t1)[e7] << 16);             \
		((mlib_u32 *)dp)[0] = e0;                     \
		((mlib_u32 *)dp)[1] = e2;                     \
		((mlib_u32 *)dp)[2] = e4;                     \
		((mlib_u32 *)dp)[3] = e6;                     \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U8_S16_3E(               \
			dp, sp, t0, t1, t2, n)                   \
	{                                                \
                                                     \
		for (k = 0; k < n - 2; k += 3) {             \
			((mlib_s16 *)dp)[k] =                    \
				t0[((STYPE *)sp)[k]];                \
			((mlib_s16 *)dp)[k + 1] =                \
				t1[((STYPE *)sp)[k + 1]];            \
			((mlib_s16 *)dp)[k + 2] =                \
				t2[((STYPE *)sp)[k + 2]];            \
		}                                            \
		if (k < n) {                                 \
			((mlib_s16 *)dp)[k] =                    \
				t0[((STYPE *)sp)[k]];                \
			if (k < n - 1) {                         \
				((mlib_s16 *)dp)[k + 1] =            \
					t1[((STYPE *)sp)[k + 1]];        \
			}                                        \
		}                                            \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S16_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s16 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
	    tab0 = (mlib_s16 *)table[0];
	    tab1 = (mlib_s16 *)table[1];
	    tab2 = (mlib_s16 *)table[2];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (n1 = dw; n1 > 23; n1 -= 24) {
			FN_NAME(U8_S16_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(U8_S16_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(U8_S16_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }
		if (n1 > 0) {
			FN_NAME(U8_S16_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);

		}

	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_2L(dp, sp, t0)         \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
	    dd1 = _mm_setr_epi32(                                 \
			((mlib_s32 *)t0)[e0], ((mlib_s32 *)t0)[e1],       \
			((mlib_s32 *)t0)[e2], ((mlib_s32 *)t0)[e3]);      \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
			((mlib_s32 *)t0)[e4], ((mlib_s32 *)t0)[e5],       \
			((mlib_s32 *)t0)[e6], ((mlib_s32 *)t0)[e7]);      \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_2L(dp, sp, t0)         \
	{                                                         \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
		e0 = ((mlib_u32 *)t0)[e0];                            \
		e1 = ((mlib_u32 *)t0)[e1];                            \
		e2 = ((mlib_u32 *)t0)[e2];                            \
		e3 = ((mlib_u32 *)t0)[e3];                            \
		((mlib_u32 *)dp)[0] = e0;                             \
		((mlib_u32 *)dp)[1] = e1;                             \
		((mlib_u32 *)dp)[2] = e2;                             \
		((mlib_u32 *)dp)[3] = e3;                             \
		dp++;                                                 \
		e4 = ((mlib_u32 *)t0)[e4];                            \
		e5 = ((mlib_u32 *)t0)[e5];                            \
		e6 = ((mlib_u32 *)t0)[e6];                            \
		e7 = ((mlib_u32 *)t0)[e7];                            \
		((mlib_u32 *)dp)[0] = e4;                             \
		((mlib_u32 *)dp)[1] = e5;                             \
		((mlib_u32 *)dp)[2] = e6;                             \
		((mlib_u32 *)dp)[3] = e7;                             \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_2E(                     \
			dp, sp, t0, n)                                    \
	{                                                         \
		for (k = 0; k < (n >> 1); k++) {                      \
			((mlib_s32 *)dp)[k] =                             \
				((mlib_s32 *)t0)[((STYPE *)sp)[k]];           \
		}                                                     \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U8_S16_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 *table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(SI_U8_S16_2L)(dp1, sp1, table);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_U8_S16_2E)(
				dp1, sp1, table, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_3L(dp, sp, t0, t1, t2) \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
		dd1 = _mm_setr_epi16(                                 \
			tab0[e0], tab1[e0], tab2[e0], tab0[e1],           \
			tab1[e1], tab2[e1], tab0[e2], tab1[e2]);          \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
		dd1 = _mm_setr_epi16(                                 \
			tab2[e2], tab0[e3], tab1[e3], tab2[e3],           \
			tab0[e4], tab1[e4], tab2[e4], tab0[e5]);          \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
		dd1 = _mm_setr_epi16(                                 \
			tab1[e5], tab2[e5], tab0[e6], tab1[e6],           \
			tab2[e6], tab0[e7], tab1[e7], tab2[e7]);          \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_3L(dp, sp, t0, t1, t2) \
	{                                                         \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		mlib_u32 w0, w1, w2, w3;                              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
		w0 = ((mlib_u16 *)t0)[e0] |                           \
			(((mlib_u16 *)t1)[e0] << 16);                     \
		w1 = ((mlib_u16 *)t2)[e0] |                           \
			(((mlib_u16 *)t0)[e1] << 16);                     \
		w2 = ((mlib_u16 *)t1)[e1] |                           \
			(((mlib_u16 *)t2)[e1] << 16);                     \
		w3 = ((mlib_u16 *)t0)[e2] |                           \
			(((mlib_u16 *)t1)[e2] << 16);                     \
		((mlib_u32 *)dp)[0] = w0;                             \
		((mlib_u32 *)dp)[1] = w1;                             \
		((mlib_u32 *)dp)[2] = w2;                             \
		((mlib_u32 *)dp)[3] = w3;                             \
		dp++;                                                 \
		w0 = ((mlib_u16 *)t2)[e2] |                           \
			(((mlib_u16 *)t0)[e3] << 16);                     \
		w1 = ((mlib_u16 *)t1)[e3] |                           \
			(((mlib_u16 *)t2)[e3] << 16);                     \
		w2 = ((mlib_u16 *)t0)[e4] |                           \
			(((mlib_u16 *)t1)[e4] << 16);                     \
		w3 = ((mlib_u16 *)t2)[e4] |                           \
			(((mlib_u16 *)t0)[e5] << 16);                     \
		((mlib_u32 *)dp)[0] = w0;                             \
		((mlib_u32 *)dp)[1] = w1;                             \
		((mlib_u32 *)dp)[2] = w2;                             \
		((mlib_u32 *)dp)[3] = w3;                             \
		dp++;                                                 \
		w0 = ((mlib_u16 *)t1)[e5] |                           \
			(((mlib_u16 *)t2)[e5] << 16);                     \
		w1 = ((mlib_u16 *)t0)[e6] |                           \
			(((mlib_u16 *)t1)[e6] << 16);                     \
		w2 = ((mlib_u16 *)t2)[e6] |                           \
			(((mlib_u16 *)t0)[e7] << 16);                     \
		w3 = ((mlib_u16 *)t1)[e7] |                           \
			(((mlib_u16 *)t2)[e7] << 16);                     \
		((mlib_u32 *)dp)[0] = w0;                             \
		((mlib_u32 *)dp)[1] = w1;                             \
		((mlib_u32 *)dp)[2] = w2;                             \
		((mlib_u32 *)dp)[3] = w3;                             \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_3E(                     \
			dp, sp, t0, t1, t2, n)                            \
{                                                             \
		for (k = 0, j = 0; k < n; k += 3, j++) {              \
			((mlib_s16 *)dp)[k] =                             \
				t0[((STYPE *)sp)[j]];                         \
			((mlib_s16 *)dp)[k + 1] =                         \
				t1[((STYPE *)sp)[j]];                         \
			((mlib_s16 *)dp)[k + 2] =                         \
				t2[((STYPE *)sp)[j]];                         \
		}                                                     \
}

/* *********************************************************** */
void
mlib_s_ImageLookUpSI_U8_S16_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
	    n1 = dw;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (; n1 > 23; n1 -= 24) {
			FN_NAME(SI_U8_S16_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }

		if (n1 > 0) {
			MLIB_S_IMAGE_LOOKUP_SI_U8_S16_3E(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */
#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_4L(dp, sp, t0)         \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
	    dd1 = _mm_setr_epi64(                                 \
		((__m64 *)t0)[e0], ((__m64 *)t0)[e1]);                \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi64(                                 \
		((__m64 *)t0)[e2], ((__m64 *)t0)[e3]);                \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi64(                                 \
		((__m64 *)t0)[e4], ((__m64 *)t0)[e5]);                \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi64(                                 \
		((__m64 *)t0)[e6], ((__m64 *)t0)[e7]);                \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}
#else /* C version for LookUp, better than STDC version */
#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_4L(dp, sp, t0)         \
	{                                                         \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e0];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e1];                 \
		dp++;                                                 \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e2];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e3];                 \
		dp++;                                                 \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e4];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e5];                 \
		dp++;                                                 \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e6];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e7];                 \
		dp++;                                                 \
	}
#endif /* End of LOOKUP_SSE2 */


/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S16_4E(dp, sp, ts, n)       \
	{                                                         \
		for (k = 0; k < (n >> 2); k++) {                      \
			((__m64 *)dp)[k] =                                \
				((__m64 *)ts)[((STYPE *)sp)[k]];              \
		}                                                     \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U8_S16_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 *table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 2;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
	    n1 = dw;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (; n1 > 31; n1 -= 32) {
			FN_NAME(SI_U8_S16_4L)(dp1, sp1, table);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }
		if (n1 > 0) {
			FN_NAME(SI_U8_S16_4E)(
				dp1, sp1, table, n1);

		}

	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_S32_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;      \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(             \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, sp);                              \
	    dd1 = _mm_setr_epi32(                         \
			t0[e0], t1[e1], t2[e2], t3[e3]);          \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	    dd1 = _mm_setr_epi32(                         \
			t0[e4], t1[e5], t2[e6], t3[e7]);          \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}
#else /* C version for LookUp, better than STDC version */
#define	MLIB_S_IMAGE_LOOKUP_U8_S32_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
		mlib_u32 e0, e1, e2, e3, e4, e5, e6, e7;      \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(             \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, sp);                              \
		((mlib_s32 *)dp)[0] = t0[e0];                 \
		((mlib_s32 *)dp)[1] = t1[e1];                 \
		((mlib_s32 *)dp)[2] = t2[e2];                 \
		((mlib_s32 *)dp)[3] = t3[e3];                 \
		dp++;                                         \
		((mlib_s32 *)dp)[0] = t0[e4];                 \
		((mlib_s32 *)dp)[1] = t1[e5];                 \
		((mlib_s32 *)dp)[2] = t2[e6];                 \
		((mlib_s32 *)dp)[3] = t3[e7];                 \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U8_S32_1E(                \
			dp, sp, t0, t1, t2, t3, n)                \
	{                                                 \
		for (k = 0; k < n - 3; k += 4) {              \
			((mlib_s32 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                 \
			((mlib_s32 *)dp)[k + 1] =                 \
				t1[((STYPE *)sp)[k + 1]];             \
			((mlib_s32 *)dp)[k + 2] =                 \
				t2[((STYPE *)sp)[k + 2]];             \
			((mlib_s32 *)dp)[k + 3] =                 \
				t3[((STYPE *)sp)[k + 3]];             \
		}                                             \
		if (k < n) {                                  \
			((mlib_s32 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                 \
			if (k < n - 1) {                          \
				((mlib_s32 *)dp)[k + 1] =             \
					t1[((STYPE *)sp)[k + 1]];         \
				if (k < n - 2) {                      \
					((mlib_s32 *)dp)[k + 2] =         \
						t2[((STYPE *)sp)[k + 2]];     \
				}                                     \
			}                                         \
		}                                             \
	}


/* *********************************************************** */


void
mlib_s_ImageLookUp_U8_S32_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 *tab0 = (mlib_s32 *)table[0];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 7; n1 -= 8) {
			FN_NAME(U8_S32_1L)
				(dp1, sp1, tab0, tab0, tab0, tab0);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(U8_S32_1E)(
				dp1, sp1, tab0, tab0,
				tab0, tab0, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S32_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 7; n1 -= 8) {
			FN_NAME(U8_S32_1L)
				(dp1, sp1, tab0, tab1, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(U8_S32_1E)(
				dp1, sp1, tab0, tab1,
				tab0, tab1, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S32_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];
	mlib_s32 *tab3 = (mlib_s32 *)table[3];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 2;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 7; n1 -= 8) {
			FN_NAME(U8_S32_1L)
				(dp1, sp1, tab0, tab1, tab2, tab3);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(U8_S32_1E)(
				dp1, sp1, tab0, tab1,
				tab2, tab3, n1);

		}


		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}


/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U8_S32_3L(                \
			dp, sp, t0, t1, t2)                       \
	{                                                 \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;      \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(             \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, sp);                              \
	    dd1 = _mm_setr_epi32(                         \
				t0[e0], t1[e1], t2[e2], t0[e3]);      \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	    dd1 = _mm_setr_epi32(                         \
				t1[e4], t2[e5], t0[e6], t1[e7]);      \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U8_S32_3L(                \
			dp, sp, t0, t1, t2)                       \
	{                                                 \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;      \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(             \
			e0, e1, e2, e3, e4, e5,                   \
			e6, e7, sp);                              \
		((mlib_s32 *)dp)[0] = t0[e0];                 \
		((mlib_s32 *)dp)[1] = t1[e1];                 \
		((mlib_s32 *)dp)[2] = t2[e2];                 \
		((mlib_s32 *)dp)[3] = t0[e3];                 \
		dp++;                                         \
		((mlib_s32 *)dp)[0] = t1[e4];                 \
		((mlib_s32 *)dp)[1] = t2[e5];                 \
		((mlib_s32 *)dp)[2] = t0[e6];                 \
		((mlib_s32 *)dp)[3] = t1[e7];                 \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U8_S32_3E(                \
			dp, sp, t0, t1, t2, n)                    \
	{                                                 \
		for (k = 0; k < n - 2; k += 3) {              \
			((mlib_s32 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                 \
			((mlib_s32 *)dp)[k + 1] =                 \
				t1[((STYPE *)sp)[k + 1]];             \
			((mlib_s32 *)dp)[k + 2] =                 \
				t2[((STYPE *)sp)[k + 2]];             \
		}                                             \
		if (k < n) {                                  \
			((mlib_s32 *)dp)[k] =                     \
				t0[((STYPE *)sp)[k]];                 \
			if (k < n - 1) {                          \
				((mlib_s32 *)dp)[k + 1] =             \
					t1[((STYPE *)sp)[k + 1]];         \
			}                                         \
		}                                             \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U8_S32_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s32 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
	    tab0 = (mlib_s32 *)table[0];
	    tab1 = (mlib_s32 *)table[1];
	    tab2 = (mlib_s32 *)table[2];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (n1 = dw; n1 > 23; n1 -= 24) {
			FN_NAME(U8_S32_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(U8_S32_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(U8_S32_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }

		if (n1 > 0) {
			FN_NAME(U8_S32_3E)(
				dp1, sp1, tab0,
				tab1, tab2, n1);
		}

	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_2L(dp, sp, t0)          \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
	    dd1 = _mm_setr_epi64(((__m64 *) t0)[e0],              \
			((__m64 *) t0)[e1]);                              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi64(((__m64 *) t0)[e2],              \
			((__m64 *) t0)[e3]);                              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi64(((__m64 *) t0)[e4],              \
			((__m64 *) t0)[e5]);                              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi64(((__m64 *) t0)[e6],              \
			((__m64 *) t0)[e7]);                              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_2L(dp, sp, t0)          \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e0];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e1];                 \
		dp++;                                                 \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e2];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e3];                 \
		dp++;                                                 \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e4];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e5];                 \
		dp++;                                                 \
		((__m64 *)dp)[0] = ((__m64 *)t0)[e6];                 \
		((__m64 *)dp)[1] = ((__m64 *)t0)[e7];                 \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_2E(dp, sp, t0, n)       \
	{                                                         \
		for (k = 0; k < (n >> 1); k++) {                      \
			((__m64 *)dp)[k] =                                \
				((__m64 *)t0)[((STYPE *)sp)[k]];              \
		}                                                     \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U8_S32_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 *table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw <<= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (n1 = dw; n1 > 15; n1 -= 16) {
			FN_NAME(SI_U8_S32_2L)(dp1, sp1, table);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }

		if (n1 > 0) {

			FN_NAME(SI_U8_S32_2E)(
				dp1, sp1, table, n1);
		}

	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_3L(dp, sp, t0, t1, t2)  \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
                                                              \
	    dd1 = _mm_setr_epi32(                                 \
				t0[e0], t1[e0], t2[e0], t0[e1]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
				t1[e1], t2[e1], t0[e2], t1[e2]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
				t2[e2], t0[e3], t1[e3], t2[e3]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
				t0[e4], t1[e4], t2[e4], t0[e5]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
				t1[e5], t2[e5], t0[e6], t1[e6]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	    dd1 = _mm_setr_epi32(                                 \
				t2[e6], t0[e7], t1[e7], t2[e7]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_3L(dp, sp, t0, t1, t2)  \
	{                                                         \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;              \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                     \
			e0, e1, e2, e3, e4, e5,                           \
			e6, e7, sp);                                      \
                                                              \
		((mlib_s32 *)dp)[0] = t0[e0];                         \
		((mlib_s32 *)dp)[1] = t1[e0];                         \
		((mlib_s32 *)dp)[2] = t2[e0];                         \
		((mlib_s32 *)dp)[3] = t0[e1];                         \
		dp++;                                                 \
		((mlib_s32 *)dp)[0] = t1[e1];                         \
		((mlib_s32 *)dp)[1] = t2[e1];                         \
		((mlib_s32 *)dp)[2] = t0[e2];                         \
		((mlib_s32 *)dp)[3] = t1[e2];                         \
		dp++;                                                 \
		((mlib_s32 *)dp)[0] = t2[e2];                         \
		((mlib_s32 *)dp)[1] = t0[e3];                         \
		((mlib_s32 *)dp)[2] = t1[e3];                         \
		((mlib_s32 *)dp)[3] = t2[e3];                         \
		dp++;                                                 \
		((mlib_s32 *)dp)[0] = t0[e4];                         \
		((mlib_s32 *)dp)[1] = t1[e4];                         \
		((mlib_s32 *)dp)[2] = t2[e4];                         \
		((mlib_s32 *)dp)[3] = t0[e5];                         \
		dp++;                                                 \
		((mlib_s32 *)dp)[0] = t1[e5];                         \
		((mlib_s32 *)dp)[1] = t2[e5];                         \
		((mlib_s32 *)dp)[2] = t0[e6];                         \
		((mlib_s32 *)dp)[3] = t1[e6];                         \
		dp++;                                                 \
		((mlib_s32 *)dp)[0] = t2[e6];                         \
		((mlib_s32 *)dp)[1] = t0[e7];                         \
		((mlib_s32 *)dp)[2] = t1[e7];                         \
		((mlib_s32 *)dp)[3] = t2[e7];                         \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */


/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_3E(                     \
			dp, sp, t0, t1, t2, n)                            \
	{                                                         \
		for (k = 0, j = 0; k < n; k += 3, j++) {              \
			((mlib_s32 *)dp)[k] =                             \
				t0[((STYPE *)sp)[j]];                         \
			((mlib_s32 *)dp)[k + 1] =                         \
				t1[((STYPE *)sp)[j]];                         \
			((mlib_s32 *)dp)[k + 2] =                         \
				t2[((STYPE *)sp)[j]];                         \
		}                                                     \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U8_S32_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7, e8,
		e9, e10, e11, e12, e13, e14, e15;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
	    n1 = dw;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (; n1 > 23; n1 -= 24) {
			FN_NAME(SI_U8_S32_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }

		if (n1 > 0) {
			FN_NAME(SI_U8_S32_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);

		}


	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}

}

/* *********************************************************** */

#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_4L(dp, sp, t0)        \
	{                                                       \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;            \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                   \
			e0, e1, e2, e3, e4, e5, e6, e7, sp);            \
		                                                    \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e0));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e1));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e2));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e3));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e4));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e5));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e6));           \
		dp++;                                               \
		_mm_storeu_si128(dp,                                \
			_mm_loadu_si128((__m128i *)t0 + e7));           \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_4L(dp, sp, t0)        \
	{                                                       \
		mlib_s32 e0, e1, e2, e3, e4, e5, e6, e7;            \
		MLIB_S_IMAGE_LOOKUP_U8_2INDEX_64(                   \
			e0, e1, e2, e3, e4, e5, e6, e7, sp);            \
		                                                    \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e0 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e0 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e1 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e1 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e2 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e2 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e3 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e3 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e4 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e4 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e5 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e5 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e6 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e6 + 1];       \
		dp++;                                               \
		((__m64 *)dp)[0] = ((__m64 *)t0)[2 * e7 + 0];       \
		((__m64 *)dp)[1] = ((__m64 *)t0)[2 * e7 + 1];       \
		dp++;                                               \
	}


#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U8_S32_4E(dp, sp, ts, n)     \
	{                                                       \
		for (k = 0, j = 0; k < n; k++, j++) {               \
			((__m64 *)dp)[k * 2 + 0] =                      \
				((__m64 *)ts)[2 * ((STYPE *)sp)[k] + 0];    \
			((__m64 *)dp)[k * 2 + 1] =                      \
				((__m64 *)ts)[2 * ((STYPE *)sp)[k] + 1];    \
		}                                                   \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U8_S32_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 *table)
{

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1, k, j;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	    for (n1 = dw; n1 > 7; n1 -= 8) {
			FN_NAME(SI_U8_S32_4L)(dp1, sp1, table);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
	    }

		if (n1 > 0) {
			FN_NAME(SI_U8_S32_4E)(
				dp1, sp1, table, n1);

		}

	    dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	    sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_C_IMAGELOOKUP(DTYPE, STYPE, TABLE)               \
	{                                                         \
	    mlib_s32 i, j, k;                                     \
	                                                          \
	    if (xsize < 2) {                                      \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) { \
		    for (k = 0; k < csize; k++) {                     \
			DTYPE *da = dst + k;                              \
			STYPE *sa = src + k;                              \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
	                                                          \
			for (i = 0; i < xsize;                            \
			    i++, da += csize, sa += csize)                \
			    *da = tab[*sa];                               \
		    }                                                 \
		}                                                     \
	    } else {                                              \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) { \
		    for (k = 0; k < csize; k++) {                     \
			DTYPE *da = dst + k;                              \
			STYPE *sa = src + k;                              \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
			mlib_s32 s0, t0, s1, t1;                          \
	                                                          \
			s0 = (mlib_s32)sa[0];                             \
			s1 = (mlib_s32)sa[csize];                         \
			sa += 2 * csize;                                  \
			for (i = 0; i < xsize - 3;                        \
			    i += 2, da += 2 * csize, sa += 2 * csize) {   \
			    t0 = (mlib_s32)tab[s0];                       \
			    t1 = (mlib_s32)tab[s1];                       \
			    s0 = (mlib_s32)sa[0];                         \
			    s1 = (mlib_s32)sa[csize];                     \
			    da[0] = (DTYPE) t0;                           \
			    da[csize] = (DTYPE) t1;                       \
			}                                                 \
			t0 = (mlib_s32)tab[s0];                           \
			t1 = (mlib_s32)tab[s1];                           \
			da[0] = (DTYPE) t0;                               \
			da[csize] = (DTYPE) t1;                           \
			if (xsize & 1)                                    \
			    da[2 * csize] = tab[sa[0]];                   \
		    }                                                 \
		}                                                     \
	    }                                                     \
	}

/* *********************************************************** */

#define	READ_U8_U8_ALIGN(table0, table1, table2, table3)        \
	t3 = table0[s0 & 0xFF];                                 \
	t2 = table1[s0 >> 8];                                   \
	t1 = table2[s1 & 0xFF];                                 \
	t0 = table3[s1 >> 8];

/* *********************************************************** */

#define	READ_U8_U8_NOTALIGN(table0, table1, table2, table3)     \
	t3 = table0[s0 >> 8];                                   \
	t2 = table1[s1 & 0xFF];                                 \
	t1 = table2[s1 >> 8];                                   \
	t0 = table3[s2 & 0xFF];

/* *********************************************************** */

void
mlib_c_ImageLookUp_U8_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_u8 **table)
{
	if (xsize * csize < 9) {
		MLIB_C_IMAGELOOKUP(mlib_u8,
		    const mlib_u8,
		    table)
	} else if (
	    csize == 1) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *da;
			mlib_u16 *sa;
			mlib_u8 *tab = (mlib_u8 *)table[0];
			mlib_u32 s0, s1, s2, t0, t1, t2, t3, t;
			mlib_s32 off;
			mlib_s32 size = xsize;
			mlib_u8 *dp = dst;
			const mlib_u8 *sp = src;

			off = (mlib_s32)((4 - ((mlib_addr)dst & 3)) & 3);

			for (i = 0; i < off; i++, sp++) {
				(*dp++) = tab[sp[0]];
				size--;
			}

			da = (mlib_u32 *)dp;

			if (((mlib_addr)sp & 1) == 0) {
				sa = (mlib_u16 *)sp;

				s0 = sa[0];
				s1 = sa[1];
				sa += 2;

				for (i = 0; i < size - 7; i += 4,
						da++, sa += 2) {
					READ_U8_U8_ALIGN(tab, tab, tab, tab)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					s0 = sa[0];
					s1 = sa[1];
					da[0] = t;
				}

				READ_U8_U8_ALIGN(tab, tab, tab, tab)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
				sp = (mlib_u8 *)sa;
				i += 4;
				for (; i < size; i++, dp++, sp++)
					dp[0] = tab[sp[0]];

			} else {
				sa = (mlib_u16 *)(sp - 1);

				s0 = sa[0];
				s1 = sa[1];
				s2 = sa[2];
				sa += 3;

				for (i = 0; i < size - 8; i += 4,
						da++, sa += 2) {
					READ_U8_U8_NOTALIGN(tab, tab, tab, tab)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					s0 = s2;
					s1 = sa[0];
					s2 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_NOTALIGN(tab, tab, tab, tab)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
#ifdef _LITTLE_ENDIAN
				(*dp++) = tab[s2 >> 8];
#else
				(*dp++) = tab[s2 & 0xFF];
#endif
				sp = (mlib_u8 *)sa;
				i += 5;
				for (; i < size; i++, dp++, sp++)
					dp[0] = tab[sp[0]];
			}
		}
	} else if (csize == 2) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *da;
			mlib_u16 *sa;
			mlib_u8 *tab0 = (mlib_u8 *)table[0];
			mlib_u8 *tab1 = (mlib_u8 *)table[1];
			mlib_u8 *tab;
			mlib_u32 s0, s1, s2, t0, t1, t2, t3, t;
			mlib_s32 off;
			mlib_s32 size = xsize * 2;
			mlib_u8 *dp = dst;
			const mlib_u8 *sp = src;

			off = (mlib_s32)((4 - ((mlib_addr)dst & 3)) & 3);

			for (i = 0; i < off - 1; i += 2, sp += 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				size -= 2;
			}

			if ((off & 1) != 0) {
				(*dp++) = tab0[sp[0]];
				size--;
				sp++;
				tab = tab0;
				tab0 = tab1;
				tab1 = tab;
			}

			da = (mlib_u32 *)dp;

			if (((mlib_addr)sp & 1) == 0) {
				sa = (mlib_u16 *)sp;

				s0 = sa[0];
				s1 = sa[1];
				sa += 2;

				for (i = 0; i < size - 7; i += 4,
						da++, sa += 2) {
					READ_U8_U8_ALIGN(tab0, tab1, tab0, tab1)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					s0 = sa[0];
					s1 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_ALIGN(tab0, tab1, tab0, tab1)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
				sp = (mlib_u8 *)sa;
				i += 4;

				for (; i < size - 1; i += 2, sp += 2) {
					(*dp++) = tab0[sp[0]];
					(*dp++) = tab1[sp[1]];
				}

				if (i < size)
					*dp = tab0[(*sp)];

			} else {
				sa = (mlib_u16 *)(sp - 1);

				s0 = sa[0];
				s1 = sa[1];
				s2 = sa[2];
				sa += 3;

				for (i = 0; i < size - 8; i += 4,
						da++, sa += 2) {
					READ_U8_U8_NOTALIGN(tab0, tab1, tab0,
					    tab1)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					s0 = s2;
					s1 = sa[0];
					s2 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_NOTALIGN(tab0, tab1, tab0, tab1)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
#ifdef _LITTLE_ENDIAN
				(*dp++) = tab0[s2 >> 8];
#else
				(*dp++) = tab0[s2 & 0xFF];
#endif
				sp = (mlib_u8 *)sa;
				i += 5;

				for (; i < size - 1; i += 2, sp += 2) {
					(*dp++) = tab1[sp[0]];
					(*dp++) = tab0[sp[1]];
				}

				if (i < size)
					*dp = tab1[(*sp)];
			}
		}
	} else if (csize == 3) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *da;
			mlib_u16 *sa;
			mlib_u8 *tab0 = (mlib_u8 *)table[0];
			mlib_u8 *tab1 = (mlib_u8 *)table[1];
			mlib_u8 *tab2 = (mlib_u8 *)table[2];
			mlib_u8 *tab;
			mlib_u32 s0, s1, s2, t0, t1, t2, t3, t;
			mlib_s32 off;
			mlib_s32 size = xsize * 3;
			mlib_u8 *dp = dst;
			const mlib_u8 *sp = src;

			off = (mlib_s32)((4 - ((mlib_addr)dst & 3)) & 3);

			if (off == 1) {
				(*dp++) = tab0[sp[0]];
				tab = tab0;
				tab0 = tab1;
				tab1 = tab2;
				tab2 = tab;
				size--;
				sp++;
			} else if (off == 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				tab = tab2;
				tab2 = tab1;
				tab1 = tab0;
				tab0 = tab;
				size -= 2;
				sp += 2;
			} else if (off == 3) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				(*dp++) = tab2[sp[2]];
				size -= 3;
				sp += 3;
			}

			da = (mlib_u32 *)dp;

			if (((mlib_addr)sp & 1) == 0) {
				sa = (mlib_u16 *)sp;

				s0 = sa[0];
				s1 = sa[1];
				sa += 2;

				for (i = 0; i < size - 7; i += 4,
						da++, sa += 2) {
					READ_U8_U8_ALIGN(tab0, tab1, tab2, tab0)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					tab = tab0;
					tab0 = tab1;
					tab1 = tab2;
					tab2 = tab;
					s0 = sa[0];
					s1 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_ALIGN(tab0, tab1, tab2, tab0)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
				sp = (mlib_u8 *)sa;
				i += 4;

				if (i < size) {
					(*dp++) = tab1[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					(*dp++) = tab2[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					(*dp++) = tab0[(*sp)];
				}

			} else {
				sa = (mlib_u16 *)(sp - 1);

				s0 = sa[0];
				s1 = sa[1];
				s2 = sa[2];
				sa += 3;

				for (i = 0; i < size - 8; i += 4,
						da++, sa += 2) {
					READ_U8_U8_NOTALIGN(tab0, tab1, tab2,
					    tab0)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					tab = tab0;
					tab0 = tab1;
					tab1 = tab2;
					tab2 = tab;
					s0 = s2;
					s1 = sa[0];
					s2 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_NOTALIGN(tab0, tab1, tab2, tab0)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
#ifdef _LITTLE_ENDIAN
				(*dp++) = tab1[s2 >> 8];
#else
				(*dp++) = tab1[s2 & 0xFF];
#endif
				sp = (mlib_u8 *)sa;
				i += 5;

				if (i < size) {
					(*dp++) = tab2[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					(*dp++) = tab0[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					*dp = tab1[(*sp)];
				}
			}
		}
	} else if (csize == 4) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *da;
			mlib_u16 *sa;
			mlib_u8 *tab0 = (mlib_u8 *)table[0];
			mlib_u8 *tab1 = (mlib_u8 *)table[1];
			mlib_u8 *tab2 = (mlib_u8 *)table[2];
			mlib_u8 *tab3 = (mlib_u8 *)table[3];
			mlib_u8 *tab;
			mlib_u32 s0, s1, s2, t0, t1, t2, t3, t;
			mlib_s32 off;
			mlib_s32 size = xsize * 4;
			mlib_u8 *dp = dst;
			const mlib_u8 *sp = src;

			off = (mlib_s32)((4 - ((mlib_addr)dst & 3)) & 3);

			if (off == 1) {
				(*dp++) = tab0[sp[0]];
				tab = tab0;
				tab0 = tab1;
				tab1 = tab2;
				tab2 = tab3;
				tab3 = tab;
				size--;
				sp++;
			} else if (off == 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				tab = tab0;
				tab0 = tab2;
				tab2 = tab;
				tab = tab1;
				tab1 = tab3;
				tab3 = tab;
				size -= 2;
				sp += 2;
			} else if (off == 3) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				(*dp++) = tab2[sp[2]];
				tab = tab3;
				tab3 = tab2;
				tab2 = tab1;
				tab1 = tab0;
				tab0 = tab;
				size -= 3;
				sp += 3;
			}

			da = (mlib_u32 *)dp;

			if (((mlib_addr)sp & 1) == 0) {
				sa = (mlib_u16 *)sp;

				s0 = sa[0];
				s1 = sa[1];
				sa += 2;

				for (i = 0; i < size - 7; i += 4,
						da++, sa += 2) {
					READ_U8_U8_ALIGN(tab0, tab1, tab2, tab3)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					s0 = sa[0];
					s1 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_ALIGN(tab0, tab1, tab2, tab3)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
				sp = (mlib_u8 *)sa;
				i += 4;

				if (i < size) {
					(*dp++) = tab0[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					(*dp++) = tab1[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					*dp = tab2[(*sp)];
				}

			} else {
				sa = (mlib_u16 *)(sp - 1);

				s0 = sa[0];
				s1 = sa[1];
				s2 = sa[2];
				sa += 3;

				for (i = 0; i < size - 8; i += 4,
						da++, sa += 2) {
					READ_U8_U8_NOTALIGN(tab0, tab1, tab2,
					    tab3)
					    t =
					    (t0 << 24) + (t1 << 16) +
					    (t2 << 8) + t3;
					s0 = s2;
					s1 = sa[0];
					s2 = sa[1];
					da[0] = t;
				}
				READ_U8_U8_NOTALIGN(tab0, tab1, tab2, tab3)
				    t =
				    (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
				da[0] = t;
				da++;
				dp = (mlib_u8 *)da;
#ifdef _LITTLE_ENDIAN
				(*dp++) = tab0[s2 >> 8];
#else
				(*dp++) = tab0[s2 & 0xFF];
#endif
				sp = (mlib_u8 *)sa;
				i += 5;

				if (i < size) {
					(*dp++) = tab1[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					(*dp++) = tab2[(*sp)];
					i++;
					sp++;
				}

				if (i < size) {
					*dp = tab3[(*sp)];
				}
			}
		}
	}
}

/* *********************************************************** */
