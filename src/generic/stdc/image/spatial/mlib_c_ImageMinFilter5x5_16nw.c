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

#pragma ident	"@(#)mlib_c_ImageMinFilter5x5_16nw.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMinFilter5x5_S16 - internal function for
 *      mlib_ImageMinFilter5x5
 *
 */

#include <mlib_image.h>

/* *********************************************************** */

#define	MAX_FILTER	0

/* *********************************************************** */

#define	FTYPE	FTYPE_S16
#define	STYPE	mlib_s16

#define	TTYPE	mlib_s32

#include <mlib_c_ImageMaxMinFilter5x5.h>

/* *********************************************************** */
