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

#ifndef _MLIB_IMAGELOGIC_BIT_H
#define	_MLIB_IMAGELOGIC_BIT_H

#pragma ident	"@(#)mlib_ImageLogic_Bit.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SIZE_IN_BYTES	1
#define	FORMAT	mlib_u8

#else		   /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG)

#define	SIZE_IN_BYTES	4
#define	FORMAT	mlib_u32

#else		   /* defined(_NO_LONGLONG) */

#define	SIZE_IN_BYTES	8
#define	FORMAT	mlib_u64

#endif		   /* defined(_NO_LONGLONG) */

#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1
#define	STEP	(SIZE_IN_BITS)

/* *********************************************************** */

#define	VALIDATE()                                                \
	mlib_u8 *sp1;                                             \
	                                                          \
/*                                                                \
 * pointers for pixel and line of source                          \
 */                                                               \
	mlib_u8 *sp2;                                             \
	                                                          \
/*                                                                \
 * pointers for pixel and line of source                          \
 */                                                               \
	mlib_u8 *dp;                                              \
	                                                          \
/*                                                                \
 * pointers for pixel and line of dst                             \
 */                                                               \
	mlib_s32 width, height, channels;                         \
	mlib_s32 stride1[1];                                      \
	                                                          \
/*                                                                \
 * for src1                                                       \
 */                                                               \
	mlib_s32 stride2[1];                                      \
	                                                          \
/*                                                                \
 * for src2                                                       \
 */                                                               \
	mlib_s32 strided[1];                                      \
	                                                          \
/*                                                                \
 * for dst                                                        \
 */                                                               \
	mlib_s32 bitoffsetd[1];                                   \
	                                                          \
/*                                                                \
 * for dst                                                        \
 */                                                               \
	mlib_s32 bitoffset1[1];                                   \
	                                                          \
/*                                                                \
 * for src1                                                       \
 */                                                               \
	mlib_s32 bitoffset2[1];                                   \
	                                                          \
/*                                                                \
 * for src2                                                       \
 */                                                               \
	MLIB_IMAGE_SIZE_EQUAL(dst, src1);                         \
	MLIB_IMAGE_TYPE_EQUAL(dst, src1);                         \
	MLIB_IMAGE_CHAN_EQUAL(dst, src1);                         \
	MLIB_IMAGE_SIZE_EQUAL(dst, src2);                         \
	MLIB_IMAGE_TYPE_EQUAL(dst, src2);                         \
	MLIB_IMAGE_CHAN_EQUAL(dst, src2);                         \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                   \
	sp1 = (mlib_u8 *)mlib_ImageGetData(src1);                 \
	sp2 = (mlib_u8 *)mlib_ImageGetData(src2);                 \
	height = mlib_ImageGetHeight(dst);                        \
	width = mlib_ImageGetWidth(dst);                          \
	stride1[0] = mlib_ImageGetStride(src1);                   \
	stride2[0] = mlib_ImageGetStride(src2);                   \
	strided[0] = mlib_ImageGetStride(dst);                    \
	channels = mlib_ImageGetChannels(dst);                    \
	bitoffset1[0] = mlib_ImageGetBitOffset(src1);             \
	bitoffset2[0] = mlib_ImageGetBitOffset(src2);             \
	bitoffsetd[0] = mlib_ImageGetBitOffset(dst);              \
	width *= channels;                                        \
	if (width > stride1[0] * 8 || width > stride2[0] * 8 ||   \
	    width > strided[0] * 8)                               \
	    return MLIB_FAILURE

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	EDGE_BIT(mask, p1, off_p1, p2, off_p2)                          \
	{                                                               \
	    mlib_s32 shift;                                             \
	                                                                \
	    mask = 0xFF;                                                \
	    shift = ((mlib_addr)p1 & (MASK)) * 8 + off_p1;              \
	    mask >>= shift;                                             \
	    if ((((mlib_addr)p1 ^ (mlib_addr)p2) & (~(MASK))) == 0) {   \
		mlib_s32 mask1 = 0xFF;                                  \
	                                                                \
		shift = ((mlib_addr)p2 & (MASK)) * 8 + off_p2;          \
		shift = SIZE_IN_BITS - 1 - shift;                       \
		mask1 <<= shift;                                        \
		mask &= mask1;                                          \
	    }                                                           \
	}

#else		   /* _LITTLE_ENDIAN */

#define	EDGE_BIT(mask, p1, off_p1, p2, off_p2)                          \
	{                                                               \
	    mask = ~((FORMAT) 0);                                       \
	    left = (mlib_addr)p1 & (MASK);                              \
	    right = (MASK);                                             \
	    mask >>= (left * 8 + off_p1);                               \
	    if ((((mlib_addr)p1 ^ (mlib_addr)p2) & (~(MASK))) == 0) {   \
		FORMAT mask1 = ~((FORMAT) 0);                           \
	                                                                \
		right = (mlib_addr)p2 & (MASK);                         \
		mask1 <<= (SIZE_IN_BITS - 1 - (right * 8 + off_p2));    \
		mask &= mask1;                                          \
	    }                                                           \
	}
#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	STORE_MASK(mask, p, a)	*p = (((*p) & (~mask)) | (a & mask))
#else		   /* _LITTLE_ENDIAN */
#define	STORE_MASK(mask, p, a)                                  \
	a = (((*p) & (~mask)) | (a & mask));                    \
	a >>= (((MASK) - right) * 8);                           \
	for (k = right; k >= left; k--) {                       \
	    ((mlib_u8 *)(p))[k] = (a);                          \
	    a >>= 8;                                            \
	}
#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	ALIGN()                                                 \
	dpp[0] = OPERATION(spp1[0], spp2[0]);                   \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

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

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	ALIGN_1_2()                                             \
	sa2 = OPERATION(spp1[0], spp2[0]);                      \
	sa2 &= 0xFF;                                            \
	dpp[0] = (sa1 << shift1) + (sa2 >> shift1_);            \
	sa1 = sa2;                                              \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++;

#else		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	ALIGN_1_2()                                             \
	sa2 = OPERATION(spp1[0], spp2[0]);                      \
	dpp[0] = (sa1 << shift1) + (sa2 >> shift1_);            \
	sa1 = sa2;                                              \
	dpp++;                                                  \
	spp1++;                                                 \
	spp2++

#endif		   /* _LITTLE_ENDIAN */

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

/* *********************************************************** */

    LOGIC_BITS
{
	mlib_s32 i, j;
	mlib_s32 offdst, offsrc1, offsrc2;
	mlib_u8 *dend;
	FORMAT *dpp, *spp2, *spp1;
	mlib_s32 shift1, shift2, shift1_, shift2_;
	mlib_s32 amount, dend_offset[1];
#ifndef _LITTLE_ENDIAN
	FORMAT sa1, sa2 = 0, sa3, sa4, da, emask;
	mlib_s32 k, left, right;
#endif		   /* _LITTLE_ENDIAN */

	VALIDATE();

	if ((width == stride1[0] * 8) &&
	    (width == stride2[0] * 8) &&
	    (width == strided[0] * 8)) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	dend_offset[0] = (bitoffsetd[0] + amount - 1) & 7;

	for (j = 0; j < height; j++) {

		dend = dp + (bitoffsetd[0] + amount - 1) / 8;
		offdst = ((mlib_addr)dp) & (MASK);
		dpp = (FORMAT *) (dp - offdst);
		offdst = offdst * 8 + bitoffsetd[0];
		spp1 =
		    (FORMAT *) (((mlib_addr)(sp1 - (offdst - bitoffset1[0] +
		    7) / 8)) & (~(MASK)));
		offsrc1 =
		    ((mlib_u8 *)sp1 - (mlib_u8 *)spp1) * 8 + bitoffset1[0] -
		    offdst;
		spp2 =
		    (FORMAT *) (((mlib_addr)(sp2 - (offdst - bitoffset2[0] +
		    7) / 8)) & (~(MASK)));
		offsrc2 =
		    ((mlib_u8 *)sp2 - (mlib_u8 *)spp2) * 8 + bitoffset2[0] -
		    offdst;

		if ((offsrc1 == 0) && (offsrc2 == 0)) {
/* offset_dst = offset_src1 = offset_src2 */
#ifdef _LITTLE_ENDIAN
			FORMAT da, emask;
#endif /* _LITTLE_ENDIAN */

			i = -offdst;

			if (i != 0) {
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = OPERATION(((*spp1++)), ((*spp2++)));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
				dpp++;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - (STEP); i += (STEP)) {
				ALIGN();
			}

			if (i < amount) {
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION((*spp1), (*spp2));
				STORE_MASK(emask, dpp, da);
			}
		} else if (offsrc1 == 0) {
/* offset_dst = offset_src1 != offset_src2 */
#ifdef _LITTLE_ENDIAN
			mlib_s32 sa1, sa2 = 0, da, emask;
#endif /* _LITTLE_ENDIAN */

			i = -offdst;
			shift2 = offsrc2;
			shift2_ = (SIZE_IN_BITS) - shift2;

			if ((mlib_addr)spp2 == ((mlib_addr) sp2 & (~(MASK))))
				sa1 = *spp2;
			spp2++;

			if (i != 0) {
				if ((offsrc2 - i + amount) > (SIZE_IN_BITS))
					sa2 = *spp2;
				spp2++;
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = OPERATION(((*spp1++)),
				    ((sa1 << shift2) + (sa2 >> shift2_)));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
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
				if (amount - i > shift2_)
					sa2 = *spp2;
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION((*spp1),
				    ((sa1 << shift2) + (sa2 >> shift2_)));
				STORE_MASK(emask, dpp, da);
			}
		} else if (offsrc2 == 0) {
/* offset_dst = offset_src2 != offset_src1 */
#ifdef _LITTLE_ENDIAN
			mlib_s32 sa1, sa2 = 0, da, emask;
#endif /* _LITTLE_ENDIAN */

			i = -offdst;
			shift1 = offsrc1;
			shift1_ = (SIZE_IN_BITS) - shift1;

			if ((mlib_addr)spp1 == ((mlib_addr) sp1 & (~(MASK))))
				sa1 = *spp1;
			spp1++;

			if (i != 0) {
				if ((offsrc1 - i + amount) > (SIZE_IN_BITS))
					sa2 = *spp1;
				spp1++;
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = OPERATION(((sa1 << shift1) +
				    (sa2 >> shift1_)), ((*spp2++)));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
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
				if (amount - i > shift1_)
					sa2 = *spp1;
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION(((sa1 << shift1) +
				    (sa2 >> shift1_)), (*spp2));
				STORE_MASK(emask, dpp, da);
			}
		} else if (offsrc1 == offsrc2) {
/* offset_src1 = offset_src2 != offset_dst */
#ifdef _LITTLE_ENDIAN
			mlib_s32 sa1, sa2 = 0, da, emask;
#endif /* _LITTLE_ENDIAN */

			i = -offdst;
			shift1 = offsrc1;
			shift1_ = (SIZE_IN_BITS) - shift1;

			if ((mlib_addr)spp1 == ((mlib_addr) sp1 & (~(MASK)))) {
				sa1 = OPERATION((*spp1), (*spp2));
#ifdef _LITTLE_ENDIAN
				sa1 &= 0xFF;
#endif /* _LITTLE_ENDIAN */
			}
			spp1++;
			spp2++;

			if (i != 0) {
				if ((offsrc1 - i + amount) > (SIZE_IN_BITS)) {
					sa2 = OPERATION((*spp1), (*spp2));
#ifdef _LITTLE_ENDIAN
					sa2 &= 0xFF;
#endif /* _LITTLE_ENDIAN */
				}
				spp1++;
				spp2++;
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = (sa1 << shift1) + (sa2 >> shift1_);
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
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
				if (amount - i > shift1_) {
					sa2 = OPERATION((*spp1), (*spp2));
#ifdef _LITTLE_ENDIAN
					sa2 &= 0xFF;
#endif /* _LITTLE_ENDIAN */
				}
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = (sa1 << shift1) + (sa2 >> shift1_);
				STORE_MASK(emask, dpp, da);
			}
		} else {
/* offset_dst != offset_src2 != offset_src1 */
#ifdef _LITTLE_ENDIAN
			mlib_s32 sa1, sa2 = 0, sa3, sa4 = 0, da, emask;
#endif /* _LITTLE_ENDIAN */

			i = -offdst;
			shift1 = offsrc1;
			shift1_ = (SIZE_IN_BITS) - shift1;
			shift2 = offsrc2;
			shift2_ = (SIZE_IN_BITS) - shift2;

			if ((offsrc1 - i) < (SIZE_IN_BITS))
				sa1 = *spp1;
			if ((offsrc2 - i) < (SIZE_IN_BITS))
				sa3 = *spp2;
			spp1++;
			spp2++;

			if (i != 0) {
				if ((offsrc1 - i + amount) > (SIZE_IN_BITS))
					sa2 = *spp1;
				if ((offsrc2 - i + amount) > (SIZE_IN_BITS))
					sa4 = *spp2;
				spp1++;
				spp2++;
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = OPERATION(((sa1 << shift1) +
				    (sa2 >> shift1_)),
				    ((sa3 << shift2) + (sa4 >> shift2_)));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
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
				if (amount - i > shift1_)
					sa2 = *spp1;
				if (amount - i > shift2_)
					sa4 = *spp2;
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION(((sa1 << shift1) +
				    (sa2 >> shift1_)),
				    ((sa3 << shift2) + (sa4 >> shift2_)));
				STORE_MASK(emask, dpp, da);
			}
		}

		sp1 += stride1[0];
		sp2 += stride2[0];
		dp += strided[0];
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGELOGIC_BIT_H */
