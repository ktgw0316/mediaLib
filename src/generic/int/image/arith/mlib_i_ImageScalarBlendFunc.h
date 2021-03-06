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

#ifndef _MLIB_I_IMAGESCALARBLENDFUNC_H
#define	_MLIB_I_IMAGESCALARBLENDFUNC_H

#pragma ident	"@(#)mlib_i_ImageScalarBlendFunc.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>


void mlib_i_ImageScalarBlend_S16_1(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_S16_2(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_S16_3(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_S16_4(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_U16_1(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_U16_2(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_U16_3(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

void mlib_i_ImageScalarBlend_U16_4(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGESCALARBLENDFUNC_H */
