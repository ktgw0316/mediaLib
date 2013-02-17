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

#pragma ident	"@(#)mlib_ImageConvolveMxN_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConvolveMxN_Fp - image convolution with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConvolveMxN_Fp(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           const mlib_d64   *kernel,
 *                                           mlib_s32         m,
 *                                           mlib_s32         n,
 *                                           mlib_s32         dm,
 *                                           mlib_s32         dn,
 *                                           mlib_s32         cmask,
 *                                           mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      kernel    Pointer to convolution kernel.
 *      m         Kernel width (m must be not less than 1).
 *      n         Kernel height (n must be not less than 1).
 *      dm, dn    Position of key element in convolution kernel.
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *      Analyze the convolution kernel and take advantage of special cases.
 *
 *      The unselected channels are not overwritten. If both src and dst have
 *      just one channel, cmask is ignored.
 *
 *      The mapping of source image to destination image is left/top corner to
 *      left/top corner, but with a shift of the destination image if it's
 *      smaller than the source image.
 *      The exact mapping formula is
 *           xs = xd + x_offset
 *           ys = yd + y_offset
 *      where
 *           x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *           y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 *
 * RESTRICTION
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4).
 *      m >= 1.   n >= 1.
 *      0 <= dm < m. 0 <= dn < n.
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConvolveMxN_Fp = __mlib_ImageConvolveMxN_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConvolveMxN_Fp) mlib_ImageConvolveMxN_Fp
    __attribute__((weak, alias("__mlib_ImageConvolveMxN_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConvolveMxN_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_d64 hkernel[7], vkernel[7];
	mlib_s32 i, j;
	mlib_s32 test_convolv = 0;
	mlib_status stat;

	if (kernel == NULL)
		return (MLIB_FAILURE);

	if (n < 1 || m < 1 || dm < 0 || dm > m - 1 || dn < 0 || dn > n - 1) {
		return (MLIB_FAILURE);
	}

	if ((m == n) && ((m == 3) || (m == 5) || (m == 7)) && (dm == dn) &&
	    (dm == (m >> 1))) {
		for (i = 0; i < m; i++)
			hkernel[i] = kernel[i];
		test_convolv = 1;
		j = 1;
		vkernel[0] = 1.0;
		while ((j < m) && (test_convolv != 0)) {
			i = 0;
			while ((i < m) && (hkernel[i]) == 0)
				i++;

			if (i == m)
				vkernel[j] = 1;
			else
				vkernel[j] = kernel[m * j + i] / hkernel[i];
			i = 0;
			while ((i < m) && (test_convolv != 0)) {

				if (hkernel[i] == 0) {
					if (kernel[m * j + i] != 0)
						test_convolv = 0;
				} else {

					if ((kernel[m * j + i] / hkernel[i]) !=
					    vkernel[j])
						test_convolv = 0;
				}

				i++;
			}

			j++;
		}
	}

	if (test_convolv == 0) {
		if ((m == n) && (dm == dn) && (dm == ((m - 1) >> 1))) {
			if (m == 2) {
				stat =
				    __mlib_ImageConv2x2_Fp(dst, src, kernel,
				    cmask, edge);
			} else if (m == 3) {
				stat =
				    __mlib_ImageConv3x3_Fp(dst, src, kernel,
				    cmask, edge);
			} else if (m == 4) {
				stat =
				    __mlib_ImageConv4x4_Fp(dst, src, kernel,
				    cmask, edge);
			} else if (m == 5) {
				stat =
				    __mlib_ImageConv5x5_Fp(dst, src, kernel,
				    cmask, edge);
			} else if (m == 7) {
				stat =
				    __mlib_ImageConv7x7_Fp(dst, src, kernel,
				    cmask, edge);
			} else {
				stat =
				    __mlib_ImageConvMxN_Fp(dst, src, kernel, m,
				    m, dm, dm, cmask, edge);
			}
		} else {
			stat =
			    __mlib_ImageConvMxN_Fp(dst, src, kernel, m, n, dm,
			    dn, cmask, edge);
		}
	} else {

		if (m == 3) {
			stat =
			    __mlib_ImageSConv3x3_Fp(dst, src, hkernel, vkernel,
			    cmask, edge);
		} else if (m == 5) {
			stat =
			    __mlib_ImageSConv5x5_Fp(dst, src, hkernel, vkernel,
			    cmask, edge);
		} else {
			stat =
			    __mlib_ImageSConv7x7_Fp(dst, src, hkernel, vkernel,
			    cmask, edge);
		}
	}

	return (stat);
}

/* *********************************************************** */
