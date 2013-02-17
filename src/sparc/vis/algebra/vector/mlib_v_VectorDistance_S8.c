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

#pragma ident	"@(#)mlib_v_VectorDistance_S8.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorDistance_[U8|S8|S16|S32]_Sat - Euclidean distance
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDistance_S8_Sat(mlib_d64      * z,
 *                                             const mlib_s8 * x,
 *                                             const mlib_s8 * y,
 *                                             mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the Euclidean distance
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *                n - 1
 *      z = sqrt(SUM (x[i] - y[i])^2)
 *                i = 0
 */

#include <mlib_algebra.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDistance_S8_Sat = __mlib_VectorDistance_S8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorDistance_S8_Sat) mlib_VectorDistance_S8_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_VectorDistance_S8_Sat_C(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n);

static void mlib_v_VectorDistance_S8_Sat_A8(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n);

static void mlib_v_VectorDistance_S8_Sat_NA(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n);

#pragma no_inline(mlib_v_VectorDistance_S8_Sat_C)
#pragma no_inline(mlib_v_VectorDistance_S8_Sat_A8)
#pragma no_inline(mlib_v_VectorDistance_S8_Sat_NA)

/* *********************************************************** */

mlib_status
__mlib_VectorDistance_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 8) {

		if (n <= 0)
			return (MLIB_FAILURE);
		mlib_v_VectorDistance_S8_Sat_C(z, x, y, n);
		return (MLIB_SUCCESS);
	}

	if ((((mlib_addr)x | (mlib_addr)y) & 0x7) == 0) {

		mlib_v_VectorDistance_S8_Sat_A8(z, x, y, n);
	} else {

		mlib_v_VectorDistance_S8_Sat_NA(z, x, y, n);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_v_VectorDistance_S8_Sat_C(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px, *py;
	mlib_d64 acc, item;
	mlib_s32 i;

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	acc = 0.0;
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		item = (mlib_d64)(*px - *py);
		acc += item * item;
		px++;
		py++;
	}

	z[0] = mlib_sqrt(acc);
}

/* *********************************************************** */

#define	MLIB_V_SQR_S8_A8(src1, src2, acc)                            \
	dsrc1l = vis_fmul8sux16(vis_faligndata(src1, src1), ones);   \
	dsrc2l = vis_fmul8sux16(vis_faligndata(src2, src2), ones)

#define	MLIB_V_SQR_S8_NA(src1, src2, acc)                         \
	dsrc1l = vis_fmul8sux16(vis_fpack32(src1, src1), ones);   \
	dsrc2l = vis_fmul8sux16(vis_fpack32(src2, src2), ones)

/* *********************************************************** */

#define	MLIB_V_VECTORDISTANCE(src1, src2, acc)                  \
	dsrc1h = vis_fmul8sux16(src1, ones);                    \
	dsrc2h = vis_fmul8sux16(src2, ones);                    \
	ddiffh = vis_fpsub16(dsrc1h, dsrc2h);                   \
	ddiffl = vis_fpsub16(dsrc1l, dsrc2l);                   \
	fdifhh = vis_read_hi(ddiffh);                           \
	fdifhl = vis_read_lo(ddiffh);                           \
	fdiflh = vis_read_hi(ddiffl);                           \
	fdifll = vis_read_lo(ddiffl);                           \
	dsqrhh =                                                \
	vis_fpadd32(vis_fmuld8sux16(fdifhh, fdifhh),            \
		vis_fmuld8ulx16(fdifhh, fdifhh));               \
	dsqrhl =                                                \
	vis_fpadd32(vis_fmuld8sux16(fdifhl, fdifhl),            \
		vis_fmuld8ulx16(fdifhl, fdifhl));               \
	dsqrlh =                                                \
	vis_fpadd32(vis_fmuld8sux16(fdiflh, fdiflh),            \
		vis_fmuld8ulx16(fdiflh, fdiflh));               \
	dsqrll =                                                \
	vis_fpadd32(vis_fmuld8sux16(fdifll, fdifll),            \
		vis_fmuld8ulx16(fdifll, fdifll));               \
	dsqrh = vis_fpadd32(dsqrhh, dsqrhl);                    \
	dsqrl = vis_fpadd32(dsqrlh, dsqrll);                    \
	dsqr = vis_fpadd32(dsqrh, dsqrl);                       \
	acc = vis_fpadd32(acc, dsqr)

/* *********************************************************** */

#define	MLIB_V_VECTORDISTANCE_S8_A8(src1, src2, acc)            \
	MLIB_V_SQR_S8_A8(src1, src2, acc);                      \
	MLIB_V_VECTORDISTANCE(src1, src2, acc)

#define	MLIB_V_VECTORDISTANCE_S8_NA(src1, src2, acc)            \
	MLIB_V_SQR_S8_NA(src1, src2, acc);                      \
	MLIB_V_VECTORDISTANCE(src1, src2, acc)

/* *********************************************************** */

static void
mlib_v_VectorDistance_S8_Sat_A8(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_d64 *pdx, dx;
	mlib_d64 *pdy, dy;
	mlib_s8 *px, *py;
	mlib_d64 accd, accum;
	mlib_s32 *pacc;
	mlib_d64 item;
	mlib_d64 ones = vis_to_double_dup(0x01000100);

/* temporaries used in macro */
	mlib_d64 dsrc1h, dsrc1l;

/* temporaries used in macro */
	mlib_d64 dsrc2h, dsrc2l;

/* temporaries used in macro */
	mlib_d64 ddiffh, ddiffl;

/* temporaries used in macro */
	mlib_f32 fdifhh, fdifhl;

/* temporaries used in macro */
	mlib_f32 fdiflh, fdifll;

/* temporaries used in macro */
	mlib_d64 dsqrhh, dsqrhl;

/* temporaries used in macro */
	mlib_d64 dsqrlh, dsqrll;

/* temporaries used in macro */
	mlib_d64 dsqrh, dsqrl;

/* temporaries used in macro */
	mlib_d64 dsqr;
	mlib_s32 i, nd8, nm8;

	nd8 = n >> 3;
	nm8 = n & 0x7;

	pdx = (mlib_d64 *)x;
	pdy = (mlib_d64 *)y;
	accd = 0.0;
	vis_alignaddr(pdx, 1);

/* #pragma pipeloop(0) */
	for (i = 0; i < nd8; i++) {
		dx = *pdx;
		pdx++;
		dy = *pdy;
		pdy++;
		MLIB_V_VECTORDISTANCE_S8_A8(dx, dy, accd);
	}

	fdifhh = vis_fpadd32s(vis_read_hi(accd), vis_read_lo(accd));
	pacc = (mlib_s32 *)&fdifhh;
	accum = (mlib_d64)pacc[0];

	if (nm8 != 0) {
		px = (mlib_s8 *)pdx;
		py = (mlib_s8 *)pdy;
		for (i = 0; i < nm8; i++) {
			item = (mlib_d64)(*px - *py);
			accum += item * item;
			px++;
			py++;
		}
	}

	z[0] = mlib_sqrt(accum);
}

/* *********************************************************** */

static void
mlib_v_VectorDistance_S8_Sat_NA(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_d64 *pdx, dx, dx0, dx1;
	mlib_d64 *pdy, dy;
	mlib_s8 *px, *py;
	mlib_d64 accd, accum;
	mlib_s32 *pacc;
	mlib_d64 item;
	mlib_d64 ones = vis_to_double_dup(0x01000100);

/* temporaries used in macro */
	mlib_d64 dsrc1h, dsrc1l;

/* temporaries used in macro */
	mlib_d64 dsrc2h, dsrc2l;

/* temporaries used in macro */
	mlib_d64 ddiffh, ddiffl;

/* temporaries used in macro */
	mlib_f32 fdifhh, fdifhl;

/* temporaries used in macro */
	mlib_f32 fdiflh, fdifll;

/* temporaries used in macro */
	mlib_d64 dsqrhh, dsqrhl;

/* temporaries used in macro */
	mlib_d64 dsqrlh, dsqrll;

/* temporaries used in macro */
	mlib_d64 dsqrh, dsqrl;

/* temporaries used in macro */
	mlib_d64 dsqr;
	mlib_s32 off;
	mlib_s32 i, nd8, nm8;

	accum = 0.0;

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;

	pdy = (mlib_d64 *)((mlib_addr)py & (~7));
	off = (mlib_addr)pdy - (mlib_addr)py;

	if (off != 0) {
		off += 8;
		for (i = 0; i < off; i++) {
			item = (mlib_d64)(*px - *py);
			accum += item * item;
			px++;
			py++;
		}
	}

	nd8 = (n - off) >> 3;
	nm8 = (n - off) & 0x7;

	pdx = (mlib_d64 *)vis_alignaddr(px, 0);
	pdy = (mlib_d64 *)py;
	accd = 0.0;

/* #pragma pipeloop(0) */
	for (i = 0; i < nd8; i++) {
		dx0 = pdx[0];
		dx1 = vis_ld_d64_nf(pdx+1);
		pdx++;
		dx = vis_faligndata(dx0, dx1);
		dy = *pdy;
		pdy++;
		MLIB_V_VECTORDISTANCE_S8_NA(dx, dy, accd);
	}

	fdifhh = vis_fpadd32s(vis_read_hi(accd), vis_read_lo(accd));
	pacc = (mlib_s32 *)&fdifhh;
	accum += (mlib_d64)pacc[0];

	if (nm8 != 0) {
		px += nd8 * 8;
		py = (mlib_s8 *)pdy;
		for (i = 0; i < nm8; i++) {
			item = (mlib_d64)(*px - *py);
			accum += item * item;
			px++;
			py++;
		}
	}

	z[0] = mlib_sqrt(accum);
}

/* *********************************************************** */
