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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_BZ.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_BZ_8,
 *    mlib_GraphicsSpamLineSet_BZ_32 - line drawing function with
 *                                     Z-buffer and blending mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_BZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_BZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
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
 *   color        Color to draw, for MLIB_BYTE type of image color must be
 *                the same in all bytes :
 *                  c = (color & 0xFF),
 *                  c |= (c << 8),
 *                  c |= (c << 16)
 *   alpha	  Blending value
 *   dcolor       dcolor = vis_to_double_dup(color)
 */

#include <vis_proto.h>
#include <mlib_graphics.h>
#include <mlib_ImageDivTables.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(RT, T)						\
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 3)) & ~7))

#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT +   	\
					((((T) >> (MLIB_SHIFT - 3)) | 	\
						ZZ) &~ 7))
#define	MLIB_POINTER_COEF	8

#else /* _LP64 */

#define	MLIB_GET_POINTER(RT, T)                                         \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 2)) & ~3))

#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT +   	\
					((((T) >> (MLIB_SHIFT - 2)) | 	\
						ZZ) &~ 3))
#define	MLIB_POINTER_COEF	4
#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_V_BLENDING_8(d, s)                                 \
	{                                                       \
	    mlib_f32 r0 =                                       \
		vis_fpack16(vis_fpsub16(d_ca,                   \
		vis_fmul8x16(vis_read_hi(s), d_a1)));           \
	    mlib_f32 r1 =                                       \
		vis_fpack16(vis_fpsub16(d_ca,                   \
		vis_fmul8x16(vis_read_lo(s), d_a1)));           \
	    d = vis_freg_pair(r0, r1);                          \
	}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_8(
    mlib_u8 *data,
    mlib_s16 *zdata,
    mlib_s32 count,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_d64 d_ca,
    mlib_d64 d_a1)
{
	mlib_d64 w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3, w_c;
	mlib_s32 mask, maskz, i, maskz1;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_s16 *zdata_end = zdata + count;
	mlib_u8 *data_end = data + count;

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

		MLIB_V_BLENDING_8(w_c, wp_data[0]);
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

			MLIB_V_BLENDING_8(w_c, wp_data[0]);
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

		MLIB_V_BLENDING_8(w_c, wp_data[0]);
		vis_pst_8(w_c, wp_data, emaskz);
		wp_data++;
		emaskz = mask_z << shiftt1;
		mask = vis_edge8(wp_data, data_end);
		emaskz &= mask;

		MLIB_V_BLENDING_8(w_c, wp_data[0]);
		vis_pst_8(w_c, wp_data, emaskz);
	} else {
		emaskz = mask_z << shiftt1;

		if ((mlib_addr)wp_data < (mlib_addr)data) {
			mask = vis_edge8(data, data_end);
		} else {
			mask = vis_edge8(wp_data, data_end);
		}

		emaskz &= mask;
		MLIB_V_BLENDING_8(w_c, wp_data[0]);
		vis_pst_8(w_c, wp_data, emaskz);
	}
}

/* *********************************************************** */

#define	MLIB_V_BLENDING_32(d, s)                                \
	{                                                       \
	    mlib_f32 d0, d1;                                    \
	    MLIB_GRAPHICS_BLEND2_32(d0, vis_read_hi(s));        \
	    MLIB_GRAPHICS_BLEND2_32(d1, vis_read_lo(s));        \
	    d = vis_freg_pair(d0, d1);             		\
	}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_32AA(
    mlib_s32 *data,
    mlib_s16 *zdata,
    mlib_s32 count,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_d64 vca,
    mlib_d64 va_1)
{
	mlib_d64 w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata, w_c;
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
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			maskz &= mask;
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, maskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
		} else
			return;
	} else {
		maskz &= mask;
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
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
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		MLIB_V_BLENDING_32(w_c, wp_data[1]);
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
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;
			emaskz = maskz << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		} else {
			emaskz = maskz << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		}
	}
}

/* *********************************************************** */

static void
MLIB_FILL_ROW_BZ_32GE(
    mlib_s32 *data,
    mlib_s16 *zdata,
    mlib_s32 count,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_d64 vca,
    mlib_d64 va_1)
{
	mlib_d64 w_tz, w_tz1, w_sz, w_data, *wp_data, *wp_zdata, w_c;
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
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
		wp_data++;

		if (((mlib_s32 *)wp_data - data) < count) {
			mask = vis_edge32(wp_data, data_end);
			emaskz = mask_z >> 1;
			emaskz &= mask;
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;

			if (((mlib_s32 *)wp_data - data) >= count)
				return;
		} else
			return;
	} else if (i != 1) {
		emaskz = mask_z >> 1;
		emaskz &= mask;
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);

		if (((mlib_s32 *)wp_data - data) >= count)
			return;
		wp_data++;
	}

	if (((mlib_s32 *)wp_data - data) == (count - 1)) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
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
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz1);
			MLIB_V_BLENDING_32(w_c, wp_data[2]);
			vis_pst_32(w_c, wp_data + 2, emaskz);
			emaskz = mask_z >> 1;
			MLIB_V_BLENDING_32(w_c, wp_data[1]);
			vis_pst_32(w_c, wp_data + 1, emaskz2);
			MLIB_V_BLENDING_32(w_c, wp_data[3]);
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
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz1);
			MLIB_V_BLENDING_32(w_c, wp_data[1]);
			vis_pst_32(w_c, wp_data + 1, emaskz2);
			wp_data += 2;
			wp_zdata++;
			i += 4;
			emaskz1 = mask_z >> 3;
			emaskz2 = mask_z >> 1;
			w_data = w_data1;
		}

		MLIB_V_BLENDING_32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz1);
		MLIB_V_BLENDING_32(w_c, wp_data[1]);
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
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
			wp_data++;
			emaskz = mask_z << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		} else {
			emaskz = mask_z << shiftt;
			mask = vis_edge32(wp_data, data_end);
			emaskz &= mask;
			MLIB_V_BLENDING_32(w_c, wp_data[0]);
			vis_pst_32(w_c, wp_data, emaskz);
		}
	} else if (i == count) {
		emaskz = mask_z << 1;
		mask = vis_edge32(wp_data, data_end);
		emaskz &= mask;
		MLIB_V_BLENDING_32(w_c, wp_data[0]);
		vis_pst_32(w_c, wp_data, emaskz);
	}
}

/* *********************************************************** */

#define	MLIB_C_BLENDING(s, ddca)	s = (mlib_U82D64[s] * dda) + ddca;

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc)
{
	mlib_d64 ddca, dda, d_ca, d_a1;
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 x, y, z, n, t, dt, dz, zz, stz, sdt, xz;
	mlib_s32 ca, a1;

	c &= 0xff;
	a &= 0xff;

	dda = ((255. - a) / 255.);
	ddca = c * a / 255.;

	a1 = (mlib_s32)(32768.0 * (a * (1.0 / 255) - 1) - 0.5);
	ca = (mlib_s32)((128.0 / 255) * c * a + 0.5);

	d_a1 = vis_to_double_dup((a1 << 16) | (a1 & 0xffff));
	d_ca = vis_to_double_dup((ca << 16) | ca);

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
						MLIB_C_BLENDING(data[xz], ddca);
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

					MLIB_C_BLENDING(data[xz], ddca);
				} else {
					MLIB_FILL_ROW_BZ_8(data + x, zdata + x,
					    n, 2 * z, 2 * dz, d_ca, d_a1);
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
					MLIB_C_BLENDING(data[xz], ddca);
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

				MLIB_C_BLENDING(data[xz], ddca);
			}
	}
}

/* *********************************************************** */

#define	MLIB_C_DEMUX_CHANNELS(r, g, b, a, s)                    \
	r = ((s) & 0xff);                                       \
	g = ((s) >> 8) & 0xff;                                  \
	b = ((s) >> 16) & 0xff;					\
	a = (mlib_s32)0xff000000;

/* *********************************************************** */

#define	MLIB_C_MUX_CHANNELS(d, r, g, b, a)                      \
	d = (a | (b << 16) | (g << 8) | r);

/* *********************************************************** */

#define	MLIB_C_BLENDING_32(d)                                   \
	{                                                       \
	    MLIB_C_DEMUX_CHANNELS(rs, gs, bs, as, d);		\
	    MLIB_C_BLENDING(rs, ddca1);                         \
	    MLIB_C_BLENDING(gs, ddca2);                         \
	    MLIB_C_BLENDING(bs, ddca3);                         \
	    MLIB_C_MUX_CHANNELS(d, rs, gs, bs, as);             \
	}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc)
{
	mlib_s32 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 x, y, z, n, t, dt, dz, zz, stz, sdt, xz;
	mlib_s32 rs, gs, bs, as;
	mlib_d64 ddca1, ddca2, ddca3, dda;

	MLIB_GRAPHICS_DECL_VAR_B_32;

	MLIB_C_DEMUX_CHANNELS(rs, gs, bs, as, c);
	a &= 0xff;

	MLIB_GRAPHICS_SET_VAR_B_32(c, a);

	dda = ((255. - a) / 255.);
	ddca1 = rs * a / 255.;
	ddca2 = gs * a / 255.;
	ddca3 = bs * a / 255.;

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
						MLIB_C_BLENDING_32(data[xz]);
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

					MLIB_C_BLENDING_32(data[xz]);
				} else {
					data += x;
					zdata += x;
					z += z;
					dz += dz;

					if ((((((mlib_addr)(data) & 7) >> 2) +
					    (((mlib_addr)(zdata) & 7) >> 1)) &
					    1) == 0) {
						MLIB_FILL_ROW_BZ_32AA(data,
						    zdata, n, z, dz, vca, va_1);
					} else {
						MLIB_FILL_ROW_BZ_32GE(data,
						    zdata, n, z, dz, vca, va_1);
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
					MLIB_C_BLENDING_32(data[xz]);
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

				MLIB_C_BLENDING_32(data[xz]);
			}
	}
}

/* *********************************************************** */
