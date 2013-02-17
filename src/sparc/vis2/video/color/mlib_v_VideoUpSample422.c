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

#pragma ident	"@(#)mlib_v_VideoUpSample422.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample422_Nearest - 422 upsampling with nearest filter
 *      mlib_VideoUpSample422         - 422 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample422(mlib_u8       *dst,
 *                                        const mlib_u8 *src,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VideoUpSample422_Nearest(mlib_u8       *dst,
 *                                                const mlib_u8 *src,
 *                                                mlib_s32      n);
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: length of dst array must be 2*n
 *                      For mlib_VideoUpSample422: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample422_Nearest:
 *        dst[i]   = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample422:
 *        dst[0]     = src[0]
 *        dst[1]     = (3*src[0] + src[1] + 2) / 4
 *        dst[2*i]   = (3*src[i] + src[i-1] + 1) / 4,  0 < i < n-1
 *        dst[2*i+1] = (3*src[i] + src[i+1] + 2) / 4,  0 < i < n-1
 *        dst[2*n-2] = (3*src[n-1] + src[n-2] + 1) / 4
 *        dst[2*n-1] = src[n-1]
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoUpSample422 = __mlib_VideoUpSample422
#pragma weak mlib_VideoUpSample422_Nearest = __mlib_VideoUpSample422_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample422) mlib_VideoUpSample422
	__attribute__((weak, alias("__mlib_VideoUpSample422")));
__typeof__(__mlib_VideoUpSample422_Nearest) mlib_VideoUpSample422_Nearest
	__attribute__((weak, alias("__mlib_VideoUpSample422_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_Nearest(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 *dp = (mlib_d64 *)dst;
	mlib_u8 *dend = dst + 2 * n - 1;
	mlib_d64 sa, da;
	mlib_s32 emask, i;

	if (n <= 0)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
		sa = *sp;
		*dp = vis_fpmerge(vis_read_hi(sa), vis_read_hi(sa));
		*(dp + 1) = vis_fpmerge(vis_read_lo(sa), vis_read_lo(sa));
		sp++;
		dp += 2;
	}

	if (i < n) {
		sa = *sp;
		da = vis_fpmerge(vis_read_hi(sa), vis_read_hi(sa));
		emask = vis_edge8(dp, dend);
		vis_pst_8(da, dp, emask);
		i += 4;
		dp++;

		if (i < n) {
			da = vis_fpmerge(vis_read_lo(sa), vis_read_lo(sa));
			emask = vis_edge8(dp, dend);
			vis_pst_8(da, dp, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_u8 *dend = dst + 2 * n - 1;
	mlib_d64 *dp = (mlib_d64 *)dst;
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 d0, d1, shift;
	mlib_d64 acc0, acc1, acc2, acc3;
	mlib_d64 ac0, ac1, ac2, ac3;
	mlib_d64 data0, data1;
	mlib_f32 fone = vis_to_float(0x1000000);
	mlib_f32 fthree = vis_to_float(0x3000000);
	mlib_d64 done = vis_to_double_dup(0x10001);
	mlib_d64 dtwo = vis_to_double_dup(0x20002);
	mlib_s32 i, emask;
	mlib_d64 st00 = 0, st01, st02;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((5 << 3) + 1);
	vis_write_bmask(0x789abcde, 0);

	d0 = *sp;
	sp++;

#pragma pipeloop(0)
	for (i = 0; i < n - 8; i += 8) {
		d1 = *sp;
		sp++;

		acc0 = vis_fmul8x16au(vis_read_hi(d0), fthree);
		acc1 = vis_fmul8x16au(vis_read_lo(d0), fthree);
		acc2 = vis_fmul8x16au(vis_read_hi(d0), fone);
		acc3 = vis_fmul8x16au(vis_read_lo(d0), fone);

		shift = vis_faligndata(d0, d1);

		ac0 = vis_fmul8x16au(vis_read_hi(shift), fone);
		ac1 = vis_fmul8x16au(vis_read_lo(shift), fone);
		ac2 = vis_fmul8x16au(vis_read_hi(shift), fthree);
		ac3 = vis_fmul8x16au(vis_read_lo(shift), fthree);

		acc0 = vis_fpadd16(acc0, dtwo);
		acc1 = vis_fpadd16(acc1, dtwo);
		acc2 = vis_fpadd16(acc2, done);
		acc3 = vis_fpadd16(acc3, done);

		acc0 = vis_fpadd16(acc0, ac0);
		acc1 = vis_fpadd16(acc1, ac1);
		acc2 = vis_fpadd16(acc2, ac2);
		acc3 = vis_fpadd16(acc3, ac3);

		data0 = vis_fpack16_pair(acc0, acc1);
		data1 = vis_fpack16_pair(acc2, acc3);

		st01 = vis_fpmerge(vis_read_hi(data0), vis_read_hi(data1));
		st02 = vis_fpmerge(vis_read_lo(data0), vis_read_lo(data1));
		dp[0] = vis_bshuffle(st00, st01);
		dp[1] = vis_bshuffle(st01, st02);

		dp += 2;
		d0 = d1;
		st00 = st02;
	}

	if (i < n) {

		acc0 = vis_fmul8x16au(vis_read_hi(d0), fthree);
		acc1 = vis_fmul8x16au(vis_read_lo(d0), fthree);
		acc2 = vis_fmul8x16au(vis_read_hi(d0), fone);
		acc3 = vis_fmul8x16au(vis_read_lo(d0), fone);

		shift = vis_faligndata(d0, d1);

		ac0 = vis_fmul8x16au(vis_read_hi(shift), fone);
		ac1 = vis_fmul8x16au(vis_read_lo(shift), fone);
		ac2 = vis_fmul8x16au(vis_read_hi(shift), fthree);
		ac3 = vis_fmul8x16au(vis_read_lo(shift), fthree);

		acc0 = vis_fpadd16(acc0, dtwo);
		acc1 = vis_fpadd16(acc1, dtwo);
		acc2 = vis_fpadd16(acc2, done);
		acc3 = vis_fpadd16(acc3, done);

		acc0 = vis_fpadd16(acc0, ac0);
		acc1 = vis_fpadd16(acc1, ac1);
		acc2 = vis_fpadd16(acc2, ac2);
		acc3 = vis_fpadd16(acc3, ac3);

		data0 = vis_fpack16_pair(acc0, acc1);
		data1 = vis_fpack16_pair(acc2, acc3);

		st01 = vis_fpmerge(vis_read_hi(data0), vis_read_hi(data1));
		acc0 = vis_bshuffle(st00, st01);

		emask = vis_edge8(dp, dend);
		vis_pst_8(acc0, dp, emask);
		i += 4;
		dp++;

		if (i < n) {
			st02 = vis_fpmerge(vis_read_lo(data0),
				vis_read_lo(data1));
			acc0 = vis_bshuffle(st01, st02);

			emask = vis_edge8(dp, dend);
			vis_pst_8(acc0, dp, emask);
		}
	}

	dst[0] = src[0];
	dst[2 * n - 1] = src[n - 1];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
