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

#pragma ident	"@(#)mlib_ImageMaximumFuncS32.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMaximum_S32_124 - used in mlib_ImageMaximum
 *                                  in the case of S32 type and 1, 2, 4 channels
 *      mlib_ImageMaximum_S32_3   - used in mlib_ImageMaximum
 *                                  in the case of S32 type and 3 channels
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageExtrema.h>

/* *********************************************************** */

#define	PXL_OPER_S32(res, x)                                    \
	{                                                       \
	    mlib_s32 _res, _x;                                  \
	                                                        \
	    _res = res;                                         \
	    _x = x;                                             \
	    if (_x > _res)                                      \
		res = _x;                                       \
	}

/* *********************************************************** */

void
mlib_ImageMaximum_S32_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s32 *sl;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_s32 res0, res1, res2, res3;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);

	slb /= 4;
	xsize *= nchan;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	res0 = res1 = res2 = res3 = MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i <= (xsize - 4); i += 4) {
			PXL_OPER_S32(res0, sl[i]);
			PXL_OPER_S32(res1, sl[i + 1]);
			PXL_OPER_S32(res2, sl[i + 2]);
			PXL_OPER_S32(res3, sl[i + 3]);
		}

		if (i < xsize)
			PXL_OPER_S32(res0, sl[i]);
		i++;

		if (i < xsize)
			PXL_OPER_S32(res1, sl[i]);
		i++;

		if (i < xsize)
			PXL_OPER_S32(res2, sl[i]);

		sl += slb;
	}

	res[0] = res0;
	res[1] = res1;
	res[2] = res2;
	res[3] = res3;
}

/* *********************************************************** */

void
mlib_ImageMaximum_S32_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s32 *sl;
	mlib_s32 xsize, ysize, slb;
	mlib_s32 res0, res1, res2;
	mlib_s32 i, j;

	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);

	slb /= 4;
	xsize *= 3;

	if (slb == xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	res0 = res1 = res2 = MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i <= (xsize - 3); i += 3) {
			PXL_OPER_S32(res0, sl[i]);
			PXL_OPER_S32(res1, sl[i + 1]);
			PXL_OPER_S32(res2, sl[i + 2]);
		}

		if (i < xsize)
			PXL_OPER_S32(res0, sl[i]);
		i++;

		if (i < xsize)
			PXL_OPER_S32(res1, sl[i]);

		sl += slb;
	}

	res[0] = res0;
	res[1] = res1;
	res[2] = res2;
}

/* *********************************************************** */
