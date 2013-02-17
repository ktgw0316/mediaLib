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

#ifndef _MLIB_SIGNALCONVERTSHIFT_H
#define	_MLIB_SIGNALCONVERTSHIFT_H

#pragma ident	"@(#)mlib_SignalConvertShift.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_Utils.h>

#define	BUFSIZE	512
/* mlib_f32 res = 2 ** shift */
/* available shift range -126 - +127 */
#define	SCALBN(res, shift)                                      \
	res = 0.f;                                              \
	((mlib_s32 *)&res)[0] = (127 + shift) << 23
#define	PREPARE_CONST(dst, shift)                               \
	{                                                       \
	    mlib_s32 e_shift = shift;                           \
	                                                        \
	    if (e_shift < -100)                                 \
		e_shift = -100;                                 \
	    if (e_shift > 100)                                  \
		e_shift = 100;                                  \
	    SCALBN(dst, e_shift);                               \
	}
#define	ITOF(reg)	((mlib_f32)*(mlib_s32 *)&(reg))

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALCONVERTSHIFT_H */
