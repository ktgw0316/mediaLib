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

#pragma ident	"@(#)mlib_v_VideoBGR2JFIFYCC444_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBGR2JFIFYCC444_S16 - color conversion from BGR 12-bit
 *                                          space to JFIF YCbCr 12-bit space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorBGR2JFIFYCC444_S16(mlib_s16       *y,
 *                                                    mlib_s16       *cb,
 *                                                    mlib_s16       *cr,
 *                                                    const mlib_s16 *rgb,
 *                                                    mlib_s32       n);
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb       Pointer to BGR multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 2048
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 2048
 *
 *        R  = rgb[3*i], G = rgb[3*i+1], B = rgb[3*i+2]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb, 0 <= i < n
 *
 * RESTRICTION
 *
 *        BGR component values must be in [0, 4095] range.
 *        The resulting YCbCr components are saturating to
 *        the same range.
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorBGR2JFIFYCC444_S16 = \
	__mlib_VideoColorBGR2JFIFYCC444_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBGR2JFIFYCC444_S16)
	mlib_VideoColorBGR2JFIFYCC444_S16
	__attribute__((weak, alias("__mlib_VideoColorBGR2JFIFYCC444_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL16(SRC1, SRC2)                                               \
	(vis_fpadd16(vis_fmul8sux16(SRC1, SRC2), vis_fmul8ulx16(SRC1,   \
		SRC2)))

/* *********************************************************** */

mlib_status
__mlib_VideoColorBGR2JFIFYCC444_S16(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *rgb,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)rgb;
	mlib_d64 *dpy = (mlib_d64 *)y;
	mlib_d64 *dpcb = (mlib_d64 *)cb;
	mlib_d64 *dpcr = (mlib_d64 *)cr;
	mlib_f32 k05 = vis_to_float(0x80808080);
	mlib_f32 k0125 = vis_to_float(0x20202020);
	mlib_d64 d2048 = vis_to_double_dup(0x8000800);

/* ((int)((1-0.587)*(1 << 16)+0.5)) */
	mlib_d64 dk1 = vis_to_double_dup(0x69ba69ba);

/* ((int)((1-1/1.402)*(1 << 16)+0.5)) */
	mlib_d64 dk17 = vis_to_double_dup(0x49674967);

/* (1-1/1.77200) * 2 */
	mlib_f32 fk03 = vis_to_float(0xdfdfdfdf);

/* 0.11400 * 8 */
	mlib_f32 fk2 = vis_to_float(0xe9e9e9e9);

/* 0.29900 * 2 */
	mlib_f32 fk0 = vis_to_float(0x99999999);
	mlib_d64 sd0, sd1, sd2, sdr, sdg, sdb, d0, d1, d2, d3, dcb, dcr;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_alignaddr((void *)0, 4);
	vis_write_bmask(0x02CE13DF, 0);

#pragma pipeloop(0)
	for (i = 0; i < (n >> 2); i++) {

		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];

		d1 = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));
		d0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));
		sdb = vis_bshuffle(d0, d1);
		d2 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));
		d3 = vis_faligndata(d0, d2);
		sdg = vis_bshuffle(d3, d3);
		sdr = vis_bshuffle(d1, d2);

/* 0.29900 * R */
		d0 = vis_fmul8x16(k05, vis_fmul8x16(fk0, sdr));
/* 0.58700 * G */
		d1 = vis_fpsub16(sdg, MUL16(sdg, dk1));
/* 0.11400 * B */
		d2 = vis_fmul8x16(k0125, vis_fmul8x16(fk2, sdb));

		d2 = vis_fpadd16(d1, d2);
		dcr = vis_fpsub16(d2, MUL16(d2, dk17));
		dcr = vis_fpsub16(vis_fmul8x16(k05, sdr), dcr);
		dpcr[i] = vis_fpadd16(dcr, d2048);

		d1 = vis_fpadd16(d0, d1);
		dcb = vis_fmul8x16(k05, vis_fmul8x16(fk03, d1));
		dcb = vis_fpsub16(d1, dcb);
		dcb = vis_fpsub16(vis_fmul8x16(k05, sdb), dcb);
		dpcb[i] = vis_fpadd16(dcb, d2048);

		dpy[i] = vis_fpadd16(d0, d2);
		sp += 3;
	}

#pragma unroll(1)
	for (i = n & ~3; i < n; i++) {
		mlib_f32 fr, fg, fb;

		fb = rgb[3 * i];
		fg = rgb[3 * i + 1];
		fr = rgb[3 * i + 2];
		y[i] = 0.29900f * fr + 0.58700f * fg + 0.11400f * fb;
		cb[i] = -0.16874f * fr - 0.33126f * fg + 0.50000f * fb + 2048;
		cr[i] = 0.50000f * fr - 0.41869f * fg - 0.08131f * fb + 2048;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
