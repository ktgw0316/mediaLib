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

#pragma ident	"@(#)mlib_c_ImageColorRGB2YCC.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorRGB2YCC - color conversion from RGB to YCC space
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2YCC(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *        |Y |   |fmat[0] fmat[1] fmat[2]|   |R|   |offset[0]|
 *        |Cb| = |fmat[3] fmat[4] fmat[5]| * |G| + |offset[1]|
 *        |Cr|   |fmat[6] fmat[7] fmat[8]|   |B|   |offset[2]|
 *
 *   where
 *
 *      fmat[] = { 65.738/256, 129.057/256,  25.064/256,
 *                -37.945/256, -74.494/256, 112.439/256,
 *                112.439/256, -94.154/256, -18.285/256 };
 *    offset[] = { 16, 128, 128 };
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2YCC = __mlib_ImageColorRGB2YCC

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2YCC) mlib_ImageColorRGB2YCC
    __attribute__((weak, alias("__mlib_ImageColorRGB2YCC")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u32 t_r2ycc[256] = {
	1059840, 1059840, 1125377, 1125377, 1125122, 1125122, 1190659, 1190659,
	1190660, 1190660, 1256196, 1255941, 1255941, 1255942, 1321478, 1321479,
	1321479, 1321223, 1386760, 1386760, 1386761, 1386761, 1452298, 1452298,
	1452043, 1452043, 1517579, 1517580, 1517580, 1517581, 1583117, 1582862,
	1582862, 1582862, 1648399, 1648399, 1648400, 1713936, 1713681, 1713681,
	1713682, 1779218, 1779218, 1779219, 1778963, 1844500, 1844500, 1844501,
	1844501, 1910038, 1910038, 1909782, 1909783, 1975319, 1975320, 1975320,
	1975321, 2040857, 2040601, 2040602, 2040602, 2106139, 2106139, 2106140,
	2106140, 2171421, 2171421, 2171421, 2171422, 2236958, 2236959, 2236703,
	2236704, 2302240, 2302241, 2302241, 2367777, 2367778, 2367522, 2367523,
	2433059, 2433060, 2433060, 2433060, 2498597, 2498341, 2498342, 2498342,
	2563879, 2563879, 2563880, 2563880, 2629160, 2629161, 2629161, 2629162,
	2694698, 2694699, 2694443, 2694443, 2759980, 2759980, 2759981, 2759981,
	2825518, 2825262, 2825263, 2825263, 2890799, 2890800, 2890800, 2956337,
	2956081, 2956082, 2956082, 3021619, 3021619, 3021619, 3021620, 3086900,
	3086901, 3086901, 3086902, 3152438, 3152438, 3152183, 3152183, 3217720,
	3217720, 3217721, 3217721, 3283258, 3283002, 3283002, 3283003, 3348539,
	3348540, 3348540, 3348541, 3413821, 3413821, 3413822, 3413822, 3479359,
	3479359, 3479360, 3479104, 3544641, 3544641, 3544641, 3610178, 3610178,
	3609923, 3609923, 3675460, 3675460, 3675461, 3675461, 3740997, 3740742,
	3740742, 3740743, 3806279, 3806280, 3806280, 3806280, 3871561, 3871561,
	3871562, 3871562, 3937099, 3937099, 3937100, 3936844, 4002380, 4002381,
	4002381, 4002382, 4067918, 4067663, 4067663, 4067663, 4133200, 4133200,
	4133201, 4198737, 4198482, 4198482, 4198483, 4264019, 4264019, 4264020,
	4264020, 4329301, 4329301, 4329302, 4329302, 4394839, 4394839, 4394839,
	4394584, 4460120, 4460121, 4460121, 4460122, 4525658, 4525402, 4525403,
	4525403, 4590940, 4590940, 4590941, 4590941, 4656222, 4656222, 4656222,
	4656223, 4721759, 4721760, 4721760, 4721505, 4787041, 4787042, 4787042,
	4852578, 4852579, 4852579, 4852324, 4917860, 4917861, 4917861, 4917861,
	4983398, 4983142, 4983143, 4983143, 5048680, 5048680, 5048681, 5048681,
	5113961, 5113962, 5113962, 5113963, 5179499, 5179500, 5179500, 5179244,
	5244781, 5244781, 5244782, 5244782, 5310319, 5310063, 5310064, 5310064
};

/* *********************************************************** */

static const mlib_u32 t_g2ycc[256] = {
	21604, 87140, 86883, 152419, 152419, 217954, 217698, 283233,
	283233, 348513, 348512, 414048, 414048, 479327, 479327, 544862,
	544606, 610142, 610141, 675421, 675421, 740956, 740956, 806236,
	806235, 871771, 871514, 937050, 937050, 1002585, 1002329, 1067865,
	1067864, 1133144, 1133143, 1198679, 1198679, 1263958, 1263958, 1329494,
	1329237, 1394773, 1394773, 1460052, 1460052, 1525587, 1525587, 1590867,
	1590866, 1656402, 1656146, 1721681, 1721681, 1787217, 1786960, 1852496,
	1852495, 1917775, 1917775, 1983310, 1983310, 2048590, 2048589, 2114125,
	2113868, 2179404, 2179404, 2244939, 2244683, 2310219, 2310218, 2375498,
	2375498, 2441033, 2440777, 2506312, 2506312, 2571848, 2571591, 2637127,
	2637127, 2702406, 2702406, 2767941, 2767941, 2833221, 2833220, 2898756,
	2898500, 2964035, 2964035, 3029571, 3029314, 3094850, 3094849, 3160129,
	3160129, 3225664, 3225408, 3290944, 3290943, 3356479, 3356222, 3421758,
	3421758, 3487037, 3487037, 3552573, 3552572, 3617852, 3617852, 3683387,
	3683131, 3748666, 3748666, 3814202, 3813945, 3879481, 3879481, 3944760,
	3944760, 4010295, 4075575, 4075575, 4141110, 4141110, 4206390, 4206389,
	4271925, 4271669, 4337204, 4337204, 4402739, 4402483, 4468019, 4468018,
	4533298, 4533298, 4598833, 4598833, 4664113, 4664112, 4729648, 4729391,
	4794927, 4794927, 4860462, 4860206, 4925742, 4925741, 4991021, 4991020,
	5056556, 5056300, 5121835, 5121835, 5187371, 5187114, 5252650, 5252650,
	5317929, 5317929, 5383464, 5383464, 5448744, 5448743, 5514279, 5514023,
	5579558, 5579558, 5645093, 5644837, 5710373, 5710372, 5775652, 5775652,
	5841187, 5840931, 5906467, 5906466, 5972002, 5971745, 6037281, 6037281,
	6102560, 6102560, 6168096, 6168095, 6233375, 6233374, 6298910, 6298654,
	6364189, 6364189, 6429725, 6429468, 6495004, 6495004, 6560283, 6560283,
	6625818, 6625818, 6691098, 6691097, 6756633, 6756377, 6821912, 6821912,
	6887191, 6887191, 6952727, 6952726, 7018006, 7018006, 7083541, 7083285,
	7148821, 7148820, 7214356, 7214099, 7279635, 7279635, 7344914, 7344914,
	7410450, 7410449, 7475729, 7475729, 7541264, 7541008, 7606543, 7606543,
	7671823, 7671822, 7737358, 7737358, 7802637, 7802637, 7868172, 7867916,
	7933452, 7933451, 7998987, 8064267, 8064266, 8129802, 8129546, 8195081,
	8195081, 8260616, 8260360, 8325896, 8325895, 8391175, 8391175, 8456710
};

/* *********************************************************** */

static const mlib_u32 t_b2ycc[256] = {
	28, 28, 284, 284, 540, 540, 66332, 66332,
	66587, 66587, 66587, 66843, 66843, 67099, 67099, 67355,
	132891, 132891, 133147, 133147, 133403, 133403, 133658, 133658,
	133914, 133914, 199450, 199706, 199706, 199962, 199962, 200218,
	200218, 200218, 200474, 200474, 266265, 266265, 266521, 266521,
	266777, 266777, 266777, 267033, 267033, 267289, 332825, 333081,
	333081, 333337, 333336, 333336, 333592, 333592, 333848, 333848,
	334104, 399640, 399640, 399896, 399896, 400152, 400152, 400408,
	400407, 400663, 400663, 466199, 466455, 466455, 466711, 466711,
	466967, 466967, 467223, 467223, 467223, 533015, 533014, 533270,
	533270, 533526, 533526, 533526, 533782, 533782, 534038, 599574,
	599830, 599830, 600086, 600086, 600085, 600341, 600341, 600597,
	600597, 600853, 666389, 666389, 666645, 666645, 666901, 666901,
	667157, 667157, 667412, 667412, 732948, 733204, 733204, 733460,
	733460, 733716, 733716, 733972, 733972, 733972, 799764, 799764,
	800019, 800019, 800275, 800275, 800275, 800531, 800531, 800787,
	866323, 866579, 866579, 866835, 866835, 866835, 867090, 867090,
	867346, 867346, 933138, 933138, 933138, 933394, 933394, 933650,
	933650, 933906, 933906, 934162, 934161, 999697, 999953, 999953,
	1000209, 1000209, 1000465, 1000465, 1000721, 1000721, 1000721, 1066513,
	1066513, 1066769, 1066768, 1067024, 1067024, 1067024, 1067280, 1067280,
	1067536, 1133072, 1133328, 1133328, 1133584, 1133584, 1133584, 1133840,
	1133839, 1134095, 1134095, 1199887, 1199887, 1199887, 1200143, 1200143,
	1200399, 1200399, 1200655, 1200655, 1200911, 1266447, 1266446, 1266702,
	1266702, 1266958, 1266958, 1267214, 1267214, 1267470, 1267470, 1267470,
	1333262, 1333262, 1333518, 1333518, 1333773, 1333773, 1333773, 1334029,
	1334029, 1334285, 1399821, 1400077, 1400077, 1400333, 1400333, 1400333,
	1400589, 1400589, 1400844, 1400844, 1466636, 1466636, 1466892, 1466892,
	1466892, 1467148, 1467148, 1467404, 1467404, 1467660, 1533196, 1533196,
	1533451, 1533451, 1533707, 1533707, 1533963, 1533963, 1534219, 1534219,
	1534219, 1600011, 1600011, 1600267, 1600267, 1600523, 1600522, 1600522,
	1600778, 1600778, 1601034, 1666570, 1666826, 1666826, 1667082, 1667082
};

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32_U8(x, a0, a1, a2, a3)                         \
	a3 = (x >> 24);                                         \
	a2 = (x & 0xff0000) >> 16;                              \
	a1 = (x & 0x00ff00) >> 8;                               \
	a0 = (x & 0x0000ff)

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32_U8(x, a0, a1, a2, a3)                         \
	a0 = (x >> 24);                                         \
	a1 = (x & 0xff0000) >> 16;                              \
	a2 = (x & 0x00ff00) >> 8;                               \
	a3 = (x & 0x0000ff)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	MLIB_CONVERT_RGB2YCC_U8_1(pd, ps)                       \
	mlib_u32 r0, g0, b0;                                    \
	mlib_u32 s0;                                            \
	                                                        \
	r0 = ps[0];                                             \
	g0 = ps[1];                                             \
	b0 = ps[2];                                             \
	s0 = t_r2ycc[r0] + t_g2ycc[g0] + t_b2ycc[b0];           \
	pd[0] = s0 >> 16;                                       \
	pd[1] = s0 >> 8;                                        \
	pd[2] = s0

/* *********************************************************** */

#define	MLIB_CONVERT_RGB2YCC_4s(ps, s0, s1, s2, s3)                    \
	{                                                              \
	    mlib_u32 r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;   \
	    mlib_u32 r0g0b0r1, g1b1r2g2, b2r3g3b3;                     \
	                                                               \
	    r0g0b0r1 = ps[0];                                          \
	    g1b1r2g2 = ps[1];                                          \
	    b2r3g3b3 = ps[2];                                          \
	    SPLIT_S32_U8(r0g0b0r1, r0, g0, b0, r1);                    \
	    SPLIT_S32_U8(g1b1r2g2, g1, b1, r2, g2);                    \
	    SPLIT_S32_U8(b2r3g3b3, b2, r3, g3, b3);                    \
	    s0 = t_r2ycc[r0] + t_g2ycc[g0] + t_b2ycc[b0];              \
	    s1 = t_r2ycc[r1] + t_g2ycc[g1] + t_b2ycc[b1];              \
	    s2 = t_r2ycc[r2] + t_g2ycc[g2] + t_b2ycc[b2];              \
	    s3 = t_r2ycc[r3] + t_g2ycc[g3] + t_b2ycc[b3];              \
	}

#ifdef _LITTLE_ENDIAN

#define	MLIB_CONVERT_RGB2YCC_4aa(pd, ps)                                   \
	mlib_u32 s0, s1, s2, s3;                                           \
	                                                                   \
	MLIB_CONVERT_RGB2YCC_4s(ps, s0, s1, s2, s3);                       \
	pd[0] =                                                            \
	    ((s1 & 0xFF0000) << 8) | ((s0 & 0xFF) << 16) | (s0 & 0xFF00)   \
	    | ((s0 & 0xFF0000) >> 16);                                     \
	pd[1] =                                                            \
	    ((s2 & 0xFF00) << 16) | (s2 & 0xFF0000) | ((s1 & 0xFF) << 8)   \
	    | ((s1 & 0xFF00) >> 8);                                        \
	pd[2] =                                                            \
	    ((s3 & 0xFF) << 24) | ((s3 & 0xFF00) << 8) | ((s3 &            \
	    0xFF0000) >> 8) | (s2 & 0xFF)

#else /* _LITTLE_ENDIAN */

#define	MLIB_CONVERT_RGB2YCC_4aa(pd, ps)                        \
	mlib_u32 s0, s1, s2, s3;                                \
	                                                        \
	MLIB_CONVERT_RGB2YCC_4s(ps, s0, s1, s2, s3);            \
	pd[0] = (s0 << 8) | (s1 >> 16);                         \
	pd[1] = (s1 << 16) | (s2 >> 8);                         \
	pd[2] = (s2 << 24) | (s3)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	MLIB_CONVERT_RGB2YCC_4ge(pd, ps)                        \
	mlib_u32 s0, s1, s2, s3;                                \
	                                                        \
	MLIB_CONVERT_RGB2YCC_4s(ps, s0, s1, s2, s3);            \
	pd[0] = s0 >> 16;                                       \
	pd[1] = s0 >> 8;                                        \
	pd[2] = s0;                                             \
	pd[3] = s1 >> 16;                                       \
	pd[4] = s1 >> 8;                                        \
	pd[5] = s1;                                             \
	pd[6] = s2 >> 16;                                       \
	pd[7] = s2 >> 8;                                        \
	pd[8] = s2;                                             \
	pd[9] = s3 >> 16;                                       \
	pd[10] = s3 >> 8;                                       \
	pd[11] = s3

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2YCC(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 fmat[9] = { 65.738 / 256, 129.057 / 256, 25.064 / 256,
		-37.945 / 256, -74.494 / 256, 112.439 / 256,
		112.439 / 256, -94.154 / 256, -18.285 / 256
	};
	mlib_d64 offset[3] = { 16, 128, 128 };
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	mlib_u8 *psrc, *pdst;
	mlib_s32 j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_BYTE) {

		if (3 * xsize == slb && dlb == slb) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {
			mlib_u8 *ps = psrc,
			    *pd = pdst, *pend = pdst + 3 * xsize;
			mlib_u32 *ps4, *pd4;
			mlib_u32 *pend4 = (mlib_u32 *)(pend - 12);

/* first pixels loop */
			while (((mlib_addr)ps & 7) && (pd < pend)) {
				MLIB_CONVERT_RGB2YCC_U8_1(pd, ps);
				pd += 3;
				ps += 3;
			}

/* 4 pixel loop */
			ps4 = (mlib_u32 *)ps;

/* pd unaligned */
			if ((mlib_addr)pd & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd <= (pend - 12); pd += 12) {
					MLIB_CONVERT_RGB2YCC_4ge(pd, ps4);
					ps4 += 3;
				}

			} else {
/* all aligned */
				pd4 = (mlib_u32 *)pd;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; pd4 <= pend4; pd4 += 3) {
					MLIB_CONVERT_RGB2YCC_4aa(pd4, ps4);
					ps4 += 3;
				}

				pd = (mlib_u8 *)pd4;
			}

			ps = (mlib_u8 *)ps4;

/* last pixels loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; pd < pend; pd += 3, ps += 3) {
				MLIB_CONVERT_RGB2YCC_U8_1(pd, ps);
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);

	} else {

		return (__mlib_ImageColorConvert2(dst, src, fmat, offset));
	}
}

/* *********************************************************** */