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

#pragma ident	"@(#)mlib_i_VectorMaximum.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMaximum - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximum_U8(mlib_u8       *max,
 *                                        const mlib_u8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMaximum_S8(mlib_s8       *max,
 *                                        const mlib_s8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMaximum_S16(mlib_s16       *max,
 *                                         const mlib_s16 *x,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

/*
 * FUNCTIONS
 *      mlib_VectorMaximumMag - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximumMag_U8C(mlib_u8       *max,
 *                                            const mlib_u8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMaximumMag_S8C(mlib_s8       *max,
 *                                            const mlib_s8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMaximumMag_S16C(mlib_s16       *max,
 *                                             const mlib_s16 *x,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *      This function finds the first element with the maximum
 *      magnitude, then puts the real and imaginary parts of it into
 *      max[0] and max[1], respectively.
 *
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

#include <mlib_algebra.h>
#include <mlib_c_VectorMaximum.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMaximum_S8 = __mlib_VectorMaximum_S8
#pragma weak mlib_VectorMaximum_U8 = __mlib_VectorMaximum_U8
#pragma weak mlib_VectorMaximum_S16 = __mlib_VectorMaximum_S16
#pragma weak mlib_VectorMaximumMag_S8C = __mlib_VectorMaximumMag_S8C
#pragma weak mlib_VectorMaximumMag_U8C = __mlib_VectorMaximumMag_U8C
#pragma weak mlib_VectorMaximumMag_S16C = __mlib_VectorMaximumMag_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMaximum_S8) mlib_VectorMaximum_S8
	__attribute__((weak, alias("__mlib_VectorMaximum_S8")));
__typeof__(__mlib_VectorMaximum_U8) mlib_VectorMaximum_U8
	__attribute__((weak, alias("__mlib_VectorMaximum_U8")));
__typeof__(__mlib_VectorMaximum_S16) mlib_VectorMaximum_S16
	__attribute__((weak, alias("__mlib_VectorMaximum_S16")));
__typeof__(__mlib_VectorMaximumMag_S8C) mlib_VectorMaximumMag_S8C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S8C")));
__typeof__(__mlib_VectorMaximumMag_U8C) mlib_VectorMaximumMag_U8C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_U8C")));
__typeof__(__mlib_VectorMaximumMag_S16C) mlib_VectorMaximumMag_S16C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*  Table for S8/U8 : x[i] = (mlib_f32)(i * i)   -128 <= i <= 255  */

const mlib_s32 mlib_sqw_s32_SU8[] = {
	16384, 16129, 15876, 15625, 15376, 15129, 14884, 14641, 14400, 14161,
	13924, 13689, 13456, 13225, 12996, 12769, 12544, 12321, 12100, 11881,
	11664, 11449, 11236, 11025, 10816, 10609, 10404, 10201, 10000, 9801,
	9604, 9409, 9216, 9025, 8836, 8649, 8464, 8281, 8100, 7921,
	7744, 7569, 7396, 7225, 7056, 6889, 6724, 6561, 6400, 6241,
	6084, 5929, 5776, 5625, 5476, 5329, 5184, 5041, 4900, 4761,
	4624, 4489, 4356, 4225, 4096, 3969, 3844, 3721, 3600, 3481,
	3364, 3249, 3136, 3025, 2916, 2809, 2704, 2601, 2500, 2401,
	2304, 2209, 2116, 2025, 1936, 1849, 1764, 1681, 1600, 1521,
	1444, 1369, 1296, 1225, 1156, 1089, 1024, 961, 900, 841,
	784, 729, 676, 625, 576, 529, 484, 441, 400, 361,
	324, 289, 256, 225, 196, 169, 144, 121, 100, 81,
	64, 49, 36, 25, 16, 9, 4, 1, 0, 1,
	4, 9, 16, 25, 36, 49, 64, 81, 100, 121,
	144, 169, 196, 225, 256, 289, 324, 361, 400, 441,
	484, 529, 576, 625, 676, 729, 784, 841, 900, 961,
	1024, 1089, 1156, 1225, 1296, 1369, 1444, 1521, 1600, 1681,
	1764, 1849, 1936, 2025, 2116, 2209, 2304, 2401, 2500, 2601,
	2704, 2809, 2916, 3025, 3136, 3249, 3364, 3481, 3600, 3721,
	3844, 3969, 4096, 4225, 4356, 4489, 4624, 4761, 4900, 5041,
	5184, 5329, 5476, 5625, 5776, 5929, 6084, 6241, 6400, 6561,
	6724, 6889, 7056, 7225, 7396, 7569, 7744, 7921, 8100, 8281,
	8464, 8649, 8836, 9025, 9216, 9409, 9604, 9801, 10000, 10201,
	10404, 10609, 10816, 11025, 11236, 11449, 11664, 11881, 12100, 12321,
	12544, 12769, 12996, 13225, 13456, 13689, 13924, 14161, 14400, 14641,
	14884, 15129, 15376, 15625, 15876, 16129, 16384, 16641,
	16900, 17161, 17424, 17689, 17956, 18225, 18496, 18769, 19044, 19321,
	19600, 19881, 20164, 20449, 20736, 21025, 21316, 21609, 21904, 22201,
	22500, 22801, 23104, 23409, 23716, 24025, 24336, 24649, 24964, 25281,
	25600, 25921, 26244, 26569, 26896, 27225, 27556, 27889, 28224, 28561,
	28900, 29241, 29584, 29929, 30276, 30625, 30976, 31329, 31684, 32041,
	32400, 32761, 33124, 33489, 33856, 34225, 34596, 34969, 35344, 35721,
	36100, 36481, 36864, 37249, 37636, 38025, 38416, 38809, 39204, 39601,
	40000, 40401, 40804, 41209, 41616, 42025, 42436, 42849, 43264, 43681,
	44100, 44521, 44944, 45369, 45796, 46225, 46656, 47089, 47524, 47961,
	48400, 48841, 49284, 49729, 50176, 50625, 51076, 51529, 51984, 52441,
	52900, 53361, 53824, 54289, 54756, 55225, 55696, 56169, 56644, 57121,
	57600, 58081, 58564, 59049, 59536, 60025, 60516, 61009, 61504, 62001,
	62500, 63001, 63504, 64009, 64516, 65025
};
const mlib_d64 mlib_sqw_S8[] = {
	16384, 16129, 15876, 15625, 15376, 15129, 14884, 14641, 14400, 14161,
	13924, 13689, 13456, 13225, 12996, 12769, 12544, 12321, 12100, 11881,
	11664, 11449, 11236, 11025, 10816, 10609, 10404, 10201, 10000, 9801,
	9604, 9409, 9216, 9025, 8836, 8649, 8464, 8281, 8100, 7921,
	7744, 7569, 7396, 7225, 7056, 6889, 6724, 6561, 6400, 6241,
	6084, 5929, 5776, 5625, 5476, 5329, 5184, 5041, 4900, 4761,
	4624, 4489, 4356, 4225, 4096, 3969, 3844, 3721, 3600, 3481,
	3364, 3249, 3136, 3025, 2916, 2809, 2704, 2601, 2500, 2401,
	2304, 2209, 2116, 2025, 1936, 1849, 1764, 1681, 1600, 1521,
	1444, 1369, 1296, 1225, 1156, 1089, 1024, 961, 900, 841,
	784, 729, 676, 625, 576, 529, 484, 441, 400, 361,
	324, 289, 256, 225, 196, 169, 144, 121, 100, 81,
	64, 49, 36, 25, 16, 9, 4, 1, 0, 1,
	4, 9, 16, 25, 36, 49, 64, 81, 100, 121,
	144, 169, 196, 225, 256, 289, 324, 361, 400, 441,
	484, 529, 576, 625, 676, 729, 784, 841, 900, 961,
	1024, 1089, 1156, 1225, 1296, 1369, 1444, 1521, 1600, 1681,
	1764, 1849, 1936, 2025, 2116, 2209, 2304, 2401, 2500, 2601,
	2704, 2809, 2916, 3025, 3136, 3249, 3364, 3481, 3600, 3721,
	3844, 3969, 4096, 4225, 4356, 4489, 4624, 4761, 4900, 5041,
	5184, 5329, 5476, 5625, 5776, 5929, 6084, 6241, 6400, 6561,
	6724, 6889, 7056, 7225, 7396, 7569, 7744, 7921, 8100, 8281,
	8464, 8649, 8836, 9025, 9216, 9409, 9604, 9801, 10000, 10201,
	10404, 10609, 10816, 11025, 11236, 11449, 11664, 11881, 12100, 12321,
	12544, 12769, 12996, 13225, 13456, 13689, 13924, 14161, 14400, 14641,
	14884, 15129, 15376, 15625, 15876, 16129, 16384, 16641,
	16900, 17161, 17424, 17689, 17956, 18225, 18496, 18769, 19044, 19321,
	19600, 19881, 20164, 20449, 20736, 21025, 21316, 21609, 21904, 22201,
	22500, 22801, 23104, 23409, 23716, 24025, 24336, 24649, 24964, 25281,
	25600, 25921, 26244, 26569, 26896, 27225, 27556, 27889, 28224, 28561,
	28900, 29241, 29584, 29929, 30276, 30625, 30976, 31329, 31684, 32041,
	32400, 32761, 33124, 33489, 33856, 34225, 34596, 34969, 35344, 35721,
	36100, 36481, 36864, 37249, 37636, 38025, 38416, 38809, 39204, 39601,
	40000, 40401, 40804, 41209, 41616, 42025, 42436, 42849, 43264, 43681,
	44100, 44521, 44944, 45369, 45796, 46225, 46656, 47089, 47524, 47961,
	48400, 48841, 49284, 49729, 50176, 50625, 51076, 51529, 51984, 52441,
	52900, 53361, 53824, 54289, 54756, 55225, 55696, 56169, 56644, 57121,
	57600, 58081, 58564, 59049, 59536, 60025, 60516, 61009, 61504, 62001,
	62500, 63001, 63504, 64009, 64516, 65025
};

/*  Table for S16 upper half : x[i] = (mlib_d64)(i * 256)   -128 <= i <= 127  */

const mlib_d64 mlib_S16uh[] = {
	-32768, -32512, -32256, -32000, -31744, -31488, -31232, -30976, -30720,
		-30464,
	-30208, -29952, -29696, -29440, -29184, -28928, -28672, -28416, -28160,
		-27904,
	-27648, -27392, -27136, -26880, -26624, -26368, -26112, -25856, -25600,
		-25344,
	-25088, -24832, -24576, -24320, -24064, -23808, -23552, -23296, -23040,
		-22784,
	-22528, -22272, -22016, -21760, -21504, -21248, -20992, -20736, -20480,
		-20224,
	-19968, -19712, -19456, -19200, -18944, -18688, -18432, -18176, -17920,
		-17664,
	-17408, -17152, -16896, -16640, -16384, -16128, -15872, -15616, -15360,
		-15104,
	-14848, -14592, -14336, -14080, -13824, -13568, -13312, -13056, -12800,
		-12544,
	-12288, -12032, -11776, -11520, -11264, -11008, -10752, -10496, -10240,
		-9984,
	-9728, -9472, -9216, -8960, -8704, -8448, -8192, -7936, -7680, -7424,
	-7168, -6912, -6656, -6400, -6144, -5888, -5632, -5376, -5120, -4864,
	-4608, -4352, -4096, -3840, -3584, -3328, -3072, -2816, -2560, -2304,
	-2048, -1792, -1536, -1280, -1024, -768, -512, -256, 0, 256,
	512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560, 2816,
	3072, 3328, 3584, 3840, 4096, 4352, 4608, 4864, 5120, 5376,
	5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936,
	8192, 8448, 8704, 8960, 9216, 9472, 9728, 9984, 10240, 10496,
	10752, 11008, 11264, 11520, 11776, 12032, 12288, 12544, 12800, 13056,
	13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616,
	15872, 16128, 16384, 16640, 16896, 17152, 17408, 17664, 17920, 18176,
	18432, 18688, 18944, 19200, 19456, 19712, 19968, 20224, 20480, 20736,
	20992, 21248, 21504, 21760, 22016, 22272, 22528, 22784, 23040, 23296,
	23552, 23808, 24064, 24320, 24576, 24832, 25088, 25344, 25600, 25856,
	26112, 26368, 26624, 26880, 27136, 27392, 27648, 27904, 28160, 28416,
	28672, 28928, 29184, 29440, 29696, 29952, 30208, 30464, 30720, 30976,
	31232, 31488, 31744, 32000, 32256, 32512
};

/*  Table for S16 lower half : x[i] = (mlib_d64) i   0 <= i <= 255  */

const mlib_d64 mlib_S16lh[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
	130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
	140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
	150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
	170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
	180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
	190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
	200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
	210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
	220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
	230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
	240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
	250, 251, 252, 253, 254, 255
};

/* *********************************************************** */

#define	MAX_LOG(max, arg)	max -= ((max - arg) & ((max - arg) >> 31))
#define	MAX_CMP(max, arg)	max = (arg > max) ? arg : max

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_U8(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *psrc = (mlib_u8 *)x;
	mlib_s16 max1, max2, max3, max4;
	mlib_s16 max5, max6, max7, max8;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	max1 = max2 = max3 = max4 = MLIB_S8_MIN;
	max8 = max5 = max6 = max7 = MLIB_S8_MIN;

	for (i = 0; i <= (n - 8); i += 8) {
		MAX_LOG(max1, psrc[i]);
		MAX_LOG(max2, psrc[i + 1]);
		MAX_LOG(max3, psrc[i + 2]);
		MAX_LOG(max4, psrc[i + 3]);
		MAX_LOG(max5, psrc[i + 4]);
		MAX_LOG(max6, psrc[i + 5]);
		MAX_LOG(max7, psrc[i + 6]);
		MAX_LOG(max8, psrc[i + 7]);
	}

	if (i < n) {
		MAX_LOG(max1, psrc[i]);
	}
	i++;

	if (i < n) {
		MAX_LOG(max2, psrc[i]);
	}
	i++;

	if (i < n) {
		MAX_LOG(max3, psrc[i]);
	}
	i++;

	if (i < n) {
		MAX_LOG(max4, psrc[i]);
	}
	i++;

	if (i < n) {
		MAX_LOG(max5, psrc[i]);
	}
	i++;

	if (i < n) {
		MAX_LOG(max6, psrc[i]);
	}
	i++;

	if (i < n) {
		MAX_LOG(max7, psrc[i]);
	}

	MAX_LOG(max1, max2);
	MAX_LOG(max3, max4);
	MAX_LOG(max5, max6);
	MAX_LOG(max7, max8);
	MAX_LOG(max1, max3);
	MAX_LOG(max5, max7);
	MAX_LOG(max1, max5);

	max[0] = max1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S8(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *psrc = (mlib_s8 *)x;
#ifdef _NO_LONGLONG
	mlib_d64 max1, max2, max3, max4;
	mlib_d64 max5, max6, max7, max8;
#else
	mlib_s16 max1, max2, max3, max4;
	mlib_s16 max5, max6, max7, max8;
#endif

	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	max1 = max2 = max3 = max4 = MLIB_S8_MIN;
	max8 = max5 = max6 = max7 = MLIB_S8_MIN;

	for (i = 0; i <= (n - 8); i += 8) {
#ifdef _NO_LONGLONG
		MAX_CMP(max1, psrc[i]);
		MAX_CMP(max2, psrc[i + 1]);
		MAX_CMP(max3, psrc[i + 2]);
		MAX_CMP(max4, psrc[i + 3]);
		MAX_CMP(max5, psrc[i + 4]);
		MAX_CMP(max6, psrc[i + 5]);
		MAX_CMP(max7, psrc[i + 6]);
		MAX_CMP(max8, psrc[i + 7]);
#else
		MAX_LOG(max1, psrc[i]);
		MAX_LOG(max2, psrc[i + 1]);
		MAX_LOG(max3, psrc[i + 2]);
		MAX_LOG(max4, psrc[i + 3]);
		MAX_LOG(max5, psrc[i + 4]);
		MAX_LOG(max6, psrc[i + 5]);
		MAX_LOG(max7, psrc[i + 6]);
		MAX_LOG(max8, psrc[i + 7]);
#endif
	}

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max1, psrc[i]);
#else
		MAX_LOG(max1, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max2, psrc[i]);
#else
		MAX_LOG(max2, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max3, psrc[i]);
#else
		MAX_LOG(max3, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max4, psrc[i]);
#else
		MAX_LOG(max4, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max5, psrc[i]);
#else
		MAX_LOG(max5, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max6, psrc[i]);
#else
		MAX_LOG(max6, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max7, psrc[i]);
#else
		MAX_LOG(max7, psrc[i]);
#endif
	}

#ifdef _NO_LONGLONG
	MAX_CMP(max1, max2);
	MAX_CMP(max3, max4);
	MAX_CMP(max5, max6);
	MAX_CMP(max7, max8);
	MAX_CMP(max1, max3);
	MAX_CMP(max5, max7);
	MAX_CMP(max1, max5);
	max[0] = (mlib_s8)max1;
#else
	MAX_LOG(max1, max2);
	MAX_LOG(max3, max4);
	MAX_LOG(max5, max6);
	MAX_LOG(max7, max8);
	MAX_LOG(max1, max3);
	MAX_LOG(max5, max7);
	MAX_LOG(max1, max5);

	max[0] = max1;
#endif
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S16(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *psrc = (mlib_s16 *)x;
#ifdef _NO_LONGLONG
	mlib_d64 max1, max2, max3, max4;
	mlib_d64 max5, max6, max7, max8;
#else
	mlib_s32 max1, max2, max3, max4;
	mlib_s32 max5, max6, max7, max8;
#endif
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	max1 = max2 = max3 = max4 = MLIB_S16_MIN;
	max5 = max6 = max7 = max8 = MLIB_S16_MIN;

	for (i = 0; i <= (n - 8); i += 8) {
#ifdef _NO_LONGLONG
		MAX_CMP(max1, psrc[i]);
		MAX_CMP(max2, psrc[i + 1]);
		MAX_CMP(max3, psrc[i + 2]);
		MAX_CMP(max4, psrc[i + 3]);
		MAX_CMP(max5, psrc[i + 4]);
		MAX_CMP(max6, psrc[i + 5]);
		MAX_CMP(max7, psrc[i + 6]);
		MAX_CMP(max8, psrc[i + 7]);
#else
		MAX_LOG(max1, psrc[i]);
		MAX_LOG(max2, psrc[i + 1]);
		MAX_LOG(max3, psrc[i + 2]);
		MAX_LOG(max4, psrc[i + 3]);
		MAX_LOG(max5, psrc[i + 4]);
		MAX_LOG(max6, psrc[i + 5]);
		MAX_LOG(max7, psrc[i + 6]);
		MAX_LOG(max8, psrc[i + 7]);
#endif
	}

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max1, psrc[i]);
#else
		MAX_LOG(max1, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max2, psrc[i]);
#else
		MAX_LOG(max2, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max3, psrc[i]);
#else
		MAX_LOG(max3, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max4, psrc[i]);
#else
		MAX_LOG(max4, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max5, psrc[i]);
#else
		MAX_LOG(max5, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max6, psrc[i]);
#else
		MAX_LOG(max6, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX_CMP(max7, psrc[i]);
#else
		MAX_LOG(max7, psrc[i]);
#endif
	}

#ifdef _NO_LONGLONG
	MAX_CMP(max1, max2);
	MAX_CMP(max3, max4);
	MAX_CMP(max5, max6);
	MAX_CMP(max7, max8);
	MAX_CMP(max1, max3);
	MAX_CMP(max5, max7);
	MAX_CMP(max1, max5);
	max[0] = (mlib_s16)max1;
#else
	MAX_LOG(max1, max2);
	MAX_LOG(max3, max4);
	MAX_LOG(max5, max6);
	MAX_LOG(max7, max8);
	MAX_LOG(max1, max3);
	MAX_LOG(max5, max7);
	MAX_LOG(max1, max5);

	max[0] = max1;
#endif
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_U8C(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *psrc = (mlib_u8 *)x;
#ifdef _NO_LONGLONG
	const mlib_d64 *psqw = mlib_sqw_S8 + 128;
	mlib_d64 val;
	mlib_d64 val_max;
#else
	const mlib_s32 *psqw = mlib_sqw_s32_SU8 + 128;
	mlib_s32 val;
	mlib_s32 val_max;
#endif
	mlib_u8 max_re, max_im;
	mlib_s32 i, index1, index2;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_re = max_im = val_max = 0;

	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		val = psqw[psrc[index1]] + psqw[psrc[index2]];

		if (val > val_max) {
			max_re = psrc[index1];
			max_im = psrc[index2];
			val_max = val;
		}
	}

	max[0] = max_re;
	max[1] = max_im;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_S8C(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *psrc = (mlib_s8 *)x;
#ifdef _NO_LONGLONG
	const mlib_d64 *psqw = mlib_sqw_S8 + 128;
	mlib_d64 val;
	mlib_d64 val_max;
#else
	const mlib_s32 *psqw = mlib_sqw_s32_SU8 + 128;
	mlib_s32 val;
	mlib_s32 val_max;
#endif
	mlib_s8 max_re, max_im;
	mlib_s32 i, index1, index2;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_re = max_im = val_max = 0;

	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		val = psqw[psrc[index1]] + psqw[psrc[index2]];

		if (val > val_max) {
			max_re = psrc[index1];
			max_im = psrc[index2];
			val_max = val;
		}
	}

	max[0] = max_re;
	max[1] = max_im;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_S16C(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *psrc = (mlib_s16 *)x;
#ifdef _NO_LONGLONG
	mlib_d64 max_re, max_im;
	const mlib_d64 *psqw = mlib_S16uh + 128;
	mlib_d64 val;
	mlib_d64 val_max;
#else
	mlib_s16 max_re, max_im;
	mlib_s64 val;
	mlib_s64 val_max;
#endif
	mlib_s32 i, index1, index2;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_re = max_im = val_max = 0;

	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
#ifdef _NO_LONGLONG
		mlib_d64 ld00 = psqw[psrc[index1] >> 8];
		mlib_d64 ld01 = mlib_S16lh[psrc[index1] & 0xFF];
		mlib_d64 ld10 = psqw[psrc[index2] >> 8];
		mlib_d64 ld11 = mlib_S16lh[psrc[index2] & 0xFF];

		val = (ld00 + ld01) * (ld00 + ld01) +
			(ld10 + ld11) * (ld10 + ld11);

		if (val > val_max) {
			max_re = (ld00 + ld01);
			max_im = (ld10 + ld11);
			val_max = val;
		}
#else
		mlib_s64 __val_re;
		mlib_s64 __val_im;

		__val_re = psrc[index1];
		__val_im = psrc[index2];
		val = __val_im * __val_im + __val_re * __val_re;
		if (val > val_max) {
			max_re = __val_re;
			max_im = __val_im;
			val_max = val;
		}
#endif
	}
#ifdef _NO_LONGLONG
	max[0] = (mlib_s16)max_re;
	max[1] = (mlib_s16)max_im;
#else
	max[0] = max_re;
	max[1] = max_im;
#endif
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
