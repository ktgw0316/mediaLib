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

#ifndef _MLIB_IMAGECONSTLOGIC_BIT_H
#define	_MLIB_IMAGECONSTLOGIC_BIT_H

#pragma ident	"@(#)mlib_ImageConstLogic_Bit.h	9.2	07/10/09 SMI"

/*
 * Macro definitions for bit image logical functions.
 */

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

#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1
#define	STEP	2*(SIZE_IN_BITS)
#define	STEP_3CH	3*(SIZE_IN_BITS)

#define	SWAP(c1, c2) {                              \
	FORMAT tmp = c2; c2 = c1; c1 = tmp;         \
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	PREPARE_CONST()                                                  \
	width = width * nchannels;                                       \
	if (nchannels == 1) {                                            \
	    c01 = (((mlib_s32)(c[0] & 1)) << 31) >> 31;                  \
	} else if (nchannels == 2) {                                     \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	} else if (nchannels == 3) {                                     \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c01 = (c01 << 2) | (c01 >> 4);                               \
	    c02 = ((c01 << 2) | (c01 >> 4)) & 0xFF;                      \
	    c03 = ((c01 << 1) | (c01 >> 5)) & 0xFF;                      \
	} else {                                                         \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	}

#else		   /* _LITTLE_ENDIAN */

#if defined(_NO_LONGLONG)

#define	PREPARE_CONST()                                                  \
	width = width * nchannels;                                       \
	if (nchannels == 1) {                                            \
	    c01 = (((mlib_s32)(c[0] & 1)) << 31) >> 31;                  \
	} else if (nchannels == 2) {                                     \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	} else if (nchannels == 3) {                                     \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c02 = (c01 << 6) | c01;                                      \
	    c02 |= (c02 << 12);                                          \
	    c01 = (c02 << 6) | c01;                                      \
	    c02 = (c01 << 4) | (c01 >> 26);                              \
	    c03 = (c01 << 3) | (c01 >> 27);                              \
	    c01 = (c01 << 2) | (c01 >> 28);                              \
	} else {                                                         \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	}

#else		   /* defined(_NO_LONGLONG) */

#define	PREPARE_CONST()                                                  \
	width = width * nchannels;                                       \
	if (nchannels == 1) {                                            \
	    c01 = (((mlib_s64)(c[0] & 1)) << 63) >> 63;                  \
	} else if (nchannels == 2) {                                     \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	    c01 |= (c01 << 32);                                          \
	} else if (nchannels == 3) {                                     \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c01 = (c01 << 6) | c01;                                      \
	    c02 = (c01 << 12) | c01;                                     \
	    c02 |= (c02 << 24);                                          \
	    c01 = (c02 << 12) | c01;                                     \
	    c02 = (c01 << 5) | (c01 >> 55);                              \
	    c03 = (c01 << 6) | (c01 >> 54);                              \
	    c01 = (c01 << 4) | (c01 >> 56);                              \
	} else {                                                         \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	    c01 |= (c01 << 32);                                          \
	}

#endif		   /* defined(_NO_LONGLONG) */

#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	VALIDATE()                                              \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	mlib_u8 *sp;                                            \
/*                                                              \
 * pointers for pixel and line of dst                           \
 */                                                             \
	mlib_u8 *dp;                                            \
	                                                        \
	mlib_s32 width, height, nchannels;                      \
/*                                                              \
 * for src                                                      \
 */                                                             \
	mlib_s32 stride;                                        \
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
	FORMAT c01 = 0, c02 = 0, c03 = 0;                       \
	                                                        \
	MLIB_IMAGE_SIZE_EQUAL(dst, src);                        \
	MLIB_IMAGE_TYPE_EQUAL(dst, src);                        \
	MLIB_IMAGE_CHAN_EQUAL(dst, src);                        \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                 \
	sp = (mlib_u8 *)mlib_ImageGetData(src);                 \
	height = mlib_ImageGetHeight(dst);                      \
	width = mlib_ImageGetWidth(dst);                        \
	stride = mlib_ImageGetStride(src);                      \
	strided = mlib_ImageGetStride(dst);                     \
	nchannels = mlib_ImageGetChannels(dst);                 \
	bitoffset[0] = mlib_ImageGetBitOffset(src);             \
	bitoffsetd[0] = mlib_ImageGetBitOffset(dst);            \
	PREPARE_CONST()                                         \
	if ((width > stride * 8) || (width > strided * 8))      \
	    return MLIB_FAILURE

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	EDGE_BIT_FIRST(mask, p1, off_p1, p2, off_p2)            \
	{                                                       \
	    mask = 0xFF;                                        \
	    mask >>= off_p1;                                    \
	    if ((mlib_addr)p1 == (mlib_addr)p2) {               \
		mlib_s32 mask1 = 0xFF;                          \
	                                                        \
		mask1 <<= (SIZE_IN_BITS - 1 - off_p2);          \
		mask &= mask1;                                  \
	    }                                                   \
	}

#define	EDGE_BIT_LAST(mask, p1, p2, off_p2)                     \
	{                                                       \
	    mask = 0xFF;                                        \
	    mask <<= (SIZE_IN_BITS - 1 - off_p2);               \
	}

#else		   /* _LITTLE_ENDIAN */

#define	EDGE_BIT_FIRST(mask, p1, off_p1, p2, off_p2)                    \
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

#define	EDGE_BIT_LAST(mask, p1, p2, off_p2)                       \
	{                                                         \
	    mask = ~((FORMAT) 0);                                 \
	    left = 0;                                             \
	    right = (mlib_addr)p2 & (MASK);                       \
	    mask <<= (SIZE_IN_BITS - 1 - (right * 8 + off_p2));   \
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

CONST_LOGIC_BITS
{
	mlib_s32 i, j;
	mlib_s32 offdst, offsrc;
	mlib_s32 shift1, shift1_, shiftc, shiftc_;
	mlib_s32 amount, dend_offset[1];
	mlib_u8 *dend;
	FORMAT *dpp, *spp;
#ifndef _LITTLE_ENDIAN
	FORMAT sa1, sa2 = 0, da;
	FORMAT c1, c2, c3, emask;
	mlib_s32 k, left, right;
#endif		   /* _LITTLE_ENDIAN */

	VALIDATE();

	if ((width == stride * 8) &&
	    (width == strided * 8)) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	dend_offset[0] = (bitoffsetd[0] + amount - 1) & 7;

	if (nchannels == 3) {

		for (j = 0; j < height; j++) {

			dend = dp + ((bitoffsetd[0] + amount - 1) >> 3);
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			offdst = offdst * 8 + bitoffsetd[0];
			spp =
			    (FORMAT *) (((mlib_addr)(sp - (offdst -
			    bitoffset[0] + 7) / 8)) & (~(MASK)));

			offsrc =
			    ((mlib_u8 *)sp - (mlib_u8 *)spp) * 8 +
			    bitoffset[0] - offdst;

			if (offsrc == 0) {
#ifdef _LITTLE_ENDIAN
				FORMAT sa1, da;
				FORMAT c1, c2, c3, emask;
#endif /* _LITTLE_ENDIAN */
				i = -offdst;
				c1 = c01;
				c2 = c02;
				c3 = c03;

				if (i != 0) {
					shiftc = offdst;
					shiftc_ = (SIZE_IN_BITS) - shiftc;
					c1 = (c01 >> shiftc) | (c03 << shiftc_);
					c2 = (c02 >> shiftc) | (c01 << shiftc_);
					c3 = (c03 >> shiftc) | (c02 << shiftc_);
					EDGE_BIT_FIRST(emask, dp, bitoffsetd[0],
					    dend, dend_offset[0]);
					sa1 = (*spp++);
					da = CONSTLOGIC(c1, sa1);
					STORE_MASK(emask, dpp, da);
					SWAP(c1, c2)
					SWAP(c2, c3)
					i += (SIZE_IN_BITS);
					dpp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= amount - (STEP_3CH);
				    i += (STEP_3CH)) {
					dpp[0] = CONSTLOGIC(c1, spp[0]);
					dpp[1] = CONSTLOGIC(c2, spp[1]);
					dpp[2] = CONSTLOGIC(c3, spp[2]);
					dpp += 3;
					spp += 3;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BITS)) {
						sa1 = (*spp++);
						dpp[0] = CONSTLOGIC(c1, sa1);
						i += (SIZE_IN_BITS);
						dpp++;

						if (i <=
						    (amount - SIZE_IN_BITS)) {
							sa1 = (*spp++);
							dpp[0] =
							    CONSTLOGIC(c2, sa1);
							i += (SIZE_IN_BITS);
							dpp++;
							c1 = c3;
						} else
							c1 = c2;
					}
					if (i < amount) {
						EDGE_BIT_LAST(emask, dpp, dend,
						    dend_offset[0]);
						sa1 = *spp;
						da = CONSTLOGIC(c1, sa1);
						STORE_MASK(emask, dpp, da);
					}
				}
			} else {
#ifdef _LITTLE_ENDIAN
				mlib_s32 sa1, sa2 = 0, da;
				mlib_s32 c1, c2, c3, emask;
#endif /* _LITTLE_ENDIAN */
				i = -offdst;
				shift1 = offsrc;
				shift1_ = (SIZE_IN_BITS) - shift1;

				c1 = c01;
				c2 = c02;
				c3 = c03;

				if ((mlib_addr)spp == ((mlib_addr) sp &
				    (~(MASK))))
					sa1 = spp[0];

				if (i != 0) {
					shiftc = offdst;
					shiftc_ = (SIZE_IN_BITS) - shiftc;
					c1 = (c01 >> shiftc) | (c03 << shiftc_);
					c2 = (c02 >> shiftc) | (c01 << shiftc_);
					c3 = (c03 >> shiftc) | (c02 << shiftc_);
					EDGE_BIT_FIRST(emask, dp, bitoffsetd[0],
					    dend, dend_offset[0]);
					if ((amount - i) > shift1_)
						sa2 = spp[1];
					da = CONSTLOGIC(c1,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
					STORE_MASK(emask, dpp, da);
					sa1 = sa2;
					SWAP(c1, c2)
					SWAP(c2, c3)
					i += (SIZE_IN_BITS);
					dpp++;
					spp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= amount - (STEP_3CH);
				    i += (STEP_3CH)) {
					sa2 = spp[1];
					dpp[0] =
					    CONSTLOGIC(c1,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
					sa1 = spp[2];
					dpp[1] =
					    CONSTLOGIC(c2,
					    ((sa2 << shift1) +
					    (sa1 >> shift1_)));
					sa2 = spp[3];
					dpp[2] =
					    CONSTLOGIC(c3,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
					sa1 = sa2;
					dpp += 3;
					spp += 3;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BITS)) {
						sa2 = spp[1];
						dpp[0] =
						    CONSTLOGIC(c1,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
						sa1 = sa2;
						i += (SIZE_IN_BITS);
						dpp++;
						spp++;

						if (i <=
						    (amount - SIZE_IN_BITS)) {
							sa2 = spp[1];
							dpp[0] =
							    CONSTLOGIC(c2,
							    ((sa1 << shift1) +
							    (sa2 >> shift1_)));
							sa1 = sa2;
							i += (SIZE_IN_BITS);
							dpp++;
							spp++;
							c1 = c3;
						} else
							c1 = c2;
					}
					if (i < amount) {
						EDGE_BIT_LAST(emask, dpp, dend,
						    dend_offset[0]);
						if (amount - i > shift1_)
							sa2 = spp[1];
						da = CONSTLOGIC(c1,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
						STORE_MASK(emask, dpp, da);
					}
				}
			}

			sp += stride;
			dp += strided;
		}
	} else {

		c01 =
		    (c01 >> bitoffsetd[0]) | (c01 << (SIZE_IN_BITS -
		    bitoffsetd[0]));

		for (j = 0; j < height; j++) {

			dend = dp + ((bitoffsetd[0] + amount - 1) >> 3);
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			offdst = offdst * 8 + bitoffsetd[0];
			spp =
			    (FORMAT *) (((mlib_addr)(sp - (offdst -
			    bitoffset[0] + 7) / 8)) & (~(MASK)));

			offsrc =
			    ((mlib_u8 *)sp - (mlib_u8 *)spp) * 8 +
			    bitoffset[0] - offdst;

			if (offsrc == 0) {
#ifdef _LITTLE_ENDIAN
				FORMAT sa1, da;
				FORMAT emask;
#endif /* _LITTLE_ENDIAN */
				i = -offdst;

				if (i != 0) {
					EDGE_BIT_FIRST(emask, dp, bitoffsetd[0],
					    dend, dend_offset[0]);
					sa1 = (*spp++);
					da = CONSTLOGIC(c01, sa1);
					STORE_MASK(emask, dpp, da);
					i += (SIZE_IN_BITS);
					dpp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < amount - (STEP); i += (STEP)) {
					dpp[0] = CONSTLOGIC(c01, spp[0]);
					dpp[1] = CONSTLOGIC(c01, spp[1]);
					dpp += 2;
					spp += 2;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BITS)) {
						sa1 = (*spp++);
						dpp[0] = CONSTLOGIC(c01, sa1);
						i += (SIZE_IN_BITS);
						dpp++;
					}
					if (i < amount) {
						EDGE_BIT_LAST(emask, dpp, dend,
						    dend_offset[0]);
						sa1 = *spp;
						da = CONSTLOGIC(c01, sa1);
						STORE_MASK(emask, dpp, da);
					}
				}
			} else {
#ifdef _LITTLE_ENDIAN
				mlib_s32 sa1, sa2 = 0, da;
				mlib_s32 emask;
#endif /* _LITTLE_ENDIAN */
				i = -offdst;
				shift1 = offsrc;
				shift1_ = (SIZE_IN_BITS) - shift1;

				if ((mlib_addr)spp == ((mlib_addr) sp &
				    (~(MASK))))
					sa1 = spp[0];

				if (i != 0) {
					EDGE_BIT_FIRST(emask, dp, bitoffsetd[0],
					    dend, dend_offset[0]);
					if ((amount - i) > shift1_)
						sa2 = spp[1];
					da = CONSTLOGIC(c01,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
					STORE_MASK(emask, dpp, da);
					sa1 = sa2;
					i += (SIZE_IN_BITS);
					dpp++;
					spp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < amount - (STEP); i += (STEP)) {
					sa2 = spp[1];
					dpp[0] =
					    CONSTLOGIC(c01,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
					sa1 = spp[2];
					dpp[1] =
					    CONSTLOGIC(c01,
					    ((sa2 << shift1) +
					    (sa1 >> shift1_)));
					dpp += 2;
					spp += 2;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BITS)) {
						sa2 = spp[1];
						dpp[0] =
						    CONSTLOGIC(c01,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
						sa1 = sa2;
						i += (SIZE_IN_BITS);
						dpp++;
						spp++;
					}
					if (i < amount) {
						EDGE_BIT_LAST(emask, dpp, dend,
						    dend_offset[0]);
						if (amount - i > shift1_)
							sa2 = spp[1];
						da = CONSTLOGIC(c01,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
						STORE_MASK(emask, dpp, da);
					}
				}
			}

			sp += stride;
			dp += strided;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGECONSTLOGIC_BIT_H */
