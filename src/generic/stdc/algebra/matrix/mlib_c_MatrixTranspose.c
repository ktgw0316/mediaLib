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

#pragma ident	"@(#)mlib_c_MatrixTranspose.c	9.5	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixTranspose_U8_U8 - transposition of 8-bit
 *                                   format element matrix
 *      mlib_MatrixTranspose_S8_S8 - transposition of 8-bit
 *                                   format element matrix
 *      mlib_MatrixTranspose_S16_S16 - transposition of 16-bit
 *                                     format element matrix
 *      mlib_MatrixTranspose_S32_S32 - transposition of 32-bit
 *                                     format element matrix
 *      mlib_MatrixTranspose_U8  - transposition of 8-bit format element matrix
 *                                (input and output vectors are the same)
 *      mlib_MatrixTranspose_S8  - transposition of 8-bit format element matrix
 *                                (input and output vectors are the same)
 *      mlib_MatrixTranspose_S16 - transposition of 16-bit format element matrix
 *                                (input and output vectors are the same)
 *      mlib_MatrixTranspose_S32 - transposition of 32-bit format element matrix
 *                                (input and output vectors are the same)
 *      mlib_MatrixTranspose_U8C_U8C - transposition of 8-bit format complex
 *                                    element matrix
 *      mlib_MatrixTranspose_S8C_S8C - transposition of 8-bit format complex
 *                                    element matrix
 *      mlib_MatrixTranspose_S16C_S16C - transposition of 16-bit format complex
 *                                    element matrix
 *      mlib_MatrixTranspose_S32C_S32C - transposition of 32-bit format complex
 *                                    element matrix
 *      mlib_MatrixTranspose_U8C       - transposition of 8-bit format complex
 *                                    element matrix
 *      mlib_MatrixTranspose_S8C       - transposition of 8-bit format complex
 *                                    element matrix (input and output vectors
 *                                    are the same)
 *      mlib_MatrixTranspose_S16C      - transposition of 16-bit format complex
 *                                    element matrix (input and output vectors
 *                                    are the same)
 *      mlib_MatrixTranspose_S32C      - transposition of 32-bit format complex
 *                                    element matrix (input and output vectors
 *                                    are the same)
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixTranspose_S16(mlib_s16 * xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S16C(mlib_s16 * xz,
 *                                            mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S16C_S16C(mlib_s16 * zz,
 *                                                 const mlib_s16 * xx,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S16_S16(mlib_s16 * zz,
 *                                               const mlib_s16 * xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S32(mlib_s32 * xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S32C(mlib_s32 * xz,
 *                                            mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S32C_S32C(mlib_s32 * zz,
 *                                                 const mlib_s32 * xx,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S32_S32(mlib_s32 * zz,
 *                                               const mlib_s32 * xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S8(mlib_s8 * xz,
 *                                          mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S8C(mlib_s8 * xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S8C_S8C(mlib_s8 * zz,
 *                                               const mlib_s8 * xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S8_S8(mlib_s8 * zz,
 *                                             const mlib_s8 * xx,
 *                                             mlib_s32 m,
 *                                             mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_U8(mlib_u8 * xz,
 *                                          mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_U8C(mlib_u8 * xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_U8C_U8C(mlib_u8 * zz,
 *                                               const mlib_u8 * xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_U8_U8(mlib_u8 * zz,
 *                                             const mlib_u8 * xx,
 *                                             mlib_s32 m,
 *                                             mlib_s32 n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      xz   pointer to the first element of the input and output matrix.
 *      x    pointer to the first element of the input matrix.
 *           The matrix is in row major order
 *      m    number of rows in the matrix
 *      n    number of columns in the matrix
 *      mn   number of rows and columns in the square matrix
 *
 * DESCRIPTION
 *          _
 *      z = x
 *
 * IMPLEMENTATION
 *
 *      In all cases we process matrix by 4*4 elements. The main idea
 *      of such processing is quite simple: inside each "square" we
 *      can  read 16 elements into registers successively (i.e. with successive
 *      source address increasing), and then write them into destination
 *      symmetric "square" also successively (with successive destination
 *      address increasing) via using registers in appropriate order.
 *      For instance, reading (x[1, 1]->a1, x[1, 2]->a2, x[1, 3]->a3,
 *      x[1, 4]->a4, x[2, 1]->a5, x[2, 2]->a6, ... x[4, 4]->a16)
 *      we store them as (a1->z[1, 1], a5->z[1, 2], a9->z[1, 3]...).
 *      This trick gives at least 25% speed - up even without using VIS.
 *
 *      For little format we also take elements by 1 loading of 32 bits
 *      and then "&" and shift instead of 4 (or 2) loads (of course,
 *      in case of appropriate alignment). This gives about 30% speed - up.
 *
 *      In case of Complex matrix we first try to transpose it in terms
 *      of mlib_d64 format. It is possible only when source and destination
 *      addresses are double - format aligned (i.e. _S8 vector is 16-bit
 *      aligned, _S16 vector is 32-bit  aligned etc.). If it is not so,
 *      we process it by 4*4 elements as in real case. The only difference
 *      is that we read not 16 elements but 2 times for 16 elements per
 *      one iteration (see comments in procedures).
 */

#include <mlib_algebra.h>
#include <mlib_MatrixTranspose.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixTranspose_S16 = __mlib_MatrixTranspose_S16
#pragma weak mlib_MatrixTranspose_S32C = __mlib_MatrixTranspose_S32C
#pragma weak mlib_MatrixTranspose_S8C_S8C = __mlib_MatrixTranspose_S8C_S8C
#pragma weak mlib_MatrixTranspose_S8 = __mlib_MatrixTranspose_S8
#pragma weak mlib_MatrixTranspose_S16C = __mlib_MatrixTranspose_S16C
#pragma weak mlib_MatrixTranspose_U8 = __mlib_MatrixTranspose_U8
#pragma weak mlib_MatrixTranspose_S8C = __mlib_MatrixTranspose_S8C
#pragma weak mlib_MatrixTranspose_U8C_U8C = __mlib_MatrixTranspose_U8C_U8C
#pragma weak mlib_MatrixTranspose_S32_S32 = __mlib_MatrixTranspose_S32_S32
#pragma weak mlib_MatrixTranspose_U8C = __mlib_MatrixTranspose_U8C
#pragma weak mlib_MatrixTranspose_S16_S16 = __mlib_MatrixTranspose_S16_S16
#pragma weak mlib_MatrixTranspose_S32C_S32C = \
	__mlib_MatrixTranspose_S32C_S32C
#pragma weak mlib_MatrixTranspose_S8_S8 = __mlib_MatrixTranspose_S8_S8
#pragma weak mlib_MatrixTranspose_S32 = __mlib_MatrixTranspose_S32
#pragma weak mlib_MatrixTranspose_S16C_S16C = \
	__mlib_MatrixTranspose_S16C_S16C
#pragma weak mlib_MatrixTranspose_U8_U8 = __mlib_MatrixTranspose_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_MatrixTranspose_S16) mlib_MatrixTranspose_S16
	__attribute__((weak, alias("__mlib_MatrixTranspose_S16")));
__typeof__(__mlib_MatrixTranspose_S32C) mlib_MatrixTranspose_S32C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S32C")));
__typeof__(__mlib_MatrixTranspose_S8C_S8C) mlib_MatrixTranspose_S8C_S8C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S8C_S8C")));
__typeof__(__mlib_MatrixTranspose_S8) mlib_MatrixTranspose_S8
	__attribute__((weak, alias("__mlib_MatrixTranspose_S8")));
__typeof__(__mlib_MatrixTranspose_S16C) mlib_MatrixTranspose_S16C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S16C")));
__typeof__(__mlib_MatrixTranspose_U8) mlib_MatrixTranspose_U8
	__attribute__((weak, alias("__mlib_MatrixTranspose_U8")));
__typeof__(__mlib_MatrixTranspose_S8C) mlib_MatrixTranspose_S8C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S8C")));
__typeof__(__mlib_MatrixTranspose_U8C_U8C) mlib_MatrixTranspose_U8C_U8C
	__attribute__((weak, alias("__mlib_MatrixTranspose_U8C_U8C")));
__typeof__(__mlib_MatrixTranspose_S32_S32) mlib_MatrixTranspose_S32_S32
	__attribute__((weak, alias("__mlib_MatrixTranspose_S32_S32")));
__typeof__(__mlib_MatrixTranspose_U8C) mlib_MatrixTranspose_U8C
	__attribute__((weak, alias("__mlib_MatrixTranspose_U8C")));
__typeof__(__mlib_MatrixTranspose_S16_S16) mlib_MatrixTranspose_S16_S16
	__attribute__((weak, alias("__mlib_MatrixTranspose_S16_S16")));
__typeof__(__mlib_MatrixTranspose_S32C_S32C) mlib_MatrixTranspose_S32C_S32C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S32C_S32C")));
__typeof__(__mlib_MatrixTranspose_S8_S8) mlib_MatrixTranspose_S8_S8
	__attribute__((weak, alias("__mlib_MatrixTranspose_S8_S8")));
__typeof__(__mlib_MatrixTranspose_S32) mlib_MatrixTranspose_S32
	__attribute__((weak, alias("__mlib_MatrixTranspose_S32")));
__typeof__(__mlib_MatrixTranspose_S16C_S16C) mlib_MatrixTranspose_S16C_S16C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S16C_S16C")));
__typeof__(__mlib_MatrixTranspose_U8_U8) mlib_MatrixTranspose_U8_U8
	__attribute__((weak, alias("__mlib_MatrixTranspose_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef i386
/*
 * do not perform the copying by mlib_d64 data type for x86
 */

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_32BIT	mlib_u32
#define	TYPE_64BIT	two_int

#else /* i386 */

#define	TYPE_32BIT	mlib_f32
#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

/* *********************************************************** */

static void mlib_transpose8_4_al4x(
	mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s8 *z,
	mlib_s32 ds);
static void mlib_transpose8_cc(
	mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s8 *z,
	mlib_s32 ds);
static void mlib_transpose8C_cc(
	mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_u8 *z,
	mlib_s32 ds);
static void mlib_transpose16_4_al4x(
	mlib_s16 const *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s16 *z,
	mlib_s32 ds);
static void mlib_transpose16_cc(
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s16 *z,
	mlib_s32 ds);
static void mlib_transpose16C_cc(
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s16 *z,
	mlib_s32 ds);
static void mlib_transpose32_cc(
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 *z,
	mlib_s32 ds);
static mlib_status mlib_MatrixTranspose_D64(
	void *xz,
	mlib_s32 nm);
static mlib_status
mlib_MatrixTranspose_S_D64(
	TYPE_64BIT *z,
	TYPE_64BIT *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
static void mlib_transpose32C_cc(
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 *z,
	mlib_s32 ds);
static void mlib_transpose64_cc(
	const TYPE_64BIT * x,
	mlib_s32 m,
	mlib_s32 n,
	TYPE_64BIT * z,
	mlib_s32 ds);
static mlib_status mlib_MatrixTranspose_D64_D64(
	void *zz,
	const void *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
static void
mlib_trans32_diag_4_alxx(
	mlib_s32 *xz,
	mlib_s32 nm,
	mlib_s32 ds);
static void
mlib_trans32_diag_4_alxx_s(
	mlib_s32 *z,
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
static void
mlib_trans64_diag_4(
	TYPE_64BIT * xz,
	mlib_s32 nm,
	mlib_s32 ds);
static void
mlib_trans64_diag_4_s(
	TYPE_64BIT * z,
	TYPE_64BIT * x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

/* *********************************************************** */

#define	INDVAR(arr, i1, i2, n)	arr[(i1)*n + i2]

/* *********************************************************** */

#define	INDVAR_C_RE(arr, i1, i2, n)	arr[(i1)*(n << 1)+2*i2]

/* *********************************************************** */

#define	INDVAR_C_IM(arr, i1, i2, n)	arr[(i1)*(n << 1)+2*i2 + 1]

/* *********************************************************** */

#define	TRANS                                                   \
	mlib_s32 i, j, inv_j;                                   \
	                                                        \
	for (i = 0; i < m; i++)                                 \
	    for (j = 0, inv_j = 0; j < n; j++) {                \
		z[inv_j + i] = (*x++);                          \
		inv_j += ds;                                    \
	    }                                                   \
	                                                        \
	return MLIB_SUCCESS

/* Macro for Complex  matrix transpose. */

#define	TRANS_C(form)                                            \
	{                                                        \
	    form *p_z;                                           \
	    mlib_s32 i, j, inv_j;                                \
	                                                         \
	    for (i = 0; i < (m << 1); i += 2)                    \
		for (j = 0, inv_j = 0; j < (n << 1); j += 2) {   \
		    *(p_z = &z[inv_j + i]) = (*x++);             \
		    *(p_z + 1) = (*x++);                         \
		    inv_j += (ds << 1);                          \
		}                                                \
	    return (MLIB_SUCCESS);                               \
	}

/*
 * Makes matrix transpose when input and output vectors are the same.
 * This is the processing of elements above main diagonal
 */

#define	TRANS_DIAG(form)                                        \
	{                                                       \
	    form *z = xz, *p_z;                                 \
	    form *x = xz, *p_x;                                 \
	    mlib_s32 i, j, inv_i = 0;                           \
	    form a;                                             \
	                                                        \
	    for (i = 0; i < nm; i++) {                          \
		j = i + 1;                                      \
		p_x = &x[inv_i + j];                            \
		p_z = &z[j * nm + i];                           \
		if ((nm - j) & 1) {                             \
		    a = *(p_z);                                 \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    p_z += nm;                                  \
		    j++;                                        \
		}                                               \
		for (; j < nm; j += 2) {                        \
		    a = *(p_z);                                 \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    a = *(p_z += nm);                           \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    p_z += nm;                                  \
		}                                               \
		inv_i += nm;                                    \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

#define	TRANSS_DIAG(form)                                       \
	{                                                       \
	    form *p_z;                                          \
	    form *p_x;                                          \
	    mlib_s32 i, j, inv_i = 0;                           \
	    form a;                                             \
	                                                        \
	    for (i = 0; i < m; i++) {                           \
		j = i + 1;                                      \
		p_x = &x[inv_i + j];                            \
		p_z = &z[j * ds + i];                           \
		if ((n - j) & 1) {                              \
		    a = *(p_z);                                 \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    p_z += ds;                                  \
		    j++;                                        \
		}                                               \
		for (; j < n; j += 2) {                         \
		    a = *(p_z);                                 \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    a = *(p_z += ds);                           \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    p_z += ds;                                  \
		}                                               \
		inv_i += ds;                                    \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}
#define	TRANSS_DIAG2(form)                                      \
	{                                                       \
	    form *p_z;                                          \
	    form *p_x;                                          \
	    mlib_s32 i, j, inv_i = 0;                           \
	    form a;                                             \
	    for (i = 0; i < m; i++) {                           \
		j = 0;                                          \
		p_x = &x[inv_i + j];                            \
		p_z = &z[j * ds + i];                           \
		if ((n - j) & 1) {                              \
		    a = *(p_z);                                 \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    p_z += ds;                                  \
		    j++;                                        \
		}                                               \
		for (; j < n; j += 2) {                         \
		    a = *(p_z);                                 \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    a = *(p_z += ds);                           \
		    *p_z = *p_x;                                \
		    (*p_x++) = a;                               \
		    p_z += ds;                                  \
		}                                               \
		inv_i += ds;                                    \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/*
 * Makes Complex matrix transpose when input and output vectors are the same.
 * This is the processing of elements above main diagonal
 */

#define	TRANS_DIAG_C(form)                                      \
	form *z = xz, *p_z, *p_z1;                              \
	form *x = xz, *p_x, *p_x1;                              \
	mlib_s32 i, j, inv_i = 0;                               \
	form a_r, a_i;                                          \
	                                                        \
	for (i = 0; i < (nm << 1); i += 2) {                    \
	    j = i + 2;                                          \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[(j * nm) + i];                             \
	    for (j; j < (nm << 1); j += 2) {                    \
		p_z1 = p_z + 1;                                 \
		p_x1 = p_x + 1;                                 \
		a_r = *(p_z);                                   \
		a_i = *(p_z1);                                  \
		*p_z = *p_x;                                    \
		*p_z1 = *p_x1;                                  \
		*p_x = a_r;                                     \
		*p_x1 = a_i;                                    \
		p_x += 2;                                       \
		p_z += (nm << 1);                               \
	    }                                                   \
	    inv_i += (nm << 1);                                 \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

#define	TRANSS_DIAG_C(form)                                     \
	form *p_z, *p_z1;                                       \
	form *p_x, *p_x1;                                       \
	mlib_s32 i, j, inv_i = 0;                               \
	form a_r, a_i;                                          \
	                                                        \
	for (i = 0; i < (m << 1); i += 2) {                     \
	    j = i + 2;                                          \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[(j * ds) + i];                             \
	    for (j; j < (n << 1); j += 2) {                     \
		p_z1 = p_z + 1;                                 \
		p_x1 = p_x + 1;                                 \
		a_r = *(p_z);                                   \
		a_i = *(p_z1);                                  \
		*p_z = *p_x;                                    \
		*p_z1 = *p_x1;                                  \
		*p_x = a_r;                                     \
		*p_x1 = a_i;                                    \
		p_x += 2;                                       \
		p_z += (ds << 1);                               \
	    }                                                   \
	    inv_i += (ds << 1);                                 \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

#define	TRANSS_DIAG2_C(form)                                    \
	form *p_z, *p_z1;                                       \
	form *p_x, *p_x1;                                       \
	mlib_s32 i, j, inv_i = 0;                               \
	form a_r, a_i;                                          \
	                                                        \
	for (i = 0; i < (m << 1); i += 2) {                     \
	    j = 0;                                              \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[(j * ds) + i];                             \
	    for (; j < (n << 1); j += 2) {                      \
		p_z1 = p_z + 1;                                 \
		p_x1 = p_x + 1;                                 \
		a_r = *(p_z);                                   \
		a_i = *(p_z1);                                  \
		*p_z = *p_x;                                    \
		*p_z1 = *p_x1;                                  \
		*p_x = a_r;                                     \
		*p_x1 = a_i;                                    \
		p_x += 2;                                       \
		p_z += (ds << 1);                               \
	    }                                                   \
	    inv_i += (ds << 1);                                 \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/*
 *
 *  Process matrix by 4*4 elements with 4-byte aligned source address.
 *  Destination address is not important.
 *  n & 3 == 0 !!!
 *
 */

static void
mlib_transpose8_4_al4x(
	mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s8 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	mlib_s8 *p_z;
	mlib_s32 *dsrc;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 2, step_m = ds << 2,
		shift =	n >> 2;

	for (i = 0; i < m; i += 4) {
		for (j = 0; j < n; j += 4) {
			dsrc = (mlib_s32 *)&x[inv_i + j];

#ifndef _LITTLE_ENDIAN

			a4 = *dsrc;
			a3 = (a4) >> 8;
			a2 = (a4) >> 16;
			a1 = (a4) >> 24;

			dsrc += shift;
			a8 = *dsrc;
			a7 = (a8) >> 8;
			a6 = (a8) >> 16;
			a5 = (a8) >> 24;

			dsrc += shift;
			a12 = *dsrc;
			a11 = (a12) >> 8;
			a10 = (a12) >> 16;
			a9 = (a12) >> 24;

			dsrc += shift;
			a16 = *dsrc;
			a15 = (a16) >> 8;
			a14 = (a16) >> 16;
			a13 = (a16) >> 24;

#else /* _LITTLE_ENDIAN */

			a1 = *dsrc;
			a2 = (a1) >> 8;
			a3 = (a1) >> 16;
			a4 = (a1) >> 24;

			dsrc += shift;
			a5 = *dsrc;
			a6 = (a5) >> 8;
			a7 = (a5) >> 16;
			a8 = (a5) >> 24;

			dsrc += shift;
			a9 = *dsrc;
			a10 = (a9) >> 8;
			a11 = (a9) >> 16;
			a12 = (a9) >> 24;

			dsrc += shift;
			a13 = *dsrc;
			a14 = (a13) >> 8;
			a15 = (a13) >> 16;
			a16 = (a13) >> 24;

#endif /* _LITTLE_ENDIAN */

			p_z = &z[inv_j + i];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + i + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + i + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + i + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

			inv_j += step_m;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 3 != 0 (common case).
 *
 */

static void
mlib_transpose8_cc(
	mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s8 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	mlib_s8 *p_x, *p_z;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 2, step_m = ds << 2;

	for (i = 0; i <= m - 4; i += 4) {
		for (j = 0; j <= n - 4; j += 4) {
			p_x = &x[inv_i + j];
			a1 = (*p_x++);
			a2 = (*p_x++);
			a3 = (*p_x++);
			a4 = *p_x;

			p_x = &x[inv_i + j + n];
			a5 = (*p_x++);
			a6 = (*p_x++);
			a7 = (*p_x++);
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];
			a9 = (*p_x++);
			a10 = (*p_x++);
			a11 = (*p_x++);
			a12 = *p_x;

			p_x = &x[inv_i + j + n + (n << 1)];
			a13 = (*p_x++);
			a14 = (*p_x++);
			a15 = (*p_x++);
			a16 = *p_x;

			p_z = &z[inv_j + i];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + i + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + i + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + i + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;
			inv_j += step_m;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 3 != 0 (common case). This
 *  procedure is used for Complex matrix elements, so read and write
 *  by 2 elements (real and imaginary parts).
 *
 *  In comments "AI" means the complex value as a whole, as if
 *  the real and imaginary parts were allocated in registers at the
 *  same time.
 */

static void
mlib_transpose8C_cc(
	mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_u8 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
/* pointer  to source matrix */
	mlib_u8 *p_x,
/* pointers to destination matrix */
	*p_z, *p_z1, *p_z2, *p_z3;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 3, step_m = ds << 3;

	for (i = 0; i <= m * 2 - 8; i += 8) {
		for (j = 0; j <= n * 2 - 8; j += 8) {
			p_z = &z[inv_j + i];
			p_z1 = &z[inv_j + i + (ds << 1)];
			p_z2 = &z[inv_j + i + (ds << 2)];
			p_z3 = &z[inv_j + i + (ds << 1) + (ds << 2)];

			inv_j += step_m;

/* First Read 16 values to allocate them in mlib_s32 registers. */

			p_x = &x[inv_i + j];

/* real part of A1 */
			a1 = (*p_x++);
/* imaginary part of A1 */
			a2 = (*p_x++);
/* real part of A2 */
			a3 = (*p_x++);
/* imaginary part of A2 */
			a4 = (*p_x++);
/* real part of A3 */
			a5 = (*p_x++);
/* imaginary part of A3 */
			a6 = (*p_x++);
/* real part of A4 */
			a7 = (*p_x++);
/* imaginary part of A4 */
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];

/* real part of A5 */
			a9 = (*p_x++);
/* imaginary part of A5 */
			a10 = (*p_x++);
/* real part of A6 */
			a11 = (*p_x++);
/* imaginary part of A6 */
			a12 = (*p_x++);
/* real part of A7 */
			a13 = (*p_x++);
/* imaginary part of A7 */
			a14 = (*p_x++);
/* real part of A8 */
			a15 = (*p_x++);
/* imaginary part of A8 */
			a16 = *p_x;

/*
 * All 16 values are read. But we've read only 8 complex
 * elements.  Now start STORES, then continue...
 */

			(*p_z++) = a1;
/* A1 */
			(*p_z++) = a2;
			(*p_z++) = a9;
/* A5 */
			(*p_z++) = a10;

			(*p_z1++) = a3;
/* A2 */
			(*p_z1++) = a4;
			(*p_z1++) = a11;
/* A6 */
			(*p_z1++) = a12;

			(*p_z2++) = a5;
/* A3 */
			(*p_z2++) = a6;
			(*p_z2++) = a13;
/* A7 */
			(*p_z2++) = a14;

			(*p_z3++) = a7;
/* A4 */
			(*p_z3++) = a8;
			(*p_z3++) = a15;
/* A8 */
			(*p_z3++) = a16;

/* Read the next portion again. */

			p_x = &x[inv_i + j + (n << 2)];

/* real part of A9 */
			a1 = (*p_x++);
/* imaginary part of A9 */
			a2 = (*p_x++);
/* real part of A10 */
			a3 = (*p_x++);
/* imaginary part of A10 */
			a4 = (*p_x++);
/* real part of A11 */
			a5 = (*p_x++);
/* imaginary part of A11 */
			a6 = (*p_x++);
/* real part of A12 */
			a7 = (*p_x++);
/* imaginary part of A12 */
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1) + (n << 2)];

/* real part of A13 */
			a9 = (*p_x++);
/* imaginary part of A13 */
			a10 = (*p_x++);
/* real part of A14 */
			a11 = (*p_x++);
/* imaginary part of A14 */
			a12 = (*p_x++);
/* real part of A15 */
			a13 = (*p_x++);
/* imaginary part of A15 */
			a14 = (*p_x++);
/* real part of A16 */
			a15 = (*p_x++);
/* imaginary part of A16 */
			a16 = *p_x;

/* Continue and finish the last STORES. */

			(*p_z++) = a1;
/* A9 */
			(*p_z++) = a2;
			(*p_z++) = a9;
/* A13 */
			(*p_z++) = a10;

			(*p_z1++) = a3;
/* A10 */
			(*p_z1++) = a4;
			(*p_z1++) = a11;
/* A14 */
			(*p_z1++) = a12;

			(*p_z2++) = a5;
/* A11 */
			(*p_z2++) = a6;
			(*p_z2++) = a13;
/* A15 */
			(*p_z2++) = a14;

			(*p_z3++) = a7;
/* A12 */
			(*p_z3++) = a8;
			(*p_z3++) = a15;
/* A16 */
			(*p_z3++) = a16;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix by 4*4 elements with 4-byte aligned source address.
 *  Destination addresses is not important. Read by 32 bits, write
 *  by 16 bits.
 *
 *  n & 1 == 0 !!!
 *
 */

static void
mlib_transpose16_4_al4x(
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s16 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	mlib_s16 *p_z;
	mlib_s32 *dsrc;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 2, step_m = ds << 2,
		shift =	n >> 1;

	for (i = 0; i <= m - 4; i += 4) {
		for (j = 0; j <= n - 4; j += 4) {
			dsrc = (mlib_s32 *)&x[inv_i + j];

#ifndef _LITTLE_ENDIAN

			a3 = *dsrc;
			a4 = *(dsrc + 1);
			a1 = (a3) >> 16;
			a2 = (a3);
			a3 = (a4) >> 16;
			a4 &= 0xffff;

			dsrc += shift;
			a7 = *dsrc;
			a8 = *(dsrc + 1);
			a5 = (a7) >> 16;
			a6 = (a7);
			a7 = (a8) >> 16;
			a8 &= 0xffff;

			dsrc += shift;
			a11 = *dsrc;
			a12 = *(dsrc + 1);
			a9 = (a11) >> 16;
			a10 = (a11);
			a11 = (a12) >> 16;
			a12 &= 0xffff;

			dsrc += shift;
			a15 = *dsrc;
			a16 = *(dsrc + 1);
			a13 = (a15) >> 16;
			a14 = (a15);
			a15 = (a16) >> 16;
			a16 &= 0xffff;

#else /* _LITTLE_ENDIAN */

			a3 = *dsrc;
			a4 = *(dsrc + 1);
			a1 = (a3);
			a2 = (a3) >> 16;
			a3 = (a4);
			a4 = (a4) >> 16;

			dsrc += shift;
			a7 = *dsrc;
			a8 = *(dsrc + 1);
			a5 = (a7);
			a6 = (a7) >> 16;
			a7 = (a8);
			a8 = (a8) >> 16;

			dsrc += shift;
			a11 = *dsrc;
			a12 = *(dsrc + 1);
			a9 = (a11);
			a10 = (a11) >> 16;
			a11 = (a12);
			a12 = (a12) >> 16;

			dsrc += shift;
			a15 = *dsrc;
			a16 = *(dsrc + 1);
			a13 = (a15);
			a14 = (a15) >> 16;
			a15 = (a16);
			a16 = (a16) >> 16;

#endif /* _LITTLE_ENDIAN */

			p_z = &z[inv_j + i];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + i + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + i + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + i + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

			inv_j += step_m;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 1 != 0 (common case).
 *
 */

static void
mlib_transpose16_cc(
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s16 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	const mlib_s16 *p_x;
	mlib_s16 *p_z;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 2, step_m = ds << 2;

	for (i = 0; i <= m - 4; i += 4) {
		for (j = 0; j <= n - 4; j += 4) {
			p_x = &x[inv_i + j];
			a1 = (*p_x++);
			a2 = (*p_x++);
			a3 = (*p_x++);
			a4 = *p_x;

			p_x = &x[inv_i + j + n];
			a5 = (*p_x++);
			a6 = (*p_x++);
			a7 = (*p_x++);
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];
			a9 = (*p_x++);
			a10 = (*p_x++);
			a11 = (*p_x++);
			a12 = *p_x;

			p_x = &x[inv_i + j + n + (n << 1)];
			a13 = (*p_x++);
			a14 = (*p_x++);
			a15 = (*p_x++);
			a16 = *p_x;

			p_z = &z[inv_j + i];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + i + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + i + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + i + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

			inv_j += step_m;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process Complex matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 3 != 0 (common case). This
 *  procedure works with Complex matrix elements, so read and write
 *  by 2 elements (real and imaginary parts).
 *
 *  In comments "AI" means the complex value as a whole, as if
 *  the real and imaginary parts were allocated in registers at the
 *  same time.
 */

static void
mlib_transpose16C_cc(
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s16 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
/* pointer to source matrix */
	mlib_s16 *p_x,
/* pointers to destination matrix */
	*p_z, *p_z1, *p_z2, *p_z3;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 3, step_m = ds << 3;

	for (i = 0; i <= m * 2 - 8; i += 8) {
		for (j = 0; j <= n * 2 - 8; j += 8) {
			p_z = &z[inv_j + i];
			p_z1 = &z[inv_j + i + (ds << 1)];
			p_z2 = &z[inv_j + i + (ds << 2)];
			p_z3 = &z[inv_j + i + (ds << 1) + (ds << 2)];

			inv_j += step_m;

/* First Read 16 values to allocate them in mlib_s32 registers. */

			p_x = &x[inv_i + j];

/* real part of A1 */
			a1 = (*p_x++);
/* imaginary part of A1 */
			a2 = (*p_x++);
/* real part of A2 */
			a3 = (*p_x++);
/* imaginary part of A2 */
			a4 = (*p_x++);
/* real part of A3 */
			a5 = (*p_x++);
/* imaginary part of A3 */
			a6 = (*p_x++);
/* real part of A4 */
			a7 = (*p_x++);
/* imaginary part of A4 */
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];

/* real part of A5 */
			a9 = (*p_x++);
/* imaginary part of A5 */
			a10 = (*p_x++);
/* real part of A6 */
			a11 = (*p_x++);
/* imaginary part of A6 */
			a12 = (*p_x++);
/* real part of A7 */
			a13 = (*p_x++);
/* imaginary part of A7 */
			a14 = (*p_x++);
/* real part of A8 */
			a15 = (*p_x++);
/* imaginary part of A8 */
			a16 = *p_x;

/*
 * All 16 values are read. But we've read only 8 complex
 * elements.  Now start STORES, then continue...
 */

			(*p_z++) = a1;
/* A1 */
			(*p_z++) = a2;
			(*p_z++) = a9;
/* A5 */
			(*p_z++) = a10;

			(*p_z1++) = a3;
/* A2 */
			(*p_z1++) = a4;
			(*p_z1++) = a11;
/* A6 */
			(*p_z1++) = a12;

			(*p_z2++) = a5;
/* A3 */
			(*p_z2++) = a6;
			(*p_z2++) = a13;
/* A7 */
			(*p_z2++) = a14;

			(*p_z3++) = a7;
/* A4 */
			(*p_z3++) = a8;
			(*p_z3++) = a15;
/* A8 */
			(*p_z3++) = a16;

/* Read the next portion again. */

			p_x = &x[inv_i + j + (n << 2)];

/* real part of A9 */
			a1 = (*p_x++);
/* imaginary part of A9 */
			a2 = (*p_x++);
/* real part of A10 */
			a3 = (*p_x++);
/* imaginary part of A10 */
			a4 = (*p_x++);
/* real part of A11 */
			a5 = (*p_x++);
/* imaginary part of A11 */
			a6 = (*p_x++);
/* real part of A12 */
			a7 = (*p_x++);
/* imaginary part of A12 */
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1) + (n << 2)];

/* real part of A13 */
			a9 = (*p_x++);
/* imaginary part of A13 */
			a10 = (*p_x++);
/* real part of A14 */
			a11 = (*p_x++);
/* imaginary part of A14 */
			a12 = (*p_x++);
/* real part of A15 */
			a13 = (*p_x++);
/* imaginary part of A15 */
			a14 = (*p_x++);
/* real part of A16 */
			a15 = (*p_x++);
/* imaginary part of A16 */
			a16 = *p_x;

/* Continue and finish the last STORES. */

			(*p_z++) = a1;
/* A9 */
			(*p_z++) = a2;
			(*p_z++) = a9;
/* A13 */
			(*p_z++) = a10;

			(*p_z1++) = a3;
/* A10 */
			(*p_z1++) = a4;
			(*p_z1++) = a11;
/* A14 */
			(*p_z1++) = a12;

			(*p_z2++) = a5;
/* A11 */
			(*p_z2++) = a6;
			(*p_z2++) = a13;
/* A15 */
			(*p_z2++) = a14;

			(*p_z3++) = a7;
/* A12 */
			(*p_z3++) = a8;
			(*p_z3++) = a15;
/* A16 */
			(*p_z3++) = a16;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 1 != 0 (common case).
 *  Read and write by 32 bits.
 *
 */

static void
mlib_transpose32_cc(
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	const mlib_s32 *p_x;
	mlib_s32 *p_z;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 2, step_m = ds << 2;

	for (i = 0; i <= m - 4; i += 4) {
		for (j = 0; j <= n - 4; j += 4) {
			p_x = &x[inv_i + j];
			a1 = (*p_x++);
			a2 = (*p_x++);
			a3 = (*p_x++);
			a4 = *p_x;

			p_x = &x[inv_i + j + n];
			a5 = (*p_x++);
			a6 = (*p_x++);
			a7 = (*p_x++);
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];
			a9 = (*p_x++);
			a10 = (*p_x++);
			a11 = (*p_x++);
			a12 = *p_x;

			p_x = &x[inv_i + j + n + (n << 1)];
			a13 = (*p_x++);
			a14 = (*p_x++);
			a15 = (*p_x++);
			a16 = *p_x;

			p_z = &z[inv_j + i];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + i + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + i + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + i + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

			inv_j += step_m;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix_32 when output vector is the same as input and
 *  is arbitrary aligned. Reading and writing by 32 bits.
 *  Function operates by 4*4 elements.
 *
 */

static void
mlib_trans32_diag_4_alxx(
	mlib_s32 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	TYPE_32BIT d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	mlib_s32 *p_x, *p_x1, *p_x2, *p_x3, *p_z, *p_z1, *p_z2, *p_z3;
	TYPE_32BIT *fdst, *fsrc;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2;

	for (i = 0; i < (nm & ~3); i += 4) {

/*
 * First, process 6 elements xz[i, i + 1], xz[i, i + 2], xz[i, i + 3],
 * xz[i + 1, i + 2], xz[i + 1, i + 3] and xz[i + 2, i + 3] which are near the
 * main diagonal at a distance less then 4 elements.
 */

		a1 = *(p_z1 = &INDVAR(xz, i + 1, i, ds));
		a2 = *(p_z2 = &INDVAR(xz, i + 2, i, ds));
		a3 = *(p_z3 = &INDVAR(xz, i + 3, i, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i, i + 1, ds));
		*p_z2 = *(p_x2 = p_x1 + 1);
		*p_z3 = *(p_x3 = p_x1 + 2);
		*p_x1 = a1;
		*p_x2 = a2;
		*p_x3 = a3;

		a1 = *(p_z1 = &INDVAR(xz, i + 2, i + 1, ds));
		a2 = *(p_z2 = &INDVAR(xz, i + 3, i + 1, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i + 1, i + 2, ds));
		*p_z2 = *(p_x2 = p_x1 + 1);
		*p_x1 = a1;
		*p_x2 = a2;

		a1 = *(p_z1 = &INDVAR(xz, i + 3, i + 2, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i + 2, i + 3, ds));
		*p_x1 = a1;

/* Then we process by loop other elements xz[i, i + 4], xz[i, i + 5] ... */

		for (j = i + 4, inv_j = j * ds + i; j < (nm & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on i - reg. */

			p_x = &xz[inv_i + j];
			a1 = *p_x;
			a2 = *(p_x + 1);
			a3 = *(p_x + 2);
			a4 = *(p_x + 3);

			p_x += ds;
			a5 = *p_x;
			a6 = *(p_x + 1);
			a7 = *(p_x + 2);
			a8 = *(p_x + 3);

			p_x += ds;
			a9 = *p_x;
			a10 = *(p_x + 1);
			a11 = *(p_x + 2);
			a12 = *(p_x + 3);

			p_x += ds;
			a13 = *p_x;
			a14 = *(p_x + 1);
			a15 = *(p_x + 2);
			a16 = *(p_x + 3);

/* Now read symmetrical elements and place them on f - registers. */

			p_z = &xz[inv_j];
			fsrc = (TYPE_32BIT *) p_z;
			d1 = (*fsrc++);
			d5 = (*fsrc++);
			d9 = (*fsrc++);
			d13 = *fsrc;

			p_z = &xz[inv_j + ds];
			fsrc = (TYPE_32BIT *) p_z;
			d2 = (*fsrc++);
			d6 = (*fsrc++);
			d10 = (*fsrc++);
			d14 = *fsrc;

			p_z = &xz[inv_j + (ds << 1)];
			fsrc = (TYPE_32BIT *) p_z;
			d3 = (*fsrc++);
			d7 = (*fsrc++);
			d11 = (*fsrc++);
			d15 = *fsrc;

			p_z = &xz[inv_j + ds + (ds << 1)];
			fsrc = (TYPE_32BIT *) p_z;
			d4 = (*fsrc++);
			d8 = (*fsrc++);
			d12 = (*fsrc++);
			d16 = *fsrc;

/* Then we store elements to symmetrical  part. */

			p_z = &xz[inv_j];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &xz[inv_j + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &xz[inv_j + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &xz[inv_j + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region. Writing in source format.
 */

			p_x = &xz[inv_i + j];
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d1;
			(*fdst++) = d2;
			(*fdst++) = d3;
			*fdst = d4;

			p_x += ds;
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d5;
			(*fdst++) = d6;
			(*fdst++) = d7;
			*fdst = d8;

			p_x += ds;
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d9;
			(*fdst++) = d10;
			(*fdst++) = d11;
			*fdst = d12;

			p_x += ds;
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d13;
			(*fdst++) = d14;
			(*fdst++) = d15;
			*fdst = d16;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

static void
mlib_trans32_diag_4_alxx_s(
	mlib_s32 *z,
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	TYPE_32BIT d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	mlib_s32 *p_x, *p_z;
	TYPE_32BIT *fdst, *fsrc;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2;

	for (i = 0; i < (m & ~3); i += 4) {
		for (j = 0, inv_j = j * ds + i; j < (n & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on i - reg. */

			p_x = &x[inv_i + j];
			a1 = *p_x;
			a2 = *(p_x + 1);
			a3 = *(p_x + 2);
			a4 = *(p_x + 3);

			p_x += ds;
			a5 = *p_x;
			a6 = *(p_x + 1);
			a7 = *(p_x + 2);
			a8 = *(p_x + 3);

			p_x += ds;
			a9 = *p_x;
			a10 = *(p_x + 1);
			a11 = *(p_x + 2);
			a12 = *(p_x + 3);

			p_x += ds;
			a13 = *p_x;
			a14 = *(p_x + 1);
			a15 = *(p_x + 2);
			a16 = *(p_x + 3);

/* Now read symmetrical elements and place them on f - registers. */

			p_z = &z[inv_j];
			fsrc = (TYPE_32BIT *) p_z;
			d1 = (*fsrc++);
			d5 = (*fsrc++);
			d9 = (*fsrc++);
			d13 = *fsrc;

			p_z = &z[inv_j + ds];
			fsrc = (TYPE_32BIT *) p_z;
			d2 = (*fsrc++);
			d6 = (*fsrc++);
			d10 = (*fsrc++);
			d14 = *fsrc;

			p_z = &z[inv_j + (ds << 1)];
			fsrc = (TYPE_32BIT *) p_z;
			d3 = (*fsrc++);
			d7 = (*fsrc++);
			d11 = (*fsrc++);
			d15 = *fsrc;

			p_z = &z[inv_j + ds + (ds << 1)];
			fsrc = (TYPE_32BIT *) p_z;
			d4 = (*fsrc++);
			d8 = (*fsrc++);
			d12 = (*fsrc++);
			d16 = *fsrc;

/* Then we store elements to symmetrical  part. */

			p_z = &z[inv_j];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region. Writing in source format.
 */

			p_x = &x[inv_i + j];
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d1;
			(*fdst++) = d2;
			(*fdst++) = d3;
			*fdst = d4;

			p_x += ds;
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d5;
			(*fdst++) = d6;
			(*fdst++) = d7;
			*fdst = d8;

			p_x += ds;
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d9;
			(*fdst++) = d10;
			(*fdst++) = d11;
			*fdst = d12;

			p_x += ds;
			fdst = (TYPE_32BIT *) p_x;
			(*fdst++) = d13;
			(*fdst++) = d14;
			(*fdst++) = d15;
			*fdst = d16;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

/*
 *
 *  Process matrix_32C by 4*4 elements with 8-byte aligned source
 *  addresses and (m&1)==0 && (n&1) == 0 when input and output vectors
 *  are the same.
 *  Read and write by 64 bits.
 *
 *  This is a complementary function for Transpose_S32C
 *  (8-byte aligned case).
 *  Cause it is used for complex matrix, ((m&1)==0 && (n&1) == 0)
 *  condition is always TRUE .
 *
 */

static void
mlib_trans64_diag_4(
	TYPE_64BIT * xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	TYPE_64BIT a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	TYPE_64BIT d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	TYPE_64BIT *p_x, *p_x1, *p_x2, *p_x3, *p_z, *p_z1, *p_z2, *p_z3;
	TYPE_64BIT *dst, *src;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2;

	for (i = 0; i < (nm & ~3); i += 4) {

/*
 * First, process 6 elements xz[i, i + 1], xz[i, i + 2], xz[i, i + 3],
 * xz[i + 1, i + 2], xz[i + 1, i + 3] and xz[i + 2, i + 3] which are near the
 * main diagonal at a distance less then 4 elements.
 */

		a1 = *(p_z1 = &INDVAR(xz, i + 1, i, ds));
		a2 = *(p_z2 = &INDVAR(xz, i + 2, i, ds));
		a3 = *(p_z3 = &INDVAR(xz, i + 3, i, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i, i + 1, ds));
		*p_z2 = *(p_x2 = p_x1 + 1);
		*p_z3 = *(p_x3 = p_x1 + 2);
		*p_x1 = a1;
		*p_x2 = a2;
		*p_x3 = a3;

		a1 = *(p_z1 = &INDVAR(xz, i + 2, i + 1, ds));
		a2 = *(p_z2 = &INDVAR(xz, i + 3, i + 1, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i + 1, i + 2, ds));
		*p_z2 = *(p_x2 = p_x1 + 1);
		*p_x1 = a1;
		*p_x2 = a2;

		a1 = *(p_z1 = &INDVAR(xz, i + 3, i + 2, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i + 2, i + 3, ds));
		*p_x1 = a1;

/* Then we process by loop other elements xz[i, i + 4], xz[i, i + 5] ... */

		for (j = i + 4, inv_j = j * ds + i; j < (nm & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on f - reg. */

			p_x = &xz[inv_i + j];
			a1 = *p_x;
			a2 = *(p_x + 1);
			a3 = *(p_x + 2);
			a4 = *(p_x + 3);

			p_x += ds;
			a5 = *p_x;
			a6 = *(p_x + 1);
			a7 = *(p_x + 2);
			a8 = *(p_x + 3);

			p_x += ds;
			a9 = *p_x;
			a10 = *(p_x + 1);
			a11 = *(p_x + 2);
			a12 = *(p_x + 3);

			p_x += ds;
			a13 = *p_x;
			a14 = *(p_x + 1);
			a15 = *(p_x + 2);
			a16 = *(p_x + 3);

/* Now read symmetrical elements and place them on f - registers. */

			src = &xz[inv_j];
			d1 = (*src++);
			d5 = (*src++);
			d9 = (*src++);
			d13 = *src;

			src = &xz[inv_j + ds];
			d2 = (*src++);
			d6 = (*src++);
			d10 = (*src++);
			d14 = *src;

			src = &xz[inv_j + (ds << 1)];
			d3 = (*src++);
			d7 = (*src++);
			d11 = (*src++);
			d15 = *src;

			src = &xz[inv_j + ds + (ds << 1)];
			d4 = (*src++);
			d8 = (*src++);
			d12 = (*src++);
			d16 = *src;

/* Then we store elements to symmetrical  part. */

			p_z = &xz[inv_j];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &xz[inv_j + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &xz[inv_j + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &xz[inv_j + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region. Writing in source format.
 */

			p_x = &xz[inv_i + j];
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d1;
			(*dst++) = d2;
			(*dst++) = d3;
			*dst = d4;

			p_x += ds;
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d5;
			(*dst++) = d6;
			(*dst++) = d7;
			*dst = d8;

			p_x += ds;
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d9;
			(*dst++) = d10;
			(*dst++) = d11;
			*dst = d12;

			p_x += ds;
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d13;
			(*dst++) = d14;
			(*dst++) = d15;
			*dst = d16;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

static void
mlib_trans64_diag_4_s(
	TYPE_64BIT * z,
	TYPE_64BIT * x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	TYPE_64BIT a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	TYPE_64BIT d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	TYPE_64BIT *p_x, *p_z;
	TYPE_64BIT *dst, *src;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2;

	for (i = 0; i < (m & ~3); i += 4) {
		for (j = 0, inv_j = j * ds + i; j < (n & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on f - reg. */

			p_x = &x[inv_i + j];
			a1 = *p_x;
			a2 = *(p_x + 1);
			a3 = *(p_x + 2);
			a4 = *(p_x + 3);

			p_x += ds;
			a5 = *p_x;
			a6 = *(p_x + 1);
			a7 = *(p_x + 2);
			a8 = *(p_x + 3);

			p_x += ds;
			a9 = *p_x;
			a10 = *(p_x + 1);
			a11 = *(p_x + 2);
			a12 = *(p_x + 3);

			p_x += ds;
			a13 = *p_x;
			a14 = *(p_x + 1);
			a15 = *(p_x + 2);
			a16 = *(p_x + 3);

/* Now read symmetrical elements and place them on f - registers. */

			src = &z[inv_j];
			d1 = (*src++);
			d5 = (*src++);
			d9 = (*src++);
			d13 = *src;

			src = &z[inv_j + ds];
			d2 = (*src++);
			d6 = (*src++);
			d10 = (*src++);
			d14 = *src;

			src = &z[inv_j + (ds << 1)];
			d3 = (*src++);
			d7 = (*src++);
			d11 = (*src++);
			d15 = *src;

			src = &z[inv_j + ds + (ds << 1)];
			d4 = (*src++);
			d8 = (*src++);
			d12 = (*src++);
			d16 = *src;

/* Then we store elements to symmetrical  part. */

			p_z = &z[inv_j];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + ds + (ds << 1)];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region. Writing in source format.
 */

			p_x = &x[inv_i + j];
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d1;
			(*dst++) = d2;
			(*dst++) = d3;
			*dst = d4;

			p_x += ds;
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d5;
			(*dst++) = d6;
			(*dst++) = d7;
			*dst = d8;

			p_x += ds;
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d9;
			(*dst++) = d10;
			(*dst++) = d11;
			*dst = d12;

			p_x += ds;
			dst = (TYPE_64BIT *) p_x;
			(*dst++) = d13;
			(*dst++) = d14;
			(*dst++) = d15;
			*dst = d16;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}


/*
 *
 *  Complementary procedure for  mlib_MatrixTranspose_S32C
 *  implementation. It operates with complex (mlib_s32 Re, mlib_s32 Im)
 *  element as if it is the single mlib_d64 object.
 *  This function is called only if xz&7 == 0 (8-byte aligned case).
 *  Read and write by 64 bits.
 *
 */

static mlib_status
mlib_MatrixTranspose_D64(
	void *xz,
	mlib_s32 nm)
{
	mlib_s32 i, j;
	TYPE_64BIT *p_x, *p_z;
	mlib_s32 n_even_4 = nm & ~3;
	TYPE_64BIT *x = xz, *z = xz;
	TYPE_64BIT a;

	if (nm <= 0)
		return (MLIB_FAILURE);

	if (nm < 8) {
		TRANS_DIAG(TYPE_64BIT)
	}

	mlib_trans64_diag_4((TYPE_64BIT *) xz, nm, nm);

	for (j = n_even_4; j < nm; j++) {
		p_z = &INDVAR(z, j, 0, nm);
		for (i = 0; i < j; i++) {
			a = *p_z;
			(*p_z++) = *(p_x = &INDVAR(x, i, j, nm));
			*p_x = a;
		}
	}

	return (MLIB_SUCCESS);
}

static mlib_status
mlib_MatrixTranspose_S_D64(
	TYPE_64BIT *z,
	TYPE_64BIT *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j, nm = m;
	TYPE_64BIT *p_x, *p_z, *xz = x;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	TYPE_64BIT a;

	if (z == x) {
		if (m < 8) {
			TRANSS_DIAG(TYPE_64BIT)
		}

		mlib_trans64_diag_4((TYPE_64BIT *) xz, nm, ds);

		for (j = n_even_4; j < nm; j++) {
			p_z = &INDVAR(z, j, 0, ds);
			for (i = 0; i < j; i++) {
				a = *p_z;
				(*p_z++) = *(p_x = &INDVAR(x, i, j, ds));
				*p_x = a;
			}
		}
		return (MLIB_SUCCESS);
	} else {
		if (m < 4 || n < 4) {
			TRANSS_DIAG2(TYPE_64BIT)
		}

		mlib_trans64_diag_4_s(z, x, m, n, ds);

		for (j = m_even_4; j < m; j++) {
			p_x = &INDVAR(x, j, 0, ds);
			for (i = 0; i < n; i++) {
				a = *p_x;
				(*p_x++) = *(p_z = &INDVAR(z, i, j, ds));
				*p_z = a;
			}
		}

		if (n_even_4 < n) {
			for (j = 0; j < m_even_4; j++) {
				p_x = &INDVAR(x, j, n_even_4, ds);
				for (i = n_even_4; i < n; i++) {
					a = *p_x;
					(*p_x++) = *(p_z = &INDVAR(z, i, j,
						ds));
					*p_z = a;
				}
			}
		}

		return (MLIB_SUCCESS);
	}
}

/*
 *
 *  Process Complex matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 3 != 0 (common case). This
 *  procedure works with Complex matrix elements, so read and write
 *  by 2 elements (real and imaginary parts).
 *
 *  In comments "AI" means the complex value as a whole, as if
 *  the real and imaginary parts were allocated in registers at the
 *  same time.
 */

static void
mlib_transpose32C_cc(
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 *z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
/* pointer to source matrix */
	mlib_s32 *p_x,
/* pointers to destination matrix */
	*p_z, *p_z1, *p_z2, *p_z3;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 3, step_m = ds << 3;

	for (i = 0; i <= m * 2 - 8; i += 8) {
		for (j = 0; j <= n * 2 - 8; j += 8) {
			p_z = &z[inv_j + i];
			p_z1 = &z[inv_j + i + (ds << 1)];
			p_z2 = &z[inv_j + i + (ds << 2)];
			p_z3 = &z[inv_j + i + (ds << 1) + (ds << 2)];

			inv_j += step_m;

/* First Read 16 values to allocate them in mlib_s32 registers. */

			p_x = &x[inv_i + j];

/* real part of A1 */
			a1 = (*p_x++);
/* imaginary part of A1 */
			a2 = (*p_x++);
/* real part of A2 */
			a3 = (*p_x++);
/* imaginary part of A2 */
			a4 = (*p_x++);
/* real part of A3 */
			a5 = (*p_x++);
/* imaginary part of A3 */
			a6 = (*p_x++);
/* real part of A4 */
			a7 = (*p_x++);
/* imaginary part of A4 */
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];

/* real part of A5 */
			a9 = (*p_x++);
/* imaginary part of A5 */
			a10 = (*p_x++);
/* real part of A6 */
			a11 = (*p_x++);
/* imaginary part of A6 */
			a12 = (*p_x++);
/* real part of A7 */
			a13 = (*p_x++);
/* imaginary part of A7 */
			a14 = (*p_x++);
/* real part of A8 */
			a15 = (*p_x++);
/* imaginary part of A8 */
			a16 = *p_x;

/*
 * All 16 values are read. But we've read only 8 complex
 * elements.  Now start STORES, then continue...
 */

			(*p_z++) = a1;
/* A1 */
			(*p_z++) = a2;
			(*p_z++) = a9;
/* A5 */
			(*p_z++) = a10;

			(*p_z1++) = a3;
/* A2 */
			(*p_z1++) = a4;
			(*p_z1++) = a11;
/* A6 */
			(*p_z1++) = a12;

			(*p_z2++) = a5;
/* A3 */
			(*p_z2++) = a6;
			(*p_z2++) = a13;
/* A7 */
			(*p_z2++) = a14;

			(*p_z3++) = a7;
/* A4 */
			(*p_z3++) = a8;
			(*p_z3++) = a15;
/* A8 */
			(*p_z3++) = a16;

/* Read the next portion again. */

			p_x = &x[inv_i + j + (n << 2)];

/* real part of A9 */
			a1 = (*p_x++);
/* imaginary part of A9 */
			a2 = (*p_x++);
/* real part of A10 */
			a3 = (*p_x++);
/* imaginary part of A10 */
			a4 = (*p_x++);
/* real part of A11 */
			a5 = (*p_x++);
/* imaginary part of A11 */
			a6 = (*p_x++);
/* real part of A12 */
			a7 = (*p_x++);
/* imaginary part of A12 */
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1) + (n << 2)];

/* real part of A13 */
			a9 = (*p_x++);
/* imaginary part of A13 */
			a10 = (*p_x++);
/* real part of A14 */
			a11 = (*p_x++);
/* imaginary part of A14 */
			a12 = (*p_x++);
/* real part of A15 */
			a13 = (*p_x++);
/* imaginary part of A15 */
			a14 = (*p_x++);
/* real part of A16 */
			a15 = (*p_x++);
/* imaginary part of A16 */
			a16 = *p_x;

/* Continue and finish the last STORES. */

			(*p_z++) = a1;
/* A9 */
			(*p_z++) = a2;
			(*p_z++) = a9;
/* A13 */
			(*p_z++) = a10;

			(*p_z1++) = a3;
/* A10 */
			(*p_z1++) = a4;
			(*p_z1++) = a11;
/* A14 */
			(*p_z1++) = a12;

			(*p_z2++) = a5;
/* A11 */
			(*p_z2++) = a6;
			(*p_z2++) = a13;
/* A15 */
			(*p_z2++) = a14;

			(*p_z3++) = a7;
/* A12 */
			(*p_z3++) = a8;
			(*p_z3++) = a15;
/* A16 */
			(*p_z3++) = a16;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Process matrix by 4*4 elements with arbitrary aligned source
 *  and destination addresses or n & 1 != 0 (common case).
 *  Read and write by 32 bits.
 *
 *  This is a complimentary function for Transpose_S32C_S32C (8-byte
 *  aligned case).
 *
 */

static void
mlib_transpose64_cc(
	const TYPE_64BIT * x,
	mlib_s32 m,
	mlib_s32 n,
	TYPE_64BIT * z,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	TYPE_64BIT a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	const TYPE_64BIT *p_x;
	TYPE_64BIT *p_z;
	mlib_s32 inv_i = 0, inv_j = 0, step_n = n << 2, step_m = ds << 2;

	for (i = 0; i <= m - 4; i += 4) {
		for (j = 0; j <= n - 4; j += 4) {
			p_x = &x[inv_i + j];
			a1 = (*p_x++);
			a2 = (*p_x++);
			a3 = (*p_x++);
			a4 = *p_x;

			p_x = &x[inv_i + j + n];
			a5 = (*p_x++);
			a6 = (*p_x++);
			a7 = (*p_x++);
			a8 = *p_x;

			p_x = &x[inv_i + j + (n << 1)];
			a9 = (*p_x++);
			a10 = (*p_x++);
			a11 = (*p_x++);
			a12 = *p_x;

			p_x = &x[inv_i + j + n + (n << 1)];
			a13 = (*p_x++);
			a14 = (*p_x++);
			a15 = (*p_x++);
			a16 = *p_x;

			p_z = &z[inv_j + i];
			(*p_z++) = a1;
			(*p_z++) = a5;
			(*p_z++) = a9;
			*p_z = a13;

			p_z = &z[inv_j + i + ds];
			(*p_z++) = a2;
			(*p_z++) = a6;
			(*p_z++) = a10;
			*p_z = a14;

			p_z = &z[inv_j + i + (ds << 1)];
			(*p_z++) = a3;
			(*p_z++) = a7;
			(*p_z++) = a11;
			*p_z = a15;

			p_z = &z[inv_j + i + (ds << 1) + ds];
			(*p_z++) = a4;
			(*p_z++) = a8;
			(*p_z++) = a12;
			*p_z = a16;

			inv_j += step_m;
		}

		inv_i += step_n;
		inv_j = 0;
	}
}

/*
 *
 *  Complementary procedure for  mlib_MatrixTranspose_S32C_S32C
 *  implementation.
 *  Read and write by 64 bits.
 *
 */

static mlib_status
mlib_MatrixTranspose_D64_D64(
	void *zz,
	const void *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	TYPE_64BIT *p_z;
	const TYPE_64BIT *p_x;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	const TYPE_64BIT *x = xx;
	TYPE_64BIT *z = zz;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	mlib_transpose64_cc(x, m, n, z, ds);

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++)
			(*p_z++) = INDVAR(x, i, j, n);
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR(x, i, 0, n);
		for (j = 0; j < n; j++)
			INDVAR(z, j, i, ds) = (*p_x++);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixTranspose_S_U8_U8(
	mlib_u8 *zz,
	const mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	const mlib_u8 *p_x;
	mlib_u8 *p_z;
	mlib_s32 n_rest_4 = n & 3, n_even_4 = n & ~3;
	mlib_s32 m_rest_4 = m & 3, m_even_4 = m & ~3;
	const mlib_u8 *x = xx;
	mlib_u8 *z = zz;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	if (n < 4 || m < 4) {
		TRANS;
	}

	if ((!((mlib_addr)x & 3)) && (!n_rest_4) && (!m_rest_4)) {

/* Source matrix is 32-bit aligned. Read by 32 bits but write by 8. */

		mlib_transpose8_4_al4x((mlib_s8 *)x, m, n, (mlib_s8 *)z, ds);
	} else {

/* Common case. */

		mlib_transpose8_cc((mlib_s8 *)x, m, n, (mlib_s8 *)z, ds);
	}

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++)
			(*p_z++) = INDVAR(x, i, j, n);
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR(x, i, 0, n);
		for (j = 0; j < n; j++)
			INDVAR(z, j, i, ds) = (*p_x++);
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_MatrixTranspose_U8_U8(
	mlib_u8 *zz,
	const mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_U8_U8(zz,
		xx, m, n, m);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S8_S8(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_U8_U8((mlib_u8 *)zz,
		(mlib_u8 *)xx, m, n, m);
}

/* *********************************************************** */

mlib_status
mlib_MatrixTranspose_S_S16_S16(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	const mlib_s16 *p_x;
	mlib_s16 *p_z;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	const mlib_s16 *x = xx;
	mlib_s16 *z = zz;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	if (n < 4 || m < 4) {
		TRANS;
	}

	if ((!((mlib_addr)x & 3)) && (!(n & 1)) && (!(m & 1))) {

/* Source matrix is 32-bit aligned. Read by 32 bits but write by 16. */

		mlib_transpose16_4_al4x(x, m, n, z, ds);
	} else {

/* Common case. */

		mlib_transpose16_cc(x, m, n, z, ds);
	}

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++)
			(*p_z++) = INDVAR(x, i, j, n);
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR(x, i, 0, n);
		for (j = 0; j < n; j++)
			INDVAR(z, j, i, ds) = (*p_x++);
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_MatrixTranspose_S16_S16(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_S16_S16(zz,
		xx, m, n, m);
}

/* *********************************************************** */

mlib_status
mlib_MatrixTranspose_S_S32_S32(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	const mlib_s32 *p_x;
	mlib_s32 *p_z;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	const mlib_s32 *x = xx;
	mlib_s32 *z = zz;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	if (n < 4 || m < 4) {
		TRANS;
	}

	mlib_transpose32_cc(x, m, n, z, ds);

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++)
			(*p_z++) = INDVAR(x, i, j, n);
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR(x, i, 0, n);
		for (j = 0; j < n; j++)
			INDVAR(z, j, i, ds) = (*p_x++);
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_MatrixTranspose_S32_S32(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_S32_S32(zz,
		xx, m, n, m);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_U8(
	mlib_u8 *xz,
	mlib_s32 nm)
{
	if (nm <= 0)
		return (MLIB_FAILURE);
	TRANS_DIAG(mlib_u8)
}

mlib_status
mlib_MatrixTranspose_S_U8(
	mlib_u8 *z,
	mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);
	if (z == x)
		TRANSS_DIAG(mlib_u8)
	else
		TRANSS_DIAG2(mlib_u8)
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S8(
	mlib_s8 *xz,
	mlib_s32 nm)
{
	return (__mlib_MatrixTranspose_U8((mlib_u8 *)xz, nm));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S16(
	mlib_s16 *xz,
	mlib_s32 nm)
{
	if (nm <= 0)
		return (MLIB_FAILURE);
	TRANS_DIAG(mlib_s16)
}

mlib_status
mlib_MatrixTranspose_S_S16(
	mlib_s16 *z,
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);
	if (z == x)
		TRANSS_DIAG(mlib_s16)
	else
		TRANSS_DIAG2(mlib_s16)
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S32(
	mlib_s32 *xz,
	mlib_s32 nm)
{
	mlib_s32 i, j;
	mlib_s32 *p_x, *p_z;
	mlib_s32 n_even_4 = nm & ~3;
	mlib_s32 *x = xz, *z = xz;
	mlib_s32 a;

	if (nm <= 0)
		return (MLIB_FAILURE);

	if (nm < 8) {
		TRANS_DIAG(mlib_s32)
	}

	mlib_trans32_diag_4_alxx((mlib_s32 *)xz, nm, nm);

	for (j = n_even_4; j < nm; j++) {
		p_z = &INDVAR(z, j, 0, nm);
		for (i = 0; i < j; i++) {
			a = *p_z;
			(*p_z++) = *(p_x = &INDVAR(x, i, j, nm));
			*p_x = a;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_MatrixTranspose_S_S32(
	mlib_s32 *z,
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j, nm = m;
	mlib_s32 *p_x, *p_z;
	mlib_s32 m_even_4 = m & ~3;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 a;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (z == x) {
		if (m < 8) {
			TRANSS_DIAG(mlib_s32)
		}

		mlib_trans32_diag_4_alxx((mlib_s32 *)z, m, ds);

		for (j = n_even_4; j < nm; j++) {
			p_z = &INDVAR(z, j, 0, ds);
			for (i = 0; i < j; i++) {
				a = *p_z;
				(*p_z++) = *(p_x = &INDVAR(x, i, j, ds));
				*p_x = a;
			}
		}
		return (MLIB_SUCCESS);
	} else {
		if (m < 4 || n < 4) {
			TRANSS_DIAG2(mlib_s32)
		}

		mlib_trans32_diag_4_alxx_s(z, x, m, n, ds);

		for (j = m_even_4; j < m; j++) {
			p_x = &INDVAR(x, j, 0, ds);
			for (i = 0; i < n; i++) {
				a = *p_x;
				(*p_x++) = *(p_z = &INDVAR(z, i, j, ds));
				*p_z = a;
			}
		}

		if (n_even_4 < n) {
			for (j = 0; j < m_even_4; j++) {
				p_x = &INDVAR(x, j, n_even_4, ds);
				for (i = n_even_4; i < n; i++) {
					a = *p_x;
					(*p_x++) = *(p_z = &INDVAR(z, i, j,
						ds));
					*p_z = a;
				}
			}
		}

		return (MLIB_SUCCESS);
	}
}

/* Complex Procedures Interface */

/* *********************************************************** */

mlib_status
mlib_MatrixTranspose_S_U8C_U8C(
	mlib_u8 *zz,
	const mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	const mlib_u8 *p_x;
	mlib_u8 *p_z;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	const mlib_u8 *x = xx;
	mlib_u8 *z = zz;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	if (n < 4 || m < 4)
		TRANS_C(mlib_u8);

	if ((!((mlib_addr)x & 1)) && (!((mlib_addr)z & 1)))

		return mlib_MatrixTranspose_S_S16_S16((mlib_s16 *)z,
			(mlib_s16 *)x, m, n, ds);
	else
		mlib_transpose8C_cc((mlib_u8 *)x, m, n, (mlib_u8 *)z, ds);

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR_C_RE(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++) {
			(*p_z++) = INDVAR_C_RE(x, i, j, n);
			(*p_z++) = INDVAR_C_IM(x, i, j, n);
		}
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR_C_RE(x, i, 0, n);
		for (j = 0; j < n; j++) {
			INDVAR_C_RE(z, j, i, ds) = (*p_x++);
			INDVAR_C_IM(z, j, i, ds) = (*p_x++);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_MatrixTranspose_U8C_U8C(
	mlib_u8 *zz,
	const mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_U8C_U8C(zz,
		xx, m, n, m);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S8C_S8C(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_U8C_U8C((mlib_u8 *)zz,
		(mlib_u8 *)xx, m, n, m);
}

/* *********************************************************** */

mlib_status
mlib_MatrixTranspose_S_S16C_S16C(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	const mlib_s16 *p_x;
	mlib_s16 *p_z;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	const mlib_s16 *x = xx;
	mlib_s16 *z = zz;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	if (n < 4 || m < 4)
		TRANS_C(mlib_s16);

	if ((!((mlib_addr)x & 3)) && (!((mlib_addr)z & 3)))

		return mlib_MatrixTranspose_S_S32_S32((mlib_s32 *)z,
			(mlib_s32 *)x, m, n, ds);
	else
		mlib_transpose16C_cc((mlib_s16 *)x, m, n, (mlib_s16 *)z, ds);

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR_C_RE(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++) {
			(*p_z++) = INDVAR_C_RE(x, i, j, n);
			(*p_z++) = INDVAR_C_IM(x, i, j, n);
		}
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR_C_RE(x, i, 0, n);
		for (j = 0; j < n; j++) {
			INDVAR_C_RE(z, j, i, ds) = (*p_x++);
			INDVAR_C_IM(z, j, i, ds) = (*p_x++);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_MatrixTranspose_S16C_S16C(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_S16C_S16C(zz,
		xx, m, n, m);
}

/* *********************************************************** */

mlib_status
mlib_MatrixTranspose_S_S32C_S32C(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_s32 *z = zz;
	const mlib_s32 *x = xx;
	mlib_s32 *p_z;
	const mlib_s32 *p_x;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;

	if ((n <= 0) || (m <= 0))
		return (MLIB_FAILURE);

	if (n < 4 || m < 4)
		TRANS_C(mlib_s32);

	if ((!((mlib_addr)x & 7)) && (!((mlib_addr)z & 7)))
		return (mlib_MatrixTranspose_D64_D64(z, x, m, n, ds));
	else
		mlib_transpose32C_cc((mlib_s32 *)x, m, n, (mlib_s32 *)z, ds);

	for (j = n_even_4; j < n; j++) {
		p_z = &INDVAR_C_RE(z, j, 0, ds);
		for (i = 0; i < m_even_4; i++) {
			(*p_z++) = INDVAR_C_RE(x, i, j, n);
			(*p_z++) = INDVAR_C_IM(x, i, j, n);
		}
	}

	for (i = m_even_4; i < m; i++) {
		p_x = &INDVAR_C_RE(x, i, 0, n);
		for (j = 0; j < n; j++) {
			INDVAR_C_RE(z, j, i, ds) = (*p_x++);
			INDVAR_C_IM(z, j, i, ds) = (*p_x++);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_MatrixTranspose_S32C_S32C(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_MatrixTranspose_S_S32C_S32C(zz,
		xx, m, n, m);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_U8C(
	mlib_u8 *xz,
	mlib_s32 nm)
{
	if (nm <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_addr)xz & 1))

		return (__mlib_MatrixTranspose_S16((mlib_s16 *)xz, nm));
	else {
		TRANS_DIAG_C(mlib_u8);
	}
}

mlib_status
mlib_MatrixTranspose_S_U8C(
	mlib_u8 *z,
	mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_addr)x | (mlib_addr)z) & 1)) {
		return (mlib_MatrixTranspose_S_S16((mlib_s16 *)z, (mlib_s16 *)x,
			m, n, ds));
	} else {
		if (z == x) {
			TRANSS_DIAG_C(mlib_u8);
		} else {
			TRANSS_DIAG2_C(mlib_u8);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S8C(
	mlib_s8 *xz,
	mlib_s32 nm)
{
	return (__mlib_MatrixTranspose_U8C((mlib_u8 *)xz, nm));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S16C(
	mlib_s16 *xz,
	mlib_s32 nm)
{
	if (nm <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_addr)xz & 3))

		return (__mlib_MatrixTranspose_S32((mlib_s32 *)xz, nm));
	else {
		TRANS_DIAG_C(mlib_s16);
	}
}

mlib_status
mlib_MatrixTranspose_S_S16C(
	mlib_s16 *z,
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_addr)x | (mlib_addr)z) & 3)) {
		return (mlib_MatrixTranspose_S_S32((mlib_s32 *)z, (mlib_s32 *)x,
			m, n, ds));
	} else {
		if (z == x) {
			TRANSS_DIAG_C(mlib_s16);
		} else {
			TRANSS_DIAG2_C(mlib_s16);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_S32C(
	mlib_s32 *xz,
	mlib_s32 nm)
{
	if (nm <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_addr)xz & 7))
		return (mlib_MatrixTranspose_D64(xz, nm));
	else {
		TRANS_DIAG_C(mlib_s32);
	}
}

mlib_status
mlib_MatrixTranspose_S_S32C(
	mlib_s32 *z,
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_addr)x | (mlib_addr)z) & 7)) {
		return (mlib_MatrixTranspose_S_D64((TYPE_64BIT *)z,
			(TYPE_64BIT *)x, m, n, ds));
	} else {
		if (z == x) {
			TRANSS_DIAG_C(mlib_s32);
		} else {
			TRANSS_DIAG2_C(mlib_s32);
		}
	}
}

/* *********************************************************** */
