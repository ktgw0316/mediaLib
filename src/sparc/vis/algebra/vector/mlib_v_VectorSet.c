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

#pragma ident	"@(#)mlib_v_VectorSet.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSet_U8 - set of 8-bit format vector to a scalar
 *      mlib_VectorSet_S8 - set of 8-bit format vector to a scalar
 *      mlib_VectorSet_S16 - set of 16-bit format vector to a scalar
 *      mlib_VectorSet_S32 - set of 32-bit format vector to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSet_S16(mlib_s16       *z,
 *                                     const mlib_s16 *c,
 *                                     mlib_s32       n);
 *      mlib_status mlib_VectorSet_S32(mlib_s32       *z,
 *                                     const mlib_s32 *c,
 *                                     mlib_s32       n);
 *      mlib_status mlib_VectorSet_S8(mlib_s8       *z,
 *                                    const mlib_s8 *c,
 *                                    mlib_s32      n);
 *      mlib_status mlib_VectorSet_U8(mlib_u8       *z,
 *                                    const mlib_u8 *c,
 *                                    mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector.
 *      c    a scalar.
 *      n    number of elements in the vector.
 *
 * DESCRIPTION
 *      z[i] = c
 */

/*
 * FUNCTIONS
 *      mlib_VectorSet_U8C - set of 8-bit format complex vector to a scalar
 *      mlib_VectorSet_S8C - set of 8-bit format complex vector to a scalar
 *      mlib_VectorSet_S16C - set of 16-bit format complex vector to a scalar
 *      mlib_VectorSet_S32C - set of 32-bit format complex vector to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSet_S16C(mlib_s16       *z,
 *                                      const mlib_s16 *c,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorSet_S32C(mlib_s32       *z,
 *                                      const mlib_s32 *c,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorSet_S8C(mlib_s8       *z,
 *                                     const mlib_s8 *c,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorSet_U8C(mlib_u8       *z,
 *                                     const mlib_u8 *c,
 *                                     mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts.
 *      c    pointer to a complex scalar. c[0] holds the real part,
 *           and c[1] holds the imaginary part.
 *      n    number of complex elements in the vector.
 *
 * DESCRIPTION
 *      z[i] = c
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSet_U8C = __mlib_VectorSet_U8C
#pragma weak mlib_VectorSet_S32 = __mlib_VectorSet_S32
#pragma weak mlib_VectorSet_S16 = __mlib_VectorSet_S16
#pragma weak mlib_VectorSet_S32C = __mlib_VectorSet_S32C
#pragma weak mlib_VectorSet_S8 = __mlib_VectorSet_S8
#pragma weak mlib_VectorSet_S16C = __mlib_VectorSet_S16C
#pragma weak mlib_VectorSet_U8 = __mlib_VectorSet_U8
#pragma weak mlib_VectorSet_S8C = __mlib_VectorSet_S8C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorSet_U8C) mlib_VectorSet_U8C
	__attribute__((weak, alias("__mlib_VectorSet_U8C")));
__typeof__(__mlib_VectorSet_S32) mlib_VectorSet_S32
	__attribute__((weak, alias("__mlib_VectorSet_S32")));
__typeof__(__mlib_VectorSet_S16) mlib_VectorSet_S16
	__attribute__((weak, alias("__mlib_VectorSet_S16")));
__typeof__(__mlib_VectorSet_S32C) mlib_VectorSet_S32C
	__attribute__((weak, alias("__mlib_VectorSet_S32C")));
__typeof__(__mlib_VectorSet_S8) mlib_VectorSet_S8
	__attribute__((weak, alias("__mlib_VectorSet_S8")));
__typeof__(__mlib_VectorSet_S16C) mlib_VectorSet_S16C
	__attribute__((weak, alias("__mlib_VectorSet_S16C")));
__typeof__(__mlib_VectorSet_U8) mlib_VectorSet_U8
	__attribute__((weak, alias("__mlib_VectorSet_U8")));
__typeof__(__mlib_VectorSet_S8C) mlib_VectorSet_S8C
	__attribute__((weak, alias("__mlib_VectorSet_S8C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SET_BY_LOOP(vec, lim, c)                                \
	for (i = 0; i < lim; i++)                               \
	    vec[i] = c;                                         \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	SET_BY_LOOP_C(vec, lim)                                 \
	for (i = 0; i < lim; i += 2) {                          \
	    vec[i] = c_r;                                       \
	    vec[i + 1] = c_i;                                   \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

static void mlib_v_set_64(
	mlib_u8 *pdst,
	mlib_d64 src,
	mlib_s32 n);

#pragma no_inline(mlib_v_set_64)

/* *********************************************************** */

mlib_status
__mlib_VectorSet_U8(
	mlib_u8 *z,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 cc = c[0], *dst = z;
	mlib_s32 i;

	if (n < 16) {
		SET_BY_LOOP(dst, n, cc);
	} else {
		mlib_s32 what = (cc << 8) | cc;
		mlib_d64 src;

		what = (what << 16) | what;
		src = vis_to_double_dup(what);
		mlib_v_set_64(dst, src, n);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S8(
	mlib_s8 *z,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSet_U8((mlib_u8 *)z, (const mlib_u8 *)c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S16(
	mlib_s16 *z,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_u16 cc = ((mlib_u16 *)c)[0], *dst = (mlib_u16 *)z;
	mlib_s32 i;

	if (n < 8) {
		SET_BY_LOOP(dst, n, cc);
	} else {
		mlib_s32 what = (cc << 16) | cc;
		mlib_d64 src = vis_to_double_dup(what);

		mlib_v_set_64((mlib_u8 *)dst, src, 2 * n);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S32(
	mlib_s32 *z,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 cc = c[0], *dst = z;
	mlib_s32 i;

	if (n < 4) {
		SET_BY_LOOP(dst, n, cc);
	} else {
		mlib_d64 src = vis_to_double_dup(cc);

		mlib_v_set_64((mlib_u8 *)dst, src, 4 * n);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_U8C(
	mlib_u8 *z,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 c_r = c[0], *dst = z, c_i = c[1];
	mlib_s32 i;

	n = n + n;

	if (n < 16) {
		SET_BY_LOOP_C(dst, n);
	} else {
		mlib_s32 what = (c_r << 8) | c_i;
		mlib_d64 src;

		what = (what << 16) | what;
		src = vis_to_double_dup(what);
		mlib_v_set_64(dst, src, n);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S8C(
	mlib_s8 *z,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSet_U8C((mlib_u8 *)z, (mlib_u8 *)c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S16C(
	mlib_s16 *z,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_u16 c_r = ((mlib_u16 *)c)[0];
	mlib_u16 *dst = (mlib_u16 *)z;
	mlib_u16 c_i = ((mlib_u16 *)c)[1];
	mlib_s32 i;

	n = n + n;

	if (n < 8) {
		SET_BY_LOOP_C(dst, n);
	} else {
		mlib_s32 what = (c_r << 16) | c_i;
		mlib_d64 src = vis_to_double_dup(what);

		mlib_v_set_64((mlib_u8 *)dst, src, 2 * n);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S32C(
	mlib_s32 *z,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 c_r = c[0], *dst = z, c_i = c[1];
	mlib_s32 i;

	n = n + n;

	if (n < 4) {
		SET_BY_LOOP_C(dst, n);
	} else {
		mlib_d64 src = vis_to_double(c_r, c_i);

		mlib_v_set_64((mlib_u8 *)dst, src, 4 * n);
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

static void
mlib_v_set_64(
	mlib_u8 *pdst,
	mlib_d64 src,
	mlib_s32 n)
{
	mlib_u8 *pend = pdst + n - 1;
	mlib_d64 *dpdst = (mlib_d64 *)((mlib_addr)pdst & ~7);
	mlib_s32 i, emask = vis_edge8(pdst, pend);

	vis_alignaddr((void *)~(mlib_addr)pdst, 1);
	src = vis_faligndata(src, src);
	vis_pst_8(src, dpdst++, emask);
	n -= (mlib_addr)dpdst - (mlib_addr)pdst;

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200

	for (i = 0; i < (n >> 3); i++) {
		(*dpdst++) = src;
	}

#else

	if (i = (n >> 3) & 1)
		dpdst[0] = src;

	for (; i < (n >> 3); i += 2) {
		dpdst[i] = src;
		dpdst[i + 1] = src;
	}

	dpdst += i;

#endif /* define(MLIB_VIS) && MLIB_VIS >= 0x200 */

	if (n & 7) {
		emask = vis_edge8(dpdst, pend);
		vis_pst_8(src, dpdst, emask);
	}
}

/* *********************************************************** */
