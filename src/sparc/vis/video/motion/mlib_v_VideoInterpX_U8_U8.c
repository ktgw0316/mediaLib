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

#pragma ident	"@(#)mlib_v_VideoInterpX_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpX_U8_U8_wxh,
 *      mlib_VideoInterpX_U8_U8     - Performs half-pixel interpolation in the
 *                                    X-direction for replenishment mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoInterpX_U8_U8(mlib_u8       *curr_block,
 *                                          const mlib_u8 *ref_block,
 *                                          mlib_s32      width,
 *                                          mlib_s32      height,
 *                                          mlib_s32      frame_stride,
 *                                          mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpX_U8_U8_16x16(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpX_U8_U8_16x8(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpX_U8_U8_8x16(mlib_u8       *curr_block,
 *                                               const mlib_u8 *ref_block,
 *                                               mlib_s32      frame_stride,
 *                                               mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpX_U8_U8_8x4(mlib_u8       *curr_block,
 *                                              const mlib_u8 *ref_block,
 *                                              mlib_s32      frame_stride,
 *                                              mlib_s32      field_stride);
 *      mlib_status mlib_VideoInterpX_U8_U8_8x8(mlib_u8       *curr_block,
 *                                              const mlib_u8 *ref_block,
 *                                              mlib_s32      frame_stride,
 *                                              mlib_s32      field_stride);
 *
 * ARGUMENT
 *      curr_block      Pointer to current block of size wxh,
 *                      must be 8-byte aligned
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

#pragma weak mlib_VideoInterpX_U8_U8 = __mlib_VideoInterpX_U8_U8
#pragma weak mlib_VideoInterpX_U8_U8_16x16 = __mlib_VideoInterpX_U8_U8_16x16
#pragma weak mlib_VideoInterpX_U8_U8_16x8 = __mlib_VideoInterpX_U8_U8_16x8
#pragma weak mlib_VideoInterpX_U8_U8_8x16 = __mlib_VideoInterpX_U8_U8_8x16
#pragma weak mlib_VideoInterpX_U8_U8_8x4 = __mlib_VideoInterpX_U8_U8_8x4
#pragma weak mlib_VideoInterpX_U8_U8_8x8 = __mlib_VideoInterpX_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpX_U8_U8) mlib_VideoInterpX_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpX_U8_U8")));
__typeof__(__mlib_VideoInterpX_U8_U8_16x16) mlib_VideoInterpX_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpX_U8_U8_16x16")));
__typeof__(__mlib_VideoInterpX_U8_U8_16x8) mlib_VideoInterpX_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpX_U8_U8_16x8")));
__typeof__(__mlib_VideoInterpX_U8_U8_8x16) mlib_VideoInterpX_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpX_U8_U8_8x16")));
__typeof__(__mlib_VideoInterpX_U8_U8_8x4) mlib_VideoInterpX_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpX_U8_U8_8x4")));
__typeof__(__mlib_VideoInterpX_U8_U8_8x8) mlib_VideoInterpX_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpX_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP4(dst, src, srcf)                               \
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

#define	MLIB_V_VIDEOINTERP1(dst, src, srcf)                               \
	{                                                                 \
	    mlib_d64 sdat, sfdat, sumhi, sumlo;                           \
	                                                                  \
	    sdat = src;                                                   \
	    sfdat = srcf;                                                 \
	    sumhi = vis_fpsub16(vis_fmul8x16al(vis_read_hi(sdat), fm1),   \
		    rounder);                                             \
	    sumlo = vis_fpsub16(vis_fmul8x16al(vis_read_lo(sdat), fm1),   \
		    rounder);                                             \
	    sumhi = vis_fpadd16(sumhi, vis_fpmerge(fzero,                 \
		    vis_read_hi(sfdat)));                                 \
	    sumlo = vis_fpadd16(sumlo, vis_fpmerge(fzero,                 \
		    vis_read_lo(sfdat)));                                 \
	    dst = vis_fpack16_pair(sumhi, sumlo);                         \
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
	    sumlo = vis_fpsub16(vis_fpmerge(fzero, vis_read_lo(sdat)),    \
		    rounder);                                             \
	    sumhi = vis_fpadd16(sumhi,                                    \
		    vis_fmul8x16al(vis_read_hi(sfdat), fm1));             \
	    sumlo = vis_fpadd16(sumlo, vis_fpmerge(fzero,                 \
		    vis_read_lo(sfdat)));                                 \
	    dst = vis_fpack16_pair(sumhi, sumlo);                         \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, d0, d1, d2, d3, *sd, *dd;
	mlib_d64 dzero = vis_fzero();
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();
	mlib_s32 y;

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	if (width == 8) {
		y = height >> 2;

		if (((mlib_s32)(ref_block + 1) & 7)) {
			do {
				s0 = sd[0];
				s1 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d0 = vis_faligndata(s0, s1);
				s2 = sd[0];
				s3 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d1 = vis_faligndata(s2, s3);
				s4 = sd[0];
				s5 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d2 = vis_faligndata(s4, s5);
				s6 = sd[0];
				s7 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d3 = vis_faligndata(s6, s7);
				vis_alignaddr((void *)(ref_block + 1), 0);
				s0 = vis_faligndata(s0, s1);
				s1 = vis_faligndata(s2, s3);
				s2 = vis_faligndata(s4, s5);
				s3 = vis_faligndata(s6, s7);

				MLIB_V_VIDEOINTERP(d0, d0, s0);
				MLIB_V_VIDEOINTERP(d1, d1, s1);
				MLIB_V_VIDEOINTERP(d2, d2, s2);
				MLIB_V_VIDEOINTERP4(d3, d3, s3);

				*dd = d0;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d2;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

				vis_alignaddr((void *)ref_block, 0);
			} while (--y);
		} else {
			do {
				s0 = sd[0];
				s1 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d0 = vis_faligndata(s0, s1);
				s2 = sd[0];
				s3 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d1 = vis_faligndata(s2, s3);
				s4 = sd[0];
				s5 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d2 = vis_faligndata(s4, s5);
				s6 = sd[0];
				s7 = sd[1];
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				d3 = vis_faligndata(s6, s7);

				MLIB_V_VIDEOINTERP4(d0, d0, s1);
				MLIB_V_VIDEOINTERP4(d1, d1, s3);
				MLIB_V_VIDEOINTERP4(d2, d2, s5);
				MLIB_V_VIDEOINTERP4(d3, d3, s7);

				*dd = d0;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d2;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				*dd = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

			} while (--y);
		}
	} else {
/* if(width==16) */

		y = height >> 1;

		if (((mlib_s32)(ref_block + 1) & 7)) {
			do {
				s0 = sd[0];
				s1 = sd[1];
				s2 = sd[2];
				d0 = vis_faligndata(s0, s1);
				d1 = vis_faligndata(s1, s2);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				s4 = sd[0];
				s5 = sd[1];
				s6 = sd[2];
				d2 = vis_faligndata(s4, s5);
				d3 = vis_faligndata(s5, s6);
				vis_alignaddr((void *)(ref_block + 1), 0);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				s0 = vis_faligndata(s0, s1);
				s1 = vis_faligndata(s1, s2);
				s2 = vis_faligndata(s4, s5);
				s3 = vis_faligndata(s5, s6);

				MLIB_V_VIDEOINTERP(d0, d0, s0);
				MLIB_V_VIDEOINTERP(d1, d1, s1);
				MLIB_V_VIDEOINTERP(d2, d2, s2);
				MLIB_V_VIDEOINTERP4(d3, d3, s3);

				dd[0] = d0;
				dd[1] = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				dd[0] = d2;
				dd[1] = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

				vis_alignaddr((void *)ref_block, 0);
			} while (--y);
		} else {
			do {
				s0 = sd[0];
				s1 = sd[1];
				s2 = sd[2];
				d0 = vis_faligndata(s0, s1);
				d1 = vis_faligndata(s1, s2);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
				s4 = sd[0];
				s5 = sd[1];
				s6 = sd[2];
				d2 = vis_faligndata(s4, s5);
				d3 = vis_faligndata(s5, s6);
				sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);

				MLIB_V_VIDEOINTERP4(d0, d0, s1);
				MLIB_V_VIDEOINTERP4(d1, d1, s2);
				MLIB_V_VIDEOINTERP4(d2, d2, s5);
				MLIB_V_VIDEOINTERP4(d3, d3, s6);

				dd[0] = d0;
				dd[1] = d1;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
				dd[0] = d2;
				dd[1] = d3;
				dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

			} while (--y);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, d0, d1, d2, d3, *sd, *dd;
	mlib_d64 dzero = vis_fzero();
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();
	mlib_s32 y;

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	y = 2;

	if (((mlib_s32)(ref_block + 1) & 7)) {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d0 = vis_faligndata(s0, s1);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d1 = vis_faligndata(s2, s3);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d2 = vis_faligndata(s4, s5);
			s6 = sd[0];
			s7 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d3 = vis_faligndata(s6, s7);
			vis_alignaddr((void *)(ref_block + 1), 0);
			s0 = vis_faligndata(s0, s1);
			s1 = vis_faligndata(s2, s3);
			s2 = vis_faligndata(s4, s5);
			s3 = vis_faligndata(s6, s7);

			MLIB_V_VIDEOINTERP(d0, d0, s0);
			MLIB_V_VIDEOINTERP(d1, d1, s1);
			MLIB_V_VIDEOINTERP(d2, d2, s2);
			MLIB_V_VIDEOINTERP4(d3, d3, s3);

			*dd = d0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

			vis_alignaddr((void *)ref_block, 0);
		} while (--y);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d0 = vis_faligndata(s0, s1);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d1 = vis_faligndata(s2, s3);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d2 = vis_faligndata(s4, s5);
			s6 = sd[0];
			s7 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d3 = vis_faligndata(s6, s7);

			MLIB_V_VIDEOINTERP4(d0, d0, s1);
			MLIB_V_VIDEOINTERP4(d1, d1, s3);
			MLIB_V_VIDEOINTERP4(d2, d2, s5);
			MLIB_V_VIDEOINTERP1(d3, d3, s7);

			*dd = d0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

		} while (--y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, d0, d1, d2, d3, *sd, *dd;
	mlib_d64 dzero = vis_fzero();
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();
	mlib_s32 y;

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	y = 4;

	if (((mlib_s32)(ref_block + 1) & 7)) {
		do {
			s0 = sd[0];
			s1 = sd[1];
			s2 = sd[2];
			d0 = vis_faligndata(s0, s1);
			d1 = vis_faligndata(s1, s2);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			s4 = sd[0];
			s5 = sd[1];
			s6 = sd[2];
			d2 = vis_faligndata(s4, s5);
			d3 = vis_faligndata(s5, s6);
			vis_alignaddr((void *)(ref_block + 1), 0);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			s0 = vis_faligndata(s0, s1);
			s1 = vis_faligndata(s1, s2);
			s2 = vis_faligndata(s4, s5);
			s3 = vis_faligndata(s5, s6);

			MLIB_V_VIDEOINTERP(d0, d0, s0);
			MLIB_V_VIDEOINTERP(d1, d1, s1);
			MLIB_V_VIDEOINTERP(d2, d2, s2);
			MLIB_V_VIDEOINTERP4(d3, d3, s3);

			dd[0] = d0;
			dd[1] = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			dd[0] = d2;
			dd[1] = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

			vis_alignaddr((void *)ref_block, 0);
		} while (--y);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			s2 = sd[2];
			d0 = vis_faligndata(s0, s1);
			d1 = vis_faligndata(s1, s2);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			s4 = sd[0];
			s5 = sd[1];
			s6 = sd[2];
			d2 = vis_faligndata(s4, s5);
			d3 = vis_faligndata(s5, s6);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);

			MLIB_V_VIDEOINTERP4(d0, d0, s1);
			MLIB_V_VIDEOINTERP4(d1, d1, s2);
			MLIB_V_VIDEOINTERP4(d2, d2, s5);
			MLIB_V_VIDEOINTERP4(d3, d3, s6);

			dd[0] = d0;
			dd[1] = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			dd[0] = d2;
			dd[1] = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

		} while (--y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, d0, d1, d2, d3, *sd, *dd;
	mlib_d64 dzero = vis_fzero();
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();
	mlib_s32 y;

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	y = 4;

	if (((mlib_s32)(ref_block + 1) & 7)) {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d0 = vis_faligndata(s0, s1);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d1 = vis_faligndata(s2, s3);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d2 = vis_faligndata(s4, s5);
			s6 = sd[0];
			s7 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d3 = vis_faligndata(s6, s7);
			vis_alignaddr((void *)(ref_block + 1), 0);
			s0 = vis_faligndata(s0, s1);
			s1 = vis_faligndata(s2, s3);
			s2 = vis_faligndata(s4, s5);
			s3 = vis_faligndata(s6, s7);

			MLIB_V_VIDEOINTERP(d0, d0, s0);
			MLIB_V_VIDEOINTERP(d1, d1, s1);
			MLIB_V_VIDEOINTERP(d2, d2, s2);
			MLIB_V_VIDEOINTERP4(d3, d3, s3);

			*dd = d0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

			vis_alignaddr((void *)ref_block, 0);
		} while (--y);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d0 = vis_faligndata(s0, s1);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d1 = vis_faligndata(s2, s3);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d2 = vis_faligndata(s4, s5);
			s6 = sd[0];
			s7 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d3 = vis_faligndata(s6, s7);

			MLIB_V_VIDEOINTERP4(d0, d0, s1);
			MLIB_V_VIDEOINTERP4(d1, d1, s3);
			MLIB_V_VIDEOINTERP4(d2, d2, s5);
			MLIB_V_VIDEOINTERP4(d3, d3, s7);

			*dd = d0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

		} while (--y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, d0, d1, d2, d3, *sd, *dd;
	mlib_d64 dzero = vis_fzero();
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();
	mlib_s32 y;

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	y = 8;

	if (((mlib_s32)(ref_block + 1) & 7)) {
		do {
			s0 = sd[0];
			s1 = sd[1];
			s2 = sd[2];
			d0 = vis_faligndata(s0, s1);
			d1 = vis_faligndata(s1, s2);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			s4 = sd[0];
			s5 = sd[1];
			s6 = sd[2];
			d2 = vis_faligndata(s4, s5);
			d3 = vis_faligndata(s5, s6);
			vis_alignaddr((void *)(ref_block + 1), 0);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			s0 = vis_faligndata(s0, s1);
			s1 = vis_faligndata(s1, s2);
			s2 = vis_faligndata(s4, s5);
			s3 = vis_faligndata(s5, s6);

			MLIB_V_VIDEOINTERP(d0, d0, s0);
			MLIB_V_VIDEOINTERP(d1, d1, s1);
			MLIB_V_VIDEOINTERP(d2, d2, s2);
			MLIB_V_VIDEOINTERP4(d3, d3, s3);

			dd[0] = d0;
			dd[1] = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			dd[0] = d2;
			dd[1] = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

			vis_alignaddr((void *)ref_block, 0);
		} while (--y);
	} else {
		do {
			s0 = sd[0];
			s1 = sd[1];
			s2 = sd[2];
			d0 = vis_faligndata(s0, s1);
			d1 = vis_faligndata(s1, s2);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			s4 = sd[0];
			s5 = sd[1];
			s6 = sd[2];
			d2 = vis_faligndata(s4, s5);
			d3 = vis_faligndata(s5, s6);
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);

			MLIB_V_VIDEOINTERP4(d0, d0, s1);
			MLIB_V_VIDEOINTERP4(d1, d1, s2);
			MLIB_V_VIDEOINTERP4(d2, d2, s5);
			MLIB_V_VIDEOINTERP4(d3, d3, s6);

			dd[0] = d0;
			dd[1] = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			dd[0] = d2;
			dd[1] = d3;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);

		} while (--y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7, d0, d1, d2, d3, *sd, *dd;
	mlib_d64 dzero = vis_fzero();
	const mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_d64 rounder = vis_fone();

	vis_write_gsr((6 << 3) + ((mlib_u32)ref_block & 7));
	dd = (mlib_d64 *)curr_block;
	sd = (mlib_d64 *)((mlib_addr)ref_block & ~7);

	if (((mlib_s32)(ref_block + 1) & 7)) {
		{
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d0 = vis_faligndata(s0, s1);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d1 = vis_faligndata(s2, s3);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d2 = vis_faligndata(s4, s5);
			s6 = sd[0];
			s7 = sd[1];
			d3 = vis_faligndata(s6, s7);
			vis_alignaddr((void *)(ref_block + 1), 0);
			s0 = vis_faligndata(s0, s1);
			s1 = vis_faligndata(s2, s3);
			s2 = vis_faligndata(s4, s5);
			s3 = vis_faligndata(s6, s7);

			MLIB_V_VIDEOINTERP(d0, d0, s0);
			MLIB_V_VIDEOINTERP(d1, d1, s1);
			MLIB_V_VIDEOINTERP(d2, d2, s2);
			MLIB_V_VIDEOINTERP4(d3, d3, s3);

			*dd = d0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d3;
		}
	} else {
		{
			s0 = sd[0];
			s1 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d0 = vis_faligndata(s0, s1);
			s2 = sd[0];
			s3 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d1 = vis_faligndata(s2, s3);
			s4 = sd[0];
			s5 = sd[1];
			sd = (mlib_d64 *)((mlib_u8 *)sd + field_stride);
			d2 = vis_faligndata(s4, s5);
			s6 = sd[0];
			s7 = sd[1];
			d3 = vis_faligndata(s6, s7);

			MLIB_V_VIDEOINTERP4(d0, d0, s1);
			MLIB_V_VIDEOINTERP4(d1, d1, s3);
			MLIB_V_VIDEOINTERP4(d2, d2, s5);
			MLIB_V_VIDEOINTERP4(d3, d3, s7);

			*dd = d0;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d1;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d2;
			dd = (mlib_d64 *)((mlib_u8 *)dd + field_stride);
			*dd = d3;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
