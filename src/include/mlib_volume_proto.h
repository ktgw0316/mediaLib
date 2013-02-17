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

#ifndef _MLIB_VOLUME_PROTO_H
#define	_MLIB_VOLUME_PROTO_H

#pragma ident	"@(#)mlib_volume_proto.h	9.3	07/10/09 SMI"

#include <mlib_types.h>
#include <mlib_status.h>
#include <mlib_volume_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  RAY CASTING */

mlib_status mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16(
    mlib_rays *rays,
    const mlib_blkvolume *blk,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

mlib_status mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit(
    mlib_rays *rays,
    const mlib_genvolume *vol,
    void *buffer);

/*  WINDOW-LEVEL OPERATION */

mlib_status mlib_VolumeWindowLevel(
    mlib_u8 *dst,
    const mlib_s16 *src,
    mlib_s32 window,
    mlib_s32 level,
    mlib_s32 gmax,
    mlib_s32 gmin,
    mlib_s32 len);

/*  MAXIMUM INTENSITY SEARCHING */

mlib_status mlib_VolumeFindMax_U8(
    mlib_u8 *max,
    const mlib_rays *rays);

mlib_status mlib_VolumeFindMax_S16(
    mlib_s16 *max,
    const mlib_rays *rays);

mlib_status mlib_VolumeFindMaxCMask_U8(
    mlib_u8 *max,
    const mlib_rays *rays,
    const mlib_u8 *cmask,
    mlib_s32 thresh);

mlib_status mlib_VolumeFindMaxCMask_S16(
    mlib_s16 *max,
    const mlib_rays *rays,
    const mlib_u8 *cmask,
    mlib_s32 thresh);

mlib_status mlib_VolumeFindMaxBMask_U8(
    mlib_u8 *max,
    const mlib_rays *rays,
    const mlib_u8 *bmask);

mlib_status mlib_VolumeFindMaxBMask_S16(
    mlib_s16 *max,
    const mlib_rays *rays,
    const mlib_u8 *bmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VOLUME_PROTO_H */
