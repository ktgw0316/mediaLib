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

#pragma ident	"@(#)mlib_SignalFFT_D64.c	9.2	07/11/05 SMI"

/* *********************************************************** */

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#ifdef _NO_LONGLONG

/*
 * FUNCTIONS
 *
 */

#include <mlib_signal.h>
#include <mlib_i_SignalFFTUtil.h>

/* *********************************************************** */

const mlib_u8 mlib_fftK_Rev8[256] = {
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

const precise_t mlib_fftK_SinesTable[] = {
/* sin(pi / 2^ 0) */
	0.000000000000000000000000000000e+00,
/* sin(pi / 2^ 1) */
	1.000000000000000000000000000000e+00,
/* sin(pi / 2^ 2) */
	7.071067811865475244008443621048e-01,
/* sin(pi / 2^ 3) */
	3.826834323650897717284599840304e-01,
/* sin(pi / 2^ 4) */
	1.950903220161282678482848684770e-01,
/* sin(pi / 2^ 5) */
	9.801714032956060199419556388864e-02,
/* sin(pi / 2^ 6) */
	4.906767432741801425495497694268e-02,
/* sin(pi / 2^ 7) */
	2.454122852291228803173452945928e-02,
/* sin(pi / 2^ 8) */
	1.227153828571992607940826195100e-02,
/* sin(pi / 2^ 9) */
	6.135884649154475359640234590373e-03,
/* sin(pi / 2^10) */
	3.067956762965976270145365490920e-03,
/* sin(pi / 2^11) */
	1.533980186284765612303697150264e-03,
/* sin(pi / 2^12) */
	7.669903187427045269385683579486e-04,
/* sin(pi / 2^13) */
	3.834951875713955890724616811814e-04,
/* sin(pi / 2^14) */
	1.917475973107033074399095619890e-04,
/* sin(pi / 2^15) */
	9.587379909597734587051721097648e-05,
/* sin(pi / 2^16) */
	4.793689960306688454900399049466e-05,
/* sin(pi / 2^17) */
	2.396844980841821872918657716502e-05,
/* sin(pi / 2^18) */
	1.198422490506970642152156159699e-05,
/* sin(pi / 2^19) */
	5.992112452642427842879711808891e-06,
/* sin(pi / 2^20) */
	2.996056226334660750454812808357e-06,
/* sin(pi / 2^21) */
	1.498028113169011228854278846155e-06,
/* sin(pi / 2^22) */
	7.490140565847157211304985667307e-07,
/* sin(pi / 2^23) */
	3.745070282923841239031691790846e-07,
/* sin(pi / 2^24) */
	1.872535141461953448688245765936e-07,
/* sin(pi / 2^25) */
	9.362675707309808279906728668089e-08,
/* sin(pi / 2^26) */
	4.681337853654909269511551813854e-08,
/* sin(pi / 2^27) */
	2.340668926827455275950549341903e-08,
/* sin(pi / 2^28) */
	1.170334463413727718124621350324e-08,
/* sin(pi / 2^29) */
	5.851672317068638690809790100834e-09,
/* sin(pi / 2^30) */
	2.925836158534319357928230469069e-09,
/* sin(pi / 2^31) */
	1.462918079267159680529532161866e-09,
/* sin(pi / 2^32) */
	7.314590396335798404604431968494e-10,
/* sin(pi / 2^33) */
	3.657295198167899202546812379143e-10,
/* sin(pi / 2^34) */
	1.828647599083949601303980738933e-10
};

/* *********************************************************** */

void
mlib_c_fftKernel1(
    void *data_r,
    void *data_i,
    mlib_s32 order,
    mlib_s32 layer_min,
    mlib_s32 layer_max,
    fft_param * prm,
    mlib_s32 inp_flag)
{
	mlib_s32 layer;

/* recursive processing */
	if (order > CACHE1_ORD) {
		mlib_s32 i, step, ord_blk = order - 3;
		mlib_d64 *pdata_r = data_r;
		mlib_d64 *pdata_i = data_i;

		if (ord_blk < CACHE1_ORD)
			ord_blk = CACHE1_ORD;

		step = (prm->ri_flag) ? (1 << ord_blk) : (2 << ord_blk);

		for (i = 0; i < (1 << (order - ord_blk)); i++) {
			mlib_c_fftKernel1(pdata_r, pdata_i, ord_blk, layer_min,
			    order - 3, prm, inp_flag);
			pdata_r += step;
			pdata_i += step;
		}

		(prm->func_L3) (data_r, data_i, order, order - 3);
		return;
	}

	if (inp_flag) {
		if (layer_min == 2) {
			(prm->func_L2_First) (data_r, data_i, order);
		} else {
			(prm->func_L3_First) (data_r, data_i, order);
		}
	}

	if (order <= 4) {
		if (order == 4 && layer_max == 4) {
			(prm->func_order4) (data_r, data_i, order);
		}

		return;
	}
#if !MORE_3LAYERS
	for (layer = layer_min; layer <= layer_max - 4; layer += 2) {
		(prm->func_L2) (data_r, data_i, order, layer);
	}

#else /* !MORE_3LAYERS */
	for (layer = layer_min; layer <= layer_max - 5; layer += 3) {
		(prm->func_L3) (data_r, data_i, order, layer);
	}

#endif /* !MORE_3LAYERS */

	switch (layer_max - layer) {
	case 2:
		(prm->func_L2) (data_r, data_i, order, layer);
		break;

	case 3:
		(prm->func_L3) (data_r, data_i, order, layer);
		break;

	case 4:
		(prm->func_L2) (data_r, data_i, order, layer);
		(prm->func_L2) (data_r, data_i, order, layer + 2);
		break;
	}
}

/* *********************************************************** */

#define	GET_NUM_FIRST_LAY(num_first, order)                     \
	num_first = order;                                      \
	while (num_first > CACHE1_ORD)                          \
	    num_first -= 3;                                     \
	num_first = 2 + (num_first & 1)

/* *********************************************************** */

void
mlib_c_fftKernel(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_F : mlib_fft_func_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data, NULL, order, layer_min, order, prm, 0);
}

/* *********************************************************** */

void
mlib_c_fftKernelInplace(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_F : mlib_fft_func_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data, NULL, order, layer_min, order, prm, 1);
}

/* *********************************************************** */

void
mlib_c_fftKernel_RI(
    deal_t *data_r,
    deal_t *data_i,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_RI_F : mlib_fft_func_RI_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data_r, data_i, order, layer_min, order, prm, 0);
}

/* *********************************************************** */

void
mlib_c_fftKernelInplace_RI(
    deal_t *data_r,
    deal_t *data_i,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_RI_F : mlib_fft_func_RI_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data_r, data_i, order, layer_min, order, prm, 1);
}

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#else

/*
 * FUNCTIONS
 *
 */

#include <mlib_signal.h>
#include <mlib_i_SignalFFTUtil.h>

/* *********************************************************** */

const mlib_u8 mlib_fftK_Rev8[256] = {
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

const precise_t mlib_fftK_SinesTable[] = {
/* sin(pi / 2^ 0) */
	0.000000000000000000000000000000e+00,
/* sin(pi / 2^ 1) */
	1.000000000000000000000000000000e+00,
/* sin(pi / 2^ 2) */
	7.071067811865475244008443621048e-01,
/* sin(pi / 2^ 3) */
	3.826834323650897717284599840304e-01,
/* sin(pi / 2^ 4) */
	1.950903220161282678482848684770e-01,
/* sin(pi / 2^ 5) */
	9.801714032956060199419556388864e-02,
/* sin(pi / 2^ 6) */
	4.906767432741801425495497694268e-02,
/* sin(pi / 2^ 7) */
	2.454122852291228803173452945928e-02,
/* sin(pi / 2^ 8) */
	1.227153828571992607940826195100e-02,
/* sin(pi / 2^ 9) */
	6.135884649154475359640234590373e-03,
/* sin(pi / 2^10) */
	3.067956762965976270145365490920e-03,
/* sin(pi / 2^11) */
	1.533980186284765612303697150264e-03,
/* sin(pi / 2^12) */
	7.669903187427045269385683579486e-04,
/* sin(pi / 2^13) */
	3.834951875713955890724616811814e-04,
/* sin(pi / 2^14) */
	1.917475973107033074399095619890e-04,
/* sin(pi / 2^15) */
	9.587379909597734587051721097648e-05,
/* sin(pi / 2^16) */
	4.793689960306688454900399049466e-05,
/* sin(pi / 2^17) */
	2.396844980841821872918657716502e-05,
/* sin(pi / 2^18) */
	1.198422490506970642152156159699e-05,
/* sin(pi / 2^19) */
	5.992112452642427842879711808891e-06,
/* sin(pi / 2^20) */
	2.996056226334660750454812808357e-06,
/* sin(pi / 2^21) */
	1.498028113169011228854278846155e-06,
/* sin(pi / 2^22) */
	7.490140565847157211304985667307e-07,
/* sin(pi / 2^23) */
	3.745070282923841239031691790846e-07,
/* sin(pi / 2^24) */
	1.872535141461953448688245765936e-07,
/* sin(pi / 2^25) */
	9.362675707309808279906728668089e-08,
/* sin(pi / 2^26) */
	4.681337853654909269511551813854e-08,
/* sin(pi / 2^27) */
	2.340668926827455275950549341903e-08,
/* sin(pi / 2^28) */
	1.170334463413727718124621350324e-08,
/* sin(pi / 2^29) */
	5.851672317068638690809790100834e-09,
/* sin(pi / 2^30) */
	2.925836158534319357928230469069e-09,
/* sin(pi / 2^31) */
	1.462918079267159680529532161866e-09,
/* sin(pi / 2^32) */
	7.314590396335798404604431968494e-10,
/* sin(pi / 2^33) */
	3.657295198167899202546812379143e-10,
/* sin(pi / 2^34) */
	1.828647599083949601303980738933e-10
};

const mlib_s64 mlib_fftK_SinesTable_i_Q30[] = {
/* sin(pi / 2^ 0) */
	0L,
/* sin(pi / 2^ 1) */
	1073741823L,
/* sin(pi / 2^ 2) */
	759250124L,
/* sin(pi / 2^ 3) */
	410903206L,
/* sin(pi / 2^ 4) */
	209476638L,
/* sin(pi / 2^ 5) */
	105245103L,
/* sin(pi / 2^ 6) */
	52686014L,
/* sin(pi / 2^ 7) */
	26350943L,
/* sin(pi / 2^ 8) */
	13176464L,
/* sin(pi / 2^ 9) */
	6588356L,
/* sin(pi / 2^ 10) */
	3294193L,
/* sin(pi / 2^ 11) */
	1647099L,
/* sin(pi / 2^ 12) */
	823550L,
/* sin(pi / 2^ 13) */
	411775L,
/* sin(pi / 2^ 14) */
	205887L,
/* sin(pi / 2^ 15) */
	102944L,
/* sin(pi / 2^ 16) */
	51472L,
/* sin(pi / 2^ 17) */
	25736L,
/* sin(pi / 2^ 18) */
	12868L,
/* sin(pi / 2^ 19) */
	6434L,
/* sin(pi / 2^ 20) */
	3217L,
/* sin(pi / 2^ 21) */
	1608L,
/* sin(pi / 2^ 22) */
	804L,
/* sin(pi / 2^ 23) */
	402L,
/* sin(pi / 2^ 24) */
	201L,
/* sin(pi / 2^ 25) */
	101L,
/* sin(pi / 2^ 26) */
	50L,
/* sin(pi / 2^ 27) */
	25L,
/* sin(pi / 2^ 28) */
	13L,
/* sin(pi / 2^ 29) */
	6L,
/* sin(pi / 2^ 30) */
	3L,
/* sin(pi / 2^ 31) */
	2L,
/* sin(pi / 2^ 32) */
	1L,
/* sin(pi / 2^ 33) */
	0L,
/* sin(pi / 2^ 34) */
	0L
};

const mlib_s64 mlib_fftK_SinesTable_i_Q15[] = {
/* sin(pi / 2^ 0) */
    0,
/* sin(pi / 2^ 1) */
    32767,
/* sin(pi / 2^ 2) */
    23170,
/* sin(pi / 2^ 3) */
    12539,
/* sin(pi / 2^ 4) */
    6393,
/* sin(pi / 2^ 5) */
    3212,
/* sin(pi / 2^ 6) */
    1608,
/* sin(pi / 2^ 7) */
    804,
/* sin(pi / 2^ 8) */
    402,
/* sin(pi / 2^ 9) */
    201,
/* sin(pi / 2^ 10) */
    101,
/* sin(pi / 2^ 11) */
    50,
/* sin(pi / 2^ 12) */
    25,
/* sin(pi / 2^ 13) */
    13,
/* sin(pi / 2^ 14) */
    6,
/* sin(pi / 2^ 15) */
    3,
/* sin(pi / 2^ 16) */
    2,
/* sin(pi / 2^ 17) */
    1,
/* sin(pi / 2^ 18) */
    0,
/* sin(pi / 2^ 19) */
    0,
/* sin(pi / 2^ 20) */
    0,
/* sin(pi / 2^ 21) */
    0,
/* sin(pi / 2^ 22) */
    0,
/* sin(pi / 2^ 23) */
    0,
/* sin(pi / 2^ 24) */
    0,
/* sin(pi / 2^ 25) */
    0,
/* sin(pi / 2^ 26) */
    0,
/* sin(pi / 2^ 27) */
    0,
/* sin(pi / 2^ 28) */
    0,
/* sin(pi / 2^ 29) */
    0,
/* sin(pi / 2^ 30) */
    0,
/* sin(pi / 2^ 31) */
    0,
/* sin(pi / 2^ 32) */
    0,
/* sin(pi / 2^ 33) */
    0,
/* sin(pi / 2^ 34) */
    0,
};

/* *********************************************************** */

void
mlib_c_fftKernel1(
    void *data_r,
    void *data_i,
    mlib_s32 order,
    mlib_s32 layer_min,
    mlib_s32 layer_max,
    fft_param * prm,
    mlib_s32 inp_flag)
{
	mlib_s32 layer;

/* recursive processing */
	if (order > CACHE1_ORD) {
		mlib_s32 i, step, ord_blk = order - 3;
		mlib_d64 *pdata_r = data_r;
		mlib_d64 *pdata_i = data_i;

		if (ord_blk < CACHE1_ORD)
			ord_blk = CACHE1_ORD;

		step = (prm->ri_flag) ? (1 << ord_blk) : (2 << ord_blk);

		for (i = 0; i < (1 << (order - ord_blk)); i++) {
			mlib_c_fftKernel1(pdata_r, pdata_i, ord_blk, layer_min,
			    order - 3, prm, inp_flag);
			pdata_r += step;
			pdata_i += step;
		}

		(prm->func_L3) (data_r, data_i, order, order - 3);
		return;
	}

	if (inp_flag) {
		if (layer_min == 2) {
			(prm->func_L2_First) (data_r, data_i, order);
		} else {
			(prm->func_L3_First) (data_r, data_i, order);
		}
	}

	if (order <= 4) {
		if (order == 4 && layer_max == 4) {
			(prm->func_order4) (data_r, data_i, order);
		}

		return;
	}
#if !MORE_3LAYERS
	for (layer = layer_min; layer <= layer_max - 4; layer += 2) {
		(prm->func_L2) (data_r, data_i, order, layer);
	}

#else /* !MORE_3LAYERS */
	for (layer = layer_min; layer <= layer_max - 5; layer += 3) {
		(prm->func_L3) (data_r, data_i, order, layer);
	}

#endif /* !MORE_3LAYERS */

	switch (layer_max - layer) {
	case 2:
		(prm->func_L2) (data_r, data_i, order, layer);
		break;

	case 3:
		(prm->func_L3) (data_r, data_i, order, layer);
		break;

	case 4:
		(prm->func_L2) (data_r, data_i, order, layer);
		(prm->func_L2) (data_r, data_i, order, layer + 2);
		break;
	}
}

void
mlib_c_fftKernel1_i(
    void *data_r,
    void *data_i,
    mlib_s32 order,
    mlib_s32 layer_min,
    mlib_s32 layer_max,
    fft_param_i * prm,
    mlib_s32 inp_flag)
{
	mlib_s32 layer;

/* recursive processing */
	if (order > CACHE1_ORD) {
		mlib_s32 i, step, ord_blk = order - 3;
		mlib_s64 *pdata_r = data_r;
		mlib_s64 *pdata_i = data_i;

		if (ord_blk < CACHE1_ORD)
			ord_blk = CACHE1_ORD;

		step = (prm->ri_flag) ? (1 << ord_blk) : (2 << ord_blk);

		for (i = 0; i < (1 << (order - ord_blk)); i++) {
			mlib_c_fftKernel1_i(pdata_r, pdata_i, ord_blk,
				layer_min, order - 3, prm, inp_flag);
			pdata_r += step;
			pdata_i += step;
		}

		(prm->func_L3) (data_r, data_i, order, order - 3);
		return;
	}

	if (inp_flag) {
		if (layer_min == 2) {
			(prm->func_L2_First) (data_r, data_i, order);
		} else {
			(prm->func_L3_First) (data_r, data_i, order);
		}
	}

	if (order <= 4) {
		if (order == 4 && layer_max == 4) {
			(prm->func_order4) (data_r, data_i, order);
		}

		return;
	}
#if !MORE_3LAYERS
	for (layer = layer_min; layer <= layer_max - 4; layer += 2) {
		(prm->func_L2) (data_r, data_i, order, layer);
	}

#else /* !MORE_3LAYERS */
	for (layer = layer_min; layer <= layer_max - 5; layer += 3) {
		(prm->func_L3) (data_r, data_i, order, layer);
	}

#endif /* !MORE_3LAYERS */

	switch (layer_max - layer) {
	case 2:
		(prm->func_L2) (data_r, data_i, order, layer);
		break;

	case 3:
		(prm->func_L3) (data_r, data_i, order, layer);
		break;

	case 4:
		(prm->func_L2) (data_r, data_i, order, layer);
		(prm->func_L2) (data_r, data_i, order, layer + 2);
		break;
	}
}

/* *********************************************************** */

#define	GET_NUM_FIRST_LAY(num_first, order)                     \
	num_first = order;                                      \
	while (num_first > CACHE1_ORD)                          \
	    num_first -= 3;                                     \
	num_first = 2 + (num_first & 1)

/* *********************************************************** */

void
mlib_c_fftKernel(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_F : mlib_fft_func_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data, NULL, order, layer_min, order, prm, 0);
}

/* *********************************************************** */

void
mlib_c_fftKernel_i(
    mlib_s64 *data,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param_i *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_F_i : mlib_fft_func_I_i;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1_i(data, NULL, order, layer_min, order, prm, 0);
}

/* *********************************************************** */

void
mlib_c_fftKernelInplace(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_F : mlib_fft_func_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data, NULL, order, layer_min, order, prm, 1);
}

/* *********************************************************** */

void
mlib_c_fftKernel_RI(
    deal_t *data_r,
    deal_t *data_i,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_RI_F : mlib_fft_func_RI_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data_r, data_i, order, layer_min, order, prm, 0);
}

/* *********************************************************** */

void
mlib_c_fftKernelInplace_RI(
    deal_t *data_r,
    deal_t *data_i,
    mlib_s32 order,
    mlib_s32 isign)
{
	fft_param *prm =
	    (isign == DIRECT_FFT) ? mlib_fft_func_RI_F : mlib_fft_func_RI_I;
	mlib_s32 layer_min;

	GET_NUM_FIRST_LAY(layer_min, order);

	mlib_c_fftKernel1(data_r, data_i, order, layer_min, order, prm, 1);
}

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#endif

/* *********************************************************** */
