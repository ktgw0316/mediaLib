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

#pragma ident	"@(#)mlib_v_VideoInterpXY_S16_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpXY_S16_U8_wxh,
 *      mlib_VideoInterpXY_S16_U8     - Performs half-pixel interpolation in
 *                                      both X and Y directions for
 *                                      non-replenishment mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoInterpXY_S16_U8(mlib_s16      *mc_block,
 *                                            const mlib_u8 *ref_block,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      frame_stride,
 *                                            mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_S16_U8_16x16(mlib_s16      *mc_block,
 *                                                  const mlib_u8 *ref_block,
 *                                                  mlib_s32      frame_stride,
 *                                                  mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_S16_U8_16x8(mlib_s16      *mc_block,
 *                                                 const mlib_u8 *ref_block,
 *                                                 mlib_s32      frame_stride,
 *                                                 mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_S16_U8_8x16(mlib_s16      *mc_block,
 *                                                 const mlib_u8 *ref_block,
 *                                                 mlib_s32      frame_stride,
 *                                                 mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_S16_U8_8x4(mlib_s16      *mc_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpXY_S16_U8_8x8(mlib_s16      *mc_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride);
 *
 * ARGUMENT
 *      mc_block        Pointer to an 8x8 motion-compensated reference block,
 *                      must be 8-byte aligned
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

#pragma weak mlib_VideoInterpXY_S16_U8 = __mlib_VideoInterpXY_S16_U8
#pragma weak mlib_VideoInterpXY_S16_U8_16x16 = \
	__mlib_VideoInterpXY_S16_U8_16x16
#pragma weak mlib_VideoInterpXY_S16_U8_16x8 = \
	__mlib_VideoInterpXY_S16_U8_16x8
#pragma weak mlib_VideoInterpXY_S16_U8_8x16 = \
	__mlib_VideoInterpXY_S16_U8_8x16
#pragma weak mlib_VideoInterpXY_S16_U8_8x4 = __mlib_VideoInterpXY_S16_U8_8x4
#pragma weak mlib_VideoInterpXY_S16_U8_8x8 = __mlib_VideoInterpXY_S16_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpXY_S16_U8) mlib_VideoInterpXY_S16_U8
	__attribute__((weak, alias("__mlib_VideoInterpXY_S16_U8")));
__typeof__(__mlib_VideoInterpXY_S16_U8_16x16) mlib_VideoInterpXY_S16_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpXY_S16_U8_16x16")));
__typeof__(__mlib_VideoInterpXY_S16_U8_16x8) mlib_VideoInterpXY_S16_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpXY_S16_U8_16x8")));
__typeof__(__mlib_VideoInterpXY_S16_U8_8x16) mlib_VideoInterpXY_S16_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpXY_S16_U8_8x16")));
__typeof__(__mlib_VideoInterpXY_S16_U8_8x4) mlib_VideoInterpXY_S16_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpXY_S16_U8_8x4")));
__typeof__(__mlib_VideoInterpXY_S16_U8_8x8) mlib_VideoInterpXY_S16_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpXY_S16_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPY8(height)                                \
	for (y = 0; y < height; y++) {                           \
	    s1 = vis_faligndata(sp1[0], sp1[1]);                 \
	    s2 = vis_faligndata(sp2[0], sp2[1]);                 \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fexpand(vis_read_hi(s2));                 \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    ss0[2 * y] = vis_fpadd16(s1hi, s2hi);                \
	    ss0[2 * y + 1] = vis_fpadd16(s1lo, s2lo);            \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOCOPY16(height)                               \
	for (y = 0; y < height; y++) {                           \
	    s1 = vis_faligndata(sp1[0], sp1[1]);                 \
	    s2 = vis_faligndata(sp2[0], sp2[1]);                 \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    ss0[4 * y] = vis_fpadd16(s1hi, s2hi);                \
	    ss0[4 * y + 1] = vis_fpadd16(s1lo, s2lo);            \
	    s1 = vis_faligndata(sp1[1], sp1[2]);                 \
	    s2 = vis_faligndata(sp2[1], sp2[2]);                 \
	    s1hi = vis_fexpand(vis_read_hi(s1));                 \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fexpand(vis_read_hi(s2));                 \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    ss0[4 * y + 2] = vis_fpadd16(s1hi, s2hi);            \
	    ss0[4 * y + 3] = vis_fpadd16(s1lo, s2lo);            \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP8(height)                              \
	for (y = 0; y < height; y++) {                           \
	    s1 = vis_faligndata(sp1[0], sp1[1]);                 \
	    s2 = vis_faligndata(sp2[0], vis_ld_d64_nf(sp2 + 1)); \
	    s1hi = vis_fexpand(vis_read_hi(s1));                 \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fexpand(vis_read_hi(s2));                 \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1hi = vis_fpadd16(s1hi, ss0[2 * y]);                \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    s1lo = vis_fpadd16(s1lo, ss0[2 * y + 1]);            \
	    dd[0] = vis_fmul8x16(strunc, s1hi);                  \
	    dd[1] = vis_fmul8x16(strunc, s1lo);                  \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	    dd += 2;                                             \
	}

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP16(height)                             \
	for (y = 0; y < height; y++) {                           \
	    s1 = sp1[0];                                         \
	    s3 = sp1[1];                                         \
	    s2 = sp2[0];                                         \
	    s4 = sp2[1];                                         \
	    s1 = vis_faligndata(s1, s3);                         \
	    s2 = vis_faligndata(s2, s4);                         \
	    s3 = vis_faligndata(s3, sp1[2]);                     \
	    s4 = vis_faligndata(s4, vis_ld_d64_nf(sp2 + 2));     \
	    s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);       \
	    s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);       \
	    s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);       \
	    s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);       \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1hi = vis_fpadd16(s1hi, ss0[4 * y]);                \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    s1lo = vis_fpadd16(s1lo, ss0[4 * y + 1]);            \
	    dd[0] = vis_fmul8x16(strunc, s1hi);                  \
	    dd[1] = vis_fmul8x16(strunc, s1lo);                  \
	    s1hi = vis_fexpand(vis_read_hi(s3));                 \
	    s1lo = vis_fmul8x16al(vis_read_lo(s3), fexpd);       \
	    s2hi = vis_fexpand(vis_read_hi(s4));                 \
	    s2lo = vis_fmul8x16al(vis_read_lo(s4), fexpd);       \
	    s1hi = vis_fpadd16(s1hi, s2hi);                      \
	    s1hi = vis_fpadd16(s1hi, ss0[4 * y + 2]);            \
	    s1lo = vis_fpadd16(s1lo, s2lo);                      \
	    s1lo = vis_fpadd16(s1lo, ss0[4 * y + 3]);            \
	    dd[2] = vis_fmul8x16(strunc, s1hi);                  \
	    dd[3] = vis_fmul8x16(strunc, s1lo);                  \
	    sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);   \
	    sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);   \
	    dd += 4;                                             \
	}

/* *********************************************************** */

#pragma align 8(mlib_IXY16const)
static const mlib_u32 mlib_IXY16const[] = { 0x04040404, 0x1000 };

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_S16_U8_8x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, ss0[16], *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_f32 strunc = vis_read_hi(*(mlib_d64 *)mlib_IXY16const);
	mlib_f32 fexpd = vis_read_lo(*(mlib_d64 *)mlib_IXY16const);

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY8(8);

	dd = (mlib_d64 *)mc_block;
	sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_S16_U8_8x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, ss0[16 * 2], *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_f32 strunc = vis_read_hi(*(mlib_d64 *)mlib_IXY16const);
	mlib_f32 fexpd = vis_read_lo(*(mlib_d64 *)mlib_IXY16const);

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY8(16);

	dd = (mlib_d64 *)mc_block;
	sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_S16_U8_8x4(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, ss0[8], *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_f32 strunc = vis_read_hi(*(mlib_d64 *)mlib_IXY16const);
	mlib_f32 fexpd = vis_read_lo(*(mlib_d64 *)mlib_IXY16const);

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY8(4);

	dd = (mlib_d64 *)mc_block;
	sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_S16_U8_16x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, ss0[16 * 2], *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2,
		s3, s4;
	mlib_f32 strunc = vis_read_hi(*(mlib_d64 *)mlib_IXY16const);
	mlib_f32 fexpd = vis_read_lo(*(mlib_d64 *)mlib_IXY16const);

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY16(8);

	dd = (mlib_d64 *)mc_block;
	sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_S16_U8_16x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 *dd, ss0[16 * 4], *sp1, *sp2, s1hi, s1lo, s2hi, s2lo, s1, s2,
		s3, s4;
	mlib_f32 strunc = vis_read_hi(*(mlib_d64 *)mlib_IXY16const);
	mlib_f32 fexpd = vis_read_lo(*(mlib_d64 *)mlib_IXY16const);

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
#pragma pipeloop(0)
	MLIB_V_VIDEOCOPY16(16);

	dd = (mlib_d64 *)mc_block;
	sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);

#pragma pipeloop(0)
	MLIB_V_VIDEOINTERP16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAXH	16
#define	MAXW	2

/* *********************************************************** */

mlib_status
__mlib_VideoInterpXY_S16_U8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_s32 y;
	mlib_d64 ss0[2 * MAXH * MAXW], *dd, *s0, *sp1, *sp2;
	mlib_d64 s1hi, s1lo, s2hi, s2lo, s1, s2, s3, s4;
	mlib_f32 strunc = vis_read_hi(*(mlib_d64 *)mlib_IXY16const);
	mlib_f32 fexpd = vis_read_lo(*(mlib_d64 *)mlib_IXY16const);

	sp1 = (mlib_d64 *)vis_alignaddr((void *)ref_block, 0);
	sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
	s0 = ss0;

	if (width == 8) {
#pragma pipeloop(0)
		for (y = 0; y < height; y++) {
			s1hi = sp1[0];
			s1lo = sp1[1];
			s1 = vis_faligndata(s1hi, s1lo);
			s2hi = sp2[0];
			s2lo = sp2[1];
			s2 = vis_faligndata(s2hi, s2lo);

			s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);
			s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);
			s2hi = vis_fexpand(vis_read_hi(s2));
			s2lo = vis_fexpand(vis_read_lo(s2));

			*(s0++) = vis_fpadd16(s1hi, s2hi);
			*(s0++) = vis_fpadd16(s1lo, s2lo);
			sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);
			sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);
		}

		dd = (mlib_d64 *)mc_block;
		sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
		sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
		s0 = ss0;

#pragma pipeloop(0)
		for (y = 0; y < height; y++) {
			s1hi = sp1[0];
			s1lo = sp1[1];
			s1 = vis_faligndata(s1hi, s1lo);
			s2hi = sp2[0];
			s2lo = vis_ld_d64_nf(sp2 + 1);
			s2 = vis_faligndata(s2hi, s2lo);

			s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);
			s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);
			s2hi = vis_fexpand(vis_read_hi(s2));
			s2lo = vis_fexpand(vis_read_lo(s2));

			s1hi = vis_fpadd16(s1hi, s2hi);
			s1hi = vis_fpadd16(s1hi, *(s0++));
			s1lo = vis_fpadd16(s1lo, s2lo);
			s1lo = vis_fpadd16(s1lo, *(s0++));

			*(dd++) = vis_fmul8x16(strunc, s1hi);
			*(dd++) = vis_fmul8x16(strunc, s1lo);
			sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);
			sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);
		}
	} else {
/* if(width==16) */

#pragma pipeloop(0)
		for (y = 0; y < height; y++) {
			s1hi = sp1[0];
			s3 = sp1[1];
			s1 = vis_faligndata(s1hi, s3);
			s2hi = sp2[0];
			s4 = sp2[1];
			s2 = vis_faligndata(s2hi, s4);

			s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);
			s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);
			s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);
			s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);

			*(s0++) = vis_fpadd16(s1hi, s2hi);
			*(s0++) = vis_fpadd16(s1lo, s2lo);

			s1 = vis_faligndata(s3, sp1[2]);
			s2 = vis_faligndata(s4, sp2[2]);

			s1hi = vis_fexpand(vis_read_hi(s1));
			s1lo = vis_fexpand(vis_read_lo(s1));
			s2hi = vis_fexpand(vis_read_hi(s2));
			s2lo = vis_fexpand(vis_read_lo(s2));

			*(s0++) = vis_fpadd16(s1hi, s2hi);
			*(s0++) = vis_fpadd16(s1lo, s2lo);

			sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);
			sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);
		}

		dd = (mlib_d64 *)mc_block;
		sp1 = (mlib_d64 *)vis_alignaddr((void *)(ref_block + 1), 0);
		sp2 = (mlib_d64 *)((mlib_u8 *)sp1 + frame_stride);
		s0 = ss0;

#pragma pipeloop(0)
		for (y = 0; y < height; y++) {
			s1hi = sp1[0];
			s3 = sp1[1];
			s1 = vis_faligndata(s1hi, s3);
			s2hi = sp2[0];
			s4 = sp2[1];
			s2 = vis_faligndata(s2hi, s4);

			s1hi = vis_fmul8x16al(vis_read_hi(s1), fexpd);
			s1lo = vis_fmul8x16al(vis_read_lo(s1), fexpd);
			s2hi = vis_fmul8x16al(vis_read_hi(s2), fexpd);
			s2lo = vis_fmul8x16al(vis_read_lo(s2), fexpd);

			s1hi = vis_fpadd16(s1hi, s2hi);
			s1hi = vis_fpadd16(s1hi, *(s0++));
			s1lo = vis_fpadd16(s1lo, s2lo);
			s1lo = vis_fpadd16(s1lo, *(s0++));

			s1 = vis_faligndata(s3, sp1[2]);
			s2 = vis_faligndata(s4, vis_ld_d64_nf(sp2 + 2));

			*(dd++) = vis_fmul8x16(strunc, s1hi);
			*(dd++) = vis_fmul8x16(strunc, s1lo);

			s1hi = vis_fexpand(vis_read_hi(s1));
			s1lo = vis_fexpand(vis_read_lo(s1));
			s2hi = vis_fexpand(vis_read_hi(s2));
			s2lo = vis_fexpand(vis_read_lo(s2));

			s1hi = vis_fpadd16(s1hi, s2hi);
			s1hi = vis_fpadd16(s1hi, *(s0++));
			s1lo = vis_fpadd16(s1lo, s2lo);
			s1lo = vis_fpadd16(s1lo, *(s0++));

			*(dd++) = vis_fmul8x16(strunc, s1hi);
			*(dd++) = vis_fmul8x16(strunc, s1lo);

			sp1 = (mlib_d64 *)((mlib_u8 *)sp1 + field_stride);
			sp2 = (mlib_d64 *)((mlib_u8 *)sp2 + field_stride);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
