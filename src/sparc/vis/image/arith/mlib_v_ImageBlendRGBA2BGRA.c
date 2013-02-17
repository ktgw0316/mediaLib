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

#pragma ident	"@(#)mlib_v_ImageBlendRGBA2BGRA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlendRGBA2BGRA - image blending and channel reordering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlendRGBA2BGRA(mlib_image       *dst,
 *                                           const mlib_image *src);
 *
 * ARGUMENTS
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *
 * DESCRIPTION
 *      Blend the source image of the RGBA format into the destination image
 *      of the BGRA format.
 *
 *      The alpha blending is performed with the following formulae:
 *
 *          Cd = Cs * As + Cd * (1 - As)
 *          Ad = Ad
 *
 *      and it can be implemented like the following pseudo code:
 *
 *          foreach pixel in dst
 *              int t;
 *
 *              t = src_a * src_r + (255 - src_a) * dst_r + 0x80;
 *              dst_r = (t + (t >> 8)) >> 8;
 *              t = src_a * src_g + (255 - src_a) * dst_g + 0x80;
 *              dst_g = (t + (t >> 8)) >> 8;
 *              t = src_a * src_b + (255 - src_a) * dst_b + 0x80;
 *              dst_b = (t + (t >> 8)) >> 8;
 *          end
 *
 *      where, src_{r, g,b, a} are the RGB and alpha values of a src pixel;
 *      dst_{r, g,b, a} are the RGB and alpha values of the corresponding dst
 *      pixel.
 *
 *      Notice that dst_a is neither used nor changed by this function.
 *
 * RESTRICTIONS
 *      Both src and dst must be 4-channel images of type MLIB_BYTE.
 *      They must be the same size.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageChannelCopy.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlendRGBA2BGRA = __mlib_ImageBlendRGBA2BGRA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlendRGBA2BGRA) mlib_ImageBlendRGBA2BGRA
    __attribute__((weak, alias("__mlib_ImageBlendRGBA2BGRA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BLEND(dd, af, sf, df)                                   \
	{                                                       \
	    mlib_d64 alp, ralp, d0, d1;                         \
	                                                        \
	    alp = vis_fmul8x16al(af, fexpand);                  \
	    ralp = vis_fpsub16(const255, alp);                  \
	    d0 = vis_fmul8x16al(sf, vis_read_lo(alp));          \
	    d1 = vis_fmul8x16al(df, vis_read_lo(ralp));         \
	    dd = vis_fpadd16(d0, d1);                           \
	    dd = vis_fpadd16(dd, d_half);                       \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageBlendRGBA2BGRA(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_u8 *sl, *dl;
	mlib_d64 *sbuff, *dbuff, *sp, *dp;
	mlib_s32 slb, dlb, nchan, width, height;
	mlib_s32 i, j;
	mlib_d64 ss, aa, dd, d_h, d_l;
	mlib_f32 fscale = vis_to_float(0x01010101);
	mlib_f32 fexpand = vis_to_float(0x4000 * 256 / 255);
	mlib_d64 const255 = vis_to_double(0, 256 << 6);
	mlib_d64 d_half = vis_to_double_dup(0x001f001f);

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dlb, dl);
	slb = mlib_ImageGetStride(src);
	sl = mlib_ImageGetData(src);

	if (type != MLIB_BYTE || nchan != 4) {
		return (MLIB_FAILURE);
	}

	sbuff = __mlib_malloc(2 * 4 * width + 16);
	dbuff = sbuff + ((width + 1) / 2);

	vis_write_gsr((7 - 6) << 3);

	for (j = 0; j < height; j++) {
		if ((mlib_s32)sl & 7) {
			mlib_ImageCopy_na(sl, (void *)sbuff, 4 * width);
			sp = sbuff;
		} else {
			sp = (mlib_d64 *)sl;
		}

		if ((mlib_s32)dl & 7) {
			mlib_ImageCopy_na(dl, (void *)dbuff, 4 * width);
			dp = dbuff;
		} else {
			dp = (mlib_d64 *)dl;
		}

#if MLIB_VIS < 0x200
		vis_alignaddr((void *)0, 1);
#else /* MLIB_VIS < 0x200 */
		vis_write_bmask(0x65432107, 0);
#endif /* MLIB_VIS < 0x200 */

#pragma pipeloop(0)
		for (i = 0; i < (width / 2); i++) {
			aa = sp[i];
#if MLIB_VIS < 0x200
			ss = vis_ld_d64_le(sp + i);
			ss = vis_faligndata(ss, ss);
#else /* MLIB_VIS < 0x200 */
			ss = vis_bshuffle(aa, aa);
#endif /* MLIB_VIS < 0x200 */
			dd = dp[i];
			BLEND(d_h, vis_read_hi(aa), vis_read_lo(ss),
			    vis_read_hi(dd));
			BLEND(d_l, vis_read_lo(aa), vis_read_hi(ss),
			    vis_read_lo(dd));
			dd = vis_fpack16_pair(d_h, d_l);
			vis_pst_8(dd, dp + i, 0xEE);
		}

		if (width & 1) {
			aa = sp[i];
#if MLIB_VIS < 0x200
			ss = vis_ld_d64_le(sp + i);
			ss = vis_faligndata(ss, ss);
#else /* MLIB_VIS < 0x200 */
			ss = vis_bshuffle(aa, aa);
#endif /* MLIB_VIS < 0x200 */
			dd = dp[i];
			BLEND(d_h, vis_read_hi(aa), vis_read_lo(ss),
			    vis_read_hi(dd));
			dd = vis_fpack16_pair(d_h, d_h);
			vis_pst_8(dd, dp + i, 0xE0);
		}

		if ((mlib_s32)dl & 7) {
			mlib_v_ImageChannelCopy_U8_4((void *)dbuff, 0, dl, 0,
			    width, 1, 0xE);
		}

		sl += slb;
		dl += dlb;
	}

	__mlib_free(sbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
