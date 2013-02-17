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

#pragma ident	"@(#)mlib_v_VideoColorARGB2JFIFYCC444.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2JFIFYCC444 - color conversion from ARGB to JFIF
 *                                       YCrCb space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorARGB2JFIFYCC444(mlib_u8       *y,
 *                                                 mlib_u8       *cb,
 *                                                 mlib_u8       *cr,
 *                                                 const mlib_u8 *argb,
 *                                                 mlib_s32      n);
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the argb array must be 4*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorARGB2JFIFYCC444
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        R  = argb[4*i+1], G = argb[4*i+2], B = argb[4*i+3]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2JFIFYCC444 = \
	__mlib_VideoColorARGB2JFIFYCC444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2JFIFYCC444) mlib_VideoColorARGB2JFIFYCC444
	__attribute__((weak, alias("__mlib_VideoColorARGB2JFIFYCC444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SIZE	1024

/* *********************************************************** */

#define	CHANNELSEPARATE_U8(sd0, sd1, sdh, sdl)                     \
	sd02 = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd1));    \
	sd13 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd1));    \
	sdh = vis_fpmerge(vis_read_hi(sd02), vis_read_hi(sd13));   \
	sdl = vis_fpmerge(vis_read_lo(sd02), vis_read_lo(sd13))

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

static void
mlib_channel_separate(
	const mlib_d64 *sp,
	mlib_d64 *dp,
	mlib_s32 n)
{
	mlib_d64 sd0, sd1, sd02, sd13;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		sd0 = vis_ld_d64_nf((mlib_d64 *)sp);
		sd1 = vis_ld_d64_nf((mlib_d64 *)sp + 1);
		CHANNELSEPARATE_U8(sd0, sd1, dp[0], dp[1]);
		sp += 2;
		dp += 2;
	}
}

/* *********************************************************** */

mlib_status
__mlib_VideoColorARGB2JFIFYCC444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	mlib_d64 buff_arr[(SIZE / 2) + 2];
	mlib_f32 *py, *pcb, *pcr;
	mlib_d64 *buff;
	mlib_d64 sdh, sdl, dr, dg, db, dd;
	mlib_s32 i, m, size, num;

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
	mlib_d64 off0 = vis_to_double_dup(0x00100010);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(2 << 3);

/*
 * 4-pixel loop
 */
	for (size = 0; size < n; size += num) {

		num = n - size;

		if (num > SIZE)
			num = SIZE;

		m = (num + 3) / 4;
		mlib_channel_separate((mlib_d64 *)argb + size / 2, buff_arr, m);

		m = (num / 4) & ~1;
		py = (mlib_f32 *)y + size / 4;
		pcb = (mlib_f32 *)cb + size / 4;
		pcr = (mlib_f32 *)cr + size / 4;
		buff = buff_arr;
#pragma pipeloop(0)
		for (i = 0; i < m; i++) {
			sdh = buff[0];
			sdl = buff[1];
			CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
				vis_read_lo(sdl), k11, k12, k13, off0, py[0]);
			CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
				vis_read_lo(sdl), k21, k22, k23, off128,
				pcb[0]);
			CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
				vis_read_lo(sdl), k31, k32, k33, off128,
				pcr[0]);
			py++;
			pcb++;
			pcr++;
			buff += 2;
		}
	}

	if (n & 7) {
		mlib_s32 emask = (0xFF00 >> (n & 7)) & 0xFF;
		mlib_d64 rbuff[3];
		mlib_f32 *prbuff = (mlib_f32 *)rbuff;

		sdh = (*buff++);
		sdl = (*buff++);
		CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
			vis_read_lo(sdl), k11, k12, k13, off0, prbuff[0]);
		CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
			vis_read_lo(sdl), k21, k22, k23, off128, prbuff[2]);
		CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
			vis_read_lo(sdl), k31, k32, k33, off128, prbuff[4]);
		sdh = (*buff++);
		sdl = (*buff++);
		CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
			vis_read_lo(sdl), k11, k12, k13, off0, prbuff[1]);
		CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
			vis_read_lo(sdl), k21, k22, k23, off128, prbuff[3]);
		CHANNELWEIGHT_U8(vis_read_lo(sdh), vis_read_hi(sdl),
			vis_read_lo(sdl), k31, k32, k33, off128, prbuff[5]);
		vis_pst_8(rbuff[0], py, emask);
		vis_pst_8(rbuff[1], pcb, emask);
		vis_pst_8(rbuff[2], pcr, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
