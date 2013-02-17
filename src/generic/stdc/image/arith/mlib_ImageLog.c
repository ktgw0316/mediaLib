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

#pragma ident	"@(#)mlib_ImageLog.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageLog       - compute the natural logarithm of the pixel values
 *      mlib_ImageLog_Inp
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLog(mlib_image       *dst,
 *                                const mlib_image *src)
 *      mlib_status mlib_ImageLog_Inp(mlib_image *srcdst)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels. The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = log(sub e)(src[x][y][c])
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = log(sub e)(srcdst[x][y][c])
 *
 *      where, e = 2.718...
 *
 *      The result will be rounded and clamped as needed.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLog = __mlib_ImageLog
#pragma weak mlib_ImageLog_Inp = __mlib_ImageLog_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageLog) mlib_ImageLog
    __attribute__((weak, alias("__mlib_ImageLog")));
__typeof__(__mlib_ImageLog_Inp) mlib_ImageLog_Inp
    __attribute__((weak, alias("__mlib_ImageLog_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* log of U8 values */
static const mlib_u8 mlib_log_U8[] = {
	0, 0, 1, 1, 1, 2, 2, 2,
	2, 2, 2, 2, 2, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6
};

/* *********************************************************** */

/* number of significant bits in U8 values */
static const mlib_u8 mlib_logb_U8[] = {
	0, 1, 2, 2, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8
};

/* *********************************************************** */

/* number of significant bits in S8 values (31 for negative val) */
static const mlib_u8 mlib_logb_S8[] = {
	0, 1, 2, 2, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 31
};

/* *********************************************************** */

/* table of log(x)*2^25 */
static const mlib_u32 mlib_log_sh25[] = {
	0, 0, 23258160, 36863311,
	46516320, 54003775, 60121471, 65293910,
	69774480, 73726623, 77261935, 80460014,
	83379631, 86065419, 88552070, 90867086,
	93032640, 95066864, 96984783, 98798978,
	100520095, 102157221, 103718174, 105209727,
	106637791, 108007550, 109323579, 110589934,
	111810230, 112987699, 114125246, 115225490,
	116290800, 117323325, 118325024, 119297685,
	120242943, 121162300, 122057137, 122928730,
	123778255, 124606801, 125415381, 126204934,
	126976334, 127730398, 128467887, 129189516,
	129895951, 130587820, 131265710, 131930176,
	132581739, 133220890, 133848094, 134463789,
	135068390, 135662289, 136245859, 136819453,
	137383406, 137938038, 138483650, 139020532,
	139548960, 140069194, 140581485, 141086073,
	141583184, 142073039, 142555845, 143031802,
	143501102, 143963930, 144420459, 144870861,
	145315297, 145753924, 146186890, 146614341,
	147036415, 147453245, 147864961, 148271687,
	148673541, 149070639, 149463093, 149851010,
	150234494, 150613644, 150988558, 151359329,
	151726047, 152088801, 152447676, 152802752,
	153154111, 153501828, 153845979, 154186637,
	154523870, 154857748, 155188336, 155515699,
	155839899, 156160996, 156479050, 156794117,
	157106254, 157415514, 157721949, 158025611,
	158326550, 158624813, 158920449, 159213502,
	159504019, 159792041, 160077613, 160360774,
	160641566, 160920028, 161196197, 161470113,
	161741810, 162011325, 162278692, 162543946,
	162807120, 163068245, 163327354, 163584477,
	163839645, 164092887, 164344233, 164593709,
	164841344, 165087165, 165331199, 165573470,
	165814005, 166052827, 166289962, 166525433,
	166759262, 166991474, 167222090, 167451131,
	167678619, 167904576, 168129021, 168351975,
	168573457, 168793487, 169012084, 169229265,
	169445050, 169659456, 169872501, 170084201,
	170294575, 170503637, 170711405, 170917895,
	171123121, 171327100, 171529847, 171731375,
	171931701, 172130838, 172328799, 172525600,
	172721253, 172915772, 173109170, 173301460,
	173492654, 173682764, 173871804, 174059784,
	174246718, 174432615, 174617489, 174801349,
	174984207, 175166075, 175346961, 175526878,
	175705836, 175883844, 176060912, 176237052,
	176412271, 176586580, 176759988, 176932505,
	177104139, 177274900, 177444796, 177613837,
	177782030, 177949384, 178115907, 178281609,
	178446496, 178610576, 178773859, 178936350,
	179098059, 179258991, 179419156, 179578560,
	179737210, 179895113, 180052277, 180208709,
	180364414, 180519400, 180673673, 180827241,
	180980109, 181132283, 181283771, 181434578,
	181584709, 181734173, 181882973, 182031116,
	182178609, 182325455, 182471662, 182617235,
	182762179, 182906499, 183050201, 183193291,
	183335773, 183477652, 183618934, 183759624,
	183899726, 184039246, 184178188, 184316557,
	184454357, 184591595, 184728273, 184864396,
	184999970, 185134998, 185269485, 185403435,
	185536852, 185669741, 185802106, 185933951
};

/* *********************************************************** */

/* table of log(2)*x*2^25 + 2^24 */
static const mlib_u32 mlib_mulLn2_sh25[] = {
	16777216, 40035376, 63293536, 86551696,
	109809856, 133068016, 156326176, 179584336,
	202842495, 226100655, 249358815, 272616975,
	295875135, 319133295, 342391455, 365649615,
	388907775, 412165935, 435424095, 458682255,
	481940415, 505198575, 528456735, 551714895,
	574973054, 598231214, 621489374, 644747534,
	668005694, 691263854, 714522014, 0
};

/* *********************************************************** */

mlib_status
__mlib_ImageLog(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_s32 nchan, xsize, ysize, dsize, dlb, slb;
	mlib_u8 *lut[4] = { (void *)mlib_log_U8,
		(void *)mlib_log_U8,
		(void *)mlib_log_U8,
		(void *)mlib_log_U8
	};
	mlib_u8 *pdst, *psrc;
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);
	xsize *= nchan;

	if (type == MLIB_BYTE) {
		return (__mlib_ImageLookUp(dst, src, (const void **)lut));
	}

	if (type == MLIB_SHORT || type == MLIB_USHORT)
		dsize = xsize * 2;
	else if (type == MLIB_INT)
		dsize = xsize * 4;
	else
		return (MLIB_FAILURE);

	if ((dsize == slb) && (dsize == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

	if (type == MLIB_SHORT) {

		for (j = 0; j < ysize; j++) {
			mlib_u16 *sp = (mlib_u16 *)psrc;
			mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_u32 s, t, dm, d, sh1;

				s = sp[i];
/* MLIB_S16_MIN for zero and negative values */
				dm = ((s | (s - 1)) & 0x8000);
/* number of significant bits in (s >> 8)    */
				sh1 = mlib_logb_S8[s >> 8];
/* t contains 8 most significant bits of s   */
				t = s >> sh1;
/* calculate log(s) ~= log(t*2^sh1) = sh1*log(2) + log(t) */
				d = (mlib_mulLn2_sh25[sh1] +
				    mlib_log_sh25[t]) >> 25;
				dp[i] = d | dm;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (type == MLIB_USHORT) {

		for (j = 0; j < ysize; j++) {
			mlib_u16 *sp = (mlib_u16 *)psrc;
			mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_u32 s, t, d, sh1;

				s = sp[i];
/* number of significant bits in (s >> 8)    */
				sh1 = mlib_logb_U8[s >> 8];
/* t contains 8 most significant bits of s   */
				t = s >> sh1;
/* calculate log(s) ~= log(t*2^sh1) = sh1*log(2) + log(t) */
				d = (mlib_mulLn2_sh25[sh1] +
				    mlib_log_sh25[t]) >> 25;
				dp[i] = d;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (type == MLIB_INT) {

		for (j = 0; j < ysize; j++) {
			mlib_u32 *sp = (mlib_u32 *)psrc;
			mlib_u32 *dp = (mlib_u32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_u32 s, t, d, dm, sh1, sh2, sh3;

				s = sp[i];
/* MLIB_S32_MIN for zero and negative values */
				dm = ((s | (s - 1)) & 0x80000000);
/* number of significant bits in (s >> 24)   */
				sh1 = mlib_logb_S8[s >> 24];
				t = s >> sh1;
				sh2 = mlib_logb_U8[t >> 16];
				t = t >> sh2;
				sh3 = mlib_logb_U8[t >> 8];
/* now t contains 8 most significant bits of s */
				t = t >> sh3;
/* calculate log(s) ~= log(t*2^sh) = sh*log(2) + log(t), sh = sh1 + sh2 + sh3 */
				d = (mlib_mulLn2_sh25[sh1 + sh2 + sh3] +
				    mlib_log_sh25[t]) >> 25;
				dp[i] = d | dm;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageLog_Inp(
    mlib_image *srcdst)
{
	return (__mlib_ImageLog(srcdst, srcdst));
}

/* *********************************************************** */
