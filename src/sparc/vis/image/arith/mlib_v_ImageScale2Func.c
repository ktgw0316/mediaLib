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

#pragma ident	"@(#)mlib_v_ImageScale2Func.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale2_fast  - fast VIS version of ImageScale2 function by
 *                               calling of ImageScale function
 */

#include <mlib_image.h>
#include <mlib_ImageScale2.h>
#include <mlib_v_ImageScale.h>

/* *********************************************************** */

mlib_status
mlib_ImageScale2_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type stype, dtype;
	mlib_d64 rbeta[4];
	mlib_d64 ad[2], bd[2];
	mlib_s32 *ialpha = (mlib_s32 *)ad;
	mlib_s32 *ibeta = (mlib_s32 *)bd;
	mlib_s16 *salpha = (mlib_s16 *)ad;
	mlib_s16 *sbeta = (mlib_s16 *)bd;
	mlib_s32 k, nchan, old_shift, fast_u8_u8 = 0, shift = -1;

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (dtype == MLIB_FLOAT || dtype == MLIB_DOUBLE)
		return (MLIB_FAILURE);

	if (stype == MLIB_BYTE) {
		if (dtype == MLIB_BYTE || dtype == MLIB_SHORT ||
		    dtype == MLIB_USHORT)
			shift = 15;
	}

	if (stype == MLIB_SHORT) {
		if (dtype == MLIB_BYTE)
			shift = 23;
		if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT))
			shift = 16;
	}

	if (stype == MLIB_USHORT) {
		if (dtype == MLIB_BYTE)
			shift = 23;
		if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT))
			shift = 16;
	}

	if (stype == MLIB_INT) {
		if (dtype == MLIB_BYTE || dtype == MLIB_SHORT ||
		    dtype == MLIB_USHORT)
			shift = 31;
	}

/* no VIS version for such types */
	if (shift < 0)
		return (MLIB_FAILURE);

/* for rounding to nearest in integer shifting */
	for (k = 0; k < nchan; k++)
		rbeta[k] = beta[k] + 0.5;

	for (k = 0; k < nchan; k++) {
		mlib_d64 fa, fb;

		fa = alpha[k] * (1u << shift) + ((alpha[k] >= 0) ? 0.5 : -0.5);
		fb = rbeta[k] + ((rbeta[k] >= 0) ? 1.0 : -1.0);

		if (fa < MLIB_S32_MIN || fa > MLIB_S32_MAX ||
		    fb < MLIB_S32_MIN || fb > MLIB_S32_MAX) {
/* Overflow */
			return (MLIB_FAILURE);
		}

		ialpha[k] = (mlib_s32)fa;
		ibeta[k] = (mlib_s32)fb;
	}

	old_shift = shift;
	shift =
	    mlib_ImageScale_CheckForVIS(ialpha, ibeta, shift, stype, dtype,
	    nchan);

/* no VIS version for such constants */
	if (shift < 0)
		return (MLIB_FAILURE);

/* special fast version for U8 -> U8 */
	if (shift >= 256) {
		fast_u8_u8 = 1;
		shift -= 256;
	}

/* recalculate alpha and beta for VIS */

	if (shift < old_shift) {
		for (k = 0; k < nchan; k++)
			salpha[k] = ialpha[k] >> (old_shift - shift);
	} else {
		for (k = 0; k < nchan; k++)
			salpha[k] = ialpha[k] << (shift - old_shift);
	}

	for (k = nchan; k < 4; k++)
		salpha[k] = salpha[k - nchan];

	if (stype == MLIB_INT)
		shift -= 16;

	if (fast_u8_u8) {

		for (k = 0; k < nchan; k++) {
			mlib_d64 fb =
			    rbeta[k] * (1u << (shift - 8)) + ((rbeta[k] >=
			    0) ? 0.5 : -0.5);
			sbeta[k] = (mlib_s32)fb;
		}

		for (k = nchan; k < 4; k++)
			sbeta[k] = sbeta[k - nchan];

	} else {

		if (dtype != MLIB_USHORT) {
			for (k = 0; k < nchan; k++) {
				mlib_d64 fb =
				    rbeta[k] * (1u << shift) + ((rbeta[k] >=
				    0) ? 0.5 : -0.5);
				ibeta[k] = (mlib_s32)fb;
			}

		} else {
			for (k = 0; k < nchan; k++) {
				mlib_d64 fb =
				    (rbeta[k] - 32768.0) * (1u << shift) +
				    ((rbeta[k] >= 32768.0) ? 0.5 : -0.5);
				ibeta[k] = (mlib_s32)fb;
			}
		}

		if (stype == MLIB_USHORT)
			for (k = 0; k < nchan; k++)
				ibeta[k] += (salpha[k] << 15);
		for (k = nchan; k < 4; k++)
			ibeta[k] = ibeta[k - nchan];
	}

	return mlib_ImageScale_VIS(dst, src, ad[0], bd[0], bd[1], shift,
	    fast_u8_u8);
}

/* *********************************************************** */
