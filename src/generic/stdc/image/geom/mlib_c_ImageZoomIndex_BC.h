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

#ifndef _MLIB_C_IMAGEZOOMINDEX_BC_H
#define	_MLIB_C_IMAGEZOOMINDEX_BC_H

#pragma ident	"@(#)mlib_c_ImageZoomIndex_BC.h	9.2	07/10/09 SMI"

#endif /* _MLIB_C_IMAGEZOOMINDEX_BC_H */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_C_BICUBIC_U8_U8_3_)

/* *********************************************************** */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_U8_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic
#define	CHL	3

#elif defined(_C_BICUBIC_U8_U8_4_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_U8_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic
#define	CHL	4

#elif defined(_C_BICUBIC_S16_S16_3_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_S16_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic
#define	CHL	3

#elif defined(_C_BICUBIC_S16_S16_4_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_S16_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic
#define	CHL	4

#elif defined(_C_BICUBIC_U8_S16_3_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_U8_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic
#define	CHL	3

#elif defined(_C_BICUBIC_U8_S16_4_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_U8_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic
#define	CHL	4

#elif defined(_C_BICUBIC_S16_U8_3_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_S16_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic
#define	CHL	3

#elif defined(_C_BICUBIC_S16_U8_4_)	/* defined(_C_BICUBIC_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_S16_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic
#define	CHL	4

#endif		   /* defined(_C_BICUBIC_U8_U8_3_) */

#if defined(_C_BICUBIC2_U8_U8_3_)

#define	FUNC4	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_U8_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_U8_3_Bicubic2
#define	CHL	3

#elif defined(_C_BICUBIC2_U8_U8_4_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_U8_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_U8_4_Bicubic2
#define	CHL	4

#elif defined(_C_BICUBIC2_S16_S16_3_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_S16_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_S16_3_Bicubic2
#define	CHL	3

#elif defined(_C_BICUBIC2_S16_S16_4_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_S16_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_S16_4_Bicubic2
#define	CHL	4

#elif defined(_C_BICUBIC2_U8_S16_3_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_U8_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_S16_3_Bicubic2
#define	CHL	3

#elif defined(_C_BICUBIC2_U8_S16_4_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_U8_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_S16_4_Bicubic2
#define	CHL	4

#elif defined(_C_BICUBIC2_S16_U8_3_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_S16_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_U8_3_Bicubic2
#define	CHL	3

#elif defined(_C_BICUBIC2_S16_U8_4_)	/* defined(_C_BICUBIC2_U8_U8_3_) */

#define	FUNC4	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic2_4_Linear
#define	FUNC3	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic2_3_Linear
#define	FUNC2	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic2_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic2_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic2_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_S16_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_U8_4_Bicubic2
#define	CHL	4

#endif		   /* defined(_C_BICUBIC2_U8_U8_3_) */

#if defined(_IDX_U8_)
#define	IDX_TYPE	mlib_u8
#else		   /* defined(_IDX_U8_) */
#define	IDX_TYPE	mlib_s16
#endif		   /* defined(_IDX_U8_) */

#if defined(_PAL_U8_)

#define	PAL_TYPE	mlib_u8

#define	MLIB_XY_SHIFT	4
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)

/* *********************************************************** */

#ifndef USE_BICUBIC2
#define	FILTER_TABLE	mlib_filters_u8f_bc
#else		   /* USE_BICUBIC2 */
#define	FILTER_TABLE	mlib_filters_u8f_bc2
#endif		   /* USE_BICUBIC2 */

/* *********************************************************** */

#define	SAT_U8	((mlib_f32)0x7F800000)
#define	SAT_U8_SH	24
#define	SAT_U8_XOR	0x80

#ifdef MLIB_USE_FTOI_CLAMPING

#define	COMPUTE_RES                                             \
	ires = res - SAT_U8;                                    \
	ires = (ires >> SAT_U8_SH) ^ SAT_U8_XOR

#else		   /* MLIB_USE_FTOI_CLAMPING */

#define	COMPUTE_RES                                                 \
	res -= SAT_U8;                                              \
	if (res >= MLIB_S32_MAX)                                    \
	    ires = MLIB_U8_MAX;                                     \
	else {                                                      \
	    if (res <= MLIB_S32_MIN)                                \
		ires = MLIB_U8_MIN;                                 \
	    else                                                    \
		ires = ((mlib_s32)res >> SAT_U8_SH) ^ SAT_U8_XOR;   \
	}

#endif		   /* MLIB_USE_FTOI_CLAMPING */

#else		   /* defined(_PAL_U8_) */

#define	PAL_TYPE	mlib_s16

#define	MLIB_XY_SHIFT	3
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)

/* *********************************************************** */

#ifndef USE_BICUBIC2
#define	FILTER_TABLE	mlib_filters_s16f_bc
#else		   /* USE_BICUBIC2 */
#define	FILTER_TABLE	mlib_filters_s16f_bc2
#endif		   /* USE_BICUBIC2 */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	COMPUTE_RES	ires = (mlib_s32)res >> MLIB_SHIFT

#else		   /* MLIB_USE_FTOI_CLAMPING */

#define	COMPUTE_RES                                             \
	if (res >= MLIB_S32_MAX)                                \
	    ires = MLIB_S16_MAX;                                \
	else {                                                  \
	    if (res <= MLIB_S32_MIN)                            \
		ires = MLIB_S16_MIN;                            \
	    else                                                \
		ires = (mlib_s32)res >> MLIB_SHIFT;             \
	}

#endif		   /* MLIB_USE_FTOI_CLAMPING */

#endif		   /* defined(_PAL_U8_) */

#define	COEF(X)                                                 \
	pos = ((X) >> MLIB_XY_SHIFT) & MLIB_XY_MASK;            \
	fptr = (mlib_f32 *)((mlib_u8 *)FILTER_TABLE + pos);     \
	X##f0 = fptr[0];                                        \
	X##f1 = fptr[1];                                        \
	X##f2 = fptr[2];                                        \
	X##f3 = fptr[3]

#define	CALC_FUNC0(N)                                                 \
	res =                                                         \
	    buf0[CHL * i + (N)] * yf0 + buf1[CHL * i + (N)] * yf1 +   \
	    buf2[CHL * i + (N)] * yf2 + buf3[CHL * i + (N)] * yf3;    \
	COMPUTE_RES;                                                  \
	tdp[CHL * i + (N)] = ires

#define	CALC_FUNC1(N)                                                 \
	res =                                                         \
	    buf0[CHL * i + (N)] * yf0 + buf1[CHL * i + (N)] * yf1 +   \
	    buf2[CHL * i + (N)] * yf2;                                \
	k0 = *(colortable + tsp4[cx + 0] * CHL + (N)) * xf0 +         \
	    *(colortable + tsp4[cx + 1] * CHL + (N)) * xf1 +          \
	    *(colortable + tsp4[cx + 2] * CHL + (N)) * xf2 +          \
	    *(colortable + tsp4[cx + 3] * CHL + (N)) * xf3;           \
	buf3[CHL * i + (N)] = k0;                                     \
	res += k0 * yf3;                                              \
	COMPUTE_RES;                                                  \
	tdp[CHL * i + (N)] = ires

#define	CALC_FUNC2(N)                                                  \
	res = buf0[CHL * i + (N)] * yf0 + buf1[CHL * i + (N)] * yf1;   \
	k0 = *(colortable + tsp3[cx + 0] * CHL + (N)) * xf0 +          \
	    *(colortable + tsp3[cx + 1] * CHL + (N)) * xf1 +           \
	    *(colortable + tsp3[cx + 2] * CHL + (N)) * xf2 +           \
	    *(colortable + tsp3[cx + 3] * CHL + (N)) * xf3;            \
	buf2[CHL * i + (N)] = k0;                                      \
	res += k0 * yf2;                                               \
	k0 = *(colortable + tsp4[cx + 0] * CHL + (N)) * xf0 +          \
	    *(colortable + tsp4[cx + 1] * CHL + (N)) * xf1 +           \
	    *(colortable + tsp4[cx + 2] * CHL + (N)) * xf2 +           \
	    *(colortable + tsp4[cx + 3] * CHL + (N)) * xf3;            \
	buf3[CHL * i + (N)] = k0;                                      \
	res += k0 * yf3;                                               \
	COMPUTE_RES;                                                   \
	tdp[CHL * i + (N)] = ires

#define	CALC_FUNC3(N)                                           \
	res = buf0[CHL * i + (N)] * yf0;                        \
	k0 = *(colortable + tsp2[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp2[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp2[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp2[cx + 3] * CHL + (N)) * xf3;     \
	buf1[CHL * i + (N)] = k0;                               \
	res += k0 * yf1;                                        \
	k0 = *(colortable + tsp3[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp3[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp3[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp3[cx + 3] * CHL + (N)) * xf3;     \
	buf2[CHL * i + (N)] = k0;                               \
	res += k0 * yf2;                                        \
	k0 = *(colortable + tsp4[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp4[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp4[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp4[cx + 3] * CHL + (N)) * xf3;     \
	buf3[CHL * i + (N)] = k0;                               \
	res += k0 * yf3;                                        \
	COMPUTE_RES;                                            \
	tdp[CHL * i + (N)] = ires

#define	CALC_FUNC4(N)                                           \
	k0 = *(colortable + tsp1[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp1[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp1[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp1[cx + 3] * CHL + (N)) * xf3;     \
	buf0[CHL * i + (N)] = k0;                               \
	res = k0 * yf0;                                         \
	k0 = *(colortable + tsp2[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp2[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp2[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp2[cx + 3] * CHL + (N)) * xf3;     \
	buf1[CHL * i + (N)] = k0;                               \
	res += k0 * yf1;                                        \
	k0 = *(colortable + tsp3[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp3[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp3[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp3[cx + 3] * CHL + (N)) * xf3;     \
	buf2[CHL * i + (N)] = k0;                               \
	res += k0 * yf2;                                        \
	k0 = *(colortable + tsp4[cx + 0] * CHL + (N)) * xf0 +   \
	    *(colortable + tsp4[cx + 1] * CHL + (N)) * xf1 +    \
	    *(colortable + tsp4[cx + 2] * CHL + (N)) * xf2 +    \
	    *(colortable + tsp4[cx + 3] * CHL + (N)) * xf3;     \
	buf3[CHL * i + (N)] = k0;                               \
	res += k0 * yf3;                                        \
	COMPUTE_RES;                                            \
	tdp[CHL * i + (N)] = ires

/* *********************************************************** */

static void
FUNC4(
    mlib_s32 width,
    mlib_s32 y,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 src_stride,
    IDX_TYPE *tsp,
    PAL_TYPE *tdp,
    mlib_d64 *colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3)
{
	IDX_TYPE *tsp1, *tsp2, *tsp3, *tsp4;
	mlib_d64 k0, res, yf0, yf1, yf2, yf3, xf0, xf1, xf2, xf3;
	mlib_f32 *fptr;
	mlib_s32 i, cx, pos, ires;

	COEF(y);
	tsp1 = tsp;
	tsp2 = tsp1 + src_stride;
	tsp3 = tsp2 + src_stride;
	tsp4 = tsp3 + src_stride;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		COEF(x);
		cx = x >> MLIB_SHIFT;
		CALC_FUNC4(0);
		CALC_FUNC4(1);
		CALC_FUNC4(2);
#if (CHL == 4)
		CALC_FUNC4(3);
#endif /* (CHL == 4) */
		x += dx;
	}
}

/* *********************************************************** */

static void
FUNC3(
    mlib_s32 width,
    mlib_s32 y,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 src_stride,
    IDX_TYPE *tsp,
    PAL_TYPE *tdp,
    mlib_d64 *colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3)
{
	IDX_TYPE *tsp1, *tsp2, *tsp3, *tsp4;
	mlib_d64 k0, res, yf0, yf1, yf2, yf3, xf0, xf1, xf2, xf3;
	mlib_f32 *fptr;
	mlib_s32 i, cx, pos, ires;

	COEF(y);
	tsp1 = tsp;
	tsp2 = tsp1 + src_stride;
	tsp3 = tsp2 + src_stride;
	tsp4 = tsp3 + src_stride;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		COEF(x);
		cx = x >> MLIB_SHIFT;
		CALC_FUNC3(0);
		CALC_FUNC3(1);
		CALC_FUNC3(2);
#if (CHL == 4)
		CALC_FUNC3(3);
#endif /* (CHL == 4) */
		x += dx;
	}
}

/* *********************************************************** */

static void
FUNC2(
    mlib_s32 width,
    mlib_s32 y,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 src_stride,
    IDX_TYPE *tsp,
    PAL_TYPE *tdp,
    mlib_d64 *colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3)
{
	IDX_TYPE *tsp1, *tsp2, *tsp3, *tsp4;
	mlib_d64 k0, res, yf0, yf1, yf2, yf3, xf0, xf1, xf2, xf3;
	mlib_f32 *fptr;
	mlib_s32 i, cx, pos, ires;

	COEF(y);
	tsp1 = tsp;
	tsp2 = tsp1 + src_stride;
	tsp3 = tsp2 + src_stride;
	tsp4 = tsp3 + src_stride;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		COEF(x);
		cx = x >> MLIB_SHIFT;
		CALC_FUNC2(0);
		CALC_FUNC2(1);
		CALC_FUNC2(2);
#if (CHL == 4)
		CALC_FUNC2(3);
#endif /* (CHL == 4) */
		x += dx;
	}
}

/* *********************************************************** */

static void
FUNC1(
    mlib_s32 width,
    mlib_s32 y,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 src_stride,
    IDX_TYPE *tsp,
    PAL_TYPE *tdp,
    mlib_d64 *colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3)
{
	IDX_TYPE *tsp1, *tsp2, *tsp3, *tsp4;
	mlib_d64 k0, res, yf0, yf1, yf2, yf3, xf0, xf1, xf2, xf3;
	mlib_f32 *fptr;
	mlib_s32 i, cx, pos, ires;

	COEF(y);
	tsp1 = tsp;
	tsp2 = tsp1 + src_stride;
	tsp3 = tsp2 + src_stride;
	tsp4 = tsp3 + src_stride;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		COEF(x);
		cx = x >> MLIB_SHIFT;
		CALC_FUNC1(0);
		CALC_FUNC1(1);
		CALC_FUNC1(2);
#if (CHL == 4)
		CALC_FUNC1(3);
#endif /* (CHL == 4) */
		x += dx;
	}
}

/* *********************************************************** */

static void
FUNC0(
    mlib_s32 width,
    mlib_s32 y,
    PAL_TYPE *tdp,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3)
{
	mlib_d64 res, yf0, yf1, yf2, yf3;
	mlib_f32 *fptr;
	mlib_s32 i, pos, ires;

	COEF(y);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		CALC_FUNC0(0);
		CALC_FUNC0(1);
		CALC_FUNC0(2);
#if (CHL == 4)
		CALC_FUNC0(3);
#endif /* (CHL == 4) */
	}
}

/* *********************************************************** */

mlib_status
FUNC_MAIN(
    mlib_work_image *param)
{
	void *colormap = GetElemStruct(colormap);
	PAL_TYPE *dpbuf = GetElemStruct(buffer_dp);
	IDX_TYPE *tsp,
	    *dp = GetElemSubStruct(current, dp),
	    *sp = GetElemSubStruct(current, sp);
	mlib_s32 j, cx, next_step,
	    dx = GetElemStruct(DX),
	    dy = GetElemStruct(DY),
	    x = GetElemSubStruct(current, srcX) & MLIB_MASK,
	    y = GetElemSubStruct(current, srcY) & MLIB_MASK,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 *colortable =
	    mlib_ImageGetLutDoubleData(colormap) -
	    mlib_ImageGetLutOffset(colormap) * CHL, *buf0, *buf1, *buf2, *buf3,
	    buffer[1024 * CHL * 4], *buff, *bufT;

	if (width > 1024) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * CHL * 4);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;

	buf0 = buff;
	buf1 = buf0 + width * CHL;
	buf2 = buf1 + width * CHL;
	buf3 = buf2 + width * CHL;

	sp -= src_stride + 1;
	tsp = sp;

	next_step = 4;
	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		cx = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
			FUNC0(width, y, dpbuf, buf0, buf1, buf2, buf3);
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = buf2;
				buf2 = buf3;
				buf3 = bufT;
				FUNC1(width, y, x, dx, src_stride,
				    tsp, dpbuf, colortable, buf0, buf1, buf2,
				    buf3);
			} else {
				if (cx == 2) {
					bufT = buf2;
					buf2 = buf0;
					buf0 = bufT;
					bufT = buf3;
					buf3 = buf1;
					buf1 = bufT;
					FUNC2(width, y, x, dx, src_stride,
					    tsp, dpbuf, colortable, buf0, buf1,
					    buf2, buf3);
				} else {
					if (cx == 3) {
						bufT = buf3;
						buf3 = buf0;
						buf0 = bufT;
						FUNC3(width, y, x, dx,
						    src_stride, tsp, dpbuf,
						    colortable, buf0, buf1,
						    buf2, buf3);
					} else {
						FUNC4(width, y, x, dx,
						    src_stride, tsp, dpbuf,
						    colortable, buf0, buf1,
						    buf2, buf3);
					}
				}
			}
		}

		FUNC_IDX(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

#undef FUNC0
#undef FUNC1
#undef FUNC2
#undef FUNC3
#undef FUNC4
#undef FUNC_IDX
#undef FUNC_MAIN
#undef CHL
#undef CALC_FUNC0
#undef CALC_FUNC1
#undef CALC_FUNC2
#undef CALC_FUNC3
#undef CALC_FUNC4
#undef IDX_TYPE
#undef PAL_TYPE
#undef COEF
#undef COMPUTE_RES
#undef FILTER_TABLE
#undef MLIB_XY_MASK
#undef MLIB_XY_SHIFT
#undef SAT_U8
#undef SAT_U8_SH
#undef SAT_U8_XOR

#ifdef __cplusplus
}
#endif
