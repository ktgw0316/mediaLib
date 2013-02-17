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

#pragma ident	"@(#)mlib_v_VideoUpSample422_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample422_Nearest_S16 - 422 upsampling with nearest filter
 *      mlib_VideoUpSample422_S16         - 422 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample422_Nearest_S16(mlib_s16       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VideoUpSample422_S16(mlib_s16       *dst,
 *                                            const mlib_s16 *src,
 *                                            mlib_s32       n);
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: length of dst array must be 2*n
 *                      For mlib_VideoUpSample422_S16: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample422_Nearest_S16:
 *        dst[i]   = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample422_S16:
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

#pragma weak mlib_VideoUpSample422_Nearest_S16 = \
	__mlib_VideoUpSample422_Nearest_S16
#pragma weak mlib_VideoUpSample422_S16 = __mlib_VideoUpSample422_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample422_Nearest_S16)
	mlib_VideoUpSample422_Nearest_S16
	__attribute__((weak, alias("__mlib_VideoUpSample422_Nearest_S16")));
__typeof__(__mlib_VideoUpSample422_S16) mlib_VideoUpSample422_S16
	__attribute__((weak, alias("__mlib_VideoUpSample422_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_Nearest_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 *dp = (mlib_d64 *)dst;
	mlib_s16 *dend = dst + 2 * n - 1;
	mlib_d64 sa0, sa1, dr, da;
	mlib_s32 emask, i;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_alignaddr((void *)0, 4);
	vis_write_bmask(0x01012323, 0);

#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
		sa0 = sp[0];
		sa1 = sp[1];

		dp[0] = vis_bshuffle(sa0, sa0);
		dr = vis_faligndata(sa0, sa0);
		dp[1] = vis_bshuffle(dr, dr);
		dp[2] = vis_bshuffle(sa1, sa1);
		dr = vis_faligndata(sa1, sa1);
		dp[3] = vis_bshuffle(dr, dr);

		sp += 2;
		dp += 4;
	}

	if ((mlib_s16 *)dp <= dend) {
		sa0 = vis_ld_d64_nf(sp);
		da = vis_bshuffle(sa0, sa0);
		emask = vis_edge16(dp, dend);
		vis_pst_16(da, dp, emask);
		dp++;
		dr = vis_faligndata(sa0, sa0);

		if ((mlib_s16 *)dp <= dend) {
			dr = vis_bshuffle(dr, dr);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dr, dp, emask);
			dp++;

			if ((mlib_s16 *)dp <= dend) {
				sa1 = vis_ld_d64_nf(sp + 1);
				da = vis_bshuffle(sa1, sa1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(da, dp, emask);
				dp++;
				dr = vis_faligndata(sa1, sa1);

				if ((mlib_s16 *)dp <= dend) {
					dr = vis_bshuffle(dr, dr);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dr, dp, emask);
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_s16 *dend = dst + 2 * n - 1;
	mlib_d64 *dp = (mlib_d64 *)dst;
	mlib_d64 *sp = (mlib_d64 *)src;
	mlib_d64 d0, d1, d2, d22;
	mlib_d64 ac0, ac1, ac2, ac3, ac4, ac5, ac6, ac7;
	mlib_f32 f13 = vis_to_float(0x10003);
	mlib_f32 f31 = vis_to_float(0x30001);
	mlib_d64 d12 = vis_to_double(1, 2);
	mlib_s32 i, emask;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((14 << 3) + 2);

	d1 = vis_ld_d64_nf(sp);
	sp++;
	d0 = vis_faligndata(d0, d1);

#pragma pipeloop(0)
	for (i = 0; i <= (n - 4); i += 4) {
		d22 = vis_ld_d64_nf(sp);
		sp++;
		d2 = vis_faligndata(d1, d22);

		ac0 = vis_fmuld8ulx16(f13, vis_read_lo(d0));
		ac1 = vis_fmuld8ulx16(f31, vis_read_hi(d1));
		ac2 = vis_fmuld8ulx16(f13, vis_read_hi(d1));
		ac3 = vis_fmuld8ulx16(f31, vis_read_hi(d2));
		ac4 = vis_fmuld8ulx16(f13, vis_read_hi(d2));
		ac5 = vis_fmuld8ulx16(f31, vis_read_lo(d1));
		ac6 = vis_fmuld8ulx16(f13, vis_read_lo(d1));
		ac7 = vis_fmuld8ulx16(f31, vis_read_lo(d2));

		ac0 = vis_fpadd32(ac0, d12);
		ac2 = vis_fpadd32(ac2, d12);
		ac4 = vis_fpadd32(ac4, d12);
		ac6 = vis_fpadd32(ac6, d12);
		ac0 = vis_fpadd32(ac0, ac1);
		ac2 = vis_fpadd32(ac2, ac3);
		ac4 = vis_fpadd32(ac4, ac5);
		ac6 = vis_fpadd32(ac6, ac7);

		dp[0] = vis_fpackfix_pair(ac0, ac2);
		dp[1] = vis_fpackfix_pair(ac4, ac6);

		dp += 2;
		d0 = d2;
		d1 = d22;
	}

	if ((mlib_s16 *)dp <= dend) {
		d2 = vis_faligndata(d1, d22);

		ac0 = vis_fmuld8ulx16(f13, vis_read_lo(d0));
		ac1 = vis_fmuld8ulx16(f31, vis_read_hi(d1));
		ac2 = vis_fmuld8ulx16(f13, vis_read_hi(d1));
		ac3 = vis_fmuld8ulx16(f31, vis_read_hi(d2));
		ac0 = vis_fpadd32(ac0, d12);
		ac2 = vis_fpadd32(ac2, d12);
		ac0 = vis_fpadd32(ac0, ac1);
		ac2 = vis_fpadd32(ac2, ac3);

		ac0 = vis_fpackfix_pair(ac0, ac2);
		emask = vis_edge16(dp, dend);
		vis_pst_16(ac0, dp, emask);
		dp++;

		if ((mlib_s16 *)dp <= dend) {
			ac4 = vis_fmuld8ulx16(f13, vis_read_hi(d2));
			ac5 = vis_fmuld8ulx16(f31, vis_read_lo(d1));
			ac6 = vis_fmuld8ulx16(f13, vis_read_lo(d1));
			ac7 = vis_fmuld8ulx16(f31, vis_read_lo(d2));
			ac4 = vis_fpadd32(ac4, d12);
			ac6 = vis_fpadd32(ac6, d12);
			ac4 = vis_fpadd32(ac4, ac5);
			ac6 = vis_fpadd32(ac6, ac7);

			ac4 = vis_fpackfix_pair(ac4, ac6);
			emask = vis_edge16(dp, dend);
			vis_pst_16(ac4, dp, emask);
		}
	}

	dst[0] = src[0];
	dst[2 * n - 1] = src[n - 1];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
