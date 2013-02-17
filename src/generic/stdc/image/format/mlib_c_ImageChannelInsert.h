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

#ifndef _MLIB_C_IMAGECHANNELINSERT_H
#define	_MLIB_C_IMAGECHANNELINSERT_H

#pragma ident	"@(#)mlib_c_ImageChannelInsert.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

void mlib_c_ImageChannelInsert_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_c_ImageChannelInsert_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_c_ImageChannelInsert_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_c_ImageChannelInsert_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGECHANNELINSERT_H */
