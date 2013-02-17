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

#pragma ident	"@(#)mlib_v_ImageHistogram2_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      internal function for mlib_ImageHistogram2
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageHistogram2.h>

/* *********************************************************** */

static void mlib_v_ImageHistogram2_S16_2_D1(
    const void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1);

static void mlib_v_ImageHistogram2_S16_4_D1(
    const void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

static void mlib_v_ImageHistogram2_S16_43_D1(
    const void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2);

/* *********************************************************** */

/* buffer size in shorts; this value have to <= 1024 and divided by 12 */
#define	SIZE	1020

/* *********************************************************** */

#define	MLIB_SCALE_INDEX(dd, sd, d_scl, d_min)                        \
	td = vis_fpsub16(sd, d_min);                                  \
	dh0 = vis_fmuld8sux16(vis_read_hi(td), vis_read_hi(d_scl));   \
	dh1 = vis_fmuld8ulx16(vis_read_hi(td), vis_read_hi(d_scl));   \
	dl0 = vis_fmuld8sux16(vis_read_lo(td), vis_read_lo(d_scl));   \
	dl1 = vis_fmuld8ulx16(vis_read_lo(td), vis_read_lo(d_scl));   \
	dh = vis_fpadd32(dh0, dh1);                                   \
	dl = vis_fpadd32(dl0, dl1);                                   \
	dd = vis_fpackfix_pair(dh, dl)

/* *********************************************************** */

mlib_status
mlib_v_ImageHistogram2_U16_124(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl)
{
	mlib_d64 bufd[(SIZE / 4) + 4];
	mlib_s32 i, j, off, size;
	static const mlib_u32 tbl_out[16] = {
		0x00000000, 0x00000001, 0x00000100, 0x00000101,
		0x00010000, 0x00010001, 0x00010100, 0x00010101,
		0x01000000, 0x01000001, 0x01000100, 0x01000101,
		0x01010000, 0x01010001, 0x01010100, 0x01010101
	};
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

	for (j = yStart; j < height; j += yPeriod) {
		mlib_u16 *sl = (mlib_u16 *)src + j * stride + xStart * nchan;
		mlib_d64 d_off, d_min, d_max, d_scl;
		mlib_s32 k = 0, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_u32 ind =
			    ((sl[0] -
			    ((mlib_u16 *)arr_min)[k]) *
			    ((mlib_u16 *)arr_scl)[k]) >> 16;

			if (sl[0] >= ((mlib_u16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_u16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}

			sl++;
			k++;
		}

		n -= k;

		d_off = arr_min[k];
		d_min = vis_fxor(arr_min[k], mask8000);
		d_max = vis_fxor(arr_max[k], mask8000);
		d_scl = arr_scl[k];

/* loop on buffer size */
		for (off = 0; off < n; off += size) {
			mlib_d64 *sp = (mlib_d64 *)(sl + off);
			mlib_d64 dzero = vis_fzero();
			mlib_u32 low_mask, hgh_mask, out_mask = 0, num_out = 0;

			size = (n - off);

			if (size > SIZE)
				size = SIZE;

/* find histogram indexes */
/* #pragma pipeloop(0) */
			for (i = 0; i < (size + 3) / 4; i++) {
				mlib_d64 sd, sdx, td, dd, dh, dl, dh0, dh1, dl0,
				    dl1;

				sd = sp[i];

				vis_pst_16(dzero, bufd + i, out_mask);
				num_out += tbl_out[out_mask];

				sdx = vis_fxor(sd, mask8000);
				low_mask = vis_fcmplt16(sdx, d_min);
				hgh_mask = vis_fcmpgt16(sdx, d_max);
				out_mask = low_mask | hgh_mask;

				MLIB_SCALE_INDEX(dd, sd, d_scl, d_off);

				bufd[i + 1] = dd;
			}

			if (size & 3)
				out_mask &= (0xF0 >> (size & 3));
			vis_pst_16(dzero, bufd + i, out_mask);
			num_out += tbl_out[out_mask];

/* correct histo[k][0] */
			histogr[k][0] -= (num_out >> 24);
			histogr[k + 1][0] -= (num_out >> 16) & 0xFF;
			histogr[k + 2][0] -= (num_out >> 8) & 0xFF;
			histogr[k + 3][0] -= (num_out & 0xFF);

			if (nchan == 1) {
				mlib_s32 *histo0 = histogr[0];
				mlib_u32 *sp = (mlib_u32 *)(bufd + 1);
				mlib_u32 src12, src1, src2, elem1;
				mlib_s32 i;

				src12 = (*sp++);
				src1 = src12 & 0xFFFF;
				src2 = src12 >> 16;
/* #pragma pipeloop can't be used here */
				for (i = 0; i < size / 2; i++) {
					elem1 = histo0[src1];
					src12 = sp[i];
					histo0[src1] = elem1 + 1;
					elem1 = histo0[src2];
					src1 = src12 & 0xFFFF;
					histo0[src2] = elem1 + 1;
					src2 = src12 >> 16;
				}

				if (size & 1)
					histo0[src2]++;
			} else if (nchan == 2) {
				mlib_v_ImageHistogram2_S16_2_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1]);
			} else if (nchan == 4) {
				mlib_v_ImageHistogram2_S16_4_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1], histogr[k + 2],
				    histogr[k + 3]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageHistogram2_U16_3(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl)
{
	mlib_d64 bufd[(SIZE / 4) + 4];
	mlib_s32 i, j, off, size;
	mlib_u32 tbl_out[16] = {
		0x00000000, 0x00000001, 0x00000100, 0x00000101, 0x00010000,
		    0x00010001,
		0x00010100, 0x00010101, 0x01000000, 0x01000001, 0x01000100,
		    0x01000101,
		0x01010000, 0x01010001, 0x01010100, 0x01010101
	};
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

	for (j = yStart; j < height; j += yPeriod) {
		mlib_u16 *sl = (mlib_u16 *)src + j * stride + xStart * nchan;
		mlib_s32 k = 0, l, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_u32 ind =
			    ((sl[0] -
			    ((mlib_u16 *)arr_min)[k]) *
			    ((mlib_u16 *)arr_scl)[k]) >> 16;

			if (sl[0] >= ((mlib_u16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_u16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}

			sl++;
			k++;
		}

		n -= k;

/* 'l' is mlib_d64 offset from sl */
		for (l = 0; l < 3; l++) {
			mlib_d64 d_off, d_min, d_max, d_scl;

			d_off = arr_min[k];
			d_min = vis_fxor(arr_min[k], mask8000);
			d_max = vis_fxor(arr_max[k], mask8000);
			d_scl = arr_scl[k];

/* loop on buffer size */
			for (off = 0; off < n; off += size) {
				mlib_d64 *sp = (mlib_d64 *)(sl + 4 * l + off);
				mlib_d64 dzero = vis_fzero();
				mlib_u32 low_mask, hgh_mask, out_mask =
				    0, num_out = 0;
				mlib_s32 num, pnum;

				size = (n - off);

				if (size > SIZE)
					size = SIZE;

/* find histogram indexes */
/* #pragma pipeloop(0) */
				for (i = 0; i < (size + 11) / 12; i++) {
					mlib_d64 sd, sdx, td, dd, dh, dl, dh0,
					    dh1, dl0, dl1;

					sd = sp[3 * i];

					vis_pst_16(dzero, bufd + i, out_mask);
					num_out += tbl_out[out_mask];

					sdx = vis_fxor(sd, mask8000);
					low_mask = vis_fcmplt16(sdx, d_min);
					hgh_mask = vis_fcmpgt16(sdx, d_max);
					out_mask = low_mask | hgh_mask;

					MLIB_SCALE_INDEX(dd, sd, d_scl, d_off);

					bufd[i + 1] = dd;
				}

				num = size / 12;
				pnum = size - 12 * num;

				if (pnum > 4)
					pnum = 4;

				if (pnum)
					out_mask &= (0xF0 >> pnum);
				vis_pst_16(dzero, bufd + i, out_mask);
				num_out += tbl_out[out_mask];

/* correct histo[k][0] */
				histogr[k][0] -= (num_out >> 24);
				histogr[k + 1][0] -= (num_out >> 16) & 0xFF;
				histogr[k + 2][0] -= (num_out >> 8) & 0xFF;
				histogr[k + 3][0] -= (num_out & 0xFF);

				mlib_v_ImageHistogram2_S16_43_D1(bufd + 1,
				    4 * num + pnum, histogr[k], histogr[k + 1],
				    histogr[k + 2]);
			}

/* correct 'k' and 'n' for next offset */
			k++;

			if (k >= 3)
				k -= 3;
			n -= 4;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageHistogram2_S16_124(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl)
{
	mlib_d64 bufd[(SIZE / 4) + 4];
	mlib_s32 i, j, off, size;
	mlib_u32 tbl_out[16] = {
		0x00000000, 0x00000001, 0x00000100, 0x00000101, 0x00010000,
		    0x00010001,
		0x00010100, 0x00010101, 0x01000000, 0x01000001, 0x01000100,
		    0x01000101,
		0x01010000, 0x01010001, 0x01010100, 0x01010101
	};

	for (j = yStart; j < height; j += yPeriod) {
		mlib_s16 *sl = (mlib_s16 *)src + j * stride + xStart * nchan;
		mlib_d64 d_min, d_max, d_scl;
		mlib_s32 k = 0, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_s32 ind =
			    ((sl[0] -
			    ((mlib_s16 *)arr_min)[k]) *
			    ((mlib_s16 *)arr_scl)[k]) >> 16;

			if (sl[0] >= ((mlib_s16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_s16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}

			sl++;
			k++;
		}

		n -= k;

		d_min = arr_min[k];
		d_max = arr_max[k];
		d_scl = arr_scl[k];

/* loop on buffer size */
		for (off = 0; off < n; off += size) {
			mlib_d64 *sp = (mlib_d64 *)(sl + off);
			mlib_d64 dzero = vis_fzero();
			mlib_u32 low_mask, hgh_mask, out_mask = 0, num_out = 0;

			size = (n - off);

			if (size > SIZE)
				size = SIZE;

/* find histogram indexes */
/* #pragma pipeloop(0) */
			for (i = 0; i < (size + 3) / 4; i++) {
				mlib_d64 sd, td, dd, dh, dl, dh0, dh1, dl0, dl1;

				sd = sp[i];

				vis_pst_16(dzero, bufd + i, out_mask);
				num_out += tbl_out[out_mask];

				low_mask = vis_fcmplt16(sd, d_min);
				hgh_mask = vis_fcmpgt16(sd, d_max);
				out_mask = low_mask | hgh_mask;

				MLIB_SCALE_INDEX(dd, sd, d_scl, d_min);

				bufd[i + 1] = dd;
			}

			if (size & 3)
				out_mask &= (0xF0 >> (size & 3));
			vis_pst_16(dzero, bufd + i, out_mask);
			num_out += tbl_out[out_mask];

/* correct histo[k][0] */
			histogr[k][0] -= (num_out >> 24);
			histogr[k + 1][0] -= (num_out >> 16) & 0xFF;
			histogr[k + 2][0] -= (num_out >> 8) & 0xFF;
			histogr[k + 3][0] -= (num_out & 0xFF);

			if (nchan == 1) {
				mlib_s32 *histo0 = histogr[0];
				mlib_u32 *sp = (mlib_u32 *)(bufd + 1);
				mlib_u32 src12, src1, src2, elem1;
				mlib_s32 i;

				src12 = (*sp++);
				src1 = src12 & 0xFFFF;
				src2 = src12 >> 16;
/* #pragma pipeloop can't be used here */
				for (i = 0; i < size / 2; i++) {
					elem1 = histo0[src1];
					src12 = sp[i];
					histo0[src1] = elem1 + 1;
					elem1 = histo0[src2];
					src1 = src12 & 0xFFFF;
					histo0[src2] = elem1 + 1;
					src2 = src12 >> 16;
				}

				if (size & 1)
					histo0[src2]++;
			} else if (nchan == 2) {
				mlib_v_ImageHistogram2_S16_2_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1]);
			} else if (nchan == 4) {
				mlib_v_ImageHistogram2_S16_4_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1], histogr[k + 2],
				    histogr[k + 3]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageHistogram2_S16_3(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl)
{
	mlib_d64 bufd[(SIZE / 4) + 4];
	mlib_s32 i, j, off, size;
	mlib_u32 tbl_out[16] = {
		0x00000000, 0x00000001, 0x00000100, 0x00000101, 0x00010000,
		    0x00010001,
		0x00010100, 0x00010101, 0x01000000, 0x01000001, 0x01000100,
		    0x01000101,
		0x01010000, 0x01010001, 0x01010100, 0x01010101
	};

	for (j = yStart; j < height; j += yPeriod) {
		mlib_s16 *sl = (mlib_s16 *)src + j * stride + xStart * nchan;
		mlib_s32 k = 0, l, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_s32 ind =
			    ((sl[0] -
			    ((mlib_s16 *)arr_min)[k]) *
			    ((mlib_s16 *)arr_scl)[k]) >> 16;

			if (sl[0] >= ((mlib_s16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_s16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}

			sl++;
			k++;
		}

		n -= k;

/* 'l' is mlib_d64 offset from sl */
		for (l = 0; l < 3; l++) {
			mlib_d64 d_min, d_max, d_scl;

			d_min = arr_min[k];
			d_max = arr_max[k];
			d_scl = arr_scl[k];

/* loop on buffer size */
			for (off = 0; off < n; off += size) {
				mlib_d64 *sp = (mlib_d64 *)(sl + 4 * l + off);
				mlib_d64 dzero = vis_fzero();
				mlib_u32 low_mask, hgh_mask, out_mask =
				    0, num_out = 0;
				mlib_s32 num, pnum;

				size = (n - off);

				if (size > SIZE)
					size = SIZE;

/* find histogram indexes */
/* #pragma pipeloop(0) */
				for (i = 0; i < (size + 11) / 12; i++) {
					mlib_d64 sd, td, dd, dh, dl, dh0, dh1,
					    dl0, dl1;

					sd = sp[3 * i];

					vis_pst_16(dzero, bufd + i, out_mask);
					num_out += tbl_out[out_mask];

					low_mask = vis_fcmplt16(sd, d_min);
					hgh_mask = vis_fcmpgt16(sd, d_max);
					out_mask = low_mask | hgh_mask;

					MLIB_SCALE_INDEX(dd, sd, d_scl, d_min);

					bufd[i + 1] = dd;
				}

				num = size / 12;
				pnum = size - 12 * num;

				if (pnum > 4)
					pnum = 4;

				if (pnum)
					out_mask &= (0xF0 >> pnum);
				vis_pst_16(dzero, bufd + i, out_mask);
				num_out += tbl_out[out_mask];

/* correct histo[k][0] */
				histogr[k][0] -= (num_out >> 24);
				histogr[k + 1][0] -= (num_out >> 16) & 0xFF;
				histogr[k + 2][0] -= (num_out >> 8) & 0xFF;
				histogr[k + 3][0] -= (num_out & 0xFF);

				mlib_v_ImageHistogram2_S16_43_D1(bufd + 1,
				    4 * num + pnum, histogr[k], histogr[k + 1],
				    histogr[k + 2]);
			}

/* correct 'k' and 'n' for next offset */
			k++;

			if (k >= 3)
				k -= 3;
			n -= 4;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageHistogram2_S16_2_D1(
    const void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1)
{
	mlib_u32 *sp = (void *)sl;
	mlib_u32 pix01, pix0, pix1;
	mlib_s32 elem0, elem1;
	mlib_s32 i;

	pix01 = (*sp++);
	pix0 = pix01 & 0xFFFF;
	pix1 = pix01 >> 16;

#pragma nomemorydepend
	for (i = 0; i < size / 2; i++) {
		elem0 = histo0[pix1] + 1;
		elem1 = histo1[pix0] + 1;
		histo0[pix1] = elem0;
		histo1[pix0] = elem1;
		pix01 = sp[i];
		pix0 = pix01 & 0xFFFF;
		pix1 = pix01 >> 16;
	}

	if (size & 1)
		histo0[pix1]++;
}

/* *********************************************************** */

void
mlib_v_ImageHistogram2_S16_4_D1(
    const void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	mlib_u32 *sp = (void *)sl, *send = sp + size / 2;
	mlib_u16 *ep;

#pragma nomemorydepend
	for (; sp <= (send - 2); sp += 2) {
		mlib_u32 pix01, pix23, pix0, pix1, pix2, pix3;

		pix01 = sp[0];
		pix23 = sp[1];

		pix0 = pix01 >> 16;
		pix1 = pix01 & 0xFFFF;
		pix2 = pix23 >> 16;
		pix3 = pix23 & 0xFFFF;

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;
	}

	ep = (mlib_u16 *)sp;

	if ((size & 3))
		histo0[ep[0]]++;

	if ((size & 3) > 1)
		histo1[ep[1]]++;

	if ((size & 3) > 2)
		histo2[ep[2]]++;
}

/* *********************************************************** */

void
mlib_v_ImageHistogram2_S16_43_D1(
    const void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2)
{
	mlib_u32 *sp = (void *)sl, *send = sp + size / 2;
	mlib_u16 *ep;

	for (; sp <= (send - 2); sp += 2) {
		mlib_u32 pix01, pix23, pix0, pix1, pix2, pix3;

		pix01 = sp[0];
		pix23 = sp[1];

		pix0 = pix01 >> 16;
		pix1 = pix01 & 0xFFFF;
		pix2 = pix23 >> 16;
		pix3 = pix23 & 0xFFFF;

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo0[pix3]++;
	}

	ep = (mlib_u16 *)sp;

	if ((size & 3))
		histo0[ep[0]]++;

	if ((size & 3) > 1)
		histo1[ep[1]]++;

	if ((size & 3) > 2)
		histo2[ep[2]]++;
}

/* *********************************************************** */
