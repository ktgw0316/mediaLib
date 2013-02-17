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

#ifndef _MLIB_C_IMAGECONSTLOGIC_H
#define	_MLIB_C_IMAGECONSTLOGIC_H

#pragma ident	"@(#)mlib_c_ImageConstLogic.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
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
#define	STEP	2*(SIZE_IN_BYTES)
#define	STEP_3CH	3*(SIZE_IN_BYTES)

#define	SWAP(c1, c2) {                              \
	FORMAT tmp = c2; c2 = c1; c1 = tmp;         \
}

#if defined(_NO_LONGLONG)

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	PREPARE_CONST()                                                    \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT)) {               \
	    width *= (2 * channels);                                       \
	    if (channels == 1) {                                           \
		c01 = c[0] & 0xFFFF;                                       \
		c01 |= (c01 << 16);                                        \
		c02 = c01;                                                 \
	    } else                                                         \
		if (channels == 2) {                                       \
		    c01 = ((c[1] & 0xFFFF) << 16) | (c[0] & 0xFFFF);       \
		    c02 = c01;                                             \
		} else                                                     \
		    if (channels == 3) {                                   \
			c01 = ((c[1] & 0xFFFF) << 16) | (c[0] & 0xFFFF);   \
			c02 = (c01 << 16) | (c[2] & 0xFFFF);               \
			c03 = (c01 >> 16) | (c02 << 16);                   \
		    } else {                                               \
			c01 = ((c[1] & 0xFFFF) << 16) | (c[0] & 0xFFFF);   \
			c02 = ((c[3] & 0xFFFF) << 16) | (c[2] & 0xFFFF);   \
		    }                                                      \
	} else                                                             \
	    if (type == MLIB_BYTE) {                                       \
		width *= channels;                                         \
		if (channels == 1) {                                       \
		    c01 = c[0] & 0xFF;                                     \
		    c01 |= (c01 << 8);                                     \
		    c01 |= (c01 << 16);                                    \
		    c02 = c01;                                             \
		} else                                                     \
		    if (channels == 2) {                                   \
			c01 = ((c[1] & 0xFF) << 8) | (c[0] & 0xFF);        \
			c01 |= (c01 << 16);                                \
			c02 = c01;                                         \
		    } else                                                 \
			if (channels == 3) {                               \
				c01 =                                      \
					((c[2] & 0xFF) << 16) |            \
					((c[1] & 0xFF) << 8)  |            \
					(c[0] & 0xFF);                     \
				c02 = (c01 << 16) | (c01 >> 8);            \
				c03 = (c01 << 8) | (c01 >> 16);            \
				c01 = (c01 << 24) | c01;                   \
			} else {                                           \
				c01 =                                      \
					((c[3] & 0xFF) << 24) |            \
					((c[2] & 0xFF) << 16) |            \
					((c[1] & 0xFF) << 8) |             \
					(c[0] & 0xFF);                     \
				c02 = c01;                                 \
			}                                                  \
	    }

/* *********************************************************** */

#else		   /* _LITTLE_ENDIAN */

#define	PREPARE_CONST()                                                    \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT)) {               \
	    width *= (2 * channels);                                       \
	    if (channels == 1) {                                           \
		c01 = c[0] & 0xFFFF;                                       \
		c01 |= (c01 << 16);                                        \
		c02 = c01;                                                 \
	    } else                                                         \
		if (channels == 2) {                                       \
		    c01 = ((c[0] & 0xFFFF) << 16) | (c[1] & 0xFFFF);       \
		    c02 = c01;                                             \
		} else                                                     \
		    if (channels == 3) {                                   \
			c01 = ((c[0] & 0xFFFF) << 16) | (c[1] & 0xFFFF);   \
			c02 = ((c[2] & 0xFFFF) << 16) | (c01 >> 16);       \
			c03 = (c01 << 16) | (c02 >> 16);                   \
		    } else {                                               \
			c01 = ((c[0] & 0xFFFF) << 16) | (c[1] & 0xFFFF);   \
			c02 = ((c[2] & 0xFFFF) << 16) | (c[3] & 0xFFFF);   \
		    }                                                      \
	} else                                                             \
	    if (type == MLIB_BYTE) {                                       \
		width *= channels;                                         \
		if (channels == 1) {                                       \
		    c01 = c[0] & 0xFF;                                     \
		    c01 |= (c01 << 8);                                     \
		    c01 |= (c01 << 16);                                    \
		    c02 = c01;                                             \
		} else                                                     \
		    if (channels == 2) {                                   \
			c01 = ((c[0] & 0xFF) << 8) | (c[1] & 0xFF);        \
			c01 |= (c01 << 16);                                \
			c02 = c01;                                         \
		    } else                                                 \
			if (channels == 3) {                               \
				c01 =                                      \
					((c[0] & 0xFF) << 16) |            \
					((c[1] & 0xFF) << 8)  |            \
					(c[2] & 0xFF);                     \
				c02 = (c01 << 16) | (c01 >> 8);            \
				c03 = (c01 << 24) | c01;                   \
				c01 = (c01 << 8) | (c01 >> 16);            \
			} else {                                           \
				c01 =                                      \
					((c[0] & 0xFF) << 24) |            \
					((c[1] & 0xFF) << 16) |            \
					((c[2] & 0xFF) << 8) |             \
					(c[3] & 0xFF);                     \
				c02 = c01;                                 \
			}                                                  \
	    }

#endif		   /* _LITTLE_ENDIAN */

#else		   /* defined(_NO_LONGLONG) */

#ifdef _LITTLE_ENDIAN
#define	PREPARE_CONST()                                                    \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT)) {               \
	    width *= (2 * channels);                                       \
	    if (channels == 1) {                                           \
		c01 = c[0] & 0xFFFF;                                       \
		c01 |= (c01 << 16);                                        \
		c01 |= (c01 << 32);                                        \
		c02 = c01;                                                 \
	    } else                                                         \
		if (channels == 2) {                                       \
		    c01 =                                                  \
			(((mlib_u64)(c[1] & 0xFFFF)) << 16) | (c[0] &      \
			0xFFFF);                                           \
		    c01 |= (c01 << 32);                                    \
		    c02 = c01;                                             \
		} else                                                     \
		    if (channels == 3) {                                   \
			c01 =                                              \
			    (((mlib_u64)(c[1] & 0xFFFF)) << 16) | (c[0]    \
			    & 0xFFFF);                                     \
			c02 =                                              \
			    ((mlib_u64)(c[2] & 0xFFFF)) | (c01 << 16);     \
			c01 = (c02 << 32) | c01;                           \
			c02 = (c02 << 16) | (c02 >> 32);                   \
			c03 = (c01 << 16) | (c01 >> 32);                   \
		    } else {                                               \
			c01 =                                              \
			    (((mlib_u64)(c[1] & 0xFFFF)) << 16) | (c[0]    \
			    & 0xFFFF);                                     \
			c02 =                                              \
			    (((mlib_u64)(c[3] & 0xFFFF)) << 16) | (c[2]    \
			    & 0xFFFF);                                     \
			c01 = (c02 << 32) | c01;                           \
			c02 = c01;                                         \
		    }                                                      \
	} else                                                             \
	    if (type == MLIB_BYTE) {                                       \
		width *= channels;                                         \
		if (channels == 1) {                                       \
			c01 = c[0] & 0xFF;                                 \
			c01 |= (c01 << 8);                                 \
			c01 |= (c01 << 16);                                \
			c01 |= (c01 << 32);                                \
			c02 = c01;                                         \
		} else                                                     \
		    if (channels == 2) {                                   \
			c01 = ((c[1] & 0xFF) << 8) | (c[0] & 0xFF);        \
			c01 |= (c01 << 16);                                \
			c01 |= (c01 << 32);                                \
			c02 = c01;                                         \
		    } else                                                 \
			if (channels == 3) {                               \
				c01 =                                      \
					((c[2] & 0xFF) << 16) |            \
					((c[1] & 0xFF) << 8)  |            \
					(c[0] & 0xFF);                     \
				c01 |= (c01 << 24);                        \
				c01 = (c01 << 24) | (c01 >> 24);           \
				c02 = (c01 << 32) | (c01 >> 16);           \
				c03 = (c01 << 16) | (c01 >> 32);           \
			} else {                                           \
				c01 =                                      \
				    (((mlib_u64)(c[3] & 0xFF)) << 24) |    \
				    ((c[2] & 0xFF) << 16) |                \
				    ((c[1] & 0xFF) << 8) |                 \
				    (c[0] & 0xFF);                         \
				c01 |= (c01 << 32);                        \
				c02 = c01;                                 \
			}                                                  \
	    } else {                                                       \
		width *= (4 * channels);                                   \
		if (channels == 1) {                                       \
			c01 = c[0] & 0xFFFFFFFF;                           \
			c01 |= (c01 << 32);                                \
			c02 = c01;                                         \
		} else                                                     \
			if (channels == 2) {                               \
				c01 = c[1] & 0xFFFFFFFF;                   \
				c02 = c[0] & 0xFFFFFFFF;                   \
				c01 = (c01 << 32) | c02;                   \
				c02 = c01;                                 \
			} else                                             \
				if (channels == 3) {                       \
					c01 = c[0] & 0xFFFFFFFF;           \
					c02 = c[1] & 0xFFFFFFFF;           \
					c03 = c[2] & 0xFFFFFFFF;           \
					c01 = (c02 << 32) | c01;           \
					c03 = (c03 << 32) | (c01 >> 32);   \
					c02 = (c01 << 32) | (c03 >> 32);   \
				} else {                                   \
					c01 = c[1] & 0xFFFFFFFF;           \
					c02 = c[0] & 0xFFFFFFFF;           \
					c01 = (c01 << 32) | c02;           \
					c02 = c[3] & 0xFFFFFFFF;           \
					c03 = c[2] & 0xFFFFFFFF;           \
					c02 = (c02 << 32) | c03;           \
				}                                          \
	    }
#else		   /* _LITTLE_ENDIAN */

#define	PREPARE_CONST()                                                    \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT)) {               \
	    width *= (2 * channels);                                       \
	    if (channels == 1) {                                           \
		c01 = c[0] & 0xFFFF;                                       \
		c01 |= (c01 << 16);                                        \
		c01 |= (c01 << 32);                                        \
		c02 = c01;                                                 \
	    } else                                                         \
		if (channels == 2) {                                       \
		    c01 =                                                  \
			(((mlib_u64)(c[0] & 0xFFFF)) << 16) | (c[1] &      \
			0xFFFF);                                           \
		    c01 |= (c01 << 32);                                    \
		    c02 = c01;                                             \
		} else                                                     \
		    if (channels == 3) {                                   \
			c01 =                                              \
			    (((mlib_u64)(c[0] & 0xFFFF)) << 16) | (c[1]    \
			    & 0xFFFF);                                     \
			c02 =                                              \
			    (((mlib_u64)(c[2] & 0xFFFF)) << 16) | (c01     \
			    >> 16);                                        \
			c01 = (c01 << 32) | c02;                           \
			c02 = (c01 << 16) | (c01 >> 32);                   \
			c03 = (c02 << 16) | (c02 >> 32);                   \
		    } else {                                               \
			c01 =                                              \
			    (((mlib_u64)(c[0] & 0xFFFF)) << 16) | (c[1]    \
			    & 0xFFFF);                                     \
			c02 =                                              \
			    (((mlib_u64)(c[2] & 0xFFFF)) << 16) | (c[3]    \
			    & 0xFFFF);                                     \
			c01 = (c01 << 32) | c02;                           \
			c02 = c01;                                         \
		    }                                                      \
	} else                                                             \
	    if (type == MLIB_BYTE) {                                       \
		width *= channels;                                         \
		if (channels == 1) {                                       \
			c01 = c[0] & 0xFF;                                 \
			c01 |= (c01 << 8);                                 \
			c01 |= (c01 << 16);                                \
			c01 |= (c01 << 32);                                \
			c02 = c01;                                         \
		} else                                                     \
		    if (channels == 2) {                                   \
			c01 = ((c[0] & 0xFF) << 8) | (c[1] & 0xFF);        \
			c01 |= (c01 << 16);                                \
			c01 |= (c01 << 32);                                \
			c02 = c01;                                         \
		    } else                                                 \
			if (channels == 3) {                               \
				c01 =                                      \
					((c[0] & 0xFF) << 16) |            \
					((c[1] & 0xFF) << 8)  |            \
					(c[2] & 0xFF);                     \
				c01 |= (c01 << 24);                        \
				c02 = (c01 << 32) | (c01 >> 16);           \
				c03 = (c01 << 48) | c01;                   \
				c01 = (c01 << 16) | (c01 >> 32);           \
			} else {                                           \
				c01 =                                      \
				    (((mlib_u64)(c[0] & 0xFF)) << 24) |    \
				    ((c[1] & 0xFF) << 16) |                \
				    ((c[2] & 0xFF) << 8) |                 \
				    (c[3] & 0xFF);                         \
				c01 |= (c01 << 32);                        \
				c02 = c01;                                 \
			}                                                  \
	    } else {                                                       \
		width *= (4 * channels);                                   \
		if (channels == 1) {                                       \
			c01 = c[0] & 0xFFFFFFFF;                           \
			c01 |= (c01 << 32);                                \
			c02 = c01;                                         \
		} else                                                     \
			if (channels == 2) {                               \
				c01 = c[0] & 0xFFFFFFFF;                   \
				c02 = c[1] & 0xFFFFFFFF;                   \
				c01 = (c01 << 32) | c02;                   \
				c02 = c01;                                 \
			} else                                             \
				if (channels == 3) {                       \
					c01 = c[0] & 0xFFFFFFFF;           \
					c02 = c[1] & 0xFFFFFFFF;           \
					c03 = c[2] & 0xFFFFFFFF;           \
					c01 = (c01 << 32) | c02;           \
					c02 = (c03 << 32) | (c01 >> 32);   \
					c03 = (c01 << 32) | (c02 >> 32);   \
				} else {                                   \
					c01 = c[0] & 0xFFFFFFFF;           \
					c02 = c[1] & 0xFFFFFFFF;           \
					c01 = (c01 << 32) | c02;           \
					c02 = c[2] & 0xFFFFFFFF;           \
					c03 = c[3] & 0xFFFFFFFF;           \
					c02 = (c02 << 32) | c03;           \
				}                                          \
	    }

#endif		   /* _LITTLE_ENDIAN */

#endif		   /* defined(_NO_LONGLONG) */

/* *********************************************************** */

#define	VALIDATE()                                                  \
/*                                                                  \
 * pointers for pixel and line of source                            \
 */                                                                 \
	mlib_u8 *sp;                                                \
/*                                                                  \
 * pointers for pixel and line of dst                               \
 */                                                                 \
	mlib_u8 *dp;                                                \
	mlib_s32 width, height, channels, type, nchannels;          \
/*                                                                  \
 * for src                                                          \
 */                                                                 \
	mlib_s32 stride;                                            \
/*                                                                  \
 * for dst                                                          \
 */                                                                 \
	mlib_s32 strided;                                           \
	                                                            \
	FORMAT c01 = 0, c02 = 0, c03 = 0;                           \
	                                                            \
	MLIB_IMAGE_SIZE_EQUAL(dst, src);                            \
	MLIB_IMAGE_TYPE_EQUAL(dst, src);                            \
	MLIB_IMAGE_CHAN_EQUAL(dst, src);                            \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                     \
	sp = (mlib_u8 *)mlib_ImageGetData(src);                     \
	height = mlib_ImageGetHeight(dst);                          \
	width = mlib_ImageGetWidth(dst);                            \
	stride = mlib_ImageGetStride(src);                          \
	strided = mlib_ImageGetStride(dst);                         \
	nchannels = channels = mlib_ImageGetChannels(dst);          \
	type = mlib_ImageGetType(dst);                              \
	PREPARE_CONST()                                             \
	if ((width > stride) || (width > strided))                  \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	EDGE_U8_FIRST(p1, p2)                                       \
	left = (mlib_addr)p1 & (MASK);                              \
	right = (MASK);                                             \
	if ((((mlib_addr)p1 ^ (mlib_addr)p2) & (~(MASK))) == 0) {   \
	    right = (mlib_addr)p2 & (MASK);                         \
	}

/* *********************************************************** */

#define	EDGE_U8_LAST(p2)	left = 0; right = (mlib_addr)p2 & (MASK);

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	STORE_MASK_U8(p, a)                                     \
	a >>= (left * 8);                                       \
	for (k = left; k <= right; k++) {                       \
	    ((mlib_u8 *)(p))[k] = (a);                          \
	    a >>= 8;                                            \
	}

#else		   /* _LITTLE_ENDIAN */

#define	STORE_MASK_U8(p, a)                                     \
	a >>= (((MASK) - right) * 8);                           \
	for (k = right; k >= left; k--) {                       \
	    ((mlib_u8 *)(p))[k] = (a);                          \
	    a >>= 8;                                            \
	}

#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

static mlib_status
mlib_c_ImageConstLogic(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s32 i, j, k, left, right;
	mlib_s32 offdst, offsrc;
	FORMAT *dpp, *spp;
	FORMAT sa1 = 0, sa2 = 0, da;
	mlib_s32 shift1, shift1_, shiftc, shiftc_;
	mlib_s32 amount;
	mlib_u8 *dend;
	FORMAT c1, c2, c3;

	VALIDATE();

	if ((width == stride) && (width == strided)) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	if (nchannels == 3) {

		for (j = 0; j < height; j++) {

			dend = dp + amount - 1;
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			spp =
			    (FORMAT *) (((mlib_addr)(sp - offdst)) & (~(MASK)));
			offsrc = (mlib_u8 *)(sp - offdst) - (mlib_u8 *)spp;

			if (offsrc == 0) {
				i = (mlib_u8 *)dpp - dp;
				c1 = c01;
				c2 = c02;
				c3 = c03;

				if (i != 0) {
					shiftc = -i * 8;
					shiftc_ = (SIZE_IN_BITS) - shiftc;
#ifdef _LITTLE_ENDIAN
					c1 = (c01 << shiftc) | (c03 >> shiftc_);
					c2 = (c02 << shiftc) | (c01 >> shiftc_);
					c3 = (c03 << shiftc) | (c02 >> shiftc_);
#else /* _LITTLE_ENDIAN */
					c1 = (c01 >> shiftc) | (c03 << shiftc_);
					c2 = (c02 >> shiftc) | (c01 << shiftc_);
					c3 = (c03 >> shiftc) | (c02 << shiftc_);
#endif /* _LITTLE_ENDIAN */
					EDGE_U8_FIRST(dp, dend);
					sa1 = (*spp++);
					da = CONSTLOGIC(c1, sa1);
					STORE_MASK_U8(dpp, da);
					SWAP(c1, c2)
					SWAP(c2, c3)
					i += (SIZE_IN_BYTES);
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

					if (i <= (amount - SIZE_IN_BYTES)) {

						sa1 = (*spp++);
						dpp[0] = CONSTLOGIC(c1, sa1);
						i += (SIZE_IN_BYTES);
						dpp++;

						if (i <=
						    (amount - SIZE_IN_BYTES)) {
							sa1 = (*spp++);
							dpp[0] =
							    CONSTLOGIC(c2, sa1);
							i += (SIZE_IN_BYTES);
							dpp++;
							c1 = c3;
						} else
							c1 = c2;
					}
					if (i < amount) {
						EDGE_U8_LAST(dend);
						sa1 = *spp;
						da = CONSTLOGIC(c1, sa1);
						STORE_MASK_U8(dpp, da);
					}
				}
			} else {
				i = (mlib_u8 *)dpp - dp;
				shift1 = offsrc * 8;
				shift1_ = (SIZE_IN_BITS) - shift1;

				c1 = c01;
				c2 = c02;
				c3 = c03;

				if ((mlib_addr)spp == ((mlib_addr) sp &
				    (~(MASK))))
					sa1 = spp[0];

				if (i != 0) {
					shiftc = -i * 8;
					shiftc_ = (SIZE_IN_BITS) - shiftc;
#ifdef _LITTLE_ENDIAN
					c1 = (c01 << shiftc) | (c03 >> shiftc_);
					c2 = (c02 << shiftc) | (c01 >> shiftc_);
					c3 = (c03 << shiftc) | (c02 >> shiftc_);
					EDGE_U8_FIRST(dp, dend);
					if ((mlib_u8 *)spp < (sp + amount -
					    SIZE_IN_BYTES))
						sa2 = spp[1];
					da = CONSTLOGIC(c1,
					    ((sa1 >> shift1) +
					    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
					c1 = (c01 >> shiftc) | (c03 << shiftc_);
					c2 = (c02 >> shiftc) | (c01 << shiftc_);
					c3 = (c03 >> shiftc) | (c02 << shiftc_);
					EDGE_U8_FIRST(dp, dend);
					if ((mlib_u8 *)spp < (sp + amount -
					    SIZE_IN_BYTES))
						sa2 = spp[1];
					da = CONSTLOGIC(c1,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
					STORE_MASK_U8(dpp, da);
					sa1 = sa2;
					SWAP(c1, c2)
					SWAP(c2, c3)
					i += (SIZE_IN_BYTES);
					dpp++;
					spp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= amount - (STEP_3CH);
				    i += (STEP_3CH)) {
#ifdef _LITTLE_ENDIAN
					sa2 = spp[1];
					dpp[0] =
					    CONSTLOGIC(c1,
					    ((sa1 >> shift1) +
					    (sa2 << shift1_)));
					sa1 = spp[2];
					dpp[1] =
					    CONSTLOGIC(c2,
					    ((sa2 >> shift1) +
					    (sa1 << shift1_)));
					sa2 = spp[3];
					dpp[2] =
					    CONSTLOGIC(c3,
					    ((sa1 >> shift1) +
					    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
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
#endif /* _LITTLE_ENDIAN */
					sa1 = sa2;
					dpp += 3;
					spp += 3;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BYTES)) {

						sa2 = spp[1];
#ifdef _LITTLE_ENDIAN
						dpp[0] =
						    CONSTLOGIC(c1,
						    ((sa1 >> shift1) +
						    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
						dpp[0] =
						    CONSTLOGIC(c1,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
						sa1 = sa2;
						i += (SIZE_IN_BYTES);
						dpp++;
						spp++;

						if (i <=
						    (amount - SIZE_IN_BYTES)) {
							sa2 = spp[1];
#ifdef _LITTLE_ENDIAN
							dpp[0] =
							    CONSTLOGIC(c2,
							    ((sa1 >> shift1) +
							    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
							dpp[0] =
							    CONSTLOGIC(c2,
							    ((sa1 << shift1) +
							    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
							sa1 = sa2;
							i += (SIZE_IN_BYTES);
							dpp++;
							spp++;
							c1 = c3;
						} else
							c1 = c2;
					}
					if (i < amount) {
						EDGE_U8_LAST(dend);
						if ((mlib_u8 *)spp < (sp +
						    amount - SIZE_IN_BYTES))
							sa2 = spp[1];
#ifdef _LITTLE_ENDIAN
						da = CONSTLOGIC(c1,
						    ((sa1 >> shift1) +
						    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
						da = CONSTLOGIC(c1,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
						STORE_MASK_U8(dpp, da);
					}
				}
			}

			sp += stride;
			dp += strided;
		}
	} else {

		for (j = 0; j < height; j++) {

			dend = dp + amount - 1;
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			spp =
			    (FORMAT *) (((mlib_addr)(sp - offdst)) & (~(MASK)));
			offsrc = (mlib_u8 *)(sp - offdst) - (mlib_u8 *)spp;

			if (offsrc == 0) {
				i = (mlib_u8 *)dpp - dp;
				c1 = c01;
				c2 = c02;

				if (i != 0) {
					shiftc = -i * 8;
					shiftc_ = (SIZE_IN_BITS) - shiftc;
#ifdef _LITTLE_ENDIAN
					c1 = (c01 << shiftc) | (c02 >> shiftc_);
					c2 = (c02 << shiftc) | (c01 >> shiftc_);
#else /* _LITTLE_ENDIAN */
					c1 = (c01 >> shiftc) | (c02 << shiftc_);
					c2 = (c02 >> shiftc) | (c01 << shiftc_);
#endif /* _LITTLE_ENDIAN */
					EDGE_U8_FIRST(dp, dend);
					sa1 = (*spp++);
					da = CONSTLOGIC(c1, sa1);
					STORE_MASK_U8(dpp, da);
					SWAP(c1, c2)
					i += (SIZE_IN_BYTES);
					dpp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < amount - (STEP); i += (STEP)) {
					dpp[0] = CONSTLOGIC(c1, spp[0]);
					dpp[1] = CONSTLOGIC(c2, spp[1]);
					dpp += 2;
					spp += 2;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BYTES)) {
						sa1 = (*spp++);
						dpp[0] = CONSTLOGIC(c1, sa1);
						i += (SIZE_IN_BYTES);
						dpp++;
						c1 = c2;
					}
					if (i < amount) {
						EDGE_U8_LAST(dend);
						sa1 = *spp;
						da = CONSTLOGIC(c1, sa1);
						STORE_MASK_U8(dpp, da);
					}
				}
			} else {
				i = (mlib_u8 *)dpp - dp;
				shift1 = offsrc * 8;
				shift1_ = (SIZE_IN_BITS) - shift1;

				c1 = c01;
				c2 = c02;

				if ((mlib_addr)spp == ((mlib_addr) sp &
				    (~(MASK))))
					sa1 = spp[0];

				if (i != 0) {
					shiftc = -i * 8;
					shiftc_ = (SIZE_IN_BITS) - shiftc;
#ifdef _LITTLE_ENDIAN
					c1 = (c01 << shiftc) | (c02 >> shiftc_);
					c2 = (c02 << shiftc) | (c01 >> shiftc_);
					EDGE_U8_FIRST(dp, dend);
					if ((mlib_u8 *)spp < (sp + amount -
					    SIZE_IN_BYTES))
						sa2 = spp[1];
					da = CONSTLOGIC(c1,
					    ((sa1 >> shift1) +
					    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
					c1 = (c01 >> shiftc) | (c02 << shiftc_);
					c2 = (c02 >> shiftc) | (c01 << shiftc_);
					EDGE_U8_FIRST(dp, dend);
					if ((mlib_u8 *)spp < (sp + amount -
					    SIZE_IN_BYTES))
						sa2 = spp[1];
					da = CONSTLOGIC(c1,
					    ((sa1 << shift1) +
					    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
					STORE_MASK_U8(dpp, da);
					sa1 = sa2;
					SWAP(c1, c2)
					i += (SIZE_IN_BYTES);
					dpp++;
					spp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < amount - (STEP); i += (STEP)) {
#ifdef _LITTLE_ENDIAN
					sa2 = spp[1];
					dpp[0] =
					    CONSTLOGIC(c1,
					    ((sa1 >> shift1) +
					    (sa2 << shift1_)));
					sa1 = spp[2];
					dpp[1] =
					    CONSTLOGIC(c2,
					    ((sa2 >> shift1) +
					    (sa1 << shift1_)));
#else /* _LITTLE_ENDIAN */
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
#endif /* _LITTLE_ENDIAN */
					dpp += 2;
					spp += 2;
				}

				if (i < amount) {

					if (i <= (amount - SIZE_IN_BYTES)) {
						sa2 = spp[1];
#ifdef _LITTLE_ENDIAN
						dpp[0] =
						    CONSTLOGIC(c1,
						    ((sa1 >> shift1) +
						    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
						dpp[0] =
						    CONSTLOGIC(c1,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
						sa1 = sa2;
						i += (SIZE_IN_BYTES);
						dpp++;
						spp++;
						c1 = c2;
					}
					if (i < amount) {
						EDGE_U8_LAST(dend);
						if ((mlib_u8 *)spp < (sp +
						    amount - SIZE_IN_BYTES))
							sa2 = spp[1];
#ifdef _LITTLE_ENDIAN
						da = CONSTLOGIC(c1,
						    ((sa1 >> shift1) +
						    (sa2 << shift1_)));
#else /* _LITTLE_ENDIAN */
						da = CONSTLOGIC(c1,
						    ((sa1 << shift1) +
						    (sa2 >> shift1_)));
#endif /* _LITTLE_ENDIAN */
						STORE_MASK_U8(dpp, da);
					}
				}
			}

			sp += stride;
			dp += strided;
		}
	}
	return (MLIB_SUCCESS);
}

#if defined(_NO_LONGLONG)

/* *********************************************************** */

static mlib_status
mlib_c_ImageConstLogic_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s32 i, j;
	mlib_u32 *dpp, *spp;
	mlib_s32 amount;
	mlib_u32 c1, c2, c3, c4;

	VALIDATE();

	if (nchannels == 3) {
		c1 = (mlib_u32)c[0];
		c2 = (mlib_u32)c[1];
		c3 = (mlib_u32)c[2];

		if (((width * 3) == (stride >> 2)) &&
		    ((width * 3) == (strided >> 2))) {

			amount = height * width;
			dpp = (mlib_u32 *)dp;
			spp = (mlib_u32 *)sp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < amount; i++) {
				dpp[0] = CONSTLOGIC(c1, spp[0]);
				dpp[1] = CONSTLOGIC(c2, spp[1]);
				dpp[2] = CONSTLOGIC(c3, spp[2]);
				dpp += 3;
				spp += 3;
			}

		} else {

			amount = width;

			for (j = 0; j < height; j++) {

				dpp = (mlib_u32 *)dp;
				spp = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < amount; i++) {
					dpp[0] = CONSTLOGIC(c1, spp[0]);
					dpp[1] = CONSTLOGIC(c2, spp[1]);
					dpp[2] = CONSTLOGIC(c3, spp[2]);
					dpp += 3;
					spp += 3;
				}

				sp += stride;
				dp += strided;
			}
		}
	} else {

		if (nchannels == 1) {
			c1 = c2 = c3 = c4 = (mlib_u32)c[0];
		} else if (nchannels == 2) {
			c1 = c3 = (mlib_u32)c[0];
			c2 = c4 = (mlib_u32)c[1];
		} else {
			c1 = (mlib_u32)c[0];
			c2 = (mlib_u32)c[1];
			c3 = (mlib_u32)c[2];
			c4 = (mlib_u32)c[3];
		}

		if (((width * channels) == (stride >> 2)) &&
		    ((width * channels) == (strided >> 2))) {

			amount = height * width * channels;
			dpp = (mlib_u32 *)dp;
			spp = (mlib_u32 *)sp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= amount - 4; i += 4) {
				dpp[0] = CONSTLOGIC(c1, spp[0]);
				dpp[1] = CONSTLOGIC(c2, spp[1]);
				dpp[2] = CONSTLOGIC(c3, spp[2]);
				dpp[3] = CONSTLOGIC(c4, spp[3]);
				dpp += 4;
				spp += 4;
			}

			if (i < amount) {
				dpp[0] = CONSTLOGIC(c1, spp[0]);
				dpp++;
				spp++;
				i++;

				if (i < amount) {
					dpp[0] = CONSTLOGIC(c2, spp[0]);
					dpp++;
					spp++;
					i++;

					if (i < amount) {
						dpp[0] = CONSTLOGIC(c3, spp[0]);
					}
				}
			}
		} else {

			amount = width * channels;

			for (j = 0; j < height; j++) {

				dpp = (mlib_u32 *)dp;
				spp = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= amount - 4; i += 4) {
					dpp[0] = CONSTLOGIC(c1, spp[0]);
					dpp[1] = CONSTLOGIC(c2, spp[1]);
					dpp[2] = CONSTLOGIC(c3, spp[2]);
					dpp[3] = CONSTLOGIC(c4, spp[3]);
					dpp += 4;
					spp += 4;
				}

				if (i < amount) {
					dpp[0] = CONSTLOGIC(c1, spp[0]);
					dpp++;
					spp++;
					i++;

					if (i < amount) {
						dpp[0] = CONSTLOGIC(c2, spp[0]);
						dpp++;
						spp++;
						i++;

						if (i < amount) {
							dpp[0] =
							    CONSTLOGIC(c3,
							    spp[0]);
						}
					}
				}

				sp += stride;
				dp += strided;
			}
		}
	}
	return (MLIB_SUCCESS);
}

#endif /* defined(_NO_LONGLONG) */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGECONSTLOGIC_H */
