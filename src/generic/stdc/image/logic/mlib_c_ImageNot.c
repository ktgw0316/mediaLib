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

#pragma ident	"@(#)mlib_c_ImageNot.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot   - not an image (C version)
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
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT or
 *      MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:
 *          dst = ~src
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNot = __mlib_ImageNot

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNot) mlib_ImageNot
    __attribute__((weak, alias("__mlib_ImageNot")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _NO_LONGLONG

#define	SIZE_IN_BYTES	4
#define	FORMAT	mlib_u32

#else /* _NO_LONGLONG */

#define	SIZE_IN_BYTES	8
#define	FORMAT	mlib_u64

#endif /* _NO_LONGLONG */

/* *********************************************************** */

#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1

#define	STEP	(SIZE_IN_BYTES)

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
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT))      \
	    width *= 2;                                         \
	else if (type == MLIB_INT)                              \
	    width *= 4;                                         \
	if ((width * channels) > stride1 ||                     \
	    (width * channels) > strided)                       \
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

#define	OPERATION(a)	~(a)

/* *********************************************************** */

#define	ALIGN()                                                 \
	dpp[0] = OPERATION(spp1[0]);                            \
	dpp[1] = OPERATION(spp1[1]);                            \
	dpp += 2;                                               \
	spp1 += 2

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	NON_ALIGN()                                               \
	sa2 = spp1[0];                                            \
	dpp[0] = OPERATION((sa1 >> shift1) + (sa2 << shift1_));   \
	sa1 = sa2;                                                \
	dpp++;                                                    \
	spp1++

#else /* _LITTLE_ENDIAN */

#define	NON_ALIGN()                                               \
	sa2 = spp1[0];                                            \
	dpp[0] = OPERATION((sa1 << shift1) + (sa2 >> shift1_));   \
	sa1 = sa2;                                                \
	dpp++;                                                    \
	spp1++

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_ImageNot(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if ((type != MLIB_BYTE) &&
	    (type != MLIB_SHORT) &&
	    (type != MLIB_USHORT) && (type != MLIB_BIT) && (type != MLIB_INT))
		return (MLIB_FAILURE);

	if (type != MLIB_BIT) {

		mlib_s32 i, j, k, left, right;
		mlib_s32 offdst, offsrc;
		mlib_u8 *dend;
		FORMAT *dpp, *spp1;
		FORMAT sa1 = 0, sa2 = 0, da;
		mlib_s32 shift1, shift1_;
		mlib_s32 amount;

		VALIDATE();

		if (!mlib_ImageIsNotOneDvector(src) &&
		    !mlib_ImageIsNotOneDvector(dst)) {
			width *= height;
			height = 1;
		}

		amount = width * channels;

		for (j = 0; j < height; j++) {

			dend = dp + amount - 1;
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			spp1 =
			    (FORMAT *) (((mlib_addr)(sp1 -
			    offdst)) & (~(MASK)));
			offsrc = (mlib_u8 *)(sp1 - offdst) - (mlib_u8 *)spp1;

			if (offsrc == 0) {
/* offset_dst = offset_src */

				i = (mlib_u8 *)dpp - dp;

				if (i != 0) {
					EDGE_U8(dp, dend);
					da = OPERATION(((*spp1++)));
					STORE_MASK_U8(dpp, da);
					i += (SIZE_IN_BYTES);
					dpp++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= amount - 2 * (STEP);
				    i += 2 * (STEP)) {
					ALIGN();
				}

				if (i < amount) {
					EDGE_U8(dpp, dend);
					da = OPERATION((*spp1));
					STORE_MASK_U8(dpp, da);
					i += (SIZE_IN_BYTES);
					dpp++;
					spp1++;
				}

				if (i < amount) {
					EDGE_U8(dpp, dend);
					da = OPERATION((*spp1));
					STORE_MASK_U8(dpp, da);
				}
			} else {
/* offset_dst != offset_src */
				i = (mlib_u8 *)dpp - dp;
				shift1 = offsrc * 8;
				shift1_ = (SIZE_IN_BITS) - shift1;

				if ((mlib_addr)spp1 == ((mlib_addr) sp1 &
				    (~(MASK))))
					sa1 = *spp1;
				spp1++;

				if (i != 0) {
					if ((offsrc - i + amount) >
					    (SIZE_IN_BYTES))
						sa2 = *spp1;
					spp1++;
					EDGE_U8(dp, dend);
#ifdef _LITTLE_ENDIAN
					da = OPERATION((sa1 >> shift1) +
					    (sa2 << shift1_));
#else /* _LITTLE_ENDIAN */
					da = OPERATION((sa1 << shift1) +
					    (sa2 >> shift1_));
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
					NON_ALIGN();
				}

				if (i < amount) {
					if ((mlib_u8 *)spp1 < (sp1 + amount))
						sa2 = *spp1;
					EDGE_U8(dpp, dend);
#ifdef _LITTLE_ENDIAN
					da = OPERATION((sa1 >> shift1) +
					    (sa2 << shift1_));
#else /* _LITTLE_ENDIAN */
					da = OPERATION((sa1 << shift1) +
					    (sa2 >> shift1_));
#endif /* _LITTLE_ENDIAN */
					STORE_MASK_U8(dpp, da);
				}
			}

			sp1 += stride1;
			dp += strided;
		}
		return (MLIB_SUCCESS);
	} else {
		return (mlib_ImageNot_Bit(dst, src));
	}
}

/* *********************************************************** */
