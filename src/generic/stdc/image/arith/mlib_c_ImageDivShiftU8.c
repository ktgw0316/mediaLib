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

#pragma ident	"@(#)mlib_c_ImageDivShiftU8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivShift_U8        - divide image of U8 type by another
 *                                     image of U8 type with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivShift_U8(mlib_image       *dst,
 *                                        const mlib_image *src1,
 *                                        const mlib_image *src2,
 *                                        mlib_s32         shift)
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *
 *      0 <= shift <= 31
 *
 * DESCRIPTION
 *      dst[x][y][c] = src1[x][y][c] / src2[x][y][c] * 2 ^ shift
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageConstDiv.h>

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

const mlib_f32 mlib_1DIVU8_F32[] = {
	(mlib_f32)MLIB_S32_MAX,
	1.f / 1, 1.f / 2, 1.f / 3, 1.f / 4, 1.f / 5, 1.f / 6, 1.f / 7,
	1.f / 8, 1.f / 9, 1.f / 10, 1.f / 11, 1.f / 12, 1.f / 13, 1.f / 14,
	    1.f / 15,
	1.f / 16, 1.f / 17, 1.f / 18, 1.f / 19, 1.f / 20, 1.f / 21, 1.f / 22,
	    1.f / 23,
	1.f / 24, 1.f / 25, 1.f / 26, 1.f / 27, 1.f / 28, 1.f / 29, 1.f / 30,
	    1.f / 31,
	1.f / 32, 1.f / 33, 1.f / 34, 1.f / 35, 1.f / 36, 1.f / 37, 1.f / 38,
	    1.f / 39,
	1.f / 40, 1.f / 41, 1.f / 42, 1.f / 43, 1.f / 44, 1.f / 45, 1.f / 46,
	    1.f / 47,
	1.f / 48, 1.f / 49, 1.f / 50, 1.f / 51, 1.f / 52, 1.f / 53, 1.f / 54,
	    1.f / 55,
	1.f / 56, 1.f / 57, 1.f / 58, 1.f / 59, 1.f / 60, 1.f / 61, 1.f / 62,
	    1.f / 63,
	1.f / 64, 1.f / 65, 1.f / 66, 1.f / 67, 1.f / 68, 1.f / 69, 1.f / 70,
	    1.f / 71,
	1.f / 72, 1.f / 73, 1.f / 74, 1.f / 75, 1.f / 76, 1.f / 77, 1.f / 78,
	    1.f / 79,
	1.f / 80, 1.f / 81, 1.f / 82, 1.f / 83, 1.f / 84, 1.f / 85, 1.f / 86,
	    1.f / 87,
	1.f / 88, 1.f / 89, 1.f / 90, 1.f / 91, 1.f / 92, 1.f / 93, 1.f / 94,
	    1.f / 95,
	1.f / 96, 1.f / 97, 1.f / 98, 1.f / 99, 1.f / 100, 1.f / 101, 1.f / 102,
	    1.f / 103,
	1.f / 104, 1.f / 105, 1.f / 106, 1.f / 107, 1.f / 108, 1.f / 109,
	    1.f / 110, 1.f / 111,
	1.f / 112, 1.f / 113, 1.f / 114, 1.f / 115, 1.f / 116, 1.f / 117,
	    1.f / 118, 1.f / 119,
	1.f / 120, 1.f / 121, 1.f / 122, 1.f / 123, 1.f / 124, 1.f / 125,
	    1.f / 126, 1.f / 127,
	1.f / 128, 1.f / 129, 1.f / 130, 1.f / 131, 1.f / 132, 1.f / 133,
	    1.f / 134, 1.f / 135,
	1.f / 136, 1.f / 137, 1.f / 138, 1.f / 139, 1.f / 140, 1.f / 141,
	    1.f / 142, 1.f / 143,
	1.f / 144, 1.f / 145, 1.f / 146, 1.f / 147, 1.f / 148, 1.f / 149,
	    1.f / 150, 1.f / 151,
	1.f / 152, 1.f / 153, 1.f / 154, 1.f / 155, 1.f / 156, 1.f / 157,
	    1.f / 158, 1.f / 159,
	1.f / 160, 1.f / 161, 1.f / 162, 1.f / 163, 1.f / 164, 1.f / 165,
	    1.f / 166, 1.f / 167,
	1.f / 168, 1.f / 169, 1.f / 170, 1.f / 171, 1.f / 172, 1.f / 173,
	    1.f / 174, 1.f / 175,
	1.f / 176, 1.f / 177, 1.f / 178, 1.f / 179, 1.f / 180, 1.f / 181,
	    1.f / 182, 1.f / 183,
	1.f / 184, 1.f / 185, 1.f / 186, 1.f / 187, 1.f / 188, 1.f / 189,
	    1.f / 190, 1.f / 191,
	1.f / 192, 1.f / 193, 1.f / 194, 1.f / 195, 1.f / 196, 1.f / 197,
	    1.f / 198, 1.f / 199,
	1.f / 200, 1.f / 201, 1.f / 202, 1.f / 203, 1.f / 204, 1.f / 205,
	    1.f / 206, 1.f / 207,
	1.f / 208, 1.f / 209, 1.f / 210, 1.f / 211, 1.f / 212, 1.f / 213,
	    1.f / 214, 1.f / 215,
	1.f / 216, 1.f / 217, 1.f / 218, 1.f / 219, 1.f / 220, 1.f / 221,
	    1.f / 222, 1.f / 223,
	1.f / 224, 1.f / 225, 1.f / 226, 1.f / 227, 1.f / 228, 1.f / 229,
	    1.f / 230, 1.f / 231,
	1.f / 232, 1.f / 233, 1.f / 234, 1.f / 235, 1.f / 236, 1.f / 237,
	    1.f / 238, 1.f / 239,
	1.f / 240, 1.f / 241, 1.f / 242, 1.f / 243, 1.f / 244, 1.f / 245,
	    1.f / 246, 1.f / 247,
	1.f / 248, 1.f / 249, 1.f / 250, 1.f / 251, 1.f / 252, 1.f / 253,
	    1.f / 254, 1.f / 255,
	1.f / 256, 1.f / 257, 1.f / 258, 1.f / 259, 1.f / 260, 1.f / 261,
	    1.f / 262, 1.f / 263,
	1.f / 264, 1.f / 265, 1.f / 266, 1.f / 267, 1.f / 268, 1.f / 269,
	    1.f / 270, 1.f / 271,
	1.f / 272, 1.f / 273, 1.f / 274, 1.f / 275, 1.f / 276, 1.f / 277,
	    1.f / 278, 1.f / 279,
	1.f / 280, 1.f / 281, 1.f / 282, 1.f / 283, 1.f / 284, 1.f / 285,
	    1.f / 286, 1.f / 287,
	1.f / 288, 1.f / 289, 1.f / 290, 1.f / 291, 1.f / 292, 1.f / 293,
	    1.f / 294, 1.f / 295,
	1.f / 296, 1.f / 297, 1.f / 298, 1.f / 299, 1.f / 300, 1.f / 301,
	    1.f / 302, 1.f / 303,
	1.f / 304, 1.f / 305, 1.f / 306, 1.f / 307, 1.f / 308, 1.f / 309,
	    1.f / 310, 1.f / 311,
	1.f / 312, 1.f / 313, 1.f / 314, 1.f / 315, 1.f / 316, 1.f / 317,
	    1.f / 318, 1.f / 319,
	1.f / 320, 1.f / 321, 1.f / 322, 1.f / 323, 1.f / 324, 1.f / 325,
	    1.f / 326, 1.f / 327,
	1.f / 328, 1.f / 329, 1.f / 330, 1.f / 331, 1.f / 332, 1.f / 333,
	    1.f / 334, 1.f / 335,
	1.f / 336, 1.f / 337, 1.f / 338, 1.f / 339, 1.f / 340, 1.f / 341,
	    1.f / 342, 1.f / 343,
	1.f / 344, 1.f / 345, 1.f / 346, 1.f / 347, 1.f / 348, 1.f / 349,
	    1.f / 350, 1.f / 351,
	1.f / 352, 1.f / 353, 1.f / 354, 1.f / 355, 1.f / 356, 1.f / 357,
	    1.f / 358, 1.f / 359,
	1.f / 360, 1.f / 361, 1.f / 362, 1.f / 363, 1.f / 364, 1.f / 365,
	    1.f / 366, 1.f / 367,
	1.f / 368, 1.f / 369, 1.f / 370, 1.f / 371, 1.f / 372, 1.f / 373,
	    1.f / 374, 1.f / 375,
	1.f / 376, 1.f / 377, 1.f / 378, 1.f / 379, 1.f / 380, 1.f / 381,
	    1.f / 382, 1.f / 383,
	1.f / 384, 1.f / 385, 1.f / 386, 1.f / 387, 1.f / 388, 1.f / 389,
	    1.f / 390, 1.f / 391,
	1.f / 392, 1.f / 393, 1.f / 394, 1.f / 395, 1.f / 396, 1.f / 397,
	    1.f / 398, 1.f / 399,
	1.f / 400, 1.f / 401, 1.f / 402, 1.f / 403, 1.f / 404, 1.f / 405,
	    1.f / 406, 1.f / 407,
	1.f / 408, 1.f / 409, 1.f / 410, 1.f / 411, 1.f / 412, 1.f / 413,
	    1.f / 414, 1.f / 415,
	1.f / 416, 1.f / 417, 1.f / 418, 1.f / 419, 1.f / 420, 1.f / 421,
	    1.f / 422, 1.f / 423,
	1.f / 424, 1.f / 425, 1.f / 426, 1.f / 427, 1.f / 428, 1.f / 429,
	    1.f / 430, 1.f / 431,
	1.f / 432, 1.f / 433, 1.f / 434, 1.f / 435, 1.f / 436, 1.f / 437,
	    1.f / 438, 1.f / 439,
	1.f / 440, 1.f / 441, 1.f / 442, 1.f / 443, 1.f / 444, 1.f / 445,
	    1.f / 446, 1.f / 447,
	1.f / 448, 1.f / 449, 1.f / 450, 1.f / 451, 1.f / 452, 1.f / 453,
	    1.f / 454, 1.f / 455,
	1.f / 456, 1.f / 457, 1.f / 458, 1.f / 459, 1.f / 460, 1.f / 461,
	    1.f / 462, 1.f / 463,
	1.f / 464, 1.f / 465, 1.f / 466, 1.f / 467, 1.f / 468, 1.f / 469,
	    1.f / 470, 1.f / 471,
	1.f / 472, 1.f / 473, 1.f / 474, 1.f / 475, 1.f / 476, 1.f / 477,
	    1.f / 478, 1.f / 479,
	1.f / 480, 1.f / 481, 1.f / 482, 1.f / 483, 1.f / 484, 1.f / 485,
	    1.f / 486, 1.f / 487,
	1.f / 488, 1.f / 489, 1.f / 490, 1.f / 491, 1.f / 492, 1.f / 493,
	    1.f / 494, 1.f / 495,
	1.f / 496, 1.f / 497, 1.f / 498, 1.f / 499, 1.f / 500, 1.f / 501,
	    1.f / 502, 1.f / 503,
	1.f / 504, 1.f / 505, 1.f / 506, 1.f / 507, 1.f / 508, 1.f / 509,
	    1.f / 510, 1.f / 511
};

/* *********************************************************** */

mlib_status
mlib_ImageDivShift_U8(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_f32 lut[256];
	mlib_u8 *dl, *sl1, *sl2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_d64 dshift, fshift;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	slb1 = mlib_ImageGetStride(src1);
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

	SCALBN(dshift, 23 + shift);
	fshift = dshift;

/* lut generation */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 256; i++) {
		lut[i] = (mlib_f32)(fshift * mlib_1DIVU8_F32[i]);
	}

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			mlib_d64 fval = mlib_U82F32[sl1[i]] * lut[sl2[i]];
			mlib_s32 ival = (mlib_s32)fval;

#ifndef MLIB_USE_FTOI_CLAMPING

/* (mlib_d64) - krakatoa bug */
			if (fval >= MLIB_S32_MAX) {
				ival = MLIB_S32_MAX;
			}
#endif /* MLIB_USE_FTOI_CLAMPING */
			dl[i] = ival >> 23;
		}

		sl1 += slb1;
		sl2 += slb2;
		dl += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
