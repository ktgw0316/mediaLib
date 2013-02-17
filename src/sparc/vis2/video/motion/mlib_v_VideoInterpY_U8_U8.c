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

#pragma ident	"@(#)mlib_v_VideoInterpY_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpY_U8_U8_wxh,
 *      mlib_VideoInterpY_U8_U8     - Performs half-pixel interpolation in the
 *                                    Y-direction for replenishment mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoInterpY_U8_U8(mlib_u8       *curr_block,
 *                                          const mlib_u8 *ref_block,
 *                                          mlib_s32      width,
 *                                          mlib_s32      height,
 *                                          mlib_s32      frame_stride,
 *                                          mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpY_U8_U8_16x16(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpY_U8_U8_16x8(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpY_U8_U8_8x16(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpY_U8_U8_8x4(mlib_u8       *curr_block,
 *                                              const mlib_u8 *ref_block,
 *                                              mlib_s32      frame_stride,
 *                                              mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpY_U8_U8_8x8(mlib_u8       *curr_block,
 *                                              const mlib_u8 *ref_block,
 *                                              mlib_s32      frame_stride,
 *                                              mlib_s32      field_stride);
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      frame_stride    Stride in bytes between adjacent rows in a frame in
 *                      both current and reference block,
 *                      must be a multiply of 8
 *      field_stride    Stride in bytes between adjacent rows in a field in
 *                      both current and reference block,
 *                      must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpY_U8_U8 = __mlib_VideoInterpY_U8_U8
#pragma weak mlib_VideoInterpY_U8_U8_16x16 = __mlib_VideoInterpY_U8_U8_16x16
#pragma weak mlib_VideoInterpY_U8_U8_16x8 = __mlib_VideoInterpY_U8_U8_16x8
#pragma weak mlib_VideoInterpY_U8_U8_8x16 = __mlib_VideoInterpY_U8_U8_8x16
#pragma weak mlib_VideoInterpY_U8_U8_8x4 = __mlib_VideoInterpY_U8_U8_8x4
#pragma weak mlib_VideoInterpY_U8_U8_8x8 = __mlib_VideoInterpY_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpY_U8_U8) mlib_VideoInterpY_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpY_U8_U8")));
__typeof__(__mlib_VideoInterpY_U8_U8_16x16) mlib_VideoInterpY_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpY_U8_U8_16x16")));
__typeof__(__mlib_VideoInterpY_U8_U8_16x8) mlib_VideoInterpY_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpY_U8_U8_16x8")));
__typeof__(__mlib_VideoInterpY_U8_U8_8x16) mlib_VideoInterpY_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpY_U8_U8_8x16")));
__typeof__(__mlib_VideoInterpY_U8_U8_8x4) mlib_VideoInterpY_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpY_U8_U8_8x4")));
__typeof__(__mlib_VideoInterpY_U8_U8_8x8) mlib_VideoInterpY_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpY_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP2(dst, src, srcf)                              \
	{                                                                \
	    mlib_d64 sdat, sfdat, sumhi, sumlo;                          \
	                                                                 \
	    sdat = src;                                                  \
	    sfdat = srcf;                                                \
	    sumhi = vis_fpsub16(vis_fpmerge(fzero, vis_read_hi(sdat)),   \
		    rounder);                                            \
	    sumlo = vis_fpsub16(vis_fpmerge(fzero, vis_read_lo(sdat)),   \
		    rounder);                                            \
	    sumhi = vis_fpadd16(sumhi,                                   \
		    vis_fmul8x16al(vis_read_hi(sfdat), fm1));            \
	    sumlo = vis_fpadd16(sumlo,                                   \
		    vis_fmul8x16al(vis_read_lo(sfdat), fm1));            \
	    dst = vis_fpack16_pair(sumhi, sumlo);                        \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP(dst, src, srcf)                                \
	{                                                                 \
	    mlib_d64 sdat, sfdat, sumhi, sumlo;                           \
	                                                                  \
	    sdat = src;                                                   \
	    sfdat = srcf;                                                 \
	    sumhi = vis_fpsub16(vis_fmul8x16al(vis_read_hi(sdat), fm1),   \
		    rounder);                                             \
	    sumlo = vis_fpsub16(vis_fmul8x16al(vis_read_lo(sdat), fm1),   \
		    rounder);                                             \
	    sumhi = vis_fpadd16(sumhi,                                    \
		    vis_fmul8x16al(vis_read_hi(sfdat), fm1));             \
	    sumlo = vis_fpadd16(sumlo,                                    \
		    vis_fmul8x16al(vis_read_lo(sfdat), fm1));             \
	    dst = vis_fpack16_pair(sumhi, sumlo);                         \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, sf0, sf1, sf2, d0, d1, *sd, *sfd, *dd;
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_d64 rounder = vis_fone();
	mlib_s32 y;

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	sfd = (mlib_d64 *)((mlib_u8 *)sd + frame_stride);

	if (width == 8) {
		if (!((mlib_u32)ref_block & 7)) {
#pragma pipeloop(0)
			for (y = 0; y < (height >> 1); ++y) {
				s0 = sd[0];
				sf0 = sfd[0];
				MLIB_V_VIDEOINTERP(d0, s0, sf0);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				sfd = (mlib_d64 *)((mlib_u8 *)sfd +
					field_stride);
				s0 = sd[0];
				sf0 = sfd[0];
				MLIB_V_VIDEOINTERP(d1, s0, sf0);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				sfd = (mlib_d64 *)((mlib_u8 *)sfd +
					field_stride);
				*dd = d0;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			}

		} else {
#pragma pipeloop(0)
			for (y = 0; y < (height >> 1); ++y) {
				s0 = sd[0];
				s1 = sd[1];
				sf0 = sfd[0];
				sf1 = sfd[1];
				MLIB_V_VIDEOINTERP(d0, vis_faligndata(s0, s1),
					vis_faligndata(sf0, sf1));
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				sfd = (mlib_d64 *)((mlib_u8 *)sfd +
					field_stride);
				s0 = sd[0];
				s1 = sd[1];
				sf0 = sfd[0];
				sf1 = sfd[1];
				MLIB_V_VIDEOINTERP(d1, vis_faligndata(s0, s1),
					vis_faligndata(sf0, sf1));
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				sfd = (mlib_d64 *)((mlib_u8 *)sfd +
					field_stride);
				*dd = d0;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			}
		}

	} else {
/* if(width==16) */

		if (!((mlib_u32)ref_block & 7)) {
#pragma pipeloop(0)
			for (y = 0; y < height; ++y) {
				s0 = sd[0];
				sf0 = sfd[0];
				MLIB_V_VIDEOINTERP(d0, s0, sf0);
				s1 = sd[1];
				sf1 = sfd[1];
				MLIB_V_VIDEOINTERP(d1, s1, sf1);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				sfd = (mlib_d64 *)((mlib_u8 *)sfd +
					field_stride);
				dd[0] = d0;
				dd[1] = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			}

		} else {
#pragma pipeloop(0)
			for (y = 0; y < height; ++y) {
				s0 = sd[0];
				s1 = sd[1];
				sf0 = sfd[0];
				sf1 = sfd[1];
				MLIB_V_VIDEOINTERP(d0, vis_faligndata(s0, s1),
					vis_faligndata(sf0, sf1));
				s2 = sd[2];
				sf2 = sfd[2];
				MLIB_V_VIDEOINTERP(d1, vis_faligndata(s1, s2),
					vis_faligndata(sf1, sf2));
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				sfd = (mlib_d64 *)((mlib_u8 *)sfd +
					field_stride);
				dd[0] = d0;
				dd[1] = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP8(height)                              \
	for (y = 0; y < height; y++) {                           \
	    s1hi = sp1[0];                                       \
	    s1lo = sp1[1];                                       \
	    s1 = vis_faligndata(s1hi, s1lo);                     \
	    s2hi = sp2[0];                                       \
	    s2lo = vis_ld_d64_nf(sp2 + 1);                       \
	    s2 = vis_faligndata(s2hi, s2lo);                     \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    s1hi = vis_fpsub16(s1hi, mone);                      \
	    s1lo = vis_fpsub16(s1lo, mone);                      \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    *dd = vis_fpack16_pair(s1hi, s1lo);                  \
	    dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);     \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP16(height)                             \
	for (y = 0; y < height; y++) {                           \
	    s1hi = sp1[0];                                       \
	    s1lo = sp1[1];                                       \
	    s1 = vis_faligndata(s1hi, s1lo);                     \
	    s2hi = sp2[0];                                       \
	    s2lo = sp2[1];                                       \
	    s2 = vis_faligndata(s2hi, s2lo);                     \
	    s1hi = vis_fpmerge(fzero, vis_read_hi(s1));          \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    s1hi = vis_fpsub16(s1hi, mone);                      \
	    s1lo = vis_fpsub16(s1lo, mone);                      \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    *dd = vis_fpack16_pair(s1hi, s1lo);                  \
	    s1hi = sp1[1];                                       \
	    s1lo = sp1[2];                                       \
	    s1 = vis_faligndata(s1hi, s1lo);                     \
	    s2hi = sp2[1];                                       \
	    s2lo = vis_ld_d64_nf(sp2 + 2);                       \
	    s2 = vis_faligndata(s2hi, s2lo);                     \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    s1hi = vis_fpsub16(s1hi, mone);                      \
	    s1lo = vis_fpsub16(s1lo, mone);                      \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    *(dd + 1) = vis_fpack16_pair(s1hi, s1lo);            \
	    dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);     \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 mone = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 mone = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_f32 fzero = vis_fzeros();

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 mone = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_f32 fzero = vis_fzeros();

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 mone = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 mone = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sp1 = (mlib_d64 *)((mlib_addr)ref_block & ~7);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
