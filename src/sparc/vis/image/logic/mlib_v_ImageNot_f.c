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

#pragma ident	"@(#)mlib_v_ImageNot_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageNot_na
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      sa       pointer to source image data
 *      da       pointer to destination image data
 *      size     size in 8-bytes
 *
 * DESCRIPTION
 *      VIS Low level functions to NOT an image for each channel:
 *      *da = ~(*sa)
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageNot.c for loop
 *      unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageLogic_proto.h>

/* *********************************************************** */

void
mlib_v_ImageNot_na(
    mlib_u8 *sa,
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
	sp = (mlib_d64 *)vis_alignaddr(sa, j);
/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

	s1 = vis_ld_d64_nf(sp);

	if (emask != 0xff) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		s0 = vis_faligndata(s0, s1);
		vis_pst_8(vis_fnot(s0), dp++, emask);
		sp++;
		j += 8;
	}
#pragma pipeloop(0)
	for (; j <= (size - 8); j += 8) {
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		(*dp++) = vis_fnot(vis_faligndata(s0, s1));
		sp++;
	}

	if (j < size) {
		s0 = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
		emask = vis_edge8(dp, dend);
		vis_pst_8(vis_fnot(s0), dp, emask);
	}
}

/* *********************************************************** */
