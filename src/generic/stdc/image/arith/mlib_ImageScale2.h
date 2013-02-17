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

#ifndef _MLIB_IMAGESCALE2_H
#define	_MLIB_IMAGESCALE2_H

#pragma ident	"@(#)mlib_ImageScale2.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

mlib_s32 mlib_ImageScale_CheckForVIS(
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift,
    mlib_type stype,
    mlib_type dtype,
    mlib_s32 nchan);

mlib_status mlib_ImageScale2_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

mlib_status mlib_ImageScale2_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

mlib_status mlib_ImageScale2_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

mlib_status mlib_ImageScale2_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

mlib_status mlib_ImageScale2_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

mlib_status mlib_ImageDConstMul_S32(
    mlib_s32 *dl,
    mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *dconsts);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGESCALE2_H */
