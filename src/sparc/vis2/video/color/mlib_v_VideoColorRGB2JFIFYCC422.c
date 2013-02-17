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

#pragma ident	"@(#)mlib_v_VideoColorRGB2JFIFYCC422.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2JFIFYCC422 - color conversion with
 *                                      422 downsampling
 *                                      with bilinear filter
 *                                      Only Cr and Cb components are
 *                                      under upsampling.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorRGB2JFIFYCC422(mlib_u8       *y,
 *                                                  mlib_u8       *cb,
 *                                                  mlib_u8       *cr,
 *                                                  const mlib_u8 *rgb,
 *                                                  mlib_s32      n);
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorRGB2JFIFYCC422:
 *        composition of mlib_VideoColorRGB2JFIFYCC444 with
 *                       mlib_VideoDownSample422
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2JFIFYCC422 = __mlib_VideoColorRGB2JFIFYCC422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2JFIFYCC422) mlib_VideoColorRGB2JFIFYCC422
	__attribute__((weak, alias("__mlib_VideoColorRGB2JFIFYCC422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHANNELSEPARATE_U8_422(sd0, sd1, sd2, r0g0, b0r1, g1b1)   \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));    \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));    \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));    \
	r0g0 = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	b0r1 = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	g1b1 = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELWEIGHT_U8_2p(rh, gh, bh, rl, gl, bl, alpha,      \
	beta, gamma, off, dd0, dd1)                             \
	{                                                       \
	    mlib_d64 dd, ddt;                                   \
	                                                        \
	    ddt = vis_fmul8x16al(rh, alpha);                    \
	    dd = vis_fpadd16(off, ddt);                         \
	    ddt = vis_fmul8x16al(gh, beta);                     \
	    dd = vis_fpadd16(dd, ddt);                          \
	    ddt = vis_fmul8x16al(bh, gamma);                    \
	    dd0 = vis_fpadd16(dd, ddt);                         \
	    dd = vis_fmul8x16al(rl, alpha);                     \
	    ddt = vis_fmul8x16al(gl, beta);                     \
	    dd = vis_fpadd16(dd, ddt);                          \
	    ddt = vis_fmul8x16al(bl, gamma);                    \
	    dd1 = vis_fpadd16(dd, ddt);                         \
	}

/* *********************************************************** */

#define	K11	0.29900
#define	K12	0.58700
#define	K13	0.11400
#define	K21	-0.16874
#define	K22	-0.33126
#define	K23	0.50000
#define	K31	0.50000
#define	K32	-0.41869
#define	K33	-0.08131

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2JFIFYCC422(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)rgb;
	mlib_d64 *py = (mlib_d64 *)y;
	mlib_f32 *pcb = (mlib_f32 *)cb;
	mlib_f32 *pcr = (mlib_f32 *)cr;
	mlib_u8 *yend = y + n;
	mlib_u8 *cbend = cb + (n >> 1);
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 r0g0, b0r1, g1b1, z0, z1;
	mlib_s32 i;

	mlib_f32 k11 = vis_to_float((mlib_s32)(K11 * 8192));
	mlib_f32 k12 = vis_to_float((mlib_s32)(K12 * 8192));
	mlib_f32 k13 = vis_to_float((mlib_s32)(K13 * 8192));
	mlib_f32 k21 = vis_to_float((mlib_s32)(K21 * 4096));
	mlib_f32 k22 = vis_to_float((mlib_s32)(K22 * 4096));
	mlib_f32 k23 = vis_to_float((mlib_s32)(K23 * 4096));
	mlib_f32 k31 = vis_to_float((mlib_s32)(K31 * 4096));
	mlib_f32 k32 = vis_to_float((mlib_s32)(K32 * 4096));
	mlib_f32 k33 = vis_to_float((mlib_s32)(K33 * 4096));
	mlib_d64 off128 = vis_to_double_dup(0x10101010);
	mlib_d64 off0 = vis_to_double_dup(0x00100010);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(2 << 3);

	n = n >> 3;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		CHANNELSEPARATE_U8_422(sd0, sd1, sd2, r0g0, b0r1, g1b1);
		CHANNELWEIGHT_U8_2p(vis_read_hi(r0g0), vis_read_lo(r0g0),
			vis_read_hi(b0r1), vis_read_lo(b0r1), vis_read_hi(g1b1),
			vis_read_lo(g1b1), k11, k12, k13, off0, z0, z1);
		z1 = vis_fpadd16(z1, off0);
		py[0] = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0g0), vis_read_lo(r0g0),
			vis_read_hi(b0r1), vis_read_lo(b0r1), vis_read_hi(g1b1),
			vis_read_lo(g1b1), k21, k22, k23, off128, z0, z1);
		pcb[0] = vis_fpack16(vis_fpadd16(z0, z1));

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0g0), vis_read_lo(r0g0),
			vis_read_hi(b0r1), vis_read_lo(b0r1), vis_read_hi(g1b1),
			vis_read_lo(g1b1), k31, k32, k33, off128, z0, z1);
		pcr[0] = vis_fpack16(vis_fpadd16(z0, z1));

		py++;
		pcb++;
		pcr++;
	}

	if ((mlib_u8 *)pcb < cbend) {
		mlib_d64 yd;
		mlib_f32 cbf, crf;
		mlib_s32 ymask, cmask;

		sd0 = (*sp++);
		sd1 = vis_ld_d64_nf(sp); sp++;
		sd2 = vis_ld_d64_nf(sp);
		CHANNELSEPARATE_U8_422(sd0, sd1, sd2, r0g0, b0r1, g1b1);
		CHANNELWEIGHT_U8_2p(vis_read_hi(r0g0), vis_read_lo(r0g0),
			vis_read_hi(b0r1), vis_read_lo(b0r1), vis_read_hi(g1b1),
			vis_read_lo(g1b1), k11, k12, k13, off0, z0, z1);
		z1 = vis_fpadd16(z1, off0);
		yd = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0g0), vis_read_lo(r0g0),
			vis_read_hi(b0r1), vis_read_lo(b0r1), vis_read_hi(g1b1),
			vis_read_lo(g1b1), k21, k22, k23, off128, z0, z1);
		cbf = vis_fpack16(vis_fpadd16(z0, z1));

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0g0), vis_read_lo(r0g0),
			vis_read_hi(b0r1), vis_read_lo(b0r1), vis_read_hi(g1b1),
			vis_read_lo(g1b1), k31, k32, k33, off128, z0, z1);
		crf = vis_fpack16(vis_fpadd16(z0, z1));

		ymask = vis_edge8(py, yend - 1);
		vis_pst_8(yd, py, ymask);
		cmask = vis_edge8(pcb, cbend - 1);

		if (cmask & 0xf0) {
			vis_pst_8(vis_freg_pair(cbf, vis_fzeros()), pcb, cmask);
			vis_pst_8(vis_freg_pair(crf, vis_fzeros()), pcr, cmask);
		} else {
			vis_pst_8(vis_freg_pair(vis_fzeros(), cbf), pcb - 1,
				cmask);
			vis_pst_8(vis_freg_pair(vis_fzeros(), crf), pcr - 1,
				cmask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
