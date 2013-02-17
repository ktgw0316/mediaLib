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

#pragma ident	"@(#)mlib_c_ImageConvMxNIndex.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageConvIndex.h>

/* *********************************************************** */

ftype_convMxNIndex_nw func_convMxNIndex_nw[] = {
	mlib_convMxNIndex_8_8nw,
	mlib_convMxNIndex_8_16nw,
	mlib_convMxNIndex_16_8nw,
	mlib_convMxNIndex_16_16nw,
};

ftype_convMxNIndex_ext func_convMxNIndex_ext[] = {
	mlib_convMxNIndex_8_8ext,
	mlib_convMxNIndex_8_16ext,
	mlib_convMxNIndex_16_8ext,
	mlib_convMxNIndex_16_16ext,
};

/* *********************************************************** */
