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

#ifndef _MLIB_I_IMAGECONSTDIV_H
#define	_MLIB_I_IMAGECONSTDIV_H

#pragma ident	"@(#)mlib_i_ImageConstDiv.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

extern const mlib_f32 mlib_1DIVU8_F32[];

mlib_status mlib_i_ImageConstDiv_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts);

mlib_status mlib_i_ImageConstDiv_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts);

mlib_status mlib_i_ImageConstDiv_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts);

mlib_status mlib_i_ImageConstDiv_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts);

mlib_status mlib_ImageDivShift_U8(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGECONSTDIV_H */
