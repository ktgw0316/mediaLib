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

#ifndef _MLIB_IMAGEEXTREMA_H
#define	_MLIB_IMAGEEXTREMA_H

#pragma ident	"@(#)mlib_ImageExtrema.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

void mlib_ImageMaximum_S32_124(
    mlib_s32 *res,
    const mlib_image *img);

void mlib_ImageMaximum_S32_3(
    mlib_s32 *res,
    const mlib_image *img);

void mlib_ImageMinimum_S32_124(
    mlib_s32 *res,
    const mlib_image *img);

void mlib_ImageMinimum_S32_3(
    mlib_s32 *res,
    const mlib_image *img);

mlib_status mlib_ImageExtrema2_fast(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEEXTREMA_H */
