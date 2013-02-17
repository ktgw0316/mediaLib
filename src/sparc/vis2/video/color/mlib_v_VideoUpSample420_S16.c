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

#pragma ident	"@(#)mlib_v_VideoUpSample420_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample420_Nearest_S16 - 420 upsampling with nearest filter
 *      mlib_VideoUpSample420_S16         - 420 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample420_Nearest_S16(mlib_s16       *dst0,
 *                                                    const mlib_s16 *dst1,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VideoUpSample420_S16(mlib_s16       *dst0,
 *                                            const mlib_s16 *dst1,
 *                                            const mlib_s16 *src0,
 *                                            const mlib_s16 *src1,
 *                                            const mlib_s16 *src2,
 *                                            mlib_s32       n);
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
 *      mlib_VideoUpSample420_Nearest_S16:
 *        dst0[i]  = src[i/2],   0 <= i < 2*n
 *        dst1[i]  = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample420_S16:
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

#pragma weak mlib_VideoUpSample420_Nearest_S16 = \
	__mlib_VideoUpSample420_Nearest_S16
#pragma weak mlib_VideoUpSample420_S16 = __mlib_VideoUpSample420_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample420_Nearest_S16)
	mlib_VideoUpSample420_Nearest_S16
	__attribute__((weak, alias("__mlib_VideoUpSample420_Nearest_S16")));
__typeof__(__mlib_VideoUpSample420_S16) mlib_VideoUpSample420_S16
	__attribute__((weak, alias("__mlib_VideoUpSample420_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420_Nearest_S16(
	mlib_s16 *dst0,
	mlib_s16 *dst1,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 *dp0 = (mlib_d64 *)dst0;
	mlib_d64 *dp1 = (mlib_d64 *)dst1;
	mlib_s16 *dend = dst0 + 2 * n - 1;
	mlib_d64 sa, dr, da;
	mlib_s32 emask, i;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_alignaddr((void *)0, 4);
	vis_write_bmask(0x01012323, 0);

#pragma pipeloop(0)
	for (i = 0; i <= (n - 4); i += 4) {
		sa = sp[0];

		dp0[0] = dp1[0] = vis_bshuffle(sa, sa);
		dr = vis_faligndata(sa, sa);
		dp0[1] = dp1[1] = vis_bshuffle(dr, dr);

		sp++;
		dp0 += 2;
		dp1 += 2;
	}

	if ((mlib_s16 *)dp0 <= dend) {
		sa = sp[0];
		dr = vis_bshuffle(sa, sa);
		da = vis_faligndata(sa, sa);
		emask = vis_edge16(dp0, dend);
		vis_pst_16(dr, dp0, emask);
		vis_pst_16(dr, dp1, emask);
		dp0++;
		dp1++;

		if ((mlib_s16 *)dp0 <= dend) {
			dr = vis_bshuffle(da, da);
			emask = vis_edge16(dp0, dend);
			vis_pst_16(dr, dp0, emask);
			vis_pst_16(dr, dp1, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420_S16(
	mlib_s16 *dst0,
	mlib_s16 *dst1,
	const mlib_s16 *src0,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_s32 n)
{
	mlib_s16 *dend = dst0 + 2 * n - 1;
	mlib_d64 *dp0 = (mlib_d64 *)dst0;
	mlib_d64 *dp1 = (mlib_d64 *)dst1;
	mlib_d64 *sp0 = (mlib_d64 *)src0;
	mlib_d64 *sp1 = (mlib_d64 *)src1;
	mlib_d64 *sp2 = (mlib_d64 *)src2;
	mlib_d64 d00, d01, d02, d03;
	mlib_d64 d10, d11, d12, d13;
	mlib_d64 d20, d21, d22, d23;
	mlib_d64 ac00, ac01, ac02, ac03, ac04, ac05, ac06, ac07;
	mlib_d64 ac10, ac11, ac12, ac13, ac14, ac15, ac16, ac17;
	mlib_d64 ac20, ac21, ac22, ac23, ac24, ac25, ac26, ac27;
	mlib_f32 f13 = vis_to_float(0x10003);
	mlib_f32 f31 = vis_to_float(0x30001);
	mlib_f32 f39 = vis_to_float(0x30009);
	mlib_f32 f93 = vis_to_float(0x90003);
	mlib_d64 d87 = vis_to_double(8, 7);
	mlib_s32 i, emask;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((12 << 3) + 2);

	d01 = sp0[0];
	d11 = sp1[0];
	d21 = sp2[0];
	sp0++;
	sp1++;
	sp2++;
	d00 = vis_faligndata(d00, d01);
	d10 = vis_faligndata(d10, d11);
	d20 = vis_faligndata(d20, d21);

#pragma pipeloop(0)
	for (i = 0; i <= n - 4; i += 4) {
		d03 = vis_ld_d64_nf(sp0);
		d13 = vis_ld_d64_nf(sp1);
		d23 = vis_ld_d64_nf(sp2);
		sp0++;
		sp1++;
		sp2++;
		d02 = vis_faligndata(d01, d03);
		d12 = vis_faligndata(d11, d13);
		d22 = vis_faligndata(d21, d23);

		ac10 = vis_fmuld8ulx16(f39, vis_read_lo(d10));
		ac12 = vis_fmuld8ulx16(f39, vis_read_hi(d11));
		ac11 = vis_fmuld8ulx16(f93, vis_read_hi(d11));
		ac13 = vis_fmuld8ulx16(f93, vis_read_hi(d12));
		ac10 = vis_fpadd32(ac10, d87);
		ac12 = vis_fpadd32(ac12, d87);
		ac10 = vis_fpadd32(ac10, ac11);
		ac12 = vis_fpadd32(ac12, ac13);

		ac14 = vis_fmuld8ulx16(f39, vis_read_hi(d12));
		ac16 = vis_fmuld8ulx16(f39, vis_read_lo(d11));
		ac15 = vis_fmuld8ulx16(f93, vis_read_lo(d11));
		ac17 = vis_fmuld8ulx16(f93, vis_read_lo(d12));
		ac14 = vis_fpadd32(ac14, d87);
		ac16 = vis_fpadd32(ac16, d87);
		ac14 = vis_fpadd32(ac14, ac15);
		ac16 = vis_fpadd32(ac16, ac17);

		ac00 = vis_fmuld8ulx16(f13, vis_read_lo(d00));
		ac01 = vis_fmuld8ulx16(f31, vis_read_hi(d01));
		ac02 = vis_fmuld8ulx16(f13, vis_read_hi(d01));
		ac03 = vis_fmuld8ulx16(f31, vis_read_hi(d02));
		ac00 = vis_fpadd32(ac00, ac01);
		ac02 = vis_fpadd32(ac02, ac03);
		ac00 = vis_fpadd32(ac10, ac00);
		ac02 = vis_fpadd32(ac12, ac02);

		ac04 = vis_fmuld8ulx16(f13, vis_read_hi(d02));
		ac05 = vis_fmuld8ulx16(f31, vis_read_lo(d01));
		ac06 = vis_fmuld8ulx16(f13, vis_read_lo(d01));
		ac07 = vis_fmuld8ulx16(f31, vis_read_lo(d02));
		ac04 = vis_fpadd32(ac04, ac05);
		ac06 = vis_fpadd32(ac06, ac07);
		ac04 = vis_fpadd32(ac14, ac04);
		ac06 = vis_fpadd32(ac16, ac06);

		ac20 = vis_fmuld8ulx16(f13, vis_read_lo(d20));
		ac21 = vis_fmuld8ulx16(f31, vis_read_hi(d21));
		ac22 = vis_fmuld8ulx16(f13, vis_read_hi(d21));
		ac23 = vis_fmuld8ulx16(f31, vis_read_hi(d22));
		ac20 = vis_fpadd32(ac20, ac21);
		ac22 = vis_fpadd32(ac22, ac23);
		ac20 = vis_fpadd32(ac10, ac20);
		ac22 = vis_fpadd32(ac12, ac22);

		ac24 = vis_fmuld8ulx16(f13, vis_read_hi(d22));
		ac25 = vis_fmuld8ulx16(f31, vis_read_lo(d21));
		ac26 = vis_fmuld8ulx16(f13, vis_read_lo(d21));
		ac27 = vis_fmuld8ulx16(f31, vis_read_lo(d22));
		ac24 = vis_fpadd32(ac24, ac25);
		ac26 = vis_fpadd32(ac26, ac27);
		ac24 = vis_fpadd32(ac14, ac24);
		ac26 = vis_fpadd32(ac16, ac26);

		dp0[0] = vis_fpackfix_pair(ac00, ac02);
		dp0[1] = vis_fpackfix_pair(ac04, ac06);
		dp1[0] = vis_fpackfix_pair(ac20, ac22);
		dp1[1] = vis_fpackfix_pair(ac24, ac26);

		dp0 += 2;
		dp1 += 2;
		d00 = d02;
		d01 = d03;
		d10 = d12;
		d11 = d13;
		d20 = d22;
		d21 = d23;
	}

	if ((mlib_s16 *)dp0 <= dend) {
		d02 = vis_faligndata(d01, d03);
		d12 = vis_faligndata(d11, d13);
		d22 = vis_faligndata(d21, d23);

		ac10 = vis_fmuld8ulx16(f39, vis_read_lo(d10));
		ac12 = vis_fmuld8ulx16(f39, vis_read_hi(d11));
		ac11 = vis_fmuld8ulx16(f93, vis_read_hi(d11));
		ac13 = vis_fmuld8ulx16(f93, vis_read_hi(d12));
		ac10 = vis_fpadd32(ac10, d87);
		ac12 = vis_fpadd32(ac12, d87);
		ac10 = vis_fpadd32(ac10, ac11);
		ac12 = vis_fpadd32(ac12, ac13);

		ac00 = vis_fmuld8ulx16(f13, vis_read_lo(d00));
		ac01 = vis_fmuld8ulx16(f31, vis_read_hi(d01));
		ac02 = vis_fmuld8ulx16(f13, vis_read_hi(d01));
		ac03 = vis_fmuld8ulx16(f31, vis_read_hi(d02));
		ac00 = vis_fpadd32(ac00, ac01);
		ac02 = vis_fpadd32(ac02, ac03);
		ac00 = vis_fpadd32(ac10, ac00);
		ac02 = vis_fpadd32(ac12, ac02);

		ac20 = vis_fmuld8ulx16(f13, vis_read_lo(d20));
		ac21 = vis_fmuld8ulx16(f31, vis_read_hi(d21));
		ac22 = vis_fmuld8ulx16(f13, vis_read_hi(d21));
		ac23 = vis_fmuld8ulx16(f31, vis_read_hi(d22));
		ac20 = vis_fpadd32(ac20, ac21);
		ac22 = vis_fpadd32(ac22, ac23);
		ac20 = vis_fpadd32(ac10, ac20);
		ac22 = vis_fpadd32(ac12, ac22);

		ac00 = vis_fpackfix_pair(ac00, ac02);
		ac20 = vis_fpackfix_pair(ac20, ac22);
		emask = vis_edge16(dp0, dend);
		vis_pst_16(ac00, dp0, emask);
		vis_pst_16(ac20, dp1, emask);
		dp0++;
		dp1++;

		if ((mlib_s16 *)dp0 <= dend) {
			ac14 = vis_fmuld8ulx16(f39, vis_read_hi(d12));
			ac16 = vis_fmuld8ulx16(f39, vis_read_lo(d11));
			ac15 = vis_fmuld8ulx16(f93, vis_read_lo(d11));
			ac17 = vis_fmuld8ulx16(f93, vis_read_lo(d12));
			ac14 = vis_fpadd32(ac14, d87);
			ac16 = vis_fpadd32(ac16, d87);
			ac14 = vis_fpadd32(ac14, ac15);
			ac16 = vis_fpadd32(ac16, ac17);

			ac04 = vis_fmuld8ulx16(f13, vis_read_hi(d02));
			ac05 = vis_fmuld8ulx16(f31, vis_read_lo(d01));
			ac06 = vis_fmuld8ulx16(f13, vis_read_lo(d01));
			ac07 = vis_fmuld8ulx16(f31, vis_read_lo(d02));
			ac04 = vis_fpadd32(ac04, ac05);
			ac06 = vis_fpadd32(ac06, ac07);
			ac04 = vis_fpadd32(ac14, ac04);
			ac06 = vis_fpadd32(ac16, ac06);

			ac24 = vis_fmuld8ulx16(f13, vis_read_hi(d22));
			ac25 = vis_fmuld8ulx16(f31, vis_read_lo(d21));
			ac26 = vis_fmuld8ulx16(f13, vis_read_lo(d21));
			ac27 = vis_fmuld8ulx16(f31, vis_read_lo(d22));
			ac24 = vis_fpadd32(ac24, ac25);
			ac26 = vis_fpadd32(ac26, ac27);
			ac24 = vis_fpadd32(ac14, ac24);
			ac26 = vis_fpadd32(ac16, ac26);

			ac04 = vis_fpackfix_pair(ac04, ac06);
			ac24 = vis_fpackfix_pair(ac24, ac26);
			emask = vis_edge16(dp0, dend);
			vis_pst_16(ac04, dp0, emask);
			vis_pst_16(ac24, dp1, emask);
		}
	}

	dst0[0] = (4 * (3 * src1[0] + src0[0]) + 8) >> 4;
	dst1[0] = (4 * (3 * src1[0] + src2[0]) + 8) >> 4;
	dst0[2 * n - 1] = (4 * (3 * src1[n - 1] + src0[n - 1]) + 7) >> 4;
	dst1[2 * n - 1] = (4 * (3 * src1[n - 1] + src2[n - 1]) + 7) >> 4;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
