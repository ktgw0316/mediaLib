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

#pragma ident	"@(#)mlib_v_VideoDownSample420_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample420_S16 - 420 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample420_S16(mlib_s16       *dst,
 *                                              const mlib_s16 *src0,
 *                                              const mlib_s16 *src1,
 *                                              mlib_s32       n);
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src2      Pointer to source lower row, 8-byte aligned
 *      n         Length of the src0, src1 arrays
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample420_S16:
 *        dst[i] = (src0[2*i] + src0[2*i+1] +
 *                  src1[2*i] + src1[2*i+1] + (1 << (i&1))) / 4, 0 <= i < n/2
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample420_S16 = __mlib_VideoDownSample420_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample420_S16) mlib_VideoDownSample420_S16
	__attribute__((weak, alias("__mlib_VideoDownSample420_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample420_S16(
	mlib_s16 *dst,
	const mlib_s16 *src0,
	const mlib_s16 *src1,
	mlib_s32 n)
{
	mlib_d64 *sp0 = (mlib_d64 *)src0;
	mlib_d64 *sp1 = (mlib_d64 *)src1;
	mlib_d64 *pd = (mlib_d64 *)dst;
	mlib_d64 ac0, ac1, ac2, ac3, d0, d1, dr0, dr1;
	mlib_d64 round = vis_to_double(1, 2);
	mlib_f32 f11 = vis_to_float(0x10001);
	mlib_s32 i, edge;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(14 << 3);

#pragma pipeloop(0)
	for (i = 0; i <= n - 8; i += 8) {
		d0 = (*sp0++);
		d1 = (*sp1++);
		ac0 = vis_fmuld8ulx16(f11, vis_read_hi(d0));
		ac1 = vis_fmuld8ulx16(f11, vis_read_lo(d0));
		ac2 = vis_fmuld8ulx16(f11, vis_read_hi(d1));
		ac3 = vis_fmuld8ulx16(f11, vis_read_lo(d1));
		ac0 = vis_fpadd32(ac0, ac2);
		ac1 = vis_fpadd32(ac1, ac3);
		dr0 = vis_freg_pair(vis_fpadd32s(vis_read_hi(ac0),
			vis_read_lo(ac0)), vis_fpadd32s(vis_read_hi(ac1),
			vis_read_lo(ac1)));
		dr0 = vis_fpadd32(dr0, round);
		d0 = (*sp0++);
		d1 = (*sp1++);
		ac0 = vis_fmuld8ulx16(f11, vis_read_hi(d0));
		ac1 = vis_fmuld8ulx16(f11, vis_read_lo(d0));
		ac2 = vis_fmuld8ulx16(f11, vis_read_hi(d1));
		ac3 = vis_fmuld8ulx16(f11, vis_read_lo(d1));
		ac0 = vis_fpadd32(ac0, ac2);
		ac1 = vis_fpadd32(ac1, ac3);
		dr1 = vis_freg_pair(vis_fpadd32s(vis_read_hi(ac0),
			vis_read_lo(ac0)), vis_fpadd32s(vis_read_hi(ac1),
			vis_read_lo(ac1)));
		dr1 = vis_fpadd32(dr1, round);
		(*pd++) = vis_fpackfix_pair(dr0, dr1);
	}

	if (i < n) {
		d0 = (*sp0++);
		d1 = (*sp1++);
		ac0 = vis_fmuld8ulx16(f11, vis_read_hi(d0));
		ac1 = vis_fmuld8ulx16(f11, vis_read_lo(d0));
		ac2 = vis_fmuld8ulx16(f11, vis_read_hi(d1));
		ac3 = vis_fmuld8ulx16(f11, vis_read_lo(d1));
		ac0 = vis_fpadd32(ac0, ac2);
		ac1 = vis_fpadd32(ac1, ac3);
		dr0 = vis_freg_pair(vis_fpadd32s(vis_read_hi(ac0),
			vis_read_lo(ac0)), vis_fpadd32s(vis_read_hi(ac1),
			vis_read_lo(ac1)));
		dr0 = vis_fpadd32(dr0, round);
		d0 = vis_ld_d64_nf(sp0);
		d1 = vis_ld_d64_nf(sp1);
		ac0 = vis_fmuld8ulx16(f11, vis_read_hi(d0));
		ac1 = vis_fmuld8ulx16(f11, vis_read_lo(d0));
		ac2 = vis_fmuld8ulx16(f11, vis_read_hi(d1));
		ac3 = vis_fmuld8ulx16(f11, vis_read_lo(d1));
		ac0 = vis_fpadd32(ac0, ac2);
		ac1 = vis_fpadd32(ac1, ac3);
		dr1 = vis_freg_pair(vis_fpadd32s(vis_read_hi(ac0),
			vis_read_lo(ac0)), vis_fpadd32s(vis_read_hi(ac1),
			vis_read_lo(ac1)));
		dr1 = vis_fpadd32(dr1, round);
		edge = vis_edge16(pd, (dst + (n / 2) - 1));
		vis_pst_16(vis_fpackfix_pair(dr0, dr1), pd, edge);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
