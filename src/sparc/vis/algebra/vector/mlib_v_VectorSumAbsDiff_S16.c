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

#pragma ident	"@(#)mlib_v_VectorSumAbsDiff_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorSumAbsDiff_[U8|S8|S16|S32]_Sat - sum of absolute difference
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSumAbsDiff_S16_Sat(mlib_d64       *z,
 *                                                const mlib_s16 *x,
 *                                                const mlib_s16 *y,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute difference
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *          n - 1
 *      z = SUM |x[i] - y[i]|
 *          i = 0
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbsDiff_S16_Sat = __mlib_VectorSumAbsDiff_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSumAbsDiff_S16_Sat) mlib_VectorSumAbsDiff_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* *********************************************************** */

#define	ABS_S24(src, dst)                                              \
	dsign = vis_fpmerge(vis_read_hi(src), vis_read_lo(src));       \
	dsign = vis_fpmerge(vis_read_hi(dsign), vis_read_hi(dsign));   \
	dsign = vis_fmuld8ulx16(ones, vis_read_hi(dsign));             \
	dst = vis_fpsub32(vis_fxor(dsign, src), dsign)

/* *********************************************************** */

#define	MLIB_V_VECTORSUMABSDIFF_S16(src1, src2, dst)            \
	dsrc1h = vis_fmuld8ulx16(ones, vis_read_hi(src1));      \
	dsrc1l = vis_fmuld8ulx16(ones, vis_read_lo(src1));      \
	dsrc2h = vis_fmuld8ulx16(ones, vis_read_hi(src2));      \
	dsrc2l = vis_fmuld8ulx16(ones, vis_read_lo(src2));      \
	diffh = vis_fpsub32(dsrc1h, dsrc2h);                    \
	diffl = vis_fpsub32(dsrc1l, dsrc2l);                    \
	ABS_S24(diffh, dabsh);                                  \
	ABS_S24(diffl, dabsl);                                  \
	dst = vis_fpadd32(dabsh, dst);                          \
	dst = vis_fpadd32(dabsl, dst)

/* *********************************************************** */

static void
mlib_v_VectorSumAbsDiff_S16_Sat_A8(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *pdx, dx;
	mlib_d64 *pdy, dy;
	mlib_s16 *px, *py;
	mlib_d64 accd, accum;
	mlib_s32 *pacc;
	mlib_f32 ones = vis_to_float(0x00010001);

/* temporaries used in macro */
	mlib_d64 dsign;

/* temporaries used in macro */
	mlib_d64 dsrc1h, dsrc1l;

/* temporaries used in macro */
	mlib_d64 dsrc2h, dsrc2l;

/* temporaries used in macro */
	mlib_d64 diffh, diffl;

/* temporaries used in macro */
	mlib_d64 dabsh, dabsl;
	mlib_s32 accs = 0, item, sign;
	mlib_s32 i, nd4, nm4;

	nd4 = n >> 3;
	nm4 = n & 0x7;

	pdx = (mlib_d64 *)x;
	pdy = (mlib_d64 *)y;
	accd = 0.0;
#pragma pipeloop(0)
	for (i = 0; i < nd4; i++) {
		dx = *pdx;
		dy = *pdy;
		MLIB_V_VECTORSUMABSDIFF_S16(dx, dy, accd);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[4] -
			((mlib_s16 *)pdy)[4]);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[5] -
			((mlib_s16 *)pdy)[5]);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[6] -
			((mlib_s16 *)pdy)[6]);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[7] -
			((mlib_s16 *)pdy)[7]);
		pdx += 2;
		pdy += 2;
	}

	ones = vis_fpadd32s(vis_read_hi(accd), vis_read_lo(accd));
	pacc = (mlib_s32 *)&ones;
	accum = (mlib_d64)pacc[0];

	if (nm4 != 0) {
		px = (mlib_s16 *)pdx;
		py = (mlib_s16 *)pdy;
		for (i = 0; i < nm4; i++) {
			item = (mlib_s32)(*px - *py);
			sign = item >> 31;
			accs += (item ^ sign) - sign;
			px++;
			py++;
		}
	}

	z[0] = accum + (mlib_d64)accs;
}

/* *********************************************************** */

static void
mlib_v_VectorSumAbsDiff_S16_Sat_NA(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *pdx, dx, dx0, dx1;
	mlib_d64 *pdy, dy;
	mlib_s16 *px, *py;
	mlib_d64 accd, accum;
	mlib_s32 *pacc;
	mlib_f32 ones = vis_to_float(0x00010001);

/* temporaries used in macro */
	mlib_d64 dsign;

/* temporaries used in macro */
	mlib_d64 dsrc1h, dsrc1l;

/* temporaries used in macro */
	mlib_d64 dsrc2h, dsrc2l;

/* temporaries used in macro */
	mlib_d64 diffh, diffl;

/* temporaries used in macro */
	mlib_d64 dabsh, dabsl;
	mlib_s32 accs = 0, item, sign;
	mlib_s32 off;
	mlib_s32 i, nd4, nm4;

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;

	pdy = (mlib_d64 *)((mlib_addr)py & (~7));
	off = (mlib_addr)pdy - (mlib_addr)py;

	if (off != 0) {
		off += 8;
		off >>= 1;
		for (i = 0; i < off; i++) {
			item = (mlib_s32)(*px - *py);
			sign = item >> 31;
			accs += (item ^ sign) - sign;
			px++;
			py++;
		}
	}

	nd4 = (n - off) >> 3;
	nm4 = (n - off) & 0x7;

	pdx = (mlib_d64 *)vis_alignaddr(px, 0);
	pdy = (mlib_d64 *)py;
	accd = 0.0;
	off = ((mlib_addr)px & 0x7) >> 1;
#pragma pipeloop(0)
	for (i = 0; i < nd4; i++) {
		dx0 = pdx[0];
		dx1 = pdx[1];
		dx = vis_faligndata(dx0, dx1);
		dy = *pdy;
		MLIB_V_VECTORSUMABSDIFF_S16(dx, dy, accd);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[4 + off] -
			((mlib_s16 *)pdy)[4]);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[5 + off] -
			((mlib_s16 *)pdy)[5]);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[6 + off] -
			((mlib_s16 *)pdy)[6]);
		accs += (mlib_s32)ABS_VALUE(((mlib_s16 *)pdx)[7 + off] -
			((mlib_s16 *)pdy)[7]);
		pdx += 2;
		pdy += 2;
	}

	ones = vis_fpadd32s(vis_read_hi(accd), vis_read_lo(accd));
	pacc = (mlib_s32 *)&ones;
	accum = (mlib_d64)pacc[0];

	if (nm4 != 0) {
		px += nd4 * 8;
		py = (mlib_s16 *)pdy;
		for (i = 0; i < nm4; i++) {
			item = (mlib_s32)(*px - *py);
			sign = item >> 31;
			accs += (item ^ sign) - sign;
			px++;
			py++;
		}
	}

	z[0] = accum + (mlib_d64)accs;
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px, *py;
	mlib_u32 acc = 0;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;

	if (n < 4) {
		for (i = 0; i < n; i++) {
			acc += (mlib_s32)ABS_VALUE(px[i] - py[i]);
		}

		z[0] = (mlib_d64)acc;
		return (MLIB_SUCCESS);
	}

	if ((((mlib_addr)px | (mlib_addr)py) & 0x7) == 0) {

		mlib_v_VectorSumAbsDiff_S16_Sat_A8(z, x, y, n);
	} else {

		mlib_v_VectorSumAbsDiff_S16_Sat_NA(z, x, y, n);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
