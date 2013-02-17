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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_Z.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_Z_8  - line drawing function with Z - buffer mode
 *    mlib_GraphicsSpamLineSet_Z_32 - line drawing function with Z - buffer mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_Z_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_Z_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    zrtable     Pointer to array of starts of row of zbuffer, zrtable[-1][-1]
 *                  is buffer for unwanted stores
 *    stride      Stride of the image
 *    zstride     Stride of the zbuffer image
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT_GZ]      holds X
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT_GZ]      holds Y
 *                  coords[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT_GZ] holds LL
 *                  coords[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ]   holds SL
 *                  if |dx| > |dy| then
 *                    X  = x0, Y = ~(MLIB_HALF + (y0 << MLIB_SHIFT))
 *                    LL = |dx|, SL = (mlib_s32) (32768. * dy / LL)
 *                  else
 *                    X = ~(MLIB_HALF + (x0 << MLIB_SHIFT)), Y = y0
 *                    LL = |dy|, SL = (mlib_s32) (32768. * dx / LL)
 *                  endif
 *                , so we could understand in *Spam* functions what is
 *                bigger |dx| or |dy| by negative value of opposite starting
 *                value. Note we always step by +1 by main axis, never -1.
 *   coordsend    for(; coords < coordsend; coords += MLIB_GDS_NEXT_GZ)
 *    c           Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *   dcolor       dcolor = vis_to_double_dup(color)
 */

#include <mlib_graphics.h>
#include <vis_proto.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 3)) & ~7))
#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT +   \
					((((T) >> (MLIB_SHIFT - 3)) | \
						ZZ) &~ 7))
#define	MLIB_POINTER_COEF	8

#else /* _LP64 */

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 2)) & ~3))
#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT +   \
					((((T) >> (MLIB_SHIFT - 2)) | \
						ZZ) &~ 3))
#define	MLIB_POINTER_COEF	4
#endif /* _LP64 */

/* *********************************************************** */

static void
MLIB_FILL_ROW_Z_8(
    mlib_u8 *data,
    mlib_s16 *zdata,
    mlib_s32 count,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 c,
    mlib_d64 w_c)
{
	mlib_d64 w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_s32 mask, maskz, i, maskz1;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3;
	mlib_u8 *data_end = data + count;
	mlib_s16 *zdata_end = zdata + count;

	count++;

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
    mlib_s16 *zdata,
    mlib_s32 count,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 c,
    mlib_d64 w_c)
{
	mlib_d64 w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_s32 mask, maskz, i, emaskz, shiftt, shif;
	mlib_s32 *data_end = data + count;
	mlib_s16 *zdata_end = zdata + count;

	count++;

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
    mlib_s16 *zdata,
    mlib_s32 count,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 c,
    mlib_d64 w_c)
{
	mlib_d64 w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata;
	mlib_s32 mask, maskz, mask_z, i, emaskz, shiftt, shif;
	mlib_d64 w_data1, w_tz2, w_tz3;
	mlib_s32 emaskz1, emaskz2, maskz1;
	mlib_s32 *data_end = data + count;
	mlib_s16 *zdata_end = zdata + count;

	count++;

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

void
mlib_GraphicsSpamLineSet_Z_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 x, y, z, n, t, dt, dz, zz, stz, sdt, xz;

	vis_write_gsr(0);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0)
			if (y < 0) {
				y = ~y;
				data = rtable[y >> MLIB_SHIFT];
				zdata = (mlib_s16 *)(zrtable[y >> MLIB_SHIFT]);

				if (dt) {
					t = y;
					n += x;
					stz = z >> MLIB_SHIFT;
					z += dz;
					data = rtable[-1];
					xz = -1;

#pragma pipeloop(0)
					for (; x <= n; x++) {
						zdata =
						    (mlib_s16
						    *)(MLIB_GET_POINTER(zrtable,
						    t));
						zz = (zdata[x] - stz) >> 31;
						data[xz] = c;
						zdata1 =
						    (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data =
						    MLIB_GET_POINTER2(rtable, t,
						    zz);
						xz = (x | zz);
						zdata1[xz] = stz;
						stz = z >> MLIB_SHIFT;
						z += dz;
						t += dt;
					}

					data[xz] = c;
				} else {
					MLIB_FILL_ROW_Z_8(data + x, zdata + x,
					    n, 2 * z, 2 * dz, c, dc);
				}
			} else {
				zdata = (mlib_s16 *)(zrtable[y]);
				data = rtable[-1];
				n += y;
				t = ~x;
				stz = z >> MLIB_SHIFT;
				sdt = t >> MLIB_SHIFT;
				xz = -1;

#pragma pipeloop(0)
				for (; y <= n; y++) {
					zz = (zdata[sdt] - stz) >> 31;
					t += dt;
					z += dz;
					data[xz] = c;
					zdata1 =
					    *(mlib_s16 **)((mlib_u8 *)zrtable +
					    MLIB_POINTER_COEF * (y | zz));
					data =
					    *(mlib_u8 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_COEF * (y | zz));
					xz = sdt | zz;
					zdata1[xz] = stz;
					sdt = t >> MLIB_SHIFT;
					stz = z >> MLIB_SHIFT;
					zdata =
					    (mlib_s16 *)((mlib_u8 *)zdata +
					    zstride);
				}

				data[xz] = c;
			}
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_Z_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_s32 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 x, y, z, n, t, dt, dz, zz, stz, sdt, xz;

	vis_write_gsr(0);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0)
			if (y < 0) {
				y = ~y;
				data = (mlib_s32 *)(rtable[y >> MLIB_SHIFT]);
				zdata = (mlib_s16 *)(zrtable[y >> MLIB_SHIFT]);

				if (dt) {
					t = y;
					n += x;
					stz = z >> MLIB_SHIFT;
					z += dz;
					data = (mlib_s32 *)(rtable[-1]);
					xz = -1;

#pragma pipeloop(0)
					for (; x <= n; x++) {
						zdata =
						    (mlib_s16
						    *)(MLIB_GET_POINTER(zrtable,
						    t));
						zz = (zdata[x] - stz) >> 31;
						data[xz] = c;
						zdata1 =
						    (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data =
						    (mlib_s32
						    *)(MLIB_GET_POINTER2(rtable,
						    t, zz));
						xz = (x | zz);
						zdata1[xz] = stz;
						stz = z >> MLIB_SHIFT;
						z += dz;
						t += dt;
					}

					data[xz] = c;
				} else {
					data += x;
					zdata += x;
					z += z;
					dz += dz;

					if ((((((mlib_addr)(data) & 7) >> 2) +
					    (((mlib_addr)(zdata) & 7) >> 1)) &
					    1) == 0) {
						MLIB_FILL_ROW_Z_32AA(data,
						    zdata, n, z, dz, c, dc);
					} else {
						MLIB_FILL_ROW_Z_32GE(data,
						    zdata, n, z, dz, c, dc);
					}
				}
			} else {
				zdata = (mlib_s16 *)(zrtable[y]);
				data = (mlib_s32 *)(rtable[-1]);
				n += y;
				t = ~x;
				stz = z >> MLIB_SHIFT;
				sdt = t >> MLIB_SHIFT;
				xz = -1;

#pragma pipeloop(0)
				for (; y <= n; y++) {
					zz = (zdata[sdt] - stz) >> 31;
					t += dt;
					z += dz;
					data[xz] = c;
					zdata1 =
					    *(mlib_s16 **)((mlib_u8 *)zrtable +
					    MLIB_POINTER_COEF * (y | zz));
					data =
					    *(mlib_s32 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_COEF * (y | zz));
					xz = sdt | zz;
					zdata1[xz] = stz;
					sdt = t >> MLIB_SHIFT;
					stz = z >> MLIB_SHIFT;
					zdata =
					    (mlib_s16 *)((mlib_u8 *)zdata +
					    zstride);
				}

				data[xz] = c;
			}
	}
}

/* *********************************************************** */

/* unoptimized version */
/*
 *  mlib_u8  *data;
 *  mlib_s16 *zdata;
 *  mlib_s32 j, x, y, z, n, t, dt, dz;
 *
 *  for(;coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
 *    n  = coords[MLIB_GDS_LENGTHLINES];
 *    y  = coords[MLIB_GDS_STARTY];
 *    x  = coords[MLIB_GDS_STARTX];
 *    z  = ~coords[MLIB_GDS_STARTZ];
 *    dz = coords[MLIB_GDS_STEPZ];
 *    dt = coords[MLIB_GDS_STEPLINES];
 *
 *    if (n >= 0)
 *    if (y < 0) {
 *      y = ~y;
 *      data  = rtable[y >> MLIB_SHIFT];
 *      zdata = (mlib_s16*)(zrtable[y >> MLIB_SHIFT]);
 *
 *      if (dt) {
 *        t = y;
 *        for(j = 0; j <= n; j++) {
 *          data  = MLIB_GET_POINTER(rtable, t + j * dt);
 *          zdata = (mlib_s16*)(MLIB_GET_POINTER(zrtable, t + j * dt));
 *
 *          if (zdata[x + j] >= (z >> MLIB_SHIFT)) {
 *            data[x + j]  = c;
 *            zdata[x + j] = (z >> MLIB_SHIFT);
 *          }
 *
 *          z += dz;
 *        }
 *
 *      } else {
 *        for(j = 0; j <= n; j++) {
 *          if (zdata[x + j] >= (z >> MLIB_SHIFT)) {
 *            data[x + j] = c;
 *            zdata[x + j] = (z >> MLIB_SHIFT);
 *          }
 *
 *          z += dz;
 *        }
 *      }
 *
 *    } else {
 *      data  = rtable[y];
 *      zdata = (mlib_s16*)(zrtable[y]);
 *      t = ~x;
 *      x = t >> MLIB_SHIFT;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++) {
 *          if (zdata[j * zstride / 2 + ((t + j * dt) >> MLIB_SHIFT)] >=
 *              (z >> MLIB_SHIFT)) {
 *            zdata[j * zstride / 2 + ((t + j * dt) >> MLIB_SHIFT)] =
 *              z >> MLIB_SHIFT;
 *            data[j * stride + ((t + j * dt) >> MLIB_SHIFT)] = c;
 *          }
 *
 *          z += dz;
 *        }
 *
 *      } else {
 *        t     = stride + (dt >> MLIB_SHIFT);
 *        dt    = zstride + (dt >> (MLIB_SHIFT - 1));
 *        for(j = 0; j <= n; j++) {
 *          if (zdata[x + j * dt / 2] >= (z >> MLIB_SHIFT)) {
 *            zdata[x + j * dt / 2] = z >> MLIB_SHIFT;
 *            data[x + j * t] = c;
 *          }
 *
 *          z += dz;
 *        }
 *      }
 *    }
 *  }
 *
 */

/* *********************************************************** */
