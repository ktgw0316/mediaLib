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

#pragma ident	"@(#)mlib_c_ImageConv_f.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>

/* *********************************************************** */

#define	MLIB_PARAMS_CONV_NW                                             \
	mlib_image *dst, const mlib_image *src, const mlib_s32 *kern,   \
	mlib_s32 scale, mlib_s32 cmask

/* *********************************************************** */

#define	MLIB_CALL_PARAMS_CONV_NW	dst, src, kern, scale, cmask

/* *********************************************************** */

#define	MLIB_PARAMS_CONV_EXT                                       \
	mlib_image *dst, const mlib_image *src, mlib_s32 dx_l,     \
	mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,               \
	const mlib_s32 *kern, mlib_s32 scale, mlib_s32 cmask

/* *********************************************************** */

#define	MLIB_CALL_PARAMS_CONV_EXT                               \
	dst, src, dx_l, dx_r, dy_t, dy_b, kern, scale, cmask

/* *********************************************************** */

#define	MLIB_PARAMS_CONV_MN_NW                                          \
	mlib_image *dst, const mlib_image *src, const mlib_s32 *kern,   \
	mlib_s32 m, mlib_s32 n, mlib_s32 dm, mlib_s32 dn,               \
	mlib_s32 scale, mlib_s32 cmask

/* *********************************************************** */

#define	MLIB_CALL_PARAMS_CONV_MN_NW                             \
	dst, src, kern, m, n, dm, dn, scale, cmask

/* *********************************************************** */

#define	MLIB_PARAMS_CONV_MN_EXT                                            \
	mlib_image *dst, const mlib_image *src, const mlib_s32 *kern,      \
	mlib_s32 m, mlib_s32 n, mlib_s32 dx_l, mlib_s32 dx_r,              \
	mlib_s32 dy_t, mlib_s32 dy_b, mlib_s32 scale, mlib_s32 cmask

/* *********************************************************** */

#define	MLIB_CALL_PARAMS_CONV_MN_EXT                                 \
	dst, src, kern, m, n, dx_l, dx_r, dy_t, dy_b, scale, cmask

/* *********************************************************** */

mlib_status
mlib_conv2x2nw_u8(
    MLIB_PARAMS_CONV_NW)
{
	return (mlib_c_conv2x2nw_u8(MLIB_CALL_PARAMS_CONV_NW));
}

/* *********************************************************** */

mlib_status
mlib_conv3x3nw_u8(
    MLIB_PARAMS_CONV_NW)
{
#ifdef __sparc
	return (mlib_c_conv3x3nw_u8(MLIB_CALL_PARAMS_CONV_NW));
#else /* __sparc */

	if (mlib_ImageConvVersion(3, 3, scale, MLIB_BYTE) == 0)
		return (mlib_c_conv3x3nw_u8(MLIB_CALL_PARAMS_CONV_NW));
	else
		return (mlib_i_conv3x3nw_u8(MLIB_CALL_PARAMS_CONV_NW));
#endif /* __sparc */
}

/* *********************************************************** */

mlib_status
mlib_conv4x4nw_u8(
    MLIB_PARAMS_CONV_NW)
{
	return (mlib_c_conv4x4nw_u8(MLIB_CALL_PARAMS_CONV_NW));
}

/* *********************************************************** */

mlib_status
mlib_conv5x5nw_u8(
    MLIB_PARAMS_CONV_NW)
{
#ifdef __sparc
	return (mlib_c_conv5x5nw_u8(MLIB_CALL_PARAMS_CONV_NW));
#else /* __sparc */

	if (mlib_ImageConvVersion(5, 5, scale, MLIB_BYTE) == 0)
		return (mlib_c_conv5x5nw_u8(MLIB_CALL_PARAMS_CONV_NW));
	else
		return (mlib_i_conv5x5nw_u8(MLIB_CALL_PARAMS_CONV_NW));
#endif /* __sparc */
}

/* *********************************************************** */

mlib_status
mlib_conv7x7nw_u8(
    MLIB_PARAMS_CONV_NW)
{
	return (mlib_c_conv7x7nw_u8(MLIB_CALL_PARAMS_CONV_NW));
}

/* *********************************************************** */

mlib_status
mlib_convMxNnw_u8(
    MLIB_PARAMS_CONV_MN_NW)
{
#ifdef __sparc
	return (mlib_c_convMxNnw_u8(MLIB_CALL_PARAMS_CONV_MN_NW));
#else /* __sparc */

	if (mlib_ImageConvVersion(m, n, scale, MLIB_BYTE) == 0)
		return (mlib_c_convMxNnw_u8(MLIB_CALL_PARAMS_CONV_MN_NW));
	else
		return (mlib_i_convMxNnw_u8(MLIB_CALL_PARAMS_CONV_MN_NW));
#endif /* __sparc */
}

/* *********************************************************** */

mlib_status
mlib_conv2x2ext_u8(
    MLIB_PARAMS_CONV_EXT)
{
	return (mlib_c_conv2x2ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
}

/* *********************************************************** */

mlib_status
mlib_conv3x3ext_u8(
    MLIB_PARAMS_CONV_EXT)
{
#ifdef __sparc
	return (mlib_c_conv3x3ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
#else /* __sparc */

	if (mlib_ImageConvVersion(3, 3, scale, MLIB_BYTE) == 0)
		return (mlib_c_conv3x3ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
	else
		return (mlib_i_conv3x3ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
#endif /* __sparc */
}

/* *********************************************************** */

mlib_status
mlib_conv4x4ext_u8(
    MLIB_PARAMS_CONV_EXT)
{
	return (mlib_c_conv4x4ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
}

/* *********************************************************** */

mlib_status
mlib_conv5x5ext_u8(
    MLIB_PARAMS_CONV_EXT)
{
#ifdef __sparc
	return (mlib_c_conv5x5ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
#else /* __sparc */

	if (mlib_ImageConvVersion(5, 5, scale, MLIB_BYTE) == 0)
		return (mlib_c_conv5x5ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
	else
		return (mlib_i_conv5x5ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
#endif /* __sparc */
}

/* *********************************************************** */

mlib_status
mlib_conv7x7ext_u8(
    MLIB_PARAMS_CONV_EXT)
{
	return (mlib_c_conv7x7ext_u8(MLIB_CALL_PARAMS_CONV_EXT));
}

/* *********************************************************** */

mlib_status
mlib_convMxNext_u8(
    MLIB_PARAMS_CONV_MN_EXT)
{
#ifdef __sparc
	return (mlib_c_convMxNext_u8(MLIB_CALL_PARAMS_CONV_MN_EXT));
#else /* __sparc */

	if (mlib_ImageConvVersion(m, n, scale, MLIB_BYTE) == 0)
		return (mlib_c_convMxNext_u8(MLIB_CALL_PARAMS_CONV_MN_EXT));
	else
		return (mlib_i_convMxNext_u8(MLIB_CALL_PARAMS_CONV_MN_EXT));
#endif /* __sparc */
}

/* *********************************************************** */

mlib_status
mlib_conv2x2nw_s16(
    MLIB_PARAMS_CONV_NW)
{
	return (mlib_c_conv2x2nw_s16(MLIB_CALL_PARAMS_CONV_NW));
}

/* *********************************************************** */

mlib_status
mlib_conv2x2nw_u16(
    MLIB_PARAMS_CONV_NW)
{
	return (mlib_c_conv2x2nw_u16(MLIB_CALL_PARAMS_CONV_NW));
}

/* *********************************************************** */

mlib_status
mlib_conv2x2ext_s16(
    MLIB_PARAMS_CONV_EXT)
{
	return (mlib_c_conv2x2ext_s16(MLIB_CALL_PARAMS_CONV_EXT));
}

/* *********************************************************** */

mlib_status
mlib_conv2x2ext_u16(
    MLIB_PARAMS_CONV_EXT)
{
	return (mlib_c_conv2x2ext_u16(MLIB_CALL_PARAMS_CONV_EXT));
}

/* *********************************************************** */
