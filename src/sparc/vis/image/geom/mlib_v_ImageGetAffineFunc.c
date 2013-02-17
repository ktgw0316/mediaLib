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

#pragma ident	"@(#)mlib_v_ImageGetAffineFunc.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageGetAffineFunc - returns pointer to VIS function
 *      if it is possible to use VIS
 */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>

/* *********************************************************** */

fun_type_nw
mlib_ImageAffine_GetFunc(
    mlib_affine_workspace *ws,
    mlib_interp_table *tbl)
{
	if ((ws->type == MLIB_BYTE) && (tbl->width != 1) &&
	    (tbl->height != 1)) {

		if (mlib_ImageInitInterpTableAffine_S16(tbl,
		    ws->nchan) != MLIB_SUCCESS)
			return (NULL);
		return (mlib_v_ImageAffineTable_8nw);
	}
	return (NULL);
}

/* *********************************************************** */
