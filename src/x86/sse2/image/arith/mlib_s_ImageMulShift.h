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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEMULSHIFT_H
#define	_MLIB_S_IMAGEMULSHIFT_H

#pragma ident	"@(#)mlib_s_ImageMulShift.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>


void mlib_s_ImageMulShift_U8(
    mlib_u8 *sp1,
    mlib_s32 stride1,
    mlib_u8 *sp2,
    mlib_s32 stride2,
    mlib_u8 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift);

void mlib_s_ImageMulShift_S16(
    mlib_s16 *sp1,
    mlib_s32 stride1,
    mlib_s16 *sp2,
    mlib_s32 stride2,
    mlib_s16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift);

void mlib_s_ImageMulShift_U16(
    mlib_u16 *sp1,
    mlib_s32 stride1,
    mlib_u16 *sp2,
    mlib_s32 stride2,
    mlib_u16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift);


#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEMULSHIFT_H */
