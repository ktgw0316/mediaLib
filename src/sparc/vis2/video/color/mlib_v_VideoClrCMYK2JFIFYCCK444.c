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

#pragma ident	"@(#)mlib_v_VideoClrCMYK2JFIFYCCK444.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorCMYK2JFIFYCCK444 - color conversion from CMYK to JFIF
 *                                        YCrCbK space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorCMYK2JFIFYCCK444(mlib_u8       *y,
 *                                                  mlib_u8       *cb,
 *                                                  mlib_u8       *cr,
 *                                                  mlib_u8       *k,
 *                                                  const mlib_u8 *cmyk,
 *                                                  mlib_s32      n);
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      k         Pointer to K component row, 8-byte aligned
 *      cmyk      Pointer to CMYK multicomponent row, 8-byte aligned
 *      n         Length of y, cb, cr, k arrays
 *                Note: length of cmyk array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorCMYK2JFIFYCCK444
 *        R = (255 - C),  G = (255 - M),  B = (255 - Y)
 *
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        Y = y[i], Cb = cb[i], Cr = cr[i]
 *        C = cmyk[4*i], M = cmyk[4*i+1], Y = cmyk[4*i+2]
 *
 *        K = k[i] = cmyk[4*i+3] - should be passed unchanged
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorCMYK2JFIFYCCK444 = \
	__mlib_VideoColorCMYK2JFIFYCCK444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorCMYK2JFIFYCCK444) mlib_VideoColorCMYK2JFIFYCCK444
	__attribute__((weak, alias("__mlib_VideoColorCMYK2JFIFYCCK444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHANNELWEIGHT_U8(rf, gf, bf, alpha, beta, gamma, off, df)   \
	dr = vis_fmul8x16al(rf, alpha);                             \
	dg = vis_fmul8x16al(gf, beta);                              \
	db = vis_fmul8x16al(bf, gamma);                             \
	dd = vis_fpadd16(dr, off);                                  \
	dd = vis_fpadd16(dd, dg);                                   \
	dd = vis_fpadd16(dd, db);                                   \
	df = vis_fpack16(dd)

/* *********************************************************** */

#define	K11	-0.29900
#define	K12	-0.58700
#define	K13	-0.11400
#define	K21	0.16874
#define	K22	0.33126
#define	K23	-0.50000
#define	K31	-0.50000
#define	K32	0.41869
#define	K33	0.08131

/* *********************************************************** */

mlib_status
__mlib_VideoColorCMYK2JFIFYCCK444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n)
{
	mlib_f32 *py, *pcb, *pcr, *pk;
	mlib_d64 *dcmyk = (mlib_d64 *)cmyk;
	mlib_d64 sdh, sdl, dr, dg, db, dd;
	mlib_s32 i, m;
	mlib_d64 sd0, sd1;
	mlib_d64 sd2, sd3, sda, sdb;

	mlib_f32 k11 = vis_to_float((mlib_s32)(K11 * 8192));
	mlib_f32 k12 = vis_to_float((mlib_s32)(K12 * 8192));
	mlib_f32 k13 = vis_to_float((mlib_s32)(K13 * 8192));
	mlib_f32 k21 = vis_to_float((mlib_s32)(K21 * 8192));
	mlib_f32 k22 = vis_to_float((mlib_s32)(K22 * 8192));
	mlib_f32 k23 = vis_to_float((mlib_s32)(K23 * 8192));
	mlib_f32 k31 = vis_to_float((mlib_s32)(K31 * 8192));
	mlib_f32 k32 = vis_to_float((mlib_s32)(K32 * 8192));
	mlib_f32 k33 = vis_to_float((mlib_s32)(K33 * 8192));
	mlib_d64 off128 = vis_to_double_dup(0x10101010);
	mlib_d64 off255 = vis_to_double_dup(0x1ff01ff0);

	vis_write_gsr(2 << 3);

	m = (n / 4) & ~1;
	py = (mlib_f32 *)y;
	pcb = (mlib_f32 *)cb;
	pcr = (mlib_f32 *)cr;
	pk = (mlib_f32 *)k;
#pragma pipeloop(0)
	for (i = 0; i <= m - 1; i += 2) {
		sd0 = dcmyk[0];
		sd1 = dcmyk[1];
		sd2 = dcmyk[2];
		sd3 = dcmyk[3];
		vis_write_bmask(0x048c159d, 0);
		sdh = vis_bshuffle(sd0, sd1);
		sda = vis_bshuffle(sd2, sd3);
		vis_write_bmask(0x26ae37bf, 0);
		sdl = vis_bshuffle(sd0, sd1);
		sdb = vis_bshuffle(sd2, sd3);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k11, k12, k13, off255, py[0]);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k21, k22, k23, off128, pcb[0]);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k31, k32, k33, off128, pcr[0]);
		CHANNELWEIGHT_U8(vis_read_hi(sda), vis_read_lo(sda),
			vis_read_hi(sdb), k11, k12, k13, off255, py[1]);
		CHANNELWEIGHT_U8(vis_read_hi(sda), vis_read_lo(sda),
			vis_read_hi(sdb), k21, k22, k23, off128, pcb[1]);
		CHANNELWEIGHT_U8(vis_read_hi(sda), vis_read_lo(sda),
			vis_read_hi(sdb), k31, k32, k33, off128, pcr[1]);
		py += 2;
		pcb += 2;
		pcr += 2;
		pk[0] = vis_read_lo(sdl);
		pk[1] = vis_read_lo(sdb);
		pk += 2;
		dcmyk += 4;
	}

	if (i < m) {
		sd0 = dcmyk[0];
		sd1 = dcmyk[1];
		vis_write_bmask(0x048c159d, 0);
		sdh = vis_bshuffle(sd0, sd1);
		vis_write_bmask(0x26ae37bf, 0);
		sdl = vis_bshuffle(sd0, sd1);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k11, k12, k13, off255, py[0]);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k21, k22, k23, off128, pcb[0]);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k31, k32, k33, off128, pcr[0]);
		py++;
		pcb++;
		pcr++;
		(*pk++) = vis_read_lo(sdl);
		dcmyk += 2;
	}

	if (n & 7) {
		mlib_s32 emask = (0xFF00 >> (n & 7)) & 0xFF;
		mlib_d64 rbuff[4];
		mlib_f32 *prbuff = (mlib_f32 *)rbuff;

		sd0 = dcmyk[0];
		sd1 = vis_ld_d64_nf(dcmyk + 1);
		sd2 = vis_ld_d64_nf(dcmyk + 2);
		sd3 = vis_ld_d64_nf(dcmyk + 3);
		vis_write_bmask(0x048c159d, 0);
		sdh = vis_bshuffle(sd0, sd1);
		sda = vis_bshuffle(sd2, sd3);
		vis_write_bmask(0x26ae37bf, 0);
		sdl = vis_bshuffle(sd0, sd1);
		sdb = vis_bshuffle(sd2, sd3);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k11, k12, k13, off255, prbuff[0]);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k21, k22, k23, off128, prbuff[2]);
		CHANNELWEIGHT_U8(vis_read_hi(sdh), vis_read_lo(sdh),
			vis_read_hi(sdl), k31, k32, k33, off128, prbuff[4]);
		CHANNELWEIGHT_U8(vis_read_hi(sda), vis_read_lo(sda),
			vis_read_hi(sdb), k11, k12, k13, off255, prbuff[1]);
		CHANNELWEIGHT_U8(vis_read_hi(sda), vis_read_lo(sda),
			vis_read_hi(sdb), k21, k22, k23, off128, prbuff[3]);
		CHANNELWEIGHT_U8(vis_read_hi(sda), vis_read_lo(sda),
			vis_read_hi(sdb), k31, k32, k33, off128, prbuff[5]);
		prbuff[6] = vis_read_lo(sdl);
		prbuff[7] = vis_read_lo(sdb);

		vis_pst_8(rbuff[0], py, emask);
		vis_pst_8(rbuff[1], pcb, emask);
		vis_pst_8(rbuff[2], pcr, emask);
		vis_pst_8(rbuff[3], pk, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
