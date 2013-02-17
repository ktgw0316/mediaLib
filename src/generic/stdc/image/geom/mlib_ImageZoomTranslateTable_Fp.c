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

#pragma ident	"@(#)mlib_ImageZoomTranslateTable_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateTable_Fp - image scaling using interpolation
 *                                        table
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageZoomTranslateTable_Fp(mlib_image       *dst,
 *                                                  const mlib_image *src,
 *                                                  mlib_d64         zoomx,
 *                                                  mlib_d64         zoomy,
 *                                                  mlib_d64         tx,
 *                                                  mlib_d64         ty,
 *                                                  const void       *table,
 *                                                  mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      tx        X translation.
 *      ty        Y translation.
 *      table     Pointer to interpolation table structure
 *      edge      Type of edge condition
 *
 * DESCRIPTION
 *                      xd = zoomx*xs + tx
 *                      yd = zoomy*ys + ty
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_EXTEND_INDEF
 *    MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT
 *      or MLIB_DOUBLE data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslateTable_Fp = __mlib_ImageZoomTranslateTable_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomTranslateTable_Fp) mlib_ImageZoomTranslateTable_Fp
    __attribute__((weak, alias("__mlib_ImageZoomTranslateTable_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslateTable_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    const void *table,
    mlib_edge edge)
{
	mlib_type type = mlib_ImageGetType(dst);

	if (type != MLIB_FLOAT && type != MLIB_DOUBLE) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageZoomTranslateTable_alltypes(dst, src, zoomx, zoomy, tx,
	    ty, table, edge);
}

/* *********************************************************** */
