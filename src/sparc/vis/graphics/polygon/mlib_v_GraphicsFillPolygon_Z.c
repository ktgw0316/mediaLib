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

#pragma ident	"@(#)mlib_v_GraphicsFillPolygon_Z.c	9.3	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_Z_[8 | 32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_Z_[8 | 32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Color used in the drawing.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image. Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0 - 7 are used and the bits 8 - 23 should be zero. For 32-bit
 *    frame buffer, the bits 24 - 31 are reserved for alpha channel and
 *    should be 0xff.
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 */

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_BZ_[8|32] - draw filled polygon enclose
 *                                         (x1, y1), (x2, y2) ...
 *					   (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_BZ_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c,
 *                                       mlib_s32       a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image. The Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer.
 *
 *    Pixels fall outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *    d = (s * (255 - a) + c * a) / 255
 */

#include <mlib_graphics.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_Z_32 = \
	__mlib_GraphicsFillPolygon_Z_32
#pragma weak mlib_GraphicsFillPolygon_Z_8 = \
	__mlib_GraphicsFillPolygon_Z_8

#pragma weak mlib_GraphicsFillPolygon_BZ_32 = \
	__mlib_GraphicsFillPolygon_BZ_32
#pragma weak mlib_GraphicsFillPolygon_BZ_8 = \
	__mlib_GraphicsFillPolygon_BZ_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_Z_32) mlib_GraphicsFillPolygon_Z_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_Z_32")));
__typeof__(__mlib_GraphicsFillPolygon_Z_8) mlib_GraphicsFillPolygon_Z_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_Z_8")));

__typeof__(__mlib_GraphicsFillPolygon_BZ_32) mlib_GraphicsFillPolygon_BZ_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BZ_32")));
__typeof__(__mlib_GraphicsFillPolygon_BZ_8) mlib_GraphicsFillPolygon_BZ_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BZ_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 z;
} SpanData, *SpanDataPtr;

/* *********************************************************** */

static mlib_s32 mlib_CountExtremaAndMinMax_Z(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax);

/* *********************************************************** */

static void mlib_ComputeSpansData_Z(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataPtr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans);

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_8(
    mlib_u8 *data,
    mlib_u8 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 c)
{
	mlib_d64 w_c, w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_s16 *zdata_end;
	mlib_s32 count, mask, maskz, i, maskz1;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3;

	vis_write_gsr(0);

	c &= 0xff;
	c |= (c << 8);
	c |= (c << 16);
	w_c = vis_to_double_dup(c);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	shif = 4 + ((mlib_addr)(data) & 7) - (((mlib_addr)(zdata) & 7) >> 1);

	if (shif >= 8)
		shif -= 8;

	if (shif >= 4)
		shif -= 4;

	w_tz = vis_to_double(tz, tz + sz);
	w_sz = vis_to_double_dup(sz + sz);
	w_tz1 = vis_fpadd32(w_tz, w_sz);
	w_sz = vis_fpadd32(w_sz, w_sz);

	shiftt1 = (8 - ((mlib_addr)(data) & 7));
	shiftt = shiftt1 + shif;

	if (count < shiftt)
		shiftt = count;

	w_data = vis_fpackfix_pair(w_tz, w_tz1);

	wp_data = vis_alignaddr(data, 0);
	wp_zdata = vis_alignaddr(zdata, 0);
	mask = vis_edge16(zdata, zdata_end);
	maskz = vis_fcmple16(w_data, *wp_zdata);
	mask_z = maskz;
	maskz &= mask;
	vis_pst_16(w_data, wp_zdata, maskz);
	wp_zdata++;
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	i = (mlib_s16 *)wp_zdata - zdata;

	if (i <= (shiftt - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);

		mask_z <<= 4;
		mask_z += maskz;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		wp_zdata++;
		i += 4;
	}

	if (i <= (shiftt - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);

		mask_z <<= 4;
		mask_z += maskz;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		wp_zdata++;
		i += 4;
	}

	if (i > count)
		mask_z >>= (i - count);

	shiftt1 -= count;

	if ((i == shiftt) && (i != count)) {

		mask = vis_edge8(data, data_end);
		emaskz = mask_z >> shif;
		emaskz &= mask;
		vis_pst_8(w_c, wp_data, emaskz);
		wp_data++;

		w_data1 = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
		w_tz2 = vis_fpadd32(w_tz, w_sz);
		w_tz3 = vis_fpadd32(w_tz1, w_sz);
		w_sz = vis_fpadd32(w_sz, w_sz);

#pragma unroll(1)
#pragma pipeloop(0)
		for (; i <= (count - 8); i += 8) {
			maskz = vis_fcmple16(w_data, *wp_zdata);
			maskz1 = vis_fcmple16(w_data1, *(wp_zdata + 1));

			mask_z <<= 4;
			mask_z += maskz;
			mask_z <<= 4;
			mask_z += maskz1;

			vis_pst_16(w_data, wp_zdata, maskz);
			vis_pst_16(w_data1, wp_zdata + 1, maskz1);

			emaskz = mask_z >> shif;

			vis_pst_8(w_c, wp_data, emaskz);

			wp_data++;
			wp_zdata += 2;

			w_data = vis_fpackfix_pair(w_tz, w_tz1);
			w_data1 = vis_fpackfix_pair(w_tz2, w_tz3);

			w_tz = vis_fpadd32(w_tz, w_sz);
			w_tz1 = vis_fpadd32(w_tz1, w_sz);
			w_tz2 = vis_fpadd32(w_tz2, w_sz);
			w_tz3 = vis_fpadd32(w_tz3, w_sz);
		}

		if ((count - i + shif) == 0)
			return;
		shiftt1 = 8 - shif - count + i;
	}

	if (i <= (count - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= 4;
		mask_z += maskz;
		w_data = w_data1;
		wp_zdata++;
		i += 4;
	}

	if (i < count) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		mask_z <<= (count - i);
		mask_z += (maskz >> (4 - count + i));
		mask = vis_edge16(wp_zdata, zdata_end);
		maskz &= mask;
		vis_pst_16(w_data, wp_zdata, maskz);
	}

	if (shiftt1 < 0) {
		shif = -shiftt1;
		shiftt1 += 8;
		emaskz = mask_z >> shif;

		if ((mlib_addr)wp_data < (mlib_addr)data) {
			mask = vis_edge8(data, data_end);
			emaskz &= mask;
		}

		vis_pst_8(w_c, wp_data, emaskz);
		wp_data++;
		emaskz = mask_z << shiftt1;
		mask = vis_edge8(wp_data, data_end);
		emaskz &= mask;
		vis_pst_8(w_c, wp_data, emaskz);
	} else {
		emaskz = mask_z << shiftt1;

		if ((mlib_addr)wp_data < (mlib_addr)data) {
			mask = vis_edge8(data, data_end);
		} else {
			mask = vis_edge8(wp_data, data_end);
		}

		emaskz &= mask;
		vis_pst_8(w_c, wp_data, emaskz);
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_32AA(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 c)
{
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_s32 count, mask, maskz, i, emaskz, shiftt, shif;
	mlib_s16 *zdata_end;

	vis_write_gsr(0);
	w_c = vis_to_double_dup(c);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;

	w_tz = vis_to_double(tz, tz + sz);
	w_sz = vis_to_double_dup(sz + sz);
	w_tz1 = vis_fpadd32(w_tz, w_sz);
	w_sz = vis_fpadd32(w_sz, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);

	wp_data = vis_alignaddr(data, 0);
	wp_zdata = vis_alignaddr(zdata, 0);
	mask = vis_edge16(zdata, zdata_end);
	maskz = vis_fcmple16(w_data, *wp_zdata);
	maskz &= mask;
	vis_pst_16(w_data, wp_zdata, maskz);
	wp_zdata++;
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	i = (mlib_s16 *)wp_zdata - zdata;

	mask = vis_edge32(data, data_end);

	if (i >= 3) {
		emaskz = maskz >> 2;
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			maskz &= mask;
			vis_pst_32(w_c, wp_data, maskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
		} else
			return;
	} else {
		maskz &= mask;
		vis_pst_32(w_c, wp_data, maskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
	}

#pragma unroll(1)
#pragma pipeloop(0)
	for (; i <= (count - 4); i += 4) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		emaskz = maskz >> 2;
		vis_pst_32(w_c, wp_data, emaskz);
		vis_pst_32(w_c, wp_data + 1, maskz);
		wp_data += 2;
		wp_zdata++;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
	}

	if (i < count) {
		shiftt = 2 - count + i;
		maskz = vis_fcmple16(w_data, *wp_zdata);
		mask = vis_edge16(wp_zdata, zdata_end);
		maskz &= mask;
		vis_pst_16(w_data, wp_zdata, maskz);
		maskz >>= (4 - count + i);

		if (shiftt < 0) {
			shif = -shiftt;
			shiftt += 2;
			emaskz = maskz >> shif;
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;
			emaskz = maskz << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			vis_pst_32(w_c, wp_data, emaskz);
		} else {
			emaskz = maskz << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			vis_pst_32(w_c, wp_data, emaskz);
		}
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_32GE(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 c)
{
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_s32 count, mask, maskz, mask_z, i, emaskz, shiftt, shif;
	mlib_s16 *zdata_end;
	mlib_d64 w_data1, w_tz2, w_tz3;
	mlib_s32 emaskz1, emaskz2, maskz1;

	vis_write_gsr(0);
	w_c = vis_to_double_dup(c);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;

	w_tz = vis_to_double(tz, tz + sz);
	w_sz = vis_to_double_dup(sz + sz);
	w_tz1 = vis_fpadd32(w_tz, w_sz);
	w_sz = vis_fpadd32(w_sz, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);

	wp_data = vis_alignaddr(data, 0);
	wp_zdata = vis_alignaddr(zdata, 0);
	mask = vis_edge16(zdata, zdata_end);
	maskz = vis_fcmple16(w_data, *wp_zdata);
	mask_z = maskz;
	maskz &= mask;
	vis_pst_16(w_data, wp_zdata, maskz);
	wp_zdata++;
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	i = (mlib_s16 *)wp_zdata - zdata;

	mask = vis_edge32(data, data_end);

	if (i == 4) {
		emaskz = mask_z >> 3;
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			emaskz = mask_z >> 1;
			emaskz &= mask;
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
		} else
			return;
	} else if (i != 1) {
		emaskz = mask_z >> 1;
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		wp_data++;
	}

	if (((mlib_s32 *)wp_data - data) == (count - 1)) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);
		return;
	}

	if (i <= (count - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= 4;
		mask_z += maskz;
		wp_zdata++;
		i += 4;
		emaskz1 = mask_z >> 3;
		emaskz2 = mask_z >> 1;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
		w_data1 = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
		w_tz2 = vis_fpadd32(w_tz, w_sz);
		w_tz3 = vis_fpadd32(w_tz1, w_sz);
		w_sz = vis_fpadd32(w_sz, w_sz);

#pragma unroll(1)
#pragma pipeloop(0)
		for (; i <= (count - 8); i += 8) {
			maskz = vis_fcmple16(w_data, *wp_zdata);
			vis_pst_16(w_data, wp_zdata, maskz);
			maskz1 = vis_fcmple16(w_data1, *(wp_zdata + 1));
			vis_pst_16(w_data1, wp_zdata + 1, maskz1);
			mask_z <<= 4;
			mask_z += maskz;
			emaskz = mask_z >> 3;
			vis_pst_32(w_c, wp_data, emaskz1);
			vis_pst_32(w_c, wp_data + 2, emaskz);
			emaskz = mask_z >> 1;
			vis_pst_32(w_c, wp_data + 1, emaskz2);
			vis_pst_32(w_c, wp_data + 3, emaskz);
			mask_z <<= 4;
			mask_z += maskz1;
			emaskz1 = mask_z >> 3;
			emaskz2 = mask_z >> 1;

			wp_data += 4;
			wp_zdata += 2;

			w_data = vis_fpackfix_pair(w_tz, w_tz1);
			w_tz = vis_fpadd32(w_tz, w_sz);
			w_tz1 = vis_fpadd32(w_tz1, w_sz);

			w_data1 = vis_fpackfix_pair(w_tz2, w_tz3);
			w_tz2 = vis_fpadd32(w_tz2, w_sz);
			w_tz3 = vis_fpadd32(w_tz3, w_sz);
		}

		if (i <= (count - 4)) {
			maskz = vis_fcmple16(w_data, *wp_zdata);
			vis_pst_16(w_data, wp_zdata, maskz);
			mask_z <<= 4;
			mask_z += maskz;
			vis_pst_32(w_c, wp_data, emaskz1);
			vis_pst_32(w_c, wp_data + 1, emaskz2);
			wp_data += 2;
			wp_zdata++;
			i += 4;
			emaskz1 = mask_z >> 3;
			emaskz2 = mask_z >> 1;
			w_data = w_data1;
		}

		vis_pst_32(w_c, wp_data, emaskz1);
		vis_pst_32(w_c, wp_data + 1, emaskz2);
		wp_data += 2;
	}

	if (i < count) {
		shiftt = 1 - count + i;
		maskz = vis_fcmple16(w_data, *wp_zdata);
		mask = vis_edge16(wp_zdata, zdata_end);
		maskz &= mask;
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= (count - i);
		mask_z += (maskz >> (4 - count + i));

		if (shiftt < 0) {
			shif = -shiftt;
			shiftt += 2;
			emaskz = mask_z >> shif;
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;
			emaskz = mask_z << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			vis_pst_32(w_c, wp_data, emaskz);
		} else {
			emaskz = mask_z << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			vis_pst_32(w_c, wp_data, emaskz);
		}
	} else if (i == count) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);
	}
}

/* *********************************************************** */

#define	MLIB_SHIFT	16
#define	MLIB_05	0x8000
#define	MLIB_SIGNSHIFT	31

/* *********************************************************** */

#define	MLIB_INTMIN(a, b)	(a + ((b - a) & ((b - a) >> MLIB_SIGNSHIFT)))
#define	MLIB_INTMAX(a, b)	(a - ((a - b) & ((a - b) >> MLIB_SIGNSHIFT)))

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_data = mlib_ImageGetData(buffer);                   \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	buf_stride = mlib_ImageGetStride(buffer);               \
	zbuf_data = mlib_ImageGetData(zbuffer);                 \
	zbuf_stride = mlib_ImageGetStride(zbuffer)

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_Z_8(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	mlib_u8 *startPtr;
	mlib_u8 *endPtr;
	mlib_s16 *zstartPtr;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

	c &= 0xff;
	c |= (c << 8);
	c |= (c << 16);

/* Fill scan lines */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			startPtr = buf_data + xs1;
			endPtr = buf_data + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			MLIB_FILL_ROW_Z_8(startPtr, endPtr, zstartPtr, zFixed,
			    zFixedStep, c);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_BLENDING8(d8x8, s8x8)                                     \
	d0 = vis_fpsub16(d_ca, vis_fmul8x16(vis_read_hi(s8x8), d_a1));   \
	d1 = vis_fpsub16(d_ca, vis_fmul8x16(vis_read_lo(s8x8), d_a1));   \
	d8x8 = vis_freg_pair(vis_fpack16(d0), vis_fpack16(d1));

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_8(
    mlib_u8 *data,
    mlib_u8 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_d64 d_ca,
    mlib_d64 d_a1)
{
	mlib_d64 d0, d1;
	mlib_d64 w_c, w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_s16 *zdata_end;
	mlib_s32 count, mask, maskz, i, maskz1;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3;

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	shif = 4 + ((mlib_addr)(data) & 7) - (((mlib_addr)(zdata) & 7) >> 1);

	if (shif >= 8)
		shif -= 8;

	if (shif >= 4)
		shif -= 4;

	w_tz = vis_to_double(tz, tz + sz);
	w_sz = vis_to_double_dup(sz + sz);
	w_tz1 = vis_fpadd32(w_tz, w_sz);
	w_sz = vis_fpadd32(w_sz, w_sz);

	shiftt1 = (8 - ((mlib_addr)(data) & 7));
	shiftt = shiftt1 + shif;

	if (count < shiftt)
		shiftt = count;

	w_data = vis_fpackfix_pair(w_tz, w_tz1);

	wp_data = vis_alignaddr(data, 0);
	wp_zdata = vis_alignaddr(zdata, 0);
	mask = vis_edge16(zdata, zdata_end);
	maskz = vis_fcmple16(w_data, *wp_zdata);
	mask_z = maskz;
	maskz &= mask;
	vis_pst_16(w_data, wp_zdata, maskz);
	wp_zdata++;
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	i = (mlib_s16 *)wp_zdata - zdata;

	if (i <= (shiftt - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);

		mask_z <<= 4;
		mask_z += maskz;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		wp_zdata++;
		i += 4;
	}

	if (i <= (shiftt - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);

		mask_z <<= 4;
		mask_z += maskz;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		wp_zdata++;
		i += 4;
	}

	if (i > count)
		mask_z >>= (i - count);

	shiftt1 -= count;

	if ((i == shiftt) && (i != count)) {

		mask = vis_edge8(data, data_end);
		emaskz = mask_z >> shif;
		emaskz &= mask;

		MLIB_V_BLENDING8(w_c, *wp_data);
		vis_pst_8(w_c, wp_data, emaskz);
		wp_data++;

		w_data1 = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
		w_tz2 = vis_fpadd32(w_tz, w_sz);
		w_tz3 = vis_fpadd32(w_tz1, w_sz);
		w_sz = vis_fpadd32(w_sz, w_sz);

#pragma unroll(1)
#pragma pipeloop(0)
		for (; i <= (count - 8); i += 8) {
			maskz = vis_fcmple16(w_data, *wp_zdata);
			maskz1 = vis_fcmple16(w_data1, *(wp_zdata + 1));

			mask_z <<= 4;
			mask_z += maskz;
			mask_z <<= 4;
			mask_z += maskz1;

			vis_pst_16(w_data, wp_zdata, maskz);
			vis_pst_16(w_data1, wp_zdata + 1, maskz1);

			emaskz = mask_z >> shif;
			MLIB_V_BLENDING8(w_c, *wp_data);
			vis_pst_8(w_c, wp_data, emaskz);

			wp_data++;
			wp_zdata += 2;

			w_data = vis_fpackfix_pair(w_tz, w_tz1);
			w_data1 = vis_fpackfix_pair(w_tz2, w_tz3);

			w_tz = vis_fpadd32(w_tz, w_sz);
			w_tz1 = vis_fpadd32(w_tz1, w_sz);
			w_tz2 = vis_fpadd32(w_tz2, w_sz);
			w_tz3 = vis_fpadd32(w_tz3, w_sz);
		}

		if ((count - i + shif) == 0)
			return;
		shiftt1 = 8 - shif - count + i;
	}

	if (i <= (count - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= 4;
		mask_z += maskz;
		w_data = w_data1;
		wp_zdata++;
		i += 4;
	}

	if (i < count) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		mask_z <<= (count - i);
		mask_z += (maskz >> (4 - count + i));
		mask = vis_edge16(wp_zdata, zdata_end);
		maskz &= mask;
		vis_pst_16(w_data, wp_zdata, maskz);
	}

	if (shiftt1 < 0) {
		shif = -shiftt1;
		shiftt1 += 8;
		emaskz = mask_z >> shif;

		if ((mlib_addr)wp_data < (mlib_addr)data) {
			mask = vis_edge8(data, data_end);
			emaskz &= mask;
		}

		MLIB_V_BLENDING8(w_c, *wp_data);
		vis_pst_8(w_c, wp_data, emaskz);
		wp_data++;
		emaskz = mask_z << shiftt1;
		mask = vis_edge8(wp_data, data_end);
		emaskz &= mask;
		MLIB_V_BLENDING8(w_c, *wp_data);
		vis_pst_8(w_c, wp_data, emaskz);
	} else {
		emaskz = mask_z << shiftt1;

		if ((mlib_addr)wp_data < (mlib_addr)data) {
			mask = vis_edge8(data, data_end);
		} else {
			mask = vis_edge8(wp_data, data_end);
		}

		emaskz &= mask;
		MLIB_V_BLENDING8(w_c, *wp_data);
		vis_pst_8(w_c, wp_data, emaskz);
	}
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BZ_8(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	mlib_u8 *startPtr;
	mlib_u8 *endPtr;
	mlib_s16 *zstartPtr;

	mlib_s32 ca = (mlib_s32)((128.0 / 255) * c * a + 0.5);
	mlib_s32 a1 = (mlib_s32)(32768.0 * (a * (1.0 / 255) - 1) - 0.5);

	mlib_d64 d_ca = vis_to_double_dup((ca << 16) | ca);
	mlib_d64 d_a1 = vis_to_double_dup((a1 << 16) | (a1 & 0x7FFF));

	vis_write_gsr(0 << 3);

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

/* Fill scan lines */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			startPtr = buf_data + xs1;
			endPtr = buf_data + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			MLIB_FILL_ROW_BZ_8(startPtr, endPtr, zstartPtr, zFixed,
			    zFixedStep, d_ca, d_a1);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_Z_32(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	mlib_s32 *startPtr;
	mlib_s32 *endPtr;
	mlib_s16 *zstartPtr;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

/* Fill scan lines */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			startPtr = ((mlib_s32 *)buf_data) + xs1;
			endPtr = ((mlib_s32 *)buf_data) + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			if ((((((mlib_addr)(startPtr) & 7) >> 2) +
			    (((mlib_addr)(zstartPtr) & 7) >> 1)) & 1) == 0)
				MLIB_FILL_ROW_Z_32AA(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedStep, c);
			else
				MLIB_FILL_ROW_Z_32GE(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedStep, c);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_BLENDING32(d8x8, s8x8)                                    \
	d0 = vis_fpsub16(d_ca, vis_fmul8x16(vis_read_hi(s8x8), d_a1));   \
	d1 = vis_fpsub16(d_ca, vis_fmul8x16(vis_read_lo(s8x8), d_a1));   \
	d8x8 = vis_freg_pair(vis_fpack16(d0), vis_fpack16(d1));

/*	d8x8 = vis_for(d8x8, d_aa); */

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_32AA(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_d64 d_ca,
    mlib_d64 d_a1)
{
	mlib_d64 d0, d1;
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_s32 count, mask, maskz, i, emaskz, shiftt, shif;
	mlib_s16 *zdata_end;
	mlib_d64 d_aa = vis_to_double_dup(0xff000000);

	vis_write_gsr(0);
	w_c = d_ca;

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;

	w_tz = vis_to_double(tz, tz + sz);
	w_sz = vis_to_double_dup(sz + sz);
	w_tz1 = vis_fpadd32(w_tz, w_sz);
	w_sz = vis_fpadd32(w_sz, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);

	wp_data = vis_alignaddr(data, 0);
	wp_zdata = vis_alignaddr(zdata, 0);
	mask = vis_edge16(zdata, zdata_end);
	maskz = vis_fcmple16(w_data, *wp_zdata);
	maskz &= mask;
	vis_pst_16(w_data, wp_zdata, maskz);
	wp_zdata++;
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	i = (mlib_s16 *)wp_zdata - zdata;

	mask = vis_edge32(data, data_end);

	if (i >= 3) {
		emaskz = maskz >> 2;
		emaskz &= mask;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			maskz &= mask;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, maskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
		} else
			return;
	} else {
		maskz &= mask;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, maskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
	}

#pragma unroll(1)
#pragma pipeloop(0)
	for (; i <= (count - 4); i += 4) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		emaskz = maskz >> 2;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		MLIB_V_BLENDING32(w_c, wp_data[1]);
		vis_pst_32(w_c, wp_data + 1, maskz);
		wp_data += 2;
		wp_zdata++;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
	}

	if (i < count) {
		shiftt = 2 - count + i;
		maskz = vis_fcmple16(w_data, *wp_zdata);
		mask = vis_edge16(wp_zdata, zdata_end);
		maskz &= mask;
		vis_pst_16(w_data, wp_zdata, maskz);
		maskz >>= (4 - count + i);

		if (shiftt < 0) {
			shif = -shiftt;
			shiftt += 2;
			emaskz = maskz >> shif;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;
			emaskz = maskz << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		} else {
			emaskz = maskz << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		}
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_32GE(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_d64 d_ca,
    mlib_d64 d_a1)
{
	mlib_d64 d0, d1;
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_s32 count, mask, maskz, mask_z, i, emaskz, shiftt, shif;
	mlib_s16 *zdata_end;
	mlib_d64 w_data1, w_tz2, w_tz3;
	mlib_s32 emaskz1, emaskz2, maskz1;
	mlib_d64 d_aa = vis_to_double_dup(0xff000000);

	vis_write_gsr(0);
	w_c = d_ca;

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;

	w_tz = vis_to_double(tz, tz + sz);
	w_sz = vis_to_double_dup(sz + sz);
	w_tz1 = vis_fpadd32(w_tz, w_sz);
	w_sz = vis_fpadd32(w_sz, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);

	wp_data = vis_alignaddr(data, 0);
	wp_zdata = vis_alignaddr(zdata, 0);
	mask = vis_edge16(zdata, zdata_end);
	maskz = vis_fcmple16(w_data, *wp_zdata);
	mask_z = maskz;
	maskz &= mask;
	vis_pst_16(w_data, wp_zdata, maskz);
	wp_zdata++;
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	w_data = vis_fpackfix_pair(w_tz, w_tz1);
	w_tz = vis_fpadd32(w_tz, w_sz);
	w_tz1 = vis_fpadd32(w_tz1, w_sz);
	i = (mlib_s16 *)wp_zdata - zdata;

	mask = vis_edge32(data, data_end);

	if (i == 4) {
		emaskz = mask_z >> 3;
		emaskz &= mask;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			emaskz = mask_z >> 1;
			emaskz &= mask;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
		} else
			return;
	} else if (i != 1) {
		emaskz = mask_z >> 1;
		emaskz &= mask;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		wp_data++;
	}

	if (((mlib_s32 *)wp_data - data) == (count - 1)) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		return;
	}

	if (i <= (count - 4)) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= 4;
		mask_z += maskz;
		wp_zdata++;
		i += 4;
		emaskz1 = mask_z >> 3;
		emaskz2 = mask_z >> 1;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
		w_data1 = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);
		w_tz2 = vis_fpadd32(w_tz, w_sz);
		w_tz3 = vis_fpadd32(w_tz1, w_sz);
		w_sz = vis_fpadd32(w_sz, w_sz);

#pragma unroll(1)
#pragma pipeloop(0)
		for (; i <= (count - 8); i += 8) {
			maskz = vis_fcmple16(w_data, *wp_zdata);
			vis_pst_16(w_data, wp_zdata, maskz);
			maskz1 = vis_fcmple16(w_data1, *(wp_zdata + 1));
			vis_pst_16(w_data1, wp_zdata + 1, maskz1);
			mask_z <<= 4;
			mask_z += maskz;
			emaskz = mask_z >> 3;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz1);
			MLIB_V_BLENDING32(w_c, wp_data[2]);
			vis_pst_32(w_c, wp_data + 2, emaskz);
			emaskz = mask_z >> 1;
			MLIB_V_BLENDING32(w_c, wp_data[1]);
			vis_pst_32(w_c, wp_data + 1, emaskz2);
			MLIB_V_BLENDING32(w_c, wp_data[3]);
			vis_pst_32(w_c, wp_data + 3, emaskz);
			mask_z <<= 4;
			mask_z += maskz1;
			emaskz1 = mask_z >> 3;
			emaskz2 = mask_z >> 1;

			wp_data += 4;
			wp_zdata += 2;

			w_data = vis_fpackfix_pair(w_tz, w_tz1);
			w_tz = vis_fpadd32(w_tz, w_sz);
			w_tz1 = vis_fpadd32(w_tz1, w_sz);

			w_data1 = vis_fpackfix_pair(w_tz2, w_tz3);
			w_tz2 = vis_fpadd32(w_tz2, w_sz);
			w_tz3 = vis_fpadd32(w_tz3, w_sz);
		}

		if (i <= (count - 4)) {
			maskz = vis_fcmple16(w_data, *wp_zdata);
			vis_pst_16(w_data, wp_zdata, maskz);
			mask_z <<= 4;
			mask_z += maskz;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz1);
			MLIB_V_BLENDING32(w_c, wp_data[1]);
			vis_pst_32(w_c, wp_data + 1, emaskz2);
			wp_data += 2;
			wp_zdata++;
			i += 4;
			emaskz1 = mask_z >> 3;
			emaskz2 = mask_z >> 1;
			w_data = w_data1;
		}

		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz1);
		MLIB_V_BLENDING32(w_c, wp_data[1]);
		vis_pst_32(w_c, wp_data + 1, emaskz2);
		wp_data += 2;
	}

	if (i < count) {
		shiftt = 1 - count + i;
		maskz = vis_fcmple16(w_data, *wp_zdata);
		mask = vis_edge16(wp_zdata, zdata_end);
		maskz &= mask;
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= (count - i);
		mask_z += (maskz >> (4 - count + i));

		if (shiftt < 0) {
			shif = -shiftt;
			shiftt += 2;
			emaskz = mask_z >> shif;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;
			emaskz = mask_z << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		} else {
			emaskz = mask_z << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		}
	} else if (i == count) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		MLIB_V_BLENDING32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
	}
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BZ_32(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataPtr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	mlib_s32 *startPtr;
	mlib_s32 *endPtr;
	mlib_s16 *zstartPtr;

	mlib_s32 a1 = (mlib_s32)(32768.0 * (a * (1.0 / 255) - 1) - 0.5);
	mlib_d64 d_a1 = vis_to_double((a1 & 0xFFFF),
				(a1 << 16) | (a1 & 0xFFFF));
	mlib_s32 ca1 = (mlib_s32)((128.0 / 255) * (c & 0xff) * a + 0.5);
	mlib_s32 ca2 = (mlib_s32)((128.0 / 255) * ((c >> 8) & 0xff) * a + 0.5);
	mlib_s32 ca3 = (mlib_s32)((128.0 / 255) * ((c >> 16) & 0xff) * a + 0.5);
	mlib_d64 d_ca = vis_to_double(0x7f800000 | ca3, (ca2 << 16) | ca1);

	vis_write_gsr(0 << 3);

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_Z(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values */
	pxTrans =
	    (SpanDataPtr) __mlib_malloc((intrsMax * sizeof (SpanData) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_Z(x, y, z, npoints, yMin, yMax, pxTrans, intrsMax,
	    rowSpans);

/* Fill scan lines */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
	    j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
	    intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			startPtr = ((mlib_s32 *)buf_data) + xs1;
			endPtr = ((mlib_s32 *)buf_data) + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			if ((((((mlib_addr)(startPtr) & 7) >> 2) +
			    (((mlib_addr)(zstartPtr) & 7) >> 1)) & 1) == 0)
				MLIB_FILL_ROW_BZ_32AA(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedStep, d_ca, d_a1);
			else
				MLIB_FILL_ROW_BZ_32GE(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedStep, d_ca, d_a1);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * Count vertical limits of polygon and max number of
 * intersections with scan line (number of polygon extrema)
 */

static mlib_s32
mlib_CountExtremaAndMinMax_Z(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax)
{
	mlib_s32 yMin, yMax, extrNum;
	mlib_s32 i, yCur, yPrev, dir, firstDir;

	yMin = MLIB_S32_MAX;
	yMax = MLIB_S32_MIN;

	yPrev = y[npoints - 1];
	dir = 0;
	firstDir = 0;
	extrNum = 0;

	for (i = 0; i < npoints; i++) {
		yCur = y[i];

		if (yCur > yPrev) {
			if (dir < 0)
				extrNum++;
			dir = 1;

			if (firstDir == 0)
				firstDir = 1;
		} else if (yCur < yPrev) {
			if (dir > 0)
				extrNum++;
			dir = -1;

			if (firstDir == 0)
				firstDir = -1;
		}

		if (yMin > yCur)
			yMin = yCur;

		if (yMax < yCur)
			yMax = yCur;

		yPrev = yCur;
	}

	if (firstDir != dir)
		extrNum++;

	*pyMin = yMin;
	*pyMax = yMax;

	return (extrNum);
}

/* *********************************************************** */

static void
mlib_ComputeSpansData_Z(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataPtr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans)
{
	SpanDataPtr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 z1, z2, zPrev;
	mlib_s32 zFixed, zFixedStep;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++) {
		rowPtr->x = MLIB_S32_MAX;
		rowPtr++;
	}

	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		rowSpans[i] = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];
	zPrev = z[npoints - 1];

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		z1 = zPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		z2 = zPrev = z[i];

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			z1 ^= z2 ^= z1 ^= z2;
		}

/* vertical clipping and horizontal edges */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		zFixed = (z1 << MLIB_SHIFT) + MLIB_05;
		zFixedStep = ((z2 - z1) << MLIB_SHIFT) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		zFixed += zFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].z = zFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));
			zFixed += zFixedStep;
		}
	}
}

/* *********************************************************** */
