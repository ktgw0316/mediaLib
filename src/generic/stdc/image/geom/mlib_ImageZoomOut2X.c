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

#pragma ident	"@(#)mlib_ImageZoomOut2X.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X -  image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */
#include <mlib_image.h>
#include <mlib_ImageZoomOut2X.h>
#include <mlib_ImageZoom2X.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomOut2X = __mlib_ImageZoomOut2X

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomOut2X) mlib_ImageZoomOut2X
    __attribute__((weak, alias("__mlib_ImageZoomOut2X")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

const mlib_u8 mlib_satU8_arr[128 + 256 + 144] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
	    111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125,
	    126, 127,
	128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
	    142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
	    158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
	    174, 175,
	176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
	    190, 191,
	192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
	    206, 207,
	208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
	    222, 223,
	224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237,
	    238, 239,
	240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
	    254, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	    255, 255
};

const mlib_u8 *mlib_satU8 = mlib_satU8_arr + 128;
const mlib_u32 mlib_mask_s16[] =
	{ 0x0000ffff, 0x7fff0000, 0x80000000, 0x0000ffff };
const mlib_u32 mlib_mask_u16[] =
	{ 0x0000ffff, 0xffff0000, 0xffff0000, 0x00000000 };

/* *********************************************************** */

void
mlib_ImageZoomOut2X_Nearest(
    NEAREST_PARAMETERS(void),
    mlib_s32 tshift,
    mlib_s32 chan)
{
	mlib_s32 align;

	align =
	    (((mlib_s32)p_src | (mlib_s32)p_dst) >> tshift) | (src_stride |
	    dst_stride);

#ifndef i386	/* do not perform the copying by mlib_d64 data type for x86 */
	while (((chan | align) & 1) == 0 && tshift < 3)
#else /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */
	while (((chan | align) & 1) == 0 && tshift < 2)
#endif /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */
	{
/* two channels as one channel of next type */
		chan >>= 1;
		align >>= 1;
		src_stride >>= 1;
		dst_stride >>= 1;
		tshift++;
	}

	switch (4 * tshift + (chan - 1)) {
	case 0:
		mlib_ImageZoomOut2X_U8_1_Nearest(NEAREST_P);
		break;
	case 1:
		mlib_ImageZoomOut2X_U8_2_Nearest(NEAREST_P);
		break;
	case 2:
		mlib_ImageZoomOut2X_U8_3_Nearest(NEAREST_P);
		break;
	case 3:
		mlib_ImageZoomOut2X_U8_4_Nearest(NEAREST_P);
		break;
	case 4:
		mlib_ImageZoomOut2X_S16_1_Nearest(NEAREST_P);
		break;
	case 5:
		mlib_ImageZoomOut2X_S16_2_Nearest(NEAREST_P);
		break;
	case 6:
		mlib_ImageZoomOut2X_S16_3_Nearest(NEAREST_P);
		break;
	case 7:
		mlib_ImageZoomOut2X_S16_4_Nearest(NEAREST_P);
		break;
	case 8:
		mlib_ImageZoomOut2X_S32_1_Nearest(NEAREST_P);
		break;
	case 9:
		mlib_ImageZoomOut2X_S32_2_Nearest(NEAREST_P);
		break;
	case 10:
		mlib_ImageZoomOut2X_S32_3_Nearest(NEAREST_P);
		break;
	case 11:
		mlib_ImageZoomOut2X_S32_4_Nearest(NEAREST_P);
		break;
	case 12:
		mlib_ImageZoomOut2X_D64_1_Nearest(NEAREST_P);
		break;
	case 13:
		mlib_ImageZoomOut2X_D64_2_Nearest(NEAREST_P);
		break;
	case 14:
		mlib_ImageZoomOut2X_D64_3_Nearest(NEAREST_P);
		break;
	case 15:
		mlib_ImageZoomOut2X_D64_4_Nearest(NEAREST_P);
		break;
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageZoomOut2X(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_type type;
	mlib_s32 tshift = 0;
	mlib_u8 *p_src, *p_dst;
	mlib_s32 src_stride, dst_stride;
	mlib_s32 src_height, dst_height, src_width, dst_width;
	mlib_s32 chan, bchan, dh, dw;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	MLIB_IMAGE_GET_ALL_PARAMS(src, type, chan, src_width, src_height,
	    src_stride, p_src);
	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, chan, dst_width, dst_height,
	    dst_stride, p_dst);

	switch (type) {
	case MLIB_BYTE:
		tshift = 0;
		break;
	case MLIB_SHORT:
		tshift = 1;
		break;
	case MLIB_USHORT:
		tshift = 1;
		break;
	case MLIB_INT:
		tshift = 2;
		break;
	default:
		return (MLIB_FAILURE);
	}

	bchan = chan << tshift;

	switch (filter) {
	case MLIB_NEAREST:
		BOUND_NEAREST();
		src_stride >>= tshift;
		dst_stride >>= tshift;

		if (dst_width > 0 && dst_height > 0) {
			mlib_ImageZoomOut2X_Nearest(NEAREST_P, tshift, chan);
		}

		return (MLIB_SUCCESS);

	case MLIB_BILINEAR:
		BOUND_BILINEAR();
		src_stride >>= tshift;
		dst_stride >>= tshift;
		switch (type) {
		case MLIB_BYTE:
			SWITCH_BL_CHANNELS(U8)
		case MLIB_SHORT:
			SWITCH_BL_CHANNELS(S16)
		case MLIB_USHORT:
			SWITCH_BL_CHANNELS(U16)
		case MLIB_INT:
			SWITCH_BL_CHANNELS(S32)
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC:
		BOUND_BICUBIC();
		src_stride >>= tshift;
		dst_stride >>= tshift;
		switch (type) {
		case MLIB_BYTE:
			SWITCH_BC_CHANNELS(U8, Bicubic)
		case MLIB_SHORT:
			SWITCH_BC_CHANNELS(S16, Bicubic)
		case MLIB_USHORT:
			SWITCH_BC_CHANNELS(U16, Bicubic)
		case MLIB_INT:
			SWITCH_BC_CHANNELS(S32, Bicubic)
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC2:
		BOUND_BICUBIC();
		src_stride >>= tshift;
		dst_stride >>= tshift;
		switch (type) {
		case MLIB_BYTE:
			SWITCH_BC_CHANNELS(U8, Bicubic2)
		case MLIB_SHORT:
			SWITCH_BC_CHANNELS(S16, Bicubic2)
		case MLIB_USHORT:
			SWITCH_BC_CHANNELS(U16, Bicubic2)
		case MLIB_INT:
			SWITCH_BC_CHANNELS(S32, Bicubic2)
		default:
			return (MLIB_FAILURE);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
