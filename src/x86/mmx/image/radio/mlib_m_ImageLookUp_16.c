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

#pragma ident	"@(#)mlib_m_ImageLookUp_16.c	9.4	07/11/05 SMI"

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
#include <mlib_mmx_utils.h>

/* *********************************************************** */

static const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(8))
#elif __SUNPRO_C
#pragma align 8(ld4_s16)
#endif /* defined(__ICL) || defined(__INTEL_COMPILER) */
	__m64 ld4_s16[] = {
		0x0000000000000000, 0x000000000000ffff,
		0x00000000ffffffff, 0x0000ffffffffffff,
		0xffffffffffffffff
	};

/* *********************************************************** */

#define	FN_NAME(F)	MLIB_M_IMAGE_LOOKUP_##F

#define	MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1)     \
	{                                                       \
	    __m64 a0 = _mm_unpacklo_pi16(_m_zero, sd1);         \
	    __m64 a1 = _mm_unpackhi_pi16(_m_zero, sd1);         \
	    __m64 b0 = _mm_srai_pi32(a0, 16);                   \
	    __m64 b2 = _mm_srai_pi32(a1, 16);                   \
	    __m64 b1 = _mm_srli_si64(b0, 32);                   \
	    __m64 b3 = _mm_srli_si64(b2, 32);                   \
	                                                        \
	    c0 = _mm_cvtsi64_si32(b0);                          \
	    c1 = _mm_cvtsi64_si32(b1);                          \
	    c2 = _mm_cvtsi64_si32(b2);                          \
	    c3 = _mm_cvtsi64_si32(b3);                          \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_U8_1L(dp, sd1, sd2, t0, t1,           \
	t2, t3)                                                       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_u8 *)dp)[0] = t0[c0];                     \
	    ((mlib_u8 *)dp)[1] = t1[c1];                              \
	    ((mlib_u8 *)dp)[2] = t2[c2];                              \
	    ((mlib_u8 *)dp)[3] = t3[c3];                              \
	    ((mlib_u8 *)dp)[4] = t0[c4];                              \
	    ((mlib_u8 *)dp)[5] = t1[c5];                              \
	    ((mlib_u8 *)dp)[6] = t2[c6];                              \
	    ((mlib_u8 *)dp)[7] = t3[c7];                              \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_U8_1E(dp, sd1, sd2, t0, t1,           \
	t2, t3)                                                       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) dp =                                             \
		_mm_mlib_setr_pi8(t0[c0], t1[c1], t2[c2], t3[c3],     \
		t0[c4], t1[c5], t2[c6], t3[c7]);                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_U8_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_u8 *tab0 = &table[0][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_U8_1L(dp1, sd1, sd2, tab0, tab0,
			    tab0, tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_S16_U8_1E(dd1, sd1, sd2, tab0, tab0,
			    tab0, tab0);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_U8_1L(dp1, sd1, sd2, tab0, tab1,
			    tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_S16_U8_1E(dd1, sd1, sd2, tab0, tab1,
			    tab0, tab1);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];
	const mlib_u8 *tab2 = &table[2][32768];
	const mlib_u8 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_U8_1L(dp1, sd1, sd2, tab0, tab1,
			    tab2, tab3);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_S16_U8_1E(dd1, sd1, sd2, tab0, tab1,
			    tab2, tab3);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_U8_3L(dp, sd1, sd2, t0, t1, t2)       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_u8 *)dp)[0] = t0[c0];                     \
	    ((mlib_u8 *)dp)[1] = t1[c1];                              \
	    ((mlib_u8 *)dp)[2] = t2[c2];                              \
	    ((mlib_u8 *)dp)[3] = t0[c3];                              \
	    ((mlib_u8 *)dp)[4] = t1[c4];                              \
	    ((mlib_u8 *)dp)[5] = t2[c5];                              \
	    ((mlib_u8 *)dp)[6] = t0[c6];                              \
	    ((mlib_u8 *)dp)[7] = t1[c7];                              \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_U8_3E(dp, sd1, sd2, t0, t1, t2)       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) dp =                                             \
		_mm_mlib_setr_pi8(t0[c0], t1[c1], t2[c2], t0[c3],     \
		t1[c4], t2[c5], t0[c6], t1[c7]);                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_U8_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;
	const mlib_u8 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = &table[0][32768];
		tab1 = &table[1][32768];
		tab2 = &table[2][32768];

		dend = (mlib_u8 *)dp1 + dw;

		for (n1 = dw; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_U8_3L(dp1, sd1, sd2, tab0, tab1,
			    tab2);
			dp1++;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_U8_3L(dp1, sd1, sd2, tab2, tab0,
			    tab1);
			dp1++;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_U8_3L(dp1, sd1, sd2, tab1, tab2,
			    tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			n1 = (n1 > 7) ? 8 : n1;
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_S16_U8_3E(dd1, sd1, sd2, tab0, tab1,
			    tab2);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				n1 = (n1 > 7) ? 8 : n1;
				sd1 =
				    _mm_and_si64(sp1[2],
				    ld4_s16[((n1 > 4) ? 4 : n1)]);
				sd2 =
				    _mm_and_si64(sp1[3],
				    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
				MLIB_M_IMAGE_LOOKUP_S16_U8_3E(dd1, sd1, sd2,
				    tab2, tab0, tab1);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
				if (n1 > 0) {
					emask = (0xFF << (8 - n1));
					sd1 =
					    _mm_and_si64(sp1[4],
					    ld4_s16[((n1 > 4) ? 4 : n1)]);
					sd2 =
					    _mm_and_si64(sp1[5],
					    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
					MLIB_M_IMAGE_LOOKUP_S16_U8_3E(dd1, sd1,
					    sd2, tab1, tab2, tab0);
					mmx_pst_8(dd1, dp1, emask);
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_U8_2L(dp, sd1, t0, t1, t2, t3)   \
	{                                                           \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                \
	                                                            \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,          \
		sd1) ((mlib_u8 *)dp)[0] = t0[c0];                   \
	    ((mlib_u8 *)dp)[1] = t1[c0];                            \
	    ((mlib_u8 *)dp)[2] = t2[c1];                            \
	    ((mlib_u8 *)dp)[3] = t3[c1];                            \
	    ((mlib_u8 *)dp)[4] = t0[c2];                            \
	    ((mlib_u8 *)dp)[5] = t1[c2];                            \
	    ((mlib_u8 *)dp)[6] = t2[c3];                            \
	    ((mlib_u8 *)dp)[7] = t3[c3];                            \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_U8_2E(dp, sd1, t0, t1, t2, t3)      \
	{                                                              \
	    mlib_s32 c0, c1, c2, c3;                                   \
	                                                               \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1) dp =   \
		_mm_mlib_setr_pi8(t0[c0], t1[c0], t2[c1], t3[c1],      \
		t0[c2], t1[c2], t2[c3], t3[c3]);                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_U8_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1 >> 1]);
			MLIB_M_IMAGE_LOOKUP_SI_S16_U8_2E(dd1, sd1, tab0, tab1,
			    tab0, tab1);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_U8_3L(dp, sd1, t0, t1, t2)         \
	{                                                             \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_u8 *)dp)[0] = t0[c0];                     \
	    ((mlib_u8 *)dp)[1] = t1[c0];                              \
	    ((mlib_u8 *)dp)[2] = t2[c0];                              \
	    ((mlib_u8 *)dp)[3] = t0[c1];                              \
	    ((mlib_u8 *)dp)[4] = t1[c1];                              \
	    ((mlib_u8 *)dp)[5] = t2[c1];                              \
	    ((mlib_u8 *)dp)[6] = t0[c2];                              \
	    ((mlib_u8 *)dp)[7] = t1[c2];                              \
	    ((mlib_u8 *)dp)[8] = t2[c2];                              \
	    ((mlib_u8 *)dp)[9] = t0[c3];                              \
	    ((mlib_u8 *)dp)[10] = t1[c3];                             \
	    ((mlib_u8 *)dp)[11] = t2[c3];                             \
	    ((mlib_u8 *)dp)[12] = t0[c4];                             \
	    ((mlib_u8 *)dp)[13] = t1[c4];                             \
	    ((mlib_u8 *)dp)[14] = t2[c4];                             \
	    ((mlib_u8 *)dp)[15] = t0[c5];                             \
	    ((mlib_u8 *)dp)[16] = t1[c5];                             \
	    ((mlib_u8 *)dp)[17] = t2[c5];                             \
	    ((mlib_u8 *)dp)[18] = t0[c6];                             \
	    ((mlib_u8 *)dp)[19] = t1[c6];                             \
	    ((mlib_u8 *)dp)[20] = t2[c6];                             \
	    ((mlib_u8 *)dp)[21] = t0[c7];                             \
	    ((mlib_u8 *)dp)[22] = t1[c7];                             \
	    ((mlib_u8 *)dp)[23] = t2[c7];                             \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_U8_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_u8 *dend;

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];
	const mlib_u8 *tab2 = &table[2][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = sp1[0];
			sd2 = sp1[1];
			MLIB_M_IMAGE_LOOKUP_SI_S16_U8_3L(dp1, sd1, tab0, tab1,
			    tab2);
			sp1 += 2;
			dp1 += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			n1 = n1 / 3;
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1)
			    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7, sd2)
			    dd1 =
			    _mm_mlib_setr_pi8(tab0[c0], tab1[c0], tab2[c0],
			    tab0[c1], tab1[c1], tab2[c1], tab0[c2], tab1[c2]);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				dd1 =
				    _mm_mlib_setr_pi8(tab2[c2], tab0[c3],
				    tab1[c3], tab2[c3], tab0[c4], tab1[c4],
				    tab2[c4], tab0[c5]);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;
			}

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				dd1 =
				    _mm_mlib_setr_pi8(tab1[c5], tab2[c5],
				    tab0[c6], tab1[c6], tab2[c6], tab0[c7],
				    tab1[c7], tab2[c7]);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_U8_4L(dp, sd1, t0, t1, t2, t3)   \
	{                                                           \
	    mlib_s32 c0, c1, c2, c3;                                \
	                                                            \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,          \
		sd1) ((mlib_u8 *)dp)[0] = t0[c0];                   \
	    ((mlib_u8 *)dp)[1] = t1[c0];                            \
	    ((mlib_u8 *)dp)[2] = t2[c0];                            \
	    ((mlib_u8 *)dp)[3] = t3[c0];                            \
	    ((mlib_u8 *)dp)[4] = t0[c1];                            \
	    ((mlib_u8 *)dp)[5] = t1[c1];                            \
	    ((mlib_u8 *)dp)[6] = t2[c1];                            \
	    ((mlib_u8 *)dp)[7] = t3[c1];                            \
	    ((mlib_u8 *)dp)[8] = t0[c2];                            \
	    ((mlib_u8 *)dp)[9] = t1[c2];                            \
	    ((mlib_u8 *)dp)[10] = t2[c2];                           \
	    ((mlib_u8 *)dp)[11] = t3[c2];                           \
	    ((mlib_u8 *)dp)[12] = t0[c3];                           \
	    ((mlib_u8 *)dp)[13] = t1[c3];                           \
	    ((mlib_u8 *)dp)[14] = t2[c3];                           \
	    ((mlib_u8 *)dp)[15] = t3[c3];                           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;

	const mlib_u8 *tab0 = &table[0][32768];
	const mlib_u8 *tab1 = &table[1][32768];
	const mlib_u8 *tab2 = &table[2][32768];
	const mlib_u8 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_u8 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_U8_4L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1 >> 2]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 =
			    _mm_mlib_setr_pi8(tab0[c0], tab1[c0], tab2[c0],
			    tab3[c0], tab0[c1], tab1[c1], tab2[c1], tab3[c1]);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				sd1 = _mm_and_si64(sp1[1], ld4_s16[n1 >> 2]);
				dd1 =
				    _mm_mlib_setr_pi8(tab0[c2], tab1[c2],
				    tab2[c2], tab3[c2], tab0[c3], tab1[c3],
				    tab2[c3], tab3[c3]);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1)     \
	{                                                       \
	    __m64 b0 = _mm_unpacklo_pi16(sd1, _m_zero);         \
	    __m64 b2 = _mm_unpackhi_pi16(sd1, _m_zero);         \
	    __m64 b1 = _mm_srli_si64(b0, 32);                   \
	    __m64 b3 = _mm_srli_si64(b2, 32);                   \
	                                                        \
	    c0 = _mm_cvtsi64_si32(b0);                          \
	    c1 = _mm_cvtsi64_si32(b1);                          \
	    c2 = _mm_cvtsi64_si32(b2);                          \
	    c3 = _mm_cvtsi64_si32(b3);                          \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_U8_1L(dp, sd1, sd2, t0, t1,           \
	t2, t3)                                                       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_u8 *)dp)[0] = t0[c0];                     \
	    ((mlib_u8 *)dp)[1] = t1[c1];                              \
	    ((mlib_u8 *)dp)[2] = t2[c2];                              \
	    ((mlib_u8 *)dp)[3] = t3[c3];                              \
	    ((mlib_u8 *)dp)[4] = t0[c4];                              \
	    ((mlib_u8 *)dp)[5] = t1[c5];                              \
	    ((mlib_u8 *)dp)[6] = t2[c6];                              \
	    ((mlib_u8 *)dp)[7] = t3[c7];                              \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_U8_1E(dp, sd1, sd2, t0, t1,           \
	t2, t3)                                                       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) dp =                                             \
		_mm_mlib_setr_pi8(t0[c0], t1[c1], t2[c2], t3[c3],     \
		t0[c4], t1[c5], t2[c6], t3[c7]);                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_U8_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_U8_1L(dp1, sd1, sd2, tab0, tab0,
			    tab0, tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_U16_U8_1E(dd1, sd1, sd2, tab0, tab0,
			    tab0, tab0);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_U8_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_U8_1L(dp1, sd1, sd2, tab0, tab1,
			    tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_U16_U8_1E(dd1, sd1, sd2, tab0, tab1,
			    tab0, tab1);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_U8_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];
	mlib_u8 *tab2 = (mlib_u8 *)table[2];
	mlib_u8 *tab3 = (mlib_u8 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_U8_1L(dp1, sd1, sd2, tab0, tab1,
			    tab2, tab3);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_U16_U8_1E(dd1, sd1, sd2, tab0, tab1,
			    tab2, tab3);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_U8_3L(dp, sd1, sd2, t0, t1, t2)       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_u8 *)dp)[0] = t0[c0];                     \
	    ((mlib_u8 *)dp)[1] = t1[c1];                              \
	    ((mlib_u8 *)dp)[2] = t2[c2];                              \
	    ((mlib_u8 *)dp)[3] = t0[c3];                              \
	    ((mlib_u8 *)dp)[4] = t1[c4];                              \
	    ((mlib_u8 *)dp)[5] = t2[c5];                              \
	    ((mlib_u8 *)dp)[6] = t0[c6];                              \
	    ((mlib_u8 *)dp)[7] = t1[c7];                              \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_U8_3E(dp, sd1, sd2, t0, t1, t2)       \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) dp =                                             \
		_mm_mlib_setr_pi8(t0[c0], t1[c1], t2[c2], t0[c3],     \
		t1[c4], t2[c5], t0[c6], t1[c7]);                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_U8_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;
	mlib_u8 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = (mlib_u8 *)table[0];
		tab1 = (mlib_u8 *)table[1];
		tab2 = (mlib_u8 *)table[2];

		dend = (mlib_u8 *)dp1 + dw;

		for (n1 = dw; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_U8_3L(dp1, sd1, sd2, tab0, tab1,
			    tab2);
			dp1++;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_U8_3L(dp1, sd1, sd2, tab2, tab0,
			    tab1);
			dp1++;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_U8_3L(dp1, sd1, sd2, tab1, tab2,
			    tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			n1 = (n1 > 7) ? 8 : n1;
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_U16_U8_3E(dd1, sd1, sd2, tab0, tab1,
			    tab2);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				n1 = (n1 > 7) ? 8 : n1;
				sd1 =
				    _mm_and_si64(sp1[2],
				    ld4_s16[((n1 > 4) ? 4 : n1)]);
				sd2 =
				    _mm_and_si64(sp1[3],
				    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
				MLIB_M_IMAGE_LOOKUP_U16_U8_3E(dd1, sd1, sd2,
				    tab2, tab0, tab1);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
				if (n1 > 0) {
					emask = (0xFF << (8 - n1));
					sd1 =
					    _mm_and_si64(sp1[4],
					    ld4_s16[((n1 > 4) ? 4 : n1)]);
					sd2 =
					    _mm_and_si64(sp1[5],
					    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
					MLIB_M_IMAGE_LOOKUP_U16_U8_3E(dd1, sd1,
					    sd2, tab1, tab2, tab0);
					mmx_pst_8(dd1, dp1, emask);
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_U8_2L(dp, sd1, t0, t1, t2, t3)   \
	{                                                           \
	    mlib_s32 c0, c1, c2, c3;                                \
	                                                            \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,          \
		sd1) ((mlib_u8 *)dp)[0] = t0[c0];                   \
	    ((mlib_u8 *)dp)[1] = t1[c0];                            \
	    ((mlib_u8 *)dp)[2] = t2[c1];                            \
	    ((mlib_u8 *)dp)[3] = t3[c1];                            \
	    ((mlib_u8 *)dp)[4] = t0[c2];                            \
	    ((mlib_u8 *)dp)[5] = t1[c2];                            \
	    ((mlib_u8 *)dp)[6] = t2[c3];                            \
	    ((mlib_u8 *)dp)[7] = t3[c3];                            \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_U8_2E(dp, sd1, t0, t1, t2, t3)      \
	{                                                              \
	    mlib_s32 c0, c1, c2, c3;                                   \
	                                                               \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1) dp =   \
		_mm_mlib_setr_pi8(t0[c0], t1[c0], t2[c1], t3[c1],      \
		t0[c2], t1[c2], t2[c3], t3[c3]);                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_U8_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_U8_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1 >> 1]);
			MLIB_M_IMAGE_LOOKUP_SI_U16_U8_2E(dd1, sd1, tab0, tab1,
			    tab0, tab1);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_U8_3L(dp, sd1, t0, t1, t2)         \
	{                                                             \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_u8 *)dp)[0] = t0[c0];                     \
	    ((mlib_u8 *)dp)[1] = t1[c0];                              \
	    ((mlib_u8 *)dp)[2] = t2[c0];                              \
	    ((mlib_u8 *)dp)[3] = t0[c1];                              \
	    ((mlib_u8 *)dp)[4] = t1[c1];                              \
	    ((mlib_u8 *)dp)[5] = t2[c1];                              \
	    ((mlib_u8 *)dp)[6] = t0[c2];                              \
	    ((mlib_u8 *)dp)[7] = t1[c2];                              \
	    ((mlib_u8 *)dp)[8] = t2[c2];                              \
	    ((mlib_u8 *)dp)[9] = t0[c3];                              \
	    ((mlib_u8 *)dp)[10] = t1[c3];                             \
	    ((mlib_u8 *)dp)[11] = t2[c3];                             \
	    ((mlib_u8 *)dp)[12] = t0[c4];                             \
	    ((mlib_u8 *)dp)[13] = t1[c4];                             \
	    ((mlib_u8 *)dp)[14] = t2[c4];                             \
	    ((mlib_u8 *)dp)[15] = t0[c5];                             \
	    ((mlib_u8 *)dp)[16] = t1[c5];                             \
	    ((mlib_u8 *)dp)[17] = t2[c5];                             \
	    ((mlib_u8 *)dp)[18] = t0[c6];                             \
	    ((mlib_u8 *)dp)[19] = t1[c6];                             \
	    ((mlib_u8 *)dp)[20] = t2[c6];                             \
	    ((mlib_u8 *)dp)[21] = t0[c7];                             \
	    ((mlib_u8 *)dp)[22] = t1[c7];                             \
	    ((mlib_u8 *)dp)[23] = t2[c7];                             \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_U8_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_u8 *dend;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];
	mlib_u8 *tab2 = (mlib_u8 *)table[2];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = sp1[0];
			sd2 = sp1[1];
			MLIB_M_IMAGE_LOOKUP_SI_U16_U8_3L(dp1, sd1, tab0, tab1,
			    tab2);
			sp1 += 2;
			dp1 += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			n1 = n1 / 3;
			sd1 =
			    _mm_and_si64(sp1[0], ld4_s16[((n1 > 4) ? 4 : n1)]);
			sd2 =
			    _mm_and_si64(sp1[1],
			    ld4_s16[((n1 > 4) ? n1 - 4 : 0)]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1)
			    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7, sd2)
			    dd1 =
			    _mm_mlib_setr_pi8(tab0[c0], tab1[c0], tab2[c0],
			    tab0[c1], tab1[c1], tab2[c1], tab0[c2], tab1[c2]);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				dd1 =
				    _mm_mlib_setr_pi8(tab2[c2], tab0[c3],
				    tab1[c3], tab2[c3], tab0[c4], tab1[c4],
				    tab2[c4], tab0[c5]);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;
			}

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				dd1 =
				    _mm_mlib_setr_pi8(tab1[c5], tab2[c5],
				    tab0[c6], tab1[c6], tab2[c6], tab0[c7],
				    tab1[c7], tab2[c7]);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_U8_4L(dp, sd1, t0, t1, t2, t3)   \
	{                                                           \
	    mlib_s32 c0, c1, c2, c3;                                \
	                                                            \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,          \
		sd1) ((mlib_u8 *)dp)[0] = t0[c0];                   \
	    ((mlib_u8 *)dp)[1] = t1[c0];                            \
	    ((mlib_u8 *)dp)[2] = t2[c0];                            \
	    ((mlib_u8 *)dp)[3] = t3[c0];                            \
	    ((mlib_u8 *)dp)[4] = t0[c1];                            \
	    ((mlib_u8 *)dp)[5] = t1[c1];                            \
	    ((mlib_u8 *)dp)[6] = t2[c1];                            \
	    ((mlib_u8 *)dp)[7] = t3[c1];                            \
	    ((mlib_u8 *)dp)[8] = t0[c2];                            \
	    ((mlib_u8 *)dp)[9] = t1[c2];                            \
	    ((mlib_u8 *)dp)[10] = t2[c2];                           \
	    ((mlib_u8 *)dp)[11] = t3[c2];                           \
	    ((mlib_u8 *)dp)[12] = t0[c3];                           \
	    ((mlib_u8 *)dp)[13] = t1[c3];                           \
	    ((mlib_u8 *)dp)[14] = t2[c3];                           \
	    ((mlib_u8 *)dp)[15] = t3[c3];                           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_U8_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;

	mlib_u8 *tab0 = (mlib_u8 *)table[0];
	mlib_u8 *tab1 = (mlib_u8 *)table[1];
	mlib_u8 *tab2 = (mlib_u8 *)table[2];
	mlib_u8 *tab3 = (mlib_u8 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_u8 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_U8_4L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1 >> 2]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 =
			    _mm_mlib_setr_pi8(tab0[c0], tab1[c0], tab2[c0],
			    tab3[c0], tab0[c1], tab1[c1], tab2[c1], tab3[c1]);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				sd1 = _mm_and_si64(sp1[1], ld4_s16[n1 >> 2]);
				dd1 =
				    _mm_mlib_setr_pi8(tab0[c2], tab1[c2],
				    tab2[c2], tab3[c2], tab0[c3], tab1[c3],
				    tab2[c3], tab3[c3]);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_S16_1L(dp, sd1, t0, t1, t2, t3)   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,        \
		sd1) ((mlib_s16 *)dp)[0] = t0[c0];                \
	    ((mlib_s16 *)dp)[1] = t1[c1];                         \
	    ((mlib_s16 *)dp)[2] = t2[c2];                         \
	    ((mlib_s16 *)dp)[3] = t3[c3];                         \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_S16_1E(dp, sd1, t0, t1, t2, t3)        \
	{                                                              \
	    mlib_s32 c0, c1, c2, c3;                                   \
	                                                               \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1) dp =   \
		_mm_mlib_setr_pi16(t0[c0], t1[c1], t2[c2], t3[c3]);    \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_s16 *tab0 = &table[0][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S16_1L(dp1, sd1, tab0, tab0,
			    tab0, tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_S16_1E(dd1, sd1, tab0, tab0,
			    tab0, tab0);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S16_1L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_S16_1E(dd1, sd1, tab0, tab1,
			    tab0, tab1);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];
	const mlib_s16 *tab2 = &table[2][32768];
	const mlib_s16 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S16_1L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_S16_1E(dd1, sd1, tab0, tab1,
			    tab2, tab3);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_S16_3L(dp, sd1, sd2, t0, t1, t2)      \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_s16 *)dp)[0] = t0[c0];                    \
	    ((mlib_s16 *)dp)[1] = t1[c1];                             \
	    ((mlib_s16 *)dp)[2] = t2[c2];                             \
	    ((mlib_s16 *)dp)[3] = t0[c3];                             \
	    ((mlib_s16 *)dp)[4] = t1[c4];                             \
	    ((mlib_s16 *)dp)[5] = t2[c5];                             \
	    ((mlib_s16 *)dp)[6] = t0[c6];                             \
	    ((mlib_s16 *)dp)[7] = t1[c7];                             \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_S16_3E(dp, sd1, t0, t1, t2)            \
	{                                                              \
	    mlib_s32 c0, c1, c2, c3;                                   \
	                                                               \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1) dp =   \
		_mm_mlib_setr_pi16(t0[c0], t1[c1], t2[c2], t0[c3]);    \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;
	const mlib_s16 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = &table[0][32768];
		tab1 = &table[1][32768];
		tab2 = &table[2][32768];

		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S16_3L(dp1, sd1, sd2, tab0,
			    tab1, tab2);
			dp1 += 2;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S16_3L(dp1, sd1, sd2, tab2,
			    tab0, tab1);
			dp1 += 2;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S16_3L(dp1, sd1, sd2, tab1,
			    tab2, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			n1 = (n1 > 3) ? 4 : n1;
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1]);
			FN_NAME(S16_S16_3E)(dd1, sd1, tab0, tab1,
			    tab2);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				n1 = (n1 > 3) ? 4 : n1;
				sd1 = _mm_and_si64(sp1[1], ld4_s16[n1]);
				FN_NAME(S16_S16_3E)(dd1, sd1, tab1,
				    tab2, tab0);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					n1 = (n1 > 3) ? 4 : n1;
					sd1 = _mm_and_si64(sp1[2], ld4_s16[n1]);
					FN_NAME(S16_S16_3E)(dd1, sd1,
					    tab2, tab0, tab1);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask =
						    (n1 >
						    3) ? 0xF : (0xF << (4 -
						    n1));
						n1 = (n1 > 3) ? 4 : n1;
						sd1 =
						    _mm_and_si64(sp1[3],
						    ld4_s16[n1]);
						FN_NAME(S16_S16_3E)
						    (dd1, sd1, tab0, tab1,
						    tab2);
						mmx_pst_16(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s16 *)dend -
						    (mlib_s16 *)dp1);
						if (n1 > 0) {
							emask =
							    (n1 >
							    3) ? 0xF : (0xF <<
							    (4 - n1));
							n1 = (n1 > 3) ? 4 : n1;
							sd1 =
							    _mm_and_si64(sp1[4],
							    ld4_s16[n1]);
							FN_NAME(S16_S16_3E)
							    (dd1, sd1, tab1,
							    tab2, tab0);
							mmx_pst_16(dd1, dp1,
							    emask);
							dp1++;

							n1 = ((mlib_s16 *)dend -
							    (mlib_s16 *)dp1);
							if (n1 > 0) {
							    emask =
								    (0xF << (4 -
								    n1));
							    sd1 =
								    _mm_and_si64
								    (sp1[5],
								    ld4_s16
								    [n1]);
							    FN_NAME(S16_S16_3E)
								    (dd1, sd1,
								    tab2, tab0,
								    tab1);
							    mmx_pst_16(dd1,
								    dp1, emask);
							}
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_S16_2L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s16 *)dp)[0] = t0[c0];              \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c1];                       \
	    ((mlib_s16 *)dp)[3] = t3[c1];                       \
	    ((mlib_s16 *)dp)[4] = t0[c2];                       \
	    ((mlib_s16 *)dp)[5] = t1[c2];                       \
	    ((mlib_s16 *)dp)[6] = t2[c3];                       \
	    ((mlib_s16 *)dp)[7] = t3[c3];                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_S16_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(ld4_s16[n1 >> 1], *sp1);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 =
			    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab0[c1],
			    tab1[c1]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				dd1 =
				    _mm_mlib_setr_pi16(tab0[c2], tab1[c2],
				    tab0[c3], tab1[c3]);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_S16_3L(dp, sd1, sd2, t0,           \
	t1, t2)                                                       \
	{                                                             \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_s16 *)dp)[0] = t0[c0];                    \
	    ((mlib_s16 *)dp)[1] = t1[c0];                             \
	    ((mlib_s16 *)dp)[2] = t2[c0];                             \
	    ((mlib_s16 *)dp)[3] = t0[c1];                             \
	    ((mlib_s16 *)dp)[4] = t1[c1];                             \
	    ((mlib_s16 *)dp)[5] = t2[c1];                             \
	    ((mlib_s16 *)dp)[6] = t0[c2];                             \
	    ((mlib_s16 *)dp)[7] = t1[c2];                             \
	    ((mlib_s16 *)dp)[8] = t2[c2];                             \
	    ((mlib_s16 *)dp)[9] = t0[c3];                             \
	    ((mlib_s16 *)dp)[10] = t1[c3];                            \
	    ((mlib_s16 *)dp)[11] = t2[c3];                            \
	    ((mlib_s16 *)dp)[12] = t0[c4];                            \
	    ((mlib_s16 *)dp)[13] = t1[c4];                            \
	    ((mlib_s16 *)dp)[14] = t2[c4];                            \
	    ((mlib_s16 *)dp)[15] = t0[c5];                            \
	    ((mlib_s16 *)dp)[16] = t1[c5];                            \
	    ((mlib_s16 *)dp)[17] = t2[c5];                            \
	    ((mlib_s16 *)dp)[18] = t0[c6];                            \
	    ((mlib_s16 *)dp)[19] = t1[c6];                            \
	    ((mlib_s16 *)dp)[20] = t2[c6];                            \
	    ((mlib_s16 *)dp)[21] = t0[c7];                            \
	    ((mlib_s16 *)dp)[22] = t1[c7];                            \
	    ((mlib_s16 *)dp)[23] = t2[c7];                            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_s16 *dend;

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];
	const mlib_s16 *tab2 = &table[2][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s16 *)dp1 + dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_S16_3L(dp1, sd1, sd2, tab0,
			    tab1, tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			n1 = (n1 > 11) ? 4 : n1 / 3;
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1)
			    dd1 =
			    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab2[c0],
			    tab0[c1]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				dd1 =
				    _mm_mlib_setr_pi16(tab1[c1], tab2[c1],
				    tab0[c2], tab1[c2]);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					dd1 =
					    _mm_mlib_setr_pi16(tab2[c2],
					    tab0[c3], tab1[c3], tab2[c3]);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask =
						    (n1 >
						    3) ? 0xF : (0xF << (4 -
						    n1));
						n1 = (n1 > 11) ? 4 : n1 / 3;
						sd1 =
						    _mm_and_si64(sp1[1],
						    ld4_s16[n1]);
						MLIB_M_IMAGE_LOOKUP_S16_2INDEX
						    (c4, c5, c6, c7, sd1)
						    dd1 =
						    _mm_mlib_setr_pi16(tab0[c4],
						    tab1[c4], tab2[c4],
						    tab0[c5]);
						mmx_pst_16(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s16 *)dend -
						    (mlib_s16 *)dp1);
						if (n1 > 0) {
						    emask =
							    (n1 >
							    3) ? 0xF : (0xF <<
							    (4 - n1));
						    dd1 =
							    _mm_mlib_setr_pi16
							    (tab1[c5], tab2[c5],
							    tab0[c6], tab1[c6]);
						    mmx_pst_16(dd1, dp1,
							    emask);
						    dp1++;

						    n1 = ((mlib_s16 *)dend -
							    (mlib_s16 *)dp1);
						    if (n1 > 0) {
							emask =
								(0xF << (4 -
								    n1));
							dd1 =
							    _mm_mlib_setr_pi16
								    (tab2[c6],
								    tab0[c7],
								    tab1[c7],
								    tab2[c7]);
							    mmx_pst_16(dd1,
								dp1, emask);
						    }
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_S16_4L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s16 *)dp)[0] = t0[c0];              \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c0];                       \
	    ((mlib_s16 *)dp)[3] = t3[c0];                       \
	    ((mlib_s16 *)dp)[4] = t0[c1];                       \
	    ((mlib_s16 *)dp)[5] = t1[c1];                       \
	    ((mlib_s16 *)dp)[6] = t2[c1];                       \
	    ((mlib_s16 *)dp)[7] = t3[c1];                       \
	    ((mlib_s16 *)dp)[8] = t0[c2];                       \
	    ((mlib_s16 *)dp)[9] = t1[c2];                       \
	    ((mlib_s16 *)dp)[10] = t2[c2];                      \
	    ((mlib_s16 *)dp)[11] = t3[c2];                      \
	    ((mlib_s16 *)dp)[12] = t0[c3];                      \
	    ((mlib_s16 *)dp)[13] = t1[c3];                      \
	    ((mlib_s16 *)dp)[14] = t2[c3];                      \
	    ((mlib_s16 *)dp)[15] = t3[c3];                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	const mlib_s16 *tab0 = &table[0][32768];
	const mlib_s16 *tab1 = &table[1][32768];
	const mlib_s16 *tab2 = &table[2][32768];
	const mlib_s16 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_S16_4L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 >> 2)]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 =
			    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab2[c0],
			    tab3[c0]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				dd1 =
				    _mm_mlib_setr_pi16(tab0[c1], tab1[c1],
				    tab2[c1], tab3[c1]);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					dd1 =
					    _mm_mlib_setr_pi16(tab0[c2],
					    tab1[c2], tab2[c2], tab3[c2]);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask = (0xF << (4 - n1));
						dd1 =
						    _mm_mlib_setr_pi16(tab0[c3],
						    tab1[c3], tab2[c3],
						    tab3[c3]);
						mmx_pst_16(dd1, dp1, emask);
					}
				}
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_S16_1L(dp, sd1, t0, t1, t2, t3)   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,        \
		sd1) ((mlib_s16 *)dp)[0] = t0[c0];                \
	    ((mlib_s16 *)dp)[1] = t1[c1];                         \
	    ((mlib_s16 *)dp)[2] = t2[c2];                         \
	    ((mlib_s16 *)dp)[3] = t3[c3];                         \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_S16_1E(dp, sd1, t0, t1, t2, t3)        \
	{                                                              \
	    mlib_s32 c0, c1, c2, c3;                                   \
	                                                               \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1) dp =   \
		_mm_mlib_setr_pi16(t0[c0], t1[c1], t2[c2], t3[c3]);    \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S16_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S16_1L(dp1, sd1, tab0, tab0,
			    tab0, tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_S16_1E(dd1, sd1, tab0, tab0,
			    tab0, tab0);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S16_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S16_1L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_S16_1E(dd1, sd1, tab0, tab1,
			    tab0, tab1);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S16_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];
	mlib_s16 *tab3 = (mlib_s16 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S16_1L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_S16_1E(dd1, sd1, tab0, tab1,
			    tab2, tab3);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_S16_3L(dp, sd1, sd2, t0, t1, t2)      \
	{                                                             \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                  \
	                                                              \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_s16 *)dp)[0] = t0[c0];                    \
	    ((mlib_s16 *)dp)[1] = t1[c1];                             \
	    ((mlib_s16 *)dp)[2] = t2[c2];                             \
	    ((mlib_s16 *)dp)[3] = t0[c3];                             \
	    ((mlib_s16 *)dp)[4] = t1[c4];                             \
	    ((mlib_s16 *)dp)[5] = t2[c5];                             \
	    ((mlib_s16 *)dp)[6] = t0[c6];                             \
	    ((mlib_s16 *)dp)[7] = t1[c7];                             \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_S16_3E(dp, sd1, t0, t1, t2)            \
	{                                                              \
	    mlib_s32 c0, c1, c2, c3;                                   \
	                                                               \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1) dp =   \
		_mm_mlib_setr_pi16(t0[c0], t1[c1], t2[c2], t0[c3]);    \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S16_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;
	mlib_s16 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = (mlib_s16 *)table[0];
		tab1 = (mlib_s16 *)table[1];
		tab2 = (mlib_s16 *)table[2];

		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S16_3L(dp1, sd1, sd2, tab0,
			    tab1, tab2);
			dp1 += 2;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S16_3L(dp1, sd1, sd2, tab2,
			    tab0, tab1);
			dp1 += 2;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S16_3L(dp1, sd1, sd2, tab1,
			    tab2, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			n1 = (n1 > 3) ? 4 : n1;
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_S16_3E(dd1, sd1, tab0, tab1,
			    tab2);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				n1 = (n1 > 3) ? 4 : n1;
				sd1 = _mm_and_si64(sp1[1], ld4_s16[n1]);
				MLIB_M_IMAGE_LOOKUP_U16_S16_3E(dd1, sd1, tab1,
				    tab2, tab0);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					n1 = (n1 > 3) ? 4 : n1;
					sd1 = _mm_and_si64(sp1[2], ld4_s16[n1]);
					MLIB_M_IMAGE_LOOKUP_U16_S16_3E(dd1, sd1,
					    tab2, tab0, tab1);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask =
						    (n1 >
						    3) ? 0xF : (0xF << (4 -
						    n1));
						n1 = (n1 > 3) ? 4 : n1;
						sd1 =
						    _mm_and_si64(sp1[3],
						    ld4_s16[n1]);
						MLIB_M_IMAGE_LOOKUP_U16_S16_3E
						    (dd1, sd1, tab0, tab1,
						    tab2);
						mmx_pst_16(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s16 *)dend -
						    (mlib_s16 *)dp1);
						if (n1 > 0) {
							emask =
							    (n1 >
							    3) ? 0xF : (0xF <<
							    (4 - n1));
							n1 = (n1 > 3) ? 4 : n1;
							sd1 =
							    _mm_and_si64(sp1[4],
							    ld4_s16[n1]);
							FN_NAME(U16_S16_3E)
							    (dd1, sd1, tab1,
							    tab2, tab0);
							mmx_pst_16(dd1, dp1,
							    emask);
							dp1++;

							n1 = ((mlib_s16 *)dend -
							    (mlib_s16 *)dp1);
							if (n1 > 0) {
							    emask =
								    (0xF << (4 -
								    n1));
							    sd1 =
								    _mm_and_si64
								    (sp1[5],
								    ld4_s16
								    [n1]);
							    FN_NAME(U16_S16_3E)
								    (dd1, sd1,
								    tab2, tab0,
								    tab1);
							    mmx_pst_16(dd1,
								    dp1, emask);
							}
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_S16_2L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s16 *)dp)[0] = t0[c0];              \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c1];                       \
	    ((mlib_s16 *)dp)[3] = t3[c1];                       \
	    ((mlib_s16 *)dp)[4] = t0[c2];                       \
	    ((mlib_s16 *)dp)[5] = t1[c2];                       \
	    ((mlib_s16 *)dp)[6] = t2[c3];                       \
	    ((mlib_s16 *)dp)[7] = t3[c3];                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_S16_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_S16_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(ld4_s16[n1 >> 1], *sp1);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 =
			    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab0[c1],
			    tab1[c1]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				dd1 =
				    _mm_mlib_setr_pi16(tab0[c2], tab1[c2],
				    tab0[c3], tab1[c3]);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_S16_3L(dp, sd1, sd2, t0,           \
	t1, t2)                                                       \
	{                                                             \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,            \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,   \
		sd2) ((mlib_s16 *)dp)[0] = t0[c0];                    \
	    ((mlib_s16 *)dp)[1] = t1[c0];                             \
	    ((mlib_s16 *)dp)[2] = t2[c0];                             \
	    ((mlib_s16 *)dp)[3] = t0[c1];                             \
	    ((mlib_s16 *)dp)[4] = t1[c1];                             \
	    ((mlib_s16 *)dp)[5] = t2[c1];                             \
	    ((mlib_s16 *)dp)[6] = t0[c2];                             \
	    ((mlib_s16 *)dp)[7] = t1[c2];                             \
	    ((mlib_s16 *)dp)[8] = t2[c2];                             \
	    ((mlib_s16 *)dp)[9] = t0[c3];                             \
	    ((mlib_s16 *)dp)[10] = t1[c3];                            \
	    ((mlib_s16 *)dp)[11] = t2[c3];                            \
	    ((mlib_s16 *)dp)[12] = t0[c4];                            \
	    ((mlib_s16 *)dp)[13] = t1[c4];                            \
	    ((mlib_s16 *)dp)[14] = t2[c4];                            \
	    ((mlib_s16 *)dp)[15] = t0[c5];                            \
	    ((mlib_s16 *)dp)[16] = t1[c5];                            \
	    ((mlib_s16 *)dp)[17] = t2[c5];                            \
	    ((mlib_s16 *)dp)[18] = t0[c6];                            \
	    ((mlib_s16 *)dp)[19] = t1[c6];                            \
	    ((mlib_s16 *)dp)[20] = t2[c6];                            \
	    ((mlib_s16 *)dp)[21] = t0[c7];                            \
	    ((mlib_s16 *)dp)[22] = t1[c7];                            \
	    ((mlib_s16 *)dp)[23] = t2[c7];                            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_S16_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_s16 *dend;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s16 *)dp1 + dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_S16_3L(dp1, sd1, sd2, tab0,
			    tab1, tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			n1 = (n1 > 11) ? 4 : n1 / 3;
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1)
			    dd1 =
			    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab2[c0],
			    tab0[c1]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				dd1 =
				    _mm_mlib_setr_pi16(tab1[c1], tab2[c1],
				    tab0[c2], tab1[c2]);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					dd1 =
					    _mm_mlib_setr_pi16(tab2[c2],
					    tab0[c3], tab1[c3], tab2[c3]);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask =
						    (n1 >
						    3) ? 0xF : (0xF << (4 -
						    n1));
						n1 = (n1 > 11) ? 4 : n1 / 3;
						sd1 =
						    _mm_and_si64(sp1[1],
						    ld4_s16[n1]);
						MLIB_M_IMAGE_LOOKUP_U16_2INDEX
						    (c4, c5, c6, c7, sd1)
						    dd1 =
						    _mm_mlib_setr_pi16(tab0[c4],
						    tab1[c4], tab2[c4],
						    tab0[c5]);
						mmx_pst_16(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s16 *)dend -
						    (mlib_s16 *)dp1);
						if (n1 > 0) {
						    emask =
							    (n1 >
							    3) ? 0xF : (0xF <<
							    (4 - n1));
						    dd1 =
							    _mm_mlib_setr_pi16
							    (tab1[c5], tab2[c5],
							    tab0[c6], tab1[c6]);
						    mmx_pst_16(dd1, dp1,
							    emask);
						    dp1++;

						    n1 = ((mlib_s16 *)dend -
							    (mlib_s16 *)dp1);
						    if (n1 > 0) {
							emask =
								(0xF << (4 -
									n1));
							dd1 =
							    _mm_mlib_setr_pi16
								    (tab2[c6],
								    tab0[c7],
								    tab1[c7],
								    tab2[c7]);
							    mmx_pst_16(dd1,
								    dp1, emask);
						    }
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_S16_4L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s16 *)dp)[0] = t0[c0];              \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c0];                       \
	    ((mlib_s16 *)dp)[3] = t3[c0];                       \
	    ((mlib_s16 *)dp)[4] = t0[c1];                       \
	    ((mlib_s16 *)dp)[5] = t1[c1];                       \
	    ((mlib_s16 *)dp)[6] = t2[c1];                       \
	    ((mlib_s16 *)dp)[7] = t3[c1];                       \
	    ((mlib_s16 *)dp)[8] = t0[c2];                       \
	    ((mlib_s16 *)dp)[9] = t1[c2];                       \
	    ((mlib_s16 *)dp)[10] = t2[c2];                      \
	    ((mlib_s16 *)dp)[11] = t3[c2];                      \
	    ((mlib_s16 *)dp)[12] = t0[c3];                      \
	    ((mlib_s16 *)dp)[13] = t1[c3];                      \
	    ((mlib_s16 *)dp)[14] = t2[c3];                      \
	    ((mlib_s16 *)dp)[15] = t3[c3];                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_S16_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];
	mlib_s16 *tab3 = (mlib_s16 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_S16_4L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 >> 2)]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 =
			    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab2[c0],
			    tab3[c0]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				dd1 =
				    _mm_mlib_setr_pi16(tab0[c1], tab1[c1],
				    tab2[c1], tab3[c1]);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					dd1 =
					    _mm_mlib_setr_pi16(tab0[c2],
					    tab1[c2], tab2[c2], tab3[c2]);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask = (0xF << (4 - n1));
						dd1 =
						    _mm_mlib_setr_pi16(tab0[c3],
						    tab1[c3], tab2[c3],
						    tab3[c3]);
						mmx_pst_16(dd1, dp1, emask);
					}
				}
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_S32_1L(dp, sd1, t0, t1, t2, t3)   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,        \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];                \
	    ((mlib_s32 *)dp)[1] = t1[c1];                         \
	    ((mlib_s32 *)dp)[2] = t2[c2];                         \
	    ((mlib_s32 *)dp)[3] = t3[c3];                         \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S32_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;

	const mlib_s32 *tab0 = &table[0][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S32_1L(dp1, sd1, tab0, tab0,
			    tab0, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 = _mm_mlib_setr_pi32(tab0[c0], tab0[c1]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				dd1 = _mm_mlib_setr_pi32(tab0[c2], tab0[c3]);
				mmx_pst_32(dd1, dp1, 2);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S32_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S32_1L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 = _mm_mlib_setr_pi32(tab0[c0], tab1[c1]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				dd1 = _mm_mlib_setr_pi32(tab0[c2], tab1[c3]);
				mmx_pst_32(dd1, dp1, 2);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S32_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 emask;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];
	const mlib_s32 *tab2 = &table[2][32768];
	const mlib_s32 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S32_1L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dp1[0] = _mm_mlib_setr_pi32(tab0[c0], tab1[c1]);
			dp1[1] = _mm_mlib_setr_pi32(tab2[c2], tab3[c3]);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S16_S32_3L(dp, sd1, sd2, sd3, t0,            \
	t1, t2)                                                          \
	{                                                                \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;   \
	                                                                 \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,               \
		sd1) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c4, c5, c6, c7,      \
		sd2) MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c8, c9, c10, c11,    \
		sd3) ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c1];                                \
	    ((mlib_s32 *)dp)[2] = t2[c2];                                \
	    ((mlib_s32 *)dp)[3] = t0[c3];                                \
	    ((mlib_s32 *)dp)[4] = t1[c4];                                \
	    ((mlib_s32 *)dp)[5] = t2[c5];                                \
	    ((mlib_s32 *)dp)[6] = t0[c6];                                \
	    ((mlib_s32 *)dp)[7] = t1[c7];                                \
	    ((mlib_s32 *)dp)[8] = t2[c8];                                \
	    ((mlib_s32 *)dp)[9] = t0[c9];                                \
	    ((mlib_s32 *)dp)[10] = t1[c10];                              \
	    ((mlib_s32 *)dp)[11] = t2[c11];                              \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S16_S32_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2, sd3;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;
	const mlib_s32 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = &table[0][32768];
		tab1 = &table[1][32768];
		tab2 = &table[2][32768];

		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 11; n1 -= 12) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_S16_S32_3L(dp1, sd1, sd2, sd3, tab0,
			    tab1, tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			n1 = (n1 > 3) ? 4 : n1;
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 = _mm_mlib_setr_pi32(tab0[c0], tab1[c1]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				dd1 = _mm_mlib_setr_pi32(tab2[c2], tab0[c3]);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					emask = (n1 > 1) ? 3 : 2;
					n1 = (n1 > 3) ? 4 : n1;
					sd1 = _mm_and_si64(sp1[1], ld4_s16[n1]);
					MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1,
					    c2, c3, sd1);
					dd1 =
					    _mm_mlib_setr_pi32(tab1[c0],
					    tab2[c1]);
					mmx_pst_32(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						emask = (n1 > 1) ? 3 : 2;
						dd1 =
						    _mm_mlib_setr_pi32(tab0[c2],
						    tab1[c3]);
						mmx_pst_32(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s32 *)dend -
						    (mlib_s32 *)dp1);
						if (n1 > 0) {
						    emask =
							    (n1 > 1) ? 3 : 2;
						    n1 = (n1 > 3) ? 4 : n1;
						    sd1 =
							    _mm_and_si64(sp1[2],
							    ld4_s16[n1]);
						    FN_NAME(S16_2INDEX)
							    (c0, c1, c2, c3,
							    sd1);
						    dd1 =
							    _mm_mlib_setr_pi32
							    (tab2[c0],
							    tab0[c1]);
						    mmx_pst_32(dd1, dp1,
							    emask);
						    dp1++;

						    n1 = ((mlib_s32 *)dend -
							    (mlib_s32 *)dp1);
						    if (n1 > 0) {
							emask =
								(n1 >
								1) ? 3 : 2;
							dd1 =
							    _mm_mlib_setr_pi32
								    (tab1[c2],
								    tab2[c3]);
							mmx_pst_32(dd1,
							    dp1, emask);
							dp1++;
						    }
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_S32_2L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];              \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c1];                       \
	    ((mlib_s32 *)dp)[3] = t3[c1];                       \
	    ((mlib_s32 *)dp)[4] = t0[c2];                       \
	    ((mlib_s32 *)dp)[5] = t1[c2];                       \
	    ((mlib_s32 *)dp)[6] = t2[c3];                       \
	    ((mlib_s32 *)dp)[7] = t3[c3];                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_S32_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 *dend;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_S32_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 >> 1)]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			(*dp1++) = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				(*dp1++) =
				    _mm_mlib_setr_pi32(tab0[c1], tab1[c1]);

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					(*dp1++) =
					    _mm_mlib_setr_pi32(tab0[c2],
					    tab1[c2]);

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						*dp1 =
						    _mm_mlib_setr_pi32(tab0[c3],
						    tab1[c3]);
					}
				}
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_S32_3L(dp, sd1, t0, t1, t2)   \
	{                                                        \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,       \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];               \
	    ((mlib_s32 *)dp)[1] = t1[c0];                        \
	    ((mlib_s32 *)dp)[2] = t2[c0];                        \
	    ((mlib_s32 *)dp)[3] = t0[c1];                        \
	    ((mlib_s32 *)dp)[4] = t1[c1];                        \
	    ((mlib_s32 *)dp)[5] = t2[c1];                        \
	    ((mlib_s32 *)dp)[6] = t0[c2];                        \
	    ((mlib_s32 *)dp)[7] = t1[c2];                        \
	    ((mlib_s32 *)dp)[8] = t2[c2];                        \
	    ((mlib_s32 *)dp)[9] = t0[c3];                        \
	    ((mlib_s32 *)dp)[10] = t1[c3];                       \
	    ((mlib_s32 *)dp)[11] = t2[c3];                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_S32_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3;
	mlib_s32 *dend;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];
	const mlib_s32 *tab2 = &table[2][32768];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s32 *)dp1 + dw;

		for (; n1 > 11; n1 -= 12) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_S32_3L(dp1, sd1, tab0, tab1,
			    tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 / 3)]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1)
			    dd1 = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				dd1 = _mm_mlib_setr_pi32(tab2[c0], tab0[c1]);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					emask = (n1 > 1) ? 3 : 2;
					dd1 =
					    _mm_mlib_setr_pi32(tab1[c1],
					    tab2[c1]);
					mmx_pst_32(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						emask = (n1 > 1) ? 3 : 2;
						dd1 =
						    _mm_mlib_setr_pi32(tab0[c2],
						    tab1[c2]);
						mmx_pst_32(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s32 *)dend -
						    (mlib_s32 *)dp1);
						if (n1 > 0) {
						    emask =
							    (n1 > 1) ? 3 : 2;
						    dd1 =
							    _mm_mlib_setr_pi32
							    (tab2[c2],
							    tab0[c3]);
						    mmx_pst_32(dd1, dp1,
							    emask);
						    dp1++;

						    n1 = ((mlib_s32 *)dend -
							    (mlib_s32 *)dp1);
						    if (n1 > 0) {
							emask =
							    (n1 >
							    1) ? 3 : 2;
							dd1 =
							    _mm_mlib_setr_pi32
								    (tab1[c3],
								    tab2[c3]);
							mmx_pst_32(dd1,
							    dp1, emask);
						    }
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S16_S32_4L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];              \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c0];                       \
	    ((mlib_s32 *)dp)[3] = t3[c0];                       \
	    ((mlib_s32 *)dp)[4] = t0[c1];                       \
	    ((mlib_s32 *)dp)[5] = t1[c1];                       \
	    ((mlib_s32 *)dp)[6] = t2[c1];                       \
	    ((mlib_s32 *)dp)[7] = t3[c1];                       \
	    ((mlib_s32 *)dp)[8] = t0[c2];                       \
	    ((mlib_s32 *)dp)[9] = t1[c2];                       \
	    ((mlib_s32 *)dp)[10] = t2[c2];                      \
	    ((mlib_s32 *)dp)[11] = t3[c2];                      \
	    ((mlib_s32 *)dp)[12] = t0[c3];                      \
	    ((mlib_s32 *)dp)[13] = t1[c3];                      \
	    ((mlib_s32 *)dp)[14] = t2[c3];                      \
	    ((mlib_s32 *)dp)[15] = t3[c3];                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S16_S32_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 *dend;

	const mlib_s32 *tab0 = &table[0][32768];
	const mlib_s32 *tab1 = &table[1][32768];
	const mlib_s32 *tab2 = &table[2][32768];
	const mlib_s32 *tab3 = &table[3][32768];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S16_S32_4L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 8;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 >> 2)]);
			MLIB_M_IMAGE_LOOKUP_S16_2INDEX(c0, c1, c2, c3, sd1);
			(*dp1++) = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);
			(*dp1++) = _mm_mlib_setr_pi32(tab2[c0], tab3[c0]);

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				(*dp1++) =
				    _mm_mlib_setr_pi32(tab0[c1], tab1[c1]);
				(*dp1++) =
				    _mm_mlib_setr_pi32(tab2[c1], tab3[c1]);

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					(*dp1++) =
					    _mm_mlib_setr_pi32(tab0[c2],
					    tab1[c2]);
					(*dp1++) =
					    _mm_mlib_setr_pi32(tab2[c2],
					    tab3[c2]);

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						dp1[0] =
						    _mm_mlib_setr_pi32(tab0[c3],
						    tab1[c3]);
						dp1[1] =
						    _mm_mlib_setr_pi32(tab2[c3],
						    tab3[c3]);
					}
				}
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_S32_1L(dp, sd1, t0, t1, t2, t3)   \
	{                                                         \
	    mlib_s32 c0, c1, c2, c3;                              \
	                                                          \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,        \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];                \
	    ((mlib_s32 *)dp)[1] = t1[c1];                         \
	    ((mlib_s32 *)dp)[2] = t2[c2];                         \
	    ((mlib_s32 *)dp)[3] = t3[c3];                         \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S32_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S32_1L(dp1, sd1, tab0, tab0,
			    tab0, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 = _mm_mlib_setr_pi32(tab0[c0], tab0[c1]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				dd1 = _mm_mlib_setr_pi32(tab0[c2], tab0[c3]);
				mmx_pst_32(dd1, dp1, 2);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S32_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S32_1L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 = _mm_mlib_setr_pi32(tab0[c0], tab1[c1]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				dd1 = _mm_mlib_setr_pi32(tab0[c2], tab1[c3]);
				mmx_pst_32(dd1, dp1, 2);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S32_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];
	mlib_s32 *tab3 = (mlib_s32 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S32_1L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dp1[0] = _mm_mlib_setr_pi32(tab0[c0], tab1[c1]);
			dp1[1] = _mm_mlib_setr_pi32(tab2[c2], tab3[c3]);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U16_S32_3L(dp, sd1, sd2, sd3, t0,            \
	t1, t2)                                                          \
	{                                                                \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;   \
	                                                                 \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,               \
		sd1) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c4, c5, c6, c7,      \
		sd2) MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c8, c9, c10, c11,    \
		sd3) ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c1];                                \
	    ((mlib_s32 *)dp)[2] = t2[c2];                                \
	    ((mlib_s32 *)dp)[3] = t0[c3];                                \
	    ((mlib_s32 *)dp)[4] = t1[c4];                                \
	    ((mlib_s32 *)dp)[5] = t2[c5];                                \
	    ((mlib_s32 *)dp)[6] = t0[c6];                                \
	    ((mlib_s32 *)dp)[7] = t1[c7];                                \
	    ((mlib_s32 *)dp)[8] = t2[c8];                                \
	    ((mlib_s32 *)dp)[9] = t0[c9];                                \
	    ((mlib_s32 *)dp)[10] = t1[c10];                              \
	    ((mlib_s32 *)dp)[11] = t2[c11];                              \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U16_S32_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1, sd2, sd3;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;
	mlib_s32 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = (mlib_s32 *)table[0];
		tab1 = (mlib_s32 *)table[1];
		tab2 = (mlib_s32 *)table[2];

		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 11; n1 -= 12) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_U16_S32_3L(dp1, sd1, sd2, sd3, tab0,
			    tab1, tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			emask = (n1 > 1) ? 3 : 2;
			n1 = (n1 > 3) ? 4 : n1;
			sd1 = _mm_and_si64(sp1[0], ld4_s16[n1]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			dd1 = _mm_mlib_setr_pi32(tab0[c0], tab1[c1]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				dd1 = _mm_mlib_setr_pi32(tab2[c2], tab0[c3]);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					emask = (n1 > 1) ? 3 : 2;
					n1 = (n1 > 3) ? 4 : n1;
					sd1 = _mm_and_si64(sp1[1], ld4_s16[n1]);
					MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1,
					    c2, c3, sd1);
					dd1 =
					    _mm_mlib_setr_pi32(tab1[c0],
					    tab2[c1]);
					mmx_pst_32(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						emask = (n1 > 1) ? 3 : 2;
						dd1 =
						    _mm_mlib_setr_pi32(tab0[c2],
						    tab1[c3]);
						mmx_pst_32(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s32 *)dend -
						    (mlib_s32 *)dp1);
						if (n1 > 0) {
						    emask =
							    (n1 > 1) ? 3 : 2;
						    n1 = (n1 > 3) ? 4 : n1;
						    sd1 =
							    _mm_and_si64(sp1[2],
							    ld4_s16[n1]);
						    FN_NAME(U16_2INDEX)
							    (c0, c1, c2, c3,
							    sd1);
						    dd1 =
							    _mm_mlib_setr_pi32
							    (tab2[c0],
							    tab0[c1]);
						    mmx_pst_32(dd1, dp1,
							    emask);
						    dp1++;

						    n1 = ((mlib_s32 *)dend -
							    (mlib_s32 *)dp1);
						    if (n1 > 0) {
							emask =
							    (n1 >
							    1) ? 3 : 2;
							dd1 =
							    _mm_mlib_setr_pi32
								    (tab1[c2],
								    tab2[c3]);
							mmx_pst_32(dd1,
							    dp1, emask);
							dp1++;
						    }
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_S32_2L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];              \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c1];                       \
	    ((mlib_s32 *)dp)[3] = t3[c1];                       \
	    ((mlib_s32 *)dp)[4] = t0[c2];                       \
	    ((mlib_s32 *)dp)[5] = t1[c2];                       \
	    ((mlib_s32 *)dp)[6] = t2[c3];                       \
	    ((mlib_s32 *)dp)[7] = t3[c3];                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_S32_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 *dend;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_S32_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 >> 1)]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			(*dp1++) = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				(*dp1++) =
				    _mm_mlib_setr_pi32(tab0[c1], tab1[c1]);

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					(*dp1++) =
					    _mm_mlib_setr_pi32(tab0[c2],
					    tab1[c2]);

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						*dp1 =
						    _mm_mlib_setr_pi32(tab0[c3],
						    tab1[c3]);
					}
				}
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_S32_3L(dp, sd1, t0, t1, t2)   \
	{                                                        \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,       \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];               \
	    ((mlib_s32 *)dp)[1] = t1[c0];                        \
	    ((mlib_s32 *)dp)[2] = t2[c0];                        \
	    ((mlib_s32 *)dp)[3] = t0[c1];                        \
	    ((mlib_s32 *)dp)[4] = t1[c1];                        \
	    ((mlib_s32 *)dp)[5] = t2[c1];                        \
	    ((mlib_s32 *)dp)[6] = t0[c2];                        \
	    ((mlib_s32 *)dp)[7] = t1[c2];                        \
	    ((mlib_s32 *)dp)[8] = t2[c2];                        \
	    ((mlib_s32 *)dp)[9] = t0[c3];                        \
	    ((mlib_s32 *)dp)[10] = t1[c3];                       \
	    ((mlib_s32 *)dp)[11] = t2[c3];                       \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_S32_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3;
	mlib_s32 *dend;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s32 *)dp1 + dw;

		for (; n1 > 11; n1 -= 12) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_S32_3L(dp1, sd1, tab0, tab1,
			    tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 / 3)]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1)
			    dd1 = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				dd1 = _mm_mlib_setr_pi32(tab2[c0], tab0[c1]);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					emask = (n1 > 1) ? 3 : 2;
					dd1 =
					    _mm_mlib_setr_pi32(tab1[c1],
					    tab2[c1]);
					mmx_pst_32(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						emask = (n1 > 1) ? 3 : 2;
						dd1 =
						    _mm_mlib_setr_pi32(tab0[c2],
						    tab1[c2]);
						mmx_pst_32(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s32 *)dend -
						    (mlib_s32 *)dp1);
						if (n1 > 0) {
						    emask =
							    (n1 > 1) ? 3 : 2;
						    dd1 =
							    _mm_mlib_setr_pi32
							    (tab2[c2],
							    tab0[c3]);
						    mmx_pst_32(dd1, dp1,
							    emask);
						    dp1++;

						    n1 = ((mlib_s32 *)dend -
							    (mlib_s32 *)dp1);
						    if (n1 > 0) {
							emask =
							    (n1 >
							    1) ? 3 : 2;
							dd1 =
							    _mm_mlib_setr_pi32
								    (tab1[c3],
								    tab2[c3]);
							mmx_pst_32(dd1,
							    dp1, emask);
						    }
						}
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U16_S32_4L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3,      \
		sd1) ((mlib_s32 *)dp)[0] = t0[c0];              \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c0];                       \
	    ((mlib_s32 *)dp)[3] = t3[c0];                       \
	    ((mlib_s32 *)dp)[4] = t0[c1];                       \
	    ((mlib_s32 *)dp)[5] = t1[c1];                       \
	    ((mlib_s32 *)dp)[6] = t2[c1];                       \
	    ((mlib_s32 *)dp)[7] = t3[c1];                       \
	    ((mlib_s32 *)dp)[8] = t0[c2];                       \
	    ((mlib_s32 *)dp)[9] = t1[c2];                       \
	    ((mlib_s32 *)dp)[10] = t2[c2];                      \
	    ((mlib_s32 *)dp)[11] = t3[c2];                      \
	    ((mlib_s32 *)dp)[12] = t0[c3];                      \
	    ((mlib_s32 *)dp)[13] = t1[c3];                      \
	    ((mlib_s32 *)dp)[14] = t2[c3];                      \
	    ((mlib_s32 *)dp)[15] = t3[c3];                      \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U16_S32_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* dst address */
	__m64 *dp1, *dl1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 *dend;

	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];
	mlib_s32 *tab3 = (mlib_s32 *)table[3];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_U16_S32_4L(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 8;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3;

			sd1 = _mm_and_si64(*sp1, ld4_s16[(n1 >> 2)]);
			MLIB_M_IMAGE_LOOKUP_U16_2INDEX(c0, c1, c2, c3, sd1);
			(*dp1++) = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);
			(*dp1++) = _mm_mlib_setr_pi32(tab2[c0], tab3[c0]);

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				(*dp1++) =
				    _mm_mlib_setr_pi32(tab0[c1], tab1[c1]);
				(*dp1++) =
				    _mm_mlib_setr_pi32(tab2[c1], tab3[c1]);

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					(*dp1++) =
					    _mm_mlib_setr_pi32(tab0[c2],
					    tab1[c2]);
					(*dp1++) =
					    _mm_mlib_setr_pi32(tab2[c2],
					    tab3[c2]);

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						dp1[0] =
						    _mm_mlib_setr_pi32(tab0[c3],
						    tab1[c3]);
						dp1[1] =
						    _mm_mlib_setr_pi32(tab2[c3],
						    tab3[c3]);
					}
				}
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */
