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

#ifndef _MLIB_C_IMAGELOGIC_H
#define	_MLIB_C_IMAGELOGIC_H

#pragma ident	"@(#)mlib_c_ImageLogic.h	9.3	07/10/09 SMI"

/*
 * Functions for C version image logical functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

/* *********************************************************** */

#if defined(_NO_LONGLONG)

#define	SIZE_IN_BYTES	4
#define	FORMAT	mlib_u32

#else		   /* defined(_NO_LONGLONG) */

#define	SIZE_IN_BYTES	8
#define	FORMAT	mlib_u64

#endif		   /* defined(_NO_LONGLONG) */

/* *********************************************************** */

#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1
#define	STEP	(SIZE_IN_BYTES)

static void mlib_c_ImageLogic_na(
    mlib_u8 *dp,
    FORMAT *dpp,
    FORMAT *spp1,
    FORMAT *spp2,
    mlib_s32 offsrc1,
    mlib_s32 offsrc2,
    mlib_s32 amount);

/* *********************************************************** */

#define	VALIDATE()                                                         \
	mlib_u8 *sp1;                                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	mlib_u8 *sp2;                                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	mlib_u8 *dp;                                                       \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of dst                                      \
 */                                                                        \
	mlib_s32 width, height, channels, type;                            \
	mlib_s32 stride1;                                                  \
	                                                                   \
/*                                                                         \
 * for src1                                                                \
 */                                                                        \
	mlib_s32 stride2;                                                  \
	                                                                   \
/*                                                                         \
 * for src2                                                                \
 */                                                                        \
	mlib_s32 strided;                                                  \
	                                                                   \
/*                                                                         \
 * for dst                                                                 \
 */                                                                        \
	MLIB_IMAGE_SIZE_EQUAL(dst, src1);                                  \
	MLIB_IMAGE_TYPE_EQUAL(dst, src1);                                  \
	MLIB_IMAGE_CHAN_EQUAL(dst, src1);                                  \
	MLIB_IMAGE_SIZE_EQUAL(dst, src2);                                  \
	MLIB_IMAGE_TYPE_EQUAL(dst, src2);                                  \
	MLIB_IMAGE_CHAN_EQUAL(dst, src2);                                  \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                            \
	sp1 = (mlib_u8 *)mlib_ImageGetData(src1);                          \
	sp2 = (mlib_u8 *)mlib_ImageGetData(src2);                          \
	height = mlib_ImageGetHeight(dst);                                 \
	width = mlib_ImageGetWidth(dst);                                   \
	stride1 = mlib_ImageGetStride(src1);                               \
	stride2 = mlib_ImageGetStride(src2);                               \
	strided = mlib_ImageGetStride(dst);                                \
	channels = mlib_ImageGetChannels(dst);                             \
	type = mlib_ImageGetType(dst);                                     \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT))                 \
	    width *= 2;                                                    \
	else if (type == MLIB_INT)                                         \
	    width *= 4;                                                    \
	if ((width * channels) > stride1 ||                                \
	    (width * channels) > stride2 || (width * channels) > strided)  \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	EDGE_U8(p1, p2)                                             \
	left = (mlib_addr)p1 & (MASK);                              \
	right = (MASK);                                             \
	if ((((mlib_addr)p1 ^ (mlib_addr)p2) & (~(MASK))) == 0) {   \
	    right = (mlib_addr)p2 & (MASK);                         \
	}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	STORE_MASK_U8(p, a)                                     \
	a >>= (left * 8);                                       \
	for (k = left; k <= right; k++) {                       \
	    ((mlib_u8 *)(p))[k] = (a);                          \
	    a >>= 8;                                            \
	}

#else /* _LITTLE_ENDIAN */

#define	STORE_MASK_U8(p, a)                                     \
	a >>= (((MASK) - right) * 8);                           \
	for (k = right; k >= left; k--) {                       \
	    ((mlib_u8 *)(p))[k] = (a);                          \
	    a >>= 8;                                            \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	ALIGN()                                                 \
	dpp[0] = OPERATION(spp1[0], spp2[0]);                   \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	ALIGN_d_1()                                                 \
	sa2 = spp2[0];                                              \
	dpp[0] =                                                    \
	OPERATION(spp1[0], ((sa1 >> shift2) + (sa2 << shift2_)));   \
	sa1 = sa2;                                                  \
	dpp++;                                                      \
	spp1++;                                                     \
	spp2++

/* *********************************************************** */

#define	ALIGN_d_2()                                                 \
	sa2 = spp1[0];                                              \
	dpp[0] =                                                    \
	OPERATION(((sa1 >> shift1) + (sa2 << shift1_)), spp2[0]);   \
	sa1 = sa2;                                                  \
	dpp++;                                                      \
	spp1++;                                                     \
	spp2++

/* *********************************************************** */

#define	ALIGN_1_2()                                             \
	sa2 = OPERATION(spp1[0], spp2[0]);                      \
	dpp[0] = (sa1 >> shift1) + (sa2 << shift1_);            \
	sa1 = sa2;                                              \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

/* *********************************************************** */

#define	NON_ALIGN()                                             \
	sa2 = spp1[0];                                          \
	sa4 = spp2[0];                                          \
	dpp[0] =                                                \
	OPERATION(((sa1 >> shift1) + (sa2 << shift1_)),         \
	    ((sa3 >> shift2) + (sa4 << shift2_)));              \
	sa1 = sa2;                                              \
	sa3 = sa4;                                              \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	ALIGN_d_1()                                                 \
	sa2 = spp2[0];                                              \
	dpp[0] =                                                    \
	OPERATION(spp1[0], ((sa1 << shift2) + (sa2 >> shift2_)));   \
	sa1 = sa2;                                                  \
	dpp++;                                                      \
	spp1++;                                                     \
	spp2++

/* *********************************************************** */

#define	ALIGN_d_2()                                                 \
	sa2 = spp1[0];                                              \
	dpp[0] =                                                    \
	OPERATION(((sa1 << shift1) + (sa2 >> shift1_)), spp2[0]);   \
	sa1 = sa2;                                                  \
	dpp++;                                                      \
	spp1++;                                                     \
	spp2++

/* *********************************************************** */

#define	ALIGN_1_2()                                             \
	sa2 = OPERATION(spp1[0], spp2[0]);                      \
	dpp[0] = (sa1 << shift1) + (sa2 >> shift1_);            \
	sa1 = sa2;                                              \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

/* *********************************************************** */

#define	NON_ALIGN()                                             \
	sa2 = *spp1;                                            \
	sa4 = *spp2;                                            \
	*dpp =                                                  \
	OPERATION(((sa1 << shift1) + (sa2 >> shift1_)),         \
	    ((sa3 << shift2) + (sa4 >> shift2_)));              \
	sa1 = sa2;                                              \
	sa3 = sa4;                                              \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static mlib_status
mlib_c_ImageLogic(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k, left, right;
	mlib_s32 offdst, offsrc1, offsrc2;
	mlib_u8 *dend;
	FORMAT *dpp, *spp2, *spp1;
	FORMAT sa1 = 0, sa2 = 0, da;
	mlib_s32 shift1, shift2, shift1_, shift2_;
	mlib_s32 amount;

	VALIDATE();

	if (!mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		width *= height;
		height = 1;
	}

	amount = width * channels;

	for (j = 0; j < height; j++) {

		dend = dp + amount - 1;
		offdst = ((mlib_addr)dp) & (MASK);
		dpp = (FORMAT *) (dp - offdst);
		spp1 = (FORMAT *) (((mlib_addr)(sp1 - offdst)) & (~(MASK)));
		offsrc1 = (mlib_u8 *)(sp1 - offdst) - (mlib_u8 *)spp1;
		spp2 = (FORMAT *) (((mlib_addr)(sp2 - offdst)) & (~(MASK)));
		offsrc2 = (mlib_u8 *)(sp2 - offdst) - (mlib_u8 *)spp2;

		if ((offsrc1 == 0) && (offsrc2 == 0)) {
/* offset_dst = offset_src1 = offset_src2 */

			i = (mlib_u8 *)dpp - dp;

			if (i != 0) {
				EDGE_U8(dp, dend);
				da = OPERATION(((*spp1++)), ((*spp2++)));
				STORE_MASK_U8(dpp, da);
				i += (SIZE_IN_BYTES);
				dpp++;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - (STEP); i += (STEP)) {
				ALIGN();
			}

			if (i < amount) {
				EDGE_U8(dpp, dend);
				da = OPERATION((*spp1), (*spp2));
				STORE_MASK_U8(dpp, da);
			}
		} else if (offsrc1 == 0) {
/* offset_dst = offset_src1 != offset_src2 */

			i = (mlib_u8 *)dpp - dp;
			shift2 = offsrc2 * 8;
			shift2_ = (SIZE_IN_BITS) - shift2;


			if ((mlib_addr)spp2 == ((mlib_addr) sp2 & (~(MASK))))
				sa1 = *spp2;
			spp2++;

			if (i != 0) {
				if ((offsrc2 - i + amount) > (SIZE_IN_BYTES))
					sa2 = *spp2;
				spp2++;
				EDGE_U8(dp, dend);
#ifdef _LITTLE_ENDIAN
				da = OPERATION(((*spp1++)),
				    ((sa1 >> shift2) + (sa2 << shift2_)));
#else /* _LITTLE_ENDIAN */
				da = OPERATION(((*spp1++)),
				    ((sa1 << shift2) + (sa2 >> shift2_)));
#endif /* _LITTLE_ENDIAN */
				STORE_MASK_U8(dpp, da);
				i += (SIZE_IN_BYTES);
				dpp++;
				sa1 = sa2;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - (STEP); i += (STEP)) {
				ALIGN_d_1();
			}

			if (i < amount) {
				if ((mlib_u8 *)spp2 < (sp2 + amount))
					sa2 = *spp2;
				EDGE_U8(dpp, dend);
#ifdef _LITTLE_ENDIAN
				da = OPERATION((*spp1),
				    ((sa1 >> shift2) + (sa2 << shift2_)));
#else /* _LITTLE_ENDIAN */
				da = OPERATION((*spp1),
				    ((sa1 << shift2) + (sa2 >> shift2_)));
#endif /* _LITTLE_ENDIAN */
				STORE_MASK_U8(dpp, da);
			}
		} else if (offsrc2 == 0) {
/* offset_dst = offset_src2 != offset_src1 */

			i = (mlib_u8 *)dpp - dp;
			shift1 = offsrc1 * 8;
			shift1_ = (SIZE_IN_BITS) - shift1;

			if ((mlib_addr)spp1 == ((mlib_addr) sp1 & (~(MASK))))
				sa1 = *spp1;
			spp1++;

			if (i != 0) {
				if ((offsrc1 - i + amount) > (SIZE_IN_BYTES))
					sa2 = *spp1;
				spp1++;
				EDGE_U8(dp, dend);
#ifdef _LITTLE_ENDIAN
				da = OPERATION(((sa1 >> shift1) +
				    (sa2 << shift1_)), ((*spp2++)));
#else /* _LITTLE_ENDIAN */
				da = OPERATION(((sa1 << shift1) +
				    (sa2 >> shift1_)), ((*spp2++)));
#endif /* _LITTLE_ENDIAN */
				STORE_MASK_U8(dpp, da);
				i += (SIZE_IN_BYTES);
				dpp++;
				sa1 = sa2;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - (STEP); i += (STEP)) {
				ALIGN_d_2();
			}

			if (i < amount) {
				if ((mlib_u8 *)spp1 < (sp1 + amount))
					sa2 = *spp1;
				EDGE_U8(dpp, dend);
#ifdef _LITTLE_ENDIAN
				da = OPERATION(((sa1 >> shift1) +
				    (sa2 << shift1_)), (*spp2));
#else /* _LITTLE_ENDIAN */
				da = OPERATION(((sa1 << shift1) +
				    (sa2 >> shift1_)), (*spp2));
#endif /* _LITTLE_ENDIAN */
				STORE_MASK_U8(dpp, da);
			}
		} else if (offsrc1 == offsrc2) {
/* offset_src1 = offset_src2 != offset_dst */

			i = (mlib_u8 *)dpp - dp;
			shift1 = offsrc1 * 8;
			shift1_ = (SIZE_IN_BITS) - shift1;

			if ((mlib_addr)spp1 == ((mlib_addr) sp1 & (~(MASK))))
				sa1 = OPERATION((*spp1), (*spp2));
			spp1++;
			spp2++;

			if (i != 0) {
				if ((offsrc1 - i + amount) > (SIZE_IN_BYTES))
					sa2 = OPERATION((*spp1), (*spp2));
				spp1++;
				spp2++;
				EDGE_U8(dp, dend);
#ifdef _LITTLE_ENDIAN
				da = (sa1 >> shift1) + (sa2 << shift1_);
#else /* _LITTLE_ENDIAN */
				da = (sa1 << shift1) + (sa2 >> shift1_);
#endif /* _LITTLE_ENDIAN */
				STORE_MASK_U8(dpp, da);
				i += (SIZE_IN_BYTES);
				dpp++;
				sa1 = sa2;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - (STEP); i += (STEP)) {
				ALIGN_1_2();
			}

			if (i < amount) {
				if ((mlib_u8 *)spp1 < (sp1 + amount))
					sa2 = OPERATION((*spp1), (*spp2));
				EDGE_U8(dpp, dend);
#ifdef _LITTLE_ENDIAN
				da = (sa1 >> shift1) + (sa2 << shift1_);
#else /* _LITTLE_ENDIAN */
				da = (sa1 << shift1) + (sa2 >> shift1_);
#endif /* _LITTLE_ENDIAN */
				STORE_MASK_U8(dpp, da);
			}
		} else {
/* offset_dst != offset_src2 != offset_src1 */

			mlib_c_ImageLogic_na(dp, dpp, spp1, spp2, offsrc1,
			    offsrc2, amount);
		}

		sp1 += stride1;
		sp2 += stride2;
		dp += strided;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_c_ImageLogic_na(
    mlib_u8 *dp,
    FORMAT *dpp,
    FORMAT *spp1,
    FORMAT *spp2,
    mlib_s32 offsrc1,
    mlib_s32 offsrc2,
    mlib_s32 amount)
{
	mlib_s32 i, k, left, right;
	mlib_u8 *dend;
	FORMAT sa1 = 0, sa2 = 0, sa3 = 0, sa4 = 0, da;
	mlib_s32 shift1, shift1_, shift2, shift2_;

	dend = dp + amount - 1;
	i = (mlib_u8 *)dpp - dp;
	shift1 = offsrc1 * 8;
	shift1_ = (SIZE_IN_BITS) - shift1;
	shift2 = offsrc2 * 8;
	shift2_ = (SIZE_IN_BITS) - shift2;

	if ((offsrc1 - i) < (SIZE_IN_BYTES))
		sa1 = *spp1;
	if ((offsrc2 - i) < (SIZE_IN_BYTES))
		sa3 = *spp2;
	spp1++;
	spp2++;

	if (i != 0) {
		if ((offsrc1 - i + amount) > (SIZE_IN_BYTES))
			sa2 = *spp1;
		if ((offsrc2 - i + amount) > (SIZE_IN_BYTES))
			sa4 = *spp2;
		spp1++;
		spp2++;
		EDGE_U8(dp, dend);
#ifdef _LITTLE_ENDIAN
		da = OPERATION(((sa1 >> shift1) + (sa2 << shift1_)),
		    ((sa3 >> shift2) + (sa4 << shift2_)));
#else /* _LITTLE_ENDIAN */
		da = OPERATION(((sa1 << shift1) + (sa2 >> shift1_)),
		    ((sa3 << shift2) + (sa4 >> shift2_)));
#endif /* _LITTLE_ENDIAN */
		STORE_MASK_U8(dpp, da);
		i += (SIZE_IN_BYTES);
		dpp++;
		sa1 = sa2;
		sa3 = sa4;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= amount - (STEP); i += (STEP)) {
		NON_ALIGN();
	}

	if (i < amount) {
		if (amount - i > (shift1_ >> 3))
			sa2 = *spp1;
		if (amount - i > (shift2_ >> 3))
			sa4 = *spp2;
		EDGE_U8(dpp, dend);
#ifdef _LITTLE_ENDIAN
		da = OPERATION(((sa1 >> shift1) + (sa2 << shift1_)),
		    ((sa3 >> shift2) + (sa4 << shift2_)));
#else /* _LITTLE_ENDIAN */
		da = OPERATION(((sa1 << shift1) + (sa2 >> shift1_)),
		    ((sa3 << shift2) + (sa4 >> shift2_)));
#endif /* _LITTLE_ENDIAN */
		STORE_MASK_U8(dpp, da);
	}
}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGELOGIC_H */
