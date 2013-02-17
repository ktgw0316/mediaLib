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

#ifndef _MLIB_I_SIGNALSIGGEN_H
#define	_MLIB_I_SIGNALSIGGEN_H

#pragma ident	"@(#)mlib_i_SignalSiggen.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */

#define	MLIB_SINE_S16	11

/* *********************************************************** */

typedef struct mlib_i_sinewave_s16
{
	mlib_s32 type, flag;
	mlib_s64 w_cstep8;
	mlib_s64 cstep;
	mlib_s64 cstep1, cstep8, cstep16, cstep32, cstep64, cstep128;
	mlib_s64 sbase1, sbase2, sbase3, sbase4;
	mlib_s64 sbase5, sbase6, sbase7, sbase8;
} mlib_i_sinewave_s16_struct;

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_SIGNALSIGGEN_H */
