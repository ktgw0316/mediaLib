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

#pragma ident	"@(#)mlib_v_ImageConv_16ext.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageConv* on U8/S16/U16 type and
 *   MLIB_EDGE_SRC_EXTEND mask
 */

#include <mlib_image.h>

/* *********************************************************** */

/*
 * This define switches between functions of different data types
 */
#define	IMG_TYPE	2

#include <mlib_v_ImageConv_ext.h>

/* *********************************************************** */
