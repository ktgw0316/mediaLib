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

#pragma ident	"@(#)mlib_v_VideoColorBGR2JFIFYCC420.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBGR2JFIFYCC420 - color conversion with
 *                                      420 downsampling
 *                                      with bilinear filter
 *                                      Only Cb and Cr components are
 *                                      1under upsampling.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorBGR2JFIFYCC420(mlib_u8 *y0,
 *                                                mlib_u8 *y1,
 *                                                mlib_u8 *cb,
 *                                                mlib_u8 *cr,
 *                                                const mlib_u8 *rgb0,
 *                                                const mlib_u8 *rgb1,
 *                                                mlib_s32 n);
 * ARGUMENTS
 *      y0        Pointer to upper Y component row, 8-byte aligned
 *      y1        Pointer to lower Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb0      Pointer to upper BGR multicomponent row, 8-byte aligned
 *      rgb1      Pointer to lower BGR multicomponent row, 8-byte aligned
 *      n         Length of y0 and y1 arrays
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb0, rgb1 arrays must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorBGR2JFIFYCC420
 *        composition of mlib_VideoColorBGR2JFIFYCC444
 *        with mlib_VideoDownSample420 on Cb and Cr:
 *
 *        mlib_VideoColorBGR2JFIFYCC444(y0, cb0, cr0, rgb0, n);
 *        mlib_VideoColorBGR2JFIFYCC444(y1, cb1, cr1, rgb0, n);
 *        mlib_VideoDownSample420(cb, cb0, cb1, n);
 *        mlib_VideoDownSample420(cr, cr0, cr1, n);
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorBGR2JFIFYCC420 = __mlib_VideoColorBGR2JFIFYCC420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBGR2JFIFYCC420) mlib_VideoColorBGR2JFIFYCC420
	__attribute__((weak, alias("__mlib_VideoColorBGR2JFIFYCC420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHANNELSEPARATE_U8_422(sd0, sd1, sd2, b0g0, r0b1, g1r1)   \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));    \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));    \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));    \
	b0g0 = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	r0b1 = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	g1r1 = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELWEIGHT_U8_2p(rh, gh, bh, rl, gl, bl, alpha,      \
	beta, gamma, dd0, dd1)                                  \
	{                                                       \
	    mlib_d64 dd, ddt;                                   \
	                                                        \
	    dd = vis_fmul8x16al(rh, alpha);                     \
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
__mlib_VideoColorBGR2JFIFYCC420(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb0,
	const mlib_u8 *rgb1,
	mlib_s32 n)
{
	mlib_d64 *sp0 = (mlib_d64 *)rgb0;
	mlib_d64 *sp1 = (mlib_d64 *)rgb1;
	mlib_d64 *py0 = (mlib_d64 *)y0;
	mlib_d64 *py1 = (mlib_d64 *)y1;
	mlib_f32 *pcb = (mlib_f32 *)cb;
	mlib_f32 *pcr = (mlib_f32 *)cr;
	mlib_u8 *y0end = y0 + n;
	mlib_u8 *cbend = cb + (n >> 1);
	mlib_d64 sd0, sd1, sd2, se0, se1, se2;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 b0g0, r0b1, g1r1, z0, z1;
	mlib_d64 _b0g0, _r0b1, _g1r1;
	mlib_d64 yd0, yd1, cb0, cb1, cr0, cr1;
	mlib_s32 i;

	mlib_f32 k11 = vis_to_float((mlib_s32)(K11 * 8192));
	mlib_f32 k12 = vis_to_float((mlib_s32)(K12 * 8192));
	mlib_f32 k13 = vis_to_float((mlib_s32)(K13 * 8192));
	mlib_f32 k21 = vis_to_float((mlib_s32)(K21 * 2048));
	mlib_f32 k22 = vis_to_float((mlib_s32)(K22 * 2048));
	mlib_f32 k23 = vis_to_float((mlib_s32)(K23 * 2048));
	mlib_f32 k31 = vis_to_float((mlib_s32)(K31 * 2048));
	mlib_f32 k32 = vis_to_float((mlib_s32)(K32 * 2048));
	mlib_f32 k33 = vis_to_float((mlib_s32)(K33 * 2048));
	mlib_d64 off128 = vis_to_double_dup(0x10101010);
	mlib_d64 off0 = vis_to_double_dup(0x00100010);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(2 << 3);

	n = n >> 3;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		sd0 = (*sp0++);
		sd1 = (*sp0++);
		sd2 = (*sp0++);
		se0 = (*sp1++);
		se1 = (*sp1++);
		se2 = (*sp1++);
		CHANNELSEPARATE_U8_422(sd0, sd1, sd2, b0g0, r0b1, g1r1);
		CHANNELSEPARATE_U8_422(se0, se1, se2, _b0g0, _r0b1, _g1r1);

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0b1), vis_read_lo(b0g0),
			vis_read_hi(b0g0), vis_read_lo(g1r1), vis_read_hi(g1r1),
			vis_read_lo(r0b1), k11, k12, k13, z0, z1);
		z0 = vis_fpadd16(z0, off0);
		z1 = vis_fpadd16(z1, off0);
		yd0 = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));
		py0[0] = yd0;

		CHANNELWEIGHT_U8_2p(vis_read_hi(_r0b1), vis_read_lo(_b0g0),
			vis_read_hi(_b0g0), vis_read_lo(_g1r1),
			vis_read_hi(_g1r1), vis_read_lo(_r0b1), k11, k12, k13,
			z0, z1);
		z0 = vis_fpadd16(z0, off0);
		z1 = vis_fpadd16(z1, off0);
		yd1 = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));
		py1[0] = yd1;

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0b1), vis_read_lo(b0g0),
			vis_read_hi(b0g0), vis_read_lo(g1r1), vis_read_hi(g1r1),
			vis_read_lo(r0b1), k21, k22, k23, z0, z1);
		cb0 = vis_fpadd16(z0, z1);
		CHANNELWEIGHT_U8_2p(vis_read_hi(_r0b1), vis_read_lo(_b0g0),
			vis_read_hi(_b0g0), vis_read_lo(_g1r1),
			vis_read_hi(_g1r1), vis_read_lo(_r0b1), k21, k22, k23,
			z0, z1);
		cb1 = vis_fpadd16(z0, z1);
		cb0 = vis_fpadd16(cb0, off128);
		cb0 = vis_fpadd16(cb0, cb1);
		pcb[0] = vis_fpack16(cb0);

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0b1), vis_read_lo(b0g0),
			vis_read_hi(b0g0), vis_read_lo(g1r1), vis_read_hi(g1r1),
			vis_read_lo(r0b1), k31, k32, k33, z0, z1);
		cr0 = vis_fpadd16(z0, z1);

		CHANNELWEIGHT_U8_2p(vis_read_hi(_r0b1), vis_read_lo(_b0g0),
			vis_read_hi(_b0g0), vis_read_lo(_g1r1),
			vis_read_hi(_g1r1), vis_read_lo(_r0b1), k31, k32, k33,
			z0, z1);
		cr1 = vis_fpadd16(z0, z1);
		cr0 = vis_fpadd16(cr0, off128);
		cr0 = vis_fpadd16(cr0, cr1);
		pcr[0] = vis_fpack16(cr0);

		py0++;
		py1++;
		pcb++;
		pcr++;
	}

	if ((mlib_u8 *)pcb < cbend) {
		mlib_d64 yd0, yd1, cb0, cb1, cr0, cr1;
		mlib_f32 cbf, crf;
		mlib_s32 ymask, cmask;

		sd0 = (*sp0++);
		sd1 = vis_ld_d64_nf(sp0); sp0++;
		sd2 = vis_ld_d64_nf(sp0);
		se0 = (*sp1++);
		se1 = vis_ld_d64_nf(sp1); sp1++;
		se2 = vis_ld_d64_nf(sp1);
		CHANNELSEPARATE_U8_422(sd0, sd1, sd2, b0g0, r0b1, g1r1);
		CHANNELSEPARATE_U8_422(se0, se1, se2, _b0g0, _r0b1, _g1r1);

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0b1), vis_read_lo(b0g0),
			vis_read_hi(b0g0), vis_read_lo(g1r1), vis_read_hi(g1r1),
			vis_read_lo(r0b1), k11, k12, k13, z0, z1);
		z0 = vis_fpadd16(z0, off0);
		z1 = vis_fpadd16(z1, off0);
		yd0 = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));

		CHANNELWEIGHT_U8_2p(vis_read_hi(_r0b1), vis_read_lo(_b0g0),
			vis_read_hi(_b0g0), vis_read_lo(_g1r1),
			vis_read_hi(_g1r1), vis_read_lo(_r0b1), k11, k12, k13,
			z0, z1);
		z0 = vis_fpadd16(z0, off0);
		z1 = vis_fpadd16(z1, off0);
		yd1 = vis_fpmerge(vis_fpack16(z0), vis_fpack16(z1));

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0b1), vis_read_lo(b0g0),
			vis_read_hi(b0g0), vis_read_lo(g1r1), vis_read_hi(g1r1),
			vis_read_lo(r0b1), k21, k22, k23, z0, z1);
		cb0 = vis_fpadd16(z0, z1);
		CHANNELWEIGHT_U8_2p(vis_read_hi(_r0b1), vis_read_lo(_b0g0),
			vis_read_hi(_b0g0), vis_read_lo(_g1r1),
			vis_read_hi(_g1r1), vis_read_lo(_r0b1), k21, k22, k23,
			z0, z1);
		cb1 = vis_fpadd16(z0, z1);
		cb0 = vis_fpadd16(cb0, off128);
		cb0 = vis_fpadd16(cb0, cb1);
		cbf = vis_fpack16(cb0);

		CHANNELWEIGHT_U8_2p(vis_read_hi(r0b1), vis_read_lo(b0g0),
			vis_read_hi(b0g0), vis_read_lo(g1r1), vis_read_hi(g1r1),
			vis_read_lo(r0b1), k31, k32, k33, z0, z1);
		cr0 = vis_fpadd16(z0, z1);

		CHANNELWEIGHT_U8_2p(vis_read_hi(_r0b1), vis_read_lo(_b0g0),
			vis_read_hi(_b0g0), vis_read_lo(_g1r1),
			vis_read_hi(_g1r1), vis_read_lo(_r0b1), k31, k32, k33,
			z0, z1);
		cr1 = vis_fpadd16(z0, z1);
		cr0 = vis_fpadd16(cr0, off128);
		cr0 = vis_fpadd16(cr0, cr1);
		crf = vis_fpack16(cr0);

		ymask = vis_edge8(py0, y0end - 1);
		vis_pst_8(yd0, py0, ymask);
		vis_pst_8(yd1, py1, ymask);

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
