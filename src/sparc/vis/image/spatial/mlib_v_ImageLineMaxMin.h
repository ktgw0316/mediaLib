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

#ifndef _MLIB_V_IMAGELINEMAXMIN_H
#define	_MLIB_V_IMAGELINEMAXMIN_H

#pragma ident	"@(#)mlib_v_ImageLineMaxMin.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_ImageLineMax(
    mlib_u8 *dp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineSortMax(
    mlib_u8 *dp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineMax_SrcNa2(
    mlib_u8 *dp,
    const mlib_u8 *tp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineMax1(
    mlib_u8 *dp,
    mlib_s32 n);

void mlib_ImageLineMax_SrcNa(
    mlib_u8 *dp,
    const mlib_u8 *tp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineMin(
    mlib_u8 *dp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineSortMin(
    mlib_u8 *dp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineMin_SrcNa2(
    mlib_u8 *dp,
    const mlib_u8 *tp,
    const mlib_u8 *sp,
    mlib_s32 n);

void mlib_ImageLineMin1(
    mlib_u8 *dp,
    mlib_s32 n);

void mlib_ImageLineMin_SrcNa(
    mlib_u8 *dp,
    const mlib_u8 *tp,
    const mlib_u8 *sp,
    mlib_s32 n);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGELINEMAXMIN_H */
