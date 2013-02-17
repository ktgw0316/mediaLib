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

#pragma ident	"@(#)mlib_v_ImageConvMxNIndex.c	9.2	07/11/05 SMI"

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageConv.h>
#include <mlib_ImageConvIndex.h>
#include <mlib_v_ImageConvIndex.h>

/* *********************************************************** */

mlib_status
mlib_v_convMxNIndex_8_8nw(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap)
{
	if (mlib_ImageConvVersion(m, n, scale, MLIB_BYTE) == 0)
		return mlib_convMxNIndex_8_8nw(src, dst, m, n, dm, dn,
		    kern, scale, colormap);

	if (mlib_ImageGetLutChannels(colormap) == 3) {
		return mlib_convMxN_Index3_8_8nw(dst, src, m, n, dm, dn,
		    kern, scale, colormap);
	} else {
		return mlib_convMxN_Index4_8_8nw(dst, src, m, n, dm, dn,
		    kern, scale, colormap);
	}
}

/* *********************************************************** */

mlib_status
mlib_v_convMxNIndex_8_16nw(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap)
{
	if (mlib_ImageConvVersion(m, n, scale, MLIB_BYTE) == 0)
		return mlib_convMxNIndex_8_16nw(src, dst, m, n, dm, dn,
		    kern, scale, colormap);

	if (mlib_ImageGetLutChannels(colormap) == 3) {
		return mlib_convMxN_Index3_8_16nw(dst, src, m, n, dm, dn,
		    kern, scale, colormap);
	} else {
		return mlib_convMxN_Index4_8_16nw(dst, src, m, n, dm, dn,
		    kern, scale, colormap);
	}
}

/* *********************************************************** */

mlib_status
mlib_v_convMxNIndex_8_8ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap)
{
	if (mlib_ImageConvVersion(m, n, scale, MLIB_BYTE) == 0)
		return mlib_convMxNIndex_8_8ext(src, dst, m, n,
		    dx_l, dx_r, dy_t, dy_b, kern, scale, colormap);

	if (mlib_ImageGetLutChannels(colormap) == 3) {
		return mlib_convMxN_Index3_8_8ext(dst, src, m, n,
		    dx_l, dx_r, dy_t, dy_b, kern, scale, colormap);
	} else {
		return mlib_convMxN_Index4_8_8ext(dst, src, m, n,
		    dx_l, dx_r, dy_t, dy_b, kern, scale, colormap);
	}
}

/* *********************************************************** */

mlib_status
mlib_v_convMxNIndex_8_16ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap)
{
	if (mlib_ImageConvVersion(m, n, scale, MLIB_BYTE) == 0)
		return mlib_convMxNIndex_8_16ext(src, dst, m, n,
		    dx_l, dx_r, dy_t, dy_b, kern, scale, colormap);

	if (mlib_ImageGetLutChannels(colormap) == 3) {
		return mlib_convMxN_Index3_8_16ext(dst, src, m, n,
		    dx_l, dx_r, dy_t, dy_b, kern, scale, colormap);
	} else {
		return mlib_convMxN_Index4_8_16ext(dst, src, m, n,
		    dx_l, dx_r, dy_t, dy_b, kern, scale, colormap);
	}
}

/* *********************************************************** */

ftype_convMxNIndex_nw func_convMxNIndex_nw[] = {
	mlib_v_convMxNIndex_8_8nw,
	mlib_v_convMxNIndex_8_16nw,
	mlib_convMxNIndex_16_8nw,
	mlib_convMxNIndex_16_16nw,
};

ftype_convMxNIndex_ext func_convMxNIndex_ext[] = {
	mlib_v_convMxNIndex_8_8ext,
	mlib_v_convMxNIndex_8_16ext,
	mlib_convMxNIndex_16_8ext,
	mlib_convMxNIndex_16_16ext,
};

/* *********************************************************** */
