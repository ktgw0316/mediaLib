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

#ifndef _MLIB_VECTORMULM_PROTO_H
#define	_MLIB_VECTORMULM_PROTO_H

#pragma ident	"@(#)mlib_VectorMulM_proto.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

mlib_status mlib_VectorMulM_S_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16_S8_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32_S16_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulM_S_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride);
mlib_status mlib_VectorMulMShift_S_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride,
	mlib_s32 shift);
mlib_status mlib_VectorMulMShift_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride,
	mlib_s32 shift);
mlib_status mlib_VectorMulMShift_S_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride,
	mlib_s32 shift);
mlib_status mlib_VectorMulMShift_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride,
	mlib_s32 shift);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VECTORMULM_PROTO_H */
