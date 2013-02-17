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

#ifndef _MLIB_IMAGECOPY_H
#define	_MLIB_IMAGECOPY_H

#pragma ident	"@(#)mlib_ImageCopy.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

void mlib_ImageCopy_bit_al(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset);

void mlib_ImageCopy_na(
    const mlib_u8 *sp,
    mlib_u8 *dp,
    mlib_s32 n);

void mlib_ImageCopy_bit_na_r(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 s_offset,
    mlib_s32 d_offset);

void mlib_ImageCopy_bit_na(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 s_offset,
    mlib_s32 d_offset);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGECOPY_H */
