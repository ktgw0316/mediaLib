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

#pragma ident	"@(#)mlib_ImageBlend_BDST_BSRC_Inp.c	9.3	07/10/09 SMI"

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_DA_DA_Inp = __mlib_ImageBlend_DA_DA_Inp
#pragma weak mlib_ImageBlend_DA_DC_Inp = __mlib_ImageBlend_DA_DC_Inp
#pragma weak mlib_ImageBlend_DA_OMDA_Inp = __mlib_ImageBlend_DA_OMDA_Inp
#pragma weak mlib_ImageBlend_DA_OMDC_Inp = __mlib_ImageBlend_DA_OMDC_Inp
#pragma weak mlib_ImageBlend_DA_OMSA_Inp = __mlib_ImageBlend_DA_OMSA_Inp
#pragma weak mlib_ImageBlend_DA_ONE_Inp = __mlib_ImageBlend_DA_ONE_Inp
#pragma weak mlib_ImageBlend_DA_SAS_Inp = __mlib_ImageBlend_DA_SAS_Inp
#pragma weak mlib_ImageBlend_DA_SA_Inp = __mlib_ImageBlend_DA_SA_Inp
#pragma weak mlib_ImageBlend_DA_ZERO_Inp = __mlib_ImageBlend_DA_ZERO_Inp
#pragma weak mlib_ImageBlend_OMDA_DA_Inp = __mlib_ImageBlend_OMDA_DA_Inp
#pragma weak mlib_ImageBlend_OMDA_DC_Inp = __mlib_ImageBlend_OMDA_DC_Inp
#pragma weak mlib_ImageBlend_OMDA_OMDA_Inp = __mlib_ImageBlend_OMDA_OMDA_Inp
#pragma weak mlib_ImageBlend_OMDA_OMDC_Inp = __mlib_ImageBlend_OMDA_OMDC_Inp
#pragma weak mlib_ImageBlend_OMDA_OMSA_Inp = __mlib_ImageBlend_OMDA_OMSA_Inp
#pragma weak mlib_ImageBlend_OMDA_ONE_Inp = __mlib_ImageBlend_OMDA_ONE_Inp
#pragma weak mlib_ImageBlend_OMDA_SAS_Inp = __mlib_ImageBlend_OMDA_SAS_Inp
#pragma weak mlib_ImageBlend_OMDA_SA_Inp = __mlib_ImageBlend_OMDA_SA_Inp
#pragma weak mlib_ImageBlend_OMDA_ZERO_Inp = __mlib_ImageBlend_OMDA_ZERO_Inp
#pragma weak mlib_ImageBlend_OMSA_DA_Inp = __mlib_ImageBlend_OMSA_DA_Inp
#pragma weak mlib_ImageBlend_OMSA_DC_Inp = __mlib_ImageBlend_OMSA_DC_Inp
#pragma weak mlib_ImageBlend_OMSA_OMDA_Inp = __mlib_ImageBlend_OMSA_OMDA_Inp
#pragma weak mlib_ImageBlend_OMSA_OMDC_Inp = __mlib_ImageBlend_OMSA_OMDC_Inp
#pragma weak mlib_ImageBlend_OMSA_OMSA_Inp = __mlib_ImageBlend_OMSA_OMSA_Inp
#pragma weak mlib_ImageBlend_OMSA_ONE_Inp = __mlib_ImageBlend_OMSA_ONE_Inp
#pragma weak mlib_ImageBlend_OMSA_SAS_Inp = __mlib_ImageBlend_OMSA_SAS_Inp
#pragma weak mlib_ImageBlend_OMSA_SA_Inp = __mlib_ImageBlend_OMSA_SA_Inp
#pragma weak mlib_ImageBlend_OMSA_ZERO_Inp = __mlib_ImageBlend_OMSA_ZERO_Inp
#pragma weak mlib_ImageBlend_OMSC_DA_Inp = __mlib_ImageBlend_OMSC_DA_Inp
#pragma weak mlib_ImageBlend_OMSC_DC_Inp = __mlib_ImageBlend_OMSC_DC_Inp
#pragma weak mlib_ImageBlend_OMSC_OMDA_Inp = __mlib_ImageBlend_OMSC_OMDA_Inp
#pragma weak mlib_ImageBlend_OMSC_OMDC_Inp = __mlib_ImageBlend_OMSC_OMDC_Inp
#pragma weak mlib_ImageBlend_OMSC_OMSA_Inp = __mlib_ImageBlend_OMSC_OMSA_Inp
#pragma weak mlib_ImageBlend_OMSC_ONE_Inp = __mlib_ImageBlend_OMSC_ONE_Inp
#pragma weak mlib_ImageBlend_OMSC_SAS_Inp = __mlib_ImageBlend_OMSC_SAS_Inp
#pragma weak mlib_ImageBlend_OMSC_SA_Inp = __mlib_ImageBlend_OMSC_SA_Inp
#pragma weak mlib_ImageBlend_OMSC_ZERO_Inp = __mlib_ImageBlend_OMSC_ZERO_Inp
#pragma weak mlib_ImageBlend_ONE_DA_Inp = __mlib_ImageBlend_ONE_DA_Inp
#pragma weak mlib_ImageBlend_ONE_DC_Inp = __mlib_ImageBlend_ONE_DC_Inp
#pragma weak mlib_ImageBlend_ONE_OMDA_Inp = __mlib_ImageBlend_ONE_OMDA_Inp
#pragma weak mlib_ImageBlend_ONE_OMDC_Inp = __mlib_ImageBlend_ONE_OMDC_Inp
#pragma weak mlib_ImageBlend_ONE_OMSA_Inp = __mlib_ImageBlend_ONE_OMSA_Inp
#pragma weak mlib_ImageBlend_ONE_ONE_Inp = __mlib_ImageBlend_ONE_ONE_Inp
#pragma weak mlib_ImageBlend_ONE_SAS_Inp = __mlib_ImageBlend_ONE_SAS_Inp
#pragma weak mlib_ImageBlend_ONE_SA_Inp = __mlib_ImageBlend_ONE_SA_Inp
#pragma weak mlib_ImageBlend_ONE_ZERO_Inp = __mlib_ImageBlend_ONE_ZERO_Inp
#pragma weak mlib_ImageBlend_SA_DA_Inp = __mlib_ImageBlend_SA_DA_Inp
#pragma weak mlib_ImageBlend_SA_DC_Inp = __mlib_ImageBlend_SA_DC_Inp
#pragma weak mlib_ImageBlend_SA_OMDA_Inp = __mlib_ImageBlend_SA_OMDA_Inp
#pragma weak mlib_ImageBlend_SA_OMDC_Inp = __mlib_ImageBlend_SA_OMDC_Inp
#pragma weak mlib_ImageBlend_SA_OMSA_Inp = __mlib_ImageBlend_SA_OMSA_Inp
#pragma weak mlib_ImageBlend_SA_ONE_Inp = __mlib_ImageBlend_SA_ONE_Inp
#pragma weak mlib_ImageBlend_SA_SAS_Inp = __mlib_ImageBlend_SA_SAS_Inp
#pragma weak mlib_ImageBlend_SA_SA_Inp = __mlib_ImageBlend_SA_SA_Inp
#pragma weak mlib_ImageBlend_SA_ZERO_Inp = __mlib_ImageBlend_SA_ZERO_Inp
#pragma weak mlib_ImageBlend_SC_DA_Inp = __mlib_ImageBlend_SC_DA_Inp
#pragma weak mlib_ImageBlend_SC_DC_Inp = __mlib_ImageBlend_SC_DC_Inp
#pragma weak mlib_ImageBlend_SC_OMDA_Inp = __mlib_ImageBlend_SC_OMDA_Inp
#pragma weak mlib_ImageBlend_SC_OMDC_Inp = __mlib_ImageBlend_SC_OMDC_Inp
#pragma weak mlib_ImageBlend_SC_OMSA_Inp = __mlib_ImageBlend_SC_OMSA_Inp
#pragma weak mlib_ImageBlend_SC_ONE_Inp = __mlib_ImageBlend_SC_ONE_Inp
#pragma weak mlib_ImageBlend_SC_SAS_Inp = __mlib_ImageBlend_SC_SAS_Inp
#pragma weak mlib_ImageBlend_SC_SA_Inp = __mlib_ImageBlend_SC_SA_Inp
#pragma weak mlib_ImageBlend_SC_ZERO_Inp = __mlib_ImageBlend_SC_ZERO_Inp
#pragma weak mlib_ImageBlend_ZERO_DA_Inp = __mlib_ImageBlend_ZERO_DA_Inp
#pragma weak mlib_ImageBlend_ZERO_DC_Inp = __mlib_ImageBlend_ZERO_DC_Inp
#pragma weak mlib_ImageBlend_ZERO_OMDA_Inp = __mlib_ImageBlend_ZERO_OMDA_Inp
#pragma weak mlib_ImageBlend_ZERO_OMDC_Inp = __mlib_ImageBlend_ZERO_OMDC_Inp
#pragma weak mlib_ImageBlend_ZERO_OMSA_Inp = __mlib_ImageBlend_ZERO_OMSA_Inp
#pragma weak mlib_ImageBlend_ZERO_ONE_Inp = __mlib_ImageBlend_ZERO_ONE_Inp
#pragma weak mlib_ImageBlend_ZERO_SAS_Inp = __mlib_ImageBlend_ZERO_SAS_Inp
#pragma weak mlib_ImageBlend_ZERO_SA_Inp = __mlib_ImageBlend_ZERO_SA_Inp
#pragma weak mlib_ImageBlend_ZERO_ZERO_Inp = __mlib_ImageBlend_ZERO_ZERO_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend_DA_DA_Inp) mlib_ImageBlend_DA_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_DA_Inp")));
__typeof__(__mlib_ImageBlend_DA_DC_Inp) mlib_ImageBlend_DA_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_DC_Inp")));
__typeof__(__mlib_ImageBlend_DA_OMDA_Inp) mlib_ImageBlend_DA_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_DA_OMDC_Inp) mlib_ImageBlend_DA_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_DA_OMSA_Inp) mlib_ImageBlend_DA_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_DA_ONE_Inp) mlib_ImageBlend_DA_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_ONE_Inp")));
__typeof__(__mlib_ImageBlend_DA_SAS_Inp) mlib_ImageBlend_DA_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_SAS_Inp")));
__typeof__(__mlib_ImageBlend_DA_SA_Inp) mlib_ImageBlend_DA_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_SA_Inp")));
__typeof__(__mlib_ImageBlend_DA_ZERO_Inp) mlib_ImageBlend_DA_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_DA_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_DA_Inp) mlib_ImageBlend_OMDA_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_DA_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_DC_Inp) mlib_ImageBlend_OMDA_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_DC_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_OMDA_Inp) mlib_ImageBlend_OMDA_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_OMDC_Inp) mlib_ImageBlend_OMDA_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_OMSA_Inp) mlib_ImageBlend_OMDA_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_ONE_Inp) mlib_ImageBlend_OMDA_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_ONE_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_SAS_Inp) mlib_ImageBlend_OMDA_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_SAS_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_SA_Inp) mlib_ImageBlend_OMDA_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_SA_Inp")));
__typeof__(__mlib_ImageBlend_OMDA_ZERO_Inp) mlib_ImageBlend_OMDA_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMDA_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_DA_Inp) mlib_ImageBlend_OMSA_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_DA_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_DC_Inp) mlib_ImageBlend_OMSA_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_DC_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_OMDA_Inp) mlib_ImageBlend_OMSA_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_OMDC_Inp) mlib_ImageBlend_OMSA_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_OMSA_Inp) mlib_ImageBlend_OMSA_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_ONE_Inp) mlib_ImageBlend_OMSA_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_ONE_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_SAS_Inp) mlib_ImageBlend_OMSA_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_SAS_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_SA_Inp) mlib_ImageBlend_OMSA_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_SA_Inp")));
__typeof__(__mlib_ImageBlend_OMSA_ZERO_Inp) mlib_ImageBlend_OMSA_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSA_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_DA_Inp) mlib_ImageBlend_OMSC_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_DA_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_DC_Inp) mlib_ImageBlend_OMSC_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_DC_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_OMDA_Inp) mlib_ImageBlend_OMSC_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_OMDC_Inp) mlib_ImageBlend_OMSC_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_OMSA_Inp) mlib_ImageBlend_OMSC_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_ONE_Inp) mlib_ImageBlend_OMSC_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_ONE_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_SAS_Inp) mlib_ImageBlend_OMSC_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_SAS_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_SA_Inp) mlib_ImageBlend_OMSC_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_SA_Inp")));
__typeof__(__mlib_ImageBlend_OMSC_ZERO_Inp) mlib_ImageBlend_OMSC_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_OMSC_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_ONE_DA_Inp) mlib_ImageBlend_ONE_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_DA_Inp")));
__typeof__(__mlib_ImageBlend_ONE_DC_Inp) mlib_ImageBlend_ONE_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_DC_Inp")));
__typeof__(__mlib_ImageBlend_ONE_OMDA_Inp) mlib_ImageBlend_ONE_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_ONE_OMDC_Inp) mlib_ImageBlend_ONE_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_ONE_OMSA_Inp) mlib_ImageBlend_ONE_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_ONE_ONE_Inp) mlib_ImageBlend_ONE_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_ONE_Inp")));
__typeof__(__mlib_ImageBlend_ONE_SAS_Inp) mlib_ImageBlend_ONE_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_SAS_Inp")));
__typeof__(__mlib_ImageBlend_ONE_SA_Inp) mlib_ImageBlend_ONE_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_SA_Inp")));
__typeof__(__mlib_ImageBlend_ONE_ZERO_Inp) mlib_ImageBlend_ONE_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ONE_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_SA_DA_Inp) mlib_ImageBlend_SA_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_DA_Inp")));
__typeof__(__mlib_ImageBlend_SA_DC_Inp) mlib_ImageBlend_SA_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_DC_Inp")));
__typeof__(__mlib_ImageBlend_SA_OMDA_Inp) mlib_ImageBlend_SA_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_SA_OMDC_Inp) mlib_ImageBlend_SA_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_SA_OMSA_Inp) mlib_ImageBlend_SA_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_SA_ONE_Inp) mlib_ImageBlend_SA_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_ONE_Inp")));
__typeof__(__mlib_ImageBlend_SA_SAS_Inp) mlib_ImageBlend_SA_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_SAS_Inp")));
__typeof__(__mlib_ImageBlend_SA_SA_Inp) mlib_ImageBlend_SA_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_SA_Inp")));
__typeof__(__mlib_ImageBlend_SA_ZERO_Inp) mlib_ImageBlend_SA_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SA_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_SC_DA_Inp) mlib_ImageBlend_SC_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_DA_Inp")));
__typeof__(__mlib_ImageBlend_SC_DC_Inp) mlib_ImageBlend_SC_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_DC_Inp")));
__typeof__(__mlib_ImageBlend_SC_OMDA_Inp) mlib_ImageBlend_SC_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_SC_OMDC_Inp) mlib_ImageBlend_SC_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_SC_OMSA_Inp) mlib_ImageBlend_SC_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_SC_ONE_Inp) mlib_ImageBlend_SC_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_ONE_Inp")));
__typeof__(__mlib_ImageBlend_SC_SAS_Inp) mlib_ImageBlend_SC_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_SAS_Inp")));
__typeof__(__mlib_ImageBlend_SC_SA_Inp) mlib_ImageBlend_SC_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_SA_Inp")));
__typeof__(__mlib_ImageBlend_SC_ZERO_Inp) mlib_ImageBlend_SC_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_SC_ZERO_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_DA_Inp) mlib_ImageBlend_ZERO_DA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_DA_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_DC_Inp) mlib_ImageBlend_ZERO_DC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_DC_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_OMDA_Inp) mlib_ImageBlend_ZERO_OMDA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_OMDA_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_OMDC_Inp) mlib_ImageBlend_ZERO_OMDC_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_OMDC_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_OMSA_Inp) mlib_ImageBlend_ZERO_OMSA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_OMSA_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_ONE_Inp) mlib_ImageBlend_ZERO_ONE_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_ONE_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_SAS_Inp) mlib_ImageBlend_ZERO_SAS_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_SAS_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_SA_Inp) mlib_ImageBlend_ZERO_SA_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_SA_Inp")));
__typeof__(__mlib_ImageBlend_ZERO_ZERO_Inp) mlib_ImageBlend_ZERO_ZERO_Inp
    __attribute__((weak, alias("__mlib_ImageBlend_ZERO_ZERO_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_ZERO_Inp - blend two images with
 *      ZERO source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_ZERO_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type,
 *      the same size and the same number of channels.
 *      They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * 0
 */

mlib_status
__mlib_ImageBlend_ZERO_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_ONE_Inp - blend two images with
 *      ONE source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_ONE_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * 1
 */

mlib_status
__mlib_ImageBlend_ZERO_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_DC_Inp - blend two images with
 *      DC source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_DC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_ZERO_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_OMDC_Inp - blend two images with
 *      OMDC source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_OMDC_Inp(mlib_image     *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_ZERO_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_SA_Inp - blend two images with
 *      SA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_SA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_ZERO_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_OMSA_Inp - blend two images with
 *      OMSA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_OMSA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_ZERO_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_DA_Inp - blend two images with
 *      DA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_DA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_ZERO_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_OMDA_Inp - blend two images with
 *      OMDA source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_OMDA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_ZERO_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ZERO_SAS_Inp - blend two images with
 *      SAS source blend factor and ZERO destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ZERO_SAS_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 0 + src2 * (f, f, f, 1),
 *                      f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_ZERO_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ZERO_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_ZERO_Inp - blend two images with
 *      ZERO source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_ZERO_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * 0
 */

mlib_status
__mlib_ImageBlend_ONE_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_ONE_Inp - blend two images with
 *      ONE source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_ONE_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * 1
 */

mlib_status
__mlib_ImageBlend_ONE_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_DC_Inp - blend two images with
 *      DC source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_DC_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_ONE_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_OMDC_Inp - blend two images with
 *      OMDC source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_OMDC_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_ONE_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_SA_Inp - blend two images with
 *      SA source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_SA_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_ONE_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_OMSA_Inp - blend two images with
 *      OMSA source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_OMSA_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_ONE_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_DA_Inp - blend two images with
 *      DA source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_DA_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_ONE_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_OMDA_Inp - blend two images with
 *      OMDA source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_OMDA_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_ONE_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_ONE_SAS_Inp - blend two images with
 *      SAS source blend factor and ONE destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_ONE_SAS_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * 1 + src2 * (f, f, f, 1),
 *                      f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_ONE_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_ONE_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_ZERO_Inp - blend two images with
 *      ZERO source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_ZERO_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * 0
 */

mlib_status
__mlib_ImageBlend_SC_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_ONE_Inp - blend two images with
 *      ONE source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_ONE_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * 1
 */

mlib_status
__mlib_ImageBlend_SC_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_DC_Inp - blend two images with
 *      DC source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_DC_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_SC_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_OMDC_Inp - blend two images with
 *      OMDC source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_OMDC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_SC_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_SA_Inp - blend two images with
 *      SA source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_SA_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_SC_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_OMSA_Inp - blend two images with
 *      OMSA source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_OMSA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_SC_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_DA_Inp - blend two images with
 *      DA source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_DA_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_SC_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_OMDA_Inp - blend two images with
 *      OMDA source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_OMDA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_SC_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SC_SAS_Inp - blend two images with
 *      SAS source blend factor and SC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SC_SAS_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * src2 + src2 * (f, f, f, 1),
 *                      f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_SC_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SC_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_ZERO_Inp - blend two images with
 *      ZERO source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_ZERO_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * 0
 */

mlib_status
__mlib_ImageBlend_OMSC_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_ONE_Inp - blend two images with
 *      ONE source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_ONE_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * 1
 */

mlib_status
__mlib_ImageBlend_OMSC_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_DC_Inp - blend two images with
 *      DC source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_DC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_OMSC_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_OMDC_Inp - blend two images with
 *      OMDC source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_OMDC_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_OMSC_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_SA_Inp - blend two images with
 *      SA source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_SA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_OMSC_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_OMSA_Inp - blend two images with
 *      OMSA source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_OMSA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_OMSC_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_DA_Inp - blend two images with
 *      DA source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_DA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_OMSC_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_OMDA_Inp - blend two images with
 *      OMDA source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_OMDA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_OMSC_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSC_SAS_Inp - blend two images with
 *      SAS source blend factor and OMSC destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSC_SAS_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - src2) + src2 * (f, f, f, 1),
 *                      f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_OMSC_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSC_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_ZERO_Inp - blend two images with
 *      ZERO source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_ZERO_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * 0
 */

mlib_status
__mlib_ImageBlend_SA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_ONE_Inp - blend two images with
 *      ONE source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_ONE_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * 1
 */

mlib_status
__mlib_ImageBlend_SA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_DC_Inp - blend two images with
 *      DC source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_DC_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_SA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_OMDC_Inp - blend two images with
 *      OMDC source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_OMDC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_SA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_SA_Inp - blend two images with
 *      SA source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_SA_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_SA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_OMSA_Inp - blend two images with
 *      OMSA source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_OMSA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_SA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_DA_Inp - blend two images with
 *      DA source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_DA_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_SA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_OMDA_Inp - blend two images with
 *      OMDA source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_OMDA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_SA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_SA_SAS_Inp - blend two images with
 *      SAS source blend factor and SA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_SA_SAS_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc2 + src2 * (f, f, f, 1),
 *                      f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_SA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_SA_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_ZERO_Inp - blend two images with
 *      ZERO source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_ZERO_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * 0
 */

mlib_status
__mlib_ImageBlend_OMSA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_ONE_Inp - blend two images with
 *      ONE source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_ONE_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * 1
 */

mlib_status
__mlib_ImageBlend_OMSA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_DC_Inp - blend two images with
 *      DC source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_DC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_OMSA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_OMDC_Inp - blend two images with
 *      OMDC source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_OMDC_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_OMSA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_SA_Inp - blend two images with
 *      SA source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_SA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_OMSA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_OMSA_Inp - blend two images with
 *      OMSA source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_OMSA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_OMSA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_DA_Inp - blend two images with
 *      DA source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_DA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_OMSA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_OMDA_Inp - blend two images with
 *      OMDA source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_OMDA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_OMSA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMSA_SAS_Inp - blend two images with
 *      SAS source blend factor and OMSA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMSA_SAS_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc2) + src2 * (f, f, f, 1),
 *              f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_OMSA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMSA_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_ZERO_Inp - blend two images with
 *      ZERO source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_ZERO_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * 0
 */

mlib_status
__mlib_ImageBlend_DA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_ONE_Inp - blend two images with
 *      ONE source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_ONE_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * 1
 */

mlib_status
__mlib_ImageBlend_DA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_DC_Inp - blend two images with
 *      DC source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_DC_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_DA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_OMDC_Inp - blend two images with
 *      OMDC source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_OMDC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_DA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_SA_Inp - blend two images with
 *      SA source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_SA_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_DA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_OMSA_Inp - blend two images with
 *      OMSA source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_OMSA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_DA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_DA_Inp - blend two images with
 *      DA source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_DA_Inp(mlib_image       *src1dst,
 *                                            const mlib_image *src2,
 *                                            mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_DA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_OMDA_Inp - blend two images with
 *      OMDA source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_OMDA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_DA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_DA_SAS_Inp - blend two images with
 *      SAS source blend factor and DA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_DA_SAS_Inp(mlib_image       *src1dst,
 *                                             const mlib_image *src2,
 *                                             mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * ALPHAsrc1dst + src2 * (f, f, f, 1),
 *                              f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_DA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_DA_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_ZERO_Inp - blend two images with
 *      ZERO source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_ZERO_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * 0
 */

mlib_status
__mlib_ImageBlend_OMDA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_ZERO(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_ONE_Inp - blend two images with
 *      ONE source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_ONE_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * 1
 */

mlib_status
__mlib_ImageBlend_OMDA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_ONE(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_DC_Inp - blend two images with
 *      DC source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_DC_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * src1dst
 */

mlib_status
__mlib_ImageBlend_OMDA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_DC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_OMDC_Inp - blend two images with
 *      OMDC source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_OMDC_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * (1 - src1dst)
 */

mlib_status
__mlib_ImageBlend_OMDA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_OMDC(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_SA_Inp - blend two images with
 *      SA source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_SA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * ALPHAsrc2
 */

mlib_status
__mlib_ImageBlend_OMDA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_SA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_OMSA_Inp - blend two images with
 *      OMSA source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_OMSA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * (1 - ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_OMDA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_OMSA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_DA_Inp - blend two images with
 *      DA source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_DA_Inp(mlib_image       *src1dst,
 *                                              const mlib_image *src2,
 *                                              mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * ALPHAsrc1dst
 */

mlib_status
__mlib_ImageBlend_OMDA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_DA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_OMDA_Inp - blend two images with
 *      OMDA source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_OMDA_Inp(mlib_image       *src1dst,
 *                                                const mlib_image *src2,
 *                                                mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * (1 - ALPHAsrc1dst)
 */

mlib_status
__mlib_ImageBlend_OMDA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_OMDA(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */

/*
 * FUNCTION
 *      mlib_ImageBlend_OMDA_SAS_Inp - blend two images with
 *      SAS source blend factor and OMDA destination blend factor
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageBlend_OMDA_SAS_Inp(mlib_image       *src1dst,
 *                                               const mlib_image *src2,
 *                                               mlib_s32         cmask)
 *
 * ARGUMENT
 *      src1dst pointer to first input and output image
 *      src2    pointer to second input image
 *      cmask   Channel mask to indicate the alpha channel.
 *
 * RESTRICTION
 *      src1dst, src2, and dst must be the same type and
 *      the same size. They can have three or four channels.
 *      They must be MLIB_BYTE data type.
 *      They must be 3 or 4 channels.
 *      cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *      If a color buffer has no ALPHA value, then it is as if the
 *      destination ALPHA value is 1.
 *
 * DESCRIPTION
 *      src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * (f, f, f, 1),
 *      f = min(1 - ALPHAsrc1dst, ALPHAsrc2)
 */

mlib_status
__mlib_ImageBlend_OMDA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask)
{
	return (__mlib_ImageBlend_OMDA_SAS(src1dst, src1dst, src2, cmask));
}

/* *********************************************************** */
