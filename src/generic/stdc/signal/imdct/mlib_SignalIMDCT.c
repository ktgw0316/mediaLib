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

#pragma ident	"@(#)mlib_SignalIMDCT.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *  NOTE: IMDCT below means I(nverse)M(odified)D(iscrete)C(osine)T(ransform)
 *      mlib_SignalIMDCT_D64      - IMDCT of double format complex array into
 *                                  complex array
 *      mlib_SignalIMDCTSplit_D64 - IMDCT of double format 2 complex arrays
 *                                  interleaved into complex array
 *      mlib_SignalIMDCT_F32      - IMDCT of float format complex array into
 *                                  complex array
 *      mlib_SignalIMDCTSplit_F32 - IMDCT of float format 2 complex arrays
 *                                  interleaved into complex array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalIMDCT_D64(mlib_d64 *data);
 *      mlib_status mlib_SignalIMDCTSplit_D64(mlib_d64 *data);
 *      mlib_status mlib_SignalIMDCT_F32(mlib_f32 *data);
 *      mlib_status mlib_SignalIMDCTSplit_F32(mlib_f32 *data);
 *
 * ARGUMENTS
 *      data    - Complex source signal array.
 *                data[ 2 * i     ] hold the real parts,
 *                data[ 2 * i + 1 ] hold the imaginary parts
 *                For *Split* functions - two interleaved complex
 *                source signal arrays.
 *                data[ 4 * i     ] hold the real parts of 1st array,
 *                data[ 4 * i + 1 ] hold the real parts of 2nd array,
 *                data[ 4 * i + 2 ] hold the imaginary parts of 1st array,
 *                data[ 4 * i + 3 ] hold the imaginary parts of 2nd array
 *                Also destination array, hols results in special order
 *
 * DESCRIPTION
 *
 *      data = IMDCT( [Split]( data ) )
 */

/* *********************************************************** */

/*
 * IMPLEMENTATION
 *
 * Uses FFT for better performance
 *  assume constant size of data array
 *  defined in IMDCT_ORDER and IMDCT_SPLIT_ORDER
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalIMDCTUtil.h>

/* *********************************************************** */
#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalIMDCTSplit_D64 = __mlib_SignalIMDCTSplit_D64
#pragma weak mlib_SignalIMDCTSplit_F32 = __mlib_SignalIMDCTSplit_F32
#pragma weak mlib_SignalIMDCT_D64 = __mlib_SignalIMDCT_D64
#pragma weak mlib_SignalIMDCT_F32 = __mlib_SignalIMDCT_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalIMDCTSplit_D64) mlib_SignalIMDCTSplit_D64
    __attribute__((weak, alias("__mlib_SignalIMDCTSplit_D64")));
__typeof__(__mlib_SignalIMDCTSplit_F32) mlib_SignalIMDCTSplit_F32
    __attribute__((weak, alias("__mlib_SignalIMDCTSplit_F32")));
__typeof__(__mlib_SignalIMDCT_D64) mlib_SignalIMDCT_D64
    __attribute__((weak, alias("__mlib_SignalIMDCT_D64")));
__typeof__(__mlib_SignalIMDCT_F32) mlib_SignalIMDCT_F32
    __attribute__((weak, alias("__mlib_SignalIMDCT_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* *********************************************************** */

/*  orders for imdct, imdct_split  */
#define	IMDCT_ORDER	8
#define	IMDCT_SPLIT_ORDER	7

#if (IMDCT_ORDER < 1) || (IMDCT_ORDER > 8)
#error wrong IMDCT_ORDER
#endif /* ( IMDCT_ORDER < 1 ) || ( IMDCT_ORDER > 8 ) */

#if (IMDCT_SPLIT_ORDER < 1) || (IMDCT_SPLIT_ORDER > 8)
#error wrong IMDCT_ORDER
#endif /* ( IMDCT_SPLIT_ORDER < 1 ) || ( IMDCT_SPLIT_ORDER > 8 ) */

/* *********************************************************** */

#define	IMDCT_D64_FFT(d_r, d_i, src0, src1)                    \
		mlib_imdct_D64_FFT8(d_r, d_i, src0, src1)
#define	IMDCT_F32_FFT(d_r, d_i, src0, src1)                    \
		mlib_imdct_F32_FFT8(d_r, d_i, src0, src1)
#define	IMDCT_D64_SPLIT_FFT(d_r, d_i, src0, src1)              \
		mlib_imdct_D64_FFT7(d_r, d_i, src0, src1)
#define	IMDCT_F32_SPLIT_FFT(d_r, d_i, src0, src1)              \
		mlib_imdct_F32_FFT7(d_r, d_i, src0, src1)

/* *********************************************************** */

/*  arrays sizes  */
#define	FIRST_LEN	3
#define	ROTS_LEN_ORDER	8
#define	ROTS_LEN	(1 << ROTS_LEN_ORDER)
#define	EIGHT	8
#define	N512	512

#define	WORKING_T	deal_d64_t

/* *********************************************************** */

/*  declarations  */

/*  service function prototypes  */

#if (IMDCT_ORDER > 4) || (IMDCT_SPLIT_ORDER > 4)

static void mlib_IMDCT_D64_2LayersLast_RI(
    deal_d64_t *CONST dat_r,
    deal_d64_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr);

static void mlib_IMDCT_F32_2LayersLast_RI(
    deal_f32_t *CONST dat_r,
    deal_f32_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr);

#endif /* ( IMDCT_ORDER > 4 ) || ( IMDCT_SPLIT_ORDER > 4 ) */

#if (IMDCT_ORDER > 5) || (IMDCT_SPLIT_ORDER > 5)

static void mlib_IMDCT_D64_2Layers_RI(
    deal_d64_t *CONST dat_r,
    deal_d64_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST n_ord_lay);

static void mlib_IMDCT_F32_2Layers_RI(
    deal_f32_t *CONST dat_r,
    deal_f32_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST n_ord_lay);

#endif /* ( IMDCT_ORDER > 5 ) || ( IMDCT_SPLIT_ORDER > 5 ) */

/* *********************************************************** */

/*  service tables declarations  */
static const deal_d64_t mlib_first_shifts[FIRST_LEN][2];
static const deal_d64_t mlib_rots[ROTS_LEN][2];
static const mlib_s32 mlib_bitreversals[1 << EIGHT];

/* *********************************************************** */

/*  service tables  */

static const deal_d64_t mlib_first_shifts[FIRST_LEN][2] = {
	0.999998823451701, 0.001533980186284,
	0.999995293809576, 0.003067956762965,
	0.999981175282601, 0.006135884649154
};

static const deal_d64_t mlib_rots[ROTS_LEN][2] = {
	1.000000000000000, 0.000000000000000,
	0.999924701839144, 0.012271538285719,
	0.999698818696204, 0.024541228522912,
	0.999322384588349, 0.036807222941358,
	0.998795456205172, 0.049067674327418,
	0.998118112900149, 0.061320736302208,
	0.997290456678690, 0.073564563599667,
	0.996312612182778, 0.085797312344439,
	0.995184726672196, 0.098017140329560,
	0.993906970002356, 0.110222207293883,
	0.992479534598709, 0.122410675199216,
	0.990902635427780, 0.134580708507126,
	0.989176509964781, 0.146730474455361,
	0.987301418157858, 0.158858143333861,
	0.985277642388941, 0.170961888760301,
	0.983105487431216, 0.183039887955140,
	0.980785280403230, 0.195090322016128,
	0.978317370719627, 0.207111376192218,
	0.975702130038528, 0.219101240156869,
	0.972939952205560, 0.231058108280671,
	0.970031253194543, 0.242980179903263,
	0.966976471044852, 0.254865659604514,
	0.963776065795439, 0.266712757474898,
	0.960430519415565, 0.278519689385053,
	0.956940335732208, 0.290284677254462,
	0.953306040354193, 0.302005949319228,
	0.949528180593036, 0.313681740398891,
	0.945607325380521, 0.325310292162262,
	0.941544065183020, 0.336889853392220,
	0.937339011912574, 0.348418680249434,
	0.932992798834738, 0.359895036534988,
	0.928506080473215, 0.371317193951837,
	0.923879532511286, 0.382683432365089,
	0.919113851690057, 0.393992040061048,
	0.914209755703530, 0.405241314004989,
	0.909167983090522, 0.416429560097637,
	0.903989293123443, 0.427555093430282,
	0.898674465693953, 0.438616238538527,
	0.893224301195515, 0.449611329654606,
	0.887639620402853, 0.460538710958240,
	0.881921264348355, 0.471396736825997,
	0.876070094195406, 0.482183772079122,
	0.870086991108711, 0.492898192229784,
	0.863972856121586, 0.503538383725717,
	0.857728610000272, 0.514102744193221,
	0.851355193105265, 0.524589682678468,
	0.844853565249707, 0.534997619887097,
	0.838224705554838, 0.545324988422046,
	0.831469612302545, 0.555570233019602,
	0.824589302785025, 0.565731810783613,
	0.817584813151583, 0.575808191417845,
	0.810457198252594, 0.585797857456438,
	0.803207531480644, 0.595699304492433,
	0.795836904608883, 0.605511041404325,
	0.788346427626606, 0.615231590580626,
	0.780737228572094, 0.624859488142386,
	0.773010453362736, 0.634393284163645,
	0.765167265622458, 0.643831542889791,
	0.757208846506484, 0.653172842953776,
	0.749136394523459, 0.662415777590171,
	0.740951125354959, 0.671558954847018,
	0.732654271672412, 0.680600997795453,
	0.724247082951467, 0.689540544737066,
	0.715730825283818, 0.698376249408972,
	0.707106781186547, 0.707106781186547,
	0.698376249408972, 0.715730825283818,
	0.689540544737066, 0.724247082951466,
	0.680600997795453, 0.732654271672412,
	0.671558954847018, 0.740951125354959,
	0.662415777590171, 0.749136394523459,
	0.653172842953776, 0.757208846506484,
	0.643831542889791, 0.765167265622458,
	0.634393284163645, 0.773010453362736,
	0.624859488142386, 0.780737228572094,
	0.615231590580626, 0.788346427626606,
	0.605511041404325, 0.795836904608883,
	0.595699304492433, 0.803207531480644,
	0.585797857456438, 0.810457198252594,
	0.575808191417845, 0.817584813151583,
	0.565731810783613, 0.824589302785025,
	0.555570233019602, 0.831469612302545,
	0.545324988422046, 0.838224705554837,
	0.534997619887097, 0.844853565249707,
	0.524589682678468, 0.851355193105265,
	0.514102744193221, 0.857728610000272,
	0.503538383725717, 0.863972856121586,
	0.492898192229784, 0.870086991108711,
	0.482183772079122, 0.876070094195406,
	0.471396736825997, 0.881921264348354,
	0.460538710958240, 0.887639620402853,
	0.449611329654606, 0.893224301195515,
	0.438616238538527, 0.898674465693953,
	0.427555093430282, 0.903989293123443,
	0.416429560097637, 0.909167983090522,
	0.405241314004989, 0.914209755703530,
	0.393992040061048, 0.919113851690057,
	0.382683432365089, 0.923879532511286,
	0.371317193951837, 0.928506080473215,
	0.359895036534988, 0.932992798834738,
	0.348418680249434, 0.937339011912574,
	0.336889853392220, 0.941544065183020,
	0.325310292162262, 0.945607325380521,
	0.313681740398891, 0.949528180593036,
	0.302005949319228, 0.953306040354193,
	0.290284677254462, 0.956940335732208,
	0.278519689385053, 0.960430519415565,
	0.266712757474898, 0.963776065795439,
	0.254865659604514, 0.966976471044852,
	0.242980179903263, 0.970031253194543,
	0.231058108280671, 0.972939952205560,
	0.219101240156869, 0.975702130038528,
	0.207111376192218, 0.978317370719627,
	0.195090322016128, 0.980785280403230,
	0.183039887955141, 0.983105487431216,
	0.170961888760301, 0.985277642388941,
	0.158858143333861, 0.987301418157858,
	0.146730474455361, 0.989176509964781,
	0.134580708507126, 0.990902635427780,
	0.122410675199216, 0.992479534598709,
	0.110222207293883, 0.993906970002356,
	0.098017140329560, 0.995184726672196,
	0.085797312344439, 0.996312612182778,
	0.073564563599667, 0.997290456678690,
	0.061320736302208, 0.998118112900149,
	0.049067674327418, 0.998795456205172,
	0.036807222941358, 0.999322384588349,
	0.024541228522912, 0.999698818696204,
	0.012271538285719, 0.999924701839144,
	0.000000000000000, 1.000000000000000,
	-0.012271538285719, 0.999924701839144,
	-0.024541228522912, 0.999698818696204,
	-0.036807222941358, 0.999322384588349,
	-0.049067674327418, 0.998795456205172,
	-0.061320736302208, 0.998118112900149,
	-0.073564563599667, 0.997290456678690,
	-0.085797312344439, 0.996312612182778,
	-0.098017140329560, 0.995184726672196,
	-0.110222207293883, 0.993906970002356,
	-0.122410675199216, 0.992479534598709,
	-0.134580708507126, 0.990902635427780,
	-0.146730474455361, 0.989176509964781,
	-0.158858143333861, 0.987301418157858,
	-0.170961888760301, 0.985277642388941,
	-0.183039887955140, 0.983105487431216,
	-0.195090322016128, 0.980785280403230,
	-0.207111376192218, 0.978317370719627,
	-0.219101240156869, 0.975702130038528,
	-0.231058108280671, 0.972939952205560,
	-0.242980179903263, 0.970031253194543,
	-0.254865659604514, 0.966976471044852,
	-0.266712757474898, 0.963776065795439,
	-0.278519689385052, 0.960430519415565,
	-0.290284677254462, 0.956940335732208,
	-0.302005949319228, 0.953306040354193,
	-0.313681740398891, 0.949528180593036,
	-0.325310292162262, 0.945607325380521,
	-0.336889853392219, 0.941544065183020,
	-0.348418680249434, 0.937339011912574,
	-0.359895036534988, 0.932992798834738,
	-0.371317193951837, 0.928506080473215,
	-0.382683432365089, 0.923879532511286,
	-0.393992040061047, 0.919113851690057,
	-0.405241314004989, 0.914209755703530,
	-0.416429560097636, 0.909167983090522,
	-0.427555093430281, 0.903989293123443,
	-0.438616238538527, 0.898674465693953,
	-0.449611329654606, 0.893224301195515,
	-0.460538710958240, 0.887639620402853,
	-0.471396736825997, 0.881921264348355,
	-0.482183772079122, 0.876070094195406,
	-0.492898192229783, 0.870086991108711,
	-0.503538383725717, 0.863972856121586,
	-0.514102744193221, 0.857728610000272,
	-0.524589682678468, 0.851355193105265,
	-0.534997619887097, 0.844853565249707,
	-0.545324988422046, 0.838224705554838,
	-0.555570233019601, 0.831469612302545,
	-0.565731810783613, 0.824589302785025,
	-0.575808191417845, 0.817584813151583,
	-0.585797857456438, 0.810457198252594,
	-0.595699304492433, 0.803207531480644,
	-0.605511041404325, 0.795836904608883,
	-0.615231590580626, 0.788346427626606,
	-0.624859488142386, 0.780737228572094,
	-0.634393284163645, 0.773010453362737,
	-0.643831542889791, 0.765167265622459,
	-0.653172842953776, 0.757208846506484,
	-0.662415777590171, 0.749136394523459,
	-0.671558954847018, 0.740951125354958,
	-0.680600997795453, 0.732654271672412,
	-0.689540544737066, 0.724247082951466,
	-0.698376249408972, 0.715730825283818,
	-0.707106781186547, 0.707106781186547
};

/* *********************************************************** */

static const mlib_s32 mlib_bitreversals[1 << EIGHT] = {
	0, 128, 64, 192, 32, 160, 96, 224,
	16, 144, 80, 208, 48, 176, 112, 240,
	8, 136, 72, 200, 40, 168, 104, 232,
	24, 152, 88, 216, 56, 184, 120, 248,
	4, 132, 68, 196, 36, 164, 100, 228,
	20, 148, 84, 212, 52, 180, 116, 244,
	12, 140, 76, 204, 44, 172, 108, 236,
	28, 156, 92, 220, 60, 188, 124, 252,
	2, 130, 66, 194, 34, 162, 98, 226,
	18, 146, 82, 210, 50, 178, 114, 242,
	10, 138, 74, 202, 42, 170, 106, 234,
	26, 154, 90, 218, 58, 186, 122, 250,
	6, 134, 70, 198, 38, 166, 102, 230,
	22, 150, 86, 214, 54, 182, 118, 246,
	14, 142, 78, 206, 46, 174, 110, 238,
	30, 158, 94, 222, 62, 190, 126, 254,
	1, 129, 65, 193, 33, 161, 97, 225,
	17, 145, 81, 209, 49, 177, 113, 241,
	9, 137, 73, 201, 41, 169, 105, 233,
	25, 153, 89, 217, 57, 185, 121, 249,
	5, 133, 69, 197, 37, 165, 101, 229,
	21, 149, 85, 213, 53, 181, 117, 245,
	13, 141, 77, 205, 45, 173, 109, 237,
	29, 157, 93, 221, 61, 189, 125, 253,
	3, 131, 67, 195, 35, 163, 99, 227,
	19, 147, 83, 211, 51, 179, 115, 243,
	11, 139, 75, 203, 43, 171, 107, 235,
	27, 155, 91, 219, 59, 187, 123, 251,
	7, 135, 71, 199, 39, 167, 103, 231,
	23, 151, 87, 215, 55, 183, 119, 247,
	15, 143, 79, 207, 47, 175, 111, 239,
	31, 159, 95, 223, 63, 191, 127, 255
};

/* *********************************************************** */

/*  code  */

/* *********************************************************** */

#if (IMDCT_ORDER == 1) || (IMDCT_SPLIT_ORDER == 1)
static void
mlib_imdct_D64_FFT1(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	deal_d64_t rot_r, rot_i, val_r, val_i;

	rot_r = COS_PI_BY8;
	rot_i = SIN_PI_BY8;
	val_r = src1[0];
	val_i = src0[0];
	*d_r = MULT_RE();
	*d_i = MULT_IM();
}

#endif /* ( IMDCT_ORDER == 1 ) || ( IMDCT_SPLIT_ORDER == 1 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 2) || (IMDCT_SPLIT_ORDER == 2)
static void
mlib_imdct_D64_FFT2(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	deal_d64_t rot_r, rot_i, val_r, val_i;
	deal_d64_t tmp0_r, tmp0_i, tmp1_r, tmp1_i, tmp2_r, tmp2_i, tmp3_r,
	    tmp3_i;
	deal_d64_t s0_r = src0[0], s0_i = src0[1], s1_r = src1[0], s1_i =
	    src1[1];
	deal_d64_t cos0 = COS_PI_BY16, sin0 = SIN_PI_BY16, cos1 =
	    SIN_PI_BY4 * (cos0 - sin0), sin1 = SIN_PI_BY4 * (cos0 + sin0);

	rot_r = cos0;
	rot_i = sin0;
	val_r = s1_i;
	val_i = s0_r;
	tmp0_r = MULT_RE();
	tmp0_i = MULT_IM();

	val_r = s0_i;
	val_i = s1_r;
	tmp1_r = MULT_RE();
	tmp1_i = MULT_IM();

	rot_r = cos1;
	rot_i = sin1;
	tmp2_r = MULT_RE();
	tmp2_i = MULT_IM();

	val_r = s1_i;
	val_i = s0_r;
	tmp3_r = MULT_RE();
	tmp3_i = MULT_IM();

	d_r[0] = (tmp0_r + tmp2_r);
	d_i[0] = (tmp0_i + tmp2_i);

	d_r[1] = (tmp3_r + tmp1_i);
	d_i[1] = (tmp3_i - tmp1_r);
}

#endif /* ( IMDCT_ORDER == 2 ) || ( IMDCT_SPLIT_ORDER == 2 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 3) || (IMDCT_SPLIT_ORDER == 3)
static void
mlib_imdct_D64_FFT3(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	deal_d64_t cosshift, sinshift;
	deal_d64_t *r3 = d_r;
	deal_d64_t *i3 = d_i;
	deal_d64_t buffer[4 * N512];
	deal_d64_t *r1 = buffer, *i1 = r1 + 4, *r2 = i1 + 4, *i2 = r2 + 4;
	deal_d64_t *work_r = r2, *work_i = i2;
	deal_d64_t rot_r, rot_i, val_r, val_i;
	deal_d64_t rot0cos, rot0sin;
	int i, j;

	if (3 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 3][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 3][1];
	} else {
		cosshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 3)][0];
		sinshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 3)][1];
	}

/*  pre_cp section  */
	rot_r = -cosshift;
	rot_i = -sinshift;
	val_r = src1[3];
	val_i = src0[0];
	r1[0] = MULT_RE();
	i1[0] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (cosshift - sinshift);
	rot_i = -SIN_PI_BY4 * (cosshift + sinshift);
	val_r = src0[3];
	val_i = src1[0];
	r1[2] = MULT_RE();
	i1[2] = MULT_IM();

	rot_r = COS_PI_BY8;
	rot_i = SIN_PI_BY8;
	val_r = cosshift;
	val_i = sinshift;
	rot0cos = MULT_RE();
	rot0sin = MULT_IM();

	rot_r = -rot0cos;
	rot_i = -rot0sin;
	val_r = src1[1];
	val_i = src0[2];
	r1[1] = MULT_RE();
	i1[1] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
	rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
	val_r = src0[1];
	val_i = src1[2];
	r1[3] = MULT_RE();
	i1[3] = MULT_IM();

/*  std_ifft section  */
	DEAL_ALL_ORD_2(AS_IS, work, r1, i1);

/*  post_cp section  */
	rot_r = -cosshift;
	rot_i = -sinshift;
	val_r = r2[0];
	val_i = i2[0];
	r3[0] = MULT_RE();
	i3[0] = MULT_IM();

	val_r = -COS_PI_BY8;
	val_i = -SIN_PI_BY8;
	rot0cos = MULT_RE();
	rot0sin = MULT_IM();

	rot_r = -rot0cos;
	rot_i = -rot0sin;
	val_r = r2[1];
	val_i = i2[1];
	r3[1] = MULT_RE();
	i3[1] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (cosshift - sinshift);
	rot_i = -SIN_PI_BY4 * (cosshift + sinshift);
	val_r = r2[2];
	val_i = i2[2];
	r3[2] = MULT_RE();
	i3[2] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
	rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
	val_r = r2[3];
	val_i = i2[3];
	r3[3] = MULT_RE();
	i3[3] = MULT_IM();
}

#endif /* ( IMDCT_ORDER == 3 ) || ( IMDCT_SPLIT_ORDER == 3 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 4) || (IMDCT_SPLIT_ORDER == 4)
static void
mlib_imdct_D64_FFT4(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	int index_delta, index_rot_angle;
	deal_d64_t cosshift, sinshift;
	deal_d64_t *r3 = d_r;
	deal_d64_t *i3 = d_i;
	deal_d64_t buffer[4 * N512];
	deal_d64_t *r1 = buffer, *i1 = r1 + 8, *r2 = i1 + 8, *i2 = r2 + 8;
	deal_d64_t *work_r = r2, *work_i = i2;
	deal_d64_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (4 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 4][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 4][1];
	} else {
		cosshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 4)][0];
		sinshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 4)][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 4;
	index_rot_angle = 0;
	for (i = 0, j = 3; i < 4; i++, j--) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 4)] = MULT_RE();
		i1[(i + 4)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	DEAL_ALL_ORD_3(AS_IS, work, r1, i1);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 4;
	index_rot_angle = 0;
	for (i = 0; i < 4; i++) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 4];
		val_i = i2[i + 4];
		r3[i + 4] = MULT_RE();
		i3[i + 4] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 4 ) || ( IMDCT_SPLIT_ORDER == 4 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 5) || (IMDCT_SPLIT_ORDER == 5)
static void
mlib_imdct_D64_FFT5(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	int index_delta, index_rot_angle;
	deal_d64_t cosshift, sinshift;
	deal_d64_t *r3 = d_r;
	deal_d64_t *i3 = d_i;
	deal_d64_t buffer[4 * N512];
	deal_d64_t *r1 = buffer, *i1 = r1 + 16, *r2 = i1 + 16, *i2 = r2 + 16;
	deal_d64_t *work_r = r2, *work_i = i2;
	deal_d64_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (5 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 5][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 5][1];
	} else {
		cosshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 5)][0];
		sinshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 5)][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 5;
	index_rot_angle = 0;
	for (i = 0, j = 7; i < 8; i++, j--) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 8)] = MULT_RE();
		i1[(i + 8)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_2(8, 4, AS_IS, work, r1, i1);
	DEAL_ALL_ORD4_RI(1, work, AS_IS);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 5;
	index_rot_angle = 0;
	for (i = 0; i < 8; i++) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 8];
		val_i = i2[i + 8];
		r3[i + 8] = MULT_RE();
		i3[i + 8] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 5 ) || ( IMDCT_SPLIT_ORDER == 5 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 6) || (IMDCT_SPLIT_ORDER == 6)
static void
mlib_imdct_D64_FFT6(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	int index_delta, index_rot_angle;
	deal_d64_t cosshift, sinshift;
	deal_d64_t *r3 = d_r;
	deal_d64_t *i3 = d_i;
	deal_d64_t buffer[4 * N512];
	deal_d64_t *r1 = buffer, *i1 = r1 + 32, *r2 = i1 + 32, *i2 = r2 + 32;
	deal_d64_t *work_r = r2, *work_i = i2;
	deal_d64_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (6 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 6][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 6][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 6;
	index_rot_angle = 0;
	for (i = 0, j = 15; i < 16; i++, j--) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 16)] = MULT_RE();
		i1[(i + 16)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_3(8, 5, AS_IS, work, r1, i1);
	mlib_IMDCT_D64_2LayersLast_RI(work_r, work_i, 5);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 6;
	index_rot_angle = 0;
	for (i = 0; i < 16; i++) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 16];
		val_i = i2[i + 16];
		r3[i + 16] = MULT_RE();
		i3[i + 16] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 6 ) || ( IMDCT_SPLIT_ORDER == 6 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 7) || (IMDCT_SPLIT_ORDER == 7)
static void
mlib_imdct_D64_FFT7(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	int index_delta, index_rot_angle;
	deal_d64_t cosshift, sinshift;
	deal_d64_t *r3 = d_r;
	deal_d64_t *i3 = d_i;
	deal_d64_t buffer[4 * N512];
	deal_d64_t *r1 = buffer, *i1 = r1 + 64, *r2 = i1 + 64, *i2 = r2 + 64;
	deal_d64_t *work_r = r2, *work_i = i2;
	deal_d64_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (7 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 7][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 7][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 7;
	index_rot_angle = 0;
	for (i = 0, j = 31; i < 32; i++, j--) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 32)] = MULT_RE();
		i1[(i + 32)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_2(8, 6, AS_IS, work, r1, i1);
	mlib_IMDCT_D64_2Layers_RI(work_r, work_i, 6, 2);
	mlib_IMDCT_D64_2LayersLast_RI(work_r, work_i, 6);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 7;
	index_rot_angle = 0;
	for (i = 0; i < 32; i++) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 32];
		val_i = i2[i + 32];
		r3[i + 32] = MULT_RE();
		i3[i + 32] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 7 ) || ( IMDCT_SPLIT_ORDER == 7 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 8) || (IMDCT_SPLIT_ORDER == 8)
static void
mlib_imdct_D64_FFT8(
    deal_d64_t *d_r,
    deal_d64_t *d_i,
    deal_d64_t *src0,
    deal_d64_t *src1)
{
	int index_delta, index_rot_angle;
	deal_d64_t cosshift, sinshift;
	deal_d64_t *r3 = d_r;
	deal_d64_t *i3 = d_i;
	deal_d64_t buffer[4 * N512];
	deal_d64_t *r1 = buffer, *i1 = r1 + 128, *r2 = i1 + 128, *i2 = r2 + 128;
	deal_d64_t *work_r = r2, *work_i = i2;
	deal_d64_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (8 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 8][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 8][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 8;
	index_rot_angle = 0;
	for (i = 0, j = 63; i < 64; i++, j--) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 64)] = MULT_RE();
		i1[(i + 64)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_3(8, 7, AS_IS, work, r1, i1);
	mlib_IMDCT_D64_2Layers_RI(work_r, work_i, 7, 3);
	mlib_IMDCT_D64_2LayersLast_RI(work_r, work_i, 7);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 8;
	index_rot_angle = 0;
	for (i = 0; i < 64; i++) {
		deal_d64_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 64];
		val_i = i2[i + 64];
		r3[i + 64] = MULT_RE();
		i3[i + 64] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 8 ) || ( IMDCT_SPLIT_ORDER == 8 ) */

/* *********************************************************** */

#if (IMDCT_ORDER > 5) || (IMDCT_SPLIT_ORDER > 5)
static void
mlib_IMDCT_D64_2Layers_RI(
    deal_d64_t *CONST dat_r,
    deal_d64_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST n_ord_lay)
{
	CONST mlib_s32
	    n_len_arr = 1 << n_ord_arr,
	    n_step = 1 << n_ord_lay,
	    three_n_step = n_step * 3,
	    four_n_step = n_step * 4,
	    n_mirr_shift = n_len_arr - three_n_step, n_step_half = n_step >> 1;
	mlib_s32 mmm, iii;
	int index_delta, index_rot_angle;

	for (mmm = 0; mmm < n_len_arr; mmm += four_n_step) {
		deal_d64_t *dat_mmm_r = dat_r + mmm, *dat_mmm_i = dat_i + mmm;

		DEAL_4_SIMPLE_RI(dat_mmm, n_step, AS_IS);
		{
			deal_d64_t *dat_mmm_r = dat_r + mmm + n_step_half,
			    *dat_mmm_i = dat_i + mmm + n_step_half;
			DEAL_4_HALFWAY_RI(dat_mmm, n_step, AS_IS);
		}
	}

	index_delta = ROTS_LEN >> (n_ord_lay + 1);
	index_rot_angle = index_delta;
	for (iii = 1; iii < n_step_half; iii++) {
		deal_d64_t rot_half_r = mlib_rots[1 * index_rot_angle][0];
		deal_d64_t rot_half_i = mlib_rots[1 * index_rot_angle][1];
		deal_d64_t rot_r = mlib_rots[2 * index_rot_angle][0];
		deal_d64_t rot_i = mlib_rots[2 * index_rot_angle][1];
		deal_d64_t r_rh_r = mlib_rots[3 * index_rot_angle][0];
		deal_d64_t r_rh_i = mlib_rots[3 * index_rot_angle][1];

		for (mmm = iii; mmm < n_len_arr; mmm += four_n_step) {
			deal_d64_t *dat_mmm_r = dat_r + mmm,
			    *dat_mmm_i = dat_i + mmm;
			deal_d64_t *dat_mirr_r = dat_r + n_mirr_shift - mmm,
			    *dat_mirr_i = dat_i + n_mirr_shift - mmm;
			DEAL_8_USUAL_MIRR_RI(dat_mmm, dat_mirr,
			    n_step, rot, rot_half, r_rh, AS_IS);
		}

		index_rot_angle += index_delta;
	}	/* end for( ; iii < n_step_half; ) */
}

#endif /* ( IMDCT_ORDER > 5 ) || ( IMDCT_SPLIT_ORDER > 5 ) */

/* *********************************************************** */

#if (IMDCT_ORDER > 4) || (IMDCT_SPLIT_ORDER > 4)
static void
mlib_IMDCT_D64_2LayersLast_RI(
    deal_d64_t *CONST dat_r,
    deal_d64_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr)
{
	CONST mlib_s32
	    n_ord_lay = n_ord_arr - 2,
	    n_step = 1 << n_ord_lay, n_step_half = n_step >> 1;
	mlib_s32 iii;
	int index_delta, index_rot_angle;

	{
		deal_d64_t *dat_mmm_r = dat_r, *dat_mmm_i = dat_i;

		DEAL_4_SIMPLE_RI(dat_mmm, n_step, AS_IS);
		{
			deal_d64_t *dat_mmm_r = dat_r + n_step_half,
			    *dat_mmm_i = dat_i + n_step_half;
			DEAL_4_HALFWAY_RI(dat_mmm, n_step, AS_IS);
		}

		index_delta = ROTS_LEN >> (n_ord_lay + 1);
		index_rot_angle = index_delta;
		for (iii = 1; iii < n_step_half; iii++) {
			deal_d64_t rot_half_r =
			    mlib_rots[1 * index_rot_angle][0];
			deal_d64_t rot_half_i =
			    mlib_rots[1 * index_rot_angle][1];
			deal_d64_t rot_r = mlib_rots[2 * index_rot_angle][0];
			deal_d64_t rot_i = mlib_rots[2 * index_rot_angle][1];
			deal_d64_t r_rh_r = mlib_rots[3 * index_rot_angle][0];
			deal_d64_t r_rh_i = mlib_rots[3 * index_rot_angle][1];

			{
				deal_d64_t *dat_mmm_r = dat_r + iii,
				    *dat_mmm_i = dat_i + iii;
				deal_d64_t *dat_mirr_r = dat_r + n_step - iii,
				    *dat_mirr_i = dat_i + n_step - iii;
				DEAL_8_USUAL_MIRR_RI(dat_mmm, dat_mirr,
				    n_step, rot, rot_half, r_rh, AS_IS);
			}

			index_rot_angle += index_delta;
		}
	}
}

#endif /* ( IMDCT_ORDER > 4 ) || ( IMDCT_SPLIT_ORDER > 4 ) */

/* *********************************************************** */

#undef WORKING_T
#define	WORKING_T	deal_f32_t

/* *********************************************************** */

#if (IMDCT_ORDER == 1) || (IMDCT_SPLIT_ORDER == 1)
static void
mlib_imdct_F32_FFT1(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	deal_f32_t rot_r, rot_i, val_r, val_i;

	rot_r = COS_PI_BY8;
	rot_i = SIN_PI_BY8;
	val_r = src1[0];
	val_i = src0[0];
	*d_r = MULT_RE();
	*d_i = MULT_IM();
}

#endif /* ( IMDCT_ORDER == 1 ) || ( IMDCT_SPLIT_ORDER == 1 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 2) || (IMDCT_SPLIT_ORDER == 2)
static void
mlib_imdct_F32_FFT2(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	deal_f32_t rot_r, rot_i, val_r, val_i;
	deal_f32_t tmp0_r, tmp0_i, tmp1_r, tmp1_i, tmp2_r, tmp2_i, tmp3_r,
	    tmp3_i;
	deal_f32_t s0_r = src0[0], s0_i = src0[1], s1_r = src1[0], s1_i =
	    src1[1];
	deal_f32_t cos0 = COS_PI_BY16, sin0 = SIN_PI_BY16, cos1 =
	    SIN_PI_BY4 * (cos0 - sin0), sin1 = SIN_PI_BY4 * (cos0 + sin0);

	rot_r = cos0;
	rot_i = sin0;
	val_r = s1_i;
	val_i = s0_r;
	tmp0_r = MULT_RE();
	tmp0_i = MULT_IM();

	val_r = s0_i;
	val_i = s1_r;
	tmp1_r = MULT_RE();
	tmp1_i = MULT_IM();

	rot_r = cos1;
	rot_i = sin1;
	tmp2_r = MULT_RE();
	tmp2_i = MULT_IM();

	val_r = s1_i;
	val_i = s0_r;
	tmp3_r = MULT_RE();
	tmp3_i = MULT_IM();

	d_r[0] = (tmp0_r + tmp2_r);
	d_i[0] = (tmp0_i + tmp2_i);

	d_r[1] = (tmp3_r + tmp1_i);
	d_i[1] = (tmp3_i - tmp1_r);
}

#endif /* ( IMDCT_ORDER == 2 ) || ( IMDCT_SPLIT_ORDER == 2 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 3) || (IMDCT_SPLIT_ORDER == 3)
static void
mlib_imdct_F32_FFT3(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	deal_f32_t cosshift, sinshift;
	deal_f32_t *r3 = d_r;
	deal_f32_t *i3 = d_i;
	deal_f32_t buffer[4 * N512];
	deal_f32_t *r1 = buffer, *i1 = r1 + 4, *r2 = i1 + 4, *i2 = r2 + 4;
	deal_f32_t *work_r = r2, *work_i = i2;
	deal_f32_t rot_r, rot_i, val_r, val_i;
	deal_f32_t rot0cos, rot0sin;
	int i, j;

	if (3 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 3][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 3][1];
	} else {
		cosshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 3)][0];
		sinshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 3)][1];
	}

/*  pre_cp section  */
	rot_r = -cosshift;
	rot_i = -sinshift;
	val_r = src1[3];
	val_i = src0[0];
	r1[0] = MULT_RE();
	i1[0] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (cosshift - sinshift);
	rot_i = -SIN_PI_BY4 * (cosshift + sinshift);
	val_r = src0[3];
	val_i = src1[0];
	r1[2] = MULT_RE();
	i1[2] = MULT_IM();

	rot_r = COS_PI_BY8;
	rot_i = SIN_PI_BY8;
	val_r = cosshift;
	val_i = sinshift;
	rot0cos = MULT_RE();
	rot0sin = MULT_IM();

	rot_r = -rot0cos;
	rot_i = -rot0sin;
	val_r = src1[1];
	val_i = src0[2];
	r1[1] = MULT_RE();
	i1[1] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
	rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
	val_r = src0[1];
	val_i = src1[2];
	r1[3] = MULT_RE();
	i1[3] = MULT_IM();

/*  std_ifft section  */
	DEAL_ALL_ORD_2(AS_IS, work, r1, i1);

/*  post_cp section  */
	rot_r = -cosshift;
	rot_i = -sinshift;
	val_r = r2[0];
	val_i = i2[0];
	r3[0] = MULT_RE();
	i3[0] = MULT_IM();

	val_r = -COS_PI_BY8;
	val_i = -SIN_PI_BY8;
	rot0cos = MULT_RE();
	rot0sin = MULT_IM();

	rot_r = -rot0cos;
	rot_i = -rot0sin;
	val_r = r2[1];
	val_i = i2[1];
	r3[1] = MULT_RE();
	i3[1] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (cosshift - sinshift);
	rot_i = -SIN_PI_BY4 * (cosshift + sinshift);
	val_r = r2[2];
	val_i = i2[2];
	r3[2] = MULT_RE();
	i3[2] = MULT_IM();

	rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
	rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
	val_r = r2[3];
	val_i = i2[3];
	r3[3] = MULT_RE();
	i3[3] = MULT_IM();
}

#endif /* ( IMDCT_ORDER == 3 ) || ( IMDCT_SPLIT_ORDER == 3 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 4) || (IMDCT_SPLIT_ORDER == 4)
static void
mlib_imdct_F32_FFT4(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	int index_delta, index_rot_angle;
	deal_f32_t cosshift, sinshift;
	deal_f32_t *r3 = d_r;
	deal_f32_t *i3 = d_i;
	deal_f32_t buffer[4 * N512];
	deal_f32_t *r1 = buffer, *i1 = r1 + 8, *r2 = i1 + 8, *i2 = r2 + 8;
	deal_f32_t *work_r = r2, *work_i = i2;
	deal_f32_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (4 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 4][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 4][1];
	} else {
		cosshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 4)][0];
		sinshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 4)][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 4;
	index_rot_angle = 0;
	for (i = 0, j = 3; i < 4; i++, j--) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 4)] = MULT_RE();
		i1[(i + 4)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	DEAL_ALL_ORD_3(AS_IS, work, r1, i1);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 4;
	index_rot_angle = 0;
	for (i = 0; i < 4; i++) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 4];
		val_i = i2[i + 4];
		r3[i + 4] = MULT_RE();
		i3[i + 4] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 4 ) || ( IMDCT_SPLIT_ORDER == 4 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 5) || (IMDCT_SPLIT_ORDER == 5)
static void
mlib_imdct_F32_FFT5(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	int index_delta, index_rot_angle;
	deal_f32_t cosshift, sinshift;
	deal_f32_t *r3 = d_r;
	deal_f32_t *i3 = d_i;
	deal_f32_t buffer[4 * N512];
	deal_f32_t *r1 = buffer, *i1 = r1 + 16, *r2 = i1 + 16, *i2 = r2 + 16;
	deal_f32_t *work_r = r2, *work_i = i2;
	deal_f32_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (5 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 5][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 5][1];
	} else {
		cosshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 5)][0];
		sinshift = mlib_rots[1 << (ROTS_LEN_ORDER - FIRST_LEN - 5)][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 5;
	index_rot_angle = 0;
	for (i = 0, j = 7; i < 8; i++, j--) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 8)] = MULT_RE();
		i1[(i + 8)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_2(8, 4, AS_IS, work, r1, i1);
	DEAL_ALL_ORD4_RI(1, work, AS_IS);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 5;
	index_rot_angle = 0;
	for (i = 0; i < 8; i++) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 8];
		val_i = i2[i + 8];
		r3[i + 8] = MULT_RE();
		i3[i + 8] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 5 ) || ( IMDCT_SPLIT_ORDER == 5 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 6) || (IMDCT_SPLIT_ORDER == 6)
static void
mlib_imdct_F32_FFT6(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	int index_delta, index_rot_angle;
	deal_f32_t cosshift, sinshift;
	deal_f32_t *r3 = d_r;
	deal_f32_t *i3 = d_i;
	deal_f32_t buffer[4 * N512];
	deal_f32_t *r1 = buffer, *i1 = r1 + 32, *r2 = i1 + 32, *i2 = r2 + 32;
	deal_f32_t *work_r = r2, *work_i = i2;
	deal_f32_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (6 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 6][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 6][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 6;
	index_rot_angle = 0;
	for (i = 0, j = 15; i < 16; i++, j--) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 16)] = MULT_RE();
		i1[(i + 16)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_3(8, 5, AS_IS, work, r1, i1);
	mlib_IMDCT_F32_2LayersLast_RI(work_r, work_i, 5);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 6;
	index_rot_angle = 0;
	for (i = 0; i < 16; i++) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 16];
		val_i = i2[i + 16];
		r3[i + 16] = MULT_RE();
		i3[i + 16] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 6 ) || ( IMDCT_SPLIT_ORDER == 6 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 7) || (IMDCT_SPLIT_ORDER == 7)
static void
mlib_imdct_F32_FFT7(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	int index_delta, index_rot_angle;
	deal_f32_t cosshift, sinshift;
	deal_f32_t *r3 = d_r;
	deal_f32_t *i3 = d_i;
	deal_f32_t buffer[4 * N512];
	deal_f32_t *r1 = buffer, *i1 = r1 + 64, *r2 = i1 + 64, *i2 = r2 + 64;
	deal_f32_t *work_r = r2, *work_i = i2;
	deal_f32_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (7 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 7][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 7][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 7;
	index_rot_angle = 0;
	for (i = 0, j = 31; i < 32; i++, j--) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 32)] = MULT_RE();
		i1[(i + 32)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_2(8, 6, AS_IS, work, r1, i1);
	mlib_IMDCT_F32_2Layers_RI(work_r, work_i, 6, 2);
	mlib_IMDCT_F32_2LayersLast_RI(work_r, work_i, 6);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 7;
	index_rot_angle = 0;
	for (i = 0; i < 32; i++) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 32];
		val_i = i2[i + 32];
		r3[i + 32] = MULT_RE();
		i3[i + 32] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 7 ) || ( IMDCT_SPLIT_ORDER == 7 ) */

/* *********************************************************** */

#if (IMDCT_ORDER == 8) || (IMDCT_SPLIT_ORDER == 8)
static void
mlib_imdct_F32_FFT8(
    deal_f32_t *d_r,
    deal_f32_t *d_i,
    deal_f32_t *src0,
    deal_f32_t *src1)
{
	int index_delta, index_rot_angle;
	deal_f32_t cosshift, sinshift;
	deal_f32_t *r3 = d_r;
	deal_f32_t *i3 = d_i;
	deal_f32_t buffer[4 * N512];
	deal_f32_t *r1 = buffer, *i1 = r1 + 128, *r2 = i1 + 128, *i2 = r2 + 128;
	deal_f32_t *work_r = r2, *work_i = i2;
	deal_f32_t rot_r, rot_i, val_r, val_i;
	int i, j;

	if (8 > ROTS_LEN_ORDER - FIRST_LEN) {
		cosshift = mlib_first_shifts[ROTS_LEN_ORDER - 8][0];
		sinshift = mlib_first_shifts[ROTS_LEN_ORDER - 8][1];
	}

/*  pre_cp section  */
	index_delta = ROTS_LEN >> 8;
	index_rot_angle = 0;
	for (i = 0, j = 63; i < 64; i++, j--) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();

		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = src1[2 * j + 1];
		val_i = src0[2 * i];
		r1[i] = MULT_RE();
		i1[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = src0[2 * j + 1];
		val_i = src1[2 * i];
		r1[(i + 64)] = MULT_RE();
		i1[(i + 64)] = MULT_IM();
		index_rot_angle += index_delta;
	}

/*  std_ifft section  */
	LOOP_REV_3(8, 7, AS_IS, work, r1, i1);
	mlib_IMDCT_F32_2Layers_RI(work_r, work_i, 7, 3);
	mlib_IMDCT_F32_2LayersLast_RI(work_r, work_i, 7);

/*  post_cp section  */
	index_delta = ROTS_LEN >> 8;
	index_rot_angle = 0;
	for (i = 0; i < 64; i++) {
		deal_f32_t rot0cos, rot0sin;

		rot_r = mlib_rots[index_rot_angle][0];
		rot_i = mlib_rots[(ROTS_LEN >> 1) - index_rot_angle][0];
		val_r = cosshift;
		val_i = sinshift;
		rot0cos = MULT_RE();
		rot0sin = MULT_IM();
		rot_r = -rot0cos;
		rot_i = -rot0sin;
		val_r = r2[i];
		val_i = i2[i];
		r3[i] = MULT_RE();
		i3[i] = MULT_IM();

		rot_r = -SIN_PI_BY4 * (rot0cos - rot0sin);
		rot_i = -SIN_PI_BY4 * (rot0cos + rot0sin);
		val_r = r2[i + 64];
		val_i = i2[i + 64];
		r3[i + 64] = MULT_RE();
		i3[i + 64] = MULT_IM();
		index_rot_angle += index_delta;
	}
}

#endif /* ( IMDCT_ORDER == 8 ) || ( IMDCT_SPLIT_ORDER == 8 ) */

/* *********************************************************** */

#if (IMDCT_ORDER > 5) || (IMDCT_SPLIT_ORDER > 5)
static void
mlib_IMDCT_F32_2Layers_RI(
    deal_f32_t *CONST dat_r,
    deal_f32_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST n_ord_lay)
{
	CONST mlib_s32
	    n_len_arr = 1 << n_ord_arr,
	    n_step = 1 << n_ord_lay,
	    three_n_step = n_step * 3,
	    four_n_step = n_step * 4,
	    n_mirr_shift = n_len_arr - three_n_step, n_step_half = n_step >> 1;
	mlib_s32 mmm, iii;
	int index_delta, index_rot_angle;

/*
 *    half way, i.e. mmm = n_step_half, all rots simple:
 *      rot.r = 0;               rot.i = iSIGNED( 1 );
 *      rot_half_r = SIN_PI_BY4; rot_half_i = iSIGNed( SIN_PI_BY4 );
 */
	for (mmm = 0; mmm < n_len_arr; mmm += four_n_step) {
		deal_f32_t *dat_mmm_r = dat_r + mmm, *dat_mmm_i = dat_i + mmm;

		DEAL_4_SIMPLE_RI(dat_mmm, n_step, AS_IS);
		{
			deal_f32_t *dat_mmm_r = dat_r + mmm + n_step_half,
			    *dat_mmm_i = dat_i + mmm + n_step_half;
			DEAL_4_HALFWAY_RI(dat_mmm, n_step, AS_IS);
		}
	}

	index_delta = ROTS_LEN >> (n_ord_lay + 1);
	index_rot_angle = index_delta;
	for (iii = 1; iii < n_step_half; iii++) {
		deal_f32_t rot_half_r = mlib_rots[1 * index_rot_angle][0];
		deal_f32_t rot_half_i = mlib_rots[1 * index_rot_angle][1];
		deal_f32_t rot_r = mlib_rots[2 * index_rot_angle][0];
		deal_f32_t rot_i = mlib_rots[2 * index_rot_angle][1];
		deal_f32_t r_rh_r = mlib_rots[3 * index_rot_angle][0];
		deal_f32_t r_rh_i = mlib_rots[3 * index_rot_angle][1];

		for (mmm = iii; mmm < n_len_arr; mmm += four_n_step) {
			deal_f32_t *dat_mmm_r = dat_r + mmm,
			    *dat_mmm_i = dat_i + mmm;
			deal_f32_t *dat_mirr_r = dat_r + n_mirr_shift - mmm,
			    *dat_mirr_i = dat_i + n_mirr_shift - mmm;
			DEAL_8_USUAL_MIRR_RI(dat_mmm, dat_mirr,
			    n_step, rot, rot_half, r_rh, AS_IS);
		}

		index_rot_angle += index_delta;
	}	/* end for( ; iii < n_step_half; ) */
}

#endif /* ( IMDCT_ORDER > 5 ) || ( IMDCT_SPLIT_ORDER > 5 ) */

/* *********************************************************** */

#if (IMDCT_ORDER > 4) || (IMDCT_SPLIT_ORDER > 4)
static void
mlib_IMDCT_F32_2LayersLast_RI(
    deal_f32_t *CONST dat_r,
    deal_f32_t *CONST dat_i,
    mlib_s32 CONST n_ord_arr)
{
	CONST mlib_s32
	    n_ord_lay = n_ord_arr - 2,
	    n_step = 1 << n_ord_lay, n_step_half = n_step >> 1;
	mlib_s32 iii;
	int index_delta, index_rot_angle;

	{
		deal_f32_t *dat_mmm_r = dat_r, *dat_mmm_i = dat_i;

		DEAL_4_SIMPLE_RI(dat_mmm, n_step, AS_IS);
		{
			deal_f32_t *dat_mmm_r = dat_r + n_step_half,
			    *dat_mmm_i = dat_i + n_step_half;
			DEAL_4_HALFWAY_RI(dat_mmm, n_step, AS_IS);
		}

		index_delta = ROTS_LEN >> (n_ord_lay + 1);
		index_rot_angle = index_delta;
		for (iii = 1; iii < n_step_half; iii++) {
			deal_f32_t rot_half_r =
			    mlib_rots[1 * index_rot_angle][0];
			deal_f32_t rot_half_i =
			    mlib_rots[1 * index_rot_angle][1];
			deal_f32_t rot_r = mlib_rots[2 * index_rot_angle][0];
			deal_f32_t rot_i = mlib_rots[2 * index_rot_angle][1];
			deal_f32_t r_rh_r = mlib_rots[3 * index_rot_angle][0];
			deal_f32_t r_rh_i = mlib_rots[3 * index_rot_angle][1];

			{
				deal_f32_t *dat_mmm_r = dat_r + iii,
				    *dat_mmm_i = dat_i + iii;
				deal_f32_t *dat_mirr_r = dat_r + n_step - iii,
				    *dat_mirr_i = dat_i + n_step - iii;
				DEAL_8_USUAL_MIRR_RI(dat_mmm, dat_mirr,
				    n_step, rot, rot_half, r_rh, AS_IS);
			}

			index_rot_angle += index_delta;
		}
	}
}

#endif /* ( IMDCT_ORDER > 4 ) || ( IMDCT_SPLIT_ORDER > 4 ) */

/* *********************************************************** */

/*  IMDCT function  */
mlib_status
__mlib_SignalIMDCT_D64(
    deal_d64_t *data)
{
	int length = (1 << (IMDCT_ORDER - 1));

	if (!data)
		return (MLIB_FAILURE);
	IMDCT_D64_FFT(data, data + length, data, data + length);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*  IMDCTSplit function  */
mlib_status
__mlib_SignalIMDCTSplit_D64(
    deal_d64_t *data)
{
	int length = 1 << (IMDCT_SPLIT_ORDER - 1);
	deal_d64_t buffer[N512], *dat0 = buffer, *dat2 = dat0 + 2 * length;
	deal_d64_t *d0, *d2;
	int i;

	if (!data)
		return (MLIB_FAILURE);
	d0 = data;
	d2 = d0 + 2 * length;
	for (i = 0; i < length; i++) {
		dat0[i] = d0[2 * i];
		dat0[i + length] = d0[2 * i + 1];
	}

	for (i = 0; i < length; i++) {
		dat2[i + length] = d2[2 * i];
		dat2[i] = d2[2 * i + 1];
	}

	IMDCT_D64_SPLIT_FFT(d0, d2 + length, dat0, dat2 + length);
	IMDCT_D64_SPLIT_FFT(d0 + length, d2, dat0 + length, dat2);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*  IMDCT function  */
mlib_status
__mlib_SignalIMDCT_F32(
    deal_f32_t *data)
{
	int length = (1 << (IMDCT_ORDER - 1));

	if (!data)
		return (MLIB_FAILURE);
	IMDCT_F32_FFT(data, data + length, data, data + length);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*  IMDCTSplit function  */
mlib_status
__mlib_SignalIMDCTSplit_F32(
    deal_f32_t *data)
{
	int length = 1 << (IMDCT_SPLIT_ORDER - 1);
	deal_f32_t buffer[N512], *dat0 = buffer, *dat2 = dat0 + 2 * length;
	deal_f32_t *d0, *d2;
	int i;

	if (!data)
		return (MLIB_FAILURE);
	d0 = data;
	d2 = d0 + 2 * length;
	for (i = 0; i < length; i++) {
		dat0[i] = d0[2 * i];
		dat0[i + length] = d0[2 * i + 1];
	}

	for (i = 0; i < length; i++) {
		dat2[i + length] = d2[2 * i];
		dat2[i] = d2[2 * i + 1];
	}

	IMDCT_F32_SPLIT_FFT(d0, d2 + length, dat0, dat2 + length);
	IMDCT_F32_SPLIT_FFT(d0 + length, d2, dat0 + length, dat2);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
