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

#pragma ident	"@(#)mlib_v_ImageChannelMerge_U8_24.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_U8
 *      mlib_ImageChannelMerge4_U8
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge2_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	mlib_s32 i, j, k, l, n = width * 2;
	mlib_u16 *dsh;

	for (j = 0, i = 0; j < height; j++, i = 0) {
		mlib_u8 *ptr0 = (void *)src_u8_0;
		mlib_u8 *ptr1 = (void *)src_u8_1;

		k = 0;

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i] = src_u8_0[0];
			ptr0 = (void *)src_u8_1;
			ptr1 = (void *)(src_u8_0 + 1);
			i++;
		}

		dsh = (mlib_u16 *)(dst_u8_0 + i);

#pragma pipeloop(0)
		for (; (i < (n - 1)) && ((mlib_addr)dsh & 7); i += 2) {
			mlib_u16 a = (mlib_u16)((*ptr0++));
			mlib_u16 b = (mlib_u16)((*ptr1++));

			(*dsh++) = (a << 8) | b;
		}

		if ((!((mlib_addr)ptr0 & 7)) && (!((mlib_addr)ptr1 & 7))) {
			mlib_d64 *srd0, *srd1;
			mlib_d64 *dstd;
			mlib_d64 sd0_m, sd1_m;

			dstd = (mlib_d64 *)dsh;
			srd0 = (mlib_d64 *)ptr0;
			srd1 = (mlib_d64 *)ptr1;

#pragma pipeloop(0)
			for (l = 0; i < (n - 15); i += 16, k += 2, l++) {
				sd0_m = srd0[l];
				sd1_m = srd1[l];
				dstd[k] =
				    vis_fpmerge(vis_read_hi(sd0_m),
				    vis_read_hi(sd1_m));
				dstd[k + 1] =
				    vis_fpmerge(vis_read_lo(sd0_m),
				    vis_read_lo(sd1_m));
			}

			ptr0 += (k << 2);
			ptr1 += (k << 2);
			dsh = (mlib_u16 *)(dstd + k);
		} else if (((mlib_addr)ptr0 & 7) == ((mlib_addr)ptr1 & 7)) {

			mlib_d64 *srd0, *srd1;
			mlib_d64 *dstd;
			mlib_d64 sd0_h, sd0_l, sd1_h, sd1_l;
			mlib_d64 sd0_m, sd1_m;

			dstd = (mlib_d64 *)dsh;
			srd0 = vis_alignaddr(ptr0, 0);
			srd1 = vis_alignaddr(ptr1, 0);
			sd0_h = srd0[0];
			sd1_h = srd1[0];

#pragma pipeloop(0)
			for (l = 1; i < (n - 15); i += 16, k += 2, l++) {
				sd0_l = srd0[l];
				sd1_l = srd1[l];
				sd0_m = vis_faligndata(sd0_h, sd0_l);
				sd1_m = vis_faligndata(sd1_h, sd1_l);
				dstd[k] =
				    vis_fpmerge(vis_read_hi(sd0_m),
				    vis_read_hi(sd1_m));
				dstd[k + 1] =
				    vis_fpmerge(vis_read_lo(sd0_m),
				    vis_read_lo(sd1_m));
				sd0_h = sd0_l;
				sd1_h = sd1_l;
			}

			ptr0 += (k << 2);
			ptr1 += (k << 2);
			dsh = (mlib_u16 *)(dstd + k);
		} else {

			mlib_d64 *srd0, *srd1;
			mlib_d64 *dstd;
			mlib_d64 sd0_h, sd0_l, sd1_h, sd1_l;
			mlib_d64 sd0_m, sd1_m;

			dstd = (mlib_d64 *)dsh;
			srd0 = vis_alignaddr(ptr0, 0);
			sd0_h = vis_ld_d64_nf(srd0);
			srd1 = vis_alignaddr(ptr1, 0);
			sd1_h = vis_ld_d64_nf(srd1);

#pragma pipeloop(0)
			for (l = 1; i < (n - 15); i += 16, k += 2, l++) {

				srd0 = vis_alignaddr(ptr0, 0);
				sd0_l = vis_ld_d64_nf(srd0 + l);
				sd0_m = vis_faligndata(sd0_h, sd0_l);

				srd1 = vis_alignaddr(ptr1, 0);
				sd1_l = vis_ld_d64_nf(srd1 + l);
				sd1_m = vis_faligndata(sd1_h, sd1_l);

				dstd[k] =
				    vis_fpmerge(vis_read_hi(sd0_m),
				    vis_read_hi(sd1_m));
				dstd[k + 1] =
				    vis_fpmerge(vis_read_lo(sd0_m),
				    vis_read_lo(sd1_m));
				sd0_h = sd0_l;
				sd1_h = sd1_l;
			}

			ptr0 += (k << 2);
			ptr1 += (k << 2);
			dsh = (mlib_u16 *)(dstd + k);
		}

#pragma pipeloop(0)
		for (; i < (n - 1); i += 2) {
			mlib_u16 a = (mlib_u16)((*ptr0++));
			mlib_u16 b = (mlib_u16)((*ptr1++));

			(*dsh++) = (a << 8) | b;
		}

		if ((mlib_addr)dst_u8_0 & 1) {
			dst_u8_0[i] = *ptr0;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    const mlib_u8 *src_u8_2,
    const mlib_u8 *src_u8_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride)
{
	mlib_s32 j, i, k, preread, l, n = width * 4;
	mlib_u8 *ptr0, *ptr1, *ptr2, *ptr3;

	for (j = 0; j < height; j++) {
		preread = (4 - (((mlib_addr)dst_u8_0) & 3)) & 3;
		i = preread;

		if (preread == 3) {
			dst_u8_0[0] = src_u8_0[0];
			dst_u8_0[1] = src_u8_1[0];
			dst_u8_0[2] = src_u8_2[0];
			ptr0 = (mlib_u8 *)src_u8_3;
			ptr1 = (mlib_u8 *)(src_u8_0 + 1);
			ptr2 = (mlib_u8 *)(src_u8_1 + 1);
			ptr3 = (mlib_u8 *)(src_u8_2 + 1);
		} else if (preread == 2) {
			dst_u8_0[0] = src_u8_0[0];
			dst_u8_0[1] = src_u8_1[0];
			ptr0 = (mlib_u8 *)src_u8_2;
			ptr1 = (mlib_u8 *)src_u8_3;
			ptr2 = (mlib_u8 *)(src_u8_0 + 1);
			ptr3 = (mlib_u8 *)(src_u8_1 + 1);
		} else if (preread == 1) {
			dst_u8_0[0] = src_u8_0[0];
			ptr0 = (mlib_u8 *)src_u8_1;
			ptr1 = (mlib_u8 *)src_u8_2;
			ptr2 = (mlib_u8 *)src_u8_3;
			ptr3 = (mlib_u8 *)(src_u8_0 + 1);
		} else if (preread == 0) {
			ptr0 = (mlib_u8 *)src_u8_0;
			ptr1 = (mlib_u8 *)src_u8_1;
			ptr2 = (mlib_u8 *)src_u8_2;
			ptr3 = (mlib_u8 *)src_u8_3;
		}

		if (((mlib_addr)(dst_u8_0 + i) & 7) && (i < (n - 3))) {
			dst_u8_0[i] = (*ptr0++);
			dst_u8_0[i + 1] = (*ptr1++);
			dst_u8_0[i + 2] = (*ptr2++);
			dst_u8_0[i + 3] = (*ptr3++);
			i += 4;
		}

		if (!(((mlib_addr)ptr0 & 7) | ((mlib_addr)ptr2 & 7) |
		    ((mlib_addr)ptr1 & 7) | ((mlib_addr)ptr3 & 7))) {
			mlib_d64 *dstd, sd_02, sd_13;
			mlib_f32 *srf0, *srf1, *srf2, *srf3;

			dstd = (mlib_d64 *)(dst_u8_0 + i);
			srf0 = (mlib_f32 *)ptr0;
			srf1 = (mlib_f32 *)ptr1;
			srf2 = (mlib_f32 *)ptr2;
			srf3 = (mlib_f32 *)ptr3;

#pragma pipeloop(0)
			for (l = 0, k = 0; i < (n - 15); l++, k += 2, i += 16) {
				sd_02 = vis_fpmerge(srf0[l], srf2[l]);
				sd_13 = vis_fpmerge(srf1[l], srf3[l]);
				dstd[k] =
				    vis_fpmerge(vis_read_hi(sd_02),
				    vis_read_hi(sd_13));
				dstd[k + 1] =
				    vis_fpmerge(vis_read_lo(sd_02),
				    vis_read_lo(sd_13));
			}

			ptr0 = (mlib_u8 *)(srf0 + l);
			ptr1 = (mlib_u8 *)(srf1 + l);
			ptr2 = (mlib_u8 *)(srf2 + l);
			ptr3 = (mlib_u8 *)(srf3 + l);
		} else if ((((mlib_addr)ptr0 & 7) == ((mlib_addr)ptr1 & 7)) &&
		    (((mlib_addr)ptr0 & 7) == ((mlib_addr)ptr2 & 7)) &&
		    (((mlib_addr)ptr0 & 7) == ((mlib_addr)ptr3 & 7))) {
			mlib_d64 *dstd, sd_02, sd_13;
			mlib_d64 sd_46, sd_57;
			mlib_d64 *srf0, *srf1, *srf2, *srf3;
			mlib_d64 s0_h, s0_l, s0_m;
			mlib_d64 s1_h, s1_l, s1_m;
			mlib_d64 s2_h, s2_l, s2_m;
			mlib_d64 s3_h, s3_l, s3_m;

			dstd = (mlib_d64 *)(dst_u8_0 + i);
			srf0 = (mlib_d64 *)vis_alignaddr(ptr0, 0);
			srf1 = (mlib_d64 *)vis_alignaddr(ptr1, 0);
			srf2 = (mlib_d64 *)vis_alignaddr(ptr2, 0);
			srf3 = (mlib_d64 *)vis_alignaddr(ptr3, 0);

			s0_h = srf0[0];
			s1_h = srf1[0];
			s2_h = srf2[0];
			s3_h = srf3[0];

#pragma pipeloop(0)
			for (l = 1, k = 0; i < (n - 31); l++, k += 4, i += 32) {
				s0_l = srf0[l];
				s1_l = srf1[l];
				s2_l = srf2[l];
				s3_l = srf3[l];

				s0_m = vis_faligndata(s0_h, s0_l);
				s1_m = vis_faligndata(s1_h, s1_l);
				s2_m = vis_faligndata(s2_h, s2_l);
				s3_m = vis_faligndata(s3_h, s3_l);

				sd_02 =
				    vis_fpmerge(vis_read_hi(s0_m),
				    vis_read_hi(s2_m));
				sd_13 =
				    vis_fpmerge(vis_read_hi(s1_m),
				    vis_read_hi(s3_m));

				sd_46 =
				    vis_fpmerge(vis_read_lo(s0_m),
				    vis_read_lo(s2_m));
				sd_57 =
				    vis_fpmerge(vis_read_lo(s1_m),
				    vis_read_lo(s3_m));

				dstd[k] =
				    vis_fpmerge(vis_read_hi(sd_02),
				    vis_read_hi(sd_13));
				dstd[k + 1] =
				    vis_fpmerge(vis_read_lo(sd_02),
				    vis_read_lo(sd_13));
				dstd[k + 2] =
				    vis_fpmerge(vis_read_hi(sd_46),
				    vis_read_hi(sd_57));
				dstd[k + 3] =
				    vis_fpmerge(vis_read_lo(sd_46),
				    vis_read_lo(sd_57));

				s0_h = s0_l;
				s1_h = s1_l;
				s2_h = s2_l;
				s3_h = s3_l;
			}

			ptr0 += (l - 1) << 3;
			ptr1 += (l - 1) << 3;
			ptr2 += (l - 1) << 3;
			ptr3 += (l - 1) << 3;
		}

		for (; i < (n - 3); i += 4) {
			mlib_u32 a = (mlib_u32)((*ptr0++));
			mlib_u32 b = (mlib_u32)((*ptr1++));
			mlib_u32 c = (mlib_u32)((*ptr2++));
			mlib_u32 d = (mlib_u32)((*ptr3++));

			((mlib_s32 *)(dst_u8_0 + i))[0] =
			    (((a << 8) | b) << 16) | (c << 8) | d;
		}

		if (preread == 3) {
			dst_u8_0[i] = *ptr0;
		} else if (preread == 2) {
			dst_u8_0[i] = *ptr0;
			dst_u8_0[i + 1] = *ptr1;
		} else if (preread == 1) {
			dst_u8_0[i] = *ptr0;
			dst_u8_0[i + 1] = *ptr1;
			dst_u8_0[i + 2] = *ptr2;
		}

		dst_u8_0 += dst_stride;
		src_u8_0 += src0_stride;
		src_u8_1 += src1_stride;
		src_u8_2 += src2_stride;
		src_u8_3 += src3_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
