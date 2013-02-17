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

#pragma ident	"@(#)mlib_v_ImageClear_f.c	9.2	07/11/05 SMI"

/*
 *
 * DESCRIPTION
 *      Clear of an image to a specific color.
 *      -- VIS version low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageClear.c
 *      for structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageClear_f.h>

#define	SIZE_IN_BYTES	8
#define	FORMAT	mlib_u64
#define	SFORMAT	mlib_s64
#define	UFORMAT	mlib_u64
#define	SIZE_IN_BITS	8*(SIZE_IN_BYTES)
#define	MASK	(SIZE_IN_BYTES) - 1

/* *********************************************************** */

#define	PREPARE_CONST()                                                  \
	if (nchannels == 1) {                                            \
	    c01 = (((SFORMAT) (c[0] & 1)) << 63) >> 63;                  \
	} else if (nchannels == 2) {                                     \
	    width = width * 2;                                           \
	    c01 = ((c[0] & 1) << 1) | (c[1] & 1);                        \
	    c01 |= (c01 << 2);                                           \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	    c01 |= (c01 << 32);                                          \
	} else if (nchannels == 3) {                                     \
	    width = width * 3;                                           \
	    c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);    \
	    c01 |= (c01 << 3);                                           \
	    c01 = (c01 << 6) | c01;                                      \
	    c02 = (c01 << 12) | c01;                                     \
	    c02 |= (c02 << 24);                                          \
	    c01 = (c02 << 12) | c01;                                     \
	    c02 = (c01 << 5) | (c01 >> 55);                              \
	    c03 = (c01 << 6) | (c01 >> 54);                              \
	    c01 = (c01 << 4) | (c01 >> 56);                              \
	} else {                                                         \
	    width = width * 4;                                           \
	    c01 =                                                        \
		((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) <<   \
		1) | (c[3] & 1);                                         \
	    c01 |= (c01 << 4);                                           \
	    c01 |= (c01 << 8);                                           \
	    c01 |= (c01 << 16);                                          \
	    c01 |= (c01 << 32);                                          \
	}

/* *********************************************************** */

#define	EDGE_BIT_FIRST(mask, p1, off_p1, p2, off_p2)                    \
	{                                                               \
	    mask = ~((FORMAT) 0);                                       \
	    mask >>= (((mlib_addr)p1 & (MASK)) * 8 + off_p1);           \
	    edge_mask = vis_edge8(p1, p2);                              \
	    if ((((mlib_addr)p1 ^ (mlib_addr)p2) & (~(MASK))) == 0) {   \
		FORMAT mask1 = ~((FORMAT) 0);                           \
	                                                                \
		mask1 <<= (SIZE_IN_BITS - 1 -                           \
		    (((mlib_addr)p2 & (MASK)) * 8 + off_p2));           \
		mask &= mask1;                                          \
	    }                                                           \
	}

/* *********************************************************** */

#define	EDGE_BIT_LAST(mask, p1, p2, off_p2)                     \
	{                                                       \
	    edge_mask = vis_edge8(p1, p2);                      \
	    mask =                                              \
		(~((FORMAT) 0)) << (SIZE_IN_BITS - 1 -          \
		(((mlib_addr)p2 & (MASK)) * 8 + off_p2));       \
	}

/* *********************************************************** */

#define	STORE_MASK(mask, p, edge_mask, a)                       \
	mask = (((*p) & (~mask)) | (a & mask));                 \
	vis_pst_8(*((mlib_d64 *)&mask), p, edge_mask);

/* *********************************************************** */

mlib_status
mlib_v_ImageClear_BIT(
    mlib_image *img,
    const mlib_s32 *c)
{
/* pointers for pixel and line of img */
	mlib_u8 *dp, *dl, *dend;
	FORMAT *dpp;
	UFORMAT c01, c02, c03;
	UFORMAT c1, c2, c3, emask;
	mlib_s32 offdst, width, height, nchannels;
	mlib_s32 amount, dend_offset;
	mlib_s32 strided, bitoffsetd;
	mlib_s32 shiftc, shiftc_, edge_mask;
	mlib_s32 i, j;

	dp = (mlib_u8 *)mlib_ImageGetData(img);
	height = mlib_ImageGetHeight(img);
	width = mlib_ImageGetWidth(img);
	strided = mlib_ImageGetStride(img);
	nchannels = mlib_ImageGetChannels(img);
	bitoffsetd = mlib_ImageGetBitOffset(img);

	PREPARE_CONST();

	if (width > strided * 8)
		return (MLIB_FAILURE);

	if (width == strided * 8) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	dend_offset = (bitoffsetd + amount - 1) & 7;
	dl = dp;

	if (nchannels == 3) {

		for (j = 0; j < height; j++) {

			dend = dp + ((bitoffsetd + amount - 1) >> 3);
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			offdst = offdst * 8 + bitoffsetd;

			i = -offdst;
			c1 = c01;
			c2 = c02;
			c3 = c03;

			if (i != 0) {
				shiftc = offdst;
				shiftc_ = (SIZE_IN_BITS) - offdst;
				c1 = (c01 >> shiftc) | (c03 << shiftc_);
				c2 = (c02 >> shiftc) | (c01 << shiftc_);
				c3 = (c03 >> shiftc) | (c02 << shiftc_);
				EDGE_BIT_FIRST(emask, dp, bitoffsetd, dend,
				    dend_offset);
				STORE_MASK(emask, dpp, edge_mask, c1);
				c2 ^= c1 ^= c2 ^= c1;
				c2 ^= c3 ^= c2 ^= c3;
				i += (SIZE_IN_BITS);
				dpp++;
			}
#pragma pipeloop(0)
			for (; i <= amount - 192; i += 192) {
				dpp[0] = c1;
				dpp[1] = c2;
				dpp[2] = c3;
				dpp += 3;
			}

			if (i < amount) {

				if (i <= (amount - SIZE_IN_BITS)) {
					dpp[0] = c1;
					i += (SIZE_IN_BITS);
					dpp++;

					if (i <= (amount - SIZE_IN_BITS)) {
						dpp[0] = c2;
						i += (SIZE_IN_BITS);
						dpp++;
						c1 = c3;
					} else
						c1 = c2;
				}
				if (i < amount) {
					EDGE_BIT_LAST(emask, dpp, dend,
					    dend_offset);
					STORE_MASK(emask, dpp, edge_mask, c1);
				}
			}

			dp = dl += strided;
		}
	} else {

		c01 =
		    (c01 >> bitoffsetd) | (c01 << (sizeof (UFORMAT) * 8 -
		    bitoffsetd));

		for (j = 0; j < height; j++) {

			dend = dp + ((bitoffsetd + amount - 1) >> 3);
			offdst = ((mlib_addr)dp) & (MASK);
			dpp = (FORMAT *) (dp - offdst);
			offdst = offdst * 8 + bitoffsetd;

			i = -offdst;

			if (i != 0) {
				EDGE_BIT_FIRST(emask, dp, bitoffsetd, dend,
				    dend_offset);
				STORE_MASK(emask, dpp, edge_mask, c01);
				i += (SIZE_IN_BITS);
				dpp++;
			}
#pragma pipeloop(0)
			for (; i <= (amount - 64); i += 64) {
				(*dpp++) = c01;
			}

			if (i < amount) {
				EDGE_BIT_LAST(emask, dpp, dend, dend_offset);
				STORE_MASK(emask, dpp, edge_mask, c01);
			}

			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PREPAREVARS(type, chan)                                           \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		l,                                                        \
		j;                                                        \
	    mlib_s32                                                      \
		emask;                                                    \
	    mlib_d64                                                      \
	    dcolor, *                                                     \
		dpimg;                                                    \
	                                                                  \
	if ((img_width * chan) == img_stride)                             \
	{                                                                 \
	img_width *= img_height;                                          \
	img_height = 1;                                                   \
	}

/* *********************************************************** */

#define	STRIP(pd, color, w, h, chan, data_type)                    \
	for (l = 0; l < chan; l++) {                               \
	    data_type color_i = color[l];                          \
	                                                           \
	    for (i = 0; i < h; i++) {                              \
		for (j = 0; j < w; j++)                            \
		    pd[i * img_stride + l + j * chan] = color_i;   \
	    }                                                      \
	}

/* *********************************************************** */

void
mlib_v_ImageClear_U8_1(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF;

	PREPAREVARS(mlib_u8,
	    1);

	if (img_width < 16) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_u8);

		return;
	}

	color0 |= (color0 << 8);
	color0 |= (color0 << 16);
	dcolor = vis_to_double_dup(color0);
	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width - 1;

		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_U8_2(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF, color1 = color[1] & 0xFF;
	mlib_d64 dcolor0;

	PREPAREVARS(mlib_u8,
	    2);

	if (img_width < 8) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_u8);

		return;
	}

	color0 = (color0 << 8) | color1;
	color0 |= (color0 << 16);
	dcolor0 = vis_to_double_dup(color0);
	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 2 - 1;

		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge8(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 2 - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_U8_3(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF, color1 = color[1] & 0xFF, color2 =
	    color[2] & 0xFF, col;
	mlib_d64 dcolor1, dcolor2, dcolor00, dcolor11, dcolor22;

	PREPAREVARS(mlib_u8,
	    3);

	if (img_width < 16) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_u8);

		return;
	}

	col = (color0 << 16) | (color1 << 8) | color2;
	color0 = (col << 8) | color0;
	color1 = (color0 << 8) | color1;
	color2 = (color1 << 8) | color2;
	dcolor = vis_to_double(color0, color1);
	dcolor1 = vis_to_double(color2, color0);
	dcolor2 = vis_to_double(color1, color2);
	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 3 - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge8(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_8(dcolor22, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 3 - 24); j += 24) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 8)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 16)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor00, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_U8_4(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF, color1 = color[1] & 0xFF, color2 =
	    color[2] & 0xFF, color3 = color[3] & 0xFF;
	mlib_d64 dcolor0;

	PREPAREVARS(mlib_u8,
	    4);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_u8);

		return;
	}

	color0 = (color0 << 24) | (color1 << 16) | (color2 << 8) | color3;
	dcolor0 = vis_to_double_dup(color0);
	for (i = 0; i < img_height; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 4 - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		emask = vis_edge8(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 4 - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S16_1(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF;

	PREPAREVARS(mlib_s16,
	    1);

	if (img_width < 8) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_s16);

		return;
	}

	color0 |= (color0 << 16);
	dcolor = vis_to_double_dup(color0);
	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width - 1;

		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge16(pimg_row, pimg_row_end);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S16_2(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF, color1 = color[1] & 0xFFFF;
	mlib_d64 dcolor0;

	PREPAREVARS(mlib_s16,
	    2);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_s16);

		return;
	}

	color0 = (color0 << 16) | color1;
	dcolor0 = vis_to_double_dup(color0);
	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 2 - 1;

		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge16(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 2 - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S16_3(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF, color1 =
	    color[1] & 0xFFFF, color2 = color[2] & 0xFFFF, col0, col1, col2;
	mlib_d64 dcolor1, dcolor2, dcolor00, dcolor11, dcolor22;

	PREPAREVARS(mlib_s16,
	    3);

	if (img_width < 8) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_s16);

		return;
	}

	col0 = (color0 << 16) | color1;
	col1 = (color2 << 16) | color0;
	col2 = (color1 << 16) | color2;
	dcolor = vis_to_double(col0, col1);
	dcolor1 = vis_to_double(col2, col0);
	dcolor2 = vis_to_double(col1, col2);
	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 3 - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge16(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_16(dcolor22, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 3 - 12); j += 12) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 4)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 8)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor00, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S16_4(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF, color1 =
	    color[1] & 0xFFFF, color2 = color[2] & 0xFFFF, color3 =
	    color[3] & 0xFFFF;
	mlib_d64 dcolor0;

	PREPAREVARS(mlib_s16,
	    4);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_s16);

		return;
	}

	color0 = (color0 << 16) | color1;
	color1 = (color2 << 16) | color3;
	dcolor0 = vis_to_double(color0, color1);
	for (i = 0; i < img_height; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 4 - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		emask = vis_edge16(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 4 - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S32_1(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0];

	PREPAREVARS(mlib_s32,
	    1);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_s32);

		return;
	}

	dcolor = vis_to_double_dup(color0);
	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width - 1;

		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge32(pimg_row, pimg_row_end);
		vis_pst_32(dcolor, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j <= (img_width - 2); j += 2)
			(*dpimg++) = dcolor;

		if (j < img_width) {
			emask = vis_edge32(dpimg, pimg_row_end);
			vis_pst_32(dcolor, dpimg, emask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S32_2(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1];
	mlib_d64 dcolor0;

	PREPAREVARS(mlib_s32,
	    2);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_s32);

		return;
	}

	dcolor0 = vis_to_double(color0, color1);
	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 2 - 1;

		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge32(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_32(dcolor, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 2 - 2); j += 2)
			(*dpimg++) = dcolor;
		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S32_3(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1], color2 = color[2];
	mlib_d64 dcolor1, dcolor2, dcolor00, dcolor11, dcolor22;

	PREPAREVARS(mlib_s32,
	    3);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_s32);

		return;
	}

	dcolor = vis_to_double(color0, color1);
	dcolor1 = vis_to_double(color2, color0);
	dcolor2 = vis_to_double(color1, color2);
	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 3 - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge32(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_32(dcolor22, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j < (img_width * 3 - 6); j += 6) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 2)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 4)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor00, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClear_S32_4(
    mlib_image *img,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1], color2 =
	    color[2], color3 = color[3];
	mlib_d64 dcolor0, dcolor00, dcolor0_, dcolor00_, dcolor1;

	PREPAREVARS(mlib_s32,
	    4);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_s32);

		return;
	}

	dcolor0 = vis_to_double(color2, color3);
	dcolor00 = vis_to_double(color0, color1);
	vis_alignaddr((void *)0, 4);
	dcolor0_ = vis_faligndata(dcolor0, dcolor00);
	dcolor00_ = vis_faligndata(dcolor00, dcolor0);
	for (i = 0; i < img_height; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride, *pimg_row_end =
		    pimg_row + img_width * 4 - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 4);
		emask = vis_edge32(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0_, dcolor00_);
		dcolor1 = vis_faligndata(dcolor00_, dcolor0_);
		vis_pst_32(dcolor, dpimg++, emask);
		(*dpimg++) = dcolor1;
		j = (mlib_s32 *)dpimg - pimg_row;

#pragma pipeloop(0)
		for (; j <= (img_width * 4 - 4); j += 4) {
			dpimg[0] = dcolor;
			dpimg[1] = dcolor1;
			dpimg += 2;
		}

		if (j < (img_width * 4)) {
			emask = vis_edge32(dpimg, pimg_row_end);
			vis_pst_32(dcolor, dpimg, emask);
		}
	}
}

/* *********************************************************** */
