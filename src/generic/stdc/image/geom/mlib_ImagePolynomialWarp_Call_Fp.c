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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_Call_Fp.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_2_5_Fp(
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
	mlib_type type = mlib_ImageGetType(dst);
	mlib_u8 *srcData = mlib_ImageGetData(src);
	mlib_u8 *dstData = mlib_ImageGetData(dst);
	mlib_s32 pos, i, channels, n = pws->degree, len;
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);
	mlib_s32 srcStride = mlib_ImageGetStride(src);
	mlib_s32 dstWidth = mlib_ImageGetWidth(dst);
	mlib_s32 dstHeight = mlib_ImageGetHeight(dst);
	mlib_s32 dstStride = mlib_ImageGetStride(dst);
	mlib_d64 y = 0.5 + preShiftY;
	mlib_IPWFCall_NN *func_array_nn;
	mlib_IPWFCall_NN func_array_nn_all[] = {
/* degree = 2 */
		mlib_ImagePolynomialWarp_2_NN_S32_1,
		mlib_ImagePolynomialWarp_2_NN_S32_2,
		mlib_ImagePolynomialWarp_2_NN_S32_3,
		mlib_ImagePolynomialWarp_2_NN_S32_4,
		mlib_ImagePolynomialWarp_2_NN_D64_1,
		mlib_ImagePolynomialWarp_2_NN_D64_2,
		mlib_ImagePolynomialWarp_2_NN_D64_3,
		mlib_ImagePolynomialWarp_2_NN_D64_4,
/* degree = 3 */
		mlib_ImagePolynomialWarp_3_NN_S32_1,
		mlib_ImagePolynomialWarp_3_NN_S32_2,
		mlib_ImagePolynomialWarp_3_NN_S32_3,
		mlib_ImagePolynomialWarp_3_NN_S32_4,
		mlib_ImagePolynomialWarp_3_NN_D64_1,
		mlib_ImagePolynomialWarp_3_NN_D64_2,
		mlib_ImagePolynomialWarp_3_NN_D64_3,
		mlib_ImagePolynomialWarp_3_NN_D64_4,
/* degree = 4 */
		mlib_ImagePolynomialWarp_4_NN_S32_1,
		mlib_ImagePolynomialWarp_4_NN_S32_2,
		mlib_ImagePolynomialWarp_4_NN_S32_3,
		mlib_ImagePolynomialWarp_4_NN_S32_4,
		mlib_ImagePolynomialWarp_4_NN_D64_1,
		mlib_ImagePolynomialWarp_4_NN_D64_2,
		mlib_ImagePolynomialWarp_4_NN_D64_3,
		mlib_ImagePolynomialWarp_4_NN_D64_4,
/* degree = 5 */
		mlib_ImagePolynomialWarp_5_NN_S32_1,
		mlib_ImagePolynomialWarp_5_NN_S32_2,
		mlib_ImagePolynomialWarp_5_NN_S32_3,
		mlib_ImagePolynomialWarp_5_NN_S32_4,
		mlib_ImagePolynomialWarp_5_NN_D64_1,
		mlib_ImagePolynomialWarp_5_NN_D64_2,
		mlib_ImagePolynomialWarp_5_NN_D64_3,
		mlib_ImagePolynomialWarp_5_NN_D64_4,
	};
	mlib_IPWClipLine array_func_clip[] = {
/* degree = 2 */
		mlib_ImagePolynomialWarpClipLine_DG_2_2,
/* degree = 3 */
		mlib_ImagePolynomialWarpClipLine_DG_3_2,
/* degree = 4 */
		mlib_ImagePolynomialWarpClipLine_DG_4_2,
/* degree = 5 */
		mlib_ImagePolynomialWarpClipLine_DG_5_2,
	};
	mlib_IPWFCall func_array[] = {
		mlib_ImagePolynomialWarp_BL_F32_1,
		mlib_ImagePolynomialWarp_BL_F32_2,
		mlib_ImagePolynomialWarp_BL_F32_3,
		mlib_ImagePolynomialWarp_BL_F32_4,
		mlib_ImagePolynomialWarp_BL_D64_1,
		mlib_ImagePolynomialWarp_BL_D64_2,
		mlib_ImagePolynomialWarp_BL_D64_3,
		mlib_ImagePolynomialWarp_BL_D64_4,

		mlib_ImagePolynomialWarp_BC_F32_1,
		mlib_ImagePolynomialWarp_BC_F32_2,
		mlib_ImagePolynomialWarp_BC_F32_3,
		mlib_ImagePolynomialWarp_BC_F32_4,
		mlib_ImagePolynomialWarp_BC_D64_1,
		mlib_ImagePolynomialWarp_BC_D64_2,
		mlib_ImagePolynomialWarp_BC_D64_3,
		mlib_ImagePolynomialWarp_BC_D64_4
	};

	func_array_nn = func_array_nn_all + pws->degree * 8;

	pos = mlib_ImageGetChannels(dst) - 1;
	pos += (type == MLIB_DOUBLE) ? 4 : 0;
	pos += (filter == MLIB_BILINEAR) ? 8 : 0;
	pos += (filter == MLIB_BICUBIC) ? 16 : 0;

	if (filter == MLIB_NEAREST) {
		channels = mlib_ImageGetChannels(dst) * 4;
		channels *= (type == MLIB_DOUBLE) ? 2 : 1;

		lineAddr[0] = srcData;
		srcData -= channels;
		for (i = 0; i <= srcHeight; i++) {
			lineAddr[i + 1] = srcData;
			srcData += srcStride;
		}

		(func_array_nn[pos]) (dstData, lineAddr,
		    xCoeffs, yCoeffs, preShiftX, preShiftY,
		    srcWidth, srcHeight, dstWidth, dstHeight, dstStride);
	} else {

		pos -= 8;

		for (i = 0; i < srcHeight; i++) {
			lineAddr[i] = srcData;
			srcData += srcStride;
		}

		for (i = 0; i < dstHeight; i++) {

			len =
			    (array_func_clip[pws->degree]) (pws, y, preShiftX,
			    dstWidth, n);

			(func_array[pos]) (dstData, lineAddr, pws, len);
			dstData += dstStride;

			y += 1.0;
		}
	}
}

/* *********************************************************** */
