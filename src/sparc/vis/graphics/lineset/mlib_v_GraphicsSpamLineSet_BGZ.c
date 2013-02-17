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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_BGZ.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_BGZ_8  - line drawing function
 *	with Gouraud shading,  Z - buffering and alpha blending
 *    mlib_GraphicsSpamLineSet_BGZ_32 - line drawing function
 *	with Gouraud shading,  Z - buffering and alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_BGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineSet_BGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha);
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
 *   alpha        Alpha value for the blending.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_ImageDivTables.h>
#include <vis_proto.h>

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
#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT + \
					((((T) >> (MLIB_SHIFT - 2)) | \
						ZZ) &~ 3))
#define	MLIB_POINTER_COEF	4
#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_BGZ_FPACK(dst, src1, src2, src3, src4)              \
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
    mlib_s32 count,
    mlib_s16 *zdata,
    mlib_s32 tz,
    mlib_s32 sz,
    mlib_s32 tc,
    mlib_s32 sc,
    mlib_s32 a)
{
	mlib_d64 w_c = 0, w_sz, w_data, w_data1, *wp_data, *wp_zdata;
	mlib_u8 *data_end;
	mlib_s16 *zdata_end;
	mlib_s32 mask, maskz, i, maskz1, sc4;
	mlib_s32 mask_z, emaskz, shiftt, shiftt1, shif;
	mlib_d64 w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc;
	mlib_d64 w_tz, w_tz1, w_tz2, w_tz3;
	mlib_s32 sa, sonema;
	mlib_f32 fa, fonema;
	mlib_d64 donema, m00, m01, m10, m11, sum0, sum1, ld, st, da;

	sa = a & 0xFF;
	sonema = ~a & 0xFF;
	da =  vis_to_double_dup((sa << 23) | (sa << 7));
	/* 0xFF - alpha */
	donema = vis_to_double_dup((sonema << 23) | (sonema << 7));

	count++;
	data_end = data + (count - 1);
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

	MLIB_BGZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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
		MLIB_BGZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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
			MLIB_BGZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);

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

		BLEND_WORD
		MLIB_GZ8_FPADD(w_tc_0, w_tc_1, w_tc_2, w_tc_3, w_sc);
		MLIB_BGZ_FPACK(w_c, w_tc_0, w_tc_1, w_tc_2, w_tc_3);
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

#define	MLIB_BLENDING_8(s, c)                                   \
	s = (mlib_U82D64[s] * A0) + (mlib_U82D64[c] * A1)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 a)
{
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, c, n, t, dt, dz, dc, zz, stz, st, stc, xz;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	vis_write_gsr(0);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		c = ~coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0)
			if (y < 0) {
				y = ~y;
				data = rtable[y >> MLIB_SHIFT];
				zdata = (mlib_s16 *)(zrtable[y >> MLIB_SHIFT]);

				if (dt) {
					t = y;
					stz = z >> MLIB_SHIFT;
					z += dz;
					stc = c >> MLIB_SHIFT;
					data = rtable[-1];
					xz = -1;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++) {
						zdata = (mlib_s16 *)
						(MLIB_GET_POINTER(zrtable, t));
						zz = (zdata[x] - stz) >> 31;
						MLIB_BLENDING_8(data[xz], stc);
						stc = c >> MLIB_SHIFT;
						c += dc;
						zdata1 = (mlib_s16 *)
						(MLIB_GET_POINTER2
							(zrtable, t, zz));
						data =
						MLIB_GET_POINTER2
								(rtable, t, zz);
						xz = (x | zz);
						zdata1[xz] = stz;
						x++;
						stz = z >> MLIB_SHIFT;
						z += dz;
						t += dt;
					}

					MLIB_BLENDING_8(data[xz], stc);
				} else {
					MLIB_FILL_ROW_BGZ_8(data + x, n,
					    zdata + x, 2 * z, 2 * dz, c << 8,
					    dc << 8, a);
				}
			} else {
				zdata = (mlib_s16 *)(zrtable[y]);
				data = rtable[-1];
				t = ~x;
				stc = c >> MLIB_SHIFT;
				stz = z >> MLIB_SHIFT;
				st = t >> MLIB_SHIFT;
				xz = -1;

#pragma pipeloop(0)
				for (j = 0; j <= n; j++) {
					zz = (zdata[st] - stz) >> 31;
					t += dt;
					z += dz;
					MLIB_BLENDING_8(data[xz], stc);
					zdata1 =
					    *(mlib_s16 **)((mlib_u8 *)zrtable +
					    MLIB_POINTER_COEF * (y | zz));
					data =
					    *(mlib_u8 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_COEF * (y | zz));
					xz = st | zz;
					y++;
					zdata1[xz] = stz;
					stc = c >> MLIB_SHIFT;
					c += dc;
					st = t >> MLIB_SHIFT;
					stz = z >> MLIB_SHIFT;
					zdata =
					    (mlib_s16 *)((mlib_u8 *)zdata +
					    zstride);
				}

				MLIB_BLENDING_8(data[xz], stc);
			}
	}
}

/* *********************************************************** */

#define	MLIB_PUT_BGZ_32(DATA, X)                    \
	ld = *(mlib_f32 *)((mlib_u8 *)DATA + (X));  \
	m0 = vis_fmul8x16(ld, donema);              \
	m1 = vis_fmul8x16(fa, w_c);                 \
	sum = vis_fpadd16(m0, m1);                  \
	st = vis_fpack16(sum);                      \
	st = vis_fors(st, falpha);                  \
	*(mlib_f32 *)((mlib_u8 *)DATA + (X)) = st;  \
	w_c = vis_fpadd16(w_c, w_dc);

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 a)
{
	mlib_d64 w_c, w_c2, w_dc, w_dc2;
	mlib_d64 w_const = vis_to_double(0x7FFFFFFF, 0xFFFFFFFF);
	mlib_f32 v_t, v_zero = vis_to_float(0);
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, zz, stz, xz;
	mlib_s32 sa, sonema;
	mlib_f32 fa, fonema, ld;
	mlib_d64 donema, m0, m1, sum;
	mlib_f32 falpha = vis_to_float(0xFF000000), st;

	sa = a & 0xFF;
	sonema = ~a & 0xFF;
	sa = (sa << 16) | (sa << 8) | sa;
	fa = vis_to_float(sa);
	/* 0xFF - alpha */
	donema = vis_to_double((sonema << 7), (sonema << 23) | (sonema << 7));

#pragma pipeloop(0)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		vis_write_gsr(8 << 3);

		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];

		w_c2 =
		    vis_freg_pair(v_zero,
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC2]);
		w_c =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STARTC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
		w_dc2 =
		    vis_freg_pair(v_zero,
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC2]);
		w_dc =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STEPC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC]);
		w_c2 = vis_fxor(w_c2, w_const);
		w_c = vis_fnot(w_c);

		w_c = vis_fpackfix_pair(w_c2, w_c);
		w_dc = vis_fpackfix_pair(w_dc2, w_dc);

		vis_write_gsr(0);

		if (n >= 0)
			if (y < 0) {
				t = ~y;

				if (dt) {
					n += x;
					x *= 2;
					n *= 2;
					stz = z >> MLIB_SHIFT;
					zdata =
					    (mlib_s16
					    *)(MLIB_GET_POINTER(zrtable, t));
					zz = (*(mlib_s16 *)((mlib_u8 *)zdata +
					    x) - stz) >> 31;
					zdata1 =
					    (mlib_s16 *)
					    MLIB_GET_POINTER2(zrtable, t, zz);

#pragma pipeloop(0)
					for (; x < n; ) {
						data = MLIB_GET_POINTER2
							(rtable, t, zz);
						t += dt;
						xz = x | (2 * zz);
						x += 2;
						*(mlib_s16 *)((mlib_u8 *)zdata1
								+ xz) = stz;
						z += dz;
						stz = z >> MLIB_SHIFT;
						zdata = (mlib_s16 *)
						(MLIB_GET_POINTER(zrtable, t));
						zz = (*(mlib_s16 *)
						((mlib_u8 *)zdata + x)
								- stz) >> 31;
						MLIB_PUT_BGZ_32(data, 2 * xz);
						zdata1 = (mlib_s16 *)
						MLIB_GET_POINTER2
							(zrtable, t, zz);
					}

					data = MLIB_GET_POINTER2(rtable, t, zz);
					xz = x | (2 * zz);
					*(mlib_s16 *)((mlib_u8 *)zdata1 + xz) =
					    stz;
					MLIB_PUT_BGZ_32(data, 2 * xz);
				} else {
					zdata =
					    (mlib_s16 *)(zrtable[t >>
					    MLIB_SHIFT]);
					stz = z >> MLIB_SHIFT;
					z += dz;
					x *= 2;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++) {
						zz = (*(mlib_s16 *)((mlib_u8 *)
						    zdata + x) - stz) >> 31;
						xz = x | (2 * zz);
						zdata1 =
						    (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data =
						    MLIB_GET_POINTER2(rtable, t,
						    zz);
						*(mlib_s16 *)((mlib_u8 *)zdata1
						    + xz) = stz;
						stz = z >> MLIB_SHIFT;
						z += dz;
						MLIB_PUT_BGZ_32(data,
						    2 * xz);
						x += 2;
					}
				}
			} else {
				data = rtable[y];
				zdata = (mlib_s16 *)(zrtable[y]);
				t = ~x;
				x = t >> MLIB_SHIFT;
				stz = z >> MLIB_SHIFT;
				z += dz;
				t += dt;
				x *= 2;

#pragma unroll(1)
#pragma pipeloop(0)
				for (j = 0; j <= n; j++) {
					zz = (*(mlib_s16 *)((mlib_u8 *)zdata +
					    x) - stz) >> 31;
					xz = x | (2 * zz);
					x = (t >> (MLIB_SHIFT - 1)) & ~1;
					t += dt;
					zdata =
					    (mlib_s16 *)((mlib_u8 *)zdata +
					    zstride);
					zdata1 = (mlib_s16 *)(zrtable[y | zz]);
					data = rtable[y | zz];
					y++;
					*(mlib_s16 *)((mlib_u8 *)zdata1 + xz) =
					    stz;
					stz = z >> MLIB_SHIFT;
					z += dz;
					MLIB_PUT_BGZ_32(data, 2 * xz);
				}
			}
	}
}

/* *********************************************************** */
