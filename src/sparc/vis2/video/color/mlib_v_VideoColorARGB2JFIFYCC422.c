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

#pragma ident	"@(#)mlib_v_VideoColorARGB2JFIFYCC422.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2JFIFYCC422 - color conversion with
 *                                       422 downsampling with bilinear filter
 *                                       Only Cr and Cb components
 *                                       are under upsampling.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorARGB2JFIFYCC422(mlib_u8  *y,
 *                                                 mlib_u8  *cb,
 *                                                 mlib_u8  *cr,
 *                                                 mlib_u8  *argb,
 *                                                 mlib_s32 n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorARGB2JFIFYCC422:
 *        composition of mlib_VideoColorARGB2JFIFYCC444 with
 *                       mlib_VideoDownSample422
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2JFIFYCC422 = \
	__mlib_VideoColorARGB2JFIFYCC422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2JFIFYCC422) mlib_VideoColorARGB2JFIFYCC422
	__attribute__((weak, alias("__mlib_VideoColorARGB2JFIFYCC422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHANNELSEPARATE_U8_422(sd01, sd23, sd45, sd67, dh0,         \
	dh1, dl0, dl1)                                              \
	sd04 = vis_fpmerge(vis_read_hi(sd01), vis_read_hi(sd45));   \
	sd26 = vis_fpmerge(vis_read_hi(sd23), vis_read_hi(sd67));   \
	sd15 = vis_fpmerge(vis_read_lo(sd01), vis_read_lo(sd45));   \
	sd37 = vis_fpmerge(vis_read_lo(sd23), vis_read_lo(sd67));   \
	dh0 = vis_fpmerge(vis_read_hi(sd04), vis_read_hi(sd26));    \
	dh1 = vis_fpmerge(vis_read_lo(sd04), vis_read_lo(sd26));    \
	dl0 = vis_fpmerge(vis_read_hi(sd15), vis_read_hi(sd37));    \
	dl1 = vis_fpmerge(vis_read_lo(sd15), vis_read_lo(sd37))

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
__mlib_VideoColorARGB2JFIFYCC422(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)argb, *py = (mlib_d64 *)y;
	mlib_f32 *pcb = (mlib_f32 *)cb, *pcr = (mlib_f32 *)cr;
	mlib_u8 *yend = y + n, *cbend = cb + (n >> 1);
	mlib_d64 sd01, sd23, sd45, sd67, sd04, sd26, sd15, sd37;
	mlib_d64 dh0, dh1, dl0, dl1, z0, z1;
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
		sd01 = (*sp++);
		sd23 = (*sp++);
		sd45 = (*sp++);
		sd67 = (*sp++);
		CHANNELSEPARATE_U8_422(sd01, sd23, sd45, sd67, dh0, dh1, dl0,
			dl1);
		CHANNELWEIGHT_U8_2p(vis_read_lo(dh0), vis_read_hi(dh1),
			vis_read_lo(dh1), vis_read_lo(dl0), vis_read_hi(dl1),
			vis_read_lo(dl1), k11, k12, k13, off0, z0, z1);
		z1 = vis_fpadd16(z1, off0);
		py[0] = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));

		CHANNELWEIGHT_U8_2p(vis_read_lo(dh0), vis_read_hi(dh1),
			vis_read_lo(dh1), vis_read_lo(dl0), vis_read_hi(dl1),
			vis_read_lo(dl1), k21, k22, k23, off128, z0, z1);
		pcb[0] = vis_fpack16(vis_fpadd16(z0, z1));

		CHANNELWEIGHT_U8_2p(vis_read_lo(dh0), vis_read_hi(dh1),
			vis_read_lo(dh1), vis_read_lo(dl0), vis_read_hi(dl1),
			vis_read_lo(dl1), k31, k32, k33, off128, z0, z1);
		pcr[0] = vis_fpack16(vis_fpadd16(z0, z1));

		py++;
		pcb++;
		pcr++;
	}

	if ((mlib_u8 *)pcb < cbend) {
		mlib_d64 yd;
		mlib_f32 cbf, crf;
		mlib_s32 ymask, cmask;

		sd01 = (*sp++);
		sd23 = vis_ld_d64_nf(sp); sp++;
		sd45 = vis_ld_d64_nf(sp); sp++;
		sd67 = vis_ld_d64_nf(sp);
		CHANNELSEPARATE_U8_422(sd01, sd23, sd45, sd67, dh0, dh1, dl0,
			dl1);
		CHANNELWEIGHT_U8_2p(vis_read_lo(dh0), vis_read_hi(dh1),
			vis_read_lo(dh1), vis_read_lo(dl0), vis_read_hi(dl1),
			vis_read_lo(dl1), k11, k12, k13, off0, z0, z1);
		z1 = vis_fpadd16(z1, off0);
		yd = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));

		CHANNELWEIGHT_U8_2p(vis_read_lo(dh0), vis_read_hi(dh1),
			vis_read_lo(dh1), vis_read_lo(dl0), vis_read_hi(dl1),
			vis_read_lo(dl1), k21, k22, k23, off128, z0, z1);
		cbf = vis_fpack16(vis_fpadd16(z0, z1));

		CHANNELWEIGHT_U8_2p(vis_read_lo(dh0), vis_read_hi(dh1),
			vis_read_lo(dh1), vis_read_lo(dl0), vis_read_hi(dl1),
			vis_read_lo(dl1), k31, k32, k33, off128, z0, z1);
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
