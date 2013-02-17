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

#pragma ident	"@(#)mlib_v_GraphicsFillPolygon_GZ.c	9.3	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsFillPolygon_GZ_[8 | 32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *    mlib_GraphicsFillPolygon_BGZ_[8 | 32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_GZ_[8 | 32]
 *                                      (mlib_image *buffer,
 *                                       mlib_image *zbuffer,
 *                                       mlib_s16   *x,
 *                                       mlib_s16   *y,
 *                                       mlib_s16   *z,
 *                                       mlib_s32   npoints,
 *                                       mlib_s32   *c);
 *    mlib_status mlib_GraphicsFillPolygon_BGZ_[8 | 32]
 *                                      (mlib_image *buffer,
 *                                       mlib_image *zbuffer,
 *                                       mlib_s16   *x,
 *                                       mlib_s16   *y,
 *                                       mlib_s16   *z,
 *                                       mlib_s32   npoints,
 *                                       mlib_s32   *c,
 *                                       mlib_s32   alpha);
 *
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Pointer to array of color of the vertices.
 *    alpha        Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image. Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer. Pixels fall outside the drawable buffer
 *    will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0 - 7 are used and the bits 8 - 23 should be zero. For 32-bit
 *    frame buffer, the bits 24 - 31 are reserved for alpha channel and
 *    should be 0xff.
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 *    The shade of any point inside a polygon is determined by linear
 *    interpolation of vertex intensities along each edge and then between
 *    edges along each scanline.
 */

#include <mlib_graphics.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_GZ_32 = \
	__mlib_GraphicsFillPolygon_GZ_32
#pragma weak mlib_GraphicsFillPolygon_GZ_8 = __mlib_GraphicsFillPolygon_GZ_8
#pragma weak mlib_GraphicsFillPolygon_BGZ_32 = \
	__mlib_GraphicsFillPolygon_BGZ_32
#pragma weak mlib_GraphicsFillPolygon_BGZ_8 = __mlib_GraphicsFillPolygon_BGZ_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_GZ_32) mlib_GraphicsFillPolygon_GZ_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_GZ_32")));
__typeof__(__mlib_GraphicsFillPolygon_GZ_8) mlib_GraphicsFillPolygon_GZ_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_GZ_8")));
__typeof__(__mlib_GraphicsFillPolygon_BGZ_32) mlib_GraphicsFillPolygon_BGZ_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BGZ_32")));
__typeof__(__mlib_GraphicsFillPolygon_BGZ_8) mlib_GraphicsFillPolygon_BGZ_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_BGZ_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 z;
	mlib_s32 c;
} SpanDataG8, *SpanDataG8Ptr;

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 z;
	mlib_s32 r;
	mlib_s32 g;
	mlib_s32 b;
} SpanDataG32, *SpanDataG32Ptr;

/* *********************************************************** */

#define	MLIB_GZ_FPACK(dst, src1, src2, src3, src4)              \
	(dst) = vis_fpack32((dst), (src1));                     \
	(dst) = vis_fpack32((dst), (src2));                     \
	(dst) = vis_fpack32((dst), (src3));                     \
	(dst) = vis_fpack32((dst), (src4))

/* *********************************************************** */

#define	MLIB_GZ32_FPADD(sl1_1, sl1_2, sl2_1, sl2_2, sl3_1, sl3_2)   \
	(sl1_1) = vis_fpadd32((sl1_1), (sl1_2));                    \
	(sl2_1) = vis_fpadd32((sl2_1), (sl2_2));                    \
	(sl3_1) = vis_fpadd32((sl3_1), (sl3_2))

/* *********************************************************** */

#define	MLIB_GZ8_FPADD(sl1, sl2, sl3, sl4, sl)                  \
	(sl1) = vis_fpadd32((sl1), (sl));                       \
	(sl2) = vis_fpadd32((sl2), (sl));                       \
	(sl3) = vis_fpadd32((sl3), (sl));                       \
	(sl4) = vis_fpadd32((sl4), (sl))

/* *********************************************************** */

#define	MLIB_GZ_32_FPACK(a, b, c)	{ c = vis_fpack16_pair((a), (b)); }

/* *********************************************************** */

#define	MLIB_GZ_32_FPADD(a, b, c)                               \
	a = vis_fpadd16(a, c);                                  \
	b = vis_fpadd16(b, c)

/* *********************************************************** */

#define	MLIB_GZ_32_FPSUB(a, b, c)                               \
	a = vis_fpsub16(a, c);                                  \
	b = vis_fpsub16(b, c)

/* *********************************************************** */

/*
 * WARNING! Color in GZ mode computing fix point (9.23)
 *
 * if ((((((mlib_s32)(data) & 7) >> 2) +
 *    (((mlib_s32)(zdata) & 7) >> 1)) & 1) == 0) {
 *
 *   MLIB_FILL_ROW_GZ_32AA
 *   (data, ptrR, zptrL, tzL, tzR, sz, d_cp_L, d_cp_R, d_sc);
 *
 * }
 * else {
 *
 *   MLIB_FILL_ROW_GZ_32GE
 *   (data, ptrR, zptrL, tzL, tzR, sz, d_cp_L, d_cp_R, d_sc);
 *
 * }
 */

/* *********************************************************** */

static void
MLIB_FILL_ROW_GZ_8(
    mlib_u8 *data,
    mlib_u8 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 tc,
    mlib_s32 sc)
{
	mlib_d64 w_c, w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_s16 *zdata_end;
	mlib_s32 count, mask, maskz, i, maskz1, sc4;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3;

	vis_write_gsr(0);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	tc -= ((mlib_addr)(data) & 7) * sc;
	shif = 4 + ((mlib_addr)(data) & 7) - (((mlib_addr)(zdata) & 7) >> 1);

	if (shif >= 8)
		shif -= 8;

	if (shif >= 4)
		shif -= 4;
	sc4 = sc << 2;

	w_tc_0 = vis_to_double(tc, tc + sc4);
	tc += sc;
	w_tc_1 = vis_to_double(tc, tc + sc4);
	tc += sc;
	w_tc_2 = vis_to_double(tc, tc + sc4);
	tc += sc;
	w_tc_3 = vis_to_double(tc, tc + sc4);
	w_sc = vis_to_double_dup(sc4 + sc4);

	MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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

		MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
		MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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

			MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
			MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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
		MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
		MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);
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

#define	BLEND_WORD                             	  \
    ld = *wp_data;                                \
    m00 = vis_fmul8x16(vis_read_hi(ld), donema);  \
    m01 = vis_fmul8x16(vis_read_hi(w_c), da);     \
    m10 = vis_fmul8x16(vis_read_lo(ld), donema);  \
    m11 = vis_fmul8x16(vis_read_lo(w_c), da);     \
    sum0 = vis_fpadd16(m00, m01);                 \
    sum1 = vis_fpadd16(m10, m11);                 \
    st = vis_fpack16_pair(sum0, sum1);            \
    vis_pst_8(st, wp_data, emaskz);

/* *********************************************************** */

static void
MLIB_FILL_ROW_BGZ_8(
    mlib_u8 *data,
    mlib_u8 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 tc,
    mlib_s32 sc,
    mlib_s32 alpha)
{
	mlib_d64 w_c, w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_s16 *zdata_end;
	mlib_s32 count, mask, maskz, i, maskz1, sc4;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3;
	mlib_s32 sa, sonema;
	mlib_f32 fa, fonema;
	mlib_d64 donema, m00, m01, m10, m11, sum0, sum1, ld, st, da;

	sa = alpha & 0xFF;
	sonema = ~alpha & 0xFF;
	da =  vis_to_double_dup((sa << 23) | (sa << 7));
	/* 0xFF - alpha */
	donema = vis_to_double_dup((sonema << 23) | (sonema << 7));

	vis_write_gsr(0);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	tz -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	tc -= ((mlib_addr)(data) & 7) * sc;
	shif = 4 + ((mlib_addr)(data) & 7) - (((mlib_addr)(zdata) & 7) >> 1);

	if (shif >= 8)
		shif -= 8;

	if (shif >= 4)
		shif -= 4;
	sc4 = sc << 2;

	w_tc_0 = vis_to_double(tc, tc + sc4);
	tc += sc;
	w_tc_1 = vis_to_double(tc, tc + sc4);
	tc += sc;
	w_tc_2 = vis_to_double(tc, tc + sc4);
	tc += sc;
	w_tc_3 = vis_to_double(tc, tc + sc4);
	w_sc = vis_to_double_dup(sc4 + sc4);

	MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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
		BLEND_WORD
		wp_data++;

		MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
		MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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

			BLEND_WORD
			MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
			MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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
		MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
		MLIB_GZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);
		wp_data++;
		emaskz = mask_z << shiftt1;
		mask = vis_edge8(wp_data, data_end);
		emaskz &= mask;
		BLEND_WORD
	} else {
		emaskz = mask_z << shiftt1;

		if ((mlib_addr)wp_data < (mlib_addr)data) {
			mask = vis_edge8(data, data_end);
		} else {
			mask = vis_edge8(wp_data, data_end);
		}

		emaskz &= mask;
		BLEND_WORD
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_GZ_32AA(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz_L,
    mlib_s32 tz_R,
    mlib_s32 sz,
    mlib_d64 d_cp_L,
    mlib_d64 d_cp_R,
    mlib_d64 d_sc)
{
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_d64 w_c_end, w_tz2, w_tz3, w_data_end, *wp_data_end, *wp_zdata_end;
	mlib_s32 count, mask, maskz, i, emaskz, maskz_end, emaskz_end;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3;
	mlib_d64 w_sc;
	mlib_s16 *zdata_end;

	vis_write_gsr(0);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz_L -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	w_sc = vis_fpadd16(d_sc, d_sc);

	if (((mlib_addr)(data) & 7) != 0) {
		w_tc_0 = vis_fpsub16(d_cp_L, d_sc);
		w_tc_1 = d_cp_L;
	} else {
		w_tc_0 = d_cp_L;
		w_tc_1 = vis_fpadd16(d_cp_L, d_sc);
	}

	MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
	MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

	w_tz = vis_to_double(tz_L, tz_L + sz);
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
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			maskz &= mask;
			vis_pst_32(w_c, wp_data, maskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
			MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
			MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		} else
			return;
	} else {
		maskz &= mask;
		vis_pst_32(w_c, wp_data, maskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
	}

	if (((mlib_addr)(data_end) & 7) != 0) {
		w_tc_2 = vis_fpsub16(d_cp_R, d_sc);
		w_tc_3 = d_cp_R;
	} else {
		w_tc_2 = d_cp_R;
		w_tc_3 = vis_fpadd16(d_cp_R, d_sc);
	}

	tz_R -= (((mlib_addr)(zdata_end) & 7) >> 1) * sz;
	w_tz2 = vis_to_double(tz_R, tz_R + sz);
	tz_R += (sz << 1);
	w_tz3 = vis_to_double(tz_R, tz_R + sz);
	MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
	MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);

	wp_data_end = vis_alignaddr(data_end, 0);
	wp_zdata_end = vis_alignaddr(zdata_end, 0);
	mask = vis_edge16(wp_zdata_end, zdata_end);
	maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);
	maskz_end &= mask;
	vis_pst_16(w_data_end, wp_zdata_end, maskz_end);
	i = zdata_end - (mlib_s16 *)wp_zdata_end + 1;
	mask = vis_edge32(wp_data_end, data_end);

	if (i >= 3) {
		emaskz = maskz_end;
		emaskz &= mask;
		vis_pst_32(w_c_end, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
		emaskz = maskz_end >> 2;
		vis_pst_32(w_c_end, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	} else {
		emaskz = maskz_end >> 2;
		emaskz &= mask;
		vis_pst_32(w_c_end, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	}

	wp_zdata_end--;
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	count = wp_zdata_end - wp_zdata + 1;

#pragma unroll(1)
#pragma pipeloop(0)
	for (i = 0; i < (count >> 1); i++) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);

		vis_pst_16(w_data, wp_zdata, maskz);
		vis_pst_16(w_data_end, wp_zdata_end, maskz_end);

		emaskz = maskz >> 2;
		emaskz_end = maskz_end >> 2;

		vis_pst_32(w_c, wp_data, emaskz);
		vis_pst_32(w_c_end, wp_data_end, maskz_end);

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		vis_pst_32(w_c, wp_data + 1, maskz);
		vis_pst_32(w_c_end, wp_data_end - 1, emaskz_end);

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		wp_data += 2;
		wp_zdata++;
		wp_data_end -= 2;
		wp_zdata_end--;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
		w_tz2 = vis_fpsub32(w_tz2, w_sz);
		w_tz3 = vis_fpsub32(w_tz3, w_sz);
	}

	if ((count & 1) != 0) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		emaskz = maskz >> 2;
		vis_pst_32(w_c, wp_data, emaskz);
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		vis_pst_32(w_c, wp_data + 1, maskz);
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_GZ_32GE(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz_L,
    mlib_s32 tz_R,
    mlib_s32 sz,
    mlib_d64 d_cp_L,
    mlib_d64 d_cp_R,
    mlib_d64 d_sc)
{
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_d64 w_c_end, w_tz2, w_tz3, w_data_end, *wp_data_end, *wp_zdata_end;
	mlib_s32 count, mask, maskz, i, emaskz, maskz_end, mask_z, mask_z_end;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3;
	mlib_d64 w_sc;
	mlib_s16 *zdata_end;

	vis_write_gsr(0);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz_L -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	w_sc = vis_fpadd16(d_sc, d_sc);

	if (((mlib_addr)(data) & 7) != 0) {
		w_tc_0 = vis_fpsub16(d_cp_L, d_sc);
		w_tc_1 = d_cp_L;
	} else {
		w_tc_0 = d_cp_L;
		w_tc_1 = vis_fpadd16(d_cp_L, d_sc);
	}

	MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
	MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

	w_tz = vis_to_double(tz_L, tz_L + sz);
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
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			emaskz = mask_z >> 1;
			emaskz &= mask;
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
			MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
			MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		} else
			return;
	} else if (i != 1) {
		emaskz = mask_z >> 1;
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
	}

	if (((mlib_s32 *)wp_data - data) == (count - 1)) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		vis_pst_32(w_c, wp_data, emaskz);
		return;
	}

	if (((mlib_addr)(data_end) & 7) != 0) {
		w_tc_2 = vis_fpsub16(d_cp_R, d_sc);
		w_tc_3 = d_cp_R;
	} else {
		w_tc_2 = d_cp_R;
		w_tc_3 = vis_fpadd16(d_cp_R, d_sc);
	}

	tz_R -= (((mlib_addr)(zdata_end) & 7) >> 1) * sz;
	w_tz2 = vis_to_double(tz_R, tz_R + sz);
	tz_R += (sz << 1);
	w_tz3 = vis_to_double(tz_R, tz_R + sz);
	MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
	MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);

	wp_data_end = vis_alignaddr(data_end, 0);
	wp_zdata_end = vis_alignaddr(zdata_end, 0);
	mask = vis_edge16(wp_zdata_end, zdata_end);
	maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);
	mask_z_end = maskz_end;
	maskz_end &= mask;
	vis_pst_16(w_data_end, wp_zdata_end, maskz_end);
	i = zdata_end - (mlib_s16 *)wp_zdata_end + 1;
	mask = vis_edge32(wp_data_end, data_end);

	if (i == 4) {
		mask_z_end <<= 1;
		emaskz = mask_z_end;
		emaskz &= mask;
		vis_pst_32(w_c_end, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
		mask_z_end >>= 2;
		emaskz = mask_z_end;
		mask_z_end >>= 2;
		vis_pst_32(w_c_end, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	} else if (i != 1) {
		mask_z_end >>= 1;
		emaskz = mask_z_end;
		emaskz &= mask;
		vis_pst_32(w_c_end, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
		mask_z_end >>= 2;
	} else
		mask_z_end >>= 3;

	wp_zdata_end--;
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	count = wp_zdata_end - wp_zdata + 1;

#pragma unroll(1)
#pragma pipeloop(0)
	for (i = 0; i < (count >> 1); i++) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);

		vis_pst_16(w_data, wp_zdata, maskz);
		vis_pst_16(w_data_end, wp_zdata_end, maskz_end);

		mask_z <<= 4;
		mask_z += maskz;
		emaskz = mask_z >> 3;

		mask_z_end = (maskz_end << 1) + mask_z_end;

		vis_pst_32(w_c, wp_data, emaskz);
		vis_pst_32(w_c_end, wp_data_end, mask_z_end);

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		emaskz = mask_z >> 1;
		mask_z_end >>= 2;

		vis_pst_32(w_c, wp_data + 1, emaskz);
		vis_pst_32(w_c_end, wp_data_end - 1, mask_z_end);

		mask_z_end >>= 2;

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		wp_data += 2;
		wp_zdata++;
		wp_data_end -= 2;
		wp_zdata_end--;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
		w_tz2 = vis_fpsub32(w_tz2, w_sz);
		w_tz3 = vis_fpsub32(w_tz3, w_sz);
	}

	if ((count & 1) != 0) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= 4;
		mask_z += maskz;
		emaskz = mask_z >> 3;
		vis_pst_32(w_c, wp_data, emaskz);
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		emaskz = mask_z >> 1;
		vis_pst_32(w_c, wp_data + 1, emaskz);
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		wp_data += 2;
	}

	emaskz = (mask_z << 1) + mask_z_end;
	vis_pst_32(w_c, wp_data, emaskz);
}

/* *********************************************************** */

#define	MLIB_PUT_BGZ_32(w_col, data)                  \
	ld = *(data);                                 \
	m00 = vis_fmul8x16(vis_read_hi(ld), donema);  \
	m01 = vis_fmul8x16(vis_read_hi(w_col), da);   \
	m10 = vis_fmul8x16(vis_read_lo(ld), donema);  \
	m11 = vis_fmul8x16(vis_read_lo(w_col), da);   \
	sum0 = vis_fpadd16(m00, m01);                 \
	sum1 = vis_fpadd16(m10, m11);                 \
	st = vis_fpack16_pair(sum0, sum1);            \
	st = vis_for(st, ff_alpha);

/* *********************************************************** */

static void
MLIB_FILL_ROW_BGZ_32AA(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz_L,
    mlib_s32 tz_R,
    mlib_s32 sz,
    mlib_d64 d_cp_L,
    mlib_d64 d_cp_R,
    mlib_d64 d_sc,
    mlib_s32 alpha)
{
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_d64 w_c_end, w_tz2, w_tz3, w_data_end, *wp_data_end, *wp_zdata_end;
	mlib_s32 count, mask, maskz, i, emaskz, maskz_end, emaskz_end;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3;
	mlib_d64 w_sc;
	mlib_s16 *zdata_end;
	mlib_d64 donema, m00, m01, m10, m11, sum0, sum1, ld, st, da;
	mlib_d64 ff_alpha = vis_to_double_dup(0xFF000000);
	mlib_s32 sa, sonema;

	sa = alpha & 0xFF;
	sonema = ~alpha & 0xFF;
	da =  vis_to_double_dup((sa << 23) | (sa << 7));
	/* 0xFF - alpha */
	donema = vis_to_double_dup((sonema << 23) | (sonema << 7));

	vis_write_gsr(0);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz_L -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	w_sc = vis_fpadd16(d_sc, d_sc);

	if (((mlib_addr)(data) & 7) != 0) {
		w_tc_0 = vis_fpsub16(d_cp_L, d_sc);
		w_tc_1 = d_cp_L;
	} else {
		w_tc_0 = d_cp_L;
		w_tc_1 = vis_fpadd16(d_cp_L, d_sc);
	}

	MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
	MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

	w_tz = vis_to_double(tz_L, tz_L + sz);
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
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		wp_data++;
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			maskz &= mask;
			MLIB_PUT_BGZ_32(w_c, wp_data)
			vis_pst_32(st, wp_data, maskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
			MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
			MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		} else
			return;
	} else {
		maskz &= mask;
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, maskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
	}

	if (((mlib_addr)(data_end) & 7) != 0) {
		w_tc_2 = vis_fpsub16(d_cp_R, d_sc);
		w_tc_3 = d_cp_R;
	} else {
		w_tc_2 = d_cp_R;
		w_tc_3 = vis_fpadd16(d_cp_R, d_sc);
	}

	tz_R -= (((mlib_addr)(zdata_end) & 7) >> 1) * sz;
	w_tz2 = vis_to_double(tz_R, tz_R + sz);
	tz_R += (sz << 1);
	w_tz3 = vis_to_double(tz_R, tz_R + sz);
	MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
	MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);

	wp_data_end = vis_alignaddr(data_end, 0);
	wp_zdata_end = vis_alignaddr(zdata_end, 0);
	mask = vis_edge16(wp_zdata_end, zdata_end);
	maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);
	maskz_end &= mask;
	vis_pst_16(w_data_end, wp_zdata_end, maskz_end);
	i = zdata_end - (mlib_s16 *)wp_zdata_end + 1;
	mask = vis_edge32(wp_data_end, data_end);

	if (i >= 3) {
		emaskz = maskz_end;
		emaskz &= mask;
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
		emaskz = maskz_end >> 2;
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	} else {
		emaskz = maskz_end >> 2;
		emaskz &= mask;
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	}

	wp_zdata_end--;
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	count = wp_zdata_end - wp_zdata + 1;

#pragma unroll(1)
#pragma pipeloop(0)
	for (i = 0; i < (count >> 1); i++) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);

		vis_pst_16(w_data, wp_zdata, maskz);
		vis_pst_16(w_data_end, wp_zdata_end, maskz_end);

		emaskz = maskz >> 2;
		emaskz_end = maskz_end >> 2;

		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, maskz_end);

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		MLIB_PUT_BGZ_32(w_c, wp_data + 1)
		vis_pst_32(st, wp_data + 1, maskz);
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end - 1)
		vis_pst_32(st, wp_data_end - 1, emaskz_end);

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		wp_data += 2;
		wp_zdata++;
		wp_data_end -= 2;
		wp_zdata_end--;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
		w_tz2 = vis_fpsub32(w_tz2, w_sz);
		w_tz3 = vis_fpsub32(w_tz3, w_sz);
	}

	if ((count & 1) != 0) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		emaskz = maskz >> 2;
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_PUT_BGZ_32(w_c, wp_data + 1)
		vis_pst_32(st, wp_data + 1, maskz);
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BGZ_32GE(
    mlib_s32 *data,
    mlib_s32 *data_end,
    mlib_s16 *zdata,
    mlib_s32 tz_L,
    mlib_s32 tz_R,
    mlib_s32 sz,
    mlib_d64 d_cp_L,
    mlib_d64 d_cp_R,
    mlib_d64 d_sc,
    mlib_s32 alpha)
{
	mlib_d64 w_c, w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_d64 w_c_end, w_tz2, w_tz3, w_data_end, *wp_data_end, *wp_zdata_end;
	mlib_s32 count, mask, maskz, i, emaskz, maskz_end, mask_z, mask_z_end;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3;
	mlib_d64 w_sc;
	mlib_s16 *zdata_end;
	mlib_d64 donema, m00, m01, m10, m11, sum0, sum1, ld, st, da;
	mlib_d64 ff_alpha = vis_to_double_dup(0xFF000000);
	mlib_s32 sa, sonema;

	sa = alpha & 0xFF;
	sonema = ~alpha & 0xFF;
	da =  vis_to_double_dup((sa << 23) | (sa << 7));
	/* 0xFF - alpha */
	donema = vis_to_double_dup((sonema << 23) | (sonema << 7));

	vis_write_gsr(0);

	count = data_end - data + 1;
	zdata_end = zdata + (count - 1);

	w_sz = vis_to_double_dup(sz + sz);
	tz_L -= (((mlib_addr)(zdata) & 7) >> 1) * sz;
	w_sc = vis_fpadd16(d_sc, d_sc);

	if (((mlib_addr)(data) & 7) != 0) {
		w_tc_0 = vis_fpsub16(d_cp_L, d_sc);
		w_tc_1 = d_cp_L;
	} else {
		w_tc_0 = d_cp_L;
		w_tc_1 = vis_fpadd16(d_cp_L, d_sc);
	}

	MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
	MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

	w_tz = vis_to_double(tz_L, tz_L + sz);
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
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		wp_data++;
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			emaskz = mask_z >> 1;
			emaskz &= mask;
			MLIB_PUT_BGZ_32(w_c, wp_data)
			vis_pst_32(st, wp_data, emaskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
			MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
			MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		} else
			return;
	} else if (i != 1) {
		emaskz = mask_z >> 1;
		emaskz &= mask;
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
	}

	if (((mlib_s32 *)wp_data - data) == (count - 1)) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		return;
	}

	if (((mlib_addr)(data_end) & 7) != 0) {
		w_tc_2 = vis_fpsub16(d_cp_R, d_sc);
		w_tc_3 = d_cp_R;
	} else {
		w_tc_2 = d_cp_R;
		w_tc_3 = vis_fpadd16(d_cp_R, d_sc);
	}

	tz_R -= (((mlib_addr)(zdata_end) & 7) >> 1) * sz;
	w_tz2 = vis_to_double(tz_R, tz_R + sz);
	tz_R += (sz << 1);
	w_tz3 = vis_to_double(tz_R, tz_R + sz);
	MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
	MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);

	wp_data_end = vis_alignaddr(data_end, 0);
	wp_zdata_end = vis_alignaddr(zdata_end, 0);
	mask = vis_edge16(wp_zdata_end, zdata_end);
	maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);
	mask_z_end = maskz_end;
	maskz_end &= mask;
	vis_pst_16(w_data_end, wp_zdata_end, maskz_end);
	i = zdata_end - (mlib_s16 *)wp_zdata_end + 1;
	mask = vis_edge32(wp_data_end, data_end);

	if (i == 4) {
		mask_z_end <<= 1;
		emaskz = mask_z_end;
		emaskz &= mask;
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
		mask_z_end >>= 2;
		emaskz = mask_z_end;
		mask_z_end >>= 2;
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
	} else if (i != 1) {
		mask_z_end >>= 1;
		emaskz = mask_z_end;
		emaskz &= mask;
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, emaskz);
		wp_data_end--;
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);
		mask_z_end >>= 2;
	} else
		mask_z_end >>= 3;

	wp_zdata_end--;
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
	w_tz2 = vis_fpsub32(w_tz2, w_sz);
	w_tz3 = vis_fpsub32(w_tz3, w_sz);
	count = wp_zdata_end - wp_zdata + 1;

#pragma unroll(1)
#pragma pipeloop(0)
	for (i = 0; i < (count >> 1); i++) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		maskz_end = vis_fcmple16(w_data_end, *wp_zdata_end);

		vis_pst_16(w_data, wp_zdata, maskz);
		vis_pst_16(w_data_end, wp_zdata_end, maskz_end);

		mask_z <<= 4;
		mask_z += maskz;
		emaskz = mask_z >> 3;

		mask_z_end = (maskz_end << 1) + mask_z_end;

		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end)
		vis_pst_32(st, wp_data_end, mask_z_end);

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		emaskz = mask_z >> 1;
		mask_z_end >>= 2;

		MLIB_PUT_BGZ_32(w_c, wp_data + 1)
		vis_pst_32(st, wp_data + 1, emaskz);
		MLIB_PUT_BGZ_32(w_c_end, wp_data_end - 1)
		vis_pst_32(st, wp_data_end - 1, mask_z_end);

		mask_z_end >>= 2;

		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		MLIB_GZ_32_FPACK(w_tc_2, w_tc_3, w_c_end);
		MLIB_GZ_32_FPSUB(w_tc_2, w_tc_3, w_sc);

		wp_data += 2;
		wp_zdata++;
		wp_data_end -= 2;
		wp_zdata_end--;

		w_data = vis_fpackfix_pair(w_tz, w_tz1);
		w_tz = vis_fpadd32(w_tz, w_sz);
		w_tz1 = vis_fpadd32(w_tz1, w_sz);

		w_data_end = vis_fpackfix_pair(w_tz2, w_tz3);
		w_tz2 = vis_fpsub32(w_tz2, w_sz);
		w_tz3 = vis_fpsub32(w_tz3, w_sz);
	}

	if ((count & 1) != 0) {
		maskz = vis_fcmple16(w_data, *wp_zdata);
		vis_pst_16(w_data, wp_zdata, maskz);
		mask_z <<= 4;
		mask_z += maskz;
		emaskz = mask_z >> 3;
		MLIB_PUT_BGZ_32(w_c, wp_data)
		vis_pst_32(st, wp_data, emaskz);
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		emaskz = mask_z >> 1;
		MLIB_PUT_BGZ_32(w_c, wp_data + 1)
		vis_pst_32(st, wp_data + 1, emaskz);
		MLIB_GZ_32_FPACK(w_tc_0, w_tc_1, w_c);
		MLIB_GZ_32_FPADD(w_tc_0, w_tc_1, w_sc);
		wp_data += 2;
	}

	emaskz = (mask_z << 1) + mask_z_end;
	MLIB_PUT_BGZ_32(w_c, wp_data)
	vis_pst_32(st, wp_data, emaskz);
}

/* *********************************************************** */

#define	MLIB_SHIFT	16
#define	MLIB_05	0x8000
#define	MLIB_SIGNSHIFT	31
#define	COLOR_MASK	0xFF
#define	MLIB_INTMIN(a, b)	(a + ((b - a) & ((b - a) >> MLIB_SIGNSHIFT)))
#define	MLIB_INTMAX(a, b)	(a - ((a - b) & ((a - b) >> MLIB_SIGNSHIFT)))

/* *********************************************************** */

static mlib_s32 mlib_CountExtremaAndMinMax_GZ(
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 *pyMin,
    mlib_s32 *pyMax);

static void mlib_ComputeSpansData_GZ8(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG8Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans);

static void mlib_ComputeSpansData_GZ32(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG32Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans);

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_data = mlib_ImageGetData(buffer);                   \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	buf_stride = mlib_ImageGetStride(buffer);               \
	zbuf_data = mlib_ImageGetData(zbuffer);                 \
	zbuf_stride = mlib_ImageGetStride(zbuffer)

mlib_status
__mlib_GraphicsFillPolygon_GZ_8(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG8Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 cFixed, cFixedStep;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	mlib_u8 *startPtr;
	mlib_u8 *endPtr;
	mlib_s16 *zstartPtr;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG8Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG8) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ8(x, y, z, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

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

			cFixed = rowPtr[i].c;
			cFixedStep =
			    (rowPtr[i + 1].c - rowPtr[i].c) / (xs2 - xs1 + 1);

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				cFixed += (cFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			startPtr = buf_data + xs1;
			endPtr = buf_data + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			MLIB_FILL_ROW_GZ_8(startPtr, endPtr, zstartPtr, zFixed,
			    zFixedStep, cFixed, cFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_GZ_32(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 rFixed, rFixedStep, rFixedEnd;
	mlib_s32 gFixed, gFixedStep, gFixedEnd;
	mlib_s32 bFixed, bFixedStep, bFixedEnd;
	mlib_s32 zFixed, zFixedStep, zFixedEnd;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG32Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	mlib_s32 *startPtr;
	mlib_s32 *endPtr;
	mlib_s16 *zstartPtr;
	mlib_d64 d_cL, d_cR, d_cS;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG32Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG32) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ32(x, y, z, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

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

			rFixed = rowPtr[i].r;
			rFixedStep =
			    (rowPtr[i + 1].r - rowPtr[i].r) / (xs2 - xs1 + 1);
			gFixed = rowPtr[i].g;
			gFixedStep =
			    (rowPtr[i + 1].g - rowPtr[i].g) / (xs2 - xs1 + 1);
			bFixed = rowPtr[i].b;
			bFixedStep =
			    (rowPtr[i + 1].b - rowPtr[i].b) / (xs2 - xs1 + 1);
			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				rFixed += (rFixedStep * (-xs1));
				gFixed += (gFixedStep * (-xs1));
				bFixed += (bFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			rFixedEnd = rFixed + (xs2 - xs1) * rFixedStep;
			gFixedEnd = gFixed + (xs2 - xs1) * gFixedStep;
			bFixedEnd = bFixed + (xs2 - xs1) * bFixedStep;
			zFixedEnd = zFixed + (xs2 - xs1) * zFixedStep;

			startPtr = ((mlib_s32 *)buf_data) + xs1;
			endPtr = ((mlib_s32 *)buf_data) + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			((mlib_s16 *)&d_cL)[0] = 0x7f80;
			((mlib_s16 *)&d_cL)[1] = (bFixed >> 9);
			((mlib_s16 *)&d_cL)[2] = (gFixed >> 9);
			((mlib_s16 *)&d_cL)[3] = (rFixed >> 9);

			((mlib_s16 *)&d_cR)[0] = 0x7f80;
			((mlib_s16 *)&d_cR)[1] = (bFixedEnd >> 9);
			((mlib_s16 *)&d_cR)[2] = (gFixedEnd >> 9);
			((mlib_s16 *)&d_cR)[3] = (rFixedEnd >> 9);

			((mlib_s16 *)&d_cS)[0] = 0;
			((mlib_s16 *)&d_cS)[1] = (bFixedStep >> 9);
			((mlib_s16 *)&d_cS)[2] = (gFixedStep >> 9);
			((mlib_s16 *)&d_cS)[3] = (rFixedStep >> 9);

			if ((((((mlib_addr)(startPtr) & 7) >> 2) +
			    (((mlib_addr)(zstartPtr) & 7) >> 1)) & 1) == 0)
				MLIB_FILL_ROW_GZ_32AA(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedEnd, zFixedStep,
				    d_cL, d_cR, d_cS);
			else
				MLIB_FILL_ROW_GZ_32GE(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedEnd, zFixedStep,
				    d_cL, d_cR, d_cS);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BGZ_8(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 alpha)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG8Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 cFixed, cFixedStep;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	mlib_u8 *startPtr;
	mlib_u8 *endPtr;
	mlib_s16 *zstartPtr;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG8Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG8) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ8(x, y, z, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

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

			cFixed = rowPtr[i].c;
			cFixedStep =
			    (rowPtr[i + 1].c - rowPtr[i].c) / (xs2 - xs1 + 1);

			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				cFixed += (cFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			startPtr = buf_data + xs1;
			endPtr = buf_data + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			MLIB_FILL_ROW_BGZ_8(startPtr, endPtr, zstartPtr, zFixed,
			    zFixedStep, cFixed, cFixedStep, alpha);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BGZ_32(
    mlib_image *buffer,
    mlib_image *zbuffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 alpha)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 rFixed, rFixedStep, rFixedEnd;
	mlib_s32 gFixed, gFixedStep, gFixedEnd;
	mlib_s32 bFixed, bFixedStep, bFixedEnd;
	mlib_s32 zFixed, zFixedStep, zFixedEnd;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG32Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	mlib_s32 *startPtr;
	mlib_s32 *endPtr;
	mlib_s16 *zstartPtr;
	mlib_d64 d_cL, d_cR, d_cS;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x - transition array and z - values. */
	pxTrans =
	    (SpanDataG32Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG32) +
	    sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ32(x, y, z, npoints, c, yMin, yMax, pxTrans,
	    intrsMax, rowSpans);

/* Fill scan lines. */

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

			rFixed = rowPtr[i].r;
			rFixedStep =
			    (rowPtr[i + 1].r - rowPtr[i].r) / (xs2 - xs1 + 1);
			gFixed = rowPtr[i].g;
			gFixedStep =
			    (rowPtr[i + 1].g - rowPtr[i].g) / (xs2 - xs1 + 1);
			bFixed = rowPtr[i].b;
			bFixedStep =
			    (rowPtr[i + 1].b - rowPtr[i].b) / (xs2 - xs1 + 1);
			zFixed = rowPtr[i].z;
			zFixedStep =
			    (rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 + 1);

			if (xs1 < 0) {
				rFixed += (rFixedStep * (-xs1));
				gFixed += (gFixedStep * (-xs1));
				bFixed += (bFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			rFixedEnd = rFixed + (xs2 - xs1) * rFixedStep;
			gFixedEnd = gFixed + (xs2 - xs1) * gFixedStep;
			bFixedEnd = bFixed + (xs2 - xs1) * bFixedStep;
			zFixedEnd = zFixed + (xs2 - xs1) * zFixedStep;

			startPtr = ((mlib_s32 *)buf_data) + xs1;
			endPtr = ((mlib_s32 *)buf_data) + xs2;
			zstartPtr = ((mlib_s16 *)zbuf_data) + xs1;

			((mlib_s16 *)&d_cL)[0] = 0x7f80;
			((mlib_s16 *)&d_cL)[1] = (bFixed >> 9);
			((mlib_s16 *)&d_cL)[2] = (gFixed >> 9);
			((mlib_s16 *)&d_cL)[3] = (rFixed >> 9);

			((mlib_s16 *)&d_cR)[0] = 0x7f80;
			((mlib_s16 *)&d_cR)[1] = (bFixedEnd >> 9);
			((mlib_s16 *)&d_cR)[2] = (gFixedEnd >> 9);
			((mlib_s16 *)&d_cR)[3] = (rFixedEnd >> 9);

			((mlib_s16 *)&d_cS)[0] = 0;
			((mlib_s16 *)&d_cS)[1] = (bFixedStep >> 9);
			((mlib_s16 *)&d_cS)[2] = (gFixedStep >> 9);
			((mlib_s16 *)&d_cS)[3] = (rFixedStep >> 9);

			if ((((((mlib_addr)(startPtr) & 7) >> 2) +
			    (((mlib_addr)(zstartPtr) & 7) >> 1)) & 1) == 0)
				MLIB_FILL_ROW_BGZ_32AA(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedEnd, zFixedStep,
				    d_cL, d_cR, d_cS, alpha);
			else
				MLIB_FILL_ROW_BGZ_32GE(startPtr, endPtr,
				    zstartPtr, zFixed, zFixedEnd, zFixedStep,
				    d_cL, d_cR, d_cS, alpha);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * Count vertical limits of polygon and max number of intersections
 * with scan line (number of polygon extrema).
 */

static mlib_s32
mlib_CountExtremaAndMinMax_GZ(
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
mlib_ComputeSpansData_GZ8(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG8Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans)
{
	SpanDataG8Ptr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 z1, z2, zPrev;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 c1, c2, cPrev;
	mlib_s32 cFixed, cFixedStep;

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
	cPrev = (COLOR_MASK & c[npoints - 1]);

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		z1 = zPrev;
		c1 = cPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		z2 = zPrev = z[i];
		c2 = cPrev = (COLOR_MASK & c[i]);

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			z1 ^= z2 ^= z1 ^= z2;
			c1 ^= c2 ^= c1 ^= c2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		zFixed = (z1 << MLIB_SHIFT) + MLIB_05;
		zFixedStep = ((z2 - z1) << MLIB_SHIFT) / dy;

		cFixed = (c1 << 23) + 0x00400000;
		cFixedStep = ((c2 - c1) << 23) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		zFixed += zFixedStep * dely;
		cFixed += cFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].z = zFixed;
			rowPtr[j + 1].c = cFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));
			zFixed += zFixedStep;
			cFixed += cFixedStep;
		}
	}
}

/* *********************************************************** */

static void
mlib_ComputeSpansData_GZ32(
    const mlib_s16 *x,
    const mlib_s16 *y,
    const mlib_s16 *z,
    mlib_s32 npoints,
    const mlib_s32 *c,
    mlib_s32 yMin,
    mlib_s32 yMax,
    SpanDataG32Ptr spansData,
    mlib_s32 maxSpans,
    mlib_s32 *rowSpans)
{
	SpanDataG32Ptr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 z1, z2, zPrev;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 c1, c2, cPrev;
	mlib_s32 rFixed, rFixedStep;
	mlib_s32 gFixed, gFixedStep;
	mlib_s32 bFixed, bFixedStep;

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
	cPrev = c[npoints - 1];

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		z1 = zPrev;
		c1 = cPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		z2 = zPrev = z[i];
		c2 = cPrev = c[i];

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			z1 ^= z2 ^= z1 ^= z2;
			c1 ^= c2 ^= c1 ^= c2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		zFixed = (z1 << MLIB_SHIFT) + MLIB_05;
		zFixedStep = ((z2 - z1) << MLIB_SHIFT) / dy;

		rFixed = ((c1 & 0xFF) << MLIB_SHIFT) + MLIB_05;
		rFixedStep = (((c2 & 0xFF) - (c1 & 0xFF)) << MLIB_SHIFT) / dy;

		gFixed = ((c1 & 0xFF00) << 8) + MLIB_05;
		gFixedStep = (((c2 & 0xFF00) - (c1 & 0xFF00)) << 8) / dy;

		bFixed = (c1 & 0xFF0000) + MLIB_05;
		bFixedStep = ((c2 & 0xFF0000) - (c1 & 0xFF0000)) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		zFixed += zFixedStep * dely;
		rFixed += rFixedStep * dely;
		gFixed += gFixedStep * dely;
		bFixed += bFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].z = zFixed;
			rowPtr[j + 1].r = rFixed;
			rowPtr[j + 1].g = gFixed;
			rowPtr[j + 1].b = bFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));

			zFixed += zFixedStep;
			rFixed += rFixedStep;
			gFixed += gFixedStep;
			bFixed += bFixedStep;
		}
	}
}

/* *********************************************************** */
