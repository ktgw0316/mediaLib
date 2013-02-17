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

#pragma ident	"@(#)mlib_v_VideoUpSample420.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample420_Nearest - 420 upsampling with nearest filter
 *      mlib_VideoUpSample420         - 420 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample420(mlib_u8       *dst0,
 *                                        mlib_u8       *dst1,
 *                                        const mlib_u8 *src0,
 *                                        const mlib_u8 *src1,
 *                                        const mlib_u8 *src2,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VideoUpSample420_Nearest(mlib_u8       *dst0,
 *                                                mlib_u8       *dst1,
 *                                                const mlib_u8 *src,
 *                                                mlib_s32      n);
 *
 * ARGUMENTS
 *      dst0      Pointer to destination upper row, 8-byte aligned
 *      dst1      Pointer to destination lower row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src1      Pointer to source middle row, 8-byte aligned
 *      src2      Pointer to source lower row, 8-byte aligned
 *      n         Length of the source arrays
 *                Note: length of dst0 and dst1 arrays must be 2*n
 *                      For mlib_VideoUpSample420: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample420_Nearest:
 *        dst0[i]  = src[i/2],   0 <= i < 2*n
 *        dst1[i]  = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample420:
 *        dst0[0]     = (4*(3*src1[0]   + src0[0]) + 8) /16
 *        dst0[1]     = (3*(3*src1[0]   + src0[0]) +
 *                         (3*src1[1]   + src0[1]) + 7) / 16
 *        dst0[2*i]   = (3*(3*src1[i]   + src0[i]) +
 *                         (3*src1[i-1] + src0[i-1]) + 8) / 16,  0 < i < n-1
 *        dst0[2*i+1] = (3*(3*src1[i]   + src0[i]) +
 *                         (3*src1[i+1] + src0[i+1]) + 7) / 16,  0 < i < n-1
 *        dst0[2*n-2] = (3*(3*src1[n-1] + src0[n-1]) +
 *                         (3*src1[n-2] + src0[n-2]) + 8) / 16
 *        dst0[2*n-1] = (4*(3*src1[n-1] + src0[n-1]) + 7) / 16
 *        dst1[0]     = (4*(3*src1[0]   + src2[0]) + 8) /16
 *        dst1[1]     = (3*(3*src1[0]   + src2[0]) +
 *                         (3*src1[1]   + src2[1]) + 7) / 16
 *        dst1[2*i]   = (3*(3*src1[i]   + src2[i]) +
 *                         (3*src1[i-1] + src2[i-1]) + 8) / 16,  0 < i < n-1
 *        dst1[2*i+1] = (3*(3*src1[i]   + src2[i]) +
 *                         (3*src1[i+1] + src2[i+1]) + 7) / 16,  0 < i < n-1
 *        dst1[2*n-2] = (3*(3*src1[n-1] + src2[n-1]) +
 *                         (3*src1[n-2] + src2[n-2]) + 8) / 16
 *        dst1[2*n-1] = (4*(3*src1[n-1] + src2[n-1]) + 7) / 16
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoUpSample420 = __mlib_VideoUpSample420
#pragma weak mlib_VideoUpSample420_Nearest = __mlib_VideoUpSample420_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample420) mlib_VideoUpSample420
	__attribute__((weak, alias("__mlib_VideoUpSample420")));
__typeof__(__mlib_VideoUpSample420_Nearest) mlib_VideoUpSample420_Nearest
	__attribute__((weak, alias("__mlib_VideoUpSample420_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420_Nearest(
	mlib_u8 *dst0,
	mlib_u8 *dst1,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 *dp0 = (mlib_d64 *)dst0;
	mlib_d64 *dp1 = (mlib_d64 *)dst1;
	mlib_u8 *dend0 = dst0 + 2 * n - 1;
	mlib_d64 sa, da;
	mlib_s32 emask, i;

	if (n <= 0)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
		sa = *sp;
		*dp0 = *dp1 = vis_fpmerge(vis_read_hi(sa), vis_read_hi(sa));
		*(dp0 + 1) = *(dp1 + 1) =
			vis_fpmerge(vis_read_lo(sa), vis_read_lo(sa));
		sp++;
		dp0 += 2;
		dp1 += 2;
	}

	if (i < n) {
		sa = vis_ld_d64_nf(sp);
		da = vis_fpmerge(vis_read_hi(sa), vis_read_hi(sa));
		emask = vis_edge8(dp0, dend0);
		vis_pst_8(da, dp0, emask);
		vis_pst_8(da, dp1, emask);
		i += 4;
		dp0++;
		dp1++;

		if (i < n) {
			da = vis_fpmerge(vis_read_lo(sa), vis_read_lo(sa));
			emask = vis_edge8(dp0, dend0);
			vis_pst_8(da, dp0, emask);
			vis_pst_8(da, dp1, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420(
	mlib_u8 *dst0,
	mlib_u8 *dst1,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	const mlib_u8 *src2,
	mlib_s32 n)
{
	mlib_u8 *dend0 = dst0 + 2 * n - 1;
	mlib_d64 *dp0 = (mlib_d64 *)dst0;
	mlib_d64 *dp1 = (mlib_d64 *)dst1;
	mlib_d64 *sp0 = (mlib_d64 *)src0;
	mlib_d64 *sp1 = (mlib_d64 *)src1;
	mlib_d64 *sp2 = (mlib_d64 *)src2;
	mlib_d64 d00, d01, d10, d11, d20, d21;
	mlib_d64 thiscolsum0_hi, thiscolsum0_lo, lastcolsum0_hi, lastcolsum0_lo;
	mlib_d64 shiftcolsum0_hi, shiftcolsum0_lo;
	mlib_d64 thiscolsum1_hi, thiscolsum1_lo, lastcolsum1_hi, lastcolsum1_lo;
	mlib_d64 shiftcolsum1_hi, shiftcolsum1_lo;
	mlib_d64 acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7;
	mlib_d64 ac0, ac1, ac2, ac3, ac4, ac5, ac6, ac7;
	mlib_d64 data0, data1, data2, data3, tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x4000000);
	mlib_f32 fthree = vis_to_float(0xC000000);
	mlib_f32 fone1 = vis_to_float(0x40404040);
	mlib_f32 fthree1 = vis_to_float(0xC0C0C0C0);
	mlib_d64 dseven = vis_to_double_dup(0x70007);
	mlib_d64 deight = vis_to_double_dup(0x80008);
	mlib_s32 i, emask;
	mlib_d64 st00 = 0, st01, st02, st10 = 0, st11, st12;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((3 << 3) + 2);
	vis_write_bmask(0x789abcde, 0);

	d00 = vis_ld_d64_nf(sp0);
	d10 = vis_ld_d64_nf(sp1);
	d20 = vis_ld_d64_nf(sp2);
	sp0++;
	sp1++;
	sp2++;
	lastcolsum0_hi = vis_fmul8x16au(vis_read_hi(d00), fone);
	lastcolsum0_lo = vis_fmul8x16au(vis_read_lo(d00), fone);
	lastcolsum1_hi = vis_fmul8x16au(vis_read_hi(d20), fone);
	lastcolsum1_lo = vis_fmul8x16au(vis_read_lo(d20), fone);
	tmp0 = vis_fmul8x16au(vis_read_hi(d10), fthree);
	tmp1 = vis_fmul8x16au(vis_read_lo(d10), fthree);
	lastcolsum0_hi = vis_fpadd16(lastcolsum0_hi, tmp0);
	lastcolsum0_lo = vis_fpadd16(lastcolsum0_lo, tmp1);
	lastcolsum1_hi = vis_fpadd16(lastcolsum1_hi, tmp0);
	lastcolsum1_lo = vis_fpadd16(lastcolsum1_lo, tmp1);

#pragma pipeloop(0)
	for (i = 0; i < n - 8; i += 8) {
		d01 = *sp0;
		d11 = *sp1;
		d21 = *sp2;
		sp0++;
		sp1++;
		sp2++;

		thiscolsum0_hi = vis_fmul8x16au(vis_read_hi(d01), fone);
		thiscolsum0_lo = vis_fmul8x16au(vis_read_lo(d01), fone);
		thiscolsum1_hi = vis_fmul8x16au(vis_read_hi(d21), fone);
		thiscolsum1_lo = vis_fmul8x16au(vis_read_lo(d21), fone);

		tmp0 = vis_fmul8x16au(vis_read_hi(d11), fthree);
		tmp1 = vis_fmul8x16au(vis_read_lo(d11), fthree);

		thiscolsum0_hi = vis_fpadd16(thiscolsum0_hi, tmp0);
		thiscolsum0_lo = vis_fpadd16(thiscolsum0_lo, tmp1);
		thiscolsum1_hi = vis_fpadd16(thiscolsum1_hi, tmp0);
		thiscolsum1_lo = vis_fpadd16(thiscolsum1_lo, tmp1);

		acc0 = vis_fmul8x16(fone1, lastcolsum0_hi);
		acc1 = vis_fmul8x16(fone1, lastcolsum0_lo);
		acc2 = vis_fmul8x16(fthree1, lastcolsum0_hi);
		acc3 = vis_fmul8x16(fthree1, lastcolsum0_lo);
		acc4 = vis_fmul8x16(fone1, lastcolsum1_hi);
		acc5 = vis_fmul8x16(fone1, lastcolsum1_lo);
		acc6 = vis_fmul8x16(fthree1, lastcolsum1_hi);
		acc7 = vis_fmul8x16(fthree1, lastcolsum1_lo);

		shiftcolsum0_hi =
			vis_faligndata(lastcolsum0_hi, lastcolsum0_lo);
		shiftcolsum0_lo =
			vis_faligndata(lastcolsum0_lo, thiscolsum0_hi);
		shiftcolsum1_hi =
			vis_faligndata(lastcolsum1_hi, lastcolsum1_lo);
		shiftcolsum1_lo =
			vis_faligndata(lastcolsum1_lo, thiscolsum1_hi);

		acc0 = vis_fpadd16(acc0, deight);
		acc1 = vis_fpadd16(acc1, deight);
		acc2 = vis_fpadd16(acc2, dseven);
		acc3 = vis_fpadd16(acc3, dseven);
		acc4 = vis_fpadd16(acc4, deight);
		acc5 = vis_fpadd16(acc5, deight);
		acc6 = vis_fpadd16(acc6, dseven);
		acc7 = vis_fpadd16(acc7, dseven);

		ac0 = vis_fmul8x16(fthree1, shiftcolsum0_hi);
		ac1 = vis_fmul8x16(fthree1, shiftcolsum0_lo);
		ac2 = vis_fmul8x16(fone1, shiftcolsum0_hi);
		ac3 = vis_fmul8x16(fone1, shiftcolsum0_lo);
		ac4 = vis_fmul8x16(fthree1, shiftcolsum1_hi);
		ac5 = vis_fmul8x16(fthree1, shiftcolsum1_lo);
		ac6 = vis_fmul8x16(fone1, shiftcolsum1_hi);
		ac7 = vis_fmul8x16(fone1, shiftcolsum1_lo);

		acc0 = vis_fpadd16(acc0, ac0);
		acc1 = vis_fpadd16(acc1, ac1);
		acc2 = vis_fpadd16(acc2, ac2);
		acc3 = vis_fpadd16(acc3, ac3);
		acc4 = vis_fpadd16(acc4, ac4);
		acc5 = vis_fpadd16(acc5, ac5);
		acc6 = vis_fpadd16(acc6, ac6);
		acc7 = vis_fpadd16(acc7, ac7);

		data0 = vis_fpack16_pair(acc0, acc1);
		data1 = vis_fpack16_pair(acc2, acc3);
		data2 = vis_fpack16_pair(acc4, acc5);
		data3 = vis_fpack16_pair(acc6, acc7);

		st01 = vis_fpmerge(vis_read_hi(data1), vis_read_hi(data0));
		st02 = vis_fpmerge(vis_read_lo(data1), vis_read_lo(data0));
		st11 = vis_fpmerge(vis_read_hi(data3), vis_read_hi(data2));
		st12 = vis_fpmerge(vis_read_lo(data3), vis_read_lo(data2));

		dp0[0] = vis_bshuffle(st00, st01);
		dp0[1] = vis_bshuffle(st01, st02);
		dp1[0] = vis_bshuffle(st10, st11);
		dp1[1] = vis_bshuffle(st11, st12);

		dp0 += 2;
		dp1 += 2;
		st00 = st02;
		st10 = st12;
		lastcolsum0_hi = thiscolsum0_hi;
		lastcolsum0_lo = thiscolsum0_lo;
		lastcolsum1_hi = thiscolsum1_hi;
		lastcolsum1_lo = thiscolsum1_lo;
	}

	if (i < n) {
		acc0 = vis_fmul8x16(fone1, lastcolsum0_hi);
		acc1 = vis_fmul8x16(fone1, lastcolsum0_lo);
		acc2 = vis_fmul8x16(fthree1, lastcolsum0_hi);
		acc3 = vis_fmul8x16(fthree1, lastcolsum0_lo);
		acc4 = vis_fmul8x16(fone1, lastcolsum1_hi);
		acc5 = vis_fmul8x16(fone1, lastcolsum1_lo);
		acc6 = vis_fmul8x16(fthree1, lastcolsum1_hi);
		acc7 = vis_fmul8x16(fthree1, lastcolsum1_lo);

		shiftcolsum0_hi =
			vis_faligndata(lastcolsum0_hi, lastcolsum0_lo);
		shiftcolsum0_lo =
			vis_faligndata(lastcolsum0_lo, lastcolsum0_lo);
		shiftcolsum1_hi =
			vis_faligndata(lastcolsum1_hi, lastcolsum1_lo);
		shiftcolsum1_lo =
			vis_faligndata(lastcolsum1_lo, lastcolsum1_lo);

		acc0 = vis_fpadd16(acc0, deight);
		acc1 = vis_fpadd16(acc1, deight);
		acc2 = vis_fpadd16(acc2, dseven);
		acc3 = vis_fpadd16(acc3, dseven);
		acc4 = vis_fpadd16(acc4, deight);
		acc5 = vis_fpadd16(acc5, deight);
		acc6 = vis_fpadd16(acc6, dseven);
		acc7 = vis_fpadd16(acc7, dseven);

		ac0 = vis_fmul8x16(fthree1, shiftcolsum0_hi);
		ac1 = vis_fmul8x16(fthree1, shiftcolsum0_lo);
		ac2 = vis_fmul8x16(fone1, shiftcolsum0_hi);
		ac3 = vis_fmul8x16(fone1, shiftcolsum0_lo);
		ac4 = vis_fmul8x16(fthree1, shiftcolsum1_hi);
		ac5 = vis_fmul8x16(fthree1, shiftcolsum1_lo);
		ac6 = vis_fmul8x16(fone1, shiftcolsum1_hi);
		ac7 = vis_fmul8x16(fone1, shiftcolsum1_lo);

		acc0 = vis_fpadd16(acc0, ac0);
		acc1 = vis_fpadd16(acc1, ac1);
		acc2 = vis_fpadd16(acc2, ac2);
		acc3 = vis_fpadd16(acc3, ac3);
		acc4 = vis_fpadd16(acc4, ac4);
		acc5 = vis_fpadd16(acc5, ac5);
		acc6 = vis_fpadd16(acc6, ac6);
		acc7 = vis_fpadd16(acc7, ac7);

		data0 = vis_fpack16_pair(acc0, acc1);
		data1 = vis_fpack16_pair(acc2, acc3);
		data2 = vis_fpack16_pair(acc4, acc5);
		data3 = vis_fpack16_pair(acc6, acc7);

		st01 = vis_fpmerge(vis_read_hi(data1), vis_read_hi(data0));
		st11 = vis_fpmerge(vis_read_hi(data3), vis_read_hi(data2));
		acc0 = vis_bshuffle(st00, st01);
		acc1 = vis_bshuffle(st10, st11);

		emask = vis_edge8(dp0, dend0);
		vis_pst_8(acc0, dp0, emask);
		vis_pst_8(acc1, dp1, emask);
		i += 4;
		dp0++;
		dp1++;

		if (i < n) {
			st02 = vis_fpmerge(vis_read_lo(data1),
				vis_read_lo(data0));
			st12 = vis_fpmerge(vis_read_lo(data3),
				vis_read_lo(data2));
			acc0 = vis_bshuffle(st01, st02);
			acc1 = vis_bshuffle(st11, st12);

			emask = vis_edge8(dp0, dend0);
			vis_pst_8(acc0, dp0, emask);
			vis_pst_8(acc1, dp1, emask);
		}
	}

	dst0[0] = (4 * (3 * src1[0] + src0[0]) + 8) >> 4;
	dst1[0] = (4 * (3 * src1[0] + src2[0]) + 8) >> 4;
	dst0[2 * n - 1] = (4 * (3 * src1[n - 1] + src0[n - 1]) + 7) >> 4;
	dst1[2 * n - 1] = (4 * (3 * src1[n - 1] + src2[n - 1]) + 7) >> 4;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
