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

#ifndef _MLIB_VIDEOCOLORAVDRESIZE_H
#define	_MLIB_VIDEOCOLORAVDRESIZE_H

#pragma ident	"@(#)mlib_VideoColorAVDResize.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif
void mlib_VideoZoom_U8_4_Copy(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 slb);

void mlib_VideoZoom_U8_4_Nearest(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 slb);

void mlib_VideoZoom_U8_4_Bilinear(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 slb);

void mlib_VideoZoom_U8_4_Bicubic(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 slb);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VIDEOCOLORAVDRESIZE_H */
