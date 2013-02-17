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

#pragma ident	"@(#)mlib_c_VideoColorYUV2ABGR420_W.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_W - color conversion-YUV420 to ABGR
 *                                     inside clipping window
 *
 * SYNOPSIS
 *
 * mlib_status mlib_VideoColorYUV2ABGR420_W(mlib_u8       *abgr,
 *                                          const mlib_u8 *y,
 *                                          const mlib_u8 *u,
 *                                          const mlib_u8 *v,
 *                                          mlib_s32      width,
 *                                          mlib_s32      height,
 *                                          mlib_s32      abgr_stride,
 *                                          mlib_s32      y_stride,
 *                                          mlib_s32      uv_stride,
 *                                          mlib_s32      left,
 *                                          mlib_s32      top,
 *                                          mlib_s32      right,
 *                                          mlib_s32      bottom)
 *
 * ARGUMENT
 *      abgr        Pointer to output ABGR image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image
 *      height      Height of the image
 *      abgr_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input image
 *      left        left border of the clipping window
 *      top         top border of the clipping window
 *      right       right opened border of the clipping window
 *      bottom      bottom opened border of the clipping window
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
 *  window borders must be: 0 <= left < right <= width
 *  and 0 <= top < bottom <= height
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR420_W = __mlib_VideoColorYUV2ABGR420_W

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_W) mlib_VideoColorYUV2ABGR420_W
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_W")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(val, dst)	dst = pClip[val]

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	TCLAMP_U8E(val, dst)	*((mlib_u16*)&(dst)) = pClip[val]<<8;
#else /* _LITTLE_ENDIAN */
#define	TCLAMP_U8E(val, dst)	*((mlib_u16*)&(dst)) = pClip[val];
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	CALC1(_dst, _y, _u, _v)                                 \
	sfb = mlib_tBU[_u] - BBi;                               \
	sfg = mlib_tGU[_u] - mlib_tGV[_v];                      \
	sfr = mlib_tRV[_v] - RRi;                               \
	sf01 = mlib_tYY[_y];                                    \
	TCLAMP_U8E(sf01 + sfb, _dst[0]);                        \
	TCLAMP_U8(sf01 + sfg, _dst[2]);                         \
	TCLAMP_U8(sf01 + sfr, _dst[3])

/* *********************************************************** */

#define	CALC2(_dst1, _dst2, _y1, _y2, _u, _v)                   \
	sfb = mlib_tBU[_u] - BBi;                               \
	sfg = mlib_tGU[_u] - mlib_tGV[_v];                      \
	sfr = mlib_tRV[_v] - RRi;                               \
	sf01 = mlib_tYY[_y1];                                   \
	sf02 = mlib_tYY[_y2];                                   \
	TCLAMP_U8E(sf01 + sfb, _dst1[0]);                       \
	TCLAMP_U8(sf01 + sfg, _dst1[2]);                        \
	TCLAMP_U8(sf01 + sfr, _dst1[3]);                        \
	TCLAMP_U8E(sf02 + sfb, _dst2[0]);                       \
	TCLAMP_U8(sf02 + sfg, _dst2[2]);                        \
	TCLAMP_U8(sf02 + sfr, _dst2[3])

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR420_W(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride,
	mlib_s32 left,
	mlib_s32 top,
	mlib_s32 right,
	mlib_s32 bottom)
{
	mlib_s32 i, j;
	const mlib_u8 *say1, *say2, *sau, *sav, *sly1, *sly2, *slu, *slv;
	mlib_u8 *da1, *dl1, *da2, *dl2;

	mlib_s32 sf01, sf02, sf03, sf04, sf1, sf2, sfr, sfg, sfb;
	mlib_s32 r0, r1, r2, r3, rr;
	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;

	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if (wd & 7 || ht & 1 || abgr_stride & 7 || uv_stride & 3 ||
		y_stride & 7)
		return (MLIB_FAILURE);

	if (left < 0 || top < 0 || right <= left || bottom <= top ||
	    wd < right || ht < bottom)
		return (MLIB_FAILURE);

	abgr += left * 4 + top * abgr_stride;
	y_inp += left + top * y_stride;
	u_inp += (left >> 1) + (top >> 1) * uv_stride;
	v_inp += (left >> 1) + (top >> 1) * uv_stride;

	wd = ((right & ~1) - left) >> 1;
	ht = ((bottom & ~1) - top) >> 1;

	if (top & 1) {
		say1 = y_inp;
		sau = u_inp;
		sav = v_inp;
		da1 = abgr;

		if (left & 1) {
			CALC1(abgr, y_inp[0], u_inp[0], v_inp[0]);
			say1++;
			sau++;
			sav++;
			da1 += 4;
		}

		for (i = 0; i < wd; i++) {
			CALC2(da1, (da1 + 4), say1[0], say1[1], sau[0], sav[0]);
			say1 += 2;
			sau++;
			sav++;
			da1 += 8;
		}

		if (right & 1) {
			CALC1(da1, say1[0], sau[0], sav[0]);
		}

		abgr += abgr_stride;
		y_inp += y_stride;
		u_inp += uv_stride;
		v_inp += uv_stride;
	}

	sly1 = say1 = y_inp;
	sly2 = say2 = y_inp + y_stride;
	slu = sau = u_inp;
	slv = sav = v_inp;
	dl1 = da1 = abgr;
	dl2 = da2 = abgr + abgr_stride;

	for (j = 0; j < ht; j++) {

		if (left & 1) {
			CALC2(da1, da2, say1[0], say2[0], sau[0], sav[0]);
			say1++;
			say2++;
			sau++;
			sav++;
			da1 += 4;
			da2 += 4;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wd; i++) {

			sf1 = sau[0];
			sf2 = sav[0];

			sf01 = *(mlib_u16 *)say1;
			sf02 = *(mlib_u16 *)say2;
#ifdef _LITTLE_ENDIAN
			sf03 = (mlib_u32)sf01 >> 8;
			sf01 = sf01 & 0xff;
			sf04 = (mlib_u32)sf02 >> 8;
			sf02 = sf02 & 0xff;
/* sfr and sfg really are sfg and sfr */
			sfr = mlib_tGU[sf1] - mlib_tGV[sf2];
			sfg = mlib_tRV[sf2] - RRi;
#else /* _LITTLE_ENDIAN */
			sf03 = sf01 & 0xff;
			sf01 = (mlib_u32)sf01 >> 8;
			sf04 = sf02 & 0xff;
			sf02 = (mlib_u32)sf02 >> 8;
			sfg = mlib_tGU[sf1] - mlib_tGV[sf2];
			sfr = mlib_tRV[sf2] - RRi;
#endif /* _LITTLE_ENDIAN */

			sfb = mlib_tBU[sf1] - BBi;

			sf01 = mlib_tYY[sf01];
			sf03 = mlib_tYY[sf03];
			sf02 = mlib_tYY[sf02];
			sf04 = mlib_tYY[sf04];

			TCLAMP_U8E(sf01 + sfb, da1[0])
				TCLAMP_U8(sf01 + sfg, r0);
			r0 <<= 8;
			TCLAMP_U8(sf01 + sfr, rr);
			r0 |= rr;

			TCLAMP_U8E(sf03 + sfb, da1[4])
				TCLAMP_U8(sf03 + sfg, r2);
			r2 <<= 8;
			TCLAMP_U8(sf03 + sfr, rr);
			r2 |= rr;

			TCLAMP_U8E(sf02 + sfb, da2[0])
				TCLAMP_U8(sf02 + sfg, r1);
			r1 <<= 8;
			TCLAMP_U8(sf02 + sfr, rr);
			r1 |= rr;

			TCLAMP_U8E(sf04 + sfb, da2[4])
				TCLAMP_U8(sf04 + sfg, r3);
			r3 <<= 8;
			TCLAMP_U8(sf04 + sfr, rr);
			r3 |= rr;
			((mlib_u16 *)da1)[1] = r0;
			((mlib_u16 *)da1)[3] = r2;
			((mlib_u16 *)da2)[1] = r1;
			((mlib_u16 *)da2)[3] = r3;

			say1 += 2;
			say2 += 2;
			sau++;
			sav++;
			da1 += 8;
			da2 += 8;
		}

		if (right & 1) {
			CALC2(da1, da2, say1[0], say2[0], sau[0], sav[0]);
		}

		sly1 = say1 = ((mlib_u8 *)sly1 + 2 * y_stride);
		sly2 = say2 = ((mlib_u8 *)sly2 + 2 * y_stride);
		slu = sau = ((mlib_u8 *)slu + uv_stride);
		slv = sav = ((mlib_u8 *)slv + uv_stride);
		dl1 = da1 = ((mlib_u8 *)dl1 + 2 * abgr_stride);
		dl2 = da2 = ((mlib_u8 *)dl2 + 2 * abgr_stride);
	}

	if (bottom & 1) {
		if (left & 1) {
			CALC1(da1, say1[0], sau[0], sav[0]);
			say1++;
			sau++;
			sav++;
			da1 += 4;
		}

		for (i = 0; i < wd; i++) {
			CALC2(da1, (da1 + 4), say1[0], say1[1], sau[0], sav[0]);
			say1 += 2;
			sau++;
			sav++;
			da1 += 8;
		}

		if (right & 1) {
			CALC1(da1, say1[0], sau[0], sav[0]);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
