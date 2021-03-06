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

#pragma ident	"@(#)mlib_v_ImageConvIndex4_8_8ext.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageConv*Index on U8 type
 *      and MLIB_EDGE_SRC_EXTEND mask
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_v_ImageConvIndex.h>

/*
 *  This defines switches between functions in
 *  files:
 *         mlib_v_ImageConvIndex3_8_8ext.c,
 *         mlib_v_ImageConvIndex4_8_8ext.c,
 *         mlib_v_ImageConvIndex3_8_16ext.c,
 *         mlib_v_ImageConvIndex4_8_16ext.c
 */

/* *********************************************************** */

#define	LOAD_NF	vis_ld_u8_nf
#define	DTYPE	mlib_u8
#define	LTYPE	mlib_u8

/* *********************************************************** */

#define	CONV_FUNC(KERN)                                                    \
	mlib_conv##KERN##_Index4_8_8ext(mlib_image *dst,                   \
	    const mlib_image *src, const mlib_s32 *kern, mlib_s32 scale,   \
	    const void *colormap)

/* *********************************************************** */

#define	CONV_FUNC_MxN                                                     \
	mlib_convMxN_Index4_8_8ext(mlib_image *dst,                       \
	    const mlib_image *src, mlib_s32 m, mlib_s32 n,                \
	    mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,   \
	    const mlib_s32 *kern, mlib_s32 scale,                         \
	    const void *colormap)

#define	ColorTrue2IndexLine	mlib_ImageColorTrue2IndexLine_U8_U8_4

/* *********************************************************** */

#define	NCHAN	4

/* *********************************************************** */

#define	DEF_VARS                                                      \
	DTYPE    *sl, *sp, *dl;                                       \
	mlib_s32 hgt = mlib_ImageGetHeight(src);                      \
	mlib_s32 wid = mlib_ImageGetWidth(src);                       \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (DTYPE);     \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (DTYPE);     \
	DTYPE    *adr_src = (DTYPE *)mlib_ImageGetData(src);          \
	DTYPE    *adr_dst = (DTYPE *)mlib_ImageGetData(dst);          \
	mlib_s32 ssize, xsize, dsize, esize, buff_ind = 0;            \
	mlib_d64 *pbuff;                                              \
	mlib_f32 *karr = (mlib_f32 *)kern;                            \
	mlib_s32 gsr_scale = (31 - scale) << 3;                       \
	mlib_d64 drnd = vis_to_double_dup(mlib_round_8[31 - scale]);  \
	mlib_s32 i, j, l

/* *********************************************************** */

#if NCHAN == 3

/* *********************************************************** */

#define	DEF_EXTRA_VARS                                                   \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);              \
	LTYPE **lut_table = (LTYPE **) mlib_ImageGetLutData(colormap);   \
	LTYPE *ltbl0 = lut_table[0] - offset;                            \
	LTYPE *ltbl1 = lut_table[1] - offset;                            \
	LTYPE *ltbl2 = lut_table[2] - offset

/* *********************************************************** */

#define	LOAD_SRC()                                                     \
	{                                                              \
	    mlib_s32 s0 = sp[0], s1 = sp[1], s2 = sp[2], s3 = sp[3];   \
	    mlib_s32 s4 = sp[4], s5 = sp[5], s6 = sp[6], s7 = sp[7];   \
	    mlib_d64 t0, t1, t2;                                       \
	                                                               \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s7), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s7), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s7), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s6), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s6), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s6), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s5), t2);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s5), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s4), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s4), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s4), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s2), t1);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s2), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s2), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s0), t0);           \
	    buffn[i] = t0;                                             \
	    buffn[i + 1] = t1;                                         \
	    buffn[i + 2] = t2;                                         \
	    sp += 8;                                                   \
	}

/* *********************************************************** */

#define	LOAD_SRC_NF()                                                  \
	{                                                              \
	    mlib_s32 s0 = LOAD_NF(sp), s1 = LOAD_NF(sp + 1),           \
		s2 = LOAD_NF(sp + 2), s3 = LOAD_NF(sp + 3);            \
	    mlib_s32 s4 = LOAD_NF(sp + 4), s5 = LOAD_NF(sp + 5),       \
		s6 = LOAD_NF(sp + 6), s7 = LOAD_NF(sp + 7);            \
	    mlib_d64 t0, t1, t2;                                       \
	                                                               \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s7), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s7), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s7), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s6), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s6), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s6), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s5), t2);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s5), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s4), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s4), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s4), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s2), t1);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s2), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s2), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s0), t0);           \
	    buffn[i] = t0;                                             \
	    buffn[i + 1] = t1;                                         \
	    buffn[i + 2] = t2;                                         \
	    sp += 8;                                                   \
	}

/* *********************************************************** */

#define	EXT_LEFT(buff, dx_l, i)                                      \
	* ((buff) + 0 + NCHAN * i) = *((buff) + 0 + NCHAN * dx_l);   \
	*((buff) + 1 + NCHAN * i) = *((buff) + 1 + NCHAN * dx_l);    \
	*((buff) + 2 + NCHAN * i) = *((buff) + 2 + NCHAN * dx_l)

/* *********************************************************** */

#define	EXT_RIGHT(buff, dx_l, i)                                \
	* ((buff) + 0 + NCHAN * i + xsize + dx_l * NCHAN) =     \
	    *((buff) + 0 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 1 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 1 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 2 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 2 + NCHAN * (dx_l - 1) + xsize)

#else /* NCHAN == 3 */

/* *********************************************************** */

#define	DEF_EXTRA_VARS                                                   \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);              \
	LTYPE **lut_table = (LTYPE **) mlib_ImageGetLutData(colormap);   \
	LTYPE *ltbl0 = lut_table[0] - offset;                            \
	LTYPE *ltbl1 = lut_table[1] - offset;                            \
	LTYPE *ltbl2 = lut_table[2] - offset;                            \
	LTYPE *ltbl3 = lut_table[3] - offset

/* *********************************************************** */

#define	LOAD_SRC()                                                     \
	{                                                              \
	    mlib_s32 s0 = sp[0], s1 = sp[1], s2 = sp[2], s3 = sp[3];   \
	    mlib_s32 s4 = sp[4], s5 = sp[5];                           \
	    mlib_d64 t0, t1, t2;                                       \
	                                                               \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl3, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl3, s4), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s4), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s4), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s4), t2);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl3, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl3, s2), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s2), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s2), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s2), t1);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl3, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl3, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s0), t0);           \
	    buffn[i] = t0;                                             \
	    buffn[i + 1] = t1;                                         \
	    buffn[i + 2] = t2;                                         \
	    sp += 6;                                                   \
	}

/* *********************************************************** */

#define	LOAD_SRC_NF()                                                  \
	{                                                              \
	    mlib_s32 s0 = LOAD_NF(sp), s1 = LOAD_NF(sp + 1),           \
		s2 = LOAD_NF(sp + 2), s3 = LOAD_NF(sp + 3);            \
	    mlib_s32 s4 = LOAD_NF(sp + 4), s5 = LOAD_NF(sp + 5);       \
	    mlib_d64 t0, t1, t2;                                       \
	                                                               \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl3, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s5), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl3, s4), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl2, s4), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl1, s4), t2);           \
	    t2 = vis_faligndata(vis_ld_u8_i(ltbl0, s4), t2);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl3, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s3), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl3, s2), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl2, s2), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl1, s2), t1);           \
	    t1 = vis_faligndata(vis_ld_u8_i(ltbl0, s2), t1);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl3, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s1), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl3, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl2, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl1, s0), t0);           \
	    t0 = vis_faligndata(vis_ld_u8_i(ltbl0, s0), t0);           \
	    buffn[i] = t0;                                             \
	    buffn[i + 1] = t1;                                         \
	    buffn[i + 2] = t2;                                         \
	    sp += 6;                                                   \
	}

/* *********************************************************** */

#define	EXT_LEFT(buff, dx_l, i)                                      \
	* ((buff) + 0 + NCHAN * i) = *((buff) + 0 + NCHAN * dx_l);   \
	*((buff) + 1 + NCHAN * i) = *((buff) + 1 + NCHAN * dx_l);    \
	*((buff) + 2 + NCHAN * i) = *((buff) + 2 + NCHAN * dx_l);    \
	*((buff) + 3 + NCHAN * i) = *((buff) + 3 + NCHAN * dx_l)

/* *********************************************************** */

#define	EXT_RIGHT(buff, dx_l, i)                                \
	* ((buff) + 0 + NCHAN * i + xsize + dx_l * NCHAN) =     \
	    *((buff) + 0 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 1 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 1 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 2 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 2 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 3 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 3 + NCHAN * (dx_l - 1) + xsize)

#endif /* NCHAN == 3 */

/* *********************************************************** */

static const mlib_s32 mlib_round_8[16] = {
	0x00400040, 0x00200020, 0x00100010, 0x00080008,
	0x00040004, 0x00020002, 0x00010001, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* *********************************************************** */

#define	KSIZE	2

mlib_status
CONV_FUNC(2x2)
{
	mlib_d64 *buffs[2 * (KSIZE + 1)];
	mlib_d64 *buff0, *buff1, *buffn, *buffd, *buffe;
	mlib_d64 s00, s01, s10, s11, s0, s1;
	mlib_d64 d0, d1, d00, d01, d10, d11;

	DEF_VARS;
	DEF_EXTRA_VARS;

	sl = adr_src;
	dl = adr_dst;

	ssize = NCHAN * (wid + (KSIZE - 1));
	dsize = (NCHAN * wid + 7) / 8;
	esize = (ssize + 7) / 8 + 4;
	pbuff = __mlib_malloc((KSIZE + 4) * esize * sizeof (mlib_d64));

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++)
		buffs[i] = pbuff + i * esize;
	for (i = 0; i < (KSIZE + 1); i++)
		buffs[(KSIZE + 1) + i] = buffs[i];
	buffd = buffs[KSIZE] + esize;
	buffe = buffd + 2 * esize;

	xsize = NCHAN * wid;

	vis_write_gsr(gsr_scale + 7);

	for (l = 0; l < KSIZE; l++) {
		mlib_d64 *buffn = buffs[l];

		sp = sl;

#pragma pipeloop(0)
		for (i = 0; i < dsize; i += 3) {
			LOAD_SRC_NF();
		}

		EXT_RIGHT((mlib_u8 *)buffn, 0, 0);

		if (l < hgt - 1)
			sl += sll;
	}

	for (j = 0; j < hgt; j++) {
		mlib_d64 **buffc = buffs + buff_ind;
		mlib_f32 *pk = karr, k0, k1;

		sp = sl;

		buff0 = buffc[0];
		buff1 = buffc[1];
		buffn = buffc[KSIZE];

		k0 = pk[1];
		k1 = pk[3];
		vis_write_gsr(gsr_scale + NCHAN);

		s01 = buff0[0];
		s11 = buff1[0];
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 7) / 8; i++) {
			s00 = s01;
			s10 = s11;
			s01 = buff0[i + 1];
			s11 = buff1[i + 1];
			s0 = vis_faligndata(s00, s01);
			s1 = vis_faligndata(s10, s11);

			d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s1), k1);

			d0 = vis_fpadd16(d00, d10);
			d1 = vis_fpadd16(d01, d11);
			buffd[2 * i] = d0;
			buffd[2 * i + 1] = d1;
		}

		k0 = pk[0];
		k1 = pk[2];

		vis_write_gsr(gsr_scale + 7);

#pragma pipeloop(0)
		for (i = 0; i < dsize; i += 3) {
			mlib_d64 d00, d01, d02, d03, d04, d05;
			mlib_d64 d10, d11, d12, d13, d14, d15;
			mlib_d64 d0, d1, d2, d3, d4, d5;
			mlib_d64 s00 = buff0[i];
			mlib_d64 s01 = buff0[i + 1];
			mlib_d64 s02 = buff0[i + 2];
			mlib_d64 s10 = buff1[i];
			mlib_d64 s11 = buff1[i + 1];
			mlib_d64 s12 = buff1[i + 2];

			d00 = vis_fmul8x16au(vis_read_hi(s00), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s00), k0);
			d02 = vis_fmul8x16au(vis_read_hi(s01), k0);
			d03 = vis_fmul8x16au(vis_read_lo(s01), k0);
			d04 = vis_fmul8x16au(vis_read_hi(s02), k0);
			d05 = vis_fmul8x16au(vis_read_lo(s02), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s10), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s10), k1);
			d12 = vis_fmul8x16au(vis_read_hi(s11), k1);
			d13 = vis_fmul8x16au(vis_read_lo(s11), k1);
			d14 = vis_fmul8x16au(vis_read_hi(s12), k1);
			d15 = vis_fmul8x16au(vis_read_lo(s12), k1);

			d0 = buffd[2 * i];
			d1 = buffd[2 * i + 1];
			d2 = buffd[2 * i + 2];
			d3 = buffd[2 * i + 3];
			d4 = buffd[2 * i + 4];
			d5 = buffd[2 * i + 5];
			d00 = vis_fpadd16(d00, d10);
			d0 = vis_fpadd16(d0, drnd);
			d0 = vis_fpadd16(d0, d00);
			d01 = vis_fpadd16(d01, d11);
			d1 = vis_fpadd16(d1, drnd);
			d1 = vis_fpadd16(d1, d01);
			d02 = vis_fpadd16(d02, d12);
			d2 = vis_fpadd16(d2, drnd);
			d2 = vis_fpadd16(d2, d02);
			d03 = vis_fpadd16(d03, d13);
			d3 = vis_fpadd16(d3, drnd);
			d3 = vis_fpadd16(d3, d03);
			d04 = vis_fpadd16(d04, d14);
			d4 = vis_fpadd16(d4, drnd);
			d4 = vis_fpadd16(d4, d04);
			d05 = vis_fpadd16(d05, d15);
			d5 = vis_fpadd16(d5, drnd);
			d5 = vis_fpadd16(d5, d05);

			buffe[i] = vis_fpack16_pair(d0, d1);
			buffe[i + 1] = vis_fpack16_pair(d2, d3);
			buffe[i + 2] = vis_fpack16_pair(d4, d5);

			LOAD_SRC_NF();
		}

		EXT_RIGHT((mlib_u8 *)buffn, 0, 0);

		ColorTrue2IndexLine((void *)buffe, dl, wid, colormap);

		if (j < hgt - 3)
			sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	3

mlib_status
CONV_FUNC(3x3)
{
	mlib_d64 *buffs[2 * (KSIZE + 1)];
	mlib_d64 *buff0, *buff1, *buff2, *buffn, *buffd, *buffe;
	mlib_d64 s00, s01, s10, s11, s20, s21, s0, s1, s2;
	mlib_d64 d0, d1, d00, d01, d10, d11, d20, d21;
	mlib_s32 ik, ik_last, off, doff;

	DEF_VARS;
	DEF_EXTRA_VARS;

	sl = adr_src;
	dl = adr_dst;

	ssize = NCHAN * (wid + (KSIZE - 1));
	dsize = (NCHAN * wid + 7) / 8;
	esize = (ssize + 7) / 8 + 4;
	pbuff = __mlib_malloc((KSIZE + 4) * esize * sizeof (mlib_d64));

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++)
		buffs[i] = pbuff + i * esize;
	for (i = 0; i < (KSIZE + 1); i++)
		buffs[(KSIZE + 1) + i] = buffs[i];
	buffd = buffs[KSIZE] + esize;
	buffe = buffd + 2 * esize;

	xsize = NCHAN * wid;

	for (l = 0; l < KSIZE; l++) {
		mlib_d64 *buffn = buffs[l];

		sp = sl;

		vis_write_gsr(gsr_scale + 7);

#pragma pipeloop(0)
		for (i = 0; i < dsize; i += 3) {
			LOAD_SRC_NF();
		}

		vis_write_gsr(gsr_scale + (8 - NCHAN));

		d0 = buffn[0];

#pragma pipeloop(0)
		for (i = 0; i <= (xsize + 7) / 8; i++) {
			d1 = buffn[i];
			buffn[i] = vis_faligndata(d0, d1);
			d0 = d1;
		}

		EXT_LEFT((mlib_u8 *)buffn, 1, 0);
		EXT_RIGHT((mlib_u8 *)buffn, 1, 0);

		if ((l >= 1) && (l < hgt))
			sl += sll;
	}

/* init buffer */
#pragma pipeloop(0)
	for (i = 0; i < (xsize + 7) / 8; i++) {
		buffd[2 * i] = drnd;
		buffd[2 * i + 1] = drnd;
	}

	for (j = 0; j < hgt; j++) {
		mlib_d64 **buffc = buffs + buff_ind, *pbuff0, *pbuff1, *pbuff2;
		mlib_f32 *pk = karr, k0, k1, k2;

		sp = sl;

		pbuff0 = buffc[0];
		pbuff1 = buffc[1];
		pbuff2 = buffc[2];
		buffn = buffc[KSIZE];

		ik_last = 0;

		for (ik = 0; ik < KSIZE; ik++) {
			k0 = pk[ik];
			k1 = pk[ik + KSIZE];
			k2 = pk[ik + 2 * KSIZE];

			off = ik * NCHAN;
			doff = off / 8;
			off &= 7;
			buff0 = pbuff0 + doff;
			buff1 = pbuff1 + doff;
			buff2 = pbuff2 + doff;
			vis_write_gsr(gsr_scale + off);

			if (ik == ik_last)
				continue;

			if (off == 0) {
#pragma pipeloop(0)
				for (i = 0; i < (xsize + 7) / 8; i++) {
					s0 = buff0[i];
					s1 = buff1[i];
					s2 = buff2[i];

					d00 =
					    vis_fmul8x16au(vis_read_hi(s0), k0);
					d01 =
					    vis_fmul8x16au(vis_read_lo(s0), k0);
					d10 =
					    vis_fmul8x16au(vis_read_hi(s1), k1);
					d11 =
					    vis_fmul8x16au(vis_read_lo(s1), k1);
					d20 =
					    vis_fmul8x16au(vis_read_hi(s2), k2);
					d21 =
					    vis_fmul8x16au(vis_read_lo(s2), k2);

					d0 = buffd[2 * i];
					d1 = buffd[2 * i + 1];
					d0 = vis_fpadd16(d00, d0);
					d0 = vis_fpadd16(d10, d0);
					d0 = vis_fpadd16(d20, d0);
					d1 = vis_fpadd16(d01, d1);
					d1 = vis_fpadd16(d11, d1);
					d1 = vis_fpadd16(d21, d1);
					buffd[2 * i] = d0;
					buffd[2 * i + 1] = d1;
				}
			} else if (off == 4) {
				s01 = buff0[0];
				s11 = buff1[0];
				s21 = buff2[0];
#pragma pipeloop(0)
				for (i = 0; i < (xsize + 7) / 8; i++) {
					s00 = s01;
					s10 = s11;
					s20 = s21;
					s01 = buff0[i + 1];
					s11 = buff1[i + 1];
					s21 = buff2[i + 1];

					d00 =
					    vis_fmul8x16au(vis_read_lo(s00),
					    k0);
					d01 =
					    vis_fmul8x16au(vis_read_hi(s01),
					    k0);
					d10 =
					    vis_fmul8x16au(vis_read_lo(s10),
					    k1);
					d11 =
					    vis_fmul8x16au(vis_read_hi(s11),
					    k1);
					d20 =
					    vis_fmul8x16au(vis_read_lo(s20),
					    k2);
					d21 =
					    vis_fmul8x16au(vis_read_hi(s21),
					    k2);

					d0 = buffd[2 * i];
					d1 = buffd[2 * i + 1];
					d0 = vis_fpadd16(d00, d0);
					d0 = vis_fpadd16(d10, d0);
					d0 = vis_fpadd16(d20, d0);
					d1 = vis_fpadd16(d01, d1);
					d1 = vis_fpadd16(d11, d1);
					d1 = vis_fpadd16(d21, d1);
					buffd[2 * i] = d0;
					buffd[2 * i + 1] = d1;
				}
			} else {
				s01 = buff0[0];
				s11 = buff1[0];
				s21 = buff2[0];
#pragma pipeloop(0)
				for (i = 0; i < (xsize + 7) / 8; i++) {
					s00 = s01;
					s10 = s11;
					s20 = s21;
					s01 = buff0[i + 1];
					s11 = buff1[i + 1];
					s21 = buff2[i + 1];
					s0 = vis_faligndata(s00, s01);
					s1 = vis_faligndata(s10, s11);
					s2 = vis_faligndata(s20, s21);

					d00 =
					    vis_fmul8x16au(vis_read_hi(s0), k0);
					d01 =
					    vis_fmul8x16au(vis_read_lo(s0), k0);
					d10 =
					    vis_fmul8x16au(vis_read_hi(s1), k1);
					d11 =
					    vis_fmul8x16au(vis_read_lo(s1), k1);
					d20 =
					    vis_fmul8x16au(vis_read_hi(s2), k2);
					d21 =
					    vis_fmul8x16au(vis_read_lo(s2), k2);

					d0 = buffd[2 * i];
					d1 = buffd[2 * i + 1];
					d0 = vis_fpadd16(d00, d0);
					d0 = vis_fpadd16(d10, d0);
					d0 = vis_fpadd16(d20, d0);
					d1 = vis_fpadd16(d01, d1);
					d1 = vis_fpadd16(d11, d1);
					d1 = vis_fpadd16(d21, d1);
					buffd[2 * i] = d0;
					buffd[2 * i + 1] = d1;
				}
			}
		}

		k0 = pk[ik_last];
		k1 = pk[ik_last + KSIZE];
		k2 = pk[ik_last + 2 * KSIZE];

		off = ik_last * NCHAN;
		doff = off / 8;
		off &= 7;
		buff0 = pbuff0 + doff;
		buff1 = pbuff1 + doff;
		buff2 = pbuff2 + doff;
		vis_write_gsr(gsr_scale + off);

		vis_write_gsr(gsr_scale + 7);

#pragma pipeloop(0)
		for (i = 0; i < dsize; i += 3) {
			mlib_d64 d00, d01, d02, d03, d04, d05;
			mlib_d64 d10, d11, d12, d13, d14, d15;
			mlib_d64 d20, d21, d22, d23, d24, d25;
			mlib_d64 d0, d1, d2, d3, d4, d5;
			mlib_d64 s00 = buff0[i];
			mlib_d64 s01 = buff0[i + 1];
			mlib_d64 s02 = buff0[i + 2];
			mlib_d64 s10 = buff1[i];
			mlib_d64 s11 = buff1[i + 1];
			mlib_d64 s12 = buff1[i + 2];
			mlib_d64 s20 = buff2[i];
			mlib_d64 s21 = buff2[i + 1];
			mlib_d64 s22 = buff2[i + 2];

			d00 = vis_fmul8x16au(vis_read_hi(s00), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s00), k0);
			d02 = vis_fmul8x16au(vis_read_hi(s01), k0);
			d03 = vis_fmul8x16au(vis_read_lo(s01), k0);
			d04 = vis_fmul8x16au(vis_read_hi(s02), k0);
			d05 = vis_fmul8x16au(vis_read_lo(s02), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s10), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s10), k1);
			d12 = vis_fmul8x16au(vis_read_hi(s11), k1);
			d13 = vis_fmul8x16au(vis_read_lo(s11), k1);
			d14 = vis_fmul8x16au(vis_read_hi(s12), k1);
			d15 = vis_fmul8x16au(vis_read_lo(s12), k1);
			d20 = vis_fmul8x16au(vis_read_hi(s20), k2);
			d21 = vis_fmul8x16au(vis_read_lo(s20), k2);
			d22 = vis_fmul8x16au(vis_read_hi(s21), k2);
			d23 = vis_fmul8x16au(vis_read_lo(s21), k2);
			d24 = vis_fmul8x16au(vis_read_hi(s22), k2);
			d25 = vis_fmul8x16au(vis_read_lo(s22), k2);

			d0 = buffd[2 * i];
			d1 = buffd[2 * i + 1];
			d2 = buffd[2 * i + 2];
			d3 = buffd[2 * i + 3];
			d4 = buffd[2 * i + 4];
			d5 = buffd[2 * i + 5];
			d0 = vis_fpadd16(d0, d00);
			d0 = vis_fpadd16(d0, d10);
			d0 = vis_fpadd16(d0, d20);
			d1 = vis_fpadd16(d1, d01);
			d1 = vis_fpadd16(d1, d11);
			d1 = vis_fpadd16(d1, d21);
			d2 = vis_fpadd16(d2, d02);
			d2 = vis_fpadd16(d2, d12);
			d2 = vis_fpadd16(d2, d22);
			d3 = vis_fpadd16(d3, d03);
			d3 = vis_fpadd16(d3, d13);
			d3 = vis_fpadd16(d3, d23);
			d4 = vis_fpadd16(d4, d04);
			d4 = vis_fpadd16(d4, d14);
			d4 = vis_fpadd16(d4, d24);
			d5 = vis_fpadd16(d5, d05);
			d5 = vis_fpadd16(d5, d15);
			d5 = vis_fpadd16(d5, d25);

			buffe[i] = vis_fpack16_pair(d0, d1);
			buffe[i + 1] = vis_fpack16_pair(d2, d3);
			buffe[i + 2] = vis_fpack16_pair(d4, d5);

			buffd[2 * i] = drnd;
			buffd[2 * i + 1] = drnd;
			buffd[2 * i + 2] = drnd;
			buffd[2 * i + 3] = drnd;
			buffd[2 * i + 4] = drnd;
			buffd[2 * i + 5] = drnd;

			LOAD_SRC_NF();
		}

		vis_write_gsr(gsr_scale + (8 - NCHAN));

		d0 = buffn[0];

#pragma pipeloop(0)
		for (i = 0; i <= (xsize + 7) / 8; i++) {
			d1 = buffn[i];
			buffn[i] = vis_faligndata(d0, d1);
			d0 = d1;
		}

		EXT_LEFT((mlib_u8 *)buffn, 1, 0);
		EXT_RIGHT((mlib_u8 *)buffn, 1, 0);

		ColorTrue2IndexLine((void *)buffe, dl, wid, colormap);

		if (j < hgt - 3)
			sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	MAX_N	11

mlib_status CONV_FUNC_MxN
{
	mlib_d64 *buffs_local[3 * (MAX_N + 1)], **buffs = buffs_local, **buff;
	mlib_d64 *buff0, *buff1, *buff2, *buff3, *buffn, *buffd, *buffe;
	mlib_d64 s00, s01, s10, s11, s20, s21, s30, s31, s0, s1, s2, s3;
	mlib_d64 d00, d01, d10, d11, d20, d21, d30, d31;
	mlib_d64 d0, d1;
	mlib_s32 ik, jk, ik_last, jk_size, coff, off, doff;
	mlib_s32 shift, off_shift;
	mlib_u8  *buffnn;
	DEF_VARS;
	DEF_EXTRA_VARS;

	if (n > MAX_N) {
		buffs = __mlib_malloc(3 * (n + 1) * sizeof (mlib_d64 *));

		if (buffs == NULL)
			return (MLIB_FAILURE);
	}
	buff = buffs + 2 * (n + 1);

	sl = adr_src;
	dl = adr_dst;

	ssize = NCHAN * (wid + (m - 1));
	dsize = (NCHAN * (wid + m - 1 - dx_l - dx_r) + 7) / 8;
	esize = (ssize + 7) / 8 + 4;
	pbuff = __mlib_malloc((n + 4) * esize * sizeof (mlib_d64));

	if (pbuff == NULL) {
		if (buffs != buffs_local)
			__mlib_free(buffs);
		return (MLIB_FAILURE);
	}

	for (i = 0; i < (n + 1); i++)
		buffs[i] = pbuff + i * esize;
	for (i = 0; i < (n + 1); i++)
		buffs[(n + 1) + i] = buffs[i];
	buffd = buffs[n] + esize;
	buffe = buffd + 2 * esize;

	shift = (NCHAN * dx_l) >> 3;
	off_shift = (NCHAN * dx_l) & 7;

	for (l = 0; l < n; l++) {
		mlib_d64 *buffn = buffs[l] + shift;

		sp = sl;

		vis_write_gsr(gsr_scale + 7);

#pragma pipeloop(0)
		for (i = 0; i < dsize; i += 3) {
			LOAD_SRC_NF();
		}

		if (off_shift != 0) {
			vis_write_gsr(gsr_scale + (8 - off_shift));

			d0 = buffn[0];

#pragma pipeloop(0)
			for (i = 0; i <= dsize; i++) {
				d1 = buffn[i];
				buffn[i] = vis_faligndata(d0, d1);
				d0 = d1;
			}
		}

		buffn -= shift;

		for (i = 0; i < dx_l; i++) {
			EXT_LEFT((mlib_u8 *)buffn, dx_l, i);
		}

		buffnn = (mlib_u8*)buffn + NCHAN*(wid + m - 1 - dx_r);
		for (i = 0; i < dx_r; i++) {
			buffnn[NCHAN * i    ] = buffnn[-NCHAN    ];
			buffnn[NCHAN * i + 1] = buffnn[-NCHAN + 1];
			buffnn[NCHAN * i + 2] = buffnn[-NCHAN + 2];
#if NCHAN == 4
			buffnn[NCHAN * i + 3] = buffnn[-NCHAN + 3];
#endif
		}

		if ((l >= dy_t) && (l < hgt + n - dy_b - 2))
			sl += sll;
	}

/* init buffer */
#pragma pipeloop(0)
	for (i = 0; i < dsize; i++) {
		buffd[2 * i] = drnd;
		buffd[2 * i + 1] = drnd;
	}

	for (j = 0; j < hgt; j++) {
		mlib_d64 **buffc = buffs + buff_ind;
		mlib_f32 *pk = karr, k0, k1, k2, k3;

		sp = sl;

		for (l = 0; l < n; l++) {
			buff[l] = buffc[l];
		}

		buffn = buffc[n] + shift;

		ik_last = 0;

		for (jk = 0; jk < n; jk += jk_size) {
			jk_size = n - jk;

			if (jk_size >= 5)
				jk_size = 3;

			if (jk_size == 4)
				jk_size = 2;

			coff = 0;

			if (jk_size == 2) {

				for (ik = 0; ik < m; ik++, coff += NCHAN) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];
					k1 = pk[ik + m];

					doff = coff / 8;
					buff0 = buff[jk] + doff;
					buff1 = buff[jk + 1] + doff;

					off = coff & 7;
					vis_write_gsr(gsr_scale + off);

					s01 = buff0[0];
					s11 = buff1[0];
#pragma pipeloop(0)
					for (i = 0; i < dsize; i++) {
						s00 = s01;
						s10 = s11;
						s01 = buff0[i + 1];
						s11 = buff1[i + 1];
						s0 = vis_faligndata(s00, s01);
						s1 = vis_faligndata(s10, s11);

						d00 =
						    vis_fmul8x16au(vis_read_hi
						    (s0), k0);
						d01 =
						    vis_fmul8x16au(vis_read_lo
						    (s0), k0);
						d10 =
						    vis_fmul8x16au(vis_read_hi
						    (s1), k1);
						d11 =
						    vis_fmul8x16au(vis_read_lo
						    (s1), k1);

						d0 = buffd[2 * i];
						d1 = buffd[2 * i + 1];
						d0 = vis_fpadd16(d00, d0);
						d0 = vis_fpadd16(d10, d0);
						d1 = vis_fpadd16(d01, d1);
						d1 = vis_fpadd16(d11, d1);
						buffd[2 * i] = d0;
						buffd[2 * i + 1] = d1;
					}
				}

				pk += 2 * m;
			} else if (jk_size == 3) {

				for (ik = 0; ik < m; ik++, coff += NCHAN) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];
					k1 = pk[ik + m];
					k2 = pk[ik + 2 * m];

					doff = coff / 8;
					buff0 = buff[jk] + doff;
					buff1 = buff[jk + 1] + doff;
					buff2 = buff[jk + 2] + doff;

					off = coff & 7;
					vis_write_gsr(gsr_scale + off);

					if (off == 0) {
#pragma pipeloop(0)
						for (i = 0; i < dsize; i++) {
							d0 = buffd[2 * i];
							d1 = buffd[2 * i + 1];

							s0 = buff0[i];
							s1 = buff1[i];
							s2 = buff2[i];

							d00 =
							    vis_fmul8x16au
							    (vis_read_hi(s0),
							    k0);
							d01 =
							    vis_fmul8x16au
							    (vis_read_lo(s0),
							    k0);
							d10 =
							    vis_fmul8x16au
							    (vis_read_hi(s1),
							    k1);
							d11 =
							    vis_fmul8x16au
							    (vis_read_lo(s1),
							    k1);
							d20 =
							    vis_fmul8x16au
							    (vis_read_hi(s2),
							    k2);
							d21 =
							    vis_fmul8x16au
							    (vis_read_lo(s2),
							    k2);

							d00 =
							    vis_fpadd16(d00,
							    d10);
							d0 = vis_fpadd16(d20,
							    d0);
							d0 = vis_fpadd16(d00,
							    d0);
							d01 =
							    vis_fpadd16(d01,
							    d11);
							d1 = vis_fpadd16(d21,
							    d1);
							d1 = vis_fpadd16(d01,
							    d1);
							buffd[2 * i] = d0;
							buffd[2 * i + 1] = d1;
						}
					} else if (off == 4) {
						s01 = buff0[0];
						s11 = buff1[0];
						s21 = buff2[0];
#pragma pipeloop(0)
						for (i = 0; i < dsize; i++) {
							d0 = buffd[2 * i];
							d1 = buffd[2 * i + 1];

							s00 = s01;
							s10 = s11;
							s20 = s21;
							s01 = buff0[i + 1];
							s11 = buff1[i + 1];
							s21 = buff2[i + 1];

							d00 =
							    vis_fmul8x16au
							    (vis_read_lo(s00),
							    k0);
							d01 =
							    vis_fmul8x16au
							    (vis_read_hi(s01),
							    k0);
							d10 =
							    vis_fmul8x16au
							    (vis_read_lo(s10),
							    k1);
							d11 =
							    vis_fmul8x16au
							    (vis_read_hi(s11),
							    k1);
							d20 =
							    vis_fmul8x16au
							    (vis_read_lo(s20),
							    k2);
							d21 =
							    vis_fmul8x16au
							    (vis_read_hi(s21),
							    k2);

							d00 =
							    vis_fpadd16(d00,
							    d10);
							d0 = vis_fpadd16(d20,
							    d0);
							d0 = vis_fpadd16(d00,
							    d0);
							d01 =
							    vis_fpadd16(d01,
							    d11);
							d1 = vis_fpadd16(d21,
							    d1);
							d1 = vis_fpadd16(d01,
							    d1);
							buffd[2 * i] = d0;
							buffd[2 * i + 1] = d1;
						}
					} else {
						s01 = buff0[0];
						s11 = buff1[0];
						s21 = buff2[0];
#pragma pipeloop(0)
						for (i = 0; i < dsize; i++) {
							d0 = buffd[2 * i];
							d1 = buffd[2 * i + 1];

							s00 = s01;
							s10 = s11;
							s20 = s21;
							s01 = buff0[i + 1];
							s11 = buff1[i + 1];
							s21 = buff2[i + 1];
							s0 = vis_faligndata(s00,
							    s01);
							s1 = vis_faligndata(s10,
							    s11);
							s2 = vis_faligndata(s20,
							    s21);

							d00 =
							    vis_fmul8x16au
							    (vis_read_hi(s0),
							    k0);
							d01 =
							    vis_fmul8x16au
							    (vis_read_lo(s0),
							    k0);
							d10 =
							    vis_fmul8x16au
							    (vis_read_hi(s1),
							    k1);
							d11 =
							    vis_fmul8x16au
							    (vis_read_lo(s1),
							    k1);
							d20 =
							    vis_fmul8x16au
							    (vis_read_hi(s2),
							    k2);
							d21 =
							    vis_fmul8x16au
							    (vis_read_lo(s2),
							    k2);

							d00 =
							    vis_fpadd16(d00,
							    d10);
							d0 = vis_fpadd16(d20,
							    d0);
							d0 = vis_fpadd16(d00,
							    d0);
							d01 =
							    vis_fpadd16(d01,
							    d11);
							d1 = vis_fpadd16(d21,
							    d1);
							d1 = vis_fpadd16(d01,
							    d1);
							buffd[2 * i] = d0;
							buffd[2 * i + 1] = d1;
						}
					}
				}

				pk += 3 * m;
			} else {
/* jk_size == 4 */

				for (ik = 0; ik < m; ik++, coff += NCHAN) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];
					k1 = pk[ik + m];
					k2 = pk[ik + 2 * m];
					k3 = pk[ik + 3 * m];

					doff = coff / 8;
					buff0 = buff[jk] + doff;
					buff1 = buff[jk + 1] + doff;
					buff2 = buff[jk + 2] + doff;
					buff3 = buff[jk + 3] + doff;

					off = coff & 7;
					vis_write_gsr(gsr_scale + off);

					if (off == 0) {

#pragma pipeloop(0)
						for (i = 0; i < dsize; i++) {
							d0 = buffd[2 * i];
							d1 = buffd[2 * i + 1];

							s0 = buff0[i];
							s1 = buff1[i];
							s2 = buff2[i];
							s3 = buff3[i];

							d00 =
							    vis_fmul8x16au
							    (vis_read_hi(s0),
							    k0);
							d01 =
							    vis_fmul8x16au
							    (vis_read_lo(s0),
							    k0);
							d10 =
							    vis_fmul8x16au
							    (vis_read_hi(s1),
							    k1);
							d11 =
							    vis_fmul8x16au
							    (vis_read_lo(s1),
							    k1);
							d20 =
							    vis_fmul8x16au
							    (vis_read_hi(s2),
							    k2);
							d21 =
							    vis_fmul8x16au
							    (vis_read_lo(s2),
							    k2);
							d30 =
							    vis_fmul8x16au
							    (vis_read_hi(s3),
							    k3);
							d31 =
							    vis_fmul8x16au
							    (vis_read_lo(s3),
							    k3);

							d00 =
							    vis_fpadd16(d00,
							    d10);
							d20 =
							    vis_fpadd16(d20,
							    d30);
							d0 = vis_fpadd16(d0,
							    d00);
							d0 = vis_fpadd16(d0,
							    d20);
							d01 =
							    vis_fpadd16(d01,
							    d11);
							d21 =
							    vis_fpadd16(d21,
							    d31);
							d1 = vis_fpadd16(d1,
							    d01);
							d1 = vis_fpadd16(d1,
							    d21);
							buffd[2 * i] = d0;
							buffd[2 * i + 1] = d1;
						}
					} else if (off == 4) {

						s01 = buff0[0];
						s11 = buff1[0];
						s21 = buff2[0];
						s31 = buff3[0];
#pragma pipeloop(0)
						for (i = 0; i < dsize; i++) {
							d0 = buffd[2 * i];
							d1 = buffd[2 * i + 1];

							s00 = s01;
							s10 = s11;
							s20 = s21;
							s30 = s31;
							s01 = buff0[i + 1];
							s11 = buff1[i + 1];
							s21 = buff2[i + 1];
							s31 = buff3[i + 1];

							d00 =
							    vis_fmul8x16au
							    (vis_read_lo(s00),
							    k0);
							d01 =
							    vis_fmul8x16au
							    (vis_read_hi(s01),
							    k0);
							d10 =
							    vis_fmul8x16au
							    (vis_read_lo(s10),
							    k1);
							d11 =
							    vis_fmul8x16au
							    (vis_read_hi(s11),
							    k1);
							d20 =
							    vis_fmul8x16au
							    (vis_read_lo(s20),
							    k2);
							d21 =
							    vis_fmul8x16au
							    (vis_read_hi(s21),
							    k2);
							d30 =
							    vis_fmul8x16au
							    (vis_read_lo(s30),
							    k3);
							d31 =
							    vis_fmul8x16au
							    (vis_read_hi(s31),
							    k3);

							d00 =
							    vis_fpadd16(d00,
							    d10);
							d20 =
							    vis_fpadd16(d20,
							    d30);
							d0 = vis_fpadd16(d0,
							    d00);
							d0 = vis_fpadd16(d0,
							    d20);
							d01 =
							    vis_fpadd16(d01,
							    d11);
							d21 =
							    vis_fpadd16(d21,
							    d31);
							d1 = vis_fpadd16(d1,
							    d01);
							d1 = vis_fpadd16(d1,
							    d21);
							buffd[2 * i] = d0;
							buffd[2 * i + 1] = d1;
						}
					} else {

						s01 = buff0[0];
						s11 = buff1[0];
						s21 = buff2[0];
						s31 = buff3[0];
#pragma pipeloop(0)
						for (i = 0; i < dsize; i++) {
							d0 = buffd[2 * i];
							d1 = buffd[2 * i + 1];

							s00 = s01;
							s10 = s11;
							s20 = s21;
							s30 = s31;
							s01 = buff0[i + 1];
							s11 = buff1[i + 1];
							s21 = buff2[i + 1];
							s31 = buff3[i + 1];
							s0 = vis_faligndata(s00,
							    s01);
							s1 = vis_faligndata(s10,
							    s11);
							s2 = vis_faligndata(s20,
							    s21);
							s3 = vis_faligndata(s30,
							    s31);

							d00 =
							    vis_fmul8x16au
							    (vis_read_hi(s0),
							    k0);
							d01 =
							    vis_fmul8x16au
							    (vis_read_lo(s0),
							    k0);
							d10 =
							    vis_fmul8x16au
							    (vis_read_hi(s1),
							    k1);
							d11 =
							    vis_fmul8x16au
							    (vis_read_lo(s1),
							    k1);
							d20 =
							    vis_fmul8x16au
							    (vis_read_hi(s2),
							    k2);
							d21 =
							    vis_fmul8x16au
							    (vis_read_lo(s2),
							    k2);
							d30 =
							    vis_fmul8x16au
							    (vis_read_hi(s3),
							    k3);
							d31 =
							    vis_fmul8x16au
							    (vis_read_lo(s3),
							    k3);

							d00 =
							    vis_fpadd16(d00,
							    d10);
							d20 =
							    vis_fpadd16(d20,
							    d30);
							d0 = vis_fpadd16(d0,
							    d00);
							d0 = vis_fpadd16(d0,
							    d20);
							d01 =
							    vis_fpadd16(d01,
							    d11);
							d21 =
							    vis_fpadd16(d21,
							    d31);
							d1 = vis_fpadd16(d1,
							    d01);
							d1 = vis_fpadd16(d1,
							    d21);
							buffd[2 * i] = d0;
							buffd[2 * i + 1] = d1;
						}
					}
				}

				pk += 4 * m;
			}
		}

/* ******************* Final iteration ****************** */

		jk_size = n;

		if (jk_size >= 5)
			jk_size = 3;

		if (jk_size == 4)
			jk_size = 2;

		k0 = karr[ik_last];
		k1 = karr[ik_last + m];
		k2 = karr[ik_last + 2 * m];

		off = ik_last * NCHAN;
		doff = off / 8;
		off &= 7;
		buff0 = buff[0] + doff;
		buff1 = buff[1] + doff;
		buff2 = buff[2] + doff;
		vis_write_gsr(gsr_scale + off);

		if (jk_size == 2) {
			vis_write_gsr(gsr_scale + 7);

#pragma pipeloop(0)
			for (i = 0; i < dsize; i += 3) {
				mlib_d64 d00, d01, d02, d03, d04, d05;
				mlib_d64 d10, d11, d12, d13, d14, d15;
				mlib_d64 d0, d1, d2, d3, d4, d5;
				mlib_d64 s00 = buff0[i];
				mlib_d64 s01 = buff0[i + 1];
				mlib_d64 s02 = buff0[i + 2];
				mlib_d64 s10 = buff1[i];
				mlib_d64 s11 = buff1[i + 1];
				mlib_d64 s12 = buff1[i + 2];

				d00 = vis_fmul8x16au(vis_read_hi(s00), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s00), k0);
				d02 = vis_fmul8x16au(vis_read_hi(s01), k0);
				d03 = vis_fmul8x16au(vis_read_lo(s01), k0);
				d04 = vis_fmul8x16au(vis_read_hi(s02), k0);
				d05 = vis_fmul8x16au(vis_read_lo(s02), k0);
				d10 = vis_fmul8x16au(vis_read_hi(s10), k1);
				d11 = vis_fmul8x16au(vis_read_lo(s10), k1);
				d12 = vis_fmul8x16au(vis_read_hi(s11), k1);
				d13 = vis_fmul8x16au(vis_read_lo(s11), k1);
				d14 = vis_fmul8x16au(vis_read_hi(s12), k1);
				d15 = vis_fmul8x16au(vis_read_lo(s12), k1);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d2 = buffd[2 * i + 2];
				d3 = buffd[2 * i + 3];
				d4 = buffd[2 * i + 4];
				d5 = buffd[2 * i + 5];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);
				d2 = vis_fpadd16(d2, d02);
				d2 = vis_fpadd16(d2, d12);
				d3 = vis_fpadd16(d3, d03);
				d3 = vis_fpadd16(d3, d13);
				d4 = vis_fpadd16(d4, d04);
				d4 = vis_fpadd16(d4, d14);
				d5 = vis_fpadd16(d5, d05);
				d5 = vis_fpadd16(d5, d15);

				buffe[i] = vis_fpack16_pair(d0, d1);
				buffe[i + 1] = vis_fpack16_pair(d2, d3);
				buffe[i + 2] = vis_fpack16_pair(d4, d5);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
				buffd[2 * i + 2] = drnd;
				buffd[2 * i + 3] = drnd;
				buffd[2 * i + 4] = drnd;
				buffd[2 * i + 5] = drnd;

				LOAD_SRC_NF();
			}
		} else {
/* if (jk_size == 3) */

			vis_write_gsr(gsr_scale + 7);

#pragma pipeloop(0)
			for (i = 0; i < dsize; i += 3) {
				mlib_d64 d00, d01, d02, d03, d04, d05;
				mlib_d64 d10, d11, d12, d13, d14, d15;
				mlib_d64 d20, d21, d22, d23, d24, d25;
				mlib_d64 d0, d1, d2, d3, d4, d5;
				mlib_d64 s00 = buff0[i];
				mlib_d64 s01 = buff0[i + 1];
				mlib_d64 s02 = buff0[i + 2];
				mlib_d64 s10 = buff1[i];
				mlib_d64 s11 = buff1[i + 1];
				mlib_d64 s12 = buff1[i + 2];
				mlib_d64 s20 = buff2[i];
				mlib_d64 s21 = buff2[i + 1];
				mlib_d64 s22 = buff2[i + 2];

				d00 = vis_fmul8x16au(vis_read_hi(s00), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s00), k0);
				d02 = vis_fmul8x16au(vis_read_hi(s01), k0);
				d03 = vis_fmul8x16au(vis_read_lo(s01), k0);
				d04 = vis_fmul8x16au(vis_read_hi(s02), k0);
				d05 = vis_fmul8x16au(vis_read_lo(s02), k0);
				d10 = vis_fmul8x16au(vis_read_hi(s10), k1);
				d11 = vis_fmul8x16au(vis_read_lo(s10), k1);
				d12 = vis_fmul8x16au(vis_read_hi(s11), k1);
				d13 = vis_fmul8x16au(vis_read_lo(s11), k1);
				d14 = vis_fmul8x16au(vis_read_hi(s12), k1);
				d15 = vis_fmul8x16au(vis_read_lo(s12), k1);
				d20 = vis_fmul8x16au(vis_read_hi(s20), k2);
				d21 = vis_fmul8x16au(vis_read_lo(s20), k2);
				d22 = vis_fmul8x16au(vis_read_hi(s21), k2);
				d23 = vis_fmul8x16au(vis_read_lo(s21), k2);
				d24 = vis_fmul8x16au(vis_read_hi(s22), k2);
				d25 = vis_fmul8x16au(vis_read_lo(s22), k2);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d2 = buffd[2 * i + 2];
				d3 = buffd[2 * i + 3];
				d4 = buffd[2 * i + 4];
				d5 = buffd[2 * i + 5];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d0 = vis_fpadd16(d0, d20);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);
				d1 = vis_fpadd16(d1, d21);
				d2 = vis_fpadd16(d2, d02);
				d2 = vis_fpadd16(d2, d12);
				d2 = vis_fpadd16(d2, d22);
				d3 = vis_fpadd16(d3, d03);
				d3 = vis_fpadd16(d3, d13);
				d3 = vis_fpadd16(d3, d23);
				d4 = vis_fpadd16(d4, d04);
				d4 = vis_fpadd16(d4, d14);
				d4 = vis_fpadd16(d4, d24);
				d5 = vis_fpadd16(d5, d05);
				d5 = vis_fpadd16(d5, d15);
				d5 = vis_fpadd16(d5, d25);

				buffe[i] = vis_fpack16_pair(d0, d1);
				buffe[i + 1] = vis_fpack16_pair(d2, d3);
				buffe[i + 2] = vis_fpack16_pair(d4, d5);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
				buffd[2 * i + 2] = drnd;
				buffd[2 * i + 3] = drnd;
				buffd[2 * i + 4] = drnd;
				buffd[2 * i + 5] = drnd;

				LOAD_SRC_NF();
			}
		}

		if (off_shift != 0) {
			vis_write_gsr(gsr_scale + (8 - off_shift));

			d0 = buffn[0];

#pragma pipeloop(0)
			for (i = 0; i <= dsize; i++) {
				d1 = buffn[i];
				buffn[i] = vis_faligndata(d0, d1);
				d0 = d1;
			}
		}

		buffn -= shift;

		for (i = 0; i < dx_l; i++) {
			EXT_LEFT((mlib_u8 *)buffn, dx_l, i);
		}

		buffnn = (mlib_u8*)buffn + NCHAN*(wid + m - 1 - dx_r);
		for (i = 0; i < dx_r; i++) {
			buffnn[NCHAN * i    ] = buffnn[-NCHAN    ];
			buffnn[NCHAN * i + 1] = buffnn[-NCHAN + 1];
			buffnn[NCHAN * i + 2] = buffnn[-NCHAN + 2];
#if NCHAN == 4
			buffnn[NCHAN * i + 3] = buffnn[-NCHAN + 3];
#endif
		}

		ColorTrue2IndexLine((void *)buffe, dl, wid, colormap);

		if (j < hgt - dy_b - 2)
			sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= (n + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	if (buffs != buffs_local)
		__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
