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

#pragma ident	"@(#)mlib_v_VectorMaximum.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMaximum - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximum_U8(mlib_u8       *max,
 *                                        const mlib_u8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMaximum_S8(mlib_s8       *max,
 *                                        const mlib_s8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMaximum_S16(mlib_s16       *max,
 *                                         const mlib_s16 *x,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

/*
 * FUNCTIONS
 *      mlib_VectorMaximumMag - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximumMag_U8C(mlib_u8       *max,
 *                                            const mlib_u8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMaximumMag_S8C(mlib_s8       *max,
 *                                            const mlib_s8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMaximumMag_S16C(mlib_s16       *max,
 *                                             const mlib_s16 *x,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *      This function finds the first element with the maximum
 *      magnitude, then puts the real and imaginary parts of it into
 *      max[0] and max[1], respectively.
 *
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>
#include <mlib_v_VectorMaximum.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMaximum_S8 = __mlib_VectorMaximum_S8
#pragma weak mlib_VectorMaximum_U8 = __mlib_VectorMaximum_U8
#pragma weak mlib_VectorMaximum_S16 = __mlib_VectorMaximum_S16
#pragma weak mlib_VectorMaximumMag_S8C = __mlib_VectorMaximumMag_S8C
#pragma weak mlib_VectorMaximumMag_U8C = __mlib_VectorMaximumMag_U8C
#pragma weak mlib_VectorMaximumMag_S16C = __mlib_VectorMaximumMag_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMaximum_S8) mlib_VectorMaximum_S8
	__attribute__((weak, alias("__mlib_VectorMaximum_S8")));
__typeof__(__mlib_VectorMaximum_U8) mlib_VectorMaximum_U8
	__attribute__((weak, alias("__mlib_VectorMaximum_U8")));
__typeof__(__mlib_VectorMaximum_S16) mlib_VectorMaximum_S16
	__attribute__((weak, alias("__mlib_VectorMaximum_S16")));
__typeof__(__mlib_VectorMaximumMag_S8C) mlib_VectorMaximumMag_S8C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S8C")));
__typeof__(__mlib_VectorMaximumMag_U8C) mlib_VectorMaximumMag_U8C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_U8C")));
__typeof__(__mlib_VectorMaximumMag_S16C) mlib_VectorMaximumMag_S16C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 db;
	struct
	{
		mlib_s32 int0, int1;
	} two_int;
	struct
	{
		mlib_s16 sh0, sh1, sh2, sh3;
	} four_short;

} type_union_d64;

/* *********************************************************** */

const mlib_u32 mlib_bmask_table32[] = {
	0x89abcdef, 0x89ab4567,
	0x0123cdef, 0x01234567
};

/* *********************************************************** */

const mlib_u32 mlib_bmask_table16[] = {
	0x89abcdef, 0x89abcd67, 0x89ab45ef, 0x89ab4567,
	0x8923cdef, 0x8923cd67, 0x892345ef, 0x89234567,
	0x01abcdef, 0x01abcd67, 0x01ab45ef, 0x01ab4567,
	0x0123cdef, 0x0123cd67, 0x012345ef, 0x01234567
};

/* *********************************************************** */

#define	MAX_U8                                                       \
	dr = vis_fand(dlow_bytes, dx);                               \
	db0 = vis_fmuld8ulx16(vis_read_hi(dr), vis_read_hi(dr));     \
	db1 = vis_fmuld8ulx16(vis_read_lo(dr), vis_read_lo(dr));     \
	dr5 = vis_faligndata(dx, dx);                                \
	/*                                                           \
	 * shift 1 byte right                                        \
	 */                                                          \
	dr5 = vis_fand(dlow_bytes, dr5);                             \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dr5), vis_read_hi(dr5));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dr5), vis_read_lo(dr5));   \
	db0 = vis_fpadd32(db0, dr3);                                 \
	db1 = vis_fpadd32(db1, dr4)

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_U8C(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *pxend, *pxend2;
	mlib_d64 *dpx;
	mlib_d64 dx, dx0, dx1, dr, dr3, dr4, dr5;
	type_union_mlib_d64 dr1;
	mlib_d64 dlow_bytes = vis_to_double_dup(0x00ff00ff);
	mlib_d64 db0, db1;
	mlib_d64 max_val0, max_val1, max_dig;
	mlib_u32 *bmask_table1 = (mlib_u32 *)mlib_bmask_table32;
	mlib_s32 bmask1, bmask2;
	mlib_s32 mask1, mask2, mask3, mask4;
	mlib_s32 maxd, emask, off;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_val0 = max_val1 = 0.0;
	max_dig = 0.0;

/* prepare the destination address */
	pxend = px + n + n - 1;
	pxend2 = (mlib_u8 *)((mlib_addr)pxend & ~7);

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	off = (mlib_addr)px & 7;

	if (off & 1) {

		dx1 = vis_ld_d64_nf(dpx);
		dx0 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx1, dx0);

		emask = vis_edge8(px, pxend);
		emask = (emask & ~1) << off;
		mask1 = ((emask >> 6) & 2) | ((emask >> 5) & 1);
		mask2 = ((emask >> 2) & 2);

		(void) vis_alignaddr(dpx, 7);

		MAX_U8;

		bmask1 = bmask_table1[mask1];
		vis_write_bmask(bmask1, 0);
		max_val0 = vis_bshuffle(db0, max_val0);

		bmask2 = bmask_table1[mask2];
		vis_write_bmask(bmask2, 0);
		max_val1 = vis_bshuffle(db1, max_val1);

		vis_pst_8(dx, (void *)&max_dig, emask);

		px = (mlib_u8 *)dpx + 8;

#pragma pipeloop(0)
		for (; px < pxend2; ) {
			dx0 = dx1;
			dx1 = ((mlib_d64 *)px)[0];
			dx = vis_faligndata(dx0, dx1);
			px += 8;
			MAX_U8;
			mask3 = vis_fcmpgt32(db0, max_val0);
			mask4 = vis_fcmpgt32(db1, max_val1);
			bmask1 = bmask_table1[mask3];
			vis_write_bmask(bmask1, 0);
			max_val0 = vis_bshuffle(db0, max_val0);
			bmask2 = bmask_table1[mask4];
			vis_write_bmask(bmask2, 0);
			max_val1 = vis_bshuffle(db1, max_val1);
			vis_pst_16(dx, (void *)&max_dig, (mask3 << 2) + mask4);
		}

		if (px <= pxend) {
			emask = vis_edge16(px, pxend);
			mask1 = emask >> 2;
			mask2 = emask & 3;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(px);
			dx = vis_faligndata(dx0, dx1);
			MAX_U8;
			mask3 = vis_fcmpgt32(db0, max_val0);
			mask4 = vis_fcmpgt32(db1, max_val1);
			mask3 &= mask1;
			mask4 &= mask2;
			bmask1 = bmask_table1[mask3];
			vis_write_bmask(bmask1, 0);
			max_val0 = vis_bshuffle(db0, max_val0);
			bmask2 = bmask_table1[mask4];
			vis_write_bmask(bmask2, 0);
			max_val1 = vis_bshuffle(db1, max_val1);
			vis_pst_16(dx, (void *)&max_dig, (mask3 << 2) + mask4);
		}
	} else {

		dx = vis_ld_d64_nf(dpx);

		emask = vis_edge16(px, pxend);
		mask1 = emask >> 2;
		mask2 = emask & 3;

		(void) vis_alignaddr(dpx, 7);

		MAX_U8;

		bmask1 = bmask_table1[mask1];
		vis_write_bmask(bmask1, 0);
		max_val0 = vis_bshuffle(db0, max_val0);

		bmask2 = bmask_table1[mask2];
		vis_write_bmask(bmask2, 0);
		max_val1 = vis_bshuffle(db1, max_val1);

		vis_pst_16(dx, (void *)&max_dig, emask);

		px = (mlib_u8 *)dpx + 8;

#pragma pipeloop(0)
		for (; px < pxend2; ) {
			dx = ((mlib_d64 *)px)[0];
			px += 8;
			MAX_U8;
			mask3 = vis_fcmpgt32(db0, max_val0);
			mask4 = vis_fcmpgt32(db1, max_val1);
			bmask1 = bmask_table1[mask3];
			vis_write_bmask(bmask1, 0);
			max_val0 = vis_bshuffle(db0, max_val0);
			bmask2 = bmask_table1[mask4];
			vis_write_bmask(bmask2, 0);
			max_val1 = vis_bshuffle(db1, max_val1);
			vis_pst_16(dx, (void *)&max_dig, (mask3 << 2) + mask4);
		}

		if (px < pxend) {
			emask = vis_edge16(px, pxend);
			mask1 = emask >> 2;
			mask2 = emask & 3;
			dx = vis_ld_d64_nf(px);
			MAX_U8;
			mask3 = vis_fcmpgt32(db0, max_val0);
			mask4 = vis_fcmpgt32(db1, max_val1);
			mask3 &= mask1;
			mask4 &= mask2;
			bmask1 = bmask_table1[mask3];
			vis_write_bmask(bmask1, 0);
			max_val0 = vis_bshuffle(db0, max_val0);
			bmask2 = bmask_table1[mask4];
			vis_write_bmask(bmask2, 0);
			max_val1 = vis_bshuffle(db1, max_val1);
			vis_pst_16(dx, (void *)&max_dig, (mask3 << 2) + mask4);
		}
	}

	mask3 = vis_fcmpgt32(max_val1, max_val0);
	bmask1 = bmask_table1[mask3];
	vis_write_bmask(bmask1, 0);
	dr1.db = vis_bshuffle(max_val1, max_val0);

	if (dr1.two_int.int1 > dr1.two_int.int0) {
		maxd = ((mlib_s16 *)&max_dig)[(mask3 & 1) * 2 + 1];
	} else {
		maxd = ((mlib_s16 *)&max_dig)[mask3 & 2];
	}

	max[0] = maxd >> 8;
	max[1] = maxd;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAX_S8                                                  \
	db0 = vis_fandnot(dlow_bytes, dx);                      \
	db0 = vis_fmul8sux16(db0, db0);                         \
	db1 = vis_fpack32(dx, dx);                              \
	db1 = vis_fandnot(dlow_bytes, db1);                     \
	db1 = vis_fmul8sux16(db1, db1);                         \
	db0 = vis_fpadd16(db0, db1);                            \
	db0 = vis_fxor(db0, xor)

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_S8C(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *pxend, *pxend2;
	mlib_d64 *dpx;
	mlib_d64 dx, dx0, dx1;
	type_union_d64 dr0;
	mlib_d64 dlow_bytes = vis_to_double_dup(0x00ff00ff);
	mlib_d64 db0, db1;
	mlib_d64 max_val, max_dig;
	mlib_d64 xor = vis_to_double_dup(0x80008000);
	mlib_u32 *bmask_table = (mlib_u32 *)mlib_bmask_table16;
	mlib_s32 mask1, mask2, emask, maxd, off;
	mlib_s32 bmask1;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_dig = 0.0;
	max_val = xor;

/* prepare the destination address */
	pxend = px + n + n - 1;
	pxend2 = (mlib_s8 *)((mlib_addr)pxend & ~7);

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	off = (mlib_addr)px & 7;

	if (off & 1) {

		dx1 = vis_ld_d64_nf(dpx);
		dx0 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx1, dx0);

		emask = vis_edge8(px, pxend);
		emask = (emask & ~1) << off;
		mask2 = (((emask >> 4) & 8) | ((emask >> 3) & 4) | ((emask >> 2)
			& 2) | ((emask >> 1) & 1));

		(void) vis_alignaddr(dpx, 7);

		MAX_S8;

		bmask1 = bmask_table[mask2];
		vis_write_bmask(bmask1, 0);
		max_val = vis_bshuffle(db0, max_val);
		max_dig = vis_bshuffle(dx, max_dig);

		px = (mlib_s8 *)dpx + 8;

#pragma pipeloop(0)
		for (; px < pxend2; ) {
			dx0 = dx1;
			dx1 = ((mlib_d64 *)px)[0];
			dx = vis_faligndata(dx0, dx1);
			px += 8;
			MAX_S8;
			mask1 = vis_fcmpgt16(db0, max_val);
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}

		if (px <= pxend) {
			mask2 = vis_edge16(px, pxend);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(px);
			dx = vis_faligndata(dx0, dx1);
			MAX_S8;
			mask1 = vis_fcmpgt16(db0, max_val);
			mask1 &= mask2;
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}
	} else {

		dx = vis_ld_d64_nf(dpx);

		mask2 = vis_edge16(px, pxend);

		MAX_S8;

		bmask1 = bmask_table[mask2];
		vis_write_bmask(bmask1, 0);
		max_val = vis_bshuffle(db0, max_val);
		max_dig = vis_bshuffle(dx, max_dig);

		px = (mlib_s8 *)dpx + 8;

#pragma pipeloop(0)
		for (; px < pxend2; ) {
			dx = ((mlib_d64 *)px)[0];
			px += 8;
			MAX_S8;
			mask1 = vis_fcmpgt16(db0, max_val);
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}

		if (px < pxend) {
			mask2 = vis_edge16(px, pxend);
			dx = vis_ld_d64_nf(px);
			MAX_S8;
			mask1 = vis_fcmpgt16(db0, max_val);
			mask1 &= mask2;
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}
	}

	db1 = vis_write_hi(max_val, vis_read_lo(max_val));
	db0 = vis_write_hi(max_dig, vis_read_lo(max_dig));

	mask1 = vis_fcmpgt16(db1, max_val);

	bmask1 = bmask_table[mask1];
	vis_write_bmask(bmask1, 0);
	dr0.db = vis_bshuffle(db1, max_val);
	max_dig = vis_bshuffle(db0, max_dig);

	if (dr0.four_short.sh1 > dr0.four_short.sh0) {
		maxd = ((mlib_s16 *)&max_dig)[1];
	} else {
		maxd = ((mlib_s16 *)&max_dig)[0];
	}

	max[0] = maxd >> 8;
	max[1] = maxd;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAX_S16                                                           \
	dr2 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dx));          \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dx));          \
	dr0 = vis_fpadd32(dr2, dr3);                                      \
	dr4 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dx));          \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dx));          \
	dr1 = vis_fpadd32(dr4, dr5);                                      \
	db0 =                                                             \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr0), vis_read_lo(dr0)),   \
		vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1)));        \
	db0 = vis_fxor(db0, xor)

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_S16C(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *pxend, *pxend2;
	mlib_d64 *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr0, dr1, dr2, dr3, dr4, dr5, db0;
	mlib_d64 max_val, max_dig;
	mlib_d64 xor = vis_to_double_dup(0x80000000);
	mlib_u32 *bmask_table = (mlib_u32 *)mlib_bmask_table32;
	mlib_s32 mask1, mask2, emask, off, maxd, m0, m1;
	mlib_s32 bmask1;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_dig = 0.0;
	max_val = xor;

/* prepare the destination address */
	pxend = px + n + n - 1;
	pxend2 = (mlib_s16 *)((mlib_addr)pxend & ~7);

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	off = (mlib_addr)px & 7;

	if (off & 2) {

		dx1 = vis_ld_d64_nf(dpx);
		dx0 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx1, dx0);

		emask = vis_edge16(px, pxend);
		mask2 = (emask >> 1) & 2;
		emask = (emask & ~1) << 1;

		(void) vis_alignaddr(dpx, 6);

		MAX_S16;

		bmask1 = bmask_table[mask2];
		vis_write_bmask(bmask1, 0);
		max_val = vis_bshuffle(db0, max_val);
		max_dig = vis_bshuffle(dx, max_dig);

		px = (mlib_s16 *)dpx + 4;

#pragma pipeloop(0)
		for (; px < pxend2; ) {
			dx0 = dx1;
			dx1 = ((mlib_d64 *)px)[0];
			dx = vis_faligndata(dx0, dx1);
			px += 4;
			MAX_S16;
			mask1 = vis_fcmpgt32(db0, max_val);
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}

		if (px <= pxend) {
			mask2 = vis_edge32(px, pxend);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(px);
			dx = vis_faligndata(dx0, dx1);
			MAX_S16;
			mask1 = vis_fcmpgt32(db0, max_val);
			mask1 &= mask2;
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}
	} else {

		dx = vis_ld_d64_nf(dpx);

		mask2 = vis_edge32(px, pxend);

		MAX_S16;

		bmask1 = bmask_table[mask2];
		vis_write_bmask(bmask1, 0);
		max_val = vis_bshuffle(db0, max_val);
		max_dig = vis_bshuffle(dx, max_dig);

		px = (mlib_s16 *)dpx + 4;

#pragma pipeloop(0)
		for (; px < pxend2; ) {
			dx = ((mlib_d64 *)px)[0];
			px += 4;
			MAX_S16;
			mask1 = vis_fcmpgt32(db0, max_val);
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}

		if (px < pxend) {
			mask2 = vis_edge32(px, pxend);
			dx = vis_ld_d64_nf(px);
			MAX_S16;
			mask1 = vis_fcmpgt32(db0, max_val);
			mask1 &= mask2;
			bmask1 = bmask_table[mask1];
			vis_write_bmask(bmask1, 0);
			max_val = vis_bshuffle(db0, max_val);
			max_dig = vis_bshuffle(dx, max_dig);
		}
	}

	m0 = *((mlib_s32 *)&max_val);
	m1 = *((mlib_s32 *)&max_val + 1);

	if (m1 > m0) {
		maxd = *((mlib_s32 *)&max_dig + 1);
	} else {
		maxd = *((mlib_s32 *)&max_dig);
	}

	max[0] = maxd >> 16;
	max[1] = maxd;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_U8(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *pxend, *pxend2;
	mlib_d64 *dpx;
	mlib_d64 dx;
	type_union_d64 dr0;
	mlib_d64 max_val0, max_val1;
	mlib_d64 sd0, sd1;
	mlib_u32 *bmask_table = (mlib_u32 *)mlib_bmask_table16;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s32 mask1, mask2, emask, m0, m1, bmask1, bmask2;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_val0 = max_val1 = 0.0;

/* prepare the destination address */
	pxend = px + n - 1;
	pxend2 = (mlib_u8 *)((mlib_addr)pxend & ~7);

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dx = vis_ld_d64_nf(dpx);

	emask = vis_edge8(px, pxend);

	sd0 = vis_fmul8x16al(vis_read_hi(dx), fone);
	sd1 = vis_fmul8x16al(vis_read_lo(dx), fone);

	bmask1 = bmask_table[emask >> 4];
	vis_write_bmask(bmask1, 0);
	max_val0 = vis_bshuffle(sd0, max_val0);

	bmask2 = bmask_table[emask & 0xf];
	vis_write_bmask(bmask2, 0);
	max_val1 = vis_bshuffle(sd1, max_val1);

	dpx++;

#pragma pipeloop(0)
	for (; (mlib_u8 *)dpx < pxend2; ) {
		dx = dpx[0];
		sd0 = vis_fmul8x16al(vis_read_hi(dx), fone);
		sd1 = vis_fmul8x16al(vis_read_lo(dx), fone);
		mask1 = vis_fcmplt16(max_val0, sd0);
		mask2 = vis_fcmplt16(max_val1, sd1);
		bmask1 = bmask_table[mask1];
		vis_write_bmask(bmask1, 0);
		max_val0 = vis_bshuffle(sd0, max_val0);
		bmask2 = bmask_table[mask2];
		vis_write_bmask(bmask2, 0);
		max_val1 = vis_bshuffle(sd1, max_val1);
		dpx++;
	}

	if ((mlib_u8 *)dpx <= pxend) {
		dx = vis_ld_d64_nf(dpx);
		sd0 = vis_fmul8x16al(vis_read_hi(dx), fone);
		sd1 = vis_fmul8x16al(vis_read_lo(dx), fone);
		mask1 = vis_fcmplt16(max_val0, sd0);
		mask2 = vis_fcmplt16(max_val1, sd1);
		mask1 &= (vis_edge8(dpx, pxend) >> 4);
		mask2 &= (vis_edge8(dpx, pxend) & 0xf);
		bmask1 = bmask_table[mask1];
		vis_write_bmask(bmask1, 0);
		max_val0 = vis_bshuffle(sd0, max_val0);
		bmask2 = bmask_table[mask2];
		vis_write_bmask(bmask2, 0);
		max_val1 = vis_bshuffle(sd1, max_val1);
	}

	mask1 = vis_fcmplt16(max_val0, max_val1);
	bmask1 = bmask_table[mask1];
	vis_write_bmask(bmask1, 0);
	max_val0 = vis_bshuffle(max_val1, max_val0);

	sd0 = vis_write_hi(max_val0, vis_read_lo(max_val0));

	mask1 = vis_fcmpgt16(sd0, max_val0);
	bmask1 = bmask_table[mask1];
	vis_write_bmask(bmask1, 0);
	dr0.db = vis_bshuffle(sd0, max_val0);

	m0 = dr0.four_short.sh0;
	m1 = dr0.four_short.sh1;

	if (m1 > m0) {
		max[0] = m1;
	} else {
		max[0] = m0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S8(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *pxend, *pxend2;
	mlib_d64 *dpx;
	mlib_d64 dx;
	type_union_d64 dr0;
	mlib_d64 max_val0, max_val1;
	mlib_d64 sd0, sd1;
	mlib_u32 *bmask_table = (mlib_u32 *)mlib_bmask_table16;
	mlib_d64 done = vis_to_double_dup(0x1000100);
	mlib_s32 mask1, mask2, emask, m0, m1, bmask1, bmask2;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_val0 = max_val1 = vis_to_double_dup(0x80008000);

/* prepare the destination address */
	pxend = px + n - 1;
	pxend2 = (mlib_s8 *)((mlib_addr)pxend & ~7);

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dx = vis_ld_d64_nf(dpx);

	emask = vis_edge8(px, pxend);

	sd0 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(dx), vis_read_hi(dx)),
		done);
	sd1 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(dx), vis_read_lo(dx)),
		done);

	bmask1 = bmask_table[emask >> 4];
	vis_write_bmask(bmask1, 0);
	max_val0 = vis_bshuffle(sd0, max_val0);

	bmask2 = bmask_table[emask & 0xf];
	vis_write_bmask(bmask2, 0);
	max_val1 = vis_bshuffle(sd1, max_val1);

	dpx++;

#pragma pipeloop(0)
	for (; (mlib_s8 *)dpx < pxend2; ) {
		dx = dpx[0];
		sd0 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(dx),
			vis_read_hi(dx)), done);
		sd1 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(dx),
			vis_read_lo(dx)), done);
		mask1 = vis_fcmplt16(max_val0, sd0);
		mask2 = vis_fcmplt16(max_val1, sd1);
		bmask1 = bmask_table[mask1];
		vis_write_bmask(bmask1, 0);
		bmask2 = bmask_table[mask2];
		max_val0 = vis_bshuffle(sd0, max_val0);
		vis_write_bmask(bmask2, 0);
		max_val1 = vis_bshuffle(sd1, max_val1);
		dpx++;
	}

	if ((mlib_s8 *)dpx <= pxend) {
		dx = vis_ld_d64_nf(dpx);
		sd0 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(dx),
			vis_read_hi(dx)), done);
		sd1 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(dx),
			vis_read_lo(dx)), done);
		mask1 = vis_fcmplt16(max_val0, sd0);
		mask2 = vis_fcmplt16(max_val1, sd1);
		mask1 &= (vis_edge8(dpx, pxend) >> 4);
		mask2 &= (vis_edge8(dpx, pxend) & 0xf);
		bmask1 = bmask_table[mask1];
		vis_write_bmask(bmask1, 0);
		max_val0 = vis_bshuffle(sd0, max_val0);
		bmask2 = bmask_table[mask2];
		vis_write_bmask(bmask2, 0);
		max_val1 = vis_bshuffle(sd1, max_val1);
	}

	mask1 = vis_fcmplt16(max_val0, max_val1);
	bmask1 = bmask_table[mask1];
	vis_write_bmask(bmask1, 0);
	max_val0 = vis_bshuffle(max_val1, max_val0);

	sd0 = vis_write_hi(max_val0, vis_read_lo(max_val0));

	mask1 = vis_fcmpgt16(sd0, max_val0);
	bmask1 = bmask_table[mask1];
	vis_write_bmask(bmask1, 0);
	dr0.db = vis_bshuffle(sd0, max_val0);

	m0 = dr0.four_short.sh0;
	m1 = dr0.four_short.sh1;

	if (m1 > m0) {
		max[0] = m1;
	} else {
		max[0] = m0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S16(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *pxend, *pxend2;
	mlib_d64 *dpx;
	mlib_d64 dx;
	type_union_d64 dr0;
	mlib_d64 max_val, sd0;
	mlib_u32 *bmask_table = (mlib_u32 *)mlib_bmask_table16;
	mlib_s32 mask1, emask, bmask, m0, m1;

	if (n <= 0)
		return (MLIB_FAILURE);

	max_val = vis_to_double_dup(0x80008000);

/* prepare the destination address */
	pxend = px + n - 1;
	pxend2 = (mlib_s16 *)((mlib_addr)pxend & ~7);

/* prepare the source address */
	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dx = vis_ld_d64_nf(dpx);

	emask = vis_edge16(px, pxend);

	bmask = bmask_table[emask];
	vis_write_bmask(bmask, 0);
	max_val = vis_bshuffle(dx, max_val);
	dpx++;

#pragma pipeloop(0)
	for (; (mlib_s16 *)dpx < pxend2; ) {
		sd0 = *dpx;
		mask1 = vis_fcmplt16(max_val, sd0);
		bmask = bmask_table[mask1];
		vis_write_bmask(bmask, 0);
		max_val = vis_bshuffle(sd0, max_val);
		dpx++;
	}

	if ((mlib_s16 *)dpx <= pxend) {
		sd0 = vis_ld_d64_nf(dpx);
		mask1 = vis_fcmplt16(max_val, sd0);
		mask1 &= vis_edge16(dpx, pxend);
		bmask = bmask_table[mask1];
		vis_write_bmask(bmask, 0);
		max_val = vis_bshuffle(sd0, max_val);
	}

	sd0 = vis_write_hi(max_val, vis_read_lo(max_val));

	mask1 = vis_fcmpgt16(sd0, max_val);
	bmask = bmask_table[mask1];
	vis_write_bmask(bmask, 0);
	dr0.db = vis_bshuffle(sd0, max_val);

	m0 = dr0.four_short.sh0;
	m1 = dr0.four_short.sh1;

	if (m1 > m0) {
		max[0] = m1;
	} else {
		max[0] = m0;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
