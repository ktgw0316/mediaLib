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

#pragma ident	"@(#)mlib_v_ImageExtrema2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageExtrema2_fast  - fast version of mlib_ImageExtrema2
 *                                 (by VIS version
 *                                 of mlib_ImageMinimum and mlib_ImageMaximum)
 */

#include <mlib_image.h>

/* *********************************************************** */

mlib_status
mlib_ImageExtrema2_fast(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img)
{
	mlib_status res;

	if (mlib_ImageGetType(img) == MLIB_INT)
		return (MLIB_FAILURE);

	res = __mlib_ImageMinimum(min, img);

	if (res != MLIB_SUCCESS)
		return (res);

	res = __mlib_ImageMaximum(max, img);

	if (res != MLIB_SUCCESS)
		return (res);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
