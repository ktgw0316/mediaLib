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

#pragma ident	"@(#)mlib_v_ImageChannelSplit_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_S16
 *      mlib_ImageChannelSplit3_S16
 *      mlib_ImageChannelSplit4_S16
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

#define	EPROC(res, f1, f2)                                      \
	dr = vis_fpmerge(f1, f2);                               \
	res = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_S16(
    mlib_s16 *dst_s16_0,
    mlib_s16 *dst_s16_1,
    const mlib_s16 *src_s16_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, n = width * 2;
	mlib_d64 sd0, sd1, sd2, sd3, sd4, dd0, dd1, dr, dr1;
	mlib_d64 s0, s1, s2, s3, *dp0, *dp1, *sp;
	mlib_u32 *dpi0, *dpi1, *spi0;
	mlib_u32 si0, si1, si2;
	mlib_addr align;

	vis_write_gsr(0);
	dst0_str >>= 1;
	dst1_str >>= 1;
	src0_str >>= 1;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = 0;

		if ((((mlib_addr)dst_s16_0 ^ (mlib_addr)dst_s16_1) & 7) == 0) {

			for (; (i < n) && ((mlib_addr)(dst_s16_0 + i0) & 7);
			    i += 2) {
				dst_s16_0[i0++] = src_s16_0[i + 0];
				dst_s16_1[i1++] = src_s16_0[i + 1];
			}

			dp0 = (mlib_d64 *)(dst_s16_0 + i0);
			dp1 = (mlib_d64 *)(dst_s16_1 + i1);
			align = (mlib_addr)(src_s16_0 + i);

			if (align & 7) {

				sp = vis_alignaddr((void *)align, 0);
				sd0 = ((*sp++));

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {
					sd1 = ((*sp++));
					sd2 = ((*sp++));
					sd3 = ((*sp++));
					sd4 = ((*sp++));

					s0 = vis_faligndata(sd0, sd1);
					s1 = vis_faligndata(sd1, sd2);
					s2 = vis_faligndata(sd2, sd3);
					s3 = vis_faligndata(sd3, sd4);

					dr = vis_fpmerge(vis_read_hi(s0),
					    vis_read_hi(s1));
					dr1 =
					    vis_fpmerge(vis_read_lo(s0),
					    vis_read_lo(s1));
					dd0 =
					    vis_fpmerge(vis_read_hi(dr),
					    vis_read_hi(dr1));
					dp0[0] =
					    vis_fpmerge(vis_read_hi(dd0),
					    vis_read_lo(dd0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dr),
					    vis_read_lo(dr1));
					dp1[0] =
					    vis_fpmerge(vis_read_hi(dd1),
					    vis_read_lo(dd1));

					vis_alignaddr((void *)sp, 2);
					dd0 = vis_faligndata(s2, s2);
					dd1 = vis_fpack32(s3, s3);
					dd1 = vis_fpack32(dd1, dd1);
					dp0[1] = vis_fpackfix_pair(s2, s3);
					dp1[1] = vis_fpackfix_pair(dd0, dd1);
					vis_alignaddr((void *)align, 0);

					dp0 += 2;
					dp1 += 2;

					sd0 = sd4;

					i0 += 8;
					i1 += 8;
				}
			} else {
				sp = (mlib_d64 *)align;
				vis_alignaddr((void *)align, 2);

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {
					sd0 = ((*sp++));
					sd1 = ((*sp++));
					sd2 = ((*sp++));
					sd3 = ((*sp++));
					dr = vis_fpmerge(vis_read_hi(sd0),
					    vis_read_hi(sd1));
					dr1 =
					    vis_fpmerge(vis_read_lo(sd0),
					    vis_read_lo(sd1));
					dd0 =
					    vis_fpmerge(vis_read_hi(dr),
					    vis_read_hi(dr1));
					dp0[0] =
					    vis_fpmerge(vis_read_hi(dd0),
					    vis_read_lo(dd0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dr),
					    vis_read_lo(dr1));
					dp1[0] =
					    vis_fpmerge(vis_read_hi(dd1),
					    vis_read_lo(dd1));
					dd0 = vis_faligndata(sd2, sd2);
					dd1 = vis_fpack32(sd3, sd3);
					dd1 = vis_fpack32(dd1, dd1);
					dp0[1] = vis_fpackfix_pair(sd2, sd3);
					dp1[1] = vis_fpackfix_pair(dd0, dd1);
					dp0 += 2;
					dp1 += 2;

					i0 += 8;
					i1 += 8;
				}
			}

			for (; i < (n - 1); i += 2) {
				dst_s16_0[i0++] = src_s16_0[i + 0];
				dst_s16_1[i1++] = src_s16_0[i + 1];
			}
		} else {

			if ((mlib_addr)dst_s16_0 & 3) {
				dst_s16_0[i0++] = src_s16_0[i++];

				if ((mlib_addr)dst_s16_1 & 3) {
					dst_s16_1[i1++] = src_s16_0[i++];
				}
			} else if ((mlib_addr)dst_s16_1 & 3) {
				dst_s16_0[i0++] = src_s16_0[i++];
				dst_s16_1[i1++] = src_s16_0[i++];

				if (i < n) {
					dst_s16_0[i0++] = src_s16_0[i++];
				}
			}

			if (i0 == i1) {
				dpi0 = (mlib_u32 *)(dst_s16_0 + i0);
				dpi1 = (mlib_u32 *)(dst_s16_1 + i1);
			} else {
				dpi1 = (mlib_u32 *)(dst_s16_0 + i0);
				dpi0 = (mlib_u32 *)(dst_s16_1 + i1);
			}

			if (((mlib_addr)(src_s16_0 + i) & 3) == 0) {
				spi0 = (mlib_u32 *)(src_s16_0 + i);

#pragma pipeloop(0)
				for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
					si0 = ((*spi0++));
					si1 = ((*spi0++));
					((*dpi0++)) =
					    (si0 & ~0xffff) + (si1 >> 16);
					((*dpi1++)) =
					    (si0 << 16) + (si1 & 0xffff);
				}
			} else {
				spi0 = (mlib_u32 *)(src_s16_0 + i - 1);
				si0 = ((*spi0++));

#pragma pipeloop(0)
				for (; i < (n - 3); i += 4, i0 += 2, i1 += 2) {
					si1 = ((*spi0++));
					si2 = ((*spi0++));
					((*dpi0++)) =
					    (si0 << 16) + (si1 & 0xffff);
					((*dpi1++)) =
					    (si1 & ~0xffff) + (si2 >> 16);
					si0 = si2;
				}
			}

			if (i < (n - 2)) {
				dst_s16_1[i1 + 0] = src_s16_0[i + 0];
				dst_s16_0[i0 + 0] = src_s16_0[i + 1];
				dst_s16_1[i1 + 1] = src_s16_0[i + 2];
			} else if (i < (n - 1)) {
				dst_s16_0[i0] = src_s16_0[i + 0];
				dst_s16_1[i1] = src_s16_0[i + 1];
			} else if (i < n) {
				dst_s16_1[i1] = src_s16_0[i];
			}
		}

		dst_s16_0 += dst0_str;
		dst_s16_1 += dst1_str;
		src_s16_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_S16(
    mlib_s16 *dst_s16_0,
    mlib_s16 *dst_s16_1,
    mlib_s16 *dst_s16_2,
    const mlib_s16 *src_s16_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, i2, n = width * 3;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2;
	mlib_d64 s0, s1, s2, dr;
	mlib_d64 *sp, *dp0, *dp1, *dp2;
	mlib_s16 *dst0, *dst1, *dst2, *src;
	mlib_u64 udb0, udb1, udb2;

	dst0_str >>= 1;
	dst1_str >>= 1;
	dst2_str >>= 1;
	src0_str >>= 1;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = i2 = 0;

		if ((((mlib_addr)dst_s16_0 & 7) == ((mlib_addr)dst_s16_1 &
		    7)) && (((mlib_addr)dst_s16_0 & 7) ==
		    ((mlib_addr)dst_s16_2 & 7))) {

			for (; (i < n) && ((mlib_addr)(dst_s16_0 + i0) & 7);
			    i += 3) {
				dst_s16_0[i0++] = src_s16_0[i + 0];
				dst_s16_1[i1++] = src_s16_0[i + 1];
				dst_s16_2[i2++] = src_s16_0[i + 2];
			}

			dp0 = (mlib_d64 *)(dst_s16_0 + i0);
			dp1 = (mlib_d64 *)(dst_s16_1 + i1);
			dp2 = (mlib_d64 *)(dst_s16_2 + i2);

			if ((mlib_addr)(src_s16_0 + i) & 7) {

				sp = vis_alignaddr((void *)(src_s16_0 + i), 0);
				sd0 = ((*sp++));

#pragma pipeloop(0)
				for (; i < (n - 11); i += 12) {
					sd1 = ((*sp++));
					sd2 = ((*sp++));
					sd3 = ((*sp++));

					s0 = vis_faligndata(sd0, sd1);
					s1 = vis_faligndata(sd1, sd2);
					s2 = vis_faligndata(sd2, sd3);

					dd0 =
					    vis_fpmerge(vis_read_hi(s0),
					    vis_read_lo(s1));
					dd1 =
					    vis_fpmerge(vis_read_lo(s0),
					    vis_read_hi(s2));
					dd2 =
					    vis_fpmerge(vis_read_hi(s1),
					    vis_read_lo(s2));
					EPROC(((*dp0++)), vis_read_hi(dd0),
					    vis_read_lo(dd1));
					EPROC(((*dp1++)), vis_read_lo(dd0),
					    vis_read_hi(dd2));
					EPROC(((*dp2++)), vis_read_hi(dd1),
					    vis_read_lo(dd2));

					sd0 = sd3;

					i0 += 4;
					i1 += 4;
					i2 += 4;
				}
			} else {

				sp = (mlib_d64 *)(src_s16_0 + i);

#pragma pipeloop(0)
				for (; i < (n - 11); i += 12) {
					sd0 = ((*sp++));
					sd1 = ((*sp++));
					sd2 = ((*sp++));
					dd0 =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_lo(sd1));
					dd1 =
					    vis_fpmerge(vis_read_lo(sd0),
					    vis_read_hi(sd2));
					dd2 =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_lo(sd2));
					EPROC(((*dp0++)), vis_read_hi(dd0),
					    vis_read_lo(dd1));
					EPROC(((*dp1++)), vis_read_lo(dd0),
					    vis_read_hi(dd2));
					EPROC(((*dp2++)), vis_read_hi(dd1),
					    vis_read_lo(dd2));

					i0 += 4;
					i1 += 4;
					i2 += 4;
				}
			}

			for (; i < (n - 1); i += 3) {
				dst_s16_0[i0++] = src_s16_0[i + 0];
				dst_s16_1[i1++] = src_s16_0[i + 1];
				dst_s16_2[i2++] = src_s16_0[i + 2];
			}
		} else {

			dst0 = dst_s16_0;
			dst1 = dst_s16_1;
			dst2 = dst_s16_2;
			src = (void *)src_s16_0;

			if ((mlib_addr)(src_s16_0 + i) & 7) {
				dst_s16_0[0] = ((*src++));
				i++;

				if ((mlib_addr)(src_s16_0 + i) & 7) {
					dst_s16_1[0] = ((*src++));
					i++;

					if ((mlib_addr)(src_s16_0 + i) & 7) {
						dst_s16_2[0] = ((*src++));
						i++;
					}
				}
			}

			if (i == 3) {
				dst0 = dst_s16_0 + 1;
				dst1 = dst_s16_1 + 1;
				dst2 = dst_s16_2 + 1;
			} else if (i == 2) {
				dst1 = dst_s16_0 + 1;
				dst2 = dst_s16_1 + 1;
				dst0 = dst_s16_2 + 0;
			} else if (i == 1) {
				dst2 = dst_s16_0 + 1;
				dst0 = dst_s16_1 + 0;
				dst1 = dst_s16_2 + 0;
			}
#pragma pipeloop(0)
			for (; i < (n - 11); i += 12) {
				udb0 = ((mlib_u64 *)src)[0];
				udb1 = ((mlib_u64 *)src)[1];
				udb2 = ((mlib_u64 *)src)[2];
				dst0[1] = udb0;
				dst2[0] = udb0 >> 16;
				dst1[0] = udb0 >> 32;
				dst0[0] = udb0 >> 48;

				dst1[2] = udb1;
				dst0[2] = udb1 >> 16;
				dst2[1] = udb1 >> 32;
				dst1[1] = udb1 >> 48;

				dst2[3] = udb2;
				dst1[3] = udb2 >> 16;
				dst0[3] = udb2 >> 32;
				dst2[2] = udb2 >> 48;

				dst0 += 4;
				dst1 += 4;
				dst2 += 4;
				src += 12;
			}

#pragma pipeloop(0)
			for (; i < (n - 2); i += 3) {
				((*dst0++)) = ((*src++));
				((*dst1++)) = ((*src++));
				((*dst2++)) = ((*src++));
			}

			if (i < (n - 1)) {
				dst_s16_1[width - 1] = ((*src++));
				dst_s16_2[width - 1] = ((*src++));
			} else if (i < n) {
				dst_s16_2[width - 1] = ((*src++));
			}
		}

		dst_s16_0 += dst0_str;
		dst_s16_1 += dst1_str;
		dst_s16_2 += dst2_str;
		src_s16_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_S16(
    mlib_s16 *dst_s16_0,
    mlib_s16 *dst_s16_1,
    mlib_s16 *dst_s16_2,
    mlib_s16 *dst_s16_3,
    const mlib_s16 *src_s16_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 dst3_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, i0, i1, i2, i3, n = width * 4;
	mlib_d64 s0, s1, s2, s3, dd0, dd1, dd2, dd3, dr0, dr1;
	mlib_d64 sd0, sd1, sd2, sd3, sd4, *dp0, *dp1, *dp2, *dp3, *sp;
	mlib_u64 udb;
	mlib_s16 *dst0, *dst1, *dst2, *dst3, *src;

	dst0_str >>= 1;
	dst1_str >>= 1;
	dst2_str >>= 1;
	dst3_str >>= 1;
	src0_str >>= 1;

	for (j = 0; j < height; j++) {

		i = i0 = i1 = i2 = i3 = 0;

		if ((((mlib_addr)dst_s16_0 & 7) == ((mlib_addr)dst_s16_1
		    & 7)) && (((mlib_addr)dst_s16_0 & 7) ==
		    ((mlib_addr)dst_s16_2 & 7)) &&
		    (((mlib_addr)dst_s16_0 & 7) ==
		    ((mlib_addr)dst_s16_3 & 7))) {

			for (; (i < n) && ((mlib_addr)(dst_s16_0 + i0) & 7);
			    i += 4) {
				dst_s16_0[i0++] = src_s16_0[i + 0];
				dst_s16_1[i1++] = src_s16_0[i + 1];
				dst_s16_2[i2++] = src_s16_0[i + 2];
				dst_s16_3[i3++] = src_s16_0[i + 3];
			}

			dp0 = (mlib_d64 *)(dst_s16_0 + i0);
			dp1 = (mlib_d64 *)(dst_s16_1 + i1);
			dp2 = (mlib_d64 *)(dst_s16_2 + i2);
			dp3 = (mlib_d64 *)(dst_s16_3 + i3);

			if ((mlib_addr)(src_s16_0 + i) & 7) {

				sp = vis_alignaddr((void *)(src_s16_0 + i), 0);
				sd0 = ((*sp++));

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {
					sd1 = ((*sp++));
					sd2 = ((*sp++));
					sd3 = ((*sp++));
					sd4 = ((*sp++));

					s0 = vis_faligndata(sd0, sd1);
					s1 = vis_faligndata(sd1, sd2);
					s2 = vis_faligndata(sd2, sd3);
					s3 = vis_faligndata(sd3, sd4);

					dr0 =
					    vis_fpmerge(vis_read_hi(s0),
					    vis_read_hi(s2));
					dr1 =
					    vis_fpmerge(vis_read_hi(s1),
					    vis_read_hi(s3));
					dd0 =
					    vis_fpmerge(vis_read_hi(dr0),
					    vis_read_hi(dr1));
					((*dp0++)) =
					    vis_fpmerge(vis_read_hi(dd0),
					    vis_read_lo(dd0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dr0),
					    vis_read_lo(dr1));
					((*dp1++)) =
					    vis_fpmerge(vis_read_hi(dd1),
					    vis_read_lo(dd1));
					dr0 =
					    vis_fpmerge(vis_read_lo(s0),
					    vis_read_lo(s2));
					dr1 =
					    vis_fpmerge(vis_read_lo(s1),
					    vis_read_lo(s3));
					dd2 =
					    vis_fpmerge(vis_read_hi(dr0),
					    vis_read_hi(dr1));
					((*dp2++)) =
					    vis_fpmerge(vis_read_hi(dd2),
					    vis_read_lo(dd2));
					dd3 =
					    vis_fpmerge(vis_read_lo(dr0),
					    vis_read_lo(dr1));
					((*dp3++)) =
					    vis_fpmerge(vis_read_hi(dd3),
					    vis_read_lo(dd3));

					sd0 = sd4;

					i0 += 4;
					i1 += 4;
					i2 += 4;
					i3 += 4;
				}
			} else {

				sp = (mlib_d64 *)(src_s16_0 + i);

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {
					s0 = ((*sp++));
					s1 = ((*sp++));
					s2 = ((*sp++));
					s3 = ((*sp++));

					dr0 =
					    vis_fpmerge(vis_read_hi(s0),
					    vis_read_hi(s2));
					dr1 =
					    vis_fpmerge(vis_read_hi(s1),
					    vis_read_hi(s3));
					dd0 =
					    vis_fpmerge(vis_read_hi(dr0),
					    vis_read_hi(dr1));
					((*dp0++)) =
					    vis_fpmerge(vis_read_hi(dd0),
					    vis_read_lo(dd0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dr0),
					    vis_read_lo(dr1));
					((*dp1++)) =
					    vis_fpmerge(vis_read_hi(dd1),
					    vis_read_lo(dd1));
					dr0 =
					    vis_fpmerge(vis_read_lo(s0),
					    vis_read_lo(s2));
					dr1 =
					    vis_fpmerge(vis_read_lo(s1),
					    vis_read_lo(s3));
					dd2 =
					    vis_fpmerge(vis_read_hi(dr0),
					    vis_read_hi(dr1));
					((*dp2++)) =
					    vis_fpmerge(vis_read_hi(dd2),
					    vis_read_lo(dd2));
					dd3 =
					    vis_fpmerge(vis_read_lo(dr0),
					    vis_read_lo(dr1));
					((*dp3++)) =
					    vis_fpmerge(vis_read_hi(dd3),
					    vis_read_lo(dd3));

					i0 += 4;
					i1 += 4;
					i2 += 4;
					i3 += 4;
				}
			}

			for (; i < (n - 1); i += 4) {
				dst_s16_0[i0++] = src_s16_0[i + 0];
				dst_s16_1[i1++] = src_s16_0[i + 1];
				dst_s16_2[i2++] = src_s16_0[i + 2];
				dst_s16_3[i3++] = src_s16_0[i + 3];
			}
		} else {

			dst0 = dst_s16_0;
			dst1 = dst_s16_1;
			dst2 = dst_s16_2;
			dst3 = dst_s16_3;
			src = (void *)src_s16_0;

			if ((mlib_addr)(src_s16_0 + i) & 7) {
				dst_s16_0[0] = ((*src++));
				i++;

				if ((mlib_addr)(src_s16_0 + i) & 7) {
					dst_s16_1[0] = ((*src++));
					i++;

					if ((mlib_addr)(src_s16_0 + i) & 7) {
						dst_s16_2[0] = ((*src++));
						i++;
					}
				}
			}

			if (i == 3) {
				dst0 = dst_s16_3 + 0;
				dst1 = dst_s16_0 + 1;
				dst2 = dst_s16_1 + 1;
				dst3 = dst_s16_2 + 1;
			} else if (i == 2) {
				dst0 = dst_s16_2 + 0;
				dst1 = dst_s16_3 + 0;
				dst2 = dst_s16_0 + 1;
				dst3 = dst_s16_1 + 1;
			} else if (i == 1) {
				dst0 = dst_s16_1 + 0;
				dst1 = dst_s16_2 + 0;
				dst2 = dst_s16_3 + 0;
				dst3 = dst_s16_0 + 1;
			}
#pragma pipeloop(0)
			for (; i < (n - 3); i += 4) {
				udb = ((mlib_u64 *)src)[0];
				((*dst3++)) = udb;
				((*dst2++)) = udb >> 16;
				((*dst1++)) = udb >> 32;
				((*dst0++)) = udb >> 48;
				src += 4;
			}

			if (i < (n - 2)) {
				dst_s16_1[width - 1] = ((*src++));
				dst_s16_2[width - 1] = ((*src++));
				dst_s16_3[width - 1] = ((*src++));
			} else if (i < (n - 1)) {
				dst_s16_2[width - 1] = ((*src++));
				dst_s16_3[width - 1] = ((*src++));
			} else if (i < n) {
				dst_s16_3[width - 1] = ((*src++));
			}
		}

		dst_s16_0 += dst0_str;
		dst_s16_1 += dst1_str;
		dst_s16_2 += dst2_str;
		dst_s16_3 += dst3_str;
		src_s16_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
