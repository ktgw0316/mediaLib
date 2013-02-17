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

#pragma ident	"@(#)mlib_i_VectorDotProd16.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S16_Sat - dot product of signed 16-bit format vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_S16_Sat(mlib_d64       *z,
 *                                            const mlib_s16 *x,
 *                                            const mlib_s16 *y,
 *                                            mlib_s32       n)
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
 *      mlib_VectorDotProd_S16C_Sat - dot product of signed 16-bit format
 *                                    complex vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_S16C_Sat(mlib_d64 *z,
 *                                             mlib_s16 *x,
 *                                             mlib_s16 *y,
 *                                             mlib_s32 n)
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

#pragma weak mlib_VectorDotProd_S16_Sat = __mlib_VectorDotProd_S16_Sat
#pragma weak mlib_VectorDotProd_S16C_Sat = __mlib_VectorDotProd_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_S16_Sat) mlib_VectorDotProd_S16_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S16_Sat")));
__typeof__(__mlib_VectorDotProd_S16C_Sat) mlib_VectorDotProd_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#ifdef _NO_LONGLONG

typedef mlib_d64 LONGTYPE;

#else

typedef mlib_s64 LONGTYPE;

#endif

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	LOAD_S16_0                                              \
	lx0 = (ldx << 16) >> 16;                                \
	lx1 = ldx >> 16;                                        \
	ly0 = (ldy << 16) >> 16;                                \
	ly1 = ldy >> 16;                                        \
	dot += lx0 * (LONGTYPE)ly0;                             \
	dot += lx1 * (LONGTYPE)ly1

#define	LOAD_S16_1                                              \
	ly0 = (ldy << 16) >> 16;                                \
	ly1 = ldy >> 16

#else /* _LITTLE_ENDIAN */

#define	LOAD_S16_0                                              \
	lx0 = ldx >> 16;                                        \
	lx1 = (ldx << 16) >> 16;                                \
	ly0 = ldy >> 16;                                        \
	ly1 = (ldy << 16) >> 16;                                \
	dot += lx0 * (LONGTYPE)ly0;                             \
	dot += lx1 * (LONGTYPE)ly1

#define	LOAD_S16_1                                              \
	ly0 = ldy >> 16;                                        \
	ly1 = (ldy << 16) >> 16

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i, *wpx, *wpy;
	LONGTYPE dot = 0;
	mlib_s16 *px, *py;
	mlib_s32 lx0, lx1;
	mlib_s32 ly0, ly1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;

	if ((mlib_addr)py & 3) {
		dot += (*px++) * (LONGTYPE)(*py++);
		n--;
	}

	if ((((mlib_addr)px) & 3) == 0) {
		wpx = (mlib_s32 *)px;
		wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1) - 1; i += 2) {
			mlib_s32 ldx = wpx[i];
			mlib_s32 ldy = wpy[i];

			LOAD_S16_0;

			ldx = wpx[i + 1];
			ldy = wpy[i + 1];

			LOAD_S16_0;
		}

		if (i < n >> 1) {
			mlib_s32 ldx = wpx[i];
			mlib_s32 ldy = wpy[i];

			LOAD_S16_0;
			i++;
		}

		px = (mlib_s16 *)(wpx + i);
		py = (mlib_s16 *)(wpy + i);
	} else {
		wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1) - 1; i += 2) {
			mlib_s32 ldy = wpy[i];

			LOAD_S16_1;
			dot += px[0] * (LONGTYPE)ly0;
			dot += px[1] * (LONGTYPE)ly1;

			ldy = wpy[i + 1];

			LOAD_S16_1;
			dot += px[2] * (LONGTYPE)ly0;
			dot += px[3] * (LONGTYPE)ly1;
			px += 4;
		}

		if (i < n >> 1) {
			mlib_s32 ldy = wpy[i];

			LOAD_S16_1;
			dot += px[0] * (LONGTYPE)ly0;
			dot += px[1] * (LONGTYPE)ly1;
			px += 2;
			i++;
		}

		py = (mlib_s16 *)(wpy + i);
	}

	i = i << 1;

	if (i < n) {
		dot += *px * (LONGTYPE)(*py);
	}

	z[0] = (mlib_d64)dot;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S16C_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	LONGTYPE dot_r = 0, dot_i = 0;
	mlib_s16 *px, *py;
	mlib_s32 lx0, lx1;
	mlib_s32 ly0, ly1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;

/* (Xr + jXi) * (Yr - jYi) = (XrYr + XiYi) + j(XiYr - XrYi) */

	if ((((mlib_addr)py) & 3) == 0) {

		if ((((mlib_addr)px) & 3) == 0) {
			mlib_s32 *wpx = (mlib_s32 *)px;
			mlib_s32 *wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n; i++) {
				mlib_s32 ldx = wpx[i];
				mlib_s32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
				lx0 = (ldx << 16) >> 16;
				lx1 = ldx >> 16;
				ly0 = (ldy << 16) >> 16;
				ly1 = ldy >> 16;
#else /* _LITTLE_ENDIAN */
				lx0 = ldx >> 16;
				lx1 = (ldx << 16) >> 16;
				ly0 = ldy >> 16;
				ly1 = (ldy << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (LONGTYPE)ly0 +
					lx1 * (LONGTYPE)ly1;
				dot_i += lx1 * (LONGTYPE)ly0 -
					lx0 * (LONGTYPE)ly1;
			}

		} else {
			mlib_s32 *wpy = (mlib_s32 *)py;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n; i++) {
				mlib_s32 ldy = wpy[i];

				lx0 = px[2 * i];
				lx1 = px[2 * i + 1];
#ifdef _LITTLE_ENDIAN
				ly0 = (ldy << 16) >> 16;
				ly1 = ldy >> 16;
#else /* _LITTLE_ENDIAN */
				ly0 = ldy >> 16;
				ly1 = (ldy << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (LONGTYPE)ly0 +
					lx1 * (LONGTYPE)ly1;
				dot_i += lx1 * (LONGTYPE)ly0 -
					lx0 * (LONGTYPE)ly1;
			}
		}

	} else {
/* (((mlib_addr)py) & 3) != 0 */

		if (((mlib_addr)px & 3) != 0) {
			mlib_s32 *wpx = (mlib_s32 *)(px + 1);
			mlib_s32 *wpy = (mlib_s32 *)(py + 1);

			lx0 = px[0];
			ly0 = py[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n - 1; i++) {
				mlib_s32 ldx = wpx[i];
				mlib_s32 ldy = wpy[i];

#ifdef _LITTLE_ENDIAN
				lx1 = (ldx << 16) >> 16;
				ly1 = (ldy << 16) >> 16;
#else /* _LITTLE_ENDIAN */
				lx1 = ldx >> 16;
				ly1 = ldy >> 16;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (LONGTYPE)ly0 +
					lx1 * (LONGTYPE)ly1;
				dot_i += lx1 * (LONGTYPE)ly0 -
					lx0 * (LONGTYPE)ly1;
#ifdef _LITTLE_ENDIAN
				lx0 = ldx >> 16;
				ly0 = ldy >> 16;
#else /* _LITTLE_ENDIAN */
				lx0 = (ldx << 16) >> 16;
				ly0 = (ldy << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
			}

			lx1 = px[2 * i + 1];
			ly1 = py[2 * i + 1];
			dot_r += lx0 * (LONGTYPE)ly0 + lx1 * (LONGTYPE)ly1;
			dot_i += lx1 * (LONGTYPE)ly0 - lx0 * (LONGTYPE)ly1;

		} else {
			mlib_s32 *wpy = (mlib_s32 *)(py + 1);

			lx0 = px[0];
			ly0 = py[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n - 1; i++) {
				mlib_s32 ldy = wpy[i];

				lx1 = px[2 * i + 1];
#ifdef _LITTLE_ENDIAN
				ly1 = (ldy << 16) >> 16;
#else /* _LITTLE_ENDIAN */
				ly1 = ldy >> 16;
#endif /* _LITTLE_ENDIAN */
				dot_r += lx0 * (LONGTYPE)ly0 +
					lx1 * (LONGTYPE)ly1;
				dot_i += lx1 * (LONGTYPE)ly0 -
					lx0 * (LONGTYPE)ly1;
				lx0 = px[2 * (i + 1)];
#ifdef _LITTLE_ENDIAN
				ly0 = ldy >> 16;
#else /* _LITTLE_ENDIAN */
				ly0 = (ldy << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
			}

			lx1 = px[2 * i + 1];
			ly1 = py[2 * i + 1];
			dot_r += lx0 * (LONGTYPE)ly0 + lx1 * (LONGTYPE)ly1;
			dot_i += lx1 * (LONGTYPE)ly0 - lx0 * (LONGTYPE)ly1;
		}
	}

	z[0] = (mlib_d64)dot_r;
	z[1] = (mlib_d64)dot_i;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
