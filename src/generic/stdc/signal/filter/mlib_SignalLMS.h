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

#ifndef _MLIB_SIGNALLMS_H
#define	_MLIB_SIGNALLMS_H

#pragma ident	"@(#)mlib_SignalLMS.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */

#define	POWER	931.379420637e-12
#define	LMS_BUFFER_SIZE	512

/* *********************************************************** */

void mlib_SignalFIR_s16_to_d64S(
    mlib_d64 *pdst0,
    mlib_d64 *pdst1,
    mlib_s16 *psrc,
    mlib_s32 n);

void mlib_SignalFIR_d64_to_s16S(
    mlib_s16 *pdst,
    mlib_d64 *psrc0,
    mlib_d64 *psrc1,
    mlib_s32 n);

mlib_status __mlib_SignalLMSInit_common(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta,
    mlib_d64 beta_scale,
    mlib_d64 scale,
    mlib_s32 stereo);

typedef struct
{
	mlib_s32 tap;
	mlib_s32 bufferSize;
	mlib_d64 U;
	mlib_d64 BE[2];
	mlib_d64 pow[2];
	mlib_d64 *data;
} LMS_Filter;

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALLMS_H */
