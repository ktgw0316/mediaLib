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

#ifndef _MLIB_S_IMAGESCALARBLEND_FP_H
#define	_MLIB_S_IMAGESCALARBLEND_FP_H

#pragma ident	"@(#)mlib_s_ImageScalarBlend_Fp.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>

#define	MLIB_C_IMAGESCALARBLEND(src1, src2, alpha)		\
		((alpha) * (src1) + (1.0 - (alpha)) * (src2))

void
mlib_s_ImageScalarBlend_Fp_f32(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src1,
	mlib_s32 slb1,
	const mlib_f32 *src2,
	mlib_s32 slb2,
	const mlib_d64 *alpha,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan);

void
mlib_s_ImageScalarBlend_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src1,
	mlib_s32 slb1,
	const mlib_d64 *src2,
	mlib_s32 slb2,
	const mlib_d64 *alpha,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGESCALARBLEND_FP_H */
