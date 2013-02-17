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

#pragma ident	"@(#)mlib_v_ImageChannelSplit_FS32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_FS32
 *      mlib_ImageChannelSplit3_FS32
 *      mlib_ImageChannelSplit4_FS32
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, n = width * 2;

	dst0_str >>= 2;
	dst1_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

		mlib_f32 *sp = (mlib_f32 *)src_s32_0;
		mlib_f32 *dp0 = (mlib_f32 *)dst_s32_0;
		mlib_f32 *dp1 = (mlib_f32 *)dst_s32_1;

		i = 0;

		if (((mlib_addr)dp0 & 7) == ((mlib_addr)dp1 & 7)) {

			if ((mlib_addr)dp0 & 7) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				i += 2;
			}

			if ((mlib_addr)sp & 7) {

#pragma pipeloop(0)
				for (; i < (n - 3); i += 4) {
					mlib_f32 s0 = sp[0];
					mlib_d64 sd0 =
					    ((mlib_d64 *)(sp + 1))[0];
					mlib_f32 s1 = sp[3];

					((mlib_d64 *)dp0)[0] =
					    vis_freg_pair(s0, vis_read_lo(sd0));
					((mlib_d64 *)dp1)[0] =
					    vis_freg_pair(vis_read_hi(sd0), s1);

					sp += 4;
					dp0 += 2;
					dp1 += 2;
				}
			} else {

#pragma pipeloop(0)
				for (; i < (n - 3); i += 4) {

					mlib_d64 sd0 = ((mlib_d64 *)sp)[0];
					mlib_d64 sd1 = ((mlib_d64 *)sp)[1];

					((mlib_d64 *)dp0)[0] =
					    vis_freg_pair(vis_read_hi(sd0),
					    vis_read_hi(sd1));
					((mlib_d64 *)dp1)[0] =
					    vis_freg_pair(vis_read_lo(sd0),
					    vis_read_lo(sd1));

					sp += 4;
					dp0 += 2;
					dp1 += 2;
				}
			}

			if (i < (n - 1)) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				i += 2;
			}
		} else {

#pragma pipeloop(0)
			for (; i < (n - 1); i += 2) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
			}
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		src_s32_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    mlib_s32 *dst_s32_2,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, n = width * 3;

	dst0_str >>= 2;
	dst1_str >>= 2;
	dst2_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

		mlib_f32 *sp = (mlib_f32 *)src_s32_0;
		mlib_f32 *dp0 = (mlib_f32 *)dst_s32_0;
		mlib_f32 *dp1 = (mlib_f32 *)dst_s32_1;
		mlib_f32 *dp2 = (mlib_f32 *)dst_s32_2;

		i = 0;

		if ((((mlib_addr)dp0 & 7) == ((mlib_addr)dp1 & 7)) &&
		    (((mlib_addr)dp0 & 7) == ((mlib_addr)dp2 & 7))) {

			if ((mlib_addr)dp0 & 7) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				(*dp2++) = (*sp++);
				i += 3;
			}

			if ((mlib_addr)sp & 7) {

#pragma pipeloop(0)
				for (; i < (n - 5); i += 6) {
					mlib_f32 s0 = sp[0];
					mlib_d64 sd0 =
					    ((mlib_d64 *)(sp + 1))[0];
					mlib_d64 sd1 =
					    ((mlib_d64 *)(sp + 3))[0];
					mlib_f32 s1 = sp[5];

					((mlib_d64 *)dp0)[0] =
					    vis_freg_pair(s0, vis_read_hi(sd1));
					((mlib_d64 *)dp1)[0] =
					    vis_freg_pair(vis_read_hi(sd0),
					    vis_read_lo(sd1));
					((mlib_d64 *)dp2)[0] =
					    vis_freg_pair(vis_read_lo(sd0), s1);

					sp += 6;
					dp0 += 2;
					dp1 += 2;
					dp2 += 2;
				}
			} else {

#pragma pipeloop(0)
				for (; i < (n - 5); i += 6) {

					mlib_d64 sd0 = ((mlib_d64 *)sp)[0];
					mlib_d64 sd1 = ((mlib_d64 *)sp)[1];
					mlib_d64 sd2 = ((mlib_d64 *)sp)[2];

					((mlib_d64 *)dp0)[0] =
					    vis_freg_pair(vis_read_hi(sd0),
					    vis_read_lo(sd1));
					((mlib_d64 *)dp1)[0] =
					    vis_freg_pair(vis_read_lo(sd0),
					    vis_read_hi(sd2));
					((mlib_d64 *)dp2)[0] =
					    vis_freg_pair(vis_read_hi(sd1),
					    vis_read_lo(sd2));

					sp += 6;
					dp0 += 2;
					dp1 += 2;
					dp2 += 2;
				}
			}

			if (i < (n - 1)) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				(*dp2++) = (*sp++);
				i += 3;
			}
		} else {

#pragma pipeloop(0)
			for (; i < (n - 1); i += 3) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				(*dp2++) = (*sp++);
			}
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		dst_s32_2 += dst2_str;
		src_s32_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    mlib_s32 *dst_s32_2,
    mlib_s32 *dst_s32_3,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 dst3_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, n = width * 4;

	dst0_str >>= 2;
	dst1_str >>= 2;
	dst2_str >>= 2;
	dst3_str >>= 2;
	src0_str >>= 2;

	for (j = 0; j < height; j++) {

		mlib_f32 *sp = (mlib_f32 *)src_s32_0;
		mlib_f32 *dp0 = (mlib_f32 *)dst_s32_0;
		mlib_f32 *dp1 = (mlib_f32 *)dst_s32_1;
		mlib_f32 *dp2 = (mlib_f32 *)dst_s32_2;
		mlib_f32 *dp3 = (mlib_f32 *)dst_s32_3;

		i = 0;

		if ((((mlib_addr)dp0 & 7) == ((mlib_addr)dp1 & 7)) &&
		    (((mlib_addr)dp0 & 7) == ((mlib_addr)dp2 & 7)) &&
		    (((mlib_addr)dp0 & 7) == ((mlib_addr)dp3 & 7))) {

			if ((mlib_addr)dp0 & 7) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				(*dp2++) = (*sp++);
				(*dp3++) = (*sp++);
				i += 4;
			}

			if ((mlib_addr)sp & 7) {

#pragma pipeloop(0)
				for (; i < (n - 7); i += 8) {
					mlib_f32 s0 = sp[0];
					mlib_d64 sd0 =
					    ((mlib_d64 *)(sp + 1))[0];
					mlib_d64 sd1 =
					    ((mlib_d64 *)(sp + 3))[0];
					mlib_d64 sd2 =
					    ((mlib_d64 *)(sp + 5))[0];
					mlib_f32 s1 = sp[7];

					((mlib_d64 *)dp0)[0] =
					    vis_freg_pair(s0, vis_read_lo(sd1));
					((mlib_d64 *)dp1)[0] =
					    vis_freg_pair(vis_read_hi(sd0),
					    vis_read_hi(sd2));
					((mlib_d64 *)dp2)[0] =
					    vis_freg_pair(vis_read_lo(sd0),
					    vis_read_lo(sd2));
					((mlib_d64 *)dp3)[0] =
					    vis_freg_pair(vis_read_hi(sd1), s1);

					sp += 8;
					dp0 += 2;
					dp1 += 2;
					dp2 += 2;
					dp3 += 2;
				}
			} else {

#pragma pipeloop(0)
				for (; i < (n - 7); i += 8) {

					mlib_d64 sd0 = ((mlib_d64 *)sp)[0];
					mlib_d64 sd1 = ((mlib_d64 *)sp)[1];
					mlib_d64 sd2 = ((mlib_d64 *)sp)[2];
					mlib_d64 sd3 = ((mlib_d64 *)sp)[3];

					((mlib_d64 *)dp0)[0] =
					    vis_freg_pair(vis_read_hi(sd0),
					    vis_read_hi(sd2));
					((mlib_d64 *)dp1)[0] =
					    vis_freg_pair(vis_read_lo(sd0),
					    vis_read_lo(sd2));
					((mlib_d64 *)dp2)[0] =
					    vis_freg_pair(vis_read_hi(sd1),
					    vis_read_hi(sd3));
					((mlib_d64 *)dp3)[0] =
					    vis_freg_pair(vis_read_lo(sd1),
					    vis_read_lo(sd3));

					sp += 8;
					dp0 += 2;
					dp1 += 2;
					dp2 += 2;
					dp3 += 2;
				}
			}

			if (i < (n - 1)) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				(*dp2++) = (*sp++);
				(*dp3++) = (*sp++);
				i += 4;
			}
		} else {

#pragma pipeloop(0)
			for (; i < (n - 1); i += 4) {
				(*dp0++) = (*sp++);
				(*dp1++) = (*sp++);
				(*dp2++) = (*sp++);
				(*dp3++) = (*sp++);
			}
		}

		dst_s32_0 += dst0_str;
		dst_s32_1 += dst1_str;
		dst_s32_2 += dst2_str;
		dst_s32_3 += dst3_str;
		src_s32_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
