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

#pragma ident	"@(#)mlib_v_ImageSConv7x7_32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_sconv7x7_32ext - separable convolve of an image with a 7x7 kernel
 *                              edge = MLIB_EDGE_SRC_EXTEND
 *
 * SYNOPSIS
 *      mlib_status mlib_sconv7x7_32ext(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_s32         dx_l,
 *                                      mlib_s32         dx_r,
 *                                      mlib_s32         dy_t,
 *                                      mlib_s32         dy_b,
 *                                      const mlib_s32   *hkernel,
 *                                      const mlib_s32   *vkernel,
 *                                      mlib_s32         scale,
 *                                      mlib_s32         cmask)
 *
 *
 * ARGUMENT
 *      dst       Pointer to the destination image data structure
 *      src       Pointer to the source image data structure
 *      hkernel   Pointer to 7 elements of the horizontal kernel
 *      vkernel   Pointer to 7 elements of the vertical kernel
 *      scale     The integer coefficients are assumed to be equal
 *                to some floating point value < 1.0, multiplied by
 *                a scalefactor that is a power of 2. Values are
 *                divided by this scalefactor before being output.
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      2-D separable convolution, 7x7 kernel.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageSConv.h>

/* *********************************************************** */

#define	LEFT	3
#define	RIGHT	3

#define	TOP	3

#define	BOTTOM	3

/* *********************************************************** */

#define	SAT_S32(x)	(mlib_s32)(x)

/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS(type)                                 \
	type *sl, *adr_src;                                         \
	type *da, *dl, *adr_dst;                                    \
	mlib_s32 row, col, block;                                   \
	mlib_s32 *row0, *row1, *row2, *row3, *row4, *row5, *row6;   \
	mlib_d64 scalef;                                            \
	mlib_d64 sum1, sum2, sum3;                                  \
	mlib_d64 kh0, kh1, kh2, kh3, kh4, kh5, kh6;                 \
	mlib_d64 kv0, kv1, kv2, kv3, kv4, kv5, kv6;                 \
	mlib_d64 p0, p1, p2, p3, p4, p5, p6, p7, p8;                \
	mlib_s32 sll, dll, wid, hgt;                                \
	mlib_s32 num_blocks;                                        \
	mlib_s32 leftover_cols;                                     \
	mlib_s32 chan, testchan, nchannel

/* *********************************************************** */

#define	LOAD_KERNEL_INTO_DOUBLE()                               \
	while (scalef_expon > 30) {                             \
	    scalef /= (1 << 30);                                \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	scalef /= (1 << scalef_expon);                          \
/*                                                              \
 * keep kernel in regs                                          \
 */                                                             \
	kh0 = scalef * hkernel[0];                              \
	kh1 = scalef * hkernel[1];                              \
	kh2 = scalef * hkernel[2];                              \
	kh3 = scalef * hkernel[3];                              \
	kh4 = scalef * hkernel[4];                              \
	kh5 = scalef * hkernel[5];                              \
	kh6 = scalef * hkernel[6];                              \
	kv0 = scalef * vkernel[0];                              \
	kv1 = scalef * vkernel[1];                              \
	kv2 = scalef * vkernel[2];                              \
	kv3 = scalef * vkernel[3];                              \
	kv4 = scalef * vkernel[4];                              \
	kv5 = scalef * vkernel[5];                              \
	kv6 = scalef * vkernel[6]

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                            \
	hgt = mlib_ImageGetHeight(src);                         \
	wid = mlib_ImageGetWidth(src);                          \
	nchannel = mlib_ImageGetChannels(src);                  \
	sll = mlib_ImageGetStride(src) / sizeof (type);         \
	dll = mlib_ImageGetStride(dst) / sizeof (type);         \
	adr_src = (type *) mlib_ImageGetData(src);              \
	adr_dst = (type *) mlib_ImageGetData(dst)

/* *********************************************************** */

#define	CONVOLVE_THREE_PIXELS(dst, shift, format)                    \
	p6 = row0[col] * kv0 + row1[col] * kv1 + row2[col] * kv2 +   \
	    row3[col] * kv3 + row4[col] * kv4 + row5[col] * kv5 +    \
	    row6[col] * kv6;                                         \
	col += shift;                                                \
	p7 = row0[col] * kv0 + row1[col] * kv1 + row2[col] * kv2 +   \
	    row3[col] * kv3 + row4[col] * kv4 + row5[col] * kv5 +    \
	    row6[col] * kv6;                                         \
	col += shift;                                                \
	p8 = row0[col] * kv0 + row1[col] * kv1 + row2[col] * kv2 +   \
	    row3[col] * kv3 + row4[col] * kv4 + row5[col] * kv5 +    \
	    row6[col] * kv6;                                         \
	col += shift;                                                \
	sum1 = p0 * kh0;                                             \
	sum2 = p1 * kh0;                                             \
	sum3 = p2 * kh0;                                             \
	sum1 += p1 * kh1;                                            \
	sum2 += p2 * kh1;                                            \
	sum3 += p3 * kh1;                                            \
	sum1 += p2 * kh2;                                            \
	sum2 += p3 * kh2;                                            \
	sum3 += p4 * kh2;                                            \
	sum1 += p3 * kh3;                                            \
	sum2 += p4 * kh3;                                            \
	sum3 += p5 * kh3;                                            \
	sum1 += p4 * kh4;                                            \
	sum2 += p5 * kh4;                                            \
	sum3 += p6 * kh4;                                            \
	sum1 += p5 * kh5;                                            \
	sum2 += p6 * kh5;                                            \
	sum3 += p7 * kh5;                                            \
	sum1 += p6 * kh6;                                            \
	sum2 += p7 * kh6;                                            \
	sum3 += p8 * kh6;                                            \
	*dst = SAT_##format(sum1);                                   \
	dst += shift;                                                \
	*dst = SAT_##format(sum2);                                   \
	dst += shift;                                                \
	*dst = SAT_##format(sum3);                                   \
	dst += shift;                                                \
	p0 = p3;                                                     \
	p1 = p4;                                                     \
	p2 = p5;                                                     \
	p3 = p6;                                                     \
	p4 = p7;                                                     \
	p5 = p8

/* *********************************************************** */

#define	CONVOLVE_ONE_PIXEL(dst, shift, format)                       \
	p6 = row0[col] * kv0 + row1[col] * kv1 + row2[col] * kv2 +   \
	    row3[col] * kv3 + row4[col] * kv4 + row5[col] * kv5 +    \
	    row6[col] * kv6;                                         \
	col += shift;                                                \
	sum1 = p0 * kh0;                                             \
	sum1 += p1 * kh1;                                            \
	sum1 += p2 * kh2;                                            \
	sum1 += p3 * kh3;                                            \
	sum1 += p4 * kh4;                                            \
	sum1 += p5 * kh5;                                            \
	sum1 += p6 * kh6;                                            \
	*dst = SAT_##format(sum1);                                   \
	dst += shift;                                                \
	p0 = p1;                                                     \
	p1 = p2;                                                     \
	p2 = p3;                                                     \
	p3 = p4;                                                     \
	p4 = p5;                                                     \
	p5 = p6

/* *********************************************************** */

mlib_status
mlib_v_sconv7x7_32ext(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	DEFINE_IDENTIFICATORS(mlib_s32);
	mlib_s32 i, dend;

	scalef = 1.0;

	LOAD_KERNEL_INTO_DOUBLE();

	GET_SRC_DST_PARAMETERS(mlib_s32);

	dend = (dx_r > wid) ? wid : dx_r;
	num_blocks = (wid - dend) / 3;
	leftover_cols = (wid - dend) - (num_blocks * 3);

	testchan = 1;
	for (chan = nchannel - 1; chan >= 0; chan--) {
		if ((cmask & testchan) == 0) {
			testchan <<= 1;
			continue;
		}

		testchan <<= 1;
		sl = adr_src + chan;
		dl = da = adr_dst + chan;

/*
 * set up rowbufs 1 - 6;
 * rowbuf 0 is always reloaded inside the loop:
 */

		row = 0;
		row1 = sl;
		row++;

		if ((row > dy_t) && (row < hgt + TOP + BOTTOM - dy_b))
			sl += sll;
		row2 = sl;
		row++;

		if ((row > dy_t) && (row < hgt + TOP + BOTTOM - dy_b))
			sl += sll;
		row3 = sl;
		row++;

		if ((row > dy_t) && (row < hgt + TOP + BOTTOM - dy_b))
			sl += sll;
		row4 = sl;
		row++;

		if ((row > dy_t) && (row < hgt + TOP + BOTTOM - dy_b))
			sl += sll;
		row5 = sl;
		row++;

		if ((row > dy_t) && (row < hgt + TOP + BOTTOM - dy_b))
			sl += sll;
		row6 = sl;
		row++;

		if ((row > dy_t) && (row < hgt + TOP + BOTTOM - dy_b))
			sl += sll;

		for (row = 0; row < hgt; row++) {

			row0 = row1;
			row1 = row2;
			row2 = row3;
			row3 = row4;
			row4 = row5;
			row5 = row6;
			row6 = sl;

			col = 0;
			i = 0;

			p0 = row0[col] * kv0 + row1[col] * kv1 +
			    row2[col] * kv2 + row3[col] * kv3 +
			    row4[col] * kv4 + row5[col] * kv5 + row6[col] * kv6;
			i++;

			if ((i > dx_l) && (i < wid + LEFT + RIGHT - dx_r))
				col += nchannel;
			p1 = row0[col] * kv0 + row1[col] * kv1 +
			    row2[col] * kv2 + row3[col] * kv3 +
			    row4[col] * kv4 + row5[col] * kv5 + row6[col] * kv6;
			i++;

			if ((i > dx_l) && (i < wid + LEFT + RIGHT - dx_r))
				col += nchannel;
			p2 = row0[col] * kv0 + row1[col] * kv1 +
			    row2[col] * kv2 + row3[col] * kv3 +
			    row4[col] * kv4 + row5[col] * kv5 + row6[col] * kv6;
			i++;

			if ((i > dx_l) && (i < wid + LEFT + RIGHT - dx_r))
				col += nchannel;
			p3 = row0[col] * kv0 + row1[col] * kv1 +
			    row2[col] * kv2 + row3[col] * kv3 +
			    row4[col] * kv4 + row5[col] * kv5 + row6[col] * kv6;
			i++;

			if ((i > dx_l) && (i < wid + LEFT + RIGHT - dx_r))
				col += nchannel;
			p4 = row0[col] * kv0 + row1[col] * kv1 +
			    row2[col] * kv2 + row3[col] * kv3 +
			    row4[col] * kv4 + row5[col] * kv5 + row6[col] * kv6;
			i++;

			if ((i > dx_l) && (i < wid + LEFT + RIGHT - dx_r))
				col += nchannel;
			p5 = row0[col] * kv0 + row1[col] * kv1 +
			    row2[col] * kv2 + row3[col] * kv3 +
			    row4[col] * kv4 + row5[col] * kv5 + row6[col] * kv6;
			col += nchannel;

/* Process along row, getting 3 output points per loop iteration: */

#pragma pipeloop(0)
			for (block = 0; block < num_blocks; block++) {
				CONVOLVE_THREE_PIXELS(da, nchannel, S32);
			}

			if (leftover_cols) {
				CONVOLVE_ONE_PIXEL(da, nchannel, S32);

				if (leftover_cols > 1) {
					CONVOLVE_ONE_PIXEL(da, nchannel, S32);
				}
			}

			for (i = 0; i < dend; i++) {
				col -= nchannel;
				CONVOLVE_ONE_PIXEL(da, nchannel, S32);
			}

			if (row < hgt - dy_b - 1)
/* set src ptrs to next row */
				sl = sl + sll;
/* set dst ptrs to next row */
			dl = da = dl + dll;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
