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

#ifndef _MLIB_VECTORMULM_H
#define	_MLIB_VECTORMULM_H

#pragma ident	"@(#)mlib_VectorMulM.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * FUNCTIONS
 *      Macros for mlib_VectorMulM_* functions
 *      mlib_VectorMulM_S32_S32_Sat
 *      mlib_VectorMulM_S16_S16_Sat
 *      mlib_VectorMulM_S32_S16_Sat
 *
 */

#include <mlib_Utils.h>
#include <mlib_VectorMulM_proto.h>

/* *********************************************************** */

#define	type_union_mlib_d64	type_union_mlib_d64_a
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#define	mlib_U8	mlib_u8
#define	mlib_S8	mlib_s8
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

#define	MAX_SIZE	400

/* *********************************************************** */

#define	FTYPE	mlib_d64

/* *********************************************************** */

#define	VECTOR_MULM_FP(DD, SS, MOD)                              \
	mlib_status                                              \
	mlib_VectorMulM_S_##DD##_##SS##_##MOD                    \
		(mlib_##DD * z, const mlib_##SS * x,             \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 n,     \
		mlib_s32 nstride)                                \
	{                                                        \
	    mlib_s32 i, j;                                       \
	    FTYPE buff[MAX_SIZE], *px = buff;                    \
	                                                         \
	    if (m <= 0 || n <= 0)                                \
		return (MLIB_FAILURE);                           \
	    if (m > MAX_SIZE) {                                  \
		px = __mlib_malloc(m * sizeof (FTYPE));          \
		if (px == NULL)                                  \
		    return mlib_MatrixMul_type(type_##SS,        \
			    type_##DD, mode_##MOD, x, y, 1, m,   \
			    n, nstride, z);                      \
	    }                                                    \
	    for (i = 0; i < m; i++) {                            \
		px[i] = (FTYPE) x[i];                            \
	    }                                                    \
	    for (j = 0; j <= (n - 8); j += 8) {                  \
		const mlib_##SS *py = y + j;                     \
		FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;    \
		FTYPE sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;    \
		FTYPE xx;                                        \
	                                                         \
		for (i = 0; i < m; i++) {                        \
		    xx = px[i];                                  \
		    sum0 += xx * py[0];                          \
		    sum1 += xx * py[1];                          \
		    sum2 += xx * py[2];                          \
		    sum3 += xx * py[3];                          \
		    sum4 += xx * py[4];                          \
		    sum5 += xx * py[5];                          \
		    sum6 += xx * py[6];                          \
		    sum7 += xx * py[7];                          \
		    py += nstride;                               \
		}                                                \
		STORE_##DD##_##MOD(z[j], sum0);                  \
		STORE_##DD##_##MOD(z[j + 1], sum1);              \
		STORE_##DD##_##MOD(z[j + 2], sum2);              \
		STORE_##DD##_##MOD(z[j + 3], sum3);              \
		STORE_##DD##_##MOD(z[j + 4], sum4);              \
		STORE_##DD##_##MOD(z[j + 5], sum5);              \
		STORE_##DD##_##MOD(z[j + 6], sum6);              \
		STORE_##DD##_##MOD(z[j + 7], sum7);              \
	    }                                                    \
	    for (; j < n; j++) {                                 \
		const mlib_##SS *py = y + j;                     \
		FTYPE sum = 0;                                   \
		FTYPE xx;                                        \
	                                                         \
		for (i = 0; i < m; i++) {                        \
		    xx = px[i];                                  \
		    sum += xx * py[0];                           \
		    py += nstride;                               \
		}                                                \
		STORE_##DD##_##MOD(z[j], sum);                   \
	    }                                                    \
	    if (px != buff)                                      \
		__mlib_free(px);                                 \
	    return (MLIB_SUCCESS);                               \
	}                                                        \
	                                                         \
	mlib_status                                              \
	__mlib_VectorMulM_##DD##_##SS##_##MOD                    \
		(mlib_##DD * z, const mlib_##SS * x,             \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 n)     \
	{                                                        \
		return mlib_VectorMulM_S_##DD##_##SS##_##MOD(z,  \
			x, y, m, n, n);                          \
	}

/* *********************************************************** */

#define	VECTORC_MULM_FP(DD, SS, MOD)                              \
	mlib_status                                               \
	mlib_VectorMulM_S_##DD##C_##SS##C_##MOD                   \
		(mlib_##DD * z, const mlib_##SS * x,              \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 n,      \
		mlib_s32 nstride)                                 \
	{                                                         \
	    mlib_s32 i, j;                                        \
	    FTYPE buff[MAX_SIZE], *px = buff;                     \
	                                                          \
	    if (m <= 0 || n <= 0)                                 \
		return (MLIB_FAILURE);                            \
	    if (2 * m > MAX_SIZE) {                               \
		px = __mlib_malloc(2 * m * sizeof (FTYPE));       \
		if (px == NULL)                                   \
		    return mlib_MatrixMul_type(type_##SS##C,      \
			    type_##DD##C, mode_##MOD, x, y,       \
			    1, m, n, nstride, z);                 \
	    }                                                     \
	    m *= 2;                                               \
	    n *= 2;                                               \
	    nstride *= 2;                                         \
	    for (i = 0; i < m; i++) {                             \
		px[i] = (FTYPE) x[i];                             \
	    }                                                     \
	    for (j = 0; j <= (n - 4); j += 2 * 2) {               \
		const mlib_##SS *py = y + j;                      \
		FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;     \
		FTYPE sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;     \
	                                                          \
		for (i = 0; i < m; i += 2) {                      \
		    FTYPE xr = px[i];                             \
		    FTYPE xi = px[i + 1];                         \
	                                                          \
		    sum0 += xr * py[0];                           \
		    sum1 += xi * py[0];                           \
		    sum2 += xr * py[1];                           \
		    sum3 += xi * py[1];                           \
		    sum4 += xr * py[2];                           \
		    sum5 += xi * py[2];                           \
		    sum6 += xr * py[3];                           \
		    sum7 += xi * py[3];                           \
		    py += nstride;                                \
		}                                                 \
		sum0 -= sum3;                                     \
		sum1 += sum2;                                     \
		sum4 -= sum7;                                     \
		sum5 += sum6;                                     \
		STORE_##DD##_##MOD(z[j], sum0);                   \
		STORE_##DD##_##MOD(z[j + 1], sum1);               \
		STORE_##DD##_##MOD(z[j + 2], sum4);               \
		STORE_##DD##_##MOD(z[j + 3], sum5);               \
	    }                                                     \
	    for (; j < n; j += 2) {                               \
		const mlib_##SS *py = y + j;                      \
		FTYPE sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;     \
	                                                          \
		for (i = 0; i < m; i += 2) {                      \
		    FTYPE xr = px[i];                             \
		    FTYPE xi = px[i + 1];                         \
	                                                          \
		    sum0 += xr * py[0];                           \
		    sum1 += xi * py[0];                           \
		    sum2 += xr * py[1];                           \
		    sum3 += xi * py[1];                           \
		    py += nstride;                                \
		}                                                 \
		sum0 -= sum3;                                     \
		sum1 += sum2;                                     \
		STORE_##DD##_##MOD(z[j], sum0);                   \
		STORE_##DD##_##MOD(z[j + 1], sum1);               \
	    }                                                     \
	    if (px != buff)                                       \
		__mlib_free(px);                                  \
	    return (MLIB_SUCCESS);                                \
	}                                                         \
	                                                          \
	mlib_status                                               \
	__mlib_VectorMulM_##DD##C_##SS##C_##MOD                   \
		(mlib_##DD * z, const mlib_##SS * x,              \
		const mlib_##SS * y, mlib_s32 m, mlib_s32 n)      \
	{                                                         \
		return mlib_VectorMulM_S_##DD##C_##SS##C_##MOD(z, \
			x, y, m, n, n);                           \
	}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VECTORMULM_H */
