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

#pragma ident	"@(#)mlib_v_VideoDownSample420.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample420 - 420 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample420(mlib_u8       *dst,
 *                                          const mlib_u8 *src0,
 *                                          const mlib_u8 *src1,
 *                                          mlib_s32      n);
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src1      Pointer to source lower row, 8-byte aligned
 *      n         Length of the src0, src1 arrays
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample420:
 *        dst[i] = (src0[2*i] + src0[2*i+1] +
 *                  src1[2*i] + src1[2*i+1] + (1 << (i&1))) / 4, 0 <= i < n/2
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample420 = __mlib_VideoDownSample420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample420) mlib_VideoDownSample420
	__attribute__((weak, alias("__mlib_VideoDownSample420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample420(
	mlib_u8 *dst,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	mlib_s32 n)
{
	mlib_d64 *sp0 = (mlib_d64 *)src0;
	mlib_d64 *sp1 = (mlib_d64 *)src1;
	mlib_d64 *pd = (mlib_d64 *)dst;
	mlib_d64 d0, d1;
	mlib_d64 tmp1, tmp3, data0, data1;
	mlib_d64 acc0_hi, acc0_lo, acc1_hi, acc1_lo;
	mlib_d64 round = vis_to_double_dup(0x10002);
	mlib_f32 fone = vis_to_float(0x1000000);
	mlib_s32 i, edge;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(5 << 3);
	vis_write_bmask(0x02461357, 0);

#pragma pipeloop(0)
	for (i = 0; i <= n - 16; i += 16) {
		d0 = (*sp0++);
		d1 = (*sp1++);
		tmp1 = vis_bshuffle(d0, d0);
		tmp3 = vis_bshuffle(d1, d1);

		acc0_hi = vis_fmul8x16au(vis_read_hi(tmp1), fone);
		acc0_lo = vis_fmul8x16au(vis_read_lo(tmp1), fone);
		acc1_hi = vis_fmul8x16au(vis_read_hi(tmp3), fone);
		acc1_lo = vis_fmul8x16au(vis_read_lo(tmp3), fone);

		acc0_hi = vis_fpadd16(acc0_hi, acc0_lo);
		acc1_hi = vis_fpadd16(acc1_hi, acc1_lo);
		acc0_hi = vis_fpadd16(acc0_hi, acc1_hi);
		data0 = vis_fpadd16(acc0_hi, round);

		d0 = (*sp0++);
		d1 = (*sp1++);
		tmp1 = vis_bshuffle(d0, d0);
		tmp3 = vis_bshuffle(d1, d1);

		acc0_hi = vis_fmul8x16au(vis_read_hi(tmp1), fone);
		acc0_lo = vis_fmul8x16au(vis_read_lo(tmp1), fone);
		acc1_hi = vis_fmul8x16au(vis_read_hi(tmp3), fone);
		acc1_lo = vis_fmul8x16au(vis_read_lo(tmp3), fone);

		acc0_hi = vis_fpadd16(acc0_hi, acc0_lo);
		acc1_hi = vis_fpadd16(acc1_hi, acc1_lo);
		acc0_hi = vis_fpadd16(acc0_hi, acc1_hi);
		data1 = vis_fpadd16(acc0_hi, round);

		(*pd++) = vis_fpack16_pair(data0, data1);
	}

	if (i < n) {
		d0 = (*sp0++);
		d1 = (*sp1++);
		tmp1 = vis_bshuffle(d0, d0);
		tmp3 = vis_bshuffle(d1, d1);

		acc0_hi = vis_fmul8x16au(vis_read_hi(tmp1), fone);
		acc0_lo = vis_fmul8x16au(vis_read_lo(tmp1), fone);
		acc1_hi = vis_fmul8x16au(vis_read_hi(tmp3), fone);
		acc1_lo = vis_fmul8x16au(vis_read_lo(tmp3), fone);

		acc0_hi = vis_fpadd16(acc0_hi, acc0_lo);
		acc1_hi = vis_fpadd16(acc1_hi, acc1_lo);
		acc0_hi = vis_fpadd16(acc0_hi, acc1_hi);
		data0 = vis_fpadd16(acc0_hi, round);

		d0 = vis_ld_d64_nf(sp0);
		d1 = vis_ld_d64_nf(sp1);
		tmp1 = vis_bshuffle(d0, d0);
		tmp3 = vis_bshuffle(d1, d1);

		acc0_hi = vis_fmul8x16au(vis_read_hi(tmp1), fone);
		acc0_lo = vis_fmul8x16au(vis_read_lo(tmp1), fone);
		acc1_hi = vis_fmul8x16au(vis_read_hi(tmp3), fone);
		acc1_lo = vis_fmul8x16au(vis_read_lo(tmp3), fone);

		acc0_hi = vis_fpadd16(acc0_hi, acc0_lo);
		acc1_hi = vis_fpadd16(acc1_hi, acc1_lo);
		acc0_hi = vis_fpadd16(acc0_hi, acc1_hi);
		data1 = vis_fpadd16(acc0_hi, round);
		edge = vis_edge8((void *)0, (void *)((n / 2 - 1) & 7));
		vis_pst_8(vis_fpack16_pair(data0, data1), pd, edge);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
