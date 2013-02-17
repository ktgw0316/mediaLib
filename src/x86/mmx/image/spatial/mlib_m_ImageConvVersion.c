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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImageConvVersion.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConvVersion - Get Conv* functions version
 *      0  - "C" version
 *      1  - "VIS" version
 *      2  - "i386" version
 *      3  - "MMX" version
 *
 * SYNOPSIS
 *      mlib_s32 mlib_ImageConvVersion(mlib_s32 m,
 *                                     mlib_s32 n,
 *                                     mlib_s32 scale,
 *                                     mlib_type type)
 */

#include <mlib_image.h>

/* *********************************************************** */

#define	MAX_U8	8
#define	MAX_S16	32

/* *********************************************************** */

mlib_s32
mlib_ImageConvVersion(
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    mlib_type type)
{
/* 16 < scale <= 31 */
	mlib_d64 dscale = 1.0 / (1 << scale);

	if (type == MLIB_BYTE) {
		if ((m * n * dscale * 32768.0) > MAX_U8)
			return (0);
		if ((m * n * dscale * 32768.0 * 1024.0) > MAX_U8)
			return (2);
		return (3);
	} else if ((type == MLIB_USHORT) || (type == MLIB_SHORT)) {

		if ((m * n * dscale * 32768.0 * 32768.0) > MAX_S16)
			return (0);
		return (3);
	} else
		return (0);
}

/* *********************************************************** */
