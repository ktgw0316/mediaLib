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

#ifndef _MLIB_I_IMAGEMOMENT2_H
#define	_MLIB_I_IMAGEMOMENT2_H

#pragma ident	"@(#)mlib_i_ImageMoment2.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

void mlib_c_ImageMoment2_U8_124(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_U8_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_U16_124(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_U16_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_S16_124(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_S16_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_S32_124(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_c_ImageMoment2_S32_3(
    const mlib_image *img,
    mlib_d64 *rez);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGEMOMENT2_H */
