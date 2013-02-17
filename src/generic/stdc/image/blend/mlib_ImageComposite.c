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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_ImageComposite.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageComposite[_Inp] - high-level wrapper API to
 *      select the appropriate combination.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageComposite(mlib_image       *dst,
 *                                      const mlib_image *src1,
 *                                      const mlib_image *src2,
 *                                      mlib_blend       bsrc1,
 *                                      mlib_blend       bsrc2,
 *                                      mlib_s32         cmask);
 *      mlib_status mlib_ImageComposite_Inp(mlib_image       *src1dst,
 *                                          const mlib_image *src2,
 *                                          mlib_blend       bsrc1,
 *                                          mlib_blend       bsrc2,
 *                                          mlib_s32         cmask);
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      bsrc1   blend factor for src1 image
 *      bsrc2   blend factor for src2 image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1, src2, src1dst, and dst must be the same type,
 *      the same size and the same number of channels.
 *      They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 * DESCRIPTION
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageComposite = __mlib_ImageComposite
#pragma weak mlib_ImageComposite_Inp = __mlib_ImageComposite_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageComposite) mlib_ImageComposite
    __attribute__((weak, alias("__mlib_ImageComposite")));
__typeof__(__mlib_ImageComposite_Inp) mlib_ImageComposite_Inp
    __attribute__((weak, alias("__mlib_ImageComposite_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageComposite(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_blend bsrc1,
    mlib_blend bsrc2,
    mlib_s32 cmask)
{
	switch ((bsrc2 << 4) | bsrc1) {

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_SAS(dst, src1, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_ZERO(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_ONE(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_DC(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_OMDC(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_SA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_OMSA(dst, src1, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_DA(dst, src1, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_OMDA(dst, src1, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_SAS(dst, src1, src2, cmask));

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageComposite_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_blend bsrc1,
    mlib_blend bsrc2,
    mlib_s32 cmask)
{
	switch ((bsrc2 << 4) | bsrc1) {

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_ZERO:
		return (__mlib_ImageBlend_ZERO_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_ONE:
		return (__mlib_ImageBlend_ONE_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_SRC_COLOR:
		return (__mlib_ImageBlend_SC_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_COLOR:
		return (__mlib_ImageBlend_OMSC_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_SRC_ALPHA:
		return (__mlib_ImageBlend_SA_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) |
		MLIB_BLEND_ONE_MINUS_SRC_ALPHA:
		return (__mlib_ImageBlend_OMSA_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) | MLIB_BLEND_DST_ALPHA:
		return (__mlib_ImageBlend_DA_SAS_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ZERO << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_ZERO_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_ONE_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_COLOR << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_DC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_COLOR << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_OMDC_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_SA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_SRC_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_OMSA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_DST_ALPHA << 4) | MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_DA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_ONE_MINUS_DST_ALPHA << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_OMDA_Inp(src1dst, src2, cmask));

	case (MLIB_BLEND_SRC_ALPHA_SATURATE << 4) |
		MLIB_BLEND_ONE_MINUS_DST_ALPHA:
		return (__mlib_ImageBlend_OMDA_SAS_Inp(src1dst, src2, cmask));

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
