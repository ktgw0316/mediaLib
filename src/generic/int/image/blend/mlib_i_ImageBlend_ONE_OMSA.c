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

#pragma ident	"@(#)mlib_i_ImageBlend_ONE_OMSA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_OMSA - blend two images with
 *      OMSA source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_OMSA(mlib_image       *dst,
 *                                           const mlib_image *src1,
 *                                           const mlib_image *src2,
 *                                           mlib_s32         cmask);
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1, src2 and dst must be the same type,
 *      the same size and the same number of channels.
 *      They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 * DESCRIPTION
 *      dst = src1 + src2 * (1 - ALPHAsrc2)
 *
 */

#include <mlib_image.h>
#include <mlib_ImageBlendUtil.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_ONE_OMSA = __mlib_ImageBlend_ONE_OMSA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_ONE_OMSA) mlib_ImageBlend_ONE_OMSA
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_OMSA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_U8_U8C(dst, src)	dst = (((src << 23) >> 31) | src)


#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_ONE_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	mlib_s32 alpha, help_alpha;
	mlib_s32 d_s0, d_s1, d_s2, a;

	BLEND_VALIDATE;

	if (channels == 3)
		return (__mlib_ImageCopy(dst, src1));

/* count index */
	alpha = ((cmask << 1) + cmask) & 0x3;

	src1_stride -= dst_width << 2;
	src2_stride -= dst_width << 2;
	dst_stride -= dst_width << 2;
	help_alpha = alpha ^ 3;

	for (i = 0; i < dst_height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < dst_width; j++) {
			d_s0 = psrc1[1];
			d_s1 = psrc1[2];
			d_s2 = psrc1[help_alpha];
			a = 256 - psrc2[alpha];

			d_s0 += (psrc2[1] * a) >> 8;
			d_s1 += (psrc2[2] * a) >> 8;
			d_s2 += (psrc2[help_alpha] * a) >> 8;

			SAT_U8_U8C(pdst[alpha],
			    (psrc1[alpha] + ((psrc2[alpha] * a) >> 8)));
			SAT_U8_U8C(pdst[1], d_s0);
			SAT_U8_U8C(pdst[2], d_s1);
			SAT_U8_U8C(pdst[help_alpha], d_s2);

			psrc1 += 4;
			psrc2 += 4;
			pdst += 4;
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
		pdst += dst_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
