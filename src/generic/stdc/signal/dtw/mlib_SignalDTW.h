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

#ifndef _MLIB_SIGNALDTW_H
#define	_MLIB_SIGNALDTW_H

#pragma ident	"@(#)mlib_SignalDTW.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_signal.h>

/* *********************************************************** */

typedef struct
{
	mlib_f32 *dref, *dobs;
	mlib_s32 lref, lobs, lbuf, ldist;
	mlib_s32 delta;
	mlib_f32 *lo0, *lo1, *lo2, *lo3;
	mlib_f32 *mi0, *mi1, *mi2, *mi3;
	mlib_f32 *path_buf;
	mlib_f32 *dists;
	mlib_s32 mt_flag;
} DTWScalar_state;

typedef struct
{
	mlib_f32 *dref, *dobs;
	mlib_s32 lref, lobs, lbuf, ldist;
	mlib_s32 delta;
	mlib_f32 *lo0, *lo1, *lo2, *lo3;
	mlib_f32 *mi0, *mi1, *mi2, *mi3;
	mlib_f32 *path_buf;
	mlib_f32 *dists;
	mlib_s32 mt_flag;
	mlib_f32 *preabs;
	mlib_s32 ndata, dtype;
} DTWVector_state;


#define	CALC_Y_MIN_MAX(j)                                       \
	{                                                       \
	    mlib_s32 f0, f1, f2, f3;                            \
	                                                        \
	    f0 = (j) >> 1;                                      \
	    f1 = (((j) - (lobs - 1)) * 2) + lref - 1 - delta;   \
	    f2 = ((j) << 1) + delta;                            \
	    f3 = (((j) - (lobs - 1)) / 2) + lref - 1;           \
	    y_min = f0 > f1 ? f0 : f1;                          \
	    y_max = f2 < f3 ? f2 : f3;                          \
	}

#define	PSEUDO_INF	(MLIB_F32_MAX)

void mlib_SignalDTWScalarCalcDists(
	void *state,
	const mlib_f32 *dobs,
	mlib_s32 lobs,
	mlib_s32 i0,
	mlib_s32 i1);
void
mlib_SignalDTWVectorCalcDists(
	void *state,
	const mlib_f32 *dobs,
	mlib_s32 lobs,
	mlib_s32 i0,
	mlib_s32 i1);

void
mlib_SignalDTWDistJoin(
	mlib_d64 *dist,
	mlib_s32 lobs,
	void *state);

void
mlib_SignalDTWPathJoin(
	mlib_d64 *dist,
	mlib_s32 *path,
	mlib_s32 *lpath,
	mlib_s32 lobs,
	void *state);

void
mlib_SignalDTWDistFwd(
	mlib_s32 lobs,
	void *state);

void
mlib_SignalDTWDistBwd(
	mlib_s32 lobs,
	void *state);

void
mlib_SignalDTWPathFwd(
	mlib_s32 lobs,
	void *state);

void
mlib_SignalDTWPathBwd(
	mlib_s32 lobs,
	void *state);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALDTW_H */
