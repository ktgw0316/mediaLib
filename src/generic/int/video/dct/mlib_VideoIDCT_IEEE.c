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

#pragma ident	"@(#)mlib_VideoIDCT_IEEE.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *    mlib_VideoIDCT_IEEE_S16_S16 - Computes the inverse IDCT in the inter mode.
 *
 *    This IDCT function is IEEE-1180 compliant
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoIDCT_IEEE_S16_S16(mlib_s16       *block,
 *                                              const mlib_s16 *coeffs);
 *
 * ARGUMENT
 *      block           Pointer to an 8x8 motion-compensated block which is the
 *                      difference between the reference block and current block
 *      coeffs          Pointer to the input DCT coefficients
 *
 * DESCRIPTION
 *
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *
 *             1   7   7                        pi*(2*x+1)*u      pi*(2*y+1)*v
 *   f(x, y) = ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *             4  u = 0 v = 0                       16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT_IEEE_S16_S16 = __mlib_VideoIDCT_IEEE_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT_IEEE_S16_S16) mlib_VideoIDCT_IEEE_S16_S16
	__attribute__((weak, alias("__mlib_VideoIDCT_IEEE_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* 2048*sqrt(2)*cos(1*pi/16) */
#define	W1	2841
/* 2048*sqrt(2)*cos(2*pi/16) */
#define	W2	2676
/* 2048*sqrt(2)*cos(3*pi/16) */
#define	W3	2408
/* 2048*sqrt(2)*cos(5*pi/16) */
#define	W5	1609
/* 2048*sqrt(2)*cos(6*pi/16) */
#define	W6	1108
/* 2048*sqrt(2)*cos(7*pi/16) */
#define	W7	565

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT_IEEE_S16_S16(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_s32 i;
	const mlib_s16 *blkin;
	mlib_s16 *blkout;
	mlib_s32 blkbuf[64], *blk;
	mlib_s32 x0, x1, x2, x3, x4, x5, x6, x7, x8;

/* row-wise iDCT */
	for (i = 0; i < 8; i++) {
		blkin = coeffs + 8 * i;
		blk = blkbuf + 8 * i;
/* shortcut */

		if (!((x1 = blkin[4] << 11) | (x2 = blkin[6]) | (x3 =
			blkin[2]) | (x4 = blkin[1]) | (x5 = blkin[7]) | (x6 =
			blkin[5]) | (x7 = blkin[3]))) {
			blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] =
				blk[6] = blk[7] = blkin[0] << 3;
			continue;
		}

/* for proper rounding in the fourth stage */
		x0 = (blkin[0] << 11) + 128;

/* first stage */
		x8 = W7 * (x4 + x5);
		x4 = x8 + (W1 - W7) * x4;
		x5 = x8 - (W1 + W7) * x5;
		x8 = W3 * (x6 + x7);
		x6 = x8 - (W3 - W5) * x6;
		x7 = x8 - (W3 + W5) * x7;

/* second stage */
		x8 = x0 + x1;
		x0 -= x1;
		x1 = W6 * (x3 + x2);
		x2 = x1 - (W2 + W6) * x2;
		x3 = x1 + (W2 - W6) * x3;
		x1 = x4 + x6;
		x4 -= x6;
		x6 = x5 + x7;
		x5 -= x7;

/* third stage */
		x7 = x8 + x3;
		x8 -= x3;
		x3 = x0 + x2;
		x0 -= x2;
		x2 = (181 * (x4 + x5) + 128) >> 8;
		x4 = (181 * (x4 - x5) + 128) >> 8;

/* fourth stage */
		blk[0] = (x7 + x1) >> 8;
		blk[1] = (x3 + x2) >> 8;
		blk[2] = (x0 + x4) >> 8;
		blk[3] = (x8 + x6) >> 8;
		blk[4] = (x8 - x6) >> 8;
		blk[5] = (x0 - x4) >> 8;
		blk[6] = (x3 - x2) >> 8;
		blk[7] = (x7 - x1) >> 8;
	}

/* column-wise iDCT */

	for (i = 0; i < 8; i++) {
		blk = blkbuf + i;
		blkout = block + i;

/* shortcut */

		if (!((x1 = (blk[8 * 4] << 8)) | (x2 = blk[8 * 6]) | (x3 =
			blk[8 * 2]) | (x4 = blk[8 * 1]) | (x5 =
			blk[8 * 7]) | (x6 = blk[8 * 5]) | (x7 = blk[8 * 3]))) {
			blkout[8 * 0] = blkout[8 * 1] = blkout[8 * 2] =
				blkout[8 * 3] = blkout[8 * 4] = blkout[8 * 5] =
				blkout[8 * 6] = blkout[8 * 7] =
				(blk[8 * 0] + 32) >> 6;
			continue;
		}

		x0 = (blk[8 * 0] << 8) + 8192;

/* first stage */
		x8 = W7 * (x4 + x5) + 4;
		x4 = (x8 + (W1 - W7) * x4) >> 3;
		x5 = (x8 - (W1 + W7) * x5) >> 3;
		x8 = W3 * (x6 + x7) + 4;
		x6 = (x8 - (W3 - W5) * x6) >> 3;
		x7 = (x8 - (W3 + W5) * x7) >> 3;

/* second stage */
		x8 = x0 + x1;
		x0 -= x1;
		x1 = W6 * (x3 + x2) + 4;
		x2 = (x1 - (W2 + W6) * x2) >> 3;
		x3 = (x1 + (W2 - W6) * x3) >> 3;
		x1 = x4 + x6;
		x4 -= x6;
		x6 = x5 + x7;
		x5 -= x7;

/* third stage */
		x7 = x8 + x3;
		x8 -= x3;
		x3 = x0 + x2;
		x0 -= x2;
		x2 = (181 * (x4 + x5) + 128) >> 8;
		x4 = (181 * (x4 - x5) + 128) >> 8;

/* fourth stage */
		blkout[8 * 0] = (x7 + x1) >> 14;
		blkout[8 * 1] = (x3 + x2) >> 14;
		blkout[8 * 2] = (x0 + x4) >> 14;
		blkout[8 * 3] = (x8 + x6) >> 14;
		blkout[8 * 4] = (x8 - x6) >> 14;
		blkout[8 * 5] = (x0 - x4) >> 14;
		blkout[8 * 6] = (x3 - x2) >> 14;
		blkout[8 * 7] = (x7 - x1) >> 14;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
