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

#pragma ident	"@(#)mlib_v_ImageScale.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale - linear scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScale(mlib_image       *dst,
 *                                  const mlib_image *src,
 *                                  const mlib_s32   *alpha,
 *                                  const mlib_s32   *beta,
 *                                  mlib_s32         shift);
 *
 * ARGUMENT
 *      dst     pointer to the output image
 *      src     pointer to the input image
 *      alpha   array of scaling factors to be multiplied to each channel
 *              of the pixels
 *      beta    array of offsets to be added to each channel of the pixels
 *      shift   right shift factor
 *
 * VALID RANGES
 *   some designations:  maxA  - maximum of absolute values of alpha[i]
 *                       maxB  - maximum of absolute values of beta[i]
 *                       2^x   - two power x
 *                       STYPE - source image type
 *                       DTYPE - destination image type
 *                       SHIFT - shift argument
 *                       RESTRICTIONS - restrictions to maxA,
 *                       maxB and shift for given types and shift
 *
 *  STYPE  DTYPE        SHIFT                 RESTRICTIONS
 *   U8     U8        [ 0, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U8     S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   U8     S16       [ 1,  8]    maxA < 2^15
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U8     S16       [ 9, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U8     U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^8  * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   U8     U16       [ 1,  8]    maxA < 2^15
 *                                2^8  * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   U8     U16       [ 9, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^23
 *   U8     S32       [ 0, 31]    no restrictions
 *   U8     F32, D64  [ 0, 31]    no restrictions
 *   S16    U8        [ 0,  7]    maxA < 2^(shift+7)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   S16    U8        [ 8, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    U8        [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   S16    S16       [ 1, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    S16       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   S16    U16       [ 1, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   S16    U16       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^15
 *   S16    S32       [ 0, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    S32       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    F32, D64  [ 0, 31]    no restrictions
 *   U16    U8        [ 0,  7]    maxA < 2^(shift+7)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U16    U8        [ 8, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    U8        [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   U16    S16       [ 1, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    S16       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   U16    U16       [ 1, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   U16    U16       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^15
 *   U16    S32       [ 0, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB +
 *                                0.5 < 2^(31-shift)
 *   U16    S32       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    F32, D64  [ 0, 31]    no restrictions
 *   S32    U8, S16   [ 0, 31]    maxA < 2^(shift-17)
 *                                2^31 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(47-shift)
 *   S32    U8, S16   [ 0, 31]    no restrictions, but may be more slow
 *                                than with restrictions
 *   S32    U16       [ 0, 31]    maxA < 2^(shift-17)
 *                                2^31 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(47-shift)
 *   S32    U16       [ 0, 31]    no restrictions, but may be more slow
 *                                than with restrictions
 *   S32    S32       [ 0, 31]    no restrictions
 *   S32    F32, D64  [ 0, 31]    no restrictions
 *
 *   If one of RESTRICTIONS is failed then mlib_ImageScale
 *   returns MLIB_OUTOFRANGE
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The input image can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *      The output image can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      dst[x][y][c] = src[x][y][c] * alpha[c] * 2^(-shift) + beta[c]
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageScale2.h>
#include <mlib_v_ImageScale.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale = __mlib_ImageScale

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScale) mlib_ImageScale
    __attribute__((weak, alias("__mlib_ImageScale")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PREPARE_AB_124                                          \
	bh[0] = bl[2] = bd0;                                    \
	bh[2] = bl[0] = bd1;                                    \
	vis_alignaddr((void *)0, 4);                            \
	bh[3] = bl[1] = vis_faligndata(bd0, bd1);               \
	bh[1] = bl[3] = vis_faligndata(bd1, bd0);               \
	ah[0] = al[2] = vis_read_hi(ad);                        \
	ah[2] = al[0] = vis_read_lo(ad);                        \
	vis_alignaddr((void *)0, 2);                            \
	ad1 = vis_faligndata(ad, ad);                           \
	ah[3] = al[1] = vis_read_hi(ad1);                       \
	ah[1] = al[3] = vis_read_lo(ad1)

/* *********************************************************** */

#define	PREPARE_AB_3                                            \
	bh[0] = bl[1] = bd0;                                    \
	bh[2] = bl[0] = bd1;                                    \
	vis_alignaddr((void *)0, 4);                            \
	bh[1] = bl[2] = vis_faligndata(bd0, bd1);               \
	ah[0] = al[1] = vis_read_hi(ad);                        \
	ah[2] = al[0] = vis_read_lo(ad);                        \
	vis_alignaddr((void *)0, 2);                            \
	ad1 = vis_faligndata(ad, ad);                           \
	ah[1] = al[2] = vis_read_hi(ad1)

/* *********************************************************** */

mlib_status
mlib_ImageScale_VIS(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 ad,
    mlib_d64 bd0,
    mlib_d64 bd1,
    mlib_s32 shift,
    mlib_s32 fast_u8_u8)
{
	mlib_type stype, dtype;
	mlib_s32 slb, dlb, xsize, ysize, dsize, nchan;
	mlib_s32 flags, flagd, flagc;
	void *sa, *da;
	mlib_d64 ad1;
	mlib_f32 ah[4], al[4];
	mlib_d64 bh[4], bl[4];
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);
	stype = mlib_ImageGetType(src);
	slb = mlib_ImageGetStride(src);
	sa = mlib_ImageGetData(src);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);
	flagc = flags | flagd;

	xsize = xsize * nchan;
	dsize = xsize * ysize;

	if (stype == MLIB_BYTE && dtype == MLIB_BYTE) {

		if (fast_u8_u8) {

/* initialize GSR scale factor */
			vis_write_gsr(((15 - shift) & 0xf) << 3);

/* 3 channels */
			if (nchan == 3) {
				if ((flagc & A8D1) == 0)
					mlib_v_ImageScale_U8_U8_3_A8D1(sa, da,
					    dsize, ad, bd0);
				else if ((flagc & A8D2) == 0)
					mlib_v_ImageScale_U8_U8_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, ad, bd0);
				else
					mlib_v_ImageScale_U8_U8_3(sa, slb, da,
					    dlb, xsize, ysize, ad, bd0);
			} else {
/* 1, 2 or 4 channels */

				if ((flagc & A8D1) == 0 && (dsize & 7) == 0)
					mlib_v_ImageScale_U8_U8_124_A8D1X8(sa,
					    da, dsize, ad, bd0);
				else if ((flagc & A8D2) == 0)
					mlib_v_ImageScale_U8_U8_124_A8D2(sa,
					    slb, da, dlb, xsize, ysize, ad,
					    bd0);
				else
					mlib_v_ImageScale_U8_U8_124(sa, slb, da,
					    dlb, xsize, ysize, ad, bd0);
			}

			return (MLIB_SUCCESS);

		} else {

/* initialize GSR scale factor */
			vis_write_gsr(((23 - shift) & 0xf) << 3);

/* 3 channels */
			if (nchan == 3) {
				if ((flagc & A8D1) == 0)
					mlib_v_ImageScale_U8_U8_D_3_A8D1(sa, da,
					    dsize, ad, bd0, bd1);
				else if ((flagc & A8D2) == 0)
					mlib_v_ImageScale_U8_U8_D_3_A8D2(sa,
					    slb, da, dlb, xsize, ysize, ad, bd0,
					    bd1);
				else
					mlib_v_ImageScale_U8_U8_D_3(sa, slb, da,
					    dlb, xsize, ysize, ad, bd0, bd1);
			} else {
/* 1, 2 or 4 channels */

				if ((flagc & A8D1) == 0 && (dsize & 7) == 0)
					mlib_v_ImageScale_U8_U8_D_124_A8D1X8(sa,
					    da, dsize, ad, bd0, bd1);
				else if ((flagc & A8D2) == 0)
					mlib_v_ImageScale_U8_U8_D_124_A8D2(sa,
					    slb, da, dlb, xsize, ysize, ad, bd0,
					    bd1);
				else
					mlib_v_ImageScale_U8_U8_D_124(sa, slb,
					    da, dlb, xsize, ysize, ad, bd0,
					    bd1);
			}
		}

	} else if (stype == MLIB_BYTE && dtype == MLIB_SHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageScale_U8_S16_3(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */

			if ((flagc & A8D1) == 0 && (dsize & 7) == 0)
				mlib_v_ImageScale_U8_S16_124_A8D1X8(sa, da,
				    dsize, ad, bd0, bd1);
			else if ((flagc & A8D2) == 0)
				mlib_v_ImageScale_U8_S16_124_A8D2(sa, slb, da,
				    dlb, xsize, ysize, ad, bd0, bd1);
			else
				mlib_v_ImageScale_U8_S16_124(sa, slb, da, dlb,
				    xsize, ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_BYTE && dtype == MLIB_USHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageScale_U8_U16_3(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */
			mlib_v_ImageScale_U8_U16_124(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		}
	}

/*
 * WARNING! No VIS version for U8->S32.                           *
 * See function mlib_ImageScale_CheckForVIS (line 61) for change. *
 * ****************************************************************
 *  else if (stype == MLIB_BYTE && dtype == MLIB_INT) {
 *
 *    if (nchan == 3) {
 *      PREPARE_AB_3;
 *
 *      if ((flagc & A8D1) == 0)
 *        mlib_v_ImageScale_U8_S32_3_A8D1(sa, da, dsize, ah, al, bh, bl, shift);
 *      else
 *        mlib_v_ImageScale_U8_S32_3
 *            (sa, slb, da, dlb, xsize, ysize, ah, al, bh, bl, shift);
 *    } else {
 *
 *      if ((flagc & A8D1) == 0 && (dsize & 3)  == 0) {
 *        mlib_v_ImageScale_U8_S32_124_A8D1(sa, da, dsize, ad, bd0, bd1, shift);
 *      } else {
 *        PREPARE_AB_124;
 *        mlib_v_ImageScale_U8_S32_124
 *            ((mlib_u8 *) ((mlib_addr) sa & (~3)), slb,
 *             da, dlb, xsize, ysize, ah, al, bh, bl, shift);
 *      }
 *    }
 *  }
 *
 */
	else if (stype == MLIB_SHORT && dtype == MLIB_SHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			if ((flagc & A8D1) == 0)
				mlib_v_ImageScale_S16_S16_3_A8D1(sa, da, dsize,
				    ad, bd0, bd1);
			else if ((flagc & A8D2) == 0)
				mlib_v_ImageScale_S16_S16_3_A8D2(sa, slb, da,
				    dlb, xsize, ysize, ad, bd0, bd1);
			else
				mlib_v_ImageScale_S16_S16_3(sa, slb, da, dlb,
				    xsize, ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */

			if ((flagc & A8D1) == 0 && (dsize & 3) == 0)
				mlib_v_ImageScale_S16_S16_124_A8D1X4(sa, da,
				    dsize, ad, bd0, bd1);
			else if ((flagc & A8D2) == 0)
				mlib_v_ImageScale_S16_S16_124_A8D2(sa, slb, da,
				    dlb, xsize, ysize, ad, bd0, bd1);
			else
				mlib_v_ImageScale_S16_S16_124(sa, slb, da, dlb,
				    xsize, ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_SHORT && dtype == MLIB_USHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageScale_S16_U16_3(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */
			mlib_v_ImageScale_S16_U16_124(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_SHORT && dtype == MLIB_BYTE) {

/* initialize GSR scale factor */
		vis_write_gsr(((23 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			if ((flagc & A8D1) == 0)
				mlib_v_ImageScale_S16_U8_3_A8D1(sa, da, dsize,
				    ad, bd0, bd1);
			else if ((flagc & A8D2) == 0)
				mlib_v_ImageScale_S16_U8_3_A8D2(sa, slb, da,
				    dlb, xsize, ysize, ad, bd0, bd1);
			else
				mlib_v_ImageScale_S16_U8_3(sa, slb, da, dlb,
				    xsize, ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */

			if ((flagc & A8D1) == 0 && (dsize & 3) == 0)
				mlib_v_ImageScale_S16_U8_124_A8D1X4(sa, da,
				    dsize, ad, bd0, bd1);
			else if ((flagc & A8D2) == 0)
				mlib_v_ImageScale_S16_U8_124_A8D2(sa, slb, da,
				    dlb, xsize, ysize, ad, bd0, bd1);
			else
				mlib_v_ImageScale_S16_U8_124(sa, slb, da, dlb,
				    xsize, ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_SHORT && dtype == MLIB_INT) {

/* 3 channels */
		if (nchan == 3) {
			PREPARE_AB_3;
			mlib_v_ImageScale_S16_S32_3(sa, slb, da, dlb, xsize,
			    ysize, ah, bh, shift);
		} else {
/* 1, 2 or 4 channels */
			PREPARE_AB_124;
			mlib_v_ImageScale_S16_S32_124(sa, slb, da, dlb, xsize,
			    ysize, ah, al, bh, bl, shift);
		}
	} else if (stype == MLIB_USHORT && dtype == MLIB_SHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageScale_U16_S16_3(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */
			mlib_v_ImageScale_U16_S16_124(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_USHORT && dtype == MLIB_USHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageScale_U16_U16_3(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */
			mlib_v_ImageScale_U16_U16_124(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_USHORT && dtype == MLIB_BYTE) {

/* initialize GSR scale factor */
		vis_write_gsr(((23 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			mlib_v_ImageScale_U16_U8_3(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		} else {
/* 1, 2 or 4 channels */
			mlib_v_ImageScale_U16_U8_124(sa, slb, da, dlb, xsize,
			    ysize, ad, bd0, bd1);
		}

	} else if (stype == MLIB_USHORT && dtype == MLIB_INT) {

/* 3 channels */
		if (nchan == 3) {
			PREPARE_AB_3;
			mlib_v_ImageScale_U16_S32_3(sa, slb, da, dlb, xsize,
			    ysize, ah, bh, shift);
		} else {
/* 1, 2 or 4 channels */
			PREPARE_AB_124;
			mlib_v_ImageScale_U16_S32_124(sa, slb, da, dlb, xsize,
			    ysize, ah, al, bh, bl, shift);
		}

	} else if (stype == MLIB_INT && dtype == MLIB_SHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			PREPARE_AB_3;

			if ((flagc & A8D1) == 0)
				mlib_v_ImageScale_S32_S16_3_A8D1(sa, da, dsize,
				    ah, al, bh, bl);
			else
				mlib_v_ImageScale_S32_S16_3(sa, slb, da, dlb,
				    xsize, ysize, ah, al, bh, bl);
		} else {
/* 1, 2 or 4 channels */

			if ((flagc & A8D1) == 0 && (dsize & 3) == 0)
				mlib_v_ImageScale_S32_S16_124_A8D1(sa, da,
				    dsize, ad, bd0, bd1);
			else {
				PREPARE_AB_124;
				mlib_v_ImageScale_S32_S16_124(sa, slb, da, dlb,
				    xsize, ysize, ah, al, bh, bl);
			}
		}

	} else if (stype == MLIB_INT && dtype == MLIB_USHORT) {

/* initialize GSR scale factor */
		vis_write_gsr(((16 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			PREPARE_AB_3;
			mlib_v_ImageScale_S32_U16_3(sa, slb, da, dlb, xsize,
			    ysize, ah, al, bh, bl);
		} else {
/* 1, 2 or 4 channels */
			PREPARE_AB_124;
			mlib_v_ImageScale_S32_U16_124(sa, slb, da, dlb, xsize,
			    ysize, ah, al, bh, bl);
		}

	} else if (stype == MLIB_INT && dtype == MLIB_BYTE) {

/* initialize GSR scale factor */
		vis_write_gsr(((23 - shift) & 0xf) << 3);

/* 3 channels */
		if (nchan == 3) {
			PREPARE_AB_3;

			if ((flagc & A8D1) == 0)
				mlib_v_ImageScale_S32_U8_3_A8D1(sa, da, dsize,
				    ah, al, bh, bl);
			else
				mlib_v_ImageScale_S32_U8_3(sa, slb, da, dlb,
				    xsize, ysize, ah, al, bh, bl);
		} else {
/* 1, 2 or 4 channels */

			if ((flagc & A8D1) == 0 && (dsize & 3) == 0)
				mlib_v_ImageScale_S32_U8_124_A8D1(sa, da, dsize,
				    ad, bd0, bd1);
			else {
				PREPARE_AB_124;
				mlib_v_ImageScale_S32_U8_124(sa, slb, da, dlb,
				    xsize, ysize, ah, al, bh, bl);
			}
		}

	} else {
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageScale(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift)
{
	mlib_type stype, dtype;
	mlib_d64 ad[1], bd[2];
	mlib_s16 *salpha = (mlib_s16 *)ad;
	mlib_s16 *sbeta = (mlib_s16 *)bd;
	mlib_s32 *ibeta = (mlib_s32 *)bd;
	mlib_s32 nchan, old_shift, k;
	mlib_s32 beta_half, fast_u8_u8 = 0;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);
	if (beta == NULL)
		return (MLIB_FAILURE);
	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	old_shift = shift;

	shift =
	    mlib_ImageScale_CheckForVIS(alpha, beta, shift, stype, dtype,
	    nchan);

	if (shift < -1)
		return (MLIB_OUTOFRANGE);

/* C version */
	if (shift == -1) {
		mlib_d64 dshift, dalpha[4], dbeta[4];

		dshift = 1.0 / (1u << old_shift);
		for (k = 0; k < nchan; k++) {
			dalpha[k] = alpha[k] * dshift;
			dbeta[k] = beta[k];
		}

		if (stype == MLIB_BYTE)
			return (mlib_ImageScale2_U8(dst, src, dalpha, dbeta));
		if (stype == MLIB_SHORT)
			return (mlib_ImageScale2_S16(dst, src, dalpha, dbeta));
		if (stype == MLIB_USHORT)
			return (mlib_ImageScale2_U16(dst, src, dalpha, dbeta));
		if (stype == MLIB_INT)
			return (mlib_ImageScale2_S32(dst, src, dalpha, dbeta));

		return (MLIB_FAILURE);
	}

/* special fast version of Scale from U8 to U8 */
	if (shift >= 256) {
		fast_u8_u8 = 1;
		shift -= 256;
	}

	if (shift < old_shift) {
		for (k = 0; k < nchan; k++)
			salpha[k] = alpha[k] >> (old_shift - shift);
	} else {
		for (k = 0; k < nchan; k++)
			salpha[k] = alpha[k] << (shift - old_shift);
	}

	for (k = nchan; k < 4; k++)
		salpha[k] = salpha[k - nchan];

	if (stype == MLIB_INT)
		shift -= 16;

	if (fast_u8_u8) {
		if (shift > 8)
			beta_half = 1u << (shift - 9);
		else
			beta_half = 0;
		for (k = 0; k < nchan; k++)
			sbeta[k] = (beta[k] << (shift - 8)) + beta_half;
		for (k = nchan; k < 4; k++)
			sbeta[k] = sbeta[k - nchan];
	} else {

		if (shift > 0)
			beta_half = 1u << (shift - 1);
		else
			beta_half = 0;
		for (k = 0; k < nchan; k++)
			ibeta[k] = (beta[k] << shift) + beta_half;
		if (dtype == MLIB_USHORT)
			for (k = 0; k < nchan; k++)
				ibeta[k] -= (32768 << shift);
		if (stype == MLIB_USHORT)
			for (k = 0; k < nchan; k++)
				ibeta[k] += (salpha[k] << 15);
		for (k = nchan; k < 4; k++)
			ibeta[k] = ibeta[k - nchan];
	}

	return mlib_ImageScale_VIS(dst, src, ad[0], bd[0], bd[1], shift,
	    fast_u8_u8);
}

/* *********************************************************** */
