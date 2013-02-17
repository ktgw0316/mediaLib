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

#pragma ident	"@(#)mlib_v_ImageChannelSplit_U8_24.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_U8
 *      mlib_ImageChannelSplit4_U8
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
mlib_ImageChannelSplit2_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, n = width * 2;
	mlib_f32 *df0, *df1;
	mlib_u16 *dp0, *dp1, *sp0;
	mlib_u16 s0, s1, s2;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = 0;

		if (((mlib_addr)dst_u8_0 & 3) == ((mlib_addr)dst_u8_1 & 3)) {
			for (; (i < n) && ((mlib_addr)(dst_u8_0 + i0) & 3);
			    i += 2) {
				dst_u8_0[i0++] = src_u8_0[i + 0];
				dst_u8_1[i1++] = src_u8_0[i + 1];
			}

			df0 = (mlib_f32 *)(dst_u8_0 + i0);
			df1 = (mlib_f32 *)(dst_u8_1 + i1);

			if ((mlib_addr)(src_u8_0 + i) & 7) {
				mlib_d64 *sd, sd_h, sd_m, sd_l, sd0, sd1, sda,
				    sdb;
				mlib_d64 dd0, dd1;

				sd = vis_alignaddr((void *)(src_u8_0 + i), 0);
				sd_h = (*sd++);

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {

					sd_m = (*sd++);
					sd_l = (*sd++);
					sd0 = vis_faligndata(sd_h, sd_m);
					sd1 = vis_faligndata(sd_m, sd_l);

					sda =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_lo(sd0));
					dd0 =
					    vis_fpmerge(vis_read_hi(sda),
					    vis_read_lo(sda));
					sdb =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_lo(sd1));
					dd1 =
					    vis_fpmerge(vis_read_hi(sdb),
					    vis_read_lo(sdb));

					(*df0++) = vis_read_hi(dd0);
					(*df1++) = vis_read_lo(dd0);
					(*df0++) = vis_read_hi(dd1);
					(*df1++) = vis_read_lo(dd1);

					sd_h = sd_l;

					i0 += 8;
					i1 += 8;
				}
			} else {
/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 8 bytes
 */
				mlib_d64 *sd, sd0, sd1, sda, sdb;
				mlib_d64 dd0, dd1;

				sd = (mlib_d64 *)(src_u8_0 + i);

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {
					sd0 = (*sd++);
					sd1 = (*sd++);

					sda =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_lo(sd0));
					dd0 =
					    vis_fpmerge(vis_read_hi(sda),
					    vis_read_lo(sda));
					sdb =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_lo(sd1));
					dd1 =
					    vis_fpmerge(vis_read_hi(sdb),
					    vis_read_lo(sdb));

					(*df0++) = vis_read_hi(dd0);
					(*df1++) = vis_read_lo(dd0);
					(*df0++) = vis_read_hi(dd1);
					(*df1++) = vis_read_lo(dd1);

					i0 += 8;
					i1 += 8;
				}
			}

			for (; i < (n - 1); i += 2) {
				dst_u8_0[i0++] = src_u8_0[i + 0];
				dst_u8_1[i1++] = src_u8_0[i + 1];
			}
		} else {

			if ((mlib_addr)dst_u8_0 & 1) {
				dst_u8_0[i0++] = src_u8_0[i++];

				if ((mlib_addr)dst_u8_1 & 1) {
					dst_u8_1[i1++] = src_u8_0[i++];
				}
			} else if ((mlib_addr)dst_u8_1 & 1) {
				dst_u8_0[i0++] = src_u8_0[i++];
				dst_u8_1[i1++] = src_u8_0[i++];

				if (i < n) {
					dst_u8_0[i0++] = src_u8_0[i++];
				}
			}

			if (i0 == i1) {
				dp0 = (mlib_u16 *)(dst_u8_0 + i0);
				dp1 = (mlib_u16 *)(dst_u8_1 + i1);
			} else {
				dp1 = (mlib_u16 *)(dst_u8_0 + i0);
				dp0 = (mlib_u16 *)(dst_u8_1 + i1);
			}

			if (((mlib_addr)(src_u8_0 + i) & 1) == 0) {
				sp0 = (mlib_u16 *)(src_u8_0 + i);

#pragma pipeloop(0)
				for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
					s0 = (*sp0++);
					s1 = (*sp0++);
					(*dp0++) = (s0 & ~0xff) + (s1 >> 8);
					(*dp1++) = (s0 << 8) + (s1 & 0xff);
				}
			} else {
				sp0 = (mlib_u16 *)(src_u8_0 + i - 1);
				s0 = (*sp0++);

#pragma pipeloop(0)
				for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
					s1 = (*sp0++);
					s2 = (*sp0++);
					(*dp0++) = (s0 << 8) + (s1 & 0xff);
					(*dp1++) = (s1 & ~0xff) + (s2 >> 8);
					s0 = s2;
				}
			}

			if (i < (n - 2)) {
				dst_u8_1[i1 + 0] = src_u8_0[i + 0];
				dst_u8_0[i0 + 0] = src_u8_0[i + 1];
				dst_u8_1[i1 + 1] = src_u8_0[i + 2];
			} else if (i < (n - 1)) {
				dst_u8_0[i0] = src_u8_0[i + 0];
				dst_u8_1[i1] = src_u8_0[i + 1];
			} else if (i < n) {
				dst_u8_1[i1] = src_u8_0[i];
			}
		}

		dst_u8_0 += dst0_str;
		dst_u8_1 += dst1_str;
		src_u8_0 += src0_str;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    mlib_u8 *dst_u8_2,
    mlib_u8 *dst_u8_3,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 dst3_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, i2, i3, n = width * 4;
	mlib_d64 sd0, sd1, sd2, sd3, sd4;
	mlib_d64 sd01, sd23, sd45, sd67, dd0, dd1, dd2, dd3;
	mlib_d64 sd04, sd26, sd15, sd37, dh0, dh1, dl0, dl1;
	mlib_d64 *dp0, *dp1, *dp2, *dp3, *sp;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = i2 = i3 = 0;

		if ((((mlib_addr)dst_u8_0 & 7) == ((mlib_addr)dst_u8_1 & 7)) &&
		    (((mlib_addr)dst_u8_0 & 7) == ((mlib_addr)dst_u8_2 & 7)) &&
		    (((mlib_addr)dst_u8_0 & 7) == ((mlib_addr)dst_u8_3 & 7))) {
			for (; (i < n) && ((mlib_addr)(dst_u8_0 + i0) & 7);
			    i += 4) {
				dst_u8_0[i0++] = src_u8_0[i + 0];
				dst_u8_1[i1++] = src_u8_0[i + 1];
				dst_u8_2[i2++] = src_u8_0[i + 2];
				dst_u8_3[i3++] = src_u8_0[i + 3];
			}

			dp0 = (mlib_d64 *)(dst_u8_0 + i0);
			dp1 = (mlib_d64 *)(dst_u8_1 + i1);
			dp2 = (mlib_d64 *)(dst_u8_2 + i2);
			dp3 = (mlib_d64 *)(dst_u8_3 + i3);

			if ((mlib_addr)(src_u8_0 + i) & 7) {

				sp = vis_alignaddr((void *)(src_u8_0 + i), 0);

				sd0 = (*sp++);
				sd1 = vis_ld_d64_nf(sp); sp++;
				sd2 = vis_ld_d64_nf(sp); sp++;
				sd3 = vis_ld_d64_nf(sp); sp++;
				sd4 = vis_ld_d64_nf(sp); sp++;

				sd01 = vis_faligndata(sd0, sd1);
				sd23 = vis_faligndata(sd1, sd2);
				sd45 = vis_faligndata(sd2, sd3);
				sd67 = vis_faligndata(sd3, sd4);
				sd04 =
				    vis_fpmerge(vis_read_hi(sd01),
				    vis_read_hi(sd45));
				sd26 =
				    vis_fpmerge(vis_read_hi(sd23),
				    vis_read_hi(sd67));
				sd15 =
				    vis_fpmerge(vis_read_lo(sd01),
				    vis_read_lo(sd45));
				sd37 =
				    vis_fpmerge(vis_read_lo(sd23),
				    vis_read_lo(sd67));

#pragma pipeloop(0)
				for (; i < (n - 31); i += 32) {

					sd0 = vis_ld_d64_nf(sp); sp++;
					sd1 = vis_ld_d64_nf(sp); sp++;
					sd2 = vis_ld_d64_nf(sp); sp++;
					sd3 = vis_ld_d64_nf(sp); sp++;

					sd01 = vis_faligndata(sd4, sd0);
					sd23 = vis_faligndata(sd0, sd1);
					sd45 = vis_faligndata(sd1, sd2);
					sd67 = vis_faligndata(sd2, sd3);

					dh0 =
					    vis_fpmerge(vis_read_hi(sd04),
					    vis_read_hi(sd26));
					dh1 =
					    vis_fpmerge(vis_read_lo(sd04),
					    vis_read_lo(sd26));
					dl0 =
					    vis_fpmerge(vis_read_hi(sd15),
					    vis_read_hi(sd37));
					dl1 =
					    vis_fpmerge(vis_read_lo(sd15),
					    vis_read_lo(sd37));
					dd0 =
					    vis_fpmerge(vis_read_hi(dh0),
					    vis_read_hi(dl0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dh0),
					    vis_read_lo(dl0));
					dd2 =
					    vis_fpmerge(vis_read_hi(dh1),
					    vis_read_hi(dl1));
					dd3 =
					    vis_fpmerge(vis_read_lo(dh1),
					    vis_read_lo(dl1));
					sd04 =
					    vis_fpmerge(vis_read_hi(sd01),
					    vis_read_hi(sd45));
					sd26 =
					    vis_fpmerge(vis_read_hi(sd23),
					    vis_read_hi(sd67));
					sd15 =
					    vis_fpmerge(vis_read_lo(sd01),
					    vis_read_lo(sd45));
					sd37 =
					    vis_fpmerge(vis_read_lo(sd23),
					    vis_read_lo(sd67));

					(*dp0++) = dd0;
					(*dp1++) = dd1;
					(*dp2++) = dd2;
					(*dp3++) = dd3;

					sd4 = sd3;

					i0 += 8;
					i1 += 8;
					i2 += 8;
					i3 += 8;
				}
			} else {
/*
 * Ideal case: all dst vectors and src vector are
 * aligned to 8 bytes
 */
				mlib_d64 dd0, dd1, dd2;

				sp = (mlib_d64 *)(src_u8_0 + i);

				sd01 = vis_ld_d64_nf(sp); sp++;
				sd23 = vis_ld_d64_nf(sp); sp++;
				sd45 = vis_ld_d64_nf(sp); sp++;
				sd67 = vis_ld_d64_nf(sp); sp++;
				sd04 =
				    vis_fpmerge(vis_read_hi(sd01),
				    vis_read_hi(sd45));
				sd26 =
				    vis_fpmerge(vis_read_hi(sd23),
				    vis_read_hi(sd67));
				sd15 =
				    vis_fpmerge(vis_read_lo(sd01),
				    vis_read_lo(sd45));
				sd37 =
				    vis_fpmerge(vis_read_lo(sd23),
				    vis_read_lo(sd67));

#pragma pipeloop(0)
				for (; i < (n - 31); i += 32) {
					sd01 = vis_ld_d64_nf(sp); sp++;
					sd23 = vis_ld_d64_nf(sp); sp++;
					sd45 = vis_ld_d64_nf(sp); sp++;
					sd67 = vis_ld_d64_nf(sp); sp++;

					dh0 =
					    vis_fpmerge(vis_read_hi(sd04),
					    vis_read_hi(sd26));
					dh1 =
					    vis_fpmerge(vis_read_lo(sd04),
					    vis_read_lo(sd26));
					dl0 =
					    vis_fpmerge(vis_read_hi(sd15),
					    vis_read_hi(sd37));
					dl1 =
					    vis_fpmerge(vis_read_lo(sd15),
					    vis_read_lo(sd37));
					dd0 =
					    vis_fpmerge(vis_read_hi(dh0),
					    vis_read_hi(dl0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dh0),
					    vis_read_lo(dl0));
					dd2 =
					    vis_fpmerge(vis_read_hi(dh1),
					    vis_read_hi(dl1));
					dd3 =
					    vis_fpmerge(vis_read_lo(dh1),
					    vis_read_lo(dl1));
					sd04 =
					    vis_fpmerge(vis_read_hi(sd01),
					    vis_read_hi(sd45));
					sd26 =
					    vis_fpmerge(vis_read_hi(sd23),
					    vis_read_hi(sd67));
					sd15 =
					    vis_fpmerge(vis_read_lo(sd01),
					    vis_read_lo(sd45));
					sd37 =
					    vis_fpmerge(vis_read_lo(sd23),
					    vis_read_lo(sd67));

					(*dp0++) = dd0;
					(*dp1++) = dd1;
					(*dp2++) = dd2;
					(*dp3++) = dd3;

					i0 += 8;
					i1 += 8;
					i2 += 8;
					i3 += 8;
				}
			}

			for (; i < (n - 1); i += 4) {
				dst_u8_0[i0++] = src_u8_0[i + 0];
				dst_u8_1[i1++] = src_u8_0[i + 1];
				dst_u8_2[i2++] = src_u8_0[i + 2];
				dst_u8_3[i3++] = src_u8_0[i + 3];
			}
		} else {

			mlib_u32 var_uint;
			mlib_u8 *dst0, *dst1, *dst2, *dst3, *src =
			    (mlib_u8 *)src_u8_0;

			if ((mlib_addr)(src_u8_0) & 3) {
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
				dst0 = dst_u8_3 + 0;
				dst1 = dst_u8_0 + 1;
				dst2 = dst_u8_1 + 1;
				dst3 = dst_u8_2 + 1;
			} else if (i == 2) {
				dst0 = dst_u8_2 + 0;
				dst1 = dst_u8_3 + 0;
				dst2 = dst_u8_0 + 1;
				dst3 = dst_u8_1 + 1;
			} else if (i == 1) {
				dst0 = dst_u8_1 + 0;
				dst1 = dst_u8_2 + 0;
				dst2 = dst_u8_3 + 0;
				dst3 = dst_u8_0 + 1;
			} else if (i == 0) {
				dst0 = dst_u8_0;
				dst1 = dst_u8_1;
				dst2 = dst_u8_2;
				dst3 = dst_u8_3;
			}
#pragma pipeloop(0)
			for (; i < (n - 3); i += 4) {
				var_uint = ((mlib_u32 *)src)[0];
				(*dst3++) = var_uint;
				(*dst2++) = var_uint >> 8;
				(*dst1++) = var_uint >> 16;
				(*dst0++) = var_uint >> 24;
				src += 4;
			}

			if (i < (n - 2)) {
				dst_u8_1[width - 1] = (*src++);
				dst_u8_2[width - 1] = (*src++);
				dst_u8_3[width - 1] = (*src++);
			} else if (i < (n - 1)) {
				dst_u8_2[width - 1] = (*src++);
				dst_u8_3[width - 1] = (*src++);
			} else if (i < n) {
				dst_u8_3[width - 1] = (*src++);
			}
		}

		dst_u8_0 += dst0_str;
		dst_u8_1 += dst1_str;
		dst_u8_2 += dst2_str;
		dst_u8_3 += dst3_str;
		src_u8_0 += src0_str;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
