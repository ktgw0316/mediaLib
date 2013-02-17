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

#pragma ident	"@(#)mlib_v_ImageConv7x7_8ext.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv7x7_8nw.c - convolve an 8-bit image with a 7x7 kernel
 *                      (edge = src extended)
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_conv7x7ext_u8(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         dx_l,
 *                                     mlib_s32         dx_r,
 *                                     mlib_s32         dy_t,
 *                                     mlib_s32         dy_b,
 *                                     const mlib_s32   *kernel,
 *                                     mlib_s32         scalef_expon,
 *                                     mlib_s32         cmask)
 * ARGUMENT
 *      src           Pointer to structure of source image
 *      dst           Pointer to structure of destination image
 *      kernel        Convolution kernel
 *      scalef_expons The scaling factor to convert the input integer
 *                    coefficients into floating-point coefficients:
 *                    floating-point coeff. = integer coeff. * 2^(-scale)
 *      cmask         Channel mask to indicate the channels to be convolved.
 *                    Each bit of which represents a channel in the image. The
 *                    channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      A 2-D convolution (7x7 kernel) for 8-bit images.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_v_ImageChannelInsert.h>

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

static void
mlib_my_store(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 size,
    mlib_s32 cmask)
{
	mlib_ImageCopy_na(src, dst, size);
}

/* *********************************************************** */

static const mlib_s32 mlib_round_8[16] = {
	0x00400040, 0x00200020, 0x00100010, 0x00080008,
	0x00040004, 0x00020002, 0x00010001, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* *********************************************************** */

#define	LEFT	3
#define	RIGHT	3

#define	TOP	3

#define	BOTTOM	3

/* *********************************************************** */

#define	GET_ALIGN_DATA(off)                                     \
	vis_alignaddr(0, off);                                  \
	curr = *s1;                                             \
	d1 = *(s1 + 1);                                         \
	d21 = vis_faligndata(curr, d1);                         \
	curr = *s2;                                             \
	d2 = *(s2 + 1);                                         \
	d22 = vis_faligndata(curr, d2);                         \
	curr = *s3;                                             \
	d3 = *(s3 + 1);                                         \
	d23 = vis_faligndata(curr, d3);                         \
	curr = *s4;                                             \
	d4 = *(s4 + 1);                                         \
	d24 = vis_faligndata(curr, d4);                         \
	curr = *s5;                                             \
	d5 = *(s5 + 1);                                         \
	d25 = vis_faligndata(curr, d5);                         \
	curr = *s6;                                             \
	d6 = *(s6 + 1);                                         \
	d26 = vis_faligndata(curr, d6);                         \
	curr = *s7;                                             \
	d7 = *(s7 + 1);                                         \
	d27 = vis_faligndata(curr, d7)

/* *********************************************************** */

#define	CONV_U8_1(str, prevval, dsrc, ka)                       \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc), ka);           \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc), ka);           \
	out0 = vis_fpadd16(out0, tmp0);                         \
	out1 = vis_fpadd16(out1, tmp1);                         \
	curr = *(str + 2);                                      \
	dsrc = vis_faligndata(prevval, curr);                   \
	prevval = curr

/* *********************************************************** */

#define	CONV_U8_2(str, prevval, dsrc, ka)                       \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc), ka);           \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc), ka);           \
	out0 = vis_fpadd16(out0, tmp0);                         \
	out1 = vis_fpadd16(out1, tmp1);                         \
	tmp0 = vis_fmul8x16al(vis_read_lo(dsrc), ka);           \
	curr = *(str + 2);                                      \
	dsrc = vis_faligndata(prevval, curr);                   \
	tmp1 = vis_fmul8x16al(vis_read_hi(dsrc), ka);           \
	out0 = vis_fpadd16(out0, tmp0);                         \
	out1 = vis_fpadd16(out1, tmp1);                         \
	prevval = curr

/* *********************************************************** */

mlib_status
mlib_conv7x7ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4, *sa5, *sa7, *prow;

/* pointers to rows in interm. src buf */
	mlib_u8 *buff_src, *sbuf1, *sbuf2, *sbuf3, *sbuf4, *sbuf5;
	mlib_u8 *sbuf6, *sbuf7;

/* pointers to row in interm. dst buf */
	mlib_u8 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5, *s6, *s7;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst, *d_dst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, rnd;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, curr;

/* shifted data */
	mlib_d64 d21, d22, d23, d24, d25, d26, d27;
	mlib_f32 k1k5, k2k6, k3k7, k4, k8k12, k9k13, k10k14, k11;
	mlib_f32 k15k19, k16k20, k17k21, k18, k22k26, k23k27, k24k28, k25;
	mlib_f32 k29k33, k30k34, k31k35, k32, k36k40, k37k41, k38k42, k39;
	mlib_f32 k43k47, k44k48, k45k49, k46;
	mlib_f32 p1, p2, p3, p4, p5, p6, p7;
	mlib_s32 rval, gsr_scale, i, j, nchannel, chan, testchan;
	void (
	    *p_proc_load) (
	    const mlib_u8 *,
	    mlib_u8 *,
	    mlib_s32,
	    mlib_s32);
	void (
	    *p_proc_store) (
	    const mlib_u8 *,
	    mlib_u8 *,
	    mlib_s32,
	    mlib_s32);

	if (mlib_ImageConvVersion(7, 7, scalef_expon, MLIB_BYTE) == 0)
		return mlib_c_conv7x7ext_u8(dst, src, dx_l, dx_r, dy_t, dy_b,
		    kernel, scalef_expon, cmask);

	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);
	nchannel = mlib_ImageGetChannels(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	dh = mlib_ImageGetHeight(dst);
	dw = mlib_ImageGetWidth(dst);

/* buf_slb - 8-byte aligned */
	buf_slb = ((dw + 6) + 24) & (~7);
/* alloc. interm. src and dst buffer */
	buff_src = (mlib_u8 *)__mlib_malloc(9 * buf_slb * sizeof (mlib_u8) + 8);

	if (buff_src == NULL)
		return (MLIB_FAILURE);

/*
 *  The 8x16 mult has built-in 8-bit R shift, and fpack16 has 7-bit
 *  fixed R shift (preceded by variable-bit L shift controlled by GSR
 *  scalefactor field). Thus net R shift = (8+7)-(GSR.scalefactor_field),
 *  so GSR.scalefactor_field = 15-(net R shift):
 */
	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3) + 1);
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));
	sbuf1 = (mlib_u8 *)((mlib_addr)(buff_src + 8) & (~7));
	sbuf2 = sbuf1 + buf_slb;
	sbuf3 = sbuf2 + buf_slb;
	sbuf4 = sbuf3 + buf_slb;
	sbuf5 = sbuf4 + buf_slb;
	sbuf6 = sbuf5 + buf_slb;
	sbuf7 = sbuf6 + buf_slb;
	dbuf = sbuf7 + buf_slb;

/* load kernel values into floats */

	k1k5 =
	    vis_to_float((kernel[0] & 0xFFFF0000) | ((kernel[4] >> 16) &
	    0xFFFF));
	k2k6 =
	    vis_to_float((kernel[1] & 0xFFFF0000) | ((kernel[5] >> 16) &
	    0xFFFF));
	k3k7 =
	    vis_to_float((kernel[2] & 0xFFFF0000) | ((kernel[6] >> 16) &
	    0xFFFF));
	k4 = vis_to_float((kernel[3] & 0xFFFF0000) | ((kernel[3] >> 16) &
	    0xFFFF));

	k8k12 =
	    vis_to_float((kernel[7] & 0xFFFF0000) | ((kernel[11] >> 16) &
	    0xFFFF));
	k9k13 =
	    vis_to_float((kernel[8] & 0xFFFF0000) | ((kernel[12] >> 16) &
	    0xFFFF));
	k10k14 =
	    vis_to_float((kernel[9] & 0xFFFF0000) | ((kernel[13] >> 16) &
	    0xFFFF));
	k11 =
	    vis_to_float((kernel[10] & 0xFFFF0000) | ((kernel[10] >> 16) &
	    0xFFFF));

	k15k19 =
	    vis_to_float((kernel[14] & 0xFFFF0000) | ((kernel[18] >> 16) &
	    0xFFFF));
	k16k20 =
	    vis_to_float((kernel[15] & 0xFFFF0000) | ((kernel[19] >> 16) &
	    0xFFFF));
	k17k21 =
	    vis_to_float((kernel[16] & 0xFFFF0000) | ((kernel[20] >> 16) &
	    0xFFFF));
	k18 =
	    vis_to_float((kernel[17] & 0xFFFF0000) | ((kernel[17] >> 16) &
	    0xFFFF));

	k22k26 =
	    vis_to_float((kernel[21] & 0xFFFF0000) | ((kernel[25] >> 16) &
	    0xFFFF));
	k23k27 =
	    vis_to_float((kernel[22] & 0xFFFF0000) | ((kernel[26] >> 16) &
	    0xFFFF));
	k24k28 =
	    vis_to_float((kernel[23] & 0xFFFF0000) | ((kernel[27] >> 16) &
	    0xFFFF));
	k25 =
	    vis_to_float((kernel[24] & 0xFFFF0000) | ((kernel[24] >> 16) &
	    0xFFFF));

	k29k33 =
	    vis_to_float((kernel[28] & 0xFFFF0000) | ((kernel[32] >> 16) &
	    0xFFFF));
	k30k34 =
	    vis_to_float((kernel[29] & 0xFFFF0000) | ((kernel[33] >> 16) &
	    0xFFFF));
	k31k35 =
	    vis_to_float((kernel[30] & 0xFFFF0000) | ((kernel[34] >> 16) &
	    0xFFFF));
	k32 =
	    vis_to_float((kernel[31] & 0xFFFF0000) | ((kernel[31] >> 16) &
	    0xFFFF));

	k36k40 =
	    vis_to_float((kernel[35] & 0xFFFF0000) | ((kernel[39] >> 16) &
	    0xFFFF));
	k37k41 =
	    vis_to_float((kernel[36] & 0xFFFF0000) | ((kernel[40] >> 16) &
	    0xFFFF));
	k38k42 =
	    vis_to_float((kernel[37] & 0xFFFF0000) | ((kernel[41] >> 16) &
	    0xFFFF));
	k39 =
	    vis_to_float((kernel[38] & 0xFFFF0000) | ((kernel[38] >> 16) &
	    0xFFFF));

	k43k47 =
	    vis_to_float((kernel[42] & 0xFFFF0000) | ((kernel[46] >> 16) &
	    0xFFFF));
	k44k48 =
	    vis_to_float((kernel[43] & 0xFFFF0000) | ((kernel[47] >> 16) &
	    0xFFFF));
	k45k49 =
	    vis_to_float((kernel[44] & 0xFFFF0000) | ((kernel[48] >> 16) &
	    0xFFFF));
	k46 =
	    vis_to_float((kernel[45] & 0xFFFF0000) | ((kernel[45] >> 16) &
	    0xFFFF));

	i = 1;

	if ((i > dy_t) && (i < dh + TOP + BOTTOM - dy_b))
		sa1 = sa + slb;
	else
		sa1 = sa;
	i++;

	if ((i > dy_t) && (i < dh + TOP + BOTTOM - dy_b))
		sa2 = sa1 + slb;
	else
		sa2 = sa1;
	i++;

	if ((i > dy_t) && (i < dh + TOP + BOTTOM - dy_b))
		sa3 = sa2 + slb;
	else
		sa3 = sa2;
	i++;

	if ((i > dy_t) && (i < dh + TOP + BOTTOM - dy_b))
		sa4 = sa3 + slb;
	else
		sa4 = sa3;
	i++;

	if ((i > dy_t) && (i < dh + TOP + BOTTOM - dy_b))
		sa5 = sa4 + slb;
	else
		sa5 = sa4;

	if (nchannel == 1) {
		p_proc_load = &mlib_my_load;
		p_proc_store = &mlib_my_store;
	} else if (nchannel == 2) {
		p_proc_load = &mlib_v_ImageChannelExtract_U8_21_D1;
		p_proc_store = &mlib_v_ImageChannelInsert_U8_12_D1;
	} else if (nchannel == 3) {
		p_proc_load = &mlib_v_ImageChannelExtract_U8_31_D1;
		p_proc_store = &mlib_v_ImageChannelInsert_U8_13_D1;
	} else {
		p_proc_load = &mlib_v_ImageChannelExtract_U8_41_D1;
		p_proc_store = &mlib_v_ImageChannelInsert_U8_14_D1;
	}

	testchan = 1;
	for (chan = 0; chan < nchannel; chan++) {
		if ((cmask & testchan) == 0) {
			testchan <<= 1;
			continue;
		}

		if ((6 > dy_t) && (6 < dh + TOP + BOTTOM - dy_b))
			sa7 = sa5 + slb;
		else
			sa7 = sa5;
		d_a = da;

/* load interm. src buff */

		(*p_proc_load) (sa, (sbuf2 + dx_l),
		    dw + LEFT + RIGHT - dx_l - dx_r, testchan);
		(*p_proc_load) (sa1, (sbuf3 + dx_l),
		    dw + LEFT + RIGHT - dx_l - dx_r, testchan);
		(*p_proc_load) (sa2, (sbuf4 + dx_l),
		    dw + LEFT + RIGHT - dx_l - dx_r, testchan);
		(*p_proc_load) (sa3, (sbuf5 + dx_l),
		    dw + LEFT + RIGHT - dx_l - dx_r, testchan);
		(*p_proc_load) (sa4, (sbuf6 + dx_l),
		    dw + LEFT + RIGHT - dx_l - dx_r, testchan);
		(*p_proc_load) (sa5, (sbuf7 + dx_l),
		    dw + LEFT + RIGHT - dx_l - dx_r, testchan);

		for (i = 0; i < dx_l; i++) {
			sbuf2[i] = sbuf2[dx_l];
			sbuf3[i] = sbuf3[dx_l];
			sbuf4[i] = sbuf4[dx_l];
			sbuf5[i] = sbuf5[dx_l];
			sbuf6[i] = sbuf6[dx_l];
			sbuf7[i] = sbuf7[dx_l];
		}

		for (i = 0; i < dx_r; i++) {
			sbuf2[dw + LEFT + RIGHT - dx_r + i] =
			    sbuf2[dw + LEFT + RIGHT - dx_r - 1];
			sbuf3[dw + LEFT + RIGHT - dx_r + i] =
			    sbuf3[dw + LEFT + RIGHT - dx_r - 1];
			sbuf4[dw + LEFT + RIGHT - dx_r + i] =
			    sbuf4[dw + LEFT + RIGHT - dx_r - 1];
			sbuf5[dw + LEFT + RIGHT - dx_r + i] =
			    sbuf5[dw + LEFT + RIGHT - dx_r - 1];
			sbuf6[dw + LEFT + RIGHT - dx_r + i] =
			    sbuf6[dw + LEFT + RIGHT - dx_r - 1];
			sbuf7[dw + LEFT + RIGHT - dx_r + i] =
			    sbuf7[dw + LEFT + RIGHT - dx_r - 1];
		}

		for (j = 0; j < dh; j++) {
/* load next row */
			(*p_proc_load) (sa7, (sbuf1 + dx_l),
			    dw + LEFT + RIGHT - dx_l - dx_r, testchan);
			for (i = 0; i < dx_l; i++) {
				sbuf1[i] = sbuf1[dx_l];
			}

			for (i = 0; i < dx_r; i++) {
				sbuf1[dw + LEFT + RIGHT - dx_r + i] =
				    sbuf1[dw + LEFT + RIGHT - dx_r - 1];
			}

			ddst = (mlib_d64 *)(dbuf);
			prow = sbuf1;
			sbuf1 = sbuf2;
			sbuf2 = sbuf3;
			sbuf3 = sbuf4;
			sbuf4 = sbuf5;
			sbuf5 = sbuf6;
			sbuf6 = sbuf7;
			sbuf7 = prow;
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			s3 = (mlib_d64 *)sbuf3;
			s4 = (mlib_d64 *)sbuf4;
			s5 = (mlib_d64 *)sbuf5;
			s6 = (mlib_d64 *)sbuf6;
			s7 = (mlib_d64 *)sbuf7;

			GET_ALIGN_DATA(0);
			p1 = k1k5;
			p2 = k8k12;
			p3 = k15k19;
			p4 = k22k26;
			p5 = k29k33;
			p6 = k36k40;
			p7 = k43k47;
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				out0 = rnd;
				out1 = rnd;
				CONV_U8_2(s1, d1, d21, p1);
				CONV_U8_2(s2, d2, d22, p2);
				CONV_U8_2(s3, d3, d23, p3);
				CONV_U8_2(s4, d4, d24, p4);
				CONV_U8_2(s5, d5, d25, p5);
				CONV_U8_2(s6, d6, d26, p6);
				CONV_U8_2(s7, d7, d27, p7);
				ddst[0] = out0;
				ddst[1] = out1;
				ddst += 2;
				s1++;
				s2++;
				s3++;
				s4++;
				s5++;
				s6++;
				s7++;
			}

			ddst = (mlib_d64 *)(dbuf);
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			s3 = (mlib_d64 *)sbuf3;
			s4 = (mlib_d64 *)sbuf4;
			s5 = (mlib_d64 *)sbuf5;
			s6 = (mlib_d64 *)sbuf6;
			s7 = (mlib_d64 *)sbuf7;
			GET_ALIGN_DATA(1);
			p1 = k2k6;
			p2 = k9k13;
			p3 = k16k20;
			p4 = k23k27;
			p5 = k30k34;
			p6 = k37k41;
			p7 = k44k48;
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				out0 = ddst[0];
				out1 = ddst[1];
				CONV_U8_2(s1, d1, d21, p1);
				CONV_U8_2(s2, d2, d22, p2);
				CONV_U8_2(s3, d3, d23, p3);
				CONV_U8_2(s4, d4, d24, p4);
				CONV_U8_2(s5, d5, d25, p5);
				CONV_U8_2(s6, d6, d26, p6);
				CONV_U8_2(s7, d7, d27, p7);
				ddst[0] = out0;
				ddst[1] = out1;
				ddst += 2;
				s1++;
				s2++;
				s3++;
				s4++;
				s5++;
				s6++;
				s7++;
			}

			ddst = (mlib_d64 *)(dbuf);
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			s3 = (mlib_d64 *)sbuf3;
			s4 = (mlib_d64 *)sbuf4;
			s5 = (mlib_d64 *)sbuf5;
			s6 = (mlib_d64 *)sbuf6;
			s7 = (mlib_d64 *)sbuf7;
			GET_ALIGN_DATA(2);
			p1 = k3k7;
			p2 = k10k14;
			p3 = k17k21;
			p4 = k24k28;
			p5 = k31k35;
			p6 = k38k42;
			p7 = k45k49;
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				out0 = ddst[0];
				out1 = ddst[1];
				CONV_U8_2(s1, d1, d21, p1);
				CONV_U8_2(s2, d2, d22, p2);
				CONV_U8_2(s3, d3, d23, p3);
				CONV_U8_2(s4, d4, d24, p4);
				CONV_U8_2(s5, d5, d25, p5);
				CONV_U8_2(s6, d6, d26, p6);
				CONV_U8_2(s7, d7, d27, p7);
				ddst[0] = out0;
				ddst[1] = out1;
				ddst += 2;
				s1++;
				s2++;
				s3++;
				s4++;
				s5++;
				s6++;
				s7++;
			}

			ddst = (mlib_d64 *)(dbuf);
			d_dst = ddst;
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			s3 = (mlib_d64 *)sbuf3;
			s4 = (mlib_d64 *)sbuf4;
			s5 = (mlib_d64 *)sbuf5;
			s6 = (mlib_d64 *)sbuf6;
			s7 = (mlib_d64 *)sbuf7;
			GET_ALIGN_DATA(3);
			p1 = k4;
			p2 = k11;
			p3 = k18;
			p4 = k25;
			p5 = k32;
			p6 = k39;
			p7 = k46;
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				out0 = ddst[0];
				out1 = ddst[1];
				CONV_U8_1(s1, d1, d21, p1);
				CONV_U8_1(s2, d2, d22, p2);
				CONV_U8_1(s3, d3, d23, p3);
				CONV_U8_1(s4, d4, d24, p4);
				CONV_U8_1(s5, d5, d25, p5);
				CONV_U8_1(s6, d6, d26, p6);
				CONV_U8_1(s7, d7, d27, p7);
				(*d_dst++) =
				    vis_freg_pair(vis_fpack16(out0),
				    vis_fpack16(out1));
				ddst += 2;
				s1++;
				s2++;
				s3++;
				s4++;
				s5++;
				s6++;
				s7++;
			}

/* store result */
			(*p_proc_store) (dbuf, d_a, dw, testchan);

			if (j < dh - 1 - dy_b)
				sa7 = sa7 + slb;
			d_a += dlb;
		}

		testchan <<= 1;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
