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

#ifndef _MLIB_MATRIXMUL_H
#define	_MLIB_MATRIXMUL_H

#pragma ident	"@(#)mlib_MatrixMul.h	9.2	07/10/08 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * FUNCTIONS
 *      Macros for mlib_MatrixMul_* functions
 *      mlib_MatrixMul_S32_S32_Sat
 *      mlib_MatrixMul_S16_S16_Sat
 *      mlib_MatrixMul_S32_S16_Sat
 */

#include <mlib_Utils.h>

/* *********************************************************** */

#define	type_union_mlib_d64	type_union_mlib_d64_a

#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#define	mlib_U8		mlib_u8
#define	mlib_S8		mlib_s8
#define	mlib_S16	mlib_s16
#define	mlib_S32	mlib_s32

/* *********************************************************** */

#ifdef _NO_LONGLONG

#define	MOD_S32_SH                                                   \
	dd.two_int.int1 = ((mlib_u32)dd.two_int.int1) >> exponent;   \
	dd.two_int.int1 |= (dd.two_int.int0) << (32 - exponent)

#else /* _NO_LONGLONG */

#define	MOD_S32_SH	dd.ll >>= exponent

#endif /* _NO_LONGLONG */

/* *********************************************************** */

#define	MOD_S32(res, x)                                               \
	{                                                             \
	    type_union_mlib_d64_a dd;                                 \
	    mlib_s32 exponent;                                        \
	                                                              \
	    dd.db = (x < 0) ? (-x) : (x);                             \
	    if (dd.db <= MLIB_S32_MAX) {                              \
		res = (mlib_s32)x;                                    \
	    } else {                                                  \
		exponent = 0x433 - (dd.two_int.int0 >> 20);           \
		if (exponent > 0) {                                   \
		    dd.two_int.int0 |= 0x100000;                      \
		    MOD_S32_SH;                                       \
		} else {                                              \
		    dd.two_int.int1 <<= -exponent;                    \
		}                                                     \
		res = (x < 0) ? -dd.two_int.int1 : dd.two_int.int1;   \
	    }                                                         \
	}

/* *********************************************************** */

#define	STORE_U8_Sat(DST, SRC)	SATUR_U8(DST, SRC)
#define	STORE_S8_Sat(DST, SRC)	SATUR_S8(DST, SRC)
#define	STORE_S16_Sat(DST, SRC)	SATUR_S16(DST, SRC)
#define	STORE_S32_Sat(DST, SRC)	SAT_S32(DST, SRC)

/* *********************************************************** */

#define	STORE_U8_Mod(DST, SRC)	DST = (mlib_s32)(SRC)
#define	STORE_S8_Mod(DST, SRC)	DST = (mlib_s32)(SRC)
#define	STORE_S16_Mod(DST, SRC)	MOD_S32(DST, SRC)
#define	STORE_S32_Mod(DST, SRC)	MOD_S32(DST, SRC)

/* *********************************************************** */

#define	MATRIX_MUL_FP(DD, SS, MOD, FTYPE)                           \
	mlib_status                                                 \
	__mlib_MatrixMul_##DD##_##SS##_##MOD                        \
		(mlib_##DD * z, const mlib_##SS * x,                \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 l,        \
		mlib_s32 n)                                         \
	{                                                           \
	    mlib_s32 n_l = n * l, size;                             \
	    FTYPE *buff, *py;                                       \
	    mlib_s32 i, j, k;                                       \
	                                                            \
	    if (m <= 0 || l <= 0 || n <= 0)                         \
		return (MLIB_FAILURE);                              \
	    size = n_l + l;                                         \
	    buff = __mlib_malloc(size * sizeof (FTYPE));            \
	    if (buff == NULL) {                                     \
		return mlib_MatrixMul_type(type_##SS, type_##DD,    \
			mode_##MOD, x, y, m, l, n, n, z);           \
	    }                                                       \
	                                                            \
	/*                                                          \
	 * convert y matrix to FTYPE                                \
	 */                                                         \
	    py = buff;                                              \
	    for (i = 0; i < n; i++) {                               \
		for (k = 0; k < l; k++) {                           \
		    py[k] = (FTYPE) y[k * n];                       \
		}                                                   \
		y++;                                                \
		py += l;                                            \
	    }                                                       \
	    for (i = 0; i < m; i++) {                               \
		FTYPE *px = buff + n_l;                             \
	                                                            \
		for (k = 0; k < l; k++) {                           \
		    px[k] = x[k];                                   \
		}                                                   \
		x += l;                                             \
		py = buff;                                          \
		for (j = 0; j <= (n - 8); j += 8) {                 \
		    FTYPE *py0 = py;                                \
		    FTYPE *py1 = py0 + l;                           \
		    FTYPE *py2 = py1 + l;                           \
		    FTYPE *py3 = py2 + l;                           \
		    FTYPE *py4 = py3 + l;                           \
		    FTYPE *py5 = py4 + l;                           \
		    FTYPE *py6 = py5 + l;                           \
		    FTYPE *py7 = py6 + l;                           \
		    FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
		    FTYPE sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;   \
	                                                            \
		    for (k = 0; k < l; k++) {                       \
			FTYPE xx = px[k];                           \
	                                                            \
			sum0 += xx * py0[k];                        \
			sum1 += xx * py1[k];                        \
			sum2 += xx * py2[k];                        \
			sum3 += xx * py3[k];                        \
			sum4 += xx * py4[k];                        \
			sum5 += xx * py5[k];                        \
			sum6 += xx * py6[k];                        \
			sum7 += xx * py7[k];                        \
		    }                                               \
		    STORE_##DD##_##MOD(z[0], sum0);                 \
		    STORE_##DD##_##MOD(z[1], sum1);                 \
		    STORE_##DD##_##MOD(z[2], sum2);                 \
		    STORE_##DD##_##MOD(z[3], sum3);                 \
		    STORE_##DD##_##MOD(z[4], sum4);                 \
		    STORE_##DD##_##MOD(z[5], sum5);                 \
		    STORE_##DD##_##MOD(z[6], sum6);                 \
		    STORE_##DD##_##MOD(z[7], sum7);                 \
		    z += 8;                                         \
		    py += 8 * l;                                    \
		}                                                   \
		for (; j < n; j++) {                                \
		    FTYPE sum0 = 0;                                 \
	                                                            \
		    for (k = 0; k < l; k++) {                       \
			sum0 += px[k] * py[k];                      \
		    }                                               \
		    STORE_##DD##_##MOD(z[0], sum0);                 \
		    z++;                                            \
		    py += l;                                        \
		}                                                   \
	    }                                                       \
	    __mlib_free(buff);                                      \
	    return (MLIB_SUCCESS);                                  \
	}

/* *********************************************************** */

#define	MATRIX_MULC_FP(DD, SS, MOD, FTYPE)                          \
	mlib_status                                                 \
	__mlib_MatrixMul_##DD##C_##SS##C_##MOD                      \
		(mlib_##DD * z, const mlib_##SS * x,                \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 l,        \
		mlib_s32 n)                                         \
	{                                                           \
	    mlib_s32 n_l, size;                                     \
	    FTYPE *buff, *py;                                       \
	    mlib_s32 i, j, k;                                       \
	                                                            \
	    if (m <= 0 || l <= 0 || n <= 0)                         \
		return (MLIB_FAILURE);                              \
	    l *= 2;                                                 \
	/*                                                          \
	 * complex                                                  \
	 */                                                         \
	    n_l = n * l;                                            \
	    size = n_l + l;                                         \
	    buff = __mlib_malloc(size * sizeof (FTYPE));            \
	    if (buff == NULL) {                                     \
		return mlib_MatrixMul_type(type_##SS##C,            \
			type_##DD##C, mode_##MOD, x, y, m,          \
			l, n, n, z);                                \
	    }                                                       \
	                                                            \
	/*                                                          \
	 * convert y matrix to FTYPE                                \
	 */                                                         \
	    py = buff;                                              \
	    for (i = 0; i < n; i++) {                               \
		for (k = 0; k < l; k += 2) {                        \
		    py[k] = (FTYPE) y[k * n];                       \
		    py[k + 1] = (FTYPE) y[k * n + 1];               \
		}                                                   \
		y += 2;                                             \
		py += l;                                            \
	    }                                                       \
	    for (i = 0; i < m; i++) {                               \
		FTYPE *px = buff + n_l;                             \
	                                                            \
		for (k = 0; k < l; k++) {                           \
		    px[k] = x[k];                                   \
		}                                                   \
		x += l;                                             \
		py = buff;                                          \
		for (j = 0; j <= (n - 2); j += 2) {                 \
		    FTYPE *py0 = py;                                \
		    FTYPE *py1 = py0 + l;                           \
		    FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
		    FTYPE sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;   \
	                                                            \
		    for (k = 0; k < l; k += 2) {                    \
			FTYPE xr = px[k];                           \
			FTYPE xi = px[k + 1];                       \
	                                                            \
			sum0 += xr * py0[k];                        \
			sum1 += xi * py0[k];                        \
			sum2 += xr * py0[k + 1];                    \
			sum3 += xi * py0[k + 1];                    \
			sum4 += xr * py1[k];                        \
			sum5 += xi * py1[k];                        \
			sum6 += xr * py1[k + 1];                    \
			sum7 += xi * py1[k + 1];                    \
		    }                                               \
		    sum0 -= sum3;                                   \
		    sum1 += sum2;                                   \
		    sum4 -= sum7;                                   \
		    sum5 += sum6;                                   \
		    STORE_##DD##_##MOD(z[0], sum0);                 \
		    STORE_##DD##_##MOD(z[1], sum1);                 \
		    STORE_##DD##_##MOD(z[2], sum4);                 \
		    STORE_##DD##_##MOD(z[3], sum5);                 \
		    z += 4;                                         \
		    py += 2 * l;                                    \
		}                                                   \
		for (; j < n; j++) {                                \
		    FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
	                                                            \
		    for (k = 0; k < l; k += 2) {                    \
			FTYPE xr = px[k];                           \
			FTYPE xi = px[k + 1];                       \
	                                                            \
			sum0 += xr * py[k];                         \
			sum1 += xi * py[k];                         \
			sum2 += xr * py[k + 1];                     \
			sum3 += xi * py[k + 1];                     \
		    }                                               \
		    sum0 -= sum3;                                   \
		    sum1 += sum2;                                   \
		    STORE_##DD##_##MOD(z[0], sum0);                 \
		    STORE_##DD##_##MOD(z[1], sum1);                 \
		    z += 2;                                         \
		    py += l;                                        \
		}                                                   \
	    }                                                       \
	    __mlib_free(buff);                                      \
	    return (MLIB_SUCCESS);                                  \
	}

/* *********************************************************** */

#define	MATRIX_MUL_FP64(DD, SS, MOD)	MATRIX_MUL_FP(DD, SS, MOD, mlib_d64)
#define	MATRIX_MULC_FP64(DD, SS, MOD)	MATRIX_MULC_FP(DD, SS, MOD, mlib_d64)

/* *********************************************************** */

#if MLIB_VIS < 0x200

#define	MATRIX_MUL_FP32(DD, SS, MOD)                                   \
	mlib_status                                                    \
	__mlib_MatrixMul_##DD##_##SS##_##MOD                           \
		(mlib_##DD * z, const mlib_##SS * x,                   \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 l,           \
		mlib_s32 n)                                            \
	{                                                              \
	    mlib_s32 n_l = n * l, size;                                \
	    mlib_f32 *buff, *py;                                       \
	    d64_2x32 y0, y1, y2, y3;                                   \
	    mlib_s32 i, j, k;                                          \
	                                                               \
	    if (m <= 0 || l <= 0 || n <= 0)                            \
		return (MLIB_FAILURE);                                 \
	    size = n_l + l;                                            \
	    buff = __mlib_malloc(size * sizeof (mlib_f32));            \
	    if (buff == NULL) {                                        \
		return mlib_MatrixMul_type(type_##SS, type_##DD,       \
			mode_##MOD, x, y, m, l, n, n, z);              \
	    }                                                          \
	                                                               \
	/*                                                             \
	 * convert y matrix to mlib_f32                                \
	 */                                                            \
	    py = buff;                                                 \
	    for (j = 0; j <= (n - 8); j += 8) {                        \
		const mlib_##SS *yl = y + j;                           \
	                                                               \
		for (k = 0; k < l; k++) {                              \
		    y0.f32s.f0 = yl[0];                                \
		    y0.f32s.f1 = yl[1];                                \
		    y1.f32s.f0 = yl[2];                                \
		    y1.f32s.f1 = yl[3];                                \
		    y2.f32s.f0 = yl[4];                                \
		    y2.f32s.f1 = yl[5];                                \
		    y3.f32s.f0 = yl[6];                                \
		    y3.f32s.f1 = yl[7];                                \
		    ((mlib_d64 *)py)[0] = y0.d64;                      \
		    ((mlib_d64 *)py)[1] = y1.d64;                      \
		    ((mlib_d64 *)py)[2] = y2.d64;                      \
		    ((mlib_d64 *)py)[3] = y3.d64;                      \
		    py += 8;                                           \
		    yl += n;                                           \
		}                                                      \
	    }                                                          \
	    for (; j < n; j++) {                                       \
		const mlib_##SS *yl = y + j;                           \
	                                                               \
		for (k = 0; k < l; k++) {                              \
		    py[k] = (mlib_f32)yl[k * n];                       \
		}                                                      \
		py += l;                                               \
	    }                                                          \
	    for (i = 0; i < m; i++) {                                  \
		mlib_f32 *px = buff + n_l;                             \
	                                                               \
		for (k = 0; k < l; k++) {                              \
		    px[k] = x[k];                                      \
		}                                                      \
		x += l;                                                \
		py = buff;                                             \
		for (j = 0; j <= (n - 8); j += 8) {                    \
		    mlib_f32 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
		    mlib_f32 sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;   \
	                                                               \
		    for (k = 0; k < l; k++) {                          \
			mlib_f32 xx = px[k];                           \
	                                                               \
			y0.d64 = ((mlib_d64 *)py)[4 * k];              \
			y1.d64 = ((mlib_d64 *)py)[4 * k + 1];          \
			y2.d64 = ((mlib_d64 *)py)[4 * k + 2];          \
			y3.d64 = ((mlib_d64 *)py)[4 * k + 3];          \
			sum0 += xx * y0.f32s.f0;                       \
			sum1 += xx * y0.f32s.f1;                       \
			sum2 += xx * y1.f32s.f0;                       \
			sum3 += xx * y1.f32s.f1;                       \
			sum4 += xx * y2.f32s.f0;                       \
			sum5 += xx * y2.f32s.f1;                       \
			sum6 += xx * y3.f32s.f0;                       \
			sum7 += xx * y3.f32s.f1;                       \
		    }                                                  \
		    STORE_##DD##_##MOD(z[0], sum0);                    \
		    STORE_##DD##_##MOD(z[1], sum1);                    \
		    STORE_##DD##_##MOD(z[2], sum2);                    \
		    STORE_##DD##_##MOD(z[3], sum3);                    \
		    STORE_##DD##_##MOD(z[4], sum4);                    \
		    STORE_##DD##_##MOD(z[5], sum5);                    \
		    STORE_##DD##_##MOD(z[6], sum6);                    \
		    STORE_##DD##_##MOD(z[7], sum7);                    \
		    z += 8;                                            \
		    py += 8 * l;                                       \
		}                                                      \
		for (; j < n; j++) {                                   \
		    mlib_f32 sum0 = 0;                                 \
	                                                               \
		    for (k = 0; k < l; k++) {                          \
			sum0 += px[k] * py[k];                         \
		    }                                                  \
		    STORE_##DD##_##MOD(z[0], sum0);                    \
		    z++;                                               \
		    py += l;                                           \
		}                                                      \
	    }                                                          \
	    __mlib_free(buff);                                         \
	    return (MLIB_SUCCESS);                                     \
	}

#define	MATRIX_MULC_FP32(DD, SS, MOD)                                  \
	mlib_status                                                    \
	__mlib_MatrixMul_##DD##C_##SS##C_##MOD                         \
		(mlib_##DD * z, const mlib_##SS * x,                   \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 l,           \
		mlib_s32 n)                                            \
	{                                                              \
	    mlib_s32 n_l, size;                                        \
	    mlib_d64 *buff, *px, *py;                                  \
	    d64_2x32 xx, yy;                                           \
	    mlib_s32 i, j, k;                                          \
	                                                               \
	    if (m <= 0 || l <= 0 || n <= 0)                            \
		return (MLIB_FAILURE);                                 \
	    n_l = n * l;                                               \
	    size = n_l + l;                                            \
	    buff = __mlib_malloc(size * sizeof (mlib_d64));            \
	    if (buff == NULL) {                                        \
		return mlib_MatrixMul_type(type_##SS##C,               \
			type_##DD##C, mode_##MOD, x, y, m,             \
			l, n, n, z);                                   \
	    }                                                          \
	                                                               \
	/*                                                             \
	 * convert y matrix to mlib_f32                                \
	 */                                                            \
	    py = buff;                                                 \
	    for (i = 0; i < n; i++) {                                  \
		for (k = 0; k < l; k++) {                              \
		    yy.f32s.f0 = y[2 * k * n];                         \
		    yy.f32s.f1 = y[2 * k * n + 1];                     \
		    py[k] = yy.d64;                                    \
		}                                                      \
		y += 2;                                                \
		py += l;                                               \
	    }                                                          \
	    px = buff + n_l;                                           \
	    for (i = 0; i < m; i++) {                                  \
		for (k = 0; k < l; k++) {                              \
		    xx.f32s.f0 = x[2 * k];                             \
		    xx.f32s.f1 = x[2 * k + 1];                         \
		    px[k] = xx.d64;                                    \
		}                                                      \
		x += 2 * l;                                            \
		py = buff;                                             \
		for (j = 0; j < n; j++) {                              \
		    mlib_f32 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
	                                                               \
		    for (k = 0; k < l; k++) {                          \
			xx.d64 = px[k];                                \
			yy.d64 = py[k];                                \
			sum0 += xx.f32s.f0 * yy.f32s.f0;               \
			sum1 += xx.f32s.f1 * yy.f32s.f0;               \
			sum2 += xx.f32s.f0 * yy.f32s.f1;               \
			sum3 += xx.f32s.f1 * yy.f32s.f1;               \
		    }                                                  \
		    sum0 -= sum3;                                      \
		    sum1 += sum2;                                      \
		    STORE_##DD##_##MOD(z[0], sum0);                    \
		    STORE_##DD##_##MOD(z[1], sum1);                    \
		    z += 2;                                            \
		    py += l;                                           \
		}                                                      \
	    }                                                          \
	    __mlib_free(buff);                                         \
	    return (MLIB_SUCCESS);                                     \
	}

#else /* MLIB_VIS < 0x200 */

#define	MATRIX_MUL_FP32(DD, SS, MOD)	MATRIX_MUL_FP(DD, SS, MOD, mlib_f32)
#define	MATRIX_MULC_FP32(DD, SS, MOD)	MATRIX_MULC_FP(DD, SS, MOD, mlib_f32)

#endif /* MLIB_VIS < 0x200 */

/* *********************************************************** */

/* Matrix multiplication with right shifting */

/* *********************************************************** */

#define	SHIFT_S16_Sat(DST, SRC)                                 \
	SRC *= dscale;                                          \
	SATUR_S16(DST, SRC)

/* *********************************************************** */

#define	SHIFT_S16_Mod(DST, SRC)                                 \
	{                                                       \
	    mlib_s32 ires;                                      \
	                                                        \
	    MOD_S32(ires, SRC);                                 \
	    DST = ires >> shift;                                \
	}

/* *********************************************************** */

#define	MATRIX_MUL_SHIFT(DD, SS, MOD, FTYPE)                        \
	mlib_status                                                 \
	__mlib_MatrixMulShift_##DD##_##SS##_##MOD                   \
		(mlib_##DD * z, const mlib_##SS * x,                \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 l,        \
		mlib_s32 n, mlib_s32 shift)                         \
	{                                                           \
	    mlib_d64 dscale = 1.0 / (1 << shift);                   \
	    mlib_s32 n_l = n * l, size;                             \
	    FTYPE *buff, *py;                                       \
	    mlib_s32 i, j, k;                                       \
	                                                            \
	    if (m <= 0 || l <= 0 || n <= 0)                         \
		return (MLIB_FAILURE);                              \
	    if (shift < 1 || shift > 16)                            \
		return (MLIB_OUTOFRANGE);                           \
	    size = n_l + l;                                         \
	    buff = __mlib_malloc(size * sizeof (FTYPE));            \
	    if (buff == NULL) {                                     \
		return mlib_MatrixMulShift_type(type_##SS,          \
			type_##DD, mode_##MOD, x, y, m, l, n, n, z, \
			shift);                                     \
	    }                                                       \
	                                                            \
	/*                                                          \
	 * convert y matrix to FTYPE                                \
	 */                                                         \
	    py = buff;                                              \
	    for (i = 0; i < n; i++) {                               \
		for (k = 0; k < l; k++) {                           \
		    py[k] = (FTYPE) y[k * n];                       \
		}                                                   \
		y++;                                                \
		py += l;                                            \
	    }                                                       \
	    for (i = 0; i < m; i++) {                               \
		FTYPE *px = buff + n_l;                             \
	                                                            \
		for (k = 0; k < l; k++) {                           \
		    px[k] = x[k];                                   \
		}                                                   \
		x += l;                                             \
		py = buff;                                          \
		for (j = 0; j <= (n - 8); j += 8) {                 \
		    FTYPE *py0 = py;                                \
		    FTYPE *py1 = py0 + l;                           \
		    FTYPE *py2 = py1 + l;                           \
		    FTYPE *py3 = py2 + l;                           \
		    FTYPE *py4 = py3 + l;                           \
		    FTYPE *py5 = py4 + l;                           \
		    FTYPE *py6 = py5 + l;                           \
		    FTYPE *py7 = py6 + l;                           \
		    FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
		    FTYPE sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;   \
	                                                            \
		    for (k = 0; k < l; k++) {                       \
			FTYPE xx = px[k];                           \
	                                                            \
			sum0 += xx * py0[k];                        \
			sum1 += xx * py1[k];                        \
			sum2 += xx * py2[k];                        \
			sum3 += xx * py3[k];                        \
			sum4 += xx * py4[k];                        \
			sum5 += xx * py5[k];                        \
			sum6 += xx * py6[k];                        \
			sum7 += xx * py7[k];                        \
		    }                                               \
		    SHIFT_##DD##_##MOD(z[0], sum0);                 \
		    SHIFT_##DD##_##MOD(z[1], sum1);                 \
		    SHIFT_##DD##_##MOD(z[2], sum2);                 \
		    SHIFT_##DD##_##MOD(z[3], sum3);                 \
		    SHIFT_##DD##_##MOD(z[4], sum4);                 \
		    SHIFT_##DD##_##MOD(z[5], sum5);                 \
		    SHIFT_##DD##_##MOD(z[6], sum6);                 \
		    SHIFT_##DD##_##MOD(z[7], sum7);                 \
		    z += 8;                                         \
		    py += 8 * l;                                    \
		}                                                   \
		for (; j < n; j++) {                                \
		    FTYPE sum0 = 0;                                 \
	                                                            \
		    for (k = 0; k < l; k++) {                       \
			sum0 += px[k] * py[k];                      \
		    }                                               \
		    SHIFT_##DD##_##MOD(z[0], sum0);                 \
		    z++;                                            \
		    py += l;                                        \
		}                                                   \
	    }                                                       \
	    __mlib_free(buff);                                      \
	    return (MLIB_SUCCESS);                                  \
	}

/* *********************************************************** */

#define	MATRIX_MULC_SHIFT(DD, SS, MOD, FTYPE)                       \
	mlib_status                                                 \
	__mlib_MatrixMulShift_##DD##C_##SS##C_##MOD                 \
		(mlib_##DD * z, const mlib_##SS * x,                \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 l,        \
		mlib_s32 n, mlib_s32 shift)                         \
	{                                                           \
	    mlib_d64 dscale = 1.0 / (1 << shift);                   \
	    mlib_s32 n_l, size;                                     \
	    FTYPE *buff, *py;                                       \
	    mlib_s32 i, j, k;                                       \
	                                                            \
	    if (m <= 0 || l <= 0 || n <= 0)                         \
		return (MLIB_FAILURE);                              \
	    if (shift < 1 || shift > 16)                            \
		return (MLIB_OUTOFRANGE);                           \
	    l *= 2;                                                 \
	/*                                                          \
	 * complex                                                  \
	 */                                                         \
	    n_l = n * l;                                            \
	    size = n_l + l;                                         \
	    buff = __mlib_malloc(size * sizeof (FTYPE));            \
	    if (buff == NULL) {                                     \
		return mlib_MatrixMulShift_type(type_##SS##C,       \
			type_##DD##C, mode_##MOD, x, y, m,          \
			l, n, n, z, shift);                         \
	    }                                                       \
	                                                            \
	/*                                                          \
	 * convert y matrix to FTYPE                                \
	 */                                                         \
	    py = buff;                                              \
	    for (i = 0; i < n; i++) {                               \
		for (k = 0; k < l; k += 2) {                        \
		    py[k] = (FTYPE) y[k * n];                       \
		    py[k + 1] = (FTYPE) y[k * n + 1];               \
		}                                                   \
		y += 2;                                             \
		py += l;                                            \
	    }                                                       \
	    for (i = 0; i < m; i++) {                               \
		FTYPE *px = buff + n_l;                             \
	                                                            \
		for (k = 0; k < l; k++) {                           \
		    px[k] = x[k];                                   \
		}                                                   \
		x += l;                                             \
		py = buff;                                          \
		for (j = 0; j <= (n - 2); j += 2) {                 \
		    FTYPE *py0 = py;                                \
		    FTYPE *py1 = py0 + l;                           \
		    FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
		    FTYPE sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;   \
	                                                            \
		    for (k = 0; k < l; k += 2) {                    \
			FTYPE xr = px[k];                           \
			FTYPE xi = px[k + 1];                       \
	                                                            \
			sum0 += xr * py0[k];                        \
			sum1 += xi * py0[k];                        \
			sum2 += xr * py0[k + 1];                    \
			sum3 += xi * py0[k + 1];                    \
			sum4 += xr * py1[k];                        \
			sum5 += xi * py1[k];                        \
			sum6 += xr * py1[k + 1];                    \
			sum7 += xi * py1[k + 1];                    \
		    }                                               \
		    sum0 -= sum3;                                   \
		    sum1 += sum2;                                   \
		    sum4 -= sum7;                                   \
		    sum5 += sum6;                                   \
		    SHIFT_##DD##_##MOD(z[0], sum0);                 \
		    SHIFT_##DD##_##MOD(z[1], sum1);                 \
		    SHIFT_##DD##_##MOD(z[2], sum4);                 \
		    SHIFT_##DD##_##MOD(z[3], sum5);                 \
		    z += 4;                                         \
		    py += 2 * l;                                    \
		}                                                   \
		for (; j < n; j++) {                                \
		    FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;   \
	                                                            \
		    for (k = 0; k < l; k += 2) {                    \
			FTYPE xr = px[k];                           \
			FTYPE xi = px[k + 1];                       \
	                                                            \
			sum0 += xr * py[k];                         \
			sum1 += xi * py[k];                         \
			sum2 += xr * py[k + 1];                     \
			sum3 += xi * py[k + 1];                     \
		    }                                               \
		    sum0 -= sum3;                                   \
		    sum1 += sum2;                                   \
		    SHIFT_##DD##_##MOD(z[0], sum0);                 \
		    SHIFT_##DD##_##MOD(z[1], sum1);                 \
		    z += 2;                                         \
		    py += l;                                        \
		}                                                   \
	    }                                                       \
	    __mlib_free(buff);                                      \
	    return (MLIB_SUCCESS);                                  \
	}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_MATRIXMUL_H */
