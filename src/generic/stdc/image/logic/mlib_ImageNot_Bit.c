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

#pragma ident	"@(#)mlib_ImageNot_Bit.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot   - not an bit type image (C version)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNot(mlib_image       *dst,
 *                                const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:  dst = ~src
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SIZE_IN_BYTES	1
#define	FORMAT	mlib_u8

#else /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG)

#define	SIZE_IN_BYTES	4
#define	FORMAT	mlib_u32

#else /* defined(_NO_LONGLONG) */

#define	SIZE_IN_BYTES	8
#define	FORMAT	mlib_u64

#endif /* defined(_NO_LONGLONG) */

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1

#define	STEP	(SIZE_IN_BITS)

/* *********************************************************** */

#define	VALIDATE()                                              \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	mlib_u8 *sp1;                                           \
/*                                                              \
 * pointers for pixel and line of dst                           \
 */                                                             \
	mlib_u8 *dp;                                            \
	                                                        \
	mlib_s32 width, height, channels;                       \
/*                                                              \
 * for src                                                      \
 */                                                             \
	mlib_s32 stride1;                                       \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	mlib_s32 strided;                                       \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	mlib_s32 bitoffsetd[1];                                 \
/*                                                              \
 * for src                                                      \
 */                                                             \
	mlib_s32 bitoffset[1];                                  \
	                                                        \
	MLIB_IMAGE_SIZE_EQUAL(dst, src);                        \
	MLIB_IMAGE_TYPE_EQUAL(dst, src);                        \
	MLIB_IMAGE_CHAN_EQUAL(dst, src);                        \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                 \
	sp1 = (mlib_u8 *)mlib_ImageGetData(src);                \
	height = mlib_ImageGetHeight(dst);                      \
	width = mlib_ImageGetWidth(dst);                        \
	stride1 = mlib_ImageGetStride(src);                     \
	strided = mlib_ImageGetStride(dst);                     \
	channels = mlib_ImageGetChannels(dst);                  \
	bitoffset[0] = mlib_ImageGetBitOffset(src);             \
	bitoffsetd[0] = mlib_ImageGetBitOffset(dst);            \
	width *= channels;                                      \
	if ((width > stride1 * 8) || (width > strided * 8))     \
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

#else /* _LITTLE_ENDIAN */

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

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	STORE_MASK(mask, p, a)	*p = (((*p) & (~mask)) | (a & mask))

#else /* _LITTLE_ENDIAN */

#define	STORE_MASK(mask, p, a)                                  \
	a = (((*p) & (~mask)) | (a & mask));                    \
	a >>= (((MASK) - right) * 8);                           \
	for (k = right; k >= left; k--) {                       \
	    ((mlib_u8 *)(p))[k] = (a);                          \
	    a >>= 8;                                            \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	OPERATION(a)	~(a)

/* *********************************************************** */

#define	ALIGN()                                                 \
	dpp[0] = OPERATION(spp1[0]);                            \
	dpp[1] = OPERATION(spp1[1]);                            \
	dpp += 2;                                               \
	spp1 += 2

/* *********************************************************** */

#define	NON_ALIGN()                                               \
	sa2 = spp1[0];                                            \
	dpp[0] = OPERATION((sa1 << shift1) + (sa2 >> shift1_));   \
	sa1 = sa2;                                                \
	dpp++;                                                    \
	spp1++

/* *********************************************************** */

mlib_status
mlib_ImageNot_Bit(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	mlib_s32 offdst, offsrc;
	mlib_u8 *dend;
	FORMAT *dpp, *spp1;
	mlib_s32 shift1, shift1_;
	mlib_s32 amount, dend_offset[1];

#ifndef _LITTLE_ENDIAN
	FORMAT sa1, sa2 = 0, da, emask;
	mlib_s32 k, left, right;
#endif /* _LITTLE_ENDIAN */

	VALIDATE();

	if ((width == stride1 * 8) && (width == strided * 8)) {
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
		    (FORMAT *) (((mlib_addr)(sp1 - (offdst - bitoffset[0] +
		    7) / 8)) & (~(MASK)));
		offsrc =
		    ((mlib_u8 *)sp1 - (mlib_u8 *)spp1) * 8 + bitoffset[0] -
		    offdst;

/* offset_dst = offset_src */
		if (offsrc == 0) {
#ifdef _LITTLE_ENDIAN
			FORMAT da, emask;
#endif /* _LITTLE_ENDIAN */

			i = -offdst;

			if (i != 0) {
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = OPERATION(((*spp1++)));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
				dpp++;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= amount - 2 * (STEP); i += 2 * (STEP)) {
				ALIGN();
			}

			if (i < amount) {
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION((*spp1));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
				dpp++;
				spp1++;
			}

			if (i < amount) {
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION((*spp1));
				STORE_MASK(emask, dpp, da);
			}
		} else {
/* offset_dst != offset_src */
#ifdef _LITTLE_ENDIAN
			mlib_s32 sa1, sa2 = 0, da, emask;
#endif /* _LITTLE_ENDIAN */
			i = -offdst;
			shift1 = offsrc;
			shift1_ = (SIZE_IN_BITS) - shift1;

			if ((mlib_addr)spp1 == ((mlib_addr) sp1 & (~(MASK))))
				sa1 = *spp1;
			spp1++;

			if (i != 0) {
				if ((offsrc - i + amount) > (SIZE_IN_BITS))
					sa2 = *spp1;
				spp1++;
				EDGE_BIT(emask, dp, bitoffsetd[0], dend,
				    dend_offset[0]);
				da = OPERATION((sa1 << shift1) +
				    (sa2 >> shift1_));
				STORE_MASK(emask, dpp, da);
				i += (SIZE_IN_BITS);
				dpp++;
				sa1 = sa2;
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
				EDGE_BIT(emask, dpp, 0, dend, dend_offset[0]);
				da = OPERATION((sa1 << shift1) +
				    (sa2 >> shift1_));
				STORE_MASK(emask, dpp, da);
			}
		}

		sp1 += stride1;
		dp += strided;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
