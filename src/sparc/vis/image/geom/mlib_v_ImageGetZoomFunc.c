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

#pragma ident	"@(#)mlib_v_ImageGetZoomFunc.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate_GetFunc - returns pointer to VIS
 *                                        function if it is possible
 *                                        to use VIS
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

#define	FILTER_TYPE	mlib_d64
#define	FP_TYPE	mlib_d64

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

fun_type_nw
mlib_ImageZoomTranslate_GetFunc(
    mlib_zoom_workspace *ws,
    mlib_interp_table *table)
{
	mlib_addr psrc = (mlib_addr)(ws->srcData);

	if (table->precisionBits <= 8 && ws->type == MLIB_BYTE) {
		if (ws->nchan == 4) {

			if (mlib_ImageInitInterpTable_S16(table) !=
			    MLIB_SUCCESS)
				return (NULL);
			return (mlib_v_ImageZoomTranslateTable_4_8nw);

		} else if (ws->nchan == 1) {

			if (mlib_ImageInitInterpTable_S16(table) !=
			    MLIB_SUCCESS)
				return (NULL);
			return (mlib_v_ImageZoomTranslateTable_1_8nw);

		} else if (ws->nchan == 3) {

			if (mlib_ImageInitInterpTable_S16(table) !=
			    MLIB_SUCCESS)
				return (NULL);
			return (mlib_v_ImageZoomTranslateTable_3_8nw);

		} else if (ws->nchan == 2 && (psrc & 1) == 0 &&
		    (ws->srcStride & 1) == 0) {

			if (mlib_ImageInitInterpTable_S16(table) !=
			    MLIB_SUCCESS)
				return (NULL);
			return (mlib_v_ImageZoomTranslateTable_2_8nw);
		}
	}

	return (NULL);
}

/* *********************************************************** */

void
mlib_calc_y4c(
    VPARAMSS)
{
	FP_TYPE d0, d1, d2, d3, bb;
	FP_TYPE yfd0, yfd1, yfd2, yfd3;
	FP_TYPE *buff0, *buff1, *buff2, *buff3;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	buff3 = buff_arr[bnd + 3];
	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];
	yfd3 = fptr[3];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		d2 = buff2[i];
		d3 = buff3[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);
		d1 = FMUL_16x16(d1, yfd1);
		d2 = FMUL_16x16(d2, yfd2);
		d3 = FMUL_16x16(d3, yfd3);

		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		bb = vis_fpadd16(bb, d0);

		dl[i] = vis_fpack16(bb);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_y4(
    VPARAMS)
{
	FP_TYPE d0, d1, d2, d3, bb;
	FP_TYPE yfd0, yfd1, yfd2, yfd3;
	FP_TYPE *buff0, *buff1, *buff2, *buff3;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	buff3 = buff_arr[bnd + 3];
	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];
	yfd3 = fptr[3];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		d2 = buff2[i];
		d3 = buff3[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);
		d1 = FMUL_16x16(d1, yfd1);
		d2 = FMUL_16x16(d2, yfd2);
		d3 = FMUL_16x16(d3, yfd3);

		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_y3c(
    VPARAMSS)
{
	FP_TYPE d0, d1, d2, bb;
	FP_TYPE yfd0, yfd1, yfd2;
	FP_TYPE *buff0, *buff1, *buff2;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		d2 = buff2[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);
		d1 = FMUL_16x16(d1, yfd1);
		d2 = FMUL_16x16(d2, yfd2);

		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		bb = vis_fpadd16(bb, d0);

		dl[i] = vis_fpack16(bb);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_y3(
    VPARAMS)
{
	FP_TYPE d0, d1, d2, bb;
	FP_TYPE yfd0, yfd1, yfd2;
	FP_TYPE *buff0, *buff1, *buff2;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		d2 = buff2[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);
		d1 = FMUL_16x16(d1, yfd1);
		d2 = FMUL_16x16(d2, yfd2);

		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_y2c(
    VPARAMSS)
{
	FP_TYPE d0, d1, bb;
	FP_TYPE yfd0, yfd1;
	FP_TYPE *buff0, *buff1;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	yfd0 = fptr[0];
	yfd1 = fptr[1];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);
		d1 = FMUL_16x16(d1, yfd1);

		d0 = vis_fpadd16(d0, d1);
		bb = vis_fpadd16(bb, d0);

		dl[i] = vis_fpack16(bb);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_y2(
    VPARAMS)
{
	FP_TYPE d0, d1, bb;
	FP_TYPE yfd0, yfd1;
	FP_TYPE *buff0, *buff1;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	yfd0 = fptr[0];
	yfd1 = fptr[1];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);
		d1 = FMUL_16x16(d1, yfd1);

		d0 = vis_fpadd16(d0, d1);
		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_y1c(
    VPARAMSS)
{
	FP_TYPE d0, bb;
	FP_TYPE yfd0;
	FP_TYPE *buff0;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);

		bb = vis_fpadd16(bb, d0);

		dl[i] = vis_fpack16(bb);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_y1(
    VPARAMS)
{
	FP_TYPE d0, bb;
	FP_TYPE yfd0;
	FP_TYPE *buff0;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		bb = ybuff[i];

		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */
