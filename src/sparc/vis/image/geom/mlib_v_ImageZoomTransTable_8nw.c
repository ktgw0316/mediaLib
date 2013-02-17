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

#pragma ident	"@(#)mlib_v_ImageZoomTransTable_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTable.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_v_ImageChannelInsert.h>
#include <mlib_v_ImageZoomTransTable_8nw.h>

/* *********************************************************** */

#define	MALLOC_EXTRA	4*8

#define	MALLOC(size)	__mlib_malloc(size + MALLOC_EXTRA)

/* *********************************************************** */

#define	FLT_BITS_X	1
#define	FLT_BITS_Y	3

#define	FILTER_TYPE	mlib_d64

#define	FP_TYPE	mlib_d64

/* *********************************************************** */

static mlib_status mlib_v_ImageZoomTranslateTable_low_1ch(
    PARAMS_NW,
    FP_TYPE *buff);
static mlib_status mlib_v_ImageZoomTranslateTable_high_1ch(
    PARAMS_NW,
    FP_TYPE *buff);
static mlib_status mlib_v_ImageZoomTranslateTable_low_2ch(
    PARAMS_NW,
    FP_TYPE *buff);
static mlib_status mlib_v_ImageZoomTranslateTable_high_2ch(
    PARAMS_NW,
    FP_TYPE *buff);
static mlib_status mlib_v_ImageZoomTranslateTable_low_3ch(
    PARAMS_NW,
    FP_TYPE *buff);
static mlib_status mlib_v_ImageZoomTranslateTable_high_3ch(
    PARAMS_NW,
    FP_TYPE *buff);
static mlib_status mlib_v_ImageZoomTranslateTable_low_4ch(
    PARAMS_NW);
static mlib_status mlib_v_ImageZoomTranslateTable_high_4ch(
    PARAMS_NW);

/* *********************************************************** */

const mlib_u32 mlib_v_tbl_256DivAlpha[] = {
	0x00000000, 0xff00ff00, 0x7fff7fff, 0x55555555,
	0x40004000, 0x33333333, 0x2aab2aab, 0x24922492,
	0x20002000, 0x1c721c72, 0x199a199a, 0x17461746,
	0x15551555, 0x13b113b1, 0x12491249, 0x11111111,
	0x10001000, 0x0f0f0f0f, 0x0e390e39, 0x0d790d79,
	0x0ccd0ccd, 0x0c310c31, 0x0ba30ba3, 0x0b210b21,
	0x0aab0aab, 0x0a3d0a3d, 0x09d909d9, 0x097b097b,
	0x09250925, 0x08d408d4, 0x08890889, 0x08420842,
	0x08000800, 0x07c207c2, 0x07880788, 0x07500750,
	0x071c071c, 0x06eb06eb, 0x06bd06bd, 0x06900690,
	0x06660666, 0x063e063e, 0x06180618, 0x05f405f4,
	0x05d105d1, 0x05b005b0, 0x05910591, 0x05720572,
	0x05550555, 0x05390539, 0x051f051f, 0x05050505,
	0x04ec04ec, 0x04d504d5, 0x04be04be, 0x04a804a8,
	0x04920492, 0x047e047e, 0x046a046a, 0x04570457,
	0x04440444, 0x04320432, 0x04210421, 0x04100410,
	0x04000400, 0x03f003f0, 0x03e103e1, 0x03d203d2,
	0x03c403c4, 0x03b603b6, 0x03a803a8, 0x039b039b,
	0x038e038e, 0x03820382, 0x03760376, 0x036a036a,
	0x035e035e, 0x03530353, 0x03480348, 0x033e033e,
	0x03330333, 0x03290329, 0x031f031f, 0x03160316,
	0x030c030c, 0x03030303, 0x02fa02fa, 0x02f102f1,
	0x02e902e9, 0x02e002e0, 0x02d802d8, 0x02d002d0,
	0x02c802c8, 0x02c102c1, 0x02b902b9, 0x02b202b2,
	0x02ab02ab, 0x02a402a4, 0x029d029d, 0x02960296,
	0x028f028f, 0x02890289, 0x02830283, 0x027c027c,
	0x02760276, 0x02700270, 0x026a026a, 0x02640264,
	0x025f025f, 0x02590259, 0x02540254, 0x024e024e,
	0x02490249, 0x02440244, 0x023f023f, 0x023a023a,
	0x02350235, 0x02300230, 0x022b022b, 0x02270227,
	0x02220222, 0x021e021e, 0x02190219, 0x02150215,
	0x02110211, 0x020c020c, 0x02080208, 0x02040204,
	0x02000200, 0x01fc01fc, 0x01f801f8, 0x01f401f4,
	0x01f001f0, 0x01ed01ed, 0x01e901e9, 0x01e501e5,
	0x01e201e2, 0x01de01de, 0x01db01db, 0x01d701d7,
	0x01d401d4, 0x01d101d1, 0x01ce01ce, 0x01ca01ca,
	0x01c701c7, 0x01c401c4, 0x01c101c1, 0x01be01be,
	0x01bb01bb, 0x01b801b8, 0x01b501b5, 0x01b201b2,
	0x01af01af, 0x01ac01ac, 0x01aa01aa, 0x01a701a7,
	0x01a401a4, 0x01a101a1, 0x019f019f, 0x019c019c,
	0x019a019a, 0x01970197, 0x01950195, 0x01920192,
	0x01900190, 0x018d018d, 0x018b018b, 0x01880188,
	0x01860186, 0x01840184, 0x01820182, 0x017f017f,
	0x017d017d, 0x017b017b, 0x01790179, 0x01760176,
	0x01740174, 0x01720172, 0x01700170, 0x016e016e,
	0x016c016c, 0x016a016a, 0x01680168, 0x01660166,
	0x01640164, 0x01620162, 0x01600160, 0x015e015e,
	0x015d015d, 0x015b015b, 0x01590159, 0x01570157,
	0x01550155, 0x01540154, 0x01520152, 0x01500150,
	0x014e014e, 0x014d014d, 0x014b014b, 0x01490149,
	0x01480148, 0x01460146, 0x01440144, 0x01430143,
	0x01410141, 0x01400140, 0x013e013e, 0x013d013d,
	0x013b013b, 0x013a013a, 0x01380138, 0x01370137,
	0x01350135, 0x01340134, 0x01320132, 0x01310131,
	0x012f012f, 0x012e012e, 0x012d012d, 0x012b012b,
	0x012a012a, 0x01290129, 0x01270127, 0x01260126,
	0x01250125, 0x01230123, 0x01220122, 0x01210121,
	0x011f011f, 0x011e011e, 0x011d011d, 0x011c011c,
	0x011a011a, 0x01190119, 0x01180118, 0x01170117,
	0x01160116, 0x01150115, 0x01130113, 0x01120112,
	0x01110111, 0x01100110, 0x010f010f, 0x010e010e,
	0x010d010d, 0x010b010b, 0x010a010a, 0x01090109,
	0x01080108, 0x01070107, 0x01060106, 0x01050105,
	0x01040104, 0x01030103, 0x01020102, 0x01010101
};

/* *********************************************************** */

#define	SRC2FP(x)	mlib_U82D64[x]

/* *********************************************************** */

#define	DECLAREVAR                                              \
	FP_TYPE *ybuff;                                         \
	FP_TYPE dsat;                                           \
	mlib_s32 shift, y, x, xoff;                             \
	mlib_s32 xLeft, xRight, x0, y0;                         \
	mlib_s32 ySrc;                                          \
	mlib_s32 yStart = ws->yStart;                           \
	mlib_s32 yFinish = ws->yFinish;                         \
	mlib_s32 max_xsize = ws->max_xsize;                     \
	mlib_s32 dstYStride = ws->dstStride;                    \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 filterpos;                                     \
	mlib_s32 size;                                          \
	mlib_f32 *dl;                                           \
	mlib_u8 *dul;                                           \
	mlib_s32 xf_shift, xf_mask, yf_shift, yf_mask;          \
	mlib_s32 m = table->width;                              \
	mlib_s32 n = table->height;                             \
	mlib_s32 xf_bits = table->width_bits;                   \
	mlib_s32 yf_bits = table->height_bits;                  \
	mlib_s32 subsampleBitsH = table->subsampleBitsH;        \
	mlib_s32 subsampleBitsV = table->subsampleBitsV;        \
	FILTER_TYPE *filterX, *filterY, *fptr;                  \
	mlib_s32 i, j, l, kw, off

/* *********************************************************** */

#define	CLIP(N)                                                 \
	dstData += dstYStride;                                  \
	xLeft = leftEdges[j];                                   \
	xRight = rightEdges[j];                                 \
	size = xRight - xLeft + 1;                              \
	x0 = xStarts[j];                                        \
	y0 = yStarts[j];                                        \
	if (xLeft > xRight)                                     \
	    continue;                                           \
	dul = dstData + N * xLeft

/* *********************************************************** */

#define	DIST_BITS()                                                    \
	xf_shift = x_shift - subsampleBitsH - xf_bits - FLT_BITS_X;    \
	xf_mask =                                                      \
	    (((1 << subsampleBitsH) - 1) << (xf_bits + FLT_BITS_X));   \
	yf_shift = y_shift - subsampleBitsV - yf_bits - FLT_BITS_Y;    \
	yf_mask =                                                      \
	    (((1 << subsampleBitsV) - 1) << (yf_bits + FLT_BITS_Y))

/* *********************************************************** */

#define	CALC_SRC_PTRY(N)	sp0 = (mlib_u8*)lineAddr[ySrc + l] + N*off

/* *********************************************************** */

#define	CALC_SRC_PTRY2(N)                                       \
	CALC_SRC_PTRY(N);                                       \
	sp1 = (mlib_u8 *)lineAddr[ySrc + l + 1] + N * off

/* *********************************************************** */

#define	CALC_SRC_PTRY3(N)                                       \
	CALC_SRC_PTRY2(N);                                      \
	sp2 = (mlib_u8 *)lineAddr[ySrc + l + 2] + N * off

/* *********************************************************** */

#define	CALC_SRC_PTRY4(N)                                       \
	CALC_SRC_PTRY3(N);                                      \
	sp3 = (mlib_u8 *)lineAddr[ySrc + l + 3] + N * off

/* *********************************************************** */

#define	MLIB_FILTER(X, filter, l)                                    \
	filterpos = ((X) >> X##f_shift) & X##f_mask;                 \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterpos + l)

/* *********************************************************** */

#define	MAX_KER	4

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_low_1ch(
    PARAMS_NW,
    FP_TYPE *buff)
{
	DECLAREVAR;
	FP_TYPE *bufft, *xbuff, *xb;
	void *sp0, *sp1, *sp2, *sp3;
	FP_TYPE bb;
	mlib_s32 kh;

	ybuff = buff;
	bufft = ybuff + (max_xsize >> 2);

	xbuff = MALLOC(((m + 3) >> 2) * max_xsize * sizeof (FP_TYPE));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;
	ws->dy = 4 * ws->dx;

#pragma pipeloop(0)

	for (i = 0; i < max_xsize >> 2; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		xLeft = leftEdges[j];
		xRight = rightEdges[j];

		if (xLeft > xRight)
			continue;
		size = xRight - xLeft + 1;
		x0 = xStarts[j];
		x0 -= ws->x_move;

		xb = xbuff;
		vis_alignaddr((void *)0, 6);
		ws->size = size;
		for (off = 0; off < m; off += kw) {
			x = x0;
			xoff = 2 * off;
			ws->off = xoff;

			kw = m - off;

			if (kw > MAX_KER)
				kw = MAX_KER;

			if (kw == 4) {
				mlib_calc_coef_4x_1ch(x, filterX, ws, xb);

			} else if (kw == 3) {
				mlib_calc_coef_3x_1ch(x, filterX, ws, xb);

			} else if (kw == 2) {
				mlib_calc_coef_2x_1ch(x, filterX, ws, xb);

			} else {
				mlib_calc_coef_1x_1ch(x, filterX, ws, xb);
			}

			xb += max_xsize;
		}

		break;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(1);
		ws->size = size;

		if (((mlib_addr)dul & 3) || (size & 0x3)) {
			dl = (mlib_f32 *)bufft;
		} else {
			dl = (mlib_f32 *)dul;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ySrc = y >> y_shift;

		vis_alignaddr((void *)0, 7);
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if (kh >= 4)
				kh = 4;
			xb = xbuff;
			MLIB_FILTER(y, filterY, 8 * l);

			if (kh == 4) {
				for (off = 0; off < m; off += kw) {
					x = x0;
					CALC_SRC_PTRY4(1);
					kw = m - off;

					if (kw > MAX_KER)
						kw = MAX_KER;

					if (kw == 4) {
						mlib_calc_4y_4x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2,
						    sp3);

					} else if (kw == 3) {
						mlib_calc_4y_3x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2,
						    sp3);

					} else if (kw == 2) {
						mlib_calc_4y_2x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2,
						    sp3);

					} else {
						mlib_calc_4y_1x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2,
						    sp3);
					}

					xb += max_xsize;
				}

			} else if (kh == 3) {

				for (off = 0; off < m; off += kw) {
					x = x0;

					CALC_SRC_PTRY3(1);
					kw = m - off;

					if (kw > MAX_KER)
						kw = MAX_KER;

					if (kw == 4) {
						mlib_calc_3y_4x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2);

					} else if (kw == 3) {
						mlib_calc_3y_3x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2);

					} else if (kw == 2) {
						mlib_calc_3y_2x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2);

					} else {
						mlib_calc_3y_1x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1, sp2);
					}

					xb += max_xsize;
				}

			} else if (kh == 2) {

				for (off = 0; off < m; off += kw) {
					x = x0;

					CALC_SRC_PTRY2(1);
					kw = m - off;

					if (kw > MAX_KER)
						kw = MAX_KER;

					if (kw == 4) {
						mlib_calc_2y_4x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1);

					} else if (kw == 3) {
						mlib_calc_2y_3x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1);

					} else if (kw == 2) {
						mlib_calc_2y_2x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1);

					} else {
						mlib_calc_2y_1x_1ch(x, ws, fptr,
						    xb, ybuff, sp0, sp1);
					}

					xb += max_xsize;
				}

			} else {
/* kh == 1 */

				for (off = 0; off < m; off += kw) {
					x = x0;

					CALC_SRC_PTRY(1);
					kw = m - off;

					if (kw > MAX_KER)
						kw = MAX_KER;

					if (kw == 4) {
						mlib_calc_1y_4x_1ch(x, ws, fptr,
						    xb, ybuff, sp0);

					} else if (kw == 3) {
						mlib_calc_1y_3x_1ch(x, ws, fptr,
						    xb, ybuff, sp0);

					} else if (kw == 2) {
						mlib_calc_1y_2x_1ch(x, ws, fptr,
						    xb, ybuff, sp0);

					} else {
						mlib_calc_1y_1x_1ch(x, ws, fptr,
						    xb, ybuff, sp0);
					}

					xb += max_xsize;
				}
			}
		}

#pragma pipeloop(0)
		for (i = 0; i < (size + 3) >> 2; i++) {
			bb = ybuff[i];
			dl[i] = vis_fpack16(bb);
			ybuff[i] = dsat;
		}

		if (((mlib_u8 *)dl != dul) || (size & 0x3)) {
			mlib_ImageCopy_na((mlib_u8 *)dl, dul, size);
		}
	}

	__mlib_free(xbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_high_1ch(
    PARAMS_NW,
    FP_TYPE *buff)
{
	DECLAREVAR;
	FP_TYPE *bufft, *pbuff, **buff_arr;
	FP_TYPE *xbuff, *xb;
	mlib_u8 *sp0;
	mlib_s32 size_div4;
	mlib_s32 bind, y_step, k, ynew;
	mlib_s32 kh;

	xbuff =
	    MALLOC(((m + 3) >> 2) * max_xsize * sizeof (FP_TYPE) +
	    2 * n * sizeof (FP_TYPE *));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	buff_arr = (FP_TYPE **) (xbuff + ((m + 3) >> 2) * max_xsize);

	for (k = 0; k < n; k++)
		buff_arr[n + k] = buff_arr[k] = buff + k * (max_xsize >> 2);
	ybuff = buff + n * (max_xsize >> 2);
	bufft = ybuff + (max_xsize >> 2);

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;
	ws->dy = 4 * ws->dx;

	y_step = n;
	bind = 0;

#pragma pipeloop(0)

	for (i = 0; i < max_xsize >> 2; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		xLeft = leftEdges[j];
		xRight = rightEdges[j];

		if (xLeft > xRight)
			continue;
		size = xRight - xLeft + 1;
		x0 = xStarts[j];
		x0 -= ws->x_move;

		xb = xbuff;
		vis_alignaddr((void *)0, 6);
		ws->size = size;
		for (off = 0; off < m; off += kw) {
			x = x0;
			xoff = 2 * off;
			ws->off = xoff;

			kw = m - off;

			if (kw == 5)
				kw = 3;
			if (kw > MAX_KER)
				kw = MAX_KER;

			if (kw == 4) {
				mlib_calc_coef_4x_high_1ch(x, filterX, ws, xb);

			} else if (kw == 3) {
				mlib_calc_coef_3x_high_1ch(x, filterX, ws, xb);

			} else {
				mlib_calc_coef_2x_high_1ch(x, filterX, ws, xb);
			}

			xb += max_xsize;
		}

		break;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(1);
		ynew = yStarts[j + 1];
		size_div4 = (size + 3) >> 2;
		ws->size = size;

		if (((mlib_addr)dul & 3) || (size & 0x3)) {
			dl = (mlib_f32 *)bufft;
		} else {
			dl = (mlib_f32 *)dul;
		}

		if (y_step >= n) {
			y_step = n;
		} else {
			bind += y_step;

			if (bind >= n)
				bind -= n;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ynew = (ynew - ws->y_move) >> y_shift;
		ySrc = y >> y_shift;

		vis_alignaddr((void *)0, 7);
		for (l = n - y_step; l < n; l++) {

			pbuff = buff_arr[bind + l];

			xb = xbuff;
			for (off = 0; off < m; off += kw) {
				x = x0;

				CALC_SRC_PTRY(1);
				kw = m - off;

				if (kw == 5)
					kw = 3;
				if (kw > MAX_KER)
					kw = MAX_KER;

				if (off == 0) {
					if (kw == 4) {
						mlib_calc_4x_off_1ch(x, ws, xb,
						    pbuff, sp0);

					} else if (kw == 3) {
						mlib_calc_3x_off_1ch(x, ws, xb,
						    pbuff, sp0);

					} else {
						mlib_calc_2x_off_1ch(x, ws, xb,
						    pbuff, sp0);
					}

				} else {

					if (kw == 4) {
						mlib_calc_4x_1ch(x, ws, xb,
						    pbuff, sp0);

					} else if (kw == 3) {
						mlib_calc_3x_1ch(x, ws, xb,
						    pbuff, sp0);

					} else {
						mlib_calc_2x_1ch(x, ws, xb,
						    pbuff, sp0);
					}
				}

				xb += max_xsize;
			}
		}

		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if (kh >= MAX_KER)
				kh = MAX_KER;
			MLIB_FILTER(y, filterY, 8 * l);

			if (kh == 4) {
				if ((l + kh) == n) {
					mlib_calc_y4c(fptr, buff_arr, bind + l,
					    ybuff, size_div4, dsat, dl);
				} else {
					mlib_calc_y4(fptr, buff_arr, bind + l,
					    ybuff, size_div4);
				}

			} else if (kh == 3) {

				if ((l + kh) == n) {
					mlib_calc_y3c(fptr, buff_arr, bind + l,
					    ybuff, size_div4, dsat, dl);
				} else {
					mlib_calc_y3(fptr, buff_arr, bind + l,
					    ybuff, size_div4);
				}

			} else if (kh == 2) {

				if ((l + kh) == n) {
					mlib_calc_y2c(fptr, buff_arr, bind + l,
					    ybuff, size_div4, dsat, dl);
				} else {
					mlib_calc_y2(fptr, buff_arr, bind + l,
					    ybuff, size_div4);
				}

			} else {

				if ((l + kh) == n) {
					mlib_calc_y1c(fptr, buff_arr, bind + l,
					    ybuff, size_div4, dsat, dl);
				} else {
					mlib_calc_y1(fptr, buff_arr, bind + l,
					    ybuff, size_div4);
				}
			}
		}

		if ((mlib_u8 *)dl != dul) {
			mlib_ImageCopy_na((mlib_u8 *)dl, dul, size);
		}

		y_step = ynew - ySrc;
	}

	__mlib_free(xbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_1_8nw(
    PARAMS_NW)
{
	FP_TYPE *buff, zoomy;
	mlib_status status;
	mlib_s32 n, div_y, max_size;

	max_size = ws->max_xsize;
	zoomy = ws->zoomy;
	div_y = (mlib_s32)(1 / zoomy);
	n = table->height;

	max_size = (max_size + 3) & ~3;

	ws->max_xsize = max_size;

	buff = MALLOC(((n + 2) * (max_size >> 2) + 1) * sizeof (FP_TYPE));

	if (buff == NULL) {
		return (MLIB_FAILURE);
	}

	if (div_y >= n) {
		status =
		    mlib_v_ImageZoomTranslateTable_low_1ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table, buff);
	} else {
		status =
		    mlib_v_ImageZoomTranslateTable_high_1ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table, buff);
	}

	if (status != MLIB_SUCCESS) {
		__mlib_free(buff);
		return (MLIB_FAILURE);
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_low_2ch(
    PARAMS_NW,
    FP_TYPE *buff)
{
	DECLAREVAR;
	FP_TYPE *dbuff, *xbuff, *xb;
	mlib_u8 *sp0, *sp1;

	ybuff = buff;
	dbuff = ybuff + (max_xsize >> 1);

	xbuff = MALLOC(2 * ((m + 3) >> 2) * max_xsize * sizeof (FP_TYPE));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

#pragma pipeloop(0)
	for (i = 0; i < max_xsize >> 1; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		xLeft = leftEdges[j];
		xRight = rightEdges[j];

		if (xLeft > xRight)
			continue;
		size = xRight - xLeft + 1;

		ws->size = (size + 1) >> 1;
		ws->dy = 2 * ws->dx;

		x0 = xStarts[j];
		x0 -= ws->x_move;

		xb = xbuff;
		vis_alignaddr((void *)0, 6);
		for (off = 0; off < m; off += kw) {
			x = x0;
			xoff = 2 * off;
			ws->off = xoff;

			kw = m - off;

			if (kw == 5)
				kw = 3;
			if (kw > MAX_KER)
				kw = MAX_KER;

			if (kw == 4) {
				mlib_calc_coef_4x_2ch(x, filterX, ws, xb);

			} else if (kw == 3) {
				mlib_calc_coef_3x_2ch(x, filterX, ws, xb);

			} else {
				mlib_calc_coef_2x_2ch(x, filterX, ws, xb);
			}

			xb += 2 * max_xsize;
		}

		break;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(2);
		ws->size = size;

		if (((mlib_addr)dul & 3) || (size & 0x1)) {
			dl = (mlib_f32 *)dbuff;
		} else {
			dl = (mlib_f32 *)dul;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ySrc = y >> y_shift;

		vis_alignaddr((void *)0, 6);
		for (l = 0; l < n; l++) {

			MLIB_FILTER(y, filterY, 8 * l);
			sp1 = lineAddr[ySrc + l];

			xb = xbuff;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {
				x = x0;
				xoff = 2 * off;
				ws->off = xoff;
				sp0 = sp1 + xoff;

				kw = m - off;

				if (kw == 5)
					kw = 3;
				if (kw > MAX_KER)
					kw = MAX_KER;

				if (kw == 4) {
					mlib_calc_h1_w4_2ch(x, ws, fptr, ybuff,
					    xb, sp0);

				} else if (kw == 3) {
					mlib_calc_h1_w3_2ch(x, ws, fptr, ybuff,
					    xb, sp0);

				} else {
					mlib_calc_h1_w2_2ch(x, ws, fptr, ybuff,
					    xb, sp0);
				}

				xb += 2 * max_xsize;
			}
		}

#pragma pipeloop(0)
		for (i = 0; i < (size + 1) >> 1; i++) {
			dl[i] = vis_fpack16(ybuff[i]);
			ybuff[i] = dsat;
		}

		if ((mlib_u8 *)dl != dul) {
			mlib_ImageCopy_na((mlib_u8 *)dl, dul, 2 * size);
		}
	}

	__mlib_free(xbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_high_2ch(
    PARAMS_NW,
    FP_TYPE *buff)
{
	DECLAREVAR;
	FP_TYPE *dbuff, *pbuff, **buff_arr;
	FP_TYPE *xbuff, *xb;
	mlib_u8 *sp0, *sp1;
	mlib_s32 div_size2;
	mlib_s32 bind, y_step, ynew;
	mlib_s32 k;
	mlib_s32 kh;

	xbuff =
	    MALLOC(2 * ((m + 3) >> 2) * max_xsize * sizeof (FP_TYPE) +
	    2 * n * sizeof (FP_TYPE *));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	buff_arr = (FP_TYPE **) (xbuff + 2 * ((m + 3) >> 2) * max_xsize);

	for (k = 0; k < n; k++)
		buff_arr[n + k] = buff_arr[k] = buff + k * (max_xsize >> 1);
	ybuff = buff + n * (max_xsize >> 1);
	dbuff = ybuff + (max_xsize >> 1);

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	y_step = n;
	bind = 0;

#pragma pipeloop(0)
	for (i = 0; i < max_xsize >> 1; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		xLeft = leftEdges[j];
		xRight = rightEdges[j];

		if (xLeft > xRight)
			continue;
		size = xRight - xLeft + 1;

		ws->size = (size + 1) >> 1;
		ws->dy = 2 * ws->dx;

		x0 = xStarts[j];
		x0 -= ws->x_move;

		xb = xbuff;
		vis_alignaddr((void *)0, 6);
		for (off = 0; off < m; off += kw) {
			x = x0;
			xoff = 2 * off;
			ws->off = xoff;

			kw = m - off;

			if (kw == 5)
				kw = 3;
			if (kw > MAX_KER)
				kw = MAX_KER;

			if (kw == 4) {
				mlib_calc_coef_4x_high_2ch(x, filterX, ws, xb);

			} else if (kw == 3) {
				mlib_calc_coef_3x_high_2ch(x, filterX, ws, xb);

			} else {
				mlib_calc_coef_2x_high_2ch(x, filterX, ws, xb);
			}

			xb += 2 * max_xsize;
		}

		break;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(2);
		div_size2 = (size + 1) >> 1;
		ws->size = size;
		ynew = yStarts[j + 1];

		if (((mlib_addr)dul & 3) || (size & 0x1)) {
			dl = (mlib_f32 *)dbuff;
		} else {
			dl = (mlib_f32 *)dul;
		}

		if (y_step >= n) {
			y_step = n;
		} else {
			bind += y_step;

			if (bind >= n)
				bind -= n;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ynew = (ynew - ws->y_move) >> y_shift;
		ySrc = y >> y_shift;

		vis_alignaddr((void *)0, 6);
		for (l = n - y_step; l < n; l++) {
			pbuff = buff_arr[bind + l];
			sp1 = lineAddr[ySrc + l];

			xb = xbuff;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {
				x = x0;
				xoff = 2 * off;
				ws->off = xoff;
				sp0 = sp1 + xoff;

				kw = m - off;

				if (kw == 5)
					kw = 3;
				if (kw > MAX_KER)
					kw = MAX_KER;

				if (off == 0) {
					if (kw == 4) {
						mlib_calc_h_4_2ch(x, ws, pbuff,
						    xb, sp0);

					} else if (kw == 3) {
						mlib_calc_h_3_2ch(x, ws, pbuff,
						    xb, sp0);

					} else {
						mlib_calc_h_2_2ch(x, ws, pbuff,
						    xb, sp0);
					}

				} else {

					if (kw == 4) {
						mlib_calc_h_4_off_2ch(x, ws,
						    pbuff, xb, sp0);

					} else if (kw == 3) {
						mlib_calc_h_3_off_2ch(x, ws,
						    pbuff, xb, sp0);

					} else {
						mlib_calc_h_2_off_2ch(x, ws,
						    pbuff, xb, sp0);
					}
				}

				xb += 2 * max_xsize;
			}
		}

		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if (kh >= MAX_KER)
				kh = MAX_KER;

			MLIB_FILTER(y, filterY, 8 * l);

			if (kh == 4) {
				if ((l + kh) == n) {
					mlib_calc_y4c(fptr, buff_arr, bind + l,
					    ybuff, div_size2, dsat, dl);
				} else {
					mlib_calc_y4(fptr, buff_arr, bind + l,
					    ybuff, div_size2);
				}

			} else if (kh == 3) {

				if ((l + kh) == n) {
					mlib_calc_y3c(fptr, buff_arr, bind + l,
					    ybuff, div_size2, dsat, dl);
				} else {
					mlib_calc_y3(fptr, buff_arr, bind + l,
					    ybuff, div_size2);
				}

			} else if (kh == 2) {

				if ((l + kh) == n) {
					mlib_calc_y2c(fptr, buff_arr, bind + l,
					    ybuff, div_size2, dsat, dl);
				} else {
					mlib_calc_y2(fptr, buff_arr, bind + l,
					    ybuff, div_size2);
				}

			} else {

				if ((l + kh) == n) {
					mlib_calc_y1c(fptr, buff_arr, bind + l,
					    ybuff, div_size2, dsat, dl);
				} else {
					mlib_calc_y1(fptr, buff_arr, bind + l,
					    ybuff, div_size2);
				}
			}
		}

		if ((mlib_u8 *)dl != dul) {
			mlib_ImageCopy_na((mlib_u8 *)dl, dul, 2 * size);
		}

		y_step = ynew - ySrc;
	}

	__mlib_free(xbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_2_8nw(
    PARAMS_NW)
{
	FP_TYPE *buff;
	mlib_d64 zoomy;
	mlib_s32 n, div_y, max_xsize;
	mlib_status status;

	max_xsize = ws->max_xsize;
	zoomy = ws->zoomy;
	div_y = (mlib_s32)(1 / zoomy);
	n = table->height;

	if (max_xsize & 1)
		max_xsize++;
	ws->max_xsize = max_xsize;

	buff = MALLOC(((n + 2) * (max_xsize >> 1) + 1) * sizeof (FP_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	if (div_y > n) {
		status =
		    mlib_v_ImageZoomTranslateTable_low_2ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table, buff);
	} else {
		status =
		    mlib_v_ImageZoomTranslateTable_high_2ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table, buff);
	}

	if (status != MLIB_SUCCESS) {
		__mlib_free(buff);
		return (MLIB_FAILURE);
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_low_3ch(
    PARAMS_NW,
    FP_TYPE *buff)
{
	DECLAREVAR;
	FP_TYPE *dbuff;
	mlib_u8 *sup0, *sup1;

	ybuff = buff;
	dbuff = ybuff + max_xsize;

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

#pragma pipeloop(0)

	for (i = 0; i < max_xsize; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(3);
		ws->size = size;
		dl = (mlib_f32 *)dbuff;

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ySrc = y >> y_shift;

		for (l = 0; l < n; l++) {

			MLIB_FILTER(y, filterY, 8 * l);
			sup1 = (mlib_u8 *)lineAddr[ySrc + l];

/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {
				x = x0;
				xoff = 2 * off;
				ws->off = xoff;
				sup0 = sup1 + 3 * off;

				kw = m - off;

				if (kw > MAX_KER)
					kw = MAX_KER;

				if (kw == 4) {
					if (((l + 1) == n) &&
					    ((off + kw) == m)) {
						mlib_calc_1_4s_3ch(fptr, x,
						    filterX, ws, ybuff, sup0,
						    dsat, dl);
					} else {
						mlib_calc_1_4_3ch(fptr, x,
						    filterX, ws, ybuff, sup0);
					}

				} else if (kw == 3) {

					if (((l + 1) == n) &&
					    ((off + kw) == m)) {
						mlib_calc_1_3s_3ch(fptr, x,
						    filterX, ws, ybuff, sup0,
						    dsat, dl);
					} else {
						mlib_calc_1_3_3ch(fptr, x,
						    filterX, ws, ybuff, sup0);
					}

				} else if (kw == 2) {

					if (((l + 1) == n) &&
					    ((off + kw) == m)) {
						mlib_calc_1_2s_3ch(fptr, x,
						    filterX, ws, ybuff, sup0,
						    dsat, dl);
					} else {
						mlib_calc_1_2_3ch(fptr, x,
						    filterX, ws, ybuff, sup0);
					}

				} else {

					if (((l + 1) == n) &&
					    ((off + kw) == m)) {
						mlib_calc_1_1s_3ch(fptr, x,
						    filterX, ws, ybuff, sup0,
						    dsat, dl);
					} else {
						mlib_calc_1_1_3ch(fptr, x,
						    filterX, ws, ybuff, sup0);
					}
				}
			}
		}

		mlib_v_ImageChannelExtract_U8_43L_D1((mlib_u8 *)dl,
		    (mlib_u8 *)dul, size);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_high_3ch(
    PARAMS_NW,
    FP_TYPE *buff)
{
	DECLAREVAR;
	FP_TYPE *dbuff, *pbuff, **buff_arr;
	mlib_f32 *xbuff, *xb;
	mlib_f32 *sp0;
	mlib_u8 *sup0;
	mlib_s32 bind, y_step;
	mlib_s32 k;
	mlib_s32 src_size = ws->srcWidth;
	mlib_s32 ynew;
	mlib_s32 kh;

	xbuff =
	    MALLOC(((src_size + 2) & ~1) * sizeof (mlib_f32) +
	    2 * n * sizeof (FP_TYPE *));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	buff_arr = (FP_TYPE **) (xbuff + ((src_size + 2) & ~1));

	for (k = 0; k < n; k++)
		buff_arr[n + k] = buff_arr[k] = buff + k * max_xsize;
	ybuff = buff + n * max_xsize;
	dbuff = ybuff + max_xsize;

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	y_step = n;
	bind = 0;

#pragma pipeloop(0)

	for (i = 0; i < max_xsize; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(3);
		ynew = yStarts[j + 1];
		ws->size = size;
		dl = (mlib_f32 *)dbuff;
		xb = (mlib_f32 *)xbuff;

		if (y_step >= n) {
			y_step = n;
		} else {
			bind += y_step;

			if (bind >= n)
				bind -= n;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ynew = (ynew - ws->y_move) >> y_shift;
		ySrc = y >> y_shift;

		for (l = n - y_step; l < n; l++) {

			pbuff = buff_arr[bind + l];
			sup0 = (mlib_u8 *)lineAddr[ySrc + l];

			mlib_v_ImageChannelInsert_U8_34R_D1((mlib_u8 *)sup0,
			    (mlib_u8 *)xb, src_size);

/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {
				x = x0;
				xoff = 2 * off;
				ws->off = xoff;
				sp0 = xb + off;

				kw = m - off;

				if (kw > MAX_KER)
					kw = MAX_KER;

				if (off == 0) {
					if (kw == 4) {
						mlib_calc_h_4_3ch(x, filterX,
						    ws, pbuff, sp0);

					} else if (kw == 3) {
						mlib_calc_h_3_3ch(x, filterX,
						    ws, pbuff, sp0);

					} else if (kw == 2) {
						mlib_calc_h_2_3ch(x, filterX,
						    ws, pbuff, sp0);

					} else {	/* if (kw == 1) */

						mlib_calc_h_1_3ch(x, filterX,
						    ws, pbuff, sp0);
					}

				} else {

					if (kw == 4) {
						mlib_calc_h_4_off_3ch(x,
						    filterX, ws, pbuff, sp0);

					} else if (kw == 3) {
						mlib_calc_h_3_off_3ch(x,
						    filterX, ws, pbuff, sp0);

					} else if (kw == 2) {
						mlib_calc_h_2_off_3ch(x,
						    filterX, ws, pbuff, sp0);

					} else {	/* if (kw == 1) */

						mlib_calc_h_1_off_3ch(x,
						    filterX, ws, pbuff, sp0);
					}
				}
			}
		}

		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if (kh >= MAX_KER)
				kh = MAX_KER;

			MLIB_FILTER(y, filterY, 8 * l);

			if (kh == 4) {
				if ((l + kh) == n) {
					mlib_calc_y4c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y4(fptr, buff_arr, bind + l,
					    ybuff, size);
				}

			} else if (kh == 3) {

				if ((l + kh) == n) {
					mlib_calc_y3c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y3(fptr, buff_arr, bind + l,
					    ybuff, size);
				}

			} else if (kh == 2) {

				if ((l + kh) == n) {
					mlib_calc_y2c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y2(fptr, buff_arr, bind + l,
					    ybuff, size);
				}

			} else {
/* kh == 1 */

				if ((l + kh) == n) {
					mlib_calc_y1c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y1(fptr, buff_arr, bind + l,
					    ybuff, size);
				}
			}
		}

		mlib_v_ImageChannelExtract_U8_43R_D1((mlib_u8 *)dl,
		    (mlib_u8 *)dul, size);

		y_step = ynew - ySrc;
	}

	__mlib_free(xbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_3_8nw(
    PARAMS_NW)
{
	FP_TYPE *buff;
	mlib_d64 zoomy;
	mlib_s32 n, div_y, max_xsize;
	mlib_status status;

	if (ws->alpha_shift)
		return mlib_v_ImageZoomTranslateTable_high_4ch(dstData,
		    lineAddr, leftEdges, rightEdges, xStarts, yStarts, ws,
		    table);

	max_xsize = ws->max_xsize;
	zoomy = ws->zoomy;
	div_y = (mlib_s32)(1 / zoomy);
	n = table->height;

	ws->max_xsize = max_xsize;

	buff = MALLOC(((n + 2) * max_xsize + 1) * sizeof (FP_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	if (div_y >= n) {
		status =
		    mlib_v_ImageZoomTranslateTable_low_3ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table, buff);
	} else {
		status =
		    mlib_v_ImageZoomTranslateTable_high_3ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table, buff);
	}

	if (status != MLIB_SUCCESS) {
		__mlib_free(buff);
		return (MLIB_FAILURE);
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_low_4ch(
    PARAMS_NW)
{
	DECLAREVAR;
	FP_TYPE val;
	FP_TYPE *buff;
	void *sp0, *sp1;
	mlib_s32 kh;

	buff = MALLOC(max_xsize * sizeof (FP_TYPE));
	if (buff == NULL)
		return (MLIB_FAILURE);

	ybuff = buff;

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	vis_write_gsr((7 - shift) << 3);

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

#pragma pipeloop(0)

	for (i = 0; i < max_xsize; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(4);
		dl = (mlib_f32 *)dul;

		ws->size = size;

/* move to kernel center */
		x0 -= ws->x_move;
		y = y0 - ws->y_move;

		ySrc = y >> y_shift;

		for (l = 0; l < n; l += kh) {

			kh = n - l;

			if (kh >= 2)
				kh = 2;

			MLIB_FILTER(y, filterY, 8 * l);

			if (kh == 2) {
/* offset in current kernel line */
				for (off = 0; off < m; off += kw) {
					x = x0;
					xoff = 2 * off;
					ws->off = xoff;

					CALC_SRC_PTRY2(4);
					kw = m - off;

					if (kw > MAX_KER)
						kw = MAX_KER;

					if (kw == 4) {
						mlib_calc_2_4_4ch(fptr, x,
						    filterX, ws, ybuff, sp0,
						    sp1);

					} else if (kw == 3) {
						mlib_calc_2_3_4ch(fptr, x,
						    filterX, ws, ybuff, sp0,
						    sp1);

					} else if (kw == 2) {
						mlib_calc_2_2_4ch(fptr, x,
						    filterX, ws, ybuff, sp0,
						    sp1);

					} else {	/* if (kw == 1) */

						mlib_calc_2_1_4ch(fptr, x,
						    filterX, ws, ybuff, sp0,
						    sp1);
					}
				}

			} else {	/* kh == 1 */

/* offset in current kernel line */
				for (off = 0; off < m; off += kw) {
					x = x0;
					xoff = 2 * off;
					ws->off = xoff;

					CALC_SRC_PTRY(4);
					kw = m - off;

					if (kw > MAX_KER)
						kw = MAX_KER;

					if (kw == 4) {
						mlib_calc_1_4_4ch(fptr, x,
						    filterX, ws, ybuff, sp0);

					} else if (kw == 3) {
						mlib_calc_1_3_4ch(fptr, x,
						    filterX, ws, ybuff, sp0);

					} else if (kw == 2) {
						mlib_calc_1_2_4ch(fptr, x,
						    filterX, ws, ybuff, sp0);

					} else {	/* if (kw == 1) */

						mlib_calc_1_1_4ch(fptr, x,
						    filterX, ws, ybuff, sp0);
					}
				}
			}
		}

#pragma pipeloop(0)
		for (i = 0; i < size; i++) {
			val = ybuff[i];
			dl[i] = vis_fpack16(val);
			ybuff[i] = dsat;
		}
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	EXPAND0(x)	vis_fpmerge(fzeros, x)

#define	MUL_ALPHA_4CH(sp, dp, h1, h2)                           \
	{                                                       \
	    mlib_d64 ss, a0, a1, d0, d1;                        \
	                                                        \
	    ss = vis_freg_pair(((mlib_f32 *)sp)[2 * i],         \
		((mlib_f32 *)sp)[2 * i + 1]);                   \
	    a0 = EXPAND0(vis_read_hi(ss));                      \
	    a1 = EXPAND0(vis_read_lo(ss));                      \
	    d0 = vis_fmul8x16##h2(vis_read_hi(ss),              \
		vis_read_##h1(a0));                             \
	    d1 = vis_fmul8x16##h2(vis_read_lo(ss),              \
		vis_read_##h1(a1));                             \
	    d0 = vis_fpack16_pair(d0, d1);                      \
	    vis_pst_8(d0, (mlib_d64 *)dp + i, ~cmask);          \
	    vis_pst_8(ss, (mlib_d64 *)dp + i, cmask);           \
	}

/* *********************************************************** */
#define	BLEND(dst, hh1, hh2, hh3, ind)                                 \
	{                                                              \
	    mlib_d64 tmp1, tmp2, tmp3;                                 \
	                                                               \
	    tmp1 = vis_fmul8x16(vis_read_##hh3(ss), done);             \
	    tmp2 = vis_fpsub16(done, tmp1);                            \
	    tmp2 = vis_fmul8x16(vis_read_##hh3(dd), tmp2);             \
	    tmp3 =                                                     \
		vis_fmul8x16##hh1(vis_read_##hh3(dd),                  \
		vis_read_##hh2(tmp2));                                 \
	    tmp3 =                                                     \
		vis_fpadd16(tmp3, vis_fmul8x16au(vis_read_##hh3(ss),   \
		aa));                                                  \
	    tmp2 = vis_fpadd16(tmp1, tmp2);                            \
	    dst = vis_for(vis_fand(mask, tmp3), tmp2);                 \
	}

/* *********************************************************** */
#define	BLEND44(hh1, hh2)                                        \
	{                                                        \
	    mlib_d64 s0, dd, ss, d0, d1;                         \
	                                                         \
	    s0 = s1;                                             \
	    s1 = vis_ld_d64_nf(dp + 1);                          \
	    dd = vis_faligndata(s0, s1);                         \
	    dp++;                                                \
	    ss = buffd[i];                                       \
	    BLEND(d0, hh1, hh2, hi, 0) BLEND(d1, hh1, hh2, lo,   \
		1) buffd[i] = vis_fpack16_pair(d0, d1);          \
	}

/* *********************************************************** */
#define	DIV_ALPHA(dst, ind0, ind1, alp_index)                       \
	{                                                           \
	    mlib_d64 d0, d1;                                        \
	    mlib_f32 coeff;                                         \
	    mlib_u64 ind;                                           \
	                                                            \
	    dst = buffd[i];                                         \
	    ind = *((mlib_u64 *)buffd + i);                         \
	    ind0 = (ind >> (8 * alp_index)) & 0xff;                 \
	    coeff = *((mlib_f32 *)mlib_v_tbl_256DivAlpha + ind0);   \
	    d0 = vis_fmul8x16al(vis_read_hi(dst), coeff);           \
	    d0 = vis_fand(d0, mask7FFF);                            \
	    ind1 = (ind >> (8 * (alp_index - 4))) & 0xff;           \
	    coeff = *((mlib_f32 *)mlib_v_tbl_256DivAlpha + ind1);   \
	    d1 = vis_fmul8x16al(vis_read_lo(dst), coeff);           \
	    d1 = vis_fand(d1, mask7FFF);                            \
	    dst = vis_fpack16_pair(d0, d1);                         \
	}

/* *********************************************************** */
#define	BLEND43(hh1, hh2)                                          \
	ss = ((mlib_f32 *)buffd)[i];                               \
	a1 = vis_fpsub16(done, vis_fmul8x16al(ss, aa));            \
	dd =                                                       \
	vis_fmul8x16##hh2(vis_read_hi(dd0), vis_read_##hh1(a1));   \
	dd = vis_fpadd16(dd, vis_fmul8x16au(ss, aa));              \
	((mlib_f32 *)buffd)[i] = vis_fpack16(dd);                  \
	dp += 3

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_high_4ch(
    PARAMS_NW)
{
	DECLAREVAR;
	FP_TYPE *pbuff, *xbuff, **buff_arr, *tmp_ptr, *buffd, *buffs;
	void *sp0;
	mlib_u8 *uptr;
	mlib_u8 *tsp;
	mlib_f32 *prow;
	mlib_s32 bind, y_step, k, ynew;
	mlib_s32 kh;
	mlib_s32 chan_d = ws->dchan;
	mlib_s32 chan_s = ws->nchan;
	mlib_s32 alp_ind = -(ws->alpha_shift);
	mlib_s32 row_size_max = ((max_xsize * ws->dx) >> x_shift) + m + 1;
	mlib_s32 buf_size, is_bufd, is_bufs, xSrc, cmask, row_size;
	mlib_f32 aa, fzeros = vis_fzeros();
	mlib_d64 fill = vis_to_double_dup(0xFFFFFFFF);
	mlib_d64 mask7FFF = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 mask_3 = vis_to_double(0xFFFFFFFF, 0xFFFF0000);
	mlib_d64 mask_1 = vis_to_double(0x0000FFFF, 0xFFFFFFFF);
	mlib_d64 done_3 = vis_to_double(0, 0x4000);
	mlib_d64 done_1 = vis_to_double(0x40000000, 0);

	buf_size = (n + 1) * max_xsize;

	is_bufd = 0;
	if (alp_ind || ((((mlib_addr)dstData | dstYStride) & 3) != 0)) {
		buf_size += (4 * max_xsize + 15) >> 3;
		is_bufd = 1;
	}

	is_bufs = 0;
	if (alp_ind || ((((mlib_addr)(ws->srcData) |
	    ws->srcStride) & 3) != 0)) {
		buf_size += (4 * row_size_max + 7) >> 3;
		is_bufs = 1;
	}

	xbuff =
	    MALLOC(buf_size * sizeof (FP_TYPE) + 2 * n * sizeof (FP_TYPE *));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	tmp_ptr = ybuff = xbuff + n * max_xsize;
	tmp_ptr += max_xsize;

	if (is_bufd) {
		buffd = tmp_ptr;
		tmp_ptr += (4 * max_xsize + 15) >> 3;
	}

	if (is_bufs) {
		buffs = tmp_ptr;
		tmp_ptr += (4 * row_size_max + 7) >> 3;
	}

	buff_arr = (FP_TYPE **) tmp_ptr;

	for (k = 0; k < n; k++)
		buff_arr[n + k] = buff_arr[k] = xbuff + k * max_xsize;

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	shift = table->shift_vis;
	dsat = vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));

	DIST_BITS();

/* ws -> x_shift  = x_shift; */
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	y_step = n;
	bind = 0;

	if (alp_ind) {
		if (chan_s == 3) {
			if (chan_d == 3) {
				aa = vis_to_float(((16384 << 16) | (16448)));
				alp_ind = 3;
			} else {
				aa = vis_to_float(((16320 << 16) | (0)));
			}

			uptr = (mlib_u8 *)buffs;
			if (alp_ind == -1)
				uptr++;

			for (i = 0; i < row_size_max; i++) {
				uptr[i * 4 + alp_ind] = 255;
			}

		} else {
			if (chan_d == 3) {
				aa = vis_to_float(((16513 << 16) | (16448)));
			} else {
				aa = vis_to_float(((16448 << 16) | (0)));
			}
		}

		if (alp_ind == 3)
			cmask = 1 | (1 << 4);
		else
			cmask = (1 << 3) | (1 << 7);
	} else {
		chan_d = chan_s;
	}

#pragma pipeloop(0)

	for (i = 0; i < max_xsize; i++) {
		ybuff[i] = dsat;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(chan_d);

		ws->size = size;

/* move to kernel center */
		x0 -= ws->x_move;
		xSrc = x0 >> x_shift;

		row_size = row_size_max;

		if (row_size > (ws->srcWidth - xSrc))
			row_size = (ws->srcWidth - xSrc);

		if (((mlib_addr)dul & 3) || alp_ind) {
			dl = (mlib_f32 *)buffd;
		} else {
			dl = (mlib_f32 *)dul;
		}

		ynew = yStarts[j + 1];

		if (y_step >= n) {
			y_step = n;
		} else {
			bind += y_step;

			if (bind >= n)
				bind -= n;
		}

		y = y0 - ws->y_move;
		ynew = (ynew - ws->y_move) >> y_shift;
		ySrc = y >> y_shift;

		for (l = n - y_step; l < n; l++) {

			pbuff = buff_arr[bind + l];
			tsp = (mlib_u8 *)lineAddr[ySrc + l];

			if (chan_s == 4) {

				tsp += 4 * xSrc;

				if ((mlib_addr)tsp & 3) {
					prow = (mlib_f32 *)buffs;
					mlib_ImageCopy_na((void *)tsp,
					    (void *)prow, 4 * row_size);
				} else {
					prow = (mlib_f32 *)tsp;
				}

				if (alp_ind) {

					vis_write_gsr(7 << 3);
					if (alp_ind == 3) {
#pragma pipeloop(0)
						for (i = 0;
						    i < (4 * row_size + 7) >> 3;
						    i++) {
							MUL_ALPHA_4CH(prow,
							    buffs, lo, al)
						}
					} else {
#pragma pipeloop(0)
						for (i = 0;
						    i < (4 * row_size + 7) >> 3;
						    i++) {
							MUL_ALPHA_4CH(prow,
							    buffs, hi, au)
						}
					}
					prow = (mlib_f32 *)buffs;
				}
			} else {
				tsp += 3 * xSrc;
				prow = (mlib_f32 *)buffs;
				if (alp_ind == 3) {
					mlib_v_ImageChannelInsert_U8_34L_D1(
					    (void *)tsp, (void *)prow,
					    row_size);
				} else {
					mlib_v_ImageChannelInsert_U8_34R_D1(
					    (void *)tsp, (void *)prow,
					    row_size);
				}
			}

/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {
				x = x0;
				xoff = 2 * off;
				ws->off = xoff;
				sp0 = (void *)(prow + off - xSrc);

				kw = m - off;

				if (kw > MAX_KER)
					kw = MAX_KER;

				if (off == 0) {
					if (kw == 4) {
						mlib_calc_h_4_4ch(x, filterX,
						    ws, pbuff, sp0);

					} else if (kw == 3) {
						mlib_calc_h_3_4ch(x, filterX,
						    ws, pbuff, sp0);

					} else if (kw == 2) {
						mlib_calc_h_2_4ch(x, filterX,
						    ws, pbuff, sp0);

					} else {	/* if (kw == 1) */

						mlib_calc_h_1_4ch(x, filterX,
						    ws, pbuff, sp0);
					}

				} else {

					if (kw == 4) {
						mlib_calc_h_4_off_4ch(x,
						    filterX, ws, pbuff, sp0);

					} else if (kw == 3) {
						mlib_calc_h_3_off_4ch(x,
						    filterX, ws, pbuff, sp0);

					} else if (kw == 2) {
						mlib_calc_h_2_off_4ch(x,
						    filterX, ws, pbuff, sp0);

					} else {	/* if (kw == 1) */

						mlib_calc_h_1_off_4ch(x,
						    filterX, ws, pbuff, sp0);
					}
				}
			}
		}

		vis_write_gsr((7 - shift) << 3);

		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if (kh >= MAX_KER)
				kh = MAX_KER;

			MLIB_FILTER(y, filterY, 8 * l);

			if (kh == 4) {
				if ((l + kh) == n) {
					mlib_calc_y4c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y4(fptr, buff_arr, bind + l,
					    ybuff, size);
				}

			} else if (kh == 3) {

				if ((l + kh) == n) {
					mlib_calc_y3c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y3(fptr, buff_arr, bind + l,
					    ybuff, size);
				}

			} else if (kh == 2) {

				if ((l + kh) == n) {
					mlib_calc_y2c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y2(fptr, buff_arr, bind + l,
					    ybuff, size);
				}

			} else {
/* kh == 1 */

				if ((l + kh) == n) {
					mlib_calc_y1c(fptr, buff_arr, bind + l,
					    ybuff, size, dsat, dl);
				} else {
					mlib_calc_y1(fptr, buff_arr, bind + l,
					    ybuff, size);
				}
			}
		}

		if (!alp_ind) {
			if ((mlib_u8 *)dl != dul) {
				mlib_ImageCopy_na((mlib_u8 *)dl, dul, 4 * size);
			}

		} else {
/* ZoomBlend */
			if (ws->blend != MLIB_BLEND_GTK_SRC) {
				if (chan_d == 3) {
					mlib_f32 ss;
					mlib_d64 dd, dd0, a1;
					mlib_u8 *dp = dul;
					mlib_d64 done =
					    vis_to_double_dup((16384 << 16) |
					    16384);

					vis_write_gsr((1 << 3) | 7);

					if (alp_ind == 3) {
#pragma pipeloop(0)
					    for (i = 0; i < size; i++) {
						dd0 =
						    vis_faligndata
						    (vis_ld_u8_i(dp, 2),
						    dd0);
						dd0 =
						    vis_faligndata
						    (vis_ld_u8_i(dp, 1),
						    dd0);
						dd0 =
						    vis_faligndata
						    (vis_ld_u8_i(dp, 0),
						    dd0);

						BLEND43(lo, al);
					    }
					    mlib_v_ImageChannelExtract_U8_43L_D1
						((void *)buffd, dul, size);
					} else {
#pragma pipeloop(0)
					    for (i = 0; i < size; i++) {
						dd0 =
						    vis_faligndata
						    (vis_ld_u8_i(dp, 2),
						    dd0);
						dd0 =
						    vis_faligndata
						    (vis_ld_u8_i(dp, 1),
						    dd0);
						dd0 =
						    vis_faligndata
						    (vis_ld_u8_i(dp, 0),
						    dd0);
						dd0 =
						    vis_faligndata(dd0,
						    dd0);

						BLEND43(hi, au);
					    }
					    mlib_v_ImageChannelExtract_U8_43R_D1
						((void *)buffd, dul, size);
					}
				} else {
					mlib_d64 *dp;
					mlib_d64 dd0, s1, mask, done;
					mlib_s32 cmask1, ind0, ind1;

					vis_write_gsr((1 << 3));

					if (alp_ind == 3) {
						mask = mask_3;
						done = done_3;
						dp = (mlib_d64 *)
						    vis_alignaddr(dul, 0);
						s1 = vis_ld_d64_nf(dp);

#pragma pipeloop(0)
						for (i = 0; i < (size + 1) >> 1;
						    i++) {
							BLEND44(al, lo)
						}
					} else {
						mask = mask_1;
						done = done_1;
						dp = (mlib_d64 *)
						    vis_alignaddr(dul, 0);
						s1 = vis_ld_d64_nf(dp);

#pragma pipeloop(0)
						for (i = 0; i < (size + 1) >> 1;
						    i++) {
							BLEND44(au, hi)
						}
					}

					vis_write_gsr((7 << 3));

					if (ws->blend ==
					    MLIB_BLEND_GTK_SRC_OVER) {
						if (alp_ind == 3) {
#pragma pipeloop(0)
							for (i = 0;
							    i < (size + 1) >> 1;
							    i++) {
								DIV_ALPHA(dd0,
								    ind0, ind1,
								    4)
								    vis_pst_8
								    (dd0,
								    (mlib_d64 *)
								    buffd + i,
								    ~cmask);
							}
						} else {
#pragma pipeloop(0)
							for (i = 0;
							    i < (size + 1) >> 1;
							    i++) {
								DIV_ALPHA(dd0,
								    ind0, ind1,
								    7)
								    vis_pst_8
								    (dd0,
								    (mlib_d64 *)
								    buffd + i,
								    ~cmask);
							}
						}
					} else {
						mlib_d64 s0, dd;

						if (alp_ind == 3) {
						    dp = (mlib_d64 *)
							vis_alignaddr(dul, 0);
						    s1 = vis_ld_d64_nf(dp);
#pragma pipeloop(0)
						    for (i = 0;
							i < (size + 1) >> 1;
							i++) {
							    s0 = s1;
							    s1 = vis_ld_d64_nf
								(dp + 1);
							    dd = vis_faligndata
								(s0, s1);
							    dp++;
							    DIV_ALPHA(dd0,
								ind0, ind1, 4)
							    cmask1 = ((((-ind0)
								>> 31) & ~0xf) |
								(((-ind1) >>
								31) & 0xf));
							    vis_pst_8(dd0,
								(mlib_d64 *)
								buffd + i,
								(~cmask) &
								cmask1);
							    vis_pst_8(dd,
								(mlib_d64 *)
								buffd + i,
								(~cmask) &
								(~cmask1));
						    }
						} else {
						    dp = (mlib_d64 *)
							vis_alignaddr(dul, 0);
						    s1 = vis_ld_d64_nf(dp);
#pragma pipeloop(0)
						    for (i = 0;
							i < (size + 1) >> 1;
							i++) {
							    s0 = s1;
							    s1 = vis_ld_d64_nf
								(dp + 1);
							    dd = vis_faligndata
								(s0, s1);
							    dp++;
							    DIV_ALPHA(dd0,
								ind0, ind1, 7)
							    cmask1 = ((((-ind0)
								>> 31) & ~0xf) |
								(((-ind1) >>
								31) & 0xf));
							    vis_pst_8(dd0,
								(mlib_d64 *)
								buffd + i,
								(~cmask) &
								cmask1);
							    vis_pst_8(dd,
								(mlib_d64 *)
								buffd + i,
								(~cmask) &
								(~cmask1));
						    }
						}
					}
					mlib_ImageCopy_na((mlib_u8 *)buffd, dul,
					    4 * size);
				}
			} else {
				if (chan_d == 3) {
					if (alp_ind == 3) {
					    mlib_v_ImageChannelExtract_U8_43L_D1
						((void *)buffd, dul, size);
					} else {
					    mlib_v_ImageChannelExtract_U8_43R_D1
						((void *)buffd, dul, size);
					}
				} else {
					if (chan_s == 3) {
						for (i = 0; i < (size + 1) >> 1;
						    i++) {
							vis_pst_8(fill,
							    buffd + i, cmask);
						}
					} else {
						mlib_d64 dd0;
						mlib_s32 ind0, ind1;

						vis_write_gsr((7 << 3));

						if (alp_ind == 3) {
#pragma pipeloop(0)
							for (i = 0;
							    i < (size + 1) >> 1;
							    i++) {
								DIV_ALPHA(dd0,
								    ind0, ind1,
								    4)
								    vis_pst_8
								    (dd0,
								    (mlib_d64 *)
								    buffd + i,
								    ~cmask);
							}
						} else {
#pragma pipeloop(0)
							for (i = 0;
							    i < (size + 1) >> 1;
							    i++) {
								DIV_ALPHA(dd0,
								    ind0, ind1,
								    7)
								    vis_pst_8
								    (dd0,
								    (mlib_d64 *)
								    buffd + i,
								    ~cmask);
							}
						}
					}
					mlib_ImageCopy_na((mlib_u8 *)buffd, dul,
					    4 * size);
				}
			}
		}

		y_step = ynew - ySrc;
	}

	__mlib_free(xbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomTranslateTable_4_8nw(
    PARAMS_NW)
{
	mlib_d64 zoomy;
	mlib_status status;
	mlib_s32 n, div_y;

	if ((ws->alpha_shift) ||
	    ((((mlib_addr)(ws->srcData) |
		(mlib_addr)(ws->dstData)) & 3) != 0) ||
	    (((ws->srcStride | ws->dstStride) & 3) != 0))
		return mlib_v_ImageZoomTranslateTable_high_4ch(dstData,
		    lineAddr, leftEdges, rightEdges, xStarts, yStarts, ws,
		    table);

	zoomy = ws->zoomy;
	div_y = (mlib_s32)(1 / zoomy);
	n = table->height;

	if (div_y >= n) {
		status =
		    mlib_v_ImageZoomTranslateTable_low_4ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table);
	} else {
		status =
		    mlib_v_ImageZoomTranslateTable_high_4ch(dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, table);
	}

	return (status);
}

/* *********************************************************** */
