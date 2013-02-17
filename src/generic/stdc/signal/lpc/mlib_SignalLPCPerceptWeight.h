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

#ifndef _MLIB_SIGNALLPCPERCEPTWEIGHT_H
#define	_MLIB_SIGNALLPCPERCEPTWEIGHT_H

#pragma ident	"@(#)mlib_SignalLPCPerceptWeight.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

    typedef struct
{
	mlib_s32 order;
	mlib_s32 length;
	mlib_d64 *zero;
	mlib_d64 *pole;
	mlib_d64 *ZeroTable;
	mlib_d64 *PoleTable;
	mlib_d64 *src_buf;
	mlib_d64 *fir_buf;
	mlib_d64 *dst_buf;
} filter_state;

void mlib_PerceptWeightOrderIsSmall(
    mlib_d64 *dst_buf,
    const mlib_d64 *src_buf,
    filter_state * filter);

void mlib_FIR_part(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    filter_state * pfilter);

void mlib_IIR_part(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    filter_state * pfilter);

void
mlib_FIR_part_Main(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    mlib_d64 *delay,
    mlib_s32 length,
    mlib_s32 order);

void
__mlib_FIR_part_Main(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    mlib_d64 *delay,
    mlib_s32 length,
    mlib_s32 order);

void
__mlib_FIR_part_Main_i(
    mlib_d64 *dst,
    const mlib_d64 *src,
    const mlib_d64 *lpc,
    mlib_d64 *delay,
    mlib_s32 length,
    mlib_s32 order,
    mlib_s32 i0,
    mlib_s32 i1);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALLPCPERCEPTWEIGHT_H */
