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

#ifndef _MLIB_V_SIGNALARITH_H
#define	_MLIB_V_SIGNALARITH_H

#pragma ident	"@(#)mlib_v_SignalArith.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

typedef union
{
	mlib_d64 db;
	struct
	{
		mlib_s32 int0;
		mlib_s32 int1;
	} two_int;
	struct
	{
		mlib_f32 fl0;
		mlib_f32 fl1;
	} two_float;
} type_union_mlib_d64;

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_SIGNALARITH_H */
