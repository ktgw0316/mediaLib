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

#pragma ident	"@(#)mlib_v_VideoJFIFYCC2RGB444_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB444_S16 - color conversion from JFIF YCbCr
 *                                          12-bit space to RGB 12-bit space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorJFIFYCC2RGB444_S16(mlib_s16       *rgb,
 *                                                    const mlib_s16 *y,
 *                                                    const mlib_s16 *cb,
 *                                                    const mlib_s16 *cr,
 *                                                    mlib_s32       n);
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y, cb, cr arrays
 *                Note: length of rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *        R  = Y                         + 1.40200 * (Cr - 2048)
 *        G  = Y - 0.34414 * (Cb - 2048) - 0.71414 * (Cr - 2048)
 *        B  = Y + 1.77200 * (Cb - 2048)
 *
 *        Y  = y[i], Cr = cr[i], Cb = cb[i]
 *        rgb[3*i] = R, rgb[3*i+1] = G, rgb[3*i+2] = B, 0 <= i < n
 *
 * RESTRICTION
 *
 *        YCbCr component values must be in [0, 4095] range.
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB444_S16 = \
	__mlib_VideoColorJFIFYCC2RGB444_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB444_S16)
	mlib_VideoColorJFIFYCC2RGB444_S16
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB444_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SCALE	(mlib_d64)0x100000
#define	SAT	(mlib_d64)0x7FF80000
#define	CLAMP_U12(src)	((FLOAT2INT_CLAMP(src) >> 20) + 0x800)

/* *********************************************************** */

#define	MUL16(SRC1, SRC2)                                               \
	(vis_fpadd16(vis_fmul8sux16(SRC1, SRC2), vis_fmul8ulx16(SRC1,   \
		SRC2)))

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2RGB444_S16(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n)
{
	mlib_d64 *dp = (mlib_d64 *)rgb;
	mlib_d64 *spy = (mlib_d64 *)y;
	mlib_d64 *spcb = (mlib_d64 *)cb;
	mlib_d64 *spcr = (mlib_d64 *)cr;
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x80008);
	mlib_d64 d2048 = vis_to_double_dup(0x8000800);

/* ((mlib_s32)((1-0.71414)*(1 << 16)+0.5)) */
	mlib_d64 dk5 = vis_to_double_dup(0x492e492e);

/* ((mlib_s32)(1.77200*(1 << 12)+0.5)) */
	mlib_d64 dk7 = vis_to_double_dup(0x1c5a1c5a);

/* ((mlib_s32)(0.40200*(1 << 8)+0.5)) */
	mlib_f32 fk2 = vis_to_float(0x67676767);

/* ((mlib_s32)(0.34414*(1 << 8)+0.5)) */
	mlib_f32 fk4 = vis_to_float(0x58585858);
	mlib_f32 f11 = vis_to_float(0x1000100);
	mlib_f32 scale = vis_to_float(0x10101010);
	mlib_d64 sdy, sdcb, sdcr, ddr, ddg, ddb, d0, d1, d2, da, db, dc;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(12 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (n >> 2); i++) {
		sdy = spy[i];
		sdcb = spcb[i];
		sdcr = spcr[i];
		sdy = vis_fpsub16(sdy, d2048);
		sdcb = vis_fpsub16(sdcb, d2048);
		sdcr = vis_fpsub16(sdcr, d2048);
/* sdcb *= 16 */
		sdcb = vis_fpackfix_pair(vis_fmuld8sux16(f11,
			vis_read_hi(sdcb)), vis_fmuld8sux16(f11,
			vis_read_lo(sdcb)));

		ddr = vis_fpadd16(sdy, vis_fmul8x16(fk2, sdcr));
		dp[3 * i] = vis_fpadd16(ddr, sdcr);

/* (1-0.71414) * (Cr - 2048) / 2 */
		ddg = MUL16(dk5, sdcr);
		ddg = vis_fpadd16(sdy, ddg);
		ddg = vis_fpsub16(ddg, sdcr);
		dp[3 * i + 1] =
			vis_fpsub16(ddg, vis_fmul8x16(scale, vis_fmul8x16(fk4,
			sdcb)));
		dp[3 * i + 2] = vis_fpadd16(sdy, MUL16(dk7, sdcb));
	}

	d0 = dp[0];
#pragma pipeloop(0)
	for (i = 0; i < (n >> 2); i++) {
		d1 = dp[3 * i + 1];
		d2 = dp[3 * i + 2];
		d0 = vis_fpmerge(vis_read_hi(d0), vis_read_lo(d0));
		d1 = vis_fpmerge(vis_read_hi(d1), vis_read_lo(d1));
		d2 = vis_fpmerge(vis_read_hi(d2), vis_read_lo(d2));
		da = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d1));
		db = vis_fpmerge(vis_read_hi(d2), vis_read_lo(d0));
		dc = vis_fpmerge(vis_read_lo(d1), vis_read_lo(d2));
		da = vis_fpmerge(vis_read_hi(da), vis_read_lo(da));
		db = vis_fpmerge(vis_read_hi(db), vis_read_lo(db));
		dc = vis_fpmerge(vis_read_hi(dc), vis_read_lo(dc));

		ddr = vis_fpackfix_pair(vis_fmuld8sux16(f11, vis_read_hi(da)),
			vis_fmuld8sux16(f11, vis_read_hi(db)));
		ddr = vis_fandnot(anti_rounding_mask, ddr);
/* result >>= 4 */
		ddr = vis_fmul8x16(scale, ddr);
		d0 = vis_ld_d64_nf(dp + 3 * i + 3);
		dp[3 * i] = vis_fpadd16(ddr, d2048);

		ddg = vis_fpackfix_pair(vis_fmuld8sux16(f11, vis_read_hi(dc)),
			vis_fmuld8sux16(f11, vis_read_lo(da)));
		ddg = vis_fandnot(anti_rounding_mask, ddg);
/* result >>= 4 */
		ddg = vis_fmul8x16(scale, ddg);
		dp[3 * i + 1] = vis_fpadd16(ddg, d2048);

		ddb = vis_fpackfix_pair(vis_fmuld8sux16(f11, vis_read_lo(db)),
			vis_fmuld8sux16(f11, vis_read_lo(dc)));
		ddb = vis_fandnot(anti_rounding_mask, ddb);
/* result >>= 4 */
		ddb = vis_fmul8x16(scale, ddb);
		dp[3 * i + 2] = vis_fpadd16(ddb, d2048);
	}

#pragma unroll(1)
	for (i = n & ~3; i < n; i++) {
		mlib_d64 fr, fg, fb, fy, fcb, fcr;

		fy = y[i] * SCALE - SAT;
		fcb = (cb[i] - 2048) << 20;
		fcr = (cr[i] - 2048) << 20;
		fr = fy + 1.40200 * fcr;
		fg = fy - 0.34414 * fcb - 0.71414 * fcr;
		fb = fy + 1.77200 * fcb;
		rgb[3 * i] = CLAMP_U12(fr);
		rgb[3 * i + 1] = CLAMP_U12(fg);
		rgb[3 * i + 2] = CLAMP_U12(fb);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
