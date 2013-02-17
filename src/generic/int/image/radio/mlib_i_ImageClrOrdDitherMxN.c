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

#pragma ident	"@(#)mlib_i_ImageClrOrdDitherMxN.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorOrderedDitherMxN - convert a true-color/grayscale image
 *                                        to an indexed-color/black-white image
 *                                        by ordered dithering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorOrderedDitherMxN(mlib_image       *dst,
 *                                                  const mlib_image *src,
 *                                                  const mlib_s32   **dmask,
 *                                                  mlib_s32         m,
 *                                                  mlib_s32         n,
 *                                                  mlib_s32         scale,
 *                                                  const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      dmask     Pointer to the dither masks, one per channel, in row major
 *                order.
 *      m         Mask width (m > 1).
 *      n         Mask height (n > 1).
 *      scale     The scaling factor for dmask to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for image dithering.
 *
 * DESCRIPTION
 *      Convert a 3 or 4-channel image to a 1-channel indexed image, or convert
 *      a 1-channel grayscale image to a 1-channel MLIB_BIT image, with the
 *      method of ordered dithering.
 *
 * RESTRICTIONS
 *      If src is a 1-channel MLIB_BYTE image then
 *      dst must be a 1-channel MLIB_BIT image.
 *      In other cases src must be an MLIB_BYTE or MLIB_SHORT image
 *      with 3 or 4 channels and dst must be a 1-channel MLIB_BYTE or
 *      MLIB_SHORT image.
 *
 *      The colormap must be created by
 *      mlib_ImageColorTrue2IndexInit with colorcube
 *      included (bits != 0) or by
 *      mlib_ImageColorDitherInit() with dimensions != NULL.
 *
 *      0 <= dmask[i][j] < 2^scale;
 *      scale > 0
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageColorOrderedDitherMxN.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorOrderedDitherMxN = \
	__mlib_ImageColorOrderedDitherMxN

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorOrderedDitherMxN) mlib_ImageColorOrderedDitherMxN
    __attribute__((weak, alias("__mlib_ImageColorOrderedDitherMxN")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageColorOrderedDither##SUFF

/* *********************************************************** */

#define	FUNC_M_ARG                                               \
	void *_dp, void *_sp, mlib_s32 *pkern, mlib_s32 mstep,   \
	    mlib_s32 sw, mlib_s32 tail

/* *********************************************************** */

#define	DEF_VAR_u8                                              \
	mlib_u8 *dp = _dp;                                      \
	mlib_u8 *sp = _sp;                                      \
	mlib_s32 mask1 = 0x000000FF;                            \
	mlib_s32 mask2 = 0x0000FF00;                            \
	mlib_s32 i

/* *********************************************************** */

static const mlib_u32 mask32[] =
{ 0x0000FFFF, 0x7FFF0000, 0x80000000, 0x0000FFFF };

/* *********************************************************** */

#define	DEF_VAR_s16                                             \
	mlib_s16 *dp = _dp;                                     \
	mlib_s16 *sp = _sp;                                     \
	mlib_u32 mask;                                          \
	mlib_u8 *pmask = (mlib_u8 *)mask32;                     \
	mlib_s32 i

/* *********************************************************** */

#define	SAT_u8(out, src)                                         \
	{                                                        \
	    mlib_s32 sum = src;                                  \
	    mlib_s32 imask = (sum >> 5) & 0x18;                  \
	                                                         \
	    out = (sum & (mask1 >> imask)) | (mask2 >> imask);   \
	}

/* *********************************************************** */

#define	SAT_s16(out, src)                                            \
	{                                                            \
	    mlib_s32 sum = src;                                      \
	                                                             \
	    mask = *((mlib_u32 *)(pmask + (((sum) >> 13) & 0xC)));   \
	    out = (mlib_s16)(((sum) & mask) | (mask >> 16));         \
	}

/* *********************************************************** */

#define	DEF_FUNC(TT)                                             \
	void FUNC(TT##_ch3_m1) (FUNC_M_ARG)                      \
	{                                                        \
	    DEF_VAR_##TT;                                        \
	    mlib_s32 k0 = pkern[0];                              \
	    mlib_s32 k1 = pkern[1];                              \
	    mlib_s32 k2 = pkern[2];                              \
	                                                         \
	    for (i = 0; i < sw; i++) {                           \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		sp += mstep;                                     \
		dp += mstep;                                     \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(TT##_ch3_m2) (FUNC_M_ARG)                      \
	{                                                        \
	    DEF_VAR_##TT;                                        \
	    mlib_s32 k0 = pkern[0];                              \
	    mlib_s32 k1 = pkern[1];                              \
	    mlib_s32 k2 = pkern[2];                              \
	    mlib_s32 k3 = pkern[3];                              \
	    mlib_s32 k4 = pkern[4];                              \
	    mlib_s32 k5 = pkern[5];                              \
	                                                         \
	    for (i = 0; i < sw; i++) {                           \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
		SAT_##TT(dp[4], sp[4] + k4);                     \
		SAT_##TT(dp[5], sp[5] + k5);                     \
		sp += mstep;                                     \
		dp += mstep;                                     \
	    }                                                    \
	                                                         \
	    if (tail > 0) {                                      \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(TT##_ch3_m3) (FUNC_M_ARG)                      \
	{                                                        \
	    DEF_VAR_##TT;                                        \
	    mlib_s32 k0 = pkern[0];                              \
	    mlib_s32 k1 = pkern[1];                              \
	    mlib_s32 k2 = pkern[2];                              \
	    mlib_s32 k3 = pkern[3];                              \
	    mlib_s32 k4 = pkern[4];                              \
	    mlib_s32 k5 = pkern[5];                              \
	    mlib_s32 k6 = pkern[6];                              \
	    mlib_s32 k7 = pkern[7];                              \
	    mlib_s32 k8 = pkern[8];                              \
	                                                         \
	    for (i = 0; i < sw; i++) {                           \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
		SAT_##TT(dp[4], sp[4] + k4);                     \
		SAT_##TT(dp[5], sp[5] + k5);                     \
		SAT_##TT(dp[6], sp[6] + k6);                     \
		SAT_##TT(dp[7], sp[7] + k7);                     \
		SAT_##TT(dp[8], sp[8] + k8);                     \
		sp += mstep;                                     \
		dp += mstep;                                     \
	    }                                                    \
	                                                         \
	    if (tail > 0) {                                      \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
	                                                         \
		if (tail > 1) {                                  \
		    SAT_##TT(dp[3], sp[3] + k3);                 \
		    SAT_##TT(dp[4], sp[4] + k4);                 \
		    SAT_##TT(dp[5], sp[5] + k5);                 \
		}                                                \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(TT##_ch4_m1) (FUNC_M_ARG)                      \
	{                                                        \
	    DEF_VAR_##TT;                                        \
	    mlib_s32 k0 = pkern[0];                              \
	    mlib_s32 k1 = pkern[1];                              \
	    mlib_s32 k2 = pkern[2];                              \
	    mlib_s32 k3 = pkern[3];                              \
	                                                         \
	    for (i = 0; i < sw; i++) {                           \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
		sp += mstep;                                     \
		dp += mstep;                                     \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(TT##_ch4_m2) (FUNC_M_ARG)                      \
	{                                                        \
	    DEF_VAR_##TT;                                        \
	    mlib_s32 k0 = pkern[0];                              \
	    mlib_s32 k1 = pkern[1];                              \
	    mlib_s32 k2 = pkern[2];                              \
	    mlib_s32 k3 = pkern[3];                              \
	    mlib_s32 k4 = pkern[4];                              \
	    mlib_s32 k5 = pkern[5];                              \
	    mlib_s32 k6 = pkern[6];                              \
	    mlib_s32 k7 = pkern[7];                              \
	                                                         \
	    for (i = 0; i < sw; i++) {                           \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
		SAT_##TT(dp[4], sp[4] + k4);                     \
		SAT_##TT(dp[5], sp[5] + k5);                     \
		SAT_##TT(dp[6], sp[6] + k6);                     \
		SAT_##TT(dp[7], sp[7] + k7);                     \
		sp += mstep;                                     \
		dp += mstep;                                     \
	    }                                                    \
	                                                         \
	    if (tail > 0) {                                      \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(TT##_ch4_m3) (FUNC_M_ARG)                      \
	{                                                        \
	    DEF_VAR_##TT;                                        \
	    mlib_s32 k0 = pkern[0];                              \
	    mlib_s32 k1 = pkern[1];                              \
	    mlib_s32 k2 = pkern[2];                              \
	    mlib_s32 k3 = pkern[3];                              \
	    mlib_s32 k4 = pkern[4];                              \
	    mlib_s32 k5 = pkern[5];                              \
	    mlib_s32 k6 = pkern[6];                              \
	    mlib_s32 k7 = pkern[7];                              \
	    mlib_s32 k8 = pkern[8];                              \
	    mlib_s32 k9 = pkern[9];                              \
	    mlib_s32 kA = pkern[10];                             \
	    mlib_s32 kB = pkern[11];                             \
	                                                         \
	    for (i = 0; i < sw; i++) {                           \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
		SAT_##TT(dp[4], sp[4] + k4);                     \
		SAT_##TT(dp[5], sp[5] + k5);                     \
		SAT_##TT(dp[6], sp[6] + k6);                     \
		SAT_##TT(dp[7], sp[7] + k7);                     \
		SAT_##TT(dp[8], sp[8] + k8);                     \
		SAT_##TT(dp[9], sp[9] + k9);                     \
		SAT_##TT(dp[10], sp[10] + kA);                   \
		SAT_##TT(dp[11], sp[11] + kB);                   \
		sp += mstep;                                     \
		dp += mstep;                                     \
	    }                                                    \
	                                                         \
	    if (tail > 0) {                                      \
		SAT_##TT(dp[0], sp[0] + k0);                     \
		SAT_##TT(dp[1], sp[1] + k1);                     \
		SAT_##TT(dp[2], sp[2] + k2);                     \
		SAT_##TT(dp[3], sp[3] + k3);                     \
	                                                         \
		if (tail > 1) {                                  \
		    SAT_##TT(dp[4], sp[4] + k4);                 \
		    SAT_##TT(dp[5], sp[5] + k5);                 \
		    SAT_##TT(dp[6], sp[6] + k6);                 \
		    SAT_##TT(dp[7], sp[7] + k7);                 \
		}                                                \
	    }                                                    \
	}

/* *********************************************************** */

    DEF_FUNC(u8)
    DEF_FUNC(s16)

/* *********************************************************** */

#define	KW_MAX	3
typedef void (*func_m_type) (FUNC_M_ARG);

static func_m_type func_m_arr[] = {
	NULL,
	FUNC(u8_ch3_m1),
	FUNC(u8_ch3_m2),
	FUNC(u8_ch3_m3),
	FUNC(u8_ch4_m1),
	FUNC(u8_ch4_m2),
	FUNC(u8_ch4_m3),
	FUNC(s16_ch3_m1),
	FUNC(s16_ch3_m2),
	FUNC(s16_ch3_m3),
	FUNC(s16_ch4_m1),
	FUNC(s16_ch4_m2),
	FUNC(s16_ch4_m3)
};

/* *********************************************************** */

typedef void (*line_func_type) (
    void *src,
    void *dst,
    mlib_s32 length,
    const void *state);

static line_func_type line_func_arr[] = {
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_U8_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_U8_4,
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_S16_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_S16_4,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_U8_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_U8_4,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_S16_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_S16_4
};

/* *********************************************************** */
#if defined(_NO_LONGLONG)
/* *********************************************************** */

#define	GET_STEPS                                                        \
	switch (mlib_ImageGetMethod(colormap)) {                         \
	case LUT_COLOR_CUBE_SEARCH:                                      \
	    off = (stype == MLIB_BYTE) ? 8 : 16;                         \
	    step0 = step1 = step2 = step3 =                              \
		1 << (off - mlib_ImageGetBits(colormap));                \
	    break;                                                       \
	case LUT_COLOR_DIMENSIONS:                                       \
	    p_dim = (mlib_s32 *)mlib_ImageGetLutInversTable(colormap);   \
	    off = nchan * (256 / sizeof (mlib_s32));                     \
	    if (dtype == MLIB_SHORT)                                     \
		off *= 2;                                                \
	    if (stype == MLIB_SHORT)                                     \
		off *= 4;                                                \
	    p_dim += off;                                                \
	    srange = (stype == MLIB_BYTE) ? 255 : ((1 << 16) - 1);       \
	    step0 =                                                      \
		(p_dim[0] >                                              \
		1) ? (mlib_s32)(srange / (p_dim[0] - 1)) : 0;            \
	    step1 =                                                      \
		(p_dim[1] >                                              \
		1) ? (mlib_s32)(srange / (p_dim[1] - 1)) : 0;            \
	    step2 =                                                      \
		(p_dim[2] >                                              \
		1) ? (mlib_s32)(srange / (p_dim[2] - 1)) : 0;            \
	    if (nchan == 4) {                                            \
		step3 =                                                  \
		    (p_dim[3] >                                          \
		    1) ? (mlib_s32)(srange / (p_dim[3] - 1)) : 0;        \
	    }                                                            \
	    break;                                                       \
	default:                                                         \
	    return (MLIB_FAILURE);                                       \
	}                                                                \
	                                                                 \
	dscale = 1.0;                                                    \
	while (scale > 30) {                                             \
	    dscale *= 1.0 / (1 << 30);                                   \
	    scale -= 30;                                                 \
	}                                                                \
	                                                                 \
	dscale /= (1 << scale);                                          \
	dscale0 = dscale * step0;                                        \
	dscale1 = dscale * step1;                                        \
	dscale2 = dscale * step2;                                        \
	half_step0 = (step0 - 1) >> 1;                                   \
	half_step1 = (step1 - 1) >> 1;                                   \
	half_step2 = (step2 - 1) >> 1;                                   \
	dmask0 = dmask[0];                                               \
	dmask1 = dmask[1];                                               \
	dmask2 = dmask[2];                                               \
	if (nchan == 4) {                                                \
	    dmask3 = dmask[3];                                           \
	    dscale3 = dscale * step3;                                    \
	    half_step3 = (step3 - 1) >> 1;                               \
	}

/* *********************************************************** */

#define	MAX_KERN	64

/* *********************************************************** */

mlib_status
__mlib_ImageColorOrderedDitherMxN(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 **dmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_type stype, dtype;
	const mlib_s32 *dmask0, *dmask1, *dmask2, *dmask3 = 0;
	mlib_u8 *sl, *dl, *pbuff;
	mlib_s32 nchan, dchan, sll, dll, sw, sh, dw, dh, num_blk, tail;
	mlib_s32 off, off1, kw, mstep, kern_size, i, j, bchan, fun_ind;
	mlib_s32 *p_dim;
	mlib_s32 kern_lcl[MAX_KERN], *kern = kern_lcl, *pkern;
	mlib_s32 step0, step1, step2, step3 = 0;
	mlib_d64 srange, dscale, dscale0, dscale1, dscale2, dscale3 = 0;
	mlib_s32 half_step0, half_step1, half_step2, half_step3 = 0;
	mlib_s32 v0, v1, v2, v3;
	func_m_type *func_m = func_m_arr;
	line_func_type line_func;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, nchan, sw, sh, sll, sl);

	if (stype == MLIB_BYTE && nchan == 1 && dtype == MLIB_BIT) {
		return mlib_ImageColorOrderedDitherBit_MxN(dst, src, dmask, m,
		    n, scale, colormap);
	}

	if (!(stype == MLIB_BYTE || stype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(dtype == MLIB_BYTE || dtype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(nchan >= 3 && nchan <= 4)) {
		return (MLIB_FAILURE);
	}

	if (dmask == NULL || colormap == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if (scale <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	fun_ind = nchan - 3;

	if (dtype == MLIB_SHORT)
		fun_ind += 2;
	if (stype == MLIB_SHORT)
		fun_ind += 4;
	line_func = line_func_arr[fun_ind];

	GET_STEPS;

	if (nchan == 4) {
		func_m += KW_MAX;
	}

	if (stype == MLIB_SHORT) {
		func_m += 2 * KW_MAX;
	}

	kern_size = n * m * nchan;

	if (kern_size > MAX_KERN) {
		kern = __mlib_malloc(kern_size * sizeof (mlib_s32));

		if (kern == NULL)
			return (MLIB_FAILURE);
	}

	if (nchan == 3) {
		for (j = 0; j < n; j++) {
			for (i = 0; i < m; i++) {
				v0 = half_step0 - (mlib_s32)(dmask0[j * m +
				    i] * dscale0);
				v1 = half_step1 - (mlib_s32)(dmask1[j * m +
				    i] * dscale1);
				v2 = half_step2 - (mlib_s32)(dmask2[j * m +
				    i] * dscale2);
				kern[3 * (j * m + i)] = v0;
				kern[3 * (j * m + i) + 1] = v1;
				kern[3 * (j * m + i) + 2] = v2;
			}
		}

	} else {
		for (j = 0; j < n; j++) {
			for (i = 0; i < m; i++) {
				v0 = half_step0 - (mlib_s32)(dmask0[j * m +
				    i] * dscale0);
				v1 = half_step1 - (mlib_s32)(dmask1[j * m +
				    i] * dscale1);
				v2 = half_step2 - (mlib_s32)(dmask2[j * m +
				    i] * dscale2);
				v3 = half_step3 - (mlib_s32)(dmask3[j * m +
				    i] * dscale3);
				kern[4 * (j * m + i)] = v0;
				kern[4 * (j * m + i) + 1] = v1;
				kern[4 * (j * m + i) + 2] = v2;
				kern[4 * (j * m + i) + 3] = v3;
			}
		}
	}

	bchan = (stype == MLIB_BYTE) ? nchan : 2 * nchan;
	num_blk = sw / m;
	tail = sw % m;
	pbuff = __mlib_malloc(sw * bchan);

	if (pbuff == NULL) {
		if (kern != kern_lcl)
			__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	mstep = m * nchan;
	pkern = kern;

	for (j = 0; j < sh; j++) {
		for (off = 0; off < m; off += kw) {
			kw = m - off;

			if (kw > KW_MAX)
				kw = KW_MAX;
			off1 = off * bchan;
			if (tail - off >= kw) {
				func_m[kw] (pbuff + off1, sl + off1,
				    pkern + off * nchan, mstep, num_blk + 1, 0);
			} else {
				func_m[kw] (pbuff + off1, sl + off1,
				    pkern + off * nchan, mstep, num_blk, tail -
				    off);
			}
		}

		pkern += mstep;

		if (pkern >= kern + kern_size)
			pkern = kern;

		line_func(pbuff, dl, sw, colormap);

		sl += sll;
		dl += dll;
	}

	__mlib_free(pbuff);

	if (kern != kern_lcl)
		__mlib_free(kern);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else
/* *********************************************************** */

#define	GET_STEPS                                                        \
	switch (mlib_ImageGetMethod(colormap)) {                         \
	case LUT_COLOR_CUBE_SEARCH:                                      \
	    off = (stype == MLIB_BYTE) ? 8 : 16;                         \
	    step0 = step1 = step2 = step3 =                              \
		1 << (off - mlib_ImageGetBits(colormap));                \
	    break;                                                       \
	case LUT_COLOR_DIMENSIONS:                                       \
	    p_dim = (mlib_s32 *)mlib_ImageGetLutInversTable(colormap);   \
	    off = nchan * (256 / sizeof (mlib_s32));                     \
	    if (dtype == MLIB_SHORT)                                     \
		off *= 2;                                                \
	    if (stype == MLIB_SHORT)                                     \
		off *= 4;                                                \
	    p_dim += off;                                                \
	    srange = (stype == MLIB_BYTE) ? 255 : ((1 << 16) - 1);       \
	    step0 =                                                      \
		(p_dim[0] >                                              \
		1) ? (mlib_s32)(srange / (p_dim[0] - 1)) : 0;            \
	    step1 =                                                      \
		(p_dim[1] >                                              \
		1) ? (mlib_s32)(srange / (p_dim[1] - 1)) : 0;            \
	    step2 =                                                      \
		(p_dim[2] >                                              \
		1) ? (mlib_s32)(srange / (p_dim[2] - 1)) : 0;            \
	    if (nchan == 4) {                                            \
		step3 =                                                  \
		    (p_dim[3] >                                          \
		    1) ? (mlib_s32)(srange / (p_dim[3] - 1)) : 0;        \
	    }                                                            \
	    break;                                                       \
	default:                                                         \
	    return (MLIB_FAILURE);                                       \
	}                                                                \
	                                                                 \
	dscale = 1;                                                      \
	while (scale > 30) {                                             \
	    dscale *= (1 << 30);                                         \
	    scale -= 30;                                                 \
	}                                                                \
	                                                                 \
	dscale *= (1 << scale);                                          \
	dscale0 = step0;                                                 \
	dscale1 = step1;                                                 \
	dscale2 = step2;                                                 \
	half_step0 = (step0 - 1) >> 1;                                   \
	half_step1 = (step1 - 1) >> 1;                                   \
	half_step2 = (step2 - 1) >> 1;                                   \
	dmask0 = dmask[0];                                               \
	dmask1 = dmask[1];                                               \
	dmask2 = dmask[2];                                               \
	if (nchan == 4) {                                                \
	    dmask3 = dmask[3];                                           \
	    dscale3 = step3;                                             \
	    half_step3 = (step3 - 1) >> 1;                               \
	}

/* *********************************************************** */

#define	MAX_KERN	64

/* *********************************************************** */

mlib_status
__mlib_ImageColorOrderedDitherMxN(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 **dmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_type stype, dtype;
	const mlib_s32 *dmask0, *dmask1, *dmask2, *dmask3 = 0;
	mlib_u8 *sl, *dl, *pbuff;
	mlib_s32 nchan, dchan, sll, dll, sw, sh, dw, dh, num_blk, tail;
	mlib_s32 off, off1, kw, mstep, kern_size, i, j, bchan, fun_ind;
	mlib_s32 *p_dim;
	mlib_s32 kern_lcl[MAX_KERN], *kern = kern_lcl, *pkern;
	mlib_s32 step0, step1, step2, step3 = 0;
	mlib_s64 srange, dscale, dscale0, dscale1, dscale2, dscale3 = 0;
	mlib_s32 half_step0, half_step1, half_step2, half_step3 = 0;
	mlib_s32 v0, v1, v2, v3;
	func_m_type *func_m = func_m_arr;
	line_func_type line_func;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, nchan, sw, sh, sll, sl);

	if (stype == MLIB_BYTE && nchan == 1 && dtype == MLIB_BIT) {
		return mlib_ImageColorOrderedDitherBit_MxN(dst, src, dmask, m,
		    n, scale, colormap);
	}

	if (!(stype == MLIB_BYTE || stype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(dtype == MLIB_BYTE || dtype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(nchan >= 3 && nchan <= 4)) {
		return (MLIB_FAILURE);
	}

	if (dmask == NULL || colormap == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if (scale <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	fun_ind = nchan - 3;

	if (dtype == MLIB_SHORT)
		fun_ind += 2;
	if (stype == MLIB_SHORT)
		fun_ind += 4;
	line_func = line_func_arr[fun_ind];

	GET_STEPS;

	if (nchan == 4) {
		func_m += KW_MAX;
	}

	if (stype == MLIB_SHORT) {
		func_m += 2 * KW_MAX;
	}

	kern_size = n * m * nchan;

	if (kern_size > MAX_KERN) {
		kern = __mlib_malloc(kern_size * sizeof (mlib_s32));

		if (kern == NULL)
			return (MLIB_FAILURE);
	}

	if (nchan == 3) {
		for (j = 0; j < n; j++) {
			for (i = 0; i < m; i++) {
				v0 = half_step0 - (mlib_s32)((dmask0[j * m +
				    i] * dscale0) / dscale);
				v1 = half_step1 - (mlib_s32)((dmask1[j * m +
				    i] * dscale1) / dscale);
				v2 = half_step2 - (mlib_s32)((dmask2[j * m +
				    i] * dscale2) / dscale);
				kern[3 * (j * m + i)] = v0;
				kern[3 * (j * m + i) + 1] = v1;
				kern[3 * (j * m + i) + 2] = v2;
			}
		}

	} else {
		for (j = 0; j < n; j++) {
			for (i = 0; i < m; i++) {
				v0 = half_step0 - (mlib_s32)((dmask0[j * m +
				    i] * dscale0) / dscale);
				v1 = half_step1 - (mlib_s32)((dmask1[j * m +
				    i] * dscale1) / dscale);
				v2 = half_step2 - (mlib_s32)((dmask2[j * m +
				    i] * dscale2) / dscale);
				v3 = half_step3 - (mlib_s32)((dmask3[j * m +
				    i] * dscale3) / dscale);
				kern[4 * (j * m + i)] = v0;
				kern[4 * (j * m + i) + 1] = v1;
				kern[4 * (j * m + i) + 2] = v2;
				kern[4 * (j * m + i) + 3] = v3;
			}
		}
	}

	bchan = (stype == MLIB_BYTE) ? nchan : 2 * nchan;
	num_blk = sw / m;
	tail = sw % m;
	pbuff = __mlib_malloc(sw * bchan);

	if (pbuff == NULL) {
		if (kern != kern_lcl)
			__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	mstep = m * nchan;
	pkern = kern;

	for (j = 0; j < sh; j++) {
		for (off = 0; off < m; off += kw) {
			kw = m - off;

			if (kw > KW_MAX)
				kw = KW_MAX;
			off1 = off * bchan;
			if (tail - off >= kw) {
				func_m[kw] (pbuff + off1, sl + off1,
				    pkern + off * nchan, mstep, num_blk + 1, 0);
			} else {
				func_m[kw] (pbuff + off1, sl + off1,
				    pkern + off * nchan, mstep, num_blk, tail -
				    off);
			}
		}

		pkern += mstep;

		if (pkern >= kern + kern_size)
			pkern = kern;

		line_func(pbuff, dl, sw, colormap);

		sl += sll;
		dl += dll;
	}

	__mlib_free(pbuff);

	if (kern != kern_lcl)
		__mlib_free(kern);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#endif
