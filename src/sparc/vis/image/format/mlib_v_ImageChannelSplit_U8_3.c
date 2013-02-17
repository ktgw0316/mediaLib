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

#pragma ident	"@(#)mlib_v_ImageChannelSplit_U8_3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit3_U8.
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
mlib_ImageChannelSplit3_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    mlib_u8 *dst_u8_2,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, i2, n = width * 3;
	mlib_d64 *dp0, *dp1, *dp2;
	mlib_u8 *dst0, *dst1, *dst2, *src;
	mlib_u32 uint0, uint1, uint2;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = i2 = 0;

		if ((((mlib_addr)dst_u8_0 & 7) == ((mlib_addr)dst_u8_1 & 7)) &&
		    (((mlib_addr)dst_u8_0 & 7) == ((mlib_addr)dst_u8_2 & 7))) {
			for (; (i < n) && ((mlib_addr)(dst_u8_0 + i0) & 7);
			    i += 3) {
				dst_u8_0[i0++] = src_u8_0[i + 0];
				dst_u8_1[i1++] = src_u8_0[i + 1];
				dst_u8_2[i2++] = src_u8_0[i + 2];
			}

			dp0 = (mlib_d64 *)(dst_u8_0 + i0);
			dp1 = (mlib_d64 *)(dst_u8_1 + i1);
			dp2 = (mlib_d64 *)(dst_u8_2 + i2);

			if ((mlib_addr)(src_u8_0 + i) & 7) {
				mlib_d64 *sd, sd_0, sd_1, sd_2, sd_3, sd0, sd1,
				    sd2, sda, sdb, sdc, sdd, sde;
				mlib_d64 dd0, dd1, dd2;

				sd = vis_alignaddr((void *)(src_u8_0 + i), 0);
				sd_0 = (*sd++);

#pragma pipeloop(0)
				for (; i < (n - 23); i += 24) {

					sd_1 = (*sd++);
					sd_2 = (*sd++);
					sd_3 = (*sd++);
					sd0 = vis_faligndata(sd_0, sd_1);
					sd1 = vis_faligndata(sd_1, sd_2);
					sd2 = vis_faligndata(sd_2, sd_3);

					sda =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_lo(sd1));
					sdb =
					    vis_fpmerge(vis_read_lo(sd0),
					    vis_read_hi(sd2));
					sdc =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_lo(sd2));
					sdd =
					    vis_fpmerge(vis_read_hi(sda),
					    vis_read_lo(sdb));
					sde =
					    vis_fpmerge(vis_read_lo(sda),
					    vis_read_hi(sdc));
					dd0 =
					    vis_fpmerge(vis_read_hi(sdd),
					    vis_read_lo(sde));
					sde =
					    vis_fpmerge(vis_read_hi(sdb),
					    vis_read_lo(sdc));
					dd1 =
					    vis_fpmerge(vis_read_lo(sdd),
					    vis_read_hi(sde));
					sdd =
					    vis_fpmerge(vis_read_lo(sda),
					    vis_read_hi(sdc));
					dd2 =
					    vis_fpmerge(vis_read_hi(sdd),
					    vis_read_lo(sde));

					(*dp0++) = dd0;
					(*dp1++) = dd1;
					(*dp2++) = dd2;

					sd_0 = sd_3;

					i0 += 8;
					i1 += 8;
					i2 += 8;
				}
			} else {
/*
 * Ideal case: all dst vectors and src vector are
 * aligned to 8 bytes
 */
				mlib_d64 *sd, sd0, sd1, sd2, sda, sdb, sdc, sdd,
				    sde;
				mlib_d64 dd0, dd1, dd2;

				sd = (mlib_d64 *)(src_u8_0 + i);

#pragma pipeloop(0)
				for (; i < (n - 23); i += 24) {
					sd0 = (*sd++);
					sd1 = (*sd++);
					sd2 = (*sd++);

					sda =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_lo(sd1));
					sdb =
					    vis_fpmerge(vis_read_lo(sd0),
					    vis_read_hi(sd2));
					sdc =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_lo(sd2));
					sdd =
					    vis_fpmerge(vis_read_hi(sda),
					    vis_read_lo(sdb));
					sde =
					    vis_fpmerge(vis_read_lo(sda),
					    vis_read_hi(sdc));
					dd0 =
					    vis_fpmerge(vis_read_hi(sdd),
					    vis_read_lo(sde));
					sde =
					    vis_fpmerge(vis_read_hi(sdb),
					    vis_read_lo(sdc));
					dd1 =
					    vis_fpmerge(vis_read_lo(sdd),
					    vis_read_hi(sde));
					sdd =
					    vis_fpmerge(vis_read_lo(sda),
					    vis_read_hi(sdc));
					dd2 =
					    vis_fpmerge(vis_read_hi(sdd),
					    vis_read_lo(sde));

					(*dp0++) = dd0;
					(*dp1++) = dd1;
					(*dp2++) = dd2;

					i0 += 8;
					i1 += 8;
					i2 += 8;
				}
			}

			for (; i < (n - 1); i += 3) {
				dst_u8_0[i0++] = src_u8_0[i + 0];
				dst_u8_1[i1++] = src_u8_0[i + 1];
				dst_u8_2[i2++] = src_u8_0[i + 2];
			}
		} else {

			dst0 = dst_u8_0;
			dst1 = dst_u8_1;
			dst2 = dst_u8_2;
			src = (void *)src_u8_0;

			if ((mlib_addr)(src_u8_0 + i) & 3) {
				dst_u8_0[0] = (*src++);
				i++;

				if ((mlib_addr)(src_u8_0 + i) & 3) {
					dst_u8_1[0] = (*src++);
					i++;

					if ((mlib_addr)(src_u8_0 + i) & 3) {
						dst_u8_2[0] = (*src++);
						i++;
					}
				}
			}

			if (i == 3) {
				dst0 = dst_u8_0 + 1;
				dst1 = dst_u8_1 + 1;
				dst2 = dst_u8_2 + 1;
			} else if (i == 2) {
				dst1 = dst_u8_0 + 1;
				dst2 = dst_u8_1 + 1;
				dst0 = dst_u8_2 + 0;
			} else if (i == 1) {
				dst2 = dst_u8_0 + 1;
				dst0 = dst_u8_1 + 0;
				dst1 = dst_u8_2 + 0;
			}
#pragma pipeloop(0)
			for (; i < (n - 11); i += 12) {
				uint0 = ((mlib_u32 *)src)[0];
				uint1 = ((mlib_u32 *)src)[1];
				uint2 = ((mlib_u32 *)src)[2];

				dst0[1] = uint0;
				dst2[0] = uint0 >> 8;
				dst1[0] = uint0 >> 16;
				dst0[0] = uint0 >> 24;

				dst1[2] = uint1;
				dst0[2] = uint1 >> 8;
				dst2[1] = uint1 >> 16;
				dst1[1] = uint1 >> 24;

				dst2[3] = uint2;
				dst1[3] = uint2 >> 8;
				dst0[3] = uint2 >> 16;
				dst2[2] = uint2 >> 24;

				dst0 += 4;
				dst1 += 4;
				dst2 += 4;
				src += 12;
			}

#pragma pipeloop(0)
			for (; i < (n - 2); i += 3) {
				(*dst0++) = (*src++);
				(*dst1++) = (*src++);
				(*dst2++) = (*src++);
			}

			if (i < (n - 1)) {
				dst_u8_1[width - 1] = (*src++);
				dst_u8_2[width - 1] = (*src++);
			} else if (i < n) {
				dst_u8_2[width - 1] = (*src++);
			}
		}

		dst_u8_0 += dst0_str;
		dst_u8_1 += dst1_str;
		dst_u8_2 += dst2_str;
		src_u8_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
