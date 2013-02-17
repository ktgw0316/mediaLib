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

#pragma ident	"@(#)mlib_v_VectorMulMShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorMulMShift_[S16_S16|S16C_S16C]_[Sat|Mod] -
 *          Multiplication of a signed 16-bit vector by a matrix with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulMShift_S16C_S16C_[Sat|Mod](mlib_s16       * z,
 *                                                           const mlib_s16 * x,
 *                                                           const mlib_s16 * y,
 *                                                           mlib_s32       m,
 *                                                           mlib_s32       n,
 *                                                           mlib_s32   shift);
 *      mlib_status mlib_VectorMulMShift_S16_S16_[Sat|Mod](mlib_s16       * z,
 *                                                         const mlib_s16 * x,
 *                                                         const mlib_s16 * y,
 *                                                         mlib_s32       m,
 *                                                         mlib_s32       n,
 *                                                         mlib_s32   shift);
 *      mlib_status mlib_VectorMulMShift_S16_S16_Mod2(mlib_s16       * z,
 *                                                    const mlib_s16 * x,
 *                                                    const mlib_s16 * y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32   shift);
 *      mlib_status mlib_VectorMulMShift_S16_S16_Sat2(mlib_s16       * z,
 *                                                    const mlib_s16 * x,
 *                                                    const mlib_s16 * y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32   shift);
 *
 * ARGUMENTS
 *     z          the pointer to the first element of the result vector
 *     x          the pointer to the first element of the input vector
 *     y          the pointer to the first element of the input matrix in
 *                row - major order
 *     m          the number of rows in the matrix, and number of elements
 *                in the input vector
 *     n          the number of columns in the matrix, and number of elements
 *                in the result vector
 *     shift      the right shifting factor
 *
 * DESCRIPTION
 *                  m
 *          z[i] = SUM (x[k] * y[k][i]) * 2^(-shift)
 *                 k = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <mlib_VectorMulM_proto.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulMShift_S16_S16_Sat = \
	__mlib_VectorMulMShift_S16_S16_Sat
#pragma weak mlib_VectorMulMShift_S16_S16_Mod = \
	__mlib_VectorMulMShift_S16_S16_Mod
#pragma weak mlib_VectorMulMShift_S16_S16_Sat2 = \
	__mlib_VectorMulMShift_S16_S16_Sat2
#pragma weak mlib_VectorMulMShift_S16_S16_Mod2 = \
	__mlib_VectorMulMShift_S16_S16_Mod2
#pragma weak mlib_VectorMulMShift_S16C_S16C_Sat = \
	__mlib_VectorMulMShift_S16C_S16C_Sat
#pragma weak mlib_VectorMulMShift_S16C_S16C_Mod = \
	__mlib_VectorMulMShift_S16C_S16C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMulMShift_S16_S16_Sat) mlib_VectorMulMShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulMShift_S16_S16_Mod) mlib_VectorMulMShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulMShift_S16_S16_Sat2)
	mlib_VectorMulMShift_S16_S16_Sat2
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Sat2")));
__typeof__(__mlib_VectorMulMShift_S16_S16_Mod2)
	mlib_VectorMulMShift_S16_S16_Mod2
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Mod2")));
__typeof__(__mlib_VectorMulMShift_S16C_S16C_Sat)
	mlib_VectorMulMShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulMShift_S16C_S16C_Mod)
	mlib_VectorMulMShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16C_S16C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	B8SIZE	1024
#define	D64SIZE	(B8SIZE/8)

#define	MAX_SIZE	400

/* *********************************************************** */

#define	MUL_S16(res0, res1, f32, d64)                           \
	{                                                       \
	    mlib_d64 drhi, drlo;                                \
	                                                        \
	    drhi = vis_fmuld8sux16(f32, vis_read_hi(d64));      \
	    drlo = vis_fmuld8ulx16(f32, vis_read_hi(d64));      \
	    res0 = vis_fpadd32(drhi, drlo);                     \
	    drhi = vis_fmuld8sux16(f32, vis_read_lo(d64));      \
	    drlo = vis_fmuld8ulx16(f32, vis_read_lo(d64));      \
	    res1 = vis_fpadd32(drhi, drlo);                     \
	}

/* *********************************************************** */

#define	ADD_S32(sum0, sum1, add0, add1)                         \
	{                                                       \
	    sum0 = vis_fpadd32(sum0, add0);                     \
	    sum1 = vis_fpadd32(sum1, add1);                     \
	}

/* *********************************************************** */

#define	PACK_S32_S16(mem, d0, d1, d2, d3, shift)                \
	{                                                       \
	    mlib_s32 val0, val1;                                \
	                                                        \
	    val0 = d0.int0;                                     \
	    val1 = d0.int1;                                     \
	    mem[0] = (val0 >> shift);                           \
	    mem[1] = (val1 >> shift);                           \
	    val0 = d1.int0;                                     \
	    val1 = d1.int1;                                     \
	    mem[2] = (val0 >> shift);                           \
	    mem[3] = (val1 >> shift);                           \
	    val0 = d2.int0;                                     \
	    val1 = d2.int1;                                     \
	    mem[4] = (val0 >> shift);                           \
	    mem[5] = (val1 >> shift);                           \
	    val0 = d3.int0;                                     \
	    val1 = d3.int1;                                     \
	    mem[6] = (val0 >> shift);                           \
	    mem[7] = (val1 >> shift);                           \
	}

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc >> 16;                         \
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC) >> 16)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Mod2(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	type_union_mlib_d64 array[D64SIZE * 2];

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	if (((mlib_addr)y | (n + n) | (ns + ns)) & 7) {
/*
 * Not aligned data
 */
		mlib_s32 noff = 0, rest;

/* for all in output vector */
		while (1) {
/*
 * check weather array < rest of output vector
 */

			if ((rest = n - noff) < 8) {
				type_union_mlib_d64 sum0, sum1, sum2, sum3;
				mlib_s16 *ypos = (void *)(y + noff);
				mlib_d64 *dpy;
				mlib_s32 row;

				if (rest == 0)
					return (MLIB_SUCCESS);
				sum0.db = 0;
				sum1.db = 0;
				sum2.db = 0;
				sum3.db = 0;

/*
 * last row is dangerous for SIGSEG so:
 */
				for (row = 0; row < m; row++) {
					mlib_f32 f32, fx;
					mlib_d64 dr0, dr1, dr2, dr3, dy0, dy1,
						in0, in1;

					((mlib_s16 *)&f32)[0] = x[row];
					((mlib_s16 *)&f32)[1] = x[row];
					fx = f32;
					dpy = vis_alignaddr(ypos, 0);
					in0 = dpy[0];
					in1 = dpy[1];
					dy0 = vis_faligndata(in0, in1);
					in0 = vis_ld_d64_nf(dpy + 2);
					dy1 = vis_faligndata(in1, in0);
					MUL_S16(dr0, dr1, fx, dy0);
					MUL_S16(dr2, dr3, fx, dy1);
					ADD_S32(sum0.db, sum1.db, dr0, dr1);
					ADD_S32(sum2.db, sum3.db, dr2, dr3);
					ypos += ns;
				}

				z += noff;
				{
					mlib_s32 val0, val1;

					switch (rest) {
					    case 7:
						    val0 = sum3.two_int.int0;
						    z[6] = (val0 >> shift);
					    case 6:
						    val1 = sum2.two_int.int1;
						    z[5] = (val1 >> shift);
					    case 5:
						    val0 = sum2.two_int.int0;
						    z[4] = (val0 >> shift);
					    case 4:
						    val1 = sum1.two_int.int1;
						    z[3] = (val1 >> shift);
					    case 3:
						    val0 = sum1.two_int.int0;
						    z[2] = (val0 >> shift);
					    case 2:
						    val1 = sum0.two_int.int1;
						    z[1] = (val1 >> shift);
					    case 1:
						    val0 = sum0.two_int.int0;
						    z[0] = (val0 >> shift);
					}
				}

				return (MLIB_SUCCESS);
			} else {
				mlib_s32 row, nlimit2, i;
				mlib_s16 *ypos = (void *)(y + noff), *mem;

				rest &= ~7;
				nlimit2 =
					(rest >
					(D64SIZE * 4)) ? (D64SIZE *
					2) : (rest >> 1);

/*
 * freeing array
 */
				for (i = 0; i < nlimit2; i++)
					array[i].db = 0.;

				for (row = 0; row < m; row++) {
					mlib_f32 f32, fx;
					mlib_d64 *dpy, lastx;

					((mlib_s16 *)&f32)[0] = x[row];
					((mlib_s16 *)&f32)[1] = x[row];
					fx = f32;
					dpy = vis_alignaddr(ypos, 0);
					lastx = *dpy;
					dpy++;

#pragma pipeloop(0)
					for (i = 0; i <= (nlimit2 - 4);
					    i += 4) {
						mlib_d64 dr0, dr1, dr2, dr3,
							dy0, dy1, nextx;

						nextx = *dpy;
						dpy++;
						dy0 = vis_faligndata(lastx,
							nextx);
						lastx = vis_ld_d64_nf(dpy);
						dpy++;
						MUL_S16(dr0, dr1, fx, dy0);
						dy1 = vis_faligndata(nextx,
							lastx);
						MUL_S16(dr2, dr3, fx, dy1);
						ADD_S32(array[i].db,
							array[i + 1].db, dr0,
							dr1);
						ADD_S32(array[i + 2].db,
							array[i + 3].db, dr2,
							dr3);
					}

					ypos += ns;
				}

				mem = z + noff;
				for (i = 0; i <= (nlimit2 - 4); i += 4)
					PACK_S32_S16((mem + (i << 1)),
						array[i].two_int,
						array[i + 1].two_int,
						array[i + 2].two_int,
						array[i + 3].two_int, shift);

				noff += (nlimit2 << 1);
			}
		}
	} else {
		mlib_s32 noff = 0, rest;

/* n number of mlib_d64 in output */
		n /= 4;

		while (1) {

			if ((rest = n - noff) < 2) {
				type_union_mlib_d64 res0, res1;
				mlib_d64 *pos = ((mlib_d64 *)y) + noff;
				mlib_s32 row;

				res0.db = 0;
				res1.db = 0;

				if (rest == 0)
					return (MLIB_SUCCESS);

				for (row = 0; row < m; row++) {
					mlib_f32 fx;
					mlib_d64 dr0, dr1, dy;

					((mlib_s16 *)&fx)[0] = x[row];
					((mlib_s16 *)&fx)[1] = x[row];

					dy = *pos;
					MUL_S16(dr0, dr1, fx, dy);
					ADD_S32(res0.db, res1.db, dr0, dr1);
					pos += ns;
				}
/*
 * PACK REST and STORE
 */
				{
					mlib_s32 val0, val1;

					val0 = res0.two_int.int0;
					val1 = res0.two_int.int1;
					(z + (noff << 2))[0] = (val0 >> shift);
					(z + (noff << 2))[1] = (val1 >> shift);
					val0 = res1.two_int.int0;
					val1 = res1.two_int.int1;
					(z + (noff << 2))[2] = (val0 >> shift);
					(z + (noff << 2))[3] = (val1 >> shift);
					return (MLIB_SUCCESS);
				}
			} else {
/* unrolled fast method */
				mlib_s32 nlimit2, i, row;
				mlib_d64 *ypos = ((mlib_d64 *)y) + noff;
				mlib_s16 *mem;

				rest &= ~1;

				nlimit2 =
					(rest >
					D64SIZE ? (D64SIZE * 2) : (rest << 1));
/*
 * freeing array
 */
				for (i = 0; i < nlimit2; i++)
					array[i].db = 0.;

				for (row = 0; row < m; row++) {
					mlib_f32 f32, fx;
					mlib_d64 *xpos = ypos;

					((mlib_s16 *)&f32)[0] = x[row];
					((mlib_s16 *)&f32)[1] = x[row];
					fx = f32;

#pragma pipeloop(0)
					for (i = 0; i <= (nlimit2 - 4);
					    i += 4) {
						mlib_d64 dr0, dr1, dr2, dr3,
							dy0, dy1;

						dy0 = xpos[0];
						dy1 = xpos[1];
						MUL_S16(dr0, dr1, fx, dy0);
						MUL_S16(dr2, dr3, fx, dy1);
						ADD_S32(array[i].db,
							array[i + 1].db, dr0,
							dr1);
						ADD_S32(array[i + 2].db,
							array[i + 3].db, dr2,
							dr3);
						xpos += 2;
					}

					ypos += ns;
				}

				mem = z + (noff << 2);
				for (i = 0; i <= (nlimit2 - 4); i += 4)
					PACK_S32_S16((mem + (i << 1)),
						array[i].two_int,
						array[i + 1].two_int,
						array[i + 2].two_int,
						array[i + 3].two_int, shift);

				noff += nlimit2 >> 1;
			}
		}
	}
}

/* *********************************************************** */

#define	MUL4S16                                                 \
	dr0 = vis_fmuld8sux16(fx, vis_read_hi(dy));             \
	dr1 = vis_fmuld8ulx16(fx, vis_read_hi(dy));             \
	dr2 = vis_fmuld8sux16(fx, vis_read_lo(dy));             \
	dr3 = vis_fmuld8ulx16(fx, vis_read_lo(dy))

/* *********************************************************** */

#define	SUM1                                                    \
	sum0.db = vis_fpadd32(sum0.db, dr0);                    \
	sum1.db = vis_fpadd32(sum1.db, dr2);                    \
	sum0.db = vis_fpadd32(sum0.db, dr1);                    \
	sum1.db = vis_fpadd32(sum1.db, dr3)

/* *********************************************************** */

#define	PACK4S16                                                \
	z[0] = (sum0.two_int.int0) >> shift;                    \
	z[1] = (sum0.two_int.int1) >> shift;                    \
	z[2] = (sum1.two_int.int0) >> shift;                    \
	z[3] = (sum1.two_int.int1) >> shift

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_d64 array[MAX_SIZE], *dpx;
	mlib_d64 dr0, dr1, dr2, dr3, dy, dy_hi, dy_lo;
	type_union_mlib_d64 sum0, sum1;
	mlib_f32 fx;
	mlib_s32 size, i, num;
	mlib_addr pzend;
	mlib_s16 *py, *py1;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	size = (m >> 1) + 1;

	if (size > MAX_SIZE) {
		dpx = __mlib_malloc(size << 3);

		if (!dpx)
			return __mlib_VectorMulMShift_S16_S16_Mod2(z, x, y, m,
				n, ns, shift);
	} else
		dpx = array;

	for (i = 0; i < m; i++)
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] = x[i];

	if (!((mlib_addr)y & 7 || n & 3 || ns & 3)) {
/*
 * aligned
 */
		pzend = (mlib_addr)(z + n);
		while ((mlib_addr)z < pzend) {
			py = (void *)y;
			sum0.db = sum1.db = 0.;

#pragma pipeloop(0)
			for (i = 0; i < m; i++) {
				dy = *(mlib_d64 *)py;
				fx = ((mlib_f32 *)dpx)[i];
				MUL4S16;
				SUM1;
				py += ns;
			}

			PACK4S16;
			y += 4;
			z += 4;
		}
	} else {
		pzend = (mlib_addr)(z + n - 4);
		while ((mlib_addr)z <= pzend) {
			sum0.db = sum1.db = 0.;
			py = (void *)y;
			py1 = vis_alignaddr((void *)y, 0);

#pragma pipeloop(0)
			for (i = 0; i < m; i++) {
				dy_hi = ((mlib_d64 *)py1)[0];
				dy_lo = vis_ld_d64_nf((mlib_d64 *)py1 + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				MUL4S16;
				SUM1;
				py += ns;
				py1 = vis_alignaddr(py, 0);
			}

			PACK4S16;
			y += 4;
			z += 4;
		}

		num = z - (mlib_s16 *)pzend;

		if (num) {
			sum0.db = sum1.db = 0.;
			py = (void *)y;
			py1 = vis_alignaddr((void *)y, 0);

#pragma pipeloop(0)
			for (i = 0; i < m; i++) {
				dy_hi = ((mlib_d64 *)py1)[0];
				dy_lo = vis_ld_d64_nf((mlib_d64 *)py1 + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				MUL4S16;
				SUM1;
				py += ns;
				py1 = vis_alignaddr(py, 0);
			}

			switch (num) {
			    case 1:
				    z[2] = (sum1.two_int.int0) >> shift;
			    case 2:
				    z[1] = (sum0.two_int.int1) >> shift;
			    case 3:
				    z[0] = (sum0.two_int.int0) >> shift;
			}
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	ADD_D64(sum0, sum1, sum2, sum3, add0, add1)             \
	{                                                       \
	    sum0 += (mlib_d64)(add0.two_int.int0);              \
	    sum1 += (mlib_d64)(add0.two_int.int1);              \
	    sum2 += (mlib_d64)(add1.two_int.int0);              \
	    sum3 += (mlib_d64)(add1.two_int.int1);              \
	}

/* *********************************************************** */

#define	PACK_D64_S16(mem, s0, s1, s2, s3, shift)                \
	{                                                       \
	    type_union_mlib_d64 dr0, dr1;                       \
	                                                        \
	    dr0.two_int.int0 = (mlib_s32)s0;                    \
	    dr0.two_int.int1 = (mlib_s32)s1;                    \
	    dr1.two_int.int0 = (mlib_s32)s2;                    \
	    dr1.two_int.int1 = (mlib_s32)s3;                    \
	    ((mlib_f32 *)(mem))[0] = vis_fpackfix(dr0.db);      \
	    ((mlib_f32 *)(mem))[1] = vis_fpackfix(dr1.db);      \
	}

/* *********************************************************** */

#define	PACK_D64_S16N(mem, sum0, sum1, sum2, sum3, shift)       \
	mem[0] = ((mlib_s32)(sum0 * mul)) >> 16;                \
	mem[1] = ((mlib_s32)(sum1 * mul)) >> 16;                \
	mem[2] = ((mlib_s32)(sum2 * mul)) >> 16;                \
	mem[3] = ((mlib_s32)(sum3 * mul)) >> 16

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Sat2(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_d64 array[D64SIZE * 2];

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	vis_write_gsr((16 - shift) << 3);

	if (((((mlib_addr)z | n | ns) << 1) | (mlib_addr)y) & 7) {
/*
 * Not aligned data
 */
		mlib_s32 noff = 0, rest;
		mlib_d64 mul = (mlib_d64)(1 << (16 - shift));

		while (1) {

			if ((rest = n - noff) < 8) {
/*
 * last 2 cols
 */
				mlib_d64 sum0, sum1, sum2, sum3, sum4, sum5,
					sum6, sum7, *dpy;
				mlib_s32 row;
				mlib_s16 *ypos = (void *)(y + noff);

				if (rest == 0)
					return (MLIB_SUCCESS);

				sum0 = 0;
				sum1 = 0;
				sum2 = 0;
				sum3 = 0;
				sum4 = 0;
				sum5 = 0;
				sum6 = 0;
				sum7 = 0;
				for (row = 0; row < m; row++) {
					mlib_f32 f32, fx;
					type_union_mlib_d64 dr0, dr1, dr2, dr3;
					mlib_d64 dy0, dy1, in0, in1;

					((mlib_s16 *)&f32)[0] = x[row];
					((mlib_s16 *)&f32)[1] = x[row];
					fx = f32;
					dpy = vis_alignaddr(ypos, 0);
					in0 = dpy[0];
					in1 = dpy[1];
					dy0 = vis_faligndata(in0, in1);
					in0 = vis_ld_d64_nf(dpy + 2);
					dy1 = vis_faligndata(in1, in0);
					MUL_S16(dr0.db, dr1.db, fx, dy0);
					MUL_S16(dr2.db, dr3.db, fx, dy1);
					ADD_D64(sum0, sum1, sum2, sum3, dr0,
						dr1);
					ADD_D64(sum4, sum5, sum6, sum7, dr2,
						dr3);
					ypos += ns;
				}

				z += noff;
				switch (rest) {
				    case 7:
					    z[6] = ((mlib_s32)(sum6 *
						    mul)) >> 16;
				    case 6:
					    z[5] = ((mlib_s32)(sum5 *
						    mul)) >> 16;
				    case 5:
					    z[4] = ((mlib_s32)(sum4 *
						    mul)) >> 16;
				    case 4:
					    z[3] = ((mlib_s32)(sum3 *
						    mul)) >> 16;
				    case 3:
					    z[2] = ((mlib_s32)(sum2 *
						    mul)) >> 16;
				    case 2:
					    z[1] = ((mlib_s32)(sum1 *
						    mul)) >> 16;
				    case 1:
					    z[0] = ((mlib_s32)(sum0 *
						    mul)) >> 16;
				}

				return (MLIB_SUCCESS);
			} else {
				mlib_s32 row, nlimit2, i;
				mlib_s16 *ypos = (void *)(y + noff), *mem;

				rest &= ~7;
				nlimit2 =
					(rest >
					(D64SIZE << 1) ? (D64SIZE << 1) : rest);
				for (i = 0; i < nlimit2; i++)
					array[i] = 0.;

				for (row = 0; row < m; row++) {
					mlib_f32 f32, fx;
					mlib_d64 *dpy, lastx;

					((mlib_s16 *)&f32)[0] = x[row];
					((mlib_s16 *)&f32)[1] = x[row];
					fx = f32;
					dpy = vis_alignaddr(ypos, 0);
					lastx = *dpy;
					dpy++;

#pragma pipeloop(0)
					for (i = 0; i <= (nlimit2 - 8);
					    i += 8) {
						mlib_d64 dy0, dy1, nextx;
						type_union_mlib_d64 dr0, dr1,
							dr2, dr3;

						nextx = *dpy;
						dpy++;
						dy0 = vis_faligndata(lastx,
							nextx);
						lastx = vis_ld_d64_nf(dpy);
						dpy++;
						MUL_S16(dr0.db, dr1.db, fx,
							dy0);
						dy1 = vis_faligndata(nextx,
							lastx);
						MUL_S16(dr2.db, dr3.db, fx,
							dy1);
						ADD_D64(array[i], array[i + 1],
							array[i + 2],
							array[i + 3], dr0, dr1);
						ADD_D64(array[i + 4],
							array[i + 5],
							array[i + 6],
							array[i + 7], dr2, dr3);
					}

					ypos += ns;
				}

				mem = z + noff;
				for (i = 0; i <= (nlimit2 - 8); i += 8) {
					PACK_D64_S16N((mem + i), array[i],
						array[i + 1], array[i + 2],
						array[i + 3], shift);
					PACK_D64_S16N((mem + i + 4),
						array[i + 4], array[i + 5],
						array[i + 6], array[i + 7],
						shift);
				}
				noff += nlimit2;
			}
		}
	} else {
/*
 * aligned data
 */
		mlib_s32 noff = 0, rest;

		while (1) {

			if ((rest = n - noff) < 4) {
				return (MLIB_SUCCESS);
			} else {
/* unrolled fast method */
				mlib_s32 nlimit2, i, row;
				mlib_s16 *ypos = (void *)(y + noff), *mem;

				rest &= ~1;

				nlimit2 =
					(rest >
					(D64SIZE * 2) ? (D64SIZE * 2) : rest);

				for (i = 0; i < nlimit2; i++)
					array[i] = 0.;

				for (row = 0; row < m; row++) {
					mlib_f32 f32, fx;
					mlib_d64 *xpos = (mlib_d64 *)ypos;

					((mlib_s16 *)&f32)[0] = x[row];
					((mlib_s16 *)&f32)[1] = x[row];
					fx = f32;

#pragma pipeloop(0)
					for (i = 0; i <= (nlimit2 - 4);
					    i += 4) {
						type_union_mlib_d64 dr0, dr1;
						mlib_d64 dy0;

						dy0 = *xpos;
						MUL_S16(dr0.db, dr1.db, fx,
							dy0);
						ADD_D64(array[i], array[i + 1],
							array[i + 2],
							array[i + 3], dr0, dr1);
						xpos++;
					}

					ypos += ns;
				}

				mem = z + noff;
				for (i = 0; i <= (nlimit2 - 4); i += 4)
					PACK_D64_S16((mem + i), array[i],
						array[i + 1], array[i + 2],
						array[i + 3], shift);

				noff += nlimit2;
			}
		}
	}
}

/* *********************************************************** */

#define	SUM2                                                    \
	ud0.db = vis_fpadd32(dr0, dr1);                         \
	ud1.db = vis_fpadd32(dr2, dr3);                         \
	sum0 += (mlib_d64)(ud0.two_int.int0);                   \
	sum1 += (mlib_d64)(ud0.two_int.int1);                   \
	sum2 += (mlib_d64)(ud1.two_int.int0);                   \
	sum3 += (mlib_d64)(ud1.two_int.int1)

/* *********************************************************** */

#define	PACK4S16SAT                                             \
	FLOAT2INT_CLAMP(z[0], sum0 * mul);                      \
	FLOAT2INT_CLAMP(z[1], sum1 * mul);                      \
	FLOAT2INT_CLAMP(z[2], sum2 * mul);                      \
	FLOAT2INT_CLAMP(z[3], sum3 * mul)

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 size, i;
	mlib_d64 array[MAX_SIZE], *dpx;
	mlib_d64 sum0, sum1, sum2, sum3;
	mlib_d64 dy, dy_hi, dy_lo, dr0, dr1, dr2, dr3;
	mlib_f32 fx;
	mlib_s16 *py;
	mlib_d64 *dpy;
	type_union_mlib_d64 ud0, ud1;
	mlib_addr pzend;
	mlib_d64 mul = (mlib_d64)(1 << (16 - shift));

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	size = (m >> 1) + 1;

	if (size > MAX_SIZE) {
		dpx = __mlib_malloc(size << 3);

		if (!dpx)
			return __mlib_VectorMulMShift_S16_S16_Sat2(z, x, y, m,
				n, ns, shift);
	} else
		dpx = array;

	for (i = 0; i < m; i++)
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] = x[i];

	if (!(((((mlib_addr)z | n | ns) << 1) | (mlib_addr)y) & 7)) {
/*
 * aligned
 */

		pzend = (mlib_addr)(z + n);

		while ((mlib_addr)z < pzend) {
			py = (void *)y;
			sum0 = sum1 = sum2 = sum3 = 0.;

#pragma pipeloop(0)
			for (i = 0; i < m; i++) {
				dy = *(mlib_d64 *)py;
				fx = ((mlib_f32 *)dpx)[i];
				MUL4S16;
				SUM2;
				py += ns;
			}

			PACK4S16SAT;
			y += 4;
			z += 4;
		}
	} else {
/*
 * not aligned
 */
		pzend = (mlib_addr)(z + n - 4);

		while ((mlib_addr)z <= pzend) {
			py = (void *)y;
			sum0 = sum1 = sum2 = sum3 = 0.;

			dpy = vis_alignaddr(py, 0);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			fx = *((mlib_f32 *)dpx);
			dy = vis_faligndata(dy_hi, dy_lo);
			MUL4S16;
			py += ns;

#pragma pipeloop(0)
			for (i = 1; i < m; i++) {
				dpy = vis_alignaddr(py, 0);
				SUM2;
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				fx = ((mlib_f32 *)dpx)[i];
				dy = vis_faligndata(dy_hi, dy_lo);
				MUL4S16;
				py += ns;
			}

			SUM2;
			PACK4S16SAT;
			y += 4;
			z += 4;
		}

		pzend = z - (mlib_s16 *)pzend;

		if (pzend) {
			py = (void *)y;
			sum0 = sum1 = sum2 = sum3 = 0.;

			dpy = vis_alignaddr(py, 0);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			fx = *((mlib_f32 *)dpx);
			dy = vis_faligndata(dy_hi, dy_lo);
			MUL4S16;
			py += ns;

#pragma pipeloop(0)
			for (i = 1; i < m; i++) {
				dpy = vis_alignaddr(py, 0);
				SUM2;
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				fx = ((mlib_f32 *)dpx)[i];
				dy = vis_faligndata(dy_hi, dy_lo);
				MUL4S16;
				py += ns;
			}

			SUM2;
			switch (pzend) {
			    case 1:
				    FLOAT2INT_CLAMP(z[2], sum2 * mul);
			    case 2:
				    FLOAT2INT_CLAMP(z[1], sum1 * mul);
			    case 3:
				    FLOAT2INT_CLAMP(z[0], sum0 * mul);
			}
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_VectorMulMShift_S16C_S16C_Mod_slow(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		mlib_d64 dz_r = 0, dz_i = 0;

#pragma pipeloop(0)
		for (j = 0; j < m; j++) {
			mlib_s32 a = (j * ns + i) * 2;

			dz_r += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a]) -
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a + 1]);
			dz_i += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a + 1]) +
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a]);
		}

		z[i * 2] = mlib_D64_to_S32_Mod(dz_r) >> shift;
		z[i * 2 + 1] = mlib_D64_to_S32_Mod(dz_i) >> shift;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL8S16C                                                   \
	dr0 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_hi(dy));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_hi(dy));   \
	dr4 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_lo(dy));   \
	dr5 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_lo(dy));   \
	dr6 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy))

/* *********************************************************** */

#define	MUL4S16C                                                   \
	dr0 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_hi(dy));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_hi(dy))

/* *********************************************************** */

#define	SUM4S16C                                                \
	sum0 = vis_fpadd32(sum0, dr0);                          \
	sum1 = vis_fpadd32(sum1, dr2);                          \
	sum2 = vis_fpadd32(sum2, dr4);                          \
	sum3 = vis_fpadd32(sum3, dr6);                          \
	sum0 = vis_fpadd32(sum0, dr1);                          \
	sum1 = vis_fpadd32(sum1, dr3);                          \
	sum2 = vis_fpadd32(sum2, dr5);                          \
	sum3 = vis_fpadd32(sum3, dr7)

/* *********************************************************** */

#define	SUM2S16C                                                \
	sum0 = vis_fpadd32(sum0, dr0);                          \
	sum1 = vis_fpadd32(sum1, dr2);                          \
	sum0 = vis_fpadd32(sum0, dr1);                          \
	sum1 = vis_fpadd32(sum1, dr3)

/* *********************************************************** */

#define	PACK4S16C                                                  \
	f0 = vis_fpsub32s(vis_read_hi(sum0), vis_read_lo(sum0));   \
	f1 = vis_fpadd32s(vis_read_hi(sum1), vis_read_lo(sum1));   \
	f2 = vis_fpsub32s(vis_read_hi(sum2), vis_read_lo(sum2));   \
	f3 = vis_fpadd32s(vis_read_hi(sum3), vis_read_lo(sum3));   \
	pz[0] = (*(mlib_s32 *)&f0) >> shift;                       \
	pz[1] = (*(mlib_s32 *)&f1) >> shift;                       \
	pz[2] = (*(mlib_s32 *)&f2) >> shift;                       \
	pz[3] = (*(mlib_s32 *)&f3) >> shift

/* *********************************************************** */

#define	PACK2S16C                                                  \
	f0 = vis_fpsub32s(vis_read_hi(sum0), vis_read_lo(sum0));   \
	f1 = vis_fpadd32s(vis_read_hi(sum1), vis_read_lo(sum1));   \
	pz[0] = (*(mlib_s32 *)&f0) >> shift;                       \
	pz[1] = (*(mlib_s32 *)&f1) >> shift

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_d64 sum0, sum1, sum2, sum3;
	mlib_d64 dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
	mlib_s32 row, i;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 dx, dy, dy_hi, dy_lo, *dpx;
	mlib_addr pzend;
	mlib_s16 *py, *py1, *pz;
	mlib_f32 f0, f1, f2, f3;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	if (m > MAX_SIZE) {
		dpx = __mlib_malloc(m << 3);

		if (!dpx)
			return mlib_VectorMulMShift_S16C_S16C_Mod_slow(z, x, y,
				m, n, ns, shift);
	} else
		dpx = array;

	pz = z;
	pzend = (mlib_addr)(z + n + n - 2);

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[4 * i] = ((mlib_s16 *)dpx)[4 * i + 3] =
			((mlib_s16 *)x)[2 * i];
		((mlib_s16 *)dpx)[4 * i + 1] = ((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)x)[2 * i + 1];
	}

	if (!((mlib_addr)y & 7 || (n & 1) || (ns & 1))) {
/*
 * aligned data
 */

		while ((mlib_addr)pz < pzend) {
			py = (void *)y;
			sum0 = sum1 = sum2 = sum3 = 0.;
			dy = *(mlib_d64 *)py;
			dx = *dpx;
			MUL8S16C;
			py += 2 * ns;

#pragma pipeloop(0)
			for (row = 1; row < m; row++) {
				SUM4S16C;
				dy = *(mlib_d64 *)py;
				dx = dpx[row];
				MUL8S16C;
				py += 2 * ns;
			}

			SUM4S16C;
			PACK4S16C;
			y += 4;
			pz += 4;
		}
	} else {
/*
 * not aligned data
 */

		while ((mlib_addr)pz < pzend) {
			sum0 = sum1 = sum2 = sum3 = 0.;

			py = (void *)y;
			py1 = vis_alignaddr((void *)y, 0);
			dy_hi = ((mlib_d64 *)py1)[0];
			dy_lo = vis_ld_d64_nf((mlib_d64 *)py1 + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx = *dpx;
			MUL8S16C;
			py += ns * 2;

			for (row = 1; row < m; row++) {
				SUM4S16C;
				py1 = vis_alignaddr(py, 0);
				dy_hi = ((mlib_d64 *)py1)[0];
				dy_lo = vis_ld_d64_nf((mlib_d64 *)py1 + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[row];
				MUL8S16C;
				py += ns * 2;
			}

			SUM4S16C;
			PACK4S16C;
			y += 4;
			pz += 4;
		}

		if ((mlib_addr)pz == pzend) {
			py = (void *)y;
			py1 = vis_alignaddr((void *)y, 0);
			dy_hi = ((mlib_d64 *)py1)[0];
			dy_lo = vis_ld_d64_nf((mlib_d64 *)py1 + 1);
			dy = vis_faligndata(dy_hi, dy_lo);

			sum0 = sum1 = sum2 = sum3 = 0.;
			dx = *dpx;
			MUL4S16C;
			py += 2 * ns;

#pragma pipeloop(0)
			for (row = 1; row < m; row++) {
				SUM2S16C;
				py1 = vis_alignaddr(py, 0);
				dy_hi = ((mlib_d64 *)py1)[0];
				dy_lo = vis_ld_d64_nf((mlib_d64 *)py1 + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[row];
				MUL4S16C;
				py += 2 * ns;
			}

			SUM2S16C;
			PACK2S16C;
		}
	}

	if (m > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	LD1(reg, addr)                                          \
	{                                                       \
	    type_union_mlib_d64 tu;                             \
	                                                        \
	    tu.db = vis_ld_u16((void *)(addr));                 \
	    tu.db = vis_fmuld8ulx16(one, vis_read_lo(tu.db));   \
	    reg = (mlib_d64)tu.two_int.int1;                    \
	}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_d64 divisor = 1 << (16 - shift);
	mlib_f32 one;
	mlib_s32 ns2;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	ns2 = ns << 1;
	one = vis_to_float(0x00010001);

	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0_r = 0.;
		mlib_d64 dz0_i = 0.;
		mlib_d64 dz1_r = 0.;
		mlib_d64 dz1_i = 0.;
		mlib_d64 dz2_r = 0.;
		mlib_d64 dz2_i = 0.;
		mlib_d64 dz3_r = 0.;
		mlib_d64 dz3_i = 0.;
		mlib_s16 *addr = (mlib_s16 *)y + (i << 1);

#pragma pipeloop(0)
		for (j = 0; j < m; j++) {
			mlib_d64 dxj_r, dxj_i;
			mlib_d64 dy0_r, dy0_i, dy1_r, dy1_i;
			mlib_d64 dy2_r, dy2_i, dy3_r, dy3_i;

			LD1(dxj_r, x + j * 2);
			LD1(dxj_i, x + j * 2 + 1);
			LD1(dy0_r, addr + 0);
			LD1(dy0_i, addr + 1);
			LD1(dy1_r, addr + 2);
			LD1(dy1_i, addr + 3);
			LD1(dy2_r, addr + 4);
			LD1(dy2_i, addr + 5);
			LD1(dy3_r, addr + 6);
			LD1(dy3_i, addr + 7);

			addr += ns2;

			dz0_r += dxj_r * dy0_r - dxj_i * dy0_i;
			dz0_i += dxj_r * dy0_i + dxj_i * dy0_r;
			dz1_r += dxj_r * dy1_r - dxj_i * dy1_i;
			dz1_i += dxj_r * dy1_i + dxj_i * dy1_r;
			dz2_r += dxj_r * dy2_r - dxj_i * dy2_i;
			dz2_i += dxj_r * dy2_i + dxj_i * dy2_r;
			dz3_r += dxj_r * dy3_r - dxj_i * dy3_i;
			dz3_i += dxj_r * dy3_i + dxj_i * dy3_r;
		}

		FLOAT2INT_CLAMP(z[i * 2 + 0], dz0_r * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 1], dz0_i * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 2], dz1_r * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 3], dz1_i * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 4], dz2_r * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 5], dz2_i * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 6], dz3_r * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 7], dz3_i * divisor);
	}

	for (; i < n; i++) {
		mlib_d64 dz_r = 0, dz_i = 0;

#pragma pipeloop(0)
		for (j = 0; j < m; j++) {
			mlib_s32 a = (j * ns + i) * 2;

			dz_r += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a]) -
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a + 1]);
			dz_i += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a + 1]) +
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a]);
		}

		FLOAT2INT_CLAMP(z[i * 2], dz_r * divisor);
		FLOAT2INT_CLAMP(z[i * 2 + 1], dz_i * divisor);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16_S16_Sat(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16_S16_Mod(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16C_S16C_Sat(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16C_S16C_Mod(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */
