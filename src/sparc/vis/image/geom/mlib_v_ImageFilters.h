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

#ifndef _MLIB_V_IMAGEFILTERS_H
#define	_MLIB_V_IMAGEFILTERS_H

#pragma ident	"@(#)mlib_v_ImageFilters.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/*
 *    These tables are used by VIS versions
 *    of the following functions:
 *      mlib_ImageRotate(Index)
 *      mlib_ImageAffine(Index)
 *      mlib_ImageZoom(Index)
 *      mlib_ImageGridWarp
 *      mlib_ImagePolynomialWarp
 */

#if defined(__INIT_TABLE)

#pragma align 8(mlib_filters_u8_bl)
#pragma align 8(mlib_filters_u8_bc)
#pragma align 8(mlib_filters_u8_bc2)
#pragma align 8(mlib_filters_u8_bc_3)
#pragma align 8(mlib_filters_u8_bc2_3)
#pragma align 8(mlib_filters_u8_bc_4)
#pragma align 8(mlib_filters_u8_bc2_4)
#pragma align 8(mlib_filters_s16_bc)
#pragma align 8(mlib_filters_s16_bc2)
#pragma align 8(mlib_filters_s16_bc_3)
#pragma align 8(mlib_filters_s16_bc2_3)
#pragma align 8(mlib_filters_s16_bc_4)
#pragma align 8(mlib_filters_s16_bc2_4)

#endif		   /* defined(__INIT_TABLE) */

extern const mlib_s16 mlib_filters_u8_bl[];
extern const mlib_s16 mlib_filters_u8_bc[];
extern const mlib_s16 mlib_filters_u8_bc2[];
extern const mlib_s16 mlib_filters_u8_bc_3[];
extern const mlib_s16 mlib_filters_u8_bc2_3[];
extern const mlib_s16 mlib_filters_u8_bc_4[];
extern const mlib_s16 mlib_filters_u8_bc2_4[];
extern const mlib_s16 mlib_filters_s16_bc[];
extern const mlib_s16 mlib_filters_s16_bc2[];
extern const mlib_s16 mlib_filters_s16_bc_3[];
extern const mlib_s16 mlib_filters_s16_bc2_3[];
extern const mlib_s16 mlib_filters_s16_bc_4[];
extern const mlib_s16 mlib_filters_s16_bc2_4[];

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEFILTERS_H */
