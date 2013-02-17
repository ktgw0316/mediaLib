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

#pragma ident	"@(#)mlib_s_ImageGetZoomFunc.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate_GetFunc - returns pointer to SSE
 *                                        function if it is possible
 *                                        to use SSE
 */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

mlib_status mlib_s_ImageZoomTranslateTable(
    PARAMS_NW);

/* *********************************************************** */

fun_type_nw
mlib_ImageZoomTranslate_GetFunc(
    mlib_zoom_workspace * ws,
    mlib_interp_table * table)
{
/* if (table->precisionBits > 8) return NULL; */

	if (ws->type == MLIB_BYTE || ws->type == MLIB_SHORT ||
	    ws->type == MLIB_USHORT) {
		if (mlib_ImageInitInterpTable_S16(table) != MLIB_SUCCESS) {
			return (NULL);
		}
		return (mlib_s_ImageZoomTranslateTable);
	}

	return (NULL);
}

/* *********************************************************** */
