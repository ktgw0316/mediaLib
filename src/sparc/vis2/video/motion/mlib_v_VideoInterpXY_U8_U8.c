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

#pragma ident	"@(#)mlib_v_VideoInterpXY_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpXY_U8_U8_wxh,
 *      mlib_VideoInterpXY_U8_U8     - Performs half-pixel interpolation in
 *                                     both X and Y directions for
 *                                     replenishment mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoInterpXY_U8_U8(mlib_u8       *curr_block,
 *                                           const mlib_u8 *ref_block,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      frame_stride,
 *                                           mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_U8_U8_16x16(mlib_u8       *curr_block,
 *                                                 const mlib_u8 *ref_block,
 *                                                 mlib_s32      frame_stride,
 *                                                 mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_U8_U8_16x8(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_U8_U8_8x16(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_U8_U8_8x4(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_U8_U8_8x8(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride);
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      frame_stride    Stride in bytes between adjacent rows in a frame in
 *                      reference block, must be a multiply of 8
 *      field_stride    Stride in bytes between adjacent rows in a field in
 *                      reference block, must be a multiply of 8
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpXY_U8_U8 = __mlib_VideoInterpXY_U8_U8
#pragma weak mlib_VideoInterpXY_U8_U8_16x16 = \
	__mlib_VideoInterpXY_U8_U8_16x16
#pragma weak mlib_VideoInterpXY_U8_U8_16x8 = __mlib_VideoInterpXY_U8_U8_16x8
#pragma weak mlib_VideoInterpXY_U8_U8_8x16 = __mlib_VideoInterpXY_U8_U8_8x16
#pragma weak mlib_VideoInterpXY_U8_U8_8x4 = __mlib_VideoInterpXY_U8_U8_8x4
#pragma weak mlib_VideoInterpXY_U8_U8_8x8 = __mlib_VideoInterpXY_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpXY_U8_U8) mlib_VideoInterpXY_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpXY_U8_U8")));
__typeof__(__mlib_VideoInterpXY_U8_U8_16x16) mlib_VideoInterpXY_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpXY_U8_U8_16x16")));
__typeof__(__mlib_VideoInterpXY_U8_U8_16x8) mlib_VideoInterpXY_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpXY_U8_U8_16x8")));
__typeof__(__mlib_VideoInterpXY_U8_U8_8x16) mlib_VideoInterpXY_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpXY_U8_U8_8x16")));
__typeof__(__mlib_VideoInterpXY_U8_U8_8x4) mlib_VideoInterpXY_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpXY_U8_U8_8x4")));
__typeof__(__mlib_VideoInterpXY_U8_U8_8x8) mlib_VideoInterpXY_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpXY_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP8(height)                              \
	for (y = 0; y < height; y++) {                           \
	    s1 = vis_bshuffle(sp0[0], sp0[1]);                   \
	    s2 = vis_bshuffle(sp1[0], sp1[1]);                   \
	    s3 = vis_faligndata(sp2[0], sp2[1]);                 \
	    s4 = vis_faligndata(sp3[0], vis_ld_d64_nf(sp3 + 1)); \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    ld0 = vis_fpadd16(s1hi, s2hi);                       \
	    ld1 = vis_fpadd16(s1lo, s2lo);                       \
	    s3hi = vis_fmul8x16al(vis_read_hi(s3), fexpd);       \
	    s3lo = vis_fmul8x16al(vis_read_lo(s3), fexpd);       \
	    s4hi = vis_fmul8x16al(vis_read_hi(s4), fexpd);       \
	    s4lo = vis_fmul8x16al(vis_read_lo(s4), fexpd);       \
	    s3hi = vis_fpsub16(s3hi, mtwo);                      \
	    s3lo = vis_fpsub16(s3lo, mtwo);                      \
	    s3hi = vis_fpadd16(s3hi, s4hi);                      \
	    s3hi = vis_fpadd16(s3hi, ld0);                       \
	    s3lo = vis_fpadd16(s3lo, s4lo);                      \
	    s3lo = vis_fpadd16(s3lo, ld1);                       \
	    *dd = vis_fpack16_pair(s3hi, s3lo);                  \
	    dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);     \
	    sp0 = (mlib_d64 *)((mlib_u8 *)sp0 + field_stride);   \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	    sp3 = (mlib_d64 *)((mlib_u8 *)sp3 + field_stride);   \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP16(height)                             \
	for (y = 0; y < height; y++) {                           \
	    s1 = vis_bshuffle(sp0[0], sp0[1]);                   \
	    s2 = vis_bshuffle(sp1[0], sp1[1]);                   \
	    s3 = vis_faligndata(sp2[0], sp2[1]);                 \
	    s4 = vis_faligndata(sp3[0], sp3[1]);                 \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    ld0 = vis_fpadd16(s1hi, s2hi);                       \
	    ld1 = vis_fpadd16(s1lo, s2lo);                       \
	    s1hi = vis_fmul8x16al(vis_read_hi(s3), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s3), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s4), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s4), fexpd);       \
	    s1hi = vis_fpsub16(s1hi, mtwo);                      \
	    s1lo = vis_fpsub16(s1lo, mtwo);                      \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1hi = vis_fpadd16(s1hi, ld0);                       \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    s1lo = vis_fpadd16(s1lo, ld1);                       \
	    *dd = vis_fpack16_pair(s1hi, s1lo);                  \
	    s1 = vis_bshuffle(sp0[1], sp0[2]);                   \
	    s2 = vis_bshuffle(sp1[1], sp1[2]);                   \
	    s3 = vis_faligndata(sp2[1], sp2[2]);                 \
	    s4 = vis_faligndata(sp3[1], vis_ld_d64_nf(sp3 + 2)); \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    ld0 = vis_fpadd16(s1hi, s2hi);                       \
	    ld1 = vis_fpadd16(s1lo, s2lo);                       \
	    s1hi = vis_fmul8x16al(vis_read_hi(s3), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s3), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s4), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s4), fexpd);       \
	    s1hi = vis_fpsub16(s1hi, mtwo);                      \
	    s1lo = vis_fpsub16(s1lo, mtwo);                      \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1hi = vis_fpadd16(s1hi, ld0);                       \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    s1lo = vis_fpadd16(s1lo, ld1);                       \
	    *(dd + 1) = vis_fpack16_pair(s1hi, s1lo);            \
	    dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);     \
	    sp0 = (mlib_d64 *)((mlib_u8 *)sp0 + field_stride);   \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	    sp3 = (mlib_d64 *)((mlib_u8 *)sp3 + field_stride);   \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y, off;
	mlib_d64 *dd, *sp0, *sp1, *sp2, *sp3;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 s3hi, s3lo, s4hi, s4lo, s3, s4;
	mlib_d64 mtwo = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_d64 ld0, ld1;

	mtwo = vis_fpadd16(mtwo, mtwo);
	dd = (mlib_d64 *)curr_block;
	off = (mlib_addr)ref_block & 7;
	sp0 = (mlib_d64 *)(ref_block - off);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp0 + frame_stride);
	vis_write_gsr((5 << 3) + ((mlib_s32)(ref_block + 1) & 7));
	sp2 = (mlib_d64 *)((mlib_addr)(ref_block + 1) & ~7);
	sp3 = (mlib_d64 *)((mlib_u8 *)sp2 + frame_stride);
	vis_write_bmask(off * 0x11111111, 0x01234567);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y, off;
	mlib_d64 *dd, *sp0, *sp1, *sp2, *sp3;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 s3, s4;
	mlib_d64 mtwo = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_d64 ld0, ld1;

	mtwo = vis_fpadd16(mtwo, mtwo);
	dd = (mlib_d64 *)curr_block;
	off = (mlib_addr)ref_block & 7;
	sp0 = (mlib_d64 *)(ref_block - off);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp0 + frame_stride);
	vis_write_gsr((5 << 3) + ((mlib_s32)(ref_block + 1) & 7));
	sp2 = (mlib_d64 *)((mlib_addr)(ref_block + 1) & ~7);
	sp3 = (mlib_d64 *)((mlib_u8 *)sp2 + frame_stride);
	vis_write_bmask(off * 0x11111111, 0x01234567);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y, off;
	mlib_d64 *dd, *sp0, *sp1, *sp2, *sp3;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 s3hi, s3lo, s4hi, s4lo, s3, s4;
	mlib_d64 mtwo = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_d64 ld0, ld1;

	mtwo = vis_fpadd16(mtwo, mtwo);
	dd = (mlib_d64 *)curr_block;
	off = (mlib_addr)ref_block & 7;
	sp0 = (mlib_d64 *)(ref_block - off);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp0 + frame_stride);
	vis_write_gsr((5 << 3) + ((mlib_s32)(ref_block + 1) & 7));
	sp2 = (mlib_d64 *)((mlib_addr)(ref_block + 1) & ~7);
	sp3 = (mlib_d64 *)((mlib_u8 *)sp2 + frame_stride);
	vis_write_bmask(off * 0x11111111, 0x01234567);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y, off;
	mlib_d64 *dd, *sp0, *sp1, *sp2, *sp3;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 s3, s4;
	mlib_d64 mtwo = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_d64 ld0, ld1;

	mtwo = vis_fpadd16(mtwo, mtwo);
	dd = (mlib_d64 *)curr_block;
	off = (mlib_addr)ref_block & 7;
	sp0 = (mlib_d64 *)(ref_block - off);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp0 + frame_stride);
	vis_write_gsr((5 << 3) + ((mlib_s32)(ref_block + 1) & 7));
	sp2 = (mlib_d64 *)((mlib_addr)(ref_block + 1) & ~7);
	sp3 = (mlib_d64 *)((mlib_u8 *)sp2 + frame_stride);
	vis_write_bmask(off * 0x11111111, 0x01234567);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y, off;
	mlib_d64 *dd, *sp0, *sp1, *sp2, *sp3;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 s3hi, s3lo, s4hi, s4lo, s3, s4;
	mlib_d64 mtwo = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_d64 ld0, ld1;

	mtwo = vis_fpadd16(mtwo, mtwo);
	dd = (mlib_d64 *)curr_block;
	off = (mlib_addr)ref_block & 7;
	sp0 = (mlib_d64 *)(ref_block - off);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp0 + frame_stride);
	vis_write_gsr((5 << 3) + ((mlib_s32)(ref_block + 1) & 7));
	sp2 = (mlib_d64 *)((mlib_addr)(ref_block + 1) & ~7);
	sp3 = (mlib_d64 *)((mlib_u8 *)sp2 + frame_stride);
	vis_write_bmask(off * 0x11111111, 0x01234567);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y, off;
	mlib_d64 *dd, *sp0, *sp1, *sp2, *sp3;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 s3hi, s3lo, s4hi, s4lo, s3, s4;
	mlib_d64 mtwo = vis_fone();
	mlib_f32 fexpd = vis_to_float(0x100);
	mlib_d64 ld0, ld1;

	mtwo = vis_fpadd16(mtwo, mtwo);
	dd = (mlib_d64 *)curr_block;
	off = (mlib_addr)ref_block & 7;
	sp0 = (mlib_d64 *)(ref_block - off);
	sp1 = (mlib_d64 *)((mlib_u8 *)sp0 + frame_stride);
	vis_write_gsr((5 << 3) + ((mlib_s32)(ref_block + 1) & 7));
	sp2 = (mlib_d64 *)((mlib_addr)(ref_block + 1) & ~7);
	sp3 = (mlib_d64 *)((mlib_u8 *)sp2 + frame_stride);
	vis_write_bmask(off * 0x11111111, 0x01234567);

	if (width == 8) {
#pragma pipeloop(0)
		MLIB_V_VIDEOINTERP8(height);
	} else {
/* if(width==16) */
#pragma pipeloop(0)
		MLIB_V_VIDEOINTERP16(height);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
