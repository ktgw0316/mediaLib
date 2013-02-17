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

#pragma ident	"@(#)mlib_c_VectorDotProd8.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_U8_Sat - dot product of unsigned 8-bit format vectors
 *      mlib_VectorDotProd_S8_Sat - dot product of signed 8-bit format vectors
 *
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_U8_Sat(mlib_d64      *z,
 *                                           const mlib_u8 *x,
 *                                           const mlib_u8 *y,
 *                                           mlib_s32      n);
 *     mlib_status mlib_VectorDotProd_S8_Sat(mlib_d64      *z,
 *                                           const mlib_s8 *x,
 *                                           const mlib_s8 *y,
 *                                           mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *           n
 *      z = SUM (x[i] * y[i])
 *          i = 1
 */

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_U8C_Sat - dot product of unsigned 8-bit format
 *                                   complex vectors
 *      mlib_VectorDotProd_S8C_Sat - dot product of signed 8-bit format
 *                                   complex vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_U8C_Sat(mlib_d64      *z,
 *                                            const mlib_u8 *x,
 *                                            const mlib_u8 *y,
 *                                            mlib_s32      n);
 *     mlib_status mlib_VectorDotProd_S8C_Sat(mlib_d64      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors. z[0] hold the
 *           real parts, and z[1] hold the imaginary parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *           n              *
 *      z = SUM (x[i] * y[i] )
 *          i = 1
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_U8C_Sat = __mlib_VectorDotProd_U8C_Sat
#pragma weak mlib_VectorDotProd_S8_Sat = __mlib_VectorDotProd_S8_Sat
#pragma weak mlib_VectorDotProd_U8_Sat = __mlib_VectorDotProd_U8_Sat
#pragma weak mlib_VectorDotProd_S8C_Sat = __mlib_VectorDotProd_S8C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_U8C_Sat) mlib_VectorDotProd_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_U8C_Sat")));
__typeof__(__mlib_VectorDotProd_S8_Sat) mlib_VectorDotProd_S8_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S8_Sat")));
__typeof__(__mlib_VectorDotProd_U8_Sat) mlib_VectorDotProd_U8_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_U8_Sat")));
__typeof__(__mlib_VectorDotProd_S8C_Sat) mlib_VectorDotProd_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S8C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i, *wpx, *wpy;
	mlib_d64 dot = 0;
	mlib_u8 *px, *py;
	mlib_s32 lx0, lx1, lx2, lx3;
	mlib_s32 ly0, ly1, ly2, ly3;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;

	while (((mlib_addr)py & 3) && (n > 0)) {
		dot += (*px++) * (*py++);
		n--;
	}

	if ((((mlib_addr)px) & 3) == 0) {
		wpx = (mlib_s32 *)px;
		wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_u32 ldx = wpx[i];
			mlib_u32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
			lx0 = ldx & 0xFF;
			lx1 = (ldx >> 8) & 0xFF;
			lx2 = (ldx >> 16) & 0xFF;
			lx3 = ldx >> 24;
			ly0 = (ldy << 24) >> 24;
			ly1 = (ldy << 16) >> 24;
			ly2 = (ldy << 8) >> 24;
			ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
			lx0 = ldx >> 24;
			lx1 = (ldx >> 16) & 0xFF;
			lx2 = (ldx >> 8) & 0xFF;
			lx3 = ldx & 0xFF;
			ly0 = ldy >> 24;
			ly1 = (ldy << 8) >> 24;
			ly2 = (ldy << 16) >> 24;
			ly3 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
			dot += lx0 * (mlib_d64)ly0;
			dot += lx1 * (mlib_d64)ly1;
			dot += lx2 * (mlib_d64)ly2;
			dot += lx3 * (mlib_d64)ly3;
		}

		px = (mlib_u8 *)(wpx + i);
		py = (mlib_u8 *)(wpy + i);
	} else {
		wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_u32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
			ly0 = (ldy << 24) >> 24;
			ly1 = (ldy << 16) >> 24;
			ly2 = (ldy << 8) >> 24;
			ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
			ly0 = ldy >> 24;
			ly1 = (ldy << 8) >> 24;
			ly2 = (ldy << 16) >> 24;
			ly3 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
			dot += px[0] * (mlib_d64)ly0;
			dot += px[1] * (mlib_d64)ly1;
			dot += px[2] * (mlib_d64)ly2;
			dot += px[3] * (mlib_d64)ly3;
			px += 4;
		}

		py = (mlib_u8 *)(wpy + i);
	}

	i = i << 2;
	for (; i < n; i++) {
		dot += *px * (mlib_d64)(*py);
		px++;
		py++;
	}

	z[0] = (mlib_d64)dot;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i, *wpx, *wpy;
	mlib_d64 dot = 0;
	mlib_s8 *px, *py;
	mlib_s32 lx0, lx1, lx2, lx3;
	mlib_s32 ly0, ly1, ly2, ly3;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;

	while (((mlib_addr)py & 3) && (n > 0)) {
		dot += (*px++) * (mlib_d64)(*py++);
		n--;
	}

	if ((((mlib_addr)px) & 3) == 0) {
		wpx = (mlib_s32 *)px;
		wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_s32 ldx = wpx[i];
			mlib_s32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
			lx0 = (ldx << 24) >> 24;
			lx1 = (ldx << 16) >> 24;
			lx2 = (ldx << 8) >> 24;
			lx3 = ldx >> 24;
			ly0 = (ldy << 24) >> 24;
			ly1 = (ldy << 16) >> 24;
			ly2 = (ldy << 8) >> 24;
			ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
			lx0 = ldx >> 24;
			lx1 = (ldx << 8) >> 24;
			lx2 = (ldx << 16) >> 24;
			lx3 = (ldx << 24) >> 24;
			ly0 = ldy >> 24;
			ly1 = (ldy << 8) >> 24;
			ly2 = (ldy << 16) >> 24;
			ly3 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
			dot += lx0 * (mlib_d64)ly0;
			dot += lx1 * (mlib_d64)ly1;
			dot += lx2 * (mlib_d64)ly2;
			dot += lx3 * (mlib_d64)ly3;
		}

		px = (mlib_s8 *)(wpx + i);
		py = (mlib_s8 *)(wpy + i);
	} else {
		wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			mlib_s32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
			ly0 = (ldy << 24) >> 24;
			ly1 = (ldy << 16) >> 24;
			ly2 = (ldy << 8) >> 24;
			ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
			ly0 = ldy >> 24;
			ly1 = (ldy << 8) >> 24;
			ly2 = (ldy << 16) >> 24;
			ly3 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
			dot += px[0] * (mlib_d64)ly0;
			dot += px[1] * (mlib_d64)ly1;
			dot += px[2] * (mlib_d64)ly2;
			dot += px[3] * (mlib_d64)ly3;
			px += 4;
		}

		py = (mlib_s8 *)(wpy + i);
	}

	i = i << 2;
	for (; i < n; i++) {
		dot += *px * (mlib_d64)(*py);
		px++;
		py++;
	}

	z[0] = (mlib_d64)dot;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_U8C_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dot_r = 0, dot_i = 0;
	mlib_u8 *px, *py;
	mlib_s32 lx0, lx1, lx2, lx3;
	mlib_s32 ly0, ly1, ly2, ly3;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;

/* (Xr + jXi) * (Yr - jYi) = (XrYr + XiYi) + j(XiYr - XrYi) */

	if ((((mlib_addr)py) & 1) == 0) {
		if ((((mlib_addr)py) & 3) != 0) {
			lx0 = px[0];
			lx1 = px[1];
			ly0 = py[0];
			ly1 = py[1];
			dot_r += lx0 * (mlib_d64)ly0 + lx1 * (mlib_d64)ly1;
			dot_i += lx1 * (mlib_d64)ly0 - lx0 * (mlib_d64)ly1;
			px += 2;
			py += 2;
			n--;
		}

		if ((((mlib_addr)px) & 3) == 0) {
			mlib_s32 *wpx = (mlib_s32 *)px;
			mlib_s32 *wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_u32 ldx = wpx[i];
				mlib_u32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
				lx0 = ldx & 0xFF;
				lx1 = (ldx >> 8) & 0xFF;
				lx2 = (ldx >> 16) & 0xFF;
				lx3 = ldx >> 24;
				ly0 = ldy & 0xFF;
				ly1 = (ldy >> 8) & 0xFF;
				ly2 = (ldy >> 16) & 0xFF;
				ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				lx0 = ldx >> 24;
				lx1 = (ldx >> 16) & 0xFF;
				lx2 = (ldx >> 8) & 0xFF;
				lx3 = ldx & 0xFF;
				ly0 = ldy >> 24;
				ly1 = (ldy >> 16) & 0xFF;
				ly2 = (ldy >> 8) & 0xFF;
				ly3 = ldy & 0xFF;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
			}

			i = i << 1;

			if (i < n) {
				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
				ly0 = py[2 * i];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}

		} else {
			mlib_s32 *wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_u32 ldy = wpy[i];

				lx0 = px[4 * i];
				lx1 = px[4 * i + 1];
				lx2 = px[4 * i + 2];
				lx3 = px[4 * i + 3];
#ifdef _LITTLE_ENDIAN
				ly0 = ldy & 0xFF;
				ly1 = (ldy >> 8) & 0xFF;
				ly2 = (ldy >> 16) & 0xFF;
				ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				ly0 = ldy >> 24;
				ly1 = (ldy >> 16) & 0xFF;
				ly2 = (ldy >> 8) & 0xFF;
				ly3 = ldy & 0xFF;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
			}

			i = i << 1;

			if (i < n) {
				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
				ly0 = py[2 * i];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}
		}

	} else {
/* (((mlib_addr)py) & 1) != 0 */

		if ((((mlib_addr)py) & 3) == 1) {
			lx0 = px[0];
			lx1 = px[1];
			ly0 = py[0];
			ly1 = py[1];
			dot_r += lx0 * (mlib_d64)ly0 + lx1 * (mlib_d64)ly1;
			dot_i += lx1 * (mlib_d64)ly0 - lx0 * (mlib_d64)ly1;
			px += 2;
			py += 2;
			n--;
		}

		if (((mlib_addr)px & 3) == 3) {
			mlib_s32 *wpx = (mlib_s32 *)(px + 1);
			mlib_s32 *wpy = (mlib_s32 *)(py + 1);

			lx0 = px[0];
			ly0 = py[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_u32 ldx = wpx[i];
				mlib_u32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
				lx1 = ldx & 0xFF;
				lx2 = (ldx >> 8) & 0xFF;
				lx3 = (ldx >> 16) & 0xFF;
				ly1 = ldy & 0xFF;
				ly2 = (ldy >> 8) & 0xFF;
				ly3 = (ldy >> 16) & 0xFF;
#else /* _LITTLE_ENDIAN */
				lx1 = ldx >> 24;
				lx2 = (ldx >> 16) & 0xFF;
				lx3 = (ldx >> 8) & 0xFF;
				ly1 = ldy >> 24;
				ly2 = (ldy >> 16) & 0xFF;
				ly3 = (ldy >> 8) & 0xFF;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
#ifdef _LITTLE_ENDIAN
				lx0 = ldx >> 24;
				ly0 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				lx0 = ldx & 0xFF;
				ly0 = ldy & 0xFF;
#endif /* _LITTLE_ENDIAN */
			}

			i = i << 1;

			if (i < n) {
				lx1 = px[2 * i + 1];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}

		} else {
			mlib_s32 *wpy = (mlib_s32 *)(py + 1);

			ly0 = py[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_u32 ldy = wpy[i];

				lx0 = px[4 * i];
				lx1 = px[4 * i + 1];
				lx2 = px[4 * i + 2];
				lx3 = px[4 * i + 3];
#ifdef _LITTLE_ENDIAN
				ly1 = ldy & 0xFF;
				ly2 = (ldy >> 8) & 0xFF;
				ly3 = (ldy >> 16) & 0xFF;
#else /* _LITTLE_ENDIAN */
				ly1 = ldy >> 24;
				ly2 = (ldy >> 16) & 0xFF;
				ly3 = (ldy >> 8) & 0xFF;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
#ifdef _LITTLE_ENDIAN
				ly0 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				ly0 = ldy & 0xFF;
#endif /* _LITTLE_ENDIAN */
			}

			i = i << 1;

			if (i < n) {
				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}
		}
	}

	z[0] = (mlib_d64)dot_r;
	z[1] = (mlib_d64)dot_i;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S8C_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dot_r = 0, dot_i = 0;
	mlib_s8 *px, *py;
	mlib_s32 lx0, lx1, lx2, lx3;
	mlib_s32 ly0, ly1, ly2, ly3;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;

/* (Xr + jXi) * (Yr - jYi) = (XrYr + XiYi) + j(XiYr - XrYi) */

	if ((((mlib_addr)py) & 1) == 0) {
		if ((((mlib_addr)py) & 3) != 0) {
			lx0 = px[0];
			lx1 = px[1];
			ly0 = py[0];
			ly1 = py[1];
			dot_r += lx0 * (mlib_d64)ly0 + lx1 * (mlib_d64)ly1;
			dot_i += lx1 * (mlib_d64)ly0 - lx0 * (mlib_d64)ly1;
			px += 2;
			py += 2;
			n--;
		}

		if ((((mlib_addr)px) & 3) == 0) {
			mlib_s32 *wpx = (mlib_s32 *)px;
			mlib_s32 *wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_s32 ldx = wpx[i];
				mlib_s32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
				lx0 = (ldx << 24) >> 24;
				lx1 = (ldx << 16) >> 24;
				lx2 = (ldx << 8) >> 24;
				lx3 = ldx >> 24;
				ly0 = (ldy << 24) >> 24;
				ly1 = (ldy << 16) >> 24;
				ly2 = (ldy << 8) >> 24;
				ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				lx0 = ldx >> 24;
				lx1 = (ldx << 8) >> 24;
				lx2 = (ldx << 16) >> 24;
				lx3 = (ldx << 24) >> 24;
				ly0 = ldy >> 24;
				ly1 = (ldy << 8) >> 24;
				ly2 = (ldy << 16) >> 24;
				ly3 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
			}

			i = i << 1;

			if (i < n) {
				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
				ly0 = py[2 * i];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}

		} else {
			mlib_s32 *wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_s32 ldy = wpy[i];

				lx0 = px[4 * i];
				lx1 = px[4 * i + 1];
				lx2 = px[4 * i + 2];
				lx3 = px[4 * i + 3];
#ifdef _LITTLE_ENDIAN
				ly0 = (ldy << 24) >> 24;
				ly1 = (ldy << 16) >> 24;
				ly2 = (ldy << 8) >> 24;
				ly3 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				ly0 = ldy >> 24;
				ly1 = (ldy << 8) >> 24;
				ly2 = (ldy << 16) >> 24;
				ly3 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
			}

			i = i << 1;

			if (i < n) {
				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
				ly0 = py[2 * i];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}
		}

	} else {
/* (((mlib_addr)py) & 1) != 0 */

		if ((((mlib_addr)py) & 3) == 1) {
			lx0 = px[0];
			lx1 = px[1];
			ly0 = py[0];
			ly1 = py[1];
			dot_r += lx0 * (mlib_d64)ly0 + lx1 * (mlib_d64)ly1;
			dot_i += lx1 * (mlib_d64)ly0 - lx0 * (mlib_d64)ly1;
			px += 2;
			py += 2;
			n--;
		}

		if (((mlib_addr)px & 3) == 3) {
			mlib_s32 *wpx = (mlib_s32 *)(px + 1);
			mlib_s32 *wpy = (mlib_s32 *)(py + 1);

			lx0 = px[0];
			ly0 = py[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_s32 ldx = wpx[i];
				mlib_s32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
				lx1 = (ldx << 24) >> 24;
				lx2 = (ldx << 16) >> 24;
				lx3 = (ldx << 8) >> 24;
				ly1 = (ldy << 24) >> 24;
				ly2 = (ldy << 16) >> 24;
				ly3 = (ldy << 8) >> 24;
#else /* _LITTLE_ENDIAN */
				lx1 = ldx >> 24;
				lx2 = (ldx << 8) >> 24;
				lx3 = (ldx << 16) >> 24;
				ly1 = ldy >> 24;
				ly2 = (ldy << 8) >> 24;
				ly3 = (ldy << 16) >> 24;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
#ifdef _LITTLE_ENDIAN
				lx0 = ldx >> 24;
				ly0 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				lx0 = (ldx << 24) >> 24;
				ly0 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
			}

			i = i << 1;

			if (i < n) {
				lx1 = px[2 * i + 1];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}

		} else {
			mlib_s32 *wpy = (mlib_s32 *)(py + 1);

			ly0 = py[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_s32 ldy = wpy[i];

				lx0 = px[4 * i];
				lx1 = px[4 * i + 1];
				lx2 = px[4 * i + 2];
				lx3 = px[4 * i + 3];
#ifdef _LITTLE_ENDIAN
				ly1 = (ldy << 24) >> 24;
				ly2 = (ldy << 16) >> 24;
				ly3 = (ldy << 8) >> 24;
#else /* _LITTLE_ENDIAN */
				ly1 = ldy >> 24;
				ly2 = (ldy << 8) >> 24;
				ly3 = (ldy << 16) >> 24;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
				dot_r += lx2 * (mlib_d64)ly2 +
					lx3 * (mlib_d64)ly3;
				dot_i += lx3 * (mlib_d64)ly2 -
					lx2 * (mlib_d64)ly3;
#ifdef _LITTLE_ENDIAN
				ly0 = ldy >> 24;
#else /* _LITTLE_ENDIAN */
				ly0 = (ldy << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
			}

			i = i << 1;

			if (i < n) {
				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
				ly1 = py[2 * i + 1];
				dot_r += lx0 * (mlib_d64)ly0 +
					lx1 * (mlib_d64)ly1;
				dot_i += lx1 * (mlib_d64)ly0 -
					lx0 * (mlib_d64)ly1;
			}
		}
	}

	z[0] = (mlib_d64)dot_r;
	z[1] = (mlib_d64)dot_i;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
