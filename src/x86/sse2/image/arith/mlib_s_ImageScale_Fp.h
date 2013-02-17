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

#ifndef _MLIB_S_IMAGESCALE_FP_H
#define	_MLIB_S_IMAGESCALE_FP_H

#pragma ident	"@(#)mlib_s_ImageScale_Fp.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>

void
mlib_s_ImageScale_Fp_u8_f32(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_u8_d64(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_s16_f32(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_s16_d64(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_u16_f32(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_u16_d64(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_s32_f32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_s32_d64(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_f32_f32(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_f32_d64(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_d64_f32(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

void
mlib_s_ImageScale_Fp_d64_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGESCALE_FP_H */
