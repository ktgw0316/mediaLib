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

#pragma ident	"@(#)mlib_v_ImageConv_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageConv* on U8 type
 *      and MLIB_EDGE_DST_NO_WRITE mask
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageConv.h>
#include <mlib_v_ImageChannelInsert.h>
#include <mlib_v_ImageChannelInsert_2.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_v_ImageChannelExtract_2.h>

/* *********************************************************** */

typedef void (*fun_load_store) (
	const mlib_u8 *,
	mlib_u8 *,
	mlib_s32,
	mlib_s32);

/* *********************************************************** */

static void
mlib_my_load(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 size,
    mlib_s32 cmask)
{
	mlib_ImageCopy_na(src, dst, size);
}

/* *********************************************************** */

static const fun_load_store mlib_fun_load[] = {
	mlib_my_load, mlib_v_ImageChannelExtract_U8_21_D1,
	mlib_v_ImageChannelExtract_U8_31_D1,
	    mlib_v_ImageChannelExtract_U8_41_D1,
	mlib_my_load, mlib_my_load,
	mlib_v_ImageChannelExtract_U8_32_D1,
	    mlib_v_ImageChannelExtract_U8_42_D1,
	mlib_my_load, mlib_my_load,
	mlib_my_load, mlib_v_ImageChannelExtract_U8_43_D1,
	mlib_my_load
};

/* *********************************************************** */

static const fun_load_store mlib_fun_store[] = {
	mlib_my_load, mlib_v_ImageChannelInsert_U8_12_D1,
	mlib_v_ImageChannelInsert_U8_13_D1, mlib_v_ImageChannelInsert_U8_14_D1,
	mlib_my_load, mlib_my_load,
	mlib_v_ImageChannelInsert_U8_23_D1, mlib_v_ImageChannelInsert_U8_24_D1,
	mlib_my_load, mlib_my_load,
	mlib_my_load, mlib_v_ImageChannelInsert_U8_34_D1,
	mlib_my_load
};

/* *********************************************************** */

static const mlib_s32 mlib_round_8[16] = {
	0x00400040, 0x00200020, 0x00100010, 0x00080008,
	0x00040004, 0x00020002, 0x00010001, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	LTYPE	mlib_u8

/* *********************************************************** */

#define	CONV_FUNC(KERN)                                                    \
	mlib_conv##KERN##_8nw_f(mlib_image *dst,                           \
	    const mlib_image *src, const mlib_s32 *kern, mlib_s32 scale,   \
	    mlib_s32 cmask)

/* *********************************************************** */

#define	NCHAN	bitcount

/* *********************************************************** */

#define	DEF_VARS                                                      \
	DTYPE    *sl, *sp, *dl;                                       \
	mlib_s32 hgt = mlib_ImageGetHeight(src);                      \
	mlib_s32 wid = mlib_ImageGetWidth(src);                       \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (DTYPE);     \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (DTYPE);     \
	DTYPE    *adr_src = (DTYPE *)mlib_ImageGetData(src);          \
	DTYPE    *adr_dst = (DTYPE *)mlib_ImageGetData(dst);          \
	mlib_s32 ssize, xsize, dsize, esize, emask, buff_ind = 0;     \
	mlib_d64 *pbuff, *dp;                                         \
	mlib_f32 *karr = (mlib_f32 *)kern;                            \
	mlib_s32 gsr_scale = (31 - scale) << 3;                       \
	mlib_d64 drnd = vis_to_double_dup(mlib_round_8[31 - scale]);  \
	mlib_s32 i, j, l;                                             \
	mlib_s32 nchan = mlib_ImageGetChannels(dst);                  \
	mlib_s32 bitcount = 0, ncmask = 0, extract, fun_index;        \
	fun_load_store fun_load;                                      \
	fun_load_store fun_store

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

/* normalize the cmask, and count the number of bit with value 1 */
	for (i = (nchan - 1); i >= 0; i--) {
		if (((cmask & (1 << i)) != 0)) {
			ncmask += (1 << i);
			bitcount++;
		}
	}

	extract = 1;

	if (bitcount == nchan)
		extract = 0;

	fun_index = (bitcount - 1) * 4 + (nchan - 1);

	if (fun_index > 12)
		fun_index = 12;

	fun_load = mlib_fun_load[fun_index];
	fun_store = mlib_fun_store[fun_index];

	sl = adr_src;
	dl = adr_dst;

	ssize = wid;
	dsize = (NCHAN * ssize + 7) / 8;
	esize = dsize + 4;
	pbuff = __mlib_malloc((KSIZE + 4) * esize * sizeof (mlib_d64));

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++)
		buffs[i] = pbuff + i * esize;
	for (i = 0; i < (KSIZE + 1); i++)
		buffs[(KSIZE + 1) + i] = buffs[i];
	buffd = buffs[KSIZE] + esize;
	buffe = buffd + 2 * esize;

	wid -= (KSIZE - 1);
	hgt -= (KSIZE - 1);
	xsize = NCHAN * wid;
	emask = (0xFF00 >> (xsize & 7)) & 0xFF;

	if (extract == 0) {
		wid = xsize;
		ssize *= NCHAN;
	}

	for (l = 0; l < KSIZE; l++) {
		mlib_d64 *buffn = buffs[l];

		sp = sl + l * sll;

		if (((mlib_addr)sp & 7) || (extract))
			fun_load((mlib_u8 *)sp, (mlib_u8 *)buffn, ssize,
			    ncmask);
	}

	for (j = 0; j < hgt; j++) {
		mlib_d64 **buffc = buffs + buff_ind;
		mlib_f32 *pk = karr, k0, k1;

		sp = sl + KSIZE * sll;

		buff0 = buffc[0];
		buff1 = buffc[1];
		buffn = buffc[KSIZE];

		if (((((mlib_addr)(sl)) & 7) == 0) && (extract == 0))
			buff0 = (mlib_d64 *)sl;

		if (((((mlib_addr)(sl + sll)) & 7) == 0) && (extract == 0))
			buff1 = (mlib_d64 *)(sl + sll);

		if (((mlib_addr)sp & 7) || (extract))
			fun_load((mlib_u8 *)sp, (mlib_u8 *)buffn, ssize,
			    ncmask);

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

		dp = (((mlib_addr)dl & 7) ||
		    (extract)) ? buffe : (mlib_d64 *)dl;

#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = buff0[i];
			s1 = buff1[i];

			d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s1), k1);

			d0 = buffd[2 * i];
			d1 = buffd[2 * i + 1];
			d00 = vis_fpadd16(d00, d10);
			d0 = vis_fpadd16(d0, drnd);
			d0 = vis_fpadd16(d0, d00);
			d01 = vis_fpadd16(d01, d11);
			d1 = vis_fpadd16(d1, drnd);
			d1 = vis_fpadd16(d1, d01);
			dp[i] = vis_fpack16_pair(d0, d1);
		}

		if (emask) {
			s0 = buff0[i];
			s1 = buff1[i];

			d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s1), k1);

			d0 = buffd[2 * i];
			d1 = buffd[2 * i + 1];

			d00 = vis_fpadd16(d00, d10);
			d0 = vis_fpadd16(d0, drnd);
			d0 = vis_fpadd16(d0, d00);
			d01 = vis_fpadd16(d01, d11);
			d1 = vis_fpadd16(d1, drnd);
			d1 = vis_fpadd16(d1, d01);

			d0 = vis_fpack16_pair(d0, d1);
			vis_pst_8(d0, dp + i, emask);
		}

		if ((mlib_u8 *)dp != dl)
			fun_store((mlib_u8 *)buffe, dl, wid, ncmask);

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
	mlib_d64 dd, d0, d1, d00, d01, d10, d11, d20, d21;
	mlib_s32 ik, ik_last, off, doff;

	DEF_VARS;

/* normalize the cmask, and count the number of bit with value 1 */
	for (i = (nchan - 1); i >= 0; i--) {
		if (((cmask & (1 << i)) != 0)) {
			ncmask += (1 << i);
			bitcount++;
		}
	}

	extract = 1;

	if (bitcount == nchan)
		extract = 0;

	fun_index = (bitcount - 1) * 4 + (nchan - 1);

	if (fun_index > 12)
		fun_index = 12;

	fun_load = mlib_fun_load[fun_index];
	fun_store = mlib_fun_store[fun_index];

	sl = adr_src;
	dl = adr_dst + ((KSIZE - 1) / 2) * (dll + nchan);

	ssize = wid;
	dsize = (NCHAN * ssize + 7) / 8;
	esize = dsize + 4;
	pbuff = __mlib_malloc((KSIZE + 4) * esize * sizeof (mlib_d64));

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++)
		buffs[i] = pbuff + i * esize;
	for (i = 0; i < (KSIZE + 1); i++)
		buffs[(KSIZE + 1) + i] = buffs[i];
	buffd = buffs[KSIZE] + esize;
	buffe = buffd + 2 * esize;

	wid -= (KSIZE - 1);
	hgt -= (KSIZE - 1);
	xsize = NCHAN * wid;
	emask = (0xFF00 >> (xsize & 7)) & 0xFF;

	if (extract == 0) {
		wid = xsize;
		ssize *= NCHAN;
	}

	for (l = 0; l < KSIZE; l++) {
		mlib_d64 *buffn = buffs[l];

		sp = sl + l * sll;

		if (((mlib_addr)sp & 7) || (extract))
			fun_load((mlib_u8 *)sp, (mlib_u8 *)buffn, ssize,
			    ncmask);
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

		sp = sl + KSIZE * sll;

		pbuff0 = buffc[0];
		pbuff1 = buffc[1];
		pbuff2 = buffc[2];
		buffn = buffc[KSIZE];

		if (((((mlib_addr)(sl)) & 7) == 0) && (extract == 0))
			pbuff0 = (mlib_d64 *)sl;

		if (((((mlib_addr)(sl + sll)) & 7) == 0) && (extract == 0))
			pbuff1 = (mlib_d64 *)(sl + sll);

		if (((((mlib_addr)(sl + 2 * sll)) & 7) == 0) && (extract == 0))
			pbuff2 = (mlib_d64 *)(sl + 2 * sll);

		if (((mlib_addr)sp & 7) || (extract))
			fun_load((mlib_u8 *)sp, (mlib_u8 *)buffn, ssize,
			    ncmask);

		ik_last = (KSIZE - 1);

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

		dp = (((mlib_addr)dl & 7) ||
		    (extract)) ? buffe : (mlib_d64 *)dl;

		s01 = buff0[0];
		s11 = buff1[0];
		s21 = buff2[0];
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s00 = s01;
			s10 = s11;
			s20 = s21;
			s01 = buff0[i + 1];
			s11 = buff1[i + 1];
			s21 = buff2[i + 1];
			s0 = vis_faligndata(s00, s01);
			s1 = vis_faligndata(s10, s11);
			s2 = vis_faligndata(s20, s21);

			d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s1), k1);
			d20 = vis_fmul8x16au(vis_read_hi(s2), k2);
			d21 = vis_fmul8x16au(vis_read_lo(s2), k2);

			d0 = buffd[2 * i];
			d1 = buffd[2 * i + 1];
			d0 = vis_fpadd16(d0, d00);
			d0 = vis_fpadd16(d0, d10);
			d0 = vis_fpadd16(d0, d20);
			d1 = vis_fpadd16(d1, d01);
			d1 = vis_fpadd16(d1, d11);
			d1 = vis_fpadd16(d1, d21);

			dd = vis_fpack16_pair(d0, d1);
			dp[i] = dd;

			buffd[2 * i] = drnd;
			buffd[2 * i + 1] = drnd;
		}

		if (emask) {
			s00 = s01;
			s10 = s11;
			s20 = s21;
			s01 = buff0[i + 1];
			s11 = buff1[i + 1];
			s21 = buff2[i + 1];
			s0 = vis_faligndata(s00, s01);
			s1 = vis_faligndata(s10, s11);
			s2 = vis_faligndata(s20, s21);

			d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
			d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
			d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
			d11 = vis_fmul8x16au(vis_read_lo(s1), k1);
			d20 = vis_fmul8x16au(vis_read_hi(s2), k2);
			d21 = vis_fmul8x16au(vis_read_lo(s2), k2);

			d0 = buffd[2 * i];
			d1 = buffd[2 * i + 1];
			d0 = vis_fpadd16(d0, d00);
			d0 = vis_fpadd16(d0, d10);
			d0 = vis_fpadd16(d0, d20);
			d1 = vis_fpadd16(d1, d01);
			d1 = vis_fpadd16(d1, d11);
			d1 = vis_fpadd16(d1, d21);

			dd = vis_fpack16_pair(d0, d1);
			vis_pst_8(dd, dp + i, emask);

			buffd[2 * i] = drnd;
			buffd[2 * i + 1] = drnd;
		}

		if ((mlib_u8 *)dp != dl)
			fun_store((mlib_u8 *)buffe, dl, wid, ncmask);

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

mlib_status
mlib_convMxN_8nw_f(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask)
{
	mlib_d64 *buffs_local[3 * (MAX_N + 1)], **buffs = buffs_local, **buff;
	mlib_d64 *buff0, *buff1, *buff2, *buff3, *buffn, *buffd, *buffe;
	mlib_d64 s00, s01, s10, s11, s20, s21, s30, s31, s0, s1, s2, s3;
	mlib_d64 d00, d01, d10, d11, d20, d21, d30, d31;
	mlib_d64 dd, d0, d1;
	mlib_s32 ik, jk, ik_last, jk_size, coff, off, doff;

	DEF_VARS;

	if (n > MAX_N) {
		buffs = __mlib_malloc(3 * (n + 1) * sizeof (mlib_d64 *));

		if (buffs == NULL)
			return (MLIB_FAILURE);
	}

	buff = buffs + 2 * (n + 1);

/* normalize the cmask, and count the number of bit with value 1 */
	for (i = (nchan - 1); i >= 0; i--) {
		if (((cmask & (1 << i)) != 0)) {
			ncmask += (1 << i);
			bitcount++;
		}
	}

	extract = 1;

	if (bitcount == nchan)
		extract = 0;

	fun_index = (bitcount - 1) * 4 + (nchan - 1);

	if (fun_index > 12)
		fun_index = 12;

	fun_load = mlib_fun_load[fun_index];
	fun_store = mlib_fun_store[fun_index];

	sl = adr_src;
	dl = adr_dst + dn * dll + dm * nchan;

	ssize = wid;
	dsize = (NCHAN * ssize + 7) / 8;
	esize = dsize + 4;
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

	wid -= (m - 1);
	hgt -= (n - 1);
	xsize = NCHAN * wid;
	emask = (0xFF00 >> (xsize & 7)) & 0xFF;

	if (extract == 0) {
		wid = xsize;
		ssize *= NCHAN;
	}

	for (l = 0; l < n; l++) {
		mlib_d64 *buffn = buffs[l];

		sp = sl + l * sll;

		if (((mlib_addr)sp & 7) || (extract))
			fun_load((mlib_u8 *)sp, (mlib_u8 *)buffn, ssize,
			    ncmask);
	}

/* init buffer */
#pragma pipeloop(0)
	for (i = 0; i < (xsize + 7) / 8; i++) {
		buffd[2 * i] = drnd;
		buffd[2 * i + 1] = drnd;
	}

	for (j = 0; j < hgt; j++) {
		mlib_d64 **buffc = buffs + buff_ind;
		mlib_f32 *pk = karr, k0, k1, k2, k3;

		sp = sl + n * sll;

		for (l = 0; l < n; l++) {
			buff[l] = buffc[l];
		}

		buffn = buffc[n];

		for (l = 0; l < n; l++) {
			if (((((mlib_addr)(sl + l * sll)) & 7) == 0) &&
			    (extract == 0))
				buff[l] = (mlib_d64 *)(sl + l * sll);
		}

		if (((mlib_addr)sp & 7) || (extract))
			fun_load((mlib_u8 *)sp, (mlib_u8 *)buffn, ssize,
			    ncmask);

		ik_last = (m - 1);

		for (jk = 0; jk < n; jk += jk_size) {
			jk_size = n - jk;

			if (jk_size >= 6)
				jk_size = 4;

			if (jk_size == 5)
				jk_size = 3;

			coff = 0;

			if (jk_size == 1) {

				for (ik = 0; ik < m; ik++, coff += NCHAN) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];

					doff = coff / 8;
					buff0 = buff[jk] + doff;

					off = coff & 7;
					vis_write_gsr(gsr_scale + off);

					s01 = buff0[0];
#pragma pipeloop(0)
					for (i = 0; i < (xsize + 7) / 8; i++) {
						s00 = s01;
						s01 = buff0[i + 1];
						s0 = vis_faligndata(s00, s01);

						d00 =
						    vis_fmul8x16au(vis_read_hi
						    (s0), k0);
						d01 =
						    vis_fmul8x16au(vis_read_lo
						    (s0), k0);

						d0 = buffd[2 * i];
						d1 = buffd[2 * i + 1];
						d0 = vis_fpadd16(d00, d0);
						d1 = vis_fpadd16(d01, d1);
						buffd[2 * i] = d0;
						buffd[2 * i + 1] = d1;
					}
				}

				pk += m;
			} else if (jk_size == 2) {

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
					for (i = 0; i < (xsize + 7) / 8; i++) {
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
						for (i = 0; i < (xsize + 7) / 8;
						    i++) {
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
						for (i = 0; i < (xsize + 7) / 8;
						    i++) {
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
						for (i = 0; i < (xsize + 7) / 8;
						    i++) {
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
						for (i = 0; i < (xsize + 7) / 8;
						    i++) {
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
						for (i = 0; i < (xsize + 7) / 8;
						    i++) {
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
						for (i = 0; i < (xsize + 7) / 8;
						    i++) {
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

/*	**************** Final iteration *************** */

		jk_size = n;

		if (jk_size >= 6)
			jk_size = 4;

		if (jk_size == 5)
			jk_size = 3;

		k0 = karr[ik_last];
		k1 = karr[ik_last + m];
		k2 = karr[ik_last + 2 * m];
		k3 = karr[ik_last + 3 * m];

		off = ik_last * NCHAN;
		doff = off / 8;
		off &= 7;
		buff0 = buff[0] + doff;
		buff1 = buff[1] + doff;
		buff2 = buff[2] + doff;
		buff3 = buff[3] + doff;
		vis_write_gsr(gsr_scale + off);

		dp = (((mlib_addr)dl & 7) ||
		    (extract)) ? buffe : (mlib_d64 *)dl;

		if (jk_size == 1) {

			s01 = buff0[0];
#pragma pipeloop(0)
			for (i = 0; i < xsize / 8; i++) {
				s00 = s01;
				s01 = buff0[i + 1];
				s0 = vis_faligndata(s00, s01);

				d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s0), k0);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d1 = vis_fpadd16(d1, d01);

				dd = vis_fpack16_pair(d0, d1);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				s00 = s01;
				s01 = buff0[i + 1];
				s0 = vis_faligndata(s00, s01);

				d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s0), k0);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d1 = vis_fpadd16(d1, d01);

				dd = vis_fpack16_pair(d0, d1);
				vis_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}
		} else if (jk_size == 2) {

			s01 = buff0[0];
			s11 = buff1[0];
#pragma pipeloop(0)
			for (i = 0; i < xsize / 8; i++) {
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

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);

				dd = vis_fpack16_pair(d0, d1);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
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

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);

				dd = vis_fpack16_pair(d0, d1);
				vis_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}
		} else if (jk_size == 3) {

			s01 = buff0[0];
			s11 = buff1[0];
			s21 = buff2[0];
#pragma pipeloop(0)
			for (i = 0; i < xsize / 8; i++) {
				s00 = s01;
				s10 = s11;
				s20 = s21;
				s01 = buff0[i + 1];
				s11 = buff1[i + 1];
				s21 = buff2[i + 1];
				s0 = vis_faligndata(s00, s01);
				s1 = vis_faligndata(s10, s11);
				s2 = vis_faligndata(s20, s21);

				d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
				d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
				d11 = vis_fmul8x16au(vis_read_lo(s1), k1);
				d20 = vis_fmul8x16au(vis_read_hi(s2), k2);
				d21 = vis_fmul8x16au(vis_read_lo(s2), k2);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d0 = vis_fpadd16(d0, d20);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);
				d1 = vis_fpadd16(d1, d21);

				dd = vis_fpack16_pair(d0, d1);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				s00 = s01;
				s10 = s11;
				s20 = s21;
				s01 = buff0[i + 1];
				s11 = buff1[i + 1];
				s21 = buff2[i + 1];
				s0 = vis_faligndata(s00, s01);
				s1 = vis_faligndata(s10, s11);
				s2 = vis_faligndata(s20, s21);

				d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
				d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
				d11 = vis_fmul8x16au(vis_read_lo(s1), k1);
				d20 = vis_fmul8x16au(vis_read_hi(s2), k2);
				d21 = vis_fmul8x16au(vis_read_lo(s2), k2);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d0 = vis_fpadd16(d0, d20);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);
				d1 = vis_fpadd16(d1, d21);

				dd = vis_fpack16_pair(d0, d1);
				vis_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}
		} else {
/* if (jk_size == 4) */

			s01 = buff0[0];
			s11 = buff1[0];
			s21 = buff2[0];
			s31 = buff3[0];
#pragma pipeloop(0)
			for (i = 0; i < xsize / 8; i++) {
				s00 = s01;
				s10 = s11;
				s20 = s21;
				s30 = s31;
				s01 = buff0[i + 1];
				s11 = buff1[i + 1];
				s21 = buff2[i + 1];
				s31 = buff3[i + 1];
				s0 = vis_faligndata(s00, s01);
				s1 = vis_faligndata(s10, s11);
				s2 = vis_faligndata(s20, s21);
				s3 = vis_faligndata(s30, s31);

				d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
				d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
				d11 = vis_fmul8x16au(vis_read_lo(s1), k1);
				d20 = vis_fmul8x16au(vis_read_hi(s2), k2);
				d21 = vis_fmul8x16au(vis_read_lo(s2), k2);
				d30 = vis_fmul8x16au(vis_read_hi(s3), k3);
				d31 = vis_fmul8x16au(vis_read_lo(s3), k3);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d0 = vis_fpadd16(d0, d20);
				d0 = vis_fpadd16(d0, d30);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);
				d1 = vis_fpadd16(d1, d21);
				d1 = vis_fpadd16(d1, d31);

				dd = vis_fpack16_pair(d0, d1);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				s00 = s01;
				s10 = s11;
				s20 = s21;
				s30 = s31;
				s01 = buff0[i + 1];
				s11 = buff1[i + 1];
				s21 = buff2[i + 1];
				s31 = buff3[i + 1];
				s0 = vis_faligndata(s00, s01);
				s1 = vis_faligndata(s10, s11);
				s2 = vis_faligndata(s20, s21);
				s3 = vis_faligndata(s30, s31);

				d00 = vis_fmul8x16au(vis_read_hi(s0), k0);
				d01 = vis_fmul8x16au(vis_read_lo(s0), k0);
				d10 = vis_fmul8x16au(vis_read_hi(s1), k1);
				d11 = vis_fmul8x16au(vis_read_lo(s1), k1);
				d20 = vis_fmul8x16au(vis_read_hi(s2), k2);
				d21 = vis_fmul8x16au(vis_read_lo(s2), k2);
				d30 = vis_fmul8x16au(vis_read_hi(s3), k3);
				d31 = vis_fmul8x16au(vis_read_lo(s3), k3);

				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];
				d0 = vis_fpadd16(d0, d00);
				d0 = vis_fpadd16(d0, d10);
				d0 = vis_fpadd16(d0, d20);
				d0 = vis_fpadd16(d0, d30);
				d1 = vis_fpadd16(d1, d01);
				d1 = vis_fpadd16(d1, d11);
				d1 = vis_fpadd16(d1, d21);
				d1 = vis_fpadd16(d1, d31);

				dd = vis_fpack16_pair(d0, d1);
				vis_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}
		}

		if ((mlib_u8 *)dp != dl)
			fun_store((mlib_u8 *)buffe, dl, wid, ncmask);

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
