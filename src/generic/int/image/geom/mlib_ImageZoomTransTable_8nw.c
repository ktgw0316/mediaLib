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

#pragma ident	"@(#)mlib_ImageZoomTransTable_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTable.
 */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

/*
 *  These defines switch between functions of different data types:
 *  Files mlib_ImageZoomTranslateTable_8ext.c,
 *        mlib_ImageZoomTranslateTable_16nw.c,
 *        mlib_ImageZoomTranslateTable_u16nw.c,
 *        mlib_ImageZoomTranslateTable_32nw.c,
 *        mlib_ImageZoomTranslateTable_F32nw.c,
 *        mlib_ImageZoomTranslateTable_D64nw.c,
 *        mlib_ImageZoomTranslateTable_8ext.c,
 *        mlib_ImageZoomTranslateTable_16ext.c,
 *        mlib_ImageZoomTranslateTable_u16ext.c,
 *        mlib_ImageZoomTranslateTable_32ext.c,
 *        mlib_ImageZoomTranslateTable_F32ext.c,
 *        mlib_ImageZoomTranslateTable_D64ext.c
 */

/* *********************************************************** */

/* #define SRC_EXTEND */
#define	LOW_SPECIAL

/* *********************************************************** */

#define	FLT_BITS	2
#define	IMG_TYPE	1

#define	FUNC_NAME	mlib_ImageZoomTranslateTable_8nw

#include <mlib_i_ImageZoomTranslateTable_f.h>

/* *********************************************************** */
