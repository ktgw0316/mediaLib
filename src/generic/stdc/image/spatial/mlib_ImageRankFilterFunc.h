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

#ifndef _MLIB_IMAGERANKFILTERFUNC_H
#define	_MLIB_IMAGERANKFILTERFUNC_H

#pragma ident	"@(#)mlib_ImageRankFilterFunc.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageRankFilterMxN_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank,
    mlib_s32 stride_d,
    mlib_s32 stride_s);

mlib_status mlib_ImageRankFilterMxN_U16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank,
    mlib_s32 stride_d,
    mlib_s32 stride_s);

void mlib_fill_MxN_s32(
    mlib_u8 *pd,
    mlib_s32 value,
    mlib_s32 size);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGERANKFILTERFUNC_H */
