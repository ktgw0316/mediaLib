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

#pragma ident	"@(#)mlib_m_ImageLookUp_8.c	9.4	07/11/05 SMI"

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
#pragma align 8(ld4_u8)
#endif /* defined(__ICL) || defined(__INTEL_COMPILER) */
	__m64 ld8_u8[] = {
		0x0000000000000000, 0x00000000000000ff,
		0x000000000000ffff, 0x0000000000ffffff,
		0x00000000ffffffff, 0x000000ffffffffff,
		0x0000ffffffffffff, 0x00ffffffffffffff,
		0xffffffffffffffff
	};

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5,    \
	c6, c7, sd1)                                             \
	{                                                        \
	    __m64 s0 = _mm_unpacklo_pi8(sd1, _m_zero);           \
	    __m64 s1 = _mm_unpackhi_pi8(sd1, _m_zero);           \
	    __m64 a2 = _mm_unpacklo_pi16(s0, _m_zero);           \
	    __m64 a3 = _mm_unpackhi_pi16(s0, _m_zero);           \
	    __m64 a4 = _mm_unpacklo_pi16(s1, _m_zero);           \
	    __m64 a5 = _mm_unpackhi_pi16(s1, _m_zero);           \
	    __m64 b2 = _mm_srli_si64(a2, 32);                    \
	    __m64 b3 = _mm_srli_si64(a3, 32);                    \
	    __m64 b4 = _mm_srli_si64(a4, 32);                    \
	    __m64 b5 = _mm_srli_si64(a5, 32);                    \
	                                                         \
	    c0 = _mm_cvtsi64_si32(a2);                           \
	    c1 = _mm_cvtsi64_si32(b2);                           \
	    c2 = _mm_cvtsi64_si32(a3);                           \
	    c3 = _mm_cvtsi64_si32(b3);                           \
	    c4 = _mm_cvtsi64_si32(a4);                           \
	    c5 = _mm_cvtsi64_si32(b4);                           \
	    c6 = _mm_cvtsi64_si32(a5);                           \
	    c7 = _mm_cvtsi64_si32(b5);                           \
	}

/* *********************************************************** */

#define	FN_NAME(F)	MLIB_M_IMAGE_LOOKUP_##F

#define	MLIB_M_IMAGE_LOOKUP_U8_U8_1L(dp, sd1, t0, t1, t2, t3)           \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_u8 *)dp)[0] = t0[c0];                   \
	    ((mlib_u8 *)dp)[1] = t1[c1];                                \
	    ((mlib_u8 *)dp)[2] = t2[c2];                                \
	    ((mlib_u8 *)dp)[3] = t3[c3];                                \
	    ((mlib_u8 *)dp)[4] = t0[c4];                                \
	    ((mlib_u8 *)dp)[5] = t1[c5];                                \
	    ((mlib_u8 *)dp)[6] = t2[c6];                                \
	    ((mlib_u8 *)dp)[7] = t3[c7];                                \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_U8_1E(dp, sd1, t0, t1, t2, t3)           \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) dp =                                           \
		_mm_mlib_setr_pi8(t0[c0], t1[c1], t2[c2], t3[c3],       \
		t0[c4], t1[c5], t2[c6], t3[c7]);                        \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_U8_1(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

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
			FN_NAME(U8_U8_1L)(dp1, sd1, tab0, tab0, tab0,
			    tab0);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_U8_1E)(dd1, sd1, tab0, tab0, tab0,
			    tab0);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_U8_2(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *tab0, *tab1;

	tab0 = (mlib_u8 *)table[0];
	tab1 = (mlib_u8 *)table[1];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(U8_U8_1L)(dp1, sd1, tab0, tab1, tab0,
			    tab1);
			dp1++;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_U8_1E)(dd1, sd1, tab0, tab1, tab0,
			    tab1);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_U8_4(
    const mlib_u8 *src,
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
	mlib_s32 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_u8 *tab0, *tab1, *tab2, *tab3;
	mlib_u8 *dend;

	tab0 = (mlib_u8 *)table[0];
	tab1 = (mlib_u8 *)table[1];
	tab2 = (mlib_u8 *)table[2];
	tab3 = (mlib_u8 *)table[3];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {
		dend = (mlib_u8 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1)
			FN_NAME(U8_2INDEX)(d0, d1, d2, d3, d4,
			    d5, d6, d7, sd2)
			((mlib_u8 *)dp1)[0] = tab0[c0];
			((mlib_u8 *)dp1)[1] = tab1[c1];
			((mlib_u8 *)dp1)[2] = tab2[c2];
			((mlib_u8 *)dp1)[3] = tab3[c3];
			((mlib_u8 *)dp1)[4] = tab0[c4];
			((mlib_u8 *)dp1)[5] = tab1[c5];
			((mlib_u8 *)dp1)[6] = tab2[c6];
			((mlib_u8 *)dp1)[7] = tab3[c7];
			((mlib_u8 *)dp1)[8] = tab0[d0];
			((mlib_u8 *)dp1)[9] = tab1[d1];
			((mlib_u8 *)dp1)[10] = tab2[d2];
			((mlib_u8 *)dp1)[11] = tab3[d3];
			((mlib_u8 *)dp1)[12] = tab0[d4];
			((mlib_u8 *)dp1)[13] = tab1[d5];
			((mlib_u8 *)dp1)[14] = tab2[d6];
			((mlib_u8 *)dp1)[15] = tab3[d7];
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			n1 = (n1 > 7) ? 8 : n1;
			sd1 = _mm_and_si64(sp1[0], ld8_u8[n1]);
			FN_NAME(U8_U8_1E)(dd1, sd1, tab0, tab1, tab2,
			    tab3);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				sd1 = _mm_and_si64(sp1[1], ld8_u8[n1]);
				FN_NAME(U8_U8_1E)(dd1, sd1, tab0,
				    tab1, tab2, tab3);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_U8_3L(dp, sd1, t0, t1, t2)               \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_u8 *)dp)[0] = t0[c0];                   \
	    ((mlib_u8 *)dp)[1] = t1[c1];                                \
	    ((mlib_u8 *)dp)[2] = t2[c2];                                \
	    ((mlib_u8 *)dp)[3] = t0[c3];                                \
	    ((mlib_u8 *)dp)[4] = t1[c4];                                \
	    ((mlib_u8 *)dp)[5] = t2[c5];                                \
	    ((mlib_u8 *)dp)[6] = t0[c6];                                \
	    ((mlib_u8 *)dp)[7] = t1[c7];                                \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_U8_3E(dp, sd1, t0, t1, t2)               \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) dp =                                           \
		_mm_mlib_setr_pi8(t0[c0], t1[c1], t2[c2], t0[c3],       \
		t1[c4], t2[c5], t0[c6], t1[c7]);                        \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_U8_3(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

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
			sd1 = sp1[0];
			FN_NAME(U8_U8_3L)(dp1, sd1, tab0, tab1,
			    tab2);
			dp1++;
			sd1 = sp1[1];
			FN_NAME(U8_U8_3L)(dp1, sd1, tab2, tab0,
			    tab1);
			dp1++;
			sd1 = sp1[2];
			FN_NAME(U8_U8_3L)(dp1, sd1, tab1, tab2,
			    tab0);
			dp1++;
			sp1 += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			n1 = (n1 > 7) ? 8 : n1;
			sd1 = _mm_and_si64(sp1[0], ld8_u8[n1]);
			FN_NAME(U8_U8_3E)(dd1, sd1, tab0, tab1,
			    tab2);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				n1 = (n1 > 7) ? 8 : n1;
				sd1 = _mm_and_si64(sp1[1], ld8_u8[n1]);
				FN_NAME(U8_U8_3E)(dd1, sd1, tab2,
				    tab0, tab1);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd1 = _mm_and_si64(sp1[2], ld8_u8[n1]);
					FN_NAME(U8_U8_3E)(dd1, sd1,
					    tab1, tab2, tab0);
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_U8_2L(dp, sd1, t0)                    \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) dp[0] =                                        \
		_mm_mlib_setr_pi16(((mlib_s16 *)t0)[c0],                \
		((mlib_s16 *)t0)[c1], ((mlib_s16 *)t0)[c2],             \
		((mlib_s16 *)t0)[c3]);                                  \
	    dp[1] =                                                     \
		_mm_mlib_setr_pi16(((mlib_s16 *)t0)[c4],                \
		((mlib_s16 *)t0)[c5], ((mlib_s16 *)t0)[c6],             \
		((mlib_s16 *)t0)[c7]);                                  \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_U8_2E(dp, sd1, t0)            \
	{                                                       \
	    mlib_s32 c0 = (sd1) & 0xff;                         \
	    mlib_s32 c1 = (sd1 >> 8) & 0xff;                    \
	    mlib_s32 c2 = (sd1 >> 16) & 0xff;                   \
	    mlib_s32 c3 = (sd1 >> 24) & 0xff;                   \
	                                                        \
	    dp = _mm_mlib_setr_pi16(((mlib_s16 *)t0)[c0],       \
		((mlib_s16 *)t0)[c1], ((mlib_s16 *)t0)[c2],     \
		((mlib_s16 *)t0)[c3]);                          \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 *table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
		dend = (mlib_u8 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			FN_NAME(SI_U8_U8_2L)(dp1, sd1, table);
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1 >> 1]);
			FN_NAME(SI_U8_U8_2E)(dd1,
			    _mm_cvtsi64_si32(sd1), table);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				FN_NAME(SI_U8_U8_2E)(dd1,
				    _mm_cvtsi64_si32(_mm_srli_si64(sd1, 32)),
				    table);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_U8_3L(dp, sd1, t0, t1, t2, ts)          \
	{                                                                 \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,     \
		c7, sd1) * ((mlib_s32 *)((mlib_u8 *)dp + 0)) =            \
		((mlib_s32 *)ts)[c0];                                     \
	    *((mlib_s32 *)((mlib_u8 *)dp + 3)) = ((mlib_s32 *)ts)[c1];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 6)) = ((mlib_s32 *)ts)[c2];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 9)) = ((mlib_s32 *)ts)[c3];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 12)) = ((mlib_s32 *)ts)[c4];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 15)) = ((mlib_s32 *)ts)[c5];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 18)) = ((mlib_s32 *)ts)[c6];   \
	    ((mlib_u8 *)dp)[21] = t0[c7];                                 \
	    ((mlib_u8 *)dp)[22] = t1[c7];                                 \
	    ((mlib_u8 *)dp)[23] = t2[c7];                                 \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_U8_3(
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
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_u8 *dend;
	mlib_u8 *tab0, *tab1, *tab2;

	tab0 = (mlib_u8 *)table[0];
	tab1 = (mlib_u8 *)table[1];
	tab2 = (mlib_u8 *)table[2];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = sp1[0];
			FN_NAME(SI_U8_U8_3L)(dp1, sd1, tab0, tab1,
			    tab2, stbl);
			sp1++;
			dp1 += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1 / 3]);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1)
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_U8_4L(dp, sd1, t0)                      \
	{                                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                      \
	                                                                  \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,     \
		c7, sd1) * ((mlib_s32 *)((mlib_u8 *)dp + 0)) =            \
		((mlib_s32 *)t0)[c0];                                     \
	    *((mlib_s32 *)((mlib_u8 *)dp + 4)) = ((mlib_s32 *)t0)[c1];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 8)) = ((mlib_s32 *)t0)[c2];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 12)) = ((mlib_s32 *)t0)[c3];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 16)) = ((mlib_s32 *)t0)[c4];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 20)) = ((mlib_s32 *)t0)[c5];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 24)) = ((mlib_s32 *)t0)[c6];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 28)) = ((mlib_s32 *)t0)[c7];   \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_U8_4E(dp, t0, c0, c1)         \
	{                                                       \
	    dp = _mm_mlib_setr_pi32(((mlib_s32 *)t0)[c0],       \
		((mlib_s32 *)t0)[c1]);                          \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_u8 *table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 31; n1 -= 32) {
			sd1 = (*sp1++);
			FN_NAME(SI_U8_U8_4L)(dp1, sd1, table);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1 >> 2]);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
			FN_NAME(SI_U8_U8_4E)(dd1, table, c0, c1);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				FN_NAME(SI_U8_U8_4E)(dd1, table, c2,
				    c3);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					FN_NAME(SI_U8_U8_4E)(dd1,
					    table, c4, c5);
					mmx_pst_8(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
					if (n1 > 0) {
						emask = (0xFF << (8 - n1));
						FN_NAME(SI_U8_U8_4E)
						    (dd1, table, c6, c7);
						mmx_pst_8(dd1, dp1, emask);
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
#if 0

void
mlib_a_ImageLookUp_U8_U8_1(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 i, n1, nu;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *tab0 = (mlib_u8 *)table[0];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		n1 = dw;

		nu = ((8 - (mlib_addr)sp1) & 7);
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 8 - nu;

			emask = 0xFF << nr;
			sd1 = *sp1;
			FN_NAME(U8_U8_1E)(dd1, sd1, tab0, tab0, tab0,
			    tab0);
			mmx_pst_8(dd1, dp1, emask);
			sp1 = (__m64 *) ((mlib_u8 *)sl1 + nu);
			dp1 = (__m64 *) ((mlib_u8 *)dl1 + nu);
			n1 -= nu;
		}

		_asm {
			mov ecx, n1
			    cmp ecx, 8
			    jb no_loop_8
			    mov esi, sp1
			    mov edi, dp1
			    mov edx, tab0
			    pxor mm0, mm0
			    nop nop loop_8:movq mm7, MMWORD PTR[esi]
			movq mm6, mm7
			    punpcklbw mm7, mm0
			    punpckhbw mm6, mm0
			    movq mm5, mm7
			    movq mm4, mm6
			    punpcklwd mm7, mm0
			    movd ebx, mm7
			    psrlq mm7, 32 mov al, BYTE PTR[edx][ebx]
			movd ebx, mm7 mov ah, BYTE PTR[edx][ebx]

			mov BYTE PTR[edi + 0], al
			    punpckhwd mm5, mm0
			    movd ebx, mm5
			    mov BYTE PTR[edi + 1], ah mov al, BYTE PTR[edx][ebx]
			psrlq mm5, 32
			    mov BYTE PTR[edi + 2], al
			    movd ebx, mm5 mov ah, BYTE PTR[edx][ebx]

			mov BYTE PTR[edi + 3], ah
			    punpcklwd mm6, mm0
			    movd ebx, mm6 mov al, BYTE PTR[edx][ebx]
			psrlq mm6, 32
			    movd ebx, mm6
			    mov BYTE PTR[edi + 4], al mov ah, BYTE PTR[edx][ebx]
			punpckhwd mm4, mm0
			    mov BYTE PTR[edi + 5], ah
			    movd ebx, mm4 mov al, BYTE PTR[edx][ebx]

			mov BYTE PTR[edi + 6], al
			    psrlq mm4, 32
			    movd ebx, mm4 mov ah, BYTE PTR[edx][ebx]
		add esi, 8
			    sub ecx, 8
			    mov BYTE PTR[edi + 7], ah
			    add edi, 8
			    cmp ecx, 7
			    jg loop_8
			    mov sp1, esi mov dp1, edi mov n1, ecx no_loop_8:}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 = _mm_and_si64(mask_pst_8[emask & 0xFF], sp1[0]);
			FN_NAME(U8_U8_1E)(dd1, sd1, tab0, tab0, tab0,
			    tab0);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

#endif

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_S16_1L(dp, sd1, t0, t1, t2, t3)          \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_s16 *)dp)[0] = t0[c0];                  \
	    ((mlib_s16 *)dp)[1] = t1[c1];                               \
	    ((mlib_s16 *)dp)[2] = t2[c2];                               \
	    ((mlib_s16 *)dp)[3] = t3[c3];                               \
	    ((mlib_s16 *)dp)[4] = t0[c4];                               \
	    ((mlib_s16 *)dp)[5] = t1[c5];                               \
	    ((mlib_s16 *)dp)[6] = t2[c6];                               \
	    ((mlib_s16 *)dp)[7] = t3[c7];                               \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_S16_1E(dp, sd1, t0, t1, t2, t3)         \
	{                                                              \
	    mlib_s32 c0 = (sd1) & 0xff;                                \
	    mlib_s32 c1 = (sd1 >> 8) & 0xff;                           \
	    mlib_s32 c2 = (sd1 >> 16) & 0xff;                          \
	    mlib_s32 c3 = (sd1 >> 24) & 0xff;                          \
	                                                               \
	    dp = _mm_mlib_setr_pi16(t0[c0], t1[c1], t2[c2], t3[c3]);   \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_S16_1(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(U8_S16_1L)(dp1, sd1, tab0, tab0,
			    tab0, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_S16_1E)(dd1,
			    _mm_cvtsi64_si32(sd1), tab0, tab0, tab0, tab0);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				FN_NAME(U8_S16_1E)(dd1,
				    _mm_cvtsi64_si32(_mm_srli_si64(sd1, 32)),
				    tab0, tab0, tab0, tab0);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_S16_2(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

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
			FN_NAME(U8_S16_1L)(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_S16_1E)(dd1,
			    _mm_cvtsi64_si32(sd1), tab0, tab1, tab0, tab1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				FN_NAME(U8_S16_1E)(dd1,
				    _mm_cvtsi64_si32(_mm_srli_si64(sd1, 32)),
				    tab0, tab1, tab0, tab1);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_S16_4(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	mlib_s16 *tab0 = (mlib_s16 *)table[0];
	mlib_s16 *tab1 = (mlib_s16 *)table[1];
	mlib_s16 *tab2 = (mlib_s16 *)table[2];
	mlib_s16 *tab3 = (mlib_s16 *)table[3];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(U8_S16_1L)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_S16_1E)(dd1,
			    _mm_cvtsi64_si32(sd1), tab0, tab1, tab2, tab3);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				FN_NAME(U8_S16_1E)(dd1,
				    _mm_cvtsi64_si32(_mm_srli_si64(sd1, 32)),
				    tab0, tab1, tab2, tab3);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_S16_3L(dp, sd1, t0, t1, t2)              \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_s16 *)dp)[0] = t0[c0];                  \
	    ((mlib_s16 *)dp)[1] = t1[c1];                               \
	    ((mlib_s16 *)dp)[2] = t2[c2];                               \
	    ((mlib_s16 *)dp)[3] = t0[c3];                               \
	    ((mlib_s16 *)dp)[4] = t1[c4];                               \
	    ((mlib_s16 *)dp)[5] = t2[c5];                               \
	    ((mlib_s16 *)dp)[6] = t0[c6];                               \
	    ((mlib_s16 *)dp)[7] = t1[c7];                               \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_S16_3E(dp, sd1, t0, t1, t2)             \
	{                                                              \
	    mlib_s32 c0 = (sd1) & 0xff;                                \
	    mlib_s32 c1 = (sd1 >> 8) & 0xff;                           \
	    mlib_s32 c2 = (sd1 >> 16) & 0xff;                          \
	    mlib_s32 c3 = (sd1 >> 24) & 0xff;                          \
	                                                               \
	    dp = _mm_mlib_setr_pi16(t0[c0], t1[c1], t2[c2], t0[c3]);   \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_S16_3(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

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
		sd1 = sp1[0];
		FN_NAME(U8_S16_3L)(dp1, sd1, tab0, tab1,
		    tab2);
		dp1 += 2;
		sd1 = sp1[1];
		FN_NAME(U8_S16_3L)(dp1, sd1, tab2, tab0,
		    tab1);
		dp1 += 2;
		sd1 = sp1[2];
		FN_NAME(U8_S16_3L)(dp1, sd1, tab1, tab2,
		    tab0);
		dp1 += 2;
		sp1 += 3;
	    }

	    if (n1 > 0) {
		emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		n1 = (n1 > 7) ? 8 : n1;
		sd1 = _mm_and_si64(sp1[0], ld8_u8[n1]);
		FN_NAME(U8_S16_3E)(dd1,
		    _mm_cvtsi64_si32(sd1), tab0, tab1, tab2);
		mmx_pst_16(dd1, dp1, emask);
		dp1++;

		n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
		if (n1 > 0) {
		    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		    FN_NAME(U8_S16_3E)(dd1,
			    _mm_cvtsi64_si32(_mm_srli_si64(sd1, 32)),
			    tab1, tab2, tab0);
		    mmx_pst_16(dd1, dp1, emask);
		    dp1++;

		    n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
		    if (n1 > 0) {
			emask =
			    (n1 > 3) ? 0xF : (0xF << (4 - n1));
				n1 = (n1 > 7) ? 8 : n1;
			sd1 = _mm_and_si64(sp1[1], ld8_u8[n1]);
			FN_NAME(U8_S16_3E)(dd1,
			    _mm_cvtsi64_si32(sd1), tab2, tab0,
			    tab1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
			    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			    FN_NAME(U8_S16_3E)(dd1,
				_mm_cvtsi64_si32
				(_mm_srli_si64(sd1, 32)),
				tab0, tab1, tab2);
			    mmx_pst_16(dd1, dp1, emask);
			    dp1++;

			    n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			    if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd1 =
				    _mm_and_si64(sp1[2],
				    ld8_u8[n1]);
				FN_NAME(U8_S16_3E)
				    (dd1,
				    _mm_cvtsi64_si32
				    (sd1), tab1, tab2,
				    tab0);
				mmx_pst_16(dd1, dp1,
					    emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
				    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				    FN_NAME(U8_S16_3E)
					    (dd1,
					    _mm_cvtsi64_si32
					    (_mm_srli_si64
					    (sd1, 32)),
					    tab2, tab0,
					    tab1);
				    mmx_pst_16(dd1, dp1, emask);
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S16_2L(dp, sd1, t0)                     \
	{                                                                 \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                      \
	                                                                  \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,     \
		c7, sd1) * ((mlib_s32 *)((mlib_u8 *)dp + 0)) =            \
		((mlib_s32 *)t0)[c0];                                     \
	    *((mlib_s32 *)((mlib_u8 *)dp + 4)) = ((mlib_s32 *)t0)[c1];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 8)) = ((mlib_s32 *)t0)[c2];    \
	    *((mlib_s32 *)((mlib_u8 *)dp + 12)) = ((mlib_s32 *)t0)[c3];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 16)) = ((mlib_s32 *)t0)[c4];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 20)) = ((mlib_s32 *)t0)[c5];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 24)) = ((mlib_s32 *)t0)[c6];   \
	    *((mlib_s32 *)((mlib_u8 *)dp + 28)) = ((mlib_s32 *)t0)[c7];   \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S16_2E(dp, t0, c0, c1)        \
	{                                                       \
	    dp = _mm_mlib_setr_pi32(((mlib_s32 *)t0)[c0],       \
		((mlib_s32 *)t0)[c1]);                          \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_S16_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 *table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
		dend = (mlib_s16 *)dp1 + dw;

		for (n1 = dw; n1 > 15; n1 -= 16) {
			sd1 = (*sp1++);
			FN_NAME(SI_U8_S16_2L)(dp1, sd1, table);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1 >> 1]);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
			FN_NAME(SI_U8_S16_2E)(dd1, table, c0, c1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				FN_NAME(SI_U8_S16_2E)(dd1, table, c2,
				    c3);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					FN_NAME(SI_U8_S16_2E)(dd1,
					    table, c4, c5);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask = (0xF << (4 - n1));
						FN_NAME(SI_U8_S16_2E)
						    (dd1, table, c6, c7);
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S16_3L(dp, sd1, t0, t1, t2)           \
	{                                                               \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_s16 *)dp)[0] = t0[c0];                  \
	    ((mlib_s16 *)dp)[1] = t1[c0];                               \
	    ((mlib_s16 *)dp)[2] = t2[c0];                               \
	    ((mlib_s16 *)dp)[3] = t0[c1];                               \
	    ((mlib_s16 *)dp)[4] = t1[c1];                               \
	    ((mlib_s16 *)dp)[5] = t2[c1];                               \
	    ((mlib_s16 *)dp)[6] = t0[c2];                               \
	    ((mlib_s16 *)dp)[7] = t1[c2];                               \
	    ((mlib_s16 *)dp)[8] = t2[c2];                               \
	    ((mlib_s16 *)dp)[9] = t0[c3];                               \
	    ((mlib_s16 *)dp)[10] = t1[c3];                              \
	    ((mlib_s16 *)dp)[11] = t2[c3];                              \
	    ((mlib_s16 *)dp)[12] = t0[c4];                              \
	    ((mlib_s16 *)dp)[13] = t1[c4];                              \
	    ((mlib_s16 *)dp)[14] = t2[c4];                              \
	    ((mlib_s16 *)dp)[15] = t0[c5];                              \
	    ((mlib_s16 *)dp)[16] = t1[c5];                              \
	    ((mlib_s16 *)dp)[17] = t2[c5];                              \
	    ((mlib_s16 *)dp)[18] = t0[c6];                              \
	    ((mlib_s16 *)dp)[19] = t1[c6];                              \
	    ((mlib_s16 *)dp)[20] = t2[c6];                              \
	    ((mlib_s16 *)dp)[21] = t0[c7];                              \
	    ((mlib_s16 *)dp)[22] = t1[c7];                              \
	    ((mlib_s16 *)dp)[23] = t2[c7];                              \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_S16_3(
    const mlib_u8 *src,
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
	__m64 *sp1, *sl1, sd1;

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
		FN_NAME(SI_U8_S16_3L)(dp1, sd1, tab0, tab1,
		    tab2);
		dp1 += 6;
	    }

	    if (n1 > 0) {
		emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		sd1 = _mm_and_si64(*sp1, ld8_u8[n1 / 3]);
		FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
		    c6, c7, sd1)
		dd1 =
		    _mm_mlib_setr_pi16(tab0[c0], tab1[c0], tab2[c0],
		    tab0[c1]);
		mmx_pst_16(dd1, dp1, emask);
		dp1++;

		n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
		if (n1 > 0) {
		    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		    dd1 = _mm_mlib_setr_pi16(tab1[c1], tab2[c1],
				tab0[c2], tab1[c2]);
		    mmx_pst_16(dd1, dp1, emask);
		    dp1++;

		    n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
		    if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			dd1 = _mm_mlib_setr_pi16(tab2[c2], tab0[c3],
					tab1[c3], tab2[c3]);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
			    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			    dd1 = _mm_mlib_setr_pi16(tab0[c4],
				    tab1[c4], tab2[c4],
				    tab0[c5]);
			    mmx_pst_16(dd1, dp1, emask);
			    dp1++;

			    n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			    if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				dd1 = _mm_mlib_setr_pi16(tab1[c5], tab2[c5],
					tab0[c6], tab1[c6]);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
				    emask = (0xF << (4 - n1));
				    dd1 = _mm_mlib_setr_pi16
						(tab2[c6], tab0[c7],
						tab1[c7], tab2[c7]);
				    mmx_pst_16(dd1, dp1, emask);
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S16_4L(dp, sd1, t0)                   \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) dp[0] = ((__m64 *) t0)[c0];                    \
	    dp[1] = ((__m64 *) t0)[c1];                                 \
	    dp[2] = ((__m64 *) t0)[c2];                                 \
	    dp[3] = ((__m64 *) t0)[c3];                                 \
	    dp[4] = ((__m64 *) t0)[c4];                                 \
	    dp[5] = ((__m64 *) t0)[c5];                                 \
	    dp[6] = ((__m64 *) t0)[c6];                                 \
	    dp[7] = ((__m64 *) t0)[c7];                                 \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S16_4E(dp, t0, c0)            \
	{                                                       \
	    dp = ((__m64 *) t0)[c0];                            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_S16_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s16 *table)
{

/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {
	    n1 = dw;
	    dend = (mlib_s16 *)dp1 + dw;

	    for (; n1 > 31; n1 -= 32) {
		sd1 = (*sp1++);
		FN_NAME(SI_U8_S16_4L)(dp1, sd1, table);
		dp1 += 8;
	    }

	    if (n1 > 0) {
		mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

		emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		sd1 = _mm_and_si64(*sp1, ld8_u8[n1 >> 2]);
		FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
		FN_NAME(SI_U8_S16_4E)(dd1, table, c0);
		mmx_pst_16(dd1, dp1, emask);
		dp1++;

		n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
		if (n1 > 0) {
		    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		    FN_NAME(SI_U8_S16_4E)(dd1, table,
					c1);
		    mmx_pst_16(dd1, dp1, emask);
		    dp1++;

		    n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
		    if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			FN_NAME(SI_U8_S16_4E)(dd1, table, c2);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
			    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			    FN_NAME(SI_U8_S16_4E)(dd1, table, c3);
			    mmx_pst_16(dd1, dp1, emask);
			    dp1++;

			    n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			    if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				FN_NAME(SI_U8_S16_4E)(dd1, table, c4);
				mmx_pst_16(dd1, dp1,
						emask);
				dp1++;

				n1 = ((mlib_s16 *)dend -
						(mlib_s16 *)dp1);
				if (n1 > 0) {
				    emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				    FN_NAME(SI_U8_S16_4E)
						(dd1, table, c5);
				    mmx_pst_16(dd1,
							dp1, emask);
				    dp1++;

				    n1 = ((mlib_s16 *)dend - (mlib_s16 *) dp1);
				    if (n1 > 0) {
					emask = (n1 > 3) ? 0xF :
							(0xF << (4 - n1));
					FN_NAME(SI_U8_S16_4E)(dd1, table,
						    c6);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
						(mlib_s16 *)dp1);
					if (n1 > 0) {
					    emask = (0xF << (4 - n1));
					    FN_NAME(SI_U8_S16_4E)
							(dd1, table, c7);
					    mmx_pst_16(dd1, dp1, emask);
					}
				    }
				}
			    }
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

#define	MLIB_M_IMAGE_LOOKUP_U8_S32_1L(dp, sd1, t0, t1, t2, t3)          \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_s32 *)dp)[0] = t0[c0];                  \
	    ((mlib_s32 *)dp)[1] = t1[c1];                               \
	    ((mlib_s32 *)dp)[2] = t2[c2];                               \
	    ((mlib_s32 *)dp)[3] = t3[c3];                               \
	    ((mlib_s32 *)dp)[4] = t0[c4];                               \
	    ((mlib_s32 *)dp)[5] = t1[c5];                               \
	    ((mlib_s32 *)dp)[6] = t2[c6];                               \
	    ((mlib_s32 *)dp)[7] = t3[c7];                               \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_S32_1E(dp, t0, t1, c0, c1)       \
	{                                                       \
	    dp = _mm_mlib_setr_pi32(t0[c0], t1[c1]);            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_S32_1(
    const mlib_u8 *src,
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
	mlib_s32 *dend;
	mlib_s32 *tab0 = (mlib_s32 *)table[0];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(U8_S32_1L)(dp1, sd1, tab0, tab0,
			    tab0, tab0);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

			emask = (n1 > 1) ? 3 : 2;
			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
			FN_NAME(U8_S32_1E)(dd1, tab0, tab0, c0, c1);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				FN_NAME(U8_S32_1E)(dd1, tab0, tab0,
				    c2, c3);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					emask = (n1 > 1) ? 3 : 2;
					FN_NAME(U8_S32_1E)(dd1, tab0,
					    tab0, c4, c5);
					mmx_pst_32(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						FN_NAME(U8_S32_1E)
						    (dd1, tab0, tab0, c6, c7);
						mmx_pst_32(dd1, dp1, 2);
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

void
mlib_m_ImageLookUp_U8_S32_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m64 *dp1, *dl1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

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
			FN_NAME(U8_S32_1L)(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
			FN_NAME(U8_S32_1E)(dp1[0], tab0, tab1, c0,
			    c1);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				FN_NAME(U8_S32_1E)(dp1[0], tab0,
				    tab1, c2, c3);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					FN_NAME(U8_S32_1E)(dp1[0],
					    tab0, tab1, c4, c5);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						FN_NAME(U8_S32_1E)
						    (dp1[0], tab0, tab1, c6,
						    c7);
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

void
mlib_m_ImageLookUp_U8_S32_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 **table)
{

/* dst address */
	__m64 *dp1, *dl1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 *dend;
	mlib_s32 *tab0 = (mlib_s32 *)table[0];
	mlib_s32 *tab1 = (mlib_s32 *)table[1];
	mlib_s32 *tab2 = (mlib_s32 *)table[2];
	mlib_s32 *tab3 = (mlib_s32 *)table[3];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {
		dend = (mlib_s32 *)dp1 + dw;

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(U8_S32_1L)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 4;
		}

		if (n1 > 0) {
			mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

			sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
			FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
			FN_NAME(U8_S32_1E)(dp1[0], tab0, tab1, c0,
			    c1);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
				FN_NAME(U8_S32_1E)(dp1[0], tab2,
				    tab3, c2, c3);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
					FN_NAME(U8_S32_1E)(dp1[0],
					    tab0, tab1, c4, c5);
					dp1++;

					n1 = ((mlib_s32 *)dend -
					    (mlib_s32 *)dp1);
					if (n1 > 0) {
						FN_NAME(U8_S32_1E)
						    (dp1[0], tab2, tab3, c6,
						    c7);
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

#define	MLIB_M_IMAGE_LOOKUP_U8_S32_3L(dp, sd1, t0, t1, t2)              \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_s32 *)dp)[0] = t0[c0];                  \
	    ((mlib_s32 *)dp)[1] = t1[c1];                               \
	    ((mlib_s32 *)dp)[2] = t2[c2];                               \
	    ((mlib_s32 *)dp)[3] = t0[c3];                               \
	    ((mlib_s32 *)dp)[4] = t1[c4];                               \
	    ((mlib_s32 *)dp)[5] = t2[c5];                               \
	    ((mlib_s32 *)dp)[6] = t0[c6];                               \
	    ((mlib_s32 *)dp)[7] = t1[c7];                               \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_U8_S32_3E(dp, t0, t1, c0, c1)       \
	{                                                       \
	    dp = _mm_mlib_setr_pi32(t0[c0], t1[c1]);            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_U8_S32_3(
    const mlib_u8 *src,
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

	    for (n1 = dw; n1 > 23; n1 -= 24) {
		sd1 = sp1[0];
		FN_NAME(U8_S32_3L)(dp1, sd1, tab0, tab1,
			    tab2);
		dp1 += 4;
		sd1 = sp1[1];
		FN_NAME(U8_S32_3L)(dp1, sd1, tab2, tab0,
			    tab1);
		dp1 += 4;
		sd1 = sp1[2];
		FN_NAME(U8_S32_3L)(dp1, sd1, tab1, tab2,
			    tab0);
		dp1 += 4;
		sp1 += 3;
	    }

	    if (n1 > 0) {
		mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

		emask = (n1 > 1) ? 3 : 2;
		n1 = (n1 > 7) ? 8 : n1;
		sd1 = _mm_and_si64(sp1[0], ld8_u8[n1]);
		FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
		FN_NAME(U8_S32_3E)(dd1, tab0, tab1, c0, c1);
		mmx_pst_32(dd1, dp1, emask);
		dp1++;

		n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
		if (n1 > 0) {
		    emask = (n1 > 1) ? 3 : 2;
		    FN_NAME(U8_S32_3E)(dd1, tab2, tab0,
				    c2, c3);
		    mmx_pst_32(dd1, dp1, emask);
		    dp1++;

		    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
		    if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			FN_NAME(U8_S32_3E)(dd1, tab1,
				    tab2, c4, c5);
			mmx_pst_32(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
			    emask = (n1 > 1) ? 3 : 2;
			    FN_NAME(U8_S32_3E)(dd1, tab0, tab1, c6, c7);
			    mmx_pst_32(dd1, dp1, emask);
			    dp1++;

			    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			    if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				n1 = (n1 > 7) ? 8 : n1;
				sd1 = _mm_and_si64(sp1[1], ld8_u8[n1]);
				FN_NAME(U8_2INDEX)
					    (c0, c1, c2, c3, c4,
						    c5, c6, c7, sd1);
				FN_NAME(U8_S32_3E)(dd1, tab2, tab0,
						    c0, c1);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
				    emask = (n1 > 1) ? 3 : 2;
				    FN_NAME(U8_S32_3E)(dd1, tab1, tab2, c2, c3);
				    mmx_pst_32(dd1, dp1, emask);
				    dp1++;

				    n1 = ((mlib_s32 *)dend - (mlib_s32 *) dp1);
				    if (n1 > 0) {
					emask = (n1 > 1) ? 3 : 2;
					FN_NAME(U8_S32_3E)(dd1, tab0,
								tab1, c4, c5);
					mmx_pst_32(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s32 *)dend -
							(mlib_s32 *)dp1);
					if (n1 > 0) {
					    emask = (n1 > 1) ? 3 : 2;
					    FN_NAME(U8_S32_3E)(dd1, tab2,
								tab0, c6, c7);
					    mmx_pst_32(dd1, dp1, emask);
					    dp1++;

					    n1 = ((mlib_s32 *)dend -
							(mlib_s32 *)dp1);
					    if (n1 > 0) {
						emask = (n1 > 1) ? 3 : 2;
						n1 = (n1 > 7) ? 8 : n1;
						sd1 = _mm_and_si64(sp1[2],
							ld8_u8[n1]);
						FN_NAME(U8_2INDEX)
							(c0, c1, c2, c3, c4,
								c5, c6, c7,
								sd1);
						FN_NAME(U8_S32_3E)
							    (dd1, tab1,
								tab2, c0, c1);
						mmx_pst_32(dd1, dp1, emask);
						dp1++;

						n1 = ((mlib_s32 *)dend -
							(mlib_s32 *)dp1);
						if (n1 > 0) {
						    emask = (n1 > 1) ? 3 : 2;
						    FN_NAME(U8_S32_3E)
							    (dd1, tab0,
								tab1, c2, c3);
						    mmx_pst_32(dd1, dp1, emask);
						    dp1++;

						    n1 = ((mlib_s32 *)dend -
							    (mlib_s32 *)dp1);
						    if (n1 > 0) {
							emask = (n1 > 1) ?
									3 : 2;
							FN_NAME(U8_S32_3E)
								(dd1, tab2,
								    tab0,
								    c4, c5);
							mmx_pst_32
								(dd1, dp1,
								    emask);
							dp1++;

							n1 = ((mlib_s32 *)
								dend -
								(mlib_s32 *)
									dp1);
							if (n1 > 0) {
							    emask = (n1 > 1) ?
									3 : 2;
							    FN_NAME(U8_S32_3E)
								    (dd1, tab1,
									tab2,
									c6,
									c7);
							    mmx_pst_32
								(dd1, dp1,
								    emask);
							}
						    }
						}
					    }
					}
				    }
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S32_2L(dp, sd1, t0)                   \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) dp[0] = ((__m64 *) t0)[c0];                    \
	    dp[1] = ((__m64 *) t0)[c1];                                 \
	    dp[2] = ((__m64 *) t0)[c2];                                 \
	    dp[3] = ((__m64 *) t0)[c3];                                 \
	    dp[4] = ((__m64 *) t0)[c4];                                 \
	    dp[5] = ((__m64 *) t0)[c5];                                 \
	    dp[6] = ((__m64 *) t0)[c6];                                 \
	    dp[7] = ((__m64 *) t0)[c7];                                 \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S32_2E(dp, t0, c0)            \
	{                                                       \
	    dp = ((__m64 *) t0)[c0];                            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_S32_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 *table)
{

/* dst address */
	__m64 *dp1, *dl1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 *dend;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {
	    dend = (mlib_s32 *)dp1 + dw;

	    for (n1 = dw; n1 > 15; n1 -= 16) {
		sd1 = (*sp1++);
		FN_NAME(SI_U8_S32_2L)(dp1, sd1, table);
		dp1 += 8;
	    }

	    if (n1 > 0) {
		mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

		sd1 = _mm_and_si64(*sp1, ld8_u8[n1 >> 1]);
		FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
		FN_NAME(SI_U8_S32_2E)(dp1[0], table, c0);
		dp1++;

		n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
		if (n1 > 0) {
		    FN_NAME(SI_U8_S32_2E)(dp1[0], table, c1);
		    dp1++;

		    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
		    if (n1 > 0) {
			FN_NAME(SI_U8_S32_2E)(dp1[0], table, c2);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
			    FN_NAME(SI_U8_S32_2E)(dp1[0], table, c3);
			    dp1++;

			    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			    if (n1 > 0) {
				FN_NAME(SI_U8_S32_2E)
				    (dp1[0], table, c4);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
				    FN_NAME(SI_U8_S32_2E)
					(dp1[0], table, c5);
				    dp1++;

				    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				    if (n1 > 0) {
					FN_NAME(SI_U8_S32_2E)
					    (dp1[0], table, c6);
					dp1++;

					n1 = ((mlib_s32 *)dend -
						(mlib_s32 *)dp1);
					if (n1 > 0) {
					    FN_NAME(SI_U8_S32_2E)
						    (dp1[0], table, c7);
					}
				    }
				}
			    }
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S32_3L(dp, sd1, t0, t1, t2)           \
	{                                                               \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) ((mlib_s32 *)dp)[0] = t0[c0];                  \
	    ((mlib_s32 *)dp)[1] = t1[c0];                               \
	    ((mlib_s32 *)dp)[2] = t2[c0];                               \
	    ((mlib_s32 *)dp)[3] = t0[c1];                               \
	    ((mlib_s32 *)dp)[4] = t1[c1];                               \
	    ((mlib_s32 *)dp)[5] = t2[c1];                               \
	    ((mlib_s32 *)dp)[6] = t0[c2];                               \
	    ((mlib_s32 *)dp)[7] = t1[c2];                               \
	    ((mlib_s32 *)dp)[8] = t2[c2];                               \
	    ((mlib_s32 *)dp)[9] = t0[c3];                               \
	    ((mlib_s32 *)dp)[10] = t1[c3];                              \
	    ((mlib_s32 *)dp)[11] = t2[c3];                              \
	    ((mlib_s32 *)dp)[12] = t0[c4];                              \
	    ((mlib_s32 *)dp)[13] = t1[c4];                              \
	    ((mlib_s32 *)dp)[14] = t2[c4];                              \
	    ((mlib_s32 *)dp)[15] = t0[c5];                              \
	    ((mlib_s32 *)dp)[16] = t1[c5];                              \
	    ((mlib_s32 *)dp)[17] = t2[c5];                              \
	    ((mlib_s32 *)dp)[18] = t0[c6];                              \
	    ((mlib_s32 *)dp)[19] = t1[c6];                              \
	    ((mlib_s32 *)dp)[20] = t2[c6];                              \
	    ((mlib_s32 *)dp)[21] = t0[c7];                              \
	    ((mlib_s32 *)dp)[22] = t1[c7];                              \
	    ((mlib_s32 *)dp)[23] = t2[c7];                              \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_S32_3(
    const mlib_u8 *src,
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
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
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

	    for (; n1 > 23; n1 -= 24) {
		sd1 = (*sp1++);
		FN_NAME(SI_U8_S32_3L)(dp1, sd1, tab0, tab1, tab2);
		dp1 += 12;
	    }

	    if (n1 > 0) {
		sd1 = _mm_and_si64(*sp1, ld8_u8[n1 / 3]);
		FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1)
		dp1[0] = _mm_mlib_setr_pi32(tab0[c0], tab1[c0]);
		dp1++;

		n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
		if (n1 > 0) {
		    emask = (n1 > 1) ? 3 : 2;
		    dd1 = _mm_mlib_setr_pi32(tab2[c0], tab0[c1]);
		    mmx_pst_32(dd1, dp1, emask);
		    dp1++;

		    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
		    if (n1 > 0) {
			dp1[0] = _mm_mlib_setr_pi32(tab1[c1], tab2[c1]);
			dp1++;

			n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			if (n1 > 0) {
			    dp1[0] = _mm_mlib_setr_pi32(tab0[c2], tab1[c2]);
			    dp1++;

			    n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
			    if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				dd1 = _mm_mlib_setr_pi32(tab2[c2], tab0[c3]);
				mmx_pst_32(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s32 *)dend - (mlib_s32 *)dp1);
				if (n1 > 0) {
				    dp1[0] = _mm_mlib_setr_pi32(tab1[c3],
							tab2[c3]);
				    dp1++;

				    n1 = ((mlib_s32 *)dend -
						(mlib_s32 *)dp1);
				    if (n1 > 0) {
					dp1[0] = _mm_mlib_setr_pi32(tab0[c4],
							tab1[c4]);
					dp1++;

					n1 = ((mlib_s32 *)dend -
						(mlib_s32 *)dp1);
					if (n1 > 0) {
					    emask = (n1 > 1) ? 3 : 2;
					    dd1 = _mm_mlib_setr_pi32(tab2[c4],
							tab0[c5]);
					    mmx_pst_32(dd1, dp1, emask);
					    dp1++;

					    n1 = ((mlib_s32 *)dend -
						(mlib_s32 *)dp1);
					    if (n1 > 0) {
						dp1[0] = _mm_mlib_setr_pi32(
							tab1[c5],
							tab2[c5]);
						dp1++;

						n1 = ((mlib_s32 *)dend -
							(mlib_s32 *)dp1);
						if (n1 > 0) {
						    dp1[0] =
							_mm_mlib_setr_pi32(
								tab0[c6],
								tab1[c6]);
						    dp1++;

						    n1 = ((mlib_s32 *)dend -
							(mlib_s32 *)dp1);
						    if (n1 > 0) {
							emask = (n1 > 1) ?
									3 : 2;
							dd1 =
							    _mm_mlib_setr_pi32(
								tab2[c6],
								tab0[c7]);
							mmx_pst_32(dd1,
								dp1, emask);
							dp1++;

							n1 = ((mlib_s32 *)
								dend -
								(mlib_s32 *)
									dp1);
							if (n1 > 0) {
							    dp1[0] =
							    _mm_mlib_setr_pi32(
								    tab1[c7],
								    tab2[c7]);
							}
						    }
						}
					    }
					}
				    }
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

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S32_4L(dp, sd1, t0)                   \
	{                                                               \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;                    \
	                                                                \
	    MLIB_M_IMAGE_LOOKUP_U8_2INDEX(c0, c1, c2, c3, c4, c5, c6,   \
		c7, sd1) dp[0] = ((__m64 *) t0)[2 * c0 + 0];            \
	    dp[1] = ((__m64 *) t0)[2 * c0 + 1];                         \
	    dp[2] = ((__m64 *) t0)[2 * c1 + 0];                         \
	    dp[3] = ((__m64 *) t0)[2 * c1 + 1];                         \
	    dp[4] = ((__m64 *) t0)[2 * c2 + 0];                         \
	    dp[5] = ((__m64 *) t0)[2 * c2 + 1];                         \
	    dp[6] = ((__m64 *) t0)[2 * c3 + 0];                         \
	    dp[7] = ((__m64 *) t0)[2 * c3 + 1];                         \
	    dp[8] = ((__m64 *) t0)[2 * c4 + 0];                         \
	    dp[9] = ((__m64 *) t0)[2 * c4 + 1];                         \
	    dp[10] = ((__m64 *) t0)[2 * c5 + 0];                        \
	    dp[11] = ((__m64 *) t0)[2 * c5 + 1];                        \
	    dp[12] = ((__m64 *) t0)[2 * c6 + 0];                        \
	    dp[13] = ((__m64 *) t0)[2 * c6 + 1];                        \
	    dp[14] = ((__m64 *) t0)[2 * c7 + 0];                        \
	    dp[15] = ((__m64 *) t0)[2 * c7 + 1];                        \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_U8_S32_4E(dp, t0, c0)            \
	{                                                       \
	    dp[0] = ((__m64 *) t0)[2 * c0 + 0];                 \
	    dp[1] = ((__m64 *) t0)[2 * c0 + 1];                 \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_U8_S32_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 dw,
    mlib_s32 dh,
    const mlib_s32 *table)
{

/* dst address */
	__m64 *dp1, *dl1;

/* src1 address */
	__m64 *sp1, *sl1, sd1;

/* loop variables */
	mlib_s32 n1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
	    n1 = dw;

	    for (; n1 > 7; n1 -= 8) {
		sd1 = (*sp1++);
		FN_NAME(SI_U8_S32_4L)(dp1, sd1, table);
		dp1 += 16;
	    }

	    if (n1 > 0) {
		mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

		sd1 = _mm_and_si64(*sp1, ld8_u8[n1]);
		FN_NAME(U8_2INDEX)(c0, c1, c2, c3, c4, c5,
			    c6, c7, sd1);
		FN_NAME(SI_U8_S32_4E)(dp1, table, c0);
		dp1 += 2;

		n1--;
		if (n1 > 0) {
		    FN_NAME(SI_U8_S32_4E)(dp1, table,
				    c1);
		    dp1 += 2;

		    n1--;
		    if (n1 > 0) {
			FN_NAME(SI_U8_S32_4E)(dp1,
				    table, c2);
			dp1 += 2;

			n1--;
			if (n1 > 0) {
			    FN_NAME(SI_U8_S32_4E)
				    (dp1, table, c3);
			    dp1 += 2;

			    n1--;
			    if (n1 > 0) {
				FN_NAME(SI_U8_S32_4E)
					    (dp1, table, c4);
				dp1 += 2;

				n1--;
				if (n1 > 0) {
				    FN_NAME(SI_U8_S32_4E)
					    (dp1, table,
						    c5);
				    dp1 += 2;

				    n1--;
				    if (n1 > 0) {
					FN_NAME(SI_U8_S32_4E)
						    (dp1, table, c6);
					dp1 += 2;

					n1--;
					if (n1 > 0) {
					    FN_NAME(SI_U8_S32_4E)
						    (dp1, table, c7);
					}
				    }
				}
			    }
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

#define	MLIB_C_IMAGELOOKUP(DTYPE, STYPE, TABLE)                           \
	{                                                                 \
	    mlib_s32 i, j, k;                                             \
	                                                                  \
	    if (xsize < 2) {                                              \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {     \
		    for (k = 0; k < csize; k++) {                         \
			DTYPE *da = dst + k;                              \
			STYPE *sa = src + k;                              \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
	                                                                  \
			for (i = 0; i < xsize;                            \
			    i++, da += csize, sa += csize)                \
			    *da = tab[*sa];                               \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {     \
		    for (k = 0; k < csize; k++) {                         \
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
		    }                                                     \
		}                                                         \
	    }                                                             \
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
