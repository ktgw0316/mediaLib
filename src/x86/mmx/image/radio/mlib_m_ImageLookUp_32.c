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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImageLookUp_32.c	9.2	07/11/05 SMI"

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

#include <mlib_image.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#define	FN_NAME(F)	MLIB_M_IMAGE_LOOKUP_##F

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1)             \
	c0 = _mm_cvtsi64_si32(sd1);                             \
	c1 = _mm_cvtsi64_si32(_mm_srli_si64(sd1, 32))

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_U8_1L(dp, sd1, sd2, sd3, sd4,   \
	t0, t1, t2, t3)                                         \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_u8 *)dp)[0] = t0[c0];                        \
	    ((mlib_u8 *)dp)[1] = t1[c1];                        \
	    ((mlib_u8 *)dp)[2] = t2[c2];                        \
	    ((mlib_u8 *)dp)[3] = t3[c3];                        \
	    ((mlib_u8 *)dp)[4] = t0[c4];                        \
	    ((mlib_u8 *)dp)[5] = t1[c5];                        \
	    ((mlib_u8 *)dp)[6] = t2[c6];                        \
	    ((mlib_u8 *)dp)[7] = t3[c7];                        \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_U8_1E(dp, sd1, sd2, sd3, sd4,   \
	t0, t1, t2, t3)                                         \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_u8 *)dp)[0] = t0[c0];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[1] = t1[c1];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[2] = t2[c2];            \
	    if (n1 > 3) ((mlib_u8 *)dp)[3] = t3[c3];            \
	    if (n1 > 4) ((mlib_u8 *)dp)[4] = t0[c4];            \
	    if (n1 > 5) ((mlib_u8 *)dp)[5] = t1[c5];            \
	    if (n1 > 6) ((mlib_u8 *)dp)[6] = t2[c6];            \
	    if (n1 > 7) ((mlib_u8 *)dp)[7] = t3[c7];            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_U8_1(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;
	const mlib_u8 *tab0 = &table[0][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_1L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab0, tab0, tab0);
			dp1++;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_1E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab0, tab0, tab0);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_U8_2(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

	const mlib_u8 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_u8 *tab1 = &table[1][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_1L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_1E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab0, tab1);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_U8_4(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

	const mlib_u8 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_u8 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_u8 *tab2 = &table[2][(mlib_u32)2147483648];
	const mlib_u8 *tab3 = &table[3][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_1L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2, tab3);
			dp1++;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_1E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2, tab3);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_U8_3L(dp, sd1, sd2, sd3, sd4,   \
	t0, t1, t2)                                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_u8 *)dp)[0] = t0[c0];                        \
	    ((mlib_u8 *)dp)[1] = t1[c1];                        \
	    ((mlib_u8 *)dp)[2] = t2[c2];                        \
	    ((mlib_u8 *)dp)[3] = t0[c3];                        \
	    ((mlib_u8 *)dp)[4] = t1[c4];                        \
	    ((mlib_u8 *)dp)[5] = t2[c5];                        \
	    ((mlib_u8 *)dp)[6] = t0[c6];                        \
	    ((mlib_u8 *)dp)[7] = t1[c7];                        \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_U8_3E(dp, sd1, sd2, sd3, sd4,   \
	t0, t1, t2)                                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_u8 *)dp)[0] = t0[c0];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[1] = t1[c1];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[2] = t2[c2];            \
	    if (n1 > 3) ((mlib_u8 *)dp)[3] = t0[c3];            \
	    if (n1 > 4) ((mlib_u8 *)dp)[4] = t1[c4];            \
	    if (n1 > 5) ((mlib_u8 *)dp)[5] = t2[c5];            \
	    if (n1 > 6) ((mlib_u8 *)dp)[6] = t0[c6];            \
	    if (n1 > 7) ((mlib_u8 *)dp)[7] = t1[c7];            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_U8_3(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

/* edge mask */

	const mlib_u8 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = &table[0][(mlib_u32)2147483648];
		tab1 = &table[1][(mlib_u32)2147483648];
		tab2 = &table[2][(mlib_u32)2147483648];

		for (n1 = dw; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2);
			dp1++;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab2, tab0, tab1);
			dp1++;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab1, tab2, tab0);
			dp1++;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2);
			dp1++;
			n1 -= 8;
		}
		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab2, tab0, tab1);
			dp1++;
			n1 -= 8;
		}
		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_U8_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab1, tab2, tab0);
			dp1++;
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_U8_2L(dp, sd1, sd2, t0,      \
	t1, t2, t3)                                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    ((mlib_u8 *)dp)[0] = t0[c0];                        \
	    ((mlib_u8 *)dp)[1] = t1[c0];                        \
	    ((mlib_u8 *)dp)[2] = t2[c1];                        \
	    ((mlib_u8 *)dp)[3] = t3[c1];                        \
	    ((mlib_u8 *)dp)[4] = t0[c2];                        \
	    ((mlib_u8 *)dp)[5] = t1[c2];                        \
	    ((mlib_u8 *)dp)[6] = t2[c3];                        \
	    ((mlib_u8 *)dp)[7] = t3[c3];                        \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_U8_2E(dp, sd1, sd2, t0,      \
	t1, t2, t3)                                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    if (n1 > 0) ((mlib_u8 *)dp)[0] = t0[c0];            \
	    if (n1 > 0) ((mlib_u8 *)dp)[1] = t1[c0];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[2] = t2[c1];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[3] = t3[c1];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[4] = t0[c2];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[5] = t1[c2];            \
	    if (n1 > 3) ((mlib_u8 *)dp)[6] = t2[c3];            \
	    if (n1 > 3) ((mlib_u8 *)dp)[7] = t3[c3];            \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_U8_2(
    const mlib_s32 *src,
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

	const mlib_u8 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_u8 *tab1 = &table[1][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 1;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(SI_S32_U8_2L)(dp1, sd1, sd2, tab0,
			    tab1, tab0, tab1);
			dp1++;
		}

		if (n1 > 0) {
			n1 >>= 1;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(SI_S32_U8_2E)(dp1, sd1, sd2, tab0,
			    tab1, tab0, tab1);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_U8_3L(dp, sd1, sd2, sd3,     \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_u8 *)dp)[0] = t0[c0];                        \
	    ((mlib_u8 *)dp)[1] = t1[c0];                        \
	    ((mlib_u8 *)dp)[2] = t2[c0];                        \
	    ((mlib_u8 *)dp)[3] = t0[c1];                        \
	    ((mlib_u8 *)dp)[4] = t1[c1];                        \
	    ((mlib_u8 *)dp)[5] = t2[c1];                        \
	    ((mlib_u8 *)dp)[6] = t0[c2];                        \
	    ((mlib_u8 *)dp)[7] = t1[c2];                        \
	    ((mlib_u8 *)dp)[8] = t2[c2];                        \
	    ((mlib_u8 *)dp)[9] = t0[c3];                        \
	    ((mlib_u8 *)dp)[10] = t1[c3];                       \
	    ((mlib_u8 *)dp)[11] = t2[c3];                       \
	    ((mlib_u8 *)dp)[12] = t0[c4];                       \
	    ((mlib_u8 *)dp)[13] = t1[c4];                       \
	    ((mlib_u8 *)dp)[14] = t2[c4];                       \
	    ((mlib_u8 *)dp)[15] = t0[c5];                       \
	    ((mlib_u8 *)dp)[16] = t1[c5];                       \
	    ((mlib_u8 *)dp)[17] = t2[c5];                       \
	    ((mlib_u8 *)dp)[18] = t0[c6];                       \
	    ((mlib_u8 *)dp)[19] = t1[c6];                       \
	    ((mlib_u8 *)dp)[20] = t2[c6];                       \
	    ((mlib_u8 *)dp)[21] = t0[c7];                       \
	    ((mlib_u8 *)dp)[22] = t1[c7];                       \
	    ((mlib_u8 *)dp)[23] = t2[c7];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_U8_3E(dp, sd1, sd2, sd3,     \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_u8 *)dp)[0] = t0[c0];            \
	    if (n1 > 0) ((mlib_u8 *)dp)[1] = t1[c0];            \
	    if (n1 > 0) ((mlib_u8 *)dp)[2] = t2[c0];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[3] = t0[c1];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[4] = t1[c1];            \
	    if (n1 > 1) ((mlib_u8 *)dp)[5] = t2[c1];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[6] = t0[c2];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[7] = t1[c2];            \
	    if (n1 > 2) ((mlib_u8 *)dp)[8] = t2[c2];            \
	    if (n1 > 3) ((mlib_u8 *)dp)[9] = t0[c3];            \
	    if (n1 > 3) ((mlib_u8 *)dp)[10] = t1[c3];           \
	    if (n1 > 3) ((mlib_u8 *)dp)[11] = t2[c3];           \
	    if (n1 > 4) ((mlib_u8 *)dp)[12] = t0[c4];           \
	    if (n1 > 4) ((mlib_u8 *)dp)[13] = t1[c4];           \
	    if (n1 > 4) ((mlib_u8 *)dp)[14] = t2[c4];           \
	    if (n1 > 5) ((mlib_u8 *)dp)[15] = t0[c5];           \
	    if (n1 > 5) ((mlib_u8 *)dp)[16] = t1[c5];           \
	    if (n1 > 5) ((mlib_u8 *)dp)[17] = t2[c5];           \
	    if (n1 > 6) ((mlib_u8 *)dp)[18] = t0[c6];           \
	    if (n1 > 6) ((mlib_u8 *)dp)[19] = t1[c6];           \
	    if (n1 > 6) ((mlib_u8 *)dp)[20] = t2[c6];           \
	    if (n1 > 7) ((mlib_u8 *)dp)[21] = t0[c7];           \
	    if (n1 > 7) ((mlib_u8 *)dp)[22] = t1[c7];           \
	    if (n1 > 7) ((mlib_u8 *)dp)[23] = t2[c7];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_U8_3(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;

	const mlib_u8 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_u8 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_u8 *tab2 = &table[2][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(SI_S32_U8_3L)(dp1, sd1, sd2, sd3,
			    sd4, tab0, tab1, tab2);
			dp1 += 3;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			n1 = n1 / 3;
			FN_NAME(SI_S32_U8_3E)(dp1, sd1, sd2, sd3,
			    sd4, tab0, tab1, tab2);
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_U8_4L(dp, sd1, t0, t1, t2, t3)   \
	{                                                           \
	    mlib_s32 c0, c1;                                        \
	                                                            \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);            \
	    ((mlib_u8 *)dp)[0] = t0[c0];                            \
	    ((mlib_u8 *)dp)[1] = t1[c0];                            \
	    ((mlib_u8 *)dp)[2] = t2[c0];                            \
	    ((mlib_u8 *)dp)[3] = t3[c0];                            \
	    ((mlib_u8 *)dp)[4] = t0[c1];                            \
	    ((mlib_u8 *)dp)[5] = t1[c1];                            \
	    ((mlib_u8 *)dp)[6] = t2[c1];                            \
	    ((mlib_u8 *)dp)[7] = t3[c1];                            \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_U8_4E(dp, sd1, t0, t1, t2, t3)       \
	{                                                               \
	    mlib_s32 c0, c1;                                            \
                                                                        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);                \
	    if (n1 > 0) ((mlib_u8 *)dp)[0] = t0[c0];                    \
	    if (n1 > 0) ((mlib_u8 *)dp)[1] = t1[c0];                    \
	    if (n1 > 0) ((mlib_u8 *)dp)[2] = t2[c0];                    \
	    if (n1 > 0) ((mlib_u8 *)dp)[3] = t3[c0];                    \
	    if (n1 > 1) ((mlib_u8 *)dp)[4] = t0[c1];                    \
	    if (n1 > 1) ((mlib_u8 *)dp)[5] = t1[c1];                    \
	    if (n1 > 1) ((mlib_u8 *)dp)[6] = t2[c1];                    \
	    if (n1 > 1) ((mlib_u8 *)dp)[7] = t3[c1];                    \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_U8_4(
    const mlib_s32 *src,
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

	const mlib_u8 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_u8 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_u8 *tab2 = &table[2][(mlib_u32)2147483648];
	const mlib_u8 *tab3 = &table[3][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw <<= 2;

	for (; dh > 0; dh--) {

		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(SI_S32_U8_4L)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1++;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			n1 >>= 2;
			FN_NAME(SI_S32_U8_4E)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S16_1L(dp, sd1, sd2, sd3,       \
	sd4, t0, t1, t2, t3)                                    \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_s16 *)dp)[0] = t0[c0];                       \
	    ((mlib_s16 *)dp)[1] = t1[c1];                       \
	    ((mlib_s16 *)dp)[2] = t2[c2];                       \
	    ((mlib_s16 *)dp)[3] = t3[c3];                       \
	    ((mlib_s16 *)dp)[4] = t0[c4];                       \
	    ((mlib_s16 *)dp)[5] = t1[c5];                       \
	    ((mlib_s16 *)dp)[6] = t2[c6];                       \
	    ((mlib_s16 *)dp)[7] = t3[c7];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S16_1E(dp, sd1, sd2, sd3,       \
	sd4, t0, t1, t2, t3)                                    \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_s16 *)dp)[0] = t0[c0];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[1] = t1[c1];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[2] = t2[c2];           \
	    if (n1 > 3) ((mlib_s16 *)dp)[3] = t3[c3];           \
	    if (n1 > 4) ((mlib_s16 *)dp)[4] = t0[c4];           \
	    if (n1 > 5) ((mlib_s16 *)dp)[5] = t1[c5];           \
	    if (n1 > 6) ((mlib_s16 *)dp)[6] = t2[c6];           \
	    if (n1 > 7) ((mlib_s16 *)dp)[7] = t3[c7];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S16_1(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

	const mlib_s16 *tab0 = &table[0][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_1L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab0, tab0, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_1E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab0, tab0, tab0);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S16_2(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

	const mlib_s16 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s16 *tab1 = &table[1][(mlib_u32)2147483648];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_1L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_1E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab0, tab1);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S16_4(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

	const mlib_s16 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s16 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_s16 *tab2 = &table[2][(mlib_u32)2147483648];
	const mlib_s16 *tab3 = &table[3][(mlib_u32)2147483648];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_1L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_1E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2, tab3);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S16_3L(dp, sd1, sd2, sd3,       \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_s16 *)dp)[0] = t0[c0];                       \
	    ((mlib_s16 *)dp)[1] = t1[c1];                       \
	    ((mlib_s16 *)dp)[2] = t2[c2];                       \
	    ((mlib_s16 *)dp)[3] = t0[c3];                       \
	    ((mlib_s16 *)dp)[4] = t1[c4];                       \
	    ((mlib_s16 *)dp)[5] = t2[c5];                       \
	    ((mlib_s16 *)dp)[6] = t0[c6];                       \
	    ((mlib_s16 *)dp)[7] = t1[c7];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S16_3E(dp, sd1, sd2, sd3,       \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_s16 *)dp)[0] = t0[c0];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[1] = t1[c1];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[2] = t2[c2];           \
	    if (n1 > 3) ((mlib_s16 *)dp)[3] = t0[c3];           \
	    if (n1 > 4) ((mlib_s16 *)dp)[4] = t1[c4];           \
	    if (n1 > 5) ((mlib_s16 *)dp)[5] = t2[c5];           \
	    if (n1 > 6) ((mlib_s16 *)dp)[6] = t0[c6];           \
	    if (n1 > 7) ((mlib_s16 *)dp)[7] = t1[c7];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S16_3(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;
	const mlib_s16 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		tab0 = &table[0][(mlib_u32)2147483648];
		tab1 = &table[1][(mlib_u32)2147483648];
		tab2 = &table[2][(mlib_u32)2147483648];

		for (n1 = dw; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2);
			dp1 += 2;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab2, tab0, tab1);
			dp1 += 2;
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab1, tab2, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2);
			dp1 += 2;
			n1 -= 8;
		}
		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab2, tab0, tab1);
			dp1 += 2;
			n1 -= 8;
		}
		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(S32_S16_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab1, tab2, tab0);
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S16_2L(dp, sd1, sd2, t0,     \
	t1, t2, t3)                                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    ((mlib_s16 *)dp)[0] = t0[c0];                       \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c1];                       \
	    ((mlib_s16 *)dp)[3] = t3[c1];                       \
	    ((mlib_s16 *)dp)[4] = t0[c2];                       \
	    ((mlib_s16 *)dp)[5] = t1[c2];                       \
	    ((mlib_s16 *)dp)[6] = t2[c3];                       \
	    ((mlib_s16 *)dp)[7] = t3[c3];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S16_2E(dp, sd1, sd2, t0,     \
	t1, t2, t3)                                             \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    if (n1 > 0) ((mlib_s16 *)dp)[0] = t0[c0];           \
	    if (n1 > 0) ((mlib_s16 *)dp)[1] = t1[c0];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[2] = t2[c1];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[3] = t3[c1];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[4] = t0[c2];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[5] = t1[c2];           \
	    if (n1 > 3) ((mlib_s16 *)dp)[6] = t2[c3];           \
	    if (n1 > 3) ((mlib_s16 *)dp)[7] = t3[c3];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_S16_2(
    const mlib_s32 *src,
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
	const mlib_s16 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s16 *tab1 = &table[1][(mlib_u32)2147483648];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(SI_S32_S16_2L)(dp1, sd1, sd2, tab0,
			    tab1, tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			n1 >>= 1;
			FN_NAME(SI_S32_S16_2E)(dp1, sd1, sd2, tab0,
			    tab1, tab0, tab1);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S16_4L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1;                                    \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    ((mlib_s16 *)dp)[0] = t0[c0];                       \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c0];                       \
	    ((mlib_s16 *)dp)[3] = t3[c0];                       \
	    ((mlib_s16 *)dp)[4] = t0[c1];                       \
	    ((mlib_s16 *)dp)[5] = t1[c1];                       \
	    ((mlib_s16 *)dp)[6] = t2[c1];                       \
	    ((mlib_s16 *)dp)[7] = t3[c1];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S16_4E(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1;                                    \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    if (n1 > 0) ((mlib_s16 *)dp)[0] = t0[c0];           \
	    if (n1 > 0) ((mlib_s16 *)dp)[1] = t1[c0];           \
	    if (n1 > 0) ((mlib_s16 *)dp)[2] = t2[c0];           \
	    if (n1 > 0) ((mlib_s16 *)dp)[3] = t3[c0];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[4] = t0[c1];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[5] = t1[c1];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[6] = t2[c1];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[7] = t3[c1];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_S16_4(
    const mlib_s32 *src,
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

	const mlib_s16 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s16 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_s16 *tab2 = &table[2][(mlib_u32)2147483648];
	const mlib_s16 *tab3 = &table[3][(mlib_u32)2147483648];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(SI_S32_S16_4L)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = *sp1;
			n1 >>= 2;
			FN_NAME(SI_S32_S16_4E)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S16_3L(dp, sd1, sd2, sd3,    \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_s16 *)dp)[0] = t0[c0];                       \
	    ((mlib_s16 *)dp)[1] = t1[c0];                       \
	    ((mlib_s16 *)dp)[2] = t2[c0];                       \
	    ((mlib_s16 *)dp)[3] = t0[c1];                       \
	    ((mlib_s16 *)dp)[4] = t1[c1];                       \
	    ((mlib_s16 *)dp)[5] = t2[c1];                       \
	    ((mlib_s16 *)dp)[6] = t0[c2];                       \
	    ((mlib_s16 *)dp)[7] = t1[c2];                       \
	    ((mlib_s16 *)dp)[8] = t2[c2];                       \
	    ((mlib_s16 *)dp)[9] = t0[c3];                       \
	    ((mlib_s16 *)dp)[10] = t1[c3];                      \
	    ((mlib_s16 *)dp)[11] = t2[c3];                      \
	    ((mlib_s16 *)dp)[12] = t0[c4];                      \
	    ((mlib_s16 *)dp)[13] = t1[c4];                      \
	    ((mlib_s16 *)dp)[14] = t2[c4];                      \
	    ((mlib_s16 *)dp)[15] = t0[c5];                      \
	    ((mlib_s16 *)dp)[16] = t1[c5];                      \
	    ((mlib_s16 *)dp)[17] = t2[c5];                      \
	    ((mlib_s16 *)dp)[18] = t0[c6];                      \
	    ((mlib_s16 *)dp)[19] = t1[c6];                      \
	    ((mlib_s16 *)dp)[20] = t2[c6];                      \
	    ((mlib_s16 *)dp)[21] = t0[c7];                      \
	    ((mlib_s16 *)dp)[22] = t1[c7];                      \
	    ((mlib_s16 *)dp)[23] = t2[c7];                      \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S16_3E(dp, sd1, sd2, sd3,    \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_s16 *)dp)[0] = t0[c0];           \
	    if (n1 > 0) ((mlib_s16 *)dp)[1] = t1[c0];           \
	    if (n1 > 0) ((mlib_s16 *)dp)[2] = t2[c0];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[3] = t0[c1];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[4] = t1[c1];           \
	    if (n1 > 1) ((mlib_s16 *)dp)[5] = t2[c1];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[6] = t0[c2];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[7] = t1[c2];           \
	    if (n1 > 2) ((mlib_s16 *)dp)[8] = t2[c2];           \
	    if (n1 > 3) ((mlib_s16 *)dp)[9] = t0[c3];           \
	    if (n1 > 3) ((mlib_s16 *)dp)[10] = t1[c3];          \
	    if (n1 > 3) ((mlib_s16 *)dp)[11] = t2[c3];          \
	    if (n1 > 4) ((mlib_s16 *)dp)[12] = t0[c4];          \
	    if (n1 > 4) ((mlib_s16 *)dp)[13] = t1[c4];          \
	    if (n1 > 4) ((mlib_s16 *)dp)[14] = t2[c4];          \
	    if (n1 > 5) ((mlib_s16 *)dp)[15] = t0[c5];          \
	    if (n1 > 5) ((mlib_s16 *)dp)[16] = t1[c5];          \
	    if (n1 > 5) ((mlib_s16 *)dp)[17] = t2[c5];          \
	    if (n1 > 6) ((mlib_s16 *)dp)[18] = t0[c6];          \
	    if (n1 > 6) ((mlib_s16 *)dp)[19] = t1[c6];          \
	    if (n1 > 6) ((mlib_s16 *)dp)[20] = t2[c6];          \
	    if (n1 > 7) ((mlib_s16 *)dp)[21] = t0[c7];          \
	    if (n1 > 7) ((mlib_s16 *)dp)[22] = t1[c7];          \
	    if (n1 > 7) ((mlib_s16 *)dp)[23] = t2[c7];          \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_S16_3(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;
	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_s32 n0;

	const mlib_s16 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s16 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_s16 *tab2 = &table[2][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;

		for (; n1 > 23; n1 -= 24) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			FN_NAME(SI_S32_S16_3L)(dp1, sd1, sd2, sd3,
			    sd4, tab0, tab1, tab2);
			dp1 += 6;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			sd3 = (*sp1++);
			sd4 = (*sp1++);
			n1 = n1 / 3;
			FN_NAME(SI_S32_S16_3E)(dp1, sd1, sd2, sd3,
			    sd4, tab0, tab1, tab2);
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S32_1L(dp, sd1, sd2, t0, t1,    \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c1];                       \
	    ((mlib_s32 *)dp)[2] = t2[c2];                       \
	    ((mlib_s32 *)dp)[3] = t3[c3];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S32_1E(dp, sd1, sd2, t0, t1,    \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3;                            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    if (n1 > 0) ((mlib_s32 *)dp)[0] = t0[c0];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[1] = t1[c1];           \
	    if (n1 > 2) ((mlib_s32 *)dp)[2] = t2[c2];           \
	    if (n1 > 3) ((mlib_s32 *)dp)[3] = t3[c3];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S32_1(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

	const mlib_s32 *tab0 = &table[0][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(S32_S32_1L)(dp1, sd1, sd2, tab0,
			    tab0, tab0, tab0);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(S32_S32_1E)(dp1, sd1, sd2, tab0,
			    tab0, tab0, tab0);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S32_2(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

	const mlib_s32 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s32 *tab1 = &table[1][(mlib_u32)2147483648];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(S32_S32_1L)(dp1, sd1, sd2, tab0,
			    tab1, tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(S32_S32_1E)(dp1, sd1, sd2, tab0,
			    tab1, tab0, tab1);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S32_4(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2;

/* loop variables */
	mlib_s32 n1;

	const mlib_s32 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s32 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_s32 *tab2 = &table[2][(mlib_u32)2147483648];
	const mlib_s32 *tab3 = &table[3][(mlib_u32)2147483648];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			sd2 = (*sp1++);
			FN_NAME(S32_S32_1L)(dp1, sd1, sd2, tab0,
			    tab1, tab2, tab3);
			dp1 += 2;
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S32_3L(dp, sd1, sd2, sd3,       \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c1];                       \
	    ((mlib_s32 *)dp)[2] = t2[c2];                       \
	    ((mlib_s32 *)dp)[3] = t0[c3];                       \
	    ((mlib_s32 *)dp)[4] = t1[c4];                       \
	    ((mlib_s32 *)dp)[5] = t2[c5];                       \
	    ((mlib_s32 *)dp)[6] = t0[c6];                       \
	    ((mlib_s32 *)dp)[7] = t1[c7];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_S32_S32_3E(dp, sd1, sd2, sd3,       \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;            \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_s32 *)dp)[0] = t0[c0];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[1] = t1[c1];           \
	    if (n1 > 2) ((mlib_s32 *)dp)[2] = t2[c2];           \
	    if (n1 > 3) ((mlib_s32 *)dp)[3] = t0[c3];           \
	    if (n1 > 4) ((mlib_s32 *)dp)[4] = t1[c4];           \
	    if (n1 > 5) ((mlib_s32 *)dp)[5] = t2[c5];           \
	    if (n1 > 6) ((mlib_s32 *)dp)[6] = t0[c6];           \
	    if (n1 > 7) ((mlib_s32 *)dp)[7] = t1[c7];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUp_S32_S32_3(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;
/* loop variables */
	mlib_s32 n1;
	const mlib_s32 *tab0, *tab1, *tab2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
	    tab0 = &table[0][(mlib_u32)2147483648];
	    tab1 = &table[1][(mlib_u32)2147483648];
	    tab2 = &table[2][(mlib_u32)2147483648];

	    for (n1 = dw; n1 > 23; n1 -= 24) {
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(S32_S32_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2);
		dp1 += 4;
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(S32_S32_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab2, tab0, tab1);
		dp1 += 4;
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(S32_S32_3L)(dp1, sd1, sd2, sd3, sd4,
			    tab1, tab2, tab0);
		dp1 += 4;
	    }

	    if (n1 > 0) {
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(S32_S32_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab0, tab1, tab2);
		dp1 += 4;
		n1 -= 8;
	    }
	    if (n1 > 0) {
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(S32_S32_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab2, tab0, tab1);
		dp1 += 4;
		n1 -= 8;
	    }
	    if (n1 > 0) {
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(S32_S32_3E)(dp1, sd1, sd2, sd3, sd4,
			    tab1, tab2, tab0);
	    }

	    sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	    dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S32_2L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1;                                    \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c1];                       \
	    ((mlib_s32 *)dp)[3] = t3[c1];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S32_2E(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1;                                    \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    if (n1 > 0) ((mlib_s32 *)dp)[0] = t0[c0];           \
	    if (n1 > 0) ((mlib_s32 *)dp)[1] = t1[c0];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[2] = t2[c1];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[3] = t3[c1];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_S32_2(
    const mlib_s32 *src,
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

	const mlib_s32 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s32 *tab1 = &table[1][(mlib_u32)2147483648];

	dw <<= 1;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S32_S32_2L(dp1, sd1, tab0, tab1,
			    tab0, tab1);
			dp1 += 2;
		}

		if (n1 > 0) {
			n1 >>= 1;
			sd1 = (*sp1++);
			MLIB_M_IMAGE_LOOKUP_SI_S32_S32_2E(dp1, sd1, tab0, tab1,
			    tab0, tab1);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S32_4L(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1;                                    \
	                                                        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c0];                       \
	    ((mlib_s32 *)dp)[3] = t3[c0];                       \
	    ((mlib_s32 *)dp)[4] = t0[c1];                       \
	    ((mlib_s32 *)dp)[5] = t1[c1];                       \
	    ((mlib_s32 *)dp)[6] = t2[c1];                       \
	    ((mlib_s32 *)dp)[7] = t3[c1];                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S32_4E(dp, sd1, t0, t1,      \
	t2, t3)                                                 \
	{                                                       \
	    mlib_s32 c0, c1;                                    \
                                                                \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    if (n1 > 0) ((mlib_s32 *)dp)[0] = t0[c0];           \
	    if (n1 > 0) ((mlib_s32 *)dp)[1] = t1[c0];           \
	    if (n1 > 0) ((mlib_s32 *)dp)[2] = t2[c0];           \
	    if (n1 > 0) ((mlib_s32 *)dp)[3] = t3[c0];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[4] = t0[c1];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[5] = t1[c1];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[6] = t2[c1];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[7] = t3[c1];           \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_S32_4(
    const mlib_s32 *src,
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

	const mlib_s32 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s32 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_s32 *tab2 = &table[2][(mlib_u32)2147483648];
	const mlib_s32 *tab3 = &table[3][(mlib_u32)2147483648];

	dw <<= 2;

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	for (; dh > 0; dh--) {
		for (n1 = dw; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			FN_NAME(SI_S32_S32_4L)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
			dp1 += 4;
		}

		if (n1 > 0) {
			sd1 = (*sp1++);
			n1 >>= 2;
			FN_NAME(SI_S32_S32_4E)(dp1, sd1, tab0, tab1,
			    tab2, tab3);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S32_3L(dp, sd1, sd2, sd3,    \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    ((mlib_s32 *)dp)[0] = t0[c0];                       \
	    ((mlib_s32 *)dp)[1] = t1[c0];                       \
	    ((mlib_s32 *)dp)[2] = t2[c0];                       \
	    ((mlib_s32 *)dp)[3] = t0[c1];                       \
	    ((mlib_s32 *)dp)[4] = t1[c1];                       \
	    ((mlib_s32 *)dp)[5] = t2[c1];                       \
	    ((mlib_s32 *)dp)[6] = t0[c2];                       \
	    ((mlib_s32 *)dp)[7] = t1[c2];                       \
	    ((mlib_s32 *)dp)[8] = t2[c2];                       \
	    ((mlib_s32 *)dp)[9] = t0[c3];                       \
	    ((mlib_s32 *)dp)[10] = t1[c3];                      \
	    ((mlib_s32 *)dp)[11] = t2[c3];                      \
	    ((mlib_s32 *)dp)[12] = t0[c4];                      \
	    ((mlib_s32 *)dp)[13] = t1[c4];                      \
	    ((mlib_s32 *)dp)[14] = t2[c4];                      \
	    ((mlib_s32 *)dp)[15] = t0[c5];                      \
	    ((mlib_s32 *)dp)[16] = t1[c5];                      \
	    ((mlib_s32 *)dp)[17] = t2[c5];                      \
	    ((mlib_s32 *)dp)[18] = t0[c6];                      \
	    ((mlib_s32 *)dp)[19] = t1[c6];                      \
	    ((mlib_s32 *)dp)[20] = t2[c6];                      \
	    ((mlib_s32 *)dp)[21] = t0[c7];                      \
	    ((mlib_s32 *)dp)[22] = t1[c7];                      \
	    ((mlib_s32 *)dp)[23] = t2[c7];                      \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOOKUP_SI_S32_S32_3E(dp, sd1, sd2, sd3,    \
	sd4, t0, t1, t2)                                        \
	{                                                       \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c0, c1, sd1);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c2, c3, sd2);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c4, c5, sd3);        \
	    MLIB_M_IMAGE_LOOKUP_S32_2INDEX(c6, c7, sd4);        \
	    if (n1 > 0) ((mlib_s32 *)dp)[0] = t0[c0];           \
	    if (n1 > 0) ((mlib_s32 *)dp)[1] = t1[c0];           \
	    if (n1 > 0) ((mlib_s32 *)dp)[2] = t2[c0];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[3] = t0[c1];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[4] = t1[c1];           \
	    if (n1 > 1) ((mlib_s32 *)dp)[5] = t2[c1];           \
	    if (n1 > 2) ((mlib_s32 *)dp)[6] = t0[c2];           \
	    if (n1 > 2) ((mlib_s32 *)dp)[7] = t1[c2];           \
	    if (n1 > 2) ((mlib_s32 *)dp)[8] = t2[c2];           \
	    if (n1 > 3) ((mlib_s32 *)dp)[9] = t0[c3];           \
	    if (n1 > 3) ((mlib_s32 *)dp)[10] = t1[c3];          \
	    if (n1 > 3) ((mlib_s32 *)dp)[11] = t2[c3];          \
	    if (n1 > 4) ((mlib_s32 *)dp)[12] = t0[c4];          \
	    if (n1 > 4) ((mlib_s32 *)dp)[13] = t1[c4];          \
	    if (n1 > 4) ((mlib_s32 *)dp)[14] = t2[c4];          \
	    if (n1 > 5) ((mlib_s32 *)dp)[15] = t0[c5];          \
	    if (n1 > 5) ((mlib_s32 *)dp)[16] = t1[c5];          \
	    if (n1 > 5) ((mlib_s32 *)dp)[17] = t2[c5];          \
	    if (n1 > 6) ((mlib_s32 *)dp)[18] = t0[c6];          \
	    if (n1 > 6) ((mlib_s32 *)dp)[19] = t1[c6];          \
	    if (n1 > 6) ((mlib_s32 *)dp)[20] = t2[c6];          \
	    if (n1 > 7) ((mlib_s32 *)dp)[21] = t0[c7];          \
	    if (n1 > 7) ((mlib_s32 *)dp)[22] = t1[c7];          \
	    if (n1 > 7) ((mlib_s32 *)dp)[23] = t2[c7];          \
	}

/* *********************************************************** */

void
mlib_m_ImageLookUpSI_S32_S32_3(
    const mlib_s32 *src,
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
	__m64 *sp1, *sl1, sd1, sd2, sd3, sd4;

/* loop variables */
	mlib_s32 n1;

	mlib_s32 c0, c1, c2, c3, c4, c5, c6, c7;
	mlib_s32 n0;

	const mlib_s32 *tab0 = &table[0][(mlib_u32)2147483648];
	const mlib_s32 *tab1 = &table[1][(mlib_u32)2147483648];
	const mlib_s32 *tab2 = &table[2][(mlib_u32)2147483648];

	dp1 = dl1 = (__m64 *) dst;
	sp1 = sl1 = (__m64 *) src;

	dw *= 3;

	for (; dh > 0; dh--) {
	    n1 = dw;

	    for (; n1 > 23; n1 -= 24) {
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(SI_S32_S32_3L)(dp1, sd1, sd2, sd3,
		    sd4, tab0, tab1, tab2);
		dp1 += 12;
	    }

	    if (n1 > 0) {
		n1 = n1 / 3;
		sd1 = (*sp1++);
		sd2 = (*sp1++);
		sd3 = (*sp1++);
		sd4 = (*sp1++);
		FN_NAME(SI_S32_S32_3E)(dp1, sd1, sd2, sd3,
		    sd4, tab0, tab1, tab2);
	    }

	    sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb);
	    dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb);

	}

	_mm_empty();
}

/* *********************************************************** */
