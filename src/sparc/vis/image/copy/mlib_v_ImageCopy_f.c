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

#pragma ident	"@(#)mlib_v_ImageCopy_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageCopy_a1         - 1-D, Aligned8, size 8x
 *      mlib_v_ImageCopy_a2         - 2-D, Aligned8, width 8x
 *      mlib_ImageCopy_na           - BYTE, non-aligned
 *      mlib_ImageCopy_bit_al       - BIT, aligned
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      sp       pointer to source image data
 *      dp       pointer to destination image data
 *      size     size in 8-bytes, bytes, or SHORTs
 *      width    image width in 8-bytes
 *      height   image height in lines
 *      stride   source image line stride in 8-bytes
 *      dstride  destination image line stride in 8-bytes
 *      s_offset source image line bit offset
 *      d_offset destination image line bit offset
 *
 * DESCRIPTION
 *      Direct copy from one image to another -- VIS version low level
 *      functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageCopy.c for loop
 *      unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageCopy_f.h>

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), Y)

/* *********************************************************** */

/*
 * Both source and destination image data are 1-d vectors and
 * 8-byte aligned. And size is in 8-bytes.
 */

void
mlib_v_ImageCopy_a1(
    mlib_d64 *sp,
    mlib_d64 *dp,
    mlib_s32 size)
{
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

/*
 * Either source or destination image data are not 1-d vectors, but
 * they are 8-byte aligned. And stride and width are in 8-bytes.
 */

void
mlib_v_ImageCopy_a2(
    mlib_d64 *sp,
    mlib_d64 *dp,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 dstride)
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
			(*dp++) = (*sp++);
		}

		sp = spl += stride;
		dp = dpl += dstride;
	}
}

/* *********************************************************** */

/*
 * Both bit offsets of source and destination are the same
 */

void
mlib_ImageCopy_bit_al(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset)
{
/* end points in dst */
	mlib_u8 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 s0, s1;

/* offset of address in dst */
	mlib_s32 j;

/* edge mask */
	mlib_s32 emask;

/* size in bytes */
	mlib_s32 b_size;
	mlib_u8 mask0 = 0xFF;
	mlib_u8 src, mask;

	if (size <= 0)
		return;

	if (size <= (8 - offset)) {
		mask = mask0 << (8 - size);
		mask >>= offset;
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
		return;
	}

	mask = mask0 >> offset;
	src = da[0];
	da[0] = (src & (~mask)) | (sa[0] & mask);
	da++;
	sa++;
	size = size - 8 + offset;
/* size in bytes */
	b_size = size >> 3;

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	j = (mlib_addr)dp - (mlib_addr)da;
	dend = da + b_size - 1;

/* prepare the source address */
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, j);
/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

	s1 = vis_ld_d64_nf(sp);

	if (emask != 0xff) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		s0 = vis_faligndata(s0, s1);
		vis_pst_8(s0, dp++, emask);
		sp++;
		j += 8;
	}
#pragma pipeloop(0)
	for (; j <= (b_size - 8); j += 8) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		(*dp++) = vis_faligndata(s0, s1);
		sp++;
	}

	if (j < b_size) {
		s0 = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
		emask = vis_edge8(dp, dend);
		vis_pst_8(s0, dp, emask);
	}

	j = size & 7;

	if (j > 0) {
		mask = mask0 << (8 - j);
		src = dend[1];
		dend[1] = (src & (~mask)) | (sa[b_size] & mask);
	}
}

/* *********************************************************** */

/*
 * Either source or destination data are not 8-byte aligned.
 * And size is is in bytes.
 */

void
mlib_ImageCopy_na(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size)
{
/* end points in dst */
	mlib_u8 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 s0, s1;

/* offset of address in dst */
	mlib_s32 j;

/* edge mask */
	mlib_s32 emask;

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	j = (mlib_addr)dp - (mlib_addr)da;
	dend = da + size - 1;

/* prepare the source address */
	sp = (mlib_d64 *)VIS_ALIGNADDR(sa, j);
/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

	s1 = vis_ld_d64_nf(sp);

	if (emask != 0xff) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		s0 = vis_faligndata(s0, s1);
		vis_pst_8(s0, dp++, emask);
		sp++;
		j += 8;
	}

	if (((mlib_addr)sa ^ (mlib_addr)da) & 7) {
#pragma pipeloop(0)
		for (; j <= (size - 8); j += 8) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			(*dp++) = vis_faligndata(s0, s1);
			sp++;
		}

		if (j < size) {
			s0 = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
			emask = vis_edge8(dp, dend);
			vis_pst_8(s0, dp, emask);
		}
	} else {
#pragma pipeloop(0)
		for (; j <= (size - 8); j += 8) {
			(*dp++) = (*sp++);
		}

		if (j < size) {
			emask = vis_edge8(dp, dend);
			vis_pst_8(vis_ld_d64_nf(sp), dp, emask);
		}
	}
}

/* *********************************************************** */
