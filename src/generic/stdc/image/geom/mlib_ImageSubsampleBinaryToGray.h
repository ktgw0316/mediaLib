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

#ifndef _MLIB_IMAGESUBSAMPLEBINARYTOGRAY_H
#define	_MLIB_IMAGESUBSAMPLEBINARYTOGRAY_H

#pragma ident	"@(#)mlib_ImageSubsampleBinaryToGray.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageCopy.h>

extern const mlib_u32 mlib_edgemask4[];
extern const mlib_u32 mlib_nbits1[];
extern const mlib_u32 mlib_nbits2[];
extern const mlib_u16 mlib_nbits3[];
extern const mlib_u16 mlib_nbits4[];

mlib_status __mlib_ImageSubsampleBinaryToGray_ty(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray);

mlib_status mlib_ImageSubsampleBinaryToGray_4x4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray);

mlib_status mlib_ImageSubsampleBinaryToGray_2x2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGESUBSAMPLEBINARYTOGRAY_H */
