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

#ifndef _MLIB_SIGNAL_TYPES_H
#define	_MLIB_SIGNAL_TYPES_H

#pragma ident	"@(#)mlib_signal_types.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	MLIB_DTW_ITAKURA = 0
} mlib_dtw_local;

typedef enum
{
	MLIB_DTW_NONE = 0
} mlib_dtw_slope;

typedef enum
{
	MLIB_DTW_L1NORM = 0,
	MLIB_DTW_L2NORM = 1
} mlib_dtw_norm;

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNAL_TYPES_H */
