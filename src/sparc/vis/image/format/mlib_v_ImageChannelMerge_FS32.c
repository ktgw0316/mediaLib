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

#pragma ident	"@(#)mlib_v_ImageChannelMerge_FS32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_FS32
 *      mlib_ImageChannelMerge3_FS32
 *      mlib_ImageChannelMerge4_FS32
 *
 * DESCRIPTION
 *      Internal functions.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge2_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	mlib_s32 i, j, n = width << 1;
	mlib_d64 *dp, s0, s1;
	mlib_s32 *f_ptr, *s_ptr;

	for (j = 0; j < height; j++) {
		i = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[0] = src_s32_0[0];
			f_ptr = (mlib_s32 *)(src_s32_1);
			s_ptr = (mlib_s32 *)(src_s32_0 + 1);
			i++;
		} else {
			f_ptr = (mlib_s32 *)src_s32_0;
			s_ptr = (mlib_s32 *)src_s32_1;
		}

		if (((mlib_addr)s_ptr & 7) && ((mlib_addr)f_ptr & 7) &&
		    (i < (n - 1))) {
			dst_s32_0[i + 0] = (*f_ptr++);
			dst_s32_0[i + 1] = (*s_ptr++);
			i += 2;
		}

		dp = (mlib_d64 *)(dst_s32_0 + i);

		if (((mlib_addr)s_ptr & 7) == ((mlib_addr)f_ptr & 7)) {

#pragma pipeloop(0)
			for (; i < (n - 3); i += 4) {
				s0 = *(mlib_d64 *)f_ptr;
				s1 = *(mlib_d64 *)s_ptr;
				dp[0] =
				    vis_freg_pair(vis_read_hi(s0),
				    vis_read_hi(s1));
				dp[1] =
				    vis_freg_pair(vis_read_lo(s0),
				    vis_read_lo(s1));
				f_ptr += 2;
				s_ptr += 2;
				dp += 2;
			}
		} else if ((mlib_addr)f_ptr & 7) {

#pragma pipeloop(0)
			for (; i < (n - 3); i += 4) {
				mlib_f32 s0_0 = *(mlib_f32 *)(f_ptr++);
				mlib_f32 s0_1 = *(mlib_f32 *)(f_ptr++);

				s1 = *(mlib_d64 *)s_ptr;
				dp[0] = vis_freg_pair(s0_0, vis_read_hi(s1));
				dp[1] = vis_freg_pair(s0_1, vis_read_lo(s1));
				s_ptr += 2;
				dp += 2;
			}
		} else {

#pragma pipeloop(0)
			for (; i < (n - 3); i += 4) {
				mlib_f32 s1_0 = *(mlib_f32 *)(s_ptr++);
				mlib_f32 s1_1 = *(mlib_f32 *)(s_ptr++);

				s0 = *(mlib_d64 *)f_ptr;
				dp[0] = vis_freg_pair(vis_read_hi(s0), s1_0);
				dp[1] = vis_freg_pair(vis_read_lo(s0), s1_1);
				f_ptr += 2;
				dp += 2;
			}
		}

		if (i < (n - 1)) {
			dp[0] =
			    vis_freg_pair(*((mlib_f32 *)f_ptr),
			    *((mlib_f32 *)s_ptr));
			i += 2;
			f_ptr++;
			s_ptr++;
		}

		if (i < n)
			dst_s32_0[i] = *f_ptr;

		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		dst_s32_0 += dst_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge3_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    const mlib_s32 *src_s32_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride)
{
	mlib_s32 i, j, n = width * 3;
	mlib_s32 *f_ptr, *s_ptr, *t_ptr;
	mlib_d64 *dp, s0, s1, s2;

	for (j = 0; j < height; j++) {

		i = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[i] = src_s32_0[0];
			f_ptr = (mlib_s32 *)src_s32_1;
			s_ptr = (mlib_s32 *)src_s32_2;
			t_ptr = (mlib_s32 *)(src_s32_0 + 1);
			i++;
		} else {
			f_ptr = (mlib_s32 *)src_s32_0;
			s_ptr = (mlib_s32 *)src_s32_1;
			t_ptr = (mlib_s32 *)src_s32_2;
		}

		dp = (mlib_d64 *)(dst_s32_0 + i);

		if (n > 5) {
			if (((mlib_addr)f_ptr & 7) || ((mlib_addr)s_ptr & 7) ||
			    ((mlib_addr)t_ptr & 7)) {

#pragma pipeloop(0)
				for (; i < (n - 5); i += 6) {
					mlib_f32 s0_0 = *(mlib_f32 *)f_ptr++;
					mlib_f32 s0_1 = *(mlib_f32 *)f_ptr++;
					mlib_f32 s1_0 = *(mlib_f32 *)s_ptr++;
					mlib_f32 s1_1 = *(mlib_f32 *)s_ptr++;
					mlib_f32 s2_0 = *(mlib_f32 *)t_ptr++;
					mlib_f32 s2_1 = *(mlib_f32 *)t_ptr++;

					dp[0] = vis_freg_pair(s0_0, s1_0);
					dp[1] = vis_freg_pair(s2_0, s0_1);
					dp[2] = vis_freg_pair(s1_1, s2_1);
					dp += 3;
				}
			} else {

#pragma pipeloop(0)
				for (; i < (n - 5); i += 6) {
					s0 = *(mlib_d64 *)f_ptr;
					s1 = *(mlib_d64 *)s_ptr;
					s2 = *(mlib_d64 *)t_ptr;
					dp[0] =
					    vis_freg_pair(vis_read_hi(s0),
					    vis_read_hi(s1));
					dp[1] =
					    vis_freg_pair(vis_read_hi(s2),
					    vis_read_lo(s0));
					dp[2] =
					    vis_freg_pair(vis_read_lo(s1),
					    vis_read_lo(s2));
					f_ptr += 2;
					s_ptr += 2;
					t_ptr += 2;
					dp += 3;
				}
			}
		}

		if (i < (n - 2)) {
			dst_s32_0[i + 0] = (*f_ptr++);
			dst_s32_0[i + 1] = (*s_ptr++);
			dst_s32_0[i + 2] = (*t_ptr++);
			i += 3;
		}

		if (i < n) {
			dst_s32_0[i + 0] = *f_ptr;
			dst_s32_0[i + 1] = *s_ptr;
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    const mlib_s32 *src_s32_2,
    const mlib_s32 *src_s32_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride)
{
	mlib_s32 i, j, n = width << 2;
	mlib_s32 *fi_ptr, *se_ptr, *th_ptr, *fo_ptr;
	mlib_d64 *dp, s0, s1, s2, s3;

	for (j = 0; j < height; j++) {

		i = 0;

		if ((mlib_addr)dst_s32_0 & 7) {
			dst_s32_0[i] = src_s32_0[0];
			fi_ptr = (mlib_s32 *)src_s32_1;
			se_ptr = (mlib_s32 *)src_s32_2;
			th_ptr = (mlib_s32 *)src_s32_3;
			fo_ptr = (mlib_s32 *)(src_s32_0 + 1);
			i++;
		} else {
			fi_ptr = (mlib_s32 *)src_s32_0;
			se_ptr = (mlib_s32 *)src_s32_1;
			th_ptr = (mlib_s32 *)src_s32_2;
			fo_ptr = (mlib_s32 *)src_s32_3;
		}

		dp = (mlib_d64 *)(dst_s32_0 + i);

		if (n > 7) {
			if (((mlib_addr)fi_ptr & 7) ||
			    ((mlib_addr)se_ptr & 7) ||
			    ((mlib_addr)th_ptr & 7) ||
			    ((mlib_addr)fo_ptr & 7)) {

#pragma pipeloop(0)
				for (; i < (n - 7); i += 8) {
					mlib_f32 s0_0 = *(mlib_f32 *)fi_ptr++;
					mlib_f32 s0_1 = *(mlib_f32 *)fi_ptr++;
					mlib_f32 s1_0 = *(mlib_f32 *)se_ptr++;
					mlib_f32 s1_1 = *(mlib_f32 *)se_ptr++;
					mlib_f32 s2_0 = *(mlib_f32 *)th_ptr++;
					mlib_f32 s2_1 = *(mlib_f32 *)th_ptr++;
					mlib_f32 s3_0 = *(mlib_f32 *)fo_ptr++;
					mlib_f32 s3_1 = *(mlib_f32 *)fo_ptr++;

					dp[0] = vis_freg_pair(s0_0, s1_0);
					dp[1] = vis_freg_pair(s2_0, s3_0);
					dp[2] = vis_freg_pair(s0_1, s1_1);
					dp[3] = vis_freg_pair(s2_1, s3_1);
					dp += 4;
				}
			} else {

#pragma pipeloop(0)
				for (; i < (n - 7); i += 8) {
					s0 = *(mlib_d64 *)fi_ptr;
					s1 = *(mlib_d64 *)se_ptr;
					s2 = *(mlib_d64 *)th_ptr;
					s3 = *(mlib_d64 *)fo_ptr;
					dp[0] =
					    vis_freg_pair(vis_read_hi(s0),
					    vis_read_hi(s1));
					dp[1] =
					    vis_freg_pair(vis_read_hi(s2),
					    vis_read_hi(s3));
					dp[2] =
					    vis_freg_pair(vis_read_lo(s0),
					    vis_read_lo(s1));
					dp[3] =
					    vis_freg_pair(vis_read_lo(s2),
					    vis_read_lo(s3));
					fi_ptr += 2;
					se_ptr += 2;
					th_ptr += 2;
					fo_ptr += 2;
					dp += 4;
				}
			}
		}

		if (i < (n - 3)) {
			dst_s32_0[i + 0] = (*fi_ptr++);
			dst_s32_0[i + 1] = (*se_ptr++);
			dst_s32_0[i + 2] = (*th_ptr++);
			dst_s32_0[i + 3] = (*fo_ptr++);
			i += 4;
		}

		if (i < n) {
			dst_s32_0[i + 0] = *fi_ptr;
			dst_s32_0[i + 1] = *se_ptr;
			dst_s32_0[i + 2] = *th_ptr;
		}

		dst_s32_0 += dst_stride;
		src_s32_0 += src0_stride;
		src_s32_1 += src1_stride;
		src_s32_2 += src2_stride;
		src_s32_3 += src3_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
