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

#ifndef _MLIB_ALGEBRAUTIL_H
#define	_MLIB_ALGEBRAUTIL_H

#pragma ident	"@(#)mlib_AlgebraUtil.h	9.2	07/10/08 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <mlib_SysMath.h>

extern const mlib_d64 mlib_SU82D64[];
extern const mlib_d64 mlib_S82D64[];
extern const mlib_d64 mlib_constarctgf[];

/* *********************************************************** */

typedef union
{
	mlib_d64 db;
#ifndef _NO_LONGLONG
	mlib_s64 ll;
#endif		   /* _NO_LONGLONG */
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_s32 int1, int0;
#else		   /* _LITTLE_ENDIAN */
		mlib_s32 int0, int1;
#endif		   /* _LITTLE_ENDIAN */
	} two_int;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_f32 fl1, fl0;
#else		   /* _LITTLE_ENDIAN */
		mlib_f32 fl0, fl1;
#endif		   /* _LITTLE_ENDIAN */
	} two_float;
} type_union_mlib_d64;

/* *********************************************************** */

enum mlibTypes
{
	type_U8,
	type_S8,
	type_S16,
	type_S32,
	type_U8C,
	type_S8C,
	type_S16C,
	type_S32C
};

/* *********************************************************** */

enum mlibModes
{
	mode_Sat,
	mode_Mod
};

/* *********************************************************** */

mlib_d64 mlib_atan2i(
    mlib_s32 yy,
    mlib_s32 xx);

void mlib_type_to_D64(
    mlib_s32 type,
    const void *x,
    mlib_s32 n,
    mlib_d64 *dx);

mlib_s32 mlib_D64_to_S32_Mod(
    mlib_d64 x);

void mlib_VectorD64_to_type(
    mlib_s32 type,
    mlib_s32 mode,
    const mlib_d64 *dx,
    mlib_s32 n,
    void *x);

void mlib_S32_to_type_Sat(
    mlib_s32 in,
    mlib_s32 type_out,
    void *out,
    mlib_s32 index);

void mlib_S32_to_type_Mod(
    mlib_s32 in,
    mlib_s32 type_out,
    void *out,
    mlib_s32 index);

mlib_status mlib_MatrixMul_type(
    mlib_s32 type_in,
    mlib_s32 type_out,
    mlib_s32 mode,
    const void *x,
    const void *y,
    mlib_s32 m,
    mlib_s32 l,
    mlib_s32 n,
    mlib_s32 ns,
    void *z);

mlib_status mlib_MatrixMulShift_type(
    mlib_s32 type_in,
    mlib_s32 type_out,
    mlib_s32 mode,
    const void *x,
    const void *y,
    mlib_s32 m,
    mlib_s32 l,
    mlib_s32 n,
    mlib_s32 ns,
    void *z,
    mlib_s32 shift);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_ALGEBRAUTIL_H */
