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

#ifndef _MLIB_IMAGEROWTABLE_H
#define	_MLIB_IMAGEROWTABLE_H

#pragma ident	"@(#)mlib_ImageRowTable.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image_types.h>

void *mlib_ImageCreateRowTable(
    mlib_image *image);
void mlib_ImageDeleteRowTable(
    mlib_image *image);

static void *mlib_ImageGetRowTable(
    mlib_image *img)
{
	return (img->state);
}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEROWTABLE_H */
