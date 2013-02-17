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

#pragma ident	"@(#)mlib_c_SignalLPCCov_S162F32.c	9.2	07/10/09 SMI"

#include <mlib_types.h>
#include <mlib_SignalLPC.h>

/* *********************************************************** */

void
mlib_SignalLPCCov_S162F32(
    mlib_f32 *signal_fp,
    const mlib_s16 *signal,
    mlib_s32 length)
{
	mlib_s32 i;
	mlib_d64 *dp = (void *)signal_fp;
	mlib_s16 *sp = (void *)signal;
	union
	{
		mlib_d64 db;
		struct
		{
			mlib_f32 f0, f1;
		} ft;
	} rs;

	if ((mlib_addr)sp & 3) {
		*(mlib_f32 *)dp = (*sp++);
		length--;
		dp = (void *)((mlib_f32 *)dp + 1);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < length - 1; i += 2) {

		rs.ft.f0 = (*sp++);
		rs.ft.f1 = (*sp++);

		(*dp++) = rs.db;
	}

	if (length & 1) {
		*(mlib_f32 *)dp = *sp;
	}
}

/* *********************************************************** */
