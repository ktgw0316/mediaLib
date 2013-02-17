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

#pragma ident	"@(#)mlib_v_ImageCopyArea_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageCopyArea_a2_r   - 2-D, Aligned8, width 8x, reversed
 *      mlib_ImageCopyArea_na_r     - BYTE, non-aligned, reversed
 *      mlib_ImageCopyArea_bit_al_r - BIT, aligned, reversed
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      sp        pointer to source data
 *      dp        pointer to destination data
 *      size      size in 8-bytes, bytes, or SHORTs
 *      width     image width in 8-bytes
 *      height    image height in lines
 *      ystride   image line stride in 8-bytes
 *      s_offset  source image line bit offset
 *      d_offset  destination image line bit offset
 *
 * DESCRIPTION
 *      Copy a specified rectangular area from one portion of the
 *      image to another (within the same image) --
 *      low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageCopyArea.c for
 *      loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageCopyArea_f.h>

/* *********************************************************** */

/*
 *      Both source and destination data are 8-byte aligned.
 *      And ystride and width are in 8-bytes.
 *
 *      Data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_v_ImageCopyArea_a2_r(
    mlib_d64 *sp,
    mlib_d64 *dp,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 ystride)
{
/* 8-byte aligned pointer for line */
	mlib_d64 *spl;

/* 8-byte aligned pointer for line */
	mlib_d64 *dpl;

/* indices for x, y */
	mlib_s32 i, j;

	spl = sp;
	dpl = dp;

/* row loop */
	for (j = 0; j < height; j++) {
/* 8-byte column loop */
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			*dp-- = *sp--;
		}

		sp = spl -= ystride;
		dp = dpl -= ystride;
	}
}

/* *********************************************************** */

/*
 *      Either source or destination data are not 8-byte aligned.
 *      And size is is in bytes.
 *
 *      Data are copied beginning from the end and proceeding
 *      to the star of the area.
 */

void
mlib_ImageCopyArea_na_r(
    mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size)
{
/* end points in dst */
	mlib_u8 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 s0, s1;

/* 8-byte source data */
	mlib_d64 sd;

/* offset of address */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 n;

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	dend = da - size + 1;
	dend2 = dend - 7;

/* prepare the source address */
	sp = (mlib_d64 *)((mlib_addr)sa & (~7));

/* calculate the address offset of src over dst, set GSR.offset */
	off = ((mlib_addr)sa & 7) - ((mlib_addr)da & 7);
	vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dend, da);
	} else
		emask = vis_edge8(dp, da);

/* load and store 8 bytes */
	s0 = vis_ld_d64_nf(sp);
	sp--;

	if (off < 0) {
		s1 = vis_ld_d64_nf(sp);
		sp--;
	} else {
		s1 = s0;
	}

	sd = vis_faligndata(s1, s0);
	vis_pst_8(sd, dp--, emask);

/* all 8 bytes of data will be saved in the loop */
	n = ((mlib_u8 *)dp - (mlib_u8 *)dend) >> 3;

#pragma pipeloop(0)
	for (; n >= 0; n--) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp);
		sp--;
		*dp-- = vis_faligndata(s1, s0);
	}

/* handle the end point */

	if ((mlib_addr)dp >= (mlib_addr)dend2) {
		sd = vis_faligndata(vis_ld_d64_nf(sp), s1);
		emask = vis_edge8(dend, (dp + 1));
		vis_pst_8(sd, dp, emask);
	}
}

/* *********************************************************** */

/*
 * Both bit offsets of source and destination are the same
 */

void
mlib_ImageCopyArea_bit_al_r(
    mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset)
{
/* end points in dst */
	mlib_u8 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 s0, s1;

/* 8-byte source data */
	mlib_d64 sd;

/* offset of address in dst */
	mlib_s32 j;

/* edge mask */
	mlib_s32 emask;

/* edge mask */
	mlib_s32 b_size;
	mlib_u8 mask0 = 0xFF;
	mlib_u8 src, mask;
	mlib_s32 n, off;

	if (size <= 0)
		return;

	if (size <= offset) {
		mask = mask0 << (8 - size);
		mask >>= (offset - size);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
		return;
	}

	mask = mask0 << (8 - offset);
	src = da[0];
	da[0] = (src & (~mask)) | (sa[0] & mask);
	da--;
	sa--;
	size = size - offset;
	b_size = size >> 3;
	j = size & 7;

	if (b_size == 0) {
		mask = mask0 >> (8 - j);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
		return;
	}

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	dend = da - b_size + 1;
	dend2 = dend - 7;

/* prepare the source address */
	sp = (mlib_d64 *)((mlib_addr)sa & (~7));

/* calculate the address offset of src over dst, set GSR.offset */
	off = ((mlib_addr)sa & 7) - ((mlib_addr)da & 7);
	vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dend, da);
	} else
		emask = vis_edge8(dp, da);

/* load and store 8 bytes */
	s0 = vis_ld_d64_nf(sp);
	sp--;

	if (off < 0) {
		s1 = vis_ld_d64_nf(sp);
		sp--;
	} else {
		s1 = s0;
	}

	sd = vis_faligndata(s1, s0);
	vis_pst_8(sd, dp--, emask);

/* all 8 bytes of data will be saved in the loop */
	n = ((mlib_u8 *)dp - (mlib_u8 *)dend) >> 3;

#pragma pipeloop(0)
	for (; n >= 0; n--) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp);
		sp--;
		*dp-- = vis_faligndata(s1, s0);
	}

/* handle the end point */

	if ((mlib_addr)dp >= (mlib_addr)dend2) {
		sd = vis_faligndata(vis_ld_d64_nf(sp), s1);
		emask = vis_edge8(dend, (dp + 1));
		vis_pst_8(sd, dp, emask);
	}

	if (j > 0) {
		mask = mask0 >> (8 - j);
		src = dend[-1];
		dend[-1] = (src & (~mask)) | (sa[-b_size] & mask);
	}
}

/* *********************************************************** */
