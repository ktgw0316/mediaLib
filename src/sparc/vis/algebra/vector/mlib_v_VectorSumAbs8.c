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

#pragma ident	"@(#)mlib_v_VectorSumAbs8.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSumAbs_U8 -  sum of the absolute values
 *                              of unsigned 8-bit format vector
 *      mlib_VectorSumAbs_S8 -  sum of the absolute values
 *                              of signed 8-bit format vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSumAbs_S8_Sat(mlib_d64      * z,
 *                                           const mlib_s8 * x,
 *                                           mlib_s32      n);
 *      mlib_status mlib_VectorSumAbs_U8_Sat(mlib_d64      * z,
 *                                           const mlib_u8 * x,
 *                                           mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute values of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *           n
 *      z = SUM |x[i]|
 *          i = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbs_S8_Sat = __mlib_VectorSumAbs_S8_Sat
#pragma weak mlib_VectorSumAbs_U8_Sat = __mlib_VectorSumAbs_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSumAbs_S8_Sat) mlib_VectorSumAbs_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S8_Sat")));
__typeof__(__mlib_VectorSumAbs_U8_Sat) mlib_VectorSumAbs_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200
#define	SPEC_OPER_UIII(X)	X = vis_fxor(X, fzero)
#else /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */
#define	SPEC_OPER_UIII(X)

#endif /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_d64 *dpx;
	mlib_d64 dx, dx1, dx2, dx3, fzero = vis_fzero();
	mlib_d64 accum = fzero, accum1 = fzero;
	mlib_d64 accum2 = fzero, accum3 = fzero;
	type_union_mlib_d64 dr;
	mlib_s32 i;

	if (n < 8) {
		mlib_s32 s = 0;

		if (n <= 0)
			return (MLIB_FAILURE);

		for (i = 0; i < n; i++)
			s += px[i];
		z[0] = (mlib_d64)s;
		return (MLIB_SUCCESS);
	}

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dx = dpx[0];
	dx = vis_faligndata(dx, fzero);
	SPEC_OPER_UIII(dx);
	accum = vis_pdist(dx, fzero, accum);

	i = 8 - ((mlib_addr)px & 7);

	for (; i <= (n - 32); i += 32) {
		dx = *((mlib_d64 *)(px + i));
		SPEC_OPER_UIII(dx);
		accum = vis_pdist(dx, fzero, accum);
		dx1 = *((mlib_d64 *)(px + i + 8));
		SPEC_OPER_UIII(dx1);
		accum1 = vis_pdist(dx1, fzero, accum1);
		dx2 = *((mlib_d64 *)(px + i + 16));
		SPEC_OPER_UIII(dx2);
		accum2 = vis_pdist(dx2, fzero, accum2);
		dx3 = *((mlib_d64 *)(px + i + 24));
		SPEC_OPER_UIII(dx3);
		accum3 = vis_pdist(dx3, fzero, accum3);
	}

	for (; i <= (n - 8); i += 8) {
		dx = *((mlib_d64 *)(px + i));
		SPEC_OPER_UIII(dx);
		accum = vis_pdist(dx, fzero, accum);
	}

	dx = vis_ld_d64_nf(px + i);
	(void) vis_alignaddr(px, n);
	dx = vis_faligndata(fzero, dx);
	SPEC_OPER_UIII(dx);
	accum = vis_pdist(dx, fzero, accum);

	accum1 = vis_fpadd32(accum1, accum2);
	accum1 = vis_fpadd32(accum1, accum3);
	accum = vis_fpadd32(accum, accum1);
	dr.two_float.fl1 = vis_read_lo(accum);
	z[0] = (mlib_d64)dr.two_int.int1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_d64 *dpx;
	mlib_d64 dx, dx1, dx2, dx3, fzero = vis_fzero();
	mlib_d64 mask = vis_to_double_dup(0x80808080);
	mlib_d64 accum = fzero, accum1 = fzero;
	mlib_d64 accum2 = fzero, accum3 = fzero;
	type_union_mlib_d64 dr;
	mlib_s32 i;

	if (n < 8) {
		mlib_s32 k, src, s = 0;

		if (n <= 0)
			return (MLIB_FAILURE);

		for (i = 0; i < n; i++) {
			src = (mlib_s32)px[i];
			k = (src >> 31);
			s += (src ^ k) - k;
		}

		z[0] = (mlib_d64)s;
		return (MLIB_SUCCESS);
	}

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dx = dpx[0];
	dx = vis_faligndata(dx, fzero);
	dx = vis_fxor(dx, mask);
	accum = vis_pdist(dx, mask, accum);

	i = 8 - ((mlib_addr)px & 7);

	for (; i <= (n - 32); i += 32) {
		dx = *((mlib_d64 *)(px + i));
		dx = vis_fxor(dx, mask);
		accum = vis_pdist(dx, mask, accum);
		dx1 = *((mlib_d64 *)(px + i + 8));
		dx1 = vis_fxor(dx1, mask);
		accum1 = vis_pdist(dx1, mask, accum1);
		dx2 = *((mlib_d64 *)(px + i + 16));
		dx2 = vis_fxor(dx2, mask);
		accum2 = vis_pdist(dx2, mask, accum2);
		dx3 = *((mlib_d64 *)(px + i + 24));
		dx3 = vis_fxor(dx3, mask);
		accum3 = vis_pdist(dx3, mask, accum3);
	}

	for (; i <= (n - 8); i += 8) {
		dx = *((mlib_d64 *)(px + i));
		dx = vis_fxor(dx, mask);
		accum = vis_pdist(dx, mask, accum);
	}

	dx = vis_ld_d64_nf(px + i);
	(void) vis_alignaddr(px, n);
	dx = vis_faligndata(fzero, dx);
	dx = vis_fxor(dx, mask);
	accum = vis_pdist(dx, mask, accum);

	accum1 = vis_fpadd32(accum1, accum2);
	accum1 = vis_fpadd32(accum1, accum3);
	accum = vis_fpadd32(accum, accum1);
	dr.two_float.fl1 = vis_read_lo(accum);
	z[0] = (mlib_d64)dr.two_int.int1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
