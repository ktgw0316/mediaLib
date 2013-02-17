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

#pragma ident	"@(#)mlib_v_ImagePolynomialWarp_Call.c	9.2	07/11/05 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	TIN(SFX)	mlib_ImagePolynomialWarp_##SFX

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_2_5(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_d64 preShiftX,
    mlib_d64 preShiftY,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_IPWFCall_NN func_array_nn_all[4][3][4] = {
		{
/* degree = 2 */
			{TIN(2_NN_U8_1), TIN(2_NN_U8_2), TIN(2_NN_U8_3),
				TIN(2_NN_U8_4)},
			{TIN(2_NN_S16_1), TIN(2_NN_S16_2),
				TIN(2_NN_S16_3), TIN(2_NN_S16_4)},
			{TIN(2_NN_S32_1), TIN(2_NN_S32_2),
				TIN(2_NN_S32_3), TIN(2_NN_S32_4)},
		},
		{
/* degree = 3 */
			{TIN(3_NN_U8_1), TIN(3_NN_U8_2), TIN(3_NN_U8_3),
				TIN(3_NN_U8_4)},
			{TIN(3_NN_S16_1), TIN(3_NN_S16_2),
				TIN(3_NN_S16_3), TIN(3_NN_S16_4)},
			{TIN(3_NN_S32_1), TIN(3_NN_S32_2),
				TIN(3_NN_S32_3), TIN(3_NN_S32_4)},
		},
		{
/* degree = 4 */
			{TIN(4_NN_U8_1), TIN(4_NN_U8_2), TIN(4_NN_U8_3),
				TIN(4_NN_U8_4)},
			{TIN(4_NN_S16_1), TIN(4_NN_S16_2),
				TIN(4_NN_S16_3), TIN(4_NN_S16_4)},
			{TIN(4_NN_S32_1), TIN(4_NN_S32_2),
				TIN(4_NN_S32_3), TIN(4_NN_S32_4)},
		},
		{
/* degree = 5 */
			{TIN(5_NN_U8_1), TIN(5_NN_U8_2), TIN(5_NN_U8_3),
				TIN(5_NN_U8_4)},
			{TIN(5_NN_S16_1), TIN(5_NN_S16_2),
				TIN(5_NN_S16_3), TIN(5_NN_S16_4)},
			{TIN(5_NN_S32_1), TIN(5_NN_S32_2),
				TIN(5_NN_S32_3), TIN(5_NN_S32_4)},
		}
	};

	mlib_IPWClipLine array_func_clip_all[12] = {
/* degree = 2 */
		mlib_ImagePolynomialWarpClipLine_DG_2_2,
		mlib_ImagePolynomialWarpClipLine_DG_2_0,
		mlib_ImagePolynomialWarpClipLine_DG_2_1,
/* degree = 3 */
		mlib_ImagePolynomialWarpClipLine_DG_3_2,
		mlib_ImagePolynomialWarpClipLine_DG_3_0,
		mlib_ImagePolynomialWarpClipLine_DG_3_1,
/* degree = 4 */
		mlib_ImagePolynomialWarpClipLine_DG_4_2,
		mlib_ImagePolynomialWarpClipLine_DG_4_0,
		mlib_ImagePolynomialWarpClipLine_DG_4_1,
/* degree = 5 */
		mlib_ImagePolynomialWarpClipLine_DG_5_2,
		mlib_ImagePolynomialWarpClipLine_DG_5_0,
		mlib_ImagePolynomialWarpClipLine_DG_5_1,
	};

	mlib_IPWFCall func_array_call[2][4][4] = {
		{
			{TIN(BL_U8_1), TIN(BL_U8_2), TIN(BL_U8_3),
				TIN(BL_U8_4)},
			{TIN(BL_S16_1), TIN(BL_S16_2), TIN(BL_S16_3),
				TIN(BL_S16_4)},
			{TIN(BL_U16_1), TIN(BL_U16_2), TIN(BL_U16_3),
				TIN(BL_U16_4)},
			{TIN(BL_S32_1), TIN(BL_S32_2), TIN(BL_S32_3),
				TIN(BL_S32_4)},
		},
		{
			{TIN(BC_U8_1), TIN(BC_U8_2), TIN(BC_U8_3),
				TIN(BC_U8_4)},
			{TIN(BC_S16_1), TIN(BC_S16_2), TIN(BC_S16_3),
				TIN(BC_S16_4)},
			{TIN(BC_U16_1), TIN(BC_U16_2), TIN(BC_U16_3),
				TIN(BC_U16_4)},
			{TIN(BC_S32_1), TIN(BC_S32_2), TIN(BC_S32_3),
				TIN(BC_S32_4)},
		}
	};

	mlib_type type = mlib_ImageGetType(dst);
	mlib_u8 *srcData = mlib_ImageGetData(src);
	mlib_u8 *dstData = mlib_ImageGetData(dst);
	mlib_s32 pos, i, channels, len, n;
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);
	mlib_s32 srcStride = mlib_ImageGetStride(src);
	mlib_s32 dstWidth = mlib_ImageGetWidth(dst);
	mlib_s32 dstHeight = mlib_ImageGetHeight(dst);
	mlib_s32 dstStride = mlib_ImageGetStride(dst);
	mlib_d64 y = 0.5 + preShiftY;
	mlib_IPWClipLine func_clip;
	mlib_IPWFCall_NN func_array_nn;
	mlib_IPWFCall func_array;

	if (filter == MLIB_NEAREST) {

		channels = mlib_ImageGetChannels(dst);
		pos = (type == MLIB_BYTE) ? 1 : ((type == MLIB_SHORT ||
		    type == MLIB_USHORT) ? 2 : 4);

		lineAddr[0] = srcData;
		srcData -= channels * pos;
		for (i = 0; i <= srcHeight; i++) {
			lineAddr[i + 1] = srcData;
			srcData += srcStride;
		}

		pos = (type == MLIB_BYTE) ? 0 : ((type == MLIB_SHORT ||
		    type == MLIB_USHORT) ? 1 : 2);
		func_array_nn =
		    func_array_nn_all[pws->degree][pos][channels - 1];

		func_array_nn(dstData, lineAddr,
		    xCoeffs, yCoeffs, preShiftX, preShiftY,
		    srcWidth, srcHeight, dstWidth, dstHeight, dstStride);
	} else {

		channels = mlib_ImageGetChannels(dst);
		pos =
		    (type == MLIB_BYTE) ? 0 : ((type ==
		    MLIB_SHORT) ? 1 : ((type == MLIB_USHORT) ? 2 : 3));

		if ((filter == MLIB_BICUBIC || filter == MLIB_BILINEAR) &&
		    type == MLIB_INT)
			func_clip = array_func_clip_all[pws->degree * 3 + 0];
		else {
			if (filter == MLIB_BILINEAR)
				func_clip =
				    array_func_clip_all[pws->degree * 3 + 1];
			else
				func_clip =
				    array_func_clip_all[pws->degree * 3 + 2];
		}

		for (i = 0; i < srcHeight; i++) {
			lineAddr[i] = srcData;
			srcData += srcStride;
		}

		func_array = func_array_call[0][pos][channels - 1];

		if (filter == MLIB_BICUBIC)
			func_array = func_array_call[1][pos][channels - 1];

		for (i = 0; i < dstHeight; i++) {

			len = func_clip(pws, y, preShiftX, dstWidth, n);

			func_array(dstData, lineAddr, pws, len);
			dstData += dstStride;

			y += 1.0;
		}
	}
}

/* *********************************************************** */
