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

#pragma ident	"@(#)mlib_v_VideoColorJFIFYCC2ABGR444.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2ABGR444 - color conversion from JFIF YCrCb
 *                                       to ABGR space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorJFIFYCC2ABGR444(mlib_u8       *abgr,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *cb,
 *                                                 const mlib_u8 *cr,
 *                                                 mlib_s32      n);
 * ARGUMENTS
 *      abgr      Pointer to ABGR multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y, cb, cr arrays must be n
 *                Note: length of abgr array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCC2ABGR444
 *        R  = Y                        + 1.40200 * (Cr - 128)
 *        G  = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)
 *        B  = Y + 1.77200 * (Cb - 128)
 *
 *        Y = y[i], Cr = cr[i], Cb = cb[i]
 *        B = abgr[4*i+1], G = abgr[4*i+2], R = abgr[4*i+3]
 *        A = abgr[4*i] - should be 0xFF
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2ABGR444 = \
	__mlib_VideoColorJFIFYCC2ABGR444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2ABGR444) mlib_VideoColorJFIFYCC2ABGR444
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2ABGR444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_MERGE4_U8(sf0, sf1, sf2, sf3, dd0, dd1)                 \
	{                                                            \
	    mlib_d64 d02, d13;                                       \
	                                                             \
	    d02 = vis_fpmerge(sf0, sf2);                             \
	    d13 = vis_fpmerge(sf1, sf3);                             \
	    dd0 = vis_fpmerge(vis_read_hi(d02), vis_read_hi(d13));   \
	    dd1 = vis_fpmerge(vis_read_lo(d02), vis_read_lo(d13));   \
	}

/* *********************************************************** */

#define	K_0	0x2000
#define	K02	0x2cdd
#define	K11	0xf4fd
#define	K12	0xe926
#define	K21	0x38b4

/* *********************************************************** */

#define	OFF0	0xe9a1U
#define	OFF1	0x10ffU
#define	OFF2	0xe3b6U

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2ABGR444(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	mlib_f32 *sp0 = (mlib_f32 *)y;
	mlib_f32 *sp1 = (mlib_f32 *)cb;
	mlib_f32 *sp2 = (mlib_f32 *)cr;
	mlib_d64 *dp = (mlib_d64 *)abgr;
	mlib_f32 k_0 = vis_to_float(K_0);
	mlib_f32 k02 = vis_to_float(K02);
	mlib_f32 k11 = vis_to_float(K11);
	mlib_f32 k12 = vis_to_float(K12);
	mlib_f32 k21 = vis_to_float(K21);
	mlib_d64 c_0 = vis_to_double_dup((OFF0 << 16) | OFF0);
	mlib_d64 c_1 = vis_to_double_dup((OFF1 << 16) | OFF1);
	mlib_d64 c_2 = vis_to_double_dup((OFF2 << 16) | OFF2);
	mlib_f32 alp = vis_to_float(0xFFFFFFFF);
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(2 << 3);

#pragma pipeloop(0)
	for (i = 0; i < (n / 4); i++) {
		mlib_d64 s_0, s11, s21, s02, s12, ss0, ss1, ss2, dd0, dd1;
		mlib_f32 x0, x1, x2, df0, df1, df2;

		x0 = (*sp0++);
		x1 = (*sp1++);
		x2 = (*sp2++);

		s_0 = vis_fmul8x16al(x0, k_0);

		s11 = vis_fmul8x16al(x1, k11);
		s21 = vis_fmul8x16al(x1, k21);

		s02 = vis_fmul8x16al(x2, k02);
		s12 = vis_fmul8x16al(x2, k12);

		ss0 = vis_fpadd16(s_0, c_0);
		ss1 = vis_fpadd16(s_0, c_1);
		ss2 = vis_fpadd16(s_0, c_2);

		ss1 = vis_fpadd16(ss1, s11);
		ss2 = vis_fpadd16(ss2, s21);
		ss0 = vis_fpadd16(ss0, s02);
		ss1 = vis_fpadd16(ss1, s12);

		df0 = vis_fpack16(ss0);
		df1 = vis_fpack16(ss1);
		df2 = vis_fpack16(ss2);

		MLIB_MERGE4_U8(alp, df2, df1, df0, dd0, dd1);

		(*dp++) = dd0;
		(*dp++) = dd1;
	}

	if (n & 3) {
		mlib_d64 s_0, s11, s21, s02, s12, ss0, ss1, ss2, dd0, dd1;
		mlib_f32 x0, x1, x2, df0, df1, df2;
		mlib_s32 emask, mask2 = 0x0FFF0000 >> (4 * (n & 3));

		x0 = (*sp0++);
		x1 = (*sp1++);
		x2 = (*sp2++);

		s_0 = vis_fmul8x16al(x0, k_0);

		s11 = vis_fmul8x16al(x1, k11);
		s21 = vis_fmul8x16al(x1, k21);

		s02 = vis_fmul8x16al(x2, k02);
		s12 = vis_fmul8x16al(x2, k12);

		ss0 = vis_fpadd16(s_0, c_0);
		ss1 = vis_fpadd16(s_0, c_1);
		ss2 = vis_fpadd16(s_0, c_2);

		ss1 = vis_fpadd16(ss1, s11);
		ss2 = vis_fpadd16(ss2, s21);
		ss0 = vis_fpadd16(ss0, s02);
		ss1 = vis_fpadd16(ss1, s12);

		df0 = vis_fpack16(ss0);
		df1 = vis_fpack16(ss1);
		df2 = vis_fpack16(ss2);

		MLIB_MERGE4_U8(alp, df2, df1, df0, dd0, dd1);

		emask = (mask2 >> 8) & 0xFF;
		vis_pst_8(dd0, dp, emask);
		dp++;
		emask = mask2 & 0xFF;

		if (emask)
			vis_pst_8(dd1, dp, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
