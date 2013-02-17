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
#pragma ident	"@(#)mlib_i_VideoDCT16x16_S16_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDCT16x16_S16_S16 - Computes the forward DCT in the inter mode.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoDCT16x16_S16_S16(mlib_s16       *coeffs,
 *                                           const mlib_s16 *block)
 *
 * ARGUMENTS
 *    coeffs          Pointer to the output DCT coefficients
 *    block           Pointer to an 16x16 motion-compensated block which is the
 *                    difference between the reference block and current block
 *
 *    coeffs and  block both must be 8-byte aligned
 *
 *    Input block values should be in [-128, 127] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1, ... ,15; y = 0, 1, ... ,15
 *   F = F(u, v)  u = 0, 1, ... ,15; v = 0, 1, ... ,15
 *
 *             1             15  15             pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             8            x = 0 y = 0             32                  32
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT16x16_S16_S16 = __mlib_VideoDCT16x16_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT16x16_S16_S16) mlib_VideoDCT16x16_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT16x16_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */


#define	ROUND(a)	((a + (1<<12)) >> 13)

/* *********************************************************** */
static mlib_u16 mlib_dct16tab[16] = {
	5793,
	3135,
	7568,
	1598,
	8035,
	6811,
	4551,
	803,
	8153,
	7839,
	2378,
	7225,
	3862,
	6333,
	5197,
	1024,
};


/* *********************************************************** */

mlib_status
__mlib_VideoDCT16x16_S16_S16(
	mlib_s16 *coeffs,
	const mlib_s16 *block)
{
	mlib_s32 j;
	mlib_s16 val_n[16 * 16];
	mlib_s32 tmp_1_0, tmp_1_1, tmp_1_2, tmp_1_3, tmp_1_4;
	mlib_s32 tmp_1_5, tmp_1_6, tmp_1_7, tmp_1_8, tmp_1_9;
	mlib_s32 tmp_1_10, tmp_1_11, tmp_1_12, tmp_1_13, tmp_1_14, tmp_1_15;

	mlib_s32 tmp_2_0, tmp_2_1, tmp_2_2, tmp_2_3, tmp_2_4;
	mlib_s32 tmp_2_5, tmp_2_6, tmp_2_7, tmp_2_8, tmp_2_9;
	mlib_s32 tmp_2_10, tmp_2_11, tmp_2_12, tmp_2_13, tmp_2_14, tmp_2_15;

	mlib_u16 COS_4 = mlib_dct16tab[0];
	mlib_u16 SIN_8 = mlib_dct16tab[1];
	mlib_u16 COS_8 = mlib_dct16tab[2];
	mlib_u16 SIN_16 = mlib_dct16tab[3];
	mlib_u16 COS_16 = mlib_dct16tab[4];
	mlib_u16 COS_3_16 = mlib_dct16tab[5];
	mlib_u16 SIN_3_16 = mlib_dct16tab[6];

	mlib_u16 SIN_32 = mlib_dct16tab[7];
	mlib_u16 COS_32 = mlib_dct16tab[8];
	mlib_u16 COS_3_32 = mlib_dct16tab[9];
	mlib_u16 SIN_3_32 = mlib_dct16tab[10];
	mlib_u16 COS_5_32 = mlib_dct16tab[11];
	mlib_u16 SIN_5_32 = mlib_dct16tab[12];
	mlib_u16 COS_7_32 = mlib_dct16tab[13];
	mlib_u16 SIN_7_32 = mlib_dct16tab[14];

/* first column based 1-D 16x16 DCT */

	for (j = 0; j < 16; j++) {

/* first butter-fly */
		tmp_1_0 = block[j + 16 * 0] + block[j + 16 * 15];
		tmp_1_15 = block[j + 16 * 0] - block[j + 16 * 15];

		tmp_1_1 = block[j + 16 * 1] + block[j + 16 * 14];
		tmp_1_14 = block[j + 16 * 1] - block[j + 16 * 14];

		tmp_1_2 = block[j + 16 * 2] + block[j + 16 * 13];
		tmp_1_13 = block[j + 16 * 2] - block[j + 16 * 13];

		tmp_1_3 = block[j + 16 * 3] + block[j + 16 * 12];
		tmp_1_12 = block[j + 16 * 3] - block[j + 16 * 12];

		tmp_1_4 = block[j + 16 * 4] + block[j + 16 * 11];
		tmp_1_11 = block[j + 16 * 4] - block[j + 16 * 11];

		tmp_1_5 = block[j + 16 * 5] + block[j + 16 * 10];
		tmp_1_10 = block[j + 16 * 5] - block[j + 16 * 10];

		tmp_1_6 = block[j + 16 * 6] + block[j + 16 * 9];
		tmp_1_9 = block[j + 16 * 6] - block[j + 16 * 9];

		tmp_1_7 = block[j + 16 * 7] + block[j + 16 * 8];
		tmp_1_8 = block[j + 16 * 7] - block[j + 16 * 8];

/* second butter-fly */
		tmp_2_0 = tmp_1_0 + tmp_1_7;
		tmp_2_1 = tmp_1_1 + tmp_1_6;
		tmp_2_2 = tmp_1_2 + tmp_1_5;
		tmp_2_3 = tmp_1_3 + tmp_1_4;
		tmp_2_4 = tmp_1_3 - tmp_1_4;
		tmp_2_5 = tmp_1_2 - tmp_1_5;
		tmp_2_6 = tmp_1_1 - tmp_1_6;
		tmp_2_7 = tmp_1_0 - tmp_1_7;
		tmp_2_8 = tmp_1_8;
		tmp_2_9 = tmp_1_9;
		tmp_2_10 = ROUND((tmp_1_13 - tmp_1_10) * COS_4);
		tmp_2_11 = ROUND((tmp_1_12 - tmp_1_11) * COS_4);
		tmp_2_12 = ROUND((tmp_1_11 + tmp_1_12) * COS_4);
		tmp_2_13 = ROUND((tmp_1_10 + tmp_1_13) * COS_4);
		tmp_2_14 = tmp_1_14;
		tmp_2_15 = tmp_1_15;

/* third butter-fly */
		tmp_1_0 = tmp_2_0 + tmp_2_3;
		tmp_1_1 = tmp_2_1 + tmp_2_2;
		tmp_1_2 = tmp_2_1 - tmp_2_2;
		tmp_1_3 = tmp_2_0 - tmp_2_3;
		tmp_1_4 = tmp_2_4;
		tmp_1_5 = ROUND((tmp_2_6 - tmp_2_5) * COS_4);
		tmp_1_6 = ROUND((tmp_2_6 + tmp_2_5) * COS_4);
		tmp_1_7 = tmp_2_7;
		tmp_1_8 = tmp_2_8 + tmp_2_11;
		tmp_1_9 = tmp_2_9 + tmp_2_10;
		tmp_1_10 = tmp_2_9 - tmp_2_10;
		tmp_1_11 = tmp_2_8 - tmp_2_11;
		tmp_1_12 = tmp_2_15 - tmp_2_12;
		tmp_1_13 = tmp_2_14 - tmp_2_13;
		tmp_1_14 = tmp_2_14 + tmp_2_13;
		tmp_1_15 = tmp_2_15 + tmp_2_12;

/* fourth butter-fly */
		tmp_2_0 = ROUND((tmp_1_0 + tmp_1_1) * COS_4);
		tmp_2_1 = ROUND((tmp_1_0 - tmp_1_1) * COS_4);
		tmp_2_2 = ROUND(tmp_1_2 * SIN_8 + tmp_1_3 * COS_8);
		tmp_2_3 = ROUND(tmp_1_3 * SIN_8 - tmp_1_2 * COS_8);
		tmp_2_4 = tmp_1_4 + tmp_1_5;
		tmp_2_5 = tmp_1_4 - tmp_1_5;
		tmp_2_6 = tmp_1_7 - tmp_1_6;
		tmp_2_7 = tmp_1_7 + tmp_1_6;
		tmp_2_8 = tmp_1_8;
		tmp_2_9 = ROUND(tmp_1_14 * SIN_8 - tmp_1_9 * COS_8);
		tmp_2_10 = ROUND(tmp_1_10 * SIN_8 + tmp_1_13 * COS_8);
		tmp_2_11 = tmp_1_11;
		tmp_2_12 = tmp_1_12;
		tmp_2_13 = ROUND(tmp_1_13 * SIN_8 - tmp_1_10 * COS_8);
		tmp_2_14 = ROUND(tmp_1_9 * SIN_8 + tmp_1_14 * COS_8);
		tmp_2_15 = tmp_1_15;

/* fifth butter-fly */
		tmp_1_4 = ROUND(tmp_2_4 * SIN_16 + tmp_2_7 * COS_16);
		tmp_1_5 = ROUND(tmp_2_5 * COS_3_16 + tmp_2_6 * SIN_3_16);
		tmp_1_6 = ROUND(tmp_2_6 * COS_3_16 - tmp_2_5 * SIN_3_16);
		tmp_1_7 = ROUND(tmp_2_7 * SIN_16 - tmp_2_4 * COS_16);
		tmp_1_8 = tmp_2_8 + tmp_2_9;
		tmp_1_9 = tmp_2_8 - tmp_2_9;
		tmp_1_10 = tmp_2_11 + tmp_2_10;
		tmp_1_11 = tmp_2_11 - tmp_2_10;
		tmp_1_12 = tmp_2_12 + tmp_2_13;
		tmp_1_13 = tmp_2_12 - tmp_2_13;
		tmp_1_14 = tmp_2_15 - tmp_2_14;
		tmp_1_15 = tmp_2_15 + tmp_2_14;

/* sixth butter-fly */
		val_n[j + 16 * 0] = tmp_2_0;
		val_n[j + 16 * 8] = tmp_2_1;
		val_n[j + 16 * 4] = tmp_2_2;
		val_n[j + 16 * 12] = tmp_2_3;
		val_n[j + 16 * 2] = tmp_1_4;
		val_n[j + 16 * 10] = tmp_1_5;
		val_n[j + 16 * 6] = tmp_1_6;
		val_n[j + 16 * 14] = tmp_1_7;
		val_n[j + 16 * 1] = ROUND((tmp_1_8 * SIN_32       \
					+ tmp_1_15 * COS_32));
		val_n[j + 16 * 9] = ROUND((tmp_1_9 * COS_7_32 	  \
					+ tmp_1_14 * SIN_7_32));
		val_n[j + 16 * 5] = ROUND((tmp_1_10 * SIN_5_32    \
					+ tmp_1_13 * COS_5_32));
		val_n[j + 16 * 13] = ROUND((tmp_1_11 * COS_3_32   \
					+ tmp_1_12 * SIN_3_32));
		val_n[j + 16 * 3] = ROUND((tmp_1_12 * COS_3_32    \
					- tmp_1_11 * SIN_3_32));
		val_n[j + 16 * 11] = ROUND((tmp_1_13 * SIN_5_32   \
					- tmp_1_10 * COS_5_32));
		val_n[j + 16 * 7] = ROUND((tmp_1_14 * COS_7_32    \
					- tmp_1_9 * SIN_7_32));
		val_n[j + 16 * 15] = ROUND((tmp_1_15 * SIN_32     \
					- tmp_1_8 * COS_32));
	}

/* then row based 1-D 16x16 DCT */

	for (j = 0; j < 16; j++) {

/* first butter-fly */
		tmp_1_0 = val_n[j * 16 + 0] + val_n[j * 16 + 15];
		tmp_1_15 = val_n[j * 16 + 0] - val_n[j * 16 + 15];

		tmp_1_1 = val_n[j * 16 + 1] + val_n[j * 16 + 14];
		tmp_1_14 = val_n[j * 16 + 1] - val_n[j * 16 + 14];

		tmp_1_2 = val_n[j * 16 + 2] + val_n[j * 16 + 13];
		tmp_1_13 = val_n[j * 16 + 2] - val_n[j * 16 + 13];

		tmp_1_3 = val_n[j * 16 + 3] + val_n[j * 16 + 12];
		tmp_1_12 = val_n[j * 16 + 3] - val_n[j * 16 + 12];

		tmp_1_4 = val_n[j * 16 + 4] + val_n[j * 16 + 11];
		tmp_1_11 = val_n[j * 16 + 4] - val_n[j * 16 + 11];

		tmp_1_5 = val_n[j * 16 + 5] + val_n[j * 16 + 10];
		tmp_1_10 = val_n[j * 16 + 5] - val_n[j * 16 + 10];

		tmp_1_6 = val_n[j * 16 + 6] + val_n[j * 16 + 9];
		tmp_1_9 = val_n[j * 16 + 6] - val_n[j * 16 + 9];

		tmp_1_7 = val_n[j * 16 + 7] + val_n[j * 16 + 8];
		tmp_1_8 = val_n[j * 16 + 7] - val_n[j * 16 + 8];

/* second butter-fly */
		tmp_2_0 = tmp_1_0 + tmp_1_7;
		tmp_2_1 = tmp_1_1 + tmp_1_6;
		tmp_2_2 = tmp_1_2 + tmp_1_5;
		tmp_2_3 = tmp_1_3 + tmp_1_4;
		tmp_2_4 = tmp_1_3 - tmp_1_4;
		tmp_2_5 = tmp_1_2 - tmp_1_5;
		tmp_2_6 = tmp_1_1 - tmp_1_6;
		tmp_2_7 = tmp_1_0 - tmp_1_7;
		tmp_2_8 = tmp_1_8;
		tmp_2_9 = tmp_1_9;
		tmp_2_10 = ROUND((tmp_1_13 - tmp_1_10) * COS_4);
		tmp_2_11 = ROUND((tmp_1_12 - tmp_1_11) * COS_4);
		tmp_2_12 = ROUND((tmp_1_11 + tmp_1_12) * COS_4);
		tmp_2_13 = ROUND((tmp_1_10 + tmp_1_13) * COS_4);
		tmp_2_14 = tmp_1_14;
		tmp_2_15 = tmp_1_15;

/* third butter-fly */
		tmp_1_0 = tmp_2_0 + tmp_2_3;
		tmp_1_1 = tmp_2_1 + tmp_2_2;
		tmp_1_2 = tmp_2_1 - tmp_2_2;
		tmp_1_3 = tmp_2_0 - tmp_2_3;
		tmp_1_4 = tmp_2_4;
		tmp_1_5 = ROUND((tmp_2_6 - tmp_2_5) * COS_4);
		tmp_1_6 = ROUND((tmp_2_6 + tmp_2_5) * COS_4);
		tmp_1_7 = tmp_2_7;
		tmp_1_8 = tmp_2_8 + tmp_2_11;
		tmp_1_9 = tmp_2_9 + tmp_2_10;
		tmp_1_10 = tmp_2_9 - tmp_2_10;
		tmp_1_11 = tmp_2_8 - tmp_2_11;
		tmp_1_12 = tmp_2_15 - tmp_2_12;
		tmp_1_13 = tmp_2_14 - tmp_2_13;
		tmp_1_14 = tmp_2_14 + tmp_2_13;
		tmp_1_15 = tmp_2_15 + tmp_2_12;

/* fourth butter-fly */
		tmp_2_0 = ROUND((tmp_1_0 + tmp_1_1) * COS_4);
		tmp_2_1 = ROUND((tmp_1_0 - tmp_1_1) * COS_4);
		tmp_2_2 = ROUND(tmp_1_2 * SIN_8 + tmp_1_3 * COS_8);
		tmp_2_3 = ROUND(tmp_1_3 * SIN_8 - tmp_1_2 * COS_8);
		tmp_2_4 = tmp_1_4 + tmp_1_5;
		tmp_2_5 = tmp_1_4 - tmp_1_5;
		tmp_2_6 = tmp_1_7 - tmp_1_6;
		tmp_2_7 = tmp_1_7 + tmp_1_6;
		tmp_2_8 = tmp_1_8;
		tmp_2_9 = ROUND(tmp_1_14 * SIN_8 - tmp_1_9 * COS_8);
		tmp_2_10 = ROUND(tmp_1_10 * SIN_8 + tmp_1_13 * COS_8);
		tmp_2_11 = tmp_1_11;
		tmp_2_12 = tmp_1_12;
		tmp_2_13 = ROUND(tmp_1_13 * SIN_8 - tmp_1_10 * COS_8);
		tmp_2_14 = ROUND(tmp_1_9 * SIN_8 + tmp_1_14 * COS_8);
		tmp_2_15 = tmp_1_15;

/* fifth butter-fly */
		tmp_1_4 = ROUND(tmp_2_4 * SIN_16 + tmp_2_7 * COS_16);
		tmp_1_5 = ROUND(tmp_2_5 * COS_3_16 + tmp_2_6 * SIN_3_16);
		tmp_1_6 = ROUND(tmp_2_6 * COS_3_16 - tmp_2_5 * SIN_3_16);
		tmp_1_7 = ROUND(tmp_2_7 * SIN_16 - tmp_2_4 * COS_16);
		tmp_1_8 = tmp_2_8 + tmp_2_9;
		tmp_1_9 = tmp_2_8 - tmp_2_9;
		tmp_1_10 = tmp_2_11 + tmp_2_10;
		tmp_1_11 = tmp_2_11 - tmp_2_10;
		tmp_1_12 = tmp_2_12 + tmp_2_13;
		tmp_1_13 = tmp_2_12 - tmp_2_13;
		tmp_1_14 = tmp_2_15 - tmp_2_14;
		tmp_1_15 = tmp_2_15 + tmp_2_14;

/* sixth butter-fly */
		coeffs[j * 16 + 0] = (tmp_2_0 >> 3);
		coeffs[j * 16 + 8] = (tmp_2_1 >> 3);
		coeffs[j * 16 + 4] = (tmp_2_2 >> 3);
		coeffs[j * 16 + 12] = (tmp_2_3 >> 3);
		coeffs[j * 16 + 2] = (tmp_1_4 >> 3);
		coeffs[j * 16 + 10] = (tmp_1_5 >> 3);
		coeffs[j * 16 + 6] = (tmp_1_6 >> 3);
		coeffs[j * 16 + 14] = (tmp_1_7 >> 3);
		coeffs[j * 16 + 1] =
			(ROUND((tmp_1_8 * SIN_32 +
			tmp_1_15 * COS_32)) >> 3);
		coeffs[j * 16 + 9] =
			(ROUND((tmp_1_9 * COS_7_32 +
			tmp_1_14 * SIN_7_32)) >> 3);
		coeffs[j * 16 + 5] =
			(ROUND((tmp_1_10 * SIN_5_32 +
			tmp_1_13 * COS_5_32)) >> 3);
		coeffs[j * 16 + 13] =
			(ROUND((tmp_1_11 * COS_3_32 +
			tmp_1_12 * SIN_3_32)) >> 3);
		coeffs[j * 16 + 3] =
			(ROUND((tmp_1_12 * COS_3_32 -
			tmp_1_11 * SIN_3_32)) >> 3);
		coeffs[j * 16 + 11] =
			(ROUND((tmp_1_13 * SIN_5_32 -
			tmp_1_10 * COS_5_32)) >> 3);
		coeffs[j * 16 + 7] =
			(ROUND((tmp_1_14 * COS_7_32 -
			tmp_1_9 * SIN_7_32)) >> 3);
		coeffs[j * 16 + 15] =
			(ROUND((tmp_1_15 * SIN_32 -
			tmp_1_8 * COS_32)) >> 3);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
