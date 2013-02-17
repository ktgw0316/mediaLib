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

#pragma ident	"@(#)mlib_v_MatrixTranspose_Inp.c	9.6	07/11/05 SMI"

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
 *
 *      mlib_MatrixTranspose_U8  - transposition of 8-bit format element matrix
 *                                (input and output vectors are the same)
 *      mlib_MatrixTranspose_S8  - transposition of 8-bit format element matrix
 *                                (input and output vectors are the same)
 *      mlib_MatrixTranspose_S16 - transposition of 16-bit
 *                                 format element matrix
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
 *      mlib_status mlib_MatrixTranspose_S16(mlib_s16 *xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S16C(mlib_s16 *xz,
 *                                            mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S16C_S16C(mlib_s16 *zz,
 *                                                 const mlib_s16 *xx,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S16_S16(mlib_s16 *zz,
 *                                               const mlib_s16 *xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S32(mlib_s32 *xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S32C(mlib_s32 *xz,
 *                                            mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S32C_S32C(mlib_s32 *zz,
 *                                                 const mlib_s32 *xx,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S32_S32(mlib_s32 *zz,
 *                                               const mlib_s32 *xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S8(mlib_s8 *xz,
 *                                          mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S8C(mlib_s8 *xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_S8C_S8C(mlib_s8 *zz,
 *                                               const mlib_s8 *xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_S8_S8(mlib_s8 *zz,
 *                                             const mlib_s8 *xx,
 *                                             mlib_s32 m,
 *                                             mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_U8(mlib_u8 *xz,
 *                                          mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_U8C(mlib_u8 *xz,
 *                                           mlib_s32 nm);
 *      mlib_status mlib_MatrixTranspose_U8C_U8C(mlib_u8 *zz,
 *                                               const mlib_u8 * xx,
 *                                               mlib_s32 m,
 *                                               mlib_s32 n);
 *      mlib_status mlib_MatrixTranspose_U8_U8(mlib_u8 *zz,
 *                                             const mlib_u8 *xx,
 *                                             mlib_s32 m,
 *                                             mlib_s32 n);
 *
 * ARGUMENTS
 *      xz   pointer to the first element of the input and output matrix.
 *      x    pointer to the first element of the input matrix.
 *           The matrix is in row major order
 *      m    number of rows in the matrix
 *      n    number of columns in the matrix
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      mn   number of rows and columns in the square matrix
 *
 * DESCRIPTION
 *          _
 *      z = x
 *
 * IMPLEMENTATION
 *
 *      If length < 4 elements, use C - version (implementation by loop
 *      in terms of source format without VIS - instructions).
 *
 *      In case of byte elements of input matrix, we process it by 8*8 elements.
 *      Otherwise we work by 4*4 elements. The main idea
 *      of such processing is quite simple: inside each block ("square") we
 *      can read 16 elements into registers successively (i.e. with successive
 *      source address increasing), and then write them into destination
 *      symmetric "square" also successively (with successive destination
 *      address increasing) via using registers in appropriate order.
 *      For instance, reading (x[1, 1]->a1, x[1, 2]->a2, x[1, 3]->a3,
 *      x[1, 4]->a4, x[2, 1]->a5, x[2, 2]->a6, ... x[4, 4]->a16)
 *      we store them as (a1->z[1, 1], a5->z[1, 2], a9->z[1, 3]...).
 *      This trick gives at least 25% speed-up even without using VIS.
 *
 *      For byte format we also take elements by loading of 64 bits,
 *      place them in d64 - registers and make transposing of 8*8-byte block
 *      onto such registers.
 *
 *      For 16-bit format we also take elements by loading of 64 bits,
 *      place them in d64 registers and make transposing of 4*4-short block
 *      onto such registers.
 *
 *      And for 32-bit elements and "good" alignment we read and write
 *      them by 2 words and take/form each element by
 *      vis_read_hi()/vis_write_lo() and vis_freg_pair();
 *
 *      In case of Complex matrix we first try to transpose it in terms
 *      of mlib_d64 format. It is possible only when source and destination
 *      addresses are double-format aligned (i.e. _S8 vector is 16-bit
 *      aligned, _S16 vector is 32-bit  aligned etc.). If it is not so,
 *      we process it by 4*4 elements as in real case. The only difference
 *      is that we read not 16 elements but 2 times by 16 elements per
 *      one iteration (see comments in procedures).
 *
 *      When destination and source addresses are the same (that is in case of
 *      mlib_MatrixTranspose_{MLIB_FORMAT}) we also use analogous algorithm.
 *      But here there is a problem: it is necessary to accumulate in registers
 *      not only 16 reading elements for later storing, but also values
 *      form symmetrical "square" which have to be re-written. For such
 *      purpose we use f registers and fill them by vis_ld_u8/16.
 */

#include <mlib_algebra.h>
#include <mlib_MatrixTranspose.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixTranspose_S16 = __mlib_MatrixTranspose_S16
#pragma weak mlib_MatrixTranspose_S32C = __mlib_MatrixTranspose_S32C
#pragma weak mlib_MatrixTranspose_S8 = __mlib_MatrixTranspose_S8
#pragma weak mlib_MatrixTranspose_S16C = __mlib_MatrixTranspose_S16C
#pragma weak mlib_MatrixTranspose_U8 = __mlib_MatrixTranspose_U8
#pragma weak mlib_MatrixTranspose_S8C = __mlib_MatrixTranspose_S8C
#pragma weak mlib_MatrixTranspose_U8C = __mlib_MatrixTranspose_U8C
#pragma weak mlib_MatrixTranspose_S32 = __mlib_MatrixTranspose_S32

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixTranspose_S16) mlib_MatrixTranspose_S16
	__attribute__((weak, alias("__mlib_MatrixTranspose_S16")));
__typeof__(__mlib_MatrixTranspose_S32C) mlib_MatrixTranspose_S32C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S32C")));
__typeof__(__mlib_MatrixTranspose_S8) mlib_MatrixTranspose_S8
	__attribute__((weak, alias("__mlib_MatrixTranspose_S8")));
__typeof__(__mlib_MatrixTranspose_S8_S8) mlib_MatrixTranspose_S8_S8
	__attribute__((weak, alias("__mlib_MatrixTranspose_S8_S8")));
__typeof__(__mlib_MatrixTranspose_S16C) mlib_MatrixTranspose_S16C
	__attribute__((weak, alias("__mlib_MatrixTranspose_S16C")));
__typeof__(__mlib_MatrixTranspose_U8) mlib_MatrixTranspose_U8
	__attribute__((weak, alias("__mlib_MatrixTranspose_U8")));
__typeof__(__mlib_MatrixTranspose_U8C) mlib_MatrixTranspose_U8C
	__attribute__((weak, alias("__mlib_MatrixTranspose_U8C")));
__typeof__(__mlib_MatrixTranspose_S32) mlib_MatrixTranspose_S32
	__attribute__((weak, alias("__mlib_MatrixTranspose_S32")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	INDVAR(arr, i1, i2, n)	arr[(i1)*n + i2]
#define	INDVAR_C_RE(arr, i1, i2, n)	arr[(i1)*(n << 1)+(i2 << 1)]
#define	INDVAR_C_IM(arr, i1, i2, n)	arr[(i1)*(n << 1)+(i2 << 1)+1]

/* *********************************************************** */

#define	TRANS                                                   \
	mlib_s32 i, j, inv_j;                                   \
	                                                        \
	for (i = 0; i < m; i++)                                 \
	    for (j = 0, inv_j = 0; j < n; j++) {                \
		z[inv_j + i] = (*x++);                          \
		inv_j += m;                                     \
	    }                                                   \
	                                                        \
	return MLIB_SUCCESS

/* Macro for Complex  matrix transpose. */

#define	TRANS_C(form)                                           \
	form *p_z;                                              \
	mlib_s32 i, j, inv_j;                                   \
	                                                        \
	for (i = 0; i < (m << 1); i += 2)                       \
	    for (j = 0, inv_j = 0; j < (n << 1); j += 2) {      \
		*(p_z = &z[inv_j + i]) = (*x++);                \
		*(p_z + 1) = (*x++);                            \
		inv_j += (m << 1);                              \
	    }                                                   \
	                                                        \
	return MLIB_SUCCESS

/*
 * Makes matrix transpose when input and output vectors are the same.
 * This is the processing of elements above main diagonal
 */

#define	TRANS_DIAG(form)                                        \
	form *z = xz, *p_z;                                     \
	form *x = xz, *p_x;                                     \
	mlib_s32 i, j, inv_i = 0;                               \
	form a;                                                 \
	                                                        \
	for (i = 0; i < nm; i++) {                              \
	    j = i + 1;                                          \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[j * nm + i];                               \
	    if ((nm - j) & 1) {                                 \
		a = *(p_z);                                     \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		p_z += nm;                                      \
		j++;                                            \
	    }                                                   \
	    for (; j < nm; j += 2) {                            \
		a = *(p_z);                                     \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		a = *(p_z += nm);                               \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		p_z += nm;                                      \
	    }                                                   \
	    inv_i += nm;                                        \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

#define	TRANSS_DIAG(form)                                       \
	form *p_z;                                              \
	form *p_x;                                              \
	mlib_s32 i, j, inv_i = 0;                               \
	form a;                                                 \
	                                                        \
	for (i = 0; i < nm; i++) {                              \
	    j = i + 1;                                          \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[j * ds + i];                               \
	    if ((nm - j) & 1) {                                 \
		a = *(p_z);                                     \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		p_z += ds;                                      \
		j++;                                            \
	    }                                                   \
	    for (; j < nm; j += 2) {                            \
		a = *(p_z);                                     \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		a = *(p_z += ds);                               \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		p_z += ds;                                      \
	    }                                                   \
	    inv_i += ds;                                        \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

#define	TRANSS_DIAG2(form)                                      \
	form *p_z;                                              \
	form *p_x;                                              \
	mlib_s32 i, j, inv_i = 0;                               \
	form a;                                                 \
	                                                        \
	for (i = 0; i < m; i++) {                               \
	    j = 0;                                              \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[j * ds + i];                               \
	    if ((n - j) & 1) {                                  \
		a = *(p_z);                                     \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		p_z += ds;                                      \
		j++;                                            \
	    }                                                   \
	    for (; j < n; j += 2) {                             \
		a = *(p_z);                                     \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		a = *(p_z += ds);                               \
		*p_z = *p_x;                                    \
		(*p_x++) = a;                                   \
		p_z += ds;                                      \
	    }                                                   \
	    inv_i += ds;                                        \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/*
 * Makes Complex matrix transpose when input and output vectors
 * are the same. This is the processing of elements above
 * main diagonal.
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
	for (i = 0; i < (nm << 1); i += 2) {                    \
	    j = i + 2;                                          \
	    p_x = &x[inv_i + j];                                \
	    p_z = &z[(j * ds) + i];                             \
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

/* *********************************************************** */

#define	TRAN_BLOCK_8                                                \
	m04 = vis_fpmerge(vis_read_hi(p0), vis_read_hi(p4));        \
	m26 = vis_fpmerge(vis_read_hi(p2), vis_read_hi(p6));        \
	m15 = vis_fpmerge(vis_read_hi(p1), vis_read_hi(p5));        \
	m37 = vis_fpmerge(vis_read_hi(p3), vis_read_hi(p7));        \
	m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));    \
	m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));    \
	q0 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q1 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));    \
	m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));    \
	q2 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q3 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	m04 = vis_fpmerge(vis_read_lo(p0), vis_read_lo(p4));        \
	m26 = vis_fpmerge(vis_read_lo(p2), vis_read_lo(p6));        \
	m15 = vis_fpmerge(vis_read_lo(p1), vis_read_lo(p5));        \
	m37 = vis_fpmerge(vis_read_lo(p3), vis_read_lo(p7));        \
	m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));    \
	m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));    \
	q4 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q5 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));    \
	m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));    \
	q6 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q7 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537))

/* *********************************************************** */

#define	TRAN_SECOND_BLOCK_8                                          \
	m04 = vis_fpmerge(vis_read_hi(p00), vis_read_hi(p44));       \
	m26 = vis_fpmerge(vis_read_hi(p22), vis_read_hi(p66));       \
	m15 = vis_fpmerge(vis_read_hi(p11), vis_read_hi(p55));       \
	m37 = vis_fpmerge(vis_read_hi(p33), vis_read_hi(p77));       \
	m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));     \
	m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));     \
	q00 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q11 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));     \
	m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));     \
	q22 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q33 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	m04 = vis_fpmerge(vis_read_lo(p00), vis_read_lo(p44));       \
	m26 = vis_fpmerge(vis_read_lo(p22), vis_read_lo(p66));       \
	m15 = vis_fpmerge(vis_read_lo(p11), vis_read_lo(p55));       \
	m37 = vis_fpmerge(vis_read_lo(p33), vis_read_lo(p77));       \
	m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));     \
	m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));     \
	q44 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q55 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));     \
	m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));     \
	q66 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	q77 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537))

/* *********************************************************** */

#define	TRAN_BLOCK_16                                               \
	m02 = vis_fpmerge(vis_read_hi(p0), vis_read_hi(p2));        \
	m13 = vis_fpmerge(vis_read_hi(p1), vis_read_hi(p3));        \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));    \
	q0 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));    \
	q1 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m02 = vis_fpmerge(vis_read_lo(p0), vis_read_lo(p2));        \
	m13 = vis_fpmerge(vis_read_lo(p1), vis_read_lo(p3));        \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));    \
	q2 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));    \
	q3 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213))

/* *********************************************************** */

#define	TRAN_SECOND_BLOCK_16                                         \
	m02 = vis_fpmerge(vis_read_hi(p00), vis_read_hi(p22));       \
	m13 = vis_fpmerge(vis_read_hi(p11), vis_read_hi(p33));       \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));     \
	q00 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));     \
	q11 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m02 = vis_fpmerge(vis_read_lo(p00), vis_read_lo(p22));       \
	m13 = vis_fpmerge(vis_read_lo(p11), vis_read_lo(p33));       \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));     \
	q22 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));     \
	q33 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213))

/* *********************************************************** */

#define	READ_D_REG(REG)                                         \
	dsrc = (mlib_d64 *)vis_alignaddr((void *)p_x, 0);       \
	tmp = *dsrc;                                            \
	REG = *(dsrc + 1);                                      \
	REG = vis_faligndata(tmp, REG)

/* *********************************************************** */

#define	WRITE_D_REG(REG)                                        \
	ddst = (mlib_d64 *)vis_alignaddr(p_z, 8);               \
	emask = vis_edge8(p_z, ddst);                           \
	vis_alignaddr((void *)~(mlib_addr)p_z, 1);              \
	tmp = vis_faligndata(REG, REG);                         \
	vis_pst_8(tmp, ddst - 1, emask);                        \
	vis_pst_8(tmp, ddst, ~emask)

/* *********************************************************** */

#define	READ_D_REG_INV(REG)                                     \
	tmp = *dsrc;                                            \
	REG = *(dsrc + 1);                                      \
	REG = vis_faligndata(tmp, REG)

/* *********************************************************** */

#define	WRITE_D_REG_INV(REG)                                    \
	tmp = vis_faligndata(REG, REG);                         \
	vis_pst_8(tmp, ddst - 1, emask);                        \
	vis_pst_8(tmp, ddst, ~emask)

/* *********************************************************** */

#define	READ_BLOCK_REG_INV                                      \
	p_x = &x[inv_i + j];                                    \
	dsrc = (mlib_d64 *)vis_alignaddr((void *)p_x, 0);       \
	READ_D_REG_INV(p0);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p1);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p2);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p3);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p4);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p5);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p6);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p7)

/* *********************************************************** */

#define	READ_BLOCK_REG                                          \
	p_x = &x[inv_i + j];                                    \
	READ_D_REG(p0);                                         \
	p_x += ds;                                              \
	READ_D_REG(p1);                                         \
	p_x += ds;                                              \
	READ_D_REG(p2);                                         \
	p_x += ds;                                              \
	READ_D_REG(p3);                                         \
	p_x += ds;                                              \
	READ_D_REG(p4);                                         \
	p_x += ds;                                              \
	READ_D_REG(p5);                                         \
	p_x += ds;                                              \
	READ_D_REG(p6);                                         \
	p_x += ds;                                              \
	READ_D_REG(p7)

/* *********************************************************** */

#define	WRITE_BLOCK_REG_INV                                     \
	p_z = &z[inv_j + i];                                    \
	ddst = (mlib_d64 *)vis_alignaddr(p_z, 8);               \
	emask = vis_edge8(p_z, ddst);                           \
	vis_alignaddr((void *)~(mlib_addr)p_z, 1);              \
	WRITE_D_REG_INV(q0);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q1);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q2);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q3);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q4);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q5);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q6);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q7)

/* *********************************************************** */

#define	WRITE_BLOCK_REG                                         \
	p_z = &z[inv_j + i];                                    \
	WRITE_D_REG(q0);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q1);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q2);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q3);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q4);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q5);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q6);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q7)

/* *********************************************************** */

#define	READ_SECOND_BLOCK_REG_INV                               \
	p_x = &z[inv_j + i];                                    \
	dsrc = (mlib_d64 *)vis_alignaddr(p_x, 0);               \
	READ_D_REG_INV(p00);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p11);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p22);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p33);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p44);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p55);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p66);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p77)

/* *********************************************************** */

#define	READ_SECOND_BLOCK_REG                                   \
	p_x = &z[inv_j + i];                                    \
	READ_D_REG(p00);                                        \
	p_x += ds;                                              \
	READ_D_REG(p11);                                        \
	p_x += ds;                                              \
	READ_D_REG(p22);                                        \
	p_x += ds;                                              \
	READ_D_REG(p33);                                        \
	p_x += ds;                                              \
	READ_D_REG(p44);                                        \
	p_x += ds;                                              \
	READ_D_REG(p55);                                        \
	p_x += ds;                                              \
	READ_D_REG(p66);                                        \
	p_x += ds;                                              \
	READ_D_REG(p77)

/* *********************************************************** */

#define	WRITE_SECOND_BLOCK_REG_INV                              \
	p_z = &x[inv_i + j];                                    \
	ddst = (mlib_d64 *)vis_alignaddr(p_z, 8);               \
	emask = vis_edge8(p_z, ddst);                           \
	vis_alignaddr((void *)~(mlib_addr)p_z, 1);              \
	WRITE_D_REG_INV(q00);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q11);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q22);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q33);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q44);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q55);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q66);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q77)

/* *********************************************************** */

#define	WRITE_SECOND_BLOCK_REG                                  \
	p_z = &x[inv_i + j];                                    \
	WRITE_D_REG(q00);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q11);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q22);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q33);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q44);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q55);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q66);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q77)

/* *********************************************************** */

#define	READ_BLOCK_REG_16_INV                                   \
	p_x = &x[inv_i + j];                                    \
	dsrc = (mlib_d64 *)vis_alignaddr((void *)p_x, 0);       \
	READ_D_REG_INV(p0);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p1);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p2);                                     \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p3)

/* *********************************************************** */

#define	READ_BLOCK_REG_16                                       \
	p_x = &x[inv_i + j];                                    \
	READ_D_REG(p0);                                         \
	p_x += ds;                                              \
	READ_D_REG(p1);                                         \
	p_x += ds;                                              \
	READ_D_REG(p2);                                         \
	p_x += ds;                                              \
	READ_D_REG(p3)

/* *********************************************************** */

#define	WRITE_BLOCK_REG_16_INV                                  \
	p_z = &z[inv_j + i];                                    \
	ddst = (mlib_d64 *)vis_alignaddr(p_z, 8);               \
	emask = vis_edge8(p_z, ddst);                           \
	vis_alignaddr((void *)~(mlib_addr)p_z, 1);              \
	WRITE_D_REG_INV(q0);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q1);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q2);                                    \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q3)

/* *********************************************************** */

#define	WRITE_BLOCK_REG_16                                      \
	p_z = &z[inv_j + i];                                    \
	WRITE_D_REG(q0);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q1);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q2);                                        \
	p_z += ds;                                              \
	WRITE_D_REG(q3)

/* *********************************************************** */

#define	READ_SECOND_BLOCK_REG_16_INV                            \
	p_x = &z[inv_j + i];                                    \
	dsrc = (mlib_d64 *)vis_alignaddr(p_x, 0);               \
	READ_D_REG_INV(p00);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p11);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p22);                                    \
	dsrc += shift_n;                                        \
	READ_D_REG_INV(p33)

/* *********************************************************** */

#define	READ_SECOND_BLOCK_REG_16                                \
	p_x = &z[inv_j + i];                                    \
	READ_D_REG(p00);                                        \
	p_x += ds;                                              \
	READ_D_REG(p11);                                        \
	p_x += ds;                                              \
	READ_D_REG(p22);                                        \
	p_x += ds;                                              \
	READ_D_REG(p33)

/* *********************************************************** */

#define	WRITE_SECOND_BLOCK_REG_16_INV                           \
	p_z = &x[inv_i + j];                                    \
	ddst = (mlib_d64 *)vis_alignaddr(p_z, 8);               \
	emask = vis_edge8(p_z, ddst);                           \
	vis_alignaddr((void *)~(mlib_addr)p_z, 1);              \
	WRITE_D_REG_INV(q00);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q11);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q22);                                   \
	ddst += shift_m;                                        \
	WRITE_D_REG_INV(q33)

/* *********************************************************** */

#define	WRITE_SECOND_BLOCK_REG_16                               \
	p_z = &x[inv_i + j];                                    \
	WRITE_D_REG(q00);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q11);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q22);                                       \
	p_z += ds;                                              \
	WRITE_D_REG(q33)

/*
 *
 *  Process matrix_8 when output vector is the same as input and
 *  is 8-byte aligned. Reading and writing by 32 bits.
 *  Function works by 8*8-byte blocks .
 *  nm & 7 == 0 !!!
 *
 */

static void
mlib_trans8_diag_al88_nm8(
	mlib_s8 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;

/* Input d64 - registers for transposing */
	mlib_d64 p0, p00, p1, p11, p2, p22, p3, p33;
	mlib_d64 p4, p44, p5, p55, p6, p66, p7, p77;

/* Output d64 - registers for transposing */
	mlib_d64 q0, q00, q1, q11, q2, q22, q3, q33;
	mlib_d64 q4, q44, q5, q55, q6, q66, q7, q77;

/* Temporary variables */
	mlib_d64 m04, m26, m15, m37, m0426, m1537;
	mlib_d64 *dsrc, *ddst;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 3, shift = ds >> 3;

	for (i = 0; i < (nm & ~7); i += 8) {
		for (j = i, inv_j = j * ds + i; j < (nm & ~7); j += 8) {

/* First, read 64 elements from "square" and place them on d - reg. */

			dsrc = (mlib_d64 *)&xz[inv_i + j];
			p0 = *dsrc;
			p1 = dsrc[shift];
			p2 = dsrc[shift << 1];
			p3 = dsrc[shift * 3];
			p4 = dsrc[shift << 2];
			p5 = dsrc[shift * 5];
			p6 = dsrc[shift * 6];
			p7 = dsrc[shift * 7];

			TRAN_BLOCK_8;

/* Now read symmetrical elements and place them on d - registers. */

			dsrc = (mlib_d64 *)&xz[inv_j];
			p00 = *dsrc;
			p11 = dsrc[shift];
			p22 = dsrc[shift << 1];
			p33 = dsrc[shift * 3];
			p44 = dsrc[shift << 2];
			p55 = dsrc[shift * 5];
			p66 = dsrc[shift * 6];
			p77 = dsrc[shift * 7];

			TRAN_SECOND_BLOCK_8;

/* Then we store elements to symmetrical  part. */

			ddst = (mlib_d64 *)&xz[inv_j];
			*ddst = q0;
			ddst[shift] = q1;
			ddst[shift << 1] = q2;
			ddst[shift * 3] = q3;
			ddst[shift << 2] = q4;
			ddst[shift * 5] = q5;
			ddst[shift * 6] = q6;
			ddst[shift * 7] = q7;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

			ddst = (mlib_d64 *)&xz[inv_i + j];
			*ddst = q00;
			ddst[shift] = q11;
			ddst[shift << 1] = q22;
			ddst[shift * 3] = q33;
			ddst[shift << 2] = q44;
			ddst[shift * 5] = q55;
			ddst[shift * 6] = q66;
			ddst[shift * 7] = q77;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

static void
mlib_trans8_diag_al88_nm8_s(
	mlib_s8 *z,
	mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;

/* Input d64 - registers for transposing */
	mlib_d64 p0, p00, p1, p11, p2, p22, p3, p33;
	mlib_d64 p4, p44, p5, p55, p6, p66, p7, p77;

/* Output d64 - registers for transposing */
	mlib_d64 q0, q00, q1, q11, q2, q22, q3, q33;
	mlib_d64 q4, q44, q5, q55, q6, q66, q7, q77;

/* Temporary variables */
	mlib_d64 m04, m26, m15, m37, m0426, m1537;
	mlib_d64 *dsrc, *ddst;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 3, shift = ds >> 3;

	for (i = 0; i < (m & ~7); i += 8) {
		for (j = 0, inv_j = j * ds + i; j < (n & ~7); j += 8) {

/* First, read 64 elements from "square" and place them on d - reg. */

			dsrc = (mlib_d64 *)&x[inv_i + j];
			p0 = *dsrc;
			p1 = dsrc[shift];
			p2 = dsrc[shift << 1];
			p3 = dsrc[shift * 3];
			p4 = dsrc[shift << 2];
			p5 = dsrc[shift * 5];
			p6 = dsrc[shift * 6];
			p7 = dsrc[shift * 7];

			TRAN_BLOCK_8;

/* Now read symmetrical elements and place them on d - registers. */

			dsrc = (mlib_d64 *)&z[inv_j];
			p00 = *dsrc;
			p11 = dsrc[shift];
			p22 = dsrc[shift << 1];
			p33 = dsrc[shift * 3];
			p44 = dsrc[shift << 2];
			p55 = dsrc[shift * 5];
			p66 = dsrc[shift * 6];
			p77 = dsrc[shift * 7];

			TRAN_SECOND_BLOCK_8;

/* Then we store elements to symmetrical  part. */

			ddst = (mlib_d64 *)&z[inv_j];
			*ddst = q0;
			ddst[shift] = q1;
			ddst[shift << 1] = q2;
			ddst[shift * 3] = q3;
			ddst[shift << 2] = q4;
			ddst[shift * 5] = q5;
			ddst[shift * 6] = q6;
			ddst[shift * 7] = q7;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

			ddst = (mlib_d64 *)&x[inv_i + j];
			*ddst = q00;
			ddst[shift] = q11;
			ddst[shift << 1] = q22;
			ddst[shift * 3] = q33;
			ddst[shift << 2] = q44;
			ddst[shift * 5] = q55;
			ddst[shift * 6] = q66;
			ddst[shift * 7] = q77;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

/*
 *
 *  Process matrix_8 when output vector is the same as input and
 *  is arbitrary aligned. Reading and writing by 64 bits.
 *  Function works by 8*8-byte blocks.
 *  nm & 7 != 0 !!!
 *
 */

static void
mlib_trans8_diag_alxx(
	mlib_s8 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;

	mlib_s8 *x = xz, *z = xz, *p_x = xz, *p_z = xz;

/* Input d64 - registers for transposing */
	mlib_d64 p0, p00, p1, p11, p2, p22, p3, p33, p4, p44, p5, p55, p6, p66,
		p7, p77;

/* Output d64 - registers for transposing */
	mlib_d64 q0, q00, q1, q11, q2, q22, q3, q33, q4, q44, q5, q55, q6, q66,
		q7, q77;

/* Temporary variables */
	mlib_d64 m04, m26, m15, m37, m0426, m1537;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 tmp;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 3, shift = ds >> 3;
	mlib_s32 shift_n = shift, shift_m = shift, emask;

	if (ds & 7) {
		for (i = 0; i < (nm & ~7); i += 8) {
#pragma pipeloop(0)
			for (j = i, inv_j = j * ds; j < (nm & ~7); j += 8) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG;
				TRAN_BLOCK_8;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG;
				TRAN_SECOND_BLOCK_8;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	} else {
		for (i = 0; i < (nm & ~7); i += 8) {
#pragma pipeloop(0)
			for (j = i, inv_j = j * ds; j < (nm & ~7); j += 8) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG_INV;
				TRAN_BLOCK_8;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG_INV;
				TRAN_SECOND_BLOCK_8;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG_INV;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG_INV;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	}
}

static void
mlib_trans8_diag_alxx_s(
	mlib_s8 *z,
	mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;

	mlib_s8 *p_x = x, *p_z = z;

/* Input d64 - registers for transposing */
	mlib_d64 p0, p00, p1, p11, p2, p22, p3, p33, p4, p44, p5, p55, p6, p66,
		p7, p77;

/* Output d64 - registers for transposing */
	mlib_d64 q0, q00, q1, q11, q2, q22, q3, q33, q4, q44, q5, q55, q6, q66,
		q7, q77;

/* Temporary variables */
	mlib_d64 m04, m26, m15, m37, m0426, m1537;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 tmp;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 3, shift = ds >> 3;
	mlib_s32 shift_n = shift, shift_m = shift, emask;

	if (ds & 7) {
		for (i = 0; i < (m & ~7); i += 8) {
#pragma pipeloop(0)
			for (j = 0, inv_j = j * ds; j < (n & ~7); j += 8) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG;
				TRAN_BLOCK_8;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG;
				TRAN_SECOND_BLOCK_8;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	} else {
		for (i = 0; i < (m & ~7); i += 8) {
#pragma pipeloop(0)
			for (j = 0, inv_j = j * ds; j < (n & ~7); j += 8) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG_INV;
				TRAN_BLOCK_8;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG_INV;
				TRAN_SECOND_BLOCK_8;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG_INV;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG_INV;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	}
}

/*
 *
 *  Process matrix_16 when output vector is the same as input and it
 *  is 8-byte aligned. Reading and writing by 64 bits.
 *  Function works by 4*4 elements.
 *
 *  nm & 3 == 0 !!!
 *
 */

static void
mlib_trans16_diag_al88_nm4(
	mlib_s16 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;

/* Source values for transpose */
	mlib_d64 p0, p1, p2, p3, p00, p11, p22, p33;

/* Resultant values for transpose */
	mlib_d64 q0, q1, q2, q3, q00, q11, q22, q33, m02, m13, m0213;
	mlib_d64 *dsrc, *ddst;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2, shift = ds >> 2;

	for (i = 0; i < (nm & ~3); i += 4) {
		for (j = i, inv_j = j * ds + i; j < (nm & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on d - reg. */

			dsrc = (mlib_d64 *)&xz[inv_i + j];
			p0 = *dsrc;

			p1 = dsrc[shift];
			p2 = dsrc[shift << 1];
			p3 = dsrc[shift * 3];

			TRAN_BLOCK_16;

/* Now read symmetrical elements and place them on d - registers. */

			dsrc = (mlib_d64 *)&xz[inv_j];
			p00 = *dsrc;

			p11 = dsrc[shift];
			p22 = dsrc[shift << 1];
			p33 = dsrc[shift * 3];

			TRAN_SECOND_BLOCK_16;

/* Then we store elements to symmetrical  part. */

			ddst = (mlib_d64 *)&xz[inv_j];
			*ddst = q0;
			ddst[shift] = q1;
			ddst[shift << 1] = q2;
			ddst[shift * 3] = q3;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

			ddst = (mlib_d64 *)&xz[inv_i + j];
			*ddst = q00;
			ddst[shift] = q11;
			ddst[shift << 1] = q22;
			ddst[shift * 3] = q33;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

static void
mlib_trans16_diag_al88_nm4_s(
	mlib_s16 *z,
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;

/* Source values for transpose */
	mlib_d64 p0, p1, p2, p3, p00, p11, p22, p33;

/* Resultant values for transpose */
	mlib_d64 q0, q1, q2, q3, q00, q11, q22, q33, m02, m13, m0213;
	mlib_d64 *dsrc, *ddst;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2, shift = ds >> 2;

	for (i = 0; i < (m & ~3); i += 4) {
		for (j = 0, inv_j = j * ds + i; j < (n & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on d - reg. */

			dsrc = (mlib_d64 *)&x[inv_i + j];
			p0 = *dsrc;

			p1 = dsrc[shift];
			p2 = dsrc[shift << 1];
			p3 = dsrc[shift * 3];

			TRAN_BLOCK_16;

/* Now read symmetrical elements and place them on d - registers. */

			dsrc = (mlib_d64 *)&z[inv_j];
			p00 = *dsrc;

			p11 = dsrc[shift];
			p22 = dsrc[shift << 1];
			p33 = dsrc[shift * 3];

			TRAN_SECOND_BLOCK_16;

/* Then we store elements to symmetrical  part. */

			ddst = (mlib_d64 *)&z[inv_j];
			*ddst = q0;
			ddst[shift] = q1;
			ddst[shift << 1] = q2;
			ddst[shift * 3] = q3;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

			ddst = (mlib_d64 *)&x[inv_i + j];
			*ddst = q00;
			ddst[shift] = q11;
			ddst[shift << 1] = q22;
			ddst[shift * 3] = q33;

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

/*
 *
 *  Process matrix_16 when output vector is the same as input and it
 *  is 8-byte aligned. Reading and writing by 64 bits.
 *  Function works by 4*4 elements.
 *
 *  nm & 3 == 0 !!!
 *
 */

static void
mlib_trans16_diag_alxx(
	mlib_s16 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;

/* Source values for transpose */
	mlib_d64 p0, p1, p2, p3, p00, p11, p22, p33;

/* Resultant values for transpose */
	mlib_d64 q0, q1, q2, q3, q00, q11, q22, q33;

/* Temporary variables for transpose and STORE. */
	mlib_d64 tmp, m02, m13, m0213;
	mlib_s16 *x = xz, *p_x = xz, *z = xz, *p_z = xz;
	mlib_d64 *dsrc, *ddst;
	mlib_s32 emask, inv_i = 0, inv_j;
	mlib_s32 step_n = ds << 2, shift = ds >> 2;
	mlib_s32 shift_n = shift, shift_m = shift;

	if (ds & 3) {
		for (i = 0; i < (nm & ~3); i += 4) {
#pragma pipeloop(0)
			for (j = i, inv_j = j * ds; j < (nm & ~3); j += 4) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG_16;

				TRAN_BLOCK_16;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG_16;

				TRAN_SECOND_BLOCK_16;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG_16;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG_16;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	} else {
		for (i = 0; i < (nm & ~3); i += 4) {
#pragma pipeloop(0)
			for (j = i, inv_j = j * ds; j < (nm & ~3); j += 4) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG_16_INV;

				TRAN_BLOCK_16;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG_16_INV;

				TRAN_SECOND_BLOCK_16;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG_16_INV;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG_16_INV;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	}
}

static void
mlib_trans16_diag_alxx_s(
	mlib_s16 *z,
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;

/* Source values for transpose */
	mlib_d64 p0, p1, p2, p3, p00, p11, p22, p33;

/* Resultant values for transpose */
	mlib_d64 q0, q1, q2, q3, q00, q11, q22, q33;

/* Temporary variables for transpose and STORE. */
	mlib_d64 tmp, m02, m13, m0213;
	mlib_s16 *p_x = x, *p_z = z;
	mlib_d64 *dsrc, *ddst;
	mlib_s32 emask, inv_i = 0, inv_j;
	mlib_s32 step_n = ds << 2, shift = ds >> 2;
	mlib_s32 shift_n = shift, shift_m = shift;

	if (ds & 3) {
		for (i = 0; i < (m & ~3); i += 4) {
#pragma pipeloop(0)
			for (j = 0, inv_j = j * ds; j < (n & ~3); j += 4) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG_16;

				TRAN_BLOCK_16;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG_16;

				TRAN_SECOND_BLOCK_16;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG_16;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG_16;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
	} else {
		for (i = 0; i < (m & ~3); i += 4) {
#pragma pipeloop(0)
			for (j = 0, inv_j = j * ds; j < (n & ~3); j += 4) {

/* First, read 64 elements from "square" and place them on d - reg. */

				READ_BLOCK_REG_16_INV;

				TRAN_BLOCK_16;

/* Now read symmetrical elements and place them on d - registers. */

				READ_SECOND_BLOCK_REG_16_INV;

				TRAN_SECOND_BLOCK_16;

/* Then we store elements to symmetrical  part. */

				WRITE_BLOCK_REG_16_INV;

/*
 * The last action - storing the elements from symmetrical square
 * to our working region.
 */

				WRITE_SECOND_BLOCK_REG_16_INV;
				inv_j += step_n;
			}

			inv_i += step_n;
		}
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
	mlib_f32 d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	mlib_s32 *p_x, *p_x1, *p_x2, *p_x3, *p_z, *p_z1, *p_z2, *p_z3;
	mlib_f32 *fdst, *fsrc;
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
			fsrc = (mlib_f32 *)p_z;
			d1 = (*fsrc++);
			d5 = (*fsrc++);
			d9 = (*fsrc++);
			d13 = *fsrc;

			p_z = &xz[inv_j + ds];
			fsrc = (mlib_f32 *)p_z;
			d2 = (*fsrc++);
			d6 = (*fsrc++);
			d10 = (*fsrc++);
			d14 = *fsrc;

			p_z = &xz[inv_j + (ds << 1)];
			fsrc = (mlib_f32 *)p_z;
			d3 = (*fsrc++);
			d7 = (*fsrc++);
			d11 = (*fsrc++);
			d15 = *fsrc;

			p_z = &xz[inv_j + ds + (ds << 1)];
			fsrc = (mlib_f32 *)p_z;
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
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = d1;
			(*fdst++) = d2;
			(*fdst++) = d3;
			*fdst = d4;

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = d5;
			(*fdst++) = d6;
			(*fdst++) = d7;
			*fdst = d8;

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = d9;
			(*fdst++) = d10;
			(*fdst++) = d11;
			*fdst = d12;

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
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
	mlib_f32 d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	mlib_s32 *p_x, *p_z;
	mlib_f32 *fdst, *fsrc;
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
			fsrc = (mlib_f32 *)p_z;
			d1 = (*fsrc++);
			d5 = (*fsrc++);
			d9 = (*fsrc++);
			d13 = *fsrc;

			p_z = &z[inv_j + ds];
			fsrc = (mlib_f32 *)p_z;
			d2 = (*fsrc++);
			d6 = (*fsrc++);
			d10 = (*fsrc++);
			d14 = *fsrc;

			p_z = &z[inv_j + (ds << 1)];
			fsrc = (mlib_f32 *)p_z;
			d3 = (*fsrc++);
			d7 = (*fsrc++);
			d11 = (*fsrc++);
			d15 = *fsrc;

			p_z = &z[inv_j + ds + (ds << 1)];
			fsrc = (mlib_f32 *)p_z;
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
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = d1;
			(*fdst++) = d2;
			(*fdst++) = d3;
			*fdst = d4;

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = d5;
			(*fdst++) = d6;
			(*fdst++) = d7;
			*fdst = d8;

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = d9;
			(*fdst++) = d10;
			(*fdst++) = d11;
			*fdst = d12;

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
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
 *  Process matrix_32 when output vector is the same as input and
 *  is 8-byte aligned. Reading by 64 bits but writing by 32 bits.
 *  Function operates by 4*4 elements.
 *
 *  nm&1 != 0
 *
 */

static void
mlib_trans32_diag_4_al88(
	mlib_s32 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, i1, i2, i3, j;
	mlib_d64 a1, a3, a5, a7, a9, a11, a13, a15;
	mlib_d64 d1, d3, d5, d7, d9, d11, d13, d15;
	mlib_s32 *p_x, *p_x1, *p_x2, *p_x3, *p_z, *p_z1, *p_z2, *p_z3;
	mlib_d64 *dsrc;
	mlib_f32 *fdst;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2;

	for (i = 0; i < (nm & ~3); i += 4) {
/*
 * First, process 6 elements xz[i, i + 1], xz[i, i + 2], xz[i, i + 3],
 * xz[i + 1, i + 2], xz[i + 1, i + 3] and xz[i + 2, i + 3] which are near the
 * main diagonal at a distance less then 4 elements.
 */

		i1 = *(p_z1 = &INDVAR(xz, i + 1, i, ds));
		i2 = *(p_z2 = &INDVAR(xz, i + 2, i, ds));
		i3 = *(p_z3 = &INDVAR(xz, i + 3, i, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i, i + 1, ds));
		*p_z2 = *(p_x2 = p_x1 + 1);
		*p_z3 = *(p_x3 = p_x1 + 2);
		*p_x1 = i1;
		*p_x2 = i2;
		*p_x3 = i3;

		i1 = *(p_z1 = &INDVAR(xz, i + 2, i + 1, ds));
		i2 = *(p_z2 = &INDVAR(xz, i + 3, i + 1, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i + 1, i + 2, ds));
		*p_z2 = *(p_x2 = p_x1 + 1);
		*p_x1 = i1;
		*p_x2 = i2;

		i1 = *(p_z1 = &INDVAR(xz, i + 3, i + 2, ds));
		*p_z1 = *(p_x1 = &INDVAR(xz, i + 2, i + 3, ds));
		*p_x1 = i1;

/* Then we process by loop other elements xz[i, i + 4], xz[i, i + 5] ... */

		for (j = i + 4, inv_j = j * ds + i; j < (nm & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on f - reg. */

			p_x = &xz[inv_i + j];
			dsrc = (mlib_d64 *)p_x;
			a1 = *dsrc;
			a3 = *(dsrc + 1);

			p_x += ds;
			dsrc = (mlib_d64 *)p_x;
			a5 = *dsrc;
			a7 = *(dsrc + 1);

			p_x += ds;
			dsrc = (mlib_d64 *)p_x;
			a9 = *dsrc;
			a11 = *(dsrc + 1);

			p_x += ds;
			dsrc = (mlib_d64 *)p_x;
			a13 = *dsrc;
			a15 = *(dsrc + 1);

/* Now read symmetrical elements and place them on f - registers. */

			p_z = &xz[inv_j];
			dsrc = (mlib_d64 *)p_z;
			d1 = *dsrc;
			d3 = *(dsrc + 1);

			p_z += ds;
			dsrc = (mlib_d64 *)p_z;
			d5 = *dsrc;
			d7 = *(dsrc + 1);

			p_z += ds;
			dsrc = (mlib_d64 *)p_z;
			d9 = *dsrc;
			d11 = *(dsrc + 1);

			p_z += ds;
			dsrc = (mlib_d64 *)p_z;
			d13 = *dsrc;
			d15 = *(dsrc + 1);

/* Then we store elements to symmetrical  part. */

			p_z = &xz[inv_j];
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_hi(a1);
			(*fdst++) = vis_read_hi(a5);
			(*fdst++) = vis_read_hi(a9);
			*fdst = vis_read_hi(a13);

			p_z += ds;
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_lo(a1);
			(*fdst++) = vis_read_lo(a5);
			(*fdst++) = vis_read_lo(a9);
			*fdst = vis_read_lo(a13);

			p_z += ds;
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_hi(a3);
			(*fdst++) = vis_read_hi(a7);
			(*fdst++) = vis_read_hi(a11);
			*fdst = vis_read_hi(a15);

			p_z += ds;
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_lo(a3);
			(*fdst++) = vis_read_lo(a7);
			(*fdst++) = vis_read_lo(a11);
			*fdst = vis_read_lo(a15);

/*
 * The last action - storing the elements from symmetrical square
 * to our working region. Writing in source format.
 */

			p_x = &xz[inv_i + j];
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_hi(d1);
			(*fdst++) = vis_read_hi(d5);
			(*fdst++) = vis_read_hi(d9);
			*fdst = vis_read_hi(d13);

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_lo(d1);
			(*fdst++) = vis_read_lo(d5);
			(*fdst++) = vis_read_lo(d9);
			*fdst = vis_read_lo(d13);

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_hi(d3);
			(*fdst++) = vis_read_hi(d7);
			(*fdst++) = vis_read_hi(d11);
			*fdst = vis_read_hi(d15);

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_lo(d3);
			(*fdst++) = vis_read_lo(d7);
			(*fdst++) = vis_read_lo(d11);
			*fdst = vis_read_lo(d15);

			inv_j += step_n;
		}

		inv_i += step_n;
	}
}

static void
mlib_trans32_diag_4_al88_s(
	mlib_s32 *z,
	mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_d64 a1, a3, a5, a7, a9, a11, a13, a15;
	mlib_d64 d1, d3, d5, d7, d9, d11, d13, d15;
	mlib_s32 *p_x, *p_z;
	mlib_d64 *dsrc;
	mlib_f32 *fdst;
	mlib_s32 inv_i = 0, inv_j, step_n = ds << 2;

	for (i = 0; i < (m & ~3); i += 4) {
		for (j = 0, inv_j = j * ds + i; j < (n & ~3); j += 4) {

/* First, read 16 elements from "square" and place them on f - reg. */

			p_x = &x[inv_i + j];
			dsrc = (mlib_d64 *)p_x;
			a1 = *dsrc;
			a3 = *(dsrc + 1);

			p_x += ds;
			dsrc = (mlib_d64 *)p_x;
			a5 = *dsrc;
			a7 = *(dsrc + 1);

			p_x += ds;
			dsrc = (mlib_d64 *)p_x;
			a9 = *dsrc;
			a11 = *(dsrc + 1);

			p_x += ds;
			dsrc = (mlib_d64 *)p_x;
			a13 = *dsrc;
			a15 = *(dsrc + 1);

/* Now read symmetrical elements and place them on f - registers. */

			p_z = &z[inv_j];
			dsrc = (mlib_d64 *)p_z;
			d1 = *dsrc;
			d3 = *(dsrc + 1);

			p_z += ds;
			dsrc = (mlib_d64 *)p_z;
			d5 = *dsrc;
			d7 = *(dsrc + 1);

			p_z += ds;
			dsrc = (mlib_d64 *)p_z;
			d9 = *dsrc;
			d11 = *(dsrc + 1);

			p_z += ds;
			dsrc = (mlib_d64 *)p_z;
			d13 = *dsrc;
			d15 = *(dsrc + 1);

/* Then we store elements to symmetrical  part. */

			p_z = &z[inv_j];
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_hi(a1);
			(*fdst++) = vis_read_hi(a5);
			(*fdst++) = vis_read_hi(a9);
			*fdst = vis_read_hi(a13);

			p_z += ds;
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_lo(a1);
			(*fdst++) = vis_read_lo(a5);
			(*fdst++) = vis_read_lo(a9);
			*fdst = vis_read_lo(a13);

			p_z += ds;
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_hi(a3);
			(*fdst++) = vis_read_hi(a7);
			(*fdst++) = vis_read_hi(a11);
			*fdst = vis_read_hi(a15);

			p_z += ds;
			fdst = (mlib_f32 *)p_z;
			(*fdst++) = vis_read_lo(a3);
			(*fdst++) = vis_read_lo(a7);
			(*fdst++) = vis_read_lo(a11);
			*fdst = vis_read_lo(a15);

/*
 * The last action - storing the elements from symmetrical square
 * to our working region. Writing in source format.
 */

			p_x = &x[inv_i + j];
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_hi(d1);
			(*fdst++) = vis_read_hi(d5);
			(*fdst++) = vis_read_hi(d9);
			*fdst = vis_read_hi(d13);

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_lo(d1);
			(*fdst++) = vis_read_lo(d5);
			(*fdst++) = vis_read_lo(d9);
			*fdst = vis_read_lo(d13);

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_hi(d3);
			(*fdst++) = vis_read_hi(d7);
			(*fdst++) = vis_read_hi(d11);
			*fdst = vis_read_hi(d15);

			p_x += ds;
			fdst = (mlib_f32 *)p_x;
			(*fdst++) = vis_read_lo(d3);
			(*fdst++) = vis_read_lo(d7);
			(*fdst++) = vis_read_lo(d11);
			*fdst = vis_read_lo(d15);

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
 *  condition is TRUE always.
 *
 */

static void
mlib_trans64_diag_4(
	mlib_d64 *xz,
	mlib_s32 nm,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_d64 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	mlib_d64 *p_x, *p_x1, *p_x2, *p_x3, *p_z, *p_z1, *p_z2, *p_z3;
	mlib_d64 *dst, *src;
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
			dst = (mlib_d64 *)p_x;
			(*dst++) = d1;
			(*dst++) = d2;
			(*dst++) = d3;
			*dst = d4;

			p_x += ds;
			dst = (mlib_d64 *)p_x;
			(*dst++) = d5;
			(*dst++) = d6;
			(*dst++) = d7;
			*dst = d8;

			p_x += ds;
			dst = (mlib_d64 *)p_x;
			(*dst++) = d9;
			(*dst++) = d10;
			(*dst++) = d11;
			*dst = d12;

			p_x += ds;
			dst = (mlib_d64 *)p_x;
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
	mlib_d64 *z,
	mlib_d64 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j;
	mlib_d64 a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
		a15, a16;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14,
		d15, d16;
	mlib_d64 *p_x, *p_z;
	mlib_d64 *dst, *src;
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
			dst = (mlib_d64 *)p_x;
			(*dst++) = d1;
			(*dst++) = d2;
			(*dst++) = d3;
			*dst = d4;

			p_x += ds;
			dst = (mlib_d64 *)p_x;
			(*dst++) = d5;
			(*dst++) = d6;
			(*dst++) = d7;
			*dst = d8;

			p_x += ds;
			dst = (mlib_d64 *)p_x;
			(*dst++) = d9;
			(*dst++) = d10;
			(*dst++) = d11;
			*dst = d12;

			p_x += ds;
			dst = (mlib_d64 *)p_x;
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
	mlib_d64 *p_x, *p_z;
	mlib_s32 n_even_4 = nm & ~3;
	mlib_d64 *x = xz, *z = xz;
	mlib_d64 a;

	if (nm <= 0)
		return (MLIB_FAILURE);

	if (nm < 8) {
		TRANS_DIAG(mlib_d64);
	}

	mlib_trans64_diag_4((mlib_d64 *)xz, nm, nm);

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
	mlib_d64 *z,
	mlib_d64 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j, nm = m;
	mlib_d64 *p_x, *p_z, *xz = x;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	mlib_d64 a;

	if (z == x) {
		if (m < 8) {
			TRANSS_DIAG(mlib_d64);
		}

		mlib_trans64_diag_4((mlib_d64 *) xz, nm, ds);

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
			TRANSS_DIAG2(mlib_d64);
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

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_U8(
	mlib_u8 *xz,
	mlib_s32 nm)
{
	mlib_s32 i, j;
	mlib_u8 *p_x, *p_z;
	mlib_s32 n_rest_8 = nm & 7, n_even_8 = nm & ~7;
	mlib_u8 *x = xz, *z = xz;
	mlib_u8 a;

	if (nm <= 0)
		return (MLIB_FAILURE);

	if (nm < 8) {
		TRANS_DIAG(mlib_u8);
	}

	if ((!((mlib_u32)xz & 7)) && (!n_rest_8)) {

/* Source matrix is 64-bit aligned. Read and write by 64 bits */

		mlib_trans8_diag_al88_nm8((mlib_s8 *)xz, nm, nm);
	} else {

/* General case. */

		mlib_trans8_diag_alxx((mlib_s8 *)xz, nm, nm);
	}

	for (j = n_even_8; j < nm; j++) {
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
mlib_MatrixTranspose_S_U8(
	mlib_u8 *z,
	mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j, nm = m;
	mlib_u8 *p_x, *p_z;
	mlib_s32 n_even_8 = n & ~7;
	mlib_s32 m_even_8 = m & ~7;
	mlib_s32 ds_rest_8 = ds & 7;
	mlib_u8 a;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (z == x) {
		if (nm < 8) {
			TRANSS_DIAG(mlib_u8);
		}

		if ((!((mlib_u32)x & 7)) && (!ds_rest_8)) {
/* Source matrix is 64-bit aligned. Read and write by 64 bits */

			mlib_trans8_diag_al88_nm8((mlib_s8 *)z, nm, ds);
		} else {
/* General case. */

			mlib_trans8_diag_alxx((mlib_s8 *)z, nm, ds);
		}

		for (j = n_even_8; j < nm; j++) {
			p_z = &INDVAR(z, j, 0, ds);
			for (i = 0; i < j; i++) {
				a = *p_z;
				(*p_z++) = *(p_x = &INDVAR(x, i, j, ds));
				*p_x = a;
			}
		}
	} else {
		if (n < 8 || m < 8) {
			TRANSS_DIAG2(mlib_u8);
		}

		if ((!(((mlib_u32)x | (mlib_u32)z) & 7)) && (!ds_rest_8)) {

/* Source matrix is 64-bit aligned. Read and write by 64 bits */

			mlib_trans8_diag_al88_nm8_s((mlib_s8 *)z,
				(mlib_s8 *)x, m, n, ds);
		} else {

/* General case. */

			mlib_trans8_diag_alxx_s((mlib_s8 *)z,
				(mlib_s8 *)x, m, n, ds);
		}

		for (j = m_even_8; j < m; j++) {
			p_x = &INDVAR(x, j, 0, ds);
			for (i = 0; i < n; i++) {
				a = *p_x;
				(*p_x++) = *(p_z = &INDVAR(z, i, j, ds));
				*p_z = a;
			}
		}

		if (n_even_8 < n) {
			for (j = 0; j < m_even_8; j++) {
				p_x = &INDVAR(x, j, n_even_8, ds);
				for (i = n_even_8; i < n; i++) {
					a = *p_x;
					(*p_x++) = *(p_z = &INDVAR(z, i, j,
						ds));
					*p_z = a;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
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
	mlib_s32 i, j;
	mlib_s16 *p_x, *p_z;
	mlib_s32 n_rest_4 = nm & 3, n_even_4 = nm & ~3;
	mlib_s16 *x = xz, *z = xz;
	mlib_s16 a;

	if (nm <= 0)
		return (MLIB_FAILURE);

	if (nm < 8) {
		TRANS_DIAG(mlib_s16);
	}

	if ((!((mlib_u32)xz & 7)) && (!n_rest_4)) {

/* Source matrix is 64-bit aligned. Read by 32 and write by 64 bits */

		mlib_trans16_diag_al88_nm4((mlib_s16 *)xz, nm, nm);
	} else {

/* General case. */

		mlib_trans16_diag_alxx((mlib_s16 *)xz, nm, nm);
	}

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
mlib_MatrixTranspose_S_S16(
	mlib_s16 *z,
	mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j, nm = m;
	mlib_s16 *p_x, *p_z;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	mlib_s32 ds_rest_4 = ds & 3;
	mlib_s16 a;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (z == x) {
		if (nm < 4) {
			TRANSS_DIAG(mlib_s16);
		}

		if ((!((mlib_u32)x & 7)) && (!ds_rest_4)) {

/* Source matrix is 64-bit aligned. Read by 32 and write by 64 bits */

			mlib_trans16_diag_al88_nm4((mlib_s16 *)z, nm, ds);
		} else {

/* General case. */

			mlib_trans16_diag_alxx((mlib_s16 *)z, nm, ds);
		}

		for (j = n_even_4; j < nm; j++) {
			p_z = &INDVAR(z, j, 0, ds);
			for (i = 0; i < j; i++) {
				a = *p_z;
				(*p_z++) = *(p_x = &INDVAR(x, i, j, ds));
				*p_x = a;
			}
		}
	} else {
		if (n < 4 || m < 4) {
			TRANSS_DIAG2(mlib_s16);
		}

		if ((!(((mlib_u32)x | (mlib_u32)z) & 7)) && (!ds_rest_4)) {

/* Source matrix is 64-bit aligned. Read by 32 and write by 64 bits */

			mlib_trans16_diag_al88_nm4_s((mlib_s16 *)z,
				(mlib_s16 *)x, m, n, ds);
		} else {

/* General case. */

			mlib_trans16_diag_alxx_s((mlib_s16 *)z,
				(mlib_s16 *)x, m, n, ds);
		}

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
	}


	return (MLIB_SUCCESS);
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
		TRANS_DIAG(mlib_s32);
	}

	if ((!((mlib_u32)xz & 7)) && (!(nm & 1))) {

/* Source matrix is 64-bit aligned. Read by 64 bits */

		mlib_trans32_diag_4_al88((mlib_s32 *)xz, nm, nm);
	} else {

/* General case. */

		mlib_trans32_diag_4_alxx((mlib_s32 *)xz, nm, nm);
	}

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
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	mlib_s32 ds_rest_2 = ds & 1;
	mlib_s32 a;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (z == x) {
		if (nm < 4) {
			TRANSS_DIAG(mlib_s32);
		}

		if ((!((mlib_u32)z & 7)) && (!ds_rest_2)) {

/* Source matrix is 64-bit aligned. Read by 64 bits */

			mlib_trans32_diag_4_al88((mlib_s32 *)z, nm, ds);
		} else {

/* General case. */

			mlib_trans32_diag_4_alxx((mlib_s32 *)z, nm, ds);
		}

		for (j = n_even_4; j < nm; j++) {
			p_z = &INDVAR(z, j, 0, ds);
			for (i = 0; i < j; i++) {
				a = *p_z;
				(*p_z++) = *(p_x = &INDVAR(x, i, j, ds));
				*p_x = a;
			}
		}
	} else {
		if (n < 4 || m < 4) {
			TRANSS_DIAG2(mlib_s32);
		}

		if ((!(((mlib_u32)z | (mlib_u32)x) & 7)) && (!ds_rest_2)) {

/* Source matrix is 64-bit aligned. Read by 64 bits */

			mlib_trans32_diag_4_al88_s((mlib_s32 *)z,
				(mlib_s32 *)x, m, n, ds);
		} else {

/* General case. */

			mlib_trans32_diag_4_alxx_s((mlib_s32 *)z,
				(mlib_s32 *)x, m, n, ds);
		}

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
	}

	return (MLIB_SUCCESS);
}


/* Complex Procedures Interface */

/* *********************************************************** */

mlib_status
__mlib_MatrixTranspose_U8C(
	mlib_u8 *xz,
	mlib_s32 nm)
{
	mlib_s32 i, j;
	mlib_u8 *p_x, *p_z;
	mlib_s32 n_even_4 = nm & ~3;
	mlib_u8 *x = xz, *z = xz;
	mlib_u8 a_r, a_i;

	if (nm <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_s32)xz & 1))

		return (__mlib_MatrixTranspose_S16((mlib_s16 *)xz, nm));
	else
		mlib_trans16_diag_alxx((mlib_s16 *)xz, nm, nm);

	for (j = n_even_4; j < nm; j++) {
		p_z = &INDVAR_C_RE(z, j, 0, nm);
		for (i = 0; i < j; i++) {
			a_r = *p_z;
			a_i = *(p_z + 1);
			p_x = &INDVAR_C_RE(x, i, j, nm);
			(*p_z++) = *(p_x);
			*p_x = a_r;
			(*p_z++) = p_x[1];
			p_x[1] = a_i;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_MatrixTranspose_S_U8C(
	mlib_u8 *z,
	mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds)
{
	mlib_s32 i, j, nm = m;
	mlib_u8 *p_x, *p_z;
	mlib_s32 n_even_4 = n & ~3;
	mlib_s32 m_even_4 = m & ~3;
	mlib_u8 a_r, a_i;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_s32)z | (mlib_s32)x) & 1))
		return (mlib_MatrixTranspose_S_S16((mlib_s16 *)z,
			(mlib_s16 *)x, m, n, ds));
	else if (z == x) {
		mlib_trans16_diag_alxx((mlib_s16 *)z, nm, ds);

		for (j = n_even_4; j < nm; j++) {
			p_z = &INDVAR_C_RE(z, j, 0, ds);
			for (i = 0; i < j; i++) {
				a_r = *p_z;
				a_i = *(p_z + 1);
				p_x = &INDVAR_C_RE(x, i, j, ds);
				(*p_z++) = *(p_x);
				*p_x = a_r;
				(*p_z++) = p_x[1];
				p_x[1] = a_i;
			}
		}
	} else {
		mlib_trans16_diag_alxx_s((mlib_s16 *)z,
			(mlib_s16 *)x, m, n, ds);

		for (j = m_even_4; j < m; j++) {
			p_x = &INDVAR_C_RE(x, j, 0, ds);
			for (i = 0; i < n; i++) {
				a_r = *p_x;
				a_i = *(p_x + 1);
				p_z = &INDVAR_C_RE(z, i, j, ds);
				(*p_x++) = *(p_z);
				*p_z = a_r;
				(*p_x++) = p_z[1];
				p_z[1] = a_i;
			}
		}
		if (n_even_4 < n) {
			for (j = 0; j < m_even_4; j++) {
				p_x = &INDVAR_C_RE(x, j, n_even_4, ds);
				for (i = n_even_4; i < n; i++) {
					a_r = *p_x;
					a_i = *(p_x + 1);
					p_z = &INDVAR_C_RE(z, i, j, ds);
					(*p_x++) = *(p_z);
					*p_z = a_r;
					(*p_x++) = p_z[1];
					p_z[1] = a_i;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
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

	if (!((mlib_s32)xz & 3))

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
	mlib_s32 nm = m;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_s32)z | (mlib_s32)x) & 3))

		return mlib_MatrixTranspose_S_S32((mlib_s32 *)z,
			(mlib_s32 *)x, m, n, ds);
	else if (z == x) {
		TRANSS_DIAG_C(mlib_s16);
	} else {
		TRANSS_DIAG2_C(mlib_s16);
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

	if (!((mlib_s32)xz & 7))
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
	mlib_s32 nm = m;

	if (n <= 0 || m <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_s32)z | (mlib_s32)x) & 7))
		return (mlib_MatrixTranspose_S_D64((mlib_d64 *)z,
			(mlib_d64 *)x, m, n, ds));
	else if (z == x) {
		TRANSS_DIAG_C(mlib_s32);
	} else {
		TRANSS_DIAG2_C(mlib_s32);
	}
}

/* *********************************************************** */
