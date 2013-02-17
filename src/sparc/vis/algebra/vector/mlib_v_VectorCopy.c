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

#pragma ident	"@(#)mlib_v_VectorCopy.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorCopy_U8   - copy of 8-bit format vector
 *      mlib_VectorCopy_S8   - copy of 8-bit format vector
 *      mlib_VectorCopy_S16  - copy of 16-bit format vector
 *      mlib_VectorCopy_S32  - copy of 32-bit format vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorCopy_S16(mlib_s16       *z,
 *                                      const mlib_s16 *x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorCopy_S32(mlib_s32       *z,
 *                                      const mlib_s32 *x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorCopy_S8(mlib_s8       *z,
 *                                     const mlib_s8 *x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorCopy_U8(mlib_u8       *z,
 *                                     const mlib_u8 *x,
 *                                     mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z[i] = x[i]
 */

/*
 * FUNCTIONS
 *      mlib_VectorCopy_U8C  - copy of 8-bit format complex vector
 *      mlib_VectorCopy_S8C  - copy of 8-bit format complex vector
 *      mlib_VectorCopy_S16C - copy of 16-bit format complex vector
 *      mlib_VectorCopy_S32C - copy of 32-bit format complex vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorCopy_S16C(mlib_s16       *z,
 *                                       const mlib_s16 *x,
 *                                       mlib_s32       n);
 *      mlib_status mlib_VectorCopy_S32C(mlib_s32       *z,
 *                                       const mlib_s32 *x,
 *                                       mlib_s32       n);
 *      mlib_status mlib_VectorCopy_S8C(mlib_s8       *z,
 *                                      const mlib_s8 *x,
 *                                      mlib_s32      n);
 *      mlib_status mlib_VectorCopy_U8C(mlib_u8       *z,
 *                                      const mlib_u8 *x,
 *                                      mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z[i] = x[i]
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorCopy_U8C = __mlib_VectorCopy_U8C
#pragma weak mlib_VectorCopy_S32 = __mlib_VectorCopy_S32
#pragma weak mlib_VectorCopy_S16 = __mlib_VectorCopy_S16
#pragma weak mlib_VectorCopy_S32C = __mlib_VectorCopy_S32C
#pragma weak mlib_VectorCopy_S8 = __mlib_VectorCopy_S8
#pragma weak mlib_VectorCopy_S16C = __mlib_VectorCopy_S16C
#pragma weak mlib_VectorCopy_U8 = __mlib_VectorCopy_U8
#pragma weak mlib_VectorCopy_S8C = __mlib_VectorCopy_S8C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorCopy_U8C) mlib_VectorCopy_U8C
	__attribute__((weak, alias("__mlib_VectorCopy_U8C")));
__typeof__(__mlib_VectorCopy_S32) mlib_VectorCopy_S32
	__attribute__((weak, alias("__mlib_VectorCopy_S32")));
__typeof__(__mlib_VectorCopy_S16) mlib_VectorCopy_S16
	__attribute__((weak, alias("__mlib_VectorCopy_S16")));
__typeof__(__mlib_VectorCopy_S32C) mlib_VectorCopy_S32C
	__attribute__((weak, alias("__mlib_VectorCopy_S32C")));
__typeof__(__mlib_VectorCopy_S8) mlib_VectorCopy_S8
	__attribute__((weak, alias("__mlib_VectorCopy_S8")));
__typeof__(__mlib_VectorCopy_S16C) mlib_VectorCopy_S16C
	__attribute__((weak, alias("__mlib_VectorCopy_S16C")));
__typeof__(__mlib_VectorCopy_U8) mlib_VectorCopy_U8
	__attribute__((weak, alias("__mlib_VectorCopy_U8")));
__typeof__(__mlib_VectorCopy_S8C) mlib_VectorCopy_S8C
	__attribute__((weak, alias("__mlib_VectorCopy_S8C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	F_8	0
#define	F_16	1
#define	F_32	2

/* *********************************************************** */

#define	CP_LARGE(format)                                          \
	n = n << format;                                          \
	if (((mlib_addr)src | (mlib_addr)dst) & 7)                \
	    mlib_v_cp_64_na((mlib_u8 *)dst, (mlib_u8 *)src, n);   \
	else                                                      \
	    mlib_v_cp_64((mlib_d64 *)dst, (mlib_d64 *)src, n)

/* *********************************************************** */

static void mlib_v_cp_64(
	mlib_d64 *dst,
	const mlib_d64 *src,
	mlib_s32 n);
static void mlib_v_cp_64_na(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n);

#pragma no_inline(mlib_v_cp_64, mlib_v_cp_64_na)

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_U8(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *dst = z;
	mlib_u8 *src = (void *)x;

	if (n > 12) {
		CP_LARGE(F_8);
		return (MLIB_SUCCESS);
	}

/*
 * For very mlib_s16 lengths we use copying by assignment statement
 * in case - operator.
 */

	switch (n) {
	    case 1:
		    dst[0] = src[0];
		    return (MLIB_SUCCESS);
	    case 2:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    return (MLIB_SUCCESS);
	    case 3:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    return (MLIB_SUCCESS);
	    case 4:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    return (MLIB_SUCCESS);
	    case 5:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    return (MLIB_SUCCESS);
	    case 6:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    return (MLIB_SUCCESS);
	    case 7:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    return (MLIB_SUCCESS);
	    case 8:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    return (MLIB_SUCCESS);
	    case 9:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    dst[8] = src[8];
		    return (MLIB_SUCCESS);
	    case 10:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    dst[8] = src[8];
		    dst[9] = src[9];
		    return (MLIB_SUCCESS);
	    case 11:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    dst[8] = src[8];
		    dst[9] = src[9];
		    dst[10] = src[10];
		    return (MLIB_SUCCESS);
	    case 12:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    dst[8] = src[8];
		    dst[9] = src[9];
		    dst[10] = src[10];
		    dst[11] = src[11];
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_u16 *dst = (mlib_u16 *)z;
	mlib_u16 *src = (mlib_u16 *)x;

	if (n > 8) {
		CP_LARGE(F_16);
		return (MLIB_SUCCESS);
	}

	switch (n) {
	    case 1:
		    dst[0] = src[0];
		    return (MLIB_SUCCESS);
	    case 2:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    return (MLIB_SUCCESS);
	    case 3:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    return (MLIB_SUCCESS);
	    case 4:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    return (MLIB_SUCCESS);
	    case 5:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    return (MLIB_SUCCESS);
	    case 6:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    return (MLIB_SUCCESS);
	    case 7:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    return (MLIB_SUCCESS);
	    case 8:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *dst = z;
	mlib_s32 *src = (void *)x;

	if (n > 8) {
		CP_LARGE(F_32);
		return (MLIB_SUCCESS);
	}

	switch (n) {
	    case 1:
		    dst[0] = src[0];
		    return (MLIB_SUCCESS);
	    case 2:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    return (MLIB_SUCCESS);
	    case 3:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    return (MLIB_SUCCESS);
	    case 4:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    return (MLIB_SUCCESS);
	    case 5:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    return (MLIB_SUCCESS);
	    case 6:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    return (MLIB_SUCCESS);
	    case 7:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    return (MLIB_SUCCESS);
	    case 8:
		    dst[0] = src[0];
		    dst[1] = src[1];
		    dst[2] = src[2];
		    dst[3] = src[3];
		    dst[4] = src[4];
		    dst[5] = src[5];
		    dst[6] = src[6];
		    dst[7] = src[7];
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

#pragma no_inline(__mlib_VectorCopy_U8)

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S8(
	mlib_s8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, (mlib_u8 *)x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_U8C(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S8C(
	mlib_s8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, (mlib_u8 *)x, n + n));
}

/* *********************************************************** */

#pragma no_inline(__mlib_VectorCopy_S16)

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S16C(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_S16(z, x, n + n));
}

/* *********************************************************** */

#pragma no_inline(__mlib_VectorCopy_S32)

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S32C(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_S32(z, x, n + n));
}

/* *********************************************************** */

static void
mlib_v_cp_64(
	mlib_d64 *dst,
	const mlib_d64 *src,
	mlib_s32 n)
{
	mlib_s32 j;

/* edge mask */
	mlib_s32 emask;

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200

	for (j = 0; j < (n - 1) >> 3; j++) {
		dst[j] = src[j];
	}

	emask = vis_edge8((void *)0, (void *)((n & 7) - 1));
	vis_pst_8(vis_ld_d64_nf((void *)(src + j)), dst + j, emask);

#else /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

	if (j = (n >> 3) & 1)
		dst[0] = src[0];

	for (; j < (n >> 3); j += 2) {
		dst[j] = src[j];
		dst[j + 1] = src[j + 1];
	}

	if (n & 7) {
		emask = vis_edge8((void *)0, (void *)((n & 7) - 1));
		vis_pst_8(vis_ld_d64_nf((void *)(src + j)), dst + j, emask);
	}
#endif /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */
}

/* *********************************************************** */

static void
mlib_v_cp_64_na(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
/* end points in dst */
	mlib_u8 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;
	mlib_d64 s0, s1, s2;
	mlib_s32 j;
	mlib_s32 emask;

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	j = (mlib_addr)dp - (mlib_addr)dst;
	dend = dst + n - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr((void *)src, j);

/* generate edge mask for the start point */
	emask = vis_edge8(dst, dend);
	s1 = vis_ld_d64_nf(sp);

	if (j) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		s0 = vis_faligndata(s0, s1);
		vis_pst_8(s0, dp++, emask);
		sp++;
		j += 8;
	}

	for (; j < (n - 8); j += 8) {
		sp++;
		s0 = s1;
		s1 = sp[0];
		s2 = vis_faligndata(s0, s1);
		(*dp++) = s2;
	}

	if (j < n) {
		s0 = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
		emask = vis_edge8(dp, dend);
		vis_pst_8(s0, dp, emask);
	}
}

/* *********************************************************** */
