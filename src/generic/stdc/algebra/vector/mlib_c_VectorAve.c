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

#pragma ident	"@(#)mlib_c_VectorAve.c	9.7	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAve_[U8_U8|S8_S8],
 *      mlib_VectorAve_S16_[U8|S8],
 *      mlib_VectorAve_[U8|S8] - average of signed/unsigned
 *                               8-bit format vectors
 *
 *      mlib_VectorAve_[S16|S32]_S16,
 *      mlib_VectorAve_S16 - average of signed 16-bit format vectors
 *
 *      mlib_VectorAve_S32_S32,
 *      mlib_VectorAve_S32 - average of signed 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAve_U8_U8(mlib_u8       *z,
 *                                       const mlib_u8 *x,
 *                                       const mlib_u8 *y,
 *                                       mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8_S8(mlib_s8       *z,
 *                                       const mlib_s8 *x,
 *                                       const mlib_s8 *y,
 *                                       mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_U8(mlib_s16      *z,
 *                                        const mlib_u8 *x,
 *                                        const mlib_u8 *y,
 *                                        mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_S8(mlib_s16      *z,
 *                                        const mlib_s8 *x,
 *                                        const mlib_s8 *y,
 *                                        mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_S16(mlib_s16       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32_S16(mlib_s32       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32_S32(mlib_s32       *z,
 *                                         const mlib_s32 *x,
 *                                         const mlib_s32 *y,
 *                                         mlib_s32       n)
 *
 *      mlib_status mlib_VectorAve_U8(mlib_u8       *xz,
 *                                    const mlib_u8 *y,
 *                                    mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8(mlib_s8       *xz,
 *                                    const mlib_s8 *y,
 *                                    mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16(mlib_s16       *xz,
 *                                     const mlib_s16 *y,
 *                                     mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32(mlib_s32       *xz,
 *                                     const mlib_s32 *y,
 *                                     mlib_s32       n)
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      xz   pointer to the first element of the first input and
 *           the output vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = (x + y + 1) / 2
 */

/*
 * FUNCTIONS
 *      mlib_VectorAve_[U8C_U8C|S8C_S8C],
 *      mlib_VectorAve_S16C_[U8C|S8C],
 *      mlib_VectorAve_[U8C|S8C] - average of signed/unsigned
 *                                 8-bit format complex vectors
 *
 *      mlib_VectorAve_[S16C|S32C]_S16C,
 *      mlib_VectorAve_S16C - average of signed 16-bit format complex vectors
 *
 *      mlib_VectorAve_S32C_S32C,
 *      mlib_VectorAve_S32C - average of signed 32-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAve_U8C_U8C(mlib_u8       *z,
 *                                         const mlib_u8 *x,
 *                                         const mlib_u8 *y,
 *                                         mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8C_S8C(mlib_s8       *z,
 *                                         const mlib_s8 *x,
 *                                         const mlib_s8 *y,
 *                                         mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_U8C(mlib_s16      *z,
 *                                          const mlib_u8 *x,
 *                                          const mlib_u8 *y,
 *                                          mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_S8C(mlib_s16      *z,
 *                                          const mlib_s8 *x,
 *                                          const mlib_s8 *y,
 *                                          mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_S16C(mlib_s16       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C_S16C(mlib_s32       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C_S32C(mlib_s32       *z,
 *                                           const mlib_s32 *x,
 *                                           const mlib_s32 *y,
 *                                           mlib_s32       n)
 *
 *      mlib_status mlib_VectorAve_U8C(mlib_u8       *xz,
 *                                     const mlib_u8 *y,
 *                                     mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8C(mlib_s8       *xz,
 *                                     const mlib_s8 *y,
 *                                     mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C(mlib_s16       *xz,
 *                                      const mlib_s16 *y,
 *                                      mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C(mlib_s32       *xz,
 *                                      const mlib_s32 *y,
 *                                      mlib_s32       n)
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      xz   pointer to the first element of the first input and
 *           the output vector.
 *           xz[2*i] hold the real parts, and xz[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = (x + y + 1) / 2
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorAve_S8 = __mlib_VectorAve_S8
#pragma weak mlib_VectorAve_U8 = __mlib_VectorAve_U8
#pragma weak mlib_VectorAve_S16	= __mlib_VectorAve_S16
#pragma weak mlib_VectorAve_S32	= __mlib_VectorAve_S32
#pragma weak mlib_VectorAve_S8C	= __mlib_VectorAve_S8C
#pragma weak mlib_VectorAve_U8C	= __mlib_VectorAve_U8C
#pragma weak mlib_VectorAve_S16C = __mlib_VectorAve_S16C
#pragma weak mlib_VectorAve_S32C = __mlib_VectorAve_S32C
#pragma weak mlib_VectorAve_S8_S8 = __mlib_VectorAve_S8_S8
#pragma weak mlib_VectorAve_U8_U8 = __mlib_VectorAve_U8_U8
#pragma weak mlib_VectorAve_S16_U8 = __mlib_VectorAve_S16_U8
#pragma weak mlib_VectorAve_S16_S8 = __mlib_VectorAve_S16_S8
#pragma weak mlib_VectorAve_S16_S16 = __mlib_VectorAve_S16_S16
#pragma weak mlib_VectorAve_S32_S16 = __mlib_VectorAve_S32_S16
#pragma weak mlib_VectorAve_S32_S32 = __mlib_VectorAve_S32_S32
#pragma weak mlib_VectorAve_S8C_S8C = __mlib_VectorAve_S8C_S8C
#pragma weak mlib_VectorAve_U8C_U8C = __mlib_VectorAve_U8C_U8C
#pragma weak mlib_VectorAve_S16C_U8C = __mlib_VectorAve_S16C_U8C
#pragma weak mlib_VectorAve_S16C_S8C = __mlib_VectorAve_S16C_S8C
#pragma weak mlib_VectorAve_S16C_S16C = __mlib_VectorAve_S16C_S16C
#pragma weak mlib_VectorAve_S32C_S16C = __mlib_VectorAve_S32C_S16C
#pragma weak mlib_VectorAve_S32C_S32C = __mlib_VectorAve_S32C_S32C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorAve_S8) mlib_VectorAve_S8
	__attribute__((weak, alias("__mlib_VectorAve_S8")));
__typeof__(__mlib_VectorAve_U8) mlib_VectorAve_U8
	__attribute__((weak, alias("__mlib_VectorAve_U8")));
__typeof__(__mlib_VectorAve_S16) mlib_VectorAve_S16
	__attribute__((weak, alias("__mlib_VectorAve_S16")));
__typeof__(__mlib_VectorAve_S32) mlib_VectorAve_S32
	__attribute__((weak, alias("__mlib_VectorAve_S32")));
__typeof__(__mlib_VectorAve_S8C) mlib_VectorAve_S8C
	__attribute__((weak, alias("__mlib_VectorAve_S8C")));
__typeof__(__mlib_VectorAve_U8C) mlib_VectorAve_U8C
	__attribute__((weak, alias("__mlib_VectorAve_U8C")));
__typeof__(__mlib_VectorAve_S16C) mlib_VectorAve_S16C
	__attribute__((weak, alias("__mlib_VectorAve_S16C")));
__typeof__(__mlib_VectorAve_S32C) mlib_VectorAve_S32C
	__attribute__((weak, alias("__mlib_VectorAve_S32C")));
__typeof__(__mlib_VectorAve_S8_S8) mlib_VectorAve_S8_S8
	__attribute__((weak, alias("__mlib_VectorAve_S8_S8")));
__typeof__(__mlib_VectorAve_U8_U8) mlib_VectorAve_U8_U8
	__attribute__((weak, alias("__mlib_VectorAve_U8_U8")));
__typeof__(__mlib_VectorAve_S16_U8) mlib_VectorAve_S16_U8
	__attribute__((weak, alias("__mlib_VectorAve_S16_U8")));
__typeof__(__mlib_VectorAve_S16_S8) mlib_VectorAve_S16_S8
	__attribute__((weak, alias("__mlib_VectorAve_S16_S8")));
__typeof__(__mlib_VectorAve_S16_S16) mlib_VectorAve_S16_S16
	__attribute__((weak, alias("__mlib_VectorAve_S16_S16")));
__typeof__(__mlib_VectorAve_S32_S16) mlib_VectorAve_S32_S16
	__attribute__((weak, alias("__mlib_VectorAve_S32_S16")));
__typeof__(__mlib_VectorAve_S32_S32) mlib_VectorAve_S32_S32
	__attribute__((weak, alias("__mlib_VectorAve_S32_S32")));
__typeof__(__mlib_VectorAve_S8C_S8C) mlib_VectorAve_S8C_S8C
	__attribute__((weak, alias("__mlib_VectorAve_S8C_S8C")));
__typeof__(__mlib_VectorAve_U8C_U8C) mlib_VectorAve_U8C_U8C
	__attribute__((weak, alias("__mlib_VectorAve_U8C_U8C")));
__typeof__(__mlib_VectorAve_S16C_U8C) mlib_VectorAve_S16C_U8C
	__attribute__((weak, alias("__mlib_VectorAve_S16C_U8C")));
__typeof__(__mlib_VectorAve_S16C_S8C) mlib_VectorAve_S16C_S8C
	__attribute__((weak, alias("__mlib_VectorAve_S16C_S8C")));
__typeof__(__mlib_VectorAve_S16C_S16C) mlib_VectorAve_S16C_S16C
	__attribute__((weak, alias("__mlib_VectorAve_S16C_S16C")));
__typeof__(__mlib_VectorAve_S32C_S16C) mlib_VectorAve_S32C_S16C
	__attribute__((weak, alias("__mlib_VectorAve_S32C_S16C")));
__typeof__(__mlib_VectorAve_S32C_S32C) mlib_VectorAve_S32C_S32C
	__attribute__((weak, alias("__mlib_VectorAve_S32C_S32C")));
#else /* defined(__SUNPRO_C) */

#error	"unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	AVE_WORK_LOOP(z, x, y, n)      	         \
for (i = 0; i < n; i++) {                    	 \
	z[i] = (x[i] + (mlib_s32)y[i] + 1) >> 1; \
}

/* *********************************************************** */

#define	AVE_WORK_LOOP_S32_IN_D64(z, x, y, n) 		      	\
for (i = 0; i < n; i++) {                               	\
	z[i] = (mlib_s32)(((mlib_d64)x[i] + y[i]) * 0.5 + 0.5);	\
}

/* *********************************************************** */

#define	AVE_WORK_LOOP_S32_IN_S64(z, x, y, n)                		\
for (i = 0; i < n; i++) {                                         	\
	z[i] = (mlib_s32)(((mlib_s64)x[i] + (mlib_s64)y[i] + 1) >> 1);  \
}

/* *********************************************************** */

#define	AVE_ALIGN_U8(spx, spy, spz, j)				\
	sr2 = ((spx[j] >> 1) & mask) + ((spy[j] >> 1) & mask);	\
	sr3 = (spx[j] | spy[j]) & (0x01010101);			\
	spz[j] = sr2 + sr3;

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8_U8(
	mlib_u8	*z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 *spx, *spy, *spz;
	mlib_s32 sr1, sr2, sr3;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 alen;
	mlib_s32 alens;
	mlib_s32 fd;
	mlib_s32 i, j;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 3) == 0) {
		spx = (mlib_s32 *)px;
		spy = (mlib_s32 *)py;
		spz = (mlib_s32 *)pz;
		alen = n >> 2;
		alens = alen << 2;
		fd = n & 3;

		if (j = alen & 1) {
			AVE_ALIGN_U8(spx, spy, spz, 0)
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j < alen; j += 2) {
			AVE_ALIGN_U8(spx, spy, spz, j)
			AVE_ALIGN_U8(spx, spy, spz, j+1)
		}

		if (fd > 2) {
		    pz[alens + 2] = (px[alens + 2] + py[alens + 2] + 1) >> 1;
		}

		if (fd > 1) {
		    pz[alens + 1] = (px[alens + 1] + py[alens + 1] + 1) >> 1;
		}

		if (fd > 0) {
		    pz[alens] = (px[alens] + py[alens] + 1) >> 1;
		}
	}
	else
	{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		AVE_WORK_LOOP(z, x, y, n)
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8_S8(
	mlib_s8	*z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16_U8(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16_S8(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32_S16(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *pxz = (mlib_u8 *)xz, *py = (mlib_u8 *)y;
	mlib_s32 *spxz, *spy;
	mlib_s32 sr1, sr2, sr3;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 alen;
	mlib_s32 alens;
	mlib_s32 fd;
	mlib_s32 i, j;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	if ((((mlib_addr)xz | (mlib_addr)y) & 3) == 0) {
		spxz = (mlib_s32 *)pxz;
		spy	= (mlib_s32 *)py;
		alen = n >> 2;
		alens = alen << 2;
		fd = n & 3;

		if (j = alen & 1) {
			AVE_ALIGN_U8(spxz, spy, spxz, 0)
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j < alen; j += 2) {
			AVE_ALIGN_U8(spxz, spy, spxz, j)
			AVE_ALIGN_U8(spxz, spy, spxz, j+1)
		}

		if (fd > 2) {
		    pxz[alens + 2] = (pxz[alens + 2] + py[alens + 2] + 1) >> 1;
		}

		if (fd > 1) {
		    pxz[alens + 1] = (pxz[alens + 1] + py[alens + 1] + 1) >> 1;
		}

		if (fd > 0) {
		    pxz[alens] = (pxz[alens] + py[alens] + 1) >> 1;
		}
	}
	else
	{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		AVE_WORK_LOOP(xz, xz, y, n)
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	AVE_WORK_LOOP(xz, xz, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(xz, xz, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8C_U8C(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_U8_U8(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8C_S8C(
	mlib_s8	*z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_U8C(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_S8C(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_S16C(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C_S16C(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(z, x, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8C(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(xz, xz, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8C(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(xz, xz, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP(xz, xz, y, n)

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C_S32C(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef _NO_LONGLONG

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_D64(z, x, y, n)

#else /* _NO_LONGLONG */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_S64(z, x, y, n)

#endif  /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	n <<= 1;

#ifdef _NO_LONGLONG

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_D64(xz, xz, y, n)

#else /* _NO_LONGLONG */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_S64(xz, xz, y, n)

#endif  /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef _NO_LONGLONG

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_D64(xz, xz, y, n)

#else /* _NO_LONGLONG */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_S64(xz, xz, y, n)

#endif  /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef _NO_LONGLONG

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_D64(z, x, y, n)

#else /* _NO_LONGLONG */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_S64(z, x, y, n)

#endif  /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
