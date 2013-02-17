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

#pragma ident	"@(#)mlib_ImageConvolveMxN.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConvolveMxN - image convolution with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConvolveMxN(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *kernel,
 *                                        mlib_s32         m,
 *                                        mlib_s32         n,
 *                                        mlib_s32         dm,
 *                                        mlib_s32         dn,
 *                                        mlib_s32         cmask,
 *                                        mlib_edge        edge)
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
 *      Analyze the convolution kernel first, convert the floating-point
 *      kernel to integer kernel, and take advantage of special cases.
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
 *      of channels (1, 2, 3, or 4). They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      m >= 1.   n >= 1.
 *      0 <= dm < m. 0 <= dn < n.
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageVisVersion.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConvolveMxN = __mlib_ImageConvolveMxN

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConvolveMxN) mlib_ImageConvolveMxN
    __attribute__((weak, alias("__mlib_ImageConvolveMxN")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConvolveMxN(
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
	mlib_type type;
	mlib_d64 fvkernel[7];
	mlib_s32 ihkernel[7], ivkernel[7];
	mlib_s32 iskernel[256];
	mlib_s32 *ikernel = iskernel;
	mlib_s32 iscale[1], i, j, scale, scale1;
	mlib_s32 test_convolv = 0;
	mlib_d64 sum, norm, norm2;
	mlib_d64 sum_pos, sum_neg;
	mlib_d64 sum_kernel, sum_kernel_pos, sum_kernel_neg;
	mlib_status stat;

	if (dst == NULL || src == NULL || kernel == NULL)
		return (MLIB_FAILURE);

	if (n < 1 || m < 1 || dm < 0 || dm > m - 1 || dn < 0 || dn > n - 1) {
		return (MLIB_FAILURE);
	}

	type = mlib_ImageGetType(src);

	if ((m == n) && ((m == 3) || (m == 5) || (m == 7)) && (dm == dn) &&
	    (dm == (m >> 1))) {
		norm = 1.0 / kernel[(m >> 1) * (m + 1)];
		for (i = 0; i < m; i++)
			fvkernel[i] = kernel[i * m + (m >> 1)] * norm;

		stat =
		    __mlib_ImageSConvKernelConvert(ihkernel, ivkernel, iscale,
		    kernel + (m >> 1) * m, fvkernel, m, n, type);

		if (stat == MLIB_SUCCESS) {
			if (mlib_ImageVisVersion() && ((type == MLIB_BYTE) ||
			    (type == MLIB_SHORT))) {
				scale1 = 16;
			} else {
				scale1 = 0;
			}

			scale = iscale[0] - scale1;
			norm = 1.0;
			while (scale > 30) {
				norm /= (1 << 30);
				scale -= 30;
			}

			norm /= (1 << scale);
			norm2 = norm * norm;

			sum_pos = 0;
			sum_neg = 0;
			sum_kernel_pos = 0;
			sum_kernel_neg = 0;
			for (i = 0; i < m; i++) {
				for (j = 0; j < m; j++) {
					sum_kernel =
					    (ihkernel[i] >> scale1) +
					    (ivkernel[j] >> scale1);
					sum =
					    norm2 * (ihkernel[i] >> scale1) *
					    (ivkernel[j] >> scale1) - kernel[i +
					    m * j];

					if (sum_kernel > 0)
						sum_kernel_pos += sum_kernel;
					else
						sum_kernel_neg -= sum_kernel;

					if (sum > 0)
						sum_pos += sum;
					else
						sum_neg -= sum;
				}
			}

			if (type == MLIB_BYTE || type == MLIB_USHORT) {
				sum = (sum_pos > sum_neg) ? sum_pos : sum_neg;
				sum_kernel =
				    (sum_kernel_pos >
				    sum_kernel_neg) ? sum_kernel_pos :
				    sum_kernel_neg;
			} else {
				sum = sum_pos + sum_neg;
				sum_kernel = sum_kernel_pos + sum_kernel_neg;
			}

			norm2 *= (mlib_d64)sum_kernel;

			switch (type) {
			case MLIB_BYTE:
				norm = 1.0 / (1u << 8);
				break;
			case MLIB_SHORT:
				norm = 1.0 / (1u << 15);
				break;
			case MLIB_USHORT:
				norm = 1.0 / (1u << 16);
				break;
			case MLIB_INT:
				norm = 1.0 / (1u << 31);
				break;
			default:
				return (MLIB_FAILURE);
			}

			if ((sum <= norm) || (sum < norm2))
				test_convolv = 1;
		}
	}

/* separable convolution */
	if (test_convolv == 1) {
		if (m == 3) {
			stat =
			    __mlib_ImageSConv3x3(dst, src, ihkernel, ivkernel,
			    iscale[0], cmask, edge);
		} else if (m == 5) {
			stat =
			    __mlib_ImageSConv5x5(dst, src, ihkernel, ivkernel,
			    iscale[0], cmask, edge);
		} else {
			stat =
			    __mlib_ImageSConv7x7(dst, src, ihkernel, ivkernel,
			    iscale[0], cmask, edge);
		}
	} else {
/* general convolution */

		if (m * n > 256) {
			ikernel =
			    (mlib_s32 *)__mlib_malloc(m * n *
			    sizeof (mlib_s32));

			if (ikernel == NULL)
				return (MLIB_FAILURE);
		}

		stat =
		    __mlib_ImageConvKernelConvert(ikernel, iscale, kernel, m, n,
		    type);

		if (stat == MLIB_SUCCESS) {
			if ((m == n) && (dm == dn) && (dm == ((m - 1) >> 1))) {
				if (m == 2) {
					stat =
					    __mlib_ImageConv2x2(dst, src,
					    ikernel, iscale[0], cmask, edge);
				} else if (m == 3) {
					stat =
					    __mlib_ImageConv3x3(dst, src,
					    ikernel, iscale[0], cmask, edge);
				} else if (m == 4) {
					stat =
					    __mlib_ImageConv4x4(dst, src,
					    ikernel, iscale[0], cmask, edge);
				} else if (m == 5) {
					stat =
					    __mlib_ImageConv5x5(dst, src,
					    ikernel, iscale[0], cmask, edge);
				} else if (m == 7) {
					stat =
					    __mlib_ImageConv7x7(dst, src,
					    ikernel, iscale[0], cmask, edge);
				} else {
					stat =
					    __mlib_ImageConvMxN(dst, src,
					    ikernel, m, m, dm, dm, iscale[0],
					    cmask, edge);
				}
			} else {
				stat =
				    __mlib_ImageConvMxN(dst, src, ikernel, m, n,
				    dm, dn, iscale[0], cmask, edge);
			}
		}

		if (ikernel != iskernel)
			__mlib_free(ikernel);
	}

	return (stat);
}

/* *********************************************************** */
