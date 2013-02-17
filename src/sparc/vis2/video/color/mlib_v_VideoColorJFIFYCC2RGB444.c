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

#pragma ident	"@(#)mlib_v_VideoColorJFIFYCC2RGB444.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB444 - color conversion from JFIF YCbCr
 *                                      to RGB space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorJFIFYCC2RGB444(mlib_u8       *rgb,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *cb,
 *                                                  const mlib_u8 *cr,
 *                                                  mlib_s32      size);
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y array
 *                Note: length of cb, cr arrays must be n
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCC2RGB444
 *        R  = Y                        + 1.40200 * (Cr - 128)
 *        G  = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)
 *        B  = Y + 1.77200 * (Cb - 128)
 *
 *        Y  = y[i], Cr = cr[i], Cb = cb[i]
 *        rgb[3*i] = R, rgb[3*i+1] = G, rgb[3*i+2] = B, 0 <= i < n
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB444 = __mlib_VideoColorJFIFYCC2RGB444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB444) mlib_VideoColorJFIFYCC2RGB444
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2RGB444(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 size)
{
	mlib_u8 *dend;
	mlib_f32 *sf0, *sf1, *sf2, *pfd;
	mlib_f32 fzero = vis_fzeros();
	mlib_s32 i, n, m, emask;
	mlib_d64 tmp_arr64[2];
	mlib_d64 k01 = vis_to_double_dup(0x0000f4fd);
	mlib_d64 k02 = vis_to_double_dup(0x2cdde926);
	mlib_d64 k11 = vis_to_double_dup(0xf4fd38b4);
	mlib_d64 k12 = vis_to_double_dup(0xe9260000);
	mlib_d64 k21 = vis_to_double_dup(0x38b40000);
	mlib_d64 k22 = vis_to_double_dup(0x00002cdd);
	mlib_d64 c_0 = vis_to_double_dup(0xe9a110ff);
	mlib_d64 c_1 = vis_to_double_dup(0x10ffe3b6);
	mlib_d64 c_2 = vis_to_double_dup(0xe3b6e9a1);
	mlib_d64 k_0 = vis_to_double_dup(0x20002000);

	if (size <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((2 << 3) + 2);
	vis_write_bmask(0x0489AB37, 0);

	do {
/* loop on buffer size */

		if (size > 2 * BUFF_SIZE) {
			n = 2 * BUFF_SIZE;
		} else {
			n = size;
		}

		m = (n - 1) >> 2;
		sf0 = (mlib_f32 *)y;
		sf1 = (mlib_f32 *)cb;
		sf2 = (mlib_f32 *)cr;
		dend = rgb + 3 * n - 1;
		pfd = (mlib_f32 *)rgb;

#pragma pipeloop(0)
#pragma unroll(4)
		for (i = 0; i < m; i++) {
			mlib_d64 s00, s01, s02, s10, s11, s12, s20, s21, s22,
				s_0;
			mlib_d64 d_0235, d_0145;
			mlib_f32 x0, x1, x2;

			x0 = (*sf0++);
			x1 = (*sf1++);
			x2 = (*sf2++);

			s_0 = vis_fmul8x16(x0, k_0);
			s01 = vis_fmul8x16(x1, k01);
			s11 = vis_fmul8x16(x1, k11);
			s21 = vis_fmul8x16(x1, k21);
			s02 = vis_fmul8x16(x2, k02);
			s12 = vis_fmul8x16(x2, k12);
			s22 = vis_fmul8x16(x2, k22);

			s00 = vis_fpadd16(s_0, s01);
			s10 = vis_fpadd16(s_0, s11);
			s20 = vis_fpadd16(s_0, s21);

			s02 = vis_fpadd16(s02, c_0);
			s12 = vis_fpadd16(s12, c_1);
			s22 = vis_fpadd16(s22, c_2);

			s00 = vis_fpadd16(s00, s02);
			s10 = vis_fpadd16(s10, s12);
			s20 = vis_fpadd16(s20, s22);

			d_0235 = vis_fpack16_pair(s00, s10);
			s20 = vis_freg_pair(vis_fpack16(s20), fzero);

			d_0145 = vis_bshuffle(d_0235, s20);
			d_0235 = vis_fpack32(d_0235, d_0235);
			d_0235 = vis_fpmerge(vis_read_hi(d_0235),
				vis_read_lo(d_0235));

			pfd[0] = vis_read_hi(d_0145);
			pfd[1] = vis_read_hi(d_0235);
			pfd[2] = vis_read_lo(d_0145);

			pfd += 3;
		}

/*
 * last pixels
 */

		if ((mlib_u8 *)pfd <= dend) {
			mlib_d64 s00, s01, s02, s10, s11, s12, s20, s21, s22,
				s_0;
			mlib_d64 d_0235, d_xx14, d_0145;
			mlib_f32 x0, x1, x2;
			mlib_f32 *tmp_arr32 = (mlib_f32 *)tmp_arr64;

			x0 = *sf0;
			x1 = *sf1;
			x2 = *sf2;

			s_0 = vis_fmul8x16(x0, k_0);
			s01 = vis_fmul8x16(x1, k01);
			s11 = vis_fmul8x16(x1, k11);
			s21 = vis_fmul8x16(x1, k21);
			s02 = vis_fmul8x16(x2, k02);
			s12 = vis_fmul8x16(x2, k12);
			s22 = vis_fmul8x16(x2, k22);

			s00 = vis_fpadd16(s_0, s01);
			s10 = vis_fpadd16(s_0, s11);
			s20 = vis_fpadd16(s_0, s21);

			s02 = vis_fpadd16(s02, c_0);
			s12 = vis_fpadd16(s12, c_1);
			s22 = vis_fpadd16(s22, c_2);

			s00 = vis_fpadd16(s00, s02);
			s10 = vis_fpadd16(s10, s12);
			s20 = vis_fpadd16(s20, s22);

			d_0235 = vis_fpack16_pair(s00, s10);
			d_xx14 = vis_freg_pair(vis_fpack16(s20), fzero);

			d_0145 = vis_bshuffle(d_0235, d_xx14);
			d_0235 = vis_fpack32(d_0235, d_0235);
			d_0235 = vis_fpmerge(vis_read_hi(d_0235),
				vis_read_lo(d_0235));

			emask = vis_edge8(pfd, dend);

			if ((mlib_addr)pfd & 7) {
				pfd--;
				tmp_arr32++;
			}

			tmp_arr32[0] = vis_read_hi(d_0145);
			tmp_arr32[1] = vis_read_hi(d_0235);
			tmp_arr32[2] = vis_read_lo(d_0145);

			vis_pst_8(tmp_arr64[0], pfd, emask);

			pfd += 2;
			emask = vis_edge8(pfd, dend);

			if ((mlib_u8 *)pfd <= dend)
				vis_pst_8(tmp_arr64[1], pfd, emask);
		}

		y += n;
		cb += n;
		cr += n;
		rgb += 3 * n;
		size -= n;

	} while (size);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
