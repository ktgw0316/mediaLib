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

#ifndef _MLIB_S_IMAGEFILTERS_H
#define	_MLIB_S_IMAGEFILTERS_H

#pragma ident	"@(#)mlib_s_ImageFilters.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *    These tables are used by SSE2 version
 *    of the following functions:
 *      mlib_ImageRotate(Index)
 *      mlib_ImageAffine(Index)
 *      mlib_ImageZoom(Index)
 *      mlib_ImageGridWarp
 *      mlib_ImagePolynomialWarp
 */

#include <mlib_image.h>

extern const mlib_s16 mlib_filters_u8_bl[],
    mlib_filters_u8_bc[],
    mlib_filters_u8_bc2[],
    mlib_filters_u8_bc_3[],
    mlib_filters_u8_bc2_3[],
    mlib_filters_u8_bc_4[],
    mlib_filters_u8_bc2_4[],
    mlib_filters_s16_bc[],
    mlib_filters_s16_bc2[],
    mlib_filters_s16_bc_3[],
    mlib_filters_s16_bc2_3[],
    mlib_filters_s16_bc_4[],
    mlib_filters_s16_bc2_4[];

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEFILTERS_H */
