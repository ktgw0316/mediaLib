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

#pragma ident	"@(#)mlib_v_VideoCopyRefAve_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRefAve_U8_U8_wxh,
 *      mlib_VideoCopyRefAve_U8_U8     - Copies and averages a wxh block from
 *                                       reference block to current block
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoCopyRefAve_U8_U8(mlib_u8       *curr_block,
 *                                             const mlib_u8 *ref_block,
 *                                             mlib_s32      width,
 *                                             mlib_s32      height,
 *                                             mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRefAve_U8_U8_16x16(mlib_u8       *curr_block,
 *                                                   const mlib_u8 *ref_block,
 *                                                   mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRefAve_U8_U8_16x8(mlib_u8       *curr_block,
 *                                                  const mlib_u8 *ref_block,
 *                                                  mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRefAve_U8_U8_8x16(mlib_u8       *curr_block,
 *                                                  const mlib_u8 *ref_block,
 *                                                  mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRefAve_U8_U8_8x4(mlib_u8       *curr_block,
 *                                                 const mlib_u8 *ref_block,
 *                                                 mlib_s32      stride);
 *      mlib_status mlib_VideoCopyRefAve_U8_U8_8x8(mlib_u8       *curr_block,
 *                                                 const mlib_u8 *ref_block,
 *                                                 mlib_s32      stride);
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      stride          Stride in bytes between adjacent rows in both current
 *                      and reference block, must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRefAve_U8_U8 = __mlib_VideoCopyRefAve_U8_U8
#pragma weak mlib_VideoCopyRefAve_U8_U8_16x16 = \
	__mlib_VideoCopyRefAve_U8_U8_16x16
#pragma weak mlib_VideoCopyRefAve_U8_U8_16x8 = \
	__mlib_VideoCopyRefAve_U8_U8_16x8
#pragma weak mlib_VideoCopyRefAve_U8_U8_8x16 = \
	__mlib_VideoCopyRefAve_U8_U8_8x16
#pragma weak mlib_VideoCopyRefAve_U8_U8_8x4 = \
	__mlib_VideoCopyRefAve_U8_U8_8x4
#pragma weak mlib_VideoCopyRefAve_U8_U8_8x8 = \
	__mlib_VideoCopyRefAve_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRefAve_U8_U8) mlib_VideoCopyRefAve_U8_U8
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_16x16) mlib_VideoCopyRefAve_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_16x16")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_16x8) mlib_VideoCopyRefAve_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_16x8")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_8x16) mlib_VideoCopyRefAve_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_8x16")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_8x4) mlib_VideoCopyRefAve_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_8x4")));
__typeof__(__mlib_VideoCopyRefAve_U8_U8_8x8) mlib_VideoCopyRefAve_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRefAve_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_s32 mlib_imult = 0x100;

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPYAVG(dst, src0, src1)                    \
	s1 = vis_faligndata(src0, src1);                        \
	s2hi = vis_fmul8x16al(vis_read_hi(dst), fexpd);         \
	s2lo = vis_fmul8x16al(vis_read_lo(dst), fexpd);         \
	s2hi = vis_fpsub16(s2hi, rnd);                          \
	s2lo = vis_fpsub16(s2lo, rnd);                          \
	s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);          \
	s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);          \
	s1hi = vis_fpadd16(s1hi, s2hi);                         \
	s1lo = vis_fpadd16(s1lo, s2lo);                         \
	dst = vis_fpack16_pair(s1hi, s1lo)

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPYAVG16(height)                           \
	for (y = 0; y < height; y++) {                          \
	    MLIB_V_VIDEOCOPYAVG(*dd, sp1[0], sp1[1]);           \
	    MLIB_V_VIDEOCOPYAVG(*(dd + 1), sp1[1],              \
		vis_ld_d64_nf(sp1 + 2));                        \
	    dd = (mlib_d64 *)((mlib_u8 *)dd + stride);          \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);        \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPYAVG8(height)                                    \
	for (y = 0; y < height; y++) {                                  \
	    MLIB_V_VIDEOCOPYAVG(*dd, sp1[0], vis_ld_d64_nf(sp1 + 1));   \
	    dd = (mlib_d64 *)((mlib_u8 *)dd + stride);                  \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);                \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, s1, s1hi, s1lo, s2hi, s2lo;
	mlib_f32 fexpd = *(mlib_f32 *)&mlib_imult;
	mlib_d64 rnd = vis_fone();

	vis_write_gsr((6 << 3) + ((mlib_s32)ref_block & 7));

	dd = (mlib_d64 *)curr_block;

	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPYAVG16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, s1, s1hi, s1lo, s2hi, s2lo;
	mlib_f32 fexpd = *(mlib_f32 *)&mlib_imult;
	mlib_d64 rnd = vis_fone();

	vis_write_gsr((6 << 3) + ((mlib_s32)ref_block & 7));

	dd = (mlib_d64 *)curr_block;

	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPYAVG16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, s1, s1hi, s1lo, s2hi, s2lo;
	mlib_f32 fexpd = *(mlib_f32 *)&mlib_imult;
	mlib_d64 rnd = vis_fone();

	vis_write_gsr((6 << 3) + ((mlib_s32)ref_block & 7));

	dd = (mlib_d64 *)curr_block;

	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPYAVG8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;
	mlib_d64 *sp1, *dd, s1, s1hi, s1lo, s2hi, s2lo;
	mlib_f32 fexpd = *(mlib_f32 *)&mlib_imult;
	mlib_d64 rnd = vis_fone();

	vis_write_gsr((6 << 3) + ((mlib_s32)ref_block & 7));

	dd = (mlib_d64 *)curr_block;

	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);

#pragma pipeloop(0)
	MLIB_V_VIDEOCOPYAVG8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_d64 *sp1, *dd, s1, s2, s3, s4, s1hi, s1lo, s2hi, s2lo;
	mlib_f32 fexpd = *(mlib_f32 *)&mlib_imult;
	mlib_d64 rnd = vis_fone();
	mlib_d64 dd0, dd1, dd2, dd3;

	vis_write_gsr((6 << 3) + ((mlib_s32)ref_block & 7));

	dd = (mlib_d64 *)curr_block;

	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	s1 = vis_faligndata(sp1[0], sp1[1]);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);
	s2 = vis_faligndata(sp1[0], sp1[1]);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);
	s3 = vis_faligndata(sp1[0], sp1[1]);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + stride);
	s4 = vis_faligndata(sp1[0], vis_ld_d64_nf(sp1 + 1));

	dd0 = *dd;
	dd1 = *((mlib_d64 *)((mlib_u8 *)dd + stride));
	dd2 = *((mlib_d64 *)((mlib_u8 *)dd + 2 * stride));
	dd3 = *((mlib_d64 *)((mlib_u8 *)dd + 3 * stride));

	s2hi = vis_fmul8x16al(vis_read_hi(dd0), fexpd);
	s2lo = vis_fmul8x16al(vis_read_lo(dd0), fexpd);
	s2hi = vis_fpsub16(s2hi, rnd);
	s2lo = vis_fpsub16(s2lo, rnd);
	s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);
	s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);
	s1hi = vis_fpadd16(s1hi, s2hi);
	s1lo = vis_fpadd16(s1lo, s2lo);
	dd0 = vis_fpack16_pair(s1hi, s1lo);
	s2hi = vis_fmul8x16al(vis_read_hi(dd1), fexpd);
	s2lo = vis_fmul8x16al(vis_read_lo(dd1), fexpd);
	s2hi = vis_fpsub16(s2hi, rnd);
	s2lo = vis_fpsub16(s2lo, rnd);
	s1hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);
	s1lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);
	s1hi = vis_fpadd16(s1hi, s2hi);
	s1lo = vis_fpadd16(s1lo, s2lo);
	dd1 = vis_fpack16_pair(s1hi, s1lo);
	s2hi = vis_fmul8x16al(vis_read_hi(dd2), fexpd);
	s2lo = vis_fmul8x16al(vis_read_lo(dd2), fexpd);
	s2hi = vis_fpsub16(s2hi, rnd);
	s2lo = vis_fpsub16(s2lo, rnd);
	s1hi = vis_fmul8x16al(vis_read_hi(s3), fexpd);
	s1lo = vis_fmul8x16al(vis_read_lo(s3), fexpd);
	s1hi = vis_fpadd16(s1hi, s2hi);
	s1lo = vis_fpadd16(s1lo, s2lo);
	dd2 = vis_fpack16_pair(s1hi, s1lo);
	s2hi = vis_fmul8x16al(vis_read_hi(dd3), fexpd);
	s2lo = vis_fmul8x16al(vis_read_lo(dd3), fexpd);
	s2hi = vis_fpsub16(s2hi, rnd);
	s2lo = vis_fpsub16(s2lo, rnd);
	s1hi = vis_fmul8x16al(vis_read_hi(s4), fexpd);
	s1lo = vis_fmul8x16al(vis_read_lo(s4), fexpd);
	s1hi = vis_fpadd16(s1hi, s2hi);
	s1lo = vis_fpadd16(s1lo, s2lo);
	dd3 = vis_fpack16_pair(s1hi, s1lo);

	*dd = dd0;
	*((mlib_d64 *)((mlib_u8 *)dd + stride)) = dd1;
	*((mlib_d64 *)((mlib_u8 *)dd + 2 * stride)) = dd2;
	*((mlib_d64 *)((mlib_u8 *)dd + 3 * stride)) = dd3;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef MLIB_V_VIDEOCOPYAVG

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPYAVG(ddat, sdat)                                    \
	{                                                                  \
	    mlib_d64 sumhi, sumlo;                                         \
	                                                                   \
	    sumhi = vis_fpsub16(vis_fmul8x16al(vis_read_hi(sdat), fm1),    \
		    rounder);                                              \
	    sumlo = vis_fpsub16(vis_fmul8x16al(vis_read_lo(sdat), fm1),    \
		    rounder);                                              \
	    sumhi = vis_fpadd16(sumhi, vis_fmul8x16al(vis_read_hi(ddat),   \
		    fm1));                                                 \
	    sumlo = vis_fpadd16(sumlo, vis_fmul8x16al(vis_read_lo(ddat),   \
		    fm1));                                                 \
	    ddat = vis_fpack16_pair(sumhi, sumlo);                         \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPYAVG_A(ddat, sdat)                                  \
	{                                                                  \
	    mlib_d64 sumhi, sumlo;                                         \
	                                                                   \
	    sumhi = vis_fpsub16(vis_fpmerge(fzero, vis_read_hi(sdat)),     \
		    rounder);                                              \
	    sumlo = vis_fpsub16(vis_fpmerge(fzero, vis_read_lo(sdat)),     \
		    rounder);                                              \
	    sumhi = vis_fpadd16(sumhi, vis_fmul8x16al(vis_read_hi(ddat),   \
		    fm1));                                                 \
	    sumlo = vis_fpadd16(sumlo, vis_fmul8x16al(vis_read_lo(ddat),   \
		    fm1));                                                 \
	    ddat = vis_fpack16_pair(sumhi, sumlo);                         \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRefAve_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_d64 dzero = vis_fzero();
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();
	mlib_d64 s0, s1, s2, s3, s4, d0, d1, d2, d3, *sd, *dd;
	const mlib_f32 fm1 = vis_to_float(0x100);

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	if (width == 8) {
		height >>= 2;

		if (!((mlib_u32)ref_block & 7)) {
			do {
				s0 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s1 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = *sd;
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s3 = *sd;
				d0 = *dd;
				d1 = *(mlib_d64 *)((mlib_u8 *)dd + stride);
				d2 = *(mlib_d64 *)((mlib_u8 *)dd + 2 * stride);
				d3 = *(mlib_d64 *)((mlib_u8 *)dd + 3 * stride);
				MLIB_V_VIDEOCOPYAVG_A(d0, s0);
				MLIB_V_VIDEOCOPYAVG_A(d1, s1);
				MLIB_V_VIDEOCOPYAVG(d2, s2);
				MLIB_V_VIDEOCOPYAVG(d3, s3);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				*dd = d0;
				*(mlib_d64 *)((mlib_u8 *)dd + stride) = d1;
				*(mlib_d64 *)((mlib_u8 *)dd + 2 * stride) = d2;
				*(mlib_d64 *)((mlib_u8 *)dd + 3 * stride) = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + 4 * stride);
			} while (--height);
		} else {
			do {
				s0 = sd[0];
				s1 = sd[1];
				s0 = vis_faligndata(s0, s1);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s1 = sd[0];
				s2 = sd[1];
				s1 = vis_faligndata(s1, s2);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = sd[0];
				s3 = sd[1];
				s2 = vis_faligndata(s2, s3);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s3 = sd[0];
				s4 = vis_ld_d64_nf(sd + 1);
				s3 = vis_faligndata(s3, s4);
				d0 = *dd;
				d1 = *(mlib_d64 *)((mlib_u8 *)dd + stride);
				d2 = *(mlib_d64 *)((mlib_u8 *)dd + 2 * stride);
				d3 = *(mlib_d64 *)((mlib_u8 *)dd + 3 * stride);
				MLIB_V_VIDEOCOPYAVG(d0, s0);
				MLIB_V_VIDEOCOPYAVG(d1, s1);
				MLIB_V_VIDEOCOPYAVG_A(d2, s2);
				MLIB_V_VIDEOCOPYAVG_A(d3, s3);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				*dd = d0;
				*(mlib_d64 *)((mlib_u8 *)dd + stride) = d1;
				*(mlib_d64 *)((mlib_u8 *)dd + 2 * stride) = d2;
				*(mlib_d64 *)((mlib_u8 *)dd + 3 * stride) = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + 4 * stride);
			} while (--height);
		}
	} else {
/* if (width==16) */

		height >>= 1;

		if (!((mlib_u32)ref_block & 7)) {
			do {
				s0 = sd[0];
				s1 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = sd[0];
				s3 = sd[1];
				d0 = dd[0];
				d1 = dd[1];
				d2 = ((mlib_d64 *)((mlib_u8 *)dd + stride))[0];
				d3 = ((mlib_d64 *)((mlib_u8 *)dd + stride))[1];
				MLIB_V_VIDEOCOPYAVG_A(d0, s0);
				MLIB_V_VIDEOCOPYAVG_A(d1, s1);
				MLIB_V_VIDEOCOPYAVG(d2, s2);
				MLIB_V_VIDEOCOPYAVG(d3, s3);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				dd[0] = d0;
				dd[1] = d1;
				((mlib_d64 *)((mlib_u8 *)dd + stride))[0] = d2;
				((mlib_d64 *)((mlib_u8 *)dd + stride))[1] = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + 2 * stride);
			} while (--height);
		} else {
			do {
				s0 = sd[0];
				s1 = sd[1];
				s2 = sd[2];
				s0 = vis_faligndata(s0, s1);
				s1 = vis_faligndata(s1, s2);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				s2 = sd[0];
				s3 = sd[1];
				s4 = vis_ld_d64_nf(sd + 2);
				s2 = vis_faligndata(s2, s3);
				s3 = vis_faligndata(s3, s4);
				d0 = dd[0];
				d1 = dd[1];
				d2 = ((mlib_d64 *)((mlib_u8 *)dd + stride))[0];
				d3 = ((mlib_d64 *)((mlib_u8 *)dd + stride))[1];
				MLIB_V_VIDEOCOPYAVG(d0, s0);
				MLIB_V_VIDEOCOPYAVG(d1, s1);
				MLIB_V_VIDEOCOPYAVG_A(d2, s2);
				MLIB_V_VIDEOCOPYAVG_A(d3, s3);
				sd = (mlib_d64 *)((mlib_u8 *)sd + stride);
				dd[0] = d0;
				dd[1] = d1;
				((mlib_d64 *)((mlib_u8 *)dd + stride))[0] = d2;
				((mlib_d64 *)((mlib_u8 *)dd + stride))[1] = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + 2 * stride);
			} while (--height);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
