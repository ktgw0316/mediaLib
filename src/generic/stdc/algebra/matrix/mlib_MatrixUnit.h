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

#ifndef _MLIB_MATRIXUNIT_H
#define	_MLIB_MATRIXUNIT_H

#pragma ident	"@(#)mlib_MatrixUnit.h	9.2	07/10/08 SMI"

#ifdef __cplusplus
extern "C" {
#endif

mlib_status
mlib_MatrixUnit_S_U8(
	mlib_u8 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_S8(
	mlib_s8 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_S16(
	mlib_s16 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_S32(
	mlib_s32 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_U8C(
	mlib_u8 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_S8C(
	mlib_s8 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_S16C(
	mlib_s16 *z,
	mlib_s32 n,
	mlib_s32 ns);

mlib_status
mlib_MatrixUnit_S_S32C(
	mlib_s32 *z,
	mlib_s32 n,
	mlib_s32 ns);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_MATRIXUNIT_H */
