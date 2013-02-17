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

#ifndef _MLIB_VIDEOWAVELETTWOTENTRANS_F_H
#define	_MLIB_VIDEOWAVELETTWOTENTRANS_F_H

#pragma ident	"@(#)mlib_VideoWaveletTwoTenTrans_f.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif
mlib_status mlib_1_Level_FTTT_S16_U8(
	mlib_s16 *dst,
	const mlib_u8 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_FTTT_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_FTTT_S32_S16(
	mlib_s32 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_FTTT_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_ITTT_U8_S16(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_ITTT_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_ITTT_S16_S32(
	mlib_s16 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height);

mlib_status mlib_1_Level_ITTT_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VIDEOWAVELETTWOTENTRANS_F_H */
