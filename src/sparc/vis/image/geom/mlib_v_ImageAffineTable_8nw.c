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

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>
#include <mlib_v_ImageAffineTable_8nw.h>
#include <mlib_v_ImageChannelExtract.h>

#define	FLT_BITS	1

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

#define	CALL_PARAMS                                                       \
	dstData, lineAddr, leftEdges, rightEdges, xStarts, yStarts, ws,   \
	    table

/* *********************************************************** */

#define	CALL_PARAMS_INT                                          \
	buff, filterX + offx, filterY + offy, lineAddr + l, ws

/* *********************************************************** */

#define	DECLAREVAR                                              \
	mlib_s32 *sides = ws->sides;                            \
	mlib_s32 affine_mask = ws->affine_mask;                 \
	mlib_s32 xLeft, xRight, x0, y0;                         \
	mlib_s32 yStart = ws->yStart;                           \
	mlib_s32 yFinish = ws->yFinish;                         \
	mlib_s32 max_xsize = ws->max_xsize;                     \
	mlib_s32 dstStride = ws->dstStride;                     \
	mlib_s32 x_shift = ws->x_shift0;                        \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift, xf_mask, yf_shift, yf_mask;          \
	mlib_s32 size, size1;                                   \
	mlib_u8 *dstPixelPtr, *dstPixelPtrEnd;                  \
	mlib_s32 m = table->width;                              \
	mlib_s32 n = table->height;                             \
	mlib_s32 xf_bits = table->vis_width_bits;               \
	mlib_s32 yf_bits = table->vis_height_bits;              \
	mlib_s32 subsampleBitsH = table->subsampleBitsH;        \
	mlib_s32 subsampleBitsV = table->subsampleBitsV;        \
	mlib_d64 *filterX, *filterY;                            \
	mlib_s32 i, j, l, kw, kh, off, offx, offy;              \
	mlib_s32 buff_size, emask;                              \
	mlib_d64 *ddst, *dp;                                    \
	mlib_d64 sd0, sd1, sd2

/* *********************************************************** */

#define	DIST_BITS()                                                        \
	xf_shift = x_shift - subsampleBitsH - xf_bits - FLT_BITS;          \
	xf_mask = (((1 << subsampleBitsH) - 1) << (xf_bits + FLT_BITS));   \
	yf_shift = y_shift - subsampleBitsV - yf_bits - FLT_BITS;          \
	yf_mask = (((1 << subsampleBitsV) - 1) << (yf_bits + FLT_BITS))

/* *********************************************************** */

#define	CLIP(NCHAN)                                             \
	dstData += dstStride;                                   \
	xLeft = leftEdges[j];                                   \
	xRight = rightEdges[j];                                 \
	if (xLeft > xRight)                                     \
	    continue;                                           \
	x0 = xStarts[j];                                        \
	y0 = yStarts[j];                                        \
	if (affine_mask != 0) {                                 \
	    ws->dx = sides[j * 2 + 2];                          \
	    ws->dy = sides[j * 2 + 3];                          \
	}                                                       \
	dstPixelPtr = (mlib_u8 *)dstData + NCHAN * xLeft;       \
	size = xRight - xLeft + 1;                              \
	dstPixelPtrEnd = dstPixelPtr + NCHAN * size - 1

/* *********************************************************** */

#define	PREPARE_TO_COPY_INTERM_BUF_TO_DST()                     \
	ddst = buff_acc;                                        \
/*                                                              \
 * prepare the destination addresses                            \
 */                                                             \
	dp = (mlib_d64 *)((mlib_addr)dstPixelPtr & (~7));       \
	i = (mlib_addr)dp - (mlib_addr)dstPixelPtr;             \
	ddst = vis_alignaddr(ddst, 2 * i);                      \
/*                                                              \
 * generate edge mask for the start point                       \
 */                                                             \
	emask = vis_edge8(dstPixelPtr, dstPixelPtrEnd);         \
	sd2 = ddst[0];                                          \
	if (emask != 0xff) {                                    \
	    sd0 = sd2;                                          \
	    sd1 = ddst[1];                                      \
	    sd2 = ddst[2];                                      \
	    sd0 =                                               \
		vis_fpack16_pair(vis_faligndata(sd0, sd1),      \
		vis_faligndata(sd1, sd2));                      \
	    vis_pst_8(sd0, dp++, emask);                        \
	    ddst += 2;                                          \
	    i += 8;                                             \
	}

/* *********************************************************** */

#define	COPY_INTERM_BUF_TO_DST()                                \
	for (; i <= (size - 8); i += 8) {                       \
	    sd0 = sd2;                                          \
	    sd1 = ddst[1];                                      \
	    sd2 = ddst[2];                                      \
	    (*dp++) =                                           \
		vis_fpack16_pair(vis_faligndata(sd0, sd1),      \
		vis_faligndata(sd1, sd2));                      \
	    ddst += 2;                                          \
	}

/* *********************************************************** */

#define	COPY_TAIL()                                             \
	if (i < size) {                                         \
	    sd0 = sd2;                                          \
	    sd1 = ddst[1];                                      \
	    sd2 = ddst[2];                                      \
	    sd0 =                                               \
		vis_fpack16_pair(vis_faligndata(sd0, sd1),      \
		vis_faligndata(sd1, sd2));                      \
	    emask = vis_edge8(dp, dstPixelPtrEnd);              \
	    vis_pst_8(sd0, dp, emask);                          \
	}

/* *********************************************************** */

#define	FUNC_NAME(case)	mlib_v_ImageAffineTableLine_8nw_##case

/* *********************************************************** */

mlib_status
mlib_v_ImageAffineTable_8nw_1(
    PARAMS_NW)
{
	DECLAREVAR;
	mlib_d64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	mlib_d64 dsat;

	buff_size = (max_xsize + 3) >> 2;

	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = __mlib_malloc(2 * (buff_size + 2) * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = table->dataH_s16_1;
	filterY = table->dataV_s16_1;

	shift = table->shift_vis_affine;

	if (shift > 0) {
		dsat =
		    vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = vis_to_double_dup(0);
	}

	DIST_BITS();

	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(1);

		size1 = (size + 3) >> 2;

#pragma pipeloop(0)
		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << x_shift);

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				if (kh == 4) {
					if (kw == 4)
						FUNC_NAME(4_4_1)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(4_4_1)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(4_2_1)
						    (CALL_PARAMS_INT);
				} else if (kh == 3) {

					if (kw == 4)
						FUNC_NAME(3_4_1)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(3_4_1)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(3_2_1)
						    (CALL_PARAMS_INT);
				} else {
/* if (kh == 2) */

					if (kw == 4)
						FUNC_NAME(2_4_1)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(2_4_1)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(2_2_1)
						    (CALL_PARAMS_INT);
				}

				offx++;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    vis_fpadd16(buff_acc[i], buff[i]);
				}
			}

			offy++;
		}

		vis_write_gsr((7 - shift) << 3);

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();
	}

	if (buff != buff_local)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageAffineTable_8nw_2(
    PARAMS_NW)
{
	DECLAREVAR;
	mlib_d64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	mlib_d64 dsat;

	buff_size = (max_xsize * 2 + 3) >> 2;

	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = __mlib_malloc(2 * (buff_size + 2) * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = table->dataH_s16_1;
	filterY = table->dataV_s16_1;

	shift = table->shift_vis_affine;

	if (shift > 0) {
		dsat =
		    vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = vis_to_double_dup(0);
	}

	DIST_BITS();

	x_shift -= 1;

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(2);

		size1 = (size * 2 + 3) >> 2;

#pragma pipeloop(0)
		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << (x_shift + 1));

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				if (kh == 4) {
					if (kw == 4)
						FUNC_NAME(4_4_2)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(4_4_2)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(4_2_2)
						    (CALL_PARAMS_INT);
				} else if (kh == 3) {

					if (kw == 4)
						FUNC_NAME(3_4_2)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(3_4_2)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(3_2_2)
						    (CALL_PARAMS_INT);
				} else {
/* if (kh == 2) */

					if (kw == 4)
						FUNC_NAME(2_4_2)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(2_4_2)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(2_2_2)
						    (CALL_PARAMS_INT);
				}

				offx++;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    vis_fpadd16(buff_acc[i], buff[i]);
				}
			}

			offy++;
		}

		size *= 2;
		vis_write_gsr((7 - shift) << 3);

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();
	}

	if (buff != buff_local)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageAffineTable_8nw_3(
    PARAMS_NW)
{
	DECLAREVAR;
	mlib_d64 buff_local[BUFF_SIZE], *buff =
	    buff_local, *buff_acc, *buff_acc1;
	mlib_s32 shift;
	mlib_d64 dsat;

	buff_size = max_xsize;

	if (3 * (buff_size + 3) > BUFF_SIZE)
		buff = __mlib_malloc(3 * (buff_size + 3) * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc1 = buff + buff_size;
	buff_acc = buff_acc1 + buff_size;

	filterX = table->dataH_s16_3;
	filterY = table->dataV_s16_1;

	shift = table->shift_vis_affine;

	if (shift > 0) {
		dsat =
		    vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = vis_to_double_dup(0);
	}

	DIST_BITS();

	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(3);

		size1 = size;

#pragma pipeloop(0)
		for (i = 0; i < size1; i++) {
			buff_acc1[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << x_shift);

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				if (kh == 4) {
					if (kw == 4)
						FUNC_NAME(4_4_3)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(4_4_3)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(4_2_3)
						    (CALL_PARAMS_INT);
				} else if (kh == 3) {

					if (kw == 4)
						FUNC_NAME(3_4_3)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(3_4_3)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(3_2_3)
						    (CALL_PARAMS_INT);
				} else {
/* if (kh == 2) */

					if (kw == 4)
						FUNC_NAME(2_4_3)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(2_4_3)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(2_2_3)
						    (CALL_PARAMS_INT);
				}

				offx += 3;

				for (i = 0; i < size1; i++) {
					buff_acc1[i] =
					    vis_fpadd16(buff_acc1[i], buff[i]);
				}
			}

			offy++;
		}

		mlib_v_ImageChannelExtract_S16_43L_A8D1X4((void *)buff_acc1,
		    (void *)buff_acc, (size1 + 3));

		size *= 3;
		vis_write_gsr((7 - shift) << 3);

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();
	}

	if (buff != buff_local)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageAffineTable_8nw_4(
    PARAMS_NW)
{
	DECLAREVAR;
	mlib_d64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	mlib_d64 dsat;

	buff_size = max_xsize;

	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = __mlib_malloc(2 * (buff_size + 2) * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = table->dataH_s16_4;
	filterY = table->dataV_s16_1;

	shift = table->shift_vis_affine;

	if (shift > 0) {
		dsat =
		    vis_to_double_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = vis_to_double_dup(0);
	}

	DIST_BITS();

	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(4);

		size1 = size;

#pragma pipeloop(0)
		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << x_shift);

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				if (kh == 4) {
					if (kw == 4)
						FUNC_NAME(4_4_4)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(4_3_4)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(4_2_4)
						    (CALL_PARAMS_INT);
				} else if (kh == 3) {

					if (kw == 4)
						FUNC_NAME(3_4_4)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(3_3_4)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(3_2_4)
						    (CALL_PARAMS_INT);
				} else {
/* if (kh == 2) */

					if (kw == 4)
						FUNC_NAME(2_4_4)
						    (CALL_PARAMS_INT);
					else if (kw == 3)
						FUNC_NAME(2_3_4)
						    (CALL_PARAMS_INT);
					else if (kw == 2)
						FUNC_NAME(2_2_4)
						    (CALL_PARAMS_INT);
				}

				offx += 4;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    vis_fpadd16(buff_acc[i], buff[i]);
				}
			}

			offy++;
		}

		size *= 4;
		vis_write_gsr((7 - shift) << 3);

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();
	}

	if (buff != buff_local)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageAffineTable_8nw(
    PARAMS_NW)
{
	mlib_status res;

	switch (ws->nchan) {
	case 1:
		res = mlib_v_ImageAffineTable_8nw_1(CALL_PARAMS);
		break;
	case 2:
		res = mlib_v_ImageAffineTable_8nw_2(CALL_PARAMS);
		break;
	case 3:
		res = mlib_v_ImageAffineTable_8nw_3(CALL_PARAMS);
		break;
	case 4:
		res = mlib_v_ImageAffineTable_8nw_4(CALL_PARAMS);
		break;
	default:
		res = MLIB_FAILURE;
		break;
	}

	return (res);
}

/* *********************************************************** */
