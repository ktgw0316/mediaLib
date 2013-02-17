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

#pragma ident	"@(#)mlib_v_ImageChannelMerge_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge2_S16
 *      mlib_ImageChannelMerge3_S16
 *      mlib_ImageChannelMerge4_S16
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageChannelMerge.h>

/* *********************************************************** */

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge2_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride)
{
	mlib_s32 i, j, k, n = width * 2;
	mlib_u32 *dp, s0, s1;
	mlib_s16 *f_ptr, *s_ptr;
	mlib_d64 *sd0_ptr, *sd1_ptr;
	mlib_d64 sd0, sd1, s0h, s1h, s0l, s1l;
	mlib_d64 t0, t1, t2, t3;

	for (j = 0; j < height; j++) {
		i = 0;

		if ((mlib_addr)dst_s16_0 & 3) {
			dst_s16_0[0] = src_s16_0[0];
			f_ptr = (mlib_s16 *)(src_s16_1);
			s_ptr = (mlib_s16 *)(src_s16_0 + 1);
			i++;
		} else {
			f_ptr = (mlib_s16 *)src_s16_0;
			s_ptr = (mlib_s16 *)src_s16_1;
		}

		if (((mlib_addr)(dst_s16_0 + i) & 7) && (i < (n - 1))) {
			dst_s16_0[i + 0] = (*f_ptr++);
			dst_s16_0[i + 1] = (*s_ptr++);
			i += 2;
		}

		dp = (mlib_u32 *)(dst_s16_0 + i);

		if ((((mlib_addr)f_ptr & 7) == 0) &&
		    (((mlib_addr)s_ptr & 7) == 0)) {

			for (; i < (n - 7); i += 8) {

				sd0 = ((mlib_d64 *)f_ptr)[0];
				sd1 = ((mlib_d64 *)s_ptr)[0];

				t0 = vis_fpmerge(vis_read_hi(sd0),
				    vis_read_lo(sd0));
				t1 = vis_fpmerge(vis_read_hi(sd1),
				    vis_read_lo(sd1));
				t0 = vis_fpmerge(vis_read_hi(t0),
				    vis_read_lo(t0));
				t1 = vis_fpmerge(vis_read_hi(t1),
				    vis_read_lo(t1));
				t2 = vis_fpmerge(vis_read_hi(t0),
				    vis_read_hi(t1));
				t3 = vis_fpmerge(vis_read_lo(t0),
				    vis_read_lo(t1));

				((mlib_d64 *)dp)[0] =
				    vis_fpmerge(vis_read_hi(t2),
				    vis_read_hi(t3));
				((mlib_d64 *)dp)[1] =
				    vis_fpmerge(vis_read_lo(t2),
				    vis_read_lo(t3));

				f_ptr += 4;
				s_ptr += 4;
				dp += 4;
			}
		} else if (((mlib_addr)f_ptr & 7) == ((mlib_addr)s_ptr & 7)) {

			sd0_ptr = VIS_ALIGNADDR(f_ptr, 0);
			sd1_ptr = VIS_ALIGNADDR(s_ptr, 0);

			s0h = (*sd0_ptr++);
			s1h = (*sd1_ptr++);

#pragma pipeloop(0)
			for (k = 0; i < (n - 7); i += 8, k++) {

				s0l = (*sd0_ptr++);
				s1l = (*sd1_ptr++);
				sd0 = vis_faligndata(s0h, s0l);
				sd1 = vis_faligndata(s1h, s1l);
				t0 = vis_fpmerge(vis_read_hi(sd0),
				    vis_read_lo(sd0));
				t1 = vis_fpmerge(vis_read_hi(sd1),
				    vis_read_lo(sd1));
				t0 = vis_fpmerge(vis_read_hi(t0),
				    vis_read_lo(t0));
				t1 = vis_fpmerge(vis_read_hi(t1),
				    vis_read_lo(t1));
				t2 = vis_fpmerge(vis_read_hi(t0),
				    vis_read_hi(t1));
				t3 = vis_fpmerge(vis_read_lo(t0),
				    vis_read_lo(t1));

				((mlib_d64 *)dp)[0] =
				    vis_fpmerge(vis_read_hi(t2),
				    vis_read_hi(t3));
				((mlib_d64 *)dp)[1] =
				    vis_fpmerge(vis_read_lo(t2),
				    vis_read_lo(t3));

				s0h = s0l;
				s1h = s1l;

				dp += 4;
			}

			f_ptr += (k << 2);
			s_ptr += (k << 2);
		} else {

			sd0_ptr = VIS_ALIGNADDR(f_ptr, 0);
			sd1_ptr = VIS_ALIGNADDR(s_ptr, 0);

			s0h = vis_ld_d64_nf(sd0_ptr); sd0_ptr++;
			s1h = vis_ld_d64_nf(sd1_ptr); sd1_ptr++;

#pragma pipeloop(0)
			for (k = 0; i < (n - 7); i += 8, k++) {

				VIS_ALIGNADDR(f_ptr, 0);
				s0l = vis_ld_d64_nf(sd0_ptr); sd0_ptr++;
				sd0 = vis_faligndata(s0h, s0l);

				VIS_ALIGNADDR(s_ptr, 0);
				s1l = vis_ld_d64_nf(sd1_ptr); sd1_ptr++;
				sd1 = vis_faligndata(s1h, s1l);

				t0 = vis_fpmerge(vis_read_hi(sd0),
				    vis_read_lo(sd0));
				t1 = vis_fpmerge(vis_read_hi(sd1),
				    vis_read_lo(sd1));
				t0 = vis_fpmerge(vis_read_hi(t0),
				    vis_read_lo(t0));
				t1 = vis_fpmerge(vis_read_hi(t1),
				    vis_read_lo(t1));
				t2 = vis_fpmerge(vis_read_hi(t0),
				    vis_read_hi(t1));
				t3 = vis_fpmerge(vis_read_lo(t0),
				    vis_read_lo(t1));

				((mlib_d64 *)dp)[0] =
				    vis_fpmerge(vis_read_hi(t2),
				    vis_read_hi(t3));
				((mlib_d64 *)dp)[1] =
				    vis_fpmerge(vis_read_lo(t2),
				    vis_read_lo(t3));

				s0h = s0l;
				s1h = s1l;

				dp += 4;
			}

			f_ptr += (k << 2);
			s_ptr += (k << 2);
		}

		for (; i < (n - 1); i += 2) {
			s0 = (mlib_u16)((*f_ptr++));
			s1 = (mlib_u16)((*s_ptr++));
			(*dp++) = (s0 << 16) + s1;
		}

		if (i < n)
			dst_s16_0[i] = *f_ptr;

		src_s16_0 += src0_stride;
		src_s16_1 += src1_stride;
		dst_s16_0 += dst_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge3_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    const mlib_s16 *src_s16_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride)
{
	mlib_s32 i, j, k, n = width * 3;
	mlib_f32 *dp;
	mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2;
	mlib_f32 f11 = vis_to_float(0x1000100);

	vis_write_gsr(8 << 3);

	for (j = 0; j < height; j++) {

		i = 0;
		k = 0;

		for (; (i < (n - 2)) && ((mlib_addr)(dst_s16_0 + i) & 7);
		    i += 3, k++) {
			dst_s16_0[i + 0] = src_s16_0[k];
			dst_s16_0[i + 1] = src_s16_1[k];
			dst_s16_0[i + 2] = src_s16_2[k];
		}

		dp = (mlib_f32 *)(dst_s16_0 + i);

		if (((mlib_addr)(src_s16_0 + k) & 7) ||
		    ((mlib_addr)(src_s16_1 + k) & 7) ||
		    ((mlib_addr)(src_s16_2 + k) & 7)) {
			mlib_d64 s0h, s0l, s1h, s1l, s2h, s2l;
			mlib_d64 *sp0;
			mlib_d64 *sp1;
			mlib_d64 *sp2;

			sp0 = VIS_ALIGNADDR((src_s16_0 + k), 0);
			s0h = vis_ld_d64_nf(sp0); sp0++;
			sp1 = VIS_ALIGNADDR((src_s16_1 + k), 0);
			s1h = vis_ld_d64_nf(sp1); sp1++;
			sp2 = VIS_ALIGNADDR((src_s16_2 + k), 0);
			s2h = vis_ld_d64_nf(sp2); sp2++;

#pragma pipeloop(0)
			for (; i < (n - 11); i += 12, k += 4) {
				s0l = vis_ld_d64_nf(sp0); sp0++;
				s1l = vis_ld_d64_nf(sp1); sp1++;
				s2l = vis_ld_d64_nf(sp2); sp2++;

				VIS_ALIGNADDR((src_s16_0 + k), 0);
				sd0 = vis_faligndata(s0h, s0l);
				VIS_ALIGNADDR((src_s16_1 + k), 0);
				sd1 = vis_faligndata(s1h, s1l);
				VIS_ALIGNADDR((src_s16_2 + k), 0);
				sd2 = vis_faligndata(s2h, s2l);

				sd0 =
				    vis_fpmerge(vis_read_hi(sd0),
				    vis_read_lo(sd0));
				sd1 =
				    vis_fpmerge(vis_read_hi(sd1),
				    vis_read_lo(sd1));
				sd2 =
				    vis_fpmerge(vis_read_hi(sd2),
				    vis_read_lo(sd2));
				dd0 =
				    vis_fpmerge(vis_read_hi(sd0),
				    vis_read_hi(sd1));
				dd1 =
				    vis_fpmerge(vis_read_hi(sd2),
				    vis_read_lo(sd0));
				dd2 =
				    vis_fpmerge(vis_read_lo(sd1),
				    vis_read_lo(sd2));
				dd0 =
				    vis_fpmerge(vis_read_hi(dd0),
				    vis_read_lo(dd0));
				dd1 =
				    vis_fpmerge(vis_read_hi(dd1),
				    vis_read_lo(dd1));
				dd2 =
				    vis_fpmerge(vis_read_hi(dd2),
				    vis_read_lo(dd2));

				((mlib_d64 *)dp)[0] =
				    vis_fpackfix_pair(vis_fmuld8sux16(f11,
				    vis_read_hi(dd0)), vis_fmuld8sux16(f11,
				    vis_read_hi(dd1)));
				((mlib_d64 *)dp)[1] =
				    vis_fpackfix_pair(vis_fmuld8sux16(f11,
				    vis_read_hi(dd2)), vis_fmuld8sux16(f11,
				    vis_read_lo(dd0)));
				dp[4] = vis_read_lo(dd1);
				dp[5] = vis_read_lo(dd2);
				dp += 6;
				s0h = s0l;
				s1h = s1l;
				s2h = s2l;
			}
		} else {

#pragma pipeloop(0)
			for (; i < (n - 11); i += 12, k += 4) {
				sd0 = *((mlib_d64 *)(src_s16_0 + k));
				sd1 = *((mlib_d64 *)(src_s16_1 + k));
				sd2 = *((mlib_d64 *)(src_s16_2 + k));
				sd0 =
				    vis_fpmerge(vis_read_hi(sd0),
				    vis_read_lo(sd0));
				sd1 =
				    vis_fpmerge(vis_read_hi(sd1),
				    vis_read_lo(sd1));
				sd2 =
				    vis_fpmerge(vis_read_hi(sd2),
				    vis_read_lo(sd2));
				dd0 =
				    vis_fpmerge(vis_read_hi(sd0),
				    vis_read_hi(sd1));
				dd1 =
				    vis_fpmerge(vis_read_hi(sd2),
				    vis_read_lo(sd0));
				dd2 =
				    vis_fpmerge(vis_read_lo(sd1),
				    vis_read_lo(sd2));
				dd0 =
				    vis_fpmerge(vis_read_hi(dd0),
				    vis_read_lo(dd0));
				dd1 =
				    vis_fpmerge(vis_read_hi(dd1),
				    vis_read_lo(dd1));
				dd2 =
				    vis_fpmerge(vis_read_hi(dd2),
				    vis_read_lo(dd2));

				((mlib_d64 *)dp)[0] =
				    vis_fpackfix_pair(vis_fmuld8sux16(f11,
				    vis_read_hi(dd0)), vis_fmuld8sux16(f11,
				    vis_read_hi(dd1)));
				((mlib_d64 *)dp)[1] =
				    vis_fpackfix_pair(vis_fmuld8sux16(f11,
				    vis_read_hi(dd2)), vis_fmuld8sux16(f11,
				    vis_read_lo(dd0)));
				dp[4] = vis_read_lo(dd1);
				dp[5] = vis_read_lo(dd2);
				dp += 6;
			}
		}

		for (; i < (n - 2); i += 3, k++) {
			dst_s16_0[i + 0] = src_s16_0[k];
			dst_s16_0[i + 1] = src_s16_1[k];
			dst_s16_0[i + 2] = src_s16_2[k];
		}

		dst_s16_0 += dst_stride;
		src_s16_0 += src0_stride;
		src_s16_1 += src1_stride;
		src_s16_2 += src2_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelMerge4_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    const mlib_s16 *src_s16_2,
    const mlib_s16 *src_s16_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride)
{
	mlib_s32 i, j, n = width << 2;
	mlib_s16 *fi_ptr, *se_ptr, *th_ptr, *fo_ptr;
	mlib_d64 *dp;

	for (j = 0; j < height; j++) {

		i = 0;

		if ((mlib_addr)(dst_s16_0 + i) & 7) {
			dst_s16_0[i++] = src_s16_0[0];

			if ((mlib_addr)(dst_s16_0 + i) & 7) {
				dst_s16_0[i++] = src_s16_1[0];

				if ((mlib_addr)(dst_s16_0 + i) & 7) {
					dst_s16_0[i++] = src_s16_2[0];
				}
			}
		}

		if (i == 0) {
			fi_ptr = (mlib_s16 *)src_s16_0;
			se_ptr = (mlib_s16 *)src_s16_1;
			th_ptr = (mlib_s16 *)src_s16_2;
			fo_ptr = (mlib_s16 *)src_s16_3;
		} else if (i == 1) {
			fi_ptr = (mlib_s16 *)src_s16_1;
			se_ptr = (mlib_s16 *)src_s16_2;
			th_ptr = (mlib_s16 *)src_s16_3;
			fo_ptr = (mlib_s16 *)(src_s16_0 + 1);
		} else if (i == 2) {
			fi_ptr = (mlib_s16 *)src_s16_2;
			se_ptr = (mlib_s16 *)src_s16_3;
			th_ptr = (mlib_s16 *)(src_s16_0 + 1);
			fo_ptr = (mlib_s16 *)(src_s16_1 + 1);
		} else if (i == 3) {
			fi_ptr = (mlib_s16 *)src_s16_3;
			se_ptr = (mlib_s16 *)(src_s16_0 + 1);
			th_ptr = (mlib_s16 *)(src_s16_1 + 1);
			fo_ptr = (mlib_s16 *)(src_s16_2 + 1);
		}

		dp = (mlib_d64 *)(dst_s16_0 + i);

		if ((n - i) > 16) {
			if (((mlib_addr)fi_ptr & 7) ||
			    ((mlib_addr)se_ptr & 7) ||
			    ((mlib_addr)th_ptr & 7) ||
			    ((mlib_addr)fo_ptr & 7)) {

				mlib_d64 sd0, sd1, sd2, sd3;
				mlib_d64 dd0, dd1, dd2, dd3, dr02, dr13;
				mlib_d64 s0h, s0l, s1h, s1l, s2h, s2l, s3h, s3l;
				mlib_d64 *sp0;
				mlib_d64 *sp1;
				mlib_d64 *sp2;
				mlib_d64 *sp3;

				sp0 = VIS_ALIGNADDR(fi_ptr, 0);
				s0h = (*sp0++);
				sp1 = VIS_ALIGNADDR(se_ptr, 0);
				s1h = (*sp1++);
				sp2 = VIS_ALIGNADDR(th_ptr, 0);
				s2h = (*sp2++);
				sp3 = VIS_ALIGNADDR(fo_ptr, 0);
				s3h = (*sp3++);

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {
					s0l = vis_ld_d64_nf(sp0); sp0++;
					s1l = vis_ld_d64_nf(sp1); sp1++;
					s2l = vis_ld_d64_nf(sp2); sp2++;
					s3l = vis_ld_d64_nf(sp3); sp3++;

					VIS_ALIGNADDR(fi_ptr, 0);
					sd0 = vis_faligndata(s0h, s0l);

					VIS_ALIGNADDR(se_ptr, 0);
					sd1 = vis_faligndata(s1h, s1l);

					VIS_ALIGNADDR(th_ptr, 0);
					sd2 = vis_faligndata(s2h, s2l);

					VIS_ALIGNADDR(fo_ptr, 0);
					sd3 = vis_faligndata(s3h, s3l);

					dr02 =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_hi(sd2));
					dr13 =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_hi(sd3));
					dd0 =
					    vis_fpmerge(vis_read_hi(dr02),
					    vis_read_hi(dr13));
					dp[0] =
					    vis_fpmerge(vis_read_hi(dd0),
					    vis_read_lo(dd0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dr02),
					    vis_read_lo(dr13));
					dp[1] =
					    vis_fpmerge(vis_read_hi(dd1),
					    vis_read_lo(dd1));
					dr02 =
					    vis_fpmerge(vis_read_lo(sd0),
					    vis_read_lo(sd2));
					dr13 =
					    vis_fpmerge(vis_read_lo(sd1),
					    vis_read_lo(sd3));
					dd2 =
					    vis_fpmerge(vis_read_hi(dr02),
					    vis_read_hi(dr13));
					dp[2] =
					    vis_fpmerge(vis_read_hi(dd2),
					    vis_read_lo(dd2));
					dd3 =
					    vis_fpmerge(vis_read_lo(dr02),
					    vis_read_lo(dr13));
					dp[3] =
					    vis_fpmerge(vis_read_hi(dd3),
					    vis_read_lo(dd3));
					dp += 4;

					s0h = s0l;
					s1h = s1l;
					s2h = s2l;
					s3h = s3l;

					fi_ptr += 4;
					se_ptr += 4;
					th_ptr += 4;
					fo_ptr += 4;
				}
			} else {

				mlib_d64 sd0, sd1, sd2, sd3;
				mlib_d64 dd0, dd1, dd2, dd3, dr02, dr13;

#pragma pipeloop(0)
				for (; i < (n - 15); i += 16) {

					sd0 = ((mlib_d64 *)fi_ptr)[0];
					sd1 = ((mlib_d64 *)se_ptr)[0];
					sd2 = ((mlib_d64 *)th_ptr)[0];
					sd3 = ((mlib_d64 *)fo_ptr)[0];

					dr02 =
					    vis_fpmerge(vis_read_hi(sd0),
					    vis_read_hi(sd2));
					dr13 =
					    vis_fpmerge(vis_read_hi(sd1),
					    vis_read_hi(sd3));
					dd0 =
					    vis_fpmerge(vis_read_hi(dr02),
					    vis_read_hi(dr13));
					dp[0] =
					    vis_fpmerge(vis_read_hi(dd0),
					    vis_read_lo(dd0));
					dd1 =
					    vis_fpmerge(vis_read_lo(dr02),
					    vis_read_lo(dr13));
					dp[1] =
					    vis_fpmerge(vis_read_hi(dd1),
					    vis_read_lo(dd1));
					dr02 =
					    vis_fpmerge(vis_read_lo(sd0),
					    vis_read_lo(sd2));
					dr13 =
					    vis_fpmerge(vis_read_lo(sd1),
					    vis_read_lo(sd3));
					dd2 =
					    vis_fpmerge(vis_read_hi(dr02),
					    vis_read_hi(dr13));
					dp[2] =
					    vis_fpmerge(vis_read_hi(dd2),
					    vis_read_lo(dd2));
					dd3 =
					    vis_fpmerge(vis_read_lo(dr02),
					    vis_read_lo(dr13));
					dp[3] =
					    vis_fpmerge(vis_read_hi(dd3),
					    vis_read_lo(dd3));
					dp += 4;

					fi_ptr += 4;
					se_ptr += 4;
					th_ptr += 4;
					fo_ptr += 4;
				}
			}
		}
#pragma pipeloop(0)
		for (; i < (n - 3); i += 4) {
			dst_s16_0[i + 0] = (*fi_ptr++);
			dst_s16_0[i + 1] = (*se_ptr++);
			dst_s16_0[i + 2] = (*th_ptr++);
			dst_s16_0[i + 3] = (*fo_ptr++);
		}

		if (i < (n - 2)) {
			dst_s16_0[i + 0] = *fi_ptr;
			dst_s16_0[i + 1] = *se_ptr;
			dst_s16_0[i + 2] = *th_ptr;
		} else if (i < (n - 1)) {
			dst_s16_0[i + 0] = *fi_ptr;
			dst_s16_0[i + 1] = *se_ptr;
		} else if (i < n) {
			dst_s16_0[i + 0] = *fi_ptr;
		}

		dst_s16_0 += dst_stride;
		src_s16_0 += src0_stride;
		src_s16_1 += src1_stride;
		src_s16_2 += src2_stride;
		src_s16_3 += src3_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
