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

#pragma ident	"@(#)mlib_ImagePolynomWarpTable_0.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>

/* *********************************************************** */

#define	COMP_POINT(TYPE, MIN, MAX)                                  \
	{                                                           \
	    TYPE *sp, *sp0;                                         \
	    mlib_d64 fsum, lsum;                                    \
	    mlib_s32 i, j, ch;                                      \
	                                                            \
	    for (ch = 0; ch < channel; ch++) {                      \
		sp0 = (TYPE *) srcData + ch;                        \
		for (fsum = 0, i = 0; i < hkernel; i++) {           \
		    sp = sp0;                                       \
		    for (lsum = 0, j = 0; j < wkernel; j++) {       \
			lsum += *sp * pH[j];                        \
			sp += channel;                              \
		    }                                               \
		    sp0 = (TYPE *) ((mlib_u8 *)sp0 + src_stride);   \
		    fsum += lsum * pV[i];                           \
		}                                                   \
		color[ch] =                                         \
		    (fsum > MAX) ? MAX : (fsum <                    \
		    MIN) ? MIN : (mlib_s32)fsum;                    \
	    }                                                       \
	}

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarpTable_0(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_d64 postScaleX,
    mlib_d64 postScaleY,
    const void *interp_table,
    mlib_edge edge)
{
	mlib_s32 color[4];
	mlib_d64 wx, wy;
	mlib_d64 Xstart, Ystart, Xstop, Ystop;
	mlib_s32 wkernel = mlib_ImageGetInterpWidth(interp_table);
	mlib_s32 hkernel = mlib_ImageGetInterpHeight(interp_table);
	mlib_s32 top = mlib_ImageGetInterpTopPadding(interp_table);
	mlib_s32 left = mlib_ImageGetInterpLeftPadding(interp_table);
	mlib_u8 *srcData = mlib_ImageGetData(src);
	mlib_type itype = mlib_ImageGetType(src);
	mlib_s32 type = (itype == MLIB_BYTE) ? 1 :
	    ((itype == MLIB_SHORT || itype == MLIB_USHORT) ? 2 : 4);
	mlib_d64 *pH = mlib_ImageGetInterpDoubleDataH(interp_table);
	mlib_d64 *pV = mlib_ImageGetInterpDoubleDataV(interp_table);
	mlib_d64 scaleH = 1 << mlib_ImageGetInterpSubsampleBitsH(interp_table);
	mlib_d64 scaleV = 1 << mlib_ImageGetInterpSubsampleBitsV(interp_table);
	mlib_s32 shiftH = mlib_ImageGetInterpWidthBits(interp_table);
	mlib_s32 shiftV = mlib_ImageGetInterpHeightBits(interp_table);
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 channel = mlib_ImageGetChannels(src);

	wx = xCoeffs[0] * postScaleX - postShiftX;
	wy = yCoeffs[0] * postScaleY - postShiftY;

	Xstart = left + 0.5;
	Ystart = top + 0.5;
	Xstop = mlib_ImageGetWidth(src) - (wkernel - Xstart);
	Ystop = mlib_ImageGetHeight(src) - (hkernel - Ystart);

	if (edge == MLIB_EDGE_SRC_PADDED) {
		mlib_u8 *paddings = mlib_ImageGetPaddings(src);

		if (paddings[0] > Xstart)
			Xstart = paddings[0];

		if (paddings[1] > Ystart)
			Ystart = paddings[1];

		if ((mlib_ImageGetWidth(src) - paddings[2]) < Xstop)
			Xstop = mlib_ImageGetWidth(src) - paddings[2];

		if ((mlib_ImageGetHeight(src) - paddings[3]) < Ystop)
			Ystop = mlib_ImageGetHeight(src) - paddings[3];
	}

	if (!(wx >= Xstart && wx < Xstop && wy >= Ystart && wy < Ystop))
		return (MLIB_SUCCESS);

	wx -= 0.5;
	wy -= 0.5;

	srcData +=
	    ((mlib_s32)wy - top) * src_stride + ((mlib_s32)wx -
	    left) * channel * type;

	pH += (mlib_s32)((wx - (mlib_s32)wx) * scaleH) << shiftH;
	pV += (mlib_s32)((wy - (mlib_s32)wy) * scaleV) << shiftV;

	switch (itype) {
	case MLIB_BYTE:
		COMP_POINT(mlib_u8,
		    MLIB_U8_MIN,
		    MLIB_U8_MAX);

		break;

	case MLIB_SHORT:
		COMP_POINT(mlib_s16,
		    MLIB_S16_MIN,
		    MLIB_S16_MAX);

		break;

	case MLIB_USHORT:
		COMP_POINT(mlib_u16,
		    MLIB_U16_MIN,
		    MLIB_U16_MAX);

		break;

	case MLIB_INT:
		COMP_POINT(mlib_s32,
		    MLIB_S32_MIN,
		    MLIB_S32_MAX);

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (__mlib_ImageClear(dst, color));
}

/* *********************************************************** */
