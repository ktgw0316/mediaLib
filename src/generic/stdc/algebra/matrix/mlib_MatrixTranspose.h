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

#ifndef _MLIB_MATRIXTRANSPOSE_H
#define	_MLIB_MATRIXTRANSPOSE_H

#pragma ident	"@(#)mlib_MatrixTranspose.h	9.2	07/10/08 SMI"

#ifdef __cplusplus
extern "C" {
#endif

mlib_status
mlib_MatrixTranspose_S_U8_U8(
	mlib_u8 *zz,
	const mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
mlib_status
mlib_MatrixTranspose_S_S16_S16(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
mlib_status
mlib_MatrixTranspose_S_S32_S32(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
mlib_status
mlib_MatrixTranspose_S_U8C_U8C(
	mlib_u8 *zz,
	const mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
mlib_status
mlib_MatrixTranspose_S_S16C_S16C(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);
mlib_status
mlib_MatrixTranspose_S_S32C_S32C(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

mlib_status
mlib_MatrixTranspose_S_U8(
	mlib_u8 *zz,
	mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

mlib_status
mlib_MatrixTranspose_S_S16(
	mlib_s16 *zz,
	mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

mlib_status
mlib_MatrixTranspose_S_S32(
	mlib_s32 *zz,
	mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

mlib_status
mlib_MatrixTranspose_S_U8C(
	mlib_u8 *zz,
	mlib_u8 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

mlib_status
mlib_MatrixTranspose_S_S16C(
	mlib_s16 *zz,
	mlib_s16 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

mlib_status
mlib_MatrixTranspose_S_S32C(
	mlib_s32 *zz,
	mlib_s32 *xx,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ds);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_MATRIXTRANSPOSE_H */
