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

#pragma ident	"@(#)mlib_c_VideoColorYUV2ABGR420_X3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_X3 - color conversion-YUV420 to ABGR
 *                                      with zoom x3
 *
 * SYNOPSIS
 *
 * mlib_status mlib_VideoColorYUV2ABGR420_X3(mlib_u8       *abgr,
 *                                           const mlib_u8 *y,
 *                                           const mlib_u8 *u,
 *                                           const mlib_u8 *v,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      abgr_stride,
 *                                           mlib_s32      y_stride,
 *                                           mlib_s32      uv_stride)
 *
 * ARGUMENT
 *      abgr            Pointer to output ABGR image
 *      y               Pointer to input Y component
 *      u               Pointer to input U component
 *      v               Pointer to input V component
 *      width           Width of the not zoomed image
 *      height          Height of the not zoomed  image
 *      abgr_stride     Stride in bytes between adjacent rows in output image
 *      y_stride        Stride in bytes between adjacent rows in Y input image
 *      uv_stride       Stride in bytes between adjacent rows in U and
 *                      V input image
 *
 * DESCRIPTION
 *        |A| = 0
 *        |R|   |1.1644   0.0000   1.5966|   [Y   16.0000 ]
 *        |G| = |1.1644  -0.3920  -0.8132| * [U - 128.0000]
 *        |B|   |1.1644   2.0184   0.0000|   [V   128.0000]
 *
 * NOTES
 *  abgr and y must be 8-byte aligned
 *  u and v must be 4-byte aligned
 *  width must be a multiple of 8
 *  height must be a multiple of 2
 *  abgr_stride and y_stride must be a multiple of 8
 *  uv_stride must be a multiple of 4
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR420_X3 = __mlib_VideoColorYUV2ABGR420_X3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_X3) mlib_VideoColorYUV2ABGR420_X3
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_X3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(val, dst)	dst = pClip[val]

/* *********************************************************** */

#define	SCLAMP_U8(s, dst)                                       \
	{                                                       \
	    mlib_s32 v = s, mask = (v - 0x1fe) >> 31;           \
	                                                        \
	    dst = (((mlib_u32)v >> 1) | ~mask) & ~(v >> 31);    \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR420_X3(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	mlib_s32 i, j;
	mlib_u8 *rgb0, *rgb1, *rgb2, *rgb3, *rgb4, *rgb5;
	const mlib_u8 *Y = y_inp;
	const mlib_u8 *U = u_inp;
	const mlib_u8 *V = v_inp;
	const mlib_u8 *Z = y_inp + y_stride;

	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;
	mlib_s32 sf01, sf02, sf03, sf04, sf1, sf2, sfr, sfg, sfb;
	mlib_s32 r0, r1, r2, r3, rr;

	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if (wd & 7 || ht & 1 || rgb_stride & 7 || uv_stride & 3 || y_stride & 7)
		return (MLIB_FAILURE);

	rgb0 = abgr;
	rgb1 = rgb0 + rgb_stride;
	rgb2 = rgb1 + rgb_stride;
	rgb3 = rgb2 + rgb_stride;
	rgb4 = rgb3 + rgb_stride;
	rgb5 = rgb4 + rgb_stride;

	uv_stride = uv_stride - (wd >> 1);
	y_stride = 2 * y_stride - wd;
	rgb_stride = 6 * rgb_stride - wd * 12;

	for (j = 0; j < (ht >> 1); j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (wd >> 1); i++) {

			sf1 = U[0];
			sf2 = V[0];

			sf01 = *(mlib_u16 *)Y;
			sf02 = *(mlib_u16 *)Z;
#ifdef _LITTLE_ENDIAN
			sf03 = (mlib_u32)sf01 >> 8;
			sf01 = sf01 & 0xff;
			sf04 = (mlib_u32)sf02 >> 8;
			sf02 = sf02 & 0xff;
#else /* _LITTLE_ENDIAN */
			sf03 = sf01 & 0xff;
			sf01 = (mlib_u32)sf01 >> 8;
			sf04 = sf02 & 0xff;
			sf02 = (mlib_u32)sf02 >> 8;
#endif /* _LITTLE_ENDIAN */

			sfb = mlib_tBU[sf1] - BBi;
			sfg = mlib_tGU[sf1] - mlib_tGV[sf2];
			sfr = mlib_tRV[sf2] - RRi;

			sf01 = mlib_tYY[sf01];
			sf03 = mlib_tYY[sf03];
			sf02 = mlib_tYY[sf02];
			sf04 = mlib_tYY[sf04];

#ifdef _LITTLE_ENDIAN
			TCLAMP_U8(sf01 + sfb, r0);
			r0 <<= 8;
			TCLAMP_U8(sf01 + sfg, rr);
			r0 |= rr << 16;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr << 24;

			TCLAMP_U8(sf03 + sfb, r2);
			r2 <<= 8;
			TCLAMP_U8(sf03 + sfg, rr);
			r2 |= rr << 16;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr << 24;

			TCLAMP_U8(sf02 + sfb, r1);
			r1 <<= 8;
			TCLAMP_U8(sf02 + sfg, rr);
			r1 |= rr << 16;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr << 24;

			TCLAMP_U8(sf04 + sfb, r3);
			r3 <<= 8;
			TCLAMP_U8(sf04 + sfg, rr);
			r3 |= rr << 16;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr << 24;
#else /* _LITTLE_ENDIAN */
			TCLAMP_U8(sf01 + sfb, r0);
			r0 <<= 16;
			TCLAMP_U8(sf01 + sfg, rr);
			r0 |= rr << 8;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr;

			TCLAMP_U8(sf03 + sfb, r2);
			r2 <<= 16;
			TCLAMP_U8(sf03 + sfg, rr);
			r2 |= rr << 8;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr;

			TCLAMP_U8(sf02 + sfb, r1);
			r1 <<= 16;
			TCLAMP_U8(sf02 + sfg, rr);
			r1 |= rr << 8;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr;

			TCLAMP_U8(sf04 + sfb, r3);
			r3 <<= 16;
			TCLAMP_U8(sf04 + sfg, rr);
			r3 |= rr << 8;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr;
#endif /* _LITTLE_ENDIAN */

			((mlib_s32 *)rgb0)[0] = r0;
			((mlib_s32 *)rgb0)[1] = r0;
			((mlib_s32 *)rgb0)[2] = r0;
			((mlib_s32 *)rgb1)[0] = r0;
			((mlib_s32 *)rgb1)[1] = r0;
			((mlib_s32 *)rgb1)[2] = r0;
			((mlib_s32 *)rgb2)[0] = r0;
			((mlib_s32 *)rgb2)[1] = r0;
			((mlib_s32 *)rgb2)[2] = r0;

			((mlib_s32 *)rgb0)[3] = r2;
			((mlib_s32 *)rgb0)[4] = r2;
			((mlib_s32 *)rgb0)[5] = r2;
			((mlib_s32 *)rgb1)[3] = r2;
			((mlib_s32 *)rgb1)[4] = r2;
			((mlib_s32 *)rgb1)[5] = r2;
			((mlib_s32 *)rgb2)[3] = r2;
			((mlib_s32 *)rgb2)[4] = r2;
			((mlib_s32 *)rgb2)[5] = r2;

			((mlib_s32 *)rgb3)[0] = r1;
			((mlib_s32 *)rgb3)[1] = r1;
			((mlib_s32 *)rgb3)[2] = r1;
			((mlib_s32 *)rgb4)[0] = r1;
			((mlib_s32 *)rgb4)[1] = r1;
			((mlib_s32 *)rgb4)[2] = r1;
			((mlib_s32 *)rgb5)[0] = r1;
			((mlib_s32 *)rgb5)[1] = r1;
			((mlib_s32 *)rgb5)[2] = r1;

			((mlib_s32 *)rgb3)[3] = r3;
			((mlib_s32 *)rgb3)[4] = r3;
			((mlib_s32 *)rgb3)[5] = r3;
			((mlib_s32 *)rgb4)[3] = r3;
			((mlib_s32 *)rgb4)[4] = r3;
			((mlib_s32 *)rgb4)[5] = r3;
			((mlib_s32 *)rgb5)[3] = r3;
			((mlib_s32 *)rgb5)[4] = r3;
			((mlib_s32 *)rgb5)[5] = r3;

			Y += 2;
			Z += 2;
			U++;
			V++;
			rgb0 += 24;
			rgb1 += 24;
			rgb2 += 24;
			rgb3 += 24;
			rgb4 += 24;
			rgb5 += 24;
		}

		Y += y_stride;
		Z += y_stride;
		rgb0 += rgb_stride;
		rgb1 += rgb_stride;
		rgb2 += rgb_stride;
		rgb3 += rgb_stride;
		rgb4 += rgb_stride;
		rgb5 += rgb_stride;
		U += uv_stride;
		V += uv_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
