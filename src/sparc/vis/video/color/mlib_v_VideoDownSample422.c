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

#pragma ident	"@(#)mlib_v_VideoDownSample422.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample422 - 422 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample422(mlib_u8       *dst,
 *                                          const mlib_u8 *src,
 *                                          mlib_s32      n);
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample422:
 *        dst[i] = (src[2*i] + src[2*i+1] + (i&1)) / 2, 0 <= i < n/2
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample422 = __mlib_VideoDownSample422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample422) mlib_VideoDownSample422
	__attribute__((weak, alias("__mlib_VideoDownSample422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample422(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_d64 *sp0 = (mlib_d64 *)src;
	mlib_f32 *pd = (mlib_f32 *)dst;
	mlib_d64 d0;
	mlib_d64 tmp0, tmp1, data;
	mlib_d64 acc0_hi, acc0_lo;
	mlib_d64 round = vis_to_double_dup(0x1);
	mlib_f32 fone = vis_to_float(0x1000000);
	mlib_s32 i, bias = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(6 << 3);

#pragma pipeloop(0)
	for (i = 0; i <= n - 8; i += 8) {
		d0 = (*sp0++);
		tmp0 = vis_fpmerge(vis_read_hi(d0), vis_read_lo(d0));
		tmp1 = vis_fpmerge(vis_read_hi(tmp0), vis_read_lo(tmp0));

		acc0_hi = vis_fmul8x16au(vis_read_hi(tmp1), fone);
		acc0_lo = vis_fmul8x16au(vis_read_lo(tmp1), fone);

		acc0_hi = vis_fpadd16(acc0_hi, acc0_lo);
		data = vis_fpadd16(acc0_hi, round);

		(*pd++) = vis_fpack16(data);
	}

	dst = (mlib_u8 *)pd;

	for (; i < n; i += 2) {
		(*dst++) = (src[i] + src[i + 1] + bias) >> 1;
/* 1=>2, 2=>1 */
		bias ^= 1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
