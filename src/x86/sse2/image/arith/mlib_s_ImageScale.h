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

#ifndef _MLIB_S_IMAGESCALE_H
#define	_MLIB_S_IMAGESCALE_H

#pragma ident	"@(#)mlib_s_ImageScale.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>

mlib_s32
mlib_ImageScale_CheckForSSE2(
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	mlib_s32 shift,
	mlib_type stype,
	mlib_type dtype,
	mlib_s32 nchan);

mlib_status
mlib_s_ImageScale_u8_u8(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_u8 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u8_u8_fast(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_u8 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u8_s16(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_u8 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u8_u16(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_u8 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u8_s32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_u8 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s16_u8(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s16_s16(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s16_u16(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s16_s32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u16_u8(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_u16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u16_s16(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_u16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u16_u16(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_u16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_u16_s32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_u16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s32_u8(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_s32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s32_s16(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_s32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s32_u16(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_s32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

mlib_status
mlib_s_ImageScale_s32_s32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_s32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGESCALE_H */
