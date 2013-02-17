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

#pragma ident	"@(#)mlib_c_VideoColorYUV2RGB411.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2RGB411 - color conversion from YUV411 to RGB
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2RGB411(mlib_u8       *rgb,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      rgb_stride,
 *                                            mlib_s32      y_stride,
 *                                            mlib_s32      uv_stride)
 *
 * ARGUMENT
 *      rgb         Pointer to the output RGB image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image
 *      height      Height of the image
 *      rgb_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input
 *                  image
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert2()
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2RGB411 = __mlib_VideoColorYUV2RGB411

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2RGB411) mlib_VideoColorYUV2RGB411
	__attribute__((weak, alias("__mlib_VideoColorYUV2RGB411")));

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
__mlib_VideoColorYUV2RGB411(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	mlib_s32 i, j;
	const mlib_u8 *say1, *sau, *sav, *sly1, *slu, *slv;
	mlib_u8 *da1, *dl1;

	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;

	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	if (rgb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if ((width & 3) != 0)
		return (MLIB_FAILURE);

	sly1 = say1 = y;
	slu = sau = u;
	slv = sav = v;
	dl1 = da1 = rgb;

/* non aligned case */

	if ((((mlib_addr)y | y_stride | (mlib_addr)rgb | rgb_stride) & 1)) {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (width >> 2); i++) {
				mlib_s32 sf01, sf02, sf03, sf04, sf1, sf2, sfr,
					sfg, sfb;

				sf1 = sau[0];
				sf2 = sav[0];

				sf01 = say1[0];
				sf03 = say1[1];
				sf02 = say1[2];
				sf04 = say1[3];

				sfr = mlib_tRV[sf2] - RRi;
				sfg = mlib_tGU[sf1] - mlib_tGV[sf2];
				sfb = mlib_tBU[sf1] - BBi;

				sf01 = mlib_tYY[sf01];
				sf03 = mlib_tYY[sf03];
				sf02 = mlib_tYY[sf02];
				sf04 = mlib_tYY[sf04];

				TCLAMP_U8(sf01 + sfr, da1[0]);
				TCLAMP_U8(sf01 + sfg, da1[1]);
				SCLAMP_U8(sf01 + sfb, da1[2]);
				TCLAMP_U8(sf03 + sfr, da1[3]);
				TCLAMP_U8(sf03 + sfg, da1[4]);
				SCLAMP_U8(sf03 + sfb, da1[5]);
				TCLAMP_U8(sf02 + sfr, da1[6]);
				TCLAMP_U8(sf02 + sfg, da1[7]);
				SCLAMP_U8(sf02 + sfb, da1[8]);
				TCLAMP_U8(sf04 + sfr, da1[9]);
				SCLAMP_U8(sf04 + sfg, da1[10]);
				TCLAMP_U8(sf04 + sfb, da1[11]);

				say1 += 4;
				sau++;
				sav++;
				da1 += 12;
			}

			sly1 = say1 = ((mlib_u8 *)sly1 + y_stride);
			slu = sau = ((mlib_u8 *)slu + uv_stride);
			slv = sav = ((mlib_u8 *)slv + uv_stride);
			dl1 = da1 = ((mlib_u8 *)dl1 + rgb_stride);
		}
	}

/* aligned case */
	else {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (width >> 2); i++) {
				mlib_s32 sf01, sf02, sf03, sf04, sf1, sf2, sfr,
					sfg, sfb;
				mlib_s32 r1, r2, r3, r4, r5, rr;

				sf1 = sau[0];
				sf2 = sav[0];

				sf01 = ((mlib_u16 *)say1)[0];
				sf02 = ((mlib_u16 *)say1)[1];
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

				sfr = mlib_tRV[sf2] - RRi;
				sfg = mlib_tGU[sf1] - mlib_tGV[sf2];
				sfb = mlib_tBU[sf1] - BBi;

				sf01 = mlib_tYY[sf01];
				sf03 = mlib_tYY[sf03];
				sf02 = mlib_tYY[sf02];
				sf04 = mlib_tYY[sf04];

				SCLAMP_U8(sf01 + sfr, da1[0]);
				SCLAMP_U8(sf01 + sfg, da1[1]);

				TCLAMP_U8(sf01 + sfb, r1);
#ifdef _LITTLE_ENDIAN
				TCLAMP_U8(sf03 + sfr, rr);
				r1 |= (rr << 8);
#else /* _LITTLE_ENDIAN */
				r1 <<= 8;
				TCLAMP_U8(sf03 + sfr, rr);
				r1 |= rr;
#endif /* _LITTLE_ENDIAN */

				TCLAMP_U8(sf03 + sfg, r2);
#ifdef _LITTLE_ENDIAN
				TCLAMP_U8(sf03 + sfb, rr);
				r2 |= (rr << 8);
#else /* _LITTLE_ENDIAN */
				r2 <<= 8;
				TCLAMP_U8(sf03 + sfb, rr);
				r2 |= rr;
#endif /* _LITTLE_ENDIAN */
				((mlib_u16 *)da1)[1] = r1;
				((mlib_u16 *)da1)[2] = r2;

				TCLAMP_U8(sf02 + sfr, r3);
#ifdef _LITTLE_ENDIAN
				TCLAMP_U8(sf02 + sfg, rr);
				r3 |= (rr << 8);
#else /* _LITTLE_ENDIAN */
				r3 <<= 8;
				TCLAMP_U8(sf02 + sfg, rr);
				r3 |= rr;
#endif /* _LITTLE_ENDIAN */

				TCLAMP_U8(sf02 + sfb, r4);
#ifdef _LITTLE_ENDIAN
				TCLAMP_U8(sf04 + sfr, rr);
				r4 |= (rr << 8);
#else /* _LITTLE_ENDIAN */
				r4 <<= 8;
				TCLAMP_U8(sf04 + sfr, rr);
				r4 |= rr;
#endif /* _LITTLE_ENDIAN */

				TCLAMP_U8(sf04 + sfg, r5);
#ifdef _LITTLE_ENDIAN
				TCLAMP_U8(sf04 + sfb, rr);
				r5 |= (rr << 8);
#else /* _LITTLE_ENDIAN */
				r5 <<= 8;
				TCLAMP_U8(sf04 + sfb, rr);
				r5 |= rr;
#endif /* _LITTLE_ENDIAN */

				((mlib_u16 *)da1)[3] = r3;
				((mlib_u16 *)da1)[4] = r4;
				((mlib_u16 *)da1)[5] = r5;

				say1 += 4;
				sau++;
				sav++;
				da1 += 12;
			}

			sly1 = say1 = ((mlib_u8 *)sly1 + y_stride);
			slu = sau = ((mlib_u8 *)slu + uv_stride);
			slv = sav = ((mlib_u8 *)slv + uv_stride);
			dl1 = da1 = ((mlib_u8 *)dl1 + rgb_stride);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
