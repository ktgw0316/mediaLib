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

#ifndef _MLIB_S_IMAGECONSTARITH_FP_H
#define	_MLIB_S_IMAGECONSTARITH_FP_H

#pragma ident	"@(#)mlib_s_ImageConstArith_Fp.h	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageConstFunc_Fp - perform operation on a constant and an image
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageConstFunc_Fp(mlib_image       *dst,
 *	                                   const mlib_image *src,
 *	                                   const mlib_d64   *consts)
 *
 * ARGUMENT
 *	src     pointer to the input image
 *	dst     pointer to the output image
 *	consts  array of the constants for each channel
 *
 * RESTRICTION
 *	src and dst must have the same type, the same size,
 *	and the same number of channels.
 *	The images can have 1, 2, 3, or 4 channels.
 *	The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = Func(consts[c], src[x][y][c])
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

/* Dependency Check */

#ifndef	mlib_ImageConstFunc_Fp
#error	mlib_ImageConstFunc_Fp undefined
#endif

#ifndef	mlib_s_ImageConstFunc_Fp_f32
#error	mlib_s_ImageConstFunc_Fp_f32 undefined
#endif

#ifndef	mlib_s_ImageConstFunc_Fp_d64
#error	mlib_s_ImageConstFunc_Fp_d64 undefined
#endif

#ifndef	MM_INST_F32
#error	MM_INST_F32 undefined
#endif

#ifndef	MM_INST_D64
#error	MM_INST_D64 undefined
#endif

#ifndef	MLIB_C_IMAGECONSTFUNC
#error	MLIB_C_IMAGECONSTFUNC undefined
#endif

/* *********************************************************** */

#define	MLIB_S_IMAGECONSTFUNC_F32(D_STORE, S_LOAD)		\
	{							\
		__m128 dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = MM_INST_F32(consta, srcs);		\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

#define	MLIB_S_IMAGECONSTFUNC3_F32(D_STORE, S_LOAD)		\
	{							\
		__m128 dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = MM_INST_F32(consta, srcs);		\
		D_STORE((void *)&(dst[i]), dsts);		\
		srcs = S_LOAD((void *)&(src[i + 4]));		\
		dsts = MM_INST_F32(constb, srcs);		\
		D_STORE((void *)&(dst[i + 4]), dsts);		\
		srcs = S_LOAD((void *)&(src[i + 8]));		\
		dsts = MM_INST_F32(constc, srcs);		\
		D_STORE((void *)&(dst[i + 8]), dsts);		\
	}

/* *********************************************************** */

#define	MLIB_S_IMAGECONSTFUNC_D64(D_STORE, S_LOAD)		\
	{							\
		__m128d dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = MM_INST_D64(consta, srcs);		\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

#define	MLIB_S_IMAGECONSTFUNC3_D64(D_STORE, S_LOAD)		\
	{							\
		__m128d dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = MM_INST_D64(consta, srcs);		\
		D_STORE((void *)&(dst[i]), dsts);		\
		srcs = S_LOAD((void *)&(src[i + 2]));		\
		dsts = MM_INST_D64(constb, srcs);		\
		D_STORE((void *)&(dst[i + 2]), dsts);		\
		srcs = S_LOAD((void *)&(src[i + 4]));		\
		dsts = MM_INST_D64(constc, srcs);		\
		D_STORE((void *)&(dst[i + 4]), dsts);		\
	}

#define	MLIB_S_IMAGECONSTFUNC4_D64(D_STORE, S_LOAD)		\
	{							\
		__m128d dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = MM_INST_D64(consta, srcs);		\
		D_STORE((void *)&(dst[i]), dsts);		\
		srcs = S_LOAD((void *)&(src[i + 2]));		\
		dsts = MM_INST_D64(constb, srcs);		\
		D_STORE((void *)&(dst[i + 2]), dsts);		\
	}

/* *********************************************************** */

static void
mlib_s_ImageConstFunc_Fp_f32(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *consts)
{
	mlib_s32 i, j, nsize;
	mlib_f32 const0, const1, const2, const3;
	__m128 consta, constb, constc;

	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		const0 = consts[0];
		consta = _mm_set1_ps(const0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_store_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i ++) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_storeu_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i ++) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_storeu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i ++) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		const0 = consts[0];
		const1 = consts[1];
		consta = _mm_set_ps(const1, const0, const1, const0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_store_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 2) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_storeu_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 2) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_storeu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 2) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		const0 = consts[0];
		const1 = consts[1];
		const2 = consts[2];
		consta = _mm_set_ps(const0, const2, const1, const0);
		constb = _mm_set_ps(const1, const0, const2, const1);
		constc = _mm_set_ps(const2, const1, const0, const2);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGECONSTFUNC3_F32(
						_mm_store_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 3) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGECONSTFUNC3_F32(
						_mm_storeu_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 3) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGECONSTFUNC3_F32(
						_mm_storeu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 3) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		const0 = consts[0];
		const1 = consts[1];
		const2 = consts[2];
		const3 = consts[3];
		consta = _mm_set_ps(const3, const2, const1, const0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_store_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 4) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
					dst[i + 3] = MLIB_C_IMAGECONSTFUNC(
							const3, src[i + 3]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_storeu_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 4) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
					dst[i + 3] = MLIB_C_IMAGECONSTFUNC(
							const3, src[i + 3]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC_F32(
						_mm_storeu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 4) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
					dst[i + 3] = MLIB_C_IMAGECONSTFUNC(
							const3, src[i + 3]);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */

static void
mlib_s_ImageConstFunc_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *consts)
{
	mlib_s32 i, j, nsize;
	mlib_d64 const0, const1, const2, const3;
	__m128d consta, constb, constc;

	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		const0 = consts[0];
		consta = _mm_set1_pd(const0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGECONSTFUNC_D64(
						_mm_store_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i ++) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGECONSTFUNC_D64(
						_mm_storeu_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i ++) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGECONSTFUNC_D64(
						_mm_storeu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i ++) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		const0 = consts[0];
		const1 = consts[1];
		consta = _mm_set_pd(const1, const0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGECONSTFUNC_D64(
						_mm_store_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 2) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGECONSTFUNC_D64(
						_mm_storeu_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 2) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGECONSTFUNC_D64(
						_mm_storeu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 2) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		const0 = consts[0];
		const1 = consts[1];
		const2 = consts[2];
		consta = _mm_set_pd(const1, const0);
		constb = _mm_set_pd(const0, const2);
		constc = _mm_set_pd(const2, const1);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_IMAGECONSTFUNC3_D64(
						_mm_store_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 3) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_IMAGECONSTFUNC3_D64(
						_mm_storeu_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 3) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_IMAGECONSTFUNC3_D64(
						_mm_storeu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 3) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		const0 = consts[0];
		const1 = consts[1];
		const2 = consts[2];
		const3 = consts[3];
		consta = _mm_set_pd(const1, const0);
		constb = _mm_set_pd(const3, const2);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC4_D64(
						_mm_store_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 4) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
					dst[i + 3] = MLIB_C_IMAGECONSTFUNC(
							const3, src[i + 3]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC4_D64(
						_mm_storeu_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 4) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
					dst[i + 3] = MLIB_C_IMAGECONSTFUNC(
							const3, src[i + 3]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGECONSTFUNC4_D64(
						_mm_storeu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 4) {
					dst[i] = MLIB_C_IMAGECONSTFUNC(
							const0, src[i]);
					dst[i + 1] = MLIB_C_IMAGECONSTFUNC(
							const1, src[i + 1]);
					dst[i + 2] = MLIB_C_IMAGECONSTFUNC(
							const2, src[i + 2]);
					dst[i + 3] = MLIB_C_IMAGECONSTFUNC(
							const3, src[i + 3]);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageConstFunc_Fp(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64   *consts)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	void *sa, *da;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_FLOAT) {
		mlib_s_ImageConstFunc_Fp_f32(da, dlb, sa, slb,
				xsize, ysize, nchan, consts);
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_DOUBLE) {
		mlib_s_ImageConstFunc_Fp_d64(da, dlb, sa, slb,
				xsize, ysize, nchan, consts);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECONSTARITH_FP_H */
