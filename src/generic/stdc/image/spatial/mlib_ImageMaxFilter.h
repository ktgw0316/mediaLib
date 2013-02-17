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

#ifndef _MLIB_IMAGEMAXFILTER_H
#define	_MLIB_IMAGEMAXFILTER_H

#pragma ident	"@(#)mlib_ImageMaxFilter.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageMaxFilter3x3_BIT(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt,
    mlib_s32 dbit_off,
    mlib_s32 sbit_off);

mlib_status mlib_ImageMaxFilter3x3_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_U16(
    mlib_u16 *dst,
    const mlib_u16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_S32(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_D64(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter5x5_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter5x5_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter5x5_U16(
    mlib_u16 *dst,
    const mlib_u16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter5x5_S32(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter5x5_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter5x5_D64(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter7x7_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter7x7_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter7x7_U16(
    mlib_u16 *dst,
    const mlib_u16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter7x7_S32(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter7x7_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter7x7_D64(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h);

mlib_status mlib_ImageMaxFilter3x3_BIT(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt,
    mlib_s32 dbit_off,
    mlib_s32 sbit_off);

mlib_status mlib_ImageMaxFilter3x3_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_U16(
    mlib_u16 *dst,
    const mlib_u16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_S32(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

mlib_status mlib_ImageMaxFilter3x3_D64(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEMAXFILTER_H */
