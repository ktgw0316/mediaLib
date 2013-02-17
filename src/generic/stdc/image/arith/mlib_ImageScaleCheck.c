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

#pragma ident	"@(#)mlib_ImageScaleCheck.c	9.4	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale_CheckForVIS - check constants for VIS code of
 *                                    ImageScale function and return
 *                                    new shift on success
 *
 * RETURN VALUES
 *      <= (-2)   OUTOFRANGE for mlib_ImageScale function
 *      == (-1)   VIS code can't be used
 *      >= 0      it is possible to use VIS code
 *                with new shift equal to returned value
 *                Note: shift >= 256 on special
 *                      fast case of scale from U8 to U8 and
 *                      (shift - 256) is real shift for such case
 * FUNCTION
 *      mlib_ImageScale_CheckForSSE2 - check constants for SSE2 code of
 *                                     ImageScale function and return
 *                                     new shift on success
 *
 * RETURN VALUES
 *      <= (-2)   OUTOFRANGE for mlib_ImageScale function
 *      == (-1)   SSE2 code can't be used
 *      >= 0      it is possible to use SSE2 code
 *                with new shift equal to returned value
 *                Note: shift >= 256 on special
 *                      fast case of scale from U8 to U8 and
 *                      (shift - 256) is real shift for such case
 */
#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageScale2.h>

/* *********************************************************** */

mlib_s32
mlib_ImageScale_CheckForVIS(
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift,
    mlib_type stype,
    mlib_type dtype,
    mlib_s32 nchan)
{
	mlib_s32 minshift = 0, maxshift = 31;
	mlib_d64 am, bm, maxa, maxb;
	mlib_s32 k;

	if (dtype != MLIB_BYTE && dtype != MLIB_SHORT &&
	    dtype != MLIB_USHORT && dtype != MLIB_INT) {
/* no VIS version */
		return (-1);
	}

	if (dtype != MLIB_USHORT) {
		maxa = (mlib_u32)abs(alpha[0]);
		maxb = (mlib_u32)abs(beta[0]);
		for (k = 1; k < nchan; k++) {
			am = (mlib_u32)abs(alpha[k]);
			bm = (mlib_u32)abs(beta[k]);

			if (am > maxa)
				maxa = am;

			if (bm > maxb)
				maxb = bm;
		}
	} else {
		maxa = (mlib_u32)abs(alpha[0]);
		maxb = mlib_fabs((beta[0] - 32768.0));
		for (k = 1; k < nchan; k++) {
			am = (mlib_u32)abs(alpha[k]);
			bm = mlib_fabs((beta[k] - 32768.0));

			if (am > maxa)
				maxa = am;

			if (bm > maxb)
				maxb = bm;
		}
	}

	maxb += 0.5;

	if (stype == MLIB_BYTE) {

		if (dtype == MLIB_BYTE) {
			minshift = 8;
			maxshift = 23;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 1;
			maxshift = 16;
		} else {
/*
 * WARNING! No VIS version for U8->S32.                  *
 * See function mlib_v_ImageScale (line 524) for change. *
 */
			return (-1);
		}

		if (shift < minshift) {
			if (maxa >= 0x8000)
/* OUTOFRANGE */
				return (-2);
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		while ((shift > 15) ||
		    (maxa >= 0x8000) ||
		    ((256 * maxa + maxb * (1u << shift)) >= 0x80000000u)) {
			shift--;
			maxa *= 0.5;

			if (shift < 8)
/* OUTOFRANGE */
				return (-2);
		}

		if (dtype == MLIB_BYTE) {
			if (maxa + maxb * (1u << (shift - 8)) < 0x8000) {
/* special fast version */
				shift += 256;
			}
		}
	} else if (stype == MLIB_SHORT) {

		if (dtype == MLIB_BYTE) {
			minshift = 8;
			maxshift = 23;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 1;
			maxshift = 16;
		}

		if (shift < minshift) {
			if (maxa >= 0x8000)
/* OUTOFRANGE */
				return (-2);
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		while ((shift > maxshift) ||
		    (maxa >= 0x8000) ||
		    (32768 * maxa + maxb * (1u << shift) >= 0x80000000u)) {
			shift--;
			maxa *= 0.5;

			if (shift < 16)
/* OUTOFRANGE */
				return (-2);
		}
	} else if (stype == MLIB_USHORT) {

		if (dtype == MLIB_BYTE) {
			minshift = 8;
			maxshift = 23;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 1;
			maxshift = 16;
		}

		if (shift < minshift) {
			if (maxa >= 0x8000)
/* OUTOFRANGE */
				return (-2);
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		while ((shift > maxshift) ||
		    (maxa >= 0x8000) ||
		    (65536 * maxa + maxb * (1u << shift) >= 0x80000000u)) {
			shift--;
			maxa *= 0.5;

			if (shift < 16)
/* OUTOFRANGE */
				return (-2);
		}
	} else if (stype == MLIB_INT) {
/* mlib_s32 a = 0; */

		if (dtype == MLIB_BYTE) {
			minshift = 24;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 17;
		} else {
/* no VIS version for U8->S32 */
			return (-1);
		}

		if (shift < minshift) {
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		if ((2 * maxa) >= (1 << (shift - 16))) {
/* no VIS version due to precision reasons */
			return (-1);
		}

/*
 *        if (maxa >= 0x8000) {
 *          mlib_u32 del = (1 << (shift-16)) - (maxa << 1);
 *          while ( maxa >= 0x8000 && (1 << (a+16)) >= del ) {
 *            maxa >>= 1;
 *            (*a++);
 *          }
 *        }
 *
 *        shift -= a;
 */

		if ((maxa >= 0x8000) ||
		    (32768 * maxa + maxb * (1u << (shift - 16)) >=
		    0x80000000u)) {
/* no VIS version */
			return (-1);
		}
	}

/* VIS version can be used */
	return (shift);
}

/* *********************************************************** */

mlib_s32
mlib_ImageScale_CheckForSSE2(
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift,
    mlib_type stype,
    mlib_type dtype,
    mlib_s32 nchan)
{
	mlib_s32 minshift = 0, maxshift = 31;
	mlib_d64 am, bm, maxa, maxb;
	mlib_s32 k;

	if (dtype != MLIB_BYTE && dtype != MLIB_SHORT &&
	    dtype != MLIB_USHORT && dtype != MLIB_INT) {
/* SSE2 FLOAT version were implemented in FP files */
		return (-1);
	}

	if (dtype != MLIB_USHORT) {
		maxa = (mlib_u32)abs(alpha[0]);
		maxb = (mlib_u32)abs(beta[0]);
		for (k = 1; k < nchan; k++) {
			am = (mlib_u32)abs(alpha[k]);
			bm = (mlib_u32)abs(beta[k]);

			if (am > maxa)
				maxa = am;

			if (bm > maxb)
				maxb = bm;
		}
	} else {
		maxa = (mlib_u32)abs(alpha[0]);
		maxb = mlib_fabs((beta[0] - 32768.0));
		for (k = 1; k < nchan; k++) {
			am = (mlib_u32)abs(alpha[k]);
			bm = mlib_fabs((beta[k] - 32768.0));

			if (am > maxa)
				maxa = am;

			if (bm > maxb)
				maxb = bm;
		}
	}

	maxb += 0.5;

	if (stype == MLIB_BYTE) {

		if (dtype == MLIB_BYTE) {
			minshift = 8;
			maxshift = 23;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 1;
			maxshift = 16;
		} else {

			if ((maxa >= 0x8000) ||
				(256 * maxa + maxb *
				(1u << shift) >= 0x80000000u)) {
/* overflow in SSE2 version  */
				return (-1);
			}
			return (shift);
		}

		if (shift < minshift) {
			if (maxa >= 0x8000)
/* OUTOFRANGE */
				return (-2);
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		while ((shift > 15) ||
		    (maxa >= 0x8000) ||
		    ((256 * maxa + maxb * (1u << shift)) >=
				0x80000000u)) {
			shift--;
			maxa *= 0.5;

			if (shift < 8)
/* OUTOFRANGE */
				return (-2);
		}

		if (dtype == MLIB_BYTE) {
			if (shift > 8) {
				if (maxa + maxb * (1u << (shift - 9)) <
					0x8000) {
/* special fast version */
					shift += 256;
				}
			}
		}

	} else if (stype == MLIB_SHORT) {

		if (dtype == MLIB_BYTE) {
			minshift = 8;
			maxshift = 23;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 1;
			maxshift = 16;
		}

		if (shift < minshift) {
			if (maxa >= 0x8000)
/* OUTOFRANGE */
				return (-2);
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		while ((shift > maxshift) ||
		    (maxa >= 0x8000) ||
		    (32768 * maxa + maxb * (1u << shift) >= 0x80000000u)) {
			shift--;
			maxa *= 0.5;

			if (shift < 16)
/* OUTOFRANGE */
				return (-2);
		}
	} else if (stype == MLIB_USHORT) {

		if (dtype == MLIB_BYTE) {
			minshift = 8;
			maxshift = 23;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 1;
			maxshift = 16;
		}

		if (shift < minshift) {
			if (maxa >= 0x8000)
/* OUTOFRANGE */
				return (-2);
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		while ((shift > maxshift) ||
		    (maxa >= 0x8000) ||
		    (65536 * maxa + maxb * (1u << shift) >= 0x80000000u)) {
			shift--;
			maxa *= 0.5;

			if (shift < 16)
/* OUTOFRANGE */
				return (-2);
		}
	} else if (stype == MLIB_INT) {
/* mlib_s32 a = 0; */

		if (dtype == MLIB_BYTE) {
			minshift = 24;
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
			minshift = 17;
		} else {
			minshift = 16;
		}

		if (shift < minshift) {
			maxa *= 1u << (minshift - shift);
			shift = minshift;
		}

		if ((2 * maxa) >= (1 << (shift - 16))) {
/* no SSE2 version due to precision reasons */
			return (-1);
		}

		if ((maxa >= 0x8000) ||
		    (32768 * maxa + maxb * (1u << (shift - 16)) >=
		    0x80000000u)) {
/* overflow in SSE2 version */
			return (-1);
		}
	}

/* SSE2 version can be used */
	return (shift);
}

/* *********************************************************** */
