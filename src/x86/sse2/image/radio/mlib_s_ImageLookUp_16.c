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

#pragma ident   "@(#)mlib_s_ImageLookUp_16.c	9.3	07/11/05 SMI"

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

/* *********************************************************** */

#include <mlib_types.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	FN_NAME(F)	MLIB_S_IMAGE_LOOKUP_##F

/* *********************************************************** */

#undef	STYPE
#define	STYPE  mlib_s16

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_2INDEX(                   \
			w0, w1, w2, w3, w4, w5, w6, w7, sp)          \
	{                                                    \
	    w0 = ((STYPE *)(sp))[0];                         \
	    w1 = ((STYPE *)(sp))[1];                         \
	    w2 = ((STYPE *)(sp))[2];                         \
	    w3 = ((STYPE *)(sp))[3];                         \
	    w4 = ((STYPE *)(sp))[4];                         \
	    w5 = ((STYPE *)(sp))[5];                         \
	    w6 = ((STYPE *)(sp))[6];                         \
	    w7 = ((STYPE *)(sp))[7];                         \
	}

#else /* C version for LookUp, better than STDC version */
#define	MLIB_S_IMAGE_LOOKUP_S16_2INDEX(                   \
			w0, w1, w2, w3, w4, w5, w6, w7, sp)          \
	{                                                    \
		mlib_s32 s0, s1, s2, s3;                         \
		s0 = ((mlib_s32 *)sp)[0];                        \
		s1 = ((mlib_s32 *)sp)[1];                        \
		s2 = ((mlib_s32 *)sp)[2];                        \
		s3 = ((mlib_s32 *)sp)[3];                        \
		w0 = (s0 << 16) >> 16;                           \
		w1 = (s0 >> 16);                                 \
		w2 = (s1 << 16) >> 16;                           \
		w3 = (s1 >> 16);                                 \
		w4 = (s2 << 16) >> 16;                           \
		w5 = (s2 >> 16);                                 \
		w6 = (s3 << 16) >> 16;                           \
		w7 = (s3 >> 16);                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* ***********************1************************************ */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(               \
			w0, w1, w2, w3, sp)                          \
	{                                                    \
	    w0 = ((STYPE *)(sp))[0];                         \
	    w1 = ((STYPE *)(sp))[1];                         \
	    w2 = ((STYPE *)(sp))[2];                         \
	    w3 = ((STYPE *)(sp))[3];                         \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(               \
			w0, w1, w2, w3, sp)                          \
	{                                                    \
		mlib_s32 s0, s1, s2, s3;                             \
		s0 = ((mlib_s32 *)sp)[0];                            \
		s1 = ((mlib_s32 *)sp)[1];                            \
		w0 = (s0 << 16) >> 16;                           \
		w1 = (s0 >> 16);                                 \
		w2 = (s1 << 16) >> 16;                           \
		w3 = (s1 >> 16);                                 \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_U8_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;      \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;\
		                                              \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,           \
		c4, c5, c6, c7, sp)                           \
		sp++;                                         \
	    FN_NAME(S16_2INDEX)(c8, c9, c10, c11,         \
		c12, c13, c14, c15, sp)                       \
		sp++;                                         \
		dd1 = _mm_setr_epi8(                          \
				t0[c0], t1[c1], t2[c2], t3[c3],       \
				t0[c4], t1[c5], t2[c6], t3[c7],       \
				t0[c8], t1[c9], t2[c10], t3[c11],     \
				t0[c12], t1[c13], t2[c14], t3[c15]);  \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_U8_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;      \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;\
		mlib_u32 e0, e1, e2, e3;                      \
		                                              \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,           \
		c4, c5, c6, c7, sp)                           \
		sp++;                                         \
	    FN_NAME(S16_2INDEX)(c8, c9, c10, c11,         \
		c12, c13, c14, c15, sp)                       \
		sp++;                                         \
		e0 = t0[c0] | (t1[c1] << 8) |                 \
			(t2[c2] << 16) | (t3[c3] << 24);          \
		e1 = t0[c4] | (t1[c5] << 8) |                 \
			(t2[c6] << 16) | (t3[c7] << 24);          \
		e2 = t0[c8] | (t1[c9] << 8) |                 \
			(t2[c10] << 16) | (t3[c11] << 24);        \
		e3 = t0[c12] | (t1[c13] << 8) |               \
			(t2[c14] << 16) | (t3[c15] << 24);        \
		((mlib_u32 *)dp)[0] = e0;                     \
		((mlib_u32 *)dp)[1] = e1;                     \
		((mlib_u32 *)dp)[2] = e2;                     \
		((mlib_u32 *)dp)[3] = e3;                     \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_S16_U8_1E(                \
			dp, sp, t0, t1, t2, t3, n)                \
	{                                                 \
		for (k = 0; k < n - 3; k += 4) {              \
			((mlib_u8 *)dp)[k] =                      \
				t0[((STYPE *)sp)[k]];                 \
			((mlib_u8 *)dp)[k + 1] =                  \
				t1[((STYPE *)sp)[k + 1]];             \
			((mlib_u8 *)dp)[k + 2] =                  \
				t2[((STYPE *)sp)[k + 2]];             \
			((mlib_u8 *)dp)[k + 3] =                  \
				t3[((STYPE *)sp)[k + 3]];             \
		}                                             \
		if (k < n) {                                  \
			((mlib_u8 *)dp)[k] =                      \
				t0[((STYPE *)sp)[k]];                 \
			if (k < n - 1) {                          \
				((mlib_u8 *)dp)[k + 1] =              \
					t1[((STYPE *)sp)[k + 1]];         \
				if (k < n - 2) {                      \
					((mlib_u8 *)dp)[k + 2] =          \
						t2[((STYPE *)sp)[k + 2]];     \
				}                                     \
			}                                         \
		}                                             \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUp_S16_U8_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1, sd2;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_u8 *tab0 = &table[0][32768];

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
			FN_NAME(S16_U8_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
		}

		if (n1 > 0) {
			FN_NAME(S16_U8_1E)(
				dp1, sp1, tab0, tab0, tab0, tab0, n1);


		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1, sd2;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];

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
			FN_NAME(S16_U8_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
		}

		if (n1 > 0) {
			FN_NAME(S16_U8_1E)(
				dp1, sp1, tab0, tab1, tab0, tab1, n1);


		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1, sd2;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];
	const mlib_u8 *tab2 = &table[2][32768];
	const mlib_u8 *tab3 = &table[3][32768];

	dw <<= 2;

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
			FN_NAME(S16_U8_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
		}

		if (n1 > 0) {
			FN_NAME(S16_U8_1E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_U8_3L(                    \
			dp, sp, t0, t1, t2)                           \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;    \
		                                                  \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,               \
		c4, c5, c6, c7, sp)                               \
		sp++;                                             \
	    FN_NAME(S16_2INDEX)(c8, c9, c10, c11,             \
		c12, c13, c14, c15, sp)                           \
		sp++;                                             \
		dd1 = _mm_setr_epi8(                              \
				t0[c0], t1[c1], t2[c2], t0[c3],           \
				t1[c4], t2[c5], t0[c6], t1[c7],           \
				t2[c8], t0[c9], t1[c10], t2[c11],         \
				t0[c12], t1[c13], t2[c14], t0[c15]);      \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_U8_3L(                    \
			dp, sp, t0, t1, t2)                           \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;    \
		mlib_u32 e0, e1, e2, e3;                          \
		                                                  \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,               \
		c4, c5, c6, c7, sp)                               \
		sp++;                                             \
	    FN_NAME(S16_2INDEX)(c8, c9, c10, c11,             \
		c12, c13, c14, c15, sp)                           \
		sp++;                                             \
		e0 = t0[c0] | (t1[c1] << 8) |                     \
			(t2[c2] << 16) | (t0[c3] << 24);              \
		e1 = t1[c4] | (t2[c5] << 8) |                     \
			(t0[c6] << 16) | (t1[c7] << 24);              \
		e2 = t2[c8] | (t0[c9] << 8) |                     \
			(t1[c10] << 16) | (t2[c11] << 24);            \
		e3 = t0[c12] | (t1[c13] << 8) |                   \
			(t2[c14] << 16) | (t0[c15] << 24);            \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_S16_U8_3E(                    \
			dp, sp, t0, t1, t2, n)                        \
	{                                                     \
		for (k = 0; k < n - 2; k += 3) {                  \
			((mlib_u8 *)dp)[k] =                          \
				t0[((STYPE *)sp)[k]];                     \
			((mlib_u8 *)dp)[k + 1] =                      \
				t1[((STYPE *)sp)[k + 1]];                 \
			((mlib_u8 *)dp)[k + 2] =                      \
				t2[((STYPE *)sp)[k + 2]];                 \
		}                                                 \
		if (k < n) {                                      \
			((mlib_u8 *)dp)[k] =                          \
				t0[((STYPE *)sp)[k]];                     \
			if (k < n - 1) {                              \
				((mlib_u8 *)dp)[k + 1] =                  \
					t1[((STYPE *)sp)[k + 1]];             \
			}                                             \
		}                                                 \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_U8_3(
    const mlib_s16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	const mlib_u8 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
		tab0 = &table[0][32768];
		tab1 = &table[1][32768];
		tab2 = &table[2][32768];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 47; n1 -= 48) {
			FN_NAME(S16_U8_3L)(dp1, sp1, tab0, tab1,
			    tab2);

			FN_NAME(S16_U8_3L)(dp1, sp1, tab1, tab2,
			    tab0);

			FN_NAME(S16_U8_3L)(dp1, sp1, tab2, tab0,
			    tab1);
		}

		if (n1 > 0) {
			FN_NAME(S16_U8_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_2L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
	                                                      \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,               \
		c4, c5, c6, c7, sp)                               \
		dd1 = _mm_setr_epi8(                              \
				t0[c0], t1[c0], t2[c1], t3[c1],           \
				t0[c2], t1[c2], t2[c3], t3[c3],           \
				t0[c4], t1[c4], t2[c5], t3[c5],           \
				t0[c6], t1[c6], t2[c7], t3[c7]);          \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_2L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
		mlib_u32 e0, e1, e2, e3;                          \
	                                                      \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,               \
		c4, c5, c6, c7, sp)                               \
		                                                  \
		e0 = t0[c0] | (t1[c0] << 8) |                     \
			(t2[c1] << 16) | (t3[c1] << 24);              \
		e1 = t0[c2] | (t1[c2] << 8) |                     \
			(t2[c3] << 16) | (t3[c3] << 24);              \
		e2 = t0[c4] | (t1[c4] << 8) |                     \
			(t2[c5] << 16) | (t3[c5] << 24);              \
		e3 = t0[c6] | (t1[c6] << 8) |                     \
			(t2[c7] << 16) | (t3[c7] << 24);              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_2E(                 \
			dp, sp, t0, t1, n)                            \
	{                                                     \
		for (k = 0, j = 0; k < n; k += 2, j++) {          \
			((mlib_u8 *)dp)[k] =                          \
				((mlib_u8 *)t0)[((STYPE *)sp)[j]];        \
			((mlib_u8 *)dp)[k + 1] =                      \
				((mlib_u8 *)t1)[((STYPE *)sp)[j]];        \
		}                                                 \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];

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
			FN_NAME(SI_S16_U8_2L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}
		if (n1 > 0) {
			FN_NAME(SI_S16_U8_2E)(
				dp1, sp1, tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */
#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_3L(dp, sp, t0, t1, t2)      \
	{                                                             \
		                                                          \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                       \
		c4, c5, c6, c7, sp)                                       \
		sp++;                                                     \
	    FN_NAME(S16_2INDEX)(c8, c9, c10, c11,                     \
		c12, c13, c14, c15, sp)                                   \
		sp++;                                                     \
                                                                  \
		dd1 = _mm_setr_epi8(                                      \
			tab0[c0], tab1[c0], tab2[c0], tab0[c1],               \
			tab1[c1], tab2[c1], tab0[c2], tab1[c2],               \
			tab2[c2], tab0[c3], tab1[c3], tab2[c3],               \
			tab0[c4], tab1[c4], tab2[c4], tab0[c5]);              \
		_mm_storeu_si128(dp, dd1);                                \
		dp++;                                                     \
		dd1 = _mm_setr_epi8(                                      \
			tab1[c5], tab2[c5], tab0[c6], tab1[c6],               \
			tab2[c6], tab0[c7], tab1[c7], tab2[c7],               \
			tab0[c8], tab1[c8], tab2[c8], tab0[c9],               \
			tab1[c9], tab2[c9], tab0[c10], tab1[c10]);            \
		_mm_storeu_si128(dp, dd1);                                \
		dp++;                                                     \
		dd1 = _mm_setr_epi8(                                      \
			tab2[c10], tab0[c11], tab1[c11], tab2[c11],           \
			tab0[c12], tab1[c12], tab2[c12], tab0[c13],           \
			tab1[c13], tab2[c13], tab0[c14], tab1[c14],           \
			tab2[c14], tab0[c15], tab1[c15], tab2[c15]);          \
		_mm_storeu_si128(dp, dd1);                                \
		dp++;                                                     \
	}


#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_3L(dp, sp, t0, t1, t2)      \
	{                                                             \
		                                                          \
		mlib_u32 e0, e1, e2, e3;                                  \
		                                                          \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                       \
		c4, c5, c6, c7, sp)                                       \
		sp++;                                                     \
	    FN_NAME(S16_2INDEX)(c8, c9, c10, c11,                     \
		c12, c13, c14, c15, sp)                                   \
		sp++;                                                     \
		                                                          \
		e0 = t0[c0] | (t1[c0] << 8) |                             \
			(t2[c0] << 16) | (t0[c1] << 24);                      \
		e1 = t1[c1] | (t2[c1] << 8) |                             \
			(t0[c2] << 16) | (t1[c2] << 24);                      \
		e2 = t2[c2] | (t0[c3] << 8) |                             \
			(t1[c3] << 16) | (t2[c3] << 24);                      \
		e3 = t0[c4] | (t1[c4] << 8) |                             \
			(t2[c4] << 16) | (t0[c5] << 24);                      \
		((mlib_u32 *)dp)[0] = e0;                                 \
		((mlib_u32 *)dp)[1] = e1;                                 \
		((mlib_u32 *)dp)[2] = e2;                                 \
		((mlib_u32 *)dp)[3] = e3;                                 \
		dp++;                                                     \
		                                                          \
		e0 = t1[c5] | (t2[c5] << 8) |                             \
			(t0[c6] << 16) | (t1[c6] << 24);                      \
		e1 = t2[c6] | (t0[c7] << 8) |                             \
			(t1[c7] << 16) | (t2[c7] << 24);                      \
		e2 = t0[c8] | (t1[c8] << 8) |                             \
			(t2[c8] << 16) | (t0[c9] << 24);                      \
		e3 = t1[c9] | (t2[c9] << 8) |                             \
			(t0[c10] << 16) | (t1[c10] << 24);                    \
		((mlib_u32 *)dp)[0] = e0;                                 \
		((mlib_u32 *)dp)[1] = e1;                                 \
		((mlib_u32 *)dp)[2] = e2;                                 \
		((mlib_u32 *)dp)[3] = e3;                                 \
		dp++;                                                     \
    		                                                      \
		e0 = t2[c10] | (t0[c11] << 8) |                           \
			(t1[c11] << 16) | (t2[c11] << 24);                    \
		e1 = t0[c12] | (t1[c12] << 8) |                           \
			(t2[c12] << 16) | (t0[c13] << 24);                    \
		e2 = t1[c13] | (t2[c13] << 8) |                           \
			(t0[c14] << 16) | (t1[c14] << 24);                    \
		e3 = t2[c14] | (t0[c15] << 8) |                           \
			(t1[c15] << 16) | (t2[c15] << 24);                    \
		((mlib_u32 *)dp)[0] = e0;                                 \
		((mlib_u32 *)dp)[1] = e1;                                 \
		((mlib_u32 *)dp)[2] = e2;                                 \
		((mlib_u32 *)dp)[3] = e3;                                 \
		dp++;                                                     \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_3E(                      \
			dp, sp, t0, t1, t2, n)                            \
{                                                             \
		for (k = 0, j = 0; k < n; k += 3, j++) {              \
			((mlib_u8 *)dp)[k] =                              \
				t0[((STYPE *)sp)[j]];                         \
			((mlib_u8 *)dp)[k + 1] =                          \
				t1[((STYPE *)sp)[j]];                         \
			((mlib_u8 *)dp)[k + 2] =                          \
				t2[((STYPE *)sp)[j]];                         \
		}                                                     \
}

/* *********************************************************** */
void
mlib_s_ImageLookUpSI_S16_U8_3(
    const mlib_s16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];
	const mlib_u8 *tab2 = &table[2][32768];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 47; n1 -= 48) {
			FN_NAME(SI_S16_U8_3L)(
				dp1, sp1, tab0, tab1, tab2);
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_U8_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_4L(                \
			dp, sp, t0, t1, t2, t3)                      \
	{                                                    \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;         \
		FN_NAME(S16_2INDEX)(                             \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)              \
                                                         \
		dd1 = _mm_setr_epi8(                             \
				t0[c0], t1[c0], t2[c0], t3[c0],          \
				t0[c1], t1[c1], t2[c1], t3[c1],          \
				t0[c2], t1[c2], t2[c2], t3[c2],          \
				t0[c3], t1[c3], t2[c3], t3[c3]);         \
		_mm_storeu_si128(dp, dd1);                       \
		dp++;                                            \
		dd1 = _mm_setr_epi8(                             \
				t0[c4], t1[c4], t2[c4], t3[c4],          \
				t0[c5], t1[c5], t2[c5], t3[c5],          \
				t0[c6], t1[c6], t2[c6], t3[c6],          \
				t0[c7], t1[c7], t2[c7], t3[c7]);         \
		_mm_storeu_si128(dp, dd1);                       \
		dp++;                                            \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_4L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
		mlib_u32 e0, e1, e2, e3;                          \
		FN_NAME(S16_2INDEX)(                              \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)               \
                                                          \
		e0 = t0[c0] | (t1[c0] << 8) |                     \
			(t2[c0] << 16) | (t3[c0] << 24);              \
		e1 = t0[c1] | (t1[c1] << 8) |                     \
			(t2[c1] << 16) | (t3[c1] << 24);              \
		e2 = t0[c2] | (t1[c2] << 8) |                     \
			(t2[c2] << 16) | (t3[c2] << 24);              \
		e3 = t0[c3] | (t1[c3] << 8) |                     \
			(t2[c3] << 16) | (t3[c3] << 24);              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
		                                                  \
		e0 = t0[c4] | (t1[c4] << 8) |                     \
			(t2[c4] << 16) | (t3[c4] << 24);              \
		e1 = t0[c5] | (t1[c5] << 8) |                     \
			(t2[c5] << 16) | (t3[c5] << 24);              \
		e2 = t0[c6] | (t1[c6] << 8) |                     \
			(t2[c6] << 16) | (t3[c6] << 24);              \
		e3 = t0[c7] | (t1[c7] << 8) |                     \
			(t2[c7] << 16) | (t3[c7] << 24);              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
	}

#endif /* End of LOOKUP_SSE2 */

/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_U8_4E(                \
			dp, sp, t0, t1, t2, t3, n)                   \
	{                                                    \
		for (k = 0, j = 0; k < n; k += 4, j++) {         \
			((mlib_u8 *)dp)[k] =                         \
				((mlib_u8 *)t0)[((STYPE *)sp)[j]];       \
			((mlib_u8 *)dp)[k + 1] =                     \
				((mlib_u8 *)t1)[((STYPE *)sp)[j]];       \
			((mlib_u8 *)dp)[k + 2] =                     \
				((mlib_u8 *)t2)[((STYPE *)sp)[j]];       \
			((mlib_u8 *)dp)[k + 3] =                     \
				((mlib_u8 *)t3)[((STYPE *)sp)[j]];       \
		}                                                \
	}



/* *********************************************************** */


void
mlib_s_ImageLookUpSI_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];
	const mlib_u8 *tab2 = &table[2][32768];
	const mlib_u8 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 31; n1 -= 32) {
			FN_NAME(SI_S16_U8_4L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_U8_4E)(
				dp1, sp1, tab0, tab1,
				tab2, tab3, n1)
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#undef	STYPE
#define	STYPE  mlib_u16
/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_2INDEX(                  \
			w0, w1, w2, w3, w4, w5, w6, w7, sp)          \
	{                                                    \
	    w0 = ((STYPE *)(sp))[0];                         \
	    w1 = ((STYPE *)(sp))[1];                         \
	    w2 = ((STYPE *)(sp))[2];                         \
	    w3 = ((STYPE *)(sp))[3];                         \
	    w4 = ((STYPE *)(sp))[4];                         \
	    w5 = ((STYPE *)(sp))[5];                         \
	    w6 = ((STYPE *)(sp))[6];                         \
	    w7 = ((STYPE *)(sp))[7];                         \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_2INDEX(                  \
			w0, w1, w2, w3, w4, w5, w6, w7, sp)          \
	{                                                    \
		mlib_u32 s0, s1, s2, s3;                         \
		s0 = ((mlib_u32 *)sp)[0];                        \
		s1 = ((mlib_u32 *)sp)[1];                        \
		s2 = ((mlib_u32 *)sp)[2];                        \
		s3 = ((mlib_u32 *)sp)[3];                        \
		w0 = (s0 & 0xFFFF);                              \
		w1 = (s0 >> 16);                                 \
		w2 = (s1 & 0xFFFF);                              \
		w3 = (s1 >> 16);                                 \
		w4 = (s2 & 0xFFFF);                              \
		w5 = (s2 >> 16);                                 \
		w6 = (s3 & 0xFFFF);                              \
		w7 = (s3 >> 16);                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(               \
			w0, w1, w2, w3, sp)                          \
	{                                                    \
	    w0 = ((STYPE *)(sp))[0];                         \
	    w1 = ((STYPE *)(sp))[1];                         \
	    w2 = ((STYPE *)(sp))[2];                         \
	    w3 = ((STYPE *)(sp))[3];                         \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(               \
			w0, w1, w2, w3, sp)                          \
	{                                                    \
		mlib_u32 s0, s1, s2, s3;                         \
		s0 = ((mlib_u32 *)sp)[0];                        \
		s1 = ((mlib_u32 *)sp)[1];                        \
		w0 = (s0 & 0xFFFF);                              \
		w1 = (s0 >> 16);                                 \
		w2 = (s1 & 0xFFFF);                              \
		w3 = (s1 >> 16);                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_U8_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;      \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;\
                                                      \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(               \
			c0, c1, c2, c3, c4, c5, c6, c7, sp)       \
		sp++;                                         \
		MLIB_S_IMAGE_LOOKUP_U16_2INDEX(               \
			c8, c9, c10, c11, c12, c13, c14, c15, sp) \
		sp++;                                         \
		dd1 = _mm_setr_epi8(                          \
				t0[c0], t1[c1], t2[c2], t3[c3],       \
				t0[c4], t1[c5], t2[c6], t3[c7],       \
				t0[c8], t1[c9], t2[c10], t3[c11],     \
				t0[c12], t1[c13], t2[c14], t3[c15]);  \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_U8_1L(                \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;      \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;\
		mlib_u32 e0, e1, e2, e3;                      \
                                                      \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(               \
			c0, c1, c2, c3, c4, c5, c6, c7, sp)       \
		sp++;                                         \
		MLIB_S_IMAGE_LOOKUP_U16_2INDEX(               \
			c8, c9, c10, c11, c12, c13, c14, c15, sp) \
		sp++;                                         \
		                                              \
		e0 = t0[c0] | (t1[c1] << 8) |                 \
			(t2[c2] << 16) | (t3[c3] << 24);          \
		e1 = t0[c4] | (t1[c5] << 8) |                 \
			(t2[c6] << 16) | (t3[c7] << 24);          \
		e2 = t0[c8] | (t1[c9] << 8) |                 \
			(t2[c10] << 16) | (t3[c11] << 24);        \
		e3 = t0[c12] | (t1[c13] << 8) |               \
			(t2[c14] << 16) | (t3[c15] << 24);        \
		((mlib_u32 *)dp)[0] = e0;                     \
		((mlib_u32 *)dp)[1] = e1;                     \
		((mlib_u32 *)dp)[2] = e2;                     \
		((mlib_u32 *)dp)[3] = e3;                     \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U16_U8_1E(                \
			dp, sp, t0, t1, t2, t3, n)                \
	{                                                 \
		for (k = 0; k < n - 3; k += 4) {              \
			((mlib_u8 *)dp)[k] =                      \
				t0[((STYPE *)sp)[k]];                 \
			((mlib_u8 *)dp)[k + 1] =                  \
				t1[((STYPE *)sp)[k + 1]];             \
			((mlib_u8 *)dp)[k + 2] =                  \
				t2[((STYPE *)sp)[k + 2]];             \
			((mlib_u8 *)dp)[k + 3] =                  \
				t3[((STYPE *)sp)[k + 3]];             \
		}                                             \
		if (k < n) {                                  \
			((mlib_u8 *)dp)[k] =                      \
				t0[((STYPE *)sp)[k]];                 \
			if (k < n - 1) {                          \
				((mlib_u8 *)dp)[k + 1] =              \
					t1[((STYPE *)sp)[k + 1]];         \
				if (k < n - 2) {                      \
					((mlib_u8 *)dp)[k + 2] =          \
						t2[((STYPE *)sp)[k + 2]];     \
				}                                     \
			}                                         \
		}                                             \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_U8_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1, sd2;

/* dst address */
	__m128i *dp1, *dl1, dd1;

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
			FN_NAME(U16_U8_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
		}
		if (n1 > 0) {
			FN_NAME(U16_U8_1E)(
				dp1, sp1, tab0, tab0, tab0, tab0, n1);
		}
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_U8_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1, sd2;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];

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
			FN_NAME(U16_U8_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
		}

		if (n1 > 0) {
			FN_NAME(U16_U8_1E)(
				dp1, sp1, tab0, tab1, tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_U8_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1, sd2;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];
	mlib_u8 *tab2 = (mlib_u8 *)table[2];
	mlib_u8 *tab3 = (mlib_u8 *)table[3];

	dw <<= 2;

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
			FN_NAME(U16_U8_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
		}

		if (n1 > 0) {
			FN_NAME(U16_U8_1E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_U8_3L(                      \
			dp, sp, t0, t1, t2)                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;      \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
		sp++;                                               \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c8, c9, c10, c11,    \
		c12, c13, c14, c15, sp)                             \
		sp++;                                               \
		dd1 = _mm_setr_epi8(                                \
				t0[c0], t1[c1], t2[c2], t0[c3],             \
				t1[c4], t2[c5], t0[c6], t1[c7],             \
				t2[c8], t0[c9], t1[c10], t2[c11],           \
				t0[c12], t1[c13], t2[c14], t0[c15]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_U8_3L(                    \
			dp, sp, t0, t1, t2)                           \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
	    mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;    \
		mlib_u32 e0, e1, e2, e3;                          \
		                                                  \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,    \
		c4, c5, c6, c7, sp)                               \
		sp++;                                             \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c8, c9, c10, c11,  \
		c12, c13, c14, c15, sp)                           \
		sp++;                                             \
		                                                  \
		e0 = t0[c0] | (t1[c1] << 8) |                     \
			(t2[c2] << 16) | (t0[c3] << 24);              \
		e1 = t1[c4] | (t2[c5] << 8) |                     \
			(t0[c6] << 16) | (t1[c7] << 24);              \
		e2 = t2[c8] | (t0[c9] << 8) |                     \
			(t1[c10] << 16) | (t2[c11] << 24);            \
		e3 = t0[c12] | (t1[c13] << 8) |                   \
			(t2[c14] << 16) | (t0[c15] << 24);            \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U16_U8_3E(                    \
			dp, sp, t0, t1, t2, n)                        \
	{                                                     \
		for (k = 0; k < n - 2; k += 3) {                  \
			((mlib_u8 *)dp)[k] =                          \
				t0[((STYPE *)sp)[k]];                     \
			((mlib_u8 *)dp)[k + 1] =                      \
				t1[((STYPE *)sp)[k + 1]];                 \
			((mlib_u8 *)dp)[k + 2] =                      \
				t2[((STYPE *)sp)[k + 2]];                 \
		}                                                 \
		if (k < n) {                                      \
			((mlib_u8 *)dp)[k] =                          \
				t0[((STYPE *)sp)[k]];                     \
			if (k < n - 1) {                              \
				((mlib_u8 *)dp)[k + 1] =                  \
					t1[((STYPE *)sp)[k + 1]];             \
			}                                             \
		}                                                 \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_U8_3(
    const mlib_u16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

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
			FN_NAME(U16_U8_3L)(dp1, sp1, tab0, tab1,
			    tab2);

			FN_NAME(U16_U8_3L)(dp1, sp1, tab1, tab2,
			    tab0);

			FN_NAME(U16_U8_3L)(dp1, sp1, tab2, tab0,
			    tab1);
		}
		if (n1 > 0) {
			FN_NAME(U16_U8_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_2L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
	                                                      \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,    \
		c4, c5, c6, c7, sp)                               \
		dd1 = _mm_setr_epi8(                              \
				t0[c0], t1[c0], t2[c1], t3[c1],           \
				t0[c2], t1[c2], t2[c3], t3[c3],           \
				t0[c4], t1[c4], t2[c5], t3[c5],           \
				t0[c6], t1[c6], t2[c7], t3[c7]);          \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
	}
#else /* C version for LookUp, better than STDC version */
#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_2L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
		mlib_u32 e0, e1, e2, e3;                          \
	                                                      \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,    \
		c4, c5, c6, c7, sp)                               \
		                                                  \
		e0 = t0[c0] | (t1[c0] << 8) |                     \
			(t2[c1] << 16) | (t3[c1] << 24);              \
		e1 = t0[c2] | (t1[c2] << 8) |                     \
			(t2[c3] << 16) | (t3[c3] << 24);              \
		e2 = t0[c4] | (t1[c4] << 8) |                     \
			(t2[c5] << 16) | (t3[c5] << 24);              \
		e3 = t0[c6] | (t1[c6] << 8) |                     \
			(t2[c7] << 16) | (t3[c7] << 24);              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_2E(                 \
			dp, sp, t0, t1, n)                            \
	{                                                     \
		for (k = 0, j = 0; k < n; k += 2, j++) {          \
			((mlib_u8 *)dp)[k] =                          \
				((mlib_u8 *)t0)[((STYPE *)sp)[j]];        \
			((mlib_u8 *)dp)[k + 1] =                      \
				((mlib_u8 *)t1)[((STYPE *)sp)[j]];        \
		}                                                 \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U16_U8_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];

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
			FN_NAME(SI_U16_U8_2L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_U8_2E)(
				dp1, sp1, tab0, tab1, n1);
		}


		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_3L(dp, sp, t0, t1, t2)      \
	{                                                             \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		c4, c5, c6, c7, sp)                                       \
		sp++;                                                     \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c8, c9, c10, c11,          \
		c12, c13, c14, c15, sp)                                   \
		sp++;                                                     \
                                                                  \
		dd1 = _mm_setr_epi8(                                      \
			tab0[c0], tab1[c0], tab2[c0], tab0[c1],               \
			tab1[c1], tab2[c1], tab0[c2], tab1[c2],               \
			tab2[c2], tab0[c3], tab1[c3], tab2[c3],               \
			tab0[c4], tab1[c4], tab2[c4], tab0[c5]);              \
		_mm_storeu_si128(dp, dd1);                                \
		dp++;                                                     \
		dd1 = _mm_setr_epi8(                                      \
			tab1[c5], tab2[c5], tab0[c6], tab1[c6],               \
			tab2[c6], tab0[c7], tab1[c7], tab2[c7],               \
			tab0[c8], tab1[c8], tab2[c8], tab0[c9],               \
			tab1[c9], tab2[c9], tab0[c10], tab1[c10]);            \
		_mm_storeu_si128(dp, dd1);                                \
		dp++;                                                     \
		dd1 = _mm_setr_epi8(                                      \
			tab2[c10], tab0[c11], tab1[c11], tab2[c11],           \
			tab0[c12], tab1[c12], tab2[c12], tab0[c13],           \
			tab1[c13], tab2[c13], tab0[c14], tab1[c14],           \
			tab2[c14], tab0[c15], tab1[c15], tab2[c15]);          \
		_mm_storeu_si128(dp, dd1);                                \
		dp++;                                                     \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_3L(dp, sp, t0, t1, t2)      \
	{                                                             \
		mlib_u32 e0, e1, e2, e3;                                  \
		                                                          \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		c4, c5, c6, c7, sp)                                       \
		sp++;                                                     \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c8, c9, c10, c11,          \
		c12, c13, c14, c15, sp)                                   \
		sp++;                                                     \
		                                                          \
		e0 = t0[c0] | (t1[c0] << 8) |                             \
			(t2[c0] << 16) | (t0[c1] << 24);                      \
		e1 = t1[c1] | (t2[c1] << 8) |                             \
			(t0[c2] << 16) | (t1[c2] << 24);                      \
		e2 = t2[c2] | (t0[c3] << 8) |                             \
			(t1[c3] << 16) | (t2[c3] << 24);                      \
		e3 = t0[c4] | (t1[c4] << 8) |                             \
			(t2[c4] << 16) | (t0[c5] << 24);                      \
		((mlib_u32 *)dp)[0] = e0;                                 \
		((mlib_u32 *)dp)[1] = e1;                                 \
		((mlib_u32 *)dp)[2] = e2;                                 \
		((mlib_u32 *)dp)[3] = e3;                                 \
		dp++;                                                     \
		                                                          \
		e0 = t1[c5] | (t2[c5] << 8) |                             \
			(t0[c6] << 16) | (t1[c6] << 24);                      \
		e1 = t2[c6] | (t0[c7] << 8) |                             \
			(t1[c7] << 16) | (t2[c7] << 24);                      \
		e2 = t0[c8] | (t1[c8] << 8) |                             \
			(t2[c8] << 16) | (t0[c9] << 24);                      \
		e3 = t1[c9] | (t2[c9] << 8) |                             \
			(t0[c10] << 16) | (t1[c10] << 24);                    \
		((mlib_u32 *)dp)[0] = e0;                                 \
		((mlib_u32 *)dp)[1] = e1;                                 \
		((mlib_u32 *)dp)[2] = e2;                                 \
		((mlib_u32 *)dp)[3] = e3;                                 \
		dp++;                                                     \
    		                                                      \
		e0 = t2[c10] | (t0[c11] << 8) |                           \
			(t1[c11] << 16) | (t2[c11] << 24);                    \
		e1 = t0[c12] | (t1[c12] << 8) |                           \
			(t2[c12] << 16) | (t0[c13] << 24);                    \
		e2 = t1[c13] | (t2[c13] << 8) |                           \
			(t0[c14] << 16) | (t1[c14] << 24);                    \
		e3 = t2[c14] | (t0[c15] << 8) |                           \
			(t1[c15] << 16) | (t2[c15] << 24);                    \
		((mlib_u32 *)dp)[0] = e0;                                 \
		((mlib_u32 *)dp)[1] = e1;                                 \
		((mlib_u32 *)dp)[2] = e2;                                 \
		((mlib_u32 *)dp)[3] = e3;                                 \
		dp++;                                                     \
                                                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_3E(                      \
			dp, sp, t0, t1, t2, n)                            \
{                                                             \
		for (k = 0, j = 0; k < n; k += 3, j++) {              \
			((mlib_u8 *)dp)[k] =                              \
				t0[((STYPE *)sp)[j]];                         \
			((mlib_u8 *)dp)[k + 1] =                          \
				t1[((STYPE *)sp)[j]];                         \
			((mlib_u8 *)dp)[k + 2] =                          \
				t2[((STYPE *)sp)[j]];                         \
		}                                                     \
}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U16_U8_3(
    const mlib_u16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_s32 c8, c9, c10, c11, c12, c13, c14, c15;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];
	mlib_u8 *tab2 = (mlib_u8 *)table[2];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 47; n1 -= 48) {
			FN_NAME(SI_U16_U8_3L)(
				dp1, sp1, tab0, tab1, tab2);
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_U8_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_4L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
		MLIB_S_IMAGE_LOOKUP_U16_2INDEX(                   \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)               \
                                                          \
		dd1 = _mm_setr_epi8(                              \
				t0[c0], t1[c0], t2[c0], t3[c0],           \
				t0[c1], t1[c1], t2[c1], t3[c1],           \
				t0[c2], t1[c2], t2[c2], t3[c2],           \
				t0[c3], t1[c3], t2[c3], t3[c3]);          \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
		dd1 = _mm_setr_epi8(                              \
				t0[c4], t1[c4], t2[c4], t3[c4],           \
				t0[c5], t1[c5], t2[c5], t3[c5],           \
				t0[c6], t1[c6], t2[c6], t3[c6],           \
				t0[c7], t1[c7], t2[c7], t3[c7]);          \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_4L(                 \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
		mlib_u32 e0, e1, e2, e3;                          \
		MLIB_S_IMAGE_LOOKUP_U16_2INDEX(                   \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)               \
		                                                  \
		e0 = t0[c0] | (t1[c0] << 8) |                     \
			(t2[c0] << 16) | (t3[c0] << 24);              \
		e1 = t0[c1] | (t1[c1] << 8) |                     \
			(t2[c1] << 16) | (t3[c1] << 24);              \
		e2 = t0[c2] | (t1[c2] << 8) |                     \
			(t2[c2] << 16) | (t3[c2] << 24);              \
		e3 = t0[c3] | (t1[c3] << 8) |                     \
			(t2[c3] << 16) | (t3[c3] << 24);              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
		                                                  \
		e0 = t0[c4] | (t1[c4] << 8) |                     \
			(t2[c4] << 16) | (t3[c4] << 24);              \
		e1 = t0[c5] | (t1[c5] << 8) |                     \
			(t2[c5] << 16) | (t3[c5] << 24);              \
		e2 = t0[c6] | (t1[c6] << 8) |                     \
			(t2[c6] << 16) | (t3[c6] << 24);              \
		e3 = t0[c7] | (t1[c7] << 8) |                     \
			(t2[c7] << 16) | (t3[c7] << 24);              \
		((mlib_u32 *)dp)[0] = e0;                         \
		((mlib_u32 *)dp)[1] = e1;                         \
		((mlib_u32 *)dp)[2] = e2;                         \
		((mlib_u32 *)dp)[3] = e3;                         \
		dp++;                                             \
                                                          \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_U8_4E(                \
			dp, sp, t0, t1, t2, t3, n)                   \
	{                                                    \
		for (k = 0, j = 0; k < n; k += 4, j++) {         \
			((mlib_u8 *)dp)[k] =                         \
				((mlib_u8 *)t0)[((STYPE *)sp)[j]];       \
			((mlib_u8 *)dp)[k + 1] =                     \
				((mlib_u8 *)t1)[((STYPE *)sp)[j]];       \
			((mlib_u8 *)dp)[k + 2] =                     \
				((mlib_u8 *)t2)[((STYPE *)sp)[j]];       \
			((mlib_u8 *)dp)[k + 3] =                     \
				((mlib_u8 *)t3)[((STYPE *)sp)[j]];       \
		}                                                \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_U16_U8_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];
	mlib_u8 *tab2 = (mlib_u8 *)table[2];
	mlib_u8 *tab3 = (mlib_u8 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 31; n1 -= 32) {
			FN_NAME(SI_U16_U8_4L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_U8_4E)(
				dp1, sp1, tab0, tab1,
				tab2, tab3, n1)
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#undef	STYPE
#define	STYPE  mlib_s16
/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_S16_1L(                  \
			dp, sp, t0, t1, t2, t3)                      \
	{                                                    \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;         \
                                                         \
		FN_NAME(S16_2INDEX)(                             \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)              \
                                                         \
		dd1 = _mm_setr_epi16(                            \
				t0[c0], t1[c1], t2[c2], t3[c3],          \
				t0[c4], t1[c5], t2[c6], t3[c7]);         \
		_mm_storeu_si128(dp, dd1);                       \
		dp++;                                            \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_S16_1L(                  \
			dp, sp, t0, t1, t2, t3)                      \
	{                                                    \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;         \
		mlib_u32 e0, e1, e2, e3;                         \
                                                         \
		FN_NAME(S16_2INDEX)(                             \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)              \
		                                                 \
		e0 = (t0[c0] & 0xFFFF) | (t1[c1] << 16);         \
		e1 = (t2[c2] & 0xFFFF) | (t3[c3] << 16);         \
		e2 = (t0[c4] & 0xFFFF) | (t1[c5] << 16);         \
		e3 = (t2[c6] & 0xFFFF) | (t3[c7] << 16);         \
		((mlib_u32 *)dp)[0] = e0;                        \
		((mlib_u32 *)dp)[1] = e1;                        \
		((mlib_u32 *)dp)[2] = e2;                        \
		((mlib_u32 *)dp)[3] = e3;                        \
		dp++;                                            \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_S16_S16_1E(              \
			dp, sp, t0, t1, t2, t3, n)               \
	{                                                \
		for (k = 0; k < n - 3; k += 4) {             \
			((mlib_s16 *)dp)[k] =                    \
				t0[((STYPE *)sp)[k]];                \
			((mlib_s16 *)dp)[k + 1] =                \
				t1[((STYPE *)sp)[k + 1]];            \
			((mlib_s16 *)dp)[k + 2] =                \
				t2[((STYPE *)sp)[k + 2]];            \
			((mlib_s16 *)dp)[k + 3] =                \
				t3[((STYPE *)sp)[k + 3]];            \
		}                                            \
		if (k < n) {                                 \
			((mlib_s16 *)dp)[k] =                    \
				t0[((STYPE *)sp)[k]];                \
			if (k < n - 1) {                         \
				((mlib_s16 *)dp)[k + 1] =            \
					t1[((STYPE *)sp)[k + 1]];        \
				if (k < n - 2) {                     \
					((mlib_s16 *)dp)[k + 2] =        \
						t2[((STYPE *)sp)[k + 2]];    \
				}                                    \
			}                                        \
		}                                            \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_s16 *tab0 = &table[0][32768];

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
			FN_NAME(S16_S16_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S16_1E)(
				dp1, sp1, tab0, tab0, tab0, tab0, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];

	dw <<= 1;

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
			FN_NAME(S16_S16_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S16_1E)(
				dp1, sp1, tab0, tab1, tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];
	const mlib_s16 *tab2 = &table[2][32768];
	const mlib_s16 *tab3 = &table[3][32768];

	dw <<= 2;

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
			FN_NAME(S16_S16_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S16_1E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_S16_3L(dp, sp, t0, t1, t2)    \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;              \
		                                                      \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                   \
		c4, c5, c6, c7, sp)                                   \
		dd1 = _mm_setr_epi16(                                 \
				t0[c0], t1[c1], t2[c2], t0[c3],               \
				t1[c4], t2[c5], t0[c6], t1[c7]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_S16_3L(dp, sp, t0, t1, t2)    \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;              \
		mlib_u32 e0, e1, e2, e3;                              \
		                                                      \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                   \
		c4, c5, c6, c7, sp)                                   \
		                                                      \
		e0 = (t0[c0] & 0xFFFF) | (t1[c1] << 16);              \
		e1 = (t2[c2] & 0xFFFF) | (t0[c3] << 16);              \
		e2 = (t1[c4] & 0xFFFF) | (t2[c5] << 16);              \
		e3 = (t0[c6] & 0xFFFF) | (t1[c7] << 16);              \
		((mlib_u32 *)dp)[0] = e0;                             \
		((mlib_u32 *)dp)[1] = e1;                             \
		((mlib_u32 *)dp)[2] = e2;                             \
		((mlib_u32 *)dp)[3] = e3;                             \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_S16_S16_3E(                  \
			dp, sp, t0, t1, t2, n)                       \
	{                                                    \
		for (k = 0; k < n - 2; k += 3) {                 \
			((mlib_s16 *)dp)[k] =                        \
				t0[((STYPE *)sp)[k]];                    \
			((mlib_s16 *)dp)[k + 1] =                    \
				t1[((STYPE *)sp)[k + 1]];                \
			((mlib_s16 *)dp)[k + 2] =                    \
				t2[((STYPE *)sp)[k + 2]];                \
		}                                                \
		if (k < n) {                                     \
			((mlib_s16 *)dp)[k] =                        \
				t0[((STYPE *)sp)[k]];                    \
			if (k < n - 1) {                             \
				((mlib_s16 *)dp)[k + 1] =                \
					t1[((STYPE *)sp)[k + 1]];            \
			}                                            \
		}                                                \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S16_3(
    const mlib_s16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	const mlib_s16 *tab0, *tab1, *tab2;

	tab0 = &table[0][32768];
	tab1 = &table[1][32768];
	tab2 = &table[2][32768];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 23; n1 -= 24) {
			FN_NAME(S16_S16_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1++;

			FN_NAME(S16_S16_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1++;

			FN_NAME(S16_S16_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S16_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_2L(dp, sp1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		                                                    \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                 \
		c4, c5, c6, c7, sp1)                                \
                                                            \
		dd1 = _mm_setr_epi16(                               \
				t0[c0], t1[c0], t2[c1], t3[c1],             \
				t0[c2], t1[c2], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c4], t1[c4], t2[c5], t3[c5],             \
				t0[c6], t1[c6], t2[c7], t3[c7]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_2L(dp, sp1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		mlib_u32 e0, e1, e2, e3;                            \
		                                                    \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                 \
		c4, c5, c6, c7, sp1)                                \
 		                                                    \
		e0 = (t0[c0] & 0xFFFF) | (t1[c0] << 16);            \
		e1 = (t2[c1] & 0xFFFF) | (t3[c1] << 16);            \
		e2 = (t0[c2] & 0xFFFF) | (t1[c2] << 16);            \
		e3 = (t2[c3] & 0xFFFF) | (t3[c3] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c4] & 0xFFFF) | (t1[c4] << 16);            \
		e1 = (t2[c5] & 0xFFFF) | (t3[c5] << 16);            \
		e2 = (t0[c6] & 0xFFFF) | (t1[c6] << 16);            \
		e3 = (t2[c7] & 0xFFFF) | (t3[c7] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_2E(                  \
			dp, sp, t0, t1, n)                              \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 2, j++) {            \
			((mlib_s16 *)dp)[k] =                           \
				((mlib_s16 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 1] =                       \
				((mlib_s16 *)t1)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_S16_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];

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
			FN_NAME(SI_S16_S16_2L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_S16_2E)(
				dp1, sp1, tab0, tab1, n1);

		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_3L(dp, sp, t0,       \
	t1, t2)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		                                                    \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                 \
		c4, c5, c6, c7, sp)                                 \
                                                            \
		dd1 = _mm_setr_epi16(                               \
			tab0[c0], tab1[c0], tab2[c0], tab0[c1],         \
			tab1[c1], tab2[c1], tab0[c2], tab1[c2]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
			tab2[c2], tab0[c3], tab1[c3], tab2[c3],         \
			tab0[c4], tab1[c4], tab2[c4], tab0[c5]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
			tab1[c5], tab2[c5], tab0[c6], tab1[c6],         \
			tab2[c6], tab0[c7], tab1[c7], tab2[c7]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_3L(dp, sp, t0,       \
	t1, t2)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		mlib_u32 e0, e1, e2, e3;                            \
		                                                    \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                 \
		c4, c5, c6, c7, sp)                                 \
 	                                                        \
		e0 = (t0[c0] & 0xFFFF) | (t1[c0] << 16);            \
		e1 = (t2[c0] & 0xFFFF) | (t0[c1] << 16);            \
		e2 = (t1[c1] & 0xFFFF) | (t2[c1] << 16);            \
		e3 = (t0[c2] & 0xFFFF) | (t1[c2] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 	                                                        \
		e0 = (t2[c2] & 0xFFFF) | (t0[c3] << 16);            \
		e1 = (t1[c3] & 0xFFFF) | (t2[c3] << 16);            \
		e2 = (t0[c4] & 0xFFFF) | (t1[c4] << 16);            \
		e3 = (t2[c4] & 0xFFFF) | (t0[c5] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 	                                                        \
		e0 = (t1[c5] & 0xFFFF) | (t2[c5] << 16);            \
		e1 = (t0[c6] & 0xFFFF) | (t1[c6] << 16);            \
		e2 = (t2[c6] & 0xFFFF) | (t0[c7] << 16);            \
		e3 = (t1[c7] & 0xFFFF) | (t2[c7] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_3E(                  \
			dp, sp, t0, t1, t2, n)                          \
{                                                           \
		for (k = 0, j = 0; k < n; k += 3, j++) {            \
			((mlib_s16 *)dp)[k] =                           \
				t0[((STYPE *)sp)[j]];                       \
			((mlib_s16 *)dp)[k + 1] =                       \
				t1[((STYPE *)sp)[j]];                       \
			((mlib_s16 *)dp)[k + 2] =                       \
				t2[((STYPE *)sp)[j]];                       \
		}                                                   \
}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_S16_S16_3(
    const mlib_s16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];
	const mlib_s16 *tab2 = &table[2][32768];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 23; n1 -= 24) {
			FN_NAME(SI_S16_S16_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_S16_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}


		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		                                                    \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                 \
		c4, c5, c6, c7, sp)                                 \
                                                            \
		dd1 = _mm_setr_epi16(                               \
				t0[c0], t1[c0], t2[c0], t3[c0],             \
				t0[c1], t1[c1], t2[c1], t3[c1]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c2], t1[c2], t2[c2], t3[c2],             \
				t0[c3], t1[c3], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c4], t1[c4], t2[c4], t3[c4],             \
				t0[c5], t1[c5], t2[c5], t3[c5]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c6], t1[c6], t2[c6], t3[c6],             \
				t0[c7], t1[c7], t2[c7], t3[c7]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		mlib_u32 e0, e1, e2, e3;                            \
		                                                    \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,                 \
		c4, c5, c6, c7, sp)                                 \
 		                                                    \
		e0 = (t0[c0] & 0xFFFF) | (t1[c0] << 16);            \
		e1 = (t2[c0] & 0xFFFF) | (t3[c0] << 16);            \
		e2 = (t0[c1] & 0xFFFF) | (t1[c1] << 16);            \
		e3 = (t2[c1] & 0xFFFF) | (t3[c1] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c2] & 0xFFFF) | (t1[c2] << 16);            \
		e1 = (t2[c2] & 0xFFFF) | (t3[c2] << 16);            \
		e2 = (t0[c3] & 0xFFFF) | (t1[c3] << 16);            \
		e3 = (t2[c3] & 0xFFFF) | (t3[c3] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c4] & 0xFFFF) | (t1[c4] << 16);            \
		e1 = (t2[c4] & 0xFFFF) | (t3[c4] << 16);            \
		e2 = (t0[c5] & 0xFFFF) | (t1[c5] << 16);            \
		e3 = (t2[c5] & 0xFFFF) | (t3[c5] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c6] & 0xFFFF) | (t1[c6] << 16);            \
		e1 = (t2[c6] & 0xFFFF) | (t3[c6] << 16);            \
		e2 = (t0[c7] & 0xFFFF) | (t1[c7] << 16);            \
		e3 = (t2[c7] & 0xFFFF) | (t3[c7] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S16_4E(                  \
			dp, sp, t0, t1, t2, t3, n)                      \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 4, j++) {            \
			((mlib_s16 *)dp)[k] =                           \
				((mlib_s16 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 1] =                       \
				((mlib_s16 *)t1)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 2] =                       \
				((mlib_s16 *)t2)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 3] =                       \
				((mlib_s16 *)t3)[((STYPE *)sp)[j]];         \
		}                                                   \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUpSI_S16_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];
	const mlib_s16 *tab2 = &table[2][32768];
	const mlib_s16 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 31; n1 -= 32) {
			FN_NAME(SI_S16_S16_4L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_S16_4E)(
				dp1, sp1, tab0, tab1,
				tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#undef	STYPE
#define	STYPE  mlib_u16
/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */
#define	MLIB_S_IMAGE_LOOKUP_U16_S16_1L(                   \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
                                                          \
		MLIB_S_IMAGE_LOOKUP_U16_2INDEX(                   \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)               \
                                                          \
		dd1 = _mm_setr_epi16(                             \
				t0[c0], t1[c1], t2[c2], t3[c3],           \
				t0[c4], t1[c5], t2[c6], t3[c7]);          \
		_mm_storeu_si128(dp, dd1);                        \
		dp++;                                             \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_S16_1L(                   \
			dp, sp, t0, t1, t2, t3)                       \
	{                                                     \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;          \
		mlib_u32 e0, e1, e2, e3;                          \
                                                          \
		MLIB_S_IMAGE_LOOKUP_U16_2INDEX(                   \
		c0, c1, c2, c3, c4, c5, c6, c7, sp)               \
                                                          \
		e0 = (t0[c0] & 0xFFFF) | (t1[c1] << 16);         \
		e1 = (t2[c2] & 0xFFFF) | (t3[c3] << 16);         \
		e2 = (t0[c4] & 0xFFFF) | (t1[c5] << 16);         \
		e3 = (t2[c6] & 0xFFFF) | (t3[c7] << 16);         \
		((mlib_u32 *)dp)[0] = e0;                        \
		((mlib_u32 *)dp)[1] = e1;                        \
		((mlib_u32 *)dp)[2] = e2;                        \
		((mlib_u32 *)dp)[3] = e3;                        \
		dp++;                                            \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */
#define	MLIB_S_IMAGE_LOOKUP_U16_S16_1E(              \
			dp, sp, t0, t1, t2, t3, n)               \
	{                                                \
		for (k = 0; k < n - 3; k += 4) {             \
			((mlib_s16 *)dp)[k] =                    \
				t0[((STYPE *)sp)[k]];                \
			((mlib_s16 *)dp)[k + 1] =                \
				t1[((STYPE *)sp)[k + 1]];            \
			((mlib_s16 *)dp)[k + 2] =                \
				t2[((STYPE *)sp)[k + 2]];            \
			((mlib_s16 *)dp)[k + 3] =                \
				t3[((STYPE *)sp)[k + 3]];            \
		}                                            \
		if (k < n) {                                 \
			((mlib_s16 *)dp)[k] =                    \
				t0[((STYPE *)sp)[k]];                \
			if (k < n - 1) {                         \
				((mlib_s16 *)dp)[k + 1] =            \
					t1[((STYPE *)sp)[k + 1]];        \
				if (k < n - 2) {                     \
					((mlib_s16 *)dp)[k + 2] =        \
						t2[((STYPE *)sp)[k + 2]];    \
				}                                    \
			}                                        \
		}                                            \
	}


/* *********************************************************** */


void
mlib_s_ImageLookUp_U16_S16_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

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
		for (n1 = dw; n1 > 7; n1 -= 8) {
			FN_NAME(U16_S16_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S16_1E)(
				dp1, sp1, tab0, tab0, tab0, tab0, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_S16_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];

	dw <<= 1;

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
			FN_NAME(U16_S16_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S16_1E)(
				dp1, sp1, tab0, tab1, tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_S16_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];
	mlib_s16 *tab3 = (mlib_s16 *)table[3];

	dw <<= 2;

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
			FN_NAME(U16_S16_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S16_1E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_S16_3L(dp, sp, t0, t1, t2)    \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;              \
		                                                      \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,        \
		c4, c5, c6, c7, sp)                                   \
		dd1 = _mm_setr_epi16(                                 \
				t0[c0], t1[c1], t2[c2], t0[c3],               \
				t1[c4], t2[c5], t0[c6], t1[c7]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_S16_3L(dp, sp, t0, t1, t2)    \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;              \
		mlib_u32 e0, e1, e2, e3;                              \
		                                                      \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,        \
		c4, c5, c6, c7, sp)                                   \
		                                                      \
		e0 = (t0[c0] & 0xFFFF) | (t1[c1] << 16);              \
		e1 = (t2[c2] & 0xFFFF) | (t0[c3] << 16);              \
		e2 = (t1[c4] & 0xFFFF) | (t2[c5] << 16);              \
		e3 = (t0[c6] & 0xFFFF) | (t1[c7] << 16);              \
		((mlib_u32 *)dp)[0] = e0;                             \
		((mlib_u32 *)dp)[1] = e1;                             \
		((mlib_u32 *)dp)[2] = e2;                             \
		((mlib_u32 *)dp)[3] = e3;                             \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U16_S16_3E(                  \
			dp, sp, t0, t1, t2, n)                       \
	{                                                    \
		for (k = 0; k < n - 2; k += 3) {                 \
			((mlib_s16 *)dp)[k] =                        \
				t0[((STYPE *)sp)[k]];                    \
			((mlib_s16 *)dp)[k + 1] =                    \
				t1[((STYPE *)sp)[k + 1]];                \
			((mlib_s16 *)dp)[k + 2] =                    \
				t2[((STYPE *)sp)[k + 2]];                \
		}                                                \
		if (k < n) {                                     \
			((mlib_s16 *)dp)[k] =                        \
				t0[((STYPE *)sp)[k]];                    \
			if (k < n - 1) {                             \
				((mlib_s16 *)dp)[k + 1] =                \
					t1[((STYPE *)sp)[k + 1]];            \
			}                                            \
		}                                                \
	}
/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_S16_3(
    const mlib_u16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	mlib_s16 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	tab0 = (mlib_s16 *)table[0];
	tab1 = (mlib_s16 *)table[1];
	tab2 = (mlib_s16 *)table[2];

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 23; n1 -= 24) {
			FN_NAME(U16_S16_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1++;

			FN_NAME(U16_S16_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1++;

			FN_NAME(U16_S16_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S16_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}


}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_2L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
                                                            \
		dd1 = _mm_setr_epi16(                               \
				t0[c0], t1[c0], t2[c1], t3[c1],             \
				t0[c2], t1[c2], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c4], t1[c4], t2[c5], t3[c5],             \
				t0[c6], t1[c6], t2[c7], t3[c7]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_2L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		mlib_u32 e0, e1, e2, e3;                            \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
  		                                                    \
		e0 = (t0[c0] & 0xFFFF) | (t1[c0] << 16);            \
		e1 = (t2[c1] & 0xFFFF) | (t3[c1] << 16);            \
		e2 = (t0[c2] & 0xFFFF) | (t1[c2] << 16);            \
		e3 = (t2[c3] & 0xFFFF) | (t3[c3] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c4] & 0xFFFF) | (t1[c4] << 16);            \
		e1 = (t2[c5] & 0xFFFF) | (t3[c5] << 16);            \
		e2 = (t0[c6] & 0xFFFF) | (t1[c6] << 16);            \
		e3 = (t2[c7] & 0xFFFF) | (t3[c7] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_2E(                  \
			dp, sp, t0, t1, n)                              \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 2, j++) {               \
			((mlib_s16 *)dp)[k] =                           \
				((mlib_s16 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 1] =                       \
				((mlib_s16 *)t1)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_U16_S16_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, j, k;

/* edge mask */

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
			FN_NAME(SI_U16_S16_2L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_S16_2E)(
				dp1, sp1, tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_3L(dp, sp, t0,       \
	t1, t2)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
                                                            \
		dd1 = _mm_setr_epi16(                               \
			tab0[c0], tab1[c0], tab2[c0], tab0[c1],         \
			tab1[c1], tab2[c1], tab0[c2], tab1[c2]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
			tab2[c2], tab0[c3], tab1[c3], tab2[c3],         \
			tab0[c4], tab1[c4], tab2[c4], tab0[c5]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
			tab1[c5], tab2[c5], tab0[c6], tab1[c6],         \
			tab2[c6], tab0[c7], tab1[c7], tab2[c7]);        \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_3L(dp, sp, t0,       \
	t1, t2)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		mlib_u32 e0, e1, e2, e3;                            \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
  	                                                        \
		e0 = (t0[c0] & 0xFFFF) | (t1[c0] << 16);            \
		e1 = (t2[c0] & 0xFFFF) | (t0[c1] << 16);            \
		e2 = (t1[c1] & 0xFFFF) | (t2[c1] << 16);            \
		e3 = (t0[c2] & 0xFFFF) | (t1[c2] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 	                                                        \
		e0 = (t2[c2] & 0xFFFF) | (t0[c3] << 16);            \
		e1 = (t1[c3] & 0xFFFF) | (t2[c3] << 16);            \
		e2 = (t0[c4] & 0xFFFF) | (t1[c4] << 16);            \
		e3 = (t2[c4] & 0xFFFF) | (t0[c5] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 	                                                        \
		e0 = (t1[c5] & 0xFFFF) | (t2[c5] << 16);            \
		e1 = (t0[c6] & 0xFFFF) | (t1[c6] << 16);            \
		e2 = (t2[c6] & 0xFFFF) | (t0[c7] << 16);            \
		e3 = (t1[c7] & 0xFFFF) | (t2[c7] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_3E(                  \
			dp, sp, t0, t1, t2, n)                          \
{                                                           \
		for (k = 0, j = 0; k < n; k += 3, j++) {            \
			((mlib_s16 *)dp)[k] =                           \
				t0[((STYPE *)sp)[j]];                       \
			((mlib_s16 *)dp)[k + 1] =                       \
				t1[((STYPE *)sp)[j]];                       \
			((mlib_s16 *)dp)[k + 2] =                       \
				t2[((STYPE *)sp)[j]];                       \
		}                                                   \
}

/* *********************************************************** */
void
mlib_s_ImageLookUpSI_U16_S16_3(
    const mlib_u16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

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
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 23; n1 -= 24) {
			FN_NAME(SI_U16_S16_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1++;
		}
		if (n1 > 0) {
			FN_NAME(SI_S16_S16_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
                                                            \
		dd1 = _mm_setr_epi16(                               \
				t0[c0], t1[c0], t2[c0], t3[c0],             \
				t0[c1], t1[c1], t2[c1], t3[c1]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c2], t1[c2], t2[c2], t3[c2],             \
				t0[c3], t1[c3], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c4], t1[c4], t2[c4], t3[c4],             \
				t0[c5], t1[c5], t2[c5], t3[c5]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi16(                               \
				t0[c6], t1[c6], t2[c6], t3[c6],             \
				t0[c7], t1[c7], t2[c7], t3[c7]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
		mlib_u32 e0, e1, e2, e3;                            \
		                                                    \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		c4, c5, c6, c7, sp)                                 \
  		                                                    \
		e0 = (t0[c0] & 0xFFFF) | (t1[c0] << 16);            \
		e1 = (t2[c0] & 0xFFFF) | (t3[c0] << 16);            \
		e2 = (t0[c1] & 0xFFFF) | (t1[c1] << 16);            \
		e3 = (t2[c1] & 0xFFFF) | (t3[c1] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c2] & 0xFFFF) | (t1[c2] << 16);            \
		e1 = (t2[c2] & 0xFFFF) | (t3[c2] << 16);            \
		e2 = (t0[c3] & 0xFFFF) | (t1[c3] << 16);            \
		e3 = (t2[c3] & 0xFFFF) | (t3[c3] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c4] & 0xFFFF) | (t1[c4] << 16);            \
		e1 = (t2[c4] & 0xFFFF) | (t3[c4] << 16);            \
		e2 = (t0[c5] & 0xFFFF) | (t1[c5] << 16);            \
		e3 = (t2[c5] & 0xFFFF) | (t3[c5] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
 		                                                    \
		e0 = (t0[c6] & 0xFFFF) | (t1[c6] << 16);            \
		e1 = (t2[c6] & 0xFFFF) | (t3[c6] << 16);            \
		e2 = (t0[c7] & 0xFFFF) | (t1[c7] << 16);            \
		e3 = (t2[c7] & 0xFFFF) | (t3[c7] << 16);            \
		((mlib_u32 *)dp)[0] = e0;                           \
		((mlib_u32 *)dp)[1] = e1;                           \
		((mlib_u32 *)dp)[2] = e2;                           \
		((mlib_u32 *)dp)[3] = e3;                           \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S16_4E(                  \
			dp, sp, t0, t1, t2, t3, n)                      \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 4, j++) {            \
			((mlib_s16 *)dp)[k] =                           \
				((mlib_s16 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 1] =                       \
				((mlib_s16 *)t1)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 2] =                       \
				((mlib_s16 *)t2)[((STYPE *)sp)[j]];         \
			((mlib_s16 *)dp)[k + 3] =                       \
				((mlib_s16 *)t3)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_U16_S16_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

/* edge mask */

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];
	mlib_s16 *tab3 = (mlib_s16 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 31; n1 -= 32) {
			FN_NAME(SI_U16_S16_4L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_S16_4E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

#undef	STYPE
#define	STYPE  mlib_s16

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_S32_1L(             \
			dp, sp, t0, t1, t2, t3)                 \
	{                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;    \
		                                            \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,         \
		c4, c5, c6, c7, sp)                         \
                                                    \
		dd1 = _mm_setr_epi32(                       \
				t0[c0], t1[c1], t2[c2], t3[c3]);    \
		_mm_storeu_si128(dp, dd1);                  \
		dp++;                                       \
		dd1 = _mm_setr_epi32(                       \
				t0[c4], t1[c5], t2[c6], t3[c7]);    \
		_mm_storeu_si128(dp, dd1);                  \
		dp++;                                       \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_S32_1L(             \
			dp, sp, t0, t1, t2, t3)                 \
	{                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;    \
		                                            \
	    FN_NAME(S16_2INDEX)(c0, c1, c2, c3,         \
		c4, c5, c6, c7, sp)                         \
                                                    \
		((mlib_s32 *)dp)[0] = t0[c0];               \
		((mlib_s32 *)dp)[1] = t1[c1];               \
		((mlib_s32 *)dp)[2] = t2[c2];               \
		((mlib_s32 *)dp)[3] = t3[c3];               \
		dp++;                                       \
		((mlib_s32 *)dp)[0] = t0[c4];               \
		((mlib_s32 *)dp)[1] = t1[c5];               \
		((mlib_s32 *)dp)[2] = t2[c6];               \
		((mlib_s32 *)dp)[3] = t3[c7];               \
		dp++;                                       \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_S16_S32_1E(             \
			dp, sp, t0, t1, t2, t3, n)              \
	{                                               \
		for (k = 0; k < n - 3; k += 4) {            \
			((mlib_s32 *)dp)[k] =                   \
				t0[((STYPE *)sp)[k]];               \
			((mlib_s32 *)dp)[k + 1] =               \
				t1[((STYPE *)sp)[k + 1]];           \
			((mlib_s32 *)dp)[k + 2] =               \
				t2[((STYPE *)sp)[k + 2]];           \
			((mlib_s32 *)dp)[k + 3] =               \
				t3[((STYPE *)sp)[k + 3]];           \
		}                                           \
		if (k < n) {                                \
			((mlib_s32 *)dp)[k] =                   \
				t0[((STYPE *)sp)[k]];               \
			if (k < n - 1) {                        \
				((mlib_s32 *)dp)[k + 1] =           \
					t1[((STYPE *)sp)[k + 1]];       \
				if (k < n - 2) {                    \
					((mlib_s32 *)dp)[k + 2] =       \
						t2[((STYPE *)sp)[k + 2]];   \
				}                                   \
			}                                       \
		}                                           \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUp_S16_S32_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_s32 *tab0 = &table[0][32768];

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
			FN_NAME(S16_S32_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S32_1E)(
				dp1, sp1, tab0, tab0,
				tab0, tab0, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S32_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];

	dw <<= 1;

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
			FN_NAME(S16_S32_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S32_1E)(
				dp1, sp1, tab0, tab1,
				tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S32_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];
	const mlib_s32 *tab2 = &table[2][32768];
	const mlib_s32 *tab3 = &table[3][32768];

	dw <<= 2;

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
			FN_NAME(S16_S32_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(S16_S32_1E)(
				dp1, sp1, tab0, tab1,
				tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_S16_S32_3L(dp, sp, t0,           \
	t1, t2)                                                   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(c0, c1, c2, c3, sp) \
                                                              \
		dd1 = _mm_setr_epi32(                                 \
				t0[c0], t1[c1], t2[c2], t0[c3]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_S16_S32_3L(dp, sp, t0,           \
	t1, t2)                                                   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(c0, c1, c2, c3, sp) \
                                                              \
		((mlib_s32 *)dp)[0] = t0[c0];                         \
		((mlib_s32 *)dp)[1] = t1[c1];                         \
		((mlib_s32 *)dp)[2] = t2[c2];                         \
		((mlib_s32 *)dp)[3] = t0[c3];                         \
		dp++;                                                 \
	}


#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_S16_S32_3E(                   \
			dp, sp, t0, t1, t2, n)                        \
	{                                                     \
		for (k = 0; k < n - 2; k += 3) {                  \
			((mlib_s32 *)dp)[k] =                         \
				t0[((STYPE *)sp)[k]];                     \
			((mlib_s32 *)dp)[k + 1] =                     \
				t1[((STYPE *)sp)[k + 1]];                 \
			((mlib_s32 *)dp)[k + 2] =                     \
				t2[((STYPE *)sp)[k + 2]];                 \
		}                                                 \
		if (k < n) {                                      \
			((mlib_s32 *)dp)[k] =                         \
				t0[((STYPE *)sp)[k]];                     \
			if (k < n - 1) {                              \
				((mlib_s32 *)dp)[k + 1] =                 \
					t1[((STYPE *)sp)[k + 1]];             \
			}                                             \
		}                                                 \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUp_S16_S32_3(
    const mlib_s16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2, sd3;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	const mlib_s32 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;

	tab0 = &table[0][32768];
	tab1 = &table[1][32768];
	tab2 = &table[2][32768];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 11; n1 -= 12) {
			FN_NAME(S16_S32_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(S16_S32_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(S16_S32_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(S16_S32_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_2L(dp, sp, t0, t1,  \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		dd1 = _mm_setr_epi32(                               \
				t0[c0], t1[c0], t2[c1], t3[c1]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c2], t1[c2], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_2L(dp, sp, t0, t1,  \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		((mlib_s32 *)dp)[0] = t0[c0];                       \
		((mlib_s32 *)dp)[1] = t1[c0];                       \
		((mlib_s32 *)dp)[2] = t2[c1];                       \
		((mlib_s32 *)dp)[3] = t3[c1];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c2];                       \
		((mlib_s32 *)dp)[1] = t1[c2];                       \
		((mlib_s32 *)dp)[2] = t2[c3];                       \
		((mlib_s32 *)dp)[3] = t3[c3];                       \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_2E(                  \
			dp, sp, t0, t1, n)                              \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 2, j++) {            \
			((mlib_s32 *)dp)[k] =                           \
				((mlib_s32 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 1] =                       \
				((mlib_s32 *)t1)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_S16_S32_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];

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
			FN_NAME(SI_S16_S32_2L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_S32_2E)(
				dp1, sp1, tab0, tab1, n1);
		}


		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_3L(                  \
			dp, sp, t0, t1, t2)                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		dd1 = _mm_setr_epi32(                               \
			tab0[c0], tab1[c0],                             \
			tab2[c0], tab0[c1]);                            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
			tab1[c1], tab2[c1],                             \
			tab0[c2], tab1[c2]);                            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
			tab2[c2], tab0[c3],                             \
			tab1[c3], tab2[c3]);                            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_3L(                  \
			dp, sp, t0, t1, t2)                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		((mlib_s32 *)dp)[0] = t0[c0];                       \
		((mlib_s32 *)dp)[1] = t1[c0];                       \
		((mlib_s32 *)dp)[2] = t2[c0];                       \
		((mlib_s32 *)dp)[3] = t0[c1];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t1[c1];                       \
		((mlib_s32 *)dp)[1] = t2[c1];                       \
		((mlib_s32 *)dp)[2] = t0[c2];                       \
		((mlib_s32 *)dp)[3] = t1[c2];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t2[c2];                       \
		((mlib_s32 *)dp)[1] = t0[c3];                       \
		((mlib_s32 *)dp)[2] = t1[c3];                       \
		((mlib_s32 *)dp)[3] = t2[c3];                       \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_3E(                  \
			dp, sp, t0, t1, t2, n)                          \
{                                                           \
		for (k = 0, j = 0; k < n; k += 3, j++) {            \
			((mlib_s32 *)dp)[k] =                           \
				t0[((STYPE *)sp)[j]];                       \
			((mlib_s32 *)dp)[k + 1] =                       \
				t1[((STYPE *)sp)[j]];                       \
			((mlib_s32 *)dp)[k + 2] =                       \
				t2[((STYPE *)sp)[j]];                       \
		}                                                   \
}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_S16_S32_3(
    const mlib_s16 *src,
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
	mlib_s32 c0, c1, c2, c3;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];
	const mlib_s32 *tab2 = &table[2][32768];

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 11; n1 -= 12) {
			FN_NAME(SI_S16_S32_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_S32_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                            \
                                                            \
		dd1 = _mm_setr_epi32(                               \
				t0[c0], t1[c0], t2[c0], t3[c0]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c1], t1[c1], t2[c1], t3[c1]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c2], t1[c2], t2[c2], t3[c2]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c3], t1[c3], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_S16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                            \
                                                            \
		((mlib_s32 *)dp)[0] = t0[c0];                       \
		((mlib_s32 *)dp)[1] = t1[c0];                       \
		((mlib_s32 *)dp)[2] = t2[c0];                       \
		((mlib_s32 *)dp)[3] = t3[c0];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c1];                       \
		((mlib_s32 *)dp)[1] = t1[c1];                       \
		((mlib_s32 *)dp)[2] = t2[c1];                       \
		((mlib_s32 *)dp)[3] = t3[c1];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c2];                       \
		((mlib_s32 *)dp)[1] = t1[c2];                       \
		((mlib_s32 *)dp)[2] = t2[c2];                       \
		((mlib_s32 *)dp)[3] = t3[c2];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c3];                       \
		((mlib_s32 *)dp)[1] = t1[c3];                       \
		((mlib_s32 *)dp)[2] = t2[c3];                       \
		((mlib_s32 *)dp)[3] = t3[c3];                       \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_S16_S32_4E(                  \
			dp, sp, t0, t1, t2, t3, n)                      \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 4, j++) {            \
			((mlib_s32 *)dp)[k] =                           \
				((mlib_s32 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 1] =                       \
				((mlib_s32 *)t1)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 2] =                       \
				((mlib_s32 *)t2)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 3] =                       \
				((mlib_s32 *)t3)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_S16_S32_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1, dd2;

/* loop variables */
	mlib_s32 n1, j, k;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];
	const mlib_s32 *tab2 = &table[2][32768];
	const mlib_s32 *tab3 = &table[3][32768];

	dw <<= 2;

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
			FN_NAME(SI_S16_S32_4L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_S16_S32_4E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */

#undef	STYPE
#define	STYPE  mlib_u16

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_S32_1L(               \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;      \
		                                              \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,\
		c4, c5, c6, c7, sp)                           \
                                                      \
		dd1 = _mm_setr_epi32(                         \
				t0[c0], t1[c1], t2[c2], t3[c3]);      \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
		dd1 = _mm_setr_epi32(                         \
				t0[c4], t1[c5], t2[c6], t3[c7]);      \
		_mm_storeu_si128(dp, dd1);                    \
		dp++;                                         \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_S32_1L(               \
			dp, sp, t0, t1, t2, t3)                   \
	{                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;      \
		                                              \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,\
		c4, c5, c6, c7, sp)                           \
                                                      \
		((mlib_s32 *)dp)[0] = t0[c0];                 \
		((mlib_s32 *)dp)[1] = t1[c1];                 \
		((mlib_s32 *)dp)[2] = t2[c2];                 \
		((mlib_s32 *)dp)[3] = t3[c3];                 \
		dp++;                                         \
		((mlib_s32 *)dp)[0] = t0[c4];                 \
		((mlib_s32 *)dp)[1] = t1[c5];                 \
		((mlib_s32 *)dp)[2] = t2[c6];                 \
		((mlib_s32 *)dp)[3] = t3[c7];                 \
		dp++;                                         \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U16_S32_1E(               \
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
mlib_s_ImageLookUp_U16_S32_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

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
			FN_NAME(U16_S32_1L)(
				dp1, sp1, tab0, tab0, tab0, tab0);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S32_1E)(
				dp1, sp1, tab0, tab0,
				tab0, tab0, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_S32_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];

	dw <<= 1;

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
			FN_NAME(U16_S32_1L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S32_1E)(
				dp1, sp1, tab0, tab1,
				tab0, tab1, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_S32_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];
	mlib_s32 *tab3 = (mlib_s32 *)table[3];

	dw <<= 2;

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
			FN_NAME(U16_S32_1L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1++;
		}

		if (n1 > 0) {
			FN_NAME(U16_S32_1E)(
				dp1, sp1, tab0, tab1,
				tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_U16_S32_3L(dp, sp, t0,           \
	t1, t2)                                                   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(c0, c1, c2, c3, sp) \
                                                              \
		dd1 = _mm_setr_epi32(                                 \
				t0[c0], t1[c1], t2[c2], t0[c3]);              \
		_mm_storeu_si128(dp, dd1);                            \
		dp++;                                                 \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_U16_S32_3L(dp, sp, t0,           \
	t1, t2)                                                   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(c0, c1, c2, c3, sp) \
                                                              \
		((mlib_s32 *)dp)[0] = t0[c0];                         \
		((mlib_s32 *)dp)[1] = t1[c1];                         \
		((mlib_s32 *)dp)[2] = t2[c2];                         \
		((mlib_s32 *)dp)[3] = t0[c3];                         \
		dp++;                                                 \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_U16_S32_3E(                   \
			dp, sp, t0, t1, t2, n)                        \
	{                                                     \
		for (k = 0; k < n - 2; k += 3) {                  \
			((mlib_s32 *)dp)[k] =                         \
				t0[((STYPE *)sp)[k]];                     \
			((mlib_s32 *)dp)[k + 1] =                     \
				t1[((STYPE *)sp)[k + 1]];                 \
			((mlib_s32 *)dp)[k + 2] =                     \
				t2[((STYPE *)sp)[k + 2]];                 \
		}                                                 \
		if (k < n) {                                      \
			((mlib_s32 *)dp)[k] =                         \
				t0[((STYPE *)sp)[k]];                     \
			if (k < n - 1) {                              \
				((mlib_s32 *)dp)[k + 1] =                 \
					t1[((STYPE *)sp)[k + 1]];             \
			}                                             \
		}                                                 \
	}


/* *********************************************************** */

void
mlib_s_ImageLookUp_U16_S32_3(
    const mlib_u16 *src,
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
	__m128i *sp1, *sl1, sd1, sd2, sd3;

/* loop variables */
	mlib_s32 n1, k;

/* edge mask */
	mlib_s32 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m128i *) dst;
	sp1 = sl1 = (__m128i *) src;

	dw *= 3;

	tab0 = (mlib_s32 *)table[0];
	tab1 = (mlib_s32 *)table[1];
	tab2 = (mlib_s32 *)table[2];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (; dh > 0; dh--) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 11; n1 -= 12) {
			FN_NAME(U16_S32_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(U16_S32_3L)(
				dp1, sp1, tab1, tab2, tab0);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);

			FN_NAME(U16_S32_3L)(
				dp1, sp1, tab2, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(U16_S32_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_2L(dp, sp, t0, t1,   \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		dd1 = _mm_setr_epi32(                               \
				t0[c0], t1[c0], t2[c1], t3[c1]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c2], t1[c2], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_2L(dp, sp, t0, t1,   \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		((mlib_s32 *)dp)[0] = t0[c0];                       \
		((mlib_s32 *)dp)[1] = t1[c0];                       \
		((mlib_s32 *)dp)[2] = t2[c1];                       \
		((mlib_s32 *)dp)[3] = t3[c1];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c2];                       \
		((mlib_s32 *)dp)[1] = t1[c2];                       \
		((mlib_s32 *)dp)[2] = t2[c3];                       \
		((mlib_s32 *)dp)[3] = t3[c3];                       \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_2E(                  \
			dp, sp, t0, t1, n)                              \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 2, j++) {            \
			((mlib_s32 *)dp)[k] =                           \
				((mlib_s32 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 1] =                       \
				((mlib_s32 *)t1)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U16_S32_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1, k, j;

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
			FN_NAME(SI_U16_S32_2L)(
				dp1, sp1, tab0, tab1, tab0, tab1);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_S32_2E)(
				dp1, sp1, tab0, tab1, n1);
		}


		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}

}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_3L(                  \
			dp, sp, t0, t1, t2)                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		dd1 = _mm_setr_epi32(                               \
			tab0[c0], tab1[c0],                             \
			tab2[c0], tab0[c1]);                            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
			tab1[c1], tab2[c1],                             \
			tab0[c2], tab1[c2]);                            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
			tab2[c2], tab0[c3],                             \
			tab1[c3], tab2[c3]);                            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}

#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_3L(                  \
			dp, sp, t0, t1, t2)                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		((mlib_s32 *)dp)[0] = t0[c0];                       \
		((mlib_s32 *)dp)[1] = t1[c0];                       \
		((mlib_s32 *)dp)[2] = t2[c0];                       \
		((mlib_s32 *)dp)[3] = t0[c1];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t1[c1];                       \
		((mlib_s32 *)dp)[1] = t2[c1];                       \
		((mlib_s32 *)dp)[2] = t0[c2];                       \
		((mlib_s32 *)dp)[3] = t1[c2];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t2[c2];                       \
		((mlib_s32 *)dp)[1] = t0[c3];                       \
		((mlib_s32 *)dp)[2] = t1[c3];                       \
		((mlib_s32 *)dp)[3] = t2[c3];                       \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_3E(                  \
			dp, sp, t0, t1, t2, n)                          \
{                                                           \
		for (k = 0, j = 0; k < n; k += 3, j++) {            \
			((mlib_s32 *)dp)[k] =                           \
				t0[((STYPE *)sp)[j]];                       \
			((mlib_s32 *)dp)[k + 1] =                       \
				t1[((STYPE *)sp)[j]];                       \
			((mlib_s32 *)dp)[k + 2] =                       \
				t2[((STYPE *)sp)[j]];                       \
		}                                                   \
}

/* *********************************************************** */


void
mlib_s_ImageLookUpSI_U16_S32_3(
    const mlib_u16 *src,
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
	mlib_s32 c0, c1, c2, c3;

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
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (n1 = dw; n1 > 11; n1 -= 12) {
			FN_NAME(SI_U16_S32_3L)(
				dp1, sp1, tab0, tab1, tab2);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_S32_3E)(
				dp1, sp1, tab0, tab1, tab2, n1);
		}

		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);

	}
}

/* *********************************************************** */
#ifdef LOOKUP_SSE2 /* SSE2 version for LookUp, bad performance now!! */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		dd1 = _mm_setr_epi32(                               \
				t0[c0], t1[c0], t2[c0], t3[c0]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c1], t1[c1], t2[c1], t3[c1]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c2], t1[c2], t2[c2], t3[c2]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
		dd1 = _mm_setr_epi32(                               \
				t0[c3], t1[c3], t2[c3], t3[c3]);            \
		_mm_storeu_si128(dp, dd1);                          \
		dp++;                                               \
	}
#else /* C version for LookUp, better than STDC version */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_4L(dp, sp, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
	                                                        \
	    MLIB_S_IMAGE_LOOKUP_U16_2INDEX_64(                  \
			c0, c1, c2, c3, sp)                             \
                                                            \
		((mlib_s32 *)dp)[0] = t0[c0];                       \
		((mlib_s32 *)dp)[1] = t1[c0];                       \
		((mlib_s32 *)dp)[2] = t2[c0];                       \
		((mlib_s32 *)dp)[3] = t3[c0];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c1];                       \
		((mlib_s32 *)dp)[1] = t1[c1];                       \
		((mlib_s32 *)dp)[2] = t2[c1];                       \
		((mlib_s32 *)dp)[3] = t3[c1];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c2];                       \
		((mlib_s32 *)dp)[1] = t1[c2];                       \
		((mlib_s32 *)dp)[2] = t2[c2];                       \
		((mlib_s32 *)dp)[3] = t3[c2];                       \
		dp++;                                               \
		((mlib_s32 *)dp)[0] = t0[c3];                       \
		((mlib_s32 *)dp)[1] = t1[c3];                       \
		((mlib_s32 *)dp)[2] = t2[c3];                       \
		((mlib_s32 *)dp)[3] = t3[c3];                       \
		dp++;                                               \
	}

#endif /* End of LOOKUP_SSE2 */
/* *********************************************************** */

#define	MLIB_S_IMAGE_LOOKUP_SI_U16_S32_4E(                  \
			dp, sp, t0, t1, t2, t3, n)                      \
	{                                                       \
		for (k = 0, j = 0; k < n; k += 4, j++) {            \
			((mlib_s32 *)dp)[k] =                           \
				((mlib_s32 *)t0)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 1] =                       \
				((mlib_s32 *)t1)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 2] =                       \
				((mlib_s32 *)t2)[((STYPE *)sp)[j]];         \
			((mlib_s32 *)dp)[k + 3] =                       \
				((mlib_s32 *)t3)[((STYPE *)sp)[j]];         \
		}                                                   \
	}

/* *********************************************************** */

void
mlib_s_ImageLookUpSI_U16_S32_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m128i *sp1, *sl1, sd1;

/* dst address */
	__m128i *dp1, *dl1, dd1, dd2;

/* loop variables */
	mlib_s32 n1, k, j;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];
	mlib_s32 *tab3 = (mlib_s32 *)table[3];

	dw <<= 2;

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
			FN_NAME(SI_U16_S32_4L)(
				dp1, sp1, tab0, tab1, tab2, tab3);
			sp1 = (__m128i *)(((__m64 *)sp1) + 1);
		}

		if (n1 > 0) {
			FN_NAME(SI_U16_S32_4E)(
				dp1, sp1, tab0, tab1, tab2, tab3, n1);
		}

		dp1 = dl1 = (__m128i *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m128i *) ((mlib_u8 *)sl1 + slb);
	}


}

/* *********************************************************** */
