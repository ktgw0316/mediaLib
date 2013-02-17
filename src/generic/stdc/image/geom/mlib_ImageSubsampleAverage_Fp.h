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

#ifndef _MLIB_IMAGESUBSAMPLEAVERAGE_FP_H
#define	_MLIB_IMAGESUBSAMPLEAVERAGE_FP_H

#pragma ident	"@(#)mlib_ImageSubsampleAverage_Fp.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

/* mlib_ImageSubsampleAverage_Fp.c */

mlib_status __mlib_ImageSubsampleAverage_ty_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin);

mlib_status mlib_ImageSubsampleAverage_D64_main(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin);

mlib_status mlib_ImageSubsampleAverage_F32_main(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGESUBSAMPLEAVERAGE_FP_H */
