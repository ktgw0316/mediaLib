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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageNotOr_Bit.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNotOr_Bit     - notor two bit type images  (C version)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNotOr_Bit(mlib_image       *dst,
 *                                      const mlib_image *src1,
 *                                      const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src1    pointer to first source image
 *      src2    pointer to second source image
 *
 * RESTRICTION
 *     The src1, src2, and dst must be the same type and the same size.
 *     They can have 1, 2, 3, or 4 channels.
 *     They can be in MLIB_BIT data type.
 *
 * DESCRIPTION
 *     NotOr two images for each band:   dst = ~(src1 | src2)
 */

/* *********************************************************** */

#define	OPERATION(a1, a2)	~((a1) | (a2))
#define	OPERATION_SSE2(a1, a2)	_mm_andnot_si128(_mm_or_si128(a1, a2), ones)

/* *********************************************************** */

#define	LOGIC_BITS                                                     \
	mlib_status                                                    \
	mlib_ImageNotOr_Bit(mlib_image *dst, const mlib_image *src1,   \
	    const mlib_image *src2)

#include <mlib_s_ImageLogic_Bit.h>

/* *********************************************************** */
