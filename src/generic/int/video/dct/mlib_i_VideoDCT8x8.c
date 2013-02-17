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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
#pragma ident	"@(#)mlib_i_VideoDCT8x8.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoDCT8x8_S16_U8[_NA]  - computes the forward DCT in
 *                                      the intra mode.
 *      mlib_VideoDCT8x8_S16_S16_B10[_NA] - computes the forward DCT in
 *                                      the inter mode.
 *
 * SYNOPSIS
 *
 *      mlib_status  mlib_VideoDCT8x8_S16_U8[_NA] (mlib_s16      coeffs[64],
 *                                                 const mlib_u8 *block,
 *                                                 mlib_s32      stride)
 *
 *      mlib_status  mlib_VideoDCT8x8_S16_S16_B10[_NA](
 *      					mlib_s16  coeffs[64],
 *                                              const mlib_s16 *block)
 *
 *
 * ARGUMENTS
 *
 *      coeffs     Pointer to output DCT coefficients.
 *      block      Pointer to 8x8 block in current frame or
 *                 motion-compensated reference block.
 *      stride     Stride in bytes between adjacent rows in a block.
 *
 *      It is assumed that block and coeffs are 8-byte aligned and stride is
 *      a multiple of 8 for not *_NA functions.
 *
 *      Input block values should be in [-512, 511] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *
 *             1             7   7              pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             4            x = 0 y = 0             16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT8x8_S16_S16_B10 = __mlib_VideoDCT8x8_S16_S16_B10
#pragma weak mlib_VideoDCT8x8_S16_S16 = __mlib_VideoDCT8x8_S16_S16_B10
#pragma weak __mlib_VideoDCT8x8_S16_S16 = __mlib_VideoDCT8x8_S16_S16_B10
#pragma weak mlib_VideoDCT8x8_S16_S16_B10_NA = __mlib_VideoDCT8x8_S16_S16_B10_NA
#pragma weak mlib_VideoDCT8x8_S16_S16_NA = __mlib_VideoDCT8x8_S16_S16_B10_NA
#pragma weak __mlib_VideoDCT8x8_S16_S16_NA = __mlib_VideoDCT8x8_S16_S16_B10_NA
#pragma weak mlib_VideoDCT8x8_S16_U8 = __mlib_VideoDCT8x8_S16_U8
#pragma weak mlib_VideoDCT8x8_S16_U8_NA = __mlib_VideoDCT8x8_S16_U8_NA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10) mlib_VideoDCT8x8_S16_S16_B10
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10")));
__typeof__(__mlib_VideoDCT8x8_S16_S16) mlib_VideoDCT8x8_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10")));
__typeof__(__mlib_VideoDCT8x8_S16_S16) __mlib_VideoDCT8x8_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10_NA) mlib_VideoDCT8x8_S16_S16_B10_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10_NA")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_NA) mlib_VideoDCT8x8_S16_S16_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10_NA")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_NA) __mlib_VideoDCT8x8_S16_S16_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10_NA")));
__typeof__(__mlib_VideoDCT8x8_S16_U8) mlib_VideoDCT8x8_S16_U8
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_U8")));
__typeof__(__mlib_VideoDCT8x8_S16_U8_NA) mlib_VideoDCT8x8_S16_U8_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_U8_NA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* ********************************************************** */
#define	FCOS_1_4	23170
#define	FSIN_1_4	FCOS_1_4

/* *********************************************************** */

#define	COS_1_4		11585
#define	SIN_1_4	COS_1_4

/* *********************************************************** */

#define	COS_1_8		15137
#define	SIN_1_8		6270

/* *********************************************************** */

#define	COS_1_16	16069
#define	SIN_1_16	3196

/* *********************************************************** */

#define	COS_3_16	13623
#define	SIN_3_16	9102

/* *********************************************************** */

#define	COS_5_16	SIN_3_16
#define	SIN_5_16	COS_3_16

/* *********************************************************** */

#define	COS_7_16	SIN_1_16
#define	SIN_7_16	COS_1_16
/* *********************************************************** */


#define	MLIB_DCT_SIZE	8

#define	ROUND(a) 	((a + (1<<14))>> 15)


#define	CALC(sp, buf)                                             \
	tmp0 = sp[7] + sp[0];                                     \
	tmp1 = sp[6] + sp[1];                                     \
	tmp2 = sp[5] + sp[2];                                     \
	tmp3 = sp[4] + sp[3];                                     \
	tmp4 = sp[3] - sp[4];                                     \
	tmp5 = sp[2] - sp[5];                                     \
	tmp6 = sp[1] - sp[6];                                     \
	tmp7 = sp[0] - sp[7];                                     \
	t0 = tmp3 + tmp0;                                         \
	t1 = tmp2 + tmp1;                                         \
	t2 = tmp1 - tmp2;                                         \
	t3 = tmp0 - tmp3;                                         \
	buf[0] = (ROUND(t0 * SIN_1_4 + t1 * COS_1_4));            \
	buf[MLIB_DCT_SIZE * 2] = (ROUND(t3 * COS_1_8              \
				+ t2 * SIN_1_8));    		  \
	buf[MLIB_DCT_SIZE * 4] = (ROUND(t0 * COS_1_4              \
				- t1 * SIN_1_4)); 		  \
	buf[MLIB_DCT_SIZE * 6] = (ROUND(t3 * SIN_1_8              \
				- t2 * COS_1_8));    		  \
	tmp8 = (ROUND(tmp6 * FCOS_1_4 + tmp5 * FCOS_1_4));        \
	tmp9 = (ROUND(tmp6 * FSIN_1_4 - tmp5 * FSIN_1_4));        \
	t4 = tmp7 + tmp8;                                         \
	t6 = tmp4 + tmp9;                                         \
	t5 = tmp7 - tmp8;                                         \
	t7 = tmp4 - tmp9;                                         \
	buf[MLIB_DCT_SIZE * 1] = (ROUND(t4 * COS_1_16             \
				+ t6 * SIN_1_16)); 		  \
	buf[MLIB_DCT_SIZE * 7] = (ROUND(t4 * COS_7_16             \
				- t6 * SIN_7_16));  		  \
	buf[MLIB_DCT_SIZE * 3] = (ROUND(t5 * COS_3_16             \
				- t7 * SIN_3_16)); 		  \
	buf[MLIB_DCT_SIZE * 5] = (ROUND(t5 * COS_5_16             \
				+ t7 * SIN_5_16))

/* *********************************************************** */

#define	CALC2(sp, buf)                                       	   \
	tmp0 = sp[7] + sp[0];                                      \
	tmp1 = sp[6] + sp[1];                                      \
	tmp2 = sp[5] + sp[2];                                      \
	tmp3 = sp[4] + sp[3];                                      \
	tmp4 = sp[3] - sp[4];                                      \
	tmp5 = sp[2] - sp[5];                                      \
	tmp6 = sp[1] - sp[6];                                      \
	tmp7 = sp[0] - sp[7];                                      \
	t0 = tmp3 + tmp0;                                          \
	t1 = tmp2 + tmp1;                                          \
	t2 = tmp1 - tmp2;                                          \
	t3 = tmp0 - tmp3;                                          \
	buf[0] = (ROUND(t0 * SIN_1_4 + t1 * COS_1_4));             \
	buf[MLIB_DCT_SIZE * 2] = (ROUND(t3 * COS_1_8               \
				+ t2 * SIN_1_8));                  \
	buf[MLIB_DCT_SIZE * 4] = (ROUND(t0 * COS_1_4               \
				- t1 * SIN_1_4));  	           \
	buf[MLIB_DCT_SIZE * 6] = (ROUND(t3 * SIN_1_8               \
				- t2 * COS_1_8));         	   \
	tmp8 = (ROUND(tmp6 * FCOS_1_4 + tmp5 * FCOS_1_4));    	   \
	tmp9 = (ROUND(tmp6 * FSIN_1_4 - tmp5 * FSIN_1_4));         \
	t4 = tmp7 + tmp8;                                          \
	t6 = tmp4 + tmp9;                                          \
	t5 = tmp7 - tmp8;                                          \
	t7 = tmp4 - tmp9;                                          \
	buf[MLIB_DCT_SIZE * 1] = (ROUND(t4 * COS_1_16              \
				+ t6 * SIN_1_16));                 \
	buf[MLIB_DCT_SIZE * 7] = (ROUND(t4 * COS_7_16              \
				- t6 * SIN_7_16));                 \
	buf[MLIB_DCT_SIZE * 3] = (ROUND(t5 * COS_3_16              \
				- t7 * SIN_3_16));                 \
	buf[MLIB_DCT_SIZE * 5] = (ROUND(t5 * COS_5_16              \
				+ t7 * SIN_5_16))
/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_U8(
	mlib_s16 coeffs[64],
	const mlib_u8 *block,
	mlib_s32 stride)
{
	mlib_s32 i;
	mlib_s16 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
	mlib_s16 tmp6, tmp7, tmp8, tmp9;
	mlib_s16 t0, t1, t2, t3, t4, t5, t6, t7;
	mlib_u8 *sp = (void *)block;
	mlib_s16 *dp = coeffs;
	mlib_s16 buffer[64], *buf = buffer;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 8; i++, buf++, sp += stride) {
		CALC(sp, buf);
	}

	buf = buffer;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 8; i++, dp++, buf += MLIB_DCT_SIZE) {
		CALC2(buf, dp);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_U8_NA(
	mlib_s16 coeffs[64],
	const mlib_u8 *block,
	mlib_s32 stride)
{

	if (block == NULL || coeffs == NULL || stride <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VideoDCT8x8_S16_U8(coeffs, block, stride));
}

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_S16_B10(
	mlib_s16 coeffs[64],
	const mlib_s16 *block)
{
	mlib_s32 i;
	mlib_s32 tmp0, tmp1, tmp2, tmp3, tmp4;
	mlib_s32 tmp5, tmp6, tmp7, tmp8, tmp9;
	mlib_s32 t0, t1, t2, t3, t4, t5, t6, t7;
	mlib_s16 *sp = (void *)block, *dp = coeffs;
	mlib_s32 buffer[64], *buf = buffer;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 8; i++, sp += MLIB_DCT_SIZE, buf++) {
		CALC(sp, buf);
	}

	buf = buffer;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 8; i++, dp++, buf += MLIB_DCT_SIZE) {
		CALC2(buf, dp);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_S16_B10_NA(
	mlib_s16 coeffs[64],
	const mlib_s16 *block)
{
	if (block == NULL || coeffs == NULL)
		return (MLIB_FAILURE);

	return (__mlib_VideoDCT8x8_S16_S16_B10(coeffs, block));
}
/* *********************************************************** */
