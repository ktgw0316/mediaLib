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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_1.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

mlib_status
mlib_ImagePolynomialWarp_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_d64 preShiftX,
    mlib_d64 preShiftY,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_d64 preScaleX,
    mlib_d64 preScaleY,
    mlib_d64 postScaleX,
    mlib_d64 postScaleY,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_type type;
	mlib_d64 a, b, c, d, ad, bc, div, tx, ty;
	mlib_d64 mtx[6];
	mlib_d64 eps = (1.0 / (1 << 12) / (1 << 12));

	a = xCoeffs[1] * preScaleX * postScaleX;
	b = xCoeffs[2] * preScaleY * postScaleX;
	c = yCoeffs[1] * preScaleX * postScaleY;
	d = yCoeffs[2] * preScaleY * postScaleY;

	tx = xCoeffs[0] + xCoeffs[1] * preShiftX * preScaleX +
	    xCoeffs[2] * preScaleY * preShiftY;
	ty = yCoeffs[0] + yCoeffs[1] * preShiftX * preScaleX +
	    yCoeffs[2] * preScaleY * preShiftY;
	tx = tx * postScaleX - postShiftX;
	ty = ty * postScaleY - postShiftY;

	ad = a * d;
	bc = b * c;
	div = ad - bc;

/* correct a little matrix */
	if (mlib_fabs(div) < eps * eps) {
		if (mlib_fabs(ad) > mlib_fabs(bc)) {
			a *= (1 + eps);
		} else if (bc == 0) {
			a = d = eps;
		} else {
			b *= (1 + eps);
		}

		div = a * d - b * c;
	}

	div = 1.0 / div;
	a *= div;
	b *= div;
	c *= div;
	d *= div;

	mtx[0] = d;
	mtx[1] = -b;
	mtx[2] = -d * tx + b * ty;
	mtx[3] = -c;
	mtx[4] = a;
	mtx[5] = c * tx - a * ty;

	type = mlib_ImageGetType(dst);

	if (type != MLIB_FLOAT && type != MLIB_DOUBLE) {
		return (__mlib_ImageAffine(dst, src, mtx, filter, edge));
	}

	return (__mlib_ImageAffine_Fp(dst, src, mtx, filter, edge));
}

/* *********************************************************** */
