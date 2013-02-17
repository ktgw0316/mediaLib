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

#pragma ident	"@(#)mlib_v_SignalLPCCov_S162F32.c	9.2	07/11/05 SMI"

#include <mlib_signal.h>
#include <mlib_SignalLPC.h>
#include <vis_proto.h>

/* *********************************************************** */

void
mlib_SignalLPCCov_S162F32(
    mlib_f32 *signal_fp,
    const mlib_s16 *signal,
    mlib_s32 length)
{
	mlib_s32 i;
	mlib_u8 *dp = (void *)signal_fp;
	mlib_u8 *sp = (void *)signal;
	mlib_f32 fone = vis_to_float(0x00010001);
	mlib_f32 ftmp;
	union
	{
		mlib_d64 db;
		struct
		{
			mlib_f32 f0, f1;
		} ft;
	} rs;
	union
	{
		mlib_d64 db;
		struct
		{
			mlib_s32 s0, s1;
		} it;
	} ss;

	if ((mlib_addr)sp & 3) {
		*(mlib_f32 *)dp = *(mlib_s16 *)sp;
		length--;
		sp += 2;
		dp += 4;
	}
#pragma pipeloop(0)
	for (i = 0; i < length - 1; i += 2) {

		ftmp = *(mlib_f32 *)sp;
		ss.db = vis_fmuld8ulx16(fone, ftmp);
		rs.ft.f0 = ss.it.s0;
		rs.ft.f1 = ss.it.s1;
		*(mlib_d64 *)dp = rs.db;
		sp += 4;
		dp += 8;
	}

	if (length & 1) {
		*(mlib_f32 *)dp = *(mlib_s16 *)sp;
	}
}

/* *********************************************************** */
